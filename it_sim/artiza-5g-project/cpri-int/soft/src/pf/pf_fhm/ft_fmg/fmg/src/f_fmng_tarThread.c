/*!
 * @skip  $ld:$
 * @file  f_fmng_tarThread.c
 * @brief tar thread process
 * @date  2014/02/20 FFCS)linb create
 * @date  2015/08/18 TDIPS)sasaki update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"


/*!
 *  @brief  tar thread process for firm download in USE state
 *  @note   This function is processed as follows.\n
 *          - if Erase flag (firm or fpag) send erase request to frw, and return;
 *          - otherwise,  send write request to frw
 *
 *
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @warning	don't get assertlog in this function.
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/

VOID f_fmng_tarThread()
{	
	INT		ret ;
	CHAR	unzipCmd[128];

	memset(unzipCmd, 0, sizeof(unzipCmd));
	
	prctl(PR_SET_NAME, "pf_tar", 0, 0, 0);

	/* decompress ZYNQ.tar.gz to \/tmp\/ */
	sprintf(unzipCmd, "tar -zxf %s%s -C %s ",D_SYS_UNZIP_DIRNAME,D_SYS_TARGZ_FILENAME,D_SYS_UNZIP_DIRNAME);
	ret = system(unzipCmd);

	/* send response to pf_fmng */
	if(ret == -1)
	{
		f_fmng_sendTarResult(D_RRH_NG);
	}
	else
	{
		f_fmng_sendTarResult(WEXITSTATUS(ret));
	}
	/* exit tar thread */
	BPF_RU_ITCM_THREAD_EXIT();
	return;
}


VOID f_fmng_sendTarResult(int ret)
{
	T_SYS_UNZIP_RSP	unzipRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_sendTarResult] ENTER" );				
	
	/* Initialize message */
	memset((CHAR*)(&unzipRsp),0, sizeof(unzipRsp));

	/************************************************************/
	/* Send Response to pf_fmng thread		*/
	/************************************************************/
	unzipRsp.thdIf.uiEventNo 	= D_SYS_MSGID_TARPROCESS_RSP;			/*!< Event  ID         */
	unzipRsp.thdIf.uiLength		= sizeof(unzipRsp);				/*!< Length            */
	unzipRsp.result				= ret;

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRMG, 
					(CHAR*)&unzipRsp,
					sizeof(unzipRsp), 
					D_SYS_THDQID_PF_FRMG, 
					D_SYS_NUM0, 
					D_SYS_MSGID_TARPROCESS_RSP );

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_sendTarResult] RETURN" );		
	
	return;
}

/*!
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/06/19 ALP) murakami TDD-ZYNQ対応
 *  @date  2015/08/18 TDIPS)sasaki MKレビュー指摘No.54対応
 *  @warning	don't get assertlog in this function.
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
VOID f_fmng_tarProcessRsp(void *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_tarProcessRsp] ENTER" );

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Uncompress End ret:%d",((T_SYS_UNZIP_RSP*)pRcvMsg)->result );

	if(((T_SYS_UNZIP_RSP*)pRcvMsg)->result != D_RRH_OK)
	{	
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"Uncompress error");
	}

	if((f_fmngw_preThrdState == D_FMNG_THRDSTA_FLSHRW) || 
		(f_fmngw_preThrdState == D_FMNG_THRDSTA_FLSHRWCANCEL))
	{
		/* clear previous thread state */
		f_fmngw_preThrdState = D_SYS_NUM0;
		
		/* Change thread state into flash R/W */
		f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRWWAIT;
		
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_tarProcessRsp] RETURN to FLSHRWWAIT" );
		
		return;
	}

	if(((T_SYS_UNZIP_RSP*)pRcvMsg)->result != D_RRH_OK)
	{	
		/* clear previous thread state */
		f_fmngw_preThrdState = D_SYS_NUM0;

		/* Change thread state into flash R/W cancel */
		f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRWCANCEL;

		return;
	}

	/* set erasing flag of EEPROM to OFF */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FIRM0, D_SYS_OFF);		
	/* set erasing flag of shared memory to OFF */
	f_cmw_tra_inv_tbl->cmx_erase_firm_flg = D_SYS_OFF;

	/* set erasing flag of EEPROM to OFF */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_RFRX0, D_SYS_OFF); 
   	/* set erasing flag of shared memory to OFF */		
   	f_cmw_tra_inv_tbl->cmx_erase_rx_flg = D_SYS_OFF; 

	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FPGA0, D_SYS_OFF); 
	/* set erasing flag of shared memory to ON */		
	f_cmw_tra_inv_tbl->cmx_erase_fpga_flg = D_SYS_OFF; 
	
	/********************/
	/* Send file info to pf_frw */	
	/********************/	
	f_fmngw_flashRsp[D_SYS_NUM0] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ, 
					(CHAR*)&f_fmngw_flashWaitWrtReq,
					sizeof(f_fmngw_flashWaitWrtReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC );

	/**********************/
	/* Send file info to pf_frwsla */	
	/**********************/
	f_fmngw_flashRsp[D_SYS_NUM1] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQS, 
					(CHAR*)&f_fmngw_flashWaitWrtReq_slave,
					sizeof(f_fmngw_flashWaitWrtReq_slave), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC );
	
	/********************/
	/* Start flash Write timer */
	/********************/
	f_fmng_com_startTimer(
			E_RRH_TIMID_TDLWRTSV,		/* Timer ID 			*/
			D_FMNG_TIMEVAL_FLSWRT,		/* Timer Value		*/
			D_SYS_MSGID_FLASH_TIMEOUT_NTC
			);		
	
	/* set received api kind */
	f_fmngw_apikind = D_FMNG_API_WRT;

	/* clear previous thread state */
	f_fmngw_preThrdState = D_SYS_NUM0;
	
	/* Change thread state into flash R/W */
	f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRW;
	
	/* reset the saved writing request */
	memset((CHAR*)&f_fmngw_flashWaitWrtReq, 0, sizeof(f_fmngw_flashWaitWrtReq));
	memset((CHAR*)&f_fmngw_flashWaitWrtReq_slave, 0, sizeof(f_fmngw_flashWaitWrtReq_slave));
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_tarProcessRsp] RETURN to FLSHRW" );		
	
	return;
}
/* @} */

