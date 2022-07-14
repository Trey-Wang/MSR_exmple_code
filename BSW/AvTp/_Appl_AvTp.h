/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  _Appl_AvTp.h
 *    Component:  Demo Audio Application for Audio Video Bridging Transport Protocol
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Header of Demo Audio Application
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Dennis Mueller                visdmi           Vector Informatik GmbH
 *  Jeroen Laverman               vislje           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-10-14  visdmi  -             created
 *  01.01.00  2014-09-15  vislje  -             Updated to IEEE 1722a-D9
 *  01.01.01  2015-03-04  vislje  -             Fixed several compiler warnings
 *  01.01.02  2015-07-20  vislje  -             Fixed further compiler warnings (nested comment / compare uint8 < 256)
 *********************************************************************************************************************/

#if !defined (APPL_AVTP_H)
#define APPL_AVTP_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "AvTp_priv.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define AVTP_APP_FILTER_ORDER              (32) /* must ne power of two */
#define AVTP_APP_STREAM_COUNT              (1)

#define AVTP_APP_TONEA_MAX_VOLUME          (255)
#define AVTP_APP_TONEA_SINUS_LUT_COUNT     (28)
#define AVTP_APP_TONEA_HIGH_SAMPLE_NR      (27)
#define AVTP_APP_TONEA_LOW_SAMPLE_NR       (-27)
#define AVTP_APP_TONEA_MAX_PACKET_COUNT    (8)
#define AVTP_APP_TONEA_SIN_UP              (1)
#define AVTP_APP_TONEA_SIN_DOWN            (-1)
#define AVTP_APP_TONEA_SIN_STEPSIZE        (1191)
#define AVTP_APP_TONEA_SIN_MAX_SIZE        (32157) // 27*1191, max Value with Volume 1 of 255
#define AVTP_APP_TONEA_IS_REQUESTED        (0)
#define AVTP_APP_TONEA_IS_NOT_REQUESTED    (1)

#define AVTP_APP_TONEA_FREQUENCY           (444)
#define AVTP_APP_TONEA_NO_FREQUENCY        (0)

#define AVTP_APP_TONEE_IS_REQUESTED        (0)
#define AVTP_APP_TONEE_IS_NOT_REQUESTED    (1)
#define AVTP_APP_TONEE_HIGH_SAMPLE_NR      (36)
#define AVTP_APP_TONEE_LOW_SAMPLE_NR       (-36)
#define AVTP_APP_TONEE_SINUS_LUT_COUNT     (37)
#define AVTP_APP_TONEE_MAX_PACKET_COUNT    (8)

/* IEC 61883 values */
#define AVTP_IEC_HDR_LEN                   (8)
#define AVTP_IEC_CHANNEL_L                 (0)
#define AVTP_IEC_CHANNEL_R                 (1)
#define AVTP_IEC_CHANNEL_COUNT             (2)
#define AVTP_IEC_SAMPLE_COUNT              (6)

/* IEC 61883 defines */
#define AVTP_IEC_HDR_TAG_BIT_POS           (6)
#define AVTP_IEC_HDR_CHANNEL_BIT_POS       (0)
#define AVTP_IEC_HDR_TCODE_BIT_POS         (4)
#define AVTP_IEC_HDR_SY_BIT_POS            (0)
#define AVTP_IEC_HDR_SID_BIT_POS           (0)
#define AVTP_IEC_HDR_FN_BIT_POS            (6)
#define AVTP_IEC_HDR_QPC_BIT_POS           (3)
#define AVTP_IEC_HDR_SPH_BIT_POS           (2)
#define AVTP_IEC_HDR_RSV_BIT_POS           (0)
#define AVTP_IEC_HDR_FMT_BIT_POS           (0)
#define AVTP_IEC_HDR_QI_BIT_POS            (6)

/* PRQA S 3453 11*/ /* MD_MSR_19.7 */
#define AVTP_IEC_HDR_SY(param)             ( ((param) & 0x0F) >> AVTP_IEC_HDR_SY_BIT_POS )
#define AVTP_IEC_HDR_SID(param)            ( ((param) & 0x3F) >> AVTP_IEC_HDR_SID_BIT_POS )
#define AVTP_IEC_HDR_FN(param)             ( ((param) & 0xC0) >> AVTP_IEC_HDR_FN_BIT_POS )
#define AVTP_IEC_HDR_QPC(param)            ( ((param) & 0x38) >> AVTP_IEC_HDR_QPC_BIT_POS )
#define AVTP_IEC_HDR_SPH(param)            ( ((param) & 0x04) >> AVTP_IEC_HDR_SPH_BIT_POS )
#define AVTP_IEC_HDR_RSV(param)            ( ((param) & 0x03) >> AVTP_IEC_HDR_RSV_BIT_POS )
#define AVTP_IEC_HDR_FMT(param)            ( ((param) & 0x3F) >> AVTP_IEC_HDR_FMT_BIT_POS )
#define AVTP_IEC_824_SAMPLE(param)         ( ((param) & 0x00FFFFFF) )

#define AVTP_IEC_TAG_CIP_HDR_NOT_INCLUDED  (0)
#define AVTP_IEC_TAG_CIP_HDR_INCLUDED      (1)
#define AVTP_IEC_SPH_NO_SOURCE_PACKET      (0)
#define AVTP_IEC_CHANNEL_AVTP_SOURCE       (31)
#define AVTP_IEC_SID_AVTP_NETWORK          (63)
#define AVTP_IEC_TCODE                     (0x0A)
#define AVTP_IEC_QI1                       (0)
#define AVTP_IEC_QI2                       (2)
#define AVTP_IEC_SYT                       (0xFFFF)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* IEC 61883 Header */
typedef struct
{
  uint8    Qi1Sid;           /* First qualed indicator - Source ID */
  uint8    Dbs;              /* Data Block Size */
  uint8    FnQpcSphRsv;      /* Fraction Count - Quadlet Padding Count - Source Packet Header - Reserved */
  uint8    Dbc;              /* Data Block Count */
  uint8    Qi2Fmt;           /* Second qualed indicator - Stream format */
  uint8    Fdf;              /* Format dependent field */
  uint16   Syt;              /* Synchronisation timing */
} AvTp_Iec61883HdrType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_ListenerAudio1(
   AVTP_P2CONST(AvTp_DataType) DataPtr,
   AVTP_P2CONST(AvTp_RxPduInfoType) StreamInfoPtr,
   uint16 LenByte);
/*As long as there is no API to the RTE this Function should be global*/
FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio_ToneA(uint16 LenByte);

FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio_ToneE(uint16 LenByte);

FUNC(void, AVTP_CODE) AvTp_App_StateChangedCbk1(AvTp_StateType NewState);

FUNC(void, AVTP_CODE) AvTp_App_StateChangedCbk2(AvTp_StateType NewState);

FUNC(boolean, AVTP_CODE) AvTp_App_SetVolume(uint8 vol);

FUNC(boolean, AVTP_CODE) AvTp_App_Set_On_Off(boolean req);

FUNC(uint8 ,AVTP_CODE) AvTp_App_GetCurrentVolume(void);

FUNC(uint16 ,AVTP_CODE) AvTp_App_GetCurrentFrequency(void);

FUNC(void, AVTP_CODE) AvTp_App_Init(void);
#endif  /* APPL_AVTP_H */

/**********************************************************************************************************************
 *  END OF FILE: Appl_AvTp.h
 *********************************************************************************************************************/
