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
#include "diagnose.h"

uint8 Diag_Sendbuf[128]={0};
uint8 Diag_Revbuf[128]={0};
uint16 Diag_len;
uint8 Diag_NAD;
uint8 Diag_flag;

BUFFTER_TYPE RX_Buffer={128,0,Diag_Revbuf};
BUFFTER_TYPE TX_Buffer={128,0,Diag_Sendbuf};

const uint8 SW_DID_FF00[10]={"SW0101-2  "};
const uint8 HW_DID_FF01[10]={"HW01      "};

CapSense_1_RAM_WD_BUTTON_STRUCT* CapSense_WD_Pointer=(CapSense_1_RAM_WD_BUTTON_STRUCT*)&CapSense_1_dsRam.wdgtList;
CapSense_1_RAM_SNS_STRUCT*       CapSense_SNS_Pointer=(CapSense_1_RAM_SNS_STRUCT*)&CapSense_1_dsRam.snsList;

void DiagRev()
{
    static uint8 state;
    switch(state)
    {
        case 0:
            ld_receive_message_LIN_1(&Diag_len,&Diag_NAD,RX_Buffer.buffer);
            state++;
            break;
        case 1:
            if(ld_rx_status_LIN_1()==LD_COMPLETED&&Diag_len>0)
            {
                Diag_flag=1;
                state++;
            }
            break;
        case 2:
            if(!Diag_flag)state=0;
            break;
    }
}

void DiagPro()
{
    uint8 LIN_SID;
    uint16 LIN_DID;
    if(Diag_flag)
    {
        Diag_flag=0;
        LIN_SID=RX_Buffer.buffer[0];
        TX_Buffer.pointer=0;
        RX_Buffer.pointer=0;
        switch(LIN_SID)
        {
            case 0x22:
            LIN_DID=((uint16)RX_Buffer.buffer[1]<<8)+RX_Buffer.buffer[2];
            LIN_SID+=0x40;
            FillInBuffer(&TX_Buffer,&LIN_SID,1);
            FillInBuffer(&TX_Buffer,RX_Buffer.buffer+1,2);
            LIN_SID22(LIN_DID);
            ld_send_message_LIN_1(TX_Buffer.pointer,DiagNAD,TX_Buffer.buffer);
            break;
            case 0x23:
            LIN_SID+=0x40;
            FillInBuffer(&TX_Buffer,&LIN_SID,1);
            LIN_SID23();
            ld_send_message_LIN_1(TX_Buffer.pointer,DiagNAD,TX_Buffer.buffer);
            break;
            case 0x24:
            LIN_SID+=0x40;
            FillInBuffer(&TX_Buffer,&LIN_SID,1);
            LIN_SID24();
            ld_send_message_LIN_1(TX_Buffer.pointer,DiagNAD,TX_Buffer.buffer);
            case 0x25:
            LIN_SID+=0x40;
            FillInBuffer(&TX_Buffer,&LIN_SID,1);
            LIN_SID25();
            ld_send_message_LIN_1(TX_Buffer.pointer,DiagNAD,TX_Buffer.buffer);
        }
    }
}

void LIN_SID22(uint16 DID)
{
    switch(DID)
    {
        case 0xFF00:
        FillInBuffer(&TX_Buffer,(uint8 *)SW_DID_FF00,10);
        break;
        case 0xFF01:
        FillInBuffer(&TX_Buffer,(uint8 *)HW_DID_FF01,10);
        break;
    }
}

void LIN_SID23()
{
    uint8 baselinebuf[CapSense_1_TOTAL_SCAN_SLOTS*2];
    for(uint8 i=0;i<(CapSense_1_TOTAL_SCAN_SLOTS);i++)
    {
        baselinebuf[i*2]=CapSense_SNS_Pointer[i].bsln[0]>>8;
        baselinebuf[i*2+1]=(uint8)(CapSense_SNS_Pointer[i].bsln[0]);
    }
    FillInBuffer(&TX_Buffer,baselinebuf,sizeof(baselinebuf)/sizeof(baselinebuf[0]));
    FillInBuffer(&TX_Buffer,baseline,sizeof(baseline)/sizeof(baseline[0]));
}

void LIN_SID24()
{
    uint8 signlinebuf[CapSense_1_TOTAL_SCAN_SLOTS*2];
    for(uint8 i=0;i<(CapSense_1_TOTAL_SCAN_SLOTS);i++)
    {
        signlinebuf[i*2]=CapSense_SNS_Pointer[i].diff>>8;
        signlinebuf[i*2+1]=(uint8)CapSense_SNS_Pointer[i].diff;
    }
    FillInBuffer(&TX_Buffer,signlinebuf,sizeof(signlinebuf)/sizeof(signlinebuf[0]));
    FillInBuffer(&TX_Buffer,signline,sizeof(signline)/sizeof(signline[0]));
}

void LIN_SID25()
{
    uint8 MasterPressBuf[2];
    MasterPressBuf[0]=ReadCommand(addr,0x03);
    MasterPressBuf[1]=ReadCommand(addr,0x04);
    FillInBuffer(&TX_Buffer,MasterPressBuf,sizeof(MasterPressBuf)/sizeof(MasterPressBuf[0]));
    FillInBuffer(&TX_Buffer,SlavePressValue,sizeof(SlavePressValue)/sizeof(SlavePressValue[0]));
}

void FillInBuffer(BUFFTER_TYPE * buffer,uint8 * date,uint8 len)
{
    if(buffer->pointer+len>buffer->length)
    {
        return;
    }
    for(uint8 i=0;i<len;i++)
    {
        buffer->buffer[buffer->pointer++]=date[i];
    }
}


/* [] END OF FILE */
