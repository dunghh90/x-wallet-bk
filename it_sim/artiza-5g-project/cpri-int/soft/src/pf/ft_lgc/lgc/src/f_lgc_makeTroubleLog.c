/*!
 * @skip		$ld:$
 * @file		f_lgc_makeTroubleLog.c
 * @brief		ログスレッド 障害ログ出力処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_lgc_inc.h"
#include "f_dbg_com_ext.h"
#include "f_dbg_thread_ext.h"
#include <errno.h>


/*!
 * @brief		関数機能概要:障害ログ出力処理を行う
 * @note		関数処理内容.
 *				-# 障害種別によって出力先を変更する
 *				-# f_lgc_abortLog()をコールする
 *					-# 統計情報をファイルに出力する
 *				-# 障害ログ書き込み要求メッセージ送信 to:f_nma_troubleLogWriteReq()
 *					-# 出力ファイルを圧縮する
 *					-# フラッシュに書き込む
 *				-# ログ格納フォルダをクリアする
 *				-# INITプロセスへ障害ログ出力完了を通知する
 * @param		trouble kind	[in]	障害種別
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_lgc_makeTroubleLog( UINT  uiTroubleKind )
{

	UINT							output_dir			= 0;
	T_SYS_TROUBLE_LOG_WRITE_REQ*	sndTroubleWriteMsg;		/* 障害ログ書き込み要求送信メッセージ	*/
	UINT							face				= 0;
	UINT							kind				= D_RRH_LOGTYPE_TROUBLE;
	UINT							offset				= 0;
	UINT							log_face			= 0;
	INT								ret;
	INT								errcd;
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_makeTroubleLog] ENTER" );

	if( E_SYS_TROUBLE_KIND_NORMAL == uiTroubleKind )
	{
		/* 通常障害の場合	*/
		/* output_dir =  "/var/log/troublelog/" */
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
	}
	else if( E_SYS_TROUBLE_KIND_RST == uiTroubleKind )
	{
		/* 障害(RESET有)の場合	*/
		/* output_dir =  "/var/log/troublelog_rst/" */
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM_RST;
	}
	/******************************************************/
	/* 統計情報を/var/log/troublelog配下に出力する        */
	/* f_lgc_abortLog()をコールする                       */
	/******************************************************/
	f_lgc_abortLog(output_dir);
	
	/******************************************************/
	/* 障害ログ書き込み要求メッセージ送信                 */
	/* 圧縮・フラッシュ書き込み・ログフォルダクリアを行う */
	/******************************************************/
	/* 障害ログの退避面を取得する	*/
	(VOID)BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_TROUBLE_LOG, (UCHAR*)&log_face );
	
	log_face++;
	if( log_face >= E_RRH_QSPI_FLASH_TROUBLELOG_FACE_MAX )
	{
		log_face = 0;
	}
	
	switch(log_face)
	{
	case 0:
		/* 障害ログ領域0面#0 */
		face = 0;
		offset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE0;
		break;
		
	case 1:
		/* 障害ログ領域0面#1 */
		face = 0;
		offset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE1;
		break;
		
	case 2:
		/* 障害ログ領域1面#0 */
		face = 1;
		offset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE2;
		break;
		
	default:
		break;
	}
	
	/* 送信手順 */
	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_TROUBLE_LOG_WRITE_REQ),		/* MessageSize						*/
										(VOID **)&sndTroubleWriteMsg,			/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	
	if( BPF_RU_IPCM_OK != ret  )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BUFFER GET NG %d", ret );
		return D_RRH_NG;
	}
	/* 送信MSG作成 */
	sndTroubleWriteMsg->head.uiEventNo		= D_SYS_MSGID_TROUBLE_LOG_WRITE_REQ;	/* メッセージID						*/
	sndTroubleWriteMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 返信先PQID						*/
	sndTroubleWriteMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先TQID						*/
	sndTroubleWriteMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元PQID						*/
	sndTroubleWriteMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_LGC;					/* 送信元TQID						*/
	
	/* フラッシュ書き込み先情報を添付		*/
	sndTroubleWriteMsg->kind		= kind;
	sndTroubleWriteMsg->face		= face;
	sndTroubleWriteMsg->offset		= offset;
	
	cmn_com_assert_th_get(f_lgcw_assert_p,  D_RRH_LOG_AST_LV_INFO, "send to nma mesage was made. kind = %d, face = %d, offset = %d ",kind,face,offset);

	
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_NMA,						/* 送信MSGキューID					*/
									(VOID *)sndTroubleWriteMsg );						/* 送信MSGポインタ					*/
	
	if( BPF_RU_IPCM_OK != ret  )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "SEND NG %d", ret );
		return D_RRH_NG;
	}
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_makeTroubleLog] RETURN" );

	return D_RRH_OK;
}

/* @} */
