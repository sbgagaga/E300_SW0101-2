/*******************************************************************************
* File Name: Slave_scl.h  
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

#if !defined(CY_PINS_Slave_scl_H) /* Pins Slave_scl_H */
#define CY_PINS_Slave_scl_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Slave_scl_aliases.h"


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
} Slave_scl_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Slave_scl_Read(void);
void    Slave_scl_Write(uint8 value);
uint8   Slave_scl_ReadDataReg(void);
#if defined(Slave_scl__PC) || (CY_PSOC4_4200L) 
    void    Slave_scl_SetDriveMode(uint8 mode);
#endif
void    Slave_scl_SetInterruptMode(uint16 position, uint16 mode);
uint8   Slave_scl_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Slave_scl_Sleep(void); 
void Slave_scl_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Slave_scl__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Slave_scl_DRIVE_MODE_BITS        (3)
    #define Slave_scl_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Slave_scl_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Slave_scl_SetDriveMode() function.
         *  @{
         */
        #define Slave_scl_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Slave_scl_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Slave_scl_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Slave_scl_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Slave_scl_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Slave_scl_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Slave_scl_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Slave_scl_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Slave_scl_MASK               Slave_scl__MASK
#define Slave_scl_SHIFT              Slave_scl__SHIFT
#define Slave_scl_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Slave_scl_SetInterruptMode() function.
     *  @{
     */
        #define Slave_scl_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Slave_scl_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Slave_scl_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Slave_scl_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Slave_scl__SIO)
    #define Slave_scl_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Slave_scl__PC) && (CY_PSOC4_4200L)
    #define Slave_scl_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Slave_scl_USBIO_DISABLE              ((uint32)(~Slave_scl_USBIO_ENABLE))
    #define Slave_scl_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Slave_scl_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Slave_scl_USBIO_ENTER_SLEEP          ((uint32)((1u << Slave_scl_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Slave_scl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Slave_scl_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Slave_scl_USBIO_SUSPEND_SHIFT)))
    #define Slave_scl_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Slave_scl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Slave_scl_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Slave_scl__PC)
    /* Port Configuration */
    #define Slave_scl_PC                 (* (reg32 *) Slave_scl__PC)
#endif
/* Pin State */
#define Slave_scl_PS                     (* (reg32 *) Slave_scl__PS)
/* Data Register */
#define Slave_scl_DR                     (* (reg32 *) Slave_scl__DR)
/* Input Buffer Disable Override */
#define Slave_scl_INP_DIS                (* (reg32 *) Slave_scl__PC2)

/* Interrupt configuration Registers */
#define Slave_scl_INTCFG                 (* (reg32 *) Slave_scl__INTCFG)
#define Slave_scl_INTSTAT                (* (reg32 *) Slave_scl__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Slave_scl_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Slave_scl__SIO)
    #define Slave_scl_SIO_REG            (* (reg32 *) Slave_scl__SIO)
#endif /* (Slave_scl__SIO_CFG) */

/* USBIO registers */
#if !defined(Slave_scl__PC) && (CY_PSOC4_4200L)
    #define Slave_scl_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Slave_scl_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Slave_scl_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Slave_scl_DRIVE_MODE_SHIFT       (0x00u)
#define Slave_scl_DRIVE_MODE_MASK        (0x07u << Slave_scl_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Slave_scl_H */


/* [] END OF FILE */
