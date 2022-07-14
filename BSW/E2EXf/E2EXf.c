/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  E2EXf.c
 *        \brief  Vector AUTOSAR E2EXf code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module E2EXf.
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

/* PRQA S 0777, 0779, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_857 */

#define E2EXF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "E2EXf.h" /* PRQA S 0810 */ /* MD_MSR_1.1_810 */

#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of E2EXf header file */
#if (  (E2EXF_SW_MAJOR_VERSION != (1U)) \
    || (E2EXF_SW_MINOR_VERSION != (3U)) \
    || (E2EXF_SW_PATCH_VERSION != (0U)) )
# error "Vendor specific version numbers of E2EXf.c and E2EXf.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (E2EXF_CFG_MAJOR_VERSION != (1U)) \
    || (E2EXF_CFG_MINOR_VERSION != (3U)) )
# error "Version numbers of E2EXf.c and E2EXf_LCfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Whether any of the profiles is enabled */
#if (E2EXF_P01_ENABLED == STD_ON) || (E2EXF_P02_ENABLED == STD_ON) || (E2EXF_P04_ENABLED == STD_ON) || (E2EXF_P05_ENABLED == STD_ON) \
    || (E2EXF_P06_ENABLED == STD_ON) || (E2EXF_P07_ENABLED == STD_ON)
# define E2EXF_ANYPROFILE_ENABLED STD_ON
#else 
# define E2EXF_ANYPROFILE_ENABLED STD_OFF
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (E2EXF_LOCAL) /* COV_E2EXF_MSR_COMPATIBILITY */
# define E2EXF_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define E2EXF_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
E2EXF_LOCAL VAR(uint8, E2EXF_VAR_ZERO_INIT) E2EXf_ModuleInitialized = E2EXF_UNINIT;

#define E2EXF_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define E2EXF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if (E2EXF_ANYPROFILE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_Check_Parameters()
 *********************************************************************************************************************/
/*! \brief      Check input parameters
 *  \details    This function checks the input parameters for plausibility.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \param[in]  minInputBufferLength    Minimum length of the input data.
 *  \pre        -
 *  \return     E_OK - input parameters plausible
 *  \return     E_SAFETY_HARD_RUNTIMEERROR - input parameters not plausible
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_Check_Parameters(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint16, AUTOMATIC, E2EXF_APPL_DATA) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  uint16 minInputBufferLength);

/**********************************************************************************************************************
 *  E2EXf_Move_Header()
 *********************************************************************************************************************/
/*! \brief      Move header
 *  \details    This function moves the header.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \pre        -
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);

/**********************************************************************************************************************
 *  E2EXf_Inv_Move_Header()
 *********************************************************************************************************************/
/*! \brief      Inverse move header
 *  \details    This function moves the header inverse.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \pre        -
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Inv_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);
#endif /* E2EXF_ANYPROFILE_ENABLED == STD_ON */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (E2EXF_ANYPROFILE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_Check_Parameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EXF_LOCAL FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_Check_Parameters(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint16, AUTOMATIC, E2EXF_APPL_DATA) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  uint16 minInputBufferLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (options->inPlace == TRUE)
  {
    /* [SWS_E2EXf_00102] 1. / [SWS_E2EXf_00105] 1. */
    if ((buffer == NULL_PTR) && (inputBufferLength != 0U))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    else if ((buffer != NULL_PTR) && (inputBufferLength < minInputBufferLength))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    /* [SWS_E2EXf_00102] 2. / [SWS_E2EXf_00105] 2. */
    else if (bufferLength == NULL_PTR)
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    /* execution of E2EXf is explicitly not wanted/expected in this case */
    /* Note: buffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00102/SWS_E2EXf_00105 */
    else if ((options->executeDespiteDataUnavailability == FALSE) && (buffer == NULL_PTR))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    else
    {
      /* nothing to do */
    }
  }
  else /* out-of-place */
  {
    /* [SWS_E2EXf_00106] 1. / [SWS_E2EXf_00103] 1. */
    if ((inputBuffer == NULL_PTR) && (inputBufferLength != 0U))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    else if ((inputBuffer != NULL_PTR) && (inputBufferLength < minInputBufferLength))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    /* [SWS_E2EXf_00106] 2. / [SWS_E2EXf_00103] 2. */
    else if (bufferLength == NULL_PTR)
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    /* [SWS_E2EXf_00106] 3. / [SWS_E2EXf_00103] 3. */
    else if (buffer == NULL_PTR)
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    /* execution of E2EXf is explicitly not wanted/expected in this case */
    /* Note: inputBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00103/SWS_E2EXf_00106 */
    else if ((options->executeDespiteDataUnavailability == FALSE) && (inputBuffer == NULL_PTR))
    {
      retVal = E_SAFETY_HARD_RUNTIMEERROR;
    }
    else
    {
      /* nothing to do */
    }
  }

  return retVal;

} /* E2EXf_Check_Parameters() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  E2EXf_Move_Header()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) psrc;
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) pdest;
  uint16 i;
  uint16 headerShiftBytes;
  uint16 headerBytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  headerShiftBytes = options->headerLength >> 3U;

  if ((inputBufferLength > 0U) && (options->upperHeaderBitsToShift > 0U))
  {
    headerBytesToCopy = options->upperHeaderBitsToShift >> 3U;
    pdest = buffer;

    if (options->inPlace == TRUE)
    {
      psrc = buffer;

      /* [SWS_E2EXf_00108] */
      /* copy header bytes */
      for (i = 0U; i < headerBytesToCopy; i++)
      {
        pdest[i] = psrc[i + headerShiftBytes];
      }
    }
    else
    {
      psrc = inputBuffer;

      /* [SWS_E2EXf_00109] */
      /* copy header bytes */
      for (i = 0U; i < headerBytesToCopy; i++)
      {
        pdest[i] = psrc[i];
      }
      /* copy remaining bytes */
      for (i = headerBytesToCopy; i < inputBufferLength; i++)
      {
        pdest[i + headerShiftBytes] = psrc[i];
      }
    }

    /* [SWS_E2EXf_00111] */
    *bufferLength = inputBufferLength + headerShiftBytes;
  }
  else
  {
    if (options->inPlace == FALSE)
    {
      /* [SWS_E2EXf_00115] */
      for (i = 0U; i < inputBufferLength; i++)
      {
        buffer[i + headerShiftBytes] = inputBuffer[i];
      }
      *bufferLength = inputBufferLength + headerShiftBytes;
    }
    else
    {
      *bufferLength = inputBufferLength;
    }
  }

} /* E2EXf_Move_Header() */

/**********************************************************************************************************************
 *  E2EXf_Inv_Move_Header()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Inv_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) psrc;
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) pdest;
  uint16 i;
  uint16 headerShiftBytes;
  uint16 headerBytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  headerShiftBytes = options->headerLength >> 3U;

  if ((inputBufferLength > 0U) && (options->upperHeaderBitsToShift > 0U))
  {
    headerBytesToCopy = options->upperHeaderBitsToShift >> 3U;
    pdest = buffer;

    if (options->inPlace == TRUE)
    {
      psrc = buffer;

      /* [SWS_E2EXf_00112] */
      /* copy header bytes */
      for (i = 0U; i < headerBytesToCopy; i++)
      {
        pdest[i + headerShiftBytes] = psrc[i];
      }
    }
    else
    {
      psrc = inputBuffer;

      /* [SWS_E2EXf_00113] */
      /* copy header bytes */
      for (i = 0U; i < headerBytesToCopy; i++)
      {
        pdest[i] = psrc[i];
      }
      /* copy remaining bytes */
      for (i = headerBytesToCopy + headerShiftBytes; i < inputBufferLength; i++)
      {
        pdest[i - headerShiftBytes] = psrc[i];
      }
    }

    /* [SWS_E2EXf_00114] */
    *bufferLength = inputBufferLength - headerShiftBytes;
  }
  else
  {
    if (options->inPlace == FALSE)
    {
      /* [SWS_E2EXf_00116] */
      for (i = headerShiftBytes; i < inputBufferLength; i++)
      {
        buffer[i - headerShiftBytes] = inputBuffer[i];
      }
      *bufferLength = inputBufferLength - headerShiftBytes;
    }
    else
    {
      *bufferLength = inputBufferLength;
    }
  }

} /* E2EXf_Inv_Move_Header() */
#endif /* (E2EXF_ANYPROFILE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2EXf_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  E2EXf_ModuleInitialized = E2EXF_UNINIT;

} /* E2EXf_InitMemory() */

/***********************************************************************************************************************
 *  E2EXf_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_Init(P2CONST(E2EXf_ConfigType, AUTOMATIC, E2EXF_PBCFG) config)
{
  /* ----- Implementation ----------------------------------------------- */
  E2EXf_Init_Calls();

  E2EXf_ModuleInitialized = (uint8)E2EXF_INIT;

  E2EXF_DUMMY_STATEMENT(config); /* PRQA S 3199 */ /* MD_MSR_14.2 */

} /* E2EXf_Init() */

/***********************************************************************************************************************
 *  E2EXf_DeInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, E2EXF_CODE) E2EXf_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = E2EXF_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* [SWS_E2EXf_00148] */
    E2EXf_ModuleInitialized = (uint8)E2EXF_UNINIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  /* [SWS_E2EXF_00146] */
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_DE_INIT, errorId);
  }
#endif

} /* E2EXf_DeInit() */

#if (E2EXF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, E2EXF_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = E2EXF_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (E2EXF_VENDOR_ID);
    versioninfo->moduleID = (E2EXF_MODULE_ID);
    versioninfo->sw_major_version = (E2EXF_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (E2EXF_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (E2EXF_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  /* [SWS_E2EXF_00149] */
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_GET_VERSION_INFO, errorId);
  }
# endif

} /* E2EXf_GetVersionInfo() */
#endif /* (E2EXF_VERSION_INFO_API == STD_ON) */

#if (E2EXF_P01_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P01()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P01(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00139] */
      if (*bufferLength != (config->DataLength >> 3U))
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00155] */
        /* if DataIDMode equals E2E_P01_DATAID_NIBBLE the low nibble of high byte of DataID is transmitted explicitly, otherwise the unused nibble is set to 0xF */
        if ((config->DataIDMode != E2E_P01_DATAID_NIBBLE) && (options->headerLength == 16U))
        {
          if ((config->CounterOffset & 0x07U) == 0U)
          {
            buffer[(config->CounterOffset >> 3U)] |= 0xF0U;
          }
          else
          {
            buffer[(config->CounterOffset >> 3U)] |= 0x0FU;
          }
        }

        /* [SWS_E2EXf_00107] */
        retVal = E2E_P01Protect(config, state, buffer);

        /* [SWS_E2EXf_00018] */
        if (retVal == E2E_E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P01() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P01_ENABLED == STD_ON) */

#if (E2EXF_P02_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P02()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P02(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00139] */
      if (*bufferLength != (config->DataLength >> 3U))
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00155] */
        if (options->headerLength == 16U)
        {
          buffer[(config->Offset >> 3U) + 1U] |= 0xF0U;
        }

        /* [SWS_E2EXf_00107] */
        retVal = E2E_P02Protect(config, state, buffer);

        /* [SWS_E2EXf_00018] */
        if (retVal == E2E_E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P02() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P02_ENABLED == STD_ON) */

#if (E2EXF_P04_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P04()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P04(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      retVal = E2E_P04Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P04() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P04_ENABLED == STD_ON) */

#if (E2EXF_P05_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P05()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P05(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      retVal = E2E_P05Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P05() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P05_ENABLED == STD_ON) */

#if (E2EXF_P06_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P06()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P06(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      retVal = E2E_P06Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P06() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P06_ENABLED == STD_ON) */

#if (E2EXF_P07_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P07()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P07(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      retVal = E2E_P07Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P07() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P07_ENABLED == STD_ON) */

#if (E2EXF_P01_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P01_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P01_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  uint8 dummyByte = 0U;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        if (options->inPlace == TRUE)
        {
          pCheckBuffer = buffer;
        }
        else
        {
          pCheckBuffer = inputBuffer;
        }

        /* [SWS_E2EXf_00140] / [SWS_E2EXf_00141] */
        /* Note: pCheckBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00105/SWS_E2EXf_00106 */
        if (pCheckBuffer == NULL_PTR)
        {
          state->NewDataAvailable = FALSE;
          pCheckBuffer = &dummyByte; /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] pointer to a 1-byte static variable */
        }
        else if (inputBufferLength == (config->DataLength >> 3U))
        {
          state->NewDataAvailable = TRUE;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR;
        }

        if (retVal == E_OK)
        {
          /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] */
          retVal = E2E_P01Check(config, state, pCheckBuffer);

          /* [SWS_E2EXf_00142] */
          if (options->profileBehavior == FALSE)
          {
            state->MaxDeltaCounter = config->MaxDeltaCounterInit;
          }

          /* [SWS_E2EXf_00029] */
          profileStatus = E2E_P01MapStatusToSM(retVal, state->Status, options->profileBehavior);

          /* [SWS_E2EXf_00028] */
          retVal = E2E_SMCheck(profileStatus, smConfig, smState);

          /* [SWS_E2EXf_00027] */
          if (retVal == E2E_E_OK)
          {
            retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
          }
          else
          {
            retVal = E_SAFETY_SOFT_RUNTIMEERROR;
          }
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P01_Inv() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P01_ENABLED == STD_ON) */

#if (E2EXF_P02_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P02_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P02_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  uint8 dummyByte = 0U;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        if (options->inPlace == TRUE)
        {
          pCheckBuffer = buffer;
        }
        else
        {
          pCheckBuffer = inputBuffer;
        }

        /* [SWS_E2EXf_00140] / [SWS_E2EXf_00141] */
        /* Note: pCheckBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00105/SWS_E2EXf_00106 */
        if (pCheckBuffer == NULL_PTR)
        {
          state->NewDataAvailable = FALSE;
          pCheckBuffer = &dummyByte; /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] pointer to a 1-byte static variable */
        }
        else if (inputBufferLength == (config->DataLength >> 3U))
        {
          state->NewDataAvailable = TRUE;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR;
        }

        if (retVal == E_OK)
        {
          /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] */
          retVal = E2E_P02Check(config, state, pCheckBuffer);

          /* [SWS_E2EXf_00142] */
          if (options->profileBehavior == FALSE)
          {
            state->MaxDeltaCounter = config->MaxDeltaCounterInit;
          }

          /* [SWS_E2EXf_00029] */
          profileStatus = E2E_P02MapStatusToSM(retVal, state->Status, options->profileBehavior);

          /* [SWS_E2EXf_00028] */
          retVal = E2E_SMCheck(profileStatus, smConfig, smState);

          /* [SWS_E2EXf_00027] */
          if (retVal == E2E_E_OK)
          {
            retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
          }
          else
          {
            retVal = E_SAFETY_SOFT_RUNTIMEERROR;
          }
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P02_Inv() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P02_ENABLED == STD_ON) */

#if (E2EXF_P04_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P04_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P04_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00104] */
        if (options->inPlace == TRUE)
        {
          retVal = E2E_P04Check(config, state, buffer, inputBufferLength);
        }
        else
        {
          retVal = E2E_P04Check(config, state, inputBuffer, inputBufferLength);
        }

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P04MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P04_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P04_ENABLED == STD_ON) */

#if (E2EXF_P05_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P05_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P05_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00104] */
        if (options->inPlace == TRUE)
        {
          retVal = E2E_P05Check(config, state, buffer, inputBufferLength);
        }
        else
        {
          retVal = E2E_P05Check(config, state, inputBuffer, inputBufferLength);
        }

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P05MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P05_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P05_ENABLED == STD_ON) */

#if (E2EXF_P06_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P06_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P06_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00104] */
        if (options->inPlace == TRUE)
        {
          retVal = E2E_P06Check(config, state, buffer, inputBufferLength);
        }
        else
        {
          retVal = E2E_P06Check(config, state, inputBuffer, inputBufferLength);
        }

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P06MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P06_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P06_ENABLED == STD_ON) */

#if (E2EXF_P07_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P07_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P07_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint16 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == TRUE)
      {
        retVal = E_SAFETY_SOFT_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00104] */
        if (options->inPlace == TRUE)
        {
          retVal = E2E_P07Check(config, state, buffer, inputBufferLength);
        }
        else
        {
          retVal = E2E_P07Check(config, state, inputBuffer, inputBufferLength);
        }

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P07MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId);
# endif

  return retVal;

} /* E2EXf_P07_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P07_ENABLED == STD_ON) */

#define E2EXF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* COV_JUSTIFICATION_BEGIN

--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

  \ID COV_E2EXF_MSR_COMPATIBILITY
  \ACCEPT TX 
  \REASON [COV_MSR_COMPATIBILITY]
  
COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: E2EXf.c
 *********************************************************************************************************************/
