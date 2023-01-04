/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_RePortCnt.c
 *  @brief  RE PORT制御関数
 *  @date   2015/08/12 TDIPS)H.Murata create
 *  @date   2015/08/17 TDIPS)H.Murata port出力状態の返却追加
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
 *  @brief	RE PORT制御応答関数
 *  @note
 *  @param	awSigsys_rslt [in] the system type LTE/3G
 *  @param	awRsp_rslt    [in] 応答結果値
 *  @return	None
 *  @date	2015/08/12 TDIPS)H.Murata
 */
/********************************************************************************************************************/
VOID m_rc_RePortCntRes(USHORT awSigsys_rslt, USHORT awRsp_rslt)
{
	CMT_TSKIF_CPRISND_REPORTCNTRES *rsp_p;
	UINT		bufget_rslt;					/* バッファ獲得結果			*/
	UINT		Snd_ret;						/* Return value of sending		*/

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 最大値のバッファを獲得 */
	rsp_p		 = NULL;
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_REPORTCNTRES),
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
					"m_rc_RePortCntRes cm_L2BfGet NG");
		return;
	}

	/* RE PORT制御応答 作成 */
		/* バッファ */
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_REPORTCNTRES);

		/* 信号種別 */
	rsp_p->cpridat_reportcntres.signal_kind = CMD_CPRID_REPORTCNTRES + awSigsys_rslt;

		/* 応答結果 */
	rsp_p->cpridat_reportcntres.result = awRsp_rslt;

		/* port出力結果(0) */
	rsp_p->cpridat_reportcntres.portout_sts = 0;



	/* RE PORT制御応答 送信 */
	Snd_ret = m_cm_L3MsgSend_REC( rsp_p );
	if( Snd_ret != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
								"m_rc_RePortCntRes m_cm_L3MsgSend_REC NG" );
	}

	return;
}

/********************************************************************************************************************/
/**
 *  @brief	RE PORT制御要求関数
 *  @note
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/08/12 TDIPS)H.Murata
 */
/********************************************************************************************************************/
VOID m_rc_RePortCntReq(CMT_TSKIF_CPRIRCV_REPORTCNTREQ *parm_p)
{
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwRsp_rslt;						/*  応答結果				  */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_reportcntreq.signal_kind, &lwSigsys_rslt);

	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
		return;
	}

	/* 3G system*/
	if( CMD_SYS_3G == lwSigsys_rslt)
	{
		/* 処理その他エラー(RE)(42)を返却 */
		lwRsp_rslt = CMD_OTHR_ERR_RE;

	}
	/* LTE system*/
	else
	{
		/* 処理NG(RE)(51)を返却 */
		lwRsp_rslt = CMD_HDL_NG;
	}

	//RECにRE PORT制御応答を送信
	m_rc_RePortCntRes(lwSigsys_rslt, lwRsp_rslt);

	/* 処理終了 */
	return;
}
/** @} */
