/****************************************************************************
 ** Main author: Ubr                     Creation date: 04/14/05
 ** $LastChangedBy: mgnida $
 ** $Date: 2011-05-26 10:03:02 +0200 (Do, 26. Mai 2011) $
 ** $URL: https://scm/svn/library-es/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_FIPS186.c $
 ** $Rev: 2010 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
 **
 ** \version(cvActLibES 1.0)
 ***************************************************************************/

/****************************************************************************
 **
 **     This file is part of the embedded systems library cvActLib/ES.
 **
 **     Layer: core level
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: RNG implementation user level  according to FIPS 186
 **                     This file provides the core functionality of a
 **                     FIPS 186 random number generator
 **                     This implementation does not use any global or
 **                     static variables. In this version it is simply set
 **                     ontop the actCLib
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initFIPS186
 **   esl_getBytesFIPS186
 **   esl_stirFIPS186
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_RNG.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actFIPS186.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION: esl_initFIPS186
 **
 **  This function initializes the random number generator according
 **  to FIPS-186-2.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - entropyLength: The length of the given entropy used as seed for
 **                the RNG.
 ** - entropy:     A pointer to the value that has to be used as seed.
 ** - savedState:  A pointer to a state that has been saved after the previous
 **                initialisation. Length is given by ESL_SIZEOF_RNGFIPS186_STATE.
 **                If this pointer is set to NULL, no savedState is used.
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_MODE_INVALID      block or padding mode is invalid
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initFIPS186(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceFIPS186 ) workSpace,
   const eslt_Length entropyLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) entropy,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) savedState )
{

   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ) workSpacePtr =
      (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ))workSpace;
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) workSpaceFips =
      (CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ))workSpace->wsFIPS186;

   /* Any NULLs ? */
   if ( (!workSpace) || (!entropy) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_FIPS186 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Check entropy */
   if ( entropyLength < ESL_SIZEOF_RNGFIPS186_STATE )
   {
      return ESL_ERC_RNG_ENTROPY_TOO_SMALL;
   }

   {
      eslt_Byte seed[ESL_SIZEOF_RNGFIPS186_STATE];

      /* Hash entropy into a 160 bit seed value */
      esl_initSHA1( workSpacePtr );
      esl_updateSHA1( workSpacePtr, entropyLength, entropy );
      if ( savedState!=0 )
      {
         esl_updateSHA1( workSpacePtr, ESL_SIZEOF_RNGFIPS186_STATE, savedState );
      }
      esl_finalizeSHA1( workSpacePtr, seed );

      /* Init actCLib FIPS */
      actInitializeFIPS186( workSpaceFips,
                            (CRYPTOCV_P2CONST_PARA( eslt_Byte ))seed );

      /* Output a savedState if desired */
      if ( savedState!=0 ) {
         actGetFIPS186( workSpaceFips,
                        savedState,
                        ESL_SIZEOF_RNGFIPS186_STATE,
                        workSpace->header.watchdog );
      }
   }

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_FIPS186 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_getBytesFIPS186
 **
 **  This function generates random numbers according to FIPS-186-2.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initFIPS186(..) function
 ** - targetLength: the number of bytes that shall be generated
 ** - target      : pointer to output buffer
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - target:      pointer to generated random bytes
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initFIPS186(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_getBytesFIPS186(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceFIPS186 ) workSpace,
   const eslt_Length targetLength, CRYPTOCV_P2VAR_PARA( eslt_Byte ) target )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!target) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_FIPS186 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_FIPS186 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Call actCLib FIPS */
   actGetFIPS186( (CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ))workSpace->wsFIPS186,
                  target,
                  (int)targetLength,
                  workSpace->header.watchdog );

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_stirFIPS186
 **
 **  This function stirs up the internal state of the random number
 **  generator.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initFIPS186(..) function
 ** - inputLength: the number of input bytes used to stir up the
 **                random number generator
 ** - input      : pointer to input bytes used to stir up the
 **                random number generator
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 **
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initFIPS186(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_stirFIPS186(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceFIPS186 ) workSpace,
   const eslt_Length inputLength, CRYPTOCV_P2VAR_PARA( eslt_Byte ) input )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_FIPS186 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_FIPS186 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   {
      eslt_Byte savedState[ESL_SIZEOF_RNGFIPS186_STATE];
      CRYPTOCV_P2VAR_PARA( actU8 ) origState;
      int i;
      origState = ((CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ))workSpace->wsFIPS186)->X_KEY;
      /* Save the internal state of the FIPS generator */

      for (i=0; i<actFIPS186_BASE_LENGTH; ++i)
      {
         savedState[i] = origState[i];
      }
      esl_initFIPS186( workSpace, inputLength, input, savedState );
   }

   return ESL_ERC_NO_ERROR;
}


