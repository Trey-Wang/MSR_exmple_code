/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  vCanCcGbt_Int.h
 *       Module:  vCanCcGbt
 *
 *  Description:  Internal header of the vCanCcGbt module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  MISRA / PClint JUSTIFICATIONS
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA S 3453 EOF */ /* MD_MSR_19.7 */
/* *INDENT-ON* */

#if !defined (VCANCCGBT_INT_H)
# define VCANCCGBT_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "vCanCcGbt_Cfg.h"
# include "vCanCcGbt_Lcfg.h"
# include "vCanCcGbt_PBcfg.h"

# include "vCanCcGbt.h"

# include "ComStack_Types.h"

# if ((VCANCCGBT_DEV_ERROR_REPORT == STD_ON) || (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON))
#  include "Det.h"
# endif

/**********************************************************************************************************************
 *  INTERNAL CONSTANT MACROS
 *********************************************************************************************************************/
# define VCANCCGBT_SNA_VALUE                                      (0xFFu)

# define VCANCCGBT_SOLE_VALUE                                     (0u)
# define VCANCCGBT_SENDTIMEOUT_INIT                               (1u)
# define VCANCCGBT_BMS_IS_READY                                   (0xAA)

# define VCANCCGBT_BHM_LENGTH                                     (2u)
# define VCANCCGBT_BHM_TMPCV_POS                                  (0u)

# define VCANCCGBT_BRM_LENGTH                                     (49u)

# define VCANCCGBT_BRM_VERSION_POS                                (0u)
# define VCANCCGBT_BRM_VERSION_LENGTH                             (3u)
# define VCANCCGBT_BRM_BT_POS                                     (3u)
# define VCANCCGBT_BRM_PSBSC_POS                                  (4u)
# define VCANCCGBT_BRM_PSBSC_LENGTH                               (2u)
# define VCANCCGBT_BRM_PSBSV_POS                                  (6u)
# define VCANCCGBT_BRM_PSBSV_LENGTH                               (2u)
# define VCANCCGBT_BRM_BMN_POS                                    (8u)
# define VCANCCGBT_BRM_BMN_LENGTH                                 (4u)
# define VCANCCGBT_BRM_BPN_POS                                    (12u)
# define VCANCCGBT_BRM_BPN_LENGTH                                 (4u)
# define VCANCCGBT_BRM_BSPDY_POS                                  (16u)
# define VCANCCGBT_BRM_BSPDY_LENGTH                               (1u)
# define VCANCCGBT_BRM_BSPDM_POS                                  (17u)
# define VCANCCGBT_BRM_BSPDM_LENGTH                               (1u)
# define VCANCCGBT_BRM_BSPDD_POS                                  (18u)
# define VCANCCGBT_BRM_BSPDD_LENGTH                               (1u)
# define VCANCCGBT_BRM_BSCF_POS                                   (19u)
# define VCANCCGBT_BRM_BSCF_LENGTH                                (3u)
# define VCANCCGBT_BRM_BSPRM_POS                                  (22u)
# define VCANCCGBT_BRM_BSPRM_LENGTH                               (1u)
# define VCANCCGBT_BRM_RESERVED_POS                               (23u)
# define VCANCCGBT_BRM_VIN_POS                                    (24u)
# define VCANCCGBT_BRM_VIN_LENGTH                                 (17u)
# define VCANCCGBT_BRM_BSVN_POS                                   (41u)
# define VCANCCGBT_BRM_BSVN_LENGTH                                (8u)

# define VCANCCGBT_BCP_LENGTH                                     (13u)
# define VCANCCGBT_BCP_MPCVSB_POS                                 (0u)
# define VCANCCGBT_BCP_MPCVSB_LENGTH                              (2u)
# define VCANCCGBT_BCP_MPCC_POS                                   (2u)
# define VCANCCGBT_BCP_MPCC_LENGTH                                (2u)
# define VCANCCGBT_BCP_TNEPSB_POS                                 (4u)
# define VCANCCGBT_BCP_TNEPSB_LENGTH                              (2u)
# define VCANCCGBT_BCP_TMPCV_POS                                  (6u)
# define VCANCCGBT_BCP_TMPV_LENGTH                                (2u)
# define VCANCCGBT_BCP_MPT_POS                                    (8u)
# define VCANCCGBT_BCP_CSPSBV_POS                                 (9u)
# define VCANCCGBT_BCP_CSPSBV_LENGTH                              (2u)
# define VCANCCGBT_BCP_CBVPSBV_POS                                (11u)
# define VCANCCGBT_BCP_CBVPSBV_LENGTH                             (2u)

# define VCANCCGBT_BRO_LENGTH                                     (1u)
# define VCANCCGBT_BRO_IBRFC_POS                                  (0u)

# define VCANCCGBT_BCL_LENGTH                                     (5u)
# define VCANCCGBT_BCL_VD_POS                                     (0u)
# define VCANCCGBT_BCL_VD_LENGTH                                  (2u)
# define VCANCCGBT_BCL_CD_POS                                     (2u)
# define VCANCCGBT_BCL_CD_LENGTH                                  (2u)
# define VCANCCGBT_BCL_CM_POS                                     (4u)
# define VCANCCGBT_BCL_CM_LENGTH                                  (1u)

# define VCANCCGBT_BCS_LENGTH                                     (9u)
# define VCANCCGBT_BCS_MCV_POS                                    (0u)
# define VCANCCGBT_BCS_MCV_LENGTH                                 (2u)
# define VCANCCGBT_BCS_MCC_POS                                    (2u)
# define VCANCCGBT_BCS_MCC_LENGTH                                 (2u)
# define VCANCCGBT_BCS_HVB_POS                                    (4u)
# define VCANCCGBT_BCS_HVB_LENGTH                                 (2u)
# define VCANCCGBT_BCS_CCS_POS                                    (6u)
# define VCANCCGBT_BCS_CCS_LENGTH                                 (1u)
# define VCANCCGBT_BCS_ERT_POS                                    (7u)
# define VCANCCGBT_BCS_ERT_LENGTH                                 (2u)

# define VCANCCGBT_BSM_LENGTH                                     (7u)
# define VCANCCGBT_BSM_SNHVB_POS                                  (0u)
# define VCANCCGBT_BSM_HTB_POS                                    (1u)
# define VCANCCGBT_BSM_SNHTDP_POS                                 (2u)
# define VCANCCGBT_BSM_LTB_POS                                    (3u)
# define VCANCCGBT_BSM_SNLTDP_POS                                 (4u)
# define VCANCCGBT_BSM_BYTE6_POS                                  (5u)
# define VCANCCGBT_BSM_BYTE7_POS                                  (6u)

# define VCANCCGBT_BMV_LENGTH                                     (512u)
# define VCANCCGBT_BMV_SPSBV_LENGTH                               (2u)
# define VCANCCGBT_BMV_ELEMENT_CNT                                (VCANCCGBT_BMV_LENGTH / VCANCCGBT_BMV_SPSBV_LENGTH)

# define VCANCCGBT_BMT_LENGTH                                     (128u)
# define VCANCCGBT_BMT_FIELD_LENGTH                               (1u)

# define VCANCCGBT_BSP_LENGTH                                     (16u)
# define VCANCCGBT_BSP_FIELD_LENGTH                               (1u)

# define VCANCCGBT_BST_LENGTH                                     (4u)
# define VCANCCGBT_BST_CBSC_POS                                   (0u)
# define VCANCCGBT_BST_FCBSC_POS                                  (1u)
# define VCANCCGBT_BST_FCBSC_LENGTH                               (2u)
# define VCANCCGBT_BST_ECBSC_POS                                  (3u)

# define VCANCCGBT_BSD_LENGTH                                     (7u)
# define VCANCCGBT_BSD_SSC_POS                                    (0u)
# define VCANCCGBT_BSD_MIVSB_POS                                  (1u)
# define VCANCCGBT_BSD_MIVSB_LENGTH                               (2u)
# define VCANCCGBT_BSD_MAVSB_POS                                  (3u)
# define VCANCCGBT_BSD_MAVSB_LENGTH                               (2u)
# define VCANCCGBT_BSD_MIT_POS                                    (5u)
# define VCANCCGBT_BSD_MAT_POS                                    (6u)

# define VCANCCGBT_BEM_LENGTH                                     (4u)
# define VCANCCGBT_BEM_BYTE1_POS                                  (0u)
# define VCANCCGBT_BEM_BYTE2_POS                                  (1u)
# define VCANCCGBT_BEM_BYTE3_POS                                  (2u)
# define VCANCCGBT_BEM_BYTE4_POS                                  (3u)
# define VCANCCGBT_BEM_CHM_TIMEOUT_MASK                           (0x1u)
# define VCANCCGBT_BEM_CRM_TIMEOUT_MASK                           (0x4u)
# define VCANCCGBT_BEM_CTS_TIMEOUT_MASK                           (0x1u)
# define VCANCCGBT_BEM_CML_TIMEOUT_MASK                           (0x1u)
# define VCANCCGBT_BEM_CRO_TIMEOUT_MASK                           (0x4u)
# define VCANCCGBT_BEM_CCS_TIMEOUT_MASK                           (0x1u)
# define VCANCCGBT_BEM_CST_TIMEOUT_MASK                           (0x4u)
# define VCANCCGBT_BEM_CSD_TIMEOUT_MASK                           (0x1u)

# define VCANCCGBT_CHM_LENGTH                                     (3u)
# define VCANCCGBT_CHM_BYTE1_POS                                  (0u)
# define VCANCCGBT_CHM_BYTE2_POS                                  (1u)
# define VCANCCGBT_CHM_BYTE3_POS                                  (2u)

# define VCANCCGBT_CRM_LENGTH                                     (8u)
# define VCANCCGBT_CRM_RR_POS                                     (0u)
# define VCANCCGBT_CRM_CN_LO_POS                                  (1u)
# define VCANCCGBT_CRM_CN_MI_LO_POS                               (2u)
# define VCANCCGBT_CRM_CN_MI_HI_POS                               (3u)
# define VCANCCGBT_CRM_CN_HI_POS                                  (4u)
# define VCANCCGBT_CRM_CSLC_POS                                   (5u)
# define VCANCCGBT_CRM_CSLC_LENGTH                                (3u)
# define VCANCCGBT_CRM_CSLC_TERMINATION_POS                       (3u)
# define VCANCCGBT_CRM_RECOGNITION_RESULT_00                      (0x00)
# define VCANCCGBT_CRM_RECOGNITION_RESULT_AA                      (0xAA)

# define VCANCCGBT_CTS_LENGTH                                     (7u)
# define VCANCCGBT_CTS_BYTE1_POS                                  (0u)
# define VCANCCGBT_CTS_BYTE2_POS                                  (1u)
# define VCANCCGBT_CTS_BYTE3_POS                                  (2u)
# define VCANCCGBT_CTS_BYTE4_POS                                  (3u)
# define VCANCCGBT_CTS_BYTE5_POS                                  (4u)
# define VCANCCGBT_CTS_BYTE6_POS                                  (5u)
# define VCANCCGBT_CTS_BYTE7_POS                                  (6u)

# define VCANCCGBT_CML_LENGTH                                     (8u)
# define VCANCCGBT_CML_BYTE1_POS                                  (0u)
# define VCANCCGBT_CML_BYTE2_POS                                  (1u)
# define VCANCCGBT_CML_BYTE3_POS                                  (2u)
# define VCANCCGBT_CML_BYTE4_POS                                  (3u)
# define VCANCCGBT_CML_BYTE5_POS                                  (4u)
# define VCANCCGBT_CML_BYTE6_POS                                  (5u)
# define VCANCCGBT_CML_BYTE7_POS                                  (6u)
# define VCANCCGBT_CML_BYTE8_POS                                  (7u)

# define VCANCCGBT_CRO_LENGTH                                     (1u)
# define VCANCCGBT_CRO_BYTE1_POS                                  (0u)

# define VCANCCGBT_CCS_LENGTH                                     (8u)
# define VCANCCGBT_CCS_BYTE1_POS                                  (0u)
# define VCANCCGBT_CCS_BYTE2_POS                                  (1u)
# define VCANCCGBT_CCS_BYTE3_POS                                  (2u)
# define VCANCCGBT_CCS_BYTE4_POS                                  (3u)
# define VCANCCGBT_CCS_BYTE5_POS                                  (4u)
# define VCANCCGBT_CCS_BYTE6_POS                                  (5u)
# define VCANCCGBT_CCS_BYTE7_POS                                  (6u)
# define VCANCCGBT_CCS_BYTE8_POS                                  (7u)

# define VCANCCGBT_CST_LENGTH                                     (4u)
# define VCANCCGBT_CST_BYTE1_POS                                  (0u)
# define VCANCCGBT_CST_BYTE2_POS                                  (1u)
# define VCANCCGBT_CST_BYTE3_POS                                  (2u)
# define VCANCCGBT_CST_BYTE4_POS                                  (3u)

# define VCANCCGBT_CSD_LENGTH                                     (8u)
# define VCANCCGBT_CSD_BYTE1_POS                                  (0u)
# define VCANCCGBT_CSD_BYTE2_POS                                  (1u)
# define VCANCCGBT_CSD_BYTE3_POS                                  (2u)
# define VCANCCGBT_CSD_BYTE4_POS                                  (3u)
# define VCANCCGBT_CSD_BYTE5_POS                                  (4u)
# define VCANCCGBT_CSD_BYTE6_POS                                  (5u)
# define VCANCCGBT_CSD_BYTE7_POS                                  (6u)
# define VCANCCGBT_CSD_BYTE8_POS                                  (7u)

# define VCANCCGBT_CEM_LENGTH                                     (4u)
# define VCANCCGBT_CEM_BYTE1_POS                                  (0u)
# define VCANCCGBT_CEM_BYTE2_POS                                  (1u)
# define VCANCCGBT_CEM_BYTE3_POS                                  (2u)
# define VCANCCGBT_CEM_BYTE4_POS                                  (3u)
# define VCANCCGBT_CEM_BRM_TIMEOUT                                (0x01u)
# define VCANCCGBT_CEM_BCP_TIMEOUT                                (0x01u)
# define VCANCCGBT_CEM_BRO_TIMEOUT                                (0x04u)
# define VCANCCGBT_CEM_BCS_TIMEOUT                                (0x01u)
# define VCANCCGBT_CEM_BSM_TIMEOUT                                (0x04u)
# define VCANCCGBT_CEM_BST_TIMEOUT                                (0x10u)
# define VCANCCGBT_CEM_BSD_TIMEOUT                                (0x01u)
# define VCANCCGBT_CEM_BRM_TIMEOUT_MASK                           (0x03u)
# define VCANCCGBT_CEM_BCP_TIMEOUT_MASK                           (0x03u)
# define VCANCCGBT_CEM_BRO_TIMEOUT_MASK                           (0x0Cu)
# define VCANCCGBT_CEM_BCS_TIMEOUT_MASK                           (0x03u)
# define VCANCCGBT_CEM_BSM_TIMEOUT_MASK                           (0x0Cu)
# define VCANCCGBT_CEM_BST_TIMEOUT_MASK                           (0x30u)
# define VCANCCGBT_CEM_BSD_TIMEOUT_MASK                           (0x03u)

/**********************************************************************************************************************
 *  INTERNAL FUNCTION MACROS
 *********************************************************************************************************************/
# define vCanCcGbt_Write_Buffer(destBuffer, value)                (*(destBuffer)) = ((uint8)(value))

/* Total maximum permissible charging voltage */
# define vCanCcGbt_Set_BHM_TMPCV(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BHM_TMPCV_POS])

/* Version */
# define vCanCcGbt_Set_BRM_Version(destData, srcData)             vCanCcGbt_InternalSet3ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BRM_VERSION_POS])

/* Battery type */
# define vCanCcGbt_Set_BRM_BT(destData, srcData)                  ((destData)[VCANCCGBT_BRM_BT_POS] = (srcData))

/* Power storage battery system capacity */
# define vCanCcGbt_Set_BRM_PSBSC(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BRM_PSBSC_POS])

/* Power storage battery system voltage */
# define vCanCcGbt_Set_BRM_PSBSV(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BRM_PSBSV_POS])

/* Battery manufacturere name */
# define vCanCcGbt_Set_BRM_BMN(destData, srcData)                 VStdLib_MemCpy(&(destData)[VCANCCGBT_BRM_BMN_POS], (srcData), VCANCCGBT_BRM_BMN_LENGTH)

/* Battery pack number */
# define vCanCcGbt_Set_BRM_BPN(destData, srcData)                 vCanCcGbt_InternalSet4ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BRM_BPN_POS])

/* Battery set production data year */
# define vCanCcGbt_Set_BRM_BSPDY(destData, srcData)               ((destData)[VCANCCGBT_BRM_BSPDY_POS] = (srcData))

/* Battery set production data month */
# define vCanCcGbt_Set_BRM_BSPDM(destData, srcData)               ((destData)[VCANCCGBT_BRM_BSPDM_POS] = (srcData))

/* Battery set production data day */
# define vCanCcGbt_Set_BRM_BSPDD(destData, srcData)               ((destData)[VCANCCGBT_BRM_BSPDD_POS] = (srcData))

/* Battery set charging frequency */
# define vCanCcGbt_Set_BRM_BSCF(destData, srcData)                vCanCcGbt_InternalSet3ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BRM_BSCF_POS])

/* Battery set property right mark */
# define vCanCcGbt_Set_BRM_BSPRM(destData, srcData)               ((destData)[VCANCCGBT_BRM_BSPRM_POS] = (srcData))

/* Reserved byte */
# define vCanCcGbt_Set_BRM_Reserved(destData, srcData)            ((destData)[VCANCCGBT_BRM_RESERVED_POS] = (srcData))

/* Vehicle identification number */
# define vCanCcGbt_Set_BRM_VIN(destData, srcData)                 VStdLib_MemCpy(&(destData)[VCANCCGBT_BRM_VIN_POS], (srcData), VCANCCGBT_BRM_VIN_LENGTH)

/* BMS software version number */
# define vCanCcGbt_Set_BRM_BSVN(destData, srcData)                VStdLib_MemCpy(&(destData)[VCANCCGBT_BRM_BSVN_POS], (srcData), VCANCCGBT_BRM_BSVN_LENGTH)

/* Maximum permissible charging voltage singe battery */
# define vCanCcGbt_Set_BCP_MPCVSB(destData, srcData)              vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_MPCVSB_POS])

/* Maximum permissible charging current */
# define vCanCcGbt_Set_BCP_MPCC(destData, srcData)                vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_MPCC_POS])

/* Total nominal energy of power storage battery */
# define vCanccGbt_Set_BCP_TNEPSB(destData, srcData)              vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_TNEPSB_POS])

/* Total maximum permissible charing voltage */
# define vCanCcGbt_Set_BCP_TMPCV(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_TMPCV_POS])

/* Maximum permissible temperature */
# define vCanCcGbt_Set_BCP_MPT(destData, srcData)                 ((destData)[VCANCCGBT_BCP_MPT_POS] = (uint8)(srcData))

/* Charger status power storage battery vehicle */
# define vCanCcGbt_Set_BCP_CSPSBV(destData, srcData)              vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_CSPSBV_POS])

/* Current battery voltage power storage battery vehicle */
# define vCanCcGbt_Set_BCP_CBVPSBV(destData, srcData)             vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCP_CBVPSBV_POS])

/* Is BMS ready for charging */
# define vCanCcGbt_Set_BRO_IBRFC(destData, srcData)               ((destData)[VCANCCGBT_BRO_IBRFC_POS] = (srcData))

/* Voltage demand */
# define vCanCcGbt_Set_BCL_VD(destData, srcData)                  vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCL_VD_POS])

/* Current demand */
# define vCanCcGbt_Set_BCL_CD(destData, srcData)                  vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCL_CD_POS])

/* Charging mode */
# define vCanCcGbt_Set_BCL_CM(destData, srcData)                  ((destData)[VCANCCGBT_BCL_CM_POS] = (srcData))

/* Measured charging voltage */
# define vCanCcGbt_Set_BCS_MCV(destData, srcData)                 vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCS_MCV_POS])

/* Measured charging current */
# define vCanCcGbt_Set_BCS_MCC(destData, srcData)                 vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCS_MCC_POS])

/* Highest voltage of battery */
# define vCanCcGbt_Set_BCS_HVB(destData, srcData)                 vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCS_HVB_POS])

/* Current charge state */
# define vCanCcGbt_Set_BCS_CCS(destData, srcData)                 ((destData)[VCANCCGBT_BCS_CCS_POS] = (srcData))

/* Estimated remaning time */
# define vCanCcGbt_Set_BCS_ERT(destData, srcData)                 vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BCS_ERT_POS])

/* Serial number highest voltage battery */
# define vCanCcGbt_Set_BSM_SNHVB(destData, srcData)               ((destData)[VCANCCGBT_BSM_SNHVB_POS] = (srcData))

/* Highest temperature battery */
# define vCanCcGbt_Set_BSM_HTB(destData, srcData)                 ((destData)[VCANCCGBT_BSM_HTB_POS] = (srcData))

/* Serial number highest temperature detection point */
# define vCanCcGbt_Set_BSM_SNHTDP(destData, srcData)              ((destData)[VCANCCGBT_BSM_SNHTDP_POS] = (srcData))

/* Lowest temperature battery */
# define vCanCcGbt_Set_BSM_LTB(destData, srcData)                 ((destData)[VCANCCGBT_BSM_LTB_POS] = (srcData))

/* Serial number lowest temperature detection point */
# define vCanCcGbt_Set_BSM_SNLTDP(destData, srcData)              ((destData)[VCANCCGBT_BSM_SNLTDP_POS] = (srcData))

/* BSM byte 6 */
# define vCanCcGbt_Set_BSM_Byte6(destData, srcData1, srcData2, srcData3, srcData4) ((destData)[VCANCCGBT_BSM_BYTE6_POS] = (((srcData1) & 0x03u) | (((srcData2) & 0x03u) << 2) | (((srcData3) & 0x03u) << 4) | (((srcData4) & 0x03u) << 6)))

/* BSM byte 7 */
# define vCanCcGbt_Set_BSM_Byte7(destData, srcData1, srcData2, srcData3) ((destData)[VCANCCGBT_BSM_BYTE7_POS] = (((srcData1) & 0x03) | (((srcData2) & 0x03) << 2) | (((srcData3) & 0x03) << 4)))

/* Single battery voltage */
# define vCanCcGbt_Set_BMV_SBV(destData, srcData, pos)            vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[pos])

/* Single battery temperature */
# define vCanCcGbt_Set_BMT_SBT(destData, pos, srcData)            ((destData)[(pos)] = (srcData))

/* Battery reserved */
# define vCanCcGbt_Set_BSP_BR(destData, pos, srcData)             ((destData)[(pos)] = (srcData))

/* Casue for BSM suspending charging */
# define vCanCcGbt_Set_BST_CBSC(destData, srcData)                ((destData)[VCANCCGBT_BST_CBSC_POS] = (srcData))

/* Fault casue for BSM suspending charging */
# define vCanCcGbt_Set_BST_FCBSC(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BST_FCBSC_POS])

/* Error cause for BSM suspending charging */
# define vCanCcGbt_Set_BST_ECBSC(destData, srcData)               ((destData)[VCANCCGBT_BST_ECBSC_POS] = (srcData))

/* Suspending state of charge */
# define vCanCcGbt_Set_BSD_SSC(destData, srcData)                 ((destData)[VCANCCGBT_BSD_SSC_POS] = (srcData))

/* Minimum voltage of single battery */
# define vCanCcGbt_Set_BSD_MIVSB(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BSD_MIVSB_POS])

/* Maximum voltage of single battery */
# define vCanCcGbt_Set_BSD_MAVSB(destData, srcData)               vCanCcGbt_InternalSet2ByteDataLittleEndian((srcData), &(destData)[VCANCCGBT_BSD_MAVSB_POS])

/* Minimum temperature */
# define vCanCcGbt_Set_BSD_MIT(destData, srcData)                 ((destData)[VCANCCGBT_BSD_MIT_POS] = (srcData))

/* Maximum temperature */
# define vCanCcGbt_Set_BSD_MAT(destData, srcData)                 ((destData)[VCANCCGBT_BSD_MAT_POS] = (srcData))

/* CHM timeout */
# define vCanCcGbt_Set_BEM_CRM00Timeout(data)                     ((data)[VCANCCGBT_BEM_BYTE1_POS] = VCANCCGBT_BEM_CHM_TIMEOUT_MASK)

/* CRM timeout */
# define vCanCcGbt_Set_BEM_CRMAATimeout(data)                     ((data)[VCANCCGBT_BEM_BYTE1_POS] = VCANCCGBT_BEM_CRM_TIMEOUT_MASK)

/* CTS timeout */
# define vCanCcGbt_Set_BEM_CTSTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE2_POS] = VCANCCGBT_BEM_CTS_TIMEOUT_MASK)

/* CML timeout */
# define vCanCcGbt_Set_BEM_CMLTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE2_POS] = VCANCCGBT_BEM_CML_TIMEOUT_MASK)

/* CRO timeout */
# define vCanCcGbt_Set_BEM_CROTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE2_POS] = VCANCCGBT_BEM_CRO_TIMEOUT_MASK)

/* CCS timeout */
# define vCanCcGbt_Set_BEM_CCSTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE3_POS] = VCANCCGBT_BEM_CCS_TIMEOUT_MASK)

/* CST timeout */
# define vCanCcGbt_Set_BEM_CSTTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE3_POS] = VCANCCGBT_BEM_CST_TIMEOUT_MASK)

/* CSD timeout */
# define vCanCcGbt_Set_BEM_CSDTimeout(data)                       ((data)[VCANCCGBT_BEM_BYTE4_POS] = VCANCCGBT_BEM_CSD_TIMEOUT_MASK)

/* Is BMS ready for charging */
# define vCanCcGbt_Get_BRO_IBMSRFC(data)                          ((boolean)((data)[VCANCCGBT_BRO_IBRFC_POS]))

/* Charger revision */
# define vCanCcGbt_Get_CHM_CR(data)                               ((data)[VCANCCGBT_CHM_BYTE1_POS])

/* Charger version */
# define vCanCcGbt_Get_CHM_CV(data)                               (((data)[VCANCCGBT_CHM_BYTE2_POS]) | ((uint16)(data)[VCANCCGBT_CHM_BYTE3_POS] << 8))

/* Recognition result */
# define vCanCcGbt_Get_CRM_RR(data)                               ((data)[VCANCCGBT_CRM_RR_POS])

/* Charger Number */
# define vCanCcGbt_Get_CRM_CN(data)                               (((data)[VCANCCGBT_CRM_CN_LO_POS]) | \
                                                                   ((uint32)(data)[VCANCCGBT_CRM_CN_MI_LO_POS] << 8) | \
                                                                   ((uint32)(data)[VCANCCGBT_CRM_CN_MI_HI_POS] << 16) | \
                                                                   ((uint32)(data)[VCANCCGBT_CRM_CN_HI_POS] << 24))

/* Charger station location code */
# define vCanCcGbt_Get_CRM_CSL(dataDest, dataSrc)                 VStdLib_MemCpy((dataDest), (&(dataSrc)[VCANCCGBT_CRM_CSLC_POS]), VCANCCGBT_CRM_CSLC_LENGTH)

/* Charger time - second */
# define vCanCcGbt_Get_CTS_Second(data)                           ((data)[VCANCCGBT_CTS_BYTE1_POS])

/* Charger time - minute */
# define vCanCcGbt_Get_CTS_Minute(data)                           ((data)[VCANCCGBT_CTS_BYTE2_POS])

/* Charger time - hour */
# define vCanCcGbt_Get_CTS_Hour(data)                             ((data)[VCANCCGBT_CTS_BYTE3_POS])

/* Charger time - day */
# define vCanCcGbt_Get_CTS_Day(data)                              ((data)[VCANCCGBT_CTS_BYTE4_POS])

/* Charger time - month */
# define vCanCcGbt_Get_CTS_Month(data)                            ((data)[VCANCCGBT_CTS_BYTE5_POS])

/* Charger time - year */
# define vCanCcGbt_Get_CTS_Year(data)                             (((data)[VCANCCGBT_CTS_BYTE6_POS]) | (((uint16)(data)[VCANCCGBT_CTS_BYTE7_POS] << 8)))

/* Maximum output voltage */
# define vCanCcGbt_Get_CML_MAOV(data)                             (((data)[VCANCCGBT_CML_BYTE1_POS]) | ((uint16)(data)[VCANCCGBT_CML_BYTE2_POS] << 8))

/* Minimum output voltage */
# define vCanCcGbt_Get_CML_MIOV(data)                             (((data)[VCANCCGBT_CML_BYTE3_POS]) | ((uint16)(data)[VCANCCGBT_CML_BYTE4_POS] << 8))

/* Maximum output current */
# define vCanCcGbt_Get_CML_MAOC(data)                             (((data)[VCANCCGBT_CML_BYTE5_POS]) | ((uint16)(data)[VCANCCGBT_CML_BYTE6_POS] << 8))

/* Minimum output current */
# define vCanCcGbt_Get_CML_MIOC(data)                             (((data)[VCANCCGBT_CML_BYTE7_POS]) | ((uint16)(data)[VCANCCGBT_CML_BYTE8_POS] << 8))

/* Charger charging state */
# define vCanCcGbt_Get_CRO_CCS(data)                              ((data)[VCANCCGBT_CRO_BYTE1_POS])

/* Voltage output */
# define vCanCcGbt_Get_CCS_VO(data)                               (((data)[VCANCCGBT_CCS_BYTE1_POS]) | ((uint16)(data)[VCANCCGBT_CCS_BYTE2_POS] << 8))

/* Current output */
# define vCanCcGbt_Get_CCS_CO(data)                               (((data)[VCANCCGBT_CCS_BYTE3_POS]) | ((uint16)(data)[VCANCCGBT_CCS_BYTE4_POS] << 8))

/* Charging time */
# define vCanCcGbt_Get_CCS_CT(data)                               (((data)[VCANCCGBT_CCS_BYTE5_POS]) | ((uint16)(data)[VCANCCGBT_CCS_BYTE6_POS] << 8))

/* Charging permissible */
# define vCanCcGbt_Get_CCS_CP(data)                               ((data)[VCANCCGBT_CCS_BYTE7_POS])

/* Cause charger susoending charging */
# define vCanCcGbt_Get_CST_CCSC(data)                             ((uint8)(data)[VCANCCGBT_CST_BYTE1_POS])

/* Fault cause charger suspending charging */
# define vCanCcGbt_Get_CST_FCCSC(data)                            ((((data)[VCANCCGBT_CST_BYTE2_POS]) | ((uint16)(data)[VCANCCGBT_CST_BYTE3_POS] << 8)))

/* Error caus charger susoending charging */
# define vCanCcGbt_Get_CST_ECCSC(data)                            ((uint8)(data)[VCANCCGBT_CST_BYTE4_POS])

/* Is BMS suspend reason */
# define vCanCcGbt_Is_CST_BMSSR(data)                             ((boolean)((data) & 0xC0) == 0x40)

/* Is Charger suspend reason */
# define vCanCcGbt_Is_CST_CSR(data)                               ((boolean)(((((data) & 0x03) == 0x01)) || ((((data) & 0x0C) == 0x04)) || ((((data) & 0x60) == 0x10))))

/* Charging time */
# define vCanCcGbt_Get_CSD_CT(data)                               (((data)[VCANCCGBT_CSD_BYTE1_POS] | ((uint16)(data)[VCANCCGBT_CSD_BYTE2_POS] << 8)))

/* Output energy */
# define vCanCcGbt_Get_CSD_OE(data)                               (((data)[VCANCCGBT_CSD_BYTE3_POS] | ((uint16)(data)[VCANCCGBT_CSD_BYTE4_POS] << 8)))

/* Charger number */
# define vCanCcGbt_Get_CSD_CN(data)                               ((((data)[VCANCCGBT_CSD_BYTE5_POS]) | ((uint32)(data)[VCANCCGBT_CSD_BYTE6_POS] << 8) | \
                                                                                 ((uint32)(data)[VCANCCGBT_CSD_BYTE7_POS] << 16) | ((uint32)(data)[VCANCCGBT_CSD_BYTE8_POS] << 24)))

/* BRM timeout */
# define vCanCcGbt_Get_CEM_BRMTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE1_POS] & VCANCCGBT_CEM_BRM_TIMEOUT_MASK)

/* BCP timeout */
# define vCanCcGbt_Get_CEM_BCPTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE2_POS] & VCANCCGBT_CEM_BCP_TIMEOUT_MASK)

/* BRO timeout */
# define vCanCcGbt_Get_CEM_BROTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE2_POS] & VCANCCGBT_CEM_BRO_TIMEOUT_MASK)

/* BCS timeout */
# define vCanCcGbt_Get_CEM_BCSTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE3_POS] & VCANCCGBT_CEM_BCS_TIMEOUT_MASK)

/* BSM timeout */
# define vCanCcGbt_Get_CEM_BSMTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE3_POS] & VCANCCGBT_CEM_BSM_TIMEOUT_MASK)

/* BST timeout */
# define vCanCcGbt_Get_CEM_BSTTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE3_POS] & VCANCCGBT_CEM_BST_TIMEOUT_MASK)

/* BSD timeout */
# define vCanCcGbt_Get_CEM_BSDTimeout(data)                       ((data)[VCANCCGBT_CEM_BYTE4_POS] & VCANCCGBT_CEM_BSD_TIMEOUT_MASK)

/* Development Error Detection */
# if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
#  define vCanCcGbt_ReportDetError(SID, ERROR)                    ((void) Det_ReportError(VCANCCGBT_MODULE_ID, VCANCCGBT_DET_INSTANCE_ID, (SID), (ERROR)))
# else
#  define vCanCcGbt_ReportDetError(SID, ERROR)
# endif

# if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
#  define vCanCcGbt_ReportRuntimeError(SID, ERROR)                ((void) Det_ReportError(VCANCCGBT_MODULE_ID, VCANCCGBT_DET_INSTANCE_ID, (SID), (ERROR)))
# else
#  define vCanCcGbt_ReportRuntimeError(SID, ERROR)
# endif

/**********************************************************************************************************************
 *  INTERNAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
  E_VCANCCGBT_STATE_INIT = 0,
  E_VCANCCGBT_STATE_HANDSHAKE = 1,
  E_VCANCCGBT_STATE_PARAM_EXCHANGE = 2,
  E_VCANCCGBT_STATE_CHARGE = 3,
  E_VCANCCGBT_STATE_CHARGE_STATISTIC = 4,
  E_VCANCCGBT_STATE_ERROR = 5
} vCanCcGbt_StateType;

typedef enum
{
  E_VCANCCGBT_MSG_ACTION_CHM_RECEIVED = 1,
  E_VCANCCGBT_MSG_ACTION_CRM_RECEIVED = 2,
  E_VCANCCGBT_MSG_ACTION_CTS_RECEIVED = 3,
  E_VCANCCGBT_MSG_ACTION_CML_RECEIVED = 4,
  E_VCANCCGBT_MSG_ACTION_CRO_RECEIVED = 5,
  E_VCANCCGBT_MSG_ACTION_CCS_RECEIVED = 6,
  E_VCANCCGBT_MSG_ACTION_CST_RECEIVED = 7,
  E_VCANCCGBT_MSG_ACTION_CSD_RECEIVED = 8,
  E_VCANCCGBT_MSG_ACTION_CEM_RECEIVED = 9,
  E_VCANCCGBT_MSG_ACTION_CHM_CYCLIC = 10,
  E_VCANCCGBT_MSG_ACTION_CRM_CYCLIC = 11,
  E_VCANCCGBT_MSG_ACTION_CRM_POSITIVE_RECOGNITION_RESULT_CYCLIC = 12,
  E_VCAMCCGNT_MSG_ACTION_CML_CYCLIC = 13,
  E_VCANCCGBT_MSG_ACTION_CRO_CYCLIC = 14,
  E_VCANCCGBT_MSG_ACTION_CCS_CYCLIC = 15,
  E_VCANCCGBT_MSG_ACTION_CST_BMS_CYCLIC = 16,
  E_VCANCCGBT_MSG_ACTION_CST_CHARGER_CYCLIC = 17,
  E_VCANCCGBT_MSG_ACTION_BEM_TRANSMIT = 18,
  E_VCANCCGBT_MSG_ACTION_BRO_TRANSMIT_CHARGING_READY = 19,
  E_VCANCCGBT_MSG_ACTION_BST_TRANSMIT = 20,
  E_VCANCCGBT_MSG_ACTION_SUSPENDCHARGING = 21
} vCanCcGbt_MsgActionType;

typedef enum
{
  E_VCANCCGBT_ERROR_CRM_00 = 1,
  E_VCANCCGBT_ERROR_CRM_AA = 2,
  E_VCANCCGBT_ERROR_CTS = 3,
  E_VCANCCGBT_ERROR_CML = 4,
  E_VCANCCGBT_ERROR_CRO = 5,
  E_VCANCCGBT_ERROR_CCS = 6,
  E_VCANCCGBT_ERROR_CST = 7,
  E_VCANCCGBT_ERROR_CSD = 8
} vCanCcGbt_TimeoutSupervisionType;

typedef struct
{
  uint8 Data[VCANCCGBT_CHM_LENGTH];
  boolean IsActive;
  boolean IsChanged;
  uint16 Timeout;
} vCanCcGbt_CHM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BHM_LENGTH];
  boolean IsActive;
} vCanCcGbt_BHM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CRM_LENGTH];
  boolean IsActive;
  boolean IsChanged;
  uint16 Timeout;
} vCanCcGbt_CRM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BRM_LENGTH];
  boolean IsActive;
  uint8 ReadPos;
  uint8 DataLength;
} vCanCcGbt_BRM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BEM_LENGTH];
  boolean IsActive;
} vCanCcGbt_BEM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CEM_LENGTH];
  boolean IsChanged;
  boolean IsActive;
} vCanCcGbt_CEM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CTS_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CTS_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CML_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CML_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CRO_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CRO_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BCP_LENGTH];
  boolean IsActive;
  uint8 ReadPos;
  uint8 DataLength;
} vCanCcGbt_BCP_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BRO_LENGTH];
  boolean IsActive;
} vCanCcGbt_BRO_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BCL_LENGTH];
  boolean IsActive;
} vCanCcGbt_BCL_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BCS_LENGTH];
  boolean IsActive;
  uint8 ReadPos;
  uint8 DataLength;
} vCanCcGbt_BCS_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BSM_LENGTH];
  boolean IsActive;
} vCanCcGbt_BSM_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BMV_LENGTH];
  boolean IsActive;
  uint16 DataLength;
  uint16 ReadPos;
} vCanCcGbt_BMV_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BMT_LENGTH];
  boolean IsActive;
  uint8 ReadPos;
  uint8 DataLength;
} vCanCcGbt_BMT_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BSP_LENGTH];
  boolean IsActive;
  uint8 ReadPos;
  uint8 DataLength;
} vCanCcGbt_BSP_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BST_LENGTH];
  boolean IsActive;
} vCanCcGbt_BST_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CCS_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CCS_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CST_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CST_MsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_BSD_LENGTH];
  boolean IsActive;
} vCanCcGbt_BSDMsgType;

typedef struct
{
  uint8 Data[VCANCCGBT_CSD_LENGTH];
  boolean IsChanged;
  boolean IsActive;
  uint16 Timeout;
} vCanCcGbt_CSDMsgType;

typedef struct
{
  boolean IsChargerPlugged;
} vCanCcGbt_InitPhaseType;

typedef struct
{
  vCanCcGbt_CHM_MsgType ChmMsg;
  vCanCcGbt_BHM_MsgType BhmMsg;
  vCanCcGbt_CRM_MsgType CrmMsg;
  vCanCcGbt_BRM_MsgType BrmMsg;
} vCanCcGbt_HandshakePhaseType;

typedef struct
{
  vCanCcGbt_CTS_MsgType CtsMsg;
  vCanCcGbt_CML_MsgType CmlMsg;
  vCanCcGbt_CRO_MsgType CroMsg;
  vCanCcGbt_BCP_MsgType BcpMsg;
  vCanCcGbt_BRO_MsgType BroMsg;
} vCanCcGbt_ParameterExchangePhaseType;

typedef struct
{
  vCanCcGbt_BCL_MsgType BclMsg;
  vCanCcGbt_BCS_MsgType BcsMsg;
  vCanCcGbt_BSM_MsgType BsmMsg;
  vCanCcGbt_BMV_MsgType BmvMsg;
  vCanCcGbt_BMT_MsgType BmtMsg;
  vCanCcGbt_BSP_MsgType BspMsg;
  vCanCcGbt_BST_MsgType BstMsg;
  vCanCcGbt_CCS_MsgType CcsMsg;
  vCanCcGbt_CST_MsgType CstMsg;
  boolean IsChargingSuspended;
  boolean IsChargerSuspendReason;
} vCanCcGbt_ChargePhaseType;

typedef struct
{
  vCanCcGbt_CSDMsgType CsdMsg;
  vCanCcGbt_BSDMsgType BsdMsg;
} vCanCcGbt_ChargeStatisticPhaseType;

typedef struct
{
  vCanCcGbt_BEM_MsgType BemMsg;
  vCanCcGbt_CEM_MsgType CemMsg;
} vCanCcGbt_ErrorPhaseType;

typedef struct
{
  vCanCcGbt_StateType ChargeState;
  vCanCcGbt_InitPhaseType InitPhase;
  vCanCcGbt_HandshakePhaseType HandshakePhase;
  vCanCcGbt_ParameterExchangePhaseType ParameterExchangePhase;
  vCanCcGbt_ChargePhaseType ChargePhase;
  vCanCcGbt_ChargeStatisticPhaseType ChargeStatisticPhase;
  vCanCcGbt_ErrorPhaseType ErrorPhase;
} vCanCcGbt_ChargeStateType;

/**********************************************************************************************************************
 *  INTERNAL DATA PROTOTYPES
 *********************************************************************************************************************/
#endif /* VCANCCGBT_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: vCanCcGbt_Int.h
 *********************************************************************************************************************/
