/*!
 * @skip  $ld:$
 * @file  f_fmng_flashEraseRsp.c
 * @brief Reception of flash erase response for pf_frw
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/13 TDIPS)sasaki RFIC関連処理削除
 * @date  2015/08/31 TDIPS)sasaki Update
 * @date  2015/10/21 TDIPS)sasaki Update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"

/*!
 *  @brief  Reception of Flash erase response
 *  @note   This function is processed as follows.\n
 *          - Stop FlashRom access timer.\n
 *          - If the flash is successfully erased, set
 *            LMC Flash slot state to erased.\n
 *          - Send API File Start Response to pf_send thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create
 *  @date  2015/06/16 ALP)murakami TDD-ZYNQ対応
 *  @date  2015/08/10 TDIPS)sasaki RFIC関連処理削除
 *  @date  2015/08/31 TDIPS)sasaki CT検出問題(NG応答受信時に処理中FW種別がクリアされない)の対処
 *  @date  2015/10/21 TDIPS)sasaki Update M-DCM-CBBUPF-03847対処
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_fmng_flashEraseRsp(void *pRcvMsg)
{
	T_SYS_FLSERASE_RSP 			*ptThrdif = NULL;
	UINT						result = D_SYS_NUM0;
	UINT						a_bootface;
	UINT						a_softver_p		= D_SYS_NUM0;				/* MPUバージョンアドレス		*/
	UINT						a_fpgaver_p		= D_SYS_NUM0;				/* FPGAバージョンアドレス		*/
	UINT						a_year_p		= D_SYS_NUM0;				/* 製造年アドレス				*/
	UINT						a_day_p			= D_SYS_NUM0;				/* 製造月日アドレス				*/

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashEraseRsp] ENTER" );					

	ptThrdif = (T_SYS_FLSERASE_RSP*)pRcvMsg;
	result = ptThrdif->result;


	if(ptThrdif->thdIf.uiSrcTQueueID == D_SYS_THDQID_PF_FRQ)
	{
		f_fmngw_flashRsp[0] = D_SYS_ON; /* frw */
	}
	else 
	{
		f_fmngw_flashRsp[1] = D_SYS_ON; /* frw(slave) */
	}
	
	if( result != D_SYS_OK )
	{
		/* Stop Flash earsing timer */
		f_fmng_com_stopTimer(E_RRH_TIMID_TDLWRTSV);

		/* reset the api kind */
		f_fmngw_apikind = D_FMNG_API_NONE;
		
		/* Change thread state into running */
		f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

		/* processing fw type clear */
		f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;

		f_fmng_com_sndLoadEraseRsp(result,"flash erase NG");
		
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashEraseRsp] RETURN to RUN(NG)" );
		
		return;
	}

	/* For slave, it should receive the response from pf_frw and pf_frwsla */
	if((f_fmngw_flashRsp[0] != D_SYS_ON) || (f_fmngw_flashRsp[1] != D_SYS_ON))
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashEraseRsp] RETURN for Another RSP" );					
		return;
	}
	
	/* Stop Flash earsing timer */
	f_fmng_com_stopTimer(E_RRH_TIMID_TDLWRTSV);

	/* reset the api kind */
	f_fmngw_apikind = D_FMNG_API_NONE;

	a_bootface = f_cmw_tra_inv_tbl->cmx_com_bootmen;
	a_bootface = (a_bootface & D_SYS_BIT1);
	
	/* プログラム起動面が0面のとき */
	if(a_bootface == D_RRH_BOOT_ACT0)
	{
		/* プログラム起動面が1面のファームウェアバージョンアドレスを取得 */
		a_softver_p = D_RRH_EEP_FLASH1_SOFT_VER;

		/* プログラム起動面が1面のファームウェア年月日アドレスを取得 */
		a_year_p = D_RRH_EEP_FLASH1_SOFT_YEAR;
		a_day_p  = D_RRH_EEP_FLASH1_SOFT_MONDAY;
	}
	/* プログラム起動面が1面のとき */
	else
	{
		/* プログラム起動面が0面のファームウェアバージョンアドレスを取得 */
		a_softver_p = D_RRH_EEP_FLASH0_SOFT_VER;
		
		/* プログラム起動面が0面のファームウェア年月日アドレスを取得 */
		a_year_p = D_RRH_EEP_FLASH0_SOFT_YEAR;
		a_day_p  = D_RRH_EEP_FLASH0_SOFT_MONDAY;
	}
	/**************************************/
	/* 年月日情報設定処理(ファームウェア) */
	/**************************************/
	f_fmng_cm_WriteEeprom(a_softver_p, 0);
	/* ソフト年情報を設定 */
	f_fmng_cm_WriteEeprom(a_year_p, 0);
	/* ソフト月日情報を設定 */
	f_fmng_cm_WriteEeprom(a_day_p, 0);
	/* set erasing flag of EEPROM to ON */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FIRM0, D_SYS_ON);	
	/* set erasing flag of shared memory to ON */
	f_cmw_tra_inv_tbl->cmx_erase_firm_flg = D_SYS_ON;	

    /**********************/
    /* PL-FPGA処理 */
    /**********************/
    /* PL-FPGA起動面情報が0面の場合 */
    if(a_bootface == D_RRH_BOOT_ACT0)
    {
    	/* PL-FPGA起動面が1面のPL-FPGAバージョンアドレスを取得 */
    	a_fpgaver_p = D_RRH_EEP_FLASH1_FPGA_RX;
    }
    else
    {
    	/* PL-FPGA起動面が0面のPL-FPGAバージョンアドレスを取得 */
    	a_fpgaver_p = D_RRH_EEP_FLASH0_FPGA_RX;
    }
    /*****************************/
    /* バージョン設定処理(PL-FPGA) */
    /*****************************/
    /* PL-FPGAバージョンを設定 */
    f_fmng_cm_WriteEeprom(a_fpgaver_p, 0);
    /* set erasing flag of EEPROM to ON */
    f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_RFRX0, D_SYS_ON);	
    /* set erasing flag of shared memory to ON */	
    f_cmw_tra_inv_tbl->cmx_erase_rx_flg = D_SYS_ON;	

	/**********************/
	/* CPRI-FPGA処理 */
	/**********************/
	a_bootface = f_cmw_tra_inv_tbl->cmx_cpri_bootmen;
	a_bootface = (a_bootface & D_SYS_BIT1);
	/* CPRI-FPGA起動面情報が0面の場合 */
	if(a_bootface == D_RRH_BOOT_ACT0)
	{
		/* CPRI-FPGA起動面が1面のCPRIFPGAバージョンアドレスを取得 */
		a_fpgaver_p = D_RRH_EEP_FLASH1_FPGA_FPGA;
	}
	else
	{
		/* CPRI-FPGA起動面が0面のCPRIFPGAバージョンアドレスを取得 */
		a_fpgaver_p = D_RRH_EEP_FLASH0_FPGA_FPGA;
	}
	/*****************************/
	/* バージョン設定処理(CPRI-FPGA) */
	/*****************************/
	/* CPRI-FPGAバージョンを設定 */
	f_fmng_cm_WriteEeprom(a_fpgaver_p, 0);
	/* set erasing flag of EEPROM to ON */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FPGA0, D_SYS_ON);	
	/* set erasing flag of shared memory to ON */	
	f_cmw_tra_inv_tbl->cmx_erase_fpga_flg = D_SYS_ON;	
    
	/*******************/	
	/* send erasing response */
	/*******************/	
	f_fmng_com_sndLoadEraseRsp(result,"flash erase OK");


	/*******************************/
	/* if thread state is flash R/W waiting,    */
	/* contine to process the writing request */
	/*******************************/
	if(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWWAIT)
	{
		/* set erasing flag of EEPROM to OFF */
		f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FIRM0, D_SYS_OFF);	
		/* set erasing flag of shared memory to OFF */
		f_cmw_tra_inv_tbl->cmx_erase_firm_flg = D_SYS_OFF;

		f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_RFRX0, D_SYS_OFF);	
		/* set erasing flag of shared memory to OFF */		
		f_cmw_tra_inv_tbl->cmx_erase_rx_flg = D_SYS_OFF; 

    	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FPGA0, D_SYS_OFF);	
    	/* set erasing flag of shared memory to OFF */		
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
		
		/* Change thread state into flash R/W */
		f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRW;

		/* reset the saved writing request */
		memset((CHAR*)&f_fmngw_flashWaitWrtReq, 0, sizeof(f_fmngw_flashWaitWrtReq));
		memset((CHAR*)&f_fmngw_flashWaitWrtReq_slave, 0, sizeof(f_fmngw_flashWaitWrtReq_slave));

		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashEraseRsp] RETURN to FLSHRW" );					
		return;
	}
	
	else if(f_fmngw_thrdState == D_FMNG_THRDSTA_TARPROCESS)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "RETURN to TARPROCESS %d",f_fmngw_preThrdState);
		/* no process */
		f_fmngw_preThrdState = D_FMNG_THRDSTA_FLSHRWWAIT;
		return;
	}
	
	/* Change thread state into running */
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashEraseRsp] RETURN to RUN" );					
	return ;    
}
/* @} */

