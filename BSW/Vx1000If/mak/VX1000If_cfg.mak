###############################################################################
# File Name  : VX1000If_cfg.mak                                                    #
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
# Copyright (c) 2007 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Aba          Andreas Barac             Vector Informatik GmbH               #
# Bir          Holger Birke              Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign Description                                      #
# ----------   -------  ---- -------------------------------------------------#
# 2007/06/13   1.00     Aba  Initial Version of Template (1.0)               #
# 2007/08/08   1.01     Bir  First version                                    #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 0.1                                                       #
###############################################################################

##############################################################
# VX1000IF_CONFIG_PATH: (not used for Vector)
# The Variable VX1000IF_CONFIG_PATH is optional. It allows the 
# configuration of the directory, where you can also find the 
# generated source files. Generally they should be in the 
# directory $(PROJECT_ROOT)\source\networks.
# But if the application should run on several hardware 
# targets, then different VX1000IF configurations of different
# directories will be necessary. Therefor the variable 
# VX1000IF_CONFIG_PATH should be used.
# 
# For example:
# VX1000IF_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
# VX1000IF_CONFIG_PATH = $(PROJECT_ROOT)\source\network\st10
# VX1000IF_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
#

#VX1000If_CONFIG_PATH =


##############################################################
# VX1000IF_DRIVER_MODE:  (not used for Vector)
# This Variable contains the name of the can driver module. At 
# the moment the Standard Software Core Supports the two driver
# can_drv.c (single VX1000IF) and can_drvm.c (multi VX1000IF). Other 
# values are not allowed.
# 
# For example:
# VX1000IF_DRIVER_MODE = singlechannel/multichannel
#

#VX1000If_DRIVER_MODE = singlechannel


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
