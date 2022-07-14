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
 *         File:  Appl_Rand.c
 *      Project:  MICROSAR IP
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  demo random generator (integration support example providing random functions required by MICROSAR IP)
 *  
 *********************************************************************************************************************/
#define APPL_RAND_SOURCE

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define APPL_RAND_CRYPTO_LIB                        STD_OFF
/* 
  Switch to use Appl_Crypto_GetRandNo() to obtain a seed for random number generatur and not to provide an actual 
  random number. Temporary workaround for issue in Crypto module.
*/
#define APPL_RAND_CRYPTO_LIB_USE_RAND_AS_SEED       STD_OFF

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Appl_Rand.h"
#if (APPL_RAND_CRYPTO_LIB == STD_ON)
#include "Crypto.h"
#elif defined C_COMP_ANSI_CANOE
#include <time.h>
#else
#include "EthIf.h"
#include "TcpIp_Types.h"
#endif
  /* APPL_RAND_CRYPTO_LIB */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#if (APPL_RAND_CRYPTO_LIB == STD_ON)
#define APPL_RAND_ENTROPY_LEN_BYTES                      2 /* uint16 seed */
#endif
  /* APPL_RAND_CRYPTO_LIB */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
static uint16 ApplRand_RandNo;

/***********************************************************************************************************************
 *  ApplRand_Init
 **********************************************************************************************************************/
/*! \brief      init random number generator
 *  \context    initialization
 **********************************************************************************************************************/
void ApplRand_Init( void )
{
#if (APPL_RAND_CRYPTO_LIB == STD_ON)
  ApplRand_RandNo   = 0; /* read stored number from flash */
  ApplRand_RandNo  |= 0; /* set to unique number (ECU serial number, MAC address) */
  ApplRand_RandNo  |= 0; /* add arbitrary part of sensor information (lower bits of voltage, current, ...) */
  Crypto_StirRNG(sizeof(ApplRand_RandNo), (uint8 *) &ApplRand_RandNo);
#elif defined C_COMP_ANSI_CANOE
  {
    uint32 Time;
    Time = (uint32)(_time32(NULL));

    ApplRand_RandNo = (uint16)Time;
  }
#else
  uint8 Sum;
  IpBase_EthPhysAddrType CtrlMacAddr;
  IpBase_EthPhysAddrType *MacAddrPtr;

  EthIf_GetPhysAddr(0, CtrlMacAddr);
  MacAddrPtr = &(CtrlMacAddr);

  Sum = (*MacAddrPtr)[0] + (*MacAddrPtr)[2] + (*MacAddrPtr)[4];
  ApplRand_RandNo = ((uint16)Sum) << 8;
  Sum = (*MacAddrPtr)[1] + (*MacAddrPtr)[3] + (*MacAddrPtr)[5];
  ApplRand_RandNo |= ((uint16)Sum);
#endif
  /* APPL_RAND_CRYPTO_LIB */
}
/***********************************************************************************************************************
 *  ApplRand_GetRandNo
 **********************************************************************************************************************/
/*! \brief      get a random number between 0x0 and 0x7FFF
 *              HINT: Currently this function is called by module Crypto to obtain a seed for random number generation
 *                    during Crypt_Init()
 *  \return     TcpIp_Rand_RandNo       random number
 *  \context    interrupt or task level
 **********************************************************************************************************************/
uint16 Appl_Crypto_GetRandNo(void)
{
  return ApplRand_GetRandNo();
}
uint16 ApplRand_GetRandNo(void)
{
#if (APPL_RAND_CRYPTO_LIB_USE_RAND_AS_SEED == STD_ON)
  /* A real random value is needed here, otherwise this will break the security of the cryptographic calculations */
  ApplRand_RandNo   = 0; /* read stored number from flash */
  ApplRand_RandNo  |= 0; /* set to unique number (ECU serial number, MAC address) */
  ApplRand_RandNo  |= 0; /* add arbitrary part of sensor information (lower bits of voltage, current, ...) */
#else
#if (APPL_RAND_CRYPTO_LIB == STD_ON)
  eslt_ErrorCode Err;

  Err = esl_getBytesRNG(sizeof(ApplRand_RandNo), (eslt_Byte *) &ApplRand_RandNo);
  if ( ESL_ERC_NO_ERROR != Err )
  {
    /* store error in Dem, use alternate possibility */
  }
#else
  sint16 Seed = (sint16)ApplRand_RandNo+1;
  const sint16 m = 32767;  /* (2^15)-1 */
  const sint8  a = 79;     /* prime number */
  const sint16 q = m / a;
  const sint8  r = (sint8)(m % a);

  Seed = (a * (Seed % q)) - ((Seed / q) * r);

  ApplRand_RandNo = (uint16)Seed;  /* range: [0x0 .. 0xFFFF] */

#endif
  /* APPL_RAND_CRYPTO_LIB */
#endif
  /* APPL_RAND_CRYPTO_LIB_USE_RAND_AS_SEED */
  return ApplRand_RandNo;
}
/***********************************************************************************************************************
 *  ApplRand_GetRandArray
 **********************************************************************************************************************/
/*! \brief      function filling array with random values
 *  \param[in]  TgtDataPtr     pointer where output data shall be written
 *  \param[in]  TgtLen         target data length that shall be filled
 *  \context    interrupt or task level
 **********************************************************************************************************************/
void Appl_Crypto_GetRandArray(uint8* TgtDataPtr, uint16 TgtLen)
{
  ApplRand_GetRandArray(TgtDataPtr, TgtLen);
}
void ApplRand_GetRandArray(uint8* TgtDataPtr, uint16 TgtLen)
{
  uint16 Rand;
  uint16 Num;
  uint16 i;

  Num = TgtLen / 2;
  for(i=0; i<Num; i++)
  {
    /* fill two bytes */
    Rand = ApplRand_GetRandNo();
    TgtDataPtr[(i*2)   ] = (uint8)(Rand >> 8);
    TgtDataPtr[(i*2) +1] = (uint8)(Rand >> 0);
  }

  if(0 != (TgtLen % 2))
  {
    /* fill last byte */
    Rand = ApplRand_GetRandNo();
    TgtDataPtr[TgtLen -1] = (uint8)(Rand);
  }
}
/***********************************************************************************************************************
 *  ApplRand_Deinit
 **********************************************************************************************************************/
/*! \brief      store random number
 *  \context    de-initialization
 **********************************************************************************************************************/
void ApplRand_Deinit( void )
{
  /* store ApplRand_RandNo in flash */
}
/**********************************************************************************************************************
 *  END OF FILE: Appl_Rand.c
 *********************************************************************************************************************/
