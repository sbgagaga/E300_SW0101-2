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
#ifndef __I2C__
#define __I2C__
    
#include "project.h"
    
/* Transfer statuses */
#define TRANSFER_CMPLT      (0x00u)
#define TRANSFER_ERROR      (0xFFu) 
/* Command statuses */
//#define STS_CMD_DONE        (0x00u)
//#define STS_CMD_FAIL        (0xFFu)
    
uint32 WriteCommand(uint8 slvadrs,uint8 subaddrs,uint8 cmd);
uint8 ReadReg (uint8 slvadrs,uint8 reg);
uint32 Read_Command(uint8 slvadrs);
uint32 Write_Read_Command(uint8 slvadrs,uint8 subaddrs);
uint32(ReadCommand)(uint8 slvadrs,uint8 subaddrs);
    
#endif
/* [] END OF FILE */
