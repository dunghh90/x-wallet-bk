/*!
 * @skip  $ld:$
 * @file  f_nma_common.c
 * @brief Process of Frw common function
 * @date  2013/11/13 FFCS)linb create
 * @date  2018/08/27 KCN)若水  create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

 /*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

#include "f_nma_inc.h"


static unsigned char f_nmarew_readdata[16];

/*!
 *  @brief  the function for flash 4 byte read before writing and erasing.
 *  @note   This function is processed as follows.
 *          1)re-file　flash read before writing and erasing.
 *  @param  face         [in] FlashROM face
 *  @param  flshOffset   [in] FlashROM offset
 *  @return VOID
 *  @date   2016/01/08 FJT)harada Create
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_nma_cm_ReadFlash(UINT face, UINT flshOffset)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;
	INT		errcd;
	UCHAR	*invTbl;
	
	/* 共有メモリアドレス取得 */
	rtn = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(BPF_RU_IPCM_OK != rtn)
	{
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "share memory address get NG(%d)",rtn);
		return;
	}
	
	memset(f_nmarew_readdata,0x55,sizeof(f_nmarew_readdata));
	if(( rtn = BPF_HM_DEVC_QSPI_FLASH_READ(face, flshOffset, 4, f_nmarew_readdata)) == BPF_HM_DEVC_COMPLETE ){
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_INFO, "Before re-file flash%d read(0x%02x%02x%02x%02x)" ,
			face, f_nmarew_readdata[0], f_nmarew_readdata[1],  f_nmarew_readdata[2],  f_nmarew_readdata[3] );
	}
	else{
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR , "re-file flash%d read NG(%d)",face, rtn );
	}
	return;
}


/*!
 *  @brief  the function for flash erasing.
 *  @note   This function is processed as follows.
 *          1) start up the common function of flash erasing,if flash is faild,abort
 *  @param  a_flashadr      [in]  the start of address to be flashed
 *  @param  a_dvlpdatasize  [in]  the size to be flashed
 *  @return Void.
 *  @retval -
 *  @date   2008/09/26 FFCS)zhengmh Create.
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq.
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_nma_cm_EraseFlash(UINT face, UINT flshOffset, UINT size)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_nma_cm_EraseFlash] ENTER" );				

	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_INFO, "NMA flash erase: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				
	
	/* フェールセーフのためにフラッシュ読み取り */
	f_nma_cm_ReadFlash(face,flshOffset);
	
	/* boot領域のアクセスを抑止 */
	if( flshOffset < D_RRH_QSPI_FLASH_OFFSET_BOOT_END )
	{
		cmn_com_assert_th_get( f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "Flash offset invalid(face=%d, offset=%08x, size=%08x)",
								face, flshOffset, size );
		f_com_almReport( D_SYS_THDID_PF_NMA, D_DU_FLTID_FWNG );
		return D_RRH_NG;
	}
	
	rtn = BPF_HM_DEVC_QSPI_FLASH_ERASE(face,flshOffset,size);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR , "NMA flash erase NG:%d",face);
		return D_RRH_NG;
	}
	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_INFO  , "NMA flash erase OK:%d",face);
	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_nma_cm_EraseFlash] RETURN" );
	return D_RRH_OK;
}

/*!
 *  @brief  Common process of writing flashRom
 *  @note   This function is processed as follows. \n
 *          - Writing data from buffer to FlashRom.  \n
 *
 *  @param  face               [in] FlashRom face
 *  @param  flshOffset        [in] FlashRom Offset
 *  @param  size                [in] Size
 *  @param  srcBuf            [in] Source Buffer address
 *  @return 0: OK, 1: NG
 *  @date   2013/04/22 FFCS)Xut create
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
#define TEMP_BUFSIZE 0x1000  /* size=4K */

UINT f_nma_com_WriteFlash(UINT face, UINT flshOffset, UINT size, UCHAR *srcBuf)
{
	UINT	rtn = BPF_HM_DEVC_COMPLETE;
	INT 	num = D_SYS_NUM0;
	INT 	block = D_SYS_NUM0;
	INT		fileDsc = D_SYS_NUM0;
	INT		errcd = D_SYS_NUM0;
	UINT	actWrtSize = D_SYS_NUM0;
	CHAR	buffer[TEMP_BUFSIZE];

	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_nma_com_WriteFlash] ENTER" );		
	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "NMA flash write: face=%d, offset=%08x, size=%08x", face, flshOffset, size);				

	memset(buffer, 0, sizeof(buffer));
	
	/* boot領域のアクセスを抑止 */
	if( flshOffset < D_RRH_QSPI_FLASH_OFFSET_BOOT_END )
	{
		cmn_com_assert_th_get( f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "Flash offset invalid(face=%d, offset=%08x, size=%08x, num=%d )",
								face, flshOffset, size, num );
		f_com_almReport( D_SYS_THDID_PF_NMA, D_DU_FLTID_FWNG );
		return D_RRH_NG;
	}

	/* open the file */
	rtn = BPF_RU_FACC_FOPEN( (bpf_ru_facc_dpathname *)D_SYS_UNZIP_DIRNAME,
								(bpf_ru_facc_fname *)srcBuf,  
								 FACC_READ, 
								 BPF_RU_FACC_NO_CREATE, 
								 &fileDsc, 
								 &errcd );
	if( rtn != BPF_RU_FACC_COMPLETE )
	{
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"NMA file open NG:rtn:%d,err:%d" , rtn ,errcd );
		return D_RRH_NG;
	}
	else
	{
		/***********************************/
		/* read and write data with each 4K */
		/***********************************/
		block = size / TEMP_BUFSIZE;

		/* フェールセーフのためにフラッシュ読み取り */
		f_nma_cm_ReadFlash(face,flshOffset);

		for(num = 0;num < block;num++)
		{
			/* read the file */
			(VOID)BPF_RU_FACC_FREAD( fileDsc, TEMP_BUFSIZE, buffer, &actWrtSize, &errcd );
			
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(face, flshOffset, TEMP_BUFSIZE, (UCHAR*)buffer);
			
			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"NMA flash write NG:rtn:%d,num:%d,ofs:%x" , rtn , num ,flshOffset);
				(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
				return D_RRH_NG;
			}
			flshOffset += TEMP_BUFSIZE;
		}
		if(size % TEMP_BUFSIZE)
		{
			/* read the file */
			(VOID)BPF_RU_FACC_FREAD( fileDsc, size%TEMP_BUFSIZE, buffer, &actWrtSize, &errcd );
			
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(face, flshOffset, size%TEMP_BUFSIZE, (UCHAR*)buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"NMA flash write NG:rtn:%d size:0x%x" , rtn , (size % TEMP_BUFSIZE));
				(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );				
				return D_RRH_NG;
			}
		}
		/* close the file */
		(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
	}
	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_INFO  , "NMA flash write OK:%d",face);
	cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_nma_com_WriteFlash] RETURN" );
	return D_RRH_OK;
}

/*!
 *  @brief  Common process of writing event log
 *  @note   This function is processed as follows. \n
 *          - Writing event log from buffer to FlashRom.  \n
 *
 *  @param  face               [in] FlashRom face
 *  @param  flshOffset        [in] FlashRom Offset
 *  @param  size                [in] Size
 *  @param  srcBuf            [in] Source Buffer address
 *  @return 0: OK, 1: NG
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

UINT f_nma_cm_WriteEvtLog(UINT face, UINT flshOffset, UINT size)
{
	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_nma_cm_WriteEvtLog] ENTER" );				

	/* call com function to save event logs */
	f_com_logSaveFlash( D_RRH_OFF );

	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_nma_cm_WriteEvtLog] RETURN" );				

	return D_RRH_OK;
}

