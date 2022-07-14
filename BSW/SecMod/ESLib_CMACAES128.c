/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library -  RFC 4493 AES-CMAC (Generation/Verification)
 *
 *  \description  This file is part of the actCLibrary.
 *                Implements AES-CMAC generation and verification according to RFC 4493
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

#include "ESLib.h"
#include "ESLib_types.h"

#include "actICMACAES.h"
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
 *  esl_initCMACAES128
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation
 *  \param[in]     workSpace  CMAC work space
 *  \param[in]     keyLength  Size of AES key to be used
 *  \param[in]     key  AES key to be used
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  AES key length is invalid
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initCMACAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceCMACAES ) workSpace,
   const eslt_Length keyLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) key )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!key) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Check keyLength */
   if ( keyLength != ESL_SIZEOF_AES128_KEY )
   {
      return ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
   }

   /* Init actCLib CMAC-AES */
   (void)actCMACAESInit( (CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ))workSpace->wsCMACAES,
                               key, ESL_SIZEOF_AES128_KEY, workSpace->header.watchdog );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_CMACAES128 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
  esl_getExpandedKeyCMACAES128()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTOCV_FUNC(eslt_ErrorCode) esl_getExpandedKeyCMACAES128(CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace, CRYPTOCV_P2VAR_PARA(eslt_Byte) keyPtr, CRYPTOCV_P2VAR_PARA(eslt_Length) keyLengthPtr) /* PRQA S 3673 */ /* MD_CRYPTOCV_16.7 */
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  if (*keyLengthPtr >= 256u)
  {
    /* Copy expanded key to provided buffer */
    *keyLengthPtr = 256u;
    actMemcpy(keyPtr, ((actCMACAESSTRUCT*)workSpace->wsCMACAES)->e_key, *keyLengthPtr); /* PRQA S 0310 */ /* MD_CRYPTOCV_11.4 */
  }
  else
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }

  return retVal;
}

/**********************************************************************************************************************
  esl_initExpandedCMACAES128()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTOCV_FUNC(eslt_ErrorCode) esl_initExpandedCMACAES128(CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace, CRYPTOCV_P2CONST_PARA(eslt_Byte) expandedKey, const eslt_Length expandedKeyLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  CRYPTOCV_P2VAR_PARA(actCMACAESSTRUCT) internalWorkspace;

  if ((workSpace == NULL_PTR) || (expandedKey == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES)
  {
    retVal = ESL_ERC_WS_TOO_SMALL;
  }
  /* Check keyLength */
  else if (expandedKeyLength != 256u)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE; /* TODO improve return type */
  }
  else
  {
    /* Init actCLib CMAC-AES */
    internalWorkspace = (CRYPTOCV_P2VAR_PARA(actCMACAESSTRUCT))workSpace->wsCMACAES; /* PRQA S 0310 */ /* MD_CRYPTOCV_11.4 */
    actMemset(internalWorkspace->buffer, 0x00u, actAES_BLOCK_SIZE);
    internalWorkspace->buffer_used = 0u;
    internalWorkspace->key_dword_len = 4u;
    actMemcpy(internalWorkspace->e_key, expandedKey, expandedKeyLength); /* PRQA S 0310 */ /* MD_CRYPTOCV_11.4 */

    /* Set workSpace state */
    workSpace->header.status = (ESL_WST_ALGO_CMACAES128 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updateCMACAES128
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation
 *  \param[in]     workSpace  CMAC work space
 *  \param[in]     inputLength  Length of input data
 *  \param[in]     input  Pointer to input data
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \retval        ESL_ERC_INPUT_INVALID  Invalid input length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateCMACAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceCMACAES ) workSpace,
   const eslt_Length inputLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
   actRETURNCODE result;

   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_CMACAES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Update actCLib CMAC-AES */
   result = actCMACAESUpdate( (CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ))workSpace->wsCMACAES,
                                    input, (int)inputLength, workSpace->header.watchdog );

   if ( result == actEXCEPTION_LENGTH )
   {
      return ESL_ERC_INPUT_INVALID;
   }

   return ESL_ERC_NO_ERROR;
}

/***********************************************************************************************************************
 *  esl_finalizeCMACAES128
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC calculation
 *  \param[in]     workSpace  CMAC work space
 *  \param[out]    messageCMAC  Pointer to computed CMAC
 *  \retval        ESL_ERC_NO_ERROR  Operation successful
 *  \retval        ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \retval        ESL_ERC_WS_TOO_SMALL  Work space too small
 *  \retval        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 **********************************************************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeCMACAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceCMACAES ) workSpace, CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageCMAC )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!messageCMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_CMACAES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Finalize actCLib CMAC-AES */
   (void)actCMACAESFinalize( (CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ))workSpace->wsCMACAES,
                                   messageCMAC, workSpace->header.watchdog );

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( workSpace->wsCMACAES, 0, (unsigned int)(workSpace->header.size) );

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
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifyCMACAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceCMACAES ) workSpace, CRYPTOCV_P2CONST_PARA( eslt_Byte ) messageCMAC )
{
   eslt_Byte tmpMAC[ESL_SIZEOF_AES128_BLOCK];

   /* Any NULLs ? */
   if ( (!workSpace) || (!messageCMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_CMACAES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Calculate MAC */
   (void)esl_finalizeCMACAES128(workSpace, tmpMAC);

   if ( actMemcmp(tmpMAC, messageCMAC, ESL_SIZEOF_AES128_BLOCK) != 0 )
   {
      return ESL_ERC_HMAC_INCORRECT_MAC;
   }

   return ESL_ERC_NO_ERROR;
}
