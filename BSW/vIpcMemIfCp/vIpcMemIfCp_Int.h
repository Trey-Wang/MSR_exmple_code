/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  vIpcMemIfCp header file
 *
 *      \details  Contains the declaration and definition of the module's internal functions.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemIfCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMIFCP_INT_H)                                                                                        /* PRQA S 0883 */ /* MD_vIpcMemIfCp_0883 */
# define VIPCMEMIFCP_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemIfCp.h"
# include "vIpcMemIfCp_PBcfg.h"

# if (VIPCMEMIFCP_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

# if (VIPCMEMIFCP_USE_INIT_POINTER == STD_ON)
#  include "EcuM_Error.h"
# else
#  define EcuM_BswErrorHook(x, y)
#  define vIpcMemIfCp_GetFinalMagicNumber()  (0)
# endif

# include "SchM_vIpcMemIfCp.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define vIpcMemIfCp_UL_CopyTxData        vIpc_CopyTxData                     /*!< Mapping of the upper layer function CopyTxData */
# define vIpcMemIfCp_UL_TxConfirmation    vIpc_TxConfirmation                 /*!< Mapping of the upper layer function TxConfirmation */

# define vIpcMemIfCp_UL_StartOfReception  vIpc_StartOfReception               /*!< Mapping of the upper layer function StartOfReception */
# define vIpcMemIfCp_UL_CopyRxData        vIpc_CopyRxData                     /*!< Mapping of the upper layer function CopyRxData */
# define vIpcMemIfCp_UL_RxIndication      vIpc_RxIndication                   /*!< Mapping of the upper layer function RxIndication */

# define vIpcMemIfCp_LL_Tx_BeginTransmit  vIpcMemCp_Tx_BeginTransmit          /*!< Mapping of the lower layer function Tx_BeginTransmit */
# define vIpcMemIfCp_LL_Tx_GetBuffer      vIpcMemCp_Tx_GetBuffer              /*!< Mapping of the lower layer function Tx_GetBuffer */
# define vIpcMemIfCp_LL_Tx_Confirm        vIpcMemCp_Tx_Confirm                /*!< Mapping of the lower layer function Tx_Confirm */
# define vIpcMemIfCp_LL_Tx_Cancel         vIpcMemCp_Tx_Cancel                 /*!< Mapping of the lower layer function Tx_Cancel */

# define vIpcMemIfCp_LL_Rx_BeginReceive   vIpcMemCp_Rx_BeginReceive           /*!< Mapping of the lower layer function Rx_BeginReceive */
# define vIpcMemIfCp_LL_Rx_GetBuffer      vIpcMemCp_Rx_GetBuffer              /*!< Mapping of the lower layer function Rx_GetBuffer */
# define vIpcMemIfCp_LL_Rx_Confirm        vIpcMemCp_Rx_Confirm                /*!< Mapping of the lower layer function Rx_Confirm */
# define vIpcMemIfCp_LL_Rx_Cancel         vIpcMemCp_Rx_Cancel                 /*!< Mapping of the lower layer function Rx_Canel */

/*! Wrapper for entering the module's exclusive area. */
# define vIpcMemIfCp_EnterExclusiveArea() SchM_Enter_vIpcMemIfCp_VIPCMEMIFCP_EXCLUSIVE_AREA_0()
/*! Wrapper for leaving the module's exclusive area. */
# define vIpcMemIfCp_ExitExclusiveArea()  SchM_Exit_vIpcMemIfCp_VIPCMEMIFCP_EXCLUSIVE_AREA_0()

/*! Value used to mark the module as initialized. */
# define VIPCMEMIFCP_INITIALIZED      (vIpcMemIfCp_InitStatusType)(TRUE)
/*! Value used to mark the module as uninitialized. */
# define VIPCMEMIFCP_NOT_INITIALIZED  (vIpcMemIfCp_InitStatusType)(FALSE)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (VIPCMEMIFCP_LOCAL)
#  define VIPCMEMIFCP_LOCAL static
# endif

# if !defined (VIPCMEMIFCP_LOCAL_INLINE)
#  define VIPCMEMIFCP_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern P2CONST(vIpcMemIfCp_ConfigType, VIPCMEMIFCP_VAR_ZERO_INIT, VIPCMEMIFCP_PBCFG) vIpcMemIfCp_ConfigDataPtr;
extern VAR(vIpcMemIfCp_InitStatusType, VIPCMEMIFCP_VAR_ZERO_INIT) vIpcMemIfCp_InitStatus;

# define VIPCMEMIFCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemIfCp_UseInitPointer()
 *********************************************************************************************************************/
/*! \brief       Returns whether the usage of init pointer is enabled.
 *  \details     -
 *  \return      TRUE if UseInitPointer is on, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_UseInitPointer
(
   void
);


/**********************************************************************************************************************
 *  vIpcMemIfCp_IsFinalMagicNumberOn()
 *********************************************************************************************************************/
/*! \brief       Returns whether the final magic number check is enabled.
 *  \details     -
 *  \return      TRUE if FinalMagicNumber is on, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_IsFinalMagicNumberOn
(
   void
);


/**********************************************************************************************************************
 *  vIpcMemIfCp_UseEcumBswErrorHook()
 *********************************************************************************************************************/
/*! \brief       Returns whether the usage EcuM Bsw ErrorHook is enabled.
 *  \details     -
 *  \return      TRUE if UseEcumBswErrorHook is on, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_UseEcumBswErrorHook
(
   void
);


/**********************************************************************************************************************
 *  vIpcMemIfCp_DevErrorDetect()
 *********************************************************************************************************************/
/*! \brief       Returns whether DevErrorDetect is enabled or not
 *  \details     -
 *  \return      TRUE if DevErrorDetect is on, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_DevErrorDetect
(
   void
);



/**********************************************************************************************************************
 *  vIpcMemIfCp_DevErrorReport()
 *********************************************************************************************************************/
/*! \brief       Returns whether DevErrorReport is enabled or not
 *  \details     -
 *  \return      TRUE if DevErrorReport is on, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_DevErrorReport
(
   void
);


/**********************************************************************************************************************
 *  vIpcMemIfCp_CheckFinalMagicNumber()
 *********************************************************************************************************************/
/*! \brief       Returns the check failed or not.
 *  \details     -
 *  \return      TRUE if FinalMagicNumber is invalid, FALSE otherwise.
 *  \pre         -
 *  \context     INTERNAL
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_CheckFinalMagicNumber
(
   void
);


/**********************************************************************************************************************
 * vIpcMemIfCp_ReportError()
 *********************************************************************************************************************/
/*! \brief         Calls Det_ReportError() if configured and an error has occured.
 *  \details       -
 *  \param[in]     ApiId                  Id of the API service.
 *  \param[in]     ErrorId                Error code.
 *  \return        E_OK if there was no error, E_NOT_OK otherwise.
 *  \pre           -
 *  \context       INTERNAL
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_ReportError
(
   vIpcMemIfCp_ServiceIdType ApiId,
   vIpcMemIfCp_ErrorCodeType ErrorId
);


/**********************************************************************************************************************
 *  vIpcMemIfCp_IsInitialized()
 *********************************************************************************************************************/
/*! \brief       Returns if the module was initialized.
 *  \details     -
 *  \return      TRUE if the module was initialized, FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_IsInitialized
(
   void
);

# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  vIpcMemIfCp_UseInitPointer()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_UseInitPointer
(
   void
)
{
   return (boolean)(VIPCMEMIFCP_USE_INIT_POINTER == STD_ON);
} /* vIpcMemIfCp_UseInitPointer */


/**********************************************************************************************************************
 *  vIpcMemIfCp_IsFinalMagicNumberOn()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_IsFinalMagicNumberOn
(
   void
)
{
   return (boolean)(VIPCMEMIFCP_FINALMAGICNUMBER == STD_ON);
} /* vIpcMemIfCp_IsFinalMagicNumberOn */


/**********************************************************************************************************************
 *  vIpcMemIfCp_UseEcumBswErrorHook()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_UseEcumBswErrorHook
(
   void
)
{
   return (boolean)(VIPCMEMIFCP_USE_ECUM_BSW_ERROR_HOOK == STD_ON);
} /* vIpcMemIfCp_UseEcumBswErrorHook */


/**********************************************************************************************************************
 *  vIpcMemIfCp_DevErrorDetect()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_DevErrorDetect
(
   void
)
{
   return (boolean)(VIPCMEMIFCP_DEV_ERROR_DETECT == STD_ON);
} /* vIpcMemIfCp_DevErrorDetect */


/**********************************************************************************************************************
 *  vIpcMemIfCp_DevErrorReport()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_DevErrorReport
(
   void
)
{
   return (boolean)(VIPCMEMIFCP_DEV_ERROR_REPORT == STD_ON);
} /* vIpcMemIfCp_DevErrorReport */


/**********************************************************************************************************************
 *  vIpcMemIfCp_CheckFinalMagicNumber()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_CheckFinalMagicNumber
(
   void
)
{
   boolean result = FALSE;
   if (vIpcMemIfCp_IsFinalMagicNumberOn() == TRUE)
   {
      result = (boolean)(vIpcMemIfCp_GetFinalMagicNumber() != VIPCMEMIFCP_FINAL_MAGIC_NUMBER);
   }
   return result;
} /* vIpcMemIfCp_CheckFinalMagicNumber */


/**********************************************************************************************************************
 * vIpcMemIfCp_ReportError()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_ReportError
(
   vIpcMemIfCp_ServiceIdType ApiId,
   vIpcMemIfCp_ErrorCodeType ErrorId
)
{
   Std_ReturnType result = E_OK;

   if((vIpcMemIfCp_DevErrorReport() == TRUE) && (ErrorId != VIPCMEMIFCP_E_NO_ERROR))
   {
      (void) Det_ReportError(VIPCMEMIFCP_MODULE_ID, VIPCMEMIFCP_INSTANCE_ID, ApiId, ErrorId);
      result = E_NOT_OK;
   }
   return result;
} /* vIpcMemIfCp_ReportError */

/**********************************************************************************************************************
 *  vIpcMemIfCp_IsInitialized()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_IsInitialized
(
   void
)
{
   return (boolean)(vIpcMemIfCp_InitStatus == VIPCMEMIFCP_INITIALIZED);
} /* vIpcMemIfCp_IsInitialized */


# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMIFCP_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp_Int.h
 *********************************************************************************************************************/
