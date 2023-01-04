

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Example code to start up APR.
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include <time.h>
#include <string.h>
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Apr.h"
#include "zl303xx_ExampleAprGlobals.h"
#include "zl303xx_ExampleMain.h"
#include "zl303xx_ExampleAprBinding.h"
#include "zl303xx_ExampleApr.h"
#include "zl303xx_DebugApr.h"

#include "BPF_HM_DEVC.h"
#include "BPF_COM_LOG.h"
#include "f_rrh_type.h"
#include "f_du_reg_eth.h"

#if defined ZLS30390_INCLUDED
#include "zl303xx_ExamplePtpGlobals.h"
#endif

/* 必ず使用するので活性化 2018/6/6 */
/* #if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX */
#include "zl303xx_ZLE30360.h"
/* #endif */



#if defined APR_INCLUDED
#include "zl303xx_Apr1Hz.h"
#include "zl303xx_ExampleUtils.h"
#endif

#if defined ZLS30341_INCLUDED
#include "zl303xx_Example34x.h"
#include "zl303xx_Dpll34xDpllLow.h"
#include "zl303xx_TodMgrApi.h"
#endif
#if defined ZLS30361_INCLUDED
#include "zl303xx_Example36x.h"
#include "zl303xx_Dpll36x.h"
#include "zl303xx_Dpll361.h"
#endif
#if defined ZLS30701_INCLUDED
#include "zl303xx_Example70x.h"
#include "zl303xx_Dpll70x.h"
#include "zl303xx_Dpll701.h"
#endif
#if defined ZLS30721_INCLUDED
#include "zl303xx_Example72x.h"
#include "zl303xx_Dpll72x.h"
#include "zl303xx_Dpll721.h"
#endif
#if defined ZLS30751_INCLUDED
#include "zl303xx_Example75x.h"
#include "zl303xx_Dpll75x.h"
#include "zl303xx_Dpll751.h"
#endif
#if defined ZLS30771_INCLUDED
#include "zl303xx_Example77x.h"
#include "zl303xx_Dpll77x.h"
#include "zl303xx_Dpll771.h"
#endif

/* RTC書込み関数用定義置き場 */
/* #include "zl303xx_ExampleFj.h" */

/*****************   DEFINES     **********************************************/
#ifndef ZL303XX_CHECK_POINTER
#define ZL303XX_CHECK_POINTER(p) \
           ((p == NULL) \
               ? (ZL303XX_TRACE_ERROR("Invalid pointer: "#p, 0,0,0,0,0,0), \
                     ZL303XX_PARAMETER_INVALID) \
               : (ZL303XX_OK))
#endif

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

zlStatusE exampleAprProcessTimestamp(void *clkGenId,
                              Uint16T serverId,
                              zl303xx_AprTimeRepresentationS *txTs,
                              zl303xx_AprTimeRepresentationS *rxTs,
                              zl303xx_BooleanE fwdPath);


/*****************   STATIC GLOBAL VARIABLES   ********************************/

zl303xx_BooleanE useFwdAndRev = ZL303XX_TRUE;   /* Normal to use both, which is also default */

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
extern Uint8T TARGET_DPLL;
extern exampleAppS zlExampleApp;


/*****************   EXPORTED GLOBAL DEFINITIONS   **************************/
/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p);

/*****************   GLOBAL FUNCTION DEFINITIONS   ****************************/

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
/*

  Function Name:
   exampleAprAddCustomAprInitParams

  Details:
   Allow the user to set their own custom device parameters to modify the defaults

  Parameters:
   [in]    Pointer to the zl303xx_AprInitS

  Return Value:
   zlStatusE

 *****************************************************************************/
static zlStatusE exampleAprAddCustomAprInitParams(zl303xx_AprInitS *aprInit);
static zlStatusE exampleAprAddCustomAprInitParams(zl303xx_AprInitS *aprInit)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprAddCustomAprInitParams";

    if ((status = ZL303XX_CHECK_POINTER(aprInit)) != ZL303XX_OK)
    {
       ZL303XX_TRACE_ALWAYS("%s: Invalid device pointer (%p)", fnName, aprInit, 0,0,0,0);
    }

    /* Change any APR init defaults here. */
    if (status == ZL303XX_OK)
    {

       /*
       ZL303XX_APR_MAX_NUM_DEVICES = 1                         - Maximum number of clock generation device in APR;
       ZL303XX_APR_MAX_NUM_MASTERS = 8                         - Maximum number of packet/hybrid server clock for each clock device;
       aprInit->logLevel = 1                                  - APR log level

       DEPRECATED in 5.0.0 (replaced by aprInit->userDelayFunc) - aprInit->PFInitParams.useHardwareTimer = ZL303XX_FALSE; - Use PSLFCL and Sample delay binding (see aprAddDevice)
       DEPRECATED in 5.0.0 (replaced by aprInit->userDelayFunc) - aprInit->PFInitParams.userDelayFunc = (swFuncPtrUserDelay)&exampleUserDelayFunc; - Hook an EXAMPLE delay binding to handler
       DEPRECATED in 5.0.0 (replaced by aprInit->userDelayFunc) - aprInit->AprSampleInitParams.userDelayFunc = (swFuncPtrUserDelay)&exampleUserDelayFunc; - Hook an EXAMPLE delay binding to handler

       Overrides for APR task priority from defaults as defined in zl303xx_Global.h
       DEPRECATED in 5.0.0 - aprInit->aprAdTaskPriority = xx; - Override APR top level task priority (ZL303XX_APR_AD_TASK_PRIORITY)
       aprInit->aprDelayTaskPriority = xx;                       - Override APR DELAY level task priority (ZL303XX_APR_DELAY_TASK_PRIORITY)
       aprInit->aprQIFTaskPriority = xx;                         - Override APR QIF level task priority (ZL303XX_APR_QIF_TASK_PRIORITY)
       DEPRECATED in 5.0.0 - aprInit->PFInitParams.PFTaskPriority = xx;                - Override PF module task priority (ZL303XX_PF_TASK_PRIORITY)
       aprInit->AprSampleInitParams.AprSampleTaskPriority = xx;  - Override APR sample module task priority (ZL303XX_APR_Sample_TASK_PRIORITY)

       Overrides for APR stack size from defaults as defined in zl303xx_Global.h
       DEPRECATED in 5.0.0 - aprInit->aprAdTaskStackSize = xx; - Override APR top level stack size (ZL303XX_APR_AD_TASK_STACK_SIZE )
       aprInit->aprDelayTaskStackSize = xx;                      - Override APR DELAY stack size (ZL303XX_APR_DELAY_TASK_STACK_SIZE )
       aprInit->aprQIFTaskStackSize = xx;                        - Override APR QIF level stack size (ZL303XX_APR_QIF_TASK_STACK_SIZE )
       DEPRECATED in 5.0.0 - aprInit->PFInitParams.PFTaskStackSize = xx;               - Override PF module stack size (ZL303XX_PF_TASK_STACK_SIZE)
       DEPRECATED in 5.0.0 - aprInit->AprSampleInitParams.AprSampleTaskStackSize = xx; - Override APR sample stack size (ZL303XX_APR_Sample_TASK_STACK_SIZE)
       */

       /* This section overrides default APR timer periods in ms
          see ZL303XX_APR_TIMER1_PERIOD_MS and ZL303XX_APR_TIMER2_PERIOD_MS in zl303xx_Apr.h. */
       /* System performance depends of these values */
       if (zl303xx_AprGetAprTaskBasePeriodMs() % zl303xx_AprGetPslFclTaskBasePeriodMs() != 0)
       {
          ZL303XX_TRACE_ALWAYS("exampleAprAddCustomAprInitParams: Warning, APR task perid = %d ms is not a multiple of the PSLFCL task period = %d ms.",
                              zl303xx_AprGetAprTaskBasePeriodMs(), zl303xx_AprGetPslFclTaskBasePeriodMs(),0,0,0,0);
          ZL303XX_TRACE_ALWAYS("exampleAprAddCustomAprInitParams: zl303xx_AprGetAprTaskBasePeriodMs() value will be modified internally to obtain a compatible period.",
                              zl303xx_AprGetAprTaskBasePeriodMs(),0,0,0,0,0);
       }
       if (status == ZL303XX_OK)
       {
          aprInit->aprTimer1PeriodMs = zl303xx_AprGetAprTaskBasePeriodMs();
          aprInit->aprTimer2PeriodMs = zl303xx_AprGetPslFclTaskBasePeriodMs();
       }

        /* Add the following line to run APR from a delay task ("zlAprDelayTask") rather than the DEFAULT H/W timer */
        /* aprInit->userDelayFunc = (swFuncPtrUserDelay)&exampleUserDelayFunc; */
    }

    return status;
}

/*

  Function Name:
   AddCustomAprAddDeviceParams

  Details:
   Allow the user to set their own custom device parameters to modify the defaults

  Parameters:
   [in]    Pointer to the exampleAprClockCreateS

  Return Value:
   zlStatusE

 *****************************************************************************/
static zlStatusE AddCustomAprAddDeviceParams(zl303xx_AprAddDeviceS *aprDevice);
static zlStatusE AddCustomAprAddDeviceParams(zl303xx_AprAddDeviceS *aprDevice)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "AddCustomAprAddDeviceParams";

    if ((status = ZL303XX_CHECK_POINTER(aprDevice)) != ZL303XX_OK)
    {
       ZL303XX_TRACE_ALWAYS("%s: Invalid device pointer (%p)", fnName, aprDevice, 0,0,0,0);
    }

    /* Change any APR device defaults here. */
    if (status == ZL303XX_OK)
    {

      /* Custom settings ... */
      /* Change default APR settings (default: packet timing). */
      /*
      zlStatusE zl303xx_AprSetDeviceOptMode(zl303xx_AprDeviceRefModeE);
      */

      /* Change default Clock Generation Unit (CGU) settings (default: 1). */
      /*
      zlStatusE zl303xx_AprSetHwDcoResolution(Uint32T);
      */

      /* Need to set the CGU clock resolution here. Below are some examples:

         1) below is based on a timestamping PHY
            - PHY dco resolution is ((2^-32ns/8ns)*10^12)

            #define CGU_UPDATE_CYCLE_NS           (Sint64T)8               - PHY uses a 8ns cycle (125MHz clock)
            #define CGU_FRAC_NS_UNITS_INVERTED    (Sint64T)1<<32           - PHY uses 2^-32ns units
            #define CGU_UPPM_UNITS                (Sint64T)1000000000000   - 10^12

            static Sint64T temp1 = (CGU_FRAC_NS_UNITS_INVERTED * CGU_UPDATE_CYCLE_NS);

            status = zl303xx_AprSetHwDcoResolution((Sint32T)(CGU_UPPM_UNITS / temp1));

         2) below is based on a timestamping NPU (where Timestamp Clock = Fnominal/(divider+frac_divider*2^-32))
            - NPU dco resolution is ((1-(d/(d+frac*2^-32)))*10^12) or ~((2^-32/d)*10^12); where d is a static divider

            #define CGU_DIVIDER                   (Sint64T)5               - DCO uses Fnominal/(5+frac_divider*2^-32))
            #define CGU_FRAC_UNITS_INVERTED       (Sint64T)1<<32           - DCO uses 2^-32 fractional units
            #define CGU_UPPM_UNITS                (Sint64T)1000000000000   - 10^12

            static Sint64T temp1 = (CGU_FRAC_UNITS_INVERTED * CGU_DIVIDER);

            status = zl303xx_AprSetHwDcoResolution((Sint32T)(CGU_UPPM_UNITS / temp1));
      */

      /* Change default APR settings related to entering/exiting PHASE LOCK state (default: 1us, 3600s). */
      /*
      zlStatusE zl303xx_AprSetEnterPhaseLockStatusThreshold(Uint32T);
      zlStatusE zl303xx_AprSetExitPhaseLockStatusThreshold(Uint32T);
      */

      /* Change default APR settings for warm start (default: disabled). */
      /*
      zlStatusE zl303xx_AprSetWarmStart(zl303xx_BooleanE);
      zlStatusE zl303xx_AprSetWsInitialOffset(Sint32T);
      */

    }

    return status;
}
#endif


/*

  Function Name:
   AddCustomAprServerParams

  Details:
   Allow the user to set their own custom device parameters to modify the defaults

  Parameters:
   [in]    Pointer to the zl303xx_AprAddServerS

  Return Value:
   zlStatusE

 *****************************************************************************/
static zlStatusE AddCustomAprServerParams(zl303xx_AprAddServerS *aprServer);
static zlStatusE AddCustomAprServerParams(zl303xx_AprAddServerS *aprServer)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "AddCustomAprServerParams";

    if ((status = ZL303XX_CHECK_POINTER(aprServer)) != ZL303XX_OK)
    {
       ZL303XX_TRACE_ALWAYS("%s: Invalid device pointer (%p)", fnName, aprServer, 0,0,0,0);
    }

    /* Change any APR server defaults here. */
    if (status == ZL303XX_OK)
    {
   #if defined ZLS30390_INCLUDED
        zl303xx_AprSetUseReversePath(zl303xx_GetUseDelayResponseFlag());                /* Use reverse if used by PTP */
   #else
        zl303xx_AprSetUseReversePath(useFwdAndRev);                                   /* Use reverse path by default */
   #endif

 #if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
       if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE ))
       {
           /* Change default state for 1Hz (default: disabled). */
           /*
           zlStatusE zl303xx_AprSet1HzEnabled(zl303xx_BooleanE);
           */

           /* Change default server clock source */
           /*
           zlStatusE zl303xx_AprSetHybridServerFlag(zl303xx_BooleanE);           - Server clock is hybrid or not
           */

           /* Change default APR settings for PIR (default: 12ppm). */
           /*
           zlStatusE zl303xx_AprSetPullInRange(zl303xx_AprPullInRangeE);
           */
       }
 #endif
       /* Change default state for 1Hz (default: disabled). */
       /*
       zlStatusE zl303xx_AprSet1HzEnabled(zl303xx_BooleanE);
       */

       /* Change default server clock source */
       /*
       zlStatusE zl303xx_AprSetHybridServerFlag(zl303xx_BooleanE);           - Server clock is hybrid or not
       */

       /* Change default APR settings for PIR (default: 12ppm). */
       /*
       zlStatusE zl303xx_AprSetPullInRange(zl303xx_AprPullInRangeE);
       */

       /* Change default APR settings (default: TCXO, G.8261 network, phase & frequency tracking). */
        /*
        zlStatusE zl303xx_AprSetOscillator(zl303xx_AprOscillatorTypesE, Uint32T);
        zlStatusE zl303xx_AprSetNetwork(zl303xx_AprNetworkTypesE);
        zlStatusE zl303xx_AprSetTracking(zl303xx_AprTrackingTypesE);
        */

        /* Change default APR settings for holdover GST (default: 0s). */
        /*
        zlStatusE zl303xx_AprSetEnterHoldoverGST(Uint32T);
        */

        /* Change default Time Stamp Unit (TSU) settings (default: PTP). */
        /*
        zlStatusE zl303xx_AprSetTsFormat(zl303xx_AprTsFormatE);               - Timing protocol
        zlStatusE zl303xx_AprSet32BitTsFlag(zl303xx_BooleanE);                - 32-bit timestamp or not
        */

        /* Change default packet/clock rate (default: 64pps/16pps) and specify if there is a reverse
         * timing path (default: TRUE). */
        /*
        zlStatusE zl303xx_AprSetUseReversePath(zl303xx_BooleanE);             - Use reverse path or not
        */

    }

    return status;
}


/* exampleAprProcessTimestamp */
/**
   An example of how to pass time stamps into APR. Assumes no correction field.

*******************************************************************************/
zlStatusE exampleAprProcessTimestamp(void *clkGenId,
                              Uint16T serverId,
                              zl303xx_AprTimeRepresentationS *txTs,
                              zl303xx_AprTimeRepresentationS *rxTs,
                              zl303xx_BooleanE fwdPath)
{
   zl303xx_AprTimestampS aprTs;

   memset(&aprTs, 0, sizeof(aprTs));

   aprTs.serverId = serverId;
   aprTs.txTs = *txTs;
   aprTs.rxTs = *rxTs;
   aprTs.bForwardPath = fwdPath;

   aprTs.corr.hi = 0;
   aprTs.corr.lo = 0;

   return zl303xx_AprProcessTimestamp(clkGenId, &aprTs);
} /* END exampleAprProcessTimestamp */


/* exampleAprSetTimeTsu */
/**
   An example of Setting Time Stamper Unit to second, trrigered from Apr.

*******************************************************************************/
Sint32T exampleAprSetTimeTsu(void *clkGenId, Uint64S deltaTimeSec, Uint32T deltaTimeNanoSec,
							 zl303xx_BooleanE negative)
{
	ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "exampleAprSetTimeTsu() arrived: sndhi=%u,sndlo=%u,nano=%u,negative=%u",
				  deltaTimeSec.hi, deltaTimeSec.lo, deltaTimeNanoSec, negative,0,0);
	
	zlStatusE status = ZL303XX_OK;

	if (clkGenId || deltaTimeSec.lo || deltaTimeNanoSec || negative) {;} /* Warning removal */


#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD)
    {
     #if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
        status = zl303xx_VTSS_set_time_delta(deltaTimeSec, deltaTimeNanoSec, negative);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAprSetTimeTsu: ERROR zl303xx_VTSS_set_time_delta returned status %d",
                 status, 0,0,0,0,0);
        }
     #else
        Uint64S tsuSec;
        Uint32T tsuNsec;

        /* Wait for large enough window to set the new time (100 msec before next 1PPS) */
        if ((status = zl303xx_VTSS_wait_for_1pps_window(100, 2000)) == ZL303XX_TIMEOUT)
        {
            printf("exampleAprSetTimeTsu: Timeout waiting for TSU set time window!\n");
        }

        /* Get the current time */
        if (status == ZL303XX_OK)
        {
            if ((status = zl303xx_VTSS_get_time(&tsuSec, &tsuNsec)) != ZL303XX_OK)
                printf("exampleAprSetTimeTsu: Call to zl303xx_VTSS_get_time() failed=%d\n", status);
        }

        /* Set the new TSU time */
        if (status == ZL303XX_OK)
        {
            Uint64S newSec = {0,0};

            /* Set the next 1PPS edge TSU time, setting nanoseconds to 0 to keep aligned with external pulse */
            newSec.hi = tsuSec.hi + (negative ? -deltaTimeSec.hi : deltaTimeSec.hi);
            newSec.lo = tsuSec.lo + (negative ? -deltaTimeSec.lo : deltaTimeSec.lo) + 1;
            if ((status = zl303xx_VTSS_set_time(newSec, 0)) != ZL303XX_OK)
                printf("exampleAprSetTimeTsu: Call to zl303xx_VTSS_set_time() failed=%d\n", status);
        }
    #endif
    }
#else

	Uint64S			tsuSec;
	Uint32T			tsuNsec;
	unsigned int	val;

	long long		tsuSec64;
	long long		deltaTimeSec64;
	long long		newSec64;

	zl303xx_AprTimeRepresentationS	cguOffset = {{0,0},0};


	/* Wait for large enough window to set the new time (100 msec before next 1PPS) */
	if ((status = zl303xx_VTSS_wait_for_1pps_window(100, 2000)) == ZL303XX_TIMEOUT)
	{
		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"exampleAprSetTimeTsu: Timeout waiting for TSU set time window!\n", 0,0,0,0,0,0);
	}

	/* Get the current time */
	if (status == ZL303XX_OK)
	{
		if ((status = zl303xx_VTSS_get_time(&tsuSec, &tsuNsec)) != ZL303XX_OK)
		{
			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"exampleAprSetTimeTsu: Call to zl303xx_VTSS_get_time() failed=%d\n", status,0,0,0,0,0);
		}
	}

	if (status != ZL303XX_OK)
	{
		return (Sint32T)status;
	}

	/* Set the next 1PPS edge TSU time, setting nanoseconds to 0 to keep aligned with external pulse */
	tsuSec64       = (long long)(((long long)(tsuSec.hi) << 32) | ((long long)(tsuSec.lo) & 0xFFFFFFFF));
	deltaTimeSec64 = (long long)(((long long)(deltaTimeSec.hi) << 32) | ((long long)(deltaTimeSec.lo) & 0xFFFFFFFF));

	newSec64       = tsuSec64 + (negative ? -deltaTimeSec64 : deltaTimeSec64) + 1;

	cguOffset.second.hi = (Uint32T)((newSec64 >> 32) & 0xFFFFFFFF);
	cguOffset.second.lo = (Uint32T)(newSec64 & 0xFFFFFFFF);
	cguOffset.subSecond = 0 + LTC_LATCH_DELAY_NS ;		/* nanosec : ラッチ遅延時間	*/

	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDSH, &(cguOffset.second.hi));
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDSL, &(cguOffset.second.lo));
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDN,  &(cguOffset.subSecond));

	/* Enableビットを立てる	*/
	val = 0x00000001;
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDEN, &val);

	char ptplog_path[] = "/proc/rru/ptplog";
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"SetTimeTsu deltaTime-Sec.Hi:%u Lo:%u NSec:%u (negative:%d)", deltaTimeSec.hi, deltaTimeSec.lo, deltaTimeNanoSec, negative );
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"SetTimeTsu        tsuSec-Hi:%u Lo:%u Nsec:%u",tsuSec.hi, tsuSec.lo, tsuNsec );
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"SetTimeTsu     cguOffset-Hi:%u Lo:%u subSec:%u",cguOffset.second.hi, cguOffset.second.lo, cguOffset.subSecond  );

	/* ハード設定値を確認 */
	ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "exampleAprSetTimeTsu() HardWrite : sndhi=%u,sndlo=%u,nano=%u",
				  cguOffset.second.hi, cguOffset.second.lo, cguOffset.subSecond, 0,0,0);


	/* RTC側にも書き込む */
	T_RRH_SYSTEM_TIME	tDateTime;
	struct tm			ltm;
	time_t				longsec;
	int					oal_rtn;


	longsec = (((time_t)(cguOffset.second.hi) << 32) + cguOffset.second.lo) ;
	localtime_r(&longsec, &ltm);

	tDateTime.msec = ( cguOffset.subSecond / 1000000 ) / 10;	/* RTCには0~99 */
	tDateTime.sec = ltm.tm_sec;
	tDateTime.min = ltm.tm_min;
	tDateTime.hour = ltm.tm_hour;
	tDateTime.day = ltm.tm_mday;
	tDateTime.month = ltm.tm_mon + 1;
	tDateTime.year = ltm.tm_year + 1900;

	oal_rtn = BPF_HM_DEVC_SETTIME(tDateTime);
	if( oal_rtn == 0 ) {

		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 1,
					  "exampleAprSetTimeTsu() RTC write: year=%d, month=%d, days=%d, hour=%d, minite=%d, second=%d", 
					  tDateTime.year, tDateTime.month, tDateTime.day, tDateTime.hour, tDateTime.min, tDateTime.sec);

		bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"SetTimeTsu set RTC %04d-%02d-%02d %02d:%02d:%02d:%02d0",
						tDateTime.year,
						tDateTime.month,
						tDateTime.day,
						tDateTime.hour,
						tDateTime.min,
						tDateTime.sec,
						tDateTime.msec);

	} else {

		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 1,
					  "exampleAprSetTimeTsu() (ERROR) RTC write NG(%08x)", oal_rtn, 0, 0, 0, 0, 0 );
	}

#endif
	return (Sint32T)status;
}

/* exampleAprStepTimeTsu */
/**
   An example of Setting Time Stamper Unit to nanosecond, trrigered from Apr.

*******************************************************************************/
Sint32T exampleAprStepTimeTsu(void *clkGenId, Sint32T deltaTimeNs)
{
	zlStatusE status = ZL303XX_OK;

	ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "exampleAprStepTimeTsu() arrived: deltaTimeNs=%d", deltaTimeNs,0,0,0,0,0);

	if (!clkGenId) {;} /* Warning removal */
	if (deltaTimeNs) {;} /* Warning removal */


#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
    if (status == ZL303XX_OK)
    {
        status = zl303xx_VTSS_step_time(deltaTimeNs);
    }
#else
	Uint64S			tsuSec;
	Uint32T			tsuNsec;

	zl303xx_AprTimeRepresentationS cguOffset = {{0,0},0};
	unsigned int val;


	/* Get the current time */
	if ((status = zl303xx_VTSS_get_time_with_step(&cguOffset.second, &cguOffset.subSecond, &tsuSec, &tsuNsec, deltaTimeNs)) != ZL303XX_OK) {

		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"exampleAprStepTimeTsu: Call to zl303xx_VTSS_get_time_with_step() failed=%d\n", status,0,0,0,0,0);
	}

	if( status == ZL303XX_OK ) {

		if ((status = zl303xx_VTSS_wait_for_1pps_window(100, 2000)) == ZL303XX_TIMEOUT)
		{
			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"exampleAprSetTimeTsu: Timeout waiting for TSU set time window!\n", 0,0,0,0,0,0);
		}
	}

	if (status != ZL303XX_OK)
	{
		return (Sint32T)status;
	}

	/* ナノ情報には0またはラッチ遅延時間を書く */
	cguOffset.subSecond = 0 + LTC_LATCH_DELAY_NS ;

	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDSH, &(cguOffset.second.hi));
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDSL, &(cguOffset.second.lo));
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDN,  &(cguOffset.subSecond));

	/* Enableビットを立てる	*/
	val = 0x00000001;
	(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_ETH_SYSTIMLDEN, &val);

	char ptplog_path[] = "/proc/rru/ptplog";
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"StepTimeTsu  deltaTimeNs:%d",deltaTimeNs );
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"StepTimeTsu    tsuSec-Hi:%u Lo:%u Nsec:%u",tsuSec.hi, tsuSec.lo, tsuNsec );
	bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"StepTimeTsu cguOffset-Hi:%u Lo:%u subSec:%u",cguOffset.second.hi, cguOffset.second.lo, cguOffset.subSecond );

	/* ハード設定値を確認 */
	ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "exampleAprStepTimeTsu() HardWrite : sndhi=%u,sndlo=%u,nano=%u",
				  cguOffset.second.hi, cguOffset.second.lo, cguOffset.subSecond, 0,0,0);

	/* RTC側にも書き込む */
	T_RRH_SYSTEM_TIME	tDateTime;
	struct tm			ltm;
	time_t				longsec;
	int					oal_rtn;


	longsec = (((time_t)(cguOffset.second.hi) << 32) + cguOffset.second.lo) ;
	localtime_r(&longsec, &ltm);

	tDateTime.msec = (cguOffset.subSecond / 1000000)/10;	/* RTCには0~99 */
	tDateTime.sec = ltm.tm_sec;
	tDateTime.min = ltm.tm_min;
	tDateTime.hour = ltm.tm_hour;
	tDateTime.day = ltm.tm_mday;
	tDateTime.month = ltm.tm_mon + 1;
	tDateTime.year = ltm.tm_year + 1900;

	oal_rtn = BPF_HM_DEVC_SETTIME(tDateTime);
	if( oal_rtn == 0 ) {

		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
					  "exampleAprStepTimeTsu() RTC write: year=%d, month=%d, days=%d, hour=%d, minite=%d, second=%d", 
					  tDateTime.year, tDateTime.month, tDateTime.day, tDateTime.hour, tDateTime.min, tDateTime.sec);

		bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
							"StepTimeTsu set RTC %04d-%02d-%02d %02d:%02d:%02d:%02d0",
							tDateTime.year,
							tDateTime.month,
							tDateTime.day,
							tDateTime.hour,
							tDateTime.min,
							tDateTime.sec,
							tDateTime.msec);
	} else {
		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 1,
					  "exampleAprStepTimeTsu() (ERROR) RTC write NG(%08x)", oal_rtn, 0, 0, 0, 0, 0 );
	}
#endif
	return (Sint32T)status;
}

/* JumpTimeTsu routine for Microsemi devices */
Sint32T exampleAprJumpTimeTsu(void *clkGenId,
                              Uint64S deltaTimeSec,
                              Uint32T deltaTimeNanoSec,
                              zl303xx_BooleanE negative)
{
   zlStatusE status = ZL303XX_OK;


   /* #warning MSCC: exampleAprJumpTimeTsu() not implemented yet for customer devices! */

   /* MSCC Implementation */
   if(( deltaTimeSec.hi != 0 ) || ( deltaTimeSec.lo != 0 ))
   {
      /* More than 1s, use setTime() */
      status = exampleAprSetTimeTsu(clkGenId, deltaTimeSec, deltaTimeNanoSec, negative);
   }
   else
   {
      /* Otherwise, use stepTime() */
      if( negative == ZL303XX_TRUE )
      {
         status = exampleAprStepTimeTsu(clkGenId, -deltaTimeNanoSec);
      }
      else
      {
         status = exampleAprStepTimeTsu(clkGenId, deltaTimeNanoSec);
      }
   }

   return( status );
}


/**

  Function Name:
   exampleAprDcoSetFreq

  Details:
   Sets the DCO offset based on the frequency offset (in uHz/MHz) provided.

  Parameters:
   [in]  zl303xx_Params      Pointer to the device instance parameter structure
   [in]  freqOffsetInPartsPerTrillion    Frequency offset of the device in units of ppm * 1e6
                                 (i.e. micro-Hz offset per Mega-Hz of frequency).

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDcoSetFreq(void *clkGenId, Sint32T freqOffsetInPartsPerTrillion)
{
   zlStatusE status = ZL303XX_OK;
#if defined STORED_IN_PARAMS
   zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)clkGenId;

   if (!zl303xx_Params)
   {
       ZL303XX_TRACE_ALWAYS("exampleAprDcoSetFreq: Null clkGenId pointer; Abort!",
                          0,0,0,0,0,0);
       return ZL303XX_PARAMETER_INVALID;
   }

   zl303xx_Params->pllParams.dcoFreq = freqOffsetInPartsPerTrillion;
#else
   (void)clkGenId; (void)freqOffsetInPartsPerTrillion;  /* Warning removal */
#endif

   /* #warning MSCC: exampleAprDcoSetFreq() not implemented yet! */

   /* Convert delta freq. (if required) to whatever the clock generation unit (CGU) needs and set it. */
   /* - In general, the conversion formula would be (freqOffsetUppm / hwDcoResolution). Some examples:

         1) below is based on a timestamping PHY
         #define CGU_UPDATE_CYCLE_NS           (Sint64T)8               - PHY uses a 8ns cycle (125MHz clock)
         #define CGU_FRAC_NS_UNITS_INVERTED    (Sint64T)1<<32           - PHY uses 2^-32ns units
         #define CGU_UPPM_UNITS                (Sint64T)1000000000000   - 10^12

         Sint32T dFractionalNsCnt;
         static Sint64T temp1 = (CGU_FRAC_NS_UNITS_INVERTED * CGU_UPDATE_CYCLE_NS);

         - PHY dco resolution is ((2^-32ns/8ns)*10^12), so dFractionalNsCnt = (freqOffsetUppm/29.104)
         dFractionalNsCnt = (Sint32T)((freqOffsetUppm * temp1) / CGU_UPPM_UNITS); */

         zl303xx_AprGetHwDcoResolution();

         /*
         2) below is based on a timestamping NPU (where Timestamp Clock = Fnominal/(divider+frac_divider*2^-32))
         #define CGU_DIVIDER                   (Sint64T)5               - DCO uses Fnominal/(5+frac_divider*2^-32))
         #define CGU_FRAC_UNITS_INVERTED       (Sint64T)1<<32           - DCO uses 2^-32 fractional units
         #define CGU_UPPM_UNITS                (Sint64T)1000000000000   - 10^12

         Sint32T dFractionalCnt;
         static Sint64T temp1 = (CGU_FRAC_UNITS_INVERTED * CGU_DIVIDER);

         - NPU dco resolution is ((1-(d/(d+frac*2^-32)))*10^12) or ~((2^-32/d)*10^12); where d is a static divider
         dFractionalCnt = (Sint32T)((freqOffsetUppm * temp1) / CGU_UPPM_UNITS);

   */

#if defined STORED_IN_PARAMS
   if (status == ZL303XX_OK)  /* Save last df value */
   {
      zl303xx_Params->pllParams.dcoFreq = freqOffsetInPartsPerTrillion;
   }
#endif

   return (Sint32T)status;
}  /* END exampleAprDcoSetFreq */

/**

  Function Name:
*  exampleAprDcoGetFreq

  Details:
   Gets the current DCO offset and converts it to an equivalent frequency
   offset (in units of uHz/MHz).

   PLL frequency offset

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    freqOffsetInPartsPerTrillion    Frequency offset of the device in units of ppm * 1e6
                        (i.e. micro-Hz offset per Mega-Hz of frequency).

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDcoGetFreq(void *clkGenId, Sint32T *freqOffsetInPartsPerTrillion)
{
   zlStatusE status = ZL303XX_OK;
#if defined STORED_IN_PARAMS
   zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)clkGenId;

   if (!zl303xx_Params)
   {
       ZL303XX_TRACE_ALWAYS("exampleAprDcoGetFreq: Null clkGenId pointer; Abort!",
                          0,0,0,0,0,0);
       return ZL303XX_PARAMETER_INVALID;
   }
#else
   (void)clkGenId;  /* Warning removal */
#endif
   if (!freqOffsetInPartsPerTrillion)
   {
       ZL303XX_TRACE_ALWAYS("exampleAprDcoGetFreq: Null freqOffsetInPartsPerTrillion pointer; Abort!",
                          0,0,0,0,0,0);
       return ZL303XX_PARAMETER_INVALID;
   }

#if defined STORED_IN_PARAMS
   if (status == ZL303XX_OK)
   {
       *freqOffsetInPartsPerTrillion = zl303xx_Params->pllParams.dcoFreq; /* Return last used */
   }
#endif

   return (Sint32T)status;
}  /* END exampleAprDcoGetFreq */

/**

  Function Name:
  exampleAprGetHwLockStatus

  Details:
   Gets the current hardware lock status (used by APR)

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprGetHwLockStatus(void *hwParams, Sint32T *manStatus)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprGetHwLockStatus() not implemented yet! */

   *manStatus = ZL303XX_FALSE; /* Return something */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprGetHwManualFreerunStatus

  Details:
   Gets the current manual freerun status (used by APR)

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprGetHwManualFreerunStatus(void *hwParams, Sint32T *manStatus)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprGetHwManualFreerunStatus() not implemented yet! */

   *manStatus = ZL303XX_FALSE; /* Return something */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprGetHwManualHoldoverStatus

  Details:
   Gets the current manual holdover status (used by APR)

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprGetHwManualHoldoverStatus(void *hwParams, Sint32T *manStatus)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprGetHwManualHoldoverStatus() not implemented yet! */

   *manStatus = ZL303XX_FALSE; /* Return something */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprDefaultgetHwSyncInputEnStatus

  Details:
   Gets the current hardware sync input enable status (used by APR)

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDefaultgetHwSyncInputEnStatus(void *hwParams, Sint32T *manStatus)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprDefaultgetHwSyncInputEnStatus() not implemented yet! */

   *manStatus = ZL303XX_FALSE; /* Return something */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprDefaultgetHwOutOfRangeStatus

  Details:
   Gets the current hardware out of range status (used by APR)

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDefaultgetHwOutOfRangeStatus(void *hwParams, Sint32T *manStatus)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprDefaultgetHwOutOfRangeStatus() not implemented yet! */

   *manStatus = ZL303XX_FALSE; /* Return something */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprDefaultSwitchToPacketRef

  Details:
   Switch the current DPLL reference to packet mode

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDefaultSwitchToPacketRef(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprDefaultSwitchToPacketRef() not implemented yet! */

   return (Sint32T)status;
}

/**

  Function Name:
  exampleAprDefaultSwitchToElectricalRef

  Details:
   Switch the current reference to electrical mode

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    manStatus    Status
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/
Sint32T exampleAprDefaultSwitchToElectricalRef(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;

   (void)hwParams;  /* Warning removal */

   /* #warning MSCC: exampleAprDefaultSwitchToElectricalRef() not implemented yet! */

   return (Sint32T)status;
}


/** exampleAdjustTime

   This routine is a simple example of a user-defined callback for adjustTime().

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure
   [in]   t                The adjustment
   [in]   recomendedTime   The recommended time to apply the adjustment

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleAdjustTime
            (
            void *hwParams,
            Sint32T t,
            Uint32T recomendedTime
            );
Sint32T exampleAdjustTime
            (
            void *hwParams,
            Sint32T t,
            Uint32T recomendedTime
            )
{
   Sint32T status = 0;


   if (hwParams) {;}
   if (t) {;}
   if (recomendedTime) {;}



   return( status );
}



/** exampleAdjModifier

   This routine is a simple example of a user-defined callback that is called
   before setTime(), stepTime(), and adjustTime().

   The user can accept, reject, or modify the passed-in adjustment.

  Parameters:
   [in]     hwParams          Pointer to the device structure
   [in,out] adjModifierData   The provided adjustment (in)
                                 The modified adjustment (out)

  Return Value: Sint32T

*****************************************************************************/
zl303xx1HzAdjModifierActionE example1HzAdjModifierAction = MA_accept;
Uint32T example1HzAdjModifierSeconds_hi = 0;
Uint32T example1HzAdjModifierSeconds_lo = 0;
Uint32T example1HzAdjModifierNanoSeconds = 0;
zl303xx_BooleanE example1HzAdjModifierBBackwardAdjust = ZL303XX_FALSE;
zl303xxoverrideAdjustmentTypeE example1HzAdjModifierOverrideAdjType = zl303xx_OAT_default;

Sint32T exampleAdjModifier
            (
            void *hwParams,
            zl303xx1HzAdjModifierDataS *adjModifierData
            );
Sint32T exampleAdjModifier
            (
            void *hwParams,
            zl303xx1HzAdjModifierDataS *adjModifierData
            )
{
   Sint32T status = 0;


   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(adjModifierData);
   }


   if (status == ZL303XX_OK)
   {
       adjModifierData->action = example1HzAdjModifierAction;

       if( adjModifierData->action == MA_useAlternateValue )
       {
          adjModifierData->seconds.hi = example1HzAdjModifierSeconds_hi;
          adjModifierData->seconds.lo = example1HzAdjModifierSeconds_lo;
          adjModifierData->nanoSeconds = example1HzAdjModifierNanoSeconds;
          adjModifierData->bBackwardAdjust = example1HzAdjModifierBBackwardAdjust;
          adjModifierData->overrideAdjType = example1HzAdjModifierOverrideAdjType;
       }
   }


#if defined ZLS30721_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
   {
      Uint32T stepThr;

      if( status == ZL303XX_OK )
      {
         zl303xx_PFConfigS par;

         status = zl303xx_AprGetPSLFCLConfigureData(hwParams, &par);
         if( status == ZL303XX_OK )
         {
            stepThr = par.stepTimeAdjustFreqThreshold;
         }
      }

      /* Depending on the 72x's configuration, the 72x device may not be able
         to jump the exact amount requested or it may not be able to jump at
         all. This code either :
         1) modifies the requested adjustment to a value that the 72x device
            can jump exactly
         or
         2) sets the override so that adjFreq() is used for this adjustment
      */

      if( status == ZL303XX_OK )
      {
         if( adjModifierData->nanoSeconds >= stepThr )
         {
            /* This adjustment size will cause a call to the 72x's stepTime()
               function; the size must be adjusted. */

            Sint32T newNsValue;
            zl303xx_BooleanE canBeDone;

            adjModifierData->action = MA_useAlternateValue;

            newNsValue = adjModifierData->nanoSeconds;
            if( adjModifierData->bBackwardAdjust == ZL303XX_TRUE )
            {
               newNsValue = -newNsValue;
            }

            status = zl303xx_Dpll72xModStepSizeFor72x(hwParams, &newNsValue, &canBeDone);

            if( status == ZL303XX_OK )
            {
               if( (Uint32T)abs(newNsValue) >= stepThr )
               {
                  if( canBeDone == ZL303XX_TRUE )
                  {
                     /* The adjustment is possible - set the new value */
                     adjModifierData->nanoSeconds = abs(newNsValue);
                  }
                  else
                  {
                     /* The adjustment is not possible. We have 2 scenarios here:
                        1) the seconds field is NOT 0
                        2) the seconds field is 0

                        If the seconds field is NOT 0, then we discard the
                        nanoseconds. This will, of course, only set the seconds
                        field: The next adjustment should be less than 1s and be
                        handled by a different part of this routine.

                        If the seconds field is 0, then we must apply this adjustment
                        using adjFreq() instead of stepTime(). This should be ok
                        since the adjustment should be fairly small by this point.

                        Note that adjTime() would also be possible when the seconds
                        field in 0. However, adjTime() is not recommended so it will
                        not be specified here.
                     */
                     if(( adjModifierData->seconds.hi != 0 ) ||
                        ( adjModifierData->seconds.lo != 0 ))
                     {
                        adjModifierData->nanoSeconds = 0;
                     }
                     else
                     {
                        adjModifierData->overrideAdjType = zl303xx_OAT_useAdjFreq;
                     }
                  }
               }
               else
               {
                  /* The old value was above the threshold but the new value is
                     below the threshold. Use the old value but force APR to
                     use adjFreq() to do the adjustment. */
                  adjModifierData->overrideAdjType = zl303xx_OAT_useAdjFreq;
               }
            }
         }
         else
         {
            /* The adjustment is too small to do a jump so allow it to happen
               the normal way. */
         }
      }
   }
#endif


#if defined ZLS30771_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
   {
      Uint32T stepThr;
      zl303xx_BooleanE modifyAdustment = ZL303XX_TRUE;


      if( status == ZL303XX_OK )
      {
         Uint32T phaseStepMaskGp;
         Uint32T phaseStepMaskHp;
         ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
         ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;

         if( zl303xx_Dpll77xPhaseStepMaskGet(hwParams, &phaseStepMaskGp, &phaseStepMaskHp,
                     &outputTypeThatDrivesTimeStamper, &outputNumThatDrivesTimeStamper) == ZL303XX_OK )
         {
            if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_GP )
            {
               /* The GP synthisizers do not need modification */
               modifyAdustment = ZL303XX_FALSE;
            }
         }
      }
      if( status == ZL303XX_OK )
      {
         if( modifyAdustment == ZL303XX_TRUE )
         {
            if( status == ZL303XX_OK )
            {
               zl303xx_PFConfigS par;

               status = zl303xx_AprGetPSLFCLConfigureData(hwParams, &par);
               if( status == ZL303XX_OK )
               {
                  stepThr = par.stepTimeAdjustFreqThreshold;
               }
            }

            /* Depending on the 77x's configuration, the 77x device may not be able
               to jump the exact amount requested. This code modifies the requested
               adjustment to a value that the 77x device can jump exactly.
            */
            if( status == ZL303XX_OK )
            {
               if( adjModifierData->nanoSeconds >= stepThr )
               {
                  /* This adjustment size will cause a call to the 77x's stepTime()
                     function; the size must be adjusted. */

                  Sint32T oldNsValue;
                  Sint32T newNsValue;

                  oldNsValue = adjModifierData->nanoSeconds;
                  if( adjModifierData->bBackwardAdjust == ZL303XX_TRUE )
                  {
                     oldNsValue = -oldNsValue;
                  }

                  status = zl303xx_Dpll77xModifyStepTimeNs(hwParams, oldNsValue, &newNsValue);

                  if( status == ZL303XX_OK )
                  {
                     adjModifierData->action = MA_useAlternateValue;

                     if( (Uint32T)abs(newNsValue) >= stepThr )
                     {
                        adjModifierData->nanoSeconds = abs(newNsValue);
                     }
                     else
                     {
                        /* The old value was above the threshold but the new value is
                           below the threshold. Use the old value but force APR to
                           use adjFreq() to do the adjustment. */
                        adjModifierData->overrideAdjType = zl303xx_OAT_useAdjFreq;
                     }
                  }
               }
               else
               {
                  /* The adjustment is too small to do a jump so allow it to happen
                     the normal way. */
               }
            }
         }
      }
   }
#endif




  return( status );
}

/** exampleAdjustTimeDR

   This routine is a simple example of a user-defined callback that is called
   before adjustTime() and returns TRUE or FALSE.

  Parameters:
   [in]   hwParams      Pointer to the device structure
   [in]   adjustment    The adjustment
   [out]  dr            TRUE or FALSE

  Return Value: Sint32T

*****************************************************************************/
Sint32T exampleAdjustTimeDR
            (
            void *hwParams,
            Sint32T adjustment,
            zl303xx_BooleanE *dr
            );

zl303xx_BooleanE exampleAdjustTimeDRFlag = ZL303XX_TRUE;

Sint32T exampleAdjustTimeDR
            (
            void *hwParams,
            Sint32T adjustment,
            zl303xx_BooleanE *dr
            )
{
   Sint32T status = 0;


   if (hwParams) {;}
   if (adjustment) {;}

   *dr = exampleAdjustTimeDRFlag;


   return( status );
}


/** exampleJumpNotification

   This routine is a simple example of a user-defined callback that is called
   to notify the user of a jump time event. Currently, only start and stop
   events are generated.

  Parameters:
   [in]   hwParams         Pointer to the device structure
   [in]   seconds          The adjustment seconds
   [in]   nanoSeconds      The adjustment nanoseconds
   [in]   bBackwardAdjust  The adjustment sign i.e. TRUE = negative
   [in]   jumpEvent        The type of adjustment: setTiem, stepTime, or adjTime

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleJumpNotification
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust,
                     zl303xx_JumpEvent_t jumpEvent
                     );
Sint32T exampleJumpNotification
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust,
                     zl303xx_JumpEvent_t jumpEvent
                     )
{
   Sint32T status = 0;


   if (hwParams) {;}
   if (seconds.hi) {;}
   if (nanoSeconds) {;}
   if (bBackwardAdjust) {;}
   if (jumpEvent) {;}

   /*
      If the redundant APR is on the same card, it may be necessary to
      inform that device that a step is about to happen. The actions may
      be very similar to the jump actions taken by the monitor card in
      exampleReceiveRedundancyData():

      if( jumpEvent == ZL303XX_JET_START )
      {
         zl303xx_RedundancyMsgS msg;

         msg.msgType = zl303xx_RMT_jumpTimeData;
         msg.msgType = 0;
         msg.rd.jumpTimeData.jumpTimeType = zl303xx_JTT_stepTime;
         msg.rd.jumpTimeData.dtSec = seconds;
         msg.rd.jumpTimeData.dtNanoSec = nanoSeconds;
         msg.rd.jumpTimeData.negative = bBackwardAdjust;

         zl303xx_AprProcessRedundancyJumpTimeData(hwParams, 0, &msg);
      }

      NOTE: Before calling these routines, the target system MUST change
            hwParams and serverId from the active's to the hwParams and
            serverId of the monitor's.
   */


   return( status );
}





/** exampleReceiveRedundancyData

   This routine is a simple example of a user-defined callback that is called
   to transport data to the redundant device.

   hwParams and serverId MUST refer to the monitor's device and server ID.

   serverId is not always present; if not present, ZL303XX_NULL_SERVER_ID
   should be given.

  Parameters:
   [in]   hwParams      Pointer to the device structure
   [in]   serverId      The server ID
   [in]   msg           The message

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleReceiveRedundancyData
                     (
                     void *hwParams,
                     Uint16T serverId,
                     zl303xx_RedundancyMsgS *msg
                     );
Sint32T exampleReceiveRedundancyData
                     (
                     void *hwParams,
                     Uint16T serverId,
                     zl303xx_RedundancyMsgS *msg
                     )
{
   Sint32T status = 0;
   zl303xx_BooleanE proxyRxEnabled;


   if( zl303xx_AprGetProxyRxEnabled(hwParams, &proxyRxEnabled) != 0 )
   {
      /* Unusual failure - just return */
      return( 0 );
   }

   if( proxyRxEnabled == ZL303XX_FALSE )
   {
      /* Receiver is not enabled - return */
      return( 0 );
   }

   switch( msg->msgType )
   {
      case zl303xx_RMT_jumpTimeData:

         switch( msg->rd.jumpTimeData.jumpTimeType )
         {
            case zl303xx_JTT_setTime:
            case zl303xx_JTT_stepTime:

               zl303xx_AprProcessRedundancyJumpTimeData(hwParams, serverId, msg);
               break;

            case zl303xx_JTT_adjTimeStart:

               /* Start discarding packets for the recommended time. */
               zl303xx_AprStartAdjTimeDiscardTimer(hwParams, msg->rd.jumpTimeData.recommendedTime);

               /* Process the rest of the message. */
               zl303xx_AprProcessRedundancyAdjustTimeData(hwParams, serverId, msg);
               break;

            case zl303xx_JTT_adjTimeEnd:

               /* The active may send us this message before the timer expires.
                  In this case, we can stop discarding packets immediately. */
               zl303xx_AprStopAdjTimeDiscardTimer(hwParams);
               break;

            default:
               break;
         }
         break;

      case zl303xx_RMT_jumpStandbyCGU:
         /* The active has moved its CGU. If desired, the standby CGU may be moved
            more quickly here.

            If using the ZLE30360 part, routine zl303xx_Dpll36xMtieSnap() can be
            used.

            If using ZLE3034x part, routine zl303xx_TodSyncToExtPulse() can be used.
         */
         break;

      case zl303xx_RMT_adjFreqData:
         zl303xx_AprProcessRedundancyAdjFreqData(hwParams, serverId, msg);
         break;

      case zl303xx_RMT_cData:
         zl303xx_AprProcessRedundancyCDataData(hwParams, serverId, msg);
         break;

      case zl303xx_RMT_stateInfo:
         zl303xx_PFProcessRedundancyStateData(hwParams, serverId, msg);
         break;

      case zl303xx_RMT_last:
      default:
         break;
   }

   return( status );
}


/** exampleSendRedundancyData

   This routine is a stub of a user-defined callback that is called to
   transport data to the redundant device.

      NOTE: Before reaching the monitor device, the target system MUST
            translate hwParams and serverId from the active's to the hwParams
            and serverId of the monitor's so that code at the monitor
            (exampleReceiveRedundancyData()) can access the monitor's context.

   The serverId may be 0xffff indicating that the serverId is not present.
   Translation of 0xffff should not be performed.

  Parameters:
   [in]   hwParams      Pointer to the device structure
   [in]   serverId      The server ID
   [in]   msg           The message

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleSendRedundancyData
                     (
                     void *hwParams,
                     Uint16T serverId,
                     zl303xx_RedundancyMsgS *msg
                     );
Sint32T exampleSendRedundancyData
                     (
                     void *hwParams,
                     Uint16T serverId,
                     zl303xx_RedundancyMsgS *msg
                     )
{
   Sint32T status = 0;

   /* ***********************************************************************
      NOTE: Before reaching the monitor device, the target system MUST
            change hwParams and serverId from the active's to the hwParams
            and serverId of the monitor's so that code at the monitor
            (exampleReceiveRedundancyData()) can access the monitor's context.
   **************************************************************************/

   /* warning removal*/
   if (hwParams) {;}
   if (serverId) {;}
   if (msg) {;}

   return( status );
}


/** exampleJumpActiveCGU

   This routine is a simple example of a user-defined callback that is called
   when active CGU's 1Hz pulse should be jumped.

  Parameters:
   [in]   hwParams         Pointer to the device structure
   [in]   seconds          The adjustment seconds
   [in]   nanoSeconds      The adjustment nanoseconds
   [in]   bBackwardAdjust  The adjustment sign i.e. TRUE = negative

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleJumpActiveCGU
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust
                     );
Sint32T exampleJumpActiveCGU
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust
                     )
{
   Sint32T status = 0;


   if (hwParams) {;}
   if (seconds.hi) {;}
   if (nanoSeconds) {;}
   if (bBackwardAdjust) {;}


   return( status );
}


/** exampleJumpStandbyCGU

   This routine is a simple example of a user-defined callback that is called
   when standby CGU's 1Hz pulse should be jumped.

  Parameters:
   [in]   hwParams         Pointer to the device structure
   [in]   seconds          The adjustment seconds
   [in]   nanoSeconds      The adjustment nanoseconds
   [in]   bBackwardAdjust  The adjustment sign i.e. TRUE = negative

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleJumpStandbyCGU
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust
                     );
Sint32T exampleJumpStandbyCGU
                     (
                     void *hwParams,
                     Uint64S seconds,
                     Uint32T nanoSeconds,
                     zl303xx_BooleanE bBackwardAdjust
                     )
{
   Sint32T status = 0;


   if (hwParams) {;}
   if (seconds.hi) {;}
   if (nanoSeconds) {;}
   if (bBackwardAdjust) {;}


   return( status );
}


/** exampleActivitySwitch

   This routine is a simple example of the steps needed to switch from
   active to monitor for a device.

   The sequence to switch activity is:

   1) active: set holdover
   2) active: set proxy TX Disable
   3) monitor: set proxy RX Disable
   4) monitor: set electrical->packet
   5) monitor: set proxy TX Enable
   6) active: set proxy RX Enable

  Parameters:
   [in]   hwParamsActive         Pointer to the device structure on the active card
                                    of the currently active packet device
   [in]   hwParamsActiveStandby   Pointer to the device structure on the active card
                                    of the standby device (on the other card)
   [in]   hwParamsMonitor        Pointer to the device structure on the monitor card
                                    of the currently active packet device
   [in]   hwParamsMonitorStandby  Pointer to the device structure on the monitor card
                                    of the standby device (on the other card)
   [in]   serverIdMonitor        Server of the monitor that is going to be active

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleActivitySwitch
                     (
                     void *hwParamsActive,
                     void *hwParamsActiveStandby,
                     void *hwParamsMonitor,
                     void *hwParamsMonitorStandby,
                     Uint16T serverIdMonitor
                     );
Sint32T exampleActivitySwitch
                     (
                     void *hwParamsActive,
                     void *hwParamsActiveStandby,
                     void *hwParamsMonitor,
                     void *hwParamsMonitorStandby,
                     Uint16T serverIdMonitor
                     )
{
   Sint32T status = 0;


   /* 1) active: set holdover */

   /*    You can set holdover via a hardware holdover or APR. Via hardware
         is beyond the scope of this example.

         The following command will set APR holdover. */
   if( zl303xx_AprSetHoldover( hwParamsActive, ZL303XX_TRUE ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetHoldover() hwParamsActive failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* 2) active: set proxy TX Disable */

   if( zl303xx_AprSetProxyTxDisabled( hwParamsActive ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyTxDisabled() hwParamsActive failed with status = %lu",
                           status, 0,0,0,0,0);
   }
   if( zl303xx_AprSetProxyTxDisabled( hwParamsActiveStandby ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyTxDisabled() hwParamsActiveStandby failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* 3) monitor: set proxy RX Disable */

   if( zl303xx_AprSetProxyRxDisabled( hwParamsMonitor ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyRxDisabled() hwParamsMonitor failed with status = %lu",
                           status, 0,0,0,0,0);
   }
   if( zl303xx_AprSetProxyRxDisabled( hwParamsMonitorStandby ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyRxDisabled() hwParamsMonitorStandby failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* 4) monitor: set electrical->packet */

   /* This step assumes that the user wants to run packet mode on the newly
      active device. If you want to continue running electrical mode, then
      this step can be skipped. */

   if( zl303xx_AprSetServerMode( hwParamsMonitor, serverIdMonitor, ZL303XX_PACKET_MODE ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetServerMode() failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* 5) monitor: set proxy TX Enable */

   if( zl303xx_AprSetProxyTxEnabled( hwParamsMonitor ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyTxEnabled() hwParamsMonitor failed with status = %lu",
                           status, 0,0,0,0,0);
   }
   if( zl303xx_AprSetProxyTxEnabled( hwParamsMonitorStandby ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyTxEnabled() hwParamsMonitorStandby failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* 6) active: set proxy RX Enable */

   if( zl303xx_AprSetProxyRxEnabled( hwParamsActive ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyRxEnabled() hwParamsActive failed with status = %lu",
                           status, 0,0,0,0,0);
   }
   if( zl303xx_AprSetProxyRxEnabled( hwParamsActiveStandby ) != 0 )
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprSetProxyRxEnabled() hwParamsActiveStandby failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   /* A this point, hwParamsActive - the newly inactive - can be managed
      i.e. it could be removed or reset */


   return( status );
}


/** exampleSyncStandbyCGUTSU

   This routine is an example of the procedure to initialise the active CGU,
   standby CGU, and TSU.


  Parameters:
   [in]

  Return Value: Sint32T

*****************************************************************************/

Sint32T exampleSyncStandbyCGUTSU(void);
Sint32T exampleSyncStandbyCGUTSU(void)
{
   Sint32T status = 0;


   /* 1) Determine the current time. The user must determine this from their
         own data.

         In the case where Microsemi hardware is being used, routine
         zl303xx_PtpClockTimeGet() can be used.

         Note: zl303xx_PtpClockTimeGet() deals with absolute time and therefor
               care must be taken to allow enough real time between
               zl303xx_PtpClockTimeGet() and zl303xx_PtpClockTimeSet() for all
               software and system delays. It is recommended that the user
               wait for a roll-over before proceeding.
   */

   /* 2) Set the active CGU's ToD

         zl303xx_PtpClockTimeSet() can be used for Microsemi equipment.
         Note that this routine can only be called after Microsemi code
         is running and a device has been added.

         Otherwise, proprietary calls must be made to set the ToD.
   */

   /* 3) Set the standby CGU's ToD

         zl303xx_PtpClockTimeSet() can be used for Microsemi equipment.
         Note that this routine can only be called after Microsemi code
         is running and a device has been added.

         Otherwise, proprietary calls must be made to set the ToD.
   */

   /* 4) Update the TSU

         Proprietary calls must be made to re-align the TSU to the CGU.
   */


   return( status );
}


/* exampleAprEnvInit */
/**
   Initializes the APR environment and the global variables used in this
   example code.

*******************************************************************************/
zlStatusE exampleAprEnvInit(void)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_AprInitS aprInit;

   OS_MEMSET(&aprInit, 0, sizeof(aprInit));

   if ((status == ZL303XX_OK) &&
       ((status = zl303xx_AprInitStructInit(&aprInit)) != ZL303XX_OK))
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprInitStructInit() failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   if (status == ZL303XX_OK)
   {
#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
       if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE ) ||
          ( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE ))
       {
           if ((status = exampleAprAddCustomAprInitParams(&aprInit)) != ZL303XX_OK)
           {
                ZL303XX_TRACE_ALWAYS("exampleAprEnvInit: exampleAprAddCustomAprInitParams() failed with status = %lu",
                                     status, 0,0,0,0,0);
           }
       }
#endif
   }



   if ((status == ZL303XX_OK) &&
       (status = zl303xx_AprInit(&aprInit)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprInit() failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   return status;
}


/* exampleAprEnvClose */
/**
   Closes the APR environment and resets global variables used in this
   example code, as necessary.

*******************************************************************************/
zlStatusE exampleAprEnvClose(void)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_AprDeleteS aprDeviceInfo;

   if ((status == ZL303XX_OK) &&
       ((status = zl303xx_AprDeleteStructInit(&aprDeviceInfo)) != ZL303XX_OK))
   {
     ZL303XX_TRACE_ALWAYS("exampleAprStop: Call to zl303xx_AprDeleteStructInit() failure=%d", status, 0,0,0,0,0);
   }

   if ((status == ZL303XX_OK) &&
       ((status = zl303xx_AprDelete(&aprDeviceInfo)) != ZL303XX_OK))
   {
     ZL303XX_TRACE_ALWAYS("exampleAprStop: Call to zl303xx_AprDelete() failure=%d", status, 0,0,0,0,0);
   }

   return status;
}



/* exampleAprClockCreateStructInit */
/**
   An example of how to initialize a APR clock/device configuration structure,
   including necessary function bindings.

*******************************************************************************/
zlStatusE exampleAprClockCreateStructInit(exampleAprClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T j;

   status = ZL303XX_CHECK_POINTER(pClock);

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
  #if defined ZLE30360_USE_ESTIMATE_SEC_INGRESS_TS
    /* Ingress TS (e.g. T2) seconds is available, place APR into full timestamp mode (setTime will be called) */
    zl303xx_AprSet32BitTsFlag(ZL303XX_FALSE);
  #else
    /* Ingress TS (e.g. T2) seconds not available so place APR into sub-second only mode (setTime will not be called) */
    /* NOTE: This mode is really "30-bit" timestamp mode (0-999,999,999 nanoseconds) */
    zl303xx_AprSet32BitTsFlag(ZL303XX_TRUE);
  #endif
#endif

   #if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE ) ||
         ( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE ) ||
         ( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE ) ||
         ( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE ) ||
         ( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE ) ||
         ( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE ))
      {
         if (status == ZL303XX_OK && (status = AddCustomAprAddDeviceParams(&pClock->device)) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleAprClockCreateStructInit: AddCustomAprAddDeviceParams() failed with status = %lu",
                                status, 0,0,0,0,0);
         }
      }
   #endif

   /* The device instance structure for pClock->cguId will be allocated in zl303xx_CreateDeviceInstance() */

   /* Initialize the clock config structure with default values. */
   if (status == ZL303XX_OK)
   {
      memset(&pClock->device, 0, sizeof(pClock->device));
      status = zl303xx_AprAddDeviceStructInit(pClock->cguId,
                                           &pClock->device);
      if (status != ZL303XX_OK)
          ZL303XX_TRACE_ALWAYS("exampleAprClockCreateStructInit: zl303xx_AprAddDeviceStructInit() failed with status = %lu",
                status, 0,0,0,0,0);
   }

   /* Overwrite defaults with values set through example globals. */
   if (status == ZL303XX_OK)
   {
      /*--------- Begin device parameter value assignments ---------*/
      pClock->device.devMode = zl303xx_AprGetDeviceOptMode(); /* PKT, ELEC, HYB */
      pClock->device.devHybridAdjMode = zl303xx_AprGetDeviceHybridAdjMode(); /* Phase, Freq, Custom_Phase, Custom_Freq, None */
      pClock->device.hwDcoResolutionInPpt = zl303xx_AprGetHwDcoResolution();
      pClock->device.enterPhaseLockStatusThreshold = zl303xx_AprGetEnterPhaseLockStatusThreshold();
      pClock->device.exitPhaseLockStatusThreshold = zl303xx_AprGetExitPhaseLockStatusThreshold();
      pClock->device.bWarmStart = zl303xx_AprGetWarmStart();
      pClock->device.warmStartInitialFreqOffset = zl303xx_AprGetWsInitialOffset();
      pClock->device.clkStabDelayLimit = zl303xx_AprGetClkStabDelayIterations();

      pClock->device.cguNotify = &exampleAprCguNotify;  /* Hook the default notify handlers */
      pClock->device.elecNotify = &exampleAprElecNotify;
      pClock->device.serverNotify = &exampleAprServerNotify;
      pClock->device.oneHzNotify = &exampleAprOneHzNotify;

      pClock->device.setTimePacketTreatment = zl303xx_AprGetSetTimePacketTreatment();
      pClock->device.stepTimePacketTreatment = zl303xx_AprGetStepTimePacketTreatment();
      pClock->device.adjustTimePacketTreatment = zl303xx_AprGetAdjustTimePacketTreatment();
      pClock->device.legacyTreatment = zl303xx_AprGetLegacyTreatment();

      pClock->device.PFConfig.lockedPhaseOutlierThreshold = zl303xx_AprGetPFLockedPhaseOutlierThreshold();
      pClock->device.PFConfig.initialFrequencyOffset      = zl303xx_AprGetPFInitialFrequencyOffset();

      pClock->device.PFConfig.fastLock1HzEnable = zl303xx_AprGetFastLock1HzEnable();
      pClock->device.PFConfig.fastLock1HzInterval = zl303xx_AprGetFastLock1HzInterval();
      pClock->device.PFConfig.fastLock1HzTotalPeriod = zl303xx_AprGetFastLock1HzTotalPeriod();
      pClock->device.PFConfig.fastLock1HzDelay = zl303xx_AprGetFastLock1HzDelay();
      pClock->device.PFConfig.stepTimeAdjustTimeThreshold = zl303xx_AprGetStepTimeAdjustTimeThreshold();
      pClock->device.PFConfig.stepTimeAdjustFreqThreshold = zl303xx_AprGetStepTimeAdjustFreqThreshold();
      pClock->device.PFConfig.stepTimeResolution = zl303xx_AprGetPFStepTimeResolution();

      pClock->device.PFConfig.APRFrequencyLockedPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit();
      pClock->device.PFConfig.APRFrequencyNotLockedPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit();
      pClock->device.PFConfig.APRFrequencyFastPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit();
      pClock->device.PFConfig.APRFrequencyLockedFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit();
      pClock->device.PFConfig.APRFrequencyNotLockedFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit();
      pClock->device.PFConfig.APRFrequencyFastFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit();
      for(j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
      {
         zl303xx_AprGetPSL(j, &(pClock->device.PFConfig.adjSize1HzPSL[j]),
                            &(pClock->device.PFConfig.PSL_1Hz[j]) );
      }
      for(j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
      {
         zl303xx_AprGetFCL(j, &(pClock->device.PFConfig.adjSize1HzFCL[j]),
                            &(pClock->device.PFConfig.FCL_1Hz[j]) );
      }
      for(j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
      {
         zl303xx_AprGetAdjScaling(j, &(pClock->device.PFConfig.adjSize1HzAdjScaling[j]),
                            &(pClock->device.PFConfig.adjScaling_1Hz[j]) );
      }
      pClock->device.PFConfig.lockInThreshold = zl303xx_AprGetPFLockInThreshold();
      pClock->device.PFConfig.lockInCount = zl303xx_AprGetPFLockInCount();
      pClock->device.PFConfig.lockOutThreshold = zl303xx_AprGetPFLockOutThreshold();
      pClock->device.PFConfig.adjustFreqMinPhase = zl303xx_AprGetPFAdjustFreqMinPhase();
      pClock->device.PFConfig.adjustTimeMinThreshold = zl303xx_GetAdjustTimeMinThreshold();
      pClock->device.PFConfig.bUseAdjustTimeHybrid = zl303xx_GetUseAdjustTimeHybrid();
      pClock->device.PFConfig.bUseAdjustTimePacket = zl303xx_GetUseAdjustTimePacket();
      pClock->device.PFConfig.stepTimeDetectableThr = zl303xx_GetStepTimeDetectableThr();
      pClock->device.PFConfig.maxAdjFreqTime = zl303xx_AprGetMaxAdjFreqTime();

      pClock->device.PFConfig.AprDynamicPhaseCorrectionEnabled = zl303xx_GetAprDynamicPhaseCorrectionEnabled();
      pClock->device.PFConfig.AprDynamicPhaseCorrectionThr = zl303xx_GetAprDynamicPhaseCorrectionThr();

      pClock->device.PFConfig.adjFreqDynamicAdjustmentEnable = zl303xx_AprGetAdjFreqDynamicAdjustmentEnable();
      pClock->device.PFConfig.hybridLockTimeTarget = zl303xx_AprGetHybridLockTimeTarget();

      pClock->device.useLegacyStreamStartUp = zl303xx_AprGetUseLegacyStreamStartUp();
      pClock->device.make1HzAdjustmentsDuringHoldover = zl303xx_AprGetAllow1HzAdjustmentsInHoldover();

#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
          pClock->device.PFConfig.stepTimeMaxTimePerAdjustment = 490000000;  /* To match 49% phase step max (register 0x1A6 = 0x31) */
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
          pClock->device.PFConfig.stepTimeMaxTimePerAdjustment = 490000000;  /* To match 49% phase step max (register 0x1A6 = 0x31) */
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
          pClock->device.PFConfig.stepTimeMaxTimePerAdjustment = 490000000;  /* To match 49% phase step max (register 0x1A6 = 0x31) */
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
        pClock->device.PFConfig.stepTimeMaxTimePerAdjustment = 490000000;  /* To match 49% phase step max (register 0x1A6 = 0x31) */
      }
#endif


      /*--------- End device parameter value assignments ---------*/


      /* BEGIN: Binding of hardware device message router ------*/
      /* Use the application message router */
      pClock->device.useUserMsgRouter  = ZL303XX_TRUE;
      /* Application Message Router Assignment */
      /* Default Example code message router for Microsemi and custom devices */
      pClock->device.userMsgRouter  = zl303xx_UserMsgRouter;
      /* END: Binding of hardware device message router ------*/


      /* BEGIN: Binding of user application message router ------*/
      /* Use the driver message router */
      pClock->device.useDriverMsgRouter  = ZL303XX_TRUE;
      /* Driver Message Router Assignment */
      pClock->device.driverMsgRouter  = NULL;
#if defined ZLS30341_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE ))
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll34xMsgRouter;
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll36xMsgRouter;
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll70xMsgRouter;
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll75xMsgRouter;
      }
#endif
#if defined ZLS30721_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll72xMsgRouter;
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_Dpll77xMsgRouter;
      }
#endif
      if( zl303xx_GetDefaultDeviceType() == CUSTOM_DEVICETYPE )
      {
         pClock->device.driverMsgRouter = zl303xx_DpllCustomMsgRouter;
      }
      /* END: Binding of user application message router ------*/




   }

   return status;
}



/* exampleAprClockCreate */
/**
   An example of how to start an APR clock/device. An APR clock represents a
   local DCO. Each clock runs as its own OS task.

*******************************************************************************/
zlStatusE exampleAprClockCreate(exampleAprClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD

#if defined ZLS30361_INCLUDED
   /* Keep frame pulse aligned to the clock on synth2/post-div B */
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         Uint32T stepTimeResolution = pClock->device.PFConfig.stepTimeResolution;

         status = zl303xx_Dpll36xGetClockPeriodLcm(pClock->cguId,
                  ZLS3036X_SYNTH2_POST_DIV_B,  /* synth2/post-div B */
                  &pClock->device.PFConfig.stepTimeResolution);

         if (status == ZL303XX_OK && stepTimeResolution != pClock->device.PFConfig.stepTimeResolution)
         {
             ZL303XX_TRACE_ALWAYS("exampleAprClockCreate: Changed stepTimeResolution from %u to %u (synth LCM)",
                             stepTimeResolution,
                             pClock->device.PFConfig.stepTimeResolution, 0,0,0,0);
         }
      }
   }
#endif
#if defined ZLS30701_INCLUDED
#endif
#if defined ZLS30751_INCLUDED
#endif

   /* If the stepTimeResolution returned by zl303xx_Dpll36xGetClockPeriodLcm()
      is larger than stepTimeAdjustTimeThreshold or stepTimeAdjustFreqThreshold,
      then we need to increase stepTimeAdjustTimeThreshold and
      stepTimeAdjustFreqThreshold since it does not make sense for these
      thresholds to be smaller than the resolution of the device.

      The simplest value to assign is stepTimeResolution. */
   {
      zl303xx_PFConfigS *p = &(pClock->device.PFConfig);

      if (status == ZL303XX_OK)
      {
         if (p->stepTimeAdjustTimeThreshold < p->stepTimeResolution)
         {
            p->stepTimeAdjustTimeThreshold = p->stepTimeResolution;

            ZL303XX_TRACE_ALWAYS("******************************************************************************************",
            0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("Warning: stepTimeAdjustTimeThreshold forced to match stepTimeResolution from configuration",
            0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("******************************************************************************************",
            0,0,0,0,0,0);
         }
      }

      if (status == ZL303XX_OK)
      {
         if (p->stepTimeAdjustFreqThreshold < p->stepTimeResolution)
         {
            p->stepTimeAdjustFreqThreshold = p->stepTimeResolution;

            ZL303XX_TRACE_ALWAYS("******************************************************************************************",
            0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("Warning: stepTimeAdjustFreqThreshold forced to match stepTimeResolution from configuration",
            0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("******************************************************************************************",
            0,0,0,0,0,0);
         }
      }
   }

#elif defined ZLS30361_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
   /* Configure stepTime resolution to keep frame pulses aligned to output clocks */
   {
      /* Ex stay aligned to synth1/post-div C and synth3/post-div B */
      /*
      Uint32T postDivBitmap = ZLS3036X_SYNTH1_POST_DIV_C |
                              ZLS3036X_SYNTH3_POST_DIV_B;

      if (status == ZL303XX_OK)
      {
         Uint32T stepTimeResolution = pClock->device.PFConfig.stepTimeResolution;

         status = zl303xx_Dpll36xGetClockPeriodLcm(pClock->cguId,
                     postDivBitmap,
                     &pClock->device.PFConfig.stepTimeResolution);

         if (status == ZL303XX_OK && stepTimeResolution != pClock->device.PFConfig.stepTimeResolution)
         {
             ZL303XX_TRACE_ALWAYS("exampleAprClockCreate: Changed stepTimeResolution from %u to %u (synth LCM)",
                                stepTimeResolution,
                                pClock->device.PFConfig.stepTimeResolution, 0,0,0,0);
         }
      }
      */

      /* If the stepTimeResolution returned by zl303xx_Dpll36xGetClockPeriodLcm()
         is larger than stepTimeAdjustTimeThreshold or stepTimeAdjustFreqThreshold,
         then we need to increase stepTimeAdjustTimeThreshold and
         stepTimeAdjustFreqThreshold since it does not make sense for these
         thresholds to be smaller than the resolution of the device.

         The simplest value to assign is stepTimeResolution. */
      /*
      {
         zl303xx_PFConfigS *p = &(pClock->device.PFConfig);

         if (status == ZL303XX_OK)
         {
            if (p->stepTimeAdjustTimeThreshold < p->stepTimeResolution)
            {
               p->stepTimeAdjustTimeThreshold = p->stepTimeResolution;

               ZL303XX_TRACE_ALWAYS("******************************************************************************************",
               0,0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("Warning: stepTimeAdjustTimeThreshold forced to match stepTimeResolution from configuration",
               0,0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("******************************************************************************************",
               0,0,0,0,0,0);
            }
         }

         if (status == ZL303XX_OK)
         {
            if (p->stepTimeAdjustFreqThreshold < p->stepTimeResolution)
            {
               p->stepTimeAdjustFreqThreshold = p->stepTimeResolution;

               ZL303XX_TRACE_ALWAYS("******************************************************************************************",
               0,0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("Warning: stepTimeAdjustFreqThreshold forced to match stepTimeResolution from configuration",
               0,0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("******************************************************************************************",
               0,0,0,0,0,0);
            }
         }
      }
      */
   }

#endif

   /* Register a hardware device with APR. */
   if ((status == ZL303XX_OK) &&
       (status = zl303xx_AprAddDevice(pClock->cguId,
                                  &pClock->device)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprAddDevice() failed with status = %lu",
                           status, 0,0,0,0,0);
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("exampleAprClockCreate: APR Device=%p added", pClock->cguId, 0,0,0,0,0);
   }

   return status;
}

/* exampleAprClockRemove */
/**
   An example of how to remove an APR clock/device

*******************************************************************************/
zlStatusE exampleAprClockRemove(exampleAprClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_AprRemoveDeviceS removeDev;

   status = ZL303XX_CHECK_POINTERS(pClock, pClock->cguId);
   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprRemoveDeviceStructInit(pClock->cguId, &removeDev);

      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("exampleAprClockRemove: Call to zl303xx_AprRemoveDeviceStructInit() failure=%d", status, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprRemoveDevice(pClock->cguId, &removeDev);

      if(status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("exampleAprClockRemove: Call to zl303xx_AprRemoveDevice() failure=%d", status, 0,0,0,0,0);
      }
      else
      {
          ZL303XX_TRACE_ALWAYS("exampleAprClockRemove: Success for zl303xx_AprRemoveDevice() device=%p", pClock->cguId, 0,0,0,0,0);
      }
   }


   return status;
}



/* exampleAprStreamCreateStructInit */
/**
   An example of how to initialize a APR stream/server configuration structure.

*******************************************************************************/
zlStatusE exampleAprStreamCreateStructInit(exampleAprStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_OK;

   /* Initialize the stream config structure with default values. */
   if (status == ZL303XX_OK)
   {
      memset(pStream, 0, sizeof(*pStream));
      status = zl303xx_AprAddServerStructInit(&pStream->server);
   }

   /* Overwrite defaults with values set through example globals. */
   if (status == ZL303XX_OK )
   {
      pStream->server.algTypeMode = zl303xx_AprGetAlgTypeMode();
      pStream->server.bUseType2BPLL = zl303xx_AprGetUseType2BPLL();
      pStream->server.EnableXOCompensation = zl303xx_AprGetEnableXOCompensation();
      pStream->server.Type2BPLLFastLock =zl303xx_AprGetUseType2BPLLFastLock();
      pStream->server.Type2bFastLockMinPhaseNs = zl303xx_AprGetType2bFastLockMinPhase();
      pStream->server.oscillatorFilterType = zl303xx_AprGetOscillatorFilterType();
      pStream->server.osciHoldoverStability = zl303xx_AprGetHoldoverStability();
      pStream->server.sModeTimeout = zl303xx_AprGetSModeTimeout();
      pStream->server.sModeAgeout = zl303xx_AprGetSModeAgeOut();
      pStream->server.bXdslHpFlag = zl303xx_AprGetXdslHpFlag();
      pStream->server.filterType = zl303xx_AprGetFilterType();
      pStream->server.fwdPacketRateType = zl303xx_AprGetPktRate(ZL303XX_TRUE);
      pStream->server.revPacketRateType = zl303xx_AprGetPktRate(ZL303XX_FALSE);
      pStream->server.tsFormat = zl303xx_AprGetTsFormat();
      pStream->server.b32BitTs = zl303xx_AprGet32BitTsFlag();
      pStream->server.bUseRevPath = zl303xx_AprGetUseReversePath();
      pStream->server.bHybridMode = zl303xx_AprGetHybridServerFlag();
      pStream->server.packetDiscardDurationInSec = zl303xx_AprGetPacketDiscardDurationInSecFlag();
      pStream->server.pullInRange = zl303xx_AprGetPullInRange();
      pStream->server.enterHoldoverGST = zl303xx_AprGetEnterHoldoverGST();
      pStream->server.exitVFlagGST = zl303xx_AprGetExitValidGST();
      pStream->server.exitLFlagGST = zl303xx_AprGetExitLockGST();
      pStream->server.lockFlagsMask = zl303xx_AprGetLockMasks();
      pStream->server.thresholdForFlagV = zl303xx_AprGetCustomerThresholdForFlagV();
      pStream->server.exitPAFlagGST = zl303xx_AprGetExitPhaseAlignGST();
      pStream->server.fastLockBW = zl303xx_AprGetFastLockBW();
      pStream->server.fastLockTime =  zl303xx_AprGetFastLockTotalTimeInSecs();
      pStream->server.fastLockWindow = zl303xx_AprGetFastLockPktSelWindowSize();
      pStream->server.L4ThresholdValue = zl303xx_AprGetL4Threshold();
      pStream->server.useOFM = zl303xx_AprGetUseOFM();
      pStream->server.useNCOAssist = zl303xx_AprGetUseNCOAssist();

      pStream->server.HoldoverFreeze = zl303xx_AprGetHoldoverFreezeValue();
      pStream->server.DFSeed = zl303xx_AprGetDFSeedValue();
      pStream->server.Type2BFastlockStartupIt = zl303xx_AprGetType2BFastlockStartupIt();
      pStream->server.Type2BFastlockThreshold = zl303xx_AprGetType2BFastlockThreshold();
      pStream->server.L2phase_varLimit = zl303xx_AprGetL2phase_varLimitValue();
      pStream->server.OutlierTimer = zl303xx_AprGetOutlierTimerValue();
      pStream->server.ClkInvalidCntr = zl303xx_AprGetClkInvalidCntr();
      pStream->server.NCOWritePeriod = zl303xx_AprGetAprTaskBasePeriodMs();

   }

   if (status == ZL303XX_OK)
   {
      if ((status = AddCustomAprServerParams(&pStream->server)) != ZL303XX_OK)
      {
           ZL303XX_TRACE_ALWAYS("exampleAprStreamCreateStructInit: AddCustomAprServerParams() failed with status = %lu",
                                status, 0,0,0,0,0);
      }

    }

   return status;
}


/* exampleAprStreamCreate */
/**
   An example of how to start a APR stream/server

*******************************************************************************/
zlStatusE exampleAprStreamCreate(exampleAprStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_CHECK_POINTERS(pStream, pStream->cguId);

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined ZLE30360_FIFOLESS_INCLUDED
   if(status == ZL303XX_OK && bUseFifolessMode == ZL303XX_TRUE)
   {
       /* Drop timestamps during stepTime because of bad estimated T3 */
       pStream->server.packetDiscardDurationInSec = 6;
   }
#endif

   /* Apply user xParam overrides */
   if (status == ZL303XX_OK)
   {
       status = exampleAprOverrideXParamApply(&pStream->server);
   }

   if (status == ZL303XX_OK &&
      (status = zl303xx_AprAddServer(pStream->cguId, &pStream->server)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_AprAddServer(%p, %p) failed with status = %lu",
                           pStream->cguId, &pStream->server, status, 0,0,0);
      {
          zl303xx_AprRemoveServerS removeServer;

         (void)zl303xx_AprRemoveServerStructInit(&removeServer);
         removeServer.serverId = pStream->server.serverId;
         (void)zl303xx_AprRemoveServer(pStream->cguId, &removeServer);
         ZL303XX_TRACE_ALWAYS("******** examplePtpEventStreamCreate: zl303xx_AprAddServer() failed with status = %lu; Removed the remains",
                              status, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("APR STREAM created. handle=%lu on %p", pStream->server.serverId, pStream->cguId, 0,0,0,0);
   }

    /* APR-1Hz settings */
    if (status == ZL303XX_OK)
    {
        status = exampleConfigure1HzWithGlobals(pStream->cguId, pStream->server.serverId);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAprStreamCreate, error config 1Hz, status %d",status,0,0,0,0,0);
        }
    }


   return status;
}

/* exampleAprStreamRemove */
/**
   An example of how to remove an APR stream/server

*******************************************************************************/
zlStatusE exampleAprStreamRemove(exampleAprStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_AprRemoveServerS aprServerInfo;

   if (status == ZL303XX_OK)
   {
       if (!pStream || !pStream->cguId)
       {
           status = ZL303XX_INVALID_POINTER;
           ZL303XX_TRACE_ALWAYS("exampleAprStreamRemove: Invalid pStream=%p or pStream->cguId=%p pointer, failure=%d",
                              pStream, (pStream?pStream->cguId:0), status, 0,0,0);
       }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprRemoveServerStructInit(&aprServerInfo);

      if(status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("exampleAprStreamRemove: Call to zl303xx_AprRemoveServerStructInit() failure=%d", status, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      aprServerInfo.serverId = pStream->server.serverId;
      status = zl303xx_AprRemoveServer(pStream->cguId, &aprServerInfo);

      if(status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("exampleAprStreamRemove: Call to zl303xx_AprRemoveServer() of serverId=%d failure=%d",  pStream->server.serverId, status, 0,0,0,0);
      }
      else
      {
          /* Housekeeping */
          zlExampleApp.stream[aprServerInfo.serverId].started = ZL303XX_FALSE;
      }
   }

   return status;
}


#if defined ZLS30361_INCLUDED
/**

  Function Name:
  zl303xx_ExampleHandle36XHoldover

  Details:
   Sets the hardware to holdover when entering the transient or returns the
   mode to the original mode when exiting the transient.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The new transient type
   [in]    oldTransient    The old transient type - only QUICK types modify
                              the hardware

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/

static zlStatusE zl303xx_ExampleHandle36XHoldover
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient,
         zl303xx_BCHybridTransientType oldTransient
         )
{
   zlStatusE status = ZL303XX_OK;
   static ZLS3036X_DpllModeE oldDpllMode = ZLS3036X_DPLL_MODE_AUTO;


   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      if( transient == ZL303XX_BHTT_QUICK )
      {
         /* Set the hardware into holdover. */
         ZLS3036X_DpllModeE tempMode;

         if( zl303xx_Dpll36xModeGet(hwParams, &tempMode) == ZL303XX_OK )
         {
            (void)zl303xx_Dpll36xMitigationEnabledSet(hwParams, ZL303XX_TRUE);

            if( zl303xx_Dpll36xModeSet(hwParams, ZLS3036X_DPLL_MODE_HOLDOVER) == ZL303XX_OK )
            {
               /* Save the mode so we can return to it later. */
               oldDpllMode = tempMode;
            }
         }
      }
      else if( transient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         if( oldTransient == ZL303XX_BHTT_QUICK )
         {
            /* Put the hardware back to the original mode */
            status = zl303xx_Dpll36xModeSet(hwParams, oldDpllMode);

            (void)zl303xx_Dpll36xMitigationEnabledSet(hwParams, ZL303XX_FALSE);
         }
      }
   }

   return( status );
}
#endif

#if defined ZLS30721_INCLUDED
/**

  Function Name:
  zl303xx_ExampleHandle72XHoldover

  Details:
   Sets the hardware to holdover when entering the transient or returns the
   mode to the original mode when exiting the transient.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The new transient type
   [in]    oldTransient    The old transient type - only QUICK types modify
                              the hardware

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/

static zlStatusE zl303xx_ExampleHandle72XHoldover
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient,
         zl303xx_BCHybridTransientType oldTransient
         )
{
   zlStatusE status = ZL303XX_OK;
   static ZLS3072X_DpllModeE oldDpllMode = ZLS3072X_DPLL_MODE_TRACKING;
   static Uint32T RefEnableValue = 0;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      if( transient == ZL303XX_BHTT_QUICK )
      {
         /* Set the hardware into holdover. */
         ZLS3072X_DpllModeE tempMode;

         if( zl303xx_Dpll72xModeGet(hwParams, &tempMode) == ZL303XX_OK )
         {
            /* Store and then disable all input references */
            status |= zl303xx_Dpll72xRefEnableGet(hwParams,&RefEnableValue);
            status |= zl303xx_Dpll72xRefEnableSet(hwParams,0x0);

            if( zl303xx_Dpll72xModeSet(hwParams, ZLS3072X_DPLL_MODE_HOLDOVER) == ZL303XX_OK )
            {
               /* Save the mode so we can return to it later. */
               oldDpllMode = tempMode;
            }
         }
      }
      else if( transient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         if( oldTransient == ZL303XX_BHTT_QUICK )
         {
            /* Put the hardware back to the original mode */
            status |= zl303xx_Dpll72xModeSet(hwParams, oldDpllMode);

            /* Restore all input references */
            status |= zl303xx_Dpll72xRefEnableSet(hwParams,RefEnableValue);
         }
      }
   }

   return( status );
}
#endif


#if defined ZLS30701_INCLUDED
/**

  Function Name:
  zl303xx_ExampleHandle70XHoldover

  Details:
   Sets the hardware to holdover when entering the transient or returns the
   mode to the original mode when exiting the transient.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The new transient type
   [in]    oldTransient    The old transient type - only QUICK types modify
                              the hardware

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/

static zlStatusE zl303xx_ExampleHandle70XHoldover
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient,
         zl303xx_BCHybridTransientType oldTransient
         )
{
   zlStatusE status = ZL303XX_OK;
   static ZLS3070X_DpllHWModeE oldDpllMode = ZLS3070X_DPLL_MODE_AUTO_LOCK;


   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      if( transient == ZL303XX_BHTT_QUICK )
      {
         /* Set the hardware into holdover. */
         ZLS3070X_DpllHWModeE tempMode;

         if( zl303xx_Dpll70xModeGet(hwParams, &tempMode) == ZL303XX_OK )
         {
            (void)zl303xx_Dpll70xMitigationEnabledSet(hwParams, ZL303XX_TRUE);

            if( zl303xx_Dpll70xModeSet(hwParams, ZLS3070X_DPLL_MODE_HOLDOVER) == ZL303XX_OK )
            {
               /* Save the mode so we can return to it later. */
               oldDpllMode = tempMode;
            }
         }
      }
      else if( transient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         if( oldTransient == ZL303XX_BHTT_QUICK )
         {
            /* Put the hardware back to the original mode */
            status = zl303xx_Dpll70xModeSet(hwParams, oldDpllMode);

            (void)zl303xx_Dpll70xMitigationEnabledSet(hwParams, ZL303XX_FALSE);
         }
      }
   }

   return( status );
}
#endif

#if defined ZLS30751_INCLUDED
/**

  Function Name:
  zl303xx_ExampleHandle75XHoldover

  Details:
   Sets the hardware to holdover when entering the transient or returns the
   mode to the original mode when exiting the transient.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The new transient type
   [in]    oldTransient    The old transient type - only QUICK types modify
                              the hardware

  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/

static zlStatusE zl303xx_ExampleHandle75XHoldover
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient,
         zl303xx_BCHybridTransientType oldTransient
         )
{
   zlStatusE status = ZL303XX_OK;
   static ZLS3075X_DpllHWModeE oldDpllMode = ZLS3075X_DPLL_MODE_AUTO_LOCK;


   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      if( transient == ZL303XX_BHTT_QUICK )
      {
         /* Set the hardware into holdover. */
         ZLS3075X_DpllHWModeE tempMode;

         if( zl303xx_Dpll75xModeGet(hwParams, &tempMode) == ZL303XX_OK )
         {
            (void)zl303xx_Dpll75xMitigationEnabledSet(hwParams, ZL303XX_TRUE);

            if( zl303xx_Dpll75xModeSet(hwParams, ZLS3075X_DPLL_MODE_HOLDOVER) == ZL303XX_OK )
            {
               /* Save the mode so we can return to it later. */
               oldDpllMode = tempMode;
            }
         }
      }
      else if( transient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         if( oldTransient == ZL303XX_BHTT_QUICK )
         {
            /* Put the hardware back to the original mode */
            status = zl303xx_Dpll75xModeSet(hwParams, oldDpllMode);

            (void)zl303xx_Dpll75xMitigationEnabledSet(hwParams, ZL303XX_FALSE);
         }
      }
   }

   return( status );
}
#endif

#if defined ZLS30771_INCLUDED
/**

  Function Name:
  zl303xx_ExampleHandle77XHoldover

  Details:
   Sets the hardware to holdover when entering the transient or returns the
   mode to the original mode when exiting the transient.
   
   For 77X device, we set the NCO-Assist DPLL to Holdover Mode.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The new transient type
                              
  Return Value:
   Sint32T

  Notes:
   None

*******************************************************************************/

static zlStatusE zl303xx_ExampleHandle77XHoldover
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient,
         zl303xx_BCHybridTransientType oldTransient
         )
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *paramsNCOAssist = NULL;
   
   /* Storage previous DPLL Mode */
   static ZLS3077X_DpllHWModeE oldDpllMode = ZLS3077X_DPLL_MODE_REFLOCK;
   /* Unused */
   oldTransient = oldTransient;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Retrieve the ParamS structure of the NCO Assist DPLL Device */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,&paramsNCOAssist);
   }

   /* Perform the main action for transient mitigation */
   if (status == ZL303XX_OK)
   {
      if( transient == ZL303XX_BHTT_QUICK )
      {
         /* Set the hardware into holdover. */
         ZLS3077X_DpllHWModeE tempMode;

         if( zl303xx_Dpll77xModeGet(paramsNCOAssist, &tempMode) == ZL303XX_OK )
         {
            (void)zl303xx_Dpll77xMitigationEnabledSet(hwParams, ZL303XX_TRUE);

            if( zl303xx_Dpll77xModeSet(paramsNCOAssist, ZLS3077X_DPLL_MODE_HOLDOVER) == ZL303XX_OK )
            {
               /* Save the mode so we can return to it later. */
               oldDpllMode = tempMode;
            }
         }
      }
      else if( transient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         /* Put the hardware back to the original mode */
         status = zl303xx_Dpll77xModeSet(paramsNCOAssist, oldDpllMode);

         (void)zl303xx_Dpll77xMitigationEnabledSet(hwParams, ZL303XX_FALSE);
      }
   }

   return( status );
}
#endif

/**

  Function Name:
  exampleAprHandleHybridTransient

  Details:
   Handles reception of transient indication from management layer.

   The type of transient passed to this routine determines the actions of this
   routine:

   ZL303XX_BHTT_QUICK

      The transient is a loss of the SYNCE connection - max ppb offset is
      100ppm for some time.

      The CGU should be placed into holdover.

      This routine calls zl303xx_AprSetHybridTransient() so that 1Hz can
      react to the transient

   ZL303XX_BHTT_OPTIONAL

      The transient is a classical transient - max ppb offset is 7500ns/s
      for 0.016s followed by 50ns/s for 15s.

      This routine calls zl303xx_AprSetHybridTransient() so that 1Hz can
      react to the transient

      The CGU is left unchanged.

   ZL303XX_BHTT_NOT_ACTIVE

      When the transient condition ends, then the user must call this routine
      with this value.

      If the CGU had been placed in holdover, then it should be returned to
      the state before holdover.

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure
   [in]    transient       The transient type

  Return Value:
   zlStatusE

  Notes:
   None

*******************************************************************************/

zlStatusE exampleAprHandleHybridTransient
         (
         void *hwParams,
         zl303xx_BCHybridTransientType transient
         )
{
   zlStatusE status = ZL303XX_OK;
   static zl303xx_BCHybridTransientType oldTransient = ZL303XX_BHTT_NOT_ACTIVE;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      if( transient >= ZL303XX_BHTT_LAST )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Informs APR-Phase and PSL&FCL modules of the current transient status. */
      status = zl303xx_AprSetHybridTransient(hwParams, transient);
   }


   if (status == ZL303XX_OK)
   {
      if( oldTransient == ZL303XX_BHTT_NOT_ACTIVE )
      {
         if( transient == ZL303XX_BHTT_NOT_ACTIVE )
         {
            /* Nothing to do */
            ZL303XX_TRACE_ALWAYS("exampleAprHandleHybridTransient: input=current - no action taken", 0, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else if( transient == ZL303XX_BHTT_QUICK )
         {
            /* Set hardware to holdover */
            #if defined ZLS30341_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
            {
               /* ZLS3034X Device does not support hybrid transient.
                  We recommend mode switching to pure-packet mode */
            }
            #endif
            #if defined ZLS30361_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle36XHoldover(hwParams, transient, oldTransient);
               if(status == ZL303XX_OK)
               {
                   status =  zl303xx_Dpll36xBCHybridActionOutOfLock(hwParams);
               }
            }
            #endif
            #if defined ZLS30721_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle72XHoldover(hwParams, transient, oldTransient);
               if(status == ZL303XX_OK)
               {
                   status = zl303xx_Dpll72xBCHybridActionOutOfLock(hwParams);
               }
            }
            #endif
            #if defined ZLS30701_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle70XHoldover(hwParams, transient, oldTransient);
               if(status == ZL303XX_OK)
               {
                   status = zl303xx_Dpll70xBCHybridActionOutOfLock(hwParams);
               }
            }
            #endif
            #if defined ZLS30751_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle75XHoldover(hwParams, transient, oldTransient);
               if(status == ZL303XX_OK)
               {
                   status = zl303xx_Dpll75xBCHybridActionOutOfLock(hwParams);
               }
            }
            #endif
            #if defined ZLS30771_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle77XHoldover(hwParams, transient, oldTransient);
               if(status == ZL303XX_OK)
               {
                   status = zl303xx_Dpll77xBCHybridActionOutOfLock(hwParams);
               }
            }
            #endif
         }
      }
      if(( oldTransient == ZL303XX_BHTT_OPTIONAL ) || ( oldTransient == ZL303XX_BHTT_QUICK ))
      {
         if((status == ZL303XX_OK) && (transient == ZL303XX_BHTT_NOT_ACTIVE) )
         {
            /* Remove hardware holdover */
            #if defined ZLS30341_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
            {
               /* ZLS3034X Device does not support hybrid transient.
                  We recommend mode switching to pure-packet mode */
            }
            #endif
            #if defined ZLS30361_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle36XHoldover(hwParams, transient, oldTransient);
            }
            #endif
            #if defined ZLS30721_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle72XHoldover(hwParams, transient, oldTransient);
            }
            #endif
            #if defined ZLS30701_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle70XHoldover(hwParams, transient, oldTransient);
            }
            #endif
            #if defined ZLS30751_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle75XHoldover(hwParams, transient, oldTransient);
            }
            #endif
            #if defined ZLS30771_INCLUDED
            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
               status = zl303xx_ExampleHandle77XHoldover(hwParams, transient, oldTransient);
            }
            #endif
         }
         else
         {
            /* Can not go directly from one type of transient to another */
            ZL303XX_TRACE_ALWAYS("exampleAprHandleHybridTransient: cannot move from 1 transient type to another", 0, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   #if defined ZLS30721_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         /* On entering/exiting electrical mode, reset feedback divider register */
         status = zl303xx_Dpll72xSetAFBDIV(hwParams,0);
      }
   }
   #endif

   if (status == ZL303XX_OK)
   {
      oldTransient = transient;
   }


   return status;
}



#if defined ZLS3038X_INCLUDED
/**
  Function Name:
   exampleAprMainStop

  Details:
   An example for stopping APR

  Parameters:
   
   None

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprMainStop(void);
zlStatusE exampleAprMainStop(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = exampleShutdown();
   }

   return status;

}

/**
  Function Name:
   exampleAprMain

  Details:
   An example for configuring the APR

  Parameters:
   
   None

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprMain(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Setup global configuration options */
   /* All this should be done in the customer code */
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
       status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
       status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

       status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }

   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* setup with 1 clock (CGU), 0 ports, 1 stream (server) */
      status = exampleAppStructInit(1, 0, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      status = exampleAppStart(&zlExampleApp);
   }

   if (status != ZL303XX_OK)
   {
       printf("exampleAprMain: Some startup code did not work. status=%d\n", status);
   }

   return status;
} /* END exampleAprMain */


/**
  Function Name:
   exampleAprHybrid

  Details:
   An example for configuring the APR in hybrid mode (hardware frequency recovery
   + 1Hz alignment from packet stream).

  Parameters:
   
   None

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprHybrid(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
       status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
       status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

       status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }

   /* Set the default device reference mode to hybrid */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
   }

   if (status == ZL303XX_OK)
   {
      /* Set the default hybrid configuration for the server to be registered */
      status = zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);
   }

   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* setup with 1 clock (CGU), 0 ports, 1 stream (server) */
      status = exampleAppStructInit(1, 0, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      status = exampleAppStart(&zlExampleApp);
   }

   /* Enable 1Hz for the device */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprSetDevice1HzEnabled(zlExampleApp.clock[TARGET_DPLL].apr.cguId, ZL303XX_TRUE);  /* Use the first clock params */
   }

   return status;
}
#endif


/* Custom Device Message Router */
/* zl303xx_DpllCustomMsgRouterLog */
/**

   Logging function for the message router.

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data
   [in]   statusToLog   Status data

*******************************************************************************/
static Sint32T zl303xx_DpllCustomMsgRouterLog(void *hwParams, void *inData, void *outData, zlStatusE statusToLog)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_DriverMsgInDataS *in;
    zl303xx_DriverMsgOutDataS *out;


    (void) out; /* warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_DriverMsgInDataS *)inData;
        out = (zl303xx_DriverMsgOutDataS *)outData;
    }

    if (status == ZL303XX_OK)
    {
        switch( in->dpllMsgType )
        {
            case ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO:
            case ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P:
            case ZL303XX_DPLL_DRIVER_MSG_SET_FREQ:
            case ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL:
            case ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL:
            case ZL303XX_DPLL_DRIVER_MSG_SET_TIME:
            case ZL303XX_DPLL_DRIVER_MSG_STEP_TIME:
            case ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU:
            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT:
            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET:
            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET:
            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE:
            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY:
            case ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC:
            case ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL:
            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE:
            case ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO:
            case ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV:
            case ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE:
            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK:
            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK:
            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL:
            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1:
            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2:
            case ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS:
            case ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP:
            case ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER:
            default:
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "DpllCustomMsgRouter: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, in->dpllMsgType, 0, 0, 0);
                break;
        }
    }

    return status;
}

/* zl303xx_DpllCustomMsgRouter */
/**

   Mesage router for custom device driver.

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data

*******************************************************************************/
Sint32T zl303xx_DpllCustomMsgRouter(void *hwParams, void *inData, void *outData)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_DriverMsgInDataS *in;
    zl303xx_DriverMsgOutDataS *out;


    (void) out; /* warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_DriverMsgInDataS *)inData;
        out = (zl303xx_DriverMsgOutDataS *)outData;
    }

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_UNSUPPORTED_MSG_ROUTER_OPERATION;

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "DpllCustomMsgRouter Call: hwP=%p  msg=%d",
            hwParams, in->dpllMsgType, 0, 0, 0, 0);

        switch( in->dpllMsgType )
        {
            case ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO:
                out->d.getDeviceInfo.devType = CUSTOM_DEVICETYPE;
                out->d.getDeviceInfo.devId = 0;
                out->d.getDeviceInfo.devRev = 0;
                status = ZL303XX_OK;
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P:
                status = exampleAprDcoGetFreq(hwParams,
                                        &(out->d.getFreq.freqOffsetInPpt)); /* Only I Part */
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_FREQ:
                status = exampleAprDcoSetFreq(hwParams,
                                        in->d.setFreq.freqOffsetInPpt);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL:
                status = exampleAprDefaultSwitchToPacketRef(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL:
                status = exampleAprDefaultSwitchToElectricalRef(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_TIME:
                status = ZL303XX_OK; /* This is the CGU part of the setTime operation */
                break;


            case ZL303XX_DPLL_DRIVER_MSG_STEP_TIME:
                status = ZL303XX_OK; /* This is the CGU part of the stepTime operation */

                break;

            case ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU:
                status = exampleJumpActiveCGU(hwParams,
                                        in->d.jumpActiveCGU.seconds,
                                        in->d.jumpActiveCGU.nanoSeconds,
                                        in->d.jumpActiveCGU.bBackwardAdjust);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS:
                status = exampleAprGetHwManualFreerunStatus(hwParams,
                                        &(out->d.getHWManualFreerun.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS:
                 status = exampleAprGetHwManualHoldoverStatus(hwParams,
                                        &(out->d.getHWManualHoldover.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS:
                status = exampleAprDefaultgetHwSyncInputEnStatus(hwParams,
                                        &(out->d.getHWSyncInputEn.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS:
                status = exampleAprDefaultgetHwOutOfRangeStatus(hwParams,
                                        &(out->d.getHWOutOfRange.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS:
                status = exampleAprGetHwLockStatus(hwParams,
                                        &(out->d.getHWLockStatus.lockStatus));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME:
                status = exampleAdjustTime(hwParams,
                                        in->d.adjustTime.adjustment,
                                        in->d.adjustTime.recomendedTime);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL:
                status = ZL303XX_OK; /* This is called for zl303xx_AprSetActiveElecRef, don't return failure */
                break;

            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE:
                status = exampleAdjustTimeDR(hwParams,
                                        in->d.adjustTimeDCOReadable.adjustment,
                                        &(out->d.adjustTimeDCOReadable.dcoReadable));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT:
            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET:
            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET:
            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE:
            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY:
            case ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC:
            case ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL:
            case ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE:
            case ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO:
            case ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV:
            case ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT:
            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK:
            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK:
            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1:
            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2:
            case ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS:
            case ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS:
            case ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP:
            case ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER:
            default:
                break;
        }

        zl303xx_DpllCustomMsgRouterLog(hwParams, inData, outData, status);
    }

    return status;
}


/* zl303xx_UserMsgRouterLog */
/**

   Logs for the mesage router

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data
   [in]   statusToLog   Status data

*******************************************************************************/
static Sint32T zl303xx_UserMsgRouterLog(void *hwParams, void *inData, void *outData, zlStatusE statusToLog)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_UserMsgInDataS *in;
    zl303xx_UserMsgOutDataS *out;


    (void) out; /* warning removal */
    (void) statusToLog; /* warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_UserMsgInDataS *)inData;
        out = (zl303xx_UserMsgOutDataS *)outData;
    }
    if (status == ZL303XX_OK)
    {
        switch( in->userMsgType )
        {
            case ZL303XX_USER_MSG_SET_TIME_TSU:
                /*ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_SET_TIME_TSU, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_STEP_TIME_TSU:
                /*ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_STEP_TIME_TSU, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_JUMP_TIME_TSU:
                /*ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_JUMP_TIME_TSU, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_JUMP_TIME_NOTIFICATION:
                /*ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_JUMP_TIME_NOTIFICATION, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_JUMP_STANDBY_CGU:
                /*ZL303XX_TRACE_ALWAYS("ApplicationQcomMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_JUMP_STANDBY_CGU, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_SEND_REDUNDANCY_DATA_TO_MONITOR:
                /*_ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_SEND_REDUNDANCY_DATA_TO_MONITOR, 0, 0, 0);*/
                break;

            case ZL303XX_USER_MSG_APR_PHASE_ADJ_MODIFIER:
                /*ZL303XX_TRACE_ALWAYS("UserMsgRouter: s=%d  hwP=%p  msg=%d",
                       statusToLog, hwParams, ZL303XX_USER_MSG_APR_PHASE_ADJ_MODIFIER, 0, 0, 0);*/
                break;

            default:
                break;
        }
    }

    return status;
}


/* zl303xx_UserMsgRouter */
/**

   Mesage router for example code.

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data

*******************************************************************************/
Sint32T zl303xx_UserMsgRouter(void *hwParams, void *inData, void *outData)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_UserMsgInDataS *in;
    zl303xx_UserMsgOutDataS *out;


    (void) out; /* warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_UserMsgInDataS *)inData;
        out = (zl303xx_UserMsgOutDataS *)outData;
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_UNSUPPORTED_MSG_ROUTER_OPERATION;

        switch( in->userMsgType )
        {
            case ZL303XX_USER_MSG_SET_TIME_TSU:

                status = exampleAprSetTimeTsu(hwParams,
                     in->d.setTimeTSU.deltaTimeSec,
                     in->d.setTimeTSU.deltaTimeNanoSec,
                     in->d.setTimeTSU.negative);
                break;

            case ZL303XX_USER_MSG_STEP_TIME_TSU:

                status = exampleAprStepTimeTsu(hwParams,
                     in->d.stepTimeTSU.deltaTimeNanoSec);
                break;

            case ZL303XX_USER_MSG_JUMP_TIME_TSU:

                status = exampleAprJumpTimeTsu(hwParams,
                     in->d.jumpTimeTSU.deltaTimeSec,
                     in->d.jumpTimeTSU.deltaTimeNanoSec,
                     in->d.jumpTimeTSU.negative);
                break;

            case ZL303XX_USER_MSG_JUMP_TIME_NOTIFICATION:

                status = exampleJumpNotification(hwParams,
                     in->d.jumpTimeNotification.seconds,
                     in->d.jumpTimeNotification.nanoSeconds,
                     in->d.jumpTimeNotification.bBackwardAdjust,
                     in->d.jumpTimeNotification.jumpEvent);

                break;

            case ZL303XX_USER_MSG_JUMP_STANDBY_CGU:

                status = exampleJumpStandbyCGU(hwParams,
                       in->d.jumpStandbyCGU.seconds,
                       in->d.jumpStandbyCGU.nanoSeconds,
                       in->d.jumpStandbyCGU.bBackwardAdjust);
                break;

            case ZL303XX_USER_MSG_SEND_REDUNDANCY_DATA_TO_MONITOR:

                status = exampleSendRedundancyData(hwParams,
                       in->d.sendRedundencyDataToMonitor.serverId,
                       &(out->d.sendRedundencyDataToMonitor.msg));
                break;

            case ZL303XX_USER_MSG_APR_PHASE_ADJ_MODIFIER:

                status = exampleAdjModifier(hwParams,
                       &(out->d.aprPhaseAdjustmentModifier.adjModifierData));
                break;

            default:
                break;
        }

        zl303xx_UserMsgRouterLog(hwParams, inData, outData, status);
    }

    return status;
}

/*****************   END   ****************************************************/















