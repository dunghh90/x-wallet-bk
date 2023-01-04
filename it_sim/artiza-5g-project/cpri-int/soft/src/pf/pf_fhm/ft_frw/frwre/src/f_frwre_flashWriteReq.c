/*!
 * @skip  $ld:$
 * @file  f_frwre_flashWriteReq.c
 * @brief Reception of Flash Write request
 * @date  2015/08/04 TDIPS)sasaki create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

#include "f_frwre_inc.h"


/*!
 *  @brief  Process of Flash Write request
 *  @note   This function is processed as follows.\n
 *          - Call function to write the flash.\n
 *          - send Flash Write Response to pf_fmng thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2014/12/12 FFCS)linb create\n
 *  @date  2015/06/26 ALPHA)ueda RRH-007-000 TDD-SRE(Zynq)
 *  @date  2015/08/04 TDIPS)sasaki Update
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_flashWriteReq( VOID *pRcvMsg )
{
	T_SYS_LMCWRT_RSP	flashWriteRsp;
	UINT				rtn = D_SYS_OK;
	INT					flashWrtRslt = D_SYS_OK;
	UCHAR*				p_data = NULL;
	UINT				a_face = D_SYS_NUM0;
	UINT				a_size = D_SYS_NUM0;
	UINT				a_offset = D_SYS_NUM0;
	UINT				a_count;

	cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frwre_flashWriteReq] ENTER");				

	/* Initialize API response message */
	memset((CHAR *)&flashWriteRsp, 0, sizeof(flashWriteRsp));

	/****************************************/
	/* Write each file from memory to flash */
	/****************************************/
	for(a_count = 0; a_count < ((T_SYS_LMCWRT_REQ*)pRcvMsg)->writeFileNum; a_count++)
	{
		a_face = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->writeFlashRom[a_count].face;
		a_offset = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->writeFlashRom[a_count].offset;
		a_size = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->writeFlashRom[a_count].size;
		p_data = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->writeFlashRom[a_count].data;

		rtn = f_frwre_com_WriteFlash(a_face, a_offset, a_size, p_data);
		if(rtn != D_SYS_OK)
		{
			flashWrtRslt = D_SYS_NG;
			break;
		}
	}

	/* Edit and Send Flash Write Response to pf_fmng thread  */
	flashWriteRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_WRITE_RSP;
	flashWriteRsp.thdIf.uiSrcTQueueID = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->thdIf.uiDstTQueueID;
	flashWriteRsp.thdIf.uiDstTQueueID = ((T_SYS_LMCWRT_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID;
	flashWriteRsp.thdIf.uiLength= sizeof(flashWriteRsp);
	flashWriteRsp.result = flashWrtRslt;

	/* save running history */
	f_frwre_cm_RunHisSet(D_FRWRE_RUNHIS_FLG_SND, (UCHAR *)&flashWriteRsp);

	f_com_msgQSendFHM( ((T_SYS_LMCWRT_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID,
					(CHAR*)&flashWriteRsp,
					sizeof(T_SYS_LMCWRT_RSP),
					D_SYS_THDQID_PF_FRQRE,
					f_frwrew_thrState,
					D_SYS_MSGID_FLASH_RE_WRITE_RSP);

	cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_frwre_flashWriteReq] RETURN");				
	return ;    
}

/* @} */
