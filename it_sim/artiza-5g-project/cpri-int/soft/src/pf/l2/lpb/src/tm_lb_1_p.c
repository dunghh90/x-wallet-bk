/*!
 * @skip    $Id$
 * @file    tm_lb_1_p.c
 * @brief   LAPB単一のリンク制御関数
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 * @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
 * @date    2013.11.25 FFCS)hongj modify for zynq.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"
#include "l2_com_inc.h"

/********************************************************************************************************/

/*!
 *  @brief   Layer 1 Data CNF処理(tm_lb_1_m1e0s0p)
 *             -(State 3,State 4,State 5,State 6 common function)
 *  @note    下位モジュールよりLayer 1 Data CNF (送信完了通知)を受信した後の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e0s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
											/************************************************/
											/*   Layer 1 Data CNF  From POLLING   			*/
											/*   Share by state3 state 4,state 5, state 6	*/
											/************************************************/

													/*-----------------------*/
													/*     Confirm 処理 1    */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_ON)
	{
		lblap_conf1(s_lblap_p);
	}
	return;
}
/********************************************************************************************************/

/*!
 *  @brief   Command Frame処理(tm_lb_1_m1e2s0p)
 *             -(State 3,State 4 common function)
 *  @note    
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e2s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*************************/
													/*       Command Frame   */
													/*************************/
	UINT		Result;                                     /*!< */

	if(s_lblap_p->lap_dt.status == LBD_L2_L1ACTSTS)
	{
		return;
	}
													/*-----------------------*/
													/*      コマンド ?       */
													/*      P = 1    ?       */
													/*-----------------------*/
	if((s_lblap_p->cmd_rsp_flg != CMD_NUM1)
		|| (s_lblap_p->pf_sav != CMD_NUM1))
	{
		return;
	}
													/*-----------------------*/
													/*  Control Field SABM ? */
													/*-----------------------*/
	if(s_lblap_p->ctl_sv == LBD_SBMCTL)
	{
													/*-----------------------*/
													/* YES (SABM)            */
													/*     ABLOG 収集        */
													/*-----------------------*/
		lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSDM);
	}
													/*-----------------------*/
													/*    L2機能用TR Hunt    */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
				/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s0p", 
						"Breq NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/* P/F Workエリア 1 Set  */
													/*-----------------------*/
	s_lblap_p->pf_wk = CMD_NUM1;
													/*-----------------------*/
													/*    DM フレーム 作成処理   */
													/*-----------------------*/
	lblap_mkdm(s_lblap_p);
													/*-----------------------*/
													/*   U  Frame 送信処理   */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   データリンク設定要求処理(tm_lb_1_m1e18s0p)
 *              -(State 3,State 4 common function)
 *  @note    データリンクを確立させる。(自局よりSABMを送出し、リンクを確立)
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_1_m1e18s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/************************************************/
												/*   データリンク設定要求処理  From CPRI MNG  	*/
												/*   Share by state 3 and state 4 				*/
												/************************************************/
													/*--------------------------*/
													/*   データ リンク 確立処理	*/
													/*--------------------------*/
	lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*        Flag ON        */
													/*-----------------------*/
	s_lblap_p->lap_dt.l3int_flg = CMD_ON;

													/*-----------------------*/
													/*       Flag Clear      */
													/*-----------------------*/
	s_lblap_p->rcv_sbm_flg = CMD_OFF;
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;

	lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_N);
	tm_lb_setupl2link_rsp(s_lblap_p->link_num, CMD_OK, "OK");
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Layer 1ビジー間隔タイマタイムアウト処理(tm_lb_1_m1e25s0p)
 *              -(State 3,State 4 common function)
 *  @note    Layer 1 busy status will be cancelled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_1_m1e25s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UINT		Result;                                     /*!< */
	T_RRH_TIMERTO_NTC *timNtc_p;
	char		work[192];
	
	if(s_lblap_p->lap_dt.l1bsy_flg != CMD_ON)
	{
		return;
	}

	timNtc_p = (T_RRH_TIMERTO_NTC*)a_inf_p;
	if((timNtc_p->data.id_os != s_lblap_p->l1bsydettmr_osid)
		|| (timNtc_p->data.id  != s_lblap_p->l1bsydettmr_id2))
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Id mismatch key:%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}
	if(timNtc_p->data.userkey != lbw_l1BusyKey[s_lblap_p->link_num])
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Key mismatch key:0x%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}
	
													/*-----------------------*/
													/*   I Frame Chaning ?   */
													/*-----------------------*/
	if(s_lblap_p->tx_qu.qu_ctl != LBD_L2_TXI)
	{
													/*-----------------------*/
													/* NO (Not I Frame)      */
													/*      Buffer 解放      */
													/*-----------------------*/
		Result = cm_BFree((void *) s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s0p",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->tx_qu.qu_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/* I Frame 廃棄 Flag ON ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.dsc_iq_flg == CMD_ON)
	{

													/*-----------------------*/
													/* YES                   */
													/* バッファ廃棄処理開始  */
													/*-----------------------*/
		while(s_lblap_p->lap_dt.i_head_p != NULL)
		{
													/*-----------------------*/
													/* I Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* I Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s0p",
							"Buffer FreeNG_2", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			s_lblap_p->iframecnt--;
		}
													/*-----------------------*/
													/*     Pointer Null 化   */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = NULL;
		s_lblap_p->lap_dt.i_head2_p = NULL;
		s_lblap_p->lap_dt.i_tail_p = NULL;
													/*-----------------------*/
													/*       Flag Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.dsc_iq_flg = CMD_OFF;
	}

	while (s_lblap_p->lap_dt.s_head_p != NULL)
	{
													/*-----------------------*/
													/* S Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.s_head_p;
													/*-----------------------*/
													/* S Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_head_p =s_lblap_p->lap_dt.s_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s0p",
							"Buffer FreeNG_3", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
		
	}
	s_lblap_p->lap_dt.s_tail_p = NULL;

	while (s_lblap_p->lap_dt.u_head_p != NULL)
	{
													/*-----------------------*/
													/* U Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.u_head_p;
													/*-----------------------*/
													/* U Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_head_p =s_lblap_p->lap_dt.u_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s0p",
							"Buffer FreeNG_4", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
		
	}
	s_lblap_p->lap_dt.u_tail_p = NULL;

	s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;

	return;
}


/********************************************************************************************************/

/*!
 *  @brief   SABM受信処理(tm_lb_1_m1e6s1p)
 *              -(State 4)
 *  @note    データリンクを確立させる。(自局よりUAを送出し、リンクを確立)
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e6s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*----------------------*/
													/*  SABM     受信 		*/
													/*----------------------*/
	UINT					Result;                         /*!< */
	INT                     errcd;
 
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*-----------------------*/
													/*     L2処理用TR Hunt   */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), s_lblap_p->link_num, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
			/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e6s1p", 
					"Breq NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;


													/*-----------------------*/
													/*   UA Frame 作成処理   */
													/*-----------------------*/
	lblap_mkua(s_lblap_p);
													/*-----------------------*/
													/*   U  Frame 送出処理   */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/*  異常状態クリア 処理  */
													/*-----------------------*/
	lblap_clrcnd(s_lblap_p);
													/*-----------------------*/
													/*  状態変数クリア 処理  */
													/*-----------------------*/
	lblap_clrval(s_lblap_p);
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;

													/*-----------------------*/
													/*  T1-Flag RUNNING ?    */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{ 
													/*-----------------------*/
													/*  YES (T1-Flag RUNNING)*/
													/*     T1タイマ停止      */
													/*-----------------------*/
 		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
													/*-----------------------*/
													/*  Check return code    */
													/*-----------------------*/
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
													/*-----------------------*/
													/*     T1 Flag OFF       */
													/*-----------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}

													/*-----------------------*/
													/*    Next Status Go !   */
	s_lblap_p->lap_dt.status = LBD_L2_INFSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_ESTIND_N);
	/*call MW to set StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
	/*call MW to clear L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);													/* (Information Transfer)*/
	/*	16link接続時の暫定対処	*/
	/* CPRI State設定 */
/*	f_cmn_com_cpri_state_set_cprist(s_lblap_p->link_num,D_RRH_CPRISTAT_F,  D_RRH_ON,  0, 0);	*/

	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);
													/*-----------------------*/
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   DM受信処理(tm_lb_1_m1e8s1p)
 *              -(State 4)
 *  @note    DM(F=1),then this frame will be discarded \n
 *           DM(F=0),then send SABM(P=1) to peer, CPRI link will be established	
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e8s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    DM  Frame  受信    */
													/*-----------------------*/
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
//	UINT					Result;                         /*!< */


													/*-----------------------*/
													/*    F = 0???   		 */
													/*-----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM0)
	{
		return;
	}
													/*--------------------------*/
													/* 			YES		  		*/
													/*     	 リンク 確立処理    */
													/*--------------------------*/
	lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*      Flag 設定        */
													/*-----------------------*/
	s_lblap_p->lap_dt.l3int_flg = CMD_OFF;
	s_lblap_p->rcv_sbm_flg = CMD_OFF;
/*データリンク設定中表示*/
//	Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//													/*-----------------------*/
//													/*   Check return code   */
//													/*-----------------------*/
//	if(Result != CMD_RES_OK)
//	{
//		/*error processing*/
//		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e8s1p", 
//					"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//	}

	sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
    lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_N);
	tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, CMD_TSKIF_ESTINGIND, (UINT *)&sndbf_p);
	
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;

	return;
}
/********************************************************************************************************/

/*!
 *  @brief   L2 STOP依頼受信(tm_lb_1_m1e22s1p)
 *              -(State 4(MDIF))
 *  @note    Set link state to state 3
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e22s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/*****************************/
										/* L2 STOP依頼 from CPRI MNG */
										/*****************************/
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 DEACT状態) */
												/*-----------------------*/
		
	s_lblap_p->lap_dt.status = LBD_L2_L1ACTSTS;

	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Test mode start処理(tm_lb_1_m1e23s1p)
 *              -(State 4)
 *  @note    LAPB task will entry TEST State
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e23s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/***********************************/
										/*   L3-TEST-START  From CPRI MNG  */
										/***********************************/
	s_lblap_p->lap_dt.status = LBD_L2_TESTSTS;

	/*call MW to clear stateF*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 error*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);

	return;
}


/********************************************************************************************************/

/*!
 *  @brief   SABM受信(tm_lb_1_m1e6s2p)
 *              -(State 5)
 *  @note    end UA, after receiving UA or T1 time out, CPRI link will be established
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e6s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*  SABM 受信 			 */
													/*-----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*-----------------------*/
													/*    L2処理用TR Hunt    */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
			/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e6s2p", 
					"Breq NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}

													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    UA Frame 作成処理  */
													/*-----------------------*/
	lblap_mkua(s_lblap_p);
													/*-----------------------*/
													/*    U  Frame 送出処理  */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/*  SABM 受信フラグON    */
													/*-----------------------*/
	s_lblap_p->rcv_sbm_flg = CMD_ON;
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   DISC受信(tm_lb_1_m1e7s2p)
 *              -(State 5)
 *  @note    Send UA, CPRI link establishment will be canceled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e7s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*   DISC Frame  受信    */
													/*-----------------------*/

	UINT		Result;                                     /*!< */
	INT         errcd;
	
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*-----------------------*/
													/*    L2処理用TR Hunt	 */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_buf_p);
	if(Result != CMD_RES_OK)
	{
			/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e7s2p", 
					"Breq NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    DM Frame 作成処理  */
													/*-----------------------*/
	lblap_mkdm(s_lblap_p);
													/*-----------------------*/
													/*     U  Frame 送出処理 */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);

	


	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
													/*-----------------------*/
													/*  T1タイマ停止処理     */
													/*-----------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
													/*-----------------------*/
													/*      Flag OFF         */
													/*-----------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
													/*-----------------------*/
													/*     統計カウント処理  */
													/*-----------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
													/*-----------------------*/
													/*     Next Status Go !  */
													/*     (L1/L2 ACT 状態)  */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 erro*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   DM受信(tm_lb_1_m1e8s2p)
 *              -(State 5)
 *  @note    Received DM(F=0), the frame will be discarded\n
 *           Received DM(F=1), CPRI link establishment will be canceled	
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e8s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    DM  Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;

													/*-----------------------*/
													/*  P/F = 1 ?            */
													/*-----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}

	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
														/*-------------------*/
														/*  T1タイマ停止処理 */
														/*-------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
														/*-------------------*/
														/*  Check return code*/
														/*-------------------*/
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
														/*-------------------*/
														/*  T1タイマフラグOFF*/
														/*-------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生回数)*/
													/*------------------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
													/*-----------------------*/
													/*     ABLOG 収集        */
													/*   (SABM-TX,DM-RX)     */
													/*-----------------------*/
	lblap_ablog(s_lblap_p->link_num, LBD_L2_ABRDM);
														/*-------------------*/
														/* Next Status Go !  */
														/* (L1/L2 ACT 状態)  */
														/*-------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 erro*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   UA受信(tm_lb_1_m1e9s2p)
 *              -(State 5)
 *  @note    Received UA(F=0), the frame will be discarded\n
 *           Received UA(F=1), CPRI link will be established
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e9s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*-----------------------*/
												/*    UA  Frame  受信    */
												/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
													/*-----------------------*/
													/*	UA(F=1)??  			 */
													/*-----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*--------------------------*/
													/* YES ( F=1)      			*/
													/* L3-INT-FLAG ON ?      	*/
													/*--------------------------*/
	if(s_lblap_p->lap_dt.l3int_flg == CMD_ON)
	{
													/*--------------------------------------*/
													/* YES (L3-INT-FLAG ON)  				*/
													/*データリンク設定確認送出To CPRI MNG	*/
													/*--------------------------------------*/
//		Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//		if(Result != CMD_RES_OK)
//		{
//			/*error processing*/
//			cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e9s2p", 
//						"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//		}

		sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
		lblap_statis(s_lblap_p->link_num, LBD_L2_ESTCNF);
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, CMD_TSKIF_ESTCNF, (UINT *)&sndbf_p);
	}
	else
	{
														/*-------------------*/
														/* 統計カウント処理  */
														/*-------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_LRST);
	}

												/*---------------------------*/
												/*      状態変数クリア 処理  */
												/*---------------------------*/
	lblap_clrval(s_lblap_p);
													/*-----------------------*/
													/*  異常状態クリア 処理  */
													/*-----------------------*/
	lblap_clrcnd(s_lblap_p);
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
		
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
													/*-----------------------*/
													/*    Next Status Go !   */
													/* (Information Transfer)*/
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_INFSTS;

	/*call MW to set StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
	/*call MW to clear L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T1)受信(tm_lb_1_m1e12s2p)
 *              -(State 5)
 *  @note    SABM 受信フラグ is ON, then CPRI link will be established.\n
 *            -N2回再送終了, then CPRI link establishment will be canceled.\n
 *            -under N2 times T1 time out, SABM will be sended again
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)] 
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 *
 */
void	tm_lb_1_m1e12s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/*****************************/
										/*   T1-EXPIRY    From  OS   */
										/*****************************/
	UINT		Result;                                     /*!< */
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
	INT         errcd;

	if(s_lblap_p->lap_dt.t1_flg != CMD_ON)
	{
		return;
	}
//	lblap_statis(s_lblap_p->link_num, LBD_L2_WAITIO);
													/*-----------------------*/
													/*  SABM 受信 ?          */
													/*-----------------------*/
	if(s_lblap_p->rcv_sbm_flg == CMD_ON)
	{
													/*-----------------------*/
													/* L3-INT-FLAG ON ?      */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.l3int_flg == CMD_ON)
		{
													/*--------------------------------------*/
													/* YES (L3-INT-FLAG ON)  				*/
													/*データリンク設定確認送出To CPRI MNG	*/
													/*--------------------------------------*/
//			Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//			if(Result != CMD_RES_OK)
//			{
//				/*error processing*/
//				cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e12s2p", 
//							"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//			}

			sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
            lblap_statis(s_lblap_p->link_num, LBD_L2_ESTCNF);
			tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, 
								CMD_TSKIF_ESTCNF, (UINT *)&sndbf_p);
								
													/*-----------------------*/
													/*        FRMR Flag OFF  */
													/*-----------------------*/
		}
		else
		{
														/*-------------------*/
														/* 統計カウント処理  */
														/*-------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_LRST);
		}

												/*---------------------------*/
												/*     状態変数クリア 処理   */
												/*---------------------------*/
		lblap_clrval(s_lblap_p);
													/*-----------------------*/
													/*  異常状態クリア 処理  */
													/*-----------------------*/
		lblap_clrcnd(s_lblap_p);
													/* --------------------- */
													/*  再送 Counter Clear  */
													/* --------------------- */
		s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*-----------------------*/
													/*    Next Status Go !   */
													/* (Information Transfer)*/
													/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_INFSTS;
		
		/*call MW to set StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
		/*call MW to clear L2 error*/
	   	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);
		
		return;
	}
	else
	{
													/*-----------------------*/
													/*  N2回再送終了 ?       */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.retry_cnt == s_lblap_p->lap_dt.n2)
		{
														/* 統計カウント処理  */
														/*-------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_LRST);

													/*-----------------------*/
													/*     各 Flag Clear     */
													/*-----------------------*/
			s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*------------------------------*/
													/*   統計カウント処理    		*/
													/* (リンク切断/リセット発生回数)*/
													/*(リンク確立/切断R.O発生数)	*/
													/*------------------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_LRO);
														/*-------------------*/
														/* Next Status Go !  */
														/* (L1/L2 ACT 状態)  */
														/*-------------------*/
			s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

            lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
			lblap_statis(s_lblap_p->link_num, LBD_L2_WAI2T1O);
			/*call MW to clear StateF*/
			tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
			/*call MW to set l2 err*/
	       	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
			tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//			l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//			memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
														/*-------------------*/
			return;
		}	
		else
		{
													/*-----------------------*/
													/* NO   (N2回未終了 )    */
													/*    N2回 Counter INC   */
													/*-----------------------*/
			s_lblap_p->lap_dt.retry_cnt ++;
														/*-------------------*/
														/* SABM SND用 TR HNUT*/
														/*-------------------*/
			Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
			if(Result != CMD_RES_OK)
			{
				/*error processing*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e12s2p", 
							"cm_L2BfGet NG3",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
			}
														/*-------------------*/
														/*   SABM 作成処理   */
														/*-------------------*/
			lblap_mksabm(s_lblap_p);
														/*-------------------*/
														/* U Frame 送出処理  */
														/*-------------------*/
			lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/*    T1 Timer Restart   */
													/*-----------------------*/
			Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
			if(Result != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
			}
													/*-----------------------*/
													/*      T1 Flag ON       */
													/*-----------------------*/
			s_lblap_p->lap_dt.t1_flg = CMD_ON;
													/*--------------------------*/
													/*    統計カウント処理   	*/
													/* (送信完了待ちT.O発生) 	*/
													/* (リンク確立/切断T.O発生) */
													/* (再送フレーム総数)       */
													/* (再送オクテット総数)  	*/
													/*--------------------------*/
			lblap_statis(s_lblap_p->link_num, LBD_L2_STO);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_LTO);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_NTXF);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_NU);
//			lblap_stats1(s_lblap_p->link_num, LBD_L2_NTXO,CMD_NUM4);
			lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_N);
		}
	}
													/*-----------------------*/
													/*    T1 EXPIRY 処理終了 */
													/*-----------------------*/
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   L2 DEACT依頼受信(tm_lb_1_m1e17s2p)
 *              -(State 5)
 *  @note    CPRI link establishment will be canceled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e17s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/*******************************/
										/*  L2 DEACT依頼 from CPRI MNG */
										/*******************************/
	UINT		Result;                                     /*!< */
	INT         errcd;

	
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
 		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
												/*----------------------------------*/
												/*	Change link status to state 4	*/
												/*----------------------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RDE);	
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   データリンク設定要求受信(tm_lb_1_m1e18s2p)
 *              -(State 5)
 *  @note    Set L3 established flag on
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_1_m1e18s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/******************************************/
										/*   データリンク設定要求  From CPRI MNG  */
										/******************************************/

													/*----------------------*/
													/*         Flag Set    	*/
													/*----------------------*/
	s_lblap_p->lap_dt.l3int_flg = CMD_ON;
    tm_lb_setupl2link_rsp(s_lblap_p->link_num, CMD_OK, "OK");
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   L2 STOP依頼受信(tm_lb_1_m1e22s2p)
 *              -(State 5)
 *  @note    CPRI link establishment will be canceled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e22s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/********************************/
										/* L2 STOP依頼 from CPRI MNG	*/
										/********************************/
	UINT		Result;                                     /*!< */
	INT         errcd;

												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 DEACT状態) */
												/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L1ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RSTO);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Layer 1ビジー間隔タイマタイムアウト処理(tm_lb_1_m1e25s2p)
 *              -(State 5)
 *  @note    Layer 1 busy status will be canceled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e25s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UINT		Result;                                     /*!< */
	INT         errcd;
	T_RRH_TIMERTO_NTC *timNtc_p;
	char		work[192];


	if(s_lblap_p->lap_dt.l1bsy_flg != CMD_ON)
	{
		return;
	}
	timNtc_p = (T_RRH_TIMERTO_NTC*)a_inf_p;
	if((timNtc_p->data.id_os != s_lblap_p->l1bsydettmr_osid)
		|| (timNtc_p->data.id  != s_lblap_p->l1bsydettmr_id2))
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Id mismatch key:%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}
	if(timNtc_p->data.userkey != lbw_l1BusyKey[s_lblap_p->link_num])
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Key mismatch key:0x%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}

													/*-----------------------*/
													/*   I Frame Chaning ?   */
													/*-----------------------*/
	if(s_lblap_p->tx_qu.qu_ctl != LBD_L2_TXI)
	{
													/*-----------------------*/
													/* NO (Not I Frame)      */
													/*      Buffer 解放      */
													/*-----------------------*/
		Result = cm_BFree((void *) s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s2p",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->tx_qu.qu_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/* I Frame 廃棄 Flag ON ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.dsc_iq_flg == CMD_ON)
	{

													/*-----------------------*/
													/* YES                   */
													/* バッファ廃棄処理開始  */
													/*-----------------------*/
		while(s_lblap_p->lap_dt.i_head_p != NULL)
		{
													/*-----------------------*/
													/* I Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* I Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s2p",
							"Buffer FreeNG_2", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			s_lblap_p->iframecnt--;
		}
													/*-----------------------*/
													/*     Pointer Null 化   */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = NULL;
		s_lblap_p->lap_dt.i_head2_p = NULL;
		s_lblap_p->lap_dt.i_tail_p = NULL;
													/*-----------------------*/
													/*       Flag Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.dsc_iq_flg = CMD_OFF;
	}

	while (s_lblap_p->lap_dt.s_head_p != NULL)
	{
													/*-----------------------*/
													/* S Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.s_head_p;
													/*-----------------------*/
													/* S Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_head_p =s_lblap_p->lap_dt.s_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s2p",
							"Buffer FreeNG_3", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
		
	}
	s_lblap_p->lap_dt.s_tail_p = NULL;

	while (s_lblap_p->lap_dt.u_head_p != NULL)
	{
													/*-----------------------*/
													/* U Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.u_head_p;
													/*-----------------------*/
													/* U Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_head_p =s_lblap_p->lap_dt.u_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s2p",
							"Buffer FreeNG_4", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}

	}
	s_lblap_p->lap_dt.u_tail_p = NULL;

	s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;

	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
 		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 ACT状態) */
												/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

}

/********************************************************************************************************/

/*!
 *  @brief   SABM受信(tm_lb_1_m1e6s3p)
 *              -(State 6 )
 *  @note    Send DM(F=1) to peer, then release the CPRI link
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e6s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*   SABM 受信  		 */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
													/*-----------------------*/
													/*    L2処理用TR Hunt    */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e6s3p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    DM Frame 作成処理  */
													/*-----------------------*/
	lblap_mkdm(s_lblap_p);
													/*-----------------------*/
													/*    U  Frame 送出処理  */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*--------------------------------------*/
													/* データリンク解放確認送出 To CPRI MNG	*/
													/* (SEND DM, F=P)        				*/
													/*--------------------------------------*/
//	Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//	if(Result != CMD_RES_OK)
//	{
//		/*error processing*/
//		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e6s3p", 
//					"Breq NG1",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//	}

	sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
	tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, CMD_TSKIF_RELCNF, (UINT *)&sndbf_p);
	
													/*-----------------------*/
													/*  T1タイマ停止処理     */
													/*      Flag Clear       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生数)  */
													/*------------------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
													/*-----------------------*/
													/*     ABLOG 収集        */
													/*   (SABM-RX,DM-TX)     */
													/*-----------------------*/
	lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSDM);
													/*-----------------------*/
													/*   Next Status Go !    */
													/*   (L1/L2 ACT 状態)    */
													/*-----------------------*/
	s_lblap_p->lap_dt.status =LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELCNF);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   UA/DM受信(tm_lb_1_m1e8s3p)
 *              -(State 6 )
 *  @note    Release the CPRI link
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e8s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*  UA/DM  Frame  受信   */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
	
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
	
													/*--------------------------------------*/
													/* データリンク解放確認送出 To CPRI MNG	*/
													/*--------------------------------------*/
//	Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//	if(Result != CMD_RES_OK)
//	{
//		/*error processing*/
//		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e8s3p", 
//					"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//	}

	sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
	tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, CMD_TSKIF_RELCNF, (UINT *)&sndbf_p);
													/*-----------------------*/
													/*  T1タイマ停止処理     */
													/*      Flag Clear       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}

													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生数)  */
													/*------------------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);

													/*-----------------------*/
													/*   Next Status Go !    */
													/*   (L1/L2 ACT 状態)    */
													/*-----------------------*/
	s_lblap_p->lap_dt.status =LBD_L2_L2ACTSTS;

	lblap_statis(s_lblap_p->link_num, LBD_L2_RELCNF);

	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T1)受信(tm_lb_1_m1e12s3p)
 *              -(State 6 )
 *  @note    N2回再送終了, then CPRI link will be released\n
 *             -under N2 times T1 time out, DISC will be sended again
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e12s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/****************************************/
										/*   タイムアウト発生通知(T1) From OS	*/
										/****************************************/
	UINT		Result;                                     /*!< */
	INT         errcd;
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
	
	if(s_lblap_p->lap_dt.t1_flg != CMD_ON)
	{
		return;
	}
//	lblap_statis(s_lblap_p->link_num, LBD_L2_WAITIO);
													/*-----------------------*/
													/*  N2回再送終了 ?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.retry_cnt == s_lblap_p->lap_dt.n2)
	{
													/*-----------------------*/
													/*  YES (N2回再送終了)   */
													/*     T1 Flag Clear     */
													/*-----------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*--------------------------------------*/
													/* データリンク解放確認送出 To CPRI MNG	*/
													/*--------------------------------------*/
//		Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//		if(Result != CMD_RES_OK)
//		{
//			/*error processing*/
//			cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e12s3p", 
//						"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//		}

		sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, CMD_TSKIF_RELCNF, (UINT *)&sndbf_p);
		
													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生数)  */
													/* (リンク確立/切断R.O.発生)	*/
													/*------------------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_LRO);
													/*-----------------------*/
													/*   Next Status Go !    */
													/*   (L1/L2 ACT 状態)    */
													/*-----------------------*/
		s_lblap_p->lap_dt.status =LBD_L2_L2ACTSTS;

        lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
		lblap_statis(s_lblap_p->link_num, LBD_L2_RELCNF);
		lblap_statis(s_lblap_p->link_num, LBD_L2_WAI2T1O);
		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	}
	else
	{
													/*-----------------------*/
													/*  NO  (N2回再送未終了) */
													/*    N2回 Counter INC   */
													/*-----------------------*/
		s_lblap_p->lap_dt.retry_cnt ++;
													/*-----------------------*/
													/*    L2処理用TR Hunt    */
													/*-----------------------*/
		Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
		if(Result != CMD_RES_OK)
		{
			/*error processing*/
			cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e12s3p", 
						"cm_L2BfGet NG1",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
		}
													/*-----------------------*/
													/*  DISC Frame 作成処理  */
													/*-----------------------*/
		lblap_mkdisc(s_lblap_p);
													/*-----------------------*/
													/*    U  Frame 送出処理  */
													/*       ( P = 1 )       */
													/*-----------------------*/
		lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/*    T1 Timer Restart   */
													/*-----------------------*/
		Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
		}
													/*-----------------------*/
													/*      T1 Flag ON       */
													/*-----------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_ON;
													/*--------------------------*/
													/*    統計カウント処理   	*/
													/* (送信完了待ちT.O.発生)	*/
													/* (リンク確立/切断R.O.発生)*/
													/* (再送フレーム総数)    	*/
													/* (再送Uフレーム総数)   	*/
													/* (再送オクテット総数)  	*/
													/*--------------------------*/
		lblap_statis(s_lblap_p->link_num, LBD_L2_STO);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_LTO);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_NTXF);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_NU);
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_NTXO,CMD_NUM4);
	}
													/*-----------------------*/
													/* T1-EXPIRY 処理終了    */
													/*-----------------------*/
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   L2 DEACT依頼受信(tm_lb_1_m1e17s3p)
 *              -(State 6 )
 *  @note    CPRI link will be released
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e17s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
										/********************************/
										/*  L2 DEACT依頼  from CPRI MNG	*/
										/********************************/
	UINT		Result;                                     /*!< */
	INT         errcd;

	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
 		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
												/*----------------------------------*/
												/* Change link status to state 4	*/
												/*----------------------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RDE);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}
/********************************************************************************************************/

/*!
 *  @brief   L2 STOP依頼受信(tm_lb_1_m1e22s3p)
 *              -(State 6 )
 *  @note    CPRI link will be released
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e22s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/********************************/
												/*   L2 STOP依頼  From CPRI MNG	*/
												/*   state 6		*/
												/********************************/
	UINT		Result;                                     /*!< */
	INT         errcd;
	
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
												/*---------------------------*/
												/*       T1タイマ停止処理    */
												/*---------------------------*/
		Result = cm_TStop(s_lblap_p->t1_id, &errcd);
			
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
 		}
												/*---------------------------*/
												/*         Flag Clear        */
												/*---------------------------*/
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 DEACT状態) */
												/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L1ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_N);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RSTO);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);	
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Layer 1 Data CNF受信(tm_lb_1_m1e0s4p)
 *              -(State 7)
 *  @note    下位モジュールよりLayer 1 Data CNF を受信した後の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e0s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
											/**************************************/
											/*   Layer 1 Data CNF  From POLLING   */
											/**************************************/

	lblap_conf2(s_lblap_p);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   CPRI信号送信通知受信(tm_lb_1_m1e1s4p)
 *              -(State 7)
 *  @note    フレーム送信中/ 相手局ビジーでない場合、I Frame を送出する。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e1s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/******************************************/
												/*   CPRI信号送信通知  From Layer 3 tasks */
												/******************************************/

													/*-----------------------*/
													/*  受信 Buffer Adr Get  */
													/*-----------------------*/
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiDstTQueueID = CMD_TSKID_TXS;
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiSrcTQueueID = CMD_TSKID_LPB;

	s_lblap_p->tr_adr.lap_buf_p = (LBT_TSKIF_CPRICOM *)a_inf_p;
													/*-----------------------*/
													/*  受信 BUFFER ADR Save */
													/*-----------------------*/
	s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->tr_adr.lap_buf_p;
	s_lblap_p->buf_adr.lap_buf_p->i_link = NULL;



													/*-----------------------*/
													/* 処理 Continue         */
													/*  I-Head Pointer NULL ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.i_head_p == NULL)
	{
													/*-----------------------*/
													/*  YES (Pointer NULL)   */
													/*    Pointer Queuing    */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_tail_p = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_head2_p = s_lblap_p->buf_adr.lap_buf_p;
	}
	else
	{
													/*-----------------------*/
													/*  NO (Buffer Queuing)  */
													/* I-Link Pointer Queuing*/
													/*-----------------------*/
		s_lblap_p->lap_dt.i_tail_p->i_link = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_tail_p = s_lblap_p->buf_adr.lap_buf_p;
													/*-----------------------*/
													/* I2-Head Pointer NULL ?*/
													/*-----------------------*/
		if(s_lblap_p->lap_dt.i_head2_p == NULL)
		{
													/*-----------------------*/
													/*  YES (Pointer NULL)   */
													/*    Pointer Queuing    */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head2_p = s_lblap_p->buf_adr.lap_buf_p;
		}

	}
	
													/*-----------------------*/
													/* I Frame 送出処理      */
													/*-----------------------*/
	lblap_sndi(s_lblap_p);
													
	s_lblap_p->iframecnt++;
	if(s_lblap_p->iframecnt > LBD_RUNHIS_IFRAME_MAX)
	{
		tm_lb_subrunhis(s_lblap_p);
	}
	lbw_keepbufflg = CMD_ON;
	return;

}

/********************************************************************************************************/

/*!
 *  @brief   I  Frame受信(tm_lb_1_m1e2s4p)
 *              -(State 7)
 *  @note    CPRI linkよりI フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e2s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*     I  Frame  受信    */
													/*-----------------------*/
	UINT     tmpflg;                                        /*!< */
	UINT		Result;                                     /*!< */
	INT         errcd;
	UINT		no1;
	UINT		no2;

	

													/*-----------------------*/
													/*   Save 状態変数       */
													/*-----------------------*/
	s_lblap_p->vs_sav = s_lblap_p->lap_dt.vs;
	s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ  */
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);
												/*-----------------------*/
												/*    Next Status Go !   */
												/*  (L1 ACT･L2 ACT状態 ) */
												/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}

													/*-----------------------*/
													/* 自局 ビジー ?         */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/* NO (Not  ビジー)      */
													/* NS = VR ?             */
													/*-----------------------*/
		if(s_lblap_p->ns_sav == s_lblap_p->lap_dt.vr)
		{
													/*-----------------------*/
													/* YES (NS = VR)         */
													/*    V(R) INC 処理      */
													/*-----------------------*/
			lblap_vrinc(s_lblap_p);
													/*-----------------------*/
													/* Reject 状態 Flag clear*/
													/*-----------------------*/
			s_lblap_p->lap_dt.rej_cond = CMD_OFF;
            /*	REJ送信した時のNS番号をクリアする	*/
            s_lblap_p->nsRejSnd = 0xFFFFFFFF;
													/*-----------------------*/
													/* L2-DATA-IND 処理      */
													/*-----------------------*/
			lblap_l2datpr(s_lblap_p);
													/*-----------------------*/
													/*    P = 1 ?            */
													/*-----------------------*/
			if(s_lblap_p->pf_sav == CMD_NUM1)
			{
													/*-----------------------*/
													/*  YES   (P = 1)        */
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
				Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
								(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
				if(Result != CMD_RES_OK)
				{
					/*error processing*/
					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s4p", 
								"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
				}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
				s_lblap_p->cmd_rsp_wk = CMD_NUM0;
				s_lblap_p->pf_wk = CMD_NUM1;
													/*-----------------------*/
													/*    RR  作成処理       */
													/*      ( F = 1 )        */
													/*-----------------------*/
				lblap_mkrr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
				lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
				lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/*       VR  Save        */
													/*-----------------------*/
				s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
//				lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF1);
			}
			else
			{
													/*-----------------------*/
													/*   NO   (P != 1)       */
													/*       VR Save         */
													/*-----------------------*/
				s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;

				if(s_lblap_p->lap_dt.t2_cnt == CMD_NUM0)
				{
					Result = cm_TStat(s_lblap_p->t2_id, 
									s_lblap_p->lap_dt.t2, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
					if(Result != CMD_RES_OK)
					{
						cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t2_id, "T2 Timer Start NG" );
					}
													/*-----------------------*/
													/*   T2 Counter INC      */
													/*-----------------------*/
					s_lblap_p->lap_dt.t2_cnt ++;
				}
				else
				{
					if(s_lblap_p->lap_dt.t2_cnt == s_lblap_p->lap_dt.peerk)
					{
 													/*-----------------------*/
													/*  YES (T2 CNT = PEER K)*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
						Result  = cm_TStop(s_lblap_p->t2_id, &errcd);
						if(Result	!= CMD_RES_OK)
						{
							cm_Assert( CMD_ASL_DBGLOW, Result, "T2 Timer Stop NG" );
						}
													/*-----------------------*/
													/* T2 Counter Clear      */
													/*-----------------------*/
						s_lblap_p->lap_dt.t2_cnt = CMD_NUM0;

						tmpflg = CMD_NG;
													/*-----------------------*/
													/* I Frame Pointer 有り? */
													/*-----------------------*/
						if(s_lblap_p->lap_dt.i_head2_p  != NULL)
		 				{
													/*-----------------------*/
													/*  YES                  */
													/* L1 BUSY FLAG OFF ?    */
													/*-----------------------*/
							if(s_lblap_p->lap_dt.l1bsy_flg == CMD_OFF)
							{
													/*-----------------------*/
													/*  YES (FLAG OFF)       */
													/* 相手局ビジー ?        */
													/*-----------------------*/
								if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
								{
													/*-----------------------*/
													/*  NO  (Not Busy)       */
													/*V(S)=V(A)+K判定処理    */
													/*-----------------------*/
									lblap_vsvak(s_lblap_p);
													/*-----------------------*/
													/* 判定結果 0 ?          */
													/*-----------------------*/
									if(s_lblap_p->cond == CMD_NUM0)
									{
										tmpflg = CMD_OK;
									}
								}
							}	
						}	

						if(tmpflg == CMD_NG)
						{
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
							Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
												(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
							if(Result != CMD_RES_OK)
							{
									/*error processing*/
								cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s4p", 
											"cm_L2BfGet NG1",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
							}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
							s_lblap_p->cmd_rsp_wk = CMD_NUM0;
							s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/*    RR  作成処理       */
													/*-----------------------*/
							lblap_mkrr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
							lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*    ST7_178の処理参照  */
													/*-----------------------*/
//                            lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF0);
						}
					}
					else
					{
						s_lblap_p->lap_dt.t2_cnt ++;
					}
				}
 			}
		}
		else
		{
													/*-----------------------*/
													/* NO  (NS != VR)        */
													/* 統計カウント処理      */
													/*  (無効 Ns受信回数)    */
													/*-----------------------*/
			lblap_statis(s_lblap_p->link_num, LBD_L2_INS);
													/*-----------------------*/
													/*  REJECT 状態フラグ 1 ?*/
													/*-----------------------*/
			if(s_lblap_p->lap_dt.rej_cond == CMD_ON)
			{
				if(s_lblap_p->nsRejSnd == 0xFFFFFFFF)
				{
					no1 = 0;
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->nsRejSnd, (UCHAR*)"REJ SEND NG" );
				}
				else
				{
					no1 = (s_lblap_p->nsRejSnd - s_lblap_p->lap_dt.vr)%8;
				}
				no2 = (s_lblap_p->ns_sav - s_lblap_p->lap_dt.vr)%8;
				
				/* 1回目REJ送信後かつ再送も受信抜けした場合*/
				if((s_lblap_p->lap_dt.rej_cond == CMD_ON) && (no2 <= no1))
				{
					/*	L2統計取得	*/
                    lblap_statis(s_lblap_p->link_num, LBD_L2_NS2END);
				}
				else
				{
    													/*-----------------------*/
    													/*  YES (REJECTフラグ 1) */
    													/* P = 1 ?               */
    													/*-----------------------*/
    				if(s_lblap_p->pf_sav == CMD_NUM1)
    				{
    													/*-----------------------*/
    													/*  YES (  P = 1  )      */
    													/*   L2 用 TR HNUT       */
    													/*-----------------------*/
    					Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
    									(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
    					if(Result != CMD_RES_OK)
    					{
    						/*error processing*/
    						cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s4p", 
    									"cm_L2BfGet NG2",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
    					}
    													/*-----------------------*/
    													/*  各 Flag Work Clear   */
    													/*-----------------------*/
    					s_lblap_p->cmd_rsp_wk = CMD_NUM0;
    					s_lblap_p->pf_wk = CMD_NUM1;
    													/*-----------------------*/
    													/*    RR  作成処理       */
    													/*-----------------------*/
    					lblap_mkrr(s_lblap_p);
    													/*-----------------------*/
    													/* S Frame 送出処理      */
    													/*-----------------------*/
    					lblap_snds(s_lblap_p);
    													/*-----------------------*/
    													/*  T2タイマ停止処理     */
    													/*-----------------------*/
    					lblap_stopt2(s_lblap_p);
    				}
                }
			}
			else
			{
                s_lblap_p->nsRejSnd = s_lblap_p->vr_sav;
													/*-----------------------*/
													/*  NO  (REJECTフラグ 0) */
													/*  Reject Condition Set */
													/*-----------------------*/
				s_lblap_p->lap_dt.rej_cond = CMD_ON;
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
				Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
								(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
				if(Result != CMD_RES_OK)
				{
					/*error processing*/
					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s4p", 
								"cm_L2BfGet NG3",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
				}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
				s_lblap_p->cmd_rsp_wk = CMD_NUM0;
				s_lblap_p->pf_wk = s_lblap_p->pf_sav;
				
													/*-----------------------*/
													/*    REJ 作成処理       */
													/*-----------------------*/
				lblap_mkrej(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
				lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
				lblap_stopt2(s_lblap_p);
			}
		}
	}
	else
	{
													/*-----------------------*/
													/* YES (自局ビジー)      */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
			Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
							(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
			if(Result != CMD_RES_OK)
			{
				/*error processing*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s4p", 
							"cm_L2BfGet NG4",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
			}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
			s_lblap_p->cmd_rsp_wk = CMD_NUM0;
			s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    RNR 作成処理       */
													/*-----------------------*/
			lblap_mkrnr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
			lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
			lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (廃棄フレーム総数)    */
													/*-----------------------*/
			lblap_statis(s_lblap_p->link_num, LBD_L2_DSC_I);
			lblap_statis(s_lblap_p->link_num, LBD_L2_BIDSC);										
		}
	}
													/*-----------------------*/
													/* YES   (結果 1)        */
													/* 相手局ビジー ?        */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
	{
		tm_lb_checknr(s_lblap_p);
	}
													/*-----------------------*/
													/* YES ( PEER BUSY )     */
													/* V(A) = V(R) 処理1     */
													/*-----------------------*/
	lblap_vanr1(s_lblap_p);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   RR Frame受信(tm_lb_1_m1e3s4p)
 *              -(State 7)
 *  @note    CPRI linkよりRR フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 *
 */
void	tm_lb_1_m1e3s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    RR  Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ  */
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 ACT状態 )  */
												/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}
													/*-----------------------*/
													/*  相手局ビジー ?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy == CMD_ON)
	{
													/*-----------------------*/
													/* YES (相手局ビジー)    */
													/*   Peer Busy Flag OFF  */
													/*-----------------------*/
		s_lblap_p->lap_dt.per_bsy = CMD_OFF;
		lblap_statis(s_lblap_p->link_num, LBD_L2_OBCRRR);
	}

	if(s_lblap_p->lap_dt.t3_flg == CMD_ON)
	{
													/*-----------------------*/
													/*  Stop T3       */
													/*-----------------------*/
		Result =	cm_TStop(s_lblap_p->t3_id, &errcd);
													/*-----------------------*/
													/*  error processing     */
													/*-----------------------*/
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T3 Timer Stop NG" );
		}
													/*--------------------------*/
													/*  Clear T3 flag       	*/
													/*--------------------------*/
		s_lblap_p->lap_dt.t3_flg = CMD_OFF;
	}

	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
		if(s_lblap_p->lap_dt.t4_flg == CMD_ON)
		{
													/*----------------------*/
													/*  Stop T4       		*/
													/*----------------------*/
			Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
													/*----------------------*/
													/*  error processing    */
													/*----------------------*/
			if(Result	!= CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
			}
													/*-----------------------*/
													/*  Clear T4 flag        */
													/*-----------------------*/
			s_lblap_p->lap_dt.t4_flg = CMD_OFF;
		}
	}
													/*-----------------------*/
													/*      COMMAND ?        */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES(Command)          */
													/*      P = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
														/*-----------------------*/
														/* YES ( P = 1 )         */
														/* Response 転送処理     */
														/*-----------------------*/
			lblap_transf(s_lblap_p);
		}
	}
	else
	{
													/*-----------------------*/
													/* NO(Response)          */
													/*      F = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/* ----------------------- */
													/*  YES                   */
													/*       F = 1            */
													/* ----------------------- */
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
/*         modify for F-968(DCM)[054-025(DCM)]        */
/*			s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;   */
		}	
	}

	tm_lb_checknr(s_lblap_p);
													/*-----------------------*/
													/* V(A) = V(R) 処理1     */
													/*-----------------------*/
	lblap_vanr1(s_lblap_p);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   RNR Frame受信(tm_lb_1_m1e4s4p)
 *              -(State 7)
 *  @note    CPRI linkよりRNR フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e4s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    RNR Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ  */
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);
												/*-----------------------*/
												/*    Next Status Go !   */
												/*  (L1 ACT･L2 ACT状態 ) */
												/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}
													/*-----------------------*/
													/*  PEER NOT BUSY?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/* YES (相手局ビジー無し)*/
													/*   Peer Busy Flag SET  */
													/*-----------------------*/
		s_lblap_p->lap_dt.per_bsy = CMD_ON;
	}
	else
	{
	    lblap_statis(s_lblap_p->link_num, LBD_L2_OBRRNR);
	}
	
	if(s_lblap_p->lap_dt.t3_flg == CMD_OFF)
	{
													/*----------------------*/
													/*	Start T3      		*/
													/*----------------------*/
		Result = cm_TStat(s_lblap_p->t3_id, s_lblap_p->lap_dt.t3, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
													/*----------------------*/
													/*Error processing    	*/
													/*----------------------*/
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t3_id, "T3 Timer Start NG" );
		}
													/*----------------------*/
													/*	Set T3 flag ON    	*/
													/*----------------------*/
		s_lblap_p->lap_dt.t3_flg = CMD_ON;
	}

													/*----------------------*/
													/*	Own is not busy?    */
													/*----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*----------------------*/
													/*Yes(Own is not busy)  */
													/*	T4 is not running?  */
													/*----------------------*/
		if(s_lblap_p->lap_dt.t4_flg == CMD_OFF)
		{
													/*--------------------------*/
													/*	Yes(T4 is not running)  */
													/*	Start T4      			*/
													/*--------------------------*/
			Result = cm_TStat(s_lblap_p->t4_id, s_lblap_p->lap_dt.t4, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
													/*--------------------------*/
													/*	Error processing    	*/
													/*--------------------------*/
			if(Result != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t4_id, "T4 Timer Start NG" );
			}
													/*----------------------*/
													/*	Set T4 flag ON    	*/
													/*----------------------*/
			s_lblap_p->lap_dt.t4_flg = CMD_ON;
		}
	}
													/*----------------------*/
													/*      Command ?       */
													/*----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES(Command)          */
													/*      P = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( P = 1 )         */
													/* Response 転送処理     */
													/*-----------------------*/
			lblap_transf(s_lblap_p);
		}
	}
	else
	{
													/*-----------------------*/
													/* NO(Response)          */
													/*      F = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/* ---------------------- */
													/*  YES                  */
													/*       F = 1          	*/
													/* ---------------------- */
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
/*         modify for F-968(DCM)[054-025(DCM)]        */
/*			s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;	  */
		}
	}

	tm_lb_checknr(s_lblap_p);
													/*-----------------------*/
													/* V(A) = V(R) 処理1     */
													/*-----------------------*/
	lblap_vanr1(s_lblap_p);
	return;


}

/********************************************************************************************************/

/*!
 *  @brief   REJ Frame受信(tm_lb_1_m1e5s4p)
 *              -(State 7)
 *  @note    CPRI linkよりREJ フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e5s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    REJ Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ	*/
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);

		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}
													/*-----------------------*/
													/*  相手局ビジー ?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy == CMD_ON)
	{
													/*-----------------------*/
													/* YES (相手局ビジー)    */
													/*   Peer Busy Flag OFF  */
													/*-----------------------*/
		s_lblap_p->lap_dt.per_bsy = CMD_OFF;
		lblap_statis(s_lblap_p->link_num, LBD_L2_OBCRRR);
	}
	
													/*----------------------*/
													/*  T3 is running?     	*/
													/*----------------------*/
	if(s_lblap_p->lap_dt.t3_flg == CMD_ON)
	{
													/*----------------------*/
													/*  Yes(T3 is running)  */
													/*  Stop T3       		*/
													/*----------------------*/
		Result =	cm_TStop(s_lblap_p->t3_id, &errcd);
													/*-----------------------*/
													/*  Error processing     */
													/*-----------------------*/
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T3 Timer Stop NG" );
		}
													/*----------------------*/
													/*  Clear T3 flag    	*/
													/*----------------------*/
		s_lblap_p->lap_dt.t3_flg = CMD_OFF;
	}

													/*-----------------------*/
													/*  Own is not busy?     */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*--------------------------*/
													/*  Yes(Own is not busy)    */
													/*  T4 is running?     		*/
													/*--------------------------*/
		if(s_lblap_p->lap_dt.t4_flg == CMD_ON)
		{
													/*--------------------------*/
													/*  Yes(T4 is running)     	*/
													/*  Stop T4    				*/
													/*--------------------------*/
			Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
													/*-----------------------*/
													/*  Error processing     */
													/*-----------------------*/
			if(Result	!= CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
			}
													/*----------------------*/
													/*  Clear T4 flag     	*/
													/*----------------------*/
			s_lblap_p->lap_dt.t4_flg = CMD_OFF;
		}
	}

													/*----------------------*/
													/*      Command?        */
													/*----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES(Command)          */
													/*      P = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( P = 1 )         */
													/* Response 転送処理     */
													/*-----------------------*/
			lblap_transf(s_lblap_p);
		}
	}

													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/* ----------------------- */
													/*  フレーム再送処理      */
													/* ----------------------- */
	lblap_retrs(s_lblap_p);
	return;


}

/********************************************************************************************************/
/*!
 *  @brief   SABM Frame受信(tm_lb_1_m1e6s4p)
 *              -(State 7)
 *  @note    CPRI linkよりSABM フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e6s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*----------------------*/
													/*  SABM    受信 		*/
													/*----------------------*/
	UINT		Result;                                    

													/*----------------------*/
													/*   P != 1?   			*/
													/*----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
													/*----------------------*/
													/*   Yes   				*/
													/*----------------------*/
		return;
	}
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*  All タイマ停止処理  */
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/* --------------------- */
													/*   No(P=1)   			*/
													/*    L2処理用TR Hunt   */
													/*----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e6s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    UA Frame 作成処理  */
													/*-----------------------*/
	lblap_mkua(s_lblap_p);
													/*-----------------------*/
													/*    U  Frame 送出処理  */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/* 状態変数クリア 処理   */
													/*-----------------------*/
	lblap_clrval(s_lblap_p);
													/*-----------------------*/
													/*  異常状態クリア 処理  */
													/*-----------------------*/
	lblap_clrcnd(s_lblap_p);


													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生数)  */
													/* (リンク リセット発生回数  )  */
													/*------------------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
//	lblap_statis(s_lblap_p->link_num, LBD_L2_LRST);

	s_lblap_p->lap_dt.status = LBD_L2_INFSTS;

	lblap_statis(s_lblap_p->link_num, LBD_L2_ESTIND_U);
	
	/*call MW to set StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
	/*call MW to clear L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   DISC Frame受信(tm_lb_1_m1e7s4p)
 *              -(State 7)
 *  @note    CPRI linkよりDISC フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e7s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*   DISC Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                   

	
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*--------------------------*/
													/*I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*  All タイマ停止処理   */
													/*-----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*--------------------------*/
													/*    P=1 continue  		*/
													/*    L2処理用TR Hunt    	*/
													/*--------------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e7s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      P/F Bit セーブ   */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    UA Frame 作成処理  */
													/*-----------------------*/
	lblap_mkua(s_lblap_p);
													/*-----------------------*/
													/*    U  Frame 送出処理  */
													/*       ( F = P )       */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);

													/*------------------------------*/
													/*    統計カウント処理   		*/
													/* (リンク切断/リセット発生数)  */
													/*------------------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RST);
													/*-----------------------*/
													/*   Next Status Go !    */
													/*   (L1/L2 ACT 状態)    */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);	
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   DM Frame受信(tm_lb_1_m1e8s4p)
 *              -(State 7)
 *  @note    CPRI linkよりDM フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e8s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    DM  Frame  受信    */
													/*-----------------------*/
													/*-----------------------*/
													/*  受信バッファ FREE    */
													/*-----------------------*/
	if(s_lblap_p->pf_sav == CMD_NUM1)
	{
													/*--------------------------*/
													/*I フレーム 強制廃棄処理	*/
													/*--------------------------*/
		lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*    All timer stop     */
													/*-----------------------*/
		tm_lb_StpAllTmr(s_lblap_p);

													/*-----------------------*/
													/*    Next Status Go !   */
													/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

        lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set L2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);		
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
		return;
	}
	else
	{
													/*--------------------------*/
													/*I フレーム 強制廃棄処理	*/
													/*--------------------------*/
		lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*    All timer stop   	*/
													/*----------------------*/
		tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/* 処理 Continue         */
													/*    << 通常モード >>   */
													/* データ リンク 確立処理*/
													/*-----------------------*/
		lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*  Flag Off 処理        */
													/*-----------------------*/
		s_lblap_p->lap_dt.l3int_flg = CMD_OFF;
		s_lblap_p->rcv_sbm_flg = CMD_OFF;
		s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;
        lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_U);
	}
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   UA Frame受信(tm_lb_1_m1e9s4p)
 *              -(State 7)
 *  @note    CPRI linkよりUA フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e9s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    UA  Frame  受信    */
													/*-----------------------*/
//	UINT		Result;                                     /*!< */
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
													/*-----------------------*/
													/*   受信バッファ FREE   */
													/*-----------------------*/
	if(s_lblap_p->pf_sav != CMD_NUM1)
	{
		return;
	}
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
	lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*    All timer stop     */
													/*-----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/* データ リンク 確立処理*/
													/*-----------------------*/
	lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*  Flag Off 処理        */
													/*-----------------------*/
	s_lblap_p->lap_dt.l3int_flg = CMD_OFF;
	s_lblap_p->rcv_sbm_flg = CMD_OFF;
													/*------------------------------*/
													/*Send データリンク設定中表示	*/
													/*  To CPRI MNG task     		*/
													/*------------------------------*/
//	Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//	if(Result != CMD_RES_OK)
//	{
//		/*error processing*/
//		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e9s4p", 
//					"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//	}
	sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
	tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, 
								CMD_TSKIF_ESTINGIND, (UINT *)&sndbf_p);
								
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;
	lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_U);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   FRMR Frame受信(tm_lb_1_m1e10s4p)
 *              -(State 7)
 *  @note    CPRI linkよりFRMR フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e10s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*  FRMR  Frame  受信    */
													/*-----------------------*/
													/*-----------------------*/
													/*  RCV Buf Disp操作     */
													/*-----------------------*/
	s_lblap_p->buf_adr.disp += CMD_NUM4;
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*    All timer stop   	*/
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (L1 ACT･L2 ACT状態 )*/
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   ERROR Frame受信(tm_lb_1_m1e11s4p)
 *              -(State 7)
 *  @note    CPRI linkよりERROR フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e11s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*************************/
													/*     ERROR  受信       */
													/*    ( FRMR ERROR )     */
													/*************************/
													/*-----------------------*/
													/*       VR,VS SET       */
													/*-----------------------*/
	s_lblap_p->vs_sav = s_lblap_p->lap_dt.vs;
	s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
													/*-----------------------*/
													/*  FRMR ERROR 処理      */
													/*-----------------------*/
	lblap_frmrer(s_lblap_p);
													/*--------------------------*/
													/* I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/* 	All Timer stop	    */
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*--------------------------*/
													/*    Next Status Go !   	*/
													/*   (L1 ACT･L2 ACT状態 )  	*/
													/*--------------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T1)受信(tm_lb_1_m1e12s4p)
 *              -(State 7)
 *  @note    (コマンドを送出後、T1 Timer(相手局のAck 待ちタイマ) Timeout したとき)コマンドを送出する
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e12s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*****************************/
												/*   T1-EXPIRY    From  OS   */
												/*****************************/

	if(s_lblap_p->lap_dt.t1_flg != CMD_ON)
	{
		return;
	}
													/*-----------------------*/
													/*   再送Counter Clear   */
													/*-----------------------*/
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*----------------------*/
													/*   T1 flag Clear   	*/
													/*----------------------*/
	s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*-----------------------*/
													/*   Command  転送処理   */
													/*-----------------------*/
	lblap_transp(s_lblap_p);
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (送信完了待ちT.O発生) */
													/* (情報転送中T.O発生)   */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_STO);
	lblap_statis(s_lblap_p->link_num, LBD_L2_ITO);
													/*-----------------------*/
													/*    Next Status Go !   */
													/*( Waiting Acknowledge )*/
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ACKSTS;

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T2)受信(tm_lb_1_m1e13s4p)
 *              -(State 7)
 *  @note    応答用の確認信号を送信する
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e13s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/************************************/
												/*   T2-EXPIRY    From  OS   		*/
												/*   common processing between      */
												/*   state 7 and state 8  			*/
												/************************************/
	UINT		Result;                                     /*!< */
	
	if(s_lblap_p->lap_dt.t2_cnt == CMD_NUM0)
	{
		return;
	}
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_buf_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e13s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
	s_lblap_p->cmd_rsp_wk = CMD_NUM0;
	s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/* OWN BUSY ?            */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/* NO (Not OWN BUSY)     */
													/*     RR   作成処理     */
													/*-----------------------*/
		lblap_mkrr(s_lblap_p);
	}
	else
	{

													/*-----------------------*/
													/* YES  (OWN BUSY)       */
													/*     RNR  作成処理     */
													/*-----------------------*/
		lblap_mkrnr(s_lblap_p);
	}
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
	lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*      Counter Clear    */
													/*-----------------------*/
	s_lblap_p->lap_dt.t2_cnt = CMD_NUM0;
													/*-----------------------*/
													/*  T2-EXPIRY 処理終了   */
													/*-----------------------*/
//    lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF0);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T3)受信(tm_lb_1_m1e14s4p)
 *              -(State 7)
 *  @note    相手局がビジー状態に遷移した場合に問い合わせを行う
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e14s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*****************************/
												/*   T3-EXPIRY    From  OS   */
												/*****************************/
	if(s_lblap_p->lap_dt.t3_flg != CMD_ON)
	{
		return;
	}
													/*-----------------------*/
													/*      T3 Flag OFF      */
													/*-----------------------*/
	s_lblap_p->lap_dt.t3_flg = CMD_OFF;
													/*-----------------------*/
													/*   再送Counter Clear   */
													/*-----------------------*/
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-----------------------*/
													/*   Command  転送処理   */
													/*-----------------------*/
	lblap_transp(s_lblap_p);
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (情報転送中T.O発生)   */
													/* (情報転送中T3 T.O発生)*/
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_ITO);
	lblap_statis(s_lblap_p->link_num, LBD_L2_TO3);
													/*-----------------------*/
													/*    Next Status Go !   */
													/*( Waiting Acknowledge )*/
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ACKSTS;

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T4)受信(tm_lb_1_m1e15s4p)
 *              -(State 7)
 *  @note    自局または相手局がビジー状態である、または、アイドル状態である場合にレイヤ2の初期化を行い、リンクの再設定を行う
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e15s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/********************************/
												/*   T4-EXPIRY    From  OS   	*/
												/*   common processing between  */
												/*   state 7 and state 8  		*/
												/********************************/
	UINT		Result;                                     /*!< */
	
	if(s_lblap_p->lap_dt.t4_flg != CMD_ON)
	{
		return;
	}
													/*-----------------------*/
													/*      T4 Flag OFF      */
													/*-----------------------*/
	s_lblap_p->lap_dt.t4_flg = CMD_OFF;
													/*--------------------------*/
													/*I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*    All timer stop   	*/
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*----------------------------------*/
													/*  Hunt buffer for send DM(F=0)	*/
													/*----------------------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e15s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
	s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/*    DM  作成処理       */
													/*      ( F = 0 )        */
													/*-----------------------*/
	lblap_mkdm(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
												/*--------------------------*/
													/*    Next Status Go !   	*/
													/*   (L1 ACT･L2 ACT状態 )  	*/
													/*--------------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	lblap_statis(s_lblap_p->link_num, LBD_L2_T4O);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(L2 busy)受信(tm_lb_1_m1e16s4p)
 *              -(State 7)
 *  @note    In self busy state, check self busy cancel or not 
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e16s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*   L2 busy timer time out  From OS	*/
												/****************************************/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*----------------------*/
													/*    Own is not busy?  */
													/*----------------------*/
	if(s_lblap_p->lap_dt.own_bsy != CMD_ON)
	{
													/*-----------------------------*/
													/*   Stop layer 2  busy timer  */
													/*-----------------------------*/
		Result = cm_TStop(s_lblap_p->l2bsydettmr_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "L2 busy Timer Stop NG" );
		}
													/*---------------------------------*/
													/* Clear layer 2  busy timer flag  */
													/*---------------------------------*/
		s_lblap_p->lap_dt.l2bsytmrflg = CMD_OFF;
		return;
	}
													/*-----------------------------------*/
													/*All type of buffer busy is cancel? */
													/*-----------------------------------*/
	if( (s_lblap_p->lap_dt.buftyp1_bsyflg != CMD_OFF)
		|| (s_lblap_p->lap_dt.buftyp2_bsyflg != CMD_OFF)
		|| (s_lblap_p->lap_dt.buftyp3_bsyflg != CMD_OFF)
		|| (s_lblap_p->lap_dt.buftyp4_bsyflg != CMD_OFF))
	{
													/*------------------------------*/
													/*No(Some buffer is still busy) */
													/*------------------------------*/
		return;
	}
													/*-----------------------*/
													/*     自局 BUSY Clear   */
													/*-----------------------*/
	s_lblap_p->lap_dt.own_bsy = CMD_OFF;
													/*-----------------------------*/
													/*   Stop layer 2  busy timer  */
													/*-----------------------------*/
	Result = cm_TStop(s_lblap_p->l2bsydettmr_id, &errcd);
	if(Result	!= CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, Result, "L2 busy Timer Stop NG" );
	}
													/*---------------------------------*/
													/* Clear layer 2  busy timer flag  */
													/*---------------------------------*/
	s_lblap_p->lap_dt.l2bsytmrflg = CMD_OFF;
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e16s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
	s_lblap_p->cmd_rsp_wk = CMD_NUM0;
	s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/* YES (I Frame FLAG OFF)*/
													/*    RR  作成処理       */
													/*     ( F = 1 )         */
													/*-----------------------*/
	lblap_mkrr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
	lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
	lblap_stopt2(s_lblap_p);
													/*----------------------*/
													/*  Peer is not busy?   */
													/*----------------------*/
//    lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF0);
	lblap_statis(s_lblap_p->link_num, LBD_L2_BCSRR);
	if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
	{
													/*----------------------*/
													/*  Yes(Not busy)     	*/
													/*  Stop T4     		*/
													/*----------------------*/
		if(s_lblap_p->lap_dt.t4_flg == CMD_ON)
		{
													/*--------------------------*/
													/*stop T4 and set flag off	*/
													/*--------------------------*/
			Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
			if(Result	!= CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
			}
			s_lblap_p->lap_dt.t4_flg = CMD_OFF;
		}
	}
	return;
}

/********************************************************************************************************/

/*!
 *  @brief    L2 DEACT依頼受信(tm_lb_1_m1e17s4p)
 *              -(State 7, State 8 common function)
 *  @note    Release the CPRI link
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e17s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/************************************/
												/*   L2 DEACT依頼  From CPRI MNG	*/
												/*   common processing between      */
												/*   state 7 and state 8  			*/
												/************************************/
													/*--------------------------*/
													/*I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*    All timer stop   	*/
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/*   Next Status Go !    */
													/*   (L1/L2 ACT 状態)    */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RDE);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   データリンク設定要求受信(tm_lb_1_m1e18s4p)
 *              -(State 7, State 8 common function)
 *  @note    The CPRI link	will be reestablished
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_1_m1e18s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/********************************************/
												/*   データリンク設定要求  From CPRI MNG    */
												/*   common processing between      		*/
												/*   state 7 and state 8  			  		*/
												/********************************************/
													/*--------------------------*/
													/* I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*----------------------*/
													/*    All timer stop   	*/
													/*----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*--------------------------*/
													/* EST-RQ 処理開始       	*/
													/*   データ リンク 確立処理	*/
													/*--------------------------*/
	lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*   Flag Clear/Set      */
													/*-----------------------*/
	s_lblap_p->lap_dt.l3int_flg = CMD_ON;
	s_lblap_p->rcv_sbm_flg = CMD_OFF;

													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;
    lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_U);
	tm_lb_setupl2link_rsp(s_lblap_p->link_num, CMD_OK, "OK");
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   データリンク解放要求受信(tm_lb_1_m1e19s4p)
 *              -(State 7, State 8 common function)
 *  @note    Send DISC(P=1) and start T1, change link state to state 6
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @date    2015.11.13 TDI)satou その他未実装-013 LBD_STT_L2SETUPへの遷移を追加
 */
void	tm_lb_1_m1e19s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/********************************************/
												/*   データリンク解放要求  From CPRI MNG  	*/
												/*   common processing between				*/
												/*   state 7 and state 8  			  		*/
												/********************************************/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*--------------------------*/
													/* I フレーム 強制廃棄処理	*/
													/*--------------------------*/
	lblap_disciq(s_lblap_p);
													/*--------------------------*/
													/*    All timer stop   		*/
													/*--------------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/*    L2機能用TR Hunt    */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
	    tm_lb_rell2link_rsp(s_lblap_p->link_num, CMD_NG, "NG");
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e19s4p", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*      DISC  作成処理   */
													/*-----------------------*/
	lblap_mkdisc(s_lblap_p);
													/*-----------------------*/
													/*     U Frame 送出処理  */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/* 再送 Counter Clear    */
													/*-----------------------*/
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-------------------------------*/
													/*restart T1 and set 1 to t1_flg */
													/*-------------------------------*/
	Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
	if(Result != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
	}
	s_lblap_p->lap_dt.t1_flg = CMD_ON;
													/*-----------------------*/
													/*    Next Status Go !   */
													/* (Link Disconnect状態) */
													/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_RELSTS;
	tm_lb_rell2link_rsp(s_lblap_p->link_num, CMD_OK, "OK");

    if ( s_lblap_p->link_num < D_RRH_CPRINO_RE_MIN )
    {
        lbw_tskstt = LBD_STT_L2SETUP;
    }

	return;
}

/********************************************************************************************************/

/*!
 *  @brief   自受信ビジー受信(tm_lb_1_m1e20s4p)
 *              -(State 7, State 8 common function)
 *  @note    Self already busy, then set the buffer type busy.\n
 *           Self  is not busy, then set self busy and send RNR to peer.
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e20s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*  自受信ビジー  from bufffer handle	*/
												/*   common processing between      	*/
												/*   state 7 and state 8  			  	*/
												/****************************************/
	UINT				Result;                             /*!< */
	UINT				buf_typ;                            /*!< */
	INT                 errcd;
	CMT_TSKIF_L2BSY *	buf_p;                              /*!< */
													/*-----------------------*/
													/* 自局 ビジー ?         */
													/*-----------------------*/
	buf_p = (CMT_TSKIF_L2BSY *)a_inf_p;
	/*get which buffer busy and set this buffer busy*/
	/* SYS_BKIND_RLAP1 = 10*/
	buf_typ = buf_p->buftyp;
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/* NO (Not 自局 ビジー)  */
													/*     自局 BUSY SET     */
													/*-----------------------*/
		s_lblap_p->lap_dt.own_bsy = CMD_ON;
		

		switch(buf_typ)
		{
			case	CMD_NUM0:
				s_lblap_p->lap_dt.buftyp1_bsyflg = CMD_ON;
				break;
			case	CMD_NUM1:
				s_lblap_p->lap_dt.buftyp2_bsyflg = CMD_ON;
				break;
			case	CMD_NUM2:
				s_lblap_p->lap_dt.buftyp3_bsyflg = CMD_ON;
				break;
			case	CMD_NUM3:
				s_lblap_p->lap_dt.buftyp4_bsyflg = CMD_ON;
				break;
			default:
				cm_Assert(CMD_ASL_USELOW, buf_typ, "tm_lb_1_m1e20s4p buf_typ error!!");
				return;
		}
		
		Result = cm_TStat(s_lblap_p->l2bsydettmr_id, s_lblap_p->lap_dt.l2bsytmrval, CMD_TIMMD_REPEAT,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l2bsydettmr_id, "L2 busy Timer Start NG" );
		}
		s_lblap_p->lap_dt.l2bsytmrflg = CMD_ON;
													/*----------------------*/
													/*   Peer is not busy?  */
													/*----------------------*/
		if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
		{
													/*--------------------------*/
													/*   Yes(Peer is not busy)  */
													/*   T4 is stop?      		*/
													/*--------------------------*/
			if(s_lblap_p->lap_dt.t4_flg == CMD_OFF)
			{
													/*----------------------*/
													/*	start T4			*/
													/*----------------------*/
				Result = cm_TStat(s_lblap_p->t4_id, 
						s_lblap_p->lap_dt.t4, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
				if(Result != CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t4_id, "T4 Timer Start NG" );
				}
													/*----------------------*/
													/*	Set T4 flag 		*/
													/*----------------------*/
				s_lblap_p->lap_dt.t4_flg = CMD_ON;
			}
			else
			{
				cm_Assert(CMD_ASL_USELOW, CMD_NUM1, 
									"tm_lb_1_m1e20s4p T4 control error");
				Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
				if(Result	!= CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
				}
				Result = cm_TStat(s_lblap_p->t4_id, 
						s_lblap_p->lap_dt.t4, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
				if(Result != CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t4_id, "T4 Timer Start NG" );
				}
													/*----------------------*/
													/*Set T4 flag 			*/
													/*----------------------*/
				s_lblap_p->lap_dt.t4_flg = CMD_ON;
			}
		}
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
		Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
		if(Result != CMD_RES_OK)
		{
			/*error processing*/
			cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e20s4p", 
						"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
		}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
		s_lblap_p->cmd_rsp_wk = CMD_NUM0;
		s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/*   RNR  作成処理       */
													/*-----------------------*/
		lblap_mkrnr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
		lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
		lblap_stopt2(s_lblap_p);
													/*--------------------------*/
													/*    統計カウント処理   	*/
													/* (受信バッファ ビジー 数)	*/
													/*--------------------------*/
		lblap_statis(s_lblap_p->link_num, LBD_L2_RBSY);
		lblap_statis(s_lblap_p->link_num, LBD_L2_BSRNR);										
	}
	else
	{

		switch(buf_typ)
		{
			case	CMD_NUM0:
				s_lblap_p->lap_dt.buftyp1_bsyflg = CMD_ON;
				break;
			case	CMD_NUM1:
				s_lblap_p->lap_dt.buftyp2_bsyflg = CMD_ON;
				break;
			case	CMD_NUM2:
				s_lblap_p->lap_dt.buftyp3_bsyflg = CMD_ON;
				break;
			case	CMD_NUM3:
				s_lblap_p->lap_dt.buftyp4_bsyflg = CMD_ON;
				break;
			default:
				cm_Assert(CMD_ASL_USELOW, buf_typ, "tm_lb_1_m1e20s4p buf_typ error!!");
				return;
		}
	}
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   自受信ビジーの解除受信(tm_lb_1_m1e21s4p)
 *              -(State 7, State 8 common function)
 *  @note    set the buffer type busy	cancel
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e21s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*自受信ビジーの解除 from buffer handle */
												/*   common processing between      	*/
												/*   state 7 and state 8  			  	*/
												/****************************************/
	UINT		buf_typ;                                    /*!< */
	CMT_TSKIF_L2BSYCLR *	buf_p;                          /*!< */
	buf_p = (CMT_TSKIF_L2BSYCLR *)a_inf_p;
	/*get which buffer busy cancel and set this buffer busy cancel*/
	buf_typ = buf_p->buftyp;

	switch(buf_typ)
	{
		case	CMD_NUM0:
			s_lblap_p->lap_dt.buftyp1_bsyflg = CMD_OFF;
			break;
		case	CMD_NUM1:
			s_lblap_p->lap_dt.buftyp2_bsyflg = CMD_OFF;
			break;
		case	CMD_NUM2:
			s_lblap_p->lap_dt.buftyp3_bsyflg = CMD_OFF;
			break;
		case	CMD_NUM3:
			s_lblap_p->lap_dt.buftyp4_bsyflg = CMD_OFF;
			break;
		default:
			cm_Assert(CMD_ASL_USELOW, buf_typ, "tm_lb_1_m1e21s4p buf_typ error!!");
			return;
	}
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   L2 STOP依頼受信(tm_lb_1_m1e22s4p)
 *              -(State 7, State 8 common function)
 *  @note    CPRI link will be released
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e22s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/********************************/
												/*   L2 STOP依頼  From CPRI MNG	*/
												/*   common processing between	*/
												/*   state 7 and state 8		*/
												/********************************/
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
	lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*    All timer stop     */
													/*-----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;

												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 DEACT状態) */
												/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L1ACTSTS;

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	lblap_statis(s_lblap_p->link_num, LBD_L2_RSTO);
	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set L2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Layer 1ビジー間隔タイマタイムアウト処理(tm_lb_1_m1e25s4p)
 *              -(State 5(MDIF))
 *  @note    Layer 1 busy status will be canceled
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e25s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UINT		Result;                                     /*!< */
	T_RRH_TIMERTO_NTC *timNtc_p;
	char		work[192];

	if(s_lblap_p->lap_dt.l1bsy_flg != CMD_ON)
	{
		return;
	}

	timNtc_p = (T_RRH_TIMERTO_NTC*)a_inf_p;
	if((timNtc_p->data.id_os != s_lblap_p->l1bsydettmr_osid)
		|| (timNtc_p->data.id  != s_lblap_p->l1bsydettmr_id2))
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Id mismatch key:%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}
	if(timNtc_p->data.userkey != lbw_l1BusyKey[s_lblap_p->link_num])
	{
		memset(work,0,sizeof(work));
		snprintf(work,sizeof(work),"Key mismatch key:0x%x cur(oid:0x%x-bid:0x%x) to(oid:0x%x-bid:0x%x) s:0x%x.0x%x t:0x%x.0x%x ",
				timNtc_p->data.userkey,
				s_lblap_p->l1bsydettmr_osid,
				s_lblap_p->l1bsydettmr_id2,
				timNtc_p->data.id_os,
				timNtc_p->data.id,
				(INT)(timNtc_p->data.tstat_count.tv_sec),
				(INT)(timNtc_p->data.tstat_count.tv_nsec),
				(INT)(timNtc_p->data.tout_count.tv_sec),
				(INT)(timNtc_p->data.tout_count.tv_nsec)
		);
		cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , work );
		return;
	}
	lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	/* L1 Busy Timerは、開始しますが、TO時は、Assertログ取得のみでreturnすること */
	cm_Assert( CMD_ASL_USELOW, lbw_l1BusyKey[s_lblap_p->link_num] , "L1 Busy Time Out" );
	return;
	
	
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
	lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*    All timer stop     */
													/*-----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);

													/*-----------------------*/
													/*   I Frame Chaning ?   */
													/*-----------------------*/
	if(s_lblap_p->tx_qu.qu_ctl != LBD_L2_TXI)
	{
													/*-----------------------*/
													/* NO (Not I Frame)      */
													/*      Buffer 解放      */
													/*-----------------------*/
		Result = cm_BFree((void *) s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s4p",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->tx_qu.qu_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/* I Frame 廃棄 Flag ON ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.dsc_iq_flg == CMD_ON)
	{

													/*-----------------------*/
													/* YES                   */
													/* バッファ廃棄処理開始  */
													/*-----------------------*/
		while(s_lblap_p->lap_dt.i_head_p != NULL)
		{
													/*-----------------------*/
													/* I Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* I Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s4p",
							"Buffer FreeNG_2", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			s_lblap_p->iframecnt--;
		}
													/*-----------------------*/
													/*     Pointer Null 化   */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = NULL;
		s_lblap_p->lap_dt.i_head2_p = NULL;
		s_lblap_p->lap_dt.i_tail_p = NULL;
													/*-----------------------*/
													/*       Flag Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.dsc_iq_flg = CMD_OFF;
	}

	while (s_lblap_p->lap_dt.s_head_p != NULL)
	{
													/*-----------------------*/
													/* S Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.s_head_p;
													/*-----------------------*/
													/* S Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_head_p =s_lblap_p->lap_dt.s_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s4p",
							"Buffer FreeNG_3", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
		
	}
	s_lblap_p->lap_dt.s_tail_p = NULL;

	while (s_lblap_p->lap_dt.u_head_p != NULL)
	{
													/*-----------------------*/
													/* U Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.u_head_p;
													/*-----------------------*/
													/* U Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_head_p =s_lblap_p->lap_dt.u_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e25s4p",
							"Buffer FreeNG_4", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
		
	}
	s_lblap_p->lap_dt.u_tail_p = NULL;

	s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
	
												/*-----------------------*/
												/*    Next Status Go !   */
												/* (L1 ACT･L2 ACT状態) */
												/*-----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
}


/********************************************************************************************************/

/*!
 *  @brief   CPRI信号送信通知受信(tm_lb_1_m1e1s5p)
 *              -(State 5(MDIF))
 *  @note    フレーム送信中/ 相手局ビジーでない場合、I Frame を送出する。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e1s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*   CPRI信号送信通知  From CPRI MNG	*/
												/****************************************/
													/*-----------------------*/
													/*  受信 Buffer Adr Get  */
													/*-----------------------*/
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiDstTQueueID = CMD_TSKID_TXS;
	( (LBT_TSKIF_CPRICOM  *)a_inf_p)->head.uiSrcTQueueID = CMD_TSKID_LPB;
												
	s_lblap_p->tr_adr.lap_buf_p = (LBT_TSKIF_CPRICOM  *)a_inf_p;
													/*-----------------------*/
													/*  受信 BUFFER ADR Save */
													/*-----------------------*/
	s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->tr_adr.lap_buf_p;
	s_lblap_p->buf_adr.lap_buf_p->i_link = NULL;
													/*-----------------------*/
													/* 処理 Continue         */
													/*  I-Head Pointer NULL ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.i_head_p == NULL)
	{
													/*-----------------------*/
													/*  YES (Pointer NULL)   */
													/*    Pointer Queuing    */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_tail_p = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_head2_p = s_lblap_p->buf_adr.lap_buf_p;
	}
	else
	{
													/*--------------------------*/
													/*  NO (Buffer Queuing) 	*/
													/* I-Link Pointer Queuing	*/
													/*--------------------------*/
		s_lblap_p->lap_dt.i_tail_p->i_link = s_lblap_p->buf_adr.lap_buf_p;
		s_lblap_p->lap_dt.i_tail_p = s_lblap_p->buf_adr.lap_buf_p;
													/*-----------------------*/
													/* I2-Head Pointer NULL ?*/
													/*-----------------------*/
		if(s_lblap_p->lap_dt.i_head2_p == NULL)
		{
													/*-----------------------*/
													/*  YES (Pointer NULL)   */
													/*    Pointer Queuing    */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head2_p = s_lblap_p->buf_adr.lap_buf_p;
		}
	}
	s_lblap_p->iframecnt++;
	if(s_lblap_p->iframecnt > LBD_RUNHIS_IFRAME_MAX)
	{
		tm_lb_subrunhis(s_lblap_p);
	}
	lbw_keepbufflg = CMD_ON;
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   I frame受信(tm_lb_1_m1e2s5p)
 *              -(State 5(MDIF))
 *  @note    CPRI linkよりI フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e2s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*     I  Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
	UINT		no1;
	UINT		no2;

													/*-----------------------*/
													/*   Save 状態変数       */
													/*-----------------------*/
	s_lblap_p->vs_sav = s_lblap_p->lap_dt.vs;
	s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ	*/
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);

		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}

													/*-----------------------*/
													/* 自局 ビジー ?         */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/* NO (Not  ビジー)      */
													/* NS = VR ?             */
													/*-----------------------*/
		if(s_lblap_p->ns_sav == s_lblap_p->lap_dt.vr)
		{
													/*-----------------------*/
													/* YES (NS = VR)         */
													/*    V(R) INC 処理      */
													/*-----------------------*/
			lblap_vrinc(s_lblap_p);
													/*-----------------------*/
													/* Reject 状態 Flag clear*/
													/*-----------------------*/
			s_lblap_p->lap_dt.rej_cond = CMD_OFF;
			s_lblap_p->nsRejSnd = 0xFFFFFFFF;
													/*-----------------------*/
													/* L2-DATA-IND 処理      */
													/*-----------------------*/
			lblap_l2datpr(s_lblap_p);
													/*-----------------------*/
													/*    P = 1 ?            */
													/*-----------------------*/
			if(s_lblap_p->pf_sav == CMD_NUM1)
			{
													/*-----------------------*/
													/*  YES   (P = 1)        */
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
				Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
									(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
				if(Result != CMD_RES_OK)
				{
					/*error processing*/
					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s5p", 
								"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
				}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
				s_lblap_p->cmd_rsp_wk = CMD_NUM0;
				s_lblap_p->pf_wk = CMD_NUM1;
													/*-----------------------*/
													/*    RR  作成処理       */
													/*      ( F = 1 )        */
													/*-----------------------*/
				lblap_mkrr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
				lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
				lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/*       VR  Save        */
													/*-----------------------*/
				s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
//				lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF1);
			}
			else
			{
													/*-----------------------*/
													/*   NO   (P != 1)       */
													/*       VR Save         */
													/*-----------------------*/
				s_lblap_p->vr_sav = s_lblap_p->lap_dt.vr;
													
				if(s_lblap_p->lap_dt.t2_cnt == CMD_NUM0)
				{
							
													/*-----------------------*/
													/*  YES  (T2_CNT = 0)    */
													/* T2 Timer Restart      */
													/*-----------------------*/
					Result = cm_TStat(s_lblap_p->t2_id, 
							s_lblap_p->lap_dt.t2, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
					if(Result != CMD_RES_OK)
					{
						cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t2_id, "T2 Timer Start NG" );
					}
													/*-----------------------*/
													/*      T2 Cnt INC       */
													/*-----------------------*/
					s_lblap_p->lap_dt.t2_cnt ++;
				}
				else
				{
					if(s_lblap_p->lap_dt.t2_cnt == s_lblap_p->lap_dt.peerk)
					{
 													/*-----------------------*/
													/* YES (T2_CNT = PEER K) */
													/*  T2タイマ停止処理     */
													/*-----------------------*/
						Result =	cm_TStop(s_lblap_p->t2_id, &errcd);
						if(Result	!= CMD_RES_OK)
						{
							cm_Assert( CMD_ASL_DBGLOW, Result, "T2 Timer Stop NG" );
						}
													/*-----------------------*/
													/*      Clear T2 Cnt     */
													/*-----------------------*/
						s_lblap_p->lap_dt.t2_cnt = CMD_NUM0;
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
						Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, 
										(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
						if(Result != CMD_RES_OK)
						{
							/*error processing*/
							cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s5p", 
										"cm_L2BfGet NG1",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
						}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
						s_lblap_p->cmd_rsp_wk = CMD_NUM0;
						s_lblap_p->pf_wk = CMD_NUM0;
													/*-----------------------*/
													/*       RR  作成処理    */
													/*         ( F = 0 )     */
													/*-----------------------*/
						lblap_mkrr(s_lblap_p);
													/*-----------------------*/
													/*     S Frame 送出処理  */
													/*-----------------------*/
						lblap_snds(s_lblap_p);
					}
					else
					{
													/*-----------------------*/
													/*       T2 Cnt INC      */
													/*-----------------------*/
						s_lblap_p->lap_dt.t2_cnt ++;
					}
				}
 			}
		}
		else
		{
													/*-----------------------*/
													/* NO  (NS != VR)        */
													/* 統計カウント処理      */
													/*  (無効 Ns受信回数)    */
													/*-----------------------*/
			lblap_statis(s_lblap_p->link_num, LBD_L2_INS);
													/*-----------------------*/
													/*  REJECT 状態フラグ 1 ?*/
													/*-----------------------*/
			if(s_lblap_p->lap_dt.rej_cond == CMD_ON)
			{
				if(s_lblap_p->nsRejSnd == 0xFFFFFFFF)
				{
					no1 = 0;
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->nsRejSnd, (UCHAR*)"REJ SEND NG" );
				}
				else
				{
					no1 = (s_lblap_p->nsRejSnd - s_lblap_p->lap_dt.vr)%8;
				}
				no2 = (s_lblap_p->ns_sav - s_lblap_p->lap_dt.vr)%8;
				
				/* 1回目REJ送信後かつ再送も受信抜けした場合*/
				if((s_lblap_p->lap_dt.rej_cond == CMD_ON) && (no2 <= no1))
				{
					/*	L2統計取得	*/
                    lblap_statis(s_lblap_p->link_num, LBD_L2_NS2END);
				}
				else
				{
													/*-----------------------*/
													/*  YES (REJECTフラグ 1) */
													/* P = 1 ?               */
													/*-----------------------*/
    				if(s_lblap_p->pf_sav == CMD_NUM1)
    				{
    													/*-----------------------*/
    													/*  YES (  P = 1  )      */
    													/*   L2 用 TR HNUT       */
    													/*-----------------------*/
    					Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14,
    									(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
    					if(Result != CMD_RES_OK)
    					{
    						/*error processing*/
    						cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s5p", 
    									"cm_L2BfGet NG2",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
    					}
    													/*-----------------------*/
    													/*  各 Flag Work Clear   */
    													/*-----------------------*/
    					s_lblap_p->cmd_rsp_wk = CMD_NUM0;
    					s_lblap_p->pf_wk = CMD_NUM1;
    													/*-----------------------*/
    													/*    RR  作成処理       */
    													/*      ( F = 1 )        */
    													/*-----------------------*/
    					lblap_mkrr(s_lblap_p);
    													/*-----------------------*/
    													/* S Frame 送出処理      */
    													/*-----------------------*/
    					lblap_snds(s_lblap_p);
    													/*-----------------------*/
    													/*  T2タイマ停止処理     */
    													/*-----------------------*/
    					lblap_stopt2(s_lblap_p);
    				}
                }
			}
			else
			{
				/*	抜けたNS番号を算出するために保持する	*/
				s_lblap_p->nsRejSnd = s_lblap_p->vr_sav;
													/*-----------------------*/
													/*  NO  (REJECTフラグ 0) */
													/* Reject Condition Set  */
													/*-----------------------*/
				s_lblap_p->lap_dt.rej_cond = CMD_ON;
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
				Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14,
								(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
				if(Result != CMD_RES_OK)
				{
					/*error processing*/
					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s5p", 
								"cm_L2BfGet NG3",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
				}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
				s_lblap_p->cmd_rsp_wk = CMD_NUM0;
				s_lblap_p->pf_wk = s_lblap_p->pf_sav;

				/* temp debug function for LAPB REJ problem, M-S3G-eNBSYS-00008 */	
#if 0
				for(loop = CMD_NUM0; loop < CMD_NUM7; loop++)
				{
					if(loop == CMD_NUM6)
					{
						regadr = CMD_REGADR_CPRI_FLT_DSP;
					}
					else
					{
						regadr = (CMD_REGADR_LAPB_IDLE_CNT_DSP + CMD_NUM2*loop);
					}

				for(loop = CMD_NUM0; loop < CMD_NUM4; loop++)
				{
					if(loop == CMD_NUM3)
					{
						regadr = CMD_REGADR_CPRI_FLT_DSP;
					}
					else
					{
						regadr = (CMD_REGADR_LAPB_IDLE_CNT_DSP + CMD_NUM4*loop);
					}

					
					regdat = cm_ReadReg(regadr );
					
					cm_RtcGet( &a_rtc_date );
					lpbw_logtbl.data[lpbw_logtbl.index].event_no = CMD_TSKIF_L1DATRCVNTC;
					lpbw_logtbl.data[lpbw_logtbl.index].year= a_rtc_date.year;
					lpbw_logtbl.data[lpbw_logtbl.index].month= a_rtc_date.month;
					lpbw_logtbl.data[lpbw_logtbl.index].day   = a_rtc_date.day;
					lpbw_logtbl.data[lpbw_logtbl.index].hour  = a_rtc_date.hour;
					lpbw_logtbl.data[lpbw_logtbl.index].min   = a_rtc_date.min;
					lpbw_logtbl.data[lpbw_logtbl.index].sec   = a_rtc_date.sec;
					lpbw_logtbl.data[lpbw_logtbl.index].msec  = a_rtc_date.msec;
					
					lpbw_logtbl.data[lpbw_logtbl.index].loglable = 0xBBBB;
					lpbw_logtbl.data[lpbw_logtbl.index].regadr = regadr;
					lpbw_logtbl.data[lpbw_logtbl.index].regdat = (UINT)regdat;		
								
					lpbw_logtbl.index++;
					if(lpbw_logtbl.index >= 1000)
					{
						lpbw_logtbl.index = 0;
					}
				}
#endif				
													/*-----------------------*/
													/*    REJ 作成処理       */
													/*     ( F = P )         */
													/*-----------------------*/
				lblap_mkrej(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
				lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
				lblap_stopt2(s_lblap_p);
			}
		}
	}
	else
	{
													/*-----------------------*/
													/* YES (自局ビジー)      */
													/*     バッファ廃棄処理  */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/*   L2 用 TR HNUT       */
													/*-----------------------*/
			Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14,
							(VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
			if(Result != CMD_RES_OK)
			{
				/*error processing*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e2s5p", 
							"cm_L2BfGet NG4",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
			}
													/*-----------------------*/
													/*  各 Flag Work Clear   */
													/*-----------------------*/
			s_lblap_p->cmd_rsp_wk = CMD_NUM0;
			s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    RNR 作成処理       */
													/*       ( F = P )       */
													/*-----------------------*/
			lblap_mkrnr(s_lblap_p);
													/*-----------------------*/
													/* S Frame 送出処理      */
													/*-----------------------*/
			lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*  T2タイマ停止処理     */
													/*-----------------------*/
			lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (廃棄フレーム総数)    */
													/*-----------------------*/
			lblap_statis(s_lblap_p->link_num, LBD_L2_DSC_I);
			lblap_statis(s_lblap_p->link_num, LBD_L2_BIDSC);									
		}
	}
													/*-----------------------*/
													/* YES  ( 結果 1 )       */
													/*  VA,NR 判定処理 2     */
													/*-----------------------*/
	lblap_vanr2(s_lblap_p);
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   RR/REJ frame受信(tm_lb_1_m1e3s5p)
 *              -(State 8)
 *  @note    CPRI linkよりRR/REJ フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)]
 *  @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e3s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/* RR/REJ  Frame  受信   */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ	*/
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);

		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);

//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}
													/*-----------------------*/
													/*  相手局ビジー ?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy == CMD_ON)
	{
													/*-----------------------*/
													/* YES (相手局ビジー)    */
													/*   Peer Busy Flag OFF  */
													/*-----------------------*/
		s_lblap_p->lap_dt.per_bsy = CMD_OFF;
		lblap_statis(s_lblap_p->link_num, LBD_L2_OBCRRR);
	}
													/*-----------------------*/
													/*  T3 is running?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t3_flg == CMD_ON)
	{
													/*-----------------------*/
													/*		Stop T3			 */
													/*-----------------------*/
		Result =	cm_TStop(s_lblap_p->t3_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T3 Timer Stop NG" );
		}
													/*-----------------------*/
													/*	Clear T3 flag		 */
													/*-----------------------*/
		s_lblap_p->lap_dt.t3_flg = CMD_OFF;
	}
													/*-----------------------*/
													/*  Own is not busy?     */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*-----------------------*/
													/*  Yes(Own is not busy) */
													/*  T4 is running?       */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.t4_flg == CMD_ON)
		{
													/*----------------------*/
													/*Stop T4				*/
													/*----------------------*/
			Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
			if(Result	!= CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
			}
													/*----------------------*/
													/*	Clear T4 flag		*/
													/*----------------------*/
			s_lblap_p->lap_dt.t4_flg = CMD_OFF;
		}
	}
													/*-----------------------*/
													/*      P = 1 ?          */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES                   */
													/*      P = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( P = 1 )         */
													/* Response 転送処理     */
													/*-----------------------*/
			lblap_transf(s_lblap_p);
		}
	}
	else
	{
													/*-----------------------*/
													/* NO                    */
													/*      F = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( F = 1 )         */
													/*-----------------------*/
													/* ---------------------- */
													/*  再送 Counter Clear   */
													/* ---------------------- */
			s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-----------------------*/
													/* フレーム再送処理      */
													/*-----------------------*/
			lblap_retrs(s_lblap_p);
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
			s_lblap_p->lap_dt.status = LBD_L2_INFSTS;

			/*call MW to clear StateF*/
			tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
			/*call MW to set L2 error*/
			tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
			tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);
			
			return;

		}

	}

/*  modify for F-968(DCM)[054-025(DCM)]        */
/*	if(s_lblap_p->ctl_sv == LBD_REJCTL)		   */
	if((s_lblap_p->ctl_sv == LBD_REJCTL) || (s_lblap_p->lap_dt.va != s_lblap_p->nr_sav))
	{
													/* ----------------------- */
													/*  YES (REJ)             */
													/* ----------------------- */
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
		s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
	}
													/* ----------------------- */
													/*  V(A) = V(R) 処理2     */
													/* ----------------------- */
	lblap_vanr2(s_lblap_p);
	
	return;

}


/********************************************************************************************************/

/*!
 *  @brief   RNR frame受信(tm_lb_1_m1e4s5p)
 *              -(State 8)
 *  @note    CPRI linkよりRNR フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2009/07/21 FFCS)Shihzh F-665(DCM)[037-030(DCM)] 
 *  @date    2010/04/02 FJT)Taniguchi F-968(DCM)[054-025(DCM)] 
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e4s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
													/*-----------------------*/
													/*    RNR Frame  受信    */
													/*-----------------------*/
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* VA,NR,VS 判定処理     */
													/*-----------------------*/
	lblap_vanrvs(s_lblap_p);
													/*-----------------------*/
													/*  判定結果 0 ?         */
													/*-----------------------*/
	if(s_lblap_p->cond == CMD_NUM0)
	{
													/*------------------------------*/
													/*  YES  (結果 0 )       		*/
													/*    バッファ ポインタ セーブ	*/
													/*------------------------------*/
		tm_lb_NrError(s_lblap_p);

		s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

		/*call MW to clear StateF*/
		tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
		/*call MW to set l2 error*/
		tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
		tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
		
//		l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//		memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

		return;
	}
													/*-----------------------*/
													/*  相手局ビジー ?       */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.per_bsy != CMD_ON)
	{
													/*-----------------------*/
													/* NO (相手局ビジー無し) */
													/*   Peer Busy Flag SET  */
													/*-----------------------*/
		s_lblap_p->lap_dt.per_bsy = CMD_ON;
	}
	else
	{
	    lblap_statis(s_lblap_p->link_num, LBD_L2_OBRRNR);
	}
	
	if(s_lblap_p->lap_dt.t3_flg == CMD_OFF)
	{
													/*----------------------*/
													/*	Start T3      		*/
													/*----------------------*/
		Result = cm_TStat(s_lblap_p->t3_id, s_lblap_p->lap_dt.t3, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
													/*----------------------*/
													/*Error processing    	*/
													/*----------------------*/
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t3_id, "T3 Timer Start NG" );
		}
													/*----------------------*/
													/*Set T3 flag ON   		*/
													/*----------------------*/
		s_lblap_p->lap_dt.t3_flg = CMD_ON;
	}

													/*----------------------*/
													/*	Own is not busy?    */
													/*----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{
													/*--------------------------*/
													/*	Yes(Own is not busy)    */
													/*	T4 is not running?     	*/
													/*--------------------------*/
		if(s_lblap_p->lap_dt.t4_flg == CMD_OFF)
		{
													/*--------------------------*/
													/*	Yes(T4 is not running)  */
													/*	Start T4      			*/
													/*--------------------------*/
			Result = cm_TStat(s_lblap_p->t4_id, s_lblap_p->lap_dt.t4, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
													/*--------------------------*/
													/*	Error processing    	*/
													/*--------------------------*/
			if(Result != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t4_id, "T4 Timer Start NG" );
			}
													/*----------------------*/
													/*	Set T4 flag ON    	*/
													/*----------------------*/
			s_lblap_p->lap_dt.t4_flg = CMD_ON;
		}
	}
													/*-----------------------*/
													/*      P = 1 ?          */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES                   */
													/*      P = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( P = 1 )         */
													/* Response 転送処理     */
													/*-----------------------*/
			lblap_transf(s_lblap_p);
		}
	}
	else
	{
													/*-----------------------*/
													/* NO                    */
													/*      F = 1 ?          */
													/*-----------------------*/
		if(s_lblap_p->pf_sav == CMD_NUM1)
		{
													/*-----------------------*/
													/* YES ( F = 1 )         */
													/*-----------------------*/
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
			s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-----------------------*/
													/* T1 Timer Restart      */
													/*      T1 Flag ON       */
													/*-----------------------*/
													/*-----------------------*/
													/* フレーム再送処理      */
													/*-----------------------*/
			lblap_retrs(s_lblap_p);
			
													/*-----------------------*/
													/* L1-DATA-IND 処理終了  */
													/*-----------------------*/
			s_lblap_p->lap_dt.status = LBD_L2_INFSTS;

			/*call MW to set StateF*/
			tm_lb_set_stateF(s_lblap_p->link_num,CMD_ON);
			/*call MW to clear L2 error*/
			tm_lb_set_L2Error(s_lblap_p->link_num,CMD_OFF);
			tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_ON,__func__);

			return;
		}
	}

	/*  modify for F-968(DCM)[054-025(DCM)]        */
	if(s_lblap_p->lap_dt.va != s_lblap_p->nr_sav)
	{
		s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
	}
													/*-----------------------*/
													/* YES (結果 1)          */
													/* V(A) = V(R) 処理2     */
													/*-----------------------*/
	lblap_vanr2(s_lblap_p);


	return;

}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T1)受信(tm_lb_1_m1e12s5p)
 *              -(State 8)
 *  @note    N2回再送終了, then CPRI link will be released\n
 *            -under N2 times T1 time out, RR or RNR will be sended again
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.25 FFCS)hongj modify for zynq.
 */
void	tm_lb_1_m1e12s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*   タイムアウト発生通知(T1) From OS	*/
												/****************************************/
//	UINT		Result;                                     /*!< */
	LBT_TSKIF_LPBCOM	sndbf_p;                           /*!< */
	
	if(s_lblap_p->lap_dt.t1_flg != CMD_ON)
	{
		return;
	}
	lblap_statis(s_lblap_p->link_num, LBD_L2_DSRT1O);
													/*-----------------------*/
													/*   N2回再送終了 ?      */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.retry_cnt == s_lblap_p->lap_dt.n2)
	{
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*-----------------------*/
													/*I フレーム 強制廃棄処理*/
													/*-----------------------*/
		lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*    Stop All Timer     */
													/*-----------------------*/
		tm_lb_StpAllTmr(s_lblap_p);
													/*-----------------------*/
													/* YES  (N2回再送終了)   */
													/* データ リンク 確立処理*/
													/*-----------------------*/
		lblap_estlnk(s_lblap_p);
													/*-----------------------*/
													/*     Flag Clear    	 */
													/*-----------------------*/
		s_lblap_p->lap_dt.l3int_flg = CMD_OFF;
		s_lblap_p->rcv_sbm_flg = CMD_OFF;
													/*------------------------------*/
													/*Send データリンク設定中表示	*/
													/*  To CPRI MNG task     		*/
													/*------------------------------*/
//		Result = cm_BReq(CMD_BUFCA_TSKIF, sizeof(LBT_TSKIF_LPBCOM), (VOID * *)&sndbf_p);
//		if(Result != CMD_RES_OK)
//		{
//			/*error processing*/
//			cm_MAbort(CMD_ALMCD_BUFGET, "tm_lb_1_m1e9s4p", 
//						"Breq NG",Result, sizeof(LBT_TSKIF_LPBCOM), CMD_NUM0);
//		}
		sndbf_p.link_num = s_lblap_p->link_num;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, 
								CMD_TSKIF_ESTINGIND, (UINT *)&sndbf_p);
								
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (情報転送中R.O発生)   */
													/*-----------------------*/
		lblap_statis(s_lblap_p->link_num, LBD_L2_IRO);
													/*-----------------------*/
													/*    Next Status Go !   */
													/*   (Link Set Up 状態)  */
													/*-----------------------*/
		s_lblap_p->lap_dt.status = LBD_L2_ESTSTS;
        lblap_statis(s_lblap_p->link_num, LBD_L2_INGIND_U);
		lblap_statis(s_lblap_p->link_num, LBD_L2_DSR2T1O);
		return;
	}
	else
	{
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
													/*-----------------------*/
													/*  NO  (N2回再送未終了) */
													/*   Command  転送処理   */
													/*-----------------------*/
		lblap_transp(s_lblap_p);
													/*-----------------------*/
													/* 処理 Continue         */
													/* 再送カウンタ INC      */
													/*-----------------------*/
		s_lblap_p->lap_dt.retry_cnt ++;
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (送信完了待ちT.O発生) */
													/* (情報転送中T.O発生)   */
													/* (再送フレーム総数)    */
													/* (再送 S フレーム総数) */
													/*-----------------------*/
		lblap_statis(s_lblap_p->link_num, LBD_L2_STO);
		lblap_statis(s_lblap_p->link_num, LBD_L2_ITO);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_NTXF);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_NS);
													/*-----------------------*/
													/* 統計カウント処理1     */
													/* (再送オクテット数)    */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_NTXO,CMD_NUM4);
	}
	return;

}

/********************************************************************************************************/

/*!
 *  @brief   タイムアウト発生通知(T3)受信(tm_lb_1_m1e14s5p)
 *              -(State 8)
 *  @note    Set T3 timer flag off
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e14s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************************/
												/*   タイムアウト発生通知(T3) From OS	*/
												/****************************************/
													/*-----------------------*/
													/*      T3 Flag OFF      */
													/*-----------------------*/
	s_lblap_p->lap_dt.t3_flg = CMD_OFF;
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   Layer 1 Data CNF受信(tm_lb_1_m1e0s8p)
 *              -(Test state)
 *  @note    下位モジュールよりLayer 1 Data CNF を受信した後の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e0s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/************************************/
												/* Layer 1 Data CNF from POLLING	*/
												/************************************/
	UINT		Result;                                     /*!< */
	INT			bpf_ret;

	if(s_lblap_p->lap_dt.l1bsy_flg != CMD_ON)
	{
		return;
	}
													/*-----------------------*/
													/* Test Data 廃棄処理    */
													/*-----------------------*/
													/*-----------------------*/
													/*   I Frame Chaning ?   */
													/*-----------------------*/
	if(s_lblap_p->tx_qu.qu_ctl != LBD_L2_TXI)
	{
													/*-----------------------*/
													/* NO (Not I Frame)      */
													/*      Buffer 解放      */
													/*-----------------------*/
		Result = cm_BFree((void *)s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e0s8p",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->tx_qu.qu_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/* Test Data QUEUE 有り? */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.tx_head_p != NULL)
	{
													/*-----------------------*/
													/*  YES ( BUF QUEUEING)  */
													/* HEAD = TAIL ?         */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.tx_head_p == s_lblap_p->lap_dt.tx_tail_p)
		{
													/*-----------------------*/
													/*  YES ( HEAD = TAIL)   */
													/* TEST DATA キューイング*/
													/*-----------------------*/
			s_lblap_p->tx_qu.qu_buf_p  = s_lblap_p->lap_dt.tx_head_p;
													/*-----------------------*/
													/*   Pointer Null化      */
													/*-----------------------*/
			s_lblap_p->lap_dt.tx_head_p = NULL;
			s_lblap_p->lap_dt.tx_tail_p = NULL;
		}
		else
		{
													/*-----------------------*/
													/*  NO ( HEAD != TAIL)   */
													/* TEST DATA キューイング*/
													/*-----------------------*/
			s_lblap_p->tx_qu.qu_buf_p  = s_lblap_p->lap_dt.tx_head_p;
			s_lblap_p->lap_dt.tx_head_p = s_lblap_p->lap_dt.tx_head_p->i_link;
		}

		s_lblap_p->tx_qu.qu_buf_p->disp = LBD_L2CTL_TXOFFSET;
		s_lblap_p->tx_qu.qu_buf_p->length += CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->Address = s_lblap_p->lap_dt.own_adr;
		s_lblap_p->tx_qu.qu_buf_p->Control = CMD_NUM0;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXT;

        s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;

													/*-----------------------*/
													/*  L1-DATA-RQ 送出      */
													/*  (Test Data) To TXIOCS*/
													/*-----------------------*/
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		bpf_ret = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
					CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(bpf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, bpf_ret, "cm_Enter NG" );
		}
	}
	else
	{
													/*-----------------------*/
													/*  NO  ( QUEUE NULL)    */
													/*   L1 Busy Clear       */
													/*-----------------------*/
		s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
	}

													/*-----------------------*/
													/* I Frame 廃棄 Flag ON ?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.dsc_iq_flg == CMD_ON)
	{

													/*-----------------------*/
													/* YES                   */
													/* バッファ廃棄処理開始  */
													/*-----------------------*/
		while(s_lblap_p->lap_dt.i_head_p != NULL)
		{
													/*-----------------------*/
													/* I Frame Pointer SAVE  */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* I Head 更新           */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *)s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e0s8p",
							"Buffer FreeNG_2", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			s_lblap_p->iframecnt--;
		}
													/*-----------------------*/
													/*     Pointer Null 化   */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = NULL;
		s_lblap_p->lap_dt.i_head2_p = NULL;
		s_lblap_p->lap_dt.i_tail_p = NULL;
													/*-----------------------*/
													/*       Flag Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.dsc_iq_flg = CMD_OFF;
	}
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   CPRI信号送信通知受信(tm_lb_1_m1e1s8p)
 *              -(Test state)
 *  @note    フレーム送信中/ 相手局ビジーでない場合、I Frame を送出する。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e1s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
	INT			bpf_ret;
												/****************************************/
												/*  CPRI信号送信通知 from layer 3 task  */
												/****************************************/
													/*-----------------------*/
													/*  受信 Buffer Adr Save */
													/*-----------------------*/
	s_lblap_p->tr_adr.lap_buf_p = (LBT_TSKIF_CPRICOM *)a_inf_p;
													/*-----------------------*/
													/*    受信Buf ADR Save   */
													/*-----------------------*/
	s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->tr_adr.lap_buf_p;
													/*-----------------------*/
													/*  L1-BUSY-FLAG OFF ?   */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_OFF)
	{
													/*-----------------------*/
													/*  YES (Not BUSY)       */
													/* バッファ キューイング */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->tr_adr.lap_buf_p;
		s_lblap_p->tx_qu.qu_buf_p->disp = LBD_L2CTL_TXOFFSET;
		s_lblap_p->tx_qu.qu_buf_p->length += CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->Address = s_lblap_p->lap_dt.own_adr;
		s_lblap_p->tx_qu.qu_buf_p->Control = CMD_NUM0;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXT;

		s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;
													/*-----------------------*/
													/*  L1-DATA-RQ 送出      */
													/* (Test Data) To TXIOCS */
													/*-----------------------*/
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		bpf_ret = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
					CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(bpf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, bpf_ret, "cm_Enter NG" );
		}
													/*-----------------------*/
													/*     Set L1 Busy Flag  */
													/*-----------------------*/
		s_lblap_p->lap_dt.l1bsy_flg = CMD_ON;
	}
	else
	{
													/*-----------------------*/
													/*  NO (L1 BUSY)         */
													/* T HEAD POINTER NULL ? */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.tx_head_p == NULL)
		{
													/*-----------------------*/
													/*  YES (POINTER NULL)   */
													/* バッファ キューイング */
													/*-----------------------*/
			s_lblap_p->lap_dt.tx_head_p = s_lblap_p->buf_adr.lap_buf_p;
			s_lblap_p->lap_dt.tx_tail_p = s_lblap_p->buf_adr.lap_buf_p;
			s_lblap_p->lap_dt.tx_tail_p->i_link = NULL;
		}
		else
		{
													/*-----------------------*/
													/*  NO  (BUF QUEUE有り)  */
													/* バッファ キューイング */
													/*-----------------------*/
			s_lblap_p->lap_dt.tx_tail_p->i_link = s_lblap_p->buf_adr.lap_buf_p;
			s_lblap_p->lap_dt.tx_tail_p = s_lblap_p->buf_adr.lap_buf_p;
			s_lblap_p->lap_dt.tx_tail_p->i_link = NULL;
		}
	}

	lbw_keepbufflg = CMD_ON;
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   I Frame受信(tm_lb_1_m1e2s8p)
 *              -(Test state)
 *  @note    CPRI linkよりI フレームを受信した時の処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_1_m1e2s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/****************************/
												/*   I frame  From RXOICS   */
												/****************************/
	s_lblap_p->tx_buf_adr.lap_buf_p = (LBT_TSKIF_CPRICOM *)a_inf_p;
													/*--------------------------*/
													/* L2-TEST-DATA-IND      	*/
													/*        送出 To CPRi MNG 	*/
													/*--------------------------*/
	tm_lb_DstrbtL3Msg(s_lblap_p);
	
	return;
}

/********************************************************************************************************/

/*!
 *  @brief   L3-TEST-STOP受信(tm_lb_1_m1e24s8p)
 *              -(Test state)
 *  @note    Release all buffer and change link state to state 4
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015/07/28 TDI)ikeda FHMでの17リンク対応
 */
void	tm_lb_1_m1e24s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
												/******************************/
												/* L3-TEST-STOP From CPRI MNG */
												/******************************/
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*     Buffer 廃棄処理   */
													/*-----------------------*/
	while(s_lblap_p->lap_dt.tx_head_p != NULL)
	{
		s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.tx_head_p;
		s_lblap_p->lap_dt.tx_head_p = s_lblap_p->lap_dt.tx_head_p->i_link;
		Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_1_m1e24s8p",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/*Buffer Pointer Null    */
													/*-----------------------*/
	s_lblap_p->lap_dt.tx_head_p = NULL;
	s_lblap_p->lap_dt.tx_tail_p = NULL;
													/*----------------------*/
													/*( L1/L2 ACT状態 )    	*/
													/*----------------------*/
	s_lblap_p->lap_dt.status = LBD_L2_L2ACTSTS;

	/*call MW to clear StateF*/
	tm_lb_set_stateF(s_lblap_p->link_num,CMD_OFF);
	/*call MW to set l2 error*/
	tm_lb_set_L2Error(s_lblap_p->link_num,CMD_ON);
	tm_lb_l2state_sub_ntc(s_lblap_p->link_num,D_L2_LINKSTATE_OFF,__func__);
	
//	l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index = (l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
//	memset(&l2_comw_stati_L2Tbl[s_lblap_p->link_num].blks[l2_comw_stati_L2Tbl[s_lblap_p->link_num].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));

	return;
}

/* @} */


