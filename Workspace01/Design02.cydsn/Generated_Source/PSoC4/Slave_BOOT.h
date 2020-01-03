/***************************************************************************//**
* \file Slave_BOOT.h
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

#if !defined(CY_SCB_BOOT_Slave_H)
#define CY_SCB_BOOT_Slave_H

#include "Slave_PVT.h"

#if (Slave_SCB_MODE_I2C_INC)
    #include "Slave_I2C.h"
#endif /* (Slave_SCB_MODE_I2C_INC) */

#if (Slave_SCB_MODE_EZI2C_INC)
    #include "Slave_EZI2C.h"
#endif /* (Slave_SCB_MODE_EZI2C_INC) */

#if (Slave_SCB_MODE_SPI_INC || Slave_SCB_MODE_UART_INC)
    #include "Slave_SPI_UART.h"
#endif /* (Slave_SCB_MODE_SPI_INC || Slave_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define Slave_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Slave) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (Slave_SCB_MODE_I2C_INC)
    #define Slave_I2C_BTLDR_COMM_ENABLED     (Slave_BTLDR_COMM_ENABLED && \
                                                            (Slave_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Slave_I2C_SLAVE_CONST))
#else
     #define Slave_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (Slave_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (Slave_SCB_MODE_EZI2C_INC)
    #define Slave_EZI2C_BTLDR_COMM_ENABLED   (Slave_BTLDR_COMM_ENABLED && \
                                                         Slave_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define Slave_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (Slave_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (Slave_SCB_MODE_SPI_INC)
    #define Slave_SPI_BTLDR_COMM_ENABLED     (Slave_BTLDR_COMM_ENABLED && \
                                                            (Slave_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Slave_SPI_SLAVE_CONST))
#else
        #define Slave_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (Slave_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (Slave_SCB_MODE_UART_INC)
       #define Slave_UART_BTLDR_COMM_ENABLED    (Slave_BTLDR_COMM_ENABLED && \
                                                            (Slave_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (Slave_UART_RX_DIRECTION && \
                                                              Slave_UART_TX_DIRECTION)))
#else
     #define Slave_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (Slave_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define Slave_BTLDR_COMM_MODE_ENABLED    (Slave_I2C_BTLDR_COMM_ENABLED   || \
                                                     Slave_SPI_BTLDR_COMM_ENABLED   || \
                                                     Slave_EZI2C_BTLDR_COMM_ENABLED || \
                                                     Slave_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void Slave_I2CCyBtldrCommStart(void);
    void Slave_I2CCyBtldrCommStop (void);
    void Slave_I2CCyBtldrCommReset(void);
    cystatus Slave_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Slave_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (Slave_SCB_MODE_I2C_CONST_CFG)
        #define Slave_CyBtldrCommStart   Slave_I2CCyBtldrCommStart
        #define Slave_CyBtldrCommStop    Slave_I2CCyBtldrCommStop
        #define Slave_CyBtldrCommReset   Slave_I2CCyBtldrCommReset
        #define Slave_CyBtldrCommRead    Slave_I2CCyBtldrCommRead
        #define Slave_CyBtldrCommWrite   Slave_I2CCyBtldrCommWrite
    #endif /* (Slave_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void Slave_EzI2CCyBtldrCommStart(void);
    void Slave_EzI2CCyBtldrCommStop (void);
    void Slave_EzI2CCyBtldrCommReset(void);
    cystatus Slave_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Slave_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (Slave_SCB_MODE_EZI2C_CONST_CFG)
        #define Slave_CyBtldrCommStart   Slave_EzI2CCyBtldrCommStart
        #define Slave_CyBtldrCommStop    Slave_EzI2CCyBtldrCommStop
        #define Slave_CyBtldrCommReset   Slave_EzI2CCyBtldrCommReset
        #define Slave_CyBtldrCommRead    Slave_EzI2CCyBtldrCommRead
        #define Slave_CyBtldrCommWrite   Slave_EzI2CCyBtldrCommWrite
    #endif /* (Slave_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void Slave_SpiCyBtldrCommStart(void);
    void Slave_SpiCyBtldrCommStop (void);
    void Slave_SpiCyBtldrCommReset(void);
    cystatus Slave_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Slave_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (Slave_SCB_MODE_SPI_CONST_CFG)
        #define Slave_CyBtldrCommStart   Slave_SpiCyBtldrCommStart
        #define Slave_CyBtldrCommStop    Slave_SpiCyBtldrCommStop
        #define Slave_CyBtldrCommReset   Slave_SpiCyBtldrCommReset
        #define Slave_CyBtldrCommRead    Slave_SpiCyBtldrCommRead
        #define Slave_CyBtldrCommWrite   Slave_SpiCyBtldrCommWrite
    #endif /* (Slave_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void Slave_UartCyBtldrCommStart(void);
    void Slave_UartCyBtldrCommStop (void);
    void Slave_UartCyBtldrCommReset(void);
    cystatus Slave_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Slave_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (Slave_SCB_MODE_UART_CONST_CFG)
        #define Slave_CyBtldrCommStart   Slave_UartCyBtldrCommStart
        #define Slave_CyBtldrCommStop    Slave_UartCyBtldrCommStop
        #define Slave_CyBtldrCommReset   Slave_UartCyBtldrCommReset
        #define Slave_CyBtldrCommRead    Slave_UartCyBtldrCommRead
        #define Slave_CyBtldrCommWrite   Slave_UartCyBtldrCommWrite
    #endif /* (Slave_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_BTLDR_COMM_ENABLED)
    #if (Slave_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void Slave_CyBtldrCommStart(void);
        void Slave_CyBtldrCommStop (void);
        void Slave_CyBtldrCommReset(void);
        cystatus Slave_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus Slave_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (Slave_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Slave)
        #define CyBtldrCommStart    Slave_CyBtldrCommStart
        #define CyBtldrCommStop     Slave_CyBtldrCommStop
        #define CyBtldrCommReset    Slave_CyBtldrCommReset
        #define CyBtldrCommWrite    Slave_CyBtldrCommWrite
        #define CyBtldrCommRead     Slave_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Slave) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Slave_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define Slave_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define Slave_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define Slave_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define Slave_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef Slave_SPI_BYTE_TO_BYTE
    #define Slave_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef Slave_UART_BYTE_TO_BYTE
    #define Slave_UART_BYTE_TO_BYTE  (2500u)
#endif /* Slave_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_Slave_H) */


/* [] END OF FILE */
