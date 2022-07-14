/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Rtp_Priv.h
 *       Module:  Tp_Rtp
 *    Generator:  Tp_Rtp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Types definitions of Realtime Transport Protocol (RTP)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (RTP_PRIV_H)
#define RTP_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Rtp.h"
#if (STD_ON == RTP_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* RTP_DEV_ERROR_DETECT */
#include "Rtp_Lcfg.h"
#include "SchM_Rtp.h"
#include "IpBase.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
#define RTP_BEGIN_CRITICAL_SECTION() SchM_Enter_Rtp_RTP_EXCLUSIVE_AREA_0()
#define RTP_END_CRITICAL_SECTION() SchM_Exit_Rtp_RTP_EXCLUSIVE_AREA_0()

#define RTP_LENGTH_IN_BYTES(Value)                    ((Value) * RTP_BYTES_PER_32WORD)
#define RTP_LENGTH_IN_32WORDS(Value)                  ((Value) / RTP_BYTES_PER_32WORD)

#define RTP_PUT_UINT8(Buffer, Offset, Value)          IPBASE_PUT_UINT8( (Buffer), (Offset), (Value))
#define RTP_PUT_UINT16(Buffer, Offset, Value)         IPBASE_PUT_UINT16((Buffer), (Offset), (Value))
#define RTP_PUT_UINT32(Buffer, Offset, Value)         IPBASE_PUT_UINT32((Buffer), (Offset), (Value))
#define RTP_PUT_UINT64(Buffer, Offset, Value)         do { (Buffer)[(Offset)+0] = (uint8)((uint64)(Value) >> 56); \
                                                           (Buffer)[(Offset)+1] = (uint8)((uint64)(Value) >> 48); \
                                                           (Buffer)[(Offset)+2] = (uint8)((uint64)(Value) >> 40); \
                                                           (Buffer)[(Offset)+3] = (uint8)((uint64)(Value) >> 32); \
                                                           (Buffer)[(Offset)+4] = (uint8)((uint64)(Value) >> 24); \
                                                           (Buffer)[(Offset)+5] = (uint8)((uint64)(Value) >> 16); \
                                                           (Buffer)[(Offset)+6] = (uint8)((uint64)(Value) >>  8); \
                                                           (Buffer)[(Offset)+7] = (uint8)((uint64)(Value)); } while(0)

#define RTP_GET_UINT8(Buffer, Offset, Variable)       IPBASE_GET_UINT8( (Buffer), (Offset), (Variable))
#define RTP_GET_UINT16(Buffer, Offset, Variable)      IPBASE_GET_UINT16((Buffer), (Offset), (Variable))
#define RTP_GET_UINT32(Buffer, Offset, Variable)      IPBASE_GET_UINT32((Buffer), (Offset), (Variable))
#define RTP_GET_UINT64(Buffer, Offset, Variable)      ((Variable) = (uint64)(((uint64)((Buffer)[(Offset)+0]) << 56) | \
                                                                             ((uint64)((Buffer)[(Offset)+1]) << 48) | \
                                                                             ((uint64)((Buffer)[(Offset)+2]) << 40) | \
                                                                             ((uint64)((Buffer)[(Offset)+3]) << 32) | \
                                                                             ((uint64)((Buffer)[(Offset)+4]) << 24) | \
                                                                             ((uint64)((Buffer)[(Offset)+5]) << 16) | \
                                                                             ((uint64)((Buffer)[(Offset)+6]) <<  8) | \
                                                                             ((uint64)((Buffer)[(Offset)+7]))))

/* PRQA L:FCT_MACROS */
#endif  /* RTP_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: Rtp_Priv.h
 *********************************************************************************************************************/
