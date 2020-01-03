/*******************************************************************************
* File Name: Crusie_in.h  
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

#if !defined(CY_PINS_Crusie_in_ALIASES_H) /* Pins Crusie_in_ALIASES_H */
#define CY_PINS_Crusie_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Crusie_in_0			(Crusie_in__0__PC)
#define Crusie_in_0_PS		(Crusie_in__0__PS)
#define Crusie_in_0_PC		(Crusie_in__0__PC)
#define Crusie_in_0_DR		(Crusie_in__0__DR)
#define Crusie_in_0_SHIFT	(Crusie_in__0__SHIFT)
#define Crusie_in_0_INTR	((uint16)((uint16)0x0003u << (Crusie_in__0__SHIFT*2u)))

#define Crusie_in_INTR_ALL	 ((uint16)(Crusie_in_0_INTR))


#endif /* End Pins Crusie_in_ALIASES_H */


/* [] END OF FILE */
