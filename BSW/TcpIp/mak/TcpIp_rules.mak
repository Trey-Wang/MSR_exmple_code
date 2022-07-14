###############################################################################
# File Name  : TcpIp_rules.mak                                                #
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
# Copyright (c) 2015 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# visal        Thorsten Albers           Vector Informatik GmbH               #
# visfdn       Frederik Dornemann        Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version Sign    Description                                    #
# ----------   ------- ------  -----------------------------------------------#
# 2014/07/16   1.00.00 visfdn  Created                                        #
#-----------------------------------------------------------------------------#
# TemplateVersion = 1.0                                                       #
# MAKEFILE        = 0.1                                                       #
###############################################################################


###############################################################
# REGISTRY
#

include $(ROOT)\$(GLOBAL_COMP_DIR)\TcpIp\mak\TcpIp_cfg.mak

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += TcpIp
TcpIp_FILES              = TcpIp\TcpIp.c TcpIp\TcpIp_Priv.c TcpIp\TcpIp_Tcp.c TcpIp\TcpIp_Udp.c

ifeq ($(TCPIP_IPV4), 1)
TcpIp_FILES             += TcpIp\TcpIp_IpV4.c TcpIp\TcpIp_IcmpV4.c TcpIp\TcpIp_DhcpV4.c TcpIp\TcpIp_Arp.c TcpIp\TcpIp_IpV4_Priv.c
endif

ifeq ($(TCPIP_IPV6), 1)
TcpIp_FILES             += TcpIp\TcpIp_IpV6.c TcpIp\TcpIp_IcmpV6.c TcpIp\TcpIp_DhcpV6.c TcpIp\TcpIp_Ndp.c TcpIp\TcpIp_IpV6_Priv.c TcpIp\TcpIp_Mld.c
endif

ifeq ($(TCPIP_DHCPV4SERVER), 1)
TcpIp_FILES             += TcpIp\TcpIp_DhcpV4Server.c
endif

ifeq ($(GET_IMPLEMENTION_FROM), LIB)
CC_FILES_TO_BUILD       += 
else
CC_FILES_TO_BUILD       += $(TcpIp_FILES)
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
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\TcpIp_Lcfg.c

ifeq ($(TCPIP_IPV4), 1)
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\TcpIp_IpV4_Lcfg.c
endif

ifeq ($(TCPIP_IPV6), 1)
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\TcpIp_IpV6_Lcfg.c
endif

ifeq ($(TCPIP_DHCPV4SERVER), 1)
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\TcpIp_DhcpV4Server_Lcfg.c
endif

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

