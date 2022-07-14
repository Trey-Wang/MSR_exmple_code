
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *           File:  Lin_Irq.c
 *      Component:  AUTOSAR LIN Driver
 *         Module:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *      Generator:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *
 *  Target system:  Canoe
 *       Compiler:  Ansi

 *    Derivatives:  VTT/EMU

 *  
 *    Description:  Implementation of Interrupt functions for AUTOSAR LIN Driver
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2013-08-12  vislpr  -             Port Canoeemu to VIP
 *  05.01.00  2014-07-17  vislpr  -             Atomic access to VipCntrl, bugfixing
 *  06.00.00  2014-09-29  vislpr  -             Add Canoeemu to VIP, ComStackLib, R11, PB-S
 *  06.00.01  2014-11-20  vislpr  -             HL Part was updated
 *  07.00.00  2015-01-29  vislpr  ESCAN00080255 FEAT-254: Globale Umbennenung von VIP nach VTT
 *                        vislpr  ESCAN00081345 FEAT-427: SafeBSW Step I
 *  07.01.00  2015-06-15  vislpr  ESCAN00083109 VTT Lin shall derive its interrupt offset from the values configured in the OS
 *  07.02.00  2015-06-15  vislpr  -             Remove VTT dependencies 
 *********************************************************************************************************************/

 /* no ISR */

