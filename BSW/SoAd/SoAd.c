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
/**        \file  SoAd.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor module.
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

#define SOAD_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */

#include "SoAd.h"
#include "SoAd_Priv.h"
#include "SoAd_Cbk.h"
#include "IpBase.h"
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
# include <sys/socket.h>
# include <sys/poll.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <net/if.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <ifaddrs.h>
# include <netinet/tcp.h>
#else
# include "TcpIp.h"
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
#if ( SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == SOAD_CONFIGURATION_VARIANT )
# include "EcuM_Error.h"
#endif /* SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == SOAD_CONFIGURATION_VARIANT */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of SoAd header file */
#if ( (SOAD_SW_MAJOR_VERSION != 10u) \
    || (SOAD_SW_MINOR_VERSION != 4u) \
    || (SOAD_SW_PATCH_VERSION != 1u) )
# error "Vendor specific version numbers of SoAd.c and SoAd.h are inconsistent"
#endif /* (SOAD_SW_MAJOR_VERSION != 10u) \
    || (SOAD_SW_MINOR_VERSION != 4u) \
    || (SOAD_SW_PATCH_VERSION != 1u) */

/* Check configuration variant for building the library */
#if ( (defined(V_EXTENDED_BUILD_LIB_CHECK)) && (SOAD_CONFIGURATION_VARIANT_PRECOMPILE == SOAD_CONFIGURATION_VARIANT) )
# error "SoAd.c: Switch configuration variant to link-time or post-build for library build!"
#endif /* (defined(V_EXTENDED_BUILD_LIB_CHECK)) &&
  (SOAD_CONFIGURATION_VARIANT_PRECOMPILE == SOAD_CONFIGURATION_VARIANT) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if defined(C_COMP_ANSI_CANOE)
# define SOAD_VENABLE_CANOE_WRITE_STRING   STD_ON
#else
# define SOAD_VENABLE_CANOE_WRITE_STRING   STD_OFF
#endif /* defined(C_COMP_ANSI_CANOE) */

#define SOAD_META_DATA_LEN 2u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS */ /* MD_MSR_19.4, MD_MSR_19.7 */

#if defined(C_COMP_ANSI_CANOE)
# if ( SOAD_VENABLE_CANOE_WRITE_STRING == STD_ON )
#  include "stdio.h"
#  define CANOE_WRITE_STRING(Txt) \
            CANoeAPI_WriteString((Txt));
#  define CANOE_WRITE_STRING_1(Txt, P1) \
            _snprintf(SoAd_CanoeWriteStr, 256, Txt, P1); \
            CANoeAPI_WriteString((SoAd_CanoeWriteStr));
#  define CANOE_WRITE_STRING_2(Txt, P1, P2) \
            _snprintf(SoAd_CanoeWriteStr, 256, Txt, P1, P2); \
            CANoeAPI_WriteString((SoAd_CanoeWriteStr));
#  define CANOE_WRITE_STRING_3(Txt, P1, P2, P3) \
            _snprintf(SoAd_CanoeWriteStr, 256, Txt, P1, P2, P3); \
            CANoeAPI_WriteString((SoAd_CanoeWriteStr));
# else
#  define CANOE_WRITE_STRING(txt)
#  define CANOE_WRITE_STRING_1(txt, p1)
#  define CANOE_WRITE_STRING_2(txt, p1, p2)
#  define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
# endif /* SOAD_VENABLE_CANOE_WRITE_STRING == STD_ON */
#else
# define CANOE_WRITE_STRING(txt)
# define CANOE_WRITE_STRING_1(txt, p1)
# define CANOE_WRITE_STRING_2(txt, p1, p2)
# define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
#endif /* defined(C_COMP_ANSI_CANOE) */

#if ( SOAD_VSOCKET_BSD_REPORT_ERROR_ENABLED == STD_ON )
# define SOAD_REPORT_SOCK_ERROR(API_ID, BSD_API_ID, ERRNO) \
            SoAd_VCallReportErrorFunc((API_ID), (BSD_API_ID), SOAD_SOCK_BSD_PARAM_NONE, \
              SOAD_SOCK_BSD_VALUE_NONE, SOAD_SOCK_BSD_VALUE_NONE, (ERRNO))
# define SOAD_REPORT_SOCK_ERROR_PARAM(API_ID, BSD_API_ID, PARAM, ERRNO) \
            SoAd_VCallReportErrorFunc((API_ID), (BSD_API_ID), (PARAM), SOAD_SOCK_BSD_VALUE_NONE, \
              SOAD_SOCK_BSD_VALUE_NONE, (ERRNO))
# define SOAD_REPORT_SOCK_ERROR_PARAM_1(API_ID, BSD_API_ID, PARAM, VAL1, ERRNO) \
            SoAd_VCallReportErrorFunc((API_ID), (BSD_API_ID), (PARAM), (VAL1), SOAD_SOCK_BSD_VALUE_NONE, (ERRNO))
# define SOAD_REPORT_SOCK_ERROR_PARAM_2(API_ID, BSD_API_ID, PARAM, VAL1, VAL2, ERRNO) \
            SoAd_VCallReportErrorFunc((API_ID), (BSD_API_ID), (PARAM), (VAL1), (VAL2), (ERRNO))
#else
# define SOAD_REPORT_SOCK_ERROR(API_ID, BSD_API_ID, ERRNO)
# define SOAD_REPORT_SOCK_ERROR_PARAM(API_ID, BSD_API_ID, PARAM, ERRNO)
# define SOAD_REPORT_SOCK_ERROR_PARAM_1(API_ID, BSD_API_ID, PARAM, VAL1, ERRNO)
# define SOAD_REPORT_SOCK_ERROR_PARAM_2(API_ID, BSD_API_ID, PARAM, VAL1, VAL2, ERRNO)
#endif /* SOAD_VSOCKET_BSD_REPORT_ERROR_ENABLED == STD_ON */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL)
# define SOAD_LOCAL static
#endif /* !defined (SOAD_LOCAL) */

#if !defined (SOAD_LOCAL_INLINE)
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (SOAD_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) )
/*! Counts the main function cycles (with wrap around) to handle timeouts for the timeout lists. */
SOAD_LOCAL VAR(uint32,         SOAD_VAR_NOINIT) SoAd_GlobalCounter;
#endif /* (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) */

#define SOAD_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SOAD_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Counts the number of open TcpIp sockets. */
SOAD_LOCAL VAR(SoAd_SockIdxType, SOAD_VAR_NOINIT) SoAd_OpenSocks;
/*! Indicates the counter for shutdown finished timeout to switch to shutdown state if sockets could not be closed. */
SOAD_LOCAL VAR(uint16,           SOAD_VAR_NOINIT) SoAd_ShutdownFinishedCnt;

#define SOAD_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SOAD_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Counts the enter and leave critical section statements to prevent that corresponding function is called nested. */
SOAD_LOCAL VAR(uint8,          SOAD_VAR_NOINIT) SoAd_CriticalSectionCount;
#if ( SOAD_VENABLE_CANOE_WRITE_STRING == STD_ON )
/*! Buffers the message string in case report to CANoe is used. */
SOAD_LOCAL VAR(uint8,          SOAD_VAR_NOINIT) SoAd_CanoeWriteStr[256];
#endif /* SOAD_VENABLE_CANOE_WRITE_STRING == STD_ON */

#define SOAD_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SOAD_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Indicates current state of the module state machine (Valid values: SOAD_STATE_*). */
SOAD_LOCAL VAR(SoAd_StateType, SOAD_VAR_ZERO_INIT) SoAd_State = SOAD_STATE_UNINIT;

#define SOAD_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define SOAD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
/*! Pointer to the post-build configuration. */
P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_PBCFG) SoAd_ConfigPtr = NULL_PTR;
#endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */

#define SOAD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SOAD_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( SOAD_CONFIGURATION_VARIANT != SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
/*! Pointer to the pre-compile configuration. */
CONSTP2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAd_ConfigPtr = &SoAd_GlobalConfig;
#endif /* SOAD_CONFIGURATION_VARIANT != SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */

#define SOAD_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  SoAd_EventQueue_Init()
**********************************************************************************************************************/
/*! \brief      Initializes all event queues.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_EventQueue_Init(void);

/**********************************************************************************************************************
  SoAd_EventQueue_AddElement()
**********************************************************************************************************************/
/*! \brief      Adds an event to a specific queue.
 *  \details    -
 *  \param[in]  EveQueIdx   Index of the event queue.
 *  \param[in]  HandleIdx   Handle index (e.g. socket connection index).
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_EventQueue_AddElement(
  uint8 EveQueIdx,
  uint16 HandleIdx);

/**********************************************************************************************************************
  SoAd_EventQueue_GetNextElement()
**********************************************************************************************************************/
/*! \brief      Gets the next element of an specific event queue.
 *  \details    -
 *  \param[in]  EveQueIdx     Index of the event queue.
 *  \param[out] HandleIdxPtr  Pointer to the handle index of next element (e.g. socket connection index).
 *  \return     E_OK          Queue has elements and next element is returned and removed from event queue.
 *  \return     E_NOT_OK      Queue has no elements left.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_EventQueue_GetNextElement(
  uint8 EveQueIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr);

#if ( (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) )
/**********************************************************************************************************************
  SoAd_TimeoutList_Init()
**********************************************************************************************************************/
/*! \brief      Initializes all timeout lists.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED | SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TimeoutList_Init(void);

/**********************************************************************************************************************
  SoAd_TimeoutList_SetElement()
**********************************************************************************************************************/
/*! \brief      Sets an element in a specific timeout list.
 *  \details    Adds an element if not already in list or updates element otherwise.
 *  \param[in]  TimeListIdx   Index of the timeout list.
 *  \param[in]  HandleIdx     Handle index (e.g. socket connection index).
 *  \param[in]  Timeout       Configured timeout value.
 *  \param[in]  TimerCounter  Running timer counter value.
 *  \return     E_OK          Element added or updated.
 *  \return     E_NOT_OK      List size is not sufficient (occurs if limitation is enabled only).
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED | SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_SetElement(
  uint8 TimeListIdx,
  uint16 HandleIdx,
  uint32 Timeout,
  uint32 TimerCounter);

/**********************************************************************************************************************
  SoAd_TimeoutList_GetCurrentTimeout()
**********************************************************************************************************************/
/*! \brief      Gets the current timeout value of an element in a specific timeout list.
 *  \details    -
 *  \param[in]  TimeListIdx   Index of the timeout list.
 *  \param[in]  HandleIdx     Handle index (e.g. socket connection index).
 *  \param[out] TimeoutPtr    Pointer to current timeout value.
 *  \param[in]  TimerCounter  Running timer counter value.
 *  \return     E_OK          Element is in timeout list and current timeout value is returned.
 *  \return     E_NOT_OK      Element is not in timeout list.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED | SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_GetCurrentTimeout(
  uint8 TimeListIdx,
  uint16 HandleIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) TimeoutPtr,
  uint32 TimerCounter);

/**********************************************************************************************************************
  SoAd_TimeoutList_CheckElements()
**********************************************************************************************************************/
/*! \brief      Checks for a timeout in a specific timeout list.
 *  \details    -
 *  \param[in]  TimeListIdx   Index of the timeout list.
 *  \param[out] ElementIdxPtr Pointer to element index with timeout.
 *  \param[out] HandleIdxPtr  Pointer to handle index with timeout.
 *  \param[in]  TimerCounter  Running timer counter value.
 *  \return     E_OK          Element with timeout found.
 *  \return     E_NOT_OK      No element with timeout found.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED | SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_CheckElements(
  uint8 TimeListIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) ElementIdxPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr,
  uint32 TimerCounter);

/**********************************************************************************************************************
  SoAd_TimeoutList_RemoveElement()
**********************************************************************************************************************/
/*! \brief      Removes an element from a specific timeout list.
 *  \details    -
 *  \param[in]  TimeListIdx   Index of the timeout list.
 *  \param[in]  HandleIdx     Handle index (e.g. socket connection index).
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED | SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TimeoutList_RemoveElement(
  uint8 TimeListIdx,
  uint16 HandleIdx);
#endif /* (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) */

/**********************************************************************************************************************
  SoAd_TcpIp_Init()
**********************************************************************************************************************/
/*! \brief      Initializes Socket API specific structs.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_Init(void);

/**********************************************************************************************************************
  SoAd_TcpIp_RequestIpAddrAssignment()
**********************************************************************************************************************/
/*! \brief      Requests IP address assignment on a local address identified by a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx         Socket connection index.
 *  \param[in]  Type             IP address type.
 *  \param[in]  LocalIpAddrPtr   Pointer to IP address which shall be assigned.
 *  \param[in]  Netmask          Netmask in CIDR.
 *  \param[in]  DefaultRouterPtr Pointer to default router (gateway) address.
 *  \return     E_OK             Assignment request was accepted..
 *  \return     E_NOT_OK         Assignment request was not accepted..
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different local IP addresses, FALSE for same local IP address
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief      Releases IP address assignment on a local address identified by a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx         Socket connection index.
 *  \return     E_OK             Release request was accepted.
 *  \return     E_NOT_OK         Release request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different local IP addresses, FALSE for same local IP address
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ReleaseIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetIpAddr()
 *********************************************************************************************************************/
/*! \brief       Returns the local IP address on a local address identified by a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx           Socket connection index.
 *  \param[out]  LocalAddrPtr       Pointer to local address (IP and Port).
 *  \param[out]  NetmaskPtr         Pointer to network mask (CIDR Notation).
 *  \param[out]  DefaultRouterPtr   Pointer to default router (gateway).
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different local IP addresses, FALSE for same local IP address
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_GetIpAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_CompareToIpAddr()
 *********************************************************************************************************************/
/*! \brief      Compares an IP address to all local IP addresses on an IP controller/interface.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  IpAddrPtr       Pointer to IP address.
 *  \return     E_OK            IP address matches one of the local IP addresses.
 *  \return     E_NOT_OK        IP address does not match one of the local IP addresses.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different local IP addresses, FALSE for same local IP address
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_CompareToIpAddr(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrConstPtrType IpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetPhysAddr()
 *********************************************************************************************************************/
/*! \brief       Returns the physical address (MAC address) of a local interface identified by a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx         Socket connection index.
 *  \param[out]  PhysAddrPtr      Pointer to physical address.
 *  \return      E_OK             Request was accepted.
 *  \return      E_NOT_OK         Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_GetPhysAddr(
  SoAd_SoConIdType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) PhysAddrPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_ReadDhcpHostNameOption()
 *********************************************************************************************************************/
/*! \brief          Returns the DHCP hostname option currently configured on a local interface identified by a
 *                  socket connection.
 *  \details        -
 *  \param[in]      SoConIdx  Socket connection index.
 *  \param[in,out]  LengthPtr Pointer to length of buffer for hostname (updated to length of hostname).
 *  \param[out]     DataPtr   Pointer to buffer for hostname.
 *  \return         E_OK      Request was accepted.
 *  \return         E_NOT_OK  Request was not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ReadDhcpHostNameOption(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_WriteDhcpHostNameOption()
 *********************************************************************************************************************/
/*! \brief        Sets the DHCP hostname option on a local interface identified by a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx  Socket connection index.
 *  \param[in]    Length    Length of buffer for hostname.
 *  \param[in]    DataPtr   Pointer to buffer for hostname.
 *  \return       E_OK      Request was accepted.
 *  \return       E_NOT_OK  Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_WriteDhcpHostNameOption(
  SoAd_SoConIdxType SoConIdx,
  uint8 Length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetSocket()
 *********************************************************************************************************************/
/*! \brief        Creates a socket and returns the assigned identifier.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *  \param[in]    Domain        IP domain (IPv4/6).
 *  \param[in]    Protocol      Transport protocol (UDP/TCP).
 *  \param[out]   SocketIdPtr   Pointer to socket identifier.
 *  \return       E_OK          Socket request was accepted.
 *  \return       E_NOT_OK      Socket request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_GetSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_DomainType Domain,
  SoAd_ProtocolType Protocol,
  P2VAR(SoAd_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIp_Bind()
 *********************************************************************************************************************/
/*! \brief          Binds a socket to a local address and port.
 *  \details        Returns a dynamically assigned port if wildcard (*PortPtr == 0) is used.
 *  \param[in]      SoConIdx      Socket connection index.
 *  \param[in]      SocketId      Socket identifier.
 *  \param[in,out]  PortPtr       Pointer to local port.
 *  \return         E_OK          Bind socket was successful.
 *  \return         E_NOT_OK      Bind socket was not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_Bind(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr);

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpListen()
 *********************************************************************************************************************/
/*! \brief      Sets a TCP socket to listen.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *  \param[in]  SocketId      Socket identifier.
 *  \param[in]  MaxChannels   Maximum number of parallel channels (TCP connections) allowed on this socket.
 *  \return     E_OK          Socket is set to listen.
 *  \return     E_NOT_OK      Socket is not set to listen.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpListen(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  uint16 MaxChannels);

/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpConnect()
 *********************************************************************************************************************/
/*! \brief      Requests connecting a TCP socket to remote entity.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *  \param[in]  SocketId      Socket identifier.
 *  \param[in]  RemAddrPtr    Pointer to remote address.
 *  \return     E_OK          Connect request accepted.
 *  \return     E_NOT_OK      Connect request not accepted.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpConnect(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr);
#endif /* SOAD_VTCP_USED */

/**********************************************************************************************************************
 *  SoAd_TcpIp_ChangeParameter()
 *********************************************************************************************************************/
/*! \brief      Changes parameter on a socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  SocketId        Socket identifier.
 *  \param[in]  ParameterId     Parameter identifier.
 *  \param[in]  ParameterValue  Parameter value.
 *  \return     E_OK            Change parameter request was successful.
 *  \return     E_NOT_OK        Change parameter request was not successful.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  SoAd_ParamIdType ParameterId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValue);

/**********************************************************************************************************************
 *  SoAd_TcpIp_Close()
 *********************************************************************************************************************/
/*! \brief      Closes a socket.
 *  \details    -
 *  \param[in]  SocketId            Socket identifier.
 *  \param[in]  Abort               Indicates if TCP connection shall be closed by reset or teardown sequence (not used
 *                                  in case of BSD socket API).
 *  \param[in]  SockIdx             Module internal socket index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_Close(
  SoAd_SocketIdType SocketId,
  boolean Abort,
  SoAd_SockIdxType SockIdx);

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TriggerAddressResolution()
 *********************************************************************************************************************/
/*! \brief      Triggers address resolution for a remote address on a socket connection (ARP/NDP).
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_TriggerAddressResolution(
  SoAd_SoConIdxType SoConIdx);
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIp_UdpTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits data over UDP socket.
 *  \details    Triggers transmission but data is copied in context of SoAd_CopyTxData.
 *  \param[in]  SockIdx           Module internal socket index.
 *  \param[in]  SocketId          Socket identifier.
 *  \param[in]  RemAddrPtr        Pointer to remote address.
 *  \param[in]  AvailableLength   Length of available data.
 *  \return     E_OK              Transmission request accepted.
 *  \return     E_NOT_OK          Transmission request not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different module internal socket indexes, FALSE for same module internal socket index
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_UdpTransmit(
  SoAd_SockIdxType SockIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC) RemAddrPtr,
  uint16 AvailableLength);

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits data over TCP socket.
 *  \details    Triggers transmission but data is copied in context of SoAd_CopyTxData.
 *  \param[in]  SoConIdx          Socket connection index.
 *  \param[in]  AvailableLength   Length of available data.
 *  \return     E_OK              Transmission request accepted.
 *  \return     E_NOT_OK          Transmission request not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket identifiers, FALSE for same socket identifier
 *  \config     SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpTransmit(
  SoAd_SoConIdxType SoConIdx,
  uint32 AvailableLength);

/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpReceived()
 *********************************************************************************************************************/
/*! \brief      Releases TCP reception buffer.
 *  \details    -
 *  \param[in]  SocketId      Socket identifier.
 *  \param[in]  Length        Length of data to be released.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE for different socket identifiers, FALSE for same socket identifier.
 *  \config     SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_TcpReceived(
  SoAd_SocketIdType SocketId,
  uint32 Length);
#endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionRx()
 *********************************************************************************************************************/
/*! \brief      Handles reception on socket layer.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionRx(void);

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionState()
 *********************************************************************************************************************/
/*! \brief      Handles states on socket layer.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionState(void);

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionTx()
 *********************************************************************************************************************/
/*! \brief      Handles transmission on socket layer.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionTx(void);

#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/**********************************************************************************************************************
  SoAd_TcpIpBsd_RequestIpAddrAssignment()
**********************************************************************************************************************/
/*! \brief      Requests IP address assignment on a local address identified by a socket connection in case of BSD
 *              Socket API.
 *  \details    -
 *  \param[in]  SoConIdx         Socket connection index.
 *  \param[in]  Type             IP address type.
 *  \param[in]  LocalIpAddrPtr   Pointer to IP address which shall be assigned.
 *  \param[in]  Netmask          Netmask in CIDR.
 *  \param[in]  DefaultRouterPtr Pointer to default router (gateway) address.
 *  \return     E_OK             Assignment request was accepted.
 *  \return     E_NOT_OK         Assignment request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different local IP addresses, FALSE for same local IP address.
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_GetSocket()
 *********************************************************************************************************************/
/*! \brief        Creates a socket on BSD Socket API and returns the assigned identifier.
 *  \details      For unicast sockets additional sockets are created to receive directed and limted broadcasts.
 *  \param[in]    SoConIdx      Socket connection index.
 *  \param[in]    Domain        IP domain (IPv4/6).
 *  \param[in]    Protocol      Transport protocol (UDP/TCP).
 *  \param[out]   SocketIdPtr   Pointer to socket identifier.
 *  \return       E_OK          Socket request was accepted.
 *  \return       E_NOT_OK      Socket request was not accepted.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_GetSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_DomainType Domain,
  SoAd_ProtocolType Protocol,
  P2VAR(SoAd_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_Bind()
 *********************************************************************************************************************/
/*! \brief          Binds a BSD Socket API socket to a local address and port.
 *  \details        Returns a dynamically assigned port if wildcard (*PortPtr == 0) is used.
 *                  For unicast sockets additional sockets are bound to receive directed and limted broadcasts.
 *  \param[in]      SoConIdx      Socket connection index.
 *  \param[in]      SocketId      Socket identifier.
 *  \param[in,out]  PortPtr       Pointer to local port.
 *  \return         E_OK          Bind socket was successful.
 *  \return         E_NOT_OK      Bind socket was not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \config         SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_Bind(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_BindSingleSocket()
 *********************************************************************************************************************/
/*! \brief          Binds a single BSD Socket API socket to a local address and port.
 *  \details        Returns a dynamically assigned port if wildcard (*PortPtr == 0) is used.
 *  \param[in]      SoConIdx      Socket connection index.
 *  \param[in]      SocketId      Socket identifier.
 *  \param[in]      LocalAddrPtr  Pointer to local address.
 *  \param[in]      LocalAddrSize Size of local address struct.
 *  \param[in,out]  PortPtr       Pointer to local port.
 *  \return         E_OK          Bind socket was successful.
 *  \return         E_NOT_OK      Bind socket was not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \config         SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_BindSingleSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2CONST(struct sockaddr, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  size_t LocalAddrSize,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ChangeParameter()
 *********************************************************************************************************************/
/*! \brief      Changes parameter on a BSD Socket API socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  SocketId        Socket identifier.
 *  \param[in]  ParameterId     Parameter identifier.
 *  \param[in]  ParameterValue  Parameter value.
 *  \return     E_OK            Change parameter request was successful.
 *  \return     E_NOT_OK        Change parameter request was not successful.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_ChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  SoAd_ParamIdType ParameterId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValue);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleSocketReception()
 *********************************************************************************************************************/
/*! \brief      Handles reception on BSD Socket API sockets.
 *  \details    Polls the BSD Socket API for pending received data.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleSocketReception(void);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_UdpReceiveFrom()
 *********************************************************************************************************************/
/*! \brief      Receives data from UDP BSD Socket API socket and forwards them to the SoAd_RxIndication.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  SocketId        Socket identifier.
 *  \param[out] RemAddrPtr      Pointer to remote address.
 *  \param[in]  RemAddrSize     Size of remote address struct.
 *  \return     E_OK            Socket is still available.
 *  \return     E_NOT_OK        Socket is not available anymore.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_UdpReceiveFrom(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(struct sockaddr, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr,
  socklen_t RemAddrSize);

# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpReceive()
 *********************************************************************************************************************/
/*! \brief      Receives data from TCP BSD Socket API socket and forwards them to the SoAd_RxIndication.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \return     E_OK            Socket is still available.
 *  \return     E_NOT_OK        Socket is not available anymore.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_TcpReceive(
  SoAd_SoConIdxType SoConIdx);
# endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleIpAddrStates()
 *********************************************************************************************************************/
/*! \brief      Handles IP address assignment states for BSD Socket API.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleIpAddrStates(void);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_AssignIpAddr()
 *********************************************************************************************************************/
/*! \brief      Assigns a IP address of a local address identified by a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \return     E_OK        IP address assignment was sucessful.
 *  \return     E_NOT_OK    IP address assignment was not sucessful.
 *  \pre        To be called within critical section.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_AssignIpAddr(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ReleaseIpAddr()
 *********************************************************************************************************************/
/*! \brief      Releases an assigned IP address of a local address identified by a socket connection and closes
 *              the corresponding sockets.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ReleaseIpAddr(
  SoAd_SoConIdxType SoConIdx);

# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleTcpSocketStates()
 *********************************************************************************************************************/
/*! \brief      Handles TCP BSD Socket API socket states.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleTcpSocketStates(void);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpCheckConnect()
 *********************************************************************************************************************/
/*! \brief      Checks if a TCP client connection is established.
 *  \details    -
 *  \param[in]  SockIdx         Module internal socket index.
 *  \return     TRUE            Retry required since connection is not yet established.
 *  \return     FALSE           Check was not successful.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_TcpIpBsd_TcpCheckConnect(
  SoAd_SockIdxType SockIdx);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpCheckAccept()
 *********************************************************************************************************************/
/*! \brief      Checks if a TCP server connection is established.
 *  \details    -
 *  \param[in]  SockIdx         Module internal socket index.
 *  \return     TRUE            Retry required since listen socket is still active.
 *  \return     FALSE           Check was not successful.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_TcpIpBsd_TcpCheckAccept(
  SoAd_SockIdxType SockIdx);
# endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ResetLocalIpAddr()
 *********************************************************************************************************************/
/*! \brief      Resets local IP address struct.
 *  \details    -
 *  \param[in]  LocalAddrId   Local address identifier.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ResetLocalIpAddr(
  SoAd_LocalAddrIdType LocalAddrId);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr()
 *********************************************************************************************************************/
/*! \brief      Checks existence of local address and overwrites local address.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  OverwriteLocal  Overwrite configured local address or not.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr(
  SoAd_SoConIdxType SoConIdx,
  boolean OverwriteLocal);

# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleTcpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Handles TxConfirmation for TCP socket connection on BSD Socket API.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleTcpTxConfirmation(
  SoAd_SoConIdxType SoConIdx);
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr()
 *********************************************************************************************************************/
/*! \brief        Converts a socket address in AUTOSAR format to a socket address in BSD socket API format.
 *  \details      -
 *  \param[out]   BsdSockAddrPtr   Pointer to socket address in BSD socket API format.
 *  \param[in]    AsrSockAddrPtr   Pointer to socket address in AUTOSAR format.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr(
  P2VAR(struct sockaddr, SOAD_APPL_DATA, SOAD_APPL_DATA) BsdSockAddrPtr,
  P2CONST(SoAd_SockAddrType, SOAD_APPL_DATA, SOAD_APPL_DATA) AsrSockAddrPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr()
 *********************************************************************************************************************/
/*! \brief        Converts a socket address in BSD socket API format to a socket address in AUTOSAR format.
 *  \details      -
 *  \param[out]   AsrSockAddrPtr   Pointer to socket address in AUTOSAR format.
 *  \param[in]    BsdSockAddrPtr   Pointer to socket address in BSD socket API format.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_BSD_API_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr(
  P2VAR(SoAd_SockAddrType, SOAD_APPL_DATA, SOAD_APPL_DATA) AsrSockAddrPtr,
  P2CONST(struct sockaddr, SOAD_APPL_DATA, SOAD_APPL_DATA) BsdSockAddrPtr);

# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_InitSocketTxBuffer()
 *********************************************************************************************************************/
/*! \brief        Initializes the socket transmission buffers.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_InitSocketTxBuffer(void);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_RequestSocketTxBuffer()
 *********************************************************************************************************************/
/*! \brief        Requests a socket transmission buffer.
 *  \details      -
 *  \param[in]    SockIdx           Module internal socket index.
 *  \param[out]   SockTxBufIdxPtr   Pointer to socket transmission buffer index.
 *  \return       E_OK              Transmission buffer was requested successfully.
 *  \return       E_NOT_OK          Transmission buffer was not requested successfully.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_RequestSocketTxBuffer(
  SoAd_SockIdxType SockIdx,
  P2VAR(SoAd_SockTxBufIdxType, SOAD_APPL_DATA, SOAD_APPL_DATA) SockTxBufIdxPtr);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_UpdateSocketTxBuffer()
 *********************************************************************************************************************/
/*! \brief      Updates a socket transmission buffer by setting data length to mark it for retry.
 *  \details    -
 *  \param[in]  SockTxBufIdx  Socket transmission buffer index.
 *  \param[in]  Length        Length of data stored in socket transmission buffer.
 *  \return     E_OK          Transmission buffer is not last buffer and length was updated.
 *  \return     E_NOT_OK      Transmission buffer is last buffer.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_UpdateSocketTxBuffer(
  SoAd_SockTxBufIdxType SockTxBufIdx,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ReleaseSocketTxBuffer()
 *********************************************************************************************************************/
/*! \brief      Releases a requested socket transmission buffer.
 *  \details    -
 *  \param[in]  SockTxBufIdx  Socket transmission buffer index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ReleaseSocketTxBuffer(
  SoAd_SockTxBufIdxType SockTxBufIdx);

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleSocketTxBuffer()
 *********************************************************************************************************************/
/*! \brief      Handles transmissions on socket transmission buffers marked for retry.
 *  \details    -
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED & SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleSocketTxBuffer(void);
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_OFF )
/**********************************************************************************************************************
  SoAd_TcpIpAsr_RequestIpAddrAssignment()
**********************************************************************************************************************/
/*! \brief      Requests IP address assignment on a local address identified by a socket connection in case of AUTOSAR
 *              Socket API.
 *  \details    -
 *  \param[in]  SoConIdx         Socket connection index.
 *  \param[in]  Type             IP address type.
 *  \param[in]  LocalIpAddrPtr   Pointer to IP address which shall be assigned.
 *  \param[in]  Netmask          Netmask in CIDR.
 *  \param[in]  DefaultRouterPtr Pointer to default router (gateway) address.
 *  \return     E_OK             Assignment request was accepted.
 *  \return     E_NOT_OK         Assignment request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different local IP addresses, FALSE for same local IP address.
 *  \config     SOAD_VSOCKET_BSD_API_ENABLED == STD_OFF
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpAsr_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_OFF */

/**********************************************************************************************************************
  SoAd_SoCon_Init()
**********************************************************************************************************************/
/*! \brief      Initializes socket connection structs.
 *  \details    -
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_Init(void);

/**********************************************************************************************************************
  SoAd_SoCon_UdpChangeParameter()
**********************************************************************************************************************/
/*! \brief      Changes parameters on a UDP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_UdpChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId);

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
  SoAd_SoCon_TcpChangeParameter()
**********************************************************************************************************************/
/*! \brief      Changes parameters on a TCP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId);
#endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndSetRemoteAddr()
 *********************************************************************************************************************/
/*! \brief      Checks a remote address and set it on a socket connection if remote address is valid.
 *  \details    Compares remote address to be set with all related local addresses and overwrites current remote
 *              address if remote address to be set is different to the local addresses.
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  RemoteAddrPtr   Pointer to remote address.
 *  \return     E_OK            Remote address is valid and set.
 *  \return     E_NOT_OK        Remote address is not valid.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_CheckAndSetRemoteAddr(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr);

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief      Updates an already running UDP Alive Supervision Timeout on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateUdpAliveTimeout(
  SoAd_SoConIdxType SoConIdx);
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
/**********************************************************************************************************************
  SoAd_RouteGrp_Init()
**********************************************************************************************************************/
/*! \brief      Initializes routing group structs.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VROUT_GROUP_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RouteGrp_Init(void);
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
  SoAd_Tx_Init()
**********************************************************************************************************************/
/*! \brief      Initializes Tx structs.
 *  \details    -
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Tx_Init(void);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckParameter()
 *********************************************************************************************************************/
/*! \brief        Checks transmission parameter for a IF-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            Parameters are valid for transmission.
 *  \return       E_NOT_OK        Parameters are not valid for transmission.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxPduLength()
 *********************************************************************************************************************/
/*! \brief        Cheks the PDU length of a IF-PDU requested for transmission.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            PDU length is valid.
 *  \return       E_NOT_OK        PDU length is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxPduLength(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxState()
 *********************************************************************************************************************/
/*! \brief        Checks the transmission state for a IF-PDU if transmission is allowed or not.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \return       E_OK            Transmission is allowed.
 *  \return       E_NOT_OK        Transmission is prohibited.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxState(
  SoAd_PduIdxType PduRouteIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits a IF-PDU over all realted PduRouteDests.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \param[out]   ErrorIdPtr          Pointer to error identifier (set if error occurred).
 *  \return       E_OK                SoConId is valid and corresponding PduRouteDest was found.
 *  \return       E_NOT_OK            SoConId is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

# if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduRouteDestIdxByMetaData()
 *********************************************************************************************************************/
/*! \brief        Returns the PduRouteDest of a PduRoute which matches the SoConId in the meta data if configured.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \param[out]   PduRouteDestIdxPtr  Pointer to index of PduRouteDest.
 *  \return       E_OK                SoConId is valid and corresponding PduRouteDest was found.
 *  \return       E_NOT_OK            SoConId is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES & SOAD_VMETA_DATA_TX_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduRouteDestIdxByMetaData(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestIdxPtr);
# endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPduRouteDest()
 *********************************************************************************************************************/
/*! \brief      Transmits a IF-PDU on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  PduInfoPtr      Pointer to PDU.
 *  \param[in]  PduRouteDestPtr Pointer to PduRouteDest.
 *  \return     E_OK            Transmit request was accepted.
 *  \return     E_NOT_OK        Transmit request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connections indexes, FALSE for same socket socket connection index.
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPduRouteDest(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr);

/**********************************************************************************************************************
  SoAd_TxIf_AddPendingConf()
**********************************************************************************************************************/
/*! \brief      Adds a pending TxConfirmation for IF-API to a specific socket connection.
 *  \details    Handles TxConfirmations generated by SoAd itself and called in context of SoAd_TxConfirmation.
 *  \param[in]  TxPduIdx    Tx PDU index.
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConf(
  SoAd_PduIdxType TxPduIdx,
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckParameter()
 *********************************************************************************************************************/
/*! \brief        Checks transmission parameter for a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            Parameters are valid for transmission.
 *  \return       E_NOT_OK        Parameters are not valid for transmission.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits a TP-PDU over the realted PduRouteDest.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \return       E_OK                Transmission request was successful.
 *  \return       E_NOT_OK            Transmission request was not successful.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckAndSetProtocolParameter()
 *********************************************************************************************************************/
/*! \brief        Checks and sets protocol specific parameters for a transmission of a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            Parameters are valid and set for transmission.
 *  \return       E_NOT_OK        Parameters are not valid and therefore not set for transmission.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckAndSetProtocolParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_PrepareTransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Prepares socket connection to perform a transmission of a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *  \param[in]    PduLength       PDU length.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxTp_PrepareTransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength);
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_GetLocalSockAddr()
 *********************************************************************************************************************/
/*! \brief        Returns local socket address (local IP address and port) of a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx            Socket connection index.
 *  \param[out]   LocalSockAddrPtr    Pointer to local socket address.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_VVERIFY_RX_PDU_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Util_GetLocalSockAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_DATA) LocalSockAddrPtr);
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_GetRemoteSockAddr()
 *********************************************************************************************************************/
/*! \brief        Returns remote socket address (remote IP address and port) of a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx            Socket connection index.
 *  \param[out]   RemSockAddrPtr      Pointer to remote socket address.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \config       SOAD_VVERIFY_RX_PDU_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Util_GetRemoteSockAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_DATA) RemSockAddrPtr);
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*! \brief      Compares two IP addresses.
 *  \details    -
 *  \param[in]  Domain          IP domain (IPv4/6).
 *  \param[in]  IpAddrAPtr      Pointer to first IP address.
 *  \param[in]  IpAddrBPtr      Pointer to second IP address.
 *  \param[in]  AllowWildcards  Flag to indicate if wildcards are allowed in first IP address.
 *  \return     E_OK            IP addresses are equal.
 *  \return     E_NOT_OK        IP addresses are not equal.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  SoAd_IpAddrConstPtrType IpAddrAPtr,
  SoAd_IpAddrConstPtrType IpAddrBPtr,
  boolean AllowWildcards);

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_CompareSockAddr()
 *********************************************************************************************************************/
/*! \brief      Compares two socket addresses.
 *  \details    -
 *  \param[in]  SockAddrAPtr    Pointer to first socket address.
 *  \param[in]  SockAddrBPtr    Pointer to second socket address.
 *  \param[in]  AllowWildcards  Flag to indicate if wildcards are allowed in first socket address.
 *  \return     E_OK            Socket addresses are equal.
 *  \return     E_NOT_OK        Socket addresses are not equal.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareSockAddr(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrAPtr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrBPtr,
  boolean AllowWildcards);
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_InitSockIdxStructs()
 *********************************************************************************************************************/
/*! \brief        Initializes module internal socket index structs.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_InitSockIdxStructs(void);

#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
  SoAd_TcpTxQueueReserveNextElement()
**********************************************************************************************************************/
/*! \brief      Reserves next free TCP Tx queue entry to store a transmission.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \param[in]  TxPduIdx    Tx PDU index.
 *  \return     E_OK        Element reserved.
 *  \return     E_NOT_OK    No elements left or an element is already reserved.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpTxQueueReserveNextElement(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx);

/**********************************************************************************************************************
  SoAd_TcpTxQueueSetElement()
**********************************************************************************************************************/
/*! \brief      Sets a previously reserved element in TCP Tx queue.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \param[in]  TxPduIdx    Tx PDU index.
 *  \param[in]  Len         Length of transmission data.
 *  \param[in]  Release     Release or set the reserved element.
 *  \pre        An element has to be reserved with SoAd_TcpTxQueueReserveNextElement().
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TcpTxQueueSetElement(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx,
  PduLengthType Len,
  boolean Release);
# endif /* SOAD_VTCP_USED == STD_ON */

# if ( (SOAD_VTX_BUF_ENABLED == STD_ON) || (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_ReconnectSockCon()
**********************************************************************************************************************/
/*! \brief      Reconnects (SOAD_SOCON_RECONNECT) a socket connection if automatic setup and wildcards are configured.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        Socket connection has to be in mode online (SOAD_SOCON_ONLINE).
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE otherwise.
 *  \config     SOAD_VPDU_ROUTES & (SOAD_VTX_BUF_ENABLED | SOAD_VUDP_IMMED_IF_TX_CONF)
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_ReconnectSockCon(
  SoAd_SoConIdxType SoConIdx);
# endif /* (SOAD_VTX_BUF_ENABLED == STD_ON) || (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) */

# if ( (SOAD_VPDU_HDR_ENABLED == STD_ON) && (SOAD_VTX_DYN_LEN_ENABLED == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_UpdatePduHdrLen()
**********************************************************************************************************************/
/*! \brief      Updates length of a PDU header in a buffer.
 *  \details    -
 *  \param[in]  PduHeaderLen    Updated PDU header length.
 *  \param[in]  BufPtr          Pointer to buffer where PDU header is stored.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different buffers, FALSE for same buffer.
 *  \config     SOAD_VPDU_ROUTES & SOAD_VPDU_HDR_ENABLED & SOAD_VTX_DYN_LEN_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_UpdatePduHdrLen(
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr);
# endif /* (SOAD_VPDU_HDR_ENABLED == STD_ON) && (SOAD_VTX_DYN_LEN_ENABLED == STD_ON) */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_CloseSockCon()
 *********************************************************************************************************************/
/*! \brief      Closes a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_CloseSockCon(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_OpenSockCon()
 *********************************************************************************************************************/
/*! \brief      Opens a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_OpenSockCon(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_ResetSockCon()
 *********************************************************************************************************************/
/*! \brief      Resets a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *  \param[in]  ResetRemAddr  Reset remote address or not.
 *  \param[in]  NormalClose   Normal close request or other reason.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetSockCon(
  SoAd_SoConIdxType SoConIdx,
  boolean ResetRemAddr,
  boolean NormalClose);

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpRxIndication()
 *********************************************************************************************************************/
/*! \brief      Receives data on TCP socket without PDU header option.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \param[in]  BufPtr      Pointer to received data.
 *  \param[in]  Length      Length of received data.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length);

#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpPduHdrRxIndication()
 *********************************************************************************************************************/
/*! \brief      Receives data on TCP socket with PDU header option.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \param[in]  BufPtr      Pointer to received data.
 *  \param[in]  Length      Length of received data.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED & SOAD_VPDU_HDR_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpPduHdrRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length);
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
# endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_UdpRxIndication()
 *********************************************************************************************************************/
/*! \brief      Receives data on UDP socket without PDU header option.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \param[in]  BufPtr      Pointer to received data.
 *  \param[in]  Length      Length of received data.
 *  \pre        -
 *  \context    ISR2
 *  \reentrant  TRUE for different sockets (socket connection groups), FALSE for same socket (socket connection group).
 *  \config     SOAD_VSOCKET_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_UdpRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length);

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_UdpPduHdrRxIndication()
 *********************************************************************************************************************/
/*! \brief      Receives data on UDP socket without PDU header option.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  RemoteAddrPtr   Pointer to remote address.
 *  \param[in]  BufPtr          Pointer to received data.
 *  \param[in]  Length          Length of received data.
 *  \pre        -
 *  \context    ISR2
 *  \reentrant  TRUE for different sockets (socket connection groups), FALSE for same socket (socket connection group).
 *  \config     SOAD_VSOCKET_ROUTES & SOAD_VPDU_HDR_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_UdpPduHdrRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length);
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_AddMetaDataToPdu()
 *********************************************************************************************************************/
/*! \brief          Adds meta data to a PDU by copying PDU to a local buffer and copying meta data behind.
 *  \details        -
 *  \param[in]      SoConIdx        Socket connection index.
 *  \param[in]      RouteIdx        Socket route index.
 *  \param[in,out]  PduInfoPtr      Pointer to PDU.
 *  \return         E_OK            Adding meta data succeeded.
 *  \return         E_NOT_OK        Adding meta data failed since local buffer size is not sufficient.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \config         SOAD_VMETA_DATA_RX_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_AddMetaDataToPdu(
  SoAd_SoConIdxType SoConIdx,
  uint16 RouteIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);
#endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Confirms a TCP transmission.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  TxPduIdx        Tx PDU index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \config     SOAD_VPDU_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpTxConfirmation(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx);
# endif /* SOAD_VTCP_USED == STD_ON */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_HandleShutdown()
 *********************************************************************************************************************/
/*! \brief      Handles shutdown at beginning of state main function.
 *  \details    -
 *  \pre        Module has to be in mode SOAD_STATE_SHUTDOWN_IN_PROGRESSS.
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleShutdown(void);

/**********************************************************************************************************************
 *  SoAd_HandleShutdownFinish()
 *********************************************************************************************************************/
/*! \brief      Handles shutdown at end of state main function.
 *  \details    -
 *  \pre        Module has to be in mode SOAD_STATE_SHUTDOWN_IN_PROGRESSS.
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleShutdownFinish(void);

/**********************************************************************************************************************
 *  SoAd_HandleSoConStates()
 *********************************************************************************************************************/
/*! \brief      Handles socket connection states.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleSoConStates(void);

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief      Handles UDP alive supervision timeout on socket connections.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VUDP_ALIVE_TIMEOUT_ENALBED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleUdpAliveTimeout(void);
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTpReception()
 *********************************************************************************************************************/
/*! \brief      Handles TP-PDU reception in main function context.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VSOCKET_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpReception(void);
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTpTransmission()
 *********************************************************************************************************************/
/*! \brief      Handles TP-PDU transmission and in case of BSD Socket API TCP transmission additionally.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpTransmission(void);

/**********************************************************************************************************************
 *  SoAd_HandleTpTransmissionInMain()
 *********************************************************************************************************************/
/*! \brief      Handles TP-PDU transmission in main function context.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpTransmissionInMain(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_HandleTpTransmissionOnSockCon()
 *********************************************************************************************************************/
/*! \brief      Handles TP-PDU transmission on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *  \param[in]  CalledByMain    Called in context of main function or transmit request context.
 *  \return     E_OK            Transmission was successful or is in progress.
 *  \return     E_NOT_OK        Transmission failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connections indexes, FALSE for same socket connection index.
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_HandleTpTransmissionOnSockCon(
  SoAd_SoConIdxType SoConIdx,
  boolean CalledByMain);
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleIfRouteGrpTransmit()
 *********************************************************************************************************************/
/*! \brief      Handles routing group transmissions on IF-PDUs.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleIfRouteGrpTransmit(void);
#endif /* SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleNPduUdpTx()
 *********************************************************************************************************************/
/*! \brief      Handles nPdu transmission and timeout.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleNPduUdpTx(void);

/**********************************************************************************************************************
 *  SoAd_NPduClear()
 *********************************************************************************************************************/
/*! \brief      Initializes nPdu management.
 *  \details    -
 *  \param[in]  NPduUdpTxIdx    NPdu index.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VN_PDU_UDP_TX_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduClear(
  uint16 NPduUdpTxIdx);
#endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_NPduQueueAddElement()
 *********************************************************************************************************************/
/*! \brief      Adds or updates an element (PDU) to a nPdu queue.
 *  \details    -
 *  \param[in]  TxPduIdx                Tx PDU index.
 *  \param[in]  PduRouteDestPtr         Pointer to PduRouteDest.
 *  \param[in]  PduInfoPtr              Pointer to PDU.
 *  \return     SOAD_NPDU_OK            Added or updated element in queue.
 *  \return     SOAD_NPDU_OVFL_NEW      New PDU causes an nPdu size overflow.
 *  \return     SOAD_NPDU_OVFL_UPDATE   Updated PDU causes an nPdu size overflow.
 *  \return     SOAD_NPDU_NOT_OK        Did not add or update element in queue.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_NPduQueueAddElement(
  SoAd_PduIdxType TxPduIdx,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_NPduQueueSend()
 *********************************************************************************************************************/
/*! \brief      Sends an nPdu stored in queue.
 *  \details    -
 *  \param[in]  NPduUdpTxIdx    NPdu index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduQueueSend(
  uint16 NPduUdpTxIdx);
#endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_NPduBufAddElement()
 *********************************************************************************************************************/
/*! \brief      Adds an element (PDU) to a nPdu buffer.
 *  \details    -
 *  \param[in]  TxPduIdx            Tx PDU index.
 *  \param[in]  PduRouteDestPtr     Pointer to PduRouteDest.
 *  \param[in]  PduInfoPtr          Pointer to PDU.
 *  \return     SOAD_NPDU_OK        Added element to queue.
 *  \return     SOAD_NPDU_OVFL_NEW  New PDU causes an nPdu size overflow.
 *  \return     SOAD_NPDU_NOT_OK    Did not add element to queue.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_BUF_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_NPduBufAddElement(
  SoAd_PduIdxType TxPduIdx,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_NPduBufSend()
 *********************************************************************************************************************/
/*! \brief      Sends an nPdu stored in buffer.
 *  \details    -
 *  \param[in]  NPduUdpTxIdx    NPdu index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VN_PDU_UDP_TX_BUF_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduBufSend(
  uint16 NPduUdpTxIdx);
#endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Handles TxConfirmation for IF-PDUs.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_VPDU_ROUTES
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTxConfirmation(void);
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled()
 *********************************************************************************************************************/
/*! \brief      Checks if any routing group is active for a specific socket route destination (SockRouteDest).
 *  \details    -
 *  \param[in]  SockRouteIdx    Socket route index (same index like for destination since multiplicity is:
 *                              SockRoute 1:1 SockRouteDest).
 *  \return     TRUE            At least one routing group is enabled.
 *  \return     FALSE           No routing group is enabled.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VROUT_GROUP_ENABLED & SOAD_VROUT_GROUP_RX_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(
  uint16 SockRouteIdx);
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled()
 *********************************************************************************************************************/
/*! \brief      Checks if any routing group is active for a specific PduRouteDest.
 *  \details    -
 *  \param[in]  PduRouteDest    Pointer to PduRouteDest.
 *  \return     TRUE            At least one routing group is enabled.
 *  \return     FALSE           No routing group is enabled.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_VROUT_GROUP_ENABLED & SOAD_VROUT_GROUP_TX_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled(
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDest);
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_GetSoConIdByBestMatchAlg()
 *********************************************************************************************************************/
/*! \brief        Returns a socket connection of a socket connection group or subset using the best match algorithm.
 *  \details      -
 *  \param[in]    DynAddr             Pointer to the dynamic address the configured addresses shall be compared to.
 *  \param[in]    SoConIdxStart       First socket connection index used for best match algorithm.
 *  \param[in]    SoConIdxMax         Last socket connection index used for best match algorithm.
 *  \param[in]    SoConModeFilter     Filter for socket connection state.
 *  \param[in]    NoSockRouteFilter   Filter for socket connections without socket routes or not.
 *  \param[out]   SoConIdxBestMatch   Pointer to socket connection index of the best matching socket connection.
 *  \return       E_OK                Best matching socket connection was found.
 *  \return       E_NOT_OK            Best matching socket connection was not found.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConIdByBestMatchAlg(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DynAddr,
  SoAd_SoConIdxType SoConIdxStart,
  SoAd_SoConIdxType SoConIdxMax,
  SoAd_SoConModeFilterType SoConModeFilter,
  boolean NoSockRouteFilter,
  P2VAR(SoAd_SoConIdxType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxBestMatch);

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_VerifyRxPdu()
 *********************************************************************************************************************/
/*! \brief      Verifies a received PDU by calling a callback with reception dependent information and PDU data.
 *  \details    -
 *  \param[in]  SoConIdx            Socket connection index.
 *  \param[in]  BufIdx              Index of segmented buffer struct.
 *  \param[in]  BufLen              Total length of data in buffer starting at "BufIdx".
 *  \param[in]  PduHdrId            PDU Header ID.
 *  \param[in]  PduHdrLen           PDU Header length.
 *  \return     SOAD_E_OK           Rx PDU verification was successful.
 *  \return     SOAD_E_NOT_OK       Rx PDU verification was not successful.
 *  \return     SOAD_E_PENDING      Rx PDU verification is pending since not enough data have been received yet.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_VVERIFY_RX_PDU_ENABLED
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(SoAd_ReturnType, SOAD_CODE) SoAd_VerifyRxPdu(
  SoAd_SoConIdxType SoConIdx,
  uint32 BufIdx,
  uint32 BufLen,
  SoAd_PduHdrIdType PduHdrId,
  SoAd_PduHdrLenType PduHdrLen);
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_GetSockIdxBySocketId()
 *********************************************************************************************************************/
/*! \brief        Returns the module internal socket index of a socket identifier.
 *  \details      -
 *  \param[in]    SocketId            Socket identifier.
 *  \param[out]   SockIdxPtr          Pointer to socket index.
 *  \return       E_OK                Socket index was found.
 *  \return       E_NOT_OK            Socket index was not found.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSockIdxBySocketId(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockIdxType, AUTOMATIC, SOAD_APPL_DATA) SockIdxPtr);

/**********************************************************************************************************************
 *  SoAd_ResetIpAddr()
 *********************************************************************************************************************/
/*! \brief      Resets the remote address of a socket connection to the configured value.
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetIpAddr(
  SoAd_SoConIdxType SoConIdx);

/**********************************************************************************************************************
 *  SoAd_ResetIpAddr2Unspec()
 *********************************************************************************************************************/
/*! \brief          Resets a remote address to "unspecified".
 *  \details        -
 *  \param[in,out]  IpAddrPtr   Pointer to socket address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetIpAddr2Unspec(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr);

/* PRQA L:FCT_LIKE_MACROS */
/* PRQA L:MACRO_LIMIT */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/* PRQA S 0306,0310,3305 TYPE_CAST */ /* MD_SoAd_11.4 */

/**********************************************************************************************************************
 *  SoAd_EventQueue_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_EventQueue_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize management struct of all event queues. */
  for ( idx = 0u; idx < SoAd_EventQueueTotal(); idx++ )
  {
    SoAd_EventQueues(idx).EveMgtPtr->Idx = 0u;
    SoAd_EventQueues(idx).EveMgtPtr->Lvl = 0u;
  }
  /* #20 Initialize event queue flags. */
  for ( idx = 0u; idx < SoAd_EventQueueFlagsArraySize(); idx++ )
  {
    SoAd_EventQueueFlags(idx) = 0u;
  }
}

/**********************************************************************************************************************
 *  SoAd_EventQueue_AddElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_EventQueue_AddElement(
  uint8 EveQueIdx,
  uint16 HandleIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if event is not yet in queue. */
  if ( (SoAd_EventQueueFlags(HandleIdx) & SoAd_EventQueues(EveQueIdx).EveQueBitPattern) == 0u )
  { /* event not yet in queue */
    uint16 writeIdx;

    /* #100 Get next queue index. */
    if ( (SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx + SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl) >=
      SoAd_EventQueues(EveQueIdx).EveQueSize )
    { /* wrap around */
      writeIdx = SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl -
        (SoAd_EventQueues(EveQueIdx).EveQueSize - SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx);
    }
    else
    { /* no wrap around */
      writeIdx = SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx + SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl;
    }

    /* #101 Set new queue entry. */
    if ( SoAd_EventQueues(EveQueIdx).EveQueType == SOAD_QUEUE_LIST_TYPE_UINT8 )
    {
      ((SOAD_P2VAR(uint8))SoAd_EventQueues(EveQueIdx).EveQuePtr)[writeIdx] = (uint8)HandleIdx;
    }
    else
    {
      ((SOAD_P2VAR(uint16))SoAd_EventQueues(EveQueIdx).EveQuePtr)[writeIdx] = HandleIdx;
    }
    SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl++;

    /* #102 Set flag to indicate that handle index is in queue. */
    SoAd_EventQueueFlags(HandleIdx) |= SoAd_EventQueues(EveQueIdx).EveQueBitPattern;
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
}

/**********************************************************************************************************************
 *  SoAd_EventQueue_GetNextElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_EventQueue_GetNextElement(
  uint8 EveQueIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if at least one event is in queue. */
  if ( SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl > 0u )
  {
    /* #100 Get next element. */
    if ( SoAd_EventQueues(EveQueIdx).EveQueType == SOAD_QUEUE_LIST_TYPE_UINT8 )
    {
      *HandleIdxPtr = ((SOAD_P2VAR(uint8))SoAd_EventQueues(EveQueIdx).EveQuePtr)[
        SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx];
    }
    else
    {
      *HandleIdxPtr = ((SOAD_P2VAR(uint16))SoAd_EventQueues(EveQueIdx).EveQuePtr)[
        SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx];
    }

    /* #101 Remove element from queue. */
    if ( (SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx + 1u) == SoAd_EventQueues(EveQueIdx).EveQueSize)
    { /* wrap around */
      SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx = 0u;
    }
    else
    { /* no wrap around */
      SoAd_EventQueues(EveQueIdx).EveMgtPtr->Idx++;
    }
    SoAd_EventQueues(EveQueIdx).EveMgtPtr->Lvl--;

    /* #102 Reset event flag for handle index. */
    SoAd_EventQueueFlags(*HandleIdxPtr) &= (SoAd_EventQueueFlagType)(~SoAd_EventQueues(EveQueIdx).EveQueBitPattern);

    retVal = E_OK;
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

  return retVal;
}

#if ( (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_TimeoutList_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TimeoutList_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 timeListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all timeout lists. */
  for ( timeListIdx = 0u; timeListIdx < SoAd_TimeoutListTotal(); timeListIdx++ )
  {
    /* #100 Reset management struct. */
    SoAd_TimeoutLists(timeListIdx).TimeMgtPtr->Lvl = 0u;
    /* #101 Check if timeout list type is uint8. */
    if ( SoAd_TimeoutLists(timeListIdx).TimeListType == SOAD_QUEUE_LIST_TYPE_UINT8 )
    {
      uint8 mapIdx;

      /* #1010 Clear timeout list for uint8. */
      for ( mapIdx = 0u; mapIdx < SoAd_TimeoutLists(timeListIdx).TimeListIdxMapSize; mapIdx++ )
      {
        ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(timeListIdx).TimeListIdxMapPtr)[mapIdx] = SOAD_INV_HND_8;
      }
    }
    /* #102 Check if timeout list type is uint16. */
    else
    {
      uint16 mapIdx;

      /* #1020 Clear timeout list for uint16. */
      for ( mapIdx = 0u; mapIdx < SoAd_TimeoutLists(timeListIdx).TimeListIdxMapSize; mapIdx++ )
      {
        ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(timeListIdx).TimeListIdxMapPtr)[mapIdx] = SOAD_INV_HND_16;
      }
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_TimeoutList_SetElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_SetElement(
  uint8 TimeListIdx,
  uint16 HandleIdx,
  uint32 Timeout,
  uint32 TimerCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if timeout list type is uint8. */
  if ( SoAd_TimeoutLists(TimeListIdx).TimeListType == SOAD_QUEUE_LIST_TYPE_UINT8 )
  {
    uint8 mapIdx = ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #100 Check if handle is not yet in list. */
    if ( mapIdx == SOAD_INV_HND_8 )
    {
      /* #1000 Check if list size is sufficient for new element. */
      if ( SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl < SoAd_TimeoutLists(TimeListIdx).TimeListSize )
      {
        /* #10000 Add handle to list. */
        ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl].HandleIdx = (uint8)HandleIdx;
        ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl].Timeout = (uint32)(TimerCounter + Timeout);

        ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx] =
          (uint8)SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl;

        SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl++;

        retVal = E_OK;
      }
    }
    /* #101 Check if handle is in list. */
    else
    {
      /* #1010 Update timeout */
      ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
        mapIdx].Timeout = (uint32)(TimerCounter + Timeout);

      retVal = E_OK;
    }
  }
  /* #20 Check if timeout list type is uint16. */
  else
  {
    uint16 mapIdx = ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #200 Check if handle is not yet in list. */
    if ( mapIdx == SOAD_INV_HND_16 )
    {
      /* #2000 Check if list size is sufficient for new element. */
      if ( SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl < SoAd_TimeoutLists(TimeListIdx).TimeListSize )
      { /* list size is sufficient for new element */
        /* #20000 Add handle to list. */
        ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl].HandleIdx = HandleIdx;
        ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl].Timeout = (uint32)(TimerCounter + Timeout);

        ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx] =
          SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl;

        SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl++;

        retVal = E_OK;
      }
    }
    /* #201 Check if handle is in list. */
    else
    {
      /* #2010 Update timeout. */
      ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
        mapIdx].Timeout = (uint32)(TimerCounter + Timeout);

      retVal = E_OK;
    }
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TimeoutList_GetCurrentTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_GetCurrentTimeout(
  uint8 TimeListIdx,
  uint16 HandleIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) TimeoutPtr,
  uint32 TimerCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if timeout list type is uint8. */
  if ( SoAd_TimeoutLists(TimeListIdx).TimeListType == SOAD_QUEUE_LIST_TYPE_UINT8 )
  {
    uint8 mapIdx = ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #100 Check if handle is in list. */
    if ( mapIdx != SOAD_INV_HND_8 )
    {
      /* #1000 Get current timeout value. */
      uint32 currentTimeout = ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
        mapIdx].Timeout;

      if ( currentTimeout >= TimerCounter )
      { /* no timer counter overflow */
        *TimeoutPtr = (uint32)(currentTimeout - TimerCounter);
      }
      else
      { /* timer counter overflow */
        *TimeoutPtr = (uint32)(currentTimeout + (SOAD_MAX_TIMEOUT_32 - TimerCounter));
      }

      retVal = E_OK;
    }
  }
  /* #20 Check if timeout list type is uint16. */
  else
  {
    uint16 mapIdx = ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #200 Check if handle is in list. */
    if ( mapIdx != SOAD_INV_HND_16 )
    {
      /* #2000 Get current timeout value. */
      uint32 currentTimeout = ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
        mapIdx].Timeout;

      if ( currentTimeout >= TimerCounter )
      { /* no timer counter overflow */
        *TimeoutPtr = (uint32)(currentTimeout - TimerCounter);
      }
      else
      { /* timer counter overflow */
        *TimeoutPtr = (uint32)(currentTimeout + (SOAD_MAX_TIMEOUT_32 - TimerCounter));
      }

      retVal = E_OK;
    }
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TimeoutList_CheckElements()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_CheckElements(
  uint8 TimeListIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) ElementIdxPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr,
  uint32 TimerCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint16_least    elementIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if timeout list type is uint8. */
  if ( SoAd_TimeoutLists(TimeListIdx).TimeListType == SOAD_QUEUE_LIST_TYPE_UINT8 )
  {
    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 Search for timeout. */
    for ( elementIdx = *ElementIdxPtr; elementIdx < SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl; elementIdx++ )
    {
      if ( ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[elementIdx].Timeout ==
        TimerCounter )
      {
        /* #1000 Return first occurred timeout. */
        *ElementIdxPtr = (uint16)elementIdx;
        *HandleIdxPtr = ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          elementIdx].HandleIdx;
        retVal = E_OK;
        break;
      }
    }

    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
  }
  /* #20 Check if timeout list type is uint16. */
  else
  {
    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #200 Search for timeout. */
    for ( elementIdx = *ElementIdxPtr; elementIdx < SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl; elementIdx++ )
    {
      if ( ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[elementIdx].Timeout ==
        TimerCounter )
      { /* #2000 Return first occurred timeout. */
        *ElementIdxPtr = (uint16)elementIdx;
        *HandleIdxPtr = ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr)[
          elementIdx].HandleIdx;
        retVal = E_OK;
        break;
      }
    }

    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
  }
  return retVal;
}
/* PRQA L:MSR_STCYC */

/**********************************************************************************************************************
 *  SoAd_TimeoutList_RemoveElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TimeoutList_RemoveElement(
  uint8 TimeListIdx,
  uint16 HandleIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if timeout list type is uint8. */
  if ( SoAd_TimeoutLists(TimeListIdx).TimeListType == SOAD_QUEUE_LIST_TYPE_UINT8 )
  {
    uint8 elementIdx = ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #100 Check if handle index has an element. */
    if ( elementIdx != SOAD_INV_HND_8 )
    {
      uint16 lastIdx;

      /* #1000 Remove element of handle index. */
      SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl--;
      ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx] = SOAD_INV_HND_8;

      /* get index of last element in list (lvl is already decremented so lvl is last index) */
      lastIdx = SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl;

      /* #1001 Check if list has more elements. */
      if ( (lastIdx > 0u) && (lastIdx != elementIdx) )
      {
        /* #10010 Replace removed element by last element. */
        SOAD_P2VAR(SoAd_TimeoutListElem8Type) timeListPtr =
          ((SOAD_P2VAR(SoAd_TimeoutListElem8Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr);

        /* replace at index */
        timeListPtr[elementIdx].HandleIdx = timeListPtr[lastIdx].HandleIdx;
        timeListPtr[elementIdx].Timeout = timeListPtr[lastIdx].Timeout;

        /* update map of replacing element */
        ((SOAD_P2VAR(uint8))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[timeListPtr[lastIdx].HandleIdx] =
          elementIdx;
      }
    }
  }
  /* #20 Check if timeout list type is uint16. */
  else
  {
    uint16 elementIdx = ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx];

    /* #200 Check if handle index has an element. */
    if ( elementIdx != SOAD_INV_HND_16 )
    {
      uint16 lastIdx;

      /* #2000 Remove element of handle index. */
      SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl--;
      ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[HandleIdx] = SOAD_INV_HND_16;

      /* get index of last element in list (lvl is already decremented so lvl is last index) */
      lastIdx = SoAd_TimeoutLists(TimeListIdx).TimeMgtPtr->Lvl;

      /* #2001 Check if list has more elements. */
      if ( (lastIdx > 0u) && (lastIdx != elementIdx) )
      {
        /* #20010 Replace removed element by last element. */
        SOAD_P2VAR(SoAd_TimeoutListElem16Type) timeListPtr =
          ((SOAD_P2VAR(SoAd_TimeoutListElem16Type))SoAd_TimeoutLists(TimeListIdx).TimeListPtr);

        /* replace at index */
        timeListPtr[elementIdx].HandleIdx = timeListPtr[lastIdx].HandleIdx;
        timeListPtr[elementIdx].Timeout = timeListPtr[lastIdx].Timeout;

        /* update map of replacing element */
        ((SOAD_P2VAR(uint16))SoAd_TimeoutLists(TimeListIdx).TimeListIdxMapPtr)[timeListPtr[lastIdx].HandleIdx] =
          elementIdx;
      }
    }
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
}
#endif /* (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) */

/**********************************************************************************************************************
 *  SoAd_TcpIp_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_Init(void)
{
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType_least idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all local addresses. */
  for ( idx = 0u; idx < SOAD_VADDR_ID_TOTAL; idx++ )
  {
    /* #100 Add element initially to local address event queue if assignment trigger is automatic. */
    if ( SoAd_LocalAddr(idx).AssignTrigger == SOAD_ASSIGN_TRIGGER_AUTOMATIC )
    {
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), idx);
    }
    /* #101 Initialize local address structs. */
    SoAd_AddrId2LocalIpAddrReqState(idx) = SOAD_LOCAL_IP_ADDR_REQ_STATE_NONE;
    SoAd_TcpIpBsd_ResetLocalIpAddr(idx);
  }

  /* #20 Iterate over all module internal socket indexes. */
  for ( idx = 0u; idx < SoAd_TcpIpSocketCnt(); idx++ )
  {
    /* #200 Initialize module internal socket index structs. */
    SoAd_SockIdx2RxMgt(idx).Len     = 0u;
    SoAd_SockIdx2RxMgt(idx).Offset  = 0u;
  }

# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
  /* #30 Initialize socket transmission buffers. */
  SoAd_TcpIpBsd_InitSocketTxBuffer();
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType  sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  Std_ReturnType    retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Request IP address in case of BSD Socket API. */
  SoAd_LocalAddrIdType addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;

  if ( Type == SoAd_LocalAddr(addrId).AssignType )
  {
    retVal = SoAd_TcpIpBsd_RequestIpAddrAssignment(
      SoConIdx,
      Type,
      LocalIpAddrPtr,
      Netmask,
      DefaultRouterPtr);
  }
  else
  {
    retVal = E_NOT_OK;
  }
#else
  /* #20 Request IP address in case of AUTOSAR Socket API. */
  retVal = SoAd_TcpIpAsr_RequestIpAddrAssignment(
    SoConIdx,
    Type,
    LocalIpAddrPtr,
    Netmask,
    DefaultRouterPtr);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  /* #30 Update local port of local static IP address if request was successful. */
  if ( retVal == E_OK )
  {
    if ( SoAd_SockIdx2LocPortDyn(sockIdx) == SOAD_PORT_ANY )
    {
      if ( Type == SOAD_IPADDR_ASSIGNMENT_STATIC )
      {
        SoAd_SockIdx2LocPortDyn(sockIdx) = ((SoAd_SockAddrInetXType*)LocalIpAddrPtr)->port;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ReleaseIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;;
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
  Std_ReturnType        retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request locally in case of BSD Socket API. */
  retVal = E_NOT_OK;
  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;

  if ( SoAd_SockConIdx2IpAddrState(SoConIdx) == SOAD_IPADDR_STATE_ASSIGNED )
  {
    /* #100 Allow release on multicast addresses and mark local address to be released in main function. */
    if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_MULTICAST )
    {
      SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_RELEASE;

      retVal = E_OK;

      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
    }
  }
  else
  {
    /* #101 Allow reverting assignment request of a not yet assigned local addresss. */
    if ( SoAd_SockConIdx2IpAddrState(SoConIdx) == SOAD_IPADDR_STATE_UNASSIGNED )
    { /* IP address is unassigned */
      /* reset previous request */
      SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_NONE;

      retVal = E_OK;
    }
  }
#else
  /* #20 Forward request to TcpIp module in case of AUTOSAR Socket API. */
  retVal = TcpIp_ReleaseIpAddrAssignment(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_GetIpAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
#else
  SoAd_SockIdxType      sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request for BSD Socket API. */

  /* #100 Copy socket address from local data structure. */
  *NetmaskPtr = SoAd_AddrId2LocalIpAddrDyn(addrId).netmask;
  LocalAddrPtr->domain = SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain;
  ((SoAd_SockAddrInetXType*)LocalAddrPtr)->port = SoAd_SockIdx2LocPortDyn(SoAd_SockCons(SoConIdx).SockIdx);
  ((SoAd_SockAddrInetXType*)LocalAddrPtr)->addr[0] = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
  {
    ((SoAd_SockAddrInetXType*)LocalAddrPtr)->addr[1] = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[1];
    ((SoAd_SockAddrInetXType*)LocalAddrPtr)->addr[2] = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[2];
    ((SoAd_SockAddrInetXType*)LocalAddrPtr)->addr[3] = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[3];
  }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
#else
  /* #20 Handle request for AUTOSAR Socket API. */

  /* #200 Get local IP address from TcpIp module. */
  if ( E_OK != TcpIp_GetIpAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId,
      (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))LocalAddrPtr,
      NetmaskPtr,
      (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))DefaultRouterPtr) )
  {
    /* #2000 Use wildcard IP address if call to TcpIp module fail. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
    ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[0] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[1] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[2] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[3] = SOAD_IP6ADDR_ANY;
# else
    ((SoAd_SockAddrInetType*)LocalAddrPtr)->addr[0] = SOAD_IPADDR_ANY;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  }

  /* #201 Get port from local data structure. */
  ((SoAd_SockAddrInetXType*)LocalAddrPtr)->port = SoAd_SockIdx2LocPortDyn(sockIdx);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_CompareToIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_CompareToIpAddr(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrConstPtrType IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request for BSD Socket API. */
  SoAd_LocalAddrIdType addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;

  /* #100 Compare IP address to local IP address stored locally. */
  if ( E_OK == SoAd_Util_CompareIpAddr(
      SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
      IpAddrPtr,
      SOAD_A_P2CONST(SoAd_IpAddrType)SoAd_AddrId2LocalIpAddrDyn(addrId).addr,
      FALSE) )
  {
    retVal = E_OK;
  }
#else
  /* #20 Handle request for AUTOSAR Socket API. */
  uint8_least tcpIpAddrIdx;
  uint8       tcpIpCtrl = SoAd_AddrId2TcpIpCtrl(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId);

  /* #200 Iterate over all local addresses of the TcpIp controller. */
  for ( tcpIpAddrIdx = 0u; tcpIpAddrIdx < SoAd_TcpIpCtrl(tcpIpCtrl).AddrIdListNum; tcpIpAddrIdx++ )
  {
    SoAd_SockAddrInetXType localIpAddr;
    SoAd_SockAddrInetXType defaultRouter;
    uint8 netmask;

    /* #2000 Get local address from TcpIp. */
    if ( E_OK == TcpIp_GetIpAddr(
        SoAd_TcpIpCtrl(tcpIpCtrl).AddrIdListPtr[tcpIpAddrIdx],
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&localIpAddr,
        &netmask,
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&defaultRouter) )
    {
      /* #20000 Compare IP address to local IP address. */
      if ( E_OK == SoAd_Util_CompareIpAddr(
          SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
          localIpAddr.addr,
          IpAddrPtr,
          FALSE) )
      {
        /* #200000 Stop iteration if a local IP address matches. */
        retVal = E_OK;
        break;
      }
    }
    /* #2001 Ignore local IP address if call to TcpIp failed. */
    else
    {
      /* nothing to do */
    }
  }
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetPhysAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_GetPhysAddr(
  SoAd_SoConIdType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  SoAd_SockIdxType      sockIdx;
  SoAd_SocketIdType     socketId;
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
  Std_ReturnType        retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request for BSD Socket API. */
  retVal = E_NOT_OK;

  /* #100 Get socket handle dependent of the socket type. */
  sockIdx = SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx;
# if ( SOAD_VTCP_USED == STD_ON )
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
  {
    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate == TRUE )
    {
      sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
    }
  }
# endif /* SOAD_VTCP_USED == STD_ON */
  socketId = SoAd_SockIdx2SocketId(sockIdx);

  if ( socketId != SOAD_INV_SOCKET_ID ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  {
    /* #101 Get interface name which is used to call BSD Socket API later. */
    char ifName[IF_NAMESIZE + 1] = {0}; /* PRQA S 5013 */ /* MD_SoAd_6.3 */

    if ( if_indextoname(SoAd_AddrId2LocalIpAddrDyn(addrId).ifcIdx, ifName) != 0 )
    {
      struct ifreq ifr;
      uint16 ifNameLen;
      uint16 idx;

      for ( idx = 0u; idx < (uint16)(IF_NAMESIZE + 1); idx++ )
      {
        if ( ifName[idx] == 0u )
        {
          ifNameLen = idx;
          break;
        }
      }

      if ( ifNameLen < sizeof(ifr.ifr_name) )
      {
        IpBase_Copy((IpBase_CopyDataType*)ifr.ifr_name,
          (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))ifName, ifNameLen);
          ifr.ifr_name[ifNameLen] = 0;

        /* #102 Get MAC address. */
        if ( ioctl(socketId, SIOCGIFHWADDR, &ifr) != SOAD_E_BSD_NOT_OK )
        {
          PhysAddrPtr[0] = (uint8)ifr.ifr_hwaddr.sa_data[0];
          PhysAddrPtr[1] = (uint8)ifr.ifr_hwaddr.sa_data[1];
          PhysAddrPtr[2] = (uint8)ifr.ifr_hwaddr.sa_data[2];
          PhysAddrPtr[3] = (uint8)ifr.ifr_hwaddr.sa_data[3];
          PhysAddrPtr[4] = (uint8)ifr.ifr_hwaddr.sa_data[4];
          PhysAddrPtr[5] = (uint8)ifr.ifr_hwaddr.sa_data[5];

          retVal = E_OK;
        }
      }
    }
  }
#else
  /* #20 Handle request for AUTOSAR Socket API. */

  /* #200 Forward request to TcpIp module. */
  retVal = TcpIp_GetPhysAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, PhysAddrPtr);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_ReadDhcpHostNameOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ReadDhcpHostNameOption(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request for BSD Socket API. */

  /* #100 Retrieve hostname via BSD Socket API. */
  if ( 0u == gethostname((char*)DataPtr, *LengthPtr) ) /* PRQA S 5013 */ /* MD_SoAd_6.3 */
  {
    uint8 idx;

    /* #1000 Determine hostname length. */
    for ( idx = 0u; idx < *LengthPtr; idx++ )
    {
      if ( DataPtr[idx] == 0u )
      {
        *LengthPtr = idx;
      }
    }

    retVal = E_OK;
  }
  /* #101 Report BSD Socket API error if call to BSD Socket API failed. */
  else
  {
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_1(SOAD_SOCK_SID_TCPIP_READ_DHCP_HOST_NAME_OPTION, SOAD_SOCK_BSD_API_GET_HOST_NAME,
      SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(SoConIdx).SoConId, errno);
  }

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#else
  /* #20 Handle request for AUTOSAR Socket API. */
# if ( (SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON) )
#  if ( SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON )
  /* #200 Forward request to TcpIp module for DHCPv6. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
  {
    uint16 tmpLength = *LengthPtr;

    if ( TcpIp_DhcpV6ReadOption(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId,
        SOAD_DHCP_OPT_V6_HOSTNAME, &tmpLength, DataPtr) == E_OK )
    {
      *LengthPtr = (uint8)tmpLength;
      retVal = E_OK;
    }
  }
#  endif /* SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON */
#  if ( SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON )
  /* #201 Forward request to TcpIp module for DHCPv4. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET )
  {
    retVal = TcpIp_DhcpReadOption(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, SOAD_DHCP_OPT_V4_HOSTNAME,
      LengthPtr, DataPtr);
  }
#  endif /* SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON */
# else
  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_VAR(LengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_VAR(DataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* (SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON) */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_WriteDhcpHostNameOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_WriteDhcpHostNameOption(
  SoAd_SoConIdxType SoConIdx,
  uint8 Length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle request for BSD Socket API. */

  /* #100 Set hostname via BSD Socket API. */
  if ( sethostname((SOAD_P2CONST(char))DataPtr, Length) == 0u ) /* PRQA S 5013 */ /* MD_SoAd_6.3 */
  {
    retVal = E_OK;
  }
  /* #101 Report BSD Socket API error if call to BSD Socket API failed. */
  else
  {
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_1(SOAD_SOCK_SID_TCPIP_WRITE_DHCP_HOST_NAME_OPTION,
      SOAD_SOCK_BSD_API_SET_HOST_NAME, SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(SoConIdx).SoConId, errno);
  }

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#else
  /* #20 Handle request for AUTOSAR Socket API. */
# if ( (SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON) )
#  if ( SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON )
  /* #200 Forward request to TcpIp module for DHCPv6. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
  {
    retVal = TcpIp_DhcpV6WriteOption(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, SOAD_DHCP_OPT_V6_HOSTNAME,
      Length, DataPtr);
  }
#  endif /* SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON */
#  if ( SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON )
  /* #201 Forward request to TcpIp module for DHCPv4. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET )
  {
    retVal = TcpIp_DhcpWriteOption(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, SOAD_DHCP_OPT_V4_HOSTNAME,
      Length, DataPtr);
  }
#  endif /* SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON */
# else
  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_VAR(Length); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_CONST_VAR(DataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* (SOAD_VDHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_VDHCPV4_CLIENT_ENABLED == STD_ON)) */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_GetSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_GetSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_DomainType Domain,
  SoAd_ProtocolType Protocol,
  P2VAR(SoAd_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* 10 Forward request to BSD Socket API. */
  retVal = SoAd_TcpIpBsd_GetSocket(SoConIdx, Domain, Protocol, SocketIdPtr);
#else
  /* #20 Forward request to TcpIp module. */
  retVal = TcpIp_SoAdGetSocket(Domain, Protocol, SocketIdPtr);

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_Bind()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_Bind(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* 10 Forward request to BSD Socket API. */
  retVal = SoAd_TcpIpBsd_Bind(SoConIdx, SocketId, PortPtr);
#else
  /* #20 Forward request to TcpIp module. */
  retVal = TcpIp_Bind(SocketId, SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, PortPtr);
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpListen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpListen(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  uint16 MaxChannels)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Forward request to BSD Socket API. */
  if ( listen(SocketId, MaxChannels) != SOAD_E_BSD_NOT_OK )
  {
    /* #100 Add event to corresponding queue to handle socket in main function if call to BSD Socket API succeeded. */
    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx);

    retVal = E_OK;
  }
  else
  {
    /* #101 Report BSD Socket API error if call to BSD Socket API failed. */
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_TCP_LISTEN, SOAD_SOCK_BSD_API_LISTEN,
      SOAD_SOCK_BSD_PARAM_NONE, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);

    retVal = E_NOT_OK;
  }
# else
  /* #20 Forward request to TcpIp module. */
  retVal = TcpIp_TcpListen(SocketId, MaxChannels);

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpConnect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpConnect(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
#  if ( SOAD_VIPV6_ENABLED == STD_ON )
  struct sockaddr_in6 sockAddr;
#  else
  struct sockaddr_in  sockAddr;
#  endif /* SOAD_VIPV6_ENABLED == STD_ON */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
  Std_ReturnType      retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Forward request to BSD Socket API. */
  /* #100 Convert remote address for BSD Socket API. */
#  if ( SOAD_VIPV6_ENABLED == STD_ON )
  if ( RemAddrPtr->domain == SOAD_AF_INET6 )
  {
    (SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_family = AF_INET6;
    (SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_scope_id =
      SoAd_AddrId2LocalIpAddrDyn(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId).ifcIdx;
  }
  else
#  else
  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    (SOAD_A_P2VAR(struct sockaddr_in)&sockAddr)->sin_family = AF_INET;
  }
  SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr(SOAD_A_P2VAR(struct sockaddr)&sockAddr, RemAddrPtr);

  /* #101 Call BSD Socket API. */
  (void)connect(SocketId, SOAD_A_P2VAR(struct sockaddr)&sockAddr, sizeof(sockAddr));

  if ( EINPROGRESS == errno ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
  {
    /* #1010 Add event to corresponding queue to handle socket in main function if call to BSD Socket API succeeded. */
    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), SoAd_SockCons(SoConIdx).SockIdx);

    retVal = E_OK;
  }
  else
  {
    /* #1011 Report BSD Socket API error if call to BSD Socket API failed. */
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_TCP_CONNECT, SOAD_SOCK_BSD_API_CONNECT,
      SOAD_SOCK_BSD_PARAM_NONE, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);

    retVal = E_NOT_OK;
  }
# else
  /* #20 Forward request to TcpIp module. */
  retVal = TcpIp_TcpConnect(SocketId, RemAddrPtr);

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}
#endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIp_ChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_ChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  SoAd_ParamIdType ParameterId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Forward request to BSD Socket API. */
  /* #100 Forward request to BSD Socket API for main socket. */
  retVal = SoAd_TcpIpBsd_ChangeParameter(SoConIdx, SocketId, ParameterId, ParameterValue);

  if ( ParameterId == SOAD_PARAMID_STACK_SPECIFIC )
  {
    if ( (retVal == E_OK) && (SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR) )
    {
      SoAd_LocalAddrIdType addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;

      /* #101 Forward request to BSD Socket API for separate broadcast sockets in case of unicast local address. */
      if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST )
      {
        retVal = SoAd_TcpIpBsd_ChangeParameter(SoConIdx,
          SoAd_SockIdx2MulticastAddr(SoAd_SockCons(SoConIdx).SockIdx).DirLinkSocketId, ParameterId, ParameterValue);
        retVal |= SoAd_TcpIpBsd_ChangeParameter(SoConIdx,
          SoAd_SockIdx2MulticastAddr(SoAd_SockCons(SoConIdx).SockIdx).LimNodeSocketId, ParameterId, ParameterValue);
      }
      /* #102 Join multicast addresses in case of multicast local address. */
      else
      {
# if ( SOAD_VIPV6_ENABLED == STD_ON )
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
        {
          struct ipv6_mreq mreq;

          mreq.ipv6mr_interface = SoAd_LocalAddr(addrId).IfcIdx;
          *((uint32*)&mreq.ipv6mr_multiaddr.s6_addr[0]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];
          *((uint32*)&mreq.ipv6mr_multiaddr.s6_addr[4]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[1];
          *((uint32*)&mreq.ipv6mr_multiaddr.s6_addr[8]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[2];
          *((uint32*)&mreq.ipv6mr_multiaddr.s6_addr[12]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[3];

          if ( setsockopt(SocketId, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == SOAD_E_BSD_NOT_OK )
          {
            retVal = E_NOT_OK;
            /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
            SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
              SOAD_SOCK_BSD_PARAM_IP_ADD_MEMBERSHIP, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
          }
        }
        else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
        {
          struct ip_mreqn mreq;

          mreq.imr_ifindex = SoAd_LocalAddr(addrId).IfcIdx;
          mreq.imr_address.s_addr = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];
          mreq.imr_multiaddr.s_addr = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];

          if ( setsockopt(SocketId, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == SOAD_E_BSD_NOT_OK )
          {
            retVal = E_NOT_OK;
            /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
            SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
              SOAD_SOCK_BSD_PARAM_IP_ADD_MEMBERSHIP, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
          }
        }
      }
    }
  }
#else
  /* #20 Forward request to TcpIp module. */
  if ( ParameterId < SOAD_PARAMID_STACK_SPECIFIC )
  {
    /* #200 Forward only if parameter identifier is valid for TcpIp module. */
    retVal = TcpIp_ChangeParameter(SocketId, ParameterId, ParameterValue);
  }
  else
  {
    retVal = E_OK;
  }

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_Close()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
  /* PRQA S 3206 4 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_Close(
  SoAd_SocketIdType SocketId,
  boolean Abort,
  SoAd_SockIdxType SockIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SoAd_SoConIdxType firstSoConIdx = SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart;
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Forward request to BSD Socket API. */
  /* #100 Close separate sockets to receive from broadcast addresses. */
# if ( SOAD_VTCP_USED == STD_ON )
  if ( SoAd_SockCons(firstSoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
# endif /* SOAD_VTCP_USED == STD_ON */
  {
    SoAd_LocalAddrIdType addrId = SoAd_SockCons(firstSoConIdx).SockConGrpPtr->LocIpAddrId;

    if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST )
    {
      /* #1000 Close separate socket for directed broadcast/link-local address. */
      if ( close(SoAd_SockIdx2MulticastAddr(SockIdx).DirLinkSocketId) == SOAD_E_BSD_NOT_OK )
      {
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_CLOSE, SOAD_SOCK_BSD_API_CLOSE, SOAD_SOCK_BSD_PARAM_NONE,
          SoAd_SockCons(firstSoConIdx).SoConId, SoAd_SockIdx2MulticastAddr(SockIdx).DirLinkSocketId, errno);
      }
      SoAd_SockIdx2MulticastAddr(SockIdx).DirLinkSocketId = SOAD_INV_SOCKET_ID;

      /* #1001 Close separate socket for limited broadcast/node-local address. */
      if ( close(SoAd_SockIdx2MulticastAddr(SockIdx).LimNodeSocketId) == SOAD_E_BSD_NOT_OK )
      {
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_CLOSE, SOAD_SOCK_BSD_API_CLOSE, SOAD_SOCK_BSD_PARAM_NONE,
          SoAd_SockCons(firstSoConIdx).SoConId, SoAd_SockIdx2MulticastAddr(SockIdx).LimNodeSocketId, errno);
      }
      SoAd_SockIdx2MulticastAddr(SockIdx).LimNodeSocketId = SOAD_INV_SOCKET_ID;
    }
  }

  /* #101 Close main socket. */
  if ( close(SocketId) == SOAD_E_BSD_NOT_OK )
  {
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_CLOSE, SOAD_SOCK_BSD_API_CLOSE, SOAD_SOCK_BSD_PARAM_NONE,
      SoAd_SockCons(firstSoConIdx).SoConId, SocketId, errno);
  }

  /* #102 Call event function to indicate that socket is closed. */
  if ( NULL_PTR != SoAd_SockCons(firstSoConIdx).SockConGrpPtr->SocketUdpPtr )
  {
    SoAd_TcpIpEvent(SocketId, SOAD_UDP_CLOSED);
  }
# if ( SOAD_VTCP_USED == STD_ON )
  else
  {
    SoAd_SockIdx2RxMgt(SockIdx).Len = 0;
    SoAd_SockIdx2RxMgt(SockIdx).Offset = 0;
    SoAd_TcpIpEvent(SocketId, SOAD_TCP_CLOSED);
  }
# endif /* SOAD_VTCP_USED == STD_ON */

  SOAD_DUMMY_STATEMENT_VAR(Abort); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#else
  /* #20 Forward request to TcpIp module. */
  (void)TcpIp_Close(SocketId, Abort);

  SOAD_DUMMY_STATEMENT_VAR(SockIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TriggerAddressResolution()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_TriggerAddressResolution(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Ignore call in case of BSD Socket API. */

  SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# else
  /* #20 Check if any Tx PDU is configured for the socket connection in case of ASR Socket API. */
  if ( SoAd_SockConIdx2PduRouteDestCol(SoConIdx).PduRouteDestListSize != 0u )
  {
    uint8 ctrlIdx = 0u;
    uint8 physAddr[6] = { 0u, 0u, 0u, 0u, 0u, 0u };

    /* #100 Get the corresponding Ethernet interface controller index. */
    if ( E_NOT_OK != TcpIp_GetCtrlIdx(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, &ctrlIdx) )
    {
      /* #1000 Trigger address resolution by getting remote physical address for the Ethernet interface controller. */
      (void)TcpIp_GetRemotePhysAddr(ctrlIdx, SOAD_A_P2VAR(SoAd_SockAddrType)&SoAd_SockConIdx2RemSockDyn(SoConIdx),
        &physAddr[0], TRUE);
    }
  }
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIp_UdpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_UdpTransmit(
  SoAd_SockIdxType SockIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC) RemAddrPtr,
  uint16 AvailableLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Forward request to BSD Socket API. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  struct sockaddr_in6 remAddr;
# else
  struct sockaddr_in remAddr;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */

  retVal = E_NOT_OK;

  /* #20 Convert remote address for BSD Socket API. */
  SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr(SOAD_A_P2VAR(struct sockaddr)&remAddr, RemAddrPtr);

  /* #30 Check if transmission buffer size is sufficient. */
  if ( AvailableLength <= SoAd_SocketTxBufferSize() )
  {
    SoAd_SockTxBufIdxType sockTxBufIdx;

    /* #40 Request socket transmission buffer. */
    if ( E_OK == SoAd_TcpIpBsd_RequestSocketTxBuffer(SockIdx, &sockTxBufIdx) )
    {
    /* #50 Let SoAd copy data to transmission buffer by calling SoAd_CopyTxData. */
# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
      if ( BUFREQ_OK == SoAd_CopyTxData(SocketId, &SoAd_SocketTxBuffer()[0], SOAD_A_P2VAR(uint16)&AvailableLength) )
# else
      if ( BUFREQ_OK == SoAd_CopyTxData(SocketId, &SoAd_SocketTxBuffer()[0], AvailableLength) )
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
      {
        /* #60 Call BSD Socket API for transmission. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
        if ( RemAddrPtr->domain == SOAD_AF_INET6 )
        {
          SoAd_LocalAddrIdType addrId = SoAd_SockCons(
              SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart).SockConGrpPtr->LocIpAddrId;
          remAddr.sin6_scope_id = SoAd_AddrId2LocalIpAddrDyn(addrId).ifcIdx;
        }
# else
        SOAD_DUMMY_STATEMENT_VAR(SockIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
        if ( sendto(SocketId, SOAD_A_P2CONST(void)&SoAd_SocketTxBuffer()[0], AvailableLength, SOAD_ZERO_FLAGS,
            SOAD_A_P2VAR(struct sockaddr)&remAddr, sizeof(remAddr)) != SOAD_E_BSD_NOT_OK )
        {
          retVal = E_OK;
        }
        else
        {
          /* #70 Report BSD Socket API error if call failed. */
          /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
          SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_UDP_TRANSMIT, SOAD_SOCK_BSD_API_SEND_TO,
            SOAD_SOCK_BSD_PARAM_NONE, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
        }
      }

      /* #51 Release socket transmission buffer. */
      SoAd_TcpIpBsd_ReleaseSocketTxBuffer(sockTxBufIdx);
    }
  }
#else
  /* #20 Forward request to TcpIp module. */
  retVal = TcpIp_UdpTransmit(SocketId, NULL_PTR, RemAddrPtr, AvailableLength);

  SOAD_DUMMY_STATEMENT_VAR(SockIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIp_TcpTransmit(
  SoAd_SoConIdxType SoConIdx,
  uint32 AvailableLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
#  if ( SOAD_VPDU_ROUTES == STD_ON )
  /* #10 Forward request to BSD Socket API. */
  retVal = E_NOT_OK;

  SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);

  /* #20 Check if socket transmission buffer size is sufficient. */
  if ( AvailableLength <= SoAd_SocketTxBufferSize() )
  {
    SoAd_SockTxBufIdxType sockTxBufIdx;

    /* #30 Request socket transmission buffer. */
    if ( E_OK == SoAd_TcpIpBsd_RequestSocketTxBuffer(sockIdx, &sockTxBufIdx) )
    {
      /* #40 Let SoAd copy data to socket transmission buffer. */
#   if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
      if ( BUFREQ_OK == SoAd_CopyTxData(socketId, &SoAd_SocketTxBuffer()[sockTxBufIdx],
          SOAD_A_P2VAR(uint16)&AvailableLength) )
#   else
      if ( BUFREQ_OK == SoAd_CopyTxData(socketId, &SoAd_SocketTxBuffer()[sockTxBufIdx],
          AvailableLength) )
#   endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
      {
        /* #50 Call BSD Socket API for transmission if data copied successfully. */
        if ( send(socketId, SOAD_A_P2CONST(void)&SoAd_SocketTxBuffer()[0], AvailableLength, SOAD_ZERO_FLAGS) !=
          SOAD_E_BSD_NOT_OK )
        {
          /* #60 Release socket transmission buffer if call was successful. */
          SoAd_TcpIpBsd_ReleaseSocketTxBuffer(sockTxBufIdx);

          /* #70 Set event to handle TCP confirmation in main function. */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTcpTxSoCon(), SoConIdx);

          retVal = E_OK;
        }
        else
        {
          /* #61 Update length of requested socket transmission buffer for retry if call failed. */
          if ( E_OK == SoAd_TcpIpBsd_UpdateSocketTxBuffer(sockTxBufIdx, (uint16)AvailableLength) )
          {
            /* #80 Set event to handle TCP confirmation in main function if there is an unused buffer left. */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTcpTxSoCon(), SoConIdx);

            retVal = E_OK;
          }
          else
          {
            /* #81 Release socket transmission buffer if requested buffer is last buffer. */
            SoAd_TcpIpBsd_ReleaseSocketTxBuffer(sockTxBufIdx);
          }
        }
      }
      else
      {
        /* #51 Release socket transmission buffer if data could not be copied. */
        SoAd_TcpIpBsd_ReleaseSocketTxBuffer(sockTxBufIdx);
      }
    }
  }
#  endif /* SOAD_VPDU_ROUTES == STD_ON */
# else
  /* #11 Forward request to TcpIp module. */
  SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(SoAd_SockCons(SoConIdx).SockIdx);

  retVal = TcpIp_TcpTransmit(socketId, NULL_PTR, AvailableLength, TRUE);
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_TcpReceived()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_TcpReceived(
  SoAd_SocketIdType SocketId,
  uint32 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SoAd_SockIdxType sockIdx = SOAD_INV_SOCK_IDX;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Release local reception ring buffer considering wrap around in case of BSD Socket API. */
  (void)SoAd_GetSockIdxBySocketId(SocketId, &sockIdx);
  if ( (SoAd_SockIdx2RxMgt(sockIdx).Offset + Length) > SoAd_SockIdx2RxBuf(sockIdx).MaxLen )
  {
    SoAd_SockIdx2RxMgt(sockIdx).Offset -= SoAd_SockIdx2RxBuf(sockIdx).MaxLen - Length;
  }
  else
  {
    SoAd_SockIdx2RxMgt(sockIdx).Offset += Length;
  }
  SoAd_SockIdx2RxMgt(sockIdx).Len -= Length;
# else
  /* #20 Forward request to TcpIp module. */
  (void)TcpIp_TcpReceived(SocketId, Length);
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}
#endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionRx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionRx(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle socket reception in case of BSD Socket API. */
  SoAd_TcpIpBsd_HandleSocketReception();
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionState(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  /* #10 Handle IP addres states in case of BSD Socket API. */
  SoAd_TcpIpBsd_HandleIpAddrStates();

  /* #20 Handle TCP socket states in case of BSD Socket API. */
# if ( SOAD_VTCP_USED == STD_ON )
  SoAd_TcpIpBsd_HandleTcpSocketStates();
# endif /* SOAD_VTCP_USED == STD_ON */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TcpIp_MainFunctionTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIp_MainFunctionTx(void)
{
#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  uint16                  handleIdx = SOAD_INV_SO_CON_IDX;
  SoAd_SoConIdxType       lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTcpTxSoCon()).EveMgtPtr->Lvl;
  SoAd_SoConIdxType_least idx;

  /* ----- Implementation ----------------------------------------------- */
#   if ( SOAD_VEVE_QUE_LIMIT_TCPTXSOCON_ENALBED == STD_ON )
  /* #10 Get TCP Tx socket connection event queue limit. */
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxTcpTxSoCon()).EveQueLimit )
  {
    lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTcpTxSoCon()).EveQueLimit;
  }
#   endif /* SOAD_VEVE_QUE_LIMIT_TCPTXSOCON_ENALBED == STD_ON */

  /* #20 Iterate over all queue elements which have to be handled in this context. */
  for ( idx = 0u; idx < lvl; idx++ )
  {
    /* #30 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxTcpTxSoCon(), &handleIdx) == E_OK )
    {
      SoAd_SoConIdxType soConIdx = (SoAd_SoConIdxType)handleIdx;

      /* #40 Handle pending TCP Tx Confirmation on socket connection. */
      SoAd_TcpIpBsd_HandleTcpTxConfirmation(soConIdx);
    }
    /* #31 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }

  /* #50 Handle socket transmission buffers. */
  SoAd_TcpIpBsd_HandleSocketTxBuffer();

#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
}

#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_LocalAddrIdType addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  Std_ReturnType       retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if local IP address is already assigned. */
  if ( SoAd_SockConIdx2IpAddrState(SoConIdx) == SOAD_IPADDR_STATE_ASSIGNED )
  {
    /* #100 If local IP address is assigned check if local IP address is of assignment type static. */
    if ( Type == SOAD_IPADDR_ASSIGNMENT_STATIC )
    {
      /* #1000 Accept request for same IP address. */
      if ( E_OK == SoAd_Util_CompareIpAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
          (SOAD_A_P2VAR(SoAd_SockAddrInetXType)LocalIpAddrPtr)->addr, SoAd_AddrId2LocalIpAddrDyn(addrId).addr,
          FALSE) )
      {
        retVal = E_OK;
      }
    }
  }
  else
  {
    /* #101 If local IP address is not assigned check if assignment trigger is manual. */
    if ( SoAd_LocalAddr(addrId).AssignTrigger == SOAD_ASSIGN_TRIGGER_MANUAL )
    {
      /* #1010 Set static IP address. */
      if ( SOAD_IPADDR_ASSIGNMENT_STATIC == Type )
      {
        /* #10100 Set IPv6 address if valid. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
        if ( SOAD_AF_INET6 == SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain )
        {
          if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_MULTICAST )
          { /* multicast address */
            /* check if address is a valid multicast address */
            if ( SOAD_IPV6_MULTICAST_PREFIX == ((SOAD_A_P2VAR(SoAd_SockAddrInet6Type)LocalIpAddrPtr)->addr[0] &
                SOAD_IPV6_MULTICAST_PREFIX_MASK) )
            { /* valid multicast address */
              retVal = E_OK;
            }
          }
          else
          { /* unicast address */
            if ( SOAD_IPV6_MULTICAST_PREFIX != ((SOAD_A_P2VAR(SoAd_SockAddrInet6Type)LocalIpAddrPtr)->addr[0] &
                SOAD_IPV6_MULTICAST_PREFIX_MASK) )
            { /* no multicast address */
              retVal = E_OK;
            }
          }

          if ( retVal == E_OK )
          { /* valid address */
            /* overwrite current local address */
            SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] =
              (SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[0];
            SoAd_AddrId2LocalIpAddrDyn(addrId).addr[1] =
              (SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[1];
            SoAd_AddrId2LocalIpAddrDyn(addrId).addr[2] =
              (SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[2];
            SoAd_AddrId2LocalIpAddrDyn(addrId).addr[3] =
              (SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[3];

            /* set local address to "requested" */
            SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_REQUEST;

            /* set event to handle local address in main function */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
          }
        }
        else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
        /* #10101 Set IPv4 address if valid. */
        {
          if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_MULTICAST )
          { /* multicast address */
            /* check if address is a valid multicast address */
            if ( SOAD_IPV4_MULTICAST_PREFIX == ((SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[0] &
                SOAD_IPV4_MULTICAST_PREFIX_MASK) )
            { /* valid multicast address */
              retVal = E_OK;
            }
          }
          else
          { /* unicast address */
            if ( DefaultRouterPtr != NULL_PTR )
            {
              if ( Netmask <= 32u )
              { /* valid netmask */
                if ( SOAD_IPV4_MULTICAST_PREFIX != ((SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[0] &
                    SOAD_IPV4_MULTICAST_PREFIX_MASK) )
                { /* no multicast address */
                  retVal = E_OK;
                }
              }
            }
            if ( retVal == E_OK )
            {
              /* overwrite current netmask */
              SoAd_AddrId2LocalIpAddrDyn(addrId).netmask = Netmask;
            }
          }

          if ( retVal == E_OK )
          { /* valid address */
            /* overwrite current local address */
            SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] =
              (SOAD_A_P2VAR(SoAd_SockAddrInetType)LocalIpAddrPtr)->addr[0];

            /* set local address to "requested" */
            SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_REQUEST;

            /* set event to handle local address in main function */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
          }
        }
      }
      /* #1011 Accept request for link-local or DHCP IP address since it is automatically started at startup. */
      else
      {
        retVal = E_OK;

        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_GetSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_GetSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_DomainType Domain,
  SoAd_ProtocolType Protocol,
  P2VAR(SoAd_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint32            domain;
  sint32            type;
  sint32            protocol = 0;
  SoAd_SocketIdType socketId;
  SoAd_SocketIdType dirLinkSocketId;
  SoAd_SocketIdType limNodeSocketId;
  Std_ReturnType    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert domain for BSD Socket API. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  if ( Domain == SOAD_AF_INET6 )
  {
    domain = AF_INET6;
  }
  else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    domain = AF_INET;
  }

  /* #20 Convert protocol for BSD Socket API. */
# if ( SOAD_VTCP_USED == STD_ON )
  if ( Protocol == SOAD_IPPROTO_TCP )
  {
    type = SOCK_STREAM;
  }
  else
# endif /* SOAD_VTCP_USED == STD_ON */
  {
    type = SOCK_DGRAM;
  }

  /* #30 Get socket from BSD Socket API. */
  socketId = socket(domain, type, protocol);

  /* #40 Check if socket is UDP and of type unicast. */
  if ( (Protocol == SOAD_IPPROTO_UDP) &&
    (SoAd_LocalAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST) )
  {
    /* #400 Get separate socket to receive from directed broadcast/link-local address. */
    dirLinkSocketId = socket(domain, type, protocol);

    /* #401 Get separate socket to receive from limited broadcast/node-local address. */
    limNodeSocketId = socket(domain, type, protocol);
  }
  else
  {
    dirLinkSocketId = socketId;
    limNodeSocketId = socketId;
  }

  /* #50 Handle errors. */
  if ( (socketId == SOAD_INV_SOCKET_ID) || (dirLinkSocketId == SOAD_INV_SOCKET_ID) ||
    (limNodeSocketId == SOAD_INV_SOCKET_ID) )
  {
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_GET_SOCKET, SOAD_SOCK_BSD_API_SOCKET,
      SOAD_SOCK_BSD_PARAM_NONE, domain, type, errno);

    if ( socketId != SOAD_INV_SOCKET_ID )
    {
      (void)close(socketId);
    }
    if ( dirLinkSocketId != SOAD_INV_SOCKET_ID )
    {
      (void)close(dirLinkSocketId);
    }
    if ( limNodeSocketId != SOAD_INV_SOCKET_ID )
    {
      (void)close(limNodeSocketId);
    }
  }
  /* #60 Return socket in case no error occurred. */
  else
  {
    *SocketIdPtr = socketId;
    SoAd_SockIdx2MulticastAddr(SoAd_SockCons(SoConIdx).SockIdx).DirLinkSocketId = dirLinkSocketId;
    SoAd_SockIdx2MulticastAddr(SoAd_SockCons(SoConIdx).SockIdx).LimNodeSocketId = limNodeSocketId;

    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_Bind()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_Bind(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType      sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  /* #10 Check if local IP address is of type IPv6. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
  {
    struct sockaddr_in6 localAddr;

    /* #100 Bind socket to local address. */
    localAddr.sin6_family = AF_INET6;
    localAddr.sin6_port = IPBASE_HTON16(*PortPtr);
    localAddr.sin6_flowinfo = 0u;
    localAddr.sin6_scope_id = SoAd_AddrId2LocalIpAddrDyn(addrId).ifcIdx;

    *((uint32*)&localAddr.sin6_addr.s6_addr[0]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];
    *((uint32*)&localAddr.sin6_addr.s6_addr[4]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[1];
    *((uint32*)&localAddr.sin6_addr.s6_addr[8]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[2];
    *((uint32*)&localAddr.sin6_addr.s6_addr[12]) = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[3];

    retVal = SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SocketId, SOAD_A_P2CONST(struct sockaddr)&localAddr,
      sizeof(localAddr), PortPtr);

    localAddr.sin6_port = IPBASE_HTON16(*PortPtr);

    /* #101 Check if socket is UDP and of type unicast. */
#  if ( SOAD_VTCP_USED == STD_ON )
    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
#  endif /* SOAD_VTCP_USED == STD_ON */
    {
      if ( SoAd_LocalAddr(addrId).AddressType != SOAD_ADDRESS_TYPE_MULTICAST )
      {
        /* #1010 Bind separate socket to receive from directed broadcast/link-local address. */
        *((uint32*)&localAddr.sin6_addr.s6_addr[0])  = IPBASE_HTON32(0xFF010000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[4])  = IPBASE_HTON32(0x00000000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[8])  = IPBASE_HTON32(0x00000000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[12]) = IPBASE_HTON32(0x00000001ul);

        retVal |= SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).DirLinkSocketId,
          SOAD_A_P2CONST(struct sockaddr)&localAddr, sizeof(localAddr), PortPtr);

        /* #1011 Bind separate socket to receive from limited broadcast/node-local address. */
        *((uint32*)&localAddr.sin6_addr.s6_addr[0])  = IPBASE_HTON32(0xFF020000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[4])  = IPBASE_HTON32(0x00000000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[8])  = IPBASE_HTON32(0x00000000ul);
        *((uint32*)&localAddr.sin6_addr.s6_addr[12]) = IPBASE_HTON32(0x00000001ul);

        retVal |= SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).LimNodeSocketId,
          SOAD_A_P2CONST(struct sockaddr)&localAddr, sizeof(localAddr), PortPtr);
      }
    }
  }
  /* #20 Check if local IP address is of type IPv4. */
  else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    struct sockaddr_in localAddr;

    /* #200 Bind socket to local address. */
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = IPBASE_HTON16(*PortPtr);
    localAddr.sin_addr.s_addr = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0];

    retVal = SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SocketId, SOAD_A_P2CONST(struct sockaddr)&localAddr,
      sizeof(localAddr), PortPtr);

    localAddr.sin_port = IPBASE_HTON16(*PortPtr);

    /* #201 Check if socket is UDP and of type unicast. */
# if ( SOAD_VTCP_USED == STD_ON )
    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
# endif /* SOAD_VTCP_USED == STD_ON */
    {
      if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST )
      {
        /* #2010 Bind separate socket to receive from directed broadcast/link-local address. */
        localAddr.sin_addr.s_addr = SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] |
          IPBASE_HTON32((INADDR_BROADCAST >> SoAd_AddrId2LocalIpAddrDyn(addrId).netmask));

        retVal |= SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).DirLinkSocketId,
          SOAD_A_P2CONST(struct sockaddr)&localAddr, sizeof(localAddr), PortPtr);

        /* #2011 Bind separate socket to receive from limited broadcast/node-local address. */
        localAddr.sin_addr.s_addr = INADDR_BROADCAST;

        retVal |= SoAd_TcpIpBsd_BindSingleSocket(SoConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).LimNodeSocketId,
          SOAD_A_P2CONST(struct sockaddr)&localAddr, sizeof(localAddr), PortPtr);
      }
    }
  }

  /* #30 Add event to queue to handle UDP socket in main function if all calls to BSD Socket API succeeded. */
  if ( (retVal == E_OK)
#  if ( SOAD_VTCP_USED == STD_ON )
    && (SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR)
#  endif /* SOAD_VTCP_USED == STD_ON */
    )
  {
    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), sockIdx);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_BindSingleSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_BindSingleSocket(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2CONST(struct sockaddr, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  size_t LocalAddrSize,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) PortPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  Std_ReturnType        retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Bind socket via SoAd Socket API. */
  if ( bind(SocketId, LocalAddrPtr, LocalAddrSize) == SOAD_E_BSD_NOT_OK )
  {
    /* #20 Check if local address is still available if bind failed. */
    if ( errno == EADDRNOTAVAIL ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
    {
      if ( E_OK != SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr(SoConIdx, FALSE) )
      {
        /* #200 Mark local address as to be released. */
        SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_RELEASE;

        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
      }
    }
    /* #21 Report BSD Socket API error. */
    /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
    SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_BIND_SINGLE_SOCKET, SOAD_SOCK_BSD_API_BIND,
      SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(SoConIdx).SoConId, SocketId, errno);

    retVal = E_NOT_OK;
  }
  else
  {
    /* #30 Check if port has been chosen randomly if bind succeeded. */
    if ( *PortPtr == 0u )
    {
# if ( SOAD_VIPV6_ENABLED == STD_ON )
      struct sockaddr_in6 sockAddr;
      socklen_t sockAddrSize = sizeof(sockAddr);
# else
      struct sockaddr_in sockAddr;
      socklen_t sockAddrSize = sizeof(sockAddr);
# endif /* SOAD_VIPV6_ENABLED == STD_ON */

      /* #300 Get port via BSD Socket API. */
      if ( getsockname(SocketId, SOAD_A_P2VAR(struct sockaddr)&sockAddr, &sockAddrSize) == SOAD_E_BSD_NOT_OK )
      {
        /* #3000 Report BSD Socket API error if call failed. */
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_BIND_SINGLE_SOCKET, SOAD_SOCK_BSD_API_GET_SOCK_NAME,
          SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(SoConIdx).SoConId, SocketId, errno);

        retVal = E_NOT_OK;
      }
      else
      {
        /* #3001 Update local port if call succeeded. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
        {
          *PortPtr = IPBASE_SWAP16((SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_port);
        }
        else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
        {
          *PortPtr = IPBASE_SWAP16((SOAD_A_P2VAR(struct sockaddr_in)&sockAddr)->sin_port);
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_ChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  SoAd_ParamIdType ParameterId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch ( ParameterId )
  {
# if ( SOAD_VTCP_USED == STD_ON )
    /* #10 Set TCP rx buffer size. */
  case SOAD_PARAMID_TCP_RXWND_MAX:
    {
      size_t sockTpRxBufMin = *((uint16*)ParameterValue);

      if ( setsockopt(SocketId, SOL_SOCKET, SO_RCVBUF, &sockTpRxBufMin, sizeof(sockTpRxBufMin)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_RCVBUF, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
#  if ( SOAD_VTCP_TX_BUF_MIN_ENABLED == STD_ON )
    /* #20 Set TCP tx buffer size. */
  case SOAD_PARAMID_TCP_TXWND_MAX:
    {
      sint32 sockTcpTxBufMin = *((uint16*)ParameterValue);

      if ( setsockopt(SocketId, SOL_SOCKET, SO_SNDBUF, &sockTcpTxBufMin, sizeof(sockTcpTxBufMin)) ==
        SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_SNDBUF, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
#  endif /* SOAD_VTCP_TX_BUF_MIN_ENABLED == STD_ON */
    /* #30 Set TCP no delay (Nagle algorithm). */
  case SOAD_PARAMID_TCP_NAGLE:
    {
      sint32 var = *((uint8*)ParameterValue);
      if ( setsockopt(SocketId, IPPROTO_TCP, TCP_NODELAY, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
            SOAD_SOCK_BSD_PARAM_NODELAY, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
#  if ( SOAD_VTCP_KEEP_ALIVE_ENABLED == STD_ON )
    /* #40 Set TCP keep alive. */
  case SOAD_PARAMID_TCP_KEEPALIVE:
    {
      sint32 var = *((uint8*)ParameterValue);;
      if ( setsockopt(SocketId, SOL_SOCKET, SO_KEEPALIVE, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
            SOAD_SOCK_BSD_PARAM_SO_KEEPALIVE, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
    /* #50 Set TCP keep alive time. */
  case SOAD_PARAMID_TCP_KEEPALIVE_TIME:
    {
      sint32 var = *((sint32*)ParameterValue);
      if ( setsockopt(SocketId, IP_PROTOTCP, TCP_KEEPALIVE, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_TCP_KEEPALIVE, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
    /* #60 Set TCP keep alive maximum probes. */
  case SOAD_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
    {
      sint32 var = *((sint32*)ParameterValue);
      if ( setsockopt(SocketId, IP_PROTOTCP, TM_TCP_KEEPALIVE_CNT, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_TCP_KEEPALIVE_CNT, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
    /* #70 Set TCP keep alive interval. */
  case SOAD_PARAMID_TCP_KEEPALIVE_INTERVAL:
    {
      sint32 var = *((sint32*)ParameterValue);
      if ( setsockopt(SocketId, IP_PROTOTCP, TM_TCP_KEEPALIVE_INTV, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_TCP_KEEPALIVE_INTV, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      break;
    }
#  endif /* SOAD_VTCP_KEEP_ALIVE_ENABLED == STD_ON */
# endif /* SOAD_VTCP_USED == STD_ON */
    /* #80 Set BSD socket API specific parameters. */
  case SOAD_PARAMID_STACK_SPECIFIC:
    {
      SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
      sint32 var;

      /* #800 Set socket to non-blocking mode. */
      var = fcntl(SocketId, F_GETFL, 0);
      if ( var == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_FCNTL,
          SOAD_SOCK_BSD_PARAM_F_GETFL, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }
      else
      {
        if ( fcntl(SocketId, F_SETFL, var|O_NONBLOCK) == SOAD_E_BSD_NOT_OK )
        {
          retVal = E_NOT_OK;
          /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
          SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
            SOAD_SOCK_BSD_PARAM_REUSEADDR, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
        }
      }

      /* #801 Set reuse option for IP address to have the possibility to bind same address to another local address. */
      var = (sint32)TRUE;
      if ( setsockopt(SocketId, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
      {
        retVal = E_NOT_OK;
        /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
        SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
          SOAD_SOCK_BSD_PARAM_REUSEADDR, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
      }

      /* #802 Set broadcast flag to have the possibility to send to broadcast addresses. */
      if ( (SoAd_SockIdx2MulticastAddr(sockIdx).DirLinkSocketId != SocketId) &&
        (SoAd_SockIdx2MulticastAddr(sockIdx).LimNodeSocketId != SocketId) )
      {
        if ( setsockopt(SocketId, SOL_SOCKET, SO_BROADCAST, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
        {
          retVal = E_NOT_OK;
          /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
          SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
            SOAD_SOCK_BSD_PARAM_BROADCAST, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
        }
      }

      /* #803 Set restriction of socket to IPv6 for UDP sockets, TCP client sockets or TCP listen sockets. */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
      {
        if ( (SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr == NULL_PTR) ||
          (SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx == SOAD_INV_SOCK_IDX) ||
          (SoAd_SockIdx2SocketId(SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx) == SOAD_INV_SOCKET_ID) )
        {
          if ( setsockopt(SocketId, IPPROTO_IPV6, IPV6_V6ONLY, &var, sizeof(var)) == SOAD_E_BSD_NOT_OK )
          {
            retVal = E_NOT_OK;
            /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
            SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
              SOAD_SOCK_BSD_PARAM_IP6ONLY, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
          }
        }
      }
# else
      SOAD_DUMMY_STATEMENT_VAR(SoConIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VIPV6_ENABLED == STD_ON */

      /* #804 Enable linger time and set it to 0 for TCP sockets to allow faster socket connection reestablishment. */
# if ( SOAD_VTCP_USED == STD_ON )
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
      {
        /* set linger time */
        if ( SoAd_LingerTimeoutEnabled() == TRUE )
        { /* linger time enabled */
          struct linger lingerConfig;
          lingerConfig.l_onoff = 1;
          lingerConfig.l_linger = 0;
          if ( setsockopt(SocketId, SOL_SOCKET, SO_LINGER, &lingerConfig, sizeof(lingerConfig)) == SOAD_E_BSD_NOT_OK )
          {
            retVal = E_NOT_OK;
            /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
            SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_CHANGE_PARAMETER, SOAD_SOCK_BSD_API_SET_SOCK_OPT,
              SOAD_SOCK_BSD_PARAM_LINGER, SOAD_SOCK_BSD_VALUE_NONE, SocketId, errno);
          }
        }
      }
# endif /* SOAD_VTCP_USED == STD_ON */
      break;
    }
    /* #90 Ignore not relevant parameters. */
  default:
    {
      
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleSocketReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleSocketReception(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16                  handleIdx = SOAD_INV_PDU_IDX;
  SoAd_SockIdxType        lvl = (SoAd_SockIdxType)SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveMgtPtr->Lvl;
  SoAd_SockIdxType_least  idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get socket index event queue limit. */
# if ( SOAD_VEVE_QUE_LIMIT_SOCKIDX_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveQueLimit )
  {
    lvl = (SoAd_PduIdxType)SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_SOCKIDX_ENALBED == STD_ON */

  /* #20 Iterate over all queue elements which have to be handled in this context. */
  for ( idx = 0u; idx < lvl; idx++ )
  {
    /* #30 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxSockIdx(), &handleIdx) == E_OK )
    {
      boolean addEventAgain = FALSE;
      SoAd_SockIdxType sockIdx = (SoAd_SockIdxType)handleIdx;

      /* #40 Check if socket is ready to receive data. */
      if ( SoAd_SockIdx2SockState(sockIdx) == SOAD_SOCK_STATE_ESTABLISHED )
      {
        SoAd_SoConIdxType soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
        SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);

        /* #50 Handle reception on UDP socket. */
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
        {
          SoAd_LocalAddrIdType addrId = SoAd_SockCons(soConIdx).SockConGrpPtr->LocIpAddrId;
          struct sockaddr_inX sockAddr;
          socklen_t addrLen = sizeof(sockAddr);

          /* #500 Receive from unicast socket. */
          if ( E_OK == SoAd_TcpIpBsd_UdpReceiveFrom(soConIdx, socketId, SOAD_A_P2VAR(struct sockaddr)&sockAddr,
              addrLen) )
          {
            /* #5000 Add event to queue again if no socket error occurred. */
            addEventAgain = TRUE;
          }

          if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST )
          {
            /* #501 Receive from directed broadcast/link-local address socket. */
            if ( SoAd_SockIdx2MulticastAddr(sockIdx).DirLinkSocketId != SOAD_INV_SOCKET_ID )
            {
              if ( E_OK == SoAd_TcpIpBsd_UdpReceiveFrom(soConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).DirLinkSocketId,
                  SOAD_A_P2VAR(struct sockaddr)&sockAddr, addrLen) )
              {
                /* #5010 Add event to queue again if no socket error occurred */
                addEventAgain = TRUE;
              }
            }

            /* #502 Receive from limited broadcast/node-local address socket. */
            if ( SoAd_SockIdx2MulticastAddr(sockIdx).LimNodeSocketId != SOAD_INV_SOCKET_ID )
            {
              if ( E_OK == SoAd_TcpIpBsd_UdpReceiveFrom(soConIdx, SoAd_SockIdx2MulticastAddr(sockIdx).LimNodeSocketId,
                  SOAD_A_P2VAR(struct sockaddr)&sockAddr, addrLen) )
              {
                /* #5020 Add event to queue again if no socket error occurred. */
                addEventAgain = TRUE;
              }
            }
          }
        }
        /* #60 Handle reception on TCP socket. */
        else
        {
          /* #600 Receive from client or server data socket. */
          if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate == TRUE) ||
            (SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx != sockIdx) )
          {
            if ( E_OK == SoAd_TcpIpBsd_TcpReceive(soConIdx) )
            {
              /* #6000 Add event to queue again if no socket error occurred. */
              addEventAgain = TRUE;
            }
          }
          /* #601 Add event to queue again if socket is listen socket. */
          else
          {
            addEventAgain = TRUE;
          }
        }
      }
      /* #41 Add event to queue again if socket is not ready to receive data. */
      else
      {
        addEventAgain = TRUE;
      }
      if ( addEventAgain == TRUE )
      {
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), sockIdx);
      }
    }
    /* #31 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_UdpReceiveFrom()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_UdpReceiveFrom(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId,
  P2VAR(struct sockaddr, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr,
  socklen_t RemAddrSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType      sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  SoAd_SocketIdType     unicastSocketId = SoAd_SockIdx2SocketId(sockIdx);
  ssize_t               dataLen;
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Receive data via BSD Socket API. */
  dataLen = recvfrom(SocketId, SoAd_SocketRxBuffer(), SoAd_SocketRxBufferSize(), 0, RemAddrPtr, &RemAddrSize);

  /* #20 Evaluate result. */
  if ( (dataLen == 0u) || (dataLen == SOAD_E_BSD_NOT_OK) )
  {
    /* #200 Mark to repeat if no data is received. */
    if ( ((errno == EAGAIN) || (errno == EWOULDBLOCK)) && (dataLen != 0) ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
    {
      retVal = E_OK;
    }
    /* #201 Close unicast socket (broadcast/multicast sockets closed in same context) if unknown error occurred. */
    else
    {
      SoAd_TcpIp_Close(unicastSocketId, TRUE, sockIdx);
    }
  }
  else
  {
    SoAd_SockAddrInetXType remAddr;

    /* #202 Check if data is not received from own local address if data is received. */
    SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr(SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr, RemAddrPtr);

    if ( SoAd_TcpIp_CompareToIpAddr(SoConIdx, SOAD_A_P2CONST(SoAd_IpAddrType)remAddr.addr) == E_NOT_OK )
    {
      /* #2020 Forward received data via unicast socket since its socket identifier is known by SoAd. */
      SoAd_RxIndication(unicastSocketId, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr, SoAd_SocketRxBuffer(),
        dataLen);
    }

    /* #2030 Mark to repeat reception. */
    retVal = E_OK;
  }

  return retVal;
}

# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_TcpReceive(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType  sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);
  ssize_t dataLen;
  ssize_t bufLen;
  ssize_t bufIdx;
  Std_ReturnType    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if reception buffer is sufficient to receive data and mark to repeat if not. */
  if ( SoAd_SockIdx2RxMgt(sockIdx).Len != SoAd_SockIdx2RxBuf(sockIdx).MaxLen )
  {
    /* #20 Get next buffer segment from local ring buffer.*/
    if ( SoAd_SockIdx2RxBuf(sockIdx).MaxLen <= (SoAd_SockIdx2RxMgt(sockIdx).Offset +
      SoAd_SockIdx2RxMgt(sockIdx).Len) )
    {
      bufLen = SoAd_SockIdx2RxBuf(sockIdx).MaxLen - SoAd_SockIdx2RxMgt(sockIdx).Len;
      bufIdx = SoAd_SockIdx2RxMgt(sockIdx).Offset - bufLen;
    }
    else
    {
      bufLen = SoAd_SockIdx2RxBuf(sockIdx).MaxLen - (SoAd_SockIdx2RxMgt(sockIdx).Offset +
        SoAd_SockIdx2RxMgt(sockIdx).Len);
      bufIdx = SoAd_SockIdx2RxMgt(sockIdx).Offset + SoAd_SockIdx2RxMgt(sockIdx).Len;
    }

    /* #30 Receive data via BSD Socket API. */
    dataLen = recv(socketId, &SoAd_SockIdx2RxBuf(sockIdx).BufPtr[bufIdx], bufLen, 0);

    /* #40 Evaluate result. */
    if ( (dataLen == 0u) || (dataLen == SOAD_E_BSD_NOT_OK) )
    {
      if ( ((errno == EAGAIN) || (errno == EWOULDBLOCK)) && (dataLen != 0) ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
      {
        /* #400 Mark to repeat if no data is received. */
        retVal = E_OK;
      }
      else
      {
        /* #401 Close socket if unknown error occurred. */
        SoAd_TcpIp_Close(socketId, TRUE, sockIdx);
      }
    }
    else
    {
      /* #402 Mark to repeat and forward data if data is received. */
      SoAd_SockIdx2RxMgt(sockIdx).Len += dataLen;

      SoAd_RxIndication(socketId, SOAD_A_P2VAR(SoAd_SockAddrType)&SoAd_SockConIdx2RemSockDyn(SoConIdx),
        &SoAd_SockIdx2RxBuf(sockIdx).BufPtr[bufIdx], dataLen);

      retVal = E_OK;
    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
}
# endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleIpAddrStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleIpAddrStates(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16                      handleIdx = SOAD_INV_PDU_IDX;
  SoAd_LocalAddrIdType        lvl = (SoAd_SockIdxType)SoAd_EventQueues(SoAd_EventQueueIdxLocalAddr()).EveMgtPtr->Lvl;
  SoAd_LocalAddrIdType_least  idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get local address event queue limit. */
# if ( SOAD_VEVE_QUE_LIMIT_LOCALADDR_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxLocalAddr()).EveQueLimit )
  {
    lvl = (SoAd_LocalAddrIdType)SoAd_EventQueues(SoAd_EventQueueIdxLocalAddr()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_LOCALADDR_ENALBED == STD_ON */

  /* #20 Iterate over all queue elements which have to be handled in this context. */
  for ( idx = 0u; idx < lvl; idx++ )
  {
    /* #30 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxLocalAddr(), &handleIdx) == E_OK )
    {
      SoAd_LocalAddrIdType  addrId = (SoAd_LocalAddrIdType)handleIdx;
      SoAd_SockIdxType      sockIdx = SoAd_AddrId2SockIdxCol(addrId).SockIdxColPtr[0];
      SoAd_SoConIdxType     firstSoConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
      SoAd_IpAddrStateType  ipAddrState = SOAD_IPADDR_STATE_ONHOLD;

      /* #40 Enter critical section to prevent that local IP address is changed via any API call. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Check if local IP address is assigned. */
      if ( SoAd_SockConIdx2IpAddrState(firstSoConIdx) != SOAD_IPADDR_STATE_ASSIGNED )
      {
        /* #500 Check if local IP address is explicitly requested or assignment trigger is automatic. */
        if ( (SoAd_LocalAddr(addrId).AssignTrigger == SOAD_ASSIGN_TRIGGER_AUTOMATIC) ||
          (SoAd_AddrId2LocalIpAddrReqState(addrId) == SOAD_LOCAL_IP_ADDR_REQ_STATE_REQUEST) )
        {
          /* #5000 Try to assign the local IP address. */
          if ( E_OK == SoAd_TcpIpBsd_AssignIpAddr(firstSoConIdx) )
          {
            /* #50000 Mark IP address as assigned and reset request state if local IP address could be assigned. */
            ipAddrState = SOAD_IPADDR_STATE_ASSIGNED;
            SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_NONE;
          }
          else
          {
            /* #50001 Add element to queue again if local IP address could not be assigned. */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);
          }
        }
      }
      /* #51 Handle local IP address if it is assigned. */
      else
      {
        /* #510 Check if local IP address shall be released. */
        if ( SoAd_AddrId2LocalIpAddrReqState(addrId) == SOAD_LOCAL_IP_ADDR_REQ_STATE_RELEASE )
        {
          /* #5100 Release local IP address and close corresponding sockets. */
          SoAd_TcpIpBsd_ReleaseIpAddr(firstSoConIdx);

          /* #5101 Mark IP address as unassigned and reset request state. */
          ipAddrState = SOAD_IPADDR_STATE_UNASSIGNED;
          SoAd_AddrId2LocalIpAddrReqState(addrId) = SOAD_LOCAL_IP_ADDR_REQ_STATE_NONE;

          /* #5102 Add element to queue again if local IP address is triggered automatically to reassign it later. */
          if ( SoAd_LocalAddr(addrId).AssignTrigger == SOAD_ASSIGN_TRIGGER_AUTOMATIC )
          {
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxLocalAddr(), addrId);;
          }
        }
      }

      /* #60 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

      /* #70 Check if local address assignment has changed. */
      if ( ipAddrState != SOAD_IPADDR_STATE_ONHOLD )
      {
        /* #700 Notify about local IP address assignment state change. */
        SoAd_LocalIpAddrAssignmentChg(addrId, ipAddrState);
      }
    }
    /* #31 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_AssignIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_AssignIpAddr(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_LocalAddrIdType  addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to assign local IP address. */
  /* #100 Check if local IP address is unicast. */
  if ( SoAd_LocalAddr(addrId).AddressType == SOAD_ADDRESS_TYPE_UNICAST )
  {
    /* #1000 Handle static IP addresses. */
    if ( SoAd_LocalAddr(addrId).AssignType == SOAD_IPADDR_ASSIGNMENT_STATIC )
    {
      /* #10000 Check if local address is available. */
      retVal = SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr(SoConIdx, FALSE);
    }
    /* #1001 Handle other IP addresses. */
    else
    {
      /* #10010 Overwrite local address with first matching local address on the configured interface. */
      retVal = SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr(SoConIdx, TRUE);
    }
  }
  /* #101 Assume that multicast local IP address is always assigned. */
  else
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ReleaseIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ReleaseIpAddr(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_LocalAddrIdType    addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  SoAd_SockIdxType_least  sockIdxNum;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all corresponding sockets. */
  for ( sockIdxNum = 0u; sockIdxNum < SoAd_AddrId2SockIdxCol(addrId).SockIdxNum; sockIdxNum++ )
  {
    SoAd_SockIdxType  sockIdx = SoAd_AddrId2SockIdxCol(addrId).SockIdxColPtr[sockIdxNum];
    SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);

    /* #100 Close socket if assigned. */
    if ( socketId != SOAD_INV_SOCKET_ID ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
    {
      SoAd_TcpIp_Close(socketId, TRUE, sockIdx);
    }
  }

  /* #20 Reset local IP address struct. */
  SoAd_TcpIpBsd_ResetLocalIpAddr(addrId);
}

# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleTcpSocketStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleTcpSocketStates(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16                  handleIdx = SOAD_INV_PDU_IDX;
  SoAd_SockIdxType        lvl = (SoAd_SockIdxType)SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveMgtPtr->Lvl;
  SoAd_SockIdxType_least  idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get socket index event queue limit. */
#  if ( SOAD_VEVE_QUE_LIMIT_SOCKIDX_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveQueLimit )
  {
    lvl = (SoAd_PduIdxType)SoAd_EventQueues(SoAd_EventQueueIdxSockIdx()).EveQueLimit;
  }
#  endif /* SOAD_VEVE_QUE_LIMIT_SOCKIDX_ENALBED == STD_ON */

  /* #20 Iterate over all queue elements which have to be handled in this context. */
  for ( idx = 0u; idx < lvl; idx++ )
  {
    /* #30 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxSockIdx(), &handleIdx) == E_OK )
    {
      boolean addEventAgain = FALSE;
      SoAd_SockIdxType sockIdx = (SoAd_SockIdxType)handleIdx;
      SoAd_SoConIdxType soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

      /* #40 Check if socket is a TCP socket. */
      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
      {
        /* #50 Check if socket is a TCP client socket. */
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate == TRUE )
        {
          /* #60 Check if socket is connecting to a remote entity. */
          if ( SoAd_SockIdx2SockState(sockIdx) == SOAD_SOCK_STATE_CONNECT )
          {
            /* #70 Check if connection is established. */
            if ( SoAd_TcpIpBsd_TcpCheckConnect(sockIdx) == TRUE )
            {
              /* #80 Mark to add element to queue again if connection is not yet established or to start reception. */
              addEventAgain = TRUE;
            }
          }
          /* #61 Mark to add element to queue again if socket is not connecting to remote entity. */
          else
          {
            addEventAgain = TRUE;
          }
        }
        /* #51 Handle TCP server sockets. */
        else
        {
          /* #90 Check if socket is a listen socket. */
          if ( sockIdx == SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx )
          {
            /* #100 Check if socket is in listen state. */
            if ( SoAd_SockIdx2SockState(sockIdx) == SOAD_SOCK_STATE_LISTEN )
            {
              /* #110 Check if any new connection has been established. */
              if ( SoAd_TcpIpBsd_TcpCheckAccept(sockIdx) == TRUE )
              {
                /* #120 Mark to add element to queue again if no socket error occurred. */
                addEventAgain = TRUE;
              }
            }
          }
          /* #81 Mark to add element to queue again in case TCP socket is a "data" socket. */
          else
          {
            addEventAgain = TRUE;
          }
        }
      }
      /* #41 Mark to add element to queue again in case of UDP socket. */
      else
      {
        addEventAgain = TRUE;
      }

      /* #130 Add element to queue again if required. */
      if ( addEventAgain == TRUE )
      {
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), sockIdx);
      }
    }
    /* #31 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpCheckConnect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_TcpIpBsd_TcpCheckConnect(
  SoAd_SockIdxType SockIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  struct sockaddr_in6 sockAddr;
# else
  struct sockaddr_in  sockAddr;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  SoAd_SoConIdxType   soConIdx = SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart;
  SoAd_SocketIdType   socketId = SoAd_SockIdx2SocketId(SockIdx);
  boolean             retry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert remote address for BSD Socket API. */
#  if ( SOAD_VIPV6_ENABLED == STD_ON )
  if ( SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET6 )
  {
    (SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_family = AF_INET6;
    (SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_scope_id =
      SoAd_AddrId2LocalIpAddrDyn(SoAd_SockCons(soConIdx).SockConGrpPtr->LocIpAddrId).ifcIdx;
  }
  else
#  endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    (SOAD_A_P2VAR(struct sockaddr_in)&sockAddr)->sin_family = AF_INET;
  }
  SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr(SOAD_A_P2VAR(struct sockaddr)&sockAddr,
    SOAD_A_P2CONST(SoAd_SockAddrType)&SoAd_SockConIdx2RemSockDyn(soConIdx));

  /* #20 Call BSD Socket API. */
  if ( 0 > connect(socketId, SOAD_A_P2CONST(struct sockaddr)&sockAddr, sizeof(sockAddr)))
  {
    /* #200 Report BSD Socket API error and close socket if call to BSD Socket API failed. */
    if ( errno != EALREADY ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
    {
      /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
      SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_TCP_CHECK_CONNECT, SOAD_SOCK_BSD_API_CONNECT,
        SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(soConIdx).SoConId, socketId, errno);
      SoAd_TcpIp_Close(socketId, TRUE, SockIdx);
    }
    /* #201 Mark to retry if connection is not yet established. */
    else
    {
      retry = TRUE;
    }
  }
  else
  {
    /* #202 Notify if socket connection is established and mark to retry to start reception in main function. */
    SoAd_TcpConnected(socketId);

    retry = TRUE;
  }

  return retry;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_TcpCheckAccept()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_TcpIpBsd_TcpCheckAccept(
  SoAd_SockIdxType SockIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  struct sockaddr_in6 sockAddr;
# else
  struct sockaddr_in  sockAddr;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  socklen_t           sockAddrLen = sizeof(sockAddr);
  SoAd_SoConIdxType   soConIdx = SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart;
  SoAd_SocketIdType   socketId = SoAd_SockIdx2SocketId(SockIdx);
  SoAd_SocketIdType   socketIdConnected;
  boolean             retry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set remote address for BSD Socket API. */
#  if ( SOAD_VIPV6_ENABLED == STD_ON )
  if ( SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET6 )
  {
    (SOAD_A_P2VAR(struct sockaddr_in6)&sockAddr)->sin6_family = AF_INET6;
  }
  else
#  endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    (SOAD_A_P2VAR(struct sockaddr_in)&sockAddr)->sin_family = AF_INET;
  }

  /* #20 Call BSD Socket API. */
  socketIdConnected = accept(socketId, SOAD_A_P2VAR(struct sockaddr)&sockAddr, &sockAddrLen);

  /* #30 Handle result if function call was successful. */
  if ( socketIdConnected != SOAD_INV_SOCKET_ID )
  {
    Std_ReturnType retVal = E_NOT_OK;
    SoAd_SockAddrInetXType remSockAddr;

    /* #300 Convert remote address for ASR Socket API. */
    remSockAddr.domain = SoAd_SockConIdx2RemSockDyn(soConIdx).domain;
    SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr(SOAD_A_P2VAR(SoAd_SockAddrType)&remSockAddr,
      SOAD_A_P2CONST(struct sockaddr)&sockAddr);

    /* #301 Change parameters on newly connected socket. */
    if ( SoAd_SoCon_TcpChangeParameter(soConIdx, socketIdConnected) == E_OK )
    {
      /* #302 Notify about newly connected socket if parameters could be changed. */
      if ( SoAd_TcpAccepted(socketId, socketIdConnected, SOAD_A_P2VAR(SoAd_SockAddrType)&remSockAddr) == E_OK )
      {
        /* #303 Set event to handle socket reception in main function. */
        SoAd_SoConIdxType_least idx;
        SoAd_SoConIdxType soConIdxStart = SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart;
        SoAd_SoConIdxType soConIdxEnd = (SoAd_SoConIdxType)((SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxStart +
            SoAd_SockIdx2SoConIdxCol(SockIdx).SoConIdxNum) - 1u);
        for ( idx = soConIdxStart; idx <= soConIdxEnd; idx++ )
        {
          if ( SoAd_SockIdx2SocketId(SoAd_SockCons(idx).SockIdx) == socketIdConnected )
          {
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxSockIdx(), SoAd_SockCons(idx).SockIdx);
            break;
          }
        }

        retVal = E_OK;
      }
    }

    /* #304 Close socket if parameters could not be changed or newly connected socket is not accepted. */
    if ( retVal == E_NOT_OK )
    {
      SoAd_TcpIp_Close(socketIdConnected, TRUE, SockIdx);
    }

    /* #305 Mark to retry since listen socket is still active. */
    retry = TRUE;
  }
  /* #31 Handle result if function call was not successful. */
  else
  {
    /* #310 Mark to retry if no new socket is available since listen socket is still active. */
    if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) /* PRQA S 5119 */ /* MD_SoAd_20.5 */
    {
      retry = TRUE;
    }
    /* #311 Report BSD Socket API error and close listen socket if any unexpected error occurred. */
    else
    {
      /* PRQA S 5119 2 */ /* MD_SoAd_20.5 */
      SOAD_REPORT_SOCK_ERROR_PARAM_2(SOAD_SOCK_SID_TCPIP_BSD_TCP_CHECK_ACCEPT, SOAD_SOCK_BSD_API_ACCEPT,
        SOAD_SOCK_BSD_PARAM_NONE, SoAd_SockCons(soConIdx).SoConId, socketId, errno);

      SoAd_TcpIp_Close(socketId, TRUE, SockIdx);
    }
  }

  return retry;
}
# endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ResetLocalIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ResetLocalIpAddr(
  SoAd_LocalAddrIdType LocalAddrId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset local IP address to wildcard if no static IP address is configured. */
  if ( SoAd_AddrId2LocalIpAddr(LocalAddrId).addrPtr == NULL_PTR )
  {
    SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).netmask = 0u;
    SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[0] = SOAD_IPADDR_ANY;
# if ( SOAD_VIPV6_ENABLED == STD_ON )
    if ( SoAd_AddrId2LocalIpAddr(LocalAddrId).domain == SOAD_AF_INET6 )
    {
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[1] = SOAD_IPADDR_ANY;
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[2] = SOAD_IPADDR_ANY;
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[3] = SOAD_IPADDR_ANY;
    }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  }
  /* #11 Reset local IP address to configured static IP address otherwise. */
  else
  {
    SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).netmask = SoAd_AddrId2LocalIpAddr(LocalAddrId).netmask;
    SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[0] = SoAd_AddrId2LocalIpAddr(LocalAddrId).addrPtr[0];
# if ( SOAD_VIPV6_ENABLED == STD_ON )
    if ( SoAd_AddrId2LocalIpAddr(LocalAddrId).domain == SOAD_AF_INET6 )
    {
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[1] = SoAd_AddrId2LocalIpAddr(LocalAddrId).addrPtr[1];
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[2] = SoAd_AddrId2LocalIpAddr(LocalAddrId).addrPtr[2];
      SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).addr[3] = SoAd_AddrId2LocalIpAddr(LocalAddrId).addrPtr[3];
    }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  }

  /* #20 Reset general local IP address part. */
  SoAd_AddrId2LocalIpAddrDyn(LocalAddrId).ifcIdx = SoAd_LocalAddr(LocalAddrId).IfcIdx;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_CheckAndOverwriteLocalAddr(
  SoAd_SoConIdxType SoConIdx,
  boolean OverwriteLocal)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(struct ifaddrs, AUTOMATIC, SOAD_APPL_VAR)  addrPtr;
  SoAd_LocalAddrIdType                              addrId = SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId;
  Std_ReturnType                                    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get all local addresses via BSD Socket API. */
  if ( 0 == getifaddrs(&addrPtr) )
  {
    struct ifaddrs * addrPtrIdx;
    uint8 family = AF_INET;

# if ( SOAD_VIPV6_ENABLED == STD_ON )
    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
    {
      family = AF_INET6;
    }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */

    /* #20 Iterate over all local addresses. */
    for (addrPtrIdx = addrPtr; addrPtrIdx != NULL; addrPtrIdx = addrPtrIdx->ifa_next)
    {
      /* #30 Check if local address interface and address family matches the configured local IP address. */
      if ( if_nametoindex(addrPtrIdx->ifa_name) == SoAd_AddrId2LocalIpAddrDyn(addrId).ifcIdx )
      {
        if (addrPtrIdx->ifa_addr != NULL )
        {
          if ( family == addrPtrIdx->ifa_addr->sa_family )
          {
            /* #40 Overwrite local IP address with first suitable local address on interface if required. */
            if ( TRUE == OverwriteLocal )
            {
# if ( SOAD_VIPV6_ENABLED == STD_ON )
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
              {
                uint8_least idxByte;

                SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] =
                  *((uint32*)&((struct sockaddr_in6*)addrPtrIdx->ifa_addr)->sin6_addr.s6_addr[0]);
                SoAd_AddrId2LocalIpAddrDyn(addrId).addr[1] =
                  *((uint32*)&((struct sockaddr_in6*)addrPtrIdx->ifa_addr)->sin6_addr.s6_addr[4]);
                SoAd_AddrId2LocalIpAddrDyn(addrId).addr[2] =
                  *((uint32*)&((struct sockaddr_in6*)addrPtrIdx->ifa_addr)->sin6_addr.s6_addr[8]);
                SoAd_AddrId2LocalIpAddrDyn(addrId).addr[3] =
                  *((uint32*)&((struct sockaddr_in6*)addrPtrIdx->ifa_addr)->sin6_addr.s6_addr[12]);

                SoAd_AddrId2LocalIpAddrDyn(addrId).netmask = 0u;
                for ( idxByte = 0u; idxByte < 16u; idxByte++ )
                {
                  uint8 addrByte = ((struct sockaddr_in6*)addrPtrIdx->ifa_netmask)->sin6_addr.s6_addr[idxByte];

                  if ( addrByte != 0xFFu )
                  {
                    uint8_least idxBit;

                    for ( idxBit = 0u; idxBit < 8u; idxBit++ )
                    {
                      if ( ((uint8)(addrByte << idxBit)) == 0u )
                      {
                        SoAd_AddrId2LocalIpAddrDyn(addrId).netmask = (idxByte * 8u) + idxBit;
                        break;
                      }
                    }
                    break;
                  }
                }

                retVal = E_OK;
                break;
              }
              else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
              {
                uint32      addr = IPBASE_SWAP32(((struct sockaddr_in*)addrPtrIdx->ifa_netmask)->sin_addr.s_addr);
                uint8_least idxBit;

                SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] =
                  (uint32)((struct sockaddr_in*)addrPtrIdx->ifa_addr)->sin_addr.s_addr;

                SoAd_AddrId2LocalIpAddrDyn(addrId).netmask = 0u;
                for ( idxBit = 0u; idxBit < 32u; idxBit++ )
                {
                  if ( ((uint32)(addr << idxBit)) == 0u )
                  {
                    SoAd_AddrId2LocalIpAddrDyn(addrId).netmask = idxBit;
                    break;
                  }
                }

                retVal = E_OK;
                break;
              }
            }
            /* #41 Check if local address matches the configured local IP address otherwise. */
            else
            {
# if ( SOAD_VIPV6_ENABLED == STD_ON )
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain == SOAD_AF_INET6 )
              {
                uint32 *localAddrPtr = SoAd_AddrId2LocalIpAddrDyn(addrId).addr;
                uint32 *bsdAddrPtr = (uint32*)&((struct sockaddr_in6*)addrPtrIdx->ifa_addr)->sin6_addr.s6_addr;

                if ( (localAddrPtr[0] == bsdAddrPtr[0]) && (localAddrPtr[1] == bsdAddrPtr[1]) &&
                    (localAddrPtr[2] == bsdAddrPtr[2]) && (localAddrPtr[3] == bsdAddrPtr[3]) )
                {
                  retVal = E_OK;
                  break;
                }
              }
              else
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
              {
                uint32 addr = *((uint32*)&((struct sockaddr_in*)addrPtrIdx->ifa_addr)->sin_addr.s_addr);

                if ( SoAd_AddrId2LocalIpAddrDyn(addrId).addr[0] == addr )
                {
                  retVal = E_OK;
                  break;
                }
              }
            }
          }
        }
      }
    }
    /* #50 Free occupied buffer of BSD Socket API used to get all local addresses. */
    freeifaddrs(addrPtr);
  }
  return retVal;
}

# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_HandleTcpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleTcpTxConfirmation(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(SoAd_TcpTxQueueConfigType, AUTOMATIC, SOAD_APPL_VAR) qPtr = SoAd_SockCons(SoConIdx).TcpTxQueuePtr;
  uint16        lvl = qPtr->MgtPtr->Lvl;
  uint16_least  idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate until no TCP tx queue elements are left. */
  for ( idx = 0u; idx < lvl; idx++ )
  {
    /* #20 Check if a element is left. */
    if ( qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx != SOAD_INV_PDU_IDX ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
    {
      /* #30 Check if element has a length (means element is reserved and set). */
      if ( qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len > 0u )
      {
        SoAd_PduIdxType txPduIdx = qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx;

        /* #40 Release element. */
        qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len = 0u;
        qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        qPtr->MgtPtr->Lvl--;
        if ( (qPtr->MgtPtr->Idx + 1u) == qPtr->Size)
        {
          qPtr->MgtPtr->Idx = 0u;
        }
        else
        {
          qPtr->MgtPtr->Idx++;
        }

        /* #50 Call TxConfirmation. */
        SoAd_TcpTxConfirmation(SoConIdx, txPduIdx);
      }
      /* #31 Stop iteration if element has no length (means element is reserved by current transmission request). */
      else
      {
        break;
      }
    }
  }
}
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ConvertAsrSockAddr2BsdSockAddr(
  P2VAR(struct sockaddr, SOAD_APPL_DATA, SOAD_APPL_DATA) BsdSockAddrPtr,
  P2CONST(SoAd_SockAddrType, SOAD_APPL_DATA, SOAD_APPL_DATA) AsrSockAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert IPv4 address. */
  if ( AsrSockAddrPtr->domain == SOAD_AF_INET )
  {
    ((struct sockaddr_in*)BsdSockAddrPtr)->sin_family = AF_INET;
    ((struct sockaddr_in*)BsdSockAddrPtr)->sin_port = ((SoAd_SockAddrInetType*)AsrSockAddrPtr)->port;
    ((struct sockaddr_in*)BsdSockAddrPtr)->sin_addr.s_addr = *((SoAd_SockAddrInetType*)AsrSockAddrPtr)->addr;
  }
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  /* #20 Convert IPv6 address. */
  else
  {
    ((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_family = AF_INET6;
    ((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_port = ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->port;
    ((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_flowinfo = 0u;

    *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[0]) =
      ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[0];
    *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[4]) =
      ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[1];
    *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[8]) =
      ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[2];
    *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[12]) =
      ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[3];
  }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ConvertBsdSockAddr2AsrSockAddr(
  P2VAR(SoAd_SockAddrType, SOAD_APPL_DATA, SOAD_APPL_DATA) AsrSockAddrPtr,
  P2CONST(struct sockaddr, SOAD_APPL_DATA, SOAD_APPL_DATA) BsdSockAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert IPv4 address. */
  if ( BsdSockAddrPtr->sa_family == AF_INET )
  {
    ((SoAd_SockAddrInetType*)AsrSockAddrPtr)->domain = SOAD_AF_INET;
    ((SoAd_SockAddrInetType*)AsrSockAddrPtr)->port = ((struct sockaddr_in*)BsdSockAddrPtr)->sin_port;
    ((SoAd_SockAddrInetType*)AsrSockAddrPtr)->addr[0] = ((struct sockaddr_in*)BsdSockAddrPtr)->sin_addr.s_addr;
  }
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  /* #20 Convert IPv6 address. */
  else
  {
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->domain = SOAD_AF_INET6;
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->port = ((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_port;
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[0] =
      *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[0]);
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[1] =
      *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[4]);
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[2] =
      *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[8]);
    ((SoAd_SockAddrInet6Type*)AsrSockAddrPtr)->addr[3] =
      *((uint32*)&((struct sockaddr_in6*)BsdSockAddrPtr)->sin6_addr.s6_addr[12]);
  }
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
}

# if ( SOAD_VPDU_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_InitSocketTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_InitSocketTxBuffer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockTxBufIdxType_least sockTxBufIdx;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Iterate over all socket transmission buffer. */
  for ( sockTxBufIdx = 0u; sockTxBufIdx < SoAd_SocketTxBufferNum(); sockTxBufIdx++ )
  {
    /* #20 Reset socket transmission buffer. */
    SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).SockIdx = SOAD_INV_SOCK_IDX;
    SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).Len     = 0u;
  }
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_RequestSocketTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_RequestSocketTxBuffer(
  SoAd_SockIdxType SockIdx,
  P2VAR(SoAd_SockTxBufIdxType, SOAD_APPL_DATA, SOAD_APPL_DATA) SockTxBufIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockTxBufIdxType_least sockTxBufIdx;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket transmission buffer struct is changed via any API call. */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Iterate over all socket transmission buffer to find an unused element. */
  for ( sockTxBufIdx = 0u; sockTxBufIdx < SoAd_SocketTxBufferNum(); sockTxBufIdx++ )
  {
    if ( SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).SockIdx == SOAD_INV_SOCK_IDX )
    {
      /* #200 Set and return socket transmission buffer index if an unused buffer was found. */
      SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).SockIdx = SockIdx;
      *SockTxBufIdxPtr = sockTxBufIdx;
      retVal = E_OK;
      break;
    }
  }

  /* #30 Leave critical section. */
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_UpdateSocketTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpBsd_UpdateSocketTxBuffer(
  SoAd_SockTxBufIdxType SockTxBufIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockTxBufIdxType_least sockTxBufIdxTmp;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket transmission buffer struct is changed via any API call. */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if there is an unused socket transmission buffer. */
  for ( sockTxBufIdxTmp = 0u; sockTxBufIdxTmp < SoAd_SocketTxBufferNum(); sockTxBufIdxTmp++ )
  {
    if ( SoAd_SockTxBufIdx2Mgt(sockTxBufIdxTmp).SockIdx == SOAD_INV_SOCK_IDX )
    {
      retVal = E_OK;
      break;
    }
  }

  /* #30 Update requested socket transmission buffer if an unused element was found. */
  if ( retVal == E_OK )
  {
    SoAd_SockTxBufIdx2Mgt(SockTxBufIdx).Len = Length;
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ReleaseSocketTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_ReleaseSocketTxBuffer(
  SoAd_SockTxBufIdxType SockTxBufIdx)
{
  /* #10 Release socket transmission buffer. */
  SoAd_SockTxBufIdx2Mgt(SockTxBufIdx).SockIdx = SOAD_INV_SOCK_IDX;
  SoAd_SockTxBufIdx2Mgt(SockTxBufIdx).Len     = 0u;
}

/**********************************************************************************************************************
 *  SoAd_TcpIpBsd_ReleaseSocketTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpIpBsd_HandleSocketTxBuffer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockTxBufIdxType_least sockTxBufIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket transmission buffer struct is changed via any API call. */
  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if there is an used socket transmission buffer. */
  for ( sockTxBufIdx = 0u; sockTxBufIdx < SoAd_SocketTxBufferNum(); sockTxBufIdx++ )
  {
    SoAd_SockIdxType  sockIdx = SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).SockIdx;
    uint16            len = SoAd_SockTxBufIdx2Mgt(sockTxBufIdx).Len;

    /* #30 Check if the unused socket transmission buffer is updated and therefore marked for retry. */
    if ( (sockIdx != SOAD_INV_SOCK_IDX) && (len != 0u) )
    {
      SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);

      /* #40 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Call BSD Socket API for transmission. */
      if ( send(socketId, SOAD_A_P2CONST(void)&SoAd_SocketTxBuffer()[sockTxBufIdx], len, SOAD_ZERO_FLAGS) !=
        SOAD_E_BSD_NOT_OK )
      {
        /* #60 Release socket transmission buffer if call was successful. */
        SoAd_TcpIpBsd_ReleaseSocketTxBuffer(sockTxBufIdx);
      }

      /* #70 Enter critical section again to continue with other transmission buffers. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* #80 Leave critical section. */
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
}
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VPDU_ROUTES == STD_ON */
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

#if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  SoAd_TcpIpAsr_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpIpAsr_RequestIpAddrAssignment(
  SoAd_SoConIdxType SoConIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType  localIpAddr;
  SoAd_SockAddrInetXType  defaultRouter;
  uint8                   netMask;
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Do not forward request to TcpIp module if same static IP address is requested. */
  if ( Type == SOAD_IPADDR_ASSIGNMENT_STATIC )
  {
    /* #100 Retrieve local IP address. */
    if ( TcpIp_GetIpAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId,
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&localIpAddr,
        &netMask,
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&defaultRouter) == E_OK )
    {
      /* #1000 Compare new IP address to local IP address. */
      if ( SoAd_Util_CompareIpAddr(SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
        localIpAddr.addr,
        ((P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, AUTOMATIC))LocalIpAddrPtr)->addr,
        FALSE) == E_OK)
      {
        retVal = E_OK;
      }
    }
  }

  /* #20 Forward request to TcpIp module otherwise. */
  if ( retVal == E_NOT_OK )
  {
    retVal = TcpIp_RequestIpAddrAssignment(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId, Type,
      LocalIpAddrPtr, Netmask, DefaultRouterPtr);
  }

  return retVal;
}
#endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_OFF */

/**********************************************************************************************************************
 *  SoAd_SoCon_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType_least soConIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all socket connection indexes. */
  for ( soConIdx = 0u; soConIdx < SoAd_SockConTotal(); soConIdx++ )
  {
    /* #20 Initialize general struct elements. */
    SoAd_SockConIdx2State(soConIdx)       = SOAD_SOCON_OFFLINE;
    SoAd_SockConIdx2IpAddrState(soConIdx) = SOAD_IPADDR_STATE_UNASSIGNED;
    SoAd_SockConIdx2FlagAbort(soConIdx)   = FALSE;
    SoAd_SockConIdx2CloseMode(soConIdx)   = SOAD_CLOSE_NONE;
    SoAd_SockConIdx2FlagCntOpCl(soConIdx) = 0u;
    SoAd_ResetIpAddr((SoAd_SoConIdType)soConIdx);
#if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
    if ( SoAd_SockCons(soConIdx).RcvRemSockPtr != NULL_PTR )
    { /* get received remote address is supported */
      /* reset remote address */
      SoAd_ResetIpAddr2Unspec(SoAd_SockCons(soConIdx).RcvRemSockPtr);
    }
#endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */

    /* #30 Initialize tx struct elements. */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    if ( SoAd_PduRouteTotal() != 0u )
    {
      SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx        = SOAD_INV_PDU_IDX;
      SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending  = 0u;
      SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen      = 0u;
      SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr        = NULL_PTR;
      SoAd_SockConIdx2TxMgt(soConIdx).CancelRequested = FALSE;

# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
      if ( SoAd_IsSockConIdx2TxBufferPresent() &&
        (SoAd_SockConIdx2TxBuffer(soConIdx).TpTxBufferOffsetPtr != NULL_PTR) )
      {
        *SoAd_SockConIdx2TxBuffer(soConIdx).TpTxBufferOffsetPtr = 0u;
      }
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */
# if ( SOAD_VTCP_USED == STD_ON )
      if ( SoAd_SockCons(soConIdx).TcpTxQueuePtr != NULL_PTR )
      {
        uint16_least idx;

        SoAd_SockCons(soConIdx).TcpTxQueuePtr->MgtPtr->Idx = 0u;
        SoAd_SockCons(soConIdx).TcpTxQueuePtr->MgtPtr->Lvl = 0u;
        for ( idx = 0u; idx < SoAd_SockCons(soConIdx).TcpTxQueuePtr->Size; idx++ )
        {
          SoAd_SockCons(soConIdx).TcpTxQueuePtr->QueuePtr[idx].TxPduIdx = SOAD_INV_PDU_IDX;
          SoAd_SockCons(soConIdx).TcpTxQueuePtr->QueuePtr[idx].Len = 0u;
        }
      }
# endif /* SOAD_VTCP_USED == STD_ON */
    }
#endif /* SOAD_VPDU_ROUTES == STD_ON */

    /* #40 Initialize rx struct elements. */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
    if ( SoAd_IsSockRoutePresent() )
    {
      SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx                 = SOAD_INV_PDU_ID;
      SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending           = 0u;
      SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Idx            = 0u;
      SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Lvl            = 0u;
      SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested          = FALSE;
      if ( SoAd_SockConIdx2RxBuffer(soConIdx).IfRxBufferOffsetPtr != NULL_PTR )
      {
        *SoAd_SockConIdx2RxBuffer(soConIdx).IfRxBufferOffsetPtr = 0u;
      }
      if ( SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr != NULL_PTR )
      {
        *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr = 0u;
      }
    }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  }
}

/**********************************************************************************************************************
 *  SoAd_SoCon_UdpChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_UdpChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VFRAME_PRIORITY_ENABLED == STD_ON )
  /* #10 Set frame priority. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->FramePriority != SOAD_INV_FRAME_PRIORITY )
  {
    uint8 framePriority = SoAd_SockCons(SoConIdx).SockConGrpPtr->FramePriority;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_FRAMEPRIO, &framePriority);
  }
# endif /* SOAD_VFRAME_PRIORITY_ENABLED == STD_ON */

# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
  /* #20 Set UDP Tx request list size. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConfListSize != 0u )
  {
    uint8 udpTxReqListSize = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConfListSize;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_UDP_TXREQLISTSIZE, &udpTxReqListSize);
  }
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */

# if ( SOAD_VUDP_RETRY_ENABLED == STD_ON )
  /* #30 Set UDP Tx retry list size (number of Eth Tx buffers used in case of physical address miss). */
  if ( SoAd_TcpIpCtrl(SoAd_AddrId2TcpIpCtrl(SoAd_SockCons(SoConIdx).SockConGrpPtr->LocIpAddrId)).RetryEnabled == TRUE )
  {
    uint8 udpTxRetryListSize = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->RetryQueueLimit;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_UDP_TXRETRYLISTSIZE, &udpTxRetryListSize);
  }
# endif /* SOAD_VUDP_RETRY_ENABLED == STD_ON */

  /* #40 Set stack (Socket API) specific parameters. */
  retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_STACK_SPECIFIC, NULL_PTR);

  return retVal;
}

#if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameter(
  SoAd_SoConIdxType SoConIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VFRAME_PRIORITY_ENABLED == STD_ON )
  /* #10 Set frame priority. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->FramePriority != SOAD_INV_FRAME_PRIORITY )
  {
    uint8 framePriority = SoAd_SockCons(SoConIdx).SockConGrpPtr->FramePriority;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_FRAMEPRIO, &framePriority);
  }
# endif /* SOAD_VFRAME_PRIORITY_ENABLED == STD_ON */

  /* #20 Set rx buffer size. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin > 0u )
  {
    uint32 sockTpRxBufMin = SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_RXWND_MAX, (uint8*)&sockTpRxBufMin);
  }

# if ( SOAD_VTCP_TX_BUF_MIN_ENABLED == STD_ON )
  /* #30 Set tx buffer size. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpTxBufMin > 0u )
  {
    uint32 sockTcpTxBufMin = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpTxBufMin;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_TXWND_MAX, (uint8*)&sockTcpTxBufMin);
  }
# endif /* SOAD_VTCP_TX_BUF_MIN_ENABLED == STD_ON */

  /* #40 Set no delay (Nagle algorithm). */
  switch ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpNoDelay )
  {
    case SOAD_TCP_NO_DELAY_ENABLED:
    {
      uint8 value = 0x00u;

      retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_NAGLE, &value);
      break;
    }
    case SOAD_TCP_NO_DELAY_DISABLED:
    {
      uint8 value = 0x01u;

      retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_NAGLE, &value);
      break;
    }
    default:
    { /* SOAD_TCP_NO_DELAY_NOT_SPECIFIED */
      /* nothing to do */
    }
  }

  /* #50 Set keep alive. */
# if ( SOAD_VTCP_KEEP_ALIVE_ENABLED == STD_ON )
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  if ( SoAd_TcpKeepAliveEnabled() == TRUE )
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  {
    uint8 tcpKeepAlive = (uint8)SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpKeepAlive;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_KEEPALIVE, &tcpKeepAlive);
    if ( (retVal == E_OK) && (tcpKeepAlive == TRUE) )
    {
      uint32 tcpKeepAliveTime = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpKeepAliveTime;
      uint32 tcpKeepAliveProbesMax = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpKeepAliveProbesMax;
      uint32 tcpKeepAliveInterval = SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpKeepAliveInterval;

      /* #60 Set keep alive time. */
      if ( tcpKeepAliveTime != SOAD_INV_KEEP_ALIVE_VALUE )
      {
        retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_KEEPALIVE_TIME,
          SOAD_A_P2VAR(uint8)&tcpKeepAliveTime);
      }
      /* #70 Set keep alive maximum probes. */
      if ( tcpKeepAliveProbesMax != SOAD_INV_KEEP_ALIVE_VALUE )
      {
        retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_KEEPALIVE_PROBES_MAX,
          SOAD_A_P2VAR(uint8)&tcpKeepAliveProbesMax);
      }
      /* #80 Set keep alive interval. */
      if ( tcpKeepAliveInterval != SOAD_INV_KEEP_ALIVE_VALUE )
      {
        retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TCP_KEEPALIVE_INTERVAL,
          SOAD_A_P2VAR(uint8)&tcpKeepAliveInterval);
      }
    }
  }
# endif /* SOAD_VTCP_KEEP_ALIVE_ENABLED == STD_ON */

# if ( SOAD_VTCP_TLS_ENABLED == STD_ON )
  /* #90 Set TLS. */
  if ( SoAd_SockCons(SoConIdx).TcpTlsConfigPtr != NULL_PTR )
  {
    uint8 tlsEnabled = TRUE;

    retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_USE_TLS, &tlsEnabled);
    if ( retVal == E_OK )
    {
      uint32 bufSize;

      /* #100 Set TLS rx buffer size. */
      bufSize = SoAd_SockCons(SoConIdx).TcpTlsConfigPtr->RxBufSize;
      retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TLS_RXBUFSIZE,
        SOAD_A_P2VAR(uint8)&bufSize);

      /* #110 Set TLS tx buffer size. */
      bufSize = SoAd_SockCons(SoConIdx).TcpTlsConfigPtr->TxBufSize;
      retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_TLS_TXBUFSIZE,
        SOAD_A_P2VAR(uint8)&bufSize);

      /* #120 Notify about TLS soket creation. */
      if ( SoAd_SockCons(SoConIdx).TcpTlsConfigPtr->FctIdTlsNotif != SOAD_FCT_ID_TLS_NOTIF_INV )
      {
        SoAd_VCallTlsSockCreatedNotifFunc(SoAd_SockCons(SoConIdx).TcpTlsConfigPtr->FctIdTlsNotif,
          SoConIdx, SocketId);
      }
    }
  }
# endif /* SOAD_VTCP_TLS_ENABLED == STD_ON */

  /* #130 Set stack (Socket API) specific parameters. */
  retVal |= SoAd_TcpIp_ChangeParameter(SoConIdx, SocketId, SOAD_PARAMID_STACK_SPECIFIC, NULL_PTR);

  return retVal;
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
#endif /* SOAD_VTCP_USED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndSetRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_CheckAndSetRemoteAddr(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compare remote address to corresponding local IP addresses to prevent setting local IP address as remote. */
  if ( E_NOT_OK == SoAd_TcpIp_CompareToIpAddr(SoConIdx, ((SOAD_P2CONST(SoAd_SockAddrInetXType))RemoteAddrPtr)->addr) )
  {
    boolean addrIsAny = FALSE;
    boolean portIsAny = FALSE;

    /* #20 Copy remote address to local struct. */
    SoAd_CopyIpAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx), RemoteAddrPtr, TRUE);

    /* #30 Set remote address state considering wildcards in remote address to be set. */
    SoAd_CheckForWildcards((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx), &addrIsAny,
      &portIsAny);
    if ( TRUE == addrIsAny )
    {
      SoAd_SockConIdx2RemAddrState(SoConIdx) =
        (uint8)(SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
    else
    {
      SoAd_SockConIdx2RemAddrState(SoConIdx) =
        (uint8)(SOAD_SOCON_IP_SET_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
    if ( TRUE == portIsAny )
    {
      SoAd_SockConIdx2RemAddrState(SoConIdx) |=
        (uint8)(SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_PORT);
    }
    else
    {
      SoAd_SockConIdx2RemAddrState(SoConIdx) |=
        (uint8)(SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_PORT);
    }

    retVal = E_OK;
  }

  return retVal;
}

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateUdpAliveTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateUdpAliveTimeout(
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          tmpCnt = 0u;
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if timeout is configured. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt != 0u )
  {
    /* #20 Check if timeout is already running. */
    retVal = SoAd_TimeoutList_GetCurrentTimeout(SoAd_TimeoutListIdxUdpAlive(), SoConIdx, &tmpCnt, SoAd_GlobalCounter);

    if ( retVal == E_OK )
    {
      /* #30 Update timeout. */
      (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxUdpAlive(), SoConIdx,
        SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt, SoAd_GlobalCounter);
    }
  }
}
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_RouteGrp_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RouteGrp_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RoutingGroupIdxType_least idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all routing groups. */
  for ( idx = 0u; idx < SoAd_RouteGrpTotal(); idx++ )
  {
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
    uint16 pduDestNum = 0;
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
    uint16 sockRouteNum = 0;
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
    /* #20 Initialize routing group states on PduRouteDests. */
    if ( SoAd_IsRouteGrpIdx2PduRouteDestColPresent() )
    {
      pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(idx).PduRouteDestListSize;
    }
    while ( pduDestNum  > 0u )
    {
      pduDestNum--;
      if ( SoAd_RouteGrpIdx2RoutingGroup(idx).IsEnabledAtInit == TRUE )
      {
        *SoAd_RouteGrpIdx2PduRouteDestCol(idx).StateColPtr[pduDestNum] = SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
      }
      else
      {
        *SoAd_RouteGrpIdx2PduRouteDestCol(idx).StateColPtr[pduDestNum] = SOAD_ROUT_GRP_BIT_PATTERN_NONE;
      }
    }
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
    /* #30 Initialize routing group states on SocketRoutes. */
    if ( SoAd_IsRouteGrpIdx2SocketRouteColPresent() )
    {
      sockRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(idx).IdNum;
    }
    while ( 0u < sockRouteNum )
    {
      sockRouteNum--;
      if ( SoAd_RouteGrpIdx2RoutingGroup(idx).IsEnabledAtInit == TRUE )
      {
        *SoAd_RouteGrpIdx2SocketRouteCol(idx).StateColPtr[sockRouteNum] = SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
      }
      else
      {
        *SoAd_RouteGrpIdx2SocketRouteCol(idx).StateColPtr[sockRouteNum] = SOAD_ROUT_GRP_BIT_PATTERN_NONE;
      }
    }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
  }
}
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Tx_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Tx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduIdxType idx;

  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Iterate over all PduRoutes. */
  for ( idx = 0u; idx < SoAd_PduRouteTotal(); idx++ )
  {
    /* #100 Initialize PduRoute structs. */
    SoAd_TxPduIdx2PendingTxConf(idx) = 0u;
  }

  /*  #20 Iterate over all nPdus. */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
  if ( SoAd_IsNPduUdpTxPresent() )
  {
    /* #20 Iterate over all nPdus. */
    for ( idx = 0u; idx < SoAd_SockConIdx2NPduUdpTxIdxSize(); idx++ )
    {
      /* #200 Initialize nPdu structs. */
      SoAd_NPduClear(idx);
    }
  }
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
}

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check module state. */
  if ( SoAd_State == SOAD_STATE_INIT )
  {
    /* #20 Check PDU length. */
    if ( SoAd_TxIf_CheckTxPduLength(PduRouteIdx, PduLength) == E_OK )
    {
      /* #30 Check transmission state. */
      if ( SoAd_TxIf_CheckTxState(PduRouteIdx) == E_OK )
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxPduLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxPduLength(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           minLength;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate minimum length in case PDU Header option is used. */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  if ( SoAd_TxPduIdx2PduRoute(PduRouteIdx).AllPduRouteDestWithPduHdr == TRUE )
  {
#  if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
    if ( SoAd_TxPduIdx2PduRoute(PduRouteIdx).MetaDataTxEnabled == TRUE )
    {
      minLength = 2u;
    }
    else
#  endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */
    {
      minLength = 0u;
    }
  }
  else
# endif /* (SOAD_VPDU_HDR_ENABLED == STD_ON */
  /* #20 Calculate minimum length in case PDU Header option is not used. */
  {
# if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
    if ( SoAd_TxPduIdx2PduRoute(PduRouteIdx).MetaDataTxEnabled == TRUE )
    {
      minLength = 3u;
    }
    else
# else
    SOAD_DUMMY_STATEMENT_VAR(PduRouteIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */
    {
      minLength = 1u;
    }
  }

  /* #30 Validate minimum length. */
  if ( PduLength >= minLength )
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxState(
  SoAd_PduIdxType PduRouteIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  SoAd_SoConIdxType soConIdx = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[0]->SoConIdx;
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
  Std_ReturnType    retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a TxConfirmation is pending on the requested PDU */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  if ( (!SoAd_IsNPduUdpTxPresent())
    || (SoAd_SockConIdx2NPduUdpTxIdx(soConIdx) == SOAD_INV_BUF_IDX_16)
    || (SoAd_TxPduIdx2PduRoute(PduRouteIdx).IfTriggerTransmitEnabled == FALSE) )
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
  {
    /* check for pending TxConfirmation */
    if ( SoAd_TxPduIdx2PendingTxConf(PduRouteIdx) != 0u )
    {
      retVal = E_NOT_OK;

      /* switch off TxConfirmation if faster transmission is required */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  uint16            pduRouteDestIdx = 0u;
  uint16            pduRouteDestNum = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListSize;
  Std_ReturnType    retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if meta data are used to send over a specific PduRouteDest. */
# if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
  if ( SoAd_TxPduIdx2PduRoute(PduRouteIdx).MetaDataTxEnabled == TRUE )
  {
    /* #100 Get PduRouteDest by extracting SoConId from meta data. */
    if ( SoAd_TxIf_GetPduRouteDestIdxByMetaData(PduRouteIdx, PduInfoPtr, &pduRouteDestIdx) == E_OK )
    {
      P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) pduRouteDestPtr =
        SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[pduRouteDestIdx];

      soConIdx = pduRouteDestPtr->SoConIdx;

      /* #1000 Check if any routing group is enabled on the PduRouteDest. */
#  if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      if ( SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled(pduRouteDestPtr) == TRUE )
#  endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
      {
        /* #10000 If enabled transmit PDU over PduRouteDest. */
        PduInfoType pduInfo;

        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength = (PduLengthType)(PduInfoPtr->SduLength - 2u);

        retVal = SoAd_TxIf_TransmitPduRouteDest(soConIdx, &pduInfo, pduRouteDestPtr);
      }
      else
      {
        /* #10001 If disabled reject transmit request. */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      retVal = E_NOT_OK;
#  if ( SOAD_DEV_ERROR_DETECT == STD_ON )
      *ErrorIdPtr = SOAD_E_INV_METADATA;
#  else
      SOAD_DUMMY_STATEMENT_VAR(ErrorIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }
  else
# else
  SOAD_DUMMY_STATEMENT_VAR(ErrorIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */
  /* #11 Send over all PduRouteDests otherwise. */
  {
    uint16 pduRouteDestsDisabled = 0u;

    /* #110 Iterate over all PduRouteDests. */
    while ( pduRouteDestIdx < pduRouteDestNum )
    {
      P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) pduRouteDestPtr =
        SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[pduRouteDestIdx];

      soConIdx = pduRouteDestPtr->SoConIdx;

      /* #1100 Check if any routing group is enabled on the PduRouteDest. */
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      if ( FALSE == SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled(pduRouteDestPtr) )
      { /* all routing groups for this PduRouteDest are disabled */
        /* discard PDU for this PduRouteDest but consider that transmission is successful */
        /* count all disabled PduRouteDests */
        pduRouteDestsDisabled++;
      }
      else
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
      {
        /* #11000 If enabled transmit PDU over PduRouteDest. */
        retVal |= SoAd_TxIf_TransmitPduRouteDest(soConIdx, PduInfoPtr, pduRouteDestPtr);
      }

      /* switch to next PduRouteDest */
      pduRouteDestIdx++;
    }

    /* #11001 If no routing group is enabled for any PduRouteDest reject transmit request. */
    if ( pduRouteDestNum == pduRouteDestsDisabled )
    { /* all PduRouteDests are disabled */
      /* return E_NOT_OK */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

# if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduRouteDestIdxByMetaData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduRouteDestIdxByMetaData(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16            soConId;
  SoAd_SoConIdxType soConIdx;
  uint16            pduRouteDestIdx;
  uint16            pduRouteDestNum = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListSize;
  Std_ReturnType    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Extract SoConId from meta data. */
  IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr, (PduInfoPtr->SduLength - 2u), soConId);

  /* #11 Check if extracted SoConId is valid. */
  if ( SoAd_IsSoConIdValid((SoAd_SoConIdType)soConId) )
  {
    /* #110 Iterate over all PduRouteDests to find the PduRouteDest with extracted SoConId. */
    for ( pduRouteDestIdx = 0u; pduRouteDestIdx < pduRouteDestNum; pduRouteDestIdx++ )
    {
      soConIdx = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[pduRouteDestIdx]->SoConIdx;
      if ( (SoAd_SoConIdType)soConId == SoAd_SockCons(soConIdx).SoConId )
      {
        *PduRouteDestIdxPtr = pduRouteDestIdx;
        retVal = E_OK;
        break;
      }
    }
  }

  return retVal;
}
# endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPduRouteDest()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPduRouteDest(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr)
{
  Std_ReturnType   retVal  = E_NOT_OK;
  SoAd_PduIdxType  txPduIdx = SoAd_TxPduId2PduRouteIdx(PduRouteDestPtr->TxPduId);
  PduLengthType    dataLen = PduInfoPtr->SduLength;
  SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

  /* check socket state, socket connection state and transmission/reception state */
  if ( (SOAD_SOCK_STATE_ESTABLISHED == SoAd_SockIdx2SockState(sockIdx)) &&
    (SOAD_SOCON_ONLINE == SoAd_SockConIdx2State(SoConIdx)) &&
    (SOAD_CLOSE_NONE == SoAd_SockConIdx2CloseMode(SoConIdx)) )
  { /* states are valid */
    if ( (SOAD_TX_CONF_STRAT_BIT_PAT_CONF & SoAd_TxPduIdx2PendingTxConf(txPduIdx)) < SOAD_TX_CONF_STRAT_BIT_PAT_CONF )
    { /* TxConfirmation queue is sufficient */
      /* further handling depends on socket configuration */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
      { /* PDU Header ID is enabled */
        dataLen += SOAD_PDU_HDR_SIZE;
      }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

# if ( SOAD_VTCP_USED == STD_ON )
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
      { /* TCP socket */
        /* store PDU in global PDU struct for CopyTxData */
        SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if ( (SOAD_INV_SO_CON_IDX == SoAd_SockIdx2ActiveSoConIdx(sockIdx)) )
        {
          /* set local socket index to active socket connection mapping */
          SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SoConIdx;
           /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#  if ( SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON )
          if ( SoAd_TxPduIdx2PduRoute(txPduIdx).IfTriggerTransmitEnabled == TRUE )
          { /* trigger transmit */
            uint16 bufIdx = SoAd_SockConIdx2IfTriggerTransmitBufIdx(SoConIdx);
            PduInfoType info;

            info.SduDataPtr = SoAd_IfTriggerTransmitBuffer(bufIdx).BufPtr;
            info.SduLength = SoAd_IfTriggerTransmitBuffer(bufIdx).MaxLen;

            if ( SoAd_VCallIfTriggerTransmit(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx), &info) == E_OK )
            { /* trigger transmit successful */
              uint8 minLen = 1u;

              /* check out parameter and update data length */
              dataLen = 0u;
#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
              {
                minLen = 0u;
                dataLen = SOAD_PDU_HDR_SIZE;
              }
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

              if ( (info.SduDataPtr != NULL_PTR) && (info.SduLength >= minLen) &&
                (info.SduLength <= SoAd_IfTriggerTransmitBuffer(bufIdx).MaxLen) )
              { /* copied valid length */
                dataLen += info.SduLength;

                /* set tx management struct for CopyTxData */
                SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = txPduIdx;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = dataLen;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = dataLen;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr = SoAd_IfTriggerTransmitBuffer(bufIdx).BufPtr;
              }
            }
          }
          else
#  endif /* SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON */
          { /* direct transmit */
            /* set tx management struct for CopyTxData */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = txPduIdx;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = dataLen;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = dataLen;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr = PduInfoPtr->SduDataPtr;
          }

#  if ( SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON )
          if ( SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx != SOAD_INV_PDU_IDX )
#  endif /* SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON */
          {
            if ( E_OK == SoAd_TcpTxQueueReserveNextElement(SoConIdx, txPduIdx) )
            {
              if ( E_OK == SoAd_TcpIp_TcpTransmit(SoConIdx, dataLen) )
              { /* transmission successful */
                SoAd_TcpTxQueueSetElement(SoConIdx, txPduIdx, SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen, FALSE);

                /* handle TxConfirmation */
                SoAd_TxIf_AddPendingConf(txPduIdx, SoConIdx);

                retVal = E_OK;
              }
              else
              { /* transmission failed */
                SoAd_TcpTxQueueSetElement(SoConIdx, txPduIdx, 0, TRUE);
              }
            }

            /* reset tx management struct */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr = NULL_PTR;
          }
        }
        else
        {
         /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }

        /* reset local socket index to active socket connection mapping */
        SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;
      }
      else
# endif /* SOAD_VTCP_USED == STD_ON */
      { /* UDP socket */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
        uint16 nPduUdpTxIdx = SOAD_INV_BUF_IDX_16;
        if ( SoAd_IsNPduUdpTxPresent() )
        {
          nPduUdpTxIdx = SoAd_SockConIdx2NPduUdpTxIdx(SoConIdx);
        }
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */

        retVal = E_NOT_OK;
# if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
        /* update alive timeout if configured */
        SoAd_SoCon_UpdateUdpAliveTimeout(SoConIdx);
# endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
        if ( SOAD_INV_BUF_IDX_16 != nPduUdpTxIdx )
        { /* nPduUdpTx is configured */
          if ( dataLen <= SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin )
          { /* PDU size is valid */
#  if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
#   if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
            if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr != NULL_PTR )
#   endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
            { /* NPduUdpTxQueue is used */
              switch ( SoAd_NPduQueueAddElement(txPduIdx, PduRouteDestPtr, PduInfoPtr) )
              {
                case SOAD_NPDU_OVFL_NEW:
                { /* remaining length is not sufficient for new PDU */
                  /* send queue now */
                  SoAd_NPduQueueSend(nPduUdpTxIdx);
                  /* add PDU */
                  if ( SoAd_NPduQueueAddElement(txPduIdx, PduRouteDestPtr, PduInfoPtr) == SOAD_NPDU_OK )
                  { /* PDU added */
                    /* check if PDU triggers transmission */
                    if ( PduRouteDestPtr->TxUdpTrigMode == SOAD_TX_UDP_TRIGGER_ALWAYS )
                    { /* PDU triggers transmission */
                      /* send queue now */
                      SoAd_NPduQueueSend(nPduUdpTxIdx);
                    }
                    retVal = E_OK;
                  }
                  break;
                }
                case SOAD_NPDU_OVFL_UPDATE:
                { /* remaining length is not sufficient for updated PDU */
                  /* send queue now (length has been updated) */
                  SoAd_NPduQueueSend(nPduUdpTxIdx);
                  retVal = E_OK;
                  break;
                }
                case SOAD_NPDU_OK:
                { /* remaining length is sufficient for new PDU */
                  if ( PduRouteDestPtr->TxUdpTrigMode == SOAD_TX_UDP_TRIGGER_ALWAYS )
                  { /* PDU triggers transmission */
                    /* send queue now */
                    SoAd_NPduQueueSend(nPduUdpTxIdx);
                  }
                  retVal = E_OK;
                  break;
                }
                default: /* SOAD_NPDU_NOT_OK */
                { /* PDU cannot be sent */
                  /* nothing to do */
                }
              }
            }
#   if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
            else
#   endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
#  endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
#  if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
            { /* NPduUdpTxBuffer is used */
              SoAd_NPduReturnType nPduRetVal = SoAd_NPduBufAddElement(txPduIdx, PduRouteDestPtr, PduInfoPtr);

              if ( nPduRetVal == SOAD_NPDU_OK )
              { /* remaining length is sufficient */
                /* check if PDU triggers transmission */
                if ( PduRouteDestPtr->TxUdpTrigMode == SOAD_TX_UDP_TRIGGER_ALWAYS )
                { /* PDU triggers transmission */
                  /* send queue now */
                  SoAd_NPduBufSend(nPduUdpTxIdx);
                }
                retVal = E_OK;
              }
              else if ( nPduRetVal == SOAD_NPDU_OVFL_NEW )
              { /* remaining length is not sufficient */
                /* send buffer now */
                SoAd_NPduBufSend(nPduUdpTxIdx);
                /* try to add PDU again */
                if ( SoAd_NPduBufAddElement(txPduIdx, PduRouteDestPtr, PduInfoPtr) == SOAD_NPDU_OK )
                { /* added PDU */
                  /* check if PDU triggers transmission */
                  if ( PduRouteDestPtr->TxUdpTrigMode == SOAD_TX_UDP_TRIGGER_ALWAYS )
                  { /* PDU triggers transmission */
                    /* send queue now */
                    SoAd_NPduBufSend(nPduUdpTxIdx);
                  }
                  retVal = E_OK;
                }
              }
              else
              { /* nPduRetVal == SOAD_NPDU_NOT_OK */
                /* PDU could not be added to nPduUdpTxBuffer */
                /* nothing to do */
              }
            }
#  endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
          }
          else
          { /* requested PDU is larger than the the nPduUdpTxBuffer */
#  if ( SOAD_DEV_ERROR_REPORT == STD_ON )
            (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_VIF_TRANSMIT, SOAD_E_NOBUFS);
#  endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
          }
        }
        else
# endif /* defined(SOAD_VN_PDU_UDP_TX_BUF_ENABLED) && (STD_ON == SOAD_VN_PDU_UDP_TX_BUF_ENABLED) */
        { /* nPduUdpTx is not used */
          /* try to send PDU */
          /* store PDU in global PDU struct for CopyTxData */
          SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if ( SOAD_INV_SO_CON_IDX == SoAd_SockIdx2ActiveSoConIdx(sockIdx) )
          {
            /* set local socket index to active socket connection mapping */
            SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SoConIdx;
            /* set tx management struct for CopyTxData */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = txPduIdx;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = dataLen;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = dataLen;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr = PduInfoPtr->SduDataPtr;
             /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
            SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            /* try to send new PDU */
            if ( E_OK == SoAd_TcpIp_UdpTransmit(sockIdx, SoAd_SockIdx2SocketId(sockIdx),
              (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(SoConIdx),
              (uint16)dataLen) )
            { /* transmission successful */
              /* handle TxConfirmation */
# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConf != TRUE )
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
              { /* No UdpImmediateIfTxConfirmation is configured */
                SoAd_TxIf_AddPendingConf(txPduIdx, SoConIdx);
              }

              retVal = E_OK;
            }

            /* reset tx management struct */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr = NULL_PTR;

            /* reset local socket index to active socket connection mapping */
            SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;
          }
          else
          {
             /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
            SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          }
        }
      }
    }
    else
    {
# if ( SOAD_DEV_ERROR_REPORT == STD_ON )
      (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_VIF_TRANSMIT, SOAD_E_NOBUFS);
# endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
    }
  }
  return retVal;
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConf(
  SoAd_PduIdxType TxPduIdx,
  SoAd_SoConIdxType SoConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TxConfirmation is enabled. */
  if ( TRUE == SoAd_TxPduIdx2PduRoute(TxPduIdx).TxConfEnabled )
  {
    /* #20 Check if API type is IF. */
    if ( SoAd_TxPduIdx2PduRoute(TxPduIdx).UpperLayer == SOAD_UL_TYPE_IF )
    {
      /* #30 Check if socket type is UDP. */
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
      {
        /* #31 Check if nPdu is used. */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
        if ( SoAd_IsNPduUdpTxPresent() && (SoAd_SockConIdx2NPduUdpTxIdx(SoConIdx) != SOAD_INV_BUF_IDX_16) &&
          (SoAd_TxPduIdx2PduRoute(TxPduIdx).IfTriggerTransmitEnabled == TRUE) )
        {
          /* #310 Increment pending TxConfirmation counter to confirm after TriggerTransmit call. */
          SoAd_TxPduIdx2PendingTxConf(TxPduIdx)++;
        }
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */

        /* #32 Check if UdpImmediateIfTxConfirmation is disabled. */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
        else
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConf == FALSE )
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
        {
          /* #320 Set flag to call TxConfirmation in next main function. */
          /* may be set multiple times if multiple PduRoutesDests are configured with this confirmation strategy */
          SoAd_TxPduIdx2PendingTxConf(TxPduIdx) |= SOAD_TX_CONF_STRAT_BIT_PAT_MAIN;
          /* set event to handle TxConfirmation in main function */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxIfUdpPduRoute(), TxPduIdx);
        }
# if ( (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) && (SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON) )
        else
        {
          /* nothing to do */
        }
# endif /* (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) && (SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON) */
      }
      /* #40 Check if socket type is TCP. */
# if ( SOAD_VTCP_USED == STD_ON )
      else
      {
        /* #400 Increment pending TxConfirmation counter to confirm in SoAd_TxConfirmation. */
        SoAd_TxPduIdx2PendingTxConf(TxPduIdx)++;
      }
# endif /* SOAD_VTCP_USED == STD_ON */
    }
  }
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) pduRouteDestPtr =
    SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[0];
  SoAd_SoConIdxType   soConIdx = pduRouteDestPtr->SoConIdx;
  Std_ReturnType      retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check module state. */
  if ( SoAd_State == SOAD_STATE_INIT )
  {
    /* #20 Check PDU length. */
    if ( PduLength > 0u )
    {
      /* #30 Check socket connection state. */
      if ( (SoAd_SockConIdx2CloseMode(soConIdx) == SOAD_CLOSE_NONE) &&
        (SoAd_SockConIdx2State(soConIdx) == SOAD_SOCON_ONLINE) )
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) pduRouteDestPtr =
    SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[0];
  SoAd_SoConIdxType   soConIdx = pduRouteDestPtr->SoConIdx;
  Std_ReturnType      retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  /* #10 Check if any routing group is enabled for this PduRouteDest. */
  if ( SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled(pduRouteDestPtr) == TRUE )
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
  {
    /* #20 Enter critical section to prevent interruption by another transmission request. */
    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Check if corresponding socket connection is not used for another transmission. */
    if ( SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(soConIdx).SockIdx) == SOAD_INV_SO_CON_IDX)
    {
      /* #40 Check and set protocol (TCP/UDP) specific parameter. */
      if ( SoAd_TxTp_CheckAndSetProtocolParameter(PduRouteIdx, PduInfoPtr->SduLength) == E_OK )
      {
        /* #50 Prepare transmission on socket connection. */
        SoAd_TxTp_PrepareTransmitPdu(PduRouteIdx, PduInfoPtr->SduLength);

        retVal = E_OK;
      }
    }

    /* #60 Leave critical section. */
    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* #70 Check if transmission is prepared. */
  if ( retVal == E_OK )
  {
# if ( SOAD_VTX_TP_OPTIMIZED_ENALBED == STD_ON )
    /* #80 Handle transmission in this context if TpOptimized is enabled for this PDU. */
    if ( SoAd_TxPduIdx2PduRoute(PduRouteIdx).TpOptimized == TRUE )
    {
      retVal = SoAd_HandleTpTransmissionOnSockCon(soConIdx, FALSE);
    }
    else
# endif /* SOAD_VTX_TP_OPTIMIZED_ENALBED == STD_ON */
    {
      /* #81 Add event to corresponding event queue otherwise to handle transmission in main function context. */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpTxSoCon(), soConIdx);
    }
  }

  return retVal;
}
/* PRQA L:MSR_STPTH */

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckAndSetProtocolParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckAndSetProtocolParameter(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( (SOAD_VTCP_USED == STD_ON) || (SOAD_VTX_BUF_ENABLED == STD_ON) )
  SoAd_SoConIdxType   soConIdx = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[0]->SoConIdx;
# endif /* (SOAD_VTCP_USED == STD_ON) || (SOAD_VTX_BUF_ENABLED == STD_ON) */
  Std_ReturnType      retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( (SOAD_VTCP_USED == STD_ON) || (SOAD_VTX_BUF_ENABLED == STD_ON) )
  retVal = E_NOT_OK;
# else
  SOAD_DUMMY_STATEMENT_VAR(PduRouteIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_VAR(PduLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  retVal = E_OK;
# endif /* (SOAD_VTCP_USED == STD_ON) || (SOAD_VTX_BUF_ENABLED == STD_ON) */

# if ( SOAD_VTCP_USED == STD_ON )
  /* #10 Handle transmission over TCP. */
  if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
  {
    /* #100 Check if TxQueue can handle new request. */
    if ( SoAd_TcpTxQueueReserveNextElement(soConIdx, PduRouteIdx) == E_OK )
    {
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
      /* #1000 Set element considering PDU header length if PDU header is eanbled. */
      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
      {
        SoAd_TcpTxQueueSetElement(soConIdx, PduRouteIdx, (PduLength + SOAD_PDU_HDR_SIZE),
          FALSE);
      }
      else
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
      /* #1001 Set element considering PDU length only if PDU header is disabled. */
      {
        SoAd_TcpTxQueueSetElement(soConIdx, PduRouteIdx, PduLength, FALSE);
      }

      retVal = E_OK;
    }
  }
# endif /* SOAD_VTCP_USED == STD_ON */
# if ( (SOAD_VTCP_USED == STD_ON) && (SOAD_VTX_BUF_ENABLED == STD_ON) )
  else
# endif /* (SOAD_VTCP_USED == STD_ON) && (SOAD_VTX_BUF_ENABLED == STD_ON) */
# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
  /* #20 Handle transmission over UDP. */
  {
    /* #200 Check if TP Tx buffer size is sufficient. */
    if ( PduLength <= SoAd_SockCons(soConIdx).SockConGrpPtr->SockTpTxBufMin )
    {
#  if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
      /* #2000 Update Alive Supervision Timeout. */
      SoAd_SoCon_UpdateUdpAliveTimeout(soConIdx);
#  endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

      retVal = E_OK;
    }
  }
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TxTp_PrepareTransmitPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxTp_PrepareTransmitPdu(
  SoAd_PduIdxType PduRouteIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx = SoAd_TxPduIdx2PduRoute(PduRouteIdx).PduRouteDestListPtr[0]->SoConIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set transmission management structure. */
  SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx       = PduRouteIdx;
  SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen     = PduLength;
  SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending = PduLength;

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
  {
    /* #100 Consider length of PDU header. */
    SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen     += SOAD_PDU_HDR_SIZE;
    SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending += SOAD_PDU_HDR_SIZE;
  }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

  /* #20 Lock socket connection for this transmission. */
  SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(soConIdx).SockIdx) = soConIdx;
}
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_GetLocalSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Util_GetLocalSockAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_DATA) LocalSockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType  dummyDefaultRouter = { 0 };
  uint8                   dummyNetmask = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward request to TcpIp. */
  SoAd_TcpIp_GetIpAddr(SoConIdx, (SOAD_P2VAR(SoAd_SockAddrType))LocalSockAddrPtr, &dummyNetmask,
    (SOAD_P2VAR(SoAd_SockAddrType))&dummyDefaultRouter);
}
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_GetRemoteSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Util_GetRemoteSockAddr(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_DATA) RemSockAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy current remote address. */
  SoAd_CopyIpAddr(
      (SOAD_P2VAR(SoAd_SockAddrType))RemSockAddrPtr,
      (SOAD_P2VAR(SoAd_SockAddrType))&SoAd_SockConIdx2RemSockDyn(SoConIdx),
      TRUE);
}
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  SoAd_IpAddrConstPtrType IpAddrAPtr,
  SoAd_IpAddrConstPtrType IpAddrBPtr,
  boolean AllowWildcards)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VIPV6_ENABLED == STD_ON )
  /* #10 Compare IPv6 IP address. */
  if ( Domain == SOAD_AF_INET6 )
  {
    /* #100 Compare both IP addresses. */
    if ( (IpAddrAPtr[0] == IpAddrBPtr[0]) &&
      (IpAddrAPtr[1] == IpAddrBPtr[1]) &&
      (IpAddrAPtr[2] == IpAddrBPtr[2]) &&
      (IpAddrAPtr[3] == IpAddrBPtr[3]) )
    {
      retVal = E_OK;
    }
    /* #101 Compare first IP address to wildcard if IP addresses are not equal. */
    else if ( (AllowWildcards == TRUE) &&
      ((IpAddrAPtr[0] == SOAD_IP6ADDR_ANY) &&
      (IpAddrAPtr[1] == SOAD_IP6ADDR_ANY) &&
      (IpAddrAPtr[2] == SOAD_IP6ADDR_ANY) &&
      (IpAddrAPtr[3] == SOAD_IP6ADDR_ANY)) )
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
# else
  SOAD_DUMMY_STATEMENT_VAR(Domain); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  /* #20 Compare IPv4 IP address. */
  {
    /* #200 Compare both IP addresses. */
    if ( IpAddrAPtr[0] == IpAddrBPtr[0] )
    {
      retVal = E_OK;
    }
    /* #201 Compare first IP address to wildcard if IP addresses are not equal. */
    else if ( (AllowWildcards == TRUE) && (IpAddrAPtr[0] == SOAD_IPADDR_ANY) )
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_CompareSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareSockAddr(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrAPtr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrBPtr,
  boolean AllowWildcards)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compare domain. */
  if ( SockAddrAPtr->domain == SockAddrBPtr->domain )
  {
    /* #20 Compare IP addresses. */
    if ( E_OK == SoAd_Util_CompareIpAddr(SockAddrAPtr->domain,
      ((SOAD_P2CONST(SoAd_SockAddrInetXType))SockAddrAPtr)->addr,
      ((SOAD_P2CONST(SoAd_SockAddrInetXType))SockAddrBPtr)->addr,
      AllowWildcards) )
    {
      /* #30 Compare both ports. */
      if ( ((SOAD_P2CONST(SoAd_SockAddrInetXType))SockAddrAPtr)->port !=
        ((SOAD_P2CONST(SoAd_SockAddrInetXType))SockAddrBPtr)->port )
      {
        /* #31 Compare first port to wildcard if ports are not equal. */
        if ( (AllowWildcards == TRUE) &&
          (((SOAD_P2CONST(SoAd_SockAddrInetXType))SockAddrAPtr)->port == SOAD_PORT_ANY) )
        {
          retVal = E_OK;
        }
      }
      else
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_InitSockIdxStructs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_InitSockIdxStructs(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType_least sockIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all module internal socket indexes. */
  for ( sockIdx = 0u; sockIdx < SoAd_TcpIpSocketCnt(); sockIdx++ )
  {
    SoAd_SoConIdxType soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

    /* #20 Initialize general struct elements. */
    SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_CLOSED;
    SoAd_SockIdx2SocketId(sockIdx) = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
    SoAd_SockIdx2LocPortDyn(sockIdx) = SoAd_SockCons(soConIdx).SockConGrpPtr->LocalPort;

#if ( SOAD_VPDU_ROUTES == STD_ON )
    /* #30 Initialize tx struct elements. */
    if ( 0 != SoAd_PduRouteTotal() )
    {
      SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;
    }
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
    /* #40 Initialize rx struct elements. */
    if ( SoAd_IsSockRoutePresent() )
    {
      SoAd_SockIdx2SkipBytes(sockIdx) = 0u;
    }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  }
}

#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpTxQueueReserveNextElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpTxQueueReserveNextElement(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx)
{
  SOAD_P2CONST(SoAd_SocketConnectionType) soConPtr = &SoAd_SockCons(SoConIdx);
  Std_ReturnType retVal = E_NOT_OK;
  uint16 idx, lvl, size, writeIdx;

  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  idx  = soConPtr->TcpTxQueuePtr->MgtPtr->Idx;
  lvl  = soConPtr->TcpTxQueuePtr->MgtPtr->Lvl;
  size = soConPtr->TcpTxQueuePtr->Size;

  /* #10 Check if TCP Tx queue has elements left. */
  if ( lvl < size )
  {
    /* #20 Try to get free element index. */
    /* #200 Check if queue is filled. */
    if ( 0u < lvl )
    {
      /* #2000 Check if last used index is reserved. */
      uint16 lastLvl = (uint16)(lvl - 1u);

      /* find last used entry in Tx Queue */
      if ( (idx + lastLvl) >= size )
      { /* wrap around necessary */
        writeIdx = lastLvl - (size - idx);
      }
      else
      { /* no wrap around necessary */
        writeIdx = idx + lastLvl;
      }

      if ( 0u != soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].Len )
      { /* last entry is completely written - new entry is possible */
        /* #20000 Use next element index. */
        if ( (writeIdx + 1) == size )
        { /* wrap around necessary */
          writeIdx = 0u;
        }
        else
        { /* no wrap around necessary */
          writeIdx++;
        }
        retVal = E_OK;
      }
    }
    /* #201 Check if queue is empty. */
    else
    {
      /* #2010 Use current element index. */
      writeIdx = idx;
      retVal = E_OK;
    }

    /* #21 Check if free element index is found. */
    if ( E_OK == retVal )
    {
      /* #210 Reserve element. */
      soConPtr->TcpTxQueuePtr->MgtPtr->Lvl++;
      soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].TxPduIdx = TxPduIdx;
      soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].Len = 0u;
    }
  }
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

  return retVal;
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TcpTxQueueSetElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TcpTxQueueSetElement(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx,
  PduLengthType Len,
  boolean Release)
{
  SOAD_P2CONST(SoAd_SocketConnectionType) soConPtr = &SoAd_SockCons(SoConIdx);
  uint16 idx, lvl, size, writeIdx;

  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  idx  = soConPtr->TcpTxQueuePtr->MgtPtr->Idx;
  lvl  = soConPtr->TcpTxQueuePtr->MgtPtr->Lvl;
  size = soConPtr->TcpTxQueuePtr->Size;

  /* #10 Check if queue is filled by at least one reserved element. */
  if ( 0u < lvl )
  {
    /* #20 Find last entry in TCP Tx queue. */
    lvl--;
    if ( (idx + lvl) >= size )
    { /* wrap around necessary */
      writeIdx = lvl - (size - idx);
    }
    else
    { /* no wrap around necessary */
      writeIdx = idx + lvl;
    }

    /* #21 Check if element is reserved. */
    if ( TxPduIdx == soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].TxPduIdx )
    {
      /* #210 Release element if required. */
      if ( TRUE == Release )
      {
        /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
        soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].TxPduIdx = SOAD_INV_PDU_IDX;
        soConPtr->TcpTxQueuePtr->MgtPtr->Lvl--;
      }
      /* #211 Set element if required. */
      else
      {
        soConPtr->TcpTxQueuePtr->QueuePtr[writeIdx].Len = Len;
      }
    }
  }
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
}
# endif /* SOAD_VTCP_USED == STD_ON */

# if ( (SOAD_VTX_BUF_ENABLED == STD_ON) || (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_ReconnectSockCon
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_ReconnectSockCon(
  SoAd_SoConIdxType SoConIdx)
{
  /* #10 Check if automatic setup is enabled. */
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
  {
    /* #100 Check if remote address contains wildcards. */
    if ( 0u != (SoAd_SockCons(SoConIdx).RemAddrState & SOAD_SOCON_MASK_ANY) )
    {
      /* #1000 Reset remote address and set socket connection to reconnect. */
      SoAd_ResetIpAddr(SoConIdx);
#  if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
      if ( NULL_PTR != SoAd_SockCons(SoConIdx).RcvRemSockPtr )
      { /* get received remote address is supported */
        /* reset remote address */
        SoAd_ResetIpAddr2Unspec(SoAd_SockCons(SoConIdx).RcvRemSockPtr);
      }
#  endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */
      SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_RECONNECT;
      /* set event to handle socket connection state in main function */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
      /* inform upper layer about socket connection state change */
      SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
        SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));
    }
  }
}
# endif /* (SOAD_VTX_BUF_ENABLED == STD_ON) || (SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON) */

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
#  if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_UpdatePduHdrLen
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_UpdatePduHdrLen(
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr)
{
  BufPtr[4] = (uint8)(PduHeaderLen >> 24);
  BufPtr[5] = (uint8)(PduHeaderLen >> 16);
  BufPtr[6] = (uint8)(PduHeaderLen >> 8);
  BufPtr[7] = (uint8)(PduHeaderLen);
}
#  endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_CloseSockCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_CloseSockCon(
  SoAd_SoConIdxType SoConIdx)
{
  boolean abortFlag = SoAd_SockConIdx2FlagAbort(SoConIdx);
  SoAd_SoConModeType oldState = SoAd_SockConIdx2State(SoConIdx);

  /* set socket connection state */
  switch ( SoAd_SockConIdx2CloseMode(SoConIdx) )
  {
    case SOAD_CLOSE_OFFLINE_RESET:
    case SOAD_CLOSE_OFFLINE:
    { /* set socket connection to offline */
      SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_OFFLINE;
      break;
    }
    case SOAD_CLOSE_RECONNECT:
    { /* set socket connection to reconnect */
      SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_RECONNECT;
      break;
    }
    default:
    {
      return;
    }
  }

  if ( SoAd_SockConIdx2State(SoConIdx) != oldState )
  {
    if ( SOAD_CLOSE_OFFLINE == SoAd_SockConIdx2CloseMode(SoConIdx) )
    { /* closed by normal close request */
      /* reset socket connection, release buffers, terminate active TP sessions and reset TxQueue */
      SoAd_ResetSockCon(SoConIdx, TRUE, TRUE);
    }
    else
    { /* closed by different reason (not normal close request) */
      if ( SOAD_CLOSE_OFFLINE_RESET == SoAd_SockConIdx2CloseMode(SoConIdx) )
      {
        /* reset open/close counter */
        SoAd_SockConIdx2FlagCntOpCl(SoConIdx) = 0u;
      }
      SoAd_ResetSockCon(SoConIdx, TRUE, FALSE);
    }

    /* inform upper layer about socket connection state change */
    SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
      SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));

    /* close TcpIp socket */
#if ( SOAD_VTCP_USED == STD_ON )
    if ( SOAD_IPPROTO_TCP == SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol )
    { /* TCP socket */
      /* get socket index */
      SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

      if ( (SOAD_SOCK_STATE_CLOSED != SoAd_SockIdx2SockState(sockIdx)) &&
        (SOAD_SOCK_STATE_CLOSING != SoAd_SockIdx2SockState(sockIdx)) )
      { /* socket is not already closed or closing */
        SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_CLOSING;
        /* call TcpIp close function */
        SoAd_TcpIp_Close(SoAd_SockIdx2SocketId(sockIdx), abortFlag, sockIdx);
      }
      else
      { /* socket is already closed or closing - reset close mode */
        SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_NONE;
      }

      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate == FALSE )
      { /* server socket */
        SoAd_SockIdxType sockIdxListen = SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx;

        if ( (SOAD_SOCK_STATE_CLOSED != SoAd_SockIdx2SockState(sockIdxListen)) &&
          (SOAD_SOCK_STATE_CLOSING != SoAd_SockIdx2SockState(sockIdxListen)) )
        { /* listen socket is not already closed or closing */
          /* check if all socket connections are closed */
          SoAd_SoConIdxType soConIdxTmp = SoAd_SockIdx2SoConIdxCol(sockIdxListen).SoConIdxStart;
          SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdxType)(soConIdxTmp +
            SoAd_SockIdx2SoConIdxCol(sockIdxListen).SoConIdxNum);

          while ( soConIdxTmp < soConIdxMax )
          {
            if ( SOAD_SOCON_OFFLINE != SoAd_SockConIdx2State(soConIdxTmp) )
            { /* at least one of the socket connections is not offline - do not close corresponding TcpIp socket */
              return;
            }
            soConIdxTmp++;
          }
          /* all socket connections are offline */
          if ( SoAd_SockIdx2SockState(sockIdxListen) != SOAD_SOCK_STATE_CLOSED )
          {
            /* close listen socket */
            SoAd_SockIdx2SockState(sockIdxListen) = SOAD_SOCK_STATE_CLOSING;
            /* call TcpIp close function */
            SoAd_TcpIp_Close(SoAd_SockIdx2SocketId(sockIdxListen), abortFlag, sockIdx);
          }
        }
      }
    }
    else
#endif /* SOAD_VTCP_USED == STD_ON */
    { /* UDP socket */
      SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

      if ( (SOAD_SOCK_STATE_CLOSED != SoAd_SockIdx2SockState(sockIdx)) &&
        (SOAD_SOCK_STATE_CLOSING != SoAd_SockIdx2SockState(sockIdx)) )
      { /* socket is not already closed or closing */
        /* check if all socket connections are closed */
        SoAd_SoConIdxType soConIdxTmp = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
        SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdxType)(soConIdxTmp +
          SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum);

        while ( soConIdxTmp < soConIdxMax )
        {
          if ( SOAD_SOCON_OFFLINE != SoAd_SockConIdx2State(soConIdxTmp) )
          { /* at least one of the socket connections is not offline - do not close corresponding TcpIp socket */
            SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_NONE;
            return;
          }
          soConIdxTmp++;
        }

        /* abort flag is set or all socket connections are offline */
        SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_CLOSING;
        /* call TcpIp close function */
        SoAd_TcpIp_Close(SoAd_SockIdx2SocketId(sockIdx), abortFlag, sockIdx);
      }
      else
      { /* socket is already closed or closing - reset close mode */
        SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_NONE;
      }
    }
  }

  /* set event to handle socket connection state in main function */
  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_OpenSockCon()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_OpenSockCon(
  SoAd_SoConIdxType SoConIdx)
{
  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP )
  { /* UDP socket */
    /* check if TcpIp socket is already assigned to socket connection group */
    SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

    if ( SOAD_SOCK_STATE_CLOSED == SoAd_SockIdx2SockState(sockIdx) )
    { /* no TcpIp socket is assigned */
      SoAd_AssignUdpSocket(SoConIdx);
    }

    /* check if TcpIp socket is now assigned completely */
    if ( SOAD_SOCK_STATE_ESTABLISHED == SoAd_SockIdx2SockState(sockIdx) )
    { /* TcpIp socket is assigned */
      /* try to open socket connection */
      /* check remote address */
      if ( (SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->UdpListenOnly == TRUE) ||
        (SOAD_SOCON_IP_SET_PORT_SET == SoAd_SockConIdx2RemAddrState(SoConIdx)) )
      { /* remote address is set or UDP is listen only */
        /* open socket connection */
        SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_ONLINE;

#if ( SOAD_VPDU_ROUTES == STD_ON )
        /* trigger address resolution */
        if ( SOAD_SOCON_IP_SET_PORT_SET == SoAd_SockConIdx2RemAddrState(SoConIdx) )
        { /* remote address set */
          if ( SoAd_IsPduRoutePresent() )
          {
            SoAd_TcpIp_TriggerAddressResolution(SoConIdx);
          }
        }
#endif /* SOAD_VPDU_ROUTES == STD_ON */

        /* inform upper layer about socket connection state change */
        SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
          SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
        if ( SoAd_IsSockRoutePresent() )
        {/* check whether StartOfReception need to be called for upper layers */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
          if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == FALSE )
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
          {
            if ( SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxNum > 0u )
            {
              uint16 routeIdx = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxStart;

              if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_TP )
              {
                PduLengthType dummyLen = 0u;
                /* call StartOfReception */
                if ( BUFREQ_OK != SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                    0u,
                    &dummyLen) )
                { /* close socket connection and reset open/close counter */
                  SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                }
                else
                {
                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = routeIdx;
                }
              }
            }
          }
        }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
      }
      else
      { /* no remote address is set */
        if ( SOAD_SOCON_RECONNECT != SoAd_SockConIdx2State(SoConIdx) )
        {
          /* set socket connection to reconnect */
          SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_RECONNECT;

          /* inform upper layer about socket connection state change */
          SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
            SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));
        }
      }
    }
    else
    { /* TcpIp socket not assigned */
      /* set event to handle socket connection state in main function again */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
    }
  }
  else
  { /* TCP socket */
#if ( SOAD_VTCP_USED == STD_ON )
    SoAd_SockIdxType sockIdx;

    if ( TRUE == SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate )
    { /* client socket */
      sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

      /* check if socket is already assigned */
      if ( SOAD_SOCK_STATE_CLOSED == SoAd_SockIdx2SockState(sockIdx) )
      { /* no socket is assigned */
        /* check remote address */
        if ( SOAD_SOCON_IP_SET_PORT_SET == SoAd_SockConIdx2RemAddrState(SoConIdx) )
        { /* remote address is set */
          SoAd_SocketIdType socketId = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */

          /* get TcpIp socket */
          if ( E_OK == SoAd_TcpIp_GetSocket(SoConIdx, SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
              SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol, &socketId) )
          { /* TcpIp socket assigned */
            /* change parameter */
            if ( E_OK == SoAd_SoCon_TcpChangeParameter(SoConIdx, socketId) )
            { /* change parameter successful */
              uint16 locPort = (uint16)(IPBASE_HTON16(SoAd_SockIdx2LocPortDyn(sockIdx)));

              if ( E_OK == SoAd_TcpIp_Bind(SoConIdx, socketId, &locPort) )
              {
                /* update local port */
                SoAd_SockIdx2LocPortDyn(sockIdx) = (uint16)(IPBASE_HTON16(locPort));
                /* connect to server socket */
                if ( E_OK == SoAd_TcpIp_TcpConnect(SoConIdx, socketId,
                    (SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx)) )
                { /* connect request accepted */
                  /* assign socketId to local management struct */
                  SoAd_SockIdx2SocketId(sockIdx) = socketId;
                  /* set socket state */
                  SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_CONNECT;
                  /* increment opened sockets for shutdown mechanism */
                  SoAd_OpenSocks++;
                }
              }
            }
            if ( SOAD_SOCK_STATE_CONNECT != SoAd_SockIdx2SockState(sockIdx) )
            { /* change parameter, bind or connect failed */
              /* set event to handle socket connection state in main function again */
              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
              /* close TcpIp socket */
              SoAd_TcpIp_Close(socketId, FALSE, sockIdx);
            }
            else
            { /* socket connection establishment running */
              if ( SOAD_SOCON_RECONNECT != SoAd_SockConIdx2State(SoConIdx) )
              {
                SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_RECONNECT;

                /* inform upper layer about socket connection state change */
                SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
                  SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));
              }
            }
          }
          else
          { /* no TcpIp socket assigned */
            /* set event to handle socket connection state in main function again */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
          }
        }
      }
    }
    else
    { /* server socket */
      sockIdx = SoAd_SockCons(SoConIdx).SockConGrpPtr->GrpSockIdx;

      if ( SOAD_SOCK_STATE_CLOSED == SoAd_SockIdx2SockState(sockIdx) )
      { /* no listen socket is assigned */
        SoAd_SocketIdType socketId = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */

        /* get TcpIp socket */
        if ( E_OK == SoAd_TcpIp_GetSocket(SoConIdx, SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
            SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol,
            &socketId) )
        { /* TcpIp socket assigned */
          /* change parameter */
          if ( E_OK == SoAd_SoCon_TcpChangeParameter(SoConIdx, socketId) )
          { /* change parameter successful */
            uint16 locPort = (uint16)(IPBASE_HTON16(SoAd_SockIdx2LocPortDyn(sockIdx)));

            if ( E_OK == SoAd_TcpIp_Bind(SoConIdx, socketId, &locPort) )
            {
              /* update local port */
              SoAd_SockIdx2LocPortDyn(sockIdx) = (uint16)(IPBASE_HTON16(locPort));
              /* set socket to listen */
              if ( E_OK == SoAd_TcpIp_TcpListen(SoConIdx, socketId,
                (uint16)SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum) )
              { /* listen request accepted */
                /* assign socketId to local management struct */
                SoAd_SockIdx2SocketId(sockIdx) = socketId;
                /* set socket state */
                SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_LISTEN;
                /* increment opened sockets for shutdown mechanism */
                SoAd_OpenSocks++;
              }
            }
          }
          if ( SOAD_SOCK_STATE_LISTEN != SoAd_SockIdx2SockState(sockIdx) )
          { /* change parameter, bind or listen failed */
            /* set event to handle socket connection state in main function again */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
            /* close TcpIp socket */
            SoAd_TcpIp_Close(socketId, FALSE, sockIdx);
          }
        }
        else
        {
          /* set event to handle socket connection state in main function again */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
        }
      }
      if ( SOAD_SOCK_STATE_LISTEN == SoAd_SockIdx2SockState(sockIdx) )
      { /* listen socket is (now) assigned */
        if ( SOAD_SOCON_OFFLINE == SoAd_SockConIdx2State(SoConIdx) )
        {
          SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_RECONNECT;

          /* inform upper layer about socket connection state change */
          SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
            SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(SoConIdx));
        }
      }
    }
#endif /* SOAD_VTCP_USED == STD_ON */
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_AssignUdpSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_AssignUdpSocket(SoAd_SoConIdxType SoConIdx)
{
  SoAd_SocketIdType socketId = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;

  if ( SoAd_TcpIp_GetSocket(SoConIdx, SoAd_SockCons(SoConIdx).SockConGrpPtr->Domain,
    SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol,
    &socketId) == E_OK )
  { /* TcpIp socket assigned */
    Std_ReturnType retVal;

    retVal = SoAd_SoCon_UdpChangeParameter(SoConIdx, socketId);

    if ( retVal == E_OK )
    {
      uint16 locPort = (uint16)(IPBASE_HTON16(SoAd_SockIdx2LocPortDyn(sockIdx)));

      retVal |= SoAd_TcpIp_Bind(SoConIdx, socketId, &locPort);

      if ( retVal == E_OK )
      {
        /* update local port */
        SoAd_SockIdx2LocPortDyn(sockIdx) = (uint16)(IPBASE_HTON16(locPort));
        /* assign TcpIp socket to socket connection group */
        SoAd_SockIdx2SocketId(sockIdx) = socketId;
        /* set socket state */
        SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_ESTABLISHED;
        /* increment opened sockets for shutdown mechanism */
        SoAd_OpenSocks++;
      }
    }
    if ( retVal != E_OK )
    { /* TcpIp socket could not be assigned */
      /* check if socket is assigned but further steps failed */
      if ( socketId != SOAD_INV_SOCKET_ID ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
      { /* socket is assigned */
        /* close TcpIp socket */
        SoAd_TcpIp_Close(socketId, FALSE, sockIdx);
      }
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_ResetSockCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 4 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetSockCon(
  SoAd_SoConIdxType SoConIdx,
  boolean ResetRemAddr,
  boolean NormalClose)
{
  if ( TRUE == ResetRemAddr )
  { /* reset remote address */
    SoAd_ResetIpAddr(SoConIdx);
#if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
    if ( NULL_PTR != SoAd_SockCons(SoConIdx).RcvRemSockPtr )
    { /* get received remote address is supported */
      /* reset remote address */
      SoAd_ResetIpAddr2Unspec(SoAd_SockCons(SoConIdx).RcvRemSockPtr);
    }
#endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */
  }

  SoAd_SockConIdx2FlagAbort(SoConIdx) = FALSE;

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
  if ( SoAd_IsSockRoutePresent() )
  { /* release buffer */
    if ( SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr != NULL_PTR )
    {
      *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr = 0u;
    }
    if ( SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr != NULL_PTR )
    {
      *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr = 0u;
    }
  }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

  /* terminate active TP session */
  /* TP transmit */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  if ( SoAd_IsPduRoutePresent() )
  {
    if ( SOAD_INV_PDU_IDX != SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
    {
      if ( SoAd_TxPduIdx2PduRoute(SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx).UpperLayer == SOAD_UL_TYPE_TP )
      { /* TP transmission active */
        /* call TxConfirmation */
        if ( SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending == 0u )
        { /* no data pending */
          SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx).UpperLayerId,
            SoAd_TxPduIdx2TxConfPduId(SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx),
            E_OK);
        }
        else
        { /* data pending */
          /* call TxConfirmation */
          SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx).UpperLayerId,
            SoAd_TxPduIdx2TxConfPduId(SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx),
            E_NOT_OK);
        }

        SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen      = 0u;
        SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending  = 0u;
        SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx        = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        SoAd_SockConIdx2TxMgt(SoConIdx).CancelRequested = FALSE;
      }
    }
  }
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  /* TP reception */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
  /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
  if ( SoAd_IsSockRoutePresent() )
  {
    if ( SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx )
    { /* reception may be ongoing */
      if ( TRUE == NormalClose )
      { /* reset caused by call to SoAd_CloseSoCon */
        /* check if all received data were forwarded to upper layer */
        if ( (SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP) &&
          (0u != SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending) )
        { /* received data pending on UDP socket */
          SoAd_VCallTpRxIndication(SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.UpperLayerId,
            SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.RxPduId,
            E_NOT_OK);
        }
# if ( SOAD_VTCP_USED == STD_ON )
        else if ( (SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP) &&
          (0u != SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt.Lvl) )
        { /* received data pending on TCP socket */
          SoAd_VCallTpRxIndication(SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.UpperLayerId,
            SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.RxPduId,
            E_NOT_OK);
        }
# endif /* SOAD_VTCP_USED == STD_ON */
        else
        { /* no received data pending on TCP socket */
          SoAd_VCallTpRxIndication(SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.UpperLayerId,
            SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.RxPduId,
            E_OK);
        }
      }
      else
      { /* reset caused by different reason (not by normal close request) */
        SoAd_VCallTpRxIndication(SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.UpperLayerId,
          SoAd_SockRoutes(SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx).SockRouteDest.RxPduId,
          E_NOT_OK);
      }
    }
    if ( SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr != NULL_PTR )
    {
      *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr = 0u;
    }
    SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt.Lvl   = 0u;
    SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending  = 0u;
    SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;
    SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx        = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(NormalClose); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VTCP_USED == STD_ON )
  /* empty tx queue if configured */
  if ( NULL_PTR != SoAd_SockCons(SoConIdx).TcpTxQueuePtr )
  {
    uint16 idx = SoAd_SockCons(SoConIdx).TcpTxQueuePtr->Size;
    while ( 0u < idx )
    {
      idx--;
      /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
      SoAd_SockCons(SoConIdx).TcpTxQueuePtr->QueuePtr[idx].TxPduIdx = SOAD_INV_PDU_IDX;
      SoAd_SockCons(SoConIdx).TcpTxQueuePtr->QueuePtr[idx].Len = 0u;
    }
    SoAd_SockCons(SoConIdx).TcpTxQueuePtr->MgtPtr->Idx = 0u;
    SoAd_SockCons(SoConIdx).TcpTxQueuePtr->MgtPtr->Lvl = 0u;
  }
#endif /* SOAD_VTCP_USED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
  if ( SoAd_IsNPduUdpTxPresent() )
  {
    uint16 idx = SoAd_SockConIdx2NPduUdpTxIdx(SoConIdx);

    if ( idx != SOAD_INV_BUF_IDX_16 )
    {
      SoAd_NPduClear(idx);
    }
  }
#endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
  if ( SoAd_IsTimeoutListIdxUdpAlivePresent() )
  {
    SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxUdpAlive(), SoConIdx);
  }
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpPduHdrRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpPduHdrRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length)
{
  /* add newly received data to segmented buffer struct */
  if ( E_OK != SoAd_WriteBufSegment(BufPtr, Length, &SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt) )
  { /* segmented buffer struct cannot handle additional segments */
    SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
    /* set event to handle socket connection state in main function */
    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_PDU_HDR_RX_INDICATION, SOAD_E_NOBUFS);
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
  }
  else
  {
    SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
    SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);

    uint32 totalLen = SoAd_GetBufSegmentTotLen(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, 0);
    uint32 releaseBufLen = 0u;

    while ( releaseBufLen < totalLen )
    {
      SoAd_PduHdrIdType  pduHdrId = 0u;
      SoAd_PduHdrLenType pduHdrLen;

      if ( SOAD_INV_PDU_ID == SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
      { /* PDU header not extracted before */
        if ( SOAD_PDU_HDR_SIZE > (totalLen - releaseBufLen) )
        { /* PDU header not received completely */
          /* wait for next TCP segment */
          break;
        }
        else
        { /* PDU header received completely */
          boolean pduIsValid = FALSE;
          uint8   pduHdrBuf[SOAD_PDU_HDR_SIZE] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

          /* extract PDU header */
          SoAd_CopyBufSeg2Buf(&pduHdrBuf[0], &SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt,
            SOAD_PDU_HDR_SIZE, releaseBufLen);
          IPBASE_GET_UINT32(pduHdrBuf, 0u, pduHdrId);
          IPBASE_GET_UINT32(pduHdrBuf, SOAD_PDU_HDR_ID_SIZE, pduHdrLen);

          if ( (SOAD_PDU_MAX_LEN < pduHdrLen) || (0u == pduHdrLen) ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          { /* discard PDU */
            /* PRQA S 3355 1 */ /* MD_SoAd_13.7_CFG */ /* PRQA S 3358 1 */ /* MD_SoAd_13.7_CFG */
            if ( (uint32)(totalLen - releaseBufLen) < (SOAD_PDU_HDR_SIZE + pduHdrLen) )
            { /* PDU is not received completely */
              /* PRQA S 3382 1 */ /* MD_SoAd_21.1 */
              SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)((SOAD_PDU_HDR_SIZE + pduHdrLen) -
                (uint32)(totalLen - releaseBufLen));
              releaseBufLen = totalLen;
            }
            else
            { /* PDU is received completely */ /* PRQA S 3201 */ /* MD_SoAd_13.7_CFG */
              releaseBufLen += (uint32)(SOAD_PDU_HDR_SIZE + pduHdrLen);
            }
          }
          else
          { /* PDU length valid */
            /* get all affected socket routes */
            uint16 routeIdx = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxStart;
            uint16 routeNum = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxNum;

            /* find socket route */
            while ( 0 < routeNum )
            {
              if ( SoAd_SockRoutes(routeIdx).RxPduHdrId == pduHdrId )
              {
#   if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
                if ( FALSE == SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
                { /* no routing group is enabled for this socket route destination */
                  /* discard PDU */
                  if ( (uint32)(totalLen - releaseBufLen) < (SOAD_PDU_HDR_SIZE + pduHdrLen) )
                  { /* PDU is not received completely */
                    /* PRQA S 3382 1 */ /* MD_SoAd_21.1 */
                    SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)((SOAD_PDU_HDR_SIZE + pduHdrLen) -
                      (uint32)(totalLen - releaseBufLen));
                    releaseBufLen = totalLen;
                  }
                  else
                  { /* PDU is received completely */
                    releaseBufLen += (uint32)(SOAD_PDU_HDR_SIZE + pduHdrLen);
                  }
                  /* valid PDU found */
                  pduIsValid = TRUE;
                  break;
                }
                else
#   endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
                {
                  if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_IF )
                  { /* IF-API */
                    /* check length (configured length does not contain PDU Header size) */
                    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockIfRxBufMin < pduHdrLen )
                    { /* rx buffer is not sufficient */
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
                      (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_PDU_HDR_RX_INDICATION,
                        SOAD_E_NOBUFS);
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
                      /* discard PDU */
                      if ( (uint32)(totalLen - releaseBufLen) < (SOAD_PDU_HDR_SIZE + pduHdrLen) )
                      { /* PDU is not received completely */
                        /* PRQA S 3382 1 */ /* MD_SoAd_21.1 */
                        SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)((SOAD_PDU_HDR_SIZE + pduHdrLen) -
                          (uint32)(totalLen - releaseBufLen));
                        releaseBufLen = totalLen;
                      }
                      else
                      { /* PDU is received completely */
                        releaseBufLen += (uint32)(SOAD_PDU_HDR_SIZE + pduHdrLen);
                      }
                    }
                    else
                    {
                      SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = routeIdx;
                      /* copy header to IF buffer */
                      IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                        &SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferPtr[0],
                        (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))&pduHdrBuf[0],
                        SOAD_PDU_HDR_SIZE);
                      *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr = SOAD_PDU_HDR_SIZE;
                      releaseBufLen += SOAD_PDU_HDR_SIZE;
                    }
                    /* valid PDU found */
                    pduIsValid = TRUE;
                    break;
                  }
                  else
                  { /* TP-API */
                    SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = routeIdx;
                    /* copy header to TP buffer */
                    IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                      &SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr[0],
                      (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))&pduHdrBuf[0],
                      SOAD_PDU_HDR_SIZE);
                    releaseBufLen += SOAD_PDU_HDR_SIZE;
                    /* valid PDU found */
                    pduIsValid = TRUE;
                    break;
                  }
                }
              }
              routeNum--;
              routeIdx++;
            }
            if ( TRUE != pduIsValid )
            { /* no socket route found */
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
              (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_PDU_HDR_RX_INDICATION,
                SOAD_E_INV_PDUHEADER_ID);
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
              /* discard PDU */
              if ( (uint32)(totalLen - releaseBufLen) < (SOAD_PDU_HDR_SIZE + pduHdrLen) )
              { /* PDU is not received completely */
                /* PRQA S 3382 1 */ /* MD_SoAd_21.1 */
                SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)((SOAD_PDU_HDR_SIZE + pduHdrLen) -
                  (uint32)(totalLen - releaseBufLen));
                releaseBufLen = totalLen;
              }
              else
              { /* PDU is received completely */
                releaseBufLen += (uint32)(SOAD_PDU_HDR_SIZE + pduHdrLen);
              }
            }
          }
        }
      }

      if ( SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
      { /* PDU header is (now) extracted */
        PduInfoType pduInfo;

        uint16 routeIdx = SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx;

        if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_IF )
        { /* IF-API */
          uint32 bufSegLen = 0u;

          /* get PDU length */
          IPBASE_GET_UINT32(SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferPtr, SOAD_PDU_HDR_ID_SIZE, pduHdrLen);

          /* check whether PDU is received within one TCP segment */
          SoAd_GetBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, releaseBufLen, &pduInfo.SduDataPtr,
            &bufSegLen);
          if ( (*SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr == SOAD_PDU_HDR_SIZE) &&
            (pduHdrLen <= bufSegLen) )
          { /* PDU is within one TCP segment */
            releaseBufLen += pduHdrLen;
            *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr += (PduLengthType)pduHdrLen;
          }
          else
          { /* PDU is segmented */
            uint32 bytesToCopy = (pduHdrLen + SOAD_PDU_HDR_SIZE) -
              *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr;

            if ( bytesToCopy > (totalLen - releaseBufLen) )
            {
              bytesToCopy = totalLen - releaseBufLen;
            }

            /* copy to rx buffers */
            SoAd_CopyBufSeg2Buf(&SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferPtr[
                *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr],
              &SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, bytesToCopy, releaseBufLen);

            *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr += (PduLengthType)bytesToCopy;
            releaseBufLen += bytesToCopy;

            pduInfo.SduDataPtr = &SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferPtr[SOAD_PDU_HDR_SIZE];
          }

          /* forward PDU if received completely */
          if ( *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr == (pduHdrLen + SOAD_PDU_HDR_SIZE) )
          { /* received completely */
            pduInfo.SduLength = (PduLengthType)pduHdrLen;

#   if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
            /* add meta data */
            if ( SoAd_AddMetaDataToPdu(SoConIdx, routeIdx, &pduInfo) == E_OK )
#   endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */
            {
              /* forward PDU */
              SoAd_VCallIfRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, &pduInfo);
            }
            *SoAd_SockConIdx2RxBuffer(SoConIdx).IfRxBufferOffsetPtr = 0u;
            SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          }
        }
        else
        { /* TP-API */
          PduLengthType bufferSize = 0u;

          /* get PDU length */
          IPBASE_GET_UINT32(SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr, SOAD_PDU_HDR_ID_SIZE, pduHdrLen);

          if ( 0u == pduHdrLen )
          { /* PDU header length is 0 - skip further processing */
            SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          }
          else
          { /* valid PDU header length */
#   if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
            SoAd_ReturnType retVal = SOAD_E_OK;

            if ( (SoAd_SockCons(SoConIdx).VerifyRxPduEnabled == TRUE) &&
              (SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending == 0u) )
            {
              /* verify Rx PDU */
              retVal = SoAd_VerifyRxPdu(SoConIdx, releaseBufLen, (totalLen - releaseBufLen), pduHdrId, pduHdrLen);

#    if ( SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u )
              if ( retVal == SOAD_E_PENDING )
              { /* did not receive enough data to verify Rx PDU */
                /* wait for next TCP segment */
                break;
              }
              else
#    endif /* SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u */
              if ( retVal == SOAD_E_NOT_OK )
              { /* verification failed */
                /* discard PDU */
                if ( (uint32)(totalLen - releaseBufLen) < pduHdrLen )
                { /* PDU is not received completely */
                  SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)(pduHdrLen - (totalLen - releaseBufLen));
                  releaseBufLen = totalLen;
                }
                else
                { /* PDU is received completely */
                  releaseBufLen += pduHdrLen;
                }
              }
              else
              { /* verification succeeded */
                /* nothing to do */
              }
            }

            if ( retVal == SOAD_E_OK )
#   endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */
            {
              if ( 0u == SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
              { /* begin reception */
                if ( BUFREQ_OK != SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                  SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, (PduLengthType)pduHdrLen, &bufferSize) )
                { /* start of reception failed */
                  /* discard PDU */
                  if ( (uint32)(totalLen - releaseBufLen) < pduHdrLen )
                  { /* PDU is not received completely */
                    SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)(pduHdrLen - (totalLen - releaseBufLen));
                    releaseBufLen = totalLen;
                  }
                  else
                  { /* PDU is received completely */
                    releaseBufLen += pduHdrLen;
                  }
                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                }
                else
                { /* start of reception successful */
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = (PduLengthType)pduHdrLen;
                }
              }
            }

            if ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
            { /* reception is ongoing */
              uint32 bufSegLen  = 0u;

              pduInfo.SduDataPtr = NULL_PTR;
              pduInfo.SduLength  = 0u;
              bufferSize         = 1u;

              /* call copy rx data until all data are received */
              while ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
              {
                if ( TRUE == SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested )
                { /* reception is canceled */
                  if ( (uint32)(totalLen - releaseBufLen) < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                  { /* PDU is not received completely */
                    /* PRQA S 3382 1 */ /* MD_SoAd_21.1 */
                    SoAd_SockIdx2SkipBytes(sockIdx) = (uint32)(SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending -
                      (uint32)(totalLen - releaseBufLen));
                    releaseBufLen = Length;
                  }
                  else
                  { /* PDU is received completely */
                    releaseBufLen += SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                  }

                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;
                  SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;

                  /* call upper layer RxIndication with negative result value */
                  SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);

                  /* close socket connection */
                  SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                  break;
                }

                if ( NULL_PTR == pduInfo.SduDataPtr )
                { /* first call of copy rx data */
                  pduInfo.SduLength  = 0u;
                }

                /* get next segment */
                SoAd_GetBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, releaseBufLen,
                  &pduInfo.SduDataPtr, &bufSegLen);

                if ( 0u == bufSegLen )
                { /* no received data pending */
                  break;
                }

                if ( pduInfo.SduLength > 0u )
                { /* consecutive call of copy rx data */
                  if ( pduInfo.SduLength > bufSegLen )
                  {
                    pduInfo.SduLength = (PduLengthType)bufSegLen;
                  }
                  if ( pduInfo.SduLength > SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                  {
                    pduInfo.SduLength = SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                  }
                }

                if ( BUFREQ_OK == SoAd_VCallTpCopyRxData(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                  SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, &pduInfo, &bufferSize) )
                { /* copying successful */
                  releaseBufLen += pduInfo.SduLength;
                  bufSegLen     -= pduInfo.SduLength;
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending -= pduInfo.SduLength;

                  if ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                  { /* reception is ongoing */
                    if ( 0u == bufferSize )
                    { /* no buffer is left */
                      /* set event to try again in main function */
                      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), SoConIdx);
                      break;
                    }
                    /* set length to current bufferSize - will be updated in next loop cycle */
                    pduInfo.SduLength = bufferSize;
                  }
                  else
                  { /* reception of PDU finished */
                    /* call upper layer TpRxIndication */
                    SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                      SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_OK);
                    SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                    SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;
                  }
                }
                else
                { /* reception not possible */
                  /* discard PDU */
                  if ( (uint32)(totalLen - releaseBufLen) < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                  { /* PDU is not received completely */
                    /* PRQA S 3382 2 */ /* MD_SoAd_21.1 */
                    SoAd_SockIdx2SkipBytes(sockIdx) =
                      (uint32)(SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending - (uint32)(totalLen - releaseBufLen));
                    releaseBufLen = totalLen;
                  }
                  else
                  { /* PDU is received completely */
                    releaseBufLen += SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                  }
                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;

                  /* call upper layer RxIndication with negative result value */
                  SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);
                }
              }

              if ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
              { /* data remaining which was not accepted by upper layer */
                break;
              }
            }
          }
        }
      }
    }
    if ( 0u != releaseBufLen )
    { /* buffer segment processed */
      if ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt.Lvl )
      { /* socket connection specific buffer struct is used */
        /* release SoAd socket connection specific buffer struct */
        SoAd_ReleaseBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, releaseBufLen);
      }
      /* release TcpIp buffer */
      SoAd_TcpIp_TcpReceived(socketId, releaseBufLen);
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TcpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length)
{
  PduInfoType pduInfo;
  SoAd_SockIdxType sockIdx = SoAd_SockCons(SoConIdx).SockIdx;
  SoAd_SocketIdType socketId = SoAd_SockIdx2SocketId(sockIdx);
  uint32 releaseBufLen = 0u;
  /* get all affected socket routes */
  uint16 routeIdx = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxStart;
  uint16 routeNum = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxNum;

  while ( 0 < routeNum )
  { /* handle reception for all configured socket routes related to this socket connection */
#  if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
    if ( TRUE == SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
    /* at least one routing group for this socket route destination is enabled */
    /* do not discard PDU for this socket route destination */
#  endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
    {
      if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_IF )
      { /* IF-API */
        pduInfo.SduDataPtr = BufPtr;
        pduInfo.SduLength  = (PduLengthType)Length;

#  if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
        /* add meta data */
        if ( SoAd_AddMetaDataToPdu(SoConIdx, routeIdx, &pduInfo) == E_OK )
#  endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */
        {
          /* call upper layer IF RxIndication */
          SoAd_VCallIfRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
            SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, &pduInfo);
        }
        releaseBufLen = Length;
      }
      else
      { /* TP-API */
        /* in this case there is only one TP session possible per socket connection */
        /* the first socket route configured for this socket connection is used */

        /* TP session must be active due socket establishment */
        if ( SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        { /* TP session is active */
          PduLengthType bufferSize;

          /* merge received buffer segment with socket connection specific segmented buffer struct */
          if ( E_OK != SoAd_WriteBufSegment(BufPtr, Length, &SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt) )
          {
            /* segmented buffer struct cannot handle additional segments */
            SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
            /* set event to handle socket connection state in main function */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
#  if ( SOAD_DEV_ERROR_REPORT == STD_ON )
            (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_RX_INDICATION, SOAD_E_NOBUFS);
#  endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
          }
          else
          {
            /* set dummy values */
            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength  = 1u;
            bufferSize         = 1u;

            /* copy data to upper layer until no upper layer buffer or TcpIp buffer is available */
            while ( pduInfo.SduLength != 0u )
            {
              if ( TRUE == SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested )
              { /* reception is canceled for this PDU */
                /* no specific handling specified by AUTOSAR - socket will be closed */
                SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;
                SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                /* set event to handle socket connection state in main function */
                SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                releaseBufLen = Length;
                break;
              }
              else
              {
                if ( NULL_PTR == pduInfo.SduDataPtr )
                { /* first call of CopyRxData */
                  pduInfo.SduLength = 0u;
                }

                /* call CopyRxData */
                if ( BUFREQ_OK != SoAd_VCallTpCopyRxData(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                    &pduInfo,
                    &bufferSize) )
                {
                  SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                  releaseBufLen = Length;
                  break;
                }
                else
                {
                  uint32 bufSegLen = 0u;

                  releaseBufLen += pduInfo.SduLength;

                  if ( bufferSize == 0u )
                  { /* no buffer left */
                    /* set event to try again in main function */
                    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), SoConIdx);
                    break;
                  }

                  /* get next TcpIp buf segment */
                  SoAd_GetBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, releaseBufLen,
                    &pduInfo.SduDataPtr, &bufSegLen);

                  pduInfo.SduLength = (PduLengthType)bufSegLen;

                  if ( pduInfo.SduLength > bufferSize )
                  {
                    pduInfo.SduLength = bufferSize;
                  }
                }
              }
            }
          }
        }
      }
    }
#  if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
    else
    {
      releaseBufLen = Length;
    }
#  endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
    routeNum--;
    routeIdx++;
  }
  if ( 0u != releaseBufLen )
  { /* buffer segment processed */
    if ( 0u < SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt.Lvl )
    { /* socket connection specific buffer struct is used */
      /* release SoAd socket connection specific TcpIp buffer struct */
      SoAd_ReleaseBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, releaseBufLen);
    }
    /* release TcpIp buffer */
    SoAd_TcpIp_TcpReceived(socketId, releaseBufLen);
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

# endif /* SOAD_VTCP_USED == STD_ON */

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_UdpPduHdrRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 3673 SoAd_167 */ /* MD_SoAd_16.7 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_UdpPduHdrRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length)
{
  SoAd_SockIdxType   sockIdx     = SoAd_SockCons(SoConIdx).SockIdx;
  uint16             readDataLen = 0u;
  SoAd_PduHdrIdType  pduHdrId;
  SoAd_PduHdrLenType pduHdrLen;
  PduInfoType        pduInfo;
  uint16             routeIdx;
  uint16             routeNum;
#  if ( SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED == STD_ON )
  uint16             offset      = Length;

  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->UdpStrictHdrLenCheckEnabled == TRUE )
  {
    offset = 0u;

    /* check if PDU Header lengths is valid for entire frame */
    while ( offset < Length )
    {
      if ( ((uint16)(Length - offset)) >= SOAD_PDU_HDR_SIZE )
      {
        IPBASE_GET_UINT32(BufPtr, (offset + SOAD_PDU_HDR_ID_SIZE), pduHdrLen);

        offset += SOAD_PDU_HDR_SIZE;
        if ( pduHdrLen > ((SoAd_PduHdrLenType)(Length - offset)) )
        {
          break;
        }
        offset += (uint16)pduHdrLen;
      }
      else
      {
        break;
      }
    }
  }

  if ( offset == Length )
  { /* PDU Header length is valid */
#  endif /* SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED == STD_ON */

    while ( Length > readDataLen )
    {
      boolean validPduIdFound = FALSE;
#  if ( SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON )
      SoAd_SoConIdxType soConIdxEnd = (SoAd_SoConIdxType)((SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart +
        SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum) - 1u);
#  endif /* SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON */

      /* check whether complete PDU header is available */
      if ( SOAD_PDU_HDR_SIZE > (uint16)(Length - readDataLen) )
      { /* invalid length - discard entire message */
        break;
      }

      /* extract PDU information from buffer */
      IPBASE_GET_UINT32(BufPtr, readDataLen, pduHdrId);
      IPBASE_GET_UINT32(BufPtr, (readDataLen + SOAD_PDU_HDR_ID_SIZE), pduHdrLen);

      /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
      if ( (SOAD_PDU_MAX_LEN < pduHdrLen) || ((Length - readDataLen - SOAD_PDU_HDR_SIZE) < pduHdrLen) )
      { /* received PDU header length invalid - discard entire message */
        break;
      }

#  if ( SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON )
      while ( SoConIdx <= soConIdxEnd )
      {
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->BestMatchWithPduHeaderEnabled == TRUE )
        {
          if ( SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum > 1u )
          { /* more than one socket connection is configured for this socket - use the best match algorithm */
            if ( E_OK != SoAd_GetSoConIdByBestMatchAlg(RemoteAddrPtr, SoConIdx, soConIdxEnd,
              SOAD_SOCON_FILTER_DISABLED,
#   if ( SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED == STD_ON )
              SoAd_SockCons(SoConIdx).SockConGrpPtr->BestMatchWithSockRouteEnabled,
#   else
              FALSE,
#   endif /* defined(SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED)&& STD_ON == SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED) */
              &SoConIdx) )
            { /* no suitable socket connection found */
              readDataLen += (uint16)(pduHdrLen + SOAD_PDU_HDR_SIZE);
              break;
            }
          }
        }
#  endif /* SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON */

        routeIdx = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxStart;
        routeNum = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxNum;

        while ( 0 < routeNum )
        { /* try to find the corresponding socket route for the extracted PDU */
          if ( SoAd_SockRoutes(routeIdx).RxPduHdrId == pduHdrId )
          { /* PDU ID match - check length */
            if ( pduHdrLen > (SoAd_PduHdrLenType)(Length - readDataLen) )
            { /* invalid length - discard entire message */
              break;
            }
            validPduIdFound = TRUE;
            if ( (0u == pduHdrLen) && (SOAD_UL_TYPE_TP == SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer) )
            { /* PDU is valid but has length 0 - not possible for TP upper layer */
              break;
            }

            /* validate transmission/reception state */
            if ( SOAD_CLOSE_NONE != SoAd_SockConIdx2CloseMode(SoConIdx) )
            { /* transmission/reception is disabled (by a previous socket connection close request) */
              break;
            }

            /* use message acceptance policy if configured */
            if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE )
            {
              if ( E_OK != SoAd_Util_CompareSockAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx),
                RemoteAddrPtr, TRUE) )
              { /* message is not accepted */
                break;
              }
            }

#  if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
            /* check routing group states */
            if ( TRUE != SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
            { /* routing group is not enabled - do not open socket connection */
              break;
            }
#  endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */

            /* validate socket connection state */
            if ( SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(SoConIdx) )
            { /* socket connection is not online - this is only allowed in specific cases */
              Std_ReturnType retVal = E_NOT_OK;

              /* check whether specific case is applicable */
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE )
              { /* message acceptance filter is enabled */
                if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->UdpListenOnly == FALSE )
                { /* open socket connection and update remote address */
                  SoAd_CopyIpAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx), RemoteAddrPtr, FALSE);
                  SoAd_SockConIdx2RemAddrState(SoConIdx) = SOAD_SOCON_IP_SET_PORT_SET;
                  if ( SOAD_SOCK_STATE_ESTABLISHED != SoAd_SockIdx2SockState(sockIdx) )
                  {
                    SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_ESTABLISHED;
                    SoAd_OpenSocks++;
                  }
                  SoAd_SockConIdx2State(SoConIdx) = SOAD_SOCON_ONLINE;
#  if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
                  /* check whether a alive timeout is configured */
                  if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt != 0u )
                  { /* alive timeout is configured */
                    /* set counter */
                    (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxUdpAlive(), SoConIdx,
                      SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt, SoAd_GlobalCounter);
                  }
#  endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */
                  /* inform upper layer about socket connection state change */
                  SoAd_VCallChgFunc(SoConIdx, SoAd_SockCons(SoConIdx).SoConChgFuncBitPattern,
                    SOAD_CHG_FUNC_TYPE_SOCONMODE, SOAD_SOCON_ONLINE);
                  retVal = E_OK;
                }
              }
              if ( E_OK != retVal )
              { /* reception on this socket connection while socket connection is not online is not valid */
                break;
              }
            }
#  if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
            else
            {
              /* update alive timeout if configured */
              SoAd_SoCon_UpdateUdpAliveTimeout(SoConIdx);
            }
#  endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

#  if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
            /* update received remote address */
            if ( NULL_PTR != SoAd_SockCons(SoConIdx).RcvRemSockPtr )
            { /* get received remote address is supported */
              SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              /* copy remote address to local buffer */
              SoAd_CopyIpAddr(SoAd_SockCons(SoConIdx).RcvRemSockPtr, RemoteAddrPtr, TRUE);
               /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
              SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
#  endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */

            if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_IF )
            { /* IF-API */
#  if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
              if ( TRUE == SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
                /* at least one routing group for this socket route destination is enabled */
                /* do not discard PDU for this socket route destination */
#  endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
              {
                pduInfo.SduDataPtr = &BufPtr[readDataLen + SOAD_PDU_HDR_SIZE];
                pduInfo.SduLength  = (PduLengthType)pduHdrLen;

#  if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
                /* add meta data */
                if ( SoAd_AddMetaDataToPdu(SoConIdx, routeIdx, &pduInfo) == E_OK )
#  endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */
                {
                  /* call upper layer IF RxIndication */
                  SoAd_VCallIfRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, &pduInfo);
                }
              }
            }
            else
            { /* TP-API */
              PduLengthType bufferSize = 0u;

              if ( SOAD_INV_PDU_ID == SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
              { /* TP session is not active - otherwise PDU is discarded */
                /* call StartOfReception */
                if ( BUFREQ_OK == SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                  SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                  (PduLengthType)pduHdrLen,
                  &bufferSize) )
                { /* start of reception successful - otherwise PDU is discarded */
                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = routeIdx;
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = (PduLengthType)pduHdrLen;

                  if ( 0u == bufferSize )
                  { /* reception possible but no buffer is available */
                    /* check whether PDU fits into rx buffer */
                    if ( SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending >
                      SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin )
                    { /* buffer is not sufficient - invalid PDU length received */
                      SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                      SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;

                      /* call upper layer RxIndication with negative result value */
                      SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                        SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);
                    }
                    else
                    {
                      /* copy PDU data (without header) to TP rx buffer */
                      IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                        &SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr[0],
                        (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
                        &BufPtr[readDataLen + SOAD_PDU_HDR_SIZE],
                        SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending);
                      *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr =
                        (uint16)SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                    }
                  }
                }
                if ( SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                { /* TP session is active */
                  pduInfo.SduDataPtr = NULL_PTR;
                  pduInfo.SduLength = 1u;

                  /* copy data to upper layer */
                  while ( 0u != SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                  {
                    if ( TRUE == SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested )
                    { /* reception is canceled */
                      SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                      SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;
                      SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;

                      /* call upper layer RxIndication with negative result value */
                      SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                        SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);

                      /* close socket connection in next MainFunction cycle */
                      SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                      /* set event to handle socket connection state in main function */
                      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                      break;
                    }
                    else
                    {
                      /* copy data to upper layer  */
                      if ( NULL_PTR == pduInfo.SduDataPtr )
                      { /* first call of CopyRxData */
                        pduInfo.SduLength = 0u;
                      }

                      /* call CopyRxData */
                      if ( BUFREQ_OK != SoAd_VCallTpCopyRxData(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                        SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                        &pduInfo,
                        &bufferSize) )
                      { /* reception is not possible - discard PDU */
                        SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                        SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;
                        SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;

                        /* call upper layer RxIndication with negative result value */
                        SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                          SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);
                      }
                      else
                      { /* data segment copied successfully */
                        SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending -= pduInfo.SduLength;
                        if ( 0u == SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                        { /* PDU received completely */
                          /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                          SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID;

                          /* call upper layer RxIndication with negative result value */
                          SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                            SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_OK);
                        }
                        else
                        { /* pending PDU data */
                          if ( 0u < bufferSize )
                          { /* upper layer has available buffer to copy data */
                            pduInfo.SduLength = SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                            if ( pduInfo.SduLength > bufferSize )
                            {
                              pduInfo.SduLength = bufferSize;
                            }
                            pduInfo.SduDataPtr = &BufPtr[readDataLen + SOAD_PDU_HDR_SIZE];
                          }
                          else
                          { /* upper layer has no buffer */
                            /* check whether PDU fits into rx buffer */
                            if ( SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending >
                              SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin )
                            { /* buffer is not sufficient - invalid PDU length received */
                              /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                              SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID;
                              SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;

                              /* call upper layer RxIndication with negative result value */
                              SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                                SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);
                            }
                            else
                            {
                              /* copy PDU data (without header) to TP rx buffer */
                              IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                                &SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr[0],
                                (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
                                &BufPtr[readDataLen + SOAD_PDU_HDR_SIZE],
                                SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending);
                              *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr =
                                (uint16)SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                              /* set event to try again in main function */
                              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), SoConIdx);
                              break;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          if ( TRUE == validPduIdFound )
          { /* valid PDU found */
            break;
          }
          routeNum--;
          routeIdx++;
        }
        /* check whether a valid PDU could be extracted */
#  if ( SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON )
        if ( TRUE == validPduIdFound )
        { /* valid PDU found */
          readDataLen += (uint16)(pduHdrLen + SOAD_PDU_HDR_SIZE);
          break;
        }
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->BestMatchWithPduHeaderEnabled == TRUE )
        { /* no valid PDU found and best match considers the PDU */
          SoConIdx++;
          if ( SoConIdx > soConIdxEnd )
          { /* no valid PDU found */
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
            (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_UDP_PDU_HDR_RX_INDICATION,
              SOAD_E_INV_PDUHEADER_ID);
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
            readDataLen += (uint16)(pduHdrLen + SOAD_PDU_HDR_SIZE);
            break;
          }
        }
        else
        { /* no valid PDU found */
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
          (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_UDP_PDU_HDR_RX_INDICATION,
            SOAD_E_INV_PDUHEADER_ID);
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
          readDataLen += (uint16)(pduHdrLen + SOAD_PDU_HDR_SIZE);
          break;
        }
      }
#  else
#   if ( SOAD_DEV_ERROR_REPORT == STD_ON )
        if ( FALSE == validPduIdFound )
        {
          (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_UDP_PDU_HDR_RX_INDICATION,
            SOAD_E_INV_PDUHEADER_ID);
        }
#   endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
        readDataLen += (uint16)(pduHdrLen + SOAD_PDU_HDR_SIZE);
#  endif /* SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON */
    }
#  if ( SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED == STD_ON )
  }
#  endif /* SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED == STD_ON */
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_167 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_UdpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_UdpRxIndication(
  SoAd_SoConIdxType SoConIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length)
{
  PduInfoType pduInfo;
  /* get all affected socket routes */
  uint16 routeIdx = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxStart;
  uint16 routeNum = SoAd_SockConIdx2SockRouteCol(SoConIdx).SockRouteIdxNum;

  while ( 0 < routeNum )
  { /* handle reception for all configured socket routes related to this socket connection */
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
    if ( FALSE == SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
    { /* all routing groups for this socket route destination are disabled */
      /* discard PDU for this socket route destination */
    }
    else
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
    {
      if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_IF )
      { /* IF-API */
        pduInfo.SduDataPtr = &BufPtr[0];
        pduInfo.SduLength  = (PduLengthType)Length;

# if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
        /* add meta data */
        if ( SoAd_AddMetaDataToPdu(SoConIdx, routeIdx, &pduInfo) == E_OK )
# endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */
        {
          /* call corresponding upper layer IF RxIndication */
          SoAd_VCallIfRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
            SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, &pduInfo);
        }
      }
      else
      { /* TP-API */
        if ( SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        { /* TP session is active - discard message otherwise */
          /* check whether rx buffer is filled already */
          if ( 0u < *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr )
          { /* buffer is filled already */
            /* check whether PDU fits into rx buffer */
            if ( (*SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr + Length) <=
              SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin )
            { /* buffer is sufficient - discard message otherwise */
              /* copy PDU data (without header) to TP rx buffer */
              IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                &SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr[
                  *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr],
                (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))&BufPtr[0],
                Length);
              *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr += Length;
              SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending += (PduLengthType)Length;
            }
          }
          else
          { /* buffer is empty - try to copy data here */
            PduLengthType bufferSize = 1u;

            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength = 1u;

            SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = (PduLengthType)Length;

            /* copy data to upper layer */
            while ( (0u < bufferSize) && (0u != SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending) )
            {
              if ( TRUE == SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested )
              { /* reception is canceled */
                SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;
                SoAd_SockConIdx2RxMgt(SoConIdx).CancelRequested = FALSE;

                /* call upper layer RxIndication with negative result value */
                SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                  SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);

                /* close socket connection in next MainFunction cycle */
                SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                /* set event to handle socket connection state in main function */
                SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                break;
              }
              else
              {
                /* copy data to upper layer  */
                if ( NULL_PTR == pduInfo.SduDataPtr )
                { /* first call of CopyRxData */
                  pduInfo.SduLength = 0u;
                }

                /* call CopyRxData */
                if ( BUFREQ_OK != SoAd_VCallTpCopyRxData(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                  SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                  &pduInfo,
                  &bufferSize) )
                { /* reception is not possible - close socket connection */
                  SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending = 0u;
                  SoAd_SockConIdx2RxMgt(SoConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                  SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
                  break;
                }
                /* data segment copied successfully */
                SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending -= pduInfo.SduLength;
                if ( 0u != SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending )
                { /* pending PDU data */
                  if ( 0u < bufferSize )
                  { /* upper layer has buffer to copy data */
                    pduInfo.SduLength = SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                    if ( pduInfo.SduLength > bufferSize )
                    {
                      pduInfo.SduLength = bufferSize;
                    }
                    pduInfo.SduDataPtr = &BufPtr[Length - SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending];
                  }
                  else
                  { /* upper layer has no buffer */
                    /* check whether PDU fits into rx buffer */
                    if ( SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending <=
                      SoAd_SockCons(SoConIdx).SockConGrpPtr->SockTpRxBufMin )
                    {
                      /* copy PDU data to TP rx buffer */
                      IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_VAR))
                          &SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferPtr[0],
                        (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
                          &BufPtr[Length - SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending],
                        SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending);
                      *SoAd_SockConIdx2RxBuffer(SoConIdx).TpRxBufferOffsetPtr =
                        (uint16)SoAd_SockConIdx2RxMgt(SoConIdx).RxBytesPending;
                      /* set event to try again in main function */
                      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), SoConIdx);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    routeNum--;
    routeIdx++;
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_AddMetaDataToPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_AddMetaDataToPdu(
  SoAd_SoConIdxType SoConIdx,
  uint16 RouteIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdType soConId = SoAd_SockCons(SoConIdx).SoConId;
  Std_ReturnType   retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if meta data are enabled for this RxPdu. */
  if ( SoAd_SockRoutes(RouteIdx).SockRouteDest.MetaDataRxEnabled == TRUE )
  {
    /* #20 Check if buffer size is sufficient. */
    if ( (PduInfoPtr->SduLength + SOAD_META_DATA_LEN) <= SoAd_MetaDataRxBufferSize() )
    {
      /* #30 Copy PDU to meta data buffer. */
      IpBase_Copy(
        (SOAD_P2VAR(IpBase_CopyDataType))SoAd_MetaDataRxBuffer(),
        (SOAD_P2VAR(IpBase_CopyDataType))PduInfoPtr->SduDataPtr,
        PduInfoPtr->SduLength);

      /* #40 Copy meta data (i.e. SoConId) to meta data buffer. */
# if ( SOAD_SO_CON_ID_TYPE_UINT16 == STD_ON )
      SoAd_MetaDataRxBuffer()[PduInfoPtr->SduLength]      = (uint8)(soConId >> 8u);
# else
      SoAd_MetaDataRxBuffer()[PduInfoPtr->SduLength]      = (uint8)(0u);
# endif /* SOAD_SO_CON_ID_TYPE_UINT16 == STD_ON */
      SoAd_MetaDataRxBuffer()[PduInfoPtr->SduLength + 1u] = (uint8)(soConId);

      /* #50 Overwrite PDU info pointer. */
      PduInfoPtr->SduDataPtr = SoAd_MetaDataRxBuffer();
      PduInfoPtr->SduLength += SOAD_META_DATA_LEN;
    }
    else
    {
# if ( SOAD_DEV_ERROR_REPORT == STD_ON )
      (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_ADD_META_DATA_TO_PDU, SOAD_E_NOBUFS);
# endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TcpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TcpTxConfirmation(
  SoAd_SoConIdxType SoConIdx,
  SoAd_PduIdxType TxPduIdx)
{
  if ( (SoAd_TxPduIdx2PduRoute(TxPduIdx).UpperLayer == SOAD_UL_TYPE_TP) )
  { /* TP-API */
    if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->ImmedTpTxConf == FALSE )
    {
      if ( TRUE != SoAd_SockConIdx2TxMgt(SoConIdx).CancelRequested )
      {
        /* socket connection handling if automatic setup is enabled and wildcards configured */
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
        {
          if ( (SOAD_SOCON_IP_ANY_PORT_ANY >= SoAd_SockConIdx2RemAddrState(SoConIdx)) &&
            (0u != (SoAd_SockCons(SoConIdx).RemAddrState & SOAD_SOCON_MASK_ANY)) )
          {
            SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_RECONNECT;
            /* set event to handle socket connection state in main function */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
          }
        }

        /* reset TxMgt struct */
        SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;

        /* reset local socket index to active socket connection mapping */
        SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

        SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(TxPduIdx).UpperLayerId,
          SoAd_TxPduIdx2TxConfPduId(TxPduIdx), E_OK);
      }
    }
  }
  else
  { /* IF-API */
    if ( SoAd_TxPduIdx2PduRoute(TxPduIdx).TxConfEnabled == TRUE )
    { /* TxConfirmation is configured for the corresponding upper layer and PDU */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if ( 0u < (SoAd_TxPduIdx2PendingTxConf(TxPduIdx) & SOAD_TX_CONF_STRAT_BIT_PAT_CONF) )
      {
        /* there is a pending TxConfirmation for one of the PduRouteDests */
        SoAd_TxPduIdx2PendingTxConf(TxPduIdx)--;
        if ( 0u == SoAd_TxPduIdx2PendingTxConf(TxPduIdx) )
        { /* call TxConfirmation of upper layer only if all PduRouteDests are confirmed (PDU fan-out) */
           /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          /* socket connection handling if automatic setup is enabled and wildcards configured */
          if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
          {
            if ( (SOAD_SOCON_IP_ANY_PORT_ANY >= SoAd_SockConIdx2RemAddrState(SoConIdx)) &&
              (0u != (SoAd_SockCons(SoConIdx).RemAddrState & SOAD_SOCON_MASK_ANY)) )
            {
              SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_RECONNECT;
              /* set event to handle socket connection state in main function */
              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
            }
          }
          /* call TxConfirmation of upper layer */
          SoAd_VCallIfTxConfirmation(SoAd_TxPduIdx2PduRoute(TxPduIdx).UpperLayerId,
            SoAd_TxPduIdx2TxConfPduId(TxPduIdx));
        }
        else
        {
         /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }
      else
      {
        SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
      }
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */
# endif /* SOAD_VTCP_USED == STD_ON */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_HandleShutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleShutdown(void)
{
  if ( 0 != SoAd_OpenSocks )
  { /* sockets still opened */
    if ( 0u < SoAd_ShutdownFinishedCnt )
    {
      SoAd_ShutdownFinishedCnt--;
      if ( 0u == SoAd_ShutdownFinishedCnt )
      {
        SoAd_SockIdxType sockIdx = SoAd_TcpIpSocketCnt();

        /* close all sockets (will implicitly close all socket connections) */
        while ( 0u < sockIdx )
        {
          SoAd_SocketIdType socketId;

          sockIdx--;

          socketId = SoAd_SockIdx2SocketId(sockIdx);
          if ( SOAD_INV_SOCKET_ID != socketId ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          { /* socket still bound */
            SoAd_TcpIp_Close(socketId, TRUE, sockIdx);
          }
        }
      }
    }
  }
  else
  { /* all sockets closed */
    SoAd_ShutdownFinishedCnt = 0u;
  }
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_HandleShutdownFinish()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleShutdownFinish(void)
{
  if ( (0u == SoAd_ShutdownFinishedCnt) || (0u == SoAd_OpenSocks) )
  { /* shutdown finished */
    SoAd_State = SOAD_STATE_SHUTDOWN;
#if ( SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
    SoAd_VCallShutdwnFinishedFuncs();
#endif /* SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
  }
  else
  {
    boolean soConsClosed = TRUE;
    SoAd_SoConIdxType soConIdx = SoAd_SockConTotal();

    while ( 0u < soConIdx )
    {
      soConIdx--;

      if ( SOAD_SOCON_OFFLINE != SoAd_SockConIdx2State(soConIdx) )
      { /* socket connection not offline */
        soConsClosed = FALSE;
        if ( SOAD_CLOSE_NONE == SoAd_SockConIdx2CloseMode(soConIdx) )
        { /* socket connection not closing */
          /* check for pending transmissions */
          if ( SOAD_INV_PDU_IDX == SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          { /* transmission is not active (e.g. TP session) */
#if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
            boolean nPduPending = FALSE;

            /* check if nPduUdpTx is filled */
            if ( SoAd_IsNPduUdpTxPresent() )
            {
              if ( SoAd_SockConIdx2NPduUdpTxIdx(soConIdx) != SOAD_INV_BUF_IDX_16 )
              {
                if ( SoAd_NPduUdpTx(SoAd_SockConIdx2NPduUdpTxIdx(soConIdx)).MgtPtr->TotalLen != 0u )
                { /* nPdu is pending */
                  /* update timeout to send nPdu immediately in next main function */
                  (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(),
                      SoAd_SockConIdx2NPduUdpTxIdx(soConIdx), 1u, SoAd_GlobalCounter);
                  nPduPending = TRUE;
                }
              }
            }
            if ( nPduPending == FALSE )
              /* nPduUdpTx is empty */
#endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
            {
#if ( SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON )
              /* check if routing group transmit is pending */
              uint16 pduRouteDestIdx;
              uint16 pduRouteDestNum = SoAd_SockConIdx2PduRouteDestCol(soConIdx).PduRouteDestListSize;
              boolean routGrpTransmitPending = FALSE;

              for ( pduRouteDestIdx = 0u; pduRouteDestIdx < pduRouteDestNum; pduRouteDestIdx++ )
              {
                SoAd_RoutingGroupIdType grpIdx;
                SoAd_RoutingGroupIdType grpNum = SoAd_SockConIdx2PduRouteDestCol(soConIdx).
                  PduRouteDestListPtr[pduRouteDestIdx]->RoutGrpNum;

                for ( grpIdx = 0u; grpIdx < grpNum; grpIdx++ )
                {
                  if ( (*SoAd_SockConIdx2PduRouteDestCol(soConIdx).PduRouteDestListPtr[
                      pduRouteDestIdx]->RoutGrpStateColPtr[grpIdx] & SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER) != 0u )
                  { /* if routing group transmit */
                    routGrpTransmitPending = TRUE;
                    break;
                  }
                }
                if ( routGrpTransmitPending == TRUE )
                {
                  break;
                }
              }
              if ( routGrpTransmitPending == FALSE )
#endif /* SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON */
              {
                SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                /* set event to handle socket connection state in main function */
                SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
              }
            }
          }
        }
      }
    }
    if ( TRUE == soConsClosed )
    { /* all socket connections are offline */
      SoAd_SockIdxType sockIdx = SoAd_TcpIpSocketCnt();

      /* release UDP sockets for socket connection which can be opened on reception */
      while ( 0u < sockIdx )
      {
        SoAd_SocketIdType socketId;

        sockIdx--;

        socketId = SoAd_SockIdx2SocketId(sockIdx);
        if ( SOAD_INV_SOCKET_ID != socketId ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        { /* socket still bound */
          if ( NULL_PTR != SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->SocketUdpPtr )
          { /* UDP socket */
            SoAd_TcpIp_Close(socketId, FALSE, sockIdx);
          }
        }
      }

      /* check if all sockets are released now */
      if ( 0u == SoAd_OpenSocks )
      {
        SoAd_State = SOAD_STATE_SHUTDOWN;
#if ( SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
        SoAd_VCallShutdwnFinishedFuncs();
#endif /* SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
      }
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_HandleSoConStates()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleSoConStates(void)
{
  uint16 handleIdx = SOAD_INV_SO_CON_IDX;
  SoAd_SoConIdxType lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxStateSoCon()).EveMgtPtr->Lvl;
  SoAd_SoConIdxType_least idx;

#if ( SOAD_VEVE_QUE_LIMIT_STATESOCON_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxStateSoCon()).EveQueLimit )
  {
    lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxStateSoCon()).EveQueLimit;
  }
#endif /* SOAD_VEVE_QUE_LIMIT_STATESOCON_ENALBED == STD_ON */

  for ( idx = 0u; idx < lvl; idx++ )
  {
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxStateSoCon(), &handleIdx) == E_OK )
    {
      SoAd_SoConIdxType soConIdx = (SoAd_SoConIdxType)handleIdx;

      if ( SOAD_CLOSE_NONE != SoAd_SockConIdx2CloseMode(soConIdx) )
      {
        /* close socket connection */
        SoAd_CloseSockCon(soConIdx);
      }
      else
      {
        if ( SoAd_State == SOAD_STATE_INIT )
        { /* perform open actions not in case of shutdown */
          if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE) ||
            (SOAD_SOCON_RECONNECT == SoAd_SockConIdx2State(soConIdx)) ||
            (0u < SoAd_SockConIdx2FlagCntOpCl(soConIdx)) )
          { /* open socket connection automatically or if requested */
            if ( (SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(soConIdx)) &&
              (SOAD_IPADDR_STATE_ASSIGNED == SoAd_SockConIdx2IpAddrState(soConIdx)) )
            { /* socket connection is not online and IP is assigned */
              SoAd_OpenSockCon(soConIdx);
            }
          }
          else
          { /* do not open socket connection but check if TcpIp socket shall be assigned */
            if ( SOAD_IPADDR_STATE_ASSIGNED == SoAd_SockConIdx2IpAddrState(soConIdx) )
            { /* IP is assigned */
              if ( SOAD_SOCK_STATE_CLOSED == SoAd_SockIdx2SockState(SoAd_SockCons(soConIdx).SockIdx) )
              { /* no TcpIp socket is assigned */
                if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP )
                { /* UDP socket connection */
                  if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE) &&
                    (SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpListenOnly == FALSE) )
                  { /* message acceptance filter is enabled and UDP listen only disabled */
                    boolean ipAddrIsAny = FALSE;
                    boolean portIsAny = FALSE;

                    SoAd_CheckForWildcards(SoAd_SockCons(soConIdx).RemSockPtr, &ipAddrIsAny, &portIsAny);
                    if ( (TRUE == ipAddrIsAny) || (TRUE == portIsAny) )
                    { /* wildcards are configured */
                      /* all conditions are fulfilled to open socket connection on reception - assign socket */
                      SoAd_AssignUdpSocket(soConIdx);
                      if ( SoAd_SockIdx2SockState(SoAd_SockCons(soConIdx).SockIdx) != SOAD_SOCK_STATE_ESTABLISHED )
                      { /* TcpIp socket not assigned */
                        /* set event to handle socket connection state in main function again */
                        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else
    { /* no element left */
      break;
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleUdpAliveTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleUdpAliveTimeout(void)
{
  uint16 elementIdx = 0u;
  uint16 handleIdx = SOAD_INV_HND_16;

  while ( SoAd_TimeoutList_CheckElements(SoAd_TimeoutListIdxUdpAlive(), &elementIdx, &handleIdx, SoAd_GlobalCounter) ==
    E_OK )
  { /* timeout occurred */
    SoAd_SoConIdxType soConIdx = (SoAd_SoConIdxType)handleIdx;

    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* handle timeout */
    if ( (SOAD_SOCON_ONLINE == SoAd_SockConIdx2State(soConIdx)) &&
      (SOAD_CLOSE_NONE == SoAd_SockConIdx2CloseMode(soConIdx)) )
    { /* socket connection is still online and not requested to be closed */
      /* close socket connection later in MainFunction */
      SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_RECONNECT;

      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

      /* set event to handle socket connection state in main function */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
    }
    else
    {
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
    }

    /* remove element from list */
    SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxUdpAlive(), handleIdx);
  }
}
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTpReception()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpReception(void)
{
  uint16 handleIdx = SOAD_INV_SO_CON_IDX;
  SoAd_SoConIdxType lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTpRxSoCon()).EveMgtPtr->Lvl;
  SoAd_SoConIdxType_least idx;

# if ( SOAD_VEVE_QUE_LIMIT_TPRXSOCON_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxTpRxSoCon()).EveQueLimit )
  {
    lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTpRxSoCon()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_TPRXSOCON_ENALBED == STD_ON */

  for ( idx = 0u; idx < lvl; idx++ )
  {
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxTpRxSoCon(), &handleIdx) == E_OK )
    {
      boolean addEventAgain = FALSE;
      SoAd_SoConIdxType soConIdx = (SoAd_SoConIdxType)handleIdx;

      /* check socket connection state */
      if ( (SOAD_SOCON_ONLINE == SoAd_SockConIdx2State(soConIdx)) &&
        (SOAD_CLOSE_NONE == SoAd_SockConIdx2CloseMode(soConIdx)) )
      { /* socket connection is online and not requested to be closed */
        SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if ( TRUE == SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested )
        { /* reception is canceled */
          uint16 routeIdx = SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx;

          if ( SOAD_INV_PDU_ID != routeIdx )
          {
            if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP )
            {
              *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr = 0u;
            }
            SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending = 0u;
            SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
             /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
            SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            /* call TpRxIndication */
            SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
              SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);
          }
          else
          {
            /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
            SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          }

          /* close socket connection in next MainFunction cycle */
          SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
          /* set event to handle socket connection state in main function */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);

          SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested = FALSE;
        }
        else
        {
          /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# if ( SOAD_VTCP_USED == STD_ON )
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP)
          { /* TCP socket */
            /* check whether TcpIp buffer is available */
            if ( 0u < SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Lvl )
            {
              uint16 curLvl = (uint16)(SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Lvl - 1u);
              uint16 lastIdx;

              /* get current index */
              if ( (SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Idx + curLvl) >= SOAD_VBUF_SEG_SIZE_TOTAL )
              { /* wrap around necessary */
                lastIdx = (uint16)(curLvl - (SOAD_VBUF_SEG_SIZE_TOTAL -
                  SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Idx));
              }
              else
              { /* no wrap around necessary */
                lastIdx = SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Idx + curLvl;
              }

              /* call RxIndication again */
              /* PRQA S 3689 4 */ /* MD_SoAd_21.1 */
              SoAd_RxIndication(SoAd_SockIdx2SocketId(SoAd_SockCons(soConIdx).SockIdx),
                (SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(soConIdx),
                SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.BufSegs[lastIdx].DataPtr,
                SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.BufSegs[lastIdx].Len);

              if ( SoAd_SockConIdx2RxMgt(soConIdx).BufSegMgt.Lvl > 0u )
              { /* buffer still contains data so retry in next main function is required */
                addEventAgain = TRUE;
              }
            }
          }
          else
# endif /* SOAD_VTCP_USED == STD_ON */
          { /* UDP socket */
            PduLengthType bufferSize = 1u;

            /* UDP over TP has always a TP rx buffer - if no buffer is configured socket connection has no TP */
            if ( NULL_PTR != SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr )
            {
              if ( (0u < *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr)
                && (SOAD_INV_PDU_ID != SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx) ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
              { /* active TP session and rx data are available */
                PduInfoType  pduInfo;
                uint16       routeIdx = SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx;

                pduInfo.SduDataPtr = NULL_PTR;
                pduInfo.SduLength  = 1u;

                /* copy data to upper layer */
                while ( (0u < bufferSize) && (0u < SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending) )
                {
                  if ( TRUE == SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested )
                  { /* reception is canceled */
                    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                    *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr = 0u;
                    SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending = 0u;
                    SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested = FALSE;
                    SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                     /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                    /* call TpRxIndication */
                    SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                      SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_NOT_OK);

                    /* close socket connection */
                    SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                    /* set event to handle socket connection state in main function */
                    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                    break;
                  }
                  else
                  {
                    /* copy data to upper layer  */
                    if ( NULL_PTR == pduInfo.SduDataPtr )
                    { /* first call of CopyRxData */
                      pduInfo.SduLength = 0u;
                    }

                    /* call CopyRxData */
                    if ( BUFREQ_OK != SoAd_VCallTpCopyRxData(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                        SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                        &pduInfo,
                        &bufferSize) )
                    { /* reception is not possible - close socket connection */
                      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                      *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr = 0u;
                      SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending = 0u;
                      SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = SOAD_INV_PDU_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
                      SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                      /* set event to handle socket connection state in main function */
                      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                      /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                    }
                    else
                    { /* data segment copied successfully */
                      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                      SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending -= pduInfo.SduLength;
                      if ( 0u == SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending )
                      { /* all data copied completely to upper layer - release rx buffer */
                        *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr = 0u;
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
                        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
                        { /* PDU Header option is enabled */
                          /* if disabled TP session is finished on socket connection state change */
                          /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                          SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = SOAD_INV_PDU_ID;
                          /* call TpRxIndication */
                          SoAd_VCallTpRxIndication(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                            SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId, E_OK);
                        }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
                      }
                      else
                      { /* pending PDU data */
                        if ( 0u < bufferSize )
                        { /* upper layer has buffer to copy data */
                          pduInfo.SduLength = SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending;
                          if ( pduInfo.SduLength > bufferSize )
                          {
                            pduInfo.SduLength = bufferSize;
                          }
                          pduInfo.SduDataPtr = &SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferPtr[
                            *SoAd_SockConIdx2RxBuffer(soConIdx).TpRxBufferOffsetPtr -
                            SoAd_SockConIdx2RxMgt(soConIdx).RxBytesPending];
                        }
                        else
                        {
                          addEventAgain = TRUE;
                        }
                      }
                      /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                    }
                  }
                }
              }
            }
          }
        }
      }

      if ( addEventAgain == TRUE )
      {
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), handleIdx);
      }
    }
    else
    { /* no element left */
      break;
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTpTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpTransmission(void)
{
  uint16 handleIdx = SOAD_INV_SO_CON_IDX;
  SoAd_SoConIdxType lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTpTxSoCon()).EveMgtPtr->Lvl;
  SoAd_SoConIdxType_least idx;

# if ( SOAD_VEVE_QUE_LIMIT_TPTXSOCON_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxTpTxSoCon()).EveQueLimit )
  {
    lvl = (SoAd_SoConIdxType)SoAd_EventQueues(SoAd_EventQueueIdxTpTxSoCon()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_TPTXSOCON_ENALBED == STD_ON */

  for ( idx = 0u; idx < lvl; idx++ )
  {
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxTpTxSoCon(), &handleIdx) == E_OK )
    {
      SoAd_SoConIdxType soConIdx = (SoAd_SoConIdxType)handleIdx;

      SoAd_HandleTpTransmissionInMain(soConIdx);
    }
    else
    { /* no element left */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  SoAd_HandleTpTransmissionInMain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTpTransmissionInMain(
  SoAd_SoConIdxType SoConIdx)
{
  SoAd_PduIdxType txPduIdx = SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx;

  if ( SOAD_INV_PDU_IDX != txPduIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  { /* pending PDU transmission */
    if ( SOAD_UL_TYPE_TP == SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayer )
    { /* pending TP PDU transmission */
      /* check socket connection state */
      if ( (SOAD_SOCON_ONLINE == SoAd_SockConIdx2State(SoConIdx)) &&
        (SOAD_CLOSE_NONE == SoAd_SockConIdx2CloseMode(SoConIdx)) )
      { /* socket connection is online and not requested to be closed */
        /* handle TP cancellation */
        if ( TRUE == SoAd_SockConIdx2TxMgt(SoConIdx).CancelRequested )
        {
          /* reset TxMgt struct */
          SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;
          SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = 0u;

# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
          /* release SoAd local buffer */
          if ( SoAd_IsSockConIdx2TxBufferPresent() &&
            (NULL_PTR != SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr) )
          { /* buffer is available */
            *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr = 0u;
          }
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */

# if ( SOAD_VTCP_USED == STD_ON )
          /* release tx queue element if available */
          if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
          {
            SoAd_TcpTxQueueSetElement(SoConIdx, txPduIdx, 0u, TRUE);
          }
# endif /* SOAD_VTCP_USED == STD_ON */

          /* call TxConfirmation */
          SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
            SoAd_TxPduIdx2TxConfPduId(txPduIdx),
            E_NOT_OK);

          SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
          /* set event to handle socket connection state in main function */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);

          SoAd_SockConIdx2TxMgt(SoConIdx).CancelRequested = FALSE;
        }
        else
        {
# if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
          /* update alive timeout if configured */
          if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
          {
            SoAd_SoCon_UpdateUdpAliveTimeout(SoConIdx);
          }
# endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */
          (void)SoAd_HandleTpTransmissionOnSockCon(SoConIdx, TRUE);
        }
      }
    }
  }
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_HandleTpTransmissionOnSockCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 3 */ /* MD_SoAd_3206 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_HandleTpTransmissionOnSockCon(
  SoAd_SoConIdxType SoConIdx,
  boolean CalledByMain)
{
  Std_ReturnType  retVal = E_OK;
  PduLengthType   availableBufLen = 1u;
  SoAd_PduIdxType txPduIdx = SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx;

  while ( (0u < availableBufLen) && (0u != SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending) )
  {
    PduInfoType pduInfo;

    /* query available data from upper layer */
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0u;

    /* call TpCopyTxData */
    if ( BUFREQ_OK == SoAd_VCallTpCopyTxData(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
        SoAd_TxPduIdx2TxConfPduId(txPduIdx), &pduInfo, &availableBufLen) )
    {
      if ( 0u < availableBufLen )
      { /* data are available */
        PduLengthType maxExpectedBufLen = SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending;
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
        PduLengthType pduHdrRestLen;

        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
        { /* PDU header enabled */
          /* calculate remaining PDU header length */
          pduHdrRestLen = SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen -
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending;

          if ( SOAD_PDU_HDR_SIZE < pduHdrRestLen )
          {
            pduHdrRestLen = 0u;
          }
          else
          {
            pduHdrRestLen = SOAD_PDU_HDR_SIZE - pduHdrRestLen;
          }

          /* maximum expected available buffer length does not contain PDU header */
          maxExpectedBufLen -= pduHdrRestLen;
        }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

        if ( maxExpectedBufLen < availableBufLen )
        { /* available buffer length is greater than expected */
          availableBufLen = maxExpectedBufLen;
        }

# if ( SOAD_VTCP_USED == STD_ON )
        if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
        { /* TCP socket */
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
          pduHdrRestLen = 0u;
          if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
          { /* PDU header enabled */
            /* add PDU header to availableBufLen if not sent yet */
            pduHdrRestLen = SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen -
              SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending;

            if ( SOAD_PDU_HDR_SIZE < pduHdrRestLen )
            {
              pduHdrRestLen = 0u;
            }
            else
            {
              pduHdrRestLen = SOAD_PDU_HDR_SIZE - pduHdrRestLen;
            }
          }

          /* transmit data over TCP */
          if ( E_OK != SoAd_TcpIp_TcpTransmit(SoConIdx, (availableBufLen + pduHdrRestLen)) )
#  else
          if ( E_OK != SoAd_TcpIp_TcpTransmit(SoConIdx, availableBufLen) )
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
          { /* transmission failed */
            /* reset TxMgt struct */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;
            SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = 0u;

            /* reset local socket index to active socket connection mapping */
            SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

            if ( SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen !=
              SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending )
            { /* transmission is already active */
              /* close socket connection in next MainFunction */
              SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
              /* set event to handle socket connection state in main function */
              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
            }
            else
            { /* failed at beginning and no data has been sent before */
              /* release tx queue element */
              SoAd_TcpTxQueueSetElement(SoConIdx, txPduIdx, 0u, TRUE);
            }

            retVal = E_NOT_OK;

            if ( CalledByMain == TRUE )
            {
              /* call upper layer TxConfirmation */
              SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_NOT_OK);
            }
          }
          else
          { /* call to TcpIp successful */
            if ( 0u == SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending )
            { /* transmission finished */
              if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SocketTcpPtr->ImmedTpTxConf == TRUE )
              {
                /* reset TxMgt struct */
                /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;

                /* reset local socket index to active socket connection mapping */
                SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

                /* socket connection handling if automatic setup is enabled and wildcards configured */
                if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
                {
                  if ( (SOAD_SOCON_IP_ANY_PORT_ANY >= SoAd_SockConIdx2RemAddrState(SoConIdx)) &&
                    (0u != (SoAd_SockCons(SoConIdx).RemAddrState & SOAD_SOCON_MASK_ANY)) )
                  {
                    SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_RECONNECT;
                  }
                }

                /* call upper layer TxConfirmation (this is MainFunction context - no need to wait) */
                SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                  SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_OK);
              }
            }
          }
        }
        else
# endif /* SOAD_VTCP_USED == STD_ON */
        { /* UDP socket */
# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
          if ( SoAd_IsSockConIdx2TxBufferPresent() )
          {
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
            if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
            { /* PDU header enabled */
              if ( 0u == *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr )
              { /* PDU header not yet copied to SoAd local buffer */
                /* copy PDU header first */
                SoAd_CopyPduHdr2Buf(SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[0]->TxPduHdrId,
                  (SoAd_PduHdrLenType)(SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen - SOAD_PDU_HDR_SIZE),
                  &SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferPtr[0],
                  0u, SOAD_PDU_HDR_SIZE);
                *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr = SOAD_PDU_HDR_SIZE;
              }
            }
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
            while ( 0u < availableBufLen )
            { /* copy upper layer data to local buffer until no buffer is available or PDU is copied */
              /* set pduInfo */
              pduInfo.SduDataPtr = &SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferPtr[
                *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr];
              pduInfo.SduLength = availableBufLen;

              if ( BUFREQ_OK == SoAd_VCallTpCopyTxData(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                  SoAd_TxPduIdx2TxConfPduId(txPduIdx), &pduInfo, &availableBufLen) )
              {
                *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr += (uint16)pduInfo.SduLength;

                if ( *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr ==
                  SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen )
                { /* PDU copied completely to local buffer */
                  SoAd_SockConIdx2TxMgt(SoConIdx).TxBufPtr =
                    &SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferPtr[0];
                    /* transmit PDU over UDP */
                  if ( E_OK != SoAd_TcpIp_UdpTransmit(SoAd_SockCons(SoConIdx).SockIdx,
                      SoAd_SockIdx2SocketId(SoAd_SockCons(SoConIdx).SockIdx),
                      (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(SoConIdx),
                      (uint16)SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen) )
                  {
                    retVal = E_NOT_OK;
                  }

                  /* release local Tx buffer */
                  *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr = 0u;

                  /* reset TxMgt struct */
                  /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                  SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX;
                  SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;
                  SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = 0u;

                  /* reset local socket index to active socket connection mapping */
                  SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

                  if ( E_OK != retVal )
                  {
                    if ( CalledByMain == TRUE )
                    {
                      /* call negative TxConfirmation */
                      SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                        SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_NOT_OK);
                    }
                  }
                  else
                  {
                    /* socket connection handling if automatic setup is enabled and wildcards configured */
                    SoAd_ReconnectSockCon(SoConIdx);
                    /* call positive TxConfirmation */
                    SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                      SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_OK);
                  }
                  break;
                }
                else
                { /* PDU not copied completely to local buffer */
                  if ( availableBufLen == 0u )
                  { /* no buffer available anymore */
                    /* set event to handle TP transmission in main function */
                    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpTxSoCon(), SoConIdx);
                  }
                }
              }
              else
              { /* call to upper layer failed */
                /* release local Tx buffer */
                *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr = 0u;

                /* reset TxMgt struct */
                /* PRQA S 0277 1 */ /* MD_SoAd_3.1 */
                SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;
                SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = 0u;

                /* reset local socket index to active socket connection mapping */
                SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

                retVal = E_NOT_OK;

                if ( CalledByMain == TRUE )
                {
                  /* call negative TxConfirmation */
                  SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                    SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_NOT_OK);
                }

                break;
              }
            }
          }
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */
        }
      }
      else
      {
        /* set event to handle TP transmission in main function */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpTxSoCon(), SoConIdx);
      }
    }
    else
    { /* call to upper layer failed */
      retVal = E_NOT_OK;
# if ( SOAD_VTCP_USED == STD_ON )
      if ( SoAd_SockCons(SoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
      { /* TCP */
        /* close socket in next MainFunction in case of TCP */
        SoAd_SockConIdx2CloseMode(SoConIdx) = SOAD_CLOSE_OFFLINE_RESET;
        /* set event to handle socket connection state in main function */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), SoConIdx);
      }
# endif /* SOAD_VTCP_USED == STD_ON */
# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
      else
#  endif /* SOAD_VTCP_USED == STD_ON */
      { /* UDP */
        if ( SoAd_IsSockConIdx2TxBufferPresent() )
        {
          /* release local Tx buffer */
          *SoAd_SockConIdx2TxBuffer(SoConIdx).TpTxBufferOffsetPtr = 0u;

          /* reset TxMgt struct */
          SoAd_SockConIdx2TxMgt(SoConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
          SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesLen = 0u;
          SoAd_SockConIdx2TxMgt(SoConIdx).TxBytesPending = 0u;

          /* reset local socket index to active socket connection mapping */
          SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(SoConIdx).SockIdx) = SOAD_INV_SO_CON_IDX;

          if ( CalledByMain == TRUE )
          {
            /* call negative TxConfirmation */
            SoAd_VCallTpTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
              SoAd_TxPduIdx2TxConfPduId(txPduIdx), E_NOT_OK);
          }
        }
      }
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */
      break;
    }
  }

# if ( (SOAD_VTCP_USED == STD_OFF) && (SOAD_VTX_BUF_ENABLED == STD_OFF) )
  SOAD_DUMMY_STATEMENT_VAR(CalledByMain); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* (SOAD_VTCP_USED == STD_OFF) && (SOAD_VTX_BUF_ENABLED == STD_OFF) */

  return retVal;
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleIfRouteGrpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleIfRouteGrpTransmit(void)
{
  uint16 handleIdx = SOAD_INV_ROUT_GRP_IDX;
  SoAd_RoutingGroupIdxType lvl = (SoAd_RoutingGroupIdxType)
    SoAd_EventQueues(SoAd_EventQueueIdxIfTxRouteGrp()).EveMgtPtr->Lvl;
  SoAd_RoutingGroupIdxType_least idx;

# if ( SOAD_VEVE_QUE_LIMIT_IFTXROUTGRP_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxIfTxRouteGrp()).EveQueLimit )
  {
    lvl = (SoAd_RoutingGroupIdxType)SoAd_EventQueues(SoAd_EventQueueIdxIfTxRouteGrp()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_IFTXROUTGRP_ENALBED == STD_ON */

  for ( idx = 0u; idx < lvl; idx++ )
  {
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxIfTxRouteGrp(), &handleIdx) == E_OK )
    {
      SoAd_RoutingGroupIdxType  routGrpIdx = (SoAd_RoutingGroupIdxType)handleIdx;
      PduInfoType               info;
      uint16                    pduRouteDestNum = 0u;
      PduIdType                 txPduId = SOAD_INV_PDU_ID;
      Std_ReturnType            retVal = E_OK;

      /* initialize struct here to prevent compiler warnings */
      info.SduDataPtr = SoAd_IfRouteGrpTransmitBufPtr();
      info.SduLength  = SoAd_IfRouteGrpTransmitBufSize();

      /* iterate over all referenced PduRouteDests */
      while ( SoAd_RouteGrpIdx2PduRouteDestCol(routGrpIdx).PduRouteDestListSize > pduRouteDestNum )
      {
        SOAD_P2CONST(SoAd_PduRouteDestType) pduRouteDestPtr =
          SoAd_RouteGrpIdx2PduRouteDestCol(routGrpIdx).PduRouteDestListPtr[pduRouteDestNum];

        if ( 0u != (*SoAd_RouteGrpIdx2PduRouteDestCol(routGrpIdx).StateColPtr[pduRouteDestNum] &
          SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER) )
        { /* trigger transmit is enabled for this PduRouteDest */
          SoAd_PduIdxType txPduIdx = SoAd_TxPduId2PduRouteIdx(pduRouteDestPtr->TxPduId);

# if ( (SOAD_VTX_DYN_LEN_ENABLED == STD_ON) && (SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON) )
          if ( SoAd_TxPduIdx2PduRoute(txPduIdx).IfTriggerTransmitEnabled == TRUE )
          { /* normal PDU transmission request as trigger transmit */
            /* transmission in context of CopyTxData */
            info.SduDataPtr = NULL_PTR;
            info.SduLength = SoAd_MaxIfRouteGrpTransmitPduSize();

            (void)SoAd_TxIf_TransmitPduRouteDest(pduRouteDestPtr->SoConIdx, &info, pduRouteDestPtr);
          }
          else
# endif /* (SOAD_VTX_DYN_LEN_ENABLED == STD_ON) && (SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON) */
          { /* normal PDU transmission request as direct transmit */
            /* use buffer and call trigger transmit now */
            if ( pduRouteDestPtr->TxPduId == txPduId )
            { /* trigger transmit is already called for the corresponding PduRoute */
              if ( E_NOT_OK != retVal )
              { /* call to upper layer was successful */
                /* send data info parameter pointing to */
                (void)SoAd_TxIf_TransmitPduRouteDest(pduRouteDestPtr->SoConIdx, &info, pduRouteDestPtr);
              }
            }
            else
            { /* retrieve data from upper layer */
              info.SduDataPtr = SoAd_IfRouteGrpTransmitBufPtr();
              info.SduLength  = SoAd_IfRouteGrpTransmitBufSize();
              retVal = E_NOT_OK;
              txPduId = pduRouteDestPtr->TxPduId;

              if ( E_OK == SoAd_VCallIfTriggerTransmit(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx), &info) )
              { /* trigger transmit function call to upper layer successful */
                /* check info parameter */
                if ( (info.SduDataPtr != NULL_PTR) && (info.SduLength != 0) &&
                  (info.SduLength <= SoAd_IfRouteGrpTransmitBufSize()) )
                { /* send data info parameter pointing to */
                  (void)SoAd_TxIf_TransmitPduRouteDest(pduRouteDestPtr->SoConIdx, &info, pduRouteDestPtr);
                  retVal = E_OK;
                }
              }
            }
          }
          /* reset trigger transmit flag later since multiple PduRouteDests of this RoutingGroup may reference the
            * same socket connection */
        }
        pduRouteDestNum++;
      }

      /* reset trigger transmit flags */
      pduRouteDestNum = 0u;
      while ( SoAd_RouteGrpIdx2PduRouteDestCol(routGrpIdx).PduRouteDestListSize > pduRouteDestNum )
      {
        /* PRQA S 0277 2 */ /* MD_SoAd_3.1 */
        *SoAd_RouteGrpIdx2PduRouteDestCol(routGrpIdx).StateColPtr[pduRouteDestNum] &=
          (uint8)(~SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER);
        pduRouteDestNum++;
      }
    }
    else
    { /* no element left */
      break;
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleNPduUdpTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleNPduUdpTx(void)
{
  uint16 elementIdx = 0u;
  uint16 handleIdx = SOAD_INV_HND_16;

  while ( SoAd_TimeoutList_CheckElements(SoAd_TimeoutListIdxNPduUdpTx(),
      &elementIdx, &handleIdx, SoAd_GlobalCounter) == E_OK )
  { /* timeout occurred */
    uint16 nPduUdpTxIdx = handleIdx;

    /* handle timeout */

    /* start transmission if not already active */
    if ( SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TransmissionActive == FALSE )
    {
      SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TransmissionActive = TRUE;
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
      if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr != NULL_PTR )
      {
        SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->RetryCnt = SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN;
      }
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
    }

    /* handle transmission */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
    if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr != NULL_PTR )
    {
      SoAd_NPduQueueSend(nPduUdpTxIdx);
    }
#  if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
    else
#  endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
    {
      SoAd_NPduBufSend(nPduUdpTxIdx);
    }
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */

    /* handling dependent of transmission state */
    if ( SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TransmissionActive == FALSE )
    { /* transmission finished or aborted */
      /* remove element from list */
      SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxNPduUdpTx(), handleIdx);
    }
    else
    { /* transmission pending */
      /* update element to retry in next main function */
      (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), handleIdx, 1u, SoAd_GlobalCounter);
    }
  }
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_NPduClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduClear(
  uint16 NPduUdpTxIdx)
{
  SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen = 0u;
  SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive = FALSE;

  SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxNPduUdpTx(), NPduUdpTxIdx);

# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  if ( SoAd_NPduUdpTx(NPduUdpTxIdx).QueueMgtPtr != NULL_PTR )
  {
    uint8 idx;
    uint8 queueSize = SoAd_SockCons(
      SoAd_NPduUdpTx(NPduUdpTxIdx).SoConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxQueueSize;

    SoAd_NPduUdpTx(NPduUdpTxIdx).QueueMgtPtr->Lvl = 0u;
    for ( idx = 0u; idx < queueSize; idx++ )
    {
      SoAd_PduIdxType txPduIdx = SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].TxPduIdx;

      if ( txPduIdx != SOAD_INV_PDU_IDX )
      {
        /* handle TxConfirmation */
        if ( SoAd_TxPduIdx2PduRoute(txPduIdx).TxConfEnabled == TRUE )
        { /* TxConfirmation is configured for the corresponding upper layer and PDU */
          SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if ( 0u < (SoAd_TxPduIdx2PendingTxConf(txPduIdx) & SOAD_TX_CONF_STRAT_BIT_PAT_CONF) )
          {
            /* there is a pending TxConfirmation for one of the PduRouteDests */
            SoAd_TxPduIdx2PendingTxConf(txPduIdx)--;
            if ( 0u == SoAd_TxPduIdx2PendingTxConf(txPduIdx) )
            { /* call TxConfirmation of upper layer only if all PduRouteDests are confirmed (PDU fan-out) */
              /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
              SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              /* call TxConfirmation of upper layer */
              SoAd_VCallIfTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx));
            }
            else
            {
              /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
              SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
          }
          else
          {
            /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
            SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          }
        }
      }

#  if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
      SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].BufIdx = 0u;
#  endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
      SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].Copied = FALSE;
      SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].Len = 0u;
      SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].TxPduIdx = SOAD_INV_PDU_IDX;
    }
  }
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
  if ( SoAd_NPduUdpTx(NPduUdpTxIdx).BufMgtPtr != NULL_PTR )
  {
    SoAd_NPduUdpTx(NPduUdpTxIdx).BufMgtPtr->Len = 0u;
  }
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_NPduQueueAddElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_NPduQueueAddElement(
  SoAd_PduIdxType TxPduIdx,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  SoAd_NPduReturnType nPduRetVal = SOAD_NPDU_OK;
  SoAd_SoConIdxType soConIdx = PduRouteDestPtr->SoConIdx;
  PduLengthType dataLen = PduInfoPtr->SduLength;
  uint16 nPduUdpTxIdx = SoAd_SockConIdx2NPduUdpTxIdx(soConIdx);
  boolean elementUpdated = FALSE;

  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
  {
    dataLen += SOAD_PDU_HDR_SIZE;
  }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

  /* check if PDU is already in queue */
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
  if ( SoAd_TxPduIdx2PduRoute(TxPduIdx).IfTriggerTransmitEnabled == TRUE )
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
  { /* data will be retrieved via trigger transmit */
    uint8 idx;

    for ( idx = 0u; idx < SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr->Lvl; idx++ )
    {
      if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].TxPduIdx == TxPduIdx )
      { /* PDU is already in queue */
        if ( dataLen != SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len )
        { /* different length */
          /* update */
          SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen -= SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len;
          SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len = dataLen;
          SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen += dataLen;

          /* check size after update */
          if ( SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen >
            SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin )
          { /* maximum NPdu size exceeded */
            nPduRetVal = SOAD_NPDU_OVFL_UPDATE;
          }
          elementUpdated = TRUE;
        }
        break;
      }
    }
  }

  if ( elementUpdated == FALSE )
  { /* new PDU */
    if ( SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TransmissionActive == FALSE )
    { /* nPdu transmission is not ongoing */
      /* check queue size */
      if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr->Lvl <
        SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxQueueSize )
      {
        /* check size */
        if ( (SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen + dataLen) <=
          SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin )
        { /* NPdu size is sufficient for new request */
          Std_ReturnType retVal = E_OK;

          /* check and restart UDP trigger timer if necessary */
          if ( PduRouteDestPtr->UdpTriggerTimeout > 0u  )
          { /* PduRouteDest specific timeout is configured */
            uint32 currentTimeoutCnt = 0u;

            if ( SoAd_TimeoutList_GetCurrentTimeout(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx, &currentTimeoutCnt,
              SoAd_GlobalCounter) == E_OK )
            { /* timer is already in list */
              /* check timer value */
              if ( PduRouteDestPtr->UdpTriggerTimeout < currentTimeoutCnt )
              { /* current timer value is bigger than configured value */
                /* update timeout */
                (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
                  PduRouteDestPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
              }
            }
            else
            { /* timer is not set */
              retVal = SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
                PduRouteDestPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
            }
          }
          else if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout > 0u )
          { /* socket connection specific timeout is configured */
            uint32 currentTimeoutCnt = 0u;

            if ( SoAd_TimeoutList_GetCurrentTimeout(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx, &currentTimeoutCnt,
              SoAd_GlobalCounter) == E_OK )
            { /* timer is already in list */
              /* check timer value */
              if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout < currentTimeoutCnt )
              { /* current timer value is bigger than configured value */
                /* update timeout */
                (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
                  SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
              }
            }
            else
            { /* timer is not set */
              retVal = SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
                SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
            }
          }
          else
          { /* no timeout is configured */
            /* nothing to do */
          }

          if ( retVal == E_OK )
          {
            uint8 lvl = SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr->Lvl;

            /* store request */
            SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[lvl].Copied = FALSE;
            SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[lvl].Len = dataLen;
            SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[lvl].TxPduIdx = TxPduIdx;
            SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr->Lvl++;

# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
            /* store data in buffer if TriggerTransmit is not enabled */
            if ( SoAd_TxPduIdx2PduRoute(TxPduIdx).IfTriggerTransmitEnabled == FALSE )
            { /* store in buffer */
              SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[lvl].BufIdx = SoAd_NPduUdpTx(nPduUdpTxIdx).BufMgtPtr->Len;

              (void)SoAd_NPduBufAddElement(TxPduIdx, PduRouteDestPtr, PduInfoPtr);
            }
            else
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
            {
              /* handle TxConfirmation */
              SoAd_TxIf_AddPendingConf(TxPduIdx, soConIdx);

              /* increment entire length */
              SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen += dataLen;
            }
          }
          else
          {
            nPduRetVal = SOAD_NPDU_NOT_OK;
          }
        }
        else
        {
          nPduRetVal = SOAD_NPDU_OVFL_NEW;
        }
      }
      else
      {
        nPduRetVal = SOAD_NPDU_OVFL_NEW;
      }
    }
    else
    {
      nPduRetVal = SOAD_NPDU_OVFL_NEW;
    }
  }

  /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return nPduRetVal;
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_NPduQueueSend()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduQueueSend(
  uint16 NPduUdpTxIdx)
{
  SoAd_SoConIdxType soConIdx = SoAd_NPduUdpTx(NPduUdpTxIdx).SoConIdx;
  SoAd_SockIdxType sockIdx = SoAd_SockCons(soConIdx).SockIdx;
  uint8 retryCnt;

  for ( retryCnt = 0u; retryCnt < SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT; retryCnt++ )
  { /* loop since PDUs may be larger than mentioned in transmission request and need to be splitted */
    SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ( SOAD_INV_SO_CON_IDX == SoAd_SockIdx2ActiveSoConIdx(sockIdx) )
    {
      /* set nPdu to transmission state and handle retry counter */
      if ( SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive == TRUE )
      { /* transmission already active */
        if ( SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->RetryCnt > 0u )
        {
          SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->RetryCnt--;
        }
        if ( SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->RetryCnt == 0u )
        { /* retry limit reached */
          /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
          SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          /* clear queue to allow new transmissions */
          SoAd_NPduClear(NPduUdpTxIdx);
          break;
        }
      }
      else
      { /* transmission not yet active */
        SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive = TRUE;
        SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->RetryCnt = SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN;
      }

      /* set local socket index to active socket connection mapping */
      SoAd_SockIdx2ActiveSoConIdx(sockIdx) = soConIdx;
      /* set tx management struct for CopyTxData */
      SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx = 0u; /* set dummy PduId */
      if ( SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen >
        SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin )
      {
        SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending =
          (PduLengthType)SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin;
        SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen =
          (PduLengthType)SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin;
      }
      else
      {
        SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending = (PduLengthType)SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen;
        SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen = (PduLengthType)SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen;
      }
      SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr = NULL_PTR;
      /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* try to send nPdu */
      if ( SoAd_TcpIp_UdpTransmit(sockIdx, SoAd_SockIdx2SocketId(sockIdx),
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(soConIdx),
        (uint16)SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending) == E_OK )
      { /* transmission successful */
        uint8 idx;

        /* release sent queue element */
        for ( idx = 0u; idx < SoAd_NPduUdpTx(NPduUdpTxIdx).QueueMgtPtr->Lvl; idx++ )
        {
          SoAd_PduIdxType txPduIdx = SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].TxPduIdx;

          if ( txPduIdx != SOAD_INV_PDU_IDX )
          {
            if ( SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].Copied == TRUE )
            {
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
              if ( SoAd_TxPduIdx2PduRoute(txPduIdx).IfTriggerTransmitEnabled == TRUE )
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
              {
                /* handle TxConfirmation */
                if ( SoAd_TxPduIdx2PduRoute(txPduIdx).TxConfEnabled == TRUE )
                { /* TxConfirmation is configured for the corresponding upper layer and PDU */
                  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                  if ( 0u < (SoAd_TxPduIdx2PendingTxConf(txPduIdx) & SOAD_TX_CONF_STRAT_BIT_PAT_CONF) )
                  {
                    /* there is a pending TxConfirmation for one of the PduRouteDests */
                    SoAd_TxPduIdx2PendingTxConf(txPduIdx)--;
                    if ( 0u == SoAd_TxPduIdx2PendingTxConf(txPduIdx) )
                    { /* call TxConfirmation of upper layer only if all PduRouteDests are confirmed (PDU fan-out) */
                      /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                      /* socket connection handling if automatic setup is enabled and wildcards configured */
                      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
                      {
                        if ( (SOAD_SOCON_IP_ANY_PORT_ANY >= SoAd_SockConIdx2RemAddrState(soConIdx)) &&
                          (0u != (SoAd_SockCons(soConIdx).RemAddrState & SOAD_SOCON_MASK_ANY)) )
                        {
                          SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_RECONNECT;
                          /* set event to handle socket connection state in main function */
                          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                        }
                      }
                      /* call TxConfirmation of upper layer */
                      SoAd_VCallIfTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                        SoAd_TxPduIdx2TxConfPduId(txPduIdx));
                    }
                    else
                    {
                      /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                    }
                  }
                  else
                  {
                    /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                  }
                }
              }

              /* release queue element */
              SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen -= SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].Len;
              SoAd_NPduUdpTx(NPduUdpTxIdx).QueuePtr[idx].TxPduIdx = SOAD_INV_PDU_IDX;
            }
            else
            { /* first PDU which could not be copied anymore */
              break;
            }
          }
        }

        /* reset tx management struct */
        SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */

        /* reset local socket index to active socket connection mapping */
        SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;

        if ( SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen == 0u )
        { /* nPduQueue sent completely */
          SoAd_NPduUdpTx(NPduUdpTxIdx).QueueMgtPtr->Lvl = 0u;
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
          if ( SoAd_NPduUdpTx(NPduUdpTxIdx).BufMgtPtr != NULL_PTR )
          {
            SoAd_NPduUdpTx(NPduUdpTxIdx).BufMgtPtr->Len = 0u;
          }
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
          /* remove timeout from list */
          SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxNPduUdpTx(), NPduUdpTxIdx);
          /* transmission of nPdu finished */
          SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive = FALSE;
          break;
        }
      }
      else
      { /* transmission failed */
        /* reset tx management struct */
        SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */

        /* reset local socket index to active socket connection mapping */
        SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;

        /* set timeout to retry in next main function */
        (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), NPduUdpTxIdx, 1u, SoAd_GlobalCounter);

        /* break here since retry in this context shall only be done if nPdu is larger than provided TcpIp buffer */
        break;
      }
    }
    else
    {
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
      break;
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */

#if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_NPduBufAddElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_NPduBufAddElement(
  SoAd_PduIdxType TxPduIdx,
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) PduRouteDestPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  SoAd_NPduReturnType nPduRetVal = SOAD_NPDU_NOT_OK;
  SoAd_SoConIdxType soConIdx = PduRouteDestPtr->SoConIdx;
  PduLengthType dataLen = PduInfoPtr->SduLength;
  uint16 nPduUdpTxIdx = SoAd_SockConIdx2NPduUdpTxIdx(soConIdx);

  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TransmissionActive == FALSE )
  { /* nPdu transmission is not ongoing */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
    {
      dataLen += SOAD_PDU_HDR_SIZE;
    }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

    if ( (SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen + dataLen) <=
      SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->NPduUdpTxBufferMin )
    { /* nPdu size is sufficient for new request */
      Std_ReturnType retVal = E_OK;

      /* check and restart UDP trigger timer if necessary */
      if ( PduRouteDestPtr->UdpTriggerTimeout > 0u  )
      { /* PduRouteDest specific timeout is configured */
        uint32 currentTimeoutCnt = 0u;

        if ( SoAd_TimeoutList_GetCurrentTimeout(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx, &currentTimeoutCnt,
          SoAd_GlobalCounter) == E_OK )
        { /* timer is already in list */
          /* check timer value */
          if ( PduRouteDestPtr->UdpTriggerTimeout < currentTimeoutCnt )
          { /* current timer value is bigger than configured value */
            /* update timeout */
            (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
              PduRouteDestPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
          }
        }
        else
        { /* timer is not set */
          retVal = SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
            PduRouteDestPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
        }
      }
      else if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout > 0u )
      { /* socket connection specific timeout is configured */
        uint32 currentTimeoutCnt = 0u;

        if ( SoAd_TimeoutList_GetCurrentTimeout(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx, &currentTimeoutCnt,
          SoAd_GlobalCounter) == E_OK )
        { /* timer is already in list */
          /* check timer value */
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout < currentTimeoutCnt )
          { /* current timer value is bigger than configured value */
            /* update timeout */
            (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
              SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
          }
        }
        else
        { /* timer is not set */
          retVal = SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxNPduUdpTx(), nPduUdpTxIdx,
            SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpTriggerTimeout, SoAd_GlobalCounter);
        }
      }
      else
      { /* no timeout is configured */
        /* nothing to do */
      }

      if ( retVal == E_OK )
      {
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
        { /* PDU Header enabled - copy first */
          SoAd_CopyPduHdr2Buf(PduRouteDestPtr->TxPduHdrId, PduInfoPtr->SduLength,
            &SoAd_NPduUdpTx(nPduUdpTxIdx).BufPtr[SoAd_NPduUdpTx(nPduUdpTxIdx).BufMgtPtr->Len], 0u, SOAD_PDU_HDR_SIZE);
          SoAd_NPduUdpTx(nPduUdpTxIdx).BufMgtPtr->Len += SOAD_PDU_HDR_SIZE;
        }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
        /* copy PDU */
        IpBase_Copy((P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))
            &SoAd_NPduUdpTx(nPduUdpTxIdx).BufPtr[SoAd_NPduUdpTx(nPduUdpTxIdx).BufMgtPtr->Len],
          (P2VAR(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&PduInfoPtr->SduDataPtr[0],
          PduInfoPtr->SduLength);
        SoAd_NPduUdpTx(nPduUdpTxIdx).BufMgtPtr->Len += (uint16)PduInfoPtr->SduLength;

        /* handle TxConfirmation */
        SoAd_TxIf_AddPendingConf(TxPduIdx, soConIdx);

        /* increment entire length */
        SoAd_NPduUdpTx(nPduUdpTxIdx).MgtPtr->TotalLen += dataLen;

        nPduRetVal = SOAD_NPDU_OK;
      }
    }
    else
    {
      nPduRetVal = SOAD_NPDU_OVFL_NEW;
    }
  }

  SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

  return nPduRetVal;
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_NPduBufSend()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_NPduBufSend(
  uint16 NPduUdpTxIdx)
{
  SoAd_SoConIdxType soConIdx = SoAd_NPduUdpTx(NPduUdpTxIdx).SoConIdx;
  SoAd_SockIdxType sockIdx = SoAd_SockCons(soConIdx).SockIdx;

  SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if ( SOAD_INV_SO_CON_IDX == SoAd_SockIdx2ActiveSoConIdx(sockIdx) )
  {
    /* set nPdu to transmission state (may be already active) */
    SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive = TRUE;

    /* set local socket index to active socket connection mapping */
    SoAd_SockIdx2ActiveSoConIdx(sockIdx) = soConIdx;
    /* set tx management struct for CopyTxData */
    SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx = 0u; /* set dummy PduId */
    SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending = (PduLengthType)SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen;
    SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen = (PduLengthType)SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen;
    SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr = NULL_PTR;
    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */

    /* try to send nPdu */
    if ( E_OK == SoAd_TcpIp_UdpTransmit(sockIdx, SoAd_SockIdx2SocketId(sockIdx),
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(soConIdx),
        (uint16)SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen) )
    { /* transmission successful */
      SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TotalLen = 0u;
      SoAd_NPduUdpTx(NPduUdpTxIdx).BufMgtPtr->Len = 0u;
      /* remove timeout from list */
      SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxNPduUdpTx(), NPduUdpTxIdx);
      /* transmission of nPdu finished */
      SoAd_NPduUdpTx(NPduUdpTxIdx).MgtPtr->TransmissionActive = FALSE;
    }

    /* reset tx management struct */
    SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */

    /* reset local socket index to active socket connection mapping */
    SoAd_SockIdx2ActiveSoConIdx(sockIdx) = SOAD_INV_SO_CON_IDX;
  }
  else
  {
    SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
  }
}
#endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
/**********************************************************************************************************************
 *  SoAd_HandleTxConfirmation()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_HandleTxConfirmation(void)
{
  uint16 handleIdx = SOAD_INV_PDU_IDX;
  SoAd_PduIdxType lvl = (SoAd_PduIdxType)SoAd_EventQueues(SoAd_EventQueueIdxIfUdpPduRoute()).EveMgtPtr->Lvl;
  SoAd_PduIdxType idx;

# if ( SOAD_VEVE_QUE_LIMIT_UDPPDUROUTE_ENALBED == STD_ON )
  if ( lvl > SoAd_EventQueues(SoAd_EventQueueIdxIfUdpPduRoute()).EveQueLimit )
  {
    lvl = (SoAd_PduIdxType)SoAd_EventQueues(SoAd_EventQueueIdxIfUdpPduRoute()).EveQueLimit;
  }
# endif /* SOAD_VEVE_QUE_LIMIT_UDPPDUROUTE_ENALBED == STD_ON */

  for ( idx = 0u; idx < lvl; idx++ )
  {
    if ( SoAd_EventQueue_GetNextElement(SoAd_EventQueueIdxIfUdpPduRoute(), &handleIdx) == E_OK )
    {
      SoAd_PduIdxType txPduIdx = (SoAd_PduIdxType)handleIdx;

      if ( 0u != (SoAd_TxPduIdx2PendingTxConf(txPduIdx) & SOAD_TX_CONF_STRAT_BIT_PAT_MAIN) )
      { /* TxConfirmation pending */
        /* IF-API only (TP TxConfirmation is handled directly after transmission or in SoAd_TxConfirmation context) */
        P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_VAR) pduRouteDestPtr =
          SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[0];
        uint16 pduRouteDestNum = SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListSize;

        /* handle multiple PduRouteDests (PDU fan-out) */
        while ( 0u < pduRouteDestNum )
        {
          SoAd_SoConIdxType soConIdx = pduRouteDestPtr->SoConIdx;

          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
          { /* UDP socket */
# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
            if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConf == FALSE )
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
            { /* TxConfirmation is handled in MainFunction context */
              SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              /* reset MainFunction bit */
              SoAd_TxPduIdx2PendingTxConf(txPduIdx) &= SOAD_TX_CONF_STRAT_BIT_PAT_CONF;
              if ( 0u == SoAd_TxPduIdx2PendingTxConf(txPduIdx) )
              { /* no TxConfirmation in SoAd_TxConfirmation context pending */
                /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                /* socket connection handling if automatic setup is enabled and wildcards configured */
                if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SockAutoSoConSetup == TRUE )
                {
                  if ( (SOAD_SOCON_IP_ANY_PORT_ANY >= SoAd_SockConIdx2RemAddrState(soConIdx)) &&
                    (0u != (SoAd_SockCons(soConIdx).RemAddrState & SOAD_SOCON_MASK_ANY)) )
                  {
                    /* reset remote socket and reconnect socket connection */
                    SoAd_ResetIpAddr(soConIdx);
# if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
                    if ( NULL_PTR != SoAd_SockCons(soConIdx).RcvRemSockPtr )
                    { /* get received remote address is supported */
                      /* reset remote address */
                      SoAd_ResetIpAddr2Unspec(SoAd_SockCons(soConIdx).RcvRemSockPtr);
                    }
# endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */
                    SoAd_SockConIdx2State(soConIdx) = SOAD_SOCON_RECONNECT;
                    /* inform upper layer about socket connection state change */
                    SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).SoConChgFuncBitPattern,
                      SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdx));
                  }
                }

                /* call TxConfirmation */
                if ( TRUE == SoAd_TxPduIdx2PduRoute(txPduIdx).TxConfEnabled )
                { /* TxConfirmation is configured for the corresponding upper layer or PDU */
                  SoAd_VCallIfTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                    SoAd_TxPduIdx2TxConfPduId(txPduIdx));
                }

                /* no pending TxConfirmation anymore - skip this PduRouteDest collection */
                break;
              }
              else
              { /* TxConfirmation in SoAd_TxConfirmation context pending */
                /* PRQA S 3355, 3358 1 */ /* MD_SoAd_13.7_SEC */
                SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                /* nothing to do */
              }
            }
          }
          else
          { /* TCP socket */
            /* TxConfirmation is always handled in SoAd_TxConfirmation context for IF-TCP connections */
          }
          pduRouteDestNum--;
          pduRouteDestPtr++; /* PRQA S 0489 */ /* MD_SoAd_17.4 */
        }
      }
    }
    else
    { /* no element left */
      break;
    }
  }
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VPDU_ROUTES == STD_ON */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_WriteBufSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3689 SoAd_21_1 */ /* MD_SoAd_21.1 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteBufSegment(
                        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcBufPtr,
                        uint16 SrcBufLen,
                        P2VAR(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) TgtBufSegPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  if ( 0u != SrcBufLen )
  { /* new data received */
    /* check if buffer struct is already used */
    if ( 0u == TgtBufSegPtr->Lvl )
    { /* buffer struct unused */
      TgtBufSegPtr->BufSegs[TgtBufSegPtr->Idx].Len = SrcBufLen;
      TgtBufSegPtr->BufSegs[TgtBufSegPtr->Idx].DataPtr = SrcBufPtr;
      TgtBufSegPtr->Lvl++;
      retVal = E_OK;
    }
    else
    { /* buffer struct contains elements (at least one segment received before) */
      uint16 lastIdx;
      uint16 writeIdx;
      uint16 curLvl = (uint16)(TgtBufSegPtr->Lvl - 1u);

      /* get current index */
      if ( (TgtBufSegPtr->Idx + curLvl) >= SOAD_VBUF_SEG_SIZE_TOTAL )
      { /* wrap around necessary */
        lastIdx = curLvl - (SOAD_VBUF_SEG_SIZE_TOTAL - TgtBufSegPtr->Idx);
      }
      else
      { /* no wrap around necessary */
        lastIdx = TgtBufSegPtr->Idx + curLvl;
      }

      /* check if new data are available */
      if ( &TgtBufSegPtr->BufSegs[lastIdx].DataPtr[0] != &SrcBufPtr[0] )
      { /* new data shall be added */
        /* check if new segment is in same memory segment like last segment */
        if ( &TgtBufSegPtr->BufSegs[lastIdx].DataPtr[TgtBufSegPtr->BufSegs[lastIdx].Len] == &SrcBufPtr[0] )
        { /* new segment is in same memory segment */
          /* increment length only */
          TgtBufSegPtr->BufSegs[lastIdx].Len += SrcBufLen;

          retVal = E_OK;
        }
        else
        { /* new segment available */
          if ( SOAD_VBUF_SEG_SIZE_TOTAL > TgtBufSegPtr->Lvl )
          { /* segmented buffer struct size sufficient */
            /* find next free segment */
            if ( (TgtBufSegPtr->Idx + TgtBufSegPtr->Lvl) >= SOAD_VBUF_SEG_SIZE_TOTAL )
            { /* wrap around necessary */
              writeIdx = TgtBufSegPtr->Lvl - (SOAD_VBUF_SEG_SIZE_TOTAL - TgtBufSegPtr->Idx);
            }
            else
            { /* no wrap around necessary */
              writeIdx = TgtBufSegPtr->Idx + TgtBufSegPtr->Lvl;
            }

            TgtBufSegPtr->BufSegs[writeIdx].Len = SrcBufLen;
            TgtBufSegPtr->BufSegs[writeIdx].DataPtr = SrcBufPtr;
            TgtBufSegPtr->Lvl++;

            retVal = E_OK;
          }
        }
      }
      else
      {
        retVal = E_OK;
      }
    }
  }
  else
  { /* no new data received */
    retVal = E_OK;
  }

  return retVal;
}
/* PRQA L:MSR_STMIF */
/* PRQA L:SoAd_21_1 */

/**********************************************************************************************************************
 *  SoAd_GetBufSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_GetBufSegment(
  P2CONST(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) BufSegPtr,
  uint32 StartPos,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) SegPtr,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) SegLen)
{
  uint16 bufSegIdx = BufSegPtr->Idx;
  uint16 bufSegLvl = BufSegPtr->Lvl;

  *SegLen = 0u;
  *SegPtr = &BufSegPtr->BufSegs[BufSegPtr->Idx].DataPtr[0];

  while ( 0u < bufSegLvl )
  {
    if ( BufSegPtr->BufSegs[bufSegIdx].Len <= StartPos )
    { /* start position in next buffer segment */
      StartPos -= BufSegPtr->BufSegs[bufSegIdx].Len;
      if ( (bufSegIdx + 1u) == SOAD_VBUF_SEG_SIZE_TOTAL )
      { /* wrap around */
        bufSegIdx = 0u;
      }
      else
      { /* no wrap around */
        bufSegIdx++;
      }
    }
    else
    { /* start position in current segment */
      /* set length */
      *SegLen = (uint32)(BufSegPtr->BufSegs[bufSegIdx].Len - StartPos);
      /* set data pointer */
      *SegPtr = &BufSegPtr->BufSegs[bufSegIdx].DataPtr[StartPos];
      break;
    }
    bufSegLvl--;
  }
}

#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_GetBufSegmentTotLen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, SOAD_CODE) SoAd_GetBufSegmentTotLen(
  P2CONST(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) BufSegPtr,
  uint32 StartPos)
{
  uint32 totSegLen = 0u;
  uint16 bufSegIdx = BufSegPtr->Idx;
  uint16 bufSegLvl = BufSegPtr->Lvl;

  while ( 0u < bufSegLvl )
  {
    if ( BufSegPtr->BufSegs[bufSegIdx].Len <= StartPos )
    { /* start position in next buffer segment */
      StartPos -= BufSegPtr->BufSegs[bufSegIdx].Len;
    }
    else
    { /* start position in current segment */
      /* set length */
      totSegLen += (uint32)(BufSegPtr->BufSegs[bufSegIdx].Len - StartPos);
    }
    if ( (bufSegIdx + 1u) == SOAD_VBUF_SEG_SIZE_TOTAL )
    { /* wrap around */
      bufSegIdx = 0u;
    }
    else
    { /* no wrap around */
      bufSegIdx++;
    }
    bufSegLvl--;
  }

  return totSegLen;
}
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_ReleaseBufSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_ReleaseBufSegment(
  P2VAR(SoAd_BufSegMgtType, AUTOMATIC, AUTOMATIC) BufSegPtr,
  uint32 Len)
{
  while ( (0u < Len) && (0u < BufSegPtr->Lvl) )
  {
    if ( Len < BufSegPtr->BufSegs[BufSegPtr->Idx].Len )
    { /* release buffer in current element only */
      BufSegPtr->BufSegs[BufSegPtr->Idx].Len -= (uint16)Len;
      BufSegPtr->BufSegs[BufSegPtr->Idx].DataPtr = &BufSegPtr->BufSegs[BufSegPtr->Idx].DataPtr[Len];
      Len = 0u;
    }
    else
    { /* release complete element */
      Len -= BufSegPtr->BufSegs[BufSegPtr->Idx].Len;
      BufSegPtr->BufSegs[BufSegPtr->Idx].Len = 0u;

      if ( (BufSegPtr->Idx + 1u) == SOAD_VBUF_SEG_SIZE_TOTAL )
      { /* wrap around necessary */
        BufSegPtr->Idx = 0u;
      }
      else
      { /* no wrap around necessary */
        BufSegPtr->Idx++;
      }
      BufSegPtr->Lvl--;
    }
  }
}

# endif /* SOAD_VTCP_USED == STD_ON */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_CheckAnyRoutGrpOnPduRouteDestEnabled(
  P2CONST(SoAd_PduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDest)
{
  SoAd_RoutingGroupIdType grpIdx;
  boolean retVal = FALSE;

  if ( PduRouteDest->RoutGrpNum > 0u )
  { /* at least one routing group is configured */
    for ( grpIdx = 0u; grpIdx < PduRouteDest->RoutGrpNum; grpIdx++ )
    {
      if ( 0u != (*PduRouteDest->RoutGrpStateColPtr[grpIdx] & SOAD_ROUT_GRP_BIT_PATTERN_ENABLED) )
      { /* at least one routing group is enabled - forward PDU */
        retVal = TRUE;
        break;
      }
    }
  }
  else
  { /* no routing group configured - forward PDU */
    retVal = TRUE;
  }
  return retVal;
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STMIF */
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(
  uint16 SockRouteIdx)
{
  boolean retVal = FALSE;
  SoAd_RoutingGroupIdType grpIdNum = SoAd_SockRoutes(SockRouteIdx).SockRouteDest.RoutGrpIdNum;

  if ( 0u < grpIdNum )
  { /* at least one routing group is configured */
    while ( 0u < grpIdNum )
    {
      SoAd_RoutingGroupIdType grpId;
      SoAd_RoutingGroupIdxType grpIdx;
      uint16 sockRouteNum = 0;

      grpIdNum--;
      grpId = SoAd_SockRoutes(SockRouteIdx).SockRouteDest.RoutGrpIdColPtr[grpIdNum];
      grpIdx = SoAd_RouteGrpId2RouteGrpIdx(grpId);

      if ( SOAD_INV_ROUT_GRP_IDX != grpIdx )
      {
        sockRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(grpIdx).IdNum;
      }

      /* find routing group state for socket route */
      while ( 0u < sockRouteNum )
      {
        uint16 sockRouteIdx;

        sockRouteNum--;
        sockRouteIdx = SoAd_RouteGrpIdx2SocketRouteCol(grpIdx).IdColPtr[sockRouteNum];

        if ( sockRouteIdx == SockRouteIdx )
        { /* socket route found */
          if ( 0u != (*SoAd_RouteGrpIdx2SocketRouteCol(grpIdx).StateColPtr[sockRouteNum] &
            SOAD_ROUT_GRP_BIT_PATTERN_ENABLED) )
          { /* at least one routing group is enabled - forward PDU */
            retVal = TRUE;
            break;
          }
        }
      }
    }
  }
  else
  { /* no routing group configured - forward PDU */
    retVal = TRUE;
  }
  return retVal;
}
/* PRQA L:MSR_STMIF */
# endif /*  SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_CheckForWildcards()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_CheckForWildcards(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddrCheck,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IpAddrIsAny,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) PortIsAny)
{
  SoAd_DomainType tmpDomain;

  /* check AddrCheck */
  if ( NULL_PTR != AddrCheck )
  {
    tmpDomain = AddrCheck->domain;

    if ( SOAD_AF_UNSPEC == tmpDomain )
    { /* domain is not set */
#if ( SOAD_VIPV6_ENABLED == STD_ON )
      /* assume IPv6 */
      tmpDomain = SOAD_AF_INET6;
#else
      /* assume IPv4 */
      tmpDomain = SOAD_AF_INET;
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
    }

    if ( SOAD_AF_INET == tmpDomain )
    { /* IPv4 */
      if ( SOAD_IPADDR_ANY == ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[0] )
      {
        *IpAddrIsAny = TRUE;
      }
      else
      {
        *IpAddrIsAny = FALSE;
      }

      if ( SOAD_PORT_ANY == ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))AddrCheck)->port )
      {
        *PortIsAny = TRUE;
      }
      else
      {
        *PortIsAny = FALSE;
      }
    }
#if ( SOAD_VIPV6_ENABLED == STD_ON )
    else if ( SOAD_AF_INET6 == tmpDomain )
    { /* IPv6 */
      if ( (SOAD_IP6ADDR_ANY == ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[0]) &&
        (SOAD_IP6ADDR_ANY == ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[1]) &&
        (SOAD_IP6ADDR_ANY == ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[2]) &&
        (SOAD_IP6ADDR_ANY == ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[3]) )
      {
        *IpAddrIsAny = TRUE;
      }
      else
      {
        *IpAddrIsAny = FALSE;
      }

      if ( SOAD_PORT_ANY == ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->port )
      {
        *PortIsAny = TRUE;
      }
      else
      {
        *PortIsAny = FALSE;
      }
    }
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
    else
    { /* unknown IP version */
      /* ignore */
    }
  }
  else
  { /* no remote address is configured */
    *IpAddrIsAny = TRUE;
    *PortIsAny   = TRUE;
  }
}

/**********************************************************************************************************************
 *  SoAd_GetSoConIdByBestMatchAlg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6060 MSR_STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConIdByBestMatchAlg(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DynAddr,
  SoAd_SoConIdxType SoConIdxStart,
  SoAd_SoConIdxType SoConIdxMax,
  SoAd_SoConModeFilterType SoConModeFilter,
  boolean NoSockRouteFilter,
  P2VAR(SoAd_SoConIdxType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxBestMatch)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 bestMatchPrio = 0u;
  SoAd_SoConIdxType soConIdx = SoConIdxStart;

  while ( soConIdx <= SoConIdxMax )
  {
    SoAd_DomainType tmpDomain;
    uint8 bestMatchPrioTmp = 0u;

    if ( SoConModeFilter == SOAD_SOCON_FILTER_ON_OFF )
    { /* filter socket connection if state is SOAD_SOCON_ONLINE or SOAD_SOCON_OFFLINE */
      if ( SoAd_SockConIdx2State(soConIdx) != SOAD_SOCON_RECONNECT )
      { /* socket connection state is not SOAD_SOCON_RECONNECT */
        /* skip socket connection */
        soConIdx++;
        continue; /* PRQA S 0770 */ /* MD_SoAd_14.5 */
      }
    }

    if ( NoSockRouteFilter == TRUE )
    { /* filter socket connection if no socket route is configured */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
      if ( (SoAd_IsSockRoutePresent()) && (SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxNum == 0) )
      { /* no socket route is configured */
        /* skip socket connection */
        soConIdx++;
        continue; /* PRQA S 0770 */ /* MD_SoAd_14.5 */
      }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
    }

    if ( (SoAd_SockConIdx2RemAddrState(soConIdx) & SOAD_SOCON_MASK_NOT) > 0u )
    { /* remote address is NOT_SET */
      /* skip socket connection */
      soConIdx++;
      continue; /* PRQA S 0770 */ /* MD_SoAd_14.5 */
    }

    tmpDomain = SoAd_SockConIdx2RemSockDyn(soConIdx).domain;

    if ( tmpDomain == 0u )
    { /* domain is not set - assume domain is equal to DynAddr */
      tmpDomain = DynAddr->domain;
    }

    if ( tmpDomain == DynAddr->domain )
    {
      if ( DynAddr->domain == SOAD_AF_INET )
      { /* IPv4 */
        uint32 dynAddr = ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))DynAddr)->addr[0];
        uint16 dynPort = ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))DynAddr)->port;

        if ( (SoAd_SockConIdx2RemSockDyn(soConIdx).addr[0] == dynAddr) &&
          (SoAd_SockConIdx2RemSockDyn(soConIdx).port == dynPort) )
        { /* IP and port match */
          bestMatchPrioTmp = 4u;
        }
        else if ( (SoAd_SockConIdx2RemSockDyn(soConIdx).addr[0] == dynAddr) &&
          (SoAd_SockConIdx2RemSockDyn(soConIdx).port == SOAD_PORT_ANY) )
        { /* IP match and port is wildcard */
          bestMatchPrioTmp = 3u;
        }
        else if ( (SoAd_SockConIdx2RemSockDyn(soConIdx).addr[0] == SOAD_IPADDR_ANY) &&
          (SoAd_SockConIdx2RemSockDyn(soConIdx).port == dynPort) )
        { /* IP is wildcard and port match */
          bestMatchPrioTmp = 2u;
        }
        else if ( (SoAd_SockConIdx2RemSockDyn(soConIdx).addr[0] == SOAD_IPADDR_ANY) &&
          (SoAd_SockConIdx2RemSockDyn(soConIdx).port == SOAD_PORT_ANY))
        { /* IP and port are wildcards */
          bestMatchPrioTmp = 1u;
        }
        else
        { /* no match */
          /* nothing to do */
        }
      }
#if ( SOAD_VIPV6_ENABLED == STD_ON )
      else
      { /* IPv6 */
        uint8 addrIdx;
        boolean ipIsWildcard = TRUE;
        boolean ipIsEqual = TRUE;
        uint16 dynPort = ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))DynAddr)->port;

        /* check wether IP address is equal */
        for ( addrIdx = 0u; addrIdx < 4u; addrIdx++ )
        {
          if ( SoAd_SockConIdx2RemSockDyn(soConIdx).addr[addrIdx] !=
            ((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))DynAddr)->addr[addrIdx] )
          { /* not equal */
            ipIsEqual = FALSE;
            break;
          }
        }

        /* check wether IP address is wildcard */
        for ( addrIdx = 0u; addrIdx < 4u; addrIdx++ )
        {
          if ( SoAd_SockConIdx2RemSockDyn(soConIdx).addr[addrIdx] != SOAD_IP6ADDR_ANY )
          { /* no wildcard */
            ipIsWildcard = FALSE;
            break;
          }
        }

        if ( (ipIsEqual == TRUE) && (SoAd_SockConIdx2RemSockDyn(soConIdx).port == dynPort) )
        { /* IP and port match */
          bestMatchPrioTmp = 4u;
        }
        else if ( (ipIsEqual == TRUE) && (SoAd_SockConIdx2RemSockDyn(soConIdx).port == SOAD_PORT_ANY) )
        { /* IP match and port is wildcard */
          bestMatchPrioTmp = 3u;
        }
        else if ( (ipIsWildcard == TRUE) && (SoAd_SockConIdx2RemSockDyn(soConIdx).port == dynPort) )
        { /* IP is wildcard and port match */
          bestMatchPrioTmp = 2u;
        }
        else if ( (ipIsWildcard == TRUE) && (SoAd_SockConIdx2RemSockDyn(soConIdx).port == SOAD_PORT_ANY) )
        { /* IP and port are wildcards */
          bestMatchPrioTmp = 1u;
        }
        else
        { /* no match */
          /* nothing to do */
        }
      }
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
    }
    if ( bestMatchPrioTmp > bestMatchPrio )
    {
      bestMatchPrio = bestMatchPrioTmp;
      *SoConIdxBestMatch = soConIdx;
      if ( bestMatchPrio == 4u )
      { /* best match found */
        break;
      }
    }
    soConIdx++;
  }

  if ( bestMatchPrio > 0u )
  {
    retVal = E_OK;
  }

  return retVal;
}
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STPAR */
/* PRQA L:MSR_STMIF */

#if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_VerifyRxPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(SoAd_ReturnType, SOAD_CODE) SoAd_VerifyRxPdu(
  SoAd_SoConIdxType SoConIdx,
  uint32 BufIdx,
  uint32 BufLen,
  SoAd_PduHdrIdType PduHdrId,
  SoAd_PduHdrLenType PduHdrLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType  localSockAddr = { 0u };
  SoAd_SockAddrInetXType  remSockAddr = { 0u };
# if ( SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u )
  uint32                  bufSegLen = 0u;
  PduInfoType             pduInfo;
# endif /* SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u  */
  SoAd_ReturnType         retVal = SOAD_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u )
  /* #10 Check if enough data is received to verify Rx PDU. */
  if ( (BufLen < SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN) && (BufLen < PduHdrLen) )
  {
    retVal = SOAD_E_PENDING;
  }
  else
# endif /* SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u */
  {
    /* #100 Retrieve local socket address. */
    SoAd_Util_GetLocalSockAddr(SoConIdx, &localSockAddr);

    /* #101 Retrieve remote socket address. */
    SoAd_Util_GetRemoteSockAddr(SoConIdx, &remSockAddr);

# if ( SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u )
    /* #102 Retrieve data. */
    if ( PduHdrLen > SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN )
    {
      pduInfo.SduLength = SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN;
    }
    else
    {
      pduInfo.SduLength = (PduLengthType)PduHdrLen;
    }
    SoAd_GetBufSegment(&SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, BufIdx, &pduInfo.SduDataPtr, &bufSegLen);
    if ( bufSegLen < pduInfo.SduLength )
    { /* PDU data part received segmented */
      /* copy to linearization buffer */
      SoAd_CopyBufSeg2Buf(SoAd_VerifyRxPduBuffer(), &SoAd_SockConIdx2RxMgt(SoConIdx).BufSegMgt, pduInfo.SduLength,
        BufIdx);
      pduInfo.SduDataPtr = SoAd_VerifyRxPduBuffer();
    }
    else
    { /* PDU data part received in one segment */
      /* nothing to do */
    }
# else
    SOAD_DUMMY_STATEMENT_VAR(BufIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(BufLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(PduHdrLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u */

    /* #103 Call verification callback. */
# if ( SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u )
    if ( SoAd_VCallVerifyRxPduFunc(
        (SOAD_P2VAR(SoAd_SockAddrType))&localSockAddr,
        (SOAD_P2VAR(SoAd_SockAddrType))&remSockAddr,
        PduHdrId,
        (SOAD_P2VAR(PduInfoType))&pduInfo) == E_OK )
# else
    if ( SoAd_VCallVerifyRxPduFunc(
        (SOAD_P2VAR(SoAd_SockAddrType))&localSockAddr,
        (SOAD_P2VAR(SoAd_SockAddrType))&remSockAddr,
        PduHdrId,
        NULL_PTR) == E_OK )
# endif /* SOAD_VVERIFY_RX_PDU_MAX_DATA_LEN > 0u */
    {
      retVal = SOAD_E_OK;
    }
  }

  return retVal;
}
#endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_GetSockIdxBySocketId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSockIdxBySocketId(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockIdxType, AUTOMATIC, SOAD_APPL_DATA) SockIdxPtr)
{
  /* PRQA S 0781 1 */ /* MD_SoAd_5.6 */
  SoAd_SockIdxType sockIdx = SoAd_TcpIpSocketCnt();
  Std_ReturnType retVal = E_NOT_OK;

  *SockIdxPtr = SOAD_INV_SOCK_IDX;

  while ( 0u < sockIdx )
  {
    sockIdx--;
    if ( SoAd_SockIdx2SocketId(sockIdx) == SocketId )
    {
      *SockIdxPtr = sockIdx;
      retVal = E_OK;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_ResetIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetIpAddr(
  SoAd_SoConIdxType SoConIdx)
{
  if ( NULL_PTR != SoAd_SockCons(SoConIdx).RemSockPtr )
  { /* reset to configured remote socket */
    SoAd_CopyIpAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx),
      SoAd_SockCons(SoConIdx).RemSockPtr, TRUE);
  }
  else
  { /* no remote socket is configured */
    SoAd_ResetIpAddr2Unspec((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(SoConIdx));
  }
  SoAd_SockConIdx2RemAddrState(SoConIdx) = SoAd_SockCons(SoConIdx).RemAddrState;
}

/**********************************************************************************************************************
 *  SoAd_ResetIpAddr2Unspec()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_ResetIpAddr2Unspec(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr)
{
  if ( NULL_PTR != IpAddrPtr )
  { /* pointer is valid */
    IpAddrPtr->domain = SOAD_AF_UNSPEC;
#if ( SOAD_VIPV6_ENABLED == STD_ON )
    ((SoAd_SockAddrInet6Type*)IpAddrPtr)->port    = SOAD_PORT_ANY;
    ((SoAd_SockAddrInet6Type*)IpAddrPtr)->addr[0] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)IpAddrPtr)->addr[1] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)IpAddrPtr)->addr[2] = SOAD_IP6ADDR_ANY;
    ((SoAd_SockAddrInet6Type*)IpAddrPtr)->addr[3] = SOAD_IP6ADDR_ANY;
#else
    ((SoAd_SockAddrInetType*)IpAddrPtr)->port    = SOAD_PORT_ANY;
    ((SoAd_SockAddrInetType*)IpAddrPtr)->addr[0] = SOAD_IPADDR_ANY;
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
  }
}

/**********************************************************************************************************************
 *  SoAd_CopyIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_CopyIpAddr(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) TgtIpAddr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) SrcIpAddr,
  boolean OverwriteAll)
{
  /* This function assumes that there is enough memory reserved to overwrite an IPv4 address with an IPv6 address */
  if ( (NULL_PTR != SrcIpAddr) && (NULL_PTR != TgtIpAddr) )
  { /* valid pointer */
    if ( SOAD_AF_UNSPEC == SrcIpAddr->domain )
    { /* unspecific domain */
      ((SoAd_SockAddrInetXType*)TgtIpAddr)->domain = SOAD_AF_UNSPEC;
      if ( (TRUE == OverwriteAll) || ( SOAD_PORT_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->port ) )
      {
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->port = (SOAD_A_P2CONST(SoAd_SockAddrInetXType)SrcIpAddr)->port;
      }
#if ( SOAD_VIPV6_ENABLED == STD_ON )
      if ( (TRUE == OverwriteAll) ||
        ((SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[0]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[1]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[2]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[3])) )
      {
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[0] = SOAD_IP6ADDR_ANY;
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[1] = SOAD_IP6ADDR_ANY;
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[2] = SOAD_IP6ADDR_ANY;
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[3] = SOAD_IP6ADDR_ANY;
      }
#else
      if ( (TRUE == OverwriteAll) || (SOAD_IPADDR_ANY == ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[0]) )
      {
        ((SoAd_SockAddrInetXType*)TgtIpAddr)->addr[0] = SOAD_IPADDR_ANY;
      }
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
    }
    else if ( SOAD_AF_INET == SrcIpAddr->domain )
    { /* IPv4 */
      ((SoAd_SockAddrInetType*)TgtIpAddr)->domain = (SOAD_A_P2CONST(SoAd_SockAddrInetType)SrcIpAddr)->domain;
      if ( (TRUE == OverwriteAll) || ( SOAD_PORT_ANY == ((SoAd_SockAddrInetType*)TgtIpAddr)->port ) )
      {
        ((SoAd_SockAddrInetType*)TgtIpAddr)->port = (SOAD_A_P2CONST(SoAd_SockAddrInetType)SrcIpAddr)->port;
      }
      if ( (TRUE == OverwriteAll) || (SOAD_IPADDR_ANY == ((SoAd_SockAddrInetType*)TgtIpAddr)->addr[0]) )
      {
        ((SoAd_SockAddrInetType*)TgtIpAddr)->addr[0] = (SOAD_A_P2CONST(SoAd_SockAddrInetType)SrcIpAddr)->addr[0];
      }
    }
#if ( SOAD_VIPV6_ENABLED == STD_ON )
    else if ( SOAD_AF_INET6 == SrcIpAddr->domain )
    { /* IPv6 */
      ((SoAd_SockAddrInet6Type*)TgtIpAddr)->domain  = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->domain;
      if ( (TRUE == OverwriteAll) || ( SOAD_PORT_ANY == ((SoAd_SockAddrInet6Type*)TgtIpAddr)->port ) )
      {
        ((SoAd_SockAddrInet6Type*)TgtIpAddr)->port    = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->port;
      }
      if ( (TRUE == OverwriteAll) ||
        ((SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[0]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[1]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[2]) &&
          (SOAD_IP6ADDR_ANY == ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[3])) )
      {
        ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[0] = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->addr[0];
        ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[1] = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->addr[1];
        ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[2] = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->addr[2];
        ((SoAd_SockAddrInet6Type*)TgtIpAddr)->addr[3] = (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcIpAddr)->addr[3];
      }
    }
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
    else
    { /* unknown IP version */
      /* ignore */
    }
  }
}
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

#if ( SOAD_VSOCKET_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_ON )
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CopyBufSeg2Buf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_CopyBufSeg2Buf(
  P2VAR(uint8, SOAD_APPL_DATA, SOAD_APPL_DATA) BufPtr,
  P2CONST(SoAd_BufSegMgtType, SOAD_APPL_DATA, SOAD_APPL_DATA) BufSegPtr,
  uint32 BufLen,
  uint32 StartPos)
{
  uint32 bytesToCopy;
  uint32 bytesCopied = 0u;
  uint16 bufSegIdx = BufSegPtr->Idx;

  /* get start position */
  while ( 0u < StartPos )
  {
    if ( BufSegPtr->BufSegs[bufSegIdx].Len < StartPos )
    { /* start position in next buffer segment */
      StartPos -= BufSegPtr->BufSegs[bufSegIdx].Len;
      if ( (bufSegIdx + 1u) == SOAD_VBUF_SEG_SIZE_TOTAL )
      { /* wrap around */
        bufSegIdx = 0u;
      }
      else
      { /* no wrap around */
        bufSegIdx++;
      }
    }
    else
    { /* start position in current segment */
      break;
    }
  }

  /* copy to target buffer */
  bytesToCopy = (uint32)(BufSegPtr->BufSegs[bufSegIdx].Len - StartPos);
  if ( bytesToCopy < BufLen )
  { /* more data shall be copied than available in current segment */
    /* copy first segment */
    IpBase_Copy((IpBase_CopyDataType*)&BufPtr[0],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&BufSegPtr->BufSegs[bufSegIdx].DataPtr[StartPos],
      bytesToCopy);
    BufLen -= bytesToCopy;
    bytesCopied += bytesToCopy;

    /* copy remaining segments */
    while ( 0u < BufLen )
    {
      /* get index of segment */
      if ( (bufSegIdx + 1u) == SOAD_VBUF_SEG_SIZE_TOTAL )
      { /* wrap around */
        bufSegIdx = 0u;
      }
      else
      { /* no wrap around */
        bufSegIdx++;
      }

      /* get remaining data size */
      if ( BufSegPtr->BufSegs[bufSegIdx].Len < BufLen )
      {
        bytesToCopy = BufSegPtr->BufSegs[bufSegIdx].Len;
      }
      else
      {
        bytesToCopy = BufLen;
      }

      IpBase_Copy((IpBase_CopyDataType*)&BufPtr[bytesCopied],
        (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&BufSegPtr->BufSegs[bufSegIdx].DataPtr[0],
        bytesToCopy);
      BufLen -= bytesToCopy;
      bytesCopied += bytesToCopy;
    }
  }
  else
  { /* segment is bigger than data to be copied */
    IpBase_Copy((IpBase_CopyDataType*)&BufPtr[0],
      (P2CONST(IpBase_CopyDataType, AUTOMATIC, AUTOMATIC))&BufSegPtr->BufSegs[bufSegIdx].DataPtr[StartPos], BufLen);
  }
}
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
# endif /* SOAD_VTCP_USED == STD_ON */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

#if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CopyPduHdr2Buf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_CopyPduHdr2Buf(
  SoAd_PduHdrIdType PduHeaderId,
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  PduLengthType Offset,
  PduLengthType Length)
{
  PduLengthType pduHdrIdx = Offset;
  PduLengthType pduHdrEndIdx = Offset + Length;

  if ( (SOAD_PDU_HDR_SIZE <= Length) && ( 0u == Offset) )
  { /* copy entire PDU Header */
    BufPtr[0] = (uint8)(PduHeaderId >> 24);
    BufPtr[1] = (uint8)(PduHeaderId >> 16);
    BufPtr[2] = (uint8)(PduHeaderId >> 8);
    BufPtr[3] = (uint8)(PduHeaderId);
    BufPtr[4] = (uint8)(PduHeaderLen >> 24);
    BufPtr[5] = (uint8)(PduHeaderLen >> 16);
    BufPtr[6] = (uint8)(PduHeaderLen >> 8);
    BufPtr[7] = (uint8)(PduHeaderLen);
  }
  else
  { /* copy PDU Header segment */
    uint8 pduHdrBuf[SOAD_PDU_HDR_SIZE];

    /* create local PDU Header buffer */
    pduHdrBuf[0] = (uint8)(PduHeaderId >> 24);
    pduHdrBuf[1] = (uint8)(PduHeaderId >> 16);
    pduHdrBuf[2] = (uint8)(PduHeaderId >> 8);
    pduHdrBuf[3] = (uint8)(PduHeaderId);
    pduHdrBuf[4] = (uint8)(PduHeaderLen >> 24);
    pduHdrBuf[5] = (uint8)(PduHeaderLen >> 16);
    pduHdrBuf[6] = (uint8)(PduHeaderLen >> 8);
    pduHdrBuf[7] = (uint8)(PduHeaderLen);

    /* copy segment */
    while ( pduHdrIdx < pduHdrEndIdx )
    {
      BufPtr[pduHdrIdx - Offset] = pduHdrBuf[pduHdrIdx];
      pduHdrIdx++;
    }
  }
}
#endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_InitMemory
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module state to uninit. */
  SoAd_State = SOAD_STATE_UNINIT;

#if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* #20 Reset configuration pointer. */
  SoAd_ConfigPtr = NULL_PTR;
#endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
}

/**********************************************************************************************************************
 *  SoAd_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 3206 1 */ /* MD_SoAd_3206 */
FUNC(void, SOAD_CODE) SoAd_Init(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAdConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  uint8 errorId = SOAD_E_NO_ERROR;
#endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* #10 Check plausibility of input parameter. */
  if ( SoAdConfigPtr == NULL_PTR )
  {
    EcuM_BswErrorHook((uint16) SOAD_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
    errorId = SOAD_E_INIT_FAILED;
  }
  else if ( SoAdConfigPtr->FinalMagicNumber != SOAD_FINAL_MAGIC_NUMBER )
  {
    EcuM_BswErrorHook((uint16) SOAD_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
    errorId =  SOAD_E_INIT_FAILED;
  }
  else
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize configuration pointer. */
#if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    SoAd_ConfigPtr = SoAdConfigPtr;
#else
    SOAD_DUMMY_STATEMENT_VAR(SoAdConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */

    /* #30 Initialize local global data. */
#if ( (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) )
    SoAd_GlobalCounter = 0u;
#endif /* (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) */
    SoAd_CriticalSectionCount = 0u;
    SoAd_OpenSocks = 0u;
    SoAd_ShutdownFinishedCnt = 0u;

    /* #40 Initialize event queues. */
    SoAd_EventQueue_Init();

#if ( (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) )
    /* #50 Initialize timeout lists. */
    SoAd_TimeoutList_Init();
#endif /* (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) || (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) */

    /* #60 Initialize Socket API specific structs. */
    SoAd_TcpIp_Init(); /*lint !e522 */

    /* #70 Initialize socket connection structs. */
    SoAd_SoCon_Init();

#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
    /* #80 Initialize routing group structs. */
    SoAd_RouteGrp_Init(); /*lint !e522 */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */

#if ( SOAD_VPDU_ROUTES == STD_ON )
    /* #90 Initialize Tx structs. */
    SoAd_Tx_Init();
#endif /* SOAD_VPDU_ROUTES == STD_ON */

    /* #100 Initialize module internal socket index structs. */
    SoAd_InitSockIdxStructs();

    /* #110 Set module state to initialized. */
    SoAd_State = SOAD_STATE_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* #120 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_INIT, errorId);
  }
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
#else
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_STCAL */

/**********************************************************************************************************************
 *  SoAd_IfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_PduIdxType   pduRouteIdx;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsTxPduIdValid(SoAdSrcPduId) )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(SoAdSrcPduId)).UpperLayer == SOAD_UL_TYPE_TP )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAdSrcPduInfoPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( (SoAdSrcPduInfoPtr->SduDataPtr == NULL_PTR)
# if ( SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON )
    && (SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(SoAdSrcPduId)).IfTriggerTransmitEnabled == FALSE)
# endif /* SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON */
    )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    pduRouteIdx = SoAd_TxPduId2PduRouteIdx(SoAdSrcPduId);

    /* #30 Check parameters for transmission. */
    if ( SoAd_TxIf_CheckParameter(pduRouteIdx, SoAdSrcPduInfoPtr->SduLength) == E_OK )
    {
      /* #40 Trigger transmission of PDU. */
      retVal = SoAd_TxIf_TransmitPdu(pduRouteIdx, SoAdSrcPduInfoPtr, &errorId);
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(SoAdSrcPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(SoAdSrcPduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_IF_TRANSMIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_IfRoutingGroupTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfRoutingGroupTransmit(
  SoAd_RoutingGroupIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) )
  uint16                    pduDestNum;
  SoAd_RoutingGroupIdxType  routeGrpIdx;
#endif /* (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_RouteGrpIdx2RoutingGroup(SoAd_RouteGrpId2RouteGrpIdx(id)).TxTriggerable == FALSE )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) )
    /* #30 Check initialization state to reject request in case of shutdown. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);
      pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

      /* #40 Store transmission request for all related PduRouteDests. */
      while ( pduDestNum  > 0u )
      {
        pduDestNum--;
        *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestNum] |= SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER;
      }

      /* #50 Set event to handle routing group transmit request in main function. */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxIfTxRouteGrp(), routeGrpIdx);

      retVal = E_OK;
    }
#else
  SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_IF_ROUT_GROUP_TRANSMIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_IfSpecificRoutingGroupTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfSpecificRoutingGroupTransmit(
  SoAd_RoutingGroupIdType id,
  SoAd_SoConIdType SoConId )
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) )
  uint16                    pduDestNum;
  uint16_least              pduDestIdx;
  SoAd_RoutingGroupIdxType  routeGrpIdx;
  SoAd_SoConIdxType         soConIdx;
#endif /* (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_RouteGrpIdx2RoutingGroup(SoAd_RouteGrpId2RouteGrpIdx(id)).TxTriggerable == FALSE )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) )
    /* #30 Check initialization state to reject request in case of shutdown. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);
      pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

      /* #40 Store transmission request for the related PduRouteDest. */
      for ( pduDestIdx = 0u; pduDestIdx < pduDestNum; pduDestIdx++ )
      {
        if ( soConIdx == SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListPtr[pduDestIdx]->SoConIdx )
        {
          *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestIdx] |=
            SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER;
          retVal = E_OK;
          break;
        }
      }

      /* #50 Set event to handle routing group transmit request in main function. */
      SoAd_EventQueue_AddElement(SoAd_EventQueueIdxIfTxRouteGrp(), routeGrpIdx);
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(SoConId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* (SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON) && (SOAD_VPDU_ROUTES == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_IF_SPEC_ROUT_GROUP_TRANSMIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpTransmit(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_PduIdxType   pduRouteIdx;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsTxPduIdValid(SoAdSrcPduId) )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(SoAdSrcPduId)).UpperLayer != SOAD_UL_TYPE_TP )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAdSrcPduInfoPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    pduRouteIdx = SoAd_TxPduId2PduRouteIdx(SoAdSrcPduId);

    /* #30 Check current module state and input parameters (non default errors). */
    if ( SoAd_TxTp_CheckParameter(pduRouteIdx, SoAdSrcPduInfoPtr->SduLength) == E_OK )
    {
      /* #40 Try to store transmission request. */
      if ( SoAd_TxTp_TransmitPdu(pduRouteIdx, SoAdSrcPduInfoPtr) == E_OK )
      {
        retVal = E_OK;
      }
    }
#else
  SOAD_DUMMY_STATEMENT_VAR(SoAdSrcPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  SOAD_DUMMY_STATEMENT_VAR(SoAdSrcPduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TP_TRANSMIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_Shutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Shutdown(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    switch (SoAd_State)
    {
      /* #20 Handle shutdown request in case module is initialized. */
      case SOAD_STATE_INIT:
      {
        /* #200 Return that shutdown is in progress if any socket is opened (continue shutdown in main function). */
        if ( 0u != SoAd_OpenSocks )
        {
          SoAd_ShutdownFinishedCnt = SOAD_SHUTDOWN_FINISHED_WAIT;
          SoAd_State = SOAD_STATE_SHUTDOWN_IN_PROGRESSS;
          retVal = SOAD_E_INPROGRESS;
        }
        /* #201 Return that shutdown is finished and notify user if all sockets are closed. */
        else
        {
          SoAd_State = SOAD_STATE_SHUTDOWN;
#if ( SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
          SoAd_VCallShutdwnFinishedFuncs();
#endif /* SOAD_VSHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
          retVal = E_OK;
        }
        break;
      }
      /* #21 Return state in case shutdown is in progress. */
      case SOAD_STATE_SHUTDOWN_IN_PROGRESSS:
      {
        retVal = SOAD_E_INPROGRESS;
        break;
      }
      /* #22 Return state in case shutdown is finished. */
      case SOAD_STATE_SHUTDOWN:
      {
        retVal = E_OK;
        break;
      }
      /* #23 Reject request in case of an invalid state. */
      default:
      {
        /* Nothing to do. */
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SHUTDOWN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_TpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelTransmit(
  PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_SoConIdxType soConIdx;
  SoAd_PduIdxType   pduRouteIdx;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsTxPduIdValid(PduId) )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(PduId)).UpperLayer != SOAD_UL_TYPE_TP )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      pduRouteIdx = SoAd_TxPduId2PduRouteIdx(PduId);
      soConIdx = SoAd_TxPduIdx2PduRoute(pduRouteIdx).PduRouteDestListPtr[0]->SoConIdx;

      /* #40 Check if PDU transmission is active on socket connection. */
      if ( pduRouteIdx == SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx )
      {
        /* #50 Set flag to perform cancellation. */
        SoAd_SockConIdx2TxMgt(soConIdx).CancelRequested = TRUE;

        /* #60 Set event to handle PDU (i.e. cancellation) in main function. */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpTxSoCon(), soConIdx);

        retVal = E_OK;
      }
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(PduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TP_CANCEL_TRANSMIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TpCancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelReceive(
  PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
  SoAd_SoConIdxType soConIdx;
  SoAd_PduIdxType   sockRouteIdx;
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  Std_ReturnType    retVal    = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsRxPduIdValid(PduId) )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAd_SockRoutes(SoAd_RxPduId2SockRouteIdx(PduId)).SockRouteDest.UpperLayer != SOAD_UL_TYPE_TP )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      sockRouteIdx = SoAd_RxPduId2SockRouteIdx(PduId);
      soConIdx = SoAd_SockRoutes(sockRouteIdx).SoConIdx;

      /* #40 Check if PDU reception is active on socket connection. */
      if ( sockRouteIdx == SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx )
      {
        /* #50 Set flag to perform cancellation. */
        SoAd_SockConIdx2RxMgt(soConIdx).CancelRequested = TRUE;

        /* #60 Set event to handle PDU (i.e. cancellation) in main function. */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxTpRxSoCon(), soConIdx);

        retVal = E_OK;
      }
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(PduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #70 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TP_CANCEL_RECEIVE, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_GetSoConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConId(
  PduIdType TxPduId,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) SoConIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_SoConIdxType soConIdx;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsTxPduIdValid(TxPduId) )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(TxPduId)).PduRouteDestListSize != 1u )
  {
    errorId = SOAD_E_INV_PDUID;
  }
  else if ( SoConIdPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #40 Return corresponding socket connection identifier. */
      soConIdx = SoAd_TxPduIdx2PduRoute(SoAd_TxPduId2PduRouteIdx(TxPduId)).PduRouteDestListPtr[0]->SoConIdx;
      *SoConIdPtr = SoAd_SockCons(soConIdx).SoConId;

      retVal = E_OK;
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(TxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(SoConIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_SO_CON_ID, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_OpenSoCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_OpenSoCon(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).SockConGrpPtr->SockAutoSoConSetup == TRUE )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Enter critical section to prevent that call is interrupted and open close counter wraps around. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Check if open close counter can handle new request. */
      if ( SoAd_SockConIdx2FlagCntOpCl(soConIdx) < SOAD_INV_BUF_IDX_8 )
      {
        /* #60 Increment counter. */
        SoAd_SockConIdx2FlagCntOpCl(soConIdx)++;

        /* #70 Set event to handle socket connection state in main function. */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);

        retVal = E_OK;
      }

      /* #80 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #90 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_OPEN_SO_CON, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_CloseSoCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_CloseSoCon(
  SoAd_SoConIdType SoConId,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter 'SoConId'. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).SockConGrpPtr->SockAutoSoConSetup == TRUE )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Enter critical section to prevent that call is interrupted and open close counter wraps around. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Check if open close counter can handle new request. */
      if ( 0u < SoAd_SockConIdx2FlagCntOpCl(soConIdx) )
      {
        /* #60 Decrement counter. */
        SoAd_SockConIdx2FlagCntOpCl(soConIdx)--;

        retVal = E_OK;
      }
      /* #70 Check if socket connection is not yet offline. */
      if ( SoAd_SockConIdx2State(soConIdx) != SOAD_SOCON_OFFLINE )
      {
        /* #80 Check if open close counter is 0 or 'Abort' flag is set. */
        if ( (SoAd_SockConIdx2FlagCntOpCl(soConIdx) == 0u) || (Abort == TRUE) )
        {
          /* #90 Set flags to close socket connection in main function. */
          SoAd_SockConIdx2FlagAbort(soConIdx)   |= Abort;
          SoAd_SockConIdx2FlagCntOpCl(soConIdx) = 0u;
          SoAd_SockConIdx2CloseMode(soConIdx)   = SOAD_CLOSE_OFFLINE;

          /* #100 Set event to handle socket connection state in main function. */
          SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);

          retVal = E_OK;
        }
      }

      /* #110 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #120 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_CLOSE_SO_CON, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter 'SoConId'. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* #30 Check plausibility of input parameter 'LocalIpAddrPtr' dependent on 'Type'. */
  else if ( (Type == SOAD_IPADDR_ASSIGNMENT_STATIC) && (LocalIpAddrPtr == NULL_PTR) )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( (Type == SOAD_IPADDR_ASSIGNMENT_STATIC) &&
    (SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).SockConGrpPtr->Domain != LocalIpAddrPtr->domain) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #50 Handle IP address assignment request. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = SoAd_TcpIp_RequestIpAddrAssignment(
        soConIdx,
        Type,
        LocalIpAddrPtr,
        Netmask,
        DefaultRouterPtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_REQ_IP_ADDR_ASSIGN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #40 Handle IP address assignment release request. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = SoAd_TcpIp_ReleaseIpAddrAssignment(soConIdx);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_RLS_IP_ADDR_ASSIGN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_GetLocalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( LocalAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( NetmaskPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( DefaultRouterPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #40 Get local IP address. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      SoAd_TcpIp_GetIpAddr(soConIdx, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_LOCAL_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_GetPhysAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetPhysAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType     soConIdx;
  Std_ReturnType        retVal = E_NOT_OK;
  uint8                 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( PhysAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #40 Get physical address. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = SoAd_TcpIp_GetPhysAddr(soConIdx, PhysAddrPtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_PHYS_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}

/**********************************************************************************************************************
 *  SoAd_GetRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Check if remote address is set and contains at least wildcards. */
      if ( ((SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET)
#if ( SOAD_VIPV6_ENABLED == STD_ON )
          || (SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET6)
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
          )
        && (SoAd_SockConIdx2RemAddrState(soConIdx) <= SOAD_SOCON_IP_ANY_PORT_ANY) )
      {
        /* #50 Return remote address. */
        SoAd_CopyIpAddr(IpAddrPtr,
          (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(soConIdx), TRUE);

        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_GetRemoteAddrState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddrState(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr,
  P2VAR(SoAd_RemAddrStateType, AUTOMATIC, SOAD_APPL_VAR) RemAddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( RemAddrState == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Return remote address state. */
      *RemAddrState = SoAd_SockConIdx2RemAddrState(soConIdx);

      /* #50 Check if remote address is set. */
      if ( (SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET)
#if ( SOAD_VIPV6_ENABLED == STD_ON )
        || (SoAd_SockConIdx2RemSockDyn(soConIdx).domain == SOAD_AF_INET6)
#endif /* SOAD_VIPV6_ENABLED == STD_ON */
        )
      { /* #60 Return remote address. */
        SoAd_CopyIpAddr(IpAddrPtr,
          (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&SoAd_SockConIdx2RemSockDyn(soConIdx), TRUE);

        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_REMOTE_ADDR_STATE, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

#if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_GetRcvRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRcvRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_VSOCKET_ROUTES == STD_ON )
  SoAd_SoConIdxType soConIdx;
  boolean           ipAddrIsAny = TRUE;
  boolean           portIsAny = TRUE;
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).RcvRemSockPtr == NULL_PTR )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
# endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( SOAD_VSOCKET_ROUTES == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Enter critical section to prevent that a reception interrupts and address becomes inconsistent. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Check if address is set and has no wildcards (i.e. received something from a remote entity). */
      SoAd_CheckForWildcards(SoAd_SockCons(soConIdx).RcvRemSockPtr, &ipAddrIsAny, &portIsAny);
      if ( (ipAddrIsAny != TRUE) && (portIsAny != TRUE) )
      {
        /* #60 Return received remote address. */
        SoAd_CopyIpAddr(IpAddrPtr, SoAd_SockCons(soConIdx).RcvRemSockPtr, TRUE);

        retVal = E_OK;
      }

      /* #70 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
# else
    SOAD_DUMMY_STATEMENT_VAR(SoConId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(IpAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #80 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_RCV_REMOTE_ADDR, errorId);
  }
# else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */
#endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_EnableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRouting(
  SoAd_RoutingGroupIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  uint16                    pduDestNum;
#endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  uint16                    socketRouteNum;
#endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
  SoAd_RoutingGroupIdxType  routeGrpIdx;
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      /* #40 Enable routing group on all dependent PduRouteDests. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2PduRouteDestColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

        while ( pduDestNum  > 0u )
        {
          pduDestNum--;
          *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestNum] |=
            SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
        }
      }
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
      /* #50 Enable routing group on all dependent SocketRoutes. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2SocketRouteColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        socketRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdNum;

        while ( socketRouteNum > 0u )
        {
          socketRouteNum--;
          *SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).StateColPtr[socketRouteNum] |=
            SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
        }
      }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */

      retVal = E_OK;
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_ENABLE_ROUTING, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_EnableSpecificRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableSpecificRouting(
  SoAd_RoutingGroupIdType id,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  uint16                    pduDestNum;
#endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  uint16                    socketRouteNum;
  uint16                    sockRouteIdx;
#endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
  SoAd_SoConIdxType         soConIdx;
  SoAd_RoutingGroupIdxType  routeGrpIdx;
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      /* #40 Iterate over all dependent PduRouteDests to find the affected PduRouteDests. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2PduRouteDestColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

        while ( pduDestNum  > 0u )
        {
          pduDestNum--;

          /* #400 Enable routing group for PduRouteDest with requested socket connection. */
          if ( soConIdx == SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListPtr[pduDestNum]->SoConIdx )
          {
            *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestNum] |=
              SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
            retVal = E_OK;
          }
        }
      }
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
      /* #50 Iterate over all dependent SocketRoutes to find the affected SocketRoutes. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2SocketRouteColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        socketRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdNum;

        while ( socketRouteNum > 0u )
        {
          socketRouteNum--;

          /* #500 Enable routing group for SocketRoute with requested socket connection. */
          sockRouteIdx = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdColPtr[socketRouteNum];
          if ( soConIdx == SoAd_SockRoutes(sockRouteIdx).SoConIdx )
          {
            *SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).StateColPtr[socketRouteNum] |=
              SOAD_ROUT_GRP_BIT_PATTERN_ENABLED;
            retVal = E_OK;
          }
        }
      }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(SoConId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_ENABLE_SPECIFIC_ROUTING, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_DisableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableRouting(
  SoAd_RoutingGroupIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  uint16                    pduDestNum;
#endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  uint16                    socketRouteNum;
#endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
  SoAd_RoutingGroupIdxType  routeGrpIdx;
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      /* #40 Disable routing group on all dependent PduRouteDests. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2PduRouteDestColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

        while ( pduDestNum  > 0u )
        {
          pduDestNum--;
          /* PRQA S 0277 2 */ /* MD_SoAd_3.1 */
          *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestNum] &=
            (uint8)(~SOAD_ROUT_GRP_BIT_PATTERN_ENABLED);
        }
      }
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
      /* #50 Disable routing group on all dependent SocketRoutes. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2SocketRouteColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        socketRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdNum;

        while ( socketRouteNum > 0u )
        {
          socketRouteNum--;
          /* PRQA S 0277 2 */ /* MD_SoAd_3.1 */
          *SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).StateColPtr[socketRouteNum] &=
            (uint8)(~SOAD_ROUT_GRP_BIT_PATTERN_ENABLED);
        }
      }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */

      retVal = E_OK;
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_DISABLE_ROUTING, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_DisableSpecificRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableSpecificRouting(
  SoAd_RoutingGroupIdType id,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  uint16                    pduDestNum;
#endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  uint16                    socketRouteNum;
  uint16                    sockRouteIdx;
#endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
  SoAd_SoConIdxType         soConIdx;
  SoAd_RoutingGroupIdxType  routeGrpIdx;
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  Std_ReturnType            retVal = E_NOT_OK;
  uint8                     errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( !SoAd_IsRouteGrpIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VROUT_GROUP_ENABLED == STD_ON )
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      routeGrpIdx = SoAd_RouteGrpId2RouteGrpIdx(id);
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
      /* #40 Iterate over all dependent PduRouteDests to find the affected PduRouteDests. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2PduRouteDestColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        pduDestNum = SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListSize;

        while ( pduDestNum  > 0u )
        {
          pduDestNum--;

          /* #400 Disable routing group for PduRouteDest with requested socket connection. */
          if ( soConIdx == SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).PduRouteDestListPtr[pduDestNum]->SoConIdx )
          {
            /* PRQA S 0277 2 */ /* MD_SoAd_3.1 */
            *SoAd_RouteGrpIdx2PduRouteDestCol(routeGrpIdx).StateColPtr[pduDestNum] &=
              (uint8)(~SOAD_ROUT_GRP_BIT_PATTERN_ENABLED);
            retVal = E_OK;
          }
        }
      }
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */

# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
      /* #50 Iterate over all dependent SocketRoutes to find the affected SocketRoutes. */
#  if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsRouteGrpIdx2SocketRouteColPresent() )
#  endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        socketRouteNum = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdNum;

        while ( socketRouteNum > 0u )
        {
          socketRouteNum--;

          /* #500 Disable routing group for SocketRoute with requested socket connection. */
          sockRouteIdx = SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).IdColPtr[socketRouteNum];
          if ( soConIdx == SoAd_SockRoutes(sockRouteIdx).SoConIdx )
          {
            /* PRQA S 0277 2 */ /* MD_SoAd_3.1 */
            *SoAd_RouteGrpIdx2SocketRouteCol(routeGrpIdx).StateColPtr[socketRouteNum] &=
              (uint8)(~SOAD_ROUT_GRP_BIT_PATTERN_ENABLED);
            retVal = E_OK;
          }
        }
      }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(SoConId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VROUT_GROUP_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_DISABLE_SPECIFIC_ROUTING, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_SetRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 SoAd_167 */ /* MD_SoAd_16.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  boolean           hasSoConModeChanged = FALSE;
  uint8             errorId = SOAD_E_NO_ERROR;
  Std_ReturnType    retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).SockConGrpPtr->SockAutoSoConSetup == TRUE )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( RemoteAddrPtr->domain != SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).SockConGrpPtr->Domain )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = E_OK;

      /* #40 Check if socket connection is offline or a UDP socket connection. */
      if ( (SoAd_SockConIdx2State(soConIdx) != SOAD_SOCON_OFFLINE) &&
        (SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr == NULL_PTR) )
      {
        retVal = E_NOT_OK;
      }

      /* #50 Check if no TP transmission is active. */
#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsPduRoutePresent() )
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        if ( SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx != SOAD_INV_PDU_ID )
        {
          retVal = E_NOT_OK;
        }
      }
#endif /* SOAD_VPDU_ROUTES == STD_ON */

      /* #60 Check if no TP reception is active. */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      if ( SoAd_IsSockRoutePresent() )
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
      {
        if ( SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx != SOAD_INV_PDU_ID )
        {
          retVal = E_NOT_OK;
        }
      }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

      /* #70 Enter critical section to prevent that remote address is set in other context. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #80 Check and set remote address if valid. */
      if ( retVal == E_OK )
      {
        retVal = SoAd_SoCon_CheckAndSetRemoteAddr(soConIdx, RemoteAddrPtr);
      }

      /* #90 Check if socket connection is online and shall be set to reconnect since remote address has wildcards. */
      if ( retVal == E_OK )
      {
        if ( (SoAd_SockConIdx2State(soConIdx) == SOAD_SOCON_ONLINE) &&
          ((SoAd_SockConIdx2RemAddrState(soConIdx) & SOAD_SOCON_MASK_ANY) != 0u) )
        {
          SoAd_SockConIdx2State(soConIdx) = SOAD_SOCON_RECONNECT;
          hasSoConModeChanged = TRUE;
        }
      }

      /* #100 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #110 Notify about changed socket conenction mode if required. */
      if ( hasSoConModeChanged == TRUE )
      {
        SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).SoConChgFuncBitPattern, SOAD_CHG_FUNC_TYPE_SOCONMODE,
          SOAD_SOCON_RECONNECT);
      }
      /* #111 Check if remote address is set otherwise to set event to handle socket connection in main function. */
      else if ( (retVal == E_OK) && (SoAd_SockConIdx2RemAddrState(soConIdx) == SOAD_SOCON_IP_SET_PORT_SET) )
      {
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
      }
      else
      {
        /* Nothing to do. */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #120 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SET_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:SoAd_167 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_SetUniqueRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3673 3 */ /* MD_SoAd_16.7 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) AssignedSoConIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  SoAd_SoConIdxType assignedSoConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  boolean           ipAddrIsAny = FALSE;
  boolean           portIsAny = FALSE;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( RemoteAddrPtr->domain != SoAd_SockCons(SoAd_SockConId2SockConIdx(SoConId)).RemSockPtr->domain )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( AssignedSoConIdPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      /* #40 Enter critical section to prevent that remote address is set in other context. */
      SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Check if remote address contains wildcards. */
      SoAd_CheckForWildcards(RemoteAddrPtr, &ipAddrIsAny, &portIsAny);
      if ( (ipAddrIsAny == FALSE) && (portIsAny == FALSE) )
      {
        SoAd_SoConIdxType soConIdxStart;
        SoAd_SoConIdxType soConIdxMax;

        /* #60 Get affected socket connections if remote address does not contain wildcards. */
#if ( defined(SOAD_VTCP_USED) && (SOAD_VTCP_USED) )
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr != NULL_PTR )
        { /* TCP socket */
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketTcpPtr->TcpInitiate == TRUE )
          { /* TCP client socket */
            SoAd_SoConIdxType soConIdxTmp = SoAd_SockConTotal();

            soConIdxStart = 0u;
            soConIdxMax = SOAD_INV_SO_CON_ID;
            while ( soConIdxTmp > 0 )
            {
              soConIdxTmp--;

              if ( SoAd_SockCons(soConIdxTmp).SockConGrpPtr == SoAd_SockCons(soConIdx).SockConGrpPtr )
              {
                if ( soConIdxMax == SOAD_INV_SO_CON_IDX )
                {
                  soConIdxMax = soConIdxTmp;
                }
                soConIdxStart = soConIdxTmp;
              }
            }
          }
          else
          { /* TCP server socket */
            soConIdxStart = SoAd_SockIdx2SoConIdxCol(SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx).SoConIdxStart;
            soConIdxMax = (SoAd_SoConIdxType)(soConIdxStart + SoAd_SockIdx2SoConIdxCol(
              SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx).SoConIdxNum - 1u);
          }
        }
        else
#endif /* defined(SOAD_VTCP_USED) && (SOAD_VTCP_USED) */
        { /* UDP Socket */
          soConIdxStart = SoAd_SockIdx2SoConIdxCol(SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx).SoConIdxStart;
          soConIdxMax = (SoAd_SoConIdxType)(soConIdxStart + SoAd_SockIdx2SoConIdxCol(
            SoAd_SockCons(soConIdx).SockConGrpPtr->GrpSockIdx).SoConIdxNum - 1u);
        }

        /* #70 Get socket connection using best match algorithm. */
        if ( SoAd_GetSoConIdByBestMatchAlg(RemoteAddrPtr, soConIdxStart, soConIdxMax, SOAD_SOCON_FILTER_DISABLED,
            FALSE, &assignedSoConIdx) == E_OK )
        {
          /* #80 Check if remote address can be set on socket connection if a socket connection was found. */
          if ( SoAd_SockConIdx2RemAddrState(assignedSoConIdx) == SOAD_SOCON_IP_SET_PORT_SET )
          { /* #81 Allow setting remote address if same remote address is already set. */
            retVal = E_OK;
          }
          else
          {
            /* #82 Allow setting remote address for TCP if socket is closed and remote address is valid. */
#if ( defined(SOAD_VTCP_USED) && (SOAD_VTCP_USED) )
            if ( SoAd_SockCons(assignedSoConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
            {
              if ( SoAd_SockIdx2SockState(SoAd_SockCons(assignedSoConIdx).SockIdx) == SOAD_SOCK_STATE_CLOSED )
              {
                if ( SoAd_SoCon_CheckAndSetRemoteAddr(assignedSoConIdx, RemoteAddrPtr) == E_OK )
                {
                  /* #820 Set event to handle socket connection state in main function. */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), assignedSoConIdx);

                  retVal = E_OK;
                }
              }
            }
            /* #83 Allow setting remote address for UDP if no transmission is pending and remote address is valid. */
            else
#endif /* defined(SOAD_VTCP_USED) && (SOAD_VTCP_USED) */
            {
#if ( SOAD_VPDU_ROUTES == STD_ON )
              if ( (!SoAd_IsPduRoutePresent()) ||
                (SoAd_SockIdx2ActiveSoConIdx(SoAd_SockCons(assignedSoConIdx).SockIdx) != assignedSoConIdx) )
#endif /* SOAD_VPDU_ROUTES == STD_ON */
              {
                if ( SoAd_SoCon_CheckAndSetRemoteAddr(assignedSoConIdx, RemoteAddrPtr) == E_OK )
                {
#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
                  /* #830 Remove alive supervision timeout from timeout list if configured and running. */
                  if ( SoAd_SockCons(assignedSoConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt != 0u )
                  {
                    SoAd_TimeoutList_RemoveElement(SoAd_TimeoutListIdxUdpAlive(), assignedSoConIdx);
                  }
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

                  /* #831 Set event to handle socket connection state in main function. */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), assignedSoConIdx);

                  retVal = E_OK;
                }
              }
            }
          }

          /* #90 Return socket connection if found by best match algorithm and it is valid to set. */
          if ( retVal == E_OK )
          {
            *AssignedSoConIdPtr = SoAd_SockCons(assignedSoConIdx).SoConId;
          }
        }
      }

      /* #110 Leave critical section. */
      SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #120 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SET_UNI_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TpChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_SoAd_3206 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpChangeParameter(
  PduIdType id,
  TPParameterType parameter,
  uint16 value )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'id' */
  else if ( !SoAd_IsTxPduIdValid(id) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */

    /* function is implemented without functionality as no requirements are available */

    SOAD_DUMMY_STATEMENT_VAR(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(parameter); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(value); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TP_CHANGE_PARAMETER, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return E_NOT_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  SoAd_ReadDhcpHostNameOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) length,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) data)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check plausibility of input parameters. */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( length == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( data == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check if component is initialized. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #30 Handle read DHCP hostname option request. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = SoAd_TcpIp_ReadDhcpHostNameOption(soConIdx, length, data);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_READ_DHCP_HOST_NAME_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_WriteDhcpHostNameOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_SoAd_3206 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  uint8 length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) data)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx;
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check plausibility of input parameters. */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else if ( !SoAd_IsSoConIdValid(SoConId) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( data == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check if component is initialized. */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      /* #30 Handle write DHCP hostname option request. */
      soConIdx = SoAd_SockConId2SockConIdx(SoConId);

      retVal = SoAd_TcpIp_WriteDhcpHostNameOption(soConIdx, length, data);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_WRITE_DHCP_HOST_NAME_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}

#if ( SOAD_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  if ( versioninfo == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
# endif /* SOAD_DEV_ERROR_DETECT == STD_ON  */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (SOAD_VENDOR_ID);
    versioninfo->moduleID = (SOAD_MODULE_ID);
    versioninfo->sw_major_version = (SOAD_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (SOAD_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (SOAD_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_VERSION_INFO, errorId);
  }
# else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* SOAD_DEV_ERROR_DETECT == STD_ON  */
}
#endif /* SOAD_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  SoAd_MainFunctionRx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 3408 MSR_88 */ /* MD_MSR_8.8 */
FUNC(void, SOAD_CODE) SoAd_MainFunctionRx(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    if ( SoAd_State == SOAD_STATE_INIT )
    {
      SoAd_TcpIp_MainFunctionRx(); /*lint !e522 */

# if ( SOAD_VSOCKET_ROUTES == STD_ON )
      if ( SoAd_IsEventQueueIdxTpRxSoConPresent() )
      { /* handle TP reception */
        SoAd_HandleTpReception();
      }
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_MAIN_FUNCTION_RX, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_88 */

/**********************************************************************************************************************
 *  SoAd_MainFunctionState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 3408 MSR_88 */ /* MD_MSR_8.8 */
FUNC(void, SOAD_CODE) SoAd_MainFunctionState(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    if ( SoAd_State != SOAD_STATE_SHUTDOWN )
    {
      if ( SoAd_State == SOAD_STATE_SHUTDOWN_IN_PROGRESSS )
      {
        SoAd_HandleShutdown();
      }
      else
      {
        SoAd_TcpIp_MainFunctionState(); /*lint !e522 */
      }

#if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
      /* handle alive supervision timeout */
      if ( SoAd_IsTimeoutListIdxUdpAlivePresent() )
      {
        SoAd_HandleUdpAliveTimeout();
      }
#endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

      /* handle socket connection states */
      SoAd_HandleSoConStates();

      if ( SoAd_State == SOAD_STATE_SHUTDOWN_IN_PROGRESSS )
      {
        SoAd_HandleShutdownFinish();
      }
    }

#if ( (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) )
    /* handle global timer counter */
    /* has to be handled at the end of this main function since timeouts for transmission (nPdu) are set in this
       context which would be overwritten in each call to this main function and no transmission would be performed in
       context of the transmission main function */
    SoAd_GlobalCounter++;
#endif /* (SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON) || (SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_MAIN_FUNCTION_STATE, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_88 */

/**********************************************************************************************************************
 *  SoAd_MainFunctionTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 3408 MSR_88 */ /* MD_MSR_8.8 */
FUNC(void, SOAD_CODE) SoAd_MainFunctionTx(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
    if ( SoAd_IsPduRoutePresent() )
    {
      if ( SoAd_State != SOAD_STATE_SHUTDOWN )
      {
        /* handle TP transmission */
        if ( SoAd_IsEventQueueIdxTpTxSoConPresent() )
        {
          SoAd_HandleTpTransmission();
        }

        SoAd_TcpIp_MainFunctionTx(); /*lint !e522 */

        /* handle IfRoutingGroupTransmit requests */
# if ( SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON )
        if ( SoAd_IsEventQueueIdxIfTxRouteGrpPresent() )
        {
          SoAd_HandleIfRouteGrpTransmit();
        }
# endif /* SOAD_VIF_ROUT_GROUP_TRANSMIT_ENABLED == STD_ON */

        /* UdpTriggerTimeout for nPduUdpTx */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
        if ( SoAd_IsNPduUdpTxPresent() )
        {
          SoAd_HandleNPduUdpTx();
        }
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */

        /* handle pending TxConfirmations */
        if ( SoAd_IsEventQueueIdxIfUdpPduRoutePresent() )
        {
          SoAd_HandleTxConfirmation();
        }
      }
    }
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_MAIN_FUNCTION_TX, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_88 */

#if ( SOAD_VSINGLE_MAIN_FUNCTION_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
# endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* call main function for reception */
    SoAd_MainFunctionRx();

    /* call main function for states */
    SoAd_MainFunctionState();

    /* call main function for transmission */
# if ( SOAD_VPDU_ROUTES == STD_ON )
    SoAd_MainFunctionTx();
# endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_MAIN_FUNCTION, errorId);
  }
# else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
#endif /* SOAD_VSINGLE_MAIN_FUNCTION_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_RxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 3673 SoAd_167 */ /* MD_SoAd_16.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 MSR_STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
/* PRQA S 3206 5 */ /* MD_SoAd_3206 */
FUNC(void, SOAD_CODE) SoAd_RxIndication(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) locBufPtr = BufPtr;
  SoAd_SoConIdxType soConIdx;
  uint16            routeIdx;
  uint16            routeNum;
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  SoAd_SockIdxType  sockIdx = SOAD_INV_SOCK_IDX;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_NOT_OK )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( (RemoteAddrPtr->domain != SOAD_AF_INET)
# if ( SOAD_VIPV6_ENABLED == STD_ON )
    && (RemoteAddrPtr->domain != SOAD_AF_INET6) )
# else
    )
# endif /* SOAD_VIPV6_ENABLED == STD_ON */
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* Check parameter 'BufPtr' */
  else if ( BufPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* Check configuration */
  else if ( !SoAd_IsSockRoutePresent() )
  {
    errorId = SOAD_E_INV_ARG;
  }
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  else
#else
  if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
    /* check whether any socket connection is configured for this socket */
    if ( SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum == 0u )
    {
      return;
    }
    /* get first socket connection identifier */
    soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

    /* verify IP Assignment state */
    if ( SOAD_IPADDR_STATE_ASSIGNED != SoAd_SockConIdx2IpAddrState(soConIdx) )
    {
      return;
    }

# if ( SOAD_VTCP_USED == STD_ON )
    /* validate socket state */
    if ( (SOAD_SOCK_STATE_ESTABLISHED != SoAd_SockIdx2SockState(sockIdx)) &&
      (SOAD_SOCK_STATE_LISTEN != SoAd_SockIdx2SockState(sockIdx)) )
    { /* socket is not in expected mode */
      return;
    }
# endif /* SOAD_VTCP_USED == STD_ON */

    /* validate length */
    if ( 0u == Length )
    { /* no data received */
      return;
    }

    /* get corresponding socket connection index */
# if ( SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON )
    /* check whether the best match algorithm considers the PDU Header for this socket */
    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->BestMatchWithPduHeaderEnabled != TRUE)
# endif /* SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON */
    { /* use the best match algorithm according to AUTOSAR */
      if ( SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum > 1u )
      { /* more than one socket connection is configured for this socket - use the best match algorithm */
        SoAd_SoConIdxType soConIdxStart = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
        SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdxType)(soConIdxStart +
          SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum - 1u);

        if ( E_OK != SoAd_GetSoConIdByBestMatchAlg(RemoteAddrPtr, soConIdxStart, soConIdxMax,
            SOAD_SOCON_FILTER_DISABLED,
# if ( SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED == STD_ON )
            SoAd_SockCons(soConIdx).SockConGrpPtr->BestMatchWithSockRouteEnabled,
# else
            FALSE,
# endif /* SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED == STD_ON */
            &soConIdx) )
        { /* no suitable socket connection found */
# if ( SOAD_VTCP_USED == STD_ON )
          if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->Protocol ==
            SOAD_IPPROTO_TCP )
          { /* socket is TCP */
            SoAd_TcpIp_TcpReceived(SocketId, Length);
          }
# endif /* SOAD_VTCP_USED == STD_ON */
          return;
        }
      }

      /* get all affected socket routes */
      routeIdx = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxStart;
      routeNum = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxNum;

      if ( 0u == routeNum )
      { /* no socket route configured for this socket connection */
        /* abort reception */
# if ( SOAD_VTCP_USED == STD_ON )
        if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->Protocol ==
          SOAD_IPPROTO_TCP )
        { /* socket is TCP */
          SoAd_TcpIp_TcpReceived(SocketId, Length);
        }
# endif /* SOAD_VTCP_USED == STD_ON */
        return;
      }

      /* validate transmission/reception state */
      if ( SOAD_CLOSE_NONE != SoAd_SockConIdx2CloseMode(soConIdx) )
      { /* transmission/reception is disabled (by a previous socket connection close request ) */
# if ( SOAD_VTCP_USED == STD_ON )
        if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->Protocol ==
          SOAD_IPPROTO_TCP )
        { /* socket is TCP */
          SoAd_TcpIp_TcpReceived(SocketId, Length);
        }
# endif /* SOAD_VTCP_USED == STD_ON */
        return;
      }

      /* use message acceptance policy if configured */
      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE )
      {
        if ( E_OK != SoAd_Util_CompareSockAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(soConIdx),
          RemoteAddrPtr, TRUE) )
        { /* message is not accepted */
# if ( SOAD_VTCP_USED == STD_ON )
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
          { /* socket is TCP */
            SoAd_TcpIp_TcpReceived(SocketId, Length);
          }
# endif /* SOAD_VTCP_USED == STD_ON */
          return;
        }
      }

      /* validate socket connection state */
      if ( SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(soConIdx) )
      { /* socket connection is not online - this is only allowed in specific cases */
        Std_ReturnType retVal = E_NOT_OK;

        /* check whether specific case is applicable */
        if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP) &&
          (SoAd_SockCons(soConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE) )
        { /* socket connection is UDP and message acceptance filter is enabled */
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->UdpListenOnly == FALSE )
          { /* UDP listen only not enabled */
            if ( (0u < (SoAd_SockConIdx2RemAddrState(soConIdx) & SOAD_SOCON_MASK_ANY)) &&
              (0u == (SoAd_SockConIdx2RemAddrState(soConIdx) & SOAD_SOCON_MASK_NOT)) )
            { /* remote address is set but contains wildcards */
              retVal = E_OK;
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
              if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == FALSE )
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
              { /* PDU header disabled means that only one SocketRouteDest(TP) is possible for socket connection */
                if ( SOAD_UL_TYPE_TP == SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer )
                { /* TP SocketRouteDest */
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON )
                  /* check if routing group is enabled */
                  if ( TRUE != SoAd_CheckAnyRoutGrpOnSockRouteDestEnabled(routeIdx) )
                  { /* routing group is not enabled - do not open socket connection */
                    retVal = E_NOT_OK;
                  }
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
                }
              }
            }
            if ( E_OK == retVal )
            {
              /* open socket connection and update remote address */
              SoAd_CopyIpAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(soConIdx), RemoteAddrPtr, FALSE);
              SoAd_SockConIdx2RemAddrState(soConIdx) = SOAD_SOCON_IP_SET_PORT_SET;
              SoAd_SockConIdx2State(soConIdx) = SOAD_SOCON_ONLINE;
# if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
              /* check if an alive timeout is configured */
              if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt != 0u )
              {
                /* return value has not to be considered since this timeout is always sufficient */
                (void)SoAd_TimeoutList_SetElement(SoAd_TimeoutListIdxUdpAlive(), soConIdx,
                  SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr->AliveTimeoutMaxCnt, SoAd_GlobalCounter);
              }
# endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */
              /* inform upper layer about socket connection state change */
              SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).SoConChgFuncBitPattern,
                SOAD_CHG_FUNC_TYPE_SOCONMODE, SOAD_SOCON_ONLINE);
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
              if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == FALSE )
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
              { /* PDU header disabled means that only one SocketRouteDest(TP) is possible for socket connection */
                if ( SOAD_UL_TYPE_TP == SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer )
                { /* TP without Pdu header option */
                  PduLengthType dummyLen = 0u;

                  /* call StartOfReception */
                  if ( BUFREQ_OK != SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                      SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                      0u,
                      &dummyLen) )
                  {
                    retVal = E_NOT_OK;
                    SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                    /* set event to handle socket connection state in main function */
                    SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                  }
                  else
                  {
                    SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = routeIdx;
                  }
                }
              }
            }
          }
        }
        if ( E_OK != retVal )
        { /* reception on this socket connection while socket connection is not online is not valid */
          return;
        }
      }
# if ( SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON )
      else
      {
        /* update alive timeout if configured */
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->SocketUdpPtr != NULL_PTR )
        {
          SoAd_SoCon_UpdateUdpAliveTimeout(soConIdx);
        }
      }
# endif /* SOAD_VUDP_ALIVE_TIMEOUT_ENALBED == STD_ON */

# if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
      /* update received remote address */
      if ( NULL_PTR != SoAd_SockCons(soConIdx).RcvRemSockPtr )
      { /* get received remote address is supported */
        SOAD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        /* copy remote address to local buffer */
        SoAd_CopyIpAddr(SoAd_SockCons(soConIdx).RcvRemSockPtr, RemoteAddrPtr, TRUE);
        SOAD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3355, 3358 */ /* MD_SoAd_13.7_SEC */
      }
# endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */

      /* skip PDU if invalid PDU was received */
      if ( 0u < SoAd_SockIdx2SkipBytes(sockIdx) )
      {
        if ( Length < SoAd_SockIdx2SkipBytes(sockIdx) )
        {
          SoAd_SockIdx2SkipBytes(sockIdx) -= Length;
# if ( SOAD_VTCP_USED == STD_ON )
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
          { /* socket is TCP */
            SoAd_TcpIp_TcpReceived(SocketId, Length);
          }
# endif /* SOAD_VTCP_USED == STD_ON */
          return;
        }
        else
        {
          Length -= (uint16)SoAd_SockIdx2SkipBytes(sockIdx);
          locBufPtr = &BufPtr[SoAd_SockIdx2SkipBytes(sockIdx)];
          SoAd_SockIdx2SkipBytes(sockIdx) = 0u;
        }
      }
    }

# if ( SOAD_VTCP_USED == STD_ON )
    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
    { /* TCP socket */
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
      { /* PDU Header enabled */
        SoAd_TcpPduHdrRxIndication(soConIdx, locBufPtr, Length);
      }
      else
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
      { /* PDU Header disabled */
        SoAd_TcpRxIndication(soConIdx, locBufPtr, Length);
      }
    }
    else
# endif /* SOAD_VTCP_USED == STD_ON */
    { /* UDP socket */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
      {
        SoAd_UdpPduHdrRxIndication(soConIdx, RemoteAddrPtr, locBufPtr, Length);
      }
      else
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
      {
        SoAd_UdpRxIndication(soConIdx, locBufPtr, Length);
      }
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(SocketId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(RemoteAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(BufPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(Length); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_RX_INDICATION, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_147 */
/* PRQA L:SoAd_167 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STCAL */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData(
  SoAd_SocketIdType SocketId,
  uint8* BufPtr,
#if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
  uint16* BufLengthPtr
#else
  uint16 BufLength
#endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
         )
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_SockIdxType  sockIdx = SOAD_INV_SOCK_IDX;
  SoAd_SoConIdxType soConIdx;
  SoAd_PduIdxType   txPduIdx;
  PduLengthType     bytesCopied;
  uint16            bufLength;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_NOT_OK )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  /* Check parameter 'BufPtr' */
  else if ( BufPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
  /* Check parameter 'BufLengthPtr' */
  else if ( BufLengthPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( *BufLengthPtr == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
# else
  /* Check parameter 'BufLength' */
  else if ( BufLength == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* Check configuration */
  else if ( !SoAd_IsPduRoutePresent() )
  {
    errorId = SOAD_E_INV_ARG;
  }
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  else
#else
  if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
    bufLength = *BufLengthPtr;
# else
    bufLength = BufLength;
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */

    /* get socket connection */
    soConIdx = SoAd_SockIdx2ActiveSoConIdx(sockIdx);
    if ( SOAD_INV_SO_CON_ID == soConIdx )
    { /* no transmission is active (TP session may be active but no preparation for transmission are made) */
      return BUFREQ_E_NOT_OK;
    }

    /* check socket connection states */
    if ( (SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(soConIdx)) ||
      ( SOAD_CLOSE_NONE != SoAd_SockConIdx2CloseMode(soConIdx)) )
    { /* socket connection is not online or requested to be closed */
      return BUFREQ_E_NOT_OK;
    }

    txPduIdx = SoAd_SockConIdx2TxMgt(soConIdx).TxPduIdx;
    bytesCopied = SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen - SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending;

    if ( bufLength <= SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending )
    { /* requested buffer length is valid */
# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
      if ( SoAd_IsSockConIdx2TxBufferPresent()
        && (NULL_PTR != SoAd_SockConIdx2TxBuffer(soConIdx).TpTxBufferOffsetPtr)
        && (0u != *SoAd_SockConIdx2TxBuffer(soConIdx).TpTxBufferOffsetPtr) )
      { /* transmission of UDP TpTxBuffer */
        /* copy data to provided buffer */
        IpBase_Copy((IpBase_CopyDataType*)&BufPtr[0],
          (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
            &SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr[bytesCopied],
          bufLength);
        SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= (PduLengthType)bufLength;
        retVal = BUFREQ_OK;
      }
      else
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
      if ( SoAd_IsNPduUdpTxPresent() && (SOAD_INV_BUF_IDX_16 != SoAd_SockConIdx2NPduUdpTxIdx(soConIdx)) )
      { /* transmission of UDP nPdu */
        uint16 nPduUdpTxIdx = SoAd_SockConIdx2NPduUdpTxIdx(soConIdx);

#  if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
        if ( SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr != NULL_PTR )
        { /* nPduUdpTxQueue */
          uint8 idx;

          retVal = BUFREQ_OK;

          for ( idx = 0u; idx < SoAd_NPduUdpTx(nPduUdpTxIdx).QueueMgtPtr->Lvl; idx++ )
          {
            txPduIdx = SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].TxPduIdx;
            if ( txPduIdx != SOAD_INV_PDU_IDX )
            {
#   if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
              if ( SoAd_TxPduIdx2PduRoute(txPduIdx).IfTriggerTransmitEnabled == FALSE )
              { /* PDU is located in nPdu buffer */
                if ( bufLength >= SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len )
                { /* provided buffer is sufficient for this PDU */
#    if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
                  IpBase_Copy((IpBase_CopyDataType*)&BufPtr[*BufLengthPtr - bufLength],
                    (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
                      &SoAd_NPduUdpTx(nPduUdpTxIdx).BufPtr[SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].BufIdx],
                    SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len);
#    else
                  IpBase_Copy((IpBase_CopyDataType*)&BufPtr[BufLength - bufLength],
                    (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
                      &SoAd_NPduUdpTx(nPduUdpTxIdx).BufPtr[SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].BufIdx],
                    SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len);
#    endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */

                  bufLength -= (uint16)SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len;
                }
                else
                { /* provided buffer is not sufficient for this PDU anymore */
                  break;
                }
              }
              else
#   endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
              { /* data has to be retrieved via trigger transmit */
                PduInfoType info;

#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
                if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == FALSE) ||
                  ((SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len - SOAD_PDU_HDR_SIZE) != 0u) )
                {
                  /* retrieve PDU from upper layer */
                  if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
                  {
                    if ( bufLength >= SOAD_PDU_HDR_SIZE )
                    {
#    if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
                      info.SduDataPtr = &BufPtr[*BufLengthPtr - bufLength + SOAD_PDU_HDR_SIZE];
#    else
                      info.SduDataPtr = &BufPtr[BufLength - bufLength + SOAD_PDU_HDR_SIZE];
#    endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
                      info.SduLength = (PduLengthType)(bufLength - SOAD_PDU_HDR_SIZE);
                    }
                    else
                    { /* provided buffer is not sufficient for this PDU anymore */
                      break;
                    }
                  }
                  else
#   else
                {
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
                  {
#   if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
                    info.SduDataPtr = &BufPtr[*BufLengthPtr - bufLength];
#   else
                    info.SduDataPtr = &BufPtr[BufLength - bufLength];
#   endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
                    info.SduLength = (PduLengthType)bufLength;
                  }

                  if ( SoAd_VCallIfTriggerTransmit(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                    SoAd_TxPduIdx2TxConfPduId(txPduIdx), &info) == E_OK )
                  {
#   if ( SOAD_VTX_DYN_LEN_ENABLED == STD_OFF )
                    PduLengthType expectedLen = SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Len;

#    if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
                    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
                    {
                      expectedLen -= SOAD_PDU_HDR_SIZE;
                    }
#    endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
                    if ( info.SduLength != expectedLen )
                    { /* upper layer copied invalid length */
                      retVal = BUFREQ_E_NOT_OK;
                      break;
                    }
#   else
                    if ( info.SduLength > bufLength )
                    { /* upper layer copied invalid length */
                      retVal = BUFREQ_E_NOT_OK;
                      break;
                    }
#   endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_OFF */

#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
                    /* handle PDU header if configured */
                    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
                    {
                      uint16 destIdx = 0u;

                      /* find corresponding PduRouteDest */
                      while ( SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListSize > destIdx )
                      {
                        if ( SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[destIdx]->SoConIdx == soConIdx )
                        { /* PduRouteDest found */
                          /* Copy PDU Header to buffer */
#    if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
                          SoAd_CopyPduHdr2Buf(
                            SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[destIdx]->TxPduHdrId,
                            info.SduLength,
                            &BufPtr[*BufLengthPtr - bufLength],
                            0,
                            SOAD_PDU_HDR_SIZE);
#    else
                          SoAd_CopyPduHdr2Buf(
                            SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[destIdx]->TxPduHdrId,
                            info.SduLength - SOAD_PDU_HDR_SIZE,
                            &BufPtr[BufLength - bufLength],
                            0,
                            SOAD_PDU_HDR_SIZE);
#    endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
                          bufLength -= SOAD_PDU_HDR_SIZE;
                          break;
                        }
                        destIdx++;
                      }
                    }
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
                    bufLength -= (uint16)info.SduLength;
                  }
                  else
                  { /* provided buffer is not sufficient for this PDU anymore */
#   if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
                    if ( *BufLengthPtr == bufLength )
#   else
                    if ( BufLength == bufLength )
#   endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
                    { /* no data has been copied */
                      retVal = BUFREQ_E_NOT_OK;
                    }
                    break;
                  }
                }
              }

              SoAd_NPduUdpTx(nPduUdpTxIdx).QueuePtr[idx].Copied = TRUE;
            }
          }
#   if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
          *BufLengthPtr -= bufLength;
          SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= *BufLengthPtr;
#   else
          SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= (BufLength - bufLength);
#   endif /* SOAD_VTX_DYN_LEN_ENABLED = STD_ON */
        }
#   if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
        else
#   endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
#  endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
#  if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
        { /* nPduUdpTxBuffer */
          /* copy data to provided buffer */
          IpBase_Copy((IpBase_CopyDataType*)&BufPtr[0],
            (P2CONST(IpBase_CopyDataType, AUTOMATIC, SOAD_APPL_DATA))
              &SoAd_NPduUdpTx(nPduUdpTxIdx).BufPtr[0],
            bufLength);
          SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending = 0u;
          retVal = BUFREQ_OK;
        }
#  endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
      }
      else
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
      { /* transmission of single PDU */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
        /* handle PDU header if configured */
        if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
        {
          uint16 destIdx = 0u;

          if ( bytesCopied < SOAD_PDU_HDR_SIZE )
          {
            /* find corresponding PduRouteDest */
            while ( SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListSize > destIdx )
            {
              if ( SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[destIdx]->SoConIdx == soConIdx )
              { /* PduRouteDest found */
                PduLengthType bytesToCopy = SOAD_PDU_HDR_SIZE - bytesCopied;

                if ( bytesToCopy > bufLength )
                {
                  bytesToCopy = bufLength;
                }
                /* Copy PDU Header to buffer */
                SoAd_CopyPduHdr2Buf(SoAd_TxPduIdx2PduRoute(txPduIdx).PduRouteDestListPtr[destIdx]->TxPduHdrId,
                  (SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen - SOAD_PDU_HDR_SIZE),
                  BufPtr, bytesCopied, bytesToCopy);
                SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= bytesToCopy;
                bufLength -= (uint16)bytesToCopy;
                bytesCopied += bytesToCopy;
                break;
              }
              destIdx++;
            }
          }
        }
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

        if ( SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayer == SOAD_UL_TYPE_IF)
        { /* IF transmission */
# if ( SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON )
          if ( (SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP) &&
            (SoAd_TxPduIdx2PduRoute(txPduIdx).IfTriggerTransmitEnabled == TRUE) )
          { /* trigger transmit API is called in this context for UDP socket connections */
            PduInfoType info;

            info.SduLength = (PduLengthType)bufLength;
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
            if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
            {
              info.SduDataPtr = &BufPtr[SOAD_PDU_HDR_SIZE];
            }
            else
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
            {
              info.SduDataPtr = BufPtr;
            }

            if ( SoAd_VCallIfTriggerTransmit(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx), &info) == E_OK)
            {
#  if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
              uint8 minLen = 1u;

#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
              if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
              {
                minLen = 0u;
              }
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

              if ( (info.SduDataPtr != NULL_PTR) && (info.SduLength >= minLen) &&
                (info.SduLength <= bufLength) )
#  else
              if ( (info.SduDataPtr != NULL_PTR) && (info.SduLength == bufLength) )
#  endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
              { /* copied valid length */
                /* update tx management struct */
                SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending = 0u;
#  if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
                if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
                {
                  if ( info.SduLength != (SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen - SOAD_PDU_HDR_SIZE) )
                  { /* length has been updated on trigger transmit call */
                    SoAd_UpdatePduHdrLen(info.SduLength, BufPtr);
                  }
                  SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen = info.SduLength + SOAD_PDU_HDR_SIZE;
                }
                else
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
                {
                  SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen = info.SduLength;
                }
                /* return updated length */
                *BufLengthPtr = (uint16)SoAd_SockConIdx2TxMgt(soConIdx).TxBytesLen;
#  endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
                retVal = BUFREQ_OK;
              }
            }
          }
          else
# endif /* SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON */
          {
            /* copy data to provided buffer */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
            if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == TRUE )
            {
#  if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
              /* PRQA S 3382 4 */ /* MD_SoAd_21.1 */
              IpBase_Copy((IpBase_CopyDataType*)&BufPtr[(uint16)(*BufLengthPtr - bufLength)],
                (IpBase_CopyDataType*)&SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr[
                  (uint16)(bytesCopied - SOAD_PDU_HDR_SIZE)],
                bufLength);
#  else
              /* PRQA S 3382 4 */ /* MD_SoAd_21.1 */
              IpBase_Copy((IpBase_CopyDataType*)&BufPtr[(uint16)(BufLength - bufLength)],
                (IpBase_CopyDataType*)&SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr[
                  (uint16)(bytesCopied - SOAD_PDU_HDR_SIZE)],
                bufLength);
#  endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
            }
            else
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
            {
              IpBase_Copy((IpBase_CopyDataType*)&BufPtr[0],
                (IpBase_CopyDataType*)&SoAd_SockConIdx2TxMgt(soConIdx).TxBufPtr[bytesCopied],
                bufLength);
            }

            SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= (PduLengthType)bufLength;
            retVal = BUFREQ_OK;
          }
        }
        else
        { /* TP transmission */
          PduInfoType pduInfo;
          PduLengthType availableBufLength = 0u;

# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
          pduInfo.SduDataPtr = &BufPtr[*BufLengthPtr - bufLength];
# else
          pduInfo.SduDataPtr = &BufPtr[BufLength - bufLength];
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
          pduInfo.SduLength = (PduLengthType)bufLength;

          /* forward copy request to upper layer */
          if ( BUFREQ_OK == SoAd_VCallTpCopyTxData(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
              SoAd_TxPduIdx2TxConfPduId(txPduIdx), &pduInfo, &availableBufLength) )
          { /* copy request successful */
            SoAd_SockConIdx2TxMgt(soConIdx).TxBytesPending -= (PduLengthType)bufLength;
            retVal = BUFREQ_OK;
          }
        }
      }
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(SocketId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(BufPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
    SOAD_DUMMY_STATEMENT_VAR(BufLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# else
    SOAD_DUMMY_STATEMENT_VAR(BufLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_COPY_TX_DATA, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 0771 SoAd_146 */ /* MD_SoAd_14.6 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_TxConfirmation(
  SoAd_SocketIdType SocketId,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
  SoAd_SockIdxType  sockIdx = SOAD_INV_SOCK_IDX;
  SoAd_SoConIdxType soConIdx;
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'Length' */
  else if ( Length == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* Check parameter 'SocketId' */
  else if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_NOT_OK )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  /* Check configuration */
  else if ( !SoAd_IsPduRoutePresent() )
  {
    errorId = SOAD_E_INV_ARG;
  }
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  else
#else
  if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VPDU_ROUTES == STD_ON )
# if ( SOAD_VTCP_USED == STD_OFF )
    /* length parameter not needed */
    SOAD_DUMMY_STATEMENT_VAR(Length); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* SOAD_VTCP_USED == STD_OFF */
    /* check socket state */
    if ( (SOAD_SOCK_STATE_ESTABLISHED != SoAd_SockIdx2SockState(sockIdx)) )
    {
      return;
    }

    /* get soConId */
    soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

    /* check socket connection state */
    if ( SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(soConIdx) )
    {
      return;
    }

    /* check close mode */
    if ( SOAD_CLOSE_NONE != SoAd_SockConIdx2CloseMode(soConIdx) )
    {
      return;
    }

# if ( SOAD_VTCP_USED == STD_ON )
    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
    { /* TCP socket */
      P2CONST(SoAd_TcpTxQueueConfigType, AUTOMATIC, SOAD_APPL_VAR) qPtr = SoAd_SockCons(soConIdx).TcpTxQueuePtr;

      while ( 0u < Length )
      { /* iterate over tx queue until all entries are released or length to be released is 0 */
        if ( 0u == qPtr->MgtPtr->Lvl )
        { /* no tx elements left */
          break;
        }
        if ( SOAD_INV_PDU_IDX != qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
        { /* found element */
          if ( 0u < qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len )
          { /* queue entry contains data to be confirmed */
            if ( Length >= qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len )
            { /* PDU is confirmed completely */
              SoAd_PduIdxType txPduIdx = qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx;

              Length -= (uint16)qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len;
              qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len = 0u;
              qPtr->QueuePtr[qPtr->MgtPtr->Idx].TxPduIdx = SOAD_INV_PDU_IDX; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
              qPtr->MgtPtr->Lvl--;
              if ( (qPtr->MgtPtr->Idx + 1u) == qPtr->Size)
              { /* wrap around */
                qPtr->MgtPtr->Idx = 0u;
              }
              else
              { /* no wrap around */
                qPtr->MgtPtr->Idx++;
              }

              SoAd_TcpTxConfirmation(soConIdx, txPduIdx);
            }
            else
            { /* PDU confirmed partly */
              qPtr->QueuePtr[qPtr->MgtPtr->Idx].Len -= (PduLengthType)Length;
              break;
            }
          }
          else
          { /* queue entry has no length (is reserved by current transmission request) */
            break;
          }
        }
      }
    }
#  if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
    else
#  endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
# endif /* SOAD_VTCP_USED == STD_ON */
# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
    { /* UDP socket */
      /* in case TxConfirmation is called within this context only one PduRoute is possible per socket connection */
      /* a TxConfirmation on a socket connection with multiple PduRoutes cannot be assigned to a PduRoute safely */
      SoAd_SoConIdxType soConIdxTmp;
      SoAd_SoConIdxType soConIdxEnd = (SoAd_SoConIdxType)((SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart +
        SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum) - 1u);

      for ( soConIdxTmp = soConIdx; soConIdxTmp <= soConIdxEnd; soConIdxTmp++ )
      {
        if ( SoAd_SockConIdx2PduRouteDestCol(soConIdxTmp).PduRouteDestListSize != 0u )
        {
          if ( SoAd_SockCons(soConIdxTmp).SockConGrpPtr->SocketUdpPtr->ImmedIfTxConf == TRUE )
          { /* immediate TxConfirmation is configured */
            PduIdType txPduId = SoAd_SockConIdx2PduRouteDestCol(soConIdxTmp).PduRouteDestListPtr[0]->TxPduId;
            SoAd_PduIdxType txPduIdx = SoAd_TxPduId2PduRouteIdx(txPduId);

            /* socket connection handling if automatic setup is enabled and wildcards configured */
            SoAd_ReconnectSockCon(soConIdxTmp);

            /* call TxConfirmation */
            if ( TRUE == SoAd_TxPduIdx2PduRoute(txPduIdx).TxConfEnabled )
            { /* TxConfirmation is configured for the corresponding upper layer or PDU */
              SoAd_VCallIfTxConfirmation(SoAd_TxPduIdx2PduRoute(txPduIdx).UpperLayerId,
                SoAd_TxPduIdx2TxConfPduId(txPduIdx));
            }

            /* break since corresponding TxPdu is found */
            break;
          }
        }
      }
    }
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
#else
    SOAD_DUMMY_STATEMENT_VAR(Length); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VPDU_ROUTES == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TX_CONFIRMATION, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_11715 */
/* PRQA L:SoAd_146 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_LocalIpAddrAssignmentChg(
  SoAd_LocalAddrIdType IpAddrId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdxType soConIdx = SoAd_SockConTotal();
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'IpAddrId' */
  else if ( IpAddrId == SOAD_IPADDRID_ANY )
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* Check parameter 'State' */
  else if ( (State != SOAD_IPADDR_STATE_ASSIGNED) &&
    (State != SOAD_IPADDR_STATE_ONHOLD) &&
    (State != SOAD_IPADDR_STATE_UNASSIGNED) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    while ( 0u < soConIdx )
    {
      soConIdx--;

      if ( SoAd_SockCons(soConIdx).SockConGrpPtr->LocIpAddrId == IpAddrId )
      { /* IpAddrId is configured for this socket connection */
        SoAd_SockConIdx2IpAddrState(soConIdx) = State;

        /* forward to consumer */
        SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).IpAddrChgFuncBitPattern,
          SOAD_CHG_FUNC_TYPE_IPASSIGN, State);

        /* set event to handle socket connection state in main function */
        SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_LOCAL_IP_ADDR_ASSIGNMENT_CHG, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TcpAccepted()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 3673 SoAd_167 */ /* MD_SoAd_16.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpAccepted(
  SoAd_SocketIdType SocketId,
  SoAd_SocketIdType SocketIdConnected,
  SoAd_SockAddrType* RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType  sockIdx = SOAD_INV_SOCK_IDX;
#if ( SOAD_VTCP_USED == STD_ON )
  SoAd_SockIdxType  sockIdxConnected = 0u;
  SoAd_SoConIdxType soConIdx;
#endif /* SOAD_VTCP_USED == STD_ON */
  Std_ReturnType    retVal = E_NOT_OK;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_NOT_OK )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->SocketTcpPtr == NULL_PTR )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->SocketTcpPtr->TcpInitiate ==
    TRUE)
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  /* Check parameter 'SocketIdConnected' */
  else if ( SocketIdConnected == SOAD_INV_SOCKET_ID ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else
#else
  if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VTCP_USED == STD_ON )
    if ( SOAD_SOCK_STATE_LISTEN != SoAd_SockIdx2SockState(sockIdx) )
    { /* listen socket is not in listen state */
      return retVal;
    }

    soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

    if ( SoAd_SockCons(soConIdx).SockConGrpPtr->MsgAcceptFilterEnabled == TRUE )
    { /* message acceptance filter is enabled */
      /* use best match algorithm to get corresponding socket connection */
      SoAd_SoConIdxType soConIdxStart = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
      SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdxType)(soConIdxStart +
        SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum - 1u);

      if ( E_OK != SoAd_GetSoConIdByBestMatchAlg(RemoteAddrPtr, soConIdxStart, soConIdxMax, SOAD_SOCON_FILTER_ON_OFF,
        FALSE, &soConIdx) )
      { /* no socket connection found */
        soConIdx = SOAD_INV_SO_CON_IDX;
      }
      else
      { /* socket connection found */
        sockIdxConnected = SoAd_SockCons(soConIdx).SockIdx;
      }
    }
    else
    { /* message acceptance filter is disabled */
      /* one socket connection only */
      if ( SOAD_SOCON_RECONNECT == SoAd_SockConIdx2State(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart) )
      { /* valid socket connection state */
        soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
        sockIdxConnected = SoAd_SockCons(soConIdx).SockIdx;
      }
      else
      { /* invalid socket connection state */
        soConIdx = SOAD_INV_SO_CON_IDX;
      }
    }

    if ( SOAD_INV_SO_CON_IDX != soConIdx )
    { /* found socket connection */
      retVal = E_OK;

      if ( SOAD_SOCK_STATE_CLOSED == SoAd_SockIdx2SockState(sockIdxConnected) )
      { /* valid TcpIp socket state */
        /* assign TcpIp socket */
        SoAd_SockIdx2SocketId(sockIdxConnected) = SocketIdConnected;

        /* update remote address */
        SoAd_CopyIpAddr((SoAd_SockAddrType*)&SoAd_SockConIdx2RemSockDyn(soConIdx), RemoteAddrPtr, TRUE);
        SoAd_SockConIdx2RemAddrState(soConIdx) = SOAD_SOCON_IP_SET_PORT_SET;

        /* set socket and socket connection state */
        SoAd_SockIdx2SockState(sockIdxConnected) = SOAD_SOCK_STATE_ESTABLISHED;
        SoAd_OpenSocks++;
        SoAd_SockConIdx2State(soConIdx) = SOAD_SOCON_ONLINE;

        /* inform upper layer about socket connection state change */
        SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).SoConChgFuncBitPattern,
          SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdx));

# if ( SOAD_VSOCKET_ROUTES == STD_ON )
        if ( SoAd_IsSockRoutePresent() )
        { /* call <Up>_[SoAd][Tp]StartOfReception() with TpSduLength = 0 if PduHeader is disabled and TP-API is used */
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == FALSE )
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
          {
            uint16 routeIdx;
            uint16 routeNum;

            routeIdx = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxStart;
            routeNum = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxNum;

            if ( 0u < routeNum )
            {
              if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_TP )
              {
                PduLengthType dummyLen = 0u;
                /* call StartOfReception */
                if ( BUFREQ_OK != SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                    0u,
                    &dummyLen) )
                { /* close socket connection in next main function again */
                  SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                  retVal = E_NOT_OK;
                }
                else
                {
                  SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = routeIdx;
                }
              }
            }
          }
        }
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */
      }
    }
#else
    SOAD_DUMMY_STATEMENT_VAR(SocketIdConnected); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    SOAD_DUMMY_STATEMENT_VAR(RemoteAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif /* SOAD_VTCP_USED == STD_ON  */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_ACCEPTED, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_147 */
/* PRQA L:SoAd_167 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TcpConnected()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_TcpConnected(
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType  sockIdx = SOAD_INV_SOCK_IDX;
#if ( SOAD_VTCP_USED == STD_ON )
  SoAd_SoConIdxType soConIdx;
#endif /* SOAD_VTCP_USED == STD_ON  */
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_NOT_OK )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->SocketTcpPtr == NULL_PTR )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( SoAd_SockCons(SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart).SockConGrpPtr->SocketTcpPtr->TcpInitiate ==
    FALSE )
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else
#else
  if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_VTCP_USED == STD_ON )
    /* there is only one TCP socket connection per TcpIp socket */
    soConIdx = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

    /* check socket state */
    if ( SOAD_SOCK_STATE_CONNECT != SoAd_SockIdx2SockState(sockIdx) )
    { /* socket has wrong state */
      return;
    }

    /* check whether socket connection is still requested to be opened */
    if ( (0u < SoAd_SockConIdx2FlagCntOpCl(soConIdx)) ||
      (TRUE == SoAd_SockCons(soConIdx).SockConGrpPtr->SockAutoSoConSetup) )
    { /* socket connection is still requested to be opened */
      if ( SOAD_SOCON_ONLINE != SoAd_SockConIdx2State(soConIdx) )
      { /* socket connection is not online */
        /* open socket connection */
        SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_ESTABLISHED;
        SoAd_SockConIdx2State(soConIdx) = SOAD_SOCON_ONLINE;
        /* inform upper layer about socket connection state change */
        SoAd_VCallChgFunc(soConIdx, SoAd_SockCons(soConIdx).SoConChgFuncBitPattern,
          SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdx));

# if ( SOAD_VSOCKET_ROUTES == STD_ON )
        if ( SoAd_IsSockRoutePresent() )
        { /* call <Up>_[SoAd][Tp]StartOfReception() with TpSduLength = 0 if PduHeader is disabled and TP-API is used */
#  if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
          if ( SoAd_SockCons(soConIdx).SockConGrpPtr->PduHdrEnabled == FALSE )
#  endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
          {
            uint16 routeIdx;
            uint16 routeNum;

            routeIdx = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxStart;
            routeNum = SoAd_SockConIdx2SockRouteCol(soConIdx).SockRouteIdxNum;

            while ( 0u < routeNum )
            {
              if ( SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayer == SOAD_UL_TYPE_TP )
              {
                PduLengthType dummyLen = 0u;
                /* call StartOfReception */
                if ( BUFREQ_OK != SoAd_VCallTpStartOfReception(SoAd_SockRoutes(routeIdx).SockRouteDest.UpperLayerId,
                    SoAd_SockRoutes(routeIdx).SockRouteDest.RxPduId,
                    0u,
                    &dummyLen) )
                { /* close socket connection */
                  SoAd_SockConIdx2CloseMode(soConIdx) = SOAD_CLOSE_OFFLINE_RESET;
                  /* set event to handle socket connection state in main function */
                  SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdx);
                }
                else
                {
                  SoAd_SockConIdx2RxMgt(soConIdx).RouteIdx = routeIdx;
                }
              }
              routeNum--;
              routeIdx++;
            }
          }
        }
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */
      }
    }
#endif /* SOAD_VTCP_USED == STD_ON  */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCP_CONNECTED, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_147 */
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  SoAd_TcpIpEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 0715 MSR_11715 */ /* MD_MSR_1.1_715 */
/* PRQA S 2006 MSR_147 */ /* MD_MSR_14.7 */
/* PRQA S 6010 MSR_STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 MSR_STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_TcpIpEvent(
  SoAd_SocketIdType SocketId,
  SoAd_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockIdxType  sockIdx;
  uint8             errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* Check initialization state of the component */
  if ( SoAd_State == SOAD_STATE_UNINIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if ( SocketId == SOAD_INV_SOCKET_ID ) /* PRQA S 0277 */ /* MD_SoAd_3.1 */
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  /* Check parameter 'Event' */
  else if ( (Event != SOAD_TCP_RESET) &&
    (Event != SOAD_TCP_CLOSED) &&
    (Event != SOAD_TCP_FIN_RECEIVED) &&
    (Event != SOAD_UDP_CLOSED) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    if ( SoAd_GetSockIdxBySocketId(SocketId, &sockIdx) == E_OK )
    {
      switch ( Event )
      {
        case SOAD_UDP_CLOSED:
        {
          SoAd_SoConIdType soConIdxFirst = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

          if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->Protocol == SOAD_IPPROTO_UDP )
          { /* UDP socket */
            SoAd_SoConIdxType soConIdxTmp = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
            SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdxType)(soConIdxTmp +
              SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum);
            boolean resetLocPort = TRUE;

            /* remove TcpIp socket assignment */
            if ( SoAd_SockIdx2SockState(sockIdx) != SOAD_SOCK_STATE_CLOSED )
            {
              if ( SoAd_OpenSocks > 0u )
              {
                SoAd_OpenSocks--;
              }
            }
            SoAd_SockIdx2SockState(sockIdx)      = SOAD_SOCK_STATE_CLOSED;
            SoAd_SockIdx2SocketId(sockIdx)       = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
#if ( SOAD_VPDU_ROUTES == STD_ON )
            if ( SoAd_IsPduRoutePresent() )
            {
              SoAd_SockIdx2ActiveSoConIdx(sockIdx)  = SOAD_INV_SO_CON_IDX;
            }
#endif /* SOAD_VPDU_ROUTES == STD_ON */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
            if ( SoAd_IsSockRoutePresent() )
            {
              SoAd_SockIdx2SkipBytes(sockIdx)      = 0u;
            }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

            /* close all socket connections */
            while ( soConIdxTmp < soConIdxMax )
            {
              if ( SOAD_SOCON_OFFLINE != SoAd_SockConIdx2State(soConIdxTmp) )
              { /* socket connection is not offline */
                /* handle socket connection state dependent on close mode */
                switch ( SoAd_SockConIdx2CloseMode(soConIdxTmp) )
                {
                  case SOAD_CLOSE_RECONNECT:
                  { /* set socket connection to reconnect */
                    if ( SOAD_SOCON_RECONNECT != SoAd_SockConIdx2State(soConIdxTmp) )
                    {
                      SoAd_SockConIdx2State(soConIdxTmp) = SOAD_SOCON_RECONNECT;
                      resetLocPort = FALSE;
                      /* reset socket connection, release buffers, terminate active TP sessions */
                      SoAd_ResetSockCon(soConIdxTmp, TRUE, FALSE);
                      /* inform upper layer about socket connection state change */
                      SoAd_VCallChgFunc(soConIdxTmp, SoAd_SockCons(soConIdxTmp).SoConChgFuncBitPattern,
                        SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdxTmp));
                    }
                    break;
                  }
                  case SOAD_CLOSE_OFFLINE_RESET:
                  { /* set socket connection to offline and reset open/close counter */
                    SoAd_SockConIdx2FlagCntOpCl(soConIdxTmp) = 0u;
                    SoAd_SockConIdx2State(soConIdxTmp) = SOAD_SOCON_OFFLINE;
                    /* reset socket connection, release buffers, terminate active TP sessions */
                    SoAd_ResetSockCon(soConIdxTmp, TRUE, FALSE);
                    /* inform upper layer about socket connection state change */
                    SoAd_VCallChgFunc(soConIdxTmp, SoAd_SockCons(soConIdxTmp).SoConChgFuncBitPattern,
                      SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdxTmp));
                    break;
                  }
                  case SOAD_CLOSE_OFFLINE:
                  { /* set socket connection to offline */
                    SoAd_SockConIdx2State(soConIdxTmp) = SOAD_SOCON_OFFLINE;
                    /* reset socket connection, release buffers, terminate active TP sessions */
                    SoAd_ResetSockCon(soConIdxTmp, TRUE, TRUE);
                    /* inform upper layer about socket connection state change */
                    SoAd_VCallChgFunc(soConIdxTmp, SoAd_SockCons(soConIdxTmp).SoConChgFuncBitPattern,
                      SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdxTmp));
                    break;
                  }
                  default:
                  { /* set socket connection to offline */
                    SoAd_SockConIdx2State(soConIdxTmp) = SOAD_SOCON_OFFLINE;
                    /* reset socket connection, release buffers, terminate active TP sessions */
                    SoAd_ResetSockCon(soConIdxTmp, TRUE, FALSE);
                    /* inform upper layer about socket connection state change */
                    SoAd_VCallChgFunc(soConIdxTmp, SoAd_SockCons(soConIdxTmp).SoConChgFuncBitPattern,
                      SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdxTmp));
                    break;
                  }
                }
              }

              /* reset close mode */
              SoAd_SockConIdx2CloseMode(soConIdxTmp) = SOAD_CLOSE_NONE;

              /* set event to handle socket connection state in main function */
              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdxTmp);

              soConIdxTmp++;
            }

            if ( TRUE == resetLocPort )
            { /* reset local port */
              SoAd_SockIdx2LocPortDyn(sockIdx) = SoAd_SockCons(soConIdxFirst).SockConGrpPtr->LocalPort;
            }
          }
          break;
        }
        case SOAD_TCP_RESET:
        case SOAD_TCP_CLOSED:
        {
          SoAd_SoConIdType soConIdxFirst = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

          if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
          { /* TCP socket */
            SoAd_SoConIdxType soConIdxTmp = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;
            SoAd_SoConIdxType soConIdxMax = (SoAd_SoConIdType)(soConIdxTmp +
              SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxNum);
            boolean resetLocPort = TRUE;

            if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->GrpSockIdx == sockIdx )
            { /* listen socket */
              /* remove TcpIp socket assignment */
              SoAd_SockIdx2SockState(sockIdx) = SOAD_SOCK_STATE_CLOSED;
              SoAd_SockIdx2SocketId(sockIdx)  = SOAD_INV_SOCKET_ID;  /* PRQA S 0277 */ /* MD_SoAd_3.1 */
              if ( SoAd_OpenSocks > 0u )
              {
                SoAd_OpenSocks--;
              }
            }
            else
            { /* no listen socket */
              /* remove TcpIp socket assignment */
              if ( SoAd_SockIdx2SockState(sockIdx) != SOAD_SOCK_STATE_CLOSED )
              {
                if ( SoAd_OpenSocks > 0u )
                {
                  SoAd_OpenSocks--;
                }
              }
              SoAd_SockIdx2SockState(sockIdx)      = SOAD_SOCK_STATE_CLOSED;
              SoAd_SockIdx2SocketId(sockIdx)       = SOAD_INV_SOCKET_ID; /* PRQA S 0277 */ /* MD_SoAd_3.1 */
#if ( SOAD_VPDU_ROUTES == STD_ON )
              if ( SoAd_IsPduRoutePresent() )
              {
                SoAd_SockIdx2ActiveSoConIdx(sockIdx)  = SOAD_INV_SO_CON_IDX;
              }
#endif /* SOAD_VPDU_ROUTES == STD_ON */
#if ( SOAD_VSOCKET_ROUTES == STD_ON )
              if ( SoAd_IsSockRoutePresent() )
              {
                SoAd_SockIdx2SkipBytes(sockIdx)      = 0u;
              }
#endif /* SOAD_VSOCKET_ROUTES == STD_ON */

              if ( SOAD_SOCON_ONLINE == SoAd_SockConIdx2State(soConIdxFirst) )
              {
                /* handle socket connection state dependent on close mode */
                switch ( SoAd_SockConIdx2CloseMode(soConIdxFirst) )
                {
                  case SOAD_CLOSE_RECONNECT:
                  { /* set socket connection to reconnect and reset remote address */
                    SoAd_SockConIdx2State(soConIdxFirst) = SOAD_SOCON_RECONNECT;
                    SoAd_ResetSockCon(soConIdxFirst, TRUE, FALSE);
                    break;
                  }
                  case SOAD_CLOSE_NONE:
                  case SOAD_CLOSE_REMOTE:
                  { /* set socket connection to reconnect */
                    SoAd_SockConIdx2State(soConIdxFirst) = SOAD_SOCON_RECONNECT;
                    if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->SocketTcpPtr->TcpInitiate == TRUE )
                    { /* client socket */
                      /* do not reset remote address to prevent that upper layer has to set remote address again */
                      SoAd_ResetSockCon(soConIdxFirst, FALSE, FALSE);
                    }
                    else
                    { /* server socket */
                      SoAd_ResetSockCon(soConIdxFirst, TRUE, FALSE);
                    }
                    break;
                  }
                  case SOAD_CLOSE_OFFLINE_RESET:
                  { /* set socket connection to offline, reset open/close counter and reset remote address */
                    SoAd_SockConIdx2FlagCntOpCl(soConIdxFirst) = 0u;
                    SoAd_SockConIdx2State(soConIdxFirst) = SOAD_SOCON_OFFLINE;
                    SoAd_ResetSockCon(soConIdxFirst, TRUE, FALSE);
                    break;
                  }
                  default:
                  { /* set socket connection to offline and reset remote address */
                    SoAd_SockConIdx2State(soConIdxFirst) = SOAD_SOCON_OFFLINE;
                    SoAd_ResetSockCon(soConIdxFirst, TRUE, TRUE);
                    break;
                  }
                }

                /* inform upper layer about socket connection state change */
                SoAd_VCallChgFunc(soConIdxFirst, SoAd_SockCons(soConIdxFirst).SoConChgFuncBitPattern,
                  SOAD_CHG_FUNC_TYPE_SOCONMODE, SoAd_SockConIdx2State(soConIdxFirst));
              }

              /* set event to handle socket connection state in main function */
              SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdxFirst);

              /* reset close mode */
              SoAd_SockConIdx2CloseMode(soConIdxFirst) = SOAD_CLOSE_NONE;
            }

            /* reset local port if all socket connections and listen socket are offline and closed */
            while ( soConIdxTmp < soConIdxMax )
            {
              if ( SOAD_SOCON_OFFLINE != SoAd_SockConIdx2State(soConIdxTmp) )
              {
                resetLocPort = FALSE;
                break;
              }
              soConIdxTmp++;
            }

            if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->SocketTcpPtr->TcpInitiate == FALSE )
            { /* server socket - listen socket is available */
              if ( SOAD_SOCK_STATE_CLOSED != SoAd_SockIdx2SockState(SoAd_SockCons(soConIdxFirst).
                  SockConGrpPtr->GrpSockIdx) )
              { /* listen socket is not closed */
                resetLocPort = FALSE;
              }
            }

            if ( TRUE == resetLocPort )
            { /* reset local port */
              SoAd_SockIdx2LocPortDyn(sockIdx) = SoAd_SockCons(soConIdxFirst).SockConGrpPtr->LocalPort;
            }
          }
          break;
        }
        default:
        { /* SOAD_TCP_FIN_RECEIVED */
          SoAd_SoConIdType soConIdxFirst = SoAd_SockIdx2SoConIdxCol(sockIdx).SoConIdxStart;

          if ( SoAd_SockCons(soConIdxFirst).SockConGrpPtr->Protocol == SOAD_IPPROTO_TCP )
          { /* TCP socket */
            /* set close mode to "closed by remote" */
            SoAd_SockConIdx2CloseMode(soConIdxFirst) = SOAD_CLOSE_REMOTE;

            /* set event to handle socket connection state in main function */
            SoAd_EventQueue_AddElement(SoAd_EventQueueIdxStateSoCon(), soConIdxFirst);

            /* close TcpIp socket */
            SoAd_TcpIp_Close(SoAd_SockIdx2SocketId(sockIdx), FALSE, sockIdx);
          }
          break;
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TCPIP_EVENT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT_VAR(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
}
/* PRQA L:MSR_11715 */
/* PRQA L:MSR_147 */
/* PRQA L:MSR_STPTH */
/* PRQA L:MSR_STCYC */
/* PRQA L:MSR_STMIF */

#define SOAD_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA L:TYPE_CAST */

/* module specific MISRA deviations:
  MD_SoAd_3.1:
      Reason:     Signed unsigned mismatch for data type independence
      Risk:       Misinterpretation of data.
      Prevention: Covered by code review.
  MD_SoAd_5.6:
      Reason:     Same name used to improve readability.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_6.3:
      Reason:     Platform specific type.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_8.8:
     Reason:      Schedulable entities are declared by the RTE and also by the BSW modules.
     Risk:        No risk.
     Prevention:  Not required.
  MD_SoAd_11.4:
      Reason:     Type cast due to hardware oriented programming
      Risk:       Misinterpretation of data.
      Prevention: Covered by code review.
  MD_SoAd_13.7_CFG:
      Reason:     Condition depends on configuration.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_13.7_SUB:
      Reason:     Value is changed in sub routine.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_13.7_SEC:
      Reason:     Check is done for security reasons.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_14.5:
      Reason:     Continue statement used for readability and performance reasons.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_14.6:
      Reason:     Multiple break statements due to runtime optimization.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_16.7:
      Reason:     No pointer to const due to modification in a called function or required by specification.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_17.4:
      Reason:     Code optimization, array indexing more complex in this case.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_20.5:
      Reason:     errno is a facility of C, but which in practice is poorly defined by the standard.
      Risk:       No risk.
      Prevention: Used with BSD Socket API only.
  MD_SoAd_21.1:
      Reason:     Wraparound past zero or index out of bounds prevented by complex mechanism.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_SoAd_3206:
      Reason:     API parameter is only used in some configurations or is required by specification but currently not
                  supported.
      Risk:       There is no risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: SoAd.c
 *********************************************************************************************************************/
