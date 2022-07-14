###############################################################################
# File Name  : Exi_rules.mak                                                  #
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
# Copyright (c) 2014 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# visalr       Alex Lunkenheimer         Vector Informatik GmbH               #
# visal        Thorsten Albers           Vector Informatik GmbH               #
# visdd        Daniel Dausend            Vector Informatik GmbH               #
# vissop       Patrick Sommer            Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign   Description                                    #
# ----------   -------  ------ -----------------------------------------------#
# 2010-11-03   1.00.00  visalr Created                                        #
# 2010-11-17   1.00.01  visal  minor name fixes                               #
# 2012-02-01   2.00.00  visdd  Created (EXI development started)              #
# 2013-10-01   2.00.01  visdd  ESCAN00069263 Remove PBcfg.c                   #
# 2013-11-14   3.00.00  visdd  ESCAN00071678 Support multiple schema versions #
# 2014-03-06   3.01.00  visdd  ESCAN00074027 Customer specific extensions     #
# 2014-06-13   4.00.00  vissop ESCAN00076252 Customer specific extensions     #
# 2014-07-18   4.01.00  vissop ESCAN00077212 Adapt package attributes         #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 0.1                                                       #
###############################################################################

###############################################################
# REGISTRY
#

#ifndef EXI_SCHEMA_SAP
# get schema variables from the variant specific config file
#include $(ROOT)\$(GLOBAL_COMP_DIR)\Exi\mak\Exi_cfg.mak
#endif

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += Exi
# General files
Exi_FILES               = Exi\Exi.c Exi\Exi_BSDecoder.c Exi\Exi_BSEncoder.c Exi\Exi_SchemaDecoder.c Exi\Exi_SchemaEncoder.c
# Use case specific files
Exi_FILES               += Exi\Exi_SAP_SchemaDecoder.c Exi\Exi_SAP_SchemaEncoder.c
Exi_FILES               += Exi\Exi_DIN_SchemaDecoder.c Exi\Exi_DIN_SchemaEncoder.c
Exi_FILES               += Exi\Exi_ISO_SchemaDecoder.c Exi\Exi_ISO_SchemaEncoder.c
Exi_FILES               += Exi\Exi_XMLSIG_SchemaDecoder.c Exi\Exi_XMLSIG_SchemaEncoder.c

# e.g.: CC_FILES_TO_BUILD       += drv\can_drv.c
ifeq ($(GET_IMPLEMENTION_FROM), LIB)
CC_FILES_TO_BUILD       += 
else
CC_FILES_TO_BUILD       += Exi\Exi.c Exi\Exi_BSDecoder.c Exi\Exi_BSEncoder.c Exi\Exi_SchemaDecoder.c Exi\Exi_SchemaEncoder.c
CC_FILES_TO_BUILD       += Exi\Exi_SAP_SchemaDecoder.c Exi\Exi_SAP_SchemaEncoder.c
CC_FILES_TO_BUILD       += Exi\Exi_DIN_SchemaDecoder.c Exi\Exi_DIN_SchemaEncoder.c
CC_FILES_TO_BUILD       += Exi\Exi_ISO_SchemaDecoder.c Exi\Exi_ISO_SchemaEncoder.c
CC_FILES_TO_BUILD       += Exi\Exi_XMLSIG_SchemaDecoder.c Exi\Exi_XMLSIG_SchemaEncoder.c
endif
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automatically 
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Exi_Lcfg.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     += 

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automatically
# so in this clean-rule it is only necessary to define additional files which
# were not delete automatically.
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

