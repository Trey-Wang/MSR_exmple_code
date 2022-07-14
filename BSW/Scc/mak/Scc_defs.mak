###############################################################################
# File Name  : Scc_defs.mak                                                   #
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

###############################################################
# INTERNAL REQUIRED CONFIGURATION (in can_cfg.mak)
#
# CAN_MODULES 	    (required)  (not necessary for Vector)
# CAN_DRIVER_MODE   (required)  (not necessary for Vector)
# CAN_PROJECT_PATH  (optional) 
# CAN_BUILD_LIBRARY (required)


###############################################################
# REQUIRED (defined in base_make)
# 
# PROJECT_ROOT
# SCC_ROOT
###############################################################


###############################################################
# SPECIFIC
# Specific variables which were defined here are only for local use in this Makefile
# if for example additional includes are necessary, these vars can be defined and used in the REGISTRY-Part.
# e.g.: $(CAN_CORE_PATH) = $(SSC_ROOT)\Can  could be used for CPP_INCLUDE_PATH    +=  $(CAN_CORE_PATH)\CPP_FILES\...
SCC_CORE_PATH        = Scc\
SCC_OUTPUT_PATH      = $(GENDATA_DIR)

#CAN_TOOL_PATH       = $(PROJECT_ROOT)\core\com\can\gentool

# it is also possible to generate own configurations with some checks like the following example
#  ifneq ( $(CAN_CONFIG_PATH),)
#      CAN_PROJECT_PATH    = $(CAN_CONFIG_PATH)
#  else
#     CAN_PROJECT_PATH    = $(PROJECT_ROOT)\source\network\can
#  endif 


###############################################################
# REGISTRY
# 
# $(SCC_PLUGINS) is a list of the components. Here can is added (not necessary for Vector, whole component-list also exists in Global.makefile.target.make...)
# e.g.: SCC_PLUGINS        += can
SCC_PLUGINS        += 
#e.g.: can_DEPENDENT_PLUGINS = 
SCC_DEPENDENT_PLUGINS = 

#-----------------------------------------------------------------------------------------
# MakeSupport usually includes all header-files which were in the same
# directory as the source-files automatically, but to ensure that the
# Asr-Makefiles will also work with other Basic-Make-Packages,
# it is necessary to define all include directories for this Module
# e.g.: CC_INCLUDE_PATH     += $(CAN_CORE_PATH)\
#------------------------------------------------------------------------------------------
CC_INCLUDE_PATH     +=
CPP_INCLUDE_PATH    +=
ASM_INCLUDE_PATH    +=

#PREPROCESSOR_DEFINES+= drivermode

#drivermode_KEY       = CAN_MODE
#drivermode_VALUE    = $(CAN_DRIVER_MODE)


###############################################################
# PROVIDE
#
# A Public Makefile does not provide resources for other basic
# software bundles or the base_make package.
###############################################################
