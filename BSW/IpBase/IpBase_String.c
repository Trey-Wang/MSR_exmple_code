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
 *         File:  IpBase_String.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  String API implementation
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
#define IPBASE_STRING_SOURCE

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
# error "IpBase_String.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define IPBASE_LOW_CASE_ASCII_OFF           (32)  /* i.e. 'a' - 'A' for case insensitive implementation */

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if ( IPBASE_STRING_ENABLE == STD_ON )
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  IpBase_ConvHexString2ArrayBaseHlp
 **********************************************************************************************************************/
/*! \brief         Convert hex string to array base helper
 *  \details       Convert an ASCII hex digit to a number
 *  \param[in]     Digit             digit to be converted 
 *  \return        Number            number of digit
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Digit 'a' -> A, '8' -> 8
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint8) IpBase_ConvHexString2ArrayBaseHlp(uint8 Digit);
/***********************************************************************************************************************
 *  IpBase_ConvStringHex2IntDynHlp
 **********************************************************************************************************************/
/*! \brief         Convert hex string to integer dynamic helper
 *  \details       Convert an ASCII string (hex values) to an integer with dynamic length
 *  \param[in]     StrPtr            pointer to string
 *  \param[in,out] StrIntLenPtr      length of the integer [digits]
 *  \param[out]    IntValPtr         pointer to integer number
 *  \return        E_OK              string could be converted to integer
 *  \return        E_NOT_OK          string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Str '12', StrLen 2 -> 0x12. Str '12', StrLen 1 -> 0x1. 
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2IntDynHlp(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                                            IPBASE_P2VARAPPLDATA(uint8) StrIntLenPtr,
                                                                            IPBASE_P2VARAPPLDATA(uint32) IntValPtr);
/***********************************************************************************************************************
 *  IpBase_StrFindSubStrHlp
 **********************************************************************************************************************/
/*! \brief         Search for sub-string
 *  \details       Searches for the first occurrence of sub-string within a string (e.g. string "hello world",
 *                 sub-string "world")
 *  \param[in]     StrPtr               pointer to string
 *  \param[in]     SubStrPtr            pointer to sub string
 *  \param[in]     StrLen               length of the string [byte]
 *  \param[in]     SubStrLen            length of the sub string [byte]
 *  \return        PosByte              position in string where the sub-string starts
 *  \return        IPBASE_STR_LEN_INVALID sub-string not found or error
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_StrFindSubStrHlp(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                             IPBASE_P2CONSTAPPLDATA(uint8) SubStrPtr,
                                                             uint16 StrLen, uint16 SubStrLen);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_StrCpy
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(uint8) IpBase_StrCpy(IPBASE_P2VARAPPLDATA(uint8) TgtPtr,
                                     IPBASE_P2CONSTAPPLDATA(uint8) SrcPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           byteIdx = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (TgtPtr == NULL_PTR) || (SrcPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Copy all string characters */
    while ( SrcPtr[byteIdx] != '\0' )
    {
      TgtPtr[byteIdx] = SrcPtr[byteIdx];
      byteIdx++;
      if ( 0xFF == byteIdx )
      {
        break; /* avoid overflow */
      }
    }
    TgtPtr[byteIdx] = '\0';
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_COPY, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return byteIdx;
} /* IpBase_StrCpy() */
/**********************************************************************************************************************
 *  IpBase_StrCpyMaxLen
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_StrCpyMaxLen(IPBASE_P2VARAPPLDATA(uint8) TgtPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcPtr, uint32 MaxLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           byteIdx = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (TgtPtr == NULL_PTR) || (SrcPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Copy all string characters */
    while ( SrcPtr[byteIdx] != '\0' )
    {
      TgtPtr[byteIdx] = SrcPtr[byteIdx];
      byteIdx++;
      if ( MaxLen == byteIdx )
      {
        break; /* avoid overflow */
      }
    }
    TgtPtr[byteIdx] = '\0';
  }
  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_COPY_MAX_LEN, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return byteIdx;
} /* IpBase_StrCpyMaxLen() */
/**********************************************************************************************************************
 *  IpBase_StrCmp
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(uint8) IpBase_StrCmp(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                     IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           retVal  = IPBASE_CMP_EQUAL;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           byteIdx = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (Str1Ptr == NULL_PTR) || (Str2Ptr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Compare all string character */
    while ( (Str1Ptr[byteIdx] != '\0') && (Str2Ptr[byteIdx] != '\0') )
    {
      if ( Str1Ptr[byteIdx] != Str2Ptr[byteIdx] )
      {
        retVal = IPBASE_CMP_NOT_EQUAL;
      }
      else
      {
        byteIdx++;
        if ( byteIdx == IPBASE_UCHAR_MAX )
        {
          retVal = IPBASE_CMP_NOT_EQUAL; /* avoid overflow */
        }
      }
      if ( retVal == IPBASE_CMP_NOT_EQUAL )
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_CMP, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrCmp() */
/***********************************************************************************************************************
 *  IpBase_StrCmpLen
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint8) IpBase_StrCmpLen(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                        IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr,
                                        uint16 StrLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           retVal  = IPBASE_CMP_EQUAL;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint16          readPos;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (Str1Ptr == NULL_PTR) || (Str2Ptr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Compare all string character */
    for(readPos = 0; readPos < StrLen; readPos++)
    {
      /* check character */
      if(Str1Ptr[readPos] != Str2Ptr[readPos])
      {
        /* mismatch in local search -> return to search */
        retVal = IPBASE_CMP_NOT_EQUAL;
        break;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_CMP_LEN, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrCmpLen() */
/**********************************************************************************************************************
 *  IpBase_StrCmpNoCase
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(uint8) IpBase_StrCmpNoCase(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                           IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           retVal  = IPBASE_CMP_EQUAL;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint16          byteIdx = 0;
  char            char1UpCase; /* PRQA S 5013 */ /* MD_IpBase_6.3_5013 */
  char            char2UpCase; /* PRQA S 5013 */ /* MD_IpBase_6.3_5013 */

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (Str1Ptr == NULL_PTR) || (Str2Ptr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Compare all string character */
    while ( ('\0' != Str1Ptr[byteIdx]) && ('\0' != Str2Ptr[byteIdx]) )
    { /* PRQA S 5013 4 */ /* MD_IpBase_6.3_5013 */
      char1UpCase = (char)((('z' >= Str1Ptr[byteIdx])&&('a' <= Str1Ptr[byteIdx])) ? \
                     (Str1Ptr[byteIdx]-IPBASE_LOW_CASE_ASCII_OFF) : (Str1Ptr[byteIdx])); 
      char2UpCase = (char)((('z' >= Str2Ptr[byteIdx])&&('a' <= Str2Ptr[byteIdx])) ? \
                     (Str2Ptr[byteIdx]-IPBASE_LOW_CASE_ASCII_OFF) : (Str2Ptr[byteIdx]));
      if ( char1UpCase != char2UpCase )
      {
        retVal = IPBASE_CMP_NOT_EQUAL;
      }
      else
      {
        byteIdx++;
        if ( byteIdx == IPBASE_USHRT_MAX )
        {
          retVal = IPBASE_CMP_NOT_EQUAL; /* avoid overflow */
        }
      }
      if ( retVal == IPBASE_CMP_NOT_EQUAL )
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_CMP_NO_CASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrCmpNoCase() */
/***********************************************************************************************************************
 *  IpBase_StrFindSubStr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_StrFindSubStr(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) SubStrPtr,
                                            uint16 StrLen, uint16 SubStrLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          retVal  = IPBASE_STR_LEN_INVALID;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (SubStrPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else if ( StrLen < SubStrLen )
  {
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Find substring helper */
    retVal = IpBase_StrFindSubStrHlp(StrPtr, SubStrPtr, StrLen, SubStrLen);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_FIND_SUB_STR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrFindSubStr() */
/***********************************************************************************************************************
 *  IpBase_StrLen
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_StrLen(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                      uint32 MaxLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          retVal = MaxLen;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           byteIdx = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( StrPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check all string character */
    while ( byteIdx <= MaxLen )
    {
      if (StrPtr[byteIdx] == '\0')
      {
        retVal = byteIdx;
        break;
      }
      byteIdx++;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_STR_LEN, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_StrLen() */
/***********************************************************************************************************************
 *  IpBase_ConvInt2String
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2String(uint32 IntVal,
                                                      IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                      IPBASE_P2VARAPPLDATA(uint8) StrLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint8           pos = 0U;
  IPBASE_P2VARAPPLDATA(uint8) intStrPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (StrLenPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Convert all digits */
    intStrPtr = (IPBASE_P2VARAPPLDATA(uint8)) *StrPtr;
    while(pos < *StrLenPtr)
    {
      digit = (uint8) (IntVal % 10U);
      intStrPtr[*StrLenPtr - (pos + 1U)] = (uint8)(digit + 0x30); /* 0..9 */
      IntVal = IntVal / 10U;
      pos++;

      if(IntVal == 0U)
      { /* #30 All digits converted */
        *StrPtr = &(intStrPtr[*StrLenPtr - pos]);
        *StrLenPtr = pos;
        retVal = E_OK;
        break;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_INT_TO_STR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvInt2String() */
/***********************************************************************************************************************
 *  IpBase_ConvInt2StringBase
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2StringBase(uint32 IntVal,
                                                          IPBASE_P2VARAPPLDATA(uint8) StrPtr,
                                                          uint8 StrLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digits[IPBASE_INT_DIGIT_MAX];
  uint8           pos = 0U;
  uint8           maxPos;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( StrPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all digits */
    do {
       digits[pos] = (uint8) (IntVal % 10U);
       IntVal = IntVal / 10U;
       pos++;
    } while ( (pos < IPBASE_INT_DIGIT_MAX ) && (IntVal != 0U) );

    if ( (pos+1) <= StrLen )
    { /* #30 Convert all digits */
      maxPos = pos;
      StrPtr[maxPos] = '\0';
      while ( 0U < pos )
      {
        StrPtr[maxPos - pos] = (uint8)(digits[pos-1U] + 0x30); /* PRQA S 3382 */ /* MD_IpBase_21.1 */
        pos--;
      }
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_INT_TO_STR_BASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvInt2StringBase() */
/***********************************************************************************************************************
 *  IpBase_ConvArray2HexStringBase
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvArray2HexStringBase(IPBASE_P2CONSTAPPLDATA(uint8) ArrayPtr,
                                                               uint16 ArrayLen,
                                                               IPBASE_P2VARAPPLDATA(uint8) StrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint16          arrayIdx = 0U;
  uint16          strIdx = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (ArrayPtr == NULL_PTR) || (StrPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan array */
    while(arrayIdx < ArrayLen)
    { /* #30 Convert hex number */
      digit = (uint8)((ArrayPtr[arrayIdx]>>4U)&0xF);
      if(digit < 10U)
      {
        StrPtr[strIdx] = (uint8)(digit + 0x30); /* 0..9 */
      }
      else
      {
        StrPtr[strIdx] = (uint8)(digit + 0x37); /* A..F */
      }
      strIdx++;
      digit = (uint8) (ArrayPtr[arrayIdx]&0xF);
      if(digit < 10U)
      {
        StrPtr[strIdx] = (uint8)(digit + 0x30); /* 0..9 */
      }
      else
      {
        StrPtr[strIdx] = (uint8)(digit + 0x37); /* A..F */
      }
      strIdx++;
      arrayIdx++;
    }
    /* #40 Finalize string */
    if ( strIdx == 0U )
    {
      StrPtr[strIdx] = '0';
      strIdx++;
    }
    StrPtr[strIdx] = '\0';
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #50 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_ARRAY_TO_STR_BASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvArray2HexStringBase() */
/***********************************************************************************************************************
 *  IpBase_ConvInt2StringFront
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2StringFront(uint32 IntVal,
                                                           IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                           IPBASE_P2VARAPPLDATA(uint8) StrLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digits[IPBASE_INT_DIGIT_MAX];
  uint8           pos = 0U;
  uint8           maxPos;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (StrLenPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all digits */
    do {
       digits[pos] = (uint8) (IntVal % 10U);
       IntVal = IntVal / 10U;
       pos++;
    } while ( (pos < IPBASE_INT_DIGIT_MAX) && (IntVal != 0U) );

    if ( pos <= *StrLenPtr )
    { /* #30 Convert all digits */
      maxPos = pos;
      while ( pos > 0U )
      {
        (*StrPtr)[maxPos - pos] = (uint8)(digits[pos-1U] | 0x30);
        pos--;
      }
      *StrLenPtr -= maxPos;
      *StrPtr    += maxPos; /* PRQA S 0488 */ /* MD_IpBase_17.4_488 */
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_INT_TO_STR_FRONT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvInt2StringFront() */
/***********************************************************************************************************************
 *  IpBase_ConvInt2HexString
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2HexString(uint32 IntVal,
                                                         IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                         IPBASE_P2VARAPPLDATA(uint8) StrLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint8           pos = 0U;
  IPBASE_P2VARAPPLDATA(uint8) intStrPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (StrLenPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all character */
    intStrPtr = (IPBASE_P2VARAPPLDATA(uint8)) *StrPtr;
    while(pos < *StrLenPtr)
    { /* #30 Convert digit */
      digit = (uint8)(IntVal & 0xF);
      if(digit < 10U)
      {
        intStrPtr[*StrLenPtr - (pos + 1U)] = (uint8)(digit + 0x30); /* 0..9 */
      }
      else
      {
        intStrPtr[*StrLenPtr - (pos + 1U)] = (uint8)(digit + 0x37); /* A..F (Hint: 0x37 == 'A'-10) */
      }

      IntVal = IntVal >> 4U;
      pos++;
      
      if(IntVal == 0U)
      { /* #40 Finalize string */
        *StrPtr = &(intStrPtr[*StrLenPtr - pos]);
        *StrLenPtr = pos;
        retVal = E_OK;
        break;
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #50 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_INT_TO_HEX_STR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvInt2HexString() */
/***********************************************************************************************************************
 *  IpBase_ConvString2Int
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvString2Int(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                      const uint8 StrLen,
                                                      IPBASE_P2VARAPPLDATA(uint32) IntValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           posA = 0U;
  uint32          locLen = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer and invalid param */
  if ( (StrPtr == NULL_PTR) || (IntValPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else if(StrLen > IPBASE_INT_DIGIT_MAX)
  { 
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all digits */
    while(posA != StrLen)
    {
      locLen *= 10U;
      locLen += StrPtr[posA] & 0x0F; /* 0x30-0x39 represents '0'-'9' -> -'0' does not have to be coded */
      posA++;
    }
    *IntValPtr = locLen;
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_STR_TO_INT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvString2Int() */
/***********************************************************************************************************************
 *  IpBase_ConvString2IntDyn
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvString2IntDyn(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                         IPBASE_P2VARAPPLDATA(uint8) StrLenPtr,
                                                         IPBASE_P2VARAPPLDATA(uint32) IntValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint8           posA = 0U;
  uint32          locLen = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (*StrPtr == NULL_PTR) || (StrLenPtr == NULL_PTR) || (IntValPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all character */
    while(posA < *StrLenPtr)
    {
      digit = (*StrPtr)[posA];
      if ( ('0'>digit) || ('9'<digit) )
      { /* #30 Found end of number */
        break;
      }
      locLen *= 10U;
      locLen += (digit - '0');
      posA++;
    }
    *StrLenPtr = posA;
    *IntValPtr = locLen;
    *StrPtr    = &((*StrPtr)[posA]);
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_STR_TO_INT_DYN, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvString2IntDyn() */
/***********************************************************************************************************************
 *  IpBase_ConvStringHex2Int
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2Int(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                         const uint8 StrLen, 
                                                         IPBASE_P2VARAPPLDATA(uint32) IntValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           pos;
  uint32          intVal = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer and invalid param */
  if ( (StrPtr == NULL_PTR) || (IntValPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else if(StrLen > IPBASE_HEX_INT_DIGIT_MAX)
  { 
    errorId = IPBASE_E_INV_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all character */
    for(pos = 0U; pos < StrLen; pos++)
    {
      /* #30 Convert ASCII character to integer */
      uint8 singleChar = StrPtr[pos];
      uint8 singleNumber;
      if(singleChar <= 0x39)
      {
        /* digit 0..9 */
        singleNumber = (uint8)(singleChar & 0x0F);
      }
      else
      {
        /* digit A..F */
        singleNumber = (uint8)((singleChar & 0x0F) + 9U);
      }
      intVal += ((uint16)(singleNumber)) << (4U * (StrLen - pos - 1U));
    }
    *IntValPtr = intVal;
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_HEX_STR_TO_INT, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvStringHex2Int() */
/***********************************************************************************************************************
 *  IpBase_ConvString2IntDyn
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_IpBase_3673*/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2IntDyn(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                            IPBASE_P2VARAPPLDATA(uint8) StrIntLenPtr,
                                                            IPBASE_P2VARAPPLDATA(uint32) IntValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (StrPtr == NULL_PTR) || (*StrPtr == NULL_PTR) || (StrIntLenPtr == NULL_PTR) || (IntValPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Convert string helper */
    retVal = IpBase_ConvStringHex2IntDynHlp(StrPtr, StrIntLenPtr, IntValPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_HEX_STR_TO_INT_DYN, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvString2IntDyn() */
/***********************************************************************************************************************
 *  IpBase_ConvString2IntBase
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_ConvString2IntBase(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                  uint8 StrMaxLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint8           posA   = 0U;
  uint32          locLen = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( StrPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all character */
    while(posA < StrMaxLen)
    {
      digit = (StrPtr)[posA];
      if ( (digit<'0') || ('9'<digit) )
      { /* end of number */
        break;
      }
      locLen *= 10U;
      locLen += (digit - '0');
      posA++;
    }
    if ( posA == 0U )
    {
      locLen = IPBASE_ULONG_MAX;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_STR_TO_INT_BASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return locLen;
} /* IpBase_ConvString2IntBase() */
/***********************************************************************************************************************
 *  IpBase_ConvString2SignedIntBase
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(sint32) IpBase_ConvString2SignedIntBase(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                        uint8 StrMaxLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           digit;
  uint8           isSigned = 0U;
  uint8           posA     = 0U;
  sint32          retVal   = 0U;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( StrPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan all character */
    while(posA < StrMaxLen)
    {
      digit = (StrPtr)[posA];
      if ( digit=='-' )
      { /* negative value */
        isSigned = 1U;
      }
      else
      {
        if ( (digit<'0') || (digit>'9') )
        { /* end of number */
          break;
        }
        retVal *= 10;
        retVal += (digit - '0');
      }
      posA++;
    }
    /* #30 Evaluate result */
    if ( posA == 0U )
    {
      retVal = IPBASE_SLONG_MAX;
    }
    else if ( isSigned != 0U )
    {
      retVal = -retVal;
    }
    else
    { /* MISRA */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_STR_TO_SIGNED_INT_BASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvString2SignedIntBase() */
/***********************************************************************************************************************
 *  IpBase_ConvHexString2ArrayBase
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvHexString2ArrayBase(IPBASE_P2VARAPPLDATA(uint8) ArrayPtr,
                                                               uint16 ArrayLen,
                                                               IPBASE_CONSTP2CONSTAPPLDATA(uint8) StrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal   = E_NOT_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint16          arrayIdx = 0U;
  uint16          strIdx = 0U;
  uint8           digit0 = 0;
  uint8           digit1 = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check for NULL pointer */
  if ( (ArrayPtr == NULL_PTR) || (StrPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Scan array */
    while(arrayIdx < ArrayLen)
    {
      if ( (digit0 != IPBASE_UCHAR_MAX) && (digit1 != IPBASE_UCHAR_MAX)  )
      {
        digit0 = IpBase_ConvHexString2ArrayBaseHlp(StrPtr[strIdx]);
      }
      strIdx++;
      if ( (digit0 != IPBASE_UCHAR_MAX) && (digit1 != IPBASE_UCHAR_MAX)  )
      {
        digit1 = IpBase_ConvHexString2ArrayBaseHlp(StrPtr[strIdx]);
      }
      strIdx++;
      if ( (digit0 == IPBASE_UCHAR_MAX) || (digit1 == IPBASE_UCHAR_MAX) )
      { /* fill up with 0 */
        ArrayPtr[arrayIdx] = 0U;
      }
      else
      { /* fill arry with digits */
        ArrayPtr[arrayIdx] = (uint8)((digit0<<4U)+digit1);
      }
      arrayIdx++;
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CONV_HEX_STR_TO_ARRAY_BASE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_ConvString2SignedIntBase() */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  IpBase_StrFindSubStrHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
static IPBASE_FUNCCODE(uint32) IpBase_StrFindSubStrHlp(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                       IPBASE_P2CONSTAPPLDATA(uint8) SubStrPtr,
                                                       uint16 StrLen, uint16 SubStrLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          retVal  = IPBASE_STR_LEN_INVALID;
  uint16          readPos;
  uint16          subReadPos = 0U;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compare all string character */
  for(readPos = 0; readPos < StrLen; readPos++)
  {
    /* check character */
    if(StrPtr[readPos] != SubStrPtr[subReadPos])
    {
      subReadPos = 0U;
    }
    if(StrPtr[readPos] == SubStrPtr[subReadPos])
    {
      subReadPos++;
      if ( subReadPos == SubStrLen )
      { /* #30 Complete sub-string found */
        retVal = (readPos+1U)-subReadPos;
        break;
      }
    }
  }

  return retVal;
} /* IpBase_StrFindSubStrHlp() */
/***********************************************************************************************************************
 *  IpBase_ConvString2IntDynHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
static IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2IntDynHlp(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                                      IPBASE_P2VARAPPLDATA(uint8) StrIntLenPtr,
                                                                      IPBASE_P2VARAPPLDATA(uint32) IntValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           digit;
  uint8           posA   = 0U;
  uint32          locLen = 0U;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Scan all character */
  while(posA < *StrIntLenPtr)
  {
    digit = (*StrPtr)[posA];
    if ( ('0' <= digit)&&(digit <= '9') )
    {
      digit -= '0';
    }
    else if ( ('A'<=digit)&&(digit<='F') )
    {
      digit -= ('A'-10U); /* i.e. digit = digit - 'A' + 10 */
    }
    else if ( ('a'<=digit)&&(digit<='f') )
    {
      digit -= ('a'-10U); /* i.e. digit = digit - 'a' + 10 */
    }
    else
    { /* end of number */
      break;
    }
    locLen <<= 4U; /* i.e. locLen = locLen * 16 */
    locLen += digit;
    posA++;
  }
  /* #20 Evaluate result */
  *StrIntLenPtr = posA;
  *IntValPtr = locLen;
  *StrPtr    = &((*StrPtr)[posA]);

  return E_OK;
} /* IpBase_ConvString2IntDynHlp() */
/***********************************************************************************************************************
 *  IpBase_ConvHexString2ArrayBaseHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
static IPBASE_FUNCCODE(uint8) IpBase_ConvHexString2ArrayBaseHlp(uint8 Digit)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle digit */
  if( ('0' <=  Digit) && (Digit <= '9') )
  {
    retVal = (uint8)(Digit - '0'); /* 0..9 */
  }
  else if( ('a' <=  Digit) && (Digit <= 'f') )
  {
    retVal = (uint8)((uint8)(Digit - 'a') + 10U); /* a..f */
  }
  else if( ('A' <=  Digit) && (Digit <= 'F') )
  {
    retVal = (uint8)((uint8)(Digit - 'A') + 10U); /* A..F */
  }
  else
  { /* #20 Handle invalid digit */
    retVal = IPBASE_UCHAR_MAX;
  }

  return retVal;
} /* IpBase_ConvHexString2ArrayBaseHlp() */
# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
  /* IPBASE_STRING_ENABLE */

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
 *  END OF FILE: IpBase_String.c
 *********************************************************************************************************************/
