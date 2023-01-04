/*!
 * @skip   $Id$
 * @file   m_lg_Wait_CpriLinkDwnNtc.c
 * @brief  the processing after receiving RE trouble log get request(the state is using).
 * @date   2016/02/05 TDI)satou
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/********************************************************************************************************************/
/**
 *  @brief  CPRIリンク断通知
 *  @note 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return Void
 *  @date   2016/02/05 TDI)satou create. 障害ログ取得中にCPRIリンク断->復旧すると、RECからログが再度すぐに取得できない問題の対処
 */
/********************************************************************************************************************/
VOID m_lg_Wait_CpriLinkDwnNtc(UCHAR *a_rcv_adr, USHORT a_rec_typ)
{
	CMT_TSKIF_CPRIDISCONN_NTC *a_parm_p;
	USHORT cpr_idx;
	USHORT cpr_idx_loop;
	INT errcd = 0;
	INT ret;

	a_parm_p = (CMT_TSKIF_CPRIDISCONN_NTC*)a_rcv_adr;

	/* RECの場合、処理なし */
	if (D_RRH_CPRINO_REC == a_parm_p->link_num)
	{
		return;
	}

	/* 3Gの場合、処理なし */
	if (CMD_SYS_3G == a_parm_p->system_type)
	{
		return;
	}

	cpr_idx = a_parm_p->link_num - 1;
	if (LGD_GETRESTS_FILSENDING == gw_lgw_REGetLogSta_tbl[a_rec_typ][cpr_idx])
	{	/*
		 * 対象REの状態が「配下REからのファイルデータ送信中」の状態の場合、
		 * (1) RE障害ログ取得状態を「未処理」に更新
		 * (2) 該当配下REに対して保存中障害ログファイルを削除
		 * (3) 他に「取得要求中」、「配下REからのファイルデータ送信中」のREがいなければ
		 *     - RE障害ログ送信確認タイマ停止
		 *     - FHMのログ作成を開始
		 *     - RE障害ログ取得段階状態に「FHMログファイル作成中状態」を設定
		 */
		cm_Assert(D_RRH_LOG_AST_LV_WARNING, a_parm_p->link_num, "link down in acquisition of the log");

		gw_lgw_REGetLogSta_tbl[a_rec_typ][cpr_idx] = LGD_GETRESTS_NOTPROC;

		if (gw_lgw_FilSndNum[a_rec_typ] > 0)
		{
			gw_lgw_FilSndNum[a_rec_typ]--;
		}

		m_lg_RELogFilDelete(a_rec_typ, a_parm_p->link_num);

		for (cpr_idx_loop = 0; cpr_idx_loop < D_RRH_CPRINO_RE_MAX; cpr_idx_loop++)
		{
			if ((LGD_GETRESTS_REQUESTING == gw_lgw_REGetLogSta_tbl[a_rec_typ][cpr_idx_loop])
			||  (LGD_GETRESTS_FILSENDING == gw_lgw_REGetLogSta_tbl[a_rec_typ][cpr_idx_loop]))
			{
				cm_Assert(D_RRH_LOG_AST_LV_INFO, cpr_idx_loop+1, "continue the acquisition of the log");
				return;
			}
		}

		ret = cm_TStop(E_RRH_TIMID_LOG_SEND_S3G, &errcd);
		if (BPF_RU_HRTM_COMPLETE != ret)
		{
			cm_Assert(D_RRH_LOG_AST_LV_WARNING, errcd, "cm_TStop NG");
		}
		gw_lgw_RETroublelogSendTimerStartFlg = D_RRH_OFF;

		m_lg_FhmLogFilMake(a_rec_typ);
		lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING;
	}
	else
	{	/*
		 * 対象REの状態が「取得要求中」の場合の中断処理は、ここで処理を行わず、
		 * 障害ログ取得応答のタイムアウト処理に任せる。
		 *
		 * RECとのCPRIリンクが切れた場合、RECにファイルデータ送信を送信する際に
		 * L3確立しているかチェックしており、そちらで送信が停止する。
		 */
	}
}
/* @} */

