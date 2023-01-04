/*!
 * @skip  $ld:$
 * @file  f_frwre_common.c
 * @brief Process of FrwRE common function
 * @date  2015/08/04 TDIPS)sasaki create
 * @date  2015/08/27 TDIPS)sasaki update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

#include "f_frwre_inc.h"


static unsigned char f_frwrew_readdata[16];

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
VOID f_frwre_cm_ReadFlash(UINT face, UINT flshOffset)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;
	INT		errcd;
	UCHAR	*invTbl;
	UCHAR	no_falesafe_flag;
	
	/* フェールセーフ抑止フラグ読み出し */
	/* 共有メモリアドレス取得 */
	rtn = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(BPF_RU_IPCM_OK != rtn)
	{
		cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR, "share memory address get NG(%d)",rtn);
		return;
	}
	
	/* フェールセーフ抑止フラグが1ならフェールセーフは実施しない */
	no_falesafe_flag = invTbl[D_RRH_EEP_FLASH_FALESAFE];
	if( no_falesafe_flag == 1)
	{
		cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO, "Flash fale safe is canseled.");
		return;
	}
	
	memset(f_frwrew_readdata,0x55,sizeof(f_frwrew_readdata));
	if(( rtn = BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ(face, flshOffset, 4, f_frwrew_readdata)) == BPF_HM_DEVC_COMPLETE ){
		cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO, "Before re-file flash%d read(0x%02x%02x%02x%02x)" ,
			face, f_frwrew_readdata[0], f_frwrew_readdata[1],  f_frwrew_readdata[2],  f_frwrew_readdata[3] );
	}
	else{
			cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR , "re-file flash%d read NG(%d)",face, rtn );
	}
	return;
}


/*!
 *  @brief  the function for flash erasing.
 *  @note   This function is processed as follows.
 *          1) start up the common function of flash erasing,if flash is faild,abort
 *  @param  face            [in]    Flash面
 *  @param  flshOffset      [in]    Flashオフセット
 *  @param  size            [in]    消去サイズ
 *  @return 0: OK, 1: NG
 *  @date   2015/08/04 TDIPS)sasaki Create
 *  @date   2015/08/14 TDIPS)sasaki BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE関数対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_frwre_cm_EraseFlash(UINT face, UINT flshOffset, UINT size)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frwre_cm_EraseFlash] ENTER");
	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO, "RE-FLASH ERASE: face=%d, offset=%08x, size=%08x", face, flshOffset, size);

	/* フェールセーフのためにフラッシュ読み取り */
	f_frwre_cm_ReadFlash(face,flshOffset);
	
	/* ERASE RE File	*/
	rtn = BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE(face, flshOffset, size);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR , "RE-FLASH ERASE NG:%d", face);
		return D_RRH_NG;
	}

	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO  , "RE-FLASH ERASE OK:%d", face);
	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwre_cm_EraseFlash] RETURN");

	return D_RRH_OK;
}

#define TEMP_BUFSIZE 0x1000  /* size=4K */

/*!
 *  @brief  Common process of writing flashRom
 *  @note   This function is processed as follows. \n
 *          - Writing data from buffer to FlashRom.  \n
 *
 *  @param  face				[in] FlashRom face
 *  @param  flshOffset			[in] FlashRom Offset
 *  @param  size				[in] Size
 *  @param  srcBuf				[in] Source Buffer address
 *  @return 0: OK, 1: NG
 *  @date   2015/08/04 TDIPS)sasaki Create
 *  @date   2015/08/14 TDIPS)sasaki BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE関数対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_frwre_com_WriteFlash(UINT face, UINT flshOffset, UINT size, UCHAR *srcBuf)
{
	UINT	rtn = BPF_HM_DEVC_COMPLETE;
	INT 	num = D_SYS_NUM0;
	INT 	block = D_SYS_NUM0;
	UCHAR	*buffer = srcBuf;

	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frwre_com_WriteFlash] ENTER");
	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO, "RE-FLASH WRITE: face=%d, offset=%08x, size=%08x", face, flshOffset, size);

	/***********************************/
	/* write data with each 4K */
	/***********************************/
	block = size / TEMP_BUFSIZE;
	
	/* フェールセーフのためにフラッシュ読み取り */
	f_frwre_cm_ReadFlash(face,flshOffset);

	for( num = 0; num < block; num++ )
	{
		rtn = BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE(face, flshOffset, TEMP_BUFSIZE, (UCHAR*)buffer);
		if( rtn != BPF_HM_DEVC_COMPLETE )
		{
			cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR, "RE-FLASH WRITE NG:rtn:%d,num:%d,ofs:%x", rtn, num, flshOffset);
			return D_RRH_NG;
		}
		flshOffset += TEMP_BUFSIZE;
		buffer += TEMP_BUFSIZE;
	}
	
	if(size % TEMP_BUFSIZE)
	{
		rtn = BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE(face, flshOffset, size % TEMP_BUFSIZE, (UCHAR*)buffer);
		if( rtn != BPF_HM_DEVC_COMPLETE )
		{
			cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR, "RE-FLASH WRITE NG:rtn:%d size:0x%x", rtn, (size % TEMP_BUFSIZE));
			return D_RRH_NG;
		}
	}

	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_INFO  , "RE-FLASH WRITE OK:%d", face);
	cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwre_com_WriteFlash] RETURN" );

	return D_RRH_OK;
}

/********************************************************************************************************************/
/**
 *  @brief    file download sending and receiving running history		
 *  @note	    file download sending and receiving running history	
 *												
 *  @param  	sndrcv_flg					[in]	sending and recieving flag
 *  
 *  
 *  @param		*datap						[in]	data pointer
 *  @return		Result code is returned
 *  @retval   -
 *  @date     2015/08/04  TDIPS)sasaki Create
 *  @date     2015/08/27  TDIPS)sasaki CT検出問題(メッセージパラメータの記録漏れ)の対処
 */
/********************************************************************************************************************/
VOID f_frwre_cm_RunHisSet(UINT sndrcv_flg, UCHAR *datap) 
{
	T_RRH_SYSTEM_TIME	datetime;
	UINT		sig_kind;		
	UINT		datas[10] = {0};
	UINT		event_id;
	UINT		task_id;

	sig_kind = D_SYS_ALLF_SHORT;
	event_id = ((T_RRH_HEAD*)datap)->uiEventNo;
	task_id = ((T_RRH_HEAD*)datap)->uiSrcPQueueID;

	switch(event_id)
	{
		case D_SYS_MSGID_FLASH_RE_ERASE_REQ:		
			datas[0] = ((T_SYS_FLSERASE_REQ *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_REQ *)datap)->clearFileNum;
			datas[2] = ((T_SYS_FLSERASE_REQ *)datap)->clearFlashRom[0].kind;
			datas[3] = ((T_SYS_FLSERASE_REQ *)datap)->clearFlashRom[0].face;
			datas[4] = ((T_SYS_FLSERASE_REQ *)datap)->clearFlashRom[0].offset;
			datas[5] = ((T_SYS_FLSERASE_REQ *)datap)->clearFlashRom[0].size;
			break;  

		case D_SYS_MSGID_FLASH_RE_WRITE_REQ:
			datas[0] = ((T_SYS_LMCWRT_REQ *)datap)->writeFileNum;
			datas[1] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].face;
			datas[2] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].offset;
			datas[3] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].size;
			break;  

		case D_SYS_MSGID_FLASH_RE_READ_REQ:
			datas[0] = ((T_SYS_FLASH_READ_REQ *)datap)->face;
			datas[1] = ((T_SYS_FLASH_READ_REQ *)datap)->offset;
			datas[2] = ((T_SYS_FLASH_READ_REQ *)datap)->size;
			break;

		case D_SYS_MSGID_FLASH_RE_ERASE_RSP:
			datas[0] = ((T_SYS_FLSERASE_RSP *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_RSP *)datap)->result;
			break;

		case D_SYS_MSGID_FLASH_RE_WRITE_RSP:
			datas[0] = ((T_SYS_LMCWRT_RSP*)datap)->result;
			break;

		case D_SYS_MSGID_FLASH_RE_READ_RSP:
			datas[0] = ((T_SYS_FLASH_READ_RSP*)datap)->result;
			datas[1] = ((T_SYS_FLASH_READ_RSP*)datap)->size;
			break;

		default:
			break;	
	}

	BPF_HM_DEVC_GETTIME( &datetime );


	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.setFlg     = D_SYS_ON;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.month      = datetime.month;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.day        = datetime.day;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.hour       = datetime.hour;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.min        = datetime.min;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.sec        = datetime.sec;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.msec       = datetime.msec;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.sndrcv_flg = (UCHAR)sndrcv_flg;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.event_id   = event_id;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.task_id    = (USHORT)task_id;
	f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].head.sig_kind   = (USHORT)sig_kind;
	memcpy((CHAR*)(f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx].body), (CHAR*)datas, sizeof(datas));

	f_frwrew_runhis_tbl.write_idx++;
	if (f_frwrew_runhis_tbl.write_idx >= D_FRWRE_RUNHIS_FACENUM_MAX)
	{
		f_frwrew_runhis_tbl.write_idx = D_SYS_NUM0;
	}

	memset((UCHAR*)&f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx], 0,
				sizeof(f_frwrew_runhis_tbl.inf[f_frwrew_runhis_tbl.write_idx]));

	return;
}

VOID f_frwre_logPrint(FILE *outf)
{
	UINT index;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};

	fprintf(outf,"PF frwre running history-------------------------------------\n");
	
	for(index=0; index<D_FRWRE_RUNHIS_FACENUM_MAX; index++)
	{
		if(f_frwrew_runhis_tbl.inf[index].head.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%2d-%2d %02d:%02d:%02d.%03d] %s;%08x;%04x;%04x\n",
			index,
			f_frwrew_runhis_tbl.inf[index].head.month,
			f_frwrew_runhis_tbl.inf[index].head.day,
			f_frwrew_runhis_tbl.inf[index].head.hour,
			f_frwrew_runhis_tbl.inf[index].head.min,
			f_frwrew_runhis_tbl.inf[index].head.sec,
			f_frwrew_runhis_tbl.inf[index].head.msec,
			strSndRcv[f_frwrew_runhis_tbl.inf[index].head.sndrcv_flg],
			f_frwrew_runhis_tbl.inf[index].head.event_id,
			f_frwrew_runhis_tbl.inf[index].head.task_id,
			f_frwrew_runhis_tbl.inf[index].head.sig_kind);
		
		for(bodyIdx = 0;bodyIdx < 10;bodyIdx++)
		{
			fprintf(outf," %02x%02x %02x%02x",
				f_frwrew_runhis_tbl.inf[index].body[bodyIdx * 4],
				f_frwrew_runhis_tbl.inf[index].body[bodyIdx * 4 + 1],
				f_frwrew_runhis_tbl.inf[index].body[bodyIdx * 4 + 2],
				f_frwrew_runhis_tbl.inf[index].body[bodyIdx * 4 + 3]);

			if((bodyIdx % 5) == 4) fprintf(outf,"\n");
		}
	}

 	return;
}
/* @} */
