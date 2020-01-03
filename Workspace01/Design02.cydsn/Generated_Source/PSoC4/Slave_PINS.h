/***************************************************************************//**
* \file Slave_PINS.h
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

#if !defined(CY_SCB_PINS_Slave_H)
#define CY_SCB_PINS_Slave_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define Slave_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define Slave_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define Slave_REMOVE_TX_SDA_MISO_PIN      (1u)
#define Slave_REMOVE_CTS_SCLK_PIN      (1u)
#define Slave_REMOVE_RTS_SS0_PIN      (1u)
#define Slave_REMOVE_SS1_PIN                 (1u)
#define Slave_REMOVE_SS2_PIN                 (1u)
#define Slave_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define Slave_REMOVE_I2C_PINS                (0u)
#define Slave_REMOVE_SPI_MASTER_PINS         (1u)
#define Slave_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define Slave_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define Slave_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define Slave_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define Slave_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define Slave_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define Slave_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define Slave_REMOVE_SPI_SLAVE_PINS          (1u)
#define Slave_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define Slave_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define Slave_REMOVE_UART_TX_PIN             (1u)
#define Slave_REMOVE_UART_RX_TX_PIN          (1u)
#define Slave_REMOVE_UART_RX_PIN             (1u)
#define Slave_REMOVE_UART_RX_WAKE_PIN        (1u)
#define Slave_REMOVE_UART_RTS_PIN            (1u)
#define Slave_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define Slave_RX_WAKE_SCL_MOSI_PIN (0u == Slave_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define Slave_RX_SCL_MOSI_PIN     (0u == Slave_REMOVE_RX_SCL_MOSI_PIN)
#define Slave_TX_SDA_MISO_PIN     (0u == Slave_REMOVE_TX_SDA_MISO_PIN)
#define Slave_CTS_SCLK_PIN     (0u == Slave_REMOVE_CTS_SCLK_PIN)
#define Slave_RTS_SS0_PIN     (0u == Slave_REMOVE_RTS_SS0_PIN)
#define Slave_SS1_PIN                (0u == Slave_REMOVE_SS1_PIN)
#define Slave_SS2_PIN                (0u == Slave_REMOVE_SS2_PIN)
#define Slave_SS3_PIN                (0u == Slave_REMOVE_SS3_PIN)

/* Mode defined pins */
#define Slave_I2C_PINS               (0u == Slave_REMOVE_I2C_PINS)
#define Slave_SPI_MASTER_PINS        (0u == Slave_REMOVE_SPI_MASTER_PINS)
#define Slave_SPI_MASTER_SCLK_PIN    (0u == Slave_REMOVE_SPI_MASTER_SCLK_PIN)
#define Slave_SPI_MASTER_MOSI_PIN    (0u == Slave_REMOVE_SPI_MASTER_MOSI_PIN)
#define Slave_SPI_MASTER_MISO_PIN    (0u == Slave_REMOVE_SPI_MASTER_MISO_PIN)
#define Slave_SPI_MASTER_SS0_PIN     (0u == Slave_REMOVE_SPI_MASTER_SS0_PIN)
#define Slave_SPI_MASTER_SS1_PIN     (0u == Slave_REMOVE_SPI_MASTER_SS1_PIN)
#define Slave_SPI_MASTER_SS2_PIN     (0u == Slave_REMOVE_SPI_MASTER_SS2_PIN)
#define Slave_SPI_MASTER_SS3_PIN     (0u == Slave_REMOVE_SPI_MASTER_SS3_PIN)
#define Slave_SPI_SLAVE_PINS         (0u == Slave_REMOVE_SPI_SLAVE_PINS)
#define Slave_SPI_SLAVE_MOSI_PIN     (0u == Slave_REMOVE_SPI_SLAVE_MOSI_PIN)
#define Slave_SPI_SLAVE_MISO_PIN     (0u == Slave_REMOVE_SPI_SLAVE_MISO_PIN)
#define Slave_UART_TX_PIN            (0u == Slave_REMOVE_UART_TX_PIN)
#define Slave_UART_RX_TX_PIN         (0u == Slave_REMOVE_UART_RX_TX_PIN)
#define Slave_UART_RX_PIN            (0u == Slave_REMOVE_UART_RX_PIN)
#define Slave_UART_RX_WAKE_PIN       (0u == Slave_REMOVE_UART_RX_WAKE_PIN)
#define Slave_UART_RTS_PIN           (0u == Slave_REMOVE_UART_RTS_PIN)
#define Slave_UART_CTS_PIN           (0u == Slave_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (Slave_RX_WAKE_SCL_MOSI_PIN)
    #include "Slave_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (Slave_RX_SCL_MOSI) */

#if (Slave_RX_SCL_MOSI_PIN)
    #include "Slave_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (Slave_RX_SCL_MOSI) */

#if (Slave_TX_SDA_MISO_PIN)
    #include "Slave_uart_tx_i2c_sda_spi_miso.h"
#endif /* (Slave_TX_SDA_MISO) */

#if (Slave_CTS_SCLK_PIN)
    #include "Slave_uart_cts_spi_sclk.h"
#endif /* (Slave_CTS_SCLK) */

#if (Slave_RTS_SS0_PIN)
    #include "Slave_uart_rts_spi_ss0.h"
#endif /* (Slave_RTS_SS0_PIN) */

#if (Slave_SS1_PIN)
    #include "Slave_spi_ss1.h"
#endif /* (Slave_SS1_PIN) */

#if (Slave_SS2_PIN)
    #include "Slave_spi_ss2.h"
#endif /* (Slave_SS2_PIN) */

#if (Slave_SS3_PIN)
    #include "Slave_spi_ss3.h"
#endif /* (Slave_SS3_PIN) */

#if (Slave_I2C_PINS)
    #include "Slave_scl.h"
    #include "Slave_sda.h"
#endif /* (Slave_I2C_PINS) */

#if (Slave_SPI_MASTER_PINS)
#if (Slave_SPI_MASTER_SCLK_PIN)
    #include "Slave_sclk_m.h"
#endif /* (Slave_SPI_MASTER_SCLK_PIN) */

#if (Slave_SPI_MASTER_MOSI_PIN)
    #include "Slave_mosi_m.h"
#endif /* (Slave_SPI_MASTER_MOSI_PIN) */

#if (Slave_SPI_MASTER_MISO_PIN)
    #include "Slave_miso_m.h"
#endif /*(Slave_SPI_MASTER_MISO_PIN) */
#endif /* (Slave_SPI_MASTER_PINS) */

#if (Slave_SPI_SLAVE_PINS)
    #include "Slave_sclk_s.h"
    #include "Slave_ss_s.h"

#if (Slave_SPI_SLAVE_MOSI_PIN)
    #include "Slave_mosi_s.h"
#endif /* (Slave_SPI_SLAVE_MOSI_PIN) */

#if (Slave_SPI_SLAVE_MISO_PIN)
    #include "Slave_miso_s.h"
#endif /*(Slave_SPI_SLAVE_MISO_PIN) */
#endif /* (Slave_SPI_SLAVE_PINS) */

#if (Slave_SPI_MASTER_SS0_PIN)
    #include "Slave_ss0_m.h"
#endif /* (Slave_SPI_MASTER_SS0_PIN) */

#if (Slave_SPI_MASTER_SS1_PIN)
    #include "Slave_ss1_m.h"
#endif /* (Slave_SPI_MASTER_SS1_PIN) */

#if (Slave_SPI_MASTER_SS2_PIN)
    #include "Slave_ss2_m.h"
#endif /* (Slave_SPI_MASTER_SS2_PIN) */

#if (Slave_SPI_MASTER_SS3_PIN)
    #include "Slave_ss3_m.h"
#endif /* (Slave_SPI_MASTER_SS3_PIN) */

#if (Slave_UART_TX_PIN)
    #include "Slave_tx.h"
#endif /* (Slave_UART_TX_PIN) */

#if (Slave_UART_RX_TX_PIN)
    #include "Slave_rx_tx.h"
#endif /* (Slave_UART_RX_TX_PIN) */

#if (Slave_UART_RX_PIN)
    #include "Slave_rx.h"
#endif /* (Slave_UART_RX_PIN) */

#if (Slave_UART_RX_WAKE_PIN)
    #include "Slave_rx_wake.h"
#endif /* (Slave_UART_RX_WAKE_PIN) */

#if (Slave_UART_RTS_PIN)
    #include "Slave_rts.h"
#endif /* (Slave_UART_RTS_PIN) */

#if (Slave_UART_CTS_PIN)
    #include "Slave_cts.h"
#endif /* (Slave_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (Slave_RX_SCL_MOSI_PIN)
    #define Slave_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define Slave_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define Slave_RX_SCL_MOSI_HSIOM_MASK      (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define Slave_RX_SCL_MOSI_HSIOM_POS       (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define Slave_RX_SCL_MOSI_HSIOM_SEL_GPIO  (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define Slave_RX_SCL_MOSI_HSIOM_SEL_I2C   (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define Slave_RX_SCL_MOSI_HSIOM_SEL_SPI   (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define Slave_RX_SCL_MOSI_HSIOM_SEL_UART  (Slave_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (Slave_RX_WAKE_SCL_MOSI_PIN)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_MASK      (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_POS       (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define Slave_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (Slave_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define Slave_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) Slave_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define Slave_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) Slave_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define Slave_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (Slave_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define Slave_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) Slave_INTCFG_TYPE_MASK << \
                                                                           Slave_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins Slave_RX_SCL_MOSI_PIN or Slave_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (Slave_RX_SCL_MOSI_PIN) */

#if (Slave_TX_SDA_MISO_PIN)
    #define Slave_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define Slave_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define Slave_TX_SDA_MISO_HSIOM_MASK      (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define Slave_TX_SDA_MISO_HSIOM_POS       (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define Slave_TX_SDA_MISO_HSIOM_SEL_GPIO  (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define Slave_TX_SDA_MISO_HSIOM_SEL_I2C   (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define Slave_TX_SDA_MISO_HSIOM_SEL_SPI   (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define Slave_TX_SDA_MISO_HSIOM_SEL_UART  (Slave_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (Slave_TX_SDA_MISO_PIN) */

#if (Slave_CTS_SCLK_PIN)
    #define Slave_CTS_SCLK_HSIOM_REG   (*(reg32 *) Slave_uart_cts_spi_sclk__0__HSIOM)
    #define Slave_CTS_SCLK_HSIOM_PTR   ( (reg32 *) Slave_uart_cts_spi_sclk__0__HSIOM)
    
    #define Slave_CTS_SCLK_HSIOM_MASK      (Slave_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define Slave_CTS_SCLK_HSIOM_POS       (Slave_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define Slave_CTS_SCLK_HSIOM_SEL_GPIO  (Slave_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define Slave_CTS_SCLK_HSIOM_SEL_I2C   (Slave_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define Slave_CTS_SCLK_HSIOM_SEL_SPI   (Slave_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define Slave_CTS_SCLK_HSIOM_SEL_UART  (Slave_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (Slave_CTS_SCLK_PIN) */

#if (Slave_RTS_SS0_PIN)
    #define Slave_RTS_SS0_HSIOM_REG   (*(reg32 *) Slave_uart_rts_spi_ss0__0__HSIOM)
    #define Slave_RTS_SS0_HSIOM_PTR   ( (reg32 *) Slave_uart_rts_spi_ss0__0__HSIOM)
    
    #define Slave_RTS_SS0_HSIOM_MASK      (Slave_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define Slave_RTS_SS0_HSIOM_POS       (Slave_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define Slave_RTS_SS0_HSIOM_SEL_GPIO  (Slave_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define Slave_RTS_SS0_HSIOM_SEL_I2C   (Slave_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define Slave_RTS_SS0_HSIOM_SEL_SPI   (Slave_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1)
    #define Slave_RTS_SS0_HSIOM_SEL_UART  (Slave_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(Slave_CY_SCBIP_V0 || Slave_CY_SCBIP_V1) */
#endif /* (Slave_RTS_SS0_PIN) */

#if (Slave_SS1_PIN)
    #define Slave_SS1_HSIOM_REG  (*(reg32 *) Slave_spi_ss1__0__HSIOM)
    #define Slave_SS1_HSIOM_PTR  ( (reg32 *) Slave_spi_ss1__0__HSIOM)
    
    #define Slave_SS1_HSIOM_MASK     (Slave_spi_ss1__0__HSIOM_MASK)
    #define Slave_SS1_HSIOM_POS      (Slave_spi_ss1__0__HSIOM_SHIFT)
    #define Slave_SS1_HSIOM_SEL_GPIO (Slave_spi_ss1__0__HSIOM_GPIO)
    #define Slave_SS1_HSIOM_SEL_I2C  (Slave_spi_ss1__0__HSIOM_I2C)
    #define Slave_SS1_HSIOM_SEL_SPI  (Slave_spi_ss1__0__HSIOM_SPI)
#endif /* (Slave_SS1_PIN) */

#if (Slave_SS2_PIN)
    #define Slave_SS2_HSIOM_REG     (*(reg32 *) Slave_spi_ss2__0__HSIOM)
    #define Slave_SS2_HSIOM_PTR     ( (reg32 *) Slave_spi_ss2__0__HSIOM)
    
    #define Slave_SS2_HSIOM_MASK     (Slave_spi_ss2__0__HSIOM_MASK)
    #define Slave_SS2_HSIOM_POS      (Slave_spi_ss2__0__HSIOM_SHIFT)
    #define Slave_SS2_HSIOM_SEL_GPIO (Slave_spi_ss2__0__HSIOM_GPIO)
    #define Slave_SS2_HSIOM_SEL_I2C  (Slave_spi_ss2__0__HSIOM_I2C)
    #define Slave_SS2_HSIOM_SEL_SPI  (Slave_spi_ss2__0__HSIOM_SPI)
#endif /* (Slave_SS2_PIN) */

#if (Slave_SS3_PIN)
    #define Slave_SS3_HSIOM_REG     (*(reg32 *) Slave_spi_ss3__0__HSIOM)
    #define Slave_SS3_HSIOM_PTR     ( (reg32 *) Slave_spi_ss3__0__HSIOM)
    
    #define Slave_SS3_HSIOM_MASK     (Slave_spi_ss3__0__HSIOM_MASK)
    #define Slave_SS3_HSIOM_POS      (Slave_spi_ss3__0__HSIOM_SHIFT)
    #define Slave_SS3_HSIOM_SEL_GPIO (Slave_spi_ss3__0__HSIOM_GPIO)
    #define Slave_SS3_HSIOM_SEL_I2C  (Slave_spi_ss3__0__HSIOM_I2C)
    #define Slave_SS3_HSIOM_SEL_SPI  (Slave_spi_ss3__0__HSIOM_SPI)
#endif /* (Slave_SS3_PIN) */

#if (Slave_I2C_PINS)
    #define Slave_SCL_HSIOM_REG  (*(reg32 *) Slave_scl__0__HSIOM)
    #define Slave_SCL_HSIOM_PTR  ( (reg32 *) Slave_scl__0__HSIOM)
    
    #define Slave_SCL_HSIOM_MASK     (Slave_scl__0__HSIOM_MASK)
    #define Slave_SCL_HSIOM_POS      (Slave_scl__0__HSIOM_SHIFT)
    #define Slave_SCL_HSIOM_SEL_GPIO (Slave_sda__0__HSIOM_GPIO)
    #define Slave_SCL_HSIOM_SEL_I2C  (Slave_sda__0__HSIOM_I2C)
    
    #define Slave_SDA_HSIOM_REG  (*(reg32 *) Slave_sda__0__HSIOM)
    #define Slave_SDA_HSIOM_PTR  ( (reg32 *) Slave_sda__0__HSIOM)
    
    #define Slave_SDA_HSIOM_MASK     (Slave_sda__0__HSIOM_MASK)
    #define Slave_SDA_HSIOM_POS      (Slave_sda__0__HSIOM_SHIFT)
    #define Slave_SDA_HSIOM_SEL_GPIO (Slave_sda__0__HSIOM_GPIO)
    #define Slave_SDA_HSIOM_SEL_I2C  (Slave_sda__0__HSIOM_I2C)
#endif /* (Slave_I2C_PINS) */

#if (Slave_SPI_SLAVE_PINS)
    #define Slave_SCLK_S_HSIOM_REG   (*(reg32 *) Slave_sclk_s__0__HSIOM)
    #define Slave_SCLK_S_HSIOM_PTR   ( (reg32 *) Slave_sclk_s__0__HSIOM)
    
    #define Slave_SCLK_S_HSIOM_MASK      (Slave_sclk_s__0__HSIOM_MASK)
    #define Slave_SCLK_S_HSIOM_POS       (Slave_sclk_s__0__HSIOM_SHIFT)
    #define Slave_SCLK_S_HSIOM_SEL_GPIO  (Slave_sclk_s__0__HSIOM_GPIO)
    #define Slave_SCLK_S_HSIOM_SEL_SPI   (Slave_sclk_s__0__HSIOM_SPI)
    
    #define Slave_SS0_S_HSIOM_REG    (*(reg32 *) Slave_ss0_s__0__HSIOM)
    #define Slave_SS0_S_HSIOM_PTR    ( (reg32 *) Slave_ss0_s__0__HSIOM)
    
    #define Slave_SS0_S_HSIOM_MASK       (Slave_ss0_s__0__HSIOM_MASK)
    #define Slave_SS0_S_HSIOM_POS        (Slave_ss0_s__0__HSIOM_SHIFT)
    #define Slave_SS0_S_HSIOM_SEL_GPIO   (Slave_ss0_s__0__HSIOM_GPIO)  
    #define Slave_SS0_S_HSIOM_SEL_SPI    (Slave_ss0_s__0__HSIOM_SPI)
#endif /* (Slave_SPI_SLAVE_PINS) */

#if (Slave_SPI_SLAVE_MOSI_PIN)
    #define Slave_MOSI_S_HSIOM_REG   (*(reg32 *) Slave_mosi_s__0__HSIOM)
    #define Slave_MOSI_S_HSIOM_PTR   ( (reg32 *) Slave_mosi_s__0__HSIOM)
    
    #define Slave_MOSI_S_HSIOM_MASK      (Slave_mosi_s__0__HSIOM_MASK)
    #define Slave_MOSI_S_HSIOM_POS       (Slave_mosi_s__0__HSIOM_SHIFT)
    #define Slave_MOSI_S_HSIOM_SEL_GPIO  (Slave_mosi_s__0__HSIOM_GPIO)
    #define Slave_MOSI_S_HSIOM_SEL_SPI   (Slave_mosi_s__0__HSIOM_SPI)
#endif /* (Slave_SPI_SLAVE_MOSI_PIN) */

#if (Slave_SPI_SLAVE_MISO_PIN)
    #define Slave_MISO_S_HSIOM_REG   (*(reg32 *) Slave_miso_s__0__HSIOM)
    #define Slave_MISO_S_HSIOM_PTR   ( (reg32 *) Slave_miso_s__0__HSIOM)
    
    #define Slave_MISO_S_HSIOM_MASK      (Slave_miso_s__0__HSIOM_MASK)
    #define Slave_MISO_S_HSIOM_POS       (Slave_miso_s__0__HSIOM_SHIFT)
    #define Slave_MISO_S_HSIOM_SEL_GPIO  (Slave_miso_s__0__HSIOM_GPIO)
    #define Slave_MISO_S_HSIOM_SEL_SPI   (Slave_miso_s__0__HSIOM_SPI)
#endif /* (Slave_SPI_SLAVE_MISO_PIN) */

#if (Slave_SPI_MASTER_MISO_PIN)
    #define Slave_MISO_M_HSIOM_REG   (*(reg32 *) Slave_miso_m__0__HSIOM)
    #define Slave_MISO_M_HSIOM_PTR   ( (reg32 *) Slave_miso_m__0__HSIOM)
    
    #define Slave_MISO_M_HSIOM_MASK      (Slave_miso_m__0__HSIOM_MASK)
    #define Slave_MISO_M_HSIOM_POS       (Slave_miso_m__0__HSIOM_SHIFT)
    #define Slave_MISO_M_HSIOM_SEL_GPIO  (Slave_miso_m__0__HSIOM_GPIO)
    #define Slave_MISO_M_HSIOM_SEL_SPI   (Slave_miso_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_MISO_PIN) */

#if (Slave_SPI_MASTER_MOSI_PIN)
    #define Slave_MOSI_M_HSIOM_REG   (*(reg32 *) Slave_mosi_m__0__HSIOM)
    #define Slave_MOSI_M_HSIOM_PTR   ( (reg32 *) Slave_mosi_m__0__HSIOM)
    
    #define Slave_MOSI_M_HSIOM_MASK      (Slave_mosi_m__0__HSIOM_MASK)
    #define Slave_MOSI_M_HSIOM_POS       (Slave_mosi_m__0__HSIOM_SHIFT)
    #define Slave_MOSI_M_HSIOM_SEL_GPIO  (Slave_mosi_m__0__HSIOM_GPIO)
    #define Slave_MOSI_M_HSIOM_SEL_SPI   (Slave_mosi_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_MOSI_PIN) */

#if (Slave_SPI_MASTER_SCLK_PIN)
    #define Slave_SCLK_M_HSIOM_REG   (*(reg32 *) Slave_sclk_m__0__HSIOM)
    #define Slave_SCLK_M_HSIOM_PTR   ( (reg32 *) Slave_sclk_m__0__HSIOM)
    
    #define Slave_SCLK_M_HSIOM_MASK      (Slave_sclk_m__0__HSIOM_MASK)
    #define Slave_SCLK_M_HSIOM_POS       (Slave_sclk_m__0__HSIOM_SHIFT)
    #define Slave_SCLK_M_HSIOM_SEL_GPIO  (Slave_sclk_m__0__HSIOM_GPIO)
    #define Slave_SCLK_M_HSIOM_SEL_SPI   (Slave_sclk_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_SCLK_PIN) */

#if (Slave_SPI_MASTER_SS0_PIN)
    #define Slave_SS0_M_HSIOM_REG    (*(reg32 *) Slave_ss0_m__0__HSIOM)
    #define Slave_SS0_M_HSIOM_PTR    ( (reg32 *) Slave_ss0_m__0__HSIOM)
    
    #define Slave_SS0_M_HSIOM_MASK       (Slave_ss0_m__0__HSIOM_MASK)
    #define Slave_SS0_M_HSIOM_POS        (Slave_ss0_m__0__HSIOM_SHIFT)
    #define Slave_SS0_M_HSIOM_SEL_GPIO   (Slave_ss0_m__0__HSIOM_GPIO)
    #define Slave_SS0_M_HSIOM_SEL_SPI    (Slave_ss0_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_SS0_PIN) */

#if (Slave_SPI_MASTER_SS1_PIN)
    #define Slave_SS1_M_HSIOM_REG    (*(reg32 *) Slave_ss1_m__0__HSIOM)
    #define Slave_SS1_M_HSIOM_PTR    ( (reg32 *) Slave_ss1_m__0__HSIOM)
    
    #define Slave_SS1_M_HSIOM_MASK       (Slave_ss1_m__0__HSIOM_MASK)
    #define Slave_SS1_M_HSIOM_POS        (Slave_ss1_m__0__HSIOM_SHIFT)
    #define Slave_SS1_M_HSIOM_SEL_GPIO   (Slave_ss1_m__0__HSIOM_GPIO)
    #define Slave_SS1_M_HSIOM_SEL_SPI    (Slave_ss1_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_SS1_PIN) */

#if (Slave_SPI_MASTER_SS2_PIN)
    #define Slave_SS2_M_HSIOM_REG    (*(reg32 *) Slave_ss2_m__0__HSIOM)
    #define Slave_SS2_M_HSIOM_PTR    ( (reg32 *) Slave_ss2_m__0__HSIOM)
    
    #define Slave_SS2_M_HSIOM_MASK       (Slave_ss2_m__0__HSIOM_MASK)
    #define Slave_SS2_M_HSIOM_POS        (Slave_ss2_m__0__HSIOM_SHIFT)
    #define Slave_SS2_M_HSIOM_SEL_GPIO   (Slave_ss2_m__0__HSIOM_GPIO)
    #define Slave_SS2_M_HSIOM_SEL_SPI    (Slave_ss2_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_SS2_PIN) */

#if (Slave_SPI_MASTER_SS3_PIN)
    #define Slave_SS3_M_HSIOM_REG    (*(reg32 *) Slave_ss3_m__0__HSIOM)
    #define Slave_SS3_M_HSIOM_PTR    ( (reg32 *) Slave_ss3_m__0__HSIOM)
    
    #define Slave_SS3_M_HSIOM_MASK      (Slave_ss3_m__0__HSIOM_MASK)
    #define Slave_SS3_M_HSIOM_POS       (Slave_ss3_m__0__HSIOM_SHIFT)
    #define Slave_SS3_M_HSIOM_SEL_GPIO  (Slave_ss3_m__0__HSIOM_GPIO)
    #define Slave_SS3_M_HSIOM_SEL_SPI   (Slave_ss3_m__0__HSIOM_SPI)
#endif /* (Slave_SPI_MASTER_SS3_PIN) */

#if (Slave_UART_RX_PIN)
    #define Slave_RX_HSIOM_REG   (*(reg32 *) Slave_rx__0__HSIOM)
    #define Slave_RX_HSIOM_PTR   ( (reg32 *) Slave_rx__0__HSIOM)
    
    #define Slave_RX_HSIOM_MASK      (Slave_rx__0__HSIOM_MASK)
    #define Slave_RX_HSIOM_POS       (Slave_rx__0__HSIOM_SHIFT)
    #define Slave_RX_HSIOM_SEL_GPIO  (Slave_rx__0__HSIOM_GPIO)
    #define Slave_RX_HSIOM_SEL_UART  (Slave_rx__0__HSIOM_UART)
#endif /* (Slave_UART_RX_PIN) */

#if (Slave_UART_RX_WAKE_PIN)
    #define Slave_RX_WAKE_HSIOM_REG   (*(reg32 *) Slave_rx_wake__0__HSIOM)
    #define Slave_RX_WAKE_HSIOM_PTR   ( (reg32 *) Slave_rx_wake__0__HSIOM)
    
    #define Slave_RX_WAKE_HSIOM_MASK      (Slave_rx_wake__0__HSIOM_MASK)
    #define Slave_RX_WAKE_HSIOM_POS       (Slave_rx_wake__0__HSIOM_SHIFT)
    #define Slave_RX_WAKE_HSIOM_SEL_GPIO  (Slave_rx_wake__0__HSIOM_GPIO)
    #define Slave_RX_WAKE_HSIOM_SEL_UART  (Slave_rx_wake__0__HSIOM_UART)
#endif /* (Slave_UART_WAKE_RX_PIN) */

#if (Slave_UART_CTS_PIN)
    #define Slave_CTS_HSIOM_REG   (*(reg32 *) Slave_cts__0__HSIOM)
    #define Slave_CTS_HSIOM_PTR   ( (reg32 *) Slave_cts__0__HSIOM)
    
    #define Slave_CTS_HSIOM_MASK      (Slave_cts__0__HSIOM_MASK)
    #define Slave_CTS_HSIOM_POS       (Slave_cts__0__HSIOM_SHIFT)
    #define Slave_CTS_HSIOM_SEL_GPIO  (Slave_cts__0__HSIOM_GPIO)
    #define Slave_CTS_HSIOM_SEL_UART  (Slave_cts__0__HSIOM_UART)
#endif /* (Slave_UART_CTS_PIN) */

#if (Slave_UART_TX_PIN)
    #define Slave_TX_HSIOM_REG   (*(reg32 *) Slave_tx__0__HSIOM)
    #define Slave_TX_HSIOM_PTR   ( (reg32 *) Slave_tx__0__HSIOM)
    
    #define Slave_TX_HSIOM_MASK      (Slave_tx__0__HSIOM_MASK)
    #define Slave_TX_HSIOM_POS       (Slave_tx__0__HSIOM_SHIFT)
    #define Slave_TX_HSIOM_SEL_GPIO  (Slave_tx__0__HSIOM_GPIO)
    #define Slave_TX_HSIOM_SEL_UART  (Slave_tx__0__HSIOM_UART)
#endif /* (Slave_UART_TX_PIN) */

#if (Slave_UART_RX_TX_PIN)
    #define Slave_RX_TX_HSIOM_REG   (*(reg32 *) Slave_rx_tx__0__HSIOM)
    #define Slave_RX_TX_HSIOM_PTR   ( (reg32 *) Slave_rx_tx__0__HSIOM)
    
    #define Slave_RX_TX_HSIOM_MASK      (Slave_rx_tx__0__HSIOM_MASK)
    #define Slave_RX_TX_HSIOM_POS       (Slave_rx_tx__0__HSIOM_SHIFT)
    #define Slave_RX_TX_HSIOM_SEL_GPIO  (Slave_rx_tx__0__HSIOM_GPIO)
    #define Slave_RX_TX_HSIOM_SEL_UART  (Slave_rx_tx__0__HSIOM_UART)
#endif /* (Slave_UART_RX_TX_PIN) */

#if (Slave_UART_RTS_PIN)
    #define Slave_RTS_HSIOM_REG      (*(reg32 *) Slave_rts__0__HSIOM)
    #define Slave_RTS_HSIOM_PTR      ( (reg32 *) Slave_rts__0__HSIOM)
    
    #define Slave_RTS_HSIOM_MASK     (Slave_rts__0__HSIOM_MASK)
    #define Slave_RTS_HSIOM_POS      (Slave_rts__0__HSIOM_SHIFT)    
    #define Slave_RTS_HSIOM_SEL_GPIO (Slave_rts__0__HSIOM_GPIO)
    #define Slave_RTS_HSIOM_SEL_UART (Slave_rts__0__HSIOM_UART)    
#endif /* (Slave_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define Slave_HSIOM_DEF_SEL      (0x00u)
#define Slave_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for Slave_CY_SCBIP_V0 
* and Slave_CY_SCBIP_V1. It is not recommended to use them for 
* Slave_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define Slave_HSIOM_UART_SEL     (0x09u)
#define Slave_HSIOM_I2C_SEL      (0x0Eu)
#define Slave_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define Slave_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define Slave_RX_SCL_MOSI_PIN_INDEX       (0u)
#define Slave_TX_SDA_MISO_PIN_INDEX       (1u)
#define Slave_CTS_SCLK_PIN_INDEX       (2u)
#define Slave_RTS_SS0_PIN_INDEX       (3u)
#define Slave_SS1_PIN_INDEX                  (4u)
#define Slave_SS2_PIN_INDEX                  (5u)
#define Slave_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define Slave_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << Slave_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define Slave_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << Slave_RX_SCL_MOSI_PIN_INDEX)
#define Slave_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << Slave_TX_SDA_MISO_PIN_INDEX)
#define Slave_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << Slave_CTS_SCLK_PIN_INDEX)
#define Slave_RTS_SS0_PIN_MASK     ((uint32) 0x01u << Slave_RTS_SS0_PIN_INDEX)
#define Slave_SS1_PIN_MASK                ((uint32) 0x01u << Slave_SS1_PIN_INDEX)
#define Slave_SS2_PIN_MASK                ((uint32) 0x01u << Slave_SS2_PIN_INDEX)
#define Slave_SS3_PIN_MASK                ((uint32) 0x01u << Slave_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define Slave_INTCFG_TYPE_MASK           (0x03u)
#define Slave_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define Slave_PIN_DM_ALG_HIZ  (0u)
#define Slave_PIN_DM_DIG_HIZ  (1u)
#define Slave_PIN_DM_OD_LO    (4u)
#define Slave_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define Slave_DM_MASK    (0x7u)
#define Slave_DM_SIZE    (3u)
#define Slave_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) Slave_DM_MASK << (Slave_DM_SIZE * (pos)))) >> \
                                                              (Slave_DM_SIZE * (pos)) )

#if (Slave_TX_SDA_MISO_PIN)
    #define Slave_CHECK_TX_SDA_MISO_PIN_USED \
                (Slave_PIN_DM_ALG_HIZ != \
                    Slave_GET_P4_PIN_DM(Slave_uart_tx_i2c_sda_spi_miso_PC, \
                                                   Slave_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (Slave_TX_SDA_MISO_PIN) */

#if (Slave_RTS_SS0_PIN)
    #define Slave_CHECK_RTS_SS0_PIN_USED \
                (Slave_PIN_DM_ALG_HIZ != \
                    Slave_GET_P4_PIN_DM(Slave_uart_rts_spi_ss0_PC, \
                                                   Slave_uart_rts_spi_ss0_SHIFT))
#endif /* (Slave_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define Slave_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define Slave_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define Slave_SET_HSIOM_SEL(reg, mask, pos, sel) Slave_SET_REGISTER_BITS(reg, mask, pos, sel)
#define Slave_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        Slave_SET_REGISTER_BITS(reg, mask, pos, intType)
#define Slave_SET_INP_DIS(reg, mask, val) Slave_SET_REGISTER_BIT(reg, mask, val)

/* Slave_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  Slave_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (Slave_CY_SCBIP_V0)
#if (Slave_I2C_PINS)
    #define Slave_SET_I2C_SCL_DR(val) Slave_scl_Write(val)

    #define Slave_SET_I2C_SCL_HSIOM_SEL(sel) \
                          Slave_SET_HSIOM_SEL(Slave_SCL_HSIOM_REG,  \
                                                         Slave_SCL_HSIOM_MASK, \
                                                         Slave_SCL_HSIOM_POS,  \
                                                         (sel))
    #define Slave_WAIT_SCL_SET_HIGH  (0u == Slave_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (Slave_RX_WAKE_SCL_MOSI_PIN)
    #define Slave_SET_I2C_SCL_DR(val) \
                            Slave_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define Slave_SET_I2C_SCL_HSIOM_SEL(sel) \
                    Slave_SET_HSIOM_SEL(Slave_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   Slave_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   Slave_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define Slave_WAIT_SCL_SET_HIGH  (0u == Slave_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (Slave_RX_SCL_MOSI_PIN)
    #define Slave_SET_I2C_SCL_DR(val) \
                            Slave_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define Slave_SET_I2C_SCL_HSIOM_SEL(sel) \
                            Slave_SET_HSIOM_SEL(Slave_RX_SCL_MOSI_HSIOM_REG,  \
                                                           Slave_RX_SCL_MOSI_HSIOM_MASK, \
                                                           Slave_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define Slave_WAIT_SCL_SET_HIGH  (0u == Slave_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define Slave_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define Slave_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define Slave_WAIT_SCL_SET_HIGH  (0u)
#endif /* (Slave_I2C_PINS) */

/* SCB I2C: sda signal */
#if (Slave_I2C_PINS)
    #define Slave_WAIT_SDA_SET_HIGH  (0u == Slave_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (Slave_TX_SDA_MISO_PIN)
    #define Slave_WAIT_SDA_SET_HIGH  (0u == Slave_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define Slave_WAIT_SDA_SET_HIGH  (0u)
#endif /* (Slave_MOSI_SCL_RX_PIN) */
#endif /* (Slave_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (Slave_RX_SCL_MOSI_PIN)
    #define Slave_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (Slave_RX_WAKE_SCL_MOSI_PIN)
    #define Slave_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) Slave_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(Slave_UART_RX_WAKE_PIN)
    #define Slave_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) Slave_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (Slave_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define Slave_REMOVE_MOSI_SCL_RX_WAKE_PIN    Slave_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define Slave_REMOVE_MOSI_SCL_RX_PIN         Slave_REMOVE_RX_SCL_MOSI_PIN
#define Slave_REMOVE_MISO_SDA_TX_PIN         Slave_REMOVE_TX_SDA_MISO_PIN
#ifndef Slave_REMOVE_SCLK_PIN
#define Slave_REMOVE_SCLK_PIN                Slave_REMOVE_CTS_SCLK_PIN
#endif /* Slave_REMOVE_SCLK_PIN */
#ifndef Slave_REMOVE_SS0_PIN
#define Slave_REMOVE_SS0_PIN                 Slave_REMOVE_RTS_SS0_PIN
#endif /* Slave_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define Slave_MOSI_SCL_RX_WAKE_PIN   Slave_RX_WAKE_SCL_MOSI_PIN
#define Slave_MOSI_SCL_RX_PIN        Slave_RX_SCL_MOSI_PIN
#define Slave_MISO_SDA_TX_PIN        Slave_TX_SDA_MISO_PIN
#ifndef Slave_SCLK_PIN
#define Slave_SCLK_PIN               Slave_CTS_SCLK_PIN
#endif /* Slave_SCLK_PIN */
#ifndef Slave_SS0_PIN
#define Slave_SS0_PIN                Slave_RTS_SS0_PIN
#endif /* Slave_SS0_PIN */

#if (Slave_MOSI_SCL_RX_WAKE_PIN)
    #define Slave_MOSI_SCL_RX_WAKE_HSIOM_REG     Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_WAKE_HSIOM_PTR     Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_WAKE_HSIOM_MASK    Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_WAKE_HSIOM_POS     Slave_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define Slave_MOSI_SCL_RX_WAKE_INTCFG_REG    Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_WAKE_INTCFG_PTR    Slave_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define Slave_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  Slave_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (Slave_RX_WAKE_SCL_MOSI_PIN) */

#if (Slave_MOSI_SCL_RX_PIN)
    #define Slave_MOSI_SCL_RX_HSIOM_REG      Slave_RX_SCL_MOSI_HSIOM_REG
    #define Slave_MOSI_SCL_RX_HSIOM_PTR      Slave_RX_SCL_MOSI_HSIOM_PTR
    #define Slave_MOSI_SCL_RX_HSIOM_MASK     Slave_RX_SCL_MOSI_HSIOM_MASK
    #define Slave_MOSI_SCL_RX_HSIOM_POS      Slave_RX_SCL_MOSI_HSIOM_POS
#endif /* (Slave_MOSI_SCL_RX_PIN) */

#if (Slave_MISO_SDA_TX_PIN)
    #define Slave_MISO_SDA_TX_HSIOM_REG      Slave_TX_SDA_MISO_HSIOM_REG
    #define Slave_MISO_SDA_TX_HSIOM_PTR      Slave_TX_SDA_MISO_HSIOM_REG
    #define Slave_MISO_SDA_TX_HSIOM_MASK     Slave_TX_SDA_MISO_HSIOM_REG
    #define Slave_MISO_SDA_TX_HSIOM_POS      Slave_TX_SDA_MISO_HSIOM_REG
#endif /* (Slave_MISO_SDA_TX_PIN_PIN) */

#if (Slave_SCLK_PIN)
    #ifndef Slave_SCLK_HSIOM_REG
    #define Slave_SCLK_HSIOM_REG     Slave_CTS_SCLK_HSIOM_REG
    #define Slave_SCLK_HSIOM_PTR     Slave_CTS_SCLK_HSIOM_PTR
    #define Slave_SCLK_HSIOM_MASK    Slave_CTS_SCLK_HSIOM_MASK
    #define Slave_SCLK_HSIOM_POS     Slave_CTS_SCLK_HSIOM_POS
    #endif /* Slave_SCLK_HSIOM_REG */
#endif /* (Slave_SCLK_PIN) */

#if (Slave_SS0_PIN)
    #ifndef Slave_SS0_HSIOM_REG
    #define Slave_SS0_HSIOM_REG      Slave_RTS_SS0_HSIOM_REG
    #define Slave_SS0_HSIOM_PTR      Slave_RTS_SS0_HSIOM_PTR
    #define Slave_SS0_HSIOM_MASK     Slave_RTS_SS0_HSIOM_MASK
    #define Slave_SS0_HSIOM_POS      Slave_RTS_SS0_HSIOM_POS
    #endif /* Slave_SS0_HSIOM_REG */
#endif /* (Slave_SS0_PIN) */

#define Slave_MOSI_SCL_RX_WAKE_PIN_INDEX Slave_RX_WAKE_SCL_MOSI_PIN_INDEX
#define Slave_MOSI_SCL_RX_PIN_INDEX      Slave_RX_SCL_MOSI_PIN_INDEX
#define Slave_MISO_SDA_TX_PIN_INDEX      Slave_TX_SDA_MISO_PIN_INDEX
#ifndef Slave_SCLK_PIN_INDEX
#define Slave_SCLK_PIN_INDEX             Slave_CTS_SCLK_PIN_INDEX
#endif /* Slave_SCLK_PIN_INDEX */
#ifndef Slave_SS0_PIN_INDEX
#define Slave_SS0_PIN_INDEX              Slave_RTS_SS0_PIN_INDEX
#endif /* Slave_SS0_PIN_INDEX */

#define Slave_MOSI_SCL_RX_WAKE_PIN_MASK Slave_RX_WAKE_SCL_MOSI_PIN_MASK
#define Slave_MOSI_SCL_RX_PIN_MASK      Slave_RX_SCL_MOSI_PIN_MASK
#define Slave_MISO_SDA_TX_PIN_MASK      Slave_TX_SDA_MISO_PIN_MASK
#ifndef Slave_SCLK_PIN_MASK
#define Slave_SCLK_PIN_MASK             Slave_CTS_SCLK_PIN_MASK
#endif /* Slave_SCLK_PIN_MASK */
#ifndef Slave_SS0_PIN_MASK
#define Slave_SS0_PIN_MASK              Slave_RTS_SS0_PIN_MASK
#endif /* Slave_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_Slave_H) */


/* [] END OF FILE */
