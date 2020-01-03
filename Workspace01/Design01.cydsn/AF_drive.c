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

#include "AF_drive.h"

uint8 reg_init[REG_INIT][2] = {
	{0x00, 0xb5},
	{0x01, 0x55},
	{0x07, 0x02},
	{0x08, 0x82},
	{0x09, 0xc0},
	{0x0a, 0x30}, /*NEED TESTING*/
	{0x0b, 0x05}, /*NEED TESTING*/
	{0x0c, 0x08},
	{0x0d, 0xc5},
	{0x0e, 0x95},
	{0x0f, 0x01}
};

void AF_Init()
{
    uint8 buf;
    addr=0x44;
    for(int i=0;i<4;i++)
    {
        if(ReadCommand(addr,0x00)!=addr)
        {
            break;
        }
        addr++;
    }
    buf=ReadCommand(addr,0x00);
    //CyDelay(5);
    buf=(buf&~0x01)|0x00;
    WriteCommand(addr,0x00,buf);
    //CyDelay(5);
    for(int i=0;i<REG_INIT;i++)
    {
        WriteCommand(addr,reg_init[i][0],reg_init[i][1]);
        //CyDelay(5);
    }
    buf=ReadCommand(addr,0x00);
    //CyDelay(5);
    buf=(buf&~0x01)|0x01;
    WriteCommand(addr,0x00,buf);
    //CyDelay(5);
}

uint16 AF_sampling()
{
    uint16 temp1,temp2,temp3;
    addr=0x44;
    for(int i=0;i<4;i++)
    {
        if(ReadCommand(addr,0x00)!=addr)
        {
            break;
        }
        addr++;
    }
    temp1=ReadCommand(addr,0x03);
    temp2=ReadCommand(addr,0x04);
    temp3=((temp1<<8)|temp2)>>4;
    if((temp3&0x800)==0)
    {
        return (temp3);
    }
    else
    {
        return 0;
    }
}

void AF_stop()
{
    uint8 buf;
    addr=0x44;
    for(int i=0;i<4;i++)
    {
        if(ReadCommand(addr,0x00)!=addr)
        {
            break;
        }
        addr++;
    }
    buf=ReadCommand(addr,0x00);
    buf=(buf&~0x01)|0x00;
    WriteCommand(addr,0x00,buf);
}

 



/* [] END OF FILE */
