/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library - 
 *
 *  \description  This file is part of the actCLibrary.
 *                Helper functions for block padding.
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

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Padding mode */
#define actPADDING_PM_OFF              0x00u
#define actPADDING_PM_ONEWITHZEROES    0x10u
/* PKCS #7 */
#define actPADDING_PM_PKCS7            0x20u

#define actPADDING_PM_MASK             0xF0u

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

CRYPTOCV_FUNC( actRETURNCODE ) actPaddingInit( actU8 mode );
CRYPTOCV_FUNC( actRETURNCODE ) actPaddingPad(
   actU8 mode, int buffer_size, CRYPTOCV_P2VAR_PARA(int) buffer_used,
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer );
CRYPTOCV_FUNC( actRETURNCODE ) actPaddingUnpad(
   actU8 mode, int buffer_size, CRYPTOCV_P2VAR_PARA(int) buffer_used,
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer );

/***********************************************************************************************************************
 *  END OF FILE: ACTPADDING.H
 **********************************************************************************************************************/
