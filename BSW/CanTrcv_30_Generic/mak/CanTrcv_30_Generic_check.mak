###############################################################################
# File Name  : CanTrcv_check.mak                                              #
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

###############################################################
# REGISTRY
#

#e.g.: PREPARE_CONFIGURATION_INTERFACE     += CAN_MODULES 
PREPARE_CONFIGURATION_INTERFACE     +=

CHECK_VARS_WHICH_ARE_REQUIRED       += 

CHECK_VARS_WHICH_ARE_OPTIONAL       +=

CHECK_VARS_WHICH_ARE_OBSOLETE       +=

CHECK_VARS_WITH_ONE_CC_FILE         +=
CHECK_VARS_WITH_MORE_CC_FILES       +=

CHECK_VARS_WITH_ONE_CPP_FILE        +=
CHECK_VARS_WITH_MORE_CPP_FILES      +=

CHECK_VARS_WITH_ONE_ASM_FILE        +=
CHECK_VARS_WITH_MORE_ASM_FILES      +=

CHECK_VARS_WITH_ONE_LIB_FILE        +=
CHECK_VARS_WITH_MORE_LIB_FILES      +=

CHECK_VARS_WITH_ONE_OBJ_FILE        +=
CHECK_VARS_WITH_MORE_OBJ_FILES      +=

#e.g: CHECK_VARS_WITH_ONE_DIRECTORY +=     $(DIRECTORIES_TO_CREATE)
#       $(DIRECTORIES_TO_CREATE) = C:\demo\drv          (this var is defined in _rules.mak)
CHECK_VARS_WITH_ONE_DIRECTORY       +=
CHECK_VARS_WITH_MORE_DIRECTORIES    +=

CHECK_VARS_WITH_ONE_FILE            +=
CHECK_VARS_WITH_MORE_FILES          +=

CHECK_VARS_WITH_MAX_LENGTH_ONE      +=

###############################################################
# SPECIFIC
#
#ifneq ($(CAN_DRIVER_MODE),singlechannel)
#ifneq ($(CAN_DRIVER_MODE),multichannel)
#$(error The value of the variable CAN_DRIVER_MODE is not valid. \
#	Please use singlechannel or multichannel)
#endif
#endif


###############################################################
# REQUIRED
#
# No other resources are required for a Configuration Check 
# Makefile
#
###############################################################


###############################################################
# PROVIDE
#
# A configuration Makefile does not provide resources for other
# basic software bundles or the base-make package.
#
###############################################################

