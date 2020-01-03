/***************************************************************************//**
* \file Master_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Master.h"
#include "Master_PVT.h"

#if(Master_SCB_MODE_I2C_INC)
    #include "Master_I2C_PVT.h"
#endif /* (Master_SCB_MODE_I2C_INC) */

#if(Master_SCB_MODE_EZI2C_INC)
    #include "Master_EZI2C_PVT.h"
#endif /* (Master_SCB_MODE_EZI2C_INC) */

#if(Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC)
    #include "Master_SPI_UART_PVT.h"
#endif /* (Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(Master_SCB_MODE_UNCONFIG_CONST_CFG || \
   (Master_SCB_MODE_I2C_CONST_CFG   && (!Master_I2C_WAKE_ENABLE_CONST))   || \
   (Master_SCB_MODE_EZI2C_CONST_CFG && (!Master_EZI2C_WAKE_ENABLE_CONST)) || \
   (Master_SCB_MODE_SPI_CONST_CFG   && (!Master_SPI_WAKE_ENABLE_CONST))   || \
   (Master_SCB_MODE_UART_CONST_CFG  && (!Master_UART_WAKE_ENABLE_CONST)))

    Master_BACKUP_STRUCT Master_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: Master_Sleep
****************************************************************************//**
*
*  Prepares the Master component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the Master_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void Master_Sleep(void)
{
#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Master_SCB_WAKE_ENABLE_CHECK)
    {
        if(Master_SCB_MODE_I2C_RUNTM_CFG)
        {
            Master_I2CSaveConfig();
        }
        else if(Master_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Master_EzI2CSaveConfig();
        }
    #if(!Master_CY_SCBIP_V1)
        else if(Master_SCB_MODE_SPI_RUNTM_CFG)
        {
            Master_SpiSaveConfig();
        }
        else if(Master_SCB_MODE_UART_RUNTM_CFG)
        {
            Master_UartSaveConfig();
        }
    #endif /* (!Master_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        Master_backup.enableState = (uint8) Master_GET_CTRL_ENABLED;

        if(0u != Master_backup.enableState)
        {
            Master_Stop();
        }
    }

#else

    #if (Master_SCB_MODE_I2C_CONST_CFG && Master_I2C_WAKE_ENABLE_CONST)
        Master_I2CSaveConfig();

    #elif (Master_SCB_MODE_EZI2C_CONST_CFG && Master_EZI2C_WAKE_ENABLE_CONST)
        Master_EzI2CSaveConfig();

    #elif (Master_SCB_MODE_SPI_CONST_CFG && Master_SPI_WAKE_ENABLE_CONST)
        Master_SpiSaveConfig();

    #elif (Master_SCB_MODE_UART_CONST_CFG && Master_UART_WAKE_ENABLE_CONST)
        Master_UartSaveConfig();

    #else

        Master_backup.enableState = (uint8) Master_GET_CTRL_ENABLED;

        if(0u != Master_backup.enableState)
        {
            Master_Stop();
        }

    #endif /* defined (Master_SCB_MODE_I2C_CONST_CFG) && (Master_I2C_WAKE_ENABLE_CONST) */

#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Master_Wakeup
****************************************************************************//**
*
*  Prepares the Master component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the Master_Wakeup() function without first calling the 
*   Master_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void Master_Wakeup(void)
{
#if(Master_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Master_SCB_WAKE_ENABLE_CHECK)
    {
        if(Master_SCB_MODE_I2C_RUNTM_CFG)
        {
            Master_I2CRestoreConfig();
        }
        else if(Master_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Master_EzI2CRestoreConfig();
        }
    #if(!Master_CY_SCBIP_V1)
        else if(Master_SCB_MODE_SPI_RUNTM_CFG)
        {
            Master_SpiRestoreConfig();
        }
        else if(Master_SCB_MODE_UART_RUNTM_CFG)
        {
            Master_UartRestoreConfig();
        }
    #endif /* (!Master_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != Master_backup.enableState)
        {
            Master_Enable();
        }
    }

#else

    #if (Master_SCB_MODE_I2C_CONST_CFG  && Master_I2C_WAKE_ENABLE_CONST)
        Master_I2CRestoreConfig();

    #elif (Master_SCB_MODE_EZI2C_CONST_CFG && Master_EZI2C_WAKE_ENABLE_CONST)
        Master_EzI2CRestoreConfig();

    #elif (Master_SCB_MODE_SPI_CONST_CFG && Master_SPI_WAKE_ENABLE_CONST)
        Master_SpiRestoreConfig();

    #elif (Master_SCB_MODE_UART_CONST_CFG && Master_UART_WAKE_ENABLE_CONST)
        Master_UartRestoreConfig();

    #else

        if(0u != Master_backup.enableState)
        {
            Master_Enable();
        }

    #endif /* (Master_I2C_WAKE_ENABLE_CONST) */

#endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
