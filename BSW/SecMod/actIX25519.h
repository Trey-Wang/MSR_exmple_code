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
 ** X25519 internal programming interface
 **
 ** constants:
 **
 ** types:
 **   actX25519Struct
 **
 ** macros:
 **
 ** functions:
 **   actX25519Init
 **   actX25519GenKeyPair
 **   actX25519GenSecret
 **
 ***************************************************************************/

#ifndef     ACTIX25519_H
    #define ACTIX25519_H

    #include "actITypes.h"


    /****************************************************************************
     ** Types and constants
     ***************************************************************************/

    #define   actX25519_KEY_SIZE    32

    /* Workspace structure for X25519 */
    typedef struct {
        actU8   privateKey  [actX25519_KEY_SIZE];   /*  private key       */
        actU8   uCoord      [actX25519_KEY_SIZE];   /*  u (x) coordinate  */

        actU8   coreWS      [520];                  /*  workspace for calculation core (layout depends on core used)  */
    } actX25519Struct;


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
     ** actRETURNCODE actX25519Init (actX25519Struct *ws)
     **
     **  This function initializes the X25519 algorithm.
     **
     ** input:
     ** - ws:           pointer to the work-space structure
     **
     ** output:
     ** - ws:           initialized work-space structure
     ** - returns:      actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    extern CRYPTOCV_FUNC(actRETURNCODE) actX25519Init (CRYPTOCV_P2VAR_PARA( actX25519Struct )ws);


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actX25519GenKeyPair (actEd25519Struct *ws, actU8 *publicKey, void (* watchdog) (void))
     **
     **  This function generates an ephemeral key-pair for EC-D/H.
     **
     ** input:
     ** - ws:           pointer to the work-space structure
     ** - publicKey:    pointer to memory for the public key
     ** - watchdog:     pointer to watchdog reset function
     **
     ** output:
     ** - returns:      actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    extern CRYPTOCV_FUNC( actRETURNCODE ) actX25519GenKeyPair (
      CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
      CRYPTOCV_P2VAR_PARA( actU8 ) publicKey,
      CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ));


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actX25519ImportPrivateKey (actEd25519Struct *ws, const actU8 *privateKey, void (* watchdog) (void))
     **
     **  This function imports the private key for EC-D/H.
     **
     ** input:
     ** - ws:           pointer to the work-space structure
     ** - privateKey:   pointer to the private key
     ** - watchdog:     pointer to watchdog reset function
     **
     ** output:
     ** - returns:      actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    extern CRYPTOCV_FUNC(actRETURNCODE) actX25519ImportPrivateKey (
      CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
      CRYPTOCV_P2CONST_PARA( actU8 ) privateKey,
      CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ));


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actX25519GenSecret (actEd25519Struct *ws, actU8 *privateKey, actU8 *publicKey, actU8 *sharedSecret, void (* watchdog) (void))
     **
     **  This function generates the shared secret for EC-D/H.
     **
     ** input:
     ** - ws:           pointer to the work-space structure
     ** - publicKey:    pointer to the public key
     ** - sharedSecret: pointer to memory for the shared secret
     ** - watchdog:     pointer to watchdog reset function
     **
     ** output:
     ** - returns:      actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
    extern CRYPTOCV_FUNC(actRETURNCODE) actX25519GenSecret (
      CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
      CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
      CRYPTOCV_P2VAR_PARA( actU8 ) sharedSecret,
      CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ));


    #ifdef  __cplusplus
        } /* extern "C" */
    #endif

#endif  /* ACTIX25519_H */
