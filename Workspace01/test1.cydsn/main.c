/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    EZI2C_Start();
    EZI2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam), sizeof(CapSense_dsRam),(uint8 *)&CapSense_dsRam);
    CapSense_Start();

    for(;;)
    {
        /* Place your application code here. */
        if(CapSense_IsBusy() == CapSense_NOT_BUSY)
        {
            CapSense_ProcessAllWidgets();
            
            CapSense_RunTuner();
            
            CapSense_ScanAllWidgets();
        }
    }
}

/* [] END OF FILE */
