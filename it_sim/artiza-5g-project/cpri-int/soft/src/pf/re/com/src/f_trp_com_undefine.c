/*
 * f_trp_rec_undefine.c
 *
 *  Created on: 2015/07/22
 *      Author: tdi-guest
 */
#include "f_trp_com.h"

/********************************************************************************************************************/
/**
 *  @brief  Memory Compare.
 *  @note   API function.
 *  @param  data1        [in]  data1 address
 *  @param  data2        [in]  data2 address
 *  @param  n            [in]  compare size
 *  @param  result       [in]  compare result
 *  @return result code
 *  @retval BPF_RU_MACC_OK        normal end
 *  @retval BPF_RU_MACC_NG        abnormal end
 *  @retval BPF_RU_MACC_EPARAM    parameter error
 *
 *    sizeが0ならば比較せずに復帰(BPF_RU_MACC_OK)
 *    data1,data2はNULLチェックを行う．
 *    サイズの最大値は特にチェックしない.
 *    領域より大きい値をいれると動作は保証しない
 *    data1のデータがdata2のデータより大きい場合は正の整数を，一致する場合は
 *    0を，小さい場合は負の整数を返す
 */
/********************************************************************************************************************/
int BPF_RU_MACC_BCMP(const void *data1, const void *data2, unsigned int n, int *result)
{
    if( ( n != 0 ) &&
        ( data1==NULL || data2==NULL )){
        return BPF_RU_MACC_EPARAM;
    }
    /********************************************
    * Momory compare processing.                *
    ********************************************/
    *result = memcmp(data1, data2, n);
    return BPF_RU_MACC_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  Memory Copy.
 *  @note   API function.
 *  @param  dst          [in]  copy destination address
 *  @param  src          [in]  copy source address
 *  @param  n            [in]  copy size
 *  @return result code
 *  @retval BPF_RU_MACC_OK        normal end
 *  @retval BPF_RU_MACC_NG        abnormal end
 *  @retval BPF_RU_MACC_EPARAM    parameter error
 */
/********************************************************************************************************************/
int BPF_RU_MACC_BCOPY(void *dst, const void *src, unsigned int n)
{
    void *dmy_dst;
    //int errcd;

    /*
     * Check the parameter *
     */
    if( src==NULL ){
        return BPF_RU_MACC_EPARAM;
    }
    /********************************************
    * Momory copy processing.                   *
    ********************************************/
    dmy_dst = memmove(dst, src, n);
    //errcd = errno;
    if( dst == dmy_dst ){
        return BPF_RU_MACC_OK;
    } else {
        return BPF_RU_MACC_NG;
  }
}

/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   API function.
 *  @param  tv          [out]
 *  @return result code
 *  @retval CMD_OK        normal end
 *  @retval CMD_NG        abnormal end
 */
/********************************************************************************************************************/
INT BPF_RU_UTTM_GETTIME( struct bpf_timeval * tv )
{
	struct timespec a_sec;

	/* 現在時刻情報の取得 */
    (void)clock_gettime( CLOCK_MONOTONIC_RAW   , &a_sec);	/* 時刻取得(bootup時刻) */
	tv->tv_sec	=	a_sec.tv_sec;
	tv->tv_usec	=	(suseconds_t)a_sec.tv_nsec/1000;
	return CMD_OK;
}

INT BPF_RU_UTTM_GETLOCALTIME( time_t* t, struct bpf_tm* tm, INT* ms )
{
	T_RRH_SYSTEM_TIME sysTime;
	
	BPF_HM_DEVC_GETTIME(&sysTime);
	
	tm->tm_sec =  (int)sysTime.sec;
	tm->tm_min =  (int)sysTime.min;
	tm->tm_hour = (int)sysTime.hour;
	tm->tm_mday = (int)sysTime.day;
	tm->tm_mon =  (int)sysTime.month;
	tm->tm_year = (int)sysTime.year-1900; /* BPFで1900西暦変換されるため */
	tm->tm_wday = 0;
	tm->tm_yday = 0;
	tm->tm_isdst=0;
	
	*ms = (INT)sysTime.msec;
	
    return BPF_RU_UTTM_OK;
}

