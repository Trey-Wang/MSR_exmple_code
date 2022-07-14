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
 *         File:  EthFw.c
 *       Module:  SysService_FwEth
 *    Generator:  SysService_FwEth.jar (DaVinci Configurator Pro)
 *
 *  Description:  Implementation of Firewall for Ethernet (ETHFW)
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

#define ETHFW_SOURCE

/*lint -e438 -e550 */ /* Return values can be used to gain debug information */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthFw.h"
#include "EthFw_Priv.h"
#include "IpBase.h"

#if(ETHFW_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* ETHFW_DEV_ERROR_REPORT == STD_ON */

#if(ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif /* ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthFw header file */
#if(   (ETHFW_SW_MAJOR_VERSION != (0x01u)) \
    || (ETHFW_SW_MINOR_VERSION != (0x00u)) \
    || (ETHFW_SW_PATCH_VERSION != (0x01u)) )
# error "Vendor specific version numbers of EthFw.c and EthFw.h are inconsistent"
#endif /* Version Check */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (ETHFW_LOCAL)
# define ETHFW_LOCAL static
#endif /* ETHFW_LOCAL */

#if !defined (ETHFW_LOCAL_INLINE)
# define ETHFW_LOCAL_INLINE LOCAL_INLINE
#endif /* ETHFW_LOCAL_INLINE */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define ETHFW_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
ETHFW_LOCAL VAR(EthFw_StateType, ETHFW_VAR_ZERO_INIT) EthFw_ModuleInitialized = ETHFW_UNINIT;
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */

#define ETHFW_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define ETHFW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(ETHFW_USE_INIT_POINTER == STD_ON)
P2CONST(EthFw_PBConfigType, ETHFW_VAR_ZERO_INIT, ETHFW_PBCFG) EthFw_ConfigDataPtr = NULL_PTR;
#endif

#define ETHFW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHFW_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthFw_IsFrameAllowed()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced frame matches the configured firewall rules and has to be forwarded
 *                  or has to be discarded.
 *  \param[in]      CtrlIdx       Relevant EthIf controller index
 *  \param[in]      FrameHdrPtr   Ethernet frame header information
 *  \param[in]      Direction     Direction of the frame
 *  \param[in]      PayloadPtr    Start pointer of the Ethernet payload
 *  \param[in]      PayloadLen    Length of the referenced payload
 *  \return         TRUE  - Frame is compatible with configured rules.
 *                  FALSE - Frame does not match a rule and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120323, CREQ-120324, CREQ-120325, CREQ-120326
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_IsFrameAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen);

#if(ETHFW_MACADDR == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckMacAddrRanges()
 *********************************************************************************************************************/
/*! \brief          Checks whether the MAC address matches the configured MAC address ranges.
 *  \param[in]      RuleIdx       Index of active rule
 *  \param[in]      FrameHdrPtr   Ethernet frame header information
 *  \param[in]      Direction     Direction of the frame
 *  \return         TRUE  - Frame is compatible with configured MAC address ranges.
 *                  FALSE - Frame does not match MAC address ranges and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120327
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckMacAddrRanges(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHFW_APPL_DATA) FrameHdrPtr,
  EthFw_DirectionType Direction);
#endif

/**********************************************************************************************************************
 *  EthFw_CheckMacAddr()
 *********************************************************************************************************************/
/*! \brief          Checks whether the MAC address is within the referenced range.
 *  \param[in]      FromMacAddrPtr  Start MAC address of range
 *  \param[in]      TillMacAddrPtr  End MAC address of range
 *  \param[in]      MacAddrPtr      MAC address to check
 *  \return         TRUE  - MAC address matches configured rules
 *                  FALSE - MAC address does not match configured rules
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckMacAddr(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) FromMacAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) TillMacAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) MacAddrPtr);

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpRules()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced frame matches the configured IP rules.
 *  \param[in]      RuleIdx         Index of active rule
 *  \param[in]      FrameHeaderPtr  Ethernet frame header information
 *  \param[in]      Direction       Direction of the frame
 *  \param[in]      DataPtr         Start pointer of the Ethernet payload
 *  \param[in]      LenByte         Length of the referenced payload
 *  \return         TRUE  - Frame matches IP rules
 *                  FALSE - Frame does not match configured IP rules
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120329
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpRules(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHFW_APPL_DATA) FrameHeaderPtr,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte);
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpV4AddrRanges()
 *********************************************************************************************************************/
/*! \brief          Checks whether the IPv4 address matches the configured address ranges.
 *  \param[in]      IpRuleIdx         Index of active IP rule
 *  \param[in]      LocalIpAddrPtr    IPv4 local address to compare
 *  \param[in]      RemoteIpAddrPtr   IPv4 remote address to compare
 *  \return         TRUE  - Frame is compatible with configured IPv4 address ranges.
 *                  FALSE - Frame does not match IPv4 address ranges and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpV4AddrRanges(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LocalIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) RemoteIpAddrPtr);
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpV6AddrRanges()
 *********************************************************************************************************************/
/*! \brief          Checks whether the IPv6 address matches the configured address ranges.
 *  \param[in]      IpRuleIdx         Index of active IP rule
 *  \param[in]      LocalIpAddrPtr    IPv6 local address to compare
 *  \param[in]      RemoteIpAddrPtr   IPv6 remote address to compare
 *  \return         TRUE  - Frame is compatible with configured IPv6 address ranges.
 *                  FALSE - Frame does not match IPv6 address ranges and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpV6AddrRanges(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LocalIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) RemoteIpAddrPtr);
#endif

#if(ETHFW_RAWPROTOCOL == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckRawProtocolTypeRules()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced protocol type matches the configured types.
 *  \param[in]      IpRuleIdx   Index of active IP rule
 *  \param[in]      Protocol    IP protocol type to check
 *  \return         TRUE  - Frame is compatible with configured RAW protocol types.
 *                  FALSE - Frame does not match a RAW protocol type and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120330
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckRawProtocolTypeRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_ProtocolType Protocol);
#endif

#if(ETHFW_UDPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckUdpRules()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced frame matches the configured UDP rules.
 *  \param[in]      IpRuleIdx   Index of active IP rule
 *  \param[in]      Direction   Direction of the frame
 *  \param[in]      DataPtr     Start pointer of the IP payload
 *  \param[in]      LenByte     Length of the referenced payload
 *  \return         TRUE  - Frame is compatible with configured UDP rules.
 *                  FALSE - Frame does not match a UDP rule and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120331
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckUdpRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte);
#endif

#if(ETHFW_TCPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckTcpRules()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced frame matches the configured TCP rules.
 *  \param[in]      IpRuleIdx   Index of active IP rule
 *  \param[in]      Direction   Direction of the frame
 *  \param[in]      DataPtr     Start pointer of the IP payload
 *  \param[in]      LenByte     Length of the referenced payload
 *  \return         TRUE  - Frame is compatible with configured TCP rules
 *                  FALSE - Frame does not match a TCP rule and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120332
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckTcpRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte);
#endif

#if(ETHFW_AVBRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckAvbRule()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced frame matches the configured AVB rule.
 *  \param[in]      RuleIdx   Index of active rule
 *  \param[in]      DataPtr   Start pointer of the IP payload
 *  \param[in]      LenByte   Length of the referenced payload
 *  \return         TRUE  - Frame is compatible with configured AVB rule
 *                  FALSE - Frame does not match AVB rule and has to be discarded.
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-120328
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckAvbRule(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte);
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseIpV4Header()
 *********************************************************************************************************************/
/*! \brief          Parse the IPv4 header and check for correctness.
 *  \param[in]      DataPtr     Start pointer of the IP payload
 *  \param[in]      LenByte     Length of the referenced payload
 *  \param[out]     HeaderPtr   IP Header information
 *  \return         none
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(void, ETHFW_CODE) EthFw_ParseIpV4Header(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_IpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) HeaderPtr);
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseIpV6Header()
 *********************************************************************************************************************/
/*! \brief          Parse the IPv6 header and check for correctness.
 *  \param[in]      DataPtr     Start pointer of the IP payload
 *  \param[in]      LenByte     Length of the referenced payload
 *  \param[out]     HeaderPtr   IP Header information
 *  \return         none
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(void, ETHFW_CODE) EthFw_ParseIpV6Header(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_IpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) HeaderPtr);
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIfExtensionHeader()
 *********************************************************************************************************************/
/*! \brief          Checks whether the actual header is an IPv4 extension header and has to be skipped.
 *  \param[in]      DataPtr         Start pointer of the IP payload
 *  \param[in]      Offset          Offset relative to the start of the Ethernet payload
 *  \param[in/out]  NextHeaderPtr   References the next header position
 *  \param[in/out]  LengthPtr       Length of the next header in bytes
 *  \return         E_OK      - Supported next header
 *                  E_NOT_OK  - Next header is not supported
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_CheckIfExtensionHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 Offset,
  P2VAR(uint8, AUTOMATIC, ETHFW_APPL_DATA) NextHeaderPtr,
  P2VAR(uint16, AUTOMATIC, ETHFW_APPL_DATA) LengthPtr);
#endif

#if(ETHFW_UDPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseUdpHeader()
 *********************************************************************************************************************/
/*! \brief          Parse the UDP header and check for correctness.
 *  \param[in]      DataPtr       Start pointer of the UDP payload
 *  \param[in]      LenByte       Length of the referenced payload
 *  \param[out]     UdpHeaderPtr  UDP Header information
 *  \return         E_OK      - Valid UDP header
 *                  E_NOT_OK  - Invalid UDP header
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_ParseUdpHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_UdpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) UdpHeaderPtr);
#endif

#if(ETHFW_TCPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseTcpHeader()
 *********************************************************************************************************************/
/*! \brief          Parse the TCP header and check for correctness.
 *  \param[in]      DataPtr       Start pointer of the TCP payload
 *  \param[in]      LenByte       Length of the referenced payload
 *  \param[out]     TcpHeaderPtr  TCP Header information
 *  \return         E_OK      - Valid TCP header
 *                  E_NOT_OK  - Invalid TCP header
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_ParseTcpHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_TcpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) TcpHeaderPtr);
#endif

/**********************************************************************************************************************
 *  EthFw_CheckByteArrayEqual()
 *********************************************************************************************************************/
/*! \brief          Checks whether two byte arrays are equal within the specified size.
 *  \param[in]      ArrayAPtr   Array A
 *  \param[in]      ArrayBPtr   Array B
 *  \param[in]      Length      Length to check both arrays
 *  \return         TRUE  - Referenced byte arrays are equal
 *                  FALSE - Referenced byte arrays are NOT equal
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckByteArrayEqual(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) ArrayAPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) ArrayBPtr,
  uint8 Length);

/**********************************************************************************************************************
 *  EthFw_CheckByteArrayWithin()
 *********************************************************************************************************************/
/*! \brief          Checks whether the CheckArrayPtr represents a value which is between the values referenced by
 *                  the UpperArrayPtr and LowerArrayPtr.
 *  \param[in]      LowerArrayPtr   Lower array
 *  \param[in]      UpperArrayPtr   Upper array
 *  \param[in]      CheckArrayPtr   Array to check against range
 *  \param[in]      Length          Length to check both arrays
 *  \return         TRUE  - CheckArray lies within LowerArray and UpperArray boarder
 *                  FALSE - Referenced byte arrays are NOT equal
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckByteArrayWithin(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LowerArrayPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) UpperArrayPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) CheckArrayPtr,
  uint8 Length);

#if((ETHFW_UDPRULE == STD_ON) || (ETHFW_TCPRULE == STD_ON))
/**********************************************************************************************************************
 *  EthFw_CheckPortRange()
 *********************************************************************************************************************/
/*! \brief          Checks whether the referenced local and remote ports correspond with the configured port range.
 *  \param[in]      PortRangeIdx  Index of the port range to check
 *  \param[in]      LocalPort     Local port
 *  \param[in]      RemotePort    Remote port
 *  \return         TRUE  - Local and remote port are within the specified range
 *                  FALSE - Referenced ports are not within the specified range
 *  \pre            Module has been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckPortRange(
  EthFw_SizeOfPortRangeType PortRangeIdx,
  uint16 LocalPort,
  uint16 RemotePort);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthFw_IsFrameAllowed()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_IsFrameAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean frameAllowed = FALSE;
  EthFw_SizeOfRuleType ruleIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all configured rules */
  for(ruleIdx = 0; ruleIdx < EthFw_GetSizeOfRule(); ruleIdx++)
  {
    retVal = E_OK;
    frameAllowed = FALSE;

    /* #20 Check if the rule is relevant (CtrlIdx, EtherType, VlanId, Priority) */
    if(EthFw_GetCtrlIdxOfRule(ruleIdx) != CtrlIdx)
    {
      /* Rule specifies a different controller index */
      retVal = E_NOT_OK;
    }
    else if(EthFw_GetEthertypeOfRule(ruleIdx) != FrameHdrPtr->EtherType)
    {
      /* Rule specifies different Ethertype */
      retVal = E_NOT_OK;
    }
    else if(FrameHdrPtr->VlanId == ETHIF_INV_VLAN_ID)
    {
#if(ETHFW_VLANID == STD_ON)
      /* No VLAN present, check if compliant with rule */
      if(EthFw_GetVlanIdIdxOfRule(ruleIdx) != ETHFW_NO_VLANIDIDXOFRULE)
      {
        /* Rule specifies VLAN ID */
        retVal = E_NOT_OK;
      }
#endif
    }
    else
    {
#if(ETHFW_VLANID == STD_ON)
      /* VLAN is present -> check ID */
      if(EthFw_GetVlanIdIdxOfRule(ruleIdx) == ETHFW_NO_VLANIDIDXOFRULE)
      {
        /* Rule specifies no VLAN ID */
        retVal = E_NOT_OK;
      }
      else if(EthFw_GetVlanId(EthFw_GetVlanIdIdxOfRule(ruleIdx)) != FrameHdrPtr->VlanId)
      {
        /* Rule specifies a different VLAN ID */
        retVal = E_NOT_OK;
      }
# if(ETHFW_FRAMEPRIORITY == STD_ON)
      else if(EthFw_GetFramePriorityStartIdxOfRule(ruleIdx) != ETHFW_NO_FRAMEPRIORITYSTARTIDXOFRULE)
      {
        EthFw_FramePriorityIterType prioIdx;
        retVal = E_NOT_OK;

        /* Check specified frame priorities */
        for(prioIdx = EthFw_GetFramePriorityStartIdxOfRule(ruleIdx);
          prioIdx <= EthFw_GetFramePriorityEndIdxOfRule(ruleIdx);
          prioIdx++)
        {
          if(EthFw_GetFramePriority(prioIdx) == FrameHdrPtr->Priority)
          {
            /* Specified frame priority is valid */
            retVal = E_OK;
            break;
          }
        }
      }
# endif
      else
      {
        /* No frame priorities specified. All priorities are allowed. */
      }
#else
      /* No VLAN ID configured. */
      retVal = E_NOT_OK;
#endif
    }

    if(retVal == E_OK)
    {
#if(ETHFW_MACADDR == STD_ON)
      /* #30 Check MAC address ranges */
      frameAllowed = EthFw_CheckMacAddrRanges(ruleIdx, FrameHdrPtr, Direction);

      if(frameAllowed == FALSE)
      {
        /* No MAC address range does match, proceed with the next rule */
        continue; /* PRQA S 0770 */ /* MD_ETHFW_14.5_0770 */
      }
#endif

      /* #40 Check MAC protocol */
#if(ETHFW_IPRULE == STD_ON)
      /* #50 Check if a IP rule is configured */
      if(EthFw_GetIpRuleStartIdxOfRule(ruleIdx) != ETHFW_NO_IPRULESTARTIDXOFRULE)
      {
        /* #51 Check IP rules */
        frameAllowed = EthFw_CheckIpRules(ruleIdx, FrameHdrPtr, Direction, PayloadPtr, PayloadLen);
      }
      else
#endif
#if(ETHFW_AVBRULE == STD_ON)
      /* #60 Check if a AVB rule is configured */
      if(EthFw_GetAvbRuleIdxOfRule(ruleIdx) != ETHFW_NO_AVBRULEIDXOFRULE)
      {
        /* #61 Check AVB rules */
        frameAllowed = EthFw_CheckAvbRule(ruleIdx, PayloadPtr, PayloadLen);
      }
      else
#endif
      {
        /* No checks for unsupported ethertypes. Message will be forwarded because the rule defines the ethertype. */
        frameAllowed = TRUE;
      }
    }

    /* #70 Stop checking further rules if actual rule matches */
    if(frameAllowed == TRUE)
    {
      /* The rule accepted the message */
      break;
    }
  }

#if((ETHFW_MACADDR != STD_ON) && (ETHFW_IPRULE != STD_ON))
  ETHFW_DUMMY_STATEMENT(Direction); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

#if((ETHFW_IPRULE != STD_ON) && (ETHFW_AVBRULE != STD_ON))
  ETHFW_DUMMY_STATEMENT(PayloadPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  ETHFW_DUMMY_STATEMENT(PayloadLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return frameAllowed;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

#if(ETHFW_MACADDR == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckMacAddrRanges()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckMacAddrRanges(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHFW_APPL_DATA) FrameHdrPtr,
  EthFw_DirectionType Direction)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) fromMacAddrPtr;
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) tillMacAddrPtr;
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) macAddrPtr;
  boolean frameAllowed = FALSE;
  boolean allowLocal, allowRemote;
  EthFw_MacAddrRangeIterType macAddrRangeIdx;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check all relevant MAC address ranges */
  for(macAddrRangeIdx = EthFw_GetMacAddrRangeStartIdxOfRule(RuleIdx);
    macAddrRangeIdx < EthFw_GetMacAddrRangeEndIdxOfRule(RuleIdx);
    macAddrRangeIdx++)
  {
    if(EthFw_GetMacAddrLocalFromIdxOfMacAddrRange(macAddrRangeIdx) == ETHFW_NO_MACADDRLOCALFROMIDXOFMACADDRRANGE)
    {
      /* #20 No local address range specified, allow all addresses */
      allowLocal = TRUE;
    }
    else
    {
      /* #30 Check the configured values for the local MAC address range */
      fromMacAddrPtr = &EthFw_GetMacAddr(EthFw_GetMacAddrLocalFromIdxOfMacAddrRange(macAddrRangeIdx))[0];

      if(EthFw_GetMacAddrLocalTillIdxOfMacAddrRange(macAddrRangeIdx) == ETHFW_NO_MACADDRLOCALTILLIDXOFMACADDRRANGE)
      {
        tillMacAddrPtr = NULL_PTR;
      }
      else
      {
        tillMacAddrPtr = &EthFw_GetMacAddr(EthFw_GetMacAddrLocalTillIdxOfMacAddrRange(macAddrRangeIdx))[0];
      }

      if(Direction == ETHFW_DIRECTION_RX)
      {
        macAddrPtr = FrameHdrPtr->DstMacAddrPtr;
      }
      else
      {
        macAddrPtr = FrameHdrPtr->SrcMacAddrPtr;
      }

      allowLocal = EthFw_CheckMacAddr(fromMacAddrPtr, tillMacAddrPtr, macAddrPtr);
    }

    if(allowLocal == TRUE)
    {
      if(EthFw_GetMacAddrRemoteFromIdxOfMacAddrRange(macAddrRangeIdx) == ETHFW_NO_MACADDRREMOTEFROMIDXOFMACADDRRANGE)
      {
        /* #40 No remote address range specified, allow all addresses */
        allowRemote = TRUE;
      }
      else
      {
        /* #50 Check the configured values for the remote MAC address range */
        fromMacAddrPtr = &EthFw_GetMacAddr(EthFw_GetMacAddrRemoteFromIdxOfMacAddrRange(macAddrRangeIdx))[0];

        if(EthFw_GetMacAddrRemoteTillIdxOfMacAddrRange(macAddrRangeIdx) == ETHFW_NO_MACADDRREMOTETILLIDXOFMACADDRRANGE)
        {
          tillMacAddrPtr = NULL_PTR;
        }
        else
        {
          tillMacAddrPtr = &EthFw_GetMacAddr(EthFw_GetMacAddrRemoteTillIdxOfMacAddrRange(macAddrRangeIdx))[0];
        }

        if(Direction == ETHFW_DIRECTION_RX)
        {
          macAddrPtr = FrameHdrPtr->SrcMacAddrPtr;
        }
        else
        {
          macAddrPtr = FrameHdrPtr->DstMacAddrPtr;
        }

        allowRemote = EthFw_CheckMacAddr(fromMacAddrPtr, tillMacAddrPtr, macAddrPtr);
      }

      /* #60 Stop checking MAC address ranges if actual range matches */
      if(allowRemote == TRUE)
      {
        /* MAC addresses matches the configured range */
        frameAllowed = TRUE;
        break;
      }
    }
  }

  return frameAllowed;
}
#endif

/**********************************************************************************************************************
 *  EthFw_CheckMacAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckMacAddr(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) FromMacAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) TillMacAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) MacAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean match;

  /* ----- Implementation ----------------------------------------------- */
  if(TillMacAddrPtr == NULL_PTR)
  {
    /* #10 Check for exact match of MAC address */
    match = EthFw_CheckByteArrayEqual(FromMacAddrPtr, MacAddrPtr, ETHFW_MAC_ADDR_SIZE);
  }
  else
  {
    /* #20 Check if MAC address is within range */
    match = EthFw_CheckByteArrayWithin(FromMacAddrPtr, TillMacAddrPtr, MacAddrPtr, ETHFW_MAC_ADDR_SIZE);
  }

  return match;
}

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpRules()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpRules(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHFW_APPL_DATA) FrameHeaderPtr,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) localIpAddrPtr;
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) remoteIpAddrPtr;
  EthFw_IpHeaderType ipHeader;
  boolean frameAllowed = FALSE;
  EthFw_SizeOfIpRuleType ipRuleIdx;
  EthFw_IpRuleStartIdxOfRuleType ipRuleStartIdx;

  /* ----- Implementation ----------------------------------------------- */
  ipRuleStartIdx = EthFw_GetIpRuleStartIdxOfRule(RuleIdx);
  ipHeader.Family = ETHFW_AF_INVALID;

  /* #10 Parse the IP header */
  if(FrameHeaderPtr->EtherType == ETHFW_ETHERTYPE_IPV4)
  {
    EthFw_ParseIpV4Header(DataPtr, LenByte, &ipHeader);
  }
  else
  {
    EthFw_ParseIpV6Header(DataPtr, LenByte, &ipHeader);
  }

  if(ipHeader.Family != ETHFW_AF_INVALID)
  {
    if(Direction == ETHFW_DIRECTION_RX)
    {
      localIpAddrPtr = &ipHeader.DstAddr[0];
      remoteIpAddrPtr = &ipHeader.SrcAddr[0];
    }
    else
    {
      localIpAddrPtr = &ipHeader.SrcAddr[0];
      remoteIpAddrPtr = &ipHeader.DstAddr[0];
    }

    /* #20 Check all relevant IP rules */
    for(ipRuleIdx = ipRuleStartIdx;
      ipRuleIdx < EthFw_GetIpRuleEndIdxOfRule(RuleIdx);
      ipRuleIdx++)
    {
      /* #30 Check the configured IP address ranges */
      if(FrameHeaderPtr->EtherType == ETHFW_ETHERTYPE_IPV4)
      {
        frameAllowed = EthFw_CheckIpV4AddrRanges(ipRuleIdx, localIpAddrPtr, remoteIpAddrPtr);
      }
      else
      {
        frameAllowed = EthFw_CheckIpV6AddrRanges(ipRuleIdx, localIpAddrPtr, remoteIpAddrPtr);
      }

      if(frameAllowed == TRUE)
      {
        /* #40 Check IP protocol */
        if(EthFw_IsAllowAllProtoOfIpRule(ipRuleIdx))
        {
          /* #41 All IP protocols shall be accepted */
          frameAllowed = TRUE;
        }
#if(ETHFW_TCPRULE == STD_ON)
        else if(ipHeader.Protocol == ETHFW_PROTOCOL_TCP)
        {
          /* #42 Check TCP rules */
          frameAllowed = EthFw_CheckTcpRules(ipRuleIdx, Direction, &DataPtr[ipHeader.PayloadOffset], LenByte - ipHeader.PayloadOffset);
        }
#endif
#if(ETHFW_UDPRULE == STD_ON)
        else if(ipHeader.Protocol == ETHFW_PROTOCOL_UDP)
        {
          /* #43 Check UDP rules */
          frameAllowed = EthFw_CheckUdpRules(ipRuleIdx, Direction, &DataPtr[ipHeader.PayloadOffset], LenByte - ipHeader.PayloadOffset);
        }
#endif
        else
        {
#if(ETHFW_RAWPROTOCOL == STD_ON)
          /* #44 Check RAW protocol types */
          frameAllowed = EthFw_CheckRawProtocolTypeRules(ipRuleIdx, ipHeader.Protocol);
#endif
        }
      }

      /* #50 Stop checking IP rules if actual rule matches */
      if(frameAllowed == TRUE)
      {
        break;
      }
    }
  }

  return frameAllowed;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpV4AddrRanges()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpV4AddrRanges(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LocalIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) RemoteIpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if(ETHFW_IPV4ADDR == STD_ON)
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) fromIpAddrPtr;
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) tillIpAddrPtr;
# endif
  EthFw_IpRangeIterType ipRangeIdx;
  boolean frameAllowed = FALSE;
  boolean allowLocal, allowRemote;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check IPv4 address ranges */
  for(ipRangeIdx = EthFw_GetIpRangeStartIdxOfIpRule(IpRuleIdx);
    ipRangeIdx < EthFw_GetIpRangeEndIdxOfIpRule(IpRuleIdx);
    ipRangeIdx++)
  {
    /* #20 Check local IPv4 address */
    if(EthFw_GetIpV4AddrLocalFromIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV4ADDRLOCALFROMIDXOFIPRANGE)
    {
      allowLocal = TRUE;
    }
    else
    {
# if(ETHFW_IPV4ADDR == STD_ON)
      fromIpAddrPtr = &EthFw_GetIpV4Addr(EthFw_GetIpV4AddrLocalFromIdxOfIpRange(ipRangeIdx))[0];

      if(EthFw_GetIpV4AddrLocalTillIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV4ADDRLOCALTILLIDXOFIPRANGE)
      {
        /* #21 Check for exact match of IPv4 address */
        allowLocal = EthFw_CheckByteArrayEqual(fromIpAddrPtr, LocalIpAddrPtr, ETHFW_IPV4_ADDR_SIZE);
      }
      else
      {
        /* #22 Check if IPv4 address is within range */
        tillIpAddrPtr = &EthFw_GetIpV4Addr(EthFw_GetIpV4AddrLocalTillIdxOfIpRange(ipRangeIdx))[0];

        allowLocal = EthFw_CheckByteArrayWithin(fromIpAddrPtr, tillIpAddrPtr, LocalIpAddrPtr, ETHFW_IPV4_ADDR_SIZE);
      }
# else
      allowLocal = FALSE;
      ETHFW_DUMMY_STATEMENT(LocalIpAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
    }

    if(allowLocal == TRUE)
    {
      /* #30 Check remote IPv4 address */
      if(EthFw_GetIpV4AddrRemoteFromIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV4ADDRREMOTEFROMIDXOFIPRANGE)
      {
        allowRemote = TRUE;
      }
      else
      {
# if(ETHFW_IPV4ADDR == STD_ON)
        fromIpAddrPtr = &EthFw_GetIpV4Addr(EthFw_GetIpV4AddrRemoteFromIdxOfIpRange(ipRangeIdx))[0];

        if(EthFw_GetIpV4AddrRemoteTillIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV4ADDRREMOTETILLIDXOFIPRANGE)
        {
          /* #31 Check for exact match of IPv4 address */
          allowRemote = EthFw_CheckByteArrayEqual(fromIpAddrPtr, RemoteIpAddrPtr, ETHFW_IPV4_ADDR_SIZE);
        }
        else
        {
          /* #32 Check if IPv4 address is within range */
          tillIpAddrPtr = &EthFw_GetIpV4Addr(EthFw_GetIpV4AddrRemoteTillIdxOfIpRange(ipRangeIdx))[0];

          allowRemote = EthFw_CheckByteArrayWithin(fromIpAddrPtr, tillIpAddrPtr, RemoteIpAddrPtr, ETHFW_IPV4_ADDR_SIZE);
        }
# else
        allowRemote = FALSE;
        ETHFW_DUMMY_STATEMENT(RemoteIpAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
      }

      /* #40 Stop checking IPv4 address ranges if actual range matches */
      if(allowRemote == TRUE)
      {
        frameAllowed = TRUE;
        break;
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIpV6AddrRanges()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckIpV6AddrRanges(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LocalIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) RemoteIpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if(ETHFW_IPV6ADDR == STD_ON)
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) fromIpAddrPtr;
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) tillIpAddrPtr;
# endif
  EthFw_IpRangeIterType ipRangeIdx;
  boolean frameAllowed = FALSE;
  boolean allowLocal, allowRemote;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check IPv6 address ranges */
  for(ipRangeIdx = EthFw_GetIpRangeStartIdxOfIpRule(IpRuleIdx);
    ipRangeIdx < EthFw_GetIpRangeEndIdxOfIpRule(IpRuleIdx);
    ipRangeIdx++)
  {
    /* #20 Check local IPv6 address */
    if(EthFw_GetIpV6AddrLocalFromIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV6ADDRLOCALFROMIDXOFIPRANGE)
    {
      allowLocal = TRUE;
    }
    else
    {
# if(ETHFW_IPV6ADDR == STD_ON)
      fromIpAddrPtr = &EthFw_GetIpV6Addr(EthFw_GetIpV6AddrLocalFromIdxOfIpRange(ipRangeIdx))[0];

      if(EthFw_GetIpV6AddrLocalTillIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV6ADDRLOCALTILLIDXOFIPRANGE)
      {
        /* #21 Check for exact match of IPv6 address */
        allowLocal = EthFw_CheckByteArrayEqual(fromIpAddrPtr, LocalIpAddrPtr, ETHFW_IPV6_ADDR_SIZE);
      }
      else
      {
        /* #22 Check if IPv6 address is within range */
        tillIpAddrPtr = &EthFw_GetIpV6Addr(EthFw_GetIpV6AddrLocalTillIdxOfIpRange(ipRangeIdx))[0];

        allowLocal = EthFw_CheckByteArrayWithin(fromIpAddrPtr, tillIpAddrPtr, LocalIpAddrPtr, ETHFW_IPV6_ADDR_SIZE);
      }
# else
      allowLocal = FALSE;
      ETHFW_DUMMY_STATEMENT(LocalIpAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
    }

    if(allowLocal == TRUE)
    {
      /* #30 Check remote IPv6 address */
      if(EthFw_GetIpV6AddrRemoteFromIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV6ADDRREMOTEFROMIDXOFIPRANGE)
      {
        allowRemote = TRUE;
      }
      else
      {
# if(ETHFW_IPV6ADDR == STD_ON)
        fromIpAddrPtr = &EthFw_GetIpV6Addr(EthFw_GetIpV6AddrRemoteFromIdxOfIpRange(ipRangeIdx))[0];

        if(EthFw_GetIpV6AddrRemoteTillIdxOfIpRange(ipRangeIdx) == ETHFW_NO_IPV6ADDRREMOTETILLIDXOFIPRANGE)
        {
          /* #31 Check for exact match of IPv6 address */
          allowRemote = EthFw_CheckByteArrayEqual(fromIpAddrPtr, RemoteIpAddrPtr, ETHFW_IPV6_ADDR_SIZE);
        }
        else
        {
          /* #32 Check if IPv6 address is within range */
          tillIpAddrPtr = &EthFw_GetIpV6Addr(EthFw_GetIpV6AddrRemoteTillIdxOfIpRange(ipRangeIdx))[0];

          allowRemote = EthFw_CheckByteArrayWithin(fromIpAddrPtr, tillIpAddrPtr, RemoteIpAddrPtr, ETHFW_IPV6_ADDR_SIZE);
        }
# else
        allowRemote = FALSE;
        ETHFW_DUMMY_STATEMENT(RemoteIpAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
      }

      /* #40 Stop checking IPv6 address ranges if actual range matches */
      if(allowRemote == TRUE)
      {
        frameAllowed = TRUE;
        break;
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_RAWPROTOCOL == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckRawProtocolTypeRules()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckRawProtocolTypeRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_ProtocolType Protocol)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean frameAllowed = FALSE;
  EthFw_RawProtocolStartIdxOfIpRuleType protocolStartIdx;
  EthFw_RawProtocolIterType protocolIdx;

  /* ----- Implementation ----------------------------------------------- */
  protocolStartIdx = EthFw_GetRawProtocolStartIdxOfIpRule(IpRuleIdx);

  /* #10 Discard frame if no RAW protocol type is configured */
  if(protocolStartIdx != ETHFW_NO_RAWPROTOCOLSTARTIDXOFIPRULE)
  {
    /* #20 Check all configured RAW IP protocol types */
    for(protocolIdx = protocolStartIdx;
      protocolIdx < EthFw_GetRawProtocolEndIdxOfIpRule(IpRuleIdx);
      protocolIdx++)
    {
      /* #30 Stop checking RAW IP protocol types if actual type matches */
      if(EthFw_GetRawProtocol(protocolIdx) == Protocol)
      {
        frameAllowed = TRUE;
        break;
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_UDPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckUdpRules()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckUdpRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthFw_UdpHeaderType UdpHeader;
  EthFw_UdpRuleIdxOfIpRuleType udpRuleIdx;
  EthFw_SizeOfPortRangeType portRangeIdx;
  uint16 localPort, remotePort;
  boolean frameAllowed = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  udpRuleIdx = EthFw_GetUdpRuleIdxOfIpRule(IpRuleIdx);

  /* #10 Discard frame if no UDP rule is configured */
  if(udpRuleIdx == ETHFW_NO_UDPRULEIDXOFIPRULE)
  {
    /* No UDP rules configured. Discard the message. */
  }
  else
  {
    /* #20 Parse UDP header */
    if(EthFw_ParseUdpHeader(DataPtr, LenByte, &UdpHeader) == E_OK)
    {
      if(Direction == ETHFW_DIRECTION_RX)
      {
        localPort = UdpHeader.DstPort;
        remotePort = UdpHeader.SrcPort;
      }
      else
      {
        localPort = UdpHeader.SrcPort;
        remotePort = UdpHeader.DstPort;
      }

      /* #30 Check all configured UDP port ranges */
      for(portRangeIdx = EthFw_GetPortRangeStartIdxOfUdpRule(udpRuleIdx);
        portRangeIdx < EthFw_GetPortRangeEndIdxOfUdpRule(udpRuleIdx);
        portRangeIdx++)
      {
        frameAllowed = EthFw_CheckPortRange(portRangeIdx, localPort, remotePort);

        /* #40 Stop checking UDP port ranges if actual range matches */
        if(frameAllowed == TRUE)
        {
          break;
        }
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_TCPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckTcpRules()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckTcpRules(
  EthFw_SizeOfIpRuleType IpRuleIdx,
  EthFw_DirectionType Direction,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthFw_TcpHeaderType TcpHeader;
  EthFw_TcpRuleIdxOfIpRuleType tcpRuleIdx;
  EthFw_SizeOfPortRangeType portRangeIdx;
  uint16 localPort, remotePort;
  boolean frameAllowed = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  tcpRuleIdx = EthFw_GetTcpRuleIdxOfIpRule(IpRuleIdx);

  /* #10 Discard frame if no TCP rule is configured */
  if(tcpRuleIdx == ETHFW_NO_TCPRULEIDXOFIPRULE)
  {
    /* No TCP rules configured. Discard the message. */
  }
  else
  {
    /* #20 Parse TCP header */
    if(EthFw_ParseTcpHeader(DataPtr, LenByte, &TcpHeader) == E_OK)
    {
      if(Direction == ETHFW_DIRECTION_RX)
      {
        localPort = TcpHeader.DstPort;
        remotePort = TcpHeader.SrcPort;
      }
      else
      {
        localPort = TcpHeader.SrcPort;
        remotePort = TcpHeader.DstPort;
      }

      /* #30 Check all configured TCP port ranges */
      for(portRangeIdx = EthFw_GetPortRangeStartIdxOfTcpRule(tcpRuleIdx);
        portRangeIdx < EthFw_GetPortRangeEndIdxOfTcpRule(tcpRuleIdx);
        portRangeIdx++)
      {
        frameAllowed = EthFw_CheckPortRange(portRangeIdx, localPort, remotePort);

        /* #40 Stop checking TCP port ranges if actual range matches */
        if(frameAllowed == TRUE)
        {
          break;
        }
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_AVBRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckAvbRule()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckAvbRule(
  EthFw_SizeOfRuleType RuleIdx,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean frameAllowed = FALSE;
  EthFw_AvbRuleIdxOfRuleType avbRuleIdx;
  EthFw_AvbStreamIdIterType avbStreamIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  avbRuleIdx = EthFw_GetAvbRuleIdxOfRule(RuleIdx);

  /* #10 Discard frame if too short */
  if(LenByte < (ETHFW_AVB_STREAMID_OFFSET + ETHFW_AVB_STREAMID_SIZE))
  {
    /* Payload does not contain entire AVB stream ID. Discard message. */
  }
  else
  {
    /* #20 Check all configured AVB stream IDs */
    for(avbStreamIdIdx = EthFw_GetAvbStreamIdStartIdxOfAvbRule(avbRuleIdx);
      avbStreamIdIdx < EthFw_GetAvbStreamIdEndIdxOfAvbRule(avbRuleIdx);
      avbStreamIdIdx++)
    {
      /* #30 Stop checking AVB stream IDs if actual ID matches */
      if(EthFw_CheckByteArrayEqual(&EthFw_GetAvbStreamId(avbStreamIdIdx)[0],
        &DataPtr[ETHFW_AVB_STREAMID_OFFSET], ETHFW_AVB_STREAMID_SIZE) == TRUE)
      {
        frameAllowed = TRUE;
        break;
      }
    }
  }

  return frameAllowed;
}
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseIpV4Header()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHFW_LOCAL FUNC(void, ETHFW_CODE) EthFw_ParseIpV4Header(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_IpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) HeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 ihl;

  /* ----- Implementation ----------------------------------------------- */
  HeaderPtr->Family = ETHFW_AF_INVALID;

  /* #10 Check if the payload contains at least the minimal IPv4 header */
  if(LenByte >= ETHFW_IPV4_HEADER_SIZE)
  {
    retVal = E_OK;

    /* #20 Parse relevant header information */
    if((DataPtr[ETHFW_IP_VERSION_OFFSET] >> 4) != ETHFW_IP_VERSION_4)
    {
      retVal = E_NOT_OK;
    }

    ihl = (uint8)(DataPtr[ETHFW_IPV4_IHL_OFFSET] & ETHFW_IPV4_IHL_MASK);
    HeaderPtr->PayloadOffset = ETHFW_GET_LEN32_IN_BYTES(ihl);
    if(HeaderPtr->PayloadOffset < ETHFW_IPV4_HEADER_SIZE)
    {
      retVal = E_NOT_OK;
    }

    IPBASE_GET_UINT16(DataPtr, ETHFW_IPV4_TOTALLENGTH_OFFSET, HeaderPtr->TotalLength);
    if(HeaderPtr->TotalLength < HeaderPtr->PayloadOffset)
    {
      retVal = E_NOT_OK;
    }
    if(HeaderPtr->TotalLength > LenByte)
    {
      retVal = E_NOT_OK;
    }

    HeaderPtr->Protocol = (EthFw_ProtocolType)DataPtr[ETHFW_IPV4_PROTOCOL_OFFSET];
    IpBase_Copy(HeaderPtr->SrcAddr, &DataPtr[ETHFW_IPV4_SRC_ADDR_OFFSET], ETHFW_IPV4_ADDR_SIZE);
    IpBase_Copy(HeaderPtr->DstAddr, &DataPtr[ETHFW_IPV4_DEST_ADDR_OFFSET], ETHFW_IPV4_ADDR_SIZE);

    if(retVal == E_OK)
    {
      HeaderPtr->Family = ETHFW_AF_INET;
    }
  }
}
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseIpV6Header()
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
ETHFW_LOCAL FUNC(void, ETHFW_CODE) EthFw_ParseIpV6Header(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_IpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) HeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint16 extensionHeaderLength = 0, offset;
  uint8 nextHeader;

  /* ----- Implementation ----------------------------------------------- */
  HeaderPtr->Family = ETHFW_AF_INVALID;

  /* #10 Check if the payload contains at least the minimal IPv6 header */
  if(LenByte >= ETHFW_IPV6_HEADER_SIZE)
  {
    retVal = E_OK;

    /* #20 Parse relevant header information */
    if((DataPtr[ETHFW_IP_VERSION_OFFSET] >> 4) != ETHFW_IP_VERSION_6)
    {
      retVal = E_NOT_OK;
    }

    IPBASE_GET_UINT16(DataPtr, ETHFW_IPV6_PAYLOAD_LENGH_OFFSET, HeaderPtr->TotalLength);
    HeaderPtr->TotalLength += ETHFW_IPV6_HEADER_SIZE;
    if(HeaderPtr->TotalLength > LenByte)
    {
      retVal = E_NOT_OK;
    }

    /* #30 Read "Next Header" field of fixed IPv6  header */
    IPBASE_GET_UINT8(DataPtr, ETHFW_IPV6_NEXT_HEADER_OFFSET, nextHeader);

    IpBase_Copy(HeaderPtr->SrcAddr, &DataPtr[ETHFW_IPV6_SRC_ADDR_OFFSET], ETHFW_IPV6_ADDR_SIZE);
    IpBase_Copy(HeaderPtr->DstAddr, &DataPtr[ETHFW_IPV6_DEST_ADDR_OFFSET], ETHFW_IPV6_ADDR_SIZE);

    offset = ETHFW_IPV6_HEADER_SIZE;

    /* #40 Process potential IPv6 extension headers */
    while(((offset + 1) < LenByte) && (retVal == E_OK))
    {
      /* #41 Check if the next header is an extension header */
      retVal |= EthFw_CheckIfExtensionHeader(DataPtr, offset, &nextHeader, &extensionHeaderLength);

      if(extensionHeaderLength == 0)
      {
        /* Next Header is not an extension header */
        break;
      }
      else
      {
        /* Iterate with next extension header */
        offset += extensionHeaderLength;
      }
    }

    HeaderPtr->Protocol = (EthFw_ProtocolType)nextHeader;
    HeaderPtr->PayloadOffset = offset;

    if(HeaderPtr->TotalLength < HeaderPtr->PayloadOffset)
    {
      retVal = E_NOT_OK;
    }

    if(retVal == E_OK)
    {
      HeaderPtr->Family = ETHFW_AF_INET6;
    }
  }
}
#endif

#if(ETHFW_IPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_CheckIfExtensionHeader()
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
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_CheckIfExtensionHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 Offset,
  P2VAR(uint8, AUTOMATIC, ETHFW_APPL_DATA) NextHeaderPtr,
  P2VAR(uint16, AUTOMATIC, ETHFW_APPL_DATA) LengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 length;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check type of header */
  switch(*NextHeaderPtr)
  {
    case ETHFW_PROTOCOL_IPV6_HOP_BY_HOP:
    case ETHFW_PROTOCOL_IPV6_ROUTE:
    case ETHFW_PROTOCOL_IPV6_DST_OPTS:
      /* #11 Read next header and length information */
      IPBASE_GET_UINT8(DataPtr, Offset, *NextHeaderPtr);
      IPBASE_GET_UINT8(DataPtr, Offset + 1, length);
      *LengthPtr = ETHFW_GET_LEN64_IN_BYTES(length + 1);
      break;

    case ETHFW_PROTOCOL_IPV6_FRAG:
      /* #12 Read next header and length information */
      IPBASE_GET_UINT8(DataPtr, Offset, *NextHeaderPtr);
      length = 0;
      *LengthPtr = ETHFW_GET_LEN64_IN_BYTES(length + 1);
      break;

    case ETHFW_PROTOCOL_AUTH:
      /* #13 Read next header and length information */
      IPBASE_GET_UINT8(DataPtr, Offset, *NextHeaderPtr);
      IPBASE_GET_UINT8(DataPtr, Offset + 1, length);
      *LengthPtr = ETHFW_GET_LEN32_IN_BYTES(length + 2);
      break;

    case ETHFW_PROTOCOL_ESP:
      /* #14 Unsupported next header, discard frame */
      *LengthPtr = 0;
      retVal = E_NOT_OK;
      break;

    case ETHFW_PROTOCOL_IPV6_NO_NEXT:
      /* #15 No next header configured */
      *LengthPtr = 0;
      break;

    default:
      /* #16 Next header is no extension header. Proceed with next level protocol.*/
      *LengthPtr = 0;
      break;
  }

  return retVal;
}
#endif

#if(ETHFW_UDPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseUdpHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_ParseUdpHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_UdpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) UdpHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the payload contains the entire UDP header */
  if(LenByte >= ETHFW_UDP_HEADER_SIZE)
  {
    /* #20 Read UDP header information */
    IPBASE_GET_UINT16(DataPtr, ETHFW_UDP_SRC_PORT_OFFSET, UdpHeaderPtr->SrcPort);
    IPBASE_GET_UINT16(DataPtr, ETHFW_UDP_DST_PORT_OFFSET, UdpHeaderPtr->DstPort);
    IPBASE_GET_UINT16(DataPtr, ETHFW_UDP_LENGTH_OFFSET, UdpHeaderPtr->Length);

    /* #30 Check if length is feasible */
    if(UdpHeaderPtr->Length <= LenByte)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif

#if(ETHFW_TCPRULE == STD_ON)
/**********************************************************************************************************************
 *  EthFw_ParseTcpHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHFW_LOCAL FUNC(Std_ReturnType, ETHFW_CODE) EthFw_ParseTcpHeader(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) DataPtr,
  uint16 LenByte,
  P2VAR(EthFw_TcpHeaderType, AUTOMATIC, ETHFW_APPL_DATA) TcpHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the payload contains the entire TCP header */
  if(LenByte >= ETHFW_TCP_HEADER_SIZE)
  {
    /* #20 Read TCP header information */
    IPBASE_GET_UINT16(DataPtr, ETHFW_TCP_SRC_PORT_OFFSET, TcpHeaderPtr->SrcPort);
    IPBASE_GET_UINT16(DataPtr, ETHFW_TCP_DST_PORT_OFFSET, TcpHeaderPtr->DstPort);
    TcpHeaderPtr->DataOffset = DataPtr[ETHFW_TCP_DATA_OFFSET_OFFSET] >> 4;

    /* #30 Check if DataOffset is feasible */
    if(ETHFW_GET_LEN32_IN_BYTES(TcpHeaderPtr->DataOffset) <= LenByte)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 *  EthFw_CheckByteArrayEqual()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckByteArrayEqual(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) ArrayAPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) ArrayBPtr,
  uint8 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean equal = TRUE;
  uint8_least idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for exact match of byte arrays */
  for(idx = 0; idx < Length; idx++)
  {
    if(ArrayAPtr[idx] != ArrayBPtr[idx])
    {
      /* #20 Byte array does not match */
      equal = FALSE;
      break;
    }
  }

  return equal;
}

/**********************************************************************************************************************
 *  EthFw_CheckByteArrayWithin()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckByteArrayWithin(
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) LowerArrayPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) UpperArrayPtr,
  P2CONST(uint8, AUTOMATIC, ETHFW_APPL_DATA) CheckArrayPtr,
  uint8 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean withinBorder = TRUE;
  uint8_least idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the array value is bigger or equal as the lower boarder */
  for(idx = 0; idx < Length; idx++)
  {
    if(CheckArrayPtr[idx] > LowerArrayPtr[idx])
    {
      /* Value bigger as lower boarder, stop Comparison. */
      break;
    }
    else if(CheckArrayPtr[idx] < LowerArrayPtr[idx])
    {
      withinBorder = FALSE;
      break; /* PRQA S 0771 */ /* MD_ETHFW_14.6_0771 */
    }
    else
    {
      /* Continue comparison */
    }
  }

  /* #20 Check if the array value is smaller or equal as the upper boarder */
  for(idx = 0; idx < Length; idx++)
  {
    if(CheckArrayPtr[idx] < UpperArrayPtr[idx])
    {
      /* Value smaller as upper boarder, stop Comparison. */
      break;
    }
    else if(CheckArrayPtr[idx] > UpperArrayPtr[idx])
    {
      withinBorder = FALSE;
      break; /* PRQA S 0771 */ /* MD_ETHFW_14.6_0771 */
    }
    else
    {
      /* Continue comparison */
    }
  }

  return withinBorder;
}

#if((ETHFW_UDPRULE == STD_ON) || (ETHFW_TCPRULE == STD_ON))
/**********************************************************************************************************************
 *  EthFw_CheckPortRange()
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
ETHFW_LOCAL FUNC(boolean, ETHFW_CODE) EthFw_CheckPortRange(
  EthFw_SizeOfPortRangeType PortRangeIdx,
  uint16 LocalPort,
  uint16 RemotePort)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean frameAllowed = FALSE, allowLocal = FALSE, allowRemote = FALSE;
  EthFw_PortLocalFromIdxOfPortRangeType localFromPortIdx;
  EthFw_PortRemoteFromIdxOfPortRangeType remoteFromPortIdx;

  /* ----- Implementation ----------------------------------------------- */
  localFromPortIdx = EthFw_GetPortLocalFromIdxOfPortRange(PortRangeIdx);

  /* #10 Accept local port if no port range is configured */
  if(localFromPortIdx == ETHFW_NO_PORTLOCALFROMIDXOFPORTRANGE)
  {
    allowLocal = TRUE;
  }
# if(ETHFW_PORT == STD_ON)
  else
  {
    /* #20 Check local port range */
    EthFw_PortLocalTillIdxOfPortRangeType tillPortIdx = EthFw_GetPortLocalTillIdxOfPortRange(PortRangeIdx);

    if(tillPortIdx == ETHFW_NO_PORTLOCALTILLIDXOFPORTRANGE)
    {
      /* #21 Check exact match of local port */
      if(EthFw_GetPort(localFromPortIdx) == LocalPort)
      {
        allowLocal = TRUE;
      }
    }
    else
    {
      /* #22 Check if the local port is within the specified range */
      if((EthFw_GetPort(localFromPortIdx) <= LocalPort) && (EthFw_GetPort(tillPortIdx) >= LocalPort))
      {
        allowLocal = TRUE;
      }
    }
  }
# endif

  if(allowLocal == TRUE)
  {
    remoteFromPortIdx = EthFw_GetPortRemoteFromIdxOfPortRange(PortRangeIdx);

    /* #30 Accept remote port if no port range is configured */
    if(remoteFromPortIdx == ETHFW_NO_PORTREMOTEFROMIDXOFPORTRANGE)
    {
      allowRemote = TRUE;
    }
# if(ETHFW_PORT == STD_ON)
    else
    {
      /* #40 Check remote port range */
      EthFw_PortRemoteTillIdxOfPortRangeType tillPortIdx = EthFw_GetPortRemoteTillIdxOfPortRange(PortRangeIdx);

      if(tillPortIdx == ETHFW_NO_PORTREMOTETILLIDXOFPORTRANGE)
      {
        /* #41 Check exact match of remote port */
        if(EthFw_GetPort(remoteFromPortIdx) == RemotePort)
        {
          allowRemote = TRUE;
        }
      }
      else
      {
        /* #42 Check if the remote port is within the specified range */
        if((EthFw_GetPort(remoteFromPortIdx) <= RemotePort) && (EthFw_GetPort(tillPortIdx) >= RemotePort))
        {
          allowRemote = TRUE;
        }
      }
    }
# endif

    if(allowRemote == TRUE)
    {
      /* Ports match the configured range */
      frameAllowed = TRUE;
    }
  }

  return frameAllowed;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthFw_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHFW_CODE) EthFw_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
  EthFw_ModuleInitialized = ETHFW_UNINIT;
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
}

/**********************************************************************************************************************
 *  EthFw_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHFW_CODE) EthFw_Init(
  P2CONST(EthFw_ConfigType, AUTOMATIC, ETHFW_PBCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHFW_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(EthFw_ModuleInitialized == (uint8)ETHFW_INIT)
  {
    errorId = ETHFW_E_ALREADY_INITIALIZED;
  }
  else
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
  {
#if(ETHFW_USE_INIT_POINTER == STD_ON)
    EthFw_ConfigDataPtr = ConfigPtr;
    if(EthFw_ConfigDataPtr == NULL_PTR)
    {
# if(ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
      EcuM_BswErrorHook(ETHFW_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif /* ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON */
      errorId = ETHFW_E_PARAM_CONFIG;
    }
# if (ETHFW_FINALMAGICNUMBER == STD_ON)
    else if(EthFw_GetFinalMagicNumber() != ETHFW_FINAL_MAGIC_NUMBER)
    {
#  if(ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
      EcuM_BswErrorHook(ETHFW_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif /* ETHFW_USE_ECUM_BSW_ERROR_HOOK == STD_ON */
      errorId = ETHFW_E_INIT_FAILED;
    }
# endif /* ETHFW_FINALMAGICNUMBER == STD_ON */
#else
    if(ConfigPtr != NULL_PTR)
    {
      errorId = ETHFW_E_PARAM_CONFIG;
    }
#endif /* ETHFW_USE_INIT_POINTER == STD_ON */
    else
    {
    /* ----- Implementation ----------------------------------------------- */
#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
      EthFw_ModuleInitialized = (uint8)ETHFW_INIT;
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(ETHFW_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if(errorId != ETHFW_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHFW_MODULE_ID, ETHFW_INSTANCE_ID_DET, ETHFW_SID_INIT, errorId);
  }
#else
  ETHFW_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETHFW_DEV_ERROR_REPORT == STD_ON */
}

#if(ETHFW_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  EthFw_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHFW_CODE) EthFw_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHFW_APPL_DATA) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHFW_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(ETHFW_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = ETHFW_E_PARAM_POINTER;
  }
  else
# endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set VersionInfo structure with corresponding macros */
    VersionInfoPtr->vendorID = (ETHFW_VENDOR_ID);
    VersionInfoPtr->moduleID = (ETHFW_MODULE_ID);
    VersionInfoPtr->sw_major_version = (ETHFW_SW_MAJOR_VERSION);
    VersionInfoPtr->sw_minor_version = (ETHFW_SW_MINOR_VERSION);
    VersionInfoPtr->sw_patch_version = (ETHFW_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if(ETHFW_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred */
  if(errorId != ETHFW_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHFW_MODULE_ID, ETHFW_INSTANCE_ID_DET, ETHFW_SID_GET_VERSION_INFO, errorId);
  }
# else
  ETHFW_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* ETHFW_DEV_ERROR_REPORT == STD_ON */
}
#endif /* ETHFW_VERSION_INFO_API */

/**********************************************************************************************************************
 *  EthFw_IsFrameRxAllowed()
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
FUNC(boolean, ETHFW_CODE) EthFw_IsFrameRxAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean frameAllowed = FALSE;
  uint8 errorId = ETHFW_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(EthFw_ModuleInitialized == ETHFW_UNINIT)
  {
    errorId = ETHFW_E_UNINIT;
  }
  else if(FrameHdrPtr == NULL_PTR)
  {
    errorId = ETHFW_E_PARAM_POINTER;
  }
  else if(PayloadPtr == NULL_PTR)
  {
    errorId = ETHFW_E_PARAM_POINTER;
  }
  else
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Analyze received Ethernet frame */
    frameAllowed = EthFw_IsFrameAllowed(CtrlIdx, FrameHdrPtr, ETHFW_DIRECTION_RX, PayloadPtr, PayloadLen);

#if(ETHFW_RXFRAMEDISCARDEDCBKFCTPTR == STD_ON)
    /* #30 Trigger callback if frame was discarded */
    if(frameAllowed == FALSE)
    {
      EthFw_GetRxFrameDiscardedCbkFctPtr()(CtrlIdx, FrameHdrPtr, PayloadPtr, PayloadLen);
    }
#endif /* ETHFW_RXFRAMEDISCARDEDCBKFCTPTR == STD_ON */

#if(!EthFw_IsEnabled())
    /* #40 Indicate frame as valid if module is disabled (EthFw/EthFwGeneral/EthFwEnabled) */
    frameAllowed = TRUE;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(ETHFW_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if(errorId != ETHFW_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHFW_MODULE_ID, ETHFW_INSTANCE_ID_DET, ETHFW_SID_IS_RX_FRAME_ALLOWED, errorId);
  }
#else
  ETHFW_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETHFW_DEV_ERROR_REPORT == STD_ON */

  return frameAllowed;
}

/**********************************************************************************************************************
 *  EthFw_IsFrameTxAllowed()
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
FUNC(boolean, ETHFW_CODE) EthFw_IsFrameTxAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean frameAllowed = FALSE;
  uint8 errorId = ETHFW_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(ETHFW_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(EthFw_ModuleInitialized == ETHFW_UNINIT)
  {
    errorId = ETHFW_E_UNINIT;
  }
  else if(FrameHdrPtr == NULL_PTR)
  {
    errorId = ETHFW_E_PARAM_POINTER;
  }
  else if(PayloadPtr == NULL_PTR)
  {
    errorId = ETHFW_E_PARAM_POINTER;
  }
  else
#endif /* ETHFW_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Analyze received Ethernet frame */
    frameAllowed = EthFw_IsFrameAllowed(CtrlIdx, FrameHdrPtr, ETHFW_DIRECTION_TX, PayloadPtr, PayloadLen);

#if(ETHFW_TXFRAMEDISCARDEDCBKFCTPTR == STD_ON)
    /* #30 Trigger callback if frame was discarded */
    if(frameAllowed == FALSE)
    {
      EthFw_GetTxFrameDiscardedCbkFctPtr()(CtrlIdx, FrameHdrPtr, PayloadPtr, PayloadLen);
    }
#endif /* ETHFW_TXFRAMEDISCARDEDCBKFCTPTR == STD_ON */

#if(!EthFw_IsEnabled())
    /* #40 Indicate frame as valid if module is disabled (EthFw/EthFwGeneral/EthFwEnabled) */
    frameAllowed = TRUE;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(ETHFW_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if(errorId != ETHFW_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHFW_MODULE_ID, ETHFW_INSTANCE_ID_DET, ETHFW_SID_IS_TX_FRAME_ALLOWED, errorId);
  }
#else
  ETHFW_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETHFW_DEV_ERROR_REPORT == STD_ON */

  return frameAllowed;
}

#define ETHFW_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

 MD_ETHFW_14.5_0770:
     Description: Rule 14.5, Message 0770
                  The continue statement shall not be used
     Reason:      The continue statement is used in a circumstance where a different approach would reduce efficiency
                  or maintainability due to increased complexity.
     Risk:        None.
     Prevention:  Covered by code review.

 MD_ETHFW_14.6_0771:
     Description: Rule 14.6, Message 0771
                  For any iteration statement there shall be at most one break statement used for loop termination.
     Reason:      The loop needs multiple exit points since error conditions cannot be verified prior to the loop.
     Risk:        None.
     Prevention:  Covered by code review.

*/

/**********************************************************************************************************************
 *  END OF FILE: EthFw.c
 *********************************************************************************************************************/
