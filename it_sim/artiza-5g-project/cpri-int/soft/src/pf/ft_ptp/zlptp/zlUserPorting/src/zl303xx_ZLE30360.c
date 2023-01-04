/******************************************************************************
*
*  $Id: zl303xx_ZLE30360.c
*
*  Module Description:
*  zl303xx_ZLE30360評価ボード向け処理相当を実施する必要があるため実装
*  関数名はそのまま流用
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "f_du_reg_eth.h"
#include "f_rrh_def.h"
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx.h"
#include "zl303xx_ZLE30360.h"

#include "zl303xx_ExampleAprGlobals.h"
#include "zl303xx_ExamplePtpGlobals.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

//#define __USE_MISC
#include <net/if.h>
//#undef __USE_MISC

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
/* Warning対策 */
int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p);

/*****************   DATA TYPES & STRUCTURES  *********************************/
typedef struct {
    zl303xx_BooleanE valid;
    Uint32T nsample;
    Uint64S sec;
    Uint32T nsec;
    Uint64S cpu_sec;
    Uint32T cpu_nsec;
} zl303xx_VTSS_time_sample_t;



/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* static zl303xx_VTSS_time_sample_t     zl303xx_VTSS_time_sample = {ZL303XX_FALSE, 0, {0,0}, 0, {0,0}, 0};*/ /* 未使用のためコメントアウト */
/* static OS_MUTEX_ID                  zl303xx_VTSS_time_sample_mutex = OS_MUTEX_INVALID; */ /* 未使用のためコメントアウト */
static volatile zl303xx_BooleanE      zl303xx_VTSS_time_set_active = ZL303XX_FALSE;
static volatile Sint32T             zl303xx_VTSS_step_time_ns = 0;
static OS_MUTEX_ID                  zl303xx_VTSS_step_time_mutex = OS_MUTEX_INVALID;


/**
 * Returns approximation of current TSU ToD by interpolating with CPU time
 */
zlStatusE zl303xx_VTSS_get_time(Uint64S *sec, Uint32T *nsec)
{
    zlStatusE status = ZL303XX_OK;
    Uint64S cpuTime = {0,0};
    Uint32T cpuEpoch = 0;
    zl303xx_VTSS_time_sample_t time_sample;

    /* ポインタ引数チェック */
    status = ZL303XX_CHECK_POINTERS(sec, nsec);

	/* 0クリア */
    if (status == ZL303XX_OK)
    {
        sec->hi = 0;
        sec->lo = 0;
        *nsec = 0;
        memset(&time_sample, 0, sizeof(time_sample));
    }

	/* 即時間が取れない場合の処理。今回は不要のためルートを飛ばす */
	#if 0
    if (status == ZL303XX_OK)
    {
		time_sample = zl303xx_VTSS_time_sample;
		
        /* Copy sampled time locally to give up mutex earlier */
        if (OS_MUTEX_TAKE(zl303xx_VTSS_time_sample_mutex) != OS_ERROR)
        {
            time_sample = zl303xx_VTSS_time_sample;

            (void) OS_MUTEX_GIVE(zl303xx_VTSS_time_sample_mutex);

            if (time_sample.valid != ZL303XX_TRUE)
                status = ZL303XX_ERROR;
        }
    }
	#endif	/* if 0 */

    if (status == ZL303XX_OK)
    {
        Uint64S cpuTimeOrig;
        Uint32T temp;
        Uint32T reg = 1;

        /* Calculate the difference in CPU time from when the sample was taken to the current time */
/*      OS_TIME_GET(&cpuTime, &cpuEpoch);	*/

        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE , D_DU_REG_ETH_SYSTIMVALLT, &reg);
        BPF_HM_DEVC_REG_READ (D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_SYSTIMVALSH, &cpuTime.hi); 
        BPF_HM_DEVC_REG_READ (D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_SYSTIMVALSL, &cpuTime.lo); 
        BPF_HM_DEVC_REG_READ (D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_SYSTIMVALN, &temp); 
        
        /* 取ってきた値をバックアップ */
        cpuTimeOrig = cpuTime;

        /* Borrow a second from the cpu and give it to the nanoseconds to make math easier */
        /* 
        if (cpuTime.lo < time_sample.cpu_nsec)
        {
            cpuTime.hi -= 1;
            cpuTime.lo += 1000000000;
        }
        */
        
        /* 1ms待ち合わせた分を考慮 */
        /* nsecDiff += 1000000; */
        
        /* secDiff.hi = cpuEpoch - time_sample.cpu_sec.hi; */
        /* secDiff.lo = cpuTime.hi - time_sample.cpu_sec.lo; */

        /* temp = (cpuTime.lo - time_sample.cpu_nsec);
        secDiff.lo += (temp / 1000000000);
        nsecDiff = temp % 1000000000;
        */

        /* Add the elapsed CPU time difference to the sampled timestampper value */
        /* sec->hi = time_sample.sec.hi + secDiff.hi; */
        /* sec->lo = time_sample.sec.lo + secDiff.lo; */
        /* *nsec = ((time_sample.nsec + nsecDiff) % 1000000000); */
        sec->hi = cpuTime.hi;
        sec->lo = cpuTime.lo;
        *nsec = temp;
        
        ZL303XX_TRACE(ZL303XX_MOD_ID_TOD_MGR, 2,
                    "TSU:\t%u:%u.%09u\tCPU:\t%u:%u.%09u\t(estimate)", sec->hi, sec->lo, *nsec, cpuTimeOrig.hi, cpuTimeOrig.lo, cpuEpoch);
    }
    return status;
}



/**
 * Informs the time engine task that the input 1PPS has moved and the TSU time should be updated on
 * the next edge. The input 1PPS must have already been adjusted prior to calling this function.
 */
zlStatusE zl303xx_VTSS_step_time(Sint32T nanosec)
{
    zlStatusE status = ZL303XX_OK;

    if (zl303xx_VTSS_step_time_mutex && OS_MUTEX_TAKE(zl303xx_VTSS_step_time_mutex) == OS_OK)
    {
        zl303xx_VTSS_step_time_ns += nanosec;
        OS_MUTEX_GIVE(zl303xx_VTSS_step_time_mutex);
    }

    return status;
}


/**
\details
   Blocks the caller until a window in the 1 PPS signal of the given minimum size is available
   (or until the timeout occurs).

\param    min_window_ms    Minimum window size in milliseconds until the next 1PPS edge
\param    timeout_ms       Timeout in milliseconds to wait for the window (minimum 1000 ms)

\retval
   ZL303XX_OK, or ZL303XX_TIMEOUT if the window cannot be acquired

\note
    This function blocks caller
*/
zlStatusE zl303xx_VTSS_wait_for_1pps_window(Uint32T min_window_ms, Uint32T timeout_ms)
{
    zlStatusE status = ZL303XX_OK;
    Uint64S currSec;
    Uint32T currNsec;
    zl303xx_BooleanE bWaiting = ZL303XX_TRUE;
    const Uint32T delay_ms = 50;

    Uint64S cpuStart, cpuEnd;
    Uint32T elapsedMs;

    if (min_window_ms >= 1000 || timeout_ms < 1000)
        return ZL303XX_PARAMETER_INVALID;

    OS_TIME_GET(&cpuStart, NULL);

    /* Wait for large enough window */
    while (bWaiting == ZL303XX_TRUE)
    {
        /* Get the approximate TSU time */
        status = zl303xx_VTSS_get_time(&currSec, &currNsec);

        if (status == ZL303XX_OK && currNsec < (ONE_BILLION - (min_window_ms * ONE_MILLION)))
        {
            /* Large enough window available */
            bWaiting = ZL303XX_FALSE;
            break;
        }

        OS_TIME_GET(&cpuEnd, NULL);

        elapsedMs = (cpuEnd.lo / ONE_MILLION + (cpuEnd.hi - cpuStart.hi) * 1000) - (cpuStart.lo / ONE_MILLION);

        if (elapsedMs >= timeout_ms)
        {
            /* Timeout waiting for window */
            status = ZL303XX_TIMEOUT;
            bWaiting = ZL303XX_FALSE;
            break;
        }

        OS_TASK_DELAY(delay_ms);
    }

    return status;
}

/**
 * Returns approximation of current 1PPS time by interpolating TSU time with CPU time and
 * adjusting by given nanosecond step amount (-1,000,000,000 to 1,000,000,000)
 */
zlStatusE zl303xx_VTSS_get_time_with_step(Uint64S *sec, Uint32T *nsec, Uint64S *baseSec, Uint32T *baseNsec, Sint32T step_ns)
{
	zlStatusE		status = ZL303XX_OK;
	long long		sec64;


	if (status == ZL303XX_OK)
	{
		status = zl303xx_VTSS_get_time(sec, nsec);
	}

	if (status == ZL303XX_OK && step_ns != 0)
	{
		sec64 = (long long)(((long long)(sec->hi) << 32) | ((long long)(sec->lo) & 0xFFFFFFFF));
		baseSec->hi = sec->hi;
		baseSec->lo = sec->lo;
		*baseNsec   = *nsec;

		/* Apply the step time adjustment to estimate where the 1PPS input pulse is located
		 * NOTE: Following code assumes abs(step_ns) < ONE_BILLION
		 */
		if (step_ns >= 0)						/* Positive step */
		{
			*nsec += step_ns;
			sec64 += (*nsec / ONE_BILLION);
			*nsec %= ONE_BILLION;
		}
		else if (*nsec >= (Uint32T) -step_ns)	/* Negative step WITHOUT seconds rollback */
		{
			*nsec += step_ns;
		}
		else									/* Negative step WITH seconds rollback */
		{
			Uint32T tmp = (Uint32T) -step_ns;
			*nsec = (ONE_BILLION - (tmp - *nsec));
			sec64 -= 1;
		}
		sec64++;
		sec->hi = (Uint32T)((sec64 >> 32) & 0xFFFFFFFF);
		sec->lo = (Uint32T)(sec64 & 0xFFFFFFFF);
	}
	return status;
}
