/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_GETTIME.c
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

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  時刻取得
 *  @note   現在時刻を取得する。
 *  @param  sysTime  [out]   日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/07/17 ALPHA)Samejima ST-26関連(排他処理関数使用に変更)
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GETTIME(T_RRH_SYSTEM_TIME* sysTime )
{
    int rtn;
    unsigned int   offset_rtc;       /* RTC1オフセット     */
    unsigned int   uiRtc[2];         /* RTC1設定エリア     */
    unsigned short tempdata;         /* 一時データ         */
    struct timeval tv;
    struct tm ltm;
    struct tm *dmy_tm;
    struct timespec	boottime;
    
    /* 時刻関連の場合はログの出力はしない */

    /* 現在時刻情報の取得 */
    rtn = gettimeofday(&tv,NULL);
    if( rtn == BPF_HM_DEVC_SYS_NG ){

        return BPF_HM_DEVC_NG;
    }
    
    /* localtime_rがNULLで復帰したらNG終了 */
    dmy_tm = localtime_r(&tv.tv_sec, &ltm);

    if( dmy_tm == NULL ){

        return BPF_HM_DEVC_NG;
    }
	
	(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);	/* 時刻取得(bootup時刻) */

	/* FPGA accessフラグOFFの場合にRTCレジスタへのアクセスを抑止する		*/
    if( di_devc_data_p->fpga_access == DEVC_FLG_OFF )
	{
        /* 年の設定 */
        /* 年は1900年を基準としているため、1900を加えて時刻を合わせる */
        sysTime->year = (ltm.tm_year + BPF_HMDEVC_YEARE_OFFSET);
        
        /* 月日の設定 */
        /* 月は0～11なので1を加えて時刻を合わせる */
        sysTime->month = ltm.tm_mon + BPF_HMDEVC_MONTH_OFFSET;
        sysTime->day   = ltm.tm_mday;

        /* 時分の設定 */
        sysTime->hour  = ltm.tm_hour;
        sysTime->min   = ltm.tm_min;

        /* 秒、ミリ秒の設定 */
        sysTime->sec   = ltm.tm_sec;
        sysTime->msec  = (tv.tv_usec / 10000);  /* 10ms単位に変換 */
        sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
    	
        return BPF_HM_DEVC_COMPLETE;

    }
    else /* ハードウェア読み込み */
    {
   		/* RTC1 OFFSET設定 */
       	offset_rtc = BPF_HM_DEVC_CONTROL_OFFSET_RTC1;
       	/* RTC_RTC1_READ */
       	rtn = BPF_HM_DEVC_REG_READ_BUFFER(0, offset_rtc,sizeof(uiRtc),uiRtc);
       	if (rtn != BPF_HM_DEVC_COMPLETE)
       	{
           	/* データ取得失敗 */
           	return BPF_HM_DEVC_NG;
       	}

    	/* RTC表示/設定(マイクロ秒)_レジスタに無いので0を設定する */
        sysTime->ucount= 0;

        /* RTC表示/設定1(時間) */
        tempdata = (uiRtc[0] >> 24) & 0xFF;
        sysTime->hour = (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);

        /* RTC表示/設定1(分) */
        tempdata = (uiRtc[0] >> 16) & 0xFF;
        sysTime->min =  (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);

        /* RTC表示/設定1(秒) */
        tempdata = (uiRtc[0] >> 8) & 0xFF;
        sysTime->sec =  (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);

        /* RTC表示/設定1(1/100秒) */
        tempdata = uiRtc[0] & 0xFF;
        sysTime->msec = (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);
    	
        /* RTC表示/設定2(年) */
        tempdata = (uiRtc[1] >> 16) & 0xFFFF;
        sysTime->year = (tempdata>>12 & 0x0F) * 1000 + (tempdata>>8 & 0x0F) * 100 + 
                        (tempdata>>4  & 0x0F) * 10   + (tempdata    & 0x0F);

        /* RTC表示/設定2(月) */
        tempdata = (uiRtc[1] >> 8) & 0xFF;
        sysTime->month = (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);

        /* RTC表示/設定2(日) */
        tempdata = uiRtc[1] & 0xFF;
        sysTime->day =   (tempdata>>4 & 0x0F)*10 + (tempdata & 0x0F);

    	/* マイクロカウンタはOS時刻から取得	*/
        sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
    	
        return BPF_HM_DEVC_COMPLETE;
    }

}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RTC時刻取得
 *  @note   RTC現在時刻を取得する(BCD)
 *  @param  sysTime  [out]   日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/07/17 ALPHA)Samejima ST-26関連(排他処理関数使用に変更)
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GETTIME_BCD(T_RRH_SYSTEM_TIME* sysTime )
{
    int rtn;
    unsigned int   offset_rtc;       /* RTC1オフセット     */
    unsigned int   uiRtc[2];         /* RTC1設定エリア     */
    unsigned short tempdata;         /* 一時データ         */
    struct timeval tv;
    struct tm ltm;
    struct tm *dmy_tm;
    struct timespec	boottime;

    /* 時刻関連の場合はログの出力はしない */

    /* 現在時刻情報の取得 */
    rtn = gettimeofday(&tv,NULL);
    if( rtn == BPF_HM_DEVC_SYS_NG ){

        return BPF_HM_DEVC_NG;
    }
    
    /* localtime_rがNULLで復帰したらNG終了 */
    dmy_tm = localtime_r(&tv.tv_sec, &ltm);

    if( dmy_tm == NULL ){

        return BPF_HM_DEVC_NG;
    }

	(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);	/* 時刻取得(bootup時刻) */

	/* FPGA accessフラグOFFの場合にRTCレジスタへのアクセスを抑止する		*/
    if( di_devc_data_p->fpga_access == DEVC_FLG_OFF )
	{
		/* ※RTC時刻を使用するのはBCD側のみであり、基本的にこのルートは通らない */
		
        /* 年の設定 */
        /* 年は1900年を基準としているため、1900を加えて時刻を合わせる */
        sysTime->year = (ltm.tm_year + BPF_HMDEVC_YEARE_OFFSET);
        
        /* 月日の設定 */
        /* 月は0～11なので1を加えて時刻を合わせる */
        sysTime->month = ltm.tm_mon + BPF_HMDEVC_MONTH_OFFSET;
        sysTime->day   = ltm.tm_mday;

        /* 時分の設定 */
        sysTime->hour  = ltm.tm_hour;
        sysTime->min   = ltm.tm_min;

        /* 秒、ミリ秒の設定 */
        sysTime->sec   = ltm.tm_sec;
        sysTime->msec  = (tv.tv_usec / 10000);  /* 10ms単位に変換 */
        sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
    	
        return BPF_HM_DEVC_COMPLETE;

    }
    else /* ハードウェア読み込み */
    {

        /* RTC1 OFFSET設定 */
        offset_rtc = BPF_HM_DEVC_CONTROL_OFFSET_RTC1;

        /* RTC_RTC1_WRITE */
        rtn = BPF_HM_DEVC_REG_READ_BUFFER(0, offset_rtc,sizeof(uiRtc),uiRtc);
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
        	/* データ取得失敗 */
        	return BPF_HM_DEVC_NG;
        }

		/* RTC表示/設定(マイクロ秒)_レジスタに無いので0を設定する */
        sysTime->ucount= 0;

        /* RTC表示/設定1(時間) */
        tempdata = (uiRtc[0] >> 24) & 0xFF;
        sysTime->hour = tempdata;

        /* RTC表示/設定1(分) */
        tempdata = (uiRtc[0] >> 16) & 0xFF;
        sysTime->min = tempdata;

        /* RTC表示/設定1(秒) */
        tempdata = (uiRtc[0] >> 8) & 0xFF;
        sysTime->sec = tempdata;

        /* RTC表示/設定1(1/100秒) */
        tempdata = uiRtc[0] & 0xFF;
        sysTime->msec = tempdata;
    	
        /* RTC表示/設定2(年) */
        tempdata = (uiRtc[1] >> 16) & 0xFFFF;
        sysTime->year = tempdata;

        /* RTC表示/設定2(月) */
        tempdata = (uiRtc[1] >> 8) & 0xFF;
        sysTime->month = tempdata;

        /* RTC表示/設定2(日) */
        tempdata = uiRtc[1] & 0xFF;
        sysTime->day =   tempdata;

    	/* マイクロカウンタはOS時刻から取得	*/
        sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
    	
        return BPF_HM_DEVC_COMPLETE;
    }

}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  時刻取得(OS)
	*  @note   現在時刻(OS)を取得する。
 *  @param  sysTime  [out]   日時
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2016/03/14 ALPHA)fujiiy create(base is BPF_HM_DEVC_GETTIME)
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GETTIME_OS(T_RRH_SYSTEM_TIME* sysTime )
{
    int rtn;
    struct timeval tv;
    struct tm ltm;
    struct tm *dmy_tm;
    struct timespec	boottime;

    /* 時刻関連の場合はログの出力はしない */

    /* 現在時刻情報の取得 */
    rtn = gettimeofday(&tv,NULL);
    if( rtn == BPF_HM_DEVC_SYS_NG ){

        return BPF_HM_DEVC_NG;
    }
    
    /* localtime_rがNULLで復帰したらNG終了 */
    dmy_tm = localtime_r(&tv.tv_sec, &ltm);

    if( dmy_tm == NULL ){

        return BPF_HM_DEVC_NG;
    }

	(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);	/* 時刻取得(bootup時刻) */

    /* 年の設定 */
    /* 年は1900年を基準としているため、1900を加えて時刻を合わせる */
    sysTime->year = (ltm.tm_year + BPF_HMDEVC_YEARE_OFFSET);
    
    /* 月日の設定 */
    /* 月は0～11なので1を加えて時刻を合わせる */
    sysTime->month = ltm.tm_mon + BPF_HMDEVC_MONTH_OFFSET;
    sysTime->day   = ltm.tm_mday;

    /* 時分の設定 */
    sysTime->hour  = ltm.tm_hour;
    sysTime->min   = ltm.tm_min;

    /* 秒、ミリ秒の設定 */
    sysTime->sec   = ltm.tm_sec;
    sysTime->msec  = (tv.tv_usec / 10000);  /* 10ms単位に変換 */
    sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
   
    return BPF_HM_DEVC_COMPLETE;

}
/* @} */

/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
