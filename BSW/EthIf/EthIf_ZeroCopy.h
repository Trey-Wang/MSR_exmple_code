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
/**        \file  EthIf_ZeroCopy.h
 *        \brief  Ethernet Interface Zero Copy extensions header file
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

#if !defined (ETHIF_ZERO_COPY_H)
# define ETHIF_ZERO_COPY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Types.h"
# include "EthIf_Cfg.h"
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- API service IDs ----- */
# define ETHIF_SID_RELEASE_RX_BUFFER               (0x0EU)
# define ETHIF_SID_PROVIDE_EXT_TX_BUFFER           (0x0FU)
# define ETHIF_SID_GET_TX_HEADER_PTR               (0x40U)
# define ETHIF_SID_GET_RX_HEADER_PTR               (0x41U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
/***********************************************************************************************************************
 *  EthIf_ProvideExtTxBuffer
 **********************************************************************************************************************/
/*! \brief          Provides an external transmission buffer for an Ethernet frame
 *  \details        Function allows to provide an external buffer for an Ethernet frame transmission.
 *  \param[in]      CtrlIdx               EthIf controller index
 *  \param[in]      FrameType             EtherType to insert into the Ethernet frame header
 *  \param[in]      Priority              Priority of the Ethernet frame, which is coded into the PCP of the IEEE802.1Q
 *                                        VLAN tag. If EthIf controller represents a physical data connection the
 *                                        priority is ignored.
 *  \param[out]     BufIdxPtr             Index to identify the external buffer in context of EthIf
 *  \param[in,out]  BufPtr                Buffer pointer:
 *                                        [in] - Location of the buffer provided externally
 *                                        [out] - Location where payload can be written to
 *  \param[in,out]  LenBytePtr            Buffer length:
 *                                        [in] - Length of the buffer in byte
 *                                        [out] - Length of the buffer reduced by Ethernet header and, dependent on EthIf
 *                                                controller, by VLAN tag size
 *  \return         BUFREQ_OK - success
 *  \return         BUFREQ_E_NOT_OK - function has been called with invalid parameters
 *  \return         BUFREQ_E_BUSY - maximum amount of external buffers that can be handled reached
 *  \return         BUFREQ_E_OVFL - provided buffer is to small to hold the Ethernet header and, dependent on EthIf
 *                                  controller, the VLAN tag
 *  \pre            Module and drivers have been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace CREQ-111281
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideExtTxBuffer(
        uint8                                       CtrlIdx,
        Eth_FrameType                               FrameType,
        uint8                                       Priority,
  P2VAR(uint8,         AUTOMATIC, ETHIF_APPL_DATA)  BufIdxPtr,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr);

/***********************************************************************************************************************
 *  EthIf_ReleaseRxBuffer
 **********************************************************************************************************************/
/*! \brief       Releases an reception buffer
 *  \details     Function releases an reception buffer and allows the underlying Ethernet driver to reuse it for
 *               further receptions.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   BufPtr                Pointer to the buffer to be released
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111287
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ReleaseRxBuffer(
        uint8                                      CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr);
# endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */

# if ( ETHIF_ENABLE_HEADER_ACCESS_API == STD_ON )
/***********************************************************************************************************************
 *  EthIf_GetTxHeaderPtr
 **********************************************************************************************************************/
/*! \brief       Retrieves the location of the Ethernet header for a given transmission buffer
 *  \details     Function retrieves the location of the Ethernet header for a given transmission buffer.
 *  \param[in]   CtrlIdx       EthIf controller index
 *  \param[in]   BufIdx        Index to identify the buffer the Ethernet header location shall be retrieved for
 *  \param[out]  BufPtr        Location of the Ethernet header
 *  \param[out]  LenBytPtr     Length of the Ethernet header in byte:
 *                             14 byte - non VLAN enabled EthIf controller
 *                             18 byte - VLAN enable EthIf controller
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \pre         Buffer to retrieve the Ethernet header location for must be previously acquired by
 *               EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer()
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111284
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTxHeaderPtr(
        uint8                                       CtrlIdx,
        uint8                                       BufIdx,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr);

/***********************************************************************************************************************
 *  EthIf_GetRxHeaderPtr
 **********************************************************************************************************************/
/*! \brief          Retrieves the location of the Ethernet header for a given reception buffer
 *  \details        Function retrieves the location of the Ethernet header for a given reception buffer.
 *  \param[in]      CtrlIdx       EthIf controller index
 *  \param[in,out]  BufPtr        Pointer to reception buffer:
 *                                [in] - Location of the payload of the Ethernet frame within the reception buffer
 *                                [out] - Location of the Ethernet header
 *  \param[out]     LenBytPtr     Length of the Ethernet header in byte:
 *                                14 byte - non VLAN enabled EthIf controller
 *                                18 byte - VLAN enable EthIf controller
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111283
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetRxHeaderPtr(
        uint8                                       CtrlIdx,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr);
# endif /* ETHIF_ENABLE_HEADER_ACCESS_API */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_ZERO_COPY_H */


/**********************************************************************************************************************
 *  END OF FILE: EthIf_ZeroCopy.h
 *********************************************************************************************************************/

