/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  RamTst.h
 *        \brief  RamTst header file
 *
 *      \details  This is the header file of the module RamTst. This version covers the document
 *                AUTOSAR_SWS_RAMTest Version 1.5.0.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials        Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Anton Eisenhut                viraet             Vector Informatik GmbH
 *  Christoph Ederer              vircer             Vector Informatik GmbH
 *  Bethina Mausz                 virbmz             Vector Informatik GmbH
 *  Christian Leder               virchl             Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  0.09.00   2010-09-14  viraet      -           Initial implementation
 *  1.00.00   2011-05-16  vircer      -           Finalization of the initial version, documentation of MISRA
 *                                                violations, addition of modification call-outs, added conditional
 *                                                overwrite for destructive testing
 *  1.00.01   2011-06-28  vircer  ESCAN00051035   BETA state removal
 *                                ESCAN00051894   Parameter check of the service RamTst_SelectAlgParams reworked
 *                                ESCAN00051895   Abraham algorithm reworked
 *                                ESCAN00051896   Galpat algorithm reworked
 *                                ESCAN00052202   Declaration of the AUTOSAR debugging variables changed to global
 *                                ESCAN00052203   Return value of RamTst_GetAlgParams() corrected
 *                                ESCAN00052204   Correction of DEM error reporting in background testing mode (DEM
 *                                                errors are now reported right after each main function cycle)
 *                                ESCAN00052206   Update mechanism of the overall test result reworked
 *                                ESCAN00052207   Module ID and vendor ID corrected
 *  1.00.02   2012-02-02  vircer  ESCAN00056498   Rework of steps 2.1 and 4.1 checkerboard algorithm
 *  1.00.03   2012-02-24  vircer  ESCAN00057068   Rework of the signature calculation in the transparent galpat test
 *                                ESCAN00057070   Rework of the block change in the main function
 *  2.00.00   2012-07-30  vircer  ESCAN00058164   Update to AUTOSAR 4.0.3 (Version information, Memory Sections,
 *                                                change of Std_VersionInfoType)
 *  2.00.01   2013-07-18  vircer  ESCAN00061621   Missing Critical Section exit added in main function
 *                                ESCAN00059093   Symbolic Name Prefixing added
 *  2.00.02   2014-01-23  virbmz  ESCAN00069558   the RAMTST_START_SEC_CONST_PBCFG memory section defines now 
 *                                                START_SEC_CONST_UNSPECIFIED because the START_SEC_CONST_PBCFG define
 *                                                is used for Postbuild-Loadable
 *  2.00.03   2014-06-18  virbmz  ESCAN00076331   Provide possibility to configure call cycle of main function - CFG 
 *                                                version update
 *  2.01.00   2014-12-17  virchl  ESCAN00076530   Implementation of RamTst_InitMemory()
 *                                ESCAN00080343   Change of Dem error according to AUTOSAR Specification 
 *                                                (RAMTST_E_RAM_FAILURE)
 *  2.01.01   2015-07-24  virchl  ESCAN00083670   Function pointer array RamTst_AlgorithmFuncTable_at declared as CONST
 *  2.02.00   2015-11-26  virchl  ESCAN00087266   FEAT-1372: Update of SwVersions due to increase of generator version
 *                                ESCAN00086792   Add component for VTT scope
 *********************************************************************************************************************/

#if !defined(RAMTST_H)
#define RAMTST_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "RamTst_Cfg.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_AsrRamTst CQComponent : <Implementation> */

/* vendor and module identification */
#define RAMTST_VENDOR_ID                            (0x1EU)
#define RAMTST_MODULE_ID                            (0x5DU)

/* AUTOSAR Software Specification Version Information */
#define RAMTST_AR_MAJOR_VERSION                     (0x01U)
#define RAMTST_AR_MINOR_VERSION                     (0x05U)
#define RAMTST_AR_PATCH_VERSION                     (0x00U)

/* AUTOSAR Release Version */
#define RAMTST_AR_RELEASE_MAJOR_VERSION             (0x04U)
#define RAMTST_AR_RELEASE_MINOR_VERSION             (0x00U)
#define RAMTST_AR_RELEASE_REVISION_VERSION          (0x03U)

/* Component Version Information */
#define RAMTST_SW_MAJOR_VERSION                     (0x02U)
#define RAMTST_SW_MINOR_VERSION                     (0x02U)
#define RAMTST_SW_PATCH_VERSION                     (0x00U)

#define RAMTST_INSTANCE_ID                          (0x00U)

/* Error codes reported to Det, in case a development error is detected                                 */
#define RAMTST_E_STATUS_FAILURE                     (0x01U)   /* A particular API is called in an unexpected state */
#define RAMTST_E_OUT_OF_RANGE                       (0x02U)   /* API parameter out of specified range              */
#define RAMTST_E_UNINIT                             (0x03U)   /* API service used without module initialization    */
#define RAMTST_E_ALREADY_INITIALIZED                (0x10U)   /* RamTst_Init() was called while the module is already
                                                                 initialized */
#define RAMTST_E_PARAM_VINFO                        (0x11U)   /* NULL_PTR passed to Dio_GetVersioninfo()           */

/* ID for the different API services, used for identification, in case a development error is  reported to Det */
#define RAMTST_SID_INIT                             (0x00U)
#define RAMTST_SID_DEINIT                           (0x01U)
#define RAMTST_SID_STOP                             (0x02U)
#define RAMTST_SID_ALLOW                            (0x03U)
#define RAMTST_SID_SUSPEND                          (0x04U)
#define RAMTST_SID_RESUME                           (0x05U)
#define RAMTST_SID_GET_EXECUTION_STATUS             (0x06U)
#define RAMTST_SID_GET_TEST_RESULT                  (0x07U)
#define RAMTST_SID_GET_TEST_RESULT_PER_BLOCK        (0x08U)
#define RAMTST_SID_GET_VERSION_INFO                 (0x09U)
#define RAMTST_SID_GET_ALG_PARAMS                   (0x0AU)
#define RAMTST_SID_GET_TEST_ALGORITHM               (0x0BU)
#define RAMTST_SID_GET_NUMBER_OF_TESTED_CELLS       (0x0CU)
#define RAMTST_SID_SELECT_ALG_PARAMS                (0x0DU)
#define RAMTST_SID_CHANGE_NUMBER_OF_TESTED_CELLS    (0x0EU)
#define RAMTST_SID_RUN_FULL_TEST                    (0x0FU)
#define RAMTST_SID_RUN_PARTIAL_TEST                 (0x10U)

#define RAMTST_DESTRUCTIVE                          (0x00U)
#define RAMTST_NON_DESTRUCTIVE                      (0x01U)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* This type indicates the test result */
typedef enum
{
  RAMTST_RESULT_NOT_TESTED = 0u,
  RAMTST_RESULT_OK,
  RAMTST_RESULT_NOT_OK,
  RAMTST_RESULT_UNDEFINED
} RamTst_TestResultType;

/* This type indicates the execution status */
typedef enum
{
  RAMTST_EXECUTION_UNINIT = 0u,
  RAMTST_EXECUTION_STOPPED,
  RAMTST_EXECUTION_RUNNING,
  RAMTST_EXECUTION_SUSPENDED,
  RAMTST_EXECUTION_ALLOWED
} RamTst_ExecutionStatusType;

/* Function pointer definition for the algorithm pointer */
typedef P2FUNC(RamTst_TestResultType, RAMTST_CODE, RamTst_AlgFunc_p)
(
  P2VAR(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_StartAdd_pu32,
  P2CONST(uint32, AUTOMATIC, RAMTST_APPL_DATA) RamTst_EndAdd_pu32,
  uint32 FillPattern_u32,
  uint8 TestPolicy_u8
);


/* This enum is to check the from which function the algorithm is call.
 * RAMTST_CALLED_BY_FUNCTION_NO_CALL: no call
 * RAMTST_CALLED_BY_FUNCTION_FULL_OR_PARTIAL: from RamTst_RunFullTest or RamTst_RunPartialTest
 * RAMTST_CALLED_BY_FUNCTION_MAIN: from RamTst_MainFunction                                     */
typedef enum
{
  RAMTST_CALLED_BY_FUNCTION_NO_CALL = 0u,
  RAMTST_CALLED_BY_FUNCTION_FULL_OR_PARTIAL,
  RAMTST_CALLED_BY_FUNCTION_MAIN
}RamTst_CalledByFunctionType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define RAMTST_START_SEC_CODE
#include "MemMap.h"

FUNC(void, RAMTST_CODE) RamTst_InitMemory(void);

FUNC(void, RAMTST_CODE) RamTst_Init(void);

FUNC(void, RAMTST_CODE) RamTst_Deinit(void);

#if (RAMTST_STOP_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_Stop(void);
#endif

#if (RAMTST_ALLOW_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_Allow(void);
#endif

#if (RAMTST_SUSPEND_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_Suspend(void);
#endif

#if (RAMTST_RESUME_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_Resume(void);
#endif

#if (RAMTST_GETEXECUTIONSTATUS_API == STD_ON)
FUNC(RamTst_ExecutionStatusType, RAMTST_CODE) RamTst_GetExecutionStatus(void);
#endif

#if (RAMTST_GETTESTRESULT_API == STD_ON)
FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GetTestResult(void);
#endif

#if (RAMTST_GETTESTRESULTPERBLOCK_API == STD_ON)
FUNC(RamTst_TestResultType, RAMTST_CODE) RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID);
#endif

#if (RAMTST_GETVERSIONINFO_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RAMTST_APPL_DATA) versioninfo);
#endif

#if (RAMTST_GETALGPARAMS_API == STD_ON)
FUNC(RamTst_AlgParamsIdType, RAMTST_CODE) RamTst_GetAlgParams(void);
#endif

#if (RAMTST_GETTESTALGORITHM_API == STD_ON)
FUNC(RamTst_AlgorithmType, RAMTST_CODE) RamTst_GetTestAlgorithm(void);
#endif

#if (RAMTST_GETNUMBEROFTESTEDCELLS_API == STD_ON)
FUNC(RamTst_NumberOfTestedCellsType, RAMTST_CODE) RamTst_GetNumberOfTestedCells(void);
#endif

#if (RAMTST_SELECTALGPARAMS_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId);
#endif

#if (RAMTST_CHANGENUMBEROFTESTEDCELLS_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells);
#endif

#if (RAMTST_RUNFULLTEST_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_RunFullTest(void);
#endif

#if (RAMTST_RUNPARTIALTEST_API == STD_ON)
FUNC(void, RAMTST_CODE) RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId);
#endif

FUNC(void, RAMTST_CODE) RamTst_MainFunction(void);

#define RAMTST_STOP_SEC_CODE
#include "MemMap.h"


#endif


/**********************************************************************************************************************
 *  END OF FILE: RamTst.h
 *********************************************************************************************************************/
