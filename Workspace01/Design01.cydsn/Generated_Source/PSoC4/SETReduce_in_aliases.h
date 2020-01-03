/*******************************************************************************
* File Name: SETReduce_in.h  
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

#if !defined(CY_PINS_SETReduce_in_ALIASES_H) /* Pins SETReduce_in_ALIASES_H */
#define CY_PINS_SETReduce_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SETReduce_in_0			(SETReduce_in__0__PC)
#define SETReduce_in_0_PS		(SETReduce_in__0__PS)
#define SETReduce_in_0_PC		(SETReduce_in__0__PC)
#define SETReduce_in_0_DR		(SETReduce_in__0__DR)
#define SETReduce_in_0_SHIFT	(SETReduce_in__0__SHIFT)
#define SETReduce_in_0_INTR	((uint16)((uint16)0x0003u << (SETReduce_in__0__SHIFT*2u)))

#define SETReduce_in_INTR_ALL	 ((uint16)(SETReduce_in_0_INTR))


#endif /* End Pins SETReduce_in_ALIASES_H */


/* [] END OF FILE */
