/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Eep.c
 *      Project:  DrvEep_VttuCanoe01Asr
 *       Module:  MICROSAR Eep
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Eep implements a EEPROM driver for the CANoe Emulation.
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
 **********************************************************************************************************************/

#define EEP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eep.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of Eep header file */
#if (  (EEP_SW_MAJOR_VERSION != (0x02u)) \
    || (EEP_SW_MINOR_VERSION != (0x04u)) \
    || (EEP_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (EEP_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (EEP_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Eep.c and Eep.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (EEP_CFG_MAJOR_VERSION != (0x03u)) \
    || (EEP_CFG_MINOR_VERSION != (0x03u)) )
  #error "Version numbers of Eep.c and Eep_Cfg.h are inconsistent!"
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
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/* Possible Jobs */
typedef enum
{
  EEP_NO_JOB    = 0,
  EEP_READ_JOB,
  EEP_WRITE_JOB,
  EEP_ERASE_JOB,
  EEP_COMPARE_JOB
} Eep_JobType;

/* Possible settings of a job */
typedef struct
{
  Eep_JobType         JobType;
  Eep_AddressType     StartAddress;
  Eep_AddressType     CurrAddress;
  Eep_LengthType      JobLength;
  uint32              CurrByte;
  const uint8*        WriteEepPointer;
  uint8*              ReadEepPointer;
} Eep_Job_Struct;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
/* Eep_Status_t holds the status of the Eep */
STATIC VAR(MemIf_StatusType, EEP_VAR)    Eep_Status_t    = MEMIF_UNINIT;
/*  Eep_JobResult_t holds the result of the last job */
STATIC VAR(MemIf_JobResultType, EEP_VAR) Eep_JobResult_t = MEMIF_JOB_OK;
/* Eep_ReadMode_t holds the current mode (normal/fast) */
STATIC VAR(MemIf_ModeType, EEP_VAR)      Eep_ReadMode_t  = MEMIF_MODE_SLOW;
/* PointToConfig_pt holds Pointer to the configuration structure */
STATIC P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_CONST) PointToConfig_pt = NULL_PTR;
/* Eep_CurrJob_t struct holds status of the current job */
STATIC VAR(Eep_Job_Struct, EEP_VAR)      Eep_CurrJob_t   = {EEP_NO_JOB, 0, 0, 0, 0, NULL_PTR, NULL_PTR};

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
 *  Eep_InitMemory
 **********************************************************************************************************************/
/*! \brief      This service initializes the global variables in case the startup code does not work
 *  \param[in]  void
 *  \return     void
 *  \pre        Module must not be initialized
 *  \context    Function is called during startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Initialize all module-global variables */
  Eep_Status_t      = MEMIF_UNINIT;               /* status of the Eep */
  Eep_JobResult_t   = MEMIF_JOB_OK;               /* result of the last job */
  Eep_ReadMode_t    = MEMIF_MODE_SLOW;            /* Currrent mode (norma/fast) */
  PointToConfig_pt  = NULL_PTR;  
  
  /* Init the struct that holds status of the current job */
  Eep_CurrJob_t.JobType = EEP_NO_JOB;
  Eep_CurrJob_t.StartAddress = 0;
  Eep_CurrJob_t.CurrAddress = 0;
  Eep_CurrJob_t.JobLength = 0;
  Eep_CurrJob_t.CurrByte = 0;
  Eep_CurrJob_t.WriteEepPointer = NULL_PTR;
  Eep_CurrJob_t.ReadEepPointer = NULL_PTR;
  
  return;
} /* End of Eep_InitMemory */


/**********************************************************************************************************************
 *  Eep_Init
 **********************************************************************************************************************/
/*! \brief      Service for EEPROM initialization. 
 *  \param[in]  ConfigPtr                      Input parameter
 *  \return     void
 *  \pre        Input parameter must not be NULL.
 *  \context    Function is called in EcuM context
 *  \note
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_Init_Vtt (P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /*is the eeprom currently busy */
  if (MEMIF_BUSY == Eep_Status_t)
  {          
    Eep_Errorhook (EEP_SID_INIT, EEP_E_BUSY);            
    return;
  }

  /*is there a valid configuration */
  if (NULL_PTR == ConfigPtr)
  {      
    Eep_Errorhook (EEP_SID_INIT, EEP_E_PARAM_CONFIG);            
    return;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ----- Implementation ----------------------------------------------- */
  /* Initialization of the emulated hardware (<Filename>.nvram) is done in in VttEep_Onload() or VttFls_OnLoad() */

  /* set the status flags and set set config ptr */
  Eep_Status_t = MEMIF_IDLE;
  Eep_JobResult_t = MEMIF_JOB_OK;
  PointToConfig_pt = ConfigPtr;              /* set the local configuration pointer to the Eep config struct        */
                                             /* now the data of the config should be available inside the module  */

  /* store the Default mode in a static global var */
  Eep_ReadMode_t = PointToConfig_pt->DefaultMode;

  return;
} /* End of Eep_Init() */

/**********************************************************************************************************************
 *  Eep_SetMode
 **********************************************************************************************************************/
/*! \brief      Service used to switch between normal and fast Eep access
 *  \param[in]  Mode                           Mode, the EEPROM operation mode has to be set 
 *  \return     void
 *  \pre        The module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_SetMode (MemIf_ModeType Mode)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /* check EEP_CFG_CHK_PARAM_DATA */
  if ((MEMIF_MODE_FAST != Mode) && (MEMIF_MODE_SLOW != Mode))
  {          
    Eep_Errorhook (EEP_SID_SETMODE, EEP_E_PARAM_DATA);            
    return;
  }

  /* check EEP_CFG_CHK_UNINIT */
  if (MEMIF_BUSY == Eep_Status_t)
  {          
    Eep_Errorhook (EEP_SID_SETMODE, EEP_E_BUSY);            
    return;
  }

  /* check EEP_CFG_CHK_BUSY */
  if (MEMIF_UNINIT == Eep_Status_t)
  {
    Eep_Errorhook (EEP_SID_SETMODE, EEP_E_UNINIT);
    return;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  if (MEMIF_IDLE == Eep_Status_t)
  {
    Eep_ReadMode_t = Mode;        /* set new read mode */
  }   /* no else needed */
  return;
} /* End of Eep_SetMode() */


/**********************************************************************************************************************
 *  Eep_Read
 **********************************************************************************************************************/
/*! \brief      Service reads a data block from the Eeprom
 *  \param[in]  EepAddress                     Address offset in Eeprom (input)
 *  \param[in]  DataBufferPtr                  Pointer to destination data buffer in ram (output parameter)
 *  \param[in]  Length                         Number of bytes to read (input)
 *  \return     Std_ReturnType
 *  \pre        The module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, EEP_CODE) Eep_Read
(
  Eep_AddressType  EepAddress,
  Eep_VarBytePtrType DataBufferPtr,
  Eep_LengthType   Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /* check EEP_CFG_CHK_UNINIT */
  if  (MEMIF_UNINIT == Eep_Status_t)
  {          
    Eep_Errorhook (EEP_SID_READ, EEP_E_UNINIT);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_PARAM_DATA */
  if (NULL_PTR == DataBufferPtr)
  {           
    Eep_Errorhook (EEP_SID_READ, EEP_E_PARAM_DATA);
    return E_NOT_OK;
  }

  /* check if the address is between ROM start and end address
     EepAddress can not get smaller than 0, its range is [0;65535]
  */
  if (EepAddress >= PointToConfig_pt->Size - 1) 
  {
    Eep_Errorhook (EEP_SID_READ, EEP_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }

  /*valid length ...length greater than 0... between upper and lower boundary*/
  if ( (Length <= 0) || (Length > (PointToConfig_pt->Size - EepAddress)) )
  {        
    Eep_Errorhook (EEP_SID_READ, EEP_E_PARAM_LENGTH);            
    return E_NOT_OK;
  }

  if (Eep_Status_t != MEMIF_BUSY)
  {
    /*do nothing*/
  }
  else
  {            
    Eep_Errorhook (EEP_SID_READ, EEP_E_BUSY);            
    return E_NOT_OK;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Eep_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Eep_CurrJob_t.JobType = EEP_READ_JOB;
  Eep_CurrJob_t.StartAddress = EepAddress;
  Eep_CurrJob_t.CurrAddress = EepAddress;
  Eep_CurrJob_t.CurrByte = 0;
  Eep_CurrJob_t.JobLength = Length;
  Eep_CurrJob_t.ReadEepPointer = DataBufferPtr;
  /*set job state starts the processing */
  Eep_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Eep_Read() */


/**********************************************************************************************************************
 *  Eep_Write
 **********************************************************************************************************************/
/*! \brief      Service writes a data block to the Eeprom
 *  \param[in]  EepAddress                     Address offset in Eeprom (input)
 *  \param[in]  DataBufferPtr                  Pointer to data source buffer in ram (input parameter)
 *  \param[in]  Length                         Number of bytes to write (input)
 *  \return     Std_ReturnType
 *  \pre        The module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, EEP_CODE) Eep_Write
(
  Eep_AddressType    EepAddress,
  Eep_ConstBytePtrType DataBufferPtr,
  Eep_LengthType     Length
)
{
/* ----- Development Error Checks ------------------------------------- */
  #if (STD_ON == EEP_DEV_ERROR_DETECT)
  /* check EEP_CFG_CHK_UNINITIs */
  if (MEMIF_UNINIT == Eep_Status_t)
  {         
    Eep_Errorhook (EEP_SID_WRITE, EEP_E_UNINIT);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_PARAM_DATA */
  if (NULL_PTR == DataBufferPtr)
  {
    Eep_Errorhook (EEP_SID_WRITE, EEP_E_PARAM_DATA);
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_PARAM_ADDRESS 
     EepAddress can not get smaller than 0, its range is [0;65535]
  */
  if (EepAddress >= PointToConfig_pt->Size - 1) 
  {
    Eep_Errorhook (EEP_SID_WRITE, EEP_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }

  /*check if length greater than 0...is less equal then eeprom end address */
  if ( (Length <= 0) || (Length > (PointToConfig_pt->Size - EepAddress)) )
  {
    Eep_Errorhook (EEP_SID_WRITE, EEP_E_PARAM_LENGTH);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_BUSY */
  if (MEMIF_BUSY == Eep_Status_t)
  {
    Eep_Errorhook (EEP_SID_WRITE, EEP_E_BUSY);            
    return E_NOT_OK;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Eep_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Eep_CurrJob_t.JobType = EEP_WRITE_JOB;
  Eep_CurrJob_t.StartAddress = EepAddress;
  Eep_CurrJob_t.CurrAddress = EepAddress;
  Eep_CurrJob_t.JobLength = Length;
  Eep_CurrJob_t.CurrByte = 0;
  Eep_CurrJob_t.WriteEepPointer = DataBufferPtr;
  /*set job state starts the processing */
  Eep_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;

} /* End of Eep_Write() */


/**********************************************************************************************************************
 *  Eep_Erase
 **********************************************************************************************************************/
/*! \brief      Service erases a data block of the Eep
 *  \param[in]  EepAddress                     Address offset in Eeprom (input)
 *  \param[in]  Length                         Number of bytes to write (input)
 *  \return     Std_ReturnType
 *  \pre        The module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, EEP_CODE) Eep_Erase
(
  Eep_AddressType  EepAddress,
  Eep_LengthType   Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
    /* check EEP_CFG_CHK_UNINIT */
  if (MEMIF_UNINIT == Eep_Status_t)
  {          
    Eep_Errorhook (EEP_SID_ERASE, EEP_E_UNINIT);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_PARAM_ADDRESS 
     EepAddress can not get smaller than 0, its range is [0;65535]
  */
  if (EepAddress >= PointToConfig_pt->Size - 1)
  {            
    Eep_Errorhook (EEP_SID_ERASE, EEP_E_PARAM_ADDRESS);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_PARAM_LENGTH */
  if ( (Length <= 0) || (Length > (PointToConfig_pt->Size - EepAddress)) )
  {           
    Eep_Errorhook (EEP_SID_ERASE, EEP_E_PARAM_LENGTH);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_BUSY */
  if (Eep_Status_t == MEMIF_BUSY)
  {       
    Eep_Errorhook (EEP_SID_ERASE, EEP_E_BUSY);            
    return E_NOT_OK;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* set driver */
  Eep_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Eep_CurrJob_t.JobType = EEP_ERASE_JOB;
  Eep_CurrJob_t.StartAddress = EepAddress;
  Eep_CurrJob_t.CurrAddress = EepAddress;
  Eep_CurrJob_t.CurrByte = 0;
  Eep_CurrJob_t.JobLength = Length;
  /*set job state starts the processing */
  Eep_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Eep_Erase() */


/**********************************************************************************************************************
 *  Eep_Compare
 **********************************************************************************************************************/
/*! \brief      Service compares an Eep data block with a data block in the ram
 *  \param[in]  EepAddress                     Address offset in Eeprom (input)
 *  \param[in]  DataBufferPtr                  Pointer to data buffer in ram that 
 *                                             contains the compare data (input parameter)
 *  \param[in]  Length                         Number of bytes to write (input)
 *  \return     Std_ReturnType
 *  \pre        The module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, EEP_CODE) Eep_Compare
(
  Eep_AddressType    EepAddress,
  Eep_ConstBytePtrType DataBufferPtr,
  Eep_LengthType     Length
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /* check EEP_CFG_CHK_UNINIT */
  if (MEMIF_UNINIT == Eep_Status_t)
  {     
    Eep_Errorhook (EEP_SID_COMPARE, EEP_E_UNINIT);            
    return E_NOT_OK;
  }

  /* check if the address is between ROM start and end address
     EepAddress can not get smaller than 0, its range is [0;65535]
  */
  if (EepAddress >= PointToConfig_pt->Size - 1)
  {     
    Eep_Errorhook (EEP_SID_COMPARE, EEP_E_PARAM_ADDRESS);            
    return E_NOT_OK;
  }

  /* EEP_CFG_CHK_PARAM_DATA */
  if (NULL_PTR == DataBufferPtr)
  {      
    Eep_Errorhook (EEP_SID_COMPARE, EEP_E_PARAM_DATA);            
    return E_NOT_OK;
  }

  /* valid length ...length greater than 0... between upper and lower boundary*/
  if ( (Length <= 0) || (Length > (PointToConfig_pt->Size - EepAddress)) )
  {          
    Eep_Errorhook (EEP_SID_COMPARE, EEP_E_PARAM_LENGTH);            
    return E_NOT_OK;
  }

  /* check EEP_CFG_CHK_BUSY */
  if (Eep_Status_t == MEMIF_BUSY)
  {
    Eep_Errorhook (EEP_SID_COMPARE, EEP_E_BUSY);            
    return E_NOT_OK;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* set driver state */
  Eep_Status_t = MEMIF_BUSY;

  /* configure the current job */
  Eep_CurrJob_t.JobType = EEP_COMPARE_JOB;
  Eep_CurrJob_t.StartAddress = EepAddress;
  Eep_CurrJob_t.CurrAddress = EepAddress;
  Eep_CurrJob_t.CurrByte = 0;
  Eep_CurrJob_t.JobLength = Length;
  Eep_CurrJob_t.WriteEepPointer = DataBufferPtr;
  /*set job state starts the processing */
  Eep_JobResult_t = MEMIF_JOB_PENDING;

  return E_OK;
} /* End of Eep_Compare() */


/**********************************************************************************************************************
 *  Eep_Cancel
 **********************************************************************************************************************/
/*! \brief      Service cancels a read, write or erase Job, if running
 *  \param[in]  void
 *  \return     void
 *  \pre        
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_Cancel (void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* set driver and job state */
  Eep_Status_t = MEMIF_IDLE;

  if (MEMIF_JOB_PENDING == Eep_JobResult_t)
  {
    Eep_JobResult_t = MEMIF_JOB_CANCELLED;
    
    /*Calling the Error Notification*/
    if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
    {
      /* call the callback */
      PointToConfig_pt->JobErrorNotif();
    }
  }

  /* configure the current job */
  Eep_CurrJob_t.JobType = EEP_NO_JOB;

  return;
} /* End of Eep_Cancel() */


/**********************************************************************************************************************
 *  Eep_GetStatus
 **********************************************************************************************************************/
/*! \brief      Service returns the status of the Eeprom driver
 *  \param[in]  void
 *  \return     MemIf_StatusType          status of the driver
 *  \pre        
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(MemIf_StatusType, EEP_CODE) Eep_GetStatus (void)
{
  /* ---------- Implementation ------------------------------------------ */
  return  Eep_Status_t;      /* return the current Eeprom status */
} /* End of Eep_GetStatus() */


/**********************************************************************************************************************
 *  Eep_GetJobResult
 **********************************************************************************************************************/
/*! \brief      Service returns the result of the last accepted job, as long as no new job is accepted.
 *  \param[in]  void
 *  \return     MemIf_JobResultType       result of the last accepted Eeprom Job
 *  \pre        
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(MemIf_JobResultType, EEP_CODE) Eep_GetJobResult (void)
{
  /* ---------- Implementation ------------------------------------------ */
  return Eep_JobResult_t;   /* return the result of the last/current Job */
} /* End of Eep_GetJobResult() */


#if (STD_ON == EEP_VERSION_INFO_API)
/**********************************************************************************************************************
 * Eep_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      Version Information Service
 *
 *              This service writes the version information of this module into the data structure referenced by the
 *              parameter. If this parameter references NULL_PTR, a development error is reported to Det. This service
 *              is only available if the pre-compile configuration parameter ADC_VERSION_INFO_API is enabled.
 *
 *  \param[out] versioninfo         data structure where the version info shall be written to
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /*check EEP_CFG_CHK_PARAM_VINFO */
  if (NULL_PTR == versioninfo)
  {
    Eep_Errorhook (EEP_SID_GETVERSIONINFO, EEP_E_PARAM_VINFO);            
    return;    /* ConfigPtr is ok, go on with initialization */
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  versioninfo->vendorID         = (EEP_VENDOR_ID);
  versioninfo->moduleID         = (EEP_MODULE_ID);
  versioninfo->sw_major_version = (EEP_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (EEP_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (EEP_SW_PATCH_VERSION);

  return;
} /* End Eep_GetVersionInfo()                                                 */
#endif  /* (STD_ON == EEP_VERSION_INFO_API)                                     */


/**********************************************************************************************************************
 *  Eep_MainFunction
 **********************************************************************************************************************/
/*! \brief      Service to perform the processing of the EEPROM jobs (read/write/erase/compare). 
 *  \param[in]  void
 *  \return     void
 *  \pre        
 *  \context    Function is called cyclically (e.g. SchM)
 *  \note
 **********************************************************************************************************************/
FUNC(void, EEP_CODE) Eep_MainFunction (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 Index = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == EEP_DEV_ERROR_DETECT)
  /* check EEP_CFG_CHK_UNINIT */
  if (MEMIF_UNINIT == Eep_Status_t)
  {
    Eep_Errorhook (EEP_SID_MAINFUNCTION, EEP_E_UNINIT);            
    return;
  }
#endif /* (STD_ON == EEP_DEV_ERROR_DETECT) */
  
  /* ---------- Implementation ------------------------------------------ */
  /* is currently a job to be processed? */
  if (MEMIF_JOB_PENDING == Eep_JobResult_t)
  {
      /* which job type is to be processed? */
      switch (Eep_CurrJob_t.JobType)
      {
        /* a read job is to process ----------------------------------------------------------------------------------*/
        case EEP_READ_JOB :
        {
          MemIf_LengthType BlockSize;
               
          if (MEMIF_MODE_SLOW == Eep_ReadMode_t)
          {
            BlockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
          }else
          {
            BlockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
          }
            
          /* Search the next position in EEPROM file */

          /* still bytes to read? */
          for (Index = 0; ( (Index < BlockSize ) && ( (Eep_CurrJob_t.CurrByte) < (Eep_CurrJob_t.JobLength)) ); Index++)
          {
            /* Save the value  */
            (Eep_CurrJob_t.ReadEepPointer)[Eep_CurrJob_t.CurrByte] = VttEep_Read(Eep_CurrJob_t.StartAddress 
                                                                                              + Eep_CurrJob_t.CurrByte);

            /* increment byte counter */
            Eep_CurrJob_t.CurrByte++;
          }
               
          /* Close the File handle */ 
               
          /* is the read complete? */
          if (Eep_CurrJob_t.CurrByte == Eep_CurrJob_t.JobLength)
          {
            Eep_Status_t = MEMIF_IDLE;
            Eep_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (NULL_PTR != PointToConfig_pt->JobEndNotif)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }

          break;
        }
        /*                                                                                                            */
        /* end read job ----------------------------------------------------------------------------------------------*/


        /* a write job is to process ---------------------------------------------------------------------------------*/
        /*                                                                                                            */
        case  EEP_WRITE_JOB:
        {           
          uint8 Value;
          MemIf_LengthType BlockSize;
               
          if (MEMIF_MODE_SLOW == Eep_ReadMode_t)
          {
            BlockSize = (PointToConfig_pt->NormalWriteBlockSize); /* normal write mode selected */
          }else
          {
            BlockSize = (PointToConfig_pt->FastWriteBlockSize); /* fast write mode selected */
          }
               
          /* still bytes to write? */
          for (Index = 0; ( (Index < BlockSize ) && (Eep_CurrJob_t.CurrByte < Eep_CurrJob_t.JobLength) ); Index++)
          {
            /* Search the next position in Eeprom file */
            Value = Eep_CurrJob_t.WriteEepPointer[Eep_CurrJob_t.CurrByte];
            VttEep_Write (Eep_CurrJob_t.StartAddress + Eep_CurrJob_t.CurrByte,Value);
      
            /* increment byte counter */
            Eep_CurrJob_t.CurrByte++;
          }

          /* write complete? */
          if (Eep_CurrJob_t.CurrByte == Eep_CurrJob_t.JobLength)
          {
            Eep_Status_t = MEMIF_IDLE;
            Eep_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (NULL_PTR != PointToConfig_pt->JobEndNotif)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
  
          break;
        }
        /*                                                                                                            */
        /* end write job ---------------------------------------------------------------------------------------------*/
        

        /* a erase job is to process -------------------------------------------------------------------------------- */
        /*                                                                                                            */
        case  EEP_ERASE_JOB:
        {
 
          /* Search the next position in Eeprom file */

          /* still bytes to erase? */
          for (Index = 0;
              ( (Index < EEP_ERASE_UNIT_SIZE ) && (Eep_CurrJob_t.CurrByte < Eep_CurrJob_t.JobLength) );
              Index++)
          {
            VttEep_Write (Eep_CurrJob_t.StartAddress + Eep_CurrJob_t.CurrByte, EEP_ERASE_VALUE);

            /* increment byte counter */
            Eep_CurrJob_t.CurrByte ++;
          }

          /* erase complete? */
          if (Eep_CurrJob_t.CurrByte == Eep_CurrJob_t.JobLength)
          {
            Eep_Status_t = MEMIF_IDLE;
            Eep_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (NULL_PTR != PointToConfig_pt->JobEndNotif)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }

          break;
        }
        /*                                                                                                            */
        /* end erase job ---------------------------------------------------------------------------------------------*/

        
        /* a compare job is to process -------------------------------------------------------------------------------*/
        /*                                                                                                            */
        case  EEP_COMPARE_JOB:
        {
          MemIf_LengthType BlockSize;
          uint8 Value;
          uint8 compare_u8 = TRUE;

          if (MEMIF_MODE_SLOW == Eep_ReadMode_t)
          {
            BlockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
          }
          else
          {
            BlockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
          }

          /* Search the next position in Eeprom file */

          /* still bytes to compare? */
          for (Index = 0; 
              ( (Index < BlockSize) && (Eep_CurrJob_t.CurrByte < Eep_CurrJob_t.JobLength) && (TRUE == compare_u8) );
              Index++ )
          {       
            /* Read a Byte into Value from Filehandler */
            Value = VttEep_Read (Eep_CurrJob_t.StartAddress + Eep_CurrJob_t.CurrByte);

            if (((Eep_CurrJob_t.WriteEepPointer)[Eep_CurrJob_t.CurrByte] ) != Value)
            {
              compare_u8 = FALSE;             /* not equal -> break */

              Eep_Status_t = MEMIF_IDLE;
              Eep_JobResult_t = MEMIF_BLOCK_INCONSISTENT/*MEMIF_COMPARE_UNEQUAL*/;

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
              Eep_CurrJob_t.CurrByte++;
            }
          }
                
          /* compare complete? */
          if (Eep_CurrJob_t.CurrByte == Eep_CurrJob_t.JobLength)
          {
            Eep_Status_t = MEMIF_IDLE;
            Eep_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (NULL_PTR != PointToConfig_pt->JobEndNotif)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /*                                                                                                            */
        /* end compare job -------------------------------------------------------------------------------------------*/

        
        default:
        {
          /* You shouldn't come round here 'cause no job should be pending! */
          break;
        }
    } /* end switch job type */
  }

  return;
} /* End of Eep_MainFunction */



/**********************************************************************************************************************
 *  Eep_SimulateError
 **********************************************************************************************************************/
/*! \brief      Service is used to simulate an error in the currently processed job
 *  \param[in]  void
 *  \return     Std_ReturnType
 *  \pre        The Module has to be initialized
 *  \context    Function is called in task context
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, EEP_CODE) Eep_SimulateError (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* is a job pending that could be ended with a simulated error? */
  if (MEMIF_JOB_PENDING == Eep_JobResult_t)
  {
    switch (Eep_CurrJob_t.JobType)
    {
      case EEP_READ_JOB :
      {
        Eep_Status_t = MEMIF_IDLE;
        Eep_JobResult_t = MEMIF_JOB_FAILED;

        /* is a error callback aligned ?*/
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case EEP_WRITE_JOB :
      {
        Eep_Status_t = MEMIF_IDLE;
        Eep_JobResult_t = MEMIF_JOB_FAILED;
        
        /* is a error callback aligned ?*/
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case EEP_ERASE_JOB :
      {
        Eep_Status_t = MEMIF_IDLE;
        Eep_JobResult_t = MEMIF_JOB_FAILED;
        
        /* is a error callback aligned ?*/
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
              /* call the callback */
              PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case EEP_COMPARE_JOB :
      {
        Eep_Status_t = MEMIF_IDLE;
        Eep_JobResult_t = MEMIF_JOB_FAILED;
        
        /* is a error callback aligned ?*/
        if (NULL_PTR != PointToConfig_pt->JobErrorNotif)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }
      default :
      {
        /* nothing to do in here, senceless call */
        RetVal = E_NOT_OK;
      }
    }
  }
  else
  {
    /* then there's nothing that could cause an error*/
    RetVal = E_NOT_OK;
  }

  return RetVal;
} /* End of Eep_SimulateError */


/**********************************************************************************************************************
 *  END OF FILE: Eep.c
 **********************************************************************************************************************/
 