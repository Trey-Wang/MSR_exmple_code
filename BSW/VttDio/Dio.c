/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dio.h
 *      Project:  DrvDio_VttCanoe01Asr
 *       Module:  MICROSAR Dio
 *    Generator:  -
 *
 *  Description:  The DIO Driver provides services for reading and writing to/from
 *                - DIO Channels (Pins)
 *                - DIO Ports
 *                - DIO Channel Groups
 *                In the emulated MCAL, the channels, channel groups and ports are simulated by CANoe environment
 *                variables.
 *
 **********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#define DIO_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Dio.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
#if (  (DIO_SW_MAJOR_VERSION != (0x02u)) \
    || (DIO_SW_MINOR_VERSION != (0x03u)) \
    || (DIO_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (DIO_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (DIO_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Dio.c and Dio.h are inconsistent!"
#endif

#if (  (DIO_CFG_MAJOR_VERSION != (0x04u)) \
    || (DIO_CFG_MINOR_VERSION != (0x01u)) )
  #error "Version numbers of Dio.c and Dio_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
/* CurrentConfigPtr keeps the reference to the post-build configuration set, which is passed to function Dio_Init() */
P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) CurrentConfigPtr = NULL_PTR;

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  Dio_InitMemory
 **********************************************************************************************************************/
/*! \brief      This service initializes the global variables in case the startup code does not work
 *  \param[in]  void
 *  \return     void
 *  \pre        Module must not be initialized
 *  \context    Function is called during startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Reset configuration pointer. All other variables are reset in Dio_Init() */
  CurrentConfigPtr = NULL_PTR;
  
  return;
}

/**********************************************************************************************************************
 *  Dio_Init
 **********************************************************************************************************************/
/*! \brief      This service initialize the DIO component.
 *  \param[in]  ConfigPtr               Pointer to the used configuration struct
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_Init_Vtt (P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the ChannelId    */
  if (ConfigPtr == NULL_PTR)
  {
    /* The ChannelId is out of range */
    Dio_ReportDet (DIO_SID_INIT, DIO_E_PARAM_CONFIG);
    return ;
  }
#endif  /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  CurrentConfigPtr = ConfigPtr;

  return;
} /* Dio_Init() */


/**********************************************************************************************************************
 *  Dio_ReadChannel
 **********************************************************************************************************************/
/*! \brief      This service reads and returns the level of a single DIO channel.
 *  \param[in]  ChannelId               ID of DIO channel
 *  \return     Dio_LevelType           The physical level of the corresponding Pin
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel (Dio_ChannelType ChannelId)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
    /* Check the ChannelId    */
    if (DIO_MAX_CHANNEL_COUNT <= ChannelId)
    {
      /* The ChannelId is out of range */
      Dio_ReportDet (DIO_SID_READ_CHANNEL, DIO_E_PARAM_INVALID_CHANNEL_ID);
      return (0);
    }
#endif  /* (STD_ON == DIO_DEV_ERROR_DETECT) */
  
  /* ----- Implementation ----------------------------------------------- */
  return (Dio_LevelType)VttDio_ReadChannel (ChannelId);

} /* Dio_ReadChannel() */



/**********************************************************************************************************************
 *  Dio_WriteChannel
 **********************************************************************************************************************/
/*! \brief      This service sets the level of a single DIO channel to STD_HIGH or STD_LOW.
 *  \param[in]  ChannelId               ID of DIO channel
 *  \param[in]  Level                   Level to be set
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level )
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the ChannelId    */
  if (DIO_MAX_CHANNEL_COUNT <= ChannelId)
  {
    /* The ChannelId is out of range                   */
    Dio_ReportDet (DIO_SID_WRITE_CHANNEL, DIO_E_PARAM_INVALID_CHANNEL_ID);
    return;
  }
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set the variable in CANoe */
  VttDio_WriteChannel (ChannelId, Level & 0x1);

  return;

} /* Dio_WriteChannel() */


/**********************************************************************************************************************
 *  Dio_ReadPort
 **********************************************************************************************************************/
/*! \brief      This servics returns the level of all channels of a port.
 *  \param[in]  PortId                  ID of DIO Port
 *  \return     Dio_PortLevelType       Returns the level of all channels
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort (Dio_PortType PortId)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the PortId */
  if (DIO_MAX_PORT_COUNT <= PortId)
  {
    /* The PortId is out of range  */
    Dio_ReportDet (DIO_SID_READ_PORT, DIO_E_PARAM_INVALID_PORT_ID);
    return (0);
  }
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return the content of the CANoe value */
  return (Dio_PortLevelType) ( (VttDio_ReadPort (PortId)) & 0xFFFF );
} /* Dio_ReadPort() */



/**********************************************************************************************************************
 *  Dio_WritePort
 **********************************************************************************************************************/
/*! \brief      This service sets the level of a DIO port to STD_HIGH or STD_LOW.
 *  \param[in]  PortId                  ID of DIO Port
 *  \param[in]  Level                   Value to be written
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the PortId    */
  if (DIO_MAX_PORT_COUNT <= PortId)
  {
    /* The PortId is out of range  */
    Dio_ReportDet (DIO_SID_WRITE_PORT, DIO_E_PARAM_INVALID_PORT_ID);
    return;
  }
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Write the content of Level into the CANoe value  */
  VttDio_WritePort (PortId, Level);
  
  return;
} /* Dio_WritePort() */


/**********************************************************************************************************************
 *  Dio_ReadChannelGroup
 **********************************************************************************************************************/
/*! \brief      This servics returns the level of all channels of a ChannelGroup.
 *  \param[in]  ChannelGroupId          Input parameter
 *  \return     Dio_PortLevelType       Returns the LSB aligned port value.
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup
(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Index = 0;
  Dio_PortLevelType Dio_TempPortLevel = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the ChannelGroupId */
  if (NULL_PTR == ChannelGroupId)
  {
    /* The ChannelGroupId is out of range */
    Dio_ReportDet (DIO_SID_READ_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
    return Dio_TempPortLevel;
  }

  #if (DIO_MAX_CHANNELGROUP_COUNT != 0)
  /* Search for the correct Index of ChannelGroupRefs */
  while ( (DIO_MAX_CHANNELGROUP_COUNT > Index) && (Dio_ChannelGroupArray[Index] != ChannelGroupId) )
  {
    Index++;
  }

  /* Check the Index of the ChannelGroupRefs */
  if (Index >= DIO_MAX_CHANNELGROUP_COUNT)
  {
    Dio_ReportDet (DIO_SID_READ_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
    return Dio_TempPortLevel;
  }
  #else
    Dio_ReportDet (DIO_SID_READ_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
  #endif /* (DIO_MAX_CHANNELGROUP_COUNT != 0)*/
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  #if (DIO_MAX_CHANNELGROUP_COUNT != 0)
  Dio_TempPortLevel = (Dio_PortLevelType)VttDio_ReadChannelGroup(Index);

  Dio_TempPortLevel &= (Dio_ChannelGroupArray[Index]->mask);
  /* Shift the result to the LSB by offset */
  Dio_TempPortLevel >>= (Dio_ChannelGroupArray[Index]->offset);
  #endif /* (DIO_MAX_CHANNELGROUP_COUNT != 0) */

  /* Return the clean value */
  return Dio_TempPortLevel;
} /* Dio_ReadChannelGroup() */


/**********************************************************************************************************************
 *  Dio_WriteChannelGroup
 **********************************************************************************************************************/
/*! \brief      This servics sets the level of a ChannelGroup.
 *  \param[in]  ChannelGroupId          Pointer to a ChannelGroup
 *  \param[in]  Level                   Value to be written
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WriteChannelGroup
(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId,
  Dio_PortLevelType Level
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Dio_PortLevelType Dio_TempValue = 0;
  uint8 Index = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  /* Check the ChannelGroupId */
  if (NULL_PTR == ChannelGroupId)
  {
    /* The ChannelGroupId is out of range */
    Dio_ReportDet (DIO_SID_WRITE_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
    return;
  }

  #if (DIO_MAX_CHANNELGROUP_COUNT != 0)
    /* Search for the correct Index of ChannelGroupRefs */
    while ( (DIO_MAX_CHANNELGROUP_COUNT > Index) && (Dio_ChannelGroupArray[Index] != ChannelGroupId) )
    {
      Index++;
    }

    /* Check the Index of the ChannelGroupRefs */
    if (Index >= DIO_MAX_CHANNELGROUP_COUNT)
    {
      Dio_ReportDet (DIO_SID_WRITE_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
      return;
    }
  #else
    Dio_ReportDet (DIO_SID_WRITE_CHL_GROUP, DIO_E_PARAM_INVALID_GROUP);
  #endif /* (DIO_MAX_CHANNELGROUP_COUNT != 0)*/
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  #if (DIO_MAX_CHANNELGROUP_COUNT != 0)
    /* Move the "Level" to the postition of the channelgroup */
    Dio_TempValue = (Level << Dio_ChannelGroupArray[Index]->offset);

    /* Mask out all bits which are not configured */
    Dio_TempValue &= Dio_ChannelGroupArray[Index]->mask;

    VttDio_WriteChannelGroup (Index, Dio_TempValue);
  #endif /* (DIO_MAX_CHANNELGROUP_COUNT != 0) */
  
  return;
} /* Dio_WriteChannelGroup() */

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**********************************************************************************************************************
 *  Dio_FlipChannel
 **********************************************************************************************************************/
/*! \brief      This servics inverts the status of a channel
 *  \param[in]  ChannelGroupId          Pointer to a ChannelGroup
 *  \return     Dio_LevelType            The level to which the channel was set
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel (Dio_ChannelType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Dio_LevelType TempValue;
  
  /* ----- Implementation ----------------------------------------------- */
  TempValue = Dio_ReadChannel (ChannelId);
  if (STD_HIGH == TempValue)
  {
    TempValue = STD_LOW;
  }else
  {
    TempValue = STD_HIGH;
  }
  Dio_WriteChannel (ChannelId,TempValue);
  return TempValue;
}
#endif /* DIO_FLIP_CHANNEL_API */


#if (DIO_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dio_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[out] versioninfo           Pointer to where to store the version information of this module.
 *  \return     void
 *  \context    Function is called from task level
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == DIO_DEV_ERROR_DETECT)
  if (NULL_PTR == versioninfo)
  {
    /* versioninfo is not defined */
    Dio_ReportDet (DIO_SID_GET_VERSION_INFO, DIO_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* versioninfo is defined */
  }
#endif /* (STD_ON == DIO_DEV_ERROR_DETECT) */
  
  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID = (DIO_VENDOR_ID);
  versioninfo->moduleID = (DIO_MODULE_ID);
  versioninfo->sw_major_version = (DIO_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (DIO_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (DIO_SW_PATCH_VERSION);
  
  return;
} /* Dio_GetVersionInfo() */
#endif  /* (DIO_GET_VERSION_INFO_API == STD_ON) */


/**********************************************************************************************************************
 *  END OF FILE: Dio.h
 **********************************************************************************************************************/
