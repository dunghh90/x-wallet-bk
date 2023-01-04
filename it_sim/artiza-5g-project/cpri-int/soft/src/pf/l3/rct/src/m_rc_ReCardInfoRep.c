/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_TrxSet.c
 *  @brief  REカード情報報告
 *  @date   2015/08/24 tdips)enoki
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "f_rrh_reg_cnt.h"

/******* ここからは配下REからのREカード情報報告応答受信処理 *********/
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告要求
 *  @note REカード情報報告要求
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/08/24 tdips)enoki
 *  @date   2021/02/23 M&C)Huan.dh add 3G and LTE handle for intermediate device 
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRepReq( VOID *parm_p )
{
	UINT	sigchk_rslt;					/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"m_rc_ReCardInfoRepReq validate signal NG");
	}

	/* 3G の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		//処理
		m_rc_ReCardInfoRepRspSend(CMD_OTHR_ERR_RE);
	}
	else
	{
		//処理
		m_rc_ReCardInfoRepReq_LTE( (CMT_TSKIF_CPRIRCV_CARDINFREQ*)parm_p );
	}
}


/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告要求(LTE)
 *  @note REカード情報報告要求
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2021/02/22 M&C)Huan.dh
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRepReq_LTE( CMT_TSKIF_CPRIRCV_CARDINFREQ* parm_p )
{
	USHORT				lwCpriNo;				//CPRI番号
	USHORT				lwReCnt;				//収集対象RE数
	//T_RRH_LAYER3		ltLayer3Sta_3g;			//3G Layaer3状態
	INT					liRet;					//
	//INT					liErrcod;				//
	//CHAR				lcBuf[128];				//ログ編集用

// [16B] add start
	UINT slotchk_rslt;				/* スロット番号チェック結果		*/
	T_RRH_SLOT_3G_RE	buf_3GslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
//	CHAR log_data[CMD_NUM64];		/* アサート情報収集用エリア		*/
	USHORT *slot_no;   /* スロット番号 */
	USHORT slot_no_max; /* スロット情報N */
    USHORT i;
// [16B] add end

	// 引数ポインタNULLチェック
	if( NULL == parm_p )
	{
		return;
	}

	// 	//REカード情報報告処理中のチェック
	// 	if( D_RRH_ON == rcw_CardInfoData_tbl.wRun )
	// 	{
	// 		cm_Assert( CMD_ASL_USELOW, 0, "m_rc_ReCardInfoRepReq_LTE already Processing" );

	// // [16B] add start
	//         // 本ルートが実施されるときは、m_rc_ReCardInfoRep_RspInit関数は実施済み
	//         // のため、ここでは実施しない。
	// 		/* 応答結果は、その他エラー(42)を返却 */
	// 	    rcw_CardInfoData_tbl.tCarInfo.result = CMD_OTHR_ERR_RE;

	// 		//REカード情報報告応答をRECに送信
	// 		//Call関数にて有効データ有無を判断する
	// 		m_rc_ReCardInfoRepRspSend();

	// // [16B] add end
	// 		return;
	// 	}

	//REカード情報報告管理テーブル　初期化
	//cm_MemClr( &rcw_CardInfoData_tbl, sizeof(RCT_CARD_INFO_DATA_T) );
	//m_rc_ReCardInfoRep_RspInit( &rcw_CardInfoData_tbl.tCarInfo );

	// 共用メモリから(3G)Layer3状態取得
	//f_cmn_com_layer3_get( CMD_SYS_3G, &ltLayer3Sta_3g );

// [16B] add start
	/************************/
	/* スロット番号チェック */
	/************************/
	slotchk_rslt = m_rc_cm_SlotNoChk(parm_p->cpridat_cardinfreq.slot_no);  //  1以外はNGを返却

	/* 戻り値判定 */
	if(slotchk_rslt != CMD_OK)
	{
		cm_Assert( CMD_ASL_USELOW, 0, "m_rc_ReCardInfoRepReq_LTE slot number is not 1." );

		/* 応答結果は、その他エラー(42)を返却 */
	    //rcw_CardInfoData_tbl.tCarInfo.result = CMD_OTHR_ERR_RE;

		//REカード情報報告応答をRECに送信
		//Call関数にて有効データ有無を判断する
		m_rc_ReCardInfoRepRspSend(CMD_OTHR_ERR_RE);
        return;
	}
// [16B] add end

	//REカード情報報告要求状態 ← 処理中
	//rcw_CardInfoData_tbl.wRun = D_RRH_ON;

	//Layer3状態が②RE起動中状態、または④運用中状態である各配下REがに
	//(3G)(0x5410)REカード情報報告要求を送信
	lwReCnt = 0;
	for( lwCpriNo=0 ; lwCpriNo < D_RRH_CPRINO_RE_MAX ; lwCpriNo++ )
	{
		// if(( E_RRH_LAYER3_RE_START != ltLayer3Sta_3g.layer3_re[lwCpriNo] )&&
		//    ( E_RRH_LAYER3_RE_OPE != ltLayer3Sta_3g.layer3_re[lwCpriNo] ))
		// {
		// 	continue;
		// }

		//配下REにREカード状態報告要求を送信
		parm_p->cprircv_inf.link_num = (lwCpriNo + D_RRH_CPRINO_RE_MIN);

// [16B] add start
		// 共有メモリから、該当ポートの3G スロット情報(RE側)を取得
		f_cmn_com_slot_3g_get(lwCpriNo+1,&buf_3GslotInfo_re);
		slot_no_max = buf_3GslotInfo_re.slot_num;
		slot_no = (USHORT *)buf_3GslotInfo_re.slot_no;

	    for(i=0; i < slot_no_max; i++) {

			parm_p->cpridat_cardinfreq.slot_no = *slot_no;
			slot_no++; // ポインタ更新

    		//配下REにREカード状態報告要求を送信
    		liRet = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, sizeof(*parm_p) );
    		if( D_RRH_OK == liRet )
    		{
    		// 	rcw_CardInfoData_tbl.wReList[lwCpriNo][i]	= D_RRH_ON;		//収集対象RE
    		 	lwReCnt++;

    		}
    		else
    		{
    		 	cm_Assert( CMD_ASL_USELOW, liRet, "->(re_rec) RECardInfoReportReq Send NG" );
    		}
		}
// [16B] add end
	}
// [16B] add start
    // REカード情報報告要求の送信数
    //rcw_CardInfoData_tbl.req_num = lwReCnt;
// [16B] add end

	//収集対象配下REなし
	if( 0 == lwReCnt )
	{
	    //配下RE未接続時は初期値で正常応答を送信
	    //rcw_CardInfoData_tbl.tCarInfo.result = CMD_NML;

		//初期値のREカード情報報告応答をRECに送信
		//Call関数にて有効データ有無を判断する
		m_rc_ReCardInfoRepRspSend(CMD_NML);

		//REカード情報報告要求状態 ←  未処理
		//rcw_CardInfoData_tbl.wRun = D_RRH_OFF;

		return;
	}

	//「REカード情報報告確認タイマ」開始
	//liRet = cm_TStat( CMD_TIMID_CARDINFOREP_3G, CMD_TIMVAL_RE_CARDINFOREP_CONF, CMD_TIMMD_SINGLE,
	//						CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &liErrcod );
	// if( liRet != CMD_RES_OK)
	// {
	// 	sprintf( &lcBuf[0], "REカード情報報告確認タイマ Start NG retval[0x%x] errcod[0x%x]", liRet,liErrcod );
	// 	cm_Assert( CMD_ASL_USELOW, CMD_TIMID_CARDINFOREP_3G, &lcBuf[0] );
	// }
}

/* @} */
/******* ここまではRECからのREカード情報報告要求受信処理 *********/

/******* ここからは配下REからのREカード情報報告応答受信処理 *********/
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告応答
 *  @note REカード情報報告応答
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/08/24 tdips)enoki
 *  @date   2021/02/23 M&C)Huan.dh intermediate forward RE response
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRepRsp( VOID *parm_p )
{
	UINT	sigchk_rslt;					/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "m_rc_ReCardInfoRepRsp validate signal NG");
	}

	/* 3G の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		/* 応答結果は、その他エラー(42)を返却 */
		m_rc_ReCardInfoRepRspSend(CMD_OTHR_ERR_RE);
		
	}
	/* LTE の場合 */
	else
	{
		m_cm_L3MsgSend_REC(parm_p);
	}
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告応答(3G)
 *  @note REカード情報報告応答
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/08/24 tdips)enoki
 *  @date   2015/10/26 TDI)satou FHM技説-QA-077
 *  @date   2015/12/01 FPT)Lay IT3問処No103の対応
 *  @date   2015/12/08 TDIPS)enoki 基本監視、拡張監視、制御項目のマスク
 *  @date   2016/02/22 TDIPS)enoki FHM技説1.0.5変更対応
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/********************************************************************************************************************/
// VOID m_rc_ReCardInfoRepRsp_3g( CMT_TSKIF_CPRIRCV_CARDINFRES* parm_p )
// {
// 	USHORT			lwCpriNo;			//CPRI番号
// 	CHAR			lcBuf[128];			//ログ編集用
// 	INT				liRet;
// 	INT				liErrcod;
// 	ULONG			llRscNum=0;
// 	ULONG			llRscNumNew=0;
// // [16B] add start
//     USHORT          slot;
// //	USHORT          i;
// 	USHORT          lwSlotNo;           //スロット番号
// //	T_RRH_SLOT_3G_RE	buf_3GslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
// //	USHORT *slot_no;   /* スロット番号 */
// //	USHORT slot_no_max; /* スロット情報N */
// // [16B] add end

// 	if( NULL == parm_p )
// 	{
// 		return;
// 	}

// 	//REカード情報報告応答待ち中のチェック
// 	if( D_RRH_ON != rcw_CardInfoData_tbl.wRun )
// 	{
// 		cm_Assert( CMD_ASL_USELOW, 0, "m_rc_ReCardInfoRepRsp_3g Not Processing... " );
// 		return;
// 	}

// 	//CPRI番号取り出し
// 	lwCpriNo = parm_p->cprircv_inf.link_num;
// 	if(( D_RRH_CPRINO_RE_MIN > lwCpriNo )||( D_RRH_CPRINO_RE_MAX < lwCpriNo ))
// 	{
// 		cm_Assert( CMD_ASL_USELOW, lwCpriNo, "m_rc_ReCardInfoRepRsp_3g  RE# Range Out" );
// 		return;
// 	}

// // [16B] add start
//     // スロット番号取り出し
//     lwSlotNo = parm_p->cpridat_cardinfres.slot_no;
// 	if(( CMD_MINSLOTNO > lwSlotNo )||( CMD_MAXSLOTNO < lwSlotNo ))
// 	{
// 		cm_Assert( CMD_ASL_USELOW, lwSlotNo, "m_rc_ReCardInfoRepRsp_3g  RE# slot_no Range Out" );
// 		return;
// 	}

//     // スロット番号から何番スロット(テーブルindex値)か抽出(3G)
//     liRet = f_cmn_com_slotsrch_3g(lwCpriNo, lwSlotNo, &slot);
//     if (D_RRH_OK != liRet) {
//         BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:SlotNo(%d) NG", lwSlotNo);
//         return;
//     }

// //	// 共有メモリから、該当ポートの3G スロット情報(RE側)を取得
// //	f_cmn_com_slot_3g_get(lwCpriNo,&buf_3GslotInfo_re);
// //	slot_no_max = buf_3GslotInfo_re.slot_num;
// //    i=0;
// //    for(; i < slot_no_max; i++) {
// //       if( lwSlotNo == buf_3GslotInfo_re.slot_no[i] ) {
// //          break;
// //       }
// //    }
// //    // スロット番号の一致の確認(一致するスロット番号がない場合)
// //    if(i >= slot_no_max) {
// //        cm_Assert( CMD_ASL_USELOW, lwSlotNo, "m_rc_ReCardInfoRepRsp_3g  RE# slot_no invalid" );
// //		return;
// //    }
// // [16B] add end

// 	//該当REの応答待ち中かを判定
// 	if( D_RRH_ON != rcw_CardInfoData_tbl.wReList[lwCpriNo-1][slot] )
// 	{
// 		cm_Assert( CMD_ASL_USELOW, lwCpriNo, "m_rc_ReCardInfoRepRsp_3g  RE Not Wait Response" );
// 		return;
// 	}
// 	//受信済のため、応答待ちなしを設定
// 	rcw_CardInfoData_tbl.wReList[lwCpriNo-1][slot] = D_RRH_OFF;

// // [16B] add start
//     // REカード情報報告要求の送信数を減算
//     rcw_CardInfoData_tbl.req_num--;
// // [16B] add end

// 	//応答結果判定
// 	if( CMD_RES_OK == parm_p->cpridat_cardinfres.result )
// 	{
// 		//配下REから収集した有効データ有無を判定
// 		if( D_RRH_OFF == rcw_CardInfoData_tbl.wDataValid )
// 		{
// 			//初回保存
// 			rcw_CardInfoData_tbl.tCarInfo.result			= CMD_NML;
// 			rcw_CardInfoData_tbl.tCarInfo.up_resrc_num1		= parm_p->cpridat_cardinfres.up_resrc_num1;
// 			rcw_CardInfoData_tbl.tCarInfo.up_resrc_num2		= parm_p->cpridat_cardinfres.up_resrc_num2;
// 			rcw_CardInfoData_tbl.tCarInfo.down_resrc_num1	= parm_p->cpridat_cardinfres.down_resrc_num1;
// 			rcw_CardInfoData_tbl.tCarInfo.down_resrc_num2	= parm_p->cpridat_cardinfres.down_resrc_num2;
// 			rcw_CardInfoData_tbl.tCarInfo.card_svitem		= (parm_p->cpridat_cardinfres.card_svitem & 0x07A0);
// 			rcw_CardInfoData_tbl.tCarInfo.card_extitem		= (parm_p->cpridat_cardinfres.card_extitem & 0x8002);
// 			rcw_CardInfoData_tbl.tCarInfo.card_cntitem		= (parm_p->cpridat_cardinfres.card_cntitem & 0x3001);

// 			//配下REから収集した有効データありに設定
// 			rcw_CardInfoData_tbl.wDataValid = D_RRH_ON;
// 		}
// 		else
// 		{
// 			//上りリソース数：配下REの中で最小のものを設定 */
// 			llRscNum     = (rcw_CardInfoData_tbl.tCarInfo.up_resrc_num1 << 16);
// 			llRscNum    += (rcw_CardInfoData_tbl.tCarInfo.up_resrc_num2);
// 			llRscNumNew  = (parm_p->cpridat_cardinfres.up_resrc_num1 << 16);
// 			llRscNumNew += (parm_p->cpridat_cardinfres.up_resrc_num2);
// 			if( llRscNum > llRscNumNew )
// 			{
// 				rcw_CardInfoData_tbl.tCarInfo.up_resrc_num1 = parm_p->cpridat_cardinfres.up_resrc_num1;
// 				rcw_CardInfoData_tbl.tCarInfo.up_resrc_num2 = parm_p->cpridat_cardinfres.up_resrc_num2;
// 			}

// 			//下りリソース数：配下REの中で最小のものを設定 */
// 			llRscNum     = (rcw_CardInfoData_tbl.tCarInfo.down_resrc_num1 << 16);
// 			llRscNum    += (rcw_CardInfoData_tbl.tCarInfo.down_resrc_num2);
// 			llRscNumNew  = (parm_p->cpridat_cardinfres.down_resrc_num1 << 16);
// 			llRscNumNew += (parm_p->cpridat_cardinfres.down_resrc_num2);
// 			if( llRscNum > llRscNumNew )
// 			{
// 				rcw_CardInfoData_tbl.tCarInfo.down_resrc_num1 = parm_p->cpridat_cardinfres.down_resrc_num1;
// 				rcw_CardInfoData_tbl.tCarInfo.down_resrc_num2 = parm_p->cpridat_cardinfres.down_resrc_num2;
// 			}

// 			//受信データ集約して保存
// 			rcw_CardInfoData_tbl.tCarInfo.card_svitem	|= (parm_p->cpridat_cardinfres.card_svitem & 0x07A0);
// 			rcw_CardInfoData_tbl.tCarInfo.card_extitem	|= (parm_p->cpridat_cardinfres.card_extitem & 0x8002);
// 			rcw_CardInfoData_tbl.tCarInfo.card_cntitem	|= (parm_p->cpridat_cardinfres.card_cntitem & 0x3001);
// 		}
		
// 	}
// 	else
// 	{
// 		sprintf( &lcBuf[0], "m_rc_ReCardInfoRepRsp_3g Result NG[0x%x]", parm_p->cpridat_cardinfres.result );
// 		cm_Assert( CMD_ASL_USELOW, lwCpriNo, &lcBuf[0] );
// 	}

// // [16B] add start
//     // 他に応答待ちの有無を判断
//     if ( rcw_CardInfoData_tbl.req_num != 0 ) {
//         return;
//     }
// // [16B] add end
	
// #if 0  // [16B] del start
// 	//他に応答の有無を判定 
// 	// ⇒ 「rcw_CardInfoData_tbl.req_num」で他の応答待ち有無の判断を実施するため削除
// 	for( lwCpriNo=0 ; lwCpriNo < D_RRH_CPRINO_RE_MAX ; lwCpriNo++ )
// 	{
//         for( i=0 ; i < 16 ; i++ )
//         {
// 	    	if( D_RRH_ON == rcw_CardInfoData_tbl.wReList[lwCpriNo][i] )
// 	    	{
// 	    		//他に応答待ちREありのため終了
// 	    		return;
// 	    	}
//         }
// 	}
// #endif // [16B] del end

// 	//---すべての受信待ち対象REからの応答受信済み

// 	//応答待ちタイマ停止
// 	liRet = cm_TStop( CMD_TIMID_CARDINFOREP_3G, &liErrcod );
// 	if( CMD_RES_OK != liRet )
// 	{
// 		sprintf( &lcBuf[0], "REカード情報報告確認タイマ Stop NG retval[0x%x] errcod[0x%x]", liRet,liErrcod );
// 		cm_Assert( CMD_ASL_USELOW, CMD_TIMID_CARDINFOREP_3G, &lcBuf[0] );
// 	}

// 	//REカード情報報告応答をRECに送信
// 	m_rc_ReCardInfoRepRspSend();

// 	//REカード情報報告要求状態 ←  未処理
// 	rcw_CardInfoData_tbl.wRun = D_RRH_OFF;
// }
/* @} */
/******* ここまでは配下REからのREカード情報報告応答受信処理 *********/

/******* ここからは配下REからのREカード情報報告応答 タイムアウト ********/
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告応答(3G)待ちタイムアウト
 *  @note   REカード情報報告応答(3G)待ちタイムアウト
 *  @param  awTimerId [in] timer id
 *  @return None 
 *  @Bug_No M-RRU-ZSYS-01777
 *  @date   2015/08/24 tdips)enoki
 *  @date   2015/10/22 FPT)Quynh M-RRU-ZSYS-01777 Fix bug IT2 (BUG_082_IT2_20-06-04_20-06-03)
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/********************************************************************************************************************/
// VOID m_rc_ReCardInfoRepRspTimeout_3g( USHORT awTimerId )
// {
//     USHORT lwCpriNo;  // リンク番号ループ
//     USHORT i;         // スロット番号ループ

// 	if( CMD_TIMID_CARDINFOREP_3G != awTimerId )
// 	{
// 		return;
// 	}

// 	//REカード情報報告応答待ち中のチェック
// 	if( D_RRH_ON != rcw_CardInfoData_tbl.wRun )
// 	{
// 		cm_Assert( CMD_ASL_USELOW, 0, "m_rc_ReCardInfoRepRspTimeout_3g Not Processing... " );
// 		return;
// 	}

// 	//REカード情報報告応答をRECに送信
// 	m_rc_ReCardInfoRepRspSend();

// 	//REカード情報報告要求状態 ←  未処理
// 	rcw_CardInfoData_tbl.wRun = D_RRH_OFF;

// // [16B] add start
//     // 収集対象REリストをクリア
// 	for( lwCpriNo=0 ; lwCpriNo < D_RRH_CPRINO_RE_MAX ; lwCpriNo++ )
// 	{
//         for( i=0 ; i < CMD_MAX_CARDNUM ; i++ ) {
//     		rcw_CardInfoData_tbl.wReList[lwCpriNo][i] = D_RRH_OFF;
//         }
// 	}

//     // REカード情報報告要求の送信数を初期化(0)
//     rcw_CardInfoData_tbl.req_num = 0;
// // [16B] add end
// }
/* @} */
/******* ここまでは配下REからのREカード情報報告応答 タイムアウト ********/

/******* ここからは部品関数群 ********/
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告応答初期値設定
 *  @note  REカード情報報告応答初期値設定
 *  @param  atpRspMsg [out] REカード情報報告応答
 *  @return None
 *  @date   2015/08/24 tdips)enoki
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRep_RspInit( CMT_CPRIF_CARDINFRES* atpRspMsg )
{
	if( NULL == atpRspMsg )
	{
		return;
	}

	//初期値設定
	atpRspMsg->signal_kind		= CMD_CPRID_CARDINFRES;
	atpRspMsg->result			= CMD_OTHR_ERR_RE;		//その他ERR
	atpRspMsg->slot_no			= 1;
	strcpy( (char*)&atpRspMsg->meker_name[0], (const char*)"FUJITSU" );
	m_rc_ReCardInfoRep_GetCardName( &rcw_CardInfoData_tbl.tCarInfo.card_name[0] );
	atpRspMsg->up_resrc_num1	= 0x0000;
	atpRspMsg->up_resrc_num2	= 0x0001;
	atpRspMsg->down_resrc_num1	= 0x0000;
	atpRspMsg->down_resrc_num2	= 0x0001;
	atpRspMsg->card_svitem		= 0x07A0;	//D10:USE、D9:ALM、D8:ERR、D7:実装、D5:ACT
	atpRspMsg->card_extitem		= 0x8002;	//D15:AMP部電源断、D1:WarmUp
	atpRspMsg->card_cntitem		= 0x3001;	//D13:ローカルリセット、D12:自律リセット、D0:リモートリセット
	atpRspMsg->funcitem			= 0x0000;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  カード名決定処理
 *  @note  カード名決定処理
 *  @param  acpCardName [out] カード名(NULL含め8文字以内の文字列)
 *  @return None
 *  @date   2015/08/24 tdips)enoki create
 *  @date   2016/02/26 tdips)enoki FHM技説1.0.5仕様変更 カード名決定方法変更
 *  @date   2016/03/01 tdips)enoki FHM技説1.0.5仕様変更 カード名決定方法変更 1.5Gの場合"15GRR2A"とする
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRep_GetCardName( UCHAR* acpCardName )
{
	USHORT				lwFreqBand=D_RRH_CRP_FRQ_2G;
	USHORT				lwCnt;
	USHORT				lwMaxCnt;
	USHORT				lwEqual;

	if( NULL == acpCardName )
	{
		return;
	}

	/* 周波数帯指定数の最大数とその周波数帯を求める */
	lwMaxCnt = 0;
	for( lwCnt=0 ; lwCnt <= D_RRH_CRP_FRQ_MAXVAL ; lwCnt++ )
	{
		if( lwMaxCnt < gw_rcw_FreqBandCounter3G[lwCnt] )
		{
			lwMaxCnt = gw_rcw_FreqBandCounter3G[lwCnt];
			lwFreqBand = lwCnt;
		}
	}

	/* 最大数と同じ周波数帯が他にもあるか？ */
	lwEqual = 0 ;
	for( lwCnt=0 ; lwCnt <= D_RRH_CRP_FRQ_MAXVAL ; lwCnt++ )
	{
		if( lwMaxCnt == gw_rcw_FreqBandCounter3G[lwCnt] )
		{
			lwEqual++;
		}
	}
	if( lwEqual > 1 )
	{
		/* 最大数と同じものがある場合は、周波数帯を2Gにする */
		lwFreqBand = D_RRH_CRP_FRQ_2G;
	}

	//「対応周波数帯」をもとにカード名を決定
	switch( lwFreqBand )
	{
	case D_RRH_CRP_FRQ_2G:	//2GHz帯
		strcpy( (char*)acpCardName, (const char*)"2GRR2A" );
		break;
	case D_RRH_CRP_FRQ_800M:	//800MHz帯
		strcpy( (char*)acpCardName, (const char*)"800RR2A" );
		break;
	case D_RRH_CRP_FRQ_17G:	//1.7GHz帯
		strcpy( (char*)acpCardName, (const char*)"17GRR2A" );
		break;
	case D_RRH_CRP_FRQ_15G:	//1.5GHz帯
		strcpy( (char*)acpCardName, (const char*)"15GRR2A" );
		break;
	default :
		strcpy( (char*)acpCardName, (const char*)"2GRR2A" );
		break;
	}
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  REカード情報報告応答送信
 *  @note REカード情報報告応答送信
 *  
 *  @return None
 *  @date   2021/02/22 M&C)Huan.dh create response to REC
 */
/********************************************************************************************************************/
VOID m_rc_ReCardInfoRepRspSend( USHORT result )
{
	CMT_TSKIF_CPRIRCV_CARDINFRES*	ltpRspMsg=NULL;
	UINT			luiRet;


    //バッファ獲得
    luiRet = cm_L2BfGet(	CMD_BUFCA_LAPSND,
                                sizeof(CMT_TSKIF_CPRIRCV_CARDINFRES),
                                CMD_NUM1,
                                (VOID **)&ltpRspMsg	);
    if( CMD_RES_OK != luiRet )
    {
        cm_Assert(CMD_ASL_USELOW, luiRet, "m_rc_ReCardInfoRepRspSend cm_L2BfGet NG" );
        return;
    }

	//タスク間ヘッダ作成
	ltpRspMsg->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_CARDINFRES);
	
	ltpRspMsg->cpridat_cardinfres.result = result;
	
	//CPRI信号をLAPBタスクに送信
	luiRet = m_cm_L3MsgSend_REC( ltpRspMsg );
	if( CMD_L3_OK != luiRet )
	{
        cm_Assert(CMD_ASL_USELOW, luiRet, "m_rc_ReCardInfoRepRspSend REC Send NG" );
	}
}
/* @} */
/******* ここまではRECへのREカード情報報告応答送信処理 ********/

/* @} */
