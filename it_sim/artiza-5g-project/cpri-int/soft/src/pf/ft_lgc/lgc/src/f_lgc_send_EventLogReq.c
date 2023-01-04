/*!
 * @skip		$ld:$
 * @file		f_lgc_send_EventLogReq.c
 * @brief		ログスレッド EventLogのClear/Write要求送信
 * @author		ALPHA)yokoyama
 * @date		2013/11/23 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:EventLogのClear/Write要求送信
 * @note		関数処理内容.
 *				-# EventLogのClear/Write要求送信
 * @param		種別			[in]  種別
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/23 ALPHA)横山 Create
 */
INT f_lgc_send_EventLogReq( UINT	kind )
{
	T_SYS_FRSP_FLSLOGWR_IND*	sndMsg_FlsLogWrInd;							/* 送信メッセージ(Flash Log WR)		*/
	INT							ret;
	INT							errcd;
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_send_EventLogReq] ENTER" );
	
	/****************************************************************************************************************/
	/* EventLog操作要求(退避ログのClear or Write)送信																*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_FRSP_FLSLOGWR_IND),		/* MessageSize						*/
										(VOID **)&sndMsg_FlsLogWrInd,			/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	
	if( BPF_RU_IPCM_OK != ret  )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BUFFER GET NG %d", ret );
		return D_RRH_NG;
	}
	
	/* 送信MSG作成 */
	sndMsg_FlsLogWrInd->head.uiEventNo		= D_SYS_MSGID_FLSLOGWR_IND;			/* メッセージID						*/
	sndMsg_FlsLogWrInd->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 返信先PQID						*/
	sndMsg_FlsLogWrInd->head.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;				/* 送信先TQID						*/
	sndMsg_FlsLogWrInd->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元PQID						*/
	sndMsg_FlsLogWrInd->head.uiSrcTQueueID	= D_SYS_THDQID_PF_LGC;				/* 送信元TQID						*/
	
	/* Flashへ退避したログの削除依頼を送付		*/
	sndMsg_FlsLogWrInd->eraseWriteKind	= kind;
	
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_NMA,						/* 送信MSGキューID					*/
									(VOID *)sndMsg_FlsLogWrInd );				/* 送信MSGポインタ					*/
	
	if( BPF_RU_IPCM_OK != ret  )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "SEND NG %d", ret );
		return D_RRH_NG;
	}
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_send_EventLogReq] RETURN" );
	return D_RRH_OK;
}

/* @} */
