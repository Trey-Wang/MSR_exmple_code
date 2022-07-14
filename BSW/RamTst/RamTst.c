/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  RamTst.c
 *        \brief  RAMTST source file
 *
 *      \details  This is the source code file of the module RamTst.
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

#define RAMTST_SOURCE


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Std_Types.h"
#include "RamTst.h"

#define RAMTST_PRIVATE_CFG_INCLUDE
#include "RamTst_PrivateCfg.h"
#undef RAMTST_PRIVATE_CFG_INCLUDE /* PRQA S 841 */ /* MD_MSR_19.6 */
#if (defined (V_RAMTST_TEST_CALLOUTS))
#include "TsxRamTst_Callouts.h"
#endif



/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* vendor specific version information is BCD coded */
#if (  (RAMTST_SW_MAJOR_VERSION != (2u)) \
    || (RAMTST_SW_MINOR_VERSION != (2u)) )
#error "Vendor specific version numbers of RamTst.c and RamTst.h are inconsistent"
#endif

/* versions of all included header files shall be checked, configuration is just an example */
#if (  (RAMTST_CFG_MAJOR_VERSION != (4u)) \
    || (RAMTST_CFG_MINOR_VERSION != (0u)) )
#error "Version numbers of RamTst.c and RamTst_Cfg.h are inconsistent!"
#endif



/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define RAMTST_MAX_NUMBER_OF_BLOCKS                      (10u)
/* Zero pattern for the algorithms */
#define RAMTST_ZERO_PATTERN                              (0x00000000u)
/* Ones pattern for the algorithms */
#define RAMTST_ONES_PATTERN                              (0xFFFFFFFFu)
#define RAMTST_MIN_NUM_OF_TESTED_CELLS                   (RamTst_Config_t.RamTst_MinNumberOfTestedCells_t)
#define RAMTST_ONE                                       (0x00000001u)
#define RAMTST_SET_A_CELL(AddValue, Index)               (uint32)( (RAMTST_ONE << (Index)) |(AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define RAMTST_RESET_A_CELL(AddValue, Index)             (uint32)( (~( RAMTST_ONE << (Index) )) & (AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Definition for the Walkpath Test */
#define RAMTST_INSPECT_WALKPATH(AddValue, Index)         (uint32)( (RAMTST_ONE << (Index))^(AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Definition for the transparent Galpat Test */
#define RAMTST_SIGNATURE_POLY                            (0x55555555u)
#define RAMTST_MASK_A_CELL(AddValue, Index)              (uint32)( ((RAMTST_ONE << (Index)) & (AddValue)) >> (Index) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Definition for the Galpat Test */
#define RAMTST_GALPAT_PATTERN(AddValue, Index)           (uint32)( (RAMTST_ONE << ((Index) - 1u)) | (AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define RAMTST_GALPAT_ANTI_PATTERN(AddValue, Index)      (uint32)(~(RAMTST_ONE << (RAMTST_MIN_NUM_OF_TESTED_CELLS - (Index))) & (AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define RAMTST_GALPAT_INSPECT_ONES(AddValue, Index)      (uint32)( (RAMTST_ONES_PATTERN << (Index))  | (AddValue) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define RAMTST_GALPAT_INSPECT_ZERO(AddValue, Index)      (uint32)( (RAMTST_ONES_PATTERN >> (RAMTST_MIN_NUM_OF_TESTED_CELLS - (Index))) ^ (AddValue)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define RAMTST_GALPAT_ANTI_INSPECT_ZERO(AddValue, Index) (uint32)(~(RAMTST_ONES_PATTERN << (RAMTST_MIN_NUM_OF_TESTED_CELLS - (Index))) ^ (AddValue)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define RAMTST_GALPAT_ANTI_INSPECT_ONES(AddValue, Index) (uint32)( (RAMTST_ONES_PATTERN << (RAMTST_MIN_NUM_OF_TESTED_CELLS - (Index))) | (AddValue)) /* PRQA S 3453 */ /* MD_MSR_19.7 */



/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/* ===== UINT8 VARIABLES =================================================== */

/* ----- VAR_NOINIT_8BIT ----- */
#define RAMTST_START_SEC_VAR_NO_INIT_8
#include "MemMap.h"

/* The BackgroundLoopCounter is for the background test and identifies the current
 * tested block of the background test. */
static VAR(uint8, RAMTST_VAR) RamTst_BackgroundLoopCounter_u8;

#define RAMTST_STOP_SEC_VAR_NO_INIT_8
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */


/* ===== UINT32 VARIABLES ================================================== */

/* ----- VAR_NOINIT_32BIT ----- */
#define RAMTST_START_SEC_VAR_NO_INIT_32
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

/* This pointer points to the next start address of a background test. */
static P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_SavedAdd_pu32; /* PRQA S 3218 */ /* MD_RamTst_3218 */

#define RAMTST_STOP_SEC_VAR_NO_INIT_32
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */


/* ===== UNSPECIFIED VARIABLES ============================================= */

/* ----- VAR_INIT_UNSPECIFIED ----- */
#define RAMTST_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

VAR(RamTst_ExecutionStatusType, RAMTST_VAR) RamTst_CurrentExecutionStatus_t = /* PRQA S 3408 */ /* MD_RamTst_3408 */
        RAMTST_EXECUTION_UNINIT;

#define RAMTST_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

/* ----- VAR_NOINIT_UNSPECIFIED ----- */
#define RAMTST_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

VAR(RamTst_AlgParamsIdType, RAMTST_VAR) RamTst_AlgorithmParameterId_t; /* PRQA S 3408 */ /* MD_RamTst_3408 */

VAR(RamTst_TestResultType, RAMTST_VAR) RamTst_OverallTestResult_t; /* PRQA S 3408 */ /* MD_RamTst_3408 */

VAR(RamTst_NumberOfTestedCellsType, RAMTST_VAR) RamTst_NoOfTestedCells_t; /* PRQA S 3408 */ /* MD_RamTst_3408 */

VAR(RamTst_TestResultType, RAMTST_VAR) RamTst_PerBlockTestResult_at[RAMTST_MAX_NUMBER_OF_BLOCKS]; /* PRQA S 3408 */ /* MD_RamTst_3408 */

static VAR(RamTst_NumberOfBlocksType, RAMTST_VAR) RamTst_NoOfBlocks_t;

#define RAMTST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */



/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define RAMTST_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

#if (STD_ON == RAMTST_CHECKERBOARD_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_CheckerboardTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

#if (STD_ON == RAMTST_MARCH_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_MarchTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

#if (STD_ON == RAMTST_WALK_PATH_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_WalkPathTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

#if (STD_ON == RAMTST_GALPAT_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GalpatTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

#if (STD_ON == RAMTST_TRANSP_GALPAT_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_TransGalpatTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

#if (STD_ON == RAMTST_ABRAHAM_TEST_SELECTED)
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_AbrahamTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);
#endif

static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_Dummy(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);

static FUNC(void, RAMTST_CODE) RamTst_CheckTestResult(void);

#define RAMTST_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

#define RAMTST_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

static CONST(RamTst_AlgFunc_p, RAMTST_CONST) RamTst_AlgorithmFuncTable_at[] = {
  RamTst_Dummy,
#if (STD_ON == RAMTST_ABRAHAM_TEST_SELECTED)
  RamTst_AbrahamTest,
#endif
#if (STD_ON == RAMTST_CHECKERBOARD_TEST_SELECTED)
  RamTst_CheckerboardTest,
#endif
#if (STD_ON == RAMTST_GALPAT_TEST_SELECTED)
  RamTst_GalpatTest,
#endif
#if (STD_ON == RAMTST_MARCH_TEST_SELECTED)
  RamTst_MarchTest,
#endif
#if (STD_ON == RAMTST_TRANSP_GALPAT_TEST_SELECTED)
  RamTst_TransGalpatTest,
#endif
#if (STD_ON == RAMTST_WALK_PATH_TEST_SELECTED)
  RamTst_WalkPathTest,
#endif
};

#define RAMTST_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#define RAMTST_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

/**********************************************************************************************************************
 *  RamTst_InitMemory()
 *********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function can be used to initialize the
 *              variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre        Module must not be initialized
 *  \pre        Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_UNINIT;

} /* RamTst_InitMemory() */



#if (RAMTST_GETVERSIONINFO_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief      Initializes all global variables
 *  \details    RamTst_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out] versioninfo             Pointer to variable of type 'Std_VersionInfoType'
 *  \pre        Input parameter must not be NULL.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RAMTST_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  if(NULL_PTR == versioninfo)
  {
    /* NULL_PTR given - call the DET */
    RamTst_DetReportError(RAMTST_SID_GET_VERSION_INFO, RAMTST_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* Valid pointer given */
  }
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID = RAMTST_VENDOR_ID;
  versioninfo->moduleID = RAMTST_MODULE_ID;
  versioninfo->sw_major_version = RAMTST_SW_MAJOR_VERSION;
  versioninfo->sw_minor_version = RAMTST_SW_MINOR_VERSION;
  versioninfo->sw_patch_version = RAMTST_SW_PATCH_VERSION;

  return;
} /* RamTst_GetVersionInfo() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (RAMTST_GETVERSIONINFO_API == STD_ON) */



/**********************************************************************************************************************
 *  RamTst_Init()
 *********************************************************************************************************************/
/*! \brief      Initializes the module.
 *  \details    RamTst_Init() initializes all RAM Test relevant global variables and changes the execution status to
 *              RAMTST_EXECUTION_STOPPED. After initialization, the test uses the default test configuration
 *              (RamTstDefaultAlgParamsId).
 *  \return     none
 *  \pre        Module has to be uninitialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Init(void)
{
  /* ----- Function local variables ------------------------------------- */
  uint8 BlockIndex;

  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_INIT, RAMTST_E_ALREADY_INITIALIZED);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_UNINIT */
  }
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the module state to RAMTST_EXECUTION_STOPPED */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_STOPPED;

  /* Set the default algorithm parameter */
  RamTst_AlgorithmParameterId_t = (RamTst_Config_t.RamTst_DefaultAlgId_u8);

  /* Set the default number of tested cells */
  RamTst_NoOfTestedCells_t
  = RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t]. RamTst_NumberOfTestedCells_t;

  RamTst_NoOfBlocks_t = RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfBlocks_t;

  /* Set the overall test result to RAMTST_RESULT_NOT_TESTED */
  RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_TESTED;

  /* Set the the test result for the configured blocks to RAMTST_RESULT_NOT_TESTED */
  for(BlockIndex = 0; BlockIndex < RamTst_NoOfBlocks_t; BlockIndex++)
  {
    RamTst_PerBlockTestResult_at[BlockIndex] = RAMTST_RESULT_NOT_TESTED;
  }

  /* The background loop counter is set to 0. */
  RamTst_BackgroundLoopCounter_u8 = 0u;

  return;
} /* RamTst_Init() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */



/**********************************************************************************************************************
 *  RamTst_DeInit()
 *********************************************************************************************************************/
/*! \brief      Deinitializes the module.
 *  \details    RamTst_DeInit() deinitializes the RAM Test module, i.e. it sets the module state to uninitialized and
 *              resets all relevant global registers.
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Deinit(void)
{
  /* ----- Function local variables ------------------------------------- */
  uint8 BlockIndex;

  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_DEINIT, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the module state to RAMTST_EXECUTION_UNINIT */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_UNINIT;

  /* Reset all the relevant global variables */
  RamTst_AlgorithmParameterId_t = 0u;
  RamTst_NoOfTestedCells_t = 0u;
  RamTst_NoOfBlocks_t = 0u;
  RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_TESTED;
  for(BlockIndex = 0u; BlockIndex < RAMTST_MAX_NUMBER_OF_BLOCKS; BlockIndex++)
  {
    RamTst_PerBlockTestResult_at[BlockIndex] = RAMTST_RESULT_NOT_TESTED;
  }

  return;
} /* RamTst_Deinit() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */



#if (RAMTST_STOP_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_Stop()
 *********************************************************************************************************************/
/*! \brief      Stops the currently running background test.
 *  \details    RamTst_Stop() stops the background test after the current atomic sequence and sets the test state to
 *              RAMTST_EXECUTION_STOPPED.
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        The current execution state must not be RAMTST_EXECUTION_STOPPED.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Stop(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_STOP, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_STOPPED? */
  if(RAMTST_EXECUTION_STOPPED == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution status is RAMTST_EXECUTION_STOPPED */
    RamTst_DetReportError(RAMTST_SID_STOP, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_STOPPED */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the module state to RAMTST_EXECUTION_STOPPED */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_STOPPED;

  /* Set the global loop counter for the background test to 0u. */
  RamTst_BackgroundLoopCounter_u8 = (0u);

  return;
} /* RamTst_Stop() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_STOP_API == STD_ON) */



#if (RAMTST_ALLOW_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_Allow()
 *********************************************************************************************************************/
/*! \brief      Enables background testing.
 *  \details    RamTst_Allow() permits the main function to perform background testing of the currently active test
 *              configuration.
 *  \return     none
 *  \pre        The current execution status has to be RAMTST_EXECUTION_STOPPED.
 *  \pre        The module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Allow(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_ALLOW, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_STOPPED? */
  if(RAMTST_EXECUTION_STOPPED != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_STOPPED? */
    RamTst_DetReportError(RAMTST_SID_ALLOW, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_STOPPED */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the appropriate execution state to start background testing */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_ALLOWED;

  return;
} /* RamTst_Allow() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_ALLOW_API == STD_ON) */



#if (RAMTST_SUSPEND_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_Suspend()
 *********************************************************************************************************************/
/*! \brief      Suspends the currently running background test.
 *  \details    RamTst_Suspend() temporarily stops the background test, i.e. the main function is no more able to
 *              continue with the background test at it's next scheduled call. The execution state is set to
 *              RAMTST_EXECUTION_SUSPENDED.
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        The current execution state has to be RAMTST_EXECUTION_RUNNING.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Suspend(void)
{
  /* ----- Development Error Checks ------------------------------------- */

  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)

  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_SUSPEND, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_RUNNING? */
  if(RAMTST_EXECUTION_RUNNING != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_RUNNING? */
    RamTst_DetReportError(RAMTST_SID_SUSPEND, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_RUNNING */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the module state to RAMTST_EXECUTION_SUSPENDED */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_SUSPENDED;

  return;
} /* RamTst_Suspend() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_SUSPEND_API == STD_ON) */



#if (RAMTST_RESUME_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_Resume()
 *********************************************************************************************************************/
/*! \brief      Resumes a suspended test.
 *  \details    RamTst_Resume() permits the main function to continue a currently suspended test. Testing continues
 *              according to the saved test states. The execution state is changed to RAMTST_EXECUTION_RUNNING.
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        The execution state has to be RAMTST_EXECUTION_SUSPENDED.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_Resume(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_RESUME, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_SUSPENDED? */
  if(RAMTST_EXECUTION_SUSPENDED != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_SUSPENDED? */
    RamTst_DetReportError(RAMTST_SID_RESUME, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_SUSPENDED */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the module state to RAMTST_EXECUTION_RUNNING */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_RUNNING;

  return;
} /* RamTst_Resume() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_RESUME_API == STD_ON) */



#if (RAMTST_GETEXECUTIONSTATUS_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetExecutionStatus()
 *********************************************************************************************************************/
/*! \brief      Returns the current execution status
 *  \details    RamTst_GetExecutionStatus() returns the current RAM Test execution status.
 *  \return     Execution status
 *  \pre        The module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_ExecutionStatusType, RAMTST_CODE) RamTst_GetExecutionStatus(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_GET_EXECUTION_STATUS, RAMTST_E_UNINIT);
    return(RamTst_CurrentExecutionStatus_t);
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return current execution status */
  return(RamTst_CurrentExecutionStatus_t);
} /* RamTst_GetExecutionStatus() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETEXECUTIONSTATUS_API == STD_ON) */



#if (RAMTST_GETTESTRESULT_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetTestResult()
 *********************************************************************************************************************/
/*! \brief      Returns the current test result
 *  \details    RamTst_GetTestResult() returns the current overall test result.
 *  \return     Overall test result
 *  \pre        The module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GetTestResult(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_GET_TEST_RESULT, RAMTST_E_UNINIT);
    return(RAMTST_RESULT_UNDEFINED);
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return the overall test result */
  return(RamTst_OverallTestResult_t);
} /* RamTst_GetTestResult() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETTESTRESULT_API == STD_ON) */



#if (RAMTST_GETTESTRESULTPERBLOCK_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetTestResultPerBlock()
 *********************************************************************************************************************/
/*! \brief      Returns the current test result of a block
 *  \details    RamTst_GetTestResultPerBlock() returns the current test result of a certain block.
 *  \param[in]  BlockID                 The block ID of the desired block(symbolic name or numeric ID)
 *  \return     Block test result
 *  \pre        The module has to be initialized.
 *  \pre        A valid block ID has to be given.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_GET_TEST_RESULT_PER_BLOCK, RAMTST_E_UNINIT);
    return(RAMTST_RESULT_UNDEFINED);
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  } /* End of execution status checking */

  /* Is the BlockId out of range? */
  if(RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfBlocks_t <= BlockID)
  {
    /* The Block Id is out of range. */
    RamTst_DetReportError(RAMTST_SID_GET_TEST_RESULT_PER_BLOCK, RAMTST_E_OUT_OF_RANGE);
    return(RAMTST_RESULT_UNDEFINED);
  }
  else
  {
    /* The Block Id is not out of range. */
  } /* End of out of range checking. */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */


  /* ----- Implementation ----------------------------------------------- */
  /* Return the test result for the block with assigned block id */
  return(RamTst_PerBlockTestResult_at[BlockID]);
} /* RamTst_GetTestResultPerBlock() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETTESTRESULTPERBLOCK_API == STD_ON) */



#if (RAMTST_GETALGPARAMS_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetAlgParams()
 *********************************************************************************************************************/
/*! \brief      Returns the ID of the test configuration (algorithm parameter set)
 *  \details    RamTst:GetAlgParams() returns the numeric ID of the currently selected test configuration/algorithm
 *              parameter set.
 *  \return     Algorithm Parameter Set ID
 *  \pre        Module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_AlgParamsIdType, RAMTST_CODE) RamTst_GetAlgParams(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)

  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_GET_ALG_PARAMS, RAMTST_E_UNINIT);
    return(0u);
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */


  /* ----- Implementation ----------------------------------------------- */
  /* Return the current selected AlgSet ID */
  return (RamTst_AlgorithmParameterId_t);
} /* RamTst_GetAlgParams() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETALGPARAMS_API == STD_ON) */



#if (RAMTST_GETTESTALGORITHM_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetTestAlgorithm()
 *********************************************************************************************************************/
/*! \brief      Returns the ID of the test algorithm
 *  \details    RamTst_GetTestAlgorithm() returns the numeric identifier of the current test algorithm.
 *  \return     Test algorithm ID
 *  \pre        The module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_AlgorithmType, RAMTST_CODE) RamTst_GetTestAlgorithm(void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
/* Is the current execution status RAMTST_EXECUTION_UNINIT? */
if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
{
  /* The current execution is RAMTST_EXECUTION_UNINIT? */
  RamTst_DetReportError(RAMTST_SID_GET_TEST_ALGORITHM, RAMTST_E_UNINIT);
  return(RAMTST_ALGORITHM_UNDEFINED);
}
else
{
  /* The current execution status is not RAMTST_EXECUTION_UNINIT */
} /* End of execution status checking */
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return the currently activated algorithm */
  return(RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_Algorithm_t);
} /* RamTst_GetTestAlgorithm() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETTESTALGORITHM_API == STD_ON) */



#if (RAMTST_GETNUMBEROFTESTEDCELLS_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_GetNumberOfTestedCells()
 *********************************************************************************************************************/
/*! \brief      Returns number of tested cells
 *  \details    RamTst_GetNumberOfTestedCells() returns the currently set number of tested cells per main function
 *              call.
 *  \return     Number of tested cells
 *  \pre        Module has to be initialized.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(RamTst_NumberOfTestedCellsType, RAMTST_CODE) RamTst_GetNumberOfTestedCells(void)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_GET_NUMBER_OF_TESTED_CELLS, RAMTST_E_UNINIT);

    return(0x00U);
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return the current cells to test */
  return(RamTst_NoOfTestedCells_t);
} /* RamTst_GetNumberOfTestedCells() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_GETNUMBEROFTESTEDCELLS_API == STD_ON) */



#if (RAMTST_SELECTALGPARAMS_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_SelectAlgParams()
 *********************************************************************************************************************/
/*! \brief      Selects the algorithm parameter set
 *  \details    RamTst_SelectAlgParams()
 *  \param[in]  NewAlgParamsId          ID of the Algorithm parameter set to switch to
 *  \return     none
 *  \pre        Module has to be initialized.
 *  \pre        Module execution status has to be RAMTST_EXECUTION_STOPPED.
 *  \pre        A valid algorithm parameter set ID has to be given.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId)
{
  /* ----- Function local variables ------------------------------------- */
  uint8 LoopCount;

  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_SELECT_ALG_PARAMS, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_STOPPED? */
  if(RAMTST_EXECUTION_STOPPED != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_STOPPED? */
    RamTst_DetReportError(RAMTST_SID_SELECT_ALG_PARAMS, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_STOPPED */
  } /* End of execution status checking */

  /* Is the the NewAlgParamsId out of range? */
  if(NewAlgParamsId > (RamTst_AlgParamsIdType) ((RamTst_Config_t.RamTst_NumberOfAlgParams_u8) - 0x01u))
  {
    /* The current execution is not RAMTST_EXECUTION_STOPPED? */
    RamTst_DetReportError(RAMTST_SID_SELECT_ALG_PARAMS, RAMTST_E_OUT_OF_RANGE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_STOPPED */
  } /* End of out of range checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Select the NewAlgParamsId. */
  RamTst_AlgorithmParameterId_t = (NewAlgParamsId);

  /* Save the configured NumberOfTestedCells. */
  RamTst_NoOfTestedCells_t = 
                       RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfTestedCells_t;

  /* Set the overall test result. */
  RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_TESTED;

  RamTst_NoOfBlocks_t = RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfBlocks_t;

  /* Set the the test result for the configured blocks to RAMTST_RESULT_NOT_TESTED */
  for(LoopCount = 0; LoopCount < RamTst_NoOfBlocks_t; LoopCount++)
  {
    RamTst_PerBlockTestResult_at[LoopCount] = RAMTST_RESULT_NOT_TESTED;
  }

  return;
} /* RamTst_SelectAlgParams() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_SELECTALGPARAMS_API == STD_ON) */



#if (RAMTST_CHANGENUMBEROFTESTEDCELLS_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_ChangeNumberOfTestedCells()
 *********************************************************************************************************************/
/*! \brief      Changes the number of tested cells
 *  \details    RamTst_ChangeNumberOfTestedCells() changes the number of tested cells for background testing.
 *  \param[in]  NewNumberOfTestedCells  The desired number of tested cells
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        Module execution status has to be RAMTST_EXECUTION_STOPPED.
 *  \pre        A valid number of tested cells has to be given.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells)
{
  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_CHANGE_NUMBER_OF_TESTED_CELLS, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_STOPPED? */
  if(RAMTST_EXECUTION_STOPPED != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_STOPPED? */
    RamTst_DetReportError(RAMTST_SID_CHANGE_NUMBER_OF_TESTED_CELLS, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_STOPPED */
  } /* End of execution status checking */

  /* Is the NewNumberOfTestedCells out of range?
  * */
  if( (RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_MaxNumberOfTestedCells_t <
                                                                                                NewNumberOfTestedCells)
     || ((RamTst_NumberOfTestedCellsType) RAMTST_MIN_NUM_OF_TESTED_CELLS > NewNumberOfTestedCells)
     || ((RamTst_NumberOfTestedCellsType)NewNumberOfTestedCells %  /* PRQA S 4109 */ /* MD_RamTst_4109 */
                                                      (RamTst_NumberOfTestedCellsType)RAMTST_MIN_NUM_OF_TESTED_CELLS) )
  {
    /* The NewNumberOfTestedCells is out of range */
    RamTst_DetReportError(RAMTST_SID_CHANGE_NUMBER_OF_TESTED_CELLS, RAMTST_E_OUT_OF_RANGE);
    return;
  }
  else
  {
    /* The NewNumberOfTestedCells is out of range */
  } /* End of out of range checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */


  /* ----- Implementation ----------------------------------------------- */
  /* Change the NumberOfTestedCells. */
  RamTst_NoOfTestedCells_t = NewNumberOfTestedCells;
  return;
} /* RamTst_ChangeNumberOfTestedCells() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_CHANGENUMBEROFTESTEDCELLS_API == STD_ON) */



#if (RAMTST_RUNFULLTEST_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_RunFullTest()
 *********************************************************************************************************************/
/*! \brief      Executes the foreground test
 *  \details    RamTst_RunFullTest() starts the foreground test for the selected algorithm parameter set.
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        Module execution status must not be RAMTST_EXECUTION_STOPPED.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_RunFullTest(void)
{
  /* ----- Function local variables ------------------------------------- */
  uint8 LoopCount;
  P2CONST(RamTst_BlockParamsConfigType, AUTOMATIC, RAMTST_CONST) CurrentBlock_pt;

  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
  /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
  if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is RAMTST_EXECUTION_UNINIT? */
    RamTst_DetReportError(RAMTST_SID_RUN_FULL_TEST, RAMTST_E_UNINIT);
    return;
  }
  else
  {
    /* The current execution status is not RAMTST_EXECUTION_UNINIT */
  }

  /* Is the current execution status RAMTST_EXECUTION_STOPPED? */
  if(RAMTST_EXECUTION_STOPPED != RamTst_CurrentExecutionStatus_t)
  {
    /* The current execution is not RAMTST_EXECUTION_STOPPED? */
    RamTst_DetReportError(RAMTST_SID_RUN_FULL_TEST, RAMTST_E_STATUS_FAILURE);
    return;
  }
  else
  {
    /* The current execution status is RAMTST_EXECUTION_STOPPED */
  } /* End of execution status checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* ===== Critical Section Start ===== */
  RamTst_EnterCritical();

  /* Toggle the status to RAMTST_EXECUTION_RUNNING. */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_RUNNING;

  for(LoopCount = 0; LoopCount
          < RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfBlocks_t; LoopCount++)
  {
    CurrentBlock_pt = &RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[LoopCount];

    RamTst_PerBlockTestResult_at[LoopCount]
    = RamTst_AlgorithmFuncTable_at[RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_Algorithm_t](
                CurrentBlock_pt->RamTst_StartAddress_pu32,
                CurrentBlock_pt->RamTst_StopAddress_pu32,
                CurrentBlock_pt->RamTst_FillPattern_u32,
                CurrentBlock_pt->RamTst_TestPolicy_u8);

    /* Is the test result for the block with the id LoopCount OK? */
    if(RAMTST_RESULT_OK != RamTst_PerBlockTestResult_at[LoopCount])
    {
      /* The result is not OK. Set the overall test result to
       * RAMTST_RESULT_NOT_OK. */
      RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_OK;
      RamTst_DemReportError(RAMTST_E_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
    }
    else
    {
      if(RAMTST_RESULT_NOT_OK != RamTst_OverallTestResult_t)
      {
        RamTst_OverallTestResult_t = RAMTST_RESULT_OK;
      }
    }
  }

  /* Toggle the current status to RAMTST_EXECUTION_STOPPED. */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_STOPPED;

  /* ===== Critical Section End ===== */
  RamTst_ExitCritical();

  return;

} /* RamTst_RunFullTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_RUNFULLTEST_API == STD_ON) */



#if (RAMTST_RUNPARTIALTEST_API == STD_ON)
/**********************************************************************************************************************
 *  RamTst_RunPartialTest()
 *********************************************************************************************************************/
/*! \brief      Executes the foreground test of one RAM block
 *  \details    RamTst_RunPartialTest() executes the foreground test for a given RAM block.
 *  \param[in]  BlockId                 Identifier of the RAM block to test
 *  \return     none
 *  \pre        The module has to be initialized.
 *  \pre        Module execution status must not be RAMTST_EXECUTION_STOPPED or RAMTST_EXECUTION_SUSPENDED.
 *  \pre        A valid block ID has to be given.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId)
{
  /* ----- Function local variables ------------------------------------- */
  RamTst_ExecutionStatusType TempExecutionStatus;
  P2CONST(RamTst_BlockParamsConfigType, AUTOMATIC, RAMTST_CONST) CurrentBlock_pt;

  /* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    /* Is the current execution status RAMTST_EXECUTION_UNINIT? */
    if(RAMTST_EXECUTION_UNINIT == RamTst_CurrentExecutionStatus_t)
    {
      /* The current execution is RAMTST_EXECUTION_UNINIT? */
      RamTst_DetReportError(RAMTST_SID_RUN_PARTIAL_TEST, RAMTST_E_UNINIT);
      return;
    }
    else
    {
      /* The current execution status is not RAMTST_EXECUTION_UNINIT */
    } /* End of execution status checking */

    /* Is the current execution status is not RAMTST_EXECUTION_STOPPED */
    if(RAMTST_EXECUTION_STOPPED != RamTst_CurrentExecutionStatus_t)
    {
      /* The current execution status is not RAMTST_EXECUTION_STOPPED */
      /* Is the current execution status is not RAMTST_EXECUTION_SUSPENDED */
      if(RAMTST_EXECUTION_SUSPENDED != RamTst_CurrentExecutionStatus_t)
      {
        /* The current execution status is not RAMTST_EXECUTION_SUSPENDED */
        RamTst_DetReportError(RAMTST_SID_RUN_PARTIAL_TEST, RAMTST_E_STATUS_FAILURE);
        return;
      }
      else
      {
        /* The current execution status is RAMTST_EXECUTION_SUSPENDED */
      }
    }
    else
    {
      /* The current execution status is RAMTST_EXECUTION_STOPPED */
    } /* End of execution status checking */

    /* Is the BlockId out of range */
    if(RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_NumberOfBlocks_t <= BlockId)
    {
      /* The current execution status is not RAMTST_EXECUTION_SUSPENDED */
      RamTst_DetReportError(RAMTST_SID_RUN_PARTIAL_TEST, RAMTST_E_OUT_OF_RANGE);
      return;
    }
    else
    {
      /* The current execution status is RAMTST_EXECUTION_SUSPENDED */
    } /* End of out of range checking */
  #endif /* (STD_ON == RAMTST_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* ===== Critical Section Start ===== */
  RamTst_EnterCritical();

  /* Save the current execution status. */
  TempExecutionStatus = RamTst_CurrentExecutionStatus_t;
  /* Set the execution status to RAMTST_EXECUTION_RUNNING */
  RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_RUNNING;

  CurrentBlock_pt = 
    &RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[BlockId];

  RamTst_PerBlockTestResult_at[BlockId]
  = RamTst_AlgorithmFuncTable_at[RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_Algorithm_t](
      CurrentBlock_pt->RamTst_StartAddress_pu32,
      CurrentBlock_pt->RamTst_StopAddress_pu32,
      CurrentBlock_pt->RamTst_FillPattern_u32,
      CurrentBlock_pt->RamTst_TestPolicy_u8);

  /* Check the test result for the specific block. */
  if(RAMTST_RESULT_NOT_OK == RamTst_PerBlockTestResult_at[BlockId])
  {
    /* The test result is not OK. */
    RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_OK;
    RamTst_DemReportError(RAMTST_E_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
  }
  else
  {
    /* The test result is OK. */

  }

  /* Toggle the current status back to the status before the call. */
  RamTst_CurrentExecutionStatus_t = TempExecutionStatus;

  /* ===== Critical Section End ===== */
  RamTst_ExitCritical();

  return;
} /* RamTst_RunPartialTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (RAMTST_RUNPARTIALTEST_API == STD_ON) */



/**********************************************************************************************************************
 *  RamTst_MainFunction()
 *********************************************************************************************************************/
/*! \brief      Scheduled function background test execution
 *  \details    RamTst_MainFunction() executes the background RAM test on each scheduled call. If all the configured
 *              RAM blocks have been tested, the service starts testing the first RAM block in the selected algorithm
 *              parameter set. The test completed notification can be used to recognize the end of a complete test
 *              cycle.
 *  \return     none
 *  \pre        Module execution state should be RAMTST_EXECUTION_ALLOWED or RAMTST_EXECUTION_RUNNING, otherwise the
 *              service returns without any action.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, RAMTST_CODE) RamTst_MainFunction(void)
{
  /* ----- Function local variables ------------------------------------- */
  uint8 LoopCount;
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StopAddCurrentCall_pu32;
  P2CONST(RamTst_BlockParamsConfigType, AUTOMATIC, RAMTST_CONST) CurrentBlock_pt;
  RamTst_TestResultType RamTst_HelpTestResult_t;

  /* ----- Implementation ----------------------------------------------- */
  /* ===== Critical Section Start ===== */
  RamTst_EnterCritical();

  CurrentBlock_pt = &RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[RamTst_BackgroundLoopCounter_u8];

  /* Is the execution status running? */
  if(RAMTST_EXECUTION_RUNNING == RamTst_CurrentExecutionStatus_t)
  {
    /* Calculation of the stop address for this MainFunction call. */
    RamTst_StopAddCurrentCall_pu32 = RamTst_SavedAdd_pu32 + ((RamTst_NoOfTestedCells_t / RAMTST_MIN_NUM_OF_TESTED_CELLS) - (1u)); /* PRQA S 488 */ /* MD_RamTst_488 */

    /* Assure that testing stops at the configured stop address */
    if(RamTst_StopAddCurrentCall_pu32 > CurrentBlock_pt->RamTst_StopAddress_pu32)
    {
      RamTst_StopAddCurrentCall_pu32 = CurrentBlock_pt->RamTst_StopAddress_pu32;
    }

    /* Call the test algorithm for the current RamTst_AlgorithmParameterId_t. */
    RamTst_HelpTestResult_t
      = RamTst_AlgorithmFuncTable_at[RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_Algorithm_t](
          RamTst_SavedAdd_pu32,
          RamTst_StopAddCurrentCall_pu32,
          CurrentBlock_pt->RamTst_FillPattern_u32,
          CurrentBlock_pt->RamTst_TestPolicy_u8);
  }
  else
  {
    /* Is the execution status allowed? */
    if(RAMTST_EXECUTION_ALLOWED == RamTst_CurrentExecutionStatus_t)
    {
    RamTst_CurrentExecutionStatus_t = RAMTST_EXECUTION_RUNNING;
    /* Calculation of the stop address for this MainFunction call. */
    RamTst_StopAddCurrentCall_pu32 = CurrentBlock_pt->RamTst_StartAddress_pu32
            + ((RamTst_NoOfTestedCells_t / RAMTST_MIN_NUM_OF_TESTED_CELLS) - (1u)); /* PRQA S 488 */ /* MD_RamTst_488 */

    RamTst_SavedAdd_pu32 = CurrentBlock_pt->RamTst_StartAddress_pu32;

    /* Call the test algorithm for the current RamTst_AlgorithmParameterId_t. */
    RamTst_HelpTestResult_t
      = RamTst_AlgorithmFuncTable_at[RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_Algorithm_t](
          RamTst_SavedAdd_pu32,
          RamTst_StopAddCurrentCall_pu32,
          CurrentBlock_pt->RamTst_FillPattern_u32,
          CurrentBlock_pt->RamTst_TestPolicy_u8);
    }
    else
    {
      /* ===== Critical Section End ===== */
      RamTst_ExitCritical();
      /* The module is not able to do a background test. */
      return;
    }
  }

  for(LoopCount = RamTst_BackgroundLoopCounter_u8;; LoopCount++)  /* ECO_DISABLE_GENERATION */
  {                                                               /* Comment above needed for test purposes */
    /* Are all cells of the current block been tested? */
    if(CurrentBlock_pt->RamTst_StopAddress_pu32 > RamTst_StopAddCurrentCall_pu32)
    {
      /* Not all cells of the current block are tested. */
      /* Is the current test result not OK? */
      if(RAMTST_RESULT_NOT_OK == RamTst_HelpTestResult_t)
      {
        /* The current test result is not OK. */
        RamTst_OverallTestResult_t = RAMTST_RESULT_NOT_OK;
        RamTst_PerBlockTestResult_at[RamTst_BackgroundLoopCounter_u8] = RAMTST_RESULT_NOT_OK;
        /* Test result is RAMTST_RESULT_NOT_OK call the DEM */
        RamTst_DemReportError(RAMTST_E_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
      }
      else
      {
        /* Is the overall test result not OK?
         * If this case is true, the test result must not be overwritten. */
        if( (RAMTST_RESULT_NOT_OK != RamTst_OverallTestResult_t) && (RAMTST_RESULT_OK != RamTst_OverallTestResult_t) )
        {
          /* The test result is either OK, undefined or not tested. */
          RamTst_OverallTestResult_t = RAMTST_RESULT_UNDEFINED;
        }
        else
        {
          /* The test result is not OK and must not be overwritten. */
        }

        if(RAMTST_RESULT_NOT_OK != RamTst_PerBlockTestResult_at[RamTst_BackgroundLoopCounter_u8])
        {
          RamTst_PerBlockTestResult_at[RamTst_BackgroundLoopCounter_u8] = RAMTST_RESULT_UNDEFINED;
        }
      }

      break;
    }

    /* Is the current stop address the stop address of the current block. */
    if(CurrentBlock_pt->RamTst_StopAddress_pu32 <= RamTst_StopAddCurrentCall_pu32)
    {
      /* The current stop address is the stop address of the current block. */
      if(RAMTST_RESULT_NOT_OK != RamTst_PerBlockTestResult_at[LoopCount])
      {
        RamTst_PerBlockTestResult_at[LoopCount] = RamTst_HelpTestResult_t;
      }

      /* Is the OverAllTestResult not OK?
      * This request is to inhibit the overwriting of the "Not_OK"-OverAllTestResult. */
      if( (RAMTST_RESULT_NOT_OK != RamTst_OverallTestResult_t) && (RAMTST_RESULT_NOT_OK == RamTst_HelpTestResult_t) )
      {
        /* Update the OverallTestResult. */
        RamTst_OverallTestResult_t = RamTst_HelpTestResult_t;
      }

      RamTst_BackgroundLoopCounter_u8 += (1u);

      if(RamTst_BackgroundLoopCounter_u8 < RamTst_NoOfBlocks_t)
      {
        CurrentBlock_pt = &RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[RamTst_BackgroundLoopCounter_u8];
        RamTst_SavedAdd_pu32 = CurrentBlock_pt->RamTst_StartAddress_pu32;

        /* ===== Critical Section End ===== */
        RamTst_ExitCritical();
        return;
      }
      break; /* PRQA S 771 */ /* MD_RamTst_771 */

    }
  }

  /* Is the RamTst_SavedAdd the last address of the configured algorithm parameter. */
  if(RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[RamTst_NoOfBlocks_t
    - 1u].RamTst_StopAddress_pu32 <= RamTst_StopAddCurrentCall_pu32)
  {
    /* Restart the background test from the first address new. */
    RamTst_SavedAdd_pu32
          = RamTst_Config_t.RamTst_AlgParams_at[RamTst_AlgorithmParameterId_t].RamTst_ConfigBlocks_at[0u].RamTst_StartAddress_pu32;
    RamTst_BackgroundLoopCounter_u8 = (0u);
    
    if(RAMTST_RESULT_NOT_OK != RamTst_OverallTestResult_t)
    {
      /* Update the OverallTestResult. */
      RamTst_OverallTestResult_t = RamTst_HelpTestResult_t;
    }

    RamTst_CheckTestResult();
  }
  else
  {
    /* Perceive the start address for the next scheduled call. */
    RamTst_SavedAdd_pu32 = RamTst_StopAddCurrentCall_pu32 + (1u); /* PRQA S 488 */ /* MD_RamTst_488 */
  }

  /* ===== Critical Section End ===== */
  RamTst_ExitCritical();
  return;
} /* RamTst_MainFunction() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH  */ /* PRQA S 6030 */ /* MD_MSR_STCYC */



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (STD_ON == RAMTST_CHECKERBOARD_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_CheckerboardTest()
 *********************************************************************************************************************/
/*! \brief      Executes checkerboard algorithm test
 *  \details    RamTst_CheckerboardTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the checkerboard algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_CheckerboardTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function local variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint32 SavedContent_u32;
    uint8 CellIndex_u8;

    /* ----- Implementation ----------------------------------------------- */
    /* Check the defined RAM Block in ascending order. */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Save the content of the address. */
      SavedContent_u32 = *HelpAdd_pu32;

      /* 1.0: Write checkerboard pattern in the addresses.
       * In this case the pattern is (0xAAAAAAAAu). */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
        /* 1.1: Set the cell with the even index to the value 0s. */
        *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        /* 1.2: Set the cell with the odd index to the value 1s. */
        *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, (CellIndex_u8 + 1u));
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_CHECKERBOARD_CALLOUT1);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 2.0: Check the test pattern from the lower to the higher cell. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
        /* Check the cells:
         *                    1. The cell with the even index must have
         *                       the value 0s.
         *
         *                    2. The cell with the odd index must have
         *                       the value 1s. */
        if( (0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8)) || (0u
                                                              == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (CellIndex_u8 + 1u))) )
        {
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* 2.1: Check the test pattern from the higher to the lower cell. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
          /* Check the cells:
           *                    1. The cell with the even index must have
           *                       the value 0s.
           *
           *                    2. The cell with the odd index must have
           *                       the value 1s. */
          if((0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 2u))) || (0u
                             == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 1u))) )
          {
            return(RAMTST_RESULT_NOT_OK);
          }
      }

      /* 3.0: Write checkerboard pattern in the addresses.
       * In this case the pattern is (0x55555555u). */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
        /* 3.1: Set the cell with the even index to the value 1s. */
        *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        /* 3.2: Set the cell with the odd index to the value 0s. */
        *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, (CellIndex_u8 + 1u));
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_CHECKERBOARD_CALLOUT2);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
          
      /* 4.0: Check the test pattern from the lower to the higher cell. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
        /* Check the cells:
         *                    1. The cell with the even index must have
         *                       the value 1s.
         *
         *                    2. The cell with the odd index must have
         *                       the value 0s. */
        if((0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8)) || (0u
                                                              != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (CellIndex_u8 + 1u))) )
        {
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* 4.1: Check the test pattern from the higher to the lower cell. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8 += 2u)
      {
        /* Check the cells:
         *                    1. The cell with the even index must have
         *                       the value 0s.
         *
         *                    2. The cell with the odd index must have
         *                       the value 1s. */
        if((0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 2u))) || (0u
                             != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 1u))) )
        {
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* Check whether the test is destructive of non-destructive and write the appropriate value */
      if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
      {
        /* Write the configured fill pattern to the tested cell */
        *HelpAdd_pu32 = FillPattern_u32;
      }
      else
      {
        /* Restore the content of the address */
        *HelpAdd_pu32 = SavedContent_u32;
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */

  return(RAMTST_RESULT_OK);
} /* RamTst_CheckerboardTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH  */  /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif  /* (STD_ON == RAMTST_CHECKERBOARD_TEST_SELECTED) */



#if (STD_ON == RAMTST_MARCH_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_MarchTest()
 *********************************************************************************************************************/
/*! \brief      Executes march algorithm test
 *  \details    RamTst_MarchTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the march algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_MarchTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function local variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint32 SavedContent_u32;
    uint8 CellIndex_u8;

    /* ----- Implementation ----------------------------------------------- */
    /* Check the defined addresses. */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Save the contents of the test address. */
      SavedContent_u32 = *HelpAdd_pu32;

      /* 1.0: Write the zero pattern (0x00000000) into the addresses */
      *HelpAdd_pu32 = RAMTST_ZERO_PATTERN;

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_MARCH_CALLOUT1);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* 2.0: Check the written pattern. In this case the value must be 0s. */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          return(RAMTST_RESULT_NOT_OK);
        }
        /* 2.1: Invert the checked cell. */
        else
        {
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_MARCH_CALLOUT2);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* 3.0: Check the written pattern. In this case the value must be 1s. */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
            return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* Check whether the test is destructive of non-destructive and write the appropriate value */
      if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
      {
        /* Write the configured fill pattern to the tested cell */
        *HelpAdd_pu32 = FillPattern_u32;
      }
      else
      {
        /* Restore the content of the address */
        *HelpAdd_pu32 = SavedContent_u32;
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */
  
  return(RAMTST_RESULT_OK);
} /* RamTst_MarchTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (STD_ON == RAMTST_MARCH_TEST_SELECTED) */



#if (STD_ON == RAMTST_WALK_PATH_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_WalkpathTest()
 *********************************************************************************************************************/
/*! \brief      Executes walkpath algorithm test
 *  \details    RamTst_WalkpathTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the walkpath algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_WalkPathTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function local variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint32 SavedContent_u32;
    uint8 CellIndex_u8;

    /* ----- Implementation ----------------------------------------------- */
    /* Check the defined addresses. */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Save the contents of the address. */
      SavedContent_u32 = *HelpAdd_pu32;

      /* 1.0: Initialize the address with a uniform bit stream.
       *      In this case the pattern is (0x00000000). */
      *HelpAdd_pu32 = RAMTST_ZERO_PATTERN;

      /* 2.0: Generation and inspection of the walking pattern. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* 2.1: Invert the cell with the CellIndex. */
        *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_WALKPATH_CALLOUT1);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* 2.2: Inspect the remaining memory area. */
        if(0u != RAMTST_INSPECT_WALKPATH(*HelpAdd_pu32, CellIndex_u8))
        {
          return(RAMTST_RESULT_NOT_OK);
        }

        /* 2.3: Re-invert the cell with the CellIndex. */
        *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
      }

      /* 3.0: Initialize the address with a uniform bit stream.
       *      In this case the pattern is (0xFFFFFFFF). */
      *HelpAdd_pu32 = RAMTST_ONES_PATTERN;

      /* 4.0: Generation and inspection of the walking pattern. */
      for(CellIndex_u8 = 0; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* 4.1: Invert the cell with the CellIndex. */
        *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_WALKPATH_CALLOUT2);

        #endif
        /* 4.2: Inspect the remaining memory area. */
        if(RAMTST_ONES_PATTERN != RAMTST_INSPECT_WALKPATH(*HelpAdd_pu32, CellIndex_u8))
        {
            return(RAMTST_RESULT_NOT_OK);
        }

        /* 4.3: Re-invert the cell with the CellIndex. */
        *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
      }

      /* Check whether the test is destructive of non-destructive and write the appropriate value */
      if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
      {
        /* Write the configured fill pattern to the tested cell */
        *HelpAdd_pu32 = FillPattern_u32;
      }
      else
      {
        /* Restore the content of the address */
        *HelpAdd_pu32 = SavedContent_u32;
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */

  return(RAMTST_RESULT_OK);
} /* RamTst_WalkpathTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (STD_ON == RAMTST_WALK_PATH_TEST_SELECTED) */



#if (STD_ON == RAMTST_GALPAT_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_GalpatTest()
 *********************************************************************************************************************/
/*! \brief      Executes galpat algorithm test
 *  \details    RamTst_GalpatTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the galpat algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GalpatTest (
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function Local Variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint8 TestCellIndex_u8;
    uint32 SavedContent_u32;

    /* ----- Implementation ----------------------------------------------- */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Save the contents of the address */
      SavedContent_u32 = *HelpAdd_pu32;
      /* 1.0: Initialization of the address with the value of RAMTST_ZERO_PATTERN. */
      *HelpAdd_pu32 = RAMTST_ZERO_PATTERN;

      for(TestCellIndex_u8 = 1u; TestCellIndex_u8 <= RAMTST_MIN_NUM_OF_TESTED_CELLS; TestCellIndex_u8++)
      {
        /* 2.0: Invert the test cell.
         *      Pattern sequence (example): 1. 0000
         *                                  2. 0001
         *                                  3. 0011 ... */
        *HelpAdd_pu32 = RAMTST_GALPAT_PATTERN(*HelpAdd_pu32, TestCellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_GALPAT_CALLOUT1);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* 2.1: Check the remaining cells. */
        if((RAMTST_ZERO_PATTERN != RAMTST_GALPAT_INSPECT_ZERO(*HelpAdd_pu32, TestCellIndex_u8))
                || (RAMTST_ONES_PATTERN != RAMTST_GALPAT_INSPECT_ONES(*HelpAdd_pu32, TestCellIndex_u8))) /* PRQA S 3371 */ /* MD_RamTst_3371 */
        {
          return(RAMTST_RESULT_NOT_OK);
        }

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_GALPAT_CALLOUT2);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* 2.2: Check the inverted Cell. */
        if(0u == (RAMTST_MASK_A_CELL(*HelpAdd_pu32, (TestCellIndex_u8 - 1u))))
        {
          /* The inverted cell has not the correct value. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      for(TestCellIndex_u8 = 1u; TestCellIndex_u8 <= RAMTST_MIN_NUM_OF_TESTED_CELLS; TestCellIndex_u8++)
      {
        /* 3.0: Invert the test cell.
         *      Pattern sequence (example): 1. 1111
         *                                  2. 0111
         *                                  3. 0011 ... */
        *HelpAdd_pu32 = RAMTST_GALPAT_ANTI_PATTERN(*HelpAdd_pu32, TestCellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_GALPAT_CALLOUT3);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* 3.1: Check the remaining cells. */
        if((RAMTST_ONES_PATTERN != RAMTST_GALPAT_ANTI_INSPECT_ONES(*HelpAdd_pu32, TestCellIndex_u8))
                            || (RAMTST_ZERO_PATTERN != RAMTST_GALPAT_ANTI_INSPECT_ZERO(*HelpAdd_pu32, TestCellIndex_u8)) )
        {
          return(RAMTST_RESULT_NOT_OK);
        }

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_GALPAT_CALLOUT4);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* 3.2: Check the inverted Cell. */
        if(0u != (RAMTST_MASK_A_CELL(*HelpAdd_pu32, (RAMTST_MIN_NUM_OF_TESTED_CELLS - TestCellIndex_u8))))
        {
          /* The inverted cell has not the correct value. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* Check whether the test is destructive of non-destructive and write the appropriate value */
      if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
      {
        /* Write the configured fill pattern to the tested cell */
        *HelpAdd_pu32 = FillPattern_u32;
      }
      else
      {
        /* Restore the content of the address */
        *HelpAdd_pu32 = SavedContent_u32;
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */

  return(RAMTST_RESULT_OK);
} /* RamTst_GalpatTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (STD_ON == RAMTST_GALPAT_TEST_SELECTED) */



#if (STD_ON == RAMTST_TRANSP_GALPAT_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_TransGalpatTest()
 *********************************************************************************************************************/
/*! \brief      Executes transparent galpat algorithm test
 *  \details    RamTst_TransGalpatTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the transparent galpat algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_TransGalpatTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function Local Variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint8 TestCellIndex_u8;
    uint32 Signature1_u32;
    uint32 Signature2_u32;
    uint32 Signature3_u32;

    /* ----- Implementation ----------------------------------------------- */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Loop over the test cells */
      for(TestCellIndex_u8 = 0u; TestCellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; TestCellIndex_u8++)
      {
        /* Calculate Signature 1 */
        Signature1_u32 = (uint32)(((*HelpAdd_pu32) ^ RAMTST_SIGNATURE_POLY) & ~(uint32)(1 << TestCellIndex_u8));

        /* Invert the current test cell */
        *HelpAdd_pu32 ^= (uint32)(1 << TestCellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_TRANSPARENTGALPAT_CALLOUT1);
        #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
        
        /* Calculate Signature 2 */
        Signature2_u32 = (uint32)(((*HelpAdd_pu32) ^ RAMTST_SIGNATURE_POLY) & ~(uint32)(1 << TestCellIndex_u8));

        /* Compare the signatures */
        if(Signature1_u32 != Signature2_u32)
        {
          /* The signatures are not equal. */
          return(RAMTST_RESULT_NOT_OK);
        }

        /* Invert the test cell again. */
        *HelpAdd_pu32 ^= (uint32)(1 << TestCellIndex_u8);

        #if (defined (V_RAMTST_TEST_CALLOUTS))
        /* ----- MODIFICATION CALLOUT --------------------------------- */
        TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_TRANSPARENTGALPAT_CALLOUT2);

        #endif
        /* Calculate Signature 3 */
        Signature3_u32 = (uint32)(((*HelpAdd_pu32) ^ RAMTST_SIGNATURE_POLY) & ~(uint32)(1 << TestCellIndex_u8));

        /* Compare the signatures */
        if(Signature1_u32 != Signature3_u32)
        {
          /* The signatures are not equal. */
          return(RAMTST_RESULT_NOT_OK);
        }

        if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
        {
          *HelpAdd_pu32 = FillPattern_u32;
        }
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */

  return(RAMTST_RESULT_OK);
} /* RamTst_TransGalpatTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (STD_ON == RAMTST_TRANSP_GALPAT_TEST_SELECTED) */



#if (STD_ON == RAMTST_ABRAHAM_TEST_SELECTED)
/**********************************************************************************************************************
 *  RamTst_AbrahamTest()
 *********************************************************************************************************************/
/*! \brief      Executes Abraham algorithm test
 *  \details    RamTst_AbrahamTest() executes a test for the given RAM area. The desired RAM area is tested with
 *              the Abraham algorithm.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_AbrahamTest(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  #ifndef _MICROSOFT_C_VTT_
    /* ----- Function Local Variables ------------------------------------- */
    volatile P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) HelpAdd_pu32;
    uint32 SavedContent_u32;
    uint8 CellIndex_u8;

    /* ----- Implementation ----------------------------------------------- */
    for(HelpAdd_pu32 = RamTst_StartAdd_pu32; HelpAdd_pu32 <= RamTst_EndAdd_pu32; HelpAdd_pu32++) /* PRQA S 489 */ /* MD_RamTst_489 */
    {
      /* Save the contents of the testing address. */
      SavedContent_u32 = *HelpAdd_pu32;

      /* Initialize all cells of the help address with the value 0 */
      *HelpAdd_pu32 = RAMTST_ZERO_PATTERN;

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT1);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 1.0: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of cell is right.
           * 1.1: Write 1s into the cell. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT2);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 1.2: Check the cells in descending order.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value equal to 1s */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT3);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 2.0: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of cell is right.
           * 2.1: Write 0s into the cell. */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT4);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 2.2: Check the cells in descending order.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value equal to 1s */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
            /* The contents of the cell is wrong. */
            return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT5);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 3.0: Check the cells in descending order.
       *      In this case the contens must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value correct? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of the cell is right.
           * 3.1: Change the value of the cell to 1. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS -
                  CellIndex_u8 - 0x01U));
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT6);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 3.2: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x01U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT7);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 4.0: Check the cells in descending order.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value correct? */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of the cell is right.
           * 4.1: Change the value of the cell to 0. */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS -
                                                                                                   CellIndex_u8 - 0x01U));
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT8);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 4.2: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x01U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT9);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 5.0: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of cell is right.
           * 5.1: Write 1s into the cell. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
          /* Write 0s into the cell. */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT10);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 5.2: Check the cells in descending order.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value equal to 1s */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT11);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 6.0: Check the cells in descending order.
       *      In this case the content must be 0. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value correct? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of the cell is right.
           * 6.1: Change the value of the cell to 1. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 -
                  0x01U));
          /* Change the value again to 0 */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS -
                  CellIndex_u8 - 0x01U));
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT12);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 6.2: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 0. */
      for(CellIndex_u8 = 0x01U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* Initialize all cells of the help address with the value 1 */
      *HelpAdd_pu32 = RAMTST_ONES_PATTERN;

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT13);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 7.0: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of cell is right.
           * 7.1: Write 0s into the cell. */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
          /* Write 1s into the cell. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, CellIndex_u8);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT14);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 7.2: Check the cells in descending order.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value equal to 1s */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT15);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */

      /* 8.0: Check the cells in descending order.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x00U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the current cell value correct? */
        if(0u == RAMTST_MASK_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 - 0x01U)))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
        else
        {
          /* The contents of the cell is right.
           * 8.1: Change the value of the cell to 1. */
          *HelpAdd_pu32 = RAMTST_SET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS - CellIndex_u8 -
                  0x01U));
          /* Change the value again to 0 */
          *HelpAdd_pu32 = RAMTST_RESET_A_CELL(*HelpAdd_pu32, (uint8)(RAMTST_MIN_NUM_OF_TESTED_CELLS -
                  CellIndex_u8 - 0x01U));
        }
      }

      #if (defined (V_RAMTST_TEST_CALLOUTS))
      /* ----- MODIFICATION CALLOUT --------------------------------- */
      TsxRamTst_ModificationCallout(HelpAdd_pu32, TSXRAMTST_ABRAHAM_CALLOUT16);
      #endif /* (defined (V_RAMTST_TEST_CALLOUTS)) */
      
      /* 8.2: Check the cells in ascending order from the lower to higher address.
       *      In this case the contents must be 1. */
      for(CellIndex_u8 = 0x01U; CellIndex_u8 < RAMTST_MIN_NUM_OF_TESTED_CELLS; CellIndex_u8++)
      {
        /* Is the contents of the cell right? */
        if(0u != RAMTST_MASK_A_CELL(*HelpAdd_pu32, CellIndex_u8))
        {
          /* The contents of the cell is wrong. */
          return(RAMTST_RESULT_NOT_OK);
        }
      }

      /* Check whether the test is destructive of non-destructive and write the appropriate value */
      if(TestPolicy_u8 == RAMTST_DESTRUCTIVE)
      {
        /* Write the configured fill pattern to the tested cell */
        *HelpAdd_pu32 = FillPattern_u32;
      }
      else
      {
        /* Restore the content of the address */
        *HelpAdd_pu32 = SavedContent_u32;
      }
    }
  #else /* #ifdef _MICROSOFT_C_VTT_ */
    /* Assign all the parameters to themselves to avoid compiler warnings */
    RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32;
    RamTst_EndAdd_pu32   = RamTst_EndAdd_pu32;
    FillPattern_u32      = FillPattern_u32;
    TestPolicy_u8        = TestPolicy_u8;
  #endif /* #ifdef _MICROSOFT_C_VTT_ */

  return(RAMTST_RESULT_OK);
} /* RamTst_AbrahamTest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH  */  /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif  /* (STD_ON == RAMTST_ABRAHAM_TEST_SELECTED) */



/**********************************************************************************************************************
 *  RamTst_Dummy()
 *********************************************************************************************************************/
/*! \brief      Dummy service for RAMTST_ALGORITHM_UNDEFINED
 *  \details    RamTst_Dummy() is called by the function pointer array in case that RAMTST_ALGORITHM_UNDEFINED is
 *              configured in an algorithm parameter set.
 *  \param[in]  RamTst_StartAdd_pu32    Pointer to the start address
 *  \param[in]  RamTst_EndAdd_pu32      Pointer to the start address
 *  \param[in]  FillPattern_u32         Pattern the tested area should be filled with if the test policy is
 *                                      configured as RAMTST_DESTRUCTIVE. In case it is RAMTST_NON_DESTRUCTIVE, this
 *                                      value is ignored.
 *  \param[in]  TestPolicy_u8           Test policy for the current test cycle
 *  \return     1 - RAMTST_RESULT_OK
 *  \return     2 - RAMTST_RESULT_NOT_OK
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_Dummy(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Assign all the parameters to themselves to avoid compiler warnings */
  RamTst_StartAdd_pu32 = RamTst_StartAdd_pu32; /* PRQA S 3199 */ /* MD_RamTst_3199 */
  RamTst_EndAdd_pu32 = RamTst_EndAdd_pu32; /* PRQA S 3199 */ /* MD_RamTst_3199 */
  FillPattern_u32 = FillPattern_u32; /* PRQA S 3199 */ /* MD_RamTst_3199 */
  TestPolicy_u8 = TestPolicy_u8; /* PRQA S 3199 */ /* MD_RamTst_3199 */

  return(RAMTST_RESULT_OK);
}

/**********************************************************************************************************************
 *  RamTst_CheckTestResult()
 *********************************************************************************************************************/
/*! \brief      Calls the appropriate result notification
 *  \details    RamTst_CheckTestResult() checks the overall test result and calls the according notification if the
 *              configured notification pointer is not the NULL_PTR.
 *  \return     none
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
static FUNC(void, RAMTST_CODE) RamTst_CheckTestResult(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Check the test result to determine which notification function has to be called */
  if(RAMTST_RESULT_OK == RamTst_OverallTestResult_t)
  {
    /* Check for NULL_PTR and call the configured notification */
    if(NULL_PTR != RamTst_Config_t.RamTst_TestCompletedNotification_pt)
    {
      RamTst_Config_t.RamTst_TestCompletedNotification_pt();
    }
  }
  else
  {
    /* Check for NULL_PTR and call the configured notification */
    if(NULL_PTR != RamTst_Config_t.RamTst_ErrorNotification_pt)
    {
      RamTst_Config_t.RamTst_ErrorNotification_pt();
    }
  }

  return;
} /* RamTst_CheckTestResult() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */


#define RAMTST_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1  */

/* Justification for module-specific MISRA deviations:

  MD_RamTst_488:
    Reason:     Pointer arithmetic is used to calculate the start and stop addresses of the tested memory areas
    Risk:       Pointer may access unintended or invalid memory addresses
    Prevention: Correctness has been verified in the component test
  
  MD_RamTst_489:
    Reason:     Pointer arithmetic is used to iterate over the tested RAM areas
    Risk:       Pointer may access unintended or invalid memory addresses
    Prevention: Correctness has been verified in the component test
  
  MD_RamTst_771:
    Reason:     Two 'break' statements are used for efficiency reasons
    Risk:       Poor readability and maintainability of the code
    Prevention: None

  MD_RamTst_3199:
    Reason:     This is a dummy function that has to have the correct prototype and contains no effective code. The
                function is needed to return the status
    Risk:       Unnecessary code
    Prevention: None
  
  MD_RamTst_3218:
    Reason:     Variable is declared global static even though it is only used in one function because of debug
                purposes (main function)
    Risk:       Unnecessary memory usage
    Prevention: None
  
  MD_RamTst_3371:
    Reason:     Intended left shift of #define (unsigned) value
    Risk:       Truncation of the value due to unsigned left shift
    Prevention: Correctness has been verified in the component test
  
  MD_RamTst_3408:
    Reason:     Variable has external linkage, but is defined without previous declaration, because the variable has 
                to be declared as 'global' variable due to AUTOSAR debugging requirements. Inclusion by another module
                is not intended.
    Risk:       Variable cannot be used in another file (no include possible)
    Prevention: None
  
  MD_RamTst_4109:
    Reason:     Arithmetic expression is intentionally used as the right hand operation of a logical operator
    Risk:       Is probably a mistake
    Prevention: Correctness has been verified in the component test
  
*/

 /*********************************************************************************************************************
  *  END OF FILE: RamTst.c
  ********************************************************************************************************************/
