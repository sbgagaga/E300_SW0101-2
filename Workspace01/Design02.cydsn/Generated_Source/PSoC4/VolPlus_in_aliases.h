/*******************************************************************************
* File Name: VolPlus_in.h  
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

#if !defined(CY_PINS_VolPlus_in_ALIASES_H) /* Pins VolPlus_in_ALIASES_H */
#define CY_PINS_VolPlus_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define VolPlus_in_0			(VolPlus_in__0__PC)
#define VolPlus_in_0_PS		(VolPlus_in__0__PS)
#define VolPlus_in_0_PC		(VolPlus_in__0__PC)
#define VolPlus_in_0_DR		(VolPlus_in__0__DR)
#define VolPlus_in_0_SHIFT	(VolPlus_in__0__SHIFT)
#define VolPlus_in_0_INTR	((uint16)((uint16)0x0003u << (VolPlus_in__0__SHIFT*2u)))

#define VolPlus_in_INTR_ALL	 ((uint16)(VolPlus_in_0_INTR))


#endif /* End Pins VolPlus_in_ALIASES_H */


/* [] END OF FILE */
