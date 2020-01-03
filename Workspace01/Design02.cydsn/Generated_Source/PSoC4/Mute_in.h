/*******************************************************************************
* File Name: Mute_in.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Mute_in_H) /* Pins Mute_in_H */
#define CY_PINS_Mute_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Mute_in_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Mute_in_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Mute_in_Read(void);
void    Mute_in_Write(uint8 value);
uint8   Mute_in_ReadDataReg(void);
#if defined(Mute_in__PC) || (CY_PSOC4_4200L) 
    void    Mute_in_SetDriveMode(uint8 mode);
#endif
void    Mute_in_SetInterruptMode(uint16 position, uint16 mode);
uint8   Mute_in_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Mute_in_Sleep(void); 
void Mute_in_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Mute_in__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Mute_in_DRIVE_MODE_BITS        (3)
    #define Mute_in_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Mute_in_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Mute_in_SetDriveMode() function.
         *  @{
         */
        #define Mute_in_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Mute_in_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Mute_in_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Mute_in_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Mute_in_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Mute_in_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Mute_in_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Mute_in_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Mute_in_MASK               Mute_in__MASK
#define Mute_in_SHIFT              Mute_in__SHIFT
#define Mute_in_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Mute_in_SetInterruptMode() function.
     *  @{
     */
        #define Mute_in_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Mute_in_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Mute_in_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Mute_in_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Mute_in__SIO)
    #define Mute_in_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Mute_in__PC) && (CY_PSOC4_4200L)
    #define Mute_in_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Mute_in_USBIO_DISABLE              ((uint32)(~Mute_in_USBIO_ENABLE))
    #define Mute_in_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Mute_in_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Mute_in_USBIO_ENTER_SLEEP          ((uint32)((1u << Mute_in_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Mute_in_USBIO_SUSPEND_DEL_SHIFT)))
    #define Mute_in_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Mute_in_USBIO_SUSPEND_SHIFT)))
    #define Mute_in_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Mute_in_USBIO_SUSPEND_DEL_SHIFT)))
    #define Mute_in_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Mute_in__PC)
    /* Port Configuration */
    #define Mute_in_PC                 (* (reg32 *) Mute_in__PC)
#endif
/* Pin State */
#define Mute_in_PS                     (* (reg32 *) Mute_in__PS)
/* Data Register */
#define Mute_in_DR                     (* (reg32 *) Mute_in__DR)
/* Input Buffer Disable Override */
#define Mute_in_INP_DIS                (* (reg32 *) Mute_in__PC2)

/* Interrupt configuration Registers */
#define Mute_in_INTCFG                 (* (reg32 *) Mute_in__INTCFG)
#define Mute_in_INTSTAT                (* (reg32 *) Mute_in__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Mute_in_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Mute_in__SIO)
    #define Mute_in_SIO_REG            (* (reg32 *) Mute_in__SIO)
#endif /* (Mute_in__SIO_CFG) */

/* USBIO registers */
#if !defined(Mute_in__PC) && (CY_PSOC4_4200L)
    #define Mute_in_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Mute_in_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Mute_in_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Mute_in_DRIVE_MODE_SHIFT       (0x00u)
#define Mute_in_DRIVE_MODE_MASK        (0x07u << Mute_in_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Mute_in_H */


/* [] END OF FILE */
