/***************************************************************************//**
* \file Master_PINS.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_Master_H)
#define CY_SCB_PINS_Master_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define Master_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define Master_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define Master_REMOVE_TX_SDA_MISO_PIN      (1u)
#define Master_REMOVE_CTS_SCLK_PIN      (1u)
#define Master_REMOVE_RTS_SS0_PIN      (1u)
#define Master_REMOVE_SS1_PIN                 (1u)
#define Master_REMOVE_SS2_PIN                 (1u)
#define Master_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define Master_REMOVE_I2C_PINS                (0u)
#define Master_REMOVE_SPI_MASTER_PINS         (1u)
#define Master_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define Master_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define Master_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define Master_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define Master_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define Master_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define Master_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define Master_REMOVE_SPI_SLAVE_PINS          (1u)
#define Master_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define Master_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define Master_REMOVE_UART_TX_PIN             (1u)
#define Master_REMOVE_UART_RX_TX_PIN          (1u)
#define Master_REMOVE_UART_RX_PIN             (1u)
#define Master_REMOVE_UART_RX_WAKE_PIN        (1u)
#define Master_REMOVE_UART_RTS_PIN            (1u)
#define Master_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define Master_RX_WAKE_SCL_MOSI_PIN (0u == Master_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define Master_RX_SCL_MOSI_PIN     (0u == Master_REMOVE_RX_SCL_MOSI_PIN)
#define Master_TX_SDA_MISO_PIN     (0u == Master_REMOVE_TX_SDA_MISO_PIN)
#define Master_CTS_SCLK_PIN     (0u == Master_REMOVE_CTS_SCLK_PIN)
#define Master_RTS_SS0_PIN     (0u == Master_REMOVE_RTS_SS0_PIN)
#define Master_SS1_PIN                (0u == Master_REMOVE_SS1_PIN)
#define Master_SS2_PIN                (0u == Master_REMOVE_SS2_PIN)
#define Master_SS3_PIN                (0u == Master_REMOVE_SS3_PIN)

/* Mode defined pins */
#define Master_I2C_PINS               (0u == Master_REMOVE_I2C_PINS)
#define Master_SPI_MASTER_PINS        (0u == Master_REMOVE_SPI_MASTER_PINS)
#define Master_SPI_MASTER_SCLK_PIN    (0u == Master_REMOVE_SPI_MASTER_SCLK_PIN)
#define Master_SPI_MASTER_MOSI_PIN    (0u == Master_REMOVE_SPI_MASTER_MOSI_PIN)
#define Master_SPI_MASTER_MISO_PIN    (0u == Master_REMOVE_SPI_MASTER_MISO_PIN)
#define Master_SPI_MASTER_SS0_PIN     (0u == Master_REMOVE_SPI_MASTER_SS0_PIN)
#define Master_SPI_MASTER_SS1_PIN     (0u == Master_REMOVE_SPI_MASTER_SS1_PIN)
#define Master_SPI_MASTER_SS2_PIN     (0u == Master_REMOVE_SPI_MASTER_SS2_PIN)
#define Master_SPI_MASTER_SS3_PIN     (0u == Master_REMOVE_SPI_MASTER_SS3_PIN)
#define Master_SPI_SLAVE_PINS         (0u == Master_REMOVE_SPI_SLAVE_PINS)
#define Master_SPI_SLAVE_MOSI_PIN     (0u == Master_REMOVE_SPI_SLAVE_MOSI_PIN)
#define Master_SPI_SLAVE_MISO_PIN     (0u == Master_REMOVE_SPI_SLAVE_MISO_PIN)
#define Master_UART_TX_PIN            (0u == Master_REMOVE_UART_TX_PIN)
#define Master_UART_RX_TX_PIN         (0u == Master_REMOVE_UART_RX_TX_PIN)
#define Master_UART_RX_PIN            (0u == Master_REMOVE_UART_RX_PIN)
#define Master_UART_RX_WAKE_PIN       (0u == Master_REMOVE_UART_RX_WAKE_PIN)
#define Master_UART_RTS_PIN           (0u == Master_REMOVE_UART_RTS_PIN)
#define Master_UART_CTS_PIN           (0u == Master_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (Master_RX_WAKE_SCL_MOSI_PIN)
    #include "Master_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (Master_RX_SCL_MOSI) */

#if (Master_RX_SCL_MOSI_PIN)
    #include "Master_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (Master_RX_SCL_MOSI) */

#if (Master_TX_SDA_MISO_PIN)
    #include "Master_uart_tx_i2c_sda_spi_miso.h"
#endif /* (Master_TX_SDA_MISO) */

#if (Master_CTS_SCLK_PIN)
    #include "Master_uart_cts_spi_sclk.h"
#endif /* (Master_CTS_SCLK) */

#if (Master_RTS_SS0_PIN)
    #include "Master_uart_rts_spi_ss0.h"
#endif /* (Master_RTS_SS0_PIN) */

#if (Master_SS1_PIN)
    #include "Master_spi_ss1.h"
#endif /* (Master_SS1_PIN) */

#if (Master_SS2_PIN)
    #include "Master_spi_ss2.h"
#endif /* (Master_SS2_PIN) */

#if (Master_SS3_PIN)
    #include "Master_spi_ss3.h"
#endif /* (Master_SS3_PIN) */

#if (Master_I2C_PINS)
    #include "Master_scl.h"
    #include "Master_sda.h"
#endif /* (Master_I2C_PINS) */

#if (Master_SPI_MASTER_PINS)
#if (Master_SPI_MASTER_SCLK_PIN)
    #include "Master_sclk_m.h"
#endif /* (Master_SPI_MASTER_SCLK_PIN) */

#if (Master_SPI_MASTER_MOSI_PIN)
    #include "Master_mosi_m.h"
#endif /* (Master_SPI_MASTER_MOSI_PIN) */

#if (Master_SPI_MASTER_MISO_PIN)
    #include "Master_miso_m.h"
#endif /*(Master_SPI_MASTER_MISO_PIN) */
#endif /* (Master_SPI_MASTER_PINS) */

#if (Master_SPI_SLAVE_PINS)
    #include "Master_sclk_s.h"
    #include "Master_ss_s.h"

#if (Master_SPI_SLAVE_MOSI_PIN)
    #include "Master_mosi_s.h"
#endif /* (Master_SPI_SLAVE_MOSI_PIN) */

#if (Master_SPI_SLAVE_MISO_PIN)
    #include "Master_miso_s.h"
#endif /*(Master_SPI_SLAVE_MISO_PIN) */
#endif /* (Master_SPI_SLAVE_PINS) */

#if (Master_SPI_MASTER_SS0_PIN)
    #include "Master_ss0_m.h"
#endif /* (Master_SPI_MASTER_SS0_PIN) */

#if (Master_SPI_MASTER_SS1_PIN)
    #include "Master_ss1_m.h"
#endif /* (Master_SPI_MASTER_SS1_PIN) */

#if (Master_SPI_MASTER_SS2_PIN)
    #include "Master_ss2_m.h"
#endif /* (Master_SPI_MASTER_SS2_PIN) */

#if (Master_SPI_MASTER_SS3_PIN)
    #include "Master_ss3_m.h"
#endif /* (Master_SPI_MASTER_SS3_PIN) */

#if (Master_UART_TX_PIN)
    #include "Master_tx.h"
#endif /* (Master_UART_TX_PIN) */

#if (Master_UART_RX_TX_PIN)
    #include "Master_rx_tx.h"
#endif /* (Master_UART_RX_TX_PIN) */

#if (Master_UART_RX_PIN)
    #include "Master_rx.h"
#endif /* (Master_UART_RX_PIN) */

#if (Master_UART_RX_WAKE_PIN)
    #include "Master_rx_wake.h"
#endif /* (Master_UART_RX_WAKE_PIN) */

#if (Master_UART_RTS_PIN)
    #include "Master_rts.h"
#endif /* (Master_UART_RTS_PIN) */

#if (Master_UART_CTS_PIN)
    #include "Master_cts.h"
#endif /* (Master_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (Master_RX_SCL_MOSI_PIN)
    #define Master_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define Master_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define Master_RX_SCL_MOSI_HSIOM_MASK      (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define Master_RX_SCL_MOSI_HSIOM_POS       (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define Master_RX_SCL_MOSI_HSIOM_SEL_GPIO  (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define Master_RX_SCL_MOSI_HSIOM_SEL_I2C   (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define Master_RX_SCL_MOSI_HSIOM_SEL_SPI   (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define Master_RX_SCL_MOSI_HSIOM_SEL_UART  (Master_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (Master_RX_WAKE_SCL_MOSI_PIN)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_MASK      (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_POS       (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define Master_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (Master_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define Master_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) Master_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define Master_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) Master_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define Master_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (Master_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define Master_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) Master_INTCFG_TYPE_MASK << \
                                                                           Master_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins Master_RX_SCL_MOSI_PIN or Master_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (Master_RX_SCL_MOSI_PIN) */

#if (Master_TX_SDA_MISO_PIN)
    #define Master_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) Master_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define Master_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) Master_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define Master_TX_SDA_MISO_HSIOM_MASK      (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define Master_TX_SDA_MISO_HSIOM_POS       (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define Master_TX_SDA_MISO_HSIOM_SEL_GPIO  (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define Master_TX_SDA_MISO_HSIOM_SEL_I2C   (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define Master_TX_SDA_MISO_HSIOM_SEL_SPI   (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define Master_TX_SDA_MISO_HSIOM_SEL_UART  (Master_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (Master_TX_SDA_MISO_PIN) */

#if (Master_CTS_SCLK_PIN)
    #define Master_CTS_SCLK_HSIOM_REG   (*(reg32 *) Master_uart_cts_spi_sclk__0__HSIOM)
    #define Master_CTS_SCLK_HSIOM_PTR   ( (reg32 *) Master_uart_cts_spi_sclk__0__HSIOM)
    
    #define Master_CTS_SCLK_HSIOM_MASK      (Master_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define Master_CTS_SCLK_HSIOM_POS       (Master_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define Master_CTS_SCLK_HSIOM_SEL_GPIO  (Master_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define Master_CTS_SCLK_HSIOM_SEL_I2C   (Master_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define Master_CTS_SCLK_HSIOM_SEL_SPI   (Master_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define Master_CTS_SCLK_HSIOM_SEL_UART  (Master_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (Master_CTS_SCLK_PIN) */

#if (Master_RTS_SS0_PIN)
    #define Master_RTS_SS0_HSIOM_REG   (*(reg32 *) Master_uart_rts_spi_ss0__0__HSIOM)
    #define Master_RTS_SS0_HSIOM_PTR   ( (reg32 *) Master_uart_rts_spi_ss0__0__HSIOM)
    
    #define Master_RTS_SS0_HSIOM_MASK      (Master_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define Master_RTS_SS0_HSIOM_POS       (Master_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define Master_RTS_SS0_HSIOM_SEL_GPIO  (Master_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define Master_RTS_SS0_HSIOM_SEL_I2C   (Master_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define Master_RTS_SS0_HSIOM_SEL_SPI   (Master_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1)
    #define Master_RTS_SS0_HSIOM_SEL_UART  (Master_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(Master_CY_SCBIP_V0 || Master_CY_SCBIP_V1) */
#endif /* (Master_RTS_SS0_PIN) */

#if (Master_SS1_PIN)
    #define Master_SS1_HSIOM_REG  (*(reg32 *) Master_spi_ss1__0__HSIOM)
    #define Master_SS1_HSIOM_PTR  ( (reg32 *) Master_spi_ss1__0__HSIOM)
    
    #define Master_SS1_HSIOM_MASK     (Master_spi_ss1__0__HSIOM_MASK)
    #define Master_SS1_HSIOM_POS      (Master_spi_ss1__0__HSIOM_SHIFT)
    #define Master_SS1_HSIOM_SEL_GPIO (Master_spi_ss1__0__HSIOM_GPIO)
    #define Master_SS1_HSIOM_SEL_I2C  (Master_spi_ss1__0__HSIOM_I2C)
    #define Master_SS1_HSIOM_SEL_SPI  (Master_spi_ss1__0__HSIOM_SPI)
#endif /* (Master_SS1_PIN) */

#if (Master_SS2_PIN)
    #define Master_SS2_HSIOM_REG     (*(reg32 *) Master_spi_ss2__0__HSIOM)
    #define Master_SS2_HSIOM_PTR     ( (reg32 *) Master_spi_ss2__0__HSIOM)
    
    #define Master_SS2_HSIOM_MASK     (Master_spi_ss2__0__HSIOM_MASK)
    #define Master_SS2_HSIOM_POS      (Master_spi_ss2__0__HSIOM_SHIFT)
    #define Master_SS2_HSIOM_SEL_GPIO (Master_spi_ss2__0__HSIOM_GPIO)
    #define Master_SS2_HSIOM_SEL_I2C  (Master_spi_ss2__0__HSIOM_I2C)
    #define Master_SS2_HSIOM_SEL_SPI  (Master_spi_ss2__0__HSIOM_SPI)
#endif /* (Master_SS2_PIN) */

#if (Master_SS3_PIN)
    #define Master_SS3_HSIOM_REG     (*(reg32 *) Master_spi_ss3__0__HSIOM)
    #define Master_SS3_HSIOM_PTR     ( (reg32 *) Master_spi_ss3__0__HSIOM)
    
    #define Master_SS3_HSIOM_MASK     (Master_spi_ss3__0__HSIOM_MASK)
    #define Master_SS3_HSIOM_POS      (Master_spi_ss3__0__HSIOM_SHIFT)
    #define Master_SS3_HSIOM_SEL_GPIO (Master_spi_ss3__0__HSIOM_GPIO)
    #define Master_SS3_HSIOM_SEL_I2C  (Master_spi_ss3__0__HSIOM_I2C)
    #define Master_SS3_HSIOM_SEL_SPI  (Master_spi_ss3__0__HSIOM_SPI)
#endif /* (Master_SS3_PIN) */

#if (Master_I2C_PINS)
    #define Master_SCL_HSIOM_REG  (*(reg32 *) Master_scl__0__HSIOM)
    #define Master_SCL_HSIOM_PTR  ( (reg32 *) Master_scl__0__HSIOM)
    
    #define Master_SCL_HSIOM_MASK     (Master_scl__0__HSIOM_MASK)
    #define Master_SCL_HSIOM_POS      (Master_scl__0__HSIOM_SHIFT)
    #define Master_SCL_HSIOM_SEL_GPIO (Master_sda__0__HSIOM_GPIO)
    #define Master_SCL_HSIOM_SEL_I2C  (Master_sda__0__HSIOM_I2C)
    
    #define Master_SDA_HSIOM_REG  (*(reg32 *) Master_sda__0__HSIOM)
    #define Master_SDA_HSIOM_PTR  ( (reg32 *) Master_sda__0__HSIOM)
    
    #define Master_SDA_HSIOM_MASK     (Master_sda__0__HSIOM_MASK)
    #define Master_SDA_HSIOM_POS      (Master_sda__0__HSIOM_SHIFT)
    #define Master_SDA_HSIOM_SEL_GPIO (Master_sda__0__HSIOM_GPIO)
    #define Master_SDA_HSIOM_SEL_I2C  (Master_sda__0__HSIOM_I2C)
#endif /* (Master_I2C_PINS) */

#if (Master_SPI_SLAVE_PINS)
    #define Master_SCLK_S_HSIOM_REG   (*(reg32 *) Master_sclk_s__0__HSIOM)
    #define Master_SCLK_S_HSIOM_PTR   ( (reg32 *) Master_sclk_s__0__HSIOM)
    
    #define Master_SCLK_S_HSIOM_MASK      (Master_sclk_s__0__HSIOM_MASK)
    #define Master_SCLK_S_HSIOM_POS       (Master_sclk_s__0__HSIOM_SHIFT)
    #define Master_SCLK_S_HSIOM_SEL_GPIO  (Master_sclk_s__0__HSIOM_GPIO)
    #define Master_SCLK_S_HSIOM_SEL_SPI   (Master_sclk_s__0__HSIOM_SPI)
    
    #define Master_SS0_S_HSIOM_REG    (*(reg32 *) Master_ss0_s__0__HSIOM)
    #define Master_SS0_S_HSIOM_PTR    ( (reg32 *) Master_ss0_s__0__HSIOM)
    
    #define Master_SS0_S_HSIOM_MASK       (Master_ss0_s__0__HSIOM_MASK)
    #define Master_SS0_S_HSIOM_POS        (Master_ss0_s__0__HSIOM_SHIFT)
    #define Master_SS0_S_HSIOM_SEL_GPIO   (Master_ss0_s__0__HSIOM_GPIO)  
    #define Master_SS0_S_HSIOM_SEL_SPI    (Master_ss0_s__0__HSIOM_SPI)
#endif /* (Master_SPI_SLAVE_PINS) */

#if (Master_SPI_SLAVE_MOSI_PIN)
    #define Master_MOSI_S_HSIOM_REG   (*(reg32 *) Master_mosi_s__0__HSIOM)
    #define Master_MOSI_S_HSIOM_PTR   ( (reg32 *) Master_mosi_s__0__HSIOM)
    
    #define Master_MOSI_S_HSIOM_MASK      (Master_mosi_s__0__HSIOM_MASK)
    #define Master_MOSI_S_HSIOM_POS       (Master_mosi_s__0__HSIOM_SHIFT)
    #define Master_MOSI_S_HSIOM_SEL_GPIO  (Master_mosi_s__0__HSIOM_GPIO)
    #define Master_MOSI_S_HSIOM_SEL_SPI   (Master_mosi_s__0__HSIOM_SPI)
#endif /* (Master_SPI_SLAVE_MOSI_PIN) */

#if (Master_SPI_SLAVE_MISO_PIN)
    #define Master_MISO_S_HSIOM_REG   (*(reg32 *) Master_miso_s__0__HSIOM)
    #define Master_MISO_S_HSIOM_PTR   ( (reg32 *) Master_miso_s__0__HSIOM)
    
    #define Master_MISO_S_HSIOM_MASK      (Master_miso_s__0__HSIOM_MASK)
    #define Master_MISO_S_HSIOM_POS       (Master_miso_s__0__HSIOM_SHIFT)
    #define Master_MISO_S_HSIOM_SEL_GPIO  (Master_miso_s__0__HSIOM_GPIO)
    #define Master_MISO_S_HSIOM_SEL_SPI   (Master_miso_s__0__HSIOM_SPI)
#endif /* (Master_SPI_SLAVE_MISO_PIN) */

#if (Master_SPI_MASTER_MISO_PIN)
    #define Master_MISO_M_HSIOM_REG   (*(reg32 *) Master_miso_m__0__HSIOM)
    #define Master_MISO_M_HSIOM_PTR   ( (reg32 *) Master_miso_m__0__HSIOM)
    
    #define Master_MISO_M_HSIOM_MASK      (Master_miso_m__0__HSIOM_MASK)
    #define Master_MISO_M_HSIOM_POS       (Master_miso_m__0__HSIOM_SHIFT)
    #define Master_MISO_M_HSIOM_SEL_GPIO  (Master_miso_m__0__HSIOM_GPIO)
    #define Master_MISO_M_HSIOM_SEL_SPI   (Master_miso_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_MISO_PIN) */

#if (Master_SPI_MASTER_MOSI_PIN)
    #define Master_MOSI_M_HSIOM_REG   (*(reg32 *) Master_mosi_m__0__HSIOM)
    #define Master_MOSI_M_HSIOM_PTR   ( (reg32 *) Master_mosi_m__0__HSIOM)
    
    #define Master_MOSI_M_HSIOM_MASK      (Master_mosi_m__0__HSIOM_MASK)
    #define Master_MOSI_M_HSIOM_POS       (Master_mosi_m__0__HSIOM_SHIFT)
    #define Master_MOSI_M_HSIOM_SEL_GPIO  (Master_mosi_m__0__HSIOM_GPIO)
    #define Master_MOSI_M_HSIOM_SEL_SPI   (Master_mosi_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_MOSI_PIN) */

#if (Master_SPI_MASTER_SCLK_PIN)
    #define Master_SCLK_M_HSIOM_REG   (*(reg32 *) Master_sclk_m__0__HSIOM)
    #define Master_SCLK_M_HSIOM_PTR   ( (reg32 *) Master_sclk_m__0__HSIOM)
    
    #define Master_SCLK_M_HSIOM_MASK      (Master_sclk_m__0__HSIOM_MASK)
    #define Master_SCLK_M_HSIOM_POS       (Master_sclk_m__0__HSIOM_SHIFT)
    #define Master_SCLK_M_HSIOM_SEL_GPIO  (Master_sclk_m__0__HSIOM_GPIO)
    #define Master_SCLK_M_HSIOM_SEL_SPI   (Master_sclk_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_SCLK_PIN) */

#if (Master_SPI_MASTER_SS0_PIN)
    #define Master_SS0_M_HSIOM_REG    (*(reg32 *) Master_ss0_m__0__HSIOM)
    #define Master_SS0_M_HSIOM_PTR    ( (reg32 *) Master_ss0_m__0__HSIOM)
    
    #define Master_SS0_M_HSIOM_MASK       (Master_ss0_m__0__HSIOM_MASK)
    #define Master_SS0_M_HSIOM_POS        (Master_ss0_m__0__HSIOM_SHIFT)
    #define Master_SS0_M_HSIOM_SEL_GPIO   (Master_ss0_m__0__HSIOM_GPIO)
    #define Master_SS0_M_HSIOM_SEL_SPI    (Master_ss0_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_SS0_PIN) */

#if (Master_SPI_MASTER_SS1_PIN)
    #define Master_SS1_M_HSIOM_REG    (*(reg32 *) Master_ss1_m__0__HSIOM)
    #define Master_SS1_M_HSIOM_PTR    ( (reg32 *) Master_ss1_m__0__HSIOM)
    
    #define Master_SS1_M_HSIOM_MASK       (Master_ss1_m__0__HSIOM_MASK)
    #define Master_SS1_M_HSIOM_POS        (Master_ss1_m__0__HSIOM_SHIFT)
    #define Master_SS1_M_HSIOM_SEL_GPIO   (Master_ss1_m__0__HSIOM_GPIO)
    #define Master_SS1_M_HSIOM_SEL_SPI    (Master_ss1_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_SS1_PIN) */

#if (Master_SPI_MASTER_SS2_PIN)
    #define Master_SS2_M_HSIOM_REG    (*(reg32 *) Master_ss2_m__0__HSIOM)
    #define Master_SS2_M_HSIOM_PTR    ( (reg32 *) Master_ss2_m__0__HSIOM)
    
    #define Master_SS2_M_HSIOM_MASK       (Master_ss2_m__0__HSIOM_MASK)
    #define Master_SS2_M_HSIOM_POS        (Master_ss2_m__0__HSIOM_SHIFT)
    #define Master_SS2_M_HSIOM_SEL_GPIO   (Master_ss2_m__0__HSIOM_GPIO)
    #define Master_SS2_M_HSIOM_SEL_SPI    (Master_ss2_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_SS2_PIN) */

#if (Master_SPI_MASTER_SS3_PIN)
    #define Master_SS3_M_HSIOM_REG    (*(reg32 *) Master_ss3_m__0__HSIOM)
    #define Master_SS3_M_HSIOM_PTR    ( (reg32 *) Master_ss3_m__0__HSIOM)
    
    #define Master_SS3_M_HSIOM_MASK      (Master_ss3_m__0__HSIOM_MASK)
    #define Master_SS3_M_HSIOM_POS       (Master_ss3_m__0__HSIOM_SHIFT)
    #define Master_SS3_M_HSIOM_SEL_GPIO  (Master_ss3_m__0__HSIOM_GPIO)
    #define Master_SS3_M_HSIOM_SEL_SPI   (Master_ss3_m__0__HSIOM_SPI)
#endif /* (Master_SPI_MASTER_SS3_PIN) */

#if (Master_UART_RX_PIN)
    #define Master_RX_HSIOM_REG   (*(reg32 *) Master_rx__0__HSIOM)
    #define Master_RX_HSIOM_PTR   ( (reg32 *) Master_rx__0__HSIOM)
    
    #define Master_RX_HSIOM_MASK      (Master_rx__0__HSIOM_MASK)
    #define Master_RX_HSIOM_POS       (Master_rx__0__HSIOM_SHIFT)
    #define Master_RX_HSIOM_SEL_GPIO  (Master_rx__0__HSIOM_GPIO)
    #define Master_RX_HSIOM_SEL_UART  (Master_rx__0__HSIOM_UART)
#endif /* (Master_UART_RX_PIN) */

#if (Master_UART_RX_WAKE_PIN)
    #define Master_RX_WAKE_HSIOM_REG   (*(reg32 *) Master_rx_wake__0__HSIOM)
    #define Master_RX_WAKE_HSIOM_PTR   ( (reg32 *) Master_rx_wake__0__HSIOM)
    
    #define Master_RX_WAKE_HSIOM_MASK      (Master_rx_wake__0__HSIOM_MASK)
    #define Master_RX_WAKE_HSIOM_POS       (Master_rx_wake__0__HSIOM_SHIFT)
    #define Master_RX_WAKE_HSIOM_SEL_GPIO  (Master_rx_wake__0__HSIOM_GPIO)
    #define Master_RX_WAKE_HSIOM_SEL_UART  (Master_rx_wake__0__HSIOM_UART)
#endif /* (Master_UART_WAKE_RX_PIN) */

#if (Master_UART_CTS_PIN)
    #define Master_CTS_HSIOM_REG   (*(reg32 *) Master_cts__0__HSIOM)
    #define Master_CTS_HSIOM_PTR   ( (reg32 *) Master_cts__0__HSIOM)
    
    #define Master_CTS_HSIOM_MASK      (Master_cts__0__HSIOM_MASK)
    #define Master_CTS_HSIOM_POS       (Master_cts__0__HSIOM_SHIFT)
    #define Master_CTS_HSIOM_SEL_GPIO  (Master_cts__0__HSIOM_GPIO)
    #define Master_CTS_HSIOM_SEL_UART  (Master_cts__0__HSIOM_UART)
#endif /* (Master_UART_CTS_PIN) */

#if (Master_UART_TX_PIN)
    #define Master_TX_HSIOM_REG   (*(reg32 *) Master_tx__0__HSIOM)
    #define Master_TX_HSIOM_PTR   ( (reg32 *) Master_tx__0__HSIOM)
    
    #define Master_TX_HSIOM_MASK      (Master_tx__0__HSIOM_MASK)
    #define Master_TX_HSIOM_POS       (Master_tx__0__HSIOM_SHIFT)
    #define Master_TX_HSIOM_SEL_GPIO  (Master_tx__0__HSIOM_GPIO)
    #define Master_TX_HSIOM_SEL_UART  (Master_tx__0__HSIOM_UART)
#endif /* (Master_UART_TX_PIN) */

#if (Master_UART_RX_TX_PIN)
    #define Master_RX_TX_HSIOM_REG   (*(reg32 *) Master_rx_tx__0__HSIOM)
    #define Master_RX_TX_HSIOM_PTR   ( (reg32 *) Master_rx_tx__0__HSIOM)
    
    #define Master_RX_TX_HSIOM_MASK      (Master_rx_tx__0__HSIOM_MASK)
    #define Master_RX_TX_HSIOM_POS       (Master_rx_tx__0__HSIOM_SHIFT)
    #define Master_RX_TX_HSIOM_SEL_GPIO  (Master_rx_tx__0__HSIOM_GPIO)
    #define Master_RX_TX_HSIOM_SEL_UART  (Master_rx_tx__0__HSIOM_UART)
#endif /* (Master_UART_RX_TX_PIN) */

#if (Master_UART_RTS_PIN)
    #define Master_RTS_HSIOM_REG      (*(reg32 *) Master_rts__0__HSIOM)
    #define Master_RTS_HSIOM_PTR      ( (reg32 *) Master_rts__0__HSIOM)
    
    #define Master_RTS_HSIOM_MASK     (Master_rts__0__HSIOM_MASK)
    #define Master_RTS_HSIOM_POS      (Master_rts__0__HSIOM_SHIFT)    
    #define Master_RTS_HSIOM_SEL_GPIO (Master_rts__0__HSIOM_GPIO)
    #define Master_RTS_HSIOM_SEL_UART (Master_rts__0__HSIOM_UART)    
#endif /* (Master_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define Master_HSIOM_DEF_SEL      (0x00u)
#define Master_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for Master_CY_SCBIP_V0 
* and Master_CY_SCBIP_V1. It is not recommended to use them for 
* Master_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define Master_HSIOM_UART_SEL     (0x09u)
#define Master_HSIOM_I2C_SEL      (0x0Eu)
#define Master_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define Master_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define Master_RX_SCL_MOSI_PIN_INDEX       (0u)
#define Master_TX_SDA_MISO_PIN_INDEX       (1u)
#define Master_CTS_SCLK_PIN_INDEX       (2u)
#define Master_RTS_SS0_PIN_INDEX       (3u)
#define Master_SS1_PIN_INDEX                  (4u)
#define Master_SS2_PIN_INDEX                  (5u)
#define Master_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define Master_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << Master_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define Master_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << Master_RX_SCL_MOSI_PIN_INDEX)
#define Master_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << Master_TX_SDA_MISO_PIN_INDEX)
#define Master_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << Master_CTS_SCLK_PIN_INDEX)
#define Master_RTS_SS0_PIN_MASK     ((uint32) 0x01u << Master_RTS_SS0_PIN_INDEX)
#define Master_SS1_PIN_MASK                ((uint32) 0x01u << Master_SS1_PIN_INDEX)
#define Master_SS2_PIN_MASK                ((uint32) 0x01u << Master_SS2_PIN_INDEX)
#define Master_SS3_PIN_MASK                ((uint32) 0x01u << Master_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define Master_INTCFG_TYPE_MASK           (0x03u)
#define Master_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define Master_PIN_DM_ALG_HIZ  (0u)
#define Master_PIN_DM_DIG_HIZ  (1u)
#define Master_PIN_DM_OD_LO    (4u)
#define Master_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define Master_DM_MASK    (0x7u)
#define Master_DM_SIZE    (3u)
#define Master_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) Master_DM_MASK << (Master_DM_SIZE * (pos)))) >> \
                                                              (Master_DM_SIZE * (pos)) )

#if (Master_TX_SDA_MISO_PIN)
    #define Master_CHECK_TX_SDA_MISO_PIN_USED \
                (Master_PIN_DM_ALG_HIZ != \
                    Master_GET_P4_PIN_DM(Master_uart_tx_i2c_sda_spi_miso_PC, \
                                                   Master_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (Master_TX_SDA_MISO_PIN) */

#if (Master_RTS_SS0_PIN)
    #define Master_CHECK_RTS_SS0_PIN_USED \
                (Master_PIN_DM_ALG_HIZ != \
                    Master_GET_P4_PIN_DM(Master_uart_rts_spi_ss0_PC, \
                                                   Master_uart_rts_spi_ss0_SHIFT))
#endif /* (Master_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define Master_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define Master_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define Master_SET_HSIOM_SEL(reg, mask, pos, sel) Master_SET_REGISTER_BITS(reg, mask, pos, sel)
#define Master_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        Master_SET_REGISTER_BITS(reg, mask, pos, intType)
#define Master_SET_INP_DIS(reg, mask, val) Master_SET_REGISTER_BIT(reg, mask, val)

/* Master_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  Master_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (Master_CY_SCBIP_V0)
#if (Master_I2C_PINS)
    #define Master_SET_I2C_SCL_DR(val) Master_scl_Write(val)

    #define Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                          Master_SET_HSIOM_SEL(Master_SCL_HSIOM_REG,  \
                                                         Master_SCL_HSIOM_MASK, \
                                                         Master_SCL_HSIOM_POS,  \
                                                         (sel))
    #define Master_WAIT_SCL_SET_HIGH  (0u == Master_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (Master_RX_WAKE_SCL_MOSI_PIN)
    #define Master_SET_I2C_SCL_DR(val) \
                            Master_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                    Master_SET_HSIOM_SEL(Master_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   Master_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   Master_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define Master_WAIT_SCL_SET_HIGH  (0u == Master_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (Master_RX_SCL_MOSI_PIN)
    #define Master_SET_I2C_SCL_DR(val) \
                            Master_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                            Master_SET_HSIOM_SEL(Master_RX_SCL_MOSI_HSIOM_REG,  \
                                                           Master_RX_SCL_MOSI_HSIOM_MASK, \
                                                           Master_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define Master_WAIT_SCL_SET_HIGH  (0u == Master_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define Master_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define Master_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define Master_WAIT_SCL_SET_HIGH  (0u)
#endif /* (Master_I2C_PINS) */

/* SCB I2C: sda signal */
#if (Master_I2C_PINS)
    #define Master_WAIT_SDA_SET_HIGH  (0u == Master_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (Master_TX_SDA_MISO_PIN)
    #define Master_WAIT_SDA_SET_HIGH  (0u == Master_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define Master_WAIT_SDA_SET_HIGH  (0u)
#endif /* (Master_MOSI_SCL_RX_PIN) */
#endif /* (Master_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (Master_RX_SCL_MOSI_PIN)
    #define Master_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (Master_RX_WAKE_SCL_MOSI_PIN)
    #define Master_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) Master_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(Master_UART_RX_WAKE_PIN)
    #define Master_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) Master_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (Master_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define Master_REMOVE_MOSI_SCL_RX_WAKE_PIN    Master_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define Master_REMOVE_MOSI_SCL_RX_PIN         Master_REMOVE_RX_SCL_MOSI_PIN
#define Master_REMOVE_MISO_SDA_TX_PIN         Master_REMOVE_TX_SDA_MISO_PIN
#ifndef Master_REMOVE_SCLK_PIN
#define Master_REMOVE_SCLK_PIN                Master_REMOVE_CTS_SCLK_PIN
#endif /* Master_REMOVE_SCLK_PIN */
#ifndef Master_REMOVE_SS0_PIN
#define Master_REMOVE_SS0_PIN                 Master_REMOVE_RTS_SS0_PIN
#endif /* Master_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define Master_MOSI_SCL_RX_WAKE_PIN   Master_RX_WAKE_SCL_MOSI_PIN
#define Master_MOSI_SCL_RX_PIN        Master_RX_SCL_MOSI_PIN
#define Master_MISO_SDA_TX_PIN        Master_TX_SDA_MISO_PIN
#ifndef Master_SCLK_PIN
#define Master_SCLK_PIN               Master_CTS_SCLK_PIN
#endif /* Master_SCLK_PIN */
#ifndef Master_SS0_PIN
#define Master_SS0_PIN                Master_RTS_SS0_PIN
#endif /* Master_SS0_PIN */

#if (Master_MOSI_SCL_RX_WAKE_PIN)
    #define Master_MOSI_SCL_RX_WAKE_HSIOM_REG     Master_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_WAKE_HSIOM_PTR     Master_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_WAKE_HSIOM_MASK    Master_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_WAKE_HSIOM_POS     Master_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define Master_MOSI_SCL_RX_WAKE_INTCFG_REG    Master_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_WAKE_INTCFG_PTR    Master_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define Master_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   Master_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  Master_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (Master_RX_WAKE_SCL_MOSI_PIN) */

#if (Master_MOSI_SCL_RX_PIN)
    #define Master_MOSI_SCL_RX_HSIOM_REG      Master_RX_SCL_MOSI_HSIOM_REG
    #define Master_MOSI_SCL_RX_HSIOM_PTR      Master_RX_SCL_MOSI_HSIOM_PTR
    #define Master_MOSI_SCL_RX_HSIOM_MASK     Master_RX_SCL_MOSI_HSIOM_MASK
    #define Master_MOSI_SCL_RX_HSIOM_POS      Master_RX_SCL_MOSI_HSIOM_POS
#endif /* (Master_MOSI_SCL_RX_PIN) */

#if (Master_MISO_SDA_TX_PIN)
    #define Master_MISO_SDA_TX_HSIOM_REG      Master_TX_SDA_MISO_HSIOM_REG
    #define Master_MISO_SDA_TX_HSIOM_PTR      Master_TX_SDA_MISO_HSIOM_REG
    #define Master_MISO_SDA_TX_HSIOM_MASK     Master_TX_SDA_MISO_HSIOM_REG
    #define Master_MISO_SDA_TX_HSIOM_POS      Master_TX_SDA_MISO_HSIOM_REG
#endif /* (Master_MISO_SDA_TX_PIN_PIN) */

#if (Master_SCLK_PIN)
    #ifndef Master_SCLK_HSIOM_REG
    #define Master_SCLK_HSIOM_REG     Master_CTS_SCLK_HSIOM_REG
    #define Master_SCLK_HSIOM_PTR     Master_CTS_SCLK_HSIOM_PTR
    #define Master_SCLK_HSIOM_MASK    Master_CTS_SCLK_HSIOM_MASK
    #define Master_SCLK_HSIOM_POS     Master_CTS_SCLK_HSIOM_POS
    #endif /* Master_SCLK_HSIOM_REG */
#endif /* (Master_SCLK_PIN) */

#if (Master_SS0_PIN)
    #ifndef Master_SS0_HSIOM_REG
    #define Master_SS0_HSIOM_REG      Master_RTS_SS0_HSIOM_REG
    #define Master_SS0_HSIOM_PTR      Master_RTS_SS0_HSIOM_PTR
    #define Master_SS0_HSIOM_MASK     Master_RTS_SS0_HSIOM_MASK
    #define Master_SS0_HSIOM_POS      Master_RTS_SS0_HSIOM_POS
    #endif /* Master_SS0_HSIOM_REG */
#endif /* (Master_SS0_PIN) */

#define Master_MOSI_SCL_RX_WAKE_PIN_INDEX Master_RX_WAKE_SCL_MOSI_PIN_INDEX
#define Master_MOSI_SCL_RX_PIN_INDEX      Master_RX_SCL_MOSI_PIN_INDEX
#define Master_MISO_SDA_TX_PIN_INDEX      Master_TX_SDA_MISO_PIN_INDEX
#ifndef Master_SCLK_PIN_INDEX
#define Master_SCLK_PIN_INDEX             Master_CTS_SCLK_PIN_INDEX
#endif /* Master_SCLK_PIN_INDEX */
#ifndef Master_SS0_PIN_INDEX
#define Master_SS0_PIN_INDEX              Master_RTS_SS0_PIN_INDEX
#endif /* Master_SS0_PIN_INDEX */

#define Master_MOSI_SCL_RX_WAKE_PIN_MASK Master_RX_WAKE_SCL_MOSI_PIN_MASK
#define Master_MOSI_SCL_RX_PIN_MASK      Master_RX_SCL_MOSI_PIN_MASK
#define Master_MISO_SDA_TX_PIN_MASK      Master_TX_SDA_MISO_PIN_MASK
#ifndef Master_SCLK_PIN_MASK
#define Master_SCLK_PIN_MASK             Master_CTS_SCLK_PIN_MASK
#endif /* Master_SCLK_PIN_MASK */
#ifndef Master_SS0_PIN_MASK
#define Master_SS0_PIN_MASK              Master_RTS_SS0_PIN_MASK
#endif /* Master_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_Master_H) */


/* [] END OF FILE */
