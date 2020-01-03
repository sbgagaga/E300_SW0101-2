/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
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

#if !defined(CY_SCB_I2C_PVT_Slave_H)
#define CY_SCB_I2C_PVT_Slave_H

#include "Slave_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 Slave_state; /* Current state of I2C FSM */

#if(Slave_I2C_SLAVE_CONST)
    extern volatile uint8 Slave_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * Slave_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  Slave_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  Slave_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * Slave_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  Slave_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  Slave_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  Slave_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   Slave_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (Slave_I2C_SLAVE_CONST) */

#if(Slave_I2C_MASTER_CONST)
    extern volatile uint16 Slave_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  Slave_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * Slave_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  Slave_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  Slave_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * Slave_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  Slave_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  Slave_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  Slave_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (Slave_I2C_MASTER_CONST) */

#if (Slave_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*Slave_customAddressHandler) (void);
#endif /* (Slave_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(Slave_SCB_MODE_I2C_CONST_CFG)
    void Slave_I2CInit(void);
#endif /* (Slave_SCB_MODE_I2C_CONST_CFG) */

void Slave_I2CStop(void);
void Slave_I2CFwBlockReset(void);

void Slave_I2CSaveConfig(void);
void Slave_I2CRestoreConfig(void);

#if(Slave_I2C_MASTER_CONST)
    void Slave_I2CReStartGeneration(void);
#endif /* (Slave_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_Slave_H) */


/* [] END OF FILE */
