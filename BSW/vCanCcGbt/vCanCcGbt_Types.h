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
 *         File:  vCanCcGbt_Types.h
 *       Module:  vCanCcGbt
 *
 *  Description:  Global types header of the vCanccGbt module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (VCANCCGBT_TYPES_H)
# define VCANCCGBT_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  uint32 Version;
  uint16 TotalMaximumPermissibleChargingVoltage;
  uint16 PowerStorageBatterySytemCapacity;
  uint16 PowerStorageBatterySystemVoltage;
  uint8 BatteryType;
} vCanCcGbt_MandatoryInitialChargingParameterType;

typedef struct
{
  uint8 VehicleIdentificationNumber[17];
  uint8 BMSSoftwareVersionNumber[8];
  uint8 BatteryManufacturerName[4];
  boolean IsBatteryManufacturerNameValid;
  boolean IsBatteryPackNoValid;
  boolean IsBatterySetProductionDataYearValid;
  boolean IsBatterySetProductionDataMonthValid;
  boolean IsBatterySetProductionDataDayValid;
  boolean IsBatterySetChargingFrequencyValid;
  boolean IsBatterySetPropertyRightMarkValid;
  boolean IsVehicleIdentificationNumberValid;
  boolean IsBMSSoftwareVersionNumberValid;
  uint32 BatteryPackNo;
  uint32 BatterySetChargingFrequency;
  uint8 BatterySetProductionDataYear;
  uint8 BatterySetProductionDataMonth;
  uint8 BatterySetProductionDataDay;
  uint8 BatterySetPropertyRightMark;
} vCanCcGbt_OptionalInitialChargingParameterType;

typedef struct
{
  uint16 MaximumPermissibleChargingVoltageSingleBattery;
  uint16 MaximumPermissibleChargingCurrent;
  uint16 TotalNominalEnergyOfPowerStorageBattery;
  uint16 TotalMaximumPermissibleChargingVoltage;
  uint16 ChargeStatusPowerStorageBatteryVehicle;
  uint16 CurrentBatteryVoltagePowerStorageBatteryVehicle;
  uint8 MaximumPermissibleTemperature;
} vCanCcGbt_MandatoryParameterExchangeDataType;

typedef struct
{
  uint16 VoltageDemand;
  uint16 CurrentDemand;
  uint8 ChargingMode;
} vCanCcGbt_BCLDataType;

typedef struct
{
  uint16 MeasuredValueOfChargingVoltage;
  uint16 MeasuredValueOfChargingCurrent;
  uint16 HighestVoltageOfBattery;
  uint16 EstimatedRemainingChargingTime;
  uint8 HighestVoltageOfBatteryGroupNumber;
  uint8 CurrentChargeState;
} vCanCcGbt_BCSDataType;

typedef struct
{
  uint8 SerialNumberHighestVoltgeBattery;
  uint8 HighestTemperatureBattery;
  uint8 SerialNumberHighestTemperatureDetectionPoint;
  uint8 LowestTemperatureBattery;
  uint8 SerialNumberLowestTemperatureDetectionPoint;
  uint8 VoltageOfSingleBateryIsOverHighOrOverLow;
  uint8 StateOfChargeOfBatteryCompleteVehicle;
  uint8 ChargingOvercurrentBattery;
  uint8 ExcessTemperatureOfBattery;
  uint8 InsulationStateOfBattery;
  uint8 ConnectionStateOfOutputConnectorOfBattery;
  uint8 ChargingPermissible;
} vCanCcGbt_BSMDataType;

typedef struct
{
  boolean IsActive;
  uint16 BatteryVoltage;
  uint8 BatteryGroupNumber;
} vCanCcGbt_BMVSingleType;

typedef struct
{
  vCanCcGbt_BMVSingleType BMVSignleBattery[256];
} vCanCcGbt_BMVDataType;

typedef struct
{
  boolean IsActive;
  uint8 BatteryTemperature;
} vCanCcGbt_BMTSingleType;

typedef struct
{
  vCanCcGbt_BMTSingleType BMTSingleTemperature[128];
} vCanCcGbt_BMTDataType;

typedef struct
{
  boolean IsActive;
  uint8 BatteryReserved;
} vCanCcGbt_BSPSingleType;

typedef struct
{
  vCanCcGbt_BSPSingleType BSPSignleBatteryReserved[16];
} vCanCcGbt_BSPDataType;

typedef struct
{
  uint16 FaultCauseForBMSSuspendingCharging;
  uint8 CauseForBMSSuspendingCharging;
  uint8 ErrorCauseForBMSSuspendingCharging;
} vCanCcGbt_BSTDataType;

typedef struct
{
  uint16 MinimumVoltageSingleBattery;
  uint16 MaximumVoltageSingleBattery;
  uint8 SuspendStateOfCharge;
  uint8 MinimumTemperatureBattery;
  uint8 MaximumTemperatureBattery;
} vCanCcGbt_BSDDataType;

typedef struct
{
  uint16 SendTimeout;
} vCanCcGbt_IfTxPduStateType;

typedef struct
{
  boolean IsLocked;
  uint16 SendTimeout;
} vCanCcGbt_TpTxPduStateType;

#endif /* VCANCCGBT_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: vCanCcGbt_Types.h
 *********************************************************************************************************************/
