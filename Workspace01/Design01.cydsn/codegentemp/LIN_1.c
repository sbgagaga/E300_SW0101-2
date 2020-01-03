/******************************************************************************
* File Name: LIN_1.c
* Version 5.0
*
* Description:
*  This file contains the implementation of the LIN Slave component.
*
*******************************************************************************
* Copyright 2011-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LIN_1.h"
#include "LIN_Dynamic.h"
#include "cyapicallbacks.h"

/* Internal APIs */
static l_u8 LIN_1_FindPidIndex(l_u8 pid);
static void LIN_1_EndFrame(l_u8 status);
static void LIN_1_SetAssociatedFlags(l_u8 pIndex);

#if (LIN_1_SAE_J2602_ENABLED)
    /* SAE J2602 is enabled */
    static void LIN_1_j2602Status_Set(l_u8 STS_code);
    static l_u8 LIN_1_j2602Status_Get(void);
    static void LIN_1_j2602Status_Clear(void);
    static l_u8 LIN_1_UpdateMessageIDs(l_u8 newNAD);
#else 
    /* SAE J2602 is disabled */
    static l_bool LIN_1_GetEtFlagValue(l_u8 pidIndex);
    static void   LIN_1_ClearEtFlagValue(l_u8 pidIndex);
#endif /* LIN_1_SAE_J2602_ENABLED */

#if (LIN_1_TL_USED)
    static l_u8 LIN_1_TransmitTLFrame(l_u8 stimulus);
    static void LIN_1_ClrRxBuffer(l_u8 stimulus);
    static void LIN_1_ClrTxBuffer(l_u8 stimulus);
    static void LIN_1_CheckNodeState(l_u8 stimulus);
    static l_u8 LIN_1_ProcessMrf(l_u8 frame[]);

    #if (LIN_1_CFG_SERVICES_ENABLED)
        #if ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) || \
             (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))
            static l_bool LIN_1_LinProductId(volatile const l_u8 frameData[]);
        #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) ||
                  (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) */

        #if ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL)) && \
             (1u == LIN_1_LIN_2_0))
            static l_u8 LIN_1_MessageId(volatile const l_u8* frameData);
        #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL)) && 
                  (1u == LIN_1_LIN_2_0) */
    #endif /* LIN_1_CFG_SERVICES_ENABLED */

    /* RAM copy of Slave NAD */
    static volatile l_u8 LIN_1_nad;
#endif /* LIN_1_TL_USED */

/******************************************************************************
*                            Parity Lookup Table
* A 6-bit identifier is given as an index. 
* The indexed value provides the correct value with the parity bit-set.
******************************************************************************/
static const l_u8 CYCODE LIN_1_parityTable[] =
{
    0x80u, 0xC1u, 0x42u, 0x03u, 0xC4u, 0x85u, 0x06u, 0x47u, 0x08u, 0x49u, 0xCAu,
    0x8Bu, 0x4Cu, 0x0Du, 0x8Eu, 0xCFu, 0x50u, 0x11u, 0x92u, 0xD3u, 0x14u, 0x55u,
    0xD6u, 0x97u, 0xD8u, 0x99u, 0x1Au, 0x5Bu, 0x9Cu, 0xDDu, 0x5Eu, 0x1Fu, 0x20u,
    0x61u, 0xE2u, 0xA3u, 0x64u, 0x25u, 0xA6u, 0xE7u, 0xA8u, 0xE9u, 0x6Au, 0x2Bu,
    0xECu, 0xADu, 0x2Eu, 0x6Fu, 0xF0u, 0xB1u, 0x32u, 0x73u, 0xB4u, 0xF5u, 0x76u,
    0x37u, 0x78u, 0x39u, 0xBAu, 0xFBu, 0x3Cu, 0x7Du, 0xFEu, 0xBFu
};
#if (1u == LIN_1_INACTIVITY_ENABLED)
    /* Free-running timer */
    static uint32 LIN_1_periodCounter;
#endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

static volatile l_u8  LIN_1_status;                /* Internal Status                  */
static volatile l_u16 LIN_1_syncCounts;            /* Sync Field Timer Counts          */
static volatile l_u16 LIN_1_breakToSyncCounts = 0u;/* Break to Sync timeout counter    */

/* Baud detect counts till LIN_1_BREAK_TO_SYNC_TIMEOUT */
static volatile l_u16 LIN_1_breakToSyncCounts1;                                               
static volatile l_u16 LIN_1_breakToSyncCounts2;   /* Baud detect counts on timeout+1ms*/
static volatile l_u8  LIN_1_auxStatus;             /* Internal AUX ISR shadow status   */
static volatile l_u16 LIN_1_ioctlStatus;           /* Status used by l_ifc_ioctl()     */
static volatile l_u16 LIN_1_ifcStatus;             /* Interface communication status   */
static volatile l_u8  LIN_1_uartFsmState;          /* Current state of the UART ISR    */
static volatile l_u8  LIN_1_fsmFlags;

#if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
    /* Initial clock divider */
    l_u16 LIN_1_initialClockDivider; 
#endif /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

#if (LIN_1_SAE_J2602_ENABLED)
    /* J2602 status variable 
    * Stores J2602_APINFO[4:0] bits after calling the
    * l_u8_wr() or l_bool_wr() from application, which writes to APINFO. 
    * These bits are saved when updating ERR[7:5] bits from ISR *
    */
    static volatile l_u8 LIN_1_j2602Status = 0u;
    static volatile l_u8 LIN_1_j2602StatusStack = 0u;  /* Stores internal set of node ERR states */
#endif  /* LIN_1_SAE_J2602_ENABLED */

/* Notification API statuses */
static l_u8 LIN_1_statusFlagArray[LIN_1_SIG_FRAME_FLAGS_SIZE];

/* SAE J2602 is disabled */
#if ((!LIN_1_SAE_J2602_ENABLED) && (0u != LIN_1_NUM_ET_FRAMES))
    static l_u8 LIN_1_etFrameFlags[LIN_1_ET_FRAMES_FLAGS_SIZE];
#endif /* (!LIN_1_SAE_J2602_ENABLED) && (0u != LIN_1_NUM_ET_FRAMES) */

/* RAM copy of the slave configuration data */
static volatile l_u8 LIN_1_volatileConfig[LIN_1_NUM_FRAMES];


/****************************************************
*   Transport Layer API
*****************************************************/
#if (LIN_1_TL_USED)
    #if (LIN_1_CFG_SERVICES_ENABLED)
        #if (0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB0_SEL | LIN_1_NCS_0xB1_SEL \
            | LIN_1_NCS_0xB2_SEL | LIN_1_NCS_0xB3_SEL)))
            /* LIN Slave Identification */
            static const LIN_1_SLAVE_ID CYCODE LIN_1_slaveId =
            {
                LIN_1_CS_SUPPLIER_ID,
                LIN_1_CS_FUNCTION_ID,
                LIN_1_CS_VARIANT
            };
        #endif /*(0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB0_SEL
               * | LIN_1_NCS_0xB1_SEL | LIN_1_NCS_0xB2_SEL | LIN_1_NCS_0xB3_SEL))
               */
        /* Serial Number */
        static l_u8* LIN_1_serialNumber;
    #endif /* LIN_1_CFG_SERVICES_ENABLED */

    /* MRF buffer */
    static l_u8 LIN_1_mrfBuffer[LIN_1_FRAME_BUFF_LEN];

    /* SRF buffer */
    static volatile l_u8 LIN_1_srfBuffer[LIN_1_FRAME_BUFF_LEN];

    /* Transport Layer Rx and Tx Statuses */
    static volatile l_u8 LIN_1_txTlStatus;
    static volatile l_u8 LIN_1_rxTlStatus;

    /* Flags that are used for Transport Layer */
    static volatile l_u8 LIN_1_tlFlags;
    static l_u8 LIN_1_nodeState = LIN_1_IDLE;

    /* Internal variable used to store the PCI of the previously transmitted frame */
    static l_u8 LIN_1_txPrevPci;

    /* Internal variable used to store the PCI of the previously received frame */
    static l_u8 LIN_1_rxPrevPci;

    static volatile l_u16  LIN_1_txMessageLength = 0u;
    static volatile l_u16  LIN_1_rxMessageLength = 0u;
    static volatile l_u8   LIN_1_txFrameCounter  = 0u;
    static volatile l_u8   LIN_1_rxFrameCounter  = 0u;
    static volatile uint32 LIN_1_tlTimeoutCnt    = 0u;

    #if (LIN_1_COOKED_TL_API)
        static const    l_u8*  LIN_1_txTlDataPointer     = NULL;
        static volatile l_u16  LIN_1_txTlDataCount;
        static volatile l_u8*  LIN_1_rxTlDataPointer     = NULL;
        static volatile l_u8*  LIN_1_rxTlInitDataPointer = NULL;
        static volatile l_u8*  LIN_1_tlNadPointer        = NULL;
        static volatile l_u16* LIN_1_tlLengthPointer     = NULL;
    #else
        /* Internal variables for buffer indexing */
        static l_u8 LIN_1_txBufDepth;
        static l_u8 LIN_1_rxBufDepth;

        /* The Master Request Frame (MRF) buffer */
        static volatile l_u8 LIN_1_rawRxQueue[LIN_1_TL_RX_QUEUE_LEN];

        /* The Slave Response Frame (SRF) buffer */
        static volatile l_u8 LIN_1_rawTxQueue[LIN_1_TL_TX_QUEUE_LEN];

        /* TX buffer indexes */
        static volatile l_u16 LIN_1_txWrIndex;
        static volatile l_u16 LIN_1_txRdIndex;

        /* RX buffer indexes */
        static volatile l_u16 LIN_1_rxWrIndex;
        static volatile l_u16 LIN_1_rxRdIndex;
    #endif /* LIN_1_COOKED_TL_API */
#endif /* LIN_1_TL_USED */


/* LIN slave configuration data */
static LIN_1_SLAVE_CONFIG const CYCODE LIN_1_LinSlaveConfig = {
    LIN_1_TL_INITIAL_NAD,

    /* Frame PID Table */
    {0x99u}
};


/* Frames declaration with initial signals values */
static volatile l_u8  LIN_1_Buffer_MFS_LIN_1[8u] = {0x00u, 0xC0u, 0x03u, 0xC0u, 0xC0u, 0xFFu, 0xF3u, 0x60u};


#if (LIN_1_SAE_J2602_ENABLED)
    /*******************************************************************************
    * Function Name: LIN_1_j2602Status_Set
    ********************************************************************************
    *
    * Summary:
    *  Sets the J2602 error according to LIN_1_J2602_STS code.
    *
    *******************************************************************************/
    static void LIN_1_j2602Status_Set(l_u8 STS_code)
    {
        LIN_1_j2602StatusStack |= (l_u8)(1u << STS_code);
    }
    
    
    /*******************************************************************************
    * Function Name: LIN_1_j2602Status_Get
    ********************************************************************************
    *
    * Summary:
    *  Gets the J2602 error with the highest priority.
    *
    * Return:
    *  LIN_1_J2602_STS code
    *
    *******************************************************************************/
    static l_u8 LIN_1_j2602Status_Get(void)
    {
        l_u8 priority;
        /* Error flags are scanned from highest (7) to lowest (1) priority */
        for (priority = 7u; priority != 0u; priority--)
        {
            /* If set flag found, its number is returned immediately */
            if (0u != (LIN_1_j2602StatusStack & (l_u8)(1u << priority)))
            {
                break;
            }
        }
        /* Updates internal J2602_Status variable to be ready for the next reply. */
        LIN_1_j2602Status &= LIN_1_J2602_STS_APINFO_MASK;
        LIN_1_j2602Status |= (l_u8)(priority << 5u);
        
        return (priority); /* result is NOT shifted to ERR bits position */
    }
    
    
    /*******************************************************************************
    * Function Name: LIN_1_j2602Status_Clear
    ********************************************************************************
    *
    * Summary:
    *  Clears the highest priority J2602 error.
    *
    *******************************************************************************/
    static void LIN_1_j2602Status_Clear(void)
    {
        l_u8 priority;
        /* Error flags are scanned from highest (7) to lowest (1) priority */
        for (priority = 7u; priority != 0u; priority--)
        {
            /* If set flag found, it is cleared */
            if (0u != (LIN_1_j2602StatusStack & (l_u8)(1u << priority)))
            {
                LIN_1_j2602StatusStack &= (l_u8)~((l_u8)(1u << priority));
                break;
            }
        }    
    }
    
    
    /*******************************************************************************
    * Function Name: LIN_1_j2602Status_Reset
    ********************************************************************************
    *
    * Summary:
    *  Clears all pending errors and sets Reset flag.
    *  Should be called after application reset (B5 Target Reset command received).
    *
    *******************************************************************************/
    void LIN_1_j2602Status_Reset(void)
    {
        l_u8 interruptState;
        /* Save current global interrupt enable and disable it */
        interruptState = CyEnterCriticalSection();    
        
        LIN_1_j2602StatusStack = (l_u8)(1u << LIN_1_J2602_STS_RESET);
        
        /* Restore global interrupt enable state */
        CyExitCriticalSection(interruptState);        
    }
#endif  /* LIN_1_SAE_J2602_ENABLED */


/*******************************************************************************
* Function Name: LIN_1_Start
********************************************************************************
*
* Summary:
*  Starts the component operation. This function is not required to use.
*
* Return:
*  Zero     - If the initialization succeeds.
*  Non-zero - If the initialization fails.
*
*******************************************************************************/
l_bool LIN_1_Start(void)
{
    l_bool returnValue;

    /* Call the initialization function
    *  which do nothing so can be called safely from any instance of API */
    (void) l_sys_init();

    /* Start LIN components */
    returnValue = l_ifc_init_LIN_1();

    return (returnValue);
}


/******************************************************************************
* Function Name: LIN_1_Stop
*******************************************************************************
*
* Summary:
*  Stops the component operation. This function is not required to be used.
*
******************************************************************************/
void LIN_1_Stop(void)
{
    /* Stop UART */
    LIN_1_SCB_Stop();

    /* Clear bit corresponding to current instance in SysTimerEnabledFlag
     * and stop SySTick timer only if both flags are cleared */

    #if (1u == LIN_1_INACTIVITY_ENABLED)
        /* Stop SysTick timer if it was enabled before */
        LIN_1_TimerDisableInt();
    #endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Disables interrupts from this instance  */
    LIN_1_ISR_Disable();
}


/* Signal interaction and notification API */








/*******************************************************************************
* Function Name: l_bool_wr_LIN_1
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for one bit signals to "v".
*  If an invalid signal handle is passed into the function, no action is done. 
*
* Parameters:
*  sss - signal handle of the signal to be set.
*  v - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bool_wr_LIN_1(l_signal_handle sss, l_bool v)
{
    switch(sss)
    {
        case MFS_RsErr_SIGNAL_HANDLE:
            l_bool_wr_MFS_RsErr(v);
        break;

        default:
        break;
    }
}

/*******************************************************************************
* Function Name: l_bool_wr_MFS_RsErr
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for one bit signals to v.
*
* Parameters:
*  v - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bool_wr_MFS_RsErr(l_bool v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    if(0u != v)
    {
        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;
    }
    else
    {
        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}






/*******************************************************************************
* Function Name: l_u8_wr_LIN_1
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*  If an invalid signal handle is passed into the function, no action is done.
*
* Parameters:
*  sss - signal handle of the signal to write.
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_LIN_1(l_signal_handle sss, l_u8 v)
{
    switch(sss)
    {
        case MFS_Return_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Return_switch_signal(v);
        break;

        case MFS_Menu_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Menu_switch_signal(v);
        break;

        case MFS_Up_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Up_switch_signal(v);
        break;

        case MFS_Down_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Down_switch_signal(v);
        break;

        case MFS_OK_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_OK_switch_signal(v);
        break;

        case MFS_Speech_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Speech_switch_signal(v);
        break;

        case MFS_Mode_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Mode_switch_signal(v);
        break;

        case MFS_VolPlus_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_VolPlus_switch_signal(v);
        break;

        case MFS_VolReduce_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_VolReduce_switch_signal(v);
        break;

        case MFS_Mute_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Mute_switch_signal(v);
        break;

        case MFS_SeekPlus_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_SeekPlus_switch_signal(v);
        break;

        case MFS_SeekReduce_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_SeekReduce_switch_signal(v);
        break;

        case MFS_Crusie_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Crusie_switch_signal(v);
        break;

        case MFS_RESPlus_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_RESPlus_switch_signal(v);
        break;

        case MFS_SETReduce_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_SETReduce_switch_signal(v);
        break;

        case MFS_DIST_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_DIST_switch_signal(v);
        break;

        case MFS_ADAS_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_ADAS_switch_signal(v);
        break;

        case MFS_DiagInfoSW_SIGNAL_HANDLE:
            l_u8_wr_MFS_DiagInfoSW(v);
        break;

        case MFS_Answer_switch_signal_SIGNAL_HANDLE:
            l_u8_wr_MFS_Answer_switch_signal(v);
        break;

        default:
        break;
    }
}

/*******************************************************************************
* Function Name: l_u8_wr_MFS_Return_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Return_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_MASK_0;

    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LIN_1_MFS_Return_switch_signal_MFS_LIN_1_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Menu_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Menu_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Menu_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Up_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Up_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Up_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Down_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Down_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Down_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_OK_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_OK_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_MASK_0;

    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LIN_1_MFS_OK_switch_signal_MFS_LIN_1_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Speech_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Speech_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Speech_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Mode_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Mode_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Mode_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_VolPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_VolPlus_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_VolPlus_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_VolReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_VolReduce_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_VolReduce_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Mute_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Mute_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Mute_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_SeekPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_SeekPlus_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_MASK_0;

    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LIN_1_MFS_SeekPlus_switch_signal_MFS_LIN_1_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_SeekReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_SeekReduce_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_SeekReduce_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Crusie_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Crusie_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Crusie_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_RESPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_RESPlus_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_MASK_0;

    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LIN_1_MFS_RESPlus_switch_signal_MFS_LIN_1_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_SETReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_SETReduce_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_SETReduce_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_DIST_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_DIST_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_DIST_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_ADAS_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_ADAS_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_ADAS_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_DiagInfoSW
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_DiagInfoSW(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_MASK_0;

    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LIN_1_MFS_DiagInfoSW_MFS_LIN_1_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_MFS_Answer_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_MFS_Answer_switch_signal(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be written */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] &= (l_u8) ~LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_MASK_0;

    /* And OR with shifted and masked value  */
    LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
       (l_u8) (((l_u8) (v << LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_BIT_OFFSET)) & LIN_1_MFS_Answer_switch_signal_MFS_LIN_1_SIG_MASK_0);

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}










/*******************************************************************************
* Function Name: l_flg_tst_LIN_1
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  fff - is the name of the flag handle.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_LIN_1(l_flag_handle fff)
{
    l_bool returnValue;

    switch(fff)
    {
        case MRF_FLAG_HANDLE:
            returnValue = l_flg_tst_MRF();
        break;

        case SRF_FLAG_HANDLE:
            returnValue = l_flg_tst_SRF();
        break;

        case MFS_Return_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Return_switch_signal();
        break;

        case MFS_Menu_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Menu_switch_signal();
        break;

        case MFS_Up_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Up_switch_signal();
        break;

        case MFS_Down_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Down_switch_signal();
        break;

        case MFS_OK_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_OK_switch_signal();
        break;

        case MFS_Speech_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Speech_switch_signal();
        break;

        case MFS_Mode_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Mode_switch_signal();
        break;

        case MFS_VolPlus_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_VolPlus_switch_signal();
        break;

        case MFS_VolReduce_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_VolReduce_switch_signal();
        break;

        case MFS_Mute_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Mute_switch_signal();
        break;

        case MFS_SeekPlus_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_SeekPlus_switch_signal();
        break;

        case MFS_SeekReduce_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_SeekReduce_switch_signal();
        break;

        case MFS_Crusie_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Crusie_switch_signal();
        break;

        case MFS_RESPlus_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_RESPlus_switch_signal();
        break;

        case MFS_SETReduce_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_SETReduce_switch_signal();
        break;

        case MFS_DIST_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_DIST_switch_signal();
        break;

        case MFS_ADAS_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_ADAS_switch_signal();
        break;

        case MFS_DiagInfoSW_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_DiagInfoSW();
        break;

        case MFS_Answer_switch_signal_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_Answer_switch_signal();
        break;

        case MFS_LIN_1_FLAG_HANDLE:
            returnValue = l_flg_tst_MFS_LIN_1();
        break;

        default:
            returnValue = LIN_1_TRUE;
        break;
    }

    return(returnValue);
}


/*******************************************************************************
* Function Name: l_flg_tst_MRF
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the Master Request Frame
*  (MRF), i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the Master Request Frame
*  (MRF):
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MRF(void)
{
    l_bool result;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MRF_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }
    else
    {
        result = LIN_1_FALSE;
    }
    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_SRF
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the Slave Response Frame
*  (SRF), i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the Slave Response Frame
*  (SRF):
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_SRF(void)
{
    l_bool result;

    if (0u != (LIN_1_statusFlagArray[LIN_1_SRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_SRF_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }
    else
    {
        result = LIN_1_FALSE;
    }
    return (result);
}



/*******************************************************************************
* Function Name: l_flg_tst_MFS_Return_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Return_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Return_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Return_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Menu_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Menu_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Up_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Up_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Up_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Up_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Down_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Down_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Down_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Down_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_OK_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_OK_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_OK_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_OK_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Speech_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Speech_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Mode_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Mode_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_VolPlus_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_VolPlus_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_VolReduce_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_VolReduce_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Mute_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Mute_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_SeekPlus_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_SeekPlus_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_SeekReduce_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_SeekReduce_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Crusie_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Crusie_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_RESPlus_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_RESPlus_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_SETReduce_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_SETReduce_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_DIST_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_DIST_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_ADAS_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_ADAS_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_DiagInfoSW
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_DiagInfoSW(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_DiagInfoSW_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_DiagInfoSW_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_Answer_switch_signal
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_Answer_switch_signal(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_MFS_LIN_1
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MFS_LIN_1(void)
{
    l_bool result = LIN_1_FALSE;

    if (0u != (LIN_1_statusFlagArray[LIN_1_MFS_LIN_1_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MFS_LIN_1_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return (result);
}






/*******************************************************************************
* Function Name: l_flg_clr_LIN_1
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  fff - is the name of the flag handle.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_LIN_1(l_flag_handle fff)
{
    switch(fff)
    {
        case MRF_FLAG_HANDLE:
            l_flg_clr_MRF();
        break;

        case SRF_FLAG_HANDLE:
            l_flg_clr_SRF();        break;

        case MFS_Return_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Return_switch_signal();
        break;

        case MFS_Menu_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Menu_switch_signal();
        break;

        case MFS_Up_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Up_switch_signal();
        break;

        case MFS_Down_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Down_switch_signal();
        break;

        case MFS_OK_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_OK_switch_signal();
        break;

        case MFS_Speech_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Speech_switch_signal();
        break;

        case MFS_Mode_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Mode_switch_signal();
        break;

        case MFS_VolPlus_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_VolPlus_switch_signal();
        break;

        case MFS_VolReduce_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_VolReduce_switch_signal();
        break;

        case MFS_Mute_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Mute_switch_signal();
        break;

        case MFS_SeekPlus_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_SeekPlus_switch_signal();
        break;

        case MFS_SeekReduce_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_SeekReduce_switch_signal();
        break;

        case MFS_Crusie_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Crusie_switch_signal();
        break;

        case MFS_RESPlus_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_RESPlus_switch_signal();
        break;

        case MFS_SETReduce_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_SETReduce_switch_signal();
        break;

        case MFS_DIST_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_DIST_switch_signal();
        break;

        case MFS_ADAS_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_ADAS_switch_signal();
        break;

        case MFS_DiagInfoSW_FLAG_HANDLE:
            l_flg_clr_MFS_DiagInfoSW();
        break;

        case MFS_Answer_switch_signal_FLAG_HANDLE:
            l_flg_clr_MFS_Answer_switch_signal();
        break;

        case MFS_LIN_1_FLAG_HANDLE:
            l_flg_clr_MFS_LIN_1();
        break;

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_flg_clr_MRF
********************************************************************************
*
* Summary:
*  Sets the current value of the Master Request Frame (MRF) flag.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MRF(void)
{
    LIN_1_statusFlagArray[LIN_1_MRF_FRAME_FLAG_BYTE_OFFSET_0] &=
        ((l_u8) ~LIN_1_MRF_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_SRF
********************************************************************************
*
* Summary:
*  Sets the current value of the Slave Response Frame (SRF) flag.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_SRF(void)
{
    LIN_1_statusFlagArray[LIN_1_SRF_FRAME_FLAG_BYTE_OFFSET_0] &=
    ((l_u8) ~LIN_1_SRF_FRAME_FLAG_MASK_0);
}



/*******************************************************************************
* Function Name: l_flg_clr_MFS_Return_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Return_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Return_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Return_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Menu_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Menu_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Menu_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Up_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Up_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Up_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Up_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Down_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Down_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Down_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Down_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_OK_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_OK_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_OK_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_OK_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Speech_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Speech_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Speech_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Mode_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Mode_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Mode_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_VolPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_VolPlus_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_VolPlus_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_VolReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_VolReduce_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_VolReduce_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Mute_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Mute_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Mute_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_SeekPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_SeekPlus_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_SeekPlus_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_SeekReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_SeekReduce_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_SeekReduce_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Crusie_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Crusie_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Crusie_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_RESPlus_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_RESPlus_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_RESPlus_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_SETReduce_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_SETReduce_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_SETReduce_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_DIST_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_DIST_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_DIST_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_ADAS_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_ADAS_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_ADAS_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_DiagInfoSW
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_DiagInfoSW(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_DiagInfoSW_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_DiagInfoSW_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_Answer_switch_signal
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_Answer_switch_signal(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_Answer_switch_signal_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_MFS_LIN_1
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MFS_LIN_1(void)
{
    LIN_1_statusFlagArray[LIN_1_MFS_LIN_1_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_MFS_LIN_1_FRAME_FLAG_MASK_0);
}






#if (1u == LIN_1_LIN_1_3)
    /**************************************************************************
    * Function Name: l_ifc_disconnect
    ***************************************************************************
    *
    * Summary:
    *  Stops the component operation.
    *  This function is implemented because it is mandatory for LIN v.1.3 API.
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    **************************************************************************/
    l_bool l_ifc_disconnect_LIN_1(void)
    {
        l_u8 return_value = LIN_1_FALSE;
        LIN_1_Stop();

        return (return_value);
    }


    /**************************************************************************
    * Function Name: l_ifc_connect
    ***************************************************************************
    *
    * Summary:
    *  Inilizes LIN interface.
    *  This function is implemented because it is mandatory for LIN v.1.3 API.
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    **************************************************************************/
    l_bool l_ifc_connect_LIN_1(void)
    {
        return (l_ifc_init_LIN_1());
    }
#endif /* (1u == LIN_1_LIN_1_3) */


/******************************************************************************
* Function Name: l_ifc_init_LIN_1
*******************************************************************************
*
* Summary:
*  The function initializes the LIN Slave component instance that is specified
*  by the name iii. It sets up internal functions such as the Baud rate and
*  starts up digital blocks used by the LIN Slave component. This is
*  the first call that must be performed before using any other interface-
*  related LIN Slave API functions.
*
* Return:
*  0        - If the operation succeeds.
*  1        - If an invalid operation parameter is passed to the function.
*
******************************************************************************/
l_bool l_ifc_init_LIN_1(void)
{
    l_u8 interruptState;
    l_u8 i;
    l_bool returnValue = LIN_1_FALSE;

    interruptState = CyEnterCriticalSection();

    /* Sets the internal clock divider with effect on the end of a cycle.
     *  The divider's value is decremented to be compatible with the
     *  SetFractionalDividerRegister() parameters */
    #if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
        LIN_1_IntClk_Stop();
        LIN_1_IntClk_SetFractionalDividerRegister((LIN_1_initialClockDivider - 1u), 0u);
        LIN_1_IntClk_Start();
    #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

    /* Initialize UART */
    LIN_1_SCB_Init();

    /* Set Break Detection Threshold in counts */
    LIN_1_SCB_UART_RX_CTRL_REG |= (uint32)((uint32)(LIN_1_BREAK_THRESHOLD - 1u) <<
                                             LIN_1_SCB_UART_RX_CTRL_BREAK_WIDTH_POS);

    /* Place SCB UART in LIN compatibility mode  */
    LIN_1_SCB_UART_RX_CTRL_REG |= (uint32)LIN_1_SCB_UART_RX_CTRL_LIN_MODE;

    /* Configure BREAK DETECT, BAUD DETECT and FIFO NOT EMPTY as interrupt sources */
    LIN_1_SCB_INTR_RX_MASK_REG |= LIN_1_SCB_INTR_RX_BREAK_DETECT |
            LIN_1_SCB_INTR_RX_BAUD_DETECT | LIN_1_SCB_INTR_RX_NOT_EMPTY;

    /* Copy PIDs from NVRAM to VRAM */
    for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
    {
        LIN_1_volatileConfig[i] = LIN_1_LinSlaveConfig.pidTable[i];
    }

    #if (LIN_1_TL_USED)
        LIN_1_nad = LIN_1_LinSlaveConfig.initialNad;
        
        #if (LIN_1_SAE_J2602_ENABLED)
            (void) LIN_1_UpdateMessageIDs(LIN_1_nad);
        #endif /* LIN_1_SAE_J2602_ENABLED */
    #endif /* LIN_1_TL_USED */    

    /* Clears interface status */
    LIN_1_ifcStatus &= (l_u16) ~LIN_1_IFC_STS_MASK;

    #if (1u == LIN_1_INACTIVITY_ENABLED)
        /* Clear period timer counter */
        LIN_1_periodCounter = 0x00u;
    #endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Clears the ioctl status register */
    LIN_1_ioctlStatus = 0x00u;

    LIN_1_SCB_Enable();

    CyExitCriticalSection(interruptState);

    /* Bus inactivity block configuration */
    #if (1u == LIN_1_INACTIVITY_ENABLED)
        /* Starts SysTick timer with default period = 1mS */
        if (CYRET_SUCCESS != LIN_1_TimerEnableInt())
        {
            /* Reports an error because all SysTick callback slots are busy */
            returnValue = LIN_1_TRUE;
        }
    #endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Clears pending SCB interrupts */
    LIN_1_ISR_ClearPending();

    /* Enables SCB interrupt */
    LIN_1_ISR_StartEx(&LIN_1_UART_ISR);

    return (returnValue);
}


/******************************************************************************
* Function Name: l_ifc_wake_up
*******************************************************************************
*
* Summary:
*  This function transmits one wakeup signal. The wakeup signal is transmitted
*  directly when this function is called. When you call this API function, the
*  application is blocked until the wakeup signal is transmitted to the LIN bus.
*  The CyDelayUs() function is used as the timing source. The delay is
*  calculated based on the clock configuration entered in PSoC Creator.
*
******************************************************************************/
void l_ifc_wake_up_LIN_1(void)
{
    #if (0u == LIN_1_LIN_1_3)
        /* For LIN 2.x: Force TXD low by transmission of N dominant bits
        *  (N depends on baudrate)
        *  To calculate how much of bit-interval takes 300uS wake-up pulse:
        *  1) Calculate bit period: 1000000(uS) divide by BAUD_RATE(bps)
        *  2) Divide WAKE_UP_SIGNAL_LENGTH by result of 1)
        *  3) Add 1 to compensate rounding error
        *  4) Shift 0xFF constant by number of bit obtained in 3)
        *  form dominant level pulse with duration proportional to Baud rate
        */
        LIN_1_SCB_SpiUartWriteTxData((uint8) (0xffu << ((LIN_1_WAKE_UP_SIGNAL_LENGTH /
            (1000000u / LIN_1_BAUD_RATE)) + 1u)));

        /* Waits until symbol transmission ends, Tdelay > 11mS ,
        *  11 bit-times at min. baudrate 1000bps,for  8N1.5 symbol */
        CyDelay(LIN_1_WAKEUP_TX_DELAY);
    #else
        /* For LIN 1.3: Simply send 0x80 as LIN 1.3 specifies */
        LIN_1_SCB_SpiUartWriteTxData((uint8) 0x80);
    #endif /* (0u == LIN_1_LIN_1_3) */
}


/******************************************************************************
* Function Name: l_ifc_ioctl
*******************************************************************************
*
* Summary:
*  This function controls functionality that is not covered by the other API
*  calls. It is used for protocol specific parameters or hardware specific
*  functionality.
*
* Parameters:
*  op - The operation to be applied.
*  pv - The pointer to the optional parameter.
*
* Return:
*  There is no error code value returned for the operation selected. This means
*  that you must ensure that the values passed into the function are correct.
*
*  L_IOCTL_READ_STATUS operation:
*  The first bit in this byte is the flag that indicates there was no
*  signalling on the bus for a certain elapsed time (available when the
*  Bus Inactivity Timeout Detection option is enabled). If the elapsed time
*  passes a certain threshold, this flag is set. Calling this API clears
*  all the status bits after they are returned. The second bit is the flag that
*  indicates that a Targeted Reset service request (0xB5) is received
*  (when J2602-1 Compliance is enabled).
*
*  Symbolic Name : LIN_1_IOCTL_STS_BUS_INACTIVITY
*  Value         : 0x0001u
*  Description   : No signal has been detected on the bus for a certain elapsed
*                  time
*
*  Symbolic Name : LIN_1_IOCTL_STS_TARGET_RESET
*  Value         : 0x0002u
*  Description   : A Targeted Reset service request (0xB5) has been received.
*
*  L_IOCTL_SET_BAUD_RATE operation:
*  0 is returned if operation succeeds and 1 if an invalid operation
*  parameter is passed to the function.
*
*  L_IOCTL_SLEEP operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*
*  L_IOCTL_WAKEUP operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*
*  L_IOCTL_SYNC_COUNTS operation:
*  Returns the current number of sync field timer counts as l_u16 in *pv.
*
*  L_IOCTL_SET_SERIAL_NUMBER operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*  
*  L_IOCTL_GET_NAD      
*  Returns configured NAD to l_u8 *pv
*
*  L_IOCTL_SET_NAD      
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or invalid NAD is passed to the function.
*
*  L_IOCTL_GET_FRAME_PID
*  Frame PID by its index in frame table, starting from 0. 
*  Uses LIN_1_NEW_PID structure to return PID.
*  LIN_1_INVALID_FRAME_PID is returned for indexes out of range.
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or out of range value is passed to the function.
*
*  L_IOCTL_SET_FRAME_PID
*  Sets Frame PID by its index in frame table, starting from 0.
*  Uses a pointer to LIN_1_NEW_PID structure as input parameter.
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or out of range value is passed to the function.
*
*  L_IOCTL_SET_DNN      
*  Sets DNN value to initial NAD and configured NAD, and 
*  limits NAD range to 0x60-0x6F. DNN and NAD are always set. 
*  CYRET_BAD_PARAM is returned if Message ID table can not be updated correctly.
*
*  L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID
*  Finds a frame with specified messageID and sets its PID.
*  CYRET_BAD_PARAM is returned if Message ID is not found in the table.
*
*******************************************************************************/
l_u16 l_ifc_ioctl_LIN_1(l_ioctl_op op, void* pv)
{
    l_u16 returnValue = (l_u16) CYRET_SUCCESS;
    l_u8 interruptState;
    #if (LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED)
        LIN_1_NEW_PID *newPID;
    #endif /* LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED*/
    switch (op)
    {
        /***********************************************************************
        * Read Status
        ***********************************************************************/
        case L_IOCTL_READ_STATUS:
            /* Return status */
            returnValue = LIN_1_ioctlStatus;

            /* Clear status */
        #if (LIN_1_SAE_J2602_ENABLED && \
            (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL)))
                LIN_1_ioctlStatus &= (l_u16)~(l_u16)LIN_1_IOCTL_STS_TARGET_RESET;
        #endif /* (LIN_1_SAE_J2602_ENABLED && 
                * (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL)) */
        break;
        /***********************************************************************
        * Set Baud Rate
        ***********************************************************************/
        case L_IOCTL_SET_BAUD_RATE:
            interruptState = CyEnterCriticalSection();
            /* Set new internal clock divider with effect on the end of a cycle.
            *  The divider's value is decremented to be compatible with the
            *  SetFractionalDividerRegister() parameters */
            LIN_1_IntClk_Stop();
            LIN_1_IntClk_SetFractionalDividerRegister(((*((l_u16*)pv)) - 1u), 0u);
            #if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
                LIN_1_initialClockDivider = *(((l_u16*)pv));
            #endif /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */
            LIN_1_IntClk_Start();
            CyExitCriticalSection(interruptState);
        break;
        /***********************************************************************
        * Prepare for low power modes
        ***********************************************************************/
        case L_IOCTL_SLEEP:
            LIN_1_SCB_Stop();
            
            /* Clears SCB FIFOs*/
            LIN_1_SCB_CLEAR_TX_FIFO;
            LIN_1_SCB_CLEAR_RX_FIFO;
            
            /* Disables interrupt from this instance */
            LIN_1_ISR_Disable();
            
             /* Clears pending interrupts */
            LIN_1_ISR_ClearPending();
            LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_ALL);
        break;
        /***********************************************************************
        * Restore after wakeup from low power modes
        ***********************************************************************/
        case L_IOCTL_WAKEUP:
            (void)l_ifc_init_LIN_1();
        break;
    #if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
        case L_IOCTL_SYNC_COUNTS:
            /* Returns current number of sync field timer counts */
            *((l_u16*) pv) = (l_u16) LIN_1_syncCounts;
        break;
    #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */
    
    #if (LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED)
        case L_IOCTL_SET_SERIAL_NUMBER:
            LIN_1_serialNumber = (l_u8*) pv;
        break;
        /***********************************************************************
        * Gets configured NAD. NAD must not be 00, 7E nor 7F.
        ***********************************************************************/
        case L_IOCTL_GET_NAD:
            *((l_u8*) pv) = LIN_1_nad;
        break;
        /***********************************************************************
        * Sets configured NAD. NAD must not be 00, 7E nor 7F.
        ***********************************************************************/
        case L_IOCTL_SET_NAD:
            if ((((*(l_u8*) pv)) != LIN_1_NAD_FUNCTIONAL) &&
                (((*(l_u8*) pv)) != LIN_1_NAD_BROADCAST)  &&
                (((*(l_u8*) pv)) != LIN_1_NAD_GO_TO_SLEEP))
            {
                #if (LIN_1_SAE_J2602_ENABLED) 
                    returnValue = (l_u16) LIN_1_UpdateMessageIDs(*((l_u8*) pv));
                    if (returnValue == (l_u16) CYRET_BAD_PARAM)
                    {
                        break; 
                    }
                #endif /* LIN_1_SAE_J2602_ENABLED */
                
                LIN_1_nad = *((l_u8*) pv);                
            }
            else
            {
                returnValue = (l_u16) CYRET_BAD_PARAM;
            }
        break;
        /***********************************************************************
        * Gets frame PIDs by the frame table index.
        * Uses pointer to the LIN_1_NEW_PID structure
        * as parameter. Input LIN_1_NEW_PID.index. 
        * Returns LIN_1_NEW_PID.PID 
        ***********************************************************************/
        case L_IOCTL_GET_FRAME_PID:
            newPID = (LIN_1_NEW_PID*) pv;
            if (newPID->index < LIN_1_NUM_FRAMES)
            {
                newPID->PID = LIN_1_volatileConfig[newPID->index];
            }
            else
            {
                returnValue = (l_u16) CYRET_BAD_PARAM;
                newPID->PID = LIN_1_INVALID_FRAME_PID;
            }
        break;
        /***********************************************************************
        * Sets frame PIDs by frame table index.
        * Uses LIN_1_NEW_PID structure
        * as input parameter.
        ***********************************************************************/
        case L_IOCTL_SET_FRAME_PID:
            newPID = (LIN_1_NEW_PID*) pv;
            
            if ((newPID->index < LIN_1_NUM_FRAMES) &&
                ((newPID->PID & LIN_1_PID_PARITY_MASK) < LIN_1_FRAME_PID_MRF))
            {
                LIN_1_volatileConfig[newPID->index] = newPID->PID;
            }
            else
            {
            returnValue = (l_u16) CYRET_BAD_PARAM;
            }
        break;
        
        #if (LIN_1_SAE_J2602_ENABLED)
            /***********************************************************************
            * Sets DNN value to initial NAD and configured NAD.
            * Limits NAD range to 0x60-0x6F.
            ***********************************************************************/
            case L_IOCTL_SET_DNN:
                    LIN_1_LinSlaveConfig.initialNad = (*((l_u8*) pv) &
                    LIN_1_NAD_DNN_MASK) | LIN_1_NAD_J2602_BASE;
                    LIN_1_nad = LIN_1_LinSlaveConfig.initialNad;
                    returnValue = (l_u16) LIN_1_UpdateMessageIDs(LIN_1_nad);
            break;
        #endif /* LIN_1_SAE_J2602_ENABLED */
        
        #if (LIN_1_LIN_2_0 == 1u)
        /***********************************************************************
        * Sets frame PIDs by Message ID. (LIN 2.0 only)
        * Uses LIN_1_NEW_PID_BY_MSG_ID structure
        * as input parameter.
        ***********************************************************************/
        l_u8 i;
        case L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID:
            returnValue = (l_u16) CYRET_BAD_PARAM;
            
            for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
            {
                if (messageIdTable[i] == (*((LIN_1_NEW_PID_BY_MSG_ID*) pv)).messageID)
                {
                    /* MessageID is found in the table */
                    LIN_1_volatileConfig[i] = (*((LIN_1_NEW_PID_BY_MSG_ID*) pv)).PID;
                    returnValue = (l_u16) CYRET_SUCCESS;
                    break;
                }
            }
        break;
        #endif /* (LIN_1_LIN_2_0 == 1u) */
    #endif /* (LIN_1_TL_USED && LIN_1_CFG_SERVICES_ENABLED) */

        /***********************************************************************
        * Unknown operation            
        ***********************************************************************/
        default:
            /* Unknown operation */
            returnValue = (l_u16) CYRET_BAD_PARAM;
        break;
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: l_ifc_rx
********************************************************************************
*
* Summary:
*  The LIN Slave component takes care of calling this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
*******************************************************************************/
void l_ifc_rx_LIN_1(void)
{
    l_u8 i;
    l_u8 interruptState;
#if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
    uint32 tmp;
    static uint32 LIN_1_correctedClockDivider;
#endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */
    static l_u16 LIN_1_interimChecksum;   /* Holds interim checksum value      */
    static l_u8 LIN_1_framePid;           /* PID of current frame              */
    static l_u8 LIN_1_frameSize;          /* Size of frame being processed     */
    static l_u8 LIN_1_bytesTransferred;   /* Number of transferred bytes       */
    static l_u8 LIN_1_tmpData;            /* Used to store transmitted byte    */
    static l_u8 LIN_1_pidIndex;           /* Index in pidInfoTable             */
    static l_u8 LIN_1_tmpRxFrameData[8u]; /* RXed data before checksum checked */
    static volatile l_u8* LIN_1_frameData = NULL;  /* Pointer to frame data, points to byte to be sent.*/
    static l_u8 LIN_1_frameErrorAfterPID;

    /* PID information table */
    static LIN_1_PID_INFO_TABLE const CYCODE LIN_1_pidInfoTable[LIN_1_NUM_FRAMES] =
{
    {(LIN_1_FRAME_DIR_PUBLISH | LIN_1_FRAME_DATA_SIZE_8 | LIN_1_FRAME_TYPE_UNCOND),
     LIN_1_Buffer_MFS_LIN_1}
};

   /* Sets bus activity interface status bit */
    LIN_1_ifcStatus |= LIN_1_IFC_STS_BUS_ACTIVITY;

#if (1u == LIN_1_INACTIVITY_ENABLED)
    /* Clears the period timer counter */
    LIN_1_periodCounter = 0x00u;
    /* Clears bus inactivity ioctl status bit */
    LIN_1_ioctlStatus &= (l_u16)(~(l_u16) LIN_1_IOCTL_STS_BUS_INACTIVITY);
#endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Checks for RX UART framing error and overflow  */
    if (0u != (LIN_1_SCB_INTR_RX_REG & (LIN_1_SCB_INTR_RX_FRAME_ERROR |
               LIN_1_SCB_INTR_RX_OVERFLOW)))
    {
        LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_ERR | 
                                                    LIN_1_SCB_INTR_RX_OVERFLOW);
         /* Set framing error */
        LIN_1_fsmFlags |= LIN_1_FSM_FRAMING_ERROR_FLAG;
    }

    /***************************************************************************
    *                       Break Field Detected
    ***************************************************************************/
    if (0u != (LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_BREAK_DETECT))
    {
        /* Resets pending flag */
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_BREAK_DETECT;
        /* Framing error or data transfer is aborted */
        if (0u != (LIN_1_FSM_DATA_RECEIVE & LIN_1_fsmFlags))
        {
            /* Sets response error */
            LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
        #if (LIN_1_SAE_J2602_ENABLED)
            /* Sets ERR2 bit */
            LIN_1_j2602Status_Set(LIN_1_J2602_STS_DATA_ERR);
        #endif  /* LIN_1_SAE_J2602_ENABLED */
           
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

        }   /* No response error, continue */
        
    /* Auto Baud Rate Sync Enabled */
    #if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)
        LIN_1_IntClk_Stop();
        /* Restores the initial clock divider */
        LIN_1_IntClk_SetFractionalDividerRegister((uint16) 
            (LIN_1_initialClockDivider - 1u), 0u);
        LIN_1_IntClk_Start();
    #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */
    
        LIN_1_fsmFlags &= ((l_u8) ~(LIN_1_FSM_DATA_RECEIVE |
                                LIN_1_FSM_FRAMING_ERROR_FLAG));
                                
        /* Sets UART ISR FSM to IDLE state */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_0_IDLE;
        /* Resets UART state machine */
        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
        
        /* Starts the BREAK to SYNC timeout counter for SYNC failure detection*/
        LIN_1_breakToSyncCounts = 1u; /* Counter increments the value if non-zero. */
        return;
    }

    /***********************************************************************
    *                       Sync Field Complete                            *
    ***********************************************************************/
    if (0u != (LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_BAUD_DETECT))
    {
        /* Clears SYNC source first */
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_BAUD_DETECT;
        
        /* Stops and disable the BREAK to SYNC timeout counter */
        LIN_1_breakToSyncCounts=0u; /* counter does not increment if zero */
        
    #if (1u == LIN_1_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Enabled */        
        LIN_1_syncCounts = (l_u16)(LIN_1_SCB_UART_RX_STATUS_REG &
            LIN_1_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
            
        /* Checks if correction is necessary */
        if (LIN_1_syncCounts != LIN_1_EXPECTED_TIME_COUNTS)
        {
            /* Clock max deviation < 14%. +0.5% from Master
             * So if deviation > (14.5*128/100) = 18,56 (0x13) - drop the packet
             */
            if ( (int32) LIN_1_TIME_COUNTS_MAX_DEVIATION > (LIN_1_ABS(
                 (int32) LIN_1_syncCounts - (int32) LIN_1_EXPECTED_TIME_COUNTS)) )
            {
                /* BR_COUNTER uses 0x80 as the base to measure the frequency,
                 * so divide its constant by 0x80 and round off the result checking the last thrown bit */
                tmp = ((uint32)LIN_1_initialClockDivider * (uint32)LIN_1_syncCounts);
                LIN_1_correctedClockDivider = (uint32) (tmp >> LIN_1_BR_BASE_SHIFT);
                LIN_1_correctedClockDivider += (l_u16)
                    (( 0u != (tmp & (l_u8)(1u << (LIN_1_BR_BASE_SHIFT - 1u))) ) ? 1u : 0u);
                LIN_1_IntClk_Stop();
                LIN_1_IntClk_SetFractionalDividerRegister((uint16)
                    (LIN_1_correctedClockDivider - 1u), 0u);
                LIN_1_IntClk_Start();
            }
            else /* Clocks differ > +/-14% so reset the state machine */
            {
                /* Reset UART state machine */
                LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                /* Nothing to process in RX/TX state machine due to bad SYNC */
                return;                
            }
        }
    #else   /* If SAE_J2602_ENABLED and there is no auto-bauding, check the SYNC byte field for errors. */
        #if (LIN_1_SAE_J2602_ENABLED)
            LIN_1_syncCounts = (l_u16)(LIN_1_SCB_UART_RX_STATUS_REG &
                LIN_1_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
                
            /* Check if Baud rate is in the range */
            if (LIN_1_syncCounts != LIN_1_EXPECTED_TIME_COUNTS)
            {
                /* Clock max deviation < 2% from Master
                 * So if deviation > (2*128/100) = 2,56 (0x03) - drop the packet
                 */
                if ( (int32) LIN_1_TIME_COUNTS_MAX_DEVIATION < (LIN_1_ABS(
                     (int32) LIN_1_syncCounts - (int32) LIN_1_EXPECTED_TIME_COUNTS)) )
                {
                 /* Set J2602 response error  */
                
                    /* Set ERR2, ERR1 and ERR0 bits */
                    LIN_1_j2602Status_Set(LIN_1_J2602_STS_DATA_ERR);
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            
                    /* Reset UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    /* Nothing to process in RX/TX state machine due to bad SYNC */
                    return;
                }
            }
        #endif /* LIN_1_SAE_J2602_ENABLED */
    #endif /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

         /* Sets the UART ISR FSM to state 1 (PID awaiting) */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_1_PID;
         /* Sets UART enabled flag */
        LIN_1_fsmFlags |= LIN_1_FSM_UART_ENABLE_FLAG;
         /* Clears any pending UART interrupt and RX FIFO */
        LIN_1_SCB_SpiUartClearRxBuffer();
        /* Clears all UART pending interrupts */
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_ALL;
    }

    /* Returns if there is nothing to process in RX/TX state machine */
    if (0u == (LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_NOT_EMPTY))
    {
        return;
    }

    switch (LIN_1_uartFsmState)
    {
        /***********************************************************************
        *                       IDLE state
        * State description:
        *  - Receives a sporadic byte not predicted by the BREAK/SYNC sequence,
        *    so does not set Response Error
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_0_IDLE:
            LIN_1_SCB_SpiUartClearRxBuffer();
            LIN_1_SCB_INTR_RX_REG = (uint32) LIN_1_SCB_INTR_RX_NOT_EMPTY;
            /* Reset UART state machine */
            LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
        break;
        /***********************************************************************
        *                       PID Field Byte Receive
        * State description:
        *  - Receives protected identifier (PID)
        *  - Checks PID parity
        *  - Set flags
        *  - Determine next state (RX or TX)
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_1_PID:
            if (0u != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags))
            {
                /* Reset UART State Machine */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
                break;
            }
        #if (LIN_1_SAE_J2602_ENABLED)
            /* Updates J2602_Status signal to be ready for next SRF */
            LIN_1_J2602_Status_Write(LIN_1_j2602Status_Get());
        #endif /* LIN_1_SAE_J2602_ENABLED */
            
            /* Saves PID */
            LIN_1_framePid = (l_u8) LIN_1_SCB_SpiUartReadRxData();
            
            /* Resets number of transferred bytes */
            LIN_1_bytesTransferred = 0u;
            
            /* Clears checksum byte */
            LIN_1_interimChecksum = 0u;
            
            /* Clears Frame Error after PID flag - such a condition must be processed separately,
             * as it can be Frame Error caused by the Break field of the next frame
             */
            LIN_1_frameErrorAfterPID = 0u;
            
            /* Verifies PID parity */
            if (LIN_1_parityTable[LIN_1_framePid & LIN_1_PID_PARITY_MASK] !=
                LIN_1_framePid)
            {
            /* Sets J2602 response error  */
            #if (LIN_1_SAE_J2602_ENABLED)
                /* Sets ERR2, ERR1 and ERR0 bits */
                LIN_1_j2602Status_Set(LIN_1_J2602_STS_PARITY_ERR);
                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            #endif /* LIN_1_SAE_J2602_ENABLED */
            
                /* Resets UART State Machine */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else    /* PID parity is correct */
            {
                /* Checks if MRF or SRF frame */
            #if (LIN_1_SAE_J2602_ENABLED)
                if ((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                    (LIN_1_FRAME_PID_MRF_J2602 == LIN_1_framePid) ||
                    (LIN_1_FRAME_PID_SRF == LIN_1_framePid))
            #else
                if ((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                    (LIN_1_FRAME_PID_SRF == LIN_1_framePid))                    
            #endif /* (LIN_1_SAE_J2602_ENABLED) */
                {
                    /*  Transport Layer section. MRF and SRF detection */
                #if (LIN_1_TL_USED)                    
                    #if (LIN_1_SAE_J2602_ENABLED)
                        if ((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                        (LIN_1_FRAME_PID_MRF_J2602 == LIN_1_framePid))
                    #else
                        if (LIN_1_FRAME_PID_MRF == LIN_1_framePid)
                    #endif /* LIN_1_SAE_J2602_ENABLED */
                    {
                        /* Indicates that Slave is required to receive data. */
                        LIN_1_tlFlags |= LIN_1_TL_RX_DIRECTION;

                        /*******************************************************
                        *               Cooked & RAW API
                        *******************************************************/
                        /* If MRF PID is detected, passes the pointer to the start of
                        * Frame Buffer and size of data to RX state to handle data receiving */

                        /* Frame equals 8 bytes */
                        LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;
                        /* Sets the frame data pointer to start of frame buffer. */
                        LIN_1_frameData = LIN_1_mrfBuffer;
                        /* Switches to subscribe data state */
                        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_3_RX;
                        
                        #if (LIN_1_SAE_J2602_ENABLED)
                            /* Checks for frame with ID == 0x3E (PID == 0xFE) */
                            if (LIN_1_framePid == LIN_1_FRAME_PID_MRF_J2602)
                            {
                                LIN_1_auxStatus |= LIN_1_AUXSTS_MRF_WAS_3E;
                            }
                            else /* and if the frame is 0x3C, clears the status */
                            {
                                LIN_1_auxStatus &= (l_u8)~LIN_1_AUXSTS_MRF_WAS_3E;
                            }
                        #endif /* LIN_1_SAE_J2602_ENABLED */
                    }

                    if (LIN_1_FRAME_PID_SRF == LIN_1_framePid)
                    {
                        if (0u != (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
                        {
                            /* Frame is always equal to 8 bytes for TL */
                            LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;
                            /* Sets the frame data pointer to the frame buffer start. */
                            LIN_1_frameData = LIN_1_srfBuffer;
                            
                            /* Sends the first byte to LIN Master. */
                            LIN_1_tmpData = *LIN_1_frameData;
                            LIN_1_frameData++;
                            LIN_1_SCB_SpiUartWriteTxData((uint32) LIN_1_tmpData);
                            LIN_1_bytesTransferred = 1u;
                            
                            /* Switch to publish data state. */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;
                            /* One or more data bytes have been received */
                            LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                            /* Indicates to Transport layer that Slave transmits the frame. */
                            LIN_1_tlFlags |= LIN_1_TL_TX_DIRECTION;
                        }
                        else
                        {
                            /* This part of code handles LIN Transport Layer. Sends one frame of a message if
                             * applications prepared the message in the TX message buffer or TX queue and LIN Slave node
                             * state machine has the appropriate state (is ready for physical response transmission).
                             */
                            if ((0u != (LIN_1_tlFlags & LIN_1_TL_TX_REQUESTED)) &&
                                       (LIN_1_nodeState == LIN_1_TX_PHY_RESPONSE))
                            {
                            #if (LIN_1_RAW_TL_API)
                                #if (LIN_1_SAE_J2602_ENABLED)
                                    /* Inserts the J2602 status byte as byte 0 for frames with ID = 0x3E or NAD >= 0x80 */
                                    if ((0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_WAS_3E)) ||
                                        (LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX] > LIN_1_NAD_BROADCAST))
                                        {
                                            /* Indicate that J2602 will be placed to the outgoing frame and
                                            can be cleared if transmitted successfuly. 
                                            Used for MRF with ID = 0x3E or NAD >= 0x80. */
                                            LIN_1_auxStatus |= LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS;    
                                        }
                                        else
                                        {
                                            LIN_1_auxStatus &= (l_u8)(~LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS);
                                        }
                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                        
                                if (0u != LIN_1_txBufDepth)
                                {
                                    /* Fills SRF from the frame buffer */
                                    #if (LIN_1_SAE_J2602_ENABLED)
                                        /* Insert the J2602 status byte as byte 0 for frames with ID = 0x3E or NAD >= 0x80 */
                                        if (0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                        {
                                            LIN_1_srfBuffer[0] = LIN_1_j2602Status;
                                            
                                            /* Uncomment the next line to replace byte 0 of user data 
                                            *  but keep the numbering of the rest of the bytes. 
                                            */
                                            /* LIN_1_txRdIndex++; */
                                        }
                                        else
                                        {
                                            LIN_1_srfBuffer[0] =
                                                LIN_1_rawTxQueue[LIN_1_txRdIndex];
                                            LIN_1_txRdIndex++;
                                        }
                                        
                                        for (i = 1u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                    #else
                                        for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                    #endif /* LIN_1_SAE_J2602_ENABLED */
                                    {
                                        LIN_1_srfBuffer[i] =
                                            LIN_1_rawTxQueue[LIN_1_txRdIndex];
                                        /* Updates the index to TX queue */
                                        LIN_1_txRdIndex++;
                                    }
                                    /* The Read index should point to the next byte in MRF */
                                    if (LIN_1_TL_TX_QUEUE_LEN == LIN_1_txRdIndex)
                                    {
                                        LIN_1_txRdIndex = 0u;
                                    }
                                    /* 8 bytes were read from the SRF, so decrement the buffer depth. */
                                    LIN_1_txBufDepth--;
                                    /* Updates status properly */
                                    LIN_1_txTlStatus = (0u == LIN_1_txBufDepth) ?
                                                                  LD_QUEUE_EMPTY : LD_QUEUE_AVAILABLE;
                                    if (LIN_1_PDU_PCI_TYPE_UNKNOWN == LIN_1_txPrevPci)
                                    {
                                        if (LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] <=
                                            LIN_1_PDU_SF_DATA_LEN)
                                        {
                                            /* Get length of message from PCI field of SF */
                                            LIN_1_txMessageLength |=
                                                ((l_u16) LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX]);
                                        }
                                        else
                                        {
                                            /* Get length of Segmented message from PCI and LEN fields of FF 
                                             * NOTE The PCI field contains four MSb of the length (Length / 256).
                                             */
                                            LIN_1_txMessageLength = (l_u16)((((l_u16)
                                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX]) &
                                                ((l_u16) ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK))) << 8u);
                                            LIN_1_txMessageLength |=
                                            ((l_u16) LIN_1_srfBuffer[LIN_1_PDU_LEN_IDX]);
                                        }
                                    }
                                }
                            #endif /* LIN_1_RAW_TL_API */

                                /* Handles PDU packing for Cooked API. */
                                /* Checks the length so it shows if the message is already sent. */
                                if (LIN_1_txMessageLength == 0u)
                                {
                                    LIN_1_tlFlags &= (l_u8)~LIN_1_TL_TX_REQUESTED;
                                    /* Resets UART State Machine */
                                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                                }
                                /* Processes message sending */
                                else
                                {
                                    /* Analyze length to find the type of frame message to be sent */
                                    if (LIN_1_txMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                                    {
                                        /* Process the FF Frame */
                                        if (LIN_1_txPrevPci == LIN_1_PDU_PCI_TYPE_UNKNOWN)
                                        {
                                            /* Fill Frame PCI field */
                                            /* Save the previous PCI */
                                            LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_FF;
                                            LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                            LIN_1_tlTimeoutCnt = 0u;

                                        #if (LIN_1_COOKED_TL_API)
                                            /* Fill Frame NAD field */
                                            #if (LIN_1_SAE_J2602_ENABLED)
                                                /* Insert J2602 status byte as byte 0 of the frame *
                                                * for MRF with ID = 0x3E or NAD >= 0x80.              */
                                                if (0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LIN_1_srfBuffer[0u] = LIN_1_j2602Status;
                                                }
                                                else
                                                {
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;    
                                                }
                                            #else
                                                LIN_1_srfBuffer[0u] = LIN_1_nad;
                                            #endif /* LIN_1_SAE_J2602_ENABLED */
                                            
                                            LIN_1_srfBuffer[1u] = (LIN_1_PDU_PCI_TYPE_FF |
                                                (HI8(LIN_1_txMessageLength)));
                                                
                                            /* Fill Frame LEN field */
                                            LIN_1_srfBuffer[2u] = LO8(LIN_1_txMessageLength);
                                            /* Fill Frame Data fields */
                                            for (i = 3u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                            {
                                                LIN_1_srfBuffer[i] = LIN_1_txTlDataPointer[(i +
                                                    LIN_1_txTlDataCount) - 3u];
                                            }
                                            /* Update user buffer pointer */
                                            LIN_1_txTlDataCount += LIN_1_FRAME_DATA_SIZE_5;
                                            LIN_1_txMessageLength -= LIN_1_FRAME_DATA_SIZE_5;
                                        #endif /* LIN_1_COOKED_TL_API */
                                        }
                                        else    /* Process the CF Frame */
                                        {
                                        #if (LIN_1_COOKED_TL_API)
                                            #if (LIN_1_SAE_J2602_ENABLED)
                                                /* Inserts J2602 status byte as byte 0 of the frame */
                                                /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                if (0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LIN_1_srfBuffer[0u] = LIN_1_j2602Status;
                                                }
                                            #endif /* LIN_1_SAE_J2602_ENABLED */
                                             /* Fills Frame PCI field */
                                            LIN_1_srfBuffer[1u] = (LIN_1_PDU_PCI_TYPE_CF |
                                                LIN_1_txFrameCounter);
                                            /* Fills Frame Data fields */
                                            for (i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                            {
                                                LIN_1_srfBuffer[i] = LIN_1_txTlDataPointer[(i +
                                                    LIN_1_txTlDataCount) - 2u];
                                            }
                                            /* Updates the user buffer pointer */
                                            LIN_1_txTlDataCount += LIN_1_FRAME_DATA_SIZE_6;
                                        #endif /* LIN_1_COOKED_TL_API */

                                            /* Saves the previous PCI */
                                            LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_CF;
                                            
                                            /* Update length pointer properly */
                                            LIN_1_txMessageLength -= LIN_1_FRAME_DATA_SIZE_6;
                                            LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                            LIN_1_tlTimeoutCnt = 0u;
                                        }
                                    }
                                    else    /* Processes SF Frame or last CF Frame */
                                    {
                                        /* Checks if the Previous frame is in "Unknown" state, which indicates that
                                         * the current frame is SF, otherwise it is the last CF frame.
                                         * Fill the Frame PCI field properly.
                                         */
                                        LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                        LIN_1_tlTimeoutCnt = 0u;

                                            if (LIN_1_PDU_PCI_TYPE_UNKNOWN == LIN_1_txPrevPci)
                                            {
                                            #if (LIN_1_COOKED_TL_API)
                                                /* Fill Frame NAD field */
                                                #if (LIN_1_SAE_J2602_ENABLED)
                                                /* Inserts J2602 status byte as byte 0 of the frame */
                                                /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                if (0u !=(LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LIN_1_srfBuffer[0u] = LIN_1_j2602Status;
                                                }
                                                else
                                                {
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;    
                                                }
                                                #else
                                                LIN_1_srfBuffer[0u] = LIN_1_nad;
                                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                                LIN_1_srfBuffer[1u] = (l_u8)LIN_1_txMessageLength;
                                            #endif /* LIN_1_COOKED_TL_API */
                                                /* Save the previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_SF;
                                            }
                                            else
                                            {
                                            #if (LIN_1_COOKED_TL_API)
                                                /* Fill Frame NAD field */
                                                
                                                #if (LIN_1_SAE_J2602_ENABLED)
                                                    /* Inserts J2602 status byte as byte 0 of the frame */
                                                    /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                    if (0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                    { 
                                                        LIN_1_srfBuffer[0u] = LIN_1_j2602Status;
                                                    }
                                                    else
                                                    {
                                                        LIN_1_srfBuffer[0u] = LIN_1_nad;    
                                                    }
                                                #else
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;
                                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                                
                                                LIN_1_srfBuffer[1u] = (LIN_1_PDU_PCI_TYPE_CF |
                                                    LIN_1_txFrameCounter);
                                            #endif /* LIN_1_COOKED_TL_API */
                                            
                                                /* Save previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_CF;
                                            }

                                    #if (LIN_1_COOKED_TL_API)
                                        /* Fill Frame Data fields */
                                        for (i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                        {
                                            if (LIN_1_txMessageLength >= ((l_u8) (i - 1u)))
                                            {
                                                LIN_1_srfBuffer[i] = LIN_1_txTlDataPointer[(i +
                                                    LIN_1_txTlDataCount) - 2u];
                                            }
                                            else
                                            {
                                                /* Fill unused data bytes with FFs */
                                                LIN_1_srfBuffer[i] = 0xFFu;
                                            }
                                        }
                                        /* Update the user buffer pointer */
                                        LIN_1_txTlDataCount += LIN_1_FRAME_DATA_SIZE_6;
                                    #endif /* LIN_1_COOKED_TL_API */

                                        /* Update length pointer properly */
                                        if (LIN_1_txMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                                        {
                                            LIN_1_txMessageLength -= LIN_1_FRAME_DATA_SIZE_6;
                                        }
                                        else
                                        {
                                            LIN_1_txMessageLength = 0u;
                                        }
                                    }
                                    /* Update the frame counter */
                                    if (LIN_1_txFrameCounter != 15u)
                                    {
                                        LIN_1_txFrameCounter++;
                                    }
                                    else
                                    {
                                        /* If frame counter is larger than 15, reset it */
                                        LIN_1_txFrameCounter = 0u;
                                    }
                                    /* Frame equals 8 bytes */
                                    LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;
                                    /* Set frame data pointer to a start of a frame buffer */
                                    LIN_1_frameData = LIN_1_srfBuffer;
                                    
                                    /* Send first byte to the LIN Master */
                                    LIN_1_tmpData = *LIN_1_frameData;
                                    LIN_1_frameData++;
                                    LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                                    LIN_1_bytesTransferred = 1u;
                                    
                                    /* Switch to the publish data state. */
                                    LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;
                                    
                                    /* One or more data bytes are received */
                                    LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                                    
                                    /* Indicates to transport layer that Slave is transmitting the frame */
                                    LIN_1_tlFlags |= LIN_1_TL_TX_DIRECTION;

                                    /* Checks if the SRF is Pending response frame */
                                    if ((LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] == 0x03u) &&
                                        (LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] == 0x7Fu) &&
                                        (LIN_1_srfBuffer[LIN_1_PDU_D2_IDX ] == 0x78u)   )
                                    {
                                        LIN_1_status |= LIN_1_STATUS_RESPONSE_PENDING;
                                    }
                                    else
                                    {
                                        LIN_1_status &= (l_u8) ~LIN_1_STATUS_RESPONSE_PENDING;
                                    }
                                }
                            }
                            else
                            {
                                /* Reset UART State Machine */
                                LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                            }
                        }
                    } /* If SRF */
                #else
                    /* These are invalid PIDs when TL is disabled - reset UART state machine */
                
                    /* Set response error */
                    #if (LIN_1_SAE_J2602_ENABLED)
                        /* Set ERR2, ERR1 and ERR0 bits */
                        LIN_1_j2602Status_Set(LIN_1_J2602_STS_PARITY_ERR);
                        LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                        
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                    #endif /* LIN_1_SAE_J2602_ENABLED */
                    
                    /* Reset UART State Machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                #endif /* LIN_1_TL_USED */
                }
                else    /* Not MRF or SRF */
                {
                    /* Gets PID index in LIN_1_pidInfoTable */
                    LIN_1_pidIndex = LIN_1_FindPidIndex(LIN_1_framePid);
                    if (LIN_1_INVALID_FRAME_PID != LIN_1_pidIndex)
                    {
                    /* Valid ID */
                    #if (0u == LIN_1_LIN_1_3)
                        /* Starts enhanced checksum calculation  */
                        LIN_1_interimChecksum = LIN_1_framePid;
                    #endif /* (0u == LIN_1_LIN_1_3) */

                        /* Gets the size of the frame */
                        LIN_1_frameSize = LIN_1_pidInfoTable[LIN_1_pidIndex].param &
                                                        LIN_1_FRAME_DATA_SIZE_MASK;
                        /* TX response (publish action) is requested by Master */
                        if (0u != (LIN_1_FRAME_DIR_PUBLISH &
                               LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                        {
                        /* SAE J2602 is disabled */
                        #if (!LIN_1_SAE_J2602_ENABLED)
                            /* This frame is event-triggered */
                            if (0u  != (LIN_1_FRAME_TYPE_EVENT &
                                       LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                            {
                                /* Checks whether to process event-triggered frame */
                                if (0u == LIN_1_GetEtFlagValue(LIN_1_pidIndex))
                                {
                                   /* Resets UART State Machine */
                                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                                   return;
                                }
                            }
                        #endif /* (!LIN_1_SAE_J2602_ENABLED) */

                            /* Gets the pointer to frame data */
                            LIN_1_frameData =
                                LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr;
                            /* Sends the first byte to LIN master */
                            LIN_1_tmpData = *LIN_1_frameData;

                        /* SAE J2602 is disabled */
                        #if (!LIN_1_SAE_J2602_ENABLED)
                            /* This frame is event-triggered */
                            if (0u != (LIN_1_FRAME_TYPE_EVENT &
                                       LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                            {
                                /* Sets the first byte equal to associated unconditional frame's PID. */
                                LIN_1_tmpData = LIN_1_parityTable[
                                    LIN_1_pidInfoTable[LIN_1_pidIndex].associatedPID];
                            }
                            else
                            {
                                LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                            }
                        #endif /* (!LIN_1_SAE_J2602_ENABLED) */

                            LIN_1_frameData++;
                            LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                            LIN_1_bytesTransferred = 1u;
                            
                            /* Switches to publish data state. */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;
                            /* Sets DATA RECEIVE flag */
                            LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                        }
                        else    /* RX response (subscribe action) is requested by Master */
                        {
                            LIN_1_bytesTransferred = 0u;
                            /* Gets the pointer to the temp RX frame data buffer */
                            LIN_1_frameData = LIN_1_tmpRxFrameData;
                            /* Switches to subscribe data state. */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_3_RX;
                        }
                    }
                    else    /* Invalid ID */
                    {
                        /* Resets UART State Machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    }
                }
            }
        break;
        /***********************************************************************
        *                       TX response (Publish)
        * State description:
        *  - Transmits data to LIN Master.
        *  - Transmits the next data byte if there have been no any errors.
        *  - Transmits checksum when data is send correctly.
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_2_TX:
            /* Sets the response active flag */
            LIN_1_status |= LIN_1_STATUS_RESPONSE_ACTIVE;
            
            /* Previously transmitted and read back bytes are not equal */
            if ((LIN_1_tmpData != (l_u8)LIN_1_SCB_SpiUartReadRxData()) ||
                (0u != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags)))
            {
            /* Mismatch Error */
            #if (LIN_1_SAE_J2602_ENABLED)
                /* Sets ERR2 bit */
                LIN_1_j2602Status_Set(LIN_1_J2602_STS_DATA_ERR);
                /* Readback error - sets the response error flag */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            #else
                /* Skips the event-triggered frame */
                if (0u  == (LIN_1_FRAME_TYPE_EVENT &
                            LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                {
                    /* Readback error - sets the response error flag */
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                }
            #endif  /* LIN_1_SAE_J2602_ENABLED */

                /* Checks for framing error */
                if (0u == (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Saves the last processed on bus PID to status variable. */
                    LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                    LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                    #if (LIN_1_SAE_J2602_ENABLED)
                        if (LIN_1_framePid == LIN_1_FRAME_PID_MRF_J2602)
                        {
                            LIN_1_auxStatus |= LIN_1_AUXSTS_MRF_WAS_3E;
                        }
                        else
                        {
                            LIN_1_auxStatus &= (l_u8)~LIN_1_AUXSTS_MRF_WAS_3E;
                        }
                    #endif  /* LIN_1_SAE_J2602_ENABLED */
                }
                /* End frame with response error */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else    /* If readback is successful, continue transmitting */
            {
                /* Adds the transmitted byte to interim checksum. */
                LIN_1_interimChecksum += LIN_1_tmpData;
                if (LIN_1_interimChecksum >= 256u)
                {
                    LIN_1_interimChecksum -= 255u;
                }
                /* Checks to see if all data bytes are sent. */
                if (LIN_1_frameSize > LIN_1_bytesTransferred)
                {
                    /* Sends out the next byte of the buffer. */
                    LIN_1_tmpData = *LIN_1_frameData;
                    LIN_1_frameData++;
                    LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                    LIN_1_bytesTransferred++;
                }
                else    /* All data bytes are sent - compute and transmit the checksum. */
                {
                    /* Computes and sends out the checksum byte */
                    LIN_1_SCB_SpiUartWriteTxData((((l_u8)LIN_1_interimChecksum) ^ (uint32)0xFFu));
                    LIN_1_bytesTransferred = 0u;
                    /* Switches to the checksum state */
                    LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_4_CHS;
                }
            }
        break;
        /***********************************************************************
        *                       RX response (Subscribe)
        * State description:
        *  - Receives data from LIN Master.
        *  - Received data saved to the temporary buffer.
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_3_RX:
            /* Sets Response error only if the error appears during receive */
            if (0u  != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags))
            {
                /* This is a workaround to suppress Response Error when only Break header + PID are received. */
                if ((0u == LIN_1_bytesTransferred) && (0u == LIN_1_frameErrorAfterPID))
                {
                    /* Reads the erroneous byte from FIFO. */
                    (void)LIN_1_SCB_SpiUartReadRxData();
                    /* Bypasses the first Framing error after PID */
                    LIN_1_frameErrorAfterPID = 1u;
                }
                else
                {
                    /* Sets a response error */
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                    
                #if (LIN_1_SAE_J2602_ENABLED)
                    /* Sets framing error bits */
                   LIN_1_j2602Status_Set(LIN_1_J2602_STS_FRAMING_ERR);
                #endif /* LIN_1_SAE_J2602_ENABLED */

                   
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                   /* Clear FE  flag and DATA_RECEIVE flags anyway */
                   LIN_1_fsmFlags &= (l_u8)(~LIN_1_FSM_DATA_RECEIVE);
                    /* Finishes frame processing */
                    LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
                }
                /* Clear FE  flag and DATA_RECEIVE flags anyway */
                LIN_1_fsmFlags &= (l_u8)(~LIN_1_FSM_FRAMING_ERROR_FLAG);
            }
            else
            {
                /* Saves received byte */
                LIN_1_tmpData = (l_u8)LIN_1_SCB_SpiUartReadRxData();
                *LIN_1_frameData = LIN_1_tmpData;
                LIN_1_frameData++;
                LIN_1_bytesTransferred++;
                
                /* Sets the response active flag */
                LIN_1_status |= LIN_1_STATUS_RESPONSE_ACTIVE;
                
                /* One or more data bytes are received */
                LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                
                /* Adds the received byte to  theinterim checksum. */
                LIN_1_interimChecksum += LIN_1_tmpData;
                
                if (LIN_1_interimChecksum >= 256u)
                {
                    LIN_1_interimChecksum -= 255u;
                }
                /* Checks if the data section completed. */
                if (LIN_1_frameSize > LIN_1_bytesTransferred)
                {
                    /* There is data to be received. */
                }
                else
                {
                    /* There is no data to be received. */
                    LIN_1_bytesTransferred = 0u;
                    /* Switches to the checksum state. */
                    LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_4_CHS;
                }
            }
        break;
        /***********************************************************************
        *                              Checksum
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_4_CHS:
            /* Previously transmitted and read back bytes are not equal */
            if (((((l_u8) LIN_1_interimChecksum) ^ 0xFFu) != (l_u8) LIN_1_SCB_SpiUartReadRxData())
                || (0u != (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG)))
            {
                /* Mismatch or Checksum Error */
                /* Sets a response error */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;
                
            #if (LIN_1_SAE_J2602_ENABLED)
                /* Sets the ERR2 and ERR0 bits */
                LIN_1_j2602Status_Set(LIN_1_J2602_STS_CHECKSUM_ERR);
            #endif /* LIN_1_SAE_J2602_ENABLED */

                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] |= 
                            LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                
                

                /* Checks for a framing error */
                if (0u == (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Saves the last processed on bus PID to the status variable. */
                    LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                    LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                }
                /* Resets the UART state machine with a checksum or mismatch error. */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else
            {
            /* Clears all error bits in the interface status. */
            #if (LIN_1_SAE_J2602_ENABLED)
                /* J2602 errors are retained until they are successfully reported 
                 * to the Master node in a message without any detected errors. 
                 * If frame containing J2602_Status was sent (not 3C frames)
                 * clears last reported error state.
                 */
                 if (((LIN_1_framePid & LIN_1_PID_PARITY_MASK) < 
                    (LIN_1_FRAME_PID_MRF & LIN_1_PID_PARITY_MASK)) || 
                    ((0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS)) &&
                    (LIN_1_framePid == LIN_1_FRAME_PID_SRF)))
                 {
                    LIN_1_j2602Status_Clear();    /* clears only last reported error state */
                 }
            #endif  /* LIN_1_SAE_J2602_ENABLED */

                /* Clears the framing error and data receive flags */
                LIN_1_fsmFlags &=
                   ((l_u8) ~(LIN_1_FSM_FRAMING_ERROR_FLAG | LIN_1_FSM_DATA_RECEIVE));
                /* Sets the successful transfer interface flag */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_SUCCESSFUL_TRANSFER;
                /* Saves the last processed on bus PID to the status variable */
                LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                /* Sets the overrun interface flag */
                if (0u != (LIN_1_FSM_OVERRUN & LIN_1_fsmFlags))
                {
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_OVERRUN;
                }
                /* Sets the Overrun flag */
                LIN_1_fsmFlags |= LIN_1_FSM_OVERRUN;
                
                /* Clears response error signal if the frame contains RESPONSE ERROR signal */
            #if (1u == LIN_1_RESPONSE_ERROR_SIGNAL)
                if (LIN_1_RESPONSE_ERROR_FRAME_INDEX == LIN_1_pidIndex)
                {
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_MFS_LIN_1[LIN_1_MFS_RsErr_MFS_LIN_1_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LIN_1_MFS_RsErr_MFS_LIN_1_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                }
            #endif /* (1u == LIN_1_RESPONSE_ERROR_SIGNAL) */

                if ( (!LIN_1_TL_USED) || (LIN_1_TL_USED && 
                     (!((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                        (LIN_1_FRAME_PID_SRF == LIN_1_framePid)))) )
                {
                /* SAE J2602 is disabled */
                #if (!LIN_1_SAE_J2602_ENABLED)
                    /* This frame is event-triggered */
                    if (0u != (LIN_1_FRAME_TYPE_EVENT &
                               LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                    {
                        /* Clears event-triggered flags */
                        LIN_1_ClearEtFlagValue(LIN_1_pidIndex);
                        /* Resets UART State Machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    }
                #endif /* (!LIN_1_SAE_J2602_ENABLED) */

                    /* Sets associated with current frame flags */
                    LIN_1_SetAssociatedFlags(LIN_1_pidIndex);
                }

            #if (LIN_1_TL_USED)
                /* Checks if received data is a "master request frame" */
                #if (LIN_1_SAE_J2602_ENABLED)
                if ((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                    (LIN_1_FRAME_PID_MRF_J2602 == LIN_1_framePid))
                #else
                if (LIN_1_FRAME_PID_MRF == LIN_1_framePid)
                #endif /* LIN_1_SAE_J2602_ENABLED */
                {                    
                    l_bool LIN_1_NAD_accepted;     /* Received NAD accepted */
                    /* Check NAD */
                    LIN_1_NAD_accepted = (l_bool)((LIN_1_nad == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]) ||
                                                             (LIN_1_NAD_BROADCAST == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]) ||
                                                             (LIN_1_LinSlaveConfig.initialNad == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]));                     

                #if (LIN_1_SAE_J2602_ENABLED)                    
                    l_bool LIN_1_is_own_DNN;       /* Received DNN accepted */
                    /* Check DNN */
                    LIN_1_is_own_DNN = (l_bool)((0x0Fu & LIN_1_nad) == 
                        (0x0Fu & LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]));
                #endif /* LIN_1_SAE_J2602_ENABLED */
                    
                    if ((LIN_1_NAD_GO_TO_SLEEP == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
                        #if (LIN_1_SAE_J2602_ENABLED)
                            /* For J2602 0x00 is a valid NAD for 0x3E requests, don't go to sleep */
                            && (LIN_1_FRAME_PID_MRF == LIN_1_framePid)
                       #endif /* LIN_1_SAE_J2602_ENABLED */
                       )
                    {
                        LIN_1_ifcStatus |= LIN_1_IFC_STS_GO_TO_SLEEP;
                    } 
    
                #if (LIN_1_SAE_J2602_ENABLED)
                    /* J2602 allows additional NADs above 0x7F, where lower nibble equals DNN */ 
                    else if ((0u != LIN_1_NAD_accepted)       ||
                         ((0u != LIN_1_is_own_DNN) &&
                         (LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX] > LIN_1_NAD_BROADCAST) &&
                            (LIN_1_FRAME_PID_MRF == LIN_1_framePid))      ||
                         ((0u != LIN_1_is_own_DNN) &&
                            (LIN_1_FRAME_PID_MRF_J2602 == LIN_1_framePid)))
                #else
                    else if (0u != LIN_1_NAD_accepted)                        
                #endif /* LIN_1_SAE_J2602_ENABLED */
                    {
                        LIN_1_CheckNodeState(LIN_1_MRF_OWN_NAD);
                    }
                    else if (LIN_1_NAD_FUNCTIONAL == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
                    {
                        LIN_1_CheckNodeState(LIN_1_MRF_FUNC_NAD);
                    }
                    else
                    {
                        LIN_1_CheckNodeState(LIN_1_MRF_ALIEN_NAD);
                    }
                    /* Clears TL RX direction flag */
                    LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_RX_DIRECTION);
                    /* Resets UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                }
                else if (LIN_1_FRAME_PID_SRF == LIN_1_framePid)
                {
                    LIN_1_CheckNodeState(LIN_1_SRF);
                    /* Resets UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                }
                else
                {
                    /* RX response (subscribe action) is requested by Master */
                    if (0u == (LIN_1_FRAME_DIR_PUBLISH &
                           LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                    {
                    interruptState = CyEnterCriticalSection();
                        /* Copoies received data from temporary buffer to frame buffer */
                        for (i = 0u; i < LIN_1_frameSize; i++)
                        {
                            *(LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr + i) =
                                LIN_1_tmpRxFrameData[i];
                        }
                    CyExitCriticalSection(interruptState);
                    }
                    /* Resets UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                }
            #else
                /* RX response (subscribe action) is requested by Master */
                if (0u == (LIN_1_FRAME_DIR_PUBLISH &
                           LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                {
                interruptState = CyEnterCriticalSection();
                    /* Copies received data from temporary buffer to frame buffer */
                    for (i = 0u; i < LIN_1_frameSize; i++)
                    {
                        *(LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr + i) =
                            LIN_1_tmpRxFrameData[i];
                    }
                CyExitCriticalSection(interruptState);
                }

                /* Resets UART state machine */
                LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
            #endif  /* LIN_1_TL_USED */
            }
        break;
        default:
            /* Resets UART state machine */
            LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
        break;
    }

    /* Clears SCB UART interrupt */
    LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_NOT_EMPTY;

    #ifdef LIN_1_L_IFC_RX_CALLBACK
        l_ifc_rx_LIN_1_Callback();
    #endif /* LIN_1_L_IFC_RX_CALLBACK */
}


/******************************************************************************
* Function Name: l_ifc_tx
*******************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
******************************************************************************/
void l_ifc_tx_LIN_1(void)
{
    l_ifc_rx_LIN_1();
}


/******************************************************************************
* Function Name: l_ifc_aux
*******************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
******************************************************************************/
void l_ifc_aux_LIN_1(void)
{
    /**************************************************************************
    *                       Bus Inactivity Interrupt Detected
    **************************************************************************/
    #if (1u == LIN_1_INACTIVITY_ENABLED)
            #if (LIN_1_TL_USED)
                /* If timeout is enabled, proceeds the timeout manage. */
                if (0u !=(LIN_1_tlFlags & LIN_1_TL_N_AS_TIMEOUT_ON))
                {
                    /* Increments the timeout. */
                    LIN_1_tlTimeoutCnt++;

                    if ((LIN_1_TL_N_AS_TIMEOUT_VALUE) <= LIN_1_tlTimeoutCnt)
                    {
                        LIN_1_CheckNodeState(LIN_1_TX_TIMEOUT);
                    }
                }
                else if (0u !=(LIN_1_tlFlags & LIN_1_TL_N_CR_TIMEOUT_ON))
                {
                    /* Increments the timeout */
                    LIN_1_tlTimeoutCnt++;

                    if ((LIN_1_TL_N_CR_TIMEOUT_VALUE) <= LIN_1_tlTimeoutCnt)
                    {
                        LIN_1_CheckNodeState(LIN_1_RX_TIMEOUT);
                    }
                }
                else
                {
                    /* Resets the timeout counter */
                    LIN_1_tlTimeoutCnt = 0u;
                }
            #endif /* LIN_1_TL_USED */

            if ((LIN_1_INACTIVITY_THRESHOLD_IN_1_MS) <= LIN_1_periodCounter)
            {
                /* Inactivity threshold is achieved. */

                /* Sets the bus inactivity ioctl status bit */
                LIN_1_ioctlStatus |= LIN_1_IOCTL_STS_BUS_INACTIVITY;
            }
            else
            {
                LIN_1_periodCounter++;
            }
    #endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */
        
    /***************************************************************************
    *                       Brake to Sync Timeout Tick
    ***************************************************************************/
    #if (LIN_1_SAE_J2602_ENABLED && (0u == LIN_1_AUTO_BAUD_RATE_SYNC))
    /* Counts time from the break detect interrupt to latest time of the SYNC-PID sequence. */
    if (0u != LIN_1_breakToSyncCounts) /* BREAK to SYNC timer counts if >0 */
    {
        LIN_1_breakToSyncCounts++; /* Increments time from the break detect interrupt. */

        /* If Break to Sync timeout is reached, checks the baud detect register two times. */
        if (LIN_1_breakToSyncCounts >= LIN_1_BREAK_TO_SYNC_TIMEOUT)
        {
            if (LIN_1_breakToSyncCounts == LIN_1_BREAK_TO_SYNC_TIMEOUT)
            {
                /* Saves the first sample of the baud detect register */
                LIN_1_breakToSyncCounts1 = (l_u16)(LIN_1_SCB_UART_RX_STATUS_REG &
                LIN_1_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
            }
            if (LIN_1_breakToSyncCounts == (LIN_1_BREAK_TO_SYNC_TIMEOUT+1u))
            {
                /* Saves the second sample of the baud detect register after one ms */
                LIN_1_breakToSyncCounts2 = (l_u16)(LIN_1_SCB_UART_RX_STATUS_REG &
                LIN_1_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
                
                if (LIN_1_breakToSyncCounts2 > LIN_1_breakToSyncCounts1)
                /* Baud detection failed. Baud detect counter is still running after a timeout. */
                {
                    LIN_1_j2602Status_Set(LIN_1_J2602_STS_DATA_ERR);                
                }
                LIN_1_breakToSyncCounts = 0u; /* stop the BREAK to SYNC timer */
            } 
        }
    }
    #endif /* LIN_1_SAE_J2602_ENABLED && (0u == LIN_1_AUTO_BAUD_RATE_SYNC)*/

    #ifdef LIN_1_L_IFC_AUX_CALLBACK
        l_ifc_aux_LIN_1_Callback();
    #endif /* LIN_1_L_IFC_AUX_CALLBACK */
}


/******************************************************************************
* Function Name: l_ifc_read_status_LIN_1
*******************************************************************************
*
* Summary:
*  This function is defined by the LIN specification. This returns the status of
*  the specified LIN interface and clears all status bits for that
*  interface. See Section 7.2.5.8 of the LIN 2.1 specification.
*
* Return:
*  The status bits of the specified LIN interface are returned. These bits have
*  the following meanings:
*    [15:8]    Last Received PID
*    [7]        0
*    [6]        Save Configuration flag
*    [5]        0
*    [4]        Bus Activity flag
*    [3]        Go To Sleep flag
*    [2]        Overrun flag
*    [1]        Successful Transfer flag
*    [0]        Error in Response flag
*
******************************************************************************/
l_u16 l_ifc_read_status_LIN_1(void)
{
    l_u16 returnValue;
    l_u8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Copies the global status variable to local temp variable. */
    returnValue = LIN_1_ifcStatus;

    /* Clears the status variable */
    LIN_1_ifcStatus &= (l_u16) ~LIN_1_IFC_STS_MASK;

    CyExitCriticalSection(interruptState);
    
    /* Returns the status in temp variable */
    return (returnValue);
}


/******************************************************************************
* Function Name: LIN_1_FindPidIndex
*******************************************************************************
*
* Summary:
*  Returns the PID index in LIN_1_volatileConfig.
*
* Parameters:
*  l_u8 pid - PID of the frame whose index is required.
*
* Return:
*  The index if PID in LIN_1_volatileConfig.
*  0xFFu - If PID is not found.
*
*******************************************************************************/
static l_u8 LIN_1_FindPidIndex(l_u8 pid)
{
    l_u8 i;
    l_u8 returnValue = LIN_1_INVALID_FRAME_PID;

    for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
    {
        if (pid == LIN_1_volatileConfig[i])
        {
            returnValue = i;
            /* Break the loop */
            break;
        }
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: LIN_1_EndFrame
********************************************************************************
*
* Summary:
*  Finishes the frame transmission.
*
*******************************************************************************/
static void LIN_1_EndFrame(l_u8 status)
{
    l_u8 interruptState;

    /* Clears the data received flag */
    LIN_1_fsmFlags &= ((l_u8) ~LIN_1_FSM_DATA_RECEIVE);
    
#if (LIN_1_TL_USED)
    /* Clears the TL TX direction flag */
    LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_TX_DIRECTION);
#endif /* LIN_1_TL_USED */

    if (status == LIN_1_HANDLING_DONT_SAVE_PID)
    {
        /* Clears the data received flag */
        LIN_1_fsmFlags &= ((l_u8) ~LIN_1_FSM_DATA_RECEIVE);
        if (0u != (LIN_1_fsmFlags & LIN_1_FSM_OVERRUN))
        {
            /* Sets overrun */
            LIN_1_ifcStatus |= LIN_1_IFC_STS_OVERRUN;
        }
    }
    if (status == LIN_1_HANDLING_SKIP_OVERRUN)
    {
        /* Sets the overrun flag */
        LIN_1_fsmFlags |= LIN_1_FSM_OVERRUN;
        
    #if (LIN_1_TL_USED)
        if (0u != (LIN_1_tlFlags & LIN_1_TL_TX_DIRECTION))
        {
        /* Transport Layer Functions: Cooked Transport Layer API */
        #if (LIN_1_COOKED_TL_API)
            /* Sets TL TX error status */
            LIN_1_txTlStatus = LD_FAILED;
        #else
            /* Sets TL TX error status */
            LIN_1_txTlStatus = LD_TRANSMIT_ERROR;
        #endif /* LIN_1_COOKED_TL_API */

            /* Clears TL flags register */
            LIN_1_tlFlags = 0u;
        }

        if (0u != (LIN_1_tlFlags & LIN_1_TL_RX_DIRECTION))
        {
        #if (LIN_1_COOKED_TL_API)
            /* Sets TL RX error status */
            LIN_1_rxTlStatus = LD_FAILED;
        #else
            /* Sets TL RX error status */
            LIN_1_rxTlStatus = LD_RECEIVE_ERROR;
        #endif /* LIN_1_COOKED_TL_API */

            /* Clears TL flags register */
            LIN_1_tlFlags = 0u;
        }
    #endif /* LIN_1_TL_USED */
    }
    else if ((status == LIN_1_HANDLING_RESET_FSM_ERR) || (status == LIN_1_HANDLING_DONT_SAVE_PID))
    {
        /* Clears UART enable flag */
        LIN_1_fsmFlags &= ((l_u8) ~(LIN_1_FSM_UART_ENABLE_FLAG |
                        LIN_1_FSM_FRAMING_ERROR_FLAG));

    interruptState = CyEnterCriticalSection();
    
        /* Clears RX and TX FIFOs after Frame or Overrun error */
        LIN_1_SCB_SpiUartClearRxBuffer();
        LIN_1_SCB_SpiUartClearTxBuffer();
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_NOT_EMPTY;
        
        /* Set IDLE FSM State */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_0_IDLE;
        
    CyExitCriticalSection(interruptState);
    }
    else
    {
    /* MISRA requires this else. Rule-14.10 */
    }
}


/*******************************************************************************
* Function Name: LIN_1_SetAssociatedFlags
********************************************************************************
*
* Summary:
*  This function sets appropriate flags. Used by ISR.
*
* Parameters:
*  The PID index in LIN_1_LinSlaveConfig.
*
*******************************************************************************/
static void LIN_1_SetAssociatedFlags(l_u8 pIndex)
{
    switch(pIndex)
    {
        case 0u:
            /* Mark frame "MFS_LIN_1" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_0] |=
                LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_0;
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_1] |=
                LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_1;
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_BYTE_OFFSET_2] |=
                LIN_1_ISR_AUX_SET_FLAGS_MFS_LIN_1_FLAG_MASK_2;
        break;


        default:
        break;    
    }
}


/* SAE J2602 is disabled */
#if (!LIN_1_SAE_J2602_ENABLED)

/*******************************************************************************
* Function Name: LIN_1_GetEtFlagValue
********************************************************************************
*
* Summary:
*  This function gets the value of an appropriate event-triggered frame flag.
*
* Parameters:
*  The PID index in LIN_1_LinSlaveConfig.
*
* Return:
*  Current flag value.
*
*******************************************************************************/
static l_bool LIN_1_GetEtFlagValue(l_u8 pidIndex)
{
    l_bool returnValue;

    switch(pidIndex)
    {
        default:
             returnValue = LIN_1_FALSE;
        break;    
    }

    return (returnValue);
}



/*******************************************************************************
* Function Name: LIN_1_ClearEtFlagValue
********************************************************************************
*
* Summary:
*  This function clears the value of an appropriate event-triggered frame flag.
*
* Parameters:
*  The PID index in LIN_1_LinSlaveConfig.
*
*******************************************************************************/
static void LIN_1_ClearEtFlagValue(l_u8 pidIndex)
{
    switch(pidIndex)
    {
        default:
        break;    
    }
}


#endif /* (!LIN_1_SAE_J2602_ENABLED) */


#if (LIN_1_TL_USED)

/*******************************************************************************
* Function Name: ld_init
********************************************************************************
*
* Summary:
*  This call (re)initializes the raw and cooked layers on interface
*  iii.
*
*  All transport layer buffers will be initialized.
*
*  If there is an ongoing diagnostic frame transporting a cooked or raw message
*  on the bus, it will not be aborted.
*
* Parameters:
*  iii - Interface.
*
*******************************************************************************/
void ld_init_LIN_1(void)
{
    l_u8 interruptState;

    #if (LIN_1_COOKED_TL_API)
        while (0u != (LIN_1_tlFlags & LIN_1_TL_TX_DIRECTION))
        {
            /* Waits until the current message is processed. */
        }

        /* Saves the interrupt state and disable interrupts. */
    interruptState = CyEnterCriticalSection();

        /* Initializes TX and RX status variables correctly */
        LIN_1_txTlStatus = LD_COMPLETED;
        LIN_1_rxTlStatus = LD_COMPLETED;

        LIN_1_tlLengthPointer = NULL;

        /* Resets frame counters */
        LIN_1_rxMessageLength = 0u;
        LIN_1_txMessageLength = 0u;

        /* Resets frame counters */
        LIN_1_txFrameCounter = 0u;
        LIN_1_rxFrameCounter = 0u;

        LIN_1_rxTlDataPointer = NULL;
        LIN_1_rxTlInitDataPointer = NULL;

        /* Previous PCI requires to be unknown after initialization */
        LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
        LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
    #else
        /* Saves interrupt state and disable interrupts */
    interruptState = CyEnterCriticalSection();

        /* Resets buffers depth to 0 to indicate that buffers are empty. */
        LIN_1_txBufDepth = 0u;
        LIN_1_rxBufDepth = 0u;

        /* Raw API buffers initialization */
        LIN_1_txWrIndex = 0u;
        LIN_1_txRdIndex = 0u;

        LIN_1_rxWrIndex = 0u;
        LIN_1_rxRdIndex = 0u;

        LIN_1_txTlStatus = LD_QUEUE_EMPTY;
        LIN_1_rxTlStatus = LD_NO_DATA;
    #endif /* LIN_1_COOKED_TL_API */

    /* Sets initial NAD as the current active NAD before initializing TL */
    LIN_1_nad = LIN_1_LinSlaveConfig.initialNad;

    LIN_1_tlFlags = 0u;

    /* Changes the node state to next state */
   LIN_1_nodeState = LIN_1_IDLE;

    /* Enables interrupts */
    CyExitCriticalSection(interruptState);
}


/******************************************************************************
* Function Name: ld_read_configuration
*******************************************************************************
*
* Summary:
*  This function reads the NAD and PID values from the volatile memory.
*  This function reads the current configuration data, and
*  saves this data into the non-volatile (flash) memory. The application should
*  save the configuration data to the flash when the "Save Configuration" bit
*  is set in the LIN status register (returned by 
*  l_ifc_read_status_LIN_1).
*  The configuration data that is read is a series of bytes. The first byte is
*  the current NAD of the slave. The next bytes are the current PID values for
*  the frames that the slave responds to. The PID values are in the order in
*  which the frames appear in the LDF or NCF file.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_READ_OK - If the service is successful.
*
*  LD_LENGTH_TOO_SHORT - If the configuration size is greater than the length.
*                        It means that the data area does not contain valid
*                        configuration.
*
*******************************************************************************/
l_u8 ld_read_configuration_LIN_1(l_u8* pData, l_u8* const length)
{
    l_u8 i;
    l_u8 result = LIN_1_LD_READ_OK;

    if (*length < (LIN_1_NUM_FRAMES + 1u))
    {
        /* Return with no action when requested length is smaller
        *  than configuration data length.
        */
        result = LIN_1_LD_LENGTH_TOO_SHORT;
    }
    else
    {
        /* Copy over configured NAD */
        pData[0u] = LIN_1_nad;

        /* Copy data from PID array to data array */
        for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
        {
            pData[i + 1u] = LIN_1_volatileConfig[i];
        }

        /* Set length parameter to actual length of configuration data */
        *length = LIN_1_NUM_FRAMES + 1u;
    }

    /* Return status */
    return (result);
}


/*******************************************************************************
* Function Name: ld_set_configuration
********************************************************************************
*
* Summary:
*  This call does not transport anything to the bus.
*
*  The function configures NAD and PIDs accordingly to the
*  configuration given by data. The intended usage is to restore the saved
*  configuration or set initial configuration (e.g. coded by I/O pins).
*
*  The function is called after calling l_ifc_init.
*
*  The caller must set the size of the data area before calling the function.
*
*  The data contains NAD and PIDs each occupying one byte.
*  The data structure is: NAD and all PIDs for the frames.
*  The PIDs order is the same as the frame list in LDF,
*  Section 9.2.2.2, and NCF, Section 8.2.5.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_SET_OK - If the service is successful.
*
*  LD_LENGTH_NOT_CORRECT - If the required size of the configuration is not
*                          equal to the given length.
*
*  LD_DATA_ERROR - The set of configuration could not be set. An error
*                  occurred while setting the configuration and the read back
*                  configuration settings don't match the required settings.
*
*  LIN_1_volatileConfig - New frame PIDs are copied.
*
*******************************************************************************/
l_u8 ld_set_configuration_LIN_1(const l_u8* const pData, l_u16 length)
{
    l_u8 i;
    l_u8 result = LIN_1_LD_SET_OK;

    if (length != (LIN_1_NUM_FRAMES + 1u))
    {
        /* Returns an error if the length isn't correct. */
        result = LIN_1_LD_LENGTH_NOT_CORRECT;
    }
    else
    {
        /* Copies NAD to volatile memory */
        LIN_1_nad = pData[0u];

        /* Data read back */
        if (LIN_1_nad != pData[0u])
        {
            /* Indicates a data error if NAD is not set correctly. */
            result = LIN_1_LD_DATA_ERROR;
        }

        /* Copies Frame PIDs to volatile memory */
        for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
        {
            LIN_1_volatileConfig[i] = pData[i + 1u];

            /* Data read back */
            if (LIN_1_volatileConfig[i] != pData[i + 1u])
            {
                /* Indicates a data error if NAD is not set correctly. */
                result = LIN_1_LD_DATA_ERROR;
            }
        }
    }

    /* Returns success code if the copy is completed. */
    return(result);
}


#if (LIN_1_SAE_J2602_ENABLED)
    /*******************************************************************************
    * Function Name: LIN_1_UpdateMessageIDs
    ********************************************************************************
    *  [J2602-1 5.7.2.2 Message ID Assignment]
    * Summary:
    *  Updates message IDs and frame PIDs according to NAD.
    *  Messages with IDs 0x38, 0x39, 0x3A and 0x3B are considered broadcast messaes
    *  and are not updated.
    *
    * Parameters:
    *  l_u8 new NAD.
    *
    * Return:
    *  CYRET_SUCCESS      - On success.
    *  CYRET_BAD_PARAM    - On an error.
    *
    *******************************************************************************/
    static l_u8 LIN_1_UpdateMessageIDs(l_u8 newNAD)
    {
        l_u8 i = 0u;
        l_u8 returnValue = CYRET_SUCCESS;
        /* Message IDs 0x38, 0x39, 0x3A and 0x3B are not configurable. 
         * Excluded from frames count later.*/
        l_u8 configurable_frames = LIN_1_NUM_FRAMES;
        /* Checks if NAD is in range 0x60-0x6F */
        if ((newNAD & (l_u8)(~LIN_1_NAD_DNN_MASK)) == LIN_1_NAD_J2602_BASE)
        {             
            /* Checks for NADs 0x6E and 0x6F, which have no assigned frame IDs. */
            if ((newNAD == 0x6Eu) || (newNAD == LIN_1_NAD_UNINITIALIZED))
            {            
                for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
                {
                    if ((messageIdTable[i] < LIN_1_FRAME_PID_BROADCAST_MIN) || 
                        (messageIdTable[i] > LIN_1_FRAME_PID_BROADCAST_MAX))
                    {
                        /* Write message ID table (these are unprotected IDs) */                
                        messageIdTable[i] = LIN_1_MESSAGE_ID_UNASSIGNED;
                        /* Write frame PIDs table (protected IDs are found in the lookup table)*/
                        LIN_1_volatileConfig[i] = LIN_1_FRAME_PID_UNASSIGNED;
                    }
                }
                returnValue = CYRET_SUCCESS;                
            }
            else
            {            
                /* Excludes messages with IDs 0x38, 0x39, 0x3A and 0x3B from frame count. */
                for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
                {
                    if ((messageIdTable[i] >= LIN_1_FRAME_PID_BROADCAST_MIN) && 
                        (messageIdTable[i] <= LIN_1_FRAME_PID_BROADCAST_MAX))
                    {
                        configurable_frames--;
                    }
                }
                /* Checks for valid NAD according to configurable messages count 
                   (J2602-1 5.7.2.2 Message ID Assignment). */
                if (configurable_frames > 16u) /* 17 to 32 messages*/
                {
                    if (newNAD != LIN_1_NAD_J2602_BASE)
                    {
                        /* NAD is not 0x60 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }                
                else if (configurable_frames > 8u) /* 9 to 16 messages*/
                {
                    if ((0u != (newNAD & LIN_1_NAD_MULTIPLE4_MASK)) ||
                        (newNAD > 0x68u))
                    {
                        /* NAD is not multiple of 4 or NAD is above 0x68 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }
                else if (configurable_frames > 4u)  /* 5 to 8 messages */
                {
                    if ((0u != (newNAD & LIN_1_NAD_MULTIPLE2_MASK)) ||
                        (newNAD == 0x6Du))
                    {
                        /* NAD is not multiple of 2 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }
                else
                {
                    /* MISRA requires this else */
                }
                if (returnValue == CYRET_SUCCESS)
                {
                    /* Calculates new messages IDs based on newNAD*/
                    for (i = 0u; i < LIN_1_NUM_FRAMES; i++)
                    {
                        if ((messageIdTable[i] >= LIN_1_FRAME_PID_BROADCAST_MIN) && 
                            (messageIdTable[i] <= LIN_1_FRAME_PID_BROADCAST_MAX))
                        {
                            /* Stops if broadcast frames are reached. */
                            break; 
                        }
                        /* Write message ID table (these are unprotected IDs) */                
                        messageIdTable[i] = ((4u * ((l_u16)newNAD & LIN_1_NAD_DNN_MASK)) + i);
                        
                        /* Write frame PIDs table (protected IDs are found in lookup table) */
                        LIN_1_volatileConfig[i] = LIN_1_parityTable[messageIdTable[i] &
                                                                LIN_1_PID_PARITY_MASK];
                    }
                }
            }
        }
        else
        {
            /* NAD is outside the allowed range */
            returnValue = CYRET_BAD_PARAM;
        }
        return (returnValue);
    }
#endif  /* LIN_1_SAE_J2602_ENABLED */

#if (LIN_1_CFG_SERVICES_ENABLED)
    #if ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) || \
         (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))
        /*******************************************************************************
        * Function Name: LIN_1_LinProductId
        ********************************************************************************
        *
        * Summary:
        *  Verify that the received LIN product identification matches.
        *
        * Parameters:
        *  frameData - The pointer to the 4 bytes that hold LIN product ID.
        *
        * Return:
        *  0 - If the LIN product IDs doesn't match.
        *  1 - If the LIN product IDs match.
        *
        *******************************************************************************/
        static l_bool LIN_1_LinProductId(volatile const l_u8 frameData[])
        {
            l_bool i = LIN_1_TRUE;

            if ((frameData[0u] != LO8(LIN_1_slaveId.supplierId)) &&
                (frameData[0u] != LO8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
            {
                i = LIN_1_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[1u] != HI8(LIN_1_slaveId.supplierId)) &&
                (frameData[1u] != HI8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
            {
                i = LIN_1_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[2u] != LO8(LIN_1_slaveId.functionId)) &&
                (frameData[2u] != LO8(LIN_1_CS_FUNCTION_ID_WILDCARD)))
            {
                i = LIN_1_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[3u] != HI8(LIN_1_slaveId.functionId)) &&
                (frameData[3u] != HI8(LIN_1_CS_FUNCTION_ID_WILDCARD)))
            {
                i = LIN_1_FALSE;        /* Data isn't for this slave */
            }

            return (i);
        }
    #endif /* ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) ||
           *  (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))
           */
           
           
    #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))
        #if (1u == LIN_1_LIN_2_0)
            /*******************************************************************************
            * Function Name: LIN_1_MessageId
            ********************************************************************************
            *
            * Summary:
            *  Looks for the message ID in the LIN message ID table.In case of success
            *  returns message ID index in the table.
            *
            * Parameters:
            *  frameData - The data pointer points to a data area with 2 bytes.
            *
            * Return:
            *  message ID index - If operation is successful.
            *  LD_INVALID_MESSAGE_INDEX - If the message ID hasn't been found.
            *
            *******************************************************************************/
            static l_u8 LIN_1_MessageId(volatile const l_u8* frameData)
            {
                l_u8 i = 0u;
                l_u8 result = LD_INVALID_MESSAGE_INDEX;

                while ((i < LIN_1_NUM_FRAMES) && (result == LD_INVALID_MESSAGE_INDEX))
                {
                    /* If LSB of the message ID from table is equal to that received,
                     * Compare MSB and in case of success set result to message index.
                     */
                    if (frameData[0u] == LO8(messageIdTable[i]))
                    {
                        if (frameData[1u] == HI8(messageIdTable[i]))
                        {
                            result = i;
                        }
                    }
                    i++;
                }
                return (result);
            }
        #endif /* (1u == LIN_1_LIN_2_0) */
    #endif /*(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL)) */

    #if ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) || \
         (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))
        /*******************************************************************************
        * Function Name: ld_read_by_id_callout
        ********************************************************************************
        *
        * Summary:
        *  This function calls out when the master node transmits a "read" by identifier
        *  request with the identifier in the user defined area. The slave node
        *  application is called from the driver when such a request is received.
        *
        * Parameters:
        *  iii - Interface.
        *  id - The id parameter is the identifier in the user defined area (32 to 63),
        *  from the "read" by identifier configuration request.
        *  frameData - The data pointer points to a data area with 5 bytes. This area
        *  is used by the application to set up a positive response.
        *
        * Return:
        *  LD_NEGATIVE_RESPONSE - The slave node responds with a negative response.
        *  In this case the data area is not considered.
        *
        *  LD_POSTIVE_RESPONSE - The slave node sets up a positive response using
        *  the data provided by the application.
        *
        *  LD_NO_RESPONSE - The slave node does not answer.
        *
        *******************************************************************************/
        l_u8 ld_read_by_id_callout_LIN_1(l_u8 id, l_u8* frameData)
        {
            /* NOTE: frameData will be modified by callback function */
            l_u8 result = LD_NEGATIVE_RESPONSE;

            #ifdef LIN_1_LD_READ_BY_ID_CALLOUT_CALLBACK
                result = ld_read_by_id_callout_LIN_1_Callback(id, frameData);
            #else
                (void) id;
                (void) frameData;
            #endif /* LIN_1_LD_READ_BY_ID_CALLOUT_CALLBACK */

            return (result);
        }
    #endif /* ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) ||
            *  (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))
            */
#endif /* LIN_1_CFG_SERVICES_ENABLED */


/******************************************************************************
* Function Name: CheckNodeState
*******************************************************************************
*
* Summary:
*  This function implements LIN Slave Node state machine as defined in LIN2.2.a,
*  item 5.5 SLAVE NODE TRANSMISSION HANDLER.
*  Its states are:
*       IDLE - initial state;
*       RX_PHY_REQUEST  - processing of incoming segmented message;
*       TX_PHY_RESPONSE - processing of outcoming RESPONSE.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_CR timeout was expired.
*     TX_TIMEOUT     - N_AS timeout was expired.
*
******************************************************************************/
static void LIN_1_CheckNodeState(l_u8 stimulus)
{
    l_u8 return_value;

    switch (LIN_1_nodeState)
    {
        case LIN_1_IDLE:
            switch (stimulus)
            {
                case LIN_1_MRF_ALIEN_NAD:
                case LIN_1_SRF:
                case LIN_1_RX_TIMEOUT:
                case LIN_1_TX_TIMEOUT:
                    LIN_1_tlFlags &= (l_u8) (~(LIN_1_TL_N_CR_TIMEOUT_ON |
                                                LIN_1_TL_N_AS_TIMEOUT_ON ));
                break;
                case LIN_1_MRF_FUNC_NAD:
                    (void)LIN_1_ProcessMrf(LIN_1_mrfBuffer);
                    LIN_1_ClrTxBuffer(stimulus);
                break;
                case LIN_1_MRF_OWN_NAD:
                    /* Receives an incoming TL frame (FF or SF) */
                    LIN_1_ClrTxBuffer(stimulus);

                    return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                    if (LIN_1_RESPONSE_REQUIRED == return_value)
                    {
                        /* Changes the node state to TX state */
                        LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                    }
                    else if (LIN_1_RECEIVE_CONTINUES == return_value)
                    {
                        /* Changes the node state to RX state */
                        LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                    }
                    else
                    {
                        /* Does nothing. */
                    }
                break;
                default:
                    /* Does nothing. */
                break;
            }
        break;
        case LIN_1_RX_PHY_REQUEST:
            switch(stimulus)
            {
                case LIN_1_MRF_ALIEN_NAD:
                case LIN_1_SRF:
                case LIN_1_RX_TIMEOUT:
                case LIN_1_TX_TIMEOUT:
                    LIN_1_ClrTxBuffer(stimulus);
                    LIN_1_ClrRxBuffer(stimulus);
                    LIN_1_nodeState = LIN_1_IDLE;
                break;
                case LIN_1_MRF_FUNC_NAD:
                    /* Misses the functional request */
                break;
                case LIN_1_MRF_OWN_NAD:
                    /* Receives the incoming TL frame (CF) */
                    return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                    if (LIN_1_RESPONSE_REQUIRED == return_value)
                    {
                        /* Changes the node state to TX state */
                        LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                    }
                    else if (LIN_1_RECEIVE_CONTINUES == return_value)
                    {
                        /* Changes the node state to RX state */
                        LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                    }
                    else    /* Does nothing. */
                    {
                        LIN_1_ClrTxBuffer(stimulus);
                        LIN_1_ClrRxBuffer(stimulus);
                        LIN_1_nodeState = LIN_1_IDLE;
                    }
                break;
                default:
                    /* Does nothing. */
                break;
            }
        break;
        case LIN_1_TX_PHY_RESPONSE:
            switch(stimulus)
            {
                case LIN_1_MRF_ALIEN_NAD:
                case LIN_1_RX_TIMEOUT:
                case LIN_1_TX_TIMEOUT:
                    LIN_1_ClrTxBuffer(stimulus);
                    LIN_1_nodeState = LIN_1_IDLE;
                break;
            case LIN_1_MRF_FUNC_NAD:
                /* Does nothing, misses the functional request here. */
            break;
            case LIN_1_SRF:
                /* Transmit RESPONSE message */
                return_value = LIN_1_TransmitTLFrame(stimulus);
                if (LIN_1_TRANSMISSION_CONTINUES != return_value)
                {
                    LIN_1_ClrTxBuffer(stimulus);
                    if (0u == (LIN_1_status & LIN_1_STATUS_RESPONSE_PENDING))
                    {
                        /* Changes the node state to idle state. */
                        LIN_1_nodeState = LIN_1_IDLE;
                    }
                }
            break;
            case LIN_1_MRF_OWN_NAD:
                /* MRF arrives during the RESPONSE transmission , drop
                transmitted message and receive new diagnostic message */

                LIN_1_ClrTxBuffer(stimulus);

                return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                if (LIN_1_RESPONSE_REQUIRED == return_value)
                {
                    /* Changes the node state to TX state. */
                    LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                }
                else if (LIN_1_RECEIVE_CONTINUES == return_value)
                {
                    /* Changes the node state to RX state. */
                    LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                }
                else
                {
                    LIN_1_ClrRxBuffer(stimulus);
                    LIN_1_ClrTxBuffer(stimulus);
                    LIN_1_nodeState = LIN_1_IDLE;
                }
            break;
            default:
                break;
            }
        break;
        default:
            break;
    }
}


/******************************************************************************
* Function Name: LIN_1_TransmitTLFrame
*******************************************************************************
*
* Summary:
*  Transmits frame of segmented message.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_CR timeout expired.
*     TX_TIMEOUT     - N_AS timeout expired.
* Return:
*  NO_RESPONSE_REQUIRED      - The message transmission completed.
*  TRANSMISSION_CONTINUES    - There are more segments to be transmitted.
*
******************************************************************************/
static l_u8 LIN_1_TransmitTLFrame(l_u8 stimulus)
{
    l_u8 return_value = LIN_1_NO_RESPONSE_REQUIRED;

    (void) stimulus;

    /* Clears CR Timeout after answering SRF. */
    LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_N_CR_TIMEOUT_ON);

    if (0u == (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
    {
        if ((0u != LIN_1_txMessageLength) &&
            (0u != (LIN_1_tlFlags & LIN_1_TL_TX_REQUESTED)))
        {
            return_value = LIN_1_TRANSMISSION_CONTINUES;
        }
    }

    return(return_value);
}


/******************************************************************************
* Function Name: LIN_1_ClrRxBuffer
*******************************************************************************
*
* Summary:
*  This function initializes the receive part of Transport Layer variables .
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_Cr timeout expired.
*     TX_TIMEOUT     - N_As timeout expired.
*
******************************************************************************/
static void LIN_1_ClrRxBuffer(l_u8 stimulus)
{
    /* Previous PCI requires to be unknown after the initialization */
    LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
    /* Resets the frame counters. */
    LIN_1_rxMessageLength = 0u;
    LIN_1_rxFrameCounter = 0u;

#if (LIN_1_COOKED_TL_API)
    if (LIN_1_tlLengthPointer != NULL)
    {
        /* Clears the length of the erroneous frame. */
        *LIN_1_tlLengthPointer = 0u;
    }
    
    if (stimulus == LIN_1_RX_TIMEOUT)
    {
        /* Sets the error status as a timeout occurrs. */
        LIN_1_rxTlStatus = LD_N_CR_TIMEOUT;
    }
    else if (stimulus == LIN_1_MRF_ALIEN_NAD)
    {
        LIN_1_rxTlStatus = LD_FAILED;
    }  
    else
    {
        if ((LIN_1_rxTlStatus != LD_FAILED)   &&
            (LIN_1_rxTlStatus != LD_WRONG_SN) &&
            (LIN_1_rxTlStatus != LD_N_CR_TIMEOUT))
        {
            /* Initializes the RX status variable properly. */
            LIN_1_rxTlStatus = LD_COMPLETED;
        }
    }

    LIN_1_rxTlDataPointer = NULL;
    LIN_1_rxTlInitDataPointer = NULL;
#else
    (void) stimulus;

    /* Resets the buffers depth to 0 to indicate the buffers are empty. */
    LIN_1_rxBufDepth = 0u;
    /* Raw API buffers initialization */
    LIN_1_rxWrIndex = 0u;
    LIN_1_rxRdIndex = 0u;
    LIN_1_rxTlStatus = LD_NO_DATA;
#endif /* LIN_1_COOKED_TL_API */

    /* Clears the Service Response ready status bit */
    LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);

    LIN_1_tlFlags &= (l_u8) (~(LIN_1_TL_RX_REQUESTED | LIN_1_TL_N_CR_TIMEOUT_ON ));

    LIN_1_tlTimeoutCnt = 0u;
}


/******************************************************************************
* Function Name: LIN_1_ClrTxBuffer
*******************************************************************************
*
* Summary:
*  This function initializes transmit part of Transport Layer variables.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_Cr timeout expired.
*     TX_TIMEOUT     - N_As timeout expired.
*
*******************************************************************************/
static void LIN_1_ClrTxBuffer(l_u8 stimulus)
{
    /* Previous PCI requires to be unknown after the initialization */
    LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

#if (LIN_1_COOKED_TL_API)
    /* Resets the frame counters */
    LIN_1_txMessageLength = 0u;
    /* Resets the frame counters */
    LIN_1_txFrameCounter = 0u;

    if (stimulus == LIN_1_TX_TIMEOUT)
    {
        /* Set error status as the timeout occurred */
        LIN_1_txTlStatus = LD_N_AS_TIMEOUT;
    }
    else if (stimulus == LIN_1_MRF_ALIEN_NAD)
    {
        LIN_1_txTlStatus = LD_FAILED;
    }
    else
    {
        if ((LIN_1_txTlStatus != LD_FAILED) && (LIN_1_txTlStatus != LD_N_AS_TIMEOUT))
        {
            /* Initializes the TX status variable properly. */
            LIN_1_txTlStatus = LD_COMPLETED;
        }
    }
    LIN_1_tlFlags &= (l_u8)(~(LIN_1_TL_TX_REQUESTED | LIN_1_TL_N_AS_TIMEOUT_ON | 
        LIN_1_TL_N_CR_TIMEOUT_ON));
#else
    (void) stimulus;
    
    if (0u == (LIN_1_status & LIN_1_STATUS_RESPONSE_PENDING))
    {   /* Regular frame */
        /* Resets the frame counters */
        LIN_1_txMessageLength = 0u;
        /* Resets the frame counters */
        LIN_1_txFrameCounter = 0u;
        /* Resets the buffers depth to 0 to indicate the buffers are empty. */
        LIN_1_txBufDepth = 0u;
        
        /* Raw API buffers initialization */
        LIN_1_txWrIndex = 0u;
        LIN_1_txRdIndex = 0u;
        LIN_1_txTlStatus = LD_QUEUE_EMPTY;
        LIN_1_tlFlags &= (l_u8)(~(LIN_1_TL_TX_REQUESTED | LIN_1_TL_N_AS_TIMEOUT_ON | 
        LIN_1_TL_N_CR_TIMEOUT_ON));
    }
    else /* Response pending frame */
    {
        /* Disables N_AS and N_CR timers, LIN Master controls P2* timeout for response pending frame. */
        LIN_1_tlFlags &= (l_u8)(~(LIN_1_TL_N_AS_TIMEOUT_ON | LIN_1_TL_N_CR_TIMEOUT_ON));
    }
#endif /* LIN_1_COOKED_TL_API */

    /* Clears Service Response Ready status bit. */
    LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);
    LIN_1_tlTimeoutCnt = 0u;
}


/******************************************************************************
* Function Name: LIN_1_ProcessMrf
*******************************************************************************
*
* Summary:
*  This API is called from ISR. It is responsible to parse the RX frames
*  that come from the LIN Master. This API handles Automatic Configuration
*  requests and receives both Raw and Cooked API frames.
*
* Parameters:
*  frame - The pointer to an array that contains a last received frame.
*
* Return:
*  NO_RESPONSE_REQUIRED      - No response to a received message is expected.
*  RESPONSE_REQUIRED         - A response to a received message is expected.
*
******************************************************************************/
static l_u8 LIN_1_ProcessMrf(l_u8 frame[])
{
    l_u8 i;
    l_u8 return_value = LIN_1_NO_RESPONSE_REQUIRED;
    l_u8 tempStatus = 0u;
    l_u8 tmpPci;
    l_u8 tmpByte;
#if (LIN_1_COOKED_TL_API)
    l_u16 tmpWord;
#endif /* LIN_1_COOKED_TL_API */

#if (LIN_1_CFG_SERVICES_ENABLED)
    l_u8 tempStatusErr = LIN_1_NO_RESPONSE_REQUIRED;
    #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))
        l_u8 idByte;
        l_u8 idMask;
        l_u8 idInvert;
    #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)) */
    /* To avoid 'unused variable' warning when relevant services are disabled */
    (void) tempStatusErr;
#endif /* LIN_1_CFG_SERVICES_ENABLED */
    (void) tempStatus;

    if ((LIN_1_NAD_GO_TO_SLEEP == frame[LIN_1_PDU_NAD_IDX]) 
        #if (LIN_1_SAE_J2602_ENABLED) /* Enable MRF with ID=0x3E for J2602 */
            /* For J2602 0x00 is a valid NAD for 0x3E requests, don't go to sleep */
            && (0u ==(LIN_1_auxStatus & LIN_1_AUXSTS_MRF_WAS_3E))
        #endif /* LIN_1_SAE_J2602_ENABLED */
       )
    {
        LIN_1_ifcStatus |= LIN_1_IFC_STS_GO_TO_SLEEP;
    }
    else if ((LIN_1_NAD_BROADCAST  == frame[LIN_1_PDU_NAD_IDX]) ||
             (LIN_1_NAD_FUNCTIONAL == frame[LIN_1_PDU_NAD_IDX]) ||
             (LIN_1_nad            == frame[LIN_1_PDU_NAD_IDX]) ||
         #if (LIN_1_SAE_J2602_ENABLED) /* Enable MRF with ID=0x3E for J2602 */
             (((LIN_1_nad & 0x0Fu) == (frame[LIN_1_PDU_NAD_IDX] & 0x0Fu)) &&
             ((frame[LIN_1_PDU_NAD_IDX] > LIN_1_NAD_BROADCAST) || 
             (0u !=(LIN_1_auxStatus & LIN_1_AUXSTS_MRF_WAS_3E)))) ||
         #endif /* LIN_1_SAE_J2602_ENABLED */
             ((LIN_1_LinSlaveConfig.initialNad == frame[LIN_1_PDU_NAD_IDX]) &&
             (LIN_1_NCS_ASSIGN_NAD            == frame[LIN_1_PDU_SID_IDX])))
    {
            #if (LIN_1_SAE_J2602_ENABLED)
            /* Inserts J2602 status byte as byte 0 for frames with ID=0x3E or NAD>=0x80 */
                if ((0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_WAS_3E)) ||
                    (LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX] > LIN_1_NAD_BROADCAST))
                    {
                /* Indicates that J2602 will be placed to the outgoing frame and
                   can be cleared if transmitted successfuly. 
                   Used for MRF with ID=0x3E or NAD>=0x80. */
                    LIN_1_auxStatus |= LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS;                
                }
                else
                {
                    LIN_1_auxStatus &= (l_u8)(~LIN_1_AUXSTS_MRF_REQUIRES_J2602STATUS);    
                }                
            #endif /* LIN_1_SAE_J2602_ENABLED */
        /* Single Frame is detected */
        if (0u == (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK))
        {
          #if (LIN_1_SAE_J2602_ENABLED)  /* Check MRF for ID=0x3E or NAD>0x7F for J2602 */ 
              if ((0u != (LIN_1_auxStatus & LIN_1_AUXSTS_MRF_WAS_3E)) ||
                 (frame[LIN_1_PDU_NAD_IDX] > LIN_1_NAD_BROADCAST))
                {
                    /* This means the user should process this Frame properly using TL API and  indicates    *
                    * that the detected SID is a diagnostic SID and it should be passed to Transport Layer. */
                    LIN_1_tlFlags |= LIN_1_TL_CS_SERVICE_DISABLED |
                                                LIN_1_TL_DIAG_FRAME_DETECTED;
                }
                else
            {
          #endif /* LIN_1_SAE_J2602_ENABLED */
            
            /* SID used for node configuration */
            switch (frame[LIN_1_PDU_SID_IDX])
            {
            #if (LIN_1_CFG_SERVICES_ENABLED)
                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL))

                case LIN_1_NCS_ASSIGN_NAD:
                    /* Checks the data length. Do not respond if the PCI length does not match this service's data length. */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    /* Checks LIN Product ID and if MRF has NAD equal to the initial NAD. */
                    if ((1u == LIN_1_LinProductId(&frame[3u])) &&
                        (LIN_1_LinSlaveConfig.initialNad == frame[LIN_1_PDU_NAD_IDX]))
                    {
                        #if (LIN_1_SAE_J2602_ENABLED)
                        /* Only change from uninitialized NAD==6F is allowed. */
                        if (LIN_1_NAD_UNINITIALIZED != LIN_1_LinSlaveConfig.initialNad)
                        {
                            break; /* NAD change is not allowed. Returns with no response. */
                        }
                        /* Tries updating messages IDs according to the new NAD.
                           Checks if the new NAD is valid in scope of the frame count. */
                        tempStatus = LIN_1_UpdateMessageIDs(frame[LIN_1_PDU_D5_NEW_NAD_IDX]);
                        if (tempStatus == CYRET_BAD_PARAM)
                        {
                            break; /* NAD is invalid for frame count. Returns with no response */
                        }
                        #endif /* LIN_1_SAE_J2602_ENABLED */
                        
                        /* Saves the received NAD */
                        LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the initial NAD. */
                        LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                            LIN_1_LinSlaveConfig.initialNad;
                            
                        /* PCI is 0 so only length is required. */
                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                        
                        /* RSID for a positive response is always SID + 0x40. */
                        LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                            LIN_1_NCS_POS_RESP_ASSIGN_NAD;
                            
                        /* Fills unused data bytes with 0xFFs. */
                        for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                        {
                            LIN_1_srfBuffer[i] = 0xFFu;
                        }
                        /* Sets a service response bit that indicates that a response is ready to be sent to Master node. */
                        LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                    }
                    else
                    {
                        /* No response because Supplier ID and Function ID are invalid or MRF NAD is not equal to the initial. */
                    }
                break;

                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))
                    /* LIN 2.0 specification only. Obsolete, and not implemented for LIN lin 2.1. */
                    case LIN_1_NCS_ASSIGN_FRAME_ID:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    #if (1u == LIN_1_LIN_2_0)
                        /* tempStatus is used in Supplier ID verification */
                        tempStatus = 1u;
                        
                        if ((frame[LIN_1_PDU_D1_IDX] != LO8(LIN_1_slaveId.supplierId)) &&
                            (frame[LIN_1_PDU_D1_IDX] != LO8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                        {
                            tempStatus = 0u;  /* Zeroes out tempStatus if data isn't for this slave */
                        }
                        if ((frame[LIN_1_PDU_D2_IDX] != HI8(LIN_1_slaveId.supplierId)) &&
                            (frame[LIN_1_PDU_D2_IDX] != HI8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                        {
                            tempStatus = 0u;   /* Zeroes out tempStatus if data isn't for this slave */
                        }
                        /* If tempStatus is not 0 then verification passed successfully */
                        if (tempStatus != 0u)
                        {
                            /* Now tempStatus is used to hold Message ID Index */
                            tempStatus = LIN_1_MessageId(frame + 5u);
                            
                            /* If Message ID index is valid, processes the request and prepares a positive response. */
                            if (tempStatus != LD_INVALID_MESSAGE_INDEX)
                            {
                                /* Set PID in the position that valid index points to */
                                LIN_1_volatileConfig[tempStatus] = frame[LIN_1_PDU_D5_IDX];
                                
                                /* Fills SRF Buffer with a response to the service. Nad field should contain the current NAD. */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                /* PCI is 0, so only the length is required. */
                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                    LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID;
                                    
                                /* Fills unused data bytes with 0xFFs */
                                for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                                {
                                    LIN_1_srfBuffer[i] = 0xFFu;
                                }
                                /* Sets service response bit that indicates that response is
                                 * ready to be sent to Master node.
                                 */
                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                            }
                        }
                    #else
                        /* Does nothing, ignores an obsolete request. */
                    #endif /* (1u == LIN_1_LIN_2_0) */
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL))
                    case LIN_1_NCS_READ_BY_ID:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* tempStatus is used to hold status of LIN_1_LinProductId() */
                        tempStatus = LIN_1_LinProductId(&frame[4u]);
                        
                        /* LIN Product Identification (only identifier is supported) */
                        if (LIN_1_NCS_READ_BY_ID_ID == frame[LIN_1_PDU_D1_IDX])
                        {
                            if (LIN_1_TRUE == tempStatus)
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                /* PCI is 0, so only the length is required. */
                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 6u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                    LIN_1_NCS_POS_RESP_READ_BY_ID;
                                    
                                /* Fills data fields with Supplier and function IDs */
                                LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] =
                                    LO8(LIN_1_slaveId.supplierId);
                                LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] =
                                    HI8(LIN_1_slaveId.supplierId);
                                LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] =
                                    LO8(LIN_1_slaveId.functionId);
                                LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] =
                                    HI8(LIN_1_slaveId.functionId);
                                LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] =
                                    LIN_1_slaveId.variant;
                                    
                                /* Set a service response bit that indicates that response is
                                 * ready to be sent to Master node.
                                 */
                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                /* No action */
                            }
                        }
                        else if (LIN_1_NCS_READ_BY_ID_SERIAL == frame[LIN_1_PDU_D1_IDX])
                        {
                            /* Serial number identification */
                            /* If Slave IDs match the received ID and SN is set, then prepares a positive response. */
                            if ((LIN_1_TRUE == tempStatus) && (NULL != LIN_1_serialNumber))
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                /* PCI is 0, so only the length is required. */
                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 5u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                    LIN_1_NCS_POS_RESP_READ_BY_ID;
                                /* Fills unused data bytes with the serial number ID. */
                                for (i = 3u; i < (LIN_1_FRAME_LEN - 1u); i++)
                                {
                                    LIN_1_srfBuffer[i] = LIN_1_serialNumber[i - 3u];
                                }
                                /* The serial number is 4-byte length, sets the last unused byte to 0xFF. */
                                LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] = 0xFFu;
                                /* Sets the service response bit that indicates that the response is
                                * ready to be sent to master node.
                                */
                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                 /* Returns a negative response, the serial number is invalid. */
                                 tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        #ifdef LIN_1_FILE_REVISION_DEFINED
                        else if (LIN_1_NCS_READ_BY_ID_FILE_REVISION == frame[LIN_1_PDU_D1_IDX])
                        {
                            /* Version of LDF/NCF identification */
                            /* If Slave IDs match the received ID and FILE_REVISION is set, then prepares a positive response. */
                            if ((LIN_1_TRUE == tempStatus) && (LIN_1_FILE_REVISION_DEFINED))
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                /* PCI is 0, so only the length is required. */
                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 5u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                    LIN_1_NCS_POS_RESP_READ_BY_ID;
                                /* Fills data bytes with revision numbers. */
                                LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] = LIN_1_FILE_REVISION_major;
                                LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] = LIN_1_FILE_REVISION_minor;
                                LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] = LIN_1_FILE_REVISION_sub;
                                LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] = LIN_1_FILE_REVISION_source;
                                /* Reserved for future use and transmitted with value 0x00. */
                                LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] = 0x00u;
                                /* Sets a service response bit that indicates that the response is
                                * ready to be sent to master node.
                                */
                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                 /* Returns a negative response, Slave IDs are invalid. */
                                 tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        #endif /* FILE_REVISION  defined*/
                        
                        else if ((frame[LIN_1_PDU_D1_IDX] >= 32u) &&
                                 (frame[LIN_1_PDU_D1_IDX] <= 63u))   /* User defined identification */
                        {
                            if (LIN_1_TRUE == tempStatus)
                            {
                                /* If user doesn't reassign the status of ld_read_by_id_callout(),
                                 * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout().
                                 * This indicates to the master that the service by the user defined
                                 * identification is not supported. tempStatus is used to hold the status of
                                 * ld_read_by_id_callout().
                                 */
                                tempStatus = ld_read_by_id_callout_LIN_1(frame[LIN_1_PDU_D1_IDX],
                                    frame + LIN_1_FRAME_DATA_SIZE_3);
                                    
                                if (tempStatus == LD_POSITIVE_RESPONSE)
                                {
                                    /* Fills SRF Buffer with a response to the service. Nad field should contain the current NAD. */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                    /* PCI is 0, so only the length is required. */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 6u;
                                    /* RSID for a positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_READ_BY_ID;
                                        
                                    /* Fills unused data bytes with user defined information */
                                    for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = frame[i];
                                    }
                                    /* Sets the service response bit that indicates that the response is
                                    * ready to be sent to master node.
                                    */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Return Negative response */
                                     tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                                }
                            }
                            else
                            {
                                /* Return Negative response, serial number is invalid */
                                tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        /* Message ID identification */
                        else if ((frame[LIN_1_PDU_D1_IDX] >= LIN_1_ID_16) &&
                                 (frame[LIN_1_PDU_D1_IDX] <= LIN_1_ID_31))
                        {
                            /* LIN 2.0 specification Only */
                        #if (1u == LIN_1_LIN_2_0)
                            /* If slave serial number matches received one, prepare positive response. */
                            if (LIN_1_TRUE == tempStatus)
                            {
                                /* Fill SRF Buffer with response to service. NAD field should contain current NAD */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                
                                /* tempStatus is now used to store calculated Message ID index */
                                tempStatus = frame[LIN_1_PDU_D1_IDX] - LD_MESSAGE_ID_BASE;
                                
                                if (LIN_1_NUM_FRAMES > tempStatus)
                                {
                                    /* PCI is 0 so only length required */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 4u;
                                    
                                    /* RSID for positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_READ_BY_ID;
                                        
                                    /* D1 = Message ID LSB */
                                    LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] =
                                        HI8(messageIdTable[tempStatus]);
                                        
                                    /* D2 = Message ID MSB */
                                    LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] =
                                        LO8(messageIdTable[tempStatus]);
                                        
                                    /* D3 = PID */
                                    LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] =
                                        LIN_1_volatileConfig[tempStatus];
                                        
                                    /* Message ID response is 3 byte length, so set last two bytes to 0xFF. */
                                    LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] = 0xFFu;
                                    LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] = 0xFFu;
                                    
                                    /* Set service response bit that indicates that response is
                                     * ready to be sent to master node.
                                     */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                     tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                                }
                            }
                            else
                            {
                                 tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        #else
                            /* Return Negative response on erroneous request */
                            tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                        #endif /* (1u == LIN_1_LIN_2_0) */
                        }
                        else
                        {
                            /* Return Negative response */
                            tempStatusErr = LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED;
                        }

                        if (LIN_1_NCS_NEGATIVE_RESPONSE_REQUIRED == tempStatusErr)
                        {
                            /* Fill SRF Buffer with negative response to service. Nad field should contain current NAD */
                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                            /* PCI is 0 so only length required */
                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 3u;
                            /* RSID for negative response is always 0x7F */
                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                LIN_1_NCS_RSID_NEG_REPLY;
                            /* D1 holds the service ID */
                            LIN_1_srfBuffer[LIN_1_PDU_D1_ID_IDX] =
                                LIN_1_NCS_READ_BY_ID;
                            /* D2 contains error code */
                            LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] = 0x12u;
                            
                            /* Fill unused data bytes with 0xFFs */
                            for (i = 5u; i < LIN_1_FRAME_LEN; i++)
                            {
                                LIN_1_srfBuffer[i] = 0xFFu;
                            }
                            /* Set service response bit that indicates that response is
                             * ready to be sent to master node.
                             */
                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                        }
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))
                    case LIN_1_NCS_COND_CHANGE_NAD:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        if (LIN_1_NCS_READ_BY_ID_ID == frame[LIN_1_PDU_D1_ID_IDX])
                        {
                            /* LIN Product Identification */
                            if ((6u > frame[LIN_1_PDU_D2_BYTE_IDX]) &&
                                (0u != frame[LIN_1_PDU_D2_BYTE_IDX]))
                            {
                                switch (frame[LIN_1_PDU_D2_BYTE_IDX])
                                {
                                    /* tempStatus is used as a temporary variable to store ID byte */
                                    case LIN_1_CS_BYTE_SUPPLIER_ID1:
                                        tempStatus = LO8(LIN_1_slaveId.supplierId);
                                    break;
                                    case LIN_1_CS_BYTE_SUPPLIER_ID2:
                                        tempStatus = HI8(LIN_1_slaveId.supplierId);
                                    break;
                                    case LIN_1_CS_BYTE_FUNCTION_ID1:
                                        tempStatus = LO8(LIN_1_slaveId.functionId);
                                    break;
                                    case LIN_1_CS_BYTE_FUNCTION_ID2:
                                        tempStatus = HI8(LIN_1_slaveId.functionId);
                                    break;
                                    case LIN_1_CS_BYTE_VARIANT:
                                        tempStatus = LIN_1_slaveId.variant;
                                    break;
                                    default:
                                        /* Never use this state. */
                                    break;
                                }

                                if (0u == ((tempStatus ^ frame[LIN_1_PDU_D4_INVERT_IDX]) &
                                    frame[LIN_1_PDU_D3_MASK_IDX]))
                                {
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                    
                                    /* Changes NAD to a new NAD. */
                                #if (LIN_1_SAE_J2602_ENABLED) 
                                    /* Try setting new NAD. Check if NAD is valid by trying to update message IDs */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LIN_1(L_IOCTL_SET_NAD, frame+LIN_1_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];
                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                    
                                    /* PCI is 0, so only the length is required. */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                                    
                                    /* RSID for a positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;
                                        
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to master node.
                                     */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request. */
                                }
                            }
                            else
                            {
                                /* Does nothing, ignores an erroneous request. */
                            }
                        }
                        else if (LIN_1_NCS_READ_BY_ID_SERIAL == frame[LIN_1_PDU_D1_ID_IDX])
                        {
                            if ((5u < frame[LIN_1_PDU_D2_BYTE_IDX]) &&
                                (0u != frame[LIN_1_PDU_D2_BYTE_IDX]))
                            {
                                /* Byte = 1 corresponds to first byte (LIN_1_serialNumber[0]) */
                                if (0u == ((LIN_1_serialNumber[frame[LIN_1_PDU_D2_BYTE_IDX] - 1u]
                                ^ frame[LIN_1_PDU_D4_INVERT_IDX]) & frame[LIN_1_PDU_D3_MASK_IDX]))
                                {
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                    /* Changes the NAD to a new NAD. */
                                   
                                #if (LIN_1_SAE_J2602_ENABLED)
                                    /* Tries setting a new NAD. Checks if the NAD is valid by trying to update the message IDs. */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LIN_1(L_IOCTL_SET_NAD, frame+LIN_1_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];
                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                    
                                    /* PCI is 0, so only the length is required. */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                                    /* RSID for a positive response is always SID + 0x40. */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;
                                        
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to Master node.
                                     */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request. */
                                }
                            }
                            else
                            {
                                /* Does nothing, ignores an erroneous request. */
                            }
                        }
                        else if ((frame[LIN_1_PDU_D1_IDX] >= 32u) &&
                                 (frame[LIN_1_PDU_D1_IDX] <= 63u))  /* User-defined identification */
                        {
                            /* Byte, Invert, and Mask must be stored in variables for user-defined
                             * identification as frame[] should contain user data after execution of
                             * ld_read_by_id_callout();
                             */
                            idByte = frame[LIN_1_PDU_D2_BYTE_IDX] - 1u;
                            idInvert = frame[LIN_1_PDU_D4_INVERT_IDX];
                            idMask = frame[LIN_1_PDU_D3_MASK_IDX];
                            
                            /* If the user doesn't re-assign the status of ld_read_by_id_callout(),
                             * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout(). This
                             * indicates to the master that the service by the user defined identification is
                             * not supported. tempStatus is used to hold the status of ld_read_by_id_callout().
                             */
                            tempStatus = ld_read_by_id_callout_LIN_1(frame[LIN_1_PDU_D1_IDX],
                                frame + LIN_1_PDU_D1_IDX);
                                
                            if ((tempStatus == LD_NEGATIVE_RESPONSE) || (tempStatus == LD_NO_RESPONSE))
                            {
                                /* Does nothing as there is no response from the user. */
                            }
                            else
                            {
                                if (0u == ((frame[idByte + LIN_1_PDU_D1_IDX] ^ idInvert) & idMask))
                                {
                                    /* Changes the NAD to a new NAD. */
                                #if (LIN_1_SAE_J2602_ENABLED)
                                    /* Tries setting a new NAD. Checks if the NAD is valid by trying to update the message IDs. */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LIN_1(L_IOCTL_SET_NAD, frame+LIN_1_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];
                                #endif /* LIN_1_SAE_J2602_ENABLED */
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the changed NAD. */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                                    /* PCI is 0, so only the length is required. */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                                    /* RSID for a positive response is always SID + 0x40. */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to master node.
                                     */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request */
                                }
                            }
                        }
                        else
                        {
                            /* Does nothing, ignores an erroneous request */
                        }
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB4_SEL))
                    case LIN_1_NCS_DATA_DUMP:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Not supported. */
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB4_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL))
                    /* Targeted reset for J2602. The node position detection for LIN20 is not supported. */
                    case LIN_1_NCS_ASSIGN_NAD_SNPD:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    /* Only Save configuaration services B5 and B6 have PCI.length=1u */
                    if (1u != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    #if (LIN_1_SAE_J2602_ENABLED)
                        LIN_1_ioctlStatus |= LIN_1_IOCTL_STS_TARGET_RESET;
                        /* Sets the reset status in J2602_Status byte */
                        LIN_1_j2602Status_Set(LIN_1_J2602_STS_RESET);
                        
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                        LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                        /* PCI is 0, so only the length is required. */
                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 6u;
                        /* RSID for a positive response is always SID + 0x40. */
                        LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                            LIN_1_NCS_POS_RESP_ASSIGN_NAD_SNPD;
                        /* Fills data fields with Supplier and function IDs. */
                        LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] =
                            LO8(LIN_1_slaveId.supplierId);
                        LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] =
                            HI8(LIN_1_slaveId.supplierId);
                        LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] =
                            LO8(LIN_1_slaveId.functionId);
                        LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] =
                            HI8(LIN_1_slaveId.functionId);
                        LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] =
                            LIN_1_slaveId.variant;
                            
                         /* Sets a service response bit that indicates that the response is
                          * ready to be sent to master node.
                          */
                         LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                    #endif /* LIN_1_SAE_J2602_ENABLED */
                    
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB6_SEL))
                    case LIN_1_NCS_SAVE_CONFIG:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    /* Only Save configuration services B5 and B6 have PCI.length=1u */
                    if (1u != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Sets the save configuration bit in status register. */
                        LIN_1_ifcStatus |= LIN_1_IFC_STS_SAVE_CONFIG;
                        
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                        LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                        /* PCI is 0, so only the length is required. */
                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                        /* RSID for a positive response is always SID + 0x40. */
                        LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                            LIN_1_NCS_POS_RESP_SAVE_CONFIG;
                            
                        /* Fills unused data bytes with 0xFFs. */
                        for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                        {
                            LIN_1_srfBuffer[i] = 0xFFu;
                        }
                        /* Set service response bit that indicates that response is ready to be sent to master node. */
                        LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB6_SEL) */

                #if (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB7_SEL))
                    case LIN_1_NCS_ASSIGN_FRAME_ID_RANGE:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LIN_1_PDU_SF_DATA_LEN != (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Zeroes out the temp status. Used as an error counter. */
                        tempStatus = 0u;
                        for (i = 0u; i < LIN_1_NCS_MAX_FRAME_ID_RANGE; i++)
                        {
                            if ((i + frame[LIN_1_PDU_D1_START_IDX]) < LIN_1_NUM_FRAMES)
                            {
                                if ((frame[i + LIN_1_PDU_D2_PID_IDX] != LIN_1_INVALID_FRAME_PID) 
                                && ((frame[i + LIN_1_PDU_D2_PID_IDX] & LIN_1_PID_PARITY_MASK) 
                                     < LIN_1_FRAME_PID_MRF))
                                {
                                    /* Unassigned value "0" is used to invalidate this frame for transportation 
                                     * to the bus. Set new received PID value */
                                    LIN_1_volatileConfig[i + frame[LIN_1_PDU_D1_START_IDX]] =
                                        frame[i + LIN_1_PDU_D2_PID_IDX];
                                }
                                else if (frame[i + LIN_1_PDU_D2_PID_IDX] == 0xFFu)
                                {
                                    /* Does nothing. */
                                }
                                else
                                {
                                    /* Indicates an error by changing the status other than 0, if Frame ID is reserved. */
                                    tempStatus++;
                                }
                            }
                            else
                            {
                                if (frame[i + LIN_1_PDU_D2_PID_IDX] != 0xFFu)
                                {
                                    tempStatus++;  /* Indicates an error by changing the status other than 0. */
                                }
                            }
                        }

                        if (tempStatus == 0u) /* No errors condition check */
                        {
                            /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;
                            /* PCI is 0, so only the length is required. */
                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;
                            /* RSID for a positive response is always SID + 0x40. */
                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID_RANGE;
                            /* Fills unused data bytes with 0xFFs. */
                            for (i = 3u; i < LIN_1_FRAME_LEN; i++)
                            {
                                LIN_1_srfBuffer[i] = 0xFFu;
                            }
                            /* Sets a service response bit that indicates that the response is
                             * ready to be sent to master node.
                             */
                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                        }
                        else
                        {
                            /* Does nothing, ignores an erroneous request */
                        }
                    break;
                #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB7_SEL) */
            #endif /* LIN_1_CFG_SERVICES_ENABLED */
            
                default:
                    /* Indicates that the requested service is disabled and Frame will be "passed" to TL.
                     * This means the user should process this Frame properly using TL API. */
                    LIN_1_tlFlags |= LIN_1_TL_CS_SERVICE_DISABLED;
                    /* Indicates that the detected SID is a diagnostic SID and it should be passed to Transport Layer. */
                    LIN_1_tlFlags |= LIN_1_TL_DIAG_FRAME_DETECTED;
                break;
            }
            #if (LIN_1_SAE_J2602_ENABLED)
            } /* End of SID based configuration services, skipped for PID == LIN_1_FRAME_PID_MRF_J2602 */
            #endif /* LIN_1_SAE_J2602_ENABLED */
            if ((0u != (LIN_1_tlFlags & LIN_1_TL_CS_SERVICE_DISABLED)) ||
                (0u != (LIN_1_tlFlags & LIN_1_TL_DIAG_FRAME_DETECTED)))
            {
                /* SID used for diagnostics */
                if (frame[LIN_1_PDU_PCI_IDX] <= LIN_1_PDU_SF_DATA_LEN)
                {
                #if (LIN_1_COOKED_TL_API)
                    /* Gets one frame of a message if there is message pending. */
                    if (LIN_1_rxTlStatus == LD_IN_PROGRESS)
                    {
                        /* Makes sure the pointer points to the receive buffer beginning. */
                        if (0u != (LIN_1_tlFlags & LIN_1_TL_CS_SERVICE_DISABLED))
                        {
                            LIN_1_rxTlDataPointer = LIN_1_rxTlInitDataPointer;
                        }
                        /* Copies data to the user buffer. */
                        for (i = 0u; i < frame[LIN_1_PDU_PCI_IDX]; i++)
                        {
                            *LIN_1_rxTlDataPointer = frame[i + 2u];
                            LIN_1_rxTlDataPointer++;
                        }
                        /* Stores NAD */
                        *LIN_1_tlNadPointer = frame[LIN_1_PDU_NAD_IDX];
                        /* Gets the data bytes length. */
                        *LIN_1_tlLengthPointer = (l_u16) frame[LIN_1_PDU_PCI_IDX];
                        /* Updates the length pointer properly. */
                        LIN_1_rxMessageLength = 0u;
                        /* SF message is received, so sets the proper status */
                        LIN_1_rxTlStatus = LD_COMPLETED;
                    }
                #else /* Raw API */
                    if (LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                    {
                        /* Fills the RX queue from MRF buffer. */
                        for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                        {
                            LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                            LIN_1_rxWrIndex++;
                        }
                        /* The Read index should point to the next byte in MRF. */
                        if (LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                        {
                            LIN_1_rxWrIndex = 0u;
                        }
                        /* 8 Bytes copied to MRF - increment buffer depth */
                        LIN_1_rxBufDepth++;
                        /* Specification doesn't require status "queue full"
                         * so unconditionally set status to data available
                         */
                        LIN_1_rxTlStatus = LD_DATA_AVAILABLE;
                        LIN_1_rxMessageLength = 0u;
                    }
                #endif /* LIN_1_COOKED_TL_API */

                    return_value = LIN_1_RESPONSE_REQUIRED;
                }
                else
                {
                    /* Does nothing. The length is not valid, data should not be trusted. */
                    return_value = LIN_1_IGNORE_TL_FRAME;
                }

                /* Clears the Service Disabled and Diagnostic Frame detected bits
                 * and process diagnostic frame receiving into user buffer or MRF.
                 */
                LIN_1_tlFlags &= ((l_u8) ~(LIN_1_TL_CS_SERVICE_DISABLED | 
                    LIN_1_TL_DIAG_FRAME_DETECTED));
            }
            else
            {
                /* Does nothing. The length is not valid, data should not be trusted. */
                return_value = LIN_1_IGNORE_TL_FRAME;
            }
        }
        else if ((frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK) ==
                 LIN_1_PDU_PCI_TYPE_FF)                 /* First Frame detected */
        {
            if (frame[LIN_1_PDU_LEN_IDX] >= LIN_1_FRAME_DATA_SIZE_7)
            {
            #if (LIN_1_COOKED_TL_API)
                /* Gets one frame of a message if there is message pending and PCI is valid. */
                LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
                
                /* Get First Frame Length with following two operations */
                tmpByte = (frame[LIN_1_PDU_PCI_IDX] & ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK));
                tmpWord = ((l_u16) ((l_u16) tmpByte << 8u)) | ((l_u16) frame[LIN_1_PDU_LEN_IDX]);

                if ((LIN_1_rxTlStatus == LD_IN_PROGRESS) && (LIN_1_TL_BUF_LEN_MAX >= tmpWord))
                {
                    LIN_1_rxMessageLength = tmpWord;
                    /* Copy Length to current length variable */
                    *LIN_1_tlLengthPointer = LIN_1_rxMessageLength;
                    for (i = 3u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                    {
                        *LIN_1_rxTlDataPointer = frame[i];   /* Get Frame Data */
                        LIN_1_rxTlDataPointer++;
                    }
            #else /* Raw API */
                if (LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                {
                    /* Copy Length to current length variable */
                    LIN_1_rxMessageLength = (l_u16)((((l_u16) frame[LIN_1_PDU_PCI_IDX]) &
                        ((l_u16) ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK))) << 8u);
                    LIN_1_rxMessageLength |= ((l_u16) frame[LIN_1_PDU_LEN_IDX]);
                    
                    /* Fill MRF from frame buffer */
                    for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                    {
                        LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                        LIN_1_rxWrIndex++;
                    }
                    /* "read" index should point to next byte in MRF */
                    if (LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                    {
                        LIN_1_rxWrIndex = 0u;
                    }
                    /* 8 Bytes copied to MRF - increment buffer depth */
                    LIN_1_rxBufDepth++;
                    /* Specification doesn't require status "queue full",
                     * so unconditionally set status to data available
                     */
                    LIN_1_rxTlStatus = LD_DATA_AVAILABLE;
            #endif /* LIN_1_COOKED_TL_API */

                    /* Updates the length pointer properly. */
                    LIN_1_rxMessageLength -= LIN_1_FRAME_DATA_SIZE_5;
                    
                    /* Saves the state of the Frame Counter to monitor future possible errors. */
                    LIN_1_rxFrameCounter = 0u;
                    
                    /* Saves the PCI type */
                    LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_FF;
                    return_value = LIN_1_RECEIVE_CONTINUES;
                }
                else
                {
                    /* Does nothing. The length is not valid. */
                    return_value = LIN_1_IGNORE_TL_FRAME;
                }
            }
            else
            {
                /* Does nothing. The length is not valid, data should not be trusted. */
                return_value = LIN_1_IGNORE_TL_FRAME;
            }
        }
        /* Consecutive Frames detected */
        else if ((frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK) ==
                 LIN_1_PDU_PCI_TYPE_CF)            /* Consecutive Frames detected */
        {
            /* Stores the previous PCI to avoid the MISRA warning. */
            tmpPci = LIN_1_rxPrevPci;
            
            /* Gets one frame of a message if there is message pending and the PCI is valid. */
            if ((tmpPci == LIN_1_PDU_PCI_TYPE_FF) || (tmpPci == LIN_1_PDU_PCI_TYPE_CF))
            {
                tmpByte = frame[LIN_1_PDU_PCI_IDX] & ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK);
                
                /* Checks if the frame counter is valid, the counter is always less than 16. */
                if (((LIN_1_rxFrameCounter + 1u) & 0x0Fu) == tmpByte)
                {
                #if (LIN_1_COOKED_TL_API)
                    /* Checks if a message is requested from the application. */
                    if (LIN_1_rxTlStatus == LD_IN_PROGRESS)
                    {
                        for (i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                        {
                            *LIN_1_rxTlDataPointer = frame[i];    /* Get Frame Data */
                            LIN_1_rxTlDataPointer++;
                        }
                #else /* Raw API */
                    if (LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                    {
                        /* Fills the MRF from the frame buffer. */
                        for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                        {
                            LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                            LIN_1_rxWrIndex++;
                        }
                        /* The Read index should point to the next byte in MRF. */
                        if (LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                        {
                            LIN_1_rxWrIndex = 0u;
                        }
                        /* 8 Bytes copied to MRF - increment buffer depth */
                        LIN_1_rxBufDepth++;
                        /* Specification doesn't require status of queue full
                         * so unconditionally set the status to data available
                         */
                        LIN_1_rxTlStatus = LD_DATA_AVAILABLE;
                #endif /* LIN_1_COOKED_TL_API */

                        /* Saves the current Frame Counter. */
                        LIN_1_rxFrameCounter = frame[LIN_1_PDU_PCI_IDX] & 
                            ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK);
                            
                        /* Saves PCI type */
                        LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_CF;
                        
                        /* Updates the length pointer properly. */
                        if (LIN_1_rxMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                        {
                            LIN_1_rxMessageLength -= LIN_1_FRAME_DATA_SIZE_6;
                            return_value = LIN_1_RECEIVE_CONTINUES;
                        }
                        else
                        {
                            LIN_1_rxMessageLength = 0u;
                            return_value = LIN_1_RESPONSE_REQUIRED;
                        }
                    }
                    else
                    {
                        /* Indicates an error if the frame counter is invalid. */
                    }
                }
                else
                {
                    /* Indicates an an error if frame counter is invalid. */
                    LIN_1_rxTlStatus = LD_WRONG_SN;
                    return_value = LIN_1_ERRONEOUS_TL_FRAME;
                }
            }
            else
            {
                /* It is a spurious CF frame */
                return_value = LIN_1_IGNORE_TL_FRAME;
            }
        }
        else
        {
            /* Does nothing. SID is invalid. */
            return_value = LIN_1_IGNORE_TL_FRAME;
        }

        if (0u != (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
        {
            /* Changes the node state to the next state. */
            return_value = LIN_1_RESPONSE_REQUIRED;
        }
    }
    else  /* Alien NAD. Indicate error */
    {
         /* Clears the service response ready status bit. */
        LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);
        
    #if (LIN_1_COOKED_TL_API)
        /* Reception failed */
        if (0u != (LIN_1_tlFlags & LIN_1_TL_RX_REQUESTED))
        {
            LIN_1_rxTlStatus = LD_FAILED;
            LIN_1_txTlStatus = LD_FAILED;
            LIN_1_tlFlags &=((l_u8) ~LIN_1_TL_RX_REQUESTED);
        }
    #endif /* LIN_1_COOKED_TL_API */
    }
        
    if (LIN_1_NAD_FUNCTIONAL != LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
    {
        if ((return_value == LIN_1_RESPONSE_REQUIRED) ||
            (return_value == LIN_1_RECEIVE_CONTINUES))
        {
            LIN_1_tlFlags |= LIN_1_TL_N_CR_TIMEOUT_ON;
            LIN_1_tlTimeoutCnt = 0u;
        }
    }

    if (return_value == LIN_1_RESPONSE_REQUIRED)
    {
        if (0u == LIN_1_rxMessageLength)
        {
            /* Stores the previous PCI to avoid the MISRA warning. */
            tmpPci = LIN_1_rxPrevPci;
            /* Resets the frame counter. */
            LIN_1_rxFrameCounter = 0u;
            /* Previous PCI is required to be unknown at the beginning of a new message. */
            LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

        #if (LIN_1_COOKED_TL_API)
            if (0u != (LIN_1_tlFlags & LIN_1_TL_RX_REQUESTED))
            {
                /* Indicates that a message is received. */
                LIN_1_rxTlStatus = LD_COMPLETED;
                /* Clears the  RX requested flag as a message was received. */
                LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_RX_REQUESTED);
            }
        #endif /* LIN_1_COOKED_TL_API */
        }
    }

    return (return_value);
}


#if (LIN_1_COOKED_TL_API)
    /*******************************************************************************
    * Function Name: ld_send_message
    ********************************************************************************
    *
    * Summary:
    *  The call packs the information specified by the data and length into one or
    *  multiple diagnostic frames. If the call is made in a master node application,
    *  the frames are transmitted to the slave node with the address NAD. If the
    *  call is made in a slave node application, the frames are transmitted to the
    *  master node with the address NAD. The parameter NAD is not used in slave
    *  nodes.
    
    *
    *  The value of the SID (or RSID) shall be the first byte in the data area.
    *
    *  The length must be in the range from 1 to 4095 bytes. The length shall also
    *  include the SID (or RSID) value, i.e. message length plus one.
    *
    *  The call is asynchronous, i.e. is not suspended until the message has been
    *  sent, and the buffer can not be changed by the application as long as calls
    *  to ld_tx_status returns LD_IN_PROGRESS.
    *
    *  The data is transmitted in suitable frames (the master request frame for
    *  master nodes and the slave response frame for slave nodes).
    *
    *  If there is a message in progress, the call returns with no action.
    *
    * Parameters:
    *  iii    - Interface.
    *  length - The size of data to be sent in bytes.
    *  nad    - the address of the slave node to which data is sent.
    *  data   - The array of data to be sent. The value of RSID is the first byte
    *           in the data area.
    *
    *******************************************************************************/
    void ld_send_message_LIN_1(l_u16 length, l_u8 nad, const l_u8* const ld_data)
    {
        volatile  l_u8 interruptState;

        /* NAD is not used in Slave node but the function definition is the same for Master and Slave. */
        (void) nad;

        if (LIN_1_txTlStatus != LD_IN_PROGRESS)
        {
        /* Interrupts can be disabled as global variables used by LIN ISR are used below. */
        interruptState = CyEnterCriticalSection();
            LIN_1_txTlDataPointer = ld_data;
            /* Resets data count */
            LIN_1_txTlDataCount = 0u;
            /* Sets up the length pointer. The length shouldn't be greater than LIN_1_TL_BUF_LEN_MAX. */
            LIN_1_txMessageLength = length;
            /* Indicates that there is a message in progress. */
            LIN_1_txTlStatus = LD_IN_PROGRESS;
            /* Indicates that a Cooked API request transmits data. */
            LIN_1_tlFlags |= LIN_1_TL_TX_REQUESTED;
        /* Restores the interrupt state. */
        CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: ld_receive_message
    ********************************************************************************
    *
    * Summary:
    *  The call prepares the LIN diagnostic module to receive one message and store
    *  it in the buffer pointed to by data. During the call, the length shall
    *  specify the maximum length allowed. When reception has completed, the length
    *  changes to the actual length and NAD to the NAD in the message.
    *
    *  SID (or RSID) are the first byte in the data area.
    *
    *  The length are in the range from 1 to 4095 bytes, but never more than the
    *  value originally set in the call. SID (or RSID) is included in the length.
    *
    *  The parameter NAD is not used in slave nodes.
    *
    *  The call is asynchronous, i.e. is not suspended until the message has been
    *  received, and the buffer can not be changed by the application as long as
    *  calls to ld_rx_status returns LD_IN_PROGRESS. If the call is made after the
    *  message transmission has started on the bus (i.e. the SF or FF is already
    *  transmitted), this message is be received. Instead, the function
    *  waits until a next message starts.
    *
    *  The data is received from the succeeding suitable frames (the master request
    *  frame for slave nodes and the slave response frame for master nodes).
    *
    *  The application shall monitor the ld_rx_status and shall not call this
    *  function until the status is LD_COMPLETED. Otherwise, this function may
    *  return inconsistent data in the parameters.
    *
    * Parameters:
    *  iii    - Interface.
    *  length - The size of data to be received in bytes.
    *  nad    - The address of Slave node from which data is received.
    *  data   - The array of data to be received. The value of the SID is the first
    *           byte in the data area.
    *
    *******************************************************************************/
    void ld_receive_message_LIN_1(l_u16* const length, l_u8* const nad, l_u8* const ld_data)
    {
        l_u8 interruptState;

        if (LIN_1_rxTlStatus != LD_IN_PROGRESS)
        {
        /* Interrupts can be disabled as global variables used by LIN ISR are used below. */
        interruptState = CyEnterCriticalSection();
        
            /* Sets user status bits */
            LIN_1_rxTlStatus = LD_IN_PROGRESS;
            /* Sets up the data pointer */
            LIN_1_rxTlDataPointer = ld_data;
            /* Sets up the initial data pointer that should always point to the user buffer beginning. */
            LIN_1_rxTlInitDataPointer = ld_data;
            /* Sets up a NAD pointer. */
            LIN_1_tlNadPointer = nad;
            
            /* Sets up the length pointer. */
            LIN_1_tlLengthPointer = length;
            LIN_1_rxMessageLength = *length;
            
            /* Indicates that Cooked API requests receive data */
            LIN_1_tlFlags |= LIN_1_TL_RX_REQUESTED;
            
        /* Restores interrupt state */
        CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: ld_tx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the last made call to ld_send_message.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  LD_IN_PROGRESS - The transmission is not yet completed.
    *
    *  LD_COMPLETED - The transmission has completed successfully (and you can
    *                 issue a new ld_send_message call). This value is also
    *                 returned after initialization of Transport Layer.
    *
    *  LD_FAILED - The transmission ended in an error. The data was only partially
    *              sent. The transport layer is reinitialized before
    *              processing further messages. To find out why transmission has
    *              failed, check the status management function l_read_status.
    *
    *  LD_N_AS_TIMEOUT - The transmission failed because of a N_As timeout,
    *
    *  Note Failed status (e.g. LD_FAILED) could be read only once by this function.
    *       On the next read status will be turned to LD_COMPLETED by the transport
    *       layer reinitialization.
    *
    *******************************************************************************/
    l_u8 ld_tx_status_LIN_1(void)
    {
        l_u8 status = LIN_1_txTlStatus;
        
        if (LIN_1_txTlStatus != LD_IN_PROGRESS)
        {
            LIN_1_txTlStatus = LD_COMPLETED;
        }
        
        return (status);
    }


    /*******************************************************************************
    * Function Name: ld_rx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the last made call to ld_receive_message.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  LD_IN_PROGRESS - The reception is not yet completed.
    *
    *  LD_COMPLETED - The reception has completed successfully and all information
    *                 (length, NAD, data) is available. (You can also issue a new
    *                 ld_receive_message call). This value is also returned after
    *                 initialization of Transport Layer.
    *
    *  LD_FAILED - The reception ended in an error. The data was only partially
    *              received and should not be trusted. Initialize before processing
    *              further transport layer messages. To find out why reception
    *              has failed, check the status management function l_read_status.
    *
    *  LD_N_CR_TIMEOUT - The reception failed because of a N_Cr timeout,
    *
    *  LD_WRONG_SN - The reception failed because of an unexpected sequence
    *                number.
    *
    *  Note Failed status (e.g. LD_WRONG_SN) could be read only once by this 
    *       function. On the next read status will be turned to LD_COMPLETED by
    *       the transport layer reinitialization.
    *
    *******************************************************************************/
    l_u8 ld_rx_status_LIN_1(void)
    {
        l_u8 status = LIN_1_rxTlStatus;
        
        if (LIN_1_rxTlStatus != LD_IN_PROGRESS)
        {
            LIN_1_rxTlStatus = LD_COMPLETED;
        }

        return(status);
    }
    
#else
    /*******************************************************************************
    * Function Name: ld_put_raw
    ********************************************************************************
    *
    * Summary:
    *  The call queues the transmission of 8 bytes of data in one frame. The data
    *  is sent in the next suitable frame (slave response frame). The data area
    *  is copied in the call, the pointer is be memorized. If no more
    *  queue resources are available, the data may be jettisoned and the
    *  appropriate error status is set.
    * Note:
    *  For J2602 mode on master requests with ID 0x3C and NAD from 0x80 to 0xFF 
    *  or ID 0x3E and NAD from 0x00 to 0xFF, API sends only 7 bytes of ld_data
    *  because Data0 contains J2602 Status byte.
    *
    * Parameters:
    *  iii - Interface.
    *  data - The array of data to be sent.
    *
    *******************************************************************************/
    void ld_put_raw_LIN_1(const l_u8* const ld_data)
    {
        l_u8 interruptState;
        l_u8 i;

        /* Interrupts can be disabled as global variables used by LIN ISR
         * are used below.
         */
        interruptState = CyEnterCriticalSection();

        /* Copies data only when the buffer is not full. */
        if (LIN_1_txBufDepth < (LIN_1_TL_TX_QUEUE_LEN / 8u))
        {
            /* Copies 8 bytes of data to Raw TX queue buffer. */
            for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
            {
                /* Copies one byte of data to SRF buffer */
                LIN_1_rawTxQueue[LIN_1_txWrIndex] = *(ld_data + i);
                LIN_1_txWrIndex++;
            }

            /* If the buffer end is reached, resets the write index. */
            if (LIN_1_txWrIndex == LIN_1_TL_TX_QUEUE_LEN)
            {
                LIN_1_txWrIndex = 0u;
            }

            /* 8 bytes of data are copied, so increment the buffer depth */
            LIN_1_txBufDepth++;

            /* Updates the status properly */
            if (LIN_1_txBufDepth == (LIN_1_TL_TX_QUEUE_LEN / 8u))
            {
                LIN_1_txTlStatus = LD_QUEUE_FULL;
            }
            else
            {
                LIN_1_txTlStatus = LD_QUEUE_AVAILABLE;
            }
        
            LIN_1_tlFlags |= LIN_1_TL_TX_REQUESTED;
        }

        /* Restores the interrupt state */
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: ld_get_raw
    ********************************************************************************
    *
    * Summary:
    *  The call copies the oldest received diagnostic frame data to the memory
    *  specified by data. The data returned is received from the master request
    *  frame If the receive queue is empty, no data is copied.
    *
    * Parameters:
    *  iii  - Interface.
    *  data - The array to which the oldest received diagnostic frame data is
    *         copied.
    *
    *******************************************************************************/
    void ld_get_raw_LIN_1(l_u8* const ld_data)
    {
        l_u8 interruptState;
        l_u8 i;

        /* Interrupts can be disabled as global variables used by LIN ISR
         * are used below.
         */
        interruptState = CyEnterCriticalSection();

        /* If the queue is empty, does not copy anything. */
        if (LIN_1_rxBufDepth != 0u)
        {
            /* Copies 8 bytes of data from Raw RX queue buffer. */
            for (i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
            {
                *(ld_data + i) = LIN_1_rawRxQueue[LIN_1_rxRdIndex];
                LIN_1_rxRdIndex++;
            }

            /* 8 bytes of data are copied. Decrement buffer depth */
            LIN_1_rxBufDepth--;

            /* If buffer end is reached, go to start */
            if (LIN_1_rxRdIndex == LIN_1_TL_RX_QUEUE_LEN)
            {
                LIN_1_rxRdIndex = 0u;
            }

            /* Update status properly */
            if (LIN_1_rxBufDepth == 0u)
            {
                LIN_1_rxTlStatus = LD_NO_DATA;
            }
            else
            {
                LIN_1_rxTlStatus = LD_DATA_AVAILABLE;
            }
        }

        /* Restore interrupt state */
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: ld_raw_tx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the raw frame transmission function.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  Status:
    *   LD_QUEUE_EMPTY - The transmit queue is empty. If previous calls to
    *   ld_put_raw, all frames in the queue have been transmitted.
    *
    *   LD_QUEUE_AVAILABLE - The transmit queue contains entries, but is not full.
    *
    *   LD_QUEUE_FULL - The transmit queue is full and can not accept further
    *   frames.
    *
    *   LD_TRANSMIT_ERROR - LIN protocol errors occur during the transfer;
    *   initialize and redo the transfer.
    *
    *******************************************************************************/
    l_u8 ld_raw_tx_status_LIN_1(void)
    {
        return (LIN_1_txTlStatus);
    }


    /*******************************************************************************
    * Function Name: LIN_1_ld_raw_rx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the raw frame receive function.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  Status:
    *   LD_NO_DATA - The receive queue is empty.
    *
    *   LD_DATA_AVAILABLE - The receive queue contains data that can be read.
    *
    *   LD_RECEIVE_ERROR - LIN protocol errors occur during the transfer;
    *
    *   initialize and redo the transfer.
    *
    *******************************************************************************/
    l_u8 ld_raw_rx_status_LIN_1(void)
    {
        return (LIN_1_rxTlStatus);
    }
#endif /* LIN_1_COOKED_TL_API */


#endif /* LIN_1_TL_USED */


/*******************************************************************************
* Function Name: LIN_1_TimerEnableInt
********************************************************************************
*
* Summary:
*  Enables Systick timer interrupts and configures the SystemTick timer callback
*  slot to the component function that updates the timestamp value.
*
* Return:
*  Status
*    Value                 Description
*     CYRET_SUCCESS         Successful
*     CYRET_INVALID_STATE   Failure - No free callback slot has been found.
*
*******************************************************************************/
cystatus LIN_1_TimerEnableInt(void)
{
    cystatus retValue = CYRET_INVALID_STATE;
    uint32 i;

    /* If SysTick timer isn't configured */
    if (CySysTickInitVar == 0u)
    {
        /* SysTick timer seems to be not configured.
         * Configure SysTick timer and set callback.
         */
        CySysTickStart();
        (void) CySysTickSetCallback(0u, &l_ifc_aux_LIN_1);
        retValue = CYRET_SUCCESS;
    }
    else
    {
        /* If SysTick timer is configured, checks if its own callback exists. */
        for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            if (CySysTickGetCallback(i) == &l_ifc_aux_LIN_1)
            {
                retValue = CYRET_SUCCESS;
                break;
            }
        }

        /* Looks for an unused callback slot to fill it with its own callback. */
        if (CYRET_SUCCESS != retValue)
        {
            for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
            {
                /* Makes sure that the callback slot is not used. */
                if (CySysTickGetCallback(i) == (void *) 0 )
                {
                    (void) CySysTickSetCallback(i, &l_ifc_aux_LIN_1);
                    retValue = CYRET_SUCCESS;
                    break;
                }
            }
        }
    }
    return (retValue);
}


/*******************************************************************************
* Function Name: LIN_1_TimerDisableInt
********************************************************************************
*
* Summary:
*  Disables Systick timer interrupts.
*
* Side Effects:
*  Stops the SysTick timer if there are no active callback slots left and
*  disable the SysTick interrupt.
*
*******************************************************************************/
void LIN_1_TimerDisableInt(void)
{
    uint32 i;
    uint32 nullPointers = 0u;

    /* Find used callback slot */
    for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
    {
        if (CySysTickGetCallback(i) == &l_ifc_aux_LIN_1)
        {
            /* Free callback */
            (void)CySysTickSetCallback(i, (void *) 0 );
            nullPointers++;
        }
        else if (CySysTickGetCallback(i) == (void *) 0 )
        {
            /* This callback slot is unused */
            nullPointers++;
        }
        else
        {
            /* This callback slot is used by some function */
        }
    }

    if (nullPointers == CY_SYS_SYST_NUM_OF_CALLBACKS)
    {
        /* If there are no used callback slots - disable SysTick timer */
        CySysTickDisableInterrupt();
        CySysTickStop();
    }
}

/* [] END OF FILE */
