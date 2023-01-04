/*!
 * @skip  $ld:$
 * @file  rrhApi_Log_Mnt_TroubleLogGetStop.c
 * @brief API : Log関連
 * @date  2013/10/29 ALPHA)yokoyama Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/** @addtogroup RRH_PF_LOG_API
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
#include "rrhApi_Log.h"

/********************************************************************************************************************/
/**
 *  @brief  API : Trouble Log Get Stop Send(障害ログファイル作成中止)
 *  @note   PF EVENT ID : 0xA0020003
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *	@param  sysKind     : 信号種別(3G/S3G)
 *  @return E_RRHAPI_RCODE
 *  @retval ret
 *  @date   2013/10/29 ALPHA)yokoyama Create.
 *  @date   2014/12/23 ALPHA)fujii modify for ppcEOL.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Log_TroubleLogGetStop_com(	INT				qid, 
													INT				wtime,
													VOID			*data_p,
													UINT			sysKind)
{
	T_API_LOG_TROUBLELOG_GET_STOP			*msgp;
	INT										errcd;
	INT 									ret;
	
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別		*/
										sizeof(T_API_LOG_TROUBLELOG_GET_STOP),	/* Size				*/
										(VOID **)&msgp,							/* msgP				*/
										&errcd );
	
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	
	/* データの設定を行う			*/
	/* Header部設定	*/
	msgp->head.uiEventNo 	 = D_API_LOG_MNT_STOP_TROUBLE_LOG_GET;		/* 障害ログファイル作成中止	*/
	msgp->head.uiDstPQueueID = D_RRH_PROCQUE_F_PF;						/* 送信先Process Queue ID	*/
	msgp->head.uiDstTQueueID = D_SYS_THDQID_PF_LOG;						/* 送信先Thread Queue ID	*/
	msgp->head.uiSrcPQueueID = qid;										/* 送信元Process Queue ID	*/
	msgp->head.uiSrcTQueueID = 0;										/* 送信元Thread Queue ID	*/
	msgp->head.uiSignalkind  = sysKind;									/* 信号種別					*/

	/* PFへ障害ログファイル作成中止を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_RRH_PROCQUE_F_PF,					/* 送信先Process Queue ID	*/
									(void *)msgp );						/* 送信Message				*/
	
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : Trouble Log Get Stop Send(障害ログファイル作成中止)
 *  @note   PF EVENT ID : 0xA0020003
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *	
 *  @return E_RRHAPI_RCODE
 *  @retval ret
 *  @date   2013/10/29 ALPHA)yokoyama Create.
 *  @date   2014/12/23 ALPHA)fujii modify for ppcEOL.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Log_Mnt_TroubleLogGetStop(	INT				qid, 
													INT				wtime,
													VOID			*data_p )
{
	E_RRHAPI_RCODE		rCode;
	
	rCode = rrhApi_Log_TroubleLogGetStop_com( qid, wtime, data_p, D_SYS_S3G );
	
	return rCode;
}

/** @} */

