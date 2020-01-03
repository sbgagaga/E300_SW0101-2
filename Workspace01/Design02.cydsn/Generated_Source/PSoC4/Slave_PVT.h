/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_Slave_H)
#define CY_SCB_PVT_Slave_H

#include "Slave.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define Slave_SetI2CExtClkInterruptMode(interruptMask) Slave_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define Slave_ClearI2CExtClkInterruptSource(interruptMask) Slave_CLEAR_INTR_I2C_EC(interruptMask)
#define Slave_GetI2CExtClkInterruptSource()                (Slave_INTR_I2C_EC_REG)
#define Slave_GetI2CExtClkInterruptMode()                  (Slave_INTR_I2C_EC_MASK_REG)
#define Slave_GetI2CExtClkInterruptSourceMasked()          (Slave_INTR_I2C_EC_MASKED_REG)

#if (!Slave_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define Slave_SetSpiExtClkInterruptMode(interruptMask) \
                                                                Slave_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define Slave_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                Slave_CLEAR_INTR_SPI_EC(interruptMask)
    #define Slave_GetExtSpiClkInterruptSource()                 (Slave_INTR_SPI_EC_REG)
    #define Slave_GetExtSpiClkInterruptMode()                   (Slave_INTR_SPI_EC_MASK_REG)
    #define Slave_GetExtSpiClkInterruptSourceMasked()           (Slave_INTR_SPI_EC_MASKED_REG)
#endif /* (!Slave_CY_SCBIP_V1) */

#if(Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void Slave_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (Slave_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER)
    extern cyisraddress Slave_customIntrHandler;
#endif /* !defined (CY_REMOVE_Slave_CUSTOM_INTR_HANDLER) */
#endif /* (Slave_SCB_IRQ_INTERNAL) */

extern Slave_BACKUP_STRUCT Slave_backup;

#if(Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 Slave_scbMode;
    extern uint8 Slave_scbEnableWake;
    extern uint8 Slave_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 Slave_mode;
    extern uint8 Slave_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * Slave_rxBuffer;
    extern uint8   Slave_rxDataBits;
    extern uint32  Slave_rxBufferSize;

    extern volatile uint8 * Slave_txBuffer;
    extern uint8   Slave_txDataBits;
    extern uint32  Slave_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 Slave_numberOfAddr;
    extern uint8 Slave_subAddrSize;
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (Slave_SCB_MODE_I2C_CONST_CFG || \
        Slave_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 Slave_IntrTxMask;
#endif /* (! (Slave_SCB_MODE_I2C_CONST_CFG || \
              Slave_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(Slave_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define Slave_SCB_MODE_I2C_RUNTM_CFG     (Slave_SCB_MODE_I2C      == Slave_scbMode)
    #define Slave_SCB_MODE_SPI_RUNTM_CFG     (Slave_SCB_MODE_SPI      == Slave_scbMode)
    #define Slave_SCB_MODE_UART_RUNTM_CFG    (Slave_SCB_MODE_UART     == Slave_scbMode)
    #define Slave_SCB_MODE_EZI2C_RUNTM_CFG   (Slave_SCB_MODE_EZI2C    == Slave_scbMode)
    #define Slave_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (Slave_SCB_MODE_UNCONFIG == Slave_scbMode)

    /* Defines wakeup enable */
    #define Slave_SCB_WAKE_ENABLE_CHECK       (0u != Slave_scbEnableWake)
#endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!Slave_CY_SCBIP_V1)
    #define Slave_SCB_PINS_NUMBER    (7u)
#else
    #define Slave_SCB_PINS_NUMBER    (2u)
#endif /* (!Slave_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_Slave_H) */


/* [] END OF FILE */
