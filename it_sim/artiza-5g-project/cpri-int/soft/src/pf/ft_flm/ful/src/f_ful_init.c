/*!
 * @skip  $ld:$
 * @file  f_ful_init.c
 * @brief FULタスクメイン処理
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include <sys/prctl.h>
#include "f_ful_inc.h"

/*!
 * @brief 関数機能概要:FULタスク初期化関数。内部データ、状態等を初期化する
 * @note  関数処理内容.
 *       -# FULタスク状態を"初期状態"に設定する
 *       -# 内部データを初期化する
 *       -# 初期設定完了通知を送信する
 * @return なし
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/01/28 KCN)上本 Create
 */
INT f_ful_init( VOID* msgP )
{
	INT				errcd ;
	INT				rtn_bpf ;
	T_SYS_INIT_COMP_NTC*			sndMsg ;		/* 送信メッセージ(初期化完了通知)	*/

	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	
	/* 内部データの初期化 */
	f_ful_thdInfo.thdState = D_FUL_TASK_STS_ACT ;

	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
											sizeof(T_SYS_INIT_COMP_NTC),		/* MessageSize						*/
											(VOID **)&sndMsg,					/* MessageBuffer					*/
											&errcd );							/* ErrorCode						*/

	if( rtn_bpf != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET error=%08x", rtn_bpf );
	}

	/************************************************************************/
	/* 送信MSG作成															*/
	/************************************************************************/
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC ;					/* メッセージID(0x00000102)			*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF ;							/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_FUL;							/* 送信元TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長					*/

	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN ,								/* 送信MSGキューID					*/
							( VOID* )sndMsg ) ;

	return rtn_bpf ;
}

/* @} */
