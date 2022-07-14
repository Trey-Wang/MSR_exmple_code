/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CddDrm_Types.h
 *        \brief  CddDrm type definitions
 *
 *      \details  CddDrm definitions of structs and enumerations and simple types.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CDDDRM_TYPES_H
# define CDDDRM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* --- Component Version Information ------------------------------------------------------------------------------- */
# define CDDDRM_TYPES_MAJOR_VERSION     (1U)
# define CDDDRM_TYPES_MINOR_VERSION     (0U)
# define CDDDRM_TYPES_PATCH_VERSION     (0U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* --- Basic Types ------------------------------------------------------------------------------------------------- */
/* Vector specific types */

/*! Timeout counter type */
typedef uint16 CddDrm_TimerType;

/*! Response message length type */
typedef uint16 CddDrm_LengthType;

/*! Length of buffer */
typedef uint16 CddDrm_BufferLengthType;

/*! Drm specific return type */
typedef uint8 CddDrm_RequestReturnType;
# define CDDDRM_REQUEST_OK                               (0x00U) /*!< Return code: No error */
# define CDDDRM_REQUEST_NO_CHANNEL                       (0x01U) /*!< Return code: Channel can not be allocated */
# define CDDDRM_REQUEST_TESTER_ACTIVE                    (0x02U) /*!< Return code: External Tester is detected */
# define CDDDRM_REQUEST_FIREWALL_BLOCKED                 (0x03U) /*!< Return code: Service is blocked by Firewall Rule */
# define CDDDRM_REQUEST_CONNECTION_BUSY                  (0x04U) /*!< Return code: Connection is in use */

/*! Ecu Detection: Connection state */
typedef uint8 CddDrm_EcudStateType;
#define CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED            (0x00U)
#define CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE             (0x01U)
#define CDDDRM_ECUD_CONNECTION_AVAILABLE                 (0x02U)

/*! Connection channel state type */
typedef uint8 CddDrm_ChannelStateType;

/*! Connection channel id type */
typedef uint8 CddDrm_ChannelIdType;

/*! used to send Response/NRC to application  */
typedef uint8 CddDrm_ResponseCodeType;
# define CDDDRM_RESPONSE_POSITIVE                        (0x00U) /* Positive response */
# define CDDDRM_RESPONSE_TIMEOUT                         (0xd0U) /* Response timeout */
# define CDDDRM_RESPONSE_INVALID_NRC_LENGTH              (0xd1U) /* NRC message length does not equal 3byte */
# define CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED             (0xd2U) /* Received response pending message limit reached */
# define CDDDRM_RESPONSE_BUFFER_TOO_SMALL                (0xd3U) /* PduR receive buffer is too small */
# define CDDDRM_RESPONSE_WRONG_SERVICE                   (0xd4U) /* Received response is an invalid service */
# define CDDDRM_RESPONSE_CHANNELS_CLOSED                 (0xd5U) /* Channels are closed by application */
# define CDDDRM_RESPONSE_TESTER_DETECTED                 (0xd6U) /* External Tester is detected */
# define CDDDRM_RESPONSE_PDUR_RX_ERROR                   (0xd7U) /* Receive from PduR is failed */
# define CDDDRM_RESPONSE_PDUR_TX_ERROR                   (0xd8U) /* Data transmission to PduR failed */
# define CDDDRM_RESPONSE_PDUR_RX_CANCELED                (0xd9U) /* PduR canceled transmission */

/*! Configuration Type */
typedef P2CONST(uint8, TYPEDEF, CDDDRM_CONST_PBCFG) CddDrm_ConfigPtrType;

/*! Buffer pointer type */
typedef P2VAR(uint8, TYPEDEF, CDDDRM_APPL_DATA) CddDrm_BufferDataPtrType;

/*! Index for the static configured connection list */
typedef PduIdType CddDrm_ConnectionIdType;

/*! Used as function parameter to referencing data */
typedef struct
{
  CddDrm_BufferDataPtrType requestBufferDataPtr;
  CddDrm_BufferLengthType  responseBufferSize;
  CddDrm_BufferDataPtrType responseBufferDataPtr;
} CddDrm_BufferStructType;

/*! Response data for application */
typedef struct
{
  CddDrm_LengthType        responseLength; /* Length of data in respData */
  CddDrm_ResponseCodeType  responseCode;   /* PRC/NRC */
  CddDrm_ConnectionIdType  connectionId;   /* reference for static ConnectionConfig Table */
} CddDrm_RespInfoStructType;

/* Callback pointer */
typedef P2FUNC(void, CDDDRM_APPL_CODE, CddDrm_ResponseNotificationPtr)(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST));
typedef P2FUNC(void, CDDDRM_APPL_CODE, CddDrm_TxConfigurationPtr)(CddDrm_ConnectionIdType);
typedef P2FUNC(boolean, CDDDRM_APPL_CODE, CddDrm_FirewallUserCallbackPtr)(CddDrm_ConnectionIdType, uint8);
typedef P2FUNC(void, CDDDRM_APPL_CODE, CddDrm_EcuDetectionFinishPtr)(void);
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CDDDRM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: CddDrm_Types.h
 *********************************************************************************************************************/