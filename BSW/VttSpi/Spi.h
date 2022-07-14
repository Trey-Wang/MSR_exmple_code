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
 *         File:  Spi.h
 *      Project:  DrvSpi_VttCanoe01Asr
 *       Module:  MICROSAR Spi
 *
 *  Description:  This module manages the data transfer over the SPI bus. The module incorporates both the driver and
 *                handler.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-01-09  virchl     -          Ported Spi driver from DrvSpi_CanoeemuCanoe01Asr Ver. 02.00.01 to Vip
 *  02.00.00  2015-01-22  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.00.01  2015-04-28  virchl  ESCAN00082464 No modifications here
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Spi_Init (if VTTOnly-SIP)
 *                                              Spi_Init renamed to Spi_Init_Vtt and a define in Spi_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *                                ESCAN00084982 Definition of variables embraced with macro SPI_DEV_ERROR_DETECT if 
 *                                              variables are only used in Development Error Checks
 *  02.02.00  2015-10-05  virchl  ESCAN00085650 Include of SchM_Spi.h added
 *  02.03.00  2016-02-25  virchl  ESCAN00088544 Update of SwVersions due to increase of generator version
 *                                              Files for Interrupt handling added (SpiIsrTx/Rx_0...49)
 *  02.04.00  2016-07-28  virchl  ESCAN00091748 HwUnitStatus supported
 *  02.04.00  2016-09-05  virchl  ESCAN00091739 Usage of CAT2 ISRs 
 *  02.05.00  2017-01-19  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update
 *********************************************************************************************************************/

#if !defined SPI_H
#define SPI_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define DRVSPI_VTTCANOE01ASR_VERSION            (0x0205u)
#define DRVSPI_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define SPI_VENDOR_ID                           (30u)
#define SPI_MODULE_ID                           (83u)
#define SPI_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define SPI_AR_RELEASE_MAJOR_VERSION            (4u)
#define SPI_AR_RELEASE_MINOR_VERSION            (0u)
#define SPI_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define SPI_SW_MAJOR_VERSION                    (DRVSPI_VTTCANOE01ASR_VERSION >> 8u)
#define SPI_SW_MINOR_VERSION                    (DRVSPI_VTTCANOE01ASR_VERSION & 0x00FFu)
#define SPI_SW_PATCH_VERSION                    (DRVSPI_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define SPI_SID_INIT                            (0x00u)
#define SPI_SID_DEINIT                          (0x01u)
#define SPI_SID_WRITEIB                         (0x02u)
#define SPI_SID_ASYNC_TRANSMIT                  (0x03u)
#define SPI_SID_READIB                          (0x04u)
#define SPI_SID_SETUP_EB                        (0x05u)
#define SPI_SID_GET_STATUS                      (0x06u)
#define SPI_SID_GET_JOB_RES                     (0x07u)
#define SPI_SID_GET_SEQ_RES                     (0x08u)
#define SPI_SID_GET_VERSION_INFO                (0x09u)
#define SPI_SID_SYNC_TRANSMIT                   (0x0Au)
#define SPI_SID_GET_HW_STAT                     (0x0Bu)
#define SPI_SID_CANCEL                          (0x0Cu)
#define SPI_SID_SET_ASYNC_MODE                  (0x0Du)
#define SPI_SID_MAINFUNCTION_HANDLING           (0x10u)

/* ----- Error codes ----- */
#define SPI_E_PARAM_CHANNEL                     (0x0Au)   /* Invalid Channel ID                      */
#define SPI_E_PARAM_JOB                         (0x0Bu)   /* Invalid Job ID                          */
#define SPI_E_PARAM_SEQ                         (0x0Cu)   /* Invalid Sequence ID                     */
#define SPI_E_PARAM_LENGTH                      (0x0Du)   /* Invalid DataLength                      */
#define SPI_E_PARAM_UNIT                        (0x0Eu)   /* Invalid HW Unit ID                      */
#define SPI_E_PARAM_CONFIG                      (0x0Fu)   /* Invalid ConfigPtr - not in Specs        */
#define SPI_E_PARAM_POINTER                     (0x10u)   /* Invalid Pointer (NULL_PTR)              */
#define SPI_E_PARAM_MODE                        (0x11u)   /* Invalid Processing mode - not in Specs  */
#define SPI_E_PARAM_VINFO                       (0x12u)   /* Invalid versioninfo pointer             */
#define SPI_E_UNINIT                            (0x1Au)   /* Not initialized                         */
#define SPI_E_SEQ_PENDING                       (0x2Au)   /* Seq Usage Conflict                      */
#define SPI_E_SEQ_IN_PROCESS                    (0x3Au)   /* Seq Usage Conflict                      */
#define SPI_E_ALREADY_INITIALIZED               (0x4Au)   /* Driver already initialized              */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8  Spi_DataType;

typedef uint16 Spi_NumberOfDataType;

/* the specification recommend an uint16 but uint8 should be sufficient */
typedef uint8  Spi_JobType;

typedef uint8  Spi_SequenceType;

typedef uint8  Spi_ChannelType;

typedef uint8  Spi_HWUnitType;

typedef uint8  Spi_ExternalDeviceIdType;

/* Notification Callback function declaration */
typedef P2FUNC(void, SPI_APPL_CODE, Spi_EndNotificationType) (void);

/* Global status type */
typedef enum
{
  SPI_UNINIT = 0,
  SPI_IDLE,
  SPI_BUSY
} Spi_StatusType;

/* Data processing mode (related to buffer, not SPI data width) */
typedef enum
{
  SPI_DATA_8BIT = 0,
  SPI_DATA_16BIT
} Spi_DataModeType;

/* Job status type */
typedef enum
{
  SPI_JOB_OK = 0,
  SPI_JOB_PENDING,
  SPI_JOB_FAILED
} Spi_JobResultType;

/* Sequence status type */
typedef enum
{
  SPI_SEQ_OK = 0,
  SPI_SEQ_PENDING,
  SPI_SEQ_FAILED,
  SPI_SEQ_CANCELLED
} Spi_SeqResultType;

typedef enum
{
  SPI_POLLING_MODE,
  SPI_INTERRUPT_MODE
} Spi_AsyncModeType;

/* Sequence Config type */
typedef struct
{
  Spi_JobType* JobList_pt;          /* The ptr to the job list                      */
  uint8 JobListSize_u8;             /* The size of the list                         */
  Spi_EndNotificationType SeqEnd_t; /* Seq end notification                         */
  uint8 SeqUninterruptable_u8;      /* The sequence is uninterruptable              */
  Spi_SeqResultType SeqStatus_t;    /* The status of the Sequence                   */
  uint8 CurrentJobIndex_u8;         /* The index of the current job in the Job List */
} Spi_SeqConfigType;

/* Job Config type */
typedef struct
{
  Spi_ChannelType* ChannelList_pt;  /* The ptr to the channel list                  */
  uint8 JobListSize_u8;             /* The size of the list                         */
  Spi_EndNotificationType JobEnd_t; /* Job end notification                         */
  uint8 JobPriority_u8;             /* The job priority value                       */
  Spi_ExternalDeviceIdType ExternalDeviceId_u8; /* ID of the external device        */
  Spi_JobResultType JobStatus_t;    /* The status of the job                        */
  uint8 CurrentChannelIndex_u8;     /* The index of the current chl in the Chl List */
} Spi_JobConfigType;

/* Channel Config type */
typedef struct
{
  Spi_DataType* BufferIB_Src_pt;            /* Pointer to the IB buffer. NULL_PTR for EB        */
  Spi_DataType* BufferIB_Dst_pt;            /* Pointer to the IB buffer. NULL_PTR for EB        */
  uint8 DefaultWriteValue_u8;               /* The default value to be used for Default writing */
  uint8 DefaultReadValue_u8;                /* The default value which is used when the Input-File reached EOF */
  uint16 HeadCtrlValue_u16;                 /* LSB/MSB parameter                                */
  Spi_NumberOfDataType MaxData_u16;         /* Max data amount for EB or Data quantity for IB   */
  Spi_DataModeType DataMode_t;              /* 8 or 16 bit mode buffer handling                 */
  const Spi_DataType* CurrentSrcBuf_t;      /* The current source buffer                        */
  Spi_DataType* CurrentDstBuf_t;            /* The current destination buffer                   */
  Spi_NumberOfDataType BufDataAmount_u16;   /* The DataAmount of the current buffers            */
} Spi_ChlConfigType;

/* External Device Config type */
typedef struct
{
  Spi_HWUnitType HwUnitId_u8;     /* The Id to identify the HWUnit       */
} Spi_ExternalDeviceType;

/* SPI driver initialization type */
typedef struct
{
  /* Sequence list and size */
  Spi_SeqConfigType * Spi_SeqList_pt;
  uint8 Spi_SeqListSize_u8;
  
  /* Job list and size */
  Spi_JobConfigType * Spi_JobList_pt;
  uint8 Spi_JobListSize_u8;
  
  /* Channel list and size */
  Spi_ChlConfigType * Spi_ChlList_pt;
  uint8 Spi_ChlListSize_u8;
  
  /* ExternalDevice list and size */
  Spi_ExternalDeviceType * Spi_ExternalDeviceList_pt;
  uint8 Spi_ExternalDeviceListSize_u8;

  /* HwUnitStatus Array and number of used HwUnits */
  Spi_StatusType * Spi_HwUnitStatus_pt;
  uint8 Spi_NrOfUsedHwUnits_u8;
} Spi_ConfigType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 *********************************************************************************************************************/
#include "Spi_Cfg.h"
#include "SchM_Spi.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, SPI_CODE) Spi_InitMemory (void);

FUNC(void, SPI_CODE) Spi_Init_Vtt (P2CONST(Spi_ConfigType, AUTOMATIC, SPI_APPL_CONST));

FUNC(Std_ReturnType, SPI_CODE) Spi_DeInit (void);

FUNC(Std_ReturnType, SPI_CODE) Spi_WriteIB (Spi_ChannelType, const Spi_DataType*);

FUNC(Std_ReturnType, SPI_CODE) Spi_ReadIB (Spi_ChannelType, Spi_DataType*);

FUNC(Std_ReturnType, SPI_CODE) Spi_SetupEB (Spi_ChannelType, const Spi_DataType*, Spi_DataType*, Spi_NumberOfDataType);

FUNC(Spi_StatusType, SPI_CODE) Spi_GetStatus (void);

FUNC(Spi_JobResultType, SPI_CODE) Spi_GetJobResult(Spi_JobType);

#if (STD_ON  == SPI_HW_STATUS_API)
  FUNC(Spi_StatusType, SPI_CODE) Spi_GetHWUnitStatus (Spi_HWUnitType);
#endif

FUNC(Spi_SeqResultType, SPI_CODE) Spi_GetSequenceResult (Spi_SequenceType);

#if (0 != SPI_LEVEL_DELIVERED)
  FUNC(Std_ReturnType, SPI_CODE) Spi_AsyncTransmit (Spi_SequenceType);
#endif

#if (1 != SPI_LEVEL_DELIVERED)
  FUNC(Std_ReturnType, SPI_CODE) Spi_SyncTransmit (Spi_SequenceType);
#endif

#if (STD_ON == SPI_CANCEL_API)
  FUNC(void, SPI_CODE) Spi_Cancel (Spi_SequenceType);
#endif

#if (2 == SPI_LEVEL_DELIVERED)
  FUNC(Std_ReturnType, SPI_CODE) Spi_SetAsyncMode (Spi_AsyncModeType);
#endif

#if (STD_ON == SPI_VERSION_INFO_API)
  FUNC(void, SPI_CODE) Spi_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, SPI_APPL_DATA) versioninfo);
#endif

/* Function declaration should be provided by the SchM */
/* Schedule functions */
/* FUNC(void, SPI_CODE) Spi_MainFunction_Handling (void); */

/* FUNC(void, SPI_CODE) Spi_MainFunction_Driving (void); */

FUNC(void, SPI_CODE) Spi_CallMainFunction (void);


#endif /* SPI_H */


/**********************************************************************************************************************
 *  END OF FILE: Spi.h
 *********************************************************************************************************************/
