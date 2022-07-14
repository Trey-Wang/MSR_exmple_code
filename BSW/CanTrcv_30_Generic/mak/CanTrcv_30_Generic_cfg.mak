###############################################################################
# File Name  : CanTrcv_cfg.mak                                                #
# Description: Autosar makefile Template                                      #
#              This makefile is a template to implement the common            #
#              features of each project.                                      #
#              It is included by the Global.Makefile.target.make.$(Version)   #
#              and is supported from version 3.24 .                           #
#                                                                             #
# This Template is based on AUTOSAR_BSW_MakefileInterface.doc version 0.4     #
#                                                                             #
#-----------------------------------------------------------------------------#
#               C O P Y R I G H T                                             #
#-----------------------------------------------------------------------------#
# Copyright (c) 2017 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Aba          Andreas Barac             Vector Informatik GmbH               #
# Ces          Senol Cendere             Vector Informatik GmbH               #
# Ou           Mihai Olariu              Vector Informatik GmbH               #
# Tvi          Timo Vanoni               Vector Informatik GmbH               #
# Seu          Eugen Stripling           Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign Description                                      #
# ----------   -------  ---- -------------------------------------------------#
# 2007/06/13   1.00     Aba  Initial Version of Template (1.0)                #
# 2008/06/11   1.01     Ces  Adapted to AUTOSAR CanTrcv                       #
# 2008/08/21   1.02     Ou   Correct the case sensitivity for "Dio" and "dio" #
# 2008/12/12   1.03     Ou   Rename the file name                             #
# 2009/04/17   1.04     Tvi  Changed Paths / Renamed files                    #
# 2009/11/12   1.05     Tvi  Renamed files                                    #
# 2010/02/05   1.06     Tvi  Renamed make files                               #
# 2017/03/14   1.08     Seu  ESCAN00094363                                    #
# 2017/03/15   1.09     Seu  Renamed make files                               #
# 2017/03/21   1.09.01  Seu  ESCAN00094443                                    #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.9.1                                                     #
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
