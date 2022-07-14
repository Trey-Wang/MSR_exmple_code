/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* STARTSINGLE_OF_MULTIPLE */

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
/*  \file     Can.c / Can_Irq.c / Can_Local.h
 *  \brief    Implementation of the CAN driver
 *  \details  see functional description below
 *
 *********************************************************************************************************************/
/* ***************************************************************************
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Refer to the module's header file.
|************************************************************************** */

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_INTERNAL
#define CAN_IRQ_SOURCE     /* testability */

/* PRQA S 0850 EOF */ /* MD_Can_0850 */
/* PRQA S 0828 EOF */ /* MD_Can_0828 */
/* PRQA S 0857 EOF */ /* MD_Can_0857 */

#include "Can.h"
#if defined(V_OSTYPE_OSEK) /* COV_CAN_OS_USAGE */
# include "osek.h"
#endif
/* \trace SPEC-1408 */
#if defined(V_OSTYPE_AUTOSAR) /* COV_CAN_OS_USAGE */
# include "Os.h"
#endif

/* ECO_IGNORE_BLOCK_END */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x0507u) /* \trace SPEC-1699, SPEC-3837 */
# error "Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x01u)
# error "Header file are inconsistent!"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_VERSION != 0x0507u)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_RELEASE_VERSION != 0x00u)
# error "Source and Header file are inconsistent!"
#endif

#if( CAN_GEN_CanoeemuCanoeAsr_VERSION != 0x0102u)
# error "Generated Data are inconsistent!"
#endif

/***************************************************************************/
/* external declarations and compatibility defines                         */
/***************************************************************************/
#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
V_DEF_VAR(extern, uint16, VAR_NOINIT) canActiveIdentityMsk; /* PRQA S 3447,3210 */ /* MD_Can_Asr_3447,MD_Can_Asr_Unused */
V_DEF_VAR(extern, uint8, VAR_NOINIT)  canActiveIdentityLog; /* PRQA S 3447,3210 */ /* MD_Can_Asr_3447,MD_Can_Asr_Unused */

# if defined V_ACTIVE_IDENTITY_MSK
#  undef V_ACTIVE_IDENTITY_MSK  /* this define from v_cfg.h is replaced and no more used (but header may included by other module) */ /* PRQA S 0841 */ /* MD_Can_0841 */
# endif
# define V_ACTIVE_IDENTITY_MSK                canActiveIdentityMsk
# if defined V_ACTIVE_IDENTITY_LOG
#  undef V_ACTIVE_IDENTITY_LOG  /* this define from v_cfg.h is replaced and no more used (but header may included by other module) */ /* PRQA S 0841 */ /* MD_Can_0841 */
# endif
# define V_ACTIVE_IDENTITY_LOG                canActiveIdentityLog
#endif

#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_DEF_P2CONST (extern, Can_ConfigType, VAR_INIT, CONST_PBCFG) Can_ConfigDataPtr; /* PRQA S 3447,3451,3210 */ /* MD_Can_Asr_3447,MD_Can_Asr_3451,MD_Can_Asr_Unused */
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/***************************************************************************/
/* Interrupt Service Routine                                               */
/***************************************************************************/
#define CAN_START_SEC_CODE  /*-----------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* \trace SPEC-1579, SPEC-1395, SPEC-1567 */
#if !defined(CAN_POSTBUILD_VARIANT_SUPPORT) /* COV_CAN_COMPATIBILITY */
# define CAN_POSTBUILD_VARIANT_SUPPORT STD_OFF
#endif
#if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
# define CAN_USE_PHYSTOLOG_MAPPING
#else
# if defined (C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
# define CAN_USE_PHYSTOLOG_MAPPING
# endif
#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_0 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_0
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_0
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_0)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_0( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(0));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_0);
#  endif
# else
  CanInterruptRx( 0 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_0
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_0
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_0)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_0( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(0));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_0);
#  endif
# else
  CanInterruptTx( 0 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_0
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_0
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_0)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_0( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(0));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_0);
#  endif
# else
  CanInterruptErr( 0 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_0
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_0
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_0)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_0( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(0));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_0);
#  endif
# else
  CanInterruptWakeup( 0 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_1 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_1
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_1
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_1)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_1( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(1));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_1);
#  endif
# else
  CanInterruptRx( 1 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_1
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_1
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_1)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_1( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(1));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_1);
#  endif
# else
  CanInterruptTx( 1 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_1
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_1
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_1)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_1( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(1));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_1);
#  endif
# else
  CanInterruptErr( 1 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_1
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_1
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_1)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_1( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(1));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_1);
#  endif
# else
  CanInterruptWakeup( 1 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_2 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_2
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_2
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_2)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_2( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(2));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_2);
#  endif
# else
  CanInterruptRx( 2 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_2
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_2
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_2)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_2( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(2));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_2);
#  endif
# else
  CanInterruptTx( 2 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_2
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_2
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_2)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_2( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(2));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_2);
#  endif
# else
  CanInterruptErr( 2 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_2
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_2
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_2)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_2( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(2));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_2);
#  endif
# else
  CanInterruptWakeup( 2 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_3 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_3
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_3
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_3)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_3( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(3));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_3);
#  endif
# else
  CanInterruptRx( 3 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_3
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_3
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_3)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_3( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(3));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_3);
#  endif
# else
  CanInterruptTx( 3 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_3
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_3
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_3)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_3( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(3));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_3);
#  endif
# else
  CanInterruptErr( 3 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_3
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_3
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_3)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_3( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(3));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_3);
#  endif
# else
  CanInterruptWakeup( 3 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_4 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_4
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_4
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_4)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_4( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(4));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_4);
#  endif
# else
  CanInterruptRx( 4 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_4
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_4
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_4)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_4( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(4));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_4);
#  endif
# else
  CanInterruptTx( 4 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_4
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_4
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_4)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_4( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(4));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_4);
#  endif
# else
  CanInterruptErr( 4 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_4
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_4
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_4)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_4( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(4));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_4);
#  endif
# else
  CanInterruptWakeup( 4 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_5 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_5
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_5
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_5)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_5( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(5));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_5);
#  endif
# else
  CanInterruptRx( 5 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_5
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_5
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_5)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_5( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(5));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_5);
#  endif
# else
  CanInterruptTx( 5 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_5
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_5
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_5)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_5( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(5));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_5);
#  endif
# else
  CanInterruptErr( 5 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_5
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_5
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_5)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_5( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(5));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_5);
#  endif
# else
  CanInterruptWakeup( 5 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_6 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_6
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_6
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_6)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_6( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(6));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_6);
#  endif
# else
  CanInterruptRx( 6 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_6
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_6
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_6)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_6( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(6));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_6);
#  endif
# else
  CanInterruptTx( 6 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_6
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_6
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_6)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_6( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(6));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_6);
#  endif
# else
  CanInterruptErr( 6 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_6
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_6
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_6)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_6( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(6));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_6);
#  endif
# else
  CanInterruptWakeup( 6 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_7 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_7
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_7
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_7)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_7( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(7));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_7);
#  endif
# else
  CanInterruptRx( 7 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_7
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_7
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_7)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_7( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(7));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_7);
#  endif
# else
  CanInterruptTx( 7 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_7
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_7
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_7)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_7( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(7));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_7);
#  endif
# else
  CanInterruptErr( 7 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_7
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_7
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_7)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_7( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(7));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_7);
#  endif
# else
  CanInterruptWakeup( 7 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_8 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_8
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_8
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_8)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_8( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(8));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_8);
#  endif
# else
  CanInterruptRx( 8 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_8
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_8
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_8)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_8( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(8));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_8);
#  endif
# else
  CanInterruptTx( 8 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_8
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_8
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_8)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_8( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(8));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_8);
#  endif
# else
  CanInterruptErr( 8 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_8
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_8
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_8)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_8( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(8));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_8);
#  endif
# else
  CanInterruptWakeup( 8 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_9 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_9
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_9
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_9)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_9( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(9));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_9);
#  endif
# else
  CanInterruptRx( 9 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_9
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_9
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_9)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_9( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(9));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_9);
#  endif
# else
  CanInterruptTx( 9 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_9
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_9
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_9)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_9( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(9));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_9);
#  endif
# else
  CanInterruptErr( 9 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_9
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_9
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_9)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_9( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(9));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_9);
#  endif
# else
  CanInterruptWakeup( 9 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_10 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_10
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_10
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_10)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_10( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(10));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_10);
#  endif
# else
  CanInterruptRx( 10 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_10
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_10
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_10)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_10( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(10));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_10);
#  endif
# else
  CanInterruptTx( 10 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_10
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_10
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_10)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_10( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(10));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_10);
#  endif
# else
  CanInterruptErr( 10 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_10
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_10
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_10)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_10( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(10));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_10);
#  endif
# else
  CanInterruptWakeup( 10 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_11 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_11
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_11
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_11)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_11( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(11));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_11);
#  endif
# else
  CanInterruptRx( 11 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_11
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_11
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_11)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_11( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(11));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_11);
#  endif
# else
  CanInterruptTx( 11 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_11
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_11
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_11)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_11( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(11));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_11);
#  endif
# else
  CanInterruptErr( 11 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_11
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_11
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_11)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_11( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(11));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_11);
#  endif
# else
  CanInterruptWakeup( 11 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_12 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_12
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_12
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_12)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_12( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(12));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_12);
#  endif
# else
  CanInterruptRx( 12 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_12
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_12
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_12)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_12( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(12));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_12);
#  endif
# else
  CanInterruptTx( 12 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_12
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_12
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_12)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_12( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(12));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_12);
#  endif
# else
  CanInterruptErr( 12 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_12
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_12
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_12)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_12( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(12));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_12);
#  endif
# else
  CanInterruptWakeup( 12 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_13 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_13
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_13
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_13)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_13( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(13));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_13);
#  endif
# else
  CanInterruptRx( 13 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_13
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_13
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_13)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_13( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(13));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_13);
#  endif
# else
  CanInterruptTx( 13 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_13
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_13
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_13)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_13( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(13));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_13);
#  endif
# else
  CanInterruptErr( 13 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_13
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_13
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_13)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_13( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(13));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_13);
#  endif
# else
  CanInterruptWakeup( 13 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_14 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_14
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_14
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_14)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_14( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(14));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_14);
#  endif
# else
  CanInterruptRx( 14 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_14
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_14
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_14)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_14( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(14));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_14);
#  endif
# else
  CanInterruptTx( 14 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_14
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_14
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_14)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_14( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(14));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_14);
#  endif
# else
  CanInterruptErr( 14 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_14
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_14
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_14)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_14( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(14));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_14);
#  endif
# else
  CanInterruptWakeup( 14 );
# endif
}

#endif

/*  ------------------------   Channel: _CH   ------------------------------ */
#if defined( kCanPhysToLogChannelIndex_15 )  /* not really phys to log mapping (phys==log), this will be done by CANoe due to API CANoeAPI_MapChannel */ /* COV_CAN_LL_ISR_USAGE */

/****************************************************************************
| NAME:             CanIsrRx_15
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrRx_15
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrRx_15)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrRx_15( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 RxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptRx(Can_GetPhysToLogChannel(15));
#  else
  CanInterruptRx(kCanPhysToLogChannelIndex_15);
#  endif
# else
  CanInterruptRx( 15 );
# endif
}

/****************************************************************************
| NAME:             CanIsrTx_15
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrTx_15
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrTx_15)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrTx_15( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 TxISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptTx(Can_GetPhysToLogChannel(15));
#  else
  CanInterruptTx(kCanPhysToLogChannelIndex_15);
#  endif
# else
  CanInterruptTx( 15 );
# endif
}

/****************************************************************************
| NAME:             CanIsrErr_15
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrErr_15
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrErr_15)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrErr_15( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 ErrISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptErr(Can_GetPhysToLogChannel(15));
#  else
  CanInterruptErr(kCanPhysToLogChannelIndex_15);
#  endif
# else
  CanInterruptErr( 15 );
# endif
}

/****************************************************************************
| NAME:             CanIsrWakeup_15
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanIsrWakeup_15
Relation_Context:
Relation:
CanInterruptControl
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# if defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_LL_OS_USAGE */
ISR(CanIsrWakeup_15)
# else
V_DEF_FUNC(V_NONE, void, CODE) CanIsrWakeup_15( void ) /* PRQA S 3408 */ /* declared in tcb.h but encapsulated by os */ /* COV_CAN_LL_ISR_USAGE */
# endif
{
  /* #10 WakeupISR */
# if !defined(C_SINGLE_RECEIVE_CHANNEL) /* COV_CAN_LL_ISR_USAGE */
#  if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_LL_OS_USAGE */
  CanInterruptWakeup(Can_GetPhysToLogChannel(15));
#  else
  CanInterruptWakeup(kCanPhysToLogChannelIndex_15);
#  endif
# else
  CanInterruptWakeup( 15 );
# endif
}

#endif

#define CAN_STOP_SEC_CODE  /*------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

