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

#include "I2C.h"

uint32 WriteCommand(uint8 slvadrs,uint8 subaddrs,uint8 cmd)
{
    uint8  txbuffer[2u];
    uint32 status = TRANSFER_ERROR;

    /* Initialize buffer with packet */
    txbuffer[0u] = subaddrs;
    txbuffer[1u] = cmd;

    (void) I2C_1_I2CMasterClearStatus();
    
    /* Start I2C write and check status*/
    if(I2C_1_I2C_MSTR_NO_ERROR == I2C_1_I2CMasterWriteBuf(slvadrs,
                                    txbuffer, 2u,
                                    I2C_1_I2C_MODE_COMPLETE_XFER))
    {
        /*If I2C write started without errors, 
        / wait until I2C Master completes write transfer 
        */
        while (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT))
        {
            /* Wait */
        }
        
        /* Display transfer status */
        if (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_ERR_XFER))
        {
            /* Check if all bytes was written */
            if (I2C_1_I2CMasterGetWriteBufSize() == 2u)
            {
                status = TRANSFER_CMPLT;
            }
        }
    }

    return (status);
}

uint32 ReadCommand(uint8 slvadrs,uint8 subaddrs)
{
    uint8 state;
    Write_Read_Command(slvadrs,subaddrs);
    state=Read_Command(slvadrs);
    return state;
}


uint32 Write_Read_Command(uint8 slvadrs,uint8 subaddrs)
{
    uint8  txbuffer[1u];
    uint32 status = TRANSFER_ERROR;

    /* Initialize buffer with packet */
    txbuffer[0u] = subaddrs;

    (void) I2C_1_I2CMasterClearStatus();
    
    /* Start I2C write and check status*/
    if(I2C_1_I2C_MSTR_NO_ERROR == I2C_1_I2CMasterWriteBuf(slvadrs,
                                    txbuffer, 1u,
                                    I2C_1_I2C_MODE_COMPLETE_XFER))
    {
        /*If I2C write started without errors, 
        / wait until I2C Master completes write transfer 
        */
        while (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT))
        {
            /* Wait */
        }
        
        /* Display transfer status */
        if (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_ERR_XFER))
        {
            /* Check if all bytes was written */
            if (I2C_1_I2CMasterGetWriteBufSize() == 1u)
            {
                status = TRANSFER_CMPLT;
            }
        }
    }

    return (status);
}

uint32 Read_Command(uint8 slvadrs)
{
    uint8  rxbuffer[1u];

    (void) I2C_1_I2CMasterClearStatus();
    
    
    if(I2C_1_I2C_MSTR_NO_ERROR == I2C_1_I2CMasterReadBuf(slvadrs,
                                    rxbuffer, 1u,
                                    I2C_1_I2C_MODE_COMPLETE_XFER))
    {

        while (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT))
        {
            
        }
        
        
        if (0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_ERR_XFER))
        {
            
            if (I2C_1_I2CMasterGetReadBufSize() == 1u)
            {

            }
        }
    }

    return (rxbuffer[0]);
}



/* [] END OF FILE */
