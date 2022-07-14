/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library - SipHash
 *
 *  \description  This file is part of the actCLibrary.
 *                Implements SipHash according to 'SipHash: a short-input PRF' from Jean-Philippe Aumasson and Daniel
 *                Bernstein
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#ifndef ACTISIPHASH_H
#define ACTISIPHASH_H

#include "actITypes.h"

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* default: SipHash-2-4 */
#ifndef ACT_SIPHASH_CROUNDS
# define ACT_SIPHASH_CROUNDS 2
#endif

#ifndef ACT_SIPHASH_DROUNDS
# define ACT_SIPHASH_DROUNDS 4
#endif

#define actSIPHASH_BLOCK_SIZE 8u

/* Workspace structure for SipHash */
typedef struct {
  actU64 v0;
  actU64 v1;
  actU64 v2;
  actU64 v3;
  actU64 buffer;
  actU32 length;
  actU8 buffer_used;
} actSipHashSTRUCT;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

CRYPTOCV_FUNC( actRETURNCODE ) actSipHashInit (
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

CRYPTOCV_FUNC( actRETURNCODE ) actSipHashUpdate (
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

CRYPTOCV_FUNC( actRETURNCODE ) actSipHashFinalize(
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU64 ) mac,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

#endif

/***********************************************************************************************************************
 *  END OF FILE: ACTISIPHASH.H
 **********************************************************************************************************************/
