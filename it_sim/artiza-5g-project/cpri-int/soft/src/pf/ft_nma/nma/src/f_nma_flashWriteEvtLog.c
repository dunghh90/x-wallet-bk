/*!
 * @skip  $ld:$
 * @file  f_nma_flashWriteEvtLog.c
 * @brief Reception of flash write event log request
 * @date  2011/11/13 FFCS)linb Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
#include "f_nma_inc.h"

/*!
 * @brief Reception of Flash write event log request
 * @note  This function is processed as follows.\n
 *       -# Write EventLog
 *       -# Write AlarmHistoryLog
 * @param  *msgPtr    [in]VOID   received message
 * @return INT.
 * @retval D_SYS_OK		0:normal
 * @retval D_SYS_NG		1:abnormal
 * @date 2013/11/13 FFCS)linb Create
 * @date  2018/08/27 KCN)若水  Create\n
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nma_flashWriteEvtLog(VOID* pRcvMsg)
{
	T_SYS_FRSP_FLSLOGWR_IND		*flsLogInd ;
	INT							ret  ;
	UINT						eraseWrtKind  ;	/* erase kinde */
	UINT						offset = D_SYS_NUM0 ;		/* log offset */
	UINT						size = D_SYS_NUM0 ;			/* log size */
	UCHAR						boot = D_SYS_NUM0 ;			/* using face */
	UINT						flashadr ;		/* flash address */
	UCHAR						temp_data[4];
	UINT						clearstate[2] = { D_RRH_EEP_CYCLIC_CLEAR_FLG0, D_RRH_EEP_CYCLIC_CLEAR_FLG1 };
	UCHAR						wData;
	UCHAR						rData = 0;
	UINT						flashoffset = D_RRH_QSPI_FLASH_OFFSET_INFO;
		
	/* Get the related values from the message */
	flsLogInd = (T_SYS_FRSP_FLSLOGWR_IND*)pRcvMsg;
	eraseWrtKind = flsLogInd->eraseWriteKind;

	/* Flash面情報取得 */
	ret = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_CYCLIC_LOG, &boot);

	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "WriteEvtLog Start kind:0x%08x boot:%d",eraseWrtKind,boot);
	/* 退避面がFlash 0面の場合 */
	if( boot == D_RRH_BOOT_ACT0 )
	{
		/* 消去面を1面に設定 */
		flashadr = D_RRH_QSPI_FLASH_FACE1 ;
	}
	/* 退避面がFlash 1面の場合 */
	else
	{
		/* 消去面を0面に設定 */
		flashadr = D_RRH_QSPI_FLASH_FACE0 ;
	}

	/* kind is erasing */
	if( eraseWrtKind == D_SYS_FLASH_ERASE )
	{
		/* 時間短縮のため必要な分のみeraseする	*/
		/* 冒頭4byteにサイズ情報が格納されているのでそのサイズを取得	*/
		ret = BPF_HM_DEVC_QSPI_FLASH_READ(	(UINT)flashadr,			
											flashoffset,
											sizeof(temp_data),
											temp_data);
		
		if(BPF_HM_DEVC_COMPLETE != ret)
		{
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "Flash Log Info load NG. ret=%08x", ret);
		}
		
		/************************************************************************************/
		/* サイズを取得する																	*/
		/************************************************************************************/
		size =	(temp_data[3] << 24) + 
				(temp_data[2] << 16) + 
				(temp_data[1] <<  8) + 
				(temp_data[0]);
		
		ret = BPF_HM_DEVC_EEPROM_READ( clearstate[ flashadr ], &rData);
		if( BPF_HM_DEVC_COMPLETE != ret )
		{
			cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "EEPROM Read NG(0x%x)", clearstate[ flashadr ]);
			return;
		}
		
		/* すでにログ領域がイレースされている場合は、イレース処理をスキップする */
		if( ( D_RRH_FLASH_ERASED == rData ) && ( size == 0xFFFFFFFF ) )
		{
			cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "WriteEvtLog Already Erased(%d)", flashadr);
			return;
		}
		
		if( D_RRH_FLASH_WRITE_FIN == rData )
		{
			/* サイズが3Mを超える場合はクリップする	*/
			if( ((size+4) >= D_RRH_QSPI_FLASH_CYCLICLOG_SIZE) || (size == 0) )
			{
				cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "FlashSize Invalid(%d,%d)", flashadr, size);
				size = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
			}
			else
			{
				/* length格納領域分もEraseする */
				size += 4;
			}
		}
		/* erasedでも All Fでない場合、または、WRITE_FINでもな場合はログサイズ分 Eraseする */
		else
		{
			size = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
		}
		
		offset = flashoffset;
		
		/* CyclicログClear状態をErase未に更新	*/
		wData = D_RRH_FLASH_NOT_ERASED;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ flashadr ], &wData);
		
		/* clear flash(from 0xfcc00000 => offset = 0x00c00000, size=必要サイズ or 1M) */
		ret = f_nma_cm_EraseFlash(flashadr, offset, size);		
		if( ret != D_SYS_OK )
		{
			/* Assert */
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_cm_EraseFlash NG ret=%08x", ret);
		}
		else
		{
			/* CyclicログClear状態をClear済みに更新	*/
			wData = D_RRH_FLASH_ERASED;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ flashadr ], &wData);
		}
	}
	/* kind is writing */
	else if( eraseWrtKind == D_SYS_FLASH_WRITE )
	{
		/* 先にCyclicログClear状態をNot erasedに更新	*/
		wData = D_RRH_FLASH_NOT_ERASED;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ flashadr ], &wData);
		
		/* compress and write */
		ret = f_nma_cm_WriteEvtLog(flashadr, offset, size);
		if( ret != D_SYS_OK )
		{
			/* Assert */
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_cm_WriteEvtLog NG ret=%08x", ret);
		}
		
	}
	else
	{
		/* Assert */
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "flash write kind invalid NG : 0x%x", eraseWrtKind);
	}
	cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "WriteEvtLog End.  write/erase log face = %d ",flashadr);

	return ;
}

/* @} */
