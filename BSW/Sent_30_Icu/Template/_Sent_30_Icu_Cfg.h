/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Sent_30_Icu_Cfg.h
 *        \brief  Header file of the Vector Sent_30_Icu Handler configuration
 *
 *      \details  The Sent_30_Icu handler is a software component which is meant to enable usage of SENT sensors on
 *                Vector ECUs. The SWC assists the application layer / sensor driver by providing the raw data which
 *                is received from the sensor.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Denis Althapp                 virdea        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  00.00.01  2015-05-12  virdea  -             Initial version
 *  00.01.00  2015-05-22  virdea  -             First working version for fast channel messages
 *  00.02.00  2015-05-27  virdea  -             Implemented slow channel functionality, added processing with a channel
 *                                              notification
 *  00.03.00  2015-05-28  virdea  -             Small bugfixes and more descriptions, FastChan+Both Slowchan modes
 *                                              tested and working
 *  00.04.00  2015-05-29  virdea  -             Bugfix for less than 6 data nibbles+pause pulse
 *  00.05.00  2015-05-29  virdea  -             Added GetVersionInfo, ASR3+4 support, DET
 *  00.06.00  2015-07-31  virdea  -             Added SENT_30_ICU_SOURCE define, reorganized init variables, fixed typo
 *                                              in compiler abstraction, added getter API instead of global variables
 *                                              and a notification function
 *  00.07.00  2015-08-05  virdea  -             Several QAC fixes
 *  01.00.00  2015-10-09  virdea  -             Added additional diagnostic features (ENABLE_DIAGNOSIS)
 *
*********************************************************************************************************************/


#ifndef SENT_30_ICU_CFG_H
#define SENT_30_ICU_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define SENT_30_ICU_CFG_SW_MAJOR_VERSION                    (1u)
#define SENT_30_ICU_CFG_SW_MINOR_VERSION                    (0u)
#define SENT_30_ICU_CFG_SW_PATCH_VERSION                    (0u)

/* defines possible serial message formats */
#define SENT_30_ICU_NO_SERIAL_MESSAGE                       (0u)
#define SENT_30_ICU_SHORT_SERIAL_MESSAGE                    (1u)
#define SENT_30_ICU_ENHANCED_SERIAL_MESSAGE                 (2u)


/* ----- Component configuration ----- */
/*! \brief defines if DET is used (ASR4) */
#define SENT_30_ICU_DEV_ERROR_DETECT                        (STD_ON)
/*! \brief defines if DET is used (ASR3) */
#define SENT_30_ICU_DEVELOPMENT_MODE                        (SENT_30_ICU_DEV_ERROR_DETECT)
/*! \brief control version info API */
#define SENT_30_ICU_VERSION_INFO_API                        (STD_ON)
/*! \brief defines the ICU timer ticks per us
 * VC54S: 120Mhz / (2(periphal prescaler) * 1(eMIOS prescaler)) = 60Mhz => 60uc-ticks per us */
#define SENT_30_ICU_TIMER_CLOCK_UC_TICKS                    (60u)
/*! \brief defines the SENT protocol clock tick time in us */
#define SENT_30_ICU_TICK_TIME                               (3u)
/*! \brief defines the amount of sent data nibbles (without crc, status) */
#define SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES                  (6u)
/*! \brief defines if a pause pulse is sent */
#define SENT_30_ICU_PAUSE_PULSE                             (STD_OFF)
/*! \brief defines if any form of serial messaging is used */
#define SENT_30_ICU_SERIAL_FORMAT                           (SENT_30_ICU_NO_SERIAL_MESSAGE)
/*! \brief defines the configured IcuChannel */
#define SENT_30_ICU_CHANNEL                                 (IcuConf_IcuChannel_SENT_IN)
/*! \brief defines if a mainfunction or an Icu callback(recommended) should be used for processing */
#define SENT_30_ICU_USE_MAINFUNCTION                        (STD_OFF)
/*! \brief defines the percentage threshold of the calibration pulse detection */
#define SENT_30_ICU_CALIB_THRESH_PERCENT                    (25u)
/*! \brief defines if the diagnostic information APIs shall be available */
#define SENT_30_ICU_ENABLE_DIAGNOSIS                        (STD_ON)
/*! \brief defines if notification functions shall be called upon receiving a complete message */
#define SENT_30_ICU_USE_NOTIFICATION                        (STD_ON)
#if (SENT_30_ICU_USE_NOTIFICATION == STD_ON)
/*! \brief defines the function to call upon receiving a fastchannel message */
#define SENT_30_ICU_NOTIFICATION_FUNC_FASTCH                (Sent_FastChannel_Cbk)
/*! \brief defines the function to call upon receiving a fastchannel message */
#define SENT_30_ICU_NOTIFICATION_FUNC_SLOWCH                (Sent_SlowChannel_Cbk)
#endif


#endif /* SENT_30_ICU_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Sent_30_Icu_Cfg.h
 *********************************************************************************************************************/
