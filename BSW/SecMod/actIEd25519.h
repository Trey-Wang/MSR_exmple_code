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
 ** Ed25519 internal programming interface
 **
 ** constants:
 **
 ** types:
 **   actEd25519Struct
 **
 ** macros:
 **
 ** functions:
 **   actEd25519Init
 **   actEd25519Update
 **   actEd25519Sign
 **   actEd25519Verify
 **   actEd25519genKeyPair
 **
 ***************************************************************************/

#ifndef     ACTIED25519_H
    #define ACTIED25519_H

    #include "actITypes.h"
    #include "actISHA2_64.h"


    /****************************************************************************
     ** Types and constants
     ***************************************************************************/

    #define   actEd25519_KEY_SIZE   32
    #define   actEd25519pure         0
    #define   actEd25519ctx          1
    #define   actEd25519ph           2

    /* Workspace structure for Ed25519 */
    typedef struct {
        actU32                        instance;                           /*  algorithm instance  (plain, context, pre-hash)  */
        actSHA512STRUCT               sha512;                             /*  SHA-512         - used for pre-hash and Ed25519  */
        actU8                         hashValue  [actHASH_SIZE_SHA512];   /*  hashed message  - used for pre-hash  */
        actU8                         privateKey [actHASH_SIZE_SHA512];   /*  private scalar & prefix  */
        actU8                         publicKey  [actEd25519_KEY_SIZE];   /*  public key  */
        CRYPTOCV_P2CONST_PARA(actU8)  message;                            /*  input to signature (probably hashValue)  */
        actU32                        msgLen;                             /*  length of message  */
        CRYPTOCV_P2CONST_PARA(actU8)  context;                            /*  context  */ 
        actU32                        ctxLen;                             /*  length of context  */

        actU8                         coreWS     [520];                   /*  workspace for calculation core (layout depends on core used)  */
    } actEd25519Struct;


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
     ** actRETURNCODE actEd25519Init (actEd25519Struct *ws, const actU8 instance, const actU8 *context, const int ctxLen)
     **
     **  This function initializes the Ed25519 algorithm.
     **
     ** input:
     ** - ws:           pointer to the work-space structure
     ** - instance:     Ed25519 instance to use; one of:
     **                 actEd25519pure
     **                 actEd25519ctx
     **                 actEd25519ph
     ** - context:      pointer to the context data
     ** - ctxLen:       length of the context data
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
     extern CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Init (
        CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
        const actU8 instance,
		CRYPTOCV_P2CONST_PARA( actU8 ) context,
		const int ctxLen);


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actEd25519Update (actEd25519Struct *ws, const actU8 *message, const int msgLen, void (* watchdog) (void))
     **
     **  This function updates the internal hash, if pre-hash is used.
     **
     ** input:
     ** - ws:         pointer to the work-space structure
     ** - message:    pointer to the input data
     ** - msgLen:     length of the input data
     ** - watchdog:   pointer to watchdog reset function
     **
     ** output:
     ** - returns:    actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
     extern CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Update (
        CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
        CRYPTOCV_P2CONST_PARA( actU8 ) message,
        const int msgLen,
        CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actEd25519Sign (actEd25519Struct *ws, const actU8 *message, const int msgLen, const actU8 *secretKey, const actU8 *publicKey, actU8 *signature, void (* watchdog) (void))
     **
     **  This function creates a signature over the data.
     **  When pre-hash is used, the data are hashed first. The final hash value is then signed.
     **
     ** input:
     ** - ws:         pointer to the work-space structure
     ** - message:    pointer to the input data
     ** - msgLen:     length of the input data
     ** - secretKey:  pointer to the secret key used to sign data (32 bytes)
     ** - publicKey:  pointer to the public key used to sign data (32 bytes - optional!)
     ** - signature:  pointer to the signature buffer (64 bytes)
     ** - watchdog:   pointer to watchdog reset function
     **
     ** output:
     ** - signature:  signature
     ** - returns:    actOK always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
     extern CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Sign (
        CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
        CRYPTOCV_P2CONST_PARA( actU8 ) message,
        const int msgLen,
        CRYPTOCV_P2CONST_PARA( actU8 ) secretKey,
        CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
        CRYPTOCV_P2VAR_PARA( actU8 ) signature,
        CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actEd25519Verify (actEd25519Struct *ws, const actU8 *message, const int msgLen, const actU8 *publicKey, const actU8 *signature, void (* watchdog) (void))
     **
     **  This function verifies the signature over the data.
     **  When pre-hash is used, the data are hashed first. The signature is then verified against the final hash value.
     **
     ** input:
     ** - ws:         pointer to the work-space structure
     ** - message:    pointer to the input data
     ** - msgLen:     length of the input data
     ** - publicKey:  pointer to the key used to verify signature (32 bytes)
     ** - signature:  pointer to the signature (64 bytes)
     ** - watchdog:   pointer to watchdog reset function
     **
     ** output:
     ** - returns:    actOK                 - signature is valid
     **               actSIGNATURE_INVALID  - signature is INvalid
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
     extern CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Verify (
        CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
        CRYPTOCV_P2CONST_PARA( actU8 ) message,
        const int msgLen,
        CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
        CRYPTOCV_P2CONST_PARA( actU8 ) signature,
        CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


    /****************************************************************************
     **
     ** FUNCTION:
     **
     ** actRETURNCODE actEd25519GenKeyPair (actEd25519Struct *ws, actU8 *secretKey, actU8 *publicKey, void (* watchdog) (void))
     **
     **  This function verifies the signature over the data.
     **  When pre-hash is used, the data are hashed first. The signature is then verified against the final hash value.
     **
     ** input:
     ** - ws:         pointer to the work-space structure
     ** - secretKey:  pointer to the secret key buffer (32 bytes)
     ** - publicKey:  pointer to the public key buffer (32 bytes)
     ** - watchdog:   pointer to watchdog reset function
     **
     ** output:
     ** - returns:    actOK   - always
     **
     ** assumes:
     **
     ** uses:
     **
     ***************************************************************************/
     extern CRYPTOCV_FUNC( actRETURNCODE ) actEd25519GenKeyPair (
        CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
        CRYPTOCV_P2VAR_PARA( actU8 ) secretKey,
        CRYPTOCV_P2VAR_PARA( actU8 ) publicKey,
        CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    #ifdef  __cplusplus
        } /* extern "C" */
    #endif

#endif  /* ACTIED25519_H */