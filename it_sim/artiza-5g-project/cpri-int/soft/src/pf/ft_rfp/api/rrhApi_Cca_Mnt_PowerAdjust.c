/*!
 * @skip  $ld:$
 * @file  rrhApi_Cca_Mnt_PowerAdjust.c
 * @brief API : RF操作関連
 * @date  2019/03/29 FJT)Taniguchi Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/** @addtogroup RRH_PF_RFP_API
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
#include "rrhApi_Rfp.h"

/********************************************************************************************************************/
/**
 *  @brief  API : 送信出力調整要求
 *  @note   PF EVENT ID : 0xA0010011
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  branch      : branch(0:branch0, 1:branch1, 0x0F:all)
 *  @param  command     : command(0~10 0:0.0dB 1:-0.1dB,2:-0.2dB,...10:-1.0dB )
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_ERR_BPF_CALL
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-010-001-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 *  @date  2019/03/29 FJT)Taniguchi Create.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Cca_Mnt_PowerAdjust(INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT command)
{
	T_API_RFP_POW_ADJ_REQ* msgp = NULL;
	INT errcd = 0;
	INT ret = 0;

	/* ブランチ番号の範囲チェック */
	ret = rrhApi_Rfa_com_BranchCheck(branch);
	if(ret != E_API_RCD_OK)
	{
		return E_API_RCD_EPARAM;
	}

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別		*/
										sizeof(T_API_RFP_POW_ADJ_REQ),		/* Size				*/
										(void**)&msgp,						/* msgP				*/
										&errcd );
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	
	/* データの設定を行う			*/
	/* Header部設定	*				*/
	msgp->head.uiEventNo 	 = D_API_RFP_MNT_POWER_ADJUST_REQ;			/* 送信出力調整要求			*/
	msgp->head.uiDstPQueueID = D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID	*/
	msgp->head.uiDstTQueueID = D_SYS_THDQID_PF_CCA;						/* 送信先Thread Queue ID	*/
	msgp->head.uiSrcPQueueID = qid;										/* 送信元Process Queue ID	*/
	msgp->head.uiSrcTQueueID = 0;										/* 送信元Thread Queue ID	*/
	
	/* 送信出力調整情報設定 */
	msgp->powAdjInfo.branch  = branch;
	msgp->powAdjInfo.command = command;

	/* 送信出力調整要求を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_RRH_PROCQUE_PF,					/* 送信先Process Queue ID	*/
									(void*)msgp );						/* 送信Message				*/
	
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	
	return E_API_RCD_OK;
}

/** @} */