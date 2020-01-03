/*******************************************************************************
* File Name: VolReduce_in.h  
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

#if !defined(CY_PINS_VolReduce_in_ALIASES_H) /* Pins VolReduce_in_ALIASES_H */
#define CY_PINS_VolReduce_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define VolReduce_in_0			(VolReduce_in__0__PC)
#define VolReduce_in_0_PS		(VolReduce_in__0__PS)
#define VolReduce_in_0_PC		(VolReduce_in__0__PC)
#define VolReduce_in_0_DR		(VolReduce_in__0__DR)
#define VolReduce_in_0_SHIFT	(VolReduce_in__0__SHIFT)
#define VolReduce_in_0_INTR	((uint16)((uint16)0x0003u << (VolReduce_in__0__SHIFT*2u)))

#define VolReduce_in_INTR_ALL	 ((uint16)(VolReduce_in_0_INTR))


#endif /* End Pins VolReduce_in_ALIASES_H */


/* [] END OF FILE */
