/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  Ea.h
 *        \brief  EA header file
 *
 *      \details  The EEPROM Abstraction abstracts from the device specific addressing scheme and segmentation and
 *                provides a virtual addressing scheme and segmentation to upper layers as well as a virtually
 *                unlimited number of erase cycles.
 *
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  AUTHOR IDENTITY
 *--------------------------------------------------------------------------------------------------------------------
 *  Initials    Name                    Company
 *  --------    ---------------------   ----------------------------------------------------------------------------
 *  virgmi      Michael Goss            Vector Informatik GmbH
 *--------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *--------------------------------------------------------------------------------------------------------------------
 *  Version     Date        Author      Change Id      Description
 * -------------------------------------------------------------------------------------------------------------------
 *  3.00.00     2015-01-14  virgmi      ESCAN00080285  FEAT-427: SafeBSW Step 1
 *                                      ESCAN00081773  FEAT-1275: SafeBSW Step 2
 *  3.01.00     2015-09-18  virgmi      ESCAN00085107  FEAT-1372: Updated Cfg Version Check 
 *              2015-12-10  virgmi      ESCAN00087047  Mapping of variables in memory was corrected
 *  3.02.00     2016-02-18  virgmi      -              Update of generator version
 *  3.02.01     2016-10-20  virgmi      ESCAN00090022  Fixed compiler warning
 *              2017-01-03  virgmi      -              DataBuffer typedef was moved to Ea_Cfg.h
 *  3.02.02     2017-05-18  virgmi      ESCAN00095114  Global parameters should be updated before callbacks to upper
 *                                                     layer are invoked
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#ifndef EA_H
# define EA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"
# include "MemIf_Types.h"
# include "Ea_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define EA_VENDOR_ID                    (30u)
# define EA_MODULE_ID                    (40u)
# define EA_INSTANCE_ID                  (0u)

# define EA_AR_RELEASE_MAJOR_VERSION     (4u)
# define EA_AR_RELEASE_MINOR_VERSION     (0u)
# define EA_AR_RELEASE_REVISION_VERSION  (3u)

# define EA_SW_MAJOR_VERSION             (3u)
# define EA_SW_MINOR_VERSION             (2u)
# define EA_SW_PATCH_VERSION             (2u)


/**** Error Codes in Development-Mode ******************************************************************************/
/* AUTOSAR 4 development error codes */ /* \trace SPEC-41655 */
# define EA_E_NO_ERROR                   (0x00u)
# define EA_E_UNINIT                     (0x01u)
# define EA_E_INVALID_BLOCK_NO           (0x02u)
# define EA_E_INVALID_BLOCK_OFS          (0x03u)
# define EA_E_INVALID_DATA_POINTER       (0x04u)
# define EA_E_INVALID_BLOCK_LEN          (0x05u)
# define EA_E_BUSY                       (0x06u)
# define EA_E_BUSY_INTERNAL              (0x07u)
# define EA_E_INVALID_CANCEL             (0x08u)

/* Development error codes additionally defined to AUTOSAR */
# define EA_E_PARAM_MODE                 (0x0Au)

/**** Ea Service Identification ************************************************************************************/
# define EA_SID_INIT                     (0x00u)
# define EA_SID_SET_MODE                 (0x01u)
# define EA_SID_READ                     (0x02u)
# define EA_SID_WRITE                    (0x03u)
# define EA_SID_CANCEL                   (0x04u)
# define EA_SID_GET_STATUS               (0x05u)
# define EA_SID_GET_JOB_RESULT           (0x06u)
# define EA_SID_INVALIDATE_BLOCK         (0x07u)
# define EA_SID_GET_VERSION_INFO         (0x08u)
# define EA_SID_ERASE_IMMEDIATE_BLOCK    (0x09u)
# define EA_SID_JOB_END_NOTIFICATION     (0x10u)
# define EA_SID_JOB_ERROR_NOTIFICATION   (0x11u)
# define EA_SID_MAINFUNCTION             (0x12u)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef enum
{
  EA_READ_JOB = 0,
  EA_WRITE_JOB,
  EA_INVALIDATE_JOB,
  EA_ERASE_JOB
} EEaJobType;

typedef struct
{
  uint16 BlockNumber;
  uint16 BlockOffset;
  uint16 Length;
  Ea_DataBufferPtrType DataBufferPtr;
  EEaJobType JobType;
}Ea_UserJobParameterType;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
 *  Ea_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the EEPROM abstraction module.
 * \details    Initializes internal component parameter in order to prepare module for successful job processing.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-711
 * \trace SPEC-41613, SPEC-41597
 */
FUNC(void, EA_PUBLIC_CODE) Ea_Init (void);

 /**********************************************************************************************************************
  *  Ea_Read()
  *********************************************************************************************************************/
/*!
 * \brief      Reads Length bytes of block BlockNumber at offset BlockOffset into the buffer DataBufferPtr.
 * \details    Reads Length bytes of block BlockNumber at offset BlockOffset into the buffer DataBufferPtr.
 * \param[in]  BlockNumber
 * \param[in]  BlockOffset - read offset within block
 * \param[in]  DataBufferPtr
 * \param[in]  Length of read job
 * \return     Returns if requested job has been accepted by the EA module.
 * \pre        EA component status is MEMIF_IDLE.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 * \trace CREQ-706
 * \trace SPEC-41627, SPEC-41603, SPEC-41719, SPEC-41622, SPEC-41651, SPEC-41621
 * \trace SPEC-41593, SPEC-41703, SPEC-41580, SPEC-41644, SPEC-41672
 * \trace DSGN-Ea23438
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_Read (uint16 BlockNumber, uint16 BlockOffset,
        Ea_DataBufferPtrType DataBufferPtr, uint16 Length);

/**********************************************************************************************************************
 *  Ea_Write()
 *********************************************************************************************************************/
/*!
 * \brief      Writes the contents of the DataBufferPtr to the block BlockNumber.
 * \details    Writes the contents of the DataBufferPtr to the block BlockNumber.
 * \param[in]  BlockNumber
 * \param[in]  DataBufferPtr
 * \return     Returns if requested job has been accepted by the EA module.
 * \pre        EA component status is MEMIF_IDLE.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 * \trace CREQ-704
 * \trace SPEC-41629, SPEC-41712, SPEC-41683, SPEC-41639, SPEC-41631
 * \trace SPEC-41702, SPEC-41588, SPEC-41579, SPEC-41664, SPEC-41626
 * \trace DSGN-Ea23439
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_Write (uint16 BlockNumber, Ea_DataBufferPtrType DataBufferPtr);

/**********************************************************************************************************************
 *  Ea_InvalidateBlock()
 *********************************************************************************************************************/
/*!
 * \brief      Invalidates the block BlockNumber.
 * \details    Invalidates the block BlockNumber.
 * \param[in]  BlockNumber
 * \return     Returns if requested job has been accepted by the EA module.
 * \pre        EA component status is MEMIF_IDLE.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 * \trace CREQ-708
 * \trace SPEC-41595, SPEC-41688, SPEC-41668, SPEC-41637, SPEC-41625, SPEC-41706
 * \trace DSGN-Ea23440
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_InvalidateBlock (uint16 BlockNumber);

/**********************************************************************************************************************
 *  Ea_EraseImmediateBlock()
 *********************************************************************************************************************/
/*!
 * \brief      Erases the block BlockNumber.
 * \details    Erases the entire dataset which is referenced by BlockNumber.
 * \param[in]  BlockNumber
 * \return     Returns if requested job has been accepted by the EA module.
 * \pre        EA component status is MEMIF_IDLE.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 * \trace CREQ-709
 * \trace SPEC-41604, SPEC-41649, SPEC-41611, SPEC-41601, SPEC-41654, SPEC-41701, SPEC-41699
 * \trace DSGN-Ea23441
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_EraseImmediateBlock (uint16 BlockNumber);

/**********************************************************************************************************************
 *  Ea_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels the ongoing asynchronous operation.
 * \details    Cancels the ongoing asynchronous operation.
 * \pre        EA component status is MEMIF_BUSY.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-713
 * \trace SPEC-41619, SPEC-41587, SPEC-41684, SPEC-41678, SPEC-41666, SPEC-41663
 * \trace DSGN-Ea23452
 */
FUNC(void, EA_PUBLIC_CODE) Ea_Cancel (void);

/**********************************************************************************************************************
 *  Ea_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Service to return the status.
 * \details    Returns current status of EA module.
 * \return     Status of EA module.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-714
 * \trace SPEC-41591, SPEC-41713, SPEC-41620, SPEC-41647
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) Ea_GetStatus (void);

/**********************************************************************************************************************
 *  Ea_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Service to return the job result.
 * \details    Returns current job result of EA module.
 * \return     Job result of EA module.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-712
 * \trace SPEC-41628, SPEC-41617, SPEC-41721, SPEC-41686
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_GetJobResult (void);

/**********************************************************************************************************************
 *  Ea_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * \brief      Service to get the version information of EA module.
 * \details    Stores version information, i.e. Module Id, Vendor Id, Vendor specific version numbers,
 *             to structure pointed by VersionInfoPtr.
 * \param[in]  VersionInfoPtr
 * \pre        -
 * \config     GetVersionInfo API is enabled via pre-compile switch.
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace CREQ-911
 * \trace SPEC-41716, SPEC-41630, SPEC-41583, SPEC-41660
 * \trace DSGN-Ea22733
 */
FUNC(void, EA_PUBLIC_CODE) Ea_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr);

/**********************************************************************************************************************
 *  Ea_SetMode()
 *********************************************************************************************************************/
/*!
 * \brief      Sets the mode.
 * \details    Sets the mode Mode for the underlying EEPROM driver.
 * \param[in]  Mode
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace SPEC-41586
 */
FUNC(void, EA_PUBLIC_CODE) Ea_SetMode (MemIf_ModeType Mode);

/**********************************************************************************************************************
 *  Ea_MainFunction()
 *********************************************************************************************************************/
/*!
 * \brief      Service to handle the requested jobs and the internal management operations.
 * \details    Service to handle the requested jobs and the internal management operations.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace SPEC-41694, SPEC-41677
 */
FUNC(void, EA_PUBLIC_CODE) Ea_MainFunction (void);

# define EA_STOP_SEC_CODE
# include "MemMap.h"


# ifdef EA_IMPLEMENTATION_SOURCE /* COV_EA_INTERNAL_SERVICE */

#  define EA_START_SEC_CODE
#  include "MemMap.h"

/**********************************************************************************************************************
 *  Ea_AlignValue()
 *********************************************************************************************************************/
/*!
 * \brief      Aligns passed Value to Alignment
 * \details    Aligns passed Value to Alignment. If Value is not aligned, the next larger aligned value is returned
 * \param[in]  Value
 * \param[in]  Alignment
 * \return     Aligned value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea22974
 */
FUNC (uint16, EA_PUBLIC_CODE) Ea_AlignValue(uint16 Value, uint16 Alignment);

#  define EA_STOP_SEC_CODE
#  include "MemMap.h"

# endif /* EA_IMPLEMENTATION_SOURCE */

#endif /* EA_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea.h
 *********************************************************************************************************************/
