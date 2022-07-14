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
 *         File:  Dbg.h
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

#if !defined(DBG_H)
# define DBG_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Dbg_Cfg.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : Monitoring_AsrDbg CQComponent : Implementation */
/* Version - BCD coded version number - Main- and Sub-Version - Release-Version */
# define MONITORING_ASRDBG_VERSION            (0x0100u)
# define MONITORING_ASRDBG_RELEASE_VERSION    (0x01u)

/* vendor and module identification */
# define DBG_VENDOR_ID                        (30u)
# define DBG_MODULE_ID                        (57u) /* ESCAN00087207 */

/* Component Version Information */
# define DBG_SW_MAJOR_VERSION                 (MONITORING_ASRDBG_VERSION >> 8u)
# define DBG_SW_MINOR_VERSION                 (MONITORING_ASRDBG_VERSION & 0x00FF)
# define DBG_SW_PATCH_VERSION                 (MONITORING_ASRDBG_RELEASE_VERSION)


/* Development Error Detection */
# define DBG_E_INVALID_DID                   ((uint8)0x01u) /* [SWS_Dbg_00228] */
# define DBG_E_PARAM_POINTER                 ((uint8)0x02u) /* [SWS_Dbg_00234]  */

/* API ID */
# define DBG_API_ID_INIT                        ((uint8)0x01u) /* [SWS_Dbg_00138] */
# define DBG_API_ID_GETVERSIONINFO              ((uint8)0x03u) /* [SWS_Dbg_00139] */
# define DBG_API_ID_ENABLEDIDCOLLECTION         ((uint8)0x14u) /* [SWS_Dbg_00152] */
# define DBG_API_ID_USELOCALTIMESTAMPACTIVATION ((uint8)0x16u) /* [SWS_Dbg_00154] */
# define DBG_API_ID_PERIODICSAMPLINGFUNC        ((uint8)0x1Eu) /* [SWS_Dbg_00124] */
# define DBG_API_ID_DEINIT                      ((uint8)0x24u) /* [SWS_Dbg_00220] */

# if defined (DBG_AUTOSARVERSION4) && !defined (STATIC)
#  define STATIC static
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

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
FUNC(void, DBG_CODE) Dbg_Init(void); /* [SWS_Dbg_00138] */

/*********************************************************************************************************************/
/*! \brief  This service deinitializes the DBG module. The deinitialization function shall disable the collection of 
*           further debugging data, cancel the  alarm for cyclic data collection, stop passing data to communication part 
*           of the debugging module, and call Dbg_ComDeInit to stop all communication with the host. 
*   \param  -
*   \pre    -
*   \post   -
*   \return -
**********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_DeInit(void); /* [SWS_Dbg_00220] */

# if defined (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON)
/*********************************************************************************************************************/
/*! \brief  Returns the version of the module
*   \param  VersionInfo: pointer to a structure which is initialized with version information
*   \pre    -
*   \post   -
*   \return -
**********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DBG_APPL_VAR) VersionInfo); /* [SWS_Dbg_00139] */
# endif /* (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
  Dbg_EnableDidCollection
**********************************************************************************************************************/
/*! \brief      Acceptance of data can be set to TRUE/FALSE in general. If set to FALSE, all data which is passed to the 
 *              debugging core module is discarded. The information if DID collection is set to TRUE/FALSE is part of the 
 *              status of the debugging core module.
 *              
 *              DID collection TRUE/FALSE does not change the individual DID activation TRUE/FALSE setting. If DID 
 *              collection is set to FALSE and then to TRUE again, the old individual settings are in place.   
 *  \param[in]  boolean DidCollectionStatus
 *  \return     void
 *  \see        The function is called by Autosar Rte
 *  \note       -
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_EnableDidCollection(boolean DidCollectionStatus); /* [SWS_Dbg_00152] */

# if defined (DBG_TIMESTAMPS_SUPPORT) && (DBG_TIMESTAMPS_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dbg_UseLocalTimestampActivation
**********************************************************************************************************************/
/*! \brief      This service allows the user of the module to set timestamp collection TRUE/FALSE for all collected DID.  
 *               
 *              The debugging core module uses a hardware free running timer (HWFRT) of the AUTOSAR GPT module to get a 
 *              timestamp. The HWFRT to be used has to be configured. If no HWFRT is applied, calls to add timestamps are 
 *              ignored. 
 *              The debugging core module will read a first value from the HWFRT during initialization of the module. 
 *              The information, if timestamp is set TRUE/FALSE, is part of the status of the debugging core module. 
 *
 *              Global timestamp TRUE/FALSE does not change the individual DID timestamp TRUE/FALSE setting (Dbg155). 
 *              If global timestamp is set to FALSE and then to TRUE again, the old individual settings are in place.    
 *  \param[in]  boolean GlobalTimestampCollectionStatus
 *  \return     void
 *  \see        The function is called by Autosar Rte
 *  \note       -
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_UseLocalTimestampActivation(boolean GlobalTimestampCollectionStatus); /* [SWS_Dbg_00154] */
# endif /* (DBG_TIMESTAMPS_SUPPORT) && (DBG_TIMESTAMPS_SUPPORT == STD_ON) */

/**********************************************************************************************************************
  Dbg_PeriodicSamplingFunction
**********************************************************************************************************************/
/*! \brief      This function is responsible for periodic sampling of debugging data. As it can be dynamically switched 
*               off, or the period can be changed, a separate OS alarm is needed to serve Dbg_PeriodicSamplingFunction. 
 *  \param[in]  void
 *  \return     void
 *  \see        The function is called by Autosar Rte
 *  \note       -
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_PeriodicSamplingFunction(void); /* [SWS_Dbg_00124] */

#define DBG_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/**********************************************************************************************************************
 *  END OF FILE: Dbg.h
 *********************************************************************************************************************/
