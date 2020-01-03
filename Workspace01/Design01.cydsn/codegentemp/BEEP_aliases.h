/*******************************************************************************
* File Name: BEEP.h  
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

#if !defined(CY_PINS_BEEP_ALIASES_H) /* Pins BEEP_ALIASES_H */
#define CY_PINS_BEEP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define BEEP_0			(BEEP__0__PC)
#define BEEP_0_PS		(BEEP__0__PS)
#define BEEP_0_PC		(BEEP__0__PC)
#define BEEP_0_DR		(BEEP__0__DR)
#define BEEP_0_SHIFT	(BEEP__0__SHIFT)
#define BEEP_0_INTR	((uint16)((uint16)0x0003u << (BEEP__0__SHIFT*2u)))

#define BEEP_INTR_ALL	 ((uint16)(BEEP_0_INTR))


#endif /* End Pins BEEP_ALIASES_H */


/* [] END OF FILE */
