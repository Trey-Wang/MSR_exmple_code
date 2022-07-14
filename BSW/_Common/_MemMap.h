/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2007 - 2017 by Vector Informatik GmbH.                                           All rights reserved.
 *
 *                Please note, that this file contains example configuration used by the 
 *                MICROSAR BSW. This code may influence the behaviour of the MICROSAR BSW
 *                in principle. Therefore, great care must be taken to verify
 *                the correctness of the implementation.
 *
 *                The contents of the originally delivered files are only examples respectively
 *                implementation proposals. With regard to the fact that these functions
 *                are meant for demonstration purposes only, the liability of Vector Informatik
 *                shall be expressly excluded in cases of ordinary negligence, 
 *                to the extent admissible by law or statute.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  _MemMap.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  This File is a template for the MemMap.h
 *                This file has to be extended with the memory section defined for all BSW modules
 *                which are used.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Joachim Kalmbach              Jk            Vector Informatik GmbH
 *  Heike Honert                  Ht            Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2007-08-01  Jk                    Initial creation
 *  01.01.00  2007-12-14  Jk                    Component specific defines filtering added
 *  01.01.02  2008-11-04  Jk                    Component specific defines filtering added
 *  01.01.03  2008-12-17  Ht                    Improve list of components  (Tp_AsrTpCan,Tp_AsrTpFr,DrvMcu,DrvIcu added)
 *  01.01.04  2009-04-27  Ht                    improve list of components  (Cp_XcpOnCanAsr, Il_AsrIpduM, If_VxFblDcm,
 *                                              If_VxFblVpm_Volvo_ab, DrvFls added)
 *  01.01.05  2009-04-24  Msr                   Renamed J1939_AsrBase as TpJ1939_AsrBase
 *  01.01.06  2009-06-03  Ht                    Improve list of components (Adc, Dio, Gpt, Pwm, Spi, Wdg, Fls, Port, Fim)
 *  01.02.00  2009-08-01  Ht                    Improve list of components (Fee_30_Inst2, Can, ...Sub)
 *                                              Support filtering for RTE
 *  01.02.01  2009-08-18  HH                    replaced C++ comment by C comment
 *  01.02.02  2009-09-02  Lo                    add external Flash driver support
 *  01.02.03  2009-09-12  Lo                    add DrvFls_Mcs12xFslftm01ExtVx
 *                        Ht                    Improve list of components (CanTrcv_30_Tja1040dio,
 *                                                Eth, EthTrcv, EthIf, SoAd, TcpIp, EthSM)
 *  01.03.00  2009-10-30  Ht                    support R8: change EthTrcv to EthTrcv_30_Canoeemu
 *                                              support EthTrcv_30_Dp83848
 *                                              change CanTrcv_30_Xdio to CanTrcv_30___Your_Trcv__
 *                                              change CanTrcv_30_Tja1040dio to CanTrcv_30_Tja1041
 *                                              change name FrTrcv to FrTrcv_30_Tja1080dio
 *                        Lo                    add Cp_AsrXcp
 *                        Ht                    add Cp_XcpOnFrAsr
 *  01.03.01  2010-01-13  Ht                    support SysService_AsrCal
 *  01.03.02  2010-02-15  Ht                    support SysService_SswRcs_Daimler, SysService_Tls, Tp_Http, 
 *                                                      SysService_Dns, SysService_Json, DrvTrans_GenericLindioAsr
 *                        Lo                    add Diag_AsrDem for all OEMs
 *                                              rename internal variables and filter methods
 *  01.04.00  2010-03-04  Ht                    change name FrTrcv_30_Tja1080dio to FrTrcv_30_Tja1080
 *  01.04.01  2010-03-10  Ht                    support DrvTrans_GenericFrAsr, DrvTrans_As8223FrspiAsr, DrvEep and If_AsrIfEa
 *  01.04.02  2010-04-07  Lo                    change IfFee to real components and add If_AsrIfWdV85xNec01Sub
 *  01.04.03  2010-06-11  Ht                    add CanTrcv_30_Tja1043
 *                        Lo                    add Il_AsrIpduMEbBmwSub
 *  01.04.04  2010-08-24  Ht                    add CanTrcv_30_Tle62512G, DrvEep_XAt25128EAsr, Tp_AsrTpFrEbBmwSub
 *  01.05.00  2010-08-24  Ht                    support R10:
 *                                              change LinTrcv_30_Tle7259dio to LinTrcv_30_Tle7259
 *  01.05.01  2010-10-14  Ht                    add VStdLib, SysService_SswScc, SysService_IpBase, SysService_Crypto
 *  01.05.02  2010-10-20  Ht                    support comments for Package Merge Tool
 *  01.05.03  2010-11-03  Ht                    add SysService_E2eLibTttechSub, SysService_E2ePwTttechSub
 *  01.05.04  2010-11-16  Ht                    add SysService_Exi, DrvTrans_Int6400EthAsr, Cdd_AsrCdd_Fiat, Diag_AsrDem_Fiat
 *  01.05.05  2010-12-17  Ht                    add SysService_AsrSchM, DrvEep_XXStubAsr, DrvIcu_Tms570Tinhet01ExtVx
 *                                                  DrvWd_XTle4278gEAsr, DrvWd_XXStubAsr
 *  01.05.06  2011-02-17  Ht                    add DrvEed, SysService_AsrBswM
 *  01.05.07  2011-03-04  Ht                    add DrvTrans_Tja1055CandioAsr
 *                                              rename CanTrcv_30_Tja1040dio to CanTrcv_30_Tja1040
 *                                              add SysService_XmlEngine
 *  01.06.00  2011-03-15  Ht                    support ASR4.0
 *                                              add Ccl_Asr4ComM, Ccl_Asr4SmCan, Nm_Asr4NmIf, Nm_AsrNmDirOsek
 *  01.06.01  2011-04-15  Ht                    add Diag_AsrDcm_<OEM>
 *  01.06.02  2011-06-17  Ht                    correct Diag_AsrDcm_<OEM>
 *                                              add Monitoring_AsrDlt and Monitoring_GenericMeasurement
 *  01.06.03  2011-09-20  Ht                    add DrvTrans_Tja1145CanSpiAsr, DrvTrans_E52013CanspiAsr, DrvFls_XXStubAsr,
 *                                              If_AsrIfFeeV85xNec05Sub, If_AsrIfFeeV85xNec06Sub, If_AsrIfFeeV85xNec07Sub
 *                                              SysService_AsrWdMTttechSub and If_AsrIfWdTttechSub
 *  01.06.04  2011-11-22  Ht                    add If_AsrIfFeeTiSub, 
 *                                              ESCAN00054718: add Cdd_AsrCdd
 *  01.06.05  2011-12-09  Ht                    add Tp_IpV4, Tp_IpV6
 *  01.06.06  2011-12-14  Ht                    add Monitoring_RuntimeMeasurement
 *  01.06.07  2012-01-03  Ht                    add DrvI2c, SysService_Asr4BswM
 *  01.06.08  2012-01-31  Ht                    add DrvTrans_Ar7000EthAsr, DrvTrans_GenericEthmiiAsr
 *  01.06.09  2012-03-06  Ht                    add If_AsrIfFeeMb9df126Fuji01Sub, 
 *                                              Infineon_Tc1767Inf01Sub, Infineon_Tc178xInf01Sub, Infineon_Tc1797Inf01Sub, Infineon_Tc1797Inf02Sub
 *  01.06.10  2012-03-13  Ht                    add Gw_AsrPduRCfg5, Il_AsrComCfg5, Il_AsrIpduMCfg5, Cdd_AsrCddCfg5,
 *                                              Tp_Asr4TpCan, Diag_Asr4Dcm, Diag_Asr4Dem
 *  01.06.11  2012-03-20  Ht                    add Cp_AsrCcp, Cp_XcpOnTcpIpAsr
 *  01.07.00  2012-07-26  Ht                    ESCAN00059365: [AUTOSAR4, compiler warning]: Wrong define name in #undef statement causes re-definition warning
 *                                              add Nm_Asr4NmCan, Nm_Asr4NmFr, Infineon_Xc2000Inf01Sub, Ccl_Asr4ComMCfg5, SysService_Asr4BswMCfg5, SysService_Asr4EcuM, SysService_AsrRamTst,
 *                                                  Ccl_Asr4SmLin 
 *            2012-09-04  Ht                    add support for ASR specification 4.0 R3
 *  01.07.01  2012-10-23  Seu                   add SysService_XmlSecurity
 *  01.07.02  2013-01-10  Seu                   MISRA deviation comments added
 *  01.08.00  2013-03-01  Seu    ESCAN00065501  AR4-325: Add support for Post-build RAM memory section
 *            2013-04-12  Seu    ESCAN00066614  Add the deviation for violation of MISRA rule 19.6
 *  01.09.00  2013-12-10  Seu    ESCAN00072537  Add *_NOCACHE_* memory sections for variables
 *            2013-12-16  Seu                   MISRA compliance: usage of character "'" removed, typos corrected
 *  01.10.00  2016-09-27  Seu                   FEATC-317: FEAT-2002: CommonAsr__Common: Support 64 Bit Signal Types for COM according to ASR 4.2.2
 *  01.11.00  2017-07-05  Seu    ESCAN00095756  FEAT-2455: Support ASR4.2 compatible MemMap for MCALs
 *            2017-08-01  Seu    ESCAN00096129  MEMMAP_SW_MINOR_VERSION / MEM_SW_MINOR_VERSION is not correct
 *********************************************************************************************************************/

/* PRQA S 0841 MEMMAP_0841_TAG */ /* MD_MSR_19.6 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

 /* PRQA S 0883 1 */ /* MD_MemMap_19.15 */
#undef MEMMAP_VENDOR_ID /* PRQA S 0841 */ /* MD_MSR_19.6 */

#define MEMMAP_VENDOR_ID        (30u)

/* AUTOSAR Software Specification Version Information */


# undef MEMMAP_AR_RELEASE_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_AR_RELEASE_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_AR_RELEASE_REVISION_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

/* AUTOSAR release 4.0 R3 */
# define MEMMAP_AR_RELEASE_MAJOR_VERSION       (4u)
# define MEMMAP_AR_RELEASE_MINOR_VERSION       (0u)
# define MEMMAP_AR_RELEASE_REVISION_VERSION    (3u)

/* compatiblity to older versions */
# undef MEM_AR_RELEASE_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_AR_RELEASE_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_AR_RELEASE_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEM_AR_RELEASE_MAJOR_VERSION          MEMMAP_AR_RELEASE_MAJOR_VERSION
# define MEM_AR_RELEASE_MINOR_VERSION          MEMMAP_AR_RELEASE_MINOR_VERSION
# define MEM_AR_RELEASE_PATCH_VERSION          MEMMAP_AR_RELEASE_REVISION_VERSION


# undef MEMMAP_SW_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_SW_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_SW_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEMMAP_SW_MAJOR_VERSION     (1u)
# define MEMMAP_SW_MINOR_VERSION     (11u)
# define MEMMAP_SW_PATCH_VERSION     (0u)

/* compatiblity to older versions */
# undef MEM_SW_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_SW_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_SW_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEM_SW_MAJOR_VERSION        MEMMAP_SW_MAJOR_VERSION 
# define MEM_SW_MINOR_VERSION        MEMMAP_SW_MINOR_VERSION 
# define MEM_SW_PATCH_VERSION        MEMMAP_SW_PATCH_VERSION 
  
#define MEMMAP_ERROR

/* Package Merger: Start Section MemMapModuleList */


#include "Rte_MemMap.h"


/**********************************************************************************************************************
 *  COMM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef COMM_START_SEC_CODE
  #undef COMM_START_SEC_CODE                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef COMM_STOP_SEC_CODE
  #undef COMM_STOP_SEC_CODE                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef COMM_START_SEC_CONST_8BIT
# undef COMM_START_SEC_CONST_8BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef COMM_STOP_SEC_CONST_8BIT
# undef COMM_STOP_SEC_CONST_8BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef COMM_START_SEC_CONST_16BIT
# undef COMM_START_SEC_CONST_16BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef COMM_STOP_SEC_CONST_16BIT
# undef COMM_STOP_SEC_CONST_16BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef COMM_START_SEC_CONST_UNSPECIFIED
# undef COMM_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef COMM_STOP_SEC_CONST_UNSPECIFIED
# undef COMM_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* CONST PBCFG sections */

#ifdef COMM_START_SEC_PBCFG
# undef COMM_START_SEC_PBCFG                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef COMM_STOP_SEC_PBCFG
# undef COMM_STOP_SEC_PBCFG                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef COMM_START_SEC_VAR_NOINIT_8BIT
# undef COMM_START_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef COMM_STOP_SEC_VAR_NOINIT_8BIT
# undef COMM_STOP_SEC_VAR_NOINIT_8BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef COMM_START_SEC_VAR_NOINIT_16BIT
# undef COMM_START_SEC_VAR_NOINIT_16BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef COMM_STOP_SEC_VAR_NOINIT_16BIT
# undef COMM_STOP_SEC_VAR_NOINIT_16BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef COMM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef COMM_START_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef COMM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef COMM_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR PBCFG sections */

#ifdef COMM_START_SEC_VAR_PBCFG
# undef COMM_START_SEC_VAR_PBCFG                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG                         /* mapped to default var pbcfg section */
#endif
#ifdef COMM_STOP_SEC_VAR_PBCFG
# undef COMM_STOP_SEC_VAR_PBCFG                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                /* default var stop section */
#endif

/* VAR ZERO INIT sections */

#ifdef COMM_START_SEC_VAR_ZERO_INIT_8BIT
# undef COMM_START_SEC_VAR_ZERO_INIT_8BIT           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef COMM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef COMM_STOP_SEC_VAR_ZERO_INIT_8BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef COMM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef COMM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef COMM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef COMM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NVRAM sections */

#ifdef COMM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
# undef COMM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#ifdef COMM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
# undef COMM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  COMM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CanSM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CANSM_START_SEC_CODE
# undef CANSM_START_SEC_CODE
# define START_SEC_CODE                                               /* mapped to default code section */
#endif

#ifdef CANSM_STOP_SEC_CODE
# undef CANSM_STOP_SEC_CODE
# define STOP_SEC_CODE                                                /* default code stop section */
#endif


#ifdef CANSM_START_SEC_APPL_CODE
# undef CANSM_START_SEC_APPL_CODE
# define START_SEC_CODE                                               /* mapped to default code section */
#endif

#ifdef CANSM_STOP_SEC_APPL_CODE
# undef CANSM_STOP_SEC_APPL_CODE
# define STOP_SEC_CODE                                                /* default code stop section */
#endif
 


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */


#ifdef CANSM_START_SEC_CONST_8BIT
# undef CANSM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif

#ifdef CANSM_STOP_SEC_CONST_8BIT
# undef CANSM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                                               /* default const stop section */
#endif

#ifdef CANSM_START_SEC_CONST_32BIT
# undef CANSM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif

#ifdef CANSM_STOP_SEC_CONST_32BIT
# undef CANSM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif


#ifdef CANSM_START_SEC_CONST_UNSPECIFIED
# undef CANSM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef CANSM_STOP_SEC_CONST_UNSPECIFIED
# undef CANSM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/* FAST CONST sections */
#ifdef CANSM_START_SEC_CONST_FAST_8BIT
# undef CANSM_START_SEC_CONST_FAST_8BIT
# define START_SEC_CONST_FAST_8BIT
#endif

#ifdef CANSM_STOP_SEC_CONST_FAST_8BIT
# undef CANSM_STOP_SEC_CONST_FAST_8BIT
# define STOP_SEC_CONST                                               /* default const stop section */
#endif

#ifdef CANSM_START_SEC_CONST_FAST_32BIT
# undef CANSM_START_SEC_CONST_FAST_32BIT
# define START_SEC_CONST_FAST_32BIT
#endif

#ifdef CANSM_STOP_SEC_CONST_FAST_32BIT
# undef CANSM_STOP_SEC_CONST_FAST_32BIT
# define STOP_SEC_CONST
#endif


#ifdef CANSM_START_SEC_CONST_FAST_UNSPECIFIED
# undef CANSM_START_SEC_CONST_FAST_UNSPECIFIED
# define START_SEC_CONST_FAST_UNSPECIFIED
#endif

#ifdef CANSM_STOP_SEC_CONST_FAST_UNSPECIFIED
# undef CANSM_STOP_SEC_CONST_FAST_UNSPECIFIED
# define STOP_SEC_CONST
#endif



/* Postbuild CFG CONST sections */



/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */


/* VAR NOINIT sections */

#ifdef CANSM_START_SEC_VAR_NOINIT_8BIT
# undef CANSM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif

#ifdef CANSM_STOP_SEC_VAR_NOINIT_8BIT
# undef CANSM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif


#ifdef CANSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef CANSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef CANSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CANSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef CANSM_START_SEC_VAR_ZERO_INIT_8BIT
# undef CANSM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif

#ifdef CANSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CANSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */

/* VAR FAST NOINIT sections */

#ifdef CANSM_START_SEC_VAR_FAST_NOINIT_8BIT
# undef CANSM_START_SEC_VAR_FAST_NOINIT_8BIT
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif

#ifdef CANSM_STOP_SEC_VAR_FAST_NOINIT_8BIT
# undef CANSM_STOP_SEC_VAR_FAST_NOINIT_8BIT
# define STOP_SEC_VAR
#endif


#ifdef CANSM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef CANSM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif

#ifdef CANSM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef CANSM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR FAST ZERO INIT sections */

#ifdef CANSM_START_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef CANSM_START_SEC_VAR_FAST_ZERO_INIT_8BIT
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif

#ifdef CANSM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef CANSM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif







/**********************************************************************************************************************
 *  CanSM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FrSM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FRSM_START_SEC_CODE
  #undef FRSM_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef FRSM_STOP_SEC_CODE
  #undef FRSM_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */



#ifdef FRSM_START_SEC_CONST_32BIT
# undef FRSM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef FRSM_STOP_SEC_CONST_32BIT
# undef FRSM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif


#ifdef FRSM_START_SEC_CONST_UNSPECIFIED
# undef FRSM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef FRSM_STOP_SEC_CONST_UNSPECIFIED
# undef FRSM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/* FAST CONST sections */



/* Postbuild CFG CONST sections */

/* Root pointer to postbuild data */
#ifdef FRSM_START_SEC_PBCFG_ROOT
# undef FRSM_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef FRSM_STOP_SEC_PBCFG_ROOT
# undef FRSM_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/* Tables for postbuild data */
#ifdef FRSM_START_SEC_PBCFG
# undef FRSM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef FRSM_STOP_SEC_PBCFG
# undef FRSM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */


/* VAR NOINIT sections */

#ifdef FRSM_START_SEC_VAR_NOINIT_8BIT
# undef FRSM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef FRSM_STOP_SEC_VAR_NOINIT_8BIT
# undef FRSM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef FRSM_START_SEC_VAR_NOINIT_16BIT
# undef FRSM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef FRSM_STOP_SEC_VAR_NOINIT_16BIT
# undef FRSM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif


#ifdef FRSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef FRSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


#ifdef FRSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef FRSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef FRSM_START_SEC_VAR_ZERO_INIT_8BIT
# undef FRSM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef FRSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef FRSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */



/**********************************************************************************************************************
 *  FrSM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LINSM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef LINSM_START_SEC_CODE
  #undef LINSM_START_SEC_CODE
  #define START_SEC_CODE
#endif
#ifdef LINSM_STOP_SEC_CODE
  #undef LINSM_STOP_SEC_CODE
  #define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef LINSM_START_SEC_CONST_8BIT
# undef LINSM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef LINSM_STOP_SEC_CONST_8BIT
# undef LINSM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef LINSM_START_SEC_CONST_UNSPECIFIED
# undef LINSM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef LINSM_STOP_SEC_CONST_UNSPECIFIED
# undef LINSM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#ifdef LINSM_START_SEC_PBCFG
# undef LINSM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef LINSM_STOP_SEC_PBCFG
# undef LINSM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef LINSM_START_SEC_VAR_NOINIT_8BIT
# undef LINSM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef LINSM_STOP_SEC_VAR_NOINIT_8BIT
# undef LINSM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef LINSM_START_SEC_VAR_NOINIT_16BIT
# undef LINSM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef LINSM_STOP_SEC_VAR_NOINIT_16BIT
# undef LINSM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef LINSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef LINSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef LINSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef LINSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef LINSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef LINSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef LINSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef LINSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef LINSM_START_SEC_VAR_ZERO_INIT_8BIT
# undef LINSM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef LINSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef LINSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  LINSM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETHSM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined ETHSM_START_SEC_CODE
  #undef ETHSM_START_SEC_CODE                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#if defined ETHSM_STOP_SEC_CODE
  #undef ETHSM_STOP_SEC_CODE                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined ETHSM_START_SEC_CONST_8BIT
# undef ETHSM_START_SEC_CONST_8BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined ETHSM_STOP_SEC_CONST_8BIT
# undef ETHSM_STOP_SEC_CONST_8BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined ETHSM_START_SEC_CONST_16BIT
# undef ETHSM_START_SEC_CONST_16BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined ETHSM_STOP_SEC_CONST_16BIT
# undef ETHSM_STOP_SEC_CONST_16BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined ETHSM_START_SEC_CONST_32BIT
# undef ETHSM_START_SEC_CONST_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined ETHSM_STOP_SEC_CONST_32BIT
# undef ETHSM_STOP_SEC_CONST_32BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined ETHSM_START_SEC_CONST_UNSPECIFIED
# undef ETHSM_START_SEC_CONST_UNSPECIFIED           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined ETHSM_STOP_SEC_CONST_UNSPECIFIED
# undef ETHSM_STOP_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined ETHSM_START_SEC_PBCFG
# undef ETHSM_START_SEC_PBCFG                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined ETHSM_STOP_SEC_PBCFG
# undef ETHSM_STOP_SEC_PBCFG                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined ETHSM_START_SEC_PBCFG_ROOT
# undef ETHSM_START_SEC_PBCFG_ROOT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined ETHSM_STOP_SEC_PBCFG_ROOT
# undef ETHSM_STOP_SEC_PBCFG_ROOT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined ETHSM_START_SEC_VAR_NOINIT_8BIT
# undef ETHSM_START_SEC_VAR_NOINIT_8BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHSM_STOP_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_NOINIT_16BIT
# undef ETHSM_START_SEC_VAR_NOINIT_16BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHSM_STOP_SEC_VAR_NOINIT_16BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_NOINIT_32BIT
# undef ETHSM_START_SEC_VAR_NOINIT_32BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHSM_STOP_SEC_VAR_NOINIT_32BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHSM_START_SEC_VAR_NOINIT_UNSPECIFIED      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETHSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_INIT_8BIT
# undef ETHSM_START_SEC_VAR_INIT_8BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_INIT_8BIT
# undef ETHSM_STOP_SEC_VAR_INIT_8BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_INIT_16BIT
# undef ETHSM_START_SEC_VAR_INIT_16BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_INIT_16BIT
# undef ETHSM_STOP_SEC_VAR_INIT_16BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_INIT_32BIT
# undef ETHSM_START_SEC_VAR_INIT_32BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_INIT_32BIT
# undef ETHSM_STOP_SEC_VAR_INIT_32BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_INIT_UNSPECIFIED
# undef ETHSM_START_SEC_VAR_INIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETHSM_START_SEC_VAR_ZERO_INIT_8BIT          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETHSM_STOP_SEC_VAR_ZERO_INIT_8BIT           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_ZERO_INIT_16BIT
# undef ETHSM_START_SEC_VAR_ZERO_INIT_16BIT         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef ETHSM_STOP_SEC_VAR_ZERO_INIT_16BIT          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_ZERO_INIT_32BIT
# undef ETHSM_START_SEC_VAR_ZERO_INIT_32BIT         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined ETHSM_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef ETHSM_STOP_SEC_VAR_ZERO_INIT_32BIT          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined ETHSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined ETHSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHSM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CDDDES START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(CDDDES_START_SEC_CODE)
# undef CDDDES_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(CDDDES_STOP_SEC_CODE)
# undef CDDDES_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(CDDDES_START_SEC_CONST_UNSPECIFIED)
# undef CDDDES_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(CDDDES_STOP_SEC_CONST_UNSPECIFIED)
# undef CDDDES_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(CDDDES_START_SEC_CONST_8BIT)
# undef CDDDES_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined(CDDDES_STOP_SEC_CONST_8BIT)
# undef CDDDES_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(CDDDES_START_SEC_VAR_INIT_8BIT)
# undef CDDDES_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(CDDDES_STOP_SEC_VAR_INIT_8BIT)
# undef CDDDES_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

/* VAR NOINIT sections */

#if defined(CDDDES_START_SEC_VAR_NOINIT_8BIT)
# undef CDDDES_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(CDDDES_STOP_SEC_VAR_NOINIT_8BIT)
# undef CDDDES_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDES_START_SEC_VAR_NOINIT_16BIT)
# undef CDDDES_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(CDDDES_STOP_SEC_VAR_NOINIT_16BIT)
# undef CDDDES_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDES_START_SEC_VAR_NOINIT_32BIT)
# undef CDDDES_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(CDDDES_STOP_SEC_VAR_NOINIT_32BIT)
# undef CDDDES_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDES_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CDDDES_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(CDDDES_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CDDDES_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CDDDES END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CDDDRM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(CDDDRM_START_SEC_CODE)
# undef CDDDRM_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(CDDDRM_STOP_SEC_CODE)
# undef CDDDRM_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(CDDDRM_START_SEC_CONST_8BIT)
# undef CDDDRM_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(CDDDRM_STOP_SEC_CONST_8BIT)
# undef CDDDRM_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(CDDDRM_START_SEC_CONST_16BIT)
# undef CDDDRM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(CDDDRM_STOP_SEC_CONST_16BIT)
# undef CDDDRM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(CDDDRM_START_SEC_CONST_32BIT)
# undef CDDDRM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(CDDDRM_STOP_SEC_CONST_32BIT)
# undef CDDDRM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif
#if defined(CDDDRM_START_SEC_CONST_UNSPECIFIED)
# undef CDDDRM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(CDDDRM_STOP_SEC_CONST_UNSPECIFIED)
# undef CDDDRM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(CDDDRM_START_SEC_VAR_INIT_8BIT)
# undef CDDDRM_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(CDDDRM_STOP_SEC_VAR_INIT_8BIT)
# undef CDDDRM_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif


/* VAR NOINIT sections */

#if defined(CDDDRM_START_SEC_VAR_NOINIT_8BIT)
# undef CDDDRM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(CDDDRM_STOP_SEC_VAR_NOINIT_8BIT)
# undef CDDDRM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDRM_START_SEC_VAR_NOINIT_16BIT)
# undef CDDDRM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(CDDDRM_STOP_SEC_VAR_NOINIT_16BIT)
# undef CDDDRM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDRM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CDDDRM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(CDDDRM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CDDDRM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(CDDDRM_START_SEC_VAR_PBCFG)
# undef CDDDRM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(CDDDRM_STOP_SEC_VAR_PBCFG)
# undef CDDDRM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(CDDDRM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef CDDDRM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(CDDDRM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef CDDDRM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(CDDDRM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef CDDDRM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(CDDDRM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef CDDDRM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NVRAM sections */

#if defined (CDDDRM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED)
# undef CDDDRM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined (CDDDRM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED)
# undef CDDDRM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  DRM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Cdd_AsrCdd START 
 *********************************************************************************************************************/
/* Copy the memory mapping defines for each of your configured CDDs and replace the prefix _CDD with the MSN of your configured CDD as higher case. */

/*******  CODE sections **********************************************************************************************/

#ifdef _CDD_START_SEC_CODE
# undef _CDD_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef _CDD_STOP_SEC_CODE
# undef _CDD_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/* Add additional memory mapping defines for each of you configured CDDs here. */

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

/* FAST CONST sections */

/* Postbuild CFG CONST sections */

/* Root table for postbuild data */

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

/* VAR NOINIT sections */

/* VAR ZERO INIT sections */

/* VAR FAST INIT sections */

/* VAR FAST NOINIT sections */

/* VAR FAST ZERO INIT sections */

/**********************************************************************************************************************
 *  Cdd_AsrCdd END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  MSN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(VX1000IF_START_SEC_CODE)
# undef VX1000IF_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(VX1000IF_STOP_SEC_CODE)
# undef VX1000IF_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(VX1000IF_START_SEC_CODE_FAST)
# undef VX1000IF_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(VX1000IF_STOP_SEC_CODE_FAST)
# undef VX1000IF_STOP_SEC_CODE_FAST  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(VX1000IF_START_SEC_CODE_ISR)
# undef VX1000IF_START_SEC_CODE_ISR  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR     /* mapped to default ISR code section */
#endif
#if defined(VX1000IF_STOP_SEC_CODE_ISR)
# undef VX1000IF_STOP_SEC_CODE_ISR   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(VX1000IF_START_SEC_CONST_8BIT)
# undef VX1000IF_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(VX1000IF_STOP_SEC_CONST_8BIT)
# undef VX1000IF_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(VX1000IF_START_SEC_CONST_16BIT)
# undef VX1000IF_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_CONST_16BIT)
# undef VX1000IF_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_CONST_32BIT)
# undef VX1000IF_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_CONST_32BIT)
# undef VX1000IF_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_CONST_UNSPECIFIED)
# undef VX1000IF_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_CONST_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#if defined(VX1000IF_START_SEC_FAST_CONST_8BIT)
# undef VX1000IF_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_FAST_CONST_8BIT)
# undef VX1000IF_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_FAST_CONST_16BIT)
# undef VX1000IF_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_FAST_CONST_16BIT)
# undef VX1000IF_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_FAST_CONST_32BIT)
# undef VX1000IF_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_FAST_CONST_32BIT)
# undef VX1000IF_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_FAST_CONST_UNSPECIFIED)
# undef VX1000IF_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* NOTE: ASR3.x till ASR4.0.1 only. Root table for postbuild data */
#if defined(VX1000IF_START_SEC_PBCFG_ROOT)
# undef VX1000IF_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(VX1000IF_STOP_SEC_PBCFG_ROOT)
# undef VX1000IF_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* ESCAN00065501 */
/* NOTE: ASR4.0.3 (CFG5) only. The origin root table for postbuild data. Relevant only for EcuM. */
#if defined(VX1000IF_START_SEC_PBCFG_GLOBALROOT)
# undef VX1000IF_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_PBCFG_GLOBALROOT
#endif
#if defined(VX1000IF_STOP_SEC_PBCFG_GLOBALROOT)
# undef VX1000IF_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VX1000IF_START_SEC_PBCFG)
# undef VX1000IF_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(VX1000IF_STOP_SEC_PBCFG)
# undef VX1000IF_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(VX1000IF_START_SEC_VAR_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(VX1000IF_STOP_SEC_VAR_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(VX1000IF_START_SEC_VAR_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(VX1000IF_START_SEC_VAR_NOINIT_8BIT)
# undef VX1000IF_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOINIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOINIT_16BIT)
# undef VX1000IF_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOINIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOINIT_32BIT)
# undef VX1000IF_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOINIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(VX1000IF_START_SEC_VAR_PBCFG)
# undef VX1000IF_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(VX1000IF_STOP_SEC_VAR_PBCFG)
# undef VX1000IF_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(VX1000IF_START_SEC_VAR_ZERO_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_ZERO_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_ZERO_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#if defined(VX1000IF_START_SEC_VAR_FAST_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#if defined(VX1000IF_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef VX1000IF_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef VX1000IF_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef VX1000IF_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef VX1000IF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  MSN END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CCP_START_SEC_CODE 
    #undef CCP_START_SEC_CODE 
    #define START_SEC_CODE
#endif

#ifdef CCP_STOP_SEC_CODE 
    #undef CCP_STOP_SEC_CODE 
    #define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CCP_START_SEC_PBCFG
    #undef CCP_START_SEC_PBCFG
    #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef CCP_STOP_SEC_PBCFG
    #undef CCP_STOP_SEC_PBCFG
    #define STOP_SEC_CONST
#endif

#ifdef CCP_START_SEC_CONST_UNSPECIFIED
    #undef CCP_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef CCP_STOP_SEC_CONST_UNSPECIFIED
    #undef CCP_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST
#endif

#ifdef CCP_START_SEC_CONST_8BIT 
    #undef CCP_START_SEC_CONST_8BIT 
    #define START_SEC_CONST_8BIT
#endif

#ifdef CCP_STOP_SEC_CONST_8BIT 
    #undef CCP_STOP_SEC_CONST_8BIT 
    #define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef CCP_START_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef CCP_START_SEC_VAR_NOINIT_UNSPECIFIED 
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #define STOP_SEC_VAR
#endif

#ifdef CCP_START_SEC_VAR_ZERO_INIT_8BIT 
    #undef CCP_START_SEC_VAR_ZERO_INIT_8BIT 
    #define START_SEC_VAR_ZERO_INIT_8BIT
#endif

#ifdef CCP_STOP_SEC_VAR_ZERO_INIT_8BIT 
    #undef CCP_STOP_SEC_VAR_ZERO_INIT_8BIT 
    #define STOP_SEC_VAR
#endif

#ifdef CCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
    #undef CCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef CCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef CCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  CCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  XCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(XCP_START_SEC_CODE)
# undef XCP_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(XCP_STOP_SEC_CODE)
# undef XCP_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(XCP_START_SEC_CONST_8BIT)
# undef XCP_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(XCP_STOP_SEC_CONST_8BIT)
# undef XCP_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(XCP_START_SEC_CONST_16BIT)
# undef XCP_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(XCP_STOP_SEC_CONST_16BIT)
# undef XCP_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(XCP_START_SEC_CONST_32BIT)
# undef XCP_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(XCP_STOP_SEC_CONST_32BIT)
# undef XCP_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(XCP_START_SEC_CONST_UNSPECIFIED)
# undef XCP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_CONST_UNSPECIFIED)
# undef XCP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#if defined(XCP_START_SEC_PBCFG)
# undef XCP_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(XCP_STOP_SEC_PBCFG)
# undef XCP_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(XCP_START_SEC_VAR_INIT_8BIT)
# undef XCP_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(XCP_STOP_SEC_VAR_INIT_8BIT)
# undef XCP_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(XCP_START_SEC_VAR_INIT_16BIT)
# undef XCP_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_INIT_16BIT)
# undef XCP_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_INIT_32BIT)
# undef XCP_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_INIT_32BIT)
# undef XCP_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_INIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(XCP_START_SEC_VAR_NOINIT_8BIT)
# undef XCP_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOINIT_8BIT)
# undef XCP_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOINIT_16BIT)
# undef XCP_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOINIT_16BIT)
# undef XCP_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOINIT_32BIT)
# undef XCP_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOINIT_32BIT)
# undef XCP_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(XCP_START_SEC_VAR_PBCFG)
# undef XCP_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(XCP_STOP_SEC_VAR_PBCFG)
# undef XCP_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(XCP_START_SEC_VAR_ZERO_INIT_8BIT)
# undef XCP_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(XCP_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef XCP_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_ZERO_INIT_16BIT)
# undef XCP_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef XCP_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_ZERO_INIT_32BIT)
# undef XCP_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef XCP_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOCACHE INIT sections */

#if defined(XCP_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef XCP_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef XCP_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef XCP_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(XCP_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef XCP_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef XCP_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef XCP_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef XCP_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef XCP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR Safe sections */
#if defined (XCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE)
# undef XCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined (XCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE)
# undef XCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  XCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CANXCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CANXCP_START_SEC_CODE
  #undef CANXCP_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif

#ifdef CANXCP_STOP_SEC_CODE
  #undef CANXCP_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CANXCP_START_SEC_CONST_8BIT
  #undef CANXCP_START_SEC_CONST_8BIT
  #define START_SEC_CONST_8BIT                      /* mapped to const 8 bit section */
#endif

#ifdef CANXCP_STOP_SEC_CONST_8BIT
  #undef CANXCP_STOP_SEC_CONST_8BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

#ifdef CANXCP_START_SEC_CONST_UNSPECIFIED
  #undef CANXCP_START_SEC_CONST_UNSPECIFIED
  #define START_SEC_CONST_UNSPECIFIED               /* mapped to const unspecified bit section */
#endif

#ifdef CANXCP_STOP_SEC_CONST_UNSPECIFIED
  #undef CANXCP_STOP_SEC_CONST_UNSPECIFIED
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


/* Post build config unspecified */

#ifdef CANXCP_START_SEC_PBCFG_ROOT
  #undef CANXCP_START_SEC_PBCFG_ROOT
  #define START_SEC_CONST_PBCFG                     /* mapped to const unspecified section */
#endif

#ifdef CANXCP_STOP_SEC_PBCFG_ROOT
  #undef CANXCP_STOP_SEC_PBCFG_ROOT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


#ifdef CANXCP_START_SEC_PBCFG
  #undef CANXCP_START_SEC_PBCFG
  #define START_SEC_CONST_PBCFG                     /* mapped to const unspecified section */
#endif

#ifdef CANXCP_STOP_SEC_PBCFG
  #undef CANXCP_STOP_SEC_PBCFG
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


/*******  VAR sections  **********************************************************************************************/

#ifdef CANXCP_START_SEC_VAR_INIT_8BIT
  #undef CANXCP_START_SEC_VAR_INIT_8BIT
  #define START_SEC_VAR_INIT_8BIT                 /* mapped to uninitialized RAM 8bit section */
#endif

#ifdef CANXCP_STOP_SEC_VAR_INIT_8BIT
  #undef CANXCP_STOP_SEC_VAR_INIT_8BIT
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif


#ifdef CANXCP_START_SEC_VAR_NOINIT_8BIT
  #undef CANXCP_START_SEC_VAR_NOINIT_8BIT
  #define START_SEC_VAR_NOINIT_8BIT                 /* mapped to uninitialized RAM 8bit section */
#endif

#ifdef CANXCP_STOP_SEC_VAR_NOINIT_8BIT
  #undef CANXCP_STOP_SEC_VAR_NOINIT_8BIT
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif


#ifdef CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #define START_SEC_VAR_NOINIT_UNSPECIFIED          /* mapped to uninitialized RAM unspecified section */
#endif

#ifdef CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif


#ifdef CANXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef CANXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef CANXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef CANXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CANXCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FRXCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FRXCP_START_SEC_CODE 
    #undef FRXCP_START_SEC_CODE 
    #define START_SEC_CODE
#endif

#ifdef FRXCP_STOP_SEC_CODE 
    #undef FRXCP_STOP_SEC_CODE 
    #define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FRXCP_START_SEC_CONST_UNSPECIFIED 
    #undef FRXCP_START_SEC_CONST_UNSPECIFIED 
    #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef FRXCP_STOP_SEC_CONST_UNSPECIFIED 
    #undef FRXCP_STOP_SEC_CONST_UNSPECIFIED 
    #define STOP_SEC_CONST
#endif

#ifdef FRXCP_START_SEC_CONST_16BIT 
    #undef FRXCP_START_SEC_CONST_16BIT 
    #define START_SEC_CONST_16BIT
#endif

#ifdef FRXCP_STOP_SEC_CONST_16BIT 
    #undef FRXCP_STOP_SEC_CONST_16BIT 
    #define STOP_SEC_CONST
#endif

#ifdef FRXCP_START_SEC_CONST_8BIT 
    #undef FRXCP_START_SEC_CONST_8BIT 
    #define START_SEC_CONST_8BIT
#endif

#ifdef FRXCP_STOP_SEC_CONST_8BIT 
    #undef FRXCP_STOP_SEC_CONST_8BIT 
    #define STOP_SEC_CONST
#endif

#ifdef FRXCP_START_SEC_PBCFG
    #undef FRXCP_START_SEC_PBCFG
    #define START_SEC_CONST_PBCFG
#endif

#ifdef FRXCP_STOP_SEC_PBCFG
    #undef FRXCP_STOP_SEC_PBCFG
    #define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

#ifdef FRXCP_START_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef FRXCP_START_SEC_VAR_NOINIT_UNSPECIFIED 
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef FRXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef FRXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #define STOP_SEC_VAR
#endif

#ifdef FRXCP_START_SEC_VAR_NOINIT_8BIT 
    #undef FRXCP_START_SEC_VAR_NOINIT_8BIT 
    #define START_SEC_VAR_NOINIT_8BIT
#endif

#ifdef FRXCP_STOP_SEC_VAR_NOINIT_8BIT 
    #undef FRXCP_STOP_SEC_VAR_NOINIT_8BIT 
    #define STOP_SEC_VAR
#endif

#ifdef FRXCP_START_SEC_VAR_PBCFG
    #undef FRXCP_START_SEC_VAR_PBCFG
    #define START_SEC_VAR_PBCFG
#endif

#ifdef FRXCP_STOP_SEC_VAR_PBCFG
    #undef FRXCP_STOP_SEC_VAR_PBCFG
    #define STOP_SEC_VAR
#endif

#ifdef FRXCP_START_SEC_VAR_INIT_8BIT 
    #undef FRXCP_START_SEC_VAR_INIT_8BIT 
    #define START_SEC_VAR_INIT_8BIT
#endif

#ifdef FRXCP_STOP_SEC_VAR_INIT_8BIT 
    #undef FRXCP_STOP_SEC_VAR_INIT_8BIT 
    #define STOP_SEC_VAR
#endif

#ifdef FRXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef FRXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef FRXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef FRXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  FRXCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  TCPIPXCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef TCPIPXCP_START_SEC_CODE
  #undef TCPIPXCP_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif

#ifdef TCPIPXCP_STOP_SEC_CODE
  #undef TCPIPXCP_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef TCPIPXCP_START_SEC_CONST_8BIT
  #undef TCPIPXCP_START_SEC_CONST_8BIT
  #define START_SEC_CONST_8BIT                      /* mapped to const 8 bit section */
#endif

#ifdef TCPIPXCP_STOP_SEC_CONST_8BIT
  #undef TCPIPXCP_STOP_SEC_CONST_8BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

#ifdef TCPIPXCP_START_SEC_CONST_UNSPECIFIED
  #undef TCPIPXCP_START_SEC_CONST_UNSPECIFIED
  #define START_SEC_CONST_UNSPECIFIED               /* mapped to const unspecified bit section */
#endif

#ifdef TCPIPXCP_STOP_SEC_CONST_UNSPECIFIED
  #undef TCPIPXCP_STOP_SEC_CONST_UNSPECIFIED
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


/* Post build config unspecified */

#ifdef TCPIPXCP_START_SEC_PBCFG_ROOT
  #undef TCPIPXCP_START_SEC_PBCFG_ROOT
  #define START_SEC_CONST_PBCFG                     /* mapped to const unspecified section */
#endif

#ifdef TCPIPXCP_STOP_SEC_PBCFG_ROOT
  #undef TCPIPXCP_STOP_SEC_PBCFG_ROOT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


#ifdef TCPIPXCP_START_SEC_PBCFG
  #undef TCPIPXCP_START_SEC_PBCFG
  #define START_SEC_CONST_PBCFG                     /* mapped to const unspecified section */
#endif

#ifdef TCPIPXCP_STOP_SEC_PBCFG
  #undef TCPIPXCP_STOP_SEC_PBCFG
  #define STOP_SEC_CONST                            /* default const stop section */
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */
#ifdef TCPIPXCP_START_SEC_VAR_NOINIT_8BIT
  #undef TCPIPXCP_START_SEC_VAR_NOINIT_8BIT
  #define START_SEC_VAR_NOINIT_8BIT                 /* mapped to uninitialized RAM 8bit section */
#endif

#ifdef TCPIPXCP_STOP_SEC_VAR_NOINIT_8BIT
  #undef TCPIPXCP_STOP_SEC_VAR_NOINIT_8BIT
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif

#ifdef TCPIPXCP_START_SEC_VAR_NOINIT_16BIT
  #undef TCPIPXCP_START_SEC_VAR_NOINIT_16BIT
  #define START_SEC_VAR_NOINIT_16BIT                /* mapped to uninitialized RAM 8bit section */
#endif

#ifdef TCPIPXCP_STOP_SEC_VAR_NOINIT_16BIT
  #undef TCPIPXCP_STOP_SEC_VAR_NOINIT_16BIT
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif

/* Var noinit unspecified */
#ifdef TCPIPXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef TCPIPXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #define START_SEC_VAR_NOINIT_UNSPECIFIED          /* mapped to uninitialized RAM unspecified section */
#endif

#ifdef TCPIPXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef TCPIPXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif

/* VAR INIT sections */
#ifdef TCPIPXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef TCPIPXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef TCPIPXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #undef TCPIPXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE 
    #define STOP_SEC_VAR
#endif

#ifdef TCPIPXCP_START_SEC_VAR_INIT_8BIT
  #undef TCPIPXCP_START_SEC_VAR_INIT_8BIT
  #define START_SEC_VAR_INIT_8BIT                 /* mapped to initialized RAM 8bit section */
#endif

#ifdef TCPIPXCP_STOP_SEC_VAR_INIT_8BIT
  #undef TCPIPXCP_STOP_SEC_VAR_INIT_8BIT
  #define STOP_SEC_VAR                              /* default RAM stop section */
#endif

/**********************************************************************************************************************
 *  TCPIPXCP END
 *********************************************************************************************************************/

/* ==== Start sections mapping ==== */
#ifdef DCM_START_SEC_CONST_8
# undef DCM_START_SEC_CONST_8                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8                                           /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CONST_16
# undef DCM_START_SEC_CONST_16                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16                                          /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CONST_32
# undef DCM_START_SEC_CONST_32                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32                                          /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CONST_UNSPECIFIED
# undef DCM_START_SEC_CONST_UNSPECIFIED                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                                 /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CALIB_8
# undef DCM_START_SEC_CALIB_8                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8                                           /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CALIB_16
# undef DCM_START_SEC_CALIB_16                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16                                          /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CALIB_32
# undef DCM_START_SEC_CALIB_32                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32                                          /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_CALIB_UNSPECIFIED
# undef DCM_START_SEC_CALIB_UNSPECIFIED                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                                 /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_PBCFG
# undef DCM_START_SEC_PBCFG                                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                                       /* mapped to default const section */
#endif

#ifdef DCM_START_SEC_VAR_PBCFG
# undef DCM_START_SEC_VAR_PBCFG                                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG                                         /* mapped to default var section */
#endif

#ifdef DCM_START_SEC_VAR_INIT_8
# undef DCM_START_SEC_VAR_INIT_8                                   	 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8                                        /* mapped to default initialized var section */
#endif

#ifdef DCM_START_SEC_VAR_NO_INIT_8
# undef DCM_START_SEC_VAR_NO_INIT_8                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_8                                     /* mapped to default var section */
#endif

#ifdef DCM_START_SEC_VAR_NO_INIT_16
# undef DCM_START_SEC_VAR_NO_INIT_16                                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_16                                    /* mapped to default var section */
#endif

#ifdef DCM_START_SEC_VAR_NO_INIT_32
# undef DCM_START_SEC_VAR_NO_INIT_32                                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_32                                    /* mapped to default var section */
#endif

#ifdef DCM_START_SEC_VAR_INIT_32
# undef DCM_START_SEC_VAR_INIT_32                                  	 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32                                       /* mapped to default initialized var section */
#endif

#ifdef DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# undef DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_UNSPECIFIED                           /* mapped to default var section */
#endif

#ifdef DCM_START_SEC_CODE
# undef DCM_START_SEC_CODE                                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                              /* mapped to default code section */
#endif

#ifdef DCM_START_SEC_APPL_CODE
# undef DCM_START_SEC_APPL_CODE                                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                              /* mapped to default code section */
#endif

#ifdef DCM_START_SEC_CALLOUT_CODE
# undef DCM_START_SEC_CALLOUT_CODE                                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                              /* mapped to default code section */
#endif

/* ==== Stop sections mapping ==== */
#ifdef DCM_STOP_SEC_CONST_8
# undef DCM_STOP_SEC_CONST_8                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CONST_16
# undef DCM_STOP_SEC_CONST_16                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CONST_32
# undef DCM_STOP_SEC_CONST_32
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CONST_UNSPECIFIED
# undef DCM_STOP_SEC_CONST_UNSPECIFIED                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CALIB_8
# undef DCM_STOP_SEC_CALIB_8                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CALIB_16
# undef DCM_STOP_SEC_CALIB_16                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CALIB_32
# undef DCM_STOP_SEC_CALIB_32
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_CALIB_UNSPECIFIED
# undef DCM_STOP_SEC_CALIB_UNSPECIFIED                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_PBCFG
# undef DCM_STOP_SEC_PBCFG                                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                              /* default const section */
#endif

#ifdef DCM_STOP_SEC_VAR_INIT_8
# undef DCM_STOP_SEC_VAR_INIT_8                                   	 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default initialized var section */
#endif

#ifdef DCM_STOP_SEC_VAR_NO_INIT_8
# undef DCM_STOP_SEC_VAR_NO_INIT_8                                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default var section */
#endif

#ifdef DCM_STOP_SEC_VAR_NO_INIT_16
# undef DCM_STOP_SEC_VAR_NO_INIT_16                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default var section */
#endif

#ifdef DCM_STOP_SEC_VAR_NO_INIT_32
# undef DCM_STOP_SEC_VAR_NO_INIT_32                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default var section */
#endif

#ifdef DCM_STOP_SEC_VAR_INIT_32
# undef DCM_STOP_SEC_VAR_INIT_32                                  	 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default initialized var section */
#endif

#ifdef DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# undef DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default var section */
#endif

#ifdef DCM_STOP_SEC_VAR_PBCFG
# undef DCM_STOP_SEC_VAR_PBCFG                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                                /* default var section */
#endif

#ifdef DCM_STOP_SEC_CODE
# undef DCM_STOP_SEC_CODE                                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                               /* default code section */
#endif

#ifdef DCM_STOP_SEC_APPL_CODE
# undef DCM_STOP_SEC_APPL_CODE                                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                               /* default code section */
#endif

#ifdef DCM_STOP_SEC_CALLOUT_CODE
# undef DCM_STOP_SEC_CALLOUT_CODE                                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                               /* default code section */
#endif

/** DEM **********************************************************************/
/*---------------------START-MODULE-SPECIFIC-START-SECTIONS------------------*/

                                                            /* code sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_START_SEC_CODE)
# undef DEM_START_SEC_CODE                                                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif

#if defined (DEM_START_SEC_CALLOUT_CODE)
# undef DEM_START_SEC_CALLOUT_CODE                                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif

#if defined (DEM_START_SEC_CODE_LIB)
# undef DEM_START_SEC_CODE_LIB                                                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif

                                               /* uninitialized RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_START_SEC_VAR_NOINIT_8BIT)
# undef DEM_START_SEC_VAR_NOINIT_8BIT                                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined (DEM_START_SEC_VAR_NOINIT_16BIT)
# undef DEM_START_SEC_VAR_NOINIT_16BIT                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined (DEM_START_SEC_VAR_NOINIT_32BIT)
# undef DEM_START_SEC_VAR_NOINIT_32BIT                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined (DEM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef DEM_START_SEC_VAR_NOINIT_UNSPECIFIED                                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_VAR_UNCACHED_UNSPECIFIED)
# undef DEM_START_SEC_VAR_UNCACHED_UNSPECIFIED                                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_VAR_NOINIT_UNSPECIFIED_SAT0)
# undef DEM_START_SEC_VAR_NOINIT_UNSPECIFIED_SAT0                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_VAR_NOINIT_UNSPECIFIED_SAT1)
# undef DEM_START_SEC_VAR_NOINIT_UNSPECIFIED_SAT1                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

                                                 /* initialized RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_START_SEC_VAR_INIT_8BIT)
# undef DEM_START_SEC_VAR_INIT_8BIT                                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined (DEM_START_SEC_VAR_INIT_32BIT)
# undef DEM_START_SEC_VAR_INIT_32BIT                                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif

                                              /* post-buildable RAM sections */
/*---------------------------------------------------------------------------*/
#if defined (DEM_START_SEC_VAR_PBCFG)
# undef DEM_START_SEC_VAR_PBCFG                                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif


                                                          /* NV RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_START_SEC_VAR_SAVED_ZONE0_8BIT)
# undef DEM_START_SEC_VAR_SAVED_ZONE0_8BIT                                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif

#if defined (DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED)
# undef DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_CONST_SAVED_RECOVERY_ZONE0)
# undef DEM_START_SEC_CONST_SAVED_RECOVERY_ZONE0                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif

                                                             /* ROM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_START_SEC_CONST_8BIT)
# undef DEM_START_SEC_CONST_8BIT                                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined (DEM_START_SEC_CONST_16BIT)
# undef DEM_START_SEC_CONST_16BIT                                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined (DEM_START_SEC_CONST_32BIT)
# undef DEM_START_SEC_CONST_32BIT                                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined (DEM_START_SEC_CONST_UNSPECIFIED)
# undef DEM_START_SEC_CONST_UNSPECIFIED                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_CONFIG_DATA_UNSPECIFIED)
# undef DEM_START_SEC_CONFIG_DATA_UNSPECIFIED                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_PBCFG_ROOT)
# undef DEM_START_SEC_PBCFG_ROOT                                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif

#if defined (DEM_START_SEC_CALIB_UNSPECIFIED)
# undef DEM_START_SEC_CALIB_UNSPECIFIED                                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif

#if defined (DEM_START_SEC_CALIB_8BIT)
# undef DEM_START_SEC_CALIB_8BIT                                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif

#if defined (DEM_START_SEC_PBCFG)
# undef DEM_START_SEC_PBCFG                                                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif



/*---------------------STOP-MODULE-SPECIFIC-START-SECTIONS-------------------*/

/*---------------------START-MODULE-SPECIFIC-STOP-SECTIONS-------------------*/


                                                            /* code sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_STOP_SEC_CODE)
# undef DEM_STOP_SEC_CODE                                                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

#if defined (DEM_STOP_SEC_CALLOUT_CODE)
# undef DEM_STOP_SEC_CALLOUT_CODE                                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

#if defined (DEM_STOP_SEC_CODE_LIB)
# undef DEM_STOP_SEC_CODE_LIB                                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

                                               /* uninitialized RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_STOP_SEC_VAR_NOINIT_8BIT)
# undef DEM_STOP_SEC_VAR_NOINIT_8BIT                                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif
#if defined (DEM_STOP_SEC_VAR_NOINIT_16BIT)
# undef DEM_STOP_SEC_VAR_NOINIT_16BIT                                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif
#if defined (DEM_STOP_SEC_VAR_NOINIT_32BIT)
# undef DEM_STOP_SEC_VAR_NOINIT_32BIT                                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif
#if defined (DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED                                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (DEM_STOP_SEC_VAR_UNCACHED_UNSPECIFIED)
# undef DEM_STOP_SEC_VAR_UNCACHED_UNSPECIFIED                                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_SAT0)
# undef DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_SAT0                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_SAT1)
# undef DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_SAT1                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


                                                 /* initialized RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_STOP_SEC_VAR_INIT_8BIT)
# undef DEM_STOP_SEC_VAR_INIT_8BIT                                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif
#if defined (DEM_STOP_SEC_VAR_INIT_32BIT)
# undef DEM_STOP_SEC_VAR_INIT_32BIT                                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

                                              /* post-buildable RAM sections */
/*---------------------------------------------------------------------------*/
#if defined (DEM_STOP_SEC_VAR_PBCFG)
# undef DEM_STOP_SEC_VAR_PBCFG                                                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

                                                          /* NV RAM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED)
# undef DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (DEM_STOP_SEC_VAR_SAVED_ZONE0_8BIT)
# undef DEM_STOP_SEC_VAR_SAVED_ZONE0_8BIT                                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (DEM_STOP_SEC_CONST_SAVED_RECOVERY_ZONE0)
# undef DEM_STOP_SEC_CONST_SAVED_RECOVERY_ZONE0                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

                                                             /* ROM sections */
/*---------------------------------------------------------------------------*/

#if defined (DEM_STOP_SEC_CONST_8BIT)
# undef DEM_STOP_SEC_CONST_8BIT                                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif
#if defined (DEM_STOP_SEC_CONST_16BIT)
# undef DEM_STOP_SEC_CONST_16BIT                                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif
#if defined (DEM_STOP_SEC_CONST_32BIT)
# undef DEM_STOP_SEC_CONST_32BIT                                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif
#if defined (DEM_STOP_SEC_CONST_UNSPECIFIED)
# undef DEM_STOP_SEC_CONST_UNSPECIFIED                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined (DEM_STOP_SEC_PBCFG_ROOT)
# undef DEM_STOP_SEC_PBCFG_ROOT                                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined (DEM_STOP_SEC_CALIB_UNSPECIFIED)
# undef DEM_STOP_SEC_CALIB_UNSPECIFIED                                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif
#if defined (DEM_STOP_SEC_CALIB_8BIT)
# undef DEM_STOP_SEC_CALIB_8BIT                                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined (DEM_STOP_SEC_PBCFG)
# undef DEM_STOP_SEC_PBCFG                                                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#include "Dem_MemMap.h"

/*---------------------STOP-MODULE-SPECIFIC-STOP-SECTIONS--------------------*/
/** End of DEM ***************************************************************/



/**********************************************************************************************************************
 *  J1939DCM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef J1939DCM_START_SEC_CODE
  #undef J1939DCM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE      /* mapped to default code section */
#endif
#ifdef J1939DCM_STOP_SEC_CODE
  #undef J1939DCM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE      /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef J1939DCM_START_SEC_CONST_8BIT
# undef J1939DCM_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT      /* mapped to default const 8bit section */
#endif
#ifdef J1939DCM_STOP_SEC_CONST_8BIT
# undef J1939DCM_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#ifdef J1939DCM_START_SEC_CONST_16BIT
# undef J1939DCM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef J1939DCM_STOP_SEC_CONST_16BIT
# undef J1939DCM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939DCM_START_SEC_CONST_32BIT
# undef J1939DCM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#ifdef J1939DCM_STOP_SEC_CONST_32BIT
# undef J1939DCM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939DCM_START_SEC_CONST_UNSPECIFIED
# undef J1939DCM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef J1939DCM_STOP_SEC_CONST_UNSPECIFIED
# undef J1939DCM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#ifdef J1939DCM_START_SEC_PBCFG
# undef J1939DCM_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef J1939DCM_STOP_SEC_PBCFG
# undef J1939DCM_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939DCM_START_SEC_PBCFG_ROOT
# undef J1939DCM_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef J1939DCM_STOP_SEC_PBCFG_ROOT
# undef J1939DCM_STOP_SEC_PBCFG_ROOT       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef J1939DCM_START_SEC_VAR_INIT_8BIT
# undef J1939DCM_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#ifdef J1939DCM_STOP_SEC_VAR_INIT_8BIT
# undef J1939DCM_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#ifdef J1939DCM_START_SEC_VAR_INIT_16BIT
# undef J1939DCM_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef J1939DCM_STOP_SEC_VAR_INIT_16BIT
# undef J1939DCM_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939DCM_START_SEC_VAR_INIT_32BIT
# undef J1939DCM_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef J1939DCM_STOP_SEC_VAR_INIT_32BIT
# undef J1939DCM_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939DCM_START_SEC_VAR_INIT_UNSPECIFIED
# undef J1939DCM_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef J1939DCM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef J1939DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef J1939DCM_START_SEC_VAR_NOINIT_8BIT
# undef J1939DCM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef J1939DCM_STOP_SEC_VAR_NOINIT_8BIT
# undef J1939DCM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939DCM_START_SEC_VAR_NOINIT_16BIT
# undef J1939DCM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef J1939DCM_STOP_SEC_VAR_NOINIT_16BIT
# undef J1939DCM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939DCM_START_SEC_VAR_NOINIT_32BIT
# undef J1939DCM_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef J1939DCM_STOP_SEC_VAR_NOINIT_32BIT
# undef J1939DCM_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939DCM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef J1939DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#ifdef J1939DCM_START_SEC_VAR_PBCFG
# undef J1939DCM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef J1939DCM_STOP_SEC_VAR_PBCFG
# undef J1939DCM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif



/**********************************************************************************************************************
 *  J1939DCM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvAdc_VttCanoe01Asr START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

/*******  CONST sections  ********************************************************************************************/

/*******  VAR sections  **********************************************************************************************/


/**********************************************************************************************************************
 *  DrvAdc_VttCanoe01Asr END
 *********************************************************************************************************************/

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/*        STARTSINGLE_OF_MULTIPLE_COMMENT      */


/**********************************************************************************************************************
 *  Can START 
 *********************************************************************************************************************/

#if defined (CAN_REMAP_PB_SECTIONS)        /* PRQA S 0883 */ /* Appears only while testing */
/* for none PB variants the code will be mapped to standard sections, so the generated and static code can ignore the variant handling */
# if defined (CAN_START_SEC_PBCFG)
#  undef CAN_START_SEC_PBCFG                /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define CAN_START_SEC_CONST_UNSPECIFIED
# endif
# if defined (CAN_STOP_SEC_PBCFG)
#  undef CAN_STOP_SEC_PBCFG                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define CAN_STOP_SEC_CONST_UNSPECIFIED
# endif

# if defined (CAN_START_SEC_VAR_PBCFG)
#  undef CAN_START_SEC_VAR_PBCFG            /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED
# endif
# if defined (CAN_STOP_SEC_VAR_PBCFG)
#  undef CAN_STOP_SEC_VAR_PBCFG             /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# endif
#endif

/*******  CODE sections **********************************************************************************************/

#ifdef CAN_START_SEC_CODE                             /* CAN code */
# undef CAN_START_SEC_CODE                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                               /* mapped to default code section */
#endif
#ifdef CAN_STOP_SEC_CODE
# undef CAN_STOP_SEC_CODE                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

#ifdef CAN_START_SEC_STATIC_CODE                      /* CAN static code */
# undef CAN_START_SEC_STATIC_CODE                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                               /* mapped to default code section */
#endif
#ifdef CAN_STOP_SEC_STATIC_CODE
# undef CAN_STOP_SEC_STATIC_CODE                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

#ifdef CAN_START_SEC_CODE_APPL                        /* Appl code (generic precopy)*/
# undef CAN_START_SEC_CODE_APPL                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                               /* mapped to default code section */
#endif
#ifdef CAN_STOP_SEC_CODE_APPL
# undef CAN_STOP_SEC_CODE_APPL                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CAN_START_SEC_CONST_8BIT                       /* Const 8bit */
# undef CAN_START_SEC_CONST_8BIT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                         /* mapped to default const 8bit section */
#endif
#ifdef CAN_STOP_SEC_CONST_8BIT
# undef CAN_STOP_SEC_CONST_8BIT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CAN_START_SEC_CONST_16BIT                      /* Const 16bit */
# undef CAN_START_SEC_CONST_16BIT                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT                        /* mapped to default const 16bit section */
#endif
#ifdef CAN_STOP_SEC_CONST_16BIT
# undef CAN_STOP_SEC_CONST_16BIT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CAN_START_SEC_CONST_32BIT                      /* Const 32bit */
# undef CAN_START_SEC_CONST_32BIT                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT                        /* mapped to default const 32bit section */
#endif
#ifdef CAN_STOP_SEC_CONST_32BIT
# undef CAN_STOP_SEC_CONST_32BIT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CAN_START_SEC_CONST_UNSPECIFIED                /* Const unspecified */
# undef CAN_START_SEC_CONST_UNSPECIFIED                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                  /* mapped to default const unspec section */
#endif
#ifdef CAN_STOP_SEC_CONST_UNSPECIFIED
# undef CAN_STOP_SEC_CONST_UNSPECIFIED                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* FAST CONST sections */
/* unused */

/* Postbuild CFG CONST sections */

/* Root table for postbuild data (not relevant for CFG5 AutoSar4.0.3) */
#ifdef CAN_START_SEC_PBCFG_ROOT                       /* Generated Postbuild Root Table */
# undef CAN_START_SEC_PBCFG_ROOT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                        /* mapped to default const postbuild section */
#endif
#ifdef CAN_STOP_SEC_PBCFG_ROOT
# undef CAN_STOP_SEC_PBCFG_ROOT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CAN_START_SEC_PBCFG                            /* Generated Postbuild */
# undef CAN_START_SEC_PBCFG                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                        /* mapped to default const postbuild section */
#endif
#ifdef CAN_STOP_SEC_PBCFG
# undef CAN_STOP_SEC_PBCFG                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef CAN_START_SEC_VAR_INIT_UNSPECIFIED             /* init RAM*/
# undef CAN_START_SEC_VAR_INIT_UNSPECIFIED              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED               /* mapped to default init var section */
#endif
#ifdef CAN_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef CAN_STOP_SEC_VAR_INIT_UNSPECIFIED               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef CAN_START_SEC_VAR_NOINIT_UNSPECIFIED           /* uninit RAM*/
# undef CAN_START_SEC_VAR_NOINIT_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED             /* mapped to default uninit var section */
#endif
#ifdef CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CAN_START_SEC_VAR_NOINIT_8BIT                  /* uninit 8bit RAM*/
# undef CAN_START_SEC_VAR_NOINIT_8BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED             /* mapped to default uninit var section */
#endif
#ifdef CAN_STOP_SEC_VAR_NOINIT_8BIT
# undef CAN_STOP_SEC_VAR_NOINIT_8BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CAN_START_SEC_VAR_NOINIT_16BIT                  /* uninit 16bit RAM*/
# undef CAN_START_SEC_VAR_NOINIT_16BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED             /* mapped to default uninit var section */
#endif
#ifdef CAN_STOP_SEC_VAR_NOINIT_16BIT
# undef CAN_STOP_SEC_VAR_NOINIT_16BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CAN_START_SEC_VAR_NOINIT_32BIT                  /* uninit 32bit RAM*/
# undef CAN_START_SEC_VAR_NOINIT_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED             /* mapped to default uninit var section */
#endif
#ifdef CAN_STOP_SEC_VAR_NOINIT_32BIT
# undef CAN_STOP_SEC_VAR_NOINIT_32BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR PBCFG sections */

#ifdef CAN_START_SEC_VAR_PBCFG           /* Generated Postbuild RAM*/
# undef CAN_START_SEC_VAR_PBCFG            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG             /* mapped to default uninit var section */
#endif
#ifdef CAN_STOP_SEC_VAR_PBCFG
# undef CAN_STOP_SEC_VAR_PBCFG             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */
/* unused */

/* VAR FAST INIT sections */
/* unused */

/* VAR FAST NOINIT sections */
/* unused */

/* VAR FAST ZERO INIT sections */
/* unused */

/**********************************************************************************************************************
 *  Can END
 *********************************************************************************************************************/

/*  STOPSINGLE_OF_MULTIPLE_COMMENT  */
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


/**********************************************************************************************************************
 *  CRYPTO_30_LIBCV START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CRYPTO_30_LIBCV_START_SEC_CODE
# undef CRYPTO_30_LIBCV_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_CODE
# undef CRYPTO_30_LIBCV_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_APPL_CODE
# undef CRYPTO_30_LIBCV_START_SEC_APPL_CODE                    /* mapped to default code section */
# define START_SEC_CODE
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_APPL_CODE
# undef CRYPTO_30_LIBCV_STOP_SEC_APPL_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_CONST_UNSPECIFIED
# undef CRYPTO_30_LIBCV_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_CONST_UNSPECIFIED
# undef CRYPTO_30_LIBCV_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif

#ifdef CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_32BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_32BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CRYPTO_30_LIBCV END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  DrvDio_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/


/**********************************************************************************************************************
 *  DrvDio_VttCanoe01Asr END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvEep_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/
#ifdef EEP_START_SEC_CODE
# undef EEP_START_SEC_CODE
# define START_SEC_CODE
#endif

#ifdef EEP_STOP_SEC_CODE
# undef EEP_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/


/**********************************************************************************************************************
 *  DrvEep_VttCanoe01Asr END
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETH_30_VTT START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined ETH_30_VTT_START_SEC_CODE
  #undef ETH_30_VTT_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#if defined ETH_30_VTT_STOP_SEC_CODE
  #undef ETH_30_VTT_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#if defined ETH_30_VTT_START_SEC_CODE_ISR
  #undef ETH_30_VTT_START_SEC_CODE_ISR
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#if defined ETH_30_VTT_STOP_SEC_CODE_ISR
  #undef ETH_30_VTT_STOP_SEC_CODE_ISR
  #define STOP_SEC_CODE_ISR                             /* default code stop section */
#endif

#if defined ETH_30_VTT_START_SEC_APPL_CODE
  #undef ETH_30_VTT_START_SEC_APPL_CODE
  #define START_SEC_APPL_CODE                        /* mapped to default APPL code section */
#endif
#if defined ETH_30_VTT_STOP_SEC_CODE
  #undef ETH_30_VTT_STOP_SEC_APPL_CODE
  #define STOP_SEC_APPL_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined ETH_30_VTT_START_SEC_CONST_8BIT
# undef ETH_30_VTT_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_CONST_8BIT
# undef ETH_30_VTT_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_VTT_START_SEC_CONST_16BIT
# undef ETH_30_VTT_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_CONST_16BIT
# undef ETH_30_VTT_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_VTT_START_SEC_CONST_32BIT
# undef ETH_30_VTT_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_CONST_32BIT
# undef ETH_30_VTT_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_VTT_START_SEC_CONST_UNSPECIFIED
# undef ETH_30_VTT_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined ETH_30_VTT_STOP_SEC_CONST_UNSPECIFIED
# undef ETH_30_VTT_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#if defined ETH_30_VTT_START_SEC_PBCFG
# undef ETH_30_VTT_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_30_VTT_STOP_SEC_PBCFG
# undef ETH_30_VTT_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#if defined ETH_30_VTT_START_SEC_PBCFG_ROOT
# undef ETH_30_VTT_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_30_VTT_STOP_SEC_PBCFG_ROOT
# undef ETH_30_VTT_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_BOOLEAN
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_BOOLEAN
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_8BIT
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETH_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETH_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_16BIT
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_16BIT
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_32BIT
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_32BIT
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef ETH_30_VTT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_30_VTT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETH_30_VTT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_30_VTT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_DESCRIPTOR
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_DESCRIPTOR

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss=".EthDesc"
   */
# endif

# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_DESCRIPTOR
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_DESCRIPTOR

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss="default"
   */
# endif

# define STOP_SEC_VAR
# endif

#if defined ETH_30_VTT_START_SEC_VAR_NOINIT_BUFFER
# undef ETH_30_VTT_START_SEC_VAR_NOINIT_BUFFER

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss=".EthBuffers"
   */
# endif

# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_VTT_STOP_SEC_VAR_NOINIT_BUFFER
# undef ETH_30_VTT_STOP_SEC_VAR_NOINIT_BUFFER

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss="default"
   */
# endif

# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  ETH_30_VTT END
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  ETH START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETH_START_SEC_CODE
  #undef ETH_START_SEC_CODE                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef ETH_STOP_SEC_CODE
  #undef ETH_STOP_SEC_CODE                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef ETH_START_SEC_CODE_ISR
  #undef ETH_START_SEC_CODE_ISR                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#ifdef ETH_STOP_SEC_CODE_ISR
  #undef ETH_STOP_SEC_CODE_ISR                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined ETH_START_SEC_CONST_8BIT
# undef ETH_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#if defined ETH_STOP_SEC_CONST_8BIT
# undef ETH_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_START_SEC_CONST_16BIT
# undef ETH_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#if defined ETH_STOP_SEC_CONST_16BIT
# undef ETH_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_START_SEC_CONST_32BIT
# undef ETH_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#if defined ETH_STOP_SEC_CONST_32BIT
# undef ETH_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_START_SEC_CONST_UNSPECIFIED
# undef ETH_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined ETH_STOP_SEC_CONST_UNSPECIFIED
# undef ETH_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#if defined ETH_START_SEC_PBCFG
# undef ETH_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_STOP_SEC_PBCFG
# undef ETH_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#if defined ETH_START_SEC_PBCFG_ROOT
# undef ETH_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_STOP_SEC_PBCFG_ROOT
# undef ETH_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined ETH_START_SEC_VAR_NOINIT_8BIT
# undef ETH_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined ETH_STOP_SEC_VAR_NOINIT_8BIT
# undef ETH_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETH_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETH_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_START_SEC_VAR_NOINIT_16BIT
# undef ETH_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined ETH_STOP_SEC_VAR_NOINIT_16BIT
# undef ETH_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_START_SEC_VAR_NOINIT_32BIT
# undef ETH_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined ETH_STOP_SEC_VAR_NOINIT_32BIT
# undef ETH_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef ETH_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETH_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


#ifdef ETH_START_SEC_VAR_NOINIT_BUFFER
# undef ETH_START_SEC_VAR_NOINIT_BUFFER             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_STOP_SEC_VAR_NOINIT_BUFFER
# undef ETH_STOP_SEC_VAR_NOINIT_BUFFER
# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  ETH END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETH_30_CORE START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined ETH_30_CORE_START_SEC_CODE
  #undef ETH_30_CORE_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#if defined ETH_30_CORE_STOP_SEC_CODE
  #undef ETH_30_CORE_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#if defined ETH_30_CORE_START_SEC_CODE_ISR
  #undef ETH_30_CORE_START_SEC_CODE_ISR
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#if defined ETH_30_CORE_STOP_SEC_CODE_ISR
  #undef ETH_30_CORE_STOP_SEC_CODE_ISR
  #define STOP_SEC_CODE_ISR                             /* default code stop section */
#endif

#if defined ETH_30_CORE_START_SEC_APPL_CODE
  #undef ETH_30_CORE_START_SEC_APPL_CODE
  #define START_SEC_APPL_CODE                        /* mapped to default APPL code section */
#endif
#if defined ETH_30_CORE_STOP_SEC_CODE
  #undef ETH_30_CORE_STOP_SEC_APPL_CODE
  #define STOP_SEC_APPL_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined ETH_30_CORE_START_SEC_CONST_8BIT
# undef ETH_30_CORE_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_CONST_8BIT
# undef ETH_30_CORE_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_CORE_START_SEC_CONST_16BIT
# undef ETH_30_CORE_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_CONST_16BIT
# undef ETH_30_CORE_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_CORE_START_SEC_CONST_32BIT
# undef ETH_30_CORE_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_CONST_32BIT
# undef ETH_30_CORE_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#if defined ETH_30_CORE_START_SEC_CONST_UNSPECIFIED
# undef ETH_30_CORE_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined ETH_30_CORE_STOP_SEC_CONST_UNSPECIFIED
# undef ETH_30_CORE_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#if defined ETH_30_CORE_START_SEC_PBCFG
# undef ETH_30_CORE_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_30_CORE_STOP_SEC_PBCFG
# undef ETH_30_CORE_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#if defined ETH_30_CORE_START_SEC_PBCFG_ROOT
# undef ETH_30_CORE_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#if defined ETH_30_CORE_STOP_SEC_PBCFG_ROOT
# undef ETH_30_CORE_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_BOOLEAN
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_BOOLEAN
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_8BIT
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_8BIT
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETH_30_CORE_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_30_CORE_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETH_30_CORE_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETH_30_CORE_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_16BIT
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_16BIT
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_32BIT
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_32BIT
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef ETH_30_CORE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_30_CORE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETH_30_CORE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETH_30_CORE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_DESCRIPTOR
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_DESCRIPTOR

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss=".EthDesc"
   */
# endif

# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_DESCRIPTOR
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_DESCRIPTOR

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss="default"
   */
# endif

# define STOP_SEC_VAR
# endif

#if defined ETH_30_CORE_START_SEC_VAR_NOINIT_BUFFER
# undef ETH_30_CORE_START_SEC_VAR_NOINIT_BUFFER

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss=".EthBuffers"
   */
# endif

# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETH_30_CORE_STOP_SEC_VAR_NOINIT_BUFFER
# undef ETH_30_CORE_STOP_SEC_VAR_NOINIT_BUFFER

# if defined _MICROSOFT_C_VTT_
   /* VTT usecase - nothing to do here */
# else
#  error "For Integrator: This section must be aligned to a 64 BYTES border!"
   /* Pragma must be placed here. */
   /* Example for GHS compiler:  
#  pragma ghs section bss="default"
   */
# endif

# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  ETH_30_CORE END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvFls_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/
#ifdef FLS_START_SEC_CODE
# undef FLS_START_SEC_CODE
# define START_SEC_CODE
#endif

#ifdef FLS_STOP_SEC_CODE
# undef FLS_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/


/**********************************************************************************************************************
 *  DrvFls_VttCanoe01Asr END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  FR START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FR_START_SEC_CODE
# undef FR_START_SEC_CODE
# define START_SEC_CODE                                               /* mapped to default code section */
#endif
#ifdef FR_STOP_SEC_CODE
# undef FR_STOP_SEC_CODE
# define STOP_SEC_CODE                                                /* default code stop section */
#endif


#ifdef FR_START_SEC_CODE_ISR
# undef FR_START_SEC_CODE_ISR
# define START_SEC_CODE_ISR                                           /* mapped to default ISR code section */
#endif
#ifdef FR_STOP_SEC_CODE_ISR
# undef FR_STOP_SEC_CODE_ISR
# define STOP_SEC_CODE                                                /* default code stop section */
#endif

#ifdef FR_APPL_START_SEC_CODE
# undef FR_APPL_START_SEC_CODE
# define START_SEC_CODE
#endif

#ifdef FR_APPL_STOP_SEC_CODE
# undef FR_APPL_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FR_START_SEC_CONST_32BIT
# undef FR_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef FR_STOP_SEC_CONST_32BIT
# undef FR_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef FR_START_SEC_CONST_16BIT
# undef FR_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef FR_STOP_SEC_CONST_16BIT
# undef FR_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef FR_START_SEC_CONST_8BIT
# undef FR_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef FR_STOP_SEC_CONST_8BIT
# undef FR_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif


#ifdef FR_START_SEC_CONST_UNSPECIFIED
# undef FR_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef FR_STOP_SEC_CONST_UNSPECIFIED
# undef FR_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#ifdef FR_START_SEC_PBCFG_ROOT
# undef FR_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef FR_STOP_SEC_PBCFG_ROOT
# undef FR_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

#ifdef FR_START_SEC_PBCFG
# undef FR_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef FR_STOP_SEC_PBCFG
# undef FR_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef FR_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FR_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef FR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR FAST NOINIT sections */

#ifdef FR_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef FR_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#ifdef FR_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef FR_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef FR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef FR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  FR END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvGpt_VttCanoe01Asr START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

/*******  CONST sections  ********************************************************************************************/

/*******  VAR sections  **********************************************************************************************/


/**********************************************************************************************************************
 *  DrvGpt_VttCanoe01Asr END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvIcu_VttCanoe01Asr START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

/*******  CONST sections  ********************************************************************************************/

/*******  VAR sections  **********************************************************************************************/


/**********************************************************************************************************************
 *  DrvIcu_VttCanoe01Asr END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  LIN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(LIN_START_SEC_CODE)
# undef LIN_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(LIN_STOP_SEC_CODE)
# undef LIN_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(LIN_START_SEC_CODE_FAST)
# undef LIN_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(LIN_STOP_SEC_CODE_FAST)
# undef LIN_STOP_SEC_CODE_FAST  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(LIN_START_SEC_CODE_ISR)
# undef LIN_START_SEC_CODE_ISR  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR     /* mapped to default ISR code section */
#endif
#if defined(LIN_STOP_SEC_CODE_ISR)
# undef LIN_STOP_SEC_CODE_ISR   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(LIN_START_SEC_CONST_8BIT)
# undef LIN_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(LIN_STOP_SEC_CONST_8BIT)
# undef LIN_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(LIN_START_SEC_CONST_16BIT)
# undef LIN_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(LIN_STOP_SEC_CONST_16BIT)
# undef LIN_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_CONST_32BIT)
# undef LIN_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(LIN_STOP_SEC_CONST_32BIT)
# undef LIN_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_CONST_UNSPECIFIED)
# undef LIN_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_CONST_UNSPECIFIED)
# undef LIN_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#if defined(LIN_START_SEC_FAST_CONST_8BIT)
# undef LIN_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_8BIT)
# undef LIN_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_16BIT)
# undef LIN_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_16BIT)
# undef LIN_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_32BIT)
# undef LIN_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_32BIT)
# undef LIN_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_UNSPECIFIED)
# undef LIN_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef LIN_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* NOTE: ASR3.x till ASR4.0.1 only. Root table for postbuild data */
#if defined(LIN_START_SEC_PBCFG_ROOT)
# undef LIN_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(LIN_STOP_SEC_PBCFG_ROOT)
# undef LIN_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* ESCAN00065501 */
/* NOTE: ASR4.0.3 (CFG5) only. The origin root table for postbuild data. Relevant only for EcuM. */
#if defined(LIN_START_SEC_PBCFG_GLOBALROOT)
# undef LIN_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_PBCFG_GLOBALROOT
#endif
#if defined(LIN_STOP_SEC_PBCFG_GLOBALROOT)
# undef LIN_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_PBCFG)
# undef LIN_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(LIN_STOP_SEC_PBCFG)
# undef LIN_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(LIN_START_SEC_VAR_INIT_8BIT)
# undef LIN_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(LIN_START_SEC_VAR_INIT_16BIT)
# undef LIN_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_INIT_32BIT)
# undef LIN_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(LIN_START_SEC_VAR_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(LIN_START_SEC_VAR_PBCFG)
# undef LIN_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(LIN_STOP_SEC_VAR_PBCFG)
# undef LIN_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#if defined(LIN_START_SEC_VAR_FAST_INIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  LIN END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  LIN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(LIN_START_SEC_CODE)
# undef LIN_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(LIN_STOP_SEC_CODE)
# undef LIN_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(LIN_START_SEC_CODE_FAST)
# undef LIN_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(LIN_STOP_SEC_CODE_FAST)
# undef LIN_STOP_SEC_CODE_FAST  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(LIN_START_SEC_CODE_ISR)
# undef LIN_START_SEC_CODE_ISR  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR     /* mapped to default ISR code section */
#endif
#if defined(LIN_STOP_SEC_CODE_ISR)
# undef LIN_STOP_SEC_CODE_ISR   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(LIN_START_SEC_CONST_8BIT)
# undef LIN_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(LIN_STOP_SEC_CONST_8BIT)
# undef LIN_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(LIN_START_SEC_CONST_16BIT)
# undef LIN_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(LIN_STOP_SEC_CONST_16BIT)
# undef LIN_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_CONST_32BIT)
# undef LIN_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(LIN_STOP_SEC_CONST_32BIT)
# undef LIN_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_CONST_UNSPECIFIED)
# undef LIN_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_CONST_UNSPECIFIED)
# undef LIN_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#if defined(LIN_START_SEC_FAST_CONST_8BIT)
# undef LIN_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_8BIT)
# undef LIN_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_16BIT)
# undef LIN_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_16BIT)
# undef LIN_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_32BIT)
# undef LIN_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_32BIT)
# undef LIN_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_FAST_CONST_UNSPECIFIED)
# undef LIN_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef LIN_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* NOTE: ASR3.x till ASR4.0.1 only. Root table for postbuild data */
#if defined(LIN_START_SEC_PBCFG_ROOT)
# undef LIN_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(LIN_STOP_SEC_PBCFG_ROOT)
# undef LIN_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* ESCAN00065501 */
/* NOTE: ASR4.0.3 (CFG5) only. The origin root table for postbuild data. Relevant only for EcuM. */
#if defined(LIN_START_SEC_PBCFG_GLOBALROOT)
# undef LIN_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_PBCFG_GLOBALROOT
#endif
#if defined(LIN_STOP_SEC_PBCFG_GLOBALROOT)
# undef LIN_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LIN_START_SEC_PBCFG)
# undef LIN_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(LIN_STOP_SEC_PBCFG)
# undef LIN_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(LIN_START_SEC_VAR_INIT_8BIT)
# undef LIN_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(LIN_START_SEC_VAR_INIT_16BIT)
# undef LIN_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_INIT_32BIT)
# undef LIN_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(LIN_START_SEC_VAR_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(LIN_START_SEC_VAR_PBCFG)
# undef LIN_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(LIN_STOP_SEC_VAR_PBCFG)
# undef LIN_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#if defined(LIN_START_SEC_VAR_FAST_INIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef LIN_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef LIN_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  LIN END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvMcu_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/



/**********************************************************************************************************************
 *  DrvMcu_VttCanoe01Asr END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvPort_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/



/**********************************************************************************************************************
 *  DrvPort_VttCanoe01Asr END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvPwm_VttCanoe01Asr START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

/*******  CONST sections  ********************************************************************************************/

/*******  VAR sections  **********************************************************************************************/


/**********************************************************************************************************************
 *  DrvPwm_VttCanoe01Asr END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  Sent_30_Icu START
 **********************************************************************************************************************/

/*******  CODE sections  **********************************************************************************************/

#if defined (SENT_30_ICU_START_SEC_CODE)
#  undef SENT_30_ICU_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CODE
#endif

#if defined (SENT_30_ICU_STOP_SEC_CODE)
#  undef SENT_30_ICU_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CODE
#endif

/*******  CONST sections  *********************************************************************************************/

#if defined (SENT_30_ICU_START_SEC_CONST_UNSPECIFIED)
#  undef SENT_30_ICU_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_UNSPECIFIED
#endif

#if defined (SENT_30_ICU_STOP_SEC_CONST_UNSPECIFIED)
#  undef SENT_30_ICU_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST
#endif

/*******  VAR sections  ***********************************************************************************************/

#if defined (SENT_30_ICU_START_SEC_VAR_ZERO_INIT_8BIT)
#  undef SENT_30_ICU_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_ZERO_INIT_8BIT
#endif

#if defined (SENT_30_ICU_STOP_SEC_VAR_ZERO_INIT_8BIT)
#  undef SENT_30_ICU_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR
#endif

#if defined (SENT_30_ICU_START_SEC_VAR_NOINIT_8BIT)
#  undef SENT_30_ICU_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_8BIT
#endif

#if defined (SENT_30_ICU_STOP_SEC_VAR_NOINIT_8BIT)
#  undef SENT_30_ICU_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR
#endif

#if defined (SENT_30_ICU_START_SEC_VAR_NOINIT_16BIT)
#  undef SENT_30_ICU_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_16BIT
#endif

#if defined (SENT_30_ICU_STOP_SEC_VAR_NOINIT_16BIT)
#  undef SENT_30_ICU_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR
#endif

#if defined (SENT_30_ICU_START_SEC_VAR_NOINIT_32BIT)
#  undef SENT_30_ICU_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_32BIT
#endif

#if defined (SENT_30_ICU_STOP_SEC_VAR_NOINIT_32BIT)
#  undef SENT_30_ICU_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR
#endif

#if defined (SENT_30_ICU_START_SEC_VAR_NOINIT_UNSPECIFIED)
#  undef SENT_30_ICU_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (SENT_30_ICU_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
#  undef SENT_30_ICU_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  Sent_30_Icu END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvSpi_VttCanoe01Asr START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef SPI_START_SEC_CODE
# undef SPI_START_SEC_CODE
# define START_SEC_CODE
#endif

#ifdef SPI_STOP_SEC_CODE
# undef SPI_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/*******  VAR sections  **********************************************************************************************/


/**********************************************************************************************************************
 *  DrvSpi_VttCanoe01Asr END
 *********************************************************************************************************************/

#if defined (CANTRCV_30_GENERICCAN_START_SEC_CONST_8BIT) /* PRQA S 0883 */ /* Appears only while testing */
#  undef CANTRCV_30_GENERICCAN_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_8BIT             /* mapped to default 8-bit const section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_CONST_16BIT)
#  undef CANTRCV_30_GENERICCAN_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_16BIT             /* mapped to default 16-bit const section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_CONST_32BIT) 
#  undef CANTRCV_30_GENERICCAN_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_32BIT             /* mapped to default 32-bit const section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_CONST_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_UNSPECIFIED      /* unspecified section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_PBCFG_ROOT) 
#  undef CANTRCV_30_GENERICCAN_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_PBCFG     /* pb section */

#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_PBCFG)
#  undef CANTRCV_30_GENERICCAN_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CONST_PBCFG      /* pb section */

#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_8BIT)
#  undef CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_8BIT        /* mapped to default 8-bit uninit var section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_VAR_INIT_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_INIT_UNSPECIFIED            /* mapped to default init var section */
#endif

#if defined (CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_VAR_NOINIT_UNSPECIFIED                /* mapped to default var section */
#endif


#if defined (CANTRCV_30_GENERICCAN_START_SEC_CODE)
#  undef CANTRCV_30_GENERICCAN_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define START_SEC_CODE                   /* mapped to default code section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_CONST_8BIT) /* PRQA S 0883 */ /* Appears only while testing */
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_CONST_16BIT)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_CONST_32BIT)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif


#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_CONST_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_8BIT)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR                    /* mapped to default uninit var stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_VAR_INIT_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR                   /* mapped to default uninit var stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_VAR                   /* mapped to default code section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_CODE)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CODE                    /* mapped to default code stop section */
#endif

#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_PBCFG_ROOT)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif


#if defined (CANTRCV_30_GENERICCAN_STOP_SEC_PBCFG)
#  undef CANTRCV_30_GENERICCAN_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
#  define STOP_SEC_CONST                  /* mapped to default const stop section */
#endif


/**********************************************************************************************************************
 *  FRTRCV START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FRTRCV_30___YOUR_TRCV___START_SEC_CODE 
    #undef FRTRCV_30___YOUR_TRCV___START_SEC_CODE 
    #define START_SEC_CODE
#endif

#ifdef FRTRCV_30___YOUR_TRCV___STOP_SEC_CODE 
    #undef FRTRCV_30___YOUR_TRCV___STOP_SEC_CODE 
    #define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_8BIT 
    #undef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_8BIT 
    #define START_SEC_CONST_8BIT
#endif

#ifdef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_8BIT 
    #undef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_8BIT 
    #define STOP_SEC_CONST
#endif

#ifdef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_32BIT 
    #undef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_32BIT 
    #define START_SEC_CONST_32BIT
#endif

#ifdef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_32BIT 
    #undef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_32BIT 
    #define STOP_SEC_CONST
#endif

#ifdef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_UNSPECIFIED 
    #undef FRTRCV_30___YOUR_TRCV___START_SEC_CONST_UNSPECIFIED 
    #define START_SEC_CONST_8BIT
#endif

#ifdef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_UNSPECIFIED 
    #undef FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_UNSPECIFIED 
    #define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR sections */

#ifdef FRTRCV_30___YOUR_TRCV___START_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef FRTRCV_30___YOUR_TRCV___START_SEC_VAR_NOINIT_UNSPECIFIED 
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef FRTRCV_30___YOUR_TRCV___STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #undef FRTRCV_30___YOUR_TRCV___STOP_SEC_VAR_NOINIT_UNSPECIFIED 
    #define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  FRTRCV END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LIN TRANSCEIVER GENERIC DRIVER START
 *********************************************************************************************************************/
/*******  CODE sections **********************************************************************************************/

#if defined(LINTRCV_30_GENERIC_START_SEC_CODE)
# undef LINTRCV_30_GENERIC_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_CODE)
# undef LINTRCV_30_GENERIC_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(LINTRCV_30_GENERIC_START_SEC_CONST_8BIT)
# undef LINTRCV_30_GENERIC_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_CONST_8BIT)
# undef LINTRCV_30_GENERIC_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(LINTRCV_30_GENERIC_START_SEC_CONST_32BIT)
# undef LINTRCV_30_GENERIC_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_CONST_32BIT)
# undef LINTRCV_30_GENERIC_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


#if defined(LINTRCV_30_GENERIC_START_SEC_CONST_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_CONST_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(LINTRCV_30_GENERIC_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_8BIT)
# undef LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  LIN TRANSCEIVER GENERIC DRIVER END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  ETHTRCV 30 VTT START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETHTRCV_30_VTT_START_SEC_CODE
  #undef ETHTRCV_30_VTT_START_SEC_CODE                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE                                    /* mapped to default code section */
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_CODE
  #undef ETHTRCV_30_VTT_STOP_SEC_CODE                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                                     /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef ETHTRCV_30_VTT_START_SEC_CONST_8BIT
# undef ETHTRCV_30_VTT_START_SEC_CONST_8BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_CONST_8BIT
# undef ETHTRCV_30_VTT_STOP_SEC_CONST_8BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_CONST_16BIT
# undef ETHTRCV_30_VTT_START_SEC_CONST_16BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_CONST_16BIT
# undef ETHTRCV_30_VTT_STOP_SEC_CONST_16BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_CONST_32BIT
# undef ETHTRCV_30_VTT_START_SEC_CONST_32BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_CONST_32BIT
# undef ETHTRCV_30_VTT_STOP_SEC_CONST_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_CONST_UNSPECIFIED
# undef ETHTRCV_30_VTT_START_SEC_CONST_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_CONST_UNSPECIFIED
# undef ETHTRCV_30_VTT_STOP_SEC_CONST_UNSPECIFIED           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_PBCFG
# undef ETHTRCV_30_VTT_START_SEC_PBCFG                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_PBCFG
# undef ETHTRCV_30_VTT_STOP_SEC_PBCFG                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_PBCFG_ROOT
# undef ETHTRCV_30_VTT_START_SEC_PBCFG_ROOT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_PBCFG_ROOT
# undef ETHTRCV_30_VTT_STOP_SEC_PBCFG_ROOT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_8BIT
# undef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_8BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_8BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTRCV_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_16BIT
# undef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_16BIT           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_16BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_32BIT
# undef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_32BIT           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_32BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_UNSPECIFIED     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_UNSPECIFIED      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_VTT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHTRCV_30_VTT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_VTT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHTRCV_30_VTT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHTRCV 30 VTT END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETHTRCV_30_CORE START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETHTRCV_30_CORE_START_SEC_CODE
  #undef ETHTRCV_30_CORE_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_CODE
  #undef ETHTRCV_30_CORE_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef ETHTRCV_30_CORE_START_SEC_CONST_8BIT
# undef ETHTRCV_30_CORE_START_SEC_CONST_8BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_CONST_8BIT
# undef ETHTRCV_30_CORE_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_CONST_16BIT
# undef ETHTRCV_30_CORE_START_SEC_CONST_16BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_CONST_16BIT
# undef ETHTRCV_30_CORE_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_CONST_32BIT
# undef ETHTRCV_30_CORE_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_CONST_32BIT
# undef ETHTRCV_30_CORE_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_CONST_UNSPECIFIED
# undef ETHTRCV_30_CORE_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_CONST_UNSPECIFIED
# undef ETHTRCV_30_CORE_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_PBCFG
# undef ETHTRCV_30_CORE_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_PBCFG
# undef ETHTRCV_30_CORE_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_PBCFG_ROOT
# undef ETHTRCV_30_CORE_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_PBCFG_ROOT
# undef ETHTRCV_30_CORE_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_8BIT
# undef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTRCV_30_CORE_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_16BIT
# undef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_32BIT
# undef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTRCV_30_CORE_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef ETHTRCV_30_CORE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHTRCV_30_CORE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETHTRCV_30_CORE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHTRCV_30_CORE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHTRCV_30_CORE END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DrvWd_VttCanoe01Asr START 
 **********************************************************************************************************************/

/*******  CODE sections ***********************************************************************************************/

/*******  CONST sections  *********************************************************************************************/

/*******  VAR sections  ***********************************************************************************************/


/**********************************************************************************************************************
 *  DrvWd_VttCanoe01Asr END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  EcuAb_AsrIoHwAb START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef IOHWAB_START_SEC_CODE
    #undef IOHWAB_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef IOHWAB_STOP_SEC_CODE
    #undef IOHWAB_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef IOHWAB_START_SEC_CONST_32
    #undef IOHWAB_START_SEC_CONST_32
    #define START_SEC_CONST_32
#endif
#ifdef IOHWAB_STOP_SEC_CONST_32
    #undef IOHWAB_STOP_SEC_CONST_32
    #define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef IOHWAB_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef IOHWAB_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#endif
#ifdef IOHWAB_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef IOHWAB_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  EcuAb_AsrIoHwAb END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  PDUR START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef PDUR_START_SEC_CODE
# undef PDUR_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef PDUR_STOP_SEC_CODE
# undef PDUR_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef PDUR_START_SEC_CONST_8BIT
# undef PDUR_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef PDUR_STOP_SEC_CONST_8BIT
# undef PDUR_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef PDUR_START_SEC_CONST_16BIT
# undef PDUR_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef PDUR_STOP_SEC_CONST_16BIT
# undef PDUR_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef PDUR_START_SEC_CONST_32BIT
# undef PDUR_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef PDUR_STOP_SEC_CONST_32BIT
# undef PDUR_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef PDUR_START_SEC_CONST_UNSPECIFIED
# undef PDUR_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef PDUR_STOP_SEC_CONST_UNSPECIFIED
# undef PDUR_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/* FAST CONST sections */


/* Postbuild CFG CONST sections */

/* Root table for postbuild data */
#ifdef PDUR_START_SEC_PBCFG
# undef PDUR_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef PDUR_STOP_SEC_PBCFG
# undef PDUR_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef PDUR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef PDUR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef PDUR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef PDUR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef PDUR_START_SEC_VAR_ZERO_INIT_8BIT
# undef PDUR_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef PDUR_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef PDUR_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef PDUR_START_SEC_VAR_NOINIT_8BIT
# undef PDUR_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef PDUR_STOP_SEC_VAR_NOINIT_8BIT
# undef PDUR_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef PDUR_START_SEC_VAR_NOINIT_16BIT
# undef PDUR_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef PDUR_STOP_SEC_VAR_NOINIT_16BIT
# undef PDUR_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef PDUR_START_SEC_VAR_NOINIT_32BIT
# undef PDUR_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef PDUR_STOP_SEC_VAR_NOINIT_32BIT
# undef PDUR_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif


#ifdef PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef PDUR_START_SEC_BUFFER_VAR_NOINIT_8BIT
# undef PDUR_START_SEC_BUFFER_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef PDUR_STOP_SEC_BUFFER_VAR_NOINIT_8BIT
# undef PDUR_STOP_SEC_BUFFER_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

/* Postbuild CFG VAR sections */

#ifdef PDUR_START_SEC_VAR_PBCFG
# undef PDUR_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG
#endif
#ifdef PDUR_STOP_SEC_VAR_PBCFG
# undef PDUR_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */



/* VAR FAST INIT sections */



/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */



/**********************************************************************************************************************
 *  PDUR END
 *********************************************************************************************************************/


/***********************************************************************************************************************
 *  MIRROR START 
 **********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef MIRROR_START_SEC_CODE
  #undef MIRROR_START_SEC_CODE                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE
#endif
#ifdef MIRROR_STOP_SEC_CODE
  #undef MIRROR_STOP_SEC_CODE                          /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
  #define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef MIRROR_START_SEC_CONST_8BIT
# undef MIRROR_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_CONST_8BIT
#endif
#ifdef MIRROR_STOP_SEC_CONST_8BIT
# undef MIRROR_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define STOP_SEC_CONST
#endif

#ifdef MIRROR_START_SEC_CONST_32BIT
# undef MIRROR_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_CONST_32BIT
#endif
#ifdef MIRROR_STOP_SEC_CONST_32
# undef MIRROR_STOP_SEC_CONST_32 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef MIRROR_START_SEC_CONST_UNSPECIFIED
# undef MIRROR_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef MIRROR_STOP_SEC_CONST_UNSPECIFIED
# undef MIRROR_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef MIRROR_START_SEC_VAR_NOINIT_8BIT
# undef MIRROR_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef MIRROR_STOP_SEC_VAR_NOINIT_8BIT
# undef MIRROR_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define STOP_SEC_VAR
#endif

#ifdef MIRROR_START_SEC_VAR_NOINIT_16BIT
# undef MIRROR_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef MIRROR_STOP_SEC_VAR_NOINIT_16BIT
# undef MIRROR_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define STOP_SEC_VAR
#endif

#ifdef MIRROR_START_SEC_VAR_NOINIT_32BIT
# undef MIRROR_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef MIRROR_STOP_SEC_VAR_NOINIT_32BIT
# undef MIRROR_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define STOP_SEC_VAR
#endif

#ifdef MIRROR_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef MIRROR_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef MIRROR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef MIRROR_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */
#ifdef MIRROR_START_SEC_VAR_ZERO_INIT_8BIT
# undef MIRROR_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef MIRROR_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef MIRROR_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef MIRROR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef MIRROR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef MIRROR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef MIRROR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* Postbuild CFG VAR sections */
#ifdef MIRROR_START_SEC_VAR_PBCFG
# undef MIRROR_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG
#endif
#ifdef MIRROR_STOP_SEC_VAR_PBCFG
# undef MIRROR_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR
#endif

#ifdef MIRROR_START_SEC_PBCFG
# undef MIRROR_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define START_SEC_CONST_PBCFG
#endif
#ifdef MIRROR_STOP_SEC_PBCFG
# undef MIRROR_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */ 
# define STOP_SEC_CONST
#endif

/***********************************************************************************************************************
 *  MIRROR END
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LIN INTERFACE START
 *********************************************************************************************************************/

/* Code */
#ifdef LINIF_START_SEC_CODE
  #undef LINIF_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef LINIF_STOP_SEC_CODE
  #undef LINIF_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/* Const 8 bit */
#ifdef LINIF_START_SEC_CONST_8BIT
  #undef LINIF_START_SEC_CONST_8BIT
  #define START_SEC_CONST_8BIT                      /* mapped to const 8 bit section */
#endif
#ifdef LINIF_STOP_SEC_CONST_8BIT
  #undef LINIF_STOP_SEC_CONST_8BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const 16 bit */
#ifdef LINIF_START_SEC_CONST_16BIT
  #undef LINIF_START_SEC_CONST_16BIT
  #define START_SEC_CONST_16BIT                      /* mapped to const 16 bit section */
#endif
#ifdef LINIF_STOP_SEC_CONST_16BIT
  #undef LINIF_STOP_SEC_CONST_16BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const 32 bit */
#ifdef LINIF_START_SEC_CONST_32BIT
  #undef LINIF_START_SEC_CONST_32BIT
  #define START_SEC_CONST_32BIT                      /* mapped to const 32 bit section */
#endif
#ifdef LINIF_STOP_SEC_CONST_32BIT
  #undef LINIF_STOP_SEC_CONST_32BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const unspecified */
#ifdef LINIF_START_SEC_CONST_UNSPECIFIED
  #undef LINIF_START_SEC_CONST_UNSPECIFIED
  #define START_SEC_CONST_UNSPECIFIED               /* mapped to const unspecified section */
#endif
#ifdef LINIF_STOP_SEC_CONST_UNSPECIFIED
  #undef LINIF_STOP_SEC_CONST_UNSPECIFIED
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Post build config unspecified */
#ifdef LINIF_START_SEC_PBCFG
  #undef LINIF_START_SEC_PBCFG
  #define START_SEC_CONST_PBCFG                     /* mapped to post build section */
#endif
#ifdef LINIF_STOP_SEC_PBCFG
  #undef LINIF_STOP_SEC_PBCFG
  #define STOP_SEC_CONST                            /* default post build stop section */
#endif

/* Var noinit unspecified */
#ifdef LINIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef LINIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #define START_SEC_VAR_NOINIT_UNSPECIFIED          /* mapped to uninitialized var unspecified section */
#endif
#ifdef LINIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef LINIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

/* Var noinit 8 bit */
#ifdef LINIF_START_SEC_VAR_NOINIT_8BIT
  #undef LINIF_START_SEC_VAR_NOINIT_8BIT
  #define START_SEC_VAR_NOINIT_8BIT                 /* mapped to uninitialized var 8 bit section */
#endif
#ifdef LINIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef LINIF_STOP_SEC_VAR_NOINIT_8BIT
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

/* Var zero init 8 bit */
#ifdef LINIF_START_SEC_VAR_ZERO_INIT_8BIT
  #undef LINIF_START_SEC_VAR_ZERO_INIT_8BIT
  #define START_SEC_VAR_ZERO_INIT_8BIT              /* mapped to zero initialized var unspecified section */
#endif
#ifdef LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
  #undef LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

/* Post build config unspecified */
#ifdef LINIF_START_SEC_VAR_PBCFG
  #undef LINIF_START_SEC_VAR_PBCFG
  #define START_SEC_VAR_PBCFG                       /* mapped to post build section */
#endif
#ifdef LINIF_STOP_SEC_VAR_PBCFG
  #undef LINIF_STOP_SEC_VAR_PBCFG
  #define STOP_SEC_VAR                              /* default post build stop section */
#endif

/**********************************************************************************************************************
 *  LIN INTERFACE END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LIN TP START
 *********************************************************************************************************************/

/* Code */
#ifdef LINTP_START_SEC_CODE
  #undef LINTP_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef LINTP_STOP_SEC_CODE
  #undef LINTP_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/* Const 8 bit */
#ifdef LINTP_START_SEC_CONST_8BIT
  #undef LINTP_START_SEC_CONST_8BIT
  #define START_SEC_CONST_8BIT                      /* mapped to const 8 bit section */
#endif
#ifdef LINTP_STOP_SEC_CONST_8BIT
  #undef LINTP_STOP_SEC_CONST_8BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const 16 bit */
#ifdef LINTP_START_SEC_CONST_16BIT
  #undef LINTP_START_SEC_CONST_16BIT
  #define START_SEC_CONST_16BIT                      /* mapped to const 16 bit section */
#endif
#ifdef LINTP_STOP_SEC_CONST_16BIT
  #undef LINTP_STOP_SEC_CONST_16BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const 32 bit */
#ifdef LINTP_START_SEC_CONST_32BIT
  #undef LINTP_START_SEC_CONST_32BIT
  #define START_SEC_CONST_32BIT                      /* mapped to const 32 bit section */
#endif
#ifdef LINTP_STOP_SEC_CONST_32BIT
  #undef LINTP_STOP_SEC_CONST_32BIT
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Const unspecified */
#ifdef LINTP_START_SEC_CONST_UNSPECIFIED
  #undef LINTP_START_SEC_CONST_UNSPECIFIED
  #define START_SEC_CONST_UNSPECIFIED               /* mapped to const unspecified section */
#endif
#ifdef LINTP_STOP_SEC_CONST_UNSPECIFIED
  #undef LINTP_STOP_SEC_CONST_UNSPECIFIED
  #define STOP_SEC_CONST                            /* default const stop section */
#endif

/* Post build config unspecified */
#ifdef LINTP_START_SEC_PBCFG
  #undef LINTP_START_SEC_PBCFG
  #define START_SEC_CONST_PBCFG                     /* mapped to post build section */
#endif
#ifdef LINTP_STOP_SEC_PBCFG
  #undef LINTP_STOP_SEC_PBCFG
  #define STOP_SEC_CONST                            /* default post build stop section */
#endif

/* Var noinit unspecified */
#ifdef LINTP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef LINTP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #define START_SEC_VAR_NOINIT_UNSPECIFIED          /* mapped to uninitialized var unspecified section */
#endif
#ifdef LINTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef LINTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

/* Var noinit 8 bit */
#ifdef LINTP_START_SEC_VAR_NOINIT_8BIT
  #undef LINTP_START_SEC_VAR_NOINIT_8BIT
  #define START_SEC_VAR_NOINIT_8BIT                 /* mapped to uninitialized var 8 bit section */
#endif
#ifdef LINTP_STOP_SEC_VAR_NOINIT_8BIT
  #undef LINTP_STOP_SEC_VAR_NOINIT_8BIT
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

/* Var zero init 8 bit */
#ifdef LINTP_START_SEC_VAR_ZERO_INIT_8BIT
  #undef LINTP_START_SEC_VAR_ZERO_INIT_8BIT
  #define START_SEC_VAR_ZERO_INIT_8BIT              /* mapped to zero initialized var unspecified section */
#endif
#ifdef LINTP_STOP_SEC_VAR_ZERO_INIT_8BIT
  #undef LINTP_STOP_SEC_VAR_ZERO_INIT_8BIT
  #define STOP_SEC_VAR                              /* default var stop section */
#endif

#ifdef LINTP_START_SEC_VAR_PBCFG
  #undef LINTP_START_SEC_VAR_PBCFG
  #define START_SEC_VAR_PBCFG                       /* mapped to post build section */
#endif
#ifdef LINTP_STOP_SEC_VAR_PBCFG
  #undef LINTP_STOP_SEC_VAR_PBCFG
  #define STOP_SEC_VAR                            /* default post build stop section */
#endif
/**********************************************************************************************************************
 *  LIN TP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  WDGIF START
 *********************************************************************************************************************/

#include "WdgIf_MemMap.h"

/*******  CODE sections **********************************************************************************************/

#if defined WDGIF_START_SEC_CODE
# undef WDGIF_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined WDGIF_STOP_SEC_CODE
# undef WDGIF_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

#if defined WDGIF_START_SEC_CONST_8BIT
# undef WDGIF_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined WDGIF_STOP_SEC_CONST_8BIT
# undef WDGIF_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGIF_START_SEC_CONST_16BIT
# undef WDGIF_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined WDGIF_STOP_SEC_CONST_16BIT
# undef WDGIF_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGIF_START_SEC_CONST_32BIT
# undef WDGIF_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined WDGIF_STOP_SEC_CONST_32BIT
# undef WDGIF_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGIF_START_SEC_CONST_UNSPECIFIED
# undef WDGIF_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined WDGIF_STOP_SEC_CONST_UNSPECIFIED
# undef WDGIF_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined WDGIF_START_SEC_VAR_INIT_8BIT
# undef WDGIF_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_INIT_8BIT
# undef WDGIF_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_INIT_16BIT
# undef WDGIF_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_INIT_16BIT
# undef WDGIF_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_INIT_32BIT
# undef WDGIF_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_INIT_32BIT
# undef WDGIF_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_INIT_UNSPECIFIED
# undef WDGIF_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined WDGIF_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef WDGIF_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined WDGIF_START_SEC_VAR_NOINIT_8BIT
# undef WDGIF_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_NOINIT_8BIT
# undef WDGIF_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_NOINIT_16BIT
# undef WDGIF_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_NOINIT_16BIT
# undef WDGIF_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_NOINIT_32BIT
# undef WDGIF_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_NOINIT_32BIT
# undef WDGIF_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef WDGIF_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined WDGIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef WDGIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO_INIT sections */

#if defined WDGIF_START_SEC_VAR_ZERO_INIT_8BIT
# undef WDGIF_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef WDGIF_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_ZERO_INIT_16BIT
# undef WDGIF_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef WDGIF_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_ZERO_INIT_32BIT
# undef WDGIF_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined WDGIF_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef WDGIF_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef WDGIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined WDGIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef WDGIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  WDGIF END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CANIF START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef CANIF_START_SEC_CODE
# undef CANIF_START_SEC_CODE                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef CANIF_STOP_SEC_CODE
# undef CANIF_STOP_SEC_CODE                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                             /* default code stop section */
#endif


#ifdef CANIF_START_SEC_APPL_CODE
# undef CANIF_START_SEC_APPL_CODE                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                            /* mapped to default code section */
#endif

#ifdef CANIF_STOP_SEC_APPL_CODE
# undef CANIF_STOP_SEC_APPL_CODE                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CANIF_START_SEC_CONST_8BIT
# undef CANIF_START_SEC_CONST_8BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                      /* mapped to const 8 bit section */
#endif
#ifdef CANIF_STOP_SEC_CONST_8BIT
# undef CANIF_STOP_SEC_CONST_8BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                            /* default const stop section */
#endif

#ifdef CANIF_START_SEC_CONST_16BIT
# undef CANIF_START_SEC_CONST_16BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef CANIF_STOP_SEC_CONST_16BIT
# undef CANIF_STOP_SEC_CONST_16BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                             /* default const stop section */
#endif


/* Const 32bit */

#ifdef CANIF_START_SEC_CONST_32BIT
# undef CANIF_START_SEC_CONST_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT                      /* mapped to const 32 bit section */
#endif
#ifdef CANIF_STOP_SEC_CONST_32BIT
# undef CANIF_STOP_SEC_CONST_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                            /* default const stop section */
#endif


/* Const unspecified */

#ifdef CANIF_START_SEC_CONST_UNSPECIFIED
# undef CANIF_START_SEC_CONST_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED               /* mapped to const unspecified section */
#endif
#ifdef CANIF_STOP_SEC_CONST_UNSPECIFIED
# undef CANIF_STOP_SEC_CONST_UNSPECIFIED           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                            /* default const stop section */
#endif


/* FAST CONST sections */

#ifdef CANIF_START_SEC_CONST_FAST_8BIT
# undef CANIF_START_SEC_CONST_FAST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_FAST_8BIT
#endif
#ifdef CANIF_STOP_SEC_CONST_FAST_8BIT
# undef CANIF_STOP_SEC_CONST_FAST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CANIF_START_SEC_CONST_FAST_16BIT
# undef CANIF_START_SEC_CONST_FAST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_FAST_16BIT
#endif
#ifdef CANIF_STOP_SEC_CONST_FAST_16BIT
# undef CANIF_STOP_SEC_CONST_FAST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CANIF_START_SEC_CONST_FAST_32BIT
# undef CANIF_START_SEC_CONST_FAST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_FAST_32BIT
#endif
#ifdef CANIF_STOP_SEC_CONST_FAST_32BIT
# undef CANIF_STOP_SEC_CONST_FAST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef CANIF_START_SEC_CONST_FAST_UNSPECIFIED
# undef CANIF_START_SEC_CONST_FAST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_FAST_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_CONST_FAST_UNSPECIFIED
# undef CANIF_STOP_SEC_CONST_FAST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#ifdef CANIF_START_SEC_PBCFG
# undef CANIF_START_SEC_PBCFG                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                     /* mapped to const unspecified section */
#endif
#ifdef CANIF_STOP_SEC_PBCFG
# undef CANIF_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef CANIF_START_SEC_VAR_INIT_8BIT
# undef CANIF_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#ifdef CANIF_STOP_SEC_VAR_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#ifdef CANIF_START_SEC_VAR_INIT_16BIT
# undef CANIF_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_INIT_32BIT
# undef CANIF_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef CANIF_START_SEC_VAR_NOINIT_8BIT
# undef CANIF_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOINIT_8BIT
# undef CANIF_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOINIT_16BIT
# undef CANIF_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOINIT_16BIT
# undef CANIF_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOINIT_32BIT
# undef CANIF_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOINIT_32BIT
# undef CANIF_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#ifdef CANIF_START_SEC_VAR_PBCFG
# undef CANIF_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef CANIF_STOP_SEC_VAR_PBCFG
# undef CANIF_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef CANIF_START_SEC_VAR_ZERO_INIT_8BIT
# undef CANIF_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_ZERO_INIT_16BIT
# undef CANIF_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_ZERO_INIT_32BIT
# undef CANIF_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#ifdef CANIF_START_SEC_VAR_FAST_INIT_8BIT
# undef CANIF_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_INIT_16BIT
# undef CANIF_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_INIT_32BIT
# undef CANIF_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#ifdef CANIF_START_SEC_VAR_FAST_NOINIT_8BIT
# undef CANIF_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_NOINIT_8BIT
# undef CANIF_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_NOINIT_16BIT
# undef CANIF_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_NOINIT_16BIT
# undef CANIF_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_NOINIT_32BIT
# undef CANIF_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_NOINIT_32BIT
# undef CANIF_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#ifdef CANIF_START_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef CANIF_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_ZERO_INIT_16BIT
# undef CANIF_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_ZERO_INIT_32BIT
# undef CANIF_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#ifdef CANIF_START_SEC_VAR_NOCACHE_INIT_8BIT
# undef CANIF_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_INIT_16BIT
# undef CANIF_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_INIT_32BIT
# undef CANIF_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#ifdef CANIF_START_SEC_VAR_NOCACHE_NOINIT_8BIT
# undef CANIF_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_NOINIT_16BIT
# undef CANIF_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_NOINIT_32BIT
# undef CANIF_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#ifdef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
# undef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
# undef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
# undef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
# undef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
# undef CANIF_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#ifdef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
# undef CANIF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CANIF END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CRYIF START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CRYIF_START_SEC_CODE
# undef CRYIF_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef CRYIF_STOP_SEC_CODE
# undef CRYIF_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CRYIF_START_SEC_CONST_UNSPECIFIED
# undef CRYIF_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef CRYIF_STOP_SEC_CONST_UNSPECIFIED
# undef CRYIF_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */


/* VAR ZERO INIT sections */

#ifdef CRYIF_START_SEC_VAR_ZERO_INIT_8BIT
# undef CRYIF_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef CRYIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CRYIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CRYIF END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  If_AsrIfEa START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef EA_START_SEC_CODE
    #undef EA_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef EA_STOP_SEC_CODE
    #undef EA_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef EA_START_SEC_CONST_8BIT
    #undef EA_START_SEC_CONST_8BIT
    #define START_SEC_CONST_8BIT
#endif
#ifdef EA_STOP_SEC_CONST_8BIT
    #undef EA_STOP_SEC_CONST_8BIT
    #define STOP_SEC_CONST
#endif

#ifdef EA_START_SEC_CONST_16BIT
    #undef EA_START_SEC_CONST_16BIT
    #define START_SEC_CONST_16BIT
#endif
#ifdef EA_STOP_SEC_CONST_16BIT
    #undef EA_STOP_SEC_CONST_16BIT
    #define STOP_SEC_CONST
#endif

#ifdef EA_START_SEC_CONST_32BIT
    #undef EA_START_SEC_CONST_32BIT
    #define START_SEC_CONST_32BIT
#endif
#ifdef EA_STOP_SEC_CONST_32BIT
    #undef EA_STOP_SEC_CONST_32BIT
    #define STOP_SEC_CONST
#endif

#ifdef EA_START_SEC_CONST_UNSPECIFIED
    #undef EA_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef EA_STOP_SEC_CONST_UNSPECIFIED
    #undef EA_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST
#endif


/*******  VAR sections  ********************************************************************************************/

/* VAR NOINIT sections */

#ifdef EA_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef EA_START_SEC_VAR_NOINIT_UNSPECIFIED
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/* VAR FAST sections */

#ifdef EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#ifdef EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  If_AsrIfEa END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETHIF START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined ETHIF_START_SEC_CODE
  #undef ETHIF_START_SEC_CODE                       /* PRQA S 0841 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#if defined ETHIF_STOP_SEC_CODE
  #undef ETHIF_STOP_SEC_CODE                        /* PRQA S 0841 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#if defined ETHIF_START_SEC_CODE_ISR
  #undef ETHIF_START_SEC_CODE_ISR                   /* PRQA S 0841 */
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#if defined ETHIF_STOP_SEC_CODE_ISR
  #undef ETHIF_STOP_SEC_CODE_ISR                    /* PRQA S 0841 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined ETHIF_START_SEC_CONST_8BIT
# undef ETHIF_START_SEC_CONST_8BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_8BIT
#endif
#if defined ETHIF_STOP_SEC_CONST_8BIT
# undef ETHIF_STOP_SEC_CONST_8BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined ETHIF_START_SEC_CONST_16BIT
# undef ETHIF_START_SEC_CONST_16BIT                 /* PRQA S 0841 */
# define START_SEC_CONST_16BIT
#endif
#if defined ETHIF_STOP_SEC_CONST_16BIT
# undef ETHIF_STOP_SEC_CONST_16BIT                  /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined ETHIF_START_SEC_CONST_32BIT
# undef ETHIF_START_SEC_CONST_32BIT                 /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#if defined ETHIF_STOP_SEC_CONST_32BIT
# undef ETHIF_STOP_SEC_CONST_32BIT                  /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined ETHIF_START_SEC_CONST_UNSPECIFIED
# undef ETHIF_START_SEC_CONST_UNSPECIFIED           /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined ETHIF_STOP_SEC_CONST_UNSPECIFIED
# undef ETHIF_STOP_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined ETHIF_START_SEC_PBCFG
# undef ETHIF_START_SEC_PBCFG                       /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#if defined ETHIF_STOP_SEC_PBCFG
# undef ETHIF_STOP_SEC_PBCFG                        /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined ETHIF_START_SEC_PBCFG_ROOT
# undef ETHIF_START_SEC_PBCFG_ROOT                  /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#if defined ETHIF_STOP_SEC_PBCFG_ROOT
# undef ETHIF_STOP_SEC_PBCFG_ROOT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined ETHIF_START_SEC_VAR_NOINIT_8BIT
# undef ETHIF_START_SEC_VAR_NOINIT_8BIT             /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined ETHIF_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHIF_STOP_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined ETHIF_START_SEC_VAR_NOINIT_16BIT
# undef ETHIF_START_SEC_VAR_NOINIT_16BIT            /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined ETHIF_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHIF_STOP_SEC_VAR_NOINIT_16BIT             /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined ETHIF_START_SEC_VAR_NOINIT_32BIT
# undef ETHIF_START_SEC_VAR_NOINIT_32BIT            /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined ETHIF_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHIF_STOP_SEC_VAR_NOINIT_32BIT             /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined ETHIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHIF_START_SEC_VAR_NOINIT_UNSPECIFIED      /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined ETHIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined ETHIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined ETHIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED      /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHIF END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FEE START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FEE_START_SEC_CODE
# undef FEE_START_SEC_CODE
# define START_SEC_CODE                                               /* mapped to default code section */
#endif
#ifdef FEE_STOP_SEC_CODE
# undef FEE_STOP_SEC_CODE
# define STOP_SEC_CODE                                                /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FEE_START_SEC_CONST_UNSPECIFIED
# undef FEE_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef FEE_STOP_SEC_CONST_UNSPECIFIED
# undef FEE_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef FEE_START_SEC_APPL_CONFIG_UNSPECIFIED
# undef FEE_START_SEC_APPL_CONFIG_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef FEE_STOP_SEC_APPL_CONFIG_UNSPECIFIED
# undef FEE_STOP_SEC_APPL_CONFIG_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef FEE_START_SEC_VAR_INIT_UNSPECIFIED
# undef FEE_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif
/**********************************************************************************************************************
 *  FEE END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FRIF START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FRIF_START_SEC_CODE
  #undef FRIF_START_SEC_CODE                        /* PRQA S 0841 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef FRIF_STOP_SEC_CODE
  #undef FRIF_STOP_SEC_CODE                         /* PRQA S 0841 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef FRIF_START_SEC_APPL_CODE
  #undef FRIF_START_SEC_APPL_CODE                   /* PRQA S 0841 */
  #define START_SEC_CODE                            /* mapped to default appl code section */
#endif
#ifdef FRIF_STOP_SEC_APPL_CODE
  #undef FRIF_STOP_SEC_APPL_CODE                    /* PRQA S 0841 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FRIF_START_SEC_CONST_32BIT
# undef FRIF_START_SEC_CONST_32BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#ifdef FRIF_STOP_SEC_CONST_32BIT
# undef FRIF_STOP_SEC_CONST_32BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#ifdef FRIF_START_SEC_CONST_UNSPECIFIED
# undef FRIF_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef FRIF_STOP_SEC_CONST_UNSPECIFIED  
# undef FRIF_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/* Postbuild CFG CONST sections */

#ifdef FRIF_START_SEC_PBCFG
# undef FRIF_START_SEC_PBCFG                        /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef FRIF_STOP_SEC_PBCFG
# undef FRIF_STOP_SEC_PBCFG                         /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#ifdef FRIF_START_SEC_PBCFG_ROOT
# undef FRIF_START_SEC_PBCFG_ROOT                   /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef FRIF_STOP_SEC_PBCFG_ROOT
# undef FRIF_STOP_SEC_PBCFG_ROOT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef FRIF_START_SEC_VAR_INIT_UNSPECIFIED
# undef FRIF_START_SEC_VAR_INIT_UNSPECIFIED         /* PRQA S 0841 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED          /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#ifdef FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef FRIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef FRIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/* PBCFG section for flexible RAM management */

#ifdef FRIF_START_SEC_VAR_PBCFG
# undef FRIF_START_SEC_VAR_PBCFG       /* PRQA S 0841 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef FRIF_STOP_SEC_VAR_PBCFG
# undef FRIF_STOP_SEC_VAR_PBCFG        /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  FRIF END
 *********************************************************************************************************************/

#ifdef MEMIF_START_SEC_CONST_8BIT 
    #undef MEMIF_START_SEC_CONST_8BIT 
    #define START_SEC_CONST_8BIT
#endif

#ifdef MEMIF_STOP_SEC_CONST_8BIT 
    #undef MEMIF_STOP_SEC_CONST_8BIT 
    #define STOP_SEC_CONST
#endif


#ifdef MEMIF_START_SEC_CONST_32BIT
    #undef MEMIF_START_SEC_CONST_32BIT
    #define START_SEC_CONST_32BIT
#endif

#ifdef MEMIF_STOP_SEC_CONST_32BIT
    #undef MEMIF_STOP_SEC_CONST_32BIT
    #define STOP_SEC_CONST
#endif


#ifdef MEMIF_START_SEC_CODE
    #undef MEMIF_START_SEC_CODE
    #define START_SEC_CODE
#endif

#ifdef MEMIF_STOP_SEC_CODE
    #undef MEMIF_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/**********************************************************************************************************************
 *  SOAD START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined SOAD_START_SEC_CODE
# undef SOAD_START_SEC_CODE                         /* PRQA S 0841 */
# define START_SEC_CODE                             /* mapped to default code section */
#endif
#if defined SOAD_STOP_SEC_CODE
# undef SOAD_STOP_SEC_CODE                          /* PRQA S 0841 */
# define STOP_SEC_CODE                              /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined SOAD_START_SEC_CONST_8BIT
# undef SOAD_START_SEC_CONST_8BIT                   /* PRQA S 0841 */
# define START_SEC_CONST_8BIT
#endif
#if defined SOAD_STOP_SEC_CONST_8BIT
# undef SOAD_STOP_SEC_CONST_8BIT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SOAD_START_SEC_CONST_16BIT
# undef SOAD_START_SEC_CONST_16BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_16BIT
#endif
#if defined SOAD_STOP_SEC_CONST_16BIT
# undef SOAD_STOP_SEC_CONST_16BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SOAD_START_SEC_CONST_32BIT
# undef SOAD_START_SEC_CONST_32BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#if defined SOAD_STOP_SEC_CONST_32BIT
# undef SOAD_STOP_SEC_CONST_32BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SOAD_START_SEC_CONST_UNSPECIFIED
# undef SOAD_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined SOAD_STOP_SEC_CONST_UNSPECIFIED
# undef SOAD_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SOAD_START_SEC_PBCFG
# undef SOAD_START_SEC_PBCFG                        /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#if defined SOAD_STOP_SEC_PBCFG
# undef SOAD_STOP_SEC_PBCFG                         /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined SOAD_START_SEC_VAR_NOINIT_8BIT
# undef SOAD_START_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined SOAD_STOP_SEC_VAR_NOINIT_8BIT
# undef SOAD_STOP_SEC_VAR_NOINIT_8BIT               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_NOINIT_16BIT
# undef SOAD_START_SEC_VAR_NOINIT_16BIT             /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined SOAD_STOP_SEC_VAR_NOINIT_16BIT
# undef SOAD_STOP_SEC_VAR_NOINIT_16BIT              /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_NOINIT_32BIT
# undef SOAD_START_SEC_VAR_NOINIT_32BIT             /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined SOAD_STOP_SEC_VAR_NOINIT_32BIT
# undef SOAD_STOP_SEC_VAR_NOINIT_32BIT              /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef SOAD_START_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined SOAD_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef SOAD_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_INIT_8BIT
# undef SOAD_START_SEC_VAR_INIT_8BIT                /* PRQA S 0841 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined SOAD_STOP_SEC_VAR_INIT_8BIT
# undef SOAD_STOP_SEC_VAR_INIT_8BIT                 /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_INIT_16BIT
# undef SOAD_START_SEC_VAR_INIT_16BIT               /* PRQA S 0841 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined SOAD_STOP_SEC_VAR_INIT_16BIT
# undef SOAD_STOP_SEC_VAR_INIT_16BIT                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_INIT_32BIT
# undef SOAD_START_SEC_VAR_INIT_32BIT               /* PRQA S 0841 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined SOAD_STOP_SEC_VAR_INIT_32BIT
# undef SOAD_STOP_SEC_VAR_INIT_32BIT                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_INIT_UNSPECIFIED
# undef SOAD_START_SEC_VAR_INIT_UNSPECIFIED         /* PRQA S 0841 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined SOAD_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef SOAD_STOP_SEC_VAR_INIT_UNSPECIFIED          /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_ZERO_INIT_8BIT
# undef SOAD_START_SEC_VAR_ZERO_INIT_8BIT           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined SOAD_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef SOAD_STOP_SEC_VAR_ZERO_INIT_8BIT            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_ZERO_INIT_16BIT
# undef SOAD_START_SEC_VAR_ZERO_INIT_16BIT          /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined SOAD_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef SOAD_STOP_SEC_VAR_ZERO_INIT_16BIT           /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_ZERO_INIT_32BIT
# undef SOAD_START_SEC_VAR_ZERO_INIT_32BIT          /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined SOAD_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef SOAD_STOP_SEC_VAR_ZERO_INIT_32BIT           /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SOAD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined SOAD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SOAD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SOAD_START_SEC_VAR_PBCFG
# undef SOAD_START_SEC_VAR_PBCFG                    /* PRQA S 0841 */
# define START_SEC_VAR_PBCFG
#endif
#if defined SOAD_STOP_SEC_VAR_PBCFG
# undef SOAD_STOP_SEC_VAR_PBCFG                     /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  SOAD END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Com START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef COM_START_SEC_CODE
# undef COM_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef COM_STOP_SEC_CODE
# undef COM_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif
/* COM application callback code section (Rte_Cbk.h)*/
#ifdef COM_START_SEC_APPL_CODE
# undef COM_START_SEC_APPL_CODE
# define START_SEC_CODE
#endif
#ifdef COM_STOP_SEC_APPL_CODE
# undef COM_STOP_SEC_APPL_CODE
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef COM_START_SEC_CONST_8BIT
# undef COM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef COM_STOP_SEC_CONST_8BIT
# undef COM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_CONST_16BIT
# undef COM_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef COM_STOP_SEC_CONST_16BIT
# undef COM_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_CONST_32BIT
# undef COM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef COM_STOP_SEC_CONST_32BIT
# undef COM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_CONST_64BIT
# undef COM_START_SEC_CONST_64BIT
# define START_SEC_CONST_64BIT
#endif
#ifdef COM_STOP_SEC_CONST_64BIT
# undef COM_STOP_SEC_CONST_64BIT
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_CONST_UNSPECIFIED
# undef COM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef COM_STOP_SEC_CONST_UNSPECIFIED
# undef COM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_PBCFG
# undef COM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef COM_STOP_SEC_PBCFG
# undef COM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef COM_START_SEC_VAR_PBCFG
# undef COM_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG
#endif
#ifdef COM_STOP_SEC_VAR_PBCFG
# undef COM_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef COM_START_SEC_VAR_INIT_UNSPECIFIED
# undef COM_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef COM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef COM_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef COM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef COM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef COM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_ZERO_INIT_8BIT
# undef COM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef COM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef COM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef COM_START_SEC_VAR_NOINIT_BOOLEAN
# undef COM_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef COM_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_NOINIT_8BIT
# undef COM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_8BIT
# undef COM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_NOINIT_16BIT
# undef COM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_16BIT
# undef COM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_NOINIT_32BIT
# undef COM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_32BIT
# undef COM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_NOINIT_64BIT
# undef COM_START_SEC_VAR_NOINIT_64BIT
# define START_SEC_VAR_NOINIT_64BIT
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_64BIT
# undef COM_STOP_SEC_VAR_NOINIT_64BIT
# define STOP_SEC_VAR
#endif

#ifdef COM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef COM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef COM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef COM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */



/* VAR FAST INIT sections */



/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */



/**********************************************************************************************************************
 *  Com END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpduM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef IPDUM_START_SEC_CODE
# undef IPDUM_START_SEC_CODE
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef IPDUM_STOP_SEC_CODE
# undef IPDUM_STOP_SEC_CODE
# define STOP_SEC_CODE  /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* Postbuild CFG CONST sections */

#ifdef IPDUM_START_SEC_PBCFG_8BIT
# undef IPDUM_START_SEC_PBCFG_8BIT
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_PBCFG_8BIT
# undef IPDUM_STOP_SEC_PBCFG_8BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_PBCFG_16BIT
# undef IPDUM_START_SEC_PBCFG_16BIT
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_PBCFG_16BIT
# undef IPDUM_STOP_SEC_PBCFG_16BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_PBCFG_32BIT
# undef IPDUM_START_SEC_PBCFG_32BIT
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_PBCFG_32BIT
# undef IPDUM_STOP_SEC_PBCFG_32BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_PBCFG_UNSPECIFIED
# undef IPDUM_START_SEC_PBCFG_UNSPECIFIED
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_PBCFG_UNSPECIFIED
# undef IPDUM_STOP_SEC_PBCFG_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_PBCFG
# undef IPDUM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_PBCFG
# undef IPDUM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_VAR_PBCFG
# undef IPDUM_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG
#endif
#ifdef IPDUM_STOP_SEC_VAR_PBCFG
# undef IPDUM_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_CONST_8BIT
# undef IPDUM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef IPDUM_STOP_SEC_CONST_8BIT
# undef IPDUM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_CONST_16BIT
# undef IPDUM_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef IPDUM_STOP_SEC_CONST_16BIT
# undef IPDUM_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_CONST_32BIT
# undef IPDUM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef IPDUM_STOP_SEC_CONST_32BIT
# undef IPDUM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef IPDUM_START_SEC_CONST_UNSPECIFIED
# undef IPDUM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef IPDUM_STOP_SEC_CONST_UNSPECIFIED
# undef IPDUM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR ZERO INIT sections */

#ifdef IPDUM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef IPDUM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef IPDUM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef IPDUM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_VAR_ZERO_INIT_8BIT
# undef IPDUM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT
#endif
#ifdef IPDUM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef IPDUM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR INIT sections */

#ifdef IPDUM_START_SEC_VAR_INIT_UNSPECIFIED
# undef IPDUM_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef IPDUM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef IPDUM_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_VAR_INIT_8BIT
# undef IPDUM_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT
#endif
#ifdef IPDUM_STOP_SEC_VAR_INIT_8BIT
# undef IPDUM_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef IPDUM_START_SEC_VAR_NOINIT_8BIT
# undef IPDUM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef IPDUM_STOP_SEC_VAR_NOINIT_8BIT
# undef IPDUM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_VAR_NOINIT_16BIT
# undef IPDUM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef IPDUM_STOP_SEC_VAR_NOINIT_16BIT
# undef IPDUM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_VAR_NOINIT_32BIT
# undef IPDUM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef IPDUM_STOP_SEC_VAR_NOINIT_32BIT
# undef IPDUM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPDUM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPDUM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef IPDUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPDUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  IpduM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LDCOM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef LDCOM_START_SEC_CODE
# undef LDCOM_START_SEC_CODE
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef LDCOM_STOP_SEC_CODE
# undef LDCOM_STOP_SEC_CODE
# define STOP_SEC_CODE  /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef LDCOM_START_SEC_CONST_8BIT
# undef LDCOM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT   /* mapped to default const 8bit section */
#endif
#ifdef LDCOM_STOP_SEC_CONST_8BIT
# undef LDCOM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST /* default const stop section */
#endif

#ifdef LDCOM_START_SEC_CONST_16BIT
# undef LDCOM_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef LDCOM_STOP_SEC_CONST_16BIT
# undef LDCOM_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef LDCOM_START_SEC_CONST_32BIT
# undef LDCOM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef LDCOM_STOP_SEC_CONST_32BIT
# undef LDCOM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef LDCOM_START_SEC_CONST_UNSPECIFIED
# undef LDCOM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef LDCOM_STOP_SEC_CONST_UNSPECIFIED
# undef LDCOM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#ifdef LDCOM_START_SEC_PBCFG
# undef LDCOM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef LDCOM_STOP_SEC_PBCFG
# undef LDCOM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef LDCOM_START_SEC_VAR_INIT_8BIT
# undef LDCOM_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT        /* mapped to default var init 8bit section */
#endif
#ifdef LDCOM_STOP_SEC_VAR_INIT_8BIT
# undef LDCOM_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR   /* default var stop section */
#endif

#ifdef LDCOM_START_SEC_VAR_INIT_16BIT
# undef LDCOM_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_INIT_16BIT
# undef LDCOM_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_INIT_32BIT
# undef LDCOM_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_INIT_32BIT
# undef LDCOM_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_INIT_UNSPECIFIED
# undef LDCOM_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef LDCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef LDCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef LDCOM_START_SEC_VAR_NOINIT_BOOLEAN
# undef LDCOM_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef LDCOM_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_NOINIT_8BIT
# undef LDCOM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_NOINIT_8BIT
# undef LDCOM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_NOINIT_16BIT
# undef LDCOM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_NOINIT_16BIT
# undef LDCOM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_NOINIT_32BIT
# undef LDCOM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_NOINIT_32BIT
# undef LDCOM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef LDCOM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef LDCOM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef LDCOM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef LDCOM_START_SEC_VAR_ZERO_INIT_8BIT
# undef LDCOM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT
#endif
#ifdef LDCOM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef LDCOM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef LDCOM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef LDCOM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef LDCOM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef LDCOM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */

/* VAR FAST NOINIT sections */

/* VAR FAST ZERO INIT sections */


/**********************************************************************************************************************
 *  LDCOM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SECOC START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(SECOC_START_SEC_CODE)
# undef SECOC_START_SEC_CODE      /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif

#if defined(SECOC_STOP_SEC_CODE)
# undef SECOC_STOP_SEC_CODE       /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define STOP_SEC_CODE  /* default code stop section */
#endif

#if defined(SECOC_START_SEC_APPL_CODE)
# undef SECOC_START_SEC_APPL_CODE      /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif

#if defined(SECOC_STOP_SEC_APPL_CODE)
# undef SECOC_STOP_SEC_APPL_CODE       /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define STOP_SEC_CODE  /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(SECOC_START_SEC_CONST_8BIT)
# undef SECOC_START_SEC_CONST_8BIT  /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT   /* mapped to default const 8bit section */
#endif
#if defined(SECOC_STOP_SEC_CONST_8BIT)
# undef SECOC_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define STOP_SEC_CONST /* default const stop section */
#endif

#if defined(SECOC_START_SEC_CONST_16BIT)
# undef SECOC_START_SEC_CONST_16BIT /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(SECOC_STOP_SEC_CONST_16BIT)
# undef SECOC_STOP_SEC_CONST_16BIT /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SECOC_START_SEC_CONST_32BIT)
# undef SECOC_START_SEC_CONST_32BIT /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(SECOC_STOP_SEC_CONST_32BIT)
# undef SECOC_STOP_SEC_CONST_32BIT /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SECOC_START_SEC_CONST_UNSPECIFIED)
# undef SECOC_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(SECOC_STOP_SEC_CONST_UNSPECIFIED)
# undef SECOC_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */      /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

#if defined(SECOC_START_SEC_PBCFG)
# undef SECOC_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(SECOC_STOP_SEC_PBCFG)
# undef SECOC_STOP_SEC_PBCFG /* PRQA S 0841 */  /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(SECOC_START_SEC_VAR_INIT_8BIT)
# undef SECOC_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT        /* mapped to default var init 8bit section */
#endif
#if defined(SECOC_STOP_SEC_VAR_INIT_8BIT)
# undef SECOC_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */  /* MD_MSR_19.6 */
# define STOP_SEC_VAR   /* default var stop section */
#endif

#if defined(SECOC_START_SEC_VAR_INIT_16BIT)
# undef SECOC_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_INIT_16BIT)
# undef SECOC_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_INIT_32BIT)
# undef SECOC_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_INIT_32BIT)
# undef SECOC_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_INIT_UNSPECIFIED)
# undef SECOC_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */  /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined SECOC_START_SEC_VAR_NOINIT_BOOLEAN
# undef SECOC_START_SEC_VAR_NOINIT_BOOLEAN /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined SECOC_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef SECOC_STOP_SEC_VAR_NOINIT_BOOLEAN /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_NOINIT_8BIT)
# undef SECOC_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */       /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_NOINIT_8BIT)
# undef SECOC_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_NOINIT_16BIT)
# undef SECOC_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */      /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_NOINIT_16BIT)
# undef SECOC_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */       /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_NOINIT_32BIT)
# undef SECOC_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */      /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_NOINIT_32BIT)
# undef SECOC_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */       /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef SECOC_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */        /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(SECOC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef SECOC_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_PBCFG)
# undef SECOC_START_SEC_VAR_PBCFG /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(SECOC_STOP_SEC_VAR_PBCFG)
# undef SECOC_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */      /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(SECOC_START_SEC_VAR_ZERO_INIT_8BIT)
# undef SECOC_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef SECOC_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_ZERO_INIT_16BIT)
# undef SECOC_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef SECOC_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_ZERO_INIT_32BIT)
# undef SECOC_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */   /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(SECOC_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef SECOC_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */    /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SECOC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef SECOC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */     /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(SECOC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef SECOC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */      /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */


/**********************************************************************************************************************
 *  SECOC END
 *********************************************************************************************************************/


/***********************************************************************************************************************
 *  VIPC START 
 **********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef VIPC_START_SEC_CODE
#undef VIPC_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_CODE
#endif
#ifdef VIPC_STOP_SEC_CODE
#undef VIPC_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef VIPC_START_SEC_CONST_32BIT
#undef VIPC_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_CONST_32BIT
#endif
#ifdef VIPC_STOP_SEC_CONST_32BIT
#undef VIPC_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef VIPC_START_SEC_VAR_NOINIT_8BIT
#undef VIPC_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef VIPC_STOP_SEC_VAR_NOINIT_8BIT
#undef VIPC_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

#ifdef VIPC_START_SEC_VAR_NOINIT_32BIT
#undef VIPC_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef VIPC_STOP_SEC_VAR_NOINIT_32BIT
#undef VIPC_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

#ifdef VIPC_START_SEC_VAR_NOINIT_UNSPECIFIED
#undef VIPC_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef VIPC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#undef VIPC_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */
#ifdef VIPC_START_SEC_VAR_ZERO_INIT_8BIT
#undef VIPC_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef VIPC_STOP_SEC_VAR_ZERO_INIT_8BIT
#undef VIPC_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

#ifdef VIPC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#undef VIPC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef VIPC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#undef VIPC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

/* VAR INIT sections */
#ifdef VIPC_START_SEC_VAR_INIT_UNSPECIFIED
#undef VIPC_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef VIPC_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef VIPC_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

/* CONST INIT sections */
#ifdef VIPC_START_SEC_CONST_UNSPECIFIED
#undef VIPC_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef VIPC_STOP_SEC_CONST_UNSPECIFIED
#undef VIPC_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_CONST
#endif

#ifdef VIPC_START_SEC_PBCFG
#undef VIPC_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_CONST_PBCFG
#endif
#ifdef VIPC_STOP_SEC_PBCFG
#undef VIPC_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_CONST
#endif

/***********************************************************************************************************************
 *  VIPC END
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  VIPCMEMCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(VIPCMEMCP_START_SEC_CODE)
# undef VIPCMEMCP_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined(VIPCMEMCP_STOP_SEC_CODE)
# undef VIPCMEMCP_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(VIPCMEMCP_START_SEC_CONST_UNSPECIFIED)
# undef VIPCMEMCP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(VIPCMEMCP_STOP_SEC_CONST_UNSPECIFIED)
# undef VIPCMEMCP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(VIPCMEMCP_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VIPCMEMCP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(VIPCMEMCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VIPCMEMCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(VIPCMEMCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VIPCMEMCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VIPCMEMCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VIPCMEMCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  VIPCMEMCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  VIPCMEMIFCP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(VIPCMEMIFCP_START_SEC_CODE)
# undef VIPCMEMIFCP_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined(VIPCMEMIFCP_STOP_SEC_CODE)
# undef VIPCMEMIFCP_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(VIPCMEMIFCP_START_SEC_CONST_UNSPECIFIED)
# undef VIPCMEMIFCP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(VIPCMEMIFCP_STOP_SEC_CONST_UNSPECIFIED)
# undef VIPCMEMIFCP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(VIPCMEMIFCP_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VIPCMEMIFCP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(VIPCMEMIFCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VIPCMEMIFCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(VIPCMEMIFCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VIPCMEMIFCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VIPCMEMIFCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VIPCMEMIFCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  VIPCMEMIFCP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  J1939RM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef J1939RM_START_SEC_CODE
# undef J1939RM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef J1939RM_STOP_SEC_CODE
# undef J1939RM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef J1939RM_START_SEC_CONST_8BIT
# undef J1939RM_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT /* mapped to default const 8bit section */
#endif
#ifdef J1939RM_STOP_SEC_CONST_8BIT
# undef J1939RM_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST /* default const stop section */
#endif

#ifdef J1939RM_START_SEC_CONST_16BIT
# undef J1939RM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef J1939RM_STOP_SEC_CONST_16BIT
# undef J1939RM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939RM_START_SEC_CONST_32BIT
# undef J1939RM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#ifdef J1939RM_STOP_SEC_CONST_32BIT
# undef J1939RM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939RM_START_SEC_CONST_UNSPECIFIED
# undef J1939RM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef J1939RM_STOP_SEC_CONST_UNSPECIFIED
# undef J1939RM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */


/* Postbuild CFG CONST sections */

#ifdef J1939RM_START_SEC_PBCFG
# undef J1939RM_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef J1939RM_STOP_SEC_PBCFG
# undef J1939RM_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef J1939RM_START_SEC_VAR_ZERO_INIT_8BIT
# undef J1939RM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef J1939RM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef J1939RM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef J1939RM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939RM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef J1939RM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939RM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939RM_START_SEC_VAR_NOINIT_8BIT
# undef J1939RM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef J1939RM_STOP_SEC_VAR_NOINIT_8BIT
# undef J1939RM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939RM_START_SEC_VAR_NOINIT_16BIT
# undef J1939RM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef J1939RM_STOP_SEC_VAR_NOINIT_16BIT
# undef J1939RM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef J1939RM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939RM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef J1939RM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939RM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */


/* Postbuild CFG VAR sections */

#ifdef J1939RM_START_SEC_VAR_PBCFG
# undef J1939RM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef J1939RM_STOP_SEC_VAR_PBCFG
# undef J1939RM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  J1939RM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  NvM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef NVM_START_SEC_CODE
    #undef NVM_START_SEC_CODE
    #define START_SEC_CODE                /* mapped to default code section */
#endif
#ifdef NVM_STOP_SEC_CODE
    #undef NVM_STOP_SEC_CODE
    #define STOP_SEC_CODE                /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef NVM_START_SEC_CONST_8
    #undef NVM_START_SEC_CONST_8
    #define START_SEC_CONST_8BIT
#endif
#ifdef NVM_STOP_SEC_CONST_8
    #undef NVM_STOP_SEC_CONST_8
    #define STOP_SEC_CONST
#endif

#ifdef NVM_START_SEC_CONST_16
    #undef NVM_START_SEC_CONST_16
    #define START_SEC_CONST_16BIT
#endif
#ifdef NVM_STOP_SEC_CONST_16
    #undef NVM_STOP_SEC_CONST_16
    #define STOP_SEC_CONST
#endif

#ifdef NVM_START_SEC_CONST_UNSPECIFIED
    #undef NVM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef NVM_STOP_SEC_CONST_UNSPECIFIED
    #undef NVM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST
#endif

#ifdef NVM_START_SEC_CONST_DESCRIPTOR_TABLE
    #undef NVM_START_SEC_CONST_DESCRIPTOR_TABLE
    #define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef NVM_STOP_SEC_CONST_DESCRIPTOR_TABLE
    #undef NVM_STOP_SEC_CONST_DESCRIPTOR_TABLE
    #define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef NVM_START_SEC_VAR_NOINIT_8
    #undef NVM_START_SEC_VAR_NOINIT_8
    #define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef NVM_STOP_SEC_VAR_NOINIT_8
    #undef NVM_STOP_SEC_VAR_NOINIT_8
    #define STOP_SEC_VAR
#endif

#ifdef NVM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef NVM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef NVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef NVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/* If Development Error Detection is ON (as well as INIT check),    *
 *   it SHALL be mapped to an initialized section (zeor is ok)      *
 * if Development Error Detection (or at least INIT check) is OFF,  *
 *   it MAY also be mapped to a NOINIT section.                     */
#ifdef NVM_START_SEC_VAR_UNSPECIFIED
    #undef NVM_START_SEC_VAR_UNSPECIFIED
    #define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef NVM_STOP_SEC_VAR_UNSPECIFIED
    #undef NVM_STOP_SEC_VAR_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/* VAR FAST NOINIT sections */
#ifdef NVM_START_SEC_VAR_FAST_8
    #undef NVM_START_SEC_VAR_FAST_8
    #define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#ifdef NVM_STOP_SEC_VAR_FAST_8
    #undef NVM_STOP_SEC_VAR_FAST_8
    #define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

/* SHALL be mapped to a section that is guaranteed to be ZEROed out after Power-On Reset       *
 * (therefore it MAY be a "normal" ZERO_INIT section, being ZEROed out after ANY reset         *
 * Make shure this helds true for all kinds of variable data (esp. "uninitialized).            *
 * If necessary, create a special section (don't map to a common one)                          */    
#ifdef NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  NvM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DBG START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef DBG_START_SEC_CODE
  #undef DBG_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef DBG_STOP_SEC_CODE
  #undef DBG_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef DBG_START_SEC_CODE_FAST
  #undef DBG_START_SEC_CODE_FAST
  #define START_SEC_CODE_FAST                       /* mapped to default fast code section */
#endif
#ifdef DBG_STOP_SEC_CODE_FAST
  #undef DBG_STOP_SEC_CODE_FAST
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef DBG_START_SEC_CONST_8BIT
# undef DBG_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef DBG_STOP_SEC_CONST_8BIT
# undef DBG_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef DBG_START_SEC_CONST_16BIT
# undef DBG_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef DBG_STOP_SEC_CONST_16BIT
# undef DBG_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef DBG_START_SEC_CONST_32BIT
# undef DBG_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef DBG_STOP_SEC_CONST_32BIT
# undef DBG_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef DBG_START_SEC_CONST_UNSPECIFIED
# undef DBG_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef DBG_STOP_SEC_CONST_UNSPECIFIED
# undef DBG_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef DBG_START_SEC_VAR_INIT_8BIT
# undef DBG_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef DBG_STOP_SEC_VAR_INIT_8BIT
# undef DBG_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef DBG_START_SEC_VAR_INIT_16BIT
# undef DBG_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef DBG_STOP_SEC_VAR_INIT_16BIT
# undef DBG_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_INIT_32BIT
# undef DBG_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef DBG_STOP_SEC_VAR_INIT_32BIT
# undef DBG_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_INIT_UNSPECIFIED
# undef DBG_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef DBG_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef DBG_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef DBG_START_SEC_VAR_NOINIT_8BIT
# undef DBG_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef DBG_STOP_SEC_VAR_NOINIT_8BIT
# undef DBG_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_NOINIT_16BIT
# undef DBG_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef DBG_STOP_SEC_VAR_NOINIT_16BIT
# undef DBG_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_NOINIT_32BIT
# undef DBG_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef DBG_STOP_SEC_VAR_NOINIT_32BIT
# undef DBG_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef DBG_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef DBG_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef DBG_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef DBG_START_SEC_VAR_ZERO_INIT_8BIT
# undef DBG_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef DBG_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef DBG_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_ZERO_INIT_16BIT
# undef DBG_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef DBG_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef DBG_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_ZERO_INIT_32BIT
# undef DBG_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef DBG_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef DBG_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DBG_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DBG_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef DBG_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DBG_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif



/**********************************************************************************************************************
 *  DBG END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  RTM START 
 *********************************************************************************************************************/


/*******  CODE sections **********************************************************************************************/

#ifdef RTM_START_SEC_CODE
  #undef RTM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef RTM_STOP_SEC_CODE
  #undef RTM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR ZERO INIT sections */

#ifdef RTM_START_SEC_VAR_ZERO_INIT_8BIT
# undef RTM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef RTM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef RTM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef RTM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef RTM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef RTM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef RTM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR INIT sections */

#ifdef RTM_START_SEC_VAR_INIT_UNSPECIFIED
# undef RTM_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef RTM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef RTM_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef RTM_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# undef RTM_START_SEC_VAR_INIT_UNSPECIFIED_SAFE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef RTM_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# undef RTM_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef RTM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef RTM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef RTM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef RTM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/*******  CONST sections  **********************************************************************************************/

#ifdef RTM_START_SEC_CONST_UNSPECIFIED
# undef RTM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef RTM_STOP_SEC_CONST_UNSPECIFIED
# undef RTM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/**********************************************************************************************************************
 *  RTM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  DLT START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef DLT_START_SEC_CODE
  #undef DLT_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef DLT_STOP_SEC_CODE
  #undef DLT_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef DLT_START_SEC_CODE_FAST
  #undef DLT_START_SEC_CODE_FAST
  #define START_SEC_CODE_FAST                       /* mapped to default fast code section */
#endif
#ifdef DLT_STOP_SEC_CODE_FAST
  #undef DLT_STOP_SEC_CODE_FAST
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


#ifdef DLTCOM_START_SEC_CODE
  #undef DLTCOM_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef DLTCOM_STOP_SEC_CODE
  #undef DLTCOM_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef DLTCOM_START_SEC_CODE_FAST
  #undef DLTCOM_START_SEC_CODE_FAST
  #define START_SEC_CODE_FAST                       /* mapped to default fast code section */
#endif
#ifdef DLTCOM_STOP_SEC_CODE_FAST
  #undef DLTCOM_STOP_SEC_CODE_FAST
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef DLT_START_SEC_CONST_8BIT
# undef DLT_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef DLT_STOP_SEC_CONST_8BIT
# undef DLT_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef DLT_START_SEC_CONST_16BIT
# undef DLT_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef DLT_STOP_SEC_CONST_16BIT
# undef DLT_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef DLT_START_SEC_CONST_32BIT
# undef DLT_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef DLT_STOP_SEC_CONST_32BIT
# undef DLT_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef DLT_START_SEC_CONST_UNSPECIFIED
# undef DLT_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef DLT_STOP_SEC_CONST_UNSPECIFIED
# undef DLT_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif



#ifdef DLTCOM_START_SEC_CONST_8BIT
# undef DLTCOM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef DLTCOM_STOP_SEC_CONST_8BIT
# undef DLTCOM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef DLTCOM_START_SEC_CONST_16BIT
# undef DLTCOM_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef DLTCOM_STOP_SEC_CONST_16BIT
# undef DLTCOM_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef DLTCOM_START_SEC_CONST_32BIT
# undef DLTCOM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef DLTCOM_STOP_SEC_CONST_32BIT
# undef DLTCOM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef DLTCOM_START_SEC_CONST_UNSPECIFIED
# undef DLTCOM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef DLTCOM_STOP_SEC_CONST_UNSPECIFIED
# undef DLTCOM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif



/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef DLT_START_SEC_VAR_INIT_8BIT
# undef DLT_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef DLT_STOP_SEC_VAR_INIT_8BIT
# undef DLT_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef DLT_START_SEC_VAR_INIT_16BIT
# undef DLT_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef DLT_STOP_SEC_VAR_INIT_16BIT
# undef DLT_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_INIT_32BIT
# undef DLT_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef DLT_STOP_SEC_VAR_INIT_32BIT
# undef DLT_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_INIT_UNSPECIFIED
# undef DLT_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif



#ifdef DLTCOM_START_SEC_VAR_INIT_8BIT
# undef DLTCOM_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef DLTCOM_STOP_SEC_VAR_INIT_8BIT
# undef DLTCOM_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef DLTCOM_START_SEC_VAR_INIT_16BIT
# undef DLTCOM_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_INIT_16BIT
# undef DLTCOM_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_INIT_32BIT
# undef DLTCOM_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_INIT_32BIT
# undef DLTCOM_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_INIT_UNSPECIFIED
# undef DLTCOM_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef DLTCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef DLTCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef DLT_START_SEC_VAR_NOINIT_8BIT
# undef DLT_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef DLT_STOP_SEC_VAR_NOINIT_8BIT
# undef DLT_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_NOINIT_16BIT
# undef DLT_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef DLT_STOP_SEC_VAR_NOINIT_16BIT
# undef DLT_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_NOINIT_32BIT
# undef DLT_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef DLT_STOP_SEC_VAR_NOINIT_32BIT
# undef DLT_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef DLT_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef DLT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef DLT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif



#ifdef DLTCOM_START_SEC_VAR_NOINIT_8BIT
# undef DLTCOM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_NOINIT_8BIT
# undef DLTCOM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_NOINIT_16BIT
# undef DLTCOM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_NOINIT_16BIT
# undef DLTCOM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_NOINIT_32BIT
# undef DLTCOM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_NOINIT_32BIT
# undef DLTCOM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef DLTCOM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef DLTCOM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef DLTCOM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif



/* VAR ZERO INIT sections */

#ifdef DLT_START_SEC_VAR_ZERO_INIT_8BIT
# undef DLT_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef DLT_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef DLT_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_ZERO_INIT_16BIT
# undef DLT_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef DLT_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef DLT_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_ZERO_INIT_32BIT
# undef DLT_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef DLT_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef DLT_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DLT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef DLT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DLT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif



#ifdef DLTCOM_START_SEC_VAR_ZERO_INIT_8BIT
# undef DLTCOM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef DLTCOM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_ZERO_INIT_16BIT
# undef DLTCOM_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef DLTCOM_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_ZERO_INIT_32BIT
# undef DLTCOM_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef DLTCOM_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef DLTCOM_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef DLTCOM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DLTCOM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef DLTCOM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DLTCOM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif




/**********************************************************************************************************************
 *  DLT END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  J1939NM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef J1939NM_START_SEC_CODE
# undef J1939NM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef J1939NM_STOP_SEC_CODE
# undef J1939NM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef J1939NM_START_SEC_CONST_8BIT
# undef J1939NM_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT /* mapped to default const 8bit section */
#endif
#ifdef J1939NM_STOP_SEC_CONST_8BIT
# undef J1939NM_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST /* default const stop section */
#endif

#ifdef J1939NM_START_SEC_CONST_16BIT
# undef J1939NM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef J1939NM_STOP_SEC_CONST_16BIT
# undef J1939NM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939NM_START_SEC_CONST_32BIT
# undef J1939NM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#ifdef J1939NM_STOP_SEC_CONST_32BIT
# undef J1939NM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939NM_START_SEC_CONST_UNSPECIFIED
# undef J1939NM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef J1939NM_STOP_SEC_CONST_UNSPECIFIED
# undef J1939NM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */


/* Postbuild CFG CONST sections */

#ifdef J1939NM_START_SEC_PBCFG
# undef J1939NM_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef J1939NM_STOP_SEC_PBCFG
# undef J1939NM_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef J1939NM_START_SEC_VAR_ZERO_INIT_8BIT
# undef J1939NM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef J1939NM_START_SEC_VAR_NOINIT_8BIT
# undef J1939NM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef J1939NM_STOP_SEC_VAR_NOINIT_8BIT
# undef J1939NM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef J1939NM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939NM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef J1939NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */


/* Postbuild CFG VAR sections */

#ifdef J1939NM_START_SEC_VAR_PBCFG
# undef J1939NM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef J1939NM_STOP_SEC_VAR_PBCFG
# undef J1939NM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  J1939NM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CANNM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CANNM_START_SEC_CODE
    #undef CANNM_START_SEC_CODE                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_CODE                              /* mapped to default code section */
#endif
#ifdef CANNM_STOP_SEC_CODE
    #undef CANNM_STOP_SEC_CODE                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_CODE                               /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CANNM_START_SEC_CONST_8BIT
    #undef CANNM_START_SEC_CONST_8BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_CONST_8BIT                        /* mapped to default const section */
#endif
#ifdef CANNM_STOP_SEC_CONST_8BIT
    #undef CANNM_STOP_SEC_CONST_8BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_CONST                              /* default const stop section */
#endif

#ifdef CANNM_START_SEC_CONST_UNSPECIFIED
    #undef CANNM_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_CONST_UNSPECIFIED                 /* mapped to default const section */
#endif
#ifdef CANNM_STOP_SEC_CONST_UNSPECIFIED
    #undef CANNM_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_CONST                              /* default const stop section */
#endif

/* Postbuild CFG CONST sections */

#ifdef CANNM_START_SEC_PBCFG
    #undef CANNM_START_SEC_PBCFG                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_CONST_PBCFG                       /* mapped to default postbuild section */
#endif
#ifdef CANNM_STOP_SEC_PBCFG
    #undef CANNM_STOP_SEC_PBCFG                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_CONST                              /* default const stop section */
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef CANNM_START_SEC_VAR_NOINIT_8BIT
    #undef CANNM_START_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_NOINIT_8BIT                   /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_NOINIT_8BIT
    #undef CANNM_STOP_SEC_VAR_NOINIT_8BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

#ifdef CANNM_START_SEC_VAR_NOINIT_16BIT
    #undef CANNM_START_SEC_VAR_NOINIT_16BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_NOINIT_16BIT                   /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_NOINIT_16BIT
    #undef CANNM_STOP_SEC_VAR_NOINIT_16BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

#ifdef CANNM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef CANNM_START_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_NOINIT_UNSPECIFIED            /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef CANNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

/* VAR INIT sections */
#ifdef CANNM_START_SEC_VAR_INIT_UNSPECIFIED
    #undef CANNM_START_SEC_VAR_INIT_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_INIT_UNSPECIFIED              /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef CANNM_STOP_SEC_VAR_INIT_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

#ifdef CANNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
    #undef CANNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_ZERO_INIT_UNSPECIFIED         /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
    #undef CANNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif


#ifdef CANNM_START_SEC_VAR_ZERO_INIT_8BIT
    #undef CANNM_START_SEC_VAR_ZERO_INIT_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_ZERO_INIT_8BIT                   /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_ZERO_INIT_8BIT
    #undef CANNM_STOP_SEC_VAR_ZERO_INIT_8BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

/* VAR PBCFG sections */

#ifdef CANNM_START_SEC_VAR_PBCFG
    #undef CANNM_START_SEC_VAR_PBCFG                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_PBCFG                         /* mapped to default var section */
#endif
#ifdef CANNM_STOP_SEC_VAR_PBCFG
    #undef CANNM_STOP_SEC_VAR_PBCFG                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                                /* default var stop section */
#endif

/**********************************************************************************************************************
 *  CANNM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FRNM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef FRNM_START_SEC_CODE
# undef FRNM_START_SEC_CODE                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                               /* mapped to default code section */
#endif
#ifdef FRNM_STOP_SEC_CODE
# undef FRNM_STOP_SEC_CODE                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef FRNM_START_SEC_CONST_8BIT
# undef FRNM_START_SEC_CONST_8BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                         /* mapped to default const 8bit section */
#endif
#ifdef FRNM_STOP_SEC_CONST_8BIT
# undef FRNM_STOP_SEC_CONST_8BIT                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                               /* default const stop section*/
#endif

#ifdef FRNM_START_SEC_CONST_16BIT
# undef FRNM_START_SEC_CONST_16BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT                         /* mapped to default const 16bit section */
#endif
#ifdef FRNM_STOP_SEC_CONST_16BIT
# undef FRNM_STOP_SEC_CONST_16BIT                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                               /* default const stop section*/
#endif

#ifdef FRNM_START_SEC_CONST_UNSPECIFIED
# undef FRNM_START_SEC_CONST_UNSPECIFIED              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                  /* mapped to default const unspecified section */
#endif
#ifdef FRNM_STOP_SEC_CONST_UNSPECIFIED
# undef FRNM_STOP_SEC_CONST_UNSPECIFIED               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                               /* default const stop section */
#endif

/* Postbuild CFG CONST sections */

#ifdef FRNM_START_SEC_PBCFG
# undef FRNM_START_SEC_PBCFG                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                        /* mapped to default pbcfg section */
#endif
#ifdef FRNM_STOP_SEC_PBCFG
# undef FRNM_STOP_SEC_PBCFG                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                               /* default const stop section */
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef FRNM_START_SEC_VAR_NOINIT_8BIT
# undef FRNM_START_SEC_VAR_NOINIT_8BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT                    /* mapped to default var noinit 8bit section */
#endif
#ifdef FRNM_STOP_SEC_VAR_NOINIT_8BIT
# undef FRNM_STOP_SEC_VAR_NOINIT_8BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                 /* default var stop section */
#endif

#ifdef FRNM_START_SEC_VAR_NOINIT_16BIT
# undef FRNM_START_SEC_VAR_NOINIT_16BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT                   /* mapped to default var noinit 16bit section */
#endif
#ifdef FRNM_STOP_SEC_VAR_NOINIT_16BIT
# undef FRNM_STOP_SEC_VAR_NOINIT_16BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                 /* default var stop section */
#endif

#ifdef FRNM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRNM_START_SEC_VAR_NOINIT_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED             /* mapped to default var noinit unspecified section */
#endif
#ifdef FRNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FRNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                 /* default var stop section */
#endif

/* VAR ZERO INIT section */
#ifdef FRNM_START_SEC_VAR_ZERO_INIT_8BIT
    #undef FRNM_START_SEC_VAR_ZERO_INIT_8BIT          /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define START_SEC_VAR_ZERO_INIT_8BIT              /* mapped to default var section */
#endif
#ifdef FRNM_STOP_SEC_VAR_ZERO_INIT_8BIT
    #undef FRNM_STOP_SEC_VAR_ZERO_INIT_8BIT           /* PRQA S 0841 */ /* MD_MSR_19.6 */
    #define STOP_SEC_VAR                              /* default var stop section */
#endif

#ifdef FRNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED          /* mapped to zero init var section */
#endif
#ifdef FRNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef FRNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                 /* default var stop section */
#endif

/* VAR PBCFG sections */

#ifdef FRNM_START_SEC_VAR_PBCFG
# undef FRNM_START_SEC_VAR_PBCFG                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG                          /* mapped to default var pbcfg section */
#endif
#ifdef FRNM_STOP_SEC_VAR_PBCFG
# undef FRNM_STOP_SEC_VAR_PBCFG                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                 /* default var stop section */
#endif

/**********************************************************************************************************************
 *  FRNM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  NM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef NM_START_SEC_CODE
# undef NM_START_SEC_CODE                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                 /* mapped to default code section */
#endif
#ifdef NM_STOP_SEC_CODE
# undef NM_STOP_SEC_CODE                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                  /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef NM_START_SEC_CONST_8
# undef NM_START_SEC_CONST_8                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8                              /* mapped to default const section */
#endif
#ifdef NM_STOP_SEC_CONST_8
# undef NM_STOP_SEC_CONST_8                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef NM_START_SEC_CONST_16
# undef NM_START_SEC_CONST_16                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16                             /* mapped to default const section */
#endif
#ifdef NM_STOP_SEC_CONST_16
# undef NM_STOP_SEC_CONST_16                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef NM_START_SEC_CONST_UNSPECIFIED
# undef NM_START_SEC_CONST_UNSPECIFIED                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /* mapped to default const section */
#endif
#ifdef NM_STOP_SEC_CONST_UNSPECIFIED
# undef NM_STOP_SEC_CONST_UNSPECIFIED                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NO_INIT sections */

#ifdef NM_START_SEC_VAR_NO_INIT_8
# undef NM_START_SEC_VAR_NO_INIT_8                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_8                        /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_NO_INIT_8
# undef NM_STOP_SEC_VAR_NO_INIT_8                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef NM_START_SEC_VAR_NO_INIT_16
# undef NM_START_SEC_VAR_NO_INIT_16                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_16                       /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_NO_INIT_16
# undef NM_STOP_SEC_VAR_NO_INIT_16                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef NM_START_SEC_VAR_NO_INIT_32
# undef NM_START_SEC_VAR_NO_INIT_32                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_32                       /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_NO_INIT_32
# undef NM_STOP_SEC_VAR_NO_INIT_32                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef NM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# undef NM_START_SEC_VAR_NO_INIT_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_UNSPECIFIED              /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# undef NM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/* VAR INIT sections */

#ifdef NM_START_SEC_VAR_INIT_8
# undef NM_START_SEC_VAR_INIT_8                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8                           /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_INIT_8
# undef NM_STOP_SEC_VAR_INIT_8                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef NM_START_SEC_VAR_INIT_UNSPECIFIED
# undef NM_START_SEC_VAR_INIT_UNSPECIFIED               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED                 /* mapped to default var section */
#endif
#ifdef NM_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef NM_STOP_SEC_VAR_INIT_UNSPECIFIED                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/**********************************************************************************************************************
 *  NM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  NMOSEK START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(NMOSEK_START_SEC_CODE)
# undef NMOSEK_START_SEC_CODE                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                 /* mapped to default code section */
#endif
#if defined(NMOSEK_STOP_SEC_CODE)
# undef NMOSEK_STOP_SEC_CODE                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                  /* default code stop section */
#endif

#if defined(NMOSEK_START_SEC_APPL_CODE)
# undef NMOSEK_START_SEC_APPL_CODE                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                 /* mapped to default code section */
#endif
#if defined(NMOSEK_STOP_SEC_APPL_CODE)
# undef NMOSEK_STOP_SEC_APPL_CODE                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                  /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(NMOSEK_START_SEC_CONST_8)
# undef NMOSEK_START_SEC_CONST_8                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8                              /* mapped to default const section */
#endif
#if defined(NMOSEK_STOP_SEC_CONST_8)
# undef NMOSEK_STOP_SEC_CONST_8                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#if defined(NMOSEK_START_SEC_CONST_32)
# undef NMOSEK_START_SEC_CONST_32                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32                             /* mapped to default const section */
#endif
#if defined(NMOSEK_STOP_SEC_CONST_32)
# undef NMOSEK_STOP_SEC_CONST_32                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#if defined(NMOSEK_START_SEC_CONST_UNSPECIFIED)
# undef NMOSEK_START_SEC_CONST_UNSPECIFIED              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /* mapped to default const section */
#endif
#if defined(NMOSEK_STOP_SEC_CONST_UNSPECIFIED)
# undef NMOSEK_STOP_SEC_CONST_UNSPECIFIED               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

/* Postbuild CFG CONST sections */

#if defined(NMOSEK_START_SEC_PBCFG)
# undef NMOSEK_START_SEC_PBCFG                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                          /* mapped to default postbuild section */
#endif
#if defined(NMOSEK_STOP_SEC_PBCFG)
# undef NMOSEK_STOP_SEC_PBCFG                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

/* Application CONST sections */
#if defined(NMOSEK_START_SEC_APPL_CONST_UNSPECIFIED)
# undef NMOSEK_START_SEC_APPL_CONST_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /* mapped to default const section */
#endif
#if defined(NMOSEK_STOP_SEC_APPL_CONST_UNSPECIFIED)
# undef NMOSEK_STOP_SEC_APPL_CONST_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(NMOSEK_START_SEC_VAR_NO_INIT_8)
# undef NMOSEK_START_SEC_VAR_NO_INIT_8                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_8                        /* mapped to default var section */
#endif
#if defined(NMOSEK_STOP_SEC_VAR_NO_INIT_8)
# undef NMOSEK_STOP_SEC_VAR_NO_INIT_8                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#if defined(NMOSEK_START_SEC_VAR_NO_INIT_16)
# undef NMOSEK_START_SEC_VAR_NO_INIT_16                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_16                       /* mapped to default var section */
#endif
#if defined(NMOSEK_STOP_SEC_VAR_NO_INIT_16)
# undef NMOSEK_STOP_SEC_VAR_NO_INIT_16                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#if defined(NMOSEK_START_SEC_VAR_NO_INIT_UNSPECIFIED)
# undef NMOSEK_START_SEC_VAR_NO_INIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NO_INIT_UNSPECIFIED              /* mapped to default var section */
#endif
#if defined(NMOSEK_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
# undef NMOSEK_STOP_SEC_VAR_NO_INIT_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/* VAR ZERO_INIT sections */
#if defined(NMOSEK_START_SEC_VAR_CLEARED_8)
# undef NMOSEK_START_SEC_VAR_CLEARED_8                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_CLEARED_8                        /* mapped to default var section */
#endif
#if defined(NMOSEK_STOP_SEC_VAR_CLEARED_8)
# undef NMOSEK_STOP_SEC_VAR_CLEARED_8                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#if defined(NMOSEK_START_SEC_VAR_CLEARED_UNSPECIFIED)
# undef NMOSEK_START_SEC_VAR_CLEARED_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_CLEARED_UNSPECIFIED              /* mapped to default var section */
#endif
#if defined(NMOSEK_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
# undef NMOSEK_STOP_SEC_VAR_CLEARED_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/* VAR PBCFG sections */

#ifdef NMOSEK_START_SEC_VAR_PBCFG
# undef NMOSEK_START_SEC_VAR_PBCFG                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG                            /* mapped to default var pbcfg section */
#endif
#ifdef NMOSEK_STOP_SEC_VAR_PBCFG
# undef NMOSEK_STOP_SEC_VAR_PBCFG                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/**********************************************************************************************************************
 *  NMOSEK END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LINNM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef LINNM_START_SEC_CODE
    #undef LINNM_START_SEC_CODE
    #define START_SEC_CODE                              /*mapped to default code section*/
#endif
#ifdef LINNM_STOP_SEC_CODE
    #undef LINNM_STOP_SEC_CODE
    #define STOP_SEC_CODE                               /*default code stop section*/
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef LINNM_START_SEC_CONST_8BIT
    #undef LINNM_START_SEC_CONST_8BIT
    #define START_SEC_CONST_8BIT                        /*mapped to default const section */
#endif
#ifdef LINNM_STOP_SEC_CONST_8BIT
    #undef LINNM_STOP_SEC_CONST_8BIT
    #define STOP_SEC_CONST                              /*default const stop section*/
#endif

#ifdef LINNM_START_SEC_CONST_32BIT
    #undef LINNM_START_SEC_CONST_32BIT
    #define START_SEC_CONST_32BIT                       /*mapped to default const section */
#endif
#ifdef LINNM_STOP_SEC_CONST_32BIT
    #undef LINNM_STOP_SEC_CONST_32BIT
    #define STOP_SEC_CONST                              /*default const stop section*/
#endif

#ifdef LINNM_START_SEC_CONST_UNSPECIFIED
    #undef LINNM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED                 /*mapped to default const section */
#endif
#ifdef LINNM_STOP_SEC_CONST_UNSPECIFIED
    #undef LINNM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST                              /*default const stop section*/
#endif

/* Postbuild CFG CONST sections */

#ifdef LINNM_START_SEC_PBCFG
    #undef LINNM_START_SEC_PBCFG
    #define START_SEC_CONST_PBCFG                       /*mapped to default postbuild section */
#endif
#ifdef LINNM_STOP_SEC_PBCFG
    #undef LINNM_STOP_SEC_PBCFG
    #define STOP_SEC_CONST                              /*default const stop section*/
#endif

#ifdef LINNM_START_SEC_PBCFG_ROOT
    #undef LINNM_START_SEC_PBCFG_ROOT
    #define START_SEC_CONST_PBCFG                       /*mapped to default postbuild section */
#endif
#ifdef LINNM_STOP_SEC_PBCFG_ROOT
    #undef LINNM_STOP_SEC_PBCFG_ROOT
    #define STOP_SEC_CONST                              /*default const stop section*/
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef LINNM_START_SEC_VAR_NOINIT_8BIT
    #undef LINNM_START_SEC_VAR_NOINIT_8BIT
    #define START_SEC_VAR_NOINIT_8BIT                   /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_NOINIT_8BIT
    #undef LINNM_STOP_SEC_VAR_NOINIT_8BIT
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

#ifdef LINNM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef LINNM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #define START_SEC_VAR_NOINIT_UNSPECIFIED            /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef LINNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

/* VAR FAST NOINIT sections */

#ifdef LINNM_START_SEC_VAR_FAST_NOINIT_8BIT
    #undef LINNM_START_SEC_VAR_FAST_NOINIT_8BIT
    #define START_SEC_VAR_FAST_NOINIT_8BIT              /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_FAST_NOINIT_8BIT
    #undef LINNM_STOP_SEC_VAR_FAST_NOINIT_8BIT
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

#ifdef LINNM_START_SEC_VAR_FAST_NOINIT_16BIT
    #undef LINNM_START_SEC_VAR_FAST_NOINIT_16BIT
    #define START_SEC_VAR_FAST_NOINIT_16BIT             /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_FAST_NOINIT_16BIT
    #undef LINNM_STOP_SEC_VAR_FAST_NOINIT_16BIT
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

#ifdef LINNM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
    #undef LINNM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
    #define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED       /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
    #undef LINNM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

/* VAR FAST ZERO INIT sections */

#ifdef LINNM_START_SEC_VAR_FAST_ZERO_INIT_8BIT
    #undef LINNM_START_SEC_VAR_FAST_ZERO_INIT_8BIT
    #define START_SEC_VAR_FAST_ZERO_INIT_8BIT           /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
    #undef LINNM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

#ifdef LINNM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
    #undef LINNM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
    #define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED    /*mapped to default var section*/
#endif
#ifdef LINNM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
    #undef LINNM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR                                /*default var stop section*/
#endif

/**********************************************************************************************************************
 *  LINNM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  UDPNM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(UDPNM_START_SEC_CODE)
# undef UDPNM_START_SEC_CODE                            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif /* UDPNM_START_SEC_CODE */
#if defined(UDPNM_STOP_SEC_CODE)
# undef UDPNM_STOP_SEC_CODE                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif /* UDPNM_STOP_SEC_CODE */

/*******  CONST sections  ********************************************************************************************/

#if defined(UDPNM_START_SEC_CONST_8BIT)
# undef UDPNM_START_SEC_CONST_8BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif /* UDPNM_START_SEC_CONST_8BIT */
#if defined(UDPNM_STOP_SEC_CONST_8BIT)
# undef UDPNM_STOP_SEC_CONST_8BIT                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif /* UDPNM_STOP_SEC_CONST_8BIT */

#if defined(UDPNM_START_SEC_CONST_16BIT)
# undef UDPNM_START_SEC_CONST_16BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif /* UDPNM_START_SEC_CONST_16BIT */
#if defined(UDPNM_STOP_SEC_CONST_16BIT)
# undef UDPNM_STOP_SEC_CONST_16BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif /* UDPNM_STOP_SEC_CONST_16BIT */

#if defined(UDPNM_START_SEC_CONST_32BIT)
# undef UDPNM_START_SEC_CONST_32BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif /* UDPNM_START_SEC_CONST_32BIT */
#if defined(UDPNM_STOP_SEC_CONST_32BIT)
# undef UDPNM_STOP_SEC_CONST_32BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif /* UDPNM_STOP_SEC_CONST_32BIT */

#if defined(UDPNM_START_SEC_CONST_UNSPECIFIED)
# undef UDPNM_START_SEC_CONST_UNSPECIFIED              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif /* UDPNM_START_SEC_CONST_UNSPECIFIED */
#if defined(UDPNM_STOP_SEC_CONST_UNSPECIFIED)
# undef UDPNM_STOP_SEC_CONST_UNSPECIFIED               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif /* UDPNM_STOP_SEC_CONST_UNSPECIFIED */

#if defined(UDPNM_START_SEC_PBCFG)
# undef UDPNM_START_SEC_PBCFG                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif /* UDPNM_START_SEC_PBCFG */
#if defined(UDPNM_STOP_SEC_PBCFG)
# undef UDPNM_STOP_SEC_PBCFG                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif /* UDPNM_STOP_SEC_PBCFG */

#if defined(UDPNM_START_SEC_PBCFG_ROOT)
# undef UDPNM_START_SEC_PBCFG_ROOT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif /* UDPNM_START_SEC_PBCFG_ROOT */
#if defined(UDPNM_STOP_SEC_PBCFG_ROOT)
# undef UDPNM_STOP_SEC_PBCFG_ROOT                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                        
#endif /* UDPNM_STOP_SEC_PBCFG_ROOT */

/*******  VAR sections  **********************************************************************************************/

#if defined(UDPNM_START_SEC_VAR_NOINIT_8BIT)
# undef UDPNM_START_SEC_VAR_NOINIT_8BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif /* UDPNM_START_SEC_VAR_NOINIT_8BIT */
#if defined(UDPNM_STOP_SEC_VAR_NOINIT_8BIT)
# undef UDPNM_STOP_SEC_VAR_NOINIT_8BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_NOINIT_8BIT */

#if defined(UDPNM_START_SEC_VAR_NOINIT_16BIT)
# undef UDPNM_START_SEC_VAR_NOINIT_16BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif /* UDPNM_START_SEC_VAR_NOINIT_16BIT */
#if defined(UDPNM_STOP_SEC_VAR_NOINIT_16BIT)
# undef UDPNM_STOP_SEC_VAR_NOINIT_16BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_NOINIT_16BIT */

#if defined(UDPNM_START_SEC_VAR_NOINIT_32BIT)
# undef UDPNM_START_SEC_VAR_NOINIT_32BIT               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif /* UDPNM_START_SEC_VAR_NOINIT_32BIT */
#if defined(UDPNM_STOP_SEC_VAR_NOINIT_32BIT)
# undef UDPNM_STOP_SEC_VAR_NOINIT_32BIT                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_NOINIT_32BIT */

#if defined(UDPNM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef UDPNM_START_SEC_VAR_NOINIT_UNSPECIFIED         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif /* UDPNM_START_SEC_VAR_NOINIT_UNSPECIFIED */
#if defined(UDPNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef UDPNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED */

#if defined(UDPNM_START_SEC_VAR_INIT_8BIT)
# undef UDPNM_START_SEC_VAR_INIT_8BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif /* UDPNM_START_SEC_VAR_INIT_8BIT */
#if defined(UDPNM_STOP_SEC_VAR_INIT_8BIT)
# undef UDPNM_STOP_SEC_VAR_INIT_8BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_INIT_8BIT */

#if defined(UDPNM_START_SEC_VAR_INIT_16BIT)
# undef UDPNM_START_SEC_VAR_INIT_16BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif /* UDPNM_START_SEC_VAR_INIT_16BIT */
#if defined(UDPNM_STOP_SEC_VAR_INIT_16BIT)
# undef UDPNM_STOP_SEC_VAR_INIT_16BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_INIT_16BIT */

#if defined(UDPNM_START_SEC_VAR_INIT_32BIT)
# undef UDPNM_START_SEC_VAR_INIT_32BIT                 /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif /* UDPNM_START_SEC_VAR_INIT_32BIT */
#if defined(UDPNM_STOP_SEC_VAR_INIT_32BIT)
# undef UDPNM_STOP_SEC_VAR_INIT_32BIT                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_INIT_32BIT */

#if defined(UDPNM_START_SEC_VAR_INIT_UNSPECIFIED)
# undef UDPNM_START_SEC_VAR_INIT_UNSPECIFIED           /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif /* UDPNM_START_SEC_VAR_INIT_UNSPECIFIED */
#if defined(UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED */

#if defined(UDPNM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef UDPNM_START_SEC_VAR_ZERO_INIT_8BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif /* UDPNM_START_SEC_VAR_ZERO_INIT_8BIT */
#if defined(UDPNM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef UDPNM_STOP_SEC_VAR_ZERO_INIT_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_ZERO_INIT_8BIT */

#if defined(UDPNM_START_SEC_VAR_ZERO_INIT_16BIT)
# undef UDPNM_START_SEC_VAR_ZERO_INIT_16BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif /* UDPNM_START_SEC_VAR_ZERO_INIT_16BIT */
#if defined(UDPNM_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef UDPNM_STOP_SEC_VAR_ZERO_INIT_16BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_ZERO_INIT_16BIT */

#if defined(UDPNM_START_SEC_VAR_ZERO_INIT_32BIT)
# undef UDPNM_START_SEC_VAR_ZERO_INIT_32BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif /* UDPNM_START_SEC_VAR_ZERO_INIT_32BIT */
#if defined(UDPNM_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef UDPNM_STOP_SEC_VAR_ZERO_INIT_32BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_ZERO_INIT_32BIT */

#if defined(UDPNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef UDPNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif /* UDPNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED */
#if defined(UDPNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef UDPNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif /* UDPNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED */

/**********************************************************************************************************************
 *  UDPNM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CSM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CSM_START_SEC_CODE
# undef CSM_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef CSM_STOP_SEC_CODE
# undef CSM_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef CSM_START_SEC_APPL_CODE
# undef CSM_START_SEC_APPL_CODE                    /* mapped to default code section */
# define START_SEC_CODE
#endif
#ifdef CSM_STOP_SEC_APPL_CODE
# undef CSM_STOP_SEC_APPL_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CSM_START_SEC_CONST_UNSPECIFIED
# undef CSM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef CSM_STOP_SEC_CONST_UNSPECIFIED
# undef CSM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef CSM_START_SEC_VAR_NOINIT_8BIT
# undef CSM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef CSM_STOP_SEC_VAR_NOINIT_8BIT
# undef CSM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef CSM_START_SEC_VAR_NOINIT_16BIT
# undef CSM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef CSM_STOP_SEC_VAR_NOINIT_16BIT
# undef CSM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef CSM_START_SEC_VAR_ZERO_INIT_8BIT
# undef CSM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef CSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CSM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef CSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef CSM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef CSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CSM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FVM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(FVM_START_SEC_CODE)
# undef FVM_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(FVM_STOP_SEC_CODE)
# undef FVM_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(FVM_START_SEC_CODE_FAST)
# undef FVM_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(FVM_STOP_SEC_CODE_FAST)
# undef FVM_STOP_SEC_CODE_FAST  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(FVM_START_SEC_CODE_ISR)
# undef FVM_START_SEC_CODE_ISR  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR     /* mapped to default ISR code section */
#endif
#if defined(FVM_STOP_SEC_CODE_ISR)
# undef FVM_STOP_SEC_CODE_ISR   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(FVM_START_SEC_CONST_8BIT)
# undef FVM_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(FVM_STOP_SEC_CONST_8BIT)
# undef FVM_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(FVM_START_SEC_CONST_16BIT)
# undef FVM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(FVM_STOP_SEC_CONST_16BIT)
# undef FVM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_CONST_32BIT)
# undef FVM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(FVM_STOP_SEC_CONST_32BIT)
# undef FVM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


#if defined(FVM_START_SEC_CONST_64BIT)
# undef FVM_START_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_64BIT
#endif
#if defined(FVM_STOP_SEC_CONST_64BIT)
# undef FVM_STOP_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


#if defined(FVM_START_SEC_CONST_UNSPECIFIED)
# undef FVM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_CONST_UNSPECIFIED)
# undef FVM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#if defined(FVM_START_SEC_FAST_CONST_8BIT)
# undef FVM_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(FVM_STOP_SEC_FAST_CONST_8BIT)
# undef FVM_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_FAST_CONST_16BIT)
# undef FVM_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(FVM_STOP_SEC_FAST_CONST_16BIT)
# undef FVM_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_FAST_CONST_32BIT)
# undef FVM_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(FVM_STOP_SEC_FAST_CONST_32BIT)
# undef FVM_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_FAST_CONST_64BIT)
# undef FVM_START_SEC_FAST_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_64BIT
#endif
#if defined(FVM_STOP_SEC_FAST_CONST_64BIT)
# undef FVM_STOP_SEC_FAST_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_FAST_CONST_UNSPECIFIED)
# undef FVM_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef FVM_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* NOTE: ASR3.x till ASR4.0.1 only. Root table for postbuild data */
#if defined(FVM_START_SEC_PBCFG_ROOT)
# undef FVM_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(FVM_STOP_SEC_PBCFG_ROOT)
# undef FVM_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* ESCAN00065501 */
/* NOTE: ASR4.0.3 (CFG5) only. The origin root table for postbuild data. Relevant only for EcuM. */
#if defined(FVM_START_SEC_PBCFG_GLOBALROOT)
# undef FVM_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_PBCFG_GLOBALROOT
#endif
#if defined(FVM_STOP_SEC_PBCFG_GLOBALROOT)
# undef FVM_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(FVM_START_SEC_PBCFG)
# undef FVM_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(FVM_STOP_SEC_PBCFG)
# undef FVM_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(FVM_START_SEC_VAR_INIT_8BIT)
# undef FVM_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(FVM_STOP_SEC_VAR_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(FVM_START_SEC_VAR_INIT_16BIT)
# undef FVM_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_INIT_32BIT)
# undef FVM_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_INIT_64BIT)
# undef FVM_START_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(FVM_START_SEC_VAR_NOINIT_8BIT)
# undef FVM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOINIT_8BIT)
# undef FVM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOINIT_16BIT)
# undef FVM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOINIT_16BIT)
# undef FVM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOINIT_32BIT)
# undef FVM_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOINIT_32BIT)
# undef FVM_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOINIT_64BIT)
# undef FVM_START_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOINIT_64BIT)
# undef FVM_STOP_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(FVM_START_SEC_VAR_PBCFG)
# undef FVM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(FVM_STOP_SEC_VAR_PBCFG)
# undef FVM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(FVM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef FVM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_ZERO_INIT_16BIT)
# undef FVM_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_ZERO_INIT_32BIT)
# undef FVM_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_ZERO_INIT_64BIT)
# undef FVM_START_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_ZERO_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#if defined(FVM_START_SEC_VAR_FAST_INIT_8BIT)
# undef FVM_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_INIT_16BIT)
# undef FVM_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_INIT_32BIT)
# undef FVM_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_INIT_64BIT)
# undef FVM_START_SEC_VAR_FAST_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_FAST_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#if defined(FVM_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef FVM_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef FVM_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef FVM_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef FVM_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef FVM_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef FVM_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_NOINIT_64BIT)
# undef FVM_START_SEC_VAR_FAST_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_NOINIT_64BIT)
# undef FVM_STOP_SEC_VAR_FAST_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(FVM_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef FVM_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef FVM_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef FVM_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_ZERO_INIT_64BIT)
# undef FVM_START_SEC_VAR_FAST_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_ZERO_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_FAST_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(FVM_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef FVM_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef FVM_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef FVM_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_INIT_64BIT)
# undef FVM_START_SEC_VAR_NOCACHE_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(FVM_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef FVM_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef FVM_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef FVM_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_NOINIT_64BIT)
# undef FVM_START_SEC_VAR_NOCACHE_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_NOINIT_64BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT)
# undef FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_64BIT)
# undef FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef FVM_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef FVM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  FVM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  BSWM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef BSWM_START_SEC_CODE
# undef BSWM_START_SEC_CODE
# define START_SEC_CODE                                 /* mapped to default code section*/
#endif

#ifdef BSWM_STOP_SEC_CODE
# undef BSWM_STOP_SEC_CODE
# define STOP_SEC_CODE                                  /* default code stop section*/
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef BSWM_START_SEC_CONST_8BIT
# undef BSWM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                           /* mapped to default const section */
#endif

#ifdef BSWM_STOP_SEC_CONST_8BIT
# undef BSWM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef BSWM_START_SEC_CONST_16BIT
# undef BSWM_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT                          /* mapped to default const section */
#endif

#ifdef BSWM_STOP_SEC_CONST_16BIT
# undef BSWM_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef BSWM_START_SEC_CONST_32BIT
# undef BSWM_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT                          /* mapped to default const section */
#endif

#ifdef BSWM_STOP_SEC_CONST_32BIT
# undef BSWM_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef BSWM_START_SEC_CONST_UNSPECIFIED
# undef BSWM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED                    /* mapped to default const section */
#endif

#ifdef BSWM_STOP_SEC_CONST_UNSPECIFIED
# undef BSWM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

#ifdef BSWM_START_SEC_PBCFG
# undef BSWM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG                          /* mapped to default const section */
#endif

#ifdef BSWM_STOP_SEC_PBCFG
# undef BSWM_STOP_SEC_PBCFG
# define STOP_SEC_CONST                                 /* default const stop section */
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef BSWM_START_SEC_VAR_NOINIT_8BIT
# undef BSWM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT                      /* mapped to default var section */
#endif

#ifdef BSWM_STOP_SEC_VAR_NOINIT_8BIT
# undef BSWM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR                                   /* default var stop section*/
#endif

#ifdef BSWM_START_SEC_VAR_NOINIT_16BIT
# undef  BSWM_START_SEC_VAR_NOINIT_16BIT                /* mapped to default var section */
# define START_SEC_VAR_NOINIT_16BIT
#endif

#ifdef BSWM_STOP_SEC_VAR_NOINIT_16BIT
# undef BSWM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef BSWM_START_SEC_VAR_NOINIT_32BIT
# undef  BSWM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT                     /* mapped to default var section */
#endif

#ifdef BSWM_STOP_SEC_VAR_NOINIT_32BIT
# undef BSWM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR                                   /* default var stop section*/
#endif

#ifdef BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED               /* mapped to default var section */
#endif

#ifdef BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef BSWM_START_SEC_VAR_ZERO_INIT_8BIT
# undef BSWM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT                   /* mapped to default var section */
#endif

#ifdef BSWM_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef BSWM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

#ifdef BSWM_START_SEC_VAR_PBCFG
# undef BSWM_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG                            /* mapped to default var section */
#endif

#ifdef BSWM_STOP_SEC_VAR_PBCFG
# undef BSWM_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR                                   /* default var stop section */
#endif

/**********************************************************************************************************************
 *  BSWM END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  SYSSERVICE_ASR4ECUM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef ECUM_START_SEC_CODE
# undef ECUM_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef ECUM_STOP_SEC_CODE
# undef ECUM_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef ECUM_START_SEC_CONST_UNSPECIFIED
# undef ECUM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ECUM_STOP_SEC_CONST_UNSPECIFIED
# undef ECUM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef ECUM_START_SEC_CONST_8BIT
# undef ECUM_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef ECUM_STOP_SEC_CONST_8BIT
# undef ECUM_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

/* Postbuild CFG CONST sections */

#ifdef ECUM_START_SEC_PBCFG
# undef ECUM_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef ECUM_STOP_SEC_PBCFG
# undef ECUM_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef ECUM_START_SEC_PBCFG_GLOBALROOT
# undef ECUM_START_SEC_PBCFG_GLOBALROOT
# define START_SEC_PBCFG_GLOBALROOT
#endif
#ifdef ECUM_STOP_SEC_PBCFG_GLOBALROOT
# undef ECUM_STOP_SEC_PBCFG_GLOBALROOT
# define STOP_SEC_CONST
#endif



/*******  VAR sections  **********************************************************************************************/


/* VAR NOINIT sections */

#ifdef ECUM_START_SEC_VAR_NOINIT_8BIT
# undef ECUM_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ECUM_STOP_SEC_VAR_NOINIT_8BIT
# undef ECUM_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ECUM_START_SEC_VAR_NOINIT_16BIT
# undef ECUM_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ECUM_STOP_SEC_VAR_NOINIT_16BIT
# undef ECUM_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ECUM_START_SEC_VAR_NOINIT_32BIT
# undef ECUM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ECUM_STOP_SEC_VAR_NOINIT_32BIT
# undef ECUM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef ECUM_START_SEC_VAR_NOCACHE_NOINIT_32BIT
# undef ECUM_START_SEC_VAR_NOCACHE_NOINIT_32BIT
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#ifdef ECUM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT
# undef ECUM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT
# define STOP_SEC_VAR
#endif


/**********************************************************************************************************************
 *  SYSSERVICE_ASR4ECUM END
 *********************************************************************************************************************/


/* -------------------- FiM Start Sections -------------------- */

#ifdef FIM_START_SEC_CONST_UNSPECIFIED
# undef FIM_START_SEC_CONST_UNSPECIFIED                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /*mapped to default const section */
#endif

#ifdef FIM_START_SEC_CALIB_UNSPECIFIED
# undef FIM_START_SEC_CALIB_UNSPECIFIED                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /*mapped to default const section */
#endif

#ifdef FIM_START_SEC_LCFG
# undef FIM_START_SEC_LCFG                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /*mapped to default const section */
#endif

#ifdef FIM_START_SEC_PBCFG
# undef FIM_START_SEC_PBCFG                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED                    /*mapped to default const section */
#endif

#ifdef FIM_START_SEC_PBCFG_ROOT
# undef FIM_START_SEC_PBCFG_ROOT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG                          /*mapped to default const section */
#endif

#ifdef FIM_START_SEC_CONST_8BIT
# undef FIM_START_SEC_CONST_8BIT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                            /*mapped to default const section*/
#endif

#ifdef FIM_START_SEC_CONST_16BIT
# undef FIM_START_SEC_CONST_16BIT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT                            /*mapped to default const section*/
#endif

#ifdef FIM_START_SEC_VAR_NOINIT_8BIT
# undef FIM_START_SEC_VAR_NOINIT_8BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT                         /*mapped to default var section*/
#endif

#ifdef FIM_START_SEC_VAR_INIT_8BIT
# undef FIM_START_SEC_VAR_INIT_8BIT                      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT                           /*mapped to default var section*/
#endif

#ifdef FIM_START_SEC_VAR_NOINIT_16BIT
# undef FIM_START_SEC_VAR_NOINIT_16BIT                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT                        /*mapped to default var section*/
#endif

#ifdef FIM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FIM_START_SEC_VAR_NOINIT_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED                  /*mapped to default var section*/
#endif

#ifdef FIM_START_SEC_CODE
# undef FIM_START_SEC_CODE                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                   /*mapped to default code section*/
#endif

/* -------------------- FiM Stop Sections -------------------- */

#ifdef FIM_STOP_SEC_CONST_UNSPECIFIED
# undef FIM_STOP_SEC_CONST_UNSPECIFIED                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /*mapped to default const section */
#endif

#ifdef FIM_STOP_SEC_CALIB_UNSPECIFIED
# undef FIM_STOP_SEC_CALIB_UNSPECIFIED                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /*mapped to default const section */
#endif

#ifdef FIM_STOP_SEC_LCFG
# undef FIM_STOP_SEC_LCFG                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /*mapped to default const section */
#endif

#ifdef FIM_STOP_SEC_PBCFG
# undef FIM_STOP_SEC_PBCFG                               /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /*mapped to default const section */
#endif

#ifdef FIM_STOP_SEC_PBCFG_ROOT
# undef FIM_STOP_SEC_PBCFG_ROOT                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                 /*mapped to default const section */
#endif

#ifdef FIM_STOP_SEC_CONST_8BIT
# undef FIM_STOP_SEC_CONST_8BIT                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                  /*mapped to default const section*/
#endif

#ifdef FIM_STOP_SEC_CONST_16BIT
# undef FIM_STOP_SEC_CONST_16BIT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST                                  /*mapped to default const section*/
#endif

#ifdef FIM_STOP_SEC_VAR_NOINIT_8BIT
# undef FIM_STOP_SEC_VAR_NOINIT_8BIT                     /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                      /*mapped to default var section*/
#endif

#ifdef FIM_STOP_SEC_VAR_INIT_8BIT
# undef FIM_STOP_SEC_VAR_INIT_8BIT                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                      /*mapped to default var section*/
#endif

#ifdef FIM_STOP_SEC_VAR_NOINIT_16BIT
# undef FIM_STOP_SEC_VAR_NOINIT_16BIT                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                      /*mapped to default var section*/
#endif

#ifdef FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                                      /*mapped to default var section*/
#endif

#ifdef FIM_STOP_SEC_CODE
# undef FIM_STOP_SEC_CODE                                /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE                                    /*mapped to default code section*/
#endif


/**********************************************************************************************************************
 *  WDGM START
 *********************************************************************************************************************/

#include "WdgM_OsMemMap.h" 

/*******  CODE sections **********************************************************************************************/

#if defined WDGM_START_SEC_CODE
# undef WDGM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined WDGM_STOP_SEC_CODE
# undef WDGM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

#if defined WDGM_START_SEC_CONST_8BIT
# undef WDGM_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined WDGM_STOP_SEC_CONST_8BIT
# undef WDGM_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGM_START_SEC_CONST_16BIT
# undef WDGM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined WDGM_STOP_SEC_CONST_16BIT
# undef WDGM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGM_START_SEC_CONST_32BIT
# undef WDGM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined WDGM_STOP_SEC_CONST_32BIT
# undef WDGM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined WDGM_START_SEC_CONST_UNSPECIFIED
# undef WDGM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined WDGM_STOP_SEC_CONST_UNSPECIFIED
# undef WDGM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined WDGM_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef WDGM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined WDGM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef WDGM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_NOINIT_8BIT
# undef WDGM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined WDGM_STOP_SEC_VAR_NOINIT_8BIT
# undef WDGM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_NOINIT_16BIT
# undef WDGM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined WDGM_STOP_SEC_VAR_NOINIT_16BIT
# undef WDGM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_NOINIT_32BIT
# undef WDGM_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined WDGM_STOP_SEC_VAR_NOINIT_32BIT
# undef WDGM_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR INIT sections */

#if defined WDGM_START_SEC_VAR_8BIT
# undef WDGM_START_SEC_VAR_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined WDGM_STOP_SEC_VAR_8BIT
# undef WDGM_STOP_SEC_VAR_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_16BIT
# undef WDGM_START_SEC_VAR_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT  
#endif
#if defined WDGM_STOP_SEC_VAR_16BIT
# undef WDGM_STOP_SEC_VAR_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_32BIT
# undef WDGM_START_SEC_VAR_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined WDGM_STOP_SEC_VAR_32BIT
# undef WDGM_STOP_SEC_VAR_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined WDGM_START_SEC_VAR_BOOLEAN
# undef WDGM_START_SEC_VAR_BOOLEAN /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined WDGM_STOP_SEC_VAR_BOOLEAN
# undef WDGM_STOP_SEC_VAR_BOOLEAN /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* Global Shared Variables of the WdgM.
 * If a SafeContext OS is used this section will be defined by the OS.
 * If no SafeContext OS is used this section has to be mapped to a memory area where all SWCs has access
 */
/*#if defined GlobalShared_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef GlobalShared_START_SEC_VAR_NOINIT_UNSPECIFIED*/ /* PRQA S 0841 */ /* MD_MSR_19.6 */
/*# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined GlobalShared_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef GlobalShared_STOP_SEC_VAR_NOINIT_UNSPECIFIED*/ /* PRQA S 0841 */ /* MD_MSR_19.6 */
/*# define STOP_SEC_VAR
#endif*/
/* or the following section (depending on which OS version is used) */
/*#if defined OS_START_SEC_GLOBALSHARED_VAR_NOINIT_UNSPECIFIED
# undef OS_START_SEC_GLOBALSHARED_VAR_NOINIT_UNSPECIFIED*/ /* PRQA S 0841 */ /* MD_MSR_19.6 */
/*# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined OS_STOP_SEC_GLOBALSHARED_VAR_NOINIT_UNSPECIFIED
# undef OS_STOP_SEC_GLOBALSHARED_VAR_NOINIT_UNSPECIFIED*/ /* PRQA S 0841 */ /* MD_MSR_19.6 */
/*# define STOP_SEC_VAR
#endif*/
 
/**********************************************************************************************************************
 *  WDGM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SYSSERVICE_ASRCRC START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef CRC_START_SEC_CODE
    #undef CRC_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef CRC_STOP_SEC_CODE
    #undef CRC_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/
#ifdef CRC_START_SEC_CONST_8
    #undef CRC_START_SEC_CONST_8
    #define START_SEC_CONST_8
#endif
#ifdef CRC_STOP_SEC_CONST_8
    #undef CRC_STOP_SEC_CONST_8
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_16
    #undef CRC_START_SEC_CONST_16
    #define START_SEC_CONST_16
#endif
#ifdef CRC_STOP_SEC_CONST_16
    #undef CRC_STOP_SEC_CONST_16
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_32
    #undef CRC_START_SEC_CONST_32
    #define START_SEC_CONST_32
#endif
#ifdef CRC_STOP_SEC_CONST_32
    #undef CRC_STOP_SEC_CONST_32
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_64
    #undef CRC_START_SEC_CONST_64
    #define START_SEC_CONST_64
#endif
#ifdef CRC_STOP_SEC_CONST_64
    #undef CRC_STOP_SEC_CONST_64
    #define STOP_SEC_CONST
#endif

/**********************************************************************************************************************
 *  SYSSERVICE_ASRCRC END
 *********************************************************************************************************************/

#ifdef DET_START_SEC_CODE
# undef DET_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef DET_STOP_SEC_CODE
# undef DET_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

#ifdef DET_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef DET_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef DET_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef DET_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef DET_START_SEC_VAR_NOINIT_8BIT
# undef DET_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef DET_STOP_SEC_VAR_NOINIT_8BIT
# undef DET_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef DET_START_SEC_VAR_INIT_UNSPECIFIED
# undef DET_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef DET_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef DET_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef DET_START_SEC_CONST_UNSPECIFIED
# undef DET_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef DET_STOP_SEC_CONST_UNSPECIFIED
# undef DET_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/**********************************************************************************************************************
 *  ETM START
 *********************************************************************************************************************/

/*******  CODE sections  *********************************************************************************************/

#if defined(ETM_START_SEC_CODE)
# undef ETM_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE    /* mapped to default code section */
#endif
#if defined(ETM_STOP_SEC_CODE)
# undef ETM_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE    /* default code stop section */
#endif

#if defined(ETM_START_SEC_CODE_FAST)
# undef ETM_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(ETM_STOP_SEC_CODE_FAST)
# undef ETM_STOP_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE         /* default code stop section */
#endif

#if defined(ETM_START_SEC_CODE_ISR)
# undef ETM_START_SEC_CODE_ISR /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR    /* mapped to default ISR code section */
#endif
#if defined(ETM_STOP_SEC_CODE_ISR)
# undef ETM_STOP_SEC_CODE_ISR /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE        /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(ETM_START_SEC_CONST_8BIT)
# undef ETM_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT    /* mapped to default const 8bit section */
#endif
#if defined(ETM_STOP_SEC_CONST_8BIT)
# undef ETM_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST         /* default const stop section */
#endif

#if defined(ETM_START_SEC_CONST_16BIT)
# undef ETM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(ETM_STOP_SEC_CONST_16BIT)
# undef ETM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_CONST_32BIT)
# undef ETM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(ETM_STOP_SEC_CONST_32BIT)
# undef ETM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_CONST_64BIT)
# undef ETM_START_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_64BIT
#endif
#if defined(ETM_STOP_SEC_CONST_64BIT)
# undef ETM_STOP_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_CONST_UNSPECIFIED)
# undef ETM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_CONST_UNSPECIFIED)
# undef ETM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* FAST CONST sections */

#if defined(ETM_START_SEC_FAST_CONST_8BIT)
# undef ETM_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(ETM_STOP_SEC_FAST_CONST_8BIT)
# undef ETM_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_FAST_CONST_16BIT)
# undef ETM_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(ETM_STOP_SEC_FAST_CONST_16BIT)
# undef ETM_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_FAST_CONST_32BIT)
# undef ETM_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(ETM_STOP_SEC_FAST_CONST_32BIT)
# undef ETM_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_FAST_CONST_64BIT)
# undef ETM_START_SEC_FAST_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_64BIT
#endif
#if defined(ETM_STOP_SEC_FAST_CONST_64BIT)
# undef ETM_STOP_SEC_FAST_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(ETM_START_SEC_FAST_CONST_UNSPECIFIED)
# undef ETM_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef ETM_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* Postbuild CFG CONST sections */

#if defined(ETM_START_SEC_PBCFG)
# undef ETM_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(ETM_STOP_SEC_PBCFG)
# undef ETM_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(ETM_START_SEC_VAR_INIT_8BIT)
# undef ETM_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT    /* mapped to default var init 8bit section */
#endif
#if defined(ETM_STOP_SEC_VAR_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR              /* default var stop section */
#endif

#if defined(ETM_START_SEC_VAR_INIT_16BIT)
# undef ETM_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_INIT_32BIT)
# undef ETM_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_INIT_64BIT)
# undef ETM_START_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#if defined(ETM_START_SEC_VAR_NOINIT_8BIT)
# undef ETM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOINIT_8BIT)
# undef ETM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOINIT_16BIT)
# undef ETM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOINIT_16BIT)
# undef ETM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOINIT_32BIT)
# undef ETM_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOINIT_32BIT)
# undef ETM_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOINIT_64BIT)
# undef ETM_START_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOINIT_64BIT)
# undef ETM_STOP_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(ETM_START_SEC_VAR_PBCFG)
# undef ETM_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(ETM_STOP_SEC_VAR_PBCFG)
# undef ETM_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(ETM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef ETM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_ZERO_INIT_16BIT)
# undef ETM_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_ZERO_INIT_32BIT)
# undef ETM_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_ZERO_INIT_64BIT)
# undef ETM_START_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_ZERO_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */

#if defined(ETM_START_SEC_VAR_FAST_INIT_8BIT)
# undef ETM_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_INIT_16BIT)
# undef ETM_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_INIT_32BIT)
# undef ETM_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_INIT_64BIT)
# undef ETM_START_SEC_VAR_FAST_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_FAST_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST NOINIT sections */

#if defined(ETM_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef ETM_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef ETM_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef ETM_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef ETM_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef ETM_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef ETM_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_NOINIT_64BIT)
# undef ETM_START_SEC_VAR_FAST_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_NOINIT_64BIT)
# undef ETM_STOP_SEC_VAR_FAST_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(ETM_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef ETM_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef ETM_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef ETM_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_ZERO_INIT_64BIT)
# undef ETM_START_SEC_VAR_FAST_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_ZERO_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_FAST_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(ETM_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef ETM_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef ETM_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef ETM_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_INIT_64BIT)
# undef ETM_START_SEC_VAR_NOCACHE_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(ETM_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef ETM_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef ETM_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef ETM_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_NOINIT_64BIT)
# undef ETM_START_SEC_VAR_NOCACHE_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_NOINIT_64BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT)
# undef ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_64BIT
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_64BIT)
# undef ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef ETM_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef ETM_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETM END
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  SysService_AsrRamTst START
 *********************************************************************************************************************/

#ifdef RAMTST_START_SEC_CODE
  #undef RAMTST_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CODE
#endif

#ifdef RAMTST_STOP_SEC_CODE
  #undef RAMTST_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CODE
#endif

#ifdef RAMTST_START_SEC_VAR_INIT_UNSPECIFIED
  #undef RAMTST_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef RAMTST_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef RAMTST_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_VAR
#endif

#ifdef RAMTST_START_SEC_VAR_NO_INIT_8
  #undef RAMTST_START_SEC_VAR_NO_INIT_8 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_VAR_NO_INIT_8
#endif

#ifdef RAMTST_STOP_SEC_VAR_NO_INIT_8
  #undef RAMTST_STOP_SEC_VAR_NO_INIT_8 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_VAR
#endif

#ifdef RAMTST_START_SEC_VAR_NO_INIT_32
  #undef RAMTST_START_SEC_VAR_NO_INIT_32 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_VAR_NO_INIT_32
#endif

#ifdef RAMTST_STOP_SEC_VAR_NO_INIT_32
  #undef RAMTST_STOP_SEC_VAR_NO_INIT_32 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_VAR
#endif

#ifdef RAMTST_START_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef RAMTST_START_SEC_VAR_NO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#endif

#ifdef RAMTST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef RAMTST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_VAR
#endif

#ifdef RAMTST_START_SEC_CONST_32
  #undef RAMTST_START_SEC_CONST_32 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CONST_32
#endif

#ifdef RAMTST_STOP_SEC_CONST_32
  #undef RAMTST_STOP_SEC_CONST_32 /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CONST
#endif

#ifdef RAMTST_START_SEC_CONST_UNSPECIFIED
  #undef RAMTST_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef RAMTST_STOP_SEC_CONST_UNSPECIFIED
  #undef RAMTST_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CONST
#endif

#ifdef RAMTST_START_SEC_CONST_PBCFG
  #undef RAMTST_START_SEC_CONST_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef RAMTST_STOP_SEC_CONST_PBCFG
  #undef RAMTST_STOP_SEC_CONST_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CONST
#endif

#ifdef RAMTST_START_SEC_CONST_PBCFG_ROOT
  #undef RAMTST_START_SEC_CONST_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define START_SEC_CONST_UNSPECIFIED
#endif

#ifdef RAMTST_STOP_SEC_CONST_PBCFG_ROOT
  #undef RAMTST_STOP_SEC_CONST_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
  #define STOP_SEC_CONST
#endif

/**********************************************************************************************************************
 *  SysService_AsrRamTst END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SD START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef SD_START_SEC_CODE
# undef SD_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef SD_STOP_SEC_CODE
# undef SD_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef SD_START_SEC_CONST_8BIT
# undef SD_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef SD_STOP_SEC_CONST_8BIT
# undef SD_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef SD_START_SEC_CONST_16BIT
# undef SD_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef SD_STOP_SEC_CONST_16BIT
# undef SD_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef SD_START_SEC_CONST_32BIT
# undef SD_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef SD_STOP_SEC_CONST_32BIT
# undef SD_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef SD_START_SEC_CONST_UNSPECIFIED
# undef SD_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef SD_STOP_SEC_CONST_UNSPECIFIED
# undef SD_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef SD_START_SEC_PBCFG
# undef SD_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef SD_STOP_SEC_PBCFG
# undef SD_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef SD_START_SEC_VAR_INIT_8BIT
# undef SD_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef SD_STOP_SEC_VAR_INIT_8BIT
# undef SD_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef SD_START_SEC_VAR_INIT_16BIT
# undef SD_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef SD_STOP_SEC_VAR_INIT_16BIT
# undef SD_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_INIT_32BIT
# undef SD_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef SD_STOP_SEC_VAR_INIT_32BIT
# undef SD_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_INIT_UNSPECIFIED
# undef SD_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef SD_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef SD_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef SD_START_SEC_VAR_NOINIT_8BIT
# undef SD_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef SD_STOP_SEC_VAR_NOINIT_8BIT
# undef SD_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_NOINIT_16BIT
# undef SD_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef SD_STOP_SEC_VAR_NOINIT_16BIT
# undef SD_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_NOINIT_32BIT
# undef SD_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef SD_STOP_SEC_VAR_NOINIT_32BIT
# undef SD_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef SD_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef SD_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef SD_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_PBCFG
# undef SD_START_SEC_VAR_PBCFG
# define START_SEC_VAR_PBCFG
#endif
#ifdef SD_STOP_SEC_VAR_PBCFG
# undef SD_STOP_SEC_VAR_PBCFG
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef SD_START_SEC_VAR_ZERO_INIT_8BIT
# undef SD_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef SD_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef SD_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_ZERO_INIT_16BIT
# undef SD_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef SD_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef SD_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_ZERO_INIT_32BIT
# undef SD_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef SD_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef SD_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef SD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef SD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  SD END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  STBM START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(STBM_START_SEC_CODE)
# undef STBM_START_SEC_CODE
# define START_SEC_CODE /* mapped to default code section */
#endif
#if defined(STBM_STOP_SEC_CODE)
# undef STBM_STOP_SEC_CODE
# define STOP_SEC_CODE  /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(STBM_START_SEC_CONST_UNSPECIFIED)
# undef STBM_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(STBM_STOP_SEC_CONST_UNSPECIFIED)
# undef STBM_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(STBM_START_SEC_VAR_NOINIT_32BIT)
# undef STBM_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(STBM_STOP_SEC_VAR_NOINIT_32BIT)
# undef STBM_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined(STBM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef STBM_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(STBM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef STBM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(STBM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef STBM_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(STBM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef STBM_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  STBM END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CANTSYN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(CANTSYN_START_SEC_CODE)
# undef CANTSYN_START_SEC_CODE
# define START_SEC_CODE          /* mapped to default code section */
#endif
#if defined(CANTSYN_STOP_SEC_CODE)
# undef CANTSYN_STOP_SEC_CODE
# define STOP_SEC_CODE           /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(CANTSYN_START_SEC_CONST_UNSPECIFIED)
# undef CANTSYN_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(CANTSYN_STOP_SEC_CONST_UNSPECIFIED)
# undef CANTSYN_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#if defined(CANTSYN_START_SEC_CONST_8BIT)
# undef CANTSYN_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#if defined(CANTSYN_STOP_SEC_CONST_8BIT)
# undef CANTSYN_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(CANTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CANTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(CANTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef CANTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#if defined(CANTSYN_START_SEC_VAR_NOINIT_32BIT)
# undef CANTSYN_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(CANTSYN_STOP_SEC_VAR_NOINIT_32BIT)
# undef CANTSYN_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined(CANTSYN_START_SEC_VAR_NOINIT_8BIT)
# undef CANTSYN_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(CANTSYN_STOP_SEC_VAR_NOINIT_8BIT)
# undef CANTSYN_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(CANTSYN_START_SEC_VAR_ZERO_INIT_8BIT)
# undef CANTSYN_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(CANTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef CANTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CANTSYN END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  ETHTSYN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETHTSYN_START_SEC_CODE
  #undef ETHTSYN_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef ETHTSYN_STOP_SEC_CODE
  #undef ETHTSYN_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef ETHTSYN_START_SEC_CONST_8BIT
# undef ETHTSYN_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_8BIT
# undef ETHTSYN_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef ETHTSYN_START_SEC_CONST_16BIT
# undef ETHTSYN_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_16BIT
# undef ETHTSYN_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHTSYN_START_SEC_CONST_32BIT
# undef ETHTSYN_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_32BIT
# undef ETHTSYN_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHTSYN_START_SEC_CONST_UNSPECIFIED
# undef ETHTSYN_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
# undef ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef ETHTSYN_START_SEC_VAR_INIT_8BIT
# undef ETHTSYN_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_8BIT
# undef ETHTSYN_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_16BIT
# undef ETHTSYN_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_16BIT
# undef ETHTSYN_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_32BIT
# undef ETHTSYN_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_32BIT
# undef ETHTSYN_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
# undef ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_8BIT
# undef ETHTSYN_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHTSYN_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_16BIT
# undef ETHTSYN_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHTSYN_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_32BIT
# undef ETHTSYN_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHTSYN_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_ZERO_INIT_16BIT
# undef ETHTSYN_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#if defined(ETHTSYN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef ETHTSYN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(ETHTSYN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHTSYN END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  FRTSYN START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(FRTSYN_START_SEC_CODE)
# undef FRTSYN_START_SEC_CODE
# define START_SEC_CODE          /* mapped to default code section */
#endif
#if defined(FRTSYN_STOP_SEC_CODE)
# undef FRTSYN_STOP_SEC_CODE
# define STOP_SEC_CODE           /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(FRTSYN_START_SEC_CONST_UNSPECIFIED)
# undef FRTSYN_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(FRTSYN_STOP_SEC_CONST_UNSPECIFIED)
# undef FRTSYN_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#if defined(FRTSYN_START_SEC_CONST_8BIT)
# undef FRTSYN_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#if defined(FRTSYN_STOP_SEC_CONST_8BIT)
# undef FRTSYN_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(FRTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef FRTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(FRTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef FRTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#if defined(FRTSYN_START_SEC_VAR_NOINIT_32BIT)
# undef FRTSYN_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(FRTSYN_STOP_SEC_VAR_NOINIT_32BIT)
# undef FRTSYN_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#if defined(FRTSYN_START_SEC_VAR_NOINIT_8BIT)
# undef FRTSYN_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(FRTSYN_STOP_SEC_VAR_NOINIT_8BIT)
# undef FRTSYN_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(FRTSYN_START_SEC_VAR_ZERO_INIT_8BIT)
# undef FRTSYN_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(FRTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef FRTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  FRTSYN END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SysService_E2eLib START
 *********************************************************************************************************************/


/*******  CODE sections **********************************************************************************************/

#ifdef E2E_START_SEC_CODE
    #undef E2E_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef E2E_STOP_SEC_CODE
    #undef E2E_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/**********************************************************************************************************************
 *  SysService_E2eLib END
 *********************************************************************************************************************/

/* E2EPW_MemMap.inc: End-To-End Protection Wrapper Memory Mapping: */


/* Code */
/* To be used for mapping code to application block, boot block, external flash etc. */
#if defined   E2EPW_START_SEC_CODE
#undef        E2EPW_START_SEC_CODE
#define           START_SEC_CODE

#elif defined E2EPW_STOP_SEC_CODE
#undef        E2EPW_STOP_SEC_CODE
#define             STOP_SEC_CODE


/* Constants */
/* To be used for global or static constants.  */
#elif defined E2EPW_START_SEC_CONST_UNSPECIFIED
#undef        E2EPW_START_SEC_CONST_UNSPECIFIED
#define             START_SEC_CONST_UNSPECIFIED

#elif defined E2EPW_STOP_SEC_CONST_UNSPECIFIED
#undef        E2EPW_STOP_SEC_CONST_UNSPECIFIED
#define             STOP_SEC_CONST


/* Var init unspecified */
#elif defined E2EPW_START_SEC_VAR_INIT_UNSPECIFIED
#undef        E2EPW_START_SEC_VAR_INIT_UNSPECIFIED
#define             START_SEC_VAR_INIT_UNSPECIFIED

#elif defined E2EPW_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef        E2EPW_STOP_SEC_VAR_INIT_UNSPECIFIED
#define             STOP_SEC_VAR


/* Var uninitialized unspecified */
/* To be used for all global or static variables that are never initialized */
#elif defined E2EPW_START_SEC_VAR_NOINIT_UNSPECIFIED
#undef        E2EPW_START_SEC_VAR_NOINIT_UNSPECIFIED
#define             START_SEC_VAR_NOINIT_UNSPECIFIED

#elif defined E2EPW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#undef        E2EPW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#define             STOP_SEC_VAR


/* Var zero init unspecified */
#elif defined E2EPW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#undef        E2EPW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#define             START_SEC_VAR_ZERO_INIT_UNSPECIFIED

#elif defined E2EPW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#undef        E2EPW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#define             STOP_SEC_VAR


/* Library code     */
/* To be used for code that shall go into library segments for <MSN> module. */
#elif defined E2EPW_START_SEC_CODE_LIB
#undef        E2EPW_START_SEC_CODE_LIB
#define             START_SEC_CODE

#elif defined E2EPW_STOP_SEC_CODE_LIB
#undef        E2EPW_STOP_SEC_CODE_LIB
#define             STOP_SEC_CODE

#endif

/* End of file E2EPW_MemMap.inc */

/**********************************************************************************************************************
 *  EXI START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef EXI_START_SEC_CODE
  #undef EXI_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef EXI_STOP_SEC_CODE
  #undef EXI_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef EXI_START_SEC_APPL_CODE
# undef EXI_START_SEC_APPL_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif

#ifdef EXI_STOP_SEC_APPL_CODE
# undef EXI_STOP_SEC_APPL_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef EXI_START_SEC_CONST_8BIT
# undef EXI_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef EXI_STOP_SEC_CONST_8BIT
# undef EXI_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef EXI_START_SEC_CONST_16BIT
# undef EXI_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef EXI_STOP_SEC_CONST_16BIT
# undef EXI_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef EXI_START_SEC_CONST_32BIT
# undef EXI_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef EXI_STOP_SEC_CONST_32BIT
# undef EXI_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef EXI_START_SEC_CONST_UNSPECIFIED
# undef EXI_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef EXI_STOP_SEC_CONST_UNSPECIFIED
# undef EXI_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef EXI_START_SEC_VAR_ZERO_INIT_8BIT
# undef EXI_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef EXI_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef EXI_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef EXI_START_SEC_VAR_NOINIT_8BIT
# undef EXI_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef EXI_STOP_SEC_VAR_NOINIT_8BIT
# undef EXI_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef EXI_START_SEC_VAR_NOINIT_16BIT
# undef EXI_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef EXI_STOP_SEC_VAR_NOINIT_16BIT
# undef EXI_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef EXI_START_SEC_VAR_NOINIT_32BIT
# undef EXI_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef EXI_STOP_SEC_VAR_NOINIT_32BIT
# undef EXI_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef EXI_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef EXI_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef EXI_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef EXI_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  EXI END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  ETHFW START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETHFW_START_SEC_CODE
# undef ETHFW_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef ETHFW_STOP_SEC_CODE
# undef ETHFW_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef ETHFW_START_SEC_CONST_8BIT
# undef ETHFW_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef ETHFW_STOP_SEC_CONST_8BIT
# undef ETHFW_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef ETHFW_START_SEC_CONST_16BIT
# undef ETHFW_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef ETHFW_STOP_SEC_CONST_16BIT
# undef ETHFW_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHFW_START_SEC_CONST_32BIT
# undef ETHFW_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef ETHFW_STOP_SEC_CONST_32BIT
# undef ETHFW_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef ETHFW_START_SEC_CONST_UNSPECIFIED
# undef ETHFW_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ETHFW_STOP_SEC_CONST_UNSPECIFIED
# undef ETHFW_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef ETHFW_START_SEC_VAR_INIT_8BIT
# undef ETHFW_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef ETHFW_STOP_SEC_VAR_INIT_8BIT
# undef ETHFW_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef ETHFW_START_SEC_VAR_INIT_16BIT
# undef ETHFW_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_INIT_16BIT
# undef ETHFW_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_INIT_32BIT
# undef ETHFW_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_INIT_32BIT
# undef ETHFW_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_INIT_UNSPECIFIED
# undef ETHFW_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef ETHFW_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef ETHFW_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef ETHFW_START_SEC_VAR_NOINIT_8BIT
# undef ETHFW_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_NOINIT_8BIT
# undef ETHFW_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_NOINIT_16BIT
# undef ETHFW_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_NOINIT_16BIT
# undef ETHFW_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_NOINIT_32BIT
# undef ETHFW_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_NOINIT_32BIT
# undef ETHFW_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHFW_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ETHFW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef ETHFW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef ETHFW_START_SEC_VAR_ZERO_INIT_8BIT
# undef ETHFW_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef ETHFW_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_ZERO_INIT_16BIT
# undef ETHFW_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef ETHFW_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_ZERO_INIT_32BIT
# undef ETHFW_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef ETHFW_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef ETHFW_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef ETHFW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHFW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef ETHFW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef ETHFW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHFW END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  IPBASE START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef IPBASE_START_SEC_CODE
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_CODE
#define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef IPBASE_STOP_SEC_CODE
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_CODE
#define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef IPBASE_START_SEC_CONST_8BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_CONST_8BIT
#define START_SEC_CONST_8BIT
#endif
#ifdef IPBASE_STOP_SEC_CONST_8BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_CONST_8BIT
#define STOP_SEC_CONST
#endif

#ifdef IPBASE_START_SEC_CONST_16BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_CONST_16BIT
#define START_SEC_CONST_16BIT
#endif
#ifdef IPBASE_STOP_SEC_CONST_16BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_CONST_16BIT
#define STOP_SEC_CONST
#endif

#ifdef IPBASE_START_SEC_CONST_32BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_CONST_32BIT
#define START_SEC_CONST_32BIT
#endif
#ifdef IPBASE_STOP_SEC_CONST_32BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_CONST_32BIT
#define STOP_SEC_CONST
#endif

#ifdef IPBASE_START_SEC_CONST_UNSPECIFIED
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_CONST_UNSPECIFIED
#define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef IPBASE_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_CONST_UNSPECIFIED
#define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef IPBASE_START_SEC_VAR_NOINIT_8BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_VAR_NOINIT_8BIT
#define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef IPBASE_STOP_SEC_VAR_NOINIT_8BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_VAR_NOINIT_8BIT
#define STOP_SEC_VAR
#endif

#ifdef IPBASE_START_SEC_VAR_NOINIT_16BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_VAR_NOINIT_16BIT
#define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef IPBASE_STOP_SEC_VAR_NOINIT_16BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_VAR_NOINIT_16BIT
#define STOP_SEC_VAR
#endif

#ifdef IPBASE_START_SEC_VAR_NOINIT_32BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_VAR_NOINIT_32BIT
#define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef IPBASE_STOP_SEC_VAR_NOINIT_32BIT
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_VAR_NOINIT_32BIT
#define STOP_SEC_VAR
#endif

#ifdef IPBASE_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_START_SEC_VAR_NOINIT_UNSPECIFIED
#define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef IPBASE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 0841 */ /* MD_MSR_19.6 */
#undef IPBASE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  IPBASE END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SRP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(SRP_START_SEC_CODE)
# undef SRP_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE         /* mapped to default code section */
#endif
#if defined(SRP_STOP_SEC_CODE)
# undef SRP_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(SRP_START_SEC_CODE_FAST)
# undef SRP_START_SEC_CODE_FAST /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_FAST    /* mapped to default fast code section */
#endif
#if defined(SRP_STOP_SEC_CODE_FAST)
# undef SRP_STOP_SEC_CODE_FAST  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif

#if defined(SRP_START_SEC_CODE_ISR)
# undef SRP_START_SEC_CODE_ISR  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE_ISR     /* mapped to default ISR code section */
#endif
#if defined(SRP_STOP_SEC_CODE_ISR)
# undef SRP_STOP_SEC_CODE_ISR   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE          /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(SRP_START_SEC_CONST_8BIT)
# undef SRP_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT     /* mapped to default const 8bit section */
#endif
#if defined(SRP_STOP_SEC_CONST_8BIT)
# undef SRP_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST           /* default const stop section */
#endif

#if defined(SRP_START_SEC_CONST_16BIT)
# undef SRP_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(SRP_STOP_SEC_CONST_16BIT)
# undef SRP_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_CONST_32BIT)
# undef SRP_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(SRP_STOP_SEC_CONST_32BIT)
# undef SRP_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_CONST_UNSPECIFIED)
# undef SRP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_CONST_UNSPECIFIED)
# undef SRP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#if defined(SRP_START_SEC_FAST_CONST_8BIT)
# undef SRP_START_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_8BIT
#endif
#if defined(SRP_STOP_SEC_FAST_CONST_8BIT)
# undef SRP_STOP_SEC_FAST_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_FAST_CONST_16BIT)
# undef SRP_START_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_16BIT
#endif
#if defined(SRP_STOP_SEC_FAST_CONST_16BIT)
# undef SRP_STOP_SEC_FAST_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_FAST_CONST_32BIT)
# undef SRP_START_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_32BIT
#endif
#if defined(SRP_STOP_SEC_FAST_CONST_32BIT)
# undef SRP_STOP_SEC_FAST_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_FAST_CONST_UNSPECIFIED)
# undef SRP_START_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_FAST_CONST_UNSPECIFIED)
# undef SRP_STOP_SEC_FAST_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* NOTE: ASR3.x till ASR4.0.1 only. Root table for postbuild data */
#if defined(SRP_START_SEC_PBCFG_ROOT)
# undef SRP_START_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(SRP_STOP_SEC_PBCFG_ROOT)
# undef SRP_STOP_SEC_PBCFG_ROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* ESCAN00065501 */
/* NOTE: ASR4.0.3 (CFG5) only. The origin root table for postbuild data. Relevant only for EcuM. */
#if defined(SRP_START_SEC_PBCFG_GLOBALROOT)
# undef SRP_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_PBCFG_GLOBALROOT
#endif
#if defined(SRP_STOP_SEC_PBCFG_GLOBALROOT)
# undef SRP_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(SRP_START_SEC_PBCFG)
# undef SRP_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#if defined(SRP_STOP_SEC_PBCFG)
# undef SRP_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(SRP_START_SEC_VAR_INIT_8BIT)
# undef SRP_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT      /* mapped to default var init 8bit section */
#endif
#if defined(SRP_STOP_SEC_VAR_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR                /* default var stop section */
#endif

#if defined(SRP_START_SEC_VAR_INIT_16BIT)
# undef SRP_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_INIT_32BIT)
# undef SRP_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(SRP_START_SEC_VAR_NOINIT_8BIT)
# undef SRP_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOINIT_8BIT)
# undef SRP_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOINIT_16BIT)
# undef SRP_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOINIT_16BIT)
# undef SRP_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOINIT_32BIT)
# undef SRP_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOINIT_32BIT)
# undef SRP_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* ESCAN00065501 */
#if defined(SRP_START_SEC_VAR_PBCFG)
# undef SRP_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#if defined(SRP_STOP_SEC_VAR_PBCFG)
# undef SRP_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(SRP_START_SEC_VAR_ZERO_INIT_8BIT)
# undef SRP_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_ZERO_INIT_16BIT)
# undef SRP_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_ZERO_INIT_32BIT)
# undef SRP_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */

#if defined(SRP_START_SEC_VAR_FAST_INIT_8BIT)
# undef SRP_START_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_FAST_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_INIT_16BIT)
# undef SRP_START_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_FAST_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_INIT_32BIT)
# undef SRP_START_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_FAST_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#if defined(SRP_START_SEC_VAR_FAST_NOINIT_8BIT)
# undef SRP_START_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_NOINIT_8BIT)
# undef SRP_STOP_SEC_VAR_FAST_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_NOINIT_16BIT)
# undef SRP_START_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_NOINIT_16BIT)
# undef SRP_STOP_SEC_VAR_FAST_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_NOINIT_32BIT)
# undef SRP_START_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_NOINIT_32BIT)
# undef SRP_STOP_SEC_VAR_FAST_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#if defined(SRP_START_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef SRP_START_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef SRP_START_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef SRP_START_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE INIT sections */

#if defined(SRP_START_SEC_VAR_NOCACHE_INIT_8BIT)
# undef SRP_START_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_INIT_16BIT)
# undef SRP_START_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_INIT_32BIT)
# undef SRP_START_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_NOCACHE_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE NOINIT sections */

#if defined(SRP_START_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef SRP_START_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef SRP_START_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef SRP_START_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR NOCACHE ZERO INIT sections */

#if defined(SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_16BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_32BIT
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT)
# undef SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef SRP_START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED
#endif
#if defined(SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED)
# undef SRP_STOP_SEC_VAR_NOCACHE_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  SRP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SCC START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined SCC_START_SEC_CODE
  #undef SCC_START_SEC_CODE                         /* PRQA S 0841 */
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#if defined SCC_STOP_SEC_CODE
  #undef SCC_STOP_SEC_CODE                          /* PRQA S 0841 */
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined SCC_START_SEC_CONST_8BIT
# undef SCC_START_SEC_CONST_8BIT                    /* PRQA S 0841 */
# define START_SEC_CONST_8BIT
#endif
#if defined SCC_STOP_SEC_CONST_8BIT
# undef SCC_STOP_SEC_CONST_8BIT                     /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_16BIT
# undef SCC_START_SEC_CONST_16BIT                   /* PRQA S 0841 */
# define START_SEC_CONST_16BIT
#endif
#if defined SCC_STOP_SEC_CONST_16BIT
# undef SCC_STOP_SEC_CONST_16BIT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_32BIT
# undef SCC_START_SEC_CONST_32BIT                    /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#if defined SCC_STOP_SEC_CONST_32BIT
# undef SCC_STOP_SEC_CONST_32BIT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_UNSPECIFIED
# undef SCC_START_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_CONST_UNSPECIFIED
# undef SCC_STOP_SEC_CONST_UNSPECIFIED              /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined SCC_START_SEC_VAR_NOINIT_8BIT
# undef SCC_START_SEC_VAR_NOINIT_8BIT               /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_8BIT
# undef SCC_STOP_SEC_VAR_NOINIT_8BIT                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_16BIT
# undef SCC_START_SEC_VAR_NOINIT_16BIT              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_16BIT
# undef SCC_STOP_SEC_VAR_NOINIT_16BIT               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_32BIT
# undef SCC_START_SEC_VAR_NOINIT_32BIT              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_32BIT
# undef SCC_STOP_SEC_VAR_NOINIT_32BIT               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef SCC_START_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED         /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_8BIT
# undef SCC_START_SEC_VAR_ZERO_INIT_8BIT            /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef SCC_STOP_SEC_VAR_ZERO_INIT_8BIT             /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_16BIT
# undef SCC_START_SEC_VAR_ZERO_INIT_16BIT           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef SCC_STOP_SEC_VAR_ZERO_INIT_16BIT            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_32BIT
# undef SCC_START_SEC_VAR_ZERO_INIT_32BIT           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef SCC_STOP_SEC_VAR_ZERO_INIT_32BIT            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SCC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef SCC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED      /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/*******  NVM CONST sections  ****************************************************************************************/

#if defined SCC_START_SEC_CONST_8BIT_NVM
# undef SCC_START_SEC_CONST_8BIT_NVM                    /* PRQA S 0841 */
# define START_SEC_CONST_8BIT
#endif
#if defined SCC_STOP_SEC_CONST_8BIT_NVM
# undef SCC_STOP_SEC_CONST_8BIT_NVM                     /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_16BIT_NVM
# undef SCC_START_SEC_CONST_16BIT_NVM                   /* PRQA S 0841 */
# define START_SEC_CONST_16BIT
#endif
#if defined SCC_STOP_SEC_CONST_16BIT_NVM
# undef SCC_STOP_SEC_CONST_16BIT_NVM                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_32BIT_NVM
# undef SCC_START_SEC_CONST_32BIT_NVM                    /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#if defined SCC_STOP_SEC_CONST_32BIT_NVM
# undef SCC_STOP_SEC_CONST_32BIT_NVM                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined SCC_START_SEC_CONST_UNSPECIFIED_NVM
# undef SCC_START_SEC_CONST_UNSPECIFIED_NVM             /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_CONST_UNSPECIFIED_NVM
# undef SCC_STOP_SEC_CONST_UNSPECIFIED_NVM              /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/*******  NVM VAR sections  ******************************************************************************************/

#if defined SCC_START_SEC_VAR_NOINIT_8BIT_NVM
# undef SCC_START_SEC_VAR_NOINIT_8BIT_NVM               /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_8BIT_NVM
# undef SCC_STOP_SEC_VAR_NOINIT_8BIT_NVM                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_16BIT_NVM
# undef SCC_START_SEC_VAR_NOINIT_16BIT_NVM              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_16BIT_NVM
# undef SCC_STOP_SEC_VAR_NOINIT_16BIT_NVM               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_32BIT_NVM
# undef SCC_START_SEC_VAR_NOINIT_32BIT_NVM              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_32BIT_NVM
# undef SCC_STOP_SEC_VAR_NOINIT_32BIT_NVM               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_NOINIT_UNSPECIFIED_NVM
# undef SCC_START_SEC_VAR_NOINIT_UNSPECIFIED_NVM        /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED_NVM
# undef SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED_NVM         /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_8BIT_NVM
# undef SCC_START_SEC_VAR_ZERO_INIT_8BIT_NVM            /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_8BIT_NVM
# undef SCC_STOP_SEC_VAR_ZERO_INIT_8BIT_NVM             /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_16BIT_NVM
# undef SCC_START_SEC_VAR_ZERO_INIT_16BIT_NVM           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_16BIT_NVM
# undef SCC_STOP_SEC_VAR_ZERO_INIT_16BIT_NVM            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_32BIT_NVM
# undef SCC_START_SEC_VAR_ZERO_INIT_32BIT_NVM           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_32BIT_NVM
# undef SCC_STOP_SEC_VAR_ZERO_INIT_32BIT_NVM            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined SCC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED_NVM
# undef SCC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED_NVM     /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined SCC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED_NVM
# undef SCC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED_NVM      /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  SCC END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef TLS_START_SEC_CODE
  #undef TLS_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef TLS_STOP_SEC_CODE
  #undef TLS_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef TLS_START_SEC_CONST_8BIT
# undef TLS_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef TLS_STOP_SEC_CONST_8BIT
# undef TLS_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef TLS_START_SEC_CONST_16BIT
# undef TLS_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef TLS_STOP_SEC_CONST_16BIT
# undef TLS_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef TLS_START_SEC_CONST_32BIT
# undef TLS_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef TLS_STOP_SEC_CONST_32BIT
# undef TLS_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef TLS_START_SEC_CONST_UNSPECIFIED
# undef TLS_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef TLS_STOP_SEC_CONST_UNSPECIFIED
# undef TLS_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef TLS_START_SEC_PBCFG
# undef TLS_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef TLS_STOP_SEC_PBCFG
# undef TLS_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef TLS_START_SEC_PBCFG_ROOT
# undef TLS_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef TLS_STOP_SEC_PBCFG_ROOT
# undef TLS_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#ifdef TLS_START_SEC_VAR_NOINIT_8BIT
# undef TLS_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef TLS_STOP_SEC_VAR_NOINIT_8BIT
# undef TLS_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef TLS_START_SEC_VAR_NOINIT_16BIT
# undef TLS_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef TLS_STOP_SEC_VAR_NOINIT_16BIT
# undef TLS_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef TLS_START_SEC_VAR_NOINIT_32BIT
# undef TLS_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef TLS_STOP_SEC_VAR_NOINIT_32BIT
# undef TLS_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef TLS_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef TLS_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef TLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef TLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef TLS_START_SEC_VAR_ZERO_INIT_8BIT
# undef TLS_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef TLS_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef TLS_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  TLS END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  VCANCCCDM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(VCANCCCDM_START_SEC_CODE)
# undef VCANCCCDM_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined(VCANCCCDM_STOP_SEC_CODE)
# undef VCANCCCDM_STOP_SEC_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

#if defined(VCANCCCDM_START_SEC_APPL_CODE)
# undef VCANCCCDM_START_SEC_APPL_CODE      /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE
#endif
#if defined(VCANCCCDM_STOP_SEC_APPL_CODE)
# undef VCANCCCDM_STOP_SEC_APPL_CODE       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(VCANCCCDM_START_SEC_CONST_8BIT)
# undef VCANCCCDM_START_SEC_CONST_8BIT  /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_CONST_8BIT)
# undef VCANCCCDM_STOP_SEC_CONST_8BIT   /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VCANCCCDM_START_SEC_CONST_16BIT)
# undef VCANCCCDM_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_CONST_16BIT)
# undef VCANCCCDM_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(VCANCCCDM_START_SEC_CONST_32BIT)
# undef VCANCCCDM_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_CONST_32BIT)
# undef VCANCCCDM_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


#if defined(VCANCCCDM_START_SEC_CONST_64BIT)
# undef VCANCCCDM_START_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_64BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_CONST_64BIT)
# undef VCANCCCDM_STOP_SEC_CONST_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


#if defined(VCANCCCDM_START_SEC_CONST_UNSPECIFIED)
# undef VCANCCCDM_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(VCANCCCDM_STOP_SEC_CONST_UNSPECIFIED)
# undef VCANCCCDM_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#if defined(VCANCCCDM_START_SEC_VAR_INIT_8BIT)
# undef VCANCCCDM_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_INIT_8BIT)
# undef VCANCCCDM_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_INIT_16BIT)
# undef VCANCCCDM_START_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_INIT_16BIT)
# undef VCANCCCDM_STOP_SEC_VAR_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_INIT_32BIT)
# undef VCANCCCDM_START_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_INIT_32BIT)
# undef VCANCCCDM_STOP_SEC_VAR_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_INIT_64BIT)
# undef VCANCCCDM_START_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_64BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_INIT_64BIT)
# undef VCANCCCDM_STOP_SEC_VAR_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_INIT_UNSPECIFIED)
# undef VCANCCCDM_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_INIT_UNSPECIFIED)
# undef VCANCCCDM_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#if defined(VCANCCCDM_START_SEC_VAR_NOINIT_8BIT)
# undef VCANCCCDM_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_NOINIT_8BIT)
# undef VCANCCCDM_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_NOINIT_16BIT)
# undef VCANCCCDM_START_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_NOINIT_16BIT)
# undef VCANCCCDM_STOP_SEC_VAR_NOINIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_NOINIT_32BIT)
# undef VCANCCCDM_START_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_NOINIT_32BIT)
# undef VCANCCCDM_STOP_SEC_VAR_NOINIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_NOINIT_64BIT)
# undef VCANCCCDM_START_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_64BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_NOINIT_64BIT)
# undef VCANCCCDM_STOP_SEC_VAR_NOINIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VCANCCCDM_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
# undef VCANCCCDM_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#if defined(VCANCCCDM_START_SEC_VAR_ZERO_INIT_8BIT)
# undef VCANCCCDM_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_8BIT)
# undef VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_ZERO_INIT_16BIT)
# undef VCANCCCDM_START_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_16BIT)
# undef VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_ZERO_INIT_32BIT)
# undef VCANCCCDM_START_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_32BIT)
# undef VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_ZERO_INIT_64BIT)
# undef VCANCCCDM_START_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_64BIT
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_64BIT)
# undef VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_64BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined(VCANCCCDM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VCANCCCDM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
# undef VCANCCCDM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  VCANCCCDM END
 *********************************************************************************************************************/
 


/**********************************************************************************************************************
 *  VCANCCGBT START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef VCANCCGBT_START_SEC_CODE
# undef VCANCCGBT_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef VCANCCGBT_STOP_SEC_CODE
# undef VCANCCGBT_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE /* default code stop section */
#endif

#ifdef VCANCCGBT_START_SEC_APPL_CODE
# undef VCANCCGBT_START_SEC_APPL_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef VCANCCGBT_STOP_SEC_APPL_CODE
# undef VCANCCGBT_STOP_SEC_APPL_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef VCANCCGBT_START_SEC_CONST_UNSPECIFIED
# undef VCANCCGBT_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef VCANCCGBT_STOP_SEC_CONST_UNSPECIFIED
# undef VCANCCGBT_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */


/* Postbuild CFG CONST sections */

#ifdef VCANCCGBT_START_SEC_PBCFG
# undef VCANCCGBT_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef VCANCCGBT_STOP_SEC_PBCFG
# undef VCANCCGBT_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */


/* VAR NOINIT sections */

#ifdef VCANCCGBT_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef VCANCCGBT_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef VCANCCGBT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef VCANCCGBT_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef VCANCCGBT_START_SEC_VAR_ZERO_INIT_8BIT
# undef VCANCCGBT_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef VCANCCGBT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef VCANCCGBT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */


/* Postbuild CFG VAR sections */

#ifdef VCANCCGBT_START_SEC_VAR_PBCFG
# undef VCANCCGBT_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef VCANCCGBT_STOP_SEC_VAR_PBCFG
# undef VCANCCGBT_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  VCANCCGBT END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  J1939TP START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef J1939TP_START_SEC_CODE
# undef J1939TP_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef J1939TP_STOP_SEC_CODE
# undef J1939TP_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef J1939TP_START_SEC_CONST_8BIT
# undef J1939TP_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT /* mapped to default const 8bit section */
#endif
#ifdef J1939TP_STOP_SEC_CONST_8BIT
# undef J1939TP_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST /* default const stop section */
#endif

#ifdef J1939TP_START_SEC_CONST_16BIT
# undef J1939TP_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#ifdef J1939TP_STOP_SEC_CONST_16BIT
# undef J1939TP_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939TP_START_SEC_CONST_32BIT
# undef J1939TP_START_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_32BIT
#endif
#ifdef J1939TP_STOP_SEC_CONST_32BIT
# undef J1939TP_STOP_SEC_CONST_32BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef J1939TP_START_SEC_CONST_UNSPECIFIED
# undef J1939TP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef J1939TP_STOP_SEC_CONST_UNSPECIFIED
# undef J1939TP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */


/* Postbuild CFG CONST sections */

#ifdef J1939TP_START_SEC_PBCFG
# undef J1939TP_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef J1939TP_STOP_SEC_PBCFG
# undef J1939TP_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef J1939TP_START_SEC_VAR_ZERO_INIT_8BIT
# undef J1939TP_START_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef J1939TP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef J1939TP_STOP_SEC_VAR_ZERO_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef J1939TP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939TP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef J1939TP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef J1939TP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef J1939TP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939TP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef J1939TP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef J1939TP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR FAST INIT sections */


/* VAR FAST NOINIT sections */


/* VAR FAST ZERO INIT sections */


/* Postbuild CFG VAR sections */

#ifdef J1939TP_START_SEC_VAR_PBCFG
# undef J1939TP_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef J1939TP_STOP_SEC_VAR_PBCFG
# undef J1939TP_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  J1939TP END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CANTP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef   CANTP_START_SEC_CODE
# undef  CANTP_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE      /* mapped to default code section */
#endif
#ifdef   CANTP_STOP_SEC_CODE
# undef  CANTP_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE      /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef   CANTP_START_SEC_CONST_UNSPECIFIED
# undef  CANTP_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef   CANTP_STOP_SEC_CONST_UNSPECIFIED
# undef  CANTP_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#if defined(CANTP_START_SEC_CONST_16BIT)
# undef CANTP_START_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_16BIT
#endif
#if defined(CANTP_STOP_SEC_CONST_16BIT)
# undef CANTP_STOP_SEC_CONST_16BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

#ifdef   CANTP_START_SEC_CONST_8BIT
# undef  CANTP_START_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_8BIT
#endif
#ifdef   CANTP_STOP_SEC_CONST_8BIT
# undef  CANTP_STOP_SEC_CONST_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif

/* Postbuild CFG CONST sections */

#ifdef   CANTP_START_SEC_PBCFG
# undef  CANTP_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CONST_PBCFG
#endif
#ifdef   CANTP_STOP_SEC_PBCFG
# undef  CANTP_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CONST
#endif



/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef   CANTP_START_SEC_VAR_INIT_8BIT
# undef  CANTP_START_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_INIT_8BIT
#endif
#ifdef   CANTP_STOP_SEC_VAR_INIT_8BIT
# undef  CANTP_STOP_SEC_VAR_INIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef   CANTP_START_SEC_VAR_NOINIT_8BIT
# undef  CANTP_START_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef   CANTP_STOP_SEC_VAR_NOINIT_8BIT
# undef  CANTP_STOP_SEC_VAR_NOINIT_8BIT /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef   CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef  CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef   CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef  CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#ifdef   CANTP_START_SEC_VAR_PBCFG
# undef  CANTP_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_PBCFG
#endif
#ifdef   CANTP_STOP_SEC_VAR_PBCFG
# undef  CANTP_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif



/**********************************************************************************************************************
 *  CANTP END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DOIP START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined DOIP_START_SEC_CODE
# undef DOIP_START_SEC_CODE                         /* PRQA S 0841 */
# define START_SEC_CODE                             /* mapped to default code section */
#endif
#if defined DOIP_STOP_SEC_CODE
# undef DOIP_STOP_SEC_CODE                          /* PRQA S 0841 */
# define STOP_SEC_CODE                              /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined DOIP_START_SEC_CONST_8BIT
# undef DOIP_START_SEC_CONST_8BIT                   /* PRQA S 0841 */
# define START_SEC_CONST_8BIT
#endif
#if defined DOIP_STOP_SEC_CONST_8BIT
# undef DOIP_STOP_SEC_CONST_8BIT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined DOIP_START_SEC_CONST_16BIT
# undef DOIP_START_SEC_CONST_16BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_16BIT
#endif
#if defined DOIP_STOP_SEC_CONST_16BIT
# undef DOIP_STOP_SEC_CONST_16BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined DOIP_START_SEC_CONST_32BIT
# undef DOIP_START_SEC_CONST_32BIT                  /* PRQA S 0841 */
# define START_SEC_CONST_32BIT
#endif
#if defined DOIP_STOP_SEC_CONST_32BIT
# undef DOIP_STOP_SEC_CONST_32BIT                   /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined DOIP_START_SEC_CONST_UNSPECIFIED
# undef DOIP_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */
# define START_SEC_CONST_UNSPECIFIED
#endif
#if defined DOIP_STOP_SEC_CONST_UNSPECIFIED
# undef DOIP_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined DOIP_START_SEC_PBCFG
# undef DOIP_START_SEC_PBCFG                        /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#if defined DOIP_STOP_SEC_PBCFG
# undef DOIP_STOP_SEC_PBCFG                         /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

#if defined DOIP_START_SEC_PBCFG_ROOT
# undef DOIP_START_SEC_PBCFG_ROOT                   /* PRQA S 0841 */
# define START_SEC_CONST_PBCFG
#endif
#if defined DOIP_STOP_SEC_PBCFG_ROOT
# undef DOIP_STOP_SEC_PBCFG_ROOT                    /* PRQA S 0841 */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

#if defined DOIP_START_SEC_VAR_NOINIT_8BIT
# undef DOIP_START_SEC_VAR_NOINIT_8BIT              /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#if defined DOIP_STOP_SEC_VAR_NOINIT_8BIT
# undef DOIP_STOP_SEC_VAR_NOINIT_8BIT               /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_NOINIT_16BIT
# undef DOIP_START_SEC_VAR_NOINIT_16BIT             /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#if defined DOIP_STOP_SEC_VAR_NOINIT_16BIT
# undef DOIP_STOP_SEC_VAR_NOINIT_16BIT              /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_NOINIT_32BIT
# undef DOIP_START_SEC_VAR_NOINIT_32BIT             /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined DOIP_STOP_SEC_VAR_NOINIT_32BIT
# undef DOIP_STOP_SEC_VAR_NOINIT_32BIT              /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef DOIP_START_SEC_VAR_NOINIT_UNSPECIFIED       /* PRQA S 0841 */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined DOIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef DOIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_INIT_8BIT
# undef DOIP_START_SEC_VAR_INIT_8BIT                /* PRQA S 0841 */
# define START_SEC_VAR_INIT_8BIT
#endif
#if defined DOIP_STOP_SEC_VAR_INIT_8BIT
# undef DOIP_STOP_SEC_VAR_INIT_8BIT                 /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_INIT_16BIT
# undef DOIP_START_SEC_VAR_INIT_16BIT               /* PRQA S 0841 */
# define START_SEC_VAR_INIT_16BIT
#endif
#if defined DOIP_STOP_SEC_VAR_INIT_16BIT
# undef DOIP_STOP_SEC_VAR_INIT_16BIT                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_INIT_32BIT
# undef DOIP_START_SEC_VAR_INIT_32BIT               /* PRQA S 0841 */
# define START_SEC_VAR_INIT_32BIT
#endif
#if defined DOIP_STOP_SEC_VAR_INIT_32BIT
# undef DOIP_STOP_SEC_VAR_INIT_32BIT                /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_INIT_UNSPECIFIED
# undef DOIP_START_SEC_VAR_INIT_UNSPECIFIED         /* PRQA S 0841 */
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#if defined DOIP_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef DOIP_STOP_SEC_VAR_INIT_UNSPECIFIED          /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_ZERO_INIT_8BIT
# undef DOIP_START_SEC_VAR_ZERO_INIT_8BIT           /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined DOIP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef DOIP_STOP_SEC_VAR_ZERO_INIT_8BIT            /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_ZERO_INIT_16BIT
# undef DOIP_START_SEC_VAR_ZERO_INIT_16BIT          /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#if defined DOIP_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef DOIP_STOP_SEC_VAR_ZERO_INIT_16BIT           /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_ZERO_INIT_32BIT
# undef DOIP_START_SEC_VAR_ZERO_INIT_32BIT          /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#if defined DOIP_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef DOIP_STOP_SEC_VAR_ZERO_INIT_32BIT           /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

#if defined DOIP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DOIP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED    /* PRQA S 0841 */
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined DOIP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef DOIP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED     /* PRQA S 0841 */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  DOIP END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TCPIP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef TCPIP_START_SEC_CODE
  #undef TCPIP_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef TCPIP_STOP_SEC_CODE
  #undef TCPIP_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef TCPIP_START_SEC_CODE_ISR
  #undef TCPIP_START_SEC_CODE_ISR
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#ifdef TCPIP_STOP_SEC_CODE_ISR
  #undef TCPIP_STOP_SEC_CODE_ISR
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef IPV4_START_SEC_CODE
  #undef IPV4_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef IPV4_STOP_SEC_CODE
  #undef IPV4_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef IPV6_START_SEC_CODE
  #undef IPV6_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef IPV6_STOP_SEC_CODE
  #undef IPV6_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef IPV6_START_SEC_CODE_FAST
  #undef IPV6_START_SEC_CODE_FAST
  #define START_SEC_CODE_FAST                       /* mapped to default fast code section */
#endif
#ifdef IPV6_STOP_SEC_CODE_FAST
  #undef IPV6_STOP_SEC_CODE_FAST
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

#ifdef IPV6_START_SEC_CODE_ISR
  #undef IPV6_START_SEC_CODE_ISR
  #define START_SEC_CODE_ISR                        /* mapped to default ISR code section */
#endif
#ifdef IPV6_STOP_SEC_CODE_ISR
  #undef IPV6_STOP_SEC_CODE_ISR
  #define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef TCPIP_START_SEC_CONST_8BIT
# undef TCPIP_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef TCPIP_STOP_SEC_CONST_8BIT
# undef TCPIP_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_CONST_16BIT
# undef TCPIP_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef TCPIP_STOP_SEC_CONST_16BIT
# undef TCPIP_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_CONST_32BIT
# undef TCPIP_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef TCPIP_STOP_SEC_CONST_32BIT
# undef TCPIP_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_CONST_UNSPECIFIED
# undef TCPIP_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef TCPIP_STOP_SEC_CONST_UNSPECIFIED
# undef TCPIP_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_CONST_BOOLEAN
# undef TCPIP_START_SEC_CONST_BOOLEAN
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef TCPIP_STOP_SEC_CONST_BOOLEAN
# undef TCPIP_STOP_SEC_CONST_BOOLEAN
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_PBCFG
# undef TCPIP_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef TCPIP_STOP_SEC_PBCFG
# undef TCPIP_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

#ifdef TCPIP_START_SEC_PBCFG_ROOT
# undef TCPIP_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef TCPIP_STOP_SEC_PBCFG_ROOT
# undef TCPIP_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

#ifdef IPV4_START_SEC_CONST_8BIT
# undef IPV4_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef IPV4_STOP_SEC_CONST_8BIT
# undef IPV4_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV4_START_SEC_CONST_16BIT
# undef IPV4_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef IPV4_STOP_SEC_CONST_16BIT
# undef IPV4_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV4_START_SEC_CONST_32BIT
# undef IPV4_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef IPV4_STOP_SEC_CONST_32BIT
# undef IPV4_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV4_START_SEC_CONST_UNSPECIFIED
# undef IPV4_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef IPV4_STOP_SEC_CONST_UNSPECIFIED
# undef IPV4_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_CONST_8BIT
# undef IPV6_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef IPV6_STOP_SEC_CONST_8BIT
# undef IPV6_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_CONST_16BIT
# undef IPV6_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef IPV6_STOP_SEC_CONST_16BIT
# undef IPV6_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_CONST_32BIT
# undef IPV6_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef IPV6_STOP_SEC_CONST_32BIT
# undef IPV6_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_CONST_UNSPECIFIED
# undef IPV6_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_CONST_UNSPECIFIED
# undef IPV6_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/* FAST CONST sections */

#ifdef IPV6_START_SEC_FAST_CONST_8BIT
# undef IPV6_START_SEC_FAST_CONST_8BIT
# define START_SEC_FAST_CONST_8BIT
#endif
#ifdef IPV6_STOP_SEC_FAST_CONST_8BIT
# undef IPV6_STOP_SEC_FAST_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_FAST_CONST_16BIT
# undef IPV6_START_SEC_FAST_CONST_16BIT
# define START_SEC_FAST_CONST_16BIT
#endif
#ifdef IPV6_STOP_SEC_FAST_CONST_16BIT
# undef IPV6_STOP_SEC_FAST_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_FAST_CONST_32BIT
# undef IPV6_START_SEC_FAST_CONST_32BIT
# define START_SEC_FAST_CONST_32BIT
#endif
#ifdef IPV6_STOP_SEC_FAST_CONST_32BIT
# undef IPV6_STOP_SEC_FAST_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_FAST_CONST_UNSPECIFIED
# undef IPV6_START_SEC_FAST_CONST_UNSPECIFIED
# define START_SEC_FAST_CONST_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_FAST_CONST_UNSPECIFIED
# undef IPV6_STOP_SEC_FAST_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/* Postbuild CFG CONST sections */

/* Root table for postbuild data */
#ifdef IPV6_START_SEC_PBCFG_ROOT
# undef IPV6_START_SEC_PBCFG_ROOT
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPV6_STOP_SEC_PBCFG_ROOT
# undef IPV6_STOP_SEC_PBCFG_ROOT
# define STOP_SEC_CONST
#endif

#ifdef IPV6_START_SEC_PBCFG
# undef IPV6_START_SEC_PBCFG
# define START_SEC_CONST_PBCFG
#endif
#ifdef IPV6_STOP_SEC_PBCFG
# undef IPV6_STOP_SEC_PBCFG
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef IPV6_START_SEC_VAR_INIT_8BIT
# undef IPV6_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef IPV6_STOP_SEC_VAR_INIT_8BIT
# undef IPV6_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef IPV6_START_SEC_VAR_INIT_16BIT
# undef IPV6_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_INIT_16BIT
# undef IPV6_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_INIT_32BIT
# undef IPV6_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_INIT_32BIT
# undef IPV6_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_INIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR NO INIT sections */

#ifdef TCPIP_START_SEC_VAR_NOINIT_8BIT
# undef TCPIP_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef TCPIP_STOP_SEC_VAR_NOINIT_8BIT
# undef TCPIP_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef TCPIP_START_SEC_VAR_NOINIT_16BIT
# undef TCPIP_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef TCPIP_STOP_SEC_VAR_NOINIT_16BIT
# undef TCPIP_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef TCPIP_START_SEC_VAR_NOINIT_32BIT
# undef TCPIP_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef TCPIP_STOP_SEC_VAR_NOINIT_32BIT
# undef TCPIP_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef TCPIP_START_SEC_VAR_NOINIT_BOOLEAN
# undef TCPIP_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef TCPIP_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef TCPIP_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_NOINIT_8BIT
# undef IPV4_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef IPV4_STOP_SEC_VAR_NOINIT_8BIT
# undef IPV4_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef TCPIP_START_SEC_VAR_BUFFERS
# undef TCPIP_START_SEC_VAR_BUFFERS
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef TCPIP_STOP_SEC_VAR_BUFFERS
# undef TCPIP_STOP_SEC_VAR_BUFFERS
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_NOINIT_16BIT
# undef IPV4_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef IPV4_STOP_SEC_VAR_NOINIT_16BIT
# undef IPV4_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_NOINIT_32BIT
# undef IPV4_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef IPV4_STOP_SEC_VAR_NOINIT_32BIT
# undef IPV4_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_NOINIT_BOOLEAN
# undef IPV4_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef IPV4_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef IPV4_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_NOINIT_8BIT
# undef IPV6_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_NOINIT_8BIT
# undef IPV6_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_NOINIT_16BIT
# undef IPV6_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_NOINIT_16BIT
# undef IPV6_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_NOINIT_32BIT
# undef IPV6_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_NOINIT_32BIT
# undef IPV6_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR FAST NOINIT sections */

#ifdef IPV6_START_SEC_VAR_FAST_NOINIT_8BIT
# undef IPV6_START_SEC_VAR_FAST_NOINIT_8BIT
# define START_SEC_VAR_FAST_NOINIT_8BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_NOINIT_8BIT
# undef IPV6_STOP_SEC_VAR_FAST_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_NOINIT_16BIT
# undef IPV6_START_SEC_VAR_FAST_NOINIT_16BIT
# define START_SEC_VAR_FAST_NOINIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_NOINIT_16BIT
# undef IPV6_STOP_SEC_VAR_FAST_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_NOINIT_32BIT
# undef IPV6_START_SEC_VAR_FAST_NOINIT_32BIT
# define START_SEC_VAR_FAST_NOINIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_NOINIT_32BIT
# undef IPV6_STOP_SEC_VAR_FAST_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef TCPIP_START_SEC_VAR_ZERO_INIT_8BIT
# undef TCPIP_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef TCPIP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef TCPIP_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV4_START_SEC_VAR_ZERO_INIT_8BIT
# undef IPV4_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef IPV4_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef IPV4_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_ZERO_INIT_8BIT
# undef IPV6_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef IPV6_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_ZERO_INIT_16BIT
# undef IPV6_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef IPV6_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_ZERO_INIT_32BIT
# undef IPV6_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef IPV6_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR FAST INIT sections */

#ifdef IPV6_START_SEC_VAR_FAST_INIT_8BIT
# undef IPV6_START_SEC_VAR_FAST_INIT_8BIT
# define START_SEC_VAR_FAST_INIT_8BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_INIT_8BIT
# undef IPV6_STOP_SEC_VAR_FAST_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_INIT_16BIT
# undef IPV6_START_SEC_VAR_FAST_INIT_16BIT
# define START_SEC_VAR_FAST_INIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_INIT_16BIT
# undef IPV6_STOP_SEC_VAR_FAST_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_INIT_32BIT
# undef IPV6_START_SEC_VAR_FAST_INIT_32BIT
# define START_SEC_VAR_FAST_INIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_INIT_32BIT
# undef IPV6_STOP_SEC_VAR_FAST_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_INIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_FAST_INIT_UNSPECIFIED
# define START_SEC_VAR_FAST_INIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR FAST ZERO INIT sections */

#ifdef IPV6_START_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef IPV6_START_SEC_VAR_FAST_ZERO_INIT_8BIT
# define START_SEC_VAR_FAST_ZERO_INIT_8BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
# undef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_ZERO_INIT_16BIT
# undef IPV6_START_SEC_VAR_FAST_ZERO_INIT_16BIT
# define START_SEC_VAR_FAST_ZERO_INIT_16BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT
# undef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_ZERO_INIT_32BIT
# undef IPV6_START_SEC_VAR_FAST_ZERO_INIT_32BIT
# define START_SEC_VAR_FAST_ZERO_INIT_32BIT
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT
# undef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef IPV6_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# undef IPV6_START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
#endif
#ifdef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# undef IPV6_STOP_SEC_VAR_FAST_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  TCPIP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  AVTP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef AVTP_START_SEC_CODE
  #undef AVTP_START_SEC_CODE
  #define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef AVTP_STOP_SEC_CODE
  #undef AVTP_STOP_SEC_CODE
  #define STOP_SEC_CODE                             /* default code stop section */
#endif


/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef AVTP_START_SEC_CONST_8BIT
# undef AVTP_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef AVTP_STOP_SEC_CONST_8BIT
# undef AVTP_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef AVTP_START_SEC_CONST_16BIT
# undef AVTP_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef AVTP_STOP_SEC_CONST_16BIT
# undef AVTP_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef AVTP_START_SEC_CONST_32BIT
# undef AVTP_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef AVTP_STOP_SEC_CONST_32BIT
# undef AVTP_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef AVTP_START_SEC_CONST_UNSPECIFIED
# undef AVTP_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef AVTP_STOP_SEC_CONST_UNSPECIFIED
# undef AVTP_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif


/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef AVTP_START_SEC_VAR_INIT_8BIT
# undef AVTP_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef AVTP_STOP_SEC_VAR_INIT_8BIT
# undef AVTP_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef AVTP_START_SEC_VAR_INIT_16BIT
# undef AVTP_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_INIT_16BIT
# undef AVTP_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_INIT_32BIT
# undef AVTP_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_INIT_32BIT
# undef AVTP_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_INIT_UNSPECIFIED
# undef AVTP_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef AVTP_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef AVTP_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR NOINIT sections */

#ifdef AVTP_START_SEC_VAR_NOINIT_8BIT
# undef AVTP_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_NOINIT_8BIT
# undef AVTP_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_NOINIT_16BIT
# undef AVTP_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_NOINIT_16BIT
# undef AVTP_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_NOINIT_32BIT
# undef AVTP_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_NOINIT_32BIT
# undef AVTP_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef AVTP_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef AVTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef AVTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif


/* VAR ZERO INIT sections */

#ifdef AVTP_START_SEC_VAR_ZERO_INIT_8BIT
# undef AVTP_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef AVTP_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_ZERO_INIT_16BIT
# undef AVTP_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef AVTP_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_ZERO_INIT_32BIT
# undef AVTP_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef AVTP_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef AVTP_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef AVTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED 
# undef AVTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED 
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED 
#endif
#ifdef AVTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED 
# undef AVTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED 
# define STOP_SEC_VAR
#endif
/**********************************************************************************************************************
 *  AVTP END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TP_ISO10681 START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined (FRTP_START_SEC_CODE)
   #undef      FRTP_START_SEC_CODE                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CODE
#endif

#if defined (FRTP_STOP_SEC_CODE)
   #undef      FRTP_STOP_SEC_CODE                    /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CODE
#endif

#if defined (FRTP_START_SEC_CODE)
   #undef      FRTP_START_SEC_CODE                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CODE
#endif

#if defined (FRTP_STOP_SEC_CODE)
   #undef      FRTP_STOP_SEC_CODE                           /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

#if defined (FRTP_START_SEC_CONST_8BIT)
   #undef      FRTP_START_SEC_CONST_8BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_8BIT
#endif

#if defined (FRTP_STOP_SEC_CONST_8BIT)
   #undef      FRTP_STOP_SEC_CONST_8BIT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

#if defined (FRTP_START_SEC_CONST_16BIT)
   #undef      FRTP_START_SEC_CONST_16BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_16BIT
#endif

#if defined (FRTP_STOP_SEC_CONST_16BIT)
   #undef      FRTP_STOP_SEC_CONST_16BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

#if defined (FRTP_START_SEC_CONST_32BIT)
   #undef      FRTP_START_SEC_CONST_32BIT            /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_32BIT
#endif

#if defined (FRTP_STOP_SEC_CONST_32BIT)
   #undef      FRTP_STOP_SEC_CONST_32BIT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

#if defined (FRTP_START_SEC_CONST_UNSPECIFIED)
   #undef      FRTP_START_SEC_CONST_UNSPECIFIED      /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_UNSPECIFIED
#endif

#if defined (FRTP_STOP_SEC_CONST_UNSPECIFIED)
   #undef      FRTP_STOP_SEC_CONST_UNSPECIFIED       /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

/*******  Postbuild CFG CONST sections *******************************************************************************/

#if defined (FRTP_START_SEC_PBCFG)
   #undef      FRTP_START_SEC_PBCFG                  /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_PBCFG
#endif

#if defined (FRTP_STOP_SEC_PBCFG)
   #undef      FRTP_STOP_SEC_PBCFG                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

#if defined (FRTP_START_SEC_PBCFG_ROOT)
   #undef      FRTP_START_SEC_PBCFG_ROOT             /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_CONST_PBCFG
#endif

#if defined (FRTP_STOP_SEC_PBCFG_ROOT)
   #undef      FRTP_STOP_SEC_PBCFG_ROOT              /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_CONST
#endif

/*******  Postbuild CFG VAR sections *********************************************************************************/

#if defined (FRTP_START_SEC_VAR_PBCFG)
   #undef      FRTP_START_SEC_VAR_PBCFG              /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_PBCFG
#endif

#if defined (FRTP_STOP_SEC_VAR_PBCFG)
   #undef      FRTP_STOP_SEC_VAR_PBCFG                   /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined (FRTP_START_SEC_VAR_NOINIT_8BIT)
   #undef      FRTP_START_SEC_VAR_NOINIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_NOINIT_8BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_NOINIT_8BIT)
   #undef      FRTP_STOP_SEC_VAR_NOINIT_8BIT         /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_NOINIT_16BIT)
   #undef      FRTP_START_SEC_VAR_NOINIT_16BIT       /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_NOINIT_16BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_NOINIT_16BIT)
   #undef      FRTP_STOP_SEC_VAR_NOINIT_16BIT        /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_NOINIT_32BIT)
   #undef      FRTP_START_SEC_VAR_NOINIT_32BIT       /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_NOINIT_32BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_NOINIT_32BIT)         /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #undef      FRTP_STOP_SEC_VAR_NOINIT_32BIT
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      FRTP_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#if defined (FRTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      FRTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

/*******  VAR ZERO INIT sections  *************************************************************************************/

/* VAR ZERO_INIT sections */

#if defined (FRTP_START_SEC_VAR_ZERO_INIT_8BIT)
   #undef      FRTP_START_SEC_VAR_ZERO_INIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_ZERO_INIT_8BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_ZERO_INIT_8BIT)
   #undef      FRTP_STOP_SEC_VAR_ZERO_INIT_8BIT         /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_ZERO_INIT_16BIT)
   #undef      FRTP_START_SEC_VAR_ZERO_INIT_16BIT       /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_ZERO_INIT_16BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_ZERO_INIT_16BIT)
   #undef      FRTP_STOP_SEC_VAR_ZERO_INIT_16BIT        /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_ZERO_INIT_32BIT)
   #undef      FRTP_START_SEC_VAR_ZERO_INIT_32BIT       /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_ZERO_INIT_32BIT
#endif

#if defined (FRTP_STOP_SEC_VAR_ZERO_INIT_32BIT)         /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #undef      FRTP_STOP_SEC_VAR_ZERO_INIT_32BIT
   #define STOP_SEC_VAR
#endif

#if defined (FRTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
   #undef      FRTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif

#if defined (FRTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
   #undef      FRTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED  /* PRQA S 0841 */ /* MD_MSR_19.6 */
   #define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  TP_ISO10681 END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  RTP START 
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef RTP_START_SEC_CODE
# undef RTP_START_SEC_CODE
# define START_SEC_CODE                            /* mapped to default code section */
#endif
#ifdef RTP_STOP_SEC_CODE
# undef RTP_STOP_SEC_CODE
# define STOP_SEC_CODE                             /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef RTP_START_SEC_CONST_8BIT
# undef RTP_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT                       /* mapped to default const 8bit section */
#endif
#ifdef RTP_STOP_SEC_CONST_8BIT
# undef RTP_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST                             /* default const stop section */
#endif

#ifdef RTP_START_SEC_CONST_16BIT
# undef RTP_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef RTP_STOP_SEC_CONST_16BIT
# undef RTP_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef RTP_START_SEC_CONST_32BIT
# undef RTP_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef RTP_STOP_SEC_CONST_32BIT
# undef RTP_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef RTP_START_SEC_CONST_UNSPECIFIED
# undef RTP_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef RTP_STOP_SEC_CONST_UNSPECIFIED
# undef RTP_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef RTP_START_SEC_VAR_INIT_8BIT
# undef RTP_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT                    /* mapped to default var init 8bit section */
#endif
#ifdef RTP_STOP_SEC_VAR_INIT_8BIT
# undef RTP_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR                               /* default var stop section */
#endif

#ifdef RTP_START_SEC_VAR_INIT_16BIT
# undef RTP_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef RTP_STOP_SEC_VAR_INIT_16BIT
# undef RTP_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_INIT_32BIT
# undef RTP_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef RTP_STOP_SEC_VAR_INIT_32BIT
# undef RTP_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_INIT_UNSPECIFIED
# undef RTP_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef RTP_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef RTP_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef RTP_START_SEC_VAR_NOINIT_8BIT
# undef RTP_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef RTP_STOP_SEC_VAR_NOINIT_8BIT
# undef RTP_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_NOINIT_16BIT
# undef RTP_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef RTP_STOP_SEC_VAR_NOINIT_16BIT
# undef RTP_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_NOINIT_32BIT
# undef RTP_START_SEC_VAR_NOINIT_32BIT
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef RTP_STOP_SEC_VAR_NOINIT_32BIT
# undef RTP_STOP_SEC_VAR_NOINIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef RTP_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef RTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef RTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef RTP_START_SEC_VAR_ZERO_INIT_8BIT
# undef RTP_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef RTP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef RTP_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_ZERO_INIT_16BIT
# undef RTP_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef RTP_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef RTP_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_ZERO_INIT_32BIT
# undef RTP_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef RTP_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef RTP_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef RTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef RTP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef RTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef RTP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  RTP END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  VSTDLIB START
 *********************************************************************************************************************/

#if defined(VSTDLIB_START_SEC_CODE)
# undef VSTDLIB_START_SEC_CODE                             /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_CODE                                    /* mapped to default code section */
#endif
#if defined(VSTDLIB_STOP_SEC_CODE)
# undef VSTDLIB_STOP_SEC_CODE                              /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_CODE
#endif

/**********************************************************************************************************************
 *  VSTDLIB END
 *********************************************************************************************************************/


/* PRQA L:MEMMAP_0841_TAG */

#include "MemMap_Common.h"

/**********************************************************************************************************************
 *  END OF FILE: MemMap.h
 *********************************************************************************************************************/

