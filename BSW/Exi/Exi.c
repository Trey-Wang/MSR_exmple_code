/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Exi.c
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  implementation
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file Exi.h.
 *********************************************************************************************************************/

#define EXI_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_Cbk.h"
#include "Exi_BSEncoder.h"
#include "Exi_BSDecoder.h"
#include "Exi_SchemaEncoder.h"
#include "Exi_SchemaDecoder.h"
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
#include "IpBase.h"
#endif

/*lint -e438 */ /* Suppress ID438 because Config pointer only used in Post-Build Variant */

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi.c are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (STATIC)
#define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define EXI_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */
#if ( EXI_CONFIG_VARIANT == 3 )
P2CONST(Exi_PostBuildConfigType, AUTOMATIC, EXI_CONST) Exi_PostBuildConfigPtr;
#endif
  /* EXI_CONFIG_VARIANT */
#define EXI_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

#define EXI_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */
VAR(Exi_StateType, EXI_VAR_ZERO_INIT) Exi_State = EXI_STATE_UNINIT; /* Exi needs state also without DET */
#define EXI_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define EXI_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

#if (STD_ON == EXI_DEV_ERROR_DETECT) && ((!defined EXI_CONFIG_VARIANT) || (3u==EXI_CONFIG_VARIANT))
STATIC FUNC(boolean, EXI_CODE) Exi_IsValidPostBuildConfigData(P2CONST(Exi_ConfigType, EXI_CONST, EXI_CONST) Exi_ConfigPtr);
#endif
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_InitMemory
 *********************************************************************************************************************/
/**
  \brief      This function is used to initialize the global variables of the Exi at startup.
  \param[in]  void
  \return
  \warning    This function shall be called before Exi_Init.\n
  \context    This function can be called in any context.\n
  \note       AUTOSAR extension\n
              Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_InitMemory(void)
{
  Exi_State = EXI_STATE_UNINIT;
}
/**********************************************************************************************************************
 *  Exi_Init
 *********************************************************************************************************************/
/**
  \brief      This function is used to initialize the Exi component. The configuration data that shall be used by the 
              Exi is passed as parameter.
  \param[in]  Exi_ConfigPtr      Pointer to the post-build configuration data structure of the Exi. If the configuration
                                 variant pre-compile is used, the pointer given is ignored.
  \return
  \context    This function shall be called for initialization.
  \warning    This function has to be called before usage of the module
  \note       Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_Init(P2CONST(Exi_ConfigType, AUTOMATIC, EXI_CONST) Exi_ConfigPtr)
{
#if defined (EXI_USE_DUMMY_STATEMENT) && (STD_ON == EXI_USE_DUMMY_STATEMENT) && (3u != EXI_CONFIG_VARIANT)
  Exi_ConfigPtr = Exi_ConfigPtr; /* PRQA S 3199 */ /* dummy assignment avoid compiler warning */
#endif

#if (3u == EXI_CONFIG_VARIANT)
  Exi_State = EXI_STATE_UNINIT; /* support multiple calls of Exi_Init */
  Exi_PostBuildConfigPtr = Exi_ConfigPtr;
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnVoid((NULL_PTR != (Exi_ConfigPtr)), EXI_API_ID_INIT, EXI_E_INV_POINTER);
  Exi_CheckDetErrorReturnVoid(Exi_IsValidPostBuildConfigData(Exi_ConfigPtr), EXI_API_ID_INIT, EXI_E_INVALID_PB_CONFIG);
#endif

#if (STD_ON  == EXI_CRC_CHECK) && (1u != EXI_CONFIG_VARIANT)
  if(Exi_ConfigPtr->Exi_PrecompileCRC != EXI_PRECOMPILE_CRC)
  {
    EcuM_GeneratorCompatibilityError((uint16) EXI_MODULE_ID, (uint8) 0u);
    return;
  }
#endif /* if (STD_ON  == EXI_CRC_CHECK) && (1u != EXI_CONFIG_VARIANT) */
#if (STD_ON  == EXI_CRC_CHECK) && (3u == EXI_CONFIG_VARIANT)
  if(Exi_ConfigPtr->Exi_LinkCRC != Exi_LinktimeCRC)
  {
    EcuM_GeneratorCompatibilityError((uint16) EXI_MODULE_ID, (uint8) 0u);
    return;
  }
#endif /* if (STD_ON  == EXI_CRC_CHECK) && (3u == EXI_CONFIG_VARIANT) */
  Exi_State = EXI_STATE_INIT;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#if (EXI_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Exi_GetVersionInfo
 *********************************************************************************************************************/
/**
  \brief        Return the BCD-coded version information of the Exi component.
  \param[in]    VersionInfoPtr        Pointer to a memory location where the Exi version information shall be stored.
  \return
  \context      This function can be called in any context.
  \note         Service ID: see table 'Service IDs'\n
  \availability This function is only available if ExiVersionInfoApi is enabled.
 *********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, EXI_APPL_DATA) VersionInfoPtr)
{
  Exi_CheckDetErrorReturnVoid((NULL_PTR != VersionInfoPtr), EXI_API_ID_GET_VERSION_INFO, EXI_E_INV_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  VersionInfoPtr->vendorID = EXI_VENDOR_ID;
  VersionInfoPtr->moduleID = EXI_MODULE_ID;
  VersionInfoPtr->sw_major_version = EXI_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = EXI_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = EXI_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif
  /* EXI_VERSION_INFO_API */

/**********************************************************************************************************************
 *  Exi_MainFunction
 *********************************************************************************************************************/
/**
  \brief      This function is used for basic administration.
  \param[in]  void
  \return
  \pre        The Exi has to be initialized with a call of Exi_Init.
  \context    This function is called cyclically by the BSW Scheduler.
  \note       Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_MainFunction(void)
{
  /* Since main functions shall not throw DET errors in uninit case, this function is obsolete and may not be called */

  if ( EXI_STATE_INIT == Exi_State )
  { 
  } /* Exi_State */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_InitEncodeWorkspace()
 **********************************************************************************************************************/
/**
  \brief         This function is called to initialize an EXI encoding workspace. The encoding workspace is used to 
                 handle all data that is required to transform a schema conform data structures into a valid EXI stream.
  \param[in,out] EncWsPtr                    Pointer to EXI workspace containing the input and output data buffer
  \param[in,out] InBufPtr                    Pointer to EXI input data buffer (EXI struct)
  \param[in,out] OutBufPtr                   Pointer to EXI output data buffer (EXI stream)
                                             Give at least 1 byte of buffer for EXI stream length calculation (TX streaming)
  \param[in]     OutBufLen                   Maximum EXI output data buffer length
  \param[in]     OutBufOfs                   Byte offset in output buffer at which encoding starts
  \param[in]     StartWriteAtStreamPos       Start writing at stream byte position, skip writing bytes until this position.
                                             Set to value 0xFFFFFFFF to calculate EXI stream length without writing
  \param[in]     CalculateStreamLength       If TRUE the complete stream length will be calculated\n
                                             If FALSE encoding will be skipped after buffer is full
  \return        E_OK:                       Finished successfully\n
                 E_PENDING:                  More buffer requiered\n
                 E_NOT_OK:                   Error\n
  \pre          The Exi has to be initialized with a call of Exi_Init.
  \context      This function can be called in any context.
  \note         Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Std_ReturnType, EXI_CODE) Exi_InitEncodeWorkspace( 
  P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA) InBufPtr,
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
  P2VAR(IpBase_PbufType, AUTOMATIC, EXI_APPL_VAR) OutPBufPtr,
#else
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) OutBufPtr,
  uint16 OutBufLen,
#endif
  uint16 OutBufOfs
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
 ,uint32 StartWriteAtStreamPos
 ,boolean CalculateStreamLength
#endif
  )
{
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_NOT_INITIALIZED, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != InBufPtr), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
  Exi_CheckDetErrorReturnValue((NULL_PTR != OutPBufPtr), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != OutPBufPtr->payload), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != OutPBufPtr->totLen), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
  Exi_CheckDetErrorReturnValue(OutBufOfs < OutPBufPtr->totLen, EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != OutBufPtr), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != OutBufLen), EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
  Exi_CheckDetErrorReturnValue(OutBufOfs < OutBufLen, EXI_API_ID_INIT_ENCODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
#endif
  /* ------------------------------------------------- */

  Exi_VBSEncodeInitWorkspace(&EncWsPtr->EncWs,
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
                             OutPBufPtr,
#else
                             OutBufPtr,
                             OutBufLen,
#endif
                             OutBufOfs
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
                            ,StartWriteAtStreamPos
                            ,CalculateStreamLength
#endif
                             );
  EncWsPtr->InputData.StoragePtr = InBufPtr;

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_InitDecodeWorkspace()
 **********************************************************************************************************************/
/**
  \brief         This function is called to initialize an EXI decoding workspace.The decoding workspace is used to 
                 handle all data that is required to transform an EXI stream into a schema conform data structures.
  \param[in,out] DecWsPtr           Pointer to EXI workspace containing the input and output data buffer
  \param[in,out] InBufPtr           Pointer to EXI input data buffer (EXI stream)
  \param[in,out] OutBufPtr          Pointer to EXI output data buffer (EXI struct)
  \param[in]     InBufLen           EXI input data length
  \param[in]     OutBufLen          Maximum EXI output data buffer length
  \param[in]     InBufOfs           byte offset in input buffer at which decoding begings
  \return        E_OK:              Finished successfully\n
                 E_PENDING:         More buffer requiered\n
                 E_NOT_OK:          Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Std_ReturnType, EXI_CODE) Exi_InitDecodeWorkspace( 
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
  P2CONST(IpBase_PbufType, AUTOMATIC, EXI_APPL_DATA) InPBufPtr,
#else
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA) InBufPtr,
#endif
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) OutBufPtr,
#if EXI_ENABLE_PBUF_SUPPORT == STD_OFF
  uint16 InBufLen,
#endif
  uint16 OutBufLen,
  uint16 InBufOfs)
{
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 14 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_NOT_INITIALIZED, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != DecWsPtr), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != OutBufPtr), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
  Exi_CheckDetErrorReturnValue((NULL_PTR != InPBufPtr), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != InPBufPtr->payload), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != InPBufPtr->totLen), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((InBufOfs < InPBufPtr->totLen), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != InBufPtr), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_POINTER, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != InBufLen), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
  Exi_CheckDetErrorReturnValue((InBufOfs < InBufLen), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
#endif
  Exi_CheckDetErrorReturnValue((0 != OutBufLen), EXI_API_ID_INIT_DECODE_WORKSPACE, EXI_E_INV_PARAM, E_NOT_OK);
  /* ------------------------------------------------- */

#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
  Exi_VBSDecodeInitWorkspace(&DecWsPtr->DecWs, InPBufPtr, InBufOfs);
#else
  Exi_VBSDecodeInitWorkspace(&DecWsPtr->DecWs, InBufPtr, InBufLen, InBufOfs);
#endif
  DecWsPtr->OutputData.StoragePtr = OutBufPtr;
  DecWsPtr->OutputData.StorageLen = OutBufLen;
  DecWsPtr->OutputData.StorageOffset = 0;
  DecWsPtr->OutputData.RootElementId = EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE;
  DecWsPtr->OutputData.SchemaSetId = EXI_UNKNOWN_SCHEMA_SET_TYPE;

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_Encode
 *********************************************************************************************************************/
/**
  \brief         This function is used to generate a schema conform EXI stream based on the schema-informed document 
                 grammar that represents the data structure included in the encoding workspace.
  \param[in,out] EncWsPtr           Pointer to EXI workspace containing the input and output data buffer
  \return        EXI_E_OK:          Finished successfully\n
                 EXI_E_BUFFER_SIZE: Target buffer to small\n
                 E_NOT_OK:          Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \warning       Make sure Exi_InitEncodeWorkspace is called before this function gets called.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Exi_ReturnType, EXI_CODE) Exi_Encode( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{
  Exi_ReturnType RetValue = EXI_E_NOT_OK;

  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_ENCODE, EXI_E_NOT_INITIALIZED, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_ENCODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_ENCODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.BufLen), EXI_API_ID_ENCODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_API_ID_ENCODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#endif
  Exi_CheckDetErrorReturnValue((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_API_ID_ENCODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  /* ------------------------------------------------- */

  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId)
  {
    Exi_VBSWriteHeader(&EncWsPtr->EncWs);
    RetValue = EncWsPtr->EncWs.StatusCode;
    if(EXI_E_OK == RetValue)
    {
      Exi_Encode_SchemaRoot(EncWsPtr);
      RetValue = EncWsPtr->EncWs.StatusCode;
    }
  }
  
  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_EncodeFragment
 *********************************************************************************************************************/
/**
  \brief         This function is used to generate a schema conform EXI stream based on the schema-informed fragment 
                 grammar that represents the data structure included in the encoding workspace.
  \param[in,out] EncWsPtr           Pointer to EXI workspace containing the input and output data buffer
  \param[in]     Namespace          Namespace identifier
  \return        EXI_E_OK:          Finished successfully\n
                 EXI_E_BUFFER_SIZE: Target buffer to small\n
                 E_NOT_OK:          Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \warning       Make sure Exi_InitEncodeWorkspace is called before this function gets called.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Exi_ReturnType, EXI_CODE) Exi_EncodeFragment( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
                                       Exi_NamespaceIdType Namespace )
{
  Exi_ReturnType RetValue = EXI_E_NOT_OK;

  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_NOT_INITIALIZED, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_POINTER, EXI_E_NOT_OK);
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.BufLen), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#endif
  Exi_CheckDetErrorReturnValue((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_API_ID_ENCODE_FRAGMENT, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  /* ------------------------------------------------- */

  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId)
  {
    Exi_VBSWriteHeader(&EncWsPtr->EncWs);
    RetValue = EncWsPtr->EncWs.StatusCode;
    if(EXI_E_OK == RetValue)
    {
#if (defined(EXI_ENCODE_SCHEMA_FRAGMENT) && (EXI_ENCODE_SCHEMA_FRAGMENT == STD_ON))
      Exi_Encode_SchemaFragment(EncWsPtr, Namespace);
      RetValue = EncWsPtr->EncWs.StatusCode;
#else
      RetValue = EXI_E_DISABLED_FEATURE;
#endif /* #if (defined(EXI_ENCODE_SCHEMA_FRAGMENT) && (EXI_ENCODE_SCHEMA_FRAGMENT == STD_ON)) */
    }
  }
  
  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_FinalizeExiStream
 *********************************************************************************************************************/
/**
  \brief         Finalize an EXI stream. Padding will be added and EncWsPtr->EncWs.BytePos will indicate EXI stream length.
  \param[in,out] EncWsPtr           Pointer to EXI workspace containing the input and output data buffer\n
                                    See Exi_BSEncodeWorkspaceType declaration for out parameters (external)
  \return        EXI_E_OK:          Finished successfully\n
                 EXI_E_BUFFER_SIZE: Target buffer to small\n
                 E_NOT_OK:          Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \warning       Make sure Exi_InitEncodeWorkspace is called before this function gets called.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Exi_ReturnType, EXI_CODE) Exi_FinalizeExiStream( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{
  Exi_ReturnType RetValue = EXI_E_OK;

  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_ENCODE, EXI_E_NOT_INITIALIZED, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos <= EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_API_ID_ENCODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != EncWsPtr->EncWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos <= EncWsPtr->EncWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#endif
  /* ------------------------------------------------- */

  if(0 != EncWsPtr->EncWs.BitPos)
  {
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos <= EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
# else
    Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos <= EncWsPtr->EncWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
# endif
#else
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.PBufPtr[EncWsPtr->EncWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
# else
    Exi_CheckDetErrorReturnValue((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
# endif
#endif
    EncWsPtr->EncWs.BitPos = 0;
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
    if (TRUE == EncWsPtr->EncWs.WritingToBufferIsActive)
    {
      EncWsPtr->EncWs.BytePos++;
    }
    else
    {
      EncWsPtr->EncWs.OverallStreamBytePos++;
    }
#else
    EncWsPtr->EncWs.BytePos++;
#endif
  }
  
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
  /* Check if writing to the EXI stream is currently active -> this is last segment */
  if (TRUE == EncWsPtr->EncWs.WritingToBufferIsActive)
  {
    /* Writing to buffer is currently active -> Stream segment is shorter than buffer -> add segment length */
    EncWsPtr->EncWs.OverallStreamBytePos += EncWsPtr->EncWs.PBufProcessedElementBytes + EncWsPtr->EncWs.BytePos - EncWsPtr->EncWs.StartOffset;
  
    /* This is the last stream segment */
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    EncWsPtr->EncWs.CurrentStreamSegmentLen = EncWsPtr->EncWs.PBufProcessedElementBytes + EncWsPtr->EncWs.BytePos - EncWsPtr->EncWs.StartOffset;
# else
    EncWsPtr->EncWs.CurrentStreamSegmentLen = EncWsPtr->EncWs.BytePos - EncWsPtr->EncWs.StartOffset;
# endif
    EncWsPtr->EncWs.TotalStreamLength = EncWsPtr->EncWs.OverallStreamBytePos;
    EncWsPtr->EncWs.StreamComplete = TRUE;
  }
  else
  {
    /* EncWsPtr->EncWs.CurrentStreamSegmentLen already written in Exi_VBSWriteBits() or nothing was written */
    EncWsPtr->EncWs.StreamComplete = FALSE;

    /* Check if the stream length should be calculated */
    if (TRUE == EncWsPtr->EncWs.CalculateStreamLength)
    {
      EncWsPtr->EncWs.TotalStreamLength = EncWsPtr->EncWs.OverallStreamBytePos;
    }
    else
    {
      EncWsPtr->EncWs.TotalStreamLength = 0;
    }
  }
#else
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  EncWsPtr->EncWs.TotalStreamLength = EncWsPtr->EncWs.PBufProcessedElementBytes + EncWsPtr->EncWs.BytePos - EncWsPtr->EncWs.StartOffset;
# else
  EncWsPtr->EncWs.TotalStreamLength = EncWsPtr->EncWs.BytePos - EncWsPtr->EncWs.StartOffset;
# endif
#endif
  
  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_Decode
 *********************************************************************************************************************/
/**
  \brief         This function is used to decode an EXI stream and store the data structures in the decoding workspace 
                 output storage.
  \param[in,out] DecWsPtr                    Pointer to EXI workspace containing the input and output data buffer
  \return        EXI_E_OK:                   Finished successfully\n
                 EXI_E_INV_EVENT_CODE:       Unsupported event code\n
                 EXI_E_BUFFER_SIZE:          Target buffer to small\n
                 EXI_E_NOT_OK:               Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \warning       Make sure Exi_InitEncodeWorkspace is called before this function gets called.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
FUNC(Exi_ReturnType, EXI_CODE) Exi_Decode( P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_ReturnType RetValue = EXI_E_NOT_OK;

  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 15 */ /* MD_MSR_14.3 */
  Exi_CheckDetErrorReturnValue((EXI_STATE_UNINIT != Exi_State), EXI_API_ID_DECODE, EXI_E_NOT_INITIALIZED, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != DecWsPtr), EXI_API_ID_DECODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_API_ID_DECODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckDetErrorReturnValue((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_API_ID_DECODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != DecWsPtr->DecWs.PBufPtr[DecWsPtr->DecWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.PBufPtr[DecWsPtr->DecWs.PBufIdx].len), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#else
  Exi_CheckDetErrorReturnValue((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_API_ID_DECODE, EXI_E_INV_POINTER, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((0 != DecWsPtr->DecWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
#endif
  Exi_CheckDetErrorReturnValue((0 != DecWsPtr->OutputData.StorageLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  Exi_CheckDetErrorReturnValue((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_API_ID_DECODE, EXI_E_INV_PARAM, EXI_E_NOT_OK);
  /* ------------------------------------------------- */

  Exi_VBSReadHeader(&DecWsPtr->DecWs);
  RetValue = DecWsPtr->DecWs.StatusCode;
  if(EXI_E_OK == RetValue)
  {
    if(NULL_PTR != Exi_DecoderFcts[DecWsPtr->OutputData.SchemaSetId])
    {
      Exi_DecoderFcts[DecWsPtr->OutputData.SchemaSetId](DecWsPtr);
      RetValue = DecWsPtr->DecWs.StatusCode;
    }
    else
    {
      RetValue = EXI_E_NOT_OK;
    }
  }

  if (EXI_E_OK == RetValue)
  {
    if(0 != DecWsPtr->DecWs.BitPos)
    {
      DecWsPtr->DecWs.BitPos = 0;
      DecWsPtr->DecWs.BytePos++;
    }
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    DecWsPtr->DecWs.TotalStreamLength = DecWsPtr->DecWs.PBufProcessedElementBytes + DecWsPtr->DecWs.BytePos - DecWsPtr->DecWs.StartOffset;
#else
    DecWsPtr->DecWs.TotalStreamLength = DecWsPtr->DecWs.BytePos - DecWsPtr->DecWs.StartOffset;
#endif
  }
  
  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
#if (STD_ON == EXI_DEV_ERROR_DETECT) && ((!defined EXI_CONFIG_VARIANT) || (3u==EXI_CONFIG_VARIANT))
/**********************************************************************************************************************
  Exi_IsValidPostBuildConfigData
**********************************************************************************************************************/
STATIC FUNC(boolean, EXI_CODE) Exi_IsValidPostBuildConfigData(P2CONST(Exi_ConfigType, AUTOMATIC, EXI_CONST) Exi_ConfigPtr)
{
  boolean Result = FALSE;
  P2CONST(Exi_PostBuildConfigType, EXI_CONST, EXI_CONST) ConfigData = \
    (P2CONST(Exi_PostBuildConfigType, AUTOMATIC, EXI_CONST)) Exi_ConfigPtr;
  if(1)
  {
    Result = TRUE;
  }
  return Result;
}
#endif /*(STD_ON == EXI_DEV_ERROR_DETECT) && ((!defined EXI_CONFIG_VARIANT) || (3u==EXI_CONFIG_VARIANT)) */

#define EXI_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* module specific MISRA deviations:
 MD_Exi_1.1:
      Reason:     Nesting of control structures is exceeded because the generated state machine is based on XSD and EXI structure.
      Risk:       No risk.
      Prevention: Covered by code review.
 MD_Exi_5.1:
      Reason:     Identifiers are derived from the XSD schema definition. Therefor some names do not differ within the first 31 characters.
      Risk:       No risk, this is generated code.
      Prevention: Not required.
 MD_Exi_5.6:
      Reason:     Elements with multiple occurences in the schema need to include a pointer to a structure from the same type. Therefor this typedef is required.
      Risk:       No risk, this is generated code.
      Prevention: Covered by code review.
 MD_Exi_11.4:
      Reason:     EXI data is stored in the EXI workspace storage. Structures are placed inside this storage container and linked 
                  using pointers to sub-structures. Casting to the required structure type is required here.
      Risk:       In all cases it has to be ensured, that the storage size is checked first and when placing structures inside
                  the EXI storage area the memory has to be cleared completely.
      Prevention: Covered by code review.
 MD_Exi_11.5:
      Reason:     EXI data is stored in the EXI workspace storage. Structures are placed inside this storage container and linked 
                  using pointers to sub-structures. Casting to the required structure type is required here.
      Risk:       In all cases it has to be ensured, that element is casted to the correct type by checking the elements ID.
      Prevention: Covered by code review.
 MD_Exi_13.6:
      Reason:     Control instruction modified inside the body of the loop to set the value to the current elements (multiple occurence element). This allows a max check 
                  against the schema defined value.
      Risk:       If not followed by a break this could lead to an error.
      Prevention: Covered by code review.
 MD_Exi_13.7:
      Reason:     This condition is always true based on configuration.
      Risk:       No risk.
      Prevention: Covered by code review.
 MD_Exi_14.5:
      Reason:     Continue statement used when EE event already encoded.
      Risk:       No risk.
      Prevention: Covered by code review.
 MD_Exi_14.6:
      Reason:     The break statement is used multiple times to prevent complex nesting of control statements. Unstructured code should be avoided.
      Risk:       No risk. If the break statement is removed an additional if-statement is required with an empty else block.
      Prevention: Covered by code review.
 MD_Exi_16.2:
      Reason:     Generic elements may include other generic elements. Therefor recursion is required for this elements.
      Risk:       No risk. Debugging code only. Schema deviation support shall be switched of in production code.
      Prevention: Covered by code review.
 MD_Exi_18.4:
      Reason:     XSD choice elements are represented as unions for optimization reason. Flags ensure that the application selects the correct type.
      Risk:       No risk, this is generated code.
      Prevention: Covered by code review.
*/
/**********************************************************************************************************************
 *  END OF FILE: Exi.c
 *********************************************************************************************************************/
