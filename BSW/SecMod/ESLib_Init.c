/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/28/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_Init.c $
 ** $Rev: 1620 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2005)
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
 ** This file contains: Initialization functions implementation file.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initESCryptoLib
 **   esl_initWorkSpaceHeader
 **
 ***************************************************************************/

#include "ESLib.h"
#include "ESLib_types.h"

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION: esl_initESCryptoLib
 **
 **  This function sets the library to a default value.
 **
 ** input:
 **   None
 **
 ** output:
 **   None
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initESCryptoLib( void )
{
   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_initWorkSpaceHeader
 **
 **  This function initializes a workSpace header. This function must be
 **  called before calling (nearly) every function from the
 **  cv act library/es to initialize the workSpace header correctly.
 **
 ** input:
 ** - workSpaceHeader: pointer to current workSpace header
 ** - workSpaceSize:   the size of the workSpace to be used (without
 **                    the size of the header, e.g.
 **                    sizeof(eslt_WorkSpaceAES128)-sizeof(eslt_WorkSpaceHeader))
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initWorkSpaceHeader(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) workSpaceHeader, const eslt_Length workSpaceSize,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   /* Any NULLs ? */
   if ( !workSpaceHeader )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* init workSpaceHeader */
   workSpaceHeader->size     = workSpaceSize;
   workSpaceHeader->status   = ESL_WST_ALLOCATED;
   workSpaceHeader->watchdog = watchdog;

   workSpaceHeader->checkSum = 0; /* not implemented at this point */

   return ESL_ERC_NO_ERROR;
}

