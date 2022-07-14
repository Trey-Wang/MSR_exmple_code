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
/**        \file  EthIf_Switch.h
 *        \brief  Ethernet Interface Ethernet switch abstraction header file
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
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (ETHIF_SWITCH_H)
# define ETHIF_SWITCH_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Cfg.h"
# include "EthIf_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- API service IDs ----- */
# define ETHIF_SID_GET_PORT_MAC_ADDR                     (0x28U)
# define ETHIF_SID_GET_ARL_TABLE                         (0x29U)
# define ETHIF_SID_GET_BUFFER_LEVEL                      (0x2AU)
# define ETHIF_SID_GET_DROP_COUNT                        (0x2BU)
# define ETHIF_SID_STORE_CONFIGURATION                   (0x2CU)
# define ETHIF_SID_RESET_CONFIGURATION                   (0x2DU)
# define ETHIF_SID_SET_SWITCH_MGMT_INFO                  (0x80U)
# define ETHIF_SID_SWITCH_ENABLE_EGRESS_TIME_STAMP       (0x81U)
# define ETHIF_SID_SWITCH_MGMT_INFO_INDICATION           (0x82U)
# define ETHIF_SID_SWITCH_EGRESS_TIME_STAMP_INDICATION   (0x83U)
# define ETHIF_SID_SWITCH_INGRESS_TIME_STAMP_INDICATION  (0x84U)
# define ETHIF_SID_SWITCH_UPDATE_MCAST_PORT_ASSIGNMENT   (0xA0U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( ETHIF_ETHSWT_USED == STD_ON )

# if ( ETHIF_GET_PORT_MAC_ADDR_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetPortMacAddr
 **********************************************************************************************************************/
/*! \brief      Retrieves the switch instance and port a MAC address is assigned to.
 *  \details    Function allows to retrieve the switch instance and port a Ethernet frame with a source MAC
 *              address matching the passed MAC address was received on.
 *  \param[in]  MacAddrPtr      MAC address to be queried
 *  \param[out] SwitchIdxPtr    Index of the switch instance the corresponding frame was received on
 *  \param[out] PortIdxPtr      Index of the port the corresponding frame was received on
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters or no port information found
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace CREQ-111217
 *  \trace SPEC-2393873
 *  \trace SPEC-2393874
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPortMacAddr(
  P2CONST(uint8,              AUTOMATIC, ETHIF_APPL_DATA)  MacAddrPtr,
  P2VAR(EthSwt_SwitchIdxType, AUTOMATIC, ETHIF_APPL_DATA)  SwitchIdxPtr,
  P2VAR(EthSwt_PortIdxType,   AUTOMATIC, ETHIF_APPL_DATA)  PortIdxPtr);
# endif /* ETHIF_GET_PORT_MAC_ADDR_API */

# if ( ETHIF_GET_ARL_TABLE_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetArlTable
 **********************************************************************************************************************/
/*! \brief         Retrieves the complete address resolution table.
 *  \details       Function allows to retrieve the valid entries of the address resolution table of a switch
 *                 instance.
 *  \param[in]     SwitchIdx       Index of the switch instance
 *  \param[in/out] LenPtr          in:  Size of the passed buffer the entries shall be written to\n
 *                                 out: Number of entries written into buffer
 *  \param[out]    ArlTablePtr     Pointer to the buffer the data shall be written to
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111218
 *  \trace SPEC-2394045
 *  \trace SPEC-2393879
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetArlTable(
        EthSwt_SwitchIdxType                             SwitchIdx,
  P2VAR(uint32,              AUTOMATIC, ETHIF_APPL_VAR)  LenPtr,
  P2VAR(EthSwt_MacVlanType,  AUTOMATIC, ETHIF_APPL_VAR)  ArlTablePtr);
# endif /* ETHIF_GET_ARL_TABLE_API */

# if ( ETHIF_GET_BUFFER_LEVEL_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetBufferLevel
 **********************************************************************************************************************/
/*! \brief         Reads the buffer level of the currently used buffer of the switch
 *  \details       Function reads the buffer level of the currently used buffer of the switch.
 *  \param[in]     SwitchIdx            Index of the switch instance
 *  \param[out]    SwitchBufferLevelPtr The interpretation of this value is switch dependent
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111219
 *  \trace SPEC-2394047
 *  \trace SPEC-2393884
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBufferLevel(
        EthSwt_SwitchIdxType                                SwitchIdx,
  P2VAR(EthSwt_BufferLevelType, AUTOMATIC, ETHIF_APPL_VAR)  SwitchBufferLevelPtr);
# endif /* ETHIF_GET_BUFFER_LEVEL_API */

# if ( ETHIF_GET_DROP_COUNT_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetDropCount
 **********************************************************************************************************************/
/*! \brief         Retrieves the drop counts according to the AUTOSAR SWS.
 *  \details       Function allows to retrieve the drop counts specified by the AUTOSAR SWS. Each count is the sum
 *                 of the drop count of all ports.
 *  \param[in]     SwitchIdx       Index of the switch instance
 *  \param[in,out] LenPtr          [in] - Size of the passed buffer the drop counts shall be written to
 *                                 [out] - Number of drop counts written into buffer
 *  \param[out]    DropCountPtr    Pointer to the buffer the data shall be written to
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111220
 *  \trace SPEC-2394049
 *  \trace SPEC-2393889
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetDropCount(
        EthSwt_SwitchIdxType                             SwitchIdx,
  P2VAR(uint16,              AUTOMATIC, ETHIF_APPL_VAR)  LenPtr,
  P2VAR(uint32,              AUTOMATIC, ETHIF_APPL_VAR)  DropCountPtr);
# endif /* ETHIF_GET_DROP_COUNT_API */

# if ( ETHIF_STORE_CONFIG_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_StoreConfiguration
 **********************************************************************************************************************/
/*! \brief         Calls EthSwt_StoreConfiguration() API of the related EthSwt-driver.
 *  \details       Function calls the EthSwt_StoreConfiguration() API of the related EthSwt-driver. Behavior
 *                 depends on the implementation of the driver. Commonly the latest MAC/Port table retrieved out of
 *                 the address resolution table of the switch is stored in NV RAM.
 *  \param[in]     SwitchIdx       Index of the switch instance
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111221
 *  \trace SPEC-2394051
 *  \trace SPEC-2393894
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StoreConfiguration(
  EthSwt_SwitchIdxType  SwitchIdx);
# endif /* ETHIF_STORE_CONFIG_API */

# if ( ETHIF_RESET_CONFIG_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_ResetConfiguration
 **********************************************************************************************************************/
/*! \brief         Calls EthSwt_ResetConfiguration() API of the related EthSwt-driver.
 *  \details       Function calls the EthSwt_ResetConfiguration() API of the related EthSwt-driver. Behavior
 *                 depends on the implementation of the driver. Commonly the MAC/Port table previously stored in NV RAM
 *                 triggered by EthIf_StoreConfiguration() is invalidated and switching behavior with regard to MACs
 *                 and VLANs is reset to initial (as defined by static configuration) behavior.
 *  \param[in]     SwitchIdx       Index of the switch instance
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111222
 *  \trace SPEC-2394053
 *  \trace SPEC-2393898
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ResetConfiguration(
  EthSwt_SwitchIdxType  SwitchIdx);
# endif /* ETHIF_RESET_CONFIG_API */

# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetSwitchMgmtInfo
 **********************************************************************************************************************/
/*! \brief         Sets management information for a frame identified by the EthIf controller and the
 *                 Ethernet buffer index.
 *  \details       Function allows to apply special treatment for an Ethernet frame. The frame is identified by
 *                 the EthIf controller and the Ethernet buffer index. This function can only be called between a
 *                 EthIf_ProvideTxBuffer() and EthIf_Transmit().
 *  \param[in]     EthIfCtrlIdx    Index of the EthIf controller
 *  \param[in]     BufIdx          Index of the Ethernet Tx buffer retrieved during EthIf_ProvideTxBuffer()
 *  \param[in]     MgmtInfo        Switch Management information
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized and buffer has to be acquired with EthIf_ProvideTxBuffer()
 *  \context       ANY - call only allowed between EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer() and EthIf_Transmit()
 *                       for a specific frame
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111223
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetSwitchMgmtInfo(
          uint8                                         EthIfCtrlIdx,
          uint8                                         BufIdx,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)  MgmtInfo);
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SwitchEnableEgressTimeStamp
 **********************************************************************************************************************/
/*! \brief         Enables time stamping within the Ethernet switch for an Ethernet frame.
 *  \details       Function enables time stamping for an Ethernet frame identified by the buffer index on the
 *                 port described by the management information.
 *  \param[in]     EthIfCtrlIdx    Index of the EthIf controller
 *  \param[in]     BufIdx          Index of the Ethernet Tx buffer retrieved during EthIf_ProvideTxBuffer()
 *  \param[in]     MgmtInfo        Switch Management information
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized and buffer has to be acquired with EthIf_ProvideTxBuffer()
 *  \context       ANY - call only allowed between EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer() and EthIf_Transmit()
 *                       for a specific frame
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchEnableEgressTimeStamp(
          uint8                                         EthIfCtrlIdx,
          uint8                                         BufIdx,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)  MgmtInfo);

# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

# if ( ETHIF_SWITCH_UPDATE_MCAST_PORT_ASSIGN_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_SwitchUpdateMCastPortAssignment
 **********************************************************************************************************************/
/*! \brief         Updates the multicast assignment for a specific port.
 *  \details       Function updates the multicast assignment for a specific Ethernet switch port.
 *  \param[in]     SwitchIdx       Index of the EthIf switch
 *  \param[in]     PortIdx         Index of the Ethernet Switch Port
 *  \param[in]     MCastAddr       Pointer to the multicast address
 *  \param[in]     Action          Action that shall be applied
 *                                - ETHSWT_MCAST_PORT_ASSIGN_ACTION_ADD: Request passing of multicast on the port
 *                                - ETHSWT_MCAST_PORT_ASSIGN_ACTION_REMOVE: Request removal of multicast on the port
 *  \return        E_OK - success
 *  \return        E_NOT_OK - function has been called with invalid parameters
 *  \pre           Module and drivers have been initialized
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchUpdateMCastPortAssignment(
          uint8                            SwitchIdx,
          uint8                            PortIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_CONST)   MCastAddr,
          EthSwt_MCastPortAssignActionType Action);
# endif /* ETHIF_SWITCH_UPDATE_MCAST_PORT_ASSIGN_API */

#endif /* ETHIF_ETHSWT_USED */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif/* ETHIF_SWITCH_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Switch.h
 *********************************************************************************************************************/

