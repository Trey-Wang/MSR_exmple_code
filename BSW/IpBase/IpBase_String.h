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
 *         File:  IpBase_String.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  String header
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (IPBASE_STRING_H)
# define IPBASE_STRING_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if ( IPBASE_STRING_ENABLE == STD_ON )
#  define IPBASE_START_SEC_CODE
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpBase_StrCpy
 *********************************************************************************************************************/
/*! \brief         String copy
 *  \details       String copy (zero terminated strings)
 *  \param[out]    TgtPtr            pointer for target string
 *  \param[in]     SrcPtr            pointer to source string
 *  \return        Number of copied bytes
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          The source string has to be terminated by '\0'
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint8) IpBase_StrCpy(IPBASE_P2VARAPPLDATA(uint8) TgtPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcPtr);
/**********************************************************************************************************************
 *  IpBase_StrCpyMaxLen
 *********************************************************************************************************************/
/*! \brief         String copy max length
 *  \details       String copy (zero terminated strings) with length limitation
 *  \param[out]    TgtPtr            pointer for target string
 *  \param[in]     SrcPtr            pointer to source string
 *  \param[in]     MaxLen            maximum length
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          The source string has to be terminated by '\0'
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint32) IpBase_StrCpyMaxLen(IPBASE_P2VARAPPLDATA(uint8) TgtPtr,
                                                   IPBASE_P2CONSTAPPLDATA(uint8) SrcPtr, uint32 MaxLen);
/**********************************************************************************************************************
 *  IpBase_StrCmp
 *********************************************************************************************************************/
/*! \brief         Compare strings
 *  \details       Compare 2 strings until end of the shorter string (accepting sub strings)
 *  \param[out]    Str1Ptr               pointer to first string
 *  \param[in]     Str2Ptr               pointer to second string
 *  \return        IPBASE_CMP_EQUAL      strings are equal
 *  \return        IPBASE_CMP_NOT_EQUAL  string pattern not found
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          The strings have to be terminated by '\0'. String subsets are accepted (i.e. "Hello"=="Hello World")
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint8) IpBase_StrCmp(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr);
/***********************************************************************************************************************
 *  IpBase_StrCmpLen
 **********************************************************************************************************************/
/*! \brief         Compare strings length
 *  \details       Compare 2 strings with limited length
 *  \param[in]     Str1Ptr               pointer to string 1
 *  \param[in]     Str2Ptr               pointer to string 2
 *  \param[in]     StrLen                length of the search string [byte]
 *  \return        IPBASE_CMP_EQUAL      strings are equal
 *  \return        IPBASE_CMP_NOT_EQUAL  strings are not equal, or other error condition occurred
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint8) IpBase_StrCmpLen(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                               IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr,
                                               uint16 StrLen);
/**********************************************************************************************************************
 *  IpBase_StrCmpNoCase
 *********************************************************************************************************************/
/*! \brief         Compare 2 strings ignore case
 *  \details       Compare 2 strings until end of the shorter string ignoring the case (accepting sub strings)
 *  \param[out]    Str1Ptr               pointer to first string
 *  \param[in]     Str2Ptr               pointer to second string
 *  \return        IPBASE_CMP_EQUAL      strings are equal
 *  \return        IPBASE_CMP_NOT_EQUAL  string pattern not found
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          The strings have to be terminated by '\0'. String subsets are accepted (i.e. "Hello"=="Hello World")
 *                 Case is ignored (i.e. "Hello" == "HELLO")
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint8) IpBase_StrCmpNoCase(IPBASE_P2CONSTAPPLDATA(uint8) Str1Ptr,
                                                  IPBASE_P2CONSTAPPLDATA(uint8) Str2Ptr);
/***********************************************************************************************************************
 *  IpBase_StrFindSubStr
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
extern IPBASE_FUNCCODE(uint32) IpBase_StrFindSubStr(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                    IPBASE_P2CONSTAPPLDATA(uint8) SubStrPtr,
                                                    uint16 StrLen, uint16 SubStrLen);
/***********************************************************************************************************************
 *  IpBase_StrLen
 **********************************************************************************************************************/
/*! \brief         String length
 *  \details       Check the length of the string
 *  \param[in]     StrPtr                 pointer to string 
 *  \param[in]     MaxLen                 maximum length of the search string [byte]
 *  \return        0..MaxLen-1            length of the string
 *  \return        MaxLen                 if MaxLen is reached
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint32) IpBase_StrLen(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                             uint32 MaxLen);
/***********************************************************************************************************************
 *  IpBase_ConvInt2String
 **********************************************************************************************************************/
/*! \brief         Convert integer to string
 *  \details       Convert an integer number to an ASCII string (dec)
 *  \param[in]     IntVal            integer number
 *  \param[in,out] StrPtr            pointer to string
 *  \param[in,out] StrLenPtr         pointer to length of the string [byte]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2String(uint32 IntVal,
                                                             IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                             IPBASE_P2VARAPPLDATA(uint8) StrLenPtr);
/***********************************************************************************************************************
 *  IpBase_ConvInt2HexString
 **********************************************************************************************************************/
/*! \brief         Convert integer to hex string
 *  \details       Convert an integer number to an ASCII string (hex)
 *  \param[in]     IntVal            integer number
 *  \param[in,out] StrPtr            pointer to string (hex coded)
 *  \param[in,out] StrLenPtr         pointer to length of the string [byte]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2HexString(uint32 IntVal,
                                                                IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                                IPBASE_P2VARAPPLDATA(uint8) StrLenPtr);
/***********************************************************************************************************************
 *  IpBase_ConvInt2StringBase
 **********************************************************************************************************************/
/*! \brief         Convert integer to string base
 *  \details       Convert an integer number to an ASCII string (dec) without incrementing StrPtr but '\0' at end
 *  \param[in]     IntVal            integer number
 *  \param[in,out] StrPtr            pointer to string
 *  \param[in,out] StrLenPtr         pointer to length of the string [byte]
 *  \return        E_OK              integer converted
 *  \return        E_NOT_OK          integer conversion failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2StringBase(uint32 IntVal,
                                                                 IPBASE_P2VARAPPLDATA(uint8) StrPtr,
                                                                 uint8 StrLen);
/***********************************************************************************************************************
 *  IpBase_ConvInt2StringFront
 **********************************************************************************************************************/
/*! \brief         Convert integer to string front
 *  \details       Convert an integer number to an ASCII string (dec) at front of string
 *  \param[in]     IntVal            integer number
 *  \param[in,out] StrPtr            pointer to string
 *  \param[in,out] StrLenPtr         pointer to length of the string [byte]
 *  \return        E_OK              integer converted
 *  \return        E_NOT_OK          integer conversion failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvInt2StringFront(uint32 IntVal,
                                                                  IPBASE_P2VARAPPLDATA(uint8) *StrPtr,
                                                                  IPBASE_P2VARAPPLDATA(uint8) StrLenPtr);
/***********************************************************************************************************************
 *  IpBase_ConvArray2HexStringBase
 **********************************************************************************************************************/
/*! \brief         Convert array to string (hex) base
 *  \details       Convert an array number to an ASCII string (hex), omits leading '00'
 *  \param[in]     ArrayPtr          pointer to array
 *  \param[in]     ArrayLen          array length [byte]
 *  \param[in,out] StrPtr            pointer to string (has to provide (ArrayLen*2)+1 chars)
 *  \return        E_OK              array converted
 *  \return        E_NOT_OK          array conversion failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Array [a0a1], ArrayLen 2 -> 'A0A1'.Array [00a1], ArrayLen 2 -> 'A1'
**********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvArray2HexStringBase(IPBASE_P2CONSTAPPLDATA(uint8) ArrayPtr,
                                                                      uint16 ArrayLen,
                                                                      IPBASE_P2VARAPPLDATA(uint8) StrPtr);
/***********************************************************************************************************************
 *  IpBase_ConvString2Int
 **********************************************************************************************************************/
/*! \brief         Convert string to integer
 *  \details       Convert an ASCII string (dec values) to an integer
 *  \param[in]     StrPtr            pointer to string
 *  \param[in]     StrLen            length of the string [byte]
 *  \param[out]    IntValPtr         pointer to integer number
 *  \return        E_OK              string could be converted to integer
 *  \return        E_NOT_OK          string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvString2Int(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                             const uint8 StrLen,
                                                             IPBASE_P2VARAPPLDATA(uint32) IntValPtr);
/***********************************************************************************************************************
 *  IpBase_ConvString2IntDyn
 **********************************************************************************************************************/
/*! \brief         Convert string to integer dynamic
 *  \details       Convert an ASCII string (dec values) to an integer with dynamic length
 *  \param[in]     StrPtr            pointer to string
 *  \param[in]     StrLen            length of the string [byte]
 *  \param[out]    IntValPtr         pointer to integer number
 *  \return        E_OK              string could be converted to integer
 *  \return        E_NOT_OK          string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Str '12', StrLen 2 -> 12. Str '12', StrLen 1 -> 1. 
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvString2IntDyn(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                                IPBASE_P2VARAPPLDATA(uint8) StrLenPtr,
                                                                IPBASE_P2VARAPPLDATA(uint32) IntValPtr);
/***********************************************************************************************************************
 *  IpBase_ConvStringHex2Int
 **********************************************************************************************************************/
/*! \brief         Convert hex string to integer
 *  \details       Convert an ASCII string (hex values) to an integer
 *  \param[in]     StrPtr            pointer to string
 *  \param[in]     StrLen            length of the string [byte]
 *  \param[out]    IntValPtr         pointer to integer number
 *  \return        E_OK              string could be converted to integer
 *  \return        E_NOT_OK          string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Str 'a2', StrLen 2 -> 0xA2. Str 'a2', StrLen 1 -> 0xA. 
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2Int(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                const uint8 StrLen, 
                                                                IPBASE_P2VARAPPLDATA(uint32) IntValPtr);
/***********************************************************************************************************************
 *  IpBase_ConvStringHex2IntDyn
 **********************************************************************************************************************/
/*! \brief         Convert hex string to integer dynamic
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
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvStringHex2IntDyn(IPBASE_P2CONSTAPPLDATA(uint8) *StrPtr,
                                                                   IPBASE_P2VARAPPLDATA(uint8) StrIntLenPtr,
                                                                   IPBASE_P2VARAPPLDATA(uint32) IntValPtr);
/***********************************************************************************************************************
 *  IpBase_ConvString2IntBase
 **********************************************************************************************************************/
/*! \brief         Convert string to integer base
 *  \details       Convert an ASCII string (dec values) to an integer
 *  \param[in]     StrPtr            pointer to string
 *  \param[in]     StrMaxLen         max length of the string [byte]
 *  \return        0-4294967295      string converted to integer
 *  \return        IPBASE_ULONG_MAX  string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Str '12', StrMaxLen 2 -> 12. Str '12', StrMaxLen 1 -> 1. 
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint32) IpBase_ConvString2IntBase(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                         uint8 StrMaxLen);
/***********************************************************************************************************************
 *  IpBase_ConvString2SignedIntBase
 **********************************************************************************************************************/
/*! \brief         Convert string to signed integer
 *  \details       Convert an ASCII string (dec values) to a signed integer
 *  \param[in]     StrPtr                  pointer to string
 *  \param[in]     StrMaxLen               max length of the string [byte]
 *  \return        -2147483646-+2147483647 string converted to integer
 *  \return        IPBASE_ULONG_MAX        string could not be converted to integer
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Str '12', StrMaxLen 2 -> 12. Str '12', StrMaxLen 1 -> 1. Str 'a' -> IPBASE_ULONG_MAX.
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(sint32) IpBase_ConvString2SignedIntBase(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                               uint8 StrMaxLen);
/***********************************************************************************************************************
 *  IpBase_ConvHexString2ArrayBase
 **********************************************************************************************************************/
/*! \brief         Convert hex string to array base
 *  \details       Convert an ASCII hex string to an array number, omits leading '00'
 *  \param[in]     ArrayPtr          pointer to array
 *  \param[in]     ArrayLen          array length [byte]
 *  \param[in,out] StrPtr            pointer to string (has to provide (ArrayLen*2)+1 chars)
 *  \return        E_OK              array converted
 *  \return        E_NOT_OK          array conversion failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Array [a0a1], ArrayLen 2 -> 'A0A1'.Array [00a1], ArrayLen 2 -> 'A1'
**********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ConvHexString2ArrayBase(IPBASE_P2VARAPPLDATA(uint8) ArrayPtr,
                                                                      uint16 ArrayLen,
                                                                      IPBASE_CONSTP2CONSTAPPLDATA(uint8) StrPtr);
#  define IPBASE_STOP_SEC_CODE
#  include "MemMap.h"
# endif
  /* IPBASE_STRING_ENABLE */
#endif 
  /* IPBASE_STRING_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_String.h
 *********************************************************************************************************************/
