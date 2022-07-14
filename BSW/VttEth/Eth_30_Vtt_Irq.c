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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Eth_30_Vtt_Irq.c
 *        \brief  Interrupt handlers
 *
 *      \details  Contains the interrupt handlers provided by the driver and to be called by the VTT OS.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
 
#define ETH_30_VTT_IRQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Eth_30_Vtt_LL.h"
#include "Eth_30_Vtt.h"
#include "Eth_30_Vtt_Int.h"
#if (ETH_30_VTT_INTERRUPT_CATEGORY == ETH_30_VTT_ISR_CATEGORY_2)
# include "Os.h"
#endif
#include "VttCntrl_Eth.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (ETH_30_VTT_SW_MAJOR_VERSION != 6U) ||   \
      (ETH_30_VTT_SW_MINOR_VERSION != 0U) ||   \
      (ETH_30_VTT_SW_PATCH_VERSION != 1U)    )
# error "Eth_30_Vtt_Irq.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  EthIsrRx_0
 **********************************************************************************************************************/
/*! \brief      Ethernet reception interrupt of controller 0
 **********************************************************************************************************************/
#if (ETH_30_VTT_ENABLE_RX_INTERRUPT == STD_ON)
# if (ETH_30_VTT_INTERRUPT_CATEGORY == ETH_30_VTT_ISR_CATEGORY_2)
ISR ( EthIsrRx_0 )
# else
FUNC(void, ETH_30_VTT_CODE_ISR) EthIsrRx_0(void) /* PRQA S 3408 */ /* MD_Eth_30_Vtt_3408 */
# endif
{
  Eth_30_Vtt_RxIrqHdlr(0);
}
#endif /* ETH_30_VTT_ENABLE_RX_INTERRUPT || ETH_30_VTT_ENABLE_TX_INTERRUPT */
  
 /***********************************************************************************************************************
 *  EthIsrTx_0
 **********************************************************************************************************************/
/*! \brief      Ethernet transmission interrupt of controller 0
 **********************************************************************************************************************/
#if (ETH_30_VTT_ENABLE_TX_INTERRUPT == STD_ON)
# if (ETH_30_VTT_INTERRUPT_CATEGORY == ETH_30_VTT_ISR_CATEGORY_2)
ISR ( EthIsrTx_0 )
# else
FUNC(void, ETH_30_VTT_CODE_ISR) EthIsrTx_0(void) /* PRQA S 3408 */ /* MD_Eth_30_Vtt_3408 */
# endif
{
  Eth_30_Vtt_TxConfirmation(0);
}
#endif /* ETH_30_VTT_ENABLE_RX_INTERRUPT || ETH_30_VTT_ENABLE_TX_INTERRUPT */

 
#define ETH_30_VTT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */


/* module specific MISRA deviations:
 MD_Eth_30_Vtt_3408
      Reason:     Interrupt service routine shall not be accessed from extern.
      Risk:       There is no risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_Irq.c
 *********************************************************************************************************************/

