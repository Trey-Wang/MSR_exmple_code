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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  _Appl_AvTp.c
 *      Project:  Demo Audio Application for Audio Video Bridging Transport Protocol
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implementation of Demo Audio Application
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Dennis Mueller                visdmi           Vector Informatik GmbH
 *  Jeroen Laverman               vislje           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-10-14  visdmi  -             created
 *  01.01.00  2014-09-15  vislje  -             Updated to IEEE 1722a-D9
 *  01.01.01  2015-03-04  vislje  -             Fixed several compiler warnings  
 *  01.01.02  2015-07-20  vislje  -             Fixed further compiler warnings (nested comment / compare uint8 < 256)
 *  01.01.03  2017-01-18  vissem  -             Fixed review findings
 *********************************************************************************************************************/

#define AVTP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "_Appl_AvTp.h"

#include "IpBase.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FCT_LIKE_MACROS */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio(uint16 LenByte);



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

const sint32 AvTp_App_FirImpulseResponse[AVTP_APP_FILTER_ORDER] = {
1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1
};

const float64 AvTp_App_ToneA_Sinus_LUT[AVTP_APP_TONEA_SINUS_LUT_COUNT]={
 0, 0.058145, 0.116093, 0.173648, 0.230615, 0.286803,
 0.342020, 0.396080, 0.448799, 0.5, 0.549509, 0.597159,
 0.642788, 0.686242, 0.727374, 0.766044, 0.802123, 0.835478,
 0.866025, 0.893633, 0.918216, 0.939693, 0.957999, 0.973045,
 0.984808, 0.993238, 0.998308, 1
};

const float64 AvTp_App_ToneE_Sinus_LUT[AVTP_APP_TONEE_SINUS_LUT_COUNT]={
  0, 
  0.043619, 0.087156, 0.130526, 0.173648, 0.216440, 0.258820,
  0.300706, 0.342020, 0.382683, 0.422618, 0.461749, 0.5,
  0.537300, 0.573576, 0.608761, 0.642788, 0.675590, 0.707107,
  0.737277, 0.766044, 0.793353, 0.819152, 0.843391, 0.866025,
  0.887010, 0.906308, 0.923880, 0.939693, 0.953717, 0.965926,
  0.976296, 0.984808, 0.991445, 0.996195, 0.999048, 1
};



sint32 AvTp_App_Samples[AVTP_APP_FILTER_ORDER];
sint32 AvTp_App_SampleIndex;
sint8 AvTp_App_ToneA_SampleIndex = 0;
uint8 AvTp_App_ToneA_CurrentVolume = 1;
uint16 AvTp_App_ToneA_CurrentFrequency  = 0;
uint8 AvTp_App_ToneA_NewVolume = 1;
sint32 AvTp_App_ToneA_Sample = 0;
sint8 AvTp_App_ToneA_UpDown = 1;
uint8 AvTp_App_ToneA_TxReady = 0;
boolean AvTp_App_ToneA_Requested = AVTP_APP_TONEA_IS_NOT_REQUESTED; 

uint8 AvTp_App_FIR_TxReady = 0;

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AvTp_App_ListenerAudio
 **********************************************************************************************************************/
/*! \brief      listen to a avb audio stream and transmits the filtered stream
 *  \param[in]  DataPtr                 Pointer to the received audio data
 *  \param[in]  LenByte                 Byte count of the received audio data
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_ListenerAudio1(
   AVTP_P2CONST(AvTp_DataType) DataPtr,
   AVTP_P2CONST(AvTp_RxPduInfoType) StreamInfoPtr,
   uint16 LenByte)
{
  AVTP_P2VAR(uint32) AvTp_AvTp_IEC_Audio;
  AVTP_P2VAR(AvTp_Iec61883HdrType) AvTp_IecHdrPtr;
  uint8 Idx;
  uint32 RawSample;
  sint32 Sample;

  StreamInfoPtr = StreamInfoPtr;
  
  if((AvTp_App_FIR_TxReady != 1) || (AvTp_App_ToneA_Requested == AVTP_APP_TONEA_IS_REQUESTED))
  {
    return;
  }

  /* length check */
  if( LenByte >=  (AVTP_IEC_HDR_LEN + (AVTP_IEC_SAMPLE_COUNT * AVTP_IEC_CHANNEL_COUNT * 4)) )
  {
    AvTp_IecHdrPtr = (AVTP_P2VAR(AvTp_Iec61883HdrType)) DataPtr;
    /* sanity check */
    if( AVTP_IEC_HDR_SPH(AvTp_IecHdrPtr->FnQpcSphRsv) == AVTP_IEC_SPH_NO_SOURCE_PACKET && \
        AVTP_IEC_HDR_SID(AvTp_IecHdrPtr->Qi1Sid) == AVTP_IEC_SID_AVTP_NETWORK )
    {
      AvTp_AvTp_IEC_Audio = (uint32*)(&DataPtr[AVTP_IEC_HDR_LEN]);
      
      /* write samples in ring buffer */
      for(Idx = 0; Idx<AVTP_IEC_SAMPLE_COUNT; Idx++)
      {
        RawSample = AvTp_Ntohl(AvTp_AvTp_IEC_Audio[Idx * AVTP_IEC_CHANNEL_COUNT + AVTP_IEC_CHANNEL_L]);
        /* check for sign bit of int24 sample data */
        if(RawSample & (1<<24))
        {
          /* fill unused byte for int32 handling */
          Sample = 0xFF000000 | RawSample;
        }
        else
        {
          Sample = RawSample;
        }
        /* write the current left channel sample in ring buffer */
        AvTp_App_Samples[AvTp_App_SampleIndex++ % AVTP_APP_FILTER_ORDER] = Sample;
      }

      /* process and transmit audio data */
      AvTp_App_TalkerAudio(AVTP_IEC_SAMPLE_COUNT * AVTP_IEC_CHANNEL_COUNT * 4);
    }
  }
}

/***********************************************************************************************************************
 *  AvTp_App_TalkerAudio
 **********************************************************************************************************************/
/*! \brief      transmits a audio stream
 *  \param[in]  LenByte                 Byte count of the received audio data
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio(
   uint16 LenByte)
{
  uint8 IdxSample;
  uint8 RingBufferIdx;
  uint8 IdxFilter;
  signed long long CurrentSample;

  AVTP_P2VAR (AvTp_Iec61883HdrType) AvTp_StrIecHdrPtr;
  AVTP_P2VAR(uint32) AvTp_AvTp_IEC_Audio;
  uint8 StreamIdx = 1;
  
  uint16 TxLenghByte = AVTP_IEC_HDR_LEN + LenByte;

  if(BUFREQ_OK ==  AvTp_ProvideTxBuffer(StreamIdx, (AVTP_P2VAR(AvTp_DataType)*) &AvTp_StrIecHdrPtr, &TxLenghByte) )
  {
    AvTp_StrIecHdrPtr->Qi1Sid =      (AVTP_IEC_QI1 << AVTP_IEC_HDR_QI_BIT_POS ) |
                                     (AVTP_IEC_SID_AVTP_NETWORK << AVTP_IEC_HDR_SID_BIT_POS);
    AvTp_StrIecHdrPtr->Dbs =         AVTP_IEC_CHANNEL_COUNT;
    AvTp_StrIecHdrPtr->FnQpcSphRsv = (FALSE << AVTP_IEC_HDR_FN_BIT_POS) |
                                     (FALSE << AVTP_IEC_HDR_QPC_BIT_POS)  |
                                     (AVTP_IEC_SPH_NO_SOURCE_PACKET << AVTP_IEC_HDR_SPH_BIT_POS);
    AvTp_StrIecHdrPtr->Dbc =         0;
    AvTp_StrIecHdrPtr->Qi2Fmt =      (AVTP_IEC_QI2 << AVTP_IEC_HDR_QI_BIT_POS) |
                                     (0x10 << AVTP_IEC_HDR_FMT_BIT_POS);
    AvTp_StrIecHdrPtr->Fdf =         0x02;
    AvTp_StrIecHdrPtr->Syt =         AVTP_IEC_SYT;

    AvTp_AvTp_IEC_Audio =            (AVTP_P2VAR(uint32)) &((AVTP_P2VAR(uint8))AvTp_StrIecHdrPtr)[AVTP_IEC_HDR_LEN];

    /* set read ring buffer index to latest sample */
    RingBufferIdx = ((uint8)(AvTp_App_SampleIndex-1)) % AVTP_APP_FILTER_ORDER;
    IdxSample = AVTP_IEC_SAMPLE_COUNT;
    /* FIR Low-pass filter processing */
    while(0u < IdxSample)
    {
      IdxSample--;
      CurrentSample = 0;
      /* convolution of FIR filter response with latest samples in ring buffer */
      for(IdxFilter = 0; IdxFilter < AVTP_IEC_SAMPLE_COUNT; IdxFilter++)
      {
        CurrentSample += AvTp_App_FirImpulseResponse[IdxFilter] * \
          AvTp_App_Samples[((uint8)(RingBufferIdx - IdxFilter - IdxSample)) % AVTP_APP_FILTER_ORDER];
      }
      /* Set left and right channel (mono) to filtered sample */
      AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_L] = \
                        AvTp_Htonl((uint32)CurrentSample);
      AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_R] = \
                        AvTp_Htonl((uint32)CurrentSample);
    }
    /* transmit the avb Pdu */
    AvTp_Transmit(StreamIdx, LenByte + AVTP_IEC_HDR_LEN);
  }
}

/***********************************************************************************************************************
 *  AvTp_App_TalkerAudio_ToneA
 **********************************************************************************************************************/
/*! \brief      transmits a audio stream
 *  \param[in]  LenByte                 Byte count of the received audio data
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio_ToneA(
   uint16 LenByte)
{
  uint8 IdxSample;
  uint8 IdxPacket;
  sint8 MultPrefix = 1;


  AVTP_P2VAR (AvTp_Iec61883HdrType) AvTp_StrIecHdrPtr;
  AVTP_P2VAR(uint32) AvTp_AvTp_IEC_Audio;
  uint8 StreamIdx = 0;
  
  uint16 TxLenghByte = AVTP_IEC_HDR_LEN + LenByte;

  if((AvTp_App_ToneA_TxReady != 1))
  {
    return;
  }


  for(IdxPacket=0; IdxPacket<AVTP_APP_TONEA_MAX_PACKET_COUNT; IdxPacket++)
  {
    if(BUFREQ_OK ==  AvTp_ProvideTxBuffer(StreamIdx, (AVTP_P2VAR(AvTp_DataType)*) &AvTp_StrIecHdrPtr, &TxLenghByte) )
    {
                        
      AvTp_StrIecHdrPtr->Qi1Sid =      (AVTP_IEC_QI1 << AVTP_IEC_HDR_QI_BIT_POS ) |
                                       (AVTP_IEC_SID_AVTP_NETWORK << AVTP_IEC_HDR_SID_BIT_POS);
      AvTp_StrIecHdrPtr->Dbs =         AVTP_IEC_CHANNEL_COUNT;
      AvTp_StrIecHdrPtr->FnQpcSphRsv = (FALSE << AVTP_IEC_HDR_FN_BIT_POS) |
                                       (FALSE << AVTP_IEC_HDR_QPC_BIT_POS)  |
                                       (AVTP_IEC_SPH_NO_SOURCE_PACKET << AVTP_IEC_HDR_SPH_BIT_POS);
      AvTp_StrIecHdrPtr->Dbc =         0;
      AvTp_StrIecHdrPtr->Qi2Fmt =      (AVTP_IEC_QI2 << AVTP_IEC_HDR_QI_BIT_POS) |
                                       (0x10 << AVTP_IEC_HDR_FMT_BIT_POS);
      AvTp_StrIecHdrPtr->Fdf =         0x02;
      AvTp_StrIecHdrPtr->Syt =         AVTP_IEC_SYT;

      AvTp_AvTp_IEC_Audio =            (AVTP_P2VAR(uint32)) &((AVTP_P2VAR(uint8))AvTp_StrIecHdrPtr)[AVTP_IEC_HDR_LEN];


      IdxSample = AVTP_IEC_SAMPLE_COUNT;

      while(0u < IdxSample)
      {
        IdxSample--;
        
        if(AvTp_App_ToneA_UpDown == AVTP_APP_TONEA_SIN_UP)
        {/*increasing sinus*/
          if(AvTp_App_ToneA_SampleIndex >= AVTP_APP_TONEA_HIGH_SAMPLE_NR)
          {/*Last sample index was already highest*/
            AvTp_App_ToneA_SampleIndex = AVTP_APP_TONEA_HIGH_SAMPLE_NR-1;
            AvTp_App_ToneA_UpDown = AVTP_APP_TONEA_SIN_DOWN;
          }
          else
          {
            AvTp_App_ToneA_SampleIndex++;
          }

        }
        else if(AvTp_App_ToneA_UpDown == AVTP_APP_TONEA_SIN_DOWN)
        {/*decreasing sinus*/
          if(AvTp_App_ToneA_SampleIndex <= AVTP_APP_TONEA_LOW_SAMPLE_NR)
          {/*Last sample index was already lowest*/
            AvTp_App_ToneA_SampleIndex = AVTP_APP_TONEA_LOW_SAMPLE_NR+1;
            AvTp_App_ToneA_UpDown = AVTP_APP_TONEA_SIN_UP;
          }
          else
          {
            AvTp_App_ToneA_SampleIndex--;
          }
        }

        if(AvTp_App_ToneA_SampleIndex > 0)
        {
          MultPrefix = 1;
        }
        else if(AvTp_App_ToneA_SampleIndex < 0)
        {
          MultPrefix = -1;
        }
        else
        {
          MultPrefix = 0;
        }

        AvTp_App_ToneA_Sample = (sint32)(AvTp_App_ToneA_Sinus_LUT[(AvTp_App_ToneA_SampleIndex*MultPrefix)]*\
          (float32)(AVTP_APP_TONEA_SIN_MAX_SIZE * AvTp_App_ToneA_CurrentVolume*MultPrefix));

        /* Set left and right channel (mono) to filtered sample */
        AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_L] = \
                          AvTp_Htonl((uint32)AvTp_App_ToneA_Sample);
        AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_R] = \
                          AvTp_Htonl((uint32)AvTp_App_ToneA_Sample);
      }
      /* transmit the avb Pdu */
      AvTp_Transmit(StreamIdx, LenByte + AVTP_IEC_HDR_LEN);

    }
  }

}

/***********************************************************************************************************************
 *  AvTp_App_TalkerAudio_ToneE
 **********************************************************************************************************************/
/*! \brief      transmits a audio stream
 *  \param[in]  LenByte                 Byte count of the received audio data
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_App_TalkerAudio_ToneE(
   uint16 LenByte)
{
  uint8 IdxSample;
  uint8 IdxPacket;
  sint8 MultPrefix = 1;

  AVTP_P2VAR (AvTp_Iec61883HdrType) AvTp_StrIecHdrPtr;
  AVTP_P2VAR(uint32) AvTp_AvTp_IEC_Audio;
  uint8 StreamIdx = 0;
  
  uint16 TxLenghByte = AVTP_IEC_HDR_LEN + LenByte;

  if((AvTp_App_ToneA_TxReady != 1) || (AvTp_App_ToneA_Requested == AVTP_APP_TONEA_IS_NOT_REQUESTED))
  {
    
    return;
  }


  for(IdxPacket=0; IdxPacket<AVTP_APP_TONEA_MAX_PACKET_COUNT; IdxPacket++)
  {
    if(BUFREQ_OK ==  AvTp_ProvideTxBuffer(StreamIdx, (AVTP_P2VAR(AvTp_DataType)*) &AvTp_StrIecHdrPtr, &TxLenghByte) )
    {
      AvTp_StrIecHdrPtr->Qi1Sid =      (AVTP_IEC_QI1 << AVTP_IEC_HDR_QI_BIT_POS ) |
                                       (AVTP_IEC_SID_AVTP_NETWORK << AVTP_IEC_HDR_SID_BIT_POS);
      AvTp_StrIecHdrPtr->Dbs =         AVTP_IEC_CHANNEL_COUNT;
      AvTp_StrIecHdrPtr->FnQpcSphRsv = (FALSE << AVTP_IEC_HDR_FN_BIT_POS) |
                                       (FALSE << AVTP_IEC_HDR_QPC_BIT_POS)  |
                                       (AVTP_IEC_SPH_NO_SOURCE_PACKET << AVTP_IEC_HDR_SPH_BIT_POS);
      AvTp_StrIecHdrPtr->Dbc =         0;
      AvTp_StrIecHdrPtr->Qi2Fmt =      (AVTP_IEC_QI2 << AVTP_IEC_HDR_QI_BIT_POS) |
                                       (0x10 << AVTP_IEC_HDR_FMT_BIT_POS);
      AvTp_StrIecHdrPtr->Fdf =         0x02;
      AvTp_StrIecHdrPtr->Syt =         AVTP_IEC_SYT;

      AvTp_AvTp_IEC_Audio =            (AVTP_P2VAR(uint32)) &((AVTP_P2VAR(uint8))AvTp_StrIecHdrPtr)[AVTP_IEC_HDR_LEN];


      IdxSample = AVTP_IEC_SAMPLE_COUNT;

      while(0u < IdxSample)
      {
        IdxSample--;
        
        if(AvTp_App_ToneA_UpDown == AVTP_APP_TONEA_SIN_UP)
        {/*increasing sinus*/
          if(AvTp_App_ToneA_SampleIndex >= AVTP_APP_TONEA_HIGH_SAMPLE_NR)
          {/*Last sample index was already highest*/
            AvTp_App_ToneA_SampleIndex = AVTP_APP_TONEA_HIGH_SAMPLE_NR-1;
            AvTp_App_ToneA_UpDown = AVTP_APP_TONEA_SIN_DOWN;
          }
          else
          {
            AvTp_App_ToneA_SampleIndex++;
          }

        }
        else if(AvTp_App_ToneA_UpDown == AVTP_APP_TONEA_SIN_DOWN)
        {/*decreasing sinus*/
          if(AvTp_App_ToneA_SampleIndex <= AVTP_APP_TONEA_LOW_SAMPLE_NR)
          {/*Last sample index was already lowest*/
            AvTp_App_ToneA_SampleIndex = AVTP_APP_TONEA_LOW_SAMPLE_NR+1;
            AvTp_App_ToneA_UpDown = AVTP_APP_TONEA_SIN_UP;
          }
          else
          {
            AvTp_App_ToneA_SampleIndex--;
          }
        }

        if(AvTp_App_ToneA_SampleIndex > 0)
        {
          MultPrefix = 1;
        }
        else if(AvTp_App_ToneA_SampleIndex < 0)
        {
          MultPrefix = -1;
        }
        else
        {
          MultPrefix = 0;
        }

        AvTp_App_ToneA_Sample = (sint32)(AvTp_App_ToneA_Sinus_LUT[(AvTp_App_ToneA_SampleIndex*MultPrefix)]*\
          (float32)(AVTP_APP_TONEA_SIN_MAX_SIZE * AvTp_App_ToneA_CurrentVolume*MultPrefix));

        /* Set left and right channel (mono) to filtered sample */
        AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_L] = \
                          AvTp_Htonl((uint32)AvTp_App_ToneA_Sample);
        AvTp_AvTp_IEC_Audio[AVTP_IEC_CHANNEL_COUNT * (5 - IdxSample) + AVTP_IEC_CHANNEL_R] = \
                          AvTp_Htonl((uint32)AvTp_App_ToneA_Sample);
      }
      /* transmit the avb Pdu */
      AvTp_Transmit(StreamIdx, LenByte + AVTP_IEC_HDR_LEN);
    }
  }

}

FUNC(uint8 ,AVTP_CODE) AvTp_App_GetCurrentVolume(void)
{
  return AvTp_App_ToneA_CurrentVolume;
}

FUNC(boolean, AVTP_CODE) AvTp_App_Set_On_Off(boolean req)
{

    if(req == AVTP_APP_TONEA_IS_REQUESTED)
    {
      AvTp_App_ToneA_Requested = req;
      AvTp_App_ToneA_CurrentFrequency = AVTP_APP_TONEA_FREQUENCY;
      return E_OK;
    }
    else if(req == AVTP_APP_TONEA_IS_NOT_REQUESTED)
    {
      AvTp_App_ToneA_Requested = req;
      AvTp_App_ToneA_CurrentFrequency = AVTP_APP_TONEA_NO_FREQUENCY;
      return E_OK;
    }
    else
    {
      return E_NOT_OK;
    }
}

FUNC(boolean, AVTP_CODE) AvTp_App_SetVolume(uint8 vol)
{
  if(AvTp_App_ToneA_CurrentVolume != vol)
  {
    AvTp_App_ToneA_CurrentVolume = vol;
  }

  return E_OK;
}


FUNC(void, AVTP_CODE) AvTp_App_Init(void)
{
  
  AvTp_App_ToneA_TxReady = 0;
  AvTp_App_ToneA_Requested = AVTP_APP_TONEA_IS_NOT_REQUESTED;
  AvTp_App_ToneA_CurrentVolume = 1;

  /*
  uint8 Idx = AVTP_APP_FILTER_ORDER;

  while(0u < Idx)
  {
    Idx--;
    AvTp_App_Samples[Idx] = 0;
  }
  AvTp_App_SampleIndex = 0;
  */
}

FUNC(uint16 ,AVTP_CODE) AvTp_App_GetCurrentFrequency(void)
{
  return AvTp_App_ToneA_CurrentFrequency;
}

FUNC(void, AVTP_CODE) AvTp_App_StateChangedCbk1(AvTp_StateType NewState)
{
  if(NewState == AVTP_STATE_TX_READY)
  {
    AvTp_App_ToneA_TxReady = 1;
  }
  else
  {
    AvTp_App_ToneA_TxReady = 0;
    AvTp_App_ToneA_CurrentFrequency = AVTP_APP_TONEA_NO_FREQUENCY;
  }
}

FUNC(void, AVTP_CODE) AvTp_App_StateChangedCbk2(AvTp_StateType NewState)
{
  if(NewState == AVTP_STATE_TX_READY)
  {
    AvTp_App_FIR_TxReady = 1;
  }
  else
  {
    AvTp_App_FIR_TxReady = 0;
  }
}

/**********************************************************************************************************************
 *  END OF FILE: Appl_AvTp.c
 *********************************************************************************************************************/




