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
/**        \file
 *        \brief  vIpcMemIfCp types header file
 *
 *      \details  Contains the type definitions of the module.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemIfCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMIFCP_TYPES_H)                                                                                      /* PRQA S 0883 */ /* MD_vIpcMemIfCp_0883 */
# define VIPCMEMIFCP_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "ComStack_Types.h"

# include "vIpcMemCp.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Type for service IDs. */
typedef uint8 vIpcMemIfCp_ServiceIdType;

/*! Type for the error codes. */
typedef uint8 vIpcMemIfCp_ErrorCodeType;


/*! Type to store the initialization status of the module. */
typedef boolean vIpcMemIfCp_InitStatusType;

/*! Type provided by the lower layer to decribe a buffer segment. */
typedef vIpcMemCp_BufferDescriptionType vIpcMemIfCp_LL_BufferDescriptionType;


/*! Function pointer type for user's notification callback. */
typedef void (*vIpcMemIfCp_UserTxNotificationCallbackType)(PduIdType ChannelId);

/*! Type to store the processing state of the transmission. */
typedef enum
{
   VIPCMEMIFCP_TX_IDLE,
   VIPCMEMIFCP_TX_COPY_PENDING,
   VIPCMEMIFCP_TX_CONFIRMATION_PENDING
} vIpcMemIfCp_TxProcessingStateType;

/*! Type to store the processing state of the reception. */
typedef enum
{
   VIPCMEMIFCP_RX_IDLE,
   VIPCMEMIFCP_RX_WAIT_FOR_BUFFER,
   VIPCMEMIFCP_RX_COPY_PENDING
} vIpcMemIfCp_RxProcessingStateType;

/*! Type to decribe the current state of a Tx channel. */
typedef struct
{
   /*! Channel state. */
   vIpcMemIfCp_TxProcessingStateType State;

   /*! Number of bytes which are yet to be copied by the upper layer. */
   uint32 RestSize;

   /*! The buffer description provided by the lower layer. */
   vIpcMemIfCp_LL_BufferDescriptionType BufferDesc;
} vIpcMemIfCp_TxChannelStateType;

/*! Type to decribe the current state of an Rx channel. */
typedef struct
{
   /*! Channel state. */
   vIpcMemIfCp_RxProcessingStateType State;

   /*! Number of bytes which are yet to be copied by the upper layer. */
   uint32 RestSize;

   /*! The buffer description provided by the lower layer. */
   vIpcMemIfCp_LL_BufferDescriptionType BufferDesc;
} vIpcMemIfCp_RxChannelStateType;


#endif /* VIPCMEMIFCP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp_Types.h
 *********************************************************************************************************************/
