/*!
 * @skip  $ld:$
 * @file  f_frwsla_flashEraseReq.c
 * @brief Reception of Flash Erase request
 * @date  2014/12/12 FFCS)linb create\n
 * @date  2015/08/28 TDIPS)sasaki update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

 /*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */

#include "f_frwsla_inc.h"


/*!
 *  @brief  Process of Flash erase request
 *  @note   This function is processed as follows.\n
 *          - Call function to erase the flash.\n
 *          - send Flash Erase Response to pf_fmng thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2014/12/12 FFCS)linb create\n
 *  @date  2015/08/28 TDIPS)sasaki 不要コメント削除
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwsla_flashEraseReq(void *pRcvMsg)
{
	UINT						fileCount;						/* Clear Flash用カウンタ							*/
	UINT						rtn = D_SYS_OK;
	T_SYS_FLSERASE_REQ			*pFlshEraseReq;
	T_SYS_FLSERASE_RSP			flshEraseRsp;

	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frwsla_flashEraseReq] ENTER" );

	/* Initialization */
	memset((CHAR*)(&flshEraseRsp), 0,sizeof(flshEraseRsp));
	
	rtn = D_SYS_OK;
	
	/* Get request side of flash from the message */
	pFlshEraseReq = (T_SYS_FLSERASE_REQ*)pRcvMsg;
	f_frwslaw_respQid = pFlshEraseReq->thdIf.uiSrcTQueueID;

	/******************/
	/* Clear FlashROM */
	/******************/
	for(fileCount = 0; fileCount < pFlshEraseReq->clearFileNum; fileCount++)
	{
		/* サイズが0であれば、書き込みを行わない */
		if(D_SYS_NUM0 == pFlshEraseReq->clearFlashRom[fileCount].size)
		{
			continue;
		}
		
		rtn |= f_frwsla_cm_EraseFlash(pFlshEraseReq->clearFlashRom[fileCount].face,	  /* 消去面						    	*/
								  	  pFlshEraseReq->clearFlashRom[fileCount].offset, /* Flash消去するオフセット			*/
									  pFlshEraseReq->clearFlashRom[fileCount].size	  /* Flash消去するサイズ				*/
									);			
	}

	/* Send Flash Erase Response to pf_fmng thread (result = ret) */
	flshEraseRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_ERASE_RSP;
	flshEraseRsp.thdIf.uiSrcTQueueID = pFlshEraseReq->thdIf.uiDstTQueueID;
	flshEraseRsp.thdIf.uiDstTQueueID = pFlshEraseReq->thdIf.uiSrcTQueueID;
	flshEraseRsp.thdIf.uiLength= sizeof(flshEraseRsp);
	flshEraseRsp.cause = pFlshEraseReq->cause;
	flshEraseRsp.result = rtn;

	/* save running history */
	f_frwsla_cm_RunHisSet(D_FRWSLA_RUNHIS_FLG_SND, (UCHAR *)&flshEraseRsp);

	/* Edit and send Response to pf_fmng thread */
	f_com_msgQSendFHM( f_frwslaw_respQid, 
					(CHAR*)&flshEraseRsp,
					sizeof( T_SYS_FLSERASE_RSP ), 
					D_SYS_THDQID_PF_FRQS, 
					f_frwslaw_thrState, 
					D_SYS_MSGID_FLASH_ERASE_RSP );
	
	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwsla_flashEraseReq] RETURN" );        
    return ;    
}
/* @} */

