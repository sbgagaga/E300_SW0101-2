/***************************************************************************//**
* \file CapSense_RegisterMap.h
* \version 6.0
*
* \brief
*   This file provides the definitions for the Component data structure register.
*
* \see CapSense v6.0 Datasheet
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

#if !defined(CY_SENSE_CapSense_REGISTER_MAP_H)
#define CY_SENSE_CapSense_REGISTER_MAP_H

#include <cytypes.h>
#include "CapSense_Configuration.h"
#include "CapSense_Structure.h"

/*****************************************************************************/
/* RAM Data structure register definitions                                   */
/*****************************************************************************/
#define CapSense_CONFIG_ID_VALUE                            (CapSense_dsRam.configId)
#define CapSense_CONFIG_ID_OFFSET                           (0u)
#define CapSense_CONFIG_ID_SIZE                             (2u)
#define CapSense_CONFIG_ID_PARAM_ID                         (0x87000000u)

#define CapSense_DEVICE_ID_VALUE                            (CapSense_dsRam.deviceId)
#define CapSense_DEVICE_ID_OFFSET                           (2u)
#define CapSense_DEVICE_ID_SIZE                             (2u)
#define CapSense_DEVICE_ID_PARAM_ID                         (0x8B000002u)

#define CapSense_HW_CLOCK_VALUE                             (CapSense_dsRam.hwClock)
#define CapSense_HW_CLOCK_OFFSET                            (4u)
#define CapSense_HW_CLOCK_SIZE                              (2u)
#define CapSense_HW_CLOCK_PARAM_ID                          (0x86000004u)

#define CapSense_TUNER_CMD_VALUE                            (CapSense_dsRam.tunerCmd)
#define CapSense_TUNER_CMD_OFFSET                           (6u)
#define CapSense_TUNER_CMD_SIZE                             (2u)
#define CapSense_TUNER_CMD_PARAM_ID                         (0xA1000006u)

#define CapSense_SCAN_COUNTER_VALUE                         (CapSense_dsRam.scanCounter)
#define CapSense_SCAN_COUNTER_OFFSET                        (8u)
#define CapSense_SCAN_COUNTER_SIZE                          (2u)
#define CapSense_SCAN_COUNTER_PARAM_ID                      (0x85000008u)

#define CapSense_STATUS_VALUE                               (CapSense_dsRam.status)
#define CapSense_STATUS_OFFSET                              (12u)
#define CapSense_STATUS_SIZE                                (4u)
#define CapSense_STATUS_PARAM_ID                            (0xCB00000Cu)

#define CapSense_WDGT_ENABLE0_VALUE                         (CapSense_dsRam.wdgtEnable[0u])
#define CapSense_WDGT_ENABLE0_OFFSET                        (16u)
#define CapSense_WDGT_ENABLE0_SIZE                          (4u)
#define CapSense_WDGT_ENABLE0_PARAM_ID                      (0xE6000010u)

#define CapSense_WDGT_STATUS0_VALUE                         (CapSense_dsRam.wdgtStatus[0u])
#define CapSense_WDGT_STATUS0_OFFSET                        (20u)
#define CapSense_WDGT_STATUS0_SIZE                          (4u)
#define CapSense_WDGT_STATUS0_PARAM_ID                      (0xCC000014u)

#define CapSense_SNS_STATUS0_VALUE                          (CapSense_dsRam.snsStatus[0u])
#define CapSense_SNS_STATUS0_OFFSET                         (24u)
#define CapSense_SNS_STATUS0_SIZE                           (1u)
#define CapSense_SNS_STATUS0_PARAM_ID                       (0x48000018u)

#define CapSense_SNS_STATUS1_VALUE                          (CapSense_dsRam.snsStatus[1u])
#define CapSense_SNS_STATUS1_OFFSET                         (25u)
#define CapSense_SNS_STATUS1_SIZE                           (1u)
#define CapSense_SNS_STATUS1_PARAM_ID                       (0x4E000019u)

#define CapSense_SNS_STATUS2_VALUE                          (CapSense_dsRam.snsStatus[2u])
#define CapSense_SNS_STATUS2_OFFSET                         (26u)
#define CapSense_SNS_STATUS2_SIZE                           (1u)
#define CapSense_SNS_STATUS2_PARAM_ID                       (0x4400001Au)

#define CapSense_SNS_STATUS3_VALUE                          (CapSense_dsRam.snsStatus[3u])
#define CapSense_SNS_STATUS3_OFFSET                         (27u)
#define CapSense_SNS_STATUS3_SIZE                           (1u)
#define CapSense_SNS_STATUS3_PARAM_ID                       (0x4200001Bu)

#define CapSense_SNS_STATUS4_VALUE                          (CapSense_dsRam.snsStatus[4u])
#define CapSense_SNS_STATUS4_OFFSET                         (28u)
#define CapSense_SNS_STATUS4_SIZE                           (1u)
#define CapSense_SNS_STATUS4_PARAM_ID                       (0x4900001Cu)

#define CapSense_SNS_STATUS5_VALUE                          (CapSense_dsRam.snsStatus[5u])
#define CapSense_SNS_STATUS5_OFFSET                         (29u)
#define CapSense_SNS_STATUS5_SIZE                           (1u)
#define CapSense_SNS_STATUS5_PARAM_ID                       (0x4F00001Du)

#define CapSense_CSD0_CONFIG_VALUE                          (CapSense_dsRam.csd0Config)
#define CapSense_CSD0_CONFIG_OFFSET                         (30u)
#define CapSense_CSD0_CONFIG_SIZE                           (2u)
#define CapSense_CSD0_CONFIG_PARAM_ID                       (0xAB80001Eu)

#define CapSense_MOD_CSD_CLK_VALUE                          (CapSense_dsRam.modCsdClk)
#define CapSense_MOD_CSD_CLK_OFFSET                         (32u)
#define CapSense_MOD_CSD_CLK_SIZE                           (1u)
#define CapSense_MOD_CSD_CLK_PARAM_ID                       (0x63800020u)

#define CapSense_ANSWER_RESOLUTION_VALUE                    (CapSense_dsRam.wdgtList.answer.resolution)
#define CapSense_ANSWER_RESOLUTION_OFFSET                   (34u)
#define CapSense_ANSWER_RESOLUTION_SIZE                     (2u)
#define CapSense_ANSWER_RESOLUTION_PARAM_ID                 (0xA7800022u)

#define CapSense_ANSWER_FINGER_TH_VALUE                     (CapSense_dsRam.wdgtList.answer.fingerTh)
#define CapSense_ANSWER_FINGER_TH_OFFSET                    (36u)
#define CapSense_ANSWER_FINGER_TH_SIZE                      (2u)
#define CapSense_ANSWER_FINGER_TH_PARAM_ID                  (0xAA800024u)

#define CapSense_ANSWER_NOISE_TH_VALUE                      (CapSense_dsRam.wdgtList.answer.noiseTh)
#define CapSense_ANSWER_NOISE_TH_OFFSET                     (38u)
#define CapSense_ANSWER_NOISE_TH_SIZE                       (1u)
#define CapSense_ANSWER_NOISE_TH_PARAM_ID                   (0x6E800026u)

#define CapSense_ANSWER_NNOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.answer.nNoiseTh)
#define CapSense_ANSWER_NNOISE_TH_OFFSET                    (39u)
#define CapSense_ANSWER_NNOISE_TH_SIZE                      (1u)
#define CapSense_ANSWER_NNOISE_TH_PARAM_ID                  (0x68800027u)

#define CapSense_ANSWER_HYSTERESIS_VALUE                    (CapSense_dsRam.wdgtList.answer.hysteresis)
#define CapSense_ANSWER_HYSTERESIS_OFFSET                   (40u)
#define CapSense_ANSWER_HYSTERESIS_SIZE                     (1u)
#define CapSense_ANSWER_HYSTERESIS_PARAM_ID                 (0x61800028u)

#define CapSense_ANSWER_ON_DEBOUNCE_VALUE                   (CapSense_dsRam.wdgtList.answer.onDebounce)
#define CapSense_ANSWER_ON_DEBOUNCE_OFFSET                  (41u)
#define CapSense_ANSWER_ON_DEBOUNCE_SIZE                    (1u)
#define CapSense_ANSWER_ON_DEBOUNCE_PARAM_ID                (0x67800029u)

#define CapSense_ANSWER_LOW_BSLN_RST_VALUE                  (CapSense_dsRam.wdgtList.answer.lowBslnRst)
#define CapSense_ANSWER_LOW_BSLN_RST_OFFSET                 (42u)
#define CapSense_ANSWER_LOW_BSLN_RST_SIZE                   (1u)
#define CapSense_ANSWER_LOW_BSLN_RST_PARAM_ID               (0x6D80002Au)

#define CapSense_ANSWER_IDAC_MOD0_VALUE                     (CapSense_dsRam.wdgtList.answer.idacMod[0u])
#define CapSense_ANSWER_IDAC_MOD0_OFFSET                    (43u)
#define CapSense_ANSWER_IDAC_MOD0_SIZE                      (1u)
#define CapSense_ANSWER_IDAC_MOD0_PARAM_ID                  (0x6600002Bu)

#define CapSense_ANSWER_SNS_CLK_VALUE                       (CapSense_dsRam.wdgtList.answer.snsClk)
#define CapSense_ANSWER_SNS_CLK_OFFSET                      (44u)
#define CapSense_ANSWER_SNS_CLK_SIZE                        (2u)
#define CapSense_ANSWER_SNS_CLK_PARAM_ID                    (0xA880002Cu)

#define CapSense_ANSWER_SNS_CLK_SOURCE_VALUE                (CapSense_dsRam.wdgtList.answer.snsClkSource)
#define CapSense_ANSWER_SNS_CLK_SOURCE_OFFSET               (46u)
#define CapSense_ANSWER_SNS_CLK_SOURCE_SIZE                 (1u)
#define CapSense_ANSWER_SNS_CLK_SOURCE_PARAM_ID             (0x4780002Eu)

#define CapSense_MENU_RESOLUTION_VALUE                      (CapSense_dsRam.wdgtList.menu.resolution)
#define CapSense_MENU_RESOLUTION_OFFSET                     (48u)
#define CapSense_MENU_RESOLUTION_SIZE                       (2u)
#define CapSense_MENU_RESOLUTION_PARAM_ID                   (0xAD810030u)

#define CapSense_MENU_FINGER_TH_VALUE                       (CapSense_dsRam.wdgtList.menu.fingerTh)
#define CapSense_MENU_FINGER_TH_OFFSET                      (50u)
#define CapSense_MENU_FINGER_TH_SIZE                        (2u)
#define CapSense_MENU_FINGER_TH_PARAM_ID                    (0xA1810032u)

#define CapSense_MENU_NOISE_TH_VALUE                        (CapSense_dsRam.wdgtList.menu.noiseTh)
#define CapSense_MENU_NOISE_TH_OFFSET                       (52u)
#define CapSense_MENU_NOISE_TH_SIZE                         (1u)
#define CapSense_MENU_NOISE_TH_PARAM_ID                     (0x64810034u)

#define CapSense_MENU_NNOISE_TH_VALUE                       (CapSense_dsRam.wdgtList.menu.nNoiseTh)
#define CapSense_MENU_NNOISE_TH_OFFSET                      (53u)
#define CapSense_MENU_NNOISE_TH_SIZE                        (1u)
#define CapSense_MENU_NNOISE_TH_PARAM_ID                    (0x62810035u)

#define CapSense_MENU_HYSTERESIS_VALUE                      (CapSense_dsRam.wdgtList.menu.hysteresis)
#define CapSense_MENU_HYSTERESIS_OFFSET                     (54u)
#define CapSense_MENU_HYSTERESIS_SIZE                       (1u)
#define CapSense_MENU_HYSTERESIS_PARAM_ID                   (0x68810036u)

#define CapSense_MENU_ON_DEBOUNCE_VALUE                     (CapSense_dsRam.wdgtList.menu.onDebounce)
#define CapSense_MENU_ON_DEBOUNCE_OFFSET                    (55u)
#define CapSense_MENU_ON_DEBOUNCE_SIZE                      (1u)
#define CapSense_MENU_ON_DEBOUNCE_PARAM_ID                  (0x6E810037u)

#define CapSense_MENU_LOW_BSLN_RST_VALUE                    (CapSense_dsRam.wdgtList.menu.lowBslnRst)
#define CapSense_MENU_LOW_BSLN_RST_OFFSET                   (56u)
#define CapSense_MENU_LOW_BSLN_RST_SIZE                     (1u)
#define CapSense_MENU_LOW_BSLN_RST_PARAM_ID                 (0x67810038u)

#define CapSense_MENU_IDAC_MOD0_VALUE                       (CapSense_dsRam.wdgtList.menu.idacMod[0u])
#define CapSense_MENU_IDAC_MOD0_OFFSET                      (57u)
#define CapSense_MENU_IDAC_MOD0_SIZE                        (1u)
#define CapSense_MENU_IDAC_MOD0_PARAM_ID                    (0x6C010039u)

#define CapSense_MENU_SNS_CLK_VALUE                         (CapSense_dsRam.wdgtList.menu.snsClk)
#define CapSense_MENU_SNS_CLK_OFFSET                        (58u)
#define CapSense_MENU_SNS_CLK_SIZE                          (2u)
#define CapSense_MENU_SNS_CLK_PARAM_ID                      (0xA381003Au)

#define CapSense_MENU_SNS_CLK_SOURCE_VALUE                  (CapSense_dsRam.wdgtList.menu.snsClkSource)
#define CapSense_MENU_SNS_CLK_SOURCE_OFFSET                 (60u)
#define CapSense_MENU_SNS_CLK_SOURCE_SIZE                   (1u)
#define CapSense_MENU_SNS_CLK_SOURCE_PARAM_ID               (0x4D81003Cu)

#define CapSense_MODE_RESOLUTION_VALUE                      (CapSense_dsRam.wdgtList.mode.resolution)
#define CapSense_MODE_RESOLUTION_OFFSET                     (62u)
#define CapSense_MODE_RESOLUTION_SIZE                       (2u)
#define CapSense_MODE_RESOLUTION_PARAM_ID                   (0xA782003Eu)

#define CapSense_MODE_FINGER_TH_VALUE                       (CapSense_dsRam.wdgtList.mode.fingerTh)
#define CapSense_MODE_FINGER_TH_OFFSET                      (64u)
#define CapSense_MODE_FINGER_TH_SIZE                        (2u)
#define CapSense_MODE_FINGER_TH_PARAM_ID                    (0xAA820040u)

#define CapSense_MODE_NOISE_TH_VALUE                        (CapSense_dsRam.wdgtList.mode.noiseTh)
#define CapSense_MODE_NOISE_TH_OFFSET                       (66u)
#define CapSense_MODE_NOISE_TH_SIZE                         (1u)
#define CapSense_MODE_NOISE_TH_PARAM_ID                     (0x6E820042u)

#define CapSense_MODE_NNOISE_TH_VALUE                       (CapSense_dsRam.wdgtList.mode.nNoiseTh)
#define CapSense_MODE_NNOISE_TH_OFFSET                      (67u)
#define CapSense_MODE_NNOISE_TH_SIZE                        (1u)
#define CapSense_MODE_NNOISE_TH_PARAM_ID                    (0x68820043u)

#define CapSense_MODE_HYSTERESIS_VALUE                      (CapSense_dsRam.wdgtList.mode.hysteresis)
#define CapSense_MODE_HYSTERESIS_OFFSET                     (68u)
#define CapSense_MODE_HYSTERESIS_SIZE                       (1u)
#define CapSense_MODE_HYSTERESIS_PARAM_ID                   (0x63820044u)

#define CapSense_MODE_ON_DEBOUNCE_VALUE                     (CapSense_dsRam.wdgtList.mode.onDebounce)
#define CapSense_MODE_ON_DEBOUNCE_OFFSET                    (69u)
#define CapSense_MODE_ON_DEBOUNCE_SIZE                      (1u)
#define CapSense_MODE_ON_DEBOUNCE_PARAM_ID                  (0x65820045u)

#define CapSense_MODE_LOW_BSLN_RST_VALUE                    (CapSense_dsRam.wdgtList.mode.lowBslnRst)
#define CapSense_MODE_LOW_BSLN_RST_OFFSET                   (70u)
#define CapSense_MODE_LOW_BSLN_RST_SIZE                     (1u)
#define CapSense_MODE_LOW_BSLN_RST_PARAM_ID                 (0x6F820046u)

#define CapSense_MODE_IDAC_MOD0_VALUE                       (CapSense_dsRam.wdgtList.mode.idacMod[0u])
#define CapSense_MODE_IDAC_MOD0_OFFSET                      (71u)
#define CapSense_MODE_IDAC_MOD0_SIZE                        (1u)
#define CapSense_MODE_IDAC_MOD0_PARAM_ID                    (0x64020047u)

#define CapSense_MODE_SNS_CLK_VALUE                         (CapSense_dsRam.wdgtList.mode.snsClk)
#define CapSense_MODE_SNS_CLK_OFFSET                        (72u)
#define CapSense_MODE_SNS_CLK_SIZE                          (2u)
#define CapSense_MODE_SNS_CLK_PARAM_ID                      (0xA8820048u)

#define CapSense_MODE_SNS_CLK_SOURCE_VALUE                  (CapSense_dsRam.wdgtList.mode.snsClkSource)
#define CapSense_MODE_SNS_CLK_SOURCE_OFFSET                 (74u)
#define CapSense_MODE_SNS_CLK_SOURCE_SIZE                   (1u)
#define CapSense_MODE_SNS_CLK_SOURCE_PARAM_ID               (0x4782004Au)

#define CapSense_SPEECH_RESOLUTION_VALUE                    (CapSense_dsRam.wdgtList.speech.resolution)
#define CapSense_SPEECH_RESOLUTION_OFFSET                   (76u)
#define CapSense_SPEECH_RESOLUTION_SIZE                     (2u)
#define CapSense_SPEECH_RESOLUTION_PARAM_ID                 (0xAA83004Cu)

#define CapSense_SPEECH_FINGER_TH_VALUE                     (CapSense_dsRam.wdgtList.speech.fingerTh)
#define CapSense_SPEECH_FINGER_TH_OFFSET                    (78u)
#define CapSense_SPEECH_FINGER_TH_SIZE                      (2u)
#define CapSense_SPEECH_FINGER_TH_PARAM_ID                  (0xA683004Eu)

#define CapSense_SPEECH_NOISE_TH_VALUE                      (CapSense_dsRam.wdgtList.speech.noiseTh)
#define CapSense_SPEECH_NOISE_TH_OFFSET                     (80u)
#define CapSense_SPEECH_NOISE_TH_SIZE                       (1u)
#define CapSense_SPEECH_NOISE_TH_PARAM_ID                   (0x64830050u)

#define CapSense_SPEECH_NNOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.speech.nNoiseTh)
#define CapSense_SPEECH_NNOISE_TH_OFFSET                    (81u)
#define CapSense_SPEECH_NNOISE_TH_SIZE                      (1u)
#define CapSense_SPEECH_NNOISE_TH_PARAM_ID                  (0x62830051u)

#define CapSense_SPEECH_HYSTERESIS_VALUE                    (CapSense_dsRam.wdgtList.speech.hysteresis)
#define CapSense_SPEECH_HYSTERESIS_OFFSET                   (82u)
#define CapSense_SPEECH_HYSTERESIS_SIZE                     (1u)
#define CapSense_SPEECH_HYSTERESIS_PARAM_ID                 (0x68830052u)

#define CapSense_SPEECH_ON_DEBOUNCE_VALUE                   (CapSense_dsRam.wdgtList.speech.onDebounce)
#define CapSense_SPEECH_ON_DEBOUNCE_OFFSET                  (83u)
#define CapSense_SPEECH_ON_DEBOUNCE_SIZE                    (1u)
#define CapSense_SPEECH_ON_DEBOUNCE_PARAM_ID                (0x6E830053u)

#define CapSense_SPEECH_LOW_BSLN_RST_VALUE                  (CapSense_dsRam.wdgtList.speech.lowBslnRst)
#define CapSense_SPEECH_LOW_BSLN_RST_OFFSET                 (84u)
#define CapSense_SPEECH_LOW_BSLN_RST_SIZE                   (1u)
#define CapSense_SPEECH_LOW_BSLN_RST_PARAM_ID               (0x65830054u)

#define CapSense_SPEECH_IDAC_MOD0_VALUE                     (CapSense_dsRam.wdgtList.speech.idacMod[0u])
#define CapSense_SPEECH_IDAC_MOD0_OFFSET                    (85u)
#define CapSense_SPEECH_IDAC_MOD0_SIZE                      (1u)
#define CapSense_SPEECH_IDAC_MOD0_PARAM_ID                  (0x6E030055u)

#define CapSense_SPEECH_SNS_CLK_VALUE                       (CapSense_dsRam.wdgtList.speech.snsClk)
#define CapSense_SPEECH_SNS_CLK_OFFSET                      (86u)
#define CapSense_SPEECH_SNS_CLK_SIZE                        (2u)
#define CapSense_SPEECH_SNS_CLK_PARAM_ID                    (0xA1830056u)

#define CapSense_SPEECH_SNS_CLK_SOURCE_VALUE                (CapSense_dsRam.wdgtList.speech.snsClkSource)
#define CapSense_SPEECH_SNS_CLK_SOURCE_OFFSET               (88u)
#define CapSense_SPEECH_SNS_CLK_SOURCE_SIZE                 (1u)
#define CapSense_SPEECH_SNS_CLK_SOURCE_PARAM_ID             (0x4D830058u)

#define CapSense_DIST_RESOLUTION_VALUE                      (CapSense_dsRam.wdgtList.dist.resolution)
#define CapSense_DIST_RESOLUTION_OFFSET                     (90u)
#define CapSense_DIST_RESOLUTION_SIZE                       (2u)
#define CapSense_DIST_RESOLUTION_PARAM_ID                   (0xAB84005Au)

#define CapSense_DIST_FINGER_TH_VALUE                       (CapSense_dsRam.wdgtList.dist.fingerTh)
#define CapSense_DIST_FINGER_TH_OFFSET                      (92u)
#define CapSense_DIST_FINGER_TH_SIZE                        (2u)
#define CapSense_DIST_FINGER_TH_PARAM_ID                    (0xA684005Cu)

#define CapSense_DIST_NOISE_TH_VALUE                        (CapSense_dsRam.wdgtList.dist.noiseTh)
#define CapSense_DIST_NOISE_TH_OFFSET                       (94u)
#define CapSense_DIST_NOISE_TH_SIZE                         (1u)
#define CapSense_DIST_NOISE_TH_PARAM_ID                     (0x6284005Eu)

#define CapSense_DIST_NNOISE_TH_VALUE                       (CapSense_dsRam.wdgtList.dist.nNoiseTh)
#define CapSense_DIST_NNOISE_TH_OFFSET                      (95u)
#define CapSense_DIST_NNOISE_TH_SIZE                        (1u)
#define CapSense_DIST_NNOISE_TH_PARAM_ID                    (0x6484005Fu)

#define CapSense_DIST_HYSTERESIS_VALUE                      (CapSense_dsRam.wdgtList.dist.hysteresis)
#define CapSense_DIST_HYSTERESIS_OFFSET                     (96u)
#define CapSense_DIST_HYSTERESIS_SIZE                       (1u)
#define CapSense_DIST_HYSTERESIS_PARAM_ID                   (0x62840060u)

#define CapSense_DIST_ON_DEBOUNCE_VALUE                     (CapSense_dsRam.wdgtList.dist.onDebounce)
#define CapSense_DIST_ON_DEBOUNCE_OFFSET                    (97u)
#define CapSense_DIST_ON_DEBOUNCE_SIZE                      (1u)
#define CapSense_DIST_ON_DEBOUNCE_PARAM_ID                  (0x64840061u)

#define CapSense_DIST_LOW_BSLN_RST_VALUE                    (CapSense_dsRam.wdgtList.dist.lowBslnRst)
#define CapSense_DIST_LOW_BSLN_RST_OFFSET                   (98u)
#define CapSense_DIST_LOW_BSLN_RST_SIZE                     (1u)
#define CapSense_DIST_LOW_BSLN_RST_PARAM_ID                 (0x6E840062u)

#define CapSense_DIST_IDAC_MOD0_VALUE                       (CapSense_dsRam.wdgtList.dist.idacMod[0u])
#define CapSense_DIST_IDAC_MOD0_OFFSET                      (99u)
#define CapSense_DIST_IDAC_MOD0_SIZE                        (1u)
#define CapSense_DIST_IDAC_MOD0_PARAM_ID                    (0x65040063u)

#define CapSense_DIST_SNS_CLK_VALUE                         (CapSense_dsRam.wdgtList.dist.snsClk)
#define CapSense_DIST_SNS_CLK_OFFSET                        (100u)
#define CapSense_DIST_SNS_CLK_SIZE                          (2u)
#define CapSense_DIST_SNS_CLK_PARAM_ID                      (0xAB840064u)

#define CapSense_DIST_SNS_CLK_SOURCE_VALUE                  (CapSense_dsRam.wdgtList.dist.snsClkSource)
#define CapSense_DIST_SNS_CLK_SOURCE_OFFSET                 (102u)
#define CapSense_DIST_SNS_CLK_SOURCE_SIZE                   (1u)
#define CapSense_DIST_SNS_CLK_SOURCE_PARAM_ID               (0x44840066u)

#define CapSense_ADAS_RESOLUTION_VALUE                      (CapSense_dsRam.wdgtList.adas.resolution)
#define CapSense_ADAS_RESOLUTION_OFFSET                     (104u)
#define CapSense_ADAS_RESOLUTION_SIZE                       (2u)
#define CapSense_ADAS_RESOLUTION_PARAM_ID                   (0xAB850068u)

#define CapSense_ADAS_FINGER_TH_VALUE                       (CapSense_dsRam.wdgtList.adas.fingerTh)
#define CapSense_ADAS_FINGER_TH_OFFSET                      (106u)
#define CapSense_ADAS_FINGER_TH_SIZE                        (2u)
#define CapSense_ADAS_FINGER_TH_PARAM_ID                    (0xA785006Au)

#define CapSense_ADAS_NOISE_TH_VALUE                        (CapSense_dsRam.wdgtList.adas.noiseTh)
#define CapSense_ADAS_NOISE_TH_OFFSET                       (108u)
#define CapSense_ADAS_NOISE_TH_SIZE                         (1u)
#define CapSense_ADAS_NOISE_TH_PARAM_ID                     (0x6285006Cu)

#define CapSense_ADAS_NNOISE_TH_VALUE                       (CapSense_dsRam.wdgtList.adas.nNoiseTh)
#define CapSense_ADAS_NNOISE_TH_OFFSET                      (109u)
#define CapSense_ADAS_NNOISE_TH_SIZE                        (1u)
#define CapSense_ADAS_NNOISE_TH_PARAM_ID                    (0x6485006Du)

#define CapSense_ADAS_HYSTERESIS_VALUE                      (CapSense_dsRam.wdgtList.adas.hysteresis)
#define CapSense_ADAS_HYSTERESIS_OFFSET                     (110u)
#define CapSense_ADAS_HYSTERESIS_SIZE                       (1u)
#define CapSense_ADAS_HYSTERESIS_PARAM_ID                   (0x6E85006Eu)

#define CapSense_ADAS_ON_DEBOUNCE_VALUE                     (CapSense_dsRam.wdgtList.adas.onDebounce)
#define CapSense_ADAS_ON_DEBOUNCE_OFFSET                    (111u)
#define CapSense_ADAS_ON_DEBOUNCE_SIZE                      (1u)
#define CapSense_ADAS_ON_DEBOUNCE_PARAM_ID                  (0x6885006Fu)

#define CapSense_ADAS_LOW_BSLN_RST_VALUE                    (CapSense_dsRam.wdgtList.adas.lowBslnRst)
#define CapSense_ADAS_LOW_BSLN_RST_OFFSET                   (112u)
#define CapSense_ADAS_LOW_BSLN_RST_SIZE                     (1u)
#define CapSense_ADAS_LOW_BSLN_RST_PARAM_ID                 (0x64850070u)

#define CapSense_ADAS_IDAC_MOD0_VALUE                       (CapSense_dsRam.wdgtList.adas.idacMod[0u])
#define CapSense_ADAS_IDAC_MOD0_OFFSET                      (113u)
#define CapSense_ADAS_IDAC_MOD0_SIZE                        (1u)
#define CapSense_ADAS_IDAC_MOD0_PARAM_ID                    (0x6F050071u)

#define CapSense_ADAS_SNS_CLK_VALUE                         (CapSense_dsRam.wdgtList.adas.snsClk)
#define CapSense_ADAS_SNS_CLK_OFFSET                        (114u)
#define CapSense_ADAS_SNS_CLK_SIZE                          (2u)
#define CapSense_ADAS_SNS_CLK_PARAM_ID                      (0xA0850072u)

#define CapSense_ADAS_SNS_CLK_SOURCE_VALUE                  (CapSense_dsRam.wdgtList.adas.snsClkSource)
#define CapSense_ADAS_SNS_CLK_SOURCE_OFFSET                 (116u)
#define CapSense_ADAS_SNS_CLK_SOURCE_SIZE                   (1u)
#define CapSense_ADAS_SNS_CLK_SOURCE_PARAM_ID               (0x4E850074u)

#define CapSense_ANSWER_SNS0_RAW0_VALUE                     (CapSense_dsRam.snsList.answer[0u].raw[0u])
#define CapSense_ANSWER_SNS0_RAW0_OFFSET                    (118u)
#define CapSense_ANSWER_SNS0_RAW0_SIZE                      (2u)
#define CapSense_ANSWER_SNS0_RAW0_PARAM_ID                  (0x88000076u)

#define CapSense_ANSWER_SNS0_BSLN0_VALUE                    (CapSense_dsRam.snsList.answer[0u].bsln[0u])
#define CapSense_ANSWER_SNS0_BSLN0_OFFSET                   (120u)
#define CapSense_ANSWER_SNS0_BSLN0_SIZE                     (2u)
#define CapSense_ANSWER_SNS0_BSLN0_PARAM_ID                 (0x87000078u)

#define CapSense_ANSWER_SNS0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.answer[0u].bslnExt[0u])
#define CapSense_ANSWER_SNS0_BSLN_EXT0_OFFSET               (122u)
#define CapSense_ANSWER_SNS0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_ANSWER_SNS0_BSLN_EXT0_PARAM_ID             (0x4300007Au)

#define CapSense_ANSWER_SNS0_DIFF_VALUE                     (CapSense_dsRam.snsList.answer[0u].diff)
#define CapSense_ANSWER_SNS0_DIFF_OFFSET                    (124u)
#define CapSense_ANSWER_SNS0_DIFF_SIZE                      (2u)
#define CapSense_ANSWER_SNS0_DIFF_PARAM_ID                  (0x8600007Cu)

#define CapSense_ANSWER_SNS0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.answer[0u].negBslnRstCnt[0u])
#define CapSense_ANSWER_SNS0_NEG_BSLN_RST_CNT0_OFFSET       (126u)
#define CapSense_ANSWER_SNS0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_ANSWER_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4200007Eu)

#define CapSense_ANSWER_SNS0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.answer[0u].idacComp[0u])
#define CapSense_ANSWER_SNS0_IDAC_COMP0_OFFSET              (127u)
#define CapSense_ANSWER_SNS0_IDAC_COMP0_SIZE                (1u)
#define CapSense_ANSWER_SNS0_IDAC_COMP0_PARAM_ID            (0x6F00007Fu)

#define CapSense_MENU_SNS0_RAW0_VALUE                       (CapSense_dsRam.snsList.menu[0u].raw[0u])
#define CapSense_MENU_SNS0_RAW0_OFFSET                      (128u)
#define CapSense_MENU_SNS0_RAW0_SIZE                        (2u)
#define CapSense_MENU_SNS0_RAW0_PARAM_ID                    (0x84000080u)

#define CapSense_MENU_SNS0_BSLN0_VALUE                      (CapSense_dsRam.snsList.menu[0u].bsln[0u])
#define CapSense_MENU_SNS0_BSLN0_OFFSET                     (130u)
#define CapSense_MENU_SNS0_BSLN0_SIZE                       (2u)
#define CapSense_MENU_SNS0_BSLN0_PARAM_ID                   (0x88000082u)

#define CapSense_MENU_SNS0_BSLN_EXT0_VALUE                  (CapSense_dsRam.snsList.menu[0u].bslnExt[0u])
#define CapSense_MENU_SNS0_BSLN_EXT0_OFFSET                 (132u)
#define CapSense_MENU_SNS0_BSLN_EXT0_SIZE                   (1u)
#define CapSense_MENU_SNS0_BSLN_EXT0_PARAM_ID               (0x4D000084u)

#define CapSense_MENU_SNS0_DIFF_VALUE                       (CapSense_dsRam.snsList.menu[0u].diff)
#define CapSense_MENU_SNS0_DIFF_OFFSET                      (134u)
#define CapSense_MENU_SNS0_DIFF_SIZE                        (2u)
#define CapSense_MENU_SNS0_DIFF_PARAM_ID                    (0x89000086u)

#define CapSense_MENU_SNS0_NEG_BSLN_RST_CNT0_VALUE          (CapSense_dsRam.snsList.menu[0u].negBslnRstCnt[0u])
#define CapSense_MENU_SNS0_NEG_BSLN_RST_CNT0_OFFSET         (136u)
#define CapSense_MENU_SNS0_NEG_BSLN_RST_CNT0_SIZE           (1u)
#define CapSense_MENU_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID       (0x4E000088u)

#define CapSense_MENU_SNS0_IDAC_COMP0_VALUE                 (CapSense_dsRam.snsList.menu[0u].idacComp[0u])
#define CapSense_MENU_SNS0_IDAC_COMP0_OFFSET                (137u)
#define CapSense_MENU_SNS0_IDAC_COMP0_SIZE                  (1u)
#define CapSense_MENU_SNS0_IDAC_COMP0_PARAM_ID              (0x63000089u)

#define CapSense_MODE_SNS0_RAW0_VALUE                       (CapSense_dsRam.snsList.mode[0u].raw[0u])
#define CapSense_MODE_SNS0_RAW0_OFFSET                      (138u)
#define CapSense_MODE_SNS0_RAW0_SIZE                        (2u)
#define CapSense_MODE_SNS0_RAW0_PARAM_ID                    (0x8A00008Au)

#define CapSense_MODE_SNS0_BSLN0_VALUE                      (CapSense_dsRam.snsList.mode[0u].bsln[0u])
#define CapSense_MODE_SNS0_BSLN0_OFFSET                     (140u)
#define CapSense_MODE_SNS0_BSLN0_SIZE                       (2u)
#define CapSense_MODE_SNS0_BSLN0_PARAM_ID                   (0x8700008Cu)

#define CapSense_MODE_SNS0_BSLN_EXT0_VALUE                  (CapSense_dsRam.snsList.mode[0u].bslnExt[0u])
#define CapSense_MODE_SNS0_BSLN_EXT0_OFFSET                 (142u)
#define CapSense_MODE_SNS0_BSLN_EXT0_SIZE                   (1u)
#define CapSense_MODE_SNS0_BSLN_EXT0_PARAM_ID               (0x4300008Eu)

#define CapSense_MODE_SNS0_DIFF_VALUE                       (CapSense_dsRam.snsList.mode[0u].diff)
#define CapSense_MODE_SNS0_DIFF_OFFSET                      (144u)
#define CapSense_MODE_SNS0_DIFF_SIZE                        (2u)
#define CapSense_MODE_SNS0_DIFF_PARAM_ID                    (0x81000090u)

#define CapSense_MODE_SNS0_NEG_BSLN_RST_CNT0_VALUE          (CapSense_dsRam.snsList.mode[0u].negBslnRstCnt[0u])
#define CapSense_MODE_SNS0_NEG_BSLN_RST_CNT0_OFFSET         (146u)
#define CapSense_MODE_SNS0_NEG_BSLN_RST_CNT0_SIZE           (1u)
#define CapSense_MODE_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID       (0x45000092u)

#define CapSense_MODE_SNS0_IDAC_COMP0_VALUE                 (CapSense_dsRam.snsList.mode[0u].idacComp[0u])
#define CapSense_MODE_SNS0_IDAC_COMP0_OFFSET                (147u)
#define CapSense_MODE_SNS0_IDAC_COMP0_SIZE                  (1u)
#define CapSense_MODE_SNS0_IDAC_COMP0_PARAM_ID              (0x68000093u)

#define CapSense_SPEECH_SNS0_RAW0_VALUE                     (CapSense_dsRam.snsList.speech[0u].raw[0u])
#define CapSense_SPEECH_SNS0_RAW0_OFFSET                    (148u)
#define CapSense_SPEECH_SNS0_RAW0_SIZE                      (2u)
#define CapSense_SPEECH_SNS0_RAW0_PARAM_ID                  (0x80000094u)

#define CapSense_SPEECH_SNS0_BSLN0_VALUE                    (CapSense_dsRam.snsList.speech[0u].bsln[0u])
#define CapSense_SPEECH_SNS0_BSLN0_OFFSET                   (150u)
#define CapSense_SPEECH_SNS0_BSLN0_SIZE                     (2u)
#define CapSense_SPEECH_SNS0_BSLN0_PARAM_ID                 (0x8C000096u)

#define CapSense_SPEECH_SNS0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.speech[0u].bslnExt[0u])
#define CapSense_SPEECH_SNS0_BSLN_EXT0_OFFSET               (152u)
#define CapSense_SPEECH_SNS0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_SPEECH_SNS0_BSLN_EXT0_PARAM_ID             (0x4B000098u)

#define CapSense_SPEECH_SNS0_DIFF_VALUE                     (CapSense_dsRam.snsList.speech[0u].diff)
#define CapSense_SPEECH_SNS0_DIFF_OFFSET                    (154u)
#define CapSense_SPEECH_SNS0_DIFF_SIZE                      (2u)
#define CapSense_SPEECH_SNS0_DIFF_PARAM_ID                  (0x8F00009Au)

#define CapSense_SPEECH_SNS0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.speech[0u].negBslnRstCnt[0u])
#define CapSense_SPEECH_SNS0_NEG_BSLN_RST_CNT0_OFFSET       (156u)
#define CapSense_SPEECH_SNS0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_SPEECH_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4A00009Cu)

#define CapSense_SPEECH_SNS0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.speech[0u].idacComp[0u])
#define CapSense_SPEECH_SNS0_IDAC_COMP0_OFFSET              (157u)
#define CapSense_SPEECH_SNS0_IDAC_COMP0_SIZE                (1u)
#define CapSense_SPEECH_SNS0_IDAC_COMP0_PARAM_ID            (0x6700009Du)

#define CapSense_DIST_SNS0_RAW0_VALUE                       (CapSense_dsRam.snsList.dist[0u].raw[0u])
#define CapSense_DIST_SNS0_RAW0_OFFSET                      (158u)
#define CapSense_DIST_SNS0_RAW0_SIZE                        (2u)
#define CapSense_DIST_SNS0_RAW0_PARAM_ID                    (0x8E00009Eu)

#define CapSense_DIST_SNS0_BSLN0_VALUE                      (CapSense_dsRam.snsList.dist[0u].bsln[0u])
#define CapSense_DIST_SNS0_BSLN0_OFFSET                     (160u)
#define CapSense_DIST_SNS0_BSLN0_SIZE                       (2u)
#define CapSense_DIST_SNS0_BSLN0_PARAM_ID                   (0x8E0000A0u)

#define CapSense_DIST_SNS0_BSLN_EXT0_VALUE                  (CapSense_dsRam.snsList.dist[0u].bslnExt[0u])
#define CapSense_DIST_SNS0_BSLN_EXT0_OFFSET                 (162u)
#define CapSense_DIST_SNS0_BSLN_EXT0_SIZE                   (1u)
#define CapSense_DIST_SNS0_BSLN_EXT0_PARAM_ID               (0x4A0000A2u)

#define CapSense_DIST_SNS0_DIFF_VALUE                       (CapSense_dsRam.snsList.dist[0u].diff)
#define CapSense_DIST_SNS0_DIFF_OFFSET                      (164u)
#define CapSense_DIST_SNS0_DIFF_SIZE                        (2u)
#define CapSense_DIST_SNS0_DIFF_PARAM_ID                    (0x8F0000A4u)

#define CapSense_DIST_SNS0_NEG_BSLN_RST_CNT0_VALUE          (CapSense_dsRam.snsList.dist[0u].negBslnRstCnt[0u])
#define CapSense_DIST_SNS0_NEG_BSLN_RST_CNT0_OFFSET         (166u)
#define CapSense_DIST_SNS0_NEG_BSLN_RST_CNT0_SIZE           (1u)
#define CapSense_DIST_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID       (0x4B0000A6u)

#define CapSense_DIST_SNS0_IDAC_COMP0_VALUE                 (CapSense_dsRam.snsList.dist[0u].idacComp[0u])
#define CapSense_DIST_SNS0_IDAC_COMP0_OFFSET                (167u)
#define CapSense_DIST_SNS0_IDAC_COMP0_SIZE                  (1u)
#define CapSense_DIST_SNS0_IDAC_COMP0_PARAM_ID              (0x660000A7u)

#define CapSense_ADAS_SNS0_RAW0_VALUE                       (CapSense_dsRam.snsList.adas[0u].raw[0u])
#define CapSense_ADAS_SNS0_RAW0_OFFSET                      (168u)
#define CapSense_ADAS_SNS0_RAW0_SIZE                        (2u)
#define CapSense_ADAS_SNS0_RAW0_PARAM_ID                    (0x8C0000A8u)

#define CapSense_ADAS_SNS0_BSLN0_VALUE                      (CapSense_dsRam.snsList.adas[0u].bsln[0u])
#define CapSense_ADAS_SNS0_BSLN0_OFFSET                     (170u)
#define CapSense_ADAS_SNS0_BSLN0_SIZE                       (2u)
#define CapSense_ADAS_SNS0_BSLN0_PARAM_ID                   (0x800000AAu)

#define CapSense_ADAS_SNS0_BSLN_EXT0_VALUE                  (CapSense_dsRam.snsList.adas[0u].bslnExt[0u])
#define CapSense_ADAS_SNS0_BSLN_EXT0_OFFSET                 (172u)
#define CapSense_ADAS_SNS0_BSLN_EXT0_SIZE                   (1u)
#define CapSense_ADAS_SNS0_BSLN_EXT0_PARAM_ID               (0x450000ACu)

#define CapSense_ADAS_SNS0_DIFF_VALUE                       (CapSense_dsRam.snsList.adas[0u].diff)
#define CapSense_ADAS_SNS0_DIFF_OFFSET                      (174u)
#define CapSense_ADAS_SNS0_DIFF_SIZE                        (2u)
#define CapSense_ADAS_SNS0_DIFF_PARAM_ID                    (0x810000AEu)

#define CapSense_ADAS_SNS0_NEG_BSLN_RST_CNT0_VALUE          (CapSense_dsRam.snsList.adas[0u].negBslnRstCnt[0u])
#define CapSense_ADAS_SNS0_NEG_BSLN_RST_CNT0_OFFSET         (176u)
#define CapSense_ADAS_SNS0_NEG_BSLN_RST_CNT0_SIZE           (1u)
#define CapSense_ADAS_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID       (0x430000B0u)

#define CapSense_ADAS_SNS0_IDAC_COMP0_VALUE                 (CapSense_dsRam.snsList.adas[0u].idacComp[0u])
#define CapSense_ADAS_SNS0_IDAC_COMP0_OFFSET                (177u)
#define CapSense_ADAS_SNS0_IDAC_COMP0_SIZE                  (1u)
#define CapSense_ADAS_SNS0_IDAC_COMP0_PARAM_ID              (0x6E0000B1u)

#define CapSense_SNR_TEST_WIDGET_ID_VALUE                   (CapSense_dsRam.snrTestWidgetId)
#define CapSense_SNR_TEST_WIDGET_ID_OFFSET                  (178u)
#define CapSense_SNR_TEST_WIDGET_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_WIDGET_ID_PARAM_ID                (0x640000B2u)

#define CapSense_SNR_TEST_SENSOR_ID_VALUE                   (CapSense_dsRam.snrTestSensorId)
#define CapSense_SNR_TEST_SENSOR_ID_OFFSET                  (179u)
#define CapSense_SNR_TEST_SENSOR_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_SENSOR_ID_PARAM_ID                (0x620000B3u)

#define CapSense_SNR_TEST_SCAN_COUNTER_VALUE                (CapSense_dsRam.snrTestScanCounter)
#define CapSense_SNR_TEST_SCAN_COUNTER_OFFSET               (180u)
#define CapSense_SNR_TEST_SCAN_COUNTER_SIZE                 (2u)
#define CapSense_SNR_TEST_SCAN_COUNTER_PARAM_ID             (0x8A0000B4u)

#define CapSense_SNR_TEST_RAW_COUNT0_VALUE                  (CapSense_dsRam.snrTestRawCount[0u])
#define CapSense_SNR_TEST_RAW_COUNT0_OFFSET                 (182u)
#define CapSense_SNR_TEST_RAW_COUNT0_SIZE                   (2u)
#define CapSense_SNR_TEST_RAW_COUNT0_PARAM_ID               (0x860000B6u)


/*****************************************************************************/
/* Flash Data structure register definitions                                 */
/*****************************************************************************/
#define CapSense_ANSWER_PTR2SNS_FLASH_VALUE                 (CapSense_dsFlash.wdgtArray[0].ptr2SnsFlash)
#define CapSense_ANSWER_PTR2SNS_FLASH_OFFSET                (0u)
#define CapSense_ANSWER_PTR2SNS_FLASH_SIZE                  (4u)
#define CapSense_ANSWER_PTR2SNS_FLASH_PARAM_ID              (0xD1000000u)

#define CapSense_ANSWER_PTR2WD_RAM_VALUE                    (CapSense_dsFlash.wdgtArray[0].ptr2WdgtRam)
#define CapSense_ANSWER_PTR2WD_RAM_OFFSET                   (4u)
#define CapSense_ANSWER_PTR2WD_RAM_SIZE                     (4u)
#define CapSense_ANSWER_PTR2WD_RAM_PARAM_ID                 (0xD0000004u)

#define CapSense_ANSWER_PTR2SNS_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[0].ptr2SnsRam)
#define CapSense_ANSWER_PTR2SNS_RAM_OFFSET                  (8u)
#define CapSense_ANSWER_PTR2SNS_RAM_SIZE                    (4u)
#define CapSense_ANSWER_PTR2SNS_RAM_PARAM_ID                (0xD3000008u)

#define CapSense_ANSWER_PTR2FLTR_HISTORY_VALUE              (CapSense_dsFlash.wdgtArray[0].ptr2FltrHistory)
#define CapSense_ANSWER_PTR2FLTR_HISTORY_OFFSET             (12u)
#define CapSense_ANSWER_PTR2FLTR_HISTORY_SIZE               (4u)
#define CapSense_ANSWER_PTR2FLTR_HISTORY_PARAM_ID           (0xD200000Cu)

#define CapSense_ANSWER_PTR2DEBOUNCE_VALUE                  (CapSense_dsFlash.wdgtArray[0].ptr2DebounceArr)
#define CapSense_ANSWER_PTR2DEBOUNCE_OFFSET                 (16u)
#define CapSense_ANSWER_PTR2DEBOUNCE_SIZE                   (4u)
#define CapSense_ANSWER_PTR2DEBOUNCE_PARAM_ID               (0xD4000010u)

#define CapSense_ANSWER_STATIC_CONFIG_VALUE                 (CapSense_dsFlash.wdgtArray[0].staticConfig)
#define CapSense_ANSWER_STATIC_CONFIG_OFFSET                (20u)
#define CapSense_ANSWER_STATIC_CONFIG_SIZE                  (4u)
#define CapSense_ANSWER_STATIC_CONFIG_PARAM_ID              (0xD5000014u)

#define CapSense_ANSWER_TOTAL_NUM_SNS_VALUE                 (CapSense_dsFlash.wdgtArray[0].totalNumSns)
#define CapSense_ANSWER_TOTAL_NUM_SNS_OFFSET                (24u)
#define CapSense_ANSWER_TOTAL_NUM_SNS_SIZE                  (2u)
#define CapSense_ANSWER_TOTAL_NUM_SNS_PARAM_ID              (0x99000018u)

#define CapSense_ANSWER_TYPE_VALUE                          (CapSense_dsFlash.wdgtArray[0].wdgtType)
#define CapSense_ANSWER_TYPE_OFFSET                         (26u)
#define CapSense_ANSWER_TYPE_SIZE                           (1u)
#define CapSense_ANSWER_TYPE_PARAM_ID                       (0x5D00001Au)

#define CapSense_ANSWER_NUM_COLS_VALUE                      (CapSense_dsFlash.wdgtArray[0].numCols)
#define CapSense_ANSWER_NUM_COLS_OFFSET                     (27u)
#define CapSense_ANSWER_NUM_COLS_SIZE                       (1u)
#define CapSense_ANSWER_NUM_COLS_PARAM_ID                   (0x5B00001Bu)

#define CapSense_MENU_PTR2SNS_FLASH_VALUE                   (CapSense_dsFlash.wdgtArray[1].ptr2SnsFlash)
#define CapSense_MENU_PTR2SNS_FLASH_OFFSET                  (28u)
#define CapSense_MENU_PTR2SNS_FLASH_SIZE                    (4u)
#define CapSense_MENU_PTR2SNS_FLASH_PARAM_ID                (0xD401001Cu)

#define CapSense_MENU_PTR2WD_RAM_VALUE                      (CapSense_dsFlash.wdgtArray[1].ptr2WdgtRam)
#define CapSense_MENU_PTR2WD_RAM_OFFSET                     (32u)
#define CapSense_MENU_PTR2WD_RAM_SIZE                       (4u)
#define CapSense_MENU_PTR2WD_RAM_PARAM_ID                   (0xD8010020u)

#define CapSense_MENU_PTR2SNS_RAM_VALUE                     (CapSense_dsFlash.wdgtArray[1].ptr2SnsRam)
#define CapSense_MENU_PTR2SNS_RAM_OFFSET                    (36u)
#define CapSense_MENU_PTR2SNS_RAM_SIZE                      (4u)
#define CapSense_MENU_PTR2SNS_RAM_PARAM_ID                  (0xD9010024u)

#define CapSense_MENU_PTR2FLTR_HISTORY_VALUE                (CapSense_dsFlash.wdgtArray[1].ptr2FltrHistory)
#define CapSense_MENU_PTR2FLTR_HISTORY_OFFSET               (40u)
#define CapSense_MENU_PTR2FLTR_HISTORY_SIZE                 (4u)
#define CapSense_MENU_PTR2FLTR_HISTORY_PARAM_ID             (0xDA010028u)

#define CapSense_MENU_PTR2DEBOUNCE_VALUE                    (CapSense_dsFlash.wdgtArray[1].ptr2DebounceArr)
#define CapSense_MENU_PTR2DEBOUNCE_OFFSET                   (44u)
#define CapSense_MENU_PTR2DEBOUNCE_SIZE                     (4u)
#define CapSense_MENU_PTR2DEBOUNCE_PARAM_ID                 (0xDB01002Cu)

#define CapSense_MENU_STATIC_CONFIG_VALUE                   (CapSense_dsFlash.wdgtArray[1].staticConfig)
#define CapSense_MENU_STATIC_CONFIG_OFFSET                  (48u)
#define CapSense_MENU_STATIC_CONFIG_SIZE                    (4u)
#define CapSense_MENU_STATIC_CONFIG_PARAM_ID                (0xDD010030u)

#define CapSense_MENU_TOTAL_NUM_SNS_VALUE                   (CapSense_dsFlash.wdgtArray[1].totalNumSns)
#define CapSense_MENU_TOTAL_NUM_SNS_OFFSET                  (52u)
#define CapSense_MENU_TOTAL_NUM_SNS_SIZE                    (2u)
#define CapSense_MENU_TOTAL_NUM_SNS_PARAM_ID                (0x93010034u)

#define CapSense_MENU_TYPE_VALUE                            (CapSense_dsFlash.wdgtArray[1].wdgtType)
#define CapSense_MENU_TYPE_OFFSET                           (54u)
#define CapSense_MENU_TYPE_SIZE                             (1u)
#define CapSense_MENU_TYPE_PARAM_ID                         (0x57010036u)

#define CapSense_MENU_NUM_COLS_VALUE                        (CapSense_dsFlash.wdgtArray[1].numCols)
#define CapSense_MENU_NUM_COLS_OFFSET                       (55u)
#define CapSense_MENU_NUM_COLS_SIZE                         (1u)
#define CapSense_MENU_NUM_COLS_PARAM_ID                     (0x51010037u)

#define CapSense_MODE_PTR2SNS_FLASH_VALUE                   (CapSense_dsFlash.wdgtArray[2].ptr2SnsFlash)
#define CapSense_MODE_PTR2SNS_FLASH_OFFSET                  (56u)
#define CapSense_MODE_PTR2SNS_FLASH_SIZE                    (4u)
#define CapSense_MODE_PTR2SNS_FLASH_PARAM_ID                (0xDA020038u)

#define CapSense_MODE_PTR2WD_RAM_VALUE                      (CapSense_dsFlash.wdgtArray[2].ptr2WdgtRam)
#define CapSense_MODE_PTR2WD_RAM_OFFSET                     (60u)
#define CapSense_MODE_PTR2WD_RAM_SIZE                       (4u)
#define CapSense_MODE_PTR2WD_RAM_PARAM_ID                   (0xDB02003Cu)

#define CapSense_MODE_PTR2SNS_RAM_VALUE                     (CapSense_dsFlash.wdgtArray[2].ptr2SnsRam)
#define CapSense_MODE_PTR2SNS_RAM_OFFSET                    (64u)
#define CapSense_MODE_PTR2SNS_RAM_SIZE                      (4u)
#define CapSense_MODE_PTR2SNS_RAM_PARAM_ID                  (0xDA020040u)

#define CapSense_MODE_PTR2FLTR_HISTORY_VALUE                (CapSense_dsFlash.wdgtArray[2].ptr2FltrHistory)
#define CapSense_MODE_PTR2FLTR_HISTORY_OFFSET               (68u)
#define CapSense_MODE_PTR2FLTR_HISTORY_SIZE                 (4u)
#define CapSense_MODE_PTR2FLTR_HISTORY_PARAM_ID             (0xDB020044u)

#define CapSense_MODE_PTR2DEBOUNCE_VALUE                    (CapSense_dsFlash.wdgtArray[2].ptr2DebounceArr)
#define CapSense_MODE_PTR2DEBOUNCE_OFFSET                   (72u)
#define CapSense_MODE_PTR2DEBOUNCE_SIZE                     (4u)
#define CapSense_MODE_PTR2DEBOUNCE_PARAM_ID                 (0xD8020048u)

#define CapSense_MODE_STATIC_CONFIG_VALUE                   (CapSense_dsFlash.wdgtArray[2].staticConfig)
#define CapSense_MODE_STATIC_CONFIG_OFFSET                  (76u)
#define CapSense_MODE_STATIC_CONFIG_SIZE                    (4u)
#define CapSense_MODE_STATIC_CONFIG_PARAM_ID                (0xD902004Cu)

#define CapSense_MODE_TOTAL_NUM_SNS_VALUE                   (CapSense_dsFlash.wdgtArray[2].totalNumSns)
#define CapSense_MODE_TOTAL_NUM_SNS_OFFSET                  (80u)
#define CapSense_MODE_TOTAL_NUM_SNS_SIZE                    (2u)
#define CapSense_MODE_TOTAL_NUM_SNS_PARAM_ID                (0x90020050u)

#define CapSense_MODE_TYPE_VALUE                            (CapSense_dsFlash.wdgtArray[2].wdgtType)
#define CapSense_MODE_TYPE_OFFSET                           (82u)
#define CapSense_MODE_TYPE_SIZE                             (1u)
#define CapSense_MODE_TYPE_PARAM_ID                         (0x54020052u)

#define CapSense_MODE_NUM_COLS_VALUE                        (CapSense_dsFlash.wdgtArray[2].numCols)
#define CapSense_MODE_NUM_COLS_OFFSET                       (83u)
#define CapSense_MODE_NUM_COLS_SIZE                         (1u)
#define CapSense_MODE_NUM_COLS_PARAM_ID                     (0x52020053u)

#define CapSense_SPEECH_PTR2SNS_FLASH_VALUE                 (CapSense_dsFlash.wdgtArray[3].ptr2SnsFlash)
#define CapSense_SPEECH_PTR2SNS_FLASH_OFFSET                (84u)
#define CapSense_SPEECH_PTR2SNS_FLASH_SIZE                  (4u)
#define CapSense_SPEECH_PTR2SNS_FLASH_PARAM_ID              (0xDD030054u)

#define CapSense_SPEECH_PTR2WD_RAM_VALUE                    (CapSense_dsFlash.wdgtArray[3].ptr2WdgtRam)
#define CapSense_SPEECH_PTR2WD_RAM_OFFSET                   (88u)
#define CapSense_SPEECH_PTR2WD_RAM_SIZE                     (4u)
#define CapSense_SPEECH_PTR2WD_RAM_PARAM_ID                 (0xDE030058u)

#define CapSense_SPEECH_PTR2SNS_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[3].ptr2SnsRam)
#define CapSense_SPEECH_PTR2SNS_RAM_OFFSET                  (92u)
#define CapSense_SPEECH_PTR2SNS_RAM_SIZE                    (4u)
#define CapSense_SPEECH_PTR2SNS_RAM_PARAM_ID                (0xDF03005Cu)

#define CapSense_SPEECH_PTR2FLTR_HISTORY_VALUE              (CapSense_dsFlash.wdgtArray[3].ptr2FltrHistory)
#define CapSense_SPEECH_PTR2FLTR_HISTORY_OFFSET             (96u)
#define CapSense_SPEECH_PTR2FLTR_HISTORY_SIZE               (4u)
#define CapSense_SPEECH_PTR2FLTR_HISTORY_PARAM_ID           (0xD3030060u)

#define CapSense_SPEECH_PTR2DEBOUNCE_VALUE                  (CapSense_dsFlash.wdgtArray[3].ptr2DebounceArr)
#define CapSense_SPEECH_PTR2DEBOUNCE_OFFSET                 (100u)
#define CapSense_SPEECH_PTR2DEBOUNCE_SIZE                   (4u)
#define CapSense_SPEECH_PTR2DEBOUNCE_PARAM_ID               (0xD2030064u)

#define CapSense_SPEECH_STATIC_CONFIG_VALUE                 (CapSense_dsFlash.wdgtArray[3].staticConfig)
#define CapSense_SPEECH_STATIC_CONFIG_OFFSET                (104u)
#define CapSense_SPEECH_STATIC_CONFIG_SIZE                  (4u)
#define CapSense_SPEECH_STATIC_CONFIG_PARAM_ID              (0xD1030068u)

#define CapSense_SPEECH_TOTAL_NUM_SNS_VALUE                 (CapSense_dsFlash.wdgtArray[3].totalNumSns)
#define CapSense_SPEECH_TOTAL_NUM_SNS_OFFSET                (108u)
#define CapSense_SPEECH_TOTAL_NUM_SNS_SIZE                  (2u)
#define CapSense_SPEECH_TOTAL_NUM_SNS_PARAM_ID              (0x9F03006Cu)

#define CapSense_SPEECH_TYPE_VALUE                          (CapSense_dsFlash.wdgtArray[3].wdgtType)
#define CapSense_SPEECH_TYPE_OFFSET                         (110u)
#define CapSense_SPEECH_TYPE_SIZE                           (1u)
#define CapSense_SPEECH_TYPE_PARAM_ID                       (0x5B03006Eu)

#define CapSense_SPEECH_NUM_COLS_VALUE                      (CapSense_dsFlash.wdgtArray[3].numCols)
#define CapSense_SPEECH_NUM_COLS_OFFSET                     (111u)
#define CapSense_SPEECH_NUM_COLS_SIZE                       (1u)
#define CapSense_SPEECH_NUM_COLS_PARAM_ID                   (0x5D03006Fu)

#define CapSense_DIST_PTR2SNS_FLASH_VALUE                   (CapSense_dsFlash.wdgtArray[4].ptr2SnsFlash)
#define CapSense_DIST_PTR2SNS_FLASH_OFFSET                  (112u)
#define CapSense_DIST_PTR2SNS_FLASH_SIZE                    (4u)
#define CapSense_DIST_PTR2SNS_FLASH_PARAM_ID                (0xDF040070u)

#define CapSense_DIST_PTR2WD_RAM_VALUE                      (CapSense_dsFlash.wdgtArray[4].ptr2WdgtRam)
#define CapSense_DIST_PTR2WD_RAM_OFFSET                     (116u)
#define CapSense_DIST_PTR2WD_RAM_SIZE                       (4u)
#define CapSense_DIST_PTR2WD_RAM_PARAM_ID                   (0xDE040074u)

#define CapSense_DIST_PTR2SNS_RAM_VALUE                     (CapSense_dsFlash.wdgtArray[4].ptr2SnsRam)
#define CapSense_DIST_PTR2SNS_RAM_OFFSET                    (120u)
#define CapSense_DIST_PTR2SNS_RAM_SIZE                      (4u)
#define CapSense_DIST_PTR2SNS_RAM_PARAM_ID                  (0xDD040078u)

#define CapSense_DIST_PTR2FLTR_HISTORY_VALUE                (CapSense_dsFlash.wdgtArray[4].ptr2FltrHistory)
#define CapSense_DIST_PTR2FLTR_HISTORY_OFFSET               (124u)
#define CapSense_DIST_PTR2FLTR_HISTORY_SIZE                 (4u)
#define CapSense_DIST_PTR2FLTR_HISTORY_PARAM_ID             (0xDC04007Cu)

#define CapSense_DIST_PTR2DEBOUNCE_VALUE                    (CapSense_dsFlash.wdgtArray[4].ptr2DebounceArr)
#define CapSense_DIST_PTR2DEBOUNCE_OFFSET                   (128u)
#define CapSense_DIST_PTR2DEBOUNCE_SIZE                     (4u)
#define CapSense_DIST_PTR2DEBOUNCE_PARAM_ID                 (0xDE040080u)

#define CapSense_DIST_STATIC_CONFIG_VALUE                   (CapSense_dsFlash.wdgtArray[4].staticConfig)
#define CapSense_DIST_STATIC_CONFIG_OFFSET                  (132u)
#define CapSense_DIST_STATIC_CONFIG_SIZE                    (4u)
#define CapSense_DIST_STATIC_CONFIG_PARAM_ID                (0xDF040084u)

#define CapSense_DIST_TOTAL_NUM_SNS_VALUE                   (CapSense_dsFlash.wdgtArray[4].totalNumSns)
#define CapSense_DIST_TOTAL_NUM_SNS_OFFSET                  (136u)
#define CapSense_DIST_TOTAL_NUM_SNS_SIZE                    (2u)
#define CapSense_DIST_TOTAL_NUM_SNS_PARAM_ID                (0x93040088u)

#define CapSense_DIST_TYPE_VALUE                            (CapSense_dsFlash.wdgtArray[4].wdgtType)
#define CapSense_DIST_TYPE_OFFSET                           (138u)
#define CapSense_DIST_TYPE_SIZE                             (1u)
#define CapSense_DIST_TYPE_PARAM_ID                         (0x5704008Au)

#define CapSense_DIST_NUM_COLS_VALUE                        (CapSense_dsFlash.wdgtArray[4].numCols)
#define CapSense_DIST_NUM_COLS_OFFSET                       (139u)
#define CapSense_DIST_NUM_COLS_SIZE                         (1u)
#define CapSense_DIST_NUM_COLS_PARAM_ID                     (0x5104008Bu)

#define CapSense_ADAS_PTR2SNS_FLASH_VALUE                   (CapSense_dsFlash.wdgtArray[5].ptr2SnsFlash)
#define CapSense_ADAS_PTR2SNS_FLASH_OFFSET                  (140u)
#define CapSense_ADAS_PTR2SNS_FLASH_SIZE                    (4u)
#define CapSense_ADAS_PTR2SNS_FLASH_PARAM_ID                (0xDE05008Cu)

#define CapSense_ADAS_PTR2WD_RAM_VALUE                      (CapSense_dsFlash.wdgtArray[5].ptr2WdgtRam)
#define CapSense_ADAS_PTR2WD_RAM_OFFSET                     (144u)
#define CapSense_ADAS_PTR2WD_RAM_SIZE                       (4u)
#define CapSense_ADAS_PTR2WD_RAM_PARAM_ID                   (0xD8050090u)

#define CapSense_ADAS_PTR2SNS_RAM_VALUE                     (CapSense_dsFlash.wdgtArray[5].ptr2SnsRam)
#define CapSense_ADAS_PTR2SNS_RAM_OFFSET                    (148u)
#define CapSense_ADAS_PTR2SNS_RAM_SIZE                      (4u)
#define CapSense_ADAS_PTR2SNS_RAM_PARAM_ID                  (0xD9050094u)

#define CapSense_ADAS_PTR2FLTR_HISTORY_VALUE                (CapSense_dsFlash.wdgtArray[5].ptr2FltrHistory)
#define CapSense_ADAS_PTR2FLTR_HISTORY_OFFSET               (152u)
#define CapSense_ADAS_PTR2FLTR_HISTORY_SIZE                 (4u)
#define CapSense_ADAS_PTR2FLTR_HISTORY_PARAM_ID             (0xDA050098u)

#define CapSense_ADAS_PTR2DEBOUNCE_VALUE                    (CapSense_dsFlash.wdgtArray[5].ptr2DebounceArr)
#define CapSense_ADAS_PTR2DEBOUNCE_OFFSET                   (156u)
#define CapSense_ADAS_PTR2DEBOUNCE_SIZE                     (4u)
#define CapSense_ADAS_PTR2DEBOUNCE_PARAM_ID                 (0xDB05009Cu)

#define CapSense_ADAS_STATIC_CONFIG_VALUE                   (CapSense_dsFlash.wdgtArray[5].staticConfig)
#define CapSense_ADAS_STATIC_CONFIG_OFFSET                  (160u)
#define CapSense_ADAS_STATIC_CONFIG_SIZE                    (4u)
#define CapSense_ADAS_STATIC_CONFIG_PARAM_ID                (0xD70500A0u)

#define CapSense_ADAS_TOTAL_NUM_SNS_VALUE                   (CapSense_dsFlash.wdgtArray[5].totalNumSns)
#define CapSense_ADAS_TOTAL_NUM_SNS_OFFSET                  (164u)
#define CapSense_ADAS_TOTAL_NUM_SNS_SIZE                    (2u)
#define CapSense_ADAS_TOTAL_NUM_SNS_PARAM_ID                (0x990500A4u)

#define CapSense_ADAS_TYPE_VALUE                            (CapSense_dsFlash.wdgtArray[5].wdgtType)
#define CapSense_ADAS_TYPE_OFFSET                           (166u)
#define CapSense_ADAS_TYPE_SIZE                             (1u)
#define CapSense_ADAS_TYPE_PARAM_ID                         (0x5D0500A6u)

#define CapSense_ADAS_NUM_COLS_VALUE                        (CapSense_dsFlash.wdgtArray[5].numCols)
#define CapSense_ADAS_NUM_COLS_OFFSET                       (167u)
#define CapSense_ADAS_NUM_COLS_SIZE                         (1u)
#define CapSense_ADAS_NUM_COLS_PARAM_ID                     (0x5B0500A7u)


#endif /* End CY_SENSE_CapSense_REGISTER_MAP_H */


/* [] END OF FILE */
