/*******************************************************************************
* File Name: Master_scl.h  
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

#if !defined(CY_PINS_Master_scl_H) /* Pins Master_scl_H */
#define CY_PINS_Master_scl_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Master_scl_aliases.h"


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
} Master_scl_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Master_scl_Read(void);
void    Master_scl_Write(uint8 value);
uint8   Master_scl_ReadDataReg(void);
#if defined(Master_scl__PC) || (CY_PSOC4_4200L) 
    void    Master_scl_SetDriveMode(uint8 mode);
#endif
void    Master_scl_SetInterruptMode(uint16 position, uint16 mode);
uint8   Master_scl_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Master_scl_Sleep(void); 
void Master_scl_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Master_scl__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Master_scl_DRIVE_MODE_BITS        (3)
    #define Master_scl_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Master_scl_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Master_scl_SetDriveMode() function.
         *  @{
         */
        #define Master_scl_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Master_scl_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Master_scl_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Master_scl_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Master_scl_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Master_scl_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Master_scl_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Master_scl_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Master_scl_MASK               Master_scl__MASK
#define Master_scl_SHIFT              Master_scl__SHIFT
#define Master_scl_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Master_scl_SetInterruptMode() function.
     *  @{
     */
        #define Master_scl_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Master_scl_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Master_scl_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Master_scl_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Master_scl__SIO)
    #define Master_scl_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Master_scl__PC) && (CY_PSOC4_4200L)
    #define Master_scl_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Master_scl_USBIO_DISABLE              ((uint32)(~Master_scl_USBIO_ENABLE))
    #define Master_scl_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Master_scl_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Master_scl_USBIO_ENTER_SLEEP          ((uint32)((1u << Master_scl_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Master_scl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Master_scl_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Master_scl_USBIO_SUSPEND_SHIFT)))
    #define Master_scl_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Master_scl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Master_scl_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Master_scl__PC)
    /* Port Configuration */
    #define Master_scl_PC                 (* (reg32 *) Master_scl__PC)
#endif
/* Pin State */
#define Master_scl_PS                     (* (reg32 *) Master_scl__PS)
/* Data Register */
#define Master_scl_DR                     (* (reg32 *) Master_scl__DR)
/* Input Buffer Disable Override */
#define Master_scl_INP_DIS                (* (reg32 *) Master_scl__PC2)

/* Interrupt configuration Registers */
#define Master_scl_INTCFG                 (* (reg32 *) Master_scl__INTCFG)
#define Master_scl_INTSTAT                (* (reg32 *) Master_scl__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Master_scl_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Master_scl__SIO)
    #define Master_scl_SIO_REG            (* (reg32 *) Master_scl__SIO)
#endif /* (Master_scl__SIO_CFG) */

/* USBIO registers */
#if !defined(Master_scl__PC) && (CY_PSOC4_4200L)
    #define Master_scl_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Master_scl_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Master_scl_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Master_scl_DRIVE_MODE_SHIFT       (0x00u)
#define Master_scl_DRIVE_MODE_MASK        (0x07u << Master_scl_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Master_scl_H */


/* [] END OF FILE */
