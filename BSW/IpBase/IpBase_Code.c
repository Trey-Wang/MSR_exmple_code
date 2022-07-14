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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  IpBase_Code.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  En- and decoding implementation
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
#define IPBASE_CODE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
#include "IpBase_Code.h"
#include "IpBase.h"
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != 3u) || (IPBASE_SW_MINOR_VERSION != 0u) || (IPBASE_SW_PATCH_VERSION != 0u) )
# error "IpBase_Code.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* IPBASE encoding constants */
#if ( IPBASE_CODE_ENABLE == STD_ON )
# define IPBASE_CODE_BASE64_EMPTY_CODE                    ((uint8) 64u)  /*!< Base64: empty digit */
# define IPBASE_CODE_BASE64_INV_CODE                      ((uint8) 65u)  /*!< Base64: invalid digit */
# define IPBASE_CODE_BASE64_LF_CHAR                       ((uint8) 0x0A) /*!< Base64: line feed digit */
# define IPBASE_CODE_BASE64_CR_CHAR                       ((uint8) 0x0D) /*!< Base64: carriage return digit */
# define IPBASE_CODE_BER_INFINITE_LEN                     ((uint8) 0U)   /*!< BER: infinite length encoding */
# define IPBASE_CODE_BER_MAX_LEN_U32                      ((uint8) 4U)   /*!< BER: max uint32 length encoding */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* IPBASE encoding extension check */
# define IPBASE_BER_IS_EXTENDED_LEN(len)  (0x80u & (len)) /* PRQA S 3453,3458 */ /* MD_MSR_19.4 MD_MSR_19.7 */

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# if ( IPBASE_CODE_BASE64_ENABLE == STD_ON )
/**********************************************************************************************************************
 *  IpBase_EncodeBase64Block
 *********************************************************************************************************************/
/*! \brief        Encodes data in base64
 *  \details      Encodes the given data block
 *  \param[in,out] TgtDataPtr     pointer for the encoded data
 *  \param[in]     SrcDataPtr     pointer to the raw data
 *  \param[in]     SrcLenByte     raw data length in bytes
 *  \return        void
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108036
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_EncodeBase64Block(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                            uint32 SrcLenByte);
/**********************************************************************************************************************
 *  IpBase_DecodeBase64Block
 *********************************************************************************************************************/
/*! \brief        Decodes base64 encoded block
 *  \details      Decodes the given data block
 *  \param[in,out] TgtDataPtr       pointer for the decoded data
 *  \param[in]     SrcDataPtr       pointer to the encoded data
 *  \param[in,out] SrcLenBytePtr    pointer for target length in bytes
 *  \param[in,out] SrcLenBytePtr    pointer for source data length in bytes
 *  \param[in]     SrcRemainingLen  remaining length of source bytes
 *  \return        void
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108036
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64Block(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                                      IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                                      IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr,
                                                                      IPBASE_P2VARAPPLDATA(uint32) SrcLenBytePtr,
                                                                      uint32 SrcRemainingLen);
# endif
  /* IPBASE_CODE_BASE64_ENABLE */
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
/**********************************************************************************************************************
 *  IpBase_BerCheckDepth
 *********************************************************************************************************************/
/*! \brief        BER depth check
 *  \details      Checks the depth of BER endoced data
 *  \param[in]    MaxDepth       maximum depth
 *  \param[in]    ElementDepth   element depth
 *  \return       E_OK           depth o.k.
 *  \return       E_NOT_OK       depth exceeded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerCheckDepth(uint8 MaxDepth, uint8 ElementDepth);
/**********************************************************************************************************************
 *  IpBase_BerResetWorkspace
 *********************************************************************************************************************/
/*! \brief        BER reset workspace
 *  \details      Resets the workspace
 *  \param[in]    WorkspacePtr   pointer to workspace
 *  \param[in]    DataPtr        pointer to data
 *  \param[in]    DataSize       size of data [bytes]
 *  \return       void
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_BerResetWorkspace(
                                   IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                   IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 DataSize);
/**********************************************************************************************************************
 *  IpBase_BerReadChapter
 *********************************************************************************************************************/
/*! \brief        BER read chapter
 *  \details      Reads one chapter try to find element
 *  \param[in]    StackElementPtr pointer to stack element
 *  \param[in]    ElementNrPtr    pointer to element number
 *  \param[in]    DataPtr         pointer to data
 *  \param[in]    Level           nesting level
 *  \return       E_OK            chapter read
 *  \return       E_NOT_OK        chapter end
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerReadChapter(
                                   IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) StackElementPtr,
                                   IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                   IPBASE_P2CONSTAPPLDATA(uint8) DataPtr,
                                   uint8 Level);
/**********************************************************************************************************************
 *  IpBase_BerSwitchChapter
 *********************************************************************************************************************/
/*! \brief         BER switch chapter
 *  \details       Switches to next chapter
 *  \param[in]     WorkspacePtr           pointer to workspace
 *  \param[in,out] StackElementPtrPtr     pointer to stack element pointer
 *  \param[in,out] NextStackElementPtrPtr pointer to next stack element pointer
 *  \param[in,out] LevelPtr               pointer to nesting level
 *  \return        E_OK                   chapter switched
 *  \return        E_NOT_OK               document end
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerSwitchChapter(
                                   IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                   IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType *) StackElementPtrPtr,
                                   IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType *) NextStackElementPtrPtr,
                                   IPBASE_P2VARAPPLDATA(uint8) LevelPtr);
/**********************************************************************************************************************
 *  IpBase_BerGetNextElement
 *********************************************************************************************************************/
/*! \brief         BER get next element
 *  \details       Gets the next element within document
 *  \param[out]    Element       retrieved element
 *  \param[in]     ElementIdx    element index
 *  \param[in]     Block         block
 *  \param[in]     Data          data
 *  \return        E_OK          element retrieved
 *  \return        E_NOT_OK      document end
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetNextElement(
                                   IPBASE_CONSTP2VARAPPLDATA(IpBase_BerElementType) Element,
                                   uint32 ElementIdx,
                                   IPBASE_CONSTP2CONSTAPPLDATA(IpBase_BerBlockType) Block,
                                   IPBASE_CONSTP2CONSTAPPLDATA(uint8) Data);
/**********************************************************************************************************************
 *  IpBase_BerGetContentSize
 *********************************************************************************************************************/
/*! \brief         BER get content size
 *  \details       Gets the size of the current content
 *  \param[in]     SizeOfLen     content size length
 *  \param[in]     ElementIdxPtr pointer with element index
 *  \param[in]     Data          data
 *  \return        length        content length [bytes]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-108038
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_BerGetContentSize(uint8 SizeOfLen, 
                                   IPBASE_CONSTP2VARAPPLDATA(uint32) ElementIdxPtr,
                                   IPBASE_CONSTP2CONSTAPPLDATA(uint8) Data);
# endif
  /* IPBASE_CODE_BER_ENABLE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_Encode
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BASE64_ENABLE == STD_ON )
IPBASE_FUNCCODE(Std_ReturnType) IpBase_Encode(uint8 Code, IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                              IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr, 
                                              uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check TgtDataPtr, SrcDataPtr, TgtLenBytePtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) || (TgtLenBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check Code (limited to BASE64 encoding) */
    if ( Code == IPBASE_CODE_BASE64 )
    {
      uint32 SrcByteIdx = 0;
      uint32 TgtByteIdx = 0;

      retVal = E_OK;
      while ( SrcLenByte > SrcByteIdx )
      { /* #30 Encode all chunks  */
        if ( (TgtByteIdx+IPBASE_CODE_BASE64_CODED_BLOCK_LEN) > (*TgtLenBytePtr) )
        { /* #40 Handle target buffer overrun */
          retVal = E_NOT_OK;
          break; 
        }
        IpBase_EncodeBase64Block(&TgtDataPtr[TgtByteIdx], &SrcDataPtr[SrcByteIdx], SrcLenByte - SrcByteIdx);
        SrcByteIdx += IPBASE_CODE_BASE64_RAW_BLOCK_LEN;
        TgtByteIdx += IPBASE_CODE_BASE64_CODED_BLOCK_LEN;
      }
      *TgtLenBytePtr = TgtByteIdx;
    }
    else
    { /* #50 Handle unknown value for Code */
      retVal = E_NOT_OK;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_ENCODE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_Encode() */
# endif
  /* IPBASE_CODE_BASE64_ENABLE */
/**********************************************************************************************************************
 *  IpBase_Decode
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BASE64_ENABLE == STD_ON )
IPBASE_FUNCCODE(Std_ReturnType) IpBase_Decode(uint8 Code, IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                              IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr, 
                                              uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check TgtDataPtr, SrcDataPtr, TgtLenBytePtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) || (TgtLenBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check Code (limited to BASE64 decoding) */
    if ( Code == IPBASE_CODE_BASE64 )
    {
      uint32 SrcByteIdx = 0;
      uint32 TgtByteIdx = 0;

      retVal = E_OK;
      while ( (SrcLenByte > SrcByteIdx) && (*TgtLenBytePtr > TgtByteIdx) )
      { /* #30 Decode all chunks  */
        retVal = IpBase_DecodeBase64Block(&TgtDataPtr[TgtByteIdx], &SrcDataPtr[SrcByteIdx], &TgtByteIdx, &SrcByteIdx,
                                          SrcLenByte - SrcByteIdx);
        if ( retVal != E_OK )
        { /* #40 Decode error */
          *TgtLenBytePtr = 0;
          break; 
        }
      }
      *TgtLenBytePtr = TgtByteIdx;
    }
    else
    { /* #50 Unknown code */
      retVal = E_NOT_OK;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_DECODE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_Decode() */
# endif
  /* IPBASE_CODE_BASE64_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerInitWorkspace
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_FUNCCODE(void) IpBase_BerInitWorkspace(IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                              IPBASE_CONSTP2VARAPPLDATA(IpBase_BerStackElementType) StackPtr,
                                              const uint8 Depth)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check WorkspacePtr, StackPtr for NULL pointer */
  if ( (WorkspacePtr == NULL_PTR) || (StackPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize workspace */
    WorkspacePtr->LastData     = NULL_PTR;
    WorkspacePtr->LastDataSize = 0;
    WorkspacePtr->Stack        = StackPtr;
    WorkspacePtr->CurrentDepth = 0;
    WorkspacePtr->MaxDepth     = Depth;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_BER_INIT_WORKSPACE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* IpBase_BerInitWorkspace() */
# endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerGetElement2
 *********************************************************************************************************************/
/*! Internal comment removed.
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
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetElement2(IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                                        IPBASE_P2VARAPPLDATA(IpBase_BerElementType) ElementPtr,
                                                        IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                                        const uint8 ElementDepth,
                                                        IPBASE_P2CONSTAPPLDATA(IpBase_BerDataType) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal;
  IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) stackElementPtr;
  IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) nextStackElementPtr;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (WorkspacePtr == NULL_PTR) || (ElementPtr == NULL_PTR) || (ElementNrPtr == NULL_PTR) ||
       (DataPtr == NULL_PTR) || (DataPtr->DataPtr == NULL_PTR) )
  {
    retVal = IPBASE_E_NOT_OK;
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check workspace */
    retVal = IpBase_BerCheckDepth(WorkspacePtr->MaxDepth, ElementDepth);
    if ( retVal == IPBASE_E_OK )
    {
      boolean         found = 0;
      uint8           level;

      /* #30 Reset workspace */
      IpBase_BerResetWorkspace(WorkspacePtr, DataPtr->DataPtr, DataPtr->DataSize);
      level = (uint8) (WorkspacePtr->CurrentDepth - 1);
      stackElementPtr = &WorkspacePtr->Stack[level];
      do
      { /* #40 Search element in document */
        if ( 0 == ElementNrPtr[level] )
        { /* #50 Invalid ElementNr */
          retVal = IPBASE_E_NOT_OK;
        }
        else
        { /* #60 Read within chapter */
          retVal = IpBase_BerReadChapter(stackElementPtr, ElementNrPtr, DataPtr->DataPtr, level);
        }
        if ( retVal != IPBASE_E_OK )
        {
          break;
        }
        if ( ElementDepth > WorkspacePtr->CurrentDepth )
        { /* #70 Proceed to next chapter */
          retVal = IpBase_BerSwitchChapter(WorkspacePtr, &stackElementPtr, &nextStackElementPtr, &level);
        }
        else
        { /* #80 element found */
          found = TRUE;
          *ElementPtr = stackElementPtr->CurrentElement;
        }
      } while ( (retVal == IPBASE_E_OK) && (found == FALSE) );
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #90 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_BER_GET_ELEMENT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_BerGetElement2() */
# endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerGetElement
 *********************************************************************************************************************/
/*! Internal comment removed.
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
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetElement(IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                                        IPBASE_P2VARAPPLDATA(IpBase_BerElementType) ElementPtr,
                                                        IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                                        const uint8 ElementDepth,
                                                        IPBASE_P2CONSTAPPLDATA(uint8) DataPtr,
                                                        const uint32 DataSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal;
  IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) stackElementPtr;
  IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) nextStackElementPtr;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           level, found;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check WorkspacePtr, ElementPtr, ElementNrPtr, ElementNrPtr for NULL pointer */
  if ( (WorkspacePtr == NULL_PTR) || (ElementPtr == NULL_PTR) || (ElementNrPtr == NULL_PTR) || (DataPtr == NULL_PTR) )
  {
    retVal = IPBASE_E_NOT_OK;
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check workspace */
    retVal = IpBase_BerCheckDepth(WorkspacePtr->MaxDepth, ElementDepth);
    if ( retVal == IPBASE_E_OK )
    {
      /* #30 Reset workspace */
      IpBase_BerResetWorkspace(WorkspacePtr, DataPtr, DataSize);
      found = 0;
      level = (uint8) (WorkspacePtr->CurrentDepth - 1);
      stackElementPtr = &WorkspacePtr->Stack[level];
      do
      { /* #40 Search element in document */
        if ( 0 == ElementNrPtr[level] )
        { /* #50 Invalid ElementNr */
          retVal = IPBASE_E_NOT_OK;
        }
        else
        { /* #60 Read within chapter */
          retVal = IpBase_BerReadChapter(stackElementPtr, ElementNrPtr, DataPtr, level);
        }
        if ( retVal != IPBASE_E_OK )
        {
          break;
        }
        if ( ElementDepth > WorkspacePtr->CurrentDepth )
        { /* #70 Proceed to next chapter */
          retVal = IpBase_BerSwitchChapter(WorkspacePtr, &stackElementPtr, &nextStackElementPtr, &level);
        }
        else
        { /* #80 element found */
          found = 1;
          *ElementPtr = stackElementPtr->CurrentElement;
        }
      } while ( (retVal == IPBASE_E_OK) && (found == 0) );
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #90 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_BER_GET_ELEMENT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_BerGetElement() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_EncodeBase64Block
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BASE64_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_EncodeBase64Block(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                            uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  static IPBASE_CONSTCONST(uint8) encodingTab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  uint32 encodingTabIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Char0: Byte0 6Bits, never '=' */
  TgtDataPtr[0] = encodingTab[(SrcDataPtr[0]>>2)&0x3F];
  /* #20 Char1: Byte0 2Bits, Byte1 4 Bits, never '=' */
  encodingTabIdx = (SrcDataPtr[0]<<4)&0x30;
  if ( SrcLenByte > 1 )
  {
    encodingTabIdx |= (SrcDataPtr[1]>>4)&0x0F;
  }
  TgtDataPtr[1] = encodingTab[encodingTabIdx];
  /* #30 Char2: Byte1 4Bits, Byte2 2 Bits, maybe '=' */
  if ( SrcLenByte > 1 )
  {
    encodingTabIdx = (SrcDataPtr[1]<<2)&0x3C;
    if ( SrcLenByte > 2 )
    {
      encodingTabIdx |= (SrcDataPtr[2]>>6)&0x03;
    }
    TgtDataPtr[2] = encodingTab[encodingTabIdx];
  }
  else
  {
    TgtDataPtr[2] = '=';
  }
  /* Char3: Byte2 6Bits, maybe '=' */
  if ( SrcLenByte > 2 )
  {
    TgtDataPtr[3] = encodingTab[(SrcDataPtr[2])&0x3F];
  }
  else
  {
    TgtDataPtr[3] = '=';
  }

} /* IpBase_EncodeBase64Block() */
# endif 
  /* IPBASE_CODE_BASE64_ENABLE */
/**********************************************************************************************************************
 *  IpBase_DecodeBase64Block
 *********************************************************************************************************************/
/*! Internal comment removed.
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
# if ( IPBASE_CODE_BASE64_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64Block(
                                               IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                               IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                               IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr,
                                               IPBASE_P2VARAPPLDATA(uint32) SrcLenBytePtr,
                                               uint32 SrcRemainingLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal  = IPBASE_E_OK;
  static IPBASE_CONSTCONST(uint8) decodingTab[] = {
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 000 - 009: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 010 - 019: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 020 - 029: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 030 - 039: */
   65, 65, 65, 62, 65, 65, 65, 63, 52, 53, /* 040 - 049: +,/,0-1 */
   54, 55, 56, 57, 58, 59, 60, 61, 65, 65, /* 050 - 059: 2-9 */
   65, 64, 65, 65, 65,  0,  1,  2,  3,  4, /* 060 - 069: =,A-E */
    5,  6,  7,  8,  9, 10, 11, 12, 13, 14, /* 070 - 079: F-O */
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, /* 080 - 089: P-Y */
   25, 65, 65, 65, 65, 65, 65, 26, 27, 28, /* 090 - 099: Z, a-c */
   29, 30, 31, 32, 33, 34, 35, 36, 37, 38, /* 100 - 109: d-m */
   39, 40, 41, 42, 43, 44, 45, 46, 47, 48, /* 110 - 119: n-w */
   49, 50, 51, 65, 65, 65, 65, 65, 65, 65, /* 120 - 129: x-z */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 130 - 139: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 140 - 149: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 150 - 159: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 160 - 169: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 170 - 179: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 180 - 189: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 190 - 199: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 200 - 209: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 210 - 219: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 220 - 229: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 230 - 239: */
   65, 65, 65, 65, 65, 65, 65, 65, 65, 65, /* 240 - 249: */
   65, 65, 65, 65, 65, 65                  /* 250 - 255: */
  };
  uint32 srcDataIdx = 0;
  uint8 srcDataTmp[4] = { 0U, 0U, 0U, 0U
                        };
  uint8 srcDataTmpIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  while ( (4 > srcDataTmpIdx) && (retVal == IPBASE_E_OK) )
  { /* #10 Decode all elements */
    if ( SrcRemainingLen <= srcDataIdx )
    { /* #20 Handle invalid padding */
      retVal = E_NOT_OK;
 
      /* configurable behavior?
         some specifications do not allow padding at the end
         (see RFC4648, 3.2. Padding of Encoded Data) */
    }
    else if ( (IPBASE_CODE_BASE64_LF_CHAR == SrcDataPtr[srcDataIdx]) ||
              (IPBASE_CODE_BASE64_CR_CHAR == SrcDataPtr[srcDataIdx]) )
    { /* #30 Ignore at least line feed and carriage return characters */
      srcDataIdx++;
 
      /* configurable behavior?
         the Base64-encoded certificate from the Certificate Update Server contains line feed (0x0A) characters
         these characters are only allowed, if specification requires them
         (see RFC4648, 3.1. Line Feeds in Encoded Data) */
    }
    else
    { /* #40 Lookup decoded value */
      srcDataTmp[srcDataTmpIdx] = decodingTab[SrcDataPtr[srcDataIdx]];
      srcDataTmpIdx++;
      srcDataIdx++;
    }
  }
 
  if ( (IPBASE_CODE_BASE64_INV_CODE == srcDataTmp[0]) || (IPBASE_CODE_BASE64_INV_CODE == srcDataTmp[1]) || 
       (IPBASE_CODE_BASE64_INV_CODE == srcDataTmp[2]) || (IPBASE_CODE_BASE64_INV_CODE == srcDataTmp[3]) )
  { /* #50 Invalid character -> abort decoding
       (see RFC4648, 3.3. Interpretation of Non-Alphabet Characters in Encoded Data, 12. Security Considerations) */
    retVal =  E_NOT_OK;
  }
  if ( retVal == E_OK )
  {
    /* #60 Byte0: Char0 6Bits, Char1 2Bits */
    TgtDataPtr[0] = (uint8)(((srcDataTmp[0]<<2)&0xFC) | ((srcDataTmp[1]>>4)&0x03));
    (*TgtLenBytePtr)++;
    /* #70 Byte1: Char1 4Bits, Char2 4Bits */
    if ( IPBASE_CODE_BASE64_EMPTY_CODE != srcDataTmp[2] )
    {
      (*TgtLenBytePtr)++;
      TgtDataPtr[1] = (uint8)(((srcDataTmp[1]<<4)&0xF0) | ((srcDataTmp[2]>>2)&0x0F));
    }
    /* #80 Byte2: Char2 2Bits, Char3 6Bits */
    if ( IPBASE_CODE_BASE64_EMPTY_CODE != srcDataTmp[3] )
    {
      (*TgtLenBytePtr)++;
      TgtDataPtr[2] = (uint8)(((srcDataTmp[2]<<6)&0xC0) | (srcDataTmp[3]&0x3F));
    }
    *SrcLenBytePtr += srcDataIdx;
  }

  return retVal;
} /* IpBase_DecodeBase64Block() */ /* PRQA S 2006 */ /* MD_MSR 14.7 */
# endif
  /* IPBASE_CODE_BASE64_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerCheckDepth
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerCheckDepth(uint8 MaxDepth, uint8 ElementDepth)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal  = IPBASE_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( MaxDepth == 0 )
  { /* #10 Check stack */
    retVal = IPBASE_E_MEM;
  }
  else if ( ElementDepth == 0 )
  { /* #20 Check element depth */
    retVal = IPBASE_E_NOT_OK;
  }
  else if ( MaxDepth < ElementDepth )
  { /* #30 Check max depth */
    retVal = IPBASE_E_MEM;
  }
  else
  { /* MISRA */
  }

  return retVal;
} /* IpBase_BerCheckDepth() */
# endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerResetWorkspace
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_BerResetWorkspace(
                                     IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                     IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 DataSize)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 reset elements of workspace */
  WorkspacePtr->LastData = DataPtr;
  WorkspacePtr->LastDataSize = DataSize;
  WorkspacePtr->CurrentDepth = 1;
  WorkspacePtr->Stack[0].CurrentBlock.Idx = 0u; 
  WorkspacePtr->Stack[0].CurrentBlock.EndIdx = DataSize;
  WorkspacePtr->Stack[0].CurrentIdx = 0u;
  WorkspacePtr->Stack[0].CurrentElementNr = 0;

} /* IpBase_BerResetWorkspace() */
# endif
  /* IPBASE_CODE_BER_ENABLE */

/**********************************************************************************************************************
 *  IpBase_BerReadChapter
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerReadChapter(
                                            IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType) StackElementPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) DataPtr,
                                            uint8 Level)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal  = IPBASE_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  while ( (retVal == IPBASE_E_OK) && (ElementNrPtr[Level] > StackElementPtr->CurrentElementNr) )
  {
    if ( StackElementPtr->CurrentBlock.EndIdx <= StackElementPtr->CurrentIdx )
    { /* #10 Reached end of Block */
      retVal = IPBASE_E_NOT_OK;
    }
    else
    { /* #20 Read next element */
      retVal = IpBase_BerGetNextElement(&StackElementPtr->CurrentElement,
                                        StackElementPtr->CurrentIdx,
                                        &StackElementPtr->CurrentBlock, DataPtr);
      if ( retVal == IPBASE_E_OK )
      { /* #30 Increase element counters */
        StackElementPtr->CurrentElementNr++;
        StackElementPtr->CurrentIdx = StackElementPtr->CurrentElement.EndIdx;
      }
    }
  }

  return retVal;
} /* IpBase_BerReadChapter() */
# endif
  /* IPBASE_CODE_BER_ENABLE */

/**********************************************************************************************************************
 *  IpBase_BerSwitchChapter
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerSwitchChapter(
                                            IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                            IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType *) StackElementPtrPtr,
                                            IPBASE_P2VARAPPLDATA(IpBase_BerStackElementType *) NextStackElementPtrPtr,
                                            IPBASE_P2VARAPPLDATA(uint8) LevelPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal  = IPBASE_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( IpBase_BerIsPrimitive((*StackElementPtrPtr)->CurrentElement.Type) )
  { /* #10 Check primitive element, cannot increase level */
    retVal = IPBASE_E_NOT_OK;
  }
  else
  {
    /* #20 Increase level (switch chapter) */
    *LevelPtr = WorkspacePtr->CurrentDepth; /* alternative: Level++; */ 
    *NextStackElementPtrPtr = &WorkspacePtr->Stack[*LevelPtr];
    (*NextStackElementPtrPtr)->CurrentBlock.Idx    = (*StackElementPtrPtr)->CurrentElement.ContentIdx;
    (*NextStackElementPtrPtr)->CurrentBlock.EndIdx = (*StackElementPtrPtr)->CurrentElement.EndIdx;
    (*NextStackElementPtrPtr)->CurrentIdx = (*NextStackElementPtrPtr)->CurrentBlock.Idx;
    (*NextStackElementPtrPtr)->CurrentElementNr = 0;
    *StackElementPtrPtr = *NextStackElementPtrPtr;
    WorkspacePtr->CurrentDepth++;
  }

  return retVal;
} /* IpBase_BerSwitchChapter() */
# endif
  /* IPBASE_CODE_BER_ENABLE */

/**********************************************************************************************************************
 *  IpBase_BerGetNextElement
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetNextElement(
                                            IPBASE_CONSTP2VARAPPLDATA(IpBase_BerElementType) Element,
                                            uint32 ElementIdx,
                                            IPBASE_CONSTP2CONSTAPPLDATA(IpBase_BerBlockType) Block,
                                            IPBASE_CONSTP2CONSTAPPLDATA(uint8) Data)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_ReturnType  retVal  = IPBASE_E_BER_PARAM;
  uint32             contentSize;
  uint8              sizeOfLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check element inside block no boundary crossing */
  if ( Block->EndIdx >= (ElementIdx + 2) )
  { /* #20 Read element */
    Element->Idx = ElementIdx;
    Element->Type = Data[ElementIdx];
    ElementIdx++;
    sizeOfLen = Data[ElementIdx];
    ElementIdx++;

    /* #30 Evaluate length size */
    if ( 0 == IPBASE_BER_IS_EXTENDED_LEN(sizeOfLen) )
    {
      Element->EndIdx     = ElementIdx + sizeOfLen;
      Element->ContentIdx = ElementIdx;
      retVal = IPBASE_E_OK;
    }
    else
    {
      sizeOfLen &= 0x7F;

      if ( (Block->EndIdx >= (ElementIdx + sizeOfLen)) && 
           (sizeOfLen <= IPBASE_CODE_BER_MAX_LEN_U32) && (sizeOfLen != IPBASE_CODE_BER_INFINITE_LEN) )
      { /* #40 Evaluate content size */
        contentSize = IpBase_BerGetContentSize(sizeOfLen, &ElementIdx, Data);
        Element->EndIdx = ElementIdx + contentSize;
        if ( Block->EndIdx >= Element->EndIdx )
        { /* #50 Get content */
          Element->ContentIdx = ElementIdx;
          retVal = IPBASE_E_OK;
        }
      }
    }
  }

  return retVal;
} /* IpBase_BerGetNextElement */
# endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerGetContentSize
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_BerGetContentSize(uint8 SizeOfLen, 
                                                              IPBASE_CONSTP2VARAPPLDATA(uint32) ElementIdxPtr,
                                                              IPBASE_CONSTP2CONSTAPPLDATA(uint8) Data)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32             contentSize;

  /* ----- Implementation ----------------------------------------------- */
  if ( SizeOfLen == 1 )
  { /* #10 Handle 1 byte length encoding */
    contentSize = Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
  }
  else if ( SizeOfLen == 2 )
  { /* #20 Handle 2 bytes length encoding */
    contentSize = Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
  }
  else if ( SizeOfLen == 3 )
  { /* #30 Handle 3 bytes length encoding */
    contentSize = Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
  }
  else
  { /* #40 Handle 4 bytes length encoding */
    contentSize = Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
    contentSize <<= 8;
    contentSize |= Data[*ElementIdxPtr];
    (*ElementIdxPtr)++;
  }

  return contentSize;
} /* IpBase_BerGetContentSize */
# endif
  /* IPBASE_CODE_BER_ENABLE */
# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
  /* IPBASE_CODE_ENABLE */

/* module specific MISRA deviations:
 MD_IpBase_5.6_781:
      Reason:     Non-unique identifier (additionally used within structs)
      Risk:       Uncritical if only within structs
      Prevention: Covered by code review
 MD_IpBase_6.3_5013:
      Reason:     Basic data types used for text
      Risk:       Size may differ on depending on platform
      Prevention: Covered by integration tests
 MD_IpBase_11.4:
      Reason:     Caused by type conversions due to message buffer interpretation
      Risk:       Possible programming error may be undetected by compiler
      Prevention: Covered by code review
 MD_IpBase_13.7:
      Reason:     Boolean check with fix result in DET checks due to code configuration
      Risk:       DET checks may fail
      Prevention: AUTOSAR DET checks shall be switched off in production code
 MD_IpBase_17.4:
      Reason:     Usage of pointer within array
      Risk:       Memory Access violation
      Prevention: Covered by code review
 MD_IpBase_21.1:
      Reason:     No wraparound in code
      Risk:       Memory Access violation
      Prevention: Covered by code review
 MD_IpBase_306:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_310:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_750:
      Reason:     union used for IPv6/IPv4 single API
      Risk:       Memory Access violation
      Prevention: Covered by component tests
 MD_IpBase_776:
      Reason:     Prefixing required by AUTOSAR
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_850:
      Reason:     Performance improvement by using macros
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_857:
      Reason:     Number of macros too high
      Risk:       Limited test depth
      Prevention: Covered by component tests
 MD_IpBase_3673:
      Reason:     Parameter no const
      Risk:       Unwanted change of variable
      Prevention: Covered by component tests
*/
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Code.c
 *********************************************************************************************************************/
