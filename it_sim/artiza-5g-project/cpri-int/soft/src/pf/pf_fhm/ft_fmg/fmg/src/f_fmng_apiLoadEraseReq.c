/*!
 * @skip  $ld:$
 * @file  f_fmng_apiLoadEraseReq.c
 * @brief Reception of API Load Erase request
 * @date  2013/11/15 FFCS)linb create
 * @date  2013/08/13 TDIPS)sasaki RFICファイル関連の処理を削除
 * @date  2015/10/21 TDIPS)sasaki Update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"

/*!
 *  @brief  Send API Load Erase Response (result = NG) 
 *  @note   This function is processed as follows.\n
 *          - Send API Load Erase Response (result = NG) .\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_rejApiLoadEraseReq(void *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_rejApiLoadEraseReq] ENTER" );				

	/* Send API Load Update Response (result = NG)  */
	f_fmng_com_sndLoadEraseRsp(D_RRH_NG,"flash is accessed");

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_rejApiLoadEraseReq] RETURN" );				
	return;
}

/*!
 *  @brief  Reception of API Load Erase request
 *  @note   This function is processed as follows.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create
 *  @date  2015/06/15 ALP)murakami TDD-ZYNQ対応
 *  @date  2015/07/14 ALP)murakami M-RRU-ZSYS-01643 OLIVE対応
 *  @date  2015/10/21 TDIPS)sasaki Update M-DCM-CBBUPF-03847対処
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_apiLoadEraseReq(void *pRcvMsg)
{
	USHORT		a_boot		= D_SYS_NUM0;		/* 起動面						*/
	USHORT		a_erase_flg	= D_SYS_OFF;		/* Eraseフラグ					*/
	UINT		a_index		= D_SYS_NUM0;	/* clear rom index */
	UINT		a_index_slave = D_SYS_NUM0;	/* clear rom index */
	UINT		a_face		= D_SYS_NUM0;
	T_API_FILE_ERASE_FIRM_REQ	*ptThrdif = NULL;
	T_SYS_FLSERASE_REQ			flshEraseReq;
	T_SYS_FLSERASE_REQ			flshEraseReq_slave;
	T_SYS_LMCWRT_RSP			*pLmcWrtRsp = NULL;
	T_RRH_SIDE_USE 				ltside_fhm;
	T_RRH_FW_INFO				a_fwinfo;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_apiLoadEraseReq] ENTER" );				

	/* Initialize API response message */
	memset((CHAR*)(&flshEraseReq), 0,sizeof(flshEraseReq));
	memset((CHAR*)(&flshEraseReq_slave), 0,sizeof(flshEraseReq_slave));

	/* if thread state is cancel updating firm */
	/* get response QID from the writing respose message */
	if(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWCANCEL)
	{
		/* Get Common thread header from Receivd message */
		pLmcWrtRsp = (T_SYS_LMCWRT_RSP*)pRcvMsg;
		
		/* Save Response QId  */
		f_fmngw_respQid = pLmcWrtRsp->thdIf.uiSrcPQueueID;
	}
	else
	/* thread state is running */
	{
		/* Get Common thread header from Receivd message */
		ptThrdif = (T_API_FILE_ERASE_FIRM_REQ*)pRcvMsg;

		/* Save Response QId  */
		f_fmngw_respQid = ptThrdif->header.uiSrcPQueueID;
	}
	
	/* change the bootface to the current using side */
	f_fmng_cm_ChgBootFace( D_SYS_BOOTFACE_USE );
	f_fmng_cm_ChgBootFace_cprifpga( D_SYS_BOOTFACE_USE );

	/* set the startup version the same as use version	*/
	f_fmngw_stupver_tbl = f_fmngw_usever_tbl;

	/*********************************************************************************/
	/* FHM保持FWファイル情報(運用面)を FHM保持FWファイル情報(待機面)共有メモリに展開 */
	/*********************************************************************************/
	f_cmn_com_side_use_get(&ltside_fhm);
	f_cmn_com_fw_info_fhm_get(ltside_fhm.use_side, &a_fwinfo);
	f_cmn_com_fw_info_fhm_set((ltside_fhm.use_side ^ 0x1), &a_fwinfo);

	/* send SubGetVerRsp to update values */
	f_fmng_com_sndSubGetVerRsp();

	a_boot = f_fmngw_usebootface;
	
	/************************************************************************************************************************/
	/* ファームウェアのFLASHイレース処理																					*/
	/************************************************************************************************************************/
	/* EEPROMからイレースフラグ情報を取得 */
	a_erase_flg = f_cmw_tra_inv_tbl->cmx_erase_firm_flg;
	a_erase_flg = (a_erase_flg & D_SYS_BIT1);

	/* イレースフラグがON(消去)の場合は続いてFPGA FLASHのイレース処理を実施 */
	if( (a_erase_flg != D_SYS_ON) ||
		(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWCANCEL) )
	{
		/* EEPROMから起動面情報を取得 */
		/* 起動面がFlash 0面の場合 */
		if (a_boot == D_RRH_BOOT_ACT0)
		{
			/* 消去面を1面に設定 */
			a_face	= D_RRH_QSPI_FLASH_FACE1 ;
		}
		/* 起動面がFlash 1面の場合 */
		else
		{
			/* 消去面を0面に設定 */
			a_face	= D_RRH_QSPI_FLASH_FACE0 ;
		}
		
		flshEraseReq.clearFlashRom[a_index].face = a_face;
		flshEraseReq.clearFlashRom[a_index].offset = D_RRH_QSPI_FLASH_OFFSET_OS;
		flshEraseReq.clearFlashRom[a_index++].size = D_DU_QSPI_FLASH_SIZE_FIRM;
	}

    /************************************************************************************************************************/
    /* PL-FPGAのFLASHイレース処理																								*/
    /************************************************************************************************************************/
    /* EEPROMからイレースフラグ情報を取得 */
    a_erase_flg = f_cmw_tra_inv_tbl->cmx_erase_rx_flg;
    a_erase_flg = (a_erase_flg & D_SYS_BIT1);
    /* イレースフラグがON(消去)の場合は無処理 */
    if( (a_erase_flg != D_SYS_ON) ||
    	(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWCANCEL) )
    {
    	/* 起動面がFlash 0面の場合 */
    	if (a_boot == D_RRH_BOOT_ACT0)
    	{
    		/* 消去面を1面に設定 */
    		a_face	= D_RRH_QSPI_FLASH_FACE1;
    	}
    	/* 起動面がFlash 1面の場合 */
    	else
    	{
    		/* 消去面を0面に設定 */
    		a_face	= D_RRH_QSPI_FLASH_FACE0;
    	}
    	flshEraseReq.clearFlashRom[a_index].face = a_face;
    	flshEraseReq.clearFlashRom[a_index].offset = D_RRH_QSPI_FLASH_OFFSET_PL;
    	flshEraseReq.clearFlashRom[a_index++].size = D_DU_QSPI_FLASH_SIZE_PL;
    }
	
	/************************************************************************************************************************/
    /* CPRI-FPGAのFLASHイレース処理                                                                                         */
	/************************************************************************************************************************/
	a_boot = f_fmngw_usebootface_cprifpga;
	/* EEPROMからイレースフラグ情報を取得 */
	a_erase_flg = f_cmw_tra_inv_tbl->cmx_erase_fpga_flg;
	a_erase_flg = (a_erase_flg & D_SYS_BIT1);
	/* イレースフラグがON(消去)の場合は無処理 */
	if( (a_erase_flg != D_SYS_ON) ||
		(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWCANCEL) )
	{
		/* 起動面がFlash 0面の場合 */
		if (a_boot == D_RRH_BOOT_ACT0)
		{
			/* 消去面を1面に設定 */
            flshEraseReq_slave.clearFlashRom[a_index_slave].face = D_RRH_QSPI_FLASH_FACE1;
			flshEraseReq_slave.clearFlashRom[a_index_slave].offset = D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE1;
		}
		/* 起動面がFlash 1面の場合 */
		else
		{
			/* 消去面を0面に設定 */
            flshEraseReq_slave.clearFlashRom[a_index_slave].face = D_RRH_QSPI_FLASH_FACE0;
			flshEraseReq_slave.clearFlashRom[a_index_slave].offset = D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE0;
		}
		flshEraseReq_slave.clearFlashRom[a_index_slave++].size = D_DU_QSPI_FLASH_SIZE_FPGA;
    }

    /************************************************************************************************************************/
    /* FLASHイレース処理の回数設定                                                                                             */
    /************************************************************************************************************************/
    flshEraseReq.clearFileNum = a_index;
    flshEraseReq_slave.clearFileNum = a_index_slave;

	/*******************************************************/
	/* Edit and send Flash Erase Request to pf_frw thread */
	/*******************************************************/
	flshEraseReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_ERASE_REQ;
	flshEraseReq.thdIf.uiLength= sizeof(flshEraseReq);

	f_fmngw_flashRsp[D_SYS_NUM0] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ, 
					(CHAR*)&flshEraseReq,
					sizeof(flshEraseReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC);

	/*************************************************************/
	/* Edit and send Flash Erase Request to pf_frw(slave) thread */
	/*************************************************************/
	flshEraseReq_slave.thdIf.uiEventNo = D_SYS_MSGID_FLASH_ERASE_REQ;
	flshEraseReq_slave.thdIf.uiLength= sizeof(flshEraseReq_slave);

	f_fmngw_flashRsp[D_SYS_NUM1] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQS, 
					(CHAR*)&flshEraseReq_slave,
					sizeof(flshEraseReq_slave), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC );
	
	/* Start flash Write timer */
	f_fmng_com_startTimer(
		E_RRH_TIMID_TDLWRTSV,			/* Timer ID				*/
		D_FMNG_TIMEVAL_FLSERASE	,		/* Timer Value			*/
		D_SYS_MSGID_FLASH_TIMEOUT_NTC
		);		

	/* set received api kind */
	f_fmngw_apikind = D_FMNG_API_ERA;

	/* Change thread state into flash reading or writing */
	f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRW;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiLoadEraseReq] RETURN to FLSHRW" );				
	return ;
}
/* @} */

