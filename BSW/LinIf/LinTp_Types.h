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
 *         File:  LinTp_Types.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4TpLin
 *    Generator:  If_Asr4TpLin
 *
 *  Description:  Declaration of types and defines of the AUTOSAR LIN Transport Protocol
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Bastian Molkenthin            visbmo        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-07-20  visbmo  ESCAN00057174 AR4 Support (derived from AR3 LinIf/LinTp)
 *  01.01.00  2013-01-22  visbmo  ESCAN00064415 Compiler warning: Conversion from 'uint16' to 'PduLengthType'
 *  01.02.00  2013-04-02  visbmo  ESCAN00064305 AR4-325: Implement Post-Build Loadable
 *  02.00.00  2013-09-10  visbmo  ESCAN00069725 AR4-389: Support for multiple TP connections to the same slave
 *  03.00.00  2014-04-14  visbmo  ESCAN00076251 Support of ComStackLib
 *  04.01.00  2015-01-07  visbmo  ESCAN00080299 FEAT-427: SafeBSW Step I
 *  04.02.00  2015-05-18  visbmo  ESCAN00081787 FEAT-1275: SafeBSW Step 2
 *  05.00.00  2015-11-05  visbmo  ESCAN00084958 Adapt to updated R14 generators (breaking change and CSL)
 *                                ESCAN00085822 Compiler error: LinTp_IsInvalidHndOfTxNSdu undefined
 *                                ESCAN00086031 Compiler error: Linker cannot found symbol LinIf_WakeupConfirmation
 *                                ESCAN00086282 Rework wakeup source handling
 *  05.01.00  2016-02-04  visbmo  ESCAN00087805 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00087859 FEAT-1731: J2602 Frame Tolerance Support
 *  05.01.01  2016-07-07  visbmo  ESCAN00090787 Communication starts after external wakeup before wakeup delay has elapsed
 *  05.02.00  2017-01-19  visbmo  FEATC-565     FEAT-2161: Support for LIN drivers with different interface type
 *                                FEATC-568     FEAT-2263: LIN ScheduleTable change after frame transmission
 *                                ESCAN00093427 Support for callback to provide Lin status to application
 *                                ESCAN00093428 Support for schedule table handler information API
 *                                ESCAN00093616 Support for particular wakeup delay after external wakeup
 *********************************************************************************************************************/

#if !defined (LINTP_TYPES_H)
# define LINTP_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* include ComStack_Types.h so all needed types are known */
# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define LINTP_TPS_MAJOR_VERSION                    (5u)
# define LINTP_TPS_MINOR_VERSION                    (2u)
# define LINTP_TPS_PATCH_VERSION                    (0u)

/* generator compatibility */
# define LINTP_GENERATOR_COMPATIBILITY_VERSION      ((uint16)0x0104u)

/* type definitions of LinTp */
# define LINTP_FUNC_REQUEST_IDLE                    (0x00u)
# define LINTP_FUNC_REQUEST_PENDING                 (0x01u)
# define LINTP_FUNC_REQUEST_TRANSMITTED             (0x02u)

# define LINTP_LOCAL_BUFFER_SIZE                    (8u) /* currently must be a multiple of 2 because of mask definition below */
# define LINTP_LOCAL_BUFFER_SIZE_MASK               (LINTP_LOCAL_BUFFER_SIZE - 1)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8                                                     LinTp_ChannelType;
typedef P2CONST(LinTp_ChannelType, TYPEDEF, LINTP_PBCFG)          LinTp_ChannelTypePtrType;

typedef struct LinTp_ControlTypeTag /*! Control structure of LinTp for one channel */
{
  uint16                           LinTp_Timer;               /*!< Current timer value for timeout handling               */
  uint16                           LinTp_TimerReload;         /*!< Timer timeout value depends on next awaited frame type */
  uint16                           LinTp_P2Timer;             /*!< Storing current timer value used for P2 and P2max      */
  uint16                           LinTp_RespPendingCounter;  /*!< Number of received response pending frames             */

  PduLengthType                    LinTp_RemainingLength;     /*!< Bytes to be stored in PduR buffer / to be transmitted  */
  PduIdType                        LinTp_Curr_UL_NSdu_Id;     /*!< Network layer Pdu ID of current request / response     */

  PduInfoType                      LinTp_PduInfoPtr;          /*!< PduInfo structure passed to PduR                       */
  PduIdType                        LinTp_Curr_Rx_Id;          /*!< Internal ID of RxNSdu belonging to current LinTpTxNSdu */

# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  PduInfoType                      LinTp_FuncPduInfoPtr;      /*!< PduInfo structure passed to PduR for functional req    */
  PduIdType                        LinTp_Curr_Func_NSdu_Id;   /*!< Network layer Pdu ID of functional request             */
  uint8                            LinTp_ComModeRestore;      /*!< Recovery to correct schedule after functional request  */
  uint8                            LinTp_FuncRequest;         /*!< Status of the functional request                       */
# endif

  uint8                            LinTp_Ch_State;            /*!< State engine                                           */
  uint8                            LinTp_NAD_Requested;       /*!< NAD of request - responses will be compared against it */
  uint8                            LinTp_Tx_Rx_Buffer_Idx;    /*!< Null based index of Tp buffer                          */
  uint8                            LinTp_Tx_Rx_Buffer_Length; /*!< Number of payload bytes of current frame               */
  uint8                            LinTp_Tx_Rx_Buffer[LINTP_LOCAL_BUFFER_SIZE];     /*!< Local Tp buffer                  */
  uint8                            LinTp_SN;                  /*!< Sequence Number of CFs                                 */

  uint8                            LinTp_PortNotification;    /*!< Indication flag to change the schedule table for diag  */

  uint8                            LinTp_PduRNotification;    /*!< Indication flag to notify PduR on Tp Tx/Rx frame       */
  uint8                            LinTp_PduRNotifyResultTx;  /*!< Result of Tx notification to PduR                      */
  PduIdType                        LinTp_PduRNSduIdTx;        /*!< Tx NsduId which shall be reported to PduR              */
  uint8                            LinTp_PduRNotifyResultRx;  /*!< Result of Rx notification to PduR                      */
  PduIdType                        LinTp_PduRNSduIdRx;        /*!< Rx NsduId which shall be reported to PduR              */
  PduIdType                        LinTp_PduRNSduIdTm;        /*!< NsduId which shall be reported to PduR                 */
} LinTp_ControlType;

#endif /* LINTP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: LinTp_Types.h
 *********************************************************************************************************************/

