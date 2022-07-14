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
 *         File:  IpBase_PBuf.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  PBuf API implementation
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
#define IPBASE_PBUF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
#include "IpBase.h"
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != 3u) || (IPBASE_SW_MINOR_VERSION != 0u) || (IPBASE_SW_PATCH_VERSION != 0u) )
# error "IpBase_PBuf.c: Source and Header file are inconsistent!"
#endif

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if ( IPBASE_PBUF_ENABLE == STD_ON )
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  IpBase_FindStringInPbufHlp
 **********************************************************************************************************************/
/*! \brief         Find string in PBuf helper
 *  \details       Find a string in a PBuf actual search
 *  \param[in]     StrPtr                pointer to search string
 *  \param[in]     PbufPtr               pointer to PBuf struct
 *  \param[in]     StrLen                length of the search string [byte]
 *  \param[in]     ReadPosMax            max read position in PBufPtr
 *  \param[out]    StrPosPtr             index in PBuf where the searched string starts
 *  \return        E_OK                  string was found
 *  \return        E_NOT_OK              string was not found or API parameters are invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_FindStringInPbufHlp(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                        IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                        IPBASE_P2VARAPPLDATA(uint32) StrPosPtr,
                                                                        uint16 StrLen, uint32 ReadPosMax);
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_StrCmpPBuf
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
IPBASE_FUNCCODE(uint8) IpBase_StrCmpPBuf(IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) *SrcPbufPtr,
                                         IPBASE_P2CONSTAPPLDATA(sint8) PatternPtr, 
                                         IPBASE_P2VARAPPLDATA(uint16) CurByteIdxPtr,
                                         IPBASE_P2VARAPPLDATA(uint32) TotByteIdxPtr,
                                         IPBASE_P2VARAPPLDATA(uint32) RestLenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                   retVal  = IPBASE_CMP_NOT_EQUAL;
  uint8                                   errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (SrcPbufPtr == NULL_PTR) || (*SrcPbufPtr == NULL_PTR) || (PatternPtr == NULL_PTR) || 
       (CurByteIdxPtr == NULL_PTR) || (TotByteIdxPtr == NULL_PTR) || (RestLenBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan through SrcPBuf */
    uint16 curByteIdx = *((IPBASE_P2VARAPPLDATA(uint16)) CurByteIdxPtr);
    uint32 totByteIdx = *((IPBASE_P2VARAPPLDATA(uint32)) TotByteIdxPtr);
    uint32 patternByteIdx = 0;
    IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) curPbufPtr = *SrcPbufPtr;

    while ( ((*SrcPbufPtr)->totLen > totByteIdx) && (retVal != IPBASE_CMP_EQUAL) )
    {
      if ( (curPbufPtr->payload[curByteIdx] != (uint8) PatternPtr[patternByteIdx]) )
      { /* #30 Handle invalid pattern */
        *RestLenBytePtr = 0;
        break;
      }
      curByteIdx++;
      totByteIdx++;
      patternByteIdx++;
      (*RestLenBytePtr)++;
      if ( (0 == PatternPtr[patternByteIdx]) )
      { /* #40 End of pattern string -> complete pattern found */
        *((IPBASE_P2VARAPPLDATA(uint16)) CurByteIdxPtr) = curByteIdx;
        *((IPBASE_P2VARAPPLDATA(uint32)) TotByteIdxPtr) = totByteIdx;
        *SrcPbufPtr                                     = curPbufPtr;
        retVal = IPBASE_CMP_EQUAL;
      }
      else if ( ((*SrcPbufPtr)->totLen > totByteIdx) && (curPbufPtr->len <= curByteIdx) )
      {
        curByteIdx = 0;
        curPbufPtr++; /* PRQA S 0488,0489 */ /* MD_IpBase_17.4 */
      }
      else
      { /* MISRA */
      }
    }
    if ( retVal != IPBASE_CMP_EQUAL )
    {  /* #50 End of string -> no character or incomplete pattern found */
      *((IPBASE_P2VARAPPLDATA(uint16)) CurByteIdxPtr) = curByteIdx;
      *((IPBASE_P2VARAPPLDATA(uint32)) TotByteIdxPtr) = totByteIdx;
      *SrcPbufPtr                                     = curPbufPtr;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_CMP_PBUF, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrCmpPBuf() */
/**********************************************************************************************************************
 *  IpBase_IncPBuf
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(void) IpBase_IncPBuf(IPBASE_P2VARAPPLDATA(IpBase_PbufType *) PbufPtr,
                                     IPBASE_P2VARAPPLDATA(uint16) CurByteIdxPtr,
                                     IPBASE_P2VARAPPLDATA(uint32) TotByteIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                   errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (PbufPtr == NULL_PTR) || (*PbufPtr == NULL_PTR) || (CurByteIdxPtr == NULL_PTR) || 
       (TotByteIdxPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {

    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check buffer length, do not increment idxs if end is reached */
    if ( (*PbufPtr)->totLen > (*TotByteIdxPtr) )
    {
      (*CurByteIdxPtr)++;
      (*TotByteIdxPtr)++;
      /* #30 Switch to next buffer segment */
      if ( ((*PbufPtr)->totLen > (*TotByteIdxPtr)) && ((*PbufPtr)->len <= (*CurByteIdxPtr)) )
      {
        (*CurByteIdxPtr) = 0;
        (*PbufPtr)++; /* PRQA S 0488,0489 */ /* MD_IpBase_17.4 */
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_INC_PBUF, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

} /* IpBase_IncPBuf() */
/***********************************************************************************************************************
 *  IpBase_CopyString2PbufAt
 **********************************************************************************************************************/
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
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyString2PbufAt(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                         uint16 StrLen,
                                                         IPBASE_P2VARAPPLDATA(IpBase_PbufType) PbufPtr,
                                                         uint32 StartPos)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8              errorId = IPBASE_E_NO_ERROR;
  uint32             writePosA = 0;    /* overall write position in PBuf -> 'A'bsolute position */
  uint32             writePosP;        /* write position in PBuf element -> 'P'buf local position */
  uint32             copiedLen = 0;
  uint32             copyLen, copyBlockLen;
  IPBASE_P2VARAPPLDATA(IpBase_PbufType) pBufElePtr = (IPBASE_P2VARAPPLDATA(IpBase_PbufType)) PbufPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (PbufPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  /* #20 Check for invalid param */
  else if(StartPos > (uint16)PbufPtr->totLen)
  {
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {

    /* ----- Implementation ----------------------------------------------- */
    /* #30 Go to start position in Pbuf */
    while( (pBufElePtr->len + writePosA) <= StartPos)
    { /* #40 Go to next PBuf segment */
      writePosA += pBufElePtr->len;
      pBufElePtr = &pBufElePtr[1];
    }
    writePosP = StartPos - writePosA;

    /* #50 Limit length to minimum of string length and PBuf */
    copyLen = StrLen;
    if ( (PbufPtr->totLen - StartPos) < copyLen )
    { 
      copyLen = PbufPtr->totLen - StartPos;
    }

    while( copiedLen < copyLen )
    { /* #60 Copy data into PBuf element */
      copyBlockLen = pBufElePtr->len - writePosP;
      if ( copyBlockLen > (copyLen - copiedLen) )
      {
        copyBlockLen = copyLen - copiedLen;
      }
      /* PRQA S 310,3305 2 */ /* MD_IpBase_310 */
      IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&pBufElePtr->payload[writePosP], 
                  (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))(&StrPtr[copiedLen]), copyBlockLen);
      copiedLen += copyBlockLen;
      /* #70 Go to next PBuf element */
      writePosP = 0;
      pBufElePtr = &pBufElePtr[1];
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #80 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY_STR_TO_PBUF_AT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return E_OK;
} /* IpBase_CopyString2PbufAt() */
/***********************************************************************************************************************
 *  IpBase_CopyPbuf2String
 **********************************************************************************************************************/
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
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyPbuf2String(IPBASE_P2VARAPPLDATA(uint8) StrPtr,
                                                       IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                       uint16 StrLen, uint32 StartPos)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint16          strPos = 1;
  uint32          readPosA = 0;   /* overall read position in PBuf */
  uint32          readPosP;       /* read position in PBuf element */
  IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) pBufElePtr = (IPBASE_P2CONSTAPPLDATA(IpBase_PbufType)) PbufPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (PbufPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  /* #20 Check for invalid param */
  else if((StartPos + StrLen) > (uint16)PbufPtr->totLen)
  {
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {

    /* ----- Implementation ----------------------------------------------- */
    /* #30 Go to start position in Pbuf */
    while( (pBufElePtr->len + readPosA) <= StartPos)
    { /* #40 Go to next PBuf segment */
      readPosA += pBufElePtr->len;
      pBufElePtr = &pBufElePtr[1];
    }
    readPosP = StartPos - readPosA;

    /* #50 Copy first byte */
    StrPtr[0] = pBufElePtr->payload[readPosP];
    readPosP++;

    /* #60 Copy remaining bytes */
    while(strPos < StrLen)
    {
      if(readPosP == pBufElePtr->len)
      { /* #70 Go to next PBuf segment */
        readPosP = 0;
        pBufElePtr = &pBufElePtr[1];
      }

      /* #80 Copy byte and inc position */
      StrPtr[strPos] = pBufElePtr->payload[readPosP];
      strPos++;
      readPosP++;
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #90 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY_PBUF_TO_STR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CopyPbuf2String() */
/***********************************************************************************************************************
 *  IpBase_FindStringInPbuf
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_FindStringInPbuf(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                        IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                        uint16 StrLen, uint32 StartPos,
                                                        IPBASE_P2VARAPPLDATA(uint32) StrPosPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32          readPosMax;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (PbufPtr == NULL_PTR)  || (StrPosPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else if( StartPos > (PbufPtr->totLen - StrLen) )
  { /* #20 Check for invalid param */
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Search for string */
    readPosMax = PbufPtr->totLen - StrLen;
    *StrPosPtr = StartPos;
    retVal = IpBase_FindStringInPbufHlp(StrPtr, PbufPtr, StrPosPtr, StrLen, readPosMax);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_FIND_STR_IN_PBUF, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_FindStringInPbuf() */
/***********************************************************************************************************************
 *  IpBase_CheckStringInPbuf
 **********************************************************************************************************************/
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
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CheckStringInPbuf(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                         IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                         uint16 StrLen, uint32 StartPos)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32          readPosMax;
  uint32          readPosA = 0;   /* overall read position in PBuf */
  uint32          readPosP;       /* read position in PBuf element */
  uint8           locStrPos = 0;
  IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) pBufElePtr = (IPBASE_P2CONSTAPPLDATA(IpBase_PbufType)) PbufPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (PbufPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {

    /* ----- Implementation ----------------------------------------------- */
    readPosMax = PbufPtr->totLen - StrLen;
    /* #20 Check for invalid param */
    if( StartPos > readPosMax )
    {
      errorId = IPBASE_E_INV_PARAM;
    }
    else
    { /* #30 Go to start position */
      while((pBufElePtr->len + readPosA) <= StartPos)
      { /* #40 Go to next PBuf segment */
        readPosA += pBufElePtr->len;
        pBufElePtr = &pBufElePtr[1];
      }
      readPosP = StartPos - readPosA;

      while(locStrPos < StrLen)
      { /* #50 Search in string */
        if(readPosP == pBufElePtr->len)
        { /* #60 Go to next PBuf segment */
          readPosP = 0;
          pBufElePtr = &pBufElePtr[1];
        }
        if(pBufElePtr->payload[readPosP] != StrPtr[locStrPos])
        { /* #70 Mismatch -> stop search */
          break;
        }

        /* #80 Character matches in search string -> go ahead */
        readPosP++;
        locStrPos++;

      }
      if(locStrPos == StrLen)
      { /* #90 String found */
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #100 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CHK_STR_IN_PBUF, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CheckStringInPbuf() */
/***********************************************************************************************************************
 *  IpBase_ReadByteInPbuf
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ReadByteInPbuf(IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                      uint32 BytePos,
                                                      IPBASE_P2VARAPPLDATA(uint8) SingleBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32          readPosA = 0;  /* overall read position in PBuf */
  uint32          readPosP;      /* read position in PBuf element */
  IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) pBufElePtr = (IPBASE_P2CONSTAPPLDATA(IpBase_PbufType)) PbufPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (PbufPtr == NULL_PTR) || (SingleBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else if( BytePos > PbufPtr->totLen )
  { /* #20 Check for invalid param */
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  { 
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Go to start position */
    while( (pBufElePtr->len + readPosA) <= BytePos)
    {
      /* #40 Go to next PBuf segment */
      readPosA += pBufElePtr->len;
      pBufElePtr = &pBufElePtr[1];
    }
    /* #50 Read byte */
    readPosP = BytePos - readPosA;
    *SingleBytePtr = pBufElePtr->payload[readPosP];
    retVal = E_OK;
  }
  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_READ_BYTE_IN_PBUF, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ReadByteInPbuf() */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_FindStringInPbufHlp
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
 *
 *
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_FindStringInPbufHlp(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                        IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                        IPBASE_P2VARAPPLDATA(uint32) StrPosPtr,
                                                                        uint16 StrLen, uint32 ReadPosMax)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint32          readPosA = 0;    /* overall read position in PBuf */
  uint32          readPosP;        /* read position in PBuf element */
  boolean         found   = 0;
  IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) pBufElePtr = (IPBASE_P2CONSTAPPLDATA(IpBase_PbufType)) PbufPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Go to start position in Pbuf */
  while( (pBufElePtr->len + readPosA) <= *StrPosPtr)
  { /* #20 Go to next PBuf segment */
    readPosA += pBufElePtr->len;
    pBufElePtr = &pBufElePtr[1];
  }
  readPosP = (*StrPosPtr) - readPosA;
  readPosA = (*StrPosPtr);
  while((readPosA <= ReadPosMax) && (found == 0) )
  { /* #30 Global search (start pattern not found) */
    if(pBufElePtr->payload[readPosP] == StrPtr[0])
    { /* #40 First character found, check following characters */
      uint8 locStrPos = 1;
      uint32 locReadPosP = readPosP +1;
      IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) locPbufElePtr = pBufElePtr;

      while(locStrPos < StrLen)
      { /* #50 Local search (start pattern found) */
        if(locReadPosP == locPbufElePtr->len)
        { /* #60 Go to next Pbuf segment */
          locReadPosP = 0;
          locPbufElePtr = &locPbufElePtr[1];
        }
        if(locPbufElePtr->payload[locReadPosP] != StrPtr[locStrPos])
        { /* #70 Mismatch in local search -> return to search */
          readPosP++;
          break;
        }
        locReadPosP++;
        locStrPos++;
      }
      if(locStrPos == StrLen)
      { /* #80 Complete string found */
        *StrPosPtr = readPosA;
        found = 1;
        retVal = E_OK;
      }
    }
    else
    { /* #90 No match -> increment */
      readPosA++;
      readPosP++;
      if(readPosP == pBufElePtr->len)
      {
        if(readPosA < pBufElePtr->totLen)
        { /* #100 Go to next PBuf segment */
          pBufElePtr = &pBufElePtr[1];
          readPosP = 0;
        }
        else
        { /* #110 End of PBuf reached */
          retVal = E_NOT_OK;
        }
      }
    }
  }

  return retVal;
} /* IpBase_FindStringInPbufHlp() */

# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
  /* IPBASE_PBUF_ENABLE */

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
 *  END OF FILE: IpBase_PBuf.c
 *********************************************************************************************************************/
