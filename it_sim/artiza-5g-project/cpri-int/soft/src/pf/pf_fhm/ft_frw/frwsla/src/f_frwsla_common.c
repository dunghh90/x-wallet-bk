/*!
 * @skip  $ld:$
 * @file  f_frwsla_common.c
 * @brief Process of Frw common function
 * @date  2014/12/12 FFCS)linb create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

 /*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */

#include "f_frwsla_inc.h"


static unsigned char f_frwslaw_readdata[16];

/*!
 *  @brief  the function for flash 4 byte read before writing and erasing.
 *  @note   This function is processed as follows.
 *          1)slave　flash read before writing and erasing.
 *  @param  flshOffset   [in] FlashROM offset
 *  @return VOID
 *  @date   2015/12/25 FJT)koshida Create
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_frwsla_cm_ReadFlash(UINT flshOffset)
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
		cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "share memory address get NG(%d)",rtn);
		return;
	}
	
	/* フェールセーフ抑止フラグが1ならフェールセーフは実施しない */
	no_falesafe_flag = invTbl[D_RRH_EEP_FLASH_FALESAFE];
	if( no_falesafe_flag == 1)
	{
		cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_INFO, "Flash fale safe is canseled.");
		return;
	}
	
	memset(f_frwslaw_readdata,0x55,sizeof(f_frwslaw_readdata));
	if(( rtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(flshOffset, 4, f_frwslaw_readdata)) == BPF_HM_DEVC_COMPLETE ){
		cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_INFO, "Before slave flash read(0x%02x%02x%02x%02x)" ,
			f_frwslaw_readdata[0], f_frwslaw_readdata[1],  f_frwslaw_readdata[2],  f_frwslaw_readdata[3] );
	}
	else{
			cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR , "slave flash read NG(%d)", rtn );
	}
	return;
}

/*!
 *  @brief  the function for flash erasing.
 *  @note   This function is processed as follows.
 *          1) start up the common function of flash erasing,if flash is faild,abort
 *  @param  face        [in] FlarhROM face
 *  @param  flshOffset  [in] FlashROM offset
 *  @param  size        [in] erase size
 *  @return Erase結果
 *  @date   2014/12/12 FFCS)linb Create for ppcEOL.
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_frwsla_cm_EraseFlash(UINT face, UINT flshOffset, UINT size)
{
	INT rtn = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frwsla_cm_EraseFlash] ENTER" );				

	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_INFO, "FRWSLA flash erase: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				

	f_frwsla_cm_ReadFlash(flshOffset);

	rtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(flshOffset,size);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR , "FRWSLA flash erase NG:%d",face);
		return D_RRH_NG;
	}
	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_INFO  , "FRWSLA flash erase OK:%d",face);
	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwsla_cm_EraseFlash] RETURN" );
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
 *  @date   2014/12/12 FFCS)linb Create for ppcEOL
 *  @date   2015/06/26 ALPHA)ueda RRH-007-000 TDD-SRE(Zynq)
 *  @date   2015/11/24 TDIPS)sasaki ST問処No.58対処
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
#define TEMP_BUFSIZE 0x1000  /* size=4K */

UINT f_frwsla_com_WriteFlash(UINT face, UINT flshOffset, UINT size, UCHAR *srcBuf)
{
	UINT	rtn = BPF_HM_DEVC_COMPLETE;
	INT 	num = D_SYS_NUM0;
	INT 	block = D_SYS_NUM0;
	INT		fileDsc = D_SYS_NUM0;
	INT		errcd = D_SYS_NUM0;
	UINT	actWrtSize = D_SYS_NUM0;
	CHAR	buffer[TEMP_BUFSIZE];
	UINT	tmpflshOffset;
	
	cmn_com_assert_th_get(f_frwslaw_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frwsla_com_WriteFlash] ENTER" );		
	
	cmn_com_assert_th_get(f_frwslaw_assert_p,  D_RRH_LOG_AST_LV_INFO, "FRWSLA flash write: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				

	memset(buffer, 0, sizeof(buffer));

	/* open the file */
	rtn = BPF_RU_FACC_FOPEN(
			(bpf_ru_facc_dpathname *)D_SYS_UNZIP_DIRNAME,
			(bpf_ru_facc_fname *)srcBuf,
			FACC_READ, 
			BPF_RU_FACC_NO_CREATE,
			&fileDsc,
			&errcd
	);
	
	if( rtn != BPF_RU_FACC_COMPLETE )
	{
		cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRWSLA file open NG:rtn:%d,err:%d" , rtn ,errcd );
		return D_RRH_NG;
	}
	else
	{

		tmpflshOffset = flshOffset;
		f_frwsla_cm_ReadFlash(tmpflshOffset);

		/***********************************/
		/* read and write data with each 4K */
		/***********************************/
		block = size / TEMP_BUFSIZE;

		for(num = 0;num < block;num++)
		{
			/* read the file */
			(VOID)BPF_RU_FACC_FREAD( fileDsc, TEMP_BUFSIZE, buffer, &actWrtSize, &errcd );
			
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(flshOffset, TEMP_BUFSIZE, (UCHAR*)buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRWSLA flash write NG:rtn:%d,num:%d,ofs:%x" , rtn , num ,flshOffset);
				(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
				return D_RRH_NG;
			}
			flshOffset += TEMP_BUFSIZE;
		}
		
		if(size % TEMP_BUFSIZE)
		{
			/* read the file */
			(VOID)BPF_RU_FACC_FREAD( fileDsc, size % TEMP_BUFSIZE, buffer, &actWrtSize, &errcd );
			
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(flshOffset, size % TEMP_BUFSIZE, (UCHAR*)buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRWSLA flash write NG:rtn:%d size:0x%x" , rtn , (size % TEMP_BUFSIZE));
				(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );				
				return D_RRH_NG;
			}
		}
		(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );				
	}
	
	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_INFO  , "FRWSLA flash write OK:%d",face);
	cmn_com_assert_th_get(f_frwslaw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frwsla_com_WriteFlash] RETURN" );

	return D_RRH_OK;
}

/********************************************************************************************************************/
/**
 *  @brief    file download sending and receiving running history		
 *  @note	    file download sending and receiving running history	
 *												
 *  @param  	sndrcv_flg				[in]		sending and recieving flag
 *  @param     *datap                               [in]        data pointer
 *  
 *  
 *  @return   Result code is returned
 *  @retval   -
 *  @date     2014/12/12  FFCS)linb Create for ppcEOL
 */
/********************************************************************************************************************/
VOID f_frwsla_cm_RunHisSet(UINT sndrcv_flg, UCHAR *datap) 
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
		case D_SYS_MSGID_FLASH_ERASE_REQ:		
			datas[0] = ((T_SYS_FLSERASE_REQ *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_REQ *)datap)->clearFileNum;
			break;

		case D_SYS_MSGID_FLASH_WRITE_REQ:
			datas[0] = ((T_SYS_LMCWRT_REQ *)datap)->writeFileNum;
			break;

		case D_SYS_MSGID_FLASH_ERASE_RSP:
			datas[0] = ((T_SYS_FLSERASE_RSP *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_RSP *)datap)->result;
			break;

		case D_SYS_MSGID_FLASH_WRITE_RSP:
			datas[0] = ((T_SYS_LMCWRT_RSP*)datap)->result;
			break;
			
		default:
			break;	
	}

	BPF_HM_DEVC_GETTIME( &datetime );


	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.setFlg     = D_SYS_ON;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.month      = datetime.month;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.day        = datetime.day;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.hour       = datetime.hour;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.min        = datetime.min;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.sec        = datetime.sec;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.msec       = datetime.msec;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.sndrcv_flg = (UCHAR)sndrcv_flg;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.event_id   = event_id;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.task_id    = (USHORT)task_id;
	f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].head.sig_kind   = (USHORT)sig_kind;
	memcpy((CHAR*)(f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx].body), (CHAR*)datas, sizeof(datas));

	f_frwslaw_runhis_tbl.write_idx++;
	if (f_frwslaw_runhis_tbl.write_idx >= D_FRWSLA_RUNHIS_FACENUM_MAX)
	{
		f_frwslaw_runhis_tbl.write_idx = D_SYS_NUM0;
	}

	memset((UCHAR*)&f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx], 0,
				sizeof(f_frwslaw_runhis_tbl.inf[f_frwslaw_runhis_tbl.write_idx]));

	return;
}

VOID f_frwsla_logPrint(FILE *outf)
{
	UINT index;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};

	fprintf(outf,"PF frwsla running history-------------------------------------\n");
	
	for(index=0; index<D_FRWSLA_RUNHIS_FACENUM_MAX; index++)
	{
		if(f_frwslaw_runhis_tbl.inf[index].head.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%2d-%2d %02d:%02d:%02d.%03d] %s;%08x;%04x;%04x\n",
			index,
			f_frwslaw_runhis_tbl.inf[index].head.month,
			f_frwslaw_runhis_tbl.inf[index].head.day,
			f_frwslaw_runhis_tbl.inf[index].head.hour,
			f_frwslaw_runhis_tbl.inf[index].head.min,
			f_frwslaw_runhis_tbl.inf[index].head.sec,
			f_frwslaw_runhis_tbl.inf[index].head.msec,
			strSndRcv[f_frwslaw_runhis_tbl.inf[index].head.sndrcv_flg],
			f_frwslaw_runhis_tbl.inf[index].head.event_id,
			f_frwslaw_runhis_tbl.inf[index].head.task_id,
			f_frwslaw_runhis_tbl.inf[index].head.sig_kind);
		
		for(bodyIdx = 0;bodyIdx < 10;bodyIdx++)
		{
			fprintf(outf," %02x%02x %02x%02x",
				f_frwslaw_runhis_tbl.inf[index].body[bodyIdx * 4],
				f_frwslaw_runhis_tbl.inf[index].body[bodyIdx * 4 + 1],
				f_frwslaw_runhis_tbl.inf[index].body[bodyIdx * 4 + 2],
				f_frwslaw_runhis_tbl.inf[index].body[bodyIdx * 4 + 3]);

			if((bodyIdx % 5) == 4) fprintf(outf,"\n");
		}
	}

 	return;
}
/* @} */
