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
 *         File:  Fee_Sector.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and 
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as 
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined (FEE_SECTOR_H)
# define FEE_SECTOR_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_SECTOR_MAJOR_VERSION                                   (8u)
# define FEE_SECTOR_MINOR_VERSION                                   (1u)
# define FEE_SECTOR_PATCH_VERSION                                   (3u)

#include "Fee_Cfg.h" /* internal API depends on configuration */

#define FEE_SECTOR_HEADER_SIZE                                        (8u)
#define FEE_SECTOR_LINKTABLE_ELEMENT_SIZE 8u

/*! Pointer/reference types for SectorInfo. Each of the functions declared below, whose first parameter is of such
 *  reference type, can be seen as member function (method) of "class Sector(Info)".
 *  The parameter's type (constness) denotes the method's constness, i.e. whether it is supposed to change obect's
 *  state.
 *  The reference to the object ("this" in C++ and Java) is usually named "self" (in the style of smalltalk).
 *  Reason: It is not recommended to use "this" in C (we use C++ for testing!).
 */
typedef P2VAR(struct Fee_SectorInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_SectorRefType;
typedef P2CONST(struct Fee_SectorInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_SectorConstRefType;

typedef uint32 Fee_SectorAddressType; /* Address Type is used for Flash addresses only */

/*********************************************************************************
 * Sector Functions
 *********************************************************************************/
/* currently we don't implement "clear buffer functionality". References just serve as place holders */
#define Fee_Sector_ClearBuffer(self, buf, length) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Fee_Sector_Init
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the given sector info struct with its start values
 * \details    Marks sector full and empty to signal that it is not usable
 * \param[in,out]  self
 * \param[in,out]  cfg
 * \pre        self is not null, cfg is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Sector_Init(Fee_SectorRefType self, Fee_SectorConfigRefType cfg);


/**********************************************************************************************************************
 * Fee_Sector_alignAddress
 *********************************************************************************************************************/
/*!
 * \brief      Aligns the given value according to sectors configured address alignment
 * \details    Aligns the given value according to sectors configured address alignment
 * \param[in]  self
 * \param[in]  value
 * \return     aligned value
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_alignAddress(Fee_SectorConstRefType self, Fee_LengthType value);


/**********************************************************************************************************************
 * Fee_Sector_alignWrite
 *********************************************************************************************************************/
/*!
 * \brief      Aligns the given value according to sectors configured write alignment
 * \details    Aligns the given value according to sectors configured write alignment
 * \param[in]  self
 * \param[in]  value
 * \return     aligned value
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_alignWrite(Fee_SectorConstRefType self, Fee_LengthType value);


/**********************************************************************************************************************
 * Fee_Sector_allocSpace
 *********************************************************************************************************************/
/*!
 * \brief      Computes address where chunk can be allocated
 * \details    Checks if chunk fits into sector at all
 * \param[in,out]  self
 * \param[in]  byteCount: Size of whole chunk
 * \pre        self is not null
 * \return     0: Chunk does not fit into sector or sector is not writeable
 * \return     address: Address where chunk can be allocated
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_allocSpace(Fee_SectorRefType self, Fee_LengthType byteCount);


/**********************************************************************************************************************
 * Fee_Sector_calcFirstFreeAddress
 *********************************************************************************************************************/
/*!
 * \brief      Calculate the first possible address, directly behind the linktable
 * \details    Calculate the first possible address, directly behind the linktable
 * \param[in]  self
 * \return     address
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_calcFirstFreeAddress(Fee_SectorConstRefType self);


/**********************************************************************************************************************
 * Fee_Sector_free
 *********************************************************************************************************************/
/*!
 * \brief      Signals that sector can be erased
 * \details    Signals that sector can be erased
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Sector_free(Fee_SectorRefType self);



/**********************************************************************************************************************
 * Fee_Sector_reset
 *********************************************************************************************************************/
/*!
 * \brief      Make given sector internally usable (again)
 * \details    n/a
 * \param[in,out]  self
 * \pre        self is not null
 * \pre        sector format succeeded
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Sector_reset(Fee_SectorRefType);



/**********************************************************************************************************************
 * Fee_Sector_evaluateHeaderData
 *********************************************************************************************************************/
/*!
 * \brief      Constructs the sector info which was read before
 * \details    Detects invalid data in buffer
 * \param[in,out]  self: sector info that will be constructed
 * \param[in]  intBuf: buffer containing the read in sector header
 * \param[in]  ReadOk:
 * \return     sectorId
 * \pre        self not null, intBuf is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint8) Fee_Sector_evaluateHeaderData(Fee_SectorRefType self,
                                                            Fee_IntBuffer_u32ConstPtrType intBuf,
                                                            Fee_Bool ReadOk);





/**********************************************************************************************************************
 * Fee_Sector_updateFillLevel
 *********************************************************************************************************************/
/*!
 * \brief      Updates next free address value of given sector with given address
 * \details    Updates next free address value of given sector with given address
 * \param[in,out]  self
 * \param[in]  alignedNfa
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void)Fee_Sector_updateFillLevel(Fee_SectorRefType, Fee_LengthType);


/**********************************************************************************************************************
 * Fee_Sector_confirmFillLevel
 *********************************************************************************************************************/
/*!
 * \brief      Updates and confirms next free address value of given sector with given address
 * \details    Updates and confirms next free address value of given sector with given address
 * \param[in,out]  self
 * \param[in]  alignedNfa
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void)Fee_Sector_confirmFillLevel(Fee_SectorRefType, Fee_LengthType);


/**********************************************************************************************************************
 * Fee_Sector_assembleHeader
 *********************************************************************************************************************/
/*!
 * \brief      Creates data buffer out of given sector info, sectorId and linktableSize
 * \details    Increments erase cycle counter when configured
 * \param[in,out]  self
 * \param[in,out]  headerBuffer_au32
 * \param[in]  sectorId
 * \param[in]  linkTableSize
 * \pre        self is not null, headerBuffer_au32 is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Sector_assembleHeader(Fee_SectorRefType self,
                                                              Fee_IntBuffer_u32PtrType headerBuffer_au32, 
                                                              uint8 sectorId, uint16 linkTableSize);


/**********************************************************************************************************************
 * Fee_Sector_erase
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to erase the given sector in flash
 * \details    Requests Flash to erase the given sector in flash
 * \param[in]  self
 * \return     FEE_REQ_ACCEPTED: Flash accepted erase request
 * \return     FEE_REQ_REJECTED: Flash rejected erase request
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_erase(Fee_SectorConstRefType self);



/**********************************************************************************************************************
 * Fee_Sector_writeData
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to write the given data at given offset within given sector
 * \details    Requests Flash to write the given data at given offset within given sector
 * \param[in]  self
 * \param[in]  offset
 * \param[in]  data
 * \param[in]  byteCount
 * \return     FEE_REQ_ACCEPTED: Flash accepted write request
 * \return     FEE_REQ_REJECTED: Flash rejected write request
 * \pre        self is not null, data is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_writeData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_UserDataConstPtrType data,
                                                                              Fee_LengthType byteCount);




/**********************************************************************************************************************
 * Fee_Sector_readData
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to read the given data at given offset within given sector into the given buffer
 * \details    Requests Flash to read the given data at given offset within given sector into the given buffer
 * \param[in]  self
 * \param[in]  offset
 * \param[in,out]  data
 * \param[in]  byteCount
 * \return     FEE_REQ_ACCEPTED: Flash accepted read request
 * \return     FEE_REQ_REJECTED: Flash rejected read request
 * \pre        self is not null, data is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_readData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_UserDataPtrType data,
                                                                              Fee_LengthType byteCount);



/**********************************************************************************************************************
 * Fee_Sector_writeMngmtData
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to request Flash to write the given management data at given offset within given sector
 * \details    This function is just a wrapping function for Fee_Sector_writeData
 * \param[in]  self
 * \param[in]  offset
 * \param[in]  data
 * \param[in]  byteCount
 * \return     FEE_REQ_ACCEPTED: Flash accepted write request
 * \return     FEE_REQ_REJECTED: Flash rejected write request
 * \pre        self is not null, data is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_writeMngmtData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_IntBuffer_u32ConstPtrType data,
                                                                              Fee_LengthType byteCount);



/**********************************************************************************************************************
 * Fee_Sector_readMngmtData
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to request Flash to read the given management data at given offset within given sector into the given buffer
 * \details    This function is just a wrapping function for Fee_Sector_readData
 * \param[in]  self
 * \param[in]  offset
 * \param[in,out]  data
 * \param[in]  byteCount
 * \return     FEE_REQ_ACCEPTED: Flash accepted read request
 * \return     FEE_REQ_REJECTED: Flash rejected read request
 * \pre        self is not null, data is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_readMngmtData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_IntBuffer_u32PtrType data,
                                                                              Fee_LengthType byteCount);


/**********************************************************************************************************************
 * Fee_Sector_AbsAddr2Offset
 *********************************************************************************************************************/
/*!
 * \brief      Computes the offset of a given address within a given sector
 * \details    Computes the offset of a given address within a given sector
 * \param[in]  self
 * \param[in]  addr
 * \return     offset
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_AbsAddr2Offset(Fee_SectorConstRefType self, Fee_SectorAddressType addr);


/**********************************************************************************************************************
 * Fee_Sector_Offset2AbsAddr
 *********************************************************************************************************************/
/*!
 * \brief      Computes the absolute address out of a given offset within a given sector
 * \details    Computes the absolute address out of a given offset within a given sector
 * \param[in]  self
 * \param[in]  addr
 * \return     absolute address
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_SectorAddressType) Fee_Sector_Offset2AbsAddr(Fee_SectorConstRefType self, Fee_LengthType ofs);


/**********************************************************************************************************************
 * Fee_Sector_containsAbsAddr
 *********************************************************************************************************************/
/*!
 * \brief      Checks if given sector contains the given absolute address
 * \details    Checks if given sector contains the given absolute address
 * \param[in]  self
 * \param[in]  addr
 * \return     FALSE: sector does not contain the address
 * \return     TRUE: sector contains the address
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_containsAbsAddr(Fee_SectorConstRefType self, Fee_SectorAddressType addr);



/**********************************************************************************************************************
 * Fee_Sector_getSize
 *********************************************************************************************************************/
/*!
 * \brief      Returns the logical size of the given sector
 * \details    Returns the logical size of the given sector
 * \param[in]  self
 * \return     size
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_getSize(Fee_SectorConstRefType self);



/**********************************************************************************************************************
 * Fee_Sector_currentLinktableEntryCount
 *********************************************************************************************************************/
/*!
 * \brief      Returns the linktable size of the given sector
 * \details    Returns the linktable size of the given sector
 * \param[in]  self
 * \return     linkTableSize
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint16) Fee_Sector_currentLinktableEntryCount(Fee_SectorConstRefType self);




/**********************************************************************************************************************
 * Fee_Sector_getEraseValueU32
 *********************************************************************************************************************/
/*!
 * \brief      Returns the configured erase value for the given sector
 * \details    Returns the configured erase value for the given sector
 * \param[in]  self
 * \return     eraseValue
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getEraseValueU32(Fee_SectorConstRefType self);


#if(FEE_GET_ERASE_CYCLE_API == STD_ON)
/**********************************************************************************************************************
 * Fee_Sector_getEraseCycleCounter
 *********************************************************************************************************************/
/*!
 * \brief      Returns the erase cycle counter of the given sector
 * \details    Returns the erase cycle counter of the given sector
 * \param[in]  self
 * \return     eraseCycleCounter
 * \config     FEE_GET_ERASE_CYCLE_API is enabled via pre-compile switch
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getEraseCycleCounter(Fee_SectorConstRefType self);
#endif



/**********************************************************************************************************************
 * Fee_Sector_getFillLevel
 *********************************************************************************************************************/
/*!
 * \brief      Returns the next free address of the given sector
 * \details    Returns the next free address of the given sector
 * \param[in]  self
 * \return     0: Sector is empty
 * \return     nextFreeAddress: Sector is not empty
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getFillLevel(Fee_SectorConstRefType self);


/**********************************************************************************************************************
 * Fee_Sector_isUsable
 *********************************************************************************************************************/
/*!
 * \brief      Checks if sector is usable
 * \details    Sector is usable when it is not empty and full at once
 * \param[in]  self
 * \return     FALSE: Sector is not usable
 * \return     TRUE: Sector is usable
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isUsable(Fee_SectorConstRefType self);


/**********************************************************************************************************************
 * Fee_Sector_isDefective
 *********************************************************************************************************************/
/*!
 * \brief      Checks if sector is defective
 * \details    Sector is defective when it is not usable and its next free address is not confirmed
 * \param[in]  self
 * \return     FALSE: Sector is not defective
 * \return     TRUE: Sector is defective
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isDefective(Fee_SectorConstRefType self);


/**********************************************************************************************************************
 * Fee_Sector_isFillLevelKnown
 *********************************************************************************************************************/
/*!
 * \brief      Checks if sectors filllevel is known
 * \details    Fill level is known when sector is unusable or when next free address is confirmed
 * \param[in]  self
 * \return     FALSE: Sectors filllevel is not known
 * \return     TRUE: Sectors filllevel is known
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isFillLevelKnown(Fee_SectorConstRefType self);


/**********************************************************************************************************************
 * Fee_Sector_markFull
 *********************************************************************************************************************/
/*!
 * \brief      Marks the passed sector as FULL.
 * \details    If sector is usable, sector mark it as "not empty", and "full":
 * \param[in,out]  self
 * \pre        self->isUsable() == TRUE
 * \post       Fee_Sector_isUsable() => !Fee_Sector_isWritable() && Fee_Sector_getFillLevel() == Fee_Sector_getSize();
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Sector_markFull(Fee_SectorRefType self);


#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */



#endif /* FEE_SECTOR_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Sector.h
 *********************************************************************************************************************/
