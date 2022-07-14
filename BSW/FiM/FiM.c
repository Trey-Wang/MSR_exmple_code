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
/*!        \file     FiM.C
 *         \brief    Source for Autosar 4 module FiM
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
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
 *  2.00.00   2013-11-01  vistne  ESCAN00070055 Compiler error: FiM.c(XXX):'FiM_FidPendingCounter':undeclared identifier
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

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */

/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Dem.h"
#include "FiM.h"

#include "SchM_FiM.h"

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* Include Development Error Tracer */
# include "Det.h"                        
#endif

#if (FIM_CFG_USE_RTE == STD_ON)
# include "Rte_FiM.h"
#endif

/* EcuM for ErrorHook */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
# include "EcuM_Error.h"
#endif

/*! Enter critical section of FiM */
#define FiM_EnterCritical()   SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_0()
/*! Leave critical section of FiM */
#define FiM_LeaveCritical()   SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_0()

/***********************************************************************************************************************
*  VERSION CHECK
***********************************************************************************************************************/
/* vendor specific version information is decimal coded */
#if (  (FIM_SW_MAJOR_VERSION != (0x04)) \
    || (FIM_SW_MINOR_VERSION != (0x02)) \
    || (FIM_SW_PATCH_VERSION != (0x00)) )      
# error "Vendor specific version numbers of FiM.c and FiM.h are inconsistent"
#endif

/* check whether whether configuration files were generated with compatible generator version */
#if (  (FIM_CFG_GEN_MAJOR_VERSION != (0x04)) \
    || (FIM_CFG_GEN_MINOR_VERSION != (0x01)) )
# error "Incompatible configuration file (generator) version used!"
#endif

/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/
#if !defined (FIM_LOCAL)                                                                  /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local functions */
# define FIM_LOCAL                 static
#endif

#if !defined (FIM_LOCAL_INLINE)                                                           /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local inline functions */
# define FIM_LOCAL_INLINE          LOCAL_INLINE
#endif

/*! FiM is in uninitialized state */
#define FIM_UNINITIALIZED           0
/*! FiM is in initialized state */
#define FIM_INITIALIZED             1

/*! \defgroup  FiM_ErrorCodes  Error codes used by FIM
 * Error codes used by FIM
 * \{ */
#define FIM_E_OK                    E_OK /*!< No error */
#define FIM_E_NOT_OK                1u   /*!< Error    */
#if (FIM_E_OK == FIM_E_NOT_OK)
# error "Something's wrong with definition of E_OK!"
#endif
/*! \} */

/*! \defgroup  FiM_IncDecCodes  Internal codes for FID increment / decrement
 * Internal codes for FID increment / decrement
 * \{ */
#define FIM_FID_NOCHANGE            (0x00u) /*!< No change */
#define FIM_FID_INC                 (0x01u) /*!< Increment */
#define FIM_FID_DEC                 (0x02u) /*!< Decrement */
/*! \} */

/* status bits defined by DEM
DEM_UDS_STATUS_TF     0x01  bit 0: TestFailed 
DEM_UDS_STATUS_TFTOC  0x02  bit 1: TestFailedThisOperationCycle 
DEM_UDS_STATUS_PDTC   0x04  bit 2: PendingDTC 
DEM_UDS_STATUS_CDTC   0x08  bit 3: ConfirmedDTC 
DEM_UDS_STATUS_TNCSLC 0x10  bit 4: TestNotCompletedSinceLastClear 
DEM_UDS_STATUS_TFSLC  0x20  bit 5: TestFailedSinceLastClear 
DEM_UDS_STATUS_TNCTOC 0x40  bit 6: TestNotCompletedThisOperationCycle 
DEM_UDS_STATUS_WIR    0x80  bit 7: WarningIndicatorRequested          */

/*! * \defgroup  DefInhModTable Defines for Inhibition Mode Table
 * Defines for masks, patterns and whether result needs to be inverted that are used in calculation of effects of
 * event status changes.
 * \{ */
/*! \defgroup  InhCfgCodeLF Inhibition Config Code "Last Failed"
 * Inhibition configuration inhibits if test failed status bit is set. \n
 * Inhibits if: TF
 * \{ */
#define FIM_LAST_FAILED_MASK                  (Dem_EventStatusExtendedType)0x01u  /*!<DEM_UDS_STATUS_TF */
#define FIM_LAST_FAILED_PATTERN               (Dem_EventStatusExtendedType)0x01u  /*!<DEM_UDS_STATUS_TF set */
#define FIM_LAST_FAILED_INVERTRESULT          (boolean)FALSE                      /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTst Inhibition Config Code "Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set. \n
 * Inhibits if: /TNCTOC
 * \{ */
#define FIM_TESTED_MASK                       (Dem_EventStatusExtendedType)0x40u  /*!<DEM_UDS_STATUS_TNCTOC */
#define FIM_TESTED_PATTERN                    (Dem_EventStatusExtendedType)0x00u  /*!<DEM_UDS_STATUS_TNCTOC not set */
#define FIM_TESTED_INVERTRESULT               (boolean)FALSE                      /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTst Inhibition Config Code "Not Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set. \n
 * Inhibits if: TNCTOC
 * \{ */
#define FIM_NOT_TESTED_MASK                   (Dem_EventStatusExtendedType)0x40u  /*!<DEM_UDS_STATUS_TNCTOC */
#define FIM_NOT_TESTED_PATTERN                (Dem_EventStatusExtendedType)0x40u  /*!<DEM_UDS_STATUS_TNCTOC set */
#define FIM_NOT_TESTED_INVERTRESULT           (boolean)FALSE                      /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTstLF Inhibition Config Code "Tested AND Failed"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set AND LastFailed status
 * bit is set. \n
 * Inhibits if: /TNCTOC && TF
 * \{ */
#define FIM_TESTED_AND_FAILED_MASK            (Dem_EventStatusExtendedType)0x41u  /*!<DEM_UDS_STATUS_TNCTOC | DEM_UDS_STATUS_TF */
#define FIM_TESTED_AND_FAILED_PATTERN         (Dem_EventStatusExtendedType)0x01u  /*!<DEM_UDS_STATUS_TNCTOC not set, DEM_UDS_STATUS_TF set */
#define FIM_TESTED_AND_FAILED_INVERTRESULT    (boolean)FALSE                      /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTstOrLF Inhibition Config Code "Not Tested OR Failed" (Autosar extension)
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set OR LastFailed status
 * bit is NOT set. \n
 * Inhibits if: TNCTOC || TF -> /(/TNCTOC && /TF)
 * \{ */
#define FIM_NOT_TESTED_OR_FAILED_MASK         (Dem_EventStatusExtendedType)0x41u  /*!<DEM_UDS_STATUS_TNCTOC | DEM_UDS_STATUS_TF */
#define FIM_NOT_TESTED_OR_FAILED_PATTERN      (Dem_EventStatusExtendedType)0x00u  /*!<DEM_UDS_STATUS_TNCTOC not set, DEM_UDS_STATUS_TF not set */
#define FIM_NOT_TESTED_OR_FAILED_INVERTRESULT (boolean)TRUE                       /*!<invert result */
/*! \} */
/*! \} */

/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/

/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/*! Pointer to configuration data in case of PB loadable. */
P2CONST(FiM_ConfigType, FIM_VAR_NOINIT, FIM_INIT_DATA)              FiM_ConfigDataPtr;
#endif

/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/

/*! Struct that holds info for the calculation of a FID depending on the inhibition configuration. */
typedef struct FiM_InhibitionModeT
{
  Dem_EventStatusExtendedType Mask;
  Dem_EventStatusExtendedType Pattern;
  boolean                     InvertResult;
}FiM_InhibitionModeType;

/*! Typedef for access to event id table. */
typedef uint16 FiM_EventIdTableIndexType;

#define FIM_START_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization status of FiM. */
FIM_LOCAL VAR(uint8, FIM_VAR_INIT)                        FiM_InitializationStatus = FIM_UNINITIALIZED;

#define FIM_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FIM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*!Offset into event id table. Used if direct table acces is used, i.e. event ids are in continuously ascending order.*/
FIM_LOCAL VAR(FiM_EventIdTableIndexType, FIM_VAR_NOINIT)  FiM_SearchEventIdTableDirectOffset;

#define FIM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FIM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Pointer that holds the used function for searching an event id in EventIdTable. */
FIM_LOCAL FUNC(boolean, FIM_CODE) (*FiM_SearchEventIdTableFct)
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */);

#define FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
*  GLOBAL DATA
***********************************************************************************************************************/

#define FIM_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Inhibition Mode Table: used in calculation of effects of event status changes. */
FIM_LOCAL CONST(FiM_InhibitionModeType, FIM_CONST)
  FiM_InhibitionModeTable[FIM_INHCODE_NUMBER] =                                       /* PRQA S 3218 */ /* MD_FiM_8.7 */
{
  { /* invalid: pattern can never match "anded" mask */
    /* FIM_INHCODE_INVALID */
    (Dem_EventStatusExtendedType)0x00u,    
    (Dem_EventStatusExtendedType)0x01u,
    (boolean) FALSE
  },
  { /* FIM_LAST_FAILED */
    FIM_LAST_FAILED_MASK,
    FIM_LAST_FAILED_PATTERN,
    FIM_LAST_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED */
    FIM_NOT_TESTED_MASK,                    
    FIM_NOT_TESTED_PATTERN,
    FIM_NOT_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED */
    FIM_TESTED_MASK,                    
    FIM_TESTED_PATTERN,
    FIM_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED_AND_FAILED */
    FIM_TESTED_AND_FAILED_MASK,            
    FIM_TESTED_AND_FAILED_PATTERN,
    FIM_TESTED_AND_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED_OR_FAILED */
    FIM_NOT_TESTED_OR_FAILED_MASK,            
    FIM_NOT_TESTED_OR_FAILED_PATTERN,
    FIM_NOT_TESTED_OR_FAILED_INVERTRESULT
  }
};                   

#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to find a given event id in FiM_EventIdTable
 *                            using direct index access.
 *
 *  \details                  There is a direct relation between the event id and it's position in FiM_EventIdTable of
 *                            the kind: event id = position + offset
 *                            Algorithm is determined by FiM_SetSearchEventIdTableFct.
 *                            Offset is stored in FiM_SearchEventIdTableDirectOffset and was determined in
 *                            FiM_SetSearchEventIdTableFct.
 *  \param[in]  eventId       Event id to be searched in event id table.
 *                            May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx    Pointer to variable where index of found event is stored.
 *                            Must point to a variable of type FiM_EventIdTableIndexType.
 *  \return                   TRUE  event id was found
 *                            FALSE event id was not found
 *  \pre                      Event Ids in FiM_EventIdTable must be in continuously ascending order without gaps.
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */);

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to find a given event id in FiM_EventIdTable using linear search.
 *
 *  \details                  Just a dumb linear search starting at the beginning of FiM_EventIdTable until either end
 *                            is reached or event id is found.
 *                            Algorithm is determined by FiM_SetSearchEventIdTableFct.
 *  \param[in]  eventId       Event id to be searched in event id table.
 *                            May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx    Pointer to variable where index of found event is stored.
 *                            Must point to a variable of type FiM_EventIdTableIndexType.
 *  \return                   TRUE  event id was found
 *                            FALSE event id was not found
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */);

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableBinary
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to find a given event id in FiM_EventIdTable using binary search.
 *
 *  \details                  Just a standard half-interval search.
 *                            Algorithm is determined by FiM_SetSearchEventIdTableFct.
 *  \param[in]  eventId       Event id to be searched in event id table.
 *                            May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx    Pointer to variable where index of found event is stored.
 *                            Must point to a variable of type FiM_EventIdTableIndexType.
 *  \return                   TRUE  event id was found
 *                            FALSE event id was not found
 *  \pre                      Event Ids in FiM_EventIdTable must be in ascending order.
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */);

/***********************************************************************************************************************
 *  FiM_SetSearchEventIdTableFct
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to determine the search algorithm in FiM_EventIdTable.
 *
 *  \details                  This function is used at startup of FIM. It's result depends on the sequence of event ids.
 *                            This is not done at generation time since FiM_EventIdTable can be calibratable.
 *                            Possible search algorithms: Linear or binary search or direct table access.
 *  \pre                      -
 *  \context                  TASK
 *  \reentrant                FALSE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE)    FiM_SetSearchEventIdTableFct(void);

/***********************************************************************************************************************
 *  FiM_IncFidCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to increment FID permission counter.
 *                        
 *  \details                  This function increments the permission counter for the passed FID.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidCounter before
 *                              write access is done.
 *  \param[in]  fid           Function Id that permission counter should be incremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_IncFidCounter_Checked(FiM_FunctionIdType fid);

/***********************************************************************************************************************
 *  FiM_DecFidCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to decrement FID permission counter if counter is not already 0.
 *                        
 *  \details                  This function decrements the permission counter for the passed FID. It includes a prior
 *                            check for 0.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidCounter before
 *                              write access is done.
 *  \param[in]  fid           Function Id that permission counter should be decremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_DecFidCounter_Checked(FiM_FunctionIdType fid);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_IncFidPendingCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to increment FID pending counter.
 *                        
 *  \details                  This function increments the pending counter for the passed FID.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be incremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_IncFidPendingCounter_Checked(FiM_FunctionIdType fid);

/***********************************************************************************************************************
 *  FiM_DecFidPendingCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to decrement FID pending counter if counter is not already 0.
 *                        
 *  \details                  This function decrements the pending counter for the passed FID. It includes a prior
 *                            check for 0.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be decremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_DecFidPendingCounter_Checked(FiM_FunctionIdType fid);
#endif

/***********************************************************************************************************************
 *  FiM_DetermineInhibitionStatusChange()
 **********************************************************************************************************************/
/*!
 *  \brief                    Internal helper function to determine whether and how FID counter has to be changed.
 *  \details                  I.e. Inhibition contribution of one event / inhibition configuration to a FID.
 *                            Considers:
 *                              - one event status
 *                              - one inhibition code
 *  \param[in]  evStatusOld   old status (byte) of event
 *  \param[in]  evStatusNew   new status (byte) of event
 *  \param[in]  inhCode       code of inhibition configuration, i.e. index into FiM_InhibitionModeTable
 *  \return                   FIM_FID_NOCHANGE: FID shall not be changed due to status change
 *                            FIM_FID_INC:      FID shall be incremented due to status change
 *                            FIM_FID_DEC:      FID shall be decremented due to status change
 *  \pre                      see preconditions of FiM_GetInhibitionStatus
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE) FiM_DetermineInhibitionStatusChange(Dem_EventStatusExtendedType evStatusOld,
                                                                           Dem_EventStatusExtendedType evStatusNew,
                                                                           uint8 inhCode);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_DeterminePendingStatusChange()
 **********************************************************************************************************************/
/*!
 *  \brief                    Internal helper function to determine whether and how FID pending counter has to be
 *                            changed.
 *  \details                  I.e. Pending contribution of one event / inhibition configuration to a FID.
 *                            Considers:
 *                              - one event status
 *  \param[in]  evStatusOld   old status (byte) of event
 *  \param[in]  evStatusNew   new status (byte) of event
 *  \return                   FIM_FID_NOCHANGE: FID Pending shall not be changed due to status change
 *                            FIM_FID_INC:      FID Pending shall be incremented due to status change
 *                            FIM_FID_DEC:      FID Pending shall be decremented due to status change
 *  \pre                      see preconditions of FiM_GetInhibitionStatus
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD             == STD_ON
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE) FiM_DeterminePendingStatusChange(Dem_EventStatusExtendedType evStatusOld,
                                                                        Dem_EventStatusExtendedType evStatusNew);
#endif

/***********************************************************************************************************************
 *  FiM_CalculateFidStates()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to calculate the state of all FIDs.
 *                        
 *  \details                  This version can handle only:
 *                              - TriggerOnEvent with separate Inhibition Configuration Table
 *                                (optimized tables, non-calibratable).
 *  \pre                      -
 *  \context                  TASK
 *  \reentrant                FALSE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE) FiM_CalculateFidStates(void);

/***********************************************************************************************************************
 *  FiM_GetInhibitionStatus()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to obtain the inhibition status
 *                            for one event status / inhibition code.
 *
 *  \details                  Obtain the inhibition status for
 *                            - one event status
 *                            - one inhibition code
 *  \param[in]  evStatus      status byte of event
 *  \param[in]  inhCode       code of inhibition configuration, i.e. index into FiM_InhibitionModeTable;
 *  \return                   TRUE:  combination inhibits FID.
 *                            FALSE: combination does not inhibit FID.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_GetInhibitionStatus(Dem_EventStatusExtendedType evStatus,
                                                                 uint8 inhCode);

/***********************************************************************************************************************
 *  FiM_ModFidCounters()
 **********************************************************************************************************************/
/*!
 *  \brief                      Internal helper function to modify FID (pending) counters.
 *  \details                    Increment, decrement or leave the counters unchanged depending on passed modification
 *                              codes.
 *  \param[in]  fid             FID for which (pending) counters are modified
 *  \param[in]  modCode         code how to modify FID counter
 *  \param[in]  modCodePending  code how to modify FID Pending counter
 *                              possible codes:
 *                              FIM_FID_NOCHANGE: FID (Pending) shall not be changed due to status change
 *                              FIM_FID_INC:      FID (Pending) shall be incremented due to status change
 *                              FIM_FID_DEC:      FID (Pending) shall be decremented due to status change
 *  \pre                      -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_ModFidCounters(FiM_FunctionIdType fid, uint8 modCode, uint8 modCodePending);

#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*!
 *  Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId /* in */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx /* out */)
{
  boolean retVal;
  /* #10 if event id is within range limits */
  if ((eventId >= FiM_SearchEventIdTableDirectOffset)                             &&
      (eventId <  (FiM_GetCountOfEvents() + FiM_SearchEventIdTableDirectOffset)))
  {
    /* #20 event id found: subtract offset from event id, return this as index into FiM_EventIdTable */
    (*evTableIdx) = eventId - FiM_SearchEventIdTableDirectOffset;             /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
    /* #30 set return value to TRUE */
    retVal = TRUE;
  }
  /* #40 otherwise */
  else
  {
    /* #50 event id not found: set return value to FALSE */
    retVal = FALSE;
  }
  return retVal;
}

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */)
{
  /* #10 set return value to FALSE */
  boolean retVal = FALSE;
  /* #30 loop through all indexes in FiM_EventIdTable until end is reached or event id was found */
  for(*evTableIdx = 0;                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
      *evTableIdx < FiM_GetCountOfEvents();
      (*evTableIdx)++)                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
  {
    /* #40 check whether event id at current probe index matches */
    if (FiM_GetEventId_EventIdTable((*evTableIdx)) == eventId)
    {
      /* #50 it matches -> event id found; return this as index into FiM_EventIdTable and set return value to TRUE */
      retVal = TRUE;
      break;
    }
  }
  return retVal;
}

/***********************************************************************************************************************
 *  FiM_SearchEventIdTableBinary
 **********************************************************************************************************************/
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
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                           /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx /* out */)
{
  /* #05 set return value to FALSE */
  boolean retVal = FALSE;
  /* #10 init BOTTOM (loIdx) and TOP (hiIdx) boundaries to first and last element in FiM_EventIdTable */
  FiM_EventIdTableIndexType loIdx = 0;
  FiM_EventIdTableIndexType hiIdx = FiM_GetCountOfEvents() - 1;

  /* #20 loop while bottom index smaller than top index, element not found and search index is still within bounds */
  while (loIdx <= hiIdx)                                                       /* PRQA S 3355, 3357 */ /* MD_FiM_13.7 */
  {
    boolean         notAvail = FALSE;
    Dem_EventIdType probeEventId;

    /* #30 calculate the probe reference (in the middle of loIdx and hiIdx) */
    (*evTableIdx) = (FiM_EventIdTableIndexType)((hiIdx + loIdx)>>1);          /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */

    /* #40 if event id found */
    probeEventId = FiM_GetEventId_EventIdTable((*evTableIdx));
    if (probeEventId == eventId)
    {
      /* found a matching element */
      /* #50 set return value to TRUE (found) */
      retVal = TRUE;
    }
    /* #60 otherwise */
    else
    {
      /* not found a matching element */
      /* #70 determine whether lower or higher half shall be taken as interval for next try */
      /* #80 if event id found is higher than the id we're looking for */
      if (probeEventId > eventId)
      {
        /* id we're looking for is lower than the current reference */
        /* #90 Use lower half of interval for next loop, i.e. set new top index to current probe index  */
        /* #100 Make sure new index stays within array bounds, remember if it wants to get beyond       */
        if((*evTableIdx) > 0)
        {
          hiIdx = (FiM_EventIdTableIndexType)((*evTableIdx) - 1);                    /* PRQA S 0291 */ /* MD_FiM_0291 */
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
      /* #110 otherwise */
      else
      {
        /* id we're looking for is greater than the current reference */
        /* #120 Use upper half of interval for next loop, i.e. set new bottom index to current probe index */
        /* #130 Make sure new index stays within array bounds, remember if it wants to get beyond          */
        if((*evTableIdx) < (FiM_GetCountOfEvents() - 1))                       /* PRQA S 3356, 3359 */ /* MD_FiM_13.7 */
        {                                                                            /* PRQA S 3201 */ /* MD_FiM_13.7 */
          loIdx = (FiM_EventIdTableIndexType)((*evTableIdx) + 1); 
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
    }
    /* #140 break loop if element was found or index went beyond array bounds */
    /* #150 check whether bottom index is still lower than top index is done in loop condition */
    if ((retVal == TRUE) || (notAvail == TRUE))
    {
      break;
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  FiM_SetSearchEventIdTableFct
 **********************************************************************************************************************/
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
 */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_SetSearchEventIdTableFct(void)
{
  FiM_EventIdTableIndexType evTableIdx;
  Dem_EventIdType           prevEventId;  /* previous event id */
  Dem_EventIdType           currEventId;  /* current event id  */

  FiM_SearchEventIdTableFct           = FiM_SearchEventIdTableDirect;
  prevEventId                         = FiM_GetEventId_EventIdTable(0);
  FiM_SearchEventIdTableDirectOffset  = prevEventId;

  for (evTableIdx = 1; evTableIdx < FiM_GetCountOfEvents(); evTableIdx++)      /* PRQA S 3356, 3359 */ /* MD_FiM_13.7 */
  {                                                                                  /* PRQA S 3201 */ /* MD_FiM_13.7 */
    currEventId = FiM_GetEventId_EventIdTable(evTableIdx);
    /* are we still hoping for direct access? if so, check whether next id is still continuous */
    if ((FiM_SearchEventIdTableFct == FiM_SearchEventIdTableDirect) &&
        (currEventId != (prevEventId + 1)))
    {
      /* not continuous anymore -> switch to binary search if list is not too small */
      /*lint -save -e506 */
      if (FiM_GetCountOfEvents() >= FIM_BINSEARCH_MINLISTLENGTH)                    /* PRQA S 3325  */ /* MD_FiM_14.1 */
      /*lint -restore */
      {                                                                             /* PRQA S 3201  */ /* MD_FiM_14.1 */
        FiM_SearchEventIdTableFct = FiM_SearchEventIdTableBinary;
      }
      else
      {                                                                             /* PRQA S 3201  */ /* MD_FiM_14.1 */
        /* use linear algorithm for small lists (if direct access cannot be used) */
        FiM_SearchEventIdTableFct = FiM_SearchEventIdTableLinear;
      }
    }
    /* are we hoping for binary algorithm, i.e. at least ordered event ids?                       */
    /* no check if (FiM_SearchEventIdTableFct == FiM_SearchEventIdTableBinary) necessary because: */
    /* - if we're still in direct access mode (current event id <= previous event id)             */
    /*   always returns FALSE                                                                     */
    /* - if we're already in linear search mode it won't do any harm; we may or may not enter the */
    /*   block but afterwards it will still be linear mode and we will stop                       */
    /* Note: the next two decisions should only be true if event ids were re-assigned after       */
    /*       FIM generation                                                                       */
    if (currEventId <= prevEventId)
    {
      /* not ordered anymore -> switch to linear search and stop */
      FiM_SearchEventIdTableFct = FiM_SearchEventIdTableLinear;
    }
    prevEventId = currEventId;
    /* are we down to linear search? */
    if (FiM_SearchEventIdTableFct == FiM_SearchEventIdTableLinear)
    {
      /* we're down to linear search -> stop the search for the optimal algorithm - it won't get any better */
      break;
    }
  }
}

/***********************************************************************************************************************
 *  FiM_IncFidCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_IncFidCounter_Checked(FiM_FunctionIdType fid)
{
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid < FiM_GetSizeOfFidCounter())
#endif
  {
    FiM_IncFidCounter(fid);                                                     /* SBSW_FIM_ACCESS_FIDCOUNTER_CHECKED */
  }
}

/***********************************************************************************************************************
 *  FiM_DecFidCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_DecFidCounter_Checked(FiM_FunctionIdType fid)
{
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid < FiM_GetSizeOfFidCounter())
#endif
  {
    if (FiM_GetFidCounter(fid) > 0)
    {
      FiM_DecFidCounter(fid);                                                   /* SBSW_FIM_ACCESS_FIDCOUNTER_CHECKED */
    }
  }
}

# if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_IncFidPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_IncFidPendingCounter_Checked(FiM_FunctionIdType fid)
{
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid < FiM_GetSizeOfFidPendingCounter())
#endif
  {
    FiM_IncFidPendingCounter(fid);                                       /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
  }
}

/***********************************************************************************************************************
 *  FiM_DecFidPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_DecFidPendingCounter_Checked(FiM_FunctionIdType fid)
{
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid < FiM_GetSizeOfFidPendingCounter())
#endif
  {
    if (FiM_GetFidPendingCounter(fid) > 0)
    {
      FiM_DecFidPendingCounter(fid);                                     /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
    }
  }
}
# endif

/***********************************************************************************************************************
 *  FiM_GetInhibitionStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_GetInhibitionStatus(Dem_EventStatusExtendedType evStatus,
                                                                 uint8 inhCode)
{
  /* #10 mask the passed evStatus with the mask that's relevant for the passed inhibition code                     */
  /* #20 this yields the bits that are relevant for the passed inhibition code                                     */
  /* #30 if these bits match the bit pattern relevant for the passed inhibition code then combination inhibits FID */
  boolean
    result = (boolean)((evStatus & FiM_InhibitionModeTable[inhCode].Mask) == FiM_InhibitionModeTable[inhCode].Pattern);
  /* #40 check whether inhibition code requests an inversion of the result                                         */
  if (FiM_InhibitionModeTable[inhCode].InvertResult == TRUE)
  {
    result = (boolean)(!result);
  }
  return result;
}

/***********************************************************************************************************************
 *  FiM_DetermineInhibitionStatusChange()
 **********************************************************************************************************************/
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
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE) FiM_DetermineInhibitionStatusChange(Dem_EventStatusExtendedType evStatusOld,
                                                                           Dem_EventStatusExtendedType evStatusNew,
                                                                           uint8 inhCode)
{
  /* #10 set default to no change */
  uint8 retVal = FIM_FID_NOCHANGE;
  /* #20 if the former Eventstatus was inhibit */
  if (FiM_GetInhibitionStatus(evStatusOld, inhCode) == TRUE)
  {
    /* #30 if the new Eventstatus is not inhibit anymore */
    if (FiM_GetInhibitionStatus(evStatusNew, inhCode) == FALSE)
    {
      /* #40 status change leads to FID counter decrement */
      retVal = FIM_FID_DEC;
    }
  }
  /* #50 otherwise the former Eventstatus was not inhibit */
  else
  {
    /* #60 if it is now inhibit */
    if (FiM_GetInhibitionStatus(evStatusNew, inhCode) == TRUE)
    {
      /* #70 status change leads to FID counter increment */
      retVal = FIM_FID_INC;
    }
  }
  return retVal;
}

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_DeterminePendingStatusChange()
 **********************************************************************************************************************/
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
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE) FiM_DeterminePendingStatusChange(Dem_EventStatusExtendedType evStatusOld,
                                                                        Dem_EventStatusExtendedType evStatusNew)
{
  /* #10 set default to no change */
  uint8 retVal = FIM_FID_NOCHANGE;
  /* #20 if the former Eventstatus was pending */
  if((evStatusOld & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
  {
    /* #30 if the new Eventstatus is not pending anymore */
    if((evStatusNew & DEM_UDS_STATUS_PDTC) != DEM_UDS_STATUS_PDTC)
    {
      /* #40 status change leads to FID pending counter decrement */
      retVal = FIM_FID_DEC;
    }
  }
  /* #50 otherwise the former Eventstatus was not pending */
  else
  {
    /* #60 if it is now pending */
    if((evStatusNew & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
    {
      /* #70 status change leads to FID pending counter increment */
      retVal = FIM_FID_INC;
    }
  }
  return retVal;
}
#endif

/***********************************************************************************************************************
 *  FiM_ModFidCounters()
 **********************************************************************************************************************/
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
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_ModFidCounters(FiM_FunctionIdType fid, uint8 modCode, uint8 modCodePending)
{
#if (FIM_FEATURE_OBD == STD_OFF)
  FIM_DUMMY_STATEMENT(modCodePending);                       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  {
    /* enter critical section to protect each increment/decrement against interruption; */
    /* only one enter/leave over the whole if/else to reduce number of enter/leaves     */
    FiM_EnterCritical();                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (modCode == FIM_FID_DEC)
    {
      /* decrement FID counter is necessary                                      */
      FiM_DecFidCounter_Checked(fid);
    }
    /* no "else" for code coverage in non-OBD case */
    if (modCode == FIM_FID_INC)
    {
      /* increment FID counter is necessary                                      */
      FiM_IncFidCounter_Checked(fid);
    }
#if (FIM_FEATURE_OBD == STD_ON)
    /* if OBD is configured */
    if (modCodePending == FIM_FID_DEC)
    {
      /* decrement FID Pending counter is necessary                              */
      FiM_DecFidPendingCounter_Checked(fid);
    }
    if (modCodePending == FIM_FID_INC)
    {
      /* increment FID Pending counter is necessary                              */
      FiM_IncFidPendingCounter_Checked(fid);
    }
#endif
    /* leave critical section */
    FiM_LeaveCritical();                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  return;
}

 /**********************************************************************************************************************
  *  FiM_CalculateFidStates()
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
 *
  */
FIM_LOCAL FUNC(void, FIM_CODE)  FiM_CalculateFidStates(void)
{
  Dem_EventStatusExtendedType   EventStatus;
  FiM_FunctionIdType            fidIter;
  FiM_EventIdTableIndexType     evTableIdx;
  uint16                        inhCodeIdx, inhCodeTableIdx_Start, inhCodeTableIdx_Stop;
  /* #10 reset all permission (and pending) counters to 0 */
  FiM_EnterCritical();                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */
  for(fidIter = 0; fidIter < FiM_GetSizeOfFidCounter(); fidIter++)
  {
    FiM_SetFidCounter(fidIter, 0);                                        /* SBSW_FIM_ACCESS_FIDCOUNTER_CALCFIDSTATES */
  }
  FiM_LeaveCritical();                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */
#if (FIM_FEATURE_OBD == STD_ON)
  FiM_EnterCritical();                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */
  for(fidIter = 0; fidIter < FiM_GetSizeOfFidPendingCounter(); fidIter++)
  {
    FiM_SetFidPendingCounter(fidIter, 0);                          /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCFIDSTATES */
  }
  FiM_LeaveCritical();                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif

  /* #20 loop through all events in FiM_EventIdTable starting at the first one until the end of the table is reached */
    for(evTableIdx = 0; evTableIdx < FiM_GetCountOfEvents(); evTableIdx++)
  {
    /* event id table is always generated, no manual calibration here -> it does not contain invalid event ids */
    /* #50 get event status from DEM for current event id */
    Std_ReturnType retValue =
      Dem_GetEventStatus(FiM_GetEventId_EventIdTable(evTableIdx), &EventStatus);   /* SBSW_FIM_DEMGETEVENTSTATUS_CALL */
    /* #60 if event status could be obtained (Dem_GetEventStatus returned E_OK) */
    if(retValue == E_OK)
    {
      /* #70 identify the parts of the complete inhibition config tables that are relevant for this event */
      /* #80 do so by identifying start index and stop index (last relevant element plus 1) in the complete array  */
      inhCodeTableIdx_Start = FiM_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx);
      inhCodeTableIdx_Stop  = FiM_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx+1);
      /* #90 loop through all inhibition configurations that are connected to the EventID */
      for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++)
      {
        /* inhibition configuration table is always generated, no manual calibration here                             */
        /* -> it does not contain invalid inhibition configurations                                                   */
        /* even if it contained an invalid event id, the first element of FiM_InhibitionModeTable would neutralise it */
        uint8  modCode        = FIM_FID_NOCHANGE;
        uint8  modCodePending = FIM_FID_NOCHANGE;
        /* #100 determine whether FID counter has to be incremented due to this particular inhibition cfg, i.e. if the inhibit condition is fulfilled for this combination of FIDs and EventID */
        if (FiM_GetInhibitionStatus(EventStatus,
                                    FiM_GetInhCodeTable(inhCodeIdx)) == TRUE)
        {
          modCode = FIM_FID_INC;
        }
#if (FIM_FEATURE_OBD == STD_ON)
        /* #110 determine whether FID pending counter has to be incremented (does not depend on inhibition cfg, just linked to dummy inhibition cfg) */
        if((EventStatus & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
        {
           modCodePending = FIM_FID_INC;
        }
#endif
        /* #120 check if an increment of FID or FID Pending (in case of OBD) counters is necessary */
        if ((modCode != FIM_FID_NOCHANGE)
# if (FIM_FEATURE_OBD == STD_ON)
            || (modCodePending != FIM_FID_NOCHANGE)
#endif
           )
        {
          /* #130 if yes, at least for one of them it is */
          uint16  evToFidTableIter;
          /* #140 identify the part that of the complete FID tables that's relevant for this event and inhibition cfg */
          /* #150 do so by identifying start index and stop index (last relevant element + 1) in the complete array   */
          uint16  evToFidTableIdx_Start = FiM_GetInhFidIdxTable(inhCodeIdx);
          uint16  evToFidTableIdx_Stop  = FiM_GetInhFidIdxTable(inhCodeIdx+1);

          /* #160 loop through all FIDs that are connected to the EventID / Inhibition Configuration */
          for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)
          {
              /* #170 modify FID (Pending) counters according to the needs identified above */
              FiM_ModFidCounters(FiM_GetFid_EventToFidTable(evToFidTableIter), modCode, modCodePending);
          }
        }
      } /* for(inhCodeIdx = 0; inhCodeIdx < nrInhCodes; inhCodeIdx++) */
    } /* if(retValue == E_OK) */
    /* #210 otherwise (Dem_GetEventStatus did not return E_OK) */
    else
    {
      /* #220 Dem_GetEventStatus returned an error: ignore event for inhibition calculation */          /* SPEC-63513 */
    } /* else if(returnValue == E_OK) */
  } /* for(...; evTableIdx < FiM_GetCountOfEvents(); ...) */
  return;
}  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FiM_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FIM_CODE) FiM_InitMemory(void)
{ 
  FiM_InitializationStatus = FIM_UNINITIALIZED;
}

/***********************************************************************************************************************
 *  FiM_Init
 **********************************************************************************************************************/
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
*/
FUNC(void, FIM_CODE) FiM_Init(P2CONST(FiM_ConfigType, AUTOMATIC, FIM_INIT_DATA) FiMConfigPtr) /*lint -e{438} */ /*lint -e{550} */
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* #10 if PB is enabled: do some checks on passed PB config */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
  uint8 errorId = FIM_E_INITIALIZATION_NOT_COMPLETED;
  /* #20 if FiMConfigPtr is NULL_PTR */
  if (FiMConfigPtr == NULL_PTR)
  {
    /* #25 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_NULLPTR);
  }
  /* #40 otherwise if there is a mismatch in the magic numbers of th PB config and the code */
  else if (FiMConfigPtr->FinalMagicNumberOfPBConfig != FIM_FINAL_MAGIC_NUMBER)
  {
    /* #45 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  /* #50 otherwise if there is a mismatch in compatibility versions of the generators used for PB config and code */
  else if (FiMConfigPtr->GeneratorCompatibilityVersionOfPBConfig != FIM_CFG_GEN_COMPATIBILITY_VERSION)
  {
    /* #55 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #60 otherwise PB config is fine */
  else
  {
    /* #70 use PB config pointer */
    FiM_ConfigDataPtr = FiMConfigPtr;
    errorId           = FIM_E_NO_ERROR;
#else
  {
    FIM_DUMMY_STATEMENT(FiMConfigPtr);                                               /* PRQA S 3112 */ /* MD_FiM_14.2 */
#endif

    /* #100 determine search algorithm for event id list */
    FiM_SetSearchEventIdTableFct();

    /* #160 initially calculate FID states */
    FiM_CalculateFidStates();
    /* #250 set component state to "initialized" */
    FiM_InitializationStatus  = FIM_INITIALIZED;
  }

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
# if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #300 if error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_INIT, errorId);
  }
# else
  FIM_DUMMY_STATEMENT(errorId);                              /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#endif
}

/***********************************************************************************************************************
 *  FiM_DemInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
*/
FUNC(void, FIM_CODE) FiM_DemInit(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 check whether FIM is initialized */
  if(FiM_InitializationStatus != FIM_INITIALIZED)
  {
    errorId = FIM_E_INITIALIZATION_NOT_COMPLETED;
  }
  else
#endif
  {
    /* #20 re-calculate FID states if it is initialized */
    FiM_CalculateFidStates();
  }
#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #30 otherwise if error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_DEMINIT, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                              /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

#if (FIM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  FiM_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)  FiM_GetVersionInfo
                       (P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo)

{
  uint8 errorId = FIM_E_NO_ERROR;
  
#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  if (versioninfo == NULL_PTR)
  {
    
    errorId = FIM_E_INVALID_POINTER;
  }
  else
#endif
  {
    versioninfo->vendorID         = FIM_VENDOR_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->moduleID         = FIM_MODULE_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_major_version = FIM_SW_MAJOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_minor_version = FIM_SW_MINOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_patch_version = FIM_SW_PATCH_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
  }
#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_GETVERSIONINFO, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                              /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}
#endif /* (FIM_VERSION_INFO_API == STD_ON) */

/***********************************************************************************************************************
 *  FiM_GetFunctionPermission
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPermission
                                 (FiM_FunctionIdType FID,                            /* PRQA S 1330 */ /* MD_FiM_16.4 */
                                  P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;
  Std_ReturnType    retCode = E_NOT_OK;

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #010 if out parameter Permission is NULL_PTR */
  if (Permission == NULL_PTR)
  {
    errorId = FIM_E_INVALID_POINTER;
  }
  /* #020 otherwise if FIM is not initialized (SPEC-31915) */
  else if(FiM_InitializationStatus != FIM_INITIALIZED)
  {
    errorId = FIM_E_WRONG_PERMISSION_REQ;
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  /* #030 otherwise if FID is out of range (SPEC-31912) */
  else if(FID >= FiM_GetSizeOfFidCounter())
  {
    errorId = FIM_E_FID_OUT_OF_RANGE;
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  else
#endif
  /* #040 otherwise return permission status of FID (SPEC-31914) */
  {
    /* add FiM_EnterCritical() if FiM_FidCounterType cannot be accessed in an atomic way,  */
    /* e.g., FiM_FidCounterType becomes 32 bits while 16 bit platforms are still supported */
    if(FiM_GetFidCounter(FID) == 0)
    {
      *Permission = TRUE;                                                                  /* SBSW_FIM_PERMISSION_PTR */
    }
    else
    {
      *Permission = FALSE;                                                                 /* SBSW_FIM_PERMISSION_PTR */
    }
    /* FiM_LeaveCritical() see above comment for FiM_EnterCritical() */
    retCode = E_OK;
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_GETFUNCTIONPERMISSION, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                              /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retCode;
}

#if (FIM_FEATURE_OBD == STD_ON)
/******************************************************************************
    FiM_GetFunctionPendingStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPendingStatus
                                 (FiM_FunctionIdType FID, 
                                  P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) pendingStatus
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;
  Std_ReturnType    retCode = E_NOT_OK;

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 if out parameter pendingStatus is NULL_PTR */
  if (pendingStatus == NULL_PTR)
  {
    errorId = FIM_E_INVALID_POINTER;
  }
  /* #20 otherwise if FIM is not initialized */
  else if(FiM_InitializationStatus != FIM_INITIALIZED)
  {
    errorId = FIM_E_WRONG_PERMISSION_REQ;
  }
  /* #30 otherwise if FID is out of range */
  else if(FID >= FiM_GetSizeOfFidPendingCounter())
  {
    errorId = FIM_E_FID_OUT_OF_RANGE;
  }
  else
#endif
  /* #40 otherwise return pending status of FID */
  {
    /* add FiM_EnterCritical() if FiM_FidCounterType cannot be accessed in an atomic way,  */
    /* e.g., FiM_FidCounterType becomes 32 bits while 16 bit platforms are still supported */
    if(FiM_GetFidPendingCounter(FID) != 0)
    {
      *pendingStatus = TRUE;                                                            /* SBSW_FIM_PENDINGSTATUS_PTR */
    }
    else
    {
      *pendingStatus = FALSE;                                                           /* SBSW_FIM_PENDINGSTATUS_PTR */
    }
    /* FiM_LeaveCritical() see above comment for FiM_EnterCritical() */
    retCode = E_OK;
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,
                          FIM_SID_FIM_GETFUNCTIONPENDINGSTATUS, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                              /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retCode;
}
#endif

/***********************************************************************************************************************
 *  FiM_DemTriggerOnEventStatus
 **********************************************************************************************************************/
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
 *
 *
 *
 *
*/
FUNC(void, FIM_CODE)  FiM_DemTriggerOnEventStatus
                       (Dem_EventIdType EventId,
                        Dem_EventStatusExtendedType EventStatusOld,
                        Dem_EventStatusExtendedType EventStatusNew
                       )                                                                               /*lint -e{529} */
{
  uint8 errorId = FIM_E_NO_ERROR;
  
#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 if FIM is not initialized skip rest of function */
  /* SlientBSW: check is silence relevant */
  if (FiM_InitializationStatus != FIM_INITIALIZED)
  {
    errorId = FIM_E_WRONG_TRIGGER_ON_EVENT;
  }
  /* #20 otherwise */
  else
#endif
  {
    FiM_EventIdTableIndexType     evTableIdx;
    uint16                        inhCodeIdx, inhCodeTableIdx_Start, inhCodeTableIdx_Stop;

    /* #30 if passed event id is not FIM_DEM_EVENT_INVALID */
    if (EventId != FIM_DEM_EVENT_INVALID)
    {
      /* #40 search for event (i.e. is event known/configured in FIM?) */
      /* Silent:
          SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR:
                    - pointer to local stack variale of type FiM_EventIdTableIndexType
          SBSW_FIM_SEARCHIDFUNC_PTR:
                    - initialization status is checked above
                    - in FiM_Init function pointer is always set */
      boolean evFound = (*FiM_SearchEventIdTableFct)                                     /* SBSW_FIM_SEARCHIDFUNC_PTR */
                         (EventId, &evTableIdx);
      /* #50 if event was found */
      if (evFound == TRUE)
      {
        /* #60 identify the parts of the complete inhibition config tables that are relevant for this event          */
        /* #70 do so by identifying start index and stop index (last relevant element plus 1) in the complete array  */
        inhCodeTableIdx_Start = FiM_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx);
        inhCodeTableIdx_Stop  = FiM_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx+1);
        /* #80 loop through all inhibition configurations that are connected to the EventID */
        for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++)
        {
          /* inhibition configuration table is always generated, no manual calibration here                           */
          /* -> it does not contain invalid inhibition configurations                                                 */
          /* even if it contained an invalid event id, first element of FiM_InhibitionModeTable would neutralise it   */
          /* #90 determine whether and how FID counter has to be changed due to this particular inhibition cfg        */
          uint8  modCode = FiM_DetermineInhibitionStatusChange
                            (EventStatusOld, EventStatusNew,
                             FiM_GetInhCodeTable(inhCodeIdx));
#if (FIM_FEATURE_OBD == STD_ON)
          /* #95 determine whether and how FID pending counter has to be changed (does not depend on inhibition cfg, just linked to dummy inhibition cfg) */
          uint8  modCodePending = FiM_DeterminePendingStatusChange
                                   (EventStatusOld, EventStatusNew);
#else
          uint8  modCodePending = FIM_FID_NOCHANGE;
#endif
          /* #100 check if a manipulation of FID or FID Pending (in case of OBD) counters is necessary */
          if ((modCode != FIM_FID_NOCHANGE)
#if (FIM_FEATURE_OBD == STD_ON)
              || (modCodePending != FIM_FID_NOCHANGE)
#endif
             )
          {
            /* #110 if yes */
            uint16  evToFidTableIter;
            /* #120 identify the part of the complete FID tables that is relevant for this event and inhibition cfg   */
            /* #130 do so by identifying start index and stop index (last relevant element + 1) in the complete array */
            /* #140 loop through relevant FIDs and manipulate their counters */
            uint16  evToFidTableIdx_Start = FiM_GetInhFidIdxTable(inhCodeIdx);
            uint16  evToFidTableIdx_Stop  = FiM_GetInhFidIdxTable(inhCodeIdx+1);
            for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)
            {
              /* #160 modify FID (Pending) counters according to the needs identified above */
              FiM_ModFidCounters(FiM_GetFid_EventToFidTable(evToFidTableIter), modCode, modCodePending);
            }
          }
        } /* for(inhCfgIdx = 0; inhCfgIdx < nrInhCfgs; inhCfgIdx++) */
      } /* if (evFound == TRUE) */
    } /* if (EventId != FIM_DEM_EVENT_INVALID) */
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,
                          FIM_SID_FIM_DEMTRIGGERONEVENTSTATUS, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FiM_MainFunction()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FIM_CODE) FiM_MainFunction(void)
{
}

#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
   MD_FiM_8.7:
     Description: Rule 8.7
                  Objects shall be defined at block scope if they are only accessed from within a single function.
     Reason:      Vector style guide prevents usage of static variables in function scope
     Risk:        None.
     Prevention:  None.

   MD_FiM_13.7:
     Description: Rule 13.7: Boolean operations whose results are invariant shall not be permitted.
     Reason:      This statement is needed only in some configurations.
     Risk:        Small code overhead.
     Prevention:  Will usually be optimized by compiler.

   MD_FiM_14.1:
     Description: Rule 14.1: There shall be no unreachable code.
     Reason:      This code is only unreachable in certain configurations. Usually compilers will optimize this.
     Risk:        Functions or global variables which are superfluous in a specific configuration may remain undetected
                  which leads to a small resource overhead.
     Prevention:  None.

   MD_FiM_14.2:
     Description: Rule 14.2: All non-null statements shall either (i) have at least one side-effect however executed,
                  or (ii) cause control flow to change.
     Reason:      This statement is used to avoid warnings caused by unused parameters.
     Risk:        None.
     Prevention:  None.

   MD_FiM_16.4:
     Description: Rule 16.4
                  The identifiers used in the declaration and definition of a function shall be identical.
     Reason:      Conflicting prototype is generated by RTE. Currently there is no way to tell RTE the parameter name.
     Risk:        None.
     Prevention:  None.

   MD_FiM_16.4:
     Description: Rule 16.7
                  The object addressed by the pointer parameter '%s' is not modified and so the pointer could be of type
                  'pointer to const'. 
     Reason:      Function is accessed via function pointer. Other variants of this function modify the object and
                  since the signature needs to be the same for all these functions it cannot be const.
     Risk:        None.
     Prevention:  None.

   MD_FiM_0291:
     Description: Rule 0291
                  An integer expression with a value that is apparently negative is being converted to an unsigned type.
     Reason:      Previous check (scanIdx > loLimit) together with the fact that loLimit is unsigned guarantees that
                  scanIdx is at least 1.
     Risk:        None.
     Prevention:  None.
*/

/* COV_JUSTIFICATION_BEGIN
\ID COV_FIM_DET_DETECT
  \REASON For SafeBSW Error Detection is always switched on.

\ID COV_FIM_RUNTIME_CHECKS
  \ACCEPT TX
  \REASON For SafeBSW Runtime Checks are always switched on.

COV_JUSTIFICATION_END */ 

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR
    \DESCRIPTION    Pointer write access to out parameter evTableIdx.
    \COUNTERMEASURE \N Search functions are internal and pointer passed by caller always points to a local (stack)
                       variable. See also SBSW_FIM_SEARCHIDFUNC_PTR.

\ID SBSW_FIM_SEARCHIDFUNC_PTR
    \DESCRIPTION Call of function via function pointer.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON.
                       Then function checks whether initialization has been performed.
                       FiM_Init ensures that function pointer is always set to a valid function if initialization
                       has been performed.

\ID SBSW_FIM_ACCESS_FIDCOUNTER_CHECKED
    \DESCRIPTION    Access of FimFidCounter using ComStackLib.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED
    \DESCRIPTION    Access of FimFidPendingCounter using ComStackLib.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]

\ID SBSW_FIM_ACCESS_FIDCOUNTER_CALCFIDSTATES
    \DESCRIPTION    Access of FimFidCounter using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCFIDSTATES
    \DESCRIPTION    Access of FimFidPendingCounter using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_VERSIONINFO_PTR
    \DESCRIPTION Pointer write access to versioninfo pointer variable in FiM_GetVersionInfo().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to the parameter
                       versioninfo is passed.

\ID SBSW_FIM_PERMISSION_PTR
    \DESCRIPTION Pointer write access to permission pointer variable in FiM_GetFunctionPermission().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

\ID SBSW_FIM_PENDINGSTATUS_PTR
    \DESCRIPTION Pointer write access to pending status pointer variable in FiM_GetFunctionPendingStatus().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

\ID SBSW_FIM_DEMGETEVENTSTATUS_CALL
    \DESCRIPTION Call of Dem_GetEventStatus with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of type Dem_EventStatusExtendedType is passed.

SBSW_JUSTIFICATION_END */

/*

\CM CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS
       Verify that FIM_DEV_RUNTIME_CHECKS == STD_ON.
       Then it's Qualified use case CSL01 of ComStackLib.

*/

/***********************************************************************************************************************
*  END OF FILE: FiM.c
***********************************************************************************************************************/
