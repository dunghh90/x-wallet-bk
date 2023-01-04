/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReUseSet.c
 *  @brief  RE使用/未使用設定
 *  @date   2015/10/05 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/********************************************************************************************************************/

/**
 * @addtogroup RRH_L3_RCT
 * @{
 */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  RE use status set request
 *  @param  parm_p [in] RE use status set request
 *  @return None
 *  @date   2015/10/05 TDI)satou create
 *  @date   2015/10/22 TDI)satou その他未実装-007 LED設定処理を追加
 *  @date   2015/11/11 TDI)enoki IT2問題 No167
 *  @date   2016/03/03 TDI)enoki RE減設にてRE自律リセット抑止解除要求をREプロセスに送信
 *  @date	2016/04/19 TDI)satou RE PORT状態報告(LTE)の共有メモリ更新処理を追加
 *  @date   2016/11/22 FJT)ohashi TILTリセットによるスロット情報初期化処理抑止
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/********************************************************************************************************************/
VOID m_rc_ReUseSet(T_API_CPRILINK_SET_REUSE_IND *parm_p) {
    USHORT offset;
    UCHAR data;
    INT ret;
    T_RRH_CONN_DISC shmConnDisc;
	CMT_TSKIF_CPRIDISCONN_NTC		tLinkDiscNtc = {};


    /* EEPROM「Master CPRIリンク#n有効/無効(DIP-SW)」更新 */
    offset = M_RRH_EEP_CPRILINK_ENABLE(parm_p->link_num);
    data   = (UCHAR)parm_p->onoff;
    ret    = BPF_HM_DEVC_EEPROM_WRITE(offset, &data);
    if (BPF_HM_DEVC_COMPLETE != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_HM_DEVC_EEPROM_WRITE NG. offset=%x data=%x", offset, data);
    }

    /*
     * REC起動未完了の場合はEEPROMのみの反映になる。L3_rctは、
     * REC起動シーケンス完了時にEEPROMの状態を参照し、REC起動シーケンス実施中に
     * 増設/減設がされていることを検出した場合、そこから以降の増設/減設の処理を
     * 実施する。
     */
    if (CMD_SYS_S3G == cmw_validsysinfo) {  /* LTE単独構成 */
        if (D_RRH_ON != rec_Startup_First_flg[CMD_SYS_S3G]) {
            return;
        }
    } else {                                /* 共用構成 */
        if ((D_RRH_ON != rec_Startup_First_flg[CMD_SYS_S3G])
        ||  (D_RRH_ON != rec_Startup_First_flg[CMD_SYS_3G ])) {
            return;
        }
    }

    /* 共有メモリ更新 */
    f_cmn_com_conn_disk_get(&shmConnDisc);
    shmConnDisc.conn_disc[parm_p->link_num - 1] = parm_p->onoff;
    f_cmn_com_conn_disk_set(&shmConnDisc);

    /* 対象REのLayer3状態を起動中アイドルに設定 */
    f_cmn_com_layer3_set_each(parm_p->link_num, CMD_SYS_3G,  (USHORT)E_RRH_LAYER3_RE_IDLE);
    f_cmn_com_layer3_set_each(parm_p->link_num, CMD_SYS_S3G, (USHORT)E_RRH_LAYER3_RE_IDLE);

    /* RE PORT状態報告(LTE)の共有メモリを更新(該当REの情報を未受信で初期化) */
    f_cmn_com_RePortSta_s3g_set(parm_p->link_num, D_RRH_REPORTSTA_RSP_NOT_RECEIVED, 0, 0);

    if (D_RRH_ON == parm_p->onoff) {    /* CPRIリンク有効に切り替え */
        /* l3_cprに「RE CPRI Link Start Notice」を送信 */
        m_rc_SendMsg_reCpriLinkSat(parm_p->link_num, E_RRH_SFPLOG_TRIGGER_CPRI_USE);
    } else {                            /* CPRIリンク無効に切り替え */
    	/* RE減設時はL1/L2断検出時と同様の動作を行う */
    	/* そのため、疑似的にL1/L2断検出信号を作成し、以下関数を呼び出す。 */
    	/* ※但し作成するメッセージバッファは型を併せて参照するだけなのでスタックメモリとする */
    	tLinkDiscNtc.link_num	= parm_p->link_num;
    	tLinkDiscNtc.layer_kind	= E_RRH_RAYER_KIND_1_2;
    	
    	/*  切断要求されたREがまだバッファにある場合はバッファから削除をして終了 */
		m_rc_ReDis_buffchk(&tLinkDiscNtc);

    	/*  CPRIリンク断発生時の自律TRX解放 */
		m_rc_CPRIdis_TrxRel(&tLinkDiscNtc);

    	/* RE自律リセット抑止解除要求送信 */
        m_rc_ReSelfReset_PreventOff_Req_Send(parm_p->link_num, CMD_ON);

    	/* l3_cprに「RE CPRI Link Stop Notice」を送信 */
        m_rc_SendMsg_reCpriLinkStp(parm_p->link_num);
		/*	CPRI未使用が確定したため、対象REのCPRI異常をREカード状態報告の故障状態に反映しない	*/
		re_funcFailValid[parm_p->link_num] = D_RRH_OFF;
    }

    /* LED設定処理 */
    m_cm_WriteLEDReg();
}

/********************************************************************************************************************/
/**
 *  @brief  REC起動完了までに変更されたRE接続有無状態の内容を反映する
 *  @return None
 *  @date   2015/10/07 TDI)satou create
 *  @date   2016/03/03 TDI)enoki RE減設にてRE自律リセット抑止解除要求をREプロセスに送信
 *  @date	2016/04/19 TDI)satou RE PORT状態報告(LTE)の共有メモリ更新処理を追加
 *  @date   2016/11/22 FJT)ohashi TILTリセットによるスロット情報初期化処理抑止
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/********************************************************************************************************************/
VOID m_rc_ReUseApplySettingsInBoot(VOID) {
    INT link_num;
    USHORT offset;
    UCHAR data;
    INT ret;
    T_RRH_CONN_DISC shmConnDisc;

    if (CMD_SYS_S3G == cmw_validsysinfo) {  /* LTE単独構成 */
        if (D_RRH_ON != rec_Startup_First_flg[CMD_SYS_S3G]) {
            return;
        }
    } else {                                /* 共用構成 */
        if ((D_RRH_ON != rec_Startup_First_flg[CMD_SYS_S3G])
        ||  (D_RRH_ON != rec_Startup_First_flg[CMD_SYS_3G ])){
            return;
        }
    }

    /* 共有メモリから現在の設定値を取得 */
    f_cmn_com_conn_disk_get(&shmConnDisc);

    /* 設定値と異なるCPRIリンク番号について増設/減設を実施 */
    for (link_num = D_RRH_CPRINO_RE_MIN; link_num <= D_RRH_CPRINO_RE_MAX; link_num++) {
        offset = M_RRH_EEP_CPRILINK_ENABLE(link_num);
        ret    = BPF_HM_DEVC_EEPROM_READ(offset, &data);
        if (BPF_HM_DEVC_COMPLETE != ret) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_HM_DEVC_EEPROM_READ NG. offset=%x", offset);
            return;
        }

        if ((data == 1) && (shmConnDisc.conn_disc[link_num - 1] == 1)) {
            continue;
        }

        if ((data != 1) && (shmConnDisc.conn_disc[link_num - 1] != 1)) {
            continue;
        }

        /* 共有メモリに最新の設定値を適用 */
        shmConnDisc.conn_disc[link_num - 1] = (data == 1) ? D_RRH_ON : D_RRH_OFF;
        f_cmn_com_conn_disk_set(&shmConnDisc);

        /* 対象REのLayer3状態を起動中アイドルに設定 */
        f_cmn_com_layer3_set_each(link_num, CMD_SYS_3G,  (USHORT)E_RRH_LAYER3_RE_IDLE);
        f_cmn_com_layer3_set_each(link_num, CMD_SYS_S3G, (USHORT)E_RRH_LAYER3_RE_IDLE);

        if (D_RRH_ON == shmConnDisc.conn_disc[link_num - 1]) {
            /* CPRIリンク有効に切り替え */
            /* l3_cprに「RE CPRI Link Start Notice」を送信 */
            m_rc_SendMsg_reCpriLinkSat(link_num, E_RRH_SFPLOG_TRIGGER_CPRI_USE);
        } else {
 			/* RE自律リセット抑止解除要求送信 */
        	m_rc_ReSelfReset_PreventOff_Req_Send(link_num, CMD_ON);
            /* CPRIリンク無効に切り替え */
            /* l3_cprに「RE CPRI Link Stop Notice」を送信 */
            m_rc_SendMsg_reCpriLinkStp(link_num);
			/*	CPRI未使用が確定したため、対象REのCPRI異常をREカード状態報告の故障状態に反映しない	*/
            re_funcFailValid[link_num] = D_RRH_OFF;
       }
    }
}

/********************************************************************************************************************/
/**
 *  @brief  RE減設された場合にRE自律リセット抑止解除要求をREプロセスに送信する
 *  @param  parm_p [in] CPRINo
 *  @return None
 *  @date   2016/03/03 tdips)enoki create
 *  @date   2016/11/22 FJT)ohashi TILTリセットによるスロット情報初期化処理抑止
 */
/********************************************************************************************************************/
VOID m_rc_ReSelfReset_PreventOff_Req_Send( USHORT linkno, USHORT reoff_flag) {
	UINT					bufget_rslt;			/* バッファ獲得結果 */
	UINT					a_rtn;         			/* 戻り値判定用     */
	CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ		*SendMsg;	/* RE切断要求作成先 */

	bufget_rslt = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ), (VOID **)&SendMsg);
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_ReSelfReset_PreventOff_Req_Send cm_BReq NG");
		return;
    }

	SendMsg->head.uiEventNo		= CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ;
	SendMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_RE;
	SendMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;
	SendMsg->head.uiDstTQueueID	= 0;
	SendMsg->head.uiSrcTQueueID	= 0;
	SendMsg->head.uiLength		= sizeof( CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ );
	SendMsg->link_num			= linkno;
	SendMsg->reoff_flag			= reoff_flag;
	a_rtn = cm_Enter(SendMsg->head.uiDstPQueueID, CMD_QRB_NORMAL, CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ, SendMsg);

	/* エラー時アサート */
	if(a_rtn != BPF_RU_IPCM_OK)
	{
	    cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	    return;
	}
}

/** @} */
