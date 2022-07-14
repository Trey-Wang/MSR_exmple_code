/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Rte_BswM_Type.h
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Application types header file for SW-C <BswM>
 *********************************************************************************************************************/

/* double include prevention */
#ifndef _RTE_BSWM_TYPE_H
# define _RTE_BSWM_TYPE_H

# ifdef __cplusplus
extern "C"
{
# endif /* __cplusplus */

# include "Rte_Type.h"

# ifndef RTE_CORE

/**********************************************************************************************************************
 * Range, Invalidation, Enumeration and Bit Field Definitions
 *********************************************************************************************************************/

#  ifndef STARTUP
#   define STARTUP (0U)
#  endif

#  ifndef RUN
#   define RUN (1U)
#  endif

#  ifndef POSTRUN
#   define POSTRUN (2U)
#  endif

#  ifndef WAKEUP
#   define WAKEUP (3U)
#  endif

#  ifndef SHUTDOWN
#   define SHUTDOWN (4U)
#  endif

#  ifndef RELEASED
#   define RELEASED (0U)
#  endif

#  ifndef REQUESTED
#   define REQUESTED (1U)
#  endif

#  ifndef TIMER_EXPIRED
#   define TIMER_EXPIRED (0U)
#  endif

#  ifndef TIMER_NOT_EXPIRED
#   define TIMER_NOT_EXPIRED (1U)
#  endif

# endif /* RTE_CORE */


/**********************************************************************************************************************
 * Definitions for Mode Management
 *********************************************************************************************************************/
# ifndef RTE_MODETYPE_ESH_Mode
#  define RTE_MODETYPE_ESH_Mode
typedef BswM_ESH_Mode Rte_ModeType_ESH_Mode;
# endif
# ifndef RTE_MODETYPE_ESH_RunRequest
#  define RTE_MODETYPE_ESH_RunRequest
typedef BswM_ESH_RunRequest Rte_ModeType_ESH_RunRequest;
# endif
# ifndef RTE_MODETYPE_MdgShutdownTimerState
#  define RTE_MODETYPE_MdgShutdownTimerState
typedef BswM_MdgShutdownTimerState Rte_ModeType_MdgShutdownTimerState;
# endif

# ifndef RTE_MODE_ESH_Mode_POSTRUN
#  define RTE_MODE_ESH_Mode_POSTRUN (0U)
# endif
# ifndef RTE_MODE_ESH_Mode_RUN
#  define RTE_MODE_ESH_Mode_RUN (1U)
# endif
# ifndef RTE_MODE_ESH_Mode_SHUTDOWN
#  define RTE_MODE_ESH_Mode_SHUTDOWN (2U)
# endif
# ifndef RTE_MODE_ESH_Mode_STARTUP
#  define RTE_MODE_ESH_Mode_STARTUP (3U)
# endif
# ifndef RTE_MODE_ESH_Mode_WAKEUP
#  define RTE_MODE_ESH_Mode_WAKEUP (4U)
# endif
# ifndef RTE_TRANSITION_ESH_Mode
#  define RTE_TRANSITION_ESH_Mode (5U)
# endif

# ifndef RTE_MODE_ESH_RunRequest_RELEASED
#  define RTE_MODE_ESH_RunRequest_RELEASED (0U)
# endif
# ifndef RTE_MODE_ESH_RunRequest_REQUESTED
#  define RTE_MODE_ESH_RunRequest_REQUESTED (1U)
# endif
# ifndef RTE_TRANSITION_ESH_RunRequest
#  define RTE_TRANSITION_ESH_RunRequest (2U)
# endif

# ifndef RTE_MODE_MdgShutdownTimerState_TIMER_EXPIRED
#  define RTE_MODE_MdgShutdownTimerState_TIMER_EXPIRED (0U)
# endif
# ifndef RTE_MODE_MdgShutdownTimerState_TIMER_NOT_EXPIRED
#  define RTE_MODE_MdgShutdownTimerState_TIMER_NOT_EXPIRED (1U)
# endif
# ifndef RTE_TRANSITION_MdgShutdownTimerState
#  define RTE_TRANSITION_MdgShutdownTimerState (2U)
# endif

# ifdef __cplusplus
} /* extern "C" */
# endif /* __cplusplus */

#endif /* _RTE_BSWM_TYPE_H */
