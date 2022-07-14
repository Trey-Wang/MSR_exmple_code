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

#include "actISipHash.h"
#include "actUtilities.h"
#include "actWatchdog.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
STATIC CRYPTOCV_FUNC(void) actSipHashBlockUpdate(
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   actU8 rounds );

STATIC CRYPTOCV_FUNC(void) actSipHashRound(
   CRYPTOCV_P2VAR_PARA(actU64) pv0,
   CRYPTOCV_P2VAR_PARA(actU64) pv1,
   CRYPTOCV_P2VAR_PARA(actU64) pv2,
   CRYPTOCV_P2VAR_PARA(actU64) pv3,
   actU8 rounds );


/***********************************************************************************************************************
 *  actSipHashBlockUpdate
 **********************************************************************************************************************/
/*! \brief         Updates SipHash calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Data block to be processed
 *  \param[in]     rounds  Round count
 **********************************************************************************************************************/
STATIC CRYPTOCV_FUNC(void) actSipHashBlockUpdate(
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   actU8 rounds )
{
   actU64 m;

   m = actU8toU64(dataIN);
   info->v3 ^= m;

   actSipHashRound( &info->v0, &info->v1, &info->v2, &info->v3, rounds);

   info->v0 ^= m;
}

/***********************************************************************************************************************
 *  actSipHashRound
 **********************************************************************************************************************/
/*! \brief         Updates SipHash calculation
 *  \param[in]     pv0  Pointer to v0
 *  \param[in]     pv1  Pointer to v1
 *  \param[in]     pv2  Pointer to v2
 *  \param[in]     pv3  Pointer to v3
 *  \param[in]     rounds  Round count
 **********************************************************************************************************************/
STATIC CRYPTOCV_FUNC(void) actSipHashRound(
   CRYPTOCV_P2VAR_PARA(actU64) pv0,
   CRYPTOCV_P2VAR_PARA(actU64) pv1,
   CRYPTOCV_P2VAR_PARA(actU64) pv2,
   CRYPTOCV_P2VAR_PARA(actU64) pv3,
   actU8 rounds )
{
  actU8 i;

  for (i = 0; i < rounds; i++)
  {
    *pv0 += *pv1;
    *pv1 = actRotl64(*pv1, 13);
    *pv1 ^= *pv0;
    *pv0 = actRotl64(*pv0, 32);
    *pv2 += *pv3;
    *pv3 = actRotl64(*pv3, 16);
    *pv3 ^= *pv2;
    *pv0 += *pv3;
    *pv3 = actRotl64(*pv3, 21);
    *pv3 ^= *pv0;
    *pv2 += *pv1;
    *pv1 = actRotl64(*pv1, 17);
    *pv1 ^= *pv2;
    *pv2 = actRotl64(*pv2, 32);
  }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSipHashInit
 **********************************************************************************************************************/
/*! \brief         Initialize SipHash calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     key  SipHash key to be used
 *  \param[in]     key_length  Size of AES key to be used
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_NULL  Key is NULL
 *  \retval        actEXCEPTION_LENGTH  Invalid key length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSipHashInit (
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;
   actU64 k[2];

   if (!key)
   {
      result = actEXCEPTION_NULL;
   }
   else if (key_length != 16u)
   {
      result = actEXCEPTION_LENGTH;
   }
   else
   {
      k[0] = actU8toU64(key);
      k[1] = actU8toU64(&key[8]);

      actL2trigger(watchdog);

      /* Initialize internal states with the SipHash constants and key k */
      info->v0 = 0x736f6d6570736575ull ^ k[0];
      info->v1 = 0x646f72616e646f6dull ^ k[1];
      info->v2 = 0x6c7967656e657261ull ^ k[0];
      info->v3 = 0x7465646279746573ull ^ k[1];

      /* Initialize variables */
      info->buffer = 0u;
      info->buffer_used = 0u;
      info->length = 0u;

      result = actOK;
   }
   return result;
}

/***********************************************************************************************************************
 *  actSipHashUpdate
 **********************************************************************************************************************/
/*! \brief         Update SipHash calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_NULL  Null pointer exception
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSipHashUpdate (
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result = actEXCEPTION_UNKNOWN;
   actU32 diff = 0u;
   actU8 blocks = 0u;
   actU8 bcount = 0u;
   CRYPTOCV_P2VAR( CRYPTOCV_NONE, actU8 ) bufferPtr;

   if ( !dataIN )
   {
      result = actEXCEPTION_NULL;
   }
   else
   {
      /* Update length */
      info->length = info->length + (length % 256u);

      /* Process left overs of last update */
      if ( info->buffer_used > 0u )
      {
         bufferPtr = (CRYPTOCV_P2VAR( CRYPTOCV_NONE, actU8 ))&info->buffer;
         diff = actMin((actU32)length, actSIPHASH_BLOCK_SIZE - info->buffer_used);
         /* Fill up buffer */
         actMemcpy(&bufferPtr[info->buffer_used],
                   (CRYPTOCV_P2VAR_PARA( void ))dataIN,
                   diff);
         info->buffer_used += (actU8)diff;

         if ( info->buffer_used < actSIPHASH_BLOCK_SIZE )
         {
            /* no complete block */
            result = actOK;
         }
         else
         {
            /* Process block and set buffer to zero */
            actSipHashBlockUpdate(info, (CRYPTOCV_P2VAR( CRYPTOCV_NONE, actU8 ))&info->buffer, ACT_SIPHASH_CROUNDS);
            actL2trigger(watchdog);
            info->buffer_used = 0u;
         }
      }

      if ( result != actOK )
      {
         blocks = (actU8)((length - diff) / actSIPHASH_BLOCK_SIZE);
         dataIN = &dataIN[(actU8)diff];
         /* Loop over data blocks */
         for (bcount = 0; bcount < blocks; ++bcount, dataIN = &dataIN[actSIPHASH_BLOCK_SIZE])
         {
           /* Process block */
           actL1trigger(watchdog);
           actSipHashBlockUpdate(info, dataIN, ACT_SIPHASH_CROUNDS);
         }

         /* Copy left overs into buffer */
         info->buffer_used = (actU8)(length - (diff + actSIPHASH_BLOCK_SIZE * blocks));
         actMemcpy(&info->buffer,
                   (CRYPTOCV_P2VAR_PARA( void ))dataIN,
                   info->buffer_used);
         result = actOK;
      }
   }
   return result;
}

/***********************************************************************************************************************
 *  actSipHashFinalize
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[out]    mac  Pointer to computed MAC
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSipHashFinalize(
   CRYPTOCV_P2VAR_PARA( actSipHashSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU64 ) mac,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR( CRYPTOCV_NONE, actU8 ) bufferPtr = (CRYPTOCV_P2VAR( CRYPTOCV_NONE, actU8 ))&info->buffer;
   actU8 i;

   /* Handle remainders */
   for ( i = info->buffer_used; i < actSIPHASH_BLOCK_SIZE; i++ )
   {
      bufferPtr[i] = 0u;
   }

   /* Write length information */
   bufferPtr[sizeof(actU64) - 1] = (actU8)info->length;

   /* Process remainings */
   actSipHashBlockUpdate(info,
                         (CRYPTOCV_P2CONST_PARA( actU8 ))&info->buffer,
                         ACT_SIPHASH_CROUNDS);
   info->buffer_used = 0u;
   actL2trigger(watchdog);

   /* XOR the constant */
   info->v2 ^= 0xFF;

   /* Perform dRounds */
   actSipHashRound(&info->v0, &info->v1, &info->v2, &info->v3, ACT_SIPHASH_DROUNDS);
   actL1trigger(watchdog);

   /* Return result */
   *mac = info->v0 ^ info->v1  ^ info->v2^  info->v3;

   return actOK;
}

/***********************************************************************************************************************
 *  END OF FILE: ACTSIPHASH.C
 **********************************************************************************************************************/
