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
#include "Apply.h"
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_BEEP_Start();
    I2C_1_Start(); 
    Timer_Init();
    Lin_Init();
    AF_Init();//需要修改
    CapSense_1_Start();

    for(;;)
    {
        /* Place your application code here. */
        Apply();
        LIN_task();
    }
}

void Timer_Init()
{
    isr_1_StartEx(TimeBase_OnInterrupt);
    Timer_1_Start();
}

void Lin_Init()
{
    unsigned int delay=250;
    if(0u != l_sys_init())
    {
        //initialize failed
    }
    if(0u != l_ifc_init(LIN_1_IFC_HANDLE))
    {
        //initialize failed
    }
    ld_init(LIN_1_IFC_HANDLE);
    LIN_EN_Write(0);
    while(delay--);
    LIN_EN_Write(1);
}

/* [] END OF FILE */
