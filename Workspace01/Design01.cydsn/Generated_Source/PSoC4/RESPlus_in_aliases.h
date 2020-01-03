/*******************************************************************************
* File Name: RESPlus_in.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RESPlus_in_ALIASES_H) /* Pins RESPlus_in_ALIASES_H */
#define CY_PINS_RESPlus_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RESPlus_in_0			(RESPlus_in__0__PC)
#define RESPlus_in_0_PS		(RESPlus_in__0__PS)
#define RESPlus_in_0_PC		(RESPlus_in__0__PC)
#define RESPlus_in_0_DR		(RESPlus_in__0__DR)
#define RESPlus_in_0_SHIFT	(RESPlus_in__0__SHIFT)
#define RESPlus_in_0_INTR	((uint16)((uint16)0x0003u << (RESPlus_in__0__SHIFT*2u)))

#define RESPlus_in_INTR_ALL	 ((uint16)(RESPlus_in_0_INTR))


#endif /* End Pins RESPlus_in_ALIASES_H */


/* [] END OF FILE */
