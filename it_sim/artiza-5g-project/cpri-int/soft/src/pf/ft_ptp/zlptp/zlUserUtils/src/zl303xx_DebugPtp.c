


/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains miscellaneous debug functions for PTP.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpStrings.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpInternal.h"  /* For ZL303XX_CHECK_POINTER() */

#include "zl303xx_DebugPtp.h"
#include "zl303xx_PtpV2AcceptMaster.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpV2SignalTlvParse.h"
#include "zl303xx_PtpProfileDef.h"

#include <string.h>



/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXTERNAL GLOBAL VARIABLES   ******************************/

extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   zl303xx_DebugClockDataSet

  Details:
   Displays clock data set statistics for the specified Clock

  Parameters:
   [in]
   clockHandle    Handle to the PTP clock for which the data sets are required.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugClockDataSet(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;

    /* Pointer to the targeted clock. */
    zl303xx_PtpClockS * pPtpClock = NULL;

    /* Clock Data Sets. */
    zl303xx_DefaultDSInternal * defaultDSImp = NULL;     /* Implementation Default Data Set. */
    zl303xx_DefaultDS * defaultDS = NULL;                /* 1588 Standard Default Data Set. */
    zl303xx_CurrentDSInternal * currentDS = NULL;
    zl303xx_ParentDS * clockParentDS = NULL;

    /* The reference properties which may be for the recovered packet stream
     * OR for the local electrical reference (depends on the mode, streams, etc.  */
    zl303xx_TimePropertiesDS * timeDS = NULL;

    /* The time properties for the local electrical clock (depends on if inputs
     * are available, clock is free-running/holdover, etc.  */
    zl303xx_TimePropertiesDS * localTimeProperties = NULL;

    Uint16T ptpVersion = ZL303XX_PTP_VERSION_2;
    Uint16T counter = 0;


    printf ("\n-------- Clock data set --------\n");

    if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK )
    {
        printf ("zl303xx_DebugClockDataSet(): Call to zl303xx_PtpClockDataGet() failed!\n");
    }

    if (status == ZL303XX_OK)
    {
        defaultDS           = &(pPtpClock->config.defaultDS);
        defaultDSImp        = &(pPtpClock->defaultDSInt);
        currentDS           = &(pPtpClock->currentDSInt);
        clockParentDS       = &(pPtpClock->parentDS);
        timeDS              = &(pPtpClock->timePropertiesDS);
        localTimeProperties = &(pPtpClock->config.localTimeProperties);

        status = ZL303XX_CHECK_POINTER(defaultDS) |
                 ZL303XX_CHECK_POINTER(defaultDSImp) |
                 ZL303XX_CHECK_POINTER(currentDS) |
                 ZL303XX_CHECK_POINTER(clockParentDS) |
                 ZL303XX_CHECK_POINTER(timeDS) |
                 ZL303XX_CHECK_POINTER(localTimeProperties);
    }

    if (status == ZL303XX_OK)
    {
        switch (ptpVersion)
        {
        case ZL303XX_PTP_VERSION_2:
            printf ("PTP version 2\n");
            if (pPtpClock->profile != NULL)
            {
               printf ("Profile  : %s\n", pPtpClock->profile->profileName);
            }
            else
            {
               printf ("Profile  : (Unspecified)\n");
            }

            /* Clock Type */
            printf ("clockType: 0x%04X\n", pPtpClock->clockType);

            /* default data set */
            printf ("Default data set information:\n");
            printf ("Two Step Clock: %s\n", ZL303XX_BOOL_TO_STR(defaultDS->twoStepFlag));
            printf ("Clock identity: ");
            for (counter = 0; counter < PTP_V2_CLK_IDENTITY_LENGTH; counter ++)
            {
                printf ("%02X:", defaultDS->clockIdentity[counter]);
            }
            printf ("\nConfigured maximum number of PTP ports on device: %u\n", defaultDS->numberPorts);

            printf ("Clock Quality Class: %u\n", defaultDS->clockQuality.clockClass);
            printf ("Clock Quality Accuracy: 0x%02X\n", defaultDS->clockQuality.clockAccuracy);
            printf ("Clock Quality Offset Scaled Log Variance: 0x%04X\n", defaultDS->clockQuality.offsetScaledLogVariance);

            printf ("Priority 1: %u\n", defaultDS->priority1);
            printf ("Priority 2: %u\n", defaultDS->priority2);

            printf("Domain Number: %u\n", defaultDS->domainNumber);

            printf ("\nSlave Only: %s\n", ZL303XX_BOOL_TO_STR(defaultDS->slaveOnly));
            printf ("\nRevertive BMCA: %s\n", ZL303XX_BOOL_TO_STR(pPtpClock->config.bmca.revertiveEn));
            printf ("\nBMCA Update Trigger: %d", pPtpClock->config.bmca.updateTrigger);
            printf ("\nAuto BMCA Update: %s (%d seconds)\n", ZL303XX_BOOL_TO_STR(pPtpClock->config.bmca.updateEventSec != 0), pPtpClock->config.bmca.updateEventSec);
			{
				zl303xx_BooleanE flagStatus;
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,STATE_FM_QUALIFICATION_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA Foreign Master Qualification flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}
				
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,PD_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA PD flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}
				
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,PF_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA PF flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}
				
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,PL_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA PL flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}
				
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,PP_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA PP flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}
				
				if (zl303xx_PtpClockBmcaFlagStatusGet(clockHandle,PU_FLAG,&flagStatus) == ZL303XX_OK)
				{
					printf ("\nBMCA PU flag: %s\n", ZL303XX_BOOL_TO_STR(flagStatus));
				}	
			}
            printf ("\nWaitToRestore Sec: %u\n", (pPtpClock->config.telecom.waitToRestoreSec));
            printf ("\nStream State Update Mode: %s\n", (pPtpClock->config.autoUpdateStreamStates) ? "AUTO" : "MANUAL");
            printf ("\nMaximum Steps Removed: %u\n", (pPtpClock->config.optionalDefaultDS.maxStepsRemoved));

            printf ("\nPATH TRACE Enabled: %u\n", (pPtpClock->config.pathTrace.enabled));
            if (pPtpClock->config.pathTrace.enabled == ZL303XX_TRUE)
            {
               (void)zl303xx_PtpClockPathTraceListShow(&pPtpClock->pathTraceList);
            }
            printf ("\nSynchronization Uncertain\n");
            printf ("                    enabled: %u\n",
                           (pPtpClock->config.synchronizationUncertain.enabled));

            /* Ingress/Egress clockClass configuration.  */
            printf ("\nClock Ingress/Egress clockClass (QL) over-ride configuration:\n");
            printf (" Ingress clockClass over-ride: %d %s\n",
                    pPtpClock->config.ingressClockClass,
                    ((pPtpClock->config.ingressClockClass == 0) ? ":DISABLED" : " "));
            printf ("  Egress clockClass over-ride: %d %s\n",
                    pPtpClock->config.egressClockClass,
                    ((pPtpClock->config.egressClockClass == 0) ? ":DISABLED" : " "));
            printf ("     Egress clockClass update: %d (UPDATE WHEN %s)\n",
                    pPtpClock->config.egressClassUpdateTrigger,
                    ((pPtpClock->config.egressClassUpdateTrigger == ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY) ? "QUALIFIED" : "LOCKED"));

            /* Hardware Time Properties information */
            printf ("\nLocal Hardware Time Properties:\n");
            printf ("   Current UTC Offset: %d\n",       localTimeProperties->currentUtcOffset);
            printf ("   Current UTC Offset valid: %s\n", ZL303XX_BOOL_TO_STR(localTimeProperties->currentUtcOffsetValid));
            printf ("   Leap 59: %s\n",                  ZL303XX_BOOL_TO_STR(localTimeProperties->leap59));
            printf ("   Leap 61: %s\n",                  ZL303XX_BOOL_TO_STR(localTimeProperties->leap61));
            printf ("   Time Tracable: %s\n",            ZL303XX_BOOL_TO_STR(localTimeProperties->timeTraceable));
            printf ("   Frequency Tracable: %s\n",       ZL303XX_BOOL_TO_STR(localTimeProperties->frequencyTraceable));
            printf ("   PTP timescale: %s\n",            ZL303XX_BOOL_TO_STR(localTimeProperties->ptpTimescale));
            printf ("   Synchronization Uncertain: %s\n",ZL303XX_BOOL_TO_STR(localTimeProperties->synchronizationUncertain));
            printf ("   Time Source: %d\n",              localTimeProperties->timeSource);

            /* PTP Clock Time Properties information */
            printf ("Time data set information:\n");
            printf ("   Current UTC Offset: %d\n",       timeDS->currentUtcOffset);
            printf ("   Current UTC Offset valid: %s\n", ZL303XX_BOOL_TO_STR(timeDS->currentUtcOffsetValid));
            printf ("   Leap 59: %s\n",                  ZL303XX_BOOL_TO_STR(timeDS->leap59));
            printf ("   Leap 61: %s\n",                  ZL303XX_BOOL_TO_STR(timeDS->leap61));
            printf ("   Time tracable: %s\n",            ZL303XX_BOOL_TO_STR(timeDS->timeTraceable));
            printf ("   Frequency tracable: %s\n",       ZL303XX_BOOL_TO_STR(timeDS->frequencyTraceable));
            printf ("   PTP timescale: %s\n",            ZL303XX_BOOL_TO_STR(timeDS->ptpTimescale));
            printf ("   Synchronization Uncertain: %s\n",ZL303XX_BOOL_TO_STR(timeDS->synchronizationUncertain));
            printf ("   Time source: %d\n\n",            timeDS->timeSource);

            /* current dataset */
            printf ("Current data set information:\n");
            printf ("----------------------------\n");
            printf ("Steps removed     : %u\n", currentDS->stepsRemoved);
            printf ("Offset from master: " PTPD_FORMAT_TIME_STR "\n",
                    formatTimeStringSign(&currentDS->offset_from_master),
                    formatTimeStringSec(&currentDS->offset_from_master),
                    formatTimeStringNs(&currentDS->offset_from_master));
            printf ("Mean path delay   : " PTPD_FORMAT_TIME_STR "\n",
                    formatTimeStringSign(&currentDS->mean_path_delay),
                    formatTimeStringSec(&currentDS->mean_path_delay),
                    formatTimeStringNs(&currentDS->mean_path_delay));

            /* parent dataset */
            printf ("\nParent data set information:");
            printf ("\n----------------------------");
            printf ("\nGrandmaster stream handle: ");
            if (pPtpClock->gmSource.pPtpStream == NULL)
            {
                printf("NULL\n");
            }
            else
            {
                printf("%u\n", pPtpClock->gmSource.pPtpStream->clockStreamHandle);
            }

            printf ("Grandmaster Source: ");
            if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_CLOCK)
            {
                printf("LOCAL CLOCK\n");
            }
            else if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM)
            {
                printf("LOCAL STREAM\n");
            }
            else if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT)
            {
                printf("LOCAL VIRTUAL PORT\n");
            }
            else if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL)
            {
                printf("EXTERNAL\n");
            }

            /* External PATH-TRACE data. */
            if (pPtpClock->config.pathTrace.enabled == ZL303XX_TRUE)
            {
               (void)zl303xx_PtpClockPathTraceListShow(&pPtpClock->gmSource.extPathTraceList);
            }

            /* Clock UNCALIBRATED configuration*/
            printf ("UNCALIBRATED behaviour:\n");
            printf ("  Use Previous Class: %d (%s)\n",
                  pPtpClock->config.uncalibrated.usePreviousClass,
                  (pPtpClock->config.uncalibrated.usePreviousClass) ? "TRUE" : "FALSE");
            printf ("      Previous Class: %d\n",
                  pPtpClock->uncalibrated.previousClockClass);

            /* Egress Override configuration. */
            (void)zl303xx_PtpClockEgressOverrideShow(clockHandle);

            /* Current ParentDS */
            zl303xx_PtpTraceParentDataSet(clockParentDS);

            /* Timers */
            zl303xx_PtpTimerListShow(clockHandle);

            /* Profile specific Clock Parameters */
            zl303xx_PtpProfileClockDataShow(pPtpClock);

            break;
        default:
            printf ("ERROR: MODE NOT SUPPORTED!");
            break;
        }
    }

    return status;
}

/**

  Function Name:
   zl303xx_DebugPortDataSet

  Details:
   Displays port data set statistics for the specified Clock

  Parameters:
   [in]
   clockPortHandle  Handle to the clock port of interest as returned
                     by the zl303xx_CreatePtpClockStream() function

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPortDataSet(zl303xx_PtpPortHandleT clockPortHandle)
{
    zlStatusE status = ZL303XX_OK;

    /* Pointers to the Stream & Port. */
    zl303xx_PortDataS    * pPortData   = NULL;

    /* zl303xx_PortConfigDataSetS * pPortConfig = NULL; */			/* ZL303XX_INCLUDE_PTP_V1時の定義 不要 */

    Uint16T counter = 0;

    printf ("\n-------- Port data set --------\n");

    if ((status = zl303xx_PtpPortDataGet(clockPortHandle, &pPortData)) != ZL303XX_OK )
    {
        printf ("zl303xx_DebugPortDataSet(): Call to zl303xx_PtpPortDataGet() failed!\n");
    }

    if ((status == ZL303XX_OK) &&
        (pPortData != NULL))
    {
        /* pPortConfig = &pPortData->portConfig; */					/* ZL303XX_INCLUDE_PTP_V1時の定義 不要 */

        switch (pPortData->portDS.versionNumber)
        {
        case 2:
            {
                printf ("PTP version 2\n");
                /* default dataset */
                printf ("Port data set information:\n");
                printf ("Port identity: ");
                {
                    for (counter = 0; counter < PTP_V2_CLK_IDENTITY_LENGTH; counter ++)
                    {
                        printf ("%02X:", pPortData->portDS.portIdentity.clockIdentity[counter]);
                    }

                    printf(" (port: 0x%04X)", (pPortData->portDS.portIdentity.portNumber & 0xFFFF));
                }

                printf ("\nPort state: ");
                printf ("PTP %s\n", zl303xx_PtpPortStateToStr(zl303xx_GetV2PortState(pPortData)));

                printf ("\nPort Type: %s Port\n", (pPortData->virtualData.isVirtualPort) ? "VIRTUAL" : "PTP");
                if (pPortData->virtualData.isVirtualPort)
                {
                   static char vpClockIdStr[32];
                   (void)zl303xx_PtpV2ClockIdentityToString(pPortData->virtualData.config.clockIdentity, vpClockIdStr);

                   printf (" Virtual Port Configuration: \n");
                   printf ("    clockIdentity: %s \n", vpClockIdStr);
                   printf ("       PRTC-Clock: %s \n", pPortData->virtualData.config.prtcConnected ? "TRUE" : "FALSE");
                   printf ("             PTSF: %s \n", pPortData->virtualData.config.ptsf ? "TRUE" : "FALSE");
                   printf ("       clockClass: %d \n", pPortData->virtualData.config.clockQuality.clockClass);
                   printf ("    clockAccuracy: 0x%02X \n", pPortData->virtualData.config.clockQuality.clockAccuracy);

                   printf ("        priority1: %d \n", pPortData->virtualData.config.priority1);
                   printf ("        priority2: %d \n", pPortData->virtualData.config.priority2);
                   printf ("    localPriority: %d \n", pPortData->virtualData.config.localPriority);

                   printf (" currentUtcOffset: %d \n", pPortData->virtualData.config.timeProperties.currentUtcOffset);
                   printf ("curUtcOffsetValid: %s \n", pPortData->virtualData.config.timeProperties.currentUtcOffsetValid ? "TRUE" : "FALSE");
                   printf ("       timeSource: %d \n", pPortData->virtualData.config.timeProperties.timeSource);
                   printf ("         Time-Set: %s \n", pPortData->virtualData.config.timeSetFlag ? "TRUE" : "FALSE");
                }
                else
                {
                   switch (pPortData->config.delayMechanism)
                   {
                   case ZL303XX_DELAY_MECHANISM_E2E:
                       printf ("Delay mechanism: end-to-end\n");
                       break;
                   case ZL303XX_DELAY_MECHANISM_P2P:
                       printf ("Delay mechanism: peer-to-peer\n");
                       printf ("Log min pdelay request interval: %d\n",
                             pPortData->portDS.logMinPdelayReqInterval);
                       {
                          Uint64S pmpdNS = RShift_U64S(pPortData->portDS.peerMeanPathDelay.scaledNanoseconds, 16);
                          printf ("Peer Mean Path Delay: 0x%08X:%08X (%u ns)\n",
                                pPortData->portDS.peerMeanPathDelay.scaledNanoseconds.hi,
                                pPortData->portDS.peerMeanPathDelay.scaledNanoseconds.lo,
                                pmpdNS.lo);
                       }
                       break;
                   case ZL303XX_DELAY_MECHANISM_DISABLED:
                       printf ("Delay mechanism: disabled\n");
                       break;
                   default:
                       break;
                   }


                   printf ("PTP version 2\n");
                   printf ("Port number: %u\n", pPortData->portDS.portIdentity.portNumber);
                   printf ("Multicast enabled: %s\n\n", ZL303XX_BOOL_TO_STR(pPortData->mcastStream != NULL)); 
				   printf ("Local port address:\n");
				   printf ("		networkProtocol: %u\n", pPortData->config.localAddr.networkProtocol);
				   printf ("		addressLength: %u\n", pPortData->config.localAddr.addressLength);
				   {
					   char localAddrStr[ZL303XX_PTP_ADDR_STR_LEN_MAX];
					   if (zl303xx_PtpConvertPortAddressToStr(&(pPortData->config.localAddr),localAddrStr) == ZL303XX_OK)
					   {
						   printf ("		addressField: %s\n",localAddrStr);
					   }
				   }

                   /* FOREIGN MASTER DATASET for this Port*/
                   printf ("Foreign master data set information:\n");
                   (void)zl303xx_DebugPtpPortFmt(clockPortHandle);

                   printf("\nUnicast Stacked-TLV setting: %s\n",
                          (pPortData->config.unicastNegotiation.stackedTlvEn) ? "ENABLED" : "DISABLED");
                   printf("Stacked-TLV on 1st Timing Request: %s\n",
                          (pPortData->clock->stackAllServicesOn1stRequest) ? "ENABLED" : "DISABLED");

                   printf("\nAcceptable Master Table Enabled: %s\n", ZL303XX_BOOL_TO_STR(pPortData->config.acceptableMasterTableEnabled));
                   printf("Acceptable Slave Table Enabled: %s\n", ZL303XX_BOOL_TO_STR(pPortData->config.acceptableSlaveTableEnabled));
                }

                /* Profile specific Port Parameters */
                zl303xx_PtpProfilePortDataShow(pPortData);

                printf("--------------------------------------------------\n");

                break;
            }

        default:
            {
                printf ("ERROR: MODE NOT SUPPORTED!");
                break;
            }
        }
    }

    return status;
}

/**

  Function Name:
   zl303xx_DebugStreamDataSet

  Details:
   Displays stream data set information for the specified Stream

  Parameters:
   [in]
   clockStreamHandle Handle to the clock stream of interest as returned
                     by the zl303xx_CreatePtpClockStream() function

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugStreamDataSet(zl303xx_PtpStreamHandleT clockStreamHandle)
{
    zlStatusE status = ZL303XX_OK;

    /* Pointer to the Stream. */
    zl303xx_ClockStreamS * pStreamData = NULL;

    printf ("\n-------- Stream data set --------\n");

    if ((status = zl303xx_PtpStreamDataGet(clockStreamHandle, &pStreamData)) != ZL303XX_OK )
    {
        printf ("zl303xx_DebugStreamDataSet(): Call to zl303xx_PtpStreamDataGet() failed!\n");
    }

    if (status == ZL303XX_OK)
    {
        printf ("\nStream state: \n");
        switch (pStreamData->streamSubstate)
        {
        case PTP_STREAM_SUBSTATE_INIT:         {printf ("Sub-State: INIT\n"); break;}
        case PTP_STREAM_SUBSTATE_IDLE:         {printf ("Sub-State: IDLE\n"); break;}
        case PTP_STREAM_SUBSTATE_PASSIVE:      {printf ("Sub-State: PASSIVE\n"); break;}
        case PTP_STREAM_SUBSTATE_LISTENING:    {printf ("Sub-State: LISTENING\n"); break;}
        case PTP_STREAM_SUBSTATE_PRE_MASTER:   {printf ("Sub-State: PRE_MASTER\n"); break;}
        case PTP_STREAM_SUBSTATE_UNCALIBRATED: {printf ("Sub-State: UNCALIBRATED\n"); break;}
        case PTP_STREAM_SUBSTATE_MASTER:       {printf ("Sub-State: MASTER\n"); break;}
        case PTP_STREAM_SUBSTATE_SLAVE:        {printf ("Sub-State: SLAVE\n"); break;}
        default:                               {printf ("Sub-State: %d - unhandled!\n", pStreamData->streamSubstate); break;}
        }

        printf("Override Mode: %s\n",
              zl303xx_PtpStreamModeToStr(pStreamData->config.mode));
        printf("OperState: %s (%d)\n",
               zl303xx_PtpStreamStateToStr(pStreamData->operState),
               pStreamData->operState);

        printf("ParentPort:%d\n", pStreamData->config.portHandle);
		printf ("Destination port address of stream connection:\n"); 
		printf ("		networkProtocol: %u\n", pStreamData->config.destAddr.networkProtocol);
		printf ("		addressLength: %u\n", pStreamData->config.destAddr.addressLength);
		{
			char destAddrStr[ZL303XX_PTP_ADDR_STR_LEN_MAX];
			if (zl303xx_PtpConvertPortAddressToStr(&(pStreamData->config.destAddr),destAddrStr) == ZL303XX_OK)
			{
			   printf ("		addressField: %s\n",destAddrStr);
			}
		}
        

        /* Delay Asymmetry. */
        printf ("Delay Asymmetry:\n");
        printf ("       ingress: 0x%08X%08X\n",
                           pStreamData->config.delayAsymmetry.ingress.scaledNanoseconds.hi,
                           pStreamData->config.delayAsymmetry.ingress.scaledNanoseconds.lo);
        printf ("        egress: 0x%08X%08X\n",
                           pStreamData->config.delayAsymmetry.egress.scaledNanoseconds.hi,
                           pStreamData->config.delayAsymmetry.egress.scaledNanoseconds.lo);

        printf("\n  Max Clock Class: %d\n", pStreamData->config.maxClockClass);
        printf("Ingress Clk Class: %d\n", pStreamData->config.ingressClockClass);
        printf(" Egress Clk Class: %d\n", pStreamData->config.egressClockClass);
        printf(" Keep Alive (Sec): %d\n", pStreamData->config.keepAliveSec);
        printf("UNCALIBRATED:\n");
        printf("      Intervals: %d\n", pStreamData->config.uncalibrated.anncIntervals);
        printf("  Lock Required: %d\n", pStreamData->config.uncalibrated.lockRequired);
        printf("UNICAST NEGOTIATION:\n");
        printf("           unicast: %s\n", (pStreamData->config.unicast == ZL303XX_TRUE) ? "TRUE" : "FALSE");
        printf("  contractDuration: %d\n", pStreamData->config.unicastNegotiationDuration);
        printf("     monitorTiming: %s\n", (pStreamData->config.unicastMonitorTiming == ZL303XX_TRUE) ? "TRUE" : "FALSE");
        printf("     renewalWindow: %d\n", pStreamData->config.unicastNegotiationRenew);

        /* current dataset */
        printf ("\nStream Current data set information:\n");
        printf ("----------------------------\n");
        printf ("Steps removed        : %u\n", pStreamData->farEndCurrentDS.stepsRemoved);
        printf ("slave-to-master delay: " PTPD_FORMAT_TIME_STR "\n",
                formatTimeStringSign(&pStreamData->farEndCurrentDS.slave_to_master_delay),
                formatTimeStringSec(&pStreamData->farEndCurrentDS.slave_to_master_delay),
                formatTimeStringNs(&pStreamData->farEndCurrentDS.slave_to_master_delay));
        printf ("master-to-slave delay: " PTPD_FORMAT_TIME_STR "\n",
                formatTimeStringSign(&pStreamData->farEndCurrentDS.master_to_slave_delay),
                formatTimeStringSec(&pStreamData->farEndCurrentDS.master_to_slave_delay),
                formatTimeStringNs(&pStreamData->farEndCurrentDS.master_to_slave_delay));
        printf ("Offset from master   : " PTPD_FORMAT_TIME_STR "\n",
                formatTimeStringSign(&pStreamData->farEndCurrentDS.offset_from_master),
                formatTimeStringSec(&pStreamData->farEndCurrentDS.offset_from_master),
                formatTimeStringNs(&pStreamData->farEndCurrentDS.offset_from_master));
        printf ("Mean path delay      : " PTPD_FORMAT_TIME_STR "\n",
                formatTimeStringSign(&pStreamData->farEndCurrentDS.mean_path_delay),
                formatTimeStringSec(&pStreamData->farEndCurrentDS.mean_path_delay),
                formatTimeStringNs(&pStreamData->farEndCurrentDS.mean_path_delay));

        /* Egress Override configuration. */
        (void)zl303xx_PtpStreamEgressOverrideShow(clockStreamHandle);

        /* Best Effort Negotiation Configuration */
        if (status == ZL303XX_OK)
        {
           zl303xx_MessageTypeE displayOrder[] = {
                 ZL303XX_MSG_ID_ANNOUNCE,
                 ZL303XX_MSG_ID_SYNC,
                 ZL303XX_MSG_ID_DELAY_RESP,
                 ZL303XX_MSG_ID_PEER_DELAY_RESP};

           zl303xx_MessageTypeE msgType;
           Uint32T count;

           printf("\nStream Message Best Effort Configuration:");
           printf("\n     Message Type       Enabled     Max Interval\n ");
           for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
           {
              msgType = displayOrder[count];
              printf("   %-19s : %-7s :    %d \n ",
                    zl303xx_PtpMessageTypeToStr(msgType),
                    pStreamData->config.unicastNegotiation.bestEffort[msgType].enabled ? "TRUE" : "FALSE",
                    pStreamData->config.unicastNegotiation.bestEffort[msgType].intervalMax);
           }
        }

        /* Unicast Retry Configuration */
        if (status == ZL303XX_OK)
        {
           printf("\nStream Unicast Retry Configuration:\n");

           if (pStreamData->config.unicastNegotiation.retry.denial.logQueryCount == (Uint32T)(-1))
           {
              printf("     DENIAL (Retries/Lockout):     FOREVER : %u (SEC)\n",
                               pStreamData->config.unicastNegotiation.retry.denial.lockoutSec);
           }
           else
           {
              printf("     DENIAL (Retries/Lockout):  %10u : %u (SEC)\n",
                    pStreamData->config.unicastNegotiation.retry.denial.logQueryCount,
                    pStreamData->config.unicastNegotiation.retry.denial.lockoutSec);
           }

           if (pStreamData->config.unicastNegotiation.retry.timeout.logQueryCount == (Uint32T)(-1))
           {
              printf("    TIMEOUT (Retries/Lockout):     FOREVER : %u (SEC)\n\n",
                    pStreamData->config.unicastNegotiation.retry.timeout.lockoutSec);
           }
           else
           {
              printf("    TIMEOUT (Retries/Lockout):  %10u : %u (SEC)\n\n",
                    pStreamData->config.unicastNegotiation.retry.timeout.logQueryCount,
                    pStreamData->config.unicastNegotiation.retry.timeout.lockoutSec);
           }
        }

        /* Message Length extension and TLV PAD bytes */
        if (status == ZL303XX_OK)
        {
           zl303xx_MessageTypeE displayOrder[] = {
                 ZL303XX_MSG_ID_SYNC,
                 ZL303XX_MSG_ID_DELAY_REQ,
                 ZL303XX_MSG_ID_PEER_DELAY_REQ,
                 ZL303XX_MSG_ID_PEER_DELAY_RESP};

           zl303xx_MessageTypeE msgType;
           Uint32T count;

           printf("\nStream Message Length extension and TLV PAD bytes:");
           printf("\n   (Padding is disabled for Message Types that are not displayed)\n");
           printf("       Message Type        PAD TLV     Extension Bytes\n ");
           for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
           {
              msgType = displayOrder[count];
              printf("   %-20s : %-7d :    %d \n ",
                    zl303xx_PtpMessageTypeToStr(msgType),
                    pStreamData->config.padTlvLen[msgType],
                    pStreamData->config.msgLenPadding[msgType]);
           }
        }

        /* Print Unicast contract Information */
        if (status == ZL303XX_OK)
        {
           zl303xx_V2UnicastNegotiatedMsgTypeE displayOrder[] = {
                 PTP_NEGOTIATED_MSG_ID_ANNOUNCE,
                 PTP_NEGOTIATED_MSG_ID_SYNC,
                 PTP_NEGOTIATED_MSG_ID_DELAY_RESP,
                 PTP_NEGOTIATED_MSG_ID_PDELAY_RESP};

           zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
           zl303xx_MessageTypeE ptpMsgType;
           Uint32T count;

           if ((pStreamData->config.unicast == ZL303XX_TRUE) &&
               (pStreamData->portData->config.unicastNegotiation.enabled == ZL303XX_TRUE))
           {
              /* Print the table header. */
              printf("\n-- Stream Contracts --| ------- Message Type Parameters ------\n");
              printf("   Stream   |  Stream | uniMsg| expiry|  log2 |    Duration   |\n");
              printf("   Handle   |  State  | state | Limit | intvl | negotd| remain|\n");
              printf("------------+---------+-------|-------|-------|-------|-------|\n");

              /* RECEIVE Contracts */
              printf("    %4u    | %4u    |       |       |       |       |       |\n",
                    pStreamData->clockStreamHandle,
                    pStreamData->streamSubstate);
              for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
              {
                 uniNegMsgType = displayOrder[count];
                 (void)zl303xx_PtpV2IndexToMessageType(uniNegMsgType, &ptpMsgType);

                 if (pStreamData->contractRx[uniNegMsgType].remainingSec > 0)
                 {
                    printf(" %-17s(RX)| %4d  | %4u  | %4d  | %4u  | %4u  |\n",
                          zl303xx_PtpMessageTypeToStr(ptpMsgType),
                          pStreamData->contractRx[uniNegMsgType].state,
                          pStreamData->contractRx[uniNegMsgType].renewSec,
                          pStreamData->contractRx[uniNegMsgType].grant.interval,
                          pStreamData->contractRx[uniNegMsgType].grant.durationSec,
                          pStreamData->contractRx[uniNegMsgType].remainingSec);
                 }
              }

              /* TRANSMIT Contracts */
              for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
              {
                 uniNegMsgType = displayOrder[count];
                 (void)zl303xx_PtpV2IndexToMessageType(uniNegMsgType, &ptpMsgType);

                 if (pStreamData->contractTx[uniNegMsgType].remainingSec > 0)
                 {
                    printf(" %-17s(TX)| %4d  |       | %4d  | %4u  | %4u  |\n",
                          zl303xx_PtpMessageTypeToStr(ptpMsgType),
                          pStreamData->contractTx[uniNegMsgType].state,
                          pStreamData->contractTx[uniNegMsgType].grant.interval,
                          pStreamData->contractTx[uniNegMsgType].grant.durationSec,
                          pStreamData->contractTx[uniNegMsgType].remainingSec);
                 }
              }

              printf("------------+---------|-------|-------|-------|-------|-------|\n");
           }

           /* MULTICAST */
           else
           {
              printf("\nMulticast Packet Intervals\n");
              printf("------------------------------\n");
              printf(" ANNOUNCE  | %4d\n", pStreamData->config.logAnnounceInterval);
              printf(" SYNC      | %4d\n", pStreamData->config.logSyncInterval);

              if (pStreamData->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
                 printf(" DELAY_REQ | %4d  \n", pStreamData->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct);
              else if (pStreamData->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
                 printf(" PEER_DELAY_REQ | %4d  \n", pStreamData->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct);
           }
        }

        /* Profile specific Clock Parameters */
         zl303xx_PtpProfileStreamDataShow(pStreamData);
    }

    return status;
}

/**

  Function Name:
   zl303xx_DebugStreamEgressAnncShow

  Details:
   Displays the ANNOUNCE data values that would be transmitted from this stream
   if one were to be issued right now.

  Parameters:
   [in]
   clockStreamHandle    Handle to the clock stream for which to display egress
                           ANNOUNCE data fields.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugStreamEgressAnncShow(zl303xx_PtpStreamHandleT clockStreamHandle)
{
   zlStatusE status = ZL303XX_OK;

   /* Structure for the data. */
   zl303xx_PtpV2MsgAnnounceS egressAnncData;

   printf ("\n-------- Stream Egress ANNOUNCE data fields --------\n");

   if ((status = zl303xx_PtpStreamEgressAnncDataGet(clockStreamHandle, &egressAnncData)) != ZL303XX_OK )
   {
      printf ("zl303xx_DebugStreamEgressAnncShow(): Call to zl303xx_PtpStreamEgressAnncDataGet() failed! %d \n", status);
   }
   else
   {
      static char gmClockIdStr[32];
      (void)zl303xx_PtpV2ClockIdentityToString(egressAnncData.grandmasterIdentity, gmClockIdStr);

      printf ("Stream Handle: %u \n", clockStreamHandle);
      printf ("          originTimestamp: NA \n");
      printf ("         currentUtcOffset: %d \n", egressAnncData.currentUtcOffset);
      printf ("     grandmasterPriority1: %u \n", egressAnncData.grandmasterPriority1);
      printf ("     grandmasterPriority2: %u \n", egressAnncData.grandmasterPriority2);
      printf ("  grandmasterClockQuality: %u:0x%02X:0x%04X \n",
            egressAnncData.grandmasterClockQuality.clockClass,
            egressAnncData.grandmasterClockQuality.clockAccuracy,
            egressAnncData.grandmasterClockQuality.offsetScaledLogVariance);
      printf ("      grandmasterIdentity: %s \n", gmClockIdStr);
      printf ("             stepsRemoved: %u \n", egressAnncData.stepsRemoved);
      printf ("               timeSource: %d \n", egressAnncData.timeSource);
   }

   return status;
}  /* END zl303xx_DebugStreamEgressAnncShow */

/**

  Function Name:
    zl303xx_DebugAllDataSet

  Details:
   Gets all valid clock streams and invokes clockDataSet and portDataSet

  Parameters:
   [in]
   None

    [out]
   Print both clock and port data set

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugAllDataSet(void)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpClockHandleT clockHandle = 0;   /* Assume clock 0 */

   zl303xx_PtpPortHandleT portIdx;
   zl303xx_PortDataS *clockPort = NULL;

   zl303xx_PtpStreamHandleT streamIdx;
   zl303xx_ClockStreamS *clockStream = NULL;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_DebugClockDataSet(clockHandle);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpFmtShow(clockHandle);
   }

   /* Show the data for all existing Ports (Packet and Virtual Ports). */
   if (status == ZL303XX_OK)
   {
      for (portIdx = 0; portIdx < ZL303XX_PTP_NUM_PORTS_MAX; portIdx++)
      {
         /* Check to see if the portHandle exists before calling the Debug
          * routine. We do this to avoid having a bunch of error messages being
          * printed for non-existent portHandles. It may just be a hole in the
          * table due to provisioning order or deleting a Port.     */
         if (ZL303XX_OK == zl303xx_PtpPortDataGet(portIdx, &clockPort))
         {
            printf ("\n\n-------- Start of PTP PORT %u -------- \n", portIdx);
            (void)zl303xx_DebugPortDataSet(portIdx);
            printf ("\n-------- End of PTP PORT %u -------- \n\n", portIdx);
         }
      }
   }

   /* Show the data for all existing Streams. */
   if (status == ZL303XX_OK)
   {
      for (streamIdx = 0; streamIdx < ZL303XX_PTP_NUM_STREAMS_MAX; streamIdx++)
      {
         /* Check to see if the streamHandle exists before calling the Debug
          * routine. Again, to avoid having a bunch of error messages being
          * printed for non-existent streamHandles.     */
         if (ZL303XX_OK == zl303xx_PtpStreamDataGet(streamIdx, &clockStream))
         {
            printf ("\n\n-------- Start of PTP STREAM %u -------- \n", streamIdx);
            (void)zl303xx_DebugStreamDataSet(streamIdx);
            zl303xx_DebugPtpMsgCountWithOptionalReset(streamIdx,ZL303XX_FALSE);
            printf ("\n-------- End of PTP STREAM %u -------- \n\n", streamIdx);
         }
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_DebugPtpClockProfileShow

  Details:
   Displays PTP Profile information for the specified Clock.
   (See IEEE-1588: Section 19.3.3).

  Parameters:
   [in]
   clockHandle    Handle to the PTP clock for which the profile is required.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpClockProfileShow(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;

    /* Pointer to the targeted clock. */
    zl303xx_PtpClockS * pPtpClock = NULL;

    printf ("\n-------- Clock[%u] PTP Profile Information --------\n", clockHandle);

    if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK )
    {
        printf ("zl303xx_DebugPtpClockProfileShow(): Call to zl303xx_PtpClockDataGet() failed!\n");
    }

    if (status == ZL303XX_OK)
    {
       if (pPtpClock->profile == NULL)
       {
          printf ("zl303xx_DebugPtpClockProfileShow(): Profile Data not configured!\n");
          status = ZL303XX_INVALID_POINTER;
       }
    }

    if (status == ZL303XX_OK)
    {
        Uint8T idByte;

        if (pPtpClock->profile != NULL)
        {
           printf ("PTP Profile\n %s\n", pPtpClock->profile->profileName);
           printf ("Version\n %s\n", pPtpClock->profile->profileVersion);
        }
        else
        {
           printf ("PTP Profile\n (Unspecified)\n");
           printf ("Version (Unknown)\n");
        }

        printf ("PTP Identifier: ");
        for (idByte = 0; idByte < 6; idByte++)
        {
            if (idByte < 5)
                printf ("%02X-", pPtpClock->profile->profileIdentifier[idByte]);
            else
                printf ("%02X\n", pPtpClock->profile->profileIdentifier[idByte]);
        }

        printf ("\n%s\n", pPtpClock->profile->organizationName);
        printf ("\n%s\n", pPtpClock->profile->sourceIdentification);
    }

    printf ("\n----------------------------------------------------\n");

    return status;
}

/* zl303xx_DebugPtpMsgCount */
/**
   Displays the PTP messages counters for the specified stream.

  Parameters:
   [in]   streamHandle   Handle to a previously created stream.

*******************************************************************************/
zlStatusE zl303xx_DebugPtpMsgCount(zl303xx_PtpStreamHandleT streamHandle)
{
   /* Reset all counters for this stream. */ 
   return zl303xx_DebugPtpMsgCountWithOptionalReset(streamHandle,ZL303XX_TRUE);
}


/* zl303xx_DebugPtpMsgCountWithOptionalReset */
/**
   Displays the PTP messages counters for the specified stream with added option of whether
   to reset counters.

  Parameters:
   [in]  -streamHandle   Handle to a previously created stream.
            -reset          Choice of whether to reset counters

*******************************************************************************/
zlStatusE zl303xx_DebugPtpMsgCountWithOptionalReset(zl303xx_PtpStreamHandleT streamHandle,zl303xx_BooleanE reset)
{
    zlStatusE status = ZL303XX_OK;

    Uint4T msgType;
    Uint32T tempList;
    zl303xx_PtpStreamCountersS streamCounters;

    /* Create a list of all the message types to print counters for. */
    /* Since we support all message types just assign the VALID mask. */
    const Uint32T msgTypeList = ZL303XX_PTP_V2_VALID_MESSAGE_TYPE_MASK;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpStreamCountersGet(streamHandle, &streamCounters);
    }

    if (status == ZL303XX_OK)
    {
        /* Print RX message counters. */
        printf("\nPTP Ingress Message Count for Stream: %u\n", streamHandle);
        printf("%21s   RX          Dropped     Lost\n", "");

        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (ZL303XX_BIT_TEST(msgTypeList, msgType))  /* Only print counters in the list. */
            {
                printf("%-21s : %-12u%-12u%-12u\n",
                       zl303xx_PtpMessageTypeToStr(msgType), streamCounters.msg[msgType].rx,
                       streamCounters.msg[msgType].rx - streamCounters.msg[msgType].rxProcessed,
                       streamCounters.msg[msgType].lost);
            }
        }

        /* Print TX message counters. */
        printf("\nPTP Egress Message Count for Stream: %u\n", streamHandle);
        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (tempList & 1)   /* Only print counters in the list. */
            {
                printf("%-21s : %u\n",
                       zl303xx_PtpMessageTypeToStr(msgType), streamCounters.msg[msgType].tx);
            }
        }

        /* check whether to reset all counters for this stream. */
        if (reset) {
            status = zl303xx_PtpStreamCountersReset(streamHandle);
        }
    }

    return status;
}

/*writing to a string the PTP messages counters for the specified stream.*/
zlStatusE zl303xx_DebugPtpMsgCountToString(zl303xx_PtpStreamHandleT streamHandle,char *str)
{
    zlStatusE status = ZL303XX_OK;

    Uint4T msgType;
    Uint32T tempList;
    zl303xx_PtpStreamCountersS streamCounters;


    /* Create a list of all the message types to print counters for. */
    /* Since we support all message types just assign the VALID mask. */
    const Uint32T msgTypeList = ZL303XX_PTP_V2_VALID_MESSAGE_TYPE_MASK;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpStreamCountersGet(streamHandle, &streamCounters);
    }

    if (status == ZL303XX_OK)
    {
        /* Print RX message counters. */
        sprintf(str,"\nPTP Ingress Message Count for Stream: %u\n", streamHandle);
        sprintf(str+strlen(str),"%15s   RX          Dropped     Lost\n", "");

        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (ZL303XX_BIT_TEST(msgTypeList, msgType))  /* Only print counters in the list. */
            {
                sprintf(str+strlen(str),"%-21s : %-12u%-12u%-12u\n",
                       zl303xx_PtpMessageTypeToStr(msgType), streamCounters.msg[msgType].rx,
                       streamCounters.msg[msgType].rx - streamCounters.msg[msgType].rxProcessed,
                       streamCounters.msg[msgType].lost);
            }
        }

        /* Print TX message counters. */
        sprintf(str+strlen(str),"\nPTP Egress Message Count for Stream: %u\n", streamHandle);
        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (tempList & 1)   /* Only print counters in the list. */
            {
                sprintf(str+strlen(str),"%-21s : %u\n",
                       zl303xx_PtpMessageTypeToStr(msgType), streamCounters.msg[msgType].tx);
            }
        }

        /* Reset all counters for this stream. */
        /*status = zl303xx_PtpStreamCountersReset(streamHandle);*/
    }

    return status;
}

/**
Fills the PTP messages count structure with the PTP messages count of a specified stream

  Parameters:
   [in]   -streamHandle   Handle to a previously created stream.
             -PtpMsgPtr      a pointer to a PTP messages count structure

****************************************************************************************/
zlStatusE zl303xx_DebugPtpMsgCountGet(zl303xx_PtpStreamHandleT streamHandle,zl303xx_DebugPtpMsgCountStruct *PtpMsgPtr)
{
    zlStatusE status = ZL303XX_OK;

    Uint4T msgType;
    Uint32T tempList;
	Uint4T ii=0;
    zl303xx_PtpStreamCountersS streamCounters;


    /* Create a list of all the message types to get counters for. */
    /* Since we support all message types just assign the VALID mask. */
    const Uint32T msgTypeList = ZL303XX_PTP_V2_VALID_MESSAGE_TYPE_MASK;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpStreamCountersGet(streamHandle, &streamCounters);
    }

    if (status == ZL303XX_OK)
    {
        /*RX message counters. */
        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (ZL303XX_BIT_TEST(msgTypeList, msgType))  /* Only counters in the list. */
            {
				PtpMsgPtr->PTP_ingressMessages[ii][0]= (Uint32T)streamCounters.msg[msgType].rx; /*# received messages for message type*/
				PtpMsgPtr->PTP_ingressMessages[ii][1]= (Uint32T)streamCounters.msg[msgType].rx - streamCounters.msg[msgType].rxProcessed; /*# dropped messages for message type*/
				PtpMsgPtr->PTP_ingressMessages[ii][2]= (Uint32T)streamCounters.msg[msgType].lost; /*# lost messages for message type*/
				ii++;
            }
        }

        /*TX message counters. */
		ii=0;
        for (msgType = 0, tempList = msgTypeList; tempList != 0; msgType++, tempList >>= 1)
        {
            if (tempList & 1)   /* Only counters in the list. */
            {
				PtpMsgPtr->PTP_egressMessages[ii]= (Uint32T)streamCounters.msg[msgType].tx; /*# transmitted messages for message type*/
				ii++;
            }
        }

    }

    return status;
}

/* zl303xx_DebugPtpPortAmt */
/**
   User function to print out entries of a PTP port's Acceptable Master Table.

  Parameters:
   [in]   portHandle   Handle to a previously created port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortAmt(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;
   zl303xx_PtpAccMasterEntryS *amt;
   Uint32T count, msgId;
   char str[ZL303XX_PTP_ADDR_STR_LEN_MAX];

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      printf("Port %u: Acceptable Master Table\n", portHandle);
      printf("   enabled=%s, size=%u, max=%u\n\n",
             ZL303XX_BOOL_TO_STR(pPort->config.acceptableMasterTableEnabled),
             pPort->accMasterTbl.actualTableSize,
             pPort->accMasterTbl.maxTableSize);

      /* Print the table printout header. */
      printf(" Port Address (PROTOCOL) | streamHandle | altPriority1 \n");
      printf("-------------------------+--------------+--------------\n");

      if (pPort->accMasterTbl.actualTableSize == 0)
      {
         printf("  >> EMPTY <<\n");
      }

      /* Traverse the Acceptable Master Table and print out entry data. */
      for (count = 0, amt = pPort->accMasterTbl.array;
           count < pPort->accMasterTbl.actualTableSize;
           count++)
      {
         (void)zl303xx_PtpConvertPortAddressToStr(&amt[count].addr, str);
         printf("%17s (%d)    |", str, amt[count].addr.networkProtocol);

         if ((amt[count].streamHandle == (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID))
            printf("    NULL");
         else
            printf("%8u", amt[count].streamHandle);

         printf("      |%8u\n", amt[count].alternatePriority1);
      }

      printf("-------------------------+--------------+--------------\n\n");

      /* Print the Dropped-PAcket Counter values for each messageType. */
      printf("           messageType   |  Dropped at AMT \n");
      printf("-------------------------+------------------------\n");

      for (msgId = 0; msgId < ZL303XX_PTP_V2_NUM_MSG_IDS; msgId++)
      {
         /* Print the stats for valid message types. */
         if (ZL303XX_CHECK_MESSAGE_TYPE(msgId) == ZL303XX_OK)
         {
            printf("%21s    |  %d \n",
                   zl303xx_PtpMessageTypeToStr(msgId),
                   pPort->amtDroppedMsgCount[msgId]);
         }
      }

      printf("-------------------------+------------------------\n");
   }

   return status;
}

/* zl303xx_DebugPtpPortAst */
/**
   User function to print out entries of a PTP port's Acceptable Slave Table.

  Parameters:
   [in]   portHandle   Handle to a previously created PTP port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortAst(zl303xx_PtpPortHandleT portHandle)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PortDataS *pPort;
    zl303xx_PtpAccSlaveEntryS *ast;
    Uint32T count;
    char str[ZL303XX_PTP_ADDR_STR_LEN_MAX];

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpPortDataGet(portHandle, &pPort);
    }

    if (status == ZL303XX_OK)
    {
        printf("Port %u: Acceptable Slave Table\n", portHandle);
        printf("   enabled=%s, size=%u, max=%u\n\n",
               ZL303XX_BOOL_TO_STR(pPort->config.acceptableSlaveTableEnabled),
               pPort->accSlaveTbl.actualTableSize,
               pPort->accSlaveTbl.maxTableSize);

        if (pPort->accSlaveTbl.actualTableSize > 0)
        {
            /* Print the table printout header. */
            printf(" Port Address (PROTOCOL) | streamHandle\n" \
                   "-------------------------+--------------\n");

            /* Traverse the Acceptable Slave Table and print out entry data. */
            for (count = 0, ast = pPort->accSlaveTbl.array;
                count < pPort->accSlaveTbl.actualTableSize;
                count++)
            {
                (void)zl303xx_PtpConvertPortAddressToStr(&ast[count].addr, str);
                printf("%17s (%d)    |", str, ast[count].addr.networkProtocol);
                if ((ast[count].streamHandle == (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID))
                    printf("    NULL");
                else
                    printf("%8u", ast[count].streamHandle);
                printf("\n");
            }
        }
    }

    return status;
}

/* zl303xx_DebugPtpPortContracts */
/**
   User function to print out the Unicast Contract data for all streams
   associated with the specified port.

  Parameters:
   [in]   portHandle   Handle to a previously created port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortContracts(zl303xx_PtpPortHandleT portHandle)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PortDataS *pPort;
    zl303xx_ClockStreamS *pStream;
    zl303xx_PtpTotalPpsS totalPps;
    Uint32T count;

    zl303xx_MessageTypeE msgType;
    zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

    zl303xx_MessageTypeE displayOrder[] = {ZL303XX_MSG_ID_ANNOUNCE,
        ZL303XX_MSG_ID_SYNC,
        ZL303XX_MSG_ID_DELAY_RESP,
        ZL303XX_MSG_ID_PEER_DELAY_RESP};

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpPortDataGet(portHandle, &pPort);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpPortTotalPpsGet(portHandle, &totalPps);
    }

    if (status == ZL303XX_OK)
    {
        printf("Port %u: Unicast Negotiation Contract Summary\n", portHandle);
        printf("--------------------------------------------------\n");
        printf("Stacked-TLV Enabled setting: %d\n\n", pPort->config.unicastNegotiation.stackedTlvEn);
        printf("Port Total Service Limit (PPS): \n");

        for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
        {
            msgType = displayOrder[count];

            printf("   %-19s : %u / ", zl303xx_PtpMessageTypeToStr(msgType), totalPps.msg[msgType]);
            printf(pPort->config.unicastNegotiation.totalPpsMax[msgType] == ZL303XX_PTP_PPS_UNLIMITED
                   ? "unlimited \n"
                   : "%u (max) \n", pPort->config.unicastNegotiation.totalPpsMax[msgType]);
        }
        printf("--------------------------------------------------\n\n");

        printf("Individual Contract Limits\n");
        printf("--------------------------------------------------\n");
        printf("                             MAX     :     MAX       :    MIN \n");
        printf("                          Contracts  : Duration(Sec) : Interval\n");
        for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
        {
            msgType = displayOrder[count];
            printf("   %-19s :", zl303xx_PtpMessageTypeToStr(msgType));

            if (ZL303XX_OK == zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType))
            {
               printf((pPort->contractLimits[uniNegMsgType].maxCount == (Uint32T)(-1))
                      ? " (unlimited) :"
                      : "  %u (max)    :", pPort->contractLimits[uniNegMsgType].maxCount);
               printf("     %-5u :      %d",
                     pPort->contractLimits[uniNegMsgType].maxDurationSec,
                     pPort->contractLimits[uniNegMsgType].minIntvl);
               printf(" (%d %s) ",
                     (pPort->contractLimits[uniNegMsgType].minIntvl <= 0)
                           ? log2IntvlToPps(pPort->contractLimits[uniNegMsgType].minIntvl)
                           : -log2IntvlToPps(pPort->contractLimits[uniNegMsgType].minIntvl),
                      (pPort->contractLimits[uniNegMsgType].minIntvl <= 0)
                           ? "PPS" : "SPP");
            }
            printf("\n");
        }
        printf("--------------------------------------------------\n\n");

        /* Print the table header. */
        printf("- Stream Params -| --------- Message Type Parameters --------\n");
        printf(" Stream | Stream | uniMsg | expiry |  log2 |     Duration    |\n");
        printf(" Handle | State  |  state |  Limit | intvl | negotd | remain |\n");
        printf("--------+--------+--------|--------|-------|--------|--------|\n");

        /* Check every stream and display the data. */
        ZL303XX_PTP_FOR_EACH_STREAM(&pPort->streamList, pStream)
        {
            printf(" %4u   |%4u    |        |        |       |        |        |\n",
                   pStream->clockStreamHandle,
                   pStream->streamSubstate);

            if (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
            {
                printf("    ANNOUNCE (RX): %4d   | %4u   | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].state,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].renewSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.interval,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.durationSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec);
            }
            if (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0)
            {
                printf("        SYNC (RX): %4d   | %4u   | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].state,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].renewSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].grant.interval,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].grant.durationSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec);
            }
            if (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0)
            {
                printf("       DELAY (RX): %4d   | %4u   | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].state,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].renewSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].grant.interval,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].grant.durationSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec);
            }
            if (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0)
            {
                printf("  PEER_DELAY (RX): %4d   | %4u   | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].state,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].renewSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].grant.interval,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].grant.durationSec,
                       pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec);
            }

            if (pStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
            {
                printf("    ANNOUNCE (TX): %4d   |        | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].state,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.interval,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.durationSec,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec);
            }
            if (pStream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0)
            {
                printf("        SYNC (TX): %4d   |        | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].state,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].grant.interval,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].grant.durationSec,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec);
            }
            if (pStream->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0)
            {
                printf("       DELAY (TX): %4d   |        | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].state,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].grant.interval,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].grant.durationSec,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec);
            }

            if (pStream->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0)
            {
                printf("  PEER_DELAY (TX): %4d   |        | %4d  |  %4u  |  %4u  |\n",
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].state,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].grant.interval,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].grant.durationSec,
                       pStream->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec);
            }

            printf("--------+--------|--------|--------|-------|--------|--------|\n");
        }
    }
    return status;
}

/* zl303xx_DebugPtpPortGrantTbl */
/**
   User function to print out entries of a PTP port's grant table.

  Parameters:
   [in]   portHandle  Handle to a previously created PTP port.

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortGrantTbl(zl303xx_PtpPortHandleT portHandle)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PortDataS *pPort;
    zl303xx_PtpGrantConfigS *grant;
    zl303xx_BooleanE first;
    char str[ZL303XX_PTP_ADDR_STR_LEN_MAX];
    Uint32T msgTypeMask, count, msgType;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpPortDataGet(portHandle, &pPort);
    }

    if (status == ZL303XX_OK)
    {
        printf("Port %u: Grant Table\n", portHandle);
        printf("   default denyService=%s, size=%u, max=%u\n\n",
               ZL303XX_BOOL_TO_STR(pPort->config.unicastNegotiation.denyServiceRequests),
               pPort->grantTbl.actualTableSize,
               pPort->grantTbl.maxTableSize);

        if (pPort->grantTbl.actualTableSize > 0)
        {
            /* Print the table printout header. */
            printf(" Port Address (PROTOCOL) | Message Types\n" \
                   "-------------------------+---------------\n");

            /* Traverse the grant table and print out entry data. */
            for (count = 0, grant = pPort->grantTbl.array;
                count < pPort->grantTbl.actualTableSize;
                count++)
            {
                first = ZL303XX_TRUE;

                (void)zl303xx_PtpConvertPortAddressToStr(&grant[count].portAddress, str);
                printf("%17s (%d)    |", str, grant[count].portAddress.networkProtocol);

                for (msgType = 0, msgTypeMask = grant[count].msgTypeMask;
                    msgTypeMask != 0;
                    msgType++, msgTypeMask >>= 1)
                {
                    if (msgTypeMask & 1)
                    {
                        if (!first)
                        {
                            printf("                         |");
                        }
                        printf(" %s\n", zl303xx_PtpMessageTypeToStr(msgType));
                        first = ZL303XX_FALSE;
                    }
                }

                if (first)
                {
                    printf(" NONE\n");
                }

                printf("-------------------------+---------------\n");
            }
        }
    }

    return status;
}

/* zl303xx_DebugPtpPortFmt */
/**
   User function to print out entries of a PTP port's Foreign Master Table.

  Parameters:
   [in]   portHandle   Handle to a previously created port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortFmt(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   char label[] = "Port handle NUMBER";  /* Description of table owner */
   zl303xx_PortDataS *pPort;

   status = ZL303XX_CHECK_PORT_HANDLE(portHandle);

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      if (sprintf(label, "Port Handle %d", (SINT_T)portHandle) < 0)
      {
         label[0] = '\0';
      }

      /* Specify this Port in the FmtDump routine. */
      zl303xx_PtpFmtDump(label, &pPort->clock->foreignMasterDS, &pPort->portDS.portIdentity);
   }

   return status;
}

/* zl303xx_DebugPtpClockFmt */
/**
   User function to print out entries of a PTP clock's Foreign Master Table.

  Parameters:
   [in]   clockHandle   Handle to a previously created clock.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpClockFmt(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;
    char label[] = "Clock handle NUMBER";  /* Description of table owner */
    zl303xx_PtpClockS *pClock;

    status = ZL303XX_CHECK_CLOCK_HANDLE(clockHandle);

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
    }

    if (status == ZL303XX_OK)
    {
        if (sprintf(label, "Clock Handle %d", (SINT_T)clockHandle) < 0)
        {
            label[0] = '\0';
        }

        zl303xx_PtpFmtDump(label, &pClock->foreignMasterDS, NULL);
    }

    return status;
}

/* zl303xx_DebugPtpPortFmtFlush */
/**
   User function to flush entries of a PTP port's Foreign Master Table.

  Parameters:
   [in]   portHandle   Handle to a previously created port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortFmtFlush(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   status = ZL303XX_CHECK_PORT_HANDLE(portHandle);

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      /* Specify this Port in the FmtFlush routine. */
      zl303xx_PtpFmtFlush(&pPort->clock->foreignMasterDS, &pPort->portDS.portIdentity);
   }

   return status;
}

/* zl303xx_DebugPtpClockFmtFlush */
/**
   User function to flush entries of a PTP clock's Foreign Master Table.

  Parameters:
   [in]   clockHandle   Handle to a previously created clock.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpClockFmtFlush(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PtpClockS *pClock;

    status = ZL303XX_CHECK_CLOCK_HANDLE(clockHandle);

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
    }

    if (status == ZL303XX_OK)
    {
      /* Remove all entries from the table. */
      status = zl303xx_PtpFmtFlush(&pClock->foreignMasterDS, NULL);
    }

    return status;
}

/* zl303xx_DebugPtpPortUmt */
/**
   User function to print out entries of a PTP clock's Unicast Master Table.

  Parameters:
   [in]   portHandle   Handle to a previously created port.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpPortUmt(zl303xx_PtpPortHandleT portHandle)
{
    zlStatusE status = ZL303XX_OK;

    if ((status = zl303xx_PtpUmtShow(portHandle)) != ZL303XX_OK)
        printf("zl303xx_DebugPtpPortUmt: call to zl303xx_PtpUmtShow() failed=%d\n", status);

    return status;
}


/* zl303xx_DebugPtpProductInfo */
/**
   Displays the productDescription, revisionData, and userDescription of the
   PTP node.

  Parameters:
     None.

*******************************************************************************/
zlStatusE zl303xx_DebugPtpProductInfo(void)
{
    zlStatusE status = ZL303XX_OK;
    char productDescription[ZL303XX_PTP_NODE_PROD_DESC_LEN + 1];
    char revisionData[ZL303XX_PTP_NODE_REV_DATA_LEN + 1];
    char userDescription[ZL303XX_PTP_NODE_USER_DESC_LEN + 1];

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpNodeProductDescriptionGet(productDescription) |
                 zl303xx_PtpNodeRevisionDataGet(revisionData) |
                 zl303xx_PtpNodeUserDescriptionGet(userDescription);
    }

    if (status == ZL303XX_OK)
    {
        printf("PTP Product Information\n" \
               "=======================\n" \
               "Product Description: %s\n" \
               "Revision Data:       %s\n" \
               "User Description:    %s\n",
               productDescription, revisionData, userDescription);
    }

    return status;
}

/* zl303xx_DebugPtpBuildInfo */
/**
   Displays information about the PTP release and build date/times.

*******************************************************************************/
void zl303xx_DebugPtpBuildInfo(void)
{
    printf("PTP Build Information\n" \
           "=====================\n" \
           "Release Version: %s\n" \
           "Release Date:    %s\n" \
           "Release Time:    %s\n" \
           "Release SW ID:   %s\n" \
           "Build Date:      %s\n" \
           "Build Time:      %s\n",
           zl303xx_PtpReleaseVersion, zl303xx_PtpReleaseDate, zl303xx_PtpReleaseTime,
           zl303xx_PtpReleaseSwId, zl303xx_PtpBuildDate, zl303xx_PtpBuildTime);
}

/* zl303xx_DebugPtpStreamRequestGrant */
/**
   User function to send a request TLV for a message type at a
   certain duration and interval. Used for testing TLV processing.

  Parameters:
   [in]   streamHandle     Handle to a previously created stream.
   [in]   messageType      Message type to request.
   [in]   pLogInterval     Log base 2 interval rate. If NULL, defaults to 1.
   [in]   pDuration        Duration of GRANT period. If NULL, defaults to 300.

  Notes:  If both pLogInterval and pDuration are NULL, the existing configured
       values will be used.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpStreamRequestGrant(zl303xx_PtpStreamHandleT streamHandle,
                                           Uint32T messageType, Sint8T *pLogInterval, Uint32T *pDuration)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PtpContractS contract;

    if (status == ZL303XX_OK)
    {
        /* If no interval or duration is specified, request the contract with
         * existing values. */
        if ((pLogInterval == NULL) && (pDuration == NULL))
        {
            return zl303xx_PtpContractRequest(streamHandle, messageType, NULL);
        }

        status = zl303xx_PtpContractStructInit(&contract);
    }

    if (status == ZL303XX_OK)
    {
        if (pLogInterval != NULL)
        {
            contract.interval = *pLogInterval;
        }
        else
        {
            contract.interval = 1;
        }

        if (pDuration != NULL)
        {
            contract.durationSec = *pDuration;
        }
        else
        {
            contract.durationSec = 300;
        }

        status = zl303xx_PtpContractRequest(streamHandle, messageType, &contract);
    }

    return status;
}

/* zl303xx_DebugPtpClockUnicastServiceLimits */
/**
   User function to print out the Unicast Contract Limits for the specified clock
   and each of its ports as well as the current negotiated PPS rates of each
   message contract.

  Parameters:
   [in]  clockHandle    Handle to a previously created clock.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugPtpClockUnicastServiceLimits(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;

    /* PTP Object Pointers. */
    zl303xx_PtpClockS *pPtpClock = NULL;
    zl303xx_PortDataS *pPtpPort = NULL;

    /* PPS and loop counters. */
    zl303xx_PtpTotalPpsS totalPps;
    Uint32T count;

    zl303xx_MessageTypeE type;
    zl303xx_MessageTypeE displayOrder[] = {ZL303XX_MSG_ID_ANNOUNCE,
        ZL303XX_MSG_ID_SYNC,
        ZL303XX_MSG_ID_DELAY_RESP,
        ZL303XX_MSG_ID_PEER_DELAY_RESP};

    printf("\nzl303xx_DebugPtpClockUnicastServiceLimits:: clockHandle[%d]\n", clockHandle);
    printf("--------------------------------------------------------------\n");

    /* Get the Clock Data */
    if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK)
    {
        printf("  >> ERROR(%d): Unable to retrieve Clock Data for clockHandle[%d]\n", status, clockHandle);
        return status;
    }

    /* Use the API command to retrieve the counts. */
    if ((status = zl303xx_PtpClockTotalPpsGet(clockHandle, &totalPps)) != ZL303XX_OK)
    {
        printf("  >> ERROR (%d): Unable to retrieve Granted PPS totals for clockHandle[%d]\n", status, clockHandle);
        return status;
    }

    if (status == ZL303XX_OK)
    {
        printf("\nClock[%u]: Unicast Negotiation Contract Limit Summary\n", clockHandle);
        printf("-----------------------------------------------------\n");
        printf("Service Limits (PPS)    Configured      GRANTed\n");

        for (count = 0; count < ZL303XX_ARRAY_SIZE(displayOrder); count++)
        {
            type = displayOrder[count];
            printf("   %-19s : ", zl303xx_PtpMessageTypeToStr(type));
            printf((pPtpClock->config.unicastNegotiation.totalPpsMax[type] == ZL303XX_PTP_PPS_UNLIMITED)
                   ? "    unlimited"
                   : "    %4u (max)", pPtpClock->config.unicastNegotiation.totalPpsMax[type]);
            printf("       %4u (pps)\n",  totalPps.msg[type]);

        }
        printf("---------------------------------------------------------------\n\n");
        printf("  Port handle       ANNC        SYNC        DELAY       PDELAY \n");
        printf("---------------------------------------------------------------\n");

        /* Get the Granted contract totals for each Unicast Neg enabled Port on the Clock. */
        {
            zl303xx_PtpTotalPpsS pTotal;

            ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
            {
                if (pPtpPort->config.unicastNegotiation.enabled == ZL303XX_TRUE)
                {
                    /* Clear the PPS count array. */
                    memset(&pTotal, 0, sizeof(pTotal));

                    /* Add the negotiated rates for this port. */
                    pTotal.msg[ZL303XX_MSG_ID_SYNC]            = zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_SYNC);
                    pTotal.msg[ZL303XX_MSG_ID_ANNOUNCE]        = zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_ANNOUNCE);
                    pTotal.msg[ZL303XX_MSG_ID_DELAY_RESP]      = zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_DELAY_RESP);
                    pTotal.msg[ZL303XX_MSG_ID_PEER_DELAY_RESP] = zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_PDELAY_RESP);

                    printf("     %3u            %4u          %4u          %4u          %4u\n",
                           pPtpPort->clockPortHandle,
                           pTotal.msg[ZL303XX_MSG_ID_ANNOUNCE],
                           pTotal.msg[ZL303XX_MSG_ID_SYNC],
                           pTotal.msg[ZL303XX_MSG_ID_DELAY_RESP],
                           pTotal.msg[ZL303XX_MSG_ID_PEER_DELAY_RESP]);
                }
            }
        }

        printf("\n---------------------- END -----------------------\n\n");

    }
    return status;
}

/* zl303xx_DebugStreamPointersShow */
/**
   Debug function to print out the Pointers of each stream associated with the
   specified clock.

  Parameters:
   [in]  clockHandle    Handle to a previously created clock.

  Return Value:
   zlStatusE

*******************************************************************************/
void zl303xx_DebugStreamPointersShow(zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS * pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort;
   zl303xx_ClockStreamS *pPtpStream;

   Uint32T streamHandle;

   printf ("\n-------- Stream Pointer List for Clock (%d) --------\n", clockHandle);

   if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK )
   {
      printf ("zl303xx_DebugClockDataSet(): Call to zl303xx_PtpClockDataGet() failed!\n");
   }

   /* Stream not found on the current port. Expand the search to all ports
    * attached to this clock. */
   if (status == ZL303XX_OK)
   {
      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
      {
         printf (">>>> Port Handle (%d): portNumber (%d): Type - %s\n",
               pPtpPort->clockPortHandle,
               pPtpPort->portDS.portIdentity.portNumber,
               (pPtpPort->virtualData.isVirtualPort) ? "VIRTUAL" : "PTP");

         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            printf("     >>>> Stream (%d): (%p) :: Array Address(%p)\n",
                  pPtpStream->clockStreamHandle,
                  (void*)pPtpStream,
                  (void*)Zl303xx_PtpStreamArray[pPtpStream->clockStreamHandle]);
         }
      }

      printf ("\n*** Stream Array List---\n");
      for (streamHandle = 0; streamHandle < ZL303XX_PTP_NUM_STREAMS_MAX; streamHandle++)
      {
         /* Point of this is to verify that the pointer stored in the array
          * is the same as that returned by the GET routine.   */
         if (Zl303xx_PtpStreamArray[streamHandle] != NULL)
         {
            (void)zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
            printf("     >>>> Stream (%d): (%p) :: Array Address(%p)\n",
                  streamHandle,
                  (void*)Zl303xx_PtpStreamArray[streamHandle],
                  (void*)pPtpStream);
         }
      }
   }
}  /* END zl303xx_DebugStreamPointersShow */

/* zl303xx_DebugPtpStreamEgressOverrideSet */
/**
   Debug function to set egress override values for a specified stream.

   Use zl303xx_PtpStreamEgressOverrideAnncFieldSet directly with NULL
   to clear the item.

  Parameters:
   [in]  streamHandle    Handle to a previously created stream.
   [in]  overrideId      See zl303xx_PtpStreamOverrideAnncEgressE.
   [in]  value           Over-ride value.

  Return Value:
   zlStatusE

*******************************************************************************/
void zl303xx_DebugPtpStreamEgressOverrideSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T overrideId,
      Sint32T value)
{
   Uint8T valueUint8T;
   Uint16T valueUint16T;
   zl303xx_BooleanE valueBool;
   /* Uint32T valueUint32T; */

   /* Needed for some cases */
   zl303xx_ClockStreamS *pPtpStream = NULL;

   switch (overrideId)
   {
   case ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET_VALID :
   case ZL303XX_PTP_STREAM_OVERRIDE_TIME_TRACEABLE :
   case ZL303XX_PTP_STREAM_OVERRIDE_FREQUENCY_TRACEABLE :
   case ZL303XX_PTP_STREAM_OVERRIDE_SYNCHRO_UNCERTAIN :
   {
      valueBool = ((value) ? ZL303XX_TRUE : ZL303XX_FALSE);
      zl303xx_PtpStreamEgressOverrideAnncFieldSet(streamHandle, overrideId, &valueBool);
      break;
   }

   case ZL303XX_PTP_STREAM_OVERRIDE_TRANSPORT :
   case ZL303XX_PTP_STREAM_OVERRIDE_DOMAIN :
   case ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY1 :
   case ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY2 :
   case ZL303XX_PTP_STREAM_OVERRIDE_GM_CLASS :
   case ZL303XX_PTP_STREAM_OVERRIDE_GM_ACCURACY :
   case ZL303XX_PTP_STREAM_OVERRIDE_TIME_SOURCE :
   {
      valueUint8T = value;
      zl303xx_PtpStreamEgressOverrideAnncFieldSet(streamHandle, overrideId, &valueUint8T);
      break;
   }

   case ZL303XX_PTP_STREAM_OVERRIDE_GM_VARIANCE :
   case ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET :
   case ZL303XX_PTP_STREAM_OVERRIDE_STEPS_REMOVED :
   {
      valueUint16T = value;
      zl303xx_PtpStreamEgressOverrideAnncFieldSet(streamHandle, overrideId, &valueUint16T);
      break;
   }

   /* For this field, just allow the portNumber to be over-written. */
   case ZL303XX_PTP_STREAM_OVERRIDE_SRC_PORT_ID :
   {
      zl303xx_PortIdentity ovrPortId;

      if (zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream) == ZL303XX_OK)
      {
         ovrPortId = pPtpStream->portData->portDS.portIdentity;
         ovrPortId.portNumber = value;
         zl303xx_PtpStreamEgressOverrideAnncFieldSet(streamHandle, overrideId, &ovrPortId);
      }
      else
      {
         printf ("zl303xx_DebugClockDataSet(): Call to zl303xx_PtpClockDataGet() failed!\n");
      }

      break;
   }

   /* For this field, just change 1 byte to make it look like another GM_ID. */
   case ZL303XX_PTP_STREAM_OVERRIDE_GM_IDENTITY :
   {
      zl303xx_ClockIdentity ovrClockId;

      if (zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream) == ZL303XX_OK)
      {
         memcpy(ovrClockId,
                pPtpStream->portData->clock->parentDS.grandmasterIdentity,
                8); /* 8 bytes copy */

         ovrClockId[7] = value;
         zl303xx_PtpStreamEgressOverrideAnncFieldSet(streamHandle, overrideId, ovrClockId);
      }
      else
      {
         printf ("zl303xx_DebugClockDataSet(): Call to zl303xx_PtpClockDataGet() failed!\n");
      }

      break;
   }

   case ZL303XX_PTP_STREAM_OVERRIDE_CORRECTION :
   case ZL303XX_PTP_STREAM_OVERRIDE_ORIGIN_TIMESTAMP :
   case ZL303XX_PTP_STREAM_OVERRIDE_NUM_ANNC_ENTRIES :
   default :
   {
      /* Should never reach here since a check is done above. */
      ZL303XX_TRACE_ALWAYS(
            "NOT IMPLEMENTED ID (%u) in Switch",
            overrideId, 0,0,0,0,0);

      break;
   }
   }
}

void testAddPort(Uint8T ip);
void testAddPort(Uint8T ip)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpPortCreateS pPortCreate;
   Uint32T createdPortHandle;

   /* Initialize the port config structure with default values. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortCreateStructInit(&pPortCreate);
   }

   /* Link the port to the parent clock. */
   if (status == ZL303XX_OK)
   {
      pPortCreate.clockHandle = 0;
      {
         pPortCreate.localAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
         pPortCreate.localAddr.addressLength = 4;
         pPortCreate.localAddr.addressField[0] = 10;
         pPortCreate.localAddr.addressField[1] = 0;
         pPortCreate.localAddr.addressField[2] = 0;
         pPortCreate.localAddr.addressField[3] = ip;
      }

      pPortCreate.portNumber = 1;
      pPortCreate.requestedHandle = 0;

      status = zl303xx_PtpPortCreate(&pPortCreate, &createdPortHandle);
   }
}
void testDeletePort(Uint32T portHandle);
void testDeletePort(Uint32T portHandle)
{
   zl303xx_PtpPortDeleteS pPortDelete;
   zl303xx_PtpPortDelete(portHandle, &pPortDelete);
}

void testAddStream(Uint32T port, Uint8T ip);
void testAddStream(Uint32T port, Uint8T ip)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpStreamCreateS pStreamCreate;
   Uint32T createdStreamHandle;

   /* Initialize the port config structure with default values. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamCreateStructInit(&pStreamCreate);
   }

   /* Link the Stream to the parent Port. */
   if (status == ZL303XX_OK)
   {
      pStreamCreate.portHandle = port;
      {
         pStreamCreate.destAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
         pStreamCreate.destAddr.addressLength = 4;
         pStreamCreate.destAddr.addressField[0] = 10;
         pStreamCreate.destAddr.addressField[1] = 0;
         pStreamCreate.destAddr.addressField[2] = 0;
         pStreamCreate.destAddr.addressField[3] = ip;
      }

      pStreamCreate.requestedHandle = 0;

      status = zl303xx_PtpStreamCreate(&pStreamCreate, &createdStreamHandle);
   }
}
void testDeleteStream(Uint32T streamHandle);
void testDeleteStream(Uint32T streamHandle)
{
   zl303xx_PtpStreamDeleteS pStreamDelete;
   zl303xx_PtpStreamDelete(streamHandle, &pStreamDelete);
}




/*****************   END   ****************************************************/
