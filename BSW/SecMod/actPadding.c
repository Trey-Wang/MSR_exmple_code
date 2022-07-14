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

#include "actPadding.h"
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
 *  actPaddingInit
 **********************************************************************************************************************/
/*! \brief      Initialize block padding
 *  \param[in]  mode  Requested block padding mode
 *  \return     Result of operation
 *  \retval     actOK  Mode supported
 *  \retval     actEXCEPTION_ALGOID  Mode not supported
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actPaddingInit( actU8 mode )
{
   actRETURNCODE result;

   /* Check padding mode */
   switch (mode)
   {
      case actPADDING_PM_OFF:
      case actPADDING_PM_ONEWITHZEROES:
      case actPADDING_PM_PKCS7:
      {
         result = actOK;

         break;
      }
      default:
      {
         result = actEXCEPTION_ALGOID;

         break;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  actPaddingPad
 **********************************************************************************************************************/
/*! \brief      Pad a block using the requested padding mode
 *  \param[in]  mode  Requested block padding mode
 *  \param[in]  buffer_size  Total size of block in bytes
 *  \param[in,out]  buffer_used  Input: Size of payload data in buffer, output: Total data size after padding
 *  \param[in,out]  buffer  Buffer to be padded
 *  \return     Result of operation
 *  \retval     actOK  Padding operation successful
 *  \retval     actEXCEPTION_PADDING  Mode not supported
 *  \retval     actEXCEPTION_LENGTH  Insufficient buffer size
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actPaddingPad(
   actU8 mode, int buffer_size, CRYPTOCV_P2VAR_PARA(int) buffer_used,
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer )
{
   actRETURNCODE result;
   int pad;
   int used;

   used = *buffer_used;

   if (buffer_size < used)
   {
      result = actEXCEPTION_LENGTH;
   }
   else
   {
      result   = actOK;
      pad      = buffer_size - used;

      /* Padding mode */
      switch (mode)
      {
         case actPADDING_PM_OFF:
         {
            if (used != 0u)
            {
               result = actEXCEPTION_LENGTH;
            }

            pad = 0u;

            break;
         }
         case actPADDING_PM_ONEWITHZEROES:
         {
            actMemset(&buffer[used], 0x00u, pad);
            buffer[used] = 0x80u;

            break;
         }
         case actPADDING_PM_PKCS7:
         {
            actMemset(&buffer[used], (actU8)(pad & 0xFFu), pad);

            break;
         }
         default:
         {
            result = actEXCEPTION_PADDING;

            break;
         }
      }

      *buffer_used = used + pad;
   }

   return result;
}

/***********************************************************************************************************************
 *  actPaddingUnpad
 **********************************************************************************************************************/
/*! \brief      Unpad a block using the requested padding mode
 *  \param[in]  mode  Requested block padding mode
 *  \param[in]  buffer_size  Total size of block in bytes
 *  \param[out] buffer_used  Size of payload data in buffer
 *  \param[in]  buffer  Buffer to be unpadded
 *  \return     Result of operation
 *  \retval     actOK  Valid padding found
 *  \retval     actEXCEPTION_PADDING  Mode not supported or invalid padding
 **********************************************************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actPaddingUnpad(
   actU8 mode, int buffer_size, CRYPTOCV_P2VAR_PARA(int) buffer_used,
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer )
{
   actRETURNCODE result;
   actU8 pad;
   int unpad;
   int index;

   result   = actOK;
   unpad    = 0u;

   /* Padding mode */
   switch (mode)
   {
      case actPADDING_PM_OFF:
      {
         unpad = buffer_size;

         break;
      }
      case actPADDING_PM_ONEWITHZEROES:
      {
         for (index = (buffer_size - 1u); index > 0u; index--)
         {
            if (buffer[index] != 0x00u)
            {
               break;
            }
         }

         if (buffer[index] != 0x80u)
         {
            result = actEXCEPTION_PADDING;
         }
         else
         {
            unpad = index;
         }

         break;
      }
      case actPADDING_PM_PKCS7:
      {
         /* PKCS #7 unpad of the block */
         pad   = buffer[buffer_size - 1u];

         if (pad > buffer_size)
         {
            result = actEXCEPTION_PADDING;
         }
         else
         {
            unpad = buffer_size - pad;

            for (index = unpad; index < buffer_size; index++)
            {
               if (buffer[index] != pad)
               {
                  result = actEXCEPTION_PADDING;
               }
            }
         }

         break;
      }
      default:
      {
         result = actEXCEPTION_PADDING;

         break;
      }
   }

   if (result == actOK)
   {
      *buffer_used = unpad;
   }

   return result;
}

/***********************************************************************************************************************
 *  END OF FILE: ACTPADDING.C
 **********************************************************************************************************************/
