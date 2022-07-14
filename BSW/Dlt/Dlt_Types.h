/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dlt_Types.h
 *        \brief  Dlt Types header file
 *
 *      \details  Types header file of the AUTOSAR Diagnostic Log and Trace, according to:
 *                AUTOSAR Diagnostic Log and Trace, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if (!defined DLT_TYPES_H_)
# define DLT_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dlt_Cfg.h"
# include "ComStack_Types.h"
# include "Rte_Dlt_Type.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* File version information */
# define DLT_TYPES_MAJOR_VERSION    (1u)
# define DLT_TYPES_MINOR_VERSION    (7u)
# define DLT_TYPES_PATCH_VERSION    (2u)

typedef uint8 Dlt_ConfigType; /* [Dlt437] */

# define DLT_OK                     0x0u
# define DLT_NOT_SUPPORTED          0x1u
# define DLT_ERROR                  0x2u

# define DLT_NOT_OK                 0x1u

# define DLT_OFF                    0x0u
# define DLT_ON                     0x1u

/* Dlt_GlobalStateType */
#define DLT_GLOBAL_STATE_UNINIT    0x0u
/* DLT_GLOBAL_STATE_OFFLINE and DLT_GLOBAL_STATE_ONLINE are defined in Rte_Dlt_Type.h */

/* Keyword macros */
# if !defined (STATIC)                 /* COV_DLT_MSR_COMPATIBILITY */
# define STATIC static
# endif

# if !defined (LOCAL_INLINE)           /* COV_DLT_MSR_COMPATIBILITY */
# define LOCAL_INLINE INLINE STATIC
# endif

# if !defined (DLT_LOCAL)              /* COV_DLT_MSR_COMPATIBILITY */
# define DLT_LOCAL STATIC
# endif

# if !defined (DLT_LOCAL_INLINE)       /* COV_DLT_MSR_COMPATIBILITY */
# define DLT_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/*! \typedef Variables of this type are used to represent the error codes Dlt can return */
typedef Std_ReturnType Dlt_ReturnType;   /*!< Return type: defining the possible return values of Dlt APIs. */ /* [Dlt238]; ESCAN00082472 */
# define DLT_E_OK                   0x0u /*!< Return type value: no error occurred. */
# define DLT_E_INT_BUFFER_FULL      0x1u /*!< Return type value: [not used].  */
# define DLT_E_MSG_TOO_LARGE        0x2u /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called with too long message. */
# define DLT_E_CONTEXT_ALREADY_REG  0x3u /*!< Return type value: API service Dlt_RegisterContext called repeatedly with same context. */
# define DLT_E_UNKNOWN_SESSION_ID   0x4u /*!< Return type value: API service Dlt_RegisterContext, Dlt_SendLogMessage or Dlt_SendTraceMessage called with invalid session ID. */
# define DLT_E_IF_NOT_AVAILABLE     0x5u /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called but no connection to lower layer available. */
# define DLT_E_IF_BUSY              0x6u /*!< Return type value: [not used]. */
# define DLT_E_ERROR_UNKNOWN        0x7u /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called with invalid config. */
# define DLT_E_PENDING              0x8u /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called while other application process is still running. */
# define DLT_E_NOT_IN_VERBOSE_MODE  0x9u /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called with verbose mode but feature not enabled. */

/* ESCAN00082472 */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if ((RTE_E_OK != (E_OK)) || (DLT_E_OK != (E_OK)))
#   error "Return values are not interpreted consistently"
#  endif
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/* Dlt458 */
typedef struct
{
  uint8  HeaderType;                   /*!< Standard header type member: contains flags defining size and content of standard header. */ /* Dlt094: HTYP */
  uint8  MessageCounter;               /*!< Standard header type member: contains message counter. */ /* Dlt319: MCNT */
  uint16 Length;                       /*!< Standard header type member: contains length of complete Dlt message. */ /* Dlt320: Length (LEN) shall hold the total length of non-static data of the Dlt message in byte. */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  uint32 EcuID;                        /*!< Standard header type member: contains ECU ID defined with DLT_ECU_ID. */ /* Optional: depending on HeaderType 0bxx xx x1 xx */
  uint32 SessionID;                    /*!< Standard header type member: contains session ID added by Rte. */ /* Optional: depending on HeaderType 0bxx xx 1x xx */
  uint32 Timestamp;                    /*!< Standard header type member: contains timestamp of current Dlt message. */ /* Optional: depending on HeaderType 0bxx x1 xx xx */
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
} Dlt_StandardHeaderType;              /*!< Standard header type: structure type for standard header of Dlt messages. */

/* Dlt228 */
typedef uint32 Dlt_MessageIDType;      /*!< Message ID type: defining the type of message ID. */

# if (DLT_USE_COMLAYER_XCP == STD_ON)
typedef uint32 Dlt_DemEventStatusType; /*!< Dem event status type: defining the type of an Dem event status. */

# else /* (DLT_USE_COMLAYER_XCP == STD_ON) */
typedef uint8 Dlt_MessageTypeType;     /*!< Message type type: defining the type of Dlt message. */
# define DLT_TYPE_LOG               0u /*!< Message type type value: a log message. */
# define DLT_TYPE_APP_TRACE         1u /*!< Message type type value: a application trace message. */
# define DLT_TYPE_NW_TRACE          2u /*!< Message type type value: a network trace message. */
# define DLT_TYPE_CONTROL           3u /*!< Message type type value: a control message. */

typedef uint8 Dlt_SoConModeType;       /*!< Socket connection mode type: defining the type of socket connection state. */
# define DLT_SOCON_ONLINE           0u /*!< Socket connection mode type value: socket connection is online. */
# define DLT_SOCON_RECONNECT        1u /*!< Socket connection mode type value: socket connection tries to reconnect. */
# define DLT_SOCON_OFFLINE          2u /*!< Socket connection mode type value: socket connection is offline. */

/* States of the diagnostic log and trace state machine */
typedef uint8 Dlt_StateType;           /*!< State type: defining the type of sub state machine. */
# define DLT_STATE_UNINIT           0u /*!< State type value: the DLT is uninitialized. */
# define DLT_STATE_WAIT_FOR_RECEP   1u /*!< State type value: the sending of log and trace message can start if a DLT message was received. */
# define DLT_STATE_READY_TO_SEND    2u /*!< State type value: waits send buffer content. */
# define DLT_STATE_TRY_SENDING      3u /*!< State type value: tries to transmit a message, if rejected goto previous state. */
# define DLT_STATE_SENDING          4u /*!< State type value: wait for successful transmission. */

/* Number of states */
# define DLT_STATE_NUMBER_OF_STATES  (0x05u)

# endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
#endif /* DLT_TYPES_H_ */

