/***************************************************************************//**
* \file Master_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_Master_H)
#define CY_SCB_BOOT_Master_H

#include "Master_PVT.h"

#if (Master_SCB_MODE_I2C_INC)
    #include "Master_I2C.h"
#endif /* (Master_SCB_MODE_I2C_INC) */

#if (Master_SCB_MODE_EZI2C_INC)
    #include "Master_EZI2C.h"
#endif /* (Master_SCB_MODE_EZI2C_INC) */

#if (Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC)
    #include "Master_SPI_UART.h"
#endif /* (Master_SCB_MODE_SPI_INC || Master_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define Master_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Master) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (Master_SCB_MODE_I2C_INC)
    #define Master_I2C_BTLDR_COMM_ENABLED     (Master_BTLDR_COMM_ENABLED && \
                                                            (Master_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Master_I2C_SLAVE_CONST))
#else
     #define Master_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (Master_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (Master_SCB_MODE_EZI2C_INC)
    #define Master_EZI2C_BTLDR_COMM_ENABLED   (Master_BTLDR_COMM_ENABLED && \
                                                         Master_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define Master_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (Master_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (Master_SCB_MODE_SPI_INC)
    #define Master_SPI_BTLDR_COMM_ENABLED     (Master_BTLDR_COMM_ENABLED && \
                                                            (Master_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Master_SPI_SLAVE_CONST))
#else
        #define Master_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (Master_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (Master_SCB_MODE_UART_INC)
       #define Master_UART_BTLDR_COMM_ENABLED    (Master_BTLDR_COMM_ENABLED && \
                                                            (Master_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (Master_UART_RX_DIRECTION && \
                                                              Master_UART_TX_DIRECTION)))
#else
     #define Master_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (Master_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define Master_BTLDR_COMM_MODE_ENABLED    (Master_I2C_BTLDR_COMM_ENABLED   || \
                                                     Master_SPI_BTLDR_COMM_ENABLED   || \
                                                     Master_EZI2C_BTLDR_COMM_ENABLED || \
                                                     Master_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void Master_I2CCyBtldrCommStart(void);
    void Master_I2CCyBtldrCommStop (void);
    void Master_I2CCyBtldrCommReset(void);
    cystatus Master_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Master_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (Master_SCB_MODE_I2C_CONST_CFG)
        #define Master_CyBtldrCommStart   Master_I2CCyBtldrCommStart
        #define Master_CyBtldrCommStop    Master_I2CCyBtldrCommStop
        #define Master_CyBtldrCommReset   Master_I2CCyBtldrCommReset
        #define Master_CyBtldrCommRead    Master_I2CCyBtldrCommRead
        #define Master_CyBtldrCommWrite   Master_I2CCyBtldrCommWrite
    #endif /* (Master_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void Master_EzI2CCyBtldrCommStart(void);
    void Master_EzI2CCyBtldrCommStop (void);
    void Master_EzI2CCyBtldrCommReset(void);
    cystatus Master_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Master_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (Master_SCB_MODE_EZI2C_CONST_CFG)
        #define Master_CyBtldrCommStart   Master_EzI2CCyBtldrCommStart
        #define Master_CyBtldrCommStop    Master_EzI2CCyBtldrCommStop
        #define Master_CyBtldrCommReset   Master_EzI2CCyBtldrCommReset
        #define Master_CyBtldrCommRead    Master_EzI2CCyBtldrCommRead
        #define Master_CyBtldrCommWrite   Master_EzI2CCyBtldrCommWrite
    #endif /* (Master_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void Master_SpiCyBtldrCommStart(void);
    void Master_SpiCyBtldrCommStop (void);
    void Master_SpiCyBtldrCommReset(void);
    cystatus Master_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Master_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (Master_SCB_MODE_SPI_CONST_CFG)
        #define Master_CyBtldrCommStart   Master_SpiCyBtldrCommStart
        #define Master_CyBtldrCommStop    Master_SpiCyBtldrCommStop
        #define Master_CyBtldrCommReset   Master_SpiCyBtldrCommReset
        #define Master_CyBtldrCommRead    Master_SpiCyBtldrCommRead
        #define Master_CyBtldrCommWrite   Master_SpiCyBtldrCommWrite
    #endif /* (Master_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void Master_UartCyBtldrCommStart(void);
    void Master_UartCyBtldrCommStop (void);
    void Master_UartCyBtldrCommReset(void);
    cystatus Master_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Master_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (Master_SCB_MODE_UART_CONST_CFG)
        #define Master_CyBtldrCommStart   Master_UartCyBtldrCommStart
        #define Master_CyBtldrCommStop    Master_UartCyBtldrCommStop
        #define Master_CyBtldrCommReset   Master_UartCyBtldrCommReset
        #define Master_CyBtldrCommRead    Master_UartCyBtldrCommRead
        #define Master_CyBtldrCommWrite   Master_UartCyBtldrCommWrite
    #endif /* (Master_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_BTLDR_COMM_ENABLED)
    #if (Master_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void Master_CyBtldrCommStart(void);
        void Master_CyBtldrCommStop (void);
        void Master_CyBtldrCommReset(void);
        cystatus Master_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus Master_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (Master_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Master)
        #define CyBtldrCommStart    Master_CyBtldrCommStart
        #define CyBtldrCommStop     Master_CyBtldrCommStop
        #define CyBtldrCommReset    Master_CyBtldrCommReset
        #define CyBtldrCommWrite    Master_CyBtldrCommWrite
        #define CyBtldrCommRead     Master_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Master) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Master_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define Master_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define Master_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define Master_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define Master_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef Master_SPI_BYTE_TO_BYTE
    #define Master_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef Master_UART_BYTE_TO_BYTE
    #define Master_UART_BYTE_TO_BYTE  (2500u)
#endif /* Master_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_Master_H) */


/* [] END OF FILE */
