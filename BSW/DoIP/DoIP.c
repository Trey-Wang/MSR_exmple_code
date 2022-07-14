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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  DoIP.c
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Implementation file of Diagnostic over IP
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define DOIP_SOURCE

/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "DoIP.h" /* includes ComStack_Types.h */
#include "DoIP_Priv.h" /* includes SoAd.h */
#include "DoIP_Cbk.h"
#include "IpBase.h"
#include "PduR_DoIP.h"

#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check consistency of source and header file. */
#if ( (DOIP_SW_MAJOR_VERSION != 4u) || (DOIP_SW_MINOR_VERSION != 4u) || (DOIP_SW_PATCH_VERSION != 0u) )
# error "DoIP.c: Source and Header file are inconsistent!"
#endif /* (DOIP_SW_MAJOR_VERSION != 4u) || (DOIP_SW_MINOR_VERSION != 4u) || (DOIP_SW_PATCH_VERSION != 0u) */

/* Check configuration variant for building the library */
#if ( (defined(V_EXTENDED_BUILD_LIB_CHECK)) && \
    (DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_PRECOMPILE) )
# error "DoIP.c: Switch configuration variant to link-time or post-build for library build!"
#endif /* (defined(V_EXTENDED_BUILD_LIB_CHECK)) && \
        (DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_PRECOMPILE) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if ( !defined (DOIP_LOCAL) )
# define DOIP_LOCAL static
#endif /* !defined (DOIP_LOCAL) */

#if ( !defined (DOIP_LOCAL_INLINE) )
# define DOIP_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (DOIP_LOCAL_INLINE) */

#define DOIP_GEN_HDR_LEN_BYTE                 8u
#define DOIP_DIAG_HDR_LEN_BYTE                4u

#if ( DOIP_VIN_GID_SYNC == STD_ON )
# define DOIP_ACK_VEHICLE_IDENT_LEN_BYTE      33u
#else
# define DOIP_ACK_VEHICLE_IDENT_LEN_BYTE      32u
#endif /* DOIP_VIN_GID_SYNC == STD_ON */

#if ( DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
# define DOIP_ACK_ENTITY_STATE_LEN_BYTE       7u
#else
# define DOIP_ACK_ENTITY_STATE_LEN_BYTE       3u
#endif /* DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

#if ( DOIP_VERSION == DOIP_ISO_DIS_13400_2_2010 )
# define DOIP_PROTOCOL_VERSION                DOIP_PROTOCOL_VERSION_2010
#endif /* DOIP_VERSION == DOIP_ISO_DIS_13400_2_2010 */
#if ( DOIP_VERSION == DOIP_ISO_FDIS_13400_2_2011 )
# define DOIP_PROTOCOL_VERSION                DOIP_PROTOCOL_VERSION_2011
#endif /* DOIP_VERSION == DOIP_ISO_FDIS_13400_2_2011 */
#if ( DOIP_VERSION == DOIP_ISO_13400_2_2012 )
# define DOIP_PROTOCOL_VERSION                DOIP_PROTOCOL_VERSION_2012
#endif /* DOIP_VERSION == DOIP_ISO_13400_2_2012 */

#define DOIP_VIN_PREFIX_LEN_BYTE              3u
#define DOIP_VIN_LEN_BYTE                     17u
#define DOIP_GID_LEN_BYTE                     6u
#define DOIP_EID_LEN_BYTE                     6u
#define DOIP_ADDR_LEN_BYTE                    2u
#define DOIP_ADDR_ANY                         ((uint16)0xFFFFu)
#define DOIP_INV_TESTER_IDX                   ((uint8)   0xFFu)
#define DOIP_INV_ROUT_ACT_IDX                 ((uint8)   0xFFu)
#define DOIP_GEN_HDR_TYPE_IDX                 2u
#define DOIP_GEN_HDR_NACK_CODE_LEN_BYTE       1u
#if ( DOIP_VERSION == DOIP_ISO_13400_2_2012 )
# define DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE     7u
# define DOIP_ROUT_ACTIV_REQ_MAX_LEN_BYTE     11u
# define DOIP_ROUT_ACTIV_REQ_TYPE_LEN_BYTE    1u
#else
# define DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE     8u
# define DOIP_ROUT_ACTIV_REQ_MAX_LEN_BYTE     12u
# define DOIP_ROUT_ACTIV_REQ_TYPE_LEN_BYTE    2u
#endif /* DOIP_VERSION == DOIP_ISO_13400_2_2012 */
#define DOIP_ROUT_ACTIV_REQ_RSVD_LEN_BYTE     4u
#define DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE      4u
#define DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE      9u
#define DOIP_ROUT_ACTIV_ACK_MAX_LEN_BYTE      13u
#define DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE      4u
#define DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE        4u
#define DOIP_DIAG_MSG_ACK_LEN_BYTE            5u

#define DOIP_VMIN_HOST_NAME_SIZE              5u

#if ( !defined(DOIP_UDP_MAX_RETRY_CNT) )
/* depending on MainFunction cycle: 5ms -> 60s */
# define DOIP_UDP_MAX_RETRY_CNT               12000u
#endif /* !defined(DOIP_UDP_MAX_RETRY_CNT) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA S 3453 1 */ /* MD_MSR_19.1 */
#define DoIP_VTcpTxQueueGetLevel(Connection)  DoIP_VTcpCon2TxMgtQueLvl[(Connection)]

/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define DOIP_MIN(a, b)                        (((a) <= (b)) ? (a) : (b))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct
{
  uint8 Ver;         /* Version */
  uint8 InvVer;      /* Bit-Inverted version */
  uint8 Type[2u];    /* Payload type */
  uint8 LenByte[4u]; /* Payload length */
} DoIP_HdrType;

typedef struct
{
  DoIP_HdrType GenHdr;
  uint8        NackCode;
} DoIP_HdrNackType;

typedef struct
{
  uint8 TestAddr[DOIP_ADDR_LEN_BYTE];                 /* Logical address of Diagnostic Tester */
  uint8 ActivType[DOIP_ROUT_ACTIV_REQ_TYPE_LEN_BYTE]; /* Routing Activation Type */
  uint8 Reserved[DOIP_ROUT_ACTIV_REQ_RSVD_LEN_BYTE];  /* Reserved by DoIP specification */
  uint8 Oem[DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE];        /* OEM specific extension */
} DoIP_RoutActiveReqType;

typedef struct
{
  uint8 SrcAddr[DOIP_ADDR_LEN_BYTE]; /* Logical address of Gateway */
  uint8 TgtAddr[DOIP_ADDR_LEN_BYTE]; /* Logical address of Diagnostic Tester */
} DoIP_DiagReqType;

typedef struct
{
  DoIP_HdrType GenHdr;
  uint8        DiagPowerMode;
} DoIP_PowerModeAckType;

typedef struct
{
  DoIP_HdrType GenHdr;
  uint8        NodeType;        /* DOIP_GATEWAY / DOIP_NODE */
  uint8        MaxTcpSockCnt;   /* Max number of concurrent TCP sockets */
  uint8        TcpSockCnt;      /* Current number of open TCP sockets */
#if ( DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
  uint8        MaxDataSize[4u]; /* Maximum size of one logical request that this DoIP entity can process */
#endif /* DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */
} DoIP_EntityStateAckType;

/* PRQA L:MACRO_LIMIT */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define DOIP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

DOIP_LOCAL VAR(DoIP_StateType, DOIP_VAR_ZERO_INIT) DoIP_State = DOIP_STATE_UNINIT;

#define DOIP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*****************************************************/

#define DOIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

DOIP_LOCAL VAR(DoIP_ActivationLineLocalType, DOIP_VAR_NOINIT) DoIP_ActivationLineLocalState;
DOIP_LOCAL VAR(DoIP_ActivationLineType, DOIP_VAR_NOINIT) DoIP_ActivationLineState;

#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
DOIP_LOCAL VAR(boolean, DOIP_VAR_NOINIT) DoIP_IsPduSizeRoutingEnabled;
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */

#if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
DOIP_LOCAL VAR(boolean, DOIP_VAR_NOINIT) DoIP_GidInSync;
#endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

#define DOIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define DOIP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  DoIP_Util_FillGenHdr()
 *********************************************************************************************************************/
/*! \brief          Fills generic header data into buffer.
 *  \details        -
 *  \param[in]      HdrType       Header type.
 *  \param[in]      HdrLen        Header length.
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGenHdr(
  uint16 HdrType,
  uint32 HdrLen,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillVin()
 *********************************************************************************************************************/
/*! \brief          Fills VIN data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \return         E_OK          VIN successful written to buffer.
 *  \return         E_NOT_OK      VIN invalid pattern is written to buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillVin(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillLocalLogAddr()
 *********************************************************************************************************************/
/*! \brief          Fills logical address data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillLocalLogAddr(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillEid()
 *********************************************************************************************************************/
/*! \brief          Fills EID data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Data successful written.
 *  \return         E_NOT_OK      Data not successful written.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillEid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_Util_FillGid()
 *********************************************************************************************************************/
/*! \brief          Fills GID data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Data successful written.
 *  \return         E_NOT_OK      Data not successful written.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillGid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_Util_FillGidOrInv()
 *********************************************************************************************************************/
/*! \brief          Fills GID or invalid data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGidOrInv(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillFurtherAction()
 *********************************************************************************************************************/
/*! \brief          Fills Central Security data into buffer.
 *  \details        -
 *  \param[in,out]  TgtBufferPtr  Pointer to target buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillFurtherAction(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Con_SoConInitAll()
 *********************************************************************************************************************/
/*! \brief          Initiates connections by extracting socket connection identifier from SoAd if not done before.
 *  \details        -
 *  \param[in,out]  ErrorIdPtr  Pointer to DET error if one occurred.
 *  \return         E_OK        All socket connections are initialized or already initialized.
 *  \return         E_NOT_OK    Not all socket connections are initzialized.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConInitAll(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_Con_SoConOpen()
 *********************************************************************************************************************/
/*! \brief          Opens a socket connection initially after IP address assignment.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConOpen(
  PduIdType ConIdx);

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_SoConCloseAll()
 *********************************************************************************************************************/
/*! \brief          Closes all socket connections.
 *  \details        -
 *  \return         E_OK      All socket connections are already closed.
 *  \return         E_NOT_OK  Not all socket connections are closed. Closing is requested but not yet performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConCloseAll(void);
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_SoConClose()
 *********************************************************************************************************************/
/*! \brief          Closes single socket connection.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConClose(
  PduIdType ConIdx);

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_ReqAllIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Requests all IP address assignments and sets DHCP hostname option.
 *  \details        Iterates over all UdpConnections and request IP address assignment on one UdpConnection of each 
 *                  local address and read/write DHCP option.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ReqAllIpAddrAssign(void);
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Requests single IP address assignment and sets DHCP hostname option.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ReqIpAddrAssign(
  uint8 LocalAddrIdx);
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignStatic()
 *********************************************************************************************************************/
/*! \brief          Requests STATIC IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Address is successfully assigned or assignment type differs.
 *  \return         E_NOT_OK      Address assignment failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_STATIC
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignStatic(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignLinkLocalDoIP()
 *********************************************************************************************************************/
/*! \brief          Requests LINKLOCAL DOIP IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Address is successfully assigned or assignment type differs.
 *  \return         E_NOT_OK      Address assignment failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignLinkLocalDoIP(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignLinkLocal()
 *********************************************************************************************************************/
/*! \brief          Requests LINKLOCAL IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Address is successfully assigned or assignment type differs.
 *  \return         E_NOT_OK      Address assignment failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignLinkLocal(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignIpV6Router()
 *********************************************************************************************************************/
/*! \brief          Requests IPV6ROUTER IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Address is successfully assigned or assignment type differs.
 *  \return         E_NOT_OK      Address assignment failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignIpV6Router(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_SetDhcpHostname()
 *********************************************************************************************************************/
/*! \brief          Sets DHCP hostname option.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          DHCP hostname option is written successfully.
 *  \return         E_NOT_OK      DHCP hostname option is not written successfully.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SetDhcpHostname(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
#endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignDhcp()
 *********************************************************************************************************************/
/*! \brief          Requests DHCP IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \param[in]      ConIdx        Connection Index.
 *  \return         E_OK          Address is successfully assigned or assignment type differs.
 *  \return         E_NOT_OK      Address assignment failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignDhcp(
  uint8 LocalAddrIdx,
  PduIdType ConIdx);
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelAllIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Releases all IP address assignments.
 *  \details        Iterate over all UdpConnections and release IP address assignment on one UdpConnection of each
 *                  local address and check if all local addresses are unassigned.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelAllIpAddrAssign(void);
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Releases single IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx Local address index
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelIpAddrAssign(
  uint8 LocalAddrIdx);
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_GetConIdxBySoConId()
 *********************************************************************************************************************/
/*! \brief          Gets the connction index of a socket connection.
 *  \details        -
 *  \param[in]      SoConId     Socket connection identifier.
 *  \param[out]     ConIdx      Connection Index.
 *  \return         E_OK        Corresponding Connection found.
 *  \return         E_NOT_OK    Corresponding Connection not found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_GetConIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(PduIdType, AUTOMATIC, DOIP_APPL_VAR) ConIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConStates()
 *********************************************************************************************************************/
/*! \brief          Handles socket connection states (i.e. reopens closed socket connections).
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE for TCP connections. TRUE for UDP connections.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConStates(void);

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_HandleIpAddrReassignment()
 *********************************************************************************************************************/
/*! \brief          Handles reassignment of IP address including DHCP hostname option.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED & DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN &
 *                  DOIP_IPADDR_ASSIGN_PATTERN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleIpAddrReassignment(void);
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_HandleGidSync()
 *********************************************************************************************************************/
/*! \brief          Handles GID synchronization.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_VGID_SYNC_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleGidSync(void);
#endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_ActLineToActive()
 *********************************************************************************************************************/
/*! \brief          Handles transition of activation line state from inactive to active.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ActLineToActive(void);

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_ActLineToInactive()
 *********************************************************************************************************************/
/*! \brief          Handles transition of activation line state from active to inactive.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_VACTIV_LINE_CALLBACK_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ActLineToInactive(void);
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_UdpTx_Transmit()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages immediately or delayed.
 *  \details        Depending on message type the message is transmitted immediately or delayed. If transmit failed
 *                  message gets delayed by adding the transmit to the retry list.
 *  \param[in]      ConIdx        Connection Index.
 *  \param[in]      HdrType       Message header type.
 *  \param[in]      Param         Parameter depending on header type (e.g. negative acknowledge code).
 *  \param[in]      CloseSocket   Indicates if a socket shall be closed after transmission.
 *  \return         E_OK          Transmission requested.
 *  \return         E_NOT_OK      Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_Transmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_UdpTx_TransmitImmediately()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx        Connection Index.
 *  \param[in]      HdrType       Message header type.
 *  \param[in]      Param         Parameter depending on header type (e.g. negative acknowledge code).
 *  \param[in]      CloseSocket   Indicates if a socket shall be closed after transmission.
 *  \return         E_OK          Transmission requested.
 *  \return         E_NOT_OK      Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_TransmitImmediately(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_UdpTx_TransmitDelayed()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages delayed.
 *  \details        Stores transmission request for DoIP messages (UDP) in a retry list.
 *  \param[in]      ConIdx        Connection Index.
 *  \param[in]      HdrType       Message header type.
 *  \param[in]      Param         Parameter depending on header type (e.g. negative acknowledge code).
 *  \param[in]      CloseSocket   Indicates if a socket shall be closed after transmission.
 *  \return         E_OK          Transmission requested.
 *  \return         E_NOT_OK      Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_TransmitDelayed(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmit()
 *********************************************************************************************************************/
/*! \brief          Transmits single UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in]      HdrType   Message header type.
 *  \param[in]      Param     Parameter depending on header type (e.g. negative acknowledge code).
 *  \return         E_OK      Transmission requested.
 *  \return         E_NOT_OK  Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param);

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitGenNack()
 *********************************************************************************************************************/
/*! \brief          Transmits Generic Negative Acknowledge UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in]      HdrType   Message header type.
 *  \param[in]      NackCode  Negative acknowledge code.
 *  \return         E_OK      Transmission requested.
 *  \return         E_NOT_OK  Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitGenNack(
  PduIdType ConIdx,
  uint16 HdrType,
  uint8 NackCode);

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Vehicle Announcements and Vehicle Identification Responses immediately.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \return         E_OK      Transmission requested.
 *  \return         E_NOT_OK  Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Diagnostic Power Mode Info Response UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      DiagPowerMode   Diagnostic Power Mode.
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes(
  PduIdType ConIdx,
  uint8 DiagPowerMode);

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitEntiyStateRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Entity State Response UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \return         E_OK      Transmission requested.
 *  \return         E_NOT_OK  Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_ENTITY_STATE_SUPPORT
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitEntiyStateRes(
  PduIdType ConIdx);
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitOemSpecific()
 *********************************************************************************************************************/
/*! \brief          Transmits Oem Specific UDP messages immediately.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      HdrTypeOem  OEM specific message header type.
 *  \return         E_OK        Transmission requested.
 *  \return         E_NOT_OK    Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_VOEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitOemSpecific(
  PduIdType ConIdx,
  uint16 HdrTypeOem);
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_UpdTxList_CloseSocket()
 *********************************************************************************************************************/
/*! \brief          Handles socket connection closing for UdpTxList entries.
 *  \details        -
 *  \param[in]      UdpTxListIdx  List index of handled entry.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_CloseSocket(
  uint16 UdpTxListIdx);

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntries()
 *********************************************************************************************************************/
/*! \brief          Handles pending UdpTxList entries.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntries(void);

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*! \brief          Handles pending UdpTxList entries for Vehicle Announcements and Vehicle Identification Responses.
 *  \details        -
 *  \param[in]      ListIdx   List index of handled entry.
 *  \param[in]      ConIdx    Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes(
  PduIdType ListIdx,
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntriesOthers()
 *********************************************************************************************************************/
/*! \brief          Handles pending UdpTxList entries for Generic Negative Acknowledge, Entity State Status and
 *                  Diagnostic Power Mode Responses.
 *  \details        -
 *  \param[in]      ListIdx           List index of handled entry.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[out]     ReleaseListEntry  Indicates if entry is handled and can be released from list.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntriesOthers(
  PduIdType ListIdx,
  PduIdType ConIdx,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) ReleaseListEntry);

/**********************************************************************************************************************
 *  DoIP_TcpTx_Transmit()
 *********************************************************************************************************************/
/*! \brief          Requests a transmission of a TCP message.
 *  \details        -
 *  \param[in]      ConIdx        Connection Index.
 *  \param[in]      HdrType       Message header type.
 *  \param[in]      Param1        First parameter depending on header type.
 *  \param[in]      Param2        Second parameter depending on header type.
 *  \param[in]      Param3        Third parameter depending on header type.
 *  \param[in]      DataPtr       Pointer to data depending on header type.
 *  \param[in]      CloseSocket   Indicates if a socket shall be closed after transmission.
 *  \return         E_OK          Request successful.
 *  \return         E_NOT_OK      Request failed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpTx_Transmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  SduDataPtrType DataPtr,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TcpTx_PrepareTransmit()
 *********************************************************************************************************************/
/*! \brief          Stores a transmit request for a message which is not ready yet.
 *  \details        In case of a received diagnostic message a corresponding negative acknowledge may be sent. If the
 *                  message is not yet received completely the transmission is prepared with this function but
 *                  performed when the message is received completely to append the diagnostic data to the negative
 *                  acknowledge message.
 *  \param[in]      ConIdx        Connection Index.
 *  \param[in]      HdrType       Message header type.
 *  \param[in]      Param1        First parameter depending on header type.
 *  \param[in]      Param2        Second parameter depending on header type.
 *  \param[in]      Param3        Third parameter depending on header type.
 *  \param[in]      CloseSocket   Indicates if a socket shall be closed after transmission.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTx_PrepareTransmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TcpTx_FinishTransmit()
 *********************************************************************************************************************/
/*! \brief          Finish a pending transmission of a message which was requested by DoIP_TcpTx_PrepareTransmit().
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTx_FinishTransmit(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_Add()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP message to TCP Tx Queue
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in]      HdrType   Message header type.
 *  \param[in]      Param1    First parameter depending on header type.
 *  \param[in]      Param2    Second parameter depending on header type.
 *  \param[in]      Param3    Third parameter depending on header type.
 *  \param[in]      DataPtr   Pointer to data depending on header type.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_Add(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  SduDataPtrType DataPtr);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddGenNack()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Generic Negative Acknowledge message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx  Index in Tcp Tx Queue.
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in]      NackCode  Negative acknowledge code.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddGenNack(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint8 NackCode);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddRoutingActivationRes()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Routing Activation Response message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx    Index in Tcp Tx Queue.
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      TesterAddr  Tester logical Address.
 *  \param[in]      AckCode     Acknowledge code.
 *  \param[in]      TesterIdx   Tester Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddRoutingActivationRes(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 TesterAddr,
  uint8 AckCode,
  uint8 TesterIdx);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddAliveCheckReq()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Alive Check Request message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx  Index in Tcp Tx Queue.
 *  \param[in]      ConIdx    Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddAliveCheckReq(
  uint8 QueueIdx,
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagReq()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Reuqest message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx    Index in Tcp Tx Queue.
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      ChannelIdx  Channel index.
 *  \param[in]      Length      PDU length.
 *  \param[in]      DataPtr     Pointer to data.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagReq(
  uint8 QueueIdx,
  PduIdType ConIdx,
  DoIP_ChannelIdType ChannelIdx,
  PduLengthType Length,
  SduDataPtrType DataPtr);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagAck()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Positive Acknowledge message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx  Index in Tcp Tx Queue.
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in]      EcuAddr   ECU logical Address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagAck(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 EcuAddr);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagNack()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Negative Acknowledge message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx    Index in Tcp Tx Queue.
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      EcuAddr     ECU logical Address.
 *  \param[in]      TesterAddr  Tester logical Address.
 *  \param[in]      NackCode    Negative acknowledge code.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagNack(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 EcuAddr,
  uint16 TesterAddr,
  uint8 NackCode);

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddOem()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP OEM message to TCP Tx Queue.
 *  \details        -
 *  \param[in]      QueueIdx    Index in Tcp Tx Queue.
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      HdrTypeOem  OEM message header type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_VOEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddOem(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 HdrTypeOem);
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_Copy()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_Copy(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyHeader()
 *********************************************************************************************************************/
/*! \brief          Copies header stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in,out]  HdrCopied         Indicates if header is copied completely.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyHeader(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyData()
 *********************************************************************************************************************/
/*! \brief          Copies data stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyData(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReq()
 *********************************************************************************************************************/
/*! \brief          Copies diagnostic request stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReq(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReqTP()
 *********************************************************************************************************************/
/*! \brief          Copies TP diagnostic request stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 6 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReqTP(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReqIF()
 *********************************************************************************************************************/
/*! \brief          Copies IF diagnostic request stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 6 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReqIF(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagAck()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_NACK_APPEND_REQ
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagAck(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

#if ( (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyOemSpecific()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first TCP Tx Queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index in Tcp Tx Queue.
 *  \param[in]      ConIdx            Connection Index.
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_VOEM_PAYLOAD_TYPE_ENABLED & DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_CopyOemSpecific(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);
#endif /* (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_TransmitElement()
 *********************************************************************************************************************/
/*! \brief          Triggers transmission of TCP Tx Queue.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_TransmitElement(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_RemoveElement()
 *********************************************************************************************************************/
/*! \brief          Removes first TCP Tx Queue element (used to confirm transmission of element).
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \param[in]      Result  Result of transmission.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_RemoveElement(
  PduIdType ConIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_HandleTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles pending TcpTxQueue elements.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_HandleTransmit(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxGeneric_CopyGenHdr()
 *********************************************************************************************************************/
/*! \brief          Copies Generic Header to header buffer.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \return         E_OK            Header (already) copied completely.
 *  \return         E_NOT_OK        Header not copied completely.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxGeneric_CopyGenHdr(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxGeneric_CopyDataToHeaderBuffer()
 *********************************************************************************************************************/
/*! \brief          Copies data to header buffer.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to source buffer and length.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \param[in]      BytesToCopy     Number of bytes to copy.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  PduLengthType BytesToCopy);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleRoutingActivationReq()
 *********************************************************************************************************************/
/*! \brief          Processes Routing Activation Request messages.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length.
 *  \param[in,out]  BufferSizePtr   Size of pending message.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \param[out]     ErrorIdPtr      Pointer to DET error if one occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleRoutingActivationReq(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleCopiedRoutingActivationReq()
 *********************************************************************************************************************/
/*! \brief          Processes Routing Activation Request messages if copied to local buffer completely.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleCopiedRoutingActivationReq(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem()
 *********************************************************************************************************************/
/*! \brief          Processes OEM part of a Routing Activation Request message.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_CheckForRegisteredTester()
 *********************************************************************************************************************/
/*! \brief          Checks if a tester is already registered on the connection and if it is the same as in this routing
 *                  activation request.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      TesterAddr  Tester logical Address.
 *  \param[in]      TesterIdx   Tester Index.
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *  \return         E_OK        Tester is already registered for this connection or not registered at all and at least
 *                              one other TCP connection is left.
 *  \return         E_NOT_OK    Tester is registered to another connection or no other TCP connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_CheckForRegisteredTester(
  PduIdType ConIdx,
  uint16 TesterAddr,
  uint8 TesterIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleNoTesterRegistered()
 *********************************************************************************************************************/
/*! \brief          Processes routing activation request message if no tester is registered on this TCP connection.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[in]      TesterAddr  Tester logical Address.
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *  \return         E_OK        Tester is not registered at all and at least one other TCP connection is left.
 *  \return         E_NOT_OK    Tester is registered to another connection or no other TCP connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_HandleNoTesterRegistered(
  PduIdType ConIdx,
  uint16 TesterAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_TriggerAliveCheckSingleConnection()
 *********************************************************************************************************************/
/*! \brief          Performs alive check a single connection with registered tester.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 3 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_TriggerAliveCheckSingleConnection(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_TriggerAliveCheckAllConnections()
 *********************************************************************************************************************/
/*! \brief          Performs alive checks on all local address related connections with registered testers.
 *  \details        -
 *  \param[in]      ConIdx      Connection Index.
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 3 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_TriggerAliveCheckAllConnections(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetTesterIdxByTesterAddr()
 *********************************************************************************************************************/
/*! \brief          Gets the tester index of a tester address.
 *  \details        -
 *  \param[in]      TesterAddr  Tester logical address.
 *  \param[out]     TesterIdx   Tester index in configured tester array.
 *  \return         E_OK        Tester index found.
 *  \return         E_NOT_OK    No tester index found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_GetTesterIdxByTesterAddr(
  uint16 TesterAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TesterIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetConIdxOfTesterAddr()
 *********************************************************************************************************************/
/*! \brief          Identifies a connection by tester address.
 *  \details        -
 *  \param[in]      TesterAddr  Tester logical address.
 *  \param[out]     ConIdx      Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_GetConIdxOfTesterAddr(
  uint16 TesterAddr,
  P2VAR(PduIdType, AUTOMATIC, DOIP_APPL_VAR) ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetAvailableSockNum()
 *********************************************************************************************************************/
/*! \brief          Returns the number of available TCP_DATA sockets (n-1 TCP sockets) for a local address.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index
 *  \return         Number        Number of available TCP_DATA sockets (n-1 TCP sockets).
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(uint8, DOIP_CODE) DoIP_TcpRxRout_GetAvailableSockNum(
  uint8 LocalAddrIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivation()
 *********************************************************************************************************************/
/*! \brief          Performs authentication and confirmation via oem specific part and activates routing if successful.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivation(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivationAuth()
 *********************************************************************************************************************/
/*! \brief          Performs routing activation authentication.
 *  \details        -
 *  \param[out]     NextStatePtr  Pointer to next routing activation state.
 *  \return         E_OK          Authentication successfully performed.
 *  \return         E_NOT_OK      Authentication not successfully performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivationAuth(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivationConf()
 *********************************************************************************************************************/
/*! \brief          Performs routing activation confirmation.
 *  \details        -
 *  \param[in,out]  NextStatePtr  Indicates next state for main function retry handling.
 *  \return         E_OK          Confirmation successfully performed.
 *  \return         E_NOT_OK      Confirmation not successfully performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivationConf(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf()
 *********************************************************************************************************************/
/*! \brief          Handles authentication and confirmation of routing activations if state is pending.
 *  \details        Called in MainFunction after routing activation was processed in reception context
 *                  (DoIP_TcpRxRout_PerformRoutingActivation).
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleAliveCheckTimeouts()
 *********************************************************************************************************************/
/*! \brief          Handles alive check DoIP protocol timeouts.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 1 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleAliveCheckTimeouts(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose()
 *********************************************************************************************************************/
/*! \brief          Closes sockets if alive check timeout occurred and no alive check response has been received on it.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 1 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_CheckConnectionAvailablility()
 *********************************************************************************************************************/
/*! \brief          Checks if tester is not active on another connection and an additional connection is left.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \return         E_OK      Tester is not active and an additional connection is left.
 *  \return         E_NOT_OK  Tester is active or no additional connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_CheckConnectionAvailablility(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleInactivityTimeouts()
 *********************************************************************************************************************/
/*! \brief          Handles inactivity DoIP protocol timeouts.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleInactivityTimeouts(void);

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_HandleAliveCheckRes()
 *********************************************************************************************************************/
/*! \brief          Processes Alive Check Response messages.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to source buffer and length.
 *  \param[in,out]  BufferSizePtr   Pointer to remaining size of the alive check response.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_HandleAliveCheckRes(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_HandleCopiedAliveCheckRes()
 *********************************************************************************************************************/
/*! \brief          Processes copied Alive Check Response messages.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_HandleCopiedAliveCheckRes(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler()
 *********************************************************************************************************************/
/*! \brief          Removes an alive check entry in alive check list of routing activation handler.
 *  \details        -
 *  \param[in]      ConIdx  Connection Index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler(
  PduIdType ConIdx);

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleDiagnosticMessage()
 *********************************************************************************************************************/
/*! \brief          Processes Diagnostic messages.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length.
 *  \param[in,out]  BufferSizePtr   Pointer to remaining size of the diagnostic message.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_HandleDiagnosticMessage(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleValidDiagnosticMessage()
 *********************************************************************************************************************/
/*! \brief          Processes valid diagnostic message.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to source buffer and length.
 *  \param[in,out]  BufferSizePtr   Pointer to remaining size of the diagnostic message.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 5 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_HandleValidDiagnosticMessage(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_CopyLogicalAddr()
 *********************************************************************************************************************/
/*! \brief          Copies logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to source buffer and length.
 *  \param[in,out]  BufferSizePtr   Pointer to remaining size of the diagnostic message.
 *  \param[in,out]  ReadDataLenPtr  Pointer to data offset for read/write. Number of copied bytes will be added.
 *  \param[in,out]  Channel         Channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_CopyLogicalAddr(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel);

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleCopiedLogicalAddr()
 *********************************************************************************************************************/
/*! \brief          Handles copied logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in,out]  Channel   Channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_HandleCopiedLogicalAddr(
  PduIdType ConIdx,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel);

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleTargetLogicalAddress()
 *********************************************************************************************************************/
/*! \brief          Handles target logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConIdx    Connection Index.
 *  \param[in,out]  Channel   Channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 3 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_HandleTargetLogicalAddress(
  PduIdType ConIdx,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel);

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_AppendNackData()
 *********************************************************************************************************************/
/*! \brief          Append Diagnostic data to Ack/Nack buffer.
 *  \details        -
 *  \param[in]      ConIdx          Connection Index.
 *  \param[in]      PduInfoPtr      Pointer to source buffer and length.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_NACK_APPEND_REQ
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_AppendNackData(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VRxHandleOemPayloadTypes()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VRxHandleOemPayloadTypes(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgVerifyRxPdu()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgVerifyRxPdu(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel);

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyVerifiedUserData()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgCopyVerifiedUserData(
  PduIdType ConIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize);
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyUserData()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VRxDiagMsgCopyUserData(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) LocalPduInfoPtr);

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyDataToPduR()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgCopyDataToPduR(
  PduIdType ConIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) LocalPduInfoPtr);

/**********************************************************************************************************************
 *  DoIP_VGetChannelByTgtAddr()
 *********************************************************************************************************************/
/*! \brief      retrieves a channel by target address
 *  \param[in]  TgtAddr       logical target address
 *  \return     Channel       channel with logical address
 *  \context    task level
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_ChannelIdType, DOIP_CODE) DoIP_VGetChannelByTgtAddr(
  uint16 TgtAddr);

/**********************************************************************************************************************
 *  DoIP_VGetChannelByTgtAddrForRoutAct()
 *********************************************************************************************************************/
/*! \brief      retrieves a channel by target address for a specific routing activation considering data length in case
 *              of PDU size routing
 *  \param[in]  RoutActIdx    routing activation index
 *  \param[in]  TgtAddr       logical target address
 *  \param[in]  DataLen       data length
 *  \return     Channel       channel with logical address
 *  \context    task level
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_ChannelIdType, DOIP_CODE) DoIP_VGetChannelByTgtAddrForRoutAct(
  uint8 RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen);

#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VPduSizeRoutValidateChannel()
 *********************************************************************************************************************/
/*! \brief          validates if a channel is the best fitting channel for a target address
 *  \param[in]      Channel         channel
 *  \param[in]      DataLen         data length
 *  \param[in,out]  BestMaxPduSize  configured length of best matching channel up to now
 *  \return         E_OK            best matching channel (up to now)
 *  \return         E_NOT_OK        channel does not match
 *  \context        task level
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VPduSizeRoutValidateChannel(
  DoIP_ChannelIdType Channel,
  uint32 DataLen,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_VAR) BestMaxPduSize);
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VHandleVerifiedUserData()
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VHandleVerifiedUserData(void);
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DoIP_Util_FillGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGenHdr(
  uint16 HdrType,
  uint32 HdrLen,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* header version */
  TgtBufferPtr[0u] = DOIP_PROTOCOL_VERSION;
  /* inverse header version */
  TgtBufferPtr[1u] = (uint8)(~TgtBufferPtr[0u]);
  /* header type */
  TgtBufferPtr[2u] = (uint8)(HdrType >> 8u);
  TgtBufferPtr[3u] = (uint8)(HdrType);
  /* header length */
  TgtBufferPtr[4u] = (uint8)(HdrLen >> 24u);
  TgtBufferPtr[5u] = (uint8)(HdrLen >> 16u);
  TgtBufferPtr[6u] = (uint8)(HdrLen >>  8u);
  TgtBufferPtr[7u] = (uint8)(HdrLen);
} /* DoIP_Util_FillGenHdr() */

/**********************************************************************************************************************
 *  DoIP_Util_FillVin()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillVin(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 vinGidBuf[DOIP_VIN_LEN_BYTE] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VCallGetVinDirect(vinGidBuf) == E_OK )
  { /* VIN successfully retrieved */
    /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&TgtBufferPtr[0],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))vinGidBuf, DOIP_VIN_LEN_BYTE);

    retVal = E_OK;
  }
  else
  { /* VIN request failed */
    uint8 idx;

    for ( idx = 0u; idx < DOIP_VIN_LEN_BYTE; idx++ )
    {
      TgtBufferPtr[idx] = DOIP_VVIN_INV_PATTERN;
    }

    retVal = E_NOT_OK;
  }

  return retVal;
} /* DoIP_Util_FillVin() */

/**********************************************************************************************************************
 *  DoIP_Util_FillLocalLogAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillLocalLogAddr(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 logAddr;

  /* ----- Implementation ----------------------------------------------- */
  logAddr = DOIP_VADDR;
  TgtBufferPtr[0u] = (uint8)(logAddr >> 8u);
  TgtBufferPtr[1u] = (uint8)(logAddr);
} /* DoIP_Util_FillLocalLogAddr() */

/**********************************************************************************************************************
 *  DoIP_Util_FillEid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillEid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
#if ( DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON )
  uint8 physAddrBuf[DOIP_UHWID_LEN_BYTE];
#endif /* DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON )
  if ( SoAd_GetPhysAddr(DoIP_VCon2SoConId[ConIdx], &physAddrBuf[0u]) == E_OK )
  {
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&TgtBufferPtr[0],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, DOIP_CONST))&physAddrBuf[0u], DOIP_UHWID_LEN_BYTE);
  }
  else
  {
    retVal = E_NOT_OK;
  }
#else
  /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
  IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&TgtBufferPtr[0],
    (P2CONST(IpBase_CopyDataType, AUTOMATIC, DOIP_CONST))(&DoIP_VEid[0u]), DOIP_UHWID_LEN_BYTE);

  DOIP_DUMMY_STATEMENT_VAR(ConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON */

  return retVal;
} /* DoIP_Util_FillEid() */

/**********************************************************************************************************************
 *  DoIP_Util_FillGid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillGid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( (DOIP_VVIN_GID_MASTER == STD_ON ) && (DOIP_VUSE_EID_AS_GID == STD_ON) )
  retVal = DoIP_Util_FillEid(TgtBufferPtr, ConIdx);
#else
  DoIP_Util_FillGidOrInv(TgtBufferPtr);

  DOIP_DUMMY_STATEMENT_VAR(ConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */

  retVal = E_OK;
#endif /* (DOIP_VVIN_GID_MASTER == STD_ON ) && (DOIP_VUSE_EID_AS_GID == STD_ON) */

  return retVal;
} /* DoIP_Util_FillGid() */

/**********************************************************************************************************************
 *  DoIP_Util_FillGidOrInv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGidOrInv(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VGID_AVAILABLE == STD_ON )
  /* use DoIP GID */
  IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&TgtBufferPtr[0],
    (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))DoIP_VGid, DOIP_GID_LEN_BYTE);
#else
  uint8 vinGidBuf[DOIP_VIN_LEN_BYTE] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};

  /* use DoIP_VCallGetGidDirect or invalidity value */
  if ( DoIP_VCallGetGidDirect(vinGidBuf) == E_OK )
  { /* GID successfully retrieved */
    /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&TgtBufferPtr[0],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))vinGidBuf, DOIP_GID_LEN_BYTE);
  }
  else
  { /* GID request failed */
    uint8 idx;

    for ( idx = 0u; idx < DOIP_GID_LEN_BYTE; idx++ )
    {
      TgtBufferPtr[idx] = DOIP_VGID_INV_PATTERN;
    }
  }
#endif /* DOIP_VGID_ENABLED == STD_ON */
} /* DoIP_Util_FillGidOrInv() */

/**********************************************************************************************************************
 *  DoIP_Util_FillFurtherAction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillFurtherAction(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  TgtBufferPtr[0] = 0x00u;
#if ( DOIP_VTESTER_WITH_CEN_SEC_AVAILABLE == STD_ON )
  {
    PduIdType idx;

    for ( idx = 0u; idx < DOIP_VCONNECTION_TCP_TOTAL; idx++ )
    {
      if ( (DoIP_VTcpCon2ActivMgt[idx].ActivState != DOIP_ROUT_ACTIV_STATE_ACTIVATED) ||
        (DoIP_VRoutingActivations[DoIP_VTcpCon2ActivMgt[ idx].RoutActIdx].RoutingActivationNum != 0xE0u) )
      {
        if ( DoIP_VTester[DoIP_VTcpCon2ActivMgt[idx].TesterIdx].CentralSecurity == TRUE )
        { /* central security enabled */
          TgtBufferPtr[0] = 0x10u;
          break;
        }
      }
    }
  }
#endif /* DOIP_VTESTER_WITH_CEN_SEC_AVAILABLE == STD_ON */
} /* DoIP_Util_FillFurtherAction() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConInitAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConInitAll(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* initiate whole socket connection struct if not done before */
  if ( DoIP_VCon2SoConId[0u] == SOAD_INV_SO_CON_ID )
  {
    PduIdType connection;
    SoAd_SoConIdType soConId = 0u;

    /* get socket connection identifier from SoAd */
    for ( connection = 0u; connection < DOIP_VCONNECTION_TOTAL; connection++ )
    {
      retVal |= SoAd_GetSoConId(DoIP_VCon2SoAdTxPduId[connection], &soConId);

      if ( retVal == E_OK )
      {
        DoIP_VCon2SoConId[connection] = soConId;
      }
      else
      {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
        *ErrorIdPtr = DOIP_E_SOAD_CALL_FAILED;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
        break;
      }
    }
  }

#if ( DOIP_DEV_ERROR_DETECT == STD_OFF )
  DOIP_DUMMY_STATEMENT_VAR(ErrorIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_OFF */

  return retVal;
} /* DoIP_Con_SoConInitAll() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConOpen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConOpen(
  PduIdType ConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( DoIP_VCon2SoConState[ConIdx] == SOAD_SOCON_OFFLINE )
  { /* call SoAd to reopen socket connection */
    /* set state to reconnect to avoid multiple open requests */
    DoIP_VCon2SoConState[ConIdx] = SOAD_SOCON_RECONNECT;

    DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    (void)SoAd_OpenSoCon(DoIP_VCon2SoConId[ConIdx]);
  }
  else
  {
    DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* DoIP_Con_SoConOpen() */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_SoConCloseAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConCloseAll(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduIdType connection;

  /* ----- Implementation ----------------------------------------------- */
  for ( connection = 0u; connection < DOIP_VCONNECTION_TOTAL; connection++ )
  {
    if ( DoIP_VCon2SoConState[connection] != SOAD_SOCON_OFFLINE )
    { /* socket connection not closed yet */
      retVal = E_NOT_OK;

      if ( DoIP_VCon2SoConClosing[connection] != TRUE )
      { /* socket connection not requested to be closed */
        DoIP_VCon2SoConClosing[connection] = TRUE;
        DoIP_Con_SoConClose(connection);
      }
    }
  }

  return retVal;
} /* DoIP_Con_SoConCloseAll() */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_SoConClose()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConClose(
  PduIdType ConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  (void)SoAd_CloseSoCon(DoIP_VCon2SoConId[ConIdx], TRUE);
} /* DoIP_Con_SoConClose() */

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_ReqAllIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ReqAllIpAddrAssign(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 localAddrIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( localAddrIdx = 0u; localAddrIdx < DOIP_VLOCAL_ADDR_TOTAL; localAddrIdx++ )
  {
    if ( DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] == DOIP_IP_ADDR_REQ_STATE_NONE )
    {
      DoIP_Con_ReqIpAddrAssign(localAddrIdx);
    }
  }
} /* DoIP_Con_ReqAllIpAddrAssign() */
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ReqIpAddrAssign(
  uint8 LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduIdType connection;

  /* ----- Implementation ----------------------------------------------- */
  for ( connection = DOIP_VCONNECTION_TCP_TOTAL; connection < DOIP_VCONNECTION_TOTAL; connection++ )
  {
    if ( DoIP_VCon2LocalAddrIdx[connection] == LocalAddrIdx )
    { /* UdpConnection for current local address found */
      DoIP_VLocalAddrIdx2IpAddrReqState[LocalAddrIdx] = DOIP_IP_ADDR_REQ_STATE_REQUESTED;

# if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
#  if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u )
      retVal |= DoIP_Con_ReqIpAddrAssignStatic(LocalAddrIdx, connection);
#  endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u */
#  if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u )
      retVal |= DoIP_Con_ReqIpAddrAssignLinkLocalDoIP(LocalAddrIdx, connection);
#  endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u */
#  if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u )
      retVal |= DoIP_Con_ReqIpAddrAssignLinkLocal(LocalAddrIdx, connection);
#  endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u */
#  if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u )
      retVal |= DoIP_Con_ReqIpAddrAssignIpV6Router(LocalAddrIdx, connection);
#  endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u */
# endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
      /* overwrite hostname option */
      retVal |= DoIP_Con_SetDhcpHostname(LocalAddrIdx, connection);
#  if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
      retVal |= DoIP_Con_ReqIpAddrAssignDhcp(LocalAddrIdx, connection);
#  endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u */

      if ( retVal == E_NOT_OK )
      { /* request failed */
        /* no requirement specified */
        /* reset DOIP_IP_ADDR_REQ_STATE_REQUESTED to support assignment callback in this context */
        DoIP_VLocalAddrIdx2IpAddrReqState[LocalAddrIdx] &= DOIP_IP_ADDR_REQ_STATE_REQUESTED_INV;

        break;
      }
    }
  }
} /* DoIP_Con_ReqIpAddrAssign() */
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignStatic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignStatic(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u )
  { /* static IP address */
    SoAd_SockAddrInet6Type localIpAddr = { SOAD_AF_INET6, SOAD_PORT_ANY,
      {SOAD_IPADDR_ANY, SOAD_IPADDR_ANY, SOAD_IPADDR_ANY, SOAD_IPADDR_ANY} };
    SoAd_SockAddrInet6Type defaultRouter = { SOAD_AF_INET6, SOAD_PORT_ANY,
      {SOAD_IPADDR_ANY, SOAD_IPADDR_ANY, SOAD_IPADDR_ANY, SOAD_IPADDR_ANY} };

    localIpAddr.domain = DoIP_VLocalAddrIdx2DomainType[LocalAddrIdx];
    defaultRouter.domain = DoIP_VLocalAddrIdx2DomainType[LocalAddrIdx];

    /* request IP address assignment */
    retVal = SoAd_RequestIpAddrAssignment(DoIP_VCon2SoConId[ConIdx], SOAD_IPADDR_ASSIGNMENT_STATIC,
      DOIP_A_P2VAR(SoAd_SockAddrType)&localIpAddr, 0u, DOIP_A_P2VAR(SoAd_SockAddrType)&defaultRouter);
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssignStatic() */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_STATIC) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignLinkLocalDoIP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignLinkLocalDoIP(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u )
  { /* link local DoIP IP address */
    /* request IP address assignment */
    retVal = SoAd_RequestIpAddrAssignment(DoIP_VCon2SoConId[ConIdx], SOAD_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP,
      NULL_PTR, 0u, NULL_PTR);
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssignLinkLocalDoIP() */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignLinkLocal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignLinkLocal(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u )
  { /* link local IP address */
    /* request IP address assignment */
    retVal = SoAd_RequestIpAddrAssignment(DoIP_VCon2SoConId[ConIdx], SOAD_IPADDR_ASSIGNMENT_LINKLOCAL,
      NULL_PTR, 0u, NULL_PTR);
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssignLinkLocal() */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignIpV6Router()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignIpV6Router(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u )
  { /* Router Advertisement IP address */
    /* request IP address assignment */
    retVal = SoAd_RequestIpAddrAssignment(DoIP_VCon2SoConId[ConIdx], SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER,
      NULL_PTR, 0u, NULL_PTR);
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssignIpV6Router() */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_SetDhcpHostname()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SetDhcpHostname(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
  { /* DHCP IP address */
    /* overwrite hostname option */
# if ( DOIP_VDHCP_OPTION_VIN_USE_ENABLED == STD_ON )
    /* PRQA S 0686 2 */ /* MD_DoIP_0686 */
    uint8 hostName[DOIP_VMIN_HOST_NAME_SIZE + DOIP_VIN_PREFIX_LEN_BYTE + DOIP_VIN_LEN_BYTE] =
      { 'D', 'o', 'I', 'P', '-', 'V', 'I', 'N'};

    if ( DoIP_VCallGetVinDirect(&hostName[DOIP_VMIN_HOST_NAME_SIZE + DOIP_VIN_PREFIX_LEN_BYTE]) != E_OK )
    { /* VIN request failed */
      /* use invaliditiy pattern */
      uint8 idx = (uint8)(DOIP_VMIN_HOST_NAME_SIZE + DOIP_VIN_PREFIX_LEN_BYTE);

      for ( ; idx < (DOIP_VMIN_HOST_NAME_SIZE + DOIP_VIN_PREFIX_LEN_BYTE + DOIP_VIN_LEN_BYTE); idx++ )
      {
        hostName[idx] = DOIP_VVIN_INV_PATTERN;
      }
    }

    retVal = SoAd_WriteDhcpHostNameOption(DoIP_VCon2SoConId[ConIdx],
        (DOIP_VMIN_HOST_NAME_SIZE + DOIP_VIN_PREFIX_LEN_BYTE + DOIP_VIN_LEN_BYTE), hostName);
# else
    uint8 hostName[DOIP_VMIN_HOST_NAME_SIZE] = { 'D', 'o', 'I', 'P', '-' };

    retVal = SoAd_WriteDhcpHostNameOption(DoIP_VCon2SoConId[ConIdx],
        DOIP_VMIN_HOST_NAME_SIZE, hostName);
# endif /* DOIP_VDHCP_OPTION_VIN_USE_ENABLED == STD_ON */
  }

  return retVal;
} /* DoIP_Con_SetDhcpHostname() */
#endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssignDhcp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssignDhcp(
  uint8 LocalAddrIdx,
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (DoIP_VLocalAddrIdx2AssignmentType[LocalAddrIdx] & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u )
  { /* DHCP IP address */
    /* request IP address assignment */
    retVal =  SoAd_RequestIpAddrAssignment(DoIP_VCon2SoConId[ConIdx], SOAD_IPADDR_ASSIGNMENT_DHCP,
      NULL_PTR, 0u, NULL_PTR);
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssignDhcp() */
# endif /* (DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelAllIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelAllIpAddrAssign(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 localAddrIdx;
  uint8 localAddrUnassigned = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for ( localAddrIdx = 0u; localAddrIdx < DOIP_VLOCAL_ADDR_TOTAL; localAddrIdx++ )
  { /* iterate over all UdpConnections and release IP address assignment on one UdpConnection of each local address */
    if ( (DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] != DOIP_IP_ADDR_REQ_STATE_NONE) &&
          (DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] != DOIP_IP_ADDR_REQ_STATE_RELEASE) )
    { /* local address not yet released on this local address */
      DoIP_Con_RelIpAddrAssign(localAddrIdx);
    }
    else if ( DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] == DOIP_IP_ADDR_REQ_STATE_NONE )
    {
      localAddrUnassigned++;
    }
    else
    {
      /* nothing to do here */
    }
  }

  /* check if all local addresses are unassigned */
  if ( localAddrUnassigned == DOIP_VLOCAL_ADDR_TOTAL )
  { /* all local addresses unassigned */
# if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
    /* reset GID sync */
    DoIP_GidInSync = FALSE;
# endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

    /* switch to inactive */
    DoIP_ActivationLineLocalState = DOIP_ACTIVATION_LINE_LOCAL_INACTIVE;
  }
} /* DoIP_Con_RelAllIpAddrAssign() */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelIpAddrAssign(
  uint8 LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection;

  /* ----- Implementation ----------------------------------------------- */
  for ( connection = DOIP_VCONNECTION_TCP_TOTAL; connection < DOIP_VCONNECTION_TOTAL; connection++ )
  {
    if ( DoIP_VCon2LocalAddrIdx[connection] == LocalAddrIdx )
    { /* UdpConnection for current local address found */
      DoIP_VLocalAddrIdx2IpAddrReqState[LocalAddrIdx] = DOIP_IP_ADDR_REQ_STATE_RELEASE;
      /* release IP address assignment */
      if ( SoAd_ReleaseIpAddrAssignment(DoIP_VCon2SoConId[connection]) != E_OK )
      { /* release failed */
        /* no requirement specified */
        /* perform retry in next MainFunction cycle */
        DoIP_VLocalAddrIdx2IpAddrReqState[LocalAddrIdx] = DOIP_IP_ADDR_REQ_STATE_ASSIGNED;
      }
      break;
    }
  }
} /* DoIP_Con_RelIpAddrAssign() */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_GetConIdxBySoConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_GetConIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(PduIdType, AUTOMATIC, DOIP_APPL_VAR) ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  PduIdType pduId;

  /* ----- Implementation ----------------------------------------------- */
  for ( pduId = 0u; pduId < DOIP_VCONNECTION_TOTAL; pduId++ )
  {
    if ( (SoConId == DoIP_VCon2SoConId[pduId]) && (SoConId != SOAD_INV_SO_CON_ID) )
    {
      *ConIdx = pduId;
      retVal = E_OK;
      break;
    }
  }

  return retVal;
} /* DoIP_Con_GetConIdxBySoConId() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConStates(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection;

  /* ----- Implementation ----------------------------------------------- */
  for ( connection = 0u; connection < DOIP_VCONNECTION_TCP_TOTAL; connection++ )
  {
    if ( (DoIP_VCon2SoConState[connection] == SOAD_SOCON_OFFLINE) &&
         (DoIP_VCon2IpAddrState[connection] == SOAD_IPADDR_STATE_ASSIGNED) )
    { /* call SoAd to reopen socket connection */
      /* set state to reconnect to avoid multiple open requests */
      DoIP_VCon2SoConState[connection] = SOAD_SOCON_RECONNECT;

      (void)SoAd_OpenSoCon(DoIP_VCon2SoConId[connection]);
    }
  }

  for ( connection = DOIP_VCONNECTION_TCP_TOTAL; connection < DOIP_VCONNECTION_TOTAL; connection++ )
  {
    if ( connection >= DOIP_VCONNECTION_TCP_UDP_TOTAL )
    { /* UDP connection for vehicle announcements */
      /* do not reopen socket connection */
    }
    else
    { /* UDP connection for requests */
      DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if ( (DoIP_VCon2SoConState[connection] == SOAD_SOCON_OFFLINE) &&
           (DoIP_VCon2IpAddrState[connection] == SOAD_IPADDR_STATE_ASSIGNED) )
      { /* call SoAd to reopen socket connection */
        /* set state to reconnect to avoid multiple open requests */
        DoIP_VCon2SoConState[connection] = SOAD_SOCON_RECONNECT;

        DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        (void)SoAd_OpenSoCon(DoIP_VCon2SoConId[connection]);
      }
      else
      {
        DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }
} /* DoIP_Con_HandleSoConStates() */

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
/**********************************************************************************************************************
 *  DoIP_Con_HandleIpAddrReassignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleIpAddrReassignment(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 localAddrIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( localAddrIdx = 0u; localAddrIdx < DOIP_VLOCAL_ADDR_TOTAL; localAddrIdx++)
  {
    if ( DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] == DOIP_IP_ADDR_REQ_STATE_NONE )
    {
      DoIP_Con_ReqIpAddrAssign(localAddrIdx);
    }
  }
} /* DoIP_Con_HandleIpAddrReassignment() */
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

#if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_HandleGidSync()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleGidSync(void)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_GidInSync != TRUE )
  {
    uint8_least localAddrIdx;

    for ( localAddrIdx = 0u; localAddrIdx < DOIP_VLOCAL_ADDR_TOTAL; localAddrIdx++ )
    {
      if ( DoIP_VLocalAddrIdx2IpAddrReqState[localAddrIdx] == DOIP_IP_ADDR_REQ_STATE_ASSIGNED )
      {
        if ( DoIP_VCallTriggerGidSync() == E_OK )
        {
          DoIP_GidInSync = TRUE;
        }
        break;
      }
    }
  }
} /* DoIP_Con_HandleGidSync() */
#endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_ActLineToActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ActLineToActive(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_Con_SoConInitAll(&errorId) == E_OK )
  {
    /* switch to active */
    DoIP_ActivationLineLocalState = DOIP_ACTIVATION_LINE_LOCAL_ACTIVE;

#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
    /* read and write DHCP hostname option and request IP address assignment */
    DoIP_Con_ReqAllIpAddrAssign();
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_VACTIVATION_LINE_TO_ACTIVE, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_Con_ActLineToActive() */

#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_ActLineToInactive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_ActLineToInactive(void)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_Con_SoConCloseAll() == E_OK )
  {
    DoIP_Con_RelAllIpAddrAssign();
  }
} /* DoIP_Con_ActLineToInactive() */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_UdpTx_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_Transmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  retVal = DoIP_UdpTx_TransmitImmediately(ConIdx, HdrType, Param, CloseSocket);

  if ( retVal == E_NOT_OK )
  { /* message has not been sent immediately */
    retVal = DoIP_UdpTx_TransmitDelayed(ConIdx, HdrType, Param, CloseSocket);
  }

  return retVal;
} /* DoIP_UdpTx_Transmit() */

/**********************************************************************************************************************
 *  DoIP_UdpTx_TransmitImmediately()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_TransmitImmediately(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( HdrType != DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
  { /* no Vehicle Announcement */
    /* try to send message immediately */
    retVal = DoIP_UdpTx_SingleTransmit(ConIdx, HdrType, Param);
  }

  if ( retVal == E_OK )
  { /* message has been sent immediately */
    boolean otherEntryFound = FALSE;
    uint16 idx;

    /* check if response is pending for this socket connection */
    for ( idx = 0u; idx < DOIP_VUDP_TX_MGT_LIST_SIZE; idx++ )
    {
      if ( ConIdx == DoIP_VUdpTxMgtList[idx].UdpCon )
      { /* response pending */
        otherEntryFound = TRUE;
        break;
      }
    }

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
    /* check if OEM specific payload type buffer has to be released */
    if ( HdrType == DOIP_HDR_TYPE_OEM_SPECIFIC )
    {
      DoIP_VOemPayloadTypeUdpBufLen = 0u;
    }
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

    if ( otherEntryFound == FALSE )
    { /* close socket connection directly (abort flag set to FALSE) */
      if ( CloseSocket == TRUE )
      {
        DoIP_Con_SoConClose(ConIdx);
      }
    }
    else
    { /* close socket connection after list does not contain elements for this connection anymore */
      DoIP_VCon2SoConClosing[ConIdx] |= CloseSocket;
    }
  }

  return retVal;
} /* DoIP_UdpTx_TransmitImmediately() */

/**********************************************************************************************************************
 *  DoIP_UdpTx_TransmitDelayed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_TransmitDelayed(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint16 idx;

  /* ----- Implementation ----------------------------------------------- */
  /* try to find an empty list element */
  for ( idx = 0u; idx < DOIP_VUDP_TX_MGT_LIST_SIZE; idx++ )
  {
    if ( DoIP_VUdpTxMgtList[idx].UdpCon == DOIP_INV_PDU_ID )
    { /* element is not used */
      /* write element */
      DoIP_VUdpTxMgtList[idx].UdpCon = ConIdx;
      DoIP_VUdpTxMgtList[idx].HdrType = HdrType;
      DoIP_VUdpTxMgtList[idx].RetryAttempt = DOIP_UDP_MAX_RETRY_CNT;
      /* write message specific parameter */
      if ( HdrType == DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
      { /* Vehicle Announcement/Vehicle Identification Response */
        /* timer */
        DoIP_VUdpTxMgtList[idx].Param1 = DOIP_VANNOUNCE_WAIT;
      }
      /* number of announcements, nack code, power mode, OEM specific header type... */
      DoIP_VUdpTxMgtList[idx].Param2 = Param;

      DoIP_VCon2SoConClosing[ConIdx] |= CloseSocket;

      retVal = E_OK;
      break;
    }
  }

  return retVal;
} /* DoIP_UdpTx_TransmitDelayed() */

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch(HdrType)
  {
    case DOIP_HDR_TYPE_GEN_NACK:
    { /* generic header negative acknowledge */
      retVal = DoIP_UdpTx_SingleTransmitGenNack(ConIdx, HdrType, (uint8)Param);
      break;
    }
    case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
    { /* vehicle announcement/identification request */
      retVal = DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes(ConIdx);
      break;
    }
    case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
    { /* diagnostic power mode information acknowledge */
      retVal = DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes(ConIdx, (uint8)Param);
      break;
    }
#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
    case DOIP_HDR_TYPE_ENTITY_STATE_RES:
    { /* DoIP entity status response */
      retVal = DoIP_UdpTx_SingleTransmitEntiyStateRes(ConIdx);
      break;
    }
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
    case DOIP_HDR_TYPE_OEM_SPECIFIC:
    { /* OEM specific payload type */
      retVal = DoIP_UdpTx_SingleTransmitOemSpecific(ConIdx, Param);
      break;
    }
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */
    default:
    { /* function does not support this header type */
      break;
    }
  }

  return retVal;
} /* DoIP_UdpTx_SingleTransmit() */

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitGenNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3199 MSR_3199 */ /* MD_DoIP_3199 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitGenNack(
  PduIdType ConIdx,
  uint16 HdrType,
  uint8 NackCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  DoIP_HdrNackType msg;
  PduInfoType msgStruct;

  /* ----- Implementation ----------------------------------------------- */
  /* build Nack message */
  msg.GenHdr.Ver         = DOIP_PROTOCOL_VERSION;
  msg.GenHdr.InvVer      = (uint8)(~msg.GenHdr.Ver); /* PRQA S 0290 */ /* MD_DoIP_21.1_INV */
  msg.GenHdr.Type[0u]    = (uint8)(HdrType >> 8u);
  msg.GenHdr.Type[1u]    = (uint8)(HdrType);
  msg.GenHdr.LenByte[0u] = 0u;
  msg.GenHdr.LenByte[1u] = 0u;
  msg.GenHdr.LenByte[2u] = 0u;
  msg.GenHdr.LenByte[3u] = 1u;

  msg.NackCode = NackCode;

  /* build message struct for request */
  msgStruct.SduDataPtr = &msg.GenHdr.Ver;
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + 1u;

  /* send message */
  retVal = SoAd_IfTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &msgStruct);

  return retVal;
} /* DoIP_UdpTx_SingleTransmitGenNack() */
/* PRQA L:MSR_3199 */

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduInfoType msgStruct;
  uint16 ackIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint8 data[DOIP_GEN_HDR_LEN_BYTE + DOIP_ACK_VEHICLE_IDENT_LEN_BYTE];

  /* ----- Implementation ----------------------------------------------- */
  /* fill VIN */
#if ( (DOIP_VIN_GID_SYNC == STD_ON) && (DOIP_VGID_SYNC_ENABLED == STD_ON) )
  Std_ReturnType retValVin = DoIP_Util_FillVin(&data[ackIdx]);
#else
  (void)DoIP_Util_FillVin(&data[ackIdx]);
#endif /* (DOIP_VIN_GID_SYNC == STD_ON) && (DOIP_VGID_SYNC_ENABLED == STD_ON) */
  ackIdx += DOIP_VIN_LEN_BYTE;

  /* fill Addr */
  DoIP_Util_FillLocalLogAddr(&data[ackIdx]);
  ackIdx += 2;

  /* fill EID */
  retVal |= DoIP_Util_FillEid(&data[ackIdx], ConIdx);
  ackIdx += DOIP_EID_LEN_BYTE;

  /* fill GID */
  retVal |= DoIP_Util_FillGid(&data[ackIdx], ConIdx);
  ackIdx += DOIP_GID_LEN_BYTE;

  /* fill Central Security */
  DoIP_Util_FillFurtherAction(&data[ackIdx]);

#if ( DOIP_VIN_GID_SYNC == STD_ON )
  /* fill VinGidSync */
  ackIdx++;
# if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
  if ( (retValVin != E_OK) && (DoIP_GidInSync != TRUE) )
  { /* VIN/GID sync not succcessful */
    data[ackIdx] = 0x10u;
  }
  else
# endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */
  {
    data[ackIdx] = 0x00u;
  }
#endif /* DOIP_VIN_GID_SYNC == STD_ON */

#if ( DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON )
  if ( retVal == E_OK )
#endif /* DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON */
  {
    /* fill generic header */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_ACK_VEHICLE_IDENT_LEN_BYTE, &data[0u]);

    /* prepare msg struct for sending */
    msgStruct.SduDataPtr = &data[0u];
    msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + DOIP_ACK_VEHICLE_IDENT_LEN_BYTE;

    /* send message */
    retVal |= SoAd_IfTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &msgStruct);
  }

  return retVal;
} /* DoIP_UdpTx_SingleTransmitVehicleAnnounceAndIdentRes() */

/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3199 MSR_3199 */ /* MD_DoIP_3199 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes(
  PduIdType ConIdx,
  uint8 DiagPowerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  DoIP_PowerModeAckType msg;
  PduInfoType msgStruct;

  /* ----- Implementation ----------------------------------------------- */
  /* build power mode information message */
  msg.GenHdr.Ver         = DOIP_PROTOCOL_VERSION;
  msg.GenHdr.InvVer      = (uint8)(~msg.GenHdr.Ver); /* PRQA S 0290 */ /* MD_DoIP_21.1_INV */
  msg.GenHdr.Type[0u]    = (uint8)(DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES >> 8u);
  msg.GenHdr.Type[1u]    = (uint8)(DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES & 0xFFu);
  msg.GenHdr.LenByte[0u] = 0x00u;
  msg.GenHdr.LenByte[1u] = 0x00u;
  msg.GenHdr.LenByte[2u] = 0x00u;
  msg.GenHdr.LenByte[3u] = 0x01u;

  msg.DiagPowerMode = DiagPowerMode;

  /* build message struct for request */
  msgStruct.SduDataPtr = &msg.GenHdr.Ver;
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + 1u;

  /* send message */
  retVal = SoAd_IfTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &msgStruct);

  return retVal;
} /* DoIP_UdpTx_SingleTransmitDiagPowerModeInfoRes() */
/* PRQA L:MSR_3199 */

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitEntiyStateRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3199 MSR_3199 */ /* MD_DoIP_3199 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitEntiyStateRes(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  DoIP_EntityStateAckType msg;
  PduInfoType msgStruct;
  PduIdType idx;
# if ( DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
  uint32 maxDataSize = DOIP_MAX_REQUEST_BYTES;
# endif /* DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* build entity status response */
  msg.GenHdr.Ver         = DOIP_PROTOCOL_VERSION;
  msg.GenHdr.InvVer      = (uint8)(~msg.GenHdr.Ver); /* PRQA S 0290 */ /* MD_DoIP_21.1_INV */
  msg.GenHdr.Type[0u]    = (uint8)(DOIP_HDR_TYPE_ENTITY_STATE_RES >> 8u);
  msg.GenHdr.Type[1u]    = (uint8)(DOIP_HDR_TYPE_ENTITY_STATE_RES & 0xFFu);
  msg.GenHdr.LenByte[0u] = 0x00u;
  msg.GenHdr.LenByte[1u] = 0x00u;
  msg.GenHdr.LenByte[2u] = 0x00u;
  msg.GenHdr.LenByte[3u] = DOIP_ACK_ENTITY_STATE_LEN_BYTE;
  msg.NodeType           = DOIP_NODE_TYPE;
  msg.MaxTcpSockCnt      = DOIP_VCONNECTION_TCP_TOTAL - DOIP_VLOCAL_ADDR_TOTAL; /* remove n+1 socket of each addr */
  msg.TcpSockCnt         = 0u;

  /* count open TCP_DATA socket (of all addr) */
  for ( idx = 0u; idx < DOIP_VCONNECTION_TCP_TOTAL; idx ++ )
  {
    if ( SOAD_SOCON_ONLINE == DoIP_VCon2SoConState[idx] )
    {
      msg.TcpSockCnt++;
    }
  }

# if ( DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
  msg.MaxDataSize[0u] = (uint8)(maxDataSize >> 24u);
  msg.MaxDataSize[1u] = (uint8)(maxDataSize >> 16u);
  msg.MaxDataSize[2u] = (uint8)(maxDataSize >>  8u);
  msg.MaxDataSize[3u] = (uint8)(maxDataSize);
# endif /* DOIP_VENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

  /* build message struct for request */
  msgStruct.SduDataPtr = &msg.GenHdr.Ver;
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + DOIP_ACK_ENTITY_STATE_LEN_BYTE;

  /* send message */
  retVal = SoAd_IfTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &msgStruct);

  return retVal;
} /* DoIP_UdpTx_SingleTransmitEntiyStateRes() */
/* PRQA L:MSR_3199 */
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_UdpTx_SingleTransmitOemSpecific()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_UdpTx_SingleTransmitOemSpecific(
  PduIdType ConIdx,
  uint16 HdrTypeOem)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType msgStruct;

  /* ----- Implementation ----------------------------------------------- */
  /* add the generic header in front of the user data */
  DoIP_Util_FillGenHdr(HdrTypeOem, (uint32)(DoIP_VOemPayloadTypeUdpBufLen - DOIP_GEN_HDR_LEN_BYTE),
    &DoIP_VOemPayloadTypeUdpBuf[0u]);

  /* build message struct */
  msgStruct.SduDataPtr = &DoIP_VOemPayloadTypeUdpBuf[0u];
  msgStruct.SduLength  = DoIP_VOemPayloadTypeUdpBufLen;

  /* send message */
  retVal = SoAd_IfTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &msgStruct);

  return retVal;
} /* DoIP_UdpTx_SingleTransmitOemSpecific() */
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_UpdTxList_CloseSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_CloseSocket(
  uint16 UdpTxListIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection = DoIP_VUdpTxMgtList[UdpTxListIdx].UdpCon;

  /* ----- Implementation ----------------------------------------------- */
  /* check if socket connection shall be closed */
  if ( (connection != DOIP_INV_PDU_ID) && (DoIP_VCon2SoConClosing[connection] == TRUE) )
  { /* socket connection shall be closed */
    /* check if other entries for socket connection exists */
    PduIdType idx;
    boolean otherEntryFound = FALSE;

    for (idx = 0u; idx < DOIP_VUDP_TX_MGT_LIST_SIZE; idx++ )
    {
      if ( (UdpTxListIdx != idx) &&
        (DoIP_VUdpTxMgtList[UdpTxListIdx].UdpCon == DoIP_VUdpTxMgtList[idx].UdpCon) )
      { /* found other entry */
        /* do not close socket connection */
        otherEntryFound = TRUE;
        break;
      }
    }

    if ( otherEntryFound == FALSE )
    { /* no other entry found */
      /* close socket connection */
      DoIP_Con_SoConClose(connection);
    }
  }
} /* DoIP_UpdTxList_CloseSocket() */

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntries()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntries(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType idx;

  /* ----- Implementation ----------------------------------------------- */
  for ( idx = 0u; idx < DOIP_VUDP_TX_MGT_LIST_SIZE; idx++ )
  {
    PduIdType connection = DoIP_VUdpTxMgtList[idx].UdpCon;

    if ( connection != DOIP_INV_PDU_ID )
    { /* pending UDP message */
      boolean releaseListEntry = FALSE;

      if ( DoIP_VCon2SoConState[connection] == SOAD_SOCON_ONLINE )
      { /* socket connection is online */
        if ( DoIP_VUdpTxMgtList[idx].HdrType == DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
        { /* Vehicle Announcement/Vehicle Identification Response */
          DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes(idx, connection);
        }
        else
        { /* Entity State Status Response, Diagnostic Power Mode Response or Generic Negative Acknowledge */
          DoIP_UpdTxList_HandleEntriesOthers(idx, connection, &releaseListEntry);
        }
      }
      else
      { /* socket connection is not online anymore */
        releaseListEntry = TRUE;
      }
      if ( releaseListEntry == TRUE )
      {
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
        /* check if OEM specific payload type buffer has to be released */
        if ( DoIP_VUdpTxMgtList[idx].HdrType == DOIP_HDR_TYPE_OEM_SPECIFIC )
        {
          DoIP_VOemPayloadTypeUdpBufLen = 0u;
        }
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */
        DoIP_UpdTxList_CloseSocket(idx);
        /* release list entry */
        DoIP_VUdpTxMgtList[idx].UdpCon = DOIP_INV_PDU_ID;
      }
    }
  }
} /* DoIP_UpdTxList_HandleEntries() */

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes(
  PduIdType ListIdx,
  PduIdType ConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  DoIP_VUdpTxMgtList[ListIdx].Param1--;
  if ( DoIP_VUdpTxMgtList[ListIdx].Param1 == 0u )
  { /* try to send message */
    if ( DoIP_UdpTx_SingleTransmit(ConIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_UNUSED_PARAM) != E_OK )
    { /* transmission not successful */
      /* increment counter again to rety in next MainFunction */
      DoIP_VUdpTxMgtList[ListIdx].Param1++;
      /* decrement retry attempt counter */
      DoIP_VUdpTxMgtList[ListIdx].RetryAttempt--;
      if ( DoIP_VUdpTxMgtList[ListIdx].RetryAttempt == 0u )
      { /* too many retries */
        /* release list entry to avoid that ECU does not answer other requests anymore */
        DoIP_VUdpTxMgtList[ListIdx].UdpCon = DOIP_INV_PDU_ID;
      }
    }
    else
    { /* transmission successful */
      DoIP_VUdpTxMgtList[ListIdx].Param2--;
      if ( DoIP_VUdpTxMgtList[ListIdx].Param2 > 0u )
      { /* further Vehicle Announcement messages are pending */
        /* reload timer */
        DoIP_VUdpTxMgtList[ListIdx].Param1 = DOIP_VANNOUNCE_INTERVAL;
      }
      else
      { /* all pending messages for this entry are sent */
        if ( ConIdx < DOIP_VCONNECTION_TCP_UDP_TOTAL )
        {
          DoIP_UpdTxList_CloseSocket(ListIdx);
        }
        /* release list entry */
        DoIP_VUdpTxMgtList[ListIdx].UdpCon = DOIP_INV_PDU_ID;
      }
    }
  }
} /* DoIP_UpdTxList_HandleEntriesVehicleAnnounceAndIdentRes() */

/**********************************************************************************************************************
 *  DoIP_UpdTxList_HandleEntriesOthers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_UpdTxList_HandleEntriesOthers(
  PduIdType ListIdx,
  PduIdType ConIdx,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) ReleaseListEntry)
{
  /* ----- Implementation ----------------------------------------------- */
  /* try to send message */
  if ( DoIP_UdpTx_SingleTransmit(ConIdx, DoIP_VUdpTxMgtList[ListIdx].HdrType,
         DoIP_VUdpTxMgtList[ListIdx].Param2) != E_OK )
  { /* message not sent */
    /* decrement retry attempt counter */
    DoIP_VUdpTxMgtList[ListIdx].RetryAttempt--;
    if ( DoIP_VUdpTxMgtList[ListIdx].RetryAttempt == 0u )
    { /* too many retries */
      *ReleaseListEntry = TRUE;
    }
  }
  else
  { /* message sent */
    *ReleaseListEntry = TRUE;
  }
} /* DoIP_UpdTxList_HandleEntriesOthers() */

/**********************************************************************************************************************
 *  DoIP_TcpTx_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpTx_Transmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  SduDataPtrType DataPtr,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check socket connection state */
  if ( DoIP_VCon2SoConState[ConIdx] == SOAD_SOCON_ONLINE )
  { /* socket connection online */
    if ( DoIP_VCon2SoConClosing[ConIdx] != TRUE )
    { /* socket is not requested to be closed by a prevoius transmission request */
      /* add element to Tx Queue */
      retVal = DoIP_TcpTxQueue_Add(ConIdx, HdrType, Param1, Param2, Param3, DataPtr);

      if ( retVal == E_OK )
      {
        if ( CloseSocket == TRUE )
        { /* socket shall be closed after sending this message */
          DoIP_VCon2SoConClosing[ConIdx] = TRUE;
        }

        DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* start transmission of Tx Queue */
        DoIP_TcpTxQueue_TransmitElement(ConIdx);
      }
      else
      {
        DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
        /* no memory in TxQueue available */
        errorId = DOIP_E_NOBUFS;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
      }
    }
    else
    {
      DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_VTCP_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TcpTx_Transmit() */
/* PRQA L:MSR_STPAR */

/**********************************************************************************************************************
 *  DoIP_TcpTx_PrepareTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTx_PrepareTransmit(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  boolean CloseSocket)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VTcpCon2TpPreTransMgt[ConIdx].HdrType == DOIP_HDR_TYPE_INVALID )
  { /* no pending prepare transmit request */
    /* store transmission parameter */
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].HdrType     = HdrType;
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param1      = Param1;
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param2      = Param2;
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param3      = Param3;
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].CloseSocket = CloseSocket;
  }
} /* DoIP_TcpTx_PrepareTransmit() */
/* PRQA L:MSR_STPAR */

/**********************************************************************************************************************
 *  DoIP_TcpTx_FinishTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTx_FinishTransmit(
  PduIdType ConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VTcpCon2TpPreTransMgt[ConIdx].HdrType != DOIP_HDR_TYPE_INVALID )
  { /* PrepareTransmit function called before */
    /* call transmit function to send message */
    (void)DoIP_TcpTx_Transmit(ConIdx,
      DoIP_VTcpCon2TpPreTransMgt[ConIdx].HdrType,
      DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param1,
      DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param2,
      DoIP_VTcpCon2TpPreTransMgt[ConIdx].Param3,
      NULL_PTR,
      DoIP_VTcpCon2TpPreTransMgt[ConIdx].CloseSocket);
    DoIP_VTcpCon2TpPreTransMgt[ConIdx].HdrType = DOIP_HDR_TYPE_INVALID;
  }
} /* DoIP_TcpTx_FinishTransmit() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_Add()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_Add(
  PduIdType ConIdx,
  uint16 HdrType,
  uint16 Param1,
  uint16 Param2,
  uint8  Param3,
  SduDataPtrType DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* check if Tx Queue can handle this request */
  if ( DoIP_VTcpCon2TxMgtQueLvl[ConIdx] < DOIP_VTCP_TX_MGT_QUEUE_SIZE )
  {
    uint8 idx;

    /* find next free entry in Tx Queue */
    if ( (DoIP_VTcpCon2TxMgtQueIdx[ConIdx] + DoIP_VTcpCon2TxMgtQueLvl[ConIdx]) >=
      DOIP_VTCP_TX_MGT_QUEUE_SIZE )
    { /* wrap around necessary */
      idx = (uint8)(DoIP_VTcpCon2TxMgtQueLvl[ConIdx] -
        (DOIP_VTCP_TX_MGT_QUEUE_SIZE - DoIP_VTcpCon2TxMgtQueIdx[ConIdx]));
    }
    else
    { /* no wrap around necessary */
      idx = (uint8)(DoIP_VTcpCon2TxMgtQueIdx[ConIdx] + DoIP_VTcpCon2TxMgtQueLvl[ConIdx]);
    }

    switch(HdrType)
    {
      case DOIP_HDR_TYPE_GEN_NACK:
      {
        DoIP_TcpTxQueue_AddGenNack(idx, ConIdx, (uint8)Param1);
        break;
      }
      case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
      {
        DoIP_TcpTxQueue_AddRoutingActivationRes(idx, ConIdx, Param1, (uint8)Param2, Param3);
        break;
      }
      case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
      {
        DoIP_TcpTxQueue_AddAliveCheckReq(idx, ConIdx);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_REQ:
      {
        DoIP_TcpTxQueue_AddDiagReq(idx, ConIdx, (DoIP_ChannelIdType)Param1, (PduLengthType)Param2, DataPtr);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_ACK:
      {
        DoIP_TcpTxQueue_AddDiagAck(idx, ConIdx, Param1);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_NACK:
      {
        DoIP_TcpTxQueue_AddDiagNack(idx, ConIdx, Param1, Param2, Param3);
        break;
      }
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      case DOIP_HDR_TYPE_OEM_SPECIFIC:
      {
        DoIP_TcpTxQueue_AddOem(idx, ConIdx, Param1);
        break;
      }
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */
      default:
      {
        break;
      }
    }

    /* update Tx Queue */
    DoIP_VTcpCon2TxMgtQueLvl[ConIdx]++;

    retVal = E_OK;
  }

  return retVal;
} /* DoIP_TcpTxQueue_Add() */
/* PRQA L:MSR_STPAR */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddGenNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddGenNack(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint8 NackCode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* generic header */
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_HDR_NACK_CODE_LEN_BYTE,
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);

  /* NACK Code */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[DOIP_GEN_HDR_LEN_BYTE] = NackCode;

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen = DOIP_GEN_HDR_LEN_BYTE + DOIP_GEN_HDR_NACK_CODE_LEN_BYTE;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength = DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddGenNack() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddRoutingActivationRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddRoutingActivationRes(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 TesterAddr,
  uint8 AckCode,
  uint8 TesterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint16 entityAddr = DOIP_VADDR;

  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* routing activation response */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = (uint8)(TesterAddr >> 8u);
  bufIdx++;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = (uint8)(TesterAddr);
  bufIdx++;

  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = (uint8)(entityAddr >> 8u);
  bufIdx++;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = (uint8)(entityAddr);
  bufIdx++;

  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = AckCode;
  bufIdx++;

  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = 0u;
  bufIdx++;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = 0u;
  bufIdx++;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = 0u;
  bufIdx++;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = 0u;
  bufIdx++;

  /* identify tester */
  if ( TesterIdx == DOIP_INV_TESTER_IDX )
  { /* unknown tester */
    /* generic header */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE,
      &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
  }
  else
  {
    /* check if OEM specific data shall be added */
    if ( DoIP_VTcpCon2ActivMgt[ConIdx].RoutActIdx != DOIP_INV_ROUT_ACT_IDX )
    { /* routing activation determined */
      if ( ((DoIP_VRoutingActivations[DoIP_VTcpCon2ActivMgt[ConIdx].RoutActIdx].AuthResLength == 0u) &&
          (DoIP_VRoutingActivations[DoIP_VTcpCon2ActivMgt[ConIdx].RoutActIdx].ConfResLength == 0u)) ||
        (DoIP_VTcpCon2ActivMgt[ConIdx].ActivState != DOIP_ROUT_ACTIV_STATE_ACTIVATED) )
      { /* no OEM specific data */
        DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE,
          &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
      }
      else
      { /* add OEM specific data */
        P2VAR(uint8, AUTOMATIC, AUTOMATIC) oemSpecPtr = DoIP_VRoutActivHandler.OemSpecRes;

        /* generic header */
        DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MAX_LEN_BYTE,
          &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);

        DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = oemSpecPtr[0u];
        bufIdx++;
        DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = oemSpecPtr[1u];
        bufIdx++;
        DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = oemSpecPtr[2u];
        bufIdx++;
        DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = oemSpecPtr[3u];
        bufIdx++;
      }
    }
    else
    { /* not routing activation determined */
      /* no OEM specific data */
      DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE,
        &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
    }
  }

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = bufIdx;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = bufIdx;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddRoutingActivationRes() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddAliveCheckReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddAliveCheckReq(
  uint8 QueueIdx,
  PduIdType ConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* generic header */
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u,
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = DOIP_GEN_HDR_LEN_BYTE;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = DOIP_GEN_HDR_LEN_BYTE;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddAliveCheckReq() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagReq(
  uint8 QueueIdx,
  PduIdType ConIdx,
  DoIP_ChannelIdType ChannelIdx,
  PduLengthType Length,
  SduDataPtrType DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VChannel2ApiType[ChannelIdx] != DOIP_API_TYPE_TP )
  {
    /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))DoIP_VChannel2IfTxBuf[ChannelIdx].BufPtr,
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))DataPtr, Length);
  }

  /* fill DoIP header in Tx buffer */
  /* generic header */
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_REQ, (DOIP_DIAG_HDR_LEN_BYTE + Length),
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);

  /* diagnostic message */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] = (uint8)(DoIP_VChannel2EcuAddr[ChannelIdx] >> 8u);
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] = (uint8)(DoIP_VChannel2EcuAddr[ChannelIdx]);
  bufIdx += DOIP_ADDR_LEN_BYTE;
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
  if ( DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr == DOIP_ADDR_ANY )
  { /* default tester */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] =
      (uint8)(DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr >> 8u);
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] =
      (uint8)DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr;
  }
  else
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */
  { /* no default tester */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] =
      (uint8)(DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr >> 8u);
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] =
      (uint8)DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr;
  }
  bufIdx += DOIP_ADDR_LEN_BYTE;

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Channel        = ChannelIdx;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = bufIdx;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = bufIdx + Length;
  if ( DoIP_VChannel2ApiType[ChannelIdx] == DOIP_API_TYPE_TP )
  { /* standard diagnostic message */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
      &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
  }
  else
  { /* UUDT */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr = DataPtr;
  }
} /* DoIP_TcpTxQueue_AddDiagReq() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagAck(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 EcuAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint8 testerIdx = DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* generic header */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_ACK, DOIP_DIAG_MSG_ACK_LEN_BYTE +
    DoIP_VTester2DiagNAckOffset[testerIdx],
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
#else
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_ACK, DOIP_DIAG_MSG_ACK_LEN_BYTE,
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

  /* diagnostic message positive ack */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] = (uint8)(EcuAddr >> 8u);
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] = (uint8)(EcuAddr);
  bufIdx += DOIP_ADDR_LEN_BYTE;
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
  if ( DoIP_VTester[testerIdx].TesterAddr == DOIP_ADDR_ANY )
  { /* default tester */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] =
      (uint8)(DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr >> 8u);
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] =
      (uint8)DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr;
  }
  else
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */
  { /* no default tester */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] =
      (uint8)(DoIP_VTester[testerIdx].TesterAddr >> 8u);
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] =
      (uint8)DoIP_VTester[testerIdx].TesterAddr;
  }
  bufIdx += DOIP_ADDR_LEN_BYTE;
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = DOIP_DIAG_MSG_RES_CODE_ACK;
  bufIdx++;

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = bufIdx;
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = (PduLengthType)(bufIdx +
    DoIP_VTester2DiagNAckOffset[testerIdx]);
#else
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = bufIdx;
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddDiagAck() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddDiagNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddDiagNack(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 EcuAddr,
  uint16 TesterAddr,
  uint8 NackCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint8 testerIdx = DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* generic header */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  if ( testerIdx != DOIP_INV_TESTER_IDX )
  { /* tester known */
    /* send diagnostic message negative ack with configured amount of user data */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_NACK, DOIP_DIAG_MSG_ACK_LEN_BYTE +
      DoIP_VTester2DiagNAckOffset[testerIdx], &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
  }
  else
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  { /* unknown tester */
    /* send diagnostic message negative ack without user data */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_NACK, DOIP_DIAG_MSG_ACK_LEN_BYTE,
      &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
  }

  /* diagnostic message negative ack */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] = (uint8)(EcuAddr >> 8u);
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] = (uint8)(EcuAddr);
  bufIdx += DOIP_ADDR_LEN_BYTE;

  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+0u] = (uint8)(TesterAddr >> 8u);
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx+1u] = (uint8)(TesterAddr);
  bufIdx += DOIP_ADDR_LEN_BYTE;

  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[bufIdx] = NackCode;
  bufIdx += 1u;

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = bufIdx;
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  if ( testerIdx != DOIP_INV_TESTER_IDX )
  { /* send diagnostic message negative ack with configured ammount of user data */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = (PduLengthType)(bufIdx +
      DoIP_VTester2DiagNAckOffset[testerIdx]);
  }
  else
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  { /* send diagnostic message negative ack without user data */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = bufIdx;
  }
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddDiagNack() */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_AddOem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_AddOem(
  uint8 QueueIdx,
  PduIdType ConIdx,
  uint16 HdrTypeOem)
{
  /* ----- Implementation ----------------------------------------------- */
  /* fill DoIP header in Tx buffer */
  /* generic header */
# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
  DoIP_Util_FillGenHdr(HdrTypeOem, DoIP_VOemPayloadTypeTcpBufLen,
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
# else
  DoIP_Util_FillGenHdr(HdrTypeOem, 0u,
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u]);
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0 */

  /* set txMgt struct */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen    = DOIP_GEN_HDR_LEN_BYTE;
# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = (PduLengthType)(DOIP_GEN_HDR_LEN_BYTE +
    DoIP_VOemPayloadTypeTcpBufLen);
# else
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength  = DOIP_GEN_HDR_LEN_BYTE;
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u */
  DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduDataPtr =
    &DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[0u];
} /* DoIP_TcpTxQueue_AddOem() */
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_Copy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_Copy(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint16  txMgtQueIdx;
  boolean hdrCopied = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check whether TP session is online */
  if ( DoIP_VTcpCon2TxTpSessionActive[ConIdx] != TRUE )
  { /* TP session is not online */
    /* nothing to do here */
  }
  else
#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
  /* Check TP cancellation */
  if ( DoIP_VTcpCon2TxTpCancelRequested[ConIdx] == TRUE )
  { /* TP cancellation is ongoing */
    /* nothing to do here */
  }
  else
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */
  /* check length and whether this is a first call of CopyTxData */
  if ( (PduInfoPtr->SduLength != 0u) && (DoIP_VTcpCon2LastTxBufSize[ConIdx] < PduInfoPtr->SduLength) )
  { /* unexpected length */
    /* nothing to do here */
  }
  else
  {
    if ( DoIP_VTcpCon2ActivMgt[ConIdx].ActivState == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    { /* routing activation active */
      /* reset inactivity timer */
      DoIP_VTcpCon2InactivCnt[ConIdx] = 0u;
    }

    txMgtQueIdx = DoIP_VTcpCon2TxMgtQueIdx[ConIdx];

    if ( DoIP_VTcpCon2TxBytesCopied[ConIdx] < DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].TxHdrBufLen )
    { /* header is not yet copied */
      retVal = DoIP_TcpTxQueue_CopyHeader(txMgtQueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, &hdrCopied);
    }
    else
    {
      retVal = BUFREQ_OK;
    }

    if ( (retVal == BUFREQ_OK) &&
      (DoIP_VTcpCon2TxBytesCopied[ConIdx] >= DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].TxHdrBufLen) &&
      (DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].TxHdrBufLen <
        DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].Msg.SduLength) )
    { /* header copied completely and more data are available */
      retVal = DoIP_TcpTxQueue_CopyData(txMgtQueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, hdrCopied);
    }
  }

  DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
} /* DoIP_TcpTxQueue_Copy() */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyHeader(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( PduInfoPtr->SduLength != 0u )
  { /* subsequent call - copy request */
    /* check requested length */
    if ( PduInfoPtr->SduLength <= ((PduLengthType)(DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen -
        DoIP_VTcpCon2TxBytesCopied[ConIdx])) )
    { /* length is in valid range - copy requested number of bytes to target buffer */
      /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
      IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))PduInfoPtr->SduDataPtr,
        (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTcpCon2TcpTxMgt[ConIdx][
          QueueIdx].TxHdrBuf[DoIP_VTcpCon2TxBytesCopied[ConIdx]],
        PduInfoPtr->SduLength);
      DoIP_VTcpCon2TxBytesCopied[ConIdx] += PduInfoPtr->SduLength;
      if ( DoIP_VTcpCon2TxBytesCopied[ConIdx] == DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen )
      { /* header copied completely */
        uint16 hdrType;
        /* extract message type */
        DOIP_GET_UINT16(&DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[DOIP_GEN_HDR_TYPE_IDX], 0u, hdrType);
        if ( (hdrType == DOIP_HDR_TYPE_ROUT_ACTIV_RES) &&
             (DoIP_VRoutActivHandler.TcpCon != DOIP_INV_PDU_ID) &&
             (DoIP_VTcpCon2ActivMgt[DoIP_VRoutActivHandler.TcpCon].ActivState != DOIP_ROUT_ACTIV_STATE_CONF_PENDING) )
        { /* Routing Activation Response */
          /* In case of confirmation a Routing Activation Response might be sent even if Routing Activation
           * is not finished. Release Routing Activation Handler in all other cases. */
          DoIP_VRoutActivHandler.TcpCon = DOIP_INV_PDU_ID;
        }

        *HdrCopied = TRUE;
      }
    }
    else
    { /* invalid length */
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  if ( retVal == BUFREQ_OK )
  {
    /* set available data length */
    *AvailableDataPtr = (PduLengthType)(DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen -
      DoIP_VTcpCon2TxBytesCopied[ConIdx]);
    DoIP_VTcpCon2LastTxBufSize[ConIdx] = *AvailableDataPtr;
  }

  return retVal;
} /* DoIP_TcpTxQueue_CopyHeader() */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyData(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;
  uint16 hdrType;

  /* ----- Implementation ----------------------------------------------- */
  /* extract message type */
  DOIP_GET_UINT16(&DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBuf[DOIP_GEN_HDR_TYPE_IDX], 0u, hdrType);

  if ( hdrType == DOIP_HDR_TYPE_DIAG_REQ )
  { /* diagnostic message */
    retVal = DoIP_TcpTxQueue_CopyDiagReq(QueueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);
  }
  else if ( (hdrType == DOIP_HDR_TYPE_DIAG_ACK) || (hdrType == DOIP_HDR_TYPE_DIAG_NACK) )
  { /* diagnostic message positive/negative acknowledge */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    retVal = DoIP_TcpTxQueue_CopyDiagAck(QueueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);
#else
    retVal = BUFREQ_OK;
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  }
  else
  { /* OEM specific payload type */
#if ( (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) )
    DoIP_TcpTxQueue_CopyOemSpecific(QueueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);
#endif /* (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) */
    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIP_TcpTxQueue_CopyData() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReq(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VChannel2ApiType[DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Channel] == DOIP_API_TYPE_TP )
  { /* TP-API */
    retVal = DoIP_TcpTxQueue_CopyDiagReqTP(QueueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);
  }
  else
  { /* IF-API (UUDT) */
    DoIP_TcpTxQueue_CopyDiagReqIF(QueueIdx, ConIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);
    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIP_TcpTxQueue_CopyDiagReq() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReqTP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReqTP(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) locPduInfoPtr = PduInfoPtr;
  PduInfoType locPduInfoStruct;

  /* ----- Implementation ----------------------------------------------- */
  /* query available upper layer buffer if header was copied successful */
  if ( HdrCopied == TRUE )
  {
    /* query Tx buffer size of upper layer */
    locPduInfoStruct.SduLength = 0u;
    locPduInfoPtr = &locPduInfoStruct;
  }

  /* call upper layer CopyTxData function to query available buffer or copying */
  retVal = PduR_DoIPTpCopyTxData(DoIP_VChannel2PduRTxConfPduId[
    DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Channel], locPduInfoPtr, NULL_PTR, AvailableDataPtr);

  /* check if Tx Queue has been modifed (e.g. connection closed) */
  if ( DoIP_VTcpCon2TxMgtQueLvl[ConIdx] == 0u )
  {
    retVal = BUFREQ_E_NOT_OK;
  }

  if ( retVal == BUFREQ_OK )
  { /* buffer request was sucessful */
    DoIP_VTcpCon2LastTxBufSize[ConIdx] = *AvailableDataPtr;
    if ( (locPduInfoPtr->SduLength == 0u) && (PduInfoPtr->SduLength != 0u) )
    { /* available buffer of upper layer was requested by DoIP - do not forward return value of upper layer */
      /* nothing to do here */
    }
    else
    {
      /* copying successful - update TxMgt struct */
      DoIP_VTcpCon2TxBytesCopied[ConIdx] += locPduInfoPtr->SduLength;
    }
  }
  else
  { /* buffer request failed */
    /* close connection since generic header has already been sent */
    DoIP_VCon2SoConClosing[ConIdx] = TRUE;

    DoIP_Con_SoConClose(ConIdx);

    retVal = BUFREQ_E_NOT_OK;
  }

  return retVal;
} /* DoIP_TcpTxQueue_CopyDiagReqTP() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagReqIF()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagReqIF(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) locPduInfoPtr = PduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  if ( (locPduInfoPtr->SduLength > 0u) && (HdrCopied == FALSE) )
  {
    PduLengthType bufIdx = (PduLengthType)(DoIP_VTcpCon2TxBytesCopied[ConIdx] -
      DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen);

    /* copy UUDT data from internal buffer to target buffer */
    /* PRQA S 0310, 3305 3 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))PduInfoPtr->SduDataPtr,
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VChannel2IfTxBuf[
        DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Channel].BufPtr[bufIdx], (uint32)PduInfoPtr->SduLength);

    /* update TxMgt struct */
    DoIP_VTcpCon2TxBytesCopied[ConIdx] += PduInfoPtr->SduLength;
  }

  /* set available data length */
  *AvailableDataPtr = (PduLengthType)(DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength -
    DoIP_VTcpCon2TxBytesCopied[ConIdx]);
  DoIP_VTcpCon2LastTxBufSize[ConIdx] = *AvailableDataPtr;
} /* DoIP_TcpTxQueue_CopyDiagReqIF() */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyDiagAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TcpTxQueue_CopyDiagAck(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (PduInfoPtr->SduLength != 0u) && (HdrCopied == FALSE) )
  { /* copy data to target buffer */
    /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))PduInfoPtr->SduDataPtr,
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTester[
        DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].DiagAckNackBufPtr[
          DoIP_VTcpCon2TxBytesCopied[ConIdx] - DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen],
      PduInfoPtr->SduLength);
    DoIP_VTcpCon2TxBytesCopied[ConIdx] += PduInfoPtr->SduLength;

    if ( DoIP_VTcpCon2TxBytesCopied[ConIdx] == DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength )
    { /* all data are copied to lower layer buffer */
      /* reset DiagNAck buffer offset to indicate that new data can be received (do not wait for TxConfirmation) */
      DoIP_VTester2DiagNAckOffset[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx] = 0u;
    }
  }

  /* set available data length */
  *AvailableDataPtr = (PduLengthType)(DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength -
    DoIP_VTcpCon2TxBytesCopied[ConIdx]);
  DoIP_VTcpCon2LastTxBufSize[ConIdx] = *AvailableDataPtr;

  return retVal;
} /* DoIP_TcpTxQueue_CopyDiagAck() */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

#if ( (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) )
/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_CopyOemSpecific()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_CopyOemSpecific(
  uint16 QueueIdx,
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( (PduInfoPtr->SduLength != 0u) && (HdrCopied == FALSE) )
  { /* copy data to target buffer */
    /* PRQA S 0310, 3305 2 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))PduInfoPtr->SduDataPtr,
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VOemPayloadTypeTcpBuf[
        DoIP_VTcpCon2TxBytesCopied[ConIdx] - DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].TxHdrBufLen],
      PduInfoPtr->SduLength);
    DoIP_VTcpCon2TxBytesCopied[ConIdx] += PduInfoPtr->SduLength;

    if ( DoIP_VTcpCon2TxBytesCopied[ConIdx] == DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength )
    { /* all data are copied to lower layer buffer */
      DoIP_VOemPayloadTypeTcpBufLen  = 0u;
      DoIP_VOemPayloadTypeTcpBufIsTx = FALSE;
      DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
    }
  }

  /* set available data length */
  *AvailableDataPtr = (PduLengthType)(DoIP_VTcpCon2TcpTxMgt[ConIdx][QueueIdx].Msg.SduLength -
    DoIP_VTcpCon2TxBytesCopied[ConIdx]);
  DoIP_VTcpCon2LastTxBufSize[ConIdx] = *AvailableDataPtr;
} /* DoIP_TcpTxQueue_CopyOemSpecific() */
#endif /* (DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON) && (DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u) */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_TransmitElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_TransmitElement(
  PduIdType ConIdx)
{
  DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( DoIP_VTcpCon2TxTpSessionActive[ConIdx] == FALSE )
  { /* no transmission is requested for this socket connection */
    /* lock since this function can be called reentrant */
    DoIP_VTcpCon2TxTpSessionActive[ConIdx] = TRUE;

    if ( DoIP_VTcpCon2TxMgtQueLvl[ConIdx] > 0u )
    { /* pending msg */
      PduInfoType txMgtMsg;
      uint16 txMgtQueIdx;

      txMgtQueIdx = DoIP_VTcpCon2TxMgtQueIdx[ConIdx];
      txMgtMsg.SduDataPtr = DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].Msg.SduDataPtr;
      txMgtMsg.SduLength  = DoIP_VTcpCon2TcpTxMgt[ConIdx][txMgtQueIdx].Msg.SduLength;

      DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* request transmission at SoAd */
      if ( SoAd_TpTransmit(DoIP_VCon2SoAdTxPduId[ConIdx], &txMgtMsg) == E_OK )
      { /* transmission request succeeded */
        /* nothing to do here */
      }
      else
      { /* transmission request failed */
        DoIP_VTcpCon2TxTpSessionActive[ConIdx] = FALSE;
      }
    }
    else
    { /* queue is empty */
      DoIP_VTcpCon2TxTpSessionActive[ConIdx] = FALSE;

      DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  { /* transmission is already active */
    DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* DoIP_TcpTxQueue_TransmitElement() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_RemoveElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_RemoveElement(
  PduIdType ConIdx,
  Std_ReturnType Result)
{
  DOIP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( DoIP_VTcpCon2TxMgtQueLvl[ConIdx] > 0u )
  {
    DoIP_ChannelIdType channel = DoIP_VTcpCon2TcpTxMgt[ConIdx][DoIP_VTcpCon2TxMgtQueIdx[ConIdx]].Channel;
    uint8 queIdx = DoIP_VTcpCon2TxMgtQueIdx[ConIdx];

    /* reset TxMgt structs */
    DoIP_VTcpCon2TcpTxMgt[ConIdx][queIdx].TxHdrBufLen = 0u;

    /* Update Tx Queue */
    DoIP_VTcpCon2TxMgtQueLvl[ConIdx]--;
    DoIP_VTcpCon2TxMgtQueIdx[ConIdx]++;
    if ( DoIP_VTcpCon2TxMgtQueIdx[ConIdx] == DOIP_VTCP_TX_MGT_QUEUE_SIZE )
    { /* wrap around */
      DoIP_VTcpCon2TxMgtQueIdx[ConIdx] = 0u;
    }

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
    /* release OEM specific payload type buffer */
    if ( DoIP_VOemPayloadTypeTcpBuf2Con == ConIdx )
    {
      DoIP_VOemPayloadTypeTcpBufLen  = 0u;
      DoIP_VOemPayloadTypeTcpBufIsTx = FALSE;
      DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
    }
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u */
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

    /* finish TP session */
    DoIP_VTcpCon2TxTpSessionActive[ConIdx] = FALSE;

    DoIP_VTcpCon2TxBytesCopied[ConIdx] = 0u;

    if ( channel != DOIP_INV_CHANNEL )
    {
      DoIP_VChannel2TxMsgLen[channel] = 0u;
      /* PRQA S 0277 1 */ /* MD_DoIP_3.1 */
      DoIP_VTcpCon2TcpTxMgt[ConIdx][queIdx].Channel = DOIP_INV_CHANNEL;

      DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* forward TxConfirmation to upper layer */
#if ( DOIP_VPDUR_IF_TXCONF_ENABLED == STD_ON )
      if ( DoIP_VChannel2ApiType[channel] == DOIP_API_TYPE_IF )
      { /* IF-API */
        if ( DoIP_VChannel2PduRTxConfPduId[channel] != DOIP_INV_PDU_ID )
        { /* TxConfirmation enabled */
          PduR_DoIPIfTxConfirmation(DoIP_VChannel2PduRTxConfPduId[channel]);
        }
      }
      else
#endif /* DOIP_VPDUR_IF_TXCONF_ENABLED == STD_ON */
      { /* TP-API */
        PduR_DoIPTpTxConfirmation(DoIP_VChannel2PduRTxConfPduId[channel], Result);
      }
    }
    else
    {
      DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    DOIP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* DoIP_TcpTxQueue_RemoveElement() */

/**********************************************************************************************************************
 *  DoIP_TcpTxQueue_HandleTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpTxQueue_HandleTransmit(void)
{
  PduIdType connection;

  for ( connection = 0u; connection < DOIP_VCONNECTION_TCP_TOTAL; connection++ )
  {
    if ( DoIP_VCon2SoConState[connection] == SOAD_SOCON_ONLINE )
    { /* socket connection is online */
#if ( DOIP_VTP_OPTIMIZED_ENABLED == STD_ON )
      uint8_least txReqIdx;

      /* handle configured number of transmissmission requests within on main function cycle */
      for ( txReqIdx = 0u; txReqIdx < DOIP_VTCP_TX_QUEUE_REQ_CNT; txReqIdx++ )
#endif /* DOIP_VTP_OPTIMIZED_ENABLED == STD_ON */
      {
        uint8 queLvl = DoIP_VTcpTxQueueGetLevel(connection);

        if ( queLvl > 0 )
        {
          DoIP_TcpTxQueue_TransmitElement(connection);
        }

#if ( DOIP_VTP_OPTIMIZED_ENABLED == STD_ON )
        if ( (DoIP_VTcpTxQueueGetLevel(connection) == 0u) || (queLvl == DoIP_VTcpTxQueueGetLevel(connection)) )
        { /* queue is empty or queue level has not been changed (e.g. transmission still active) */
          break;
        }
#endif /* DOIP_VTP_OPTIMIZED_ENABLED == STD_ON */
      }
    }
  }
} /* DoIP_TcpTxQueue_HandleTransmit() */

/**********************************************************************************************************************
 *  DoIP_TcpRxGeneric_CopyGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxGeneric_CopyGenHdr(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] < DOIP_GEN_HDR_LEN_BYTE )
  { /* generic header was not received completely */
    PduLengthType bytesToCopy = PduInfoPtr->SduLength;

    /* PRQA S 0310, 3305 4 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
        DoIP_VTcpCon2RxMsgOffset[ConIdx]],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&PduInfoPtr->SduDataPtr[*ReadDataLenPtr],
      (uint32)bytesToCopy);
    *ReadDataLenPtr                   += bytesToCopy;
    DoIP_VTcpCon2RxMsgOffset[ConIdx] += bytesToCopy;
  }

  if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] >= DOIP_GEN_HDR_LEN_BYTE )
  { /* generic header was received completely */
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_TcpRxGeneric_CopyGenHdr() */

/**********************************************************************************************************************
 *  DoIP_TcpRxGeneric_CopyDataToHeaderBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  PduLengthType BytesToCopy)
{
  if ( BytesToCopy > (PduLengthType)(PduInfoPtr->SduLength - *ReadDataLenPtr) )
  {
    BytesToCopy = (PduLengthType)(PduInfoPtr->SduLength - *ReadDataLenPtr);
  }

  if ( BytesToCopy > 0u )
  {
    /* copy data to header buffer */
    /* PRQA S 0310, 3305 3 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
      DoIP_VTcpCon2RxMsgOffset[ConIdx]],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&PduInfoPtr->SduDataPtr[*ReadDataLenPtr],
      (uint32)BytesToCopy);
    *ReadDataLenPtr                   += BytesToCopy;
    DoIP_VTcpCon2RxMsgOffset[ConIdx] += BytesToCopy;
  }
} /* DoIP_TcpRxGeneric_CopyDataToHeaderBuffer() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleRoutingActivationReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleRoutingActivationReq(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  PduLengthType bytesToCopy;
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

  /* check message length */
  if ( hdrLen > DOIP_MAX_REQUEST_BYTES )
  { /* max length exceeded */
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR, FALSE);
  }
  /* check message length */
  else if ( (hdrLen != DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE) &&
            (hdrLen != (DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE + DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE)) )
  { /* length check failed */
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE);
  }
  else
  {
    bytesToCopy = (PduLengthType)((hdrLen + DOIP_GEN_HDR_LEN_BYTE) - DoIP_VTcpCon2RxMsgOffset[ConIdx]);

    DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(ConIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);

    /* check whether message is now copied completely */
    if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] == (hdrLen + DOIP_GEN_HDR_LEN_BYTE) )
    { /* message copied completely */
      DoIP_TcpRxRout_HandleCopiedRoutingActivationReq(ConIdx, ErrorIdPtr);
    }
    else
    { /* set bufferSize to length of pending message part */
      *BufferSizePtr = (PduLengthType)((hdrLen + DOIP_GEN_HDR_LEN_BYTE) - DoIP_VTcpCon2RxMsgOffset[ConIdx]);
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
    }
  }

#if ( DOIP_DEV_ERROR_DETECT == STD_OFF )
  DOIP_DUMMY_STATEMENT_VAR(ErrorIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_OFF */
} /* DoIP_TcpRxRout_HandleRoutingActivationReq() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleCopiedRoutingActivationReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleCopiedRoutingActivationReq(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_RoutActiveReqType, AUTOMATIC, DOIP_APPL_VAR) reqPtr =
    ((P2VAR(DoIP_RoutActiveReqType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
      DOIP_GEN_HDR_LEN_BYTE]);
  uint16 testerAddr;
  uint8  testerIdx = DOIP_VTESTER_TOTAL;
  uint8  routActColIdx = 0u;
  uint8  routActIdx = 0u;
#if ( DOIP_VERSION == DOIP_ISO_13400_2_2012 )
  uint8  activType;
#else
  uint16 activType;
#endif /* DOIP_VERSION == DOIP_ISO_13400_2_2012 */

  /* ----- Implementation ----------------------------------------------- */
  DoIP_VTcpCon2InactivCnt[ConIdx]  = 0u;  /* reset inactivity timer */
  DoIP_VTcpCon2RxMsgOffset[ConIdx] = 0u;  /* finish message */
  DoIP_VRoutActivHandler.TcpCon = ConIdx; /* lock routing activation handler */

  DOIP_GET_UINT16(reqPtr->TestAddr, 0u, testerAddr);

  /* check if any tester with received tester address is configured */
  if ( DoIP_TcpRxRout_GetTesterIdxByTesterAddr(testerAddr, &testerIdx) != E_OK )
  { /* tester address is not configured */
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
      DOIP_ROUT_ACTIV_RES_CODE_UNKNOWN_TESTER, testerIdx, NULL_PTR, TRUE);
  }
  else
  {
    DoIP_VRoutActivHandler.TesterIdx = testerIdx;
    DoIP_VRoutActivHandler.TesterAddr = testerAddr;

    /* check if any routing activation is configured for this activation number/type */
#if ( DOIP_VERSION == DOIP_ISO_13400_2_2012 )
    activType = reqPtr->ActivType[0u];
#else
    DOIP_GET_UINT16(reqPtr->ActivType, 0u, activType);
#endif /* DOIP_VERSION == DOIP_ISO_13400_2_2012 */
    for ( ; routActColIdx < DoIP_VTester[testerIdx].RoutActNum; routActColIdx++ )
    {
      routActIdx = DoIP_VTester[testerIdx].RoutActColPtr[routActColIdx];
      if ( DoIP_VRoutingActivations[routActIdx].RoutingActivationNum == activType )
      { /* routing activation with corresponding activation number/type found */
        break;
      }
    }

    if ( DoIP_VTester[testerIdx].RoutActNum == routActColIdx )
    { /* activation type check failed */
      (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
        DOIP_ROUT_ACTIV_RES_CODE_INV_ACTIV_TYPE, testerIdx, NULL_PTR, TRUE);
    }
    else
    {
      DoIP_VRoutActivHandler.RoutActIdx = routActIdx;

      /* get OEM specific part */
      DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem(ConIdx);

      if ( DoIP_TcpRxRout_CheckForRegisteredTester(ConIdx, testerAddr, testerIdx, ErrorIdPtr) ==
        E_OK )
      { /* tester already or not yet activated */
        DoIP_TcpRxRout_PerformRoutingActivation();
      }
    }
  }
} /* DoIP_TcpRxRout_HandleCopiedRoutingActivationReq() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 4 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  P2VAR(DoIP_RoutActiveReqType, AUTOMATIC, DOIP_APPL_VAR) reqPtr =
  ((P2VAR(DoIP_RoutActiveReqType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
    DOIP_GEN_HDR_LEN_BYTE]);
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

  if ( hdrLen == DOIP_ROUT_ACTIV_REQ_MAX_LEN_BYTE )
  { /* OEM specific part available */
    /* initalize OEM specific part for response */
    DoIP_VRoutActivHandler.OemSpecRes[0u] = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[1u] = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[2u] = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[3u] = 0u;

    /* store OEM specific part for request */
    DoIP_VRoutActivHandler.OemSpecReq[0u] = reqPtr->Oem[0u];
    DoIP_VRoutActivHandler.OemSpecReq[1u] = reqPtr->Oem[1u];
    DoIP_VRoutActivHandler.OemSpecReq[2u] = reqPtr->Oem[2u];
    DoIP_VRoutActivHandler.OemSpecReq[3u] = reqPtr->Oem[3u];

    DoIP_VRoutActivHandler.OemSpecUsed = TRUE;
  }
  else
  {
    DoIP_VRoutActivHandler.OemSpecUsed = FALSE;
  }
} /* DoIP_TcpRxRout_HandleCopiedRoutingActivationReqOem() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_CheckForRegisteredTester()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_CheckForRegisteredTester(
  PduIdType ConIdx,
  uint16 TesterAddr,
  uint8 TesterIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx != DOIP_INV_TESTER_IDX )
  { /* a tester is already assigned to this socket connection */
    if ( TesterIdx != DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx )
    { /* other TesterAddr is already active on this socket connection -> tx nack and close socket */
      (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, TesterAddr,
        DOIP_ROUT_ACTIV_RES_CODE_DIFFERENT_TESTER, TesterIdx, NULL_PTR, TRUE);
      retVal = E_NOT_OK;
    }
    else
    { /* tester address is already active on this socket */
      /* nothing to do here */
    }
  }
  else
  { /* current socket connection has no registered tester */
    retVal = DoIP_TcpRxRout_HandleNoTesterRegistered(ConIdx, TesterAddr, ErrorIdPtr);
  }

  return retVal;
} /* DoIP_TcpRxRout_CheckForRegisteredTester() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleNoTesterRegistered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_HandleNoTesterRegistered(
  PduIdType ConIdx,
  uint16 TesterAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 locAddrIdx = DoIP_VCon2LocalAddrIdx[ConIdx];
  PduIdType tmpConnection;

  /* ----- Implementation ----------------------------------------------- */
  DoIP_TcpRxRout_GetConIdxOfTesterAddr(TesterAddr, &tmpConnection);
  if ( tmpConnection != DOIP_INV_PDU_ID )
  {
    DoIP_TcpRxRout_TriggerAliveCheckSingleConnection(tmpConnection, ErrorIdPtr);
  }
  else if ( DoIP_TcpRxRout_GetAvailableSockNum(locAddrIdx) == 0u )
  {
    DoIP_TcpRxRout_TriggerAliveCheckAllConnections(ConIdx, ErrorIdPtr);
  }
  else
  { /* tester is not active on a different socket and a TCP_DATA socket is available */
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_TcpRxRout_HandleNoTesterRegistered() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_TriggerAliveCheckSingleConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0779 4 */ /* MD_MSR_5.1_779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_TriggerAliveCheckSingleConnection(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* TesterAddr is already assigned to another socket -> issue single alive check on this socket */
  if ( DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u, 0u, 0u, NULL_PTR, FALSE) == E_OK )
  {
    DoIP_VRoutActivHandler.AliveCheckList[0u] = ConIdx;
    DoIP_VRoutActivHandler.AliveCheckElemNum = 1u;
    /* load alive check timer */
    DoIP_VRoutActivHandler.AliveCheckCnt = DOIP_VALIVE_CHECK_WAIT;
  }
  else
  {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
    *ErrorIdPtr = DOIP_E_NOBUFS;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  }
} /* DoIP_TcpRxRout_TriggerAliveCheckSingleConnection() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_TriggerAliveCheckAllConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_TriggerAliveCheckAllConnections(
  PduIdType ConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType tmpConnection;

  /* ----- Implementation ----------------------------------------------- */
  /* all sockets but max one are in use -> issue alive check on all active sockets */
  for ( tmpConnection = 0u; tmpConnection < DOIP_VCONNECTION_TCP_TOTAL; tmpConnection++ )
  {
    /* filter local address */
    if ( (DoIP_VCon2LocalAddrIdx[tmpConnection] == DoIP_VCon2LocalAddrIdx[ConIdx]) &&
          (DoIP_VTcpCon2ActivMgt[tmpConnection].TesterIdx != DOIP_INV_TESTER_IDX) )
    { /* valid socket found -> issue alive check */
      if ( DoIP_TcpTx_Transmit(tmpConnection, DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u, 0u, 0u, NULL_PTR, FALSE) == E_OK )
      {
        DoIP_VRoutActivHandler.AliveCheckList[DoIP_VRoutActivHandler.AliveCheckElemNum] = tmpConnection;
        DoIP_VRoutActivHandler.AliveCheckElemNum++;
      }
      else
      {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
        *ErrorIdPtr = DOIP_E_NOBUFS;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
      }
    }
  }

  /* load alive check timer */
  DoIP_VRoutActivHandler.AliveCheckCnt = DOIP_VALIVE_CHECK_WAIT;
} /* DoIP_TcpRxRout_TriggerAliveCheckAllConnections() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetTesterIdxByTesterAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_GetTesterIdxByTesterAddr(
  uint16 TesterAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TesterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 testerIdx;

  /* ----- Implementation ----------------------------------------------- */
  *TesterIdx = DOIP_INV_TESTER_IDX;

  for ( testerIdx = 0u; testerIdx < DOIP_VTESTER_TOTAL; testerIdx++ )
  {
    if ( DoIP_VTester[testerIdx].TesterAddr == TesterAddr )
    {
      *TesterIdx = testerIdx;
      retVal = E_OK;
      break;
    }
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
    if ( DoIP_VTester[testerIdx].TesterAddr == DOIP_ADDR_ANY )
    { /* default tester */
      /* check if tester is used on another connection with same address */
      PduIdType connection;

      *TesterIdx = testerIdx;
      retVal = E_OK;

      for ( connection = 0u; connection < DOIP_VCONNECTION_TCP_TOTAL; connection++ )
      {
        if ( (testerIdx == DoIP_VTcpCon2ActivMgt[connection].TesterIdx) &&
            (TesterAddr != DoIP_VTcpCon2ActivMgt[connection].TesterAddr) )
        { /* default tester is already in use with different logical address */
          /* default tester is not available */
          *TesterIdx = DOIP_INV_TESTER_IDX;
          retVal = E_NOT_OK;
          break;
        }
      }
    }
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */
  }

  return retVal;
} /* DoIP_TcpRxRout_GetTesterIdxByTesterAddr() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetConIdxOfTesterAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_GetConIdxOfTesterAddr(
  uint16 TesterAddr,
  P2VAR(PduIdType, AUTOMATIC, DOIP_APPL_VAR) ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType idx;

  /* ----- Implementation ----------------------------------------------- */
  *ConIdx = DOIP_INV_PDU_ID;

  for ( idx = 0u; idx < DOIP_VCONNECTION_TCP_TOTAL; idx++ )
  {
    if ( DoIP_VTcpCon2ActivMgt[idx].TesterAddr == TesterAddr )
    {
      *ConIdx = idx;
      break;
    }
  }
} /* DoIP_TcpRxRout_GetConIdxOfTesterAddr() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_GetAvailableSockNum()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(uint8, DOIP_CODE) DoIP_TcpRxRout_GetAvailableSockNum(
  uint8 LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType localSoAdRxPduId;
  PduIdType routActivNum = 0u;
  PduIdType connectionNum = 0u;
  uint8 availableSockNum;

  /* ----- Implementation ----------------------------------------------- */
  for ( localSoAdRxPduId = 0u; localSoAdRxPduId < DOIP_VCONNECTION_TCP_TOTAL; localSoAdRxPduId++ )
  {
    if ( DoIP_VCon2LocalAddrIdx[localSoAdRxPduId] == LocalAddrIdx )
    { /* local address index match */
      connectionNum++;
      if ( DoIP_VTcpCon2ActivMgt[localSoAdRxPduId].TesterIdx != DOIP_INV_TESTER_IDX )
      { /* tester connected */
        routActivNum++;
      }
    }
  }

  availableSockNum = (uint8)((connectionNum - routActivNum) - 1u); /* PRQA S 3382 */ /* MD_DoIP_21.1_CFG */

  return availableSockNum;
} /* DoIP_TcpRxRout_GetAvailableSockNum() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivation(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduIdType connection = DoIP_VRoutActivHandler.TcpCon;
  DoIP_RoutActivStateType nextState = DOIP_ROUT_ACTIV_STATE_NONE;

  uint16 testerAddr = DoIP_VRoutActivHandler.TesterAddr;
  uint8  testerIdx  = DoIP_VRoutActivHandler.TesterIdx;
  uint8  routActIdx = DoIP_VRoutActivHandler.RoutActIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* check OEM part */
  if ( (DoIP_VRoutActivHandler.OemSpecUsed == FALSE) &&
     ((DoIP_VRoutingActivations[routActIdx].AuthReqLength != 0u) ||
      (DoIP_VRoutingActivations[routActIdx].ConfReqLength != 0u)) )
  { /* routing activation requires OEM specific part */
    DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

    (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
      DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);
  }
  else
  {
    /* call authentication callback */
    if ( (DoIP_VRoutingActivations[routActIdx].AuthFuncPtr != NULL_PTR) )
    {
      retVal = DoIP_TcpRxRout_PerformRoutingActivationAuth(&nextState);
    }

    /* call confirmation callback */
    if ( (DoIP_VRoutingActivations[routActIdx].ConfFuncPtr != NULL_PTR) && (retVal == E_OK) )
    {
      retVal = DoIP_TcpRxRout_PerformRoutingActivationConf(&nextState);
    }

    if ( retVal == E_OK )
    {
      /* register tester */
      DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
      DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
      DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;

      /* set mapping for connection and tester */
      DoIP_VTester2TcpCon[testerIdx] = connection;

      /* routing activated */
      DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_ACTIVATED;

      /* reset inactivity counter */
      DoIP_VTcpCon2InactivCnt[connection] = 0u;

      (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
        DOIP_ROUT_ACTIV_RES_CODE_SUCCESS, testerIdx, NULL_PTR, FALSE);
    }
  }
} /* DoIP_TcpRxRout_PerformRoutingActivation() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivationAuth()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivationAuth(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean authentified = FALSE;

  PduIdType connection = DoIP_VRoutActivHandler.TcpCon;
  uint16    testerAddr = DoIP_VRoutActivHandler.TesterAddr;
  uint8     testerIdx  = DoIP_VRoutActivHandler.TesterIdx;
  uint8     routActIdx = DoIP_VRoutActivHandler.RoutActIdx;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON )
  if ( DoIP_VRoutingActivations[routActIdx].AuthFuncWithRemAddr == TRUE )
  {
    SoAd_SockAddrInet6Type remAddr;

    remAddr.domain = SOAD_AF_UNSPEC;

    /* PRQA S 0310 6 */ /* MD_DoIP_11.4 */
    if ( SoAd_GetRemoteAddr(DoIP_VCon2SoConId[connection], DOIP_A_P2VAR(SoAd_SockAddrType)&remAddr) == E_OK )
    {
      /* PRQA S 0313 1 */ /* MD_DoIP_11.1 */
      retVal = ((DoIP_AuthWithRemAddrFctPtrType)DoIP_VRoutingActivations[routActIdx].AuthFuncPtr)
        (&authentified, DoIP_VRoutActivHandler.OemSpecReq, DoIP_VRoutActivHandler.OemSpecRes,
        DOIP_A_P2CONST(SoAd_SockAddrType)&remAddr);
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
#endif /* DOIP_VROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON */
  {
    retVal = DoIP_VRoutingActivations[routActIdx].AuthFuncPtr
      (&authentified, DoIP_VRoutActivHandler.OemSpecReq, DoIP_VRoutActivHandler.OemSpecRes);
  }

  if ( retVal == E_NOT_OK )
  { /* authentication failed */
    DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
    DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
    DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;
    DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

    (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
      DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);
  }
  else if ( retVal == DOIP_E_PENDING )
  { /* authentication pending */
    DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
    DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
    DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;
    DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_AUTH_PENDING;
  }
  else
  { /* authentication successful (callback return value) */
    if ( authentified != TRUE )
    { /* authentication failed */
      DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
      DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
      DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;
      DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

      (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
        DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);
    }
    else
    { /* authentication successful */
      *NextStatePtr = DOIP_ROUT_ACTIV_STATE_ACTIVATED;
    }
  }

  return retVal;
} /* DoIP_TcpRxRout_PerformRoutingActivationAuth() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_PerformRoutingActivationConf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_PerformRoutingActivationConf(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean confirmed = FALSE;
  uint8 offsetReq = 0u;
  uint8 offsetRes = 0u;

  PduIdType connection = DoIP_VRoutActivHandler.TcpCon;
  uint16    testerAddr = DoIP_VRoutActivHandler.TesterAddr;
  uint8     testerIdx  = DoIP_VRoutActivHandler.TesterIdx;
  uint8     routActIdx = DoIP_VRoutActivHandler.RoutActIdx;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VRoutingActivations[routActIdx].ConfReqLength > 0u )
  {
    offsetReq = (uint8)(DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE - DoIP_VRoutingActivations[routActIdx].ConfReqLength);
  }
  if ( DoIP_VRoutingActivations[routActIdx].ConfResLength > 0u )
  {
    offsetRes = (uint8)(DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE - DoIP_VRoutingActivations[routActIdx].ConfResLength);
  }

#if ( DOIP_VROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON )
  if ( DoIP_VRoutingActivations[routActIdx].ConfFuncWithRemAddr == TRUE )
  {
    SoAd_SockAddrInet6Type remAddr;

    remAddr.domain = SOAD_AF_UNSPEC;

    /* PRQA S 0310 7 */ /* MD_DoIP_11.4 */
    if ( SoAd_GetRemoteAddr(DoIP_VCon2SoConId[connection], DOIP_A_P2VAR(SoAd_SockAddrType)&remAddr) == E_OK )
    {
      /* PRQA S 0313 2 */ /* MD_DoIP_11.1 */
      /* PRQA S 3689 2 */ /* MD_DoIP_21.1_IDX */
      retVal = ((DoIP_ConfWithRemAddrFctPtrType)DoIP_VRoutingActivations[routActIdx].ConfFuncPtr)
        (&confirmed, &DoIP_VRoutActivHandler.OemSpecReq[offsetReq], &DoIP_VRoutActivHandler.OemSpecRes[offsetRes],
        DOIP_A_P2CONST(SoAd_SockAddrType)&remAddr);
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
#endif /* DOIP_VROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON */
  {
    /* PRQA S 3689 2 */ /* MD_DoIP_21.1_IDX */
    retVal = DoIP_VRoutingActivations[routActIdx].ConfFuncPtr
      (&confirmed, &DoIP_VRoutActivHandler.OemSpecReq[offsetReq], &DoIP_VRoutActivHandler.OemSpecRes[offsetRes]);
  }

  if ( retVal == E_NOT_OK )
  { /* confirmation failed */
    DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
    DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
    DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;
    DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

    (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
      DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REJECTED, testerIdx, NULL_PTR, TRUE);
  }
  else if ( retVal == DOIP_E_PENDING )
  { /* confirmation pending */
    DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
    DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
    DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;

    if ( *NextStatePtr == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    { /* authentication performed in same context - first pending returned for confirmation */
      DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_CONF_PENDING;

      /* routing activation response has to be sent */
      (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
        DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REQUIRED, testerIdx, NULL_PTR, FALSE);
    }
  }
  else
  { /* confirmation successful (callback return value) */
    if ( confirmed != TRUE )
    { /* confirmation failed */
      DoIP_VTcpCon2ActivMgt[connection].TesterIdx  = testerIdx;
      DoIP_VTcpCon2ActivMgt[connection].TesterAddr = testerAddr;
      DoIP_VTcpCon2ActivMgt[connection].RoutActIdx = routActIdx;
      DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

      (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
        DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REJECTED, testerIdx, NULL_PTR, TRUE);
    }
    else
    { /* confirmation successful */
      *NextStatePtr = DOIP_ROUT_ACTIV_STATE_ACTIVATED;
    }
  }

  return retVal;
} /* DoIP_TcpRxRout_PerformRoutingActivationConf() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection = DoIP_VRoutActivHandler.TcpCon;

  /* ----- Implementation ----------------------------------------------- */
  if ( (connection != DOIP_INV_PDU_ID) && (DoIP_VRoutActivHandler.AliveCheckCnt == 0u) )
  { /* routing activation but no alive check is pending */
    DoIP_RoutActivStateType nextState = DOIP_ROUT_ACTIV_STATE_NONE;
    DoIP_RoutActivStateType lastState = DoIP_VTcpCon2ActivMgt[connection].ActivState;

    uint16 testerAddr = DoIP_VRoutActivHandler.TesterAddr;
    uint8  testerIdx  = DoIP_VRoutActivHandler.TesterIdx;
    uint8  routActIdx = DoIP_VRoutActivHandler.RoutActIdx;

    /* call authentication callback */
    if ( lastState == DOIP_ROUT_ACTIV_STATE_AUTH_PENDING )
    {
      (void)DoIP_TcpRxRout_PerformRoutingActivationAuth(&nextState);
    }

    /* call confirmation callback */
    if ( (lastState == DOIP_ROUT_ACTIV_STATE_CONF_PENDING) || ((nextState == DOIP_ROUT_ACTIV_STATE_ACTIVATED) &&
      (DoIP_VRoutingActivations[routActIdx].ConfFuncPtr != NULL_PTR)) )
    {
      (void)DoIP_TcpRxRout_PerformRoutingActivationConf(&nextState);
    }

    /* routing activated */
    if ( nextState == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    { /* routing activation successful */
      /* set mapping for connection and tester */
      DoIP_VTester2TcpCon[testerIdx] = connection;

      DoIP_VTcpCon2ActivMgt[connection].ActivState = DOIP_ROUT_ACTIV_STATE_ACTIVATED;

      if ( lastState != DOIP_ROUT_ACTIV_STATE_CONF_PENDING )
      { /* not activated after confirmation was pending (i.e. response was already sent) */
        /* reset inactivity counter */
        DoIP_VTcpCon2InactivCnt[connection] = 0u;

        /* send routing activation response */
        (void)DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,
          DOIP_ROUT_ACTIV_RES_CODE_SUCCESS, testerIdx, NULL_PTR, FALSE);
      }
      else
      { /* activated after confirmation was pending (i.e. response was already sent) */
        /* release routing activation handler */
        DoIP_VRoutActivHandler.TcpCon = DOIP_INV_PDU_ID;
      }
    }
  }
} /* DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleAliveCheckTimeouts()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleAliveCheckTimeouts(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection = DoIP_VRoutActivHandler.TcpCon;

  /* ----- Implementation ----------------------------------------------- */
  if ( connection != DOIP_INV_PDU_ID )
  {
    /* alive check */
    if ( DoIP_VRoutActivHandler.AliveCheckCnt > 0u )
    { /* handle alive check */
      DoIP_VRoutActivHandler.AliveCheckCnt--;
      if ( DoIP_VRoutActivHandler.AliveCheckCnt == 0u )
      { /* clean up all timed-out socket connections */
        DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose();
        if ( DoIP_TcpRxRout_CheckConnectionAvailablility(connection) == E_OK )
        { /* continue with received routing activation request */
          DoIP_TcpRxRout_PerformRoutingActivation();
        }
      }
    }
  }
} /* DoIP_TcpRxRout_HandleAliveCheckTimeouts() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType listIdx;

  /* ----- Implementation ----------------------------------------------- */
  for (listIdx = 0u; listIdx < DOIP_VMAX_CONS_PER_LOC_ADDR; listIdx++ )
  {
    if ( DoIP_VRoutActivHandler.AliveCheckElemNum == 0u )
    { /* all sockets closed or Alive Check Response received */
      break;
    }
    if ( DoIP_VRoutActivHandler.AliveCheckList[listIdx] != DOIP_INV_PDU_ID )
    { /* alive check timeout -> close socket connection */
      PduIdType conIdx = DoIP_VRoutActivHandler.AliveCheckList[listIdx];

      DoIP_Con_SoConClose(conIdx);
      DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx = DOIP_INV_TESTER_IDX;
      DoIP_VTcpCon2ActivMgt[conIdx].TesterAddr = DOIP_ADDR_ANY;
      DoIP_VTcpCon2ActivMgt[conIdx].RoutActIdx = DOIP_INV_ROUT_ACT_IDX;
      DoIP_VTcpCon2ActivMgt[conIdx].ActivState = DOIP_ROUT_ACTIV_STATE_NONE;

      DoIP_VRoutActivHandler.AliveCheckList[listIdx] = DOIP_INV_PDU_ID;
      DoIP_VRoutActivHandler.AliveCheckElemNum--;
    }
  }
} /* DoIP_TcpRxRout_HandleAliveCheckTimeoutsSocketsClose() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_CheckConnectionAvailablility()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxRout_CheckConnectionAvailablility(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VCon2SoConState[ConIdx] == SOAD_SOCON_ONLINE )
  { /* socket connection with routing activation request is still online */
    PduIdType tmpSoAdRxPduId = DOIP_INV_PDU_ID;

    /* check if routing is (still) active on other socket */
    DoIP_TcpRxRout_GetConIdxOfTesterAddr(DoIP_VRoutActivHandler.TesterAddr, &tmpSoAdRxPduId);

    if ( tmpSoAdRxPduId != DOIP_INV_PDU_ID )
    { /* deny routing activation for active address on new socket connection */
      (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, DoIP_VRoutActivHandler.TesterAddr,
        DOIP_ROUT_ACTIV_RES_CODE_ALREADY_ACTIV, 0u, NULL_PTR, TRUE);
    }
    else
    { /* routing activation of new address */
      if ( DoIP_TcpRxRout_GetAvailableSockNum(DoIP_VCon2LocalAddrIdx[ConIdx]) == 0u )
      { /* all sockets but max one are in use - deny RoutingActivation request */
        (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, DoIP_VRoutActivHandler.TesterAddr,
          DOIP_ROUT_ACTIV_RES_CODE_SOCKETS_IN_USE, 0u, NULL_PTR, TRUE);
      }
      else
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* DoIP_TcpRxRout_CheckConnectionAvailablility() */

/**********************************************************************************************************************
 *  DoIP_TcpRxRout_HandleInactivityTimeouts()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxRout_HandleInactivityTimeouts(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType connection;

  /* ----- Implementation ----------------------------------------------- */
  for ( connection = 0u; connection < DOIP_VCONNECTION_TCP_TOTAL; connection++ )
  {
    if ( DoIP_VCon2SoConState[connection] == SOAD_SOCON_ONLINE )
    {
      if ( DoIP_VTcpCon2ActivMgt[connection].ActivState != DOIP_ROUT_ACTIV_STATE_ACTIVATED )
      { /* no routing active */
        if ( DoIP_VTcpCon2InactivCnt[connection] == DOIP_VINIT_INACTIV_WAIT )
        { /* initial inactivity timeout */
          /* close socket */
          DoIP_Con_SoConClose(connection);
        }
      }
      else
      { /* routing activated */
        if ( DoIP_VTcpCon2InactivCnt[connection] == DOIP_VGEN_INACTIV_WAIT )
        { /* general inactivity timeout */
          /* close socket */
          DoIP_Con_SoConClose(connection);
        }
      }
      DoIP_VTcpCon2InactivCnt[connection]++;
    }
  }
} /* DoIP_TcpRxRout_HandleInactivityTimeouts() */

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_HandleAliveCheckRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_HandleAliveCheckRes(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  PduLengthType bytesToCopy;
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

  /* check message length */
  if ( hdrLen > DOIP_MAX_REQUEST_BYTES )
  { /* max length exceeded */
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR, FALSE);
  }
  /* check message length */
  else if ( hdrLen != DOIP_ADDR_LEN_BYTE )
  { /* length check failed */
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE);
  }
  else
  {
    bytesToCopy = (PduLengthType)((hdrLen + DOIP_GEN_HDR_LEN_BYTE) - DoIP_VTcpCon2RxMsgOffset[ConIdx]);

    DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(ConIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);

    /* check whether message is now copied completely */
    if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] == (hdrLen + DOIP_GEN_HDR_LEN_BYTE) )
    {
      DoIP_TcpRxAlive_HandleCopiedAliveCheckRes(ConIdx);
    }
    else
    { /* set bufferSize to length of pending message part */
      *BufferSizePtr = (PduLengthType)((hdrLen + DOIP_GEN_HDR_LEN_BYTE) - DoIP_VTcpCon2RxMsgOffset[ConIdx]);
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
    }
  }
} /* DoIP_TcpRxAlive_HandleAliveCheckRes() */

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_HandleCopiedAliveCheckRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_HandleCopiedAliveCheckRes(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 testerIdx = DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx;
  uint16 testerAddr = DoIP_VTester[testerIdx].TesterAddr;
  uint16 testerAddrTmp;

  /* ----- Implementation ----------------------------------------------- */
  if ( testerIdx != DOIP_INV_TESTER_IDX )
  {
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
    if ( testerAddr == DOIP_ADDR_ANY )
    { /* default tester */
      testerAddr = DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr;
    }
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */

    DOIP_GET_UINT16(DoIP_VTcpCon2RxHdrBuf[ConIdx], DOIP_GEN_HDR_LEN_BYTE, testerAddrTmp);

    if ( testerAddrTmp == testerAddr )
    { /* alive check response with expected tester address */
      DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler(ConIdx);
    }
    else
    { /* alive check response with unexpected tester address */
      /* close socket */
      DoIP_VCon2SoConClosing[ConIdx] = TRUE;
      DoIP_Con_SoConClose(ConIdx);
    }
  }

  /* finish message */
  DoIP_VTcpCon2RxMsgOffset[ConIdx] = 0u;
} /* DoIP_TcpRxAlive_HandleCopiedAliveCheckRes() */

/**********************************************************************************************************************
 *  DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler(
  PduIdType ConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 tmpListIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( tmpListIdx = 0u; tmpListIdx < DOIP_VMAX_CONS_PER_LOC_ADDR; tmpListIdx++ )
  {
    if ( ConIdx == DoIP_VRoutActivHandler.AliveCheckList[tmpListIdx] )
    { /* entry found */
      /* remove it */
      DoIP_VRoutActivHandler.AliveCheckList[tmpListIdx] = DOIP_INV_PDU_ID;
      /* decrement size */
      DoIP_VRoutActivHandler.AliveCheckElemNum--;
      break;
    }
  }
} /* DoIP_TcpRxAlive_RemoveAliveCheckFromRoutActHandler() */

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleDiagnosticMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_HandleDiagnosticMessage(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

  /* check message length */
  if ( hdrLen > DOIP_MAX_REQUEST_BYTES )
  { /* max length exceeded */
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR, FALSE);
  }
  /* check message length */
  else if ( hdrLen <= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE )
  { /* length check failed */
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE);
  }
  else
  {
    DoIP_TcpRxDiag_HandleValidDiagnosticMessage(ConIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);
  }
} /* DoIP_TcpRxDiag_HandleDiagnosticMessage() */

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleValidDiagnosticMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_HandleValidDiagnosticMessage(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  DoIP_ChannelIdType channel = DOIP_INV_CHANNEL;
  PduInfoType localPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  retVal = DoIP_TcpRxDiag_CopyLogicalAddr(ConIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr, &channel);

  if ( retVal == E_OK )
  {
    /* set PDU info struct */
    localPduInfoPtr.SduLength  = (PduLengthType)(PduInfoPtr->SduLength - *ReadDataLenPtr);
    localPduInfoPtr.SduDataPtr = &PduInfoPtr->SduDataPtr[*ReadDataLenPtr];

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    /* check whether acknowledge message shall contain user data */
    DoIP_TcpRxDiag_AppendNackData(ConIdx, &localPduInfoPtr);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

    retVal = DoIP_VRxDiagMsgVerifyRxPdu(ConIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr, &channel);

    if ( retVal == E_OK )
    {
#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
      if ( DoIP_VRxDiagMsgCopyVerifiedUserData(ConIdx, BufferSizePtr, FALSE) == E_OK )
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
      {
        DoIP_VRxDiagMsgCopyUserData(ConIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr, &localPduInfoPtr);
      }
    }
  }

  if ( retVal == E_NOT_OK )
  {
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;
  }
} /* DoIP_TcpRxDiag_HandleValidDiagnosticMessage() */

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_CopyLogicalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_CopyLogicalAddr(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduLengthType bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* check whether diagnostic message header was copied completely to header buffer */
  if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] < (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
  { /* diagnostic message header was not copied completely to header buffer */

    bytesToCopy = (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) -
      DoIP_VTcpCon2RxMsgOffset[ConIdx]);

    DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(ConIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);

    /* check whether message header was copied completely */
    if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] == (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
    { /* message header copied completely */
      retVal = DoIP_TcpRxDiag_HandleCopiedLogicalAddr(ConIdx, Channel);
    }
    else
    { /* set bufferSize to length of pending message part */
      *BufferSizePtr = (PduLengthType)((DOIP_DIAG_HDR_LEN_BYTE + DOIP_GEN_HDR_LEN_BYTE) -
        DoIP_VTcpCon2RxMsgOffset[ConIdx]);
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
      retVal = DOIP_E_PENDING;
    }
  }

  return retVal;
} /* DoIP_TcpRxDiag_CopyLogicalAddr() */
/* PRQA L:MSR_STPAR */

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleCopiedLogicalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_HandleCopiedLogicalAddr(
  PduIdType ConIdx,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_APPL_VAR) diagReqPtr =
    (P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
    DOIP_GEN_HDR_LEN_BYTE];
  Std_ReturnType retVal;
  uint16 srcAddr;
  uint16 tgtAddr;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT16(diagReqPtr->SrcAddr, 0u, srcAddr);
  DOIP_GET_UINT16(diagReqPtr->TgtAddr, 0u, tgtAddr);

  /* check whether routing is active */
  if ( DoIP_VTcpCon2ActivMgt[ConIdx].ActivState != DOIP_ROUT_ACTIV_STATE_ACTIVATED )
  { /* missing routing activation */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR, TRUE);
    retVal = E_NOT_OK;
  }
  else
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
  if ( (DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr == DOIP_ADDR_ANY) &&
       (srcAddr != DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr) )
  { /* invalid tester address (default tester) */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR, TRUE);
    retVal = E_NOT_OK;
  }
  else if ( ( DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr != DOIP_ADDR_ANY ) &&
      (srcAddr != DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr) )
#else
  if ( srcAddr != DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr )
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */
  { /* invalid tester address (no default tester) */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR, TRUE);
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = DoIP_TcpRxDiag_HandleTargetLogicalAddress(ConIdx, Channel);
  }

  return retVal;
} /* DoIP_TcpRxDiag_HandleCopiedLogicalAddr() */

/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_HandleTargetLogicalAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TcpRxDiag_HandleTargetLogicalAddress(
  PduIdType ConIdx,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 5 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_APPL_VAR) diagReqPtr =
    (P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
    DOIP_GEN_HDR_LEN_BYTE];
  Std_ReturnType retVal = E_OK;
  uint8 routActIdx = DoIP_VTcpCon2ActivMgt[ConIdx].RoutActIdx;
  uint32 diagMsgLength;
  uint32 hdrLen;
  uint16 srcAddr;
  uint16 tgtAddr;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);
  DOIP_GET_UINT16(diagReqPtr->SrcAddr, 0u, srcAddr);
  DOIP_GET_UINT16(diagReqPtr->TgtAddr, 0u, tgtAddr);

  diagMsgLength = (uint32)(hdrLen - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE);

  *Channel = DoIP_VGetChannelByTgtAddrForRoutAct(routActIdx, tgtAddr, diagMsgLength);

  if ( *Channel == DOIP_INV_CHANNEL )
  { /* target address not configured for routing activation */
    /* check if target address is configured at all */
    *Channel = DoIP_VGetChannelByTgtAddr(tgtAddr);

    if ( *Channel != DOIP_INV_CHANNEL )
    { /* target address is configured for an unreachable channel */
      DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
        DOIP_DIAG_MSG_RES_CODE_TGT_UNREACHABLE, FALSE);
    }
    else
    {
      /* target address is not configured */
      DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
        DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR, FALSE);
    }
    retVal = E_NOT_OK;
  }
  else if ( DoIP_VChannel2TpMaxLen[*Channel] < diagMsgLength )
  { /* max length of target Transport Protocol exceeded */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE, FALSE);
    retVal = E_NOT_OK;
  }
#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
  else if ( (DoIP_IsPduSizeRoutingEnabled == TRUE) && (DoIP_VChannel2MaxPduSize[*Channel] < diagMsgLength) )
  { /* max length of PDU exceeded */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE, FALSE);
    retVal = E_NOT_OK;
  }
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */
#if ( DOIP_VVERIFY_TARGET_ADDR_ENABLED == STD_ON )
  else if ( DoIP_VCallVerifyTargetAddrDirect(tgtAddr) != E_OK )
  { /* target address not accepted */
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR, FALSE);
    retVal = E_NOT_OK;
  }
#endif /* DOIP_VVERIFY_TARGET_ADDR_ENABLED == STD_ON */
  else
  { /* valid target address and within max length of the target Transport Protocol */
    /* nothing to do here */
  }

  return retVal;
} /* DoIP_TcpRxDiag_HandleTargetLogicalAddress() */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TcpRxDiag_AppendNackData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TcpRxDiag_AppendNackData(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* check whether acknowledge message shall contain user data */
  if ( PduInfoPtr->SduLength > 0u )
  {
    uint8 testerIdx = DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx;

    if ( DoIP_VTester[testerIdx].DiagAckNackNumByte > 0u )
    { /* diagnostic message acknowledge shall contain user data */
      bytesToCopy = (PduLengthType)(DoIP_VTester[testerIdx].DiagAckNackNumByte -
        DoIP_VTester2DiagNAckOffset[testerIdx]);
      if ( bytesToCopy > PduInfoPtr->SduLength )
      {
        bytesToCopy = PduInfoPtr->SduLength;
      }
      if ( bytesToCopy > 0u )
      { /* copy data to DiagNAck buffer */
        /* PRQA S 0310, 3305 3 */ /* MD_DoIP_11.4 */
        IpBase_Copy(
          (P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTester[testerIdx].DiagAckNackBufPtr[
            DoIP_VTester2DiagNAckOffset[testerIdx]],
          (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))PduInfoPtr->SduDataPtr, (uint32)bytesToCopy);
        DoIP_VTester2DiagNAckOffset[testerIdx] += bytesToCopy;
      }
    }
  }
} /* DoIP_TcpRxDiag_AppendNackData() */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DoIP_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  DoIP_State = DOIP_STATE_UNINIT;
} /* DoIP_InitMemory() */

/**********************************************************************************************************************
 *  DoIP_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
FUNC(void, DOIP_CODE) DoIP_Init(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) DoIPConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check parameter 'DoIPConfigPtr' */
  if ( DoIPConfigPtr != NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT_VAR(DoIPConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint16_least idx;

    /* initialize TCP socket info */
    idx = DOIP_VCONNECTION_TOTAL;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VCon2IpAddrState[idx]  = SOAD_IPADDR_STATE_UNASSIGNED;
      DoIP_VCon2SoConId[idx]      = SOAD_INV_SO_CON_ID;
      DoIP_VCon2SoConState[idx]   = SOAD_SOCON_OFFLINE;
      DoIP_VCon2SoConClosing[idx] = FALSE;
    }

    idx = DOIP_VCONNECTION_TCP_TOTAL;
    while ( idx > 0u )
    {
      uint16_least idx2 = DOIP_VTCP_TX_MGT_QUEUE_SIZE;

      idx--;
      DoIP_VTcpCon2ActivMgt[idx].TesterAddr    = DOIP_ADDR_ANY;
      DoIP_VTcpCon2ActivMgt[idx].TesterIdx     = DOIP_INV_TESTER_IDX;
      DoIP_VTcpCon2ActivMgt[idx].RoutActIdx    = DOIP_INV_ROUT_ACT_IDX;
      DoIP_VTcpCon2ActivMgt[idx].ActivState    = DOIP_ROUT_ACTIV_STATE_NONE;
      DoIP_VTcpCon2InactivCnt[idx]             = 0u;
      DoIP_VTcpCon2SkipRxMsg[idx]              = FALSE;
      DoIP_VTcpCon2RxMsgOffset[idx]            = 0u;
      DoIP_VTcpCon2TxBytesCopied[idx]          = 0u;
      DoIP_VTcpCon2LastRxBufSize[idx]          = 0u;
      DoIP_VTcpCon2LastTxBufSize[idx]          = 0u;
      DoIP_VTcpCon2RxChannelActive[idx]        = DOIP_INV_CHANNEL;
      DoIP_VTcpCon2TxMgtQueLvl[idx]            = 0u;
      DoIP_VTcpCon2TxMgtQueIdx[idx]            = 0u;
      DoIP_VTcpCon2TxTpSessionActive[idx]      = FALSE;
      DoIP_VTcpCon2RxTpSessionActive[idx]      = FALSE;
      DoIP_VTcpCon2TpPreTransMgt[idx].HdrType  = DOIP_HDR_TYPE_INVALID;
#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
      DoIP_VTcpCon2TxTpCancelRequested[idx]    = FALSE;
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#if ( DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON )
      DoIP_VTcpCon2RxTpCancelRequested[idx]    = FALSE;
#endif /* DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON */
      while ( idx2 > 0u )
      {
        idx2--;
        DoIP_VTcpCon2TcpTxMgt[idx][idx2].Channel     = DOIP_INV_CHANNEL;
        DoIP_VTcpCon2TcpTxMgt[idx][idx2].TxHdrBufLen = 0u;
      }

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.domain   = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.port     = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.addr[0u] = 0u;
# if ( SOAD_VIPV6_ENABLED == STD_ON )
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.addr[1u] = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.addr[2u] = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].RemIpAddr.addr[3u] = 0u;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.domain   = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.port     = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.addr[0u] = 0u;
# if ( SOAD_VIPV6_ENABLED == STD_ON )
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.addr[1u] = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.addr[2u] = 0u;
      DoIP_VTcpCon2RemLocIpAddr[idx].LocIpAddr.addr[3u] = 0u;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
      DoIP_VTcpCon2VerifyRxPduOffset[idx] = 0u;
    }

    idx = DOIP_VUDP_TX_MGT_LIST_SIZE;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VUdpTxMgtList[idx].UdpCon = DOIP_INV_PDU_ID;
    }

    idx = DOIP_VCHANNELS_TOTAL;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VChannel2TxMsgLen[idx] = 0u;
    }

    idx = DOIP_VTESTER_TOTAL;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VTester2TcpCon[idx] = DOIP_INV_PDU_ID;
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
      DoIP_VTester2DiagNAckOffset[idx] = 0u;
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
    }

    idx = DOIP_VLOCAL_ADDR_TOTAL;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VLocalAddrIdx2IpAddrReqState[idx] = DOIP_IP_ADDR_REQ_STATE_NONE;
    }

    idx = DOIP_VMAX_CONS_PER_LOC_ADDR;
    while ( idx > 0u )
    {
      idx--;
      DoIP_VRoutActivHandler.AliveCheckList[idx] = DOIP_INV_PDU_ID;
    }
    DoIP_VRoutActivHandler.OemSpecReq[0u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecReq[1u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecReq[2u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecReq[3u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[0u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[1u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[2u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecRes[3u]    = 0u;
    DoIP_VRoutActivHandler.OemSpecUsed       = FALSE;
    DoIP_VRoutActivHandler.RoutActIdx        = DOIP_INV_ROUT_ACT_IDX;
    DoIP_VRoutActivHandler.TesterAddr        = DOIP_ADDR_ANY;
    DoIP_VRoutActivHandler.TesterIdx         = DOIP_INV_TESTER_IDX;
    DoIP_VRoutActivHandler.AliveCheckCnt     = 0u;
    DoIP_VRoutActivHandler.AliveCheckElemNum = 0u;
    DoIP_VRoutActivHandler.TcpCon            = DOIP_INV_PDU_ID;

    DoIP_ActivationLineLocalState = DOIP_ACTIVATION_LINE_LOCAL_INACTIVE;
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( DOIP_VACTIV_LINE_DEFAULT_ACTIVE == STD_ON )
    DoIP_ActivationLineState = DOIP_ACTIVATION_LINE_ACTIVE;
# else
    DoIP_ActivationLineState = DOIP_ACTIVATION_LINE_INACTIVE;
# endif /* DOIP_VACTIV_LINE_DEFAULT_ACTIVE == STD_ON */
#else
    DoIP_ActivationLineState = DOIP_ACTIVATION_LINE_ACTIVE;
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
    DoIP_IsPduSizeRoutingEnabled = FALSE;
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */

#if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
    DoIP_GidInSync = FALSE;
#endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
    DoIP_VOemPayloadTypeTcpBufLen  = 0u;
    DoIP_VOemPayloadTypeTcpBufIsTx = FALSE;
    DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u */
    DoIP_VOemPayloadTypeUdpBufLen = 0u;
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

    DoIP_State = DOIP_STATE_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_INIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_Init() */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */

#if ( DOIP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check parameter 'versioninfo' */
  if ( versioninfo == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID         = DOIP_VENDOR_ID;
    versioninfo->moduleID         = DOIP_MODULE_ID;
    versioninfo->sw_major_version = DOIP_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = DOIP_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = DOIP_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_GET_VERSION_INFO, errorId);
  }
# else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_GetVersionInfo() */
#endif /* DOIP_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpTransmit(
  PduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) DoIPPduRTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRTxId' */
  else if ( DoIPPduRTxId >= DOIP_VCHANNELS_TX_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( DoIPPduRTxInfoPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( DoIPPduRTxInfoPtr->SduLength == 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'channel' - DoIP_VChannel2ApiType[channel] */
  else if ( DoIP_VChannel2ApiType[DoIP_VPduRTxPduId2Channel[DoIPPduRTxId]] != DOIP_API_TYPE_TP )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' - DoIP_VChannel2PduRTxConfPduId[channel] */
  else if ( DoIP_VChannel2PduRTxConfPduId[DoIP_VPduRTxPduId2Channel[DoIPPduRTxId]] == DOIP_INV_PDU_ID )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ChannelIdType channel = DoIP_VPduRTxPduId2Channel[DoIPPduRTxId];

    /* check whether TP session is active or requested for this channel */
    if ( DoIP_VChannel2TxMsgLen[channel] == 0u )
    {
      PduIdType connection = DoIP_VTester2TcpCon[DoIP_VChannel2TesterIdx[channel]];

      /* check whether tester is connected to channel */
      if ( connection != DOIP_INV_PDU_ID )
      { /* tester is connected */
        DoIP_VChannel2TxMsgLen[channel] = DoIPPduRTxInfoPtr->SduLength;

        if ( DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_DIAG_REQ, channel,
          (uint16)DoIPPduRTxInfoPtr->SduLength, 0u, NULL_PTR, FALSE) == E_OK )
        { /* transmit request successful */
          retVal = E_OK;
        }
      }
      if ( retVal == E_NOT_OK )
      { /* reset channel information */
        DoIP_VChannel2TxMsgLen[channel] = 0u;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpTransmit() */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_TpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit(
  PduIdType DoIPPduRTxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRTxId' */
  else if ( DoIPPduRTxId >= DOIP_VCHANNELS_TX_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' - DoIP_VChannel2ApiType[channel] */
  else if ( DoIP_VChannel2ApiType[DoIP_VPduRTxPduId2Channel[DoIPPduRTxId]] != DOIP_API_TYPE_TP )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
    DoIP_ChannelIdType channel = DoIP_VPduRTxPduId2Channel[DoIPPduRTxId];
    PduIdType connection = DoIP_VTester2TcpCon[DoIP_VChannel2TesterIdx[channel]];

    if ( connection != DOIP_INV_PDU_ID )
    { /* channel to connection mapping is available */
      if ( DoIP_VTcpCon2TxTpSessionActive[connection] == TRUE )
      { /* TP session of the corresponding socket connection is active */
        if ( DoIP_VChannel2TxMsgLen[channel] != 0u )
        { /* transmission of PDU is active */
          if ( SoAd_TpCancelTransmit(DoIP_VCon2SoAdTxPduId[connection]) == E_OK )
          { /* cancellation request successful */
            DoIP_VTcpCon2TxTpCancelRequested[connection] = TRUE;
            retVal = E_OK;
          }
        }
      }
    }
#else
    DOIP_DUMMY_STATEMENT_VAR(DoIPPduRTxId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpCancelTransmit() */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_TpCancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive(
  PduIdType DoIPPduRRxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRRxId' */
  else if ( DoIPPduRRxId >= DOIP_VCHANNELS_RX_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' - DoIP_VChannel2ApiType[channel] */
  else if ( DoIP_VChannel2ApiType[DoIP_VPduRRxCancelPduId2Channel[DoIPPduRRxId]] != DOIP_API_TYPE_TP )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON )
    DoIP_ChannelIdType channel = DoIP_VPduRRxCancelPduId2Channel[DoIPPduRRxId];
    PduIdType connection = DoIP_VTester2TcpCon[DoIP_VChannel2TesterIdx[channel]];

    if ( connection != DOIP_INV_PDU_ID )
    { /* channel to connection mapping is available */
      if ( DoIP_VTcpCon2RxChannelActive[connection] != DOIP_INV_CHANNEL )
      { /* TP session of the corresponding socket connection is active */
        if ( SoAd_TpCancelReceive(DoIP_VCon2SoAdRxCancelPduId[connection]) == E_OK )
        { /* cancellation request successful */
          DoIP_VTcpCon2RxTpCancelRequested[connection] = TRUE;
          retVal = E_OK;
        }
      }
    }
#else
    DOIP_DUMMY_STATEMENT_VAR(DoIPPduRRxId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_CANCEL_RECEIVE, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpCancelReceive() */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_IfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfTransmit(
  PduIdType id,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DOIP_VCHANNELS_TX_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( info->SduDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( info->SduLength == 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'channel' - DoIP_VChannel2ApiType[channel] */
  else if ( DoIP_VChannel2ApiType[DoIP_VPduRTxPduId2Channel[id]] != DOIP_API_TYPE_IF )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' - DoIP_VChannel2IfTxBuf[channel].MaxBufLen */
  else if ( info->SduLength > DoIP_VChannel2IfTxBuf[DoIP_VPduRTxPduId2Channel[id]].MaxBufLen )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ChannelIdType channel = DoIP_VPduRTxPduId2Channel[id];

    /* check if previous transmission is pending */
    if ( DoIP_VChannel2TxMsgLen[channel] == 0u )
    { /* no transmission pending */
      /* get tester socket connection */
      PduIdType connection = DoIP_VTester2TcpCon[DoIP_VChannel2TesterIdx[channel]];

      if ( connection != DOIP_INV_PDU_ID )
      { /* tester is connected */
        DoIP_VChannel2TxMsgLen[channel] = info->SduLength;

        retVal = DoIP_TcpTx_Transmit(connection, DOIP_HDR_TYPE_DIAG_REQ, channel, (uint16)info->SduLength, 0u,
          info->SduDataPtr, FALSE);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_IfTransmit() */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_IfCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 2 */ /* MD_DoIP_3206 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit(
  PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DOIP_VCHANNELS_TX_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' - DoIP_VChannel2ApiType[channel] */
  else if ( DoIP_VChannel2ApiType[DoIP_VPduRTxPduId2Channel[id]] != DOIP_API_TYPE_IF )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
    DoIP_ChannelIdType channel = DoIP_VPduRTxPduId2Channel[id];
    PduIdType connection = DoIP_VTester2TcpCon[DoIP_VChannel2TesterIdx[channel]];

    if ( connection != DOIP_INV_PDU_ID )
    { /* channel to connection mapping is available */
      if ( DoIP_VTcpCon2TxTpSessionActive[connection] == TRUE )
      { /* TP session of the corresponding socket connection is active */
        if ( DoIP_VChannel2TxMsgLen[channel] != 0u )
        { /* transmission of PDU is active */
          if ( SoAd_TpCancelTransmit(DoIP_VCon2SoAdTxPduId[connection]) == E_OK )
          { /* cancellation request successful */
            DoIP_VTcpCon2TxTpCancelRequested[connection] = TRUE;
            retVal = E_OK;
          }
        }
      }
    }
#else
    DOIP_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_IfCancelTransmit() */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_TpChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpChangeParameter(
  PduIdType id,
  TPParameterType parameter,
  uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_DUMMY_STATEMENT_VAR(id);        /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(parameter); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(value);     /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */

  return retVal;
} /* DoIP_TpChangeParameter() */

/**********************************************************************************************************************
 *  DoIP_EnablePduSizeRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_EnablePduSizeRouting(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
    DoIP_IsPduSizeRoutingEnabled = TRUE;
#else
  /* API disabled */
  /* nothing to do here */
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_ENABLE_PDU_SIZE_ROUTING, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_EnablePduSizeRouting() */

/**********************************************************************************************************************
 *  DoIP_DisablePduSizeRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_DisablePduSizeRouting(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
    DoIP_IsPduSizeRoutingEnabled = FALSE;
#else
  /* API disabled */
  /* nothing to do here */
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_DISABLE_PDU_SIZE_ROUTING, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_DisablePduSizeRouting() */

/**********************************************************************************************************************
 *  DoIP_Shutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Shutdown(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
# if ( DOIP_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
  retVal = SoAd_Shutdown();
# else
  DoIP_State = DOIP_STATE_SHUTDOWN;
  retVal = SoAd_Shutdown();
# endif /* DOIP_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
#else
  /* API disabled */
  /* nothing to do here */
  retVal = E_NOT_OK;
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */

  return retVal;
} /* DoIP_Shutdown() */

/**********************************************************************************************************************
 *  DoIP_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      /* handling depends on activation line state */
      switch (DoIP_ActivationLineLocalState)
      {
        case DOIP_ACTIVATION_LINE_LOCAL_INACTIVE:
        { /* local activation line state inactive */
          if ( DoIP_ActivationLineState == DOIP_ACTIVATION_LINE_ACTIVE )
          { /* requested to be active */
            DoIP_ActivationLineLocalState = DOIP_ACTIVATION_LINE_LOCAL_INACTIVE_TO_ACTIVE;
            /* copy case DOIP_ACTIVATION_LINE_LOCAL_INACTIVE_TO_ACTIVE to handle state in this main function cycle */
            DoIP_Con_ActLineToActive();
          }
          break;
        }
        case DOIP_ACTIVATION_LINE_LOCAL_INACTIVE_TO_ACTIVE:
        { /* local activation line state in transition from inactive to active */
          DoIP_Con_ActLineToActive();
          break;
        }
        case DOIP_ACTIVATION_LINE_LOCAL_ACTIVE:
        { /* local activation line state active */
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
          if ( DoIP_ActivationLineState == DOIP_ACTIVATION_LINE_INACTIVE )
          { /* requested to be inactive */
            DoIP_ActivationLineLocalState = DOIP_ACTIVATION_LINE_LOCAL_ACTIVE_TO_INACTIVE;
            /* copy case DOIP_ACTIVATION_LINE_LOCAL_ACTIVE_TO_INACTIVE to handle state in this main function cycle */
            DoIP_Con_ActLineToInactive();
          }
          else
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
          {
#if ( (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
    ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) )
            DoIP_Con_HandleIpAddrReassignment();
#endif /* (DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON) || \
         ((DOIP_VIPADDR_ASSIGN_GLOBAL_PATTERN & DOIP_IPADDR_ASSIGN_PATTERN_DHCP) != 0u) */

            DoIP_Con_HandleSoConStates();

#if ( DOIP_VGID_SYNC_ENABLED == STD_ON )
            DoIP_Con_HandleGidSync();
#endif /* DOIP_VGID_SYNC_ENABLED == STD_ON */

            DoIP_TcpRxRout_HandleRetryRoutingActivationAuthAndConf();

            DoIP_UpdTxList_HandleEntries();

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
            DoIP_VHandleVerifiedUserData();
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

            DoIP_TcpTxQueue_HandleTransmit();

            DoIP_TcpRxRout_HandleAliveCheckTimeouts();

            DoIP_TcpRxRout_HandleInactivityTimeouts();
          }
          break;
        }
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
        case DOIP_ACTIVATION_LINE_LOCAL_ACTIVE_TO_INACTIVE:
        { /* local activation line state in transition from active to inactive */
          DoIP_Con_ActLineToInactive();
          break;
        }
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
        default:
        { /* invalid state */
          /* nothing to do here */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_MAIN_FUNCTION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_MainFunction() */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 3673 DoIP_16_7 */ /* MD_DoIP_16.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyTxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(RetryInfoType, AUTOMATIC, DOIP_APPL_VAR) retry,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduIdType conIdx = id; /* connection index is equal to id */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( conIdx >= DOIP_VCONNECTION_TCP_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( (info->SduLength != 0u) &&
            (info->SduDataPtr == NULL_PTR) )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'retry' */
  else if ( retry != NULL_PTR )
  {
    errorId = DOIP_E_INVALID_PARAMETER; /* AUTOSAR */
  }
  /* Check parameter 'availableDataPtr' */
  else if ( availableDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT_VAR(retry); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    /* check socket connection state */
    if ( DoIP_VCon2SoConState[conIdx] != SOAD_SOCON_ONLINE )
    { /* socket connection is not online */
      /* nothing to do here */
    }
    else
    {
      retVal = DoIP_TcpTxQueue_Copy(conIdx, info, availableDataPtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_COPY_TX_DATA, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpCopyTxData() */
/* PRQA L:MSR_14_7 */
/* PRQA L: DoIP_16_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoAdTpTxConfirmation(
  PduIdType id,
  Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType conIdx = id; /* connection index is equal to id */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( conIdx >= DOIP_VCONNECTION_TCP_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'result' */
  else if ( (result != NTFRSLT_OK) &&
            (result != NTFRSLT_E_NOT_OK) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      DoIP_TcpTxQueue_RemoveElement(conIdx, result);

#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
      /* reset cancellation */
      if ( DoIP_VTcpCon2TxTpCancelRequested[conIdx] == TRUE )
      {
        DoIP_VTcpCon2TxTpCancelRequested[conIdx] = FALSE;
      }
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */

      if ( (DoIP_VCon2SoConClosing[conIdx] == TRUE) && (DoIP_VTcpTxQueueGetLevel(conIdx) == 0u) )
      { /* socket shall be closed */
        DoIP_Con_SoConClose(conIdx);
      }
      else
      { /* check if Tx Queue contains elements */
#if ( DOIP_VTP_OPTIMIZED_ENABLED == STD_OFF )
        if ( DoIP_VTcpTxQueueGetLevel(conIdx) > 0u )
        { /* Tx Queue not empty, start transmission */
          DoIP_TcpTxQueue_TransmitElement(conIdx);
        }
#endif /* DOIP_VTP_OPTIMIZED_ENABLED == STD_OFF */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_TX_CONFIRMATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdTpTxConfirmation() */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduIdType conIdx = id; /* connection index is equal to id */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( conIdx >= DOIP_VCONNECTION_TCP_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'bufferSizePtr' */
  else if ( bufferSizePtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    /* Check socket connection state */
    if ( DoIP_VCon2SoConState[conIdx] != SOAD_SOCON_ONLINE )
    { /* socket connection is not online */
      /* nothing to do here */
    }
    /* Check TP session state */
    else if ( DoIP_VTcpCon2RxTpSessionActive[conIdx] != TRUE )
    { /* TP session is not active */
      /* nothing to do here */
    }
    /* Check whether socket connection is requested to be closed */
    else if ( DoIP_VCon2SoConClosing[conIdx] == TRUE )
    { /* socket connection closing is requested - do not handle more than pending rx data of message */
      *bufferSizePtr                         = 0u;
      DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
      retVal = BUFREQ_OK;
    }
#if ( DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON )
    /* Check TP cancellation */
    else if ( DoIP_VTcpCon2RxTpCancelRequested[conIdx] == TRUE )
    { /* TP cancellation is ongoing */
      /* nothing to do here */
    }
#endif /* DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    /* the feature to send previously received user data cannot be used if tester could not be determined */
    else if ( (DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx != DOIP_INV_TESTER_IDX) &&
              (DoIP_VTcpCon2RxMsgOffset[conIdx] == 0u) &&
              (DoIP_VTester2DiagNAckOffset[DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx] > 0u) )
    { /* diagnostic message acknowledge is pending - do not accept new data until acknowledge is sent */
      *bufferSizePtr                 = 0u;
      DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
      retVal = BUFREQ_OK;
    }
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
    /* SoAd queries Rx buffer size */
    else if ( (info->SduLength == 0u) &&
          (DoIP_VTcpCon2RxMsgOffset[conIdx] < DOIP_GEN_HDR_LEN_BYTE) )
    { /* generic header was not completely received */
      /* do only provide buffer for generic header */
      *bufferSizePtr = DOIP_GEN_HDR_LEN_BYTE - DoIP_VTcpCon2RxMsgOffset[conIdx];
      DoIP_VTcpCon2LastRxBufSize[conIdx] = *bufferSizePtr;
      retVal = BUFREQ_OK;
    }
    /* SoAd has Rx data */
    else if ( (info->SduLength != 0u) &&
              (info->SduDataPtr == NULL_PTR) )
    {
      errorId = DOIP_E_INVALID_PARAMETER;
      /* nothing to do here */
    }
    /* SoAd has Rx data AND check length */
    else if ( (info->SduLength != 0u) &&
          (DoIP_VTcpCon2LastRxBufSize[conIdx] < info->SduLength) )
    { /* unexpected length */
      /* nothing to do here */
    }
    else
    {
      PduLengthType readDataLen = 0u;

      if ( DoIP_VTcpCon2ActivMgt[conIdx].ActivState == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
      { /* routing activation active */
        /* reset inactivity timer */
        DoIP_VTcpCon2InactivCnt[conIdx] = 0u;
      }

      do
      {
        if ( DoIP_TcpRxGeneric_CopyGenHdr(conIdx, info, &readDataLen) == E_OK )
        { /* generic header was received completely */
          P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr;
          uint32 hdrLen;
          uint16 hdrType;

          /* PRQA S 0310 1 */ /* MD_DoIP_11.4 */
          hdrPtr = ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[conIdx][0u]);

          if ( ((uint8)hdrPtr->Ver != DOIP_PROTOCOL_VERSION) || ((uint8)(~hdrPtr->InvVer) != DOIP_PROTOCOL_VERSION) )
          { /* invalid header pattern */
            (void)DoIP_TcpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_FORMAT, 0u, 0u, NULL_PTR, TRUE);
            return BUFREQ_OK;
          }

          DOIP_GET_UINT16(hdrPtr->Type, 0u, hdrType);
          DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

          if ( (hdrType == DOIP_HDR_TYPE_ROUT_ACTIV_REQ) && (DoIP_VRoutActivHandler.TcpCon != DOIP_INV_PDU_ID) )
          { /* routing activation handler is busy */
            /* handle routing activation later */
            *bufferSizePtr                     = 0u;
            DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
            break;
          }

          /* check whether message shall be skipped */
          if ( DoIP_VTcpCon2SkipRxMsg[conIdx] == TRUE )
          { /* message shall be skipped */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
            if ( hdrType == DOIP_HDR_TYPE_DIAG_REQ )
            { /* diagnostic message */
              if ( (DoIP_VTcpCon2TpPreTransMgt[conIdx].HdrType == DOIP_HDR_TYPE_DIAG_NACK) ||
                   (DoIP_VTcpCon2TpPreTransMgt[conIdx].HdrType == DOIP_HDR_TYPE_DIAG_ACK) )
              { /* diagnostic message negative acknowledge shall be sent */
                uint8 testerIdx = DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx;

                if ( testerIdx != DOIP_INV_TESTER_IDX )
                { /* the feature to send previously received user data
                     cannot be used if tester could not be determined */
                  if ( DoIP_VTester[testerIdx].DiagAckNackNumByte > 0u )
                  { /* diagnostic message acknowledge shall contain user data */
                    PduLengthType bytesToCopy = info->SduLength - readDataLen;

                    /* info->SduLength has been checked before against bufferSizePtr of last call to this function */

                    if ( (DoIP_VTester[testerIdx].DiagAckNackNumByte - DoIP_VTester2DiagNAckOffset[testerIdx]) <
                      bytesToCopy )
                    { /* DiagAckNack buffer is smaller than the received diagnostic message user data */
                      bytesToCopy = (PduLengthType)(DoIP_VTester[testerIdx].DiagAckNackNumByte -
                        DoIP_VTester2DiagNAckOffset[testerIdx]);
                    }
                    if ( bytesToCopy > 0u )
                    { /* copy data to DiagNAck buffer */
                      /* PRQA S 0310, 3305 4 */ /* MD_DoIP_11.4 */
                      IpBase_Copy(
                        (P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VTester[testerIdx].DiagAckNackBufPtr[
                          DoIP_VTester2DiagNAckOffset[testerIdx]],
                            (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&info->SduDataPtr[readDataLen],
                            (uint32)bytesToCopy);
                      DoIP_VTester2DiagNAckOffset[testerIdx] += bytesToCopy;
                    }
                  }
                }
              }
            }
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
            if ( (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) - DoIP_VTcpCon2RxMsgOffset[conIdx]) >
              ((PduLengthType)(info->SduLength - readDataLen)) )
            { /* message to be skipped is not received completely */
              DoIP_VTcpCon2RxMsgOffset[conIdx] += info->SduLength - readDataLen;
              readDataLen = (PduLengthType)info->SduLength;
              *bufferSizePtr = (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) - DoIP_VTcpCon2RxMsgOffset[conIdx]);
              DoIP_VTcpCon2LastRxBufSize[conIdx] = *bufferSizePtr;
            }
            else
            { /* message to be skipped is received completely */
              /* finish message */
              DoIP_TcpTx_FinishTransmit(conIdx);

              readDataLen                            = (PduLengthType)info->SduLength;
              *bufferSizePtr                         = 0u;
              DoIP_VTcpCon2VerifyRxPduOffset[conIdx] = 0u;
              DoIP_VTcpCon2LastRxBufSize[conIdx]     = 0u;
              DoIP_VTcpCon2RxMsgOffset[conIdx]       = 0u;
              DoIP_VTcpCon2SkipRxMsg[conIdx]         = FALSE;
            }
          }
          else
          {
            /* further handling depends on message type */
            switch(hdrType)
            {
              case DOIP_HDR_TYPE_ROUT_ACTIV_REQ:
              { /* routing activation request */
                DoIP_TcpRxRout_HandleRoutingActivationReq(conIdx, info, bufferSizePtr, &readDataLen, &errorId);
                break;
              }
              case DOIP_HDR_TYPE_ALIVE_CHECK_RES:
              { /* alive check response */
                DoIP_TcpRxAlive_HandleAliveCheckRes(conIdx, info, bufferSizePtr, &readDataLen);
                break;
              }
              case DOIP_HDR_TYPE_DIAG_REQ:
              { /* diagnostic message */
                DoIP_TcpRxDiag_HandleDiagnosticMessage(conIdx, info, bufferSizePtr, &readDataLen);
                break;
              }
              case DOIP_HDR_TYPE_GEN_NACK:
              case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ:
#if ( DOIP_VVEHICLE_IDENT_EID_SUPPORT == STD_ON )
              case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_EID:
#endif /* DOIP_VVEHICLE_IDENT_EID_SUPPORT == STD_ON */
              case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN:
              case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
              case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
              case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
              case DOIP_HDR_TYPE_ENTITY_STATE_REQ:
              case DOIP_HDR_TYPE_ENTITY_STATE_RES:
              case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_REQ:
              case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
              case DOIP_HDR_TYPE_DIAG_ACK:
              case DOIP_HDR_TYPE_DIAG_NACK:
              { /* not relevant payload type */
                DoIP_VTcpCon2SkipRxMsg[conIdx] = TRUE;
                break;
              }
              default:
              { /* unknown payload type */
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
                DoIP_VRxHandleOemPayloadTypes(conIdx, info, bufferSizePtr, &readDataLen);
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */
                break;
              }
            }
          }

          if ( DoIP_VTcpCon2SkipRxMsg[conIdx] == TRUE )
          { /* message shall be skipped */
            /* set buffer size to receive entire message in next function call */
            *bufferSizePtr = (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) - DoIP_VTcpCon2RxMsgOffset[conIdx]);
            DoIP_VTcpCon2LastRxBufSize[conIdx] = *bufferSizePtr;
          }
        }
        else
        { /* generic header was not received completely */
          *bufferSizePtr = DOIP_GEN_HDR_LEN_BYTE - DoIP_VTcpCon2RxMsgOffset[conIdx];
          DoIP_VTcpCon2LastRxBufSize[conIdx] = *bufferSizePtr;
        }
      } while ( info->SduLength > readDataLen );

      if ( DoIP_VTcpCon2RxMsgOffset[conIdx] == 0u )
      { /* message processed completely */
#if ( DOIP_VTCP_RX_MULTI_MSG == STD_ON )
# if ( DOIP_NACK_APPEND_REQ == STD_ON )
        if ( DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx != DOIP_INV_TESTER_IDX )
        { /* the feature to send previously received user data cannot be used if tester could not be determined */
          if ( DoIP_VTester2DiagNAckOffset[DoIP_VTcpCon2ActivMgt[conIdx].TesterIdx] > 0u )
          { /* diagnostic message acknowledge is pending - do not accept new data until acknowledge is sent */
            *bufferSizePtr                     = 0u;
            DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
          }
        }
        else
# endif /* DOIP_NACK_APPEND_REQ == STD_ON */
        {
          *bufferSizePtr                     = DOIP_GEN_HDR_LEN_BYTE;
          DoIP_VTcpCon2LastRxBufSize[conIdx] = DOIP_GEN_HDR_LEN_BYTE;
        }
#else
        *bufferSizePtr                     = 0u;
        DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
#endif /* DOIP_VTCP_RX_MULTI_MSG == STD_ON */
      }
      else if ( DoIP_VTcpTxQueueGetLevel(conIdx) >= (DOIP_VTCP_TX_MGT_QUEUE_SIZE - 1u) )
      { /* high tx queue fill level - slow down rx path */
        *bufferSizePtr                     = 0u;
        DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
      }
      else if ( (DoIP_VTcpCon2ActivMgt[conIdx].ActivState == DOIP_ROUT_ACTIV_STATE_AUTH_PENDING) ||
        (DoIP_VTcpCon2ActivMgt[conIdx].ActivState == DOIP_ROUT_ACTIV_STATE_CONF_PENDING) )
      { /* activation state is pending */
        /* stop reception until state changes */
        *bufferSizePtr                     = 0u;
        DoIP_VTcpCon2LastRxBufSize[conIdx] = 0u;
      }
      else
      {
        /* nothing to do here */
      }

      retVal = BUFREQ_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_COPY_RX_DATA, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpCopyRxData() */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdTpStartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 3673 DoIP_16_7 */ /* MD_DoIP_16.7 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 5 */ /* MD_DoIP_3206 */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduIdType conIdx = id; /* connection index is equal to id */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( conIdx >= DOIP_VCONNECTION_TCP_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'bufferSizePtr' */
  else if ( bufferSizePtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( TpSduLength != 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT_VAR(TpSduLength);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(bufferSizePtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DOIP_DUMMY_STATEMENT_VAR(info); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */

#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    /* check socket connection state */
    if ( DoIP_VCon2SoConState[conIdx] != SOAD_SOCON_ONLINE )
    { /* socket connection is not online */
      /* nothing to do here */
    }
    else if ( DoIP_VTcpCon2RxTpSessionActive[conIdx] == TRUE )
    { /* TP Routing is already active */
      /* nothing to do here */
    }
    else
    { /* TP Routing is not active */
      /* activate TP session */
      DoIP_VTcpCon2RxTpSessionActive[conIdx] = TRUE;
      retVal = BUFREQ_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_START_OF_RECEPTION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpStartOfReception() */
/* PRQA L: DoIP_16_7 */
/* PRQA L: MSR_14_7 */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdTpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_SoAdTpRxIndication(
  PduIdType id,
  Std_ReturnType result)
{ /* will be called when socket connection shall be closed */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType conIdx = id; /* connection index is equal to id */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( conIdx >= DOIP_VCONNECTION_TCP_TOTAL )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'result' */
  else if ( (result != NTFRSLT_OK) &&
            (result != NTFRSLT_E_NOT_OK) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT_VAR(result); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      /* TP session is now offline */
      DoIP_VTcpCon2RxTpSessionActive[conIdx] = FALSE;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_RX_INDICATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdTpRxIndication() */

/**********************************************************************************************************************
 *  DoIP_SoAdIfRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 3333 DoIP_14_6 */ /* MD_DoIP_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoAdIfRxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType conIdx = RxPduId; /* connection index is equal to RxPduId */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( (conIdx <  DOIP_VCONNECTION_TCP_TOTAL) ||
            (conIdx >= DOIP_VCONNECTION_TCP_UDP_TOTAL) )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'PduInfoPtr' */
  else if ( PduInfoPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'PduInfoPtr' */
  else if ( PduInfoPtr->SduDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    if ( DoIP_VCon2IpAddrState[conIdx] != SOAD_IPADDR_STATE_ASSIGNED )
    { /* no IP is assigned */
      /* nothing to do here */
    }
    else if ( (DoIP_VCon2SoConState[conIdx] != SOAD_SOCON_ONLINE) || (DoIP_VCon2SoConClosing[conIdx] == TRUE) )
    { /* socket connection is not online or requested to be closed */
      /* nothing to do here */
    }
    else
    {
      boolean closeSocket = FALSE;
      uint8 srcHdrVer;
      uint8 srcHdrInvVer;
      uint32 srcHdrLen;
      uint16 srcHdrType;
      uint32 dataOffset = 0u;
      uint8 reqCnt = 0u;

      while ( PduInfoPtr->SduLength > dataOffset )
      {
        if ( (PduInfoPtr->SduLength - dataOffset) < DOIP_GEN_HDR_LEN_BYTE )
        { /* message too short */
          DoIP_Con_SoConClose(conIdx);
          break;
        }

        /* extract generic header */
        DOIP_GET_UINT8 (PduInfoPtr->SduDataPtr, (dataOffset), srcHdrVer);
        DOIP_GET_UINT8 (PduInfoPtr->SduDataPtr, (dataOffset + 1u), srcHdrInvVer);
        DOIP_GET_UINT16(PduInfoPtr->SduDataPtr, (dataOffset + 2u), srcHdrType);
        DOIP_GET_UINT32(PduInfoPtr->SduDataPtr, (dataOffset + 4u), srcHdrLen);

        if ( ((uint8)srcHdrVer != DOIP_PROTOCOL_VERSION) || ((uint8)(~srcHdrInvVer) != DOIP_PROTOCOL_VERSION) )
        { /* header pattern does not match configured DoIP version */
          /* check whether default protocol version is used and valid here */
          if ( ((uint8)srcHdrVer != DOIP_PROTOCOL_VERSION_DEFAULT) ||
            ((uint8)(~srcHdrInvVer) != DOIP_PROTOCOL_VERSION_DEFAULT) ||
            (srcHdrType < DOIP_HDR_TYPE_VEHICLE_IDENT_REQ) ||
            (srcHdrType > DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN) )
          { /* invalid header pattern */
            (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_FORMAT, TRUE);
            return;
          }
        }
        if ( (DOIP_GEN_HDR_LEN_BYTE + srcHdrLen) > (PduInfoPtr->SduLength - dataOffset) )
        { /* invalid header length relative to received data length */
          (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, TRUE);
          return;
        }

        reqCnt++;
        if ( reqCnt > DOIP_VMAX_UDP_REQ_PER_MSG )
        { /* maximum requests per message exceeded */
          if ( (DOIP_GEN_HDR_LEN_BYTE + srcHdrLen) == (PduInfoPtr->SduLength - dataOffset) )
          { /* last message in frame */
            if ( DoIP_VLocalAddrIdx2HasUdpAliveTimeout[DoIP_VCon2LocalAddrIdx[conIdx]] == FALSE )
            { /* socket connection has to be closed manually since no UDP alive timeout is configured */
              /* close socket connection after sending */
              (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, TRUE);
            }
            else
            {
              (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, FALSE);
            }
          }
          else
          {
            (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, FALSE);
          }
        }
        else
        {
          if ( (DOIP_GEN_HDR_LEN_BYTE + srcHdrLen) == (PduInfoPtr->SduLength - dataOffset) )
          { /* last message in frame */
            if ( DoIP_VLocalAddrIdx2HasUdpAliveTimeout[DoIP_VCon2LocalAddrIdx[conIdx]] == FALSE )
            { /* socket connection has to be closed manually since no UDP alive timeout is configured */
              /* close socket connection after sending */
              closeSocket = TRUE;
            }
          }

          /* further handling depends on message type */
          switch(srcHdrType)
          {
            case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ:
            { /* vehicle identification request */
              if ( srcHdrLen > DOIP_MAX_REQUEST_BYTES )
              { /* max length exceeded */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, closeSocket);
                break;
              }
              if ( srcHdrLen != 0u )
              { /* length check failed */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, closeSocket);
              }
              else
              {
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM,
                  closeSocket);
              }
              break;
            }
            case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN:
            { /* vehicle identification with VIN (check for our VIN) */
              if ( srcHdrLen > DOIP_MAX_REQUEST_BYTES )
              { /* max length exceeded */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, closeSocket);
                break;
              }
              if ( srcHdrLen != DOIP_VIN_LEN_BYTE )
              { /* length check failed */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, closeSocket);
              }
              else
              { /* length check ok */
                boolean IdentOk = TRUE;
                uint8 IdentIdx = DOIP_VIN_LEN_BYTE;
                uint8 vin[DOIP_VIN_LEN_BYTE] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};

                if ( DoIP_VCallGetVinDirect(vin) == E_OK )
                { /* VIN successfully retrieved */
                  /* compare to retrieved VIN */
                  while ( IdentIdx > 0u )
                  {
                    IdentIdx--;
                    if ( PduInfoPtr->SduDataPtr[dataOffset + DOIP_GEN_HDR_LEN_BYTE + IdentIdx] != vin[IdentIdx] )
                    {
                      IdentOk = FALSE;
                      break;
                    }
                  }
                }
                else
                { /* VIN request failed */
                  /* compare to invaliditiy pattern */
                  while ( IdentIdx > 0u )
                  {
                    IdentIdx--;
                    if ( PduInfoPtr->SduDataPtr[dataOffset + DOIP_GEN_HDR_LEN_BYTE + IdentIdx] !=
                          DOIP_VVIN_INV_PATTERN )
                    {
                      IdentOk = FALSE;
                      break;
                    }
                  }
                }

                if ( IdentOk == TRUE )
                {
                  (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM,
                    closeSocket);
                }
              }
              break;
            }
#if ( DOIP_VVEHICLE_IDENT_EID_SUPPORT == STD_ON )
            case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_EID:
            { /* vehicle identification with entity id (check for our physical address (MAC address)) */
              if ( srcHdrLen > DOIP_MAX_REQUEST_BYTES )
              { /* max length exceeded */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, closeSocket);
                break;
              }
              if ( srcHdrLen != DOIP_UHWID_LEN_BYTE )
              { /* length check failed */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, closeSocket);
              }
              else
              { /* length check ok */
                boolean identOk  = TRUE;
                uint8   identIdx = DOIP_UHWID_LEN_BYTE;

# if ( DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON )
                uint8   physAddrBuf[DOIP_UHWID_LEN_BYTE];

                if ( SoAd_GetPhysAddr(DoIP_VCon2SoConId[conIdx], &physAddrBuf[0u]) == E_OK )
                {
                  while ( identIdx > 0u )
                  {
                    identIdx--;
                    if ( PduInfoPtr->SduDataPtr[dataOffset + DOIP_GEN_HDR_LEN_BYTE + identIdx] !=
                          physAddrBuf[identIdx] )
                    {
                      identOk = FALSE;
                      break;
                    }
                  }
                }
                else
                {
                  identOk = FALSE;
                }
# else
                while ( identIdx > 0u )
                {
                  identIdx--;
                  if ( PduInfoPtr->SduDataPtr[dataOffset + DOIP_GEN_HDR_LEN_BYTE + identIdx] !=
                    (uint8)(DoIP_VEid[identIdx]) )
                  {
                    identOk = FALSE;
                    break;
                  }
                }
# endif /* DOIP_VUSE_MAC_ADDR_FOR_IDENT == STD_ON */

                if ( identOk == TRUE )
                {
                  (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM,
                    closeSocket);
                }
              }
              break;
            }
#endif /* DOIP_VVEHICLE_IDENT_EID_SUPPORT == STD_ON */
            case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_REQ:
            { /* diagnostic power mode request */
              if ( srcHdrLen > DOIP_MAX_REQUEST_BYTES )
              { /* max length exceeded */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, closeSocket);
                break;
              }
              if ( srcHdrLen != 0u )
              { /* length check failed */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, closeSocket);
              }
              else
              { /* length check ok */
                DoIP_PowerModeType diagPowerMode = DOIP_POWER_MODE_NOT_SUPPORTED;

                if ( DoIP_VCallPowerModeDirect(&diagPowerMode) == E_NOT_OK )
                {
                  diagPowerMode = DOIP_POWER_MODE_NOT_READY;
                }
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES, diagPowerMode,
                  closeSocket);
              }
              break;
            }
#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
            case DOIP_HDR_TYPE_ENTITY_STATE_REQ:
            { /* entity status request */
              if ( srcHdrLen > DOIP_MAX_REQUEST_BYTES )
              { /* max length exceeded */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, closeSocket);
                break;
              }
              if ( srcHdrLen != 0u )
              { /* length check failed */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, closeSocket);
              }
              else
              { /* length check ok */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_ENTITY_STATE_RES, DOIP_UNUSED_PARAM, closeSocket);
              }
              break;
            }
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */
            case DOIP_HDR_TYPE_GEN_NACK:
            case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
            case DOIP_HDR_TYPE_ROUT_ACTIV_REQ:
            case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
            case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
            case DOIP_HDR_TYPE_ALIVE_CHECK_RES:
            case DOIP_HDR_TYPE_ENTITY_STATE_RES:
            case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
            case DOIP_HDR_TYPE_DIAG_REQ:
            case DOIP_HDR_TYPE_DIAG_ACK:
            case DOIP_HDR_TYPE_DIAG_NACK:
            { /* ignore not relevant header type */
              break;
            }
            default:
            { /* TxNack: unknown header type */
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
              PduInfoType rxUserData;

              rxUserData.SduLength = (PduLengthType)srcHdrLen;
              if ( srcHdrLen == 0u )
              {
                rxUserData.SduDataPtr = NULL_PTR;
              }
              else
              {
                rxUserData.SduDataPtr = &PduInfoPtr->SduDataPtr[dataOffset + DOIP_GEN_HDR_LEN_BYTE];
              }

              /* check if tx buffer is available */
              if ( DoIP_VOemPayloadTypeUdpBufLen == 0u )
              { /* tx buffer is available */
                uint16 txHdrType = DOIP_HDR_TYPE_INVALID;
                PduInfoType txUserData;

                txUserData.SduDataPtr = &DoIP_VOemPayloadTypeUdpBuf[DOIP_GEN_HDR_LEN_BYTE];
                txUserData.SduLength = DOIP_VOEM_PAYLOAD_TYPE_UDP_BUF_SIZE;

                /* call user callback to forward message to upper layer */
                if ( DoIP_VCallOemPayloadTypeDirect(srcHdrType, &rxUserData, DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_UDP,
                    &txHdrType, &txUserData) == E_NOT_OK )
                {
                  (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, closeSocket);
                }
                else
                {
                  if ( txHdrType != DOIP_HDR_TYPE_INVALID )
                  {
                    if ( txUserData.SduLength <= DOIP_VOEM_PAYLOAD_TYPE_UDP_BUF_SIZE )
                    {
                      DoIP_VOemPayloadTypeUdpBufLen = (uint16)(txUserData.SduLength + DOIP_GEN_HDR_LEN_BYTE);
                      (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, txHdrType, closeSocket);
                    }
                  }
                }
              }
              else
              { /* no tx buffer is available */
                /* send out of memory */
                (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, closeSocket);
              }
#else
              (void)DoIP_UdpTx_Transmit(conIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, closeSocket);
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */
            }
          }
        }

        dataOffset += DOIP_GEN_HDR_LEN_BYTE + srcHdrLen;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_RX_INDICATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdIfRxIndication() */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_14_7 */
/* PRQA L:DoIP_14_6 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_SoAdIfTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation(
  PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType conIdx = TxPduId; /* connection index is equal to TxPduId */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'conIdx' */
  else if ( (conIdx >= DOIP_VCONNECTION_TOTAL) ||
            (conIdx <  DOIP_VCONNECTION_TCP_TOTAL) )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#else
  DOIP_DUMMY_STATEMENT_VAR(TxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* nothing to do here */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_TX_CONFIRMATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdIfTxConfirmation() */

/**********************************************************************************************************************
 *  DoIP_SoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType connection;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check 'SoConId' */
  else if ( DoIP_Con_GetConIdxBySoConId(SoConId, &connection) != E_OK )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'Mode' */
  else if ( (Mode != SOAD_SOCON_ONLINE) &&
            (Mode != SOAD_SOCON_RECONNECT) &&
            (Mode != SOAD_SOCON_OFFLINE) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  /* Check 'SoConId' */
  if ( DoIP_Con_GetConIdxBySoConId(SoConId, &connection) == E_OK )
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      if ( Mode != SOAD_SOCON_ONLINE )
      { /* terminate all transmission requests */
        DoIP_VCon2SoConState[connection] = Mode;
        DoIP_VCon2SoConClosing[connection] = FALSE;

        if ( connection < DOIP_VCONNECTION_TCP_TOTAL )
        { /* TCP connection */
          uint8 testerIdx = DoIP_VTcpCon2ActivMgt[connection].TesterIdx;

          /* reset socket connection specific structs */
          if ( testerIdx != DOIP_INV_TESTER_IDX )
          {
            DoIP_VTester2TcpCon[testerIdx] = DOIP_INV_PDU_ID;
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
            DoIP_VTester2DiagNAckOffset[testerIdx] = 0u;
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
          }
          DoIP_VTcpCon2ActivMgt[connection].TesterIdx    = DOIP_INV_TESTER_IDX;
          DoIP_VTcpCon2ActivMgt[connection].TesterAddr   = DOIP_ADDR_ANY;
          DoIP_VTcpCon2ActivMgt[connection].RoutActIdx   = DOIP_INV_ROUT_ACT_IDX;
          DoIP_VTcpCon2ActivMgt[connection].ActivState   = DOIP_ROUT_ACTIV_STATE_NONE;
          DoIP_VTcpCon2InactivCnt[connection]            = 0u;
          DoIP_VTcpCon2RxMsgOffset[connection]           = 0u;
          DoIP_VTcpCon2TxBytesCopied[connection]         = 0u;
          DoIP_VTcpCon2TxTpSessionActive[connection]     = FALSE;
          DoIP_VTcpCon2RxTpSessionActive[connection]     = FALSE;
          DoIP_VTcpCon2SkipRxMsg[connection]             = FALSE;
          DoIP_VTcpCon2TpPreTransMgt[connection].HdrType = DOIP_HDR_TYPE_INVALID;
#if ( DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON )
          DoIP_VTcpCon2TxTpCancelRequested[connection]   = FALSE;
#endif /* DOIP_VPROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#if ( DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON )
          DoIP_VTcpCon2RxTpCancelRequested[connection]   = FALSE;
#endif /* DOIP_VPROVIDE_CANCEL_RECEIVE_API == STD_ON */
          DoIP_VTcpCon2VerifyRxPduOffset[connection]     = 0u;

          /* alive check ongoing */
          if ( DoIP_VRoutActivHandler.TcpCon != DOIP_INV_PDU_ID )
          { /* Routing Activation active */
            if ( DoIP_VRoutActivHandler.AliveCheckElemNum > 0u )
            {
              uint8 listIdx = DOIP_VMAX_CONS_PER_LOC_ADDR;

              while ( listIdx > 0u )
              {
                listIdx--;
                if ( connection == DoIP_VRoutActivHandler.AliveCheckList[listIdx] )
                { /* alive check pending */
                  DoIP_VRoutActivHandler.AliveCheckList[listIdx] = DOIP_INV_PDU_ID;
                  DoIP_VRoutActivHandler.AliveCheckElemNum--;
                }
              }
            }
            if ( (DoIP_VRoutActivHandler.AliveCheckElemNum == 0u) &&
              (DoIP_VCon2SoConState[DoIP_VRoutActivHandler.TcpCon] != SOAD_SOCON_ONLINE) )
            { /* all routing activation relevant socket connections closed or alive check received */
              /* release handler */
              DoIP_VRoutActivHandler.TcpCon = DOIP_INV_PDU_ID;
            }
          }

          /* PRQA S 0277 1 */ /* MD_DoIP_3.1 */
          if ( DoIP_VTcpCon2RxChannelActive[connection] != DOIP_INV_CHANNEL )
          { /* reception is ongoing */
            PduR_DoIPTpRxIndication(DoIP_VChannel2PduRRxPduId[DoIP_VTcpCon2RxChannelActive[connection]],
              E_NOT_OK);
            DoIP_VTcpCon2RxChannelActive[connection] = DOIP_INV_CHANNEL;
          }

          /* reset Tx Queue */
          while ( DoIP_VTcpTxQueueGetLevel(connection) > 0u )
          {
            DoIP_TcpTxQueue_RemoveElement(connection, E_NOT_OK);
          }
        }
        else
        { /* UDP connection */
          uint16 idx = DOIP_VUDP_TX_MGT_LIST_SIZE;

          /* reset Tx list */
          while ( idx > 0u )
          {
            idx--;
            if ( DoIP_VUdpTxMgtList[idx].UdpCon == connection )
            {
#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
              if ( DoIP_VUdpTxMgtList[idx].HdrType == DOIP_HDR_TYPE_OEM_SPECIFIC )
              {
                DoIP_VOemPayloadTypeUdpBufLen = 0u;
              }
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

              DoIP_VUdpTxMgtList[idx].UdpCon = DOIP_INV_PDU_ID;
            }
          }
        }
      }
      else
      { /* socket connection is online */
        if ( DoIP_VCon2IpAddrState[connection] == SOAD_IPADDR_STATE_ASSIGNED )
        { /* IP address assigned */
          DoIP_VCon2SoConState[connection] = Mode;
          DoIP_VCon2SoConClosing[connection] = FALSE;

          if ( connection >= DOIP_VCONNECTION_TCP_UDP_TOTAL )
          { /* UDP Vehicle Announcement connection */
            /* trigger Vehicle Announcements */
            /* list Vehicle Announcement in Tx list */
            if ( DoIP_UdpTx_Transmit(connection, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_VANNOUNCE_NUM,
                FALSE) != E_OK )
            { /* buffer overflow after socket connection setup should not happen */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
              errorId = DOIP_E_NOBUFS;
#else
              /* nothing to do here */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
            }
          }
#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
          if ( connection < DOIP_VCONNECTION_TCP_TOTAL )
          {
            uint8 netmask = 0u;
            SoAd_SockAddrInetXType defaultRouter = {0u};

            /* save ip addresses */
            /* PRQA S 0310 7 */ /* MD_DoIP_11.4 */
            (void)SoAd_GetRemoteAddr(SoConId,
              (P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR))
                  (&(DoIP_VTcpCon2RemLocIpAddr[connection].RemIpAddr)));
            (void)SoAd_GetLocalAddr (SoConId,
              (P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR))
                  (&(DoIP_VTcpCon2RemLocIpAddr[connection].LocIpAddr)),
              &netmask, (P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR))(&defaultRouter));
          }
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
        }
        else
        {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
          errorId = DOIP_E_UNEXPECTED_ASSIGNMENT;
#else
          /* nothing to do here */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SO_CON_MODE_CHG, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoConModeChg() */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  PduIdType connection;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check 'State' */
  else if ( (State != SOAD_IPADDR_STATE_ASSIGNED) &&
            (State != SOAD_IPADDR_STATE_ONHOLD) &&
            (State != SOAD_IPADDR_STATE_UNASSIGNED) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'SoConId' */
  else if ( DoIP_Con_GetConIdxBySoConId(SoConId, &connection) != E_OK )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  /* Check 'SoConId' */
  if ( DoIP_Con_GetConIdxBySoConId(SoConId, &connection) == E_OK )
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
#endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      uint8 addrIdx = DoIP_VCon2LocalAddrIdx[connection];

      switch(DoIP_ActivationLineLocalState)
      {
        case DOIP_ACTIVATION_LINE_LOCAL_ACTIVE:
        {
          DoIP_VCon2IpAddrState[connection] = State;
          if ( State == SOAD_IPADDR_STATE_UNASSIGNED )
          {
            DoIP_VLocalAddrIdx2IpAddrReqState[addrIdx] = DOIP_IP_ADDR_REQ_STATE_NONE;
            /* request socket connection close to reset open/close sequence in SoAd */
            DoIP_Con_SoConClose(connection);
          }
          else if ( State == SOAD_IPADDR_STATE_ASSIGNED )
          {
            DoIP_VLocalAddrIdx2IpAddrReqState[addrIdx] = DOIP_IP_ADDR_REQ_STATE_ASSIGNED;
            /* open socket connections initially after IP address reassignment */
            DoIP_Con_SoConOpen(connection);
          }
          else
          {
            /* nothing to do here */
          }
          break;
        }
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
        case DOIP_ACTIVATION_LINE_LOCAL_ACTIVE_TO_INACTIVE:
        {
          DoIP_VCon2IpAddrState[connection] = State;
          if ( State == SOAD_IPADDR_STATE_UNASSIGNED )
          { /* IP address not assigned */
            /* IpAddrReqState is set multiple times (for each SoConId) */
            DoIP_VLocalAddrIdx2IpAddrReqState[addrIdx] = DOIP_IP_ADDR_REQ_STATE_NONE;
          }
          break;
        }
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
        default:
        { /* invalid state */
          errorId = DOIP_E_UNEXPECTED_ASSIGNMENT;
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_LOC_IP_ADDR_ASSIGN_CHG, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_LocalIpAddrAssignmentChg() */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  DoIP_ActivationLineSwitch()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitch(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
  uint8 errorId = DOIP_E_NO_ERROR;
#else
  uint8 errorId = DOIP_E_ACTIVATION_LINE; /* activation line switch not supported since callback is not configured */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
# if ( DOIP_VSUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state of the component */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* nothing to do here */
    }
    else
# endif /* DOIP_VSUPPORT_SHUTDOWN == STD_ON */
    {
      DoIP_ActivationLineType state;

      DoIP_VCallActivationLineStateDirect(&state);
      if ( state == DOIP_ACTIVATION_LINE_INACTIVE )
      { /* switch to inactive */
        DoIP_ActivationLineState = DOIP_ACTIVATION_LINE_INACTIVE;
      }
      else
      { /* switch to active */
        DoIP_ActivationLineState = DOIP_ACTIVATION_LINE_ACTIVE;
      }
    }
#else
    /* nothing to do here */
#endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# if ( DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
# endif /* DOIP_VACTIV_LINE_CALLBACK_ENABLED == STD_ON */
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_ACTIVATION_LINE_SWITCH, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_ActivationLineSwitch() */
/* PRQA L:MSR_14_7 */

/**********************************************************************************************************************
 *  DoIP_ShutdownFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_ShutdownFinished(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
  DoIP_State = DOIP_STATE_SHUTDOWN;
  DoIP_VCallShutdownFinished();
#else
  /* API disabled */
  /* nothing to do here */
#endif /* DOIP_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
} /* DoIP_ShutdownFinished() */

#if ( DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VRxHandleOemPayloadTypes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 5 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VRxHandleOemPayloadTypes(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  uint16 txHdrType = DOIP_HDR_TYPE_INVALID;
# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
  PduLengthType bytesToCopy;
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u */
  PduInfoType rxUserData;
  PduInfoType txUserData;
  uint16 hdrType;
  uint32 hdrLen;
  uint8 flags;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);
  DOIP_GET_UINT16(hdrPtr->Type, 0u, hdrType);

  /* check length */
  if ( hdrLen > DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE )
  { /* unsupported header length */
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR, FALSE);
    return;
  }

  /* set flags for callback */
  flags = DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP;
  if ( DoIP_VTcpCon2ActivMgt[ConIdx].ActivState == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
  {
    flags |= DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV;
  }

# if ( DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u )
  /* check buffer availability */
  if ( DoIP_VOemPayloadTypeTcpBuf2Con != DOIP_INV_PDU_ID )
  { /* buffer is already used */
    if ( (DoIP_VOemPayloadTypeTcpBuf2Con != ConIdx) || (DoIP_VOemPayloadTypeTcpBufIsTx == TRUE) )
    { /* buffer is used by another connection or buffer is used for transmission */
      /* wait until buffer is free again */
      *BufferSizePtr = 0u;
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
      return;
    }
  }
  else
  { /* buffer is not used */
    /* occupy buffer */
    DoIP_VOemPayloadTypeTcpBuf2Con = ConIdx;
  }

  /* copy to OEM specific payload type user data buffer */
  bytesToCopy = (PduLengthType)((hdrLen + DOIP_GEN_HDR_LEN_BYTE) - DoIP_VTcpCon2RxMsgOffset[ConIdx]);
  if ( bytesToCopy > ((PduLengthType)(PduInfoPtr->SduLength - *ReadDataLenPtr)) )
  {
    bytesToCopy = (PduLengthType)(PduInfoPtr->SduLength - *ReadDataLenPtr);
  }

  if ( bytesToCopy > 0u )
  {
    /* copy data */
    /* PRQA S 0310, 3305 3 */ /* MD_DoIP_11.4 */
    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&DoIP_VOemPayloadTypeTcpBuf[
      DoIP_VTcpCon2RxMsgOffset[ConIdx] - DOIP_GEN_HDR_LEN_BYTE],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&PduInfoPtr->SduDataPtr[*ReadDataLenPtr],
      (uint32)bytesToCopy);
    *ReadDataLenPtr                   += bytesToCopy;
    DoIP_VTcpCon2RxMsgOffset[ConIdx] += bytesToCopy;
  }

  /* check whether message is now copied completely */
  if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] == (hdrLen + DOIP_GEN_HDR_LEN_BYTE) )
  {
    rxUserData.SduDataPtr = DoIP_VOemPayloadTypeTcpBuf;
    rxUserData.SduLength  = (PduLengthType)(DoIP_VTcpCon2RxMsgOffset[ConIdx] - DOIP_GEN_HDR_LEN_BYTE);

    /* use rx buffer as tx buffer to save RAM */
    txUserData.SduDataPtr = DoIP_VOemPayloadTypeTcpBuf;
    txUserData.SduLength  = DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE;

    /* call user callback to forward message to upper layer */
    if ( DoIP_VCallOemPayloadTypeDirect(hdrType, &rxUserData, flags, &txHdrType, &txUserData) == E_NOT_OK )
    { /* unknown payload type */
      (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, 0u, 0u, NULL_PTR, FALSE);

      DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
    }
    else
    { /* payload type known by user */
      if ( txHdrType != DOIP_HDR_TYPE_INVALID )
      { /* send response */
        if ( txUserData.SduLength <= DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE )
        { /* response has valid length */
          DoIP_VOemPayloadTypeTcpBufLen = (uint16)txUserData.SduLength;
          if ( txUserData.SduLength > 0u )
          {
            DoIP_VOemPayloadTypeTcpBufIsTx = TRUE;
          }
          /* send response */
          if ( DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, txHdrType, 0u, 0u, NULL_PTR, FALSE) ==
              E_NOT_OK)
          { /* response could not be sent */
            DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
          }
        }
        else
        { /* response has invalid length - ignore response */
          DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
        }
      }
      else
      { /* no response */
        DoIP_VOemPayloadTypeTcpBuf2Con = DOIP_INV_PDU_ID;
      }
    }

    /* finish message */
    DoIP_VTcpCon2RxMsgOffset[ConIdx] = 0u;
  }
  else
  {
    *BufferSizePtr = (PduLengthType)(hdrLen - (DOIP_GEN_HDR_LEN_BYTE - DoIP_VTcpCon2RxMsgOffset[ConIdx]));
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
  }
# else
  DOIP_DUMMY_STATEMENT_CONST_VAR(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(BufferSizePtr);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(ReadDataLenPtr);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */

  rxUserData.SduDataPtr = NULL_PTR;
  rxUserData.SduLength  = 0u;

  txUserData.SduDataPtr = NULL_PTR;
  txUserData.SduLength  = 0u;

  /* call user callback to forward message to upper layer */
  if ( DoIP_VCallOemPayloadTypeDirect(hdrType, &rxUserData, flags, &txHdrType, &txUserData) == E_NOT_OK )
  { /* unknown payload type */
    (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, 0u, 0u, NULL_PTR, FALSE);
  }
  else
  { /* payload type known by user */
    if ( txHdrType != DOIP_HDR_TYPE_INVALID )
    { /* send response */
      (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, txHdrType, 0u, 0u, NULL_PTR, FALSE);
    }
  }

  /* finish message */
  DoIP_VTcpCon2RxMsgOffset[ConIdx] = 0u;
# endif /* DOIP_VOEM_PAYLOAD_TYPE_TCP_BUF_SIZE > 0u */
} /* DoIP_VRxHandleOemPayloadTypes() */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STPAR */
/* PRQA L:MSR_STMIF */
#endif /* DOIP_VOEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgVerifyRxPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 6 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgVerifyRxPdu(
  PduIdType ConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2VAR(DoIP_ChannelIdType, AUTOMATIC, DOIP_APPL_VAR) Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  Std_ReturnType retVal = E_NOT_OK;
#if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
  PduLengthType bytesToCopy;
#endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

#if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
  if ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] >= (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)) &&
       (DoIP_VTcpCon2RxMsgOffset[ConIdx] <  (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) &&
       (DoIP_VTcpCon2RxMsgOffset[ConIdx] <
         (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)) )
  {
    bytesToCopy = DOIP_MIN(
      (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)
          - DoIP_VTcpCon2RxMsgOffset[ConIdx]),
      (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) - DoIP_VTcpCon2RxMsgOffset[ConIdx]));

    DoIP_TcpRxGeneric_CopyDataToHeaderBuffer(ConIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);
#else
  DOIP_DUMMY_STATEMENT_CONST_VAR(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT_VAR(ReadDataLenPtr);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */

  if ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] == (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)) &&
       (DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] == 0u) )
  {
    DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] = 1u;
#endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */

#if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
    /* check whether message header was copied completely */
    if ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] == (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) ||
         (DoIP_VTcpCon2RxMsgOffset[ConIdx] ==
           (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)) )
#endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
    { /* message header copied completely */

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
      PduInfoType *pduInfoPtr;
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
      uint32 diagMsgLength = (uint32)(hdrLen - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE);
      uint8 routActIdx = DoIP_VTcpCon2ActivMgt[ConIdx].RoutActIdx;
      uint16 srcAddr;
      uint16 tgtAddr;
      /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
      P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_APPL_VAR) diagReqPtr =
        (P2VAR(DoIP_DiagReqType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][
        DOIP_GEN_HDR_LEN_BYTE];
#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
# if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
      PduInfoType pduInfo;

      pduInfo.SduDataPtr =
        &(DoIP_VTcpCon2RxHdrBuf[ConIdx][(DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]);
      pduInfo.SduLength =
        (PduLengthType)DOIP_MIN(DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH, (hdrLen - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE));
      pduInfoPtr = &pduInfo;
# else
      pduInfoPtr = NULL_PTR;
# endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

      DOIP_GET_UINT16(diagReqPtr->SrcAddr, 0u, srcAddr);
      DOIP_GET_UINT16(diagReqPtr->TgtAddr, 0u, tgtAddr);

      if ( *Channel == DOIP_INV_CHANNEL )
      {
        *Channel = DoIP_VGetChannelByTgtAddrForRoutAct(routActIdx, tgtAddr, diagMsgLength);
      }

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
      /* get pdu data to verify */
      /* verify pdu */
      /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
      if ( DoIP_VCallVerifyRxPduDirect(
            (SoAd_SockAddrType*)&(DoIP_VTcpCon2RemLocIpAddr[ConIdx].LocIpAddr),
            (SoAd_SockAddrType*)&(DoIP_VTcpCon2RemLocIpAddr[ConIdx].RemIpAddr),
            srcAddr, tgtAddr, pduInfoPtr) == E_OK )
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
      { /* rx pdu accept */
        /* call start of reception at PduR */
        if ( PduR_DoIPTpStartOfReception(DoIP_VChannel2PduRRxPduId[*Channel], NULL_PTR,
          (PduLengthType)(hdrLen - DOIP_DIAG_HDR_LEN_BYTE), BufferSizePtr) != BUFREQ_OK )
        { /* no buffer obtained */
          DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
            DOIP_DIAG_MSG_RES_CODE_OUT_OF_MEMORY, FALSE);
          return E_NOT_OK;
        }
        else
        { /* start of reception successful */
          PduInfoType pduInfoLocal;
          pduInfoLocal.SduLength = 0u;

          /* set active channel for this socket connection */
          DoIP_VTcpCon2RxChannelActive[ConIdx] = *Channel;
          DoIP_VTcpCon2LastRxTgtAddr[ConIdx] = tgtAddr;

          retVal = DoIP_VRxDiagMsgCopyDataToPduR(ConIdx, BufferSizePtr, &pduInfoLocal);

          if ( retVal == E_OK )
          {
            DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;

#if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH == 0u )
            return DOIP_E_PENDING;
#endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH == 0u */
          }

          return retVal;
        }
      }
#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
      else
      { /* rx pdu discard */
        /* sends a diagnostic message positive acknowledge back to tester and rejects the diagnostic message */
        /* send diagnostic message positive acknowledge */
        DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_ACK, tgtAddr, 0u, 0u, FALSE);
        return E_NOT_OK;
      }
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */
    }
#if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
    else
    { /* set bufferSize to length of pending message part */
      *BufferSizePtr = DOIP_MIN(
        (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)
            - DoIP_VTcpCon2RxMsgOffset[ConIdx]),
        (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) - DoIP_VTcpCon2RxMsgOffset[ConIdx]));
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;
      return DOIP_E_PENDING;
    }
#endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
  }

  if ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] >=
          (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)) ||
       (DoIP_VTcpCon2RxMsgOffset[ConIdx] >= (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) )
  {
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_VRxDiagMsgVerifyRxPdu() */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPAR */
/* PRQA L:MSR_STMIF */

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyVerifiedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_14_7 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgCopyVerifiedUserData(
  PduIdType ConIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
  P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
    ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
  Std_ReturnType retVal = E_NOT_OK;
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

  if ( /* less than pdu-max-data-length */
       ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] == (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) &&
         (DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] <
           ((DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH) - hdrLen)) )
# if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
       ||
       /* at least pdu-max-data-length */
       ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] ==
            (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)) &&
         (DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] < DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH) )
# endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
     )
  {
    PduInfoType pduInfo;
    pduInfo.SduLength = 0u;
    pduInfo.SduDataPtr =
      &(DoIP_VTcpCon2RxHdrBuf[ConIdx]
          [(DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DoIP_VTcpCon2VerifyRxPduOffset[ConIdx])]);
    retVal = E_OK;

    if ( QueryBufSize == TRUE )
    {
      pduInfo.SduLength = 0u;
      retVal = DoIP_VRxDiagMsgCopyDataToPduR(ConIdx, BufferSizePtr, &pduInfo);
    }

    if ( retVal == E_OK )
    {
      pduInfo.SduLength =
# if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
        (PduLengthType)DOIP_MIN(DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH, (hdrLen - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE));
      if ( pduInfo.SduLength > *BufferSizePtr )
      {
        pduInfo.SduLength = *BufferSizePtr;
      }
# else
        DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH;
# endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
      retVal = DoIP_VRxDiagMsgCopyDataToPduR(ConIdx, BufferSizePtr, &pduInfo);
    }

    if ( retVal == E_OK )
    {
      /* CopyRxData call was successful */
      PduLengthType bytesLeft;

      DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] += pduInfo.SduLength;

      if ( hdrLen < (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH) )
      {
        bytesLeft = (PduLengthType)(hdrLen - DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE);
      }
# if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
      else if ( DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] < DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH )
      {
        bytesLeft = (PduLengthType)(DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH - DoIP_VTcpCon2VerifyRxPduOffset[ConIdx]);
      }
# endif /* ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u ) */
      else
      {
        bytesLeft = (PduLengthType)(hdrLen - (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH));
      }

      if ( bytesLeft < *BufferSizePtr )
      { /* upper layer has more buffer than expected */
        *BufferSizePtr = bytesLeft;
      }
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;

      /* PRQA S 3356, 3359, 3201 11 */ /* MD_DoIP_13.7_TOOL */
      if ( bytesLeft == 0u )
      { /* entire message is processed */
        /* send diagnostic message positive acknowledge */
        (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_DIAG_ACK, DoIP_VTcpCon2LastRxTgtAddr[ConIdx],
          0u, 0u, NULL_PTR, FALSE);
        /* call PduR */
        PduR_DoIPTpRxIndication(DoIP_VChannel2PduRRxPduId[DoIP_VTcpCon2RxChannelActive[ConIdx]], E_OK);
        /* finish message */
        DoIP_VTcpCon2RxMsgOffset[ConIdx]       = 0u;
        DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] = 0u;
        /* reset active channel */
        DoIP_VTcpCon2RxChannelActive[ConIdx] = DOIP_INV_CHANNEL;
      }
      else
      {
        return DOIP_E_PENDING;
      }
    }
  }

  if ( /* less than pdu-max-data-length */
       ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] == (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) &&
         (DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] >=
           ((DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH) - hdrLen)) )
       ||
       /* at least pdu-max-data-length */
       ( (DoIP_VTcpCon2RxMsgOffset[ConIdx] >=
           (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH))
# if ( DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
         && (DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] >= DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)
# endif /* DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */
       )
     )
  {
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_VRxDiagMsgCopyVerifiedUserData() */
/* PRQA L:MSR_14_7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VRxDiagMsgCopyUserData(
  PduIdType ConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) LocalPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_VTcpCon2RxMsgOffset[ConIdx] >=
      (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH) )
  {
    /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
    P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
      ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_VAR_NOINIT))&DoIP_VTcpCon2RxHdrBuf[ConIdx][0u]);
    uint32 hdrLen;

    DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

    if ( DoIP_VTcpCon2RxChannelActive[ConIdx] != DOIP_INV_CHANNEL )
    {
      retVal = DoIP_VRxDiagMsgCopyDataToPduR(ConIdx, BufferSizePtr, PduInfoPtr);
    }

    if ( retVal == E_OK )
    {
      /* CopyRxData call was successful */
      PduLengthType bytesLeft = (PduLengthType)((DOIP_GEN_HDR_LEN_BYTE + hdrLen) -
        (DoIP_VTcpCon2RxMsgOffset[ConIdx] + LocalPduInfoPtr->SduLength));

      if ( bytesLeft < *BufferSizePtr )
      { /* upper layer has more buffer than expected */
        *BufferSizePtr = bytesLeft;
      }
      DoIP_VTcpCon2LastRxBufSize[ConIdx] = *BufferSizePtr;

      /* PRQA S 3356, 3359, 3201 11 */ /* MD_DoIP_13.7_TOOL */
      if ( bytesLeft == 0u )
      { /* entire message is processed */
        /* send diagnostic message positive acknowledge */
        (void)DoIP_TcpTx_Transmit(ConIdx, DOIP_HDR_TYPE_DIAG_ACK, DoIP_VTcpCon2LastRxTgtAddr[ConIdx],
          0u, 0u, NULL_PTR, FALSE);
        /* call PduR */
        PduR_DoIPTpRxIndication(DoIP_VChannel2PduRRxPduId[DoIP_VTcpCon2RxChannelActive[ConIdx]], E_OK);
        /* finish message */
        DoIP_VTcpCon2RxMsgOffset[ConIdx]       = 0u;
        DoIP_VTcpCon2VerifyRxPduOffset[ConIdx] = 0u;
        /* reset active channel */
        DoIP_VTcpCon2RxChannelActive[ConIdx]   = DOIP_INV_CHANNEL;
      }
      else
      { /* another part of the message is received */
        /* increment offset */
        DoIP_VTcpCon2RxMsgOffset[ConIdx] += LocalPduInfoPtr->SduLength;
      }
    }

    *ReadDataLenPtr += LocalPduInfoPtr->SduLength;
  }
} /* DoIP_VRxDiagMsgCopyUserData() */
/* PRQA L:MSR_STPAR */

/**********************************************************************************************************************
 *  DoIP_VRxDiagMsgCopyDataToPduR()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VRxDiagMsgCopyDataToPduR(
  PduIdType ConIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) LocalPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* call CopyRxData of PduR */
  if ( PduR_DoIPTpCopyRxData(DoIP_VChannel2PduRRxPduId[DoIP_VTcpCon2RxChannelActive[
      ConIdx]], LocalPduInfoPtr, BufferSizePtr) != BUFREQ_OK )
  { /* CopyRxData call was not successful */
    uint16 testerAddr;
    uint16 entityAddr = DoIP_VTcpCon2LastRxTgtAddr[ConIdx];

    /* send diagnostic message negative acknowledge */
#if ( DOIP_VDEFAULT_TESTER_ENABLED == STD_ON )
    if ( DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr == DOIP_ADDR_ANY )
    { /* default tester */
      testerAddr = DoIP_VTcpCon2ActivMgt[ConIdx].TesterAddr;
    }
    else
#endif /* DOIP_VDEFAULT_TESTER_ENABLED == STD_ON */
    { /* no default tester */
      testerAddr = DoIP_VTester[DoIP_VTcpCon2ActivMgt[ConIdx].TesterIdx].TesterAddr;
    }
    DoIP_TcpTx_PrepareTransmit(ConIdx, DOIP_HDR_TYPE_DIAG_NACK, entityAddr, testerAddr,
      DOIP_DIAG_MSG_RES_CODE_NETWORK_TRANSPROT, FALSE);
    /* call PduR */
    PduR_DoIPTpRxIndication(DoIP_VChannel2PduRRxPduId[DoIP_VTcpCon2RxChannelActive[ConIdx]], E_NOT_OK);
    /* reset active channel */
    DoIP_VTcpCon2RxChannelActive[ConIdx] = DOIP_INV_CHANNEL;
    /* skip message */
    DoIP_VTcpCon2SkipRxMsg[ConIdx] = TRUE;

    retVal = E_NOT_OK;
  }

  return retVal;
} /* DoIP_VRxDiagMsgCopyDataToPduR() */

/**********************************************************************************************************************
 *  DoIP_VGetChannelByTgtAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_ChannelIdType, DOIP_CODE) DoIP_VGetChannelByTgtAddr(
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ChannelIdType channel = DOIP_INV_CHANNEL;
  DoIP_ChannelIdType channelLeft = 0u;
  DoIP_ChannelIdType channelRight = (DoIP_ChannelIdType)(DOIP_VCHANNELS_TOTAL - 1u);
  DoIP_ChannelIdType channelMiddle;

  /* ----- Implementation ----------------------------------------------- */
  /* binary search in routing activation channel collection */
  while ( channelLeft <= channelRight )
  {
    boolean searchFinished = FALSE;

    /* calculate middle */
    channelMiddle = (DoIP_ChannelIdType)(channelLeft + ((DoIP_ChannelIdType)(channelRight - channelLeft) / 2u));

    /* check channel */
#if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
    if ( (DoIP_VChannel2EcuAddr[channelMiddle] & DoIP_VChannel2BitMask[channelMiddle]) ==
        (TgtAddr & DoIP_VChannel2BitMask[channelMiddle]) )
#else
    if ( DoIP_VChannel2EcuAddr[channelMiddle] == TgtAddr )
#endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
    { /* match */
      channel = channelMiddle;
      searchFinished = TRUE;
    }
    else
    { /* no match */
      /* check if target address of found channel is bigger than target address of channel to be searched */
#if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
      if ( (DoIP_VChannel2EcuAddr[channelMiddle] & DoIP_VChannel2BitMask[channelMiddle]) >
          (TgtAddr & DoIP_VChannel2BitMask[channelMiddle]) )
#else
      if ( DoIP_VChannel2EcuAddr[channelMiddle] > TgtAddr )
#endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
      { /* target address of found channel is bigger */
        if ( channelMiddle == 0u )
        {
          searchFinished = TRUE;
        }
        else
        {
          channelRight = (DoIP_ChannelIdType)(channelMiddle - 1u);
        }
      }
      else
      { /* target address of found channel is smaller */
        if ( channelMiddle == (DOIP_VCHANNELS_TOTAL - 1u) )
        {
          searchFinished = TRUE;
        }
        else
        {
          channelLeft = (DoIP_ChannelIdType)(channelMiddle + 1u);
        }
      }
    }
    if ( searchFinished == TRUE )
    {
      break;
    }
  }

  return channel;
} /* DoIP_VGetChannelByTgtAddr() */

/**********************************************************************************************************************
 *  DoIP_VGetChannelByTgtAddrForRoutAct()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(DoIP_ChannelIdType, DOIP_CODE) DoIP_VGetChannelByTgtAddrForRoutAct(
  uint8 RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen)
{
  DoIP_ChannelIdType channel = DOIP_INV_CHANNEL;

  if ( DoIP_VRoutingActivations[RoutActIdx].RxChannelNum > 0u )
  { /* routing activation has at least one rx channel */
    DoIP_ChannelIdType idxLeft = 0u;
    DoIP_ChannelIdType idxRight = (DoIP_ChannelIdType)(DoIP_VRoutingActivations[RoutActIdx].RxChannelNum - 1u);
    DoIP_ChannelIdType idxFound = DOIP_INV_CHANNEL;

    /* binary search in routing activation channel collection */
    while ( idxLeft <= idxRight )
    {
      boolean searchFinished = FALSE;

      /* calculate middle */
      DoIP_ChannelIdType idxMiddle = (DoIP_ChannelIdType)(idxLeft + ((DoIP_ChannelIdType)(idxRight - idxLeft) / 2u));

      /* get corresponding channel */
      DoIP_ChannelIdType channelTmp = DoIP_VRoutingActivations[RoutActIdx].RxChannelColPtr[idxMiddle];

      /* check channel */
#if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
      if ( (DoIP_VChannel2EcuAddr[channelTmp] & DoIP_VChannel2BitMask[channelTmp]) ==
          (TgtAddr & DoIP_VChannel2BitMask[channelTmp]) )
#else
      if ( DoIP_VChannel2EcuAddr[channelTmp] == TgtAddr )
#endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
      { /* match */
        idxFound = idxMiddle;
        searchFinished = TRUE;
      }
      else
      { /* no match */
        /* check if target address of found channel is bigger than target address of channel to be searched */
#if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
        if ( (DoIP_VChannel2EcuAddr[channelTmp] & DoIP_VChannel2BitMask[channelTmp]) >
            (TgtAddr & DoIP_VChannel2BitMask[channelTmp]) )
#else
        if ( DoIP_VChannel2EcuAddr[channelTmp] > TgtAddr )
#endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
        { /* target address of found channel is bigger */
          if ( idxMiddle == 0u )
          {
            searchFinished = TRUE;
          }
          else
          {
            idxRight = (DoIP_ChannelIdType)(idxMiddle - 1u);
          }
        }
        else
        { /* target address of found channel is smaller */
          if ( idxMiddle == (DoIP_ChannelIdType)(DoIP_VRoutingActivations[RoutActIdx].RxChannelNum - 1u) )
          {
            searchFinished = TRUE;
          }
          else
          {
            idxLeft = (DoIP_ChannelIdType)(idxMiddle + 1u);
          }
        }
      }
      if ( searchFinished == TRUE )
      {
        break;
      }
    }

    if ( idxFound != DOIP_INV_CHANNEL )
    { /* found a channel */
#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
      /* check for other better fitting channels (PDU size dependent routing feature) */
      P2CONST(DoIP_RoutingActivationType, AUTOMATIC, DOIP_APPL_VAR) routActPtr = &DoIP_VRoutingActivations[RoutActIdx];
      uint32 bestMaxPduSize = 0u;
      boolean bestChannelFound = FALSE;

      channel = routActPtr->RxChannelColPtr[idxFound];

      idxLeft = 0u;
      idxRight = (DoIP_ChannelIdType)(routActPtr->RxChannelNum - 1u);

      /* check current channel */
      if ( (DoIP_VPduSizeRoutValidateChannel(channel, DataLen, &bestMaxPduSize) == E_OK)
        && (DoIP_IsPduSizeRoutingEnabled == FALSE) )
      { /* channel is default channel and PDU size dependent routing is disabled */
        bestChannelFound = TRUE;
      }

      if ( bestChannelFound == FALSE )
      {
        /* search right */
        if ( idxFound != idxRight )
        { /* there are elements on the right */
          DoIP_ChannelIdType idx;

          for ( idx = (DoIP_ChannelIdType)(idxFound + 1u); idx <= idxRight; idx++ )
          {
            DoIP_ChannelIdType channelTmpRight = routActPtr->RxChannelColPtr[idx];
            boolean searchFinishedRight = FALSE;

# if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
            if ( (DoIP_VChannel2EcuAddr[channelTmpRight] & DoIP_VChannel2BitMask[channelTmpRight]) ==
                (TgtAddr & DoIP_VChannel2BitMask[channelTmpRight]) )
# else
            if ( DoIP_VChannel2EcuAddr[channelTmpRight] == TgtAddr )
# endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
            { /* match */
              if ( DoIP_VPduSizeRoutValidateChannel(channelTmpRight, DataLen, &bestMaxPduSize) == E_OK )
              {
                channel = channelTmpRight;
                if ( DoIP_IsPduSizeRoutingEnabled == FALSE )
                { /* channel is default channel */
                  /* no further search required */
                  bestChannelFound = TRUE;
                  searchFinishedRight = TRUE;
                }
              }
            }
            else
            { /* no match - stop search right */
              searchFinishedRight = TRUE;
            }

            if ( searchFinishedRight == TRUE )
            {
              break;
            }
          }
        }

        if ( bestChannelFound == FALSE )
        {
          /* search left */
          if ( idxFound != idxLeft )
          { /* there are elements on the left */
            DoIP_ChannelIdType idx;

            for ( idx = idxLeft; idx < idxFound; idx++ )
            {
              DoIP_ChannelIdType channelTmpLeft = routActPtr->RxChannelColPtr[(idxFound - idx) - 1u];
              boolean searchFinishedLeft = FALSE;

# if ( DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
              if ( (DoIP_VChannel2EcuAddr[channelTmpLeft] & DoIP_VChannel2BitMask[channelTmpLeft]) ==
                  (TgtAddr & DoIP_VChannel2BitMask[channelTmpLeft]) )
# else
              if ( DoIP_VChannel2EcuAddr[channelTmpLeft] == TgtAddr )
# endif /* DOIP_VTARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
              { /* match */
                if ( DoIP_VPduSizeRoutValidateChannel(channelTmpLeft, DataLen, &bestMaxPduSize) == E_OK )
                {
                  channel = channelTmpLeft;
                  if ( DoIP_IsPduSizeRoutingEnabled == FALSE )
                  { /* channel is default channel */
                    /* no further search required */
                    searchFinishedLeft = TRUE;
                  }
                }
              }
              else
              { /* no match - stop search left */
                searchFinishedLeft = TRUE;
              }

              if ( searchFinishedLeft == TRUE )
              {
                break;
              }
            }
          }
        }
      }
#else
      channel = DoIP_VRoutingActivations[RoutActIdx].RxChannelColPtr[idxFound];

      DOIP_DUMMY_STATEMENT_VAR(DataLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */
    }
  }

  return channel;
} /* DoIP_VGetChannelByTgtAddrForRoutAct() */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

#if ( DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VPduSizeRoutValidateChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_VPduSizeRoutValidateChannel(
  DoIP_ChannelIdType Channel,
  uint32 DataLen,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_VAR) BestMaxPduSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_IsPduSizeRoutingEnabled == TRUE )
  { /* PDU size dependent routing is enabled */
    if ( (DoIP_VChannel2MaxPduSize[Channel] >= DataLen) &&
      ((*BestMaxPduSize == 0u) || (DoIP_VChannel2MaxPduSize[Channel] < *BestMaxPduSize)) )
    { /* best fitting channel up to now */
      *BestMaxPduSize = DoIP_VChannel2MaxPduSize[Channel];
      retVal = E_OK;
    }
  }
  else
  { /* PDU size dependent routing is disabled */
    if ( DoIP_VChannel2IsDefaultChannel[Channel] == TRUE )
    { /* channel is default channel */
      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIP_VPduSizeRoutValidateChannel() */
#endif /* DOIP_VSUPPORT_PDU_SIZE_ROUTING == STD_ON */

#if ( DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_VHandleVerifiedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_VHandleVerifiedUserData(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 idx;

  /* ----- Implementation ----------------------------------------------- */
  for ( idx = 0u; idx < DOIP_VCONNECTION_TCP_TOTAL; idx++ )
  {
    /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
    P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA) hdrPtr =
      ((P2VAR(DoIP_HdrType, AUTOMATIC, DOIP_APPL_DATA))&DoIP_VTcpCon2RxHdrBuf[idx][0]);
    uint16 hdrType;

    DOIP_GET_UINT16(hdrPtr->Type, 0u, hdrType);

    if ( (DoIP_VTcpCon2ActivMgt[idx].ActivState == DOIP_ROUT_ACTIV_STATE_ACTIVATED) &&
         (hdrType == DOIP_HDR_TYPE_DIAG_REQ) )
    {
      uint32 hdrLen;

      DOIP_GET_UINT32(hdrPtr->LenByte, 0u, hdrLen);

      if ( (DoIP_VTcpCon2SkipRxMsg[idx] == FALSE) && (hdrLen >= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
      {
        (void)DoIP_VRxDiagMsgCopyVerifiedUserData(
          idx, &(DoIP_VTcpCon2LastRxBufSize[idx]), TRUE);
      }

      if ( /* skip message if all data are received */
           ( (DoIP_VTcpCon2SkipRxMsg[idx] == TRUE) &&
             (DoIP_VTcpCon2RxMsgOffset[idx] >= (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) ) ||
           /* finish message if no user data present */
           ( (hdrLen <= (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_VVERIFY_RX_PDU_MAX_DATA_LENGTH)) &&
             (DoIP_VTcpCon2RxMsgOffset[idx] == (DOIP_GEN_HDR_LEN_BYTE + hdrLen)) ) )
      {
        /* finish message */
        DoIP_TcpTx_FinishTransmit(idx);

        DoIP_VTcpCon2VerifyRxPduOffset[idx] = 0u;
        DoIP_VTcpCon2LastRxBufSize[idx]     = 0u;
        DoIP_VTcpCon2RxMsgOffset[idx]       = 0u;
        DoIP_VTcpCon2SkipRxMsg[idx]         = FALSE;
      }
    }
  }
} /* DoIP_VHandleVerifiedUserData() */
#endif /* DOIP_VVERIFY_RX_PDU_ENABLED == STD_ON */

#define DOIP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
  MD_DoIP_3.1:
      Reason:     Signed unsigned mismatch for data type independence
      Risk:       Misinterpretation of data.
      Prevention: Covered by code review.
  MD_DoIP_5.1:
      Reason:     Identifier does not differ from other identifiers within 31 significant characters.
      Risk:       There is no risk as such statements have no effect on the code.
      Prevention: Covered by code review.
  MD_DoIP_11.1:
      Reason:     Casting to different function pointer type
      Risk:       Misinterpretation of data.
      Prevention: Covered by code review.
  MD_DoIP_11.4:
      Reason:     Casting to different object pointer type
      Risk:       Misinterpretation of data.
      Prevention: Covered by code review.
  MD_DoIP_13.7_TOOL:
      Reason:     Expression is always 'true' or 'false' misinterpreted by test tool
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_14.6:
      Reason:     Multiple break statements due to runtime optimization.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_16.7:
      Reason:     No pointer to const due to modification in a called function or required by specification.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_21.1_INV:
      Reason:     Used to get an invalid value depending on data type or to get the inverse value.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_21.1_CFG:
      Reason:     Error does not apply since prevention is given in configuration.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_21.1_IDX:
      Reason:     Indexing out of bounds prevented by complex mechanism (misinterpreted by test tool).
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_0686:
      Reason:     All not initialized elements are overwritten later.
      Risk:       Uninitialized elements may remain.
      Prevention: Covered by code review.
  MD_DoIP_3199:
      Reason:     Symbol is part of struct overlaying a byte array (misinterpreted by test tool).
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_DoIP_3206:
      Reason:     API parameter is only used in some configurations or is required by specification but currently not
                  supported.
      Risk:       No risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: DoIP.c
 *********************************************************************************************************************/
