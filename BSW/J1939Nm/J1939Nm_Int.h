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
 *         File:  J1939Nm_Int.h
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Internal header of the SAE J1939 Network Management module.
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

#if !defined (J1939NM_INT_H)
# define J1939NM_INT_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Nm_Cfg.h"
# include "J1939Nm_Lcfg.h"
# include "J1939Nm_PBcfg.h"

# include "J1939Nm.h"

# include "ComStack_Types.h"

# if (J1939NM_DEV_ERROR_DETECT == STD_ON)
#  include "Det.h"
# endif


/**********************************************************************************************************************
 *  INTERNAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  INTERNAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Development Error Detection */
# define J1939NM_INSTANCE_ID_DET                (0x00u)

/* Global module state */
# define J1939NM_UNINIT                         (0u)
# define J1939NM_INIT                           (1u)

/* Special node */
# define J1939NM_INVALID_NODE                   (65535u)

/* Special addresses */
# define J1939NM_NULL_ADDRESS                   (0xFEu)
# define J1939NM_BROADCAST_ADDRESS              (0xFFu)
# define J1939NM_MIN_DELAY_ADDRESS              (0x80u)
# define J1939NM_MAX_DELAY_ADDRESS              (0xF7u)

/* Known PGNs */
# define J1939NM_PGN_AC                         (0x0EE00u)
# define J1939NM_PGN_NM                         (0x09300u)

/* BusAddressState constants */
# define J1939NM_BUSADDRESS_UNUSED              (0u)
# define J1939NM_BUSADDRESS_ALLOCATED           (0xFFu)

/* Random number calculation */
# define J1939NM_RANDOM_FACTOR                  (5u)
# define J1939NM_RANDOM_OFFSET                  (13u)
# define J1939NM_RANDOM_SCALING                 (2048u)

/* Priority */
# define J1939NM_DEFAULT_PRIORITY               (6u)
# define J1939NM_MAX_PRIORITY                   (7u)
# define J1939NM_PRIORITY_MASK                  (0x1Cu)
# define J1939NM_PRIORITY_BITPOS                (2u)

/* MetaData sizes and positions */
# define J1939NM_MDL_SIZE_NM                    (2u)
# define J1939NM_MDL_SIZE_AC                    (1u)
# define J1939NM_MDL_SA_POS                     (0u)
# define J1939NM_MDL_DA_POS                     (1u)

/* PDU size and MDL positions */
/* AC */
# define J1939NM_AC_PDU_LENGTH                  (J1939NM_AC_FRAME_LENGTH + J1939NM_MDL_SIZE_AC)
# define J1939NM_AC_MDL_POS                     J1939NM_AC_FRAME_LENGTH
# define J1939NM_AC_SA_POS                      (J1939NM_AC_FRAME_LENGTH + J1939NM_MDL_SA_POS)
# define J1939NM_AC_DA_POS                      (J1939NM_AC_FRAME_LENGTH + J1939NM_MDL_DA_POS)
/* CA */
# define J1939NM_CA_PDU_LENGTH                  (J1939NM_CA_FRAME_LENGTH)
# define J1939NM_CA_MDL_POS                     (0u)
# define J1939NM_CA_SA_POS                      J1939NM_MDL_SA_POS
# define J1939NM_CA_DA_POS                      J1939NM_MDL_DA_POS
/* NM */
# define J1939NM_NM_PDU_LENGTH                  (J1939NM_NM_FRAME_LENGTH + J1939NM_MDL_SIZE_NM)
# define J1939NM_NM_MDL_POS                     J1939NM_NM_FRAME_LENGTH
# define J1939NM_NM_SA_POS                      (J1939NM_NM_FRAME_LENGTH + J1939NM_MDL_SA_POS)
# define J1939NM_NM_DA_POS                      (J1939NM_NM_FRAME_LENGTH + J1939NM_MDL_DA_POS)

/* Standardized sizes and positions */
/* AC */
# define J1939NM_AC_FRAME_LENGTH                (8u)
# define J1939NM_AC_NAME_BYTE1                  (0u)
# define J1939NM_AC_NAME_BYTE2                  (1u)
# define J1939NM_AC_NAME_BYTE3                  (2u)
# define J1939NM_AC_NAME_BYTE4                  (3u)
# define J1939NM_AC_NAME_BYTE5                  (4u)
# define J1939NM_AC_NAME_BYTE6                  (5u)
# define J1939NM_AC_NAME_BYTE7                  (6u)
# define J1939NM_AC_NAME_BYTE8                  (7u)
/* CA */
# define J1939NM_CA_FRAME_LENGTH                (9u)
# define J1939NM_CA_NAME_BYTE1                  (0u)
# define J1939NM_CA_NAME_BYTE2                  (1u)
# define J1939NM_CA_NAME_BYTE3                  (2u)
# define J1939NM_CA_NAME_BYTE4                  (3u)
# define J1939NM_CA_NAME_BYTE5                  (4u)
# define J1939NM_CA_NAME_BYTE6                  (5u)
# define J1939NM_CA_NAME_BYTE7                  (6u)
# define J1939NM_CA_NAME_BYTE8                  (7u)
# define J1939NM_CA_ADDRESS                     (8u)
/* NM */
# define J1939NM_NM_FRAME_LENGTH                (8u)
# define J1939NM_NM_BYTE1                       (0u)
# define J1939NM_NM_BYTE2                       (1u)
# define J1939NM_NM_BYTE3                       (2u)
# define J1939NM_NM_BYTE4                       (3u)
# define J1939NM_NM_BYTE5                       (4u)
# define J1939NM_NM_BYTE6                       (5u)
# define J1939NM_NM_BYTE7                       (6u)
# define J1939NM_NM_BYTE8                       (7u)

/* NAME */
# define J1939NM_NAME_LENGTH                    (8u)
# define J1939NM_NAME_BYTE1                     (0u)
# define J1939NM_NAME_BYTE2                     (1u)
# define J1939NM_NAME_BYTE3                     (2u)
# define J1939NM_NAME_BYTE4                     (3u)
# define J1939NM_NAME_BYTE5                     (4u)
# define J1939NM_NAME_BYTE6                     (5u)
# define J1939NM_NAME_BYTE7                     (6u)
# define J1939NM_NAME_BYTE8                     (7u)

# define J1939NM_NAME_BYTE8_MASK_AAC         (0x80u)
# define J1939NM_NAME_BYTE8_SHIFT_AAC        (7u)
# define J1939NM_NAME_BYTE8_MASK_IG          (0x70u)
# define J1939NM_NAME_BYTE8_SHIFT_IG         (4u)
# define J1939NM_NAME_BYTE8_MASK_VSI         (0x0Fu)
# define J1939NM_NAME_BYTE8_SHIFT_VSI        (0u)
# define J1939NM_NAME_BYTE7_MASK_VS          (0xFEu)
# define J1939NM_NAME_BYTE7_SHIFT_VS         (1u)
# define J1939NM_NAME_BYTE7_MASK_RES         (0x01u)
# define J1939NM_NAME_BYTE7_SHIFT_RES        (0u)
# define J1939NM_NAME_BYTE6_MASK_F           (0xFFu)
# define J1939NM_NAME_BYTE6_SHIFT_F          (0u)
# define J1939NM_NAME_BYTE5_MASK_FI          (0xF8u)
# define J1939NM_NAME_BYTE5_SHIFT_FI         (3u)
# define J1939NM_NAME_BYTE5_MASK_EI          (0x07u)
# define J1939NM_NAME_BYTE5_SHIFT_EI         (0u)
# define J1939NM_NAME_BYTE4_MASK_MC          (0xFFu)
# define J1939NM_NAME_BYTE4_SHIFT_MC         (0u)
# define J1939NM_NAME_BYTE3_MASK_MC          (0xE0u)
# define J1939NM_NAME_BYTE3_SHIFT_MC         (5u)
# define J1939NM_NAME_BYTE3_MASK_IN          (0x1Fu)
# define J1939NM_NAME_BYTE3_SHIFT_IN         (0u)
# define J1939NM_NAME_BYTE2_MASK_IN          (0xFFu)
# define J1939NM_NAME_BYTE2_SHIFT_IN         (0u)
# define J1939NM_NAME_BYTE1_MASK_IN          (0xFFu)
# define J1939NM_NAME_BYTE1_SHIFT_IN         (0u)

# define J1939NM_NM_CHECKSUM_MOD                (256u)
# define J1939NM_NM_NAME_LENGTH                 (8u)
# define J1939NM_NM_MODE_POS                    (2u)
# define J1939NM_NM_MODE_MASK                   (0x0Fu)
# define J1939NM_NM_CHECKSUM_POS                (0)
# define J1939NM_NM_SNA                         (0xFFu)
# define J1939NM_NM_RESERVED_BYTE3              (0x10u)
# define J1939NM_NM_RESERVED_BYTE7              (0x01u)
# define J1939NM_NM_NODE_ANY                    (0x00u)

# define J1939NM_NM_QF_POS                      (1u)
# define J1939NM_NM_QF_MC_MASK                  (0x01u)
# define J1939NM_NM_QF_EI_MASK                  (0x02u)
# define J1939NM_NM_QF_FI_MASK                  (0x04u)
# define J1939NM_NM_QF_F_MASK                   (0x08u)
# define J1939NM_NM_QF_VS_MASK                  (0x10u)
# define J1939NM_NM_QF_VSI_MASK                 (0x20u)
# define J1939NM_NM_QF_IG_MASK                  (0x40u)
# define J1939NM_NM_QF_AAC_MASK                 (0x80u)
# define J1939NM_NM_CMC_POS_HI                  (2u)
# define J1939NM_NM_CMC_POS_LO                  (3u)
# define J1939NM_NM_CMC_MASK_HI                 (0xE0u)
# define J1939NM_NM_CMC_MASK_LO                 (0xFFu)
# define J1939NM_NM_CEI_POS                     (4u)
# define J1939NM_NM_CEI_MASK                    (0x07u)
# define J1939NM_NM_CFI_POS                     (4u)
# define J1939NM_NM_CFI_MASK                    (0xF8u)
# define J1939NM_NM_CF_POS                      (5u)
# define J1939NM_NM_CF_MASK                     (0xFFu)
# define J1939NM_NM_CVS_POS                     (6u)
# define J1939NM_NM_CVS_MASK                    (0xFEu)
# define J1939NM_NM_CVSI_POS                    (7u)
# define J1939NM_NM_CVSI_MASK                   (0x0Fu)
# define J1939NM_NM_CIG_POS                     (7u)
# define J1939NM_NM_CIG_MASK                    (0x70u)
# define J1939NM_NM_CAAC_POS                    (7u)
# define J1939NM_NM_CAAC_MASK                   (0x80u)

# define J1939NM_NM_QUEUE_DATA_POS1              (0u)
# define J1939NM_NM_QUEUE_DATA_POS2              (1u)
# define J1939NM_NM_QUEUE_DATA_POS3              (2u)
# define J1939NM_NM_QUEUE_DATA_POS4              (3u)
# define J1939NM_NM_QUEUE_DATA_POS5              (4u)
# define J1939NM_NM_QUEUE_DATA_POS6              (5u)
# define J1939NM_NM_QUEUE_DATA_POS7              (6u)
# define J1939NM_NM_QUEUE_DATA_POS8              (7u)
# define J1939NM_NM_QUEUE_DATA_POS9              (8u)
# define J1939NM_NM_QUEUE_DATA_POS10             (9u)

/**********************************************************************************************************************
 *  INTERNAL FUNCTION MACROS
 *********************************************************************************************************************/

/* Generic access macros */
# define J1939Nm_AssembleTwoBytes(lo, hi)       (((uint16)(hi) << 8) | (lo))
# define J1939Nm_AssembleThreeBytes(lo, md, hi) (((uint32)(hi) << 16) | ((uint16)(md) << 8) | (lo))
# define J1939Nm_ExtractByteOne(val)            ((uint8)((val) & 0xFFu))
# define J1939Nm_ExtractByteTwo(val)            ((uint8)(((uint16_least)(val) >> 8) & 0xFFu))
# define J1939Nm_ExtractByteThree(val)          ((uint8)(((uint32)(val) >> 16) & 0xFFu))

/* PGN access and check macros */
# define J1939Nm_PGN_GetDP(pgn)                 (J1939Nm_ExtractByteThree(pgn) & 0x03u)
# define J1939Nm_PGN_GetPDUF(pgn)               J1939Nm_ExtractByteTwo(pgn)
# define J1939Nm_PGN_GetPDUS(pgn)               J1939Nm_ExtractByteOne(pgn)
# define J1939Nm_PGN_IsPDU2(pgn)                (J1939Nm_PGN_GetPDUF(pgn) >= 0xF0)
# define J1939Nm_PGN_ValidDP(pgn)               (J1939Nm_PGN_GetDP(pgn) != 0x03u)
# define J1939Nm_PGN_ValidPDUS(pgn)             (J1939Nm_PGN_IsPDU2(pgn) || (J1939Nm_PGN_GetPDUS(pgn) == 0x00u))
# define J1939Nm_PGN_Valid(pgn)                 (J1939Nm_PGN_ValidDP(pgn) && J1939Nm_PGN_ValidPDUS(pgn))

/* PGN conversion macros */
# define J1939Nm_Get_PGN(pgn_lo, pgn_md, pgn_hi) J1939Nm_AssembleThreeBytes(pgn_lo, pgn_md, pgn_hi)
# define J1939Nm_Make_PGN_LO(pgn)               J1939Nm_PGN_GetPDUS(pgn)
# define J1939Nm_Make_PGN_MD(pgn)               J1939Nm_PGN_GetPDUF(pgn)
# define J1939Nm_Make_PGN_HI(pgn)               J1939Nm_PGN_GetDP(pgn)

/* Priority conversion macros */
# define J1939Nm_Get_Priority(priority)         ((uint8)(((priority) & J1939NM_PRIORITY_MASK) >> J1939NM_PRIORITY_BITPOS))
# define J1939Nm_Make_Priority(priority)        ((uint8)((priority) << J1939NM_PRIORITY_BITPOS))

/* Parameter check macros */
# define J1939Nm_Channel_Valid(channel)         (((channel) < J1939Nm_GetSizeOfComMChannel()) && J1939Nm_IsChannelUsedOfComMChannel(channel))
# define J1939Nm_DA_Valid(da)                   ((da) != J1939NM_NULL_ADDRESS)
# define J1939Nm_SA_Valid(sa)                   ((sa) != J1939NM_BROADCAST_ADDRESS)
# define J1939Nm_SA_ValidNonNull(sa)            (((sa) != J1939NM_NULL_ADDRESS) && ((sa) != J1939NM_BROADCAST_ADDRESS))
# define J1939Nm_AckCode_Valid(ackCode)         ((ackCode) <= J1939NM_ACK_CANNOT_RESPOND)
# define J1939Nm_Priority_Valid(priority)       ((priority) <= J1939NM_MAX_PRIORITY)

/* Access macros for BusAddressState */
# define J1939Nm_SetBusAddressAllocated(channelState, busAddress) ((channelState)->BusAddressState[(busAddress) >> 3] |= (1u << ((busAddress) & 7)))
# define J1939Nm_ClearBusAddressAllocated(channelState, busAddress) ((channelState)->BusAddressState[(busAddress) >> 3] &= ~(1u << ((busAddress) & 7)))
# define J1939Nm_IsBusAddressAllocated(channelState, busAddress) (((channelState)->BusAddressState[(busAddress) >> 3] & (1u << ((busAddress) & 7))) != 0)

/* Access macros for Name Management */
# define J1939Nm_NM_GetQf(data)                  (uint8)((data)[J1939NM_NM_QF_POS])
# define J1939Nm_NM_IsQfMCEnabled(data)          (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_MC_MASK) == FALSE)
# define J1939Nm_NM_IsQfEIEnabled(data)          (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_EI_MASK) == FALSE)
# define J1939Nm_NM_IsQfFIEnabled(data)          (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_FI_MASK) == FALSE)
# define J1939Nm_NM_IsQfFEnabled(data)           (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_F_MASK) == FALSE)
# define J1939Nm_NM_IsQfVSEnabled(data)          (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_VS_MASK) == FALSE)
# define J1939Nm_NM_IsQfVSIEnabled(data)         (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_VSI_MASK) == FALSE)
# define J1939Nm_NM_IsQfIGEnabled(data)          (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_IG_MASK) == FALSE)
# define J1939Nm_NM_IsQfAACEnabled(data)         (((data)[J1939NM_NM_QF_POS] & J1939NM_NM_QF_AAC_MASK) == FALSE)

# define J1939Nm_NM_GetChecksum(data)           (uint8)((data)[J1939NM_NM_CHECKSUM_POS])
# define J1939Nm_NM_GetMode(data)               (uint8)((data)[J1939NM_NM_MODE_POS] & J1939NM_NM_MODE_MASK)
# define J1939Nm_NM_GetCMC_HI(data)             (uint8)((data)[J1939NM_NM_CMC_POS_HI] & J1939NM_NM_CMC_MASK_HI)
# define J1939Nm_NM_GetCMC_LO(data)             (uint8)((data)[J1939NM_NM_CMC_POS_LO] & J1939NM_NM_CMC_MASK_LO)
# define J1939Nm_NM_GetCEI(data)                (uint8)((data)[J1939NM_NM_CEI_POS] & J1939NM_NM_CEI_MASK)
# define J1939Nm_NM_GetCFI(data)                (uint8)((data)[J1939NM_NM_CFI_POS] & J1939NM_NM_CFI_MASK)
# define J1939Nm_NM_GetCF(data)                 (uint8)((data)[J1939NM_NM_CF_POS] & J1939NM_NM_CF_MASK)
# define J1939Nm_NM_GetCVS(data)                (uint8)((data)[J1939NM_NM_CVS_POS] & J1939NM_NM_CVS_MASK)
# define J1939Nm_NM_GetCVSI(data)               (uint8)((data)[J1939NM_NM_CVSI_POS] & J1939NM_NM_CVSI_MASK)
# define J1939Nm_NM_GetCIG(data)                (uint8)((data)[J1939NM_NM_CIG_POS] & J1939NM_NM_CIG_MASK)
# define J1939Nm_NM_GetCAAC(data)               (uint8)((data)[J1939NM_NM_CAAC_POS] & J1939NM_NM_CAAC_MASK)

# define J1939Nm_NM_SetMC_LO(pendingName, data)  (pendingName)[J1939NM_NAME_BYTE4] = J1939Nm_NM_GetCMC_LO((data))
# define J1939Nm_NM_SetMC_HI(pendingName, data)  (pendingName)[J1939NM_NAME_BYTE3] = (J1939Nm_NM_GetCMC_HI((data)) | ((pendingName)[J1939NM_NAME_BYTE3] & J1939NM_NAME_BYTE3_MASK_IN))
# define J1939Nm_NM_SetEI(pendingName, data)     (pendingName)[J1939NM_NAME_BYTE5] = (J1939Nm_NM_GetCEI((data)) | ((pendingName)[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_FI))
# define J1939Nm_NM_SetFI(pendingName, data)     (pendingName)[J1939NM_NAME_BYTE5] = (J1939Nm_NM_GetCFI(data) | ((pendingName)[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_EI))
# define J1939Nm_NM_SetF(pendingName, data)      (pendingName)[J1939NM_NAME_BYTE6] = J1939Nm_NM_GetCF((data))
# define J1939Nm_NM_SetVS(pendingName, data)     (pendingName)[J1939NM_NAME_BYTE7] = J1939Nm_NM_GetCVS((data))
# define J1939Nm_NM_SetVSI(pendingName, data)    (pendingName)[J1939NM_NAME_BYTE8] = (J1939Nm_NM_GetCVSI((data)) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_AAC) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_IG))
# define J1939Nm_NM_SetIG(pendingName, data)     (pendingName)[J1939NM_NAME_BYTE8] = (J1939Nm_NM_GetCIG(data) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_AAC) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_VSI))
# define J1939Nm_NM_SetAAC(pendingName, data)    (pendingName)[J1939NM_NAME_BYTE8] = (J1939Nm_NM_GetCAAC((data)) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_IG) | ((pendingName)[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_VSI))

/* Development Error Detection */
# if (J1939NM_DEV_ERROR_DETECT == STD_ON)
#  define J1939Nm_ReportDetError(SID, ERROR) ((void)Det_ReportError(J1939NM_MODULE_ID, J1939NM_INSTANCE_ID_DET, (SID), (ERROR)))

#  define J1939Nm_Assert(CONDITION, SID, ERROR) \
  { \
    if (!(CONDITION)) \
    { \
      J1939Nm_ReportDetError((SID), (ERROR)); \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define J1939Nm_AssertRet(CONDITION, SID, ERROR) \
  { \
    if (!(CONDITION)) \
    { \
      J1939Nm_ReportDetError((SID), (ERROR)); \
      return; \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define J1939Nm_AssertRetVal(CONDITION, SID, ERROR) \
  { \
    if (!(CONDITION)) \
    { \
      J1939Nm_ReportDetError((SID), (ERROR)); \
      return E_NOT_OK; \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define J1939Nm_AssertRetBrtVal(CONDITION, SID, ERROR) \
  { \
    if (!(CONDITION)) \
    { \
      J1939Nm_ReportDetError((SID), (ERROR)); \
      return BUFREQ_E_NOT_OK; \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */
# else
#  define J1939Nm_ReportDetError(SID, ERROR)
#  define J1939Nm_Assert(CONDITION, SID, ERROR)
#  define J1939Nm_AssertRet(CONDITION, SID, ERROR)
#  define J1939Nm_AssertRetVal(CONDITION, SID, ERROR)
#  define J1939Nm_AssertRetBrtVal(CONDITION, SID, ERROR)
# endif


/**********************************************************************************************************************
 *  INTERNAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*======================================= J1939NM_START_SEC_VAR_ZERO_INIT_8BIT =======================================*/
# define J1939NM_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the J1399Rm */
extern VAR(uint8, J1939NM_VAR_ZERO_INIT) J1939Nm_ModuleInitialized;

# define J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*======================================== J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT =======================================*/

/*=================================== J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/
# define J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/*! \brief Post-build configuration data of the J1399Nm */
# if(J1939NM_USE_INIT_POINTER == STD_ON)
extern P2CONST(J1939Nm_ConfigType, J1939NM_VAR_ZERO_INIT, J1939NM_PBCFG) J1939Nm_ConfigDataPtr;
# endif

# define J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*=================================== J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED ====================================*/


/**********************************************************************************************************************
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
# define J1939NM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

FUNC(void, J1939NM_CODE) J1939Nm_InternalGetName(J1939Nm_NameType nodeName,
                                                 const J1939Nm_SizeOfNodeNameType nodeNameIdx);
# if (((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON))) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
FUNC(void, J1939NM_CODE) J1939Nm_InternalCopyName(J1939Nm_NameType newNodeName, J1939Nm_ConstNameType oldNodeName);
# endif
# if ((J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
FUNC(sint16_least, J1939NM_CODE) J1939Nm_InternalNameCompare(J1939Nm_ConstNameType ownNodeName,
                                                             J1939Nm_ConstNameType otherNodeName);
# endif

# define J1939NM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/

#endif /* J1939NM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Nm_Int.h
 *********************************************************************************************************************/
