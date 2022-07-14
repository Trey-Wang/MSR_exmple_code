/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Fr_Irq.c
 *        \brief  FlexRay Driver IRQ source file
 *
 *      \details  Implementation of FlexRay Driver Interrupt functions according to:
 *                AUTOSAR FlexRay Driver, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define FR_IRQ_SOURCE

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0777 EOF */ /* MD_MSR_5.1_777 */
/* PRQA S 0303 EOF */ /* MD_Fr_0306 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Fr.h"

#if (FR_CFG_ISR_TYPE == FR_ISR_CAT2)  /* COV_FR_UNSUPPORTED_ISR_TYPE */
# include "Os.h"
#endif

#if (FR_CFG_APPL_CALLBACK_TIMER0 == STD_OFF)
# include "FrIf.h"
#endif

/* PRQA S 1503 EOF */ /* MD_MSR_14.1 */

/*lint -e506 */ /* Suppress ID506 due to MD_MSR_14.1 */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check consistency of source and header file. */
#if ((FR_SW_MAJOR_VERSION != 0x5u) || (FR_SW_MINOR_VERSION != 0x00u) || (FR_SW_PATCH_VERSION != 0x01u))
# error "Fr_Irq.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define FR_START_SEC_CODE_ISR
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

# if (FR_CFG_APPL_CALLBACK_CYCLE_START == STD_ON) /* COV_FR_TESTSUITE_INSTRUMENTATION */
/***********************************************************************************************************************
 *  Fr_IrqLine0
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
#  if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
#   if(FR_CFG_STATUS_ISR_LINE == 0)
FUNC(void, FR_CODE_ISR) Fr_IrqLine1(void) {}
FUNC(void, FR_CODE_ISR) Fr_IrqLine0(void)
#   else
FUNC(void, FR_CODE_ISR) Fr_IrqLine0(void) {}
FUNC(void, FR_CODE_ISR) Fr_IrqLine1(void)
#   endif
#  else
#   if(FR_CFG_STATUS_ISR_LINE == 0)
ISR(Fr_IrqLine1)  {}
ISR(Fr_IrqLine0)  
#   else
ISR(Fr_IrqLine0)  {}
ISR(Fr_IrqLine1)  
#   endif
#  endif
{
  /* #10 Call the status handling ISR of the FR driver to handle a cycle start interrupt. */
  Fr_ISR_Status(FR_COM_CONTROLLER_0);
}
# endif
/***********************************************************************************************************************
 *  Fr_IrqTimer0
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
# if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
ISR(Fr_IrqTimer0)
# else
FUNC(void, FR_CODE_ISR) Fr_IrqTimer0(void)
# endif
{
  /* #10 If FR_CFG_APPL_CALLBACK_TIMER0 is enabled:
   *      Call the Timer0 application callback. */
# if (FR_CFG_APPL_CALLBACK_TIMER0 == STD_ON)
  ApplFr_ISR_Timer0();
# else
#  if defined (VGEN_ENABLE_IF_ASRIFFR) /* COV_FR_TESTSUITE_INSTRUMENTATION */
  /* #20 Otherwise:
   *       Directly trigger the FRIF job list execution. */
  FrIf_JobListExec_0();
#  endif
# endif
}

# if (FR_NUM_CTRL_USED > 1)
#  if (FR_CFG_APPL_CALLBACK_CYCLE_START == STD_ON) /* COV_FR_TESTSUITE_INSTRUMENTATION */
/***********************************************************************************************************************
*  Fr_IrqLine0_1
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
#   if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
#    if(FR_CFG_STATUS_ISR_LINE_1 == 0)
FUNC(void, FR_CODE_ISR) Fr_IrqLine1_1(void) {}
FUNC(void, FR_CODE_ISR) Fr_IrqLine0_1(void)
#    else
FUNC(void, FR_CODE_ISR) Fr_IrqLine0_1(void) {}
FUNC(void, FR_CODE_ISR) Fr_IrqLine1_1(void)
#    endif
#   else
#    if(FR_CFG_STATUS_ISR_LINE_1 == 0)
ISR(Fr_IrqLine1_1)  {}
ISR(Fr_IrqLine0_1)
#    else
ISR(Fr_IrqLine0_1)  {}
ISR(Fr_IrqLine1_1)  
#    endif
#   endif
{
	/* #10 Call the status handling ISR of the FR driver to handle a cycle start interrupt. */
	Fr_ISR_Status(FR_COM_CONTROLLER_1);
}
#  endif
/***********************************************************************************************************************
*  Fr_IrqTimer0_1
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
 *
*/
#  if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
ISR(Fr_IrqTimer0_1)
#  else
FUNC(void, FR_CODE_ISR) Fr_IrqTimer0_1(void)
#  endif
{
	/* #10 If FR_CFG_APPL_CALLBACK_TIMER0 is enabled:
	*      Call the Timer0 application callback. */
#  if (FR_CFG_APPL_CALLBACK_TIMER0 == STD_ON)
	ApplFr_ISR_Timer0_1();
#  else
#   if defined (VGEN_ENABLE_IF_ASRIFFR) /* COV_FR_TESTSUITE_INSTRUMENTATION */
	/* #20 Otherwise:
	*       Directly trigger the FRIF job list execution. */
	FrIf_JobListExec_1();
#   endif
#  endif
}
# endif

#define FR_STOP_SEC_CODE_ISR
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

/**********************************************************************************************************************
 *  END OF FILE: Fr_Irq.c
 *********************************************************************************************************************/
