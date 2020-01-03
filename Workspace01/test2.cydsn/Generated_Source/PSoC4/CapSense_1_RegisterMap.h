/***************************************************************************//**
* \file CapSense_1_RegisterMap.h
* \version 6.0
*
* \brief
*   This file provides the definitions for the Component data structure register.
*
* \see CapSense_1 v6.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2018), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#if !defined(CY_SENSE_CapSense_1_REGISTER_MAP_H)
#define CY_SENSE_CapSense_1_REGISTER_MAP_H

#include <cytypes.h>
#include "CapSense_1_Configuration.h"
#include "CapSense_1_Structure.h"

/*****************************************************************************/
/* RAM Data structure register definitions                                   */
/*****************************************************************************/
#define CapSense_1_CONFIG_ID_VALUE                          (CapSense_1_dsRam.configId)
#define CapSense_1_CONFIG_ID_OFFSET                         (0u)
#define CapSense_1_CONFIG_ID_SIZE                           (2u)
#define CapSense_1_CONFIG_ID_PARAM_ID                       (0x87000000u)

#define CapSense_1_DEVICE_ID_VALUE                          (CapSense_1_dsRam.deviceId)
#define CapSense_1_DEVICE_ID_OFFSET                         (2u)
#define CapSense_1_DEVICE_ID_SIZE                           (2u)
#define CapSense_1_DEVICE_ID_PARAM_ID                       (0x8B000002u)

#define CapSense_1_HW_CLOCK_VALUE                           (CapSense_1_dsRam.hwClock)
#define CapSense_1_HW_CLOCK_OFFSET                          (4u)
#define CapSense_1_HW_CLOCK_SIZE                            (2u)
#define CapSense_1_HW_CLOCK_PARAM_ID                        (0x86000004u)

#define CapSense_1_TUNER_CMD_VALUE                          (CapSense_1_dsRam.tunerCmd)
#define CapSense_1_TUNER_CMD_OFFSET                         (6u)
#define CapSense_1_TUNER_CMD_SIZE                           (2u)
#define CapSense_1_TUNER_CMD_PARAM_ID                       (0xA1000006u)

#define CapSense_1_SCAN_COUNTER_VALUE                       (CapSense_1_dsRam.scanCounter)
#define CapSense_1_SCAN_COUNTER_OFFSET                      (8u)
#define CapSense_1_SCAN_COUNTER_SIZE                        (2u)
#define CapSense_1_SCAN_COUNTER_PARAM_ID                    (0x85000008u)

#define CapSense_1_STATUS_VALUE                             (CapSense_1_dsRam.status)
#define CapSense_1_STATUS_OFFSET                            (12u)
#define CapSense_1_STATUS_SIZE                              (4u)
#define CapSense_1_STATUS_PARAM_ID                          (0xCB00000Cu)

#define CapSense_1_WDGT_ENABLE0_VALUE                       (CapSense_1_dsRam.wdgtEnable[0u])
#define CapSense_1_WDGT_ENABLE0_OFFSET                      (16u)
#define CapSense_1_WDGT_ENABLE0_SIZE                        (4u)
#define CapSense_1_WDGT_ENABLE0_PARAM_ID                    (0xE6000010u)

#define CapSense_1_WDGT_STATUS0_VALUE                       (CapSense_1_dsRam.wdgtStatus[0u])
#define CapSense_1_WDGT_STATUS0_OFFSET                      (20u)
#define CapSense_1_WDGT_STATUS0_SIZE                        (4u)
#define CapSense_1_WDGT_STATUS0_PARAM_ID                    (0xCC000014u)

#define CapSense_1_SNS_STATUS0_VALUE                        (CapSense_1_dsRam.snsStatus[0u])
#define CapSense_1_SNS_STATUS0_OFFSET                       (24u)
#define CapSense_1_SNS_STATUS0_SIZE                         (1u)
#define CapSense_1_SNS_STATUS0_PARAM_ID                     (0x48000018u)

#define CapSense_1_SNS_STATUS1_VALUE                        (CapSense_1_dsRam.snsStatus[1u])
#define CapSense_1_SNS_STATUS1_OFFSET                       (25u)
#define CapSense_1_SNS_STATUS1_SIZE                         (1u)
#define CapSense_1_SNS_STATUS1_PARAM_ID                     (0x4E000019u)

#define CapSense_1_SNS_STATUS2_VALUE                        (CapSense_1_dsRam.snsStatus[2u])
#define CapSense_1_SNS_STATUS2_OFFSET                       (26u)
#define CapSense_1_SNS_STATUS2_SIZE                         (1u)
#define CapSense_1_SNS_STATUS2_PARAM_ID                     (0x4400001Au)

#define CapSense_1_SNS_STATUS3_VALUE                        (CapSense_1_dsRam.snsStatus[3u])
#define CapSense_1_SNS_STATUS3_OFFSET                       (27u)
#define CapSense_1_SNS_STATUS3_SIZE                         (1u)
#define CapSense_1_SNS_STATUS3_PARAM_ID                     (0x4200001Bu)

#define CapSense_1_SNS_STATUS4_VALUE                        (CapSense_1_dsRam.snsStatus[4u])
#define CapSense_1_SNS_STATUS4_OFFSET                       (28u)
#define CapSense_1_SNS_STATUS4_SIZE                         (1u)
#define CapSense_1_SNS_STATUS4_PARAM_ID                     (0x4900001Cu)

#define CapSense_1_SNS_STATUS5_VALUE                        (CapSense_1_dsRam.snsStatus[5u])
#define CapSense_1_SNS_STATUS5_OFFSET                       (29u)
#define CapSense_1_SNS_STATUS5_SIZE                         (1u)
#define CapSense_1_SNS_STATUS5_PARAM_ID                     (0x4F00001Du)

#define CapSense_1_CSD0_CONFIG_VALUE                        (CapSense_1_dsRam.csd0Config)
#define CapSense_1_CSD0_CONFIG_OFFSET                       (30u)
#define CapSense_1_CSD0_CONFIG_SIZE                         (2u)
#define CapSense_1_CSD0_CONFIG_PARAM_ID                     (0xAB80001Eu)

#define CapSense_1_MOD_CSD_CLK_VALUE                        (CapSense_1_dsRam.modCsdClk)
#define CapSense_1_MOD_CSD_CLK_OFFSET                       (32u)
#define CapSense_1_MOD_CSD_CLK_SIZE                         (1u)
#define CapSense_1_MOD_CSD_CLK_PARAM_ID                     (0x63800020u)

#define CapSense_1_RETURNS_RESOLUTION_VALUE                 (CapSense_1_dsRam.wdgtList.returns.resolution)
#define CapSense_1_RETURNS_RESOLUTION_OFFSET                (34u)
#define CapSense_1_RETURNS_RESOLUTION_SIZE                  (2u)
#define CapSense_1_RETURNS_RESOLUTION_PARAM_ID              (0xA7800022u)

#define CapSense_1_RETURNS_FINGER_TH_VALUE                  (CapSense_1_dsRam.wdgtList.returns.fingerTh)
#define CapSense_1_RETURNS_FINGER_TH_OFFSET                 (36u)
#define CapSense_1_RETURNS_FINGER_TH_SIZE                   (2u)
#define CapSense_1_RETURNS_FINGER_TH_PARAM_ID               (0xAA800024u)

#define CapSense_1_RETURNS_NOISE_TH_VALUE                   (CapSense_1_dsRam.wdgtList.returns.noiseTh)
#define CapSense_1_RETURNS_NOISE_TH_OFFSET                  (38u)
#define CapSense_1_RETURNS_NOISE_TH_SIZE                    (1u)
#define CapSense_1_RETURNS_NOISE_TH_PARAM_ID                (0x6E800026u)

#define CapSense_1_RETURNS_NNOISE_TH_VALUE                  (CapSense_1_dsRam.wdgtList.returns.nNoiseTh)
#define CapSense_1_RETURNS_NNOISE_TH_OFFSET                 (39u)
#define CapSense_1_RETURNS_NNOISE_TH_SIZE                   (1u)
#define CapSense_1_RETURNS_NNOISE_TH_PARAM_ID               (0x68800027u)

#define CapSense_1_RETURNS_HYSTERESIS_VALUE                 (CapSense_1_dsRam.wdgtList.returns.hysteresis)
#define CapSense_1_RETURNS_HYSTERESIS_OFFSET                (40u)
#define CapSense_1_RETURNS_HYSTERESIS_SIZE                  (1u)
#define CapSense_1_RETURNS_HYSTERESIS_PARAM_ID              (0x61800028u)

#define CapSense_1_RETURNS_ON_DEBOUNCE_VALUE                (CapSense_1_dsRam.wdgtList.returns.onDebounce)
#define CapSense_1_RETURNS_ON_DEBOUNCE_OFFSET               (41u)
#define CapSense_1_RETURNS_ON_DEBOUNCE_SIZE                 (1u)
#define CapSense_1_RETURNS_ON_DEBOUNCE_PARAM_ID             (0x67800029u)

#define CapSense_1_RETURNS_LOW_BSLN_RST_VALUE               (CapSense_1_dsRam.wdgtList.returns.lowBslnRst)
#define CapSense_1_RETURNS_LOW_BSLN_RST_OFFSET              (42u)
#define CapSense_1_RETURNS_LOW_BSLN_RST_SIZE                (1u)
#define CapSense_1_RETURNS_LOW_BSLN_RST_PARAM_ID            (0x6D80002Au)

#define CapSense_1_RETURNS_IDAC_MOD0_VALUE                  (CapSense_1_dsRam.wdgtList.returns.idacMod[0u])
#define CapSense_1_RETURNS_IDAC_MOD0_OFFSET                 (43u)
#define CapSense_1_RETURNS_IDAC_MOD0_SIZE                   (1u)
#define CapSense_1_RETURNS_IDAC_MOD0_PARAM_ID               (0x6600002Bu)

#define CapSense_1_RETURNS_SNS_CLK_VALUE                    (CapSense_1_dsRam.wdgtList.returns.snsClk)
#define CapSense_1_RETURNS_SNS_CLK_OFFSET                   (44u)
#define CapSense_1_RETURNS_SNS_CLK_SIZE                     (2u)
#define CapSense_1_RETURNS_SNS_CLK_PARAM_ID                 (0xA880002Cu)

#define CapSense_1_RETURNS_SNS_CLK_SOURCE_VALUE             (CapSense_1_dsRam.wdgtList.returns.snsClkSource)
#define CapSense_1_RETURNS_SNS_CLK_SOURCE_OFFSET            (46u)
#define CapSense_1_RETURNS_SNS_CLK_SOURCE_SIZE              (1u)
#define CapSense_1_RETURNS_SNS_CLK_SOURCE_PARAM_ID          (0x4780002Eu)

#define CapSense_1_SOURCE_RESOLUTION_VALUE                  (CapSense_1_dsRam.wdgtList.source.resolution)
#define CapSense_1_SOURCE_RESOLUTION_OFFSET                 (48u)
#define CapSense_1_SOURCE_RESOLUTION_SIZE                   (2u)
#define CapSense_1_SOURCE_RESOLUTION_PARAM_ID               (0xAD810030u)

#define CapSense_1_SOURCE_FINGER_TH_VALUE                   (CapSense_1_dsRam.wdgtList.source.fingerTh)
#define CapSense_1_SOURCE_FINGER_TH_OFFSET                  (50u)
#define CapSense_1_SOURCE_FINGER_TH_SIZE                    (2u)
#define CapSense_1_SOURCE_FINGER_TH_PARAM_ID                (0xA1810032u)

#define CapSense_1_SOURCE_NOISE_TH_VALUE                    (CapSense_1_dsRam.wdgtList.source.noiseTh)
#define CapSense_1_SOURCE_NOISE_TH_OFFSET                   (52u)
#define CapSense_1_SOURCE_NOISE_TH_SIZE                     (1u)
#define CapSense_1_SOURCE_NOISE_TH_PARAM_ID                 (0x64810034u)

#define CapSense_1_SOURCE_NNOISE_TH_VALUE                   (CapSense_1_dsRam.wdgtList.source.nNoiseTh)
#define CapSense_1_SOURCE_NNOISE_TH_OFFSET                  (53u)
#define CapSense_1_SOURCE_NNOISE_TH_SIZE                    (1u)
#define CapSense_1_SOURCE_NNOISE_TH_PARAM_ID                (0x62810035u)

#define CapSense_1_SOURCE_HYSTERESIS_VALUE                  (CapSense_1_dsRam.wdgtList.source.hysteresis)
#define CapSense_1_SOURCE_HYSTERESIS_OFFSET                 (54u)
#define CapSense_1_SOURCE_HYSTERESIS_SIZE                   (1u)
#define CapSense_1_SOURCE_HYSTERESIS_PARAM_ID               (0x68810036u)

#define CapSense_1_SOURCE_ON_DEBOUNCE_VALUE                 (CapSense_1_dsRam.wdgtList.source.onDebounce)
#define CapSense_1_SOURCE_ON_DEBOUNCE_OFFSET                (55u)
#define CapSense_1_SOURCE_ON_DEBOUNCE_SIZE                  (1u)
#define CapSense_1_SOURCE_ON_DEBOUNCE_PARAM_ID              (0x6E810037u)

#define CapSense_1_SOURCE_LOW_BSLN_RST_VALUE                (CapSense_1_dsRam.wdgtList.source.lowBslnRst)
#define CapSense_1_SOURCE_LOW_BSLN_RST_OFFSET               (56u)
#define CapSense_1_SOURCE_LOW_BSLN_RST_SIZE                 (1u)
#define CapSense_1_SOURCE_LOW_BSLN_RST_PARAM_ID             (0x67810038u)

#define CapSense_1_SOURCE_IDAC_MOD0_VALUE                   (CapSense_1_dsRam.wdgtList.source.idacMod[0u])
#define CapSense_1_SOURCE_IDAC_MOD0_OFFSET                  (57u)
#define CapSense_1_SOURCE_IDAC_MOD0_SIZE                    (1u)
#define CapSense_1_SOURCE_IDAC_MOD0_PARAM_ID                (0x6C010039u)

#define CapSense_1_SOURCE_SNS_CLK_VALUE                     (CapSense_1_dsRam.wdgtList.source.snsClk)
#define CapSense_1_SOURCE_SNS_CLK_OFFSET                    (58u)
#define CapSense_1_SOURCE_SNS_CLK_SIZE                      (2u)
#define CapSense_1_SOURCE_SNS_CLK_PARAM_ID                  (0xA381003Au)

#define CapSense_1_SOURCE_SNS_CLK_SOURCE_VALUE              (CapSense_1_dsRam.wdgtList.source.snsClkSource)
#define CapSense_1_SOURCE_SNS_CLK_SOURCE_OFFSET             (60u)
#define CapSense_1_SOURCE_SNS_CLK_SOURCE_SIZE               (1u)
#define CapSense_1_SOURCE_SNS_CLK_SOURCE_PARAM_ID           (0x4D81003Cu)

#define CapSense_1_DOWN_RESOLUTION_VALUE                    (CapSense_1_dsRam.wdgtList.down.resolution)
#define CapSense_1_DOWN_RESOLUTION_OFFSET                   (62u)
#define CapSense_1_DOWN_RESOLUTION_SIZE                     (2u)
#define CapSense_1_DOWN_RESOLUTION_PARAM_ID                 (0xA782003Eu)

#define CapSense_1_DOWN_FINGER_TH_VALUE                     (CapSense_1_dsRam.wdgtList.down.fingerTh)
#define CapSense_1_DOWN_FINGER_TH_OFFSET                    (64u)
#define CapSense_1_DOWN_FINGER_TH_SIZE                      (2u)
#define CapSense_1_DOWN_FINGER_TH_PARAM_ID                  (0xAA820040u)

#define CapSense_1_DOWN_NOISE_TH_VALUE                      (CapSense_1_dsRam.wdgtList.down.noiseTh)
#define CapSense_1_DOWN_NOISE_TH_OFFSET                     (66u)
#define CapSense_1_DOWN_NOISE_TH_SIZE                       (1u)
#define CapSense_1_DOWN_NOISE_TH_PARAM_ID                   (0x6E820042u)

#define CapSense_1_DOWN_NNOISE_TH_VALUE                     (CapSense_1_dsRam.wdgtList.down.nNoiseTh)
#define CapSense_1_DOWN_NNOISE_TH_OFFSET                    (67u)
#define CapSense_1_DOWN_NNOISE_TH_SIZE                      (1u)
#define CapSense_1_DOWN_NNOISE_TH_PARAM_ID                  (0x68820043u)

#define CapSense_1_DOWN_HYSTERESIS_VALUE                    (CapSense_1_dsRam.wdgtList.down.hysteresis)
#define CapSense_1_DOWN_HYSTERESIS_OFFSET                   (68u)
#define CapSense_1_DOWN_HYSTERESIS_SIZE                     (1u)
#define CapSense_1_DOWN_HYSTERESIS_PARAM_ID                 (0x63820044u)

#define CapSense_1_DOWN_ON_DEBOUNCE_VALUE                   (CapSense_1_dsRam.wdgtList.down.onDebounce)
#define CapSense_1_DOWN_ON_DEBOUNCE_OFFSET                  (69u)
#define CapSense_1_DOWN_ON_DEBOUNCE_SIZE                    (1u)
#define CapSense_1_DOWN_ON_DEBOUNCE_PARAM_ID                (0x65820045u)

#define CapSense_1_DOWN_LOW_BSLN_RST_VALUE                  (CapSense_1_dsRam.wdgtList.down.lowBslnRst)
#define CapSense_1_DOWN_LOW_BSLN_RST_OFFSET                 (70u)
#define CapSense_1_DOWN_LOW_BSLN_RST_SIZE                   (1u)
#define CapSense_1_DOWN_LOW_BSLN_RST_PARAM_ID               (0x6F820046u)

#define CapSense_1_DOWN_IDAC_MOD0_VALUE                     (CapSense_1_dsRam.wdgtList.down.idacMod[0u])
#define CapSense_1_DOWN_IDAC_MOD0_OFFSET                    (71u)
#define CapSense_1_DOWN_IDAC_MOD0_SIZE                      (1u)
#define CapSense_1_DOWN_IDAC_MOD0_PARAM_ID                  (0x64020047u)

#define CapSense_1_DOWN_SNS_CLK_VALUE                       (CapSense_1_dsRam.wdgtList.down.snsClk)
#define CapSense_1_DOWN_SNS_CLK_OFFSET                      (72u)
#define CapSense_1_DOWN_SNS_CLK_SIZE                        (2u)
#define CapSense_1_DOWN_SNS_CLK_PARAM_ID                    (0xA8820048u)

#define CapSense_1_DOWN_SNS_CLK_SOURCE_VALUE                (CapSense_1_dsRam.wdgtList.down.snsClkSource)
#define CapSense_1_DOWN_SNS_CLK_SOURCE_OFFSET               (74u)
#define CapSense_1_DOWN_SNS_CLK_SOURCE_SIZE                 (1u)
#define CapSense_1_DOWN_SNS_CLK_SOURCE_PARAM_ID             (0x4782004Au)

#define CapSense_1_SEEKREDUCE_RESOLUTION_VALUE              (CapSense_1_dsRam.wdgtList.seekreduce.resolution)
#define CapSense_1_SEEKREDUCE_RESOLUTION_OFFSET             (76u)
#define CapSense_1_SEEKREDUCE_RESOLUTION_SIZE               (2u)
#define CapSense_1_SEEKREDUCE_RESOLUTION_PARAM_ID           (0xAA83004Cu)

#define CapSense_1_SEEKREDUCE_FINGER_TH_VALUE               (CapSense_1_dsRam.wdgtList.seekreduce.fingerTh)
#define CapSense_1_SEEKREDUCE_FINGER_TH_OFFSET              (78u)
#define CapSense_1_SEEKREDUCE_FINGER_TH_SIZE                (2u)
#define CapSense_1_SEEKREDUCE_FINGER_TH_PARAM_ID            (0xA683004Eu)

#define CapSense_1_SEEKREDUCE_NOISE_TH_VALUE                (CapSense_1_dsRam.wdgtList.seekreduce.noiseTh)
#define CapSense_1_SEEKREDUCE_NOISE_TH_OFFSET               (80u)
#define CapSense_1_SEEKREDUCE_NOISE_TH_SIZE                 (1u)
#define CapSense_1_SEEKREDUCE_NOISE_TH_PARAM_ID             (0x64830050u)

#define CapSense_1_SEEKREDUCE_NNOISE_TH_VALUE               (CapSense_1_dsRam.wdgtList.seekreduce.nNoiseTh)
#define CapSense_1_SEEKREDUCE_NNOISE_TH_OFFSET              (81u)
#define CapSense_1_SEEKREDUCE_NNOISE_TH_SIZE                (1u)
#define CapSense_1_SEEKREDUCE_NNOISE_TH_PARAM_ID            (0x62830051u)

#define CapSense_1_SEEKREDUCE_HYSTERESIS_VALUE              (CapSense_1_dsRam.wdgtList.seekreduce.hysteresis)
#define CapSense_1_SEEKREDUCE_HYSTERESIS_OFFSET             (82u)
#define CapSense_1_SEEKREDUCE_HYSTERESIS_SIZE               (1u)
#define CapSense_1_SEEKREDUCE_HYSTERESIS_PARAM_ID           (0x68830052u)

#define CapSense_1_SEEKREDUCE_ON_DEBOUNCE_VALUE             (CapSense_1_dsRam.wdgtList.seekreduce.onDebounce)
#define CapSense_1_SEEKREDUCE_ON_DEBOUNCE_OFFSET            (83u)
#define CapSense_1_SEEKREDUCE_ON_DEBOUNCE_SIZE              (1u)
#define CapSense_1_SEEKREDUCE_ON_DEBOUNCE_PARAM_ID          (0x6E830053u)

#define CapSense_1_SEEKREDUCE_LOW_BSLN_RST_VALUE            (CapSense_1_dsRam.wdgtList.seekreduce.lowBslnRst)
#define CapSense_1_SEEKREDUCE_LOW_BSLN_RST_OFFSET           (84u)
#define CapSense_1_SEEKREDUCE_LOW_BSLN_RST_SIZE             (1u)
#define CapSense_1_SEEKREDUCE_LOW_BSLN_RST_PARAM_ID         (0x65830054u)

#define CapSense_1_SEEKREDUCE_IDAC_MOD0_VALUE               (CapSense_1_dsRam.wdgtList.seekreduce.idacMod[0u])
#define CapSense_1_SEEKREDUCE_IDAC_MOD0_OFFSET              (85u)
#define CapSense_1_SEEKREDUCE_IDAC_MOD0_SIZE                (1u)
#define CapSense_1_SEEKREDUCE_IDAC_MOD0_PARAM_ID            (0x6E030055u)

#define CapSense_1_SEEKREDUCE_SNS_CLK_VALUE                 (CapSense_1_dsRam.wdgtList.seekreduce.snsClk)
#define CapSense_1_SEEKREDUCE_SNS_CLK_OFFSET                (86u)
#define CapSense_1_SEEKREDUCE_SNS_CLK_SIZE                  (2u)
#define CapSense_1_SEEKREDUCE_SNS_CLK_PARAM_ID              (0xA1830056u)

#define CapSense_1_SEEKREDUCE_SNS_CLK_SOURCE_VALUE          (CapSense_1_dsRam.wdgtList.seekreduce.snsClkSource)
#define CapSense_1_SEEKREDUCE_SNS_CLK_SOURCE_OFFSET         (88u)
#define CapSense_1_SEEKREDUCE_SNS_CLK_SOURCE_SIZE           (1u)
#define CapSense_1_SEEKREDUCE_SNS_CLK_SOURCE_PARAM_ID       (0x4D830058u)

#define CapSense_1_OK_RESOLUTION_VALUE                      (CapSense_1_dsRam.wdgtList.ok.resolution)
#define CapSense_1_OK_RESOLUTION_OFFSET                     (90u)
#define CapSense_1_OK_RESOLUTION_SIZE                       (2u)
#define CapSense_1_OK_RESOLUTION_PARAM_ID                   (0xAB84005Au)

#define CapSense_1_OK_FINGER_TH_VALUE                       (CapSense_1_dsRam.wdgtList.ok.fingerTh)
#define CapSense_1_OK_FINGER_TH_OFFSET                      (92u)
#define CapSense_1_OK_FINGER_TH_SIZE                        (2u)
#define CapSense_1_OK_FINGER_TH_PARAM_ID                    (0xA684005Cu)

#define CapSense_1_OK_NOISE_TH_VALUE                        (CapSense_1_dsRam.wdgtList.ok.noiseTh)
#define CapSense_1_OK_NOISE_TH_OFFSET                       (94u)
#define CapSense_1_OK_NOISE_TH_SIZE                         (1u)
#define CapSense_1_OK_NOISE_TH_PARAM_ID                     (0x6284005Eu)

#define CapSense_1_OK_NNOISE_TH_VALUE                       (CapSense_1_dsRam.wdgtList.ok.nNoiseTh)
#define CapSense_1_OK_NNOISE_TH_OFFSET                      (95u)
#define CapSense_1_OK_NNOISE_TH_SIZE                        (1u)
#define CapSense_1_OK_NNOISE_TH_PARAM_ID                    (0x6484005Fu)

#define CapSense_1_OK_HYSTERESIS_VALUE                      (CapSense_1_dsRam.wdgtList.ok.hysteresis)
#define CapSense_1_OK_HYSTERESIS_OFFSET                     (96u)
#define CapSense_1_OK_HYSTERESIS_SIZE                       (1u)
#define CapSense_1_OK_HYSTERESIS_PARAM_ID                   (0x62840060u)

#define CapSense_1_OK_ON_DEBOUNCE_VALUE                     (CapSense_1_dsRam.wdgtList.ok.onDebounce)
#define CapSense_1_OK_ON_DEBOUNCE_OFFSET                    (97u)
#define CapSense_1_OK_ON_DEBOUNCE_SIZE                      (1u)
#define CapSense_1_OK_ON_DEBOUNCE_PARAM_ID                  (0x64840061u)

#define CapSense_1_OK_LOW_BSLN_RST_VALUE                    (CapSense_1_dsRam.wdgtList.ok.lowBslnRst)
#define CapSense_1_OK_LOW_BSLN_RST_OFFSET                   (98u)
#define CapSense_1_OK_LOW_BSLN_RST_SIZE                     (1u)
#define CapSense_1_OK_LOW_BSLN_RST_PARAM_ID                 (0x6E840062u)

#define CapSense_1_OK_IDAC_MOD0_VALUE                       (CapSense_1_dsRam.wdgtList.ok.idacMod[0u])
#define CapSense_1_OK_IDAC_MOD0_OFFSET                      (99u)
#define CapSense_1_OK_IDAC_MOD0_SIZE                        (1u)
#define CapSense_1_OK_IDAC_MOD0_PARAM_ID                    (0x65040063u)

#define CapSense_1_OK_SNS_CLK_VALUE                         (CapSense_1_dsRam.wdgtList.ok.snsClk)
#define CapSense_1_OK_SNS_CLK_OFFSET                        (100u)
#define CapSense_1_OK_SNS_CLK_SIZE                          (2u)
#define CapSense_1_OK_SNS_CLK_PARAM_ID                      (0xAB840064u)

#define CapSense_1_OK_SNS_CLK_SOURCE_VALUE                  (CapSense_1_dsRam.wdgtList.ok.snsClkSource)
#define CapSense_1_OK_SNS_CLK_SOURCE_OFFSET                 (102u)
#define CapSense_1_OK_SNS_CLK_SOURCE_SIZE                   (1u)
#define CapSense_1_OK_SNS_CLK_SOURCE_PARAM_ID               (0x44840066u)

#define CapSense_1_SEEKPLUS_RESOLUTION_VALUE                (CapSense_1_dsRam.wdgtList.seekplus.resolution)
#define CapSense_1_SEEKPLUS_RESOLUTION_OFFSET               (104u)
#define CapSense_1_SEEKPLUS_RESOLUTION_SIZE                 (2u)
#define CapSense_1_SEEKPLUS_RESOLUTION_PARAM_ID             (0xAB850068u)

#define CapSense_1_SEEKPLUS_FINGER_TH_VALUE                 (CapSense_1_dsRam.wdgtList.seekplus.fingerTh)
#define CapSense_1_SEEKPLUS_FINGER_TH_OFFSET                (106u)
#define CapSense_1_SEEKPLUS_FINGER_TH_SIZE                  (2u)
#define CapSense_1_SEEKPLUS_FINGER_TH_PARAM_ID              (0xA785006Au)

#define CapSense_1_SEEKPLUS_NOISE_TH_VALUE                  (CapSense_1_dsRam.wdgtList.seekplus.noiseTh)
#define CapSense_1_SEEKPLUS_NOISE_TH_OFFSET                 (108u)
#define CapSense_1_SEEKPLUS_NOISE_TH_SIZE                   (1u)
#define CapSense_1_SEEKPLUS_NOISE_TH_PARAM_ID               (0x6285006Cu)

#define CapSense_1_SEEKPLUS_NNOISE_TH_VALUE                 (CapSense_1_dsRam.wdgtList.seekplus.nNoiseTh)
#define CapSense_1_SEEKPLUS_NNOISE_TH_OFFSET                (109u)
#define CapSense_1_SEEKPLUS_NNOISE_TH_SIZE                  (1u)
#define CapSense_1_SEEKPLUS_NNOISE_TH_PARAM_ID              (0x6485006Du)

#define CapSense_1_SEEKPLUS_HYSTERESIS_VALUE                (CapSense_1_dsRam.wdgtList.seekplus.hysteresis)
#define CapSense_1_SEEKPLUS_HYSTERESIS_OFFSET               (110u)
#define CapSense_1_SEEKPLUS_HYSTERESIS_SIZE                 (1u)
#define CapSense_1_SEEKPLUS_HYSTERESIS_PARAM_ID             (0x6E85006Eu)

#define CapSense_1_SEEKPLUS_ON_DEBOUNCE_VALUE               (CapSense_1_dsRam.wdgtList.seekplus.onDebounce)
#define CapSense_1_SEEKPLUS_ON_DEBOUNCE_OFFSET              (111u)
#define CapSense_1_SEEKPLUS_ON_DEBOUNCE_SIZE                (1u)
#define CapSense_1_SEEKPLUS_ON_DEBOUNCE_PARAM_ID            (0x6885006Fu)

#define CapSense_1_SEEKPLUS_LOW_BSLN_RST_VALUE              (CapSense_1_dsRam.wdgtList.seekplus.lowBslnRst)
#define CapSense_1_SEEKPLUS_LOW_BSLN_RST_OFFSET             (112u)
#define CapSense_1_SEEKPLUS_LOW_BSLN_RST_SIZE               (1u)
#define CapSense_1_SEEKPLUS_LOW_BSLN_RST_PARAM_ID           (0x64850070u)

#define CapSense_1_SEEKPLUS_IDAC_MOD0_VALUE                 (CapSense_1_dsRam.wdgtList.seekplus.idacMod[0u])
#define CapSense_1_SEEKPLUS_IDAC_MOD0_OFFSET                (113u)
#define CapSense_1_SEEKPLUS_IDAC_MOD0_SIZE                  (1u)
#define CapSense_1_SEEKPLUS_IDAC_MOD0_PARAM_ID              (0x6F050071u)

#define CapSense_1_SEEKPLUS_SNS_CLK_VALUE                   (CapSense_1_dsRam.wdgtList.seekplus.snsClk)
#define CapSense_1_SEEKPLUS_SNS_CLK_OFFSET                  (114u)
#define CapSense_1_SEEKPLUS_SNS_CLK_SIZE                    (2u)
#define CapSense_1_SEEKPLUS_SNS_CLK_PARAM_ID                (0xA0850072u)

#define CapSense_1_SEEKPLUS_SNS_CLK_SOURCE_VALUE            (CapSense_1_dsRam.wdgtList.seekplus.snsClkSource)
#define CapSense_1_SEEKPLUS_SNS_CLK_SOURCE_OFFSET           (116u)
#define CapSense_1_SEEKPLUS_SNS_CLK_SOURCE_SIZE             (1u)
#define CapSense_1_SEEKPLUS_SNS_CLK_SOURCE_PARAM_ID         (0x4E850074u)

#define CapSense_1_RETURNS_SNS0_RAW0_VALUE                  (CapSense_1_dsRam.snsList.returns[0u].raw[0u])
#define CapSense_1_RETURNS_SNS0_RAW0_OFFSET                 (118u)
#define CapSense_1_RETURNS_SNS0_RAW0_SIZE                   (2u)
#define CapSense_1_RETURNS_SNS0_RAW0_PARAM_ID               (0x88000076u)

#define CapSense_1_RETURNS_SNS0_BSLN0_VALUE                 (CapSense_1_dsRam.snsList.returns[0u].bsln[0u])
#define CapSense_1_RETURNS_SNS0_BSLN0_OFFSET                (120u)
#define CapSense_1_RETURNS_SNS0_BSLN0_SIZE                  (2u)
#define CapSense_1_RETURNS_SNS0_BSLN0_PARAM_ID              (0x87000078u)

#define CapSense_1_RETURNS_SNS0_BSLN_EXT0_VALUE             (CapSense_1_dsRam.snsList.returns[0u].bslnExt[0u])
#define CapSense_1_RETURNS_SNS0_BSLN_EXT0_OFFSET            (122u)
#define CapSense_1_RETURNS_SNS0_BSLN_EXT0_SIZE              (1u)
#define CapSense_1_RETURNS_SNS0_BSLN_EXT0_PARAM_ID          (0x4300007Au)

#define CapSense_1_RETURNS_SNS0_DIFF_VALUE                  (CapSense_1_dsRam.snsList.returns[0u].diff)
#define CapSense_1_RETURNS_SNS0_DIFF_OFFSET                 (124u)
#define CapSense_1_RETURNS_SNS0_DIFF_SIZE                   (2u)
#define CapSense_1_RETURNS_SNS0_DIFF_PARAM_ID               (0x8600007Cu)

#define CapSense_1_RETURNS_SNS0_NEG_BSLN_RST_CNT0_VALUE     (CapSense_1_dsRam.snsList.returns[0u].negBslnRstCnt[0u])
#define CapSense_1_RETURNS_SNS0_NEG_BSLN_RST_CNT0_OFFSET    (126u)
#define CapSense_1_RETURNS_SNS0_NEG_BSLN_RST_CNT0_SIZE      (1u)
#define CapSense_1_RETURNS_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID  (0x4200007Eu)

#define CapSense_1_RETURNS_SNS0_IDAC_COMP0_VALUE            (CapSense_1_dsRam.snsList.returns[0u].idacComp[0u])
#define CapSense_1_RETURNS_SNS0_IDAC_COMP0_OFFSET           (127u)
#define CapSense_1_RETURNS_SNS0_IDAC_COMP0_SIZE             (1u)
#define CapSense_1_RETURNS_SNS0_IDAC_COMP0_PARAM_ID         (0x6F00007Fu)

#define CapSense_1_SOURCE_SNS0_RAW0_VALUE                   (CapSense_1_dsRam.snsList.source[0u].raw[0u])
#define CapSense_1_SOURCE_SNS0_RAW0_OFFSET                  (128u)
#define CapSense_1_SOURCE_SNS0_RAW0_SIZE                    (2u)
#define CapSense_1_SOURCE_SNS0_RAW0_PARAM_ID                (0x84000080u)

#define CapSense_1_SOURCE_SNS0_BSLN0_VALUE                  (CapSense_1_dsRam.snsList.source[0u].bsln[0u])
#define CapSense_1_SOURCE_SNS0_BSLN0_OFFSET                 (130u)
#define CapSense_1_SOURCE_SNS0_BSLN0_SIZE                   (2u)
#define CapSense_1_SOURCE_SNS0_BSLN0_PARAM_ID               (0x88000082u)

#define CapSense_1_SOURCE_SNS0_BSLN_EXT0_VALUE              (CapSense_1_dsRam.snsList.source[0u].bslnExt[0u])
#define CapSense_1_SOURCE_SNS0_BSLN_EXT0_OFFSET             (132u)
#define CapSense_1_SOURCE_SNS0_BSLN_EXT0_SIZE               (1u)
#define CapSense_1_SOURCE_SNS0_BSLN_EXT0_PARAM_ID           (0x4D000084u)

#define CapSense_1_SOURCE_SNS0_DIFF_VALUE                   (CapSense_1_dsRam.snsList.source[0u].diff)
#define CapSense_1_SOURCE_SNS0_DIFF_OFFSET                  (134u)
#define CapSense_1_SOURCE_SNS0_DIFF_SIZE                    (2u)
#define CapSense_1_SOURCE_SNS0_DIFF_PARAM_ID                (0x89000086u)

#define CapSense_1_SOURCE_SNS0_NEG_BSLN_RST_CNT0_VALUE      (CapSense_1_dsRam.snsList.source[0u].negBslnRstCnt[0u])
#define CapSense_1_SOURCE_SNS0_NEG_BSLN_RST_CNT0_OFFSET     (136u)
#define CapSense_1_SOURCE_SNS0_NEG_BSLN_RST_CNT0_SIZE       (1u)
#define CapSense_1_SOURCE_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID   (0x4E000088u)

#define CapSense_1_SOURCE_SNS0_IDAC_COMP0_VALUE             (CapSense_1_dsRam.snsList.source[0u].idacComp[0u])
#define CapSense_1_SOURCE_SNS0_IDAC_COMP0_OFFSET            (137u)
#define CapSense_1_SOURCE_SNS0_IDAC_COMP0_SIZE              (1u)
#define CapSense_1_SOURCE_SNS0_IDAC_COMP0_PARAM_ID          (0x63000089u)

#define CapSense_1_DOWN_SNS0_RAW0_VALUE                     (CapSense_1_dsRam.snsList.down[0u].raw[0u])
#define CapSense_1_DOWN_SNS0_RAW0_OFFSET                    (138u)
#define CapSense_1_DOWN_SNS0_RAW0_SIZE                      (2u)
#define CapSense_1_DOWN_SNS0_RAW0_PARAM_ID                  (0x8A00008Au)

#define CapSense_1_DOWN_SNS0_BSLN0_VALUE                    (CapSense_1_dsRam.snsList.down[0u].bsln[0u])
#define CapSense_1_DOWN_SNS0_BSLN0_OFFSET                   (140u)
#define CapSense_1_DOWN_SNS0_BSLN0_SIZE                     (2u)
#define CapSense_1_DOWN_SNS0_BSLN0_PARAM_ID                 (0x8700008Cu)

#define CapSense_1_DOWN_SNS0_BSLN_EXT0_VALUE                (CapSense_1_dsRam.snsList.down[0u].bslnExt[0u])
#define CapSense_1_DOWN_SNS0_BSLN_EXT0_OFFSET               (142u)
#define CapSense_1_DOWN_SNS0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_1_DOWN_SNS0_BSLN_EXT0_PARAM_ID             (0x4300008Eu)

#define CapSense_1_DOWN_SNS0_DIFF_VALUE                     (CapSense_1_dsRam.snsList.down[0u].diff)
#define CapSense_1_DOWN_SNS0_DIFF_OFFSET                    (144u)
#define CapSense_1_DOWN_SNS0_DIFF_SIZE                      (2u)
#define CapSense_1_DOWN_SNS0_DIFF_PARAM_ID                  (0x81000090u)

#define CapSense_1_DOWN_SNS0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_1_dsRam.snsList.down[0u].negBslnRstCnt[0u])
#define CapSense_1_DOWN_SNS0_NEG_BSLN_RST_CNT0_OFFSET       (146u)
#define CapSense_1_DOWN_SNS0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_1_DOWN_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x45000092u)

#define CapSense_1_DOWN_SNS0_IDAC_COMP0_VALUE               (CapSense_1_dsRam.snsList.down[0u].idacComp[0u])
#define CapSense_1_DOWN_SNS0_IDAC_COMP0_OFFSET              (147u)
#define CapSense_1_DOWN_SNS0_IDAC_COMP0_SIZE                (1u)
#define CapSense_1_DOWN_SNS0_IDAC_COMP0_PARAM_ID            (0x68000093u)

#define CapSense_1_SEEKREDUCE_SNS0_RAW0_VALUE               (CapSense_1_dsRam.snsList.seekreduce[0u].raw[0u])
#define CapSense_1_SEEKREDUCE_SNS0_RAW0_OFFSET              (148u)
#define CapSense_1_SEEKREDUCE_SNS0_RAW0_SIZE                (2u)
#define CapSense_1_SEEKREDUCE_SNS0_RAW0_PARAM_ID            (0x80000094u)

#define CapSense_1_SEEKREDUCE_SNS0_BSLN0_VALUE              (CapSense_1_dsRam.snsList.seekreduce[0u].bsln[0u])
#define CapSense_1_SEEKREDUCE_SNS0_BSLN0_OFFSET             (150u)
#define CapSense_1_SEEKREDUCE_SNS0_BSLN0_SIZE               (2u)
#define CapSense_1_SEEKREDUCE_SNS0_BSLN0_PARAM_ID           (0x8C000096u)

#define CapSense_1_SEEKREDUCE_SNS0_BSLN_EXT0_VALUE          (CapSense_1_dsRam.snsList.seekreduce[0u].bslnExt[0u])
#define CapSense_1_SEEKREDUCE_SNS0_BSLN_EXT0_OFFSET         (152u)
#define CapSense_1_SEEKREDUCE_SNS0_BSLN_EXT0_SIZE           (1u)
#define CapSense_1_SEEKREDUCE_SNS0_BSLN_EXT0_PARAM_ID       (0x4B000098u)

#define CapSense_1_SEEKREDUCE_SNS0_DIFF_VALUE               (CapSense_1_dsRam.snsList.seekreduce[0u].diff)
#define CapSense_1_SEEKREDUCE_SNS0_DIFF_OFFSET              (154u)
#define CapSense_1_SEEKREDUCE_SNS0_DIFF_SIZE                (2u)
#define CapSense_1_SEEKREDUCE_SNS0_DIFF_PARAM_ID            (0x8F00009Au)

#define CapSense_1_SEEKREDUCE_SNS0_NEG_BSLN_RST_CNT0_VALUE  (CapSense_1_dsRam.snsList.seekreduce[0u].negBslnRstCnt[0u])
#define CapSense_1_SEEKREDUCE_SNS0_NEG_BSLN_RST_CNT0_OFFSET (156u)
#define CapSense_1_SEEKREDUCE_SNS0_NEG_BSLN_RST_CNT0_SIZE   (1u)
#define CapSense_1_SEEKREDUCE_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID (0x4A00009Cu)

#define CapSense_1_SEEKREDUCE_SNS0_IDAC_COMP0_VALUE         (CapSense_1_dsRam.snsList.seekreduce[0u].idacComp[0u])
#define CapSense_1_SEEKREDUCE_SNS0_IDAC_COMP0_OFFSET        (157u)
#define CapSense_1_SEEKREDUCE_SNS0_IDAC_COMP0_SIZE          (1u)
#define CapSense_1_SEEKREDUCE_SNS0_IDAC_COMP0_PARAM_ID      (0x6700009Du)

#define CapSense_1_OK_SNS0_RAW0_VALUE                       (CapSense_1_dsRam.snsList.ok[0u].raw[0u])
#define CapSense_1_OK_SNS0_RAW0_OFFSET                      (158u)
#define CapSense_1_OK_SNS0_RAW0_SIZE                        (2u)
#define CapSense_1_OK_SNS0_RAW0_PARAM_ID                    (0x8E00009Eu)

#define CapSense_1_OK_SNS0_BSLN0_VALUE                      (CapSense_1_dsRam.snsList.ok[0u].bsln[0u])
#define CapSense_1_OK_SNS0_BSLN0_OFFSET                     (160u)
#define CapSense_1_OK_SNS0_BSLN0_SIZE                       (2u)
#define CapSense_1_OK_SNS0_BSLN0_PARAM_ID                   (0x8E0000A0u)

#define CapSense_1_OK_SNS0_BSLN_EXT0_VALUE                  (CapSense_1_dsRam.snsList.ok[0u].bslnExt[0u])
#define CapSense_1_OK_SNS0_BSLN_EXT0_OFFSET                 (162u)
#define CapSense_1_OK_SNS0_BSLN_EXT0_SIZE                   (1u)
#define CapSense_1_OK_SNS0_BSLN_EXT0_PARAM_ID               (0x4A0000A2u)

#define CapSense_1_OK_SNS0_DIFF_VALUE                       (CapSense_1_dsRam.snsList.ok[0u].diff)
#define CapSense_1_OK_SNS0_DIFF_OFFSET                      (164u)
#define CapSense_1_OK_SNS0_DIFF_SIZE                        (2u)
#define CapSense_1_OK_SNS0_DIFF_PARAM_ID                    (0x8F0000A4u)

#define CapSense_1_OK_SNS0_NEG_BSLN_RST_CNT0_VALUE          (CapSense_1_dsRam.snsList.ok[0u].negBslnRstCnt[0u])
#define CapSense_1_OK_SNS0_NEG_BSLN_RST_CNT0_OFFSET         (166u)
#define CapSense_1_OK_SNS0_NEG_BSLN_RST_CNT0_SIZE           (1u)
#define CapSense_1_OK_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID       (0x4B0000A6u)

#define CapSense_1_OK_SNS0_IDAC_COMP0_VALUE                 (CapSense_1_dsRam.snsList.ok[0u].idacComp[0u])
#define CapSense_1_OK_SNS0_IDAC_COMP0_OFFSET                (167u)
#define CapSense_1_OK_SNS0_IDAC_COMP0_SIZE                  (1u)
#define CapSense_1_OK_SNS0_IDAC_COMP0_PARAM_ID              (0x660000A7u)

#define CapSense_1_SEEKPLUS_SNS0_RAW0_VALUE                 (CapSense_1_dsRam.snsList.seekplus[0u].raw[0u])
#define CapSense_1_SEEKPLUS_SNS0_RAW0_OFFSET                (168u)
#define CapSense_1_SEEKPLUS_SNS0_RAW0_SIZE                  (2u)
#define CapSense_1_SEEKPLUS_SNS0_RAW0_PARAM_ID              (0x8C0000A8u)

#define CapSense_1_SEEKPLUS_SNS0_BSLN0_VALUE                (CapSense_1_dsRam.snsList.seekplus[0u].bsln[0u])
#define CapSense_1_SEEKPLUS_SNS0_BSLN0_OFFSET               (170u)
#define CapSense_1_SEEKPLUS_SNS0_BSLN0_SIZE                 (2u)
#define CapSense_1_SEEKPLUS_SNS0_BSLN0_PARAM_ID             (0x800000AAu)

#define CapSense_1_SEEKPLUS_SNS0_BSLN_EXT0_VALUE            (CapSense_1_dsRam.snsList.seekplus[0u].bslnExt[0u])
#define CapSense_1_SEEKPLUS_SNS0_BSLN_EXT0_OFFSET           (172u)
#define CapSense_1_SEEKPLUS_SNS0_BSLN_EXT0_SIZE             (1u)
#define CapSense_1_SEEKPLUS_SNS0_BSLN_EXT0_PARAM_ID         (0x450000ACu)

#define CapSense_1_SEEKPLUS_SNS0_DIFF_VALUE                 (CapSense_1_dsRam.snsList.seekplus[0u].diff)
#define CapSense_1_SEEKPLUS_SNS0_DIFF_OFFSET                (174u)
#define CapSense_1_SEEKPLUS_SNS0_DIFF_SIZE                  (2u)
#define CapSense_1_SEEKPLUS_SNS0_DIFF_PARAM_ID              (0x810000AEu)

#define CapSense_1_SEEKPLUS_SNS0_NEG_BSLN_RST_CNT0_VALUE    (CapSense_1_dsRam.snsList.seekplus[0u].negBslnRstCnt[0u])
#define CapSense_1_SEEKPLUS_SNS0_NEG_BSLN_RST_CNT0_OFFSET   (176u)
#define CapSense_1_SEEKPLUS_SNS0_NEG_BSLN_RST_CNT0_SIZE     (1u)
#define CapSense_1_SEEKPLUS_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID (0x430000B0u)

#define CapSense_1_SEEKPLUS_SNS0_IDAC_COMP0_VALUE           (CapSense_1_dsRam.snsList.seekplus[0u].idacComp[0u])
#define CapSense_1_SEEKPLUS_SNS0_IDAC_COMP0_OFFSET          (177u)
#define CapSense_1_SEEKPLUS_SNS0_IDAC_COMP0_SIZE            (1u)
#define CapSense_1_SEEKPLUS_SNS0_IDAC_COMP0_PARAM_ID        (0x6E0000B1u)

#define CapSense_1_SNR_TEST_WIDGET_ID_VALUE                 (CapSense_1_dsRam.snrTestWidgetId)
#define CapSense_1_SNR_TEST_WIDGET_ID_OFFSET                (178u)
#define CapSense_1_SNR_TEST_WIDGET_ID_SIZE                  (1u)
#define CapSense_1_SNR_TEST_WIDGET_ID_PARAM_ID              (0x640000B2u)

#define CapSense_1_SNR_TEST_SENSOR_ID_VALUE                 (CapSense_1_dsRam.snrTestSensorId)
#define CapSense_1_SNR_TEST_SENSOR_ID_OFFSET                (179u)
#define CapSense_1_SNR_TEST_SENSOR_ID_SIZE                  (1u)
#define CapSense_1_SNR_TEST_SENSOR_ID_PARAM_ID              (0x620000B3u)

#define CapSense_1_SNR_TEST_SCAN_COUNTER_VALUE              (CapSense_1_dsRam.snrTestScanCounter)
#define CapSense_1_SNR_TEST_SCAN_COUNTER_OFFSET             (180u)
#define CapSense_1_SNR_TEST_SCAN_COUNTER_SIZE               (2u)
#define CapSense_1_SNR_TEST_SCAN_COUNTER_PARAM_ID           (0x8A0000B4u)

#define CapSense_1_SNR_TEST_RAW_COUNT0_VALUE                (CapSense_1_dsRam.snrTestRawCount[0u])
#define CapSense_1_SNR_TEST_RAW_COUNT0_OFFSET               (182u)
#define CapSense_1_SNR_TEST_RAW_COUNT0_SIZE                 (2u)
#define CapSense_1_SNR_TEST_RAW_COUNT0_PARAM_ID             (0x860000B6u)


/*****************************************************************************/
/* Flash Data structure register definitions                                 */
/*****************************************************************************/
#define CapSense_1_RETURNS_PTR2SNS_FLASH_VALUE              (CapSense_1_dsFlash.wdgtArray[0].ptr2SnsFlash)
#define CapSense_1_RETURNS_PTR2SNS_FLASH_OFFSET             (0u)
#define CapSense_1_RETURNS_PTR2SNS_FLASH_SIZE               (4u)
#define CapSense_1_RETURNS_PTR2SNS_FLASH_PARAM_ID           (0xD1000000u)

#define CapSense_1_RETURNS_PTR2WD_RAM_VALUE                 (CapSense_1_dsFlash.wdgtArray[0].ptr2WdgtRam)
#define CapSense_1_RETURNS_PTR2WD_RAM_OFFSET                (4u)
#define CapSense_1_RETURNS_PTR2WD_RAM_SIZE                  (4u)
#define CapSense_1_RETURNS_PTR2WD_RAM_PARAM_ID              (0xD0000004u)

#define CapSense_1_RETURNS_PTR2SNS_RAM_VALUE                (CapSense_1_dsFlash.wdgtArray[0].ptr2SnsRam)
#define CapSense_1_RETURNS_PTR2SNS_RAM_OFFSET               (8u)
#define CapSense_1_RETURNS_PTR2SNS_RAM_SIZE                 (4u)
#define CapSense_1_RETURNS_PTR2SNS_RAM_PARAM_ID             (0xD3000008u)

#define CapSense_1_RETURNS_PTR2FLTR_HISTORY_VALUE           (CapSense_1_dsFlash.wdgtArray[0].ptr2FltrHistory)
#define CapSense_1_RETURNS_PTR2FLTR_HISTORY_OFFSET          (12u)
#define CapSense_1_RETURNS_PTR2FLTR_HISTORY_SIZE            (4u)
#define CapSense_1_RETURNS_PTR2FLTR_HISTORY_PARAM_ID        (0xD200000Cu)

#define CapSense_1_RETURNS_PTR2DEBOUNCE_VALUE               (CapSense_1_dsFlash.wdgtArray[0].ptr2DebounceArr)
#define CapSense_1_RETURNS_PTR2DEBOUNCE_OFFSET              (16u)
#define CapSense_1_RETURNS_PTR2DEBOUNCE_SIZE                (4u)
#define CapSense_1_RETURNS_PTR2DEBOUNCE_PARAM_ID            (0xD4000010u)

#define CapSense_1_RETURNS_STATIC_CONFIG_VALUE              (CapSense_1_dsFlash.wdgtArray[0].staticConfig)
#define CapSense_1_RETURNS_STATIC_CONFIG_OFFSET             (20u)
#define CapSense_1_RETURNS_STATIC_CONFIG_SIZE               (4u)
#define CapSense_1_RETURNS_STATIC_CONFIG_PARAM_ID           (0xD5000014u)

#define CapSense_1_RETURNS_TOTAL_NUM_SNS_VALUE              (CapSense_1_dsFlash.wdgtArray[0].totalNumSns)
#define CapSense_1_RETURNS_TOTAL_NUM_SNS_OFFSET             (24u)
#define CapSense_1_RETURNS_TOTAL_NUM_SNS_SIZE               (2u)
#define CapSense_1_RETURNS_TOTAL_NUM_SNS_PARAM_ID           (0x99000018u)

#define CapSense_1_RETURNS_TYPE_VALUE                       (CapSense_1_dsFlash.wdgtArray[0].wdgtType)
#define CapSense_1_RETURNS_TYPE_OFFSET                      (26u)
#define CapSense_1_RETURNS_TYPE_SIZE                        (1u)
#define CapSense_1_RETURNS_TYPE_PARAM_ID                    (0x5D00001Au)

#define CapSense_1_RETURNS_NUM_COLS_VALUE                   (CapSense_1_dsFlash.wdgtArray[0].numCols)
#define CapSense_1_RETURNS_NUM_COLS_OFFSET                  (27u)
#define CapSense_1_RETURNS_NUM_COLS_SIZE                    (1u)
#define CapSense_1_RETURNS_NUM_COLS_PARAM_ID                (0x5B00001Bu)

#define CapSense_1_SOURCE_PTR2SNS_FLASH_VALUE               (CapSense_1_dsFlash.wdgtArray[1].ptr2SnsFlash)
#define CapSense_1_SOURCE_PTR2SNS_FLASH_OFFSET              (28u)
#define CapSense_1_SOURCE_PTR2SNS_FLASH_SIZE                (4u)
#define CapSense_1_SOURCE_PTR2SNS_FLASH_PARAM_ID            (0xD401001Cu)

#define CapSense_1_SOURCE_PTR2WD_RAM_VALUE                  (CapSense_1_dsFlash.wdgtArray[1].ptr2WdgtRam)
#define CapSense_1_SOURCE_PTR2WD_RAM_OFFSET                 (32u)
#define CapSense_1_SOURCE_PTR2WD_RAM_SIZE                   (4u)
#define CapSense_1_SOURCE_PTR2WD_RAM_PARAM_ID               (0xD8010020u)

#define CapSense_1_SOURCE_PTR2SNS_RAM_VALUE                 (CapSense_1_dsFlash.wdgtArray[1].ptr2SnsRam)
#define CapSense_1_SOURCE_PTR2SNS_RAM_OFFSET                (36u)
#define CapSense_1_SOURCE_PTR2SNS_RAM_SIZE                  (4u)
#define CapSense_1_SOURCE_PTR2SNS_RAM_PARAM_ID              (0xD9010024u)

#define CapSense_1_SOURCE_PTR2FLTR_HISTORY_VALUE            (CapSense_1_dsFlash.wdgtArray[1].ptr2FltrHistory)
#define CapSense_1_SOURCE_PTR2FLTR_HISTORY_OFFSET           (40u)
#define CapSense_1_SOURCE_PTR2FLTR_HISTORY_SIZE             (4u)
#define CapSense_1_SOURCE_PTR2FLTR_HISTORY_PARAM_ID         (0xDA010028u)

#define CapSense_1_SOURCE_PTR2DEBOUNCE_VALUE                (CapSense_1_dsFlash.wdgtArray[1].ptr2DebounceArr)
#define CapSense_1_SOURCE_PTR2DEBOUNCE_OFFSET               (44u)
#define CapSense_1_SOURCE_PTR2DEBOUNCE_SIZE                 (4u)
#define CapSense_1_SOURCE_PTR2DEBOUNCE_PARAM_ID             (0xDB01002Cu)

#define CapSense_1_SOURCE_STATIC_CONFIG_VALUE               (CapSense_1_dsFlash.wdgtArray[1].staticConfig)
#define CapSense_1_SOURCE_STATIC_CONFIG_OFFSET              (48u)
#define CapSense_1_SOURCE_STATIC_CONFIG_SIZE                (4u)
#define CapSense_1_SOURCE_STATIC_CONFIG_PARAM_ID            (0xDD010030u)

#define CapSense_1_SOURCE_TOTAL_NUM_SNS_VALUE               (CapSense_1_dsFlash.wdgtArray[1].totalNumSns)
#define CapSense_1_SOURCE_TOTAL_NUM_SNS_OFFSET              (52u)
#define CapSense_1_SOURCE_TOTAL_NUM_SNS_SIZE                (2u)
#define CapSense_1_SOURCE_TOTAL_NUM_SNS_PARAM_ID            (0x93010034u)

#define CapSense_1_SOURCE_TYPE_VALUE                        (CapSense_1_dsFlash.wdgtArray[1].wdgtType)
#define CapSense_1_SOURCE_TYPE_OFFSET                       (54u)
#define CapSense_1_SOURCE_TYPE_SIZE                         (1u)
#define CapSense_1_SOURCE_TYPE_PARAM_ID                     (0x57010036u)

#define CapSense_1_SOURCE_NUM_COLS_VALUE                    (CapSense_1_dsFlash.wdgtArray[1].numCols)
#define CapSense_1_SOURCE_NUM_COLS_OFFSET                   (55u)
#define CapSense_1_SOURCE_NUM_COLS_SIZE                     (1u)
#define CapSense_1_SOURCE_NUM_COLS_PARAM_ID                 (0x51010037u)

#define CapSense_1_DOWN_PTR2SNS_FLASH_VALUE                 (CapSense_1_dsFlash.wdgtArray[2].ptr2SnsFlash)
#define CapSense_1_DOWN_PTR2SNS_FLASH_OFFSET                (56u)
#define CapSense_1_DOWN_PTR2SNS_FLASH_SIZE                  (4u)
#define CapSense_1_DOWN_PTR2SNS_FLASH_PARAM_ID              (0xDA020038u)

#define CapSense_1_DOWN_PTR2WD_RAM_VALUE                    (CapSense_1_dsFlash.wdgtArray[2].ptr2WdgtRam)
#define CapSense_1_DOWN_PTR2WD_RAM_OFFSET                   (60u)
#define CapSense_1_DOWN_PTR2WD_RAM_SIZE                     (4u)
#define CapSense_1_DOWN_PTR2WD_RAM_PARAM_ID                 (0xDB02003Cu)

#define CapSense_1_DOWN_PTR2SNS_RAM_VALUE                   (CapSense_1_dsFlash.wdgtArray[2].ptr2SnsRam)
#define CapSense_1_DOWN_PTR2SNS_RAM_OFFSET                  (64u)
#define CapSense_1_DOWN_PTR2SNS_RAM_SIZE                    (4u)
#define CapSense_1_DOWN_PTR2SNS_RAM_PARAM_ID                (0xDA020040u)

#define CapSense_1_DOWN_PTR2FLTR_HISTORY_VALUE              (CapSense_1_dsFlash.wdgtArray[2].ptr2FltrHistory)
#define CapSense_1_DOWN_PTR2FLTR_HISTORY_OFFSET             (68u)
#define CapSense_1_DOWN_PTR2FLTR_HISTORY_SIZE               (4u)
#define CapSense_1_DOWN_PTR2FLTR_HISTORY_PARAM_ID           (0xDB020044u)

#define CapSense_1_DOWN_PTR2DEBOUNCE_VALUE                  (CapSense_1_dsFlash.wdgtArray[2].ptr2DebounceArr)
#define CapSense_1_DOWN_PTR2DEBOUNCE_OFFSET                 (72u)
#define CapSense_1_DOWN_PTR2DEBOUNCE_SIZE                   (4u)
#define CapSense_1_DOWN_PTR2DEBOUNCE_PARAM_ID               (0xD8020048u)

#define CapSense_1_DOWN_STATIC_CONFIG_VALUE                 (CapSense_1_dsFlash.wdgtArray[2].staticConfig)
#define CapSense_1_DOWN_STATIC_CONFIG_OFFSET                (76u)
#define CapSense_1_DOWN_STATIC_CONFIG_SIZE                  (4u)
#define CapSense_1_DOWN_STATIC_CONFIG_PARAM_ID              (0xD902004Cu)

#define CapSense_1_DOWN_TOTAL_NUM_SNS_VALUE                 (CapSense_1_dsFlash.wdgtArray[2].totalNumSns)
#define CapSense_1_DOWN_TOTAL_NUM_SNS_OFFSET                (80u)
#define CapSense_1_DOWN_TOTAL_NUM_SNS_SIZE                  (2u)
#define CapSense_1_DOWN_TOTAL_NUM_SNS_PARAM_ID              (0x90020050u)

#define CapSense_1_DOWN_TYPE_VALUE                          (CapSense_1_dsFlash.wdgtArray[2].wdgtType)
#define CapSense_1_DOWN_TYPE_OFFSET                         (82u)
#define CapSense_1_DOWN_TYPE_SIZE                           (1u)
#define CapSense_1_DOWN_TYPE_PARAM_ID                       (0x54020052u)

#define CapSense_1_DOWN_NUM_COLS_VALUE                      (CapSense_1_dsFlash.wdgtArray[2].numCols)
#define CapSense_1_DOWN_NUM_COLS_OFFSET                     (83u)
#define CapSense_1_DOWN_NUM_COLS_SIZE                       (1u)
#define CapSense_1_DOWN_NUM_COLS_PARAM_ID                   (0x52020053u)

#define CapSense_1_SEEKREDUCE_PTR2SNS_FLASH_VALUE           (CapSense_1_dsFlash.wdgtArray[3].ptr2SnsFlash)
#define CapSense_1_SEEKREDUCE_PTR2SNS_FLASH_OFFSET          (84u)
#define CapSense_1_SEEKREDUCE_PTR2SNS_FLASH_SIZE            (4u)
#define CapSense_1_SEEKREDUCE_PTR2SNS_FLASH_PARAM_ID        (0xDD030054u)

#define CapSense_1_SEEKREDUCE_PTR2WD_RAM_VALUE              (CapSense_1_dsFlash.wdgtArray[3].ptr2WdgtRam)
#define CapSense_1_SEEKREDUCE_PTR2WD_RAM_OFFSET             (88u)
#define CapSense_1_SEEKREDUCE_PTR2WD_RAM_SIZE               (4u)
#define CapSense_1_SEEKREDUCE_PTR2WD_RAM_PARAM_ID           (0xDE030058u)

#define CapSense_1_SEEKREDUCE_PTR2SNS_RAM_VALUE             (CapSense_1_dsFlash.wdgtArray[3].ptr2SnsRam)
#define CapSense_1_SEEKREDUCE_PTR2SNS_RAM_OFFSET            (92u)
#define CapSense_1_SEEKREDUCE_PTR2SNS_RAM_SIZE              (4u)
#define CapSense_1_SEEKREDUCE_PTR2SNS_RAM_PARAM_ID          (0xDF03005Cu)

#define CapSense_1_SEEKREDUCE_PTR2FLTR_HISTORY_VALUE        (CapSense_1_dsFlash.wdgtArray[3].ptr2FltrHistory)
#define CapSense_1_SEEKREDUCE_PTR2FLTR_HISTORY_OFFSET       (96u)
#define CapSense_1_SEEKREDUCE_PTR2FLTR_HISTORY_SIZE         (4u)
#define CapSense_1_SEEKREDUCE_PTR2FLTR_HISTORY_PARAM_ID     (0xD3030060u)

#define CapSense_1_SEEKREDUCE_PTR2DEBOUNCE_VALUE            (CapSense_1_dsFlash.wdgtArray[3].ptr2DebounceArr)
#define CapSense_1_SEEKREDUCE_PTR2DEBOUNCE_OFFSET           (100u)
#define CapSense_1_SEEKREDUCE_PTR2DEBOUNCE_SIZE             (4u)
#define CapSense_1_SEEKREDUCE_PTR2DEBOUNCE_PARAM_ID         (0xD2030064u)

#define CapSense_1_SEEKREDUCE_STATIC_CONFIG_VALUE           (CapSense_1_dsFlash.wdgtArray[3].staticConfig)
#define CapSense_1_SEEKREDUCE_STATIC_CONFIG_OFFSET          (104u)
#define CapSense_1_SEEKREDUCE_STATIC_CONFIG_SIZE            (4u)
#define CapSense_1_SEEKREDUCE_STATIC_CONFIG_PARAM_ID        (0xD1030068u)

#define CapSense_1_SEEKREDUCE_TOTAL_NUM_SNS_VALUE           (CapSense_1_dsFlash.wdgtArray[3].totalNumSns)
#define CapSense_1_SEEKREDUCE_TOTAL_NUM_SNS_OFFSET          (108u)
#define CapSense_1_SEEKREDUCE_TOTAL_NUM_SNS_SIZE            (2u)
#define CapSense_1_SEEKREDUCE_TOTAL_NUM_SNS_PARAM_ID        (0x9F03006Cu)

#define CapSense_1_SEEKREDUCE_TYPE_VALUE                    (CapSense_1_dsFlash.wdgtArray[3].wdgtType)
#define CapSense_1_SEEKREDUCE_TYPE_OFFSET                   (110u)
#define CapSense_1_SEEKREDUCE_TYPE_SIZE                     (1u)
#define CapSense_1_SEEKREDUCE_TYPE_PARAM_ID                 (0x5B03006Eu)

#define CapSense_1_SEEKREDUCE_NUM_COLS_VALUE                (CapSense_1_dsFlash.wdgtArray[3].numCols)
#define CapSense_1_SEEKREDUCE_NUM_COLS_OFFSET               (111u)
#define CapSense_1_SEEKREDUCE_NUM_COLS_SIZE                 (1u)
#define CapSense_1_SEEKREDUCE_NUM_COLS_PARAM_ID             (0x5D03006Fu)

#define CapSense_1_OK_PTR2SNS_FLASH_VALUE                   (CapSense_1_dsFlash.wdgtArray[4].ptr2SnsFlash)
#define CapSense_1_OK_PTR2SNS_FLASH_OFFSET                  (112u)
#define CapSense_1_OK_PTR2SNS_FLASH_SIZE                    (4u)
#define CapSense_1_OK_PTR2SNS_FLASH_PARAM_ID                (0xDF040070u)

#define CapSense_1_OK_PTR2WD_RAM_VALUE                      (CapSense_1_dsFlash.wdgtArray[4].ptr2WdgtRam)
#define CapSense_1_OK_PTR2WD_RAM_OFFSET                     (116u)
#define CapSense_1_OK_PTR2WD_RAM_SIZE                       (4u)
#define CapSense_1_OK_PTR2WD_RAM_PARAM_ID                   (0xDE040074u)

#define CapSense_1_OK_PTR2SNS_RAM_VALUE                     (CapSense_1_dsFlash.wdgtArray[4].ptr2SnsRam)
#define CapSense_1_OK_PTR2SNS_RAM_OFFSET                    (120u)
#define CapSense_1_OK_PTR2SNS_RAM_SIZE                      (4u)
#define CapSense_1_OK_PTR2SNS_RAM_PARAM_ID                  (0xDD040078u)

#define CapSense_1_OK_PTR2FLTR_HISTORY_VALUE                (CapSense_1_dsFlash.wdgtArray[4].ptr2FltrHistory)
#define CapSense_1_OK_PTR2FLTR_HISTORY_OFFSET               (124u)
#define CapSense_1_OK_PTR2FLTR_HISTORY_SIZE                 (4u)
#define CapSense_1_OK_PTR2FLTR_HISTORY_PARAM_ID             (0xDC04007Cu)

#define CapSense_1_OK_PTR2DEBOUNCE_VALUE                    (CapSense_1_dsFlash.wdgtArray[4].ptr2DebounceArr)
#define CapSense_1_OK_PTR2DEBOUNCE_OFFSET                   (128u)
#define CapSense_1_OK_PTR2DEBOUNCE_SIZE                     (4u)
#define CapSense_1_OK_PTR2DEBOUNCE_PARAM_ID                 (0xDE040080u)

#define CapSense_1_OK_STATIC_CONFIG_VALUE                   (CapSense_1_dsFlash.wdgtArray[4].staticConfig)
#define CapSense_1_OK_STATIC_CONFIG_OFFSET                  (132u)
#define CapSense_1_OK_STATIC_CONFIG_SIZE                    (4u)
#define CapSense_1_OK_STATIC_CONFIG_PARAM_ID                (0xDF040084u)

#define CapSense_1_OK_TOTAL_NUM_SNS_VALUE                   (CapSense_1_dsFlash.wdgtArray[4].totalNumSns)
#define CapSense_1_OK_TOTAL_NUM_SNS_OFFSET                  (136u)
#define CapSense_1_OK_TOTAL_NUM_SNS_SIZE                    (2u)
#define CapSense_1_OK_TOTAL_NUM_SNS_PARAM_ID                (0x93040088u)

#define CapSense_1_OK_TYPE_VALUE                            (CapSense_1_dsFlash.wdgtArray[4].wdgtType)
#define CapSense_1_OK_TYPE_OFFSET                           (138u)
#define CapSense_1_OK_TYPE_SIZE                             (1u)
#define CapSense_1_OK_TYPE_PARAM_ID                         (0x5704008Au)

#define CapSense_1_OK_NUM_COLS_VALUE                        (CapSense_1_dsFlash.wdgtArray[4].numCols)
#define CapSense_1_OK_NUM_COLS_OFFSET                       (139u)
#define CapSense_1_OK_NUM_COLS_SIZE                         (1u)
#define CapSense_1_OK_NUM_COLS_PARAM_ID                     (0x5104008Bu)

#define CapSense_1_SEEKPLUS_PTR2SNS_FLASH_VALUE             (CapSense_1_dsFlash.wdgtArray[5].ptr2SnsFlash)
#define CapSense_1_SEEKPLUS_PTR2SNS_FLASH_OFFSET            (140u)
#define CapSense_1_SEEKPLUS_PTR2SNS_FLASH_SIZE              (4u)
#define CapSense_1_SEEKPLUS_PTR2SNS_FLASH_PARAM_ID          (0xDE05008Cu)

#define CapSense_1_SEEKPLUS_PTR2WD_RAM_VALUE                (CapSense_1_dsFlash.wdgtArray[5].ptr2WdgtRam)
#define CapSense_1_SEEKPLUS_PTR2WD_RAM_OFFSET               (144u)
#define CapSense_1_SEEKPLUS_PTR2WD_RAM_SIZE                 (4u)
#define CapSense_1_SEEKPLUS_PTR2WD_RAM_PARAM_ID             (0xD8050090u)

#define CapSense_1_SEEKPLUS_PTR2SNS_RAM_VALUE               (CapSense_1_dsFlash.wdgtArray[5].ptr2SnsRam)
#define CapSense_1_SEEKPLUS_PTR2SNS_RAM_OFFSET              (148u)
#define CapSense_1_SEEKPLUS_PTR2SNS_RAM_SIZE                (4u)
#define CapSense_1_SEEKPLUS_PTR2SNS_RAM_PARAM_ID            (0xD9050094u)

#define CapSense_1_SEEKPLUS_PTR2FLTR_HISTORY_VALUE          (CapSense_1_dsFlash.wdgtArray[5].ptr2FltrHistory)
#define CapSense_1_SEEKPLUS_PTR2FLTR_HISTORY_OFFSET         (152u)
#define CapSense_1_SEEKPLUS_PTR2FLTR_HISTORY_SIZE           (4u)
#define CapSense_1_SEEKPLUS_PTR2FLTR_HISTORY_PARAM_ID       (0xDA050098u)

#define CapSense_1_SEEKPLUS_PTR2DEBOUNCE_VALUE              (CapSense_1_dsFlash.wdgtArray[5].ptr2DebounceArr)
#define CapSense_1_SEEKPLUS_PTR2DEBOUNCE_OFFSET             (156u)
#define CapSense_1_SEEKPLUS_PTR2DEBOUNCE_SIZE               (4u)
#define CapSense_1_SEEKPLUS_PTR2DEBOUNCE_PARAM_ID           (0xDB05009Cu)

#define CapSense_1_SEEKPLUS_STATIC_CONFIG_VALUE             (CapSense_1_dsFlash.wdgtArray[5].staticConfig)
#define CapSense_1_SEEKPLUS_STATIC_CONFIG_OFFSET            (160u)
#define CapSense_1_SEEKPLUS_STATIC_CONFIG_SIZE              (4u)
#define CapSense_1_SEEKPLUS_STATIC_CONFIG_PARAM_ID          (0xD70500A0u)

#define CapSense_1_SEEKPLUS_TOTAL_NUM_SNS_VALUE             (CapSense_1_dsFlash.wdgtArray[5].totalNumSns)
#define CapSense_1_SEEKPLUS_TOTAL_NUM_SNS_OFFSET            (164u)
#define CapSense_1_SEEKPLUS_TOTAL_NUM_SNS_SIZE              (2u)
#define CapSense_1_SEEKPLUS_TOTAL_NUM_SNS_PARAM_ID          (0x990500A4u)

#define CapSense_1_SEEKPLUS_TYPE_VALUE                      (CapSense_1_dsFlash.wdgtArray[5].wdgtType)
#define CapSense_1_SEEKPLUS_TYPE_OFFSET                     (166u)
#define CapSense_1_SEEKPLUS_TYPE_SIZE                       (1u)
#define CapSense_1_SEEKPLUS_TYPE_PARAM_ID                   (0x5D0500A6u)

#define CapSense_1_SEEKPLUS_NUM_COLS_VALUE                  (CapSense_1_dsFlash.wdgtArray[5].numCols)
#define CapSense_1_SEEKPLUS_NUM_COLS_OFFSET                 (167u)
#define CapSense_1_SEEKPLUS_NUM_COLS_SIZE                   (1u)
#define CapSense_1_SEEKPLUS_NUM_COLS_PARAM_ID               (0x5B0500A7u)


#endif /* End CY_SENSE_CapSense_1_REGISTER_MAP_H */


/* [] END OF FILE */
