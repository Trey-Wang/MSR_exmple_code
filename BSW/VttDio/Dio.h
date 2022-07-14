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
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Peter Lang                    virlg         Vector Informatik GmbH
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-09-20  virlg   ESCAN00068486 Initial development
 *  01.00.01  2014-04-04  virlg                 Modification of the channel type to allow 32 bit values
 *  01.00.02  2014-11-21  virchl  ESCAN00079355 Version number modified to 2.00.01 generator version
 *  02.00.00  2015-01-19  virchl   FEAT-254     Global renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Dio_Init (if VTTOnly-SIP)
 *                                              Dio_Init renamed to Dio_Init_Vtt and a define in Dio_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version 
 *  02.03.00  2017-01-12  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 **********************************************************************************************************************/

#if !defined DIO_H
#define DIO_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVDIO_VTTCANOE01ASR_VERSION            (0x0203u)
#define DRVDIO_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define DIO_VENDOR_ID                           (30u)
#define DIO_MODULE_ID                           (120u)
#define DIO_INSTANCE_ID                         (0u)

/* AUTOSAR Software specification version information */
#define DIO_AR_RELEASE_MAJOR_VERSION            (4u)
#define DIO_AR_RELEASE_MINOR_VERSION            (0u)
#define DIO_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define DIO_SW_MAJOR_VERSION                    (DRVDIO_VTTCANOE01ASR_VERSION >> 8)
#define DIO_SW_MINOR_VERSION                    (DRVDIO_VTTCANOE01ASR_VERSION & 0x00FF)
#define DIO_SW_PATCH_VERSION                    (DRVDIO_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define DIO_SID_INIT                            (0x10u)
#define DIO_SID_READ_CHANNEL                    (0x00u)
#define DIO_SID_WRITE_CHANNEL                   (0x01u)
#define DIO_SID_READ_PORT                       (0x02u)
#define DIO_SID_WRITE_PORT                      (0x03u)
#define DIO_SID_READ_CHL_GROUP                  (0x04u)
#define DIO_SID_WRITE_CHL_GROUP                 (0x05u)
#define DIO_SID_GET_VERSION_INFO                (0x12u)
#define DIO_SID_FLIP_CHANNEL                    (0x11u)

/* ----- Error codes ----- */
#define DIO_E_PARAM_INVALID_CHANNEL_ID          (0x0Au)   /* Invalid channel name requested */
#define DIO_E_PARAM_CONFIG                      (0x10u)   /* Invalid channel name requested */
#define DIO_E_PARAM_INVALID_PORT_ID             (0x14u)   /* Invalid port name requested    */
#define DIO_E_PARAM_INVALID_GROUP               (0x1Fu)   /* Invalid ChannelGroup id passed */
#define DIO_E_PARAM_VINFO                       (0x20u)   /* NULL_PTR passed to Dio_GetVersioninfo() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Numeric ID of a DIO channel */
typedef uint32 Dio_ChannelType;

/* Address offset of a DIO port */
typedef uint32 Dio_PortType;

/* Value of the largest DIO port on this device */
typedef uint32 Dio_PortLevelType;

/* Dio_LevelType: These are the possible levels a DIO channel can have (input
 * or output). The defines STD_HIGH and STD_LOW are defined in Std_Types.h
 * and the type itself is an uint8 */
typedef uint8  Dio_LevelType;

/* Type for the definition of a channel group, which consists of
 * several adjoining channels within a port */
typedef struct
{
  Dio_PortType       port;        /* This is the port address offset on which the channel group works */   
  uint8              offset;      /* This is the position of the channel group on the port, counted from the LSB */    
  Dio_PortLevelType  mask;        /* This is the mask which defines the positions of the channel group */    
} Dio_ChannelGroupType;

/* Currently only one runtime is supported, thus the configuration struct keeps the same  */
typedef uint8 Dio_ConfigType;


/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Dio_Cfg.h"
#include "VttCntrl_Dio.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Service functions */
FUNC(void, DIO_CODE) Dio_InitMemory (void);

FUNC(void, DIO_CODE) Dio_Init_Vtt (P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST)ConfigPtr);

FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel (Dio_ChannelType ChannelId);

FUNC(void, DIO_CODE) Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level);

FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort (Dio_PortType PortId);

FUNC(void, DIO_CODE) Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level);

FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId
);

FUNC(void, DIO_CODE) Dio_WriteChannelGroup
(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId,
  Dio_PortLevelType           Level
);

#if (DIO_VERSION_INFO_API == STD_ON)
  FUNC(void, DIO_CODE) Dio_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) versioninfo);
#endif

#if (DIO_FLIP_CHANNEL_API == STD_ON)
  FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel (Dio_ChannelType ChannelId);
#endif

#endif  /* DIO_H */

/**********************************************************************************************************************
 *  END OF FILE: Dio.h
 **********************************************************************************************************************/
