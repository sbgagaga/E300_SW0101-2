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

#if !defined(CY_SCB_PVT_Master_H)
#define CY_SCB_PVT_Master_H

#include "Master.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define Master_SetI2CExtClkInterruptMode(interruptMask) Master_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define Master_ClearI2CExtClkInterruptSource(interruptMask) Master_CLEAR_INTR_I2C_EC(interruptMask)
#define Master_GetI2CExtClkInterruptSource()                (Master_INTR_I2C_EC_REG)
#define Master_GetI2CExtClkInterruptMode()                  (Master_INTR_I2C_EC_MASK_REG)
#define Master_GetI2CExtClkInterruptSourceMasked()          (Master_INTR_I2C_EC_MASKED_REG)

#if (!Master_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define Master_SetSpiExtClkInterruptMode(interruptMask) \
                                                                Master_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define Master_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                Master_CLEAR_INTR_SPI_EC(interruptMask)
    #define Master_GetExtSpiClkInterruptSource()                 (Master_INTR_SPI_EC_REG)
    #define Master_GetExtSpiClkInterruptMode()                   (Master_INTR_SPI_EC_MASK_REG)
    #define Master_GetExtSpiClkInterruptSourceMasked()           (Master_INTR_SPI_EC_MASKED_REG)
#endif /* (!Master_CY_SCBIP_V1) */

#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void Master_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (Master_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER)
    extern cyisraddress Master_customIntrHandler;
#endif /* !defined (CY_REMOVE_Master_CUSTOM_INTR_HANDLER) */
#endif /* (Master_SCB_IRQ_INTERNAL) */

extern Master_BACKUP_STRUCT Master_backup;

#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 Master_scbMode;
    extern uint8 Master_scbEnableWake;
    extern uint8 Master_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 Master_mode;
    extern uint8 Master_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * Master_rxBuffer;
    extern uint8   Master_rxDataBits;
    extern uint32  Master_rxBufferSize;

    extern volatile uint8 * Master_txBuffer;
    extern uint8   Master_txDataBits;
    extern uint32  Master_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 Master_numberOfAddr;
    extern uint8 Master_subAddrSize;
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (Master_SCB_MODE_I2C_CONST_CFG || \
        Master_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 Master_IntrTxMask;
#endif /* (! (Master_SCB_MODE_I2C_CONST_CFG || \
              Master_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define Master_SCB_MODE_I2C_RUNTM_CFG     (Master_SCB_MODE_I2C      == Master_scbMode)
    #define Master_SCB_MODE_SPI_RUNTM_CFG     (Master_SCB_MODE_SPI      == Master_scbMode)
    #define Master_SCB_MODE_UART_RUNTM_CFG    (Master_SCB_MODE_UART     == Master_scbMode)
    #define Master_SCB_MODE_EZI2C_RUNTM_CFG   (Master_SCB_MODE_EZI2C    == Master_scbMode)
    #define Master_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (Master_SCB_MODE_UNCONFIG == Master_scbMode)

    /* Defines wakeup enable */
    #define Master_SCB_WAKE_ENABLE_CHECK       (0u != Master_scbEnableWake)
#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!Master_CY_SCBIP_V1)
    #define Master_SCB_PINS_NUMBER    (7u)
#else
    #define Master_SCB_PINS_NUMBER    (2u)
#endif /* (!Master_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_Master_H) */


/* [] END OF FILE */
