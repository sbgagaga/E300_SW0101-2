/***************************************************************************//**
* \file Slave.c
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

#include "Slave_PVT.h"

#if (Slave_SCB_MODE_I2C_INC)
    #include "Slave_I2C_PVT.h"
#endif /* (Slave_SCB_MODE_I2C_INC) */

#if (Slave_SCB_MODE_EZI2C_INC)
    #include "Slave_EZI2C_PVT.h"
#endif /* (Slave_SCB_MODE_EZI2C_INC) */

#if (Slave_SCB_MODE_SPI_INC || Slave_SCB_MODE_UART_INC)
    #include "Slave_SPI_UART_PVT.h"
#endif /* (Slave_SCB_MODE_SPI_INC || Slave_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 Slave_scbMode = Slave_SCB_MODE_UNCONFIG;
    uint8 Slave_scbEnableWake;
    uint8 Slave_scbEnableIntr;

    /* I2C configuration variables */
    uint8 Slave_mode;
    uint8 Slave_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * Slave_rxBuffer;
    uint8  Slave_rxDataBits;
    uint32 Slave_rxBufferSize;

    volatile uint8 * Slave_txBuffer;
    uint8  Slave_txDataBits;
    uint32 Slave_txBufferSize;

    /* EZI2C configuration variables */
    uint8 Slave_numberOfAddr;
    uint8 Slave_subAddrSize;
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** Slave_initVar indicates whether the Slave 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the Slave_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  Slave_Init() function can be called before the 
*  Slave_Start() or Slave_Enable() function.
*/
uint8 Slave_initVar = 0u;


#if (! (Slave_SCB_MODE_I2C_CONST_CFG || \
        Slave_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * Slave_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent Slave_Enable() call.
    */
    uint16 Slave_IntrTxMask = 0u;
#endif /* (! (Slave_SCB_MODE_I2C_CONST_CFG || \
              Slave_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (Slave_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER)
    void (*Slave_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER) */
#endif /* (Slave_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void Slave_ScbEnableIntr(void);
static void Slave_ScbModeStop(void);
static void Slave_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: Slave_Init
****************************************************************************//**
*
*  Initializes the Slave component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  Slave_I2CInit, Slave_SpiInit, 
*  Slave_UartInit or Slave_EzI2CInit.
*
*******************************************************************************/
void Slave_Init(void)
{
#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Slave_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Slave_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (Slave_SCB_MODE_I2C_CONST_CFG)
    Slave_I2CInit();

#elif (Slave_SCB_MODE_SPI_CONST_CFG)
    Slave_SpiInit();

#elif (Slave_SCB_MODE_UART_CONST_CFG)
    Slave_UartInit();

#elif (Slave_SCB_MODE_EZI2C_CONST_CFG)
    Slave_EzI2CInit();

#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Slave_Enable
****************************************************************************//**
*
*  Enables Slave component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  Slave_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The Slave configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured Slave”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void Slave_Enable(void)
{
#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!Slave_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Slave_CTRL_REG |= Slave_CTRL_ENABLED;

        Slave_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        Slave_ScbModePostEnable();
    }
#else
    Slave_CTRL_REG |= Slave_CTRL_ENABLED;

    Slave_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    Slave_ScbModePostEnable();
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Slave_Start
****************************************************************************//**
*
*  Invokes Slave_Init() and Slave_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  Slave_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void Slave_Start(void)
{
    if (0u == Slave_initVar)
    {
        Slave_Init();
        Slave_initVar = 1u; /* Component was initialized */
    }

    Slave_Enable();
}


/*******************************************************************************
* Function Name: Slave_Stop
****************************************************************************//**
*
*  Disables the Slave component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function Slave_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void Slave_Stop(void)
{
#if (Slave_SCB_IRQ_INTERNAL)
    Slave_DisableInt();
#endif /* (Slave_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    Slave_ScbModeStop();

    /* Disable SCB IP */
    Slave_CTRL_REG &= (uint32) ~Slave_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    Slave_SetTxInterruptMode(Slave_NO_INTR_SOURCES);

#if (Slave_SCB_IRQ_INTERNAL)
    Slave_ClearPendingInt();
#endif /* (Slave_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Slave_SetRxFifoLevel
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
void Slave_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = Slave_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~Slave_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (Slave_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Slave_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: Slave_SetTxFifoLevel
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
void Slave_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = Slave_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~Slave_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (Slave_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Slave_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (Slave_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: Slave_SetCustomInterruptHandler
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
    void Slave_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER)
        Slave_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER) */
    }
#endif /* (Slave_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: Slave_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void Slave_ScbEnableIntr(void)
{
#if (Slave_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != Slave_scbEnableIntr)
        {
            Slave_EnableInt();
        }

    #else
        Slave_EnableInt();

    #endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (Slave_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Slave_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void Slave_ScbModePostEnable(void)
{
#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!Slave_CY_SCBIP_V1)
    if (Slave_SCB_MODE_SPI_RUNTM_CFG)
    {
        Slave_SpiPostEnable();
    }
    else if (Slave_SCB_MODE_UART_RUNTM_CFG)
    {
        Slave_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!Slave_CY_SCBIP_V1) */

#elif (Slave_SCB_MODE_SPI_CONST_CFG)
    Slave_SpiPostEnable();

#elif (Slave_SCB_MODE_UART_CONST_CFG)
    Slave_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Slave_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void Slave_ScbModeStop(void)
{
#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Slave_SCB_MODE_I2C_RUNTM_CFG)
    {
        Slave_I2CStop();
    }
    else if (Slave_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Slave_EzI2CStop();
    }
#if (!Slave_CY_SCBIP_V1)
    else if (Slave_SCB_MODE_SPI_RUNTM_CFG)
    {
        Slave_SpiStop();
    }
    else if (Slave_SCB_MODE_UART_RUNTM_CFG)
    {
        Slave_UartStop();
    }
#endif /* (!Slave_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (Slave_SCB_MODE_I2C_CONST_CFG)
    Slave_I2CStop();

#elif (Slave_SCB_MODE_EZI2C_CONST_CFG)
    Slave_EzI2CStop();

#elif (Slave_SCB_MODE_SPI_CONST_CFG)
    Slave_SpiStop();

#elif (Slave_SCB_MODE_UART_CONST_CFG)
    Slave_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: Slave_SetPins
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
    void Slave_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[Slave_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!Slave_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!Slave_CY_SCBIP_V1) */
        
        uint32 hsiomSel[Slave_SCB_PINS_NUMBER] = 
        {
            Slave_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            Slave_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (Slave_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (Slave_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < Slave_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = Slave_PIN_DM_ALG_HIZ;
        }

        if ((Slave_SCB_MODE_I2C   == mode) ||
            (Slave_SCB_MODE_EZI2C == mode))
        {
        #if (Slave_RX_SCL_MOSI_PIN)
            hsiomSel[Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_OD_LO;
        #elif (Slave_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[Slave_RX_WAKE_SCL_MOSI_PIN_INDEX] = Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Slave_RX_WAKE_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_OD_LO;
        #else
        #endif /* (Slave_RX_SCL_MOSI_PIN) */
        
        #if (Slave_TX_SDA_MISO_PIN)
            hsiomSel[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [Slave_TX_SDA_MISO_PIN_INDEX] = Slave_PIN_DM_OD_LO;
        #endif /* (Slave_TX_SDA_MISO_PIN) */
        }
    #if (!Slave_CY_SCBIP_V1)
        else if (Slave_SCB_MODE_SPI == mode)
        {
        #if (Slave_RX_SCL_MOSI_PIN)
            hsiomSel[Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (Slave_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[Slave_RX_WAKE_SCL_MOSI_PIN_INDEX] = Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (Slave_RX_SCL_MOSI_PIN) */
        
        #if (Slave_TX_SDA_MISO_PIN)
            hsiomSel[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (Slave_TX_SDA_MISO_PIN) */
        
        #if (Slave_CTS_SCLK_PIN)
            hsiomSel[Slave_CTS_SCLK_PIN_INDEX] = Slave_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (Slave_CTS_SCLK_PIN) */

            if (Slave_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
                pinsDm[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsDm[Slave_CTS_SCLK_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;

            #if (Slave_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[Slave_RTS_SS0_PIN_INDEX] = Slave_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [Slave_RTS_SS0_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
            #endif /* (Slave_RTS_SS0_PIN) */

            #if (Slave_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= Slave_TX_SDA_MISO_PIN_MASK;
            #endif /* (Slave_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsDm[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
                pinsDm[Slave_CTS_SCLK_PIN_INDEX] = Slave_PIN_DM_STRONG;

            #if (Slave_RTS_SS0_PIN)
                hsiomSel [Slave_RTS_SS0_PIN_INDEX] = Slave_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [Slave_RTS_SS0_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsInBuf |= Slave_RTS_SS0_PIN_MASK;
            #endif /* (Slave_RTS_SS0_PIN) */

            #if (Slave_SS1_PIN)
                hsiomSel [Slave_SS1_PIN_INDEX] = Slave_SS1_HSIOM_SEL_SPI;
                pinsDm   [Slave_SS1_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsInBuf |= Slave_SS1_PIN_MASK;
            #endif /* (Slave_SS1_PIN) */

            #if (Slave_SS2_PIN)
                hsiomSel [Slave_SS2_PIN_INDEX] = Slave_SS2_HSIOM_SEL_SPI;
                pinsDm   [Slave_SS2_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsInBuf |= Slave_SS2_PIN_MASK;
            #endif /* (Slave_SS2_PIN) */

            #if (Slave_SS3_PIN)
                hsiomSel [Slave_SS3_PIN_INDEX] = Slave_SS3_HSIOM_SEL_SPI;
                pinsDm   [Slave_SS3_PIN_INDEX] = Slave_PIN_DM_STRONG;
                pinsInBuf |= Slave_SS3_PIN_MASK;
            #endif /* (Slave_SS3_PIN) */

                /* Disable input buffers */
            #if (Slave_RX_SCL_MOSI_PIN)
                pinsInBuf |= Slave_RX_SCL_MOSI_PIN_MASK;
            #elif (Slave_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= Slave_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (Slave_RX_SCL_MOSI_PIN) */

            #if (Slave_CTS_SCLK_PIN)
                pinsInBuf |= Slave_CTS_SCLK_PIN_MASK;
            #endif /* (Slave_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (Slave_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (Slave_TX_SDA_MISO_PIN)
                hsiomSel[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [Slave_TX_SDA_MISO_PIN_INDEX] = Slave_PIN_DM_OD_LO;
            #endif /* (Slave_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (Slave_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (Slave_RX_SCL_MOSI_PIN)
                    hsiomSel[Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Slave_RX_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
                #elif (Slave_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[Slave_RX_WAKE_SCL_MOSI_PIN_INDEX] = Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Slave_RX_WAKE_SCL_MOSI_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
                #else
                #endif /* (Slave_RX_SCL_MOSI_PIN) */
                }

                if (0u != (Slave_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (Slave_TX_SDA_MISO_PIN)
                    hsiomSel[Slave_TX_SDA_MISO_PIN_INDEX] = Slave_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [Slave_TX_SDA_MISO_PIN_INDEX] = Slave_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= Slave_TX_SDA_MISO_PIN_MASK;
                #endif /* (Slave_TX_SDA_MISO_PIN) */
                }

            #if !(Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1)
                if (Slave_UART_MODE_STD == subMode)
                {
                    if (0u != (Slave_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (Slave_CTS_SCLK_PIN)
                        hsiomSel[Slave_CTS_SCLK_PIN_INDEX] = Slave_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [Slave_CTS_SCLK_PIN_INDEX] = Slave_PIN_DM_DIG_HIZ;
                    #endif /* (Slave_CTS_SCLK_PIN) */
                    }

                    if (0u != (Slave_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (Slave_RTS_SS0_PIN)
                        hsiomSel[Slave_RTS_SS0_PIN_INDEX] = Slave_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [Slave_RTS_SS0_PIN_INDEX] = Slave_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= Slave_RTS_SS0_PIN_MASK;
                    #endif /* (Slave_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1) */
            }
        }
    #endif /* (!Slave_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (Slave_RX_SCL_MOSI_PIN)
        Slave_SET_HSIOM_SEL(Slave_RX_SCL_MOSI_HSIOM_REG,
                                       Slave_RX_SCL_MOSI_HSIOM_MASK,
                                       Slave_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[Slave_RX_SCL_MOSI_PIN_INDEX]);

        Slave_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[Slave_RX_SCL_MOSI_PIN_INDEX]);

        #if (!Slave_CY_SCBIP_V1)
            Slave_SET_INP_DIS(Slave_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         Slave_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & Slave_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!Slave_CY_SCBIP_V1) */
    
    #elif (Slave_RX_WAKE_SCL_MOSI_PIN)
        Slave_SET_HSIOM_SEL(Slave_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       Slave_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       Slave_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[Slave_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        Slave_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[Slave_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     Slave_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & Slave_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        Slave_SET_INCFG_TYPE(Slave_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        Slave_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        Slave_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        Slave_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (Slave_RX_WAKE_SCL_MOSI_PIN) */

    #if (Slave_TX_SDA_MISO_PIN)
        Slave_SET_HSIOM_SEL(Slave_TX_SDA_MISO_HSIOM_REG,
                                       Slave_TX_SDA_MISO_HSIOM_MASK,
                                       Slave_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[Slave_TX_SDA_MISO_PIN_INDEX]);

        Slave_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[Slave_TX_SDA_MISO_PIN_INDEX]);

    #if (!Slave_CY_SCBIP_V1)
        Slave_SET_INP_DIS(Slave_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     Slave_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & Slave_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!Slave_CY_SCBIP_V1) */
    #endif /* (Slave_RX_SCL_MOSI_PIN) */

    #if (Slave_CTS_SCLK_PIN)
        Slave_SET_HSIOM_SEL(Slave_CTS_SCLK_HSIOM_REG,
                                       Slave_CTS_SCLK_HSIOM_MASK,
                                       Slave_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[Slave_CTS_SCLK_PIN_INDEX]);

        Slave_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[Slave_CTS_SCLK_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_uart_cts_spi_sclk_INP_DIS,
                                     Slave_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & Slave_CTS_SCLK_PIN_MASK)));
    #endif /* (Slave_CTS_SCLK_PIN) */

    #if (Slave_RTS_SS0_PIN)
        Slave_SET_HSIOM_SEL(Slave_RTS_SS0_HSIOM_REG,
                                       Slave_RTS_SS0_HSIOM_MASK,
                                       Slave_RTS_SS0_HSIOM_POS,
                                       hsiomSel[Slave_RTS_SS0_PIN_INDEX]);

        Slave_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[Slave_RTS_SS0_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_uart_rts_spi_ss0_INP_DIS,
                                     Slave_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & Slave_RTS_SS0_PIN_MASK)));
    #endif /* (Slave_RTS_SS0_PIN) */

    #if (Slave_SS1_PIN)
        Slave_SET_HSIOM_SEL(Slave_SS1_HSIOM_REG,
                                       Slave_SS1_HSIOM_MASK,
                                       Slave_SS1_HSIOM_POS,
                                       hsiomSel[Slave_SS1_PIN_INDEX]);

        Slave_spi_ss1_SetDriveMode((uint8) pinsDm[Slave_SS1_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_spi_ss1_INP_DIS,
                                     Slave_spi_ss1_MASK,
                                     (0u != (pinsInBuf & Slave_SS1_PIN_MASK)));
    #endif /* (Slave_SS1_PIN) */

    #if (Slave_SS2_PIN)
        Slave_SET_HSIOM_SEL(Slave_SS2_HSIOM_REG,
                                       Slave_SS2_HSIOM_MASK,
                                       Slave_SS2_HSIOM_POS,
                                       hsiomSel[Slave_SS2_PIN_INDEX]);

        Slave_spi_ss2_SetDriveMode((uint8) pinsDm[Slave_SS2_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_spi_ss2_INP_DIS,
                                     Slave_spi_ss2_MASK,
                                     (0u != (pinsInBuf & Slave_SS2_PIN_MASK)));
    #endif /* (Slave_SS2_PIN) */

    #if (Slave_SS3_PIN)
        Slave_SET_HSIOM_SEL(Slave_SS3_HSIOM_REG,
                                       Slave_SS3_HSIOM_MASK,
                                       Slave_SS3_HSIOM_POS,
                                       hsiomSel[Slave_SS3_PIN_INDEX]);

        Slave_spi_ss3_SetDriveMode((uint8) pinsDm[Slave_SS3_PIN_INDEX]);

        Slave_SET_INP_DIS(Slave_spi_ss3_INP_DIS,
                                     Slave_spi_ss3_MASK,
                                     (0u != (pinsInBuf & Slave_SS3_PIN_MASK)));
    #endif /* (Slave_SS3_PIN) */
    }

#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: Slave_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void Slave_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (Slave_CTRL_REG & Slave_CTRL_EC_AM_MODE)) &&
            (0u == (Slave_I2C_CTRL_REG & Slave_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            Slave_CTRL_REG &= ~Slave_CTRL_EC_AM_MODE;
            Slave_CTRL_REG |=  Slave_CTRL_EC_AM_MODE;
        }

        Slave_I2C_SLAVE_CMD_REG = Slave_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1) */


/* [] END OF FILE */
