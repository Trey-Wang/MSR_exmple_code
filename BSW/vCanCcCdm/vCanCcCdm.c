/***********************************************************************************************************************
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
 *  ----------------------------------------------------------------------------------------------------------------- */
/*!        \file  vCanCcCdm.c
 *        \brief  vCanCcCdm source file
 *      \details  TODO
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#define VCANCCCDM_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "vCanCcCdm.h"
#include "SchM_vCanCcCdm.h"
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif


/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of vCanCcCdm header file */
#if (  (VCANCCCDM_SW_MAJOR_VERSION != (1u)) \
    || (VCANCCCDM_SW_MINOR_VERSION != (0u)) \
    || (VCANCCCDM_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of vCanCcCdm.c and vCanCcCdm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VCANCCCDM_CFG_MAJOR_VERSION != (1u)) \
    || (VCANCCCDM_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of vCanCcCdm.c and vCanCcCdm_Cfg.h are inconsistent!"
#endif


/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Example symbolic constant */
#define VCANCCCDM_MY_SYMBOLIC_CONSTANT        (0x05u)
/*! Reset value for ... */
#define vCanCcCdm_Bit0                     ((uint8)(0x01 << 0))
#define vCanCcCdm_Bit1                     ((uint8)(0x01 << 1))
#define vCanCcCdm_Bit2                     ((uint8)(0x01 << 2))
#define vCanCcCdm_Bit3                     ((uint8)(0x01 << 3))
#define vCanCcCdm_Bit4                     ((uint8)(0x01 << 4))
#define vCanCcCdm_Bit5                     ((uint8)(0x01 << 5))

#define H102Data_StatusFlagsSetVehicleChargingEnabled()            (H102Data.StatusFlags |= vCanCcCdm_Bit0)
#define H102Data_StatusFlagsSetVehicleShiftPosition()              (H102Data.StatusFlags |= vCanCcCdm_Bit1)
#define H102Data_StatusFlagsSetChargingSystemError()               (H102Data.StatusFlags |= vCanCcCdm_Bit2)
#define H102Data_StatusFlagsSetVehicleStatus()                     (H102Data.StatusFlags |= vCanCcCdm_Bit3)
#define H102Data_StatusFlagsSetNormalStopRequestBeforeCharging()   (H102Data.StatusFlags |= vCanCcCdm_Bit4)

#define H102Data_StatusFlagsResetVehicleChargingEnabled()          (H102Data.StatusFlags &= ((uint8)(~vCanCcCdm_Bit0)))
#define H102Data_StatusFlagsResetVehicleShiftPosition()            (H102Data.StatusFlags &= ((uint8)(~vCanCcCdm_Bit1)))
#define H102Data_StatusFlagsResetChargingSystemError()             (H102Data.StatusFlags &= ((uint8)(~vCanCcCdm_Bit2)))
#define H102Data_StatusFlagsResetVehicleStatus()                   (H102Data.StatusFlags &= ((uint8)(~vCanCcCdm_Bit3)))
#define H102Data_StatusFlagsResetNormalStopRequestBeforeCharging() (H102Data.StatusFlags &= ((uint8)(~vCanCcCdm_Bit4)))


#define H102Data_FaultFlagsSetBatteryOvervoltage()                  (H102Data.FaultFlags |= vCanCcCdm_Bit0)
#define H102Data_FaultFlagsSetBatteryUndervoltage()                 (H102Data.FaultFlags |= vCanCcCdm_Bit1)
#define H102Data_FaultFlagsSetBatteryCurrentDeviationError()        (H102Data.FaultFlags |= vCanCcCdm_Bit2)
#define H102Data_FaultFlagsSetHighBatteryTemperature()              (H102Data.FaultFlags |= vCanCcCdm_Bit3)
#define H102Data_FaultFlagsSetBatteryVoltageDeviationError()        (H102Data.FaultFlags |= vCanCcCdm_Bit4)

#define H102Data_FaultFlagsResetBatteryOvervoltage()                (H102Data.FaultFlags &= ((uint8)(~vCanCcCdm_Bit0)))
#define H102Data_FaultFlagsResetBatteryUndervoltage()               (H102Data.FaultFlags &= ((uint8)(~vCanCcCdm_Bit1)))
#define H102Data_FaultFlagsResetBatteryCurrentDeviationError()      (H102Data.FaultFlags &= ((uint8)(~vCanCcCdm_Bit2)))
#define H102Data_FaultFlagsResetHighBatteryTemperature()            (H102Data.FaultFlags &= ((uint8)(~vCanCcCdm_Bit3)))
#define H102Data_FaultFlagsResetBatteryVoltageDeviationError()      (H102Data.FaultFlags &= ((uint8)(~vCanCcCdm_Bit4)))

#define H109Data_StatusFlagsSetChargerStatus()                (H109Data.StatusFaultFlags |= vCanCcCdm_Bit0)
#define H109Data_StatusFlagsSetChargerError()                 (H109Data.StatusFaultFlags |= vCanCcCdm_Bit1)
#define H109Data_StatusFlagsSetEnergizingState()              (H109Data.StatusFaultFlags |= vCanCcCdm_Bit2)
#define H109Data_StatusFlagsSetBatteryIncompatible()          (H109Data.StatusFaultFlags |= vCanCcCdm_Bit3)
#define H109Data_StatusFlagsSetChargingSystemError()          (H109Data.StatusFaultFlags |= vCanCcCdm_Bit4)
#define H109Data_StatusFlagsSetChargingStopControl()          (H109Data.StatusFaultFlags |= vCanCcCdm_Bit5)

#define H109Data_StatusFlagsResetChargerStatus()              (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit0)))
#define H109Data_StatusFlagsResetChargerError()               (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit1)))
#define H109Data_StatusFlagsResetEnergizingState()            (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit2)))
#define H109Data_StatusFlagsResetBatteryIncompatible()        (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit3)))
#define H109Data_StatusFlagsResetChargingSystemError()        (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit4)))
#define H109Data_StatusFlagsResetChargingStopControl()        (H109Data.StatusFaultFlags &= ((uint8)(~vCanCcCdm_Bit5)))


/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if !defined (VCANCCCDM_LOCAL) /* COV_VCANCCCDM_COMPATIBILITY */
# define VCANCCCDM_LOCAL static
#endif

#if !defined (VCANCCCDM_LOCAL_INLINE) /* COV_VCANCCCDM_COMPATIBILITY */
# define VCANCCCDM_LOCAL_INLINE LOCAL_INLINE
#endif


/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define VCANCCCDM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
VCANCCCDM_LOCAL VAR(vCanCcCdm_StatusType, VCANCCCDM_VAR_ZERO_INIT) vCanCcCdm_ModuleInitialized = VCANCCCDM_UNINIT;
#endif

#define VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define VCANCCCDM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VCANCCCDM_LOCAL VAR(vCanCcCdm_CommunicationHandlerType, VCANCCCDM_VAR_INIT) vCanCcCdm_CommunicationHandler;

VCANCCCDM_LOCAL VAR(vCanCcCdm_H100DataType, VCANCCCDM_VAR_INIT) H100Data;
VCANCCCDM_LOCAL VAR(vCanCcCdm_H101DataType, VCANCCCDM_VAR_INIT) H101Data;
VCANCCCDM_LOCAL VAR(vCanCcCdm_H102DataType, VCANCCCDM_VAR_INIT) H102Data;

VCANCCCDM_LOCAL VAR(vCanCcCdm_H108DataType, VCANCCCDM_VAR_INIT) H108Data;
VCANCCCDM_LOCAL VAR(vCanCcCdm_H109DataType, VCANCCCDM_VAR_INIT) H109Data;

#define VCANCCCDM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define VCANCCCDM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
  vCanCcCdm_CopyInitialValues()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_CopyInitialValues(vCanCcCdm_InitialValuesType initValues);


/***********************************************************************************************************************
  vCanCcCdm_ProcessTransmitJobs()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessTransmitJobs(void);


/***********************************************************************************************************************
  vCanCcCdm_TransmitData()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_TransmitData(
  PduIdType CanTxPduId,
  P2VAR(uint8, AUTOMATIC, VCANCCCDM_APPL_VAR) transmitMessage
  );


/***********************************************************************************************************************
  vCanCcCdm_ProcessReceiveJobs()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobs(void);


/***********************************************************************************************************************
  vCanCcCdm_ProcessReceiveJobsEvery100ms()
 **********************************************************************************************************************/
/*!
 * ToDo
 */


VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobsEvery100ms(void);


/***********************************************************************************************************************
  vCanCcCdm_ProcessReceiveJobsEvery1s()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobsEvery1s(void);


#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
/***********************************************************************************************************************
  vCanCcCdm_CheckUpdatedReceiveValues()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_CheckUpdatedReceiveValues(void);
#endif


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  vCanCcCdm_CopyInitialValues()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
VCANCCCDM_LOCAL_INLINE FUNC(void, VCANCCCDM_CODE) vCanCcCdm_CopyInitialValues(vCanCcCdm_InitialValuesType initValues)
{
  H100Data.MinumumCurrent = initValues.MinumumCurrent;
  H100Data.MaximumBatteryVoltage = initValues.MaximumBatteryVoltage;
  H100Data.ChargedRateConstantValue = VCANCCCDM_CHARGED_RATE_CONST_VALUE;

  H101Data.MaximumChargingTime_10s = initValues.MaximumChargingTime_10s;
  H101Data.MaximumChargingTime_1min = initValues.MaximumChargingTime_1min;
  H101Data.EstimatedChargingTime_1min = initValues.EstimatedChargingTime_1min;
  H101Data.TotalCapacityOfTractionBattery = initValues.TotalCapacityOfTractionBattery;

  H102Data.ChademoProtocolNumber = VCANCCCDM_CHADEMO_PROTOCOL_VERSION;
  H102Data.TargetBatteryVoltage = initValues.TargetBatteryVoltage;
  H102Data.ChargingCurrentRequest = initValues.ChargingCurrentRequest;
  H102Data.FaultFlags = 0x00;
  H102Data.StatusFlags = 0x08;
  if(initValues.VehicleShiftPosition)
  {
    H102Data_StatusFlagsSetVehicleShiftPosition();
  }
  else
  {
    H102Data_StatusFlagsResetVehicleShiftPosition();
  }
  H102Data.StateOfCharge = initValues.StateOfCharge;
}


/***********************************************************************************************************************
 *  vCanCcCdm_ProcessTransmitJobs()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessTransmitJobs(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 transmitMessage[8];
  vCanCcCdm_MessageTransmitHandlerType* transmitHandlerPtr = &(vCanCcCdm_CommunicationHandler.MessageTransmitHandler);
#if (VCANCCCDM_USE_BSWM_FOR_COM_REQUEST == STD_ON)
  ComM_ModeType ComMode = COMM_NO_COMMUNICATION;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (VCANCCCDM_USE_BSWM_FOR_COM_REQUEST == STD_ON)
  ComM_GetCurrentComMode(VCANCCCDM_COMM_USER, &ComMode);
  if (ComMode == COMM_FULL_COMMUNICATION)
#endif
  {
    /******************************************************************/
    /*                        Transmit Part                           */
    /******************************************************************/
    if (transmitHandlerPtr->MessageBufferEmpty)
    {
      switch (transmitHandlerPtr->NextMessageToBeSent)
      {
        case Message100:
          if (vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter % VCANCCCDM_REFERENCE_VALUE_100MS == 0)
          {
            transmitMessage[0] = H100Data.MinumumCurrent;
            transmitMessage[1] = 0;
            transmitMessage[2] = 0;
            transmitMessage[3] = 0;
            transmitMessage[4] = (uint8)(H100Data.MaximumBatteryVoltage & 0xFF);
            transmitMessage[5] = (uint8)((H100Data.MaximumBatteryVoltage >> 8) & 0xFF);
            transmitMessage[6] = H100Data.ChargedRateConstantValue;
            transmitMessage[7] = 0;
            transmitHandlerPtr->NextMessageToBeSent = Message101;
            transmitHandlerPtr->MessageBufferEmpty = FALSE;
            vCanCcCdm_TransmitData(transmitHandlerPtr->CanIfTxPduIdPtr->Message100TxPduId, transmitMessage);
          }
          break;
        case Message101:
          transmitMessage[0] = 0;
          transmitMessage[1] = H101Data.MaximumChargingTime_10s;
          transmitMessage[2] = H101Data.MaximumChargingTime_1min;
          transmitMessage[3] = H101Data.EstimatedChargingTime_1min;
          transmitMessage[4] = 0;
          transmitMessage[5] = (uint8)(H101Data.TotalCapacityOfTractionBattery & 0xFF);
          transmitMessage[6] = (uint8)((H101Data.TotalCapacityOfTractionBattery >> 8) & 0xFF);
          transmitMessage[7] = 0;
          transmitHandlerPtr->NextMessageToBeSent = Message102;
          transmitHandlerPtr->MessageBufferEmpty = FALSE;
          vCanCcCdm_TransmitData(transmitHandlerPtr->CanIfTxPduIdPtr->Message101TxPduId, transmitMessage);
          break;
        case Message102:
          transmitMessage[0] = H102Data.ChademoProtocolNumber;
          transmitMessage[1] = (uint8)(H102Data.TargetBatteryVoltage & 0xFF);
          transmitMessage[2] = (uint8)((H102Data.TargetBatteryVoltage >> 8) & 0xFF);
          transmitMessage[3] = H102Data.ChargingCurrentRequest;
          transmitMessage[4] = H102Data.FaultFlags;
          transmitMessage[5] = H102Data.StatusFlags;
          transmitMessage[6] = H102Data.StateOfCharge;
          transmitMessage[7] = 0;
          transmitHandlerPtr->NextMessageToBeSent = Message100;
          transmitHandlerPtr->MessageBufferEmpty = FALSE;
          vCanCcCdm_TransmitData(transmitHandlerPtr->CanIfTxPduIdPtr->Message102TxPduId, transmitMessage);
          break;
        default:
          break;
      }
    }
  }
  vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter++;
}


/***********************************************************************************************************************
 *  vCanCcCdm_TransmitData()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
void vCanCcCdm_TransmitData(PduIdType CanTxPduId, P2VAR(uint8, AUTOMATIC, VCANCCCDM_APPL_VAR) transmitMessage)
{
  PduInfoType transmitPdu;
  transmitPdu.SduDataPtr = transmitMessage;
  transmitPdu.SduLength = 8;

  CanIf_Transmit(CanTxPduId, &transmitPdu);
}


/***********************************************************************************************************************
 *  vCanCcCdm_ProcessReceiveJobs()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /*! Check the following every 100ms */
  vCanCcCdm_ProcessReceiveJobsEvery100ms();

  /*! Check the following every 1s */
  vCanCcCdm_ProcessReceiveJobsEvery1s();
}


/***********************************************************************************************************************
 *  vCanCcCdm_ProcessReceiveJobsEvery100ms()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobsEvery100ms(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vCanCcCdm_MessageReceiveHandlerType* receiveHandlerPtr = &(vCanCcCdm_CommunicationHandler.MessageReceiveHandler);

  /* ----- Implementation ----------------------------------------------- */
  if (receiveHandlerPtr->NoCheckCounter < VCANCCCDM_REFERENCE_VALUE_100MS)
  {
    receiveHandlerPtr->NoCheckCounter++;
  }
  else
  {
    uint8 MessageCounter;
    uint8 MessageCounterStamp;
    receiveHandlerPtr->NoCheckCounter = 1;
    vCanCcCdm_CommunicationHandler.Counter_100ms++;

    /*! Check Message108 */
    MessageCounter = receiveHandlerPtr->Message108Counter;
    MessageCounterStamp = receiveHandlerPtr->Message108CounterStamp_100ms;
    receiveHandlerPtr->Message108CounterStamp_100ms = MessageCounter;
    if (MessageCounter == MessageCounterStamp)
    {
      receiveHandlerPtr->Message108NotReceivedCounter++;
    }
    else
    {
      receiveHandlerPtr->Message108NotReceivedCounter = 0;
    }

    /*! Check Message109 */
    MessageCounter = receiveHandlerPtr->Message109Counter;
    MessageCounterStamp = receiveHandlerPtr->Message109CounterStamp_100ms;
    receiveHandlerPtr->Message109CounterStamp_100ms = MessageCounter;
    if (MessageCounter == MessageCounterStamp)
    {
      receiveHandlerPtr->Message109NotReceivedCounter++;
    }
    else
    {
      receiveHandlerPtr->Message109NotReceivedCounter = 0;
    }

#if (VCANCCCDM_CHECK_SKIPPED_MESSAGES == STD_ON)
    if (receiveHandlerPtr->MessageSkippedCounter >= VCANCCCDM_MAX_SKIPPED_MESSAGES)
    {
      receiveHandlerPtr->ReceiveStatus = VCANCCCDM_MESSAGES_APERIODIC;
      receiveHandlerPtr->MessageSkippedCounter = 0;
    }
#endif

    /*! Notify application if 1s timeout has elapsed without receiving any CAN message */
    if ( (receiveHandlerPtr->Message108NotReceivedCounter >= 10)
      || (receiveHandlerPtr->Message109NotReceivedCounter >= 10) )
    {
      receiveHandlerPtr->ReceiveStatus = VCANCCCDM_RECEPTION_STOPPED;
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
      if (receiveHandlerPtr->CallbackFuntionTablePtr->vCanCcCdm_NoDataReception != NULL_PTR)
      {
        receiveHandlerPtr->CallbackFuntionTablePtr->vCanCcCdm_NoDataReception();
      }
#endif
      /*
       * It is strongly recommended to restart CAN communication. There is no way out from ReceiveStatus
       * VCANCCCDM_RECEPTION_STOPPED.
       * Application is no more notified if a new data will be received later
       */
    }
  }
}


/***********************************************************************************************************************
 *  vCanCcCdm_ProcessReceiveJobsEvery1s()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_ProcessReceiveJobsEvery1s(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vCanCcCdm_MessageReceiveHandlerType* receiveHandlerPtr = &(vCanCcCdm_CommunicationHandler.MessageReceiveHandler);

  /* ----- Implementation ----------------------------------------------- */
  /*! Check the following every 1s */
  if (vCanCcCdm_CommunicationHandler.Counter_100ms >= VCANCCCDM_REFERENCE_VALUE_1S)
  {
    vCanCcCdm_CommunicationHandler.Counter_100ms = 0;

    if (receiveHandlerPtr->ReceiveStatus != VCANCCCDM_RECEPTION_STOPPED)
    {
      /*! Check receive rate of Message108 */
      if ( (receiveHandlerPtr->Message108Counter < VCANCCCDM_MIN_MESSAGES_PER_S)
        || (receiveHandlerPtr->Message108Counter > VCANCCCDM_MAX_MESSAGES_PER_S ) )
      {
        receiveHandlerPtr->ReceiveStatus = VCANCCCDM_MESSAGES_APERIODIC;
      }
      /*! Check receive rate of Message109 */
      else if ( (receiveHandlerPtr->Message109Counter < VCANCCCDM_MIN_MESSAGES_PER_S)
        || (receiveHandlerPtr->Message109Counter > VCANCCCDM_MAX_MESSAGES_PER_S ) )
      {
        receiveHandlerPtr->ReceiveStatus = VCANCCCDM_MESSAGES_APERIODIC;
      }
      else
      {
        receiveHandlerPtr->ReceiveStatus = VCANCCCDM_RUNNING;
      }
    }

    receiveHandlerPtr->Message108Counter = 0;
    receiveHandlerPtr->Message108CounterStamp_100ms = 0;
    receiveHandlerPtr->Message109Counter = 0;
    receiveHandlerPtr->Message109CounterStamp_100ms = 0;
  }
}


#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
/***********************************************************************************************************************
 *  vCanCcCdm_CheckUpdatedReceiveValues()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_CheckUpdatedReceiveValues(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vCanCcCdm_MessageReceiveHandlerType* receiveHandlerPtr = &(vCanCcCdm_CommunicationHandler.MessageReceiveHandler);

  /* ----- Implementation ----------------------------------------------- */
  if (receiveHandlerPtr->ReceiveStatus != VCANCCCDM_RECEPTION_STOPPED)
  {
    const vCanCcCdm_NotificationFunctionsType* callbackFunctions = receiveHandlerPtr->CallbackFuntionTablePtr;

    /* H108Data */
    if (H108Data.IdentifierOfWeldingDetection.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H108WeldingDetected != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H108WeldingDetected();
      }
      H108Data.IdentifierOfWeldingDetection.Changed = FALSE;
    }
    if (H108Data.AvailableOutputCurrent.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H108OutputCurrent != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H108OutputCurrent(H108Data.AvailableOutputCurrent.Value);
      }
      H108Data.AvailableOutputCurrent.Changed = FALSE;
    }
    if (H108Data.ThresholdVoltage.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H108ThresholdVoltage != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H108ThresholdVoltage(H108Data.ThresholdVoltage.Value);
      }
      H108Data.ThresholdVoltage.Changed = FALSE;
    }

    /* H109Data */
    if (H109Data.PresentVoltage.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H109PresentVoltage != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H109PresentVoltage(H109Data.PresentVoltage.Value);
      }
      H109Data.PresentVoltage.Changed = FALSE;
    }
    if (H109Data.PresentChargingCurrent.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H109PresentCurrent != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H109PresentCurrent(H109Data.PresentChargingCurrent.Value);
      }
      H109Data.PresentChargingCurrent.Changed = FALSE;
    }
    if (H109Data.StatusFaultFlags.Changed)
    {
      if (callbackFunctions->vCanCcCdm_H109StatusFaultFlags != NULL_PTR)
      {
        callbackFunctions->vCanCcCdm_H109StatusFaultFlags(H109Data.StatusFaultFlags.Value);
      }
      H109Data.StatusFaultFlags.Changed = FALSE;
    }
  }
}
#endif


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  vCanCcCdm_InitMemory()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  vCanCcCdm_ModuleInitialized = VCANCCCDM_UNINIT;
#endif
}


/***********************************************************************************************************************
 *  vCanCcCdm_Init()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* #100 Check if component is uninitialized */
  /* #200 Check Value parameter for NULL pointer */

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == (uint8)VCANCCCDM_INIT)
  {
    errorId = VCANCCCDM_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    vCanCcCdm_CommunicationHandler.ProtocolStatus = VCANCCCDM_PROTOCOL_NOT_IN_SYNC;
    vCanCcCdm_CommunicationHandler.CommunicationStatus = VCANCCCDM_COMMUNICATION_NOT_STARTED;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.NextMessageToBeSent = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.MessageBufferEmpty = FALSE;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.InitialValueFunctionPtr = &vCanCcCdm_InitialValuesFunction;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.CanIfTxPduIdPtr = &vCanCcCdm_CanIfTxPduIds;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_MESSAGES_NOT_RECEIVED;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.NoCheckCounter = 1;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.MessageSkippedCounter = 0;
  #if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.CallbackFuntionTablePtr = &vCanCcCdm_NotifFuncTable;
  #else
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.CallbackFuntionTablePtr = NULL_PTR;
  #endif
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ComIfHandleIdPtrType = &vCanCcCdm_ComIfHandleIds;
    vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter = 0;
    vCanCcCdm_CommunicationHandler.Counter_100ms = 0;

#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
    vCanCcCdm_ModuleInitialized = (uint8)VCANCCCDM_INIT;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_INIT, errorId);
  }
#else
   VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}


/***********************************************************************************************************************
 *  vCanCcCdm_UnInit()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_DeInit( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* #100 Check if component is uninitialized */
  /* #200 Check Value parameter for NULL pointer */

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == (uint8)VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    vCanCcCdm_CommunicationHandler.ProtocolStatus = VCANCCCDM_PROTOCOL_NOT_IN_SYNC;
    vCanCcCdm_CommunicationHandler.CommunicationStatus = VCANCCCDM_COMMUNICATION_NOT_STARTED;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.NextMessageToBeSent = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.MessageBufferEmpty = FALSE;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_MESSAGES_NOT_RECEIVED;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.NoCheckCounter = 1;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.MessageSkippedCounter = 0;
    vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter = 0;
    vCanCcCdm_CommunicationHandler.Counter_100ms = 0;

#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
    vCanCcCdm_ModuleInitialized = (uint8)VCANCCCDM_UNINIT;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_DEINIT, errorId);
  }
#else
   VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetUsedProtocolVersion()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ProtocolStatusType, VCANCCCDM_CODE) vCanCcCdm_GetUsedProtocolVersion(vCanCcCdm_ProtocolNumberPtrType Value)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = VCANCCCDM_PROTOCOL_NOT_IN_SYNC;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check output parameter */
  if (vCanCcCdm_ModuleInitialized == (uint8)VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (Value == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *Value = H102Data.ChademoProtocolNumber;
    retVal = vCanCcCdm_CommunicationHandler.ProtocolStatus;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_USED_PROTOCOL_VERSION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_StartCanCommunication()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_StartCanCommunication(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_InitialValuesType initValues = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (vCanCcCdm_CommunicationHandler.CommunicationStatus == VCANCCCDM_COMMUNICATION_STARTED)
  {
    errorId = VCANCCCDM_E_COM_ALREADY_STARTED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (VCANCCCDM_USE_BSWM_FOR_COM_REQUEST == STD_ON)
    BswM_RequestMode(VCANCCCDM_BSWM_COM_REQUESTER, VCANCCCDM_BSWM_COM_REQUESTED_MODE_COM);
#endif
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.InitialValueFunctionPtr->vCanCcCdm_GetInitialValues(&initValues);

    vCanCcCdm_CopyInitialValues(initValues);

    vCanCcCdm_CommunicationHandler.CommunicationStatus = VCANCCCDM_COMMUNICATION_STARTED;

    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.NextMessageToBeSent = Message100;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.MessageBufferEmpty = TRUE;

    vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter = 0;

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_START_CAN_COMMUNICATION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_TerminateCanCommunication()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_TerminateCanCommunication(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (vCanCcCdm_CommunicationHandler.CommunicationStatus == VCANCCCDM_COMMUNICATION_NOT_STARTED)
  {
    errorId = VCANCCCDM_E_COM_NOT_STARTED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (VCANCCCDM_USE_BSWM_FOR_COM_REQUEST == STD_ON)
    BswM_RequestMode(VCANCCCDM_BSWM_COM_REQUESTER, VCANCCCDM_BSWM_COM_REQUESTED_MODE_NO_COM);
#endif
    vCanCcCdm_CommunicationHandler.ProtocolStatus = VCANCCCDM_PROTOCOL_NOT_IN_SYNC;
    vCanCcCdm_CommunicationHandler.CommunicationStatus = VCANCCCDM_COMMUNICATION_NOT_STARTED;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.NextMessageToBeSent = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.MessageBufferEmpty = FALSE;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_MESSAGES_NOT_RECEIVED;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage = InvalidMessage;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109Counter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109CounterStamp_100ms = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109NotReceivedCounter = 0;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.NoCheckCounter = 1;
    vCanCcCdm_CommunicationHandler.MessageReceiveHandler.MessageSkippedCounter = 0;
    vCanCcCdm_CommunicationHandler.MainFunctionTransmitCounter = 0;
    vCanCcCdm_CommunicationHandler.Counter_100ms = 0;

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_TERMINATE_CAN_COMMUNICATION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/***********************************************************************************************************************
 * vCanCcCdm_MainFunction()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(vCanCcCdm_CommunicationHandler.CommunicationStatus == VCANCCCDM_COMMUNICATION_STARTED)
    {
      vCanCcCdm_ProcessTransmitJobs();

      vCanCcCdm_ProcessReceiveJobs();
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
      vCanCcCdm_CheckUpdatedReceiveValues();
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_MAIN_FUNCTION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateMinimumChargeCurrent()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateMinimumChargeCurrent(vCanCcCdm_CurrentType MinChargeCurrent)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H100Data.MinumumCurrent = MinChargeCurrent;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_MINIMUM_CHARGE_CURRENT, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateEstimatedChargingTime()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateEstimatedChargingTime(vCanCcCdm_TimeType ChargingTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H101Data.EstimatedChargingTime_1min = ChargingTime;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_ESTIMATED_CHARGING_TIME, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateChargingCurrentRequest()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateChargingCurrentRequest(vCanCcCdm_CurrentType CurrentRequest)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H102Data.ChargingCurrentRequest = CurrentRequest;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_CHARGING_CURRENT_REQUEST, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_SetVehicleFaultFlags()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_SetVehicleFaultFlags(vCanCcCdm_FlagByteType FaultFlags)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FaultFlags > 0x1F)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H102Data.FaultFlags = FaultFlags;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_SET_VEHICLE_FAULT_FLAGS, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_SetVehicleStatusFlags()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_SetVehicleStatusFlags(vCanCcCdm_FlagByteType StatusFlags)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (StatusFlags > 0x1F)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H102Data.StatusFlags = StatusFlags;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_SET_VEHICLE_STATUS_FLAGS, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateStateOfCharge()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateStateOfCharge(vCanCcCdm_StateOfChargeType StateOfCharge)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    H102Data.StateOfCharge = StateOfCharge;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_STATE_OF_CHARGE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryOvervoltageFlag()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryOvervoltageFlag(vCanCcCdm_FlagType FaultFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (FaultFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (FaultFlag)
    {
      H102Data_FaultFlagsSetBatteryOvervoltage();
    }
    else
    {
      H102Data_FaultFlagsResetBatteryOvervoltage();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_BATTERY_OVERVOLTAGE_FLAG, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryUndervoltageFlag()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryUndervoltageFlag(vCanCcCdm_FlagType FaultFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (FaultFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (FaultFlag)
    {
      H102Data_FaultFlagsSetBatteryUndervoltage();
    }
    else
    {
      H102Data_FaultFlagsResetBatteryUndervoltage();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_BATTERY_UNDERVOLTAGE_FLAG, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryCurrentDeviationErrorFlag()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryCurrentDeviationErrorFlag(vCanCcCdm_FlagType FaultFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (FaultFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (FaultFlag)
    {
      H102Data_FaultFlagsSetBatteryCurrentDeviationError();
    }
    else
    {
      H102Data_FaultFlagsResetBatteryCurrentDeviationError();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_BATTERY_CURRENT_DEVIATION_ERROR_FLAG, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateHighBatteryTemperatureFlag()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateHighBatteryTemperatureFlag(vCanCcCdm_FlagType FaultFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (FaultFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (FaultFlag)
    {
      H102Data_FaultFlagsSetHighBatteryTemperature();
    }
    else
    {
      H102Data_FaultFlagsResetHighBatteryTemperature();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_HIGH_BATTERY_TEMPERATURE_FLAG, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateBatteryVoltageDeviationErrorFlag()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateBatteryVoltageDeviationErrorFlag(vCanCcCdm_FlagType FaultFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }

  else if (FaultFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (FaultFlag)
    {
      H102Data_FaultFlagsSetBatteryVoltageDeviationError();
    }
    else
    {
      H102Data_FaultFlagsResetBatteryVoltageDeviationError();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_BATTERY_VOLTAGE_DEVIATION_ERROR_FLAG, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleChargingEnabled()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleChargingEnabled(vCanCcCdm_FlagType StatusFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (StatusFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (StatusFlag)
    {
      H102Data_StatusFlagsSetVehicleChargingEnabled();
    }
    else
    {
      H102Data_StatusFlagsResetVehicleChargingEnabled();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_VEHICLE_CHARGING_ENABLED, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleShiftPosition()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleShiftPosition(vCanCcCdm_FlagType StatusFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (StatusFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (StatusFlag)
    {
      H102Data_StatusFlagsSetVehicleShiftPosition();
    }
    else
    {
      H102Data_StatusFlagsResetVehicleShiftPosition();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_VEHICLE_SHIFT_POSITION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateChargingSystemError()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateChargingSystemError(vCanCcCdm_FlagType StatusFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (StatusFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (StatusFlag)
    {
      H102Data_StatusFlagsSetChargingSystemError();
    }
    else
    {
      H102Data_StatusFlagsResetChargingSystemError();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_CHARGING_SYSTEM_ERROR, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateVehicleStatus()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateVehicleStatus(vCanCcCdm_FlagType StatusFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (StatusFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (StatusFlag)
    {
      H102Data_StatusFlagsSetVehicleStatus();
    }
    else
    {
      H102Data_StatusFlagsResetVehicleStatus();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_VEHICLE_STATUS, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_UpdateNormalStopRequestBeforeCharging()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(Std_ReturnType, VCANCCCDM_CODE) vCanCcCdm_UpdateNormalStopRequestBeforeCharging(vCanCcCdm_FlagType StatusFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (StatusFlag > 0x01)
  {
    errorId = VCANCCCDM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (StatusFlag)
    {
      H102Data_StatusFlagsSetNormalStopRequestBeforeCharging();
    }
    else
    {
      H102Data_StatusFlagsResetNormalStopRequestBeforeCharging();
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_UPDATE_NORMAL_STOP_REQUEST_BEFORE_CHARGING, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetIdentifierOfWeldingDetection()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetIdentifierOfWeldingDetection(vCanCcCdm_WeldingDetectionPtrType IdOfWeldingDetectionPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (IdOfWeldingDetectionPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *IdOfWeldingDetectionPtr = H108Data.IdentifierOfWeldingDetection.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_IDENTIFIER_OF_WELDING_DETECTION, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetAvailableOutputVoltage()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetAvailableOutputVoltage(vCanCcCdm_VoltagePtrType VoltagePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (VoltagePtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *VoltagePtr = H108Data.AvailableOutputVoltage;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_AVAILABLE_OUTPUT_VOLTAGE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetAvailableOutputCurrent()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetAvailableOutputCurrent(vCanCcCdm_CurrentPtrType CurrentPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (CurrentPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *CurrentPtr = H108Data.AvailableOutputCurrent.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_AVAILABLE_OUTPUT_CURRENT, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetThresholdVoltage()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetThresholdVoltage(vCanCcCdm_VoltagePtrType VoltagePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (VoltagePtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *VoltagePtr = H108Data.ThresholdVoltage.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_THRESHOLD_VOLTAGE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetPresentChargingVoltage()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetPresentChargingVoltage(vCanCcCdm_VoltagePtrType VoltagePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (VoltagePtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *VoltagePtr = H109Data.PresentVoltage.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_PRESENT_CHARGING_VOLTAGE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetPresentChargingCurrent()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetPresentChargingCurrent(vCanCcCdm_CurrentPtrType CurrentPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (CurrentPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *CurrentPtr = H109Data.PresentChargingCurrent.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_PRESENT_CHARGING_CURRENT, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetStateFaultsFlagByte()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetStateFaultsFlagByte(vCanCcCdm_FlagBytePtrType FlagBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagBytePtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *FlagBytePtr = H109Data.StatusFaultFlags.Value;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_STATE_FAULTS_FLAG_BYTE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}


/***********************************************************************************************************************
 *  vCanCcCdm_GetRemainingChargingTime()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetRemainingChargingTime(vCanCcCdm_TimePtrType TimePtr, vCanCcCdm_TimeUnitPtrType UnitPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (TimePtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else if (UnitPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(H109Data.RemainingChargingTime_10s == 0xFF)
    {
      *TimePtr = H109Data.RemainingChargingTime_1min;
      *UnitPtr = VCANCCCDM_TIME_UNIT_1MIN;
    }
    else
    {
      *TimePtr = H109Data.RemainingChargingTime_10s;
      *UnitPtr = VCANCCCDM_TIME_UNIT_10S;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_REMAINING_CHARGING_TIME, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargerStatus()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargerStatus(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    *FlagPtr = (H109Data.StatusFaultFlags.Value & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_CHARGER_STATUS, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargerError()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargerError(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_FlagByteType FaultFlagByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FaultFlagByte = H109Data.StatusFaultFlags.Value;
    *FlagPtr = ((FaultFlagByte >> 1) & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_CHARGER_ERROR, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetEnergizingState()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetEnergizingState(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_FlagByteType FaultFlagByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FaultFlagByte = H109Data.StatusFaultFlags.Value;
    *FlagPtr = ((FaultFlagByte >> 2) & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_ENERGIZING_STATE, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetBatteryIncompatibility()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetBatteryIncompatibility(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_FlagByteType FaultFlagByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FaultFlagByte = H109Data.StatusFaultFlags.Value;
    *FlagPtr = ((FaultFlagByte >> 3) & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_BATTERY_INCOMPATIBILITY, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargingSystemError()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargingSystemError(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_FlagByteType FaultFlagByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FaultFlagByte = H109Data.StatusFaultFlags.Value;
    *FlagPtr = ((FaultFlagByte >> 4) & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_CHARGING_SYSTEM_ERROR, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



/***********************************************************************************************************************
 *  vCanCcCdm_GetChargingStopControl()
 **********************************************************************************************************************/
/*!
 * ToDo
 */
FUNC(vCanCcCdm_ReveiveStatusType, VCANCCCDM_CODE) vCanCcCdm_GetChargingStopControl(vCanCcCdm_FlagPtrType FlagPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  vCanCcCdm_FlagByteType FaultFlagByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (vCanCcCdm_ModuleInitialized == VCANCCCDM_UNINIT)
  {
    errorId = VCANCCCDM_E_UNINIT;
  }
  else if (FlagPtr == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FaultFlagByte = H109Data.StatusFaultFlags.Value;
    *FlagPtr = ((FaultFlagByte >> 5) & 0x01);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_CHARGING_STOP_CONTROL, errorId);
  }
#else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus;
}



#if (VCANCCCDM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  vCanCcCdm_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 ToDo
 */
FUNC(void, VCANCCCDM_CODE) vCanCcCdm_GetVersionInfo(vCanCcCdm_VersionInfoPtrType versioninfo)
{
  uint8 errorId = VCANCCCDM_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (VCANCCCDM_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = VCANCCCDM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (VCANCCCDM_VENDOR_ID);
    versioninfo->moduleID = (VCANCCCDM_MODULE_ID);
    versioninfo->sw_major_version = (VCANCCCDM_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (VCANCCCDM_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (VCANCCCDM_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (VCANCCCDM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCCDM_E_NO_ERROR)
  {
    vCanCcCdm_ReportDet(VCANCCCDM_SID_GET_VERSION_INFO, errorId);
  }
# else
  VCANCCCDM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
}
#endif


/***********************************************************************************************************************
 *  vCanCcCdm_RxIndication()
 **********************************************************************************************************************/
/*!
 ToDo
 */
FUNC(void, CANIF_APPL_MSG_CODE) vCanCcCdm_RxIndication (VAR(PduIdType, AUTOMATIC) CanRxPduId, P2CONST(uint8, AUTOMATIC, CANIF_UL_STANDARD_VAR) CanSduPtr)
{
  uint16 availableOutputVoltage_temp;
  uint16 thresholdVoltage_temp;
  uint16 presentVoltage_temp;

  if (vCanCcCdm_CommunicationHandler.CommunicationStatus == VCANCCCDM_COMMUNICATION_STARTED)
  {
    if (CanRxPduId == vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ComIfHandleIdPtrType->Message108TxPduId)
    {
      availableOutputVoltage_temp = (uint16)((((uint16)CanSduPtr[2]) << 8) | (uint16)CanSduPtr[1]);
      thresholdVoltage_temp = (uint16)((((uint16)CanSduPtr[5]) << 8) | (uint16)CanSduPtr[4]);

      if (H108Data.IdentifierOfWeldingDetection.Value != CanSduPtr[0])
      {
        H108Data.IdentifierOfWeldingDetection.Value = CanSduPtr[0];
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter != 0)
        {
          H108Data.IdentifierOfWeldingDetection.Changed = TRUE;
        }
#endif
      }

      if (H108Data.AvailableOutputVoltage != availableOutputVoltage_temp)
      {
        H108Data.AvailableOutputVoltage = availableOutputVoltage_temp;
      }

      if (H108Data.AvailableOutputCurrent.Value != CanSduPtr[3])
      {
        H108Data.AvailableOutputCurrent.Value = CanSduPtr[3];
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter != 0)
        {
          H108Data.AvailableOutputCurrent.Changed = TRUE;
        }
#endif
      }

      if (H108Data.ThresholdVoltage.Value != thresholdVoltage_temp)
      {
        H108Data.ThresholdVoltage.Value = thresholdVoltage_temp;
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter != 0)
        {
          H108Data.ThresholdVoltage.Changed = TRUE;
        }
#endif
      }

#if (VCANCCCDM_CHECK_SKIPPED_MESSAGES == STD_ON)
      /* Monitor the correct reception of Message108 */
      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage == Message108)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.MessageSkippedCounter++;
      }
#endif

      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus == VCANCCCDM_MESSAGES_NOT_RECEIVED)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_MESSAGE109_NOT_RECEIVED;
      }
      else if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus == VCANCCCDM_MESSAGE108_NOT_RECEIVED)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_RUNNING;
      }

      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108Counter++;
      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage = Message108;

#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter++;
      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter > 1)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message108RxIndicationCounter = 1;
      }
#endif
    }
    else if (CanRxPduId == vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ComIfHandleIdPtrType->Message109TxPduId)
    {
      presentVoltage_temp = (uint16)((((uint16)CanSduPtr[2]) << 8) | (uint16)CanSduPtr[1]);

      if (H109Data.ChademoProtocolNumber != CanSduPtr[0])
      {
        H109Data.ChademoProtocolNumber = CanSduPtr[0];
      }

      if (H109Data.PresentVoltage.Value != presentVoltage_temp)
      {
        H109Data.PresentVoltage.Value = presentVoltage_temp;
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter != 0)
        {
          H109Data.PresentVoltage.Changed = TRUE;
        }
#endif
      }

      if (H109Data.PresentChargingCurrent.Value != CanSduPtr[3])
      {
        H109Data.PresentChargingCurrent.Value = CanSduPtr[3];
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter != 0)
        {
          H109Data.PresentChargingCurrent.Changed = TRUE;
        }
#endif
      }

      if (H109Data.StatusFaultFlags.Value != CanSduPtr[5])
      {
        H109Data.StatusFaultFlags.Value = CanSduPtr[5];
#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
        if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter != 0)
        {
          H109Data.StatusFaultFlags.Changed = TRUE;
        }
#endif
      }

      if (H109Data.RemainingChargingTime_10s != CanSduPtr[6])
      {
        H109Data.RemainingChargingTime_10s = CanSduPtr[6];
      }

      if (H109Data.RemainingChargingTime_1min != CanSduPtr[7])
      {
        H109Data.RemainingChargingTime_1min = CanSduPtr[7];
      }

      if (vCanCcCdm_CommunicationHandler.ProtocolStatus == VCANCCCDM_PROTOCOL_NOT_IN_SYNC)
      {
        if (H109Data.ChademoProtocolNumber < H102Data.ChademoProtocolNumber)
        {
          H102Data.ChademoProtocolNumber = H109Data.ChademoProtocolNumber;
        }
        vCanCcCdm_CommunicationHandler.ProtocolStatus = VCANCCCDM_PROTOCOL_IN_SYNC;
      }

#if (VCANCCCDM_CHECK_SKIPPED_MESSAGES == STD_ON)
      /* Monitor the correct reception of Message109 */
      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage == Message109)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.MessageSkippedCounter++;
      }
#endif

      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus == VCANCCCDM_MESSAGES_NOT_RECEIVED)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_MESSAGE108_NOT_RECEIVED;
      }
      else if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus == VCANCCCDM_MESSAGE109_NOT_RECEIVED)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.ReceiveStatus = VCANCCCDM_RUNNING;
      }

      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109Counter++;
      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.RecentlyReceivedMessage = Message109;

#if (VCANCCCDM_NOTIFIY_APPLICATION == STD_ON)
      vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter++;
      if (vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter > 1)
      {
        vCanCcCdm_CommunicationHandler.MessageReceiveHandler.Message109RxIndicationCounter = 1;
      }
#endif
    }
    else
    {
      /* Do nothing, because this message is not defined in vCanCcCdm) */
    }
  }
}


/***********************************************************************************************************************
 *  vCanCcCdm_TxConfirmation()
 **********************************************************************************************************************/
/*!
 ToDo
 */
FUNC(void, CANIF_APPL_MSG_CODE) vCanCcCdm_TxConfirmation(VAR(PduIdType, AUTOMATIC) CanTxPduId)
{
  if (vCanCcCdm_CommunicationHandler.CommunicationStatus == VCANCCCDM_COMMUNICATION_STARTED)
  {
    vCanCcCdm_CommunicationHandler.MessageTransmitHandler.MessageBufferEmpty = TRUE;
  }
  VCANCCCDM_DUMMY_STATEMENT(CanTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
}


#define VCANCCCDM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* Justification for module-specific MISRA deviations:



*/


/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_vCanCcCdm_3
  \DESCRIPTION Any text
  \COUNTERMEASURE \N ANY

\ID SBSW_vCanCcCdm_60
  \DESCRIPTION text ToDo text
  \COUNTERMEASURE \N text

SBSW_JUSTIFICATION_END */


/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

  \ID COV_VCANCCCDM_COMPATIBILITY
    \ACCEPT TX
    \REASON [COV_MSR_COMPATIBILITY]

  \ID COV_VCANCCCDM_MISRA
    \ACCEPT TX
    \ACCEPT XX
    \REASON [COV_MSR_MISRA]

END_COVERAGE_JUSTIFICATION
*/


/***********************************************************************************************************************
 *  END OF FILE: vCanCcCdm.c
 **********************************************************************************************************************/
