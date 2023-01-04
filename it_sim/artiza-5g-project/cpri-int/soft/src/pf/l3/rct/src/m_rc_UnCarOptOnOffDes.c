/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_UnCarOptOnOffDes.c
 *  @brief  無変調キャリア出力ON/OFF指定関数
 *  @date   2015/08/11 TDIPS)H.Murata
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
#include "m_rc_def.h"
#include "m_rc_ftyp.h"


/********************************************************************************************************************/
/**
*  @brief	無変調キャリア出力ON/OFF指定応答関数(LTE)
 *  @note
 *  @param	parm_p        [in] 無変調キャリア出力ON/OFF指定要求
 *  @param	awRsp_rslt    [in] 応答結果値
 *  @return	None
 *  @date	2015/08/11 TDIPS)H.Murata
 *  @date	2021/02/04 M&C)Huan.dh forward CARONOFF request to RE
 */
/********************************************************************************************************************/
VOID m_rc_UnCarOptOnOffDes_lte_Res( CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ *parm_p, USHORT awRsp_rslt )
{
	// CMT_TSKIF_CPRISND_CARONOFFRES_S3G *rsp_p;
	// UINT		bufget_rslt;					/* バッファ獲得結果(LTE)		*/
	// UINT		Snd_ret;						/* Return value of sending(LTE) */
	USHORT			lwCnt;
	T_RRH_LAYER3	lwLayer3Sts;
	
	// /****************/
	// /* バッファ獲得 */
	// /****************/
	// /* 最大値のバッファを獲得 */
	// 	rsp_p	 = NULL;
	// 	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
	// 									sizeof(CMT_TSKIF_CPRISND_CARONOFFRES_S3G),
	// 									CMD_NUM1,
	// 									(VOID **)&rsp_p	);

	// /********************/
	// /* バッファ獲得失敗 */
	// /********************/
	// if(bufget_rslt != CMD_RES_OK){

	// 	/* ASSERT */
	// 	cm_Assert(	CMD_ASL_USELOW,
	// 				bufget_rslt,
	// 				"m_rc_UnCarOptOnOffDes_lte_Res cm_L2Bf_LTE_Get NG");
	// 	return;
	// }

	// /* 無変調キャリア出力ON/OFF指定応答 作成 */
	// rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_CARONOFFRES_S3G);

	// 	/* キャリア番号 */
	// rsp_p->cpridat_caronoffres.carno =  parm_p->cpridat_caronoffreq.carno;
	
	// 	/* CPRI信号応答内容(CPRI信号種別)設定 */
	// rsp_p->cpridat_caronoffres.signal_kind = CMD_CPRID_CARONOFFRES + CMD_SYS_S3G;

	// 	/* CPRI信号応答結果にNG(RE)(51)を設定 */
	// rsp_p->cpridat_caronoffres.result = awRsp_rslt;

	// /* 無変調キャリア出力ON/OFF指定応答 送信 */
	// /* LTE */
	// Snd_ret = m_cm_L3MsgSend_REC( rsp_p );
	// if( Snd_ret != CMD_L3_OK )
	// {
	// 	cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
	// 							"m_rc_UnCarOptOnOffDes_lte_Res m_cm_L3MsgSend_REC NG" );
	// }

	/* Layer3状態取得 */
	f_cmn_com_layer3_get(CMD_SYS_S3G, &lwLayer3Sts);

	/*  CPRIリンク数だけループループ */
	for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	{
		if( E_RRH_LAYER3_RE_OPE == lwLayer3Sts.layer3_re[lwCnt])
		{
			((CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ*)parm_p)->cprircv_inf.link_num = lwCnt + CMD_NUM1;
			/* l3/rctからの正常メッセージをre/recに送信する(無変調キャリア出力ON/OFF指定要求) */
			printf("\n[INFO] [L3] [%s:%d] Before sending message to RE. Link num = %d, signal kind = 0x%x", __FILE__, __LINE__, parm_p->cprircv_inf.link_num, parm_p->cpridat_caronoffreq.signal_kind);
			l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ ) );
		}
	}
	return;
}
/********************************************************************************************************************/
/**
*  @brief	無変調キャリア出力ON/OFF指定応答関数(3G)
 *  @note
 *  @param	parm_p        [in] 無変調キャリア出力ON/OFF指定要求
 *  @param	awRsp_rslt    [in] 応答結果値
 *  @return	None
 *  @date	2015/08/11 TDIPS)H.Murata
 */
/********************************************************************************************************************/
VOID m_rc_UnCarOptOnOffDes_3g_Res(CMT_TSKIF_CPRIRCV_CARONOFFREQ *parm_p, USHORT awRsp_rslt)
{

	CMT_TSKIF_CPRISND_CARONOFFRES *rsp_p;		/* 応答送信ポインタ */
	UINT		bufget_rslt;					/* バッファ獲得結果(3G)			*/
	UINT		Snd_ret;						/* Return value of sending(3G)	*/


	/* ポインタ初期化 */
	rsp_p = NULL;
	
	/****************/
	/* バッファ獲得 */
	/****************/
	/* 最大値のバッファを獲得 */	
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
									sizeof(CMT_TSKIF_CPRISND_CARONOFFRES),
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
					"m_rc_UnCarOptOnOffDes_3g_Res cm_L2BfGet NG");
		return;
	}

	/* 無変調キャリア出力ON/OFF指定応答 作成 */
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_CARONOFFRES);

		/* キャリア番号 */
	rsp_p->cpridat_caronoffres.carno =  parm_p->cpridat_caronoffreq.carno;
	
		/* CPRI信号応答内容(CPRI信号種別)設定 */
	rsp_p->cpridat_caronoffres.signal_kind = CMD_CPRID_CARONOFFRES + CMD_SYS_3G;

		/* CPRI信号応答結果にその他エラー(RE)(42)を設定*/
	rsp_p->cpridat_caronoffres.result = awRsp_rslt;
	
		/* 系指定*/
	rsp_p->cpridat_caronoffres.br_app = parm_p->cpridat_caronoffreq.br_app;

	
	/* 無変調キャリア出力ON/OFF指定応答 送信 */
	/* 3G */
	Snd_ret = m_cm_L3MsgSend_REC( rsp_p );
	if( Snd_ret != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
								"m_rc_UnCarOptOnOffDes_3g_Res m_cm_L3MsgSend_REC NG" );
	}

	return;
}
/********************************************************************************************************************/
/**
 *  @brief	無変調キャリア出力ON/OFF指定要求関数(3G/LTE)
 *  @note
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/08/11 TDIPS)H.Murata
 */
/********************************************************************************************************************/
VOID m_rc_UnCarOptOnOffDesReq(VOID *parm_p)
{
	UINT	lwSigchk_rslt;					/*  result of checking signal */
	USHORT	lwSigsys_rslt;					/*  system by checking signal */
	USHORT	lwRsp_rslt;						/*  応答結果  */

	/*In case buffer is Null*/
	if( NULL == parm_p ){
		
		/*end processing*/
		return;
	}
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &lwSigsys_rslt );
	
	/* signalがNGの場合 */
	if((lwSigchk_rslt != CMD_RES_OK))
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
		return;
	}
	/* 3G System */
	if( CMD_SYS_3G == lwSigsys_rslt)
	{
			/* その他エラー(RE)(42)を返却 */
			lwRsp_rslt = CMD_OTHR_ERR_RE;
		
		/* 応答関数(3G)呼び出し */
		m_rc_UnCarOptOnOffDes_3g_Res((CMT_TSKIF_CPRIRCV_CARONOFFREQ *)parm_p, lwRsp_rslt);
	}
	/* LTE System */
	else
	{
			/* 処理NG(RE)(51)を返却 */
			lwRsp_rslt = CMD_HDL_NG;
		
		/* 応答関数(LTE)呼び出し */
		m_rc_UnCarOptOnOffDes_lte_Res( (CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ *)parm_p, lwRsp_rslt );
	}

	/* 処理終了 */
	return;
}
/** @} */

/********************************************************************************************************************/
/**
 *  @brief  Receive CARONOFF from re/rec
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 2017/01/25 M&C)Huan.dh forward CARONOFF from RE response to REC
 */
/********************************************************************************************************************/

VOID m_rc_CarOnOff_Rcv_Rsp( CMT_TSKIF_CPRISND_CARONOFFRES_S3G* parm_p  )
{
	printf("\n[INFO] [L3] [%s:%d] Enter function", __FILE__, __LINE__);
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
							"m_rc_CarOnOff_Rcv_Rsp validate signal NG");
		return;
	}

	/* 3G信号の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		parm_p->cpridat_caronoffres.result = CMD_OTHR_ERR_RE;
	}
	
	/* l3/rctからの正常メッセージをRECに送信する(無変調キャリアON/OFF指定応答) */
	printf("\n[INFO] [L3] [%s:%d] Before sending response msg to REC", __FILE__, __LINE__);
	m_cm_L3MsgSend_REC( parm_p );
}

/* @} */

/* @{ */
