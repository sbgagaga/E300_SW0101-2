/***************************************************************************//**
* \file Master_I2C.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
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
#include "Master_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 Master_state;  /* Current state of I2C FSM */

#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const Master_I2C_INIT_STRUCT Master_configI2C =
    {
        Master_I2C_MODE,
        Master_I2C_OVS_FACTOR_LOW,
        Master_I2C_OVS_FACTOR_HIGH,
        Master_I2C_MEDIAN_FILTER_ENABLE,
        Master_I2C_SLAVE_ADDRESS,
        Master_I2C_SLAVE_ADDRESS_MASK,
        Master_I2C_ACCEPT_ADDRESS,
        Master_I2C_WAKE_ENABLE,
        Master_I2C_BYTE_MODE_ENABLE,
        Master_I2C_DATA_RATE,
        Master_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: Master_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the Master for I2C operation.
    *
    *  This function is intended specifically to be used when the Master 
    *  configuration is set to “Unconfigured Master” in the customizer. 
    *  After initializing the Master in I2C mode using this function, 
    *  the component can be enabled using the Master_Start() or 
    * Master_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void Master_I2CInit(const Master_I2C_INIT_STRUCT *config)
    {
        uint32 medianFilter;
        uint32 locEnableWake;

        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            Master_SetPins(Master_SCB_MODE_I2C, Master_DUMMY_PARAM,
                                     Master_DUMMY_PARAM);

            /* Store internal configuration */
            Master_scbMode       = (uint8) Master_SCB_MODE_I2C;
            Master_scbEnableWake = (uint8) config->enableWake;
            Master_scbEnableIntr = (uint8) Master_SCB_IRQ_INTERNAL;

            Master_mode          = (uint8) config->mode;
            Master_acceptAddr    = (uint8) config->acceptAddr;

        #if (Master_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            Master_SET_I2C_CFG_SDA_FILT_TRIM(Master_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (Master_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((Master_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= Master_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (Master_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                Master_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = Master_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                Master_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = Master_DIGITAL_FILTER_ENABLE;
            }

        #if (!Master_CY_SCBIP_V0)
            locEnableWake = (Master_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!Master_CY_SCBIP_V0) */

            /* Configure I2C interface */
            Master_CTRL_REG     = Master_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            Master_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            Master_GET_CTRL_EC_AM_MODE (locEnableWake);

            Master_I2C_CTRL_REG = Master_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    Master_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    Master_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    Master_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            Master_RX_CTRL_REG      = Master_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                Master_I2C_RX_CTRL;
            Master_RX_FIFO_CTRL_REG = Master_CLEAR_REG;

            /* Set default address and mask */
            Master_RX_MATCH_REG    = ((Master_I2C_SLAVE) ?
                                                (Master_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 Master_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (Master_CLEAR_REG));


            /* Configure TX direction */
            Master_TX_CTRL_REG      = Master_I2C_TX_CTRL;
            Master_TX_FIFO_CTRL_REG = Master_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (Master_ISR_NUMBER);
            CyIntSetPriority(Master_ISR_NUMBER, Master_ISR_PRIORITY);
            (void) CyIntSetVector(Master_ISR_NUMBER, &Master_I2C_ISR);

            /* Configure interrupt sources */
        #if(!Master_CY_SCBIP_V1)
            Master_INTR_SPI_EC_MASK_REG = Master_NO_INTR_SOURCES;
        #endif /* (!Master_CY_SCBIP_V1) */

            Master_INTR_I2C_EC_MASK_REG = Master_NO_INTR_SOURCES;
            Master_INTR_RX_MASK_REG     = Master_NO_INTR_SOURCES;
            Master_INTR_TX_MASK_REG     = Master_NO_INTR_SOURCES;

            Master_INTR_SLAVE_MASK_REG  = ((Master_I2C_SLAVE) ?
                            (Master_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             Master_I2C_INTR_SLAVE_MASK) : (Master_CLEAR_REG));

            Master_INTR_MASTER_MASK_REG = Master_NO_INTR_SOURCES;

            /* Configure global variables */
            Master_state = Master_I2C_FSM_IDLE;

            /* Internal slave variables */
            Master_slStatus        = 0u;
            Master_slRdBufIndex    = 0u;
            Master_slWrBufIndex    = 0u;
            Master_slOverFlowCount = 0u;

            /* Internal master variables */
            Master_mstrStatus     = 0u;
            Master_mstrRdBufIndex = 0u;
            Master_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: Master_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void Master_I2CInit(void)
    {
    #if(Master_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        Master_SET_I2C_CFG_SDA_FILT_TRIM(Master_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (Master_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        Master_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        Master_CTRL_REG     = Master_I2C_DEFAULT_CTRL;
        Master_I2C_CTRL_REG = Master_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        Master_RX_CTRL_REG      = Master_I2C_DEFAULT_RX_CTRL;
        Master_RX_FIFO_CTRL_REG = Master_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        Master_RX_MATCH_REG     = Master_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        Master_TX_CTRL_REG      = Master_I2C_DEFAULT_TX_CTRL;
        Master_TX_FIFO_CTRL_REG = Master_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (Master_ISR_NUMBER);
        CyIntSetPriority(Master_ISR_NUMBER, Master_ISR_PRIORITY);
    #if(!Master_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(Master_ISR_NUMBER, &Master_I2C_ISR);
    #endif /* (Master_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!Master_CY_SCBIP_V1)
        Master_INTR_SPI_EC_MASK_REG = Master_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!Master_CY_SCBIP_V1) */

        Master_INTR_I2C_EC_MASK_REG = Master_I2C_DEFAULT_INTR_I2C_EC_MASK;
        Master_INTR_SLAVE_MASK_REG  = Master_I2C_DEFAULT_INTR_SLAVE_MASK;
        Master_INTR_MASTER_MASK_REG = Master_I2C_DEFAULT_INTR_MASTER_MASK;
        Master_INTR_RX_MASK_REG     = Master_I2C_DEFAULT_INTR_RX_MASK;
        Master_INTR_TX_MASK_REG     = Master_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        Master_state = Master_I2C_FSM_IDLE;

    #if(Master_I2C_SLAVE)
        /* Internal slave variable */
        Master_slStatus        = 0u;
        Master_slRdBufIndex    = 0u;
        Master_slWrBufIndex    = 0u;
        Master_slOverFlowCount = 0u;
    #endif /* (Master_I2C_SLAVE) */

    #if(Master_I2C_MASTER)
    /* Internal master variable */
        Master_mstrStatus     = 0u;
        Master_mstrRdBufIndex = 0u;
        Master_mstrWrBufIndex = 0u;
    #endif /* (Master_I2C_MASTER) */
    }
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: Master_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void Master_I2CStop(void)
{
    /* Clear command registers because they keep assigned value after IP block was disabled */
    Master_I2C_MASTER_CMD_REG = 0u;
    Master_I2C_SLAVE_CMD_REG  = 0u;
    
    Master_state = Master_I2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: Master_I2CFwBlockReset
****************************************************************************//**
*
* Resets the scb IP block and I2C into the known state.
*
*******************************************************************************/
void Master_I2CFwBlockReset(void)
{
    /* Disable scb IP: stop respond to I2C traffic */
    Master_CTRL_REG &= (uint32) ~Master_CTRL_ENABLED;

    /* Clear command registers they are not cleared after scb IP is disabled */
    Master_I2C_MASTER_CMD_REG = 0u;
    Master_I2C_SLAVE_CMD_REG  = 0u;

    Master_DISABLE_AUTO_DATA;

    Master_SetTxInterruptMode(Master_NO_INTR_SOURCES);
    Master_SetRxInterruptMode(Master_NO_INTR_SOURCES);
    
#if(Master_CY_SCBIP_V0)
    /* Clear interrupt sources as they are not cleared after scb IP is disabled */
    Master_ClearTxInterruptSource    (Master_INTR_TX_ALL);
    Master_ClearRxInterruptSource    (Master_INTR_RX_ALL);
    Master_ClearSlaveInterruptSource (Master_INTR_SLAVE_ALL);
    Master_ClearMasterInterruptSource(Master_INTR_MASTER_ALL);
#endif /* (Master_CY_SCBIP_V0) */

    Master_state = Master_I2C_FSM_IDLE;

    /* Enable scb IP: start respond to I2C traffic */
    Master_CTRL_REG |= (uint32) Master_CTRL_ENABLED;
}


#if(Master_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: Master_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables Master_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void Master_I2CSaveConfig(void)
    {
    #if (!Master_CY_SCBIP_V0)
        #if (Master_I2C_MULTI_MASTER_SLAVE_CONST && Master_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (Master_CTRL_REG & Master_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                Master_Stop();
                Master_CTRL_REG |= Master_CTRL_EC_AM_MODE;
                Master_Enable();
            }
        #endif /* (Master_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (Master_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            Master_SCBCLK_Stop();
        #endif /* (Master_SCB_CLK_INTERNAL) */
    #endif /* (!Master_CY_SCBIP_V0) */

        Master_SetI2CExtClkInterruptMode(Master_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: Master_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables Master_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void Master_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        Master_SetI2CExtClkInterruptMode(Master_NO_INTR_SOURCES);

    #if (!Master_CY_SCBIP_V0)
        #if (Master_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            Master_SCBCLK_Start();
        #endif /* (Master_SCB_CLK_INTERNAL) */
    #endif /* (!Master_CY_SCBIP_V0) */
    }
#endif /* (Master_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
