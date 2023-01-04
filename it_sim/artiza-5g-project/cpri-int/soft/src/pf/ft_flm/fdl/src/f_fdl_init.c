/*!
 * @skip  $ld:$
 * @file  f_fdl_init.c
 * @brief FDLタスクメイン処理
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include <sys/prctl.h>
#include "f_fdl_inc.h"

/*!
 * @brief 関数機能概要:FDLタスク初期化関数。内部データ、状態等を初期化する
 * @note  関数処理内容.
 *       -# FDLタスク状態を"初期状態"に設定する
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
INT f_fdl_init( VOID* rcvMsg_p )
{
	INT				errcd ;
	INT				rtn_bpf ;
	T_SYS_INIT_COMP_NTC*			sndMsg ;		/* 送信メッセージ(初期化完了通知)	*/

	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	
	/* 内部データの初期化 */
	f_fdl_swact_data[D_SYS_NUM0] = D_FDL_ACT_STATE_RUN ;
	f_fdl_swact_data[D_SYS_NUM1] = D_FDL_ACT_STATE_RUN ;
	memset(&f_fdl_inst_state_data , D_RRH_ZERO , sizeof(f_fdl_inst_state_data)) ;
	/* プロダクトIDの初期化 */
	memset(&f_fdl_prdct_id , D_RRH_ZERO , sizeof(f_fdl_prdct_id)) ;
	sprintf(f_fdl_prdct_id , "%02x%02x" , f_cmw_tra_inv_tbl->cmx_device_typ , f_cmw_tra_inv_tbl->cmx_device_BW) ;

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
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;							/* 送信元TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長					*/

	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN ,								/* 送信MSGキューID					*/
							( VOID* )sndMsg ) ;

	f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;

	return rtn_bpf ;
}

/* @} */
