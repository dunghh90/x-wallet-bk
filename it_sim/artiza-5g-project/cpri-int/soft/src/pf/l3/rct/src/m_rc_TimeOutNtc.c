/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_TimeOutNtc.c
 *  @brief  Timeout process.

 *  @date
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  タイムアウト発生通知受信処逅
 *  @note
 *  @param  adwTimerId [in] TimerId
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01988
 *  @date
 *  @date 2015/08/26 TDIPS)maruyama modify RE起動確認待ちタイムアウト時の処理追加(タイマ停止関数は呼ばない）
 *  @date 2015/09/02 TDIPS)maruyama modify RE起動確認待ちタイムアウト時の処理(m_rc_infoAllRE()に変更）、応答がないREは強制停止
 *  @date 2015/09/04 TDIPS)watanabe modify REバージョン報告確認待ちタイムアウト時の処理追加
 *  @date 2015/11/05 TDI)satou その他未実装-008 CMD_TIMID_WAIT_COMPLETE_STEP7を追加
 *  @date 2015/11/13 TDIPS)sasaki M-RRU-ZSYS-01988 update IT2問処No.181対処 Step7完了待ちタイムアウト時の強制停止削除
 *  @date 2017/03/10 FJT)ohashi TS-96水平展開 RE_STARTUP_RSPとStep3/Step7タイムアウトすれ違い対策
 */
/********************************************************************************************************************/
VOID m_rc_TimeOutNtc( UINT adwTimerId )
{
    INT delayPrmChkEn;

	/* タイムチェック */
	switch( adwTimerId )
	{
// 		/*SV-MT/eNB-MT接続強制解放確認タイマ (3G)場合*/
// 		/*SV-MT/eNB-MT接続強制解放確認タイマ (LTE)場合*/
// 		case CMD_TIMID_SVMT_CONN_REL_FORCE_3G:
// 		case CMD_TIMID_SVMT_CONN_REL_FORCE_LTE:
// 			/*SV-MT/eNB-MT接続強制解放確認タイマ タイムアウト処理*/
// 			m_rc_SvMt_ForReleaseConnectTO( adwTimerId );
// 			break;
// 		/* FLD-MTデータコマンド転送受信応答確認タイマ (3G)場合 */
// 		/* FLD-MTデータコマンド転送受信応答確認タイマ (LTE)場合 */
// 		case CMD_TIMID_FMCOMRXCF_3G:
// 		case CMD_TIMID_FMCOMRXCF_LTE:
// 			/* FLD-MTデータコマンド転送受信応答確認タイマ タイムアウト処理 */
// 			m_rc_Fld_CmdRcvResConfirmTo( adwTimerId );
// 			break;
// 		/* FLD-MTデータファイル転送送信確認タイマ (3G)場合 */
// 		/* FLD-MTデータファイル転送送信確認タイマ (LTE)場合 */
// 		case CMD_TIMID_FLD_FILE_SND_3G:
// 		case CMD_TIMID_FLD_FILE_SND_LTE:
// 			/* FLD-MTデータファイル転送送信確認タイマタイムアウト処理 */
// 			m_rc_Fld_FilSndResConfirmTo( adwTimerId );
// 			break;
// 		/*FLD-MT接続強制解放確認タイマ (3G)場合*/
// 		/*FLD-MT接続強制解放確認タイマ (LTE)場合*/
// 		case CMD_TIMID_FLD_MT_CONN_CONF_3G:
// 		case CMD_TIMID_FLD_MT_CONN_CONF_LTE:
// 			/*FLD-MT接続強制解放確認タイマ タイムアウト処理*/
// 			m_rc_FldMt_ConnectTo( adwTimerId  );
// 			break;
// 		/* FLD-MTデータコマンド転送送信確認タイマ (3G)場合 */
// 		/* FLD-MTデータコマンド転送送信確認タイマ (LTE)場合 */
// 		case CMD_TIMID_MT_DATA_TRANS_SND_3G:
// 		case CMD_TIMID_MT_DATA_TRANS_SND_LTE:
// 			/* FLD-MTデータファイル転送送信確認タイマタイムアウト処理 */
// 			m_rc_FldMt_DataCmdTransferSndTO( adwTimerId );
// 			break;
// 		/*SV-MT/eNB-MT接続強制解放確認タイマ (3G)場合*/
// 		/*SV-MT/eNB-MT接続強制解放確認タイマ (LTE)場合*/
// 		case CMD_TIMID_SVeNB_MT_CONN_CONF_3G:
// 		case CMD_TIMID_SVeNB_MT_CONN_CONF_LTE:
// 			/*SV-MT/eNB-MT接続確認タイマタイムアウト処理*/
// 			m_rc_SvMt_ConnectTo( adwTimerId );
// 			break;
// 		/*FLD-MT接続強制解放確認タイマ (3G)場合*/
// 		/*FLD-MT接続強制解放確認タイマ (LTE)場合*/
// 		case CMD_TIMID_FLD_MT_CONN_REL_FORCE_3G:
// 		case CMD_TIMID_FLD_MT_CONN_REL_FORCE_LTE:
// 			/*FLD-MT接続強制解放確認タイマ タイムアウト処理*/
// 			m_rc_Fld_Mt_ForReleaseConnectTO( adwTimerId );
// 			break;
// 		/*SV-MT/eNB-MT接続解放確認タイマ(3G)場合*/
// 		/*SV-MT/eNB-MT接続解放確認タイマ(LTE)場合*/
// 		case CMD_TIMID_SVMTCNTREL_3G:
// 		case CMD_TIMID_SVMTCNTREL_LTE:
// 			/*SV-MT/eNB-MT接続解放確認 タイムアウト処理*/
// 			m_rc_SVMTCntRelTO(adwTimerId);
// 			break;
// 		/*SV-MT/eNB-MTデータ受信確認タイマ(3G)場合*/
// 		/*SV-MT/eNB-MTデータ受信確認タイマ(LTE)場合*/
// 		case CMD_TIMID_SVMTDATRCV_3G:
// 		case CMD_TIMID_SVMTDATRCV_LTE:
// 			/*SV-MT/eNB-MTデータ受信確認 タイムアウト処理*/
// 			m_rc_SVMTDatRcvTO(adwTimerId);
// 			break;
// 		/*SV-MT/eNB-MTデータ送信確認タイマ(3G)場合*/
// 		/*SV-MT/eNB-MTデータ送信確認タイマ(LTE)場合*/
// 		case CMD_TIMID_SVMTDATSND_3G:
// 		case CMD_TIMID_SVMTDATSND_LTE:
// 			/*SV-MT/eNB-MTデータ受信確認 タイムアウト処理*/
// 			m_rc_SvMt_DataSndTo(adwTimerId);
// 			break;
		/* 外部装置データ送信応答待ちタイムアウト(3G) */
		/* 外部装置データ送信応答待ちタイムアウト(LTE) */
		// case CMD_TIMID_EXT_DAT_SND_3G: /* 2021/01/27 M&C)DuongMX Item No.25 - RE AISG/TILT Monitoring and Control (modify) */
		case CMD_TIMID_EXT_DAT_SND_LTE:
			m_rc_ExtDataSndRspTO(adwTimerId);
			break;

// 		/*FLD-MTデータファイル転送受信確認タイマ(3G)場合*/
// 		/*FLD-MTデータファイル転送受信確認タイマ(LTE)場合*/
// 		case CMD_TIMID_FLDMT_FILE_TRANS_3G:
// 		case CMD_TIMID_FLDMT_FILE_TRANS_LTE:
// 			/* FLD-MTデータファイル転送受信確認タイマタイムアウト処理*/
// 			m_rc_FldMt_FilTransTO(adwTimerId);
// 			break;
// 		/* FLD-MT接続解放確認タイマ */
// 		/* FLD-MT接続解放確認タイマ for S3G */
// 		case CMD_TIMID_FMRELCF:
// 		case CMD_TIMID_FMRELCF_S3G:
// 			/*FLD-MT接続解放確認タイマタイムアウト処理*/
// 			m_rc_ReFldReDiscReqTO( adwTimerId  );
// 			break;

		/* 外部装置データ受信応答待ちタイムアウト(3G) */
		/* 外部装置データ受信応答待ちタイムアウト(LTE) */
		// case CMD_TIMID_EXT_DAT_RCV_3G: /* 2021/01/27 M&C)DuongMX Item No.25 - RE AISG/TILT Monitoring and Control (modify) */
		case CMD_TIMID_EXT_DAT_RCV_LTE:
			m_rc_ExtDataRcvRspTo(adwTimerId);
			break;
// 		/* MTアドレス設定応答待ちタイムアウト(3G) */
// 		/* MTアドレス設定応答待ちタイムアウト(LTE) */
// 		case CMD_TIMID_MTADDSET_3G:
// 		case CMD_TIMID_MTADDSET_LTE:
// 			m_rc_Mt_AddressSetTo( adwTimerId );
// 			break;
		/* RE立ち上げ待ちタイマ タイムアウト	*/
		case CMD_TIMID_STARTUPRE:
			if(delay_adjust_step3 == D_RRH_OFF){
				/* RE立ち上げ待ちタイマ がTOしたら 全RE起動待ちを終了する	*/
			    delayPrmChkEn = D_RRH_OFF;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE starting timer TO");
				m_rc_infoAllRE(delayPrmChkEn);
			}
			else{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Step3 TO after a CMD_TSKIF_RE_STARTUP_RSP");
			}
			break;
		/* Step7完了待ちタイマ タイムアウト */
		case CMD_TIMID_WAIT_COMPLETE_STEP7:
			if(delay_adjust_step3 == D_RRH_ON){
				re_StartUp_CPRI_Conn_flg = D_RRH_ON;	/* RE CPRI接続完了	*/
				delayPrmChkEn = D_RRH_OFF;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE starting Step7 timer TO");
				m_rc_infoAllRE(delayPrmChkEn);
			}
			else{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Step7 TO after a CMD_TSKIF_RE_STARTUP_RSP");
			}
			break;

/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete start) */
// 		/* REバージョン報告確認タイマ for 3G */
// 		/* REバージョン報告確認タイマ for LTE */
// 		case CMD_TIMID_RE_VER_REP_3G:
// 		case CMD_TIMID_RE_VER_REP_LTE:
// 			m_rc_ReVerRepTO(adwTimerId);
// 			break;
/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete end) */

		/* REカード情報報告応答(3G)待ちタイムアウト */
		//case CMD_TIMID_CARDINFOREP_3G:
		//	m_rc_ReCardInfoRepRspTimeout_3g(adwTimerId);
		//	break;
		default:
			break;
	}
	if((CMD_TIMID_TRXSET_3GMIN <= adwTimerId) && (adwTimerId<= CMD_TIMID_TRXSET_LTEMAX))
	{
		/* rctスレッドのTRX設定確認タイマ タイムアウト */
		m_rc_TrxSet_Trans_TO( adwTimerId );
	}

	if((CMD_TIMID_TRXREL_3GMIN <= adwTimerId) && (adwTimerId<= CMD_TIMID_TRXREL_LTEMAX))
	{
		/* rctスレッドのTRX解放確認タイマ タイムアウト */
		m_rc_TrxRel_Trans_TO( adwTimerId );
	}
	return;

}

/* @} */


/* @} */

