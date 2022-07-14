/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
 /**       \file  VX1000If.h
 *        \brief  VX1000 Interface header file
 *
 *      \details  Implementation of an API wrapper between ASAM names and proprietary VX1000 driver names.
 *
 *********************************************************************************************************************/ 

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Oliver Reineke                visore        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2015-03-16  visore  ESCAN00082675 FEAT-1488: Implement VX1000 Interface
 *  1.01.00   2017-02-01  visore  ESCAN00093798 Add support for address update of dynamic-address signals.
 *  1.02.00   2017-02-17  visore  ESCAN00094061 Add reference to supplemental VX1000If documentation.
 *********************************************************************************************************************/

#if !defined(VX1000IF_H)
# define VX1000IF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "VX1000.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VX1000IF_SW_MAJOR_VERSION                    (1u)
# define VX1000IF_SW_MINOR_VERSION                    (2u)
# define VX1000IF_SW_PATCH_VERSION                    (0u)

# if defined(VX1000IF_CFG_NO_TARGET)
#  error "no target selected that tells the interface which driver files to include"
# else /* !VX1000IF_CFG_NO_TARGET */
#  if !defined(VX1000If_IsVX1000DriverAccessEnabled)
#   error "no boolean expression defined that tells the interface whether to call or to not call the actual driver"
#  else /* VX1000If_IsVX1000DriverAccessEnabled */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ------- Modes ------- */
#   define VX1000IF_UNINIT                              (0x00u)
#   define VX1000IF_PRE_INIT                            (0x01u)
#   define VX1000IF_INITIALIZED                         (0x02u)

/* --- Return Values --- */
#   define VX1000IF_RET_VOID                            (void)0
#   define VX1000IF_RET_E_NOT_OK                        (0xFFu)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA S 0342 3 */ /* MD_VX1000IF_0342 */
#   define VX1000IF_IMPL(functionName, retVal)      ((!VX1000If_IsVX1000DriverAccessEnabled) || (VX1000If_State != VX1000IF_INITIALIZED))?(VX1000If_ErrorCount++,retVal):VX1000_##functionName
/* PRQA S 3412 1 */ /* MD_MSR_19.4 */
#   define VX1000IF_IMPL_VOID(functionName)          if ((!VX1000If_IsVX1000DriverAccessEnabled) || (VX1000If_State != VX1000IF_INITIALIZED)) { VX1000If_ErrorCount++; } else VX1000_##functionName

/**********************************************************************************************************************
 *  VX1000If_InitAsyncStart()
 *********************************************************************************************************************/
/*! \brief       Macro to perform the VX1000 start-up handshaking
 *  \details     Makes the VX1000 device driver detect an attached VX1000 device and perform a first handshake. 
 *  \pre         VX1000If_Init must have been called.
 *  \context     TASK                                                     
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-812
 *********************************************************************************************************************/
/* PRQA S 3412 4 */ /* MD_MSR_19.4 */
#   define   VX1000If_InitAsyncStart()             if ( (VX1000If_IsVX1000DriverAccessEnabled)                     &&       \
                                                        (VX1000If_State == VX1000IF_PRE_INIT))                              \
                                                        { VX1000_INIT_ASYNC_START(); VX1000If_State = VX1000IF_INITIALIZED; } \
                                                   else { VX1000If_ErrorCount++; }

/**********************************************************************************************************************
 *  VX1000If_InitAsyncEnd()
 *********************************************************************************************************************/
/*! \brief       Macro to finish the VX1000 start-up handshake
 *  \details     Makes the VX1000 device driver perform the second part of the handshake with an attached VX1000 device. 
 *               Waits for end of handshake and can be used to synchronize the instrumentation on several cores.
 *  \pre         VX1000If_InitAsyncStart must have been called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-813
 *********************************************************************************************************************/
/* PRQA S 3412 4 */ /* MD_MSR_19.4 */
#   define   VX1000If_InitAsyncEnd                    VX1000IF_IMPL_VOID(INIT_ASYNC_END)

/**********************************************************************************************************************
 *  VX1000If_PrepareSoftreset() / VX1000If_PrepareSoftresetVoid()
 *********************************************************************************************************************/
/*! \brief       Macros for Pre-restart handshake with the VX1000 device driver
 *  \details     Makes the VX1000 device driver inform an attached VX1000 device about an upcoming software reset. 
 *  \return      0 - reset procedure confirmed by tool
 *  \return      1 - handshake failed (measurement won't be resumed after the reset)
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-814
 *********************************************************************************************************************/
#   define   VX1000If_PrepareSoftreset                VX1000IF_IMPL(PREPARE_SOFTRESET        , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_PrepareSoftresetVoid            VX1000IF_IMPL_VOID(PREPARE_SOFTRESET_VOID)

/********************** STIM API BEGIN *******************************************************************************/

/**********************************************************************************************************************
 *  VX1000If_StimControl() / VX1000If_BypassControl()
 *********************************************************************************************************************/
/*! \brief       Macros for the STIM-Keep-Alive-Handler
 *  \details     DEPRECATED: VX1000If_BypassControl should be used for new projects instead of  VX1000If_StimControl! 
 *               Must be cyclically called by the application if STIM/Bypassing is used. Makes the VX1000 device driver 
 *               perform bypassing management tasks, like globally starting and stopping bypassing operation. 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-805
 *********************************************************************************************************************/
#   define   VX1000If_StimControl                     VX1000IF_IMPL_VOID(STIM_CONTROL)
#   define   VX1000If_BypassControl                   VX1000IF_IMPL_VOID(BYPASS_CONTROL)

/**********************************************************************************************************************
 *  VX1000If_StimRequest()
 *********************************************************************************************************************/
/*! \brief       Macro to a request STIM data set
 *  \details     Makes the VX1000 device driver request a specific STIM data set associated to event channel stim_event.
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-806
 *********************************************************************************************************************/
#   define   VX1000If_StimRequest                     VX1000IF_IMPL_VOID(STIM_REQUEST)

/**********************************************************************************************************************
 *  VX1000If_StimWait() / VX1000If_StimWaitVoid() / VX1000If_BypassStim() / VX1000If_BypassStimVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to wait until a specific STIM request is fulfilled
 *  \details     Makes the VX1000 device driver wait in a busy polling loop until a specific STIM request is fulfilled. 
 *               Depending on the STIM method used, on success all transfer descriptors assigned to stim_event are 
 *               processed and the STIM data is transferred to its destination. 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \param[in]   timeout_us:          timeout in microseconds, starting from related call to VX1000If_StimRequest 
 *  \return      0 - data arrived before timeout or timeout but data still copied successfully
 *  \return      1 - timeout, no new data arrived or error during copying and destination data corrupted
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_StimRequest and VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-808
 *********************************************************************************************************************/
#   define   VX1000If_StimWait                        VX1000IF_IMPL(STIM_WAIT                , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_StimWaitVoid                    VX1000IF_IMPL_VOID(STIM_WAIT_VOID)
#   define   VX1000If_BypassStim                      VX1000IF_IMPL(BYPASS_STIM              , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_BypassStimVoid                  VX1000IF_IMPL_VOID(BYPASS_STIM_VOID)

/**********************************************************************************************************************
 *  VX1000If_StimActive()
 *********************************************************************************************************************/
/*! \brief       Macro to check the STIM status of an event
 *  \details     Makes the VX1000 device driver return whether STIM is active for the specific event channel and globally. 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \return      1 - active
 *  \return      0 - inactive
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-807
 *********************************************************************************************************************/
#   define   VX1000If_StimActive                      VX1000IF_IMPL(STIM_ACTIVE              , VX1000IF_RET_E_NOT_OK)

/**********************************************************************************************************************
 *  VX1000If_StimSkip()
 *********************************************************************************************************************/
/*! \brief       Macro to send a STIM skip event
 *  \details     Makes the VX1000 device driver send a STIM skip event to tell the attached VX1000 device not to 
 *               stimulate the next cycle. This is in effect a dummy STIM request. 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-809
 *********************************************************************************************************************/
#   define   VX1000If_StimSkip                        VX1000IF_IMPL_VOID(STIM_SKIP)

/**********************************************************************************************************************
 *  VX1000If_Stimulate() / VX1000If_StimulateVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to initiate a bypass without sending DAQ data
 *  \details     Makes the VX1000 device driver perform a complete stimulation. This is done by requesting STIM data 
 *               using the stim_trigger_event which is in fact a DAQ event with or without DAQ data. The first cycle_delay 
 *               calls are used to fill a STIM data pipeline in the VX1000. During these calls the function will return 0. 
 *               After the first cycle_delay calls, this function will busy wait with timeout for the VX1000 to actually 
 *               complete the stimulation (stim_event). The pipeline depth / initial delay has to be considered when 
 *               generating the STIM data. 
 *  \param[in]   stim_trigger_event:  DAQ event range 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \param[in]   cycle_delay:         Specifies the number of cycles between triggering and the associated 
 *                                    stimulation. During the first cycle_delay cycles there is no stimulation in the 
 *                                    ECU, instead the VX1000 device fills its STIM buffer FIFO. 
 *  \param[in]   timeout_us:          Timeout in microseconds, starting upon calling this function. 
 *  \return      0 - code to be bypassed shall be executed as bypassing is not active 
 *  \return      1 - STIM successful, code to be bypassed shall be skipped 
 *  \return      2 - STIM failed, it is up to the application to handle this error 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-816
 *********************************************************************************************************************/
#   define   VX1000If_Stimulate                       VX1000IF_IMPL(STIMULATE                , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_StimulateVoid                   VX1000IF_IMPL_VOID(STIMULATE_VOID)

/**********************************************************************************************************************
 *  VX1000If_Bypass() / VX1000If_BypassVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to initiate a bypass with sending DAQ data and wait for completion
 *  \details     Makes the VX1000 device driver initiate a bypass by sending DAQ data to the tool and implicitly requesting 
 *               a stimulation (daq_event), then busy wait with timeout for the tool to complete the stimulation (stim_event). 
 *  \param[in]   daq_event:           DAQ event range 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \param[in]   timeout_us:          Timeout in microseconds, starting with the function call 
 *  \return      0 - code to be bypassed shall be executed as bypassing is not active 
 *  \return      1 - Bypass successful, code to be bypassed shall be skipped 
 *  \return      2 - Bypass failed, it is up to the application to handle this error 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-815
 *********************************************************************************************************************/
#   define   VX1000If_Bypass                          VX1000IF_IMPL(BYPASS                   , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_BypassVoid                      VX1000IF_IMPL_VOID(BYPASS_VOID)

/**********************************************************************************************************************
 *  VX1000If_BypassDaq() / VX1000If_BypassDaqVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to initiate a bypass by sending a DAQ event
 *  \details     Makes the VX1000 device driver initiate a bypass by sending a DAQ event followed by a STIM request. 
 *  \param[in]   daq_event:           DAQ event range 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \return      0 - bypassing is not active: code to be bypassed shall be executed 
 *  \return      1 - bypassing is active: code to be bypassed shall shall be skipped 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-810
 *********************************************************************************************************************/
#   define   VX1000If_BypassDaq                      VX1000IF_IMPL(BYPASS_DAQ               , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_BypassDaqVoid                  VX1000IF_IMPL_VOID(BYPASS_DAQ_VOID) 

/**********************************************************************************************************************
 *  VX1000If_BypassTrigger() / VX1000If_BypassTriggerVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to initiate a bypass with sending DAQ data
 *  \details     Makes the VX1000 device driver initiate a bypass by sending DAQ data to the tool and implicitly requesting 
 *               a stimulation (daq_event). 
 *  \param[in]   daq_event:           DAQ event range 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \return      0 - bypassing is not active: code to be bypassed shall be executed 
 *  \return      1 - bypassing is active: code to be bypassed shall shall be skipped 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-839
 *********************************************************************************************************************/
#   define   VX1000If_BypassTrigger                  VX1000IF_IMPL(BYPASS_TRIGGER           , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_BypassTriggerVoid              VX1000IF_IMPL_VOID(BYPASS_TRIGGER_VOID)

/**********************************************************************************************************************
 *  VX1000If_BypassWait() / VX1000If_BypassWaitVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to wait for the too to complete a stimulation
 *  \details     Makes the VX1000 device driver perform a busy wait with timeout for the tool to complete a stimulation 
 *               (stim_event) that has been initiated beforehand by an appropriate call to VX1000If_BypassTrigger. 
 *  \param[in]   stim_event:          The range for stim_event is defined in the VX1000 device driver configuration. 
 *                                    It is a subset of the available DAQ event channels. 
 *  \param[in]   timeout_us:          Timeout in microseconds, starting with the function call 
 *  \return      0 - bypassed code shall be activated because bypassing is not active 
 *  \return      1 - everything done, bypassed code shall be disabled
 *  \return      2 - bypassing failed; it's up to the application design whether executing the bypassed code makes sense here
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_BypassTrigger and VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-817
 *********************************************************************************************************************/
#   define   VX1000If_BypassWait                     VX1000IF_IMPL(BYPASS_WAIT              , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_BypassWaitVoid                 VX1000IF_IMPL_VOID(BYPASS_WAIT_VOID)

/**********************************************************************************************************************
 *  VX1000If_Event() / VX1000If_OvlSetConfigVoid()
 *********************************************************************************************************************/
/*! \brief       Macro to processes all transfer descriptors assigned to the given event.
 *  \details     Makes the VX1000 device driver trigger an XCP event. For copying-mechanism-based DAQ, makes the 
 *               VX1000 device driver process all transfer descriptors assigned to eventNumber and to copy the DAQ data 
 *               to an intermediate buffer to be read by the VX1000. 
 *  \param[in]   eventNumber:         DAQ event range 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-836
 *********************************************************************************************************************/
#   define   VX1000If_Event                          VX1000IF_IMPL_VOID(EVENT)

/********************** STIM API END *********************************************************************************/

/********************** Generic Hooking Control BEGIN ****************************************************************/

/**********************************************************************************************************************
 *  VX1000If_HookTrigger() / VX1000If_HookTriggerVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to trigger a generic bypass whose event IDs are related to HookID. 
 *  \details     Makes the VX1000 device driver trigger a generic bypass whose event IDs are related to HookID. 
 *  \param[in]   HookID:              Hook id range 
 *  \return      0 - inactive bypass or active bypass and original code enabled
 *  \return      1 - bypass active and original code disabled
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-821
 *********************************************************************************************************************/
#   define   VX1000If_HookTrigger                    VX1000IF_IMPL(HOOK_TRIGGER             , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_HookTriggerVoid                VX1000IF_IMPL_VOID(HOOK_TRIGGER_VOID) 

/**********************************************************************************************************************
 *  VX1000If_HookWait() / VX1000If_HookWaitVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to trigger with timeout for a generic bypass whose event IDs are related to HookID.
 *  \details     Makes the VX1000 device driver stimulate with timeout for a generic bypass whose event IDs are related to 
 *               HookID. Optionally, an additional DAQ event will be triggered. 
 *  \param[in]   HookID:              Hook id range 
 *  \param[in]   timeout_us:          Timeout in microseconds, starting with the function call 
 *  \return      0 - bypass inactive 
 *  \return      1 - stimulation done, no timeout, OK
 *  \return      2 - stimulation not done, timeout
 *  \return      3 - stimulation not done, timeout, execute original code
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-822
 *********************************************************************************************************************/
#   define   VX1000If_HookWait                       VX1000IF_IMPL(HOOK_WAIT                , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_HookWaitVoid                   VX1000IF_IMPL_VOID(HOOK_WAIT_VOID)

/**********************************************************************************************************************
 *  VX1000If_GenericEvent()
 *********************************************************************************************************************/
/*! \brief       Macro to trigger a generic event whose event ID is related to HookID.
 *  \details     Makes the VX1000 device driver trigger a generic event whose event ID is related to hook_id. 
 *  \param[in]   HookID:              Hook id range 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-823
 *********************************************************************************************************************/
#   define   VX1000If_GenericEvent                   VX1000IF_IMPL_VOID(GENERIC_EVENT)

/**********************************************************************************************************************
 *  VX1000If_Hook()
 *********************************************************************************************************************/
/*! \brief       Macro to trigger a generic event whose event ID is related to HookID.
 *  \details     Makes the VX1000 device driver trigger a generic bypass for STIM events with VX1000  Hooks, then wait 
 *               until the data set requests of this generic bypass are finished successfully and then trigger a DAQ event or 
 *               in case of inactive hook or failed stimulation to execute user code without triggering the DAQ event. 
 *  \param[in]   HookID:              Hook id range as configured in the VX1000 device driver 
 *  \param[in]   timeout_us:          Timeout in microseconds, starting with the function call 
 *  \param[in]   code:                User code to be executed in case of failed stimulation or inactive hook. 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-838
 *********************************************************************************************************************/
#   define   VX1000If_Hook                           VX1000IF_IMPL_VOID(HOOK)

/********************** Generic Hooking Control END ******************************************************************/

/********************** Hook based bypassing macros  BEGIN ***********************************************************/

/**********************************************************************************************************************
 *  VX1000If_BypassHbbGetval<8,16,32,64,Float,Double> ()
 *********************************************************************************************************************/
/*! \brief       Macros to wait for the too to complete a stimulation
 *  \details     Makes the VX1000 device driver check whether valid data corresponding to the given Hook ID is present in 
 *               the buffer. 
 *  \param[in]   HookID:              HBB id range as configured in the VX1000 device driver 
 *  \param[in]   DefaultValue:        Specifies the default value to be returned if hook is not valid. 
 *  \return      Data corresponding to the stimulated value, if hook is valid, data available and VX1000If_IsVX1000DriverAccessEnabled returned TRUE. 
 *               Otherwise DefaultValue is returned
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-824
 *********************************************************************************************************************/
/* PRQA S 3453 FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */
#   define   VX1000If_BypassHbbGetval8(H, D)        VX1000IF_IMPL(BYPASS_HBB_GETVAL_8,       (D))((H), D)
#   define   VX1000If_BypassHbbGetval16(H, D)       VX1000IF_IMPL(BYPASS_HBB_GETVAL_16,      (D))((H), D)
#   define   VX1000If_BypassHbbGetval32(H, D)       VX1000IF_IMPL(BYPASS_HBB_GETVAL_32,      (D))((H), D)
#   define   VX1000If_BypassHbbGetval64(H, D)       VX1000IF_IMPL(BYPASS_HBB_GETVAL_64,      (D))((H), D)
#   define   VX1000If_BypassHbbGetvalFloat(H, D)    VX1000IF_IMPL(BYPASS_HBB_GETVAL_FLOAT,   (D))((H), D)
#   define   VX1000If_BypassHbbGetvalDouble(H, D)   VX1000IF_IMPL(BYPASS_HBB_GETVAL_DOUBLE,  (D))((H), D)
/* PRQA L:FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */

/********************** Hook based bypassing macros  END *************************************************************/

/********************** VX1000 Mailbox BEGIN *************************************************************************/

/**********************************************************************************************************************
 *  VX1000If_MailboxControl()
 *********************************************************************************************************************/
/*! \brief       Macro to check the VX1000 mailbox system for pending requests
 *  \details     Makes the VX1000 device driver check the VX1000 mailbox for pending requests and trigger necessary 
 *               actions. 
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox read function.                                 
 *               This function must not interrupt any VX1000 mailbox read function.                                         
 *  \trace       CREQ-825
 *********************************************************************************************************************/
#   define   VX1000If_MailboxControl                 VX1000IF_IMPL_VOID(MAILBOX_CONTROL)

/**********************************************************************************************************************
 *  VX1000If_MailboxWrite() / VX1000If_MailboxWriteVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to write to the Slave->Master mailbox and notify the master.
 *  \details     Makes the VX1000 device driver write len bytes from pBuf to the Slave->Master mailbox and notify the 
 *               master. 
 *  \param[in]   len:                 Speficies the message size in bytes.
 *  \param[in]   pBuf:                Specifies the pointer to message data input.
 *  \return      VX1000_MAILBOX_OK       - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_FULL - error: no free mailbox slots available 
 *  \return      VX1000_MAILBOX_ERR_NULL - error: pBuf is null pointer 
 *  \return      VX1000_MAILBOX_ERR_SIZE - error: len exceeds mailbox slot size 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox write function.                                 
 *               This function must not interrupt any VX1000 mailbox write function.                                         
 *  \trace       CREQ-826
 *********************************************************************************************************************/
#   define   VX1000If_MailboxWrite                   VX1000IF_IMPL(MAILBOX_WRITE            , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxWriteVoid               VX1000IF_IMPL_VOID(MAILBOX_WRITE_VOID)

/**********************************************************************************************************************
 *  VX1000If_MailboxWrite() / VX1000If_MailboxWriteVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to find out the location of the next unused message buffer
 *  \details     Makes the VX1000 device driver find out the location of the next unused message buffer and return the info 
 *               to the caller. 
 *               The mailbox state is neither changed nor is the master notified. To finalize writing data to the mailbox, 
 *               VX1000If_MailboxWriteDone must be called. 
 *  \param[in,out] ppBuf:             (IN)  pointer to a pointer variable. 
 *                                    (OUT) pointer to the data field of the next free Slave->Master mailbox. 
 *  \return      VX1000_MAILBOX_OK             - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_FULL       - error: no free mailbox slots available 
 *  \return      VX1000_MAILBOX_ERR_NULL       - error: pBuf is null pointer 
 *  \return      VX1000_MAILBOX_ERR_SPLIT_PEND - error: no pending write transaction 
 *  \return      VX1000IF_RET_E_NOT_OK         - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox read function.                                 
 *               This function must not interrupt any VX1000 mailbox read function.                                         
 *  \trace       CREQ-827
 *********************************************************************************************************************/
#   define   VX1000If_MailboxWriteSplit              VX1000IF_IMPL(MAILBOX_WRITESPLIT       , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxWriteSplitVoid          VX1000IF_IMPL_VOID(MAILBOX_WRITESPLIT_VOID)

/**********************************************************************************************************************
 *  VX1000If_MailboxWrite() / VX1000If_MailboxWriteVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to Finish a Slave->Master mailbox transfer.
 *  \details     Makes the VX1000 device driver finalize a Slave->Master mailbox transfer that has been started by calling 
 *               VX1000If_MailboxWriteSplit. 
 *  \param[in]   len:                 The size of the entire message in bytes. 
 *  \return      VX1000_MAILBOX_OK             - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_SIZE       - error: len exceeds mailbox slot size 
 *  \return      VX1000_MAILBOX_ERR_SPLIT_PEND - error: no pending write transaction 
 *  \return      VX1000IF_RET_E_NOT_OK         - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox write function.                                 
 *               This function must not interrupt any VX1000 mailbox write function.                                         
 *  \trace       CREQ-828
 *********************************************************************************************************************/
#   define   VX1000If_MailboxWriteDone               VX1000IF_IMPL(MAILBOX_WRITEDONE        , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxWriteDoneVoid           VX1000IF_IMPL_VOID(MAILBOX_WRITEDONE_VOID)

/**********************************************************************************************************************
 *  VX1000If_MailboxRead() / VX1000If_MailboxReadVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to read the data from next filled Master->Slave mailbox slot.
 *  \details     Makes the VX1000 device driver read the data from the next filled Master->Slave mailbox slot into pBuf 
 *               and return the number of bytes in pLen. 
 *  \param[in,out]   pLen:            Pointer holding the maximum allowed message size. 
 *                                    The value is overwritten with the actual message size if successful.
 *  \param[in,out]   pBuf:            Pointer to destination for the next message.
 *                                    The caller is responsible that the destination contains at least *pLen writeable bytes.
 *                                    The function aborts with an error if the buffer is too small for the current message 
 *                                    (no bytes copied).
 *                                    pBuf holds a copy of the entire message if successful.
 *  \return      VX1000_MAILBOX_OK             - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_EMPTY      - error: mailbox is empty  
 *  \return      VX1000_MAILBOX_ERR_NULL       - error: pLen or pBuf are null pointers 
 *  \return      VX1000_MAILBOX_ERR_SIZE       - error: mailbox slot content exceeds pLen 
 *  \return      VX1000_MAILBOX_ERR_SPLIT_PEND - error: split read transaction pending 
 *  \return      VX1000IF_RET_E_NOT_OK         - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox read function.                                 
 *               This function must not interrupt any VX1000 mailbox read function.                                    
 *  \trace       CREQ-829
 *********************************************************************************************************************/
#   define   VX1000If_MailboxRead                    VX1000IF_IMPL(MAILBOX_READ             , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxReadVoid                VX1000IF_IMPL_VOID(MAILBOX_READ_VOID)

/**********************************************************************************************************************
 *  VX1000If_MailboxReadSplit() / VX1000If_MailboxReadSplitVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to find out the location and length of the next unread message.
 *  \details     Makes the VX1000 device driver return the location and length of the next unread mailbox message. 
 *               Note: the mailbox state is not changed nor is the master notified. 
 *               VX1000If_MailboxReadDone must be called to complete the transaction. 
 *  \param[out]  pLen:                Pointer to a 32bit variable.
 *                                    The value is overwritten with the byte count of the next message if successful.
 *                                    The caller is responsible that the pointer is valid and that the destination is writeable.
 *  \param[in,out]   pBuf:            Pointer to the data field of the next unread message.
 *                                    The caller is responsible that the pointer is valid and that the destination is writeable.
 *  \return      VX1000_MAILBOX_OK        - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_EMPTY - error: mailbox is empty  
 *  \return      VX1000_MAILBOX_ERR_NULL  - error: pLen or ppBuf  is a null pointer 
 *  \return      VX1000_MAILBOX_ERR_SIZE  - error: mailbox slot content exceeds pLen 
 *  \return      VX1000_MAILBOX_ERR_SPLIT_PEND - error: another split read transaction is pending 
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox read function.                                 
 *               This function must not interrupt any VX1000 mailbox read function.                                         
 *  \trace       CREQ-830
 *********************************************************************************************************************/
#   define   VX1000If_MailboxReadSplit               VX1000IF_IMPL(MAILBOX_READSPLIT        , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxReadSplitVoid           VX1000IF_IMPL_VOID(MAILBOX_READSPLIT_VOID)

/**********************************************************************************************************************
 *  VX1000If_MailboxReadDone() / VX1000If_MailboxReadDoneVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to mark the filled Master->Slave mailbox slot as empty
 *  \details     Makes the VX1000 device driver mark the Master->Slave mailbox slot for the pending read transaction as 
 *               empty and notify the master afterwards. 
 *  \return      VX1000_MAILBOX_OK             - mailbox transfer successful 
 *  \return      VX1000_MAILBOX_ERR_SPLIT_PEND - no pending read split transaction 
 *  \return      VX1000IF_RET_E_NOT_OK         - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_MailboxReadSplit and VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function must not be interrupted by any VX1000 mailbox read function.                                 
 *               This function must not interrupt any VX1000 mailbox read function.                                         
 *  \trace       CREQ-831
 *********************************************************************************************************************/
#   define   VX1000If_MailboxReadDone                VX1000IF_IMPL(MAILBOX_READDONE         , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_MailboxReadDoneVoid            VX1000IF_IMPL_VOID(MAILBOX_READDONE_VOID)

/********************** VX1000 Mailbox END ***************************************************************************/

/********************** VX1000 Overlay BEGIN *************************************************************************/

/**********************************************************************************************************************
 *  VX1000If_OvlSetConfig() / VX1000If_OvlSetConfigVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to globally enable/disable overlays
 *  \details     Makes the VX1000 device driver execute a derivative-specific method to globally enable/disable overlays.
 *  \param[in]   value:               Overlay windows to be activated/deactivated
 *  \param[in]   mask:                Resource Mask
 *  \param[in]   page:                Overlay Page
 *  \param[in]   master:              Masters to be activated
 *  \param[in]   calMaster:           Masters resource Mask
 *  \return      0 - Nothing done
 *  \return      1 - Page switch done  
 *  \return      2 - Value not written correctly 
 *  \return      3 - No single-master page-switch possible
 *  \return      4 - Generic error
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-832
 *********************************************************************************************************************/
#   define   VX1000If_OvlSetConfig                  VX1000IF_IMPL(OVL_SET_CONFIG           , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_OvlSetConfigVoid              VX1000IF_IMPL_VOID(OVL_SET_CONFIG_VOID)

/**********************************************************************************************************************
 *  VX1000If_OvlSetConfigDone() / VX1000If_OvlSetConfigDoneVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to transmit the status of a page switching attempt to the VX1000 driver
 *  \details     Transmits the status of a page switching attempt to the VX1000 driver and optionally also to the XCP tool.
 *  \param[in]   value:               Overlay windows to be activated/deactivated
 *  \param[in]   mask:                Resource Mask
 *  \param[in]   page:                Overlay Page
 *  \param[in]   master:              Masters to be activated
 *  \param[in]   calMaster:           Masters resource Mask
 *  \return      0 - Nothing done
 *  \return      1 - Page switch done  
 *  \return      2 - Value not written correctly 
 *  \return      3 - No single-master page-switch possible
 *  \return      4 - Generic error
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-864
 *********************************************************************************************************************/
#   define   VX1000If_OvlSetConfigDone             VX1000IF_IMPL_VOID(OVL_SET_CONFIG_DONE)
#   define   VX1000If_OvlSetConfigDoneVoid         VX1000IF_IMPL_VOID(OVL_SET_CONFIG_DONE_STUP)

/**********************************************************************************************************************
 *  VX1000If_OvlChkPageSwDone() / VX1000If_OvlChkPageSwDoneVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to check page switching status of all bus masters for which a page switch was requested.
 *  \details     Makes the VX1000 driver check and finalize page switching status of all bus masters for which a page 
 *               switch was requested.
 *  \return      0 - Nothing done
 *  \return      1 - Page switch done  
 *  \return      2 - Value not written correctly 
 *  \return      3 - No single-master page-switch possible
 *  \return      4 - Generic error
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called.  
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-865
 *********************************************************************************************************************/
#   define   VX1000If_OvlChkPageSwDone             VX1000IF_IMPL(OVL_CHK_PAGESW_DONE      , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_OvlChkPageSwDoneVoid         VX1000IF_IMPL_VOID(OVL_CHK_PAGESW_DONE_VOID)

/**********************************************************************************************************************
 *  VX1000If_OvlChkPageSwCore() / VX1000If_OvlChkPageSwCoreVoid()
 *********************************************************************************************************************/
/*! \brief       Macros to check page switching status of a specific bus master
 *  \details     Makes the VX1000 driver check the page switching status of a specific bus master.
 *  \param[in]   master:              bus master to be checked
 *  \return      0 - Nothing done
 *  \return      1 - Page switch done  
 *  \return      2 - Value not written correctly 
 *  \return      3 - No single-master page-switch possible
 *  \return      4 - Generic error
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called.  
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-866
 *********************************************************************************************************************/
#   define   VX1000If_OvlChkPageSwCore             VX1000IF_IMPL(OVL_CHK_PAGESW_CORE      , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_OvlChkPageSwCoreVoid         VX1000IF_IMPL_VOID(OVL_CHK_PAGESW_CORE_VOID)

/**********************************************************************************************************************
 *  VX1000If_OvlIsPageSwRequested()
 *********************************************************************************************************************/
/*! \brief       Macros to check whether a page switch request is pending for a bus master
 *  \details     Makes the VX1000 driver check whether a page switch request is pending for a bus master or not.
 *  \param[in]   master:              bus master to be checked
 *  \return      0 - Page switch is not pending
 *  \return      1 - Page switch is pending
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called.  
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-867
 *********************************************************************************************************************/
#   define   VX1000If_OvlIsPageSwRequested         VX1000IF_IMPL(OVL_IS_PAGESW_REQUESTED  , VX1000IF_RET_E_NOT_OK)

/**********************************************************************************************************************
 *  VX1000If_InvalidateEmem()
 *********************************************************************************************************************/
/*! \brief       Macros to invalidate the signature of the VX1000-allocated persistent ECU-RAM
 *  \details     Invalidate the signature of the VX1000-allocated persistent ECU-RAM.
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-868
 *********************************************************************************************************************/
#   define   VX1000If_InvalidateEmem               VX1000IF_IMPL_VOID(INVALIDATE_EMEM)

/**********************************************************************************************************************
 *  VX1000If_CalWakeupRequested()
 *********************************************************************************************************************/
/*! \brief       Macros to check whether the XCP tool requested a wakeup for calibration purposes or not.
 *  \details     Makes the VX1000 driver check whether the XCP tool has requested a wakeup for calibration purposes.
 *  \return      0 - No request pending
 *  \return      1 - Request pending
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-869
 *********************************************************************************************************************/
#   define   VX1000If_CalWakeupRequested           VX1000IF_IMPL(CAL_WAKEUP_REQUESTED     , VX1000IF_RET_E_NOT_OK)

/**********************************************************************************************************************
 *  VX1000If_IsCalWakeupActive()
 *********************************************************************************************************************/
/*! \brief       Macros to check whether the ECU must stay awake for calibration purposes or not.
 *  \details     Makes the VX1000 driver check whether the ECU must stay awake for calibration purposes or not.
 *  \return      0 - ECU must not stay awake
 *  \return      1 - ECU must stay awake
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The VX1000 device driver assumes exclusive ownership of the overlay unit.
 *  \trace       CREQ-870
 *********************************************************************************************************************/
#   define   VX1000If_IsCalWakeupActive            VX1000IF_IMPL(IS_CAL_WAKEUP_ACTIVE     , VX1000IF_RET_E_NOT_OK)
/********************** VX1000 Overlay END ***************************************************************************/

/********************** VX1000 Resource Management BEGIN *************************************************************/

/**********************************************************************************************************************
 *  VX1000If_EnableAccess()
 *********************************************************************************************************************/
/*! \brief       Macro to enable the VX1000 tool access.
 *  \details     Makes the VX1000 driver enable the VX1000 tool access.
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-833
 *********************************************************************************************************************/
#   define   VX1000If_EnableAccess                 VX1000IF_IMPL_VOID(ENABLE_ACCESS)

/**********************************************************************************************************************
 *  VX1000If_DisableAccess() / VX1000If_DisableAccessVoid()
 *********************************************************************************************************************/
/*! \brief       Macro to disable the VX1000 tool access.
 *  \details     Disables the VX1000 tool access.
 *  \return      0 - VX1000 tool access successfully disabled
 *  \return      1 - unable to disable VX1000 tool access
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-834
 *********************************************************************************************************************/
#   define   VX1000If_DisableAccess                VX1000IF_IMPL(DISABLE_ACCESS           , VX1000IF_RET_E_NOT_OK)
#   define   VX1000If_DisableAccessVoid            VX1000IF_IMPL_VOID(DISABLE_ACCESS_VOID)

/**********************************************************************************************************************
 *  VX1000If_IsAccessDisabled()
 *********************************************************************************************************************/
/*! \brief       Macro to check the VX1000 tool access state.
 *  \details     Makes the VX1000 driver check whether the VX1000 tool access is disabled or not.
 *  \return      0 - VX1000 tool access disabled
 *  \return      1 - VX1000 tool access enabled
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-835
 *********************************************************************************************************************/
#   define   VX1000If_IsAccessDisabled             VX1000IF_IMPL(IS_ACCESS_DISABLED       , VX1000IF_RET_E_NOT_OK)

/********************** VX1000 Resource Management END ***************************************************************/

/********************** VX1000 User functions and macros *************************************************************/

/**********************************************************************************************************************
 *  VX1000If_DetectFklRequests()
 *********************************************************************************************************************/
/*! \brief       Macro to prevents the application from writing to RAM.
 *  \details     Makes the VX1000 driver prevent the application from writing to RAM to allow a flash kernel download 
 *               by the VX1000 and to busily wait for trigger command to jump to the flash kernel execution start address 
 *               provided by the VX1000.
 *  \pre         VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-837
 *********************************************************************************************************************/
#   define   VX1000If_DetectFklRequests            VX1000IF_IMPL_VOID(DETECT_FKL_REQUESTS)

/**********************************************************************************************************************
 *  VX1000If_DeviceDetected()
 *********************************************************************************************************************/
/*! \brief       Macro to check whether a VX1000 was already detected.
 *  \details     Makes the VX1000 driver check whether a VX1000 has been detected.
 *  \return      0 - not detected
 *  \return      1 - detected
 *  \return      VX1000IF_RET_E_NOT_OK - while VX1000If_IsVX1000DriverAccessEnabled returned FALSE
 *  \pre         VXWRAP_VX1000_DETECT_VX_ASYNC_START and VX1000If_InitAsyncStart must have been called. 
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-820
 *********************************************************************************************************************/
#   define   VX1000If_DeviceDetected               VX1000IF_IMPL(DETECTED                 , VX1000IF_RET_E_NOT_OK)

/**********************************************************************************************************************
 *  VX1000If_DynAddrSig_UpdateAddress()
 *********************************************************************************************************************/
/*! \brief       This API function is not described here. The appropriate VX1000 AppDriver Addons supplement to the 
 *               VX1000If Technical Reference can be obtained via VXsupport@vector.com.
 * Internal comment removed. *
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
 *********************************************************************************************************************/ 
#   define   VX1000If_DynAddrSig_UpdateAddress     VX1000IF_IMPL_VOID(DYNADDRSIG_UPDATEADDRESS)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#   define   VX1000IF_START_SEC_VAR_ZERO_INIT_8BIT
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
extern VAR(uint8, VX1000IF_VAR_ZERO_INIT) VX1000If_State;

/*! Counter for errors that are caused by API calls when :
 *     - VX1000 Interface is in incorrect state (e.g. not initialized) 
 *     - or if VX1000If_IsVX1000DriverAccessEnabled returned FALSE 
 */
extern VAR(uint8, VX1000IF_VAR_ZERO_INIT) VX1000If_ErrorCount;

#   define   VX1000IF_STOP_SEC_VAR_ZERO_INIT_8BIT
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#   define   VX1000IF_START_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  VX1000If_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VX1000IF_CODE) VX1000If_InitMemory(void);

/**********************************************************************************************************************
 * VX1000If_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module VX1000If. It initializes all variables and sets the module state
 *               to initialized.
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         VX1000If_InitMemory has been called unless VX1000If_State is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This service does not initialize the VX1000 device driver by calling VX1000If_InitAsyncStart(). 
 *               This must be done by the application.
 *  \trace       CREQ-811
 *********************************************************************************************************************/
FUNC(void, VX1000IF_CODE) VX1000If_Init(void);

#   define   VX1000IF_STOP_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#  endif /* VX1000If_IsVX1000DriverAccessEnabled */
# endif /* !VX1000IF_CFG_NO_TARGET */
#endif /* !VX1000IF_H */

/**********************************************************************************************************************
 *  END OF FILE: VX1000If.h
 *********************************************************************************************************************/
