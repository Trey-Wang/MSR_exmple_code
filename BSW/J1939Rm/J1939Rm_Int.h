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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*! \file  J1939Rm_Int.h
 *  \brief SAE J1939 Request Manager (J1939Rm)
 *
 *  \details Internal header of the SAE J1939 Request Manager.
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

#if !defined (J1939RM_INT_H)
# define J1939RM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "J1939Rm_Cfg.h"
# include "J1939Rm_Lcfg.h"
# include "J1939Rm_PBcfg.h"

# include "J1939Rm.h"

# include "ComStack_Types.h"

# if ((J1939RM_DEV_ERROR_REPORT == STD_ON) || (J1939RM_RUNTIME_ERROR_REPORT == STD_ON))
#  include "Det.h"
# endif

/**********************************************************************************************************************
 *  INTERNAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Development Error Detection */
# define J1939RM_INSTANCE_ID_DET                (0u)

/* Special node channel state id if J1939RM_USERSENDREQUESTOFUSER == STD_OFF */
# define J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX (0xFFFF)

/* Special addresses */
# define J1939RM_NULL_ADDRESS                   (0xFEu)
# define J1939RM_BROADCAST_ADDRESS              (0xFFu)

/* Known PGNs */
# define J1939RM_PGN_AC                         (0x0EE00u)

/* Priority */
# define J1939RM_DEFAULT_PRIORITY               (6u)
# define J1939RM_MAX_PRIORITY                   (7u)
# define J1939RM_PRIORITY_MASK                  (0x1Cu)
# define J1939RM_PRIORITY_BITPOS                (2u)

/* MetaData sizes and positions */
# define J1939RM_MDL_SIZE                       (4u)
# define J1939RM_MDL_SA_POS                     (0u)
# define J1939RM_MDL_DA_POS                     (1u)
# define J1939RM_MDL_PRI_POS                    (3u)

/* Standardized sizes and positions */
/* RQST */
# define J1939RM_RQST_FRAME_LENGTH              (3u)
# define J1939RM_RQST_PGN_LO                    (0u)
# define J1939RM_RQST_PGN_MD                    (1u)
# define J1939RM_RQST_PGN_HI                    (2u)
/* RQST2 */
# define J1939RM_RQST2_FRAME_LENGTH             (8u)
# define J1939RM_RQST2_PGN_LO                   (0u)
# define J1939RM_RQST2_PGN_MD                   (1u)
# define J1939RM_RQST2_PGN_HI                   (2u)
# define J1939RM_RQST2_SI                       (3u)
# define J1939RM_RQST2_EIB1                     (4u)
# define J1939RM_RQST2_EIB2                     (5u)
# define J1939RM_RQST2_EIB3                     (6u)
# define J1939RM_RQST2_RES                      (7u)
/* ACKM */
# define J1939RM_ACKM_FRAME_LENGTH              (8u)
# define J1939RM_ACKM_CB                        (0u)
# define J1939RM_ACKM_GF1                       (1u)
# define J1939RM_ACKM_GF2                       (2u)
# define J1939RM_ACKM_GF3                       (3u)
# define J1939RM_ACKM_ADDR                      (4u)
# define J1939RM_ACKM_PGN_LO                    (5u)
# define J1939RM_ACKM_PGN_MD                    (6u)
# define J1939RM_ACKM_PGN_HI                    (7u)

/* standardized values */
# if (J1939RM_SUPPORT_REQUEST2 == STD_ON)
/* RQST2 */
#  define J1939RM_RQST2_TRANSFER_MASK           (0x03u)
#  define J1939RM_RQST2_TRANSFER_NO             (0x00u)
#  define J1939RM_RQST2_TRANSFER_YES            (0x01u)
#  define J1939RM_RQST2_TRANSFER_UNDEF          (0x02u)
#  define J1939RM_RQST2_TRANSFER_NA             (0x03u)
#  define J1939RM_RQST2_EXTIDTYPE_MASK          (0x1Cu)
#  define J1939RM_RQST2_EXTIDTYPE_NONE          (0x00u)
#  define J1939RM_RQST2_EXTIDTYPE_ONE           (0x04u)
#  define J1939RM_RQST2_EXTIDTYPE_TWO           (0x08u)
#  define J1939RM_RQST2_EXTIDTYPE_THREE         (0x0Cu)
#  define J1939RM_RQST2_EXTIDTYPE_NA            (0x1Cu)
# endif
/* ACKM */
# define J1939RM_ACKM_CODE_EXTID_NONE           (0u)
# define J1939RM_ACKM_CODE_EXTID_ONE            (8u)
# define J1939RM_ACKM_CODE_EXTID_TWO            (9u)
# define J1939RM_ACKM_CODE_EXTID_THREE          (10u)
# define J1939RM_ACKM_CODE_BASE_MASK            (0x0Fu)
# define J1939RM_ACKM_CODE_EXTID_MASK           (0xF0u)
# define J1939RM_ACKM_CODE_EXTID_BITPOS         (4u)
/* Pattern for unused areas */
# define J1939RM_SNA_VALUE                      (0xFFu)

/* PDU size and MDL positions */
/* RQST */
# define J1939RM_RQST_PDU_LENGTH                (J1939RM_RQST_FRAME_LENGTH + J1939RM_MDL_SIZE)
# define J1939RM_RQST_MDL_POS                   J1939RM_RQST_FRAME_LENGTH
# define J1939RM_RQST_SA_POS                    (J1939RM_RQST_FRAME_LENGTH + J1939RM_MDL_SA_POS)
# define J1939RM_RQST_DA_POS                    (J1939RM_RQST_FRAME_LENGTH + J1939RM_MDL_DA_POS)
# define J1939RM_RQST_PRI_POS                   (J1939RM_RQST_FRAME_LENGTH + J1939RM_MDL_PRI_POS)
/* RQST2 */
# define J1939RM_RQST2_PDU_LENGTH               (J1939RM_RQST2_FRAME_LENGTH + J1939RM_MDL_SIZE)
# define J1939RM_RQST2_MDL_POS                  J1939RM_RQST2_FRAME_LENGTH
# define J1939RM_RQST2_SA_POS                   (J1939RM_RQST2_FRAME_LENGTH + J1939RM_MDL_SA_POS)
# define J1939RM_RQST2_DA_POS                   (J1939RM_RQST2_FRAME_LENGTH + J1939RM_MDL_DA_POS)
# define J1939RM_RQST2_PRI_POS                  (J1939RM_RQST2_FRAME_LENGTH + J1939RM_MDL_PRI_POS)
/* ACKM */
# define J1939RM_ACKM_PDU_LENGTH                (J1939RM_ACKM_FRAME_LENGTH + J1939RM_MDL_SIZE)
# define J1939RM_ACKM_MDL_POS                   J1939RM_ACKM_FRAME_LENGTH
# define J1939RM_ACKM_SA_POS                    (J1939RM_ACKM_FRAME_LENGTH + J1939RM_MDL_SA_POS)
# define J1939RM_ACKM_DA_POS                    (J1939RM_ACKM_FRAME_LENGTH + J1939RM_MDL_DA_POS)
# define J1939RM_ACKM_PRI_POS                   (J1939RM_ACKM_FRAME_LENGTH + J1939RM_MDL_PRI_POS)

/**********************************************************************************************************************
 *  INTERNAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Generic access macros */
# define J1939Rm_AssembleTwoBytes(lo, hi)       (((uint16) (hi) << 8) | (lo))
# define J1939Rm_AssembleThreeBytes(lo, md, hi) (((uint32) (hi) << 16) | ((uint16)(md) << 8) | (lo))
# define J1939Rm_ExtractByteOne(val)            ((uint8) ((val) & 0xFFu))
# define J1939Rm_ExtractByteTwo(val)            ((uint8) (((uint16_least)(val) >> 8) & 0xFFu))
# define J1939Rm_ExtractByteThree(val)          ((uint8) (((uint32)(val) >> 16) & 0xFFu))

/* PGN access and check macros */
# define J1939Rm_PGN_GetDP(pgn)                 (J1939Rm_ExtractByteThree(pgn) & 0x03u)
# define J1939Rm_PGN_GetPDUF(pgn)               J1939Rm_ExtractByteTwo(pgn)
# define J1939Rm_PGN_GetPDUS(pgn)               J1939Rm_ExtractByteOne(pgn)
# define J1939Rm_PGN_IsPDU2(pgn)                (J1939Rm_PGN_GetPDUF(pgn) >= 0xF0)
# define J1939Rm_PGN_ValidDP(pgn)               (J1939Rm_PGN_GetDP(pgn) != 0x03u)
# define J1939Rm_PGN_ValidPDUS(pgn)             (J1939Rm_PGN_IsPDU2(pgn) || (J1939Rm_PGN_GetPDUS(pgn) == 0x00u))
# define J1939Rm_PGN_Valid(pgn)                 (J1939Rm_PGN_ValidDP(pgn) && J1939Rm_PGN_ValidPDUS(pgn))

/* PGN conversion macros */
# define J1939Rm_Get_PGN(pgn_lo, pgn_md, pgn_hi) J1939Rm_AssembleThreeBytes(pgn_lo, pgn_md, pgn_hi)
# define J1939Rm_Make_PGN_LO(pgn)               J1939Rm_PGN_GetPDUS(pgn)
# define J1939Rm_Make_PGN_MD(pgn)               J1939Rm_PGN_GetPDUF(pgn)
# define J1939Rm_Make_PGN_HI(pgn)               J1939Rm_PGN_GetDP(pgn)

/* Priority conversion macros */
# define J1939Rm_Get_Priority(priority)         ((uint8) \
                                                 (((priority) & J1939RM_PRIORITY_MASK) >> J1939RM_PRIORITY_BITPOS))
# define J1939Rm_Make_Priority(priority)        ((uint8) ((priority) << J1939RM_PRIORITY_BITPOS))

/* Ack code conversion macros */
# define J1939Rm_Get_BaseAckCode(ackCb)         ((J1939Rm_AckCode) ((ackCb) & J1939RM_ACKM_CODE_BASE_MASK))
# define J1939Rm_Get_ExtIdAckCode(ackCb)        ((uint8) (((ackCb) & J1939RM_ACKM_CODE_EXTID_MASK) \
                                                 >> J1939RM_ACKM_CODE_EXTID_BITPOS))
# define J1939Rm_Make_AckCb(extIdCode, ackCode) ((uint8) (((extIdCode) << J1939RM_ACKM_CODE_EXTID_BITPOS) | (ackCode)))

/* Parameter check macros */
# define J1939Rm_Channel_Valid(channel)         (((channel) < J1939Rm_GetSizeOfComMChannel()) && \
                                                 J1939Rm_IsChannelUsedOfComMChannel(channel))
# define J1939Rm_DA_Valid(da)                   ((da) != J1939RM_NULL_ADDRESS)
# define J1939Rm_SA_Valid(sa)                   (((sa) != J1939RM_NULL_ADDRESS) && ((sa) != J1939RM_BROADCAST_ADDRESS))
# define J1939Rm_AckCode_Valid(ackCode)         ((ackCode) <= J1939RM_ACK_CANNOT_RESPOND)
# define J1939Rm_AckExtIdInfo_Valid(extIdInfo)  (((extIdInfo) == NULL_PTR) || ((extIdInfo)->ExtIdType <= J1939RM_EXTID_GF))
# define J1939Rm_ReqExtIdInfo_Valid(extIdInfo)  (((extIdInfo) == NULL_PTR) || ((extIdInfo)->ExtIdType < J1939RM_EXTID_GF))
# define J1939Rm_Priority_Valid(priority)       ((priority) <= J1939RM_MAX_PRIORITY)

/* ACKM access and check macros */
# define J1939Rm_AckExtIdCode_Valid(extIdCode)  (((extIdCode) == J1939RM_ACKM_CODE_EXTID_NONE) || \
                                                 ((((extIdCode) >= J1939RM_ACKM_CODE_EXTID_ONE) && \
                                                  ((extIdCode) <= J1939RM_ACKM_CODE_EXTID_THREE))))

/* RQST2 access and check macros */
# define J1939Rm_GetTransferType(specInstr)     ((specInstr) & J1939RM_RQST2_TRANSFER_MASK)
# define J1939Rm_TransferType_Valid(specInstr)  (J1939Rm_GetTransferType(specInstr) != J1939RM_RQST2_TRANSFER_UNDEF)
# define J1939Rm_GetExtIdType(specInstr)        ((specInstr) & J1939RM_RQST2_EXTIDTYPE_MASK)
# define J1939Rm_ExtIdType_Valid(specInstr)     ((J1939Rm_GetExtIdType(specInstr) <= J1939RM_RQST2_EXTIDTYPE_THREE) || \
                                                 (J1939Rm_GetExtIdType(specInstr) == J1939RM_RQST2_EXTIDTYPE_NA))

/* Development Error Detection */
# if (J1939RM_DEV_ERROR_REPORT == STD_ON)
#  define J1939Rm_ReportDetError(SID, ERROR)    ((void) Det_ReportError(J1939RM_MODULE_ID, J1939RM_INSTANCE_ID_DET, \
                                                                       (SID), (ERROR)))
# else
#  define J1939Rm_ReportDetError(SID, ERROR)
# endif

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
#  define J1939Rm_ReportRuntimeError(SID, ERROR)    ((void) Det_ReportError(J1939RM_MODULE_ID, J1939RM_INSTANCE_ID_DET, \
                                                                       (SID), (ERROR)))
# else
#  define J1939Rm_ReportRuntimeError(SID, ERROR)
# endif

/**********************************************************************************************************************
 *  INTERNAL DATA PROTOTYPES
 *********************************************************************************************************************/

#endif /* J1939RM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Rm_Int.h
 *********************************************************************************************************************/
