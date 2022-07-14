/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!     \file      FiM.h
 *      \brief     Header for Autosar 4 module FiM
 *
 *      \details   Static header file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Joachim Kalmbach              visjk         Vector Informatik GmbH
 *  Katrin  Thurow                visktw        Vector Informatik GmbH
 *  Thomas  Necker                vistne        Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *            2012-08-22  visjk   ---           3.01.00: AR 3 FiM version that this module was originally based on
 *                        visktw  ---           ditto
 *  1.00.00   2012-10-11  vistne  ESCAN00061742 AR4-148: FiM AR4 Support
 *                        vistne  ESCAN00061816 AR4-220: Remove STATIC
 *  1.01.00   2013-03-15  vistne  ESCAN00063875 Add Offline Calibration Support
 *                        vistne  ESCAN00063883 FiM include hierarchy does not conform to AR standard
 *                        vistne  ESCAN00064191 Support Cyclic Event Evaluation
 *                        vistne  ESCAN00064193 Support of FiM_GetFunctionPendingStatus
 *  1.02.00   2013-06-28  vistne  ESCAN00067461 Include of Dem.h was moved to different file
 *                        vistne  ESCAN00067956 Support absence of RTE generated files
 *  2.00.00   2013-11-01  vistne  ESCAN00070055 Compiler error: FiM.c(XXX) : 'FiM_FidPendingCounter' : undeclared identifier
 *                        vistne  ESCAN00070532 Possible out of bounds access to EventIdTable
 *                        vistne  ESCAN00070624 New Binary Search Algorithm for EventIds
 *                        vistne  ESCAN00070687 New Direct Table Access for EventIds
 *  2.01.00   2014-03-07  vistne  ESCAN00071946 Harmonize usage of boolean and Boolean
 *                        vistne  ESCAN00072079 Pending status of FID is influenced by event even if corresponding
 *                                              event/FID link has invalid inhibition configuration
 *                        vistne  ESCAN00072092 AR4-580: FiM Inhibition Config Optimizations
 *                        vistne  ESCAN00072428 AR4-580: FiM Postbuild Loadable
 *  3.00.00   2014-10-31  vistne  ESCAN00078673 Introduce DET code FIM_E_INITIALIZATION_NOT_COMPLETED
 *                        vistne  ESCAN00078676 FiM_GetFunctionPermission and FiMGetFunctionPendingStatus return
 *                                              E_NOT_OK in all error cases
 *  3.01.00   2015-03-20  vistne  ESCAN00080166 Return value for FiM_DemTriggerOnEventStatus does not comply to
 *                                              AUTOSAR-Standard
 *                        vistne  ESCAN00080369 FiM_GetFunctionPermission and FiMGetFunctionPendingStatus do
 *                                              plausibility checks only if FIM_DEV_ERROR_DETECT is enabled
 *                        vistne  ESCAN00080605 Ignore events when Dem_GetEventStatus does not return E_OK
 *                        vistne  ESCAN00080284 FEAT-427: SafeBSW Step I
 *  4.00.00   2016-01-08  vistne  ESCAN00083200 Remove inclusion of v_ver.h
 *                        vistne  ESCAN00083639 Compiler warning: narrowing or signed-to-unsigned type conversion found
 *                        vistne  ESCAN00085144 FEAT-1527: SafeBSW Step 3
 *                        vistne  ESCAN00086406 Reduce number of entries into critical sections
 *  4.01.00   2016-07-27  vistne  ESCAN00089387 FEAT-1734: SafeBSW Step 5
 *  4.02.00   2016-11-18  vistne  FEATC-12      FEAT-1920: FiM - SafeBSW Step6
 *                        vistne  ESCAN00091090 Incorrect initialization status detection
 *                        vistne  ESCAN00091787 Removed Critical Sections in FiM_GetFunctionPermission and
 *                                              FiM_GetFunctionPendingStatus
***********************************************************************************************************************/
#if !defined(FIM_H)
#define FIM_H

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "FiM_Cfg.h"
#include "FiM_Types.h"
#include "FiM_Lcfg.h"
#include "FiM_PBcfg.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Vendor and module identification */
#define FIM_VENDOR_ID                               (30u) /* Vector Informatik GmbH */
#define FIM_MODULE_ID                               (11u) /* FIM */
/* AUTOSAR Software Specification Version Information - decimal encoding */
#define FIM_AR_RELEASE_MAJOR_VERSION                (4u)
#define FIM_AR_RELEASE_MINOR_VERSION                (0u)
#define FIM_AR_RELEASE_REVISION_VERSION             (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define FIM_SW_MAJOR_VERSION                        (0x04)/* decimal coded version number         */
#define FIM_SW_MINOR_VERSION                        (0x02)
#define FIM_SW_PATCH_VERSION                        (0x00)/* decimal coded release version number */

#define FIM_INSTANCE_ID_DET                         (0x00u)

/*! * \defgroup  FimDetSids API Service IDs for reporting errors to the DET
 * API Service IDs for reporting errors to the DET with Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID, ServiceId,
 * ErrorId)
 * \{ */
#define FIM_SID_FIM_INIT                            (0x00u) /*!< Service ID: FiM_Init() */
#define FIM_SID_FIM_GETFUNCTIONPERMISSION           (0x01u) /*!< Service ID: FiM_GetFunctionPermission() */
#define FIM_SID_FIM_DEMTRIGGERONEVENTSTATUS         (0x02u) /*!< Service ID: FiM_DemTriggerOnEventStatus() */
#define FIM_SID_FIM_DEMINIT                         (0x03u) /*!< Service ID: Fim_DemInit() */
#define FIM_SID_FIM_GETVERSIONINFO                  (0x04u) /*!< Service ID: FiM_GetVersionInfo() */
#define FIM_SID_FIM_MAINFUNCTION                    (0x05u) /*!< Service ID: FiM_MainFunction() */
/* Additional API Service IDs for reporting errors to the DET */
# if (FIM_FEATURE_OBD == STD_ON)
#  define FIM_SID_FIM_GETFUNCTIONPENDINGSTATUS      (0x80u) /*!< Service ID: FiM_GetFunctionPendingStatus() */
#endif
/*! \} */

/*! * \defgroup  FimDetErrId ErrorIds for reporting errors to the DET
 * ErrorIds for reporting errors to the DET
 * \{ */
#define FIM_E_NO_ERROR                              (0x00u) /*!< used to check if no error occurred */
#define FIM_E_WRONG_PERMISSION_REQ                  (0x01u) /*!< FiM_GetFunctionPermission is called by the application before complete initialization */
#define FIM_E_WRONG_TRIGGER_ON_EVENT                (0x02u) /*!< DEM calls FIM before the FIM is initialized */
#define FIM_E_FID_OUT_OF_RANGE                      (0x03u) /*!< FiM_GetFunctionPermission called with wrong FID */
#define FIM_E_INVALID_POINTER                       (0x05u) /*!< API is invoked with NULL pointer */
#define FIM_E_INVALID_EVENTSTATUSEXTENDEDTYPE       (0x06u) /*!< DEM calls FIM with wrong Dem_EventStatusExtendedType */
/* Additional ErrorIds for reporting errors to the DET */
#define FIM_E_DEM_GETEVENTSTATUS_WRONG_RETURN_VALUE (0x80u) /*!< Call to Dem_GetEventStatus not successful */
#define FIM_E_INITIALIZATION_NOT_COMPLETED          (0x81u) /*!< Initialization of FIM not completed */
/*! \} */

/*! * \defgroup  IdxInhModTable Indexes for inhibition configuration.
 * Indexes for inhibition configuration.
 * \{ */
#define FIM_INHCODE_INVALID                         (0u)
#define FIM_INHCODE_LAST_FAILED                     (1u)
#define FIM_INHCODE_NOT_TESTED                      (2u)
#define FIM_INHCODE_TESTED                          (3u)
#define FIM_INHCODE_TESTED_AND_FAILED               (4u)
#define FIM_INHCODE_NOT_TESTED_OR_FAILED            (5u)
#define FIM_INHCODE_NUMBER                          (6u)
/*! \} */

/*! Invalid DEM event id. */
#define FIM_DEM_EVENT_INVALID                       (0u)

/*! Minimum list length necessary to apply binary search algorithm. */
#if !defined FIM_BINSEARCH_MINLISTLENGTH                                                  /* COV_MSR_COMPATIBILITY TX */
# define FIM_BINSEARCH_MINLISTLENGTH                (5u)
#endif

#if (FIM_DATA_FIXED == STD_ON)
#  error "Invalid configuration: Calibration not supported anymore"
#endif

#if (FIM_CYCLIC_EVENT_EVALUATION == STD_ON)
#  error "Invalid configuration: FIM_CYCLIC_EVENT_EVALUATION not supported"
#endif


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/* for tables that are accessed via Event Id (which references several FIDs) - TriggerOnEvent (optimized)     */
/* ------------------------------------------------------------------------------------------------------     */
/* main use case; uses ComStackLib                                                                            */

/* one element in the event id table                                  */
/* optimized data structure (NO calibration), FIM_EVENTTOINHTABLE     */
/* FiM_EventIdTableType:                                              */
/*    eventIdType                   EventId;                          */
/*    eventToInhCodeTableIndexType  EventToFidTableIndex;             */

/* one element in the inhibition configuration tables (InhCode/FIDIdx)*/
/* actually two separate tables                                       */
/* FiM_InhCodeTableType             InhCode                           */
/* FiM_InhFidIdxTableType           InhFidIdx                         */

/* one element in the FID table                                       */
/* FiM_FunctionIdType               Fid                               */


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
extern P2CONST(FiM_ConfigType, FIM_VAR_NOINIT, FIM_INIT_DATA)  FiM_ConfigDataPtr;
#endif

#define FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */                                                          


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FiM_InitMemory()
 **********************************************************************************************************************/
/*! \brief                    Use this function to initialize static RAM variables.
 *                            - Extension to Autosar -  
 *  \details                  Use when the start-up code is not used to initialize RAM.
 *  \pre                      Module is uninitialized.
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \trace                    CREQ-103737
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_InitMemory(void);

/***********************************************************************************************************************
 *  FiM_Init()
 **********************************************************************************************************************/
/*! \brief                    Initializes FiM
 *
 *  \details                  This function shall be used during the startup phase of the ECU after
 *                            the DEM is initialized to initialize the FIM.
 *                            It initializes all component variables and sets the component state to initialized
 *                            if no error occurred.
 *
 *  \param[in]  FiMConfigPtr  Root pointer to FiM's configuration.
 *
 *  \pre                      DEM is initialized.
 *  \pre                      FiM_InitMemory has been called unless FiM_InitializationStatus is initialized by
 *                            start-up code.
 *  \context                  TASK
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22910
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_Init
                                  (P2CONST(FiM_ConfigType,
                                   AUTOMATIC, FIM_INIT_DATA) FiMConfigPtr);

/***********************************************************************************************************************
 *  Fim_DemInit()
 **********************************************************************************************************************/
/*! \brief                    This service re-initializes the FiM with current status of DEM.
 *
 *  \details                  Function shall be called by DEM to reinitialize the FIM in case the DEM detects a status
 *                            change of a certain number of events.
 *                            CAUTION: During the complete runtime of FiM_DemInit, FiM_DemTriggerOnEventStatus
 *                            must not be called! 
 *                            CAUTION: The FiM loops through all configured events or FIDs and calls Dem_GetEventStatus
 *                            for each of them. Depending on the configuration, FiM_DemInit can take a considerable
 *                            time.
 *  \pre                      -
 *  \context                  TASK
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22936, DSGN-FiM22867
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemInit(void);

/***********************************************************************************************************************
 *  FiM_GetFunctionPermission()
 **********************************************************************************************************************/
/*! \brief      This function shall be used to query the permission of the FID.
 *
 *  \details    - This function  reports the permission state of the functionality assigned to the FID.
 *              - If development error detection is enabled:
 *                - Permission will be set to FALSE, if the FIM is not initialized or if the FID is not valid.
 *                - If development error reporting is enabled, an error will additionally be reported to the DET.
 *
 *  \param[in]  FID           Function ID that the query is for (identifies a functionality).
 *                            Min.:   0
 *                            Max.:   Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 *  \param[out] Permission    Must point to a variable of type boolean.
 *                            TRUE:   FID has permission to run 
 *                            FALSE:  FID has no permission to run, i.e. shall not be executed 
 *  \return                   E_OK:     The request is accepted, Permission could be obtained and is valid
 *                            E_NOT_OK: The request is not accepted, e.g. initialization of FIM not completed 
 *                                      Permission could not be obtained.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22861
 **********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPermission
                                  (FiM_FunctionIdType FID,
                                   P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission
                                  );

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_GetFunctionPendingStatus()
 **********************************************************************************************************************/
/*! \brief      This function shall be used to query the pending status of the events connected to the FID.
 *
 *  \details    - This function is used in context of IUMPR calculation for OBD.
 *              - It reports the pending status of the event assigned to the FID.
 *              - If development error detection is enabled:
 *                - pending status will be set to FALSE, if the FIM is not initialized or if the FID is not valid,
 *                - if development error reporting is enabled, an error will additionally be reported to the DET.
 *
 *  \param[in]  FID           Function ID that the query is for (identifies a functionality).
 *                            Min.:   0
 *                            Max.:   Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 *  \param[out] pendingStatus Must point to a variable of type boolean.
 *                            TRUE:   any event connected to FID has status bit DEM_UDS_STATUS_PDTC set
 *                            FALSE:  no event connected to FID has status bit DEM_UDS_STATUS_PDTC set
 *  \return                   E_OK:     The request is accepted, pending status could be obtained and is valid
 *                            E_NOT_OK: The request is not accepted, e.g. initialization of FIM not completed 
 *                                      Pending status could not be obtained.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON (OBD license available and OBD enabled at compile time)
 *  \trace                    DSGN-FiM22868
 **********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPendingStatus
                                  (FiM_FunctionIdType FID, 
                                   P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) pendingStatus
                                  );
#endif

/***********************************************************************************************************************
 *  FiM_DemTriggerOnEventStatus()
 **********************************************************************************************************************/
/*! \brief      Function is provided to DEM in order to call FIM upon status changes.
 *
 *  \details                    This function is provided to DEM in order to call FIM upon status changes.
 *  \param[in]  EventId         Identification of an Event by assigned event number.
 *                              The Event Number is configured in the DEM. 
 *                              Min.: 1 (0: Indication of no Event or Failure) 
 *                              Max.: depending on configuration of Event Numbers in DEM (Max is either 255 or 65535) 
 *  \param[in]  EventStatusOld  extended event status before change
 *  \param[in]  EventStatusNew  extended event status after change
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 *  \synchronous                TRUE
 *  \trace                      DSGN-FiM22911
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemTriggerOnEventStatus
                                  (Dem_EventIdType EventId,
                                   Dem_EventStatusExtendedType EventStatusOld,
                                   Dem_EventStatusExtendedType EventStatusNew
                                  );

#if (FIM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  FiM_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief                      This function returns the version information of the FiM.
 *
 *  \details                    The version information contains vendor ID, moduleID, major/minor/patch version number. 
 *                              The version numbers are decimal coded.
 *  \param[out] versioninfo     Pointer to where to store the version information of this module. Must point to a
 *                              variable of type Std_VersionInfoType.
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 *  \synchronous                TRUE
 *  \config                     FIM_VERSION_INFO_API == STD_ON
 *  \trace                      CREQ-103738
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_GetVersionInfo
                                  (P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo
                                  );   
#endif

/***********************************************************************************************************************
 *  FiM_MainFunction()
 **********************************************************************************************************************/
/*! \brief                      Main function without functionality. Needs not be cyclically scheduled. 
 *
 *  \details                    This function does nothing, as cyclic event evaluation is not supported.
 *
 *  \pre                        -
 *  \context                    TASK
 *  \reentrant                  FALSE
 *  \synchronous                TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_MainFunction(void);

#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/*!
 * \exclusivearea FIM_EXCLUSIVE_AREA_0
 *                Ensures consistency of the FID (Pending) counters.
 * \protects      FiM_FidCounter, FiM_FidPendingCounter
 * \usedin        FiM_Init, FiM_DemInit, FiM_DemTriggerOnEventStatus, FiM_GetFunctionPermission,
 *                FiM_GetFunctionPendingStatus
 * \exclude       -
 * \length        SHORT  protects a short code section for inc/decrementing up to two counters
 * \endexclusivearea
 */


#endif /* FIM_H */

/***********************************************************************************************************************
 *  END OF FILE: FiM.h
 **********************************************************************************************************************/
