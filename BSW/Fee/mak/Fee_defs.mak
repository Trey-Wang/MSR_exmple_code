###############################################################################
# File Name  : Fee_defs.mak                                                   #
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
# Copyright (c) 2009 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Aba          Andreas Barac             Vector Informatik GmbH               #
# Pp           Peter Paulus              Vector Informatik GmbH               #
# Jbn          Juergen Braun             Vector Informatik GmbH               #
# Ck           Christian Kaiser          Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign Description                                      #
# ----------   -------  ---- -------------------------------------------------#
# 2007-06-13   1.00     Aba  Initial Version of Template (1.0)                #
# 2007-10-02   1.01     Pp   Created ASR2.1 make files                        #
# 2008-05-05   1.02     fbg  Changes for ASR library support                  #
# 2008-12-15   1.04     Jbn  supporting ASR3.0 Fee                            #
# 2009-07-21   1.05     Jbn  ESCAN00036203: improve the CM-possibility of     #
#                            the Fee30Inst2                                   #
# 2012-06-22   2.00     ck   ESCAN00057779 results in additional source files #
# 2015-12-01   3.00     ck   keep major version consistent between make files #
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
# SSC_ROOT
###############################################################


###############################################################
# SPECIFIC
# Specific variables which were defined here are only for local use in this Makefile
# if for example additional includes are necessary, these vars can be defined and used in the REGISTRY-Part.
# e.g.: $(CAN_CORE_PATH) = $(SSC_ROOT)\Can  could be used for CPP_INCLUDE_PATH    +=  $(CAN_CORE_PATH)\CPP_FILES\...


FEE_CORE_PATH       =   $(SSC_ROOT)\Fee


# e.g.: CAN_OUTPUT_PATH = $(GENDATA_DIR)


FEE_OUTPUT_PATH     =  $(GENDATA_DIR)




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
# $(SSC_PLUGINS) is a list of the components. Here can is added (not necessary for Vector, whole component-list also exists in Global.makefile.target.make...)
# e.g.: SSC_PLUGINS        += can
SSC_PLUGINS        +=
#e.g.: can_DEPENDENT_PLUGINS =
FEE_DEPENDENT_PLUGINS =

#-----------------------------------------------------------------------------------------
# MakeSupport usually includes all header-files which were in the same
# directory as the source-files automatically, but to ensure that the
# Asr-Makefiles will also work with other Basic-Make-Packages,
# it is necessary to define all include directories for this Module
# e.g.: CC_INCLUDE_PATH     += $(CAN_CORE_PATH)
#------------------------------------------------------------------------------------------
CC_INCLUDE_PATH     +=  $(FEE_CORE_PATH)  $(FEE_OUTPUT_PATH)
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
