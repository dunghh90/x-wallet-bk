/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReSlotRep.c
 *  @brief  response to RE slot info report request, carry out RE slot info report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2013/11/27 FFCS)Xut modify for ZYNQ.
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE slot info report process
 *  @note     RE slot info report process
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date     2015/09/27 TDI)satou エンディアン変換処理を削除(L2プロセスでエンディアン変換するため)
 *  @date     2016/10/04 KCN)hfuku 16B機能追加
 *  @date     2021/01/27 M&C)Huan.dh return NG for 3G. Pre-allocated slot numbers for the intermediate 
 */
/********************************************************************************************************************/
VOID m_rc_ReSlotRep(VOID* parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	CMT_TSKIF_CPRISND_SLOTINFRES *rsp_p;	/* タスク間フレームポインタ		*/
	UINT   datalength;						/* データ長						*/
	UINT   secno;							/* セクタ番号					*/
	UINT   bufget_rslt;						/* バッファ獲得結果				*/
	USHORT set_data_cnt = CMD_NUM0;			/* 設定データカウンタ			*/
	USHORT cardinsplc;						/* カード実装位置				*/
	USHORT set_data[CMD_NUM1	+			/* スロット情報数N				*/
					CMD_NUM8	+			/* スロット番号					*/
					CMD_NUM8	];			/* REカード機能部情報			*/
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	//T_RRH_SLOT_3G		ltSlotInf3g;
	//T_RRH_SLOT_S3G		ltSlotInfLte;
	USHORT				lwCardInf = CMD_NUM0;
	USHORT		lwCnt;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	USHORT lwSignal;
	USHORT lwResult;
#endif
	
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&lwSigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

	/* ポインタ初期化 */
	rsp_p = NULL;

	/* 可変長設定用データ	*/
	cm_MemClr(set_data, sizeof(set_data));

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_SLOTINFRES)+	/* 応答結果まで	*/
								sizeof(set_data)					,
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_ReSlotRep cm_L2BfGet NG");
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;

	}
	
	/* CPRI信号種別設定 */
	rsp_p->cpridat_slotinfres.signal_kind = CMD_CPRID_SLOTINFRES + lwSigsys_rslt;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1	
	lwSignal = rsp_p->cpridat_slotinfres.signal_kind;
	lwResult = rsp_p->cpridat_slotinfres.result;
#endif

	/**************************/
	/* CPRI信号応答データ設定 */
	/**************************/

	/**************/
	/* OF-TRX構成 */
	/**************/
	cardinsplc = CMD_NUM1;
	
	/* セクタ番号算出 */
	secno = cardinsplc;
	
	/*----------------*/
	/* SLOT情報数設定 */
	/*----------------*/
	/* スロット情報数設定(1CPRIリンク配下のMAXカード数(1)を設定) */
//	set_data[set_data_cnt] = CMD_MAX_CARDNUM; /* 16B:CMD_MAX_CARDNUMは16に変更して、別の用途で使用する */
	set_data[set_data_cnt] = CMD_NUM1;
	set_data_cnt++;

	/*--------------------*/
	/* MDIFのSLOT番号設定 */
	/*--------------------*/
	/* 取得したセクタ番号から、取り得る各カードのスロット番号を設定する */
	/* MDIFのスロット番号設定 「獲得セクタ番号」を設定*/
	set_data[set_data_cnt] = (USHORT)secno;
	set_data_cnt++;

	/*------------------------*/
	/* REカード機能部情報設定 */
	/*------------------------*/

	/* 3G systerm */
	if( CMD_SYS_3G == lwSigsys_rslt )
	{
		// cm_MemClr(&ltSlotInf3g, sizeof(T_RRH_SLOT_3G));
		// /* 共有メモリ(3G)スロット情報の呼び出し取得用 */
		// f_cmn_com_bootslot_3g_get(&ltSlotInf3g);
		// lwCardInf = ltSlotInf3g.card_inf;

		/* Intermediate set NG for CPRI signal response result */
		rsp_p->cpridat_slotinfres.result = CMD_HDL_NG;

		set_data[set_data_cnt] = lwCardInf;
		set_data_cnt++;

		/* 応答データに可変長データを設定 */
		cm_MemCpy(&(rsp_p->cpridat_slotinfres.result) + CMD_NUM1, 
				set_data, 
				sizeof(USHORT) * (set_data_cnt));

		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/

		/****************/
		/* データ長設定 */
		/****************/
		datalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
						+	sizeof(CMT_CPRIF_SLOTINFRES)	/* 信号種別～キャリア情報	*/
						+	(set_data_cnt * CMD_NUM2));		/* スロット情報数 +			*/
															/* スロット情報   +			*/
															/* REカード機能部情報		*/

		/* 応答信号にレングス長設定 */
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;
		
		/************/
		/* 応答送信 */
		/************/
		m_cm_L3MsgSend_REC(rsp_p);

	}
	/* LTE systerm */
	else
	{
		// cm_MemClr(&ltSlotInfLte, sizeof(T_RRH_SLOT_S3G));
		// /* 共有メモリ(LTE)スロット情報の呼び出し取得用 */
		// f_cmn_com_bootslot_s3g_get(&ltSlotInfLte);
		// lwCardInf = ltSlotInfLte.card_inf;
		
		/*  CPRIリンク数だけループループ */
		for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
		{
			((CMT_TSKIF_CPRIRCV_SLOTINFREQ *)parm_p)->cprircv_inf.link_num = lwCnt + CMD_NUM1;
			/* l3/rctからの正常メッセージをre/recに送信する(REスロット情報報告要求) */
			l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIRCV_SLOTINFREQ ) );
		}
	}

	
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s System type :[%d] \n Signal kind : [%d] \n Result: [%d] \n ",__LINE__,__FUNCTION__,lwSigsys_rslt,lwSignal,lwResult);
	printf( "[%d]%s end \n ",__LINE__,__FUNCTION__);
#endif
	
	return ;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get RE card function info process
 *  @note     Get RE card function info process
 *  @return   RE card function info
 *  @date     2015/04/21 FFCS)Zhangxq modify for TDD.
 */
/********************************************************************************************************************/
USHORT m_rc_GetReCardFbBits()
{
	/* For TDD SRE  BPF/SWITCH LNA T-PA TRX TRX-INF PORT-INF MT-INF  is on */
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return rcr_cardFb_tdd_sre.cardFbBit;
}
/* @} */


/* @} */

/********************************************************************************************************************/
/**
 *  @brief  Receive slot info from re/rec
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 2017/01/25 M&C)Huan.dh forward CARONOFF from RE response to REC
 */
/********************************************************************************************************************/

VOID m_rc_SlotInf_Rcv_Rsp( CMT_TSKIF_CPRISND_SLOTINFRES* parm_p  )
{
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* 信号を有効にしてください。 */
	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"m_rc_SlotInf_Rcv_Rsp validate signal NG");
		return;
	}

	/* 3G信号の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		parm_p->cpridat_slotinfres.result = CMD_OTHR_ERR_RE;
	}
	
	/* l3/rctからの正常メッセージをRECに送信する(REスロット情報報告要求) */
	m_cm_L3MsgSend_REC( parm_p );
}

/* @} */

/* @{ */
