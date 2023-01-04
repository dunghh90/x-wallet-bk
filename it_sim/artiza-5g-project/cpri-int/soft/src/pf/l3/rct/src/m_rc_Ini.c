/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Ini.c
 *  @brief  rct task init process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief     rct task init process
 *  @note     rct task init process	 
 *  @param    -
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2013/11/15 FFCS)Xut modify for ZYNQ.
 *  @date     2015/07/28 TDI)satou RSVへのPORT状態subscribe要求送信処理を削除
 *  @date     2015/08/27 TDI)satou 「Initialize PASW state」削除
 */
/********************************************************************************************************************/

VOID m_rc_Ini(VOID)
{
	INT			wtime;						/*  応答メッセージ待ち時間(msec単位)*/
	UINT   		rtn  = E_API_RCD_OK;			/* 戻り値					*/
	CMT_TSKIF_HEAD		*head_pt;				/**<  タスク共通ヘッダ		 */
	INT			buf_ret;
	USHORT		lwCpri;
	INT			ret;
	INT			errcd;

	/* Local variable initialization */
	wtime = CMD_NUM0;
	
	/******************************/
	/* 各種テーブルの初期化を実施 */
	/******************************/
	/* カード制御状態管理テーブル初期化(初期値：0)	*/
	cm_MemClr(&cmw_cardcnt_tbl, sizeof(CMT_CARDCNTTBL));

	/* 制御種別は初期値Fを入れる */
	/* ローカル／リモート制御されると、RE監視で値設定する */
	cmw_cardcnt_tbl.cnt_kind = 0xFFFF;

	/* ログカウンタテーブルを初期化する	*/
	cm_MemClr( &rcw_logcnt, sizeof( rcw_logcnt ));
	
	/* ログ取得テーブルを初期化する */
	cm_MemClr( rcw_log_tbl, sizeof( rcw_log_tbl ));

	/*	SREの場合	*/
	if(cmw_device_kind == CMD_DEVKND_RRE_15G_SRE)
	{
		/*	最大値	*/
		rcw_device_kind = CMD_DEVKND_SRE_15G;
	}
	/*	SRE以外の場合	*/
	else
	{
		rcw_device_kind = cmw_re_dev_tbl.dev_kind;
	}

	m_cm_L3LogTblInit();

	/* テーブルTRX設定(一時保存用)(3G) */
	cm_MemClr( &gt_rcw_trxset_save_3g_tmp[0], sizeof( CMT_CPRIF_TRXSETREQ ) * CMD_MAX_CAR_3G );
	/* テーブルTRX設定(一時保存用)(LTE) */
	cm_MemClr( &gt_rcw_trxset_save_lte_tmp[0], sizeof( CMT_CPRIF_TRXSETREQ_S3G ) * CMD_MAX_CAR_LTE );

	/* 配下REのLayer3状態の初期化 */
	for( lwCpri=D_RRH_CPRINO_RE_MIN ; lwCpri <= D_RRH_CPRINO_RE_MAX; lwCpri++ )
	{
		f_cmn_com_layer3_set_each( lwCpri, CMD_SYS_3G, E_RRH_LAYER3_RE_IDLE );
		f_cmn_com_layer3_set_each( lwCpri, CMD_SYS_LTE, E_RRH_LAYER3_RE_IDLE );
	}
	/* 共有メモリのアドレス取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_REPORTSTA_S3G, (VOID**)&gw_rcw_shmRePortSta_logp, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", E_RRH_SHMID_APL_REPORTSTA_S3G, errcd);
        return ;
    }

	/* 戻り値判螳?*/
	if( cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_HEAD ), (VOID**)&head_pt ) != CMD_RES_OK )
	{
			/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_rc_Ini", 
							   "cm_BReq NG", 
							   CMD_NG, sizeof( CMT_TSKIF_HEAD ), CMD_NUM0 );
	}
	
	head_pt->uiEventNo = CMD_TSKIF_INIRSP;
	head_pt->uiSrcTQueueID = CMD_TSKID_RCT;
	head_pt->uiDstTQueueID = CMD_TSKID_INI;
	head_pt->uiLength = sizeof(CMT_TSKIF_HEAD);

	/******************/
	/* 初期化応答送信 */
	/******************/
	buf_ret = cm_Enter(	CMD_TSKID_INI,
				CMD_QRB_NORMAL,
				CMD_TSKIF_INIRSP,
				head_pt);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}
	/* タスク状態はMainで運用中に設定する */

	/* Call API function to subscribe card state change */
	rtn = rrhApi_Svp_Mpr_SubscribeCardState(D_RRH_PROCQUE_L3, wtime, D_RRH_NULL);

	if(rtn != E_API_RCD_OK)
	{
		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					"rrhApi_Svp_Mpr_SubscribeCardState NG");
	}

	return ;
}


/* @} */


