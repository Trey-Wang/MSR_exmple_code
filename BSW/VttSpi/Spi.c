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
 *         File:  Spi.c
 *      Project:  DrvSpi_VttCanoe01Asr
 *       Module:  MICROSAR Spi
 *    Generator:  -
 *
 *  Description:  This module manages the data transfer over the SPI bus. The module incorporates both the driver and
 *                handler.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define SPI_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Spi.h"


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Fls header file */
#if (  (SPI_SW_MAJOR_VERSION != (2U)) \
    || (SPI_SW_MINOR_VERSION != (5U)) \
    || (SPI_AR_RELEASE_MAJOR_VERSION != (4U)) \
    || (SPI_AR_RELEASE_MINOR_VERSION != (0U)) \
    || (SPI_AR_RELEASE_PATCH_VERSION != (3U)))
  #error "Version numbers of Spi.c and Spi.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (SPI_CFG_MAJOR_VERSION != (3U)) \
    || (SPI_CFG_MINOR_VERSION != (3U)))
  #error "Version numbers of Spi.c and Spi_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define SPI_MAX_NUMBER_OF_HW_UNITS        (12u)

/* HeadCtrlValue Define LSB / MSB */
#define SPI_CHL_MSB_FIRST                 (0u)
#define SPI_CHL_LSB_FIRST                 (1u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define SPI_CHL(channel)    (Spi_Config_pt->Spi_ChlList_pt[channel])
#define SPI_ED(extDev)      (Spi_Config_pt->Spi_ExternalDeviceList_pt[extDev])
#define SPI_JOB(job)        (Spi_Config_pt->Spi_JobList_pt[job])
#define SPI_SEQ(seq)        (Spi_Config_pt->Spi_SeqList_pt[seq])
#define SPI_HWUNIT_STATUS(hwUnit)  (Spi_Config_pt->Spi_HwUnitStatus_pt[hwUnit])

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#if (STD_ON == SPI_DEV_ERROR_DETECT)
static VAR(boolean, SPI_VAR) Spi_ModuleInitialized_b = FALSE;
#endif

/* The current Config pointer, given by the client upon init */
STATIC P2CONST(Spi_ConfigType, AUTOMATIC, SPI_APPL_CONST) Spi_Config_pt = NULL_PTR;

/* The current mode of the Spi module   */
static VAR(Spi_AsyncModeType, SPI_VAR) Spi_CurrentMode   = SPI_POLLING_MODE;  

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SPI_CODE) Spi_TransmitChannel    (Spi_ChannelType Channel, Spi_HWUnitType HWUnit);
STATIC FUNC(uint8, SPI_CODE)          Spi_ChangeBitDirection (uint8 Data);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Spi_ChangeBitDirection
 *********************************************************************************************************************/
/*! \brief      The function changes Bit Dircetion. LSB => MSB or MSB => LSB
 *  \param[in]  uint8 Data
 *  \return     uint8 Data
 *  \pre        
 *  \context    Function is called in driver context
 *  \note
 *********************************************************************************************************************/
STATIC FUNC(uint8, SPI_CODE) Spi_ChangeBitDirection (uint8 Data)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 PermutationArray[]=
  {
    0x00, /* 0 fliped  */
    0x08, /* 1 fliped  */
    0x04, /* 2 fliped  */
    0x0C, /* 3 fliped  */
    0x02, /* 4 fliped  */
    0x0A, /* 5 fliped  */
    0x06, /* 6 fliped  */
    0x0E, /* 7 fliped  */
    0x01, /* 8 fliped  */
    0x09, /* 9 fliped  */
    0x05, /* A fliped  */
    0x0D, /* B fliped  */
    0x03, /* C fliped  */
    0x0B, /* D fliped  */
    0x07, /* E fliped  */
    0x0F  /* F fliped  */
  };
  uint8 HighNibble;
  uint8 LowNibble;

  /* ----- Implementation ----------------------------------------------- */
  /* Fill nibbles with the data */
  HighNibble = (Data >> 4) & 0x0F;
  LowNibble  = Data & 0x0F;

  /* Flip the bits in the nibbles */
  HighNibble = PermutationArray[HighNibble];
  LowNibble = PermutationArray[LowNibble];

  /* Rotate the nibbles and insert it into Data */
  Data = (LowNibble << 4) | HighNibble;

  return Data;
} /* End of Spi_ChangeBitDirection() */


/**********************************************************************************************************************
 *  Spi_TransmitChannel
 *********************************************************************************************************************/
/*! \brief      The function transmits a Channel over the passed HWUnit
 *  \param[in]  Spi_ChannelType   Channel
 *  \param[in]  Spi_HWUnitType    HWUnit
 *  \return     Std_ReturnType
 *  \pre        
 *  \context    Function is called in Spi_MainFunction_Handling driver context
 *  \note
 *********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SPI_CODE) Spi_TransmitChannel (Spi_ChannelType Channel, Spi_HWUnitType HwUnit)
{
  /* ----- Local Variables ---------------------------------------------- */
  Spi_ChannelType temp_ch; 
  Spi_HWUnitType temp_hwunit;

  /* ----- Implementation ----------------------------------------------- */
  temp_ch = Channel; 
  temp_hwunit = HwUnit;

  return E_OK;
} /* End of Spi_TransmitChannel() */


/**********************************************************************************************************************
 *  Spi_Transmit
 *********************************************************************************************************************/
/*! \brief      This function sets the status and flags before the initiation of the transmission process. 
 *  \param[in]  Sequence                       Sequence ID
 *  \param[in]  ServiceID                      Seervice ID (not used)
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in Spi_AsyncTransmit() and Spi_SyncTransmit() driver's context
 *  \note
 *********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SPI_CODE) Spi_Transmit (Spi_SequenceType Sequence, uint8 ServiceID)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if any Job of this Seq is pending => if yes the E_NOT_OK because a other Seq is Pending */
  for (Index=0; Index < SPI_SEQ(Sequence).JobListSize_u8; Index++)
  {
    if (SPI_JOB_PENDING == SPI_JOB(SPI_SEQ(Sequence).JobList_pt[Index]).JobStatus_t)
    {
      /* The Job is busy => So the Sequnece can't start */
      return E_NOT_OK;
    }
  }
  /* Service ID not used  => So prevent warning */
  ServiceID = ServiceID;

  /* All preconditions are ok => go ahead */

  /* Set Seq State to pending */
  SPI_SEQ(Sequence).SeqStatus_t = SPI_SEQ_PENDING;

  /* Set first Job of the Sequence to pending */
  SPI_JOB(SPI_SEQ(Sequence).JobList_pt[0]).JobStatus_t = SPI_JOB_PENDING;
  SPI_SEQ(Sequence).CurrentJobIndex_u8 = 0;

  return E_OK;
} /* End of Spi_Transmit() */



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Spi_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, SPI_CODE) Spi_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
   Spi_ModuleInitialized_b = FALSE;
#endif
  Spi_Config_pt           = NULL_PTR;
  Spi_CurrentMode         = SPI_POLLING_MODE;
  
  return;
}


/**********************************************************************************************************************
 *  Spi_Init
 *********************************************************************************************************************/
/*! \brief      This service initializes the driver and the Spi hardware
 *  \param[in]  ConfigPtr                      Input parameter
 *  \return     void
 *  \pre        Input parameter must not be NULL.
 *  \context    Function is called by EcuM
 *  \note
 *********************************************************************************************************************/
FUNC(void, SPI_CODE) Spi_Init_Vtt (P2CONST(Spi_ConfigType, AUTOMATIC, SPI_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;
  char OutputMode[2];
  OutputMode[1] = 0x00; /* String delimiter */

#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_PARAM_POINTER */
  if (ConfigPtr == NULL_PTR)
    {
      Spi_Errorhook (SPI_SID_INIT, SPI_E_PARAM_CONFIG);
      return;
    }

  /* check if driver is already initialized */
  if (Spi_ModuleInitialized_b == TRUE)
  {
    Spi_Errorhook (SPI_SID_INIT, SPI_E_ALREADY_INITIALIZED);
    return;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Store the config pointer */
  Spi_Config_pt = ConfigPtr;

  /* Init Channels */
  for (Index = 0; Index < Spi_Config_pt->Spi_ChlListSize_u8; Index++)
  {
    SPI_CHL(Index).CurrentDstBuf_t = NULL_PTR;
    SPI_CHL(Index).CurrentSrcBuf_t = NULL_PTR;
    SPI_CHL(Index).BufDataAmount_u16 = 0;
  }

  /* Init Jobs */
  for (Index = 0; Index < Spi_Config_pt->Spi_JobListSize_u8; Index++)
  {
    SPI_JOB(Index).CurrentChannelIndex_u8 = SPI_CHL_NONE;
    SPI_JOB(Index).JobStatus_t = SPI_JOB_OK;
    SPI_HWUNIT_STATUS(SPI_ED(SPI_JOB(Index).ExternalDeviceId_u8).HwUnitId_u8) = SPI_IDLE;
  }

  /* Init Seqs */
  for (Index = 0; Index < Spi_Config_pt->Spi_SeqListSize_u8; Index++)
  {
    SPI_SEQ(Index).CurrentJobIndex_u8 = SPI_JOB_NONE;
    SPI_SEQ(Index).SeqStatus_t = SPI_SEQ_OK;
  }

#if (STD_ON == SPI_DEV_ERROR_DETECT)
  Spi_ModuleInitialized_b = TRUE;
#endif

  return;
} /* End of Spi_Init() */


/**********************************************************************************************************************
 *  Spi_DeInit
 *********************************************************************************************************************/
/*! \brief      The function resets the module state
 *  \param[in]  void
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called by EcuM
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_DeInit (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_DEINIT, SPI_E_UNINIT);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  if (SPI_BUSY == Spi_GetStatus())
  {
    /* A Process is ongoing so do not DeInit the Spi module */
    return E_NOT_OK;
  }

  /* Deinit Channels */
  for (Index = 0; Index < Spi_Config_pt->Spi_ChlListSize_u8; Index++)
  {
    SPI_CHL(Index).CurrentDstBuf_t = NULL_PTR;
    SPI_CHL(Index).CurrentSrcBuf_t = NULL_PTR;
    SPI_CHL(Index).BufDataAmount_u16 = 0;
  }

  /* Deinit Jobs */
  for (Index = 0; Index < Spi_Config_pt->Spi_JobListSize_u8; Index++)
  {
    SPI_JOB(Index).CurrentChannelIndex_u8 = SPI_CHL_NONE;
    SPI_JOB(Index).JobStatus_t = SPI_JOB_OK;
  }

  /* Deinit Seqs */
  for (Index = 0; Index < Spi_Config_pt->Spi_SeqListSize_u8; Index++)
  {
    SPI_SEQ(Index).CurrentJobIndex_u8 = SPI_JOB_NONE;
    SPI_SEQ(Index).SeqStatus_t = SPI_SEQ_OK;
  }

  Spi_Config_pt = (Spi_ConfigType*)NULL_PTR;

  #if (STD_ON == SPI_DEV_ERROR_DETECT)
    Spi_ModuleInitialized_b = FALSE;
  #endif

  return E_OK;
} /* End of Spi_Deinit() */


#if ( (SPI_CHANNEL_BUFFERS_ALLOWED == 1 || SPI_CHANNEL_BUFFERS_ALLOWED == 2) )
/**********************************************************************************************************************
 *  Spi_SetupEB
 *********************************************************************************************************************/
/*! \brief      Service to setup the buffers and the length of data for the EB SPI Handler/Driver Channel specified. 
 *  \param[in]  Channel                        Channel Id
 *  \param[in]  SrcDataBufferPtr               Pointer to source data buffer
 *  \param[in]  DesDataBufferPtr               Pointer to destination data buffer
 *  \param[in]  Length                         Number of data words to transfer
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_SetupEB 
(
  Spi_ChannelType Channel, 
  const Spi_DataType* SrcDataBufferPtr,
  Spi_DataType* DesDataBufferPtr,
  Spi_NumberOfDataType Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt)
  {
    Spi_Errorhook (SPI_SID_SETUP_EB, SPI_E_UNINIT);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_CHL */
  if (Channel >= Spi_Config_pt->Spi_ChlListSize_u8)
  {
    Spi_Errorhook (SPI_SID_SETUP_EB, SPI_E_PARAM_CHANNEL);
    return E_NOT_OK;
  }

  if ( ( NULL_PTR != SPI_CHL(Channel).BufferIB_Src_pt ) ||
       ( NULL_PTR != SPI_CHL(Channel).BufferIB_Dst_pt ) )
  {
    Spi_Errorhook (SPI_SID_SETUP_EB, SPI_E_PARAM_CHANNEL);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_LENGTH */
  /* Check if Length is zero or to big. In 16-Bit channel mode also check if Length is even */
  if ( (0 == Length) ||
       (Length > (SPI_CHL(Channel).MaxData_u16)) ||
       ( (SPI_DATA_16BIT == SPI_CHL(Channel).DataMode_t)  && (Length % 2 == 1) ) )
  {
    Spi_Errorhook (SPI_SID_SETUP_EB, SPI_E_PARAM_LENGTH);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  SPI_CHL(Channel).CurrentDstBuf_t = DesDataBufferPtr;
  SPI_CHL(Channel).CurrentSrcBuf_t = SrcDataBufferPtr;
  SPI_CHL(Channel).BufDataAmount_u16 = Length;

  return E_OK;
} /* End of Spi_SetupEB() */
#endif /* ( (SPI_CHANNEL_BUFFERS_ALLOWED == 1 || SPI_CHANNEL_BUFFERS_ALLOWED == 2) ) */


#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0 || SPI_CHANNEL_BUFFERS_ALLOWED == 2))
/**********************************************************************************************************************
 *  Spi_ReadIB
 *********************************************************************************************************************/
/*! \brief      Service for reading synchronously one or more data from an 
 *              IB SPI Handler/Driver Channel specified by parameter. 
 *  \param[in]  Channel                        Channel Id
 *  \param[in]  DataBufferPtr                  Pointer to destination data buffer in RAM 
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_ReadIB (Spi_ChannelType Channel, Spi_DataType* DataBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_READIB, SPI_E_UNINIT);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_CHL */
  if ( (Channel  >= (Spi_Config_pt->Spi_ChlListSize_u8)) ||
       (NULL_PTR == SPI_CHL(Channel).BufferIB_Dst_pt) )
  {
    Spi_Errorhook (SPI_SID_READIB, SPI_E_PARAM_CHANNEL);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_POINTER */
  if (DataBufferPtr == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_READIB, SPI_E_PARAM_POINTER);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  for (Index = 0; Index < SPI_CHL(Channel).MaxData_u16; Index++)
  {
    DataBufferPtr[Index] = SPI_CHL(Channel).BufferIB_Dst_pt[Index];
  }
   
  return E_OK;
} /* End of Spi_ReadIB() */
#endif /* ((SPI_CHANNEL_BUFFERS_ALLOWED == 0 || SPI_CHANNEL_BUFFERS_ALLOWED == 2)) */


#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0 || SPI_CHANNEL_BUFFERS_ALLOWED == 2))
/**********************************************************************************************************************
 *  Spi_WriteIB
 *********************************************************************************************************************/
/*! \brief      Service for writing one or more data to an IB SPI Handler/Driver Channel specified by parameter. 
 *  \param[in]  Channel                        Channel Id
 *  \param[in]  DataBufferPtr                  Pointer to destination data buffer in RAM 
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note       If the DataBufferPtr is null, it is assumed that the data to be transmitted 
 *              is not relevant and the default transmit value of this channel will be used instead. 
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_WriteIB (Spi_ChannelType Channel, const Spi_DataType* DataBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt)
  {
    Spi_Errorhook (SPI_SID_WRITEIB, SPI_E_UNINIT);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_CHL */
  if ( (Channel  >= (Spi_Config_pt->Spi_ChlListSize_u8)) || 
       (NULL_PTR == SPI_CHL(Channel).BufferIB_Src_pt ) )
  {
    Spi_Errorhook (SPI_SID_WRITEIB, SPI_E_PARAM_CHANNEL);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  if (NULL_PTR == DataBufferPtr)
  {
    /* Fill the buffer with dummy data */
    for (Index = 0; Index < SPI_CHL(Channel).MaxData_u16; Index++)
    {
      SPI_CHL(Channel).BufferIB_Src_pt[Index] = SPI_CHL(Channel).DefaultWriteValue_u8;
    }
  }
  else
  {
    /* Fill the buffer with the user data from DataBufferPtr */
    for (Index = 0; Index < SPI_CHL(Channel).MaxData_u16; Index++)
    {
      SPI_CHL(Channel).BufferIB_Src_pt[Index] = DataBufferPtr[Index];
    }
  }

  return E_OK;
} /* End of Spi_WriteIB() */
#endif /* ((SPI_CHANNEL_BUFFERS_ALLOWED == 0 || SPI_CHANNEL_BUFFERS_ALLOWED == 2)) */


/**********************************************************************************************************************
 *  Spi_GetStatus
 *********************************************************************************************************************/
/*! \brief      Service returns the SPI Handler/Driver software module status. 
 *  \param[in]  void
 *  \return     Spi_StatusType
 *  \pre        
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Spi_StatusType, SPI_CODE) Spi_GetStatus (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;

  if (NULL_PTR == Spi_Config_pt)
  {
      return SPI_UNINIT;
  }

    /* Check the HWUnit's */
    for (Index=0; Index < Spi_Config_pt->Spi_ExternalDeviceListSize_u8; Index++)
    {
      if (SPI_BUSY == SPI_HWUNIT_STATUS(SPI_ED(Index).HwUnitId_u8))
      {
         /* A HWUnit is busy => So the module is busy */
          return SPI_BUSY;
      }
    }
    

  /* ----- Implementation ------------------------------------------------- */

  /* Check Jobs */
  for (Index=0; Index < Spi_Config_pt->Spi_JobListSize_u8; Index++)
  {
    if (SPI_JOB_PENDING == SPI_JOB(Index).JobStatus_t)
    {
      /* The Job is busy => So the module is busy */
      return SPI_BUSY;
    }
  }

  /* Check Seqs */
  for (Index=0; Index < Spi_Config_pt->Spi_SeqListSize_u8; Index++)
  {
    if (SPI_SEQ_PENDING == SPI_SEQ(Index).SeqStatus_t)
    {
      /* The Seq is pending => So the module is busy */
      return SPI_BUSY;
    }
  }

  /* No activity, IDLE state */
  return SPI_IDLE;
} /* End of Spi_GetStatus() */


/**********************************************************************************************************************
 *  Spi_GetJobResult
 *********************************************************************************************************************/
/*! \brief      This service returns the last transmission result of the specified Job.
 *  \param[in]  Job                            Job Id
 *  \return     Spi_JobResultType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Spi_JobResultType, SPI_CODE) Spi_GetJobResult (Spi_JobType Job)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_GET_JOB_RES, SPI_E_UNINIT);
    return SPI_JOB_FAILED;
  }

  /* check SPI_CFG_CHK_PARAM_JOB */
  if (Job >= Spi_Config_pt->Spi_JobListSize_u8)
  {
    Spi_Errorhook (SPI_SID_GET_JOB_RES, SPI_E_PARAM_JOB);
    return SPI_JOB_FAILED;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  return SPI_JOB(Job).JobStatus_t;
} /* End of Spi_GetJobResult() */


#if (STD_ON == SPI_HW_STATUS_API)
/**********************************************************************************************************************
 *  Spi_GetHWUnitStatus
 *********************************************************************************************************************/
/*! \brief      This service returns the status of the specified SPI Hardware microcontroller peripheral. 
 *  \param[in]  HWUnit                         SPI Hardware microcontroller peripheral (unit) ID
 *  \return     Spi_StatusType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Spi_StatusType, SPI_CODE) Spi_GetHWUnitStatus (Spi_HWUnitType HwUnit)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt)
  {
    Spi_Errorhook (SPI_SID_GET_HW_STAT, SPI_E_UNINIT);
    return SPI_UNINIT;
  }

  /* check if HWUnit is within the specified range */
  if (HwUnit >= SPI_MAX_NUMBER_OF_HW_UNITS)
  {
    Spi_Errorhook (SPI_SID_GET_HW_STAT, SPI_E_PARAM_UNIT);
    return SPI_UNINIT;                  
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  return SPI_HWUNIT_STATUS(HwUnit);
} /* End of Spi_GetHWUnitStatus() */
#endif /* (STD_ON == SPI_HW_STATUS_API) */


/**********************************************************************************************************************
 *  Spi_GetSequenceResult
 *********************************************************************************************************************/
/*! \brief      This service returns the last transmission result of the specified Sequence. 
 *  \param[in]  Sequence                       Sequence ID
 *  \return     Spi_SeqResultType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Spi_SeqResultType, SPI_CODE) Spi_GetSequenceResult (Spi_SequenceType Sequence)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt)
  {
    Spi_Errorhook (SPI_SID_GET_SEQ_RES, SPI_E_UNINIT);
    return SPI_SEQ_FAILED;
  }

  /* check SPI_CFG_CHK_PARAM_SEQ */
  if (Sequence >= Spi_Config_pt->Spi_SeqListSize_u8)
  {
    Spi_Errorhook (SPI_SID_GET_SEQ_RES, SPI_E_PARAM_SEQ);
    return SPI_SEQ_FAILED;
  }
#endif (STD_ON == SPI_DEV_ERROR_DETECT)

  /* ----- Implementation ------------------------------------------------- */
  return SPI_SEQ(Sequence).SeqStatus_t;
} /* End of Spi_GetSequenceResult() */


#if (SPI_LEVEL_DELIVERED == 1 || SPI_LEVEL_DELIVERED == 2)
/**********************************************************************************************************************
 *  Spi_AsyncTransmit
 *********************************************************************************************************************/
/*! \brief      Service to transmit data on the SPI bus. 
 *  \param[in]  Sequence                       Sequence ID
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_AsyncTransmit (Spi_SequenceType Sequence)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  uint16 Index1, Index2, Index3;
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt )
  {
    Spi_Errorhook (SPI_SID_ASYNC_TRANSMIT, SPI_E_UNINIT);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_SEQ */
  if (Sequence >= Spi_Config_pt->Spi_SeqListSize_u8)
  {
    Spi_Errorhook (SPI_SID_ASYNC_TRANSMIT, SPI_E_PARAM_SEQ);
    return E_NOT_OK;
  }

  /* Check the status of all sequences with same jobs */
  for (Index1 = 0; Index1 < Spi_Config_pt->Spi_SeqListSize_u8; Index1++)
  {
    if (SPI_SEQ_PENDING == SPI_SEQ(Index1).SeqStatus_t)
    {
      for (Index2 = 0; Index2 < SPI_SEQ(Index1).JobListSize_u8; Index2++)
      {
        for (Index3 = 0; Index3 < SPI_SEQ(Sequence).JobListSize_u8; Index3++)
        {
          if (SPI_SEQ(Index1).JobList_pt[Index2] == SPI_SEQ(Sequence).JobList_pt[Index3])
          {
            Spi_Errorhook (SPI_SID_ASYNC_TRANSMIT, SPI_E_SEQ_PENDING);
            return E_NOT_OK;
          }
        }
      }
    }
  }

  /* Check the status of the sequence */
  if (SPI_SEQ_PENDING == SPI_SEQ(Sequence).SeqStatus_t)
  {
    Spi_Errorhook (SPI_SID_ASYNC_TRANSMIT, SPI_E_SEQ_PENDING);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  /* We can relay the request to someone else */
  return Spi_Transmit (Sequence, SPI_SID_ASYNC_TRANSMIT);
} /* End of Spi_AsyncTransmit() */
#endif /* (SPI_LEVEL_DELIVERED == 1 || SPI_LEVEL_DELIVERED == 2) */


#if (SPI_LEVEL_DELIVERED == 0 || SPI_LEVEL_DELIVERED == 2)
/**********************************************************************************************************************
 *  Spi_SyncTransmit
 *********************************************************************************************************************/
/*! \brief      Service to transmit data on the SPI bus. 
 *  \param[in]  Sequence                       Sequence ID
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_SyncTransmit (Spi_SequenceType Sequence)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType Return_t;
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  uint16 Index;
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* check SPI_CFG_CHK_STATUS_UNINIT */
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_SYNC_TRANSMIT, SPI_E_UNINIT);
    return E_NOT_OK;
  }

  /* check SPI_CFG_CHK_PARAM_SEQ */
  if (Sequence >= Spi_Config_pt->Spi_SeqListSize_u8)
  {
    Spi_Errorhook (SPI_SID_SYNC_TRANSMIT, SPI_E_PARAM_SEQ);
    return E_NOT_OK;
  }

  /* Check Seqs */
  for (Index=0; Index < Spi_Config_pt->Spi_SeqListSize_u8; Index++)
  {
    if (SPI_SEQ_PENDING == SPI_SEQ(Index).SeqStatus_t)
    {
      /* A Seq is pending */
      Spi_Errorhook (SPI_SID_SYNC_TRANSMIT, SPI_E_SEQ_IN_PROCESS);
      return E_NOT_OK;
    }
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  /* Forward this request to the other service */
  Return_t = Spi_Transmit (Sequence, SPI_SID_SYNC_TRANSMIT);

  if (Return_t != E_OK)
  {
    /* Something wrong */
    return Return_t;
  }

  {
    /* Please refer to AUTOSAR SWS LEVEL 0, Simple Synchronous Behaviour */
    /* 
     * A simple synchronous transmission means that the user calling the
     * transmission service is blocked until the transmission is not terminated. 
     */
    volatile Spi_SeqResultType State_t;
    do
    {
      /* Waste the time and look at the state */
      State_t = SPI_SEQ(Sequence).SeqStatus_t;
      Spi_CallMainFunction();
      /* Spi_MainFunction_Handling(); */

    }  while (SPI_SEQ_PENDING == State_t);
  }
  
  /* Finally it is over... */
  return E_OK;
} /* End of Spi_SyncTransmit() */
#endif /* (SPI_LEVEL_DELIVERED == 0 || SPI_LEVEL_DELIVERED == 2) */


#if (STD_ON == SPI_CANCEL_API)
/**********************************************************************************************************************
 *  Spi_Cancel
 *********************************************************************************************************************/
/*! \brief      Service cancels the specified on-going sequence transmission. 
 *  \param[in]  Sequence                       Sequence ID
 *  \return     void
 *  \pre        Module must be initialized and transmission must be ongoing
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(void, SPI_CODE) Spi_Cancel (Spi_SequenceType Sequence)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* SPI_CFG_CHK_STATUS_UNINIT */
  if (NULL_PTR == Spi_Config_pt )
  {
    Spi_Errorhook (SPI_SID_CANCEL, SPI_E_UNINIT);
    return;
  }
  /* SPI_CFG_CHK_PARAM_SEQ */
  if (Sequence >= Spi_Config_pt->Spi_SeqListSize_u8)
  {
    Spi_Errorhook (SPI_SID_CANCEL, SPI_E_PARAM_SEQ);
    return;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  /* Check the status of the sequence */
  if (SPI_SEQ_PENDING != SPI_SEQ(Sequence).SeqStatus_t)
  {
    /* Wrong sequence. Perhaps it is already stopped */
    return;
  }

  /* Set the next job to failed */
  SPI_JOB(SPI_SEQ(Sequence).JobList_pt[SPI_SEQ(Sequence).CurrentJobIndex_u8]).JobStatus_t = SPI_JOB_FAILED;

  /* Set the Sequence to canceled */
  SPI_SEQ(Sequence).SeqStatus_t =  SPI_SEQ_CANCELLED;
  SPI_HWUNIT_STATUS(SPI_ED(SPI_JOB(SPI_SEQ(Sequence).CurrentJobIndex_u8).ExternalDeviceId_u8).HwUnitId_u8) = SPI_IDLE;
  SPI_SEQ(Sequence).CurrentJobIndex_u8 = SPI_JOB_NONE;

  /* Check if the sequence was cancelled before and call the notification if configured */
  if (NULL_PTR != SPI_SEQ(Sequence).SeqEnd_t)
  {
    SPI_SEQ(Sequence).SeqEnd_t();
  }

  return;
} /* End of Spi_Cancel() */
#endif /* (STD_ON == SPI_CANCEL_API) */


#if (SPI_LEVEL_DELIVERED == 2)
/**********************************************************************************************************************
 *  Spi_SetAsyncMode
 *********************************************************************************************************************/
/*! \brief      This service sets the async mode (POLLING or INTERRUPT)
 *              currently not implemented due to the fact that this is an emulation
 *  \param[in]  ModeId                         Mode Id 
 *  \return     Std_ReturnType
 *  \pre        Module must be initialized
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_SetAsyncMode (Spi_AsyncModeType ModeId)
{
    /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  /* SPI_CFG_CHK_STATUS_UNINIT */
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_SET_ASYNC_MODE, SPI_E_UNINIT);
    return E_NOT_OK;
  }
    
  /* SPI_CFG_CHK_PARAM_MODE */
  if ( (ModeId != SPI_POLLING_MODE) && (ModeId != SPI_INTERRUPT_MODE) )
  {
    Spi_Errorhook (SPI_SID_SET_ASYNC_MODE, SPI_E_PARAM_MODE);
    return E_NOT_OK;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  /* Ask for the module status */ 
  /* TODO: When synchronous transmission is ongoing, even so set the ModeId and return E_OK */
  if (Spi_GetStatus() != SPI_IDLE)
  {
    /* Wrong question at the wrong time */
    return E_NOT_OK;
  }

  /* Mode selection is not possible due to the fact that this is an emulation */
  return E_OK;
} /* End of Spi_SetAsyncMode() */
#endif /* (SPI_LEVEL_DELIVERED == 2) */


#if (STD_ON == SPI_VERSION_INFO_API)
/**********************************************************************************************************************
 *  Spi_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *              The version information includes:
 *              - Module Id
 *              - Vendor Id
 *              - Vendor specific version numbers
 *  \param[in]  versioninfo (out)               Pointer to where to store the version information of this module. 
 *  \return     void
 *  \pre        
 *  \context    Function is called in driver's context
 *  \note
 *********************************************************************************************************************/
FUNC(void, SPI_CODE) Spi_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, SPI_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  if (NULL_PTR == versioninfo)
  {
    Spi_Errorhook (SPI_SID_GET_VERSION_INFO, SPI_E_PARAM_VINFO);
    return;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  versioninfo->vendorID         = (SPI_VENDOR_ID);
  versioninfo->moduleID         = (SPI_MODULE_ID);
  versioninfo->sw_major_version = (SPI_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (SPI_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (SPI_SW_PATCH_VERSION);

  return;
}   /* End of Spi_GetVersionInfo() */
#endif  /* (STD_ON == SPI_VERSION_INFO_API) */

/**********************************************************************************************************************
 *  Spi_MainFunction_Handling
 *********************************************************************************************************************/
/*! \brief      The function receives the periodical handling  calls
 *  \param[in]  void
 *  \return     void
 *  \pre        Module must be initialized
 *  \context    Function is called by Os scheduler
 *  \note
 *********************************************************************************************************************/
FUNC(void, SPI_CODE) Spi_MainFunction_Handling (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Index;
  uint16 SeqIndex = SPI_SEQ_NONE;
  uint16 JobIndex = SPI_JOB_NONE;
  uint8 HwUnitId;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == SPI_DEV_ERROR_DETECT)
  if (Spi_Config_pt == NULL_PTR)
  {
    Spi_Errorhook (SPI_SID_MAINFUNCTION_HANDLING, SPI_E_UNINIT);
    return;
  }
#endif /* (STD_ON == SPI_DEV_ERROR_DETECT) */

  /* ----- Implementation ------------------------------------------------- */
  /* Do Spi Handling */
  /* Search for a pending Seq  NOTICE: This implementation don't consider Job priorities  */
  for (Index = 0; Index < Spi_Config_pt->Spi_SeqListSize_u8; Index++)
  {
    if (SPI_SEQ_PENDING == SPI_SEQ(Index).SeqStatus_t)
    {
      /* Seq to process found so break */
      SeqIndex = Index;
      break;
    }
  }

  /* Check if a Seq was found */
  if (SPI_SEQ_NONE == SeqIndex)
  {
    /* No pending Seq found  => Nothing to do */
    return;
  }

  /* Get the Job ID from Seq for the next Job */
  JobIndex = SPI_SEQ(SeqIndex).JobList_pt[SPI_SEQ(SeqIndex).CurrentJobIndex_u8];

    /* Set the HwUnit is a busy state */
  HwUnitId = SPI_ED(SPI_JOB(JobIndex).ExternalDeviceId_u8).HwUnitId_u8;
  SPI_HWUNIT_STATUS(HwUnitId) = SPI_BUSY;

  /* Process the next pending Job */
  /* Process all channels in the Job */
  for (Index = 0; Index < SPI_JOB(JobIndex).JobListSize_u8; Index++)
  {
    if (E_NOT_OK ==  Spi_TransmitChannel(SPI_JOB(JobIndex).ChannelList_pt[Index], HwUnitId))
    {
      SPI_JOB(JobIndex).JobStatus_t = SPI_JOB_FAILED;
      SPI_SEQ(SeqIndex).SeqStatus_t = SPI_SEQ_FAILED;

      SPI_HWUNIT_STATUS(HwUnitId) = SPI_IDLE;
      return;
    }
  }
  SPI_HWUNIT_STATUS(HwUnitId) = SPI_IDLE;

  /* Set job status to ok because job finished */
  SPI_JOB(JobIndex).JobStatus_t = SPI_JOB_OK;


  /* Job finished and notification existing  => Call Notification */
  if (NULL_PTR != SPI_JOB(JobIndex).JobEnd_t)
  {
    SPI_JOB(JobIndex).JobEnd_t();
  }

  /* Check if this Job was the last Job in the Seq */
  if (SPI_SEQ(SeqIndex).CurrentJobIndex_u8 == (SPI_SEQ(SeqIndex).JobListSize_u8 - 1))
  {
    /* This was the Last job in the Seq => So change State of Seq and call Notification */
    SPI_SEQ(SeqIndex).SeqStatus_t = SPI_SEQ_OK;

    /* Seq finished and notification existing  => Call Notification */
    if (NULL_PTR != SPI_SEQ(SeqIndex).SeqEnd_t)
    {
      /* Call Notification */
      SPI_SEQ(SeqIndex).SeqEnd_t();
    }
  }
  else
  {
    /* The Job was not the last Job in the Seq => so increase JobIndex */
    SPI_SEQ(SeqIndex).CurrentJobIndex_u8++;

    /* Set new Job to pending */
    JobIndex = SPI_SEQ(SeqIndex).JobList_pt[SPI_SEQ(SeqIndex).CurrentJobIndex_u8];
    SPI_JOB(JobIndex).JobStatus_t = SPI_JOB_PENDING;
  }

  return;
} /* End of Spi_MainFunction_Handling() */


FUNC(void, SPI_CODE) Spi_MainFunction_Driving(void)
{
  /* This is just a stub, provided for compatibility reasons. */
} /* End of Spi_MainFunction_Driving() */

FUNC(void, SPI_CODE) Spi_CallMainFunction (void)
{
  Spi_MainFunction_Handling();
}


/**********************************************************************************************************************
 *  END OF FILE: Spi.c
 *********************************************************************************************************************/
