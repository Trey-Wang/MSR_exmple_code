/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file   CddMirror.h
 *  \brief  MIRROR header file
 *
 *  \details  Implementation of Mirror component.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Matthias Mueller              vismmu        Vector Informatik GmbH
 *  Simon Gutjahr                 visgsi        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-09-30  vismmu  -             Creation
 *  01.00.01  2017-01-10  vismmu  -             No changes here
 *  01.01.00  2017-02-21  visgsi  -             Mirror_CanTxFrame changed to Mirror_ReportCanFrame
*********************************************************************************************************************/

/* Configuration, interrupt handling and call-back headers use different names for the multiple inclusion protection
 * define, shown in this example. The appropriate names for these files can be taken from this list:
 *
 * Mirror_Cfg.h:     MIRROR_CFG_H
 * Mirror_Irq.h:     MIRROR_IRQ_H
 * Mirror_Cbk.h:     MIRROR_CBK_H
 */
#if !defined (CDDMIRROR_H)
# define CDDMIRROR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Mirror.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define Appl_GenericConfirmation   Mirror_ReportCanFrame
# define Appl_GenericPrecopy        Mirror_CanRxFrame

# define CddMirror_MirrorModeType   Mirror_ModeType
# define CDDMIRROR_INACTIVE         MIRROR_INACTIVE

# endif
