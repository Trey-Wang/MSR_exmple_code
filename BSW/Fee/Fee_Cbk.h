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
 *         File:  Fee_Cbk.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined (FEE_CBK_H)
# define FEE_CBK_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_CBK_MAJOR_VERSION                                        (8u)
# define FEE_CBK_MINOR_VERSION                                        (0u)
# define FEE_CBK_PATCH_VERSION                                        (2u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define FEE_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
 * Fee_JobErrorNotification
 *********************************************************************************************************************/
/*!
 * \brief      Called by Flash driver for reporting an error in job processing
 * \details    Called by Flash driver for reporting an error in job processing
 * \pre        Module is initialized
 * \context    TASK
 * \reentrant  FALSE
 */
FUNC(void, FEE_API_CODE) Fee_JobErrorNotification(void);


/**********************************************************************************************************************
 * Fee_JobEndNotification
 *********************************************************************************************************************/
/*!
 * \brief      Called by Flash driver for reporting a successful job processing
 * \details    Called by Flash driver for reporting a successful job processing
 * \pre        Module is initialized
 * \context    TASK
 * \reentrant  FALSE
 */
FUNC(void, FEE_API_CODE) Fee_JobEndNotification(void);

# define FEE_STOP_SEC_CODE
# include "MemMap.h"
#endif /* FEE_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: Fee_Cbk.h
 *********************************************************************************************************************/
