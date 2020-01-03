/*******************************************************************************
* File Name: Slave_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Slave_SCBCLK_H)
#define CY_CLOCK_Slave_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Slave_SCBCLK_StartEx(uint32 alignClkDiv);
#define Slave_SCBCLK_Start() \
    Slave_SCBCLK_StartEx(Slave_SCBCLK__PA_DIV_ID)

#else

void Slave_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Slave_SCBCLK_Stop(void);

void Slave_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Slave_SCBCLK_GetDividerRegister(void);
uint8  Slave_SCBCLK_GetFractionalDividerRegister(void);

#define Slave_SCBCLK_Enable()                         Slave_SCBCLK_Start()
#define Slave_SCBCLK_Disable()                        Slave_SCBCLK_Stop()
#define Slave_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    Slave_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define Slave_SCBCLK_SetDivider(clkDivider)           Slave_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define Slave_SCBCLK_SetDividerValue(clkDivider)      Slave_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Slave_SCBCLK_DIV_ID     Slave_SCBCLK__DIV_ID

#define Slave_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Slave_SCBCLK_CTRL_REG   (*(reg32 *)Slave_SCBCLK__CTRL_REGISTER)
#define Slave_SCBCLK_DIV_REG    (*(reg32 *)Slave_SCBCLK__DIV_REGISTER)

#define Slave_SCBCLK_CMD_DIV_SHIFT          (0u)
#define Slave_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define Slave_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define Slave_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define Slave_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Slave_SCBCLK_CMD_DISABLE_SHIFT))
#define Slave_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Slave_SCBCLK_CMD_ENABLE_SHIFT))

#define Slave_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define Slave_SCBCLK_DIV_FRAC_SHIFT (3u)
#define Slave_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define Slave_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define Slave_SCBCLK_DIV_REG        (*(reg32 *)Slave_SCBCLK__REGISTER)
#define Slave_SCBCLK_ENABLE_REG     Slave_SCBCLK_DIV_REG
#define Slave_SCBCLK_DIV_FRAC_MASK  Slave_SCBCLK__FRAC_MASK
#define Slave_SCBCLK_DIV_FRAC_SHIFT (16u)
#define Slave_SCBCLK_DIV_INT_MASK   Slave_SCBCLK__DIVIDER_MASK
#define Slave_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Slave_SCBCLK_H) */

/* [] END OF FILE */
