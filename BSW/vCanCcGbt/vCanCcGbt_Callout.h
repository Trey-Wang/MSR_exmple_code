/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  vCanCcGbt_Callout.h
 *       Module:  vCanCcGbt
 *
 *  Description:  Callout header of the vCanCcGbt module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (VCANCCGBT_CALLOUT_H)
# define VCANCCGBT_CALLOUT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "vCanCcGbt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*========================================== VCANCCGBT_START_SEC_APPL_CODE ==========================================*/
# define VCANCCGBT_START_SEC_APPL_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerVersion()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CHM message.
 *  \details      Handshake phase.
 *  \param[in]    chargerRevision  Revision of the Charger
 *  \param[in]    chargerVersion   Version of the Charger
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerVersion(uint8 chargerRevision, uint16 chargerVersion);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerInformation()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CRM message.
 *  \details      Handshake phase.
 *  \param[in]    chargerNo                        Charger number
 *  \param[in]    chargerStationLocationCode       Charger station location code
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerInformation(uint32 chargerNo,
                                                                       uint8 chargerStationLocationCode[4]);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideHandshakeSuccess()
 *********************************************************************************************************************/
/*! \brief        Indicates the success of the handshake phase.
 *  \details      Handshake phase.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideHandshakeSuccess(void);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerTime()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CTS message.
 *  \details      Parameter exchange phase.
 *  \param[in]    second  Seconds of the Chargers time.
 *  \param[in]    minute  Minutes of the Chargers time.
 *  \param[in]    hour    Hour of the Chargers time.
 *  \param[in]    day     Day of the Chargers time.
 *  \param[in]    month   Month of the Chargers time.
 *  \param[in]    year    Year of the Chargers time.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerTime(uint8 second, uint8 minute, uint8 hour, uint8 day,
                                                                uint8 month, uint16 year);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerMaximumOutput()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CML message.
 *  \details      Parameter exchange phase.
 *  \param[in]    maxOutputVoltage  Maximum voltage output
 *  \param[in]    minOutputVoltage  Minimum voltage output
 *  \param[in]    maxOutputCurrent  Maximum current output
 *  \param[in]    minOutputCurrent  Minimum current output
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerMaximumOutput(uint16 maxOutputVoltage,
                                                                         uint16 minOutputVoltage,
                                                                         uint16 maxOutputCurrent,
                                                                         uint16 minOutputCurrent);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideParameterConfigurationSuccess()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CRO message.
 *  \details      Parameter exchange phase.
 *  \param[in]    chargerChargingState  State of the Charger
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideParameterConfigurationSuccess(uint8 chargerChargingState);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerChargingState()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CCS message.
 *  \details      Charging phase.
 *  \param[in]    voltgeOutput        Voltage output
 *  \param[in]    currentOutput       Current output
 *  \param[in]    chargingTime        Charging time
 *  \param[in]    chargingPermissible Charging permissible
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerChargingState(uint16 voltgeOutput, uint16 currentOutput,
                                                                         uint16 chargingTime,
                                                                         uint8 chargingPermissible);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerSuspendInformation()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CST message.
 *  \details      Charging phase.
 *  \param[in]    causeChargerSuspendingCharging       Charger cause for suspending the charging
 *  \param[in]    faultCauseChargerSuspendingCharging  Charger fault cause for suspending the charging
 *  \param[in]    errorCauseChargerSuspendingCharging  Charger error cause for suspending the charging
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerSuspendInformation(uint8 causeChargerSuspendingCharging,
                                                                              uint16
                                                                              faultCauseChargerSuspendingCharging,
                                                                              uint8
                                                                              errorCauseChargerSuspendingCharging);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargingStatistics()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CSD message.
 *  \details      Charge statistic phase.
 *  \param[in]    cumulativeChargingTime  Cumulative charging time
 *  \param[in]    outputEnergy            Output energy
 *  \param[in]    chargerNumber           Charger number
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargingStatistics(uint16 cumulativeChargingTime,
                                                                       uint16 outputEnergy, uint32 chargerNumber);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideChargerErrors()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of a CEM message.
 *  \details      Error phase.
 *  \param[in]    timeoutValueBRM  BRM error value
 *  \param[in]    timeoutValueBCP  BCP error value
 *  \param[in]    timeoutValueBRO  BRO error value
 *  \param[in]    timeoutValueBCS  BCS error value
 *  \param[in]    timeoutValueBSM  BSM error value
 *  \param[in]    timeoutValueBST  BST error value
 *  \param[in]    timeoutValueBSD  BSD error value
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideChargerErrors(uint8 timeoutValueBRM, uint8 timeoutValueBCP,
                                                                  uint8 timeoutValueBRO, uint8 timeoutValueBCS,
                                                                  uint8 timeoutValueBSM, uint8 timeoutValueBST,
                                                                  uint8 timeoutValueBSD);

/***********************************************************************************************************************
 *  AppVCanCcGbt_ProvideBmsErrors()
 *********************************************************************************************************************/
/*! \brief        Indicates the detection of a timeout of the reception of a Charger message.
 *  \details      Error phase.
 *  \param[in]    crm00Timeout  CRM00 timeout
 *  \param[in]    crmAATimeout  CRMAA timeout
 *  \param[in]    ctsTimeout    CTS timeout
 *  \param[in]    croTimeout    CRO timeout
 *  \param[in]    ccsTimeout    CCS timeout
 *  \param[in]    cstTimeout    CST timeout
 *  \param[in]    csdTimeout    CSD timeout
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_APPL_CODE) AppVCanCcGbt_ProvideBmsErrors(boolean crm00Timeout, boolean crmAATimeout,
                                                              boolean ctsTimeout, boolean croTimeout,
                                                              boolean ccsTimeout, boolean cstTimeout,
                                                              boolean csdTimeout);

/***********************************************************************************************************************
 *  vCanCcGbt_GetMandatoryInitialChargingParameter()
 *********************************************************************************************************************/
/*! \brief        Function returns the mandatory initial application specific parameter needed for charging.
 *  \details      Handshake phase.
 *  \param[in]    chargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void,
     VCANCCGBT_APPL_CODE)
AppVCanCcGbt_GetMandatoryInitialChargingParameter(P2VAR
                                                  (vCanCcGbt_MandatoryInitialChargingParameterType, AUTOMATIC,
                                                   VCANCCGBT_VAR_NOINIT) mandatoryInitialChargingParameter);

/***********************************************************************************************************************
 *  vCanCcGbt_GetOptionalInitialChargingParameter()
 *********************************************************************************************************************/
/*! \brief        Function returns the optional initial application specific parameter needed for charging.
 *  \details      Handshake phase.
 *  \param[in]    optionalInitialChargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void,
     VCANCCGBT_APPL_CODE)
AppVCanCcGbt_GetOptionalInitialChargingParameter(P2VAR
                                                 (vCanCcGbt_OptionalInitialChargingParameterType, AUTOMATIC,
                                                  VCANCCGBT_VAR_NOINIT) optionalInitialChargingParameter);

/***********************************************************************************************************************
 *  vCanCcGbt_GetOptionalInitialChargingParameter()
 *********************************************************************************************************************/
/*! \brief        Function returns the optional initial application specific parameter needed for charging.
 *  \details      Parameter exchange phase.
 *  \param[in]    optionalInitialChargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetParameterExchangeData(P2VAR(vCanCcGbt_MandatoryParameterExchangeDataType,
                                                                      AUTOMATIC,
                                                                      VCANCCGBT_VAR_NOINIT) parameterExchangeData);

/***********************************************************************************************************************
 *  vCanCcGbt_IsBmsReadyForCharging()
 *********************************************************************************************************************/
/*! \brief        Function returns weather the BMS is ready for charging phase or not.
 *  \details      -
 *  \return       TRUE BMS is ready for charging phase
 *                FALSE BMS is not read for charging phase
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, VCANCCGBT_APPL_CODE) AppVCanCcGbt_IsBmsReadyForCharging(void);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetChargingMode()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BCL message.
 *  \details      Parameter exchange phase.
 *  \param[in]    bclData  Data buffer for BCL message data
 *  \return       E_OK - New data are available.
 *                E_NOT_OK - No new data are available.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetChargingMode(P2VAR(vCanCcGbt_BCLDataType, AUTOMATIC,
                                                             VCANCCGBT_VAR_NOINIT) bclData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetChargingStatus()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BCS message.
 *  \details      Charging phase.
 *  \param[in]    bcsData  Data buffer for BCS message data
 *  \return       E_OK - New data are available.
 *                E_NOT_OK - No new data are available.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetChargingStatus(P2VAR(vCanCcGbt_BCSDataType, AUTOMATIC,
                                                               VCANCCGBT_VAR_NOINIT) bcsData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetBatteryStatus()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BSM message.
 *  \details      Charging phase.
 *  \param[in]    bsmData  Data buffer for BSM message data
 *  \return       E_OK - New data are available.
 *                E_NOT_OK - No new data are available.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetBatteryStatus(P2VAR(vCanCcGbt_BSMDataType, AUTOMATIC,
                                                              VCANCCGBT_VAR_NOINIT) bsmData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetBatteryVoltages()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BMV message.
 *  \details      Charging phase.
 *  \param[in]    bmvData  Data buffer for BMV message data
 *  \return       E_OK - At least one data field is active.
 *                E_NOT_OK - No data field is active.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetBatteryVoltages(P2VAR(vCanCcGbt_BMVDataType, AUTOMATIC,
                                                                VCANCCGBT_VAR_NOINIT) bmvData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetBatteryTemperatures()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BMT message.
 *  \details      Charging phase.
 *  \param[in]    bmtData  Data buffer for BMT message data
 *  \return       E_OK - At least one data field is active.
 *                E_NOT_OK - No data field is active.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetBatteryTemperatures(P2VAR(vCanCcGbt_BMTDataType, AUTOMATIC,
                                                                    VCANCCGBT_VAR_NOINIT) bmtData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetReservedBatteries()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BSP message.
 *  \details      Charging phase.
 *  \param[in]    bspData  Data buffer for BSP message data
 *  \return       E_OK - At least one data field is active.
 *                E_NOT_OK - No data field is active.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetReservedBatteries(P2VAR(vCanCcGbt_BSPDataType, AUTOMATIC,
                                                                  VCANCCGBT_VAR_NOINIT) bspData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetSuspendReason()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BST message.
 *  \details      Charging phase.
 *  \param[in]    bstData  Data buffer for BST message data
 *  \return       E_OK - New data are available.
 *                E_NOT_OK - No new data are available.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetSuspendReason(P2VAR(vCanCcGbt_BSTDataType, AUTOMATIC,
                                                              VCANCCGBT_VAR_NOINIT) bstData);

/***********************************************************************************************************************
 *  AppVCanCcGbt_GetChargingStatistics()
 *********************************************************************************************************************/
/*! \brief        Requests the data for a BSD message.
 *  \details      Charge statistic phase.
 *  \param[in]    bsdData  Data buffer for BSD message data
 *  \return       E_OK - New data are available.
 *                E_NOT_OK - No new data are available.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType,
     VCANCCGBT_APPL_CODE) AppVCanCcGbt_GetChargingStatistics(P2VAR(vCanCcGbt_BSDDataType, AUTOMATIC,
                                                                   VCANCCGBT_VAR_NOINIT) bsdData);

# define VCANCCGBT_STOP_SEC_APPL_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*========================================== VCANCCGBT_STOP_SEC_APPL_CODE ===========================================*/

#endif /* VCANCCGBT_CALLOUT_H */
/**********************************************************************************************************************
 *  END OF FILE: vCanCcGbt_Callout.h
 *********************************************************************************************************************/
