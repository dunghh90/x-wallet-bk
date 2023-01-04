/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_SETTIME.c
 *  @brief
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  閏年チェック関数
 *  @note   閏年なら真、閏年以外なら偽を返却
 *  @param  uiYear    [in]   年
 *  @return result code
 *  @retval 1:閏年、0:閏年以外

 */
/********************************************************************************************************************/
int bpf_hm_devc_IsLeapYear(unsigned int uiYear)
{
	/* 西暦年が4で割り切れ,かつ100で割り切れない年は閏年 */
	/* または400で割り切れる年は閏年 */
	if ((uiYear % 4 == 0) && ((uiYear % 100 != 0) || (uiYear % 400 == 0)))
	{
		return 1;
	}
	else{
		return 0;
	}
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  指定月の最終日取得関数
 *  @note   指定月の最終日を返却
 *  @param  tDateTime    [in]   時刻構造体
 *  @return result code
 *  @retval 指定月の最終日付

 */
/********************************************************************************************************************/
int bpf_hm_devc_days_in_month(T_RRH_SYSTEM_TIME *tDateTime)
{
	const int nDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	/* 閏年チェック */
	if((2 == tDateTime->month) && bpf_hm_devc_IsLeapYear(tDateTime->year))
	{
		return 29;
	}

	return nDaysInMonth[tDateTime->month-1];
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  有効年月日時分秒判定関数
 *  @note   設定時刻が有効かを判定
 *  @param  tDateTime    [in]   時刻構造体
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG

 */
/********************************************************************************************************************/
int bpf_hm_devc_judg_date(T_RRH_SYSTEM_TIME *tDateTime)
{
	int					iDay;

	/* 年判定 */
	/* 2000年 - 2099年以外は無効 */
	if((tDateTime->year < 2000) || (2099 < tDateTime->year))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input Year Error %d",tDateTime->year);
		return BPF_HM_DEVC_NG;
	}

	/* 月判定 */
	/* 1月 - 12月以外は無効 */
	if((tDateTime->month < 1) || (12 < tDateTime->month))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input month Error %d",tDateTime->month);
		return BPF_HM_DEVC_NG;
	}

	/* 日判定 */
	/* 1,3,5,7,8,10,12月の場合 : 1日 - 31日以外は無効 */
	/* 4,6,9,11月の場合        : 1日 - 30日以外は無効 */
	/* 2月(閏年)の場合         : 1日 - 28(29)日以外は無効 */
	iDay = bpf_hm_devc_days_in_month(tDateTime);
	if((tDateTime->day < 1) || (iDay < tDateTime->day))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input day Error %d",tDateTime->day);
		return BPF_HM_DEVC_NG;
	}

	/* 時判定 */
	/* 0時 - 23時以外は無効 */
	if( 23 < tDateTime->hour)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input hour Error %d",tDateTime->hour);
		return BPF_HM_DEVC_NG;
	}

	/* 分判定 */
	/* 0分 - 59分以外は無効 */
	if( 59 < tDateTime->min )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input min Error %d",tDateTime->min);
		return BPF_HM_DEVC_NG;
	}

	/* 秒判定 */
	/* 0秒 - 59秒以外は無効 */
	if( 59 < tDateTime->sec)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input sec Error %d",tDateTime->sec);
		return BPF_HM_DEVC_NG;
	}

	/* ミリ秒判定 */
	/* 0ミリ秒 - 990ミリ秒(単位：10ms)以外は無効 */
	if( 99 < tDateTime->msec)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date) Input msec Error %d",tDateTime->msec);
		return BPF_HM_DEVC_NG;
	}
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  有効年月日時分秒判定関数(BCD用)
 *  @note   設定時刻が有効かを判定
 *  @param  tDateTime    [in]   時刻構造体
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG

 */
/********************************************************************************************************************/
int bpf_hm_devc_judg_date_bcd(T_RRH_SYSTEM_TIME *tDateTime)
{
	int					iDay;
	T_RRH_SYSTEM_TIME	tDateTime_temp;
	
	/* 年判定 */
	/* 2000年 - 2099年以外は無効 */
	if((tDateTime->year < 0x2000) || (0x2099 < tDateTime->year))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input year Error(0x%x)",tDateTime->year);
		return BPF_HM_DEVC_NG;
	}

	/* 月判定 */
	/* 1月 - 12月以外は無効 */
	if((tDateTime->month < 0x1) || (0x12 < tDateTime->month))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input month Error(0x%x)",tDateTime->month);
		return BPF_HM_DEVC_NG;
	}

	/* 日判定 */
	/* 1,3,5,7,8,10,12月の場合 : 1日 - 31日以外は無効 */
	/* 4,6,9,11月の場合        : 1日 - 30日以外は無効 */
	/* 2月(閏年)の場合         : 1日 - 28(29)日以外は無効 */
	tDateTime_temp.year =	(((tDateTime->year >> 12) & 0x000F) * 1000) +
							(((tDateTime->year >>  8) & 0x000F) * 100)  +
							(((tDateTime->year >>  4) & 0x000F) * 10)   +
							  (tDateTime->year & 0x000F);
	tDateTime_temp.month =	(((tDateTime->month >>  4) & 0x000F) * 10)  +
							  (tDateTime->month & 0x000F);
	
	iDay = bpf_hm_devc_days_in_month( &tDateTime_temp );
	iDay = ((iDay/10)*16 + (iDay%10));						/* BCDに変換 */
	if((tDateTime->day < 1) || (iDay < tDateTime->day))
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input day Error(0x%x)",tDateTime->day);
		return BPF_HM_DEVC_NG;
	}

	/* 時判定 */
	/* 0時 - 23時以外は無効 */
	if( 0x23 < tDateTime->hour)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input hour Error(0x%x)",tDateTime->hour);
		return BPF_HM_DEVC_NG;
	}

	/* 分判定 */
	/* 0分 - 59分以外は無効 */
	if( 0x59 < tDateTime->min )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input min Error(0x%x)",tDateTime->min);
		return BPF_HM_DEVC_NG;
	}

	/* 秒判定 */
	/* 0秒 - 59秒以外は無効 */
	if( 0x59 < tDateTime->sec)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input sec Error(0x%x)",tDateTime->sec);
		return BPF_HM_DEVC_NG;
	}

	/* ミリ秒判定 */
	/* 0ミリ秒 - 990ミリ秒(単位：10ms)以外は無効 */
	if( 0x99 < tDateTime->msec)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. (bpf_hm_devc_judg_date_bcd) Input msec Error(0x%x)",tDateTime->msec);
		return BPF_HM_DEVC_NG;
	}
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  時刻設定(OS)
	*  @note   OSの現在時刻を設定する。
 *  @param  sysTime  日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/07/17 ALPHA)Samejima ST-26関連(排他処理関数使用に変更)
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
static void BPF_HM_DEVC_SETTIME_OS(T_RRH_SYSTEM_TIME* sysTime )
{
	struct timeval tv;
	struct tm  ltm;
	int			ret;

	/* 本関数をコールする前に必ずセマフォを獲得しておくこと!!!!! */
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"OS Time set Pre(Y:%d M:%d D:%d H:%d Mi:%d S:%d Ms:%d0)", sysTime->year,sysTime->month,sysTime->day,sysTime->hour,sysTime->min,sysTime->sec,sysTime->msec);
	memset(&ltm,0,sizeof(ltm));
	/* 年の設定 */
	ltm.tm_year = sysTime->year - BPF_HMDEVC_YEARE_OFFSET;
	/* 月日の設定 */
	ltm.tm_mon  = sysTime->month - BPF_HMDEVC_MONTH_OFFSET;
	ltm.tm_mday = sysTime->day;
	/* 時分の設定 */
	ltm.tm_hour = sysTime->hour;
	ltm.tm_min  = sysTime->min;
	/* 秒の設定 */
	ltm.tm_sec  = sysTime->sec;
	/* mktimeの結果は直接 *timep にいれる．結果がNGであればNG復帰．*/
	/* その場合は*timepの中身はDon't care */
	tv.tv_sec = mktime(&ltm);
	if( tv.tv_sec  == BPF_HM_DEVC_SYS_NG ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. mktime Return Error(%d)",errno );
		return;
	}
	/* 10ms単位のためマイクロ秒に変換 */
	tv.tv_usec = sysTime->msec * 10000;
	/* settimeofdayがNG復帰したらNGを返す */
	ret = settimeofday(&tv,NULL);
	if( ret  == BPF_HM_DEVC_SYS_NG ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. settimeofday Return Error(%d) tv_sec:0x%lx tv_usec:0x%lx",errno, tv.tv_sec , tv.tv_usec );
		return;
	}
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"OS Time set After(0x%lx.0x%lx)", tv.tv_sec,tv.tv_usec);
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  時刻設定
 *  @note   現在時刻を設定する。現在時刻はハードタイマであるRTCに設定する。
 *  @param  sysTime  日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/07/17 ALPHA)Samejima ST-26関連(排他処理関数使用に変更)
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_SETTIME(T_RRH_SYSTEM_TIME sysTime )
{
    /* 変数宣言 */
    unsigned short rtc_hourmin;         /* RTC用 時間/分      */
    unsigned short rtc_secmsec;         /* RTC用 秒/(1/100秒) */
    unsigned short rtc_year;            /* RTC用 年           */
    unsigned short rtc_monthday;        /* RTC用 月/日        */
    int            rtn;                 /* 戻り値             */
    unsigned int   uiRtc[2];            /* RTC1設定エリア     */
    unsigned int   rtc_size;            /* RTC設定サイズ      */
    unsigned int   *rtc_int;            /* RTC設定ポインタ    */
    unsigned int   offset_rtc;          /* RTC1オフセット     */


	/* FPGA accessフラグOFFの場合にRTCレジスタへのアクセスを抑止する		*/
	if( di_devc_data_p->fpga_access == DEVC_FLG_OFF )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. FPGA access Flag OFF " );	
		return BPF_HM_DEVC_NG;
	}
	
	/*	年月日時分秒の有効判定処理	*/
	rtn = bpf_hm_devc_judg_date(&sysTime);
	if(rtn == BPF_HM_DEVC_NG)
	{
		/* bpf_hm_devc_judg_date内でログとるため不要 */
		return BPF_HM_DEVC_NG;
	}

	/* ---------------------- ソフトウェアタイマの設定  ---------------------- */
	int errcd;
	unsigned int intr_count;
	intr_count = 0;
	while(1){
		rtn = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,BPF_RU_IPCM_WAIT,NULL,&errcd);
		if(( BPF_RU_IPCM_OK != rtn ) && (BPF_RU_IPCM_EINTR != rtn )){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. BPF_RU_IPCM_PROCSEM_TAKE Error (%x)" , rtn );
			return BPF_HM_DEVC_NG;
		}
		if(BPF_RU_IPCM_EINTR != rtn){	/* OK */
			break;
		}
		intr_count++;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"EINTR raised (0x%d)" , intr_count );
	}

	if( di_devc_data_p->time_correction_suppress_cnt > 0){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"Currently, Time setting suppress mode %d %d " , di_devc_data_p->time_correction_suppress_cnt,di_devc_data_p->time_force_update_flg );
		di_devc_data_p->time_force_update_flg = DEVC_FLG_ON;
	}else{
		BPF_HM_DEVC_SETTIME_OS(&sysTime);
	}
	BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,&errcd);

    /* ---------------------- RTCタイマの設定  ---------------------- */

    /* 時間/分の設定 */
    rtc_hourmin  = (unsigned short)((sysTime.hour/10)*16 + (sysTime.hour%10));
    rtc_hourmin <<= 8;
    rtc_hourmin |= (unsigned short)((sysTime.min/10)*16  + (sysTime.min%10));

    /* 秒/ミリ秒の設定 */
    rtc_secmsec  = (unsigned short)((sysTime.sec/10)*16 + (sysTime.sec%10));
    rtc_secmsec <<= 8;
    rtc_secmsec |= (unsigned short)((sysTime.msec/10)*16  + (sysTime.msec%10));

    /* 年の設定 */
    rtc_year = (unsigned short)(((sysTime.year)/1000)*4096
                + (((sysTime.year)%1000)/100)*256
                + (((sysTime.year)%100)/10)*16
                + (sysTime.year)%10);

    /* 月/日の設定 */
    rtc_monthday  = (unsigned short)((sysTime.month/10)*16 + (sysTime.month%10));
    rtc_monthday <<= 8;
    rtc_monthday |= (unsigned short)((sysTime.day/10)*16  + (sysTime.day%10));

	/* RTC1への設定 (時間/分/秒/(1/100秒))*/
	uiRtc[0] = (rtc_hourmin << 16) | rtc_secmsec;

	/* RTC2への設定 (年/月/日)*/
	uiRtc[1] = (rtc_year << 16) | rtc_monthday;

	/* RTC1 OFFSET設定 */
	offset_rtc = BPF_HM_DEVC_CONTROL_OFFSET_RTC1;
		
	rtc_int = uiRtc;
	rtc_size = sizeof(uiRtc);

    /* RTC_RTC1_WRITE */
	rtn = BPF_HM_DEVC_REG_WRITE_BUFFER(0, offset_rtc, rtc_size, rtc_int );

    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. RTC Register Buffer Error (%x)" , rtn );
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Setting RTC %08X-%08X",uiRtc[1],uiRtc[0]);

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  時刻設定を抑止する
 *  @date   2019/04/27 Taniguch create
 */
/********************************************************************************************************************/
void BPF_HM_DEVC_SETTIME_OS_SUPPRESS(void){
	int ret;
	int errcd;
	unsigned int intr_count;

	intr_count = 0;
	while(1){
		ret = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,BPF_RU_IPCM_WAIT,NULL,&errcd);
		if(( BPF_RU_IPCM_OK != ret ) && (BPF_RU_IPCM_EINTR != ret )){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. BPF_RU_IPCM_PROCSEM_TAKE (%x)" , ret );
			return;
		}
		if(BPF_RU_IPCM_EINTR != ret){	/* OK */
			break;
		}
		intr_count++;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"EINTR raised (0x%d)" , intr_count );
	}

	di_devc_data_p->time_correction_suppress_cnt++;
	if(di_devc_data_p->time_correction_suppress_cnt > 1){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"Setting Time suppress mode %d " , di_devc_data_p->time_correction_suppress_cnt );
	}
	BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,&errcd);
	return;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  時刻設定を補正する
 *  @date   2019/04/27 Taniguch create
 */
/********************************************************************************************************************/
void BPF_HM_DEVC_SETTIME_OS_CORRECTION(void){

	int ret;
	int errcd;
	unsigned int intr_count;
	intr_count = 0;
	while(1){
		ret = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,BPF_RU_IPCM_WAIT,NULL,&errcd);
		if(( BPF_RU_IPCM_OK != ret ) && (BPF_RU_IPCM_EINTR != ret )){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. BPF_RU_IPCM_PROCSEM_TAKE (%x)" , ret );
			return;
		}
		if(BPF_RU_IPCM_EINTR != ret){	/* OK */
			break;
		}
		intr_count++;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"EINTR raised (0x%d)" , intr_count );
	}
	
	if(di_devc_data_p->time_correction_suppress_cnt > 0){
		di_devc_data_p->time_correction_suppress_cnt--;
	}else{
		/* do nothing */
		BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,&errcd);
		return;
	}

	if(di_devc_data_p->time_correction_suppress_cnt == 0){
		if(di_devc_data_p->time_force_update_flg == DEVC_FLG_ON){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"Setting Time correction %d " , di_devc_data_p->time_correction_suppress_cnt );

			T_RRH_SYSTEM_TIME sysTime ;
			if(( ret = BPF_HM_DEVC_GETTIME(&sysTime )) == BPF_HM_DEVC_COMPLETE ){
				BPF_HM_DEVC_SETTIME_OS(&sysTime);
				di_devc_data_p->time_force_update_flg = DEVC_FLG_OFF;
			}else{
				bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Setting Time correction ERROR(%d) " , ret );
			}
		}
	}

	BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_TIMER_HISTORY,BPF_RU_IPCM_LOCK_RW,&errcd);
	return;
}
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  時刻設定(BCD)
	*  @note   BCD形式の現在時刻を設定する。現在時刻はハードタイマであるRTCに設定する。
 *  @param  sysTime  日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/07/17 ALPHA)Samejima ST-26関連(排他処理関数使用に変更)
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_SETTIME_BCD(T_RRH_SYSTEM_TIME sysTime )
{
    /* 変数宣言 */
    unsigned short rtc_hourmin;          /* RTC用 時間/分      */
    unsigned short rtc_secmsec;          /* RTC用 秒/(1/100秒) */
    unsigned short rtc_year;             /* RTC用 年           */
    unsigned short rtc_monthday;         /* RTC用 月/日        */
    int            rtn;                  /* 戻り値             */
    unsigned int   uiRtc[2];             /* RTC1設定エリア     */
    unsigned int   rtc_size;             /* RTC設定サイズ      */
    unsigned int   *rtc_int;             /* RTC設定ポインタ    */
    unsigned int offset_rtc;             /* RTC1オフセット     */
    struct timeval tv;
    struct tm  ltm;
    int ret;
    struct tm *dmy_tm;
	unsigned short tmp_data;

	/* FPGA accessフラグOFFの場合にRTCレジスタへのアクセスを抑止する		*/
	if( di_devc_data_p->fpga_access == DEVC_FLG_OFF )
	{
		return BPF_HM_DEVC_NG;
	}
	
	/*	年月日時分秒の有効判定処理	*/
	rtn = bpf_hm_devc_judg_date_bcd(&sysTime);
	if(rtn == BPF_HM_DEVC_NG)
	{
		return BPF_HM_DEVC_NG;
	}

	/* 2038年以降はOS時刻は設定しない */
	if(sysTime.year <= BPF_HMDEVC_2037_OFFSET_BCD)
	{
		/* ---------------------- ソフトウェアタイマの設定  ---------------------- */

		/* 現在時刻情報の取得 */
		ret = gettimeofday(&tv,NULL);
		if( ret == BPF_HM_DEVC_SYS_NG ){

		    return BPF_HM_DEVC_NG;
		}

		/* localtime_rがNULLで復帰したらNG終了 */
		dmy_tm = localtime_r(&tv.tv_sec, &ltm);

		if( dmy_tm == NULL ){

		    return BPF_HM_DEVC_NG;
		}

		/* 年の設定 */
		tmp_data = (sysTime.year>>12 & 0x0F) * 1000 + (sysTime.year>>8 & 0x0F) * 100 + 
                   (sysTime.year>>4  & 0x0F) * 10   + (sysTime.year    & 0x0F);
		ltm.tm_year = tmp_data - BPF_HMDEVC_YEARE_OFFSET;

		/* 月日の設定 */
		tmp_data = (sysTime.month>>4 & 0x0F)*10 + (sysTime.month & 0x0F);
		ltm.tm_mon  = tmp_data - BPF_HMDEVC_MONTH_OFFSET;
		tmp_data = (sysTime.day>>4 & 0x0F)*10 + (sysTime.day & 0x0F);
		ltm.tm_mday = tmp_data;

		/* 時分の設定 */
		tmp_data = (sysTime.hour>>4 & 0x0F)*10 + (sysTime.hour & 0x0F);
		ltm.tm_hour = tmp_data;
		tmp_data = (sysTime.min>>4 & 0x0F)*10 + (sysTime.min & 0x0F);
		ltm.tm_min  = tmp_data;

		/* 秒の設定 */
		tmp_data = (sysTime.sec>>4 & 0x0F)*10 + (sysTime.sec & 0x0F);
		ltm.tm_sec = tmp_data;

		/* mktimeの結果は直接 *timep にいれる．結果がNGであればNG復帰. */
		/* その場合は*timepの中身はDon't care */
		tv.tv_sec = mktime(&ltm);
		if( tv.tv_sec  == BPF_HM_DEVC_SYS_NG ){

		    return BPF_HM_DEVC_NG;
		}

		/* 10ms単位のためマイクロ秒に変換 */
		tmp_data = (sysTime.msec>>4 & 0x0F)*10 + (sysTime.msec & 0x0F);
		tv.tv_usec = tmp_data * 10000;

		/* settimeofdayがNG復帰したらNGを返す */
		ret = settimeofday(&tv,NULL);
		if( ret  == BPF_HM_DEVC_SYS_NG ){

		    return BPF_HM_DEVC_NG;
		}
	}
    /* ---------------------- RTCタイマの設定  ---------------------- */

    /* 時間/分の設定 */
    rtc_hourmin  = (unsigned short)(sysTime.hour);
    rtc_hourmin <<= 8;
    rtc_hourmin |= (unsigned short)(sysTime.min);

    /* 秒/ミリ秒の設定 */
    rtc_secmsec  = (unsigned short)(sysTime.sec);
    rtc_secmsec <<= 8;
    rtc_secmsec |= (unsigned short)(sysTime.msec);

    /* 年の設定 */
    rtc_year = (unsigned short)(sysTime.year);

    /* 月/日の設定 */
    rtc_monthday  = (unsigned short)(sysTime.month);
    rtc_monthday <<= 8;
    rtc_monthday |= (unsigned short)(sysTime.day);

	/* 装置種別でアドレスを変更	*/
	switch( di_devc_data_p->device_kind )
	{
	case D_RRH_EEP_DEV_KIND_FHM:

    	/* RTC1への設定 (時間/分/秒/(1/100秒))*/
    	uiRtc[0] = (rtc_hourmin << 16) | rtc_secmsec;

    	/* RTC2への設定 (年/月/日)*/
    	uiRtc[1] = (rtc_year << 16) | rtc_monthday;

    	/* RTC1 OFFSET設定 */
    	offset_rtc = BPF_HM_DEVC_CONTROL_OFFSET_RTC1;
		
		rtc_int = uiRtc;
		rtc_size = sizeof(uiRtc);

		break;
	
	default:
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"NG. this machine(%d) cannot use", di_devc_data_p->device_kind );
		return BPF_HM_DEVC_NG;
	}

    /* RTC_RTC1_WRITE */
	rtn = BPF_HM_DEVC_REG_WRITE_BUFFER(0, offset_rtc, rtc_size, rtc_int );

    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* RRH-001 MD alpha)matsuhashi add end */
