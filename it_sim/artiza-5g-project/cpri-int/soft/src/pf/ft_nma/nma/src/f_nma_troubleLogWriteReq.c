/*!
 * @skip  $ld:$
 * @file  f_nma_troubleLogWriteReq.c
 * @brief Reception of Trouble Log Flash Write request
 * @date  2014/01/30 ALPHA)yokoyama create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

 /*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

#include "f_nma_inc.h"


/*!
 *  @brief  Process of Trouble Log Flash Write request
 *  @note   This function is processed as follows.\n
 *          - Call function to write the flash.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2014/01/30 ALPHA)yokoyama create\n
 *  @date  2018/08/27 KCN)若水  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nma_troubleLogWriteReq(void *pRcvMsg)
{	
	FILE*								LOG_r;
	T_SYS_TROUBLE_LOG_WRITE_REQ			*pTrWrtReq = NULL;
	UINT								kind;
	UINT								face;
	UINT								offset;
	UINT								log_face = 0;
	LONG								size;
	INT									iReadSize;
	INT									iRet;
	CHAR				compfile[3][32] = {	"/var/log/trouble_rst00.tar.gz",
											"/var/log/trouble_rst01.tar.gz",
											"/var/log/trouble_rst02.tar.gz" };
	UINT				filename_index;

	/* Get trouble log write information */
	pTrWrtReq = (T_SYS_TROUBLE_LOG_WRITE_REQ*)pRcvMsg;
	
	/* ログ種別/Flash書き込み面/FlashOffset取得	*/
	kind	= pTrWrtReq->kind;
	face	= pTrWrtReq->face;
	offset	= pTrWrtReq->offset;

	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "troubleLogWrite Start kind:0x%08x,face:0x%08x,ofs:0x%08x",kind,face,offset);
	
	/* boot領域のアクセスを抑止 */
	if( offset < D_RRH_QSPI_FLASH_OFFSET_BOOT_END )
	{
		cmn_com_assert_th_get( f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "Flash offset invalid(face=%d, offset=%08x, kind=%d,)",
								face, offset, kind );
		f_com_almReport( D_SYS_THDID_PF_NMA, D_DU_FLTID_FWNG );
		return;
	}
	
	/* INITで圧縮すると時間がかかるためここで行う	*/
	if( D_RRH_LOGTYPE_TROUBLE == kind )
	{
		/* 面とOffsetから圧縮ファイル名を設定する	*/
		if( (face == 0) && (offset == D_RRH_QSPI_FLASH_OFFSET_TROUBLE0) ){
			filename_index = 0;
		}else if( (face == 0) && (offset == D_RRH_QSPI_FLASH_OFFSET_TROUBLE1) ){
			filename_index = 1;
		}else{
			filename_index = 2;
		}
		
		/* 圧縮ファイルをOPENする	*/
		LOG_r = fopen( compfile[filename_index] ,"rb");
		if(LOG_r == NULL)
		{
			cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_nma_troubleLogWriteReq] RETURN" );
			return;
		}
	}
	else
	{
		/* 圧縮ファイルをOPENする	*/
		LOG_r = fopen( "/var/log/healthchklog.tar.gz" ,"rb");
		if(LOG_r == NULL)
		{
			cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_nma_troubleLogWriteReq] RETURN" );
			return;
		}
	}
	
	/****************************************************************/
	/* FLASH CLEARを行ってからWRITEする								*/
	/* CLEARサイズは今回書き込む分のサイズ+復元用サイズ領域(4byte)	*/
	/****************************************************************/
	/* 書き込み対象のファイルサイズを求める */
	fseek(LOG_r, 0, SEEK_END);
	size = ftell(LOG_r);
	
	/* 復帰値を整合性を判定*/
	if(size == -1 ) { /* pgr0539 */
		cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_ERROR, "File tell error " );
		cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_nma_troubleLogWriteReq] RETURN" );
		fclose(LOG_r);
		return;
	}
	
	/* 2048kbyte以上は書き込み不可	*/
	if( (size + 4) >= D_RRH_QSPI_FLASH_TROUBLELOG_SIZE ){ /* pgr0539 */
		cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_ERROR, "File size is too big" );
		fclose(LOG_r);
		return;
	}

	fseek(LOG_r,0,SEEK_SET);
	
	/************************************************/
	/* FlashClearを実施								*/
	/* 実体＋復元用のサイズ(4byte)					*/
	/************************************************/
	BPF_HM_DEVC_QSPI_FLASH_ERASE( face, offset, (size + 4) );
	
	/* FileSizeを冒頭4byteに書き込む	*/
	memcpy( &f_nmaw_compressData[0],
			(CHAR*)&size,
			sizeof(size));
	
	iRet = BPF_HM_DEVC_QSPI_FLASH_WRITE(face,
										offset,
										4,
										f_nmaw_compressData);
	offset += 4;
	cmn_com_assert_th_get(f_nmaw_assert_p,D_RRH_LOG_AST_LV_INFO, "flash clear done. size:%d+4 offset = %08x",size,offset);
	
	/************************************************/
	/* FlashWriteを実施								*/
	/************************************************/
	while( (iReadSize = fread( f_nmaw_compressData, 1, D_NMA_COMPDATA, LOG_r ) ) != 0 )
	{
		iRet = BPF_HM_DEVC_QSPI_FLASH_WRITE(face,
											offset,
											iReadSize,
											f_nmaw_compressData);
		if( iRet != BPF_HM_DEVC_COMPLETE )
		{
			cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_nma_troubleLogWriteReq] RETURN" );
			fclose(LOG_r);
			return;
		}
		
		memset( f_nmaw_compressData, 0xFF, sizeof(f_nmaw_compressData) );
		offset += iReadSize;
	}
	
	if( D_RRH_LOGTYPE_TROUBLE == kind )
	{
		/* 障害ログの退避面を取得する	*/
		(VOID)BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_TROUBLE_LOG, (UCHAR*)&log_face );
		
		/* 退避面を更新	*/
		/* EEPROMへの反映は最後に行う	*/
		log_face++;
		if( log_face >= E_RRH_QSPI_FLASH_TROUBLELOG_FACE_MAX )
		{
			log_face = 0;
		}
		
		/* 障害ログ退避面を更新	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_TROUBLE_LOG, (UCHAR *)&log_face);
	}

	/* 圧縮ファイルをCLOSEする	*/
	fclose(LOG_r);

	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "troubleLogWrite End %d",log_face);

	return ;    
}

/* @} */
