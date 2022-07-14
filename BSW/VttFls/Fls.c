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
 *         File:  Fls.c
 *      Project:  DrvFls_VttCanoe01Asr
 *       Module:  MICROSAR Fls
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Fls implements a Flash driver for the CANoe Emulation. Instead of a real Flash
 *                peripheral a text file is used as non-volatile memory.
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

#define FLS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fls.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Fls header file */
#if (  (FLS_SW_MAJOR_VERSION != (0x03u)) \
    || (FLS_SW_MINOR_VERSION != (0x03u)) \
    || (FLS_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (FLS_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (FLS_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Fls.c and Fls.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (FLS_CFG_MAJOR_VERSION != (0x04u)) \
    || (FLS_CFG_MINOR_VERSION != (0x04u)) )
  #error "Version numbers of Fls.c and Fls_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

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
/* Fls_JobResult_t holds the result of the last job */
STATIC VAR(MemIf_JobResultType, FLS_VAR) Fls_JobResult_t = MEMIF_JOB_OK;
/* Fls_ReadMode_t holds the currrent mode (norma/fast) */
STATIC VAR(MemIf_ModeType, FLS_VAR)      Fls_ReadMode_t  = MEMIF_MODE_SLOW;
/* PointToConfig_pt holds the pointer to the configuration structure  */
STATIC P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_CONST) PointToConfig_pt = NULL_PTR;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
/* Fls_CurrJob_t holds the struct holds status of the current job */
VAR(Fls_Job_Struct, FLS_VAR)      Fls_CurrJob_t   = {FLS_NO_JOB, 0, 0, 0, 0, NULL_PTR, NULL_PTR};
/* Fls_Status_t holds the status of the Fls */
VAR(MemIf_StatusType, FLS_VAR)    Fls_Status_t    = MEMIF_UNINIT;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Fls_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_InitMemory (void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* Initialize all module-global variables */
  Fls_Status_t     = MEMIF_UNINIT;          /* status of the Fls */
  Fls_JobResult_t  = MEMIF_JOB_OK;       /* result of the last job */
  Fls_ReadMode_t   = MEMIF_MODE_SLOW;     /* Currrent mode (normal/fast) */
  PointToConfig_pt = NULL_PTR;          /* Pointer to conf. struct. */

  /* Init the struct that holds status of the current job */
  Fls_CurrJob_t.JobType      = FLS_NO_JOB;
  Fls_CurrJob_t.StartAddress = 0;
  Fls_CurrJob_t.CurrAddress  = 0;
  Fls_CurrJob_t.JobLength    = 0;
  Fls_CurrJob_t.CurrByte     = 0;
  Fls_CurrJob_t.WriteFlsPointer = NULL_PTR;
  Fls_CurrJob_t.ReadFlsPointer  = NULL_PTR;

  return;
} /* End of Fls_InitMemory() */


/**********************************************************************************************************************
 *  Fls_Init
 *********************************************************************************************************************/
/*! \brief      Initialisation of the Fls Driver
 *  \param[in]  ConfigPtr  Pointer to flash driver configuration set
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \context    Function could be called from task level during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_Init_Vtt (P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_INIT, FLS_E_BUSY);
    return;
  }

  /* Is there a valid configuration */
  if (NULL_PTR == ConfigPtr)
  {
    Fls_Errorhook (FLS_SID_INIT, FLS_E_PARAM_CONFIG);
    return;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Initialization of the emulated hardware (<Filename>.nvram) is done in VttEep_Onload() or VttFls_OnLoad() */

  Fls_Status_t = MEMIF_IDLE;
  Fls_JobResult_t = MEMIF_JOB_OK;
  PointToConfig_pt = ConfigPtr; /* set the local configuration pointer to the Fls config struct    */
  /* now the data of the config should be available inside the module  */

  Fls_ReadMode_t = PointToConfig_pt->DefaultMode;

  return;
} /* End of Fls_Init() */


/**********************************************************************************************************************
 *  Fls_Erase
 *********************************************************************************************************************/
/*! \brief      This function erases a data block.
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to erase
 *  \return     E_OK                    Erase command has been accepted
 *  \return     E_NOT_OK                Erase command has not been accepted
 *  \pre        Module has to be initialized.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Erase (Fls_AddressType FlsAddress, Fls_LengthType Length)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
      /* Raise development error when this check is enabled. */
      Fls_Errorhook (FLS_SID_ERASE, FLS_E_UNINIT);
      return E_NOT_OK;
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_ERASE, FLS_E_BUSY);
    return E_NOT_OK;
  }

  /* Check Length parameter */
  if ((0 < Length) && (Length <= (PointToConfig_pt->Size - FlsAddress)))
  {
    /*do nothing*/
  }
  else
  {
    Fls_Errorhook (FLS_SID_ERASE, FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }
    
  /* Check Address parameter */
  if (PointToConfig_pt->Size > FlsAddress)
  {
    /*do nothing*/
  }
  else
  {
    Fls_Errorhook (FLS_SID_ERASE, FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */
  
  /* ----- Implementation ----------------------------------------------- */
  /* Set driver status to busy */
  Fls_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Fls_CurrJob_t.JobType = FLS_ERASE_JOB;
  Fls_CurrJob_t.StartAddress = FlsAddress;
  Fls_CurrJob_t.CurrAddress = FlsAddress;
  Fls_CurrJob_t.CurrByte = 0;
  Fls_CurrJob_t.JobLength = Length;
  /*set job state starts the processing */
  Fls_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Fls_Erase() */


/**********************************************************************************************************************
 *  Fls_Write
 *********************************************************************************************************************/
/*! \brief      Writes one or more complete flash pages. 
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  DataBufferPtr           Pointer to data source buffer in RAM
 *  \param[in]  Length                  Number of bytes to write
 *  \return     E_OK                    Write command has been accepted
 *  \return     E_NOT_OK                Write command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Write
(
  Fls_AddressType FlsAddress,
  Fls_ConstBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    /* Raise development error when this check is enabled. */
    Fls_Errorhook (FLS_SID_WRITE, FLS_E_UNINIT);
    return E_NOT_OK;
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_WRITE, FLS_E_BUSY);
    return E_NOT_OK;
  }

  /* Check parameter DataBufferPtr for referencing NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    Fls_Errorhook (FLS_SID_WRITE, FLS_E_PARAM_DATA);
    return E_NOT_OK;
  }
    
 /*check if the address is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= FlsAddress)
  {
    Fls_Errorhook (FLS_SID_WRITE, FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }

  /* Check parameter length */
  /*length greater than 0...is less equal then flash end address */
  if ((0 < Length) && (Length <= ((*PointToConfig_pt).Size - FlsAddress)))
  {
    /* do nothing */
  }
  else
  {
    Fls_Errorhook (FLS_SID_WRITE, FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Fls_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Fls_CurrJob_t.JobType = FLS_WRITE_JOB;
  Fls_CurrJob_t.StartAddress = FlsAddress;
  Fls_CurrJob_t.CurrAddress = FlsAddress;
  Fls_CurrJob_t.JobLength = Length;
  Fls_CurrJob_t.CurrByte = 0;
  Fls_CurrJob_t.WriteFlsPointer = DataBufferPtr;
  /*set job state starts the processing */
  Fls_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Fls_Write() */


#if (STD_ON == FLS_CANCEL_API)
/**********************************************************************************************************************
 *  Fls_Cancel
 *********************************************************************************************************************/
/*! \brief      This function cancels a read, write or erase Job.
 *  \param      void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_Cancel (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
      /* Raise development error when this check is enabled. */
    Fls_Errorhook (FLS_SID_CANCEL, FLS_E_UNINIT);
    return;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Set driver and job state */
  Fls_Status_t = MEMIF_IDLE;

  if (MEMIF_JOB_PENDING == Fls_JobResult_t)
  {
    Fls_JobResult_t = MEMIF_JOB_CANCELLED;
    /*Calling the Error Notification*/
    if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
    {
      /* call the callback */
      PointToConfig_pt->JobErrorNotif();
    }
  }

  /* Configure the current job */
  Fls_CurrJob_t.JobType = FLS_NO_JOB;

  return;
} /* End of Fls_Cancel() */
#endif


#if (STD_ON == FLS_GET_STATUS_API)
/**********************************************************************************************************************
 *  Fls_GetStatus
 *********************************************************************************************************************/
/*! \brief      This function returns the status of the Fls driver.
 *  \param[in]  void
 *  \return     MemIf_StatusType          Status of the driver
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_StatusType, FLS_CODE) Fls_GetStatus (void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* return the current Flash status */
  return  Fls_Status_t;
} /* End of Fls_GetStatus() */
#endif /* (STD_ON == FLS_GET_STATUS_API) */


#if (STD_ON == FLS_GET_JOB_RESULT_API)
/**********************************************************************************************************************
 *  Fls_GetJobResult
 *********************************************************************************************************************/
/*! \brief      This function returns the result of the last accepted job, as long as no new job
 *  \param[in]  void
 *  \return     MemIf_JobResultType     Result of the last accepted Fls job
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_JobResultType, FLS_CODE) Fls_GetJobResult (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    /* Raise development error when this check is enabled. */
    Fls_Errorhook (FLS_SID_GETJOBRESULT, FLS_E_UNINIT);
    return MEMIF_JOB_FAILED;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* return the result of the last/current job */
  return Fls_JobResult_t;
} /* End of Fls_GetJobResult() */
#endif /* (STD_ON == FLS_GET_JOB_RESULT_API) */


/**********************************************************************************************************************
 *  Fls_Read
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \param[out] DataBufferPtr           Pointer to destination data buffer in RAM
 *  \return     E_OK                    Read command has been accepted
 *  \return     E_NOT_OK                Read command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Read
(
  Fls_AddressType  FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType   Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    /* Raise development error when this check is enabled. */
    Fls_Errorhook (FLS_SID_READ, FLS_E_UNINIT);
    return E_NOT_OK;
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_READ, FLS_E_BUSY);
    return E_NOT_OK;
  }
  
  /* Check the DataBufferPtr for being a NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    Fls_Errorhook (FLS_SID_READ, FLS_E_PARAM_DATA);
    return E_NOT_OK;
  }

  /* check if the address is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= FlsAddress)
  {
    Fls_Errorhook (FLS_SID_READ, FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }

  /* valid length ...length greater than 0... between upper and lower boundary */
  if ((Length <= 0) || ((Length + FlsAddress) > (PointToConfig_pt->Size)))
  {
    Fls_Errorhook (FLS_SID_READ, FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Fls_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Fls_CurrJob_t.JobType = FLS_READ_JOB;
  Fls_CurrJob_t.StartAddress = FlsAddress;
  Fls_CurrJob_t.CurrAddress = FlsAddress;
  Fls_CurrJob_t.CurrByte = 0;
  Fls_CurrJob_t.JobLength = Length;
  Fls_CurrJob_t.ReadFlsPointer = DataBufferPtr;
  
  /* set job state starts the processing */
  Fls_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Fls_Read() */


#if (STD_ON == FLS_COMPARE_API)
/**********************************************************************************************************************
 *  Fls_Compare
 *********************************************************************************************************************/
/*! \brief      Compares the contents of an area of flash memory with that of an application data buffer. 
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  DataBufferPtr           Pointer to data buffer that contains the compare data
 *  \param[in]  Length                  Number of bytes to compare
 *  \return     E_OK                    Compare command has been accepted
 *  \return     E_NOT_OK                Compare command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Compare
(
  Fls_AddressType    FlsAddress,
  Fls_ConstBytePtrType DataBufferPtr,
  Fls_LengthType     Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_COMPARE, FLS_E_UNINIT);
    return E_NOT_OK;
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_COMPARE, FLS_E_BUSY);
    return E_NOT_OK;
  }

  /* Check DataBufferPtr for not referencing NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    Fls_Errorhook (FLS_SID_COMPARE, FLS_E_PARAM_DATA);
    return E_NOT_OK;
  }

  /* Check if the address is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= FlsAddress)
  {
    Fls_Errorhook (FLS_SID_COMPARE, FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }

  /*valid length ...length greater than 0... between upper and lower boundary*/
  if ((0 < Length) && (Length <= (PointToConfig_pt->Size - FlsAddress)))
  {
    /* Nothing to do */
  }
  else
  {
    Fls_Errorhook (FLS_SID_COMPARE, FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Fls_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Fls_CurrJob_t.JobType = FLS_COMPARE_JOB;
  Fls_CurrJob_t.StartAddress = FlsAddress;
  Fls_CurrJob_t.CurrAddress = FlsAddress;
  Fls_CurrJob_t.CurrByte = 0;
  Fls_CurrJob_t.JobLength = Length;
  Fls_CurrJob_t.WriteFlsPointer = DataBufferPtr;
  
  /*set job state starts the processing */
  Fls_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Fls_Compare() */
#endif /* (STD_ON == FLS_COMPARE_API) */


#if (STD_ON == FLS_SET_MODE_API)
/**********************************************************************************************************************
 *  Fls_SetMode
 *********************************************************************************************************************/
/*! \brief      Function used to switch between normal and fast Fls access.
 *  \param[in]  Mode                    Flash operation mode (normal/fast access)
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_SetMode (MemIf_ModeType Mode)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Check the Mode for being either MEMIF_MODE_FAST or MEMIF_MODE_SLOW */
  if ((MEMIF_MODE_FAST != Mode) && (MEMIF_MODE_SLOW != Mode))
  {
    Fls_Errorhook (FLS_SID_SETMODE, FLS_E_PARAM_DATA);
    return;
  }

  /* Is the driver initialized */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_SETMODE, FLS_E_UNINIT);
    return;
  }

  /* Is the driver initialized or currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_SETMODE, FLS_E_BUSY);
    return;
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  if (MEMIF_UNINIT != Fls_Status_t )
  {
    Fls_ReadMode_t = Mode; /* set new read mode */
  }
  else
  {
    /* nothing to do */
  }
  
  return;
} /* End of Fls_SetMode() */
#endif /* (STD_ON == FLS_SET_MODE_API) */


#if (STD_ON == FLS_VERSION_INFO_API)
/**********************************************************************************************************************
 *  Fls_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This function provides the module version information.
 *              The version information includes:
 *                - Module Id
 *                - Vendor Id
 *                - Vendor specific version numbers
 *  \param[out] versioninfo             Pointer to where to store the version information of this module.
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, FLS_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
    if (NULL_PTR != versioninfo)
    {
      /* ConfigPtr is ok, go on with initialization */
    }
    else
    {
      Fls_Errorhook (FLS_SID_GETVERSIONINFO, FLS_E_PARAM_VINFO);
      return;
    }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  versioninfo->vendorID = (FLS_VENDOR_ID);
  versioninfo->moduleID = (FLS_MODULE_ID);
  versioninfo->sw_major_version = (FLS_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (FLS_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (FLS_SW_PATCH_VERSION);

  return;
} /* End of Fls_GetVersionInfo() */
#endif  /* (FLS_VERSION_INFO_API == ON) */


#if (STD_ON == FLS_COPY_API)
/**********************************************************************************************************************
 *  Fls_Copy
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls and copies block to Fls again
 *  \param[in]  TargetAddress           TargetAddress offset in Fls
 *  \param[in]  SourceAddress           SourceAddress offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \return     E_OK                    Copy command has been accepted
 *  \return     E_NOT_OK                Copy command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Copy
(
  Fls_AddressType TargetAddress,
  Fls_AddressType SourceAddress,
  Fls_LengthType Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    /* Raise development error when this check is enabled. */
    Fls_Errorhook (FLS_SID_COPY, FLS_E_UNINIT);
    return (E_NOT_OK);
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_COPY, FLS_E_BUSY);
    return (E_NOT_OK);
  }

  /* check if the Targetaddress is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= TargetAddress)
  {
    Fls_Errorhook (FLS_SID_COPY, FLS_E_PARAM_ADDRESS);
    return (E_NOT_OK);
  }

  /* check if the Sourceaddress is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= SourceAddress)
  {
    Fls_Errorhook (FLS_SID_COPY, FLS_E_PARAM_ADDRESS);
    return (E_NOT_OK);
  }


  /* valid length ...length greater than 0... between upper and lower boundary */
  if ((Length <= 0) || ((Length + TargetAddress) > (PointToConfig_pt->Size)))
  {
    Fls_Errorhook (FLS_SID_COPY, FLS_E_PARAM_LENGTH);
    return (E_NOT_OK);
  }
  if ((Length <= 0) || ((Length + SourceAddress) > (PointToConfig_pt->Size)))
  {
    Fls_Errorhook (FLS_SID_COPY, FLS_E_PARAM_LENGTH);
    return (E_NOT_OK);
  }

  /* Overlap Sourceaddress and Targetaddress? */
  if (TargetAddress < SourceAddress)
  {
    if (TargetAddress + Length > SourceAddress)
    {
      Fls_Errorhook (FLS_SID_COPY, FLS_E_ADDRESS_OVERLAP);
      return (E_NOT_OK);
    }
  }
  else
  {
    if (SourceAddress + Length > TargetAddress)
    {
      Fls_Errorhook (FLS_SID_COPY, FLS_E_ADDRESS_OVERLAP);
      return (E_NOT_OK);
    }
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Fls_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Fls_CurrJob_t.JobType = FLS_COPY_JOB;
  Fls_CurrJob_t.StartAddress = SourceAddress;
  Fls_CurrJob_t.CurrAddress = TargetAddress;
  Fls_CurrJob_t.CurrByte = 0;
  Fls_CurrJob_t.JobLength = Length;

  /*set job state starts the processing */
  Fls_JobResult_t = MEMIF_JOB_PENDING;

return (E_OK);
} /* End of Fls_Copy() */
#endif /* (STD_ON == FLS_COPY_API) */


#if (STD_ON == FLS_READSYNC_API)
/**********************************************************************************************************************
 *  Fls_ReadSync
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls synchronous
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \param[out] DataBufferPtr           Pointer to destination data buffer in RAM
 *  \return     E_OK                    Read command has been accepted
 *  \return     E_NOT_OK                Read command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_JobResultType, FLS_CODE) Fls_ReadSync
(
  Fls_AddressType FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Value;
  uint16 Index;
  Fls_LengthType BlockSize;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_READ_SYNC, FLS_E_UNINIT);
    return (MEMIF_JOB_FAILED);
  }

  /* Is the flash currently busy */
  if (MEMIF_BUSY == Fls_Status_t)
  {
    Fls_Errorhook (FLS_SID_READ_SYNC, FLS_E_BUSY);
    return (MEMIF_JOB_FAILED);
  }

  /* Check the DataBufferPtr for being a NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    Fls_Errorhook (FLS_SID_READ_SYNC, FLS_E_PARAM_DATA);
    return (MEMIF_JOB_FAILED);
  }

  /* valid length ...length greater than 0... between upper and lower boundary */
  if ((Length <= 0) || ((Length + FlsAddress) > (PointToConfig_pt->Size)))
  {
    Fls_Errorhook (FLS_SID_READ_SYNC, FLS_E_PARAM_LENGTH);
    return (MEMIF_JOB_FAILED);
  }

  /*check if the address is between ROM start and end address */
  if ((PointToConfig_pt->Size) <= FlsAddress)
  {
    Fls_Errorhook (FLS_SID_READ_SYNC, FLS_E_PARAM_ADDRESS);
    return (MEMIF_JOB_FAILED);
  }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
  {
    BlockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
  }
  else
  {
    BlockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
  }

  /* Still bytes to read? */
  for (Index = 0; ((Index < BlockSize) && ((Index) < (Length))); Index++)
  {
    /* Read 1 Bytes into Value from Filehandler */
    Value= VttFls_Read (FlsAddress + Index);

    /* Save the value  */
    (DataBufferPtr)[Index] = Value;
  }


  /* is the read complete? */
  if (Index == Length)
  {
    /* is a end process callback aligned */
    if (NULL_PTR != PointToConfig_pt->JobEndNotif)
    {
    /* call the callback */
    PointToConfig_pt->JobEndNotif();
    }
    return (MEMIF_JOB_OK);
  }
  else
  {
    /* is a error process callback aligned */
    if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
    {
    /* call the callback */
    PointToConfig_pt->JobErrorNotif();
    }

    return (MEMIF_JOB_FAILED);
  }
} /* End of Fls_ReadSync() */
#endif /* (STD_ON == FLS_READSYNC_API) */


/**********************************************************************************************************************
 *  Fls_SimulateError
 *********************************************************************************************************************/
/*! \brief      This function is used to simulate an error in the currently processed job
 *  \param[in]  void
 *  \return     E_OK                    An error was simulated
 *  \return     E_NOT_OK                No error could be simulated
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_SimulateError (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ---------- Implementation ------------------------------------------ */
  /* Is a job pending that could be ended with a simulated error? */
  if (MEMIF_JOB_PENDING == Fls_JobResult_t)
  {
    switch (Fls_CurrJob_t.JobType)
    {
      case FLS_READ_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;

        /* is a error callback aligned? */
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_WRITE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned ? */
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_ERASE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned? */
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_COMPARE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned? */
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }
      default:
      {
        /* Nothing to do in here */
        RetVal = E_NOT_OK;
      }
    }
  }
  else
  {
    /* There's nothing that could cause an error*/
    RetVal = E_NOT_OK;
  }
  return RetVal;
} /* End of Fls_SimulateError() */

/**********************************************************************************************************************
 *  Fls_MainFunction
 *********************************************************************************************************************/
/*! \brief      This function is cyclically called and processes the Fls Jobs read, write, erase and compare
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \context    Function could be called from task level (cyclically)
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_MainFunction (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 Index = 0;

/* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == FLS_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Fls_Status_t)
    {
      Fls_Errorhook (FLS_SID_MAINFUNCTION, FLS_E_UNINIT);
      return;
    }
#endif /* (STD_ON == FLS_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* Is currently a job to be processed? */
  if (MEMIF_JOB_PENDING == Fls_JobResult_t)
  {
    /* Which job type is to be processed? */
    switch (Fls_CurrJob_t.JobType)
    {
      /* a read job is to process ---------------------------------------------------------------------------------- */
      case FLS_READ_JOB:
      {
        Fls_LengthType BlockSize;

        if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
        {
          BlockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
        }
        else
        {
          BlockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
        }

    
        /* Still bytes to read? */
        for (Index = 0; ((Index < BlockSize) && ((Fls_CurrJob_t.CurrByte) < (Fls_CurrJob_t.JobLength))); Index++)
        {

          /* Save the value  */
          (Fls_CurrJob_t.ReadFlsPointer)[Fls_CurrJob_t.CurrByte] = VttFls_Read (Fls_CurrJob_t.StartAddress 
                                                                                             + Fls_CurrJob_t.CurrByte);

          /* increment byte counter */
          Fls_CurrJob_t.CurrByte++;
        }


        /* is the read complete? */
        if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
        {
          Fls_Status_t = MEMIF_IDLE;
          Fls_JobResult_t = MEMIF_JOB_OK;

          /* is a end process callback aligned */
          if (NULL_PTR != PointToConfig_pt->JobEndNotif)
          {
            /* call the callback */
            PointToConfig_pt->JobEndNotif();
          }
        }
        break;
      }
      /* end read job -----------------------------------------------------------------------------------------------*/

      /* a write job is to process ----------------------------------------------------------------------------------*/
      case FLS_WRITE_JOB:
      {
        uint8 Value;
        Fls_LengthType BlockSize;
        
        if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
        {
          BlockSize = (PointToConfig_pt->NormalWriteBlockSize); /* normal write mode selected */
        }
        else
        {
          BlockSize = (PointToConfig_pt->FastWriteBlockSize); /* fast write mode selected */
        }
        
        /* still bytes to write? */
        for (Index = 0; ((Index < BlockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength)); Index++)
        {
     
          Value = Fls_CurrJob_t.WriteFlsPointer[Fls_CurrJob_t.CurrByte];
          VttFls_Write (Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte,Value);

          /* increment byte counter */
          Fls_CurrJob_t.CurrByte++;

        }

        /* write complete? */
        if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
        {
          Fls_Status_t = MEMIF_IDLE;
          Fls_JobResult_t = MEMIF_JOB_OK;

          /* is a end process callback aligned */
          if (NULL_PTR != PointToConfig_pt->JobEndNotif)
          {
            /* call the callback */
            PointToConfig_pt->JobEndNotif();
          }
        }
        break;
      }
      /* end write job --------------------------------------------------------------------------------------------- */

      /* a erase job is to process --------------------------------------------------------------------------------- */
      case FLS_ERASE_JOB:
      {
        /* still bytes to erase? */
        for (Index = 0; ((Index < FLS_ERASE_UNIT_SIZE) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength)); Index++)
        {
          VttFls_Write (Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte,FLS_ERASE_VALUE);

          /* increment byte counter */
          Fls_CurrJob_t.CurrByte++;
        }

        /* erase complete? */
        if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
        {
          Fls_Status_t = MEMIF_IDLE;
          Fls_JobResult_t = MEMIF_JOB_OK;

          /* is a end process callback aligned */
          if (NULL_PTR != PointToConfig_pt->JobEndNotif)
          {
            /* call the callback */
            PointToConfig_pt->JobEndNotif();
          }
        }
        break;
      }
      /* end erase job --------------------------------------------------------------------------------------------- */

      /* a compare job is to process ------------------------------------------------------------------------------- */
      case FLS_COMPARE_JOB:
      {
        Fls_LengthType BlockSize;
        uint8 Value;
        uint8 compare_u8 = TRUE;

        if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
        {
          BlockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
        }
        else
        {
          BlockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
        }

     
        /* still bytes to compare? */
        for (Index = 0; ((Index < BlockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength) &&
            (TRUE == compare_u8)); Index++)
        {
          /* Read a Byte into Value from Filehandler */
          Value = VttFls_Read (Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte);

          if (((Fls_CurrJob_t.WriteFlsPointer)[Fls_CurrJob_t.CurrByte]) != Value)
          {
            compare_u8 = FALSE; /* not equal -> break */

            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_BLOCK_INCONSISTENT/*MEMIF_COMPARE_UNEQUAL*/;

            /* is a error callback aligned */
            if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
            {
              /* call the callback */
              PointToConfig_pt->JobErrorNotif();
              break;
            }
          }
          else
          {
            /* increment byte counter */
            Fls_CurrJob_t.CurrByte++;
          }

        } /* end loop */


        /* compare complete? */
        if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
        {
          Fls_Status_t = MEMIF_IDLE;
          Fls_JobResult_t = MEMIF_JOB_OK;

          /* is a end process callback aligned */
          if (NULL_PTR != PointToConfig_pt->JobEndNotif)
          {
            /* call the callback */
            PointToConfig_pt->JobEndNotif();
          }
        }
        break;
      }
      
      /* A copy job is to process ---------------------------------------------------------------------------------- */
      case FLS_COPY_JOB:
      {
        uint8 TmpValue;
        Fls_LengthType BlockSize;
        
        if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
        {
          BlockSize = (PointToConfig_pt->NormalWriteBlockSize); /* normal write mode selected */
        }
        else
        {
          BlockSize = (PointToConfig_pt->FastWriteBlockSize); /* fast write mode selected */
        }
   
         /* still bytes to copy? */
        for (Index = 0; ((Index < BlockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength)); Index++)
        {

          TmpValue = VttFls_Read (Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte);
          VttFls_Write (Fls_CurrJob_t.CurrAddress + Fls_CurrJob_t.CurrByte,TmpValue);
     
          /* increment byte counter */
          Fls_CurrJob_t.CurrByte++;

        }

        /* is the copy complete? */
        if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
        {
          Fls_Status_t = MEMIF_IDLE;
          Fls_JobResult_t = MEMIF_JOB_OK;

          /* is a end process callback aligned */
          if (NULL_PTR != PointToConfig_pt->JobEndNotif)
          {
            /* call the callback */
            PointToConfig_pt->JobEndNotif();
          }
        }
        break;
      }
      /* end compare job ------------------------------------------------------------------------------------------- */

      default:
      {
        /* Shall never be reached */
        break;
      }
    } /* end switch job type */
  }

  return;
} /* End of Fls_MainFunction() */


/**********************************************************************************************************************
 *  END OF FILE: Fls.c
 *********************************************************************************************************************/
