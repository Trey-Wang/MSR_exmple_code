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
 *         File:  IpBase_Code.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  En- and decoding header
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
#if !defined (IPBASE_CODE_H)
# define IPBASE_CODE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define IPBASE_CODE_BASE64                               ((uint8) 0x01)

# define IPBASE_CODE_BASE64_RAW_BLOCK_LEN                 ((uint8)  3u)
# define IPBASE_CODE_BASE64_CODED_BLOCK_LEN               ((uint8)  4u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if ( IPBASE_CODE_BER_ENABLE == STD_ON )
/* PRQA S 3453,3458 12 */ /* MD_MSR_19.4 MD_MSR_19.7 */
#  define IpBase_BerSizeOfBlock(block)          ((block).EndIdx - (block).Idx)
#  define IpBase_BerSizeOfElement(elt)          ((elt).EndIdx - (elt).Idx)
#  define IpBase_BerSizeOfElementContent(elt)   ((elt).EndIdx - (elt).ContentIdx)
#  define IpBase_BerIsUniversal(tag)            (0x00u == (0xC0u & (tag)))
#  define IpBase_BerIsApplication(tag)          (0x40u == (0xC0u & (tag)))
#  define IpBase_BerIsContext(tag)              (0x80u == (0xC0u & (tag)))
#  define IpBase_BerIsPrivate(tag)              (0xC0u == (0xC0u & (tag)))
#  define IpBase_BerIsConstructed(tag)          (0x20u == (0x20u & (tag)))
#  define IpBase_BerIsPrimitive(tag)            (0x00u == (0x20u & (tag)))
#  define IpBase_BerGetClass(tag)               ((0xC0u & (tag)) >> 6u)
#  define IpBase_BerGetPc(tag)                  ((0x20u & (tag)) >> 5u)
#  define IpBase_BerGetNumber(tag)              (0x1Fu & (tag))
# endif
  /* IPBASE_CODE_BER_ENABLE */

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if ( IPBASE_CODE_ENABLE == STD_ON )
#  if ( IPBASE_CODE_BER_ENABLE == STD_ON )
typedef struct
{
  uint32 Idx;
  uint32 EndIdx;
} IpBase_BerBlockType;
typedef struct
{
  uint32 ContentIdx;
  uint32 EndIdx;
  uint32 Idx;
  uint8 Type;
} IpBase_BerElementType;
typedef struct
{
  IPBASE_P2CONSTAPPLDATA(uint8) DataPtr;
  uint32 DataSize;
} IpBase_BerDataType;
typedef struct
{
  uint32 CurrentIdx;
  IpBase_BerBlockType CurrentBlock;
  IpBase_BerElementType CurrentElement;
  uint8 CurrentElementNr;
} IpBase_BerStackElementType;
typedef struct
{
  uint32 LastDataSize;
  IPBASE_P2CONSTAPPLDATA(uint8) LastData;
  IpBase_BerStackElementType * Stack;
  uint8 CurrentDepth;
  uint8 MaxDepth;
} IpBase_BerWorkspaceType;
#  endif
  /* IPBASE_CODE_BER_ENABLE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_Encode
 *********************************************************************************************************************/
/*! \brief        Encodes data 
 *  \details      Encodes the given data using the specified code
 *  \param[in]     Code           defines the code used for encoding
 *  \param[in,out] TgtDataPtr     pointer for the encoded data
 *  \param[in]     SrcDataPtr     pointer to the raw data
 *  \param[in,out] TgtLenBytePtr  pointer for the encoded data length in bytes
 *  \param[in]     SrcLenByte     raw data length in bytes
 *  \return        E_OK                 data encoded
 *  \return        E_NOT_OK             data code not be encoded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108036
 *  \note         General encoding
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_Encode(uint8 Code,
                                                     IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                     IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                     IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr, 
                                                     uint32 SrcLenByte);
/**********************************************************************************************************************
 *  IpBase_Decode
 *********************************************************************************************************************/
/*! \brief        Decodes data
 *  \details      Decodes the given data using the specified code
 *  \param[in]     Code           defines the code used for decoding
 *  \param[out]    TgtDataPtr     pointer for the decoded data
 *  \param[in]     SrcDataPtr     pointer to the raw data
 *  \param[in,out] TgtLenBytePtr  pointer for the decoded data length in bytes
 *  \param[in]     SrcLenByte     raw data length in bytes
 *  \return        E_OK                 data decoded
 *  \return        E_NOT_OK             data code could not be decoded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108037
 *  \note         General decoding
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_Decode(uint8 Code,
                                                     IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                     IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                     IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr, 
                                                     uint32 SrcLenByte);
/**********************************************************************************************************************
 *  IpBase_BerInitWorkspace
 *********************************************************************************************************************/
/*! \brief        Initializes the ASN.1/BER parser workspace
 *  \details      Sets all members of workspace to default values or given parameter
 *  \param[out]   Workspace      workspace to initialize
 *  \param[in]    Stack          stack to use
 *  \param[in]    Depth          depth of the stack
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
#  if ( IPBASE_CODE_BER_ENABLE == STD_ON )
extern IPBASE_FUNCCODE(void) IpBase_BerInitWorkspace(IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                                     IPBASE_CONSTP2VARAPPLDATA(IpBase_BerStackElementType) StackPtr,
                                                     const uint8 Depth);
#  endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerGetElement2
 *********************************************************************************************************************/
/*! \brief        Get an ASN.1/BER element
 *  \details      Get an ASN.1/BER element with a given number out of ASN.1/BER encoded data. Variant with MISRA
 *                compliant number of parameters.
 *  \param[in]    WorkspacePtr   internally used workspace
 *  \param[out]   ElementPtr     found element
 *  \param[in]    ElementNrPtr   element number (chapter.section.subsection. ...)
 *  \param[in]    ElementDepth   depth of the element (chapter = 1, chapter.section = 2, ...)
 *  \param[in]    DataPtr        data
 *  \return       IPBASE_E_OK        element was found
 *  \return       IPBASE_E_NOT_OK    element was not found
 *  \return       IPBASE_E_INV_PARAM data is corrupt
 *  \return       IPBASE_E_MEM       memory limit was exceeded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
#  if ( IPBASE_CODE_BER_ENABLE == STD_ON )
extern IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetElement2(
                                            IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                            IPBASE_P2VARAPPLDATA(IpBase_BerElementType) ElementPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                            const uint8 ElementDepth,
                                            IPBASE_P2CONSTAPPLDATA(IpBase_BerDataType) DataPtr);
#  endif
  /* IPBASE_CODE_BER_ENABLE */
/**********************************************************************************************************************
 *  IpBase_BerGetElement
 *********************************************************************************************************************/
/*! \brief        Get an ASN.1/BER element
 *  \details      Get an ASN.1/BER element with a given number out of ASN.1/BER encoded data. Outdated variant.
 *  \param[in]    WorkspacePtr   internally used workspace
 *  \param[out]   ElementPtr     found element
 *  \param[in]    ElementNrPtr   element number (chapter.section.subsection. ...)
 *  \param[in]    ElementDepth   depth of the element (chapter = 1, chapter.section = 2, ...)
 *  \param[in]    DataPtr        data
 *  \param[in]    DataSize       size of the data
 *  \return       IPBASE_E_OK        element was found
 *  \return       IPBASE_E_NOT_OK    element was not found
 *  \return       IPBASE_E_INV_PARAM data is corrupt
 *  \return       IPBASE_E_MEM       memory limit was exceeded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-108038
 *********************************************************************************************************************/
#  if ( IPBASE_CODE_BER_ENABLE == STD_ON )
extern IPBASE_FUNCCODE(IpBase_ReturnType) IpBase_BerGetElement(
                                            IPBASE_CONSTP2VARAPPLDATA(IpBase_BerWorkspaceType) WorkspacePtr,
                                            IPBASE_P2VARAPPLDATA(IpBase_BerElementType) ElementPtr,
                                            IPBASE_P2CONSTAPPLDATA(uint8) ElementNrPtr,
                                            const uint8 ElementDepth,
                                            IPBASE_P2CONSTAPPLDATA(uint8) DataPtr,
                                            const uint32 DataSize);
#  endif
  /* IPBASE_CODE_BER_ENABLE */
# endif
  /* IPBASE_CODE_ENABLE */
#endif
  /* IPBASE_CODE_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Code.h
 *********************************************************************************************************************/
