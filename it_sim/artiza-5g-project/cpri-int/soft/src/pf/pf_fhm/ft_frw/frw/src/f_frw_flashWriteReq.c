/*!
 * @skip  $ld:$
 * @file  f_frw_flashWriteReq.c
 * @brief Reception of Flash Write request
 * @date  2013/11/13 FFCS)linb create\n
 * @date  2015/08/14 TDIPS)sasaki deelte WRITE retry process
 * @date  2015/08/28 TDIPS)sasaki update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"


/*!
 *  @brief  Process of Flash Write request
 *  @note   This function is processed as follows.\n
 *          - Call function to write the flash.\n
 *          - send Flash Write Response to pf_fmng thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/13 FFCS)linb create\n
 *  @date  2015/08/14 TDIPS)sasaki delte WRITE retry process
 *  @date  2015/08/28 TDIPS)sasaki 不要コメント削除
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frw_flashWriteReq(void *pRcvMsg)
{	
	T_SYS_LMCWRT_REQ*	pLmcWrtReq = NULL;
	T_SYS_LMCWRT_RSP	lmcWrtRsp;
	UINT				fileCnt;
	INT					flashWrtRslt = D_SYS_OK;		/* Flash書き込み結果								*/
	UINT				rtn = D_SYS_OK;
	FILE*				fileDsc;
	CHAR				fileName[64];
	LONG				file_size;

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frw_flashWriteReq] ENTER" );				

	/* Get request side of flash from the message */
	pLmcWrtReq = (T_SYS_LMCWRT_REQ*)pRcvMsg;
	f_frww_respQid = pLmcWrtReq->thdIf.uiSrcTQueueID;

	/* Initialize API response message */
	memset((CHAR*)(&lmcWrtRsp),0, sizeof(lmcWrtRsp));

	/****************************************/
	/* Write each file from memory to flash */
	/****************************************/
	for(fileCnt = 0; fileCnt < pLmcWrtReq->writeFileNum; fileCnt++)
	{	
		if(pLmcWrtReq->writeFlashRom[fileCnt].data != NULL)
		{
			rtn |= f_frw_com_WriteFlash_slc1chp(pLmcWrtReq->writeFlashRom[fileCnt].face,	/* 書込み面 						*/
												pLmcWrtReq->writeFlashRom[fileCnt].offset,	/* Flash書込み先のオフセット		*/
												pLmcWrtReq->writeFlashRom[fileCnt].size,	/* Flash書込みサイズ				*/
												pLmcWrtReq->writeFlashRom[fileCnt].data 	/* Flash書込みデータ				*/
												);	
		}
		else
		{
			memset(fileName, 0, sizeof(fileName));
			sprintf(fileName, "%s%s",D_SYS_UNZIP_DIRNAME,pLmcWrtReq->writeFlashRom[fileCnt].fileName);
			fileDsc = fopen( fileName ,"rb"); /* pgr0520 */
			fseek(fileDsc, 0, SEEK_END);      /* pgr0520 */
			file_size = ftell(fileDsc);
			fclose(fileDsc);                  /* pgr0520 */
			
			if(file_size != pLmcWrtReq->writeFlashRom[fileCnt].size)
			{
				cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ERROR, "[f_frw_flashWriteReq] size NG (%d != %d)",
										pLmcWrtReq->writeFlashRom[fileCnt].size, file_size);				
				flashWrtRslt = D_SYS_NG;
				break;
			}
			rtn |= f_frw_com_WriteFlash(pLmcWrtReq->writeFlashRom[fileCnt].face,	/* 書込み面 						*/
										pLmcWrtReq->writeFlashRom[fileCnt].offset,	/* Flash書込み先のオフセット		*/
										pLmcWrtReq->writeFlashRom[fileCnt].size,	/* Flash書込みサイズ				*/
										pLmcWrtReq->writeFlashRom[fileCnt].fileName /* Flash書込みデータ				*/
										);	
		}
		if(rtn != D_SYS_OK)
		{
			flashWrtRslt = D_SYS_NG;
			break;
		}
	}

	/* Edit and Send Flash Write Response to pf_fmng thread  */
	lmcWrtRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_RSP;
	lmcWrtRsp.thdIf.uiSrcTQueueID = pLmcWrtReq->thdIf.uiDstTQueueID;
	lmcWrtRsp.thdIf.uiDstTQueueID = pLmcWrtReq->thdIf.uiSrcTQueueID;
	lmcWrtRsp.thdIf.uiLength= sizeof(lmcWrtRsp);
	lmcWrtRsp.result = flashWrtRslt;

	/* save running history */
	f_frw_cm_RunHisSet(D_FRW_RUNHIS_FLG_SND, (UCHAR *)&lmcWrtRsp);

	f_com_msgQSendFHM( f_frww_respQid, 
					(CHAR*)&lmcWrtRsp,
					sizeof( T_SYS_LMCWRT_RSP ), 
					D_SYS_THDQID_PF_FRQ, 
					f_frww_thrState, 
					lmcWrtRsp.thdIf.uiEventNo);

	
	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_frw_flashWriteReq] RETURN" );				
	return ;    
}

/* @} */
