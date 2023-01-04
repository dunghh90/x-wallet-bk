/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_l3log.c
 *  @brief  RCT L3 Signal LOG
 *  @date   2015/05/04 FFCS)wuqy create for TDD
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_cm_l3log.h"

typedef void (*VOIDFUNCPTR)();

/****************************************************************************/
/* L3 Log Table for RCT */
/****************************************************************************/
T_RRH_L3_SR_LOG*						cmw_l3log_tbl_p;

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    L3 log save
 *  @note     L3 log save
 *  @param    log_tbl       [in] log table
 *  @param    mngitem_idx   [in] log manage table index now only D_RRH_L3LOG_MNGIDX_CARDSTS and D_RRH_L3LOG_MNGIDX_CARSTS
 *  @param    cur_log       [in] current log
 *  @param    len           [in] current log len
 *  @param    dir           [in] directory
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_L3LogSave(T_RRH_L3_SR_LOG *log_tbl, int mngitem_idx, void* cur_log, USHORT len, USHORT dir)
{
	T_RRH_L3LOG_MNGITEM *mng_item     =  &(log_tbl->mng.item[mngitem_idx]);
	T_RRH_L3LOG_BLOCKITEM *next_bufitem = (T_RRH_L3LOG_BLOCKITEM *)( ((UCHAR*)log_tbl) + mng_item->start_offset+ mng_item->next_block_no * mng_item->block_size);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "mngitem_idx:%x,len:%x,dir:%x,next_block_no:%x",
						mngitem_idx, len, dir, mng_item->next_block_no);

	if(len > mng_item->max_log_len)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,
			"len:0x%x, block_size:0x%x,max_log_len:0x%x",
			len, mng_item->block_size,mng_item->max_log_len);
		return;
	}

	cm_MemClr(next_bufitem, mng_item->block_size);
	cm_RtcGet( &(next_bufitem->log_time) );
	next_bufitem->log_len = len;
	next_bufitem->log_dir = dir;
	cm_MemCpy(&(next_bufitem->log_data), cur_log, len);
	mng_item->next_block_no =  (mng_item->next_block_no + CMD_NUM1) % (mng_item->block_num);
	
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    card status report response log to tbl
 *  @note     card status report response log to tbl
 *  @param    log_data	[in] message
 *  @param    len	[in] len
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_CardStsResLogSave( CMT_CPRIF_CARDSTSRES_S3G *log_data, USHORT len)
{
	T_RRH_L3_SR_LOG *log_tbl = cmw_l3log_tbl_p;
	UINT mngitem_idx = D_RRH_L3LOG_MNGIDX_CARDSTS;
	T_RRH_L3LOG_MNGITEM *mng_item     =  &(log_tbl->mng.item[mngitem_idx]);
	UINT                 old_block_no  = (mng_item->next_block_no + mng_item->block_num - CMD_NUM1) % (mng_item->block_num);
	T_RRH_L3LOG_BLOCKITEM *old_bufitem  = (T_RRH_L3LOG_BLOCKITEM *)( ((UCHAR*)log_tbl) + mng_item->start_offset + old_block_no * mng_item->block_size);
	CMT_CPRIF_CARDSTSRES_S3G *stsRes_old_p = (CMT_CPRIF_CARDSTSRES_S3G *)(&(old_bufitem->log_data));
	RCT_CARDSTS_MNG_T stsRes_temp;
	
	cm_MemClr(&stsRes_temp, sizeof(stsRes_temp));
	cm_MemCpy(&stsRes_temp, log_data, len);
	((CMT_CPRIF_CARDSTSRES_S3G *)&stsRes_temp)->cardsts_inf.year = stsRes_old_p->cardsts_inf.year;
	((CMT_CPRIF_CARDSTSRES_S3G *)&stsRes_temp)->cardsts_inf.month_day =stsRes_old_p->cardsts_inf.month_day;
	((CMT_CPRIF_CARDSTSRES_S3G *)&stsRes_temp)->cardsts_inf.hour =stsRes_old_p->cardsts_inf.hour;
	((CMT_CPRIF_CARDSTSRES_S3G *)&stsRes_temp)->cardsts_inf.minsec = stsRes_old_p->cardsts_inf.minsec;
	
	if( ( len != old_bufitem->log_len ) ||
		( cm_MemCmp( &stsRes_temp, stsRes_old_p, len ) != 0 ) )
	{
		m_cm_L3LogSave(log_tbl, mngitem_idx, log_data, len, D_RRH_L3LOG_DIR_SND);
	}

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    carrier status report response log save
 *  @note     carrier status report response log save
 *  @param    res	[in] carrier status report response
 *  @param    log_len	[in] carrier status report len
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_CarStsResLogSave(CMT_CPRIF_CARLSTSRES *res, USHORT log_len)
{
	T_RRH_L3_SR_LOG *log_tbl = cmw_l3log_tbl_p;
	UINT mngitem_idx = D_RRH_L3LOG_MNGIDX_CARSTS;
	T_RRH_L3LOG_MNGITEM *mng_item     =  &(log_tbl->mng.item[mngitem_idx]);
	UINT                 old_block_no  = (mng_item->next_block_no + mng_item->block_num - CMD_NUM1) % (mng_item->block_num);
	T_RRH_L3LOG_BLOCKITEM *old_bufitem  = (T_RRH_L3LOG_BLOCKITEM *)( ((UCHAR*)log_tbl) + mng_item->start_offset + old_block_no * mng_item->block_size);

	if( ( log_len != old_bufitem->log_len ) ||
		( cm_MemCmp( res, &(old_bufitem->log_data), log_len ) != 0 ) )
	{
		m_cm_L3LogSave(log_tbl,
				mngitem_idx,
				res,
				log_len,
				D_RRH_L3LOG_DIR_SND);
	}
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    FB ID to name
 *  @note     FB ID to name
 *  @param    fb_id     [in]    FB ID
 *  @param    fb_name   [out]   FB name取得先
 *  @param    len       [in]    変換最大サイズ(snprintf())
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_CardStsResFbIdToName(USHORT fb_id, CHAR *fb_name, int len)
{
	char fb_name_array[CMD_NUM21][CMD_NUM11] = 
	{
		"BPF/SWITCH", 	/* fb id = 0 */
		"LNA", 			/* fb id = 1 */
		"T-PA", 		/* fb id = 2 */
		"TRX", 			/* fb id = 3 */
		"TRX-INF", 		/* fb id = 4 */
		"5", 			/* fb id = 5 */
		"6", 			/* fb id = 6 */
		"7", 			/* fb id = 7 */
		"8", 			/* fb id = 8 */
		"9", 			/* fb id = 9 */
		"A", 			/* fb id = 10 */
		"B", 			/* fb id = 11 */
		"C", 			/* fb id = 12 */
		"D", 			/* fb id = 13 */
		"E", 			/* fb id = 14 */
		"F", 			/* fb id = 15 */
		"10", 			/* fb id = 16 */
		"11", 			/* fb id = 17 */
		"12", 			/* fb id = 18 */
		"PORT-INF", 	/* fb id = 19 */
		"MT-INF" 		/* fb id = 20 */
	};
	int fb_id_max = CMD_NUM21;
	
	if( fb_id < fb_id_max )
	{
		snprintf(fb_name, len, "%s", fb_name_array[fb_id]);
	}
	else 
	{
		snprintf(fb_name, len, "%x", fb_id);
	}
}

/* @} */
VOID m_cm_CardStsResAlmstsIdToName(USHORT almsts_id, CHAR *almsts_name, int len)
{
	char almsts_name_array[CMD_NUM4][CMD_NUM11] = 
	{
		"Unmount", 	/* fb id = 0 */
		"Normal", 			/* fb id = 1 */
		"High-ALM", 		/* fb id = 2 */
		"SYS-ALM" 		/* fb id = 20 */
	};
	int almsts_id_max = CMD_NUM4;
	
	if( almsts_id < almsts_id_max )
	{
		snprintf(almsts_name, len, "%s", almsts_name_array[almsts_id]);
	}
	else 
	{
		snprintf(almsts_name, len, "%x", almsts_id);
	}
}


VOID m_cm_CardStsResPrintOneReform(CMT_CPRIF_CARDSTSRES* log_data,FILE *outf,UINT disp_no,USHORT msec)
{
	UINT fb_count;
	UINT car_count;
	UINT nocar_count;
	UINT br_count;
	UINT nobr_count;
	UINT alm_count;
	UINT err_count;
	CHAR fb_name[CMD_NUM12];
	CHAR almsts_name[CMD_NUM12];
	CHAR code_set[CMD_NUM256];
	USHORT curmsec = 0;
	CHAR everycode[CMD_NUM5];
	CHAR carbarbuf[CMD_NUM128];
	CHAR everycarbarbuf[12];
	RCT_CARDSTS_FB_EVERY_COM_T *fb_every_com;
	RCT_CARDSTS_FB_CAR_EVERY_COM_T *car_every_com;
	RCT_CARDSTS_FB_CAR_BR_EVERY_T *br_every;
	RCT_CARDSTS_ALM_COM_T *alm;
	RCT_CARDSTS_ALM_EVERY_T *alm_every;
	RCT_CARDSTS_ERR_COM_T *err;
	RCT_CARDSTS_ERR_EVERY_T *err_every;
	RCT_CPRIF_CARDSTSRES_T *res;
	UCHAR *next_p = (UCHAR *)log_data;
	
	RCT_CARDSTS_MNG_T sign_0x5721_mng_t;
	memset(&sign_0x5721_mng_t,'\0',sizeof(RCT_CARDSTS_MNG_T));
	memset(almsts_name,'\0',sizeof(almsts_name));
	memset(code_set,'\0',sizeof(code_set));
	
	
	res = (RCT_CPRIF_CARDSTSRES_T *)next_p;
	next_p += sizeof(*res);
	
	sign_0x5721_mng_t.res.signal_kind = res->signal_kind;
	sign_0x5721_mng_t.res.result = res->result;
	sign_0x5721_mng_t.res.cardsts_inf.slot_no = res->cardsts_inf.slot_no;
	memcpy(sign_0x5721_mng_t.res.cardsts_inf.card_name,res->cardsts_inf.card_name,strlen((CHAR *)res->cardsts_inf.card_name));
	sign_0x5721_mng_t.res.cardsts_inf.year = res->cardsts_inf.year;
	sign_0x5721_mng_t.res.cardsts_inf.month_day = res->cardsts_inf.month_day;
	sign_0x5721_mng_t.res.cardsts_inf.hour = res->cardsts_inf.hour;
	sign_0x5721_mng_t.res.cardsts_inf.minsec = res->cardsts_inf.minsec;
	sign_0x5721_mng_t.res.cardsts_inf.card_svitem = res->cardsts_inf.card_svitem;
	sign_0x5721_mng_t.res.cardsts_inf.card_extitem = res->cardsts_inf.card_extitem;
	sign_0x5721_mng_t.res.cardsts_inf.card_cntitem = res->cardsts_inf.card_cntitem;
	sign_0x5721_mng_t.res.cardsts_inf.card_extcntitem = res->cardsts_inf.card_extcntitem;
	sign_0x5721_mng_t.res.cardsts_inf.card_fbnum = res->cardsts_inf.card_fbnum;
	
	for(fb_count = 0; fb_count< res->cardsts_inf.card_fbnum; fb_count++)
	{
		fb_every_com = (RCT_CARDSTS_FB_EVERY_COM_T *)next_p;
		next_p += sizeof(*fb_every_com);

		m_cm_CardStsResFbIdToName(fb_every_com->fbId, fb_name,sizeof(fb_name));
		
		if(fb_count<D_RCT_RECARDSTS_FB_NUM)
		{
			sign_0x5721_mng_t.fb_every[fb_count].com.fbId = fb_every_com->fbId;
			sign_0x5721_mng_t.fb_every[fb_count].com.fbAlmSta = fb_every_com->fbAlmSta;
			sign_0x5721_mng_t.fb_every[fb_count].com.fbBaseSVItem = fb_every_com->fbBaseSVItem;
			sign_0x5721_mng_t.fb_every[fb_count].com.fbExtSVItem  = fb_every_com->fbExtSVItem;
			sign_0x5721_mng_t.fb_every[fb_count].com.fbCarNum = fb_every_com->fbCarNum;
		}
		
		for(car_count=0; car_count < fb_every_com->fbCarNum && car_count < CMD_MAX_CARNUM; car_count++)
		{
			car_every_com = (RCT_CARDSTS_FB_CAR_EVERY_COM_T *)next_p;
			next_p += sizeof(*car_every_com);

			
			if(fb_count<D_RCT_RECARDSTS_FB_NUM && car_count <CMD_MAX_CARNUM)
			{
				sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carNo = car_every_com->carNo;
				sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carBandwidth = car_every_com->carBandwidth;
				sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carBaseSVItem = car_every_com->carBaseSVItem;
				sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carExtSVItem = car_every_com->carExtSVItem;
				sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carBrNum = car_every_com->carBrNum;
			}
			
			
			for(br_count=0; br_count < car_every_com->carBrNum && br_count < CMD_MAX_BRNUM; br_count++)
			{
				br_every = (RCT_CARDSTS_FB_CAR_BR_EVERY_T *)next_p;
				next_p += sizeof(*br_every);

				
				if(fb_count<D_RCT_RECARDSTS_FB_NUM && car_count <CMD_MAX_CARNUM && br_count <CMD_MAX_BRNUM)
				{
					sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].br_every[br_count].brNo = br_every->brNo;
					sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].br_every[br_count].brBaseSVItem = br_every->brBaseSVItem;
					sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].br_every[br_count].brExtSVItem = br_every->brExtSVItem;
				}	
			}
		}
	}
	alm = (RCT_CARDSTS_ALM_COM_T *)next_p;
	next_p += sizeof(*alm);
	
	sign_0x5721_mng_t.alm.almNum = alm->almNum;
	
	for(alm_count = 0;alm_count<alm->almNum && alm_count < CMD_MAX_ALM_NUM;alm_count++)
	{
		alm_every = (RCT_CARDSTS_ALM_EVERY_T *)next_p;
		next_p += sizeof(*alm_every);
		if(alm_count < D_RCT_RECARDSTS_ALM_NUM)
		{
			sign_0x5721_mng_t.alm_every[alm_count].almcd = alm_every->almcd; /* pgr0013 pgr0862 */
		}
	}
	err = (RCT_CARDSTS_ERR_COM_T *)next_p;
	next_p += sizeof(*err);
	sign_0x5721_mng_t.err.errNum = err->errNum;
	for(err_count = 0; err_count < err->errNum && err_count < CMD_MAX_ERR_NUM; err_count++)
	{
		err_every = (RCT_CARDSTS_ERR_EVERY_T *)next_p;
		next_p += sizeof(*err_every);
		
		if(err_count<D_RCT_RECARDSTS_ERR_NUM)
		{
			sign_0x5721_mng_t.err_every[err_count].errcd = err_every->errcd; /* pgr0013 pgr0862 */
		}
		
	}
	curmsec = msec*16;

	fprintf(outf,"(%04d)(%04X)%04X/%02X/%02X %02X:%02X:%02X:%03X : %-10s :   -      :%04X/%04X:%04X/%04X:                                                                              \n",\
		disp_no,sign_0x5721_mng_t.res.signal_kind,sign_0x5721_mng_t.res.cardsts_inf.year,sign_0x5721_mng_t.res.cardsts_inf.month_day >> CMD_BIT8,sign_0x5721_mng_t.res.cardsts_inf.month_day & 0xFF,\
			sign_0x5721_mng_t.res.cardsts_inf.hour,sign_0x5721_mng_t.res.cardsts_inf.minsec >> CMD_BIT8,sign_0x5721_mng_t.res.cardsts_inf.minsec & 0xFF,curmsec,\
			sign_0x5721_mng_t.res.cardsts_inf.card_name,sign_0x5721_mng_t.res.cardsts_inf.card_svitem,sign_0x5721_mng_t.res.cardsts_inf.card_extitem,\
			sign_0x5721_mng_t.res.cardsts_inf.card_cntitem,sign_0x5721_mng_t.res.cardsts_inf.card_extcntitem);

	memset(carbarbuf,'\0',sizeof(carbarbuf));
	memset(everycarbarbuf,'\0',sizeof(everycarbarbuf));
	
	for(fb_count = 0; fb_count< sign_0x5721_mng_t.res.cardsts_inf.card_fbnum; fb_count++)
	{
		if(fb_count>=D_RCT_RECARDSTS_FB_NUM)
		{
			break;
		}	
		m_cm_CardStsResFbIdToName(sign_0x5721_mng_t.fb_every[fb_count].com.fbId, fb_name,sizeof(fb_name));

		m_cm_CardStsResAlmstsIdToName(sign_0x5721_mng_t.fb_every[fb_count].com.fbAlmSta,almsts_name,sizeof(almsts_name));

		
		memset(carbarbuf,'\0',sizeof(carbarbuf));
		for(car_count=0; car_count < sign_0x5721_mng_t.fb_every[fb_count].com.fbCarNum && car_count < CMD_MAX_CARNUM; car_count++)
		{
			snprintf(everycarbarbuf,sizeof(everycarbarbuf),"(%04X/%04X)",sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carBaseSVItem,sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carExtSVItem);
			strcat(carbarbuf,everycarbarbuf);
			for(br_count=0; br_count < sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].com.carBrNum && br_count < CMD_MAX_BRNUM; br_count++)
			{
				snprintf(everycarbarbuf,sizeof(everycarbarbuf),"(%04X/%04X)",sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].br_every[br_count].brBaseSVItem,sign_0x5721_mng_t.fb_every[fb_count].car_every[car_count].br_every[br_count].brExtSVItem);
				strcat(carbarbuf,everycarbarbuf);
			}
			for(nobr_count = 0;nobr_count<(CMD_MAX_BRNUM - br_count);nobr_count++)
			{
				strcpy(everycarbarbuf,"(  - / -  )");
				strcat(carbarbuf,everycarbarbuf);
			}
			if(car_count==0)
			{
				strcat(carbarbuf,":");
			}
				
		}
		for(nocar_count = car_count;nocar_count<CMD_MAX_CARNUM;nocar_count++)
		{
			strcat(carbarbuf,"(  - / -  )");
			for(nobr_count = 0;nobr_count<CMD_MAX_BRNUM;nobr_count++)
			{
				strcpy(everycarbarbuf,"(  - / -  )");
				strcat(carbarbuf,everycarbarbuf);
			}
			if(nocar_count==0)
			{
				strcat(carbarbuf,":");
			}
		}
		fprintf(outf,"                                    : %-10s : %-8s :%04X/%04X:  - / -  :%s\n",fb_name,almsts_name,\
			sign_0x5721_mng_t.fb_every[fb_count].com.fbBaseSVItem,sign_0x5721_mng_t.fb_every[fb_count].com.fbExtSVItem,carbarbuf);	
	}

	memset(everycode,'\0',sizeof(everycode));
	memset(code_set,'\0',sizeof(code_set));

	for(alm_count = 0;alm_count<sign_0x5721_mng_t.alm.almNum && alm_count < D_RCT_RECARDSTS_ALM_NUM;alm_count++)
	{
		strcat(code_set,"/");
		snprintf(everycode,sizeof(everycode),"%04X",sign_0x5721_mng_t.alm_every[alm_count].almcd);
		strcat(code_set,everycode);
	}
	fprintf(outf,"                                    : ALM Num=%04X          :AlmCode  %s\n",sign_0x5721_mng_t.alm.almNum,code_set);	

	memset(everycode,'\0',sizeof(everycode));
	memset(code_set,'\0',sizeof(code_set));
	for(err_count = 0; err_count < sign_0x5721_mng_t.err.errNum && err_count < D_RCT_RECARDSTS_ERR_NUM; err_count++)
	{
		strcat(code_set,"/");
		snprintf(everycode,sizeof(everycode),"%04X",sign_0x5721_mng_t.err_every[err_count].errcd);
		strcat(code_set,everycode);
	}
	fprintf(outf,"                                    : ERR Num=%04X          :ErrCode  %s\n",sign_0x5721_mng_t.err.errNum,code_set);
	fprintf(outf, "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	
}

void m_cm_CarMsgTurnToString(RCT_CARSTS_CAR_EVERY_T car_every,CHAR *pCarBuf)
{
	CHAR everycode[CMD_NUM24];
	USHORT nobr_count = 0;
	USHORT br_count = 0;
	memset(everycode,'\0',sizeof(everycode));
	if(car_every.com.car_no!=0x0000)
	{
		snprintf(everycode,sizeof(everycode),"%04X/%04X/%04X:",car_every.com.car_sta,car_every.com.car_sta_ul,car_every.com.car_sta_dl);
		strcat(pCarBuf,everycode);
		for(br_count = 0; br_count<car_every.com.br_num && br_count<CMD_MAX_BRNUM; br_count++)
		{
			snprintf(everycode,sizeof(everycode),"(%04X/%04X/%04X)",car_every.br_every[br_count].br_car_sta,car_every.br_every[br_count].br_car_sta_ul,car_every.br_every[br_count].br_car_sta_dl);
			strcat(pCarBuf,everycode);
		}
	}
	else
	{
		strcat(pCarBuf,"  - /  - /  - :");
	}
	for(nobr_count = br_count;nobr_count<CMD_MAX_BRNUM;nobr_count++)
	{
		strcat(pCarBuf,"(  - /  - /  - )");
	}
}


VOID m_cm_CarStsResPrintOneReform(CMT_CPRIF_CARLSTSRES* log_data, FILE *outf,CMT_CPRI_EXTENDED_FIELD *pExtended_field)
{
	RCT_CARSTS_CAR_EVERY_COM_T    *car_every_com;
	RCT_CARSTS_CAR_BR_EVERY_T    *br_every;
	RCT_CARSTS_ALM_T    *alm;
	RCT_CARSTS_ALM_EVERY_T    *alm_every;
	CMT_CPRIF_CARLSTSRES *res;
	RCT_CARSTS_CARD_T *card;
	RCT_CARSTS_CARD_EVERY_T *card_every;
	RCT_CARSTS_MNG_T sign_0x5821_mng_t;

	UCHAR                *cur_p = (UCHAR*)log_data;

	USHORT car_count;
	USHORT br_count;
	USHORT alm_count;
	USHORT cursec = 0;
	CHAR everycode[CMD_NUM24];
	CHAR carcode[CMD_NUM200];
	CHAR almcodebuf[CMD_NUM1000];
	CHAR carcodePosOne[CMD_NUM100];
	CHAR carcodePosTwo[CMD_NUM100];
	
	memset(everycode,'\0',sizeof(everycode));
	memset(carcode,'\0',sizeof(carcode));
	memset(carcodePosOne,'\0',sizeof(carcodePosOne));
	memset(carcodePosTwo,'\0',sizeof(carcodePosTwo));
	
	memset(almcodebuf,'\0',sizeof(almcodebuf));
	memset(&sign_0x5821_mng_t,0x00,sizeof(sign_0x5821_mng_t));
	
	res = (CMT_CPRIF_CARLSTSRES *)cur_p;
	cur_p += sizeof(*res);
	
	sign_0x5821_mng_t.res.signal_kind = res->signal_kind;
	sign_0x5821_mng_t.res.result = res->result;
	sign_0x5821_mng_t.res.car_num = res->car_num;

	fprintf(outf,"%s\n","---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	
	for(car_count = 0;car_count < res->car_num && car_count<CMD_MAX_CARNUM_S3G; car_count++)
	{
		
		car_every_com = (RCT_CARSTS_CAR_EVERY_COM_T *)cur_p;
		cur_p += sizeof(*car_every_com);
		
		sign_0x5821_mng_t.car_every[car_count].com.car_no = car_every_com->car_no;
		sign_0x5821_mng_t.car_every[car_count].com.car_sta = car_every_com->car_sta;
		sign_0x5821_mng_t.car_every[car_count].com.car_sta_ul = car_every_com->car_sta_ul;
		sign_0x5821_mng_t.car_every[car_count].com.car_sta_dl = car_every_com->car_sta_dl;
		sign_0x5821_mng_t.car_every[car_count].com.br_num = car_every_com->br_num;
		
		for(br_count = 0; br_count<car_every_com->br_num && br_count<CMD_MAX_BRNUM; br_count++)
		{
			br_every = (RCT_CARSTS_CAR_BR_EVERY_T *)cur_p;
			cur_p += sizeof(*br_every);
			sign_0x5821_mng_t.car_every[car_count].br_every[br_count].br_no = br_every->br_no;
			sign_0x5821_mng_t.car_every[car_count].br_every[br_count].br_car_sta = br_every->br_car_sta;
			sign_0x5821_mng_t.car_every[car_count].br_every[br_count].br_car_sta_ul = br_every->br_car_sta_ul;
			sign_0x5821_mng_t.car_every[car_count].br_every[br_count].br_car_sta_dl = br_every->br_car_sta_dl;
			
		}
		
	}

	memset(carcodePosOne,'\0',sizeof(carcodePosOne));
	memset(carcodePosTwo,'\0',sizeof(carcodePosTwo));

	m_cm_CarMsgTurnToString(sign_0x5821_mng_t.car_every[0],carcodePosOne);
	m_cm_CarMsgTurnToString(sign_0x5821_mng_t.car_every[1],carcodePosTwo);
	if(sign_0x5821_mng_t.car_every[0].com.car_no == 0x0001 || sign_0x5821_mng_t.car_every[0].com.car_no == 0x0000)
	{
		strcat(carcode,carcodePosOne);
		strcat(carcode,":");
		strcat(carcode,carcodePosTwo);
	}
	else if(sign_0x5821_mng_t.car_every[0].com.car_no == 0x0002)
	{
		strcat(carcode,carcodePosTwo);
		strcat(carcode,":");
		strcat(carcode,carcodePosOne);
	}
	cursec = pExtended_field->msec * 16;

	fprintf(outf,"(%04d)(%04X)%04X/%02X/%02X %02X:%02X:%02X.%03X :%04x:%s\n",\
		pExtended_field->disp_no,res->signal_kind,pExtended_field->year,pExtended_field->month,pExtended_field->day,pExtended_field->hour,pExtended_field->min,pExtended_field->sec,cursec/*pExtended_field->msec*/,res->car_num,\
		carcode);
	
	alm = (RCT_CARSTS_ALM_T *)cur_p;
	cur_p += sizeof(*alm);
	
	sign_0x5821_mng_t.alm.alm_num = alm->alm_num;
	
	memset(everycode,'\0',sizeof(everycode));
	
	for(alm_count = 0; alm_count<alm->alm_num && alm_count<CMD_ALM_NUM; alm_count++)
	{
		alm_every = (RCT_CARSTS_ALM_EVERY_T *)cur_p;
		cur_p += sizeof(*alm_every);
		
		sign_0x5821_mng_t.alm_every[alm_count].rcd_almcode = alm_every->rcd_almcode; /* pgr0013 pgr0862 */
		snprintf(everycode,sizeof(everycode),"%04X/",alm_every->rcd_almcode);
		
		strcat(almcodebuf,everycode);
	}
	fprintf(outf,"                                    : ALM Num=%04X      : AlmCode  %s\n",sign_0x5821_mng_t.alm.alm_num,almcodebuf);

	card = (RCT_CARSTS_CARD_T *)cur_p;
	cur_p += sizeof(*card );
	
	sign_0x5821_mng_t.card.card_num = card->card_num;
	

	card_every = (RCT_CARSTS_CARD_EVERY_T *)cur_p;
	cur_p += sizeof(*card_every );
	
	
	sign_0x5821_mng_t.card_every[0].slot_no = card_every->slot_no;
	fprintf(outf,"                                    : CardNum=%04X      : SlotNo   %04X\n",sign_0x5821_mng_t.card.card_num,sign_0x5821_mng_t.card_every[0].slot_no);
	return;
}


VOID m_cm_L3ItemLogPrintReform(T_RRH_L3_SR_LOG *log_tbl,int mngitem_idx, FILE *outf, VOIDFUNCPTR print_func)
{
	UINT count;
	T_RRH_SYSTEM_TIME *log_time;
	T_RRH_L3LOG_BLOCKITEM *item;
	UCHAR    *log_start;
	T_RRH_L3LOG_MNGITEM *mng_item = &(log_tbl->mng.item[mngitem_idx]);
	UINT disp_no;

	USHORT loopnum = 0;
	
	for(count = 0; count <mng_item->block_num; count++)
	{
		disp_no = (count + mng_item->next_block_no)% mng_item->block_num;
		item =  (T_RRH_L3LOG_BLOCKITEM *)( ((UCHAR*)log_tbl) + mng_item->start_offset + disp_no * mng_item->block_size);
		log_time = &(item->log_time);
		log_start = &(item->log_data);
		if(item->log_len != CMD_NUM0)
		{
			
			if(loopnum==0)
			{
				fprintf(outf, "%s","----------- The information of L3Log -----------\n");
				fprintf(outf, "%s","----------- The information of card sta res -----------\n");
				fprintf(outf,"%193s",": svItem  : cntItme :(   Car1  )( Car1Br0 )( Car1Br1 )( Car1Br2 )( Car1Br3 ):(   Car2  )( Car2Br0 )( Car2Br1 )( Car2Br2 )( Car2Br3 )\n");
				fprintf(outf,"(ofst)( sig)yyyy/mm/dd hh:mm:ss.msc :   name     : almsts   :base/extr:base/extr:(svBs/svEx)(svBs/svEx)(svBs/svEx)(svBs/svEx)(svBs/svEx):(svBs/svEx)(svBs/svEx)(svBs/svEx)(svBs/svEx)(svBs/svEx)\n");
				fprintf(outf, "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				loopnum++;
			}
			print_func(log_start,outf,disp_no,log_time->msec);
			
			
		}
	}
	
}

VOID m_cm_L3ItemLogCarPrintReform(T_RRH_L3_SR_LOG *log_tbl,int mngitem_idx, FILE *outf, VOIDFUNCPTR print_func)
{
	UINT count;
	T_RRH_SYSTEM_TIME *log_time;
	T_RRH_L3LOG_BLOCKITEM *item;
	UCHAR    *log_start;
	T_RRH_L3LOG_MNGITEM *mng_item = &(log_tbl->mng.item[mngitem_idx]);
	UINT disp_no;

	USHORT loopnum = 0;
	
	for(count = 0; count <mng_item->block_num; count++)
	{
		disp_no = (count + mng_item->next_block_no)% mng_item->block_num;
		item =  (T_RRH_L3LOG_BLOCKITEM *)( ((UCHAR*)log_tbl) + mng_item->start_offset + disp_no * mng_item->block_size);
		log_time = &(item->log_time);
		log_start = &(item->log_data);
		if(item->log_len != CMD_NUM0)
		{
			if(loopnum==0)
			{
				fprintf(outf, "%s\n","----------- The information of carrier status res -----------");
				fprintf(outf,"%s\n","                                    :           Car1    :(   Car1Br0    )(   Car1Br1    )(   Car1Br2    )(   Car1Br3    ):     Car2     :(   Car2Br0    )(   Car2Br1    )(   Car2Br2    )(   Car2Br3    )");
				fprintf(outf,"%s\n","(ofst)( sig)yyyy/mm/dd hh:mm:ss.msc :carN:sts /stsU/stsD:(sts /stsU/stsD)(sts /stsU/stsD)(sts /stsU/stsD)(sts /stsU/stsD):sts /stsU/stsD:(sts /stsU/stsD)(sts /stsU/stsD)(sts /stsU/stsD)(sts /stsU/stsD)");
				loopnum++;
			}
			CMT_CPRI_EXTENDED_FIELD extended_field;
			memset(&extended_field,0x00,sizeof(extended_field));
			extended_field.disp_no = disp_no;
			extended_field.year = log_time->year;
			extended_field.month = log_time->month;
			extended_field.day = log_time->day;
			extended_field.hour = log_time->hour;
			extended_field.min = log_time->min;
			extended_field.sec = log_time->sec;
			extended_field.msec = log_time->msec;
			
			print_func(log_start,outf,&extended_field);
		}
	}
}
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    L3 Msg Log Dump
 *  @note     L3 Msg Log Dump
 *  @param    outf	[in] output file
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_L3LogPrint(FILE *outf)
{
	T_RRH_L3_SR_LOG *log_tbl = cmw_l3log_tbl_p;

	//add Reform Log by@20150608
	m_cm_L3ItemLogPrintReform(log_tbl, D_RRH_L3LOG_MNGIDX_CARDSTS, outf, (VOIDFUNCPTR)m_cm_CardStsResPrintOneReform);
	m_cm_L3ItemLogCarPrintReform(log_tbl, D_RRH_L3LOG_MNGIDX_CARSTS, outf,  (VOIDFUNCPTR)m_cm_CarStsResPrintOneReform);
	//end Reform Log by@20150608
	fflush(outf);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    L3 LOG Table Init for L2 L3
 *  @note     L3 LOG Table Init for L2 L3
 *  @return   None
 *  @retval   -
 *  @date     2015/05/14 FFCS)wuqy create
 */
/********************************************************************************************************************/
VOID m_cm_L3LogTblInit()
{
	T_RRH_L3LOG_MNGITEM *cardsts_item;
	T_RRH_L3LOG_MNGITEM *carlsts_item;
	int errcd;
	int ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L3_SR_LOG,(VOID**)&cmw_l3log_tbl_p,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cm_L3LogTblInit Abort", "share memory get NG", errcd, CMD_NUM0, CMD_NUM0);
		return;
	}

	cardsts_item = &(cmw_l3log_tbl_p->mng.item[D_RRH_L3LOG_MNGIDX_CARDSTS]);
	cardsts_item->start_offset = (UINT)(((UCHAR*)(&(cmw_l3log_tbl_p->card_sts[0]))) - ((UCHAR*)cmw_l3log_tbl_p));
	cardsts_item->block_num = D_RRH_L3LOG_NUM;
	cardsts_item->block_size = sizeof(cmw_l3log_tbl_p->card_sts[0]);
	cardsts_item->max_log_len = sizeof(cmw_l3log_tbl_p->card_sts[0].log_data);
	cardsts_item->next_block_no = (cardsts_item->next_block_no ) % (cardsts_item->block_num);

	carlsts_item = &(cmw_l3log_tbl_p->mng.item[D_RRH_L3LOG_MNGIDX_CARSTS]);
	carlsts_item->start_offset = (UINT)(((UCHAR*)(&(cmw_l3log_tbl_p->car_sts[0]))) - ((UCHAR*)cmw_l3log_tbl_p));
	carlsts_item->block_num = D_RRH_L3LOG_NUM;
	carlsts_item->block_size = sizeof(cmw_l3log_tbl_p->car_sts[0]);
	carlsts_item->max_log_len = sizeof(cmw_l3log_tbl_p->car_sts[0].log_data);
	carlsts_item->next_block_no = (carlsts_item->next_block_no ) % (carlsts_item->block_num);
}
/* @} */


/* @} */
