/*!
 * @skip  $ld:$
 * @file  f_fmng_flashWrtRsp.c
 * @brief Reception of Flash writing response
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/13 TDIPS)sasaki RFIC関連処理削除
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date  2015/08/31 TDIPS)sasaki Update
 * @date  2015/09/16 TDIPS)sasaki Update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"


/*!
 *  @brief  Reception of Flash writing response
 *  @note   This function is processed as follows.\n
 *          - Stop Flash write timer.\n
 *          - If the response is NG, send API Load Update Response
 *            (result = NG).\n
 *          - Update File information to EEPROM and RAM table.\n
 *          - Send API File Update Response to pf_send thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/06/16 ALP)murakami TDD-ZYNQ対応
 *  @date  2015/08/10 TDIPS)sasaki RFIC関連処理削除
 *  @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 *  @date  2015/08/31 TDIPS)sasaki CT検出問題(NG応答受信時に処理中FW種別がクリアされない)の対処
 *  @date  2015/09/16 TDIPS)sasaki FHM保持FWファイル情報の共有メモリへの展開処理追加
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_fmng_flashWrtRsp(void *pRcvMsg)
{
	T_SYS_LMCWRT_RSP			*pLmcWrtRsp = NULL;
	T_SYS_RCVDATA_MKHEAD_S3G 	*rcvdata_p = NULL;
	UINT						result;	
	T_RRH_FW_INFO				a_fwinfo;
	USHORT						a_save_side;

	UINT	a_softver_p		= D_SYS_NUM0;				/* MPUバージョンアドレス		*/
	UINT	a_fpgaver_p		= D_SYS_NUM0;				/* FPGAバージョンアドレス		*/
	UINT	a_year_p		= D_SYS_NUM0;				/* 製造年アドレス				*/
	UINT	a_day_p			= D_SYS_NUM0;				/* 製造月日アドレス				*/
	USHORT	a_bootface		= D_SYS_NUM0;				/* プログラム起動面				*/
	USHORT	a_soft_ver		= D_SYS_NUM0;				/* ソフトバージョン				*/
	USHORT	a_firm_ver		= D_SYS_NUM0;				/* ファームバージョン			*/
	USHORT	a_firm_year		= D_SYS_NUM0;				/* ファーム製造年情報			*/
	USHORT	a_firm_day		= D_SYS_NUM0;				/* ファーム製造月日情報			*/
	UINT	a_firmsize_h_p	= D_SYS_NUM0;				/* ファームサイズ(上位)			*/
	UINT	a_firmsize_l_p	= D_SYS_NUM0;				/* ファームサイズ(下位)			*/
	USHORT	a_fpga_ver		= D_SYS_NUM0;				/* FPGAバージョン				*/
	UINT	a_fpgasize_h_p 	= D_SYS_NUM0;				/* FPGAサイズ(上位)				*/
	UINT	a_fpgasize_l_p 	= D_SYS_NUM0;				/* FPGAサイズ(下位)				*/	
	USHORT	rcv_mjr_ver		= D_SYS_NUM0;
	USHORT	rcv_min_ver		= D_SYS_NUM0;
	USHORT	rcv_year		= D_SYS_NUM0;
	UCHAR	almreset_flag;
	INT		endcd;
	
		
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashWrtRsp] ENTER" );					

	/* Get the pointer of LMC Write Response  */
	pLmcWrtRsp = (T_SYS_LMCWRT_RSP*)pRcvMsg;
	result = pLmcWrtRsp->result;

	if(pLmcWrtRsp->thdIf.uiSrcTQueueID == D_SYS_THDQID_PF_FRQ)
	{
		f_fmngw_flashRsp[0] = D_SYS_ON; /* frw */
	}
	else 
	{
		f_fmngw_flashRsp[1] = D_SYS_ON; /* frw(slave) */	
	}
	
	if( result != D_SYS_OK )
	{
		/* Stop Flash write timer */
		f_fmng_com_stopTimer(E_RRH_TIMID_TDLWRTSV);
		
		/* reset the api kind */
		f_fmngw_apikind = D_FMNG_API_NONE;
	
		/* Change thread state into running */
		f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

		/* processing fw type clear */
		f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;

		f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"flash write NG");
		
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashWrtRsp] RETURN to RUN(NG)" );
		
		return;
	}
	
	/* For slave, it should receive the response from pf_frw and pf_frwsla */
	if((f_fmngw_flashRsp[0] != D_SYS_ON) || (f_fmngw_flashRsp[1] != D_SYS_ON))
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashWrtRsp] RETURN for Another RSP" );					
		return;
	}
	
	/* get pointers from data */
	rcvdata_p = (T_SYS_RCVDATA_MKHEAD_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32);

	/* Stop Flash write timer */
	f_fmng_com_stopTimer(E_RRH_TIMID_TDLWRTSV);

	/* reset the api kind */
	f_fmngw_apikind = D_FMNG_API_NONE;

	/******************/
	/* データ取得処理 */
	/******************/
	/* ソフトバージョンをデータ格納テーブルより取得 */
	rcv_mjr_ver = swap_16(rcvdata_p->mjr_ver);
	rcv_min_ver = swap_16(rcvdata_p->min_ver);
	rcv_year = swap_16(rcvdata_p->year);

	a_soft_ver = (USHORT)
		((((( rcv_mjr_ver >> D_SYS_BIT8) - '0') & D_SYS_VER_MSK ) << D_SYS_BIT12 ) |
		  ((( rcv_mjr_ver			  - '0') & D_SYS_VER_MSK ) << D_SYS_BIT8 )	|
		 (((( rcv_min_ver >> D_SYS_BIT8) - '0') & D_SYS_VER_MSK ) << D_SYS_BIT4 )	|
		  ((( rcv_min_ver			  - '0') & D_SYS_VER_MSK )));
	
	/* ファームウェアバージョンを取得 */
	a_firm_ver = a_soft_ver;
	
	/* ファームウェア年月日をデータ格納テーブルより取得 */
	a_firm_year = rcv_year;
	a_firm_day	= (rcvdata_p->month << D_SYS_BIT8) | (rcvdata_p->day);

	/*****************************/
	/* FHM保持FWファイル情報作成 */
	/*****************************/
	a_fwinfo.maker_id = 'X';
	a_fwinfo.major_ver = (((rcv_mjr_ver >> 8) - '0') * 10) + ((rcv_mjr_ver - '0') & D_SYS_VER_MSK);
	a_fwinfo.minor_ver = (((rcv_min_ver >> 8) - '0') * 10) + ((rcv_min_ver - '0') & D_SYS_VER_MSK);
	a_fwinfo.year = ((rcv_year >> 12) * 1000) + (((rcv_year >> 8) & 0xF) * 100) + (((rcv_year >> 4) & 0xF) * 10) + (rcv_year & 0xF);
	a_fwinfo.month = ((rcvdata_p->month >> 4) * 10) + (rcvdata_p->month & 0xF);
	a_fwinfo.day = ((rcvdata_p->day >> 4) * 10) + (rcvdata_p->day & 0xF);

	/* FPGA*/
	a_fpga_ver =  a_soft_ver;
		
	/**********************/
	/* ファームウェア処理 */
	/**********************/
	/* read the bootface from common table */
	a_bootface = f_cmw_tra_inv_tbl->cmx_com_bootmen;
	a_bootface = (a_bootface & D_SYS_BIT1);
	/* プログラム起動面が0面のとき */
	if(a_bootface == D_RRH_BOOT_ACT0)
	{
		/* プログラム起動面が1面のファームウェアバージョンアドレスを取得 */
		a_softver_p = D_RRH_EEP_FLASH1_SOFT_VER;
		a_save_side = 1;

		/* プログラム起動面が1面のファームウェア年月日アドレスを取得 */
		a_year_p = D_RRH_EEP_FLASH1_SOFT_YEAR;
		a_day_p  = D_RRH_EEP_FLASH1_SOFT_MONDAY;
		
		/* プログラム起動面が1面のファームウェアファイルサイズを取得 */
		a_firmsize_h_p = D_RRH_EEP_FIRM1_CHECKSUM_CALC_H0;
		a_firmsize_l_p = D_RRH_EEP_FIRM1_CHECKSUM_CALC_L0;
		
		f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM0] = a_firm_ver;
		f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM1] = a_firm_year;
		f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM2] = a_firm_day;
	}
	/* プログラム起動面が1面のとき */
	else
	{
		/* プログラム起動面が0面のファームウェアバージョンアドレスを取得 */
		a_softver_p = D_RRH_EEP_FLASH0_SOFT_VER;
		a_save_side = 0;

		/* プログラム起動面が0面のファームウェア年月日アドレスを取得 */
		a_year_p = D_RRH_EEP_FLASH0_SOFT_YEAR;
		a_day_p  = D_RRH_EEP_FLASH0_SOFT_MONDAY;
		
		/* プログラム起動面が0面のファームウェアファイルサイズを取得 */
		a_firmsize_h_p = D_RRH_EEP_FIRM0_CHECKSUM_CALC_H0;
		a_firmsize_l_p = D_RRH_EEP_FIRM0_CHECKSUM_CALC_L0;

		f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM0] = a_firm_ver;
		f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM1] = a_firm_year;
		f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM2] = a_firm_day;
	}

	/**************************************/
	/* 年月日情報設定処理(ファームウェア) */
	/**************************************/
	/* ソフト年情報を設定 */
	f_fmng_cm_WriteEeprom(a_year_p, a_firm_year);
	/* ソフト月日情報を設定 */
	f_fmng_cm_WriteEeprom(a_day_p, a_firm_day);
	
	/**********************/
	/* PL-FPGA処理 */
	/**********************/
	/* PL-FPGA起動面情報が0面の場合 */
	if(a_bootface == D_RRH_BOOT_ACT0)
	{
		/* PL-FPGA起動面が1面のPL-FPGAバージョンアドレスを取得 */
		a_fpgaver_p = D_RRH_EEP_FLASH1_FPGA_RX;
		/* PL-FPGA起動面が1面のPL-FPGAファイルサイズを取得 */
		a_fpgasize_h_p = D_RRH_EEP_APDIC1_CHECKSUM_CALC_H0;
		a_fpgasize_l_p = D_RRH_EEP_APDIC1_CHECKSUM_CALC_L0;
	}
	else
	{
		/* PL-FPGA起動面が0面のPL-FPGAバージョンアドレスを取得 */
		a_fpgaver_p = D_RRH_EEP_FLASH0_FPGA_RX;
		/* PL-FPGA起動面が0面のPL-FPGAファイルサイズを取得 */
		a_fpgasize_h_p = D_RRH_EEP_APDIC0_CHECKSUM_CALC_H0;
		a_fpgasize_l_p = D_RRH_EEP_APDIC0_CHECKSUM_CALC_L0;
	}

	/*****************************/
	/* バージョン設定処理(PL-FPGA) */
	/*****************************/
	/* PL-FPGAバージョンを設定 */
	f_fmng_cm_WriteEeprom(a_fpgaver_p, a_fpga_ver);
	/* PL-FPGAファイルサイズを設定 */
	f_fmng_cm_WriteEeprom(a_fpgasize_h_p, (USHORT)(f_fmngw_hard_pl_decode_size >> D_SYS_NUM16));
	f_fmng_cm_WriteEeprom(a_fpgasize_l_p, (USHORT)f_fmngw_hard_pl_decode_size);

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
		
		/* CPRI-FPGA起動面が1面のCPRIFPGAファイルサイズを取得 */
		a_fpgasize_h_p = D_RRH_EEP_FPGA1_CHECKSUM_CALC_H0;
		a_fpgasize_l_p = D_RRH_EEP_FPGA1_CHECKSUM_CALC_L0;
	}
	else
	{
		/* CPRI-FPGA起動面が0面のCPRIFPGAバージョンアドレスを取得 */
		a_fpgaver_p = D_RRH_EEP_FLASH0_FPGA_FPGA;
		
		/* CPRI-FPGA起動面が0面のCPRIFPGAファイルサイズを取得 */
		a_fpgasize_h_p = D_RRH_EEP_FPGA0_CHECKSUM_CALC_H0;
		a_fpgasize_l_p = D_RRH_EEP_FPGA0_CHECKSUM_CALC_L0;
	}
	
	/*****************************/
	/* バージョン設定処理(CPRI-FPGA) */
	/*****************************/
	/* CPRI-FPGAバージョンを設定 */
	f_fmng_cm_WriteEeprom(a_fpgaver_p, a_fpga_ver);
	/* CPRIFPGAファイルサイズを設定 */
	f_fmng_cm_WriteEeprom(a_fpgasize_h_p, (USHORT)(f_fmngw_hard_cpri_decode_size >> D_SYS_NUM16));
	f_fmng_cm_WriteEeprom(a_fpgasize_l_p, (USHORT)f_fmngw_hard_cpri_decode_size);
	
	/******************/
	/* 起動面変更処理 */
	/******************/
	f_fmng_cm_ChgBootFace(D_SYS_BOOTFACE_OPP );
	/* for cprifpga */
	f_fmng_cm_ChgBootFace_cprifpga( D_SYS_BOOTFACE_OPP );
	/**************************************/
	/* バージョン設定処理(ファームウェア) */
	/**************************************/
	/* ファームウェアバージョンを設定 */
	f_fmng_cm_WriteEeprom(a_softver_p, a_firm_ver);
	/* ファームウェアファイルサイズを設定 */
	f_fmng_cm_WriteEeprom(a_firmsize_h_p, (USHORT)(f_fmngw_hard_firm_decode_size >> D_SYS_NUM16));
	f_fmng_cm_WriteEeprom(a_firmsize_l_p, (USHORT)f_fmngw_hard_firm_decode_size);
	/* save the next RE startup version */
	f_fmngw_stupver_tbl = a_firm_ver;
	/* send SubGetVerRsp to update values */
	f_fmng_com_sndSubGetVerRsp();

	/*******************************************/
	/* FHM保持FWファイル情報を共有メモリに展開 */
	/*******************************************/
	f_cmn_com_fw_info_fhm_set(a_save_side, &a_fwinfo);


	/************************************************************/
	/* Edit and send Firm Update Response to pf_Send thread		*/
	/************************************************************/
	/* Send API Load Update Response   */
	f_fmng_com_sndLoadUpdRsp(result,"flash write OK");

	/* Change thread state into running */
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

	//TODO 自律リセットフラグ要否未確定。とりあえずそのまま残しておく
    /* 自律リセットフラグをOFFする */
    almreset_flag = D_RRH_ALM_SELF_RESET_OFF;
    endcd = BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_ALM_SELF_RESET_FLG, &almreset_flag);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEPROM(0x%08x) write error=0x%08x", D_RRH_EEP_ALM_SELF_RESET_FLG, endcd);
	}
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashWrtRsp] RETURN to RUN" );
	return ;
}
/* @} */

