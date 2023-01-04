/*!
 * @skip  $ld:$
 * @file  f_frwre_flashReadReq.c
 * @brief Reception of Flash Read request
 * @date  2015/08/04 TDIPS)sasaki create
 * @date  2015/08/14 TDIPS)sasaki Add READ Cancel process
 * @date  2015/08/18 TDIPS)sasaki Update
 * @date  2015/08/27 TDIPS)sasaki Update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

#include "f_frwre_inc.h"
#include "f_fmng_inc.h"

/*!
 *  @brief  Process of Flash Read request
 *  @note   This function is processed as follows.
 *          - get flash information from flash.\n
 *          - send Flash Read Response to pf_fmng thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date   2015/08/04 TDIPS)sasaki create\n
 *  @date   2015/08/14 TDIPS)sasaki Add READ Cancel process
 *  @date   2015/08/18 TDIPS)sasaki BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ()引数のOFFSET加算処理漏れを修正
 *  @date   2015/08/21 TDIPS)sasaki BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ()がNGの場合、READ処理を中断し、FMNGスレッドへNG応答を返すよう変更
 *  @date   2015/08/27 TDIPS)sasaki MKレビュー指摘No.116対処
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_flashReadReq( VOID *pRcvMsg )
{
	UCHAR*						p_readbuf = f_cmw_rcvfirm_tbl + D_SYS_NUM32;
	T_SYS_FLASH_READ_RSP		flashReadRsp;
	INT							ret = BPF_HM_DEVC_COMPLETE;
	UINT						a_face = D_SYS_NUM0;
	UINT						a_size = D_SYS_NUM0;
	UINT						a_offset = D_SYS_NUM0;
	UINT						a_size_read;
	UINT						a_size_remain;

	cmn_com_assert_th_get( f_frwrew_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frwre_flashReadReq] ENTER" );
	
	/* Initialization */
	memset((CHAR*)(&flashReadRsp), 0, sizeof(flashReadRsp));

	a_face = ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->face;
	a_offset = ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->offset;
	a_size = ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->size;
	a_size_remain = a_size; 

	while(a_size_remain > 0)
	{
		a_size_read = (a_size_remain > D_FRWRE_BLOCK_SIZE_READ) ? (D_FRWRE_BLOCK_SIZE_READ) : (a_size_remain);

		/* READ RE File	*/
		ret = BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ(a_face, a_offset, a_size_read, p_readbuf);
		if(ret != BPF_HM_DEVC_COMPLETE)
		{
			/* Assert */
			cmn_com_assert_th_get( f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR ,"BPF_HM_DEVC_QSPI_FLASH_READ return NG %d",ret);
			break;
		}

		/* 配下REへのファイルダウンロードを中止を要求された場合、FlashのReadを中止する */
		if(f_fmngw_read_cancel_re == D_SYS_ON)
		{
			break;
		}
		p_readbuf += a_size_read;
		a_offset += a_size_read;
		a_size_remain -= a_size_read;
		/* コア占有を防ぐためにsleep */
		usleep(1*1000);
	}

	/* Send Flash Read Response to pf_fmg thread (result = ret) */
	flashReadRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_READ_RSP;
	flashReadRsp.thdIf.uiSrcTQueueID = ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->thdIf.uiDstTQueueID;
	flashReadRsp.thdIf.uiDstTQueueID = ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID;
	flashReadRsp.thdIf.uiLength= sizeof(flashReadRsp);
	flashReadRsp.result = (ret != BPF_HM_DEVC_COMPLETE) ? D_SYS_NG : D_SYS_OK;
	flashReadRsp.size = a_size - a_size_remain;

	/* save running history */
	f_frwre_cm_RunHisSet(D_FRWRE_RUNHIS_FLG_SND, (UCHAR *)&flashReadRsp);
	
	/* Edit and send Response to pf_dia thread */
	f_com_msgQSendFHM( ((T_SYS_FLASH_READ_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID, 
					(CHAR*)&flashReadRsp,
					sizeof(T_SYS_FLASH_READ_RSP), 
					D_SYS_THDQID_PF_FRQRE, 
					f_frwrew_thrState, 
					D_SYS_MSGID_FLASH_RE_READ_RSP );

	cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_frwre_flashReadReq] RETURN" );		
    return ;    
}
/* @} */

