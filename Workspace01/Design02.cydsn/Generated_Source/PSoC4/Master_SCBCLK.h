/*******************************************************************************
* File Name: Master_SCBCLK.h
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

#if !defined(CY_CLOCK_Master_SCBCLK_H)
#define CY_CLOCK_Master_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Master_SCBCLK_StartEx(uint32 alignClkDiv);
#define Master_SCBCLK_Start() \
    Master_SCBCLK_StartEx(Master_SCBCLK__PA_DIV_ID)

#else

void Master_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Master_SCBCLK_Stop(void);

void Master_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Master_SCBCLK_GetDividerRegister(void);
uint8  Master_SCBCLK_GetFractionalDividerRegister(void);

#define Master_SCBCLK_Enable()                         Master_SCBCLK_Start()
#define Master_SCBCLK_Disable()                        Master_SCBCLK_Stop()
#define Master_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    Master_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define Master_SCBCLK_SetDivider(clkDivider)           Master_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define Master_SCBCLK_SetDividerValue(clkDivider)      Master_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Master_SCBCLK_DIV_ID     Master_SCBCLK__DIV_ID

#define Master_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Master_SCBCLK_CTRL_REG   (*(reg32 *)Master_SCBCLK__CTRL_REGISTER)
#define Master_SCBCLK_DIV_REG    (*(reg32 *)Master_SCBCLK__DIV_REGISTER)

#define Master_SCBCLK_CMD_DIV_SHIFT          (0u)
#define Master_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define Master_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define Master_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define Master_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Master_SCBCLK_CMD_DISABLE_SHIFT))
#define Master_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Master_SCBCLK_CMD_ENABLE_SHIFT))

#define Master_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define Master_SCBCLK_DIV_FRAC_SHIFT (3u)
#define Master_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define Master_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define Master_SCBCLK_DIV_REG        (*(reg32 *)Master_SCBCLK__REGISTER)
#define Master_SCBCLK_ENABLE_REG     Master_SCBCLK_DIV_REG
#define Master_SCBCLK_DIV_FRAC_MASK  Master_SCBCLK__FRAC_MASK
#define Master_SCBCLK_DIV_FRAC_SHIFT (16u)
#define Master_SCBCLK_DIV_INT_MASK   Master_SCBCLK__DIVIDER_MASK
#define Master_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Master_SCBCLK_H) */

/* [] END OF FILE */
