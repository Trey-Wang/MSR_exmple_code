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
 *        \brief  vIpcMemCp header file
 *
 *      \details  Contains the declaration and definition of the module's internal functions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMCP_INT_H)                                                                                          /* PRQA S 0883 */ /* MD_vIpcMemCp_0883 */
# define VIPCMEMCP_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemCp.h"
# include "vIpcMemCp_PBcfg.h"

# if (VIPCMEMCP_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

# if (VIPCMEMCP_USE_INIT_POINTER == STD_ON)
#  include "EcuM_Error.h"
# else
#  define EcuM_BswErrorHook(x, y)
#  define vIpcMemCp_GetFinalMagicNumber()  (0)
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (VIPCMEMCP_LOCAL)
#  define VIPCMEMCP_LOCAL static
# endif

# if !defined (VIPCMEMCP_LOCAL_INLINE)
#  define VIPCMEMCP_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern P2CONST(vIpcMemCp_ConfigType, VIPCMEMCP_VAR_ZERO_INIT, VIPCMEMCP_PBCFG) vIpcMemCp_ConfigDataPtr;

# define VIPCMEMCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemCp_UseInitPointer()
 *********************************************************************************************************************/
/*! \brief       Returns whether the usage of init pointer is enabled.
 *  \details     -
 *  \return      TRUE if UseInitPointer is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_UseInitPointer
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_IsFinalMagicNumberOn()
 *********************************************************************************************************************/
/*! \brief       Returns whether the final magic number generation is active.
 *  \details     -
 *  \return      TRUE if FinalMagicNumber is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_IsFinalMagicNumberOn
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_UseEcumBswErrorHook()
 *********************************************************************************************************************/
/*! \brief       Returns whether the usage EcuM Bsw ErrorHook is enabled.
 *  \details     -
 *  \return      TRUE if UseEcumBswErrorHook is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_UseEcumBswErrorHook
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_DevErrorDetect()
 *********************************************************************************************************************/
/*! \brief       Returns whether DevErrorDetect is enabled or not.
 *  \details     -
 *  \return      TRUE if DevErrorDetect is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_DevErrorDetect
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_DevErrorReport()
 *********************************************************************************************************************/
/*! \brief       Returns whether DevErrorReport is enabled or not.
 *  \details     -
 *  \return      TRUE if DevErrorReport is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_DevErrorReport
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_IsByteOrderLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Returns whether little endian byte order is enabled or not.
 *  \details     -
 *  \return      TRUE if ByteorderLittleEndian is on, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_IsByteOrderLittleEndian
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemCp_CheckFinalMagicNumber()
 *********************************************************************************************************************/
/*! \brief       Returns the check failed or not.
 *  \details     -
 *  \return      TRUE if FinalMagicNumber is invalid, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_CheckFinalMagicNumber
(
   void
);

/**********************************************************************************************************************
 * vIpcMemCp_ReportError()
 *********************************************************************************************************************/
/*! \brief         Calls Det_ReportError() if configured and an error has occured.
 *  \details       -
 *  \param[in]     ApiId                  Id of the API service.
 *  \param[in]     ErrorId                Error code.
 *  \return        E_OK if there was no error, E_NOT_OK otherwise.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_ReportError
(
   vIpcMemCp_ServiceIdType ApiId,
   vIpcMemCp_ErrorCodeType ErrorId
);

/**********************************************************************************************************************
 * vIpcMemCp_InvertEndianess()
 *********************************************************************************************************************/
/*! \brief         Returns the value after inverted byte order.
 *  \details       -
 *  \param[in]     Value                 Value to be changed.
 *  \return        Value with inverted byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePosType, VIPCMEMCP_CODE)
vIpcMemCp_InvertEndianess
(
   uint32 Value
);

/**********************************************************************************************************************
 * vIpcMemCp_ReadQueuePointer()
 *********************************************************************************************************************/
/*! \brief         Returns the value of a queue position.
 *  \details       -
 *  \param[in]     QueuePosPtr             Pointer to the queue position. Parameter must not be NULL_PTR.
 *  \return        Queue position value.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePosType, VIPCMEMCP_CODE)
vIpcMemCp_ReadQueuePointer
(
   P2CONST(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_CONST) QueuePosPtr
);

/**********************************************************************************************************************
 * vIpcMemCp_WriteQueuePointer()
 *********************************************************************************************************************/
/*! \brief         Stores a new value as a queue position.
 *  \details       -
 *  \param[in]     QueuePosPtr             Pointer to the queue position. Parameter must not be NULL_PTR.
 *  \param[in]     Value                   The new value.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_WriteQueuePointer
(
   P2VAR(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) QueuePosPtr,
   vIpcMemCp_QueuePosType Value
);

/**********************************************************************************************************************
 * vIpcMemCp_ReadMessageHeader()
 *********************************************************************************************************************/
/*! \brief         Retrieves the message header from the buffer.
 *  \details       -
 *  \param[in]     BufferMsgPtr            Pointer to the message header in the buffer. Parameter must not be NULL_PTR.
 *  \param[out]    MsgPtr                  Pointer the messager header to store into. Parameter must not be NULL_PTR.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_ReadMessageHeader
(
   P2CONST(uint8, AUTOMATIC, VIPCMEMCP_CONST) BufferMsgPtr,
   P2VAR(vIpcMemCp_MessageHeaderType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) MsgPtr
);

/**********************************************************************************************************************
 * vIpcMemCp_WriteMessageHeader()
 *********************************************************************************************************************/
/*! \brief         Writes the message header into the buffer.
 *  \details       -
 *  \param[in]     BufferMsgPtr            Pointer to the message header in the buffer. Parameter must not be NULL_PTR.
 *  \param[in]     MsgPtr                  Pointer the messager header to get data from. Parameter must not be NULL_PTR.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_WriteMessageHeader
(
   P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferMsgPtr,
   P2CONST(vIpcMemCp_MessageHeaderType, AUTOMATIC, VIPCMEMCP_CONST) MsgPtr
);

/**********************************************************************************************************************
 * vIpcMemCp_QueueIsInitialized()
 *********************************************************************************************************************/
/*! \brief         Returns if the queue was fully initialized.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \return        TRUE if the queue is initialized, FALSE otherwise.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_QueueIsInitialized
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
);

/**********************************************************************************************************************
 * vIpcMemCp_QueueIsEmpty()
 *********************************************************************************************************************/
/*! \brief         Returns if the queue contains no messages.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \return        TRUE if the queue is empty, FALSE otherwise.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_QueueIsEmpty
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
);

/**********************************************************************************************************************
 * vIpcMemCp_QueueGetDistance()
 *********************************************************************************************************************/
/*! \brief         Returns the distance of two positions with a optional wrap-around.
 *  \details       -
 *  \param[in]     FrontPos                The position to be considered in front.
 *  \param[in]     RearPos                 The position to be consisered in rear.
 *  \param[in]     BufferSize              The wrap-around position.
 *  \return        The distance in in bytes.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_QueueGetDistance
(
   vIpcMemCp_QueuePosType FrontPos,
   vIpcMemCp_QueuePosType RearPos,
   uint32 BufferSize
);

/**********************************************************************************************************************
 * vIpcMemCp_QueueLeft()
 *********************************************************************************************************************/
/*! \brief         Returns the available free space size in the queue.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \param[in]     BufferSize              Size of the underlying buffer.
 *  \return        Free space size in bytes.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_QueueLeft
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
);

/**********************************************************************************************************************
 * vIpcMemCp_QueueGetCorrectedPos()
 *********************************************************************************************************************/
/*! \brief         Returns the value of a queue position after an optional wrap-around correction.
 *  \details       -
 *  \param[in]     QueuePosPtr             Pointer to the queue position. Parameter must not be NULL_PTR.
 *  \param[in]     BufferSize              Size of the underlying buffer.
 *  \return        Corrected queue position value.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePosType, VIPCMEMCP_CODE)
vIpcMemCp_QueueGetCorrectedPos
(
   P2CONST(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_CONST) QueuePosPtr,
   vIpcMemCp_BufferSizeType BufferSize
);

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemCp_UseInitPointer()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_UseInitPointer
(
   void
)
{
   return (boolean)(VIPCMEMCP_USE_INIT_POINTER == STD_ON);
} /* vIpcMemCp_UseInitPointer */

/**********************************************************************************************************************
 *  vIpcMemCp_IsFinalMagicNumberOn()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_IsFinalMagicNumberOn
(
   void
)
{
   return (boolean)(VIPCMEMCP_FINALMAGICNUMBER == STD_ON);
} /* vIpcMemCp_IsFinalMagicNumberOn */

/**********************************************************************************************************************
 *  vIpcMemCp_UseEcumBswErrorHook()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_UseEcumBswErrorHook
(
   void
)
{
   return (boolean)(VIPCMEMCP_USE_ECUM_BSW_ERROR_HOOK == STD_ON);
} /* vIpcMemCp_UseEcumBswErrorHook */

/**********************************************************************************************************************
 *  vIpcMemCp_DevErrorDetect()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_DevErrorDetect
(
   void
)
{
   return (boolean)(VIPCMEMCP_DEV_ERROR_DETECT == STD_ON);
} /* vIpcMemCp_DevErrorDetect */

/**********************************************************************************************************************
 *  vIpcMemCp_DevErrorReport()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_DevErrorReport
(
   void
)
{
   return (boolean)(VIPCMEMCP_DEV_ERROR_REPORT == STD_ON);
} /* vIpcMemCp_DevErrorReport */

/**********************************************************************************************************************
 *  vIpcMemCp_IsByteOrderLittleEndian()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_IsByteOrderLittleEndian
(
   void
)
{
   return (boolean)(VIPCMEMCP_BYTEORDER_LITTLE_ENDIAN == STD_ON);
} /* vIpcMemCp_IsByteOrderLittleEndian */

/**********************************************************************************************************************
 *  vIpcMemCp_CheckFinalMagicNumber()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_CheckFinalMagicNumber
(
   void
)
{
   boolean result = FALSE;
   if (vIpcMemCp_IsFinalMagicNumberOn() == TRUE)
   {
      result = (boolean)(vIpcMemCp_GetFinalMagicNumber() != VIPCMEMCP_FINAL_MAGIC_NUMBER);
   }
   return result;
} /* vIpcMemCp_CheckFinalMagicNumber */

/**********************************************************************************************************************
 * vIpcMemCp_ReportError()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_ReportError
(
   vIpcMemCp_ServiceIdType ApiId,
   vIpcMemCp_ErrorCodeType ErrorId
)
{
   Std_ReturnType result = E_OK;

   if((vIpcMemCp_DevErrorReport() == TRUE) && (ErrorId != VIPCMEMCP_E_NO_ERROR))
   {
      (void) Det_ReportError(VIPCMEMCP_MODULE_ID, VIPCMEMCP_INSTANCE_ID, ApiId, ErrorId);
      result = E_NOT_OK;
   }
   return result;
} /* vIpcMemCp_ReportError */

/**********************************************************************************************************************
 * vIpcMemCp_InvertEndianess()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_InvertEndianess
(
   uint32 Value
)
{
   uint32 result;
   result =  (Value >> 24);
   result |= (Value >> 8) & 0x0000FF00;
   result |= (Value << 8) & 0x00FF0000;
   result |= (Value << 24);

   return result;
} /* vIpcMemCp_InvertEndianess */

/**********************************************************************************************************************
 * vIpcMemCp_ReadQueuePointer()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePosType, VIPCMEMCP_CODE)
vIpcMemCp_ReadQueuePointer
(
   P2CONST(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_CONST) QueuePosPtr
)
{
   uint32 result;
   if (vIpcMemCp_IsByteOrderLittleEndian() == TRUE)
   {
      result = *QueuePosPtr;
   }
   else
   {
      result = vIpcMemCp_InvertEndianess(*QueuePosPtr);
   }

   return (vIpcMemCp_QueuePosType)result;
} /* vIpcMemCp_ReadQueuePointer */

/**********************************************************************************************************************
 * vIpcMemCp_WriteQueuePointer()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_WriteQueuePointer
(
   P2VAR(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) QueuePosPtr,
   vIpcMemCp_QueuePosType Value
)
{
   if (vIpcMemCp_IsByteOrderLittleEndian() == TRUE)
   {
      *QueuePosPtr = Value;
   }
   else
   {
      *QueuePosPtr = vIpcMemCp_InvertEndianess(Value);
   }
} /* vIpcMemCp_WriteQueuePointer */

/**********************************************************************************************************************
 * vIpcMemCp_ReadMessageHeader()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_ReadMessageHeader
(
   P2CONST(uint8, AUTOMATIC, VIPCMEMCP_CONST) BufferMsgPtr,
   P2VAR(vIpcMemCp_MessageHeaderType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) MsgPtr
)
{
   P2CONST(uint32, AUTOMATIC, VIPCMEMCP_CONST) ptr = (P2CONST(uint32, AUTOMATIC, VIPCMEMCP_CONST))BufferMsgPtr;         /* PRQA S 0310 */ /* MD_vIpcMemCp_0310 */

   if (vIpcMemCp_IsByteOrderLittleEndian() == TRUE)
   {
      MsgPtr->Size = *ptr;
   }
   else
   {
      MsgPtr->Size = vIpcMemCp_InvertEndianess(*ptr);
   }
} /* vIpcMemCp_ReadMessageHeader */

/**********************************************************************************************************************
 * vIpcMemCp_WriteMessageHeader()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(void, VIPCMEMCP_CODE)
vIpcMemCp_WriteMessageHeader
(
   P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferMsgPtr,
   P2CONST(vIpcMemCp_MessageHeaderType, AUTOMATIC, VIPCMEMCP_CONST) MsgPtr
)
{
   P2VAR(uint32, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) ptr = (P2VAR(uint32, AUTOMATIC, VIPCMEMCP_VAR_NOINIT))BufferMsgPtr;   /* PRQA S 0310 */ /* MD_vIpcMemCp_0310 */

   if (vIpcMemCp_IsByteOrderLittleEndian() == TRUE)
   {
      *ptr = MsgPtr->Size;
   }
   else
   {
      *ptr = vIpcMemCp_InvertEndianess(MsgPtr->Size);
   }
} /* vIpcMemCp_WriteMessageHeader */

/**********************************************************************************************************************
 *  vIpcMemCp_QueueIsInitialized()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_QueueIsInitialized
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
)
{
   return (boolean)((Queue->HeadInitialized == TRUE) && (Queue->TailInitialized == TRUE));
} /* vIpcMemCp_QueueIsInitialized */

/**********************************************************************************************************************
 *  vIpcMemCp_QueueIsEmpty()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_QueueIsEmpty
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
)
{
   return (boolean)(Queue->Head == Queue->Tail);
} /* vIpcMemCp_QueueIsEmpty */

/**********************************************************************************************************************
 *  vIpcMemCp_QueueGetDistance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_QueueGetDistance
(
   vIpcMemCp_QueuePosType FrontPos,
   vIpcMemCp_QueuePosType RearPos,
   uint32 BufferSize
)
{
   uint32 result;

   /* #10 If the front position is after the rear position, calcualte the free space between them. */
   if(FrontPos > RearPos)
   {
      result = (uint32)(FrontPos - RearPos);
   }
   /* #20 Otherwise: */
   else
   {
      /* #30 Calculate the free space between rear position and buffer's end. */
      result = (uint32)(BufferSize - RearPos);

      /* #40 Set it to zero if it is too small to fit minimal data size and the header. */
      if (result < (VIPCMEMCP_MIN_DATA_COPY_SIZE + sizeof(vIpcMemCp_MessageHeaderType)))
      {
         result = 0;
      }

      /* #50 Calculate the free space between front position and buffer's begin. */
      result += (uint32)(FrontPos);
   }
   return result;
}

/**********************************************************************************************************************
 *  vIpcMemCp_QueueLeft()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_QueueLeft
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
)
{
   uint32 result;

   vIpcMemCp_QueuePosType curHeadPos;
   vIpcMemCp_QueuePosType curTailPos;

   /* #10 Retrieve the positions from the queue management structure. */
   curHeadPos = vIpcMemCp_ReadQueuePointer(&(Queue->Head));
   curTailPos = vIpcMemCp_ReadQueuePointer(&(Queue->Tail));

   /* #20 Get the distance between tail and head. */
   result = vIpcMemCp_QueueGetDistance(curTailPos, curHeadPos, BufferSize);

   /* #30 Subtract one byte from the result if it is greater than zero. */
   if (result > 0)
   {
      result -= 1;
   }

   return result;
} /* vIpcMemCp_QueueLeft */

/**********************************************************************************************************************
 * vIpcMemCp_QueueGetCorrectedPos()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePosType, VIPCMEMCP_CODE)
vIpcMemCp_QueueGetCorrectedPos
(
   P2CONST(vIpcMemCp_QueuePosType, AUTOMATIC, VIPCMEMCP_CONST) QueuePosPtr,
   vIpcMemCp_BufferSizeType BufferSize
)
{
   vIpcMemCp_QueuePosType result;

   /* #10 Read the position. */
   result = vIpcMemCp_ReadQueuePointer(QueuePosPtr);

   /* #20 Wrap-around if the next header and minimal data size does not fit until end of the buffer. */
   if ((BufferSize - result) < (VIPCMEMCP_MIN_DATA_COPY_SIZE + sizeof(vIpcMemCp_MessageHeaderType)))
   {
      result = 0;
   }
   return result;
} /* vIpcMemCp_QueueGetCorrectedPos */

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMCP_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp_Int.h
 *********************************************************************************************************************/
