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

#include "actICMACAES.h"
#include "actUtilities.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/** Constant for sub-key generation */
#define CMAC_CONST_RB      0x87u
/** Most significant bit */
#define CMAC_MSB           0x80u

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actCMACAESProcess
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation
 *  \details       Helper function used by actual update with new data and finalization with sub-key
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     final  Finalization trigger
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_LENGTH  Invalid input length
 **********************************************************************************************************************/
static CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESProcess (
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   int final, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;
   int used;
   int xorLength;
   int offset;

   if (length == 0)
   {
      result = actEXCEPTION_LENGTH;
   }
   else
   {
      result   = actOK;
      offset   = 0;

      if (final == 0)
      {
         used = info->buffer_used;
      }
      else
      {
         used = 0;
      }

      while ((length > 0) || (final != 0))
      {
         if (used == actAES_BLOCK_SIZE)
         {
            actAESEncryptBlock(info, info->buffer, info->buffer, watchdog);
            used  = 0;
            final = 0;
         }
         else
         {
            xorLength = actAES_BLOCK_SIZE - used;
            if (xorLength > length)
            {
               xorLength = length;
            }

            actXOR(&info->buffer[used], &dataIN[offset], xorLength);

            used     += xorLength;
            offset   += xorLength;
            length   -= xorLength;
         }
      }

      info->buffer_used = used;
   }

   return result;
}

/***********************************************************************************************************************
 *  actCMACAESGenerateSubKey
 **********************************************************************************************************************/
/*! \brief         Generate sub-key used during finalization
 *  \param[in]     info  Pointer to context structure
 *  \param[out]    subKey  Pointer to sub-key buffer
 *  \param[in]     watchdog  Watchdog trigger function
 **********************************************************************************************************************/
static CRYPTOCV_FUNC( void ) actCMACAESGenerateSubKey (
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) subKey,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actU8 msb;
   int index;
   int keyIndex;
   int keyCount;

   actMemset(subKey, 0x00u, actAES_BLOCK_SIZE);
   actAESEncryptBlock(info, subKey, subKey, watchdog);

   if (info->buffer_used == actAES_BLOCK_SIZE)
   {
      keyCount = 1;
   }
   else
   {
      keyCount = 2;
   }

   for (keyIndex = 0; keyIndex < keyCount; keyIndex++)
   {
      msb = subKey[0] & CMAC_MSB;

      for (index = 0; index < (actAES_BLOCK_SIZE - 1); index++)
      {
         subKey[index] = (actU8)(((subKey[index] << 1) | (subKey[index + 1] >> 7)) & 0xFFu);
      }

      subKey[index] = (actU8)((subKey[index] << 1) & 0xFFu);

      if (msb != 0)
      {
         subKey[index] ^= CMAC_CONST_RB;
      }
   }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actCMACAESInit
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     key  AES key to be used
 *  \param[in]     key_length  Size of AES key to be used
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_NULL  Key is NULL
 *  \retval        actEXCEPTION_LENGTH  Invalid key length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESInit (
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;

   result = actAESInitEncryptBlock(info, key, key_length, watchdog);

   actMemset(info->buffer, 0x00u, actAES_BLOCK_SIZE);
   info->buffer_used = 0;

   return result;
}

/***********************************************************************************************************************
 *  actCMACAESUpdate
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_LENGTH  Invalid input length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESUpdate (
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actCMACAESProcess(info, dataIN, length, 0, watchdog);
}

/***********************************************************************************************************************
 *  actCMACAESFinalize
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC calculation
 *  \param[in]     info  Pointer to context structure
 *  \param[out]    mac  Pointer to computed CMAC
 *  \param[in]     watchdog  Watchdog trigger function
 *  \retval        actOK  Operation successful
 *  \retval        actEXCEPTION_LENGTH  Invalid input length
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actCMACAESFinalize (
   CRYPTOCV_P2VAR_PARA( actCMACAESSTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) mac,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;
   CRYPTOCV_P2VAR_PARA( actU8 ) subKey;
   int used;

   /* Use previous block buffer for sub-key generation */
   subKey   = info->prev_block;
   used     = info->buffer_used;

   actCMACAESGenerateSubKey(info, subKey, watchdog);

   if (used < actAES_BLOCK_SIZE)
   {
      subKey[used] ^= CMAC_MSB;
   }

   result = actCMACAESProcess(info, subKey, actAES_BLOCK_SIZE, 1, watchdog);

   if (result == actOK)
   {
      actMemcpy(mac, info->buffer, actAES_BLOCK_SIZE);
   }

   return result;
}

/***********************************************************************************************************************
 *  END OF FILE: ACTCMACAES.C
 **********************************************************************************************************************/
