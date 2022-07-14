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
/**       \file
 *        \brief  vIpc_Priv header file
 *
 *        \details ----
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
#ifndef VIPC_PRIV_H
# define VIPC_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpc.h"
#include "vIpc_XCfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define VIPC_MSG_HEADER_SIZE_SF 2u
#define VIPC_MSG_HEADER_SIZE_FF 6u
#define VIPC_MSG_HEADER_SIZE_CF 2u
#define VIPC_MSG_HEADER_SIZE_LF 2u

#define VIPC_MSG_SF 0x00u
#define VIPC_MSG_FF 0x01u
#define VIPC_MSG_CF 0x02u
#define VIPC_MSG_LF 0x03u

#define VIPC_MSG_ADDRESS_POS 0u
#define VIPC_MSG_HEADERTYPE_POS 1u
#define VIPC_MSG_HEADERTYPE_MASK 0xf0u
#define VIPC_MSG_HEADERTYPE_MASK_NEG 0x0fu
#define VIPC_MSG_LENGTH_POS 2u
#define VIPC_MSG_SEQCTR_POS 1u
#define VIPC_MSG_SEQCTR_MASK 0x0fu
#define VIPC_MSG_SEQCTR_MASK_NEG 0xf0u




/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vIpc_Msg_SetLength()
 *********************************************************************************************************************/
/*! \brief       Sets the length of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \param[in]   Length             The new connection data length.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetLength(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint32 Length);

/**********************************************************************************************************************
 *  vIpc_Msg_GetLength()
 *********************************************************************************************************************/
/*! \brief       Gets the length of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \return      The length of the message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint32, VIPC_CODE)
  vIpc_Msg_GetLength(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  vIpc_Msg_SetSeqCtr()
 *********************************************************************************************************************/
/*! \brief       Sets the sequence counter of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \param[in]   Counter            The new connection counter.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetSeqCtr(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint8 Counter);

/**********************************************************************************************************************
 *  vIpc_Msg_GetSeqCtr()
 *********************************************************************************************************************/
/*! \brief       Gets the sequence counter of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \return      The sequence counter of the message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint8, VIPC_CODE)
  vIpc_Msg_GetSeqCtr(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);


/**********************************************************************************************************************
 *  vIpc_Msg_SetType()
 *********************************************************************************************************************/
/*! \brief       Sets the type of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \param[in]   Type            The new connection type.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetType(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, vIpc_MessageType Type);


/**********************************************************************************************************************
 *  vIpc_Msg_GetType()
 *********************************************************************************************************************/
/*! \brief       Gets the message type of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \return      The type of the message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(vIpc_MessageType, VIPC_CODE)
  vIpc_Msg_GetType(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);


/**********************************************************************************************************************
 *  vIpc_Msg_SetAddress()
 *********************************************************************************************************************/
/*! \brief       Sets the address of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \param[in]   Address            The new address.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetAddress(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint8 Address);


/**********************************************************************************************************************
 *  vIpc_Msg_GetAddress()
 *********************************************************************************************************************/
/*! \brief       Gets the address of a message.
 *  \details     -
 *  \param[in]   PduInfoPtr         The connection data.
 *  \return      The address of the message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint8, VIPC_CODE)
  vIpc_Msg_GetAddress(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_Msg_SetLength()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetLength(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint32 Length)
{
  SduDataPtrType data           = PduInfoPtr->SduDataPtr;
  data[VIPC_MSG_LENGTH_POS]     = (uint8)((Length >> 24u) & 0xff);
  data[VIPC_MSG_LENGTH_POS + 1] = (uint8)((Length >> 16u) & 0xff);
  data[VIPC_MSG_LENGTH_POS + 2] = (uint8)((Length >> 8u) & 0xff);
  data[VIPC_MSG_LENGTH_POS + 3] = (uint8)(Length & 0xff);
}

/**********************************************************************************************************************
 * vIpc_Msg_GetLength()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint32, VIPC_CODE)
  vIpc_Msg_GetLength(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;

  uint32 first  = (uint32)(data[VIPC_MSG_LENGTH_POS] << 24u);
  uint32 second = (uint32)(data[VIPC_MSG_LENGTH_POS + 1] << 16u);
  uint32 third  = (uint32)(data[VIPC_MSG_LENGTH_POS + 2] << 8u);
  uint32 fourth = (uint32)(data[VIPC_MSG_LENGTH_POS + 3]);

  return first | second | third | fourth;
}

/**********************************************************************************************************************
 * vIpc_Msg_SetSeqCtr()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetSeqCtr(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint8 Counter)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;
  data[VIPC_MSG_SEQCTR_POS] =
    (uint8)((data[VIPC_MSG_SEQCTR_POS] & VIPC_MSG_SEQCTR_MASK_NEG) | (Counter & VIPC_MSG_SEQCTR_MASK));
}

/**********************************************************************************************************************
 * vIpc_Msg_GetSeqCtr()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint8, VIPC_CODE)
  vIpc_Msg_GetSeqCtr(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;

  return (uint8)(data[VIPC_MSG_SEQCTR_POS] & VIPC_MSG_SEQCTR_MASK);
}

/**********************************************************************************************************************
 * vIpc_Msg_SetType()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetType(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, vIpc_MessageType Type)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;
  data[VIPC_MSG_HEADERTYPE_POS] =
    (uint8)(((uint8)(data[VIPC_MSG_HEADERTYPE_POS]) & VIPC_MSG_HEADERTYPE_MASK_NEG) | ((uint8)(Type << 4u)));
}

/**********************************************************************************************************************
 * vIpc_Msg_GetType()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(vIpc_MessageType, VIPC_CODE)
  vIpc_Msg_GetType(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;

  return (uint8)((data[VIPC_MSG_HEADERTYPE_POS] & VIPC_MSG_HEADERTYPE_MASK) >> 4u);
}

/**********************************************************************************************************************
 * vIpc_Msg_SetAddress()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(void, VIPC_CODE)
  vIpc_Msg_SetAddress(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr, uint8 Address)
{
  SduDataPtrType data        = PduInfoPtr->SduDataPtr;
  data[VIPC_MSG_ADDRESS_POS] = Address;
}

/**********************************************************************************************************************
 * vIpc_Msg_GetAddress()
 *********************************************************************************************************************/
VIPC_ALWAYS_INLINE FUNC(uint8, VIPC_CODE)
  vIpc_Msg_GetAddress(P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  SduDataPtrType data = PduInfoPtr->SduDataPtr;
  return (uint8)(data[VIPC_MSG_ADDRESS_POS]);
}

#endif /* VIPC_PRIV_H  */
