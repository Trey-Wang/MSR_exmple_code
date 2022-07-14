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
 *        \brief  vIpcMemCp types header file
 *
 *      \details  Contains the type definitions of the module.
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMCP_TYPES_H)                                                                                        /* PRQA S 0883 */ /* MD_vIpcMemCp_0883 */
# define VIPCMEMCP_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! \brief Type for service IDs.
 */
typedef uint8 vIpcMemCp_ServiceIdType;


/*! \brief Type for the error codes.
 */
typedef uint8 vIpcMemCp_ErrorCodeType;


/*! \brief Type to address a channel.
 */
typedef uint8_least vIpcMemCp_ChannelIndexType;


/*! \brief Type to store a position in a queue.
 */
typedef uint32 vIpcMemCp_QueuePosType;


/*! \brief Type to store buffer size.
 */
typedef uint32 vIpcMemCp_BufferSizeType;

/*! \brief Type to use for buffer location.
 */
typedef P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) vIpcMemCp_BufferPtrType;

/*! \brief Data structure for the management of a single queue.
 */
typedef struct
{
   /*! Offset to the free space for the next message. The value is used from both sides. */
   vIpcMemCp_QueuePosType Head;

   /*! Offset to the next free data block during current transmission. The value is only used by the sender. */
   vIpcMemCp_QueuePosType HeadReserved;

   /*! Offset to the next receivable message. The value is used from both sides. */
   vIpcMemCp_QueuePosType Tail;

   /*! Offset to the next receivable data block during current transmission. The value is only used by the receiver. */
   vIpcMemCp_QueuePosType TailReserved;

   /*! Flag to mark if the sender part is initialized. */
   boolean HeadInitialized;

   /*! Flag mark if the receiver part is initialized. */
   boolean TailInitialized;
}vIpcMemCp_QueueType;


/*! \brief Pointer to the queue management structure.
 */
typedef P2VAR(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) vIpcMemCp_QueuePtrType;


/*! \brief Header preceding each message in the queue.
 */
typedef struct
{
   /*! Size of the message in bytes. */
   uint32 Size;
}vIpcMemCp_MessageHeaderType;


/*! \brief Contains the description of a single continuous data block.
 */
typedef struct
{
   /*! Pointer to the begin of the data block. */
   P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) Ptr;

   /*! Data block size. */
   uint32 Size;
}vIpcMemCp_BufferDescriptionType;


#endif /* VIPCMEMCP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp_Types.h
 *********************************************************************************************************************/
