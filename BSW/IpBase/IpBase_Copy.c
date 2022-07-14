/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  IpBase_Copy.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Copy implementation
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define IPBASE_COPY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
#include "IpBase.h"
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != 3u) || (IPBASE_SW_MINOR_VERSION != 0u) || (IPBASE_SW_PATCH_VERSION != 0u) )
# error "IpBase_Copy.c: Source and Header file are inconsistent!"
#endif

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if ( IPBASE_COPY_ENABLE == STD_ON )
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpBase_Copy
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_COPY_MACRO_ENABLE == STD_ON )
# else
IPBASE_FUNCCODE(void) IpBase_Copy(
    IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
    IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType) SrcDataPtr,
    uint32 LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32_least    idx; /* PRQA S 0781 */ /* MD_IpBase_5.6_781 */

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check TgtDataPtr, SrcDataPtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* PRQA S 0310,3305 2 */ /* MD_IpBase_310 */
    IPBASE_P2VARAPPLDATA(uint32) u32TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint32)) TgtDataPtr;
    IPBASE_P2CONSTAPPLDATA(uint32) u32SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint32)) SrcDataPtr;

    /* ----- Implementation ----------------------------------------------- */
    if ( (((uint32)(TgtDataPtr)%4) == 0) && (((uint32)(SrcDataPtr)%4) == 0) ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #20 Copy 4 bytes aligned data */
      idx = LenByte/4;
      while ( idx > 63 )
      { /* #30 Copy 64 32bit words */
        idx-=64;
        u32TgtDataPtr[idx+63U] = u32SrcDataPtr[idx+63U];
        u32TgtDataPtr[idx+62U] = u32SrcDataPtr[idx+62U];
        u32TgtDataPtr[idx+61U] = u32SrcDataPtr[idx+61U];
        u32TgtDataPtr[idx+60U] = u32SrcDataPtr[idx+60U];
        u32TgtDataPtr[idx+59U] = u32SrcDataPtr[idx+59U];
        u32TgtDataPtr[idx+58U] = u32SrcDataPtr[idx+58U];
        u32TgtDataPtr[idx+57U] = u32SrcDataPtr[idx+57U];
        u32TgtDataPtr[idx+56U] = u32SrcDataPtr[idx+56U];
        u32TgtDataPtr[idx+55U] = u32SrcDataPtr[idx+55U];
        u32TgtDataPtr[idx+54U] = u32SrcDataPtr[idx+54U];
        u32TgtDataPtr[idx+53U] = u32SrcDataPtr[idx+53U];
        u32TgtDataPtr[idx+52U] = u32SrcDataPtr[idx+52U];
        u32TgtDataPtr[idx+51U] = u32SrcDataPtr[idx+51U];
        u32TgtDataPtr[idx+50U] = u32SrcDataPtr[idx+50U];
        u32TgtDataPtr[idx+49U] = u32SrcDataPtr[idx+49U];
        u32TgtDataPtr[idx+48U] = u32SrcDataPtr[idx+48U];
        u32TgtDataPtr[idx+47U] = u32SrcDataPtr[idx+47U];
        u32TgtDataPtr[idx+46U] = u32SrcDataPtr[idx+46U];
        u32TgtDataPtr[idx+45U] = u32SrcDataPtr[idx+45U];
        u32TgtDataPtr[idx+44U] = u32SrcDataPtr[idx+44U];
        u32TgtDataPtr[idx+43U] = u32SrcDataPtr[idx+43U];
        u32TgtDataPtr[idx+42U] = u32SrcDataPtr[idx+42U];
        u32TgtDataPtr[idx+41U] = u32SrcDataPtr[idx+41U];
        u32TgtDataPtr[idx+40U] = u32SrcDataPtr[idx+40U];
        u32TgtDataPtr[idx+39U] = u32SrcDataPtr[idx+39U];
        u32TgtDataPtr[idx+38U] = u32SrcDataPtr[idx+38U];
        u32TgtDataPtr[idx+37U] = u32SrcDataPtr[idx+37U];
        u32TgtDataPtr[idx+36U] = u32SrcDataPtr[idx+36U];
        u32TgtDataPtr[idx+35U] = u32SrcDataPtr[idx+35U];
        u32TgtDataPtr[idx+34U] = u32SrcDataPtr[idx+34U];
        u32TgtDataPtr[idx+33U] = u32SrcDataPtr[idx+33U];
        u32TgtDataPtr[idx+32U] = u32SrcDataPtr[idx+32U];
        u32TgtDataPtr[idx+31U] = u32SrcDataPtr[idx+31U];
        u32TgtDataPtr[idx+30U] = u32SrcDataPtr[idx+30U];
        u32TgtDataPtr[idx+29U] = u32SrcDataPtr[idx+29U];
        u32TgtDataPtr[idx+28U] = u32SrcDataPtr[idx+28U];
        u32TgtDataPtr[idx+27U] = u32SrcDataPtr[idx+27U];
        u32TgtDataPtr[idx+26U] = u32SrcDataPtr[idx+26U];
        u32TgtDataPtr[idx+25U] = u32SrcDataPtr[idx+25U];
        u32TgtDataPtr[idx+24U] = u32SrcDataPtr[idx+24U];
        u32TgtDataPtr[idx+23U] = u32SrcDataPtr[idx+23U];
        u32TgtDataPtr[idx+22U] = u32SrcDataPtr[idx+22U];
        u32TgtDataPtr[idx+21U] = u32SrcDataPtr[idx+21U];
        u32TgtDataPtr[idx+20U] = u32SrcDataPtr[idx+20U];
        u32TgtDataPtr[idx+19U] = u32SrcDataPtr[idx+19U];
        u32TgtDataPtr[idx+18U] = u32SrcDataPtr[idx+18U];
        u32TgtDataPtr[idx+17U] = u32SrcDataPtr[idx+17U];
        u32TgtDataPtr[idx+16U] = u32SrcDataPtr[idx+16U];
        u32TgtDataPtr[idx+15U] = u32SrcDataPtr[idx+15U];
        u32TgtDataPtr[idx+14U] = u32SrcDataPtr[idx+14U];
        u32TgtDataPtr[idx+13U] = u32SrcDataPtr[idx+13U];
        u32TgtDataPtr[idx+12U] = u32SrcDataPtr[idx+12U];
        u32TgtDataPtr[idx+11U] = u32SrcDataPtr[idx+11U];
        u32TgtDataPtr[idx+10U] = u32SrcDataPtr[idx+10U];
        u32TgtDataPtr[idx+9U]  = u32SrcDataPtr[idx+9U];
        u32TgtDataPtr[idx+8U]  = u32SrcDataPtr[idx+8U];
        u32TgtDataPtr[idx+7U]  = u32SrcDataPtr[idx+7U];
        u32TgtDataPtr[idx+6U]  = u32SrcDataPtr[idx+6U];
        u32TgtDataPtr[idx+5U]  = u32SrcDataPtr[idx+5U];
        u32TgtDataPtr[idx+4U]  = u32SrcDataPtr[idx+4U];
        u32TgtDataPtr[idx+3U]  = u32SrcDataPtr[idx+3U];
        u32TgtDataPtr[idx+2U]  = u32SrcDataPtr[idx+2U];
        u32TgtDataPtr[idx+1U]  = u32SrcDataPtr[idx+1U];
        u32TgtDataPtr[idx]     = u32SrcDataPtr[idx];
      }
      while ( idx > 15 )
      { /* #40 Copy 16 32bit words */
        idx-=16;
        u32TgtDataPtr[idx+15U] = u32SrcDataPtr[idx+15U];
        u32TgtDataPtr[idx+14U] = u32SrcDataPtr[idx+14U];
        u32TgtDataPtr[idx+13U] = u32SrcDataPtr[idx+13U];
        u32TgtDataPtr[idx+12U] = u32SrcDataPtr[idx+12U];
        u32TgtDataPtr[idx+11U] = u32SrcDataPtr[idx+11U];
        u32TgtDataPtr[idx+10U] = u32SrcDataPtr[idx+10U];
        u32TgtDataPtr[idx+9U]  = u32SrcDataPtr[idx+9U];
        u32TgtDataPtr[idx+8U]  = u32SrcDataPtr[idx+8U];
        u32TgtDataPtr[idx+7U]  = u32SrcDataPtr[idx+7U];
        u32TgtDataPtr[idx+6U]  = u32SrcDataPtr[idx+6U];
        u32TgtDataPtr[idx+5U]  = u32SrcDataPtr[idx+5U];
        u32TgtDataPtr[idx+4U]  = u32SrcDataPtr[idx+4U];
        u32TgtDataPtr[idx+3U]  = u32SrcDataPtr[idx+3U];
        u32TgtDataPtr[idx+2U]  = u32SrcDataPtr[idx+2U];
        u32TgtDataPtr[idx+1U]  = u32SrcDataPtr[idx+1U];
        u32TgtDataPtr[idx]     = u32SrcDataPtr[idx];
      }
      while ( idx > 3 )
      { /* #50 Copy 4 32bit words */
        idx-=4;
        u32TgtDataPtr[idx+3U] = u32SrcDataPtr[idx+3U];
        u32TgtDataPtr[idx+2U] = u32SrcDataPtr[idx+2U];
        u32TgtDataPtr[idx+1U] = u32SrcDataPtr[idx+1U];
        u32TgtDataPtr[idx]    = u32SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #60 copy 32bit words */
        idx--;
        u32TgtDataPtr[idx] = u32SrcDataPtr[idx];
      }
      if ( (LenByte % 4) != 0 )
      { /* #70 Copy remaining single bytes */
        /* PRQA S 0310 2 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &u32TgtDataPtr[LenByte>>2];
        IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &u32SrcDataPtr[LenByte>>2];
        idx = LenByte % 4;
        while ( idx > 0 )
        {
          idx--;
          u8TgtDataPtr[idx] = u8SrcDataPtr[idx];
        }
      }
    }
    else if ( (((uint32)(TgtDataPtr)%2) == 0) && (((uint32)(SrcDataPtr)%2) == 0) ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #80 Copy 2 bytes aligned data */
      /* PRQA S 0310,3305 2 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint16)   u16TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint16)) &TgtDataPtr[0];
      IPBASE_P2CONSTAPPLDATA(uint16) u16SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint16)) &SrcDataPtr[0];
      idx = LenByte/2;
      while ( idx > 63 )
      { /* #90 Copy 64 16bit words */
        idx-=64;
        u16TgtDataPtr[idx+63U] = u16SrcDataPtr[idx+63U];
        u16TgtDataPtr[idx+62U] = u16SrcDataPtr[idx+62U];
        u16TgtDataPtr[idx+61U] = u16SrcDataPtr[idx+61U];
        u16TgtDataPtr[idx+60U] = u16SrcDataPtr[idx+60U];
        u16TgtDataPtr[idx+59U] = u16SrcDataPtr[idx+59U];
        u16TgtDataPtr[idx+58U] = u16SrcDataPtr[idx+58U];
        u16TgtDataPtr[idx+57U] = u16SrcDataPtr[idx+57U];
        u16TgtDataPtr[idx+56U] = u16SrcDataPtr[idx+56U];
        u16TgtDataPtr[idx+55U] = u16SrcDataPtr[idx+55U];
        u16TgtDataPtr[idx+54U] = u16SrcDataPtr[idx+54U];
        u16TgtDataPtr[idx+53U] = u16SrcDataPtr[idx+53U];
        u16TgtDataPtr[idx+52U] = u16SrcDataPtr[idx+52U];
        u16TgtDataPtr[idx+51U] = u16SrcDataPtr[idx+51U];
        u16TgtDataPtr[idx+50U] = u16SrcDataPtr[idx+50U];
        u16TgtDataPtr[idx+49U] = u16SrcDataPtr[idx+49U];
        u16TgtDataPtr[idx+48U] = u16SrcDataPtr[idx+48U];
        u16TgtDataPtr[idx+47U] = u16SrcDataPtr[idx+47U];
        u16TgtDataPtr[idx+46U] = u16SrcDataPtr[idx+46U];
        u16TgtDataPtr[idx+45U] = u16SrcDataPtr[idx+45U];
        u16TgtDataPtr[idx+44U] = u16SrcDataPtr[idx+44U];
        u16TgtDataPtr[idx+43U] = u16SrcDataPtr[idx+43U];
        u16TgtDataPtr[idx+42U] = u16SrcDataPtr[idx+42U];
        u16TgtDataPtr[idx+41U] = u16SrcDataPtr[idx+41U];
        u16TgtDataPtr[idx+40U] = u16SrcDataPtr[idx+40U];
        u16TgtDataPtr[idx+39U] = u16SrcDataPtr[idx+39U];
        u16TgtDataPtr[idx+38U] = u16SrcDataPtr[idx+38U];
        u16TgtDataPtr[idx+37U] = u16SrcDataPtr[idx+37U];
        u16TgtDataPtr[idx+36U] = u16SrcDataPtr[idx+36U];
        u16TgtDataPtr[idx+35U] = u16SrcDataPtr[idx+35U];
        u16TgtDataPtr[idx+34U] = u16SrcDataPtr[idx+34U];
        u16TgtDataPtr[idx+33U] = u16SrcDataPtr[idx+33U];
        u16TgtDataPtr[idx+32U] = u16SrcDataPtr[idx+32U];
        u16TgtDataPtr[idx+31U] = u16SrcDataPtr[idx+31U];
        u16TgtDataPtr[idx+30U] = u16SrcDataPtr[idx+30U];
        u16TgtDataPtr[idx+29U] = u16SrcDataPtr[idx+29U];
        u16TgtDataPtr[idx+28U] = u16SrcDataPtr[idx+28U];
        u16TgtDataPtr[idx+27U] = u16SrcDataPtr[idx+27U];
        u16TgtDataPtr[idx+26U] = u16SrcDataPtr[idx+26U];
        u16TgtDataPtr[idx+25U] = u16SrcDataPtr[idx+25U];
        u16TgtDataPtr[idx+24U] = u16SrcDataPtr[idx+24U];
        u16TgtDataPtr[idx+23U] = u16SrcDataPtr[idx+23U];
        u16TgtDataPtr[idx+22U] = u16SrcDataPtr[idx+22U];
        u16TgtDataPtr[idx+21U] = u16SrcDataPtr[idx+21U];
        u16TgtDataPtr[idx+20U] = u16SrcDataPtr[idx+20U];
        u16TgtDataPtr[idx+19U] = u16SrcDataPtr[idx+19U];
        u16TgtDataPtr[idx+18U] = u16SrcDataPtr[idx+18U];
        u16TgtDataPtr[idx+17U] = u16SrcDataPtr[idx+17U];
        u16TgtDataPtr[idx+16U] = u16SrcDataPtr[idx+16U];
        u16TgtDataPtr[idx+15U] = u16SrcDataPtr[idx+15U];
        u16TgtDataPtr[idx+14U] = u16SrcDataPtr[idx+14U];
        u16TgtDataPtr[idx+13U] = u16SrcDataPtr[idx+13U];
        u16TgtDataPtr[idx+12U] = u16SrcDataPtr[idx+12U];
        u16TgtDataPtr[idx+11U] = u16SrcDataPtr[idx+11U];
        u16TgtDataPtr[idx+10U] = u16SrcDataPtr[idx+10U];
        u16TgtDataPtr[idx+9U]  = u16SrcDataPtr[idx+9U];
        u16TgtDataPtr[idx+8U]  = u16SrcDataPtr[idx+8U];
        u16TgtDataPtr[idx+7U]  = u16SrcDataPtr[idx+7U];
        u16TgtDataPtr[idx+6U]  = u16SrcDataPtr[idx+6U];
        u16TgtDataPtr[idx+5U]  = u16SrcDataPtr[idx+5U];
        u16TgtDataPtr[idx+4U]  = u16SrcDataPtr[idx+4U];
        u16TgtDataPtr[idx+3U]  = u16SrcDataPtr[idx+3U];
        u16TgtDataPtr[idx+2U]  = u16SrcDataPtr[idx+2U];
        u16TgtDataPtr[idx+1U]  = u16SrcDataPtr[idx+1U];
        u16TgtDataPtr[idx]     = u16SrcDataPtr[idx];
      }
      while ( idx > 15 )
      { /* #100 Copy 16 16bit words */
        idx-=16;
        u16TgtDataPtr[idx+15U] = u16SrcDataPtr[idx+15U];
        u16TgtDataPtr[idx+14U] = u16SrcDataPtr[idx+14U];
        u16TgtDataPtr[idx+13U] = u16SrcDataPtr[idx+13U];
        u16TgtDataPtr[idx+12U] = u16SrcDataPtr[idx+12U];
        u16TgtDataPtr[idx+11U] = u16SrcDataPtr[idx+11U];
        u16TgtDataPtr[idx+10U] = u16SrcDataPtr[idx+10U];
        u16TgtDataPtr[idx+9U]  = u16SrcDataPtr[idx+9U];
        u16TgtDataPtr[idx+8U]  = u16SrcDataPtr[idx+8U];
        u16TgtDataPtr[idx+7U]  = u16SrcDataPtr[idx+7U];
        u16TgtDataPtr[idx+6U]  = u16SrcDataPtr[idx+6U];
        u16TgtDataPtr[idx+5U]  = u16SrcDataPtr[idx+5U];
        u16TgtDataPtr[idx+4U]  = u16SrcDataPtr[idx+4U];
        u16TgtDataPtr[idx+3U]  = u16SrcDataPtr[idx+3U];
        u16TgtDataPtr[idx+2U]  = u16SrcDataPtr[idx+2U];
        u16TgtDataPtr[idx+1U]  = u16SrcDataPtr[idx+1U];
        u16TgtDataPtr[idx]     = u16SrcDataPtr[idx];
      }
      while ( idx > 3 )
      { /* #110 Copy 4 16bit words */
        idx-=4;
        u16TgtDataPtr[idx+3U] = u16SrcDataPtr[idx+3U];
        u16TgtDataPtr[idx+2U] = u16SrcDataPtr[idx+2U];
        u16TgtDataPtr[idx+1U] = u16SrcDataPtr[idx+1U];
        u16TgtDataPtr[idx]    = u16SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #120 Copy 16bit words */
        idx--;
        u16TgtDataPtr[idx] = u16SrcDataPtr[idx];
      }
      if ( (LenByte % 2) != 0 )
      { /* #130 Copy remaining single byte */
        /* PRQA S 0310 2 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8) u8TgtDataPtr   = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
        IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &SrcDataPtr[0];
        u8TgtDataPtr[LenByte-1] = u8SrcDataPtr[LenByte-1];
      }
    }
    else
    { /* #140 Copy 1 byte aligned data */
      /* PRQA S 0310 2 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
      IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &SrcDataPtr[0];
      idx = LenByte;
      while ( idx > 63 )
      { /* #150 Copy 64 8bit words */
        idx-=64;
        u8TgtDataPtr[idx+63U] = u8SrcDataPtr[idx+63U];
        u8TgtDataPtr[idx+62U] = u8SrcDataPtr[idx+62U];
        u8TgtDataPtr[idx+61U] = u8SrcDataPtr[idx+61U];
        u8TgtDataPtr[idx+60U] = u8SrcDataPtr[idx+60U];
        u8TgtDataPtr[idx+59U] = u8SrcDataPtr[idx+59U];
        u8TgtDataPtr[idx+58U] = u8SrcDataPtr[idx+58U];
        u8TgtDataPtr[idx+57U] = u8SrcDataPtr[idx+57U];
        u8TgtDataPtr[idx+56U] = u8SrcDataPtr[idx+56U];
        u8TgtDataPtr[idx+55U] = u8SrcDataPtr[idx+55U];
        u8TgtDataPtr[idx+54U] = u8SrcDataPtr[idx+54U];
        u8TgtDataPtr[idx+53U] = u8SrcDataPtr[idx+53U];
        u8TgtDataPtr[idx+52U] = u8SrcDataPtr[idx+52U];
        u8TgtDataPtr[idx+51U] = u8SrcDataPtr[idx+51U];
        u8TgtDataPtr[idx+50U] = u8SrcDataPtr[idx+50U];
        u8TgtDataPtr[idx+49U] = u8SrcDataPtr[idx+49U];
        u8TgtDataPtr[idx+48U] = u8SrcDataPtr[idx+48U];
        u8TgtDataPtr[idx+47U] = u8SrcDataPtr[idx+47U];
        u8TgtDataPtr[idx+46U] = u8SrcDataPtr[idx+46U];
        u8TgtDataPtr[idx+45U] = u8SrcDataPtr[idx+45U];
        u8TgtDataPtr[idx+44U] = u8SrcDataPtr[idx+44U];
        u8TgtDataPtr[idx+43U] = u8SrcDataPtr[idx+43U];
        u8TgtDataPtr[idx+42U] = u8SrcDataPtr[idx+42U];
        u8TgtDataPtr[idx+41U] = u8SrcDataPtr[idx+41U];
        u8TgtDataPtr[idx+40U] = u8SrcDataPtr[idx+40U];
        u8TgtDataPtr[idx+39U] = u8SrcDataPtr[idx+39U];
        u8TgtDataPtr[idx+38U] = u8SrcDataPtr[idx+38U];
        u8TgtDataPtr[idx+37U] = u8SrcDataPtr[idx+37U];
        u8TgtDataPtr[idx+36U] = u8SrcDataPtr[idx+36U];
        u8TgtDataPtr[idx+35U] = u8SrcDataPtr[idx+35U];
        u8TgtDataPtr[idx+34U] = u8SrcDataPtr[idx+34U];
        u8TgtDataPtr[idx+33U] = u8SrcDataPtr[idx+33U];
        u8TgtDataPtr[idx+32U] = u8SrcDataPtr[idx+32U];
        u8TgtDataPtr[idx+31U] = u8SrcDataPtr[idx+31U];
        u8TgtDataPtr[idx+30U] = u8SrcDataPtr[idx+30U];
        u8TgtDataPtr[idx+29U] = u8SrcDataPtr[idx+29U];
        u8TgtDataPtr[idx+28U] = u8SrcDataPtr[idx+28U];
        u8TgtDataPtr[idx+27U] = u8SrcDataPtr[idx+27U];
        u8TgtDataPtr[idx+26U] = u8SrcDataPtr[idx+26U];
        u8TgtDataPtr[idx+25U] = u8SrcDataPtr[idx+25U];
        u8TgtDataPtr[idx+24U] = u8SrcDataPtr[idx+24U];
        u8TgtDataPtr[idx+23U] = u8SrcDataPtr[idx+23U];
        u8TgtDataPtr[idx+22U] = u8SrcDataPtr[idx+22U];
        u8TgtDataPtr[idx+21U] = u8SrcDataPtr[idx+21U];
        u8TgtDataPtr[idx+20U] = u8SrcDataPtr[idx+20U];
        u8TgtDataPtr[idx+19U] = u8SrcDataPtr[idx+19U];
        u8TgtDataPtr[idx+18U] = u8SrcDataPtr[idx+18U];
        u8TgtDataPtr[idx+17U] = u8SrcDataPtr[idx+17U];
        u8TgtDataPtr[idx+16U] = u8SrcDataPtr[idx+16U];
        u8TgtDataPtr[idx+15U] = u8SrcDataPtr[idx+15U];
        u8TgtDataPtr[idx+14U] = u8SrcDataPtr[idx+14U];
        u8TgtDataPtr[idx+13U] = u8SrcDataPtr[idx+13U];
        u8TgtDataPtr[idx+12U] = u8SrcDataPtr[idx+12U];
        u8TgtDataPtr[idx+11U] = u8SrcDataPtr[idx+11U];
        u8TgtDataPtr[idx+10U] = u8SrcDataPtr[idx+10U];
        u8TgtDataPtr[idx+9U]  = u8SrcDataPtr[idx+9U];
        u8TgtDataPtr[idx+8U]  = u8SrcDataPtr[idx+8U];
        u8TgtDataPtr[idx+7U]  = u8SrcDataPtr[idx+7U];
        u8TgtDataPtr[idx+6U]  = u8SrcDataPtr[idx+6U];
        u8TgtDataPtr[idx+5U]  = u8SrcDataPtr[idx+5U];
        u8TgtDataPtr[idx+4U]  = u8SrcDataPtr[idx+4U];
        u8TgtDataPtr[idx+3U]  = u8SrcDataPtr[idx+3U];
        u8TgtDataPtr[idx+2U]  = u8SrcDataPtr[idx+2U];
        u8TgtDataPtr[idx+1U]  = u8SrcDataPtr[idx+1U];
        u8TgtDataPtr[idx]     = u8SrcDataPtr[idx];
      }
      while ( idx > 15 )
      { /* #160 Copy 16 8bit words */
        idx-=16;
        u8TgtDataPtr[idx+15U] = u8SrcDataPtr[idx+15U];
        u8TgtDataPtr[idx+14U] = u8SrcDataPtr[idx+14U];
        u8TgtDataPtr[idx+13U] = u8SrcDataPtr[idx+13U];
        u8TgtDataPtr[idx+12U] = u8SrcDataPtr[idx+12U];
        u8TgtDataPtr[idx+11U] = u8SrcDataPtr[idx+11U];
        u8TgtDataPtr[idx+10U] = u8SrcDataPtr[idx+10U];
        u8TgtDataPtr[idx+9U]  = u8SrcDataPtr[idx+9U];
        u8TgtDataPtr[idx+8U]  = u8SrcDataPtr[idx+8U];
        u8TgtDataPtr[idx+7U]  = u8SrcDataPtr[idx+7U];
        u8TgtDataPtr[idx+6U]  = u8SrcDataPtr[idx+6U];
        u8TgtDataPtr[idx+5U]  = u8SrcDataPtr[idx+5U];
        u8TgtDataPtr[idx+4U]  = u8SrcDataPtr[idx+4U];
        u8TgtDataPtr[idx+3U]  = u8SrcDataPtr[idx+3U];
        u8TgtDataPtr[idx+2U]  = u8SrcDataPtr[idx+2U];
        u8TgtDataPtr[idx+1U]  = u8SrcDataPtr[idx+1U];
        u8TgtDataPtr[idx]     = u8SrcDataPtr[idx];
      }
      while ( idx > 3 )
      { /* #170 Copy 4 8bit words */
        idx-=4;
        u8TgtDataPtr[idx+3U] = u8SrcDataPtr[idx+3U];
        u8TgtDataPtr[idx+2U] = u8SrcDataPtr[idx+2U];
        u8TgtDataPtr[idx+1U] = u8SrcDataPtr[idx+1U];
        u8TgtDataPtr[idx]    = u8SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #180 Copy 8bit words */
        idx--;
        u8TgtDataPtr[idx] = u8SrcDataPtr[idx];
      }
    }
  }
  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #190 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

} /* IpBase_CopyData() */ /* PRQA S 6010,6030,6060 */ /* MD_MSR_STPTH MD_MSR_STCYC MD_MSR_STPAR */
# endif
  /* IPBASE_COPY_MACRO_ENABLE */
/**********************************************************************************************************************
 *  IpBase_CopySmallData
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_COPY_MACRO_ENABLE == STD_ON )
# else
IPBASE_FUNCCODE(void) IpBase_CopySmallData(
    IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
    IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType) SrcDataPtr,
    uint32 LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32_least    idx; /* PRQA S 0781 */ /* MD_IpBase_5.6_781 */

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check TgtDataPtr, SrcDataPtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* PRQA S 0310,3305 2 */ /* MD_IpBase_310 */
    IPBASE_P2VARAPPLDATA(uint32) u32TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint32)) TgtDataPtr;
    IPBASE_P2CONSTAPPLDATA(uint32) u32SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint32)) SrcDataPtr;
    /* ----- Implementation ----------------------------------------------- */
    if ( (((uint32)(TgtDataPtr)%4) == 0) && (((uint32)(SrcDataPtr)%4) == 0) ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #20 Copy 4 bytes aligned data */
      idx = LenByte/4;
      while ( idx > 3 )
      { /* #30 Copy 4 32bit words */
        idx-=4;
        u32TgtDataPtr[idx+3U] = u32SrcDataPtr[idx+3U];
        u32TgtDataPtr[idx+2U] = u32SrcDataPtr[idx+2U];
        u32TgtDataPtr[idx+1U] = u32SrcDataPtr[idx+1U];
        u32TgtDataPtr[idx]    = u32SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #40 Copy 32bit words */
        idx--;
        u32TgtDataPtr[idx] = u32SrcDataPtr[idx];
      }
      if ( (LenByte % 4) != 0 )
      { /* #50 Copy remaining single bytes */
        /* PRQA S 0310 2 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &u32TgtDataPtr[LenByte>>2];
        IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &u32SrcDataPtr[LenByte>>2];
        idx = LenByte % 4;
        while ( idx > 0 )
        {
          idx--;
          u8TgtDataPtr[idx] = u8SrcDataPtr[idx];
        }
      }
    }
    else if ( (((uint32)(TgtDataPtr)%2) == 0) && (((uint32)(SrcDataPtr)%2) == 0) ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #60 Copy 2 bytes aligned data */
      /* PRQA S 0310,3305 2 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint16)   u16TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint16)) &TgtDataPtr[0];
      IPBASE_P2CONSTAPPLDATA(uint16) u16SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint16)) &SrcDataPtr[0];
      idx = LenByte/2;
      while ( idx > 3 )
      { /* #70 Copy 4 16bit words */
        idx-=4;
        u16TgtDataPtr[idx+3U] = u16SrcDataPtr[idx+3U];
        u16TgtDataPtr[idx+2U] = u16SrcDataPtr[idx+2U];
        u16TgtDataPtr[idx+1U] = u16SrcDataPtr[idx+1U];
        u16TgtDataPtr[idx]    = u16SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #80 Copy 16bit words */
        idx--;
        u16TgtDataPtr[idx] = u16SrcDataPtr[idx];
      }
      if ( (LenByte % 2) != 0 )
      { /* #90 Copy remaining single byte */
        /* PRQA S 0310 2 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8) u8TgtDataPtr   = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
        IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &SrcDataPtr[0];
        u8TgtDataPtr[LenByte-1] = u8SrcDataPtr[LenByte-1];
      }
    }
    else
    { /* #100 Copy 1 byte aligned data */
      /* PRQA S 0310 2 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
      IPBASE_P2CONSTAPPLDATA(uint8) u8SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8)) &SrcDataPtr[0];
      idx = LenByte;
      while ( idx > 3 )
      { /* #110 Copy 4 8bit words */
        idx-=4;
        u8TgtDataPtr[idx+3U] = u8SrcDataPtr[idx+3U];
        u8TgtDataPtr[idx+2U] = u8SrcDataPtr[idx+2U];
        u8TgtDataPtr[idx+1U] = u8SrcDataPtr[idx+1U];
        u8TgtDataPtr[idx]    = u8SrcDataPtr[idx];
      }
      while ( idx > 0 )
      { /* #120 Copy 8bit words */
        idx--;
        u8TgtDataPtr[idx] = u8SrcDataPtr[idx];
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #130 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY_SMALL_DATA, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  } /* IpBase_CopySmallData() */ /* PRQA S 6010,6030,6060 */ /* MD_MSR_STPTH MD_MSR_STCYC MD_MSR_STPAR */
# endif
  /* IPBASE_COPY_MACRO_ENABLE */
/**********************************************************************************************************************
 *  IpBase_Fill
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
# if ( IPBASE_COPY_MACRO_ENABLE == STD_ON )
# else
IPBASE_FUNCCODE(void) IpBase_Fill(IPBASE_P2VARAPPLDATA(IpBase_CopyDataType) TgtDataPtr,
                                  uint8 Pattern, uint32 LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32          idx; /* PRQA S 0781 */ /* MD_IpBase_5.6_781 */

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check TgtDataPtr for NULL pointer */
  if ( TgtDataPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* PRQA S 0310,3305 1 */ /* MD_IpBase_310 */
    IPBASE_P2VARAPPLDATA(uint32) u32TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint32)) TgtDataPtr;

    /* ----- Implementation ----------------------------------------------- */
    if ( ((uint32)(TgtDataPtr)%4) == 0 ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #20 Fill 4 bytes aligned data */
      idx = LenByte/4;
      while ( idx > 0 )
      { /* #30 Fill 32bit words */
        idx--;
        u32TgtDataPtr[idx] = (((uint32)(Pattern))<<24) | (((uint32)(Pattern))<<16) | (((uint32)(Pattern))<<8) | Pattern;
      }
      if ( (LenByte % 4) != 0 )
      { /* #40 Fill remaining single bytes */
        /* PRQA S 0310 1 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &u32TgtDataPtr[LenByte>>2];
        idx = LenByte % 4;
        while ( idx > 0 )
        {
          idx--;
          u8TgtDataPtr[idx] = Pattern;
        }
      }
    }
    else if ( ((uint32)(TgtDataPtr)%2) == 0 ) /* PRQA S 0306 */ /* MD_IpBase_306 */
    { /* #50 Fill 2 bytes aligned data */
      /* PRQA S 0310,3305 1 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint16)   u16TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint16)) &TgtDataPtr[0];
      idx = LenByte/2;
      while ( idx > 0  )
      { /* #60 Fill 16bit words */
        idx--;
        u16TgtDataPtr[idx] = (uint16)((((uint16)(Pattern))<<8) | Pattern);
      }
      if ( (LenByte % 2) != 0 )
      { /* #70 Fill remaining single byte */
        /* PRQA S 0310 1 */ /* MD_IpBase_310 */
        IPBASE_P2VARAPPLDATA(uint8) u8TgtDataPtr   = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
        u8TgtDataPtr[LenByte-1] = Pattern;
      }
    }
    else
    { /* #80 Fill 1 byte aligned data */
      /* PRQA S 0310 1 */ /* MD_IpBase_310 */
      IPBASE_P2VARAPPLDATA(uint8)   u8TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint8)) &TgtDataPtr[0];
      idx = LenByte;
      while ( idx > 0 )
      { /* #90 Fill 8bit words */
        idx--;
        u8TgtDataPtr[idx] = Pattern;
      }
    }
  }
  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #100 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_FILL, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

} /* IpBase_Fill() */ 
# endif
  /* IPBASE_COPY_MACRO_ENABLE */
# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
  /* IPBASE_CODE_ENABLE */

/* module specific MISRA deviations:
 MD_IpBase_5.6_781:
      Reason:     Non-unique identifier (additionally used within structs)
      Risk:       Uncritical if only within structs
      Prevention: Covered by code review
 MD_IpBase_6.3_5013:
      Reason:     Basic data types used for text
      Risk:       Size may differ on depending on platform
      Prevention: Covered by integration tests
 MD_IpBase_11.4:
      Reason:     Caused by type conversions due to message buffer interpretation
      Risk:       Possible programming error may be undetected by compiler
      Prevention: Covered by code review
 MD_IpBase_13.7:
      Reason:     Boolean check with fix result in DET checks due to code configuration
      Risk:       DET checks may fail
      Prevention: AUTOSAR DET checks shall be switched off in production code
 MD_IpBase_17.4:
      Reason:     Usage of pointer within array
      Risk:       Memory Access violation
      Prevention: Covered by code review
 MD_IpBase_21.1:
      Reason:     No wraparound in code
      Risk:       Memory Access violation
      Prevention: Covered by code review
 MD_IpBase_306:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_310:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_750:
      Reason:     union used for IPv6/IPv4 single API
      Risk:       Memory Access violation
      Prevention: Covered by component tests
 MD_IpBase_776:
      Reason:     Prefixing required by AUTOSAR
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_850:
      Reason:     Performance improvement by using macros
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_857:
      Reason:     Number of macros too high
      Risk:       Limited test depth
      Prevention: Covered by component tests
 MD_IpBase_3673:
      Reason:     Parameter no const
      Risk:       Unwanted change of variable
      Prevention: Covered by component tests
*/
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Copy.c
 *********************************************************************************************************************/
