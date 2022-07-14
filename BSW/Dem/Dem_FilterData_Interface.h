/* ********************************************************************************************************************
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
 */
/*! \ingroup    Dem_Master
 *  \defgroup   Dem_FilterData DCM Filter Data
 *  \{
 *  \file       Dem_FilterData_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of FilterData subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Savas Ates                    vissat        Vector Informatik GmbH
 *  Anna Bosch                    visanh        Vector Informatik GmbH
 *  Stefan Huebner                vishrs        Vector Informatik GmbH
 *  Thomas Dedler                 visdth        Vector Informatik GmbH
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  Matthias Heil                 vismhe        Vector Informatik GmbH
 *  Erik Jeglorz                  visejz        Vector Informatik GmbH
 *  Friederike Schulze            visfrs        Vector Informatik GmbH
 *  Aswin Vijayamohanan Nair      visavi        Vector Informatik GmbH
 *  Fabian Wild                   viszfa        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_FILTERDATA_INTERFACE_H)
#define DEM_FILTERDATA_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_FilterData_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_FilterData_Properties Properties
 * \{
 */

/* ****************************************************************************
 * Dem_FilterData_TestFilterSet()
 *****************************************************************************/
/*!
 * \brief         Test if a filter has been set.
 *
 * \details       Test if a filter has been set.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        TRUE
 *                A filter has been set.
 * \return        FALSE
 *                No filter has been set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestFilterSet(
  CONST(Dem_FilterData_HandleType, AUTOMATIC) FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_TestUseStatusMask()
 *****************************************************************************/
/*!
 * \brief         Get the UseStatusMask flag of the given filter flags.
 *
 * \details       Get the UseStatusMask flag of the given filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \return        The state of the UseStatusMask flag.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestUseStatusMask(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_TestUseFdc()
 *****************************************************************************/
/*!
 * \brief         Get the UseFdc flag of the given filter flags.
 *
 * \details       Get the UseFdc flag of the given filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \return        The state of the UseFdc flag.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestUseFdc(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_TestUseSeverity()
 *****************************************************************************/
/*!
 * \brief         Get the UseSeverity flag of the given filter flags.
 *
 * \details       Get the UseSeverity flag of the given filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \return        The state of the UseSeverity flag.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestUseSeverity(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_TestUseFormatOBD()
 *****************************************************************************/
/*!
 * \brief         Get the UseFormatOBD flag of the given filter flags.
 *
 * \details       Get the UseFormatOBD flag of the given filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \return        The state of the UseFormatOBD flag.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestUseFormatOBD(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_TestFilterEmissionRelevant()
 *****************************************************************************/
/*!
 * \brief         Test the emission relevant flag in the filter flags
 *
 * \details       Test the emission relevant flag in the filter flags
 *
 * \param[in]     FilterId
 *                The Filter object handle
 *
 * \return        TRUE
 *                The emission relevant flag is set
 * \return        TRUE
 *                The emission relevant flag is not set
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_TestFilterEmissionRelevant(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_FilterData_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_FilterData_GetState()
 *****************************************************************************/
/*!
 * \brief         Get the filter state of the given filter object.
 *
 * \details       Get the filter state of the given filter object.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The filter state of the filter object.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_FilterData_FilterType, DEM_CODE)
Dem_FilterData_GetState(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetIteratorIndexOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the iterator index stored in the filter object.
 *
 * \details       Get the iterator index stored in the filter object for DTC 
 *                filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The iterator index stored in the filter object for DTC 
 *                filtering.
 *
 * \return        DEM_EVENT_INVALID if no DTC filter is set up.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_GetIteratorIndexOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetChronoIndexOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the Chrono Index stored in the filter object.
 *
 * \details       Get the Chrono Index in the filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The Chrono Index stored in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FilterData_GetChronoIndexOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetStatusMaskOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the status mask stored in the filter object.
 *
 * \details       Get the status mask stored in the filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The status mask stored in the filter object for DTC filtering.
 *
 * \return        0x00 if no DTC filter is set up.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FilterData_GetStatusMaskOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetFDCOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the fault detection counter in the given filter object.
 *
 * \details       Set the fault detection counter in the given filter object.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The FDC stored filter object.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_FilterData_GetFDCOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetMemoryIdOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the MemoryId stored in the filter object.
 *
 * \details       Get the MemoryId stored in the filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The MemoryId stored in the filter object for DTC filtering.
 *
 * \return        DEM_CFG_MEMORYID_INVALID if no DTC filter is set up.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FilterData_GetMemoryIdOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetSeverityMaskOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Get the severity mask stored in the filter object.
 *
 * \details       Get the severity mask stored in the filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        The severity mask stored in the filter object for DTC filtering.
 *
 * \return        0x00 if no DTC filter is set up.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FilterData_GetSeverityMaskOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_GetFilterFunction()
 *****************************************************************************/
/*!
 * \brief         Get the filter function stored in the filter object.
 *
 * \details       Get a pointer to the filter function stored in the filter object.
 *
 * \param[in]     FilterId
 *                The object to read from.
 *
 * \return        A pointer to the filter function stored in the filter object.
 *
 * \return        NULL_PTR if no filter is set up.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_FilterData_FuncPtrType, DEM_CODE)
Dem_FilterData_GetFilterFunction(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_SetState()
 *****************************************************************************/
/*!
 * \brief         Set the filter state in the given filter object
 *
 * \details       Set the filter state in the given filter object to the given value
 *
 * \param[in]     FilterId
 *                The object to write to.
 * \param[in]     NewState
 *                The new state to be set in the filter object.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetState(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_FilterData_FilterType, AUTOMATIC)  NewState
  );

/* ****************************************************************************
 * Dem_FilterData_SetEventIdOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the EventId in the given filter object
 *
 * \details       Set the EventId in the given filter object for DTC filtering
 *
 * \param[in]     FilterId
 *                The object to write to.
 * \param[in]     EventId
 *                The EventId to be set in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetEventIdOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_FilterData_SetChronoIndexOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the Chrono Index in the filter object.
 *
 * \details       Set the Chrono Index in the filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to read from.
 * \param[in]     ChronoIndex
 *                The Chrono Index to be set in the filter object.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetChronoIndexOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  ChronoIndex
  );

/* ****************************************************************************
 * Dem_FilterData_SetStatusMaskOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the status mask in the given filter object.
 *
 * \details       Set the status mask in the given filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \param[in]     StatusMask
 *                The status mask to be set in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetStatusMaskOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  StatusMask
  );

/* ****************************************************************************
 * Dem_FilterData_SetFDCOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the status mask in the given filter object.
 *
 * \details       Set the status mask in the given filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \param[in]     FDC
 *                The FDC to be set in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetFDCOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(sint8, AUTOMATIC)  FDC
  );

/* ****************************************************************************
 * Dem_FilterData_SetMemoryIdOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the memory Id in the given filter object.
 *
 * \details       Set the memory Id in the given filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \param[in]     MemoryId
 *                The memory Id to be set in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetMemoryIdOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  MemoryId
  );

/* ****************************************************************************
 * Dem_FilterData_SetSeverityMaskOfDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Set the severity mask in the given filter object.
 *
 * \details       Set the severity mask in the given filter object for DTC filtering.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \param[in]     SeverityMask
 *                The severity mask to be set in the filter object for DTC filtering.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetSeverityMaskOfDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  SeverityMask
  );

/* ****************************************************************************
 * Dem_FilterData_SetFilterFunction()
 *****************************************************************************/
/*!
 * \brief         Set the filter function in the given filter object.
 *
 * \details       Set the pointer to the filter function in the given filter object.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \param[in]     FilterFunction
 *                The pointer of the filter function to be set in the filter object.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetFilterFunction(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_FilterData_FuncPtrType, AUTOMATIC)  FilterFunction
  );

/* ****************************************************************************
 * Dem_FilterData_SetUseStatusMask()
 *****************************************************************************/
/*!
 * \brief         Set the UseStatusMask flag in the filter flags.
 *
 * \details       Set the UseStatusMask flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetUseStatusMask(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_ResetUseStatusMask()
 *****************************************************************************/
/*!
 * \brief         Reset the UseStatusMask flag in the filter flags.
 *
 * \details       Reset the UseStatusMask flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetUseStatusMask(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_SetUseFdc()
 *****************************************************************************/
/*!
 * \brief         Set the UseFdc flag in the filter flags.
 *
 * \details       Set the UseFdc flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetUseFdc(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_ResetUseFdc()
 *****************************************************************************/
/*!
 * \brief         Reset the UseStatusMask flag in the filter flags.
 *
 * \details       Reset the UseStatusMask flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetUseFdc(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_SetUseSeverity()
 *****************************************************************************/
/*!
 * \brief         Set the UseSeverity flag in the filter flags.
 *
 * \details       Set the UseSeverity flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetUseSeverity(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_ResetUseSeverity()
 *****************************************************************************/
/*!
 * \brief         Reset the UseSeverity flag in the filter flags.
 *
 * \details       Reset the UseSeverity flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetUseSeverity(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_SetUseFormatOBD()
 *****************************************************************************/
/*!
 * \brief         Set the UseFormatOBD flag in the filter flags.
 *
 * \details       Set the UseFormatOBD flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetUseFormatOBD(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_ResetUseFormatOBD()
 *****************************************************************************/
/*!
 * \brief         Reset the UseFormatOBD flag in the filter flags.
 *
 * \details       Reset the UseFormatOBD flag in the filter flags.
 *
 * \param[in]     FilterId
 *                The Filter object handle.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetUseFormatOBD(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_SetFilterEmissionRelevant()
 *****************************************************************************/
/*!
 * \brief         Set the emission relevant flag in the filter flags
 *
 * \details       Set the emission relevant flag in the filter flags
 *
 * \param[in]     FilterId
 *                The Filter object handle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_SetFilterEmissionRelevant(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

/* ****************************************************************************
 * Dem_FilterData_ResetFilterEmissionRelevant()
 *****************************************************************************/
/*!
 * \brief         Set the emission relevant flag in the filter flags
 *
 * \details       Set the emission relevant flag in the filter flags
 *
 * \param[in]     FilterId
 *                The Filter object handle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetFilterEmissionRelevant(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_ResetReportedEvents()
 *****************************************************************************/
/*!
 * \brief         Clear the bitfield of reported events for the given filter.
 *
 * \details       Clear the bitfield of reported events for the given filter.
 *                Function does nothing if chronological reporting is not 
 *                required.
 *
 * \param[in]     FilterId
 *                The object to write to.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_ResetReportedEvents(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_InitDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Initialize a filter for a 'filter DTC' request.
 *
 * \details       Initialize the event and memory Id and the filter function
 *                for a 'filter DTC' request.
 *
 * \param[in]     FilterId
 *                The filter to be set.
 * \param[in]     MemoryId
 *                The event memory handle to be set in the filter.
 * \param[in]     StatusMask
 *                The status mask to be considered in the filter.
 *
 * \return        E_OK
 *                The filter was initialized.
 * \return        E_NOT_OK
 *                The filter was not initialized due to an unconfigured memory.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_InitDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  MemoryId,
  CONST(uint8, AUTOMATIC)  StatusMask
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_InitFilterFlagsAndMasks()
 *****************************************************************************/
/*!
 * \brief         Initialize the filter flags and masks for a 'filter DTC' request.
 *
 * \details       Initialize the filter flags and the status and severity mask
 *                for a 'filter DTC' request.
 *
 * \param[in]     FilterId
 *                The filter to be set.
 * \param[in]     DTCFormat
 *                The DTC number format (OBD/UDS/J1939) to be used for the filter.
 * \param[in]     StatusMask
 *                The status mask to be set in the filter.
 * \param[in]     FilterForOBD
 *                Defines if filtering is restricted to OBD related DTCs.
 * \param[in]     FilterWithSeverity
 *                Defines if severity information is used for filtering.
 * \param[in]     SeverityMask
 *                The severity mask to be set in the filter (only evaluated
 *                if severity information is used for filtering).
 * \param[in]     FilterForFDC
 *                Defines if FDC information is used for filtering.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_InitFilterFlagsAndMasks(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  StatusMask,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONST(boolean, AUTOMATIC)  FilterForOBD,
  CONST(boolean, AUTOMATIC)  FilterWithSeverity,
  CONST(Dem_DTCSeverityType, AUTOMATIC)  SeverityMask,
  CONST(boolean, AUTOMATIC)  FilterForFDC
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) \
    || ((DEM_CFG_FILTERINFOTABLE == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON))
/* ****************************************************************************
 * Dem_FilterData_FilterForEventMatch()
 *****************************************************************************/
/*!
 * \brief         Test whether a given EventId matches the filter.
 *
 * \details       Test whether a given EventId matches the filter.
 *
 * \param[in]     FilterId
 *                The filter to be set.
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \return        E_OK
 *                EventId matches the filter criteria
 * \return        E_NOT_OK
 *                EventId does not mmatche the filter criteria
 *
 * \pre           A filter was set for the FilterId with 
 *                Dem_FilterData_InitFilterFlagsAndMasks()
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                || ((DEM_CFG_FILTERINFOTABLE == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterId
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_FilterForEventMatch(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) \
    || ((DEM_CFG_FILTERINFOTABLE == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON))
/* ****************************************************************************
 * Dem_FilterData_FilterMemoryByEvent
 *****************************************************************************/
/*!
 * \brief         Iterates events in the event memory matching the Dcm DTC filter.
 *
 * \details       This function iterates all events in order of their event ID.
 *                Events which have been reported before by the chronological
 *                iterator Dem_Dcm_FilterMemoryChrono will be ignored.
 *
 * \param[in]     FilterId
 *                The filter to be used.
 * \param[in]     MemoryInfo
 *                Event memory descriptor.
 *
 * \return        EventId of the next event maching the filter criteria.
 *
 * \pre           A Dcm filter has been set.
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                || ((DEM_CFG_FILTERINFOTABLE == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON))
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_FilterMemoryByEvent(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_Mem_MemoryInfoPtrType, AUTOMATIC)  MemoryInfo
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF xf tf */
/* ****************************************************************************
 * Dem_FilterData_FilterPermanentByEvent
 *****************************************************************************/
/*!
 * \brief         Iterates events in permanent memory matching the Dcm DTC
 *                filter.
 *
 * \details       This function iterates all events in order of their event ID.
 *                Events which have been reported before by the chronological
 *                iterator will be ignored.
 *
 * \param[in]     FilterId
 *                The filter to be used.
 * \param[in]     MemoryInfo
 *                Event memory descriptor.
 *
 * \return        EventId of the next event maching the filter criteria.
 *
 * \pre           A Dcm filter has been set.
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON &&
 *                DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_FilterPermanentByEvent(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_Mem_MemoryInfoPtrType, AUTOMATIC)  MemoryInfo
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_FilterMemoryChrono
 *****************************************************************************/
/*!
 * \brief         Iterates events in memory matching the Dcm DTC
 *                filter.
 *
 * \details       If chronological reporting is required this function iterates 
 *                all events in order of occurrence.
 *                If all stored events have been checked, this function
 *                replaces the iterator function by the non-chronological
 *                version Dem_Dcm_FilterMemoryByEvent to also find the
 *                remaining events.
 *                If chronological reporting is not required the function
 *                directly starts filtering events in configuration order.
 *
 * \param[in]     FilterId
 *                The filter to be used.
 * \param[in]     MemoryInfo
 *                Event memory descriptor
 *
 * \return        EventId of the next event maching the filter criteria.
 *
 * \pre           A Dcm filter has been set.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_FilterMemoryChrono(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(Dem_Mem_MemoryInfoPtrType, AUTOMATIC)  MemoryInfo
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_CountMatchesInPermanentMemory
 *****************************************************************************/
/*!
 * \brief         Calculates the number of filtered DTCs for permanent memory.
 *
 * \details       Calculates the number of filtered DTCs for permanent memory.
 *
 * \return        Number of DTCs matching the filter criteria.
 *
 * \pre           A Dcm filter has been set.
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_CountMatchesInPermanentMemory(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_CountMatchesInEventMemory
 *****************************************************************************/
/*!
 * \brief         Calculates the number of filtered events in a standard event memory.
 *
 * \details       Calculates the number of filtered events in a standard event memory.
 *
 * \param[in]     FilterId
 *                The filter to be used.
 *
 * \return        Number of DTCs matching the filter criteria.
 *
 * \pre           A Dcm filter has been set.
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_CountMatchesInEventMemory(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_FilterData_Public Public Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_InitDefault()
 *****************************************************************************/
/*!
 * \brief         Initialize a filter.
 *
 * \details       Initializes a filter, the filter does not contain filter
 *                data afterwards.
 *
 * \param[in]     FilterId
 *                The filter to modify.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
 Dem_FilterData_InitDefault(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_SetDTCFilter()
 *****************************************************************************/
/*!
 * \brief         Initialize a DTC filter.
 *
 * \details       Initializes a DTC filter based on the passed filter information.
 *
 * \param[in]     FilterId
 *                The filter to modify.
 *
 * \param[in]     DTCStatusMask
 *                The status byte mask for DTC status byte filtering.
 *
 * \param[in]     DTCFormat
 *                The DTC number format (OBD/UDS/J1939).
 *
 * \param[in]     MemoryId
 *                The event memory handle.
 *
 * \param[in]     FilterForOBD
 *                Defines if filtering is restricted to OBD relevant DTCs.
 *
 * \param[in]     FilterWithSeverity
 *                Defines if severity information is used for filtering.
 *
 * \param[in]     DTCSeverityMask
 *                The severity mask for severity filtering (only evaluated
 *                if severity information is used for filtering).
 *
 * \param[in]     FilterForFDC
 *                Defines if FDC information is used for filtering.
 *
 * \return        E_OK
 *                Filter was accepted.
 *                E_NOT_OK
 *                Filter was not accepted due to unconfigured memory.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_SetDTCFilter(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONST(uint8, AUTOMATIC)  DTCStatusMask,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONST(uint8, AUTOMATIC)  MemoryId,
  CONST(boolean, AUTOMATIC)  FilterForOBD,
  CONST(boolean, AUTOMATIC)  FilterWithSeverity,
  CONST(Dem_DTCSeverityType, AUTOMATIC)  DTCSeverityMask,
  CONST(boolean, AUTOMATIC)  FilterForFDC
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_CountMatchingDTCs
 *****************************************************************************/
/*!
 * \brief         Calculates the number of filtered events.
 *
 * \details       Calculates the number of filtered events.
 *
 * \param[in]     FilterId
 *                The filter to modify.
 *
 * \return        Number of DTCs matching the filter criteria.
 *
 * \pre           The client specific filter has been set.
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_FilterData_CountMatchingDTCs(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FilterData_GetNextFilteredDTC
 *****************************************************************************/
/*!
 * \brief         Get the next matching event.
 *
 * \details       Get the next event matching the filter criteria set with 
 *                Dem_ClientAccess_SetDTCFilter.
 *
 * \param[in]     FilterId
 *                The filter to modify.
 *
 * \param[out]    FilterResult
 *                Pointer to receive the next matching event.
 *                FilterResult->DTC : 
 *                              The DTC of the matching event.
 *                FilterResult->FDC : 
 *                              The FDC of the matching event, updated only 
 *                              when FilterForFDC is TRUE.
 *                FilterResult->DTCStatus : 
 *                              The DTC status of the matching event.
 *                FilterResult->Severity : 
 *                              The severity of the matching event, updated 
 *                              only when FilterWithSeverity is true.
 *                FilterResult->FunctionalUnit : 
 *                              The functional unit of the matching event, 
 *                              updated only when FilterWithSeverity is true.

 *
 * \return        E_OK
 *                Matching element returned in FilterResult.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No more matching elements, FilterResult not modified.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) Dem_FilterData_GetNextFilteredDTC(
  CONST(Dem_FilterData_HandleType, AUTOMATIC)  FilterId,
  CONSTP2VAR(Dem_FilterData_ResultType, AUTOMATIC, AUTOMATIC)  FilterResult
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_FILTERDATA_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_FilterData_Interface.h
 *********************************************************************************************************************/
