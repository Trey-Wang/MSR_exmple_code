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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  EthIf_Time.h
 *        \brief  Ethernet Interface timer handling extension header file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
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

#if !defined (ETHIF_TIME_H)
# define ETHIF_TIME_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Types.h"
# include "EthIf_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- API service IDs ----- */
# define ETHIF_SID_GET_CURRENT_TIME         (0x22U)
# define ETHIF_SID_ENABLE_EGRESS_TIMESTAMP  (0x23U)
# define ETHIF_SID_GET_EGRESS_TIMESTAMP     (0x24U)
# define ETHIF_SID_GET_INGRESS_TIMESTAMP    (0x25U)
# define ETHIF_SID_SET_CORRECTION_TIME      (0x26U)
# define ETHIF_SID_SET_GLOBAL_TIME          (0x27U)

# if ( ETHIF_GLOBAL_TIME_SUPPORT == STD_ON )
/*! Internal comment removed.
 *
 *
 *
 *
 */
/*! \brief Quality of the time stamp */
typedef Eth_TimestampQualityType  Eth_TimeStampQualType;  /* Mapping of Vector specific Pre-ASR type to ASR4.3 type */
#  define ETH_VALID      ETH_TIMESTAMP_VALID              /* Mapping of Vector specific Pre-ASR define to ASR4.3 define */
#  define ETH_INVALID    ETH_TIMESTAMP_INVALID            /* Mapping of Vector specific Pre-ASR define to ASR4.3 define */
#  define ETH_UNCERTAIN  ETH_TIMESTAMP_UNCERTAIN          /* Mapping of Vector specific Pre-ASR define to ASR4.3 define */
# endif /* ETHIF_GLOBAL_TIME_SUPPORT */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ( ETHIF_GLOBAL_TIME_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  EthIf_GetCurrentTime
 **********************************************************************************************************************/
/*! \brief       Retrieves the current time of the Ethernet controllers timer module
 *  \details     Function redirects the call to the Ethernet controller driver to retrieve the current time
 *               of the controllers timer module.
 *  \param[in]   CtrlIdx       EthIf Controller index
 *  \param[out]  timeQualPtr   Pointer to the buffer the quality of the time retrieved shall be stored to
 *  \param[out]  timeStampPtr  Pointer to the buffer the current time shall be stored to
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111211
 *  \trace SPEC-2393901
 *  \trace SPEC-2393905
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCurrentTime(
        uint8                                              CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_VAR)  timeQualPtr,
  P2VAR(Eth_TimeStampType,     AUTOMATIC, ETHIF_APPL_VAR)  timeStampPtr);

/***********************************************************************************************************************
 *  EthIf_SetGlobalTime
 **********************************************************************************************************************/
/*! \brief       Sets the current time of the Ethernet controllers timer module
 *  \details     Function redirects the call to the Ethernet controller driver to set the current time
 *               of the controllers timer module to a specific value.
 *  \param[in]   CtrlIdx                  EthIf Controller index
 *  \param[in]   TimestampPtr             Pointer to the buffer the current time shall be stored to
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111216
 *  \trace SPEC-2393927
 *  \trace SPEC-2393931
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetGlobalTime(
          uint8                                            CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_CONST)  TimestampPtr);

/***********************************************************************************************************************
 *  EthIf_SetCorrectionTime
 **********************************************************************************************************************/
/*! \brief       Corrects the time of the Ethernet controllers timer module
 *  \details     Function redirects the call to the Ethernet controller driver to correct the time of the timer
 *               module by a offset and/or de-/accelerate the time.
 *  \param[in]   CtrlIdx                  EthIf Controller index
 *  \param[in]   OffsetTimePtr            Offset to correct the time
 *  \param[in]   RateRatioPtr             Ratio used to de-/accelerate the time
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111215
 *  \trace SPEC-2394059
 *  \trace SPEC-2393925
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetCorrectionTime(
          uint8                                           CtrlIdx,
  P2CONST(Eth_TimediffType,  AUTOMATIC, ETHIF_APPL_CONST) OffsetTimePtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETHIF_APPL_CONST) RateRatioPtr);

/***********************************************************************************************************************
 *  EthIf_EnableEgressTimestamp
 **********************************************************************************************************************/
/*! \brief       Enable timestamping of a frame in Ethernet controller
 *  \details     Function redirects the call to the Ethernet controller driver to enable egress timestamping of
 *               a frame.
 *  \param[in]   CtrlIdx                  EthIf Controller index
 *  \param[in]   BufIdx                   Buffer index identifying the frame
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY - call only allowed between EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer() and EthIf_Transmit()
 *                     for a specific frame
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111212
 *  \trace SPEC-2394055
 *  \trace SPEC-2393909
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_EnableEgressTimestamp(
  uint8  CtrlIdx,
  uint8  BufIdx);

/***********************************************************************************************************************
 *  EthIf_GetEgressTimestamp
 **********************************************************************************************************************/
/*! \brief       Retrieve the egress timestamp of a frame
 *  \details     Function redirects the call to the Ethernet controller driver to retrieve the egress timestamp of
 *               a frame.
 *  \param[in]   CtrlIdx                  EthIf Controller index
 *  \param[in]   BufIdx                   Buffer index identifying the frame
 *  \param[out]  TimestampPtr             Pointer to buffer for timestamp storage
 *  \param[out]  TimestampQualityPtr      Pointer to buffer for timestamp quality storage
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     TASK|ISR1|ISR2 - call only allowed in EthIf_TxConfirmation()
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111213
 *  \trace SPEC-2393911
 *  \trace SPEC-2393915
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimestamp(
        uint8                                                 CtrlIdx,
        uint8                                                 BufIdx,
  P2VAR(Eth_TimeStampType,        AUTOMATIC, ETHIF_APPL_VAR)  TimestampPtr,
  P2VAR(Eth_TimestampQualityType, AUTOMATIC, ETHIF_APPL_VAR)  TimestampQualityPtr);

/***********************************************************************************************************************
 *  EthIf_GeIngressTimestamp
 **********************************************************************************************************************/
/*! \brief       Retrieve the ingress timestamp of a frame
 *  \details     Function redirects the call to the Ethernet controller driver to retrieve the ingress timestamp of
 *               a frame.
 *  \param[in]   CtrlIdx                  EthIf Controller index
 *  \param[in]   DataPtr                  Pointer to the frame payload for identifying the frame
 *  \param[out]  TimestampPtr             Pointer to buffer for timestamp storage
 *  \param[out]  TimestampQualityPtr      Pointer to buffer for timestamp quality storage
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     TASK|ISR1|ISR2 - call only allowed in EthIf_RxIndication()
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111214
 *  \trace SPEC-2394057
 *  \trace SPEC-2393920
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimestamp(
        uint8                                                  CtrlIdx,
  P2VAR(Eth_DataType,             AUTOMATIC, ETHIF_APPL_DATA)  DataPtr,
  P2VAR(Eth_TimeStampType,        AUTOMATIC, ETHIF_APPL_VAR)   TimestampPtr,
  P2VAR(Eth_TimestampQualityType, AUTOMATIC, ETHIF_APPL_VAR)   TimestampQualityPtr);

# endif /* ETHIF_GLOBAL_TIME_SUPPORT */

# define ETHIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_TIME_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Time.h
 *********************************************************************************************************************/

