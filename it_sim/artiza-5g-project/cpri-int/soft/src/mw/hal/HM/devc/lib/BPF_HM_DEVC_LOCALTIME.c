/* RRH-001 MD alpha)Nakajima add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_LOCALTIME.c
 *  @brief  
 *  @date   2013/07/17 ALPHA)Nakajima create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  localtime擬似処理
 *  @note   BPF_HM_DEVC_LOCALTIME
 *  @param  pTimeUtc          [in]UINT *
 *  @param  sysTime           [out]tm *
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/07/17 ALPHA)Nakajima create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_LOCALTIME(unsigned int *pTimeUtc, T_RRH_SYSTEM_TIME *sysTime)
{
	unsigned int				year_offset;	/*	年オフセット				*/
	unsigned int				time_utc_cal;	/*	UTCベースタイム(計算用)		*/
	struct	tm					tim_tm_cal_t;	/*	時刻(tm型localtime OUTパラ)	*/

	/*	変数初期化	*/
	year_offset = 0;
	memset(&tim_tm_cal_t, 0, sizeof(tim_tm_cal_t));

	/****************************************************************************/
	/*	2030年未満、2090年未満、2100年未満、2100年以上か判定					*/
	/****************************************************************************/
	if( *pTimeUtc < BPF_HMDEVC_TIM_OFFSET_2030 )
	{
		time_utc_cal = *pTimeUtc;
		year_offset = BPF_HMDEVC_YEAR_1970;
	}
	else if( *pTimeUtc < BPF_HMDEVC_TIM_OFFSET_2090 )
	{
		time_utc_cal = *pTimeUtc - BPF_HMDEVC_TIM_OFFSET_2030;
		year_offset = BPF_HMDEVC_YEAR_2030;
	}
	else if( *pTimeUtc < BPF_HMDEVC_TIM_OFFSET_2100 )
	{
		time_utc_cal = *pTimeUtc - BPF_HMDEVC_TIM_OFFSET_2090;
		year_offset = BPF_HMDEVC_YEAR_2090;
	}
	/*	2100年以上の場合は無効値を設定	*/
	else
	{
		year_offset = 0;
	}

	/****************************************************************************/
	/*	UTCからの1秒インクリメント値から年月日算出(localtime)					*/
	/****************************************************************************/
	/*	2100年以上の場合は処理しない(無効値を設定し処理終了)	*/
	if( year_offset == 0 )
	{
		return BPF_HM_DEVC_NG;
	}
	else
	{
		/*	tm型に変換	*/
		(void)localtime_r((time_t *)&time_utc_cal, &tim_tm_cal_t);

		/*	1970年の場合はそのまま設定	*/
		if( year_offset == BPF_HMDEVC_YEAR_1970 )
		{
			/* オフセット不要 */
		}
		/*	2030年の場合は2030年分のオフセット値を加算	*/
		else if( year_offset == BPF_HMDEVC_YEAR_2030 )
		{
			tim_tm_cal_t.tm_year += BPF_HMDEVC_YEAR_2030 - BPF_HMDEVC_YEAR_1970;
		}
		/*	2090年の場合は2090年分のオフセット値を加算	*/
		else if( year_offset == BPF_HMDEVC_YEAR_2090 )
		{
			tim_tm_cal_t.tm_year += BPF_HMDEVC_YEAR_2090 - BPF_HMDEVC_YEAR_1970;
		}
		/*	その他の場合は無効値を返す	*/
		else
		{
			return BPF_HM_DEVC_NG;
		}
		/* 設定 */
		sysTime->year = (unsigned short)(tim_tm_cal_t.tm_year + BPF_HMDEVC_YEARE_OFFSET);
		sysTime->month = (unsigned char)(tim_tm_cal_t.tm_mon + BPF_HMDEVC_MONTH_OFFSET);
		sysTime->day = (unsigned char)(tim_tm_cal_t.tm_mday);
		sysTime->hour = (unsigned char)(tim_tm_cal_t.tm_hour);
		sysTime->min = (unsigned char)(tim_tm_cal_t.tm_min);
		sysTime->sec = (unsigned char)(tim_tm_cal_t.tm_sec);
	}
	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
