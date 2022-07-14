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
 *         File:  IpBase_Copy.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Copy API header
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
#if !defined (IPBASE_COPY_H)
# define IPBASE_COPY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "IpBase_Types.h"
# if ( IPBASE_COPY_MACRO_ENABLE == STD_ON )
#  include "vstdlib.h"
# endif
  /* IPBASE_COPY_MACRO_ENABLE */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if ( IPBASE_COPY_ENABLE == STD_ON )

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define IPBASE_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpBase_Copy
 *********************************************************************************************************************/
/*! \brief        Copy data (memcpy)
 *  \details      Copy data alike memcpy
 *  \param[out]   TgtDataPtr            pointer for target data
 *  \param[in]    SrcDataPtr            pointer to source data
 *  \param[in]    LenByte               data length in bytes
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-120245
 *********************************************************************************************************************/
#  if ( (IPBASE_COPY_MACRO_ENABLE == STD_ON) && defined VStdMemCpy )
/* PRQA S 3453,3458 2 */ /* MD_MSR_19.4 MD_MSR_19.7 */
#   define IpBase_Copy(TgtDataPtr, SrcDataPtr, LenByte) \
             VStdMemCpy((TgtDataPtr), (SrcDataPtr), ((uint16_least)(LenByte)))
#  else
extern IPBASE_FUNCCODE(void) IpBase_Copy(IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
                                    IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType) SrcDataPtr,
                                    uint32 LenByte );
#  endif
  /* IPBASE_COPY_MACRO_ENABLE */
/**********************************************************************************************************************
 *  IpBase_CopySmallData
 *********************************************************************************************************************/
/*! \brief        Copy small data
 *  \details      Copy small data (memcpy, for data < 32 aligned chunks, i.e. < 32 bytes for 1 byte aligned data)
 *  \param[out]   TgtDataPtr            pointer for target data
 *  \param[in]    SrcDataPtr            pointer to source data
 *  \param[in]    LenByte               data length in bytes
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-120245
 *********************************************************************************************************************/
#  if ( (IPBASE_COPY_MACRO_ENABLE == STD_ON) && defined VStdMemCpy )
/* PRQA S 3453,3458 2 */ /* MD_MSR_19.4 MD_MSR_19.7 */
#   define IpBase_CopySmallData(TgtDataPtr, SrcDataPtr, LenByte) \
             VStdMemCpy((TgtDataPtr), (SrcDataPtr), ((uint16_least)(LenByte)))
#  else
extern IPBASE_FUNCCODE(void) IpBase_CopySmallData(IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
                                    IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType) SrcDataPtr,
                                    uint32 LenByte );
#  endif
  /* IPBASE_COPY_MACRO_ENABLE */
/**********************************************************************************************************************
 *  IpBase_Fill
 *********************************************************************************************************************/
/*! \brief        Fill data
 *  \details      Fill memory with given pattern (memset)
 *  \param[out]   TgtDataPtr            pointer for target data
 *  \param[in]    Pattern               fill pattern
 *  \param[in]    LenByte               data length in bytes
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-120245
 *********************************************************************************************************************/
#  if ( (IPBASE_COPY_MACRO_ENABLE == STD_ON) && defined VStdMemSet )
/* PRQA S 3453,3458 1 */ /* MD_MSR_19.4 MD_MSR_19.7 */
#   define IpBase_Fill(TgtDataPtr, Pattern, LenByte) VStdMemSet((TgtDataPtr), (Pattern), (LenByte))
#  else
extern IPBASE_FUNCCODE(void) IpBase_Fill(IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
                                         uint8 Pattern, uint32 LenByte );
#  endif
  /* IPBASE_COPY_MACRO_ENABLE */
#  define IPBASE_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# endif
  /* IPBASE_CODE_ENABLE */
#endif 
  /* IPBASE_COPY_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Copy.h
 *********************************************************************************************************************/
