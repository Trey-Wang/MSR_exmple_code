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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Dcm.h
 *    Component:  MICROSAR Diagnostic Communication Manager for SAE J1939
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  main header file for J1939 DCM
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials       Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Thomas Dedler                 Dth            Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-02-07  Dth     ESCAN00071378  Component Diag_Asr4J1939Dcm created
 *  01.00.01  2014-04-11  Dth     ESCAN00074274: no changes
 *  02.00.00  2014-05-12  Dth     ESCAN00073996: J1939Dcm_InitMemory added
 *                        Dth     ESCAN00071929: StartOfReception changed according to AR4.1.2
 *                        Dth     ESCAN00073477: no changes
 *                        Dth     ESCAN00073465: no changes
 *                        Dth     ESCAN00073464: no changes
 *                        Dth     ESCAN00076282: no changes
 *  02.01.00  2014-09-01  Dth     ESCAN00076487: no changes
 *                        Dth     ESCAN00077928: no changes
 *                        Dth     ESCAN00076604: Support for J1939 Diagnostics (release status)
 *                        Dth     ESCAN00078264: no changes
 *  03.00.00  2015-07-01  Dth     ESCAN00080031: no changes
 *                        Dth     ESCAN00083142: no changes
 *                        Dth     ESCAN00083700: no changes
 *  04.00.00  2016-02-23  Aey     ESCAN00087869: Support DM5/27/53/54/55 in J1939Dcm
 *                        Aey     ESCAN00084694: Adapt to changes of J1939Dcm_RequestIndication and J1939Rm_SendAck
 *  04.01.00  2016-05-17  Aey     ESCAN00089033: PB-S and PB-L for J1939Dcm
 *                        Aey     ESCAN00089284: uninitialized local variable 'lFilterHandler'
 *                        Aey     ESCAN00089006: Node goes offline only when all the referenced channels are offline
 *  04.02.00  2016-06-20  Aey     ESCAN00089269: ISOBUS support in J1939Dcm
  *********************************************************************************************************************/


#if !defined (J1939DCM_H)
#define J1939DCM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
/* module configuration */
#include "J1939Dcm_Cfg.h"
#include "J1939Dcm_Lcfg.h"
#include "J1939Dcm_PBcfg.h"

#include "J1939Dcm_Types.h"

/* callback prototypes */
#include "J1939Dcm_Cbk.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* --- Version Information ------------------------------------------------------------------------------------------*/
/* ##V_CFG_MANAGEMENT ##CQProject : Diag_Asr4J1939Dcm CQComponent : Implementation */
/* Version - BCD coded version number - Main- and Sub-Version - Release-Version */
#define DIAG_ASR4J1939DCM_VERSION          (0x0402u)
#define DIAG_ASR4J1939DCM_RELEASE_VERSION  (0x00u)

/* vendor and module identification */
#define J1939DCM_VENDOR_ID              (30u)
#define J1939DCM_MODULE_ID              (58u)
#define J1939DCM_INSTANCE_ID            (0u)

/* AUTOSAR Software Specification Version Information */
#define J1939DCM_AR_MAJOR_VERSION       (0x04)
#define J1939DCM_AR_MINOR_VERSION       (0x01)
#define J1939DCM_AR_PATCH_VERSION       (0x02)

/* Component Version Information */
#define J1939DCM_SW_MAJOR_VERSION       (DIAG_ASR4J1939DCM_VERSION >> 8u)
#define J1939DCM_SW_MINOR_VERSION       (DIAG_ASR4J1939DCM_VERSION & 0x00FF)
#define J1939DCM_SW_PATCH_VERSION       DIAG_ASR4J1939DCM_RELEASE_VERSION


/* --- Error Reporting Parameters -----------------------------------------------------------------------------------*/
/* DET error-IDs (SWS_J1939Dcm_00090) */
#define J1939DCM_E_INVALID_PDU_SDU_ID  (0x01u)
#define J1939DCM_E_PARAM_POINTER       (0x02u)
#define J1939DCM_E_INVALID_STATE       (0x06u)
#define J1939DCM_E_PARAM_ID            (0x07u)      /* RfC 58535 */
#define J1939DCM_E_INVALID_NODE        (0x08u)      /* RfC 58535 */
#define J1939DCM_E_INVALID_PGN         (0x09u)      /* RfC 58535 */
#define J1939DCM_E_INVALID_PRIO        (0x0Au)      /* RfC 58535 */
#define J1939DCM_E_INVALID_CHANNEL     (0x0Bu)      /* RfC 58535 */
#define J1939DCM_E_PARAM_CONFIG        (0x10u)      /* non-AUTOSAR extension */
#define J1939DCM_E_OVERRUN             (0x11u)      /* non-AUTOSAR extension */
#define J1939DCM_E_UNINIT              (0x20u)
#define J1939DCM_E_REINIT              (0x21u)
#define J1939DCM_E_NO_ERROR            (0xFFu)      /* Not Autosar error. Used for local variables initialization */

/* DET API-IDs */
#define J1939DCM_API_INIT                     (0x01u) /* SWS_J1939Dcm_00098 */
#define J1939DCM_API_DEINIT                   (0x02u) /* SWS_J1939Dcm_00099 */
#define J1939DCM_API_GETVERSIONINFO           (0x03u) /* SWS_J1939Dcm_00100 */
#define J1939DCM_API_MAINFUNCTION             (0x04u) /* SWS_J1939Dcm_00107 */
#define J1939DCM_API_COPYRXDATA               (0x05u) /* SWS_J1939Dcm_00103 */
#define J1939DCM_API_COPYTXDATA               (0x06u) /* SWS_J1939Dcm_00105 */
#define J1939DCM_API_STARTOFRECEPTION         (0x07u) /* SWS_J1939Dcm_00102 */
#define J1939DCM_API_TPRXINDICATION           (0x08u) /* SWS_J1939Dcm_00104 */
#define J1939DCM_API_TPTXCONFIRMATION         (0x09u) /* SWS_J1939Dcm_00106 */
#define J1939DCM_API_DEMTRIGGERONDTCSTATUS    (0x0Au) /* SWS_J1939Dcm_00122 */
#define J1939DCM_API_SETSTATE                 (0x0Bu) /* SWS_J1939Dcm_00124 */
#define J1939DCM_API_TXCONFIRMATION           (0x0Cu) /* SWS_J1939Dcm_????? */
#define J1939DCM_API_RXINDICATION             (0x42u) /* SWS_J1939Dcm_00128 */
#define J1939DCM_API_REQUESTINDICATION        (0x43u) /* SWS_J1939Dcm_00101 */
#define J1939DCM_API_MEMRESPONSETRANSMIT      (0x50u) /* vector specific */
#define J1939DCM_API_MEMDATATRANSMIT          (0x51u) /* vector specific */



/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define J1939DCM_START_SEC_CODE
#include "MemMap.h"                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* --- Service Functions --------------------------------------------------------------------------------------------*/
FUNC(void, J1939DCM_CODE) J1939Dcm_Init( P2CONST(J1939Dcm_ConfigType, AUTOMATIC, J1939DCM_INIT_DATA) configPtr ); /* SWS_J1939Dcm_00098 */

FUNC(void, J1939DCM_CODE) J1939Dcm_InitMemory(void);

FUNC(void, J1939DCM_CODE) J1939Dcm_DeInit( void ); /* SWS_J1939Dcm_00099 */

FUNC(void, J1939DCM_CODE) J1939Dcm_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, J1939DCM_APPL_DATA) versioninfo ); /* SWS_J1939Dcm_00100 */

FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_SetState( NetworkHandleType channel,
                                                       uint8 node,
                                                       J1939Dcm_StateType newState ); /* SWS_J1939Dcm_00124 */


/* --- Memory Access Functions --------------------------------------------------------------------------------------*/
#if (J1939DCM_DM15_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_MemResponseTransmit()
 **********************************************************************************************************************/
/*! \brief      Triggers transmission of a memory access response message (DM15).
 *  \details    Constructs the memory handler state for DM15.
 *  \param[in]  AddressInfo               Channel, source and destination address
 *  \param[in]  MemResponseParameter      Parameters of the DM15 message
 *  \return     E_OK                      If the transmission initiation succeeded
 *  \return     E_NOT_OK                  If the transmission initiation failed
 *  \context    ISR1|ISR2
 *  \reentrant  FALSE
 *  \pre        -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_MemResponseTransmit (P2CONST(J1939Dcm_AddressInfoType, AUTOMATIC, J1939DCM_APPL_DATA) AddressInfo,
                                                                  P2CONST(J1939Dcm_MemRespParamType, AUTOMATIC, J1939DCM_APPL_DATA) MemResponseParameter);
#endif

#if ( (J1939DCM_DM16_SUPPORT == STD_ON) || (J1939DCM_DM18_SUPPORT == STD_ON) )
/**********************************************************************************************************************
 * J1939Dcm_MemDataTransmit()
 **********************************************************************************************************************/
/*! \brief      Triggers transmission of either binary (DM16) or security data (DM18)
 *  \details    Constructs the memory handler state for DM16 or DM18.
 *  \param[in]  DataKind                  kind of data (binary or security)
 *  \param[in]  AddressInfo               channel, source- and destination address
 *  \param[in]  PduInfo                   Data to be transmitted
 *  \return     E_OK                      If the transmission initiation succeeded
 *  \return     E_NOT_OK                  If the transmission initiation failed
 *  \context    ISR1|ISR2
 *  \reentrant  FALSE
 *  \pre        -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_MemDataTransmit      (J1939Dcm_MemDataType DataKind,
                                                                   P2CONST(J1939Dcm_AddressInfoType, AUTOMATIC, J1939DCM_APPL_DATA) AddressInfo,
                                                                   P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) PduInfo);
#endif


#define J1939DCM_STOP_SEC_CODE
#include "MemMap.h"                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif  /* J1939DCM_H */

/**********************************************************************************************************************
 *  END OF FILE: Msn.h
 *********************************************************************************************************************/
