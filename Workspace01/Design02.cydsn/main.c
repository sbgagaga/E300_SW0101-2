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
    ADC_SAR_Seq_1_Start();
    ADC_SAR_Seq_1_StartConvert();
    CapSense_1_Start();
    I2C_Slave_I2CSlaveInitReadBuf(I2CWriteBuf,I2C_LEN);
    I2C_Slave_I2CSlaveInitWriteBuf(I2CReadBuf,1);
    Timer_Init();
    I2C_Slave_Start();
    I2C_1_Start();  
    CyDelay(10);
    AF_Init();

    for(;;)
    {
        /* Place your application code here. */
        Apply();
        I2C_task();
        SleepMode();
    }
}

void Timer_Init()
{
    isr_1_StartEx(TimeBase_OnInterrupt);
    Timer_1_Start();
}

/* [] END OF FILE */
