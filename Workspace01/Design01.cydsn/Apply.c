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

#include "Apply.h"

UNION_TOUCH_KEY touch_key;
uint8 RESPlus,Crusie,SETReduce;
UNION_TOUCH_LIN touch_lin;
UNION_MECH_LIN mech_lin;

uint8 baseline[12];
uint8 signline[12];
uint8 SlavePressValue[2];

uint8 TouchKeyCount,TouchKeyBuf,TouchKeyLock;

uint8 I2CReadBuf[I2C_LEN]={0};
uint8 I2CWriteBuf[1]={0};
uint8 SlaveBuf[I2C_LEN]={0},LastReadBuf[I2C_LEN]={0};

uint8 BEEP_Flag;
uint8 ResTime;

uint8 sleep_flag;
uint16 sleep_time;

void Apply()
{
    if(DEF_TICK_2mS == 1)
    {	 
        DEF_TICK_2mS = 0;
    }

    if(DEF_TICK_5mS == 1)
    {
        DEF_TICK_5mS = 0;
    }

    if(DEF_TICK_10mS == 1)
    {
        DEF_TICK_10mS = 0;
        SleepCheck();
        I2C_task();
    }
    
    if(DEF_TICK_15mS == 1)
    {
        DEF_TICK_15mS = 0;
    }
    
    if(DEF_TICK_20mS == 1)
    {
        DEF_TICK_20mS = 0;
        TouchKeyScan();
        MechKeyScan();
    }

    if(DEF_TICK_50mS == 1)
    {
        DEF_TICK_50mS = 0;
        Beep_EN();
    }

    if(DEF_TICK_100mS == 1)
    {
        DEF_TICK_100mS = 0;
    }	    
}

uint32 CapSense_IsWidgetActive_bit(uint32 widgetId)
{
    return (CapSense_1_IsWidgetActive(widgetId)>>widgetId);
}

void TouchKeyScan()
{
    uint16 Force_Value;
    if(CapSense_1_NOT_BUSY == CapSense_1_IsBusy())//检查传感器是否忙碌
    {
        CapSense_1_ProcessAllWidgets();       //处理所有传感器
        
        touch_key.Key.Menu=CapSense_IsWidgetActive_bit(CapSense_1_MENU_WDGT_ID);
        touch_key.Key.Mode=CapSense_IsWidgetActive_bit(CapSense_1_MODE_WDGT_ID);
        touch_key.Key.ADAS=CapSense_IsWidgetActive_bit(CapSense_1_ADAS_WDGT_ID);
        touch_key.Key.Answer=CapSense_IsWidgetActive_bit(CapSense_1_ANSWER_WDGT_ID);
        touch_key.Key.Speech=CapSense_IsWidgetActive_bit(CapSense_1_SPEECH_WDGT_ID);
        touch_key.Key.DIST=CapSense_IsWidgetActive_bit(CapSense_1_DIST_WDGT_ID);
        
        if(KeyNumCheck())
        {
            touch_key.Date=0;
        }
        
        if(touch_key.Date!=0&&touch_key.Date==TouchKeyBuf&&!TouchKeyLock)
        {
            ResTime=0;
            TouchKeyCount++;
            if(TouchKeyCount>=2)
            {
                TouchKeyCount=2;
                for(uint8 i=0,mask=0x20; mask!=0; mask>>=1,i++)
                {
                    if(mask&touch_key.Date)
                    {
                        if(i==0||i==1)
                        {
                            Force_Value=AF_sampling();
                            if(Force_Value>=250)
                            {
                                TouchKeyLock=1;
                                BEEP_Flag=1;
                                KeytoLin(touch_lin.Date,short_press,TouchKey);
                            }
                        }
                        else if(i==5)
                        {
                            Force_Value=AF_sampling();
                            if(Force_Value>=200)
                            {
                                TouchKeyLock=1;
                                BEEP_Flag=1;
                                KeytoLin(touch_lin.Date,short_press,TouchKey);
                            }
                        }
                        else
                        {
                            Force_Value=AF_sampling();
                            if(Force_Value>=450)
                            {
                                TouchKeyLock=1;
                                BEEP_Flag=1;
                                KeytoLin(touch_lin.Date,short_press,TouchKey);
                            }
                        }
                        break;
                    }
                }
            }
        }
//        else if(touch_key.Date!=0&&touch_key.Date==TouchKeyBuf&&TouchKeyLock)
//        {
//            ResTime=0;
//            TouchKeyCount++;
//            if(TouchKeyCount>=25)
//            {
//                TouchKeyCount=25;
//                KeytoLin(touch_lin.Date,long_press,TouchKey);
//            }
//        }
        else if(touch_key.Date==0||touch_key.Date!=TouchKeyBuf)
        {
            TouchKeyCount=0;
            TouchKeyLock=0;
            KeytoLin(touch_lin.Date,no_press,TouchKey);
        }
        TouchKeyBuf=touch_key.Date;
        CapSense_1_ScanAllWidgets();    //扫描所有的传感器
    }
}

void MechKeyScan()
{
    static uint8 RESPlus_cnt,Crusie_cnt,SETReduce_cnt;
    static uint8 RESPlus_lock,Crusie_lock,SETReduce_lock;
    RESPlus=RESPlus_in_Read();
    Crusie=Crusie_in_Read();
    SETReduce=SETReduce_in_Read();
    if(KeyNumCheck())
    {
        RESPlus_cnt=0;
        Crusie_cnt=0;
        SETReduce_cnt=0;
    }
    /*RESPlus*/
    if(RESPlus==0&&!RESPlus_lock)
    {
        RESPlus_cnt++;
        if(RESPlus_cnt>=2)
        {
            RESPlus_cnt=2;
            BEEP_Flag=1;
            RESPlus_lock=1;
            mech_lin.Lin.RESPlus=1;
        }
    }
//    else if(RESPlus==0&&RESPlus_lock)
//    {
//        RESPlus_cnt++;
//        if(RESPlus_cnt>=25)
//        {
//            RESPlus_cnt=25;
//            mech_lin.Lin.RESPlus=2;
//        }
//    }
    else if(RESPlus)
    {
        RESPlus_cnt=0;
        RESPlus_lock=0;
        mech_lin.Lin.RESPlus=0;
    }
    /*Crusie*/
    if(Crusie==0&&!Crusie_lock)
    {
        Crusie_cnt++;
        if(Crusie_cnt>=2)
        {
            Crusie_cnt=2;
            BEEP_Flag=1;
            Crusie_lock=1;
            mech_lin.Lin.Crusie=1;
        }
    }
//    else if(Crusie==0&&Crusie_lock)
//    {
//        Crusie_cnt++;
//        if(Crusie_cnt>=25)
//        {
//            Crusie_cnt=25;
//            mech_lin.Lin.Crusie=2;
//        }
//    }
    else if(Crusie)
    {
        Crusie_cnt=0;
        Crusie_lock=0;
        mech_lin.Lin.Crusie=0;
    }
    /*SETReduce*/
    if(SETReduce==0&&!SETReduce_lock)
    {
        SETReduce_cnt++;
        if(SETReduce_cnt>=2)
        {
            SETReduce_cnt=2;
            BEEP_Flag=1;
            SETReduce_lock=1;
            mech_lin.Lin.SETReduce=1;
        }
    }
//    else if(SETReduce==0&&SETReduce_lock)
//    {
//        SETReduce_cnt++;
//        if(SETReduce_cnt>=25)
//        {
//            SETReduce_cnt=25;
//            mech_lin.Lin.SETReduce=2;
//        }
//    }
    else if(SETReduce)
    {
        SETReduce_cnt=0;
        SETReduce_lock=0;
        mech_lin.Lin.SETReduce=0;
    }
}

void KeytoLin(unsigned char *buf,uint8 key_date,uint8 key_type)
{
    uint8 index=0;
    if(key_date==0)
    {
        for(uint8 i=0;i<key_type;i++)
        {
            buf[i]=key_date;
        }
        return;
    }
    for(uint8 mask=0x01; mask!=(0x01<<key_type); mask<<=1)
    {
        if(touch_key.Date&mask)
        {
            buf[index]=key_date;
            return;
        }
        else
        {
           index++; 
        }
    }
}

uint8 KeyNumCheck()
{
    uint8 KeySum=0;
    for(uint8 mask=0x20; mask!=0; mask>>=1)
    {
        if(mask&touch_key.Date)
        {
            KeySum++;
            if(KeySum>=2) 
            {
                return 1;
            }
        }
    }
    if(!RESPlus)
    {
        KeySum++;
    }
    if(!Crusie)
    {
        KeySum++;
    }
    if(!SETReduce)
    {
        KeySum++;
    }
    if(KeySum>=2) 
    {
        return 1;
    }
    return 0; 
}

void I2C_task()
{
    uint8 I2C_temp,KeySum=0; 
    static uint8 I2C_cnt,same_flag,beep_flag;
    do
    {
        I2C_temp=I2C_1_I2CMasterReadBuf(I2C_Slave_dress, I2CReadBuf, I2C_LEN,
                                        I2C_1_I2C_MODE_COMPLETE_XFER);
    }while(I2C_temp!=I2C_1_I2C_MSTR_NO_ERROR);
    CyDelay(2);
    I2C_temp=I2C_1_I2CMasterClearStatus();
    if(I2C_temp&I2C_1_I2C_MSTAT_ERR_XFER)
    {
        return;
    }
    else
    {
        for(uint8 i=0;i<10;i++)
        {
            if(I2CReadBuf[i]!=LastReadBuf[i]&&I2CReadBuf[i]!=3)
            {
                I2C_cnt=0;
                same_flag=0;
                beep_flag=0;
                break;
            }
            else
            {
                same_flag=1;
            }
        }
        if(same_flag)
        {
            I2C_cnt++;
        }
        if(I2C_cnt>=5)
        {
            I2C_cnt=5;
            for(uint8 i=0;i<10;i++)
            {
                SlaveBuf[i]=LastReadBuf[i];
            }
            for(uint8 i=0;i<9;i++)
            {
                if(SlaveBuf[i]!=0&&SlaveBuf[i]!=0xFF&&SlaveBuf[i]!=2)
                {
                    KeySum++;
                }
            }
            if(KeySum!=0&&!beep_flag)
            {
                BEEP_Flag=1;
                beep_flag=1;
            }
            for(int i=0;i<12;i++)
            {
                baseline[i]=I2CReadBuf[10+i];
            }
            for(int i=0;i<12;i++)
            {
                signline[i]=I2CReadBuf[22+i];
            }
            SlavePressValue[0]=I2CReadBuf[34];
            SlavePressValue[1]=I2CReadBuf[35];
        }
        for(uint8 i=0;i<34;i++)
        {
            LastReadBuf[i]=I2CReadBuf[i];
        }
    }
}

void LIN_task()
{
    l_u8_wr_MFS_Menu_switch_signal(touch_lin.Date[0]);
    l_u8_wr_MFS_Mode_switch_signal(touch_lin.Date[1]);
    l_u8_wr_MFS_ADAS_switch_signal(touch_lin.Date[2]);
    l_u8_wr_MFS_Answer_switch_signal(touch_lin.Date[3]);
    l_u8_wr_MFS_Speech_switch_signal(touch_lin.Date[4]);
    l_u8_wr_MFS_DIST_switch_signal(touch_lin.Date[5]);
    l_u8_wr_MFS_RESPlus_switch_signal(mech_lin.Date[0]);
    l_u8_wr_MFS_Crusie_switch_signal(mech_lin.Date[1]);
    l_u8_wr_MFS_SETReduce_switch_signal(mech_lin.Date[2]);
    l_u8_wr_MFS_Return_switch_signal(SlaveBuf[0]);
    l_u8_wr_MFS_Up_switch_signal(SlaveBuf[1]);
    l_u8_wr_MFS_Down_switch_signal(SlaveBuf[2]);
    l_u8_wr_MFS_SeekReduce_switch_signal(SlaveBuf[3]);
    l_u8_wr_MFS_OK_switch_signal(SlaveBuf[4]);
    l_u8_wr_MFS_SeekPlus_switch_signal(SlaveBuf[5]);
    l_u8_wr_MFS_VolPlus_switch_signal(SlaveBuf[6]);
    l_u8_wr_MFS_Mute_switch_signal(SlaveBuf[7]);
    l_u8_wr_MFS_VolReduce_switch_signal(SlaveBuf[8]);
    l_u8_wr_MFS_DiagInfoSW(SlaveBuf[9]);
    DiagRev();
    DiagPro();
}

void Beep_EN()
{
    static uint8 BEEP_cnt;
    if(BEEP_Flag)
    {
        PWM_BEEP_WriteCompare(250);
        BEEP_cnt++;
        if(BEEP_cnt>=3)
        {
            BEEP_cnt=0;
            BEEP_Flag=0;
            PWM_BEEP_WriteCompare(0);
        }
    }
}

void SleepCheck()
{
    uint8 state;
    state=l_ifc_read_status_LIN_1();
    if((state&0x02)&&(state&0x08)==0)
    {
        sleep_time=0;
        sleep_flag=0;
        SlaveGoSleep(sleep_flag);
    }
    else if(state&0x08)
    {
        sleep_flag=1;
        SlaveGoSleep(sleep_flag);
        SleepMode();
    }
    else if((state&0x02)==0)
    {
        sleep_time++;
        if(sleep_time>=1000)
        {
            sleep_flag=1;
            SlaveGoSleep(sleep_flag);
            SleepMode();
        }
    }
}

void SleepMode()
{
    if(sleep_flag)
    {
        LIN_EN_Write(0);
        LIN_1_SCB_rx_Sleep();
        LIN_1_SCB_tx_Sleep();
        LIN_1_SCB_rx_SetDriveMode(LIN_1_SCB_rx_DM_RES_UP);
        CyIntEnable(3);
        CyIntSetVector(3, &LINRx);
        LIN_1_SCB_rx_SetInterruptMode(LIN_1_SCB_rx_0_INTR, LIN_1_SCB_rx_INTR_FALLING);
        AF_stop();
        PWM_BEEP_Stop();
        I2C_1_Stop();
        CapSense_1_Stop();
        CySysPmDeepSleep();
        CySoftwareReset();
    }
}

void LINRx()
{
    LIN_1_SCB_rx_ClearInterrupt();
    LIN_1_SCB_tx_ClearInterrupt();
}

void SlaveGoSleep(uint8 state)
{
    uint8 I2C_temp;
    I2CWriteBuf[0]=state;
    do
    {
        I2C_temp = I2C_1_I2CMasterWriteBuf(I2C_Slave_dress, I2CWriteBuf, 
										1, I2C_1_I2C_MODE_COMPLETE_XFER);
    }
    while(I2C_temp != I2C_1_I2C_MSTR_NO_ERROR);
    while(I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_XFER_INP);
    I2C_temp = I2C_1_I2CMasterClearStatus();
}

/* [] END OF FILE */
