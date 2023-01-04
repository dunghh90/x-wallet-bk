/*!
 * @skip  $ld:$
 * @file  f_nmas_InstallFilWriteReq.c
 * @brief File Update flash write request
 * @date  2019/02/21 KCN)kamimoto Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */
#include "f_nmas_inc.h"

/*!
 * @brief File Update Flash write request
 * @note  This function is processed as follows.\n
 *       -# Write EventLog
 *       -# Write AlarmHistoryLog
 * @param  *msgPtr    [in]VOID   received message
 * @return NON.
 * @date 2019/02/21 KCN)kamimoto Create\n
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nmas_InstallFilWriteReq(VOID* pRcvMsg)
{
	T_SYS_NMA_FILE_WRITE_REQ *	RCVMsg;
	T_SYS_NMA_FILE_WRITE_RSP *	sndMsg;

	int							ret ;
	int							c_rtn ;
	int							errcd ;
	unsigned char 				er_flg	= D_SYS_NUM0 ;
	unsigned int				offset	= D_SYS_NUM0 ;
	unsigned int				size	= D_SYS_NUM0 ;
	unsigned char				wData;
	unsigned short				eep_offset = D_SYS_NUM0 ;
	FILE *						fp ;
	char *						filename = NULL ;

	/* 初期設定(返却バッファの確保) */
	RCVMsg = (T_SYS_NMA_FILE_WRITE_REQ *)pRcvMsg ;
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_NMA_FILE_WRITE_RSP),		/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg							*/
										&errcd );
	if( ret != D_SYS_OK )
	{
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nmas_InstallFilWriteReq buffer error=0x%08x", errcd);
		return ;
	}
	else
	{
		sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMAS_FILE_WRITE_RSP ;		/* RX-ON(M-Plane)応答				*/
		sndMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID			*/
		sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信先Thread Queue ID			*/
		sndMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID			*/
		sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_NMAS;					/* 送信元Thread Queue ID			*/
		sndMsg->kind				= RCVMsg->kind;							/* ファイル種別(kind)				*/
	}

	/* アドレス変換処理 */
	switch(RCVMsg->kind)
	{
		case	E_RRH_FILE_KIND_TXRPL0 :									/* TRX-PL(Slave)のファイル更新(0面) */
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F03_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_TRX_FPGA_FACE0 ;
			size		= D_DU_QSPI_FLASH_SIZE_TRX_FPGA ;
			break ;
		case	E_RRH_FILE_KIND_TXRPL1 :									/* TRX-PL(Slave)のファイル更新(1面) */
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F03_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_TRX_FPGA_FACE1 ;
			size		= D_DU_QSPI_FLASH_SIZE_TRX_FPGA ;
			break ;
		default :															/* その他(エラー) */
			cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nmas_InstallFilWriteReq Parameter NG kind=%08x", RCVMsg->kind);
			/* パラメータエラーの返却 */
			sndMsg->result = D_SYS_FLSRSLT_PRAERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
											(void*)sndMsg );				/* 送信Message						*/
			return ;
			break ;
	}

	/* kind is erasing */
	(VOID)BPF_HM_DEVC_EEPROM_READ( eep_offset , &er_flg );
	if( er_flg != D_RRH_EEP_ERASE_ON )
	{
		/* Flash 未イレースなら */
		ret = f_nmas_cm_EraseFlash(offset, size);
		if( ret != D_SYS_OK )
		{
			/* Assert */
			cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nmas_cm_EraseFlash NG ret=%08x", ret);
			sndMsg->result = D_SYS_FLSRSLT_ERSERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
											(void*)sndMsg );				/* 送信Message						*/
			return ;
		}
		else
		{
			/* EEP-ROMのイレースフラグをON設定 */
			wData = D_RRH_EEP_ERASE_ON ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE( eep_offset , &wData );
		}
	}

	filename = &RCVMsg->name[0] ;
	/* ファイル名から更新データをリード */
	if(( fp = fopen(filename , "rb") ) == NULL)
	{
		/* ファイルオープン失敗 */
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nmas_InstallFilWriteReq File Open Error Filename = %s", filename );
		sndMsg->result = D_SYS_FLSRSLT_OPNERR ;
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
										(void*)sndMsg );				/* 送信Message						*/
		return ;
	}

	/* EEP-ROMのイレースフラグをOFF設定 */
	wData = D_RRH_EEP_ERASE_OFF ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( eep_offset , &wData );

	while ( 1 )
	{
		/* ファイルリード */
		c_rtn = fread( (VOID*)&f_nmasw_dldata_buf , 1 , sizeof(f_nmasw_dldata_buf) , fp );
		if( c_rtn <= D_RRH_ZERO )
		{
			/* ファイルの読込み終了 */
			break ;
		}

		/* 読み出したファイルの書込み */
		ret = BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE( offset , (unsigned int)c_rtn , (unsigned char*)&f_nmasw_dldata_buf );

		if( ret != BPF_HM_DEVC_COMPLETE )
		{
			/* 書込み失敗なら */
			cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_QSPI_FLASH_WRITE Error ret = 0x%08x size = 0x%x", ret , c_rtn);
			sndMsg->result = D_SYS_FLSRSLT_WRTERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
											(void*)sndMsg );				/* 送信Message						*/
			fclose( fp ) ;
			return ;
		}
		offset = offset + c_rtn ;
	}

	/* 書込み終了応答の送信 */
	sndMsg->result = D_SYS_FLSRSLT_OK ;
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
									(void*)sndMsg );				/* 送信Message						*/
	fclose( fp ) ;
	return ;
}
/* @} */


