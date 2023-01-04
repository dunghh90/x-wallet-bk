

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions that are bound into PSLFCL to provide the
*     user with the ability to use their own methods to handle certain situations.
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ExampleAprBinding.h"
#include "zl303xx_Apr.h"

#if defined ZL303XX_REFERENCE_SELECTION
   #include "zl303xx_ExampleRefSelectApi.h"
#endif

#if defined OS_LINUX
char *ctime_r(const time_t *timep, char *buf);
#else
#include <time.h>
#endif

/*****************   DEFINES     **********************************************/
#ifndef NANOSECONDS_IN_1SEC
    #define NANOSECONDS_IN_1SEC 1000000000L
#endif
#ifndef TEN_e6
    #define TEN_e6 1000000
#endif

#ifndef DATETIMESTR_MAXLEN
    #define DATETIMESTR_MAXLEN 32
#endif

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


static zlStatusE getCurrentTimeStr(char *dateTimeStr);
static zlStatusE getCurrentTimeStr(char *dateTimeStr)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "getCurrentTimeStr";

    if (status == ZL303XX_OK)
    {
       Uint64S localTime;

       OS_TIME_GET(&localTime, NULL);
       {
          time_t lTime_t = (time_t)localTime.hi;
       #if defined OS_VXWORKS && !(defined __VXWORKS_69)
          size_t bytes;
          if (ctime_r(&lTime_t, dateTimeStr, &bytes) == NULL)
       #else
          if (ctime_r(&lTime_t, dateTimeStr) == NULL)
       #endif
          {
             ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1, "%s: Error return from ctime_r() for localTime.hi=%u",
                         fnName, localTime.hi, 0,0,0,0);
             status = ZL303XX_ERROR;
          }
          else
          {
              Uint8T count;

              for (count = 0; count < DATETIMESTR_MAXLEN; count++)
              {
                  if (dateTimeStr[count] == '\n')
                  {
                      dateTimeStr[count] = '\0';
                      break;
                  }
              }
          }
       }
    }

    return status;
}


void exampleAprCguNotify(zl303xx_AprCGUNotifyS *msg)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprCguNotify";
    char dateTimeStr[DATETIMESTR_MAXLEN] = {0};

    if (status == ZL303XX_OK)
    {
        zl303xx_BooleanE value;

        getCurrentTimeStr(dateTimeStr);

        #define DO_NOTIFY(fmt, a, b, c) ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1, "%s  %p  %s:: " fmt, dateTimeStr, msg->hwParams, fnName, a, b, c)

        switch (msg->type)
        {
            case ZL303XX_CGU_HW_L_FLAG:
               value = msg->flags.hwL;
               DO_NOTIFY("HW L flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_HW_H_FLAG:
               value = msg->flags.hwH;
               DO_NOTIFY("HW H flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_HW_RF_FLAG:
               value = msg->flags.RF;
               DO_NOTIFY("HW RF flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_HW_SE_FLAG:
               value = msg->flags.SE;
               DO_NOTIFY("HW SE flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_HW_MH_FLAG:
               value = msg->flags.hwMH;
               DO_NOTIFY("HW MH flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_HW_MF_FLAG:
               value = msg->flags.hwMF;
               DO_NOTIFY("HW MF flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_SW_L_FLAG:
               value = msg->flags.swL;
               DO_NOTIFY("SW L flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_SW_H_FLAG:
               value = msg->flags.swH;
               DO_NOTIFY("SW H flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_SW_V_FLAG:
               value = msg->flags.V;
               DO_NOTIFY("SW V flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_SW_MH_FLAG:
               value = msg->flags.swMH;
               DO_NOTIFY("SW MH flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_TS_FLAG:
               value = msg->flags.TS;
               DO_NOTIFY("TS flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_WS_FLAG:
               value = msg->flags.WS;
               DO_NOTIFY("WS flag changed to: %d", value, 0,0);
               break;

           case ZL303XX_CGU_STATE:
               value = msg->flags.state;
               DO_NOTIFY("STATE flag changed to: %d", value, 0,0);
               break;

          case ZL303XX_CGU_RSTATUS:
               value = msg->flags.RStatus;
               DO_NOTIFY("RStatus flag changed to: %d", value, 0,0);
               break;

          case ZL303XX_CGU_SW_H_VALID:
               value = msg->flags.swHV;
               DO_NOTIFY("Validity of S/W Holdover Value: %d", value, 0,0);
               break;

        case ZL303XX_CGU_DROPPED_PKTS:
             value = msg->flags.DroppedPackets;
             DO_NOTIFY("DroppedPackets Flag changed to: %d", value, 0,0);
             break;

           case ZL303XX_CGU_NUM_TYPES:
           default:
               break;
        }
    }
}


void exampleAprElecNotify(zl303xx_AprElecNotifyS *msg)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprElecNotify";
    char dateTimeStr[DATETIMESTR_MAXLEN] = {0};

    if (status == ZL303XX_OK)
    {
        zl303xx_BooleanE value;

        getCurrentTimeStr(dateTimeStr);

       #define DO_NOTIFY(fmt, a, b, c) ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1, "%s  %p  %s:: " fmt, dateTimeStr, msg->hwParams, fnName, a, b, c)

        switch (msg->type)
        {
            case ZL303XX_ELEC_REF_L_FLAG:
                value = msg->flags.L;
                DO_NOTIFY("HW L flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_H_FLAG:
                value = msg->flags.H;
                DO_NOTIFY("HW H flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_RF_FLAG:
                value = msg->flags.RF;
                DO_NOTIFY("HW RF flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_SE_FLAG:
                value = msg->flags.SE;
                DO_NOTIFY("HW SE flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_OOR_FLAG:
                value = msg->flags.OOR;
                DO_NOTIFY("OOR flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_STATE:
                value = msg->flags.state;
                DO_NOTIFY("STATE flag changed to: %d", value, 0,0);
                break;

            case ZL303XX_ELEC_REF_NUM_TYPES:
            default:
                break;
       }

       #undef DO_NOTIFY

    }
}

void exampleAprServerNotify(zl303xx_AprServerNotifyS *msg)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprServerNotify";
    char dateTimeStr[DATETIMESTR_MAXLEN] = {0};

    if (status == ZL303XX_OK)
    {
        zl303xx_BooleanE newFlagVal;
        zl303xx_AprAlgPathFlagE newAlgPathFlag;
        zl303xx_AprAlgPathStateE newAlgFwdState;
        zl303xx_AprAlgPathStateE newAlgRevState;
        zl303xx_AprStateE newAprState;
        Uint16T serverId = msg->serverId;

       getCurrentTimeStr(dateTimeStr);

       #define DO_NOTIFY(fmt, a, b) ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1, "%s  %p  %s: serverId %d:: " fmt, dateTimeStr, msg->hwParams, fnName, serverId, a, b)

       switch (msg->type)
       {
          case ZL303XX_SERVER_CLK_L1_FLAG :
              newFlagVal = msg->flags.L1;
              DO_NOTIFY("L1 flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_L2_FLAG :
              newFlagVal = msg->flags.L2;
              DO_NOTIFY("L2 flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_L3_FLAG :
              newFlagVal = msg->flags.L3;
              DO_NOTIFY("L3 flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_L4_FLAG :
              newFlagVal = msg->flags.L4;
              DO_NOTIFY("L4 flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_L_FLAG :
              newFlagVal = msg->flags.L;
              DO_NOTIFY("L flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_GST_L_FLAG :
              newFlagVal = msg->flags.gstL;
              DO_NOTIFY("GST L flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_V_FLAG :
              newFlagVal = msg->flags.V;
              DO_NOTIFY("V flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_GST_V_FLAG :
              newFlagVal = msg->flags.gstV;
              DO_NOTIFY("GST V flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_S_FLAG :
              newFlagVal = msg->flags.S;
              DO_NOTIFY("S flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_U_FLAG :
              newFlagVal = msg->flags.U;
              DO_NOTIFY("U flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_U1_FLAG :
              newFlagVal = msg->flags.U1;
              DO_NOTIFY("U1 flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PE_FLAG :
              newFlagVal = msg->flags.PE;
              DO_NOTIFY("PE flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PA_FLAG :
              newFlagVal = msg->flags.PA;
              DO_NOTIFY("PA flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_GST_PA_FLAG :
              newFlagVal = msg->flags.gstPA;
              DO_NOTIFY("GST PA flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PA_HOLDOVER_FLAG :
              newFlagVal = msg->flags.PA_holdover;
              DO_NOTIFY("PA_holdover flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_H_FLAG :
              newFlagVal = msg->flags.H;
              DO_NOTIFY("H flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_GST_H_FLAG :
              newFlagVal = msg->flags.gstH;
              DO_NOTIFY("GST H flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_OOR_FLAG :
              newFlagVal = msg->flags.OOR;
              DO_NOTIFY("OOR flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_TT_ERR_FLAG :
              newFlagVal = msg->flags.ttErrDetected;
              DO_NOTIFY("TT ERR flag changed to: %d", newFlagVal, 0);
              if (zl303xx_GetAprUserHandlesA_90000Reset())
              {
                  ZL303XX_TRACE_ALWAYS("Customer must run zl303xx_AprWarmStartServer() for serverId=%d on %p to continue",
                                     msg->serverId, msg->hwParams, 0,0,0,0);
              }
              break;

          case ZL303XX_SERVER_CLK_OUTAGE_FLAG :
              newFlagVal = msg->flags.outageDetected;
              DO_NOTIFY("OUTAGE flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_OUTLIER_FLAG :
              newFlagVal = msg->flags.outlierDetected;
              DO_NOTIFY("OUTLIER flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_FRR_FLAG :
              newFlagVal = msg->flags.frrDetected;
              DO_NOTIFY("FWD_RR flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_RRR_FLAG :
              newFlagVal = msg->flags.rrrDetected;
              DO_NOTIFY("REV_RR flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_STEP_FLAG :
              newFlagVal = msg->flags.stepDetected;
              DO_NOTIFY("STEP flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PKT_LOSS_FWD_FLAG :
              newFlagVal = msg->flags.pktLossDetectedFwd;
              DO_NOTIFY("LOSS FWD flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PKT_LOSS_REV_FLAG :
              newFlagVal = msg->flags.pktLossDetectedRev;
              DO_NOTIFY("LOSS REV flag changed to: %d", newFlagVal, 0);
              break;

          case ZL303XX_SERVER_CLK_PATH_FLAG :
             newAlgPathFlag = msg->flags.algPathFlag;
              DO_NOTIFY("PATH flag changed to: %d", newAlgPathFlag, 0);
              break;

          case ZL303XX_SERVER_CLK_FWD_STATE :
             newAlgFwdState = msg->flags.algFwdState;
              DO_NOTIFY("STATE FWD flag changed to: %d", newAlgFwdState, 0);
              break;

          case ZL303XX_SERVER_CLK_REV_STATE :
             newAlgRevState = msg->flags.algRevState;
              DO_NOTIFY("STATE REV flag changed to: %d", newAlgRevState, 0);
              break;

          case ZL303XX_SERVER_CLK_STATE :
             newAprState = msg->flags.state;
             DO_NOTIFY("STATE flag changed to: %d", newAprState, 0);
             break;

          case ZL303XX_SERVER_CLK_PSL_ON :
             newFlagVal = msg->flags.PSLOn;
             DO_NOTIFY("PSL flag changed to: %d", newFlagVal, 0);
             break;

          case ZL303XX_SERVER_CLK_SA_ON :
             newFlagVal = msg->flags.SA;
             DO_NOTIFY("SA flag changed to: %d", newFlagVal, 0);
             break;

          case ZL303XX_SERVER_CLK_NUM_TYPES:
          default:
              DO_NOTIFY("UNHANDLED TYPE: %d", msg->type, 0);
              ZL303XX_TRACE_ALWAYS("%s UNHANDLED TYPE: %d", fnName, msg->type, 0,0,0,0);
              break;

       }

       #undef DO_NOTIFY

#if defined ZL303XX_REFERENCE_SELECTION
       /* Send a message for this PTP Stream to the G.781 Handler. */
       {
          exampleAlgorithmUpdateMsgS algUpdateData;

          algUpdateData.serverId = msg->serverId;
          algUpdateData.statusFlagId = msg->type;

          /* Package the Flag State */
          if (msg->type <= ZL303XX_SERVER_CLK_PKT_LOSS_REV_FLAG)
             algUpdateData.value = newFlagVal;
          else if (msg->type == ZL303XX_SERVER_CLK_PATH_FLAG)
             algUpdateData.value = newAlgPathFlag;
          else if (msg->type == ZL303XX_SERVER_CLK_FWD_STATE)
             algUpdateData.value = newAlgFwdState;
          else if (msg->type == ZL303XX_SERVER_CLK_REV_STATE)
             algUpdateData.value = newAlgRevState;
          else if (msg->type == ZL303XX_SERVER_CLK_STATE)
             algUpdateData.value = newAprState;

          (void)exampleRefSelectAlgorithmUpdate(algUpdateData.serverId, &algUpdateData);
       }
#endif
    }
}


const char *oneHzMsgTypeNames[ZL303XX_1HZ_FLAG_LAST+1] = {
    "1HZ_START_FLAG",
    "1HZ_COLLECTION_END_FLAG",
    "1HZ_END_FLAG",
    "1HZ_ADJUSTMENT_SIZE_FLAG",
    "1HZ_ADJUSTMENT_NETWORK_QUALITY_FLAG",
    "1HZ_SET_TIME_TIMEOUT_FLAG",
    "1HZ_STEP_TIME_TIMEOUT_FLAG",
    "1HZ_ADJUST_TIME_TIMEOUT_FLAG",
    "1HZ_ABORT_FLAG",
    "1HZ_MAX_FREQ_ADJ_TIME_EXCEEDED_FLAG",
    "1HZ_FLAG_LAST"
};
const Uint32T oneHzMsgTypeNamesCount = sizeof(oneHzMsgTypeNames)/sizeof(char*);


void exampleAprOneHzNotify(zl303xx_Apr1HzNotifyS *msg)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprOneHzNotify";
    char dateTimeStr[DATETIMESTR_MAXLEN] = {0};
    const char *msgType;

    if (status == ZL303XX_OK)
    {
        getCurrentTimeStr(dateTimeStr);
        if (msg->type < oneHzMsgTypeNamesCount)
            msgType = oneHzMsgTypeNames[msg->type];
        else
            msgType = "ERROR UNHANDLED MSGTYPE";

        ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1,
                   "%s  %p  %s: 1Hz serverId=%d type=%s(%d)",
                   dateTimeStr, msg->hwParams, fnName, msg->serverId, msgType, msg->type);

        if( msg->type == ZL303XX_1HZ_ADJUSTMENT_SIZE_FLAG )
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1,
                   "  Size=%u:%u",
                   msg->flags.adjustmentSize.hi, msg->flags.adjustmentSize.lo, 0,0,0,0);
        }

        if( msg->type == ZL303XX_1HZ_MAX_FREQ_ADJ_TIME_EXCEEDED_FLAG )
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1,
                   "  limited adustment size=%u:%u",
                   msg->flags.limitedAdjustmentSize.hi, msg->flags.limitedAdjustmentSize.lo, 0,0,0,0);
        }

        if( msg->type == ZL303XX_1HZ_ADJUSTMENT_NETWORK_QUALITY_FLAG )
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_NOTIFY, 1,
                   "  Quality=%d",
                   msg->flags.adjustmentNetworkQuality, 0,0,0,0,0);
        }
    }
}

/* exampleUserDelayFunc */
/**
   Allows the user to code a subsecond delay mechanism which does not rely on Posix timers
   to control task delay (i.e., 125 ms for PSL and APR).

  Parameters:
   [in]   requiredDelayMs    Expected delay time (in ms).
   [in]   startOfRun         System time at start of run in sec and ns.
   [in]   endOfRun           System time at end of run in sec and ns.
   [in]   lastStartTime      Pointer to the last start time.

  Return Value:
     ZL303XX_OK               No problems with the delay.
     ZL303XX_ERROR            Indicates the OS-specific delay function failed

*******************************************************************************/
zlStatusE exampleUserDelayFunc(Uint32T requiredDelayMs, Uint64S startOfRun, Uint64S endOfRun, Uint64S *lastStartTime)
{
   zlStatusE status = ZL303XX_OK;
   Uint64S usualDelayNs = {0,0}, delay = {0,0}, runTime = {0,0};

   zl303xx_CalcTimeDiff(*lastStartTime, endOfRun, &runTime);       /*  calc runTime (includes delay) */
/*    printf("Delay+Run=%dms ", runTime.lo / TEN_e6);  */

   *lastStartTime = startOfRun;                                  /* Ready for next time */
   usualDelayNs.lo = requiredDelayMs * TEN_e6;

   zl303xx_CalcNsDelay(runTime, usualDelayNs, &delay);            /* calc next delay based on runtime */
/*    printf("nextDelay=%dms ", delay.lo / TEN_e6);  */

   if (OS_TASK_DELAY(delay.lo / TEN_e6) != OS_OK)               /* New delay ms to compensate */
       status = ZL303XX_ERROR;

   return status;
}

void zl303xx_CalcTimeDiff(Uint64S startTime, Uint64S stopTime, Uint64S *diffTime)
{
    if (stopTime.hi > startTime.hi)    /* sec roll-over */
    {
        if (stopTime.hi > startTime.hi+1)    /* multi-sec roll-over */
        {
            if (stopTime.hi < startTime.hi+5)   /* ToD changed so ignore */
            {
                diffTime->hi = stopTime.hi - startTime.hi;  /* total secs */
            }
        }
        diffTime->lo = stopTime.lo + (NANOSECONDS_IN_1SEC - startTime.lo);   /* add nanos */
    }

    if (stopTime.hi == startTime.hi)
    {
        diffTime->lo = stopTime.lo - startTime.lo;  /* Same sec nanos */
    }
}

void zl303xx_CalcNsDelay(Uint64S runTimeNs, Uint64S usualDelayNs, Uint64S *newDelayNs)
{
    if (runTimeNs.lo > 2 * usualDelayNs.lo) /* More than twice as long as expected - ignore */
    {
        newDelayNs->lo = usualDelayNs.lo;   /* use usual delay */
        return;
    }
    if (runTimeNs.lo > usualDelayNs.lo)     /* runtime exceeds usual - as expected */
        newDelayNs->lo = usualDelayNs.lo - (runTimeNs.lo - usualDelayNs.lo);  /* so remove runtime */
    else
        newDelayNs->lo = usualDelayNs.lo;   /* or use usual */
}


/*****************   END   ****************************************************/
