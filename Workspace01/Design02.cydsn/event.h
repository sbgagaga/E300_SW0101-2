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

#ifndef __EVENT_H__
#define __EVENT_H__
    
#include <project.h>
    
#define DEF_TICK_2mS    (struct_TimerTick.flag_2mStick  )
#define DEF_TICK_5mS    (struct_TimerTick.flag_5mStick  )
#define DEF_TICK_10mS   (struct_TimerTick.flag_10mStick )
#define DEF_TICK_15mS   (struct_TimerTick.flag_15mStick )
#define DEF_TICK_20mS   (struct_TimerTick.flag_20mStick )
#define DEF_TICK_50mS   (struct_TimerTick.flag_50mStick )
#define DEF_TICK_100mS  (struct_TimerTick.flag_100mStick)

    
typedef struct
{
    uint8_t flag_2mStick:1;
    uint8_t flag_5mStick:1;
    uint8_t flag_10mStick:1;
    uint8_t flag_15mStick:1;
    uint8_t flag_20mStick:1;
    uint8_t flag_50mStick:1;
    uint8_t flag_100mStick:1;
    uint8_t flag_200mStick:1;
}TIMERTICK_STRUCT;



extern TIMERTICK_STRUCT struct_TimerTick;



void TimeBase_OnInterrupt(void);


#endif

/* [] END OF FILE */
