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
#ifndef __APPLY__
#define __APPLY__
    
#include "project.h"
#include "event.h"
#include "I2C.h"
#include "AF_drive.h"

/*触摸按键*/
typedef struct
{
    uint8 Return:1;
    uint8 Up:1;
    uint8 Down:1;
    uint8 SeekReduce:1;
    uint8 OK:1;
    uint8 SeekPlus:1;
    uint8 reserve:2;
    
}STRUCT_TOUCH_KEY;    

typedef union
{
    uint8 Date;
    STRUCT_TOUCH_KEY Key;
    
}UNION_TOUCH_KEY;

/*触摸LIN*/
typedef struct
{
    uint8 Return;
    uint8 Source;
    uint8 Down;
    uint8 SeekReduce;
    uint8 OK;
    uint8 SeekPlus;
    
}STRUCT_TOUCH_LIN;

typedef union
{
    uint8 Date[6];
    STRUCT_TOUCH_LIN Lin;
    
}UNION_TOUCH_LIN;
    
/*机械LIN*/
typedef struct
{
    uint8 VolPlus;
    uint8 Mute;
    uint8 VolReduce;
    
}STRUCT_MECH_LIN;

typedef union
{
    uint8 Date[3];
    STRUCT_MECH_LIN Lin;
    
}UNION_MECH_LIN;


#define short_press   1
#define long_press   2
#define no_press    0
#define I2C_Slave_dress 0x08
#define I2C_LEN 36
#define Vol_16   1910
#define Vol_9   1075
#define ADC_index   9
#define TouchKey 6
#define MechKey 3
#define RestReg         0x0F
#define Rest            0x01

extern uint8 I2CWriteBuf[I2C_LEN];
extern uint8 I2CReadBuf[1];

void TouchKeyScan();
void MechKeyScan();
uint8 KeyNumCheck();
void Timer_Init();
void Apply();       
void KeytoLin(unsigned char *buf,uint8 key_date,uint8 key_type);
void I2C_task();
void I2C_Date_Pro();
void ADC_Check();
void Lin_Init();
void SleepMode();

uint32 CapSense_IsWidgetActive_bit(uint32 widgetId);

    
#endif
/* [] END OF FILE */
