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

#if !defined(CY_SCB_I2C_PVT_Master_H)
#define CY_SCB_I2C_PVT_Master_H

#include "Master_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 Master_state; /* Current state of I2C FSM */

#if(Master_I2C_SLAVE_CONST)
    extern volatile uint8 Master_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * Master_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  Master_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  Master_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * Master_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  Master_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  Master_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  Master_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   Master_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (Master_I2C_SLAVE_CONST) */

#if(Master_I2C_MASTER_CONST)
    extern volatile uint16 Master_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  Master_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * Master_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  Master_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  Master_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * Master_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  Master_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  Master_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  Master_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (Master_I2C_MASTER_CONST) */

#if (Master_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*Master_customAddressHandler) (void);
#endif /* (Master_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(Master_SCB_MODE_I2C_CONST_CFG)
    void Master_I2CInit(void);
#endif /* (Master_SCB_MODE_I2C_CONST_CFG) */

void Master_I2CStop(void);
void Master_I2CFwBlockReset(void);

void Master_I2CSaveConfig(void);
void Master_I2CRestoreConfig(void);

#if(Master_I2C_MASTER_CONST)
    void Master_I2CReStartGeneration(void);
#endif /* (Master_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_Master_H) */


/* [] END OF FILE */
