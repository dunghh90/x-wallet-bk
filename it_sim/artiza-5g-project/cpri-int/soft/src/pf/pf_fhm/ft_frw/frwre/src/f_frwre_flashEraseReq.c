/*!
 * @skip  $ld:$
 * @file  f_frwre_flashEraseReq.c
 * @brief Reception of Flash Erase request
 * @date  2015/08/04 TDIPS)sasaki create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

#include "f_frwre_inc.h"


/*!
 *  @brief  Process of Flash erase request
 *  @note   This function is processed as follows.\n
 *          - Call function to erase the flash.\n
 *          - send Flash Erase Response to pf_fmng thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2015/08/04 TDIPS)sasaki create\n
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_flashEraseReq( VOID *pRcvMsg )
{
	T_SYS_FLSERASE_RSP			flashEraseRsp;
	UINT						rtn = D_SYS_OK;
	UINT						a_face = D_SYS_NUM0;
	UINT						a_size = D_SYS_NUM0;
	UINT						a_offset = D_SYS_NUM0;
	UINT						a_count;

	cmn_com_assert_th_get( f_frwrew_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frwre_flashEraseReq] ENTER" );

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FLASH ERASE REQ Start");
	/* Initialization */
	memset((CHAR *)(&flashEraseRsp), 0, sizeof(flashEraseRsp));
	
	/******************/
	/* Clear FlashROM */
	/******************/
	for(a_count = 0; a_count < ((T_SYS_FLSERASE_REQ*)pRcvMsg)->clearFileNum; a_count++)
	{
		a_face = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->clearFlashRom[a_count].face;
		a_offset = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->clearFlashRom[a_count].offset;
		a_size = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->clearFlashRom[a_count].size;

		/* サイズが0であれば、消去を行わない */
		if(D_SYS_NUM0 == a_size)
		{
			continue;
		}

		rtn |= f_frwre_cm_EraseFlash(a_face, a_offset, a_size);
	}

	/* Send Flash Erase Response to pf_fmng thread (result = ret) */
	flashEraseRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_ERASE_RSP;
	flashEraseRsp.thdIf.uiSrcTQueueID = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->thdIf.uiDstTQueueID;
	flashEraseRsp.thdIf.uiDstTQueueID = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID;
	flashEraseRsp.thdIf.uiLength= sizeof(flashEraseRsp);
	flashEraseRsp.cause = ((T_SYS_FLSERASE_REQ*)pRcvMsg)->cause;
	flashEraseRsp.result = rtn;

	/* save running history */
	f_frwre_cm_RunHisSet(D_FRWRE_RUNHIS_FLG_SND, (UCHAR *)&flashEraseRsp);

	/* Edit and send Response to pf_fmng thread */
	f_com_msgQSendFHM( ((T_SYS_FLSERASE_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID,
					(CHAR*)&flashEraseRsp,
					sizeof(T_SYS_FLSERASE_RSP),
					D_SYS_THDQID_PF_FRQRE,
					f_frwrew_thrState,
					D_SYS_MSGID_FLASH_RE_ERASE_RSP );

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FLASH ERASE REQ End");
	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwre_flashEraseReq] RETURN" );        
    return ;    
}
/* @} */

