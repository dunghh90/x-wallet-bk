/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCardDia.c
 *  @brief  REカード診断関数
 *  @date   2015/08/12 TDIPS)H.Murata
 *  @date   2021/02/19 M&C)Tri.hn Create based on No.67 (sequence 3-30)
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
 *  @brief	REカード診断要求関数
 *  @note
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/08/12 TDIPS)H.Murata
 *  @date   2021/02/19 M&C)Tri.hn Update based on No.67 (sequence 3-30)
 */
/********************************************************************************************************************/
VOID m_rc_ReCardDiaReq(CMT_TSKIF_CPRIRCV_RECRDDIAREQ* parm_p)
{
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwRsp_rslt;						/*  応答結果					*/

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_recrddiareq.signal_kind, &lwSigsys_rslt);

	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
		if( CMD_SYS_3G == lwSigsys_rslt) {
			//RECにREカード診断応答を送信
			m_rc_ReCardDiaResSnd( lwSigsys_rslt, parm_p );
			return;
		}
	}

	/* Forward msg request to re/rec */
	l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, sizeof( CMT_TSKIF_CPRIRCV_RECRDDIAREQ ) );

	/* 処理終了 */
	return;
}

/********************************************************************************************************************/
/**
 *  @brief	REカード診断応答関数
 *  @param  parm_p        [in] REカード診断要求
 *  @return	None
 *  @date   2021/02/19 M&C)Tri.hn Create based on No.67 (sequence 3-30)
 */
/********************************************************************************************************************/
VOID m_rc_ReCardDiaRes( CMT_TSKIF_CPRIRCV_RECRDDIARES* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	UINT			lwSigchk_rslt;						/*  result of checking signal */
	USHORT			lwSigsys_rslt;					/*  system by checking signal */
	INT				errcd;
	INT				ldwResult;
	USHORT			lwTimerId;
	USHORT			lwLoop;
	T_RRH_LAYER3	ltLayer3Sts;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_recrddiares.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s m_cm_chksig:%d, lwSigsys_rslt:%d \n",__LINE__,__FUNCTION__,lwSigchk_rslt,lwSigsys_rslt);
#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(result:%d, Systerm:%d) \n",__LINE__,__FUNCTION__,awResult, awSystem);
#endif

	/* FTPデータ送信用PORT番号設定応答 */
	m_rc_ReCardDiaResSnd(lwSigsys_rslt, parm_p);

	return;
}

/********************************************************************************************************************/
/**
 *  @brief	REカード診断応答関数
 *  @param	awSigsys_rslt [in] the system type LTE/3G
 *  @param  parm_p        [in] REカード診断要求
 *  @return	None
 *  @date	2015/08/12 TDIPS)H.Murata
 *  @date   2021/02/19 M&C)Tri.hn Update based on No.67 (sequence 3-30)
 */
/********************************************************************************************************************/
VOID m_rc_ReCardDiaResSnd( USHORT awSigsys_rslt, CMT_TSKIF_CPRIRCV_RECRDDIARES* parm_p)
{
	CMT_TSKIF_CPRISND_RECRDDIARES *rsp_p;
	UINT		bufget_rslt;					/* バッファ獲得結果			*/
	UINT		Snd_ret;						/* Return value of sending		*/

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 最大値のバッファを獲得 */
	rsp_p		 = NULL;
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_RECRDDIARES),
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
					"m_rc_ReCardDiaResSnd cm_L2BfGet NG");
		return;
	}

	/* REカード診断応答 作成 */
	/* バッファ */
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_RECRDDIARES);

	/* 信号種別 */
	rsp_p->cpridat_recrddiares.signal_kind = CMD_CPRID_RECRDDIARES + awSigsys_rslt;

	/* 3G signal */
	if( CMD_SYS_3G == awSigsys_rslt) {
		/* 処理NG(RE)(51)を返却 */
		rsp_p->cpridat_recrddiares.result = CMD_HDL_NG;
	}
	else {
		/* 応答結果 */
		rsp_p->cpridat_recrddiares.result = parm_p->cpridat_recrddiares.result;

		/* スロット番号 */
		rsp_p->cpridat_recrddiares.slot_no = parm_p->cpridat_recrddiares.slot_no;

		/* 診断結果 */
		rsp_p->cpridat_recrddiares.dia_result = 0;
	}

	/* REカード診断応答 送信 */
	Snd_ret = m_cm_L3MsgSend_REC( rsp_p );
	if( Snd_ret != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
								"m_rc_ReCardDiaResSnd m_cm_L3MsgSend_REC NG" );
	}

	return;
}

/** @} */