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
uint8 VolPlus,Mute,VolReduce;
UNION_TOUCH_LIN touch_lin;
UNION_MECH_LIN mech_lin;

uint8 TouchKeyCount,TouchKeyBuf,TouchKeyLock;


uint8 I2CWriteBuf[I2C_LEN];
uint8 I2CReadBuf[1];

CapSense_1_RAM_WD_BUTTON_STRUCT* CapSense_WD_Pointer=(CapSense_1_RAM_WD_BUTTON_STRUCT*)&CapSense_1_dsRam.wdgtList;
CapSense_1_RAM_SNS_STRUCT*       CapSense_SNS_Pointer=(CapSense_1_RAM_SNS_STRUCT*)&CapSense_1_dsRam.snsList;

int16 ADC_Value;
uint8 ResTime;

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
        ADC_Check();
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
        I2C_Date_Pro();
    }

    if(DEF_TICK_50mS == 1)
    {
        DEF_TICK_50mS = 0;
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
        
        touch_key.Key.Return=CapSense_IsWidgetActive_bit(CapSense_1_RET_WDGT_ID);
        touch_key.Key.Up=CapSense_IsWidgetActive_bit(CapSense_1_UP_WDGT_ID);
        touch_key.Key.Down=CapSense_IsWidgetActive_bit(CapSense_1_DOWN_WDGT_ID);
        touch_key.Key.SeekReduce=CapSense_IsWidgetActive_bit(CapSense_1_SEEKREDUCE_WDGT_ID);
        touch_key.Key.OK=CapSense_IsWidgetActive_bit(CapSense_1_OK_WDGT_ID);
        touch_key.Key.SeekPlus=CapSense_IsWidgetActive_bit(CapSense_1_SEEKPLUS_WDGT_ID);
        
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
                        if(i==3||i==4||i==2)
                        {
                            Force_Value=AF_sampling();
                            if(Force_Value>=250)
                            {
                                TouchKeyLock=1;
                                KeytoLin(touch_lin.Date,short_press,TouchKey);
                            }
                        }
                        else
                        {
                            Force_Value=AF_sampling();
                            if(Force_Value>=800)
                            {
                                TouchKeyLock=1;
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
//            ResTime++;
//            if(ResTime>=15)
//            {
//                ResTime=0;
//                WriteCommand(NIAddr,RestReg,Rest);
//            }
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
    static uint8 VolPlus_cnt,Mute_cnt,VolReduce_cnt;
    static uint8 VolPlus_lock,Mute_lock,VolReduce_lock;
    VolPlus=VolPlus_in_Read();
    Mute=Mute_in_Read();
    VolReduce=VolReduce_in_Read();
    if(KeyNumCheck())
    {
        VolPlus_cnt=0;
        Mute_cnt=0;
        VolReduce_cnt=0;
    }
    /*VolPlus*/
    if(VolPlus==0&&!VolPlus_lock)
    {
        VolPlus_cnt++;
        if(VolPlus_cnt>=2)
        {
            VolPlus_cnt=2;
            VolPlus_lock=1;
            mech_lin.Lin.VolPlus=1;
        }
    }
//    else if(VolPlus==0&&VolPlus_lock)
//    {
//        VolPlus_cnt++;
//        if(VolPlus_cnt>=25)
//        {
//            VolPlus_cnt=25;
//            mech_lin.Lin.VolPlus=2;
//        }
//    }
    else if(VolPlus)
    {
        VolPlus_cnt=0;
        VolPlus_lock=0;
        mech_lin.Lin.VolPlus=0;
    }
    /*Mute*/
    if(Mute==0&&!Mute_lock)
    {
        Mute_cnt++;
        if(Mute_cnt>=2)
        {
            Mute_cnt=2;
            Mute_lock=1;
            mech_lin.Lin.Mute=1;
        }
    }
//    else if(Mute==0&&Mute_lock)
//    {
//        Mute_cnt++;
//        if(Mute_cnt>=25)
//        {
//            Mute_cnt=25;
//            mech_lin.Lin.Mute=2;
//        }
//    }
    else if(Mute)
    {
        Mute_cnt=0;
        Mute_lock=0;
        mech_lin.Lin.Mute=0;
    }
    /*VolReduce*/
    if(VolReduce==0&&!VolReduce_lock)
    {
        VolReduce_cnt++;
        if(VolReduce_cnt>=2)
        {
            VolReduce_cnt=2;
            VolReduce_lock=1;
            mech_lin.Lin.VolReduce=1;
        }
    }
//    else if(VolReduce==0&&VolReduce_lock)
//    {
//        VolReduce_cnt++;
//        if(VolReduce_cnt>=25)
//        {
//            VolReduce_cnt=25;
//            mech_lin.Lin.VolReduce=2;
//        }
//    }
    else if(VolReduce)
    {
        VolReduce_cnt=0;
        VolReduce_lock=0;
        mech_lin.Lin.VolReduce=0;
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
    if(!VolPlus)
    {
        KeySum++;
    }
    if(!Mute)
    {
        KeySum++;
    }
    if(!VolReduce)
    {
        KeySum++;
    }
    if(KeySum>=2)
    {
        return 1;
    }
    return 0;
}

void I2C_Date_Pro()
{
    int i=0;
    for(i=0;i<6;i++)
    {
        I2CWriteBuf[i]=touch_lin.Date[i];
    }
    for(;i<9;i++)
    {
        I2CWriteBuf[i]=mech_lin.Date[i-6];
    }
    for(i=0;i<6;i++)
    {
        I2CWriteBuf[i*2+10]=CapSense_SNS_Pointer[i].bsln[0]>>8;
        I2CWriteBuf[i*2+1+10]=(uint8)(CapSense_SNS_Pointer[i].bsln[0]);
    }
    for(i=0;i<6;i++)
    {
        I2CWriteBuf[i*2+22]=CapSense_SNS_Pointer[i].diff>>8;
        I2CWriteBuf[i*2+1+22]=(uint8)CapSense_SNS_Pointer[i].diff;
    }
    I2CWriteBuf[34]=ReadCommand(addr,0x03);
    I2CWriteBuf[35]=ReadCommand(addr,0x04);
}

void I2C_task()
{
    if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_RD_CMPLT))
        {
            /* Clear slave read buffer and status */
            I2C_Slave_I2CSlaveClearReadBuf();
            (void) I2C_Slave_I2CSlaveClearReadStatus();      
        }
    if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_WR_CMPLT))
    {
        /* Clear slave write buffer and status */
        I2C_Slave_I2CSlaveClearWriteBuf();
        (void)I2C_Slave_I2CSlaveClearWriteStatus();
    }
}

void ADC_Check()
{
    if(ADC_SAR_Seq_1_IsEndConversion(ADC_SAR_Seq_1_WAIT_FOR_RESULT)==1)
    {
        ADC_Value=ADC_SAR_Seq_1_GetResult16(0);
        if(ADC_Value>Vol_16)
        {
            I2CWriteBuf[ADC_index]=0x02;
        }
        else if(ADC_Value<Vol_9)
        {
            I2CWriteBuf[ADC_index]=0x03;
        }
        else
        {
            I2CWriteBuf[ADC_index]=0;
        }
    }
}

void SleepMode()
{
    if(I2CReadBuf[0])
    {
        I2CReadBuf[0]=0;
        I2C_Slave_Sleep();
        ADC_SAR_Seq_1_Stop();
        AF_stop();
        CapSense_1_Stop();
        I2C_1_Stop();
        CySysPmDeepSleep();
        CySoftwareReset();
        I2C_Slave_Wakeup();
    }
 }










/* [] END OF FILE */
