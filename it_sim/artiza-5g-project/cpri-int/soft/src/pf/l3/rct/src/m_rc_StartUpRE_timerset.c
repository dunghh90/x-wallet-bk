/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_StartUpRE_timerset.c
 *  @brief  RE起動待ちタイマ設定
 *  @date   2015/08/27 TDIPS)maruyama create(立ち上げ対象RE特定(m_rc_getStartUpRE.c)から分割）
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "f_rrh_def.h"



/********************************************************************************************************************/
/**
 *  @brief	RE起動待ちタイマ設定
 *  @return	None
 *  @date	2015/08/07	TDIPS) maruyama create
 *  @date	2015/08/24	TDIPS) maruyama ログ出力内容変更
 *  @date   2015/08/27 TDIPS)maruyama create(立ち上げ対象RE特定(m_rc_getStartUpRE.c)から分割）
 */
/********************************************************************************************************************/
VOID m_rc_StartUpRE_timerset()
{
	UINT	ret_Result		= 0;						/* 関数戻り値	*/
	INT		stup_errcd		= 0;						/* リターンコード	*/
	char	assert_work[64]	= {0};						/* アサートログ		*/
	struct timespec rstBaseTim;                                 /* 時刻 */
	
	/* 「RE起動待ちタイマ設定」開始 */
		/* cm_TStat(timerid, time,  notify_type, eventno, queueid, errcd);	*/
		/* timerId		:開始するタイマID									*/
		/*		cmn\dcm\inc\m_cm_def.h て定義								*/
		/*			#define	CMD_TIMID_STARTUPRE		E_RRD_TIMID_STARTUPRE	*/
		/*				E_RRD_TIMID_STARTUPREはsys\inc\f_rrh_def.hで定義	*/
		/* time			:タイマ値 単位：									*/
		/*		cmn\dcm\inc\m_cm_def.hで定義								*/
		/*			#define CMD_TIMVAL_STARTUP_RE							*/
		/* notify_type	:タイマモード （シングルモード）					*/
		/* eventno		：イベント番号										*/
		/* queueid		:送信先スレッド										*/
		/* errcd		:エラーコード										*/
	/*	現在から360秒後のカウンター値を保持する	*/
	(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &rstBaseTim);	/* 時刻取得(bootup時刻) */
	baseTim360 = rstBaseTim.tv_sec + CMD_TIMVAL_LIMIT_RESTARTUPE;
	/*	Step3待ちタイマ(340sec)を生成する	*/
	ret_Result = cm_TStat(CMD_TIMID_STARTUPRE, CMD_TIMVAL_STARTUP_RE, CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &stup_errcd);
	
	if( ret_Result != CMD_RES_OK )
	{
		memset(assert_work, 0, sizeof(assert_work));
		snprintf(assert_work, sizeof(assert_work), "RE StartUpTimer Start NG ret_Result=%d,errcd=%d", ret_Result, stup_errcd);
		cm_Assert(CMD_ASL_DBGLOW, ret_Result, assert_work);
		return;
	}
}
/********************************************************************************************************************/
/**
 *  @brief  Step7完了待ちタイマ設定
 *  @return None
 *  @date   2015/11/05 TDI)satou その他未実装-008 create
 */
/********************************************************************************************************************/
VOID m_rc_Step7_timerset(VOID)
{
    UINT ret;
    INT errcd;
    char assert_work[128] = {0};
    bpf_ru_hrtm_notify_type_t notify_type;
    UINT timid;
    UINT timval;
    UINT eventno;
    UINT queueid;
	struct timespec rstBaseTim;                                 /* 時刻 */

    timid       = CMD_TIMID_WAIT_COMPLETE_STEP7;
//	timval      = CMD_TIMVAL_WAIT_COMPLETE_STEP7;
    notify_type = CMD_TIMMD_SINGLE;
    eventno     = CMD_TSKIF_TIMOUTNTC;
    queueid     = CMD_TSKID_RCT;
	
	/*	現在の時刻カウンタを取得し、RE起動開始から360秒後のタイマ値を算出する	*/
	(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &rstBaseTim);	/* 時刻取得(bootup時刻) */
	/*	現在カウンターと360秒後のカウンタの差が20秒未満の場合	*/
	if(baseTim360 < (rstBaseTim.tv_sec + 20))
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Wait Complete Step7 timer value Error1:Timer=%d, 360Timer=%d", rstBaseTim.tv_sec, baseTim360);
		/*	20秒とする	*/
		timval = CMD_TIMVAL_WAIT_COMPLETE_STEP7;
	}
	/*	現在カウンターと360秒後のカウンタの差が90秒超過の場合	*/
	else if(baseTim360 > (rstBaseTim.tv_sec + 90))
	{
		/*	最大待ち時間を90秒とする	*/
		timval = 90000;
	}
	/*	Step3～Step7の待ち時間が20～90秒の場合	*/
	else
	{
		/*	待ち時間を算出する	*/
		timval = (baseTim360 - rstBaseTim.tv_sec)*1000;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Wait Complete Step7 timer start:value=%d(%d-%d)", timval, baseTim360, rstBaseTim.tv_sec);
    ret = cm_TStat(timid, timval, notify_type, eventno, queueid, &errcd);
    if (BPF_RU_HRTM_COMPLETE != ret)
    {
        snprintf(assert_work, sizeof(assert_work), "Wait Complete Step7 timer start NG. ret=%d errcd=%d", ret, errcd);
        cm_Assert(CMD_ASL_DBGLOW, ret, assert_work);
    }
}
/* @} */
