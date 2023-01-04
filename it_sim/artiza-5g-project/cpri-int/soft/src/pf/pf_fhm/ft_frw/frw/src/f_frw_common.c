/*!
 * @skip  $ld:$
 * @file  f_frw_common.c
 * @brief Process of Frw common function
 * @date  2013/11/13 FFCS)linb create
 * @date  2015/08/14 TDIPS)sasaki update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"

/*!
 *  @brief  the function for flash erasing.
 *  @note   This function is processed as follows.
 *          1) start up the common function of flash erasing,if flash is failed,abort
 *  @param  face        [in]  Flash ROM face
 *  @param  flshOffset  [in]  Flash ROM offset
 *  @param  size        [in]  erase size
 *  @return Void.
 *  @date   2008/09/26 FFCS)zhengmh Create.
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq.
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_frw_cm_EraseFlash(UINT face, UINT flshOffset, UINT size)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frw_cm_EraseFlash] ENTER" );				

	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_INFO, "FRW flash erase: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				

	rtn = BPF_HM_DEVC_QSPI_FLASH_ERASE(face,flshOffset,size);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR , "FRW flash erase NG:%d",face);
		return D_RRH_NG;
	}
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_INFO  , "FRW flash erase OK:%d",face);
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frw_cm_EraseFlash] RETURN" );
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

UINT f_frw_com_WriteFlash(UINT face, UINT flshOffset, UINT size, UCHAR *srcBuf)
{
	UINT	rtn = BPF_HM_DEVC_COMPLETE;
	INT 	num = D_SYS_NUM0;
	INT 	block = D_SYS_NUM0;
	INT		fileDsc = D_SYS_NUM0;
	INT		errcd = D_SYS_NUM0;
	UINT	actWrtSize = D_SYS_NUM0;
	CHAR	buffer[TEMP_BUFSIZE];

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frw_com_WriteFlash] ENTER" );		
	
	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_INFO, "FRW flash write: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				

	memset(buffer, 0, sizeof(buffer));

	/* open the file */
	rtn = BPF_RU_FACC_FOPEN( (bpf_ru_facc_dpathname *)D_SYS_UNZIP_DIRNAME,
								(bpf_ru_facc_fname *)srcBuf,  
								 FACC_READ, 
								 BPF_RU_FACC_NO_CREATE, 
								 &fileDsc, 
								 &errcd );
	if( rtn != BPF_RU_FACC_COMPLETE )
	{
		cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRW file open NG:rtn:%d,err:%d" , rtn ,errcd );
		return D_RRH_NG;
	}
	else
	{
		/***********************************/
		/* read and write data with each 4K */
		/***********************************/
		block = size / TEMP_BUFSIZE;

		for(num = 0;num < block;num++)
		{
			/* read the file */
			(VOID)BPF_RU_FACC_FREAD( fileDsc, TEMP_BUFSIZE, buffer, &actWrtSize, &errcd );
				
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(face, flshOffset, TEMP_BUFSIZE, (UCHAR*)buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRW flash write NG:rtn:%d,num:%d,ofs:%x" , rtn , num ,flshOffset);
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
				cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRW flash write NG:rtn:%d size:0x%x" , rtn , (size % TEMP_BUFSIZE));
				(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );				
				return D_RRH_NG;
			}
		}
		/* close the file */
		(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
	}
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_INFO  , "FRW flash write OK:%d",face);
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frw_com_WriteFlash] RETURN" );
	return D_RRH_OK;
}

UINT f_frw_com_WriteFlash_slc1chp(UINT face, UINT flshOffset, UINT size, UCHAR *srcBuf)
{
	UINT	rtn = BPF_HM_DEVC_COMPLETE;
	INT 	num = D_SYS_NUM0;
	INT 	block = D_SYS_NUM0;
	UCHAR	*buffer = NULL;

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frw_com_WriteFlash_slc1chp] ENTER" );		
	
	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_INFO, "FRW flash write: face=%d, offset=%08x, size=%08x",
						face, flshOffset, size);				

	if( srcBuf == NULL )
	{
		cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"DATA POINTER NG");
		return D_RRH_NG;
	}
	else
	{
		/***********************************/
		/* read and write data with each 4K */
		/***********************************/
		block = size / TEMP_BUFSIZE;

		for(num = 0;num < block;num++)
		{
			buffer = srcBuf + num*TEMP_BUFSIZE;
							
			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(face, flshOffset, TEMP_BUFSIZE, buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRW flash write NG:rtn:%d,num:%d,ofs:%x" , rtn , num ,flshOffset);
				return D_RRH_NG;
			}
			flshOffset += TEMP_BUFSIZE;
		}
		if(size % TEMP_BUFSIZE)
		{
			buffer = srcBuf + block*TEMP_BUFSIZE;

			/* write the data */
			rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(face, flshOffset, size%TEMP_BUFSIZE, buffer);

			if( rtn != BPF_HM_DEVC_COMPLETE )
			{
				cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR,"FRW flash write NG:rtn:%d size:0x%x" , rtn , (size % TEMP_BUFSIZE));
				return D_RRH_NG;
			}
		}
	}
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_INFO  , "FRW flash write OK:%d",face);
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frw_com_WriteFlash_slc1chp] RETURN" );
	return D_RRH_OK;
}

/*!
 *  @brief  Common process of writing event log
 *  @note   This function is processed as follows. \n
 *          - Writing event log from buffer to FlashRom.  \n
 *
 *  @param  face               [in] FlashRom face
 *  @param  flshOffset         [in] FlashRom Offset
 *  @param  size               [in] Size
 *  
 *  @return 0: OK, 1: NG
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

UINT f_frw_cm_WriteEvtLog(UINT face, UINT flshOffset, UINT size)
{
	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frw_cm_WriteEvtLog] ENTER" );				

	/* call com function to save event logs */
	f_com_logSaveFlashFHM();

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_frw_cm_WriteEvtLog] RETURN" );				

	return D_RRH_OK;
}


/********************************************************************************************************************/
/**
 *  @brief    file download sending and receiving running history		
 *  @note	    file download sending and receiving running history	
 *												
 *  @param  	sndrcv_flg				[in]		sending and receiving flag
 *  @param     *datap                               [in]        data pointer
 * 
 * 
 *  @return   Result code is returned
 *  @retval   -
 *  @date     2014/01/16  FFCS)linb Create
 *  @date     2015/08/14  TDIPS)sasaki Add case D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ/RSP
 */
/********************************************************************************************************************/
VOID f_frw_cm_RunHisSet(UINT sndrcv_flg, UCHAR *datap) 
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
			
		case D_SYS_MSGID_FLASH_READ_REQ:	
			datas[0] = ((T_SYS_FLASH_READ_REQ *)datap)->face;
			datas[1] = ((T_SYS_FLASH_READ_REQ *)datap)->offset;
			datas[2] = ((T_SYS_FLASH_READ_REQ *)datap)->size;
			break;
			
		case D_SYS_MSGID_FLSLOGWR_IND:	
			datas[0] = ((T_SYS_FRSP_FLSLOGWR_IND *)datap)->eraseWriteKind;
			break;

		case D_SYS_MSGID_FLASH_ERASE_RSP:
			datas[0] = ((T_SYS_FLSERASE_RSP *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_RSP *)datap)->result;
			break;

		case D_SYS_MSGID_FLASH_READ_RSP:
			datas[0] = ((T_SYS_FLASH_READ_RSP*)datap)->size;
			break;

		case D_SYS_MSGID_FLASH_WRITE_RSP:
			datas[0] = ((T_SYS_LMCWRT_RSP*)datap)->result;
			break;
		case D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ:
			datas[0] = ((T_SYS_FWINFO_SAVE_REQ*)datap)->savepos;
			datas[1] = ((T_SYS_FWINFO_SAVE_REQ*)datap)->fwinfo.maker_id;
			break;
		case D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP:
			datas[0] = ((T_SYS_FWINFO_SAVE_RSP*)datap)->result;
			break;
		default:
			break;	
	}

	BPF_HM_DEVC_GETTIME( &datetime );


	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.setFlg     = D_SYS_ON;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.month      = datetime.month;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.day        = datetime.day;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.hour       = datetime.hour;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.min        = datetime.min;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.sec        = datetime.sec;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.msec       = datetime.msec;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.sndrcv_flg = (UCHAR)sndrcv_flg;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.event_id   = event_id;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.task_id    = (USHORT)task_id;
	f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].head.sig_kind   = (USHORT)sig_kind;
	memcpy((CHAR*)(f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx].body), (CHAR*)datas, sizeof(datas));

	f_frww_runhis_tbl.write_idx++;
	if (f_frww_runhis_tbl.write_idx >= D_FRW_RUNHIS_FACENUM_MAX)
	{
		f_frww_runhis_tbl.write_idx = D_SYS_NUM0;
	}

	memset((UCHAR*)&f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx], 0,
				sizeof(f_frww_runhis_tbl.inf[f_frww_runhis_tbl.write_idx]));

	return;
}

VOID f_frw_logPrint(FILE *outf)
{
	UINT index;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};

	fprintf(outf,"PF frw running history----------------------------------------\n");
	
	for(index=0; index<D_FRW_RUNHIS_FACENUM_MAX; index++)
	{
		if(f_frww_runhis_tbl.inf[index].head.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%2d-%2d %02d:%02d:%02d.%03d] %s;%08x;%04x;%04x\n",
			index,
			f_frww_runhis_tbl.inf[index].head.month,
			f_frww_runhis_tbl.inf[index].head.day,
			f_frww_runhis_tbl.inf[index].head.hour,
			f_frww_runhis_tbl.inf[index].head.min,
			f_frww_runhis_tbl.inf[index].head.sec,
			f_frww_runhis_tbl.inf[index].head.msec,
			strSndRcv[f_frww_runhis_tbl.inf[index].head.sndrcv_flg],
			f_frww_runhis_tbl.inf[index].head.event_id,
			f_frww_runhis_tbl.inf[index].head.task_id,
			f_frww_runhis_tbl.inf[index].head.sig_kind);
		
		for(bodyIdx = 0;bodyIdx < 10;bodyIdx++)
		{
			fprintf(outf," %02x%02x %02x%02x",
				f_frww_runhis_tbl.inf[index].body[bodyIdx * 4],
				f_frww_runhis_tbl.inf[index].body[bodyIdx * 4 + 1],
				f_frww_runhis_tbl.inf[index].body[bodyIdx * 4 + 2],
				f_frww_runhis_tbl.inf[index].body[bodyIdx * 4 + 3]);

			if((bodyIdx % 5) == 4) fprintf(outf,"\n");
		}
	}

 	return;
}
/* @} */
