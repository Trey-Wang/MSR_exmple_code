
#if !defined (VCANCCCDM_TYPES_H)
# define VCANCCCDM_TYPES_H

typedef uint16 vCanCcCdm_VoltageType;
typedef uint16 vCanCcCdm_CapacityType;
typedef uint8  vCanCcCdm_CurrentType;
typedef uint8  vCanCcCdm_FlagType;
typedef uint8  vCanCcCdm_FlagByteType;
typedef uint8  vCanCcCdm_TimeType;
typedef uint8  vCanCcCdm_ProtocolNumberType;
typedef uint8  vCanCcCdm_WeldingDetectionType;
typedef uint8  vCanCcCdm_StateOfChargeType;
typedef uint8  vCanCcCdm_ChargeRateType;

/*! Type for ... */
typedef enum {
  VCANCCCDM_TIME_UNIT_10S = 0u, /*!<  */
  VCANCCCDM_TIME_UNIT_1MIN = 1u /*!<  */
} vCanCcCdm_TimeUnitType;

typedef struct
{
  vCanCcCdm_CurrentType       MinumumCurrent;
  vCanCcCdm_VoltageType       MaximumBatteryVoltage;
  vCanCcCdm_TimeType          MaximumChargingTime_10s;
  vCanCcCdm_TimeType          MaximumChargingTime_1min;
  vCanCcCdm_TimeType          EstimatedChargingTime_1min;
  vCanCcCdm_CapacityType      TotalCapacityOfTractionBattery;
  vCanCcCdm_VoltageType       TargetBatteryVoltage;
  vCanCcCdm_CurrentType       ChargingCurrentRequest;
  vCanCcCdm_FlagType          VehicleShiftPosition;
  vCanCcCdm_StateOfChargeType StateOfCharge;
} vCanCcCdm_InitialValuesType;

typedef P2VAR(vCanCcCdm_VoltageType,  AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_VoltagePtrType;
typedef P2VAR(vCanCcCdm_CurrentType,  AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_CurrentPtrType;
typedef P2VAR(vCanCcCdm_FlagType,     AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_FlagPtrType;
typedef P2VAR(vCanCcCdm_FlagByteType, AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_FlagBytePtrType;
typedef P2VAR(vCanCcCdm_TimeType,     AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_TimePtrType;
typedef P2VAR(vCanCcCdm_ProtocolNumberType,   AUTOMATIC, VCANCCCDM_APPL_VAR)  vCanCcCdm_ProtocolNumberPtrType;
typedef P2VAR(vCanCcCdm_WeldingDetectionType, AUTOMATIC, VCANCCCDM_APPL_VAR)  vCanCcCdm_WeldingDetectionPtrType;
typedef P2VAR(vCanCcCdm_TimeUnitType, AUTOMATIC, VCANCCCDM_APPL_VAR)          vCanCcCdm_TimeUnitPtrType;
typedef P2VAR(vCanCcCdm_InitialValuesType, AUTOMATIC, VCANCCCDM_APPL_VAR)     vCanCcCdm_InitialValuesPtrType;
typedef P2VAR(Std_VersionInfoType, AUTOMATIC, VCANCCCDM_APPL_VAR)             vCanCcCdm_VersionInfoPtrType;

typedef struct
{
  vCanCcCdm_CurrentType         MinumumCurrent;
  vCanCcCdm_VoltageType         MaximumBatteryVoltage;
  vCanCcCdm_ChargeRateType      ChargedRateConstantValue;
} vCanCcCdm_H100DataType;

typedef struct
{
  vCanCcCdm_TimeType            MaximumChargingTime_10s;
  vCanCcCdm_TimeType            MaximumChargingTime_1min;
  vCanCcCdm_TimeType            EstimatedChargingTime_1min;
  vCanCcCdm_CapacityType        TotalCapacityOfTractionBattery;
} vCanCcCdm_H101DataType;

typedef struct
{
  vCanCcCdm_ProtocolNumberType  ChademoProtocolNumber;
  vCanCcCdm_VoltageType         TargetBatteryVoltage;
  vCanCcCdm_CurrentType         ChargingCurrentRequest;
  vCanCcCdm_FlagByteType        FaultFlags;
  vCanCcCdm_FlagByteType        StatusFlags;
  vCanCcCdm_StateOfChargeType   StateOfCharge;
} vCanCcCdm_H102DataType;

typedef struct
{
  vCanCcCdm_WeldingDetectionType Value;
  vCanCcCdm_FlagType             Changed;
} vCanCcCdm_WeldingDetectionTupleType;

typedef struct
{
  vCanCcCdm_VoltageType Value;
  vCanCcCdm_FlagType    Changed;
} vCanCcCdm_VoltageTupleType;

typedef struct
{
  vCanCcCdm_CurrentType Value;
  vCanCcCdm_FlagType    Changed;
} vCanCcCdm_CurrentTupleType;

typedef struct
{
  vCanCcCdm_FlagByteType  Value;
  vCanCcCdm_FlagType      Changed;
} vCanCcCdm_ChargerStatusFaultFlagsTupleType;

typedef struct
{
  vCanCcCdm_FlagByteType  Value;
  vCanCcCdm_FlagType      Changed;
} vCanCcCdm_FlagByteTypleType;

typedef struct
{
  vCanCcCdm_WeldingDetectionTupleType IdentifierOfWeldingDetection;
  vCanCcCdm_VoltageType               AvailableOutputVoltage;
  vCanCcCdm_CurrentTupleType          AvailableOutputCurrent;
  vCanCcCdm_VoltageTupleType          ThresholdVoltage;
} vCanCcCdm_H108DataType;

typedef struct
{
  vCanCcCdm_ProtocolNumberType        ChademoProtocolNumber;
  vCanCcCdm_VoltageTupleType          PresentVoltage;
  vCanCcCdm_CurrentTupleType          PresentChargingCurrent;
  vCanCcCdm_FlagByteTypleType         StatusFaultFlags;
  vCanCcCdm_TimeType                  RemainingChargingTime_10s;
  vCanCcCdm_TimeType                  RemainingChargingTime_1min;
} vCanCcCdm_H109DataType;

/*! Type for ... */
typedef enum {
  VCANCCCDM_PROTOCOL_NOT_IN_SYNC = 0u, /*!<  */
  VCANCCCDM_PROTOCOL_IN_SYNC = 1u  /*!<  */
} vCanCcCdm_ProtocolStatusType;

/*! Type for ... */
typedef enum {
  VCANCCCDM_UNINIT = 0u, /*!<  */
  VCANCCCDM_INIT = 1u /*!<  */
} vCanCcCdm_StatusType;

/*! Type for ... */
typedef enum {
  VCANCCCDM_COMMUNICATION_NOT_STARTED = 0u, /*!<  */
  VCANCCCDM_COMMUNICATION_STARTED = 1u /*!<  */
} vCanCcCdm_CommunicationStatusType;

/*! Type for ... */
typedef enum {
  VCANCCCDM_MESSAGES_NOT_RECEIVED = 0u, /*!<  */
  VCANCCCDM_MESSAGE108_NOT_RECEIVED = 1u, /*!<  */
  VCANCCCDM_MESSAGE109_NOT_RECEIVED = 2u, /*!<  */
  VCANCCCDM_RUNNING = 3u, /*!<  */
  VCANCCCDM_MESSAGES_APERIODIC = 4u, /*!<  */
  VCANCCCDM_RECEPTION_STOPPED = 5u /*!<  */
} vCanCcCdm_ReveiveStatusType;

/*! Type for ... */
typedef enum {
  Message100 = 0, /*!<  */
  Message101 = 1, /*!<  */
  Message102 = 2, /*!<  */
  Message108 = 3, /*!<  */
  Message109 = 4, /*!<  */
  InvalidMessage = 0xFF /*!<  */
} vCanCcCdm_MessageType;

typedef struct {
  /*!< H108 Callbacks */
  /*!< Welding detected callback, NULL_PTR when not used */
  void (*vCanCcCdm_H108WeldingDetected) (void);
  /*!< Output current callback, NULL_PTR when not used */
  void (*vCanCcCdm_H108OutputCurrent) (vCanCcCdm_CurrentType);
  /*!< Threshold voltage callback, NULL_PTR when not used */
  void (*vCanCcCdm_H108ThresholdVoltage) (vCanCcCdm_VoltageType);
  /*!< H109 Callbacks */
  /*!< Present voltage callback, NULL_PTR when not used */
  void (*vCanCcCdm_H109PresentVoltage) (vCanCcCdm_VoltageType);
  /*!< Present current callback, NULL_PTR when not used */
  void (*vCanCcCdm_H109PresentCurrent) (vCanCcCdm_CurrentType);
  /*!< StatusFaultFlags callback, NULL_PTR when not used */
  void (*vCanCcCdm_H109StatusFaultFlags) (vCanCcCdm_FlagByteType);
  /*!< NoDataReception callback, NULL_PTR when not used */
  void (*vCanCcCdm_NoDataReception) (void);
} vCanCcCdm_NotificationFunctionsType;

/*! Type for ... */
typedef struct {
  const PduIdType Message108TxPduId;
  const PduIdType Message109TxPduId;
} vCanCcCdm_ComIfHandleIdType;

/*! Type for ... */
typedef struct {
  vCanCcCdm_ReveiveStatusType ReceiveStatus;
  vCanCcCdm_MessageType RecentlyReceivedMessage;
  uint8 Message108Counter;
  uint8 Message108CounterStamp_100ms;
  uint8 Message108NotReceivedCounter;
  uint8 Message108RxIndicationCounter;
  uint8 Message109Counter;
  uint8 Message109CounterStamp_100ms;
  uint8 Message109NotReceivedCounter;
  uint8 Message109RxIndicationCounter;
  uint8 NoCheckCounter;
  uint8 MessageSkippedCounter;
  P2CONST(vCanCcCdm_NotificationFunctionsType, VCANCCCDM_CONST, VCANCCCDM_APPL_CONST) CallbackFuntionTablePtr;
  P2CONST(vCanCcCdm_ComIfHandleIdType, VCANCCCDM_CONST, VCANCCCDM_APPL_CONST) ComIfHandleIdPtrType;
} vCanCcCdm_MessageReceiveHandlerType;

/*! Type for ... */
typedef struct {
  /*!< Initial data request function */
  void (*vCanCcCdm_GetInitialValues) (vCanCcCdm_InitialValuesType*);
} vCanCcCdm_InitialValuesFunctionType;

/*! Type for ... */
typedef struct {
  const PduIdType Message100TxPduId;
  const PduIdType Message101TxPduId;
  const PduIdType Message102TxPduId;
} vCanCcCdm_CanIfTxPduIdType;

/*! Type for ... */
typedef struct {
  vCanCcCdm_MessageType NextMessageToBeSent;
  boolean MessageBufferEmpty;
  P2CONST(vCanCcCdm_InitialValuesFunctionType, VCANCCCDM_CONST, VCANCCCDM_APPL_CONST) InitialValueFunctionPtr;
  P2CONST(vCanCcCdm_CanIfTxPduIdType, VCANCCCDM_CONST, VCANCCCDM_APPL_CONST) CanIfTxPduIdPtr;
} vCanCcCdm_MessageTransmitHandlerType;


/*! Type for ... */
typedef struct {
  vCanCcCdm_ProtocolStatusType ProtocolStatus;
  vCanCcCdm_CommunicationStatusType CommunicationStatus;
  vCanCcCdm_MessageTransmitHandlerType MessageTransmitHandler;
  vCanCcCdm_MessageReceiveHandlerType MessageReceiveHandler;
  uint8 MainFunctionTransmitCounter;
  uint8 Counter_100ms;
} vCanCcCdm_CommunicationHandlerType;

#endif

