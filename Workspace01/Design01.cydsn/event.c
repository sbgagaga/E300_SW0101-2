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

#include "event.h"

uint16_t TimerTick;
TIMERTICK_STRUCT struct_TimerTick;

void TimeBase_OnInterrupt()
{
    TimerTick++;
	if(TimerTick % 2 == 0)
	{
		struct_TimerTick.flag_2mStick = 1;
	}       
	if(TimerTick % 5 == 0)
	{
		struct_TimerTick.flag_5mStick = 1;
	}
	if(TimerTick % 10 == 0)
	{
		struct_TimerTick.flag_10mStick = 1;
	}
    if(TimerTick % 15 == 0)
	{
		struct_TimerTick.flag_15mStick = 1;
	}
	if(TimerTick % 20 == 0)
	{
		struct_TimerTick.flag_20mStick = 1;
	}
	if(TimerTick %50 == 0)
	{
		struct_TimerTick.flag_50mStick = 1;
	}
	if(TimerTick % 100 == 0)
	{
		struct_TimerTick.flag_100mStick = 1;
		TimerTick=0;
	}
    Timer_1_ClearInterrupt(Timer_1_INTR_MASK_TC);
}

/* [] END OF FILE */
