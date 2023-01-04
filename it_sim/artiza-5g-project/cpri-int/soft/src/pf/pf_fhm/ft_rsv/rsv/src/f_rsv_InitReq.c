/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_InitReq.c
 *  @brief  task init function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2011/11/24 COMAS)Uchida  M-S3G-eNBSYS-02539対応
 *	@date   2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04092 CR-00054-000 15G-RREのPA OFF問題対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup TRA_RSV
* @{ */
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief task init function
* @note Init table and send init response.\n
* @param -
* @return None.
* @date 2008/07/16 FFCS)Wangjuancreate
* @date 2013/11/28 FFCS)Xut Modify for Zynq
* @date 2014/12/23 FFCS)Xut Modify for ppcEOL
* @date 2015/07/28 TDI)satou PA状態フラグ制御を削除
* @date 2015/07/28 TDI)satou PORT-IN state tableを削除
* @date 2015/08/27 TDI)satou 「Update TX system number」削除(FHMで送信系統数は参照しない)
* @date 2015/08/27 TDI)satou 「Init TRA card first install flag」削除(設定のみで参照していない)
* @date 2015/08/27 TDI)satou 「ログカウンタテーブル」削除(初期化のみで以降参照していない)
* @date 2015/08/27 TDI)satou 「ログ取得テーブル」削除(初期化のみで以降参照していない)
* @date 2015/09/04 TDI)satou 「Unmask SV state IRQ」をFHM用の設定値に修正
* @date 2015/09/04 TDI)satou 「clear all sv bit」をFHM用に修正
* @date 2015/10/30 TDI)satou IT3問処No.33
* @warning	N/A
* @FeatureID	PF-Svp-001-001-001
* @Bug_No	N/A
* @CR_No	N/A
*/
VOID f_rsv_InitReq( VOID )
{
	UINT		loop;
	INT		errcd;		
	INT		rtn_bpf;
	UINT		regData = 0;
	T_SYS_COMMON_THDIF		msgHead;
	UINT                    linkno;         /* CPRIリンク番号 */
	T_RRH_CONN_DISC         shmConnDisc;    /* 共有メモリ(RE接続有無状態) */
	INT                     ret;            /* 戻り値評価用 */
	
	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	memset((VOID*)&msgHead, 0, sizeof(msgHead));

	/* sv new status table init */
	memset( tra_sv_newsta, 0, sizeof( tra_sv_newsta ));

	memset( tra_sv_oldsta, 0, sizeof( tra_sv_oldsta ));
	memset( f_rsvw_almCount, 0, sizeof( f_rsvw_almCount ));
	
	/* Init TRA card first active flag */
	f_rsvw_first_act_flg = D_SYS_OFF;

	/* Init TRA card restart both alm flag */
	f_rsvw_restart_both_alm_flg = D_SYS_OFF;

	/* Error control table */
	f_rsvw_errctl_tbl = D_SYS_NUM0;

	/* Get share memory address of SV State history  */
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SV_STATE_HISTORY, 
		(VOID **)&f_comw_svStateChg_his, 
		&errcd);

	/*  Control code = CTL */
	f_comw_svStateLog.chgType = D_SYS_SVSTATE_SET;
	/* Initialize SV state log table */
	for(loop = D_SYS_NUM0; loop < D_RRH_SV_INF_NUM; loop++)
	{
		f_comw_svStateLog.svState[loop] = D_SYS_NUM0;
	}

	/* 共有メモリ(RE接続有無状態)を取得 */
	ret = f_cmn_com_conn_disk_get(&shmConnDisc);
	if (D_RRH_OK != ret) {
	    memset(&shmConnDisc, 0, sizeof shmConnDisc);
	    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
	        "f_cmn_com_conn_disk_get() error. ret=0x%08x", ret);
	}

	/* Take semphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SVCTL,
		BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
			
	/* clear all sv bit*/
	regData = D_SYS_NUM0;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV1INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV2INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV3INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV4INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV5INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV6INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV7INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV8INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV9INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV10INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV11INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV12INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV13INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV14INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV15INF, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV16INF, &regData);

	/* Unmask SV state IRQ ※全bit割り込み有効に設定する。バグでない限り未使用のbitは操作しないため  */
	regData = 0;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV1CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV2CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV3CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV4CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV5CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV6CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV7CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV8CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV9CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV10CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV11CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV12CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV13CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV14CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV15CMPM, &regData);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_RRH_REG_CNT_SV16CMPM, &regData);
	
	/* Give semphore of SV control */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SVCTL,
		BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	
	/* Set INS bit ON */
	f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_INS, D_SYS_ON);

	/* Set L1-L3 ERR ON (slave port) */
	f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_L1ERR(D_RRH_CPRINO_REC), D_SYS_ON);
	f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_L2ERR(D_RRH_CPRINO_REC), D_SYS_ON);
	f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_3G_L3ERR(D_RRH_CPRINO_REC), D_SYS_ON);
	f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_S3G_L3ERR(D_RRH_CPRINO_REC), D_SYS_ON);

	/* Set L1-L3 ERR ON (master using port) */
	for (linkno = D_RRH_CPRINO_RE_MIN; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    if (CMD_ON == shmConnDisc.conn_disc[linkno-1]) {
            f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_L1ERR(linkno), D_SYS_ON);
            f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_L2ERR(linkno), D_SYS_ON);
            f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_3G_L3ERR(linkno), D_SYS_ON);
            f_com_SVCtl(D_SYS_THDID_PF_RSV, M_SYS_SVCTL_S3G_L3ERR(linkno), D_SYS_ON);
	    }
	}

	/* Set ERR bit ON */
	f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ERR, D_SYS_ON);

	/****************************/
	/* SV状態変化履歴ログ取得   */
	/****************************/
	f_com_SVLogSet(D_SYS_THDID_PF_RSV);

	/*==============================*/
	/* 初期化タスクへ初期化応答送信 */
	/*==============================*/
	/* send CPRI link startup notice to CPRI thread */
	msgHead.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
	msgHead.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
	msgHead.uiDstTQueueID = D_SYS_THDQID_PF_F_MAIN;
	msgHead.uiSrcPQueueID = D_RRH_PROCQUE_F_PF;
	msgHead.uiSrcTQueueID = D_SYS_THDQID_PF_RSV;
 	msgHead.uiLength = sizeof(T_SYS_COMMON_THDIF);

	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
			(CHAR*)&msgHead,
			sizeof( msgHead ), 
			D_SYS_THDQID_PF_RSV, 
			f_rsvw_thrdstate, 
			D_SYS_MSGID_INIT_COMP_NTC );

	/* Update task state to all  task init complete wait state */
	f_rsvw_thrdstate = D_RSV_TSK_STATE_ALLTSK_STA_WAIT;
	
	return ;
}
/* @} */

/* @} */
