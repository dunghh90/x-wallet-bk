/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_GETTIME_STR.c
 *  @brief  
 *  @date   2018/08/02 KCN)Nanjo create
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
 *  @brief  時刻文字列取得
 *  @note   BPF_HM_DEVC_GETTIME_STR
 *  @param  timestring  [out]   時刻文字列取得
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GETTIME_STR(char *timestring)
{
    int ret = 0;
    struct timeval tv;
    struct tm ltm;
    struct tm *dmy_tm;

    /* 時刻関連の場合はログの出力はしない */

    /* 現在時刻情報の取得 */
    ret = gettimeofday(&tv,NULL);
    if( ret == BPF_HM_DEVC_SYS_NG ){

        return BPF_HM_DEVC_NG;
    }

    /* localtime_rがNULLで復帰したらNG終了 */
    dmy_tm = localtime_r(&tv.tv_sec, &ltm);
    if( dmy_tm == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "localtime_r == NULL");
        return BPF_HM_DEVC_NG;
    }

    /* 時刻文字列取得 */
    strftime(timestring,BPF_HM_DEVC_TIME_STR_SIZE,"%F",&ltm);

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
