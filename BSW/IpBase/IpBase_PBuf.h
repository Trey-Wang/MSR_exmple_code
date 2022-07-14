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
 *         File:  IpBase_PBuf.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  PBuf header
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
#if !defined (IPBASE_PBUF_H)
# define IPBASE_PBUF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if ( IPBASE_PBUF_ENABLE == STD_ON )
#  define IPBASE_START_SEC_CODE
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpBase_StrCmpPBuf
 *********************************************************************************************************************/
/*! \brief         Compare string with PBuf
 *  \details       Try to find string within PBuf structure
 *  \param[in]     SrcPbufPtr            pointer to source data
 *  \param[in]     PatternPtr            pointer to string pattern
 *  \param[in,out] CurByteIdxPtr         pointer to local start index
 *  \param[in,out] TotByteIdxPtr         pointer to total start index
 *  \param[in,out] RestLenBytePtr        pointer to end index (behind pattern)
 *  \return        IPBASE_CMP_EQUAL      string pattern found
 *  \return        IPBASE_CMP_NOT_EQUAL  string pattern not found
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Supports PBuf, only for short string comparisons (bytewise access)
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint8) IpBase_StrCmpPBuf(IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) *SrcPbufPtr,
                                                IPBASE_P2CONSTAPPLDATA(sint8) PatternPtr, 
                                                IPBASE_P2VARAPPLDATA(uint16) CurByteIdxPtr,
                                                IPBASE_P2VARAPPLDATA(uint32) TotByteIdxPtr,
                                                IPBASE_P2VARAPPLDATA(uint32) RestLenBytePtr);
/**********************************************************************************************************************
 *  IpBase_IncPBuf
 *********************************************************************************************************************/
/*! \brief         Increment PBuf
 *  \details       Increment within PBuf structure
 *  \param[in,out] PbufPtr               pointer to PBuf struct
 *  \param[in,out] CurByteIdxPtr         pointer to current byte idx within PBuf struct
 *  \param[in,out] TotByteIdxPtr         pointer to total byte idx within PBuf struct
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 *  \note          Increments the PBuf byte access. Switches to next PBuf at end of segment
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(void) IpBase_IncPBuf(IPBASE_P2VARAPPLDATA(IpBase_PbufType) *PbufPtr,
                                            IPBASE_P2VARAPPLDATA(uint16) CurByteIdxPtr,
                                            IPBASE_P2VARAPPLDATA(uint32) TotByteIdxPtr);
/***********************************************************************************************************************
 *  IpBase_CopyString2Pbuf
 **********************************************************************************************************************/
/*! \brief         Copy string to PBuf
 *  \details       Copy a string to a PBuf at a defined position. Length will be limited to PBuf size and string length.
 *  \param[in]     StrPtr                pointer to source string
 *  \param[in]     StrLen                length of the source string [byte]
 *  \param[out]    PbufPtr               pointer to destination Pbuf struct
 *  \param[in]     StartPos              start position in Pbuf
 *  \return        E_OK                  string could be copied
 *  \return        E_NOT_OK              string could not be copied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyString2PbufAt(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                uint16 StrLen,
                                                                IPBASE_P2VARAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                uint32 StartPos);
/***********************************************************************************************************************
 *  IpBase_CopyPbuf2String
 **********************************************************************************************************************/
/*! \brief         Copy PBuf into string
 *  \details       Copy PBuf content into string starting at a given position within PBuf
 *  \param[in]     StrPtr                pointer to string
 *  \param[in]     PbufPtr               pointer to PBuf struct
 *  \param[in]     StrLen                length of the string [byte]
 *  \param[in]     StartPos              absolute start position in PBuf
 *  \return        E_OK                  string was copied
 *  \return        E_NOT_OK              string was not copied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyPbuf2String(IPBASE_P2VARAPPLDATA(uint8) StrPtr,
                                                                IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                uint16 StrLen, uint32 StartPos);
/***********************************************************************************************************************
 *  IpBase_FindStringInPbuf
 **********************************************************************************************************************/
/*! \brief         Find string in PBuf
 *  \details       Find a string in a PBuf
 *  \param[in]     StrPtr                pointer to search string
 *  \param[in]     PbufPtr               pointer to PBuf struct
 *  \param[in]     StrLen                length of the search string [byte]
 *  \param[in]     StartPos              start position for search in PBuf
 *  \param[out]    StrPosPtr             index in PBuf where the searched string starts
 *  \return        E_OK                  string was found
 *  \return        E_NOT_OK              string was not found or API parameters are invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_FindStringInPbuf(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                 IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                 uint16 StrLen, uint32 StartPos,
                                                                 IPBASE_P2VARAPPLDATA(uint32) StrPosPtr);
/***********************************************************************************************************************
 *  IpBase_CheckStringInPbuf
 **********************************************************************************************************************/
/*! \brief         Check for string in PBuf
 *  \details       Check whether a string is found in a PBuf at the given position
 *  \param[in]     StrPtr                pointer to search string
 *  \param[in]     PbufPtr               pointer to PBuf struct
 *  \param[in]     StrLen                length of the search string [byte]
 *  \param[in]     StartPos              start position for search in PBuf
 *  \return        E_OK                  string was found
 *  \return        E_NOT_OK              string was not found or API parameters are invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_CheckStringInPbuf(IPBASE_P2CONSTAPPLDATA(uint8) StrPtr,
                                                                  IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                                  uint16 StrLen, uint32 StartPos);
/***********************************************************************************************************************
 *  IpBase_ReadByteInPbuf
 **********************************************************************************************************************/
/*! \brief         Read byte in PBuf
 *  \details       Read a byte in a PBuf
 *  \param[in]     PbufPtr               pointer to Pbuf struct
 *  \param[in]     BytePos               absolute byte position in Pbuf
 *  \param[out]    SingleBytePtr         pointer where the byte shall be copied to
 *  \return        E_OK                  byte was copied
 *  \return        E_NOT_OK              byte was not copied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120249
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_ReadByteInPbuf(IPBASE_P2CONSTAPPLDATA(IpBase_PbufType) PbufPtr,
                                                               uint32 BytePos,
                                                               IPBASE_P2VARAPPLDATA(uint8) SingleBytePtr);
#  define IPBASE_STOP_SEC_CODE
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
# endif
  /* IPBASE_PBUF_ENABLE */
#endif 
  /* IPBASE_PBUF_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_PBuf.h
 *********************************************************************************************************************/
