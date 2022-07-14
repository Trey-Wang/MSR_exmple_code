/***********************************************************************************************************************
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
/*!        \file  vCanCcCdm.h
 *        \brief  vCanCcCdm header file
 *      \details  TODO
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-07-17  virchl  -             Initial version (beta)
 *  01.00.01  2017-09-14  virchl  -             Updated beta version
 **********************************************************************************************************************/

#if !defined (VCANCCCDM_H)
# define VCANCCCDM_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/*# include "Std_Types.h" */ /* Already included within ComStack_Types.h */
# include "ComStack_Types.h"
# include "CanIf.h"
# include "vCanCcCdm_Types.h"
# include "vCanCcCdm_Cfg.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Vendor and module identification */
# define VCANCCCDM_VENDOR_ID                           (30u)
# define VCANCCCDM_MODULE_ID                           (0xFFu)


/* AUTOSAR Software specification version information */
# define VCANCCCDM_AR_RELEASE_MAJOR_VERSION            (4u)
# define VCANCCCDM_AR_RELEASE_MINOR_VERSION            (0u)
# define VCANCCCDM_AR_RELEASE_REVISION_VERSION         (3u)


/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VCANCCCDM_SW_MAJOR_VERSION                    (1u)
# define VCANCCCDM_SW_MINOR_VERSION                    (0u)
# define VCANCCCDM_SW_PATCH_VERSION                    (1u)

# define VCANCCCDM_INSTANCE_ID_DET                     (0x00u)


/* ----- API service IDs ----- */
/*!< Embedded Interface vCanCcCdm_General */
/*!< Service ID: vCanCcCdm_Init() */
# define VCANCCCDM_SID_INIT                               (0x00u)
/*!< Service ID: vCanCcCdm_DeInit() */
# define VCANCCCDM_SID_DEINIT                             (0x01u)
/*!< Service ID: vCanCcCdm_StartCanCommunication() */
# define VCANCCCDM_SID_START_CAN_COMMUNICATION            (0x02u)
/*!< Service ID: vCanCcCdm_TerminateCanCommunication() */
# define VCANCCCDM_SID_TERMINATE_CAN_COMMUNICATION        (0x03u)
/*!< Service ID: vCanCcCdm_MainFunction() */
# define VCANCCCDM_SID_MAIN_FUNCTION                      (0x04u)
/*!< Service ID: vCanCcCdm_GetUsedProtocolVersion() */
# define VCANCCCDM_SID_GET_USED_PROTOCOL_VERSION          (0x05u)
/*!< Service ID: vCanCcCdm_GetVersionInfo() */
# define VCANCCCDM_SID_GET_VERSION_INFO                   (0x09u)

/*!< Embedded Interface vCanCcCdm_General */
/*!< Service ID: vCanCcCdm_InitialValues() */
# define VCANCCCDM_SID_GET_ALL_INITIAL_CAN_SIGNAL_VALUES  (0x10u)

/*!< Embedded Interface vCanCcCdm_TransmitSignals */
/*!< Service ID: vCanCcCdm_UpdateMinimumChargeCurrent() */
# define VCANCCCDM_SID_UPDATE_MINIMUM_CHARGE_CURRENT      (0x20u)
/*!< Service ID: vCanCcCdm_UpdateEstimatedChargingTime() */
# define VCANCCCDM_SID_UPDATE_ESTIMATED_CHARGING_TIME     (0x21u)
/*!< Service ID: vCanCcCdm_UpdateChargingCurrentRequest() */
# define VCANCCCDM_SID_UPDATE_CHARGING_CURRENT_REQUEST    (0x22u)
/*!< Service ID: vCanCcCdm_SetVehicleFaultFlags() */
# define VCANCCCDM_SID_SET_VEHICLE_FAULT_FLAGS            (0x23u)
/*!< Service ID: vCanCcCdm_SetVehicleStatusFlags() */
# define VCANCCCDM_SID_SET_VEHICLE_STATUS_FLAGS           (0x24u)
/*!< Service ID: vCanCcCdm_UpdateStateOfCharge() */
# define VCANCCCDM_SID_UPDATE_STATE_OF_CHARGE             (0x25u)

/*!< Embedded Interface vCanCcCdm_UpdateFaultFlags */
/*!< Service ID: vCanCcCdm_UpdateBatteryOvervoltageFlag() */
# define VCANCCCDM_SID_UPDATE_BATTERY_OVERVOLTAGE_FLAG                  (0x30u)
/*!< Service ID: vCanCcCdm_UpdateBatteryUndervoltageFlag() */
# define VCANCCCDM_SID_UPDATE_BATTERY_UNDERVOLTAGE_FLAG                 (0x31u)
/*!< Service ID: vCanCcCdm_UpdateBatteryCurrentDeviationErrorFlag() */
# define VCANCCCDM_SID_UPDATE_BATTERY_CURRENT_DEVIATION_ERROR_FLAG      (0x32u)
/*!< Service ID: vCanCcCdm_UpdateHighBatteryTemperatureFlag() */
# define VCANCCCDM_SID_UPDATE_HIGH_BATTERY_TEMPERATURE_FLAG             (0x33u)
/*!< Service ID: vCanCcCdm_UpdateBatteryVoltageDeviationErrorFlag() */
# define VCANCCCDM_SID_UPDATE_BATTERY_VOLTAGE_DEVIATION_ERROR_FLAG      (0x34u)

/*!< Embedded Interface vCanCcCdm_UpdateStatusFlags */
/*!< Service ID: vCanCcCdm_UpdateVehicleChargingEnabled() */
# define VCANCCCDM_SID_UPDATE_VEHICLE_CHARGING_ENABLED            (0x45u)
/*!< Service ID: vCanCcCdm_UpdateVehicleShiftPosition() */
# define VCANCCCDM_SID_UPDATE_VEHICLE_SHIFT_POSITION              (0x45u)
/*!< Service ID: vCanCcCdm_UpdateChargingSystemError() */
# define VCANCCCDM_SID_UPDATE_CHARGING_SYSTEM_ERROR               (0x45u)
/*!< Service ID: vCanCcCdm_UpdateVehicleStatus() */
# define VCANCCCDM_SID_UPDATE_VEHICLE_STATUS                      (0x45u)
/*!< Service ID: vCanCcCdm_UpdateNormalStopRequestBeforeCharging() */
# define VCANCCCDM_SID_UPDATE_NORMAL_STOP_REQUEST_BEFORE_CHARGING (0x45u)

/*!< Embedded Interface vCanCcCdm_ReceiveSignals */
/*!< Service ID: vCanCcCdm_GetIdentifierOfWeldingDetection() */
# define VCANCCCDM_SID_GET_IDENTIFIER_OF_WELDING_DETECTION  (0x50u)
/*!< Service ID: vCanCcCdm_GetAvailableOutputVoltage() */
# define VCANCCCDM_SID_GET_AVAILABLE_OUTPUT_VOLTAGE         (0x51u)
/*!< Service ID: vCanCcCdm_GetAvailableOutputCurrent() */
# define VCANCCCDM_SID_GET_AVAILABLE_OUTPUT_CURRENT         (0x52u)
/*!< Service ID: vCanCcCdm_GetThresholdVoltage() */
# define VCANCCCDM_SID_GET_THRESHOLD_VOLTAGE                (0x53u)
/*!< Service ID: vCanCcCdm_GetPresentChargingVoltage() */
# define VCANCCCDM_SID_GET_PRESENT_CHARGING_VOLTAGE         (0x54u)
/*!< Service ID: vCanCcCdm_GetPresentChargingCurrent() */
# define VCANCCCDM_SID_GET_PRESENT_CHARGING_CURRENT         (0x55u)
/*!< Service ID: vCanCcCdm_GetStateFaultsFlagByte() */
# define VCANCCCDM_SID_GET_STATE_FAULTS_FLAG_BYTE           (0x56u)
/*!< Service ID: vCanCcCdm_GetRemainingChargingTime() */
# define VCANCCCDM_SID_GET_REMAINING_CHARGING_TIME          (0x57u)
/*!< Service ID: vCanCcCdm_GetChargerStatus() */
# define VCANCCCDM_SID_GET_CHARGER_STATUS                   (0x58u)
/*!< Service ID: vCanCcCdm_GetChargerError() */
# define VCANCCCDM_SID_GET_CHARGER_ERROR                    (0x59u)
/*!< Service ID: vCanCcCdm_GetEnergizingState() */
# define VCANCCCDM_SID_GET_ENERGIZING_STATE                 (0x60u)
/*!< Service ID: vCanCcCdm_GetBatteryIncompatibility() */
# define VCANCCCDM_SID_GET_BATTERY_INCOMPATIBILITY          (0x61u)
/*!< Service ID: vCanCcCdm_GetChargingSystemError() */
# define VCANCCCDM_SID_GET_CHARGING_SYSTEM_ERROR            (0x62u)
/*!< Service ID: vCanCcCdm_GetChargingStopControl() */
# define VCANCCCDM_SID_GET_CHARGING_STOP_CONTROL            (0x63u)

/*!< Embedded Interface vCanCcCdm_CanIfCallbackInterface */
/*!< Service ID: vCanCcCdm_RxIndication() */
# define VCANCCCDM_SID_RX_INDICATION          (0x70u)
/*!< Service ID: vCanCcCdm_TxConfirmation() */
# define VCANCCCDM_SID_TX_CONFIRMATION        (0x71u)

/*!< Embedded Interface vCanCcCdm_InformApplicationAboutChanges */
/*!< Service ID: vCanCcCdm_AvailableOutputCurrentChanged() */
# define VCANCCCDM_SID_AVAILABLE_OUTPUT_CURRENT_CHANGED       (0x80u)
/*!< Service ID: vCanCcCdm_ChargersStatusChanged() */
# define VCANCCCDM_SID_CHARGERS_STATUS_CHANGED                (0x81u)
/*!< Service ID: vCanCcCdm_PresentChargingCurrentChanged() */
# define VCANCCCDM_SID_PRESENT_CHARGING_CURRENT_CHANGED       (0x82u)
/*!< Service ID: vCanCcCdm_PresentOutputVoltageChanged() */
# define VCANCCCDM_SID_PRESENT_OUTPUT_VOLTAGE_CHANGED         (0x83u)
/*!< Service ID: vCanCcCdm_TresholdVoltageChanged() */
# define VCANCCCDM_SID_TRESHOLD_VOLTAGE_CHANGED               (0x84u)
/*!< Service ID: vCanCcCdm_WeldingDetected() */
# define VCANCCCDM_SID_WELDING_DETECTED                       (0x85u)


/* ----- Error codes ----- */
/*!
 *  /defgroup errorIDs Error IDs
 */
/*! used to check if no error occurred - use a value unequal to any error code */
# define VCANCCCDM_E_NO_ERROR                           (0x00u)
/*! Error code: API service vCanCcCdm_Init() called with wrong parameter */
# define VCANCCCDM_E_PARAM                              (0x0Fu)
/*! Error code: API service called with NULL_PTR as parameter  */
# define VCANCCCDM_E_PARAM_POINTER                      (0x0Cu)
/*! Error code: API service used without module initialization */
# define VCANCCCDM_E_UNINIT                             (0x10u)
/*! Error code: The service vCanCcCdm_Init() is called while the module is already initialized  */
# define VCANCCCDM_E_ALREADY_INITIALIZED                (0x11u)
/*! Error code: The service vCanCcCdm_StartCanCommunication() is called while the communication is already started */
# define VCANCCCDM_E_COM_ALREADY_STARTED                (0x12u)
/*! Error code: The service vCanCcCdm_TerminateCanCommunication() is called while the communication is not started */
# define VCANCCCDM_E_COM_NOT_STARTED                    (0x13u)


/* ----- Functional macros ----- */
/*! used to define Protocol Version */
# define VCANCCCDM_CHARGED_RATE_CONST_VALUE             (100u)    /*! 100 per cent */
/*! used to define Protocol Version */
# define VCANCCCDM_CHADEMO_PROTOCOL_VERSION             (0x02u)    /*! ver. 1.1.0 */
/*! used to determine 100ms - cycle for CAN transmission */
# define VCANCCCDM_REFERENCE_VALUE_100MS                (10u)
/*! used to determine 1s - cycle for CAN transmission */
# define VCANCCCDM_REFERENCE_VALUE_1S                   (10u)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! Example of a function-like macro */
# define VCANCCCDM_DO_SOMETHING(XYZ, ABC)              ((XYZ) = (ABC) + 1) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if ( VCANCCCDM_DEV_ERROR_DETECT == STD_ON )
/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
#define vCanCcCdm_ReportDet(a,b)     (void)Det_ReportError(VCANCCCDM_MODULE_ID, VCANCCCDM_INSTANCE_ID_DET, a, b)
#else
#define vCanCcCdm_ReportDet(a,b)
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

# define VCANCCCDM_START_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Example global variable definition */
//extern VAR(uint32, VCANCCCDM_VAR_NOINIT) vCanCcCdm_ExampleValue;

# define VCANCCCDM_STOP_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define VCANCCCDM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  vCanCcCdm_InitMemory()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_InitMemory(void);


/***********************************************************************************************************************
 * vCanCcCdm_Init()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_Init(void);


/***********************************************************************************************************************
 *  vCanCcCdm_UnInit()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_DeInit(void);


/***********************************************************************************************************************
 *  vCanCcCdm_GetUsedProtocolVersion()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ProtocolStatusType, VCANCCCDM_CODE) vCanCcCdm_GetUsedProtocolVersion(
  vCanCcCdm_ProtocolNumberPtrType Value
  );


/***********************************************************************************************************************
 *  vCanCcCdm_StartCanCommunication()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_StartCanCommunication(void);


/***********************************************************************************************************************
 *  vCanCcCdm_TerminateCanCommunication()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_TerminateCanCommunication(void);


/***********************************************************************************************************************
 * vCanCcCdm_MainFunction()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_MainFunction(void);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateMinimumChargeCurrent()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateMinimumChargeCurrent(vCanCcCdm_CurrentType MinChargeCurrent);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateEstimatedChargingTime()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateEstimatedChargingTime(vCanCcCdm_TimeType ChargingTime);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateChargingCurrentRequest()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateChargingCurrentRequest(vCanCcCdm_CurrentType CurrentRequest);


/***********************************************************************************************************************
 *  vCanCcCdm_SetVehicleFaultFlags()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_SetVehicleFaultFlags(vCanCcCdm_FlagByteType FaultFlags);


/***********************************************************************************************************************
 *  vCanCcCdm_SetVehicleStatusFlags()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_SetVehicleStatusFlags(vCanCcCdm_FlagByteType StatusFlags);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateStateOfCharge()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateStateOfCharge(vCanCcCdm_StateOfChargeType StateOfCharge);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryOvervoltageFlag()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryOvervoltageFlag(vCanCcCdm_FlagType FaultFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryUndervoltageFlag()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryUndervoltageFlag(vCanCcCdm_FlagType FaultFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryCurrentDeviationErrorFlag()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryCurrentDeviationErrorFlag(vCanCcCdm_FlagType FaultFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateHighBatteryTemperatureFlag()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateHighBatteryTemperatureFlag(vCanCcCdm_FlagType FaultFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryVoltageDeviationErrorFlag()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryVoltageDeviationErrorFlag(vCanCcCdm_FlagType FaultFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleChargingEnabled()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleChargingEnabled(vCanCcCdm_FlagType StatusFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleShiftPosition()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleShiftPosition(vCanCcCdm_FlagType StatusFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateChargingSystemError()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateChargingSystemError(vCanCcCdm_FlagType StatusFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleStatus()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleStatus(vCanCcCdm_FlagType StatusFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateNormalStopRequestBeforeCharging()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateNormalStopRequestBeforeCharging(vCanCcCdm_FlagType StatusFlag);


/***********************************************************************************************************************
 *  vCanCcCdm_GetIdentifierOfWeldingDetection()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetIdentifierOfWeldingDetection(
  vCanCcCdm_WeldingDetectionPtrType IdOfWeldingDetectionPtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetAvailableOutputVoltage()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetAvailableOutputVoltage(
  vCanCcCdm_VoltagePtrType VoltagePtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetAvailableOutputCurrent()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetAvailableOutputCurrent(
  vCanCcCdm_CurrentPtrType CurrentPtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetThresholdVoltage()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetThresholdVoltage(vCanCcCdm_VoltagePtrType  VoltagePtr);


/***********************************************************************************************************************
 *  vCanCcCdm_GetPresentChargingVoltage()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetPresentChargingVoltage(
  vCanCcCdm_VoltagePtrType  VoltagePtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetPresentChargingCurrent()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetPresentChargingCurrent(
  vCanCcCdm_CurrentPtrType  CurrentPtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetStateFaultsFlagByte()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetStateFaultsFlagByte(
  vCanCcCdm_FlagBytePtrType  FlagBytePtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetRemainingChargingTime()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetRemainingChargingTime(
  vCanCcCdm_TimePtrType TimePtr,
  vCanCcCdm_TimeUnitPtrType UnitPtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_GetChargerStatus()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargerStatus(vCanCcCdm_FlagPtrType FlagPtr);



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargerError()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargerError(vCanCcCdm_FlagPtrType FlagPtr);



/***********************************************************************************************************************
 *  vCanCcCdm_GetEnergizingState()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetEnergizingState(vCanCcCdm_FlagPtrType FlagPtr);



/***********************************************************************************************************************
 *  vCanCcCdm_GetBatteryIncompatibility()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetBatteryIncompatibility(vCanCcCdm_FlagPtrType FlagPtr);



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargingSystemError()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargingSystemError(vCanCcCdm_FlagPtrType FlagPtr);



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargingStopControl()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargingStopControl(vCanCcCdm_FlagPtrType FlagPtr);


# if (VCANCCCDM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  vCanCcCdm_GetVersionInfo()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_GetVersionInfo(vCanCcCdm_VersionInfoPtrType versioninfo);
# endif


/***********************************************************************************************************************
 *  vCanCcCdm_RxIndication()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
extern FUNC(void, CANIF_APPL_MSG_CODE) vCanCcCdm_RxIndication(
  VAR(PduIdType, AUTOMATIC) CanRxPduId,
  P2CONST(uint8, AUTOMATIC, CANIF_UL_STANDARD_VAR) CanSduPtr
  );


/***********************************************************************************************************************
 *  vCanCcCdm_TxConfirmation()
 **********************************************************************************************************************/
/*! ToDo
 **********************************************************************************************************************/
extern FUNC(void, CANIF_APPL_MSG_CODE) vCanCcCdm_TxConfirmation(VAR(PduIdType, AUTOMATIC) CanTxPduId);


# define VCANCCCDM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 **********************************************************************************************************************/
/*!
 * \exclusivearea VCANCCCDM_EXCLUSIVE_AREA_0
 * TODO - beyond is still template test
 * Ensures consistent entries when writing to the RxQueue.
 * \protects canRxQueueBuffer, canRxQueueInfo (write)
 * \usedin Can_EnableControllerInterrupts, Can_DisableControllerInterrupts
 * \exclude Rx Interrupt, Can_MainFunction_Receive
 * \length SHORT The CAN frame is copied to the RX queue.
 * \endexclusivearea
 */

#endif /* VCANCCCDM_H */


/***********************************************************************************************************************
 *  END OF FILE: vCanCcCdm.h
 **********************************************************************************************************************/
