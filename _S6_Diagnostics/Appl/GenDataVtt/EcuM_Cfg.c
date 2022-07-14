/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: EcuM
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: EcuM_Cfg.c
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/




/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0310 EOF */ /* MD_CSL_0310 */
#include "EcuM_Cbk.h"

#define ECUM_PRIVATE_CFG_PARTLY_INCLUDE
#include "EcuM_PrivateCfg.h"
#undef ECUM_PRIVATE_CFG_PARTLY_INCLUDE /* PRQA S 0841 */ /* MD_EcuM_0841 */

#if((ECUM_CHANNELOFWAKEUPSOURCELIST == STD_ON) ||(ECUM_PNCOFCOMMPNCS == STD_ON) || (ECUM_SUPPORT_COMM == STD_ON))
#include "ComM.h"
#endif

#if defined(STATIC)
#else
# define STATIC static
#endif


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA
**********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  EcuM_DriverInitOne
**********************************************************************************************************************/
/** 
  \var    EcuM_DriverInitOne
  \details
  Element     Description
  Function
*/ 
#define ECUM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_DriverInitOneType, ECUM_CONST) EcuM_DriverInitOne[1] = {
    /* Index    Function              */
  { /*     0 */ EcuM_AL_DriverInitOne }
};
#define ECUM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  EcuM_DriverRestartList
**********************************************************************************************************************/
/** 
  \var    EcuM_DriverRestartList
  \details
  Element     Description
  Function
*/ 
#define ECUM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_DriverRestartListType, ECUM_CONST) EcuM_DriverRestartList[1] = {
    /* Index    Function                  */
  { /*     0 */ EcuM_AL_DriverRestartList }
};
#define ECUM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  EcuM_WakeupSourceList
**********************************************************************************************************************/
/** 
  \var    EcuM_WakeupSourceList
  \brief  Contains all parameters configured for Wakeup Sources
  \details
  Element    Description
  Channel    Mapped ComM channel number - 255 if no ComM channel is assigned.
*/ 
#define ECUM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_WakeupSourceListType, ECUM_CONST) EcuM_WakeupSourceList[7] = {
    /* Index    Channel                                    Comment                                    Referable Keys */
  { /*     0 */ 255                                 },  /* [ECUM_WKSOURCE_POWER]          */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_POWER, ECUM_WKSOURCE_POWER] */
  { /*     1 */ 255                                 },  /* [ECUM_WKSOURCE_RESET]          */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_RESET, ECUM_WKSOURCE_RESET] */
  { /*     2 */ 255                                 },  /* [ECUM_WKSOURCE_INTERNAL_RESET] */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_INTERNAL_RESET, ECUM_WKSOURCE_INTERNAL_RESET] */
  { /*     3 */ 255                                 },  /* [ECUM_WKSOURCE_INTERNAL_WDG]   */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_INTERNAL_WDG, ECUM_WKSOURCE_INTERNAL_WDG] */
  { /*     4 */ 255                                 },  /* [ECUM_WKSOURCE_EXTERNAL_WDG]   */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_EXTERNAL_WDG, ECUM_WKSOURCE_EXTERNAL_WDG] */
  { /*     5 */ ComMConf_ComMChannel_CAN00_f26020e5 },  /* [ECUM_WKSOURCE_CAN]            */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/ECUM_WKSOURCE_CAN, ECUM_WKSOURCE_CAN] */
  { /*     6 */ ComMConf_ComMChannel_CAN00_f26020e5 }   /* [CAN00_f26020e5]               */  /* [/ActiveEcuC/EcuM/EcuMConfiguration/EcuMCommonConfiguration/CAN00_f26020e5, CAN00_f26020e5] */
};
#define ECUM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  EcuM_ModuleState
**********************************************************************************************************************/
#define ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(EcuM_StateType, ECUM_VAR_NOINIT) EcuM_ModuleState;
#define ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */



/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL FUNCTIONS
**********************************************************************************************************************/



