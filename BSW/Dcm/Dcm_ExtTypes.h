/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_ExtTypes.h
 *        \brief  Public data type definition interface of DCM
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#if !defined(DCM_EXTTYPES_H)
# define DCM_EXTTYPES_H
/* ----------------------------------------------
 ~&&&   Versions
---------------------------------------------- */

/*! Implementation version */
# define DCM_EXTTYPES_MAJOR_VERSION                                  8u
# define DCM_EXTTYPES_MINOR_VERSION                                  3u
# define DCM_EXTTYPES_PATCH_VERSION                                  1u
# if (DCM_EXTENSION_ID != 0x96ACD543UL)
#  error "Mismatching OEMs between static and generated code"
# endif
/* ----------------------------------------------
 ~&&&   Typedefs
---------------------------------------------- */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_DCM_UNSUPPORTED XF */
typedef uint8  Dcm_ExtSvc86STRTStateType;
typedef uint8  Dcm_ExtSvc86RoeStateType;
struct DCM_EXTSVC86NVMDATATYPE_TAG
{
  Dcm_ExtSvc86RoeStateType    RoEState;       /* internal state of RoE Event */
  uint8                       DTCStatusMask;
  PduIdType                   RxPduId;        /* Id of connection currently requesting RoE */
  uint16                      ClientSrcAddr;  /* The testers source identifier */
};
typedef struct DCM_EXTSVC86NVMDATATYPE_TAG Dcm_ExtSvc86NvMDataType;
# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_EXTTYPES_H) */
/* ********************************************************************************************************************
 * END OF FILE: Dcm_ExtTypes.h
 * ******************************************************************************************************************** */
