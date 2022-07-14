###############################################################################
# File Name  : EthIf_rules.mak                                               #
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
# Copyright (c) 2016 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# visalr       Alex Lunkenheimer         Vector Informatik GmbH               #
# vishaw       Harald Walter             Vector Informatik GmbH               #
# vismha       Mark Harsch               Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version Sign    Description                                    #
# ----------   ------- ------  -----------------------------------------------#
# 2008/11/19   1.00    visalr  Created                                        #
# 2009/10/08   1.00.03 visalr  bugfix library support                         #
# 2013/08/05   2.00.00 vishaw  MSR4-R8 support                                #
# 2014/03/04   2.01.00 vishaw  PTP support                                    #
# 2014/07/08   2.02.00 vishaw  ESCAN00077151: AR4-500:                        #
#                              AUTOSAR CONC_600_SwitchConfiguration           #
# 2015/02/16   2.03.00 vismha  Removed PB config related parts                #
# 2015/06/22   2.04.00 vismha  Extended Traffic Handling source files added   #
# 2015/11/25   2.05.00 vismha  Introduced files containing switch hardware    #
#                              abstraction implementation                     #
# 2016/06/15   3.00.00 vismha  Introduced files for Zero Copy extensions      #
#                              Renamed EthIf_Ptp.h/.c files to EthIf_Time.h/.c#
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 3.00.00                                                   #
###############################################################################


###############################################################
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += EthIf
EthIf_FILES              = EthIf\EthIf.c EthIf\EthIf_Switch.c EthIf\EthIf_Time.c EthIf\EthIf_ExtndTrafficHndl.c EthIf\EthIf_ZeroCopy.c

# e.g.: CC_FILES_TO_BUILD       += drv\can_drv.c
ifeq ($(GET_IMPLEMENTION_FROM), LIB)
CC_FILES_TO_BUILD       += 
else
CC_FILES_TO_BUILD       += EthIf\EthIf.c EthIf\EthIf_Switch.c EthIf\EthIf_Time.c EthIf\EthIf_ExtndTrafficHndl.c EthIf\EthIf_ZeroCopy.c
endif
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly 
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\EthIf_Lcfg.c $(GENDATA_DIR)\EthIf_EthCtrlCfg.c $(GENDATA_DIR)\EthIf_EthTrcvCfg.c $(GENDATA_DIR)\EthIf_EthSwtCfg.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     += 

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automaticly
# so in this clean-rule it is only necessary to define additional files which
# were not delete automaticly.
# e.g.: $(<PATH>)\can_test.c
#-------------------------------------------------------------------------------------------------
MAKE_CLEAN_RULES        +=
#MAKE_GENERATE_RULES     +=
#MAKE_COMPILER_RULES     +=
#MAKE_DEBUG_RULES        +=
#MAKE_CONFIG_RULES       +=
#MAKE_ADD_RULES          +=


###############################################################
# REQUIRED   (defined in BaseMake (global.Makefile.target.make...))
#
# SSC_ROOT		(required)
# PROJECT_ROOT	(required)
#
# LIB_OUTPUT_PATH	(optional)
# OBJ_OUTPUT_PATH	(optional)
#
# OBJ_FILE_SUFFIX	
# LIB_FILE_SUFFIX
#
###############################################################


###############################################################
# PROVIDE   this Section can be used to define own additional rules
#
# In vendorx_can_cfg.mak:
# Please configure the project file:
#CAN_CONFIG_FILE = $(PROJECT_ROOT)\source\network\can\my_can_config.cfg

#In vendorx_can_config :
#generate_can_config:
#$(SSC_ROOT)\core\com\can\tools\canconfiggen.exe -o $(CAN_CONFIG_FILE)


###############################################################
# SPECIFIC
#
# There are no rules defined for the Specific part of the 
# Rules-Makefile. Each author is free to create temporary 
# variables or to use other resources of GNU-MAKE
#
###############################################################

