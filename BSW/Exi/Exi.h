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
 *         File:  Exi.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  module header
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Daniel Dausend                visdd         Vector Informatik GmbH
 *  Frederik Dornemann            visfdn        Vector Informatik GmbH
 *  Patrick Sommer                vissop        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-02-01  visdd   -             created
 *  01.00.01  2012-04-19  visdd   -             Encoder bugfix: multiple optional elements with substitution group
 *                                -             bugfix MemMap and CompilerCfg
 *  01.01.00  2012-07-04  visdd   ESCAN00058666 Support final DIN 70121 schema
 *                                ESCAN00058667 Offset calculation for range limited values
 *                                ESCAN00059066 Bugfix negative integer decoding
 *                        visfdn  ESCAN00058734 Enhance Encoding / Decoding by Offset
 *                                ESCAN00059073 Change return values of failed internal Det checks
 *                                ESCAN00059077 Decoding of string and byte-arrays fails under certain circumstances
 *                                ESCAN00059078 DET check missing is some internal functions
 *                                ESCAN00059119 Int decoding functions return incorrect result if value is too large
 *                                ESCAN00059111 Exi_VBSEncodeBytes() returns E_OK even if buffer is too small
 *                        visdd   ESCAN00059287 Using new Exi_ReturnType instead of Std_ReturnType
 *                                ESCAN00059448 Bugfix De-/Encoding UInt8 > 127 and negative Int8 values
 *                                ESCAN00059587 Basic schema deviation support
 *                        visfdn  ESCAN00059318 Bugfix decoding UInt > 32bit
 *                        visdd   ESCAN00059715 Encoding error for CurrentDemandReq depending on optional parameters
 *                                ESCAN00059905 Added support for XSD choice elements
 *  01.01.01  2012-09-27  visdd   ESCAN00060205 Bugfix buffer elements (strings, binary data) with MaxOccurs > 1
 *                                ESCAN00060304 Bugfix encoding PowerDeliveryReq with ChargingProfile
 *                                ESCAN00061353 Bugfix CurrentDemandRes encoding
 *                                ESCAN00061356 Added EXI_E_DISABLED_FEATURE return code
 *                                ESCAN00061379 Bugfix: ChargingProfile enabled for basic message set
 *                        visfdn  ESCAN00061516 Exi_VBSDecodeSkipBits() sets bit position after EOS
 *  01.02.00  2012-09-28  visdd   ESCAN00061406 Removed return values and conditional checks to reduce code size and 
 *                                                improve readability
 *                                -             Added StatusCode to encoding and decoding workspace
 *  01.03.00  2013-01-16  visdd   ESCAN00063173 Add configuration support for unbounded buffer elements
 *                                ESCAN00062410 XML Security support: Added possiblity to encode all root elements
 *                                ESCAN00061243 Map xsd:long and xsd:unsignedLong to 64 bit integer values
 *                                ESCAN00064184 Compiler error in Exi_InitDecodeWorkspace if PBuf support disabled and 
 *                                                det enabled
 *                                ESCAN00064179 Support schema-informed fragment grammar encoding for XML Signatures
 *  01.03.01  2013-07-05  visdd   ESCAN00066902 EXI fails to decode document streams that do not start with V2G_Message
 *                                ESCAN00066944 Generated fragment stream is invalid for some messages
 *                                ESCAN00068523 EXI fails to decode/encode integers of arbitrary length
 *                                ESCAN00068674 Encoding/Decoding of Base64 encoded binary data not possible for length
 *                                                limited elements
 *                                ESCAN00068813 Decoding of supportedAppProtocolReq message with exactly 20 AppProtocol elements fails
 *  01.03.02  2013-07-23  visdd   ESCAN00069290 Encoding and Decoding of multiple choice elements fails
 *  01.03.03  2013-10-01  visdd   ESCAN00069263 AR4-450: Usage of section PBCFG in PB files
 *  02.00.00  2013-11-05  visdd   ESCAN00071678 Support ISO 15118 FDIS schema version
 *  02.00.01  2014-02-20  visdd   ESCAN00073827 Compiler error: identifier "Exi_XMLSIG_SignatureType" is undefined
 *                                              Wrong encoding of eMAID (CertificateInstallation/UpdateRes)
 *  02.01.00  2014-03-03  visdd   ESCAN00074027 Customer specific extensions
 *                                ESCAN00073480 Wrong fragment encoding for eMAID XML Fragment
 *  02.01.01  2014-05-27  vissop  ESCAN00074805 Fix: Enable string encoding for ISO and DIN
 *  02.02.00  2014-06-10  vissop  ESCAN00076165 Add: Customer specific extensions
 *  03.00.00  2014-07-18  vissop  ESCAN00077211 Adapt package attributes
 *            2014-08-08  vissop  ESCAN00077713 Fix: Decoder PBuf handling to support more than two segments
 *            2014-08-08  vissop  ESCAN00077725 Remove single source S from Vector internal filenames
 *            2014-08-08  vissop  ESCAN00077726 Remove version checks from various files
 *  03.00.01  2014-08-22  vissop  ESCAN00078008 Fix: Encoder PBuf handling to support more than two segments
 *            2014-08-25  vissop  ESCAN00078016 Customer specific: Fixed bug in customer specific schema
 *  03.00.02  2014-08-28  vissop  ESCAN00078123 Fix: Decode error in case last sub element is optional and not present
 *  03.00.03  2014-09-29  vissop  ESCAN00078623 Fix: EXI decode may read from invalid memory after truncated string
 *  03.00.04  2014-10-29  vissop  ESCAN00079205 Fix: 64 bit data type usage on 16 bit platforms
 *  03.01.00  2014-10-27  vissop  ESCAN00079157 Add: EXI simple TX streaming support
 *            2014-12-15  vissop  ESCAN00079971 Fix EVSE only: Wrong encoding in case an optional element is missing in
 *                                              front of a list element
 *  03.02.00  2015-04-16  vissop  ESCAN00081584 Add: Configurable manual structure padding for 16 and 32 bit
 *  03.02.01  2015-06-12  vissop  ESCAN00083369 Fix EVSE only: Wrong encoding of the ISO CurrentDemandRes with missing optional elements
 *            2015-06-12  vissop  ESCAN00083250 Fix: Compiler error in case PBuf support is disabled
 *  03.02.02  2015-07-07  vissop  ESCAN00083840 Fix: Compiler warning with EXI struct padding default configuration
 *  03.02.03  2015-09-02  vissop  ESCAN00083713 Fix: Remove extended version for MICROSAR 4
 *  03.02.04  2016-02-26  vissop  ESCAN00088287 Fix: ISO15118: Optional PnC elements cannot be decoded in case PnC charging is disabled
 *            2016-03-01  vissop  ESCAN00088649 Fix: ISO15118: MeterReading value cannot be bigger than uint32 maximum while encoding
 *  03.02.05  2016-12-07  vissop  ESCAN00092055 Fix: MainFunction shall not throw DET's in UNINIT case
 *********************************************************************************************************************/

#if !defined (EXI_H)
# define EXI_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"
#include "Exi_Lcfg.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define SYSSERVICE_EXI_VERSION          (0x0302u) /* BCD coded version number */
#define SYSSERVICE_EXI_RELEASE_VERSION  (0x05u)   /* BCD coded release version number */

/* software version defines */
#define EXI_SW_MAJOR_VERSION                      3u
#define EXI_SW_MINOR_VERSION                      2u
#define EXI_SW_PATCH_VERSION                      5u

/* Exi ModuleId */
#define EXI_VENDOR_ID                                 30u   /* Vector ID */
#define EXI_MODULE_ID                                255u   /* CDD module ID used for EXI */
#define EXI_INSTANCE_ID                              107u   /* EXI is identified via instance ID */
#define EXI_ENCODER_INSTANCE_ID                      108u   /* EXI Encoder is identified via instance ID */
#define EXI_DECODER_INSTANCE_ID                      109u   /* EXI Decoder is identified via instance ID */
/* Exi ApiIds */
#define EXI_API_ID_INIT                             0x01u
#define EXI_API_ID_MAIN_FUNCTION                    0x02u
#define EXI_API_ID_INIT_DECODE_WORKSPACE            0x03u
#define EXI_API_ID_INIT_ENCODE_WORKSPACE            0x04u
#define EXI_API_ID_DECODE                           0x05u
#define EXI_API_ID_ENCODE                           0x06u
#define EXI_API_ID_FINALIZE_EXI_STREAM              0x07u
#define EXI_API_ID_GET_VERSION_INFO                 0x08u
#define EXI_API_ID_V_INTERNAL_FUNCTION              0x09u
#define EXI_API_ID_ENCODE_FRAGMENT                  0x0Au

/* EXI DET errors */
#define EXI_E_NOT_INITIALIZED                       0x01u
#define EXI_E_INV_POINTER                           0x02u
#define EXI_E_INV_PARAM                             0x03u
#define EXI_E_INVALID_PB_CONFIG                     0x04u
#define EXI_E_INVALID_PBBUF_POS                     0x05u

/* EXI state */
#define EXI_STATE_UNINIT                     (uint8)0x00u
#define EXI_STATE_INIT                       (uint8)0x01u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */
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
extern FUNC(void, EXI_CODE) Exi_InitMemory(void);

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
extern FUNC(void, EXI_CODE)Exi_Init(P2CONST(Exi_ConfigType, AUTOMATIC, EXI_CONST) Exi_ConfigPtr);

#if EXI_VERSION_INFO_API == STD_ON
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
extern FUNC(void, EXI_CODE) Exi_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EXI_APPL_DATA) VersionInfoPtr);
#endif
  /* EXI_VERSION_INFO_API == STD_ON */

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
extern FUNC(void, EXI_CODE) Exi_MainFunction(void);

/**********************************************************************************************************************
 *  Exi_InitEncodeWorkspace()
 **********************************************************************************************************************/
/**
  \brief         This function is called to initialize an EXI encoding workspace. The encoding workspace is used to 
                 handle all data that is required to transform a schema conform data structures into a valid EXI stream.
  \param[in,out] EncWsPtr                    Pointer to EXI workspace containing the input and output data buffer
  \param[in,out] InBufPtr                    Pointer to EXI input data buffer (EXI struct)
  \param[in,out] OutBufPtr                   Pointer to EXI output data buffer (EXI stream)\n
                                             Give at least 1 byte of buffer for EXI stream length calculation (TX streaming)
  \param[in]     OutBufLen                   Maximum EXI output data buffer length
  \param[in]     OutBufOfs                   Byte offset in output buffer at which encoding starts
  \param[in]     StartWriteAtStreamPos       Start writing at stream byte position, skip writing bytes until this position.\n
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
extern FUNC(Std_ReturnType, EXI_CODE) Exi_InitEncodeWorkspace( 
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
  );

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
extern FUNC(Std_ReturnType, EXI_CODE) Exi_InitDecodeWorkspace( 
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
  uint16 InBufOfs);

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
extern FUNC(Exi_ReturnType, EXI_CODE) Exi_Encode( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );

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
extern FUNC(Exi_ReturnType, EXI_CODE) Exi_EncodeFragment( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
                                       Exi_NamespaceIdType Namespace );

/**********************************************************************************************************************
 *  Exi_FinalizeExiStream
 *********************************************************************************************************************/
/**
  \brief         Finalize an EXI stream. Padding will be added and EncWsPtr->EncWs.BytePos will indicate EXI stream length.
  \param[in,out] EncWsPtr           Pointer to EXI workspace containing the input and output data buffer
  \return        EXI_E_OK:          Finished successfully\n
                 EXI_E_BUFFER_SIZE: Target buffer to small\n
                 E_NOT_OK:          Error\n
  \pre           The Exi has to be initialized with a call of Exi_Init.
  \warning       Make sure Exi_InitEncodeWorkspace is called before this function gets called.
  \context       This function can be called in any context.
  \note          Service ID: see table 'Service IDs'\n
 *********************************************************************************************************************/
extern FUNC(Exi_ReturnType, EXI_CODE) Exi_FinalizeExiStream( P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );

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
extern FUNC(Exi_ReturnType, EXI_CODE) Exi_Decode( P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */
#endif 
  /* EXI_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi.h
 *********************************************************************************************************************/
