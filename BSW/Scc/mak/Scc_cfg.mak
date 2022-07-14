###############################################################################
# File Name  : Scc_cfg.mak                                                    #
# Description: Autosar makefile Template                                      #
#              This makefile is a template to implement the common            #
#              features of each project.                                      #
#              It is included by the Global.Makefile.target.make.$(Version)   #
#              and is supported from version 3.24 .                           #
#                                                                             #
# This Template is based on AUTOSAR_BSW_MakefileInterface.doc version 0.4     #
#                                                                             #
#-----------------------------------------------------------------------------#
#                            C O P Y R I G H T                                #
# --------------------------------------------------------------------------- #
# Copyright (c) 2015 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#                      A U T H O R   I D E N T I T Y                          #
# --------------------------------------------------------------------------- #
# Initial  Name               Company                                         #
# -------  -----------------  ----------------------------------------------- #
# visalr   Alex Lunkenheimer  Vector Informatik GmbH                          #
# visefa   Fabian Eisele      Vector Informatik GmbH                          #
#-----------------------------------------------------------------------------#
#                     R E V I S I O N   H I S T O R Y                         #
# --------------------------------------------------------------------------- #
# Date        Version  Initial  Description                                   #
# ----------  -------  -------  --------------------------------------------- #
# 2010/01/01  1.00.00  visalr   Created                                       #
# 2015/02/27  1.xx.xx  visefa   removed revision history for version 1.xx.xx  #
# 2015/02/27  2.xx.xx  visefa   removed revision history for version 2.xx.xx  #
# 2012/12/17  4.00.00  visefa   Update to DIS of ISO15118                     #
# 2013/07/05  4.01.00  visefa   Scc_StateM_Vector added                       #
# 2014/01/30  5.00.00  visefa   Added support for DIN70121                    #
# 2014/02/14  5.01.00  visefa   Scc_Rte.c removed                             #
# 2014/03/19  5.01.01  visefa   removed Scc_ExiZx_DIN.c for some OEM          #
# 2014/03/25  5.02.00  visefa   Added support for another OEM                 #
# 2014/06/10  5.02.01  visefa   Added DIN files for some OEM                  #
# 2014/06/25  5.03.00  visefa   Added files for customer specific schema      #
# 2014/07/28  5.03.01  visefa   Fixed filtering                               #
# 2015/02/27  6.00.00  visefa   Added support for another OEM                 #
# 2015/07/13  6.00.01  visefa   Fixed filter rules for one OEM                #
# 2016/02/03  7.00.00  visefa   Added new OEM, changed file names             #
# 2016/06/27  8.00.00  visefa   Updated file names for some OEM               #
# 2016/07/07  8.01.00  visefa   Added new OEM specific files                  #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 0.1                                                       #
###############################################################################

##############################################################
# CAN_CONFIG_PATH: (not used for Vector)
# The Variable CAN_CONFIG_PATH is optional. It allows the 
# configuration of the directory, where you can also find the 
# generated source files. Generally they should be in the 
# directory $(PROJECT_ROOT)\source\networks.
# But if the application should run on several hardware 
# targets, then different CAN configurations of different
# directories will be necessary. Therefor the variable 
# CAN_CONFIG_PATH should be used.
# 
# For example:
# CAN_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
# CAN_CONFIG_PATH = $(PROJECT_ROOT)\source\network\st10
# CAN_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
#

##############################################################
# CAN_DRIVER_MODE:  (not used for Vector)
# This Variable contains the name of the can driver module. At 
# the moment the Standard Software Core Supports the two driver
# can_drv.c (single CAN) and can_drvm.c (multi CAN). Other 
# values are not allowed.
# 
# For example:
# CAN_DRIVER_MODE = singlechannel/multichannel
#

###############################################################
# REQUIRED
#
# No Other resources are required to write a Configuration 
# Makefile.
#
###############################################################


###############################################################
# PROVIDE
#
# A Configuration Makefile does not provide resources for other
# basic software bundles or the base_make package
#
###############################################################


###############################################################
# REGISTRY
# 
# Like the Provide-Section this block is empty.
#
###############################################################


###############################################################
# SPECIFIC
#
# All variables in the _cfg makefile are "Specific" variables,
# because they should only be used in the *_check.mak,
# *_defs.mak and *_rules.mak Makefiles of the same basic 
# software bundle.
###############################################################
