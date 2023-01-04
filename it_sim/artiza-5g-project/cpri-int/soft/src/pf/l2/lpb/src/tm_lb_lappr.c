/*!
 * @skip    $Id$
 * @file    tm_lb_lappr.c
 * @brief   CPRI link 制御機能ブロック
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
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
 *  @brief   データリンク確立処理(lblap_estlnk)
 *  @note    相手局のL2の対してSABM(E)を送出し、リンクを確立させる 処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_estlnk(struct LBT_MNGTBL * s_lblap_p)
{
	UINT Result;                                            /*!< */
	INT  errcd;
													/*-----------------------*/
													/*   Retry Counter clear */
													/*-----------------------*/
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-----------------------*/
													/*     L2 用 TR HNUT     */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_buf_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "lblap_estlnk", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}
													/*-----------------------*/
													/*    SABM  作成処理     */
													/*-----------------------*/
	lblap_mksabm(s_lblap_p);
													/*-----------------------*/
													/*   U Frame 送出処理    */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
													/*-----------------------*/
													/*   T1 Timer Restart    */
													/*-----------------------*/
	Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
													/*-----------------------*/
													/*   Check return code    */
													/*-----------------------*/
	if(Result != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
	}
													/*-----------------------*/
													/*      T1 Flag ON       */
													/*-----------------------*/
	s_lblap_p->lap_dt.t1_flg = CMD_ON;
	return;
}												/*  END OF  "lblap_estlnk"   */

/********************************************************************************************************/

/*!
 *  @brief   コマンド転送処理(lblap_transp)
 *  @note    RR/RNRフレームをコマンドとして相手局に対して送出する処理を行う。 ( Poll Bit = 1 )  
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_transp(struct LBT_MNGTBL * s_lblap_p)
{
	UINT Result;                                            /*!< */
	INT  errcd;
													/*-----------------------*/
													/*     L2 用 TR HNUT     */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_buf_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "lblap_transp", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}

													/*-----------------------*/
													/*       Work Set        */
													/*-----------------------*/
	s_lblap_p->cmd_rsp_wk = CMD_NUM1;
	s_lblap_p->pf_wk = CMD_NUM1;

													/*-----------------------*/
													/* OWN BUSY ?            */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.own_bsy == CMD_OFF)
	{

													/*-----------------------*/
													/* NO (Not OWN Busy)     */
													/*     RR   作成処理     */
													/*-----------------------*/
		lblap_mkrr(s_lblap_p);
	}
	else
	{

													/*-----------------------*/
													/* YES (OWN Busy)        */
													/*     RNR  作成処理     */
													/*-----------------------*/
		lblap_mkrnr(s_lblap_p);
	}
													/*-----------------------*/
													/*   S Frame 送出処理    */
													/*-----------------------*/
	lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*    T2タイマ停止処理   */
													/*-----------------------*/
	lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/*   T1 Timer Restart    */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
													/*----------------------*/
													/*	stop t1				*/
													/*----------------------*/
		Result =	cm_TStop(s_lblap_p->t1_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
													/*----------------------*/
													/*	start t1			*/
													/*----------------------*/
		Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Start NG" );
		}
	}
	else
	{
													/*----------------------*/
													/*	start t1			*/
													/*----------------------*/
		Result = cm_TStat(s_lblap_p->t1_id, s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
		if(Result != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
		}
	}
													/*-----------------------*/
													/*      T1 Flag ON       */
													/*-----------------------*/
	s_lblap_p->lap_dt.t1_flg = CMD_ON;
	return;
}												/*  END OF   "lblap_transp"  */



/********************************************************************************************************/

/*!
 *  @brief   レスポンス転送処理(lblap_transf)
 *  @note    RR/RNRフレームをレスポンスとして相手局に対して送出する処理を行う。 ( Final Bit = 1 )
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_transf(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*     L2 用 TR HNUT     */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(struct LAP_LAYER2_BUF), CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_layer_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "lblap_transf", 
					"cm_L2BfGet NG",Result, sizeof(struct LAP_LAYER2_BUF), CMD_NUM0);
	}

													/*-----------------------*/
													/*       Work Set        */
													/*-----------------------*/
	s_lblap_p->cmd_rsp_wk = CMD_NUM0;
	s_lblap_p->pf_wk = CMD_NUM1;
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
													/*   S Frame 送出処理    */
													/*-----------------------*/
	lblap_snds(s_lblap_p);
													/*-----------------------*/
													/*    T2タイマ停止処理   */
													/*-----------------------*/
	lblap_stopt2(s_lblap_p);
	return;
}												/*  END OF  "lblap_transf"   */

/********************************************************************************************************/

/*!
 *  @brief   CONFIRM 受信処理 1(lblap_conf1)
 *  @note    フレーム送信要求応答受信後の処理を行う。(タスク状態7、8以外の時処理される。) 
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
void lblap_conf1(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UINT		Result;                                     /*!< */
//	INT         errcd;
												
//	Result =	cm_TStop(s_lblap_p->l1bsydettmr_id, &errcd);
//	if(Result	!= CMD_RES_OK)
//	{
//		cm_Assert( CMD_ASL_DBGLOW, Result, "L1 busy Timer Stop NG" );
//	}

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
			cm_MAbort(CMD_ALMCD_BUFREL, "lblap_conf1",
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
				cm_MAbort(CMD_ALMCD_BUFREL, "lblap_conf1",
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
													/*-----------------------*/
													/*  U Head NULL ?        */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.u_head_p != NULL)
	{
													/*-----------------------*/
													/* NO (U Head EXIST)     */
													/* U Head Queuing        */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->lap_dt.u_head_p;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXU;
														/*----------------------*/
														/*  L1-DATA-RQ 送出  	*/
														/*  処理開始 To  TXOICS */
														/*----------------------*/
		s_lblap_p->lap_dt.u_head_p = s_lblap_p->lap_dt.u_head_p->i_link;
													/*-----------------------*/
													/*  U Head NULL ?        */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.u_head_p == NULL)
		{
													/*-----------------------*/
													/* YES (U Head NULL)     */
													/*     Pointer Null 化   */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_tail_p = NULL;
		}
													/*-----------------------*/
													/*   統計カウント処理    */
													/*  (送信Uフレーム総数)  */
													/*  (送信Cフレーム総数)  */
													/*  (送信フレーム総数)   */
													/*-----------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SU);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SC);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*(送信 Frame Octet 数)  */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,(USHORT)(s_lblap_p->tx_qu.qu_buf_p->length + CMD_NUM2));
													/*-----------------------*/
													/* Buffer SEND 処理      */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
			CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
		}
#if 0
		/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//		lbw_l1BusyKey[s_lblap_p->link_num] = random();
//		Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//					&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//		if(Result != CMD_RES_OK)
//		{
//			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l1bsydettmr_id, "L1 busy Timer Start NG" );
//		}
#endif
	}
	else
	{												/* U Frame Pointer Null */
													/*-----------------------*/
													/* YES ( U Head NULL)    */
													/*       Flag Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
	}
	return;
}												/*  END OF  "lblap_conf1"   */



/********************************************************************************************************/

/*!
 *  @brief   CONFIRM 受信処理 2(lblap_conf2)
 *  @note    フレーム送信要求応答受信後の処理を行う。(タスク状態7、8の時のみ処理される。) 
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
void lblap_conf2(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UINT		Result;                                     /*!< */
//	INT         errcd;
												
//	Result =	cm_TStop(s_lblap_p->l1bsydettmr_id, &errcd);
//	if(Result	!= CMD_RES_OK)
//	{
//		cm_Assert( CMD_ASL_DBGLOW, Result, "L1 busy Timer Stop NG" );
//	}
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
			cm_MAbort(CMD_ALMCD_BUFREL, "lblap_conf2",
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
													/* YES (C-Link NULL)     */
													/*  I Head Pointer 更新  */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*      Buffer 解放      */
													/*-----------------------*/
			Result = cm_BFree((void *) s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "lblap_conf2",
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
													/*-----------------------*/
													/*  U Head NULL ?        */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.u_head_p != NULL)
	{
													/*-----------------------*/
													/* NO (U Head EXIST)     */
													/* U Head Queuing        */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->lap_dt.u_head_p;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXU;
													/*-----------------------*/
													/*  U Head Pointer 更新  */
													/*-----------------------*/
		s_lblap_p->lap_dt.u_head_p = s_lblap_p->lap_dt.u_head_p->i_link;

													/*-----------------------*/
													/*  U Head NULL ?        */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.u_head_p == NULL)
		{											/* U Frame Pointer Null  */
													/*-----------------------*/
													/* YES (U Head NULL)     */
													/*     Pointer Null 化   */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_tail_p = NULL;
		}
													/*-----------------------*/
													/*   統計カウント処理    */
													/*  (送信Uフレーム総数)  */
													/*  (送信Cフレーム総数)  */
													/*  (送信フレーム総数)   */
													/*-----------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SU);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SC);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*(送信 Frame Octet 数)  */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,(USHORT)(s_lblap_p->tx_qu.qu_buf_p->length + CMD_NUM2));
													/*-----------------------*/
													/*   BUFFER SEND 処理    */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
			CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
		}
#if 0
		/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//		lbw_l1BusyKey[s_lblap_p->link_num] = random();
//		Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//				&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//		if(Result != CMD_RES_OK)
//		{
//			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l1bsydettmr_id, "L1 busy Timer Start NG" );
//		}
#endif
	}
	else
	{
													/*-----------------------*/
													/* YES (U Head NULL)     */
													/* S Head NULL ?         */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.s_head_p != NULL)
		{
													/*-----------------------*/
													/* NO (S Head EXIST)     */
													/* S Head Queuing        */
													/*-----------------------*/
			s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->lap_dt.s_head_p;
			s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXS;
													/*-----------------------*/
													/*  S Head Pointer 更新  */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_head_p = s_lblap_p->lap_dt.s_head_p->i_link;

													/*-----------------------*/
													/*  S Head NULL ?        */
													/*-----------------------*/
			if(s_lblap_p->lap_dt.s_head_p == NULL)
			{
													/*-----------------------*/
													/* YES ( Pointer Null)   */
													/*     Pointer Null 化   */
													/*-----------------------*/
				s_lblap_p->lap_dt.s_tail_p = NULL;
			}
													/*-----------------------*/
													/*   統計カウント処理    */
													/*  (送信Sフレーム総数)  */
													/*  (送信Cフレーム総数)  */
													/*  (送信フレーム総数)   */
													/*-----------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_SS);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_SC);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*(送信 Frame Octet 数)  */
													/*-----------------------*/
//			lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,(USHORT)(s_lblap_p->tx_qu.qu_buf_p->length + CMD_NUM2));
													/*-----------------------*/
													/* BUFFER SEND 処理      */
													/*-----------------------*/
			s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
			s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
			s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
			s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
			s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
			s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;
											/* LAPB走行履歴書込み	*/
			tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
			Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
				CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
			}
#if 0
			/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//			lbw_l1BusyKey[s_lblap_p->link_num] = random();
//			Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//					&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//			if(Result != CMD_RES_OK)
//			{
//				cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l1bsydettmr_id, "L1 busy Timer Start NG" );
//			}
#endif
		}
		else
		{
													/*-----------------------*/
													/* NO (S Head EXIST)     */
													/* タスク状態 7 ?       */
													/*-----------------------*/
			if((s_lblap_p->lap_dt.status == LBD_L2_INFSTS)
				|| (s_lblap_p->lap_dt.status == LBD_L2_NE_INFSTS))
			{
													/*----------------------*/
													/* YES (状態 7)        */
													/* N/E switch start??	*/
													/*----------------------*/
				if(s_lblap_p->lap_dt.status == LBD_L2_NE_INFSTS)
				{
					tm_lb_Disc10kBf(s_lblap_p);
				}

				if(s_lblap_p->lap_dt.i_head2_p != NULL)
				{
													/*-----------------------*/
													/* NO (I Frame Exist)    */
													/*       Flag Clear      */
													/*-----------------------*/
					s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
													/*-----------------------*/
													/*   I Frame 送出処理    */
													/*-----------------------*/
					lblap_sndi(s_lblap_p);
				}
				else
				{
													/*-----------------------*/
													/* YES (I Frame NULL)    */
													/*       Flag Clear      */
													/*-----------------------*/
					s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
				}

			}
			else
			{
													/*-----------------------*/
													/*  NO (状態 7以外)     */
													/*       Flag Clear      */
													/*-----------------------*/
				s_lblap_p->lap_dt.l1bsy_flg = CMD_OFF;
			}
		}
	}
	
	return;
	
}												/*  END OF  "lblap_conf2"   */

/********************************************************************************************************/

/*!
 *  @brief   フレーム再送処理(lblap_retrs)
 *  @note    Iフレームの再送処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_retrs(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	LBT_TSKIF_CPRICOM	*rej_i_p;                           /*!< ワーク  ポインタ    */
	USHORT			no;                                     /*!< */
	UINT			Result;                                 /*!< */
	INT             errcd;

													/*-----------------------*/
													/* 応答確認済 I Frame    */
													/*             廃棄処理  */
													/*-----------------------*/
	lblap_freeak(s_lblap_p);
													/*-----------------------*/
													/* I Frame Pointer Save  */
													/*-----------------------*/
	rej_i_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* ワーク カウンタ クリア*/
													/*-----------------------*/
	no = CMD_NUM0;
													/*-----------------------*/
													/*  I Head = I Head2     */
													/*        まで処理継続   */
													/*-----------------------*/
	while(rej_i_p != s_lblap_p->lap_dt.i_head2_p)
	{
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*(再送Frame Octet 数)   */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_NTXO,(USHORT)(rej_i_p->length + CMD_NUM2));
													/*-----------------------*/
													/*    Buffer操作         */
													/*-----------------------*/

		/*rej_i_p->disp +=2; */
		rej_i_p->length -= CMD_NUM2; 
													/*-----------------------*/
													/*  I Head の更新        */
													/*-----------------------*/
		rej_i_p = rej_i_p->i_link;
													/*-----------------------*/
													/*  ワークカウンタ INC   */
													/*-----------------------*/
		no++;
	}
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*   (再送フレーム 数)   */
													/*   (再送 I Frame総数)  */
													/*-----------------------*/
//	lblap_stats1(s_lblap_p->link_num, LBD_L2_NTXF,no);
//	lblap_stats1(s_lblap_p->link_num, LBD_L2_NI,no);
													/*-----------------------*/
													/*      Head2 = Head     */
													/*-----------------------*/
	s_lblap_p->lap_dt.i_head2_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/*     状態変数セーブ    */
													/*-----------------------*/
	s_lblap_p->vs_sav = s_lblap_p->lap_dt.vs;
	s_lblap_p->lap_dt.vs = s_lblap_p->nr_sav;			/*     V(S) = N(R)   */
	s_lblap_p->lap_dt.va = s_lblap_p->nr_sav;			/*     V(A) = N(R)   */

	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
		Result =	cm_TStop(s_lblap_p->t1_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
	
	if((s_lblap_p->lap_dt.status == LBD_L2_NE_ACKSTS)
		|| (s_lblap_p->lap_dt.status == LBD_L2_NE_INFSTS))
	{
		tm_lb_Disc10kBf(s_lblap_p);
	}
													/*-----------------------*/
													/* I Head Pointer 2 NULL?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.i_head2_p != NULL)
	{
													/*-----------------------*/
													/* NO (Pointer Exist)    */
													/*   I Frame 送出処理    */
													/*-----------------------*/
		lblap_sndi(s_lblap_p);
	}
	lblap_statis(s_lblap_p->link_num, LBD_L2_RSEND);
	return;
}												/*  END OF  "lblap_retrs"   */


/********************************************************************************************************/

/*!
 *  @brief   V(A)=N(R)処理1 (lblap_vanr1)
 *  @note    V(A)を更新し、未送出Iフレームの送信処理を行う。(タスク状態7の場合のみ処理される。)
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2010/04/02 FJT)Taniguchi modify for F-968(DCM)[054-025(DCM)] 
 */
void lblap_vanr1(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/* 応答確認済 I Frame    */
													/*             廃棄処理  */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.va != s_lblap_p->nr_sav)
	{
		lblap_freeak(s_lblap_p);
													/*-----------------------*/
													/*   状態変数セット      */
													/*-----------------------*/
		s_lblap_p->lap_dt.va = s_lblap_p->nr_sav;

		/* modify for F-968(DCM)[054-025(DCM)] */
													/* ----------------------- */
													/*  再送 Counter Clear    */
													/* ----------------------- */
		s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
	
	}
	
	if((s_lblap_p->lap_dt.status == LBD_L2_NE_ACKSTS)
		|| (s_lblap_p->lap_dt.status == LBD_L2_NE_INFSTS))
	{
		tm_lb_Disc10kBf(s_lblap_p);
	}
													/*-----------------------*/
													/* I Head Pointer 2 NULL?*/
													/*-----------------------*/
	if(s_lblap_p->lap_dt.i_head2_p != NULL)
	{
													/*-----------------------*/
													/* NO (Pointer Exist)    */
													/*   I Frame 送出処理    */
													/*-----------------------*/
		lblap_sndi(s_lblap_p);
	}
	

	return;
}												/*  END OF  "lblap_vanr1"   */


/********************************************************************************************************/

/*!
 *  @brief   V(A)=N(R)処理2  (lblap_vanr2)
 *  @note    V(A)の更新処理を行う。(タスク状態8の場合のみ処理される。)
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_vanr2(struct LBT_MNGTBL * s_lblap_p)
{
	if(s_lblap_p->lap_dt.va != s_lblap_p->nr_sav)
	{
													/*-----------------------*/
													/* 応答確認済 I Frame    */
													/*             廃棄処理  */
													/*-----------------------*/
		lblap_freeak(s_lblap_p);
													/*-----------------------*/
													/*   状態変数セット      */
													/*-----------------------*/
		s_lblap_p->lap_dt.va = s_lblap_p->nr_sav;
	}
	return;
}												/*  END OF  "lblap_vanr2"   */


/********************************************************************************************************/

/*!
 *  @brief   FRMR  ERROR処理  (lblap_frmrer)
 *  @note    FRMRデータを送出する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_frmrer(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/* FRMR 情報 吸い上げ    */
													/*-----------------------*/
	s_lblap_p->fmr1_sav.fmr1_inf.rej_ctl = s_lblap_p->ctl1_sav.ctl1_all;
	s_lblap_p->fmr1_sav.fmr1_inf.zero = CMD_NUM0;
													/*-----------------------*/
													/*コマンド･レスポンス フラグ 1 ?  */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
	{
													/*-----------------------*/
													/*YES  (フラグ 1)        */
													/*-----------------------*/
		s_lblap_p->fmr1_sav.fmr1_inf.n = CMD_NUM0;
	}
	else
	{
													/*-----------------------*/
													/* NO  (フラグ0)         */
													/*-----------------------*/
		s_lblap_p->fmr1_sav.fmr1_inf.n = CMD_NUM1;
	}
													/*-----------------------*/
													/*     状態変数セット    */
													/*-----------------------*/
	s_lblap_p->fmr1_sav.fmr1_inf.vs = s_lblap_p->vs_sav;
	s_lblap_p->fmr1_sav.fmr1_inf.vr = s_lblap_p->vr_sav;

													/*-----------------------*/
													/*  FRMR ERROR種別       */
													/*           判別処理    */
													/*-----------------------*/
	switch(s_lblap_p->ctl_sv)
	{
		case LBD_ERRORW :
			s_lblap_p->fmr1_sav.fmr1_inf.w = CMD_NUM1;
			s_lblap_p->fmr1_sav.fmr1_inf.x = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.y = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.z = CMD_NUM0;
			break;

		case LBD_ERRORX :
			s_lblap_p->fmr1_sav.fmr1_inf.w = CMD_NUM1;
			s_lblap_p->fmr1_sav.fmr1_inf.x = CMD_NUM1;
			s_lblap_p->fmr1_sav.fmr1_inf.y = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.z = CMD_NUM0;
			break;

		case LBD_ERRORY :
			s_lblap_p->fmr1_sav.fmr1_inf.w = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.x = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.y = CMD_NUM1;
			s_lblap_p->fmr1_sav.fmr1_inf.z = CMD_NUM0;
			break;

		case LBD_ERRORZ :
			s_lblap_p->fmr1_sav.fmr1_inf.w = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.x = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.y = CMD_NUM0;
			s_lblap_p->fmr1_sav.fmr1_inf.z = CMD_NUM1;
													/*-----------------------*/
													/*     ABLOG 収集        */
													/* ( N(R)誤りフレーム 受信 ) */
													/*-----------------------*/
			lblap_ablog(s_lblap_p->link_num, LBD_L2_ABNRF);
			break;

		default :
			break;
	}
	s_lblap_p->fmr1_sav.fmr1_inf.zero2 = CMD_NUM0;
	
													/*-----------------------*/
													/*     L2 用 TR HNUT     */
													/*-----------------------*/
	Result = cm_L2BfGet(CMD_BUFCA_LAPSND, LBD_FRMRSIZE, CMD_NUM14, (VOID * *)&s_lblap_p->hnt_adr.lap_buf_p);
	if(Result != CMD_RES_OK)
	{
		/*error processing*/
		cm_MAbort(CMD_ALMCD_BUFGET, "lblap_frmrer", 
					"cm_L2BfGet NG",Result, LBD_FRMRSIZE, CMD_NUM0);
	}
													/*-----------------------*/
													/*     状態変数セット    */
													/*-----------------------*/
	s_lblap_p->pf_wk = s_lblap_p->pf_sav;
													/*-----------------------*/
													/*    FRMR  作成処理     */
													/*-----------------------*/
	lblap_mkfrmr(s_lblap_p);
													/*-----------------------*/
													/*   U Frame 送出処理    */
													/*-----------------------*/
	lblap_sndu(s_lblap_p);
	return;
}												/*  END OF  "lblap_frmrer"   */



/********************************************************************************************************/

/*!
 *  @brief   V(R)インクリメント処理  (lblap_vrinc)
 *  @note    V(R)カウンタ(受信状態変数)をインクリメントする 処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_vrinc(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UCHAR			no;                                     /*!< */
													/*-----------------------*/
													/* モジュロ値セット      */
													/*-----------------------*/
	no = CMD_NUM8;
													/*-----------------------*/
													/*    Modulo 計算        */
													/*-----------------------*/
	s_lblap_p->lap_dt.vr = (UCHAR)((s_lblap_p->lap_dt.vr + CMD_NUM1) % no);
	return;
}												/*  END OF  "lblap_vrinc"   */


/********************************************************************************************************/

/*!
 *  @brief   V(A)=< N(R)=< V(S)チェック処理 (lblap_vanrvs)
 *  @note    状態変数の値をチェックし結果を､呼びもとに返す処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_vanrvs(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	UCHAR				no;                                 /*!< */
	union	CTL_FLD1	work1;                              /*!< Control Field 1 Work  */
	union	CTL_FLD1	work2;                              /*!< Control Field 2 Work  */

													/*-----------------------*/
													/* モジュロ値セット      */
													/*-----------------------*/
	no = CMD_NUM8;

													/*-----------------------*/
													/*   V(S) - V(A) 計算    */
													/*-----------------------*/
	work1.ctl1_all = (UCHAR)(s_lblap_p->lap_dt.vs - s_lblap_p->lap_dt.va);
													/*-----------------------*/
													/*    Modulo 計算        */
													/*-----------------------*/
	work1.ctl1_all %= no;

													/*-----------------------*/
													/*   N(R) - V(A) 計算    */
													/*-----------------------*/
	work2.ctl1_all = (UCHAR)(s_lblap_p->nr_sav - s_lblap_p->lap_dt.va);
													/*-----------------------*/
													/*    Modulo 計算        */
													/*-----------------------*/
	work2.ctl1_all %= no;

													/*-----------------------*/
													/* (VS-VA) >= (NR-VA) ?  */
													/*-----------------------*/
	if((work1.ctl1_all-work2.ctl1_all) >= CMD_NUM0)
	{
													/*-----------------------*/
													/* YES                   */
													/* 判定結果エリア1セット */
													/*-----------------------*/
		s_lblap_p->cond = CMD_NUM1;
	}
	else
	{
													/*-----------------------*/
													/* NO                    */
													/* 判定結果エリア0セット */
													/*-----------------------*/
		s_lblap_p->cond = CMD_NUM0;
	}
	return;
}												/*  END OF  "lblap_vanrvs"   */


/********************************************************************************************************/

/*!
 *  @brief   Control  Field  Check処理  (lblap_chkctl)
 *  @note    フレームのコントロール・フィールドを判定し､フレーム種別を切りわける処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_chkctl(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	union CTL_FLD1	ctl_work;                               /*!< Control Field work   */
	union CTL_WK	work;                                   /*!< Control Field のBit   */
													/* 合わせに使用する。    */
	UINT			lwk;                                    /*!< Length Work      */
	UINT			rxctl_sv;                               /*!< */

	rxctl_sv  = CMD_NUM0;
													/*-----------------------*/
													/*   Total Length Save   */
													/*-----------------------*/
	s_lblap_p->leng_wk = s_lblap_p->tr_adr.lap_buf_p->length;
	lwk = s_lblap_p->leng_wk;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*   ( 受信フレーム 総数 )   */
													/*-----------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_RXF);
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/*   ( 受信オクテット総数 )   */
													/*-----------------------*/
//	lblap_stats1(s_lblap_p->link_num, LBD_L2_RXO,(USHORT)(lwk + CMD_NUM2));

													/*-----------------------*/
													/*  Total Length 1以下 ? */
													/*-----------------------*/
	if(s_lblap_p->leng_wk <= CMD_NUM1)
	{
													/*-----------------------*/
													/* YES (INVALID Frame)   */
													/*   (Short Frame)       */
													/*-----------------------*/
		s_lblap_p->ctl_sv = LBD_OTHERS;
													/*-----------------------*/
													/* ABLOG_INVSHRT 処理    */
													/*     ABLOG 収集        */
													/* ( 無効フレーム受信        */
													/*          (未定義長))  */
													/*-----------------------*/
		lblap_ablog(s_lblap_p->link_num, LBD_L2_ABUFU);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
		return;
	}
	
	if(s_lblap_p->adr_wk == s_lblap_p->lap_dt.own_adr)
	{
													/*-----------------------*/
													/* YES                   */
													/*      Response Frame   */
													/*-----------------------*/
		s_lblap_p->cmd_rsp_flg = CMD_NUM0;
	}
	else
	{
		if(s_lblap_p->adr_wk == s_lblap_p->lap_dt.per_adr)
		{
													/*-----------------------*/
 													/* アドレス = 相手局アドレス ? */
													/*-----------------------*/
													/*-----------------------*/
													/* YES                   */
													/*      Command  Frame   */
													/*-----------------------*/
			s_lblap_p->cmd_rsp_flg = CMD_NUM1;
		}
		else
		{
													/*-----------------------*/
													/* NO (INVALID Frame)    */
													/*   無効アドレス        */
													/*-----------------------*/
			s_lblap_p->ctl_sv = LBD_OTHERS;
													/*-----------------------*/
													/*     Flag Clear        */
													/*-----------------------*/
			s_lblap_p->cmd_rsp_flg = CMD_NUM0;
			s_lblap_p->pf_sav = CMD_NUM0;
													/*-----------------------*/
													/* ABLOG_INVADR  処理    */
													/*     ABLOG 収集        */
													/* ( 無効フレーム受信        */
													/*      (未定義アドレス) */
													/*-----------------------*/
			lblap_ablog(s_lblap_p->link_num, LBD_L2_ABUFA);

			return;
		}
	}
 
	if(s_lblap_p->ctl1_sav.ctli_8.i8zero == CMD_NUM0)
	{
		rxctl_sv = LBD_L2_RXI;
	}
	else
	{
		if(s_lblap_p->ctl1_sav.ctls_8.s82s != CMD_NUM1)
		{
			rxctl_sv = LBD_L2_RXS;
		}
		else
		{
			rxctl_sv = LBD_L2_RXU;
		}
	}

	switch(rxctl_sv)
	{
											/*-------------------------------*/
											/*         I  Frame        */
											/*-------------------------------*/

		case LBD_L2_RXI:
													/*-----------------------*/
													/* YES (i8zero 0)        */
													/*コマンド･レスポンス フラグ 1 ?  */
													/*-----------------------*/
			if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
			{
													/*-----------------------*/
													/* YES                   */
													/* (Command Frame)       */
													/*   Frame 種別設定      */
													/*-----------------------*/
				s_lblap_p->ctl_sv = LBD_ICTL;

														/*-------------------*/
														/*      Save N(S)    */
														/*-------------------*/
				work.work_all = CMD_NUM0;
				work.wk_8.field3 = s_lblap_p->ctl1_sav.ctli_8.i8ns;
				s_lblap_p->ns_sav = work.work_all;
														/*-------------------*/
														/*      Save N(R)    */
														/*-------------------*/
				work.work_all = CMD_NUM0;
				work.wk_8.field3 = s_lblap_p->ctl1_sav.ctli_8.i8nr;
				s_lblap_p->nr_sav = work.work_all;
														/*-------------------*/
														/*    Save P/F Bit   */
														/*-------------------*/
				s_lblap_p->pf_sav = CMD_NUM0;
				work.work_all = CMD_NUM0;
				work.others.field1 = s_lblap_p->ctl1_sav.ctli_8.i8pf;
				s_lblap_p->pf_sav = work.work_all;
														/*-------------------*/
														/*  Length Decriment */
														/* (アドレス制御部削*/
														/*-------------------*/
				s_lblap_p->leng_wk = s_lblap_p->tr_adr.lap_buf_p->length - CMD_NUM2;

													/*-----------------------*/
													/* 最大I Frame 長越え ?  */
													/*-----------------------*/
				if((s_lblap_p->leng_wk + CMD_NUM4) > s_lblap_p->lap_dt.n1)
				{
													/*-----------------------*/
													/* YES (INVALID Frame)   */
													/*   (Long Frame)        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_ERRORY;
													/*-----------------------*/
													/* ABLOG_N1OVR 処理参照  */
													/*-----------------------*/
					lblap_ablog(s_lblap_p->link_num, LBD_L2_ABNOF);
													/*-----------------------*/
													/*   統計カウント処理    */
													/* ( 受信データ長オーバー 数) */
													/*  ( イリーガル受信総数 )   */
													/*-----------------------*/
//					lblap_statis(s_lblap_p->link_num, LBD_L2_DOV);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_ILR);
				}
				else
				{
													/*-----------------------*/
													/*   統計カウント処理    */
													/*  ( 受信 I Frame 数 )  */
													/*-----------------------*/
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RI);
													/*-----------------------*/
													/*   統計カウント処理 1  */
													/* ( 受信データ オクテット 数 ) */
													/*-----------------------*/
//					lblap_stats1(s_lblap_p->link_num, LBD_L2_RDTO,(USHORT)s_lblap_p->leng_wk);
				}
													/*-----------------------*/
													/*      Goto END         */
													/*-----------------------*/
			}
			else
			{
													/*-----------------------*/
													/*   Frame 種別設定      */
													/*-----------------------*/
				s_lblap_p->ctl_sv = LBD_ERRORW;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
				lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);

			}
			break;
											/*-------------------------------*/
											/*         S  Frame        */
											/*-------------------------------*/

		case LBD_L2_RXS:
													/*-----------------------*/
													/* Total Length 2 以外 ? */
													/*-----------------------*/
			if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM2)
			{
													/*-----------------------*/
													/* YES (ERROR Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
				s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
				lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
				break;

			}
												/*---------------------------*/
												/*   S  Frame 種別判別処理   */
												/* 1:RR Frame                */
												/* 2:RNR Frame               */
												/* 3:REJ Frame               */
												/* OTHERS                    */
												/*---------------------------*/
			switch(s_lblap_p->ctl1_sav.ctls_8.s83s)
			{
				case  0:							/*-----------------------*/
													/*     RR Frame (B'00)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_RRCTL;
					break;

				case  1:							/*-----------------------*/
													/*    RNR Frame (B'01)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_RNRCTL;
					break;

				case  2:							/*-----------------------*/
													/*    REJ Frame (B'10)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_REJCTL;
					break;

				default:							/*-----------------------*/
													/*         OTHERS        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_ERRORW;
													/*-----------------------*/
													/* ABLOG_UND    処理参照 */
													/*-----------------------*/
					lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
					return;
			}



													/*-----------------------*/
													/*      Save N(R)        */
													/*-----------------------*/
			work.work_all = CMD_NUM0;
			work.wk_8.field3 = s_lblap_p->ctl1_sav.ctls_8.s8nr;
			s_lblap_p->nr_sav = work.work_all;
													/*-----------------------*/
													/*    Save P/F Bit       */
													/*-----------------------*/
			s_lblap_p->pf_sav = CMD_NUM0;
			work.work_all = CMD_NUM0;
			work.others.field1 = s_lblap_p->ctl1_sav.ctls_8.s8pf;
			s_lblap_p->pf_sav = work.work_all;

			switch(s_lblap_p->ctl_sv)
			{
				case LBD_RNRCTL :
													/*-----------------------*/
													/*   統計カウント処理    */
													/*( 受信 RNR Frame 数 )  */
													/*-----------------------*/
					lblap_statis(s_lblap_p->link_num, LBD_L2_RRNR);
					break;

				case LBD_REJCTL :
													/*-----------------------*/
													/*   統計カウント処理    */
													/*( 受信 REJ Frame 数 )  */
													/*-----------------------*/
					lblap_statis(s_lblap_p->link_num, LBD_L2_RREJ);
					break;

				default :
					break;
			}												/* End Of Switch */

													/*-----------------------*/
													/*   統計カウント処理    */
													/*  ( 受信 S Frame 数 )  */
													/*  ( 受信 C Frame 数 )  */
													/*-----------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_RS);
//			lblap_statis(s_lblap_p->link_num, LBD_L2_RC);

			break;
											/*-------------------------------*/
											/*         U  Frame        */
											/*-------------------------------*/

		case LBD_L2_RXU:


													/*-----------------------*/
													/* Clear  P/F Save       */
													/*-----------------------*/
			s_lblap_p->pf_sav  = CMD_NUM0;
													/*-----------------------*/
													/*    Save P/F Bit       */
													/*-----------------------*/
			work.work_all = CMD_NUM0;
			work.others.field1 = s_lblap_p->ctl1_sav.ctlus_8.us8pf;
			s_lblap_p->pf_sav = work.work_all;
													/*-----------------------*/
													/*       Work 設定       */
													/*-----------------------*/
			ctl_work.ctl1_all =  s_lblap_p->ctl1_sav.ctl1_all;
			ctl_work.ctlus_8.us8pf  = CMD_NUM0;
	
												/*---------------------------*/
												/*    U Frame 種別判別処理   */
												/* 1:SABM Frame              */
												/* 2:SABME Frame             */
												/* 3:DISC Frame              */
												/* 4:DM Frame                */
												/* 5:UA Frame                */
												/* 6:FRMR Frame              */
												/* 7:XID Frame               */
												/* OTHERS                    */
												/*---------------------------*/
			switch(ctl_work.ctl1_all)
			{
				case  0x2F :						/*-----------------------*/
													/*        SABM Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_SBMCTL;
													/*-----------------------*/
													/* コマンド･レスポンス フラグ 1 ? */
													/*-----------------------*/
					if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
					{
													/*-----------------------*/
													/* YES (Command Frame)   */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
							lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
							break;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Response Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
						lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
						break;
					}
					
					lblap_statis(s_lblap_p->link_num, LBD_L2_RSBM);

//					lblap_statis(s_lblap_p->link_num, LBD_L2_RU);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RC);

					break;

				case 0x43 :							/*-----------------------*/
													/*       DISC  Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_DISCCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 1 ? */
													/*-----------------------*/
					if(s_lblap_p->cmd_rsp_flg == CMD_NUM1)
					{
													/*-----------------------*/
													/* YES (Command Frame)   */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
							lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
							break;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Response Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
						lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
						break;
					}

					lblap_statis(s_lblap_p->link_num, LBD_L2_RDSC);

//					lblap_statis(s_lblap_p->link_num, LBD_L2_RU);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RC);
					break;


				case 0x0F :							/*-----------------------*/
													/*        DM   Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_DMCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(s_lblap_p->cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* YES (Response Frame)  */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
							lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
							break;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
						lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
						return;
					}

					lblap_statis(s_lblap_p->link_num, LBD_L2_RDM);

//					lblap_statis(s_lblap_p->link_num, LBD_L2_RU);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RC);

					break;

				case 0x63 :							/*-----------------------*/
													/*        UA   Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_UACTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(s_lblap_p->cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* YES (Response Frame)  */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
							lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
							return;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
						lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
						return;
					}
					lblap_statis(s_lblap_p->link_num, LBD_L2_RUA);

//					lblap_statis(s_lblap_p->link_num, LBD_L2_RU);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RC);

					break;

				case 0x87 :							/*-----------------------*/
													/*       FRMR  Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_FRMRCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(s_lblap_p->cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* NO  (モジュロ  8)     */
													/* Total Length 5 以外 ? */
													/*-----------------------*/
						if(s_lblap_p->tr_adr.lap_buf_p->length != CMD_NUM5)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*    ABLOG_ABSU 処理    */
													/*     ABLOG 収集        */
													/* (異常S/Uフレーム長受信数) */
													/*-----------------------*/
							lblap_ablog(s_lblap_p->link_num, LBD_L2_ABSUF);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
							return;
						}
					}
					else
					{
													/*-----------------------*/
													/* NO  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						s_lblap_p->ctl_sv = LBD_ERRORX;
													/*-----------------------*/
													/*     ABLOG_UND 処理    */
													/*     ABLOG 収集        */
													/* ( 未定義フレーム受信数 )  */
													/*-----------------------*/
						lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
						return;
					}

													/*-----------------------*/
													/*   統計カウント処理    */
													/*    ( FRMR受信数)      */
													/*-----------------------*/
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RFMR);
					switch(s_lblap_p->ctl_sv)
					{
					    case LBD_ERRORW :
							lblap_statis(s_lblap_p->link_num, LBD_L2_RFRMW);
							break;
					    case LBD_ERRORX :
							lblap_statis(s_lblap_p->link_num, LBD_L2_RFRMX);
							break;
						case LBD_ERRORY :
							lblap_statis(s_lblap_p->link_num, LBD_L2_RFRMY);
							break;
						case LBD_ERRORZ :
							lblap_statis(s_lblap_p->link_num, LBD_L2_RFRMZ);
							break;
						default:
							break;

					}
													/*-----------------------*/
													/*     ABLOG 収集        */
													/*    ( FRMR受信数)      */
													/*-----------------------*/
					lblap_ablog(s_lblap_p->link_num, LBD_L2_ABRSF);

//					lblap_statis(s_lblap_p->link_num, LBD_L2_RU);
//					lblap_statis(s_lblap_p->link_num, LBD_L2_RC);

					break;

				default:							/*-----------------------*/
													/*         OTHERS        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					s_lblap_p->ctl_sv = LBD_ERRORW;
					lblap_ablog(s_lblap_p->link_num, LBD_L2_ABIFM);
					return;
			}

			break;
				
		default:
			break;
	}



}												/*  END OF  "lblap_chkctl"   */



/********************************************************************************************************/

/*!
 *  @brief   DISC  フレーム作成処理 (lblap_mkdisc)
 *  @note    DISCフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkdisc(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;

													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.own_adr;
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x53;
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;


													/*-----------------------*/
													/*   統計カウント処理    */
													/*   ( DISC 送信総数 )   */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SDSC);
	return;
}												/*  END OF  "lblap_mkdisc"   */


/********************************************************************************************************/

/*!
 *  @brief   UA フレーム作成処理 (lblap_mkua)
 *  @note    UAフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkua(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;

													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.per_adr;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/*   Poll/Final 1 ?      */
													/*-----------------------*/
	if(s_lblap_p->pf_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES                   */
													/* Contorl Field Set     */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x73;
	}
	else
	{
													/*-----------------------*/
													/* NO                    */
													/* Contorl Field Set     */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x63;
	}
													/*-----------------------*/
													/* Buffer Length/ttl Set */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*    ( UA 送信総数 )    */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SUA);
	return;
}												/*  END OF  "lblap_mkua"     */


/********************************************************************************************************/

/*!
 *  @brief   DMフレーム作成処理 (lblap_mkdm)
 *  @note    DMフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkdm(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;
													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.per_adr;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/*   Poll/Final 1 ?      */
													/*-----------------------*/
	if(s_lblap_p->pf_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES                   */
													/* Contorl Field Set     */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x1F;
	}
	else
	{
													/*-----------------------*/
													/* NO                    */
													/* Contorl Field Set     */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x0F;
	}
													/*-----------------------*/
													/* Buffer Length/ttl Set */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*    ( DM 送信総数 )    */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SDM);
	return;
}												/*  END OF  "lblap_mkdm"     */


/********************************************************************************************************/

/*!
 *  @brief   REJフレーム作成処理 (lblap_mkrej)
 *  @note    REJフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkrej(struct LBT_MNGTBL * s_lblap_p)
{
												/*****************************/
												/* 本モジュール専用 ローカル 変数*/
												/*****************************/
	union CTL_WK	work;                                   /*!< Control Field のBit   */
	union CTL_FLD1	*wk_ctl;                                /*!< */

													/* 合わせに使用する。    */

													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;
													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/* コマンド･レスポンス ワーク 1 ?  */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES ( Command Frame ) */
													/* 自局 アドレス 設定    */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.own_adr;
	}
	else
	{
													/*-----------------------*/
													/* NO (Response  Frame ) */
													/* 相手局 アドレス 設定  */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.per_adr;
	}

	wk_ctl = (union CTL_FLD1	*)&s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1;

												/*---------------------------*/
												/*  Set Control Field        */
												/*---------------------------*/

	wk_ctl->ctlms_8.ms81s = 0x09;
													/*-----------------------*/
													/*  P/F 値設定処理       */
													/*-----------------------*/
	work.work_all = s_lblap_p->pf_wk;
	wk_ctl->ctlms_8.ms8pf = work.others.field1; /* pgr0872 */

													/*-----------------------*/
													/*      N(R)  Set 処理   */
													/*-----------------------*/
	work.work_all = s_lblap_p->lap_dt.vr;
	wk_ctl->ctlms_8.ms8nr = work.wk_8.field3; /* pgr0872 */

													/*-----------------------*/
													/*     Length 操作       */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*    ( REJ 送信総数 )   */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SREJ);
	return;
}												/*  END OF  "lblap_mkrej"    */


/********************************************************************************************************/

/*!
 *  @brief   L1データ受信通知処理 (lblap_l2datpr)
 *  @note    L1データ受信通知受信時の処理を行う( I  Frame  時のみ有効)
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_l2datpr(struct LBT_MNGTBL * s_lblap_p)
{

													/*-----------------------*/
													/*                       */
													/* lblap_l2datpr 処理開始*/
													/*                       */
													/*-----------------------*/

													/*-----------------------*/
													/*  受信Buffer Adr Save  */
													/*-----------------------*/
	s_lblap_p->tx_buf_adr.lap_buf_p = s_lblap_p->tr_adr.lap_buf_p;

													/*-----------------------*/
													/*  L2-DATA-IND送出      */
													/*   To  Layer 3 tasks   */
													/*-----------------------*/
	tm_lb_DstrbtL3Msg(s_lblap_p);
	
	return;
}												/*  END OF  "lblap_l2datpr"  */


/********************************************************************************************************/

/*!
 *  @brief   異常状態クリア処理 (lblap_clrcnd)
 *  @note    ビジーフラグの解除処理を行う。
 *               -Reject状態フラグの解除処理を行う。
 *               -その他の状態フラグをクリアする処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_clrcnd(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   相手局ビジークリア  */
													/*-----------------------*/
	s_lblap_p->lap_dt.per_bsy = CMD_OFF;
													/*-----------------------*/
													/*    自局ビジークリア   */
													/*-----------------------*/
	s_lblap_p->lap_dt.own_bsy = CMD_OFF;
													/*-----------------------*/
													/*    Reject 状態クリア  */
													/*-----------------------*/
	s_lblap_p->lap_dt.rej_cond = CMD_OFF;
													/*--------------------------*/
													/* buffer type 1busy clear	*/
													/*--------------------------*/
	s_lblap_p->lap_dt.buftyp1_bsyflg = CMD_OFF;
													/*------------------------*/
													/* buffer type 2busy clear*/
													/*------------------------*/
	s_lblap_p->lap_dt.buftyp2_bsyflg = CMD_OFF;
													/*--------------------------*/
													/* buffer type 3busy clear	*/
													/*--------------------------*/
	s_lblap_p->lap_dt.buftyp3_bsyflg = CMD_OFF;
													/*--------------------------*/
													/* buffer type 4busy clear	*/
													/*--------------------------*/
	s_lblap_p->lap_dt.buftyp4_bsyflg = CMD_OFF;
	return;
}												/*  END OF  "lblap_clrcnd"   */



/********************************************************************************************************/

/*!
 *  @brief   T2タイマ停止処理 (lblap_stopt2)
 *  @note    T2タイマ(IフレームACK送出遅延タイマ)を停止させる処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル。T2カウンタクリア処理を行う。
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_stopt2(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/*    T2 Timer 使用 ?    */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t2_cnt != CMD_NUM0)
	{
													/*-----------------------*/
													/* T2タイマ停止処理      */
													/*-----------------------*/
		Result =	cm_TStop(s_lblap_p->t2_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T2 Timer Stop NG" );
		}
													/*-----------------------*/
													/* T2 Counter Clear      */
													/*-----------------------*/
		s_lblap_p->lap_dt.t2_cnt = CMD_NUM0;
	}
	return;
}												/*  END OF  "lblap_stopt2"   */


/********************************************************************************************************/

/*!
 *  @brief   状態変数クリア処理 (lblap_clrval)
 *  @note    V(S)､V(A),V(R)(状態変数)をクリア処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_clrval(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   送信状態変数クリア  */
													/*-----------------------*/
	s_lblap_p->lap_dt.vs = CMD_NUM0;
													/*-----------------------*/
													/*   受信状態変数クリア  */
													/*-----------------------*/
	s_lblap_p->lap_dt.vr = CMD_NUM0;
													/*-----------------------*/
													/*   応答確認変数クリア  */
													/*-----------------------*/
	s_lblap_p->lap_dt.va = CMD_NUM0;
	return;
}												/*  END OF  "lblap_clrval"   */


/********************************************************************************************************/

/*!
 *  @brief   Iフレームキュー強制廃棄処理 (lblap_disciq)
 *  @note    Iフレーム用キューにリンクしたフレームを強制廃棄する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_disciq(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	LBT_TSKIF_CPRICOM *i2_ref_p;                            /*!< I Frame Poinetr 2   */
	LBT_TSKIF_CPRICOM *i_ref_p;                             /*!< I Frame Pointer     */
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*                       */
													/* lblap_disciq  処理開始*/
													/*                       */
													/*-----------------------*/

													/*-----------------------*/
													/*  L1 Busy Flag ON ?    */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_ON)
	{
													/*-----------------------*/
													/*  YES  (L1 Busy)       */
													/*       Pointer Save    */
													/*-----------------------*/
		i2_ref_p = s_lblap_p->lap_dt.i_head2_p;
												/*---------------------------*/
												/*  バッファ解放処理開始     */
												/*---------------------------*/
		while(i2_ref_p != NULL)
		{
													/*-----------------------*/
													/* I Head Pointer 2 SAVE */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = i2_ref_p;
													/*-----------------------*/
													/* YES (C-LINK NULL)     */
													/*     I-Link更新        */
													/*-----------------------*/
			i2_ref_p = i2_ref_p->i_link;
													/*-----------------------*/
													/*  バッファ解放  !!     */
													/*-----------------------*/
			Result = cm_BFree((void *)s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "lblap_disciq",
							"Buffer FreeNG_1", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			lblap_statis(s_lblap_p->link_num, LBD_L2_IDSC);
			s_lblap_p->iframecnt--;
		}												/* LOOP END */
													/*-----------------------*/
													/* I Head = I Head 2 ?   */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.i_head_p == s_lblap_p->lap_dt.i_head2_p)
		{
													/*-----------------------*/
													/*  YES (Head = Head2)   */
													/*   Pointer Null化      */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p = NULL;
			s_lblap_p->lap_dt.i_head2_p = NULL;
			s_lblap_p->lap_dt.i_tail_p = NULL;
		}
		else
		{
													/*-----------------------*/
													/*  NO (Head != Head2)   */
													/*   Pointer SAVE        */
													/*-----------------------*/
			i_ref_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/*   I Linkの更新処理    */
													/*-----------------------*/
			while(i_ref_p->i_link != s_lblap_p->lap_dt.i_head2_p)
			{
				i_ref_p = i_ref_p->i_link;
			}
													/*-----------------------*/
													/*   Pointer Null化      */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head2_p = NULL;
													/*-----------------------*/
													/*      Tailの更新       */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_tail_p = i_ref_p;
													/*-----------------------*/
													/*   Pointer Null化      */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_tail_p->i_link = NULL;
		}
													/*-----------------------*/
													/*       Flag ON         */
													/*-----------------------*/
		s_lblap_p->lap_dt.dsc_iq_flg = CMD_ON;

	}
	else
	{
													/*-----------------------*/
													/*  NO   (L1 Not Busy)   */
													/*-----------------------*/
												/*---------------------------*/
												/* I Frame  解放処理開始     */
												/*---------------------------*/
		while(s_lblap_p->lap_dt.i_head_p != NULL)
		{
													/*-----------------------*/
													/*   I-Head Adr セーブ     */
													/*-----------------------*/
			s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;

													/*-----------------------*/
													/* I-Head の更新         */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_head_p =s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/* バッファ解放処理 !!!  */
													/*-----------------------*/
			Result = cm_BFree((void *)s_lblap_p->buf_adr.lap_buf_p);
			if(Result != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "lblap_disciq",
							"Buffer FreeNG_2", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
			}
			lblap_statis(s_lblap_p->link_num, LBD_L2_IDSC);
			s_lblap_p->iframecnt--;
		}
													/*-----------------------*/
													/*   Pointer Null化      */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = NULL;
		s_lblap_p->lap_dt.i_head2_p = NULL;
		s_lblap_p->lap_dt.i_tail_p = NULL;
	}
												/*---------------------------*/
												/* S Frame  解放処理開始     */
												/*---------------------------*/
	while(s_lblap_p->lap_dt.s_head_p != NULL)
	{
													/*-----------------------*/
													/* S Head Pointer SAVE   */
													/*-----------------------*/
		s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.s_head_p;
													/*-----------------------*/
													/* S Head Pointerの更新  */
													/*-----------------------*/
		s_lblap_p->lap_dt.s_head_p = s_lblap_p->lap_dt.s_head_p->i_link;
													/*-----------------------*/
													/* バッファ解放処理 !!!  */
													/*-----------------------*/
		Result = cm_BFree((void *)s_lblap_p->buf_adr.lap_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "lblap_disciq",
						"Buffer FreeNG_3", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
		}
	}
													/*-----------------------*/
													/*   Pointer Null化      */
													/*-----------------------*/
	s_lblap_p->lap_dt.s_head_p = NULL;
	s_lblap_p->lap_dt.s_tail_p = NULL;
	return;
}												/*  END OF  "lblap_disciq"   */



/********************************************************************************************************/

/*!
 *  @brief   Uフレーム送出処理 (lblap_sndu)
 *  @note    Uフレームを送出する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
void lblap_sndu(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*  L1 Busy Flag OFF ?   */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_OFF)
	{
													/*-----------------------*/
													/* YES ( L1 Not Busy )   */
													/* U Frame Queueing      */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->hnt_adr.lap_buf_p;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXU;
													/*--------------------------*/
													/*  L1データ送信要求 送出	*/
													/*             To  TXIOCS   */
													/*--------------------------*/
		s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcPQueueID = 0;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstPQueueID = 0;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
			CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
		}
#if 0
		/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//		lbw_l1BusyKey[s_lblap_p->link_num] = random();
//		Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//				&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//		if(Result != CMD_RES_OK)
//		{
//			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l1bsydettmr_id, "L1 busy Timer Start NG" );
//		}
#endif
													/*-----------------------*/
													/*   L1 Busy Flag ON     */
													/*-----------------------*/
		s_lblap_p->lap_dt.l1bsy_flg = CMD_ON;
													/*-----------------------*/
													/* 統計カウント処理      */
													/*    (TX-U Frame)       */
													/*    (TX-C Frame)       */
													/* (送信 Frame 総数)     */
													/*-----------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SU);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SC);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
													/*-----------------------*/
													/* 統計カウント処理1     */
													/*   (送信 Octet 総数)   */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,CMD_NUM4);
	}
	else
	{
													/*-----------------------*/
													/* NO  ( L1  Busy )      */
													/* U Head Pointer NULL ? */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.u_head_p == NULL)
		{
													/*-----------------------*/
													/* YES (Pointer NULL)    */
													/* U Frame Queueing      */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_head_p = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.u_tail_p = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.u_tail_p->i_link = NULL;
		}
		else
		{
													/*-----------------------*/
													/* NO  (Pointer EXIST)   */
													/* U Frame Queueing      */
													/*-----------------------*/
			s_lblap_p->lap_dt.u_tail_p->i_link = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.u_tail_p = s_lblap_p->lap_dt.u_tail_p->i_link;
			s_lblap_p->lap_dt.u_tail_p->i_link = NULL;
		}
	}
	return;
}												/*  END OF  "lblap_sndu"     */


/********************************************************************************************************/

/*!
 *  @brief   Sフレーム送出処理 (lblap_snds)
 *  @note    Sフレームを送出する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
void lblap_snds(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
													/*-----------------------*/
													/*  L1 Busy Flag OFF ?   */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_OFF)
	{
													/*-----------------------*/
													/* YES ( L1 Not Busy )   */
													/* S Frame Queueing      */
													/*-----------------------*/
		s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->hnt_adr.lap_buf_p;
		s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXS;
													/*--------------------------*/
													/*  L1データ送信要求 送出	*/
													/*             To  TXIOCS   */
													/*--------------------------*/
		s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
		s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
		s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
		s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
											/* LAPB走行履歴書込み	*/
		tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
		Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
		}
#if 0
		/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//		lbw_l1BusyKey[s_lblap_p->link_num] = random();
//		Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//				&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//		if(Result != CMD_RES_OK)
//		{
//			cm_Assert( CMD_ASL_DBGLOW, Result, "L1 busy Timer Start NG" );
//		}
#endif
													/*-----------------------*/
													/*   L1 Busy Flag ON     */
													/*-----------------------*/
		s_lblap_p->lap_dt.l1bsy_flg = CMD_ON;
													/*-----------------------*/
													/* 統計カウント処理      */
													/*    (TX-S Frame)       */
													/*    (TX-C Frame)       */
													/* (送信 Frame 総数)     */
													/*-----------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SS);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SC);
//		lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
													/*-----------------------*/
													/* 統計カウント処理1     */
													/*   (送信 Octet 総数)   */
													/*-----------------------*/
//		lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,CMD_NUM4);
	}
	else
	{
													/*-----------------------*/
													/* NO  ( L1  Busy )      */
													/* S Head Pointer NULL ? */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.s_head_p == NULL)
		{
													/*-----------------------*/
													/* YES (Pointer NULL)    */
													/* S Frame Queueing      */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_head_p = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.s_tail_p = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.s_tail_p->i_link = NULL;
		}
		else
		{
													/*-----------------------*/
													/* NO  (Pointer EXIST)   */
													/* S Frame Queueing      */
													/*-----------------------*/
			s_lblap_p->lap_dt.s_tail_p->i_link = s_lblap_p->hnt_adr.lap_buf_p;
			s_lblap_p->lap_dt.s_tail_p = s_lblap_p->lap_dt.s_tail_p->i_link;
			s_lblap_p->lap_dt.s_tail_p->i_link = NULL;
		}
	}
	return;
}												/*  END OF  "lblap_snds"     */


/********************************************************************************************************/

/*!
 *  @brief   Iフレーム送出処理 (lblap_sndi)
 *  @note    Iフレームを送出する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
void lblap_sndi(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/*  L1 Busy Flag OFF ?   */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.l1bsy_flg == CMD_OFF)
	{
													/*-----------------------*/
													/* YES ( L1 Not Busy )   */
													/* 相手局ビジー ?        */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.per_bsy == CMD_OFF)
		{
													/*-----------------------*/
													/* NO ( 相手局 Not Busy )*/
													/*V(S)=V(A)+K判定処理    */
													/*-----------------------*/
			lblap_vsvak(s_lblap_p);
													/*-----------------------*/
													/* 判定結果 0 ?          */
													/*-----------------------*/
			if(s_lblap_p->cond == CMD_NUM0)
			{
													/*-----------------------*/
													/* YES (判定結果 0)      */
													/*  I Frame 作成処理     */
													/*-----------------------*/
				lblap_mki(s_lblap_p);
													/*-----------------------*/
													/*   Buffer Queueing     */
													/*-----------------------*/
				s_lblap_p->tx_qu.qu_buf_p = s_lblap_p->lap_dt.i_head2_p;
													/*-----------------------*/
													/*  Flag Set (送信)      */
													/*-----------------------*/
				s_lblap_p->tx_qu.qu_ctl = LBD_L2_TXI;
													/*-----------------------*/
													/*  I Head Pointer2更新  */
													/*-----------------------*/
				s_lblap_p->lap_dt.i_head2_p = s_lblap_p->lap_dt.i_head2_p->i_link;
													/*--------------------------*/
													/*  L1データ送信要求 送出	*/
													/*             To  TXOICS   */
													/*--------------------------*/
				s_lblap_p->tx_qu.qu_buf_p->head.uiLength = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2;
				s_lblap_p->tx_qu.qu_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
				s_lblap_p->tx_qu.qu_buf_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
				s_lblap_p->tx_qu.qu_buf_p->head.uiDstTQueueID = CMD_TSKID_TXS;
											/* LAPB走行履歴書込み	*/
				tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_TXS, 
							CMD_TSKIF_L1DATTXREQ, (UINT *)s_lblap_p->tx_qu.qu_buf_p);
				Result = cm_Enter(CMD_TSKID_TXS, CMD_QRB_NORMAL, 
					CMD_TSKIF_L1DATTXREQ, (VOID *)s_lblap_p->tx_qu.qu_buf_p);
				if(Result != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, Result, "cm_Enter NG" );
				}
#if 0
//				/*Result = cm_TStat(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);*/
//				lbw_l1BusyKey[s_lblap_p->link_num] = random();
//				Result = cm_TStat_id(s_lblap_p->l1bsydettmr_id, s_lblap_p->lap_dt.l1bsytmrval, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd,
//						&s_lblap_p->l1bsydettmr_id2,&s_lblap_p->l1bsydettmr_osid,lbw_l1BusyKey[s_lblap_p->link_num]);
//				if(Result != CMD_RES_OK)
//				{
//					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->l1bsydettmr_id, "L1 busy Timer Start NG" );
//				}
#endif
													/*-----------------------*/
													/* 統計カウント処理      */
													/* (送信 Frame 総数)     */
													/* (TX-I Frame)          */
													/*-----------------------*/
//				lblap_statis(s_lblap_p->link_num, LBD_L2_TXF);
//				lblap_statis(s_lblap_p->link_num, LBD_L2_SI);
													/*-----------------------*/
													/* 統計カウント処理1     */
													/* (送信 Octet 総数)     */
													/*-----------------------*/
//				lblap_stats1(s_lblap_p->link_num, LBD_L2_TXO,(USHORT)(s_lblap_p->tx_qu.qu_buf_p->length + CMD_NUM2));
													/*-----------------------*/
													/* 統計カウント処理1     */
													/* (送信 Data Octet 総数)*/
													/*-----------------------*/
//				lblap_stats1(s_lblap_p->link_num, LBD_L2_SDTO,(USHORT)(s_lblap_p->tx_qu.qu_buf_p->length - CMD_NUM2));
													/*-----------------------*/
													/*      V(S) Inc 処理    */
													/*-----------------------*/
				lblap_vsinc(s_lblap_p);
													/*-----------------------*/
													/*   T2タイマ停止処理    */
													/*-----------------------*/
				lblap_stopt2(s_lblap_p);
													/*-----------------------*/
													/*    T1 Flag OFF ?      */
													/*-----------------------*/
				if(s_lblap_p->lap_dt.t1_flg == CMD_OFF)
				{
													/*-----------------------*/
													/* YES (T1 Flag OFF)     */
													/*  T1 Timer Restart     */
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
				}
													/*-----------------------*/
													/*  L1 Busy Flag ON      */
													/*-----------------------*/
				s_lblap_p->lap_dt.l1bsy_flg = CMD_ON;
			}
		}
	}
	return;
}												/*  END OF  "lblap_sndi"    */


/********************************************************************************************************/

/*!
 *  @brief   V(S)= V(A)+kチェック処理 (lblap_vsvak)
 *  @note    状態変数の値をチェックし結果を､呼びもとに返す処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_vsvak(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	UCHAR				no;                                 /*!< */
	union	CTL_FLD1	work1;                              /*!< Control Field 1 Work  */

													/*-----------------------*/
													/*  NO  (モジュロ  8)    */
													/* モジュロ 値 セット    */
													/*-----------------------*/
	no = CMD_NUM8;

													/*-----------------------*/
													/*  V(A)+k 値セーブ      */
													/*-----------------------*/
	work1.ctl1_all = (UCHAR)(s_lblap_p->lap_dt.va + s_lblap_p->lap_dt.ownk);
													/*-----------------------*/
													/*  モジュロ 計算        */
													/*-----------------------*/
	work1.ctl1_all %= no;

													/*-----------------------*/
													/*   V(S) = V(A)+k ?     */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.vs == work1.ctl1_all)
	{
													/*-----------------------*/
													/* YES  (V(S) = V(A)+k)  */
													/* 判定結果 1 Set        */
													/*-----------------------*/
		s_lblap_p->cond = CMD_NUM1;
		lblap_statis(s_lblap_p->link_num, LBD_L2_QUENUM);
	}
	else
	{
													/*-----------------------*/
													/* NO  (V(S) != V(A)+k)  */
													/* 判定結果 0 Set        */
													/*-----------------------*/
		s_lblap_p->cond = CMD_NUM0;
	}
	return;
}												/*  END OF  "lblap_vsvak"    */



/********************************************************************************************************/

/*!
 *  @brief   Iフレーム作成処理 (lblap_mki)
 *  @note    Iフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mki(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	union CTL_WK	work;                                   /*!< Control Field のBit   */
	union CTL_FLD1	*wk_ctl;                                /*!< */
													/* 合わせに使用する。    */

													/*-----------------------*/
													/* I Head Pointer 2 Save */
													/*-----------------------*/
	s_lblap_p->tx_buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head2_p;

													/*-----------------------*/
													/*   DISP/Length 操作    */
													/*-----------------------*/
	s_lblap_p->tx_buf_adr.lap_buf_p->link_num = s_lblap_p->link_num;
	s_lblap_p->tx_buf_adr.lap_buf_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->tx_buf_adr.lap_buf_p->length += CMD_NUM2;
	s_lblap_p->tx_buf_adr.disp += LBD_L2CTL_OFFSET;

												/*---------------------------*/
												/*     Set Control Field     */
												/*---------------------------*/

													/*-----------------------*/
													/*     Address Set       */
													/*-----------------------*/
	s_lblap_p->tx_buf_adr.lap_l2_p->address = s_lblap_p->lap_dt.own_adr;
	wk_ctl	=	(union CTL_FLD1	*)&s_lblap_p->tx_buf_adr.lap_l2_p->l2_cntl1;
	wk_ctl->ctli_8.i8zero = CMD_NUM0;

													/*-----------------------*/
													/*       N(S) Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->lap_dt.vs;
	wk_ctl->ctli_8.i8ns = work.wk_8.field3; /* pgr0872 */
													/*-----------------------*/
													/*          P/F Set (0)  */
													/*-----------------------*/
	wk_ctl->ctli_8.i8pf = CMD_NUM0;

													/*-----------------------*/
													/*       N(R) Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->lap_dt.vr;
	wk_ctl->ctli_8.i8nr = work.wk_8.field3;

	return;
}												/*  END OF  "lblap_mki"      */


/********************************************************************************************************/

/*!
 *  @brief   V(S)インクリメント処理 (lblap_vsinc)
 *  @note    V(S)カウンタ(送信状態変数)をインクリメントする処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void  lblap_vsinc(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	UCHAR			no;                                     /*!< */

													/*-----------------------*/
													/* モジュロ値セーブ      */
													/*-----------------------*/
	no = CMD_NUM8;
													/*-----------------------*/
													/* モジュロ値計算        */
													/*-----------------------*/
	s_lblap_p->lap_dt.vs = (UCHAR)((s_lblap_p->lap_dt.vs + CMD_NUM1) % no);
	return;
}												/*  END OF  "lblap_vsinc"   */


/********************************************************************************************************/

/*!
 *  @brief   SABMフレーム作成処理 (lblap_mksabm)
 *  @note    SABMフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mksabm(struct LBT_MNGTBL * s_lblap_p)
{
													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;

													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.own_adr;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/*   Set Control Field   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1 = 0x3F;
													/*-----------------------*/
													/*       Length 操作     */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*   ( SABM 送信総数 )   */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SSBM);
	return;
}												/*  END OF  "lblap_mksabm"   */


/********************************************************************************************************/

/*!
 *  @brief   RRフレーム作成処理 (lblap_mkrr)
 *  @note    RRフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkrr(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	union CTL_WK	work;                                   /*!< Control Field のBit   */
	union CTL_FLD1	*wk_ctl;                                /*!< */
													/* 合わせに使用する。    */

													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;
													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/* コマンド･レスポンス ワーク 1 ?  */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES   (Command Frame) */
													/*     Address Set       */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.own_adr;
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SRRF1);
	}
	else
	{
													/*-----------------------*/
													/* NO  (Response Frame)  */
													/*     Address Set       */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.per_adr;
//		lblap_statis(s_lblap_p->link_num, LBD_L2_SRRP1);
	}

	wk_ctl = (union CTL_FLD1	*)&s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1;
													/*-----------------------*/
													/* Set Control Field     */
													/*-----------------------*/
	wk_ctl->ctlms_8.ms81s = CMD_NUM1;

													/*-----------------------*/
													/*        P/F Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->pf_wk;
	wk_ctl->ctlms_8.ms8pf = work.others.field1; /* pgr0872 */

													/*-----------------------*/
													/*      N(R)  Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->lap_dt.vr;
	wk_ctl->ctlms_8.ms8nr = work.wk_8.field3; /* pgr0872 */

													/*-----------------------*/
													/*     Length 操作       */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
	return;
}												/*  END OF  "lblap_mkrr"     */


/********************************************************************************************************/

/*!
 *  @brief   RNRフレーム作成処理 (lblap_mkrnr)
 *  @note    RNRフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkrnr(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	union CTL_WK	work;                                   /*!< Control Field のBit   */
	union CTL_FLD1	*wk_ctl;                                /*!< */
													/* 合わせに使用する。    */

													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p = s_lblap_p->hnt_adr.lap_layer_p;
													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->link_num = s_lblap_p->link_num;
	s_lblap_p->xid_adr.lap_layer_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->xid_adr.lap_layer_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);
	s_lblap_p->xid_adr.lap_layer_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->xid_adr.lap_layer_p->head.uiDstTQueueID = 0;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->xid_adr.lap_layer_p->head.uiSrcTQueueID = 0;

													/*-----------------------*/
													/* コマンド･レスポンス ワーク 1 ?  */
													/*-----------------------*/
	if(s_lblap_p->cmd_rsp_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES   (Command Frame) */
													/*     Address Set       */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.own_adr;
	}
	else
	{
													/*-----------------------*/
													/* NO  (Response Frame) */
													/*     Address Set       */
													/*-----------------------*/
		s_lblap_p->xid_adr.lap_layer_p->l2_dt.address = s_lblap_p->lap_dt.per_adr;
	}

	wk_ctl = (union CTL_FLD1	*)&s_lblap_p->xid_adr.lap_layer_p->l2_dt.l2_cntl1;

													/*-----------------------*/
													/* Set Control Field     */
													/*-----------------------*/
	wk_ctl->ctlms_8.ms81s = 0x05;

													/*-----------------------*/
													/*        P/F Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->pf_wk;
	wk_ctl->ctlms_8.ms8pf = work.others.field1; /* pgr0872 */

													/*-----------------------*/
													/*      N(R)  Set        */
													/*-----------------------*/
	work.work_all = s_lblap_p->lap_dt.vr;
	wk_ctl->ctlms_8.ms8nr = work.wk_8.field3; /* pgr0872 */
													/*-----------------------*/
													/*     Length 操作       */
													/*-----------------------*/
	s_lblap_p->xid_adr.lap_layer_p->length = CMD_NUM2;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*    ( RNR 送信総数 )   */
													/*-----------------------*/
	lblap_statis(s_lblap_p->link_num, LBD_L2_SRNR);
	return;
}												/*  END OF  "lblap_mkrnr"    */



/********************************************************************************************************/

/*!
 *  @brief   FRMRフレーム作成処理 (lblap_mkfrmr)
 *  @note    FRMRフレームを作成する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_mkfrmr(struct LBT_MNGTBL * s_lblap_p)
{

													/*-----------------------*/
													/*   Hunt Buffer ADR Get */
													/*-----------------------*/
	s_lblap_p->layer2_adr.lap_buf_p = s_lblap_p->hnt_adr.lap_buf_p;

													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->layer2_adr.lap_buf_p->link_num = s_lblap_p->link_num;
	s_lblap_p->layer2_adr.lap_buf_p->disp = LBD_L2CTL_TXOFFSET;
	s_lblap_p->layer2_adr.lap_buf_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + CMD_NUM3 * sizeof(UCHAR);
	s_lblap_p->layer2_adr.lap_buf_p->head.uiEventNo = CMD_TSKIF_L1DATTXREQ;
	s_lblap_p->layer2_adr.lap_buf_p->head.uiDstPQueueID = CMD_TSKID_TXS;
	s_lblap_p->layer2_adr.lap_buf_p->head.uiDstTQueueID = 0;
	s_lblap_p->layer2_adr.lap_buf_p->head.uiSrcPQueueID = CMD_TSKID_LPB;
	s_lblap_p->layer2_adr.lap_buf_p->head.uiSrcTQueueID = 0;
													/*-----------------------*/
													/*  Buffer Address 変更  */
													/*-----------------------*/
	s_lblap_p->layer2_adr.disp += s_lblap_p->layer2_adr.lap_buf_p->disp;
													/*-----------------------*/
													/*    Buffer Value Set   */
													/*-----------------------*/
	s_lblap_p->layer2_adr.lap_dt_p->data[0] = s_lblap_p->lap_dt.per_adr;

													/*-----------------------*/
													/* Poll/Final Work 1 ?   */
													/*-----------------------*/
	if(s_lblap_p->pf_wk == CMD_NUM1)
	{
													/*-----------------------*/
													/* YES  (Poll/Final 1)   */
													/* Control Field Set     */
													/*-----------------------*/
		s_lblap_p->layer2_adr.lap_dt_p->data[1] = 0x97;
	}
	else
	{
													/*-----------------------*/
													/* NO   (Poll/Final 0)   */
													/* Control Field Set     */
													/*-----------------------*/
		s_lblap_p->layer2_adr.lap_dt_p->data[1] = 0x87;
	}
													/*-----------------------*/
													/*  FRMR DATA SET        */
													/*-----------------------*/

	s_lblap_p->layer2_adr.lap_dt_p->data[2] = s_lblap_p->fmr1_sav.fmr1[0];
	s_lblap_p->layer2_adr.lap_dt_p->data[3] = s_lblap_p->fmr1_sav.fmr1[1];
	s_lblap_p->layer2_adr.lap_dt_p->data[4] = s_lblap_p->fmr1_sav.fmr1[2];

													/*-----------------------*/
													/*     Length 操作       */
													/*-----------------------*/
	s_lblap_p->hnt_adr.lap_buf_p->length = CMD_NUM5;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*   ( FRMR 送信総数 )   */
													/*-----------------------*/
//	lblap_statis(s_lblap_p->link_num, LBD_L2_SFMR2);
	switch(s_lblap_p->ctl_sv)
	{
		case LBD_ERRORW :
			lblap_statis(s_lblap_p->link_num, LBD_L2_SFRMW);
			break;
		case LBD_ERRORX :
			lblap_statis(s_lblap_p->link_num, LBD_L2_SFRMX);
			break;
		case LBD_ERRORY :
			lblap_statis(s_lblap_p->link_num, LBD_L2_SFRMY);
			break;
		case LBD_ERRORZ :
			lblap_statis(s_lblap_p->link_num, LBD_L2_SFRMZ);
			break;
		default:
			break;
	
	}
	return;
}												/*  END OF  "lblap_mkfrmr"   */


/********************************************************************************************************/

/*!
 *  @brief   応答確認済Iフレーム廃棄処理 (lblap_freeak)
 *  @note    応答確認のとれたIフレームを解放する処理を行う。
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void lblap_freeak(struct LBT_MNGTBL * s_lblap_p)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	UCHAR		ack_cnt;                                    /*!< Ack Counter       */
	UCHAR		no;                                         /*!< Modulo 種別 Save Area */
	UINT			Result;                                 /*!< */
													/*-----------------------*/
													/*  Ack_cnt = N(R)-V(A)  */
													/*-----------------------*/
	ack_cnt = (UCHAR)(s_lblap_p->nr_sav - s_lblap_p->lap_dt.va);
													/*-----------------------*/
													/*  モジュロ値セット     */
													/*-----------------------*/
	no = CMD_NUM8;
													/*-----------------------*/
													/*    Modulo 計算        */
													/*-----------------------*/
	ack_cnt %= no;
													/*-----------------------*/
													/*  ACK_CNT = 0 ?        */
													/*-----------------------*/
	while(ack_cnt != CMD_NUM0)
	{
													/*-----------------------*/
													/* I Frame ポインタ NULL ?  */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.i_head_p == NULL)
		{
													/*-----------------------*/
													/* YES (Pointer NULL)    */
													/* I Tail ポインタ NULL     */
													/*-----------------------*/
			s_lblap_p->lap_dt.i_tail_p = NULL;
			break;
		}
													/*-----------------------*/
													/* NO (Ack_CNT 0以外)    */
													/* I Head Pointer SAve   */
													/*-----------------------*/
		s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->lap_dt.i_head_p;
													/*-----------------------*/
													/* I Frame ポインタ更新     */
													/*-----------------------*/
		s_lblap_p->lap_dt.i_head_p = s_lblap_p->lap_dt.i_head_p->i_link;
													/*-----------------------*/
													/*     バッファ廃棄処理  */
													/*-----------------------*/
		Result = cm_BFree((void *)s_lblap_p->buf_adr.lap_buf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "lblap_freeak",
						"Buffer FreeNG_1", Result, (UINT)s_lblap_p->buf_adr.lap_buf_p, CMD_NUM0);
		}
		s_lblap_p->iframecnt--;
													/*-----------------------*/
													/*       Ack_Cnt Dec     */
													/*-----------------------*/
		ack_cnt --;
													/*-----------------------*/
													/*   統計カウント処理    */
													/*  ( データ正常送信数 )   */
													/*-----------------------*/
//		lblap_statis(s_lblap_p->link_num, LBD_L2_DSD);
													/*-----------------------*/
													/*    Goto FRACK10       */
													/* ACK_CNT = 0 迄 Loop   */
													/*-----------------------*/
	}
	return;
}												/*  END OF  "lblap_freeak"   */

/* @} */


