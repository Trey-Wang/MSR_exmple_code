/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dbg.c
 *      Project:  Debugging
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implements partly the functionality of the AUTOSAR 4.x module DBG based on XCP.
 *
 *
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Oliver Reineke                 visore       Vector Informatik GmbH
 *  David Zentner                  viszda       Vector Informatik GmbH
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-10-04  visore  ESCAN00050129   Creation
 *  01.00.01  2015-12-16  viszda  ESCAN00087182   Fix violation of MISRA rule 16.10
 *                        viszda  ESCAN00087207   Wrong module id of DBG
 *********************************************************************************************************************/

#define DBG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dbg.h"

#include "SchM_Dbg.h"

#if(DBG_DEV_ERROR_DETECT == STD_ON)
# include "Det.h" /* Include Development Error Tracer */
#endif

#if(DBG_USE_COMLAYER_XCP == STD_ON)
# include "Xcp.h"
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
/* SWS_BSW_00036 */
#if((DBG_SW_MAJOR_VERSION != (0x01u)) || (DBG_SW_MINOR_VERSION != (0x00u)) || (DBG_SW_PATCH_VERSION != (0x01u)))
# error "Vendor specific version numbers of Dbg.c and Dbg.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if(DBG_USE_COMLAYER_XCP == STD_ON) && !defined (Dbg_TransmitData)
/* PRQA S 3453 4 */ /*  MD_MSR_19.7 */
# define Dbg_TransmitData(event)                Xcp_Event(event)
#endif

/***********************************************************************************************************************/
/* General definition of checking macros */
/* PRQA S 3458 16 */ /* MD_MSR_19.4 */
# define Dbg_CheckErrorReturnVoid(cond, api, err)        \
{ if(!(cond)){                                           \
    Dbg_CallDetReportError((api), (err));                \
    return;                                              \
}}

# define Dbg_CheckErrorReturnValue(cond, api, err, ret)  \
{ if(!(cond)){                                           \
    Dbg_CallDetReportError((api), (err));                \
    return(ret);                                         \
}}

# define Dbg_CheckErrorContinue(cond, api, err)          \
{ if(!(cond)){                                           \
    Dbg_CallDetReportError((api), (err));                \
}}

/***********************************************************************************************************************/
/* Macros for DET abstraction */

/* Dbg006 */
/* PRQA S 3453 10 */ /*  MD_MSR_19.7 */
#if(DBG_DEV_ERROR_DETECT == STD_ON)
# define Dbg_CheckDetErrorReturnVoid(cond, api, err)       Dbg_CheckErrorReturnVoid((cond), (api), (err))
# define Dbg_CheckDetErrorReturnValue(cond, api, err, ret) Dbg_CheckErrorReturnValue((cond), (api), (err), (ret))
# define Dbg_CheckDetErrorContinue(cond, api, err)         Dbg_CheckErrorContinue((cond), (api), (err))
#if DBG_DEV_ERROR_REPORT == STD_ON
# define Dbg_CallDetReportError(api, err)                  (void)Det_ReportError(DBG_MODULE_ID, 0, (api), (err))
#else
# define Dbg_CallDetReportError(api, err)
#endif
#else
# define Dbg_CheckDetErrorReturnVoid(cond, api, err)
# define Dbg_CheckDetErrorReturnValue(cond, api, err, ret) 
# define Dbg_CheckDetErrorContinue(cond, api, err) 
# define Dbg_CallDetReportError(api, err) 
#endif

/* PRQA S 3453 7 */ /* MD_MSR_19.7 */
# define DBG_BEGIN_CRITICAL_SECTION_0() SchM_Enter_Dbg_DBG_EXCLUSIVE_AREA_0()
# define DBG_END_CRITICAL_SECTION_0()   SchM_Exit_Dbg_DBG_EXCLUSIVE_AREA_0()




/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define DBG_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*********************************************************************************************************************/
/*! \brief  This service initializes the DBG module. It shall initialize the internal transfer protocol of the debugging 
*           module and call Dbg_ComInit for initialization of communication. The initialization of the internal buffer 
*           and all internal variables needed to manage the buffer and shall be done by the standard C-initialization. 
*           Excluding these data items from the standard C-initialization allows for post mortem data analysis. 
*
*           In order to be able to save timestamps together with the collected data, the DBG module shall be initialized 
*           after the Operating System. 
*
*           The alarm needed for cyclic data collection shall be activated at initialization of the DBG module.
*   \param  -
*   \pre    -
*   \post   -
*   \return -
**********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_Init(void)
{ /* [SWS_Dbg_00138] */
	Dbg_EnableDidCollection(TRUE);
#if defined (DBG_TIMESTAMPS_SUPPORT) && (DBG_TIMESTAMPS_SUPPORT == STD_ON)
	Dbg_UseLocalTimestampActivation(TRUE);
#endif
}

/*********************************************************************************************************************/
/*! \brief  This service deinitializes the DBG module. The deinitialization function shall disable the collection of 
*           further debugging data, cancel the  alarm for cyclic data collection, stop passing data to communication part 
*           of the debugging module, and call Dbg_ComDeInit to stop all communication with the host. 
*   \param  -
*   \pre    -
*   \post   -
*   \return -
**********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_DeInit(void)
{ /* [SWS_Dbg_00220] */
	Dbg_EnableDidCollection(FALSE);
}

#if defined (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON)
/*********************************************************************************************************************/
/*! \brief  Returns the version of the module
*   \param  VersionInfo: pointer to a structure which is initialized with version information
*   \pre    -
*   \post   -
*   \return -
**********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DBG_APPL_VAR) VersionInfo)
{ /* [SWS_Dbg_00139] */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Dbg_CheckDetErrorReturnVoid((NULL_PTR != VersionInfo), DBG_API_ID_GETVERSIONINFO, DBG_E_PARAM_POINTER);  
  
  VersionInfo -> vendorID         = DBG_VENDOR_ID;
  VersionInfo -> moduleID         = DBG_MODULE_ID;
  VersionInfo -> sw_major_version = DBG_SW_MAJOR_VERSION;
  VersionInfo -> sw_minor_version = DBG_SW_MINOR_VERSION;
  VersionInfo -> sw_patch_version = DBG_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON) */

#define DBG_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  END OF FILE: Dbg.c
 *********************************************************************************************************************/
