###############################################################################
# File Name  : Scc_rules.mak                                                  #
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
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += Scc
Scc_FILES               = Scc\Scc.c Scc\Scc_Priv.c Scc\Scc_Exi.c Scc\Scc_ExiRx_ISO.c Scc\Scc_ExiTx_ISO.c Scc\Scc_StateM_Vector.c
Scc_FILES              += Scc\Scc_ExiRx_DIN.c Scc\Scc_ExiTx_DIN.c

# e.g.: CC_FILES_TO_BUILD       += drv\can_drv.c
ifeq ($(GET_IMPLEMENTION_FROM), LIB)
CC_FILES_TO_BUILD       += 
else
CC_FILES_TO_BUILD       += Scc\Scc.c Scc\Scc_Priv.c Scc\Scc_Exi.c Scc\Scc_ExiRx_ISO.c Scc\Scc_ExiTx_ISO.c Scc\Scc_StateM_Vector.c
CC_FILES_TO_BUILD       += Scc\Scc_ExiRx_DIN.c Scc\Scc_ExiTx_DIN.c
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
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Scc_PBcfg.c $(GENDATA_DIR)\Scc_Lcfg.c

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

