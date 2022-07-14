###############################################################################
# File Name  : vIpcMemCp_cfg.mak                                              #
# Description: Autosar makefile Template                                      #
#              This makefile is a template to implement the common            #
#              features of each project.                                      #
#              It is included by the Global.Makefile.target.make.$(Version)   #
#              and is supported from version 3.24.                            #
#                                                                             #
# This Template is based on AUTOSAR_BSW_MakefileInterface.doc version 0.4     #
#                                                                             #
#-----------------------------------------------------------------------------#
#               C O P Y R I G H T                                             #
#-----------------------------------------------------------------------------#
# Copyright (c) 2017 Vector Informatik GmbH.  All rights reserved.            #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Bse          Benjamin Seifert          Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign Description                                      #
# ----------   -------  ---- -------------------------------------------------#
# 2017-06-07   1.00     Bse  Initial Version                                  #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 1.0                                                       #
###############################################################################


##############################################################
# VIPCMEMCP_CONFIG_PATH: (not used for Vector)
# The Variable VIPCMEMCP_CONFIG_PATH is optional. It allows the
# configuration of the directory, where you can also find the
# generated source files. Generally they should be in the
# directory $(PROJECT_ROOT)\source\networks.
# But if the application should run on several hardware
# targets, then different J1939Rm configurations of different
# directories will be necessary. Therefor the variable
# VIPCMEMCP_CONFIG_PATH should be used.
#
# For example:
# VIPCMEMCP_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
# VIPCMEMCP_CONFIG_PATH = $(PROJECT_ROOT)\source\network\st10
# VIPCMEMCP_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
#
##############################################################


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
#
###############################################################
