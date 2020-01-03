/***************************************************************************//**
* \file Master.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Master_PVT.h"

#if (Master_SCB_MODE_I2C_INC)
    #include "Master_I2C_PVT.h"
#endif /* (Master_SCB_MODE_I2C_INC) */

#if (Master_SCB_MODE_EZI2C_INC)
    #include "Master_EZI2C_PVT.h"
#endif /* (Master_SCB_MODE_EZI2C_INC) */

#if (Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC)
    #include "Master_SPI_UART_PVT.h"
#endif /* (Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 Master_scbMode = Master_SCB_MODE_UNCONFIG;
    uint8 Master_scbEnableWake;
    uint8 Master_scbEnableIntr;

    /* I2C configuration variables */
    uint8 Master_mode;
    uint8 Master_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * Master_rxBuffer;
    uint8  Master_rxDataBits;
    uint32 Master_rxBufferSize;

    volatile uint8 * Master_txBuffer;
    uint8  Master_txDataBits;
    uint32 Master_txBufferSize;

    /* EZI2C configuration variables */
    uint8 Master_numberOfAddr;
    uint8 Master_subAddrSize;
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** Master_initVar indicates whether the Master 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the Master_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  Master_Init() function can be called before the 
*  Master_Start() or Master_Enable() function.
*/
uint8 Master_initVar = 0u;


#if (! (Master_SCB_MODE_I2C_CONST_CFG || \
        Master_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * Master_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent Master_Enable() call.
    */
    uint16 Master_IntrTxMask = 0u;
#endif /* (! (Master_SCB_MODE_I2C_CONST_CFG || \
              Master_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (Master_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER)
    void (*Master_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER) */
#endif /* (Master_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void Master_ScbEnableIntr(void);
static void Master_ScbModeStop(void);
static void Master_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: Master_Init
****************************************************************************//**
*
*  Initializes the Master component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  Master_I2CInit, Master_SpiInit, 
*  Master_UartInit or Master_EzI2CInit.
*
*******************************************************************************/
void Master_Init(void)
{
#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Master_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Master_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (Master_SCB_MODE_I2C_CONST_CFG)
    Master_I2CInit();

#elif (Master_SCB_MODE_SPI_CONST_CFG)
    Master_SpiInit();

#elif (Master_SCB_MODE_UART_CONST_CFG)
    Master_UartInit();

#elif (Master_SCB_MODE_EZI2C_CONST_CFG)
    Master_EzI2CInit();

#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Master_Enable
****************************************************************************//**
*
*  Enables Master component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  Master_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The Master configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured Master”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void Master_Enable(void)
{
#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!Master_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Master_CTRL_REG |= Master_CTRL_ENABLED;

        Master_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        Master_ScbModePostEnable();
    }
#else
    Master_CTRL_REG |= Master_CTRL_ENABLED;

    Master_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    Master_ScbModePostEnable();
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Master_Start
****************************************************************************//**
*
*  Invokes Master_Init() and Master_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  Master_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void Master_Start(void)
{
    if (0u == Master_initVar)
    {
        Master_Init();
        Master_initVar = 1u; /* Component was initialized */
    }

    Master_Enable();
}


/*******************************************************************************
* Function Name: Master_Stop
****************************************************************************//**
*
*  Disables the Master component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function Master_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void Master_Stop(void)
{
#if (Master_SCB_IRQ_INTERNAL)
    Master_DisableInt();
#endif /* (Master_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    Master_ScbModeStop();

    /* Disable SCB IP */
    Master_CTRL_REG &= (uint32) ~Master_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    Master_SetTxInterruptMode(Master_NO_INTR_SOURCES);

#if (Master_SCB_IRQ_INTERNAL)
    Master_ClearPendingInt();
#endif /* (Master_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Master_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void Master_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = Master_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~Master_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (Master_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Master_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: Master_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void Master_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = Master_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~Master_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (Master_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Master_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (Master_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: Master_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void Master_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER)
        Master_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER) */
    }
#endif /* (Master_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: Master_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void Master_ScbEnableIntr(void)
{
#if (Master_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != Master_scbEnableIntr)
        {
            Master_EnableInt();
        }

    #else
        Master_EnableInt();

    #endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (Master_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Master_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void Master_ScbModePostEnable(void)
{
#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!Master_CY_SCBIP_V1)
    if (Master_SCB_MODE_SPI_RUNTM_CFG)
    {
        Master_SpiPostEnable();
    }
    else if (Master_SCB_MODE_UART_RUNTM_CFG)
    {
        Master_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!Master_CY_SCBIP_V1) */

#elif (Master_SCB_MODE_SPI_CONST_CFG)
    Master_SpiPostEnable();

#elif (Master_SCB_MODE_UART_CONST_CFG)
    Master_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Master_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void Master_ScbModeStop(void)
{
#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Master_SCB_MODE_I2C_RUNTM_CFG)
    {
        Master_I2CStop();
    }
    else if (Master_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Master_EzI2CStop();
    }
#if (!Master_CY_SCBIP_V1)
    else if (Master_SCB_MODE_SPI_RUNTM_CFG)
    {
        Master_SpiStop();
    }
    else if (Master_SCB_MODE_UART_RUNTM_CFG)
    {
        Master_UartStop();
    }
#endif /* (!Master_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (Master_SCB_MODE_I2C_CONST_CFG)
    Master_I2CStop();

#elif (Master_SCB_MODE_EZI2C_CONST_CFG)
    Master_EzI2CStop();

#elif (Master_SCB_MODE_SPI_CONST_CFG)
    Master_SpiStop();

#elif (Master_SCB_MODE_UART_CONST_CFG)
    Master_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: Master_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void Master_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[Master_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!Master_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!Master_CY_SCBIP_V1) */
        
        uint32 hsiomSel[Master_SCB_PINS_NUMBER] = 
        {
            Master_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            Master_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (Master_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (Master_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < Master_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = Master_PIN_DM_ALG_HIZ;
        }

        if ((Master_SCB_MODE_I2C   == mode) ||
            (Master_SCB_MODE_EZI2C == mode))
        {
        #if (Master_RX_SCL_MOSI_PIN)
            hsiomSel[Master_RX_SCL_MOSI_PIN_INDEX] = Master_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Master_RX_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_OD_LO;
        #elif (Master_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[Master_RX_WAKE_SCL_MOSI_PIN_INDEX] = Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Master_RX_WAKE_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_OD_LO;
        #else
        #endif /* (Master_RX_SCL_MOSI_PIN) */
        
        #if (Master_TX_SDA_MISO_PIN)
            hsiomSel[Master_TX_SDA_MISO_PIN_INDEX] = Master_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [Master_TX_SDA_MISO_PIN_INDEX] = Master_PIN_DM_OD_LO;
        #endif /* (Master_TX_SDA_MISO_PIN) */
        }
    #if (!Master_CY_SCBIP_V1)
        else if (Master_SCB_MODE_SPI == mode)
        {
        #if (Master_RX_SCL_MOSI_PIN)
            hsiomSel[Master_RX_SCL_MOSI_PIN_INDEX] = Master_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (Master_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[Master_RX_WAKE_SCL_MOSI_PIN_INDEX] = Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (Master_RX_SCL_MOSI_PIN) */
        
        #if (Master_TX_SDA_MISO_PIN)
            hsiomSel[Master_TX_SDA_MISO_PIN_INDEX] = Master_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (Master_TX_SDA_MISO_PIN) */
        
        #if (Master_CTS_SCLK_PIN)
            hsiomSel[Master_CTS_SCLK_PIN_INDEX] = Master_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (Master_CTS_SCLK_PIN) */

            if (Master_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[Master_RX_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
                pinsDm[Master_TX_SDA_MISO_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsDm[Master_CTS_SCLK_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;

            #if (Master_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[Master_RTS_SS0_PIN_INDEX] = Master_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [Master_RTS_SS0_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
            #endif /* (Master_RTS_SS0_PIN) */

            #if (Master_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= Master_TX_SDA_MISO_PIN_MASK;
            #endif /* (Master_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[Master_RX_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsDm[Master_TX_SDA_MISO_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
                pinsDm[Master_CTS_SCLK_PIN_INDEX] = Master_PIN_DM_STRONG;

            #if (Master_RTS_SS0_PIN)
                hsiomSel [Master_RTS_SS0_PIN_INDEX] = Master_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [Master_RTS_SS0_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsInBuf |= Master_RTS_SS0_PIN_MASK;
            #endif /* (Master_RTS_SS0_PIN) */

            #if (Master_SS1_PIN)
                hsiomSel [Master_SS1_PIN_INDEX] = Master_SS1_HSIOM_SEL_SPI;
                pinsDm   [Master_SS1_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsInBuf |= Master_SS1_PIN_MASK;
            #endif /* (Master_SS1_PIN) */

            #if (Master_SS2_PIN)
                hsiomSel [Master_SS2_PIN_INDEX] = Master_SS2_HSIOM_SEL_SPI;
                pinsDm   [Master_SS2_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsInBuf |= Master_SS2_PIN_MASK;
            #endif /* (Master_SS2_PIN) */

            #if (Master_SS3_PIN)
                hsiomSel [Master_SS3_PIN_INDEX] = Master_SS3_HSIOM_SEL_SPI;
                pinsDm   [Master_SS3_PIN_INDEX] = Master_PIN_DM_STRONG;
                pinsInBuf |= Master_SS3_PIN_MASK;
            #endif /* (Master_SS3_PIN) */

                /* Disable input buffers */
            #if (Master_RX_SCL_MOSI_PIN)
                pinsInBuf |= Master_RX_SCL_MOSI_PIN_MASK;
            #elif (Master_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= Master_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (Master_RX_SCL_MOSI_PIN) */

            #if (Master_CTS_SCLK_PIN)
                pinsInBuf |= Master_CTS_SCLK_PIN_MASK;
            #endif /* (Master_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (Master_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (Master_TX_SDA_MISO_PIN)
                hsiomSel[Master_TX_SDA_MISO_PIN_INDEX] = Master_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [Master_TX_SDA_MISO_PIN_INDEX] = Master_PIN_DM_OD_LO;
            #endif /* (Master_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (Master_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (Master_RX_SCL_MOSI_PIN)
                    hsiomSel[Master_RX_SCL_MOSI_PIN_INDEX] = Master_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Master_RX_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
                #elif (Master_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[Master_RX_WAKE_SCL_MOSI_PIN_INDEX] = Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Master_RX_WAKE_SCL_MOSI_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
                #else
                #endif /* (Master_RX_SCL_MOSI_PIN) */
                }

                if (0u != (Master_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (Master_TX_SDA_MISO_PIN)
                    hsiomSel[Master_TX_SDA_MISO_PIN_INDEX] = Master_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [Master_TX_SDA_MISO_PIN_INDEX] = Master_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= Master_TX_SDA_MISO_PIN_MASK;
                #endif /* (Master_TX_SDA_MISO_PIN) */
                }

            #if !(Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1)
                if (Master_UART_MODE_STD == subMode)
                {
                    if (0u != (Master_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (Master_CTS_SCLK_PIN)
                        hsiomSel[Master_CTS_SCLK_PIN_INDEX] = Master_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [Master_CTS_SCLK_PIN_INDEX] = Master_PIN_DM_DIG_HIZ;
                    #endif /* (Master_CTS_SCLK_PIN) */
                    }

                    if (0u != (Master_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (Master_RTS_SS0_PIN)
                        hsiomSel[Master_RTS_SS0_PIN_INDEX] = Master_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [Master_RTS_SS0_PIN_INDEX] = Master_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= Master_RTS_SS0_PIN_MASK;
                    #endif /* (Master_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1) */
            }
        }
    #endif /* (!Master_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (Master_RX_SCL_MOSI_PIN)
        Master_SET_HSIOM_SEL(Master_RX_SCL_MOSI_HSIOM_REG,
                                       Master_RX_SCL_MOSI_HSIOM_MASK,
                                       Master_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[Master_RX_SCL_MOSI_PIN_INDEX]);

        Master_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[Master_RX_SCL_MOSI_PIN_INDEX]);

        #if (!Master_CY_SCBIP_V1)
            Master_SET_INP_DIS(Master_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         Master_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & Master_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!Master_CY_SCBIP_V1) */
    
    #elif (Master_RX_WAKE_SCL_MOSI_PIN)
        Master_SET_HSIOM_SEL(Master_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       Master_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       Master_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[Master_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        Master_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[Master_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        Master_SET_INP_DIS(Master_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     Master_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & Master_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        Master_SET_INCFG_TYPE(Master_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        Master_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        Master_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        Master_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (Master_RX_WAKE_SCL_MOSI_PIN) */

    #if (Master_TX_SDA_MISO_PIN)
        Master_SET_HSIOM_SEL(Master_TX_SDA_MISO_HSIOM_REG,
                                       Master_TX_SDA_MISO_HSIOM_MASK,
                                       Master_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[Master_TX_SDA_MISO_PIN_INDEX]);

        Master_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[Master_TX_SDA_MISO_PIN_INDEX]);

    #if (!Master_CY_SCBIP_V1)
        Master_SET_INP_DIS(Master_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     Master_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & Master_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!Master_CY_SCBIP_V1) */
    #endif /* (Master_RX_SCL_MOSI_PIN) */

    #if (Master_CTS_SCLK_PIN)
        Master_SET_HSIOM_SEL(Master_CTS_SCLK_HSIOM_REG,
                                       Master_CTS_SCLK_HSIOM_MASK,
                                       Master_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[Master_CTS_SCLK_PIN_INDEX]);

        Master_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[Master_CTS_SCLK_PIN_INDEX]);

        Master_SET_INP_DIS(Master_uart_cts_spi_sclk_INP_DIS,
                                     Master_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & Master_CTS_SCLK_PIN_MASK)));
    #endif /* (Master_CTS_SCLK_PIN) */

    #if (Master_RTS_SS0_PIN)
        Master_SET_HSIOM_SEL(Master_RTS_SS0_HSIOM_REG,
                                       Master_RTS_SS0_HSIOM_MASK,
                                       Master_RTS_SS0_HSIOM_POS,
                                       hsiomSel[Master_RTS_SS0_PIN_INDEX]);

        Master_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[Master_RTS_SS0_PIN_INDEX]);

        Master_SET_INP_DIS(Master_uart_rts_spi_ss0_INP_DIS,
                                     Master_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & Master_RTS_SS0_PIN_MASK)));
    #endif /* (Master_RTS_SS0_PIN) */

    #if (Master_SS1_PIN)
        Master_SET_HSIOM_SEL(Master_SS1_HSIOM_REG,
                                       Master_SS1_HSIOM_MASK,
                                       Master_SS1_HSIOM_POS,
                                       hsiomSel[Master_SS1_PIN_INDEX]);

        Master_spi_ss1_SetDriveMode((uint8) pinsDm[Master_SS1_PIN_INDEX]);

        Master_SET_INP_DIS(Master_spi_ss1_INP_DIS,
                                     Master_spi_ss1_MASK,
                                     (0u != (pinsInBuf & Master_SS1_PIN_MASK)));
    #endif /* (Master_SS1_PIN) */

    #if (Master_SS2_PIN)
        Master_SET_HSIOM_SEL(Master_SS2_HSIOM_REG,
                                       Master_SS2_HSIOM_MASK,
                                       Master_SS2_HSIOM_POS,
                                       hsiomSel[Master_SS2_PIN_INDEX]);

        Master_spi_ss2_SetDriveMode((uint8) pinsDm[Master_SS2_PIN_INDEX]);

        Master_SET_INP_DIS(Master_spi_ss2_INP_DIS,
                                     Master_spi_ss2_MASK,
                                     (0u != (pinsInBuf & Master_SS2_PIN_MASK)));
    #endif /* (Master_SS2_PIN) */

    #if (Master_SS3_PIN)
        Master_SET_HSIOM_SEL(Master_SS3_HSIOM_REG,
                                       Master_SS3_HSIOM_MASK,
                                       Master_SS3_HSIOM_POS,
                                       hsiomSel[Master_SS3_PIN_INDEX]);

        Master_spi_ss3_SetDriveMode((uint8) pinsDm[Master_SS3_PIN_INDEX]);

        Master_SET_INP_DIS(Master_spi_ss3_INP_DIS,
                                     Master_spi_ss3_MASK,
                                     (0u != (pinsInBuf & Master_SS3_PIN_MASK)));
    #endif /* (Master_SS3_PIN) */
    }

#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: Master_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void Master_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (Master_CTRL_REG & Master_CTRL_EC_AM_MODE)) &&
            (0u == (Master_I2C_CTRL_REG & Master_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            Master_CTRL_REG &= ~Master_CTRL_EC_AM_MODE;
            Master_CTRL_REG |=  Master_CTRL_EC_AM_MODE;
        }

        Master_I2C_SLAVE_CMD_REG = Master_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1) */


/* [] END OF FILE */
