/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/testBench/source/testSHA512-256.c $
 ** $Revision: 2198 $
 ** $Date: 2016-01-13 08:46:05 +0100 (Mi, 13 Jan 2016) $
 ** $Author: mneuhaus $
 **
 ** \copyright(cv cryptovision GmbH, 2016)
 **
 ** \version(cv library/es 1.2)
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** Ed25519 helper functions
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actEd25519hashPrefixAndMessage
 **   actEd25519hashRAandMessage
 **
 ***************************************************************************/

#ifndef     ACTED25519_H
    #define ACTED25519_H

    #include "actITypes.h"
    #include "actIED25519.h"
    #include "actISHA2_64.h"


    /****************************************************************************
     ** Function Prototypes
     ***************************************************************************/

    #ifdef  __cplusplus
        extern "C" {
    #endif


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE act25519_hashPM (actEd25519Struct *ws, actU8 *hash)
     **
     **  This function performs step 2 of the Ed25519 signature algorithm
     **  (https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6).
     **
     ** input:
     ** - ws:       pointer to the work-space structure
     ** - hash:     pointer to the output buffer for the hash value
     ** - watchdog: pointer to watchdog reset function
     **
     ** output:
     ** - hash:     updated with the hash value
     ** - returns:  actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    CRYPTOCV_FUNC( actRETURNCODE ) act25519_hashPM (
      CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
      CRYPTOCV_P2VAR_PARA( actU8 ) hash,
      CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void));


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE act25519_hashRAM (actEd25519Struct *ws, actU8 *hash)
     **
     **  This function performs step 4 of the Ed25519 signature algorithm
     **  (https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6)
     **  or step 2 of the Ed25519 verification algorithm
     **  (https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.7).
     **
     ** input:
     ** - ws:       pointer to the work-space structure
     ** - R:        pointer to the R value
     ** - hash:     pointer to the output buffer for the hash value
     ** - watchdog: pointer to watchdog reset function
     **
     ** output:
     ** - hash:     updated with the hash value
     ** - returns:  actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    CRYPTOCV_FUNC( actRETURNCODE ) act25519_hashRAM (
      CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
      CRYPTOCV_P2CONST_PARA( actU8 ) R,
      CRYPTOCV_P2VAR_PARA( actU8 ) hash,
      CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void));


    #ifdef  __cplusplus
        } /* extern "C" */
    #endif

#endif  //  ACTED25519_H