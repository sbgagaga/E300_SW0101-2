/***************************************************************************//**
* \file Slave_I2C.c
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

#include "Slave_PVT.h"
#include "Slave_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 Slave_state;  /* Current state of I2C FSM */

#if(Slave_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const Slave_I2C_INIT_STRUCT Slave_configI2C =
    {
        Slave_I2C_MODE,
        Slave_I2C_OVS_FACTOR_LOW,
        Slave_I2C_OVS_FACTOR_HIGH,
        Slave_I2C_MEDIAN_FILTER_ENABLE,
        Slave_I2C_SLAVE_ADDRESS,
        Slave_I2C_SLAVE_ADDRESS_MASK,
        Slave_I2C_ACCEPT_ADDRESS,
        Slave_I2C_WAKE_ENABLE,
        Slave_I2C_BYTE_MODE_ENABLE,
        Slave_I2C_DATA_RATE,
        Slave_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: Slave_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the Slave for I2C operation.
    *
    *  This function is intended specifically to be used when the Slave 
    *  configuration is set to “Unconfigured Slave” in the customizer. 
    *  After initializing the Slave in I2C mode using this function, 
    *  the component can be enabled using the Slave_Start() or 
    * Slave_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void Slave_I2CInit(const Slave_I2C_INIT_STRUCT *config)
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
            Slave_SetPins(Slave_SCB_MODE_I2C, Slave_DUMMY_PARAM,
                                     Slave_DUMMY_PARAM);

            /* Store internal configuration */
            Slave_scbMode       = (uint8) Slave_SCB_MODE_I2C;
            Slave_scbEnableWake = (uint8) config->enableWake;
            Slave_scbEnableIntr = (uint8) Slave_SCB_IRQ_INTERNAL;

            Slave_mode          = (uint8) config->mode;
            Slave_acceptAddr    = (uint8) config->acceptAddr;

        #if (Slave_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            Slave_SET_I2C_CFG_SDA_FILT_TRIM(Slave_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (Slave_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((Slave_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= Slave_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (Slave_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                Slave_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = Slave_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                Slave_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = Slave_DIGITAL_FILTER_ENABLE;
            }

        #if (!Slave_CY_SCBIP_V0)
            locEnableWake = (Slave_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!Slave_CY_SCBIP_V0) */

            /* Configure I2C interface */
            Slave_CTRL_REG     = Slave_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            Slave_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            Slave_GET_CTRL_EC_AM_MODE (locEnableWake);

            Slave_I2C_CTRL_REG = Slave_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    Slave_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    Slave_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    Slave_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            Slave_RX_CTRL_REG      = Slave_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                Slave_I2C_RX_CTRL;
            Slave_RX_FIFO_CTRL_REG = Slave_CLEAR_REG;

            /* Set default address and mask */
            Slave_RX_MATCH_REG    = ((Slave_I2C_SLAVE) ?
                                                (Slave_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 Slave_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (Slave_CLEAR_REG));


            /* Configure TX direction */
            Slave_TX_CTRL_REG      = Slave_I2C_TX_CTRL;
            Slave_TX_FIFO_CTRL_REG = Slave_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (Slave_ISR_NUMBER);
            CyIntSetPriority(Slave_ISR_NUMBER, Slave_ISR_PRIORITY);
            (void) CyIntSetVector(Slave_ISR_NUMBER, &Slave_I2C_ISR);

            /* Configure interrupt sources */
        #if(!Slave_CY_SCBIP_V1)
            Slave_INTR_SPI_EC_MASK_REG = Slave_NO_INTR_SOURCES;
        #endif /* (!Slave_CY_SCBIP_V1) */

            Slave_INTR_I2C_EC_MASK_REG = Slave_NO_INTR_SOURCES;
            Slave_INTR_RX_MASK_REG     = Slave_NO_INTR_SOURCES;
            Slave_INTR_TX_MASK_REG     = Slave_NO_INTR_SOURCES;

            Slave_INTR_SLAVE_MASK_REG  = ((Slave_I2C_SLAVE) ?
                            (Slave_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             Slave_I2C_INTR_SLAVE_MASK) : (Slave_CLEAR_REG));

            Slave_INTR_MASTER_MASK_REG = Slave_NO_INTR_SOURCES;

            /* Configure global variables */
            Slave_state = Slave_I2C_FSM_IDLE;

            /* Internal slave variables */
            Slave_slStatus        = 0u;
            Slave_slRdBufIndex    = 0u;
            Slave_slWrBufIndex    = 0u;
            Slave_slOverFlowCount = 0u;

            /* Internal master variables */
            Slave_mstrStatus     = 0u;
            Slave_mstrRdBufIndex = 0u;
            Slave_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: Slave_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void Slave_I2CInit(void)
    {
    #if(Slave_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        Slave_SET_I2C_CFG_SDA_FILT_TRIM(Slave_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (Slave_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        Slave_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        Slave_CTRL_REG     = Slave_I2C_DEFAULT_CTRL;
        Slave_I2C_CTRL_REG = Slave_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        Slave_RX_CTRL_REG      = Slave_I2C_DEFAULT_RX_CTRL;
        Slave_RX_FIFO_CTRL_REG = Slave_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        Slave_RX_MATCH_REG     = Slave_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        Slave_TX_CTRL_REG      = Slave_I2C_DEFAULT_TX_CTRL;
        Slave_TX_FIFO_CTRL_REG = Slave_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (Slave_ISR_NUMBER);
        CyIntSetPriority(Slave_ISR_NUMBER, Slave_ISR_PRIORITY);
    #if(!Slave_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(Slave_ISR_NUMBER, &Slave_I2C_ISR);
    #endif /* (Slave_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!Slave_CY_SCBIP_V1)
        Slave_INTR_SPI_EC_MASK_REG = Slave_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!Slave_CY_SCBIP_V1) */

        Slave_INTR_I2C_EC_MASK_REG = Slave_I2C_DEFAULT_INTR_I2C_EC_MASK;
        Slave_INTR_SLAVE_MASK_REG  = Slave_I2C_DEFAULT_INTR_SLAVE_MASK;
        Slave_INTR_MASTER_MASK_REG = Slave_I2C_DEFAULT_INTR_MASTER_MASK;
        Slave_INTR_RX_MASK_REG     = Slave_I2C_DEFAULT_INTR_RX_MASK;
        Slave_INTR_TX_MASK_REG     = Slave_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        Slave_state = Slave_I2C_FSM_IDLE;

    #if(Slave_I2C_SLAVE)
        /* Internal slave variable */
        Slave_slStatus        = 0u;
        Slave_slRdBufIndex    = 0u;
        Slave_slWrBufIndex    = 0u;
        Slave_slOverFlowCount = 0u;
    #endif /* (Slave_I2C_SLAVE) */

    #if(Slave_I2C_MASTER)
    /* Internal master variable */
        Slave_mstrStatus     = 0u;
        Slave_mstrRdBufIndex = 0u;
        Slave_mstrWrBufIndex = 0u;
    #endif /* (Slave_I2C_MASTER) */
    }
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: Slave_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void Slave_I2CStop(void)
{
    /* Clear command registers because they keep assigned value after IP block was disabled */
    Slave_I2C_MASTER_CMD_REG = 0u;
    Slave_I2C_SLAVE_CMD_REG  = 0u;
    
    Slave_state = Slave_I2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: Slave_I2CFwBlockReset
****************************************************************************//**
*
* Resets the scb IP block and I2C into the known state.
*
*******************************************************************************/
void Slave_I2CFwBlockReset(void)
{
    /* Disable scb IP: stop respond to I2C traffic */
    Slave_CTRL_REG &= (uint32) ~Slave_CTRL_ENABLED;

    /* Clear command registers they are not cleared after scb IP is disabled */
    Slave_I2C_MASTER_CMD_REG = 0u;
    Slave_I2C_SLAVE_CMD_REG  = 0u;

    Slave_DISABLE_AUTO_DATA;

    Slave_SetTxInterruptMode(Slave_NO_INTR_SOURCES);
    Slave_SetRxInterruptMode(Slave_NO_INTR_SOURCES);
    
#if(Slave_CY_SCBIP_V0)
    /* Clear interrupt sources as they are not cleared after scb IP is disabled */
    Slave_ClearTxInterruptSource    (Slave_INTR_TX_ALL);
    Slave_ClearRxInterruptSource    (Slave_INTR_RX_ALL);
    Slave_ClearSlaveInterruptSource (Slave_INTR_SLAVE_ALL);
    Slave_ClearMasterInterruptSource(Slave_INTR_MASTER_ALL);
#endif /* (Slave_CY_SCBIP_V0) */

    Slave_state = Slave_I2C_FSM_IDLE;

    /* Enable scb IP: start respond to I2C traffic */
    Slave_CTRL_REG |= (uint32) Slave_CTRL_ENABLED;
}


#if(Slave_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: Slave_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables Slave_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void Slave_I2CSaveConfig(void)
    {
    #if (!Slave_CY_SCBIP_V0)
        #if (Slave_I2C_MULTI_MASTER_SLAVE_CONST && Slave_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (Slave_CTRL_REG & Slave_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                Slave_Stop();
                Slave_CTRL_REG |= Slave_CTRL_EC_AM_MODE;
                Slave_Enable();
            }
        #endif /* (Slave_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (Slave_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            Slave_SCBCLK_Stop();
        #endif /* (Slave_SCB_CLK_INTERNAL) */
    #endif /* (!Slave_CY_SCBIP_V0) */

        Slave_SetI2CExtClkInterruptMode(Slave_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: Slave_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables Slave_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void Slave_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        Slave_SetI2CExtClkInterruptMode(Slave_NO_INTR_SOURCES);

    #if (!Slave_CY_SCBIP_V0)
        #if (Slave_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            Slave_SCBCLK_Start();
        #endif /* (Slave_SCB_CLK_INTERNAL) */
    #endif /* (!Slave_CY_SCBIP_V0) */
    }
#endif /* (Slave_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
