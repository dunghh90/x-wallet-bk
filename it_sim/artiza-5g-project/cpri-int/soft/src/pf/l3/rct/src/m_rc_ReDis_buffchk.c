/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReDis_buffchk.c
 *  @brief  CPRI切断通知
 *  @date   2015/08/13 TDIPS)uchida create
 */
/********************************************************************************************************************/

/*****************************************************************************************************************//**
 *
 * @addtogroup RRH_L3_RCT
 * @{
 *
 ********************************************************************************************************************/

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "f_rrh_reg_cnt.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/********************************************************************************************************************/
/**
 *  @brief    CPRI切断通知
 *  @param    buff_adr [in] メッセージデータ
 *  @return   none
 *  @date     2015/08/13 TDIPS uchida create
 *  @date     2015/08/13 TDIPS)ikeda RE増減設コーディングミス修正
 *  @date     2015/08/20 TDIPS)uchida 不適切だったファイル名、関数名を変更
 *  @date     2015/10/16 FPT)yen add REファイル更新中止指示
 *  @date     2015/10/26 TDI)satou FHM技説-QA-103
 */
/********************************************************************************************************************/
VOID m_rc_ReDis_buffchk(VOID* buff_adr)
{
	UINT 	cnt1;
	CMT_TSKIF_CPRIDISCONN_NTC *parm_p;
	parm_p = (CMT_TSKIF_CPRIDISCONN_NTC*)buff_adr;
	CMT_TSKIF_RENEWSTPNTC*			lReNewStpNtc_p;	/* RE更新停止指示 */
	UINT							a_rtn = CMD_RES_OK;

	/* REC の場合 */
	if (parm_p->link_num < D_RRH_CPRINO_RE_MIN)
	{
		/* バッファ取得 */
		a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_RENEWSTPNTC ), (VOID **)&lReNewStpNtc_p );
		/* 戻り値判定 */
		if( a_rtn != CMD_RES_OK ){
			/* バッファ取得失敗時はアボート処理 */
			cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_CpriDisConnectNtc",
							   "cm_BReq NG",
							   a_rtn, sizeof( CMT_TSKIF_CPRIDISCONN_NTC ), CMD_NUM0 );
		}

		/* タスク間フレームのデータ作成 */
		lReNewStpNtc_p->head.uiEventNo = CMD_TSKIF_REFIRM_DL_STOP_IND;
		lReNewStpNtc_p->head.uiDstPQueueID = D_RRH_PROCQUE_L3;
		lReNewStpNtc_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
		lReNewStpNtc_p->head.uiDstTQueueID = CMD_TSKID_DLM;
		lReNewStpNtc_p->head.uiSrcTQueueID = CMD_TSKID_RCT;
		lReNewStpNtc_p->head.uiLength = sizeof( CMT_TSKIF_RENEWSTPNTC );
		lReNewStpNtc_p->systerm = parm_p->system_type;

		/* REファイル更新中止指示を送信 */
		a_rtn = cm_Enter( CMD_TSKID_DLM, CMD_QRB_NORMAL,
				CMD_TSKIF_REFIRM_DL_STOP_IND, lReNewStpNtc_p );
		if(a_rtn != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
		}
	}
	else
	{
	    /* 起動完了をクリア */
	    if (E_RRH_RAYER_KIND_1_2 == parm_p->layer_kind)
	    {
            re_startupRSP[parm_p->link_num][CMD_SYS_3G ] = D_RRH_OFF;
            re_startupRSP[parm_p->link_num][CMD_SYS_S3G] = D_RRH_OFF;
            
			/*	強制停止状態ERRを回復させる	*/
			m_rc_forceStopErrRel(parm_p->link_num, parm_p->system_type);
	    }

		/* MT接続されているCPRIリンクの断を検出した場合、RECに対してMT接続解放要求を送信する */
		m_rc_RelFLDMT_CpriDwn(parm_p->link_num, parm_p->system_type);
		m_rc_RelSVMT_CpriDwn(parm_p->link_num, parm_p->system_type);
		
		/* CPRI接続通知バッファを確認 */
		for(cnt1 =0; cnt1 <rcw_buffcnt; cnt1++)
		{
			/* 切断要求のCPRI番号がバッファにある場合 */
			if(rcw_system_type[cnt1] == parm_p->system_type && rcw_cpri[cnt1] == parm_p->link_num)
			{
				/* そのCPRI番号をバッファから削除しバッファ配列を詰める */
				memcpy(&rcw_cpri[cnt1],			&rcw_cpri[cnt1+1],			sizeof(rcw_cpri[0])			* (rcw_buffcnt - cnt1 - 1));
				memcpy(&rcw_system_type[cnt1],	&rcw_system_type[cnt1+1],	sizeof(rcw_system_type[0])	* (rcw_buffcnt - cnt1 - 1));
				memcpy(&rcw_thread_qid[cnt1],	&rcw_thread_qid[cnt1+1],	sizeof(rcw_thread_qid[0])	* (rcw_buffcnt - cnt1 - 1));
				rcw_buffcnt--;
				return;
			}
		}
		/* バッファに無い場合RE切断要求を送信 */
		m_rc_ReDis_Req(parm_p->link_num, parm_p->layer_kind, parm_p->system_type);
	}

	return;
}

/********************************************************************************************************************/
/**
 *  @brief    FLD-MT接続解放(CPRIリンク断発生時)
 *  @note     FLD-MT接続中の配下REとのCPRIリンクが切断された場合、自律でRECに解放を要求する
 *  @param    link_num    [in] CPRIリンク番号
 *  @param    system_type [in] 3G/LTE
 *  @return   none
 *  @date     2015/10/26 TDI)satou FHM技説-QA-103 create
 */
/********************************************************************************************************************/
VOID m_rc_RelFLDMT_CpriDwn(USHORT link_num, USHORT system_type)
{
    CMT_TSKIF_CPRISND_FLMTCNCRELREQ *relReq;
    T_RRH_LAYER3 layer3;
    UINT ret;

    if (D_L3_MT_CONNECT != gw_rcw_FldMt_ConnSts[system_type])
    {
        return;
    }

    gw_rcw_FldMt_ConnSts[system_type] = D_L3_MT_NOT_CONNECT;

    /* 当該REがリンク断前にFLD-MT接続解放要求を送信していた場合、RECに要求を送信しない */
    if (D_L3_ON == gw_rcw_FldMt_ConnRel_WaitFlg[system_type])
    {
        return;
    }

    /*
     * FLD-MT接続解放要求をRECに送信する
     * RECとのCPRIリンク断が発生していた場合は、要求を送信せず、解放済みの状態に.
     * RECからの応答を待つ必要がないため応答待ちタイマ起動もせず、投げっぱなしにする.
     */
    f_cmn_com_layer3_get(system_type, &layer3);
    if (E_RRH_LAYER3_REC_OPE != layer3.layer3_rec)
    {
        return;
    }

    ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof *relReq, 1, (VOID**)&relReq);
    if (CMD_RES_OK != ret)
    {
        cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_CutOffFLDMT_CpriDwn", "buffer hunt fail", ret, sizeof *relReq, 0);
    }

    relReq->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_FLMTCNCRELREQ);
    relReq->head.uiSrcPQueueID   = D_RRH_PROCQUE_L3;
    relReq->head.uiSrcTQueueID   = CMD_TSKID_RCT;
    relReq->cprisnd_inf.link_num = D_RRH_CPRINO_REC;
    relReq->cpridat_flmtcncrelreq.signal_kind = CMD_CPRID_FLMTCNCRELREQ + system_type;

    ret = m_cm_L3MsgSend_REC(relReq);
    if (D_RRH_OK != ret)
    {
        cm_Assert(CMD_ASL_DBGLOW, ret, "m_rc_RelFLDMT_CpriDwn NG");
    }
}

/********************************************************************************************************************/
/**
 *  @brief    SV-MT接続解放(CPRIリンク断発生時)
 *  @note     SV-MT接続中の配下REとのCPRIリンクが切断された場合、自律でRECに解放を要求する
 *  @param    link_num    [in] CPRIリンク番号
 *  @param    system_type [in] 3G/LTE
 *  @return   none
 *  @date     2015/10/26 TDI)satou FHM技説-QA-103 create
 */
/********************************************************************************************************************/
VOID m_rc_RelSVMT_CpriDwn(USHORT link_num, USHORT system_type)
{
    CMT_TSKIF_CPRISND_SVMTCNTRELREQ *relReq;
    T_RRH_LAYER3 layer3;
    UINT ret;

    if (D_L3_MT_CONNECT != gw_rcw_SvEnbMt_ConnSts[system_type])
    {
        return;
    }

    gw_rcw_SvEnbMt_ConnSts[system_type] = D_L3_MT_NOT_CONNECT;

    /* 当該REがリンク断前にFLD-MT接続解放要求を送信していた場合、RECに要求を送信しない */
    if (D_L3_ON == gw_rcw_SvMt_CntRel_WaitFlg[system_type])
    {
        return;
    }

    /*
     * SV-MT/eNB-MT接続解放要求をRECに送信する
     * RECとのCPRIリンク断が発生していた場合は、要求を送信せず、解放済みの状態に.
     * RECからの応答を待つ必要がないため応答待ちタイマ起動もせず、投げっぱなしにする.
     */
    f_cmn_com_layer3_get(system_type, &layer3);
    if (E_RRH_LAYER3_REC_OPE != layer3.layer3_rec)
    {
        return;
    }

    ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof *relReq, 1, (VOID**)&relReq);
    if (CMD_RES_OK != ret)
    {
        cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_RelSVMT_CpriDwn", "buffer hunt fail", ret, sizeof *relReq, 0);
    }

    relReq->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_FLMTCNCRELREQ);
    relReq->head.uiSrcPQueueID   = D_RRH_PROCQUE_L3;
    relReq->head.uiSrcTQueueID   = CMD_TSKID_RCT;
    relReq->cprisnd_inf.link_num = D_RRH_CPRINO_REC;
    relReq->cpridat_svmtcntrelreq.signal_kind = CMD_CPRID_SVMTCNTRELREQ + system_type;

    ret = m_cm_L3MsgSend_REC(relReq);
    if (D_RRH_OK != ret)
    {
        cm_Assert(CMD_ASL_DBGLOW, ret, "m_rc_RelSVMT_CpriDwn NG");
    }
}

/********************************************************************************************************************/
/**
 *  @brief    強制停止ERR解除(CPRIリンク断発生時)
 *  @note     強制停止中にCPRIリンクが切断された場合、強制停止ERRを回復させる
 *  @param    link_num    [in] CPRIリンク番号
 *  @param    system_type [in] 3G/LTE
 *  @return   none
 *  @date     2015/12/04 FJT)koshida create
 */
/********************************************************************************************************************/
VOID m_rc_forceStopErrRel(USHORT link_num, USHORT system_type)
{
	T_RRH_LAYER3	layer3;
	UINT svRegAdr, svRegDat;
	
	/*	Master Portの場合	*/
	if(link_num != D_RRH_CPRINO_REC)
	{
		f_cmn_com_layer3_get(system_type, &layer3);
		/*	3Gリンクが強制停止の場合	*/
		if(layer3.layer3_re[ link_num-1 ] == E_RRH_LAYER3_RE_STOP)
		{
			/*	3Gシステムの場合	*/
			if(system_type == CMD_SYS_3G)
			{
				/*	キャリア状態不一致(3G)	*/
				svRegAdr = D_RRH_REG_CNT_SV15INF;
				svRegDat = M_RRH_REG_CNT_3G_CARRRESET_ERR(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
				
				/*	遅延ERR(3G)	*/
				svRegAdr = D_RRH_REG_CNT_SV13INF;
				svRegDat = M_RRH_REG_CNT_DELAY_ERR1(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
				
				/*	起動NG(3G)	*/
				svRegAdr = D_RRH_REG_CNT_SV14INF;
				svRegDat = M_RRH_REG_CNT_STARTUP_ERR1(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
			}
			/*	LTEシステムの場合	*/
			else
			{
				/*	キャリア状態不一致(LTE)	*/
				svRegAdr = D_RRH_REG_CNT_SV15INF;
				svRegDat = M_RRH_REG_CNT_S3G_CARRRESET_ERR(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
				
				/*	ファイル更新ERR(LTE)	*/
				svRegAdr = D_RRH_REG_CNT_SV14INF;
				svRegDat = M_RRH_REG_CNT_DL_NG_ERR(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
				
				/*	遅延ERR(LTE)	*/
				svRegAdr = D_RRH_REG_CNT_SV13INF;
				svRegDat = M_RRH_REG_CNT_DELAY_ERR2(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);

				/*	起動NG(LTE)	*/
				svRegAdr = D_RRH_REG_CNT_SV16INF;
				svRegDat = M_RRH_REG_CNT_S3G_RESTARTUP_ERR(link_num);
				m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
			}
		}
	}
	return;
}
/*****************************************************************************************************************//**
 *
 * @} addtogroup RRH_L3_RCT
 *
 ********************************************************************************************************************/
