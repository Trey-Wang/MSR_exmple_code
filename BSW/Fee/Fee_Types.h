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
 *         File:  Fee_Types.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and 
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as 
 *                a virtually unlimited number of erase cycles to the upper layer.
 *                
 *                This file provides all public FEE types. It may be included directly by other
 *                SW, but Fee.h should be preferred.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined (FEE_TYPES_H)
# define FEE_TYPES_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_TYPES_MAJOR_VERSION                                   (8u)
# define FEE_TYPES_MINOR_VERSION                                   (1u)
# define FEE_TYPES_PATCH_VERSION                                   (2u)

#include "Std_Types.h"
#include "MemIf_Types.h"

/******************************************************************************
 *  PUBLISHED INFORMATION
 *****************************************************************************/
# define FEE_BLOCK_OVERHEAD                                           (2u)
# define FEE_PAGE_OVERHEAD                                            (0u)

/******************************************************************************
 *  ERROR CODES IN DEVELOPMENT-MODE
 *****************************************************************************/
typedef enum {
    FEE_E_NO_ERROR = 0u,
    FEE_E_INVALID_BLOCK_NO =                           (0x02u),

    FEE_E_PARAM_DATABUFFERPTR =                        (0x10u),
    FEE_E_PARAM_SECTOR_NUMBER =                        (0x11u),
    FEE_E_PARAM_LENGTH_OFFSET =                        (0x12u),

    FEE_E_BUSY =                                       (0x13u),
    FEE_E_NO_INIT =                                    (0x14u),
    FEE_E_PARAM_MODE =                                 (0x15u) /* not used; exists for compatibility reasons, only */
} Fee_DetErrorCodeType;

/******************************************************************************
 *  FEE SERVICE IDENTIFICATION
 *****************************************************************************/
# define FEE_SID_INIT                                                 (0x00u)
# define FEE_SID_SETMODE                                              (0x01u)
# define FEE_SID_READ                                                 (0x02u)
# define FEE_SID_WRITE                                                (0x03u)
# define FEE_SID_CANCEL                                               (0x04u)
# define FEE_SID_GETSTATUS                                            (0x05u)
# define FEE_SID_GETJOBRESULT                                         (0x06u)
# define FEE_SID_INVALIDATEBLOCK                                      (0x07u)
# define FEE_SID_GETVERSIONINFO                                       (0x08u)
# define FEE_SID_ERASEIMMEDIATEBLOCK                                  (0x09u)
# define FEE_SID_JOBENDNOTIFICATION                                   (0x10u)
# define FEE_SID_JOBERRORNOTIFICATION                                 (0x11u)
# define FEE_SID_MAINFUNCTION                                         (0x12u)

# define FEE_SID_GET_ERASE_CYCLE                                      (0x20u)
# define FEE_SID_GET_WRITE_CYCLE                                      (0x21u)
# define FEE_SID_GET_SECTOR_SWITCH_STATUS                             (0x22u)
# define FEE_SID_FORCE_SECTOR_SWITCH                                  (0x23u)

#define FEE_SID_CONVERT_BLOCK_CONFIG                                  (0x24u)

#define FEE_SID_SUSPEND_WRITE                                         (0x30u)
#define FEE_SID_RESUME_WRITE                                          (0x31u)
#define FEE_SID_DISABLE_FSS                                           (0x32u)
#define FEE_SID_ENABLE_FSS                                            (0x33u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) Fee_UserDataPtrType;
typedef P2CONST(uint8, AUTOMATIC, FEE_APPL_DATA) Fee_UserDataConstPtrType;

typedef P2VAR(uint32, AUTOMATIC, FEE_APPL_DATA) Fee_UserData32PtrType;

typedef P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_APPL_DATA) Fee_StdVersionInfoRefType;

typedef P2CONST(void, AUTOMATIC, FEE_APPL_DATA) Fee_UserDataVoidConstPtrType;

/* ALWAYS provide related types; even if DataConversion API was disabled by configuration,
 * it does not consume any run-time resources */

/* BlockConvCbkReturnType -> is just an uint8 type, enumerators are defines */
#define FEE_CONVERSION_WRITE_OLD_LENGTH 0
#define FEE_CONVERSION_WRITE_NEW_LENGTH 1
#define FEE_CONVERSION_SKIP             2

typedef P2FUNC(uint8, FEE_APPL_CODE, Fee_DataConversionCallbackType) (Fee_UserDataPtrType userBuffer,
                                                                      uint32 blockId,
                                                                      uint16 oldLength,
                                                                      uint16 newLength);

typedef struct /*< Data Conversion  structure, to be passed by caller to Fee_ConvertBlockConfig */
{
    Fee_UserDataPtrType             userBuffer; /*<! pointer to user buffer; must be large enough to hold
                                                     any data FEE can deliver or it shall write */
    Fee_DataConversionCallbackType  notificationPtr; /*<! user's callback */
} Fee_ConversionOptionsType;
typedef P2CONST(Fee_ConversionOptionsType, AUTOMATIC, FEE_APPL_CONFIG) Fee_ConversionOptionsConstRefType;

typedef enum
{
  FEE_SECTOR_SWITCH_IDLE = 0u,
  FEE_SECTOR_SWITCH_BLOCK_COPY,
  FEE_SECTOR_SWITCH_ERASE,
  FEE_SECTOR_SWITCH_UNINIT
} Fee_SectorSwitchStatusType;

typedef enum
{
  FEE_SECTOR_OVERFLOW = 0u,
  FEE_SECTORS_CORRUPTED,
  FEE_SECTOR_FORMAT_FAILED,
  FEE_SECTOR_CRITICAL_FILL_LEVEL
} Fee_SectorErrorType;

typedef uint8 Fee_ErrorCallbackReturnType;
#define FEE_ERRCBK_REJECT_WRITE                                       (0x0u)
#define FEE_ERRCBK_REJECT_ALL                                         (0x1u)
#define FEE_ERRCBK_RESOLVE_AUTOMATICALLY                              (0x80)

#endif 

/**********************************************************************************************************************
 *  END OF FILE: Fee_Types.h
 *********************************************************************************************************************/
