/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library -  RFC 4493 AES-CMAC
 *
 *  \description  This file is part of the actCLibrary.
 *                Implements AES-CMAC generation according to RFC 4493
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
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

#include "actITypes.h"
#include "actAES.h"

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* Workspace structure for AES CMAC */
typedef actAESSTRUCT actCMACAESSTRUCT;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESInit(
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESUpdate(
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESFinalize(
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) mac,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/***********************************************************************************************************************
 *  END OF FILE: ACTICMACAES.H
 **********************************************************************************************************************/
