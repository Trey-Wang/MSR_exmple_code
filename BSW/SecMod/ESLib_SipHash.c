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

#include "ESLib.h"
#include "ESLib_types.h"

#include "actISipHash.h"
#include "actUtilities.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  esl_initSipHash
 **********************************************************************************************************************/
/*! \brief         Initialize SipHash calculation
 *  \param[in]     workSpace  SipHash work space
 *  \param[in]     keyLength  Size of SipHash key to be used
 *  \param[in]     key  SipHash key to be used
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  SipHash key length is invalid
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initSipHash(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSipHash ) workSpace,
   const eslt_Length keyLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) key )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!key) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SIPHASH )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Check keyLength */
   if ( keyLength != ESL_SIZEOF_SIPHASH_KEY )
   {
      return ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
   }

   /* Init actCLib SipHash */
   (void)actSipHashInit( (CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ))workSpace->wsSipHash,
                         key, ESL_SIZEOF_SIPHASH_KEY, workSpace->header.watchdog );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_SIPHASH | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}

/***********************************************************************************************************************
 *  esl_updateSipHash
 **********************************************************************************************************************/
/*! \brief         Update SipHash calculation
 *  \param[in]     workSpace  SipHash work space
 *  \param[in]     inputLength  Length of input data
 *  \param[in]     input  Pointer to input data
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \retval        ESL_ERC_INPUT_INVALID  Invalid input length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateSipHash(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSipHash ) workSpace,
   const eslt_Length inputLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
   actRETURNCODE result;

   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SIPHASH )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SIPHASH )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Update actCLib SipHash */
   result = actSipHashUpdate ( (CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ))workSpace->wsSipHash,
                               input, (int)inputLength, workSpace->header.watchdog );

   if ( result == actEXCEPTION_LENGTH )
   {
      return ESL_ERC_INPUT_INVALID;
   }

   return ESL_ERC_NO_ERROR;
}

/***********************************************************************************************************************
 *  esl_finalizeSipHash
 **********************************************************************************************************************/
/*! \brief         Finalize SipHash calculation
 *  \param[in]     workSpace  SipHash work space
 *  \param[out]    messageMAC  Pointer to computed MAC
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeSipHash(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSipHash ) workSpace, CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageMAC )
{
   actU64 mac64;
   /* Any NULLs ? */
   if ( (!workSpace) || (!messageMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SIPHASH )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SIPHASH )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Finalize actCLib SipHash */
   (void)actSipHashFinalize( (CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ))workSpace->wsSipHash,
                             &mac64, workSpace->header.watchdog );

   messageMAC[0] = (eslt_Byte)mac64;
   messageMAC[1] = (eslt_Byte)((mac64 >> 8) & 0xFFull);
   messageMAC[2] = (eslt_Byte)((mac64 >> 16) & 0xFFull);
   messageMAC[3] = (eslt_Byte)((mac64 >> 24) & 0xFFull);
   messageMAC[4] = (eslt_Byte)((mac64 >> 32) & 0xFFull);
   messageMAC[5] = (eslt_Byte)((mac64 >> 40) & 0xFFull);
   messageMAC[6] = (eslt_Byte)((mac64 >> 48) & 0xFFull);
   messageMAC[7] = (eslt_Byte)((mac64 >> 56) & 0xFFull);

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( workSpace->wsSipHash, 0, (unsigned int)(workSpace->header.size) );

   return ESL_ERC_NO_ERROR;
}

/***********************************************************************************************************************
 *  esl_verifyCMACAES128
 **********************************************************************************************************************/
/*! \brief         Verify given CMAC against calculated one
 *  \param[in]     workSpace  CMAC work space
 *  \param[in]     messageCMAC  Pointer to CMAC to be compared
 *  \retval        ESL_ERC_NO_ERROR  CMAC is correct
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifySipHash(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSipHash ) workSpace, CRYPTOCV_P2CONST_PARA( eslt_Byte ) messageMAC )
{
   eslt_Byte tmpMAC[sizeof(actU64)];

   /* Any NULLs ? */
   if ( (!workSpace) || (!messageMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SIPHASH )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SIPHASH )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Calculate MAC */
   (void)esl_finalizeSipHash(workSpace, tmpMAC);

   if ( actMemcmp(tmpMAC, messageMAC, sizeof(actU64)) != 0 )
   {
      return ESL_ERC_HMAC_INCORRECT_MAC;
   }

   return ESL_ERC_NO_ERROR;
}
