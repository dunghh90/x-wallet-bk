

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     Local clock control functions
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Ptpd.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Calculate the delay in the reverse path. */
zl303xx_BooleanE zl303xx_PtpServoCalculateDelay(
      zl303xx_ClockStreamS *clockStream,
      const char *label,
      zl303xx_TimeStamp *send_time,
      zl303xx_TimeStamp *recv_time,
      Uint64S *correctionFieldRx,
      zl303xx_TimeStamp *e2eDelay);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
extern	zl303xx_BooleanE	ptpLockedFlag;

static Uint32T			previousNanosecondsField = 0;
static Sint32T			deltaNanosecondsField[2] = {0, 0};
static zl303xx_BooleanE	dropFlag[2] = {ZL303XX_FALSE, ZL303XX_FALSE};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpUpdateDelay */
/** 

   Updates the link delay using current measurements.

  Parameters:
   [in]   clockStream      Pointer to the clock stream instance structure.
   [in]   send_time        The send time of a delay_request.
   [in]   recv_time        The receive time of the request.
   [in]   correctionField  The correction value from the delay request.
                                The correction factor is in 1/2^16 ns units.
   [in]   seqId            SequenceId associated with the time stamps.
   [in]   srcPort          Port Identity that sent the Delay message.

*******************************************************************************/
void zl303xx_PtpUpdateDelay(zl303xx_ClockStreamS *clockStream, zl303xx_TimeStamp *send_time,
                            zl303xx_TimeStamp *recv_time, Uint64S *correctionField,
                            Uint16T seqId, zl303xx_PortIdentity *srcPort)
{
/***** Added code to drop duplicate T4 TS	*****/
	if (previousNanosecondsField == recv_time->nanosecondsField) {

		ZL303XX_TRACE_ALWAYS( "Dropped packet Stream %u: Delay msg rx "
							  PTPD_FORMAT_TIME_STR,
							  clockStream->clockStreamHandle,
							  formatTimeStringSign(recv_time),
							  formatTimeStringSec(recv_time),
							  formatTimeStringNs(recv_time), 0,0 );
		return;
	}
	previousNanosecondsField = recv_time->nanosecondsField;
/***** End of code to drop duplicate T4 TS	*****/

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "Stream %u: updateDelay",
                 clockStream->clockStreamHandle,0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3, "Stream %u: updateDelay",
                 clockStream->clockStreamHandle,0,0,0,0,0);

   /* Process the timestamp information */
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Delay msg tx "
         PTPD_FORMAT_TIME_STR, clockStream->clockStreamHandle,
                               formatTimeStringSign(send_time),
                               formatTimeStringSec(send_time),
                               formatTimeStringNs(send_time), 0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Delay msg rx "
         PTPD_FORMAT_TIME_STR, clockStream->clockStreamHandle,
                               formatTimeStringSign(recv_time),
                               formatTimeStringSec(recv_time),
                               formatTimeStringNs(recv_time), 0,0);

/***** Added code to drop duplicate [ (T4 - T3) < (prevNs - 200ns) ] or [ (prevNs + 200ns) < (T4 - T3) ]	*****/
	Sint32T			delta_ns;

	long long		recvSec64;
	long long		sendSec64;
	long long		deltaSec64;

	/* Check of PTP Status	*/
	if( ptpLockedFlag == ZL303XX_TRUE ) {	/* PHASE_LOCKED_ACCURED	*/

		recvSec64 = (long long)(((long long)(recv_time->secondsField.hi) << 32) | ((long long)(recv_time->secondsField.lo) & 0xFFFFFFFF));
		sendSec64 = (long long)(((long long)(send_time->secondsField.hi) << 32) | ((long long)(send_time->secondsField.lo) & 0xFFFFFFFF));
		deltaSec64 = recvSec64 - sendSec64;

		/* Calcurate of "T4 - T3" => "recv_time - send_time"	*/
		if( deltaSec64 == 0 ) {
			delta_ns = recv_time->nanosecondsField - send_time->nanosecondsField;
		} else if( deltaSec64 > 0 ) {
			delta_ns = (1000000000 + recv_time->nanosecondsField) - send_time->nanosecondsField;
		} else {
			delta_ns = recv_time->nanosecondsField - (1000000000 + send_time->nanosecondsField);
		}

		/* correctionField分の補正を行う */
		/* correctionFieldは64bitの値であり、2の16乗(65536)で割った値がnsとなる。小数点は誤差なので切り捨てる。 */
		long long		correct64;
		if(( correctionField->hi == 0 ) && ( correctionField->lo == 0 )){
			correct64 = 0;
		}else{
			correct64 = (long long)(((long long)(((long long)(correctionField->hi) << 32) | ((long long)(correctionField->lo) & 0xFFFFFFFF)))/65536 );
		}
		delta_ns -= correct64;

		/* First check after Changing "PHASE_LOCKED_ACCURED" State	*/
		if( deltaNanosecondsField[1] == 0 ) {	/* First time	*/

			/* Keep "T4 - T3" for "deltaNanosecondsField"	*/
			deltaNanosecondsField[1] = delta_ns;

			ZL303XX_TRACE_ALWAYS( "Stream %u: First Set, deltaNs=%d, T3.ns=%d, T4.ns=%d, correct.ns=%ld ",
								  clockStream->clockStreamHandle,
								  deltaNanosecondsField[1],
								  send_time->nanosecondsField,
								  recv_time->nanosecondsField,
								  correct64, 0);
		} else {

			/* Checking to Range of "T4 - T3"	*/
			if( (delta_ns < (deltaNanosecondsField[1] - 200)) || ((deltaNanosecondsField[1] + 200) < delta_ns) ) {

				if( dropFlag[1] != ZL303XX_TRUE ) {	/* Drop	*/

					ZL303XX_TRACE_ALWAYS( "[Error] Stream %u: Drop data, deltaNs=%d, T3.ns=%d, T4.ns=%d, correct.ns=%ld ",
										  clockStream->clockStreamHandle,
										  deltaNanosecondsField[1],
										  send_time->nanosecondsField,
										  recv_time->nanosecondsField,
										  correct64, 0);
					dropFlag[1] = ZL303XX_TRUE;
					return;
				} else {	/* Recv	*/

					ZL303XX_TRACE_ALWAYS( "[Error] Stream %u: Recv Abnormal Data, deltaNs=%d, T3.ns=%d, T4.ns=%d, correct.ns=%ld ",
										  clockStream->clockStreamHandle,
										  deltaNanosecondsField[1],
										  send_time->nanosecondsField,
										  recv_time->nanosecondsField,
										  correct64, 0);
				}
			} else {
				deltaNanosecondsField[1] = delta_ns;
				dropFlag[1] = ZL303XX_FALSE;
			}
		}
	} else {	/* not PHASE_LOCKED_ACCURED	*/

		deltaNanosecondsField[1] = 0;
		dropFlag[1] = ZL303XX_FALSE;
	}
/***** End of code to drop duplicate [ (T4 - T3) < -200ns ] or [ 200ns < (T4 - T3) ]	*****/

   /* Save this DELAY source port. It will be used to determine if the
    * offsetFromMaster value is valid when a SYNC is received. */
   clockStream->farEndCurrentDS.delaySrcPort = *srcPort;

   /* For normal operations, we expect the SYNC and DELAY sources to be the same. */
   clockStream->farEndCurrentDS.s2mValid =
            zl303xx_PtpV2EqualPortIdentities(
                  /* Current DELAY source  */ srcPort,
                  /* Previous SYNC source  */ &clockStream->farEndCurrentDS.syncSrcPort);

   /* Calculate the delay in the reverse path. */
   clockStream->farEndCurrentDS.s2mValid &= zl303xx_PtpServoCalculateDelay(
                                                   clockStream, "Slave to Master delay",
                                                   send_time, recv_time, correctionField,
                                                   &clockStream->farEndCurrentDS.slave_to_master_delay);

   /* Every time we get a reverse path packet, calculate the mean_path_delay. */
   {
      zl303xx_TimeStamp tempDelta;

      addTime(&tempDelta,
              &clockStream->farEndCurrentDS.slave_to_master_delay,
              &clockStream->farEndCurrentDS.master_to_slave_delay);
      divTime(&clockStream->farEndCurrentDS.mean_path_delay,
              &tempDelta,
              2);

      ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            clockStream->clockStreamHandle, "  Mean Path Delay     ",
            &clockStream->farEndCurrentDS.mean_path_delay);
   }

   /* Issue a message to the Servo interface with the updated stats. */
   {
      /* Servo Data structure. */
      zl303xx_PtpEventServoDataS servoData;

      if ((clockStream->portData->config.mixedMode == ZL303XX_TRUE) &&
          (clockStream->config.unicast == ZL303XX_TRUE))
      {
         /* For mixed mode, let the servo algorithm believe that DELAY time stamps
          * are coming from the multicast stream. */
         if (clockStream->portData->mcastStream != NULL)
         {
            servoData.streamHandle = clockStream->portData->mcastStream->clockStreamHandle;
            servoData.streamExtData = clockStream->portData->mcastStream->config.extData;
         }
         else
         {
            /* If the multicast stream has not been created for this mixed mode
             * port, there is no stream handle to send to the servo algorithm. */
            return;
         }
      }
      else
      {
      #if defined MUX_PTP_STREAMS_TO_APR_SLOTS
          servoData.streamHandle = clockStream->aprSlotHandle;
      #else
          servoData.streamHandle = clockStream->clockStreamHandle;
      #endif
          servoData.streamExtData = clockStream->config.extData;
      }

      servoData.txTs            = *send_time;
      servoData.rxTs            = *recv_time;
      servoData.correctionField = *correctionField;
      servoData.tsPairType      = ZL303XX_PTP_TS_PAIR_DELAY;
      servoData.sequenceId      = seqId;

      servoData.offsetFromMasterValid = (clockStream->farEndCurrentDS.s2mValid &&
                                         clockStream->farEndCurrentDS.m2sValid);
      servoData.offsetFromMaster      = clockStream->farEndCurrentDS.offset_from_master;
      servoData.meanPathDelayValid    = servoData.offsetFromMasterValid;
      servoData.meanPathDelay         = clockStream->farEndCurrentDS.mean_path_delay;

      if (clockStream->portData->clock->config.eventCallback != NULL)
      {
         clockStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_SERVO_DATA,
                                                               &servoData);
      }

      zl303xx_PtpStreamTsLogSend(clockStream, &servoData);
   }
}

/* zl303xx_PtpUpdateOffset */
/** 

   Updates the offset from master value for a remote master.

  Parameters:
   [in]   clockStream      Pointer to the clock stream instance structure.
   [in]   send_time        The transmit time of a sync message.
   [in]   recv_time        The receive time of the sync message.
   [in]   correctionField  PTP correction factor from the sync message.
                                 The correction factor is in 1/2^16 ns units.
   [in]   seqId            SequenceId associated with the time stamps.
   [in]   srcPort          Port Identity that sent the Delay message.

*******************************************************************************/
void zl303xx_PtpUpdateOffset(zl303xx_ClockStreamS *clockStream, zl303xx_TimeStamp *send_time,
                             zl303xx_TimeStamp *recv_time, Uint64S *correctionField,
                             Uint16T seqId, zl303xx_PortIdentity *srcPort)
{
   zl303xx_BooleanE offsetFromMasterValid = ZL303XX_TRUE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "Stream %u: updateOffset",
                 clockStream->clockStreamHandle, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3, "Stream %u: updateOffset",
                 clockStream->clockStreamHandle, 0,0,0,0,0);

   /* Process the timestamp information */
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Sync msg tx "
         PTPD_FORMAT_TIME_STR, clockStream->clockStreamHandle,
                               formatTimeStringSign(send_time),
                               formatTimeStringSec(send_time),
                               formatTimeStringNs(send_time), 0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Sync msg rx "
         PTPD_FORMAT_TIME_STR, clockStream->clockStreamHandle,
                               formatTimeStringSign(recv_time),
                               formatTimeStringSec(recv_time),
                               formatTimeStringNs(recv_time), 0,0);

   /* Save this SYNC source port. */
   clockStream->farEndCurrentDS.syncSrcPort = *srcPort;

   /* Calculate the delay and the offset */
   /* calc 'master_to_slave_delay' */
   /* According to the standard:
    *    For 1-step:: offset_from_master = <T2> - <T1> - <meanPathDelay> - correctionField of Sync message.
    *    For 2-step:: offset_from_master = <T2> - <T1> - <meanPathDelay>
    *                                      - correctionField of Sync message
    *                                      - correctionField of Follow_Up message.
    *
    * Since the correctionField value supplied to us in this routine is:
    *    For 1-step:: = correctionField of Sync message.
    *    For 2-step:: = correctionField of Sync message + correctionField of Follow_Up message.
    * the calculation is the same for both scenarios:
    *   E2E: offset_from_master = <T2> - <T1> - <meanPathDelay> - correctionField (supplied).
    *   P2P: offset_from_master = <T2> - <T1> - <peerMeanPathDelay> - correctionField (supplied). */

/***** Added code to drop duplicate [ (T2 - T1) < (prevNs - 200ns) ] or [ (prevNs + 200ns) < (T2 - T1) ]	*****/
	Sint32T		delta_ns;

	long long		recvSec64;
	long long		sendSec64;
	long long		deltaSec64;

	/* Check of PTP Status	*/
	if( ptpLockedFlag == ZL303XX_TRUE ) {	/* PHASE_LOCKED_ACCURED	*/

		recvSec64 = (long long)(((long long)(recv_time->secondsField.hi) << 32) | ((long long)(recv_time->secondsField.lo) & 0xFFFFFFFF));
		sendSec64 = (long long)(((long long)(send_time->secondsField.hi) << 32) | ((long long)(send_time->secondsField.lo) & 0xFFFFFFFF));
		deltaSec64 = recvSec64 - sendSec64;

		/* Calcurate of "T2 - T1" => "recv_time - send_time"	*/
		if( deltaSec64 == 0 ) {
			delta_ns = recv_time->nanosecondsField - send_time->nanosecondsField;
		} else if( deltaSec64 > 0 ) {
			delta_ns = (1000000000 + recv_time->nanosecondsField) - send_time->nanosecondsField;
		} else {
			delta_ns = recv_time->nanosecondsField - (1000000000 + send_time->nanosecondsField);
		}

		/* correctionField分の補正を行う */
		/* correctionFieldは64bitの値であり、2の16乗(65536)で割った値がnsとなる。小数点は誤差なので切り捨てる。 */
		long long		correct64;
		if(( correctionField->hi == 0 ) && ( correctionField->lo == 0 )){
			correct64 = 0;
		}else{
			correct64 = (long long)(((long long)(((long long)(correctionField->hi) << 32) | ((long long)(correctionField->lo) & 0xFFFFFFFF)))/65536 );
		}
		delta_ns -= correct64;

		/* First check after Changing "PHASE_LOCKED_ACCURED" State	*/
		if( deltaNanosecondsField[0] == 0 ) {	/* First time	*/

			/* Keep "T2 - T1" for "deltaNanosecondsField"	*/
			deltaNanosecondsField[0] = delta_ns;

			ZL303XX_TRACE_ALWAYS( "Stream %u: First Set, deltaNs=%d, T1.ns=%d, T2.ns=%d, correct.ns=%ld ",
								  clockStream->clockStreamHandle,
								  deltaNanosecondsField[0],
								  send_time->nanosecondsField,
								  recv_time->nanosecondsField,
								  correct64, 0);
		} else {
			/* Checking to Range of "T2 - T1"	*/
			if( (delta_ns < (deltaNanosecondsField[0] - 200)) || ((deltaNanosecondsField[0] + 200) < delta_ns) ) {

				if( dropFlag[0] != ZL303XX_TRUE ) {	/* Drop	*/

					ZL303XX_TRACE_ALWAYS( "[Error] Stream %u: Drop data, deltaNs=%d, T1.ns=%d, T2.ns=%d, correct.ns=%ld",
										  clockStream->clockStreamHandle,
										  deltaNanosecondsField[0],
										  send_time->nanosecondsField,
										  recv_time->nanosecondsField,
										  correct64, 0);
					dropFlag[0] = ZL303XX_TRUE;
					return;
				} else {	/* Recv	*/

					ZL303XX_TRACE_ALWAYS( "[Error] Stream %u: Recv Abnormal Data, deltaNs=%d, T1.ns=%d, T2.ns=%d, correct.ns=%ld",
										  clockStream->clockStreamHandle,
										  deltaNanosecondsField[0],
										  send_time->nanosecondsField,
										  recv_time->nanosecondsField,
										  correct64, 0);
				}
			} else {
				deltaNanosecondsField[0] = delta_ns;
				dropFlag[0] = ZL303XX_FALSE;
			}
		}
	} else {	/* not PHASE_LOCKED_ACCURED	*/

		deltaNanosecondsField[0] = 0;
		dropFlag[0] = ZL303XX_FALSE;
	}
/***** End of code to drop duplicate [ (T2 - T1) < -200ns ] or [ 200ns < (T2 - T1) ]	*****/

   /* Calculate the delay in the forward path. */
   clockStream->farEndCurrentDS.m2sValid =zl303xx_PtpServoCalculateDelay(
                                                clockStream, "  Master to slave delay  ",
                                                send_time, recv_time, correctionField,
                                                &clockStream->farEndCurrentDS.master_to_slave_delay);

   /* update 'offset_from_master' */
   {
      if (clockStream->portData->portDS.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
      {
         zl303xx_TimeStamp peerDelay = {{0,0},0};

         zl303xx_PtpConvTimeIntervalToTimeStamp(&clockStream->portData->portDS.peerMeanPathDelay, &peerDelay);
         subTime(&clockStream->farEndCurrentDS.offset_from_master,
                 &clockStream->farEndCurrentDS.master_to_slave_delay,
                 &peerDelay);

         /* We have received a valid SYNC in the forward path so mark this direction valid. */
         clockStream->farEndCurrentDS.m2sValid &= ZL303XX_TRUE;

         /* peerMeanPathDelay is 0 until a Peer-Delay message is processed. */
         if ((clockStream->portData->portDS.peerMeanPathDelay.scaledNanoseconds.hi == 0) &&
             (clockStream->portData->portDS.peerMeanPathDelay.scaledNanoseconds.lo == 0))
         {
            offsetFromMasterValid = ZL303XX_FALSE;
         }
         else
         {
            offsetFromMasterValid = ZL303XX_TRUE;
         }
      }
      else if (clockStream->portData->portDS.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
      {
         subTime(&clockStream->farEndCurrentDS.offset_from_master,
                 &clockStream->farEndCurrentDS.master_to_slave_delay,
                 &clockStream->farEndCurrentDS.mean_path_delay);

         /* For normal operations, we expect the SYNC and DELAY sources to be the same. */
         clockStream->farEndCurrentDS.m2sValid &=
            zl303xx_PtpV2EqualPortIdentities(
                        /* Current SYNC source   */ srcPort,
                        /* Previous DELAY source */ &clockStream->farEndCurrentDS.delaySrcPort);

         /* Valid if both directions were valid. */
         offsetFromMasterValid = (clockStream->farEndCurrentDS.m2sValid &&
                                  clockStream->farEndCurrentDS.s2mValid);
      }

      ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            clockStream->clockStreamHandle, "  Raw offset from master ",
            &clockStream->farEndCurrentDS.offset_from_master);
   }

   /* Issue a message to the Servo interface with the updated stats. */
   {
      /* Servo Data structure. */
      zl303xx_PtpEventServoDataS servoData;

   #if defined MUX_PTP_STREAMS_TO_APR_SLOTS
      servoData.streamHandle = clockStream->aprSlotHandle;
   #else
      servoData.streamHandle = clockStream->clockStreamHandle;
   #endif
      servoData.streamExtData         = clockStream->config.extData;
      servoData.txTs                  = *send_time;
      servoData.rxTs                  = *recv_time;
      servoData.correctionField       = *correctionField;
      servoData.peerMeanPathDelay     = clockStream->portData->portDS.peerMeanPathDelay.scaledNanoseconds;
      servoData.sequenceId            = seqId;
      servoData.offsetFromMasterValid = offsetFromMasterValid;
      servoData.offsetFromMaster      = clockStream->farEndCurrentDS.offset_from_master;

      if (clockStream->portData->portDS.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
      {
         servoData.tsPairType = ZL303XX_PTP_TS_PAIR_SYNC_PEER;
         servoData.meanPathDelayValid = ZL303XX_FALSE;
      }
      else
      {
         servoData.tsPairType = ZL303XX_PTP_TS_PAIR_SYNC;
         servoData.meanPathDelayValid = offsetFromMasterValid;
         servoData.meanPathDelay = clockStream->farEndCurrentDS.mean_path_delay;
      }


      if (clockStream->portData->clock->config.eventCallback != NULL)
      {
         clockStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_SERVO_DATA,
                                                               &servoData);
      }

      zl303xx_PtpStreamTsLogSend(clockStream, &servoData);
   }


}

/* zl303xx_PtpUpdatePeerDelay */
/** 

   Updates the peer delay value for the specified link.

  Parameters:
   [in]   clockStream      Pointer to the clock stream instance structure.
   [in]   send_time        The transmit time of a Delay Request message.
   [in]   recv_time        The receive time of the Delay Response message.
   [in]   correctionField  PTP correction factor from the sync message.
                                 The correction factor is in 1/2^16 ns units.
   [in]   seqId            SequenceId associated with the time stamps.

*******************************************************************************/
void zl303xx_PtpUpdatePeerDelay(zl303xx_ClockStreamS *clockStream,
                                zl303xx_TimeStamp *send_time,   /* PReq : T1 */
                                zl303xx_TimeStamp *recv_time,   /* PResp: T4 */
                                Uint64S * correctionField,    /* From the RX PResp or FollowUp message. */
                                Uint16T seqId)
{
   /* Eliminate warning. */
   if (seqId) {;}

   /* Process the timestamp information */
   ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         clockStream->clockStreamHandle, "updatePeerDelay-Req msg tx ",
         send_time);
   ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         clockStream->clockStreamHandle, "updatePeerDelay-Resp msg rx",
         recv_time);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "Correction: 0x%08X:%08X", correctionField->hi, correctionField->lo, 0,0,0,0);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "Stream %u: updatePeerDelay",
                 clockStream->clockStreamHandle, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2, "Stream %u: updatePeerDelay",
                 clockStream->clockStreamHandle, 0,0,0,0,0);


   /* Determine peerPathDelay */
   {
      zl303xx_TimeStamp deltaTs          = {{0,0},0};   /* T4-T1: as a TimeStamp */
      zl303xx_TimeInterval deltaInterval = {{0,0}};     /* T4-T1: as a TimeInterval */
      zl303xx_TimeInterval corrField = {{0,0}};
      zl303xx_TimeInterval pathDelay = {{0,0}};

      /* Determine T4-T1 as a Client Time Interval. */
      /**********************************************/
      /* If the seconds portion of the RX time is 0 then assume only 30-bit NS are present. */
      if (recv_time->secondsField.lo == 0)
      {
         deltaTs.nanosecondsField = (((recv_time->nanosecondsField + 1000000000) - send_time->nanosecondsField) % 1000000000);
      }
      else
      {
         deltaTs.secondsField.lo = recv_time->secondsField.lo - send_time->secondsField.lo;
         deltaTs.nanosecondsField = recv_time->nanosecondsField - send_time->nanosecondsField;

         /* Check for -ve nanoseconds. */
         /* Since ns only goes to 1 billion, anything larger is actually negative. */
         while ((deltaTs.nanosecondsField >= 1000000000) &&
                (deltaTs.secondsField.lo > 0))
         {
            --deltaTs.secondsField.lo;
            deltaTs.nanosecondsField += 1000000000;
         }
      }

      /* Convert the delta to a TimeInterval */
      zl303xx_PtpConvTimeStampToTimeInterval(&deltaTs, &deltaInterval);

      /* Make sure the local and far end deltas are a positive value; preferably
       * non-zero. */
      {
         Sint8T localSignage = 0;
         Sint8T farEndSignage = 0;

         corrField.scaledNanoseconds = *correctionField;
         localSignage = zl303xx_PtpTimeIntervalSignage(deltaInterval);
         farEndSignage = zl303xx_PtpTimeIntervalSignage(corrField);

         /* The local delta must be positive since no correctionField is involved. */
         if (localSignage != PTP_TIME_INTERVAL_POSITIVE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Stream %u: updatePeerDelay : Local T4-T1 value invalid (0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  deltaInterval.scaledNanoseconds.hi,
                  deltaInterval.scaledNanoseconds.lo, 0,0,0);
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "Stream %u: updatePeerDelay : Local T4-T1 value invalid (0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  deltaInterval.scaledNanoseconds.hi,
                  deltaInterval.scaledNanoseconds.lo, 0,0,0);

            /* Just dump. */
            return;
         }

         /* The far end delta is expected to be positive but with
          * correctionField values inserted, it may be negative or 0 as well. */
         if ((farEndSignage == PTP_TIME_INTERVAL_OVERFLOW) ||
             (farEndSignage == PTP_TIME_INTERVAL_UNDERFLOW))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Stream %u: updatePeerDelay : Far End T3-T2 value invalid (0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  correctionField->hi, correctionField->lo, 0,0,0);
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "Stream %u: updatePeerDelay : Far End T3-T2 value invalid (0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  correctionField->hi, correctionField->lo, 0,0,0);

            /* Just dump. */
            return;
         }
         else if ((farEndSignage == PTP_TIME_INTERVAL_ZERO) ||
                  (farEndSignage == PTP_TIME_INTERVAL_NEGATIVE))
         {
            /* Log but keep */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Stream %u: updatePeerDelay : Far End T3-T2 value warning Corr=(0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  correctionField->hi, correctionField->lo, 0,0,0);
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "Stream %u: updatePeerDelay : Far End T3-T2 value warning Corr=(0x%08X:%08X)",
                  clockStream->clockStreamHandle,
                  correctionField->hi, correctionField->lo, 0,0,0);
         }
      }

      /* Subtract the far-end resident time. */
      pathDelay = zl303xx_PtpTimeIntervalDiff(deltaInterval, corrField);
      /* Divide by 2 */
      pathDelay = zl303xx_PtpTimeIntervalRShift(pathDelay, 1);

      /* Assign this to the Port's value. */
      clockStream->portData->portDS.peerMeanPathDelay = pathDelay;

      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2)
      {
         /* Get rid of the fractional nanoseconds for display purposes. */
         Uint64S pmpdNS = RShift_U64S(pathDelay.scaledNanoseconds, 16);

         ZL303XX_TRACE_ALWAYS(
               "Stream %3u: peerMeanPathDelay (NS):  seqId=%5u: 0x%08X:%08X  (%10u:%10u)",
               clockStream->clockStreamHandle, seqId,
               pmpdNS.hi, pmpdNS.lo, pmpdNS.hi, pmpdNS.lo);
      }
   }
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/


zl303xx_BooleanE zl303xx_PtpServoCalculateDelay(
      zl303xx_ClockStreamS *clockStream,
      const char *label,
      zl303xx_TimeStamp *send_time,
      zl303xx_TimeStamp *recv_time,
      Uint64S *correctionFieldRx,
      zl303xx_TimeStamp *e2eDelay)
{
   zl303xx_TimeStamp tempDelay = {{0, 0}, 0};
   zl303xx_TimeStamp correctionTs = {{0, 0}, 0};
   zl303xx_TimeInterval correctionInterval = {{ 0, 0}};

   zl303xx_BooleanE calculationValid = ZL303XX_TRUE;

   /* First, convert the correctionField from Interval format to Timestamp format. */
   correctionInterval.scaledNanoseconds = *correctionFieldRx;

   if(zl303xx_PtpConvTimeIntervalToTimeStamp(&correctionInterval, &correctionTs) != ZL303XX_OK)
   {
      /* The conversion of the received correctionField failed for some reason.
       * Log a message & set the value to 0.   */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Unable to convert correctionField (%08lX:%08lX)",
            label, correctionFieldRx->hi, correctionFieldRx->lo, 0,0,0);

      correctionTs.secondsField.hi = 0;
      correctionTs.secondsField.lo = 0;
      correctionTs.nanosecondsField = 0;

      /* This may indicate a problem so mark the offsetFromMaster value as invalid. */
      calculationValid = ZL303XX_FALSE;
   }

   subTime(&tempDelay, recv_time, send_time);
   subTime(e2eDelay, &tempDelay, &correctionTs);

   ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         clockStream->clockStreamHandle, label,
         e2eDelay);

   return calculationValid;
}


/*****************   END   ****************************************************/
