/*!
 *  @skip   $Id$
 *  @file   f_fmng_common.c
 *  @brief  Common functions used in pf_fmng thread
 *  @date   2013/11/15 FFCS)linb Create\n
 *  @date   2015/08/28 TDIPS)sasaki Update
 *  @date   2015/08/31 TDIPS)sasaki Update(f_fmng_logPrint出力項目追加)
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-2015
*/ 

#include "f_fmng_inc.h"
#include "BPF_HM_DEVC.h"

/*!
 *  @brief  Send API Load Update Response
 *  @note   This function is processed as follows. \n
 *          - Send API Load Update Response to pf_Send thread.  \n
 *  @param  result       [in] result
 *  @param  errMsg       [in] error message
 *  @return none
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_com_sndLoadUpdRsp(UINT result,CHAR *errMsg)
{
	T_API_FILE_SAVE_FW_RSP			loadUpdRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_com_sndLoadUpdRsp] ENTER" );				
	
	/* Initialize API response message */
	memset((CHAR*)(&loadUpdRsp),0, sizeof(loadUpdRsp));

	/************************************************************/
	/* Edit and send Load Update Response to pf_Send thread		*/
	/************************************************************/
	loadUpdRsp.header.uiEventNo		= D_API_MSGID_FILE_FIRM_UPDATE_RSP;	/*!< Event  ID         */
	loadUpdRsp.header.uiSignalkind	= 0;								/*!< Signal Kind       */
	loadUpdRsp.header.uiDstPQueueID	= f_fmngw_respQid;				/*!< Destination P QID */
	loadUpdRsp.header.uiDstTQueueID	= 0;								/*!< Destination T QID */
	loadUpdRsp.header.uiSrcPQueueID	= D_RRH_PROCID_Q_PF;				/*!< Source P QID      */
	loadUpdRsp.header.uiSrcTQueueID	= 0;								/*!< Source T QID      */
	loadUpdRsp.header.uiLength		= sizeof(loadUpdRsp);				/*!< Length            */
	loadUpdRsp.data.dlwType			= f_fmngw_dlwType;
	/* Set the result of API Load Update Response */
	loadUpdRsp.data.dlwResult		= result;

	/* save running history */
	f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&loadUpdRsp);
	
	/* Edit and send API File Update Response to pf_send thread */
	f_com_sendMsgToAplFHM( f_fmngw_respQid, &loadUpdRsp, sizeof(loadUpdRsp),
						D_API_MSGID_FILE_FIRM_UPDATE_RSP,D_SYS_THDQID_PF_FRMG);					 

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_com_sndLoadUpdRsp] RETURN" );					
	return;
}

/*!
 *  @brief  Send API Load Erase Response
 *  @note   This function is processed as follows. \n
 *          - Send API Load Erase Response to pf_Send thread.  \n
 *  @param  result       [in] result
 *  @param  errMsg       [in] error message
 *  @return none
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
VOID f_fmng_com_sndLoadEraseRsp(UINT result,CHAR *errMsg)
{
	T_API_FILE_ERASE_FIRM_RSP			loadEraRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_com_sndLoadEraseRsp] ENTER" );					
	
	/* Initialize API response message */
	memset((CHAR*)(&loadEraRsp),0, sizeof(loadEraRsp));

	/************************************************************/
	/* Edit and send Load Update Response to pf_Send thread		*/
	/************************************************************/
	loadEraRsp.header.uiEventNo		= D_API_MSGID_FILE_FIRM_ERASE_RSP;	/*!< Event  ID         */
	loadEraRsp.header.uiSignalkind	= 0;								/*!< Signal Kind       */
	loadEraRsp.header.uiDstPQueueID	= f_fmngw_respQid;				/*!< Destination P QID */
	loadEraRsp.header.uiDstTQueueID	= 0;								/*!< Destination T QID */
	loadEraRsp.header.uiSrcPQueueID	= D_RRH_PROCID_Q_PF;				/*!< Source P QID      */
	loadEraRsp.header.uiSrcTQueueID	= 0;								/*!< Source T QID      */
	loadEraRsp.header.uiLength		= sizeof(loadEraRsp);				/*!< Length            */
	/* Set the result of API Load Erase Response */
	loadEraRsp.data.result			= result;
	strcpy(loadEraRsp.data.errMsg, errMsg);

	/* save running history */
	f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&loadEraRsp);
	
	/* Edit and send API File Update Response to pf_send thread */
	f_com_sendMsgToAplFHM( f_fmngw_respQid,&loadEraRsp, sizeof(loadEraRsp),
						D_API_MSGID_FILE_FIRM_ERASE_RSP,D_SYS_THDQID_PF_FRMG);					 

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_com_sndLoadEraseRsp] RETURN" );					
	return;
}

/*!
 *  @brief  Send Subscribe Get Version Response
 *  @note   This function is processed as follows. \n
 *          - Send Subscribe Get Version Response to pf_Send thread.  \n
 *
 *
 *  @return none
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
VOID f_fmng_com_sndSubGetVerRsp()
{
	T_API_FILE_FIRM_VERSION_RSP fileVerRsp;	

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_com_sndSubVerRsp] ENTER" );				
	
	memset(&fileVerRsp, 0, sizeof(fileVerRsp));
	
	/************************************************************/
	/* Edit and send Get Firm Version Response to l3_dlm thread		*/
	/************************************************************/
	fileVerRsp.header.uiEventNo		= D_API_MSGID_FILE_FIRM_VERSION_RSP;	/*!< Event  ID         */
	fileVerRsp.header.uiSignalkind	= 0;									/*!< Signal Kind       */
	fileVerRsp.header.uiDstPQueueID	= f_fmngw_verRespQid;					/*!< Destination P QID */
	fileVerRsp.header.uiDstTQueueID	= 0;									/*!< Destination T QID */
	fileVerRsp.header.uiSrcPQueueID	= D_RRH_PROCID_Q_PF;					/*!< Source P QID      */
	fileVerRsp.header.uiSrcTQueueID	= 0;									/*!< Source T QID      */
	fileVerRsp.header.uiLength		= sizeof(fileVerRsp);					/*!< Length            */
	fileVerRsp.data.usever			= f_fmngw_usever_tbl;
	fileVerRsp.data.stupver			= f_fmngw_stupver_tbl;
	fileVerRsp.data.year			= f_fmngw_useyear_tbl;
	fileVerRsp.data.mon_day			= f_fmngw_usemonthday_tbl;
	strcpy((CHAR*)fileVerRsp.data.stringVer, "SRE");

	/* save running history */
	f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&fileVerRsp);

	/* Edit and send API File Update Response to pf_send thread */
	f_com_sendMsgToAplFHM( f_fmngw_verRespQid, &fileVerRsp,sizeof(fileVerRsp) ,
						D_API_MSGID_FILE_FIRM_VERSION_RSP, D_SYS_THDQID_PF_FRMG);

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_com_sndSubVerRsp] RETURN" );				
	return ;
}

/*!
 *  @brief  According to devinfo to deceide whether need to download
 *  @note   This function is processed as follows. \n
 *  @param  devinfo  [in ] device information
 *  @param  flshAddr [in ] FlashROM address
 *  @param  fileType [out] file type
 *  @return none
 *  @date   2013/11/15 FFCS)linb create
 *  @date   2015/06/18 ALP)murakami TDD-ZYNQ対応
 *  @warning	N/A
 *  @FeatureID	PF_File-003-003-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
INT f_fmng_checkDLInfo(UINT devinfo,UINT flshAddr,UINT* fileType)
{
	UINT rrhkind = D_FMNG_RRH_INVALID;
	
	switch(f_comw_rrhKind)
	{
		case E_SYS_RRHKIND_SRE_35G:
			rrhkind = D_FMNG_RRH_SRE_35G_ZYNQ;
			break;
		default:
			rrhkind = D_FMNG_RRH_INVALID;
	}
/* FLASH EOL対応 add start */
	/* PS側、PL側、RE側いずれかがMacronix製のFlashの場合はダウングレードを抑止する */
	if((f_comw_psManufactureId == QSPI_MACRONIX_ID) ||(f_comw_plManufactureId == QSPI_MACRONIX_ID) ||
		(f_comw_reManufactureId[0] == QSPI_MACRONIX_ID) || (f_comw_reManufactureId[1] == QSPI_MACRONIX_ID) ||
		(f_comw_reManufactureId[2] == QSPI_MACRONIX_ID) || (f_comw_reManufactureId[3] == QSPI_MACRONIX_ID))
	{
		rrhkind |= D_FMNG_RRH_FLASH_EOL_FLAG;   /* +FLASH EOL */
	}
/* FLASH EOL対応 add end */
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "[f_fmng_checkDLInfo] rrhkind=%d, devinfo=%d", rrhkind, devinfo );					
	
	if((devinfo != D_SYS_NUM0) && ((devinfo & rrhkind) == rrhkind))
	{
		*fileType = D_SYS_NUM0;
		return D_SYS_OK;
	}
	else
	{
		*fileType = D_SYS_NUM1;
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "no need to download %d",devinfo);
		return D_SYS_NG;
	}
}

/*!
 *  @brief  Common process of start timer
 *  @note   This function is processed as follows. \n
 *          - Common process of start timer.  \n
 *  @param  timerId       [in] timer ID
 *  @param  timeVal       [in] timer value
 *  @param  eventId       [in] event id
 *  @return none
 *  @date   2013/04/22 FFCS)Xut create
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_com_startTimer(UINT timerId,INT timeVal,UINT eventId)
{
	INT ret;
	INT errcd;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_com_startTimer] ENTER" );					
	
	ret = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_ONCE,
								timerId,					/* TimerID						*/
								timeVal,					/* TOカウント					*/
								eventId, 					/* TO時のEventID				*/
								D_SYS_THDQID_PF_FRMG, 		/* TO時の送信先ThreadQueueID	*/
								&errcd );
	if(ret != BPF_RU_HRTM_COMPLETE)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "timer start NG %d,%d,0x%x,0x%x,0x%x",
				ret,errcd,timerId, timeVal, eventId);
		return;
	}
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_com_startTimer] RETURN" );					
	return;
}

/*!
 *  @brief  Common process of stop timer
 *  @note   This function is processed as follows. \n
 *          - Common process of stop timer.  \n
 *
 *  @param  timerId       [in] timer ID
 *
 *  @return none
 *  @date   2013/04/22 FFCS)Xut create
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_com_stopTimer(UINT timerId)
{
	INT ret;
	INT errcd;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_com_stopTimer] ENTER" );					

	ret = BPF_RU_HRTM_CANCEL(timerId, &errcd);
	if(ret != BPF_RU_HRTM_COMPLETE)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "timer stop NG %d,%d,0x%x,0x%x,0x%x",
				ret,errcd,timerId);
	}
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_com_stopTimer] RETURN" );					
	return;
}

/*!
 *  @brief  Common process of getting erasing flag by address
 *  @note   This function is processed as follows. \n
 *          - Common process of getting erasing flag by address  \n
 *
 *  @param  addr       [in] writing address
 *  @return fix value (0)
 *  @date   2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_fmng_getEraseFlgByAddr(UINT addr)
{
	/* T.B.D */
	return D_SYS_NUM0;
}


/********************************************************************************************************************/
/**
 *  @brief    file download sending and receiving running history		
 *  @note	    file download sending and receiving running history	
 *												
 *  @param  	sndrcv_flg				[in]		sending and recieving flag
 *  
 *  
 *  @param     *datap                               [in]        data pointer
 *  @return   Result code is returned
 *  @retval   -
 *  @date     2014/01/16  FFCS)linb Create
 *  @date     2015/08/28  TDIPS)sasaki CT検出問題(RE-FLASH READ応答の受信ログが記録されない)の対処
 */
/********************************************************************************************************************/
VOID f_fmng_cm_RunHisSet(UINT sndrcv_flg, UCHAR *datap) 
{
	T_RRH_SYSTEM_TIME	datetime;
	UINT		datas[10] = {0};
	UINT		event_id;
	UINT		task_id;	

	event_id = ((T_RRH_HEAD*)datap)->uiEventNo;
	task_id = ((T_RRH_HEAD*)datap)->uiSrcPQueueID;
	
	switch(event_id)
	{
		case D_API_MSGID_FILE_FIRM_ERASE_REQ:		
			datas[0] = ((T_API_FILE_ERASE_FIRM_REQ *)datap)->data.side;
			break;

		case D_API_MSGID_FILE_FIRM_ERASE_RSP:
			datas[0] = ((T_API_FILE_ERASE_FIRM_RSP *)datap)->data.result;
			break;

		case D_SYS_MSGID_FLASH_ERASE_REQ:
			datas[0] = ((T_SYS_FLSERASE_REQ *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_REQ *)datap)->clearFileNum;
			break;

		case D_SYS_MSGID_FLASH_ERASE_RSP:
			datas[0] = ((T_SYS_FLSERASE_RSP *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_RSP *)datap)->result;
			break;

		case D_SYS_MSGID_FLASH_RE_ERASE_RSP:
			datas[0] = ((T_SYS_FLSERASE_RSP *)datap)->cause;
			datas[1] = ((T_SYS_FLSERASE_RSP *)datap)->result;
			break;
			
		case D_API_MSGID_FILE_FIRM_UPDATE_REQ:	
			datas[0] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.dlwType;
			datas[1] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.chksum;
			datas[2] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.time;
			datas[3] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.fw_type;
			datas[4] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.save_pos;
			datas[5] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.fw_info.maker_id;
			datas[6] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.fw_info.major_ver;
			datas[7] = ((T_API_FILE_SAVE_FW_REQ *)datap)->data.fw_info.minor_ver;
			break;
			
		case D_API_MSGID_FILE_FIRM_UPDATE_RSP:
			datas[0] = ((T_API_FILE_SAVE_FW_RSP *)datap)->data.dlwType;	
			datas[1] = ((T_API_FILE_SAVE_FW_RSP *)datap)->data.dlwResult;			
			break;
			
		case D_SYS_MSGID_FLASH_WRITE_REQ:	
			datas[0] = ((T_SYS_LMCWRT_REQ *)datap)->writeFileNum;
			break;
			
		case D_SYS_MSGID_FLASH_WRITE_RSP:	
			datas[0] = ((T_SYS_LMCWRT_RSP *)datap)->result;
			break;
			
		case D_SYS_MSGID_FLASH_RE_WRITE_REQ:	
			datas[0] = ((T_SYS_LMCWRT_REQ *)datap)->writeFileNum;
			datas[1] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].face;
			datas[2] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].offset;
			datas[3] = ((T_SYS_LMCWRT_REQ *)datap)->writeFlashRom[0].size;
			break;
			
		case D_SYS_MSGID_FLASH_RE_WRITE_RSP:	
			datas[0] = ((T_SYS_LMCWRT_RSP *)datap)->result;
			break;
			
		case D_API_MSGID_FILE_FIRM_VERSION_REQ:
			break;

		case D_API_MSGID_FILE_FIRM_VERSION_RSP:
			datas[0] = ((T_API_FILE_FIRM_VERSION_RSP *)datap)->data.usever;
			datas[1] = ((T_API_FILE_FIRM_VERSION_RSP *)datap)->data.stupver;
			datas[2] = ((T_API_FILE_FIRM_VERSION_RSP *)datap)->data.year;
			datas[3] = ((T_API_FILE_FIRM_VERSION_RSP *)datap)->data.mon_day;		
			break;
			
		case D_API_MSGID_FILE_FIRM_CANCEL_IND:
			break;

		case D_API_MSGID_FILE_FIRM_CANCEL_RSP:
			datas[0] = ((T_API_FILE_FIRM_CANCEL_RSP *)datap)->data.result;
			break;

		case D_API_MSGID_FILE_FIRM_READ_REQ:
			datas[0] = ((T_API_FILE_LOAD_FW_DATA_REQ *)datap)->data.save_pos;
			break;
		
		case D_API_MSGID_FILE_FIRM_READ_RSP:
			datas[0] = ((T_API_FILE_LOAD_FW_DATA_RSP *)datap)->data.result;
			break;

		case D_SYS_MSGID_FLASH_RE_READ_REQ:	
			datas[0] = ((T_SYS_FLASH_READ_REQ *)datap)->face;
			datas[1] = ((T_SYS_FLASH_READ_REQ *)datap)->offset;
			datas[2] = ((T_SYS_FLASH_READ_REQ *)datap)->size;
			break;
			
		case D_SYS_MSGID_FLASH_RE_READ_RSP:	
			datas[0] = ((T_SYS_FLASH_READ_RSP *)datap)->result;
			datas[1] = ((T_SYS_FLASH_READ_RSP *)datap)->size;
			break;
			
		case D_SYS_MSGID_FLASH_TIMEOUT_NTC:
		case D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC:
			break;

		case D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ:
			datas[0] = ((T_SYS_FWINFO_SAVE_REQ *)datap)->savepos;
			datas[1] = ((T_SYS_FWINFO_SAVE_REQ *)datap)->fwinfo.maker_id;
			datas[2] = ((T_SYS_FWINFO_SAVE_REQ *)datap)->fwinfo.major_ver;
			datas[3] = ((T_SYS_FWINFO_SAVE_REQ *)datap)->fwinfo.minor_ver;
			break;

		case D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP:
			datas[0] = ((T_SYS_FWINFO_SAVE_RSP *)datap)->result;
			break;

		default:
			break;	
	}

	BPF_HM_DEVC_GETTIME( &datetime );

	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.setFlg	= D_SYS_ON;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.month      = datetime.month;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.day        = datetime.day;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.hour       = datetime.hour;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.min        = datetime.min;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.sec        = datetime.sec;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.msec       = datetime.msec;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.sndrcv_flg = (UCHAR)sndrcv_flg;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.event_id   = event_id;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.task_id    = (USHORT)task_id;
	f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].head.sig_kind   = (USHORT)f_fmngw_thrdState;
	memcpy((CHAR*)(f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx].body), (CHAR*)datas, sizeof(datas));
	
	f_fmngw_runhis_tbl.write_idx++;
	if (f_fmngw_runhis_tbl.write_idx >= D_FMNG_RUNHIS_FACENUM_MAX)
	{
		f_fmngw_runhis_tbl.write_idx = D_SYS_NUM0;
	}

	memset((UCHAR*)&f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx], 0,
				sizeof(f_fmngw_runhis_tbl.inf[f_fmngw_runhis_tbl.write_idx]));

	return;
}

VOID f_fmng_logPrint(FILE *outf)
{
	UINT index;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};

	fprintf(outf,"PF fmg running history----------------------------------------\n");
	
	for(index=0; index<D_FMNG_RUNHIS_FACENUM_MAX; index++)
	{
		if(f_fmngw_runhis_tbl.inf[index].head.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%2d-%2d %02d:%02d:%02d.%03d] %s;%08x;%04x;%04x\n",
			index,
			f_fmngw_runhis_tbl.inf[index].head.month,
			f_fmngw_runhis_tbl.inf[index].head.day,
			f_fmngw_runhis_tbl.inf[index].head.hour,
			f_fmngw_runhis_tbl.inf[index].head.min,
			f_fmngw_runhis_tbl.inf[index].head.sec,
			f_fmngw_runhis_tbl.inf[index].head.msec,
			strSndRcv[f_fmngw_runhis_tbl.inf[index].head.sndrcv_flg],
			f_fmngw_runhis_tbl.inf[index].head.event_id,
			f_fmngw_runhis_tbl.inf[index].head.task_id,
			f_fmngw_runhis_tbl.inf[index].head.sig_kind);
		
		for(bodyIdx = 0;bodyIdx < 10;bodyIdx++)
		{
			fprintf(outf," %02x%02x %02x%02x",
				(UCHAR)f_fmngw_runhis_tbl.inf[index].body[bodyIdx * 4],
				(UCHAR)f_fmngw_runhis_tbl.inf[index].body[bodyIdx * 4 + 1],
				(UCHAR)f_fmngw_runhis_tbl.inf[index].body[bodyIdx * 4 + 2],
				(UCHAR)f_fmngw_runhis_tbl.inf[index].body[bodyIdx * 4 + 3]);

			if((bodyIdx % 5)== 4) fprintf(outf,"\n");
		}
	}

	fprintf(outf,"\n ============= RAM TABLE =============\n");
	fprintf(outf," usebootface:%d\n",f_fmngw_usebootface);
	fprintf(outf," usebootface_cprifpga:%d\n",f_fmngw_usebootface_cprifpga);
	fprintf(outf," verRespQid:0x%08x\n",f_fmngw_verRespQid);
	fprintf(outf," stupver:%d\n",f_fmngw_stupver_tbl);
	fprintf(outf," usever:%d\n",f_fmngw_usever_tbl);

	fprintf(outf," thrdState:%d\n",f_fmngw_thrdState);
	fprintf(outf," preThrdState:%d\n",f_fmngw_preThrdState);
	fprintf(outf," apikind:%d\n",f_fmngw_apikind);
	fprintf(outf," dlwType:%d\n",f_fmngw_dlwType);
    fprintf(outf," fwType:%d\n",f_fmngw_fw_type);
    fprintf(outf," respQid:0x%08x\n",f_fmngw_respQid);
	fprintf(outf," re_thrdState:%d\n",f_fmngw_re_thrdState);
    fprintf(outf," re_apikind:%d\n",f_fmngw_re_apikind);
    fprintf(outf," re_rspkind:%d\n",f_fmngw_rsp_kind);
    fprintf(outf," re_fwUpdResult:%d\n",f_fmngw_re_fileUpdResult);
    fprintf(outf," re_fwReadCancelFlag:%d\n",f_fmngw_read_cancel_re);

	fprintf(outf,"\n ========= flashWaitWrtReq_re =========\n");
    fprintf(outf," flashWaitWrtReq_re.writeFileNum:%d\n",f_fmngw_flashWaitWrtReq_re.writeFileNum);
	for(index = 0; index < f_fmngw_flashWaitWrtReq_re.writeFileNum; index++)
	{
	    fprintf(outf," flashWaitWrtReq_re.writeFlashRom[%d]\n",index);
	    fprintf(outf," \tface:%d\n",f_fmngw_flashWaitWrtReq_re.writeFlashRom[index].face);
	    fprintf(outf," \toffset:0x%08x\n",f_fmngw_flashWaitWrtReq_re.writeFlashRom[index].offset);
	    fprintf(outf," \tfileName:%s\n",f_fmngw_flashWaitWrtReq_re.writeFlashRom[index].fileName);
	    fprintf(outf," \tsize:%d\n",f_fmngw_flashWaitWrtReq_re.writeFlashRom[index].size);
	    fprintf(outf," \tdata:%p\n\n",f_fmngw_flashWaitWrtReq_re.writeFlashRom[index].data);
	}

	fprintf(outf,"\n ========= flashWaitEraReq_re =========\n");
    fprintf(outf," flashWaitEraReq_re.clearFileNum:%d\n",f_fmngw_flashWaitEraReq_re.clearFileNum);
	for(index = 0; index < f_fmngw_flashWaitEraReq_re.clearFileNum; index++)
	{
	    fprintf(outf," flashWaitEraReq_re.clearFlashRom[%d]\n",index);
	    fprintf(outf," \tkind:%d\n",f_fmngw_flashWaitEraReq_re.clearFlashRom[index].kind);
	    fprintf(outf," \tface:%d\n",f_fmngw_flashWaitEraReq_re.clearFlashRom[index].face);
	    fprintf(outf," \toffset:0x%08x\n",f_fmngw_flashWaitEraReq_re.clearFlashRom[index].offset);
	    fprintf(outf," \tsize:%d\n",f_fmngw_flashWaitEraReq_re.clearFlashRom[index].size);
	}

	fprintf(outf,"\n ========== save_fw_reqdata ==========\n");
    fprintf(outf," save_fw_reqdata.dlwType:%d\n",f_fmngw_save_fw_reqdata.dlwType);
    fprintf(outf," save_fw_reqdata.chksum:%08x\n",f_fmngw_save_fw_reqdata.chksum);
    fprintf(outf," save_fw_reqdata.time:%d\n",f_fmngw_save_fw_reqdata.time);
    fprintf(outf," save_fw_reqdata.fw_type:%d\n",f_fmngw_save_fw_reqdata.fw_type);
    fprintf(outf," save_fw_reqdata.save_pos:%d\n",f_fmngw_save_fw_reqdata.save_pos);
    fprintf(outf," save_fw_reqdata.fw_info.maker_id:%d\n",f_fmngw_save_fw_reqdata.fw_info.maker_id);
    fprintf(outf," save_fw_reqdata.fw_info.major_ver:%d\n",f_fmngw_save_fw_reqdata.fw_info.major_ver);
    fprintf(outf," save_fw_reqdata.fw_info.minor_ver:%d\n",f_fmngw_save_fw_reqdata.fw_info.minor_ver);
    fprintf(outf," save_fw_reqdata.fw_info.file_size:%ld\n",f_fmngw_save_fw_reqdata.fw_info.file_size);
    fprintf(outf," save_fw_reqdata.fw_info.file_name:%s\n",f_fmngw_save_fw_reqdata.fw_info.file_name);
    fprintf(outf," save_fw_reqdata.fw_info.year:%d\n",f_fmngw_save_fw_reqdata.fw_info.year);
    fprintf(outf," save_fw_reqdata.fw_info.month:%d\n",f_fmngw_save_fw_reqdata.fw_info.month);
    fprintf(outf," save_fw_reqdata.fw_info.day:%d\n",f_fmngw_save_fw_reqdata.fw_info.day);

 	return;
}
/* @} */
