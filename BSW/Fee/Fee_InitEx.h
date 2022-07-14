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
 *         File:  Fee_InitEx.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  This File provides only declarations, necessary to use extended initialization, Fee_InitEx.
 *                It helps preventing FEE users (who include Fee.h) from getting MISRA violations,
 *                 because of incomplete structure
 *                "struct Fee_ConfigStruct" (Rule 18.1); very most of them will not even refer to it (nor to Fee_InitEx)
 *********************************************************************************************************************/

#if !defined (FEE_INITEX_H)
# define FEE_INITEX_H

# define FEE_INITEX_MAJOR_VERSION 0x08u
# define FEE_INITEX_MINOR_VERSION 0x01u
# define FEE_INITEX_PATCH_VERSION 0x02u

#include "Fee_Types.h"

/* It is intended to have opaque data types, in order to increase encapsulation and hiding of information
 * Nobody needs to know exact definition of the structures, in order to point to them, unless she accesses it.
 * E.g. a Fee's user using Fee_InitEx(ptr_to_config) shall not de-reference the config.
 * In fact, she does not even need to know, the defintion of Fee_ConfigStruct.
 *
 * Fee_ConfigRefPubType is the very same pointer type as Fee_ConfigRefTypeInternal.
 * But the former one is publicly visible, while the other should not be.
 * Chose different names to avoid clashes, when both typedefs are visible */
typedef P2CONST(struct Fee_ConfigStruct, AUTOMATIC, FEE_APPL_CONFIG) Fee_ConfigRefType;


/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/
# define FEE_START_SEC_APPL_CONFIG_UNSPECIFIED
# include "MemMap.h"

  /*! Extern declaration of FEE configuration.
   *  Users need it, if and only if they want to call Fee_InitEx */
  extern CONST(struct Fee_ConfigStruct, FEE_APPL_CONFIG) Fee_Config;

# define FEE_STOP_SEC_APPL_CONFIG_UNSPECIFIED
# include "MemMap.h"

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */


  /**********************************************************************************************************************
 * Fee_InitEx
 *********************************************************************************************************************/
/*!
 * \brief      Initialization of module Fee and resetting of the state machine.
 * \details    Initialization of module Fee and resetting of the state machine
 * \param[in,out] ConfigPtr
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, FEE_API_CODE) Fee_InitEx(Fee_ConfigRefType ConfigPtr);

# define FEE_STOP_SEC_CODE
# include "MemMap.h"

#endif

/**********************************************************************************************************************
 *  END OF FILE: Fee_InitEx.h
 *********************************************************************************************************************/
