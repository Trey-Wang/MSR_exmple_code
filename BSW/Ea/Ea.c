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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  Ea.c
 *        \brief  EA source file
 *
 *      \details  The EEPROM Abstraction abstracts from the device specific addressing scheme and segmentation and
 *                provides a virtual addressing scheme and segmentation to upper layers as well as a virtually
 *                unlimited number of erase cycles.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DESIGN DECISIONS
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
 *
 *
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Ea.h"
#include "Ea_Cbk.h"

/**** Sub-Component Includes *****************************************************************************************/
#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

/* Data Storage Handler */
#include "Ea_PartitionHandler.h"
#include "Ea_BlockHandler.h"
#include "Ea_DatasetHandler.h"

/* Layer 1 sub-components */
#include "Ea_Layer1_Read.h"
#include "Ea_Layer1_Write.h"
#include "Ea_Layer1_Invalidate.h"
#include "Ea_Layer1_Erase.h"

/* Layer 2 sub-components */
#include "Ea_Layer2_WriteInstance.h"
#include "Ea_Layer2_InstanceFinder.h"
#include "Ea_Layer2_InvalidateInstance.h"

/* Layer 3 sub-component */
#include "Ea_Layer3_ReadManagementBytes.h"

/**********************************************************************************************************************
 *  VERSION CHECK  \trace SPEC-41711
 *********************************************************************************************************************/

#if (   (EA_SW_MAJOR_VERSION != (3u)) \
     || (EA_SW_MINOR_VERSION != (2u)) \
     || (EA_AR_RELEASE_MAJOR_VERSION != (4u)) \
     || (EA_AR_RELEASE_MINOR_VERSION != (0u)))
# error "Version numbers of Ea.c and Ea.h are inconsistent!"
#endif

#if (   (EA_CFG_MAJOR_VERSION != (5u)) \
     || (EA_CFG_MINOR_VERSION != (2u)))
# error "Version numbers of Ea.c and Ea_Cfg.h are inconsistent!"
#endif

#if (   (EA_CBK_MAJOR_VERSION != (3u)) \
     || (EA_CBK_MINOR_VERSION != (2u)))
# error "Version numbers of Ea.c and Ea_Cbk.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* \trace SPEC-41596 */
#define EA_RESERVED_BLOCK_NUMBER_1             (0x0000u)
#define EA_RESERVED_BLOCK_NUMBER_2             (0xFFFFu)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (EA_LOCAL) /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#if !defined (EA_LOCAL_INLINE) /* COV_EA_COMPATIBILITY */
# define EA_LOCAL_INLINE LOCAL_INLINE
#endif

/*---------- FAST INIT TYPES ----------*/
#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef struct
{
  MemIf_JobResultType Ea_JobResult;
  MemIf_StatusType Ea_Status;
}Ea_ComponentParameterType;

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*---------- NO INIT TYPES ----------*/
#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef enum
{
  EA_STATE_UNINIT = 0,
  EA_STATE_IDLE,
  EA_STATE_BUSY
} EEaStateType;

/*!
 * Internal comment removed.
 *
 *
 */
typedef struct
{
  boolean MainFunctionBusy;
  boolean JobRequested;
  boolean CancelRequested;
} Ea_InternalFlagsType;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*---------- FAST INIT VARIABLES ----------*/
#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(Ea_ComponentParameterType, EA_FAST_DATA) Ea_ComponentParameter = {MEMIF_JOB_FAILED, MEMIF_UNINIT};

EA_LOCAL VAR(EEaStateType, EA_APPL_DATA) Ea_StateMachine = EA_STATE_UNINIT;

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*---------- NO INIT VARIABLES ----------*/
#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(Ea_UserJobParameterType, EA_APPL_DATA) Ea_UserJobParameter;

EA_LOCAL VAR(Ea_InternalFlagsType, EA_APPL_DATA) Ea_InternalFlags;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Ea_SetFlagJobRequested()
 *********************************************************************************************************************/
/*!
 * \brief      Sets flag to signal requested job
 * \details    Sets flag to signal requested job
 * \param[in]  value is either TRUE or FALSE
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagJobRequested(boolean value);

/**********************************************************************************************************************
 *  Ea_SetFlagCancelRequested()
 *********************************************************************************************************************/
/*!
 * \brief      Sets flag to signal that cancel is requested
 * \details    Sets flag to signal that cancel is requested
 * \param[in]  value is either TRUE or FALSE
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagCancelRequested(boolean value);

/**********************************************************************************************************************
 *  Ea_SetFlagMainFunctionBusy()
 *********************************************************************************************************************/
/*!
 * \brief      Sets flag to signal that main function is busy
 * \details    Sets flag to signal that main function is busy
 * \param[in]  value is either TRUE or FALSE
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagMainFunctionBusy(boolean value);

/**********************************************************************************************************************
 *  Ea_GetLayer1Status()
 *********************************************************************************************************************/
/*!
 * \brief      Returns status of active layer 1 sub-component
 * \details    Returns status of active layer 1 sub-component
 * \return     Status of sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(MemIf_StatusType, EA_PRIVATE_CODE) Ea_GetLayer1Status(void);

/**********************************************************************************************************************
 *  Ea_GetLayer1JobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns job result of active layer 1 sub-component
 * \details    Returns job result of active layer 1 sub-component
 * \return     JobResult of sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(MemIf_JobResultType, EA_PRIVATE_CODE) Ea_GetLayer1JobResult(void);

/**********************************************************************************************************************
 *  Ea_StartLayer1Job()
 *********************************************************************************************************************/
/*!
 * \brief      Calls start service of layer 1 job
 * \details    Depending on currently requested job, this service calls the
 *             appropriate layer 1 sub-component's start service
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_StartLayer1Job(void);

/**********************************************************************************************************************
 *  Ea_SetUserParameter()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes global struct with user job parameter
 * \details    Initializes global struct with user job parameter which were passed by API service call
 * \param[in]  BlockNumber
 * \param[in]  BlockOffset
 * \param[in]  DataBufferPtr
 * \param[in]  Length
 * \param[in]  JobType
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetUserParameter(uint16 BlockNumber,
                                                             uint16 BlockOffset,
                                                             Ea_DataBufferPtrType DataBufferPtr,
                                                             uint16 Length,
                                                             EEaJobType JobType);

/**********************************************************************************************************************
 *  Ea_SetJobParameter()
 *********************************************************************************************************************/
/*!
 * \brief      Relevant internal parameter are set upon job request
 * \details    Upon job request internal parameter are set, such as status, job result and internal flags
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetJobParameter(void);

/**********************************************************************************************************************
 *  Ea_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes job parameter and calls Ea_StartLayer1Job
 * \details    Initializes job parameter and calls Ea_StartLayer1Job
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_StartJob(void);

/**********************************************************************************************************************
 *  Ea_DetChkIdle()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if EA module is idle
 * \details    Checks if EA module is idle
 * \return     E_OK: EA module is idle
 * \return     E_NOT_OK: EA module is busy/busy_internal or uninit
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkIdle(void);

#if (EA_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  Ea_DetChkInit()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if EA module is initialized
 * \details    Checks if EA module is initialized
 * \return     E_OK: EA module is initialized
 * \return     E_NOT_OK: EA module is not initialized
 * \pre        -
 * \config     Switch EA_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkInit(void);

/**********************************************************************************************************************
 *  Ea_DetChkBlockNumberOk()
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks if BlockNumber is invalid
 * \details    Checks if BlockNumber is either one of the reserved values or if BlockNumber is not configured
 * \param[in]  BlockNumber
 * \return     E_OK: BlockNumber is valid
 * \return     E_NOT_OK: BlockNumber is invalid
 * \pre        -
 * \config     Switch EA_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID);

/**********************************************************************************************************************
 *  Ea_DetChkDataPointerOk()
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks if DataBufferPtr is invalid
 * \details    Checks if DataBufferPtr is NULL_PTR
 * \param[in]  DataBufferPtr
 * \return     E_OK: DataBufferPtr is valid
 * \return     E_NOT_OK: DataBufferPtr is NULL
 * \pre        -
 * \config     Switch EA_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, EA_APPL_DATA) DataBufferPtr);

/**********************************************************************************************************************
 *  Ea_DetChkBlockOffsetOk()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if BlockOffset is out of range
 * \details    Checks if BlockOffset is out of range
 * \param[in]  BlockOffset
 * \return     E_OK: BlockOffset is in range
 * \return     E_NOT_OK: BlockOffset is out of range
 * \pre        -
 * \config     Switch EA_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset);

/**********************************************************************************************************************
 *  Ea_DetChkBlockLengthOk()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if Length is out of range
 * \details    Checks if Length is out of range
 * \param[in]  BlockNumber
 * \param[in]  Length
 * \param[in]  BlockOffset
 * \return     E_OK: Length is in range
 * \return     E_NOT_OK: Length is out of range
 * \pre        -
 * \config     Switch EA_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockLengthOk(uint16 BlockNumber,
        uint16 Length, uint16 BlockOffset);
#endif

/**********************************************************************************************************************
 *  Ea_InitDataHandler()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes all data handlers of storage system
 * \details    Initializes PartitionHandler, BlockHandler and InstanceHandler
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_InitDataHandler(void);

/**********************************************************************************************************************
 *  Ea_ProcessStateMachine()
 *********************************************************************************************************************/
/*!
 * \brief      Processes main state machine
 * \details    Processes main state machine based on current state
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  Ea_FinishJob()
 *********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \trace SPEC-41590, SPEC-41714, SPEC-41594, SPEC-41585, SPEC-41704, SPEC-41635, SPEC-41700, SPEC-41679
 * \trace DSGN-Ea22977
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Ea_SetFlagJobRequested()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagJobRequested(boolean value)
{
  Ea_InternalFlags.JobRequested = value;
}

/**********************************************************************************************************************
 *  Ea_SetFlagCancelRequested()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagCancelRequested(boolean value)
{
  Ea_InternalFlags.CancelRequested = value;
}

/**********************************************************************************************************************
 *  Ea_SetFlagMainfunctionBusy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetFlagMainFunctionBusy(boolean value)
{
  Ea_InternalFlags.MainFunctionBusy = value;
}

/**********************************************************************************************************************
 *  Ea_GetLayer1Status()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(MemIf_StatusType, EA_PRIVATE_CODE) Ea_GetLayer1Status(void)
{
  MemIf_StatusType retVal = MEMIF_IDLE;

  switch (Ea_UserJobParameter.JobType)
  {
    case (EA_READ_JOB):
      retVal = EaRd_GetStatus();
      break;
    case (EA_WRITE_JOB):
      retVal = EaWr_GetStatus();
      break;
    case (EA_INVALIDATE_JOB):
      retVal = EaInv_GetStatus();
      break;
    case (EA_ERASE_JOB):
      retVal = EaEr_GetStatus();
      break;
  } /* PRQA S 2002 */ /* MD_EA_15.3 */
  return retVal;
}

/**********************************************************************************************************************
 *  Ea_GetLayer1JobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(MemIf_JobResultType, EA_PRIVATE_CODE) Ea_GetLayer1JobResult(void)
{
  MemIf_JobResultType retVal = MEMIF_JOB_FAILED;

  switch (Ea_UserJobParameter.JobType)
  {
    case (EA_READ_JOB):
      retVal = EaRd_GetJobResult();
      break;
    case (EA_WRITE_JOB):
      retVal = EaWr_GetJobResult();
      break;
    case (EA_INVALIDATE_JOB):
      retVal = EaInv_GetJobResult();
      break;
    case (EA_ERASE_JOB):
      retVal = EaEr_GetJobResult();
      break;
  } /* PRQA S 2002 */ /* MD_EA_15.3 */
  return retVal;
}

/**********************************************************************************************************************
 *  Ea_StartLayer1Job()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_StartLayer1Job(void)
{
  switch (Ea_UserJobParameter.JobType)
  {
    case (EA_READ_JOB):
      (void) EaRd_StartJob(Ea_UserJobParameter); /* SBSW_EA_02 */
      break;
    case (EA_WRITE_JOB):
      (void) EaWr_StartJob(Ea_UserJobParameter); /* SBSW_EA_02 */
      break;
    case (EA_INVALIDATE_JOB):
      (void) EaInv_StartJob();
      break;
    case (EA_ERASE_JOB):
      (void) EaEr_StartJob();
      break;
  } /* PRQA S 2002 */ /* MD_EA_15.3 */
}

/**********************************************************************************************************************
 *  Ea_SetUserParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetUserParameter(uint16 BlockNumber,
                                                            uint16 BlockOffset,
                                                            Ea_DataBufferPtrType DataBufferPtr,
                                                            uint16 Length,
                                                            EEaJobType JobType)
{
  Ea_UserJobParameter.BlockNumber = BlockNumber;
  Ea_UserJobParameter.BlockOffset = BlockOffset;
  Ea_UserJobParameter.DataBufferPtr = DataBufferPtr;
  Ea_UserJobParameter.Length = Length;
  Ea_UserJobParameter.JobType = JobType;
}

/**********************************************************************************************************************
 *  Ea_SetJobParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_SetJobParameter(void)
{
  Ea_ComponentParameter.Ea_Status = MEMIF_BUSY;
  Ea_ComponentParameter.Ea_JobResult = MEMIF_JOB_PENDING;

  Ea_SetFlagJobRequested(TRUE);
  Ea_SetFlagMainFunctionBusy(TRUE);
}

/**********************************************************************************************************************
 *  Ea_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_StartJob(void)
{
  /* #10 TaskManager has to be initialized with every job in order to clear its service stack */
  EaTm_Init();

  /* #20 DataHandlers have to be initialized in order to set values of current job */
  Ea_InitDataHandler();

  /* #30 Start requested job by initiating appropriate sub-component */
  Ea_StartLayer1Job();

  Ea_StateMachine = EA_STATE_BUSY;
}

/**********************************************************************************************************************
 *  Ea_DetChkIdle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkIdle(void)
{
  return (Std_ReturnType) ((Ea_ComponentParameter.Ea_Status == MEMIF_IDLE) ? E_OK : E_NOT_OK);
}

#if (EA_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  Ea_DetChkInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkInit(void)
{
  return (Std_ReturnType) ((Ea_ComponentParameter.Ea_Status == MEMIF_UNINIT) ? E_NOT_OK : E_OK);
}

/**********************************************************************************************************************
 *  Ea_DetChkBlockNumberOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID)
{
  Std_ReturnType retVal;

  if ((BlockNumber == (uint16)EA_RESERVED_BLOCK_NUMBER_1) ||
      (BlockNumber == (uint16)EA_RESERVED_BLOCK_NUMBER_2))
  {
    retVal = E_NOT_OK;
  }
  /* #10 BlockIndex is zero based, thus (1u) has to be added to BlockIndex */
  else if ((EaBh_GetBlockIndex(BlockNumber) + 1u) > EA_NUMBER_OF_BLOCKS)
  {
    retVal = E_NOT_OK;
  }
  else if (ApiID == (uint8) EA_SID_ERASE_IMMEDIATE_BLOCK)
  {
    /* #20 Erase immediate block API only works with immediate data blocks */
    retVal = ( (EaBh_IsImmediateData(BlockNumber) == FALSE) ? E_NOT_OK : E_OK);
  }
  else
  {
    retVal = E_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  Ea_DetChkDataPointerOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, EA_APPL_DATA) DataBufferPtr)
{
  return (Std_ReturnType) ((DataBufferPtr == NULL_PTR) ? E_NOT_OK : E_OK);
}

/**********************************************************************************************************************
 *  Ea_DetChkBlockOffsetOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset)
{
  return (Std_ReturnType) ((BlockOffset >= EaBh_GetDataLength(BlockNumber)) ? E_NOT_OK : E_OK);
}

/**********************************************************************************************************************
 *  Ea_DetChkBlockLengthOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL_INLINE FUNC(Std_ReturnType, EA_PRIVATE_CODE) Ea_DetChkBlockLengthOk(uint16 BlockNumber,
        uint16 Length, uint16 BlockOffset)
{
  Std_ReturnType retVal;

  if(Length == 0u)
  {
    retVal = E_NOT_OK;
  }
  /* #10 It must be assured that data is only accessed within block bounds */
  else if( EaBh_GetDataLength(BlockNumber) < (Length + BlockOffset) )
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
  }

  return (Std_ReturnType) retVal;
}
#endif

/**********************************************************************************************************************
 *  Ea_InitDataHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_InitDataHandler(void)
{
  EaPh_Init(Ea_UserJobParameter.BlockNumber);
  EaBh_Init(Ea_UserJobParameter.BlockNumber, EaPh_GetPartitionStartAddress());
  EaDh_Init(Ea_UserJobParameter.BlockNumber, EaBh_GetDataLength(Ea_UserJobParameter.BlockNumber),
          EaBh_GetNumberOfInstances(), EaBh_GetBlockStartAddress(), EaPh_GetPartitionAddressAlignment());
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Ea_ProcessStateMachine()
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
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_ProcessStateMachine(void)
{
  switch (Ea_StateMachine)
  {
    /**---------- STATE IDLE ----------------------------------------------------*/
    case EA_STATE_IDLE:
      if (Ea_InternalFlags.CancelRequested == TRUE)
      {
        /* #5 Job requests were not yet processed, thus it is sufficient to clear the internal flag */
        Ea_SetFlagCancelRequested(FALSE);
      }
      else
      {
        /* #7 Requested job has to be started */
        Ea_StartJob();
        Ea_SetFlagJobRequested(FALSE);
      }

      break;

      /**---------- STATE BUSY ----------------------------------------------------*/
    case EA_STATE_BUSY:
      if (Ea_InternalFlags.CancelRequested == TRUE)
      {
        EaTm_CancelTasks();

        /* #10 If no new job was requested during cancellation, MainFunctionBusy flag has to be cleared */
        if (Ea_InternalFlags.JobRequested == FALSE)
        {
          Ea_SetFlagMainFunctionBusy(FALSE);
        }

        Ea_StateMachine = EA_STATE_IDLE;
        Ea_SetFlagCancelRequested(FALSE);
      }
      else
      {
        if (Ea_Eep_GetStatus() == MEMIF_IDLE)
        {
          /* #20 Process sub-components */
          EaTm_ExecuteTask();

          /* #25 Check if job is finished and if there was no cancel request while job processing*/
          if ((Ea_GetLayer1Status() == MEMIF_IDLE) && (Ea_InternalFlags.CancelRequested == FALSE)) /* COV_EA_NO_CODE_INSTRUMENTATION */
          {
            Ea_FinishJob(Ea_GetLayer1JobResult());
          }
        }
      }
      break;
    default: /* COV_EA_MISRA */
      break;
  }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Ea_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) Ea_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 Reset local parameters upon job finish */
  Ea_SetFlagMainFunctionBusy(FALSE);
  Ea_StateMachine = EA_STATE_IDLE;

  Ea_ComponentParameter.Ea_JobResult = JobResult;
  Ea_ComponentParameter.Ea_Status = MEMIF_IDLE;

  /* #20 Depending on configuration, job finish is reported to upper layer via callback hooks */
#if (EA_NVM_POLLING_MODE == STD_OFF)
  if (JobResult == MEMIF_JOB_OK)
  {
    Ea_CbkJobEndNotification(); /* SBSW_EA_05 */
  }
  else
  {
    Ea_CbkJobErrorNotification(); /* SBSW_EA_05 */
  }
#endif
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Ea_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_Init(void)
{
  Ea_ComponentParameter.Ea_JobResult = MEMIF_JOB_OK;
  Ea_ComponentParameter.Ea_Status = MEMIF_IDLE;

  Ea_StateMachine = EA_STATE_IDLE;

  Ea_SetFlagMainFunctionBusy(FALSE);
  Ea_SetFlagJobRequested(FALSE);
  Ea_SetFlagCancelRequested(FALSE);

  /* #10 Initialize EA's sub-components */
  EaRd_Init();
  EaWr_Init();
  EaInv_Init();
  EaEr_Init();
  EaWrInst_Init();
  EaInvInst_Init();
  EaIf_Init();
  EaRdMgmt_Init();
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Ea_Read()
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
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_Read(uint16 BlockNumber, uint16 BlockOffset,
        Ea_DataBufferPtrType DataBufferPtr,uint16 Length)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else
  /* ----- INVALID BLOCK NO CHECK ----------------------------------------------------------------------*/
  /* #20 Perform Invalid Block Number check */
  if (Ea_DetChkBlockNumberOk(BlockNumber, EA_SID_READ) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_NO;
  }
  else
  /* ----- BUFFER POINTER CHECK ------------------------------------------------------------------------*/
  /* #30 Perform Buffer Pointer check */
  if (Ea_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK) /* SBSW_EA_03 */
  {
    errorId = EA_E_INVALID_DATA_POINTER;
  }
  else
  /* ----- BLOCK OFFSET CHECK --------------------------------------------------------------------------*/
  /* #40 Perform Block Offset check */
  if (Ea_DetChkBlockOffsetOk(BlockNumber, BlockOffset) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_OFS;
  }
  else
  /* ----- BLOCK LENGTH CHECK --------------------------------------------------------------------------*/
  /* #50 Perform Block Length check */
  if (Ea_DetChkBlockLengthOk(BlockNumber, Length, BlockOffset) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_LEN;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */

  /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
  /* #60 Perform Busy check. At this point "Not-Idle" is equal to "Busy" since "Busy-internal" is not used */
  if (Ea_DetChkIdle() == E_NOT_OK)
  {
    errorId = EA_E_BUSY;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/
    Ea_SetUserParameter(BlockNumber, BlockOffset, DataBufferPtr, Length, EA_READ_JOB); /* SBSW_EA_03 */
    Ea_SetJobParameter();

    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #70 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_READ, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
/* Ea_Read */

/**********************************************************************************************************************
 *  Ea_Write()
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
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_Write(uint16 BlockNumber, Ea_DataBufferPtrType DataBufferPtr)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

/* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else
  /* ----- INVALID BLOCK NO CHECK ----------------------------------------------------------------------*/
  /* #20 Perform Invalid Block Number check */
  if (Ea_DetChkBlockNumberOk(BlockNumber, EA_SID_WRITE) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_NO;
  }
  else
  /* ----- BUFFER POINTER CHECK ------------------------------------------------------------------------*/
  /* #30 Perform Buffer Pointer check */
  if (Ea_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK) /* SBSW_EA_03 */
  {
    errorId = EA_E_INVALID_DATA_POINTER;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */

  /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
  /* #40 Perform Busy check. At this point "Not-Idle" is equal to "Busy" since "Busy-internal" is not used */
  if (Ea_DetChkIdle() == E_NOT_OK)
  {
    errorId = EA_E_BUSY;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/
    Ea_SetUserParameter(BlockNumber, 0x0000U, DataBufferPtr, EaBh_GetDataLength(BlockNumber), EA_WRITE_JOB); /* SBSW_EA_03 */
    Ea_SetJobParameter();

    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #60 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_WRITE, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
/* Ea_Write */

/**********************************************************************************************************************
 *  Ea_InvalidateBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_InvalidateBlock(uint16 BlockNumber)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else
  /* ----- INVALID BLOCK NO CHECK ----------------------------------------------------------------------*/
  /* #20 Perform Invalid Block Number check */
  if (Ea_DetChkBlockNumberOk(BlockNumber, EA_SID_INVALIDATE_BLOCK) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_NO;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */

  /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
  /* #30 Perform Busy check. At this point "Not-Idle" is equal to "Busy" since "Busy-internal" is not used */
  if (Ea_DetChkIdle() == E_NOT_OK)
  {
    errorId = EA_E_BUSY;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/
    Ea_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, EA_INVALIDATE_JOB); /* SBSW_EA_04 */
    Ea_SetJobParameter();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #40 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_INVALIDATE_BLOCK, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}
/* Ea_InvalidateBlock */

/**********************************************************************************************************************
 *  Ea_EraseImmediateBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_EraseImmediateBlock(uint16 BlockNumber)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else
  /* ----- INVALID BLOCK NO CHECK ----------------------------------------------------------------------*/
  /* #30 Perform Invalid Block Number check */
  if (Ea_DetChkBlockNumberOk(BlockNumber, EA_SID_ERASE_IMMEDIATE_BLOCK) == E_NOT_OK)
  {
    errorId = EA_E_INVALID_BLOCK_NO;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */

  /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
  /* #40 Perform Busy check. At this point "Not-Idle" is equal to "Busy" since "Busy-internal" is not used */
  if (Ea_DetChkIdle() == E_NOT_OK)
  {
    errorId = EA_E_BUSY;
  }
  else
  {
  /* ----- Implementation ------------------------------------------------------------------------------*/
    Ea_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, EA_ERASE_JOB); /* SBSW_EA_04 */
    Ea_SetJobParameter();

    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #50 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_ERASE_IMMEDIATE_BLOCK, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}
/* Ea_EraseImmediateBlock */

/**********************************************************************************************************************
 *  Ea_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_Cancel(void)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */

  /* ----- IDLE CHECK --------------------------------------------------------------------------------*/
  /* #20 Perform invalid cancel check */
  if (Ea_DetChkIdle() == E_OK)
  {
    errorId = EA_E_INVALID_CANCEL;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/
    Ea_SetFlagCancelRequested(TRUE);
    Ea_SetFlagJobRequested(FALSE);

    Ea_Eep_Cancel();

    Ea_ComponentParameter.Ea_JobResult = MEMIF_JOB_CANCELLED;
    Ea_ComponentParameter.Ea_Status = MEMIF_IDLE;
  }

  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #30 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_CANCEL, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
}
/* Ea_Cancel */

/**********************************************************************************************************************
 *  Ea_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) Ea_GetStatus(void)
{
  return Ea_ComponentParameter.Ea_Status;
}
/* Ea_GetStatus */

/**********************************************************************************************************************
 *  Ea_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_GetJobResult(void)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  MemIf_JobResultType retVal;
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
    retVal = MEMIF_JOB_FAILED;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/

    retVal = Ea_ComponentParameter.Ea_JobResult;

  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #20 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_GET_JOB_RESULT, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}
/* Ea_GetJobResult */

#if (EA_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Ea_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
# if (EA_DEV_ERROR_DETECT == STD_ON) /* COV_EA_DET_NEVER_ON */

  /* ----- VERSION POINTER CHECK ------------------------------------------------------------------------*/
  /* #10 Perform Version Pointer check */
  if (Ea_DetChkDataPointerOk((void *)VersionInfoPtr) == E_NOT_OK) /* SBSW_EA_01 */
  {
    errorId = EA_E_INVALID_DATA_POINTER;
  }
  else

# endif /* EA_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ------------------------------------------------------------------------------*/

    VersionInfoPtr->vendorID = EA_VENDOR_ID; /* SBSW_EA_01 */
    VersionInfoPtr->moduleID = EA_MODULE_ID; /* SBSW_EA_01 */

    VersionInfoPtr->sw_major_version = EA_SW_MAJOR_VERSION; /* SBSW_EA_01 */
    VersionInfoPtr->sw_minor_version = EA_SW_MINOR_VERSION; /* SBSW_EA_01 */
    VersionInfoPtr->sw_patch_version = EA_SW_PATCH_VERSION; /* SBSW_EA_01 */

  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #20 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON) /* COV_EA_DET_NEVER_ON */
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_GET_VERSION_INFO, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
}
/* Ea_GetVersionInfo */
#endif /* EA_VERSION_INFO_API */

#if (EA_SETMODE_API == STD_ON) /* COV_EA_SETMODE_ALWAYS_ON */
/**********************************************************************************************************************
 *  Ea_SetMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_SetMode(MemIf_ModeType Mode) /* PRQA S 3206 */ /* MD_MSR_14.2 */
{
  /* #10 Implementation intentionally left empty - See design decisions */
  EA_DUMMY_STATEMENT(Mode); /* PRQA S 3112 */ /* MD_MSR_14.2 */
}
#endif /* EA_SETMODE_API */

#if (EA_EEP_POLLING_MODE == STD_OFF)
/**********************************************************************************************************************
 *  Ea_JobEndNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_JobEndNotification(void)
{
  Ea_Eep_SetJobResult(MEMIF_JOB_OK);
}
/* Ea_JobEndNotification */

/**********************************************************************************************************************
 *  Ea_JobErrorNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_JobErrorNotification(void)
{
  Ea_Eep_SetJobResult(MEMIF_JOB_FAILED);
}
/* Ea_JobErrorNotification */
#endif

/**********************************************************************************************************************
 *  Ea_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) Ea_MainFunction (void)
{
  /* ----- Local Variables -----------------------------------------------------------------------------*/
  uint8 errorId = EA_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EA_DEV_ERROR_DETECT == STD_ON)

  /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
  /* #10 Perform Uninitialized check */
  if (Ea_DetChkInit() == E_NOT_OK)
  {
    errorId = EA_E_UNINIT;
  }
  else

#endif /* EA_DEV_ERROR_DETECT */
  {
  /* ----- Implementation ------------------------------------------------------------------------------*/
    /* #20 MainFunction is only processed if it's busy */
    if(Ea_InternalFlags.MainFunctionBusy == TRUE)
    {
        Ea_ProcessStateMachine();
    }
  }
  /* ----- Development Error Report --------------------------------------------------------------------*/
  /* #30 Report development error if any occurred */
#if (EA_DEV_ERROR_REPORT == STD_ON)
  if (errorId != EA_E_NO_ERROR)
  {
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, EA_SID_MAINFUNCTION, errorId);
  }
#else
  EA_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

}
/* Ea_MainFunction */

/**********************************************************************************************************************
  Ea_AlignValue()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) Ea_AlignValue(uint16 Value, uint16 Alignment)
{
  uint16 retVal;

  /* #10 Aligns Value to Alignment */
  if ((Value % Alignment) == 0)
  {
    retVal = Value;
  }
  else
  {
    /* #20 If Value is not aligned, next greater value is returned which is aligned to Alignment */
    retVal = ((Value / Alignment) + 1u) * Alignment;
  }

  return retVal;
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

 MD_EA_15.3: rule 15.3
      Reason:     No default case is included since the type of the switch parameter is an enumeration and all
                  possible enumeration values are covered.
      Risk:       No risk. JobType is always set with valid enumeration member.
      Prevention: Program flow has been verified by component tests and review.

 MD_EA_8.7: rule 8.7
      Reason:     File scope static is reasonable since parameter is used in more than one function if
                  callback services are used (EEP Polling mode disabled).
      Risk:       No risk.
      Prevention: Program flow has been verified by component tests and review.

*/

/* START_COVERAGE_JUSTIFICATION
    \ID COV_EA_NO_CODE_INSTRUMENTATION
    \ACCEPT TF tf tx
    \REASON Condition "Ea_InternalFlags.CancelRequested == FALSE" is never false in TscStandard.
            Condition can only be true in case Ea_Cancel() service is called while Layer1 sub-component was executed
            for the LAST time. It is not possible to force this cancel without instrumenting the source code.

    \ID COV_EA_DET_NEVER_ON
    \ACCEPT TX
    \REASON Due to low complexity there is no separate configuration with GetVersionInfo API enabled and DET
            (Detection and Reporting) disabled.

    \ID COV_EA_SETMODE_ALWAYS_ON
    \ACCEPT TX
    \REASON SetMode API is always enabled because upper layer (NvM), who is calling EA's SetMode API would not
            recognize if SetMode API is disabled.
            If NvM calls SetMode while it's disabled, a compilation/linker error occurs.

    \ID COV_EA_INTERNAL_SERVICE
    \ACCEPT TX
    \REASON Function Ea_AlignValue shall only be accessed by module internal sub-components.
            This condition is never false, since only EA and its sub-components are considered within
            variant coverage analysis.

    \ID COV_EA_MISRA
    \ACCEPT XX
    \REASON [MSR_COV_MISRA]

    \ID COV_EA_COMPATIBILITY
    \ACCEPT XF
    \ACCEPT TX
    \REASON [COV_MSR_COMPATIBILITY]
END_COVERAGE_JUSTIFICATION */

/* SBSW_JUSTIFICATION_BEGIN
   \ID SBSW_EA_01
    \DESCRIPTION The function Ea_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr.
                 Additionally, VersionInfoPtr is checked if it's valid.
    \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameters VersionInfoPtr is valid.

   \ID SBSW_EA_02
    \DESCRIPTION Ea_UserJobParameter structure contains pointer to data buffer (DataBufferPtr), which was passed by
                 caller of EA component.
    \COUNTERMEASURE \N The caller of EA ensures that the pointers passed to public APIs (e.g. Read, Write) are valid.
                       DataBufferPtr which is contained in Ea_UserJobParameter structure is always initialized in an
                       asynchronous API service before being used in Ea_MainFunction. Ea_MainFunction is only processed
                       if any asynchronous job is pending.

   \ID SBSW_EA_03
    \DESCRIPTION DataBufferPtr, which points to data array of caller, is passed to local function.
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the public APIs is valid.

   \ID SBSW_EA_04
    \DESCRIPTION NULL_PTR is passed to local function Ea_SetUserParameter in Ea_InvalidateBlock() and
                 Ea_EraseImmediateBlock() API.
    \COUNTERMEASURE \N Local structure Ea_UserJobParameter is initialized with values passed to
                       function Ea_SetUserParameter. Structure member DataBufferPtr is initialized with NULL_PTR
                       in case Ea_InvalidateBlock() or Ea_EraseImmediateBlock() API is called.
                       Though DataBufferPtr is not used if an invalidate or erase job is requested.

   \ID SBSW_EA_05
    \DESCRIPTION Callback function of NvM component is called via function pointer
    \COUNTERMEASURE \S Verify that valid notification routines are provided for 'EaNvmJobEndNotification' and
                       'EaNvmJobErrorNotification'. SMI-93
SBSW_JUSTIFICATION_END */
