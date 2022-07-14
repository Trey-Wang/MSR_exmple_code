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
 *          File:  Rte_CtSaDoor_Type.h
 *        Config:  D:/usr/usage/Presentations/MSR_Exercises/MSR_Exercises_AiP4_UPDATE/MSR_Exercises_AiP4/_S6_Diagnostics/MyECU.dpa
 *     SW-C Type:  CtSaDoor
 *  Generated at:  Tue Jun 26 09:53:39 2018
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Application types header file for SW-C <CtSaDoor> (Contract Phase)
 *********************************************************************************************************************/

/* double include prevention */
#ifndef _RTE_CTSADOOR_TYPE_H
# define _RTE_CTSADOOR_TYPE_H

# ifdef __cplusplus
extern "C"
{
# endif /* __cplusplus */

# include "Rte_Type.h"

/**********************************************************************************************************************
 * Range, Invalidation, Enumeration and Bit Field Definitions
 *********************************************************************************************************************/

# ifndef CMDOORSTATE_DOORCLOSED
#  define CMDOORSTATE_DOORCLOSED (0U)
# endif

# ifndef CMDOORSTATE_DOOROPEN
#  define CMDOORSTATE_DOOROPEN (1U)
# endif



/**********************************************************************************************************************
 * Definitions for Mode Management
 *********************************************************************************************************************/
# ifndef RTE_MODETYPE_ComMMode
#  define RTE_MODETYPE_ComMMode
typedef uint8 Rte_ModeType_ComMMode;
# endif
# ifndef RTE_MODETYPE_ESH_Mode
#  define RTE_MODETYPE_ESH_Mode
typedef uint8 Rte_ModeType_ESH_Mode;
# endif

# ifndef RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION
#  define RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION (0U)
# endif
# ifndef RTE_MODE_ComMMode_COMM_NO_COMMUNICATION
#  define RTE_MODE_ComMMode_COMM_NO_COMMUNICATION (1U)
# endif
# ifndef RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION
#  define RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION (2U)
# endif
# ifndef RTE_TRANSITION_ComMMode
#  define RTE_TRANSITION_ComMMode (3U)
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

# ifdef __cplusplus
} /* extern "C" */
# endif /* __cplusplus */

#endif /* _RTE_CTSADOOR_TYPE_H */
