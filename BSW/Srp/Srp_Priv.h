/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Srp_Priv.h
 *        \brief  SRP private header file
 *
 *      \details  Private header file of the Stream Registration Protocol (SRP)
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

#if !defined(SRP_PRIV_H)
# define SRP_PRIV_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Srp_Cfg.h"
# include "SchM_Srp.h"   /* Include scheduler module for critical sections */
# include "IpBase.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define SRP_MBITS_TO_BITS_FACTOR         (1000000UL)

# define SRP_INV_QUEUE_PRIO               (255U)
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! \brief Mapping of SchM Exclusive Areas */
# define SRP_ENTER_CRITICAL_SECTION_0()   SchM_Enter_Srp_SRP_EXCLUSIVE_AREA_0()
# define SRP_LEAVE_CRITICAL_SECTION_0()   SchM_Exit_Srp_SRP_EXCLUSIVE_AREA_0()

/*! \brief      Mapping of IpBase Functions for swapping of Byte-order */
/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
# define SRP_BYTE_SWAP64(Data)  ( (((uint64)(Data) >> 56u)&0x00000000000000FFULL) | \
                                  (((uint64)(Data) >> 40u)&0x000000000000FF00ULL) | \
                                  (((uint64)(Data) >> 24u)&0x0000000000FF0000ULL) | \
                                  (((uint64)(Data) >> 8u )&0x00000000FF000000ULL) | \
                                  (((uint64)(Data) << 56u)&0xFF00000000000000ULL) | \
                                  (((uint64)(Data) << 40u)&0x00FF000000000000ULL) | \
                                  (((uint64)(Data) << 24u)&0x0000FF0000000000ULL) | \
                                  (((uint64)(Data) << 8u )&0x000000FF00000000ULL) )

/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
# define Srp_Htons(HostShort16)                       IPBASE_HTON16((HostShort16))
# define Srp_Htonl(HostLong32)                        IPBASE_HTON32((HostLong32))
# define Srp_Ntohs(NetworkShort16)                    IPBASE_HTON16((NetworkShort16))
# define Srp_Ntohl(NetworkLong32)                     IPBASE_HTON32((NetworkLong32))

# if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
#  define Srp_Hton64(HotsLong64)
#  define Srp_Ntoh64(NetworkLong64)
# else
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
#  define Srp_Hton64(HostLong64)     SRP_BYTE_SWAP64((HostLong64))
#  define Srp_Ntoh64(NetworkLong64)  SRP_BYTE_SWAP64((NetworkLong64))
# endif /* ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST ) */

/*! \brief   Copy Data to Buffer (Aligned) */
/* PRQA S 3458 MACRO_BRACES */ /* MD_MSR_19.4 */
# define Srp_CopyUint16ToBuffer(U8BufPtr, Offset, Value) \
  { \
  (U8BufPtr)[(Offset)] = (uint8)(((uint16)(Value)) >> 8u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(Value); \
  (Offset)++; \
  }

# define Srp_CopyUint32ToBuffer(U8BufPtr, Offset, Value) \
  { \
  (U8BufPtr)[(Offset)] = (uint8)(((uint32)(Value)) >> 24u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint32)(Value)) >> 16u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint32)(Value)) >> 8u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)((uint32)(Value)); \
  (Offset)++; \
  }

# define Srp_CopyUint64ToBuffer(U8BufPtr, Offset, Value) \
  { \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 56u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 48u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 40u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 32u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 24u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 16u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)(((uint64)(Value)) >> 8u); \
  (Offset)++; \
  (U8BufPtr)[(Offset)] = (uint8)((uint64)(Value)); \
  (Offset)++; \
  }
/* PRQA L: MACRO_BRACES */

/* Convert MBits to Bits and vice versa */
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define SRP_MBITS_TO_BITS_UINT32(MBITS)   ((uint32)((float32)(MBITS) * (float32)SRP_MBITS_TO_BITS_FACTOR))
# define SRP_BITS_TO_MBITS_FLOAT32(BITS)   ((float32)((float32)(BITS) / (float32)SRP_MBITS_TO_BITS_FACTOR))
/**********************************************************************************************************************
*  CANOE DEBUG SUPPORT 
**********************************************************************************************************************/
# ifndef CANOE_WRITE_STRING
#  define CANOE_WRITE_STRING(txt)
# endif
# ifndef CANOE_WRITE_STRING_1
#  define CANOE_WRITE_STRING_1(txt, p1)
# endif
# ifndef CANOE_WRITE_STRING_2
#  define CANOE_WRITE_STRING_2(txt, p1, p2)
# endif
# ifndef CANOE_WRITE_STRING_3
#  define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
# endif
# ifndef CANOE_WRITE_STRING_4
#  define CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define SRP_START_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define SRP_STOP_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SRP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define SRP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* SRP_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: Srp_Priv.h
 *********************************************************************************************************************/
