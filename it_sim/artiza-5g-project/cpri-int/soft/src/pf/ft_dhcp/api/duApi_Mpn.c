/*!
 * @skip  $ld:$
 * @file  duApi_Mpn.c
 * @brief API : M-Plane操作系API
 * @date  2018/02/15 FJT)H.Yoshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/** @addtogroup RRH_API_MPN
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"

#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"

#include "rrhApi_Com.h"
#include "duApi_Mpn.h"

/********************************************************************************************************************/
/**
 *  @brief	API : M-Plane接続準備要求
 *  @note	PF EVENT ID : 0xA00F0001
 * 			Reponse  ID : -
 * 			TYPE		: MPN
 *  @param	qid			: response queue id (応答送信先QueueID)
 *  @param	wtime		: wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param	*data_p		: 取得データポインタ
 *  @param	lowVid		: 検索VID最小値
 *  @param	highVid		: 検索VID最大値
 *  @param	lastUsedVid	: VID前回値
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/02/15	FJT)H.Yoshida	Create.
 *  @date	2018/02/23	FJT)H.Yoshida	delete input parameter "Method".
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
/***** 2018.02.14 H.Yoshida 5G-DU 3M機能追加 Memo	pf_dhcpにM-Plane接続準備要求を送信する	*****/
E_RRHAPI_RCODE	duApi_Mpn_Mnr_MplaneReady( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< 応答待ち時間		*/
										   VOID		*data_p,			/**< 取得データ			*/
										   USHORT	Interface,			/**< Interface指定		*/
										   USHORT	Flag,				/**< 接続指定			*/
										   USHORT	lowVid,				/**< 検索VID最小値		*/
										   USHORT	highVid,			/**< 検索VID最大値		*/
										   USHORT	lastUsedVid)		/**< VID前回値			*/
{
	T_API_MPLANE_READY_REQ			*apiReq_p = NULL;					/**< M-Plane接続準備要求*/
	INT								errcd;
	INT								ret;

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別			*//* ★ */
										sizeof(T_API_MPLANE_READY_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* エラーコード			*/

	/* OAL処理結果チェック	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API：M-Plane接続準備要求ヘッダ設定	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_MPN_MPLANE_READY_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PF_DHC;				/* Destination T QID	*//* ★ */
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_MPLANE_READY_REQ);	/* Length				*/

	/* API：M-Plane接続準備要求データ部設定	*/
	apiReq_p->data.Interface		= Interface;
	apiReq_p->data.Flag				= Flag;
	apiReq_p->data.lowVid			= lowVid;							/* 検索VID最小値		*/
	apiReq_p->data.highVid			= highVid;							/* 検索VID最大値		*/
	apiReq_p->data.lastUsedVid		= lastUsedVid;						/* VID前回値			*/

	/* PF_DHCPへM-Plane接続準備要求を送信する	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PF,						/* 送信先Proc Queue ID	*/
								 (void *)apiReq_p );					/* 送信Message			*/

	/* OAL処理結果チェック	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

#if 0
/********************************************************************************************************************/
/**
 *  @brief	API : M-Plane接続開始要求
 *  @note	PF EVENT ID : 0xA00F0003
 * 			Reponse  ID : -
 * 			TYPE		: MPN
 *  @param	qid			: response queue id (応答送信先QueueID)
 *  @param	wtime		: wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param	*data_p		: 取得データポインタ
 *  @param	remote_ipadr: CU側IPアドレス
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/02/15	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
/***** 2018.02.14 H.Yoshida 5G-DU 3M機能追加 Memo	pf_mpsにM-Plane接続開始要求を送信する	*****/
E_RRHAPI_RCODE	duApi_Mpn_Mnr_MplaneStart( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< 応答待ち時間		*/
										   VOID		*data_p,			/**< 取得データ			*/
										   UINT		local_ipadr,		/**< DU側IPアドレス		*/
										   UINT		remote_ipadr)		/**< CU側IPアドレス		*/
{
	T_API_MPLANE_START_REQ			*apiReq_p = NULL;					/**< M-Plane接続準備要求*/
	INT								errcd;
	INT								ret;

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別			*//* ★ */
										sizeof(T_API_MPLANE_START_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* エラーコード			*/

	/* OAL処理結果チェック	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API：M-Plane接続開始要求ヘッダ設定	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_MPN_MPLANE_START_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PF_MPS;				/* Destination T QID	*//* ★ */
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_MPLANE_START_REQ);	/* Length				*/

	/* API：M-Plane接続開始要求データ部設定	*/
	apiReq_p->data.local_ipadr		= local_ipadr;						/* DU側IPアドレス		*/
	apiReq_p->data.remote_ipadr		= remote_ipadr;						/* CU側IPアドレス		*/

	/* PF_MPSへM-Plane接続開始要求を送信する	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PF,						/* 送信先Proc Queue ID	*/
								 (void *)apiReq_p );					/* 送信Message			*/

	/* OAL処理結果チェック	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}
#endif
/** @} */

