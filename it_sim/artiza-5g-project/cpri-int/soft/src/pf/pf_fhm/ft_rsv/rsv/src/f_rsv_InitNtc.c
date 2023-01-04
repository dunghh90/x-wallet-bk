/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_InitNtc.c
 *  @brief  all task init complete notice handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *  @date   2009/11/10 FFCS)Niemsh modify for M-S3G-eNBPF-02830
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/*********************************************************************************/

/** @addtogroup TRA_RSV
* @{ */

#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/
#include "BPF_RU_HRTM.h"
/* @{ */
/**
* @brief all task init complete notice handle function
* @note TRA card initialization.\n
* @param -
* @return None.
* @date 2008/07/16 FFCS)Wangjuancreate
* @date 2008/07/16 FFCS)Wangjuan CR-00055-000,modify carrier value.
* @date 2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
* 						modify the system parameter E&F initialization processing
* @date 2009/11/10 FFCS)Niemsh modify for M-S3G-eNBPF-02830
*                       delete alarm judegement and alarm case process
* @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
* @date   2013/11/23 FFCS)Xut Modify for Zynq
* @date   2015/08/27 TDI)satou 「Update TX system number」削除(FHMで送信系統数は参照しない)
* @warning	N/A
* @FeatureID	PF-Svp-001-002-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_InitNtc( VOID )
{
	UINT	a_count	= D_SYS_NUM0;
	UINT	regData;

	INT		errCode;
	UINT 		num;

	T_SYS_COMMON_THDIF		msgHead;
	
	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	memset((VOID*)&msgHead, 0, sizeof(msgHead));

	/* 両系ブランチALMの場合を判定する */
	if (f_rsvw_restart_both_alm_flg == D_RRH_ON)
	{
		f_com_assert(D_SYS_NUM0,
					 "TRA CARD RESTART BOTH-ALM" );

		/* if TRA card first time ACT, start up CPRI link */
		if ( f_rsvw_first_act_flg == D_SYS_OFF)
		{	
			f_rsvw_first_act_flg  = D_RRH_ON;
		}
	}
	else
	{
		/*  Control code = SET */
		f_comw_svStateLog.chgType = D_SYS_SVSTATE_SET;
		/* Initialize SV state log table */
		for(a_count = D_SYS_NUM0; a_count < D_RRH_SV_INF_NUM; a_count++)
		{
			f_comw_svStateLog.svState[a_count] = D_SYS_NUM0;
		}
		
		/****************************************************************/
		/* Act setting, software startup normally		*/
		/****************************************************************/
		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ACT,D_RRH_ON);

		/****************************/
		/* SV状態変化履歴ログ取得   */
		/****************************/
		f_com_SVLogSet(D_SYS_THDID_PF_RSV);
	}

	/********************************************************************/
	/* SV状態収集														*/
	/********************************************************************/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV1INF, &regData);
	tra_sv_newsta[0] = (USHORT)regData & D_SYS_ALLF_SHORT;
	tra_sv_newsta[1] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV2INF, &regData);
	tra_sv_newsta[2] = (USHORT)regData  & D_SYS_ALLF_SHORT;
	tra_sv_newsta[3] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV3INF, &regData);
	tra_sv_newsta[4] = (USHORT)regData & D_SYS_ALLF_SHORT;
	tra_sv_newsta[5] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV4INF, &regData);
	tra_sv_newsta[6] = (USHORT)regData  & D_SYS_ALLF_SHORT;
	tra_sv_newsta[7] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV5INF, &regData);
	tra_sv_newsta[8] = (USHORT)regData  & D_SYS_ALLF_SHORT;
	tra_sv_newsta[9] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV6INF, &regData);
	tra_sv_newsta[10] = (USHORT)regData & D_SYS_ALLF_SHORT;
	tra_sv_newsta[11] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV7INF, &regData);
	tra_sv_newsta[12] = (USHORT)regData  & D_SYS_ALLF_SHORT;
	tra_sv_newsta[13] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV8INF, &regData);
	tra_sv_newsta[14] = (USHORT)regData  & D_SYS_ALLF_SHORT;
	tra_sv_newsta[15] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV9INF, &regData);
    tra_sv_newsta[16] = (USHORT)regData  & D_SYS_ALLF_SHORT;
    tra_sv_newsta[17] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;
    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV10INF, &regData);
    tra_sv_newsta[18] = (USHORT)regData  & D_SYS_ALLF_SHORT;
    tra_sv_newsta[19] = (USHORT)(regData >> D_SYS_NUM16) & D_SYS_ALLF_SHORT;

//#ifndef OPT_RRH_SIM /* 仮想環境上では障害監視しない */
	/* Check whether alarm occurs */
	f_rsv_AlmInfChk();

	/* Start Alarm Cycling detect timer */
	BPF_RU_HRTM_REGISTER(
			BPF_RU_HRTM_REG_PERIODIC,
			E_RRH_TIMID_ALARM_POLL,
			100,
			D_SYS_MSGID_TIMOUTNTC,
			D_SYS_THDQID_PF_RSV,
			&errCode
		);
//#endif

	/****************************************************************/
	/* 状態遷移														*/
	/****************************************************************/
	/* 通常状態 */
	f_rsvw_thrdstate = D_RSV_TSK_STATE_USE;	

	/* Get ACT state */
	if(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_REG_CNT_SV1INF_ACT) != 0)
	{
		/* Get register data from LED control register */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
					D_RRH_REG_CNT_SYSLED, &regData);
		/* Set ACT ON */
		regData = (regData & ~D_RRH_CNT_FHM_LEDSYSG1_MASK) | D_RRH_CNT_FHM_LEDSYSG1_ON;
		/* Update LED control register */
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
					D_RRH_REG_CNT_SYSLED, &regData);
	}

	/* Send message to the requested Process */
	for(num = D_SYS_NUM0; num < sizeof(f_rsvw_cardStaChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_rsvw_cardStaChgNtc_QID[ num] != D_SYS_NUM0)
		{
			/* Send API Card state change Notification with current state */
			f_rsv_rptCurCardSta(f_rsvw_cardStaChgNtc_QID[ num]);
		}
	}

	return ;
}
/* @} */

/* @} */
