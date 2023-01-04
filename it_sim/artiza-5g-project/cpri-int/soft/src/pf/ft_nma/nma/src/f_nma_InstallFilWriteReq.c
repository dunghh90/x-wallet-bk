/*!
 * @skip  $ld:$
 * @file  f_nma_InstallFilWriteReq.c
 * @brief File Update flash write request
 * @date  2019/02/20 KCN)kamimoto Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
#include "f_nma_inc.h"

/*!
 * @brief File Update Flash write request
 * @note  This function is processed as follows.\n
 *       -# Write EventLog
 *       -# Write AlarmHistoryLog
 * @param  *msgPtr    [in]VOID   received message
 * @return NON.
 * @date 2019/02/20 KCN)kamimoto Create\n
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nma_InstallFilWriteReq(VOID* pRcvMsg)
{
	T_SYS_NMA_FILE_WRITE_REQ *	RCVMsg;
	T_SYS_NMA_FILE_WRITE_RSP *	sndMsg;

	int							ret ;
	int							c_rtn ;
	int							errcd ;
	unsigned char 				er_flg	= D_SYS_NUM0 ;
	unsigned int				offset	= D_SYS_NUM0 ;
	unsigned int				face	= D_SYS_NUM0 ;
	unsigned int				size	= D_SYS_NUM0 ;
	unsigned char				wData;
	unsigned short				eep_offset = D_SYS_NUM0 ;
	void *						tmp_buf = NULL ;
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
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_InstallFilWriteReq buffer error=0x%08x", errcd);
		return ;
	}
	else
	{
		sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_RSP ;		/* RX-ON(M-Plane)応答				*/
		sndMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID			*/
		sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信先Thread Queue ID			*/
		sndMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID			*/
		sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信元Thread Queue ID			*/
		sndMsg->kind				= RCVMsg->kind;							/* ファイル種別(kind)				*/
	}

	/* アドレス変換処理 */
	switch( RCVMsg->kind )
	{
		/* FPGAのファイル更新(0面) */
		case	E_RRH_FILE_KIND_PLTFRM0:
			/* イレースフラグの読出し */
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F02_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_FPGA_PL ;
			size		= D_DU_QSPI_FLASH_SIZE_FPGA_PL ;
			face		= D_DU_QSPI_FLASH_FACE0 ;
			break;
		/* カーネル＋OSのファイル(0面) */
		case	E_RRH_FILE_KIND_KERNEL0:
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F01_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_KRNL_OS ;
			size		= D_DU_QSPI_FLASH_SIZE_KRNL_OS ;
			face		= D_DU_QSPI_FLASH_FACE0 ;
			break;
		/* RFICのファイル(ARM) (0面) */
		case	E_RRH_FILE_KIND_RFICARM0:
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F04_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_RFIC_BOOT ;
			size		= D_DU_QSPI_FLASH_SIZE_RFIC_BOOT ;
			face		= D_DU_QSPI_FLASH_FACE0 ;
			break;
		/* RFICのファイル(Firm) (0面) */
		case	E_RRH_FILE_KIND_RFICFRM0:
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F05_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_RFIC_ARM ;
			size		= D_DU_QSPI_FLASH_SIZE_RFIC_ARM ;
			face		= D_DU_QSPI_FLASH_FACE0 ;
			break;
		/* PTP-PLL初期設定ファイル(0面) */
		case	E_RRH_FILE_KIND_PTPPLL0:
			eep_offset	= D_DU_EEP_FSET1_FLASH0_F07_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_PTP_PARA ;
			size		= D_DU_QSPI_FLASH_SIZE_PTP_PARA ;
			face		= D_DU_QSPI_FLASH_FACE0 ;
			break;
		/* FPGAのファイル更新(1面) */
		case	E_RRH_FILE_KIND_PLTFRM1:
			/* イレースフラグの読出し */
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F02_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_FPGA_PL ;
			size		= D_DU_QSPI_FLASH_SIZE_FPGA_PL ;
			face		= D_DU_QSPI_FLASH_FACE1 ;
			break;
		/* カーネル＋OSのファイル(1面) */
		case	E_RRH_FILE_KIND_KERNEL1:
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F01_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_KRNL_OS ;
			size		= D_DU_QSPI_FLASH_SIZE_KRNL_OS ;
			face		= D_DU_QSPI_FLASH_FACE1 ;
			break;
		/* RFICのファイル(ARM) (1面) */
		case	E_RRH_FILE_KIND_RFICARM1:
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F04_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_RFIC_BOOT ;
			size		= D_DU_QSPI_FLASH_SIZE_RFIC_BOOT ;
			face		= D_DU_QSPI_FLASH_FACE1 ;
			break;
		/* RFICのファイル(Firm) (1面) */
		case	E_RRH_FILE_KIND_RFICFRM1:
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F05_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_RFIC_ARM ;
			size		= D_DU_QSPI_FLASH_SIZE_RFIC_ARM ;
			face		= D_DU_QSPI_FLASH_FACE1 ;
			break;
		/* PTP-PLL初期設定ファイル(1面) */
		case	E_RRH_FILE_KIND_PTPPLL1:
			eep_offset	= D_DU_EEP_FSET1_FLASH1_F07_EFLG ;
			offset		= D_DU_QSPI_FLASH_OFFSET_PTP_PARA ;
			size		= D_DU_QSPI_FLASH_SIZE_PTP_PARA ;
			face		= D_DU_QSPI_FLASH_FACE1 ;
			break;
		/* デフォルト（その他）		*/
		default:
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_InstallFilWriteReq Parameter NG kind=%08x", RCVMsg->kind);
			/* パラメータエラーの返却 */
			sndMsg->result = D_SYS_FLSRSLT_PRAERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
											(void*)sndMsg );				/* 送信Message						*/
			return ;
			break;
	}

	/* kind is erasing */
	(VOID)BPF_HM_DEVC_EEPROM_READ( eep_offset , &er_flg );
	if( er_flg != D_RRH_EEP_ERASE_ON )
	{
		/* Flash 未イレースなら */
		ret = f_nma_cm_EraseFlash(face, offset, size);
		if( ret != D_SYS_OK )
		{
			/* Assert */
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_cm_EraseFlash NG ret=%08x", ret);
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
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_InstallFilWriteReq File Open Error Filename = %s", filename );
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
		c_rtn = fread( (VOID*)&f_nmaw_dldata_buf , 1 , sizeof(f_nmaw_dldata_buf) , fp );
		if( c_rtn <= D_RRH_ZERO )
		{
			/* ファイルの読込み終了 */
			break ;
		}

		/* 読み出したファイルの書込み */
		ret = BPF_HM_DEVC_QSPI_FLASH_WRITE( face , offset , (unsigned int)c_rtn , (unsigned char*)&f_nmaw_dldata_buf );

		if( ret != BPF_HM_DEVC_COMPLETE )
		{
			/* 書込み失敗なら */
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_QSPI_FLASH_WRITE Error ret = 0x%08x size = 0x%x", ret , c_rtn);
			sndMsg->result = D_SYS_FLSRSLT_WRTERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
											(void*)sndMsg );				/* 送信Message						*/
			free( tmp_buf ) ;
			fclose( fp ) ;
			return ;
		}
		offset = offset + c_rtn ;
	}

	/* 書込み終了応答の送信 */
	sndMsg->result = D_SYS_FLSRSLT_OK ;
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL ,			/* 送信先Thread Queue ID			*/
									(void*)sndMsg );				/* 送信Message						*/
	free( tmp_buf ) ;
	fclose( fp ) ;
	return ;
}
/* @} */


