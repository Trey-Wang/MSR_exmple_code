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
/*! \addtogroup Dem_FreezeFrameIterator
 *  \{
 *  \file       Dem_FreezeFrameIterator_Types.h
 *  \brief      Diagnostic Event Manager (Dem) Type and Macro definition file
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

#if !defined (DEM_FREEZEFRAMEITERATOR_TYPES_H)
#define DEM_FREEZEFRAMEITERATOR_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
#include "Dem_SnapshotEntry_Types.h"
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

/*! This used for FreezeFrameIterator handles. */
typedef uint8 Dem_FreezeFrameIterator_HandleType;

/*!
* \defgroup  Dem_FreezeFrameIterator_DTCFormatType Dem_FreezeFrameIterator_DTCFormatType
* \{
*/
/*! This type is used to encode the Dem_DTCFormatType of a FreezeFrameIterator. */
typedef uint8 Dem_FreezeFrameIterator_DTCFormatType;
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT           0  /*!< Initialization value, filter is unused */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_UDS            1  /*!< Selects the 3-byte UDS DTC format (refer to configuration parameter DemUdsDTC) */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD            2  /*!< Selects the 2-byte OBD DTC format (refer to configuration parameter DemObdDTC) */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_J1939          3  /*!< Selects the merged SPN + FMI to 3-byte J1939 DTC format (refer to DemJ1939DTC) */
/*!
* \}
*/

/*! Iterator type for iteration over snapshot records relevant for an event */
typedef Dem_Cfg_EventSRecIterType Dem_FreezeFrameIterator_IterType;
/*! Address of the iterator (const pointer to iterator). */
typedef P2VAR(Dem_FreezeFrameIterator_IterType, TYPEDEF, DEM_VAR_NOINIT) Dem_FreezeFrameIterator_IterPtrType;


/*! FreezeFrameIterator data for Dcm requests */
struct Dem_FreezeFrameIterator_FilterType_s
{
  Dem_FreezeFrameIterator_DTCFormatType FilterDTCFormat;         /*!< DTCFormat: UDS, OBD, J1939, INIT (no filter) */
  uint8                                 MemoryIndex;             /*!< Memory entry type */
  Dem_SnapshotEntry_IterType            SnapshotEntryIter;       /*!< Iterator for the snapshot entries */
  uint8                                 TimeSeriesRecordNumber;  /*!< time series iterator position */
  Dem_FreezeFrameIterator_IterType      Iterator;                /*!< Snapshot record iterator */
};

/*! FreezeFrameIterator data type for Dcm requests */
typedef struct Dem_FreezeFrameIterator_FilterType_s Dem_FreezeFrameIterator_FilterType;

#endif /* DEM_FREEZEFRAMEITERATOR_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_FreezeFrameIterator_Types.h
 *********************************************************************************************************************/
