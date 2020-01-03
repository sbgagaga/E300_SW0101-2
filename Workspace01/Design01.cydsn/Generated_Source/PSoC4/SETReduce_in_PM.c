/*******************************************************************************
* File Name: SETReduce_in.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "SETReduce_in.h"

static SETReduce_in_BACKUP_STRUCT  SETReduce_in_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: SETReduce_in_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet SETReduce_in_SUT.c usage_SETReduce_in_Sleep_Wakeup
*******************************************************************************/
void SETReduce_in_Sleep(void)
{
    #if defined(SETReduce_in__PC)
        SETReduce_in_backup.pcState = SETReduce_in_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            SETReduce_in_backup.usbState = SETReduce_in_CR1_REG;
            SETReduce_in_USB_POWER_REG |= SETReduce_in_USBIO_ENTER_SLEEP;
            SETReduce_in_CR1_REG &= SETReduce_in_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SETReduce_in__SIO)
        SETReduce_in_backup.sioState = SETReduce_in_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        SETReduce_in_SIO_REG &= (uint32)(~SETReduce_in_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: SETReduce_in_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to SETReduce_in_Sleep() for an example usage.
*******************************************************************************/
void SETReduce_in_Wakeup(void)
{
    #if defined(SETReduce_in__PC)
        SETReduce_in_PC = SETReduce_in_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            SETReduce_in_USB_POWER_REG &= SETReduce_in_USBIO_EXIT_SLEEP_PH1;
            SETReduce_in_CR1_REG = SETReduce_in_backup.usbState;
            SETReduce_in_USB_POWER_REG &= SETReduce_in_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SETReduce_in__SIO)
        SETReduce_in_SIO_REG = SETReduce_in_backup.sioState;
    #endif
}


/* [] END OF FILE */
