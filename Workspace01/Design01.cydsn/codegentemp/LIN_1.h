/******************************************************************************
* File Name: LIN_1.h
* Version 5.0
*
* Description:
*  This file provides constants and parameter values for the LIN Slave Component.
*
*******************************************************************************
* Copyright 2011-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
******************************************************************************/


#if !defined(CY_LIN_LIN_1_H)
#define CY_LIN_LIN_1_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

#include "LIN_1_SCB.h"
#include "LIN_1_SCB_SPI_UART.h"
#include "LIN_1_IntClk.h"
#include "LIN_1_ISR.h"

#if !defined LIN_API_TYPES
#define LIN_API_TYPES
/******************************************************************************
*                        LIN Core Types 
******************************************************************************/
    typedef unsigned char  l_bool;
    typedef unsigned char  l_u8;
    typedef unsigned short l_u16;
    typedef unsigned char  l_irqmask;
    typedef unsigned char  l_ioctl_op;
    typedef unsigned char  l_signal_handle;
    typedef unsigned char  l_ifc_handle;
    typedef unsigned char  l_flag_handle;
#endif /*!defined LIN_API_TYPES */

/******************************************************************************
*                 Conditional Compilation Parameters
******************************************************************************/

/* General */
#define LIN_1_RESPONSE_ERROR_SIGNAL  (1u)
#define LIN_1_ISO_17987              (1u)
#define LIN_1_SAE_J2602              (0u)
#define LIN_1_LIN_2_2                (1u)
#define LIN_1_LIN_2_0                (0u)
#define LIN_1_LIN_1_3                (0u)

#define LIN_1_SAE_J2602_ENABLED      (0u != LIN_1_SAE_J2602)

#define LIN_1_INACTIVITY_ENABLED     (1u)
#define LIN_1_INACTIVITY_THRESHOLD   (7000u)

/*  LDF/NCF file version for ISO_17987 */
#if (0 != LIN_1_ISO_17987)
    #define LIN_1_FILE_REVISION_major    (0u)
    #define LIN_1_FILE_REVISION_minor    (0u)
    #define LIN_1_FILE_REVISION_sub      (0u)
    #define LIN_1_FILE_REVISION_source   (0u)
    #define LIN_1_FILE_REVISION_source_LDF (1u)
    #define LIN_1_FILE_REVISION_source_NCF (2u)
    #define LIN_1_FILE_REVISION_DEFINED  ((LIN_1_FILE_REVISION_source ==  LIN_1_FILE_REVISION_source_LDF) || (LIN_1_FILE_REVISION_source == LIN_1_FILE_REVISION_source_NCF))
#endif

/* Break threshold detection value assuming 16x oversampling */
#define LIN_1_BREAK_THRESHOLD        (11u)

/* Baud rate */
#define LIN_1_AUTO_BAUD_RATE_SYNC    (1u)
#define LIN_1_BAUD_RATE              (19200u)
#if (0u == LIN_1_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Disabled */
/* BREAK to SYNC timeout is 21 bits from break detection +1 ms CySysTick */
    #define LIN_1_BREAK_TO_SYNC_TIMEOUT  (l_u16)((21000u/LIN_1_BAUD_RATE)+1u) 
#endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

/* Transport Layer */
#define LIN_1_TL_ENABLED             (1u)
#define LIN_1_TL_INITIAL_NAD         (33u)
#define LIN_1_TL_API_FORMAT          (1u)
#define LIN_1_TL_BUF_LEN_MAX         (512u)
#define LIN_1_TL_TX_QUEUE_LEN        (32u)
#define LIN_1_TL_RX_QUEUE_LEN        (32u)

#define LIN_1_TL_USED                (1u == LIN_1_TL_ENABLED)
#define LIN_1_COOKED_TL_API          (1u == LIN_1_TL_API_FORMAT)
#define LIN_1_RAW_TL_API             (0u == LIN_1_TL_API_FORMAT)

/* Config. Services */
#define LIN_1_CS_ENABLED             (0u)
#define LIN_1_CS_SUPPLIER_ID         (195u)
#define LIN_1_CS_FUNCTION_ID         (65534u)
#define LIN_1_CS_VARIANT             (0u)
#define LIN_1_CS_SEL_SERVICES01      (128u)

#define LIN_1_CFG_SERVICES_ENABLED   (1u == LIN_1_CS_ENABLED)


#define LIN_1_ABS(x)(( (x) < 0 ) ? -(x) : (x) )


/* General frames and signals information */
#define LIN_1_NUM_FRAMES            (0x01u)
#define LIN_1_NUM_ET_FRAMES         (0x00u)
#define LIN_1_NUM_SIGNALS           (0x14u)
#define LIN_1_NUM_UNIQUE_SIGNALS    (0x14u)
#define LIN_1_RESPONSE_ERROR_FRAME_INDEX    (0x00u)


#define LIN_1_NUM_SERVICE_FRAMES     (0x02u)     /* MRF and SRF */

#define LIN_1_SIG_FRAME_FLAGS_SIZE   (((LIN_1_NUM_FRAMES         + \
                                                   LIN_1_NUM_SIGNALS        + \
                                                   LIN_1_NUM_UNIQUE_SIGNALS + \
                                                   LIN_1_NUM_SERVICE_FRAMES) / 8u) + 1u)

#define LIN_1_ET_FRAMES_FLAGS_SIZE   (((LIN_1_NUM_ET_FRAMES) / 8u) + 1u)

#define LIN_1_INACTIVITY_THRESHOLD_IN_100_MS  (LIN_1_INACTIVITY_THRESHOLD / 100u)
#define LIN_1_INACTIVITY_THRESHOLD_IN_1_MS    (LIN_1_INACTIVITY_THRESHOLD )

#if (LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED)
/* Bit masks for selected services */
    #define LIN_1_NCS_0xB0_SEL   (0x01u)
    #define LIN_1_NCS_0xB1_SEL   (0x02u)
    #define LIN_1_NCS_0xB2_SEL   (0x04u)
    #define LIN_1_NCS_0xB3_SEL   (0x08u)
    #define LIN_1_NCS_0xB4_SEL   (0x10u)
    #define LIN_1_NCS_0xB5_SEL   (0x20u)
    #define LIN_1_NCS_0xB6_SEL   (0x40u)
    #define LIN_1_NCS_0xB7_SEL   (0x80u)
/* Macro for simple check of selected services */
    #define LIN_1_NCS_0xB0_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL))
    #define LIN_1_NCS_0xB1_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))
    #define LIN_1_NCS_0xB2_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL))
    #define LIN_1_NCS_0xB3_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))
    #define LIN_1_NCS_0xB4_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB4_SEL))
    #define LIN_1_NCS_0xB5_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL))
    #define LIN_1_NCS_0xB6_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB6_SEL))
    #define LIN_1_NCS_0xB7_ENABLED (0 != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB7_SEL))
#endif /* (LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED) */


/******************************************************************************
*     Data Types Definitions
******************************************************************************/

/* Detailed PID information */
typedef struct
{
    l_u8   param;
    volatile l_u8 *dataPtr;
    /* Unconditional PID associated if this is the Event Frame  */
    volatile l_u8 associatedPID;

} LIN_1_PID_INFO_TABLE;

/* LIN Slave Configuration */
typedef struct
{
    #if (LIN_1_TL_USED)
        /* Initial NAD */
        l_u8 initialNad;
    #endif /* LIN_1_TL_USED */

    /* PID Table */
    l_u8 pidTable[LIN_1_NUM_FRAMES];
 } LIN_1_SLAVE_CONFIG;
 
typedef struct
{   
/* Used as the second parameter for l_ifc_ioctl_LIN_1() */
    l_u8 index;
    l_u8 PID;
} LIN_1_NEW_PID; 

typedef struct
{   
/* Used as the second parameter for l_ifc_ioctl_LIN_1() */
    l_u16 messageID;
    l_u8 PID;
} LIN_1_NEW_PID_BY_MSG_ID; 

#if LIN_1_CFG_SERVICES_ENABLED
    /* LIN Slave Identification */
    typedef struct
    {
        l_u16 supplierId;
        l_u16 functionId;
        l_u8  variant;
    } LIN_1_SLAVE_ID;
#endif /* LIN_1_CFG_SERVICES_ENABLED */

/* Sleep Mode API Support */
typedef struct
{
    l_u8 enableState;
    l_u8 control;
    l_u8 statusMask;

    #if (1u == LIN_1_INACTIVITY_ENABLED)
        l_u8 inactivityDiv0;
        l_u8 inactivityDiv1;
    #endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */

} LIN_1_BACKUP_STRUCT;


/******************************************************************************
*  Function Prototypes
******************************************************************************/

cystatus LIN_1_TimerEnableInt(void);
void     LIN_1_TimerDisableInt(void);
l_bool l_sys_init(void);
l_bool LIN_1_Start(void);
void   LIN_1_Stop(void);
#if (LIN_1_SAE_J2602_ENABLED)
    void LIN_1_j2602Status_Reset(void);
#endif  /* LIN_1_SAE_J2602_ENABLED */

/* Core API: Signal Interaction  Functions */

void l_bool_wr_MFS_RsErr(l_bool v);
void l_bool_wr_LIN_1(l_signal_handle sss, l_bool v);
void l_u8_wr_MFS_Return_switch_signal(l_u8 v);
void l_u8_wr_MFS_Menu_switch_signal(l_u8 v);
void l_u8_wr_MFS_Up_switch_signal(l_u8 v);
void l_u8_wr_MFS_Down_switch_signal(l_u8 v);
void l_u8_wr_MFS_OK_switch_signal(l_u8 v);
void l_u8_wr_MFS_Speech_switch_signal(l_u8 v);
void l_u8_wr_MFS_Mode_switch_signal(l_u8 v);
void l_u8_wr_MFS_VolPlus_switch_signal(l_u8 v);
void l_u8_wr_MFS_VolReduce_switch_signal(l_u8 v);
void l_u8_wr_MFS_Mute_switch_signal(l_u8 v);
void l_u8_wr_MFS_SeekPlus_switch_signal(l_u8 v);
void l_u8_wr_MFS_SeekReduce_switch_signal(l_u8 v);
void l_u8_wr_MFS_Crusie_switch_signal(l_u8 v);
void l_u8_wr_MFS_RESPlus_switch_signal(l_u8 v);
void l_u8_wr_MFS_SETReduce_switch_signal(l_u8 v);
void l_u8_wr_MFS_DIST_switch_signal(l_u8 v);
void l_u8_wr_MFS_ADAS_switch_signal(l_u8 v);
void l_u8_wr_MFS_DiagInfoSW(l_u8 v);
void l_u8_wr_MFS_Answer_switch_signal(l_u8 v);
void l_u8_wr_LIN_1(l_signal_handle sss, l_u8 v);

/* Signals API for current instance definition */
#define  L_BOOL_RD_INST1     (0x00u)
#define  L_U8_RD_INST1       (0x00u)
#define  L_U16_RD_INST1      (0x00u)
#define  L_BYTES_RD_INST1    (0x00u)
#define  L_BOOL_WR_INST1     (0x01u)
#define  L_U8_WR_INST1       (0x01u)
#define  L_U16_WR_INST1      (0x00u)
#define  L_BYTES_WR_INST1    (0x00u)

/* Signals API for second instance definition */
#define  L_BOOL_RD_INST2     (0x00u)
#define  L_U8_RD_INST2       (0x00u)
#define  L_U16_RD_INST2      (0x00u)
#define  L_BYTES_RD_INST2    (0x00u)
#define  L_BOOL_WR_INST2     (0x00u)
#define  L_U8_WR_INST2       (0x00u)
#define  L_U16_WR_INST2      (0x00u)
#define  L_BYTES_WR_INST2    (0x00u)

/* Core API Functions: Notification */
l_bool l_flg_tst_MRF(void);
l_bool l_flg_tst_SRF(void);
l_bool l_flg_tst_MFS_Return_switch_signal(void);
l_bool l_flg_tst_MFS_Menu_switch_signal(void);
l_bool l_flg_tst_MFS_Up_switch_signal(void);
l_bool l_flg_tst_MFS_Down_switch_signal(void);
l_bool l_flg_tst_MFS_OK_switch_signal(void);
l_bool l_flg_tst_MFS_Speech_switch_signal(void);
l_bool l_flg_tst_MFS_Mode_switch_signal(void);
l_bool l_flg_tst_MFS_VolPlus_switch_signal(void);
l_bool l_flg_tst_MFS_VolReduce_switch_signal(void);
l_bool l_flg_tst_MFS_Mute_switch_signal(void);
l_bool l_flg_tst_MFS_SeekPlus_switch_signal(void);
l_bool l_flg_tst_MFS_SeekReduce_switch_signal(void);
l_bool l_flg_tst_MFS_Crusie_switch_signal(void);
l_bool l_flg_tst_MFS_RESPlus_switch_signal(void);
l_bool l_flg_tst_MFS_SETReduce_switch_signal(void);
l_bool l_flg_tst_MFS_DIST_switch_signal(void);
l_bool l_flg_tst_MFS_ADAS_switch_signal(void);
l_bool l_flg_tst_MFS_DiagInfoSW(void);
l_bool l_flg_tst_MFS_Answer_switch_signal(void);
l_bool l_flg_tst_MFS_LIN_1(void);
l_bool l_flg_tst_LIN_1(l_flag_handle fff);

void l_flg_clr_MRF(void);
void l_flg_clr_SRF(void);
void   l_flg_clr_MFS_Return_switch_signal(void);
void   l_flg_clr_MFS_Menu_switch_signal(void);
void   l_flg_clr_MFS_Up_switch_signal(void);
void   l_flg_clr_MFS_Down_switch_signal(void);
void   l_flg_clr_MFS_OK_switch_signal(void);
void   l_flg_clr_MFS_Speech_switch_signal(void);
void   l_flg_clr_MFS_Mode_switch_signal(void);
void   l_flg_clr_MFS_VolPlus_switch_signal(void);
void   l_flg_clr_MFS_VolReduce_switch_signal(void);
void   l_flg_clr_MFS_Mute_switch_signal(void);
void   l_flg_clr_MFS_SeekPlus_switch_signal(void);
void   l_flg_clr_MFS_SeekReduce_switch_signal(void);
void   l_flg_clr_MFS_Crusie_switch_signal(void);
void   l_flg_clr_MFS_RESPlus_switch_signal(void);
void   l_flg_clr_MFS_SETReduce_switch_signal(void);
void   l_flg_clr_MFS_DIST_switch_signal(void);
void   l_flg_clr_MFS_ADAS_switch_signal(void);
void   l_flg_clr_MFS_DiagInfoSW(void);
void   l_flg_clr_MFS_Answer_switch_signal(void);
void   l_flg_clr_MFS_LIN_1(void);
void l_flg_clr_LIN_1(l_flag_handle fff);


l_bool l_ifc_init_LIN_1(void);
void   l_ifc_wake_up_LIN_1(void);
l_u16  l_ifc_ioctl_LIN_1(l_ioctl_op op, void* pv);
void   l_ifc_rx_LIN_1(void);
void   l_ifc_tx_LIN_1(void);
void   l_ifc_aux_LIN_1(void);
l_u16  l_ifc_read_status_LIN_1(void);

#if (1u == LIN_1_LIN_1_3)
    l_bool l_ifc_connect_LIN_1(void);
    l_bool l_ifc_disconnect_LIN_1(void);
#endif /* (1u == LIN_1_LIN_1_3) */

l_u8 ld_read_by_id_callout_LIN_1(l_u8 id, l_u8* frameData);

/* Transport Layer Functions: Initialization */
void ld_init_LIN_1(void);

/* Transport Layer Functions: Node Configuration Functions */
l_u8 ld_read_configuration_LIN_1(l_u8* pData, l_u8* const length);
l_u8 ld_set_configuration_LIN_1(const l_u8* const pData, l_u16 length);

/* Transport Layer Functions: Cooked Transport Layer API */
void ld_send_message_LIN_1(l_u16 length, l_u8 nad, const l_u8* const ld_data);
void ld_receive_message_LIN_1(l_u16* const length, l_u8* const nad, l_u8* const ld_data);
l_u8 ld_tx_status_LIN_1(void);
l_u8 ld_rx_status_LIN_1(void);

/* Transport Layer Functions: Raw Transport Layer API */
void ld_put_raw_LIN_1(const l_u8* const ld_data);
void ld_get_raw_LIN_1(l_u8* const ld_data);
l_u8 ld_raw_tx_status_LIN_1(void);
l_u8 ld_raw_rx_status_LIN_1(void);

/* ISR Prototypes */
CY_ISR_PROTO(LIN_1_UART_ISR);

/* Swap U16 macro */
#define LIN_1_SWAP_U8_TO_U16(x,y)    (l_u16) ((l_u16) (((l_u16) (x)) << 8u) | (l_u16) (y))


/******************************************************************************
*   API Constants
******************************************************************************/
/* Constants for l_u8_wr_MFS_Return_switch_signal() */
#define LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x00u)
#define LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x03u)

/* Constants for l_u8_wr_MFS_Menu_switch_signal() */
#define LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x00u)
#define LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_Up_switch_signal() */
#define LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x00u)
#define LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x04u)
#define LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x30u)

/* Constants for l_u8_wr_MFS_Down_switch_signal() */
#define LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x00u)
#define LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x06u)
#define LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_MASK_0         (0xC0u)

/* Constants for l_u8_wr_MFS_OK_switch_signal() */
#define LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x01u)
#define LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x03u)

/* Constants for l_u8_wr_MFS_Speech_switch_signal() */
#define LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x01u)
#define LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_Mode_switch_signal() */
#define LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x01u)
#define LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x04u)
#define LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x30u)

/* Constants for l_u8_wr_MFS_VolPlus_switch_signal() */
#define LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x02u)
#define LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_VolReduce_switch_signal() */
#define LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x02u)
#define LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x04u)
#define LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x30u)

/* Constants for l_u8_wr_MFS_Mute_switch_signal() */
#define LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x02u)
#define LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x06u)
#define LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_MASK_0         (0xC0u)

/* Constants for l_u8_wr_MFS_SeekPlus_switch_signal() */
#define LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x03u)
#define LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x03u)

/* Constants for l_u8_wr_MFS_SeekReduce_switch_signal() */
#define LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x03u)
#define LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_Crusie_switch_signal() */
#define LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x03u)
#define LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x04u)
#define LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x30u)

/* Constants for l_u8_wr_MFS_RESPlus_switch_signal() */
#define LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x04u)
#define LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x03u)

/* Constants for l_u8_wr_MFS_SETReduce_switch_signal() */
#define LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x04u)
#define LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_DIST_switch_signal() */
#define LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x04u)
#define LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x04u)
#define LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x30u)

/* Constants for l_u8_wr_MFS_ADAS_switch_signal() */
#define LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x06u)
#define LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x02u)
#define LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x0Cu)

/* Constants for l_u8_wr_MFS_DiagInfoSW() */
#define LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_BYTE_OFFSET    (0x07u)
#define LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_MASK_0         (0x07u)

/* Constants for l_u8_wr_MFS_Answer_switch_signal() */
#define LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET    (0x07u)
#define LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET      (0x03u)
#define LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_MASK_0         (0x18u)

/* Constants for l_bool_wr_MFS_RsErr() */
#define LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET    (0x07u)
#define LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BIT_OFFSET     (0x07u)
#define LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0         (0x80u)


/* l_flg_tst_MRF() and l_flg_clr_MRF() */
#define LIN_1_MRF_FRAME_FLAG_BYTE_OFFSET_0           (0x00u)
#define LIN_1_MRF_FRAME_FLAG_MASK_0                  (0x01u)

/* l_flg_tst_SRF() and l_flg_clr_SRF() */
#define LIN_1_SRF_FRAME_FLAG_BYTE_OFFSET_0           (0x00u)
#define LIN_1_SRF_FRAME_FLAG_MASK_0                  (0x02u)


/* l_flg_tst_MFS_Return_switch_signal() and l_flg_clr_MFS_Return_switch_signal() */
#define LIN_1_MFS_Return_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_Return_switch_signal_FRAME_FLAG_MASK_0           (0x04u)

/* l_flg_tst_MFS_Menu_switch_signal() and l_flg_clr_MFS_Menu_switch_signal() */
#define LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_MASK_0           (0x08u)

/* l_flg_tst_MFS_Up_switch_signal() and l_flg_clr_MFS_Up_switch_signal() */
#define LIN_1_MFS_Up_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_Up_switch_signal_FRAME_FLAG_MASK_0           (0x10u)

/* l_flg_tst_MFS_Down_switch_signal() and l_flg_clr_MFS_Down_switch_signal() */
#define LIN_1_MFS_Down_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_Down_switch_signal_FRAME_FLAG_MASK_0           (0x20u)

/* l_flg_tst_MFS_OK_switch_signal() and l_flg_clr_MFS_OK_switch_signal() */
#define LIN_1_MFS_OK_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_OK_switch_signal_FRAME_FLAG_MASK_0           (0x40u)

/* l_flg_tst_MFS_Speech_switch_signal() and l_flg_clr_MFS_Speech_switch_signal() */
#define LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_MASK_0           (0x80u)

/* l_flg_tst_MFS_Mode_switch_signal() and l_flg_clr_MFS_Mode_switch_signal() */
#define LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_MASK_0           (0x01u)

/* l_flg_tst_MFS_VolPlus_switch_signal() and l_flg_clr_MFS_VolPlus_switch_signal() */
#define LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_MASK_0           (0x02u)

/* l_flg_tst_MFS_VolReduce_switch_signal() and l_flg_clr_MFS_VolReduce_switch_signal() */
#define LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_MASK_0           (0x04u)

/* l_flg_tst_MFS_Mute_switch_signal() and l_flg_clr_MFS_Mute_switch_signal() */
#define LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_MASK_0           (0x08u)

/* l_flg_tst_MFS_SeekPlus_switch_signal() and l_flg_clr_MFS_SeekPlus_switch_signal() */
#define LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_MASK_0           (0x10u)

/* l_flg_tst_MFS_SeekReduce_switch_signal() and l_flg_clr_MFS_SeekReduce_switch_signal() */
#define LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_MASK_0           (0x20u)

/* l_flg_tst_MFS_Crusie_switch_signal() and l_flg_clr_MFS_Crusie_switch_signal() */
#define LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_MASK_0           (0x40u)

/* l_flg_tst_MFS_RESPlus_switch_signal() and l_flg_clr_MFS_RESPlus_switch_signal() */
#define LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x01u)
#define LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_MASK_0           (0x80u)

/* l_flg_tst_MFS_SETReduce_switch_signal() and l_flg_clr_MFS_SETReduce_switch_signal() */
#define LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_MASK_0           (0x01u)

/* l_flg_tst_MFS_DIST_switch_signal() and l_flg_clr_MFS_DIST_switch_signal() */
#define LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_MASK_0           (0x02u)

/* l_flg_tst_MFS_ADAS_switch_signal() and l_flg_clr_MFS_ADAS_switch_signal() */
#define LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_MASK_0           (0x04u)

/* l_flg_tst_MFS_DiagInfoSW() and l_flg_clr_MFS_DiagInfoSW() */
#define LIN_1_MFS_DiagInfoSW_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_DiagInfoSW_FRAME_FLAG_MASK_0           (0x08u)

/* l_flg_tst_MFS_Answer_switch_signal() and l_flg_clr_MFS_Answer_switch_signal() */
#define LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_MASK_0           (0x10u)

/* l_flg_tst_MFS_LIN_1() and l_flg_clr_MFS_LIN_1() */
#define LIN_1_MFS_LIN_1_FRAME_FLAG_BYTE_OFFSET_0    (0x02u)
#define LIN_1_MFS_LIN_1_FRAME_FLAG_MASK_0           (0x20u)



#define MFS_Return_switch_signal_SIGNAL_HANDLE        (0x00u)
#define MFS_Menu_switch_signal_SIGNAL_HANDLE        (0x01u)
#define MFS_Up_switch_signal_SIGNAL_HANDLE        (0x02u)
#define MFS_Down_switch_signal_SIGNAL_HANDLE        (0x03u)
#define MFS_OK_switch_signal_SIGNAL_HANDLE        (0x04u)
#define MFS_Speech_switch_signal_SIGNAL_HANDLE        (0x05u)
#define MFS_Mode_switch_signal_SIGNAL_HANDLE        (0x06u)
#define MFS_VolPlus_switch_signal_SIGNAL_HANDLE        (0x07u)
#define MFS_VolReduce_switch_signal_SIGNAL_HANDLE        (0x08u)
#define MFS_Mute_switch_signal_SIGNAL_HANDLE        (0x09u)
#define MFS_SeekPlus_switch_signal_SIGNAL_HANDLE        (0x0Au)
#define MFS_SeekReduce_switch_signal_SIGNAL_HANDLE        (0x0Bu)
#define MFS_Crusie_switch_signal_SIGNAL_HANDLE        (0x0Cu)
#define MFS_RESPlus_switch_signal_SIGNAL_HANDLE        (0x0Du)
#define MFS_SETReduce_switch_signal_SIGNAL_HANDLE        (0x0Eu)
#define MFS_DIST_switch_signal_SIGNAL_HANDLE        (0x0Fu)
#define MFS_ADAS_switch_signal_SIGNAL_HANDLE        (0x10u)
#define MFS_DiagInfoSW_SIGNAL_HANDLE        (0x11u)
#define MFS_Answer_switch_signal_SIGNAL_HANDLE        (0x12u)
#define MFS_RsErr_SIGNAL_HANDLE        (0x13u)
#define MRF_SIGNAL_HANDLE        (0x14u)
#define SRF_SIGNAL_HANDLE        (0x15u)


#define MRF_FLAG_HANDLE        (0x00u)
#define SRF_FLAG_HANDLE        (0x01u)
#define MFS_Return_switch_signal_FLAG_HANDLE        (0x02u)
#define MFS_Menu_switch_signal_FLAG_HANDLE        (0x03u)
#define MFS_Up_switch_signal_FLAG_HANDLE        (0x04u)
#define MFS_Down_switch_signal_FLAG_HANDLE        (0x05u)
#define MFS_OK_switch_signal_FLAG_HANDLE        (0x06u)
#define MFS_Speech_switch_signal_FLAG_HANDLE        (0x07u)
#define MFS_Mode_switch_signal_FLAG_HANDLE        (0x08u)
#define MFS_VolPlus_switch_signal_FLAG_HANDLE        (0x09u)
#define MFS_VolReduce_switch_signal_FLAG_HANDLE        (0x0Au)
#define MFS_Mute_switch_signal_FLAG_HANDLE        (0x0Bu)
#define MFS_SeekPlus_switch_signal_FLAG_HANDLE        (0x0Cu)
#define MFS_SeekReduce_switch_signal_FLAG_HANDLE        (0x0Du)
#define MFS_Crusie_switch_signal_FLAG_HANDLE        (0x0Eu)
#define MFS_RESPlus_switch_signal_FLAG_HANDLE        (0x0Fu)
#define MFS_SETReduce_switch_signal_FLAG_HANDLE        (0x10u)
#define MFS_DIST_switch_signal_FLAG_HANDLE        (0x11u)
#define MFS_ADAS_switch_signal_FLAG_HANDLE        (0x12u)
#define MFS_DiagInfoSW_FLAG_HANDLE        (0x13u)
#define MFS_Answer_switch_signal_FLAG_HANDLE        (0x14u)
#define MFS_LIN_1_FLAG_HANDLE        (0x15u)


#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_0        (0x00u)
#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_0        (0xFCu)
#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_1        (0x01u)
#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_1        (0xFFu)
#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_2        (0x02u)
#define LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_2        (0x3Fu)




/* 8 bits with 16x oversampling rate */
#define LIN_1_EXPECTED_TIME_COUNTS       (128u)
#if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Enabled */
    #define LIN_1_TIME_COUNTS_MAX_DEVIATION  (0x13u)
#else                                                /* Auto Baud Rate Sync Disabled */
    #define LIN_1_TIME_COUNTS_MAX_DEVIATION  (0x03u)
#endif    
#define LIN_1_BR_BASE_SHIFT              (7u)

/* Bus inactivity block reconfiguration */
#if (1u == LIN_1_INACTIVITY_ENABLED)
    /* Bus inactivity block configuration to issue interrupt */
    #define LIN_1_INACT_100MS_IN_S       (10u)
    #define LIN_1_INACT_OVERSAMPLE_RATE  (16u)
    #define LIN_1_INACT_DIVIDE_FACTOR    (256u)

    #define LIN_1_INACT_TIME_FACTOR      ((l_u16) (LIN_1_INACT_OVERSAMPLE_RATE * \
                                (LIN_1_BAUD_RATE / LIN_1_INACT_100MS_IN_S)))

    /* Divider 1 for specified interrupt rate */
    #define LIN_1_INACT_DIV1             (HI8(LIN_1_INACT_TIME_FACTOR))

    /* Divider 0 for specified interrupt rate */
    #define LIN_1_INACT_DIV0             (LO8(LIN_1_INACT_TIME_FACTOR))
#endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

#define LIN_1_FALSE                      (0x00u)
#define LIN_1_TRUE                       (0x01u)
#define LIN_1_ALL_IRQ_ENABLE             (0x03u)
#define LIN_1_FRAME_PID_MRF              (0x3Cu)
#define LIN_1_FRAME_PID_SRF              (0x7Du)
#define LIN_1_FRAME_PID_BROADCAST_MIN    (0x38u)
#define LIN_1_FRAME_PID_BROADCAST_MAX    (0x3Bu)
#define LIN_1_FRAME_PID_UNASSIGNED       (0x40u)
#define LIN_1_MESSAGE_ID_UNASSIGNED    (0xFFFFu)
#define LIN_1_FRAME_SYNC_BYTE            (0x55u)
#define LIN_1_ID_16                      (16u)
#define LIN_1_ID_31                      (31u)

#if LIN_1_SAE_J2602_ENABLED
    #define LIN_1_FRAME_PID_MRF_J2602    (0xFEu)
#endif /* LIN_1_SAE_J2602_ENABLED */

/* Numbers and priorities of bLIN and UART interrupts */
#define LIN_1_BLIN_ISR_NUMBER            (LIN_1_BLIN_ISR__INTC_NUMBER)
#define LIN_1_BLIN_ISR_PRIORITY          (LIN_1_BLIN_ISR__INTC_PRIOR_NUM)
#define LIN_1_UART_ISR_NUMBER            (LIN_1_UART_ISR__INTC_NUMBER)
#define LIN_1_UART_ISR_PRIORITY          (LIN_1_UART_ISR__INTC_PRIOR_NUM)

/* Wakeup signal length in us */
#define LIN_1_WAKE_UP_SIGNAL_LENGTH      (300u)
/* Wakeup fake symbol length in ms */
#define LIN_1_WAKEUP_TX_DELAY            (11u)

#define LIN_1_INVALID_FRAME_PID          (0xFFu)

/* Used to clear UART Rx FIFO */
#define LIN_1_UART_RX_FIFO_CLEAR         (0x01u)


/******************************************************************************
*                       UART State Machine                                    *
******************************************************************************/

/* Sync Field detection(UDB) / IDLE state (SCB) */
#define LIN_1_UART_ISR_STATE_0_IDLE      (0x00u)

/* Receive PID was detected. Analyze recieved PID and determine action */
#define LIN_1_UART_ISR_STATE_1_PID       (0x01u)

/* Transmit data and checksum byte to the LIN master */
#define LIN_1_UART_ISR_STATE_2_TX        (0x02u)

/* Read data from the LIN master */
#define LIN_1_UART_ISR_STATE_3_RX        (0x03u)

/* Checksum verification */
#define LIN_1_UART_ISR_STATE_4_CHS       (0x04u)

/* PID Parity Error Mask */
#define LIN_1_PID_PARITY_MASK            (0x3Fu)


/******************************************************************************
*                     LIN_1_PID_INFO_TABLE                         *
******************************************************************************/
#define LIN_1_FRAME_DATA_SIZE_1          (0x01u)
#define LIN_1_FRAME_DATA_SIZE_2          (0x02u)
#define LIN_1_FRAME_DATA_SIZE_3          (0x03u)
#define LIN_1_FRAME_DATA_SIZE_4          (0x04u)
#define LIN_1_FRAME_DATA_SIZE_5          (0x05u)
#define LIN_1_FRAME_DATA_SIZE_6          (0x06u)
#define LIN_1_FRAME_DATA_SIZE_7          (0x07u)
#define LIN_1_FRAME_DATA_SIZE_8          (0x08u)

/* LIN slave direction = subscribe */
#define LIN_1_FRAME_DIR_SUBSCRIBE        (0x00u)

/* LIN slave direction = publish */
#define LIN_1_FRAME_DIR_PUBLISH          (0x10u)

/* LIN frame type = event-triggered */
#define LIN_1_FRAME_TYPE_EVENT           (0x20u)

/* LIN frame type = unconditional */
#define LIN_1_FRAME_TYPE_UNCOND          (0x00u)

#define LIN_1_FRAME_DATA_SIZE_MASK       (0x0Fu)
#define LIN_1_FRAME_DIR_MASK             (0xEFu)
#define LIN_1_FRAME_TYPE_MASK            (0xDEu)


/******************************************************************************
*                                Interface Status
*                          LIN_1_ifcStatus
******************************************************************************/
/* Error in response */
#define LIN_1_IFC_STS_ERROR_IN_RESPONSE  (0x0001u)

/* Successful frame transfer */
#define LIN_1_IFC_STS_SUCCESSFUL_TRANSFER (0x0002u)

/* Overrun */
#define LIN_1_IFC_STS_OVERRUN            (0x0004u)

/* Go to sleep */
#define LIN_1_IFC_STS_GO_TO_SLEEP        (0x0008u)

/* Bus activity */
#define LIN_1_IFC_STS_BUS_ACTIVITY       (0x0010u)

/* Event-triggered frame collision */
#define LIN_1_IFC_STS_EVTRIG_COLLISION   (0x0020u)

/* Save configuration */
#define LIN_1_IFC_STS_SAVE_CONFIG        (0x0040u)

/* Last frame PID mask */
#define LIN_1_IFC_STS_PID_MASK           (0xFF00u)

/* Status mask */
#define LIN_1_IFC_STS_MASK               (0xFFFFu)


/******************************************************************************
*                          Internal LIN Slave Status
*                           LIN_1_status
******************************************************************************/

/* This bit indicates that there is a response for ACRH service
*  and it is ready to be sent to Master node.
*/
#define LIN_1_STATUS_SRVC_RSP_RDY        (0x80u)
#define LIN_1_STATUS_ERROR_MIRROR        (0x01u)
#define LIN_1_STATUS_NOT_CHECKED         (0x04u)
#define LIN_1_STATUS_RESPONSE_ACTIVE     (0x10u)
/* Indicates that a response pending frame (NRC 0x78 code) was issued. */
#define LIN_1_STATUS_RESPONSE_PENDING    (0x20u)


#if LIN_1_SAE_J2602_ENABLED
    /***************************************************************************
    *  SAE J2602-1 Status codes [ERR2:ERR0] for
    * LIN_1_j2602Status[7:5]
    * used with: 
    *     LIN_1_j2602Status_Set()
    *     LIN_1_j2602Status_Get()
    *     LIN_1_j2602Status_Clear()
    ***************************************************************************/

    /* Data error */
    #define LIN_1_J2602_STS_RESET            (1u)
    
    /* Data error */
    #define LIN_1_J2602_STS_DATA_ERR         (4u)

    /* Checksum error  */
    #define LIN_1_J2602_STS_CHECKSUM_ERR     (5u)

    /* Framing error  */
    #define LIN_1_J2602_STS_FRAMING_ERR      (6u)

    /* Parity error  */
    #define LIN_1_J2602_STS_PARITY_ERR       (7u)
    
    /* APINFO mask  */
    #define LIN_1_J2602_STS_APINFO_MASK   (0x1Fu)    
    
    /***************************************************************************
    * Internal AUX ISR shadow status  
    *  used for 
    * LIN_1_auxStatus; 
    ***************************************************************************/
    /* Previous MRF requires J2602_Status in SRF (J2602-1 5.7.2.4)*/
    #define LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS  (1u)
    /* Previous MRF ID==0x3E (PID==FE) */
    #define LIN_1_AUXSTS_MRF_WAS_3E                (2u)
    
    
#endif /* LIN_1_SAE_J2602_ENABLED */


/******************************************************************************
*                       LIN_1_EndFrame(l_u16 status)
******************************************************************************/
#define LIN_1_HANDLING_RESET_FSM_ERR     (0x01u)
#define LIN_1_HANDLING_DONT_SAVE_PID     (0x10u)
#define LIN_1_HANDLING_SKIP_OVERRUN      (0x20u)
#define LIN_1_HANDLING_PID_ERR           (0x40u)


/******************************************************************************
*                           LIN_1_fsmFlags
******************************************************************************/

/* Break signal detected  */
#define LIN_1_FSM_BREAK_FLAG             (0x01u)

/* Sleep API was called */
#define LIN_1_FSM_SLEEP_API_FLAG         (0x02u)

/* UART receives at least 1 byte of data */
#define LIN_1_FSM_DATA_RECEIVE           (0x04u)

/* UART enable flag */
#define LIN_1_FSM_UART_ENABLE_FLAG       (0x08u)

/* Overrun flag is used for status word */
#define LIN_1_FSM_OVERRUN                (0x40u)

/* Framing error */
#define LIN_1_FSM_FRAMING_ERROR_FLAG     (0x80u)


/******************************************************************************
*                           Transport Layer
******************************************************************************/
#if LIN_1_CFG_SERVICES_ENABLED
    /* Wildcard ID understandable for every slave node */
    #define LIN_1_CS_SUPPLIER_ID_WILDCARD    (0x7FFFu)
    #define LIN_1_CS_FUNCTION_ID_WILDCARD    (0xFFFFu)

    #define LIN_1_CS_BYTE_SUPPLIER_ID1       (0x01u)
    #define LIN_1_CS_BYTE_SUPPLIER_ID2       (0x02u)
    #define LIN_1_CS_BYTE_FUNCTION_ID1       (0x03u)
    #define LIN_1_CS_BYTE_FUNCTION_ID2       (0x04u)
    #define LIN_1_CS_BYTE_VARIANT            (0x05u)
#endif /* LIN_1_CFG_SERVICES_ENABLED */

/* Specifies the value for the TL timeouts  */
#define LIN_1_TL_N_AS_TIMEOUT_VALUE      (1000u)
#define LIN_1_TL_N_CR_TIMEOUT_VALUE      (1000u)
#define LIN_1_TL_P2_TIMEOUT_VALUE        (500u)

/* Specifies the Frame buffer length for Transport Layer */
#define LIN_1_FRAME_BUFF_LEN             (8u)

/* Specifies the Frame length for Transport Layer */
#define LIN_1_FRAME_LEN                  (8u)

/* Packet Data Unit (PDU) Offsets */
#define LIN_1_PDU_NAD_IDX                (0u)
#define LIN_1_PDU_PCI_IDX                (1u)
#define LIN_1_PDU_SID_IDX                (2u)
#define LIN_1_PDU_LEN_IDX                (2u)
#define LIN_1_PDU_D1_IDX                 (3u)
#define LIN_1_PDU_D1_START_IDX           (3u)
#define LIN_1_PDU_D1_ID_IDX              (3u)
#define LIN_1_PDU_D2_IDX                 (4u)
#define LIN_1_PDU_D2_PID_IDX             (4u)
#define LIN_1_PDU_D2_BYTE_IDX            (4u)
#define LIN_1_PDU_D3_IDX                 (5u)
#define LIN_1_PDU_D3_MASK_IDX            (5u)
#define LIN_1_PDU_D4_IDX                 (6u)
#define LIN_1_PDU_D4_INVERT_IDX          (6u)
#define LIN_1_PDU_D5_IDX                 (7u)
#define LIN_1_PDU_D5_NEW_NAD_IDX         (7u)

/* Single Frame data length  */
#define LIN_1_PDU_SF_DATA_LEN            (6u)

/* Protocol Control Information (PCI) Types */
#define LIN_1_PDU_PCI_TYPE_SF            (0x00u)     /* Single Frame */
#define LIN_1_PDU_PCI_TYPE_FF            (0x10u)     /* First Frame */
#define LIN_1_PDU_PCI_TYPE_CF            (0x20u)     /* Consecutive Frame */
#define LIN_1_PDU_PCI_TYPE_UNKNOWN       (0xFFu)
#define LIN_1_PDU_PCI_TYPE_MASK          (0xF0u)
#define LIN_1_PDU_PCI_LENGTH_MASK        (0x0Fu)

#define LIN_1_NAD_J2602_BASE             (0x60u)
#define LIN_1_NAD_MULTIPLE2_MASK         (0x01u)
#define LIN_1_NAD_MULTIPLE4_MASK         (0x03u)
#define LIN_1_NAD_MULTIPLE8_MASK         (0x07u)
#define LIN_1_NAD_DNN_MASK               (0x0Fu)
#define LIN_1_NAD_UNINITIALIZED          (0x6Fu)
#define LIN_1_NAD_FUNCTIONAL             (0x7Eu)

/* Wildcard NAD */
#define LIN_1_NAD_BROADCAST              (0x7Fu)

/* Go to sleep command ID */
#define LIN_1_NAD_GO_TO_SLEEP            (0x00u)

/* Max and min Config Services IDs */
#define LIN_1_SID_CONF_MIN               (0xB0u)
#define LIN_1_SID_CONF_MAX               (0xB7u)

/* Node Configuration Services */
#define LIN_1_NCS_ASSIGN_NAD             (0xB0u)
#define LIN_1_NCS_ASSIGN_FRAME_ID        (0xB1u) /* Used only in LIN 2.0 */
#define LIN_1_NCS_READ_BY_ID             (0xB2u)
#define LIN_1_NCS_COND_CHANGE_NAD        (0xB3u)
#define LIN_1_NCS_DATA_DUMP              (0xB4u) /* Not supported */
#define LIN_1_NCS_ASSIGN_NAD_SNPD        (0xB5u) /* Not supported */
#define LIN_1_NCS_SAVE_CONFIG            (0xB6u)
#define LIN_1_NCS_ASSIGN_FRAME_ID_RANGE  (0xB7u)

/* Used with tempStatusErr to Return Negative response for Config Service */
#define LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED      (0x01u)

/* Positive responses for Node Configuration Services requests */
#define LIN_1_NCS_POS_RESP_ASSIGN_NAD             (0xF0u)
#define LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID        (0xF1u) /* Used only in LIN 2.0 */
#define LIN_1_NCS_POS_RESP_READ_BY_ID             (0xF2u)
#define LIN_1_NCS_POS_RESP_COND_CHANGE_NAD        (0xF3u)
#define LIN_1_NCS_POS_RESP_DATA_DUMP              (0xF4u) /* Not supported */
#define LIN_1_NCS_POS_RESP_ASSIGN_NAD_SNPD        (0xF5u) /* Not supported */
#define LIN_1_NCS_POS_RESP_SAVE_CONFIG            (0xF6u)
#define LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID_RANGE  (0xF7u)

/* Other LIN TL constants */
#define LIN_1_NCS_READ_BY_ID_ID          (0x00u)
#define LIN_1_NCS_READ_BY_ID_SERIAL      (0x01u)
#define LIN_1_NCS_READ_BY_ID_FILE_REVISION (0x03u)
#define LIN_1_NCS_RSID_NEG_REPLY         (0x7Fu)
#define LIN_1_NCS_MAX_FRAME_ID_RANGE     (0x04u)

/* ld_read_by_id_callout() return constants */
#define LD_NEGATIVE_RESPONSE                        (0x00u)
#define LD_NO_RESPONSE                              (0x01u)
#define LD_POSITIVE_RESPONSE                        (0x02u)

#define LD_INVALID_MESSAGE_INDEX                    (0xFFu)
#define LD_MESSAGE_ID_BASE                          (0x10u)


/******************************************************************************
*       LIN_1_txTlStatus and LIN_1_rxTlStatus
******************************************************************************/

/* Reception or transmission is not yet completed */
#define LD_IN_PROGRESS                              (0x01u)

/* Reception or transmission completed successfully */
#define LD_COMPLETED                                (0x02u)

/* Reception or transmission ended in an error */
#define LD_FAILED                                   (0x03u)

/* Transmission failed because of a N_As timeout */
#define LD_N_AS_TIMEOUT                             (0x04u)

/* Reception failed because of a N_Cr timeout */
#define LD_N_CR_TIMEOUT                             (0x05u)

/* Reception failed because of an unexpected sequence number */
#define LD_WRONG_SN                                 (0x06u)

/* The transmit queue is empty */
#define LD_QUEUE_EMPTY                              (0x07u)

/* Transmit queue contains entires */
#define LD_QUEUE_AVAILABLE                          (0x08u)

/* Transmit queue is full and cannott accept further frames. */
#define LD_QUEUE_FULL                               (0x09u)

/* LIN protocol errors occurred during transfer */
#define LD_TRANSMIT_ERROR                           (0x0Au)

/* Receive queue is empty */
#define LD_NO_DATA                                  (0x0Bu)

/* Receive queue contains data that can be read */
#define LD_DATA_AVAILABLE                           (0x0Cu)

/* LIN protocol errors occurred during transfer */
#define LD_RECEIVE_ERROR                            (0x0Du)


/******************************************************************************
*                           LIN_1_tlFlags
******************************************************************************/

/* The requested service is disabled and diagnostic frame will
*  be "passed" to Transport Layer.
*/
#define LIN_1_TL_CS_SERVICE_DISABLED     (0x01u)

/* SID isn't ACRH but a simple diagnostic SID */
#define LIN_1_TL_DIAG_FRAME_DETECTED     (0x02u)

/* The last PID that occurred is SRF PID */
#define LIN_1_TL_TX_DIRECTION            (0x04u)

/* The last PID that occurred is MRF PID */
#define LIN_1_TL_RX_DIRECTION            (0x08u)

/* Indicates that Cooked API requested transmit data */
#define LIN_1_TL_TX_REQUESTED            (0x10u)

/* Indicates that Cooked API requested receive data */
#define LIN_1_TL_RX_REQUESTED            (0x20u)

/* Indicates that the N_AS timeout monitoring is in progress */
#define LIN_1_TL_N_AS_TIMEOUT_ON         (0x40u)

/* Indicates that the N_CR timeout monitoring is in progress */
#define LIN_1_TL_N_CR_TIMEOUT_ON         (0x80u)


/******************************************************************************
*            ld_read_configuration() and ld_set_configuration()
******************************************************************************/

/* Read configuration is complete */
#define LIN_1_LD_READ_OK                 (0x01u)

/* Read configuration failed. The configuration size is greater than the length. */
#define LIN_1_LD_LENGTH_TOO_SHORT        (0x02u)

/* Set configuration completed. */
#define LIN_1_LD_SET_OK                  (0x01u)

/* Set configuration failed.
*  The required configuration size is not equal to the given length.
*/
#define LIN_1_LD_LENGTH_NOT_CORRECT      (0x02u)

/* Set configuration is fail. Set conf could not be made. */
#define LIN_1_LD_DATA_ERROR              (0x04u)


/******************************************************************************
*            CheckNodeState() Parameters
******************************************************************************/

#define LIN_1_MRF_ALIEN_NAD              (0x01u)
#define LIN_1_MRF_OWN_NAD                (0x02u)
#define LIN_1_MRF_FUNC_NAD               (0x04u)
#define LIN_1_RX_TIMEOUT                 (0x08u)
#define LIN_1_TX_TIMEOUT                 (0x10u)
#define LIN_1_RX_ERROR                   (0x20u)
#define LIN_1_TX_ERROR                   (0x40u)
#define LIN_1_SRF                        (0x80u)


/******************************************************************************
*            CheckNodeState() Return Values
******************************************************************************/

#define LIN_1_NO_RESPONSE_REQUIRED       (0x00u)
#define LIN_1_RESPONSE_REQUIRED          (0x01u)
#define LIN_1_RECEIVE_CONTINUES          (0x02u)
#define LIN_1_TRANSMISSION_CONTINUES     (0x04u)
#define LIN_1_IGNORE_TL_FRAME            (0x08u)
#define LIN_1_ERRONEOUS_TL_FRAME         (0x10u)


/*******************************************************************************
*                         LIN_1_nodeState
*           Slave Node states as defined in part 5.5 of LIN2.2a Spec.
*******************************************************************************/

#define LIN_1_IDLE                       (0x00u)
#define LIN_1_RX_PHY_REQUEST             (0x01u)
#define LIN_1_TX_PHY_RESPONSE            (0x02u)


/******************************************************************************
*           l_ifc_ioctl() Parameters
******************************************************************************/
#define L_IOCTL_READ_STATUS                         (0x00u)
#define L_IOCTL_SET_BAUD_RATE                       (0x01u)
#define L_IOCTL_SLEEP                               (0x02u)
#define L_IOCTL_WAKEUP                              (0x03u)
#define L_IOCTL_SYNC_COUNTS                         (0x04u)
#define L_IOCTL_SET_SERIAL_NUMBER                   (0x05u)
#define L_IOCTL_GET_NAD                             (0x06u)
#define L_IOCTL_SET_NAD                             (0x07u)
#define L_IOCTL_GET_FRAME_PID                       (0x08u)
#define L_IOCTL_SET_FRAME_PID                       (0x09u)
#define L_IOCTL_SET_DNN                             (0x0Au)
#define L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID         (0x0Bu)


/******************************************************************************
*           Variable LIN_1_ioctlStatus flags
******************************************************************************/
/* Bus inactivity */
#define LIN_1_IOCTL_STS_BUS_INACTIVITY   (0x0001u)

/* Target reset (0xB5) */
#define LIN_1_IOCTL_STS_TARGET_RESET     (0x0002u)

/* Status Register */
#define LIN_1_STATUS_BREAK_DETECTED      (0x01u)
#define LIN_1_STATUS_EDGE_DETECTED       (0x02u)
#define LIN_1_STATUS_SYNC_COMPLETED      (0x08u)
#if (1u == LIN_1_INACTIVITY_ENABLED)
    #define LIN_1_STATUS_INACTIVITY_INT  (0x04u)
#endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */

/* Control Register */
#define LIN_1_CONTROL_ENABLE             (0x01u)
#define LIN_1_CONTROL_TX_DIS             (0x02u)
#define LIN_1_CONTROL_RX_DIS             (0x04u)

/* Status Mask Register */
#define LIN_1_INT_MASK_BREAK             (0x01u)
#define LIN_1_INT_MASK_EDGE              (0x02u)
#define LIN_1_INT_MASK_SYNC              (0x08u)

#if (1u == LIN_1_INACTIVITY_ENABLED)
    #define LIN_1_INT_MASK_INACTIVITY    (0x04u)
#else
    #define LIN_1_INT_MASK_INACTIVITY    (0x00u)
#endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */

#define LIN_1_STATUS_AUX_CONTROL_INT_EN  (0x10u)

#endif /* CY_LIN_LIN_1_H */

/* [] END OF FILE */
