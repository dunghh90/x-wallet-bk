/*!
 * @skip  $ld:$
 * @file  f_nmas_LutLogWriteNtf.c.c
 * @brief LUT LOG write request
 * @date  2019/07/16 Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */
#include "f_nmas_inc.h"


static UINT		f_nmasr_lut_log_addr[D_DU_LUT_LOG_MAX] = {
	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_0,
	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_1,
	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_2,
	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_3
};


/* work用	*/
static UCHAR				f_nmasw_lut_log_cmpress[D_DU_QSPI_FLASH_SIZE_LUT_LOG_ONE];
static T_RRH_LUT_LOG_HEAD	f_nmasw_lut_log_header;
static T_RRH_LUT_LOG_DATA	f_nmasw_lut_log_data;
static FLDC4_ELEM			f_nmasw_lut_fa_dictionary;

/*!
 * @brief LUT LOG Write Notice
 * @note  LUT LOG Write Notice
 * @param  *pRcvMsg    [in]VOID   received message
 * @return N/A
 * @date  2019/07/16 Taniguchi Create
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nmas_LutLogWriteNtf(VOID* pRcvMsg)
{
	T_SYS_NMA_LUTLOG_WRITE_NFT*	msg;
	UINT						outlen;
	INT							rtn_bpf;
	INT							errcd;
	CHAR						outlogfile[256];
	FILE*						fp;
	struct bpf_timespec			dtime;	/* sem waittime */	
	dtime.tv_sec = 1;
	dtime.tv_nsec = 0;

	msg = (T_SYS_NMA_LUTLOG_WRITE_NFT*)pRcvMsg;

	if(msg->uiIndex >= D_DU_LUT_LOG_MAX){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "LUT LOG Index ERROR:0x%08x",msg->uiIndex);
		return;
	}else{
	}

	memset(&f_nmasw_lut_log_header,0,sizeof(f_nmasw_lut_log_header));
	memset(&f_nmasw_lut_log_data,0,sizeof(f_nmasw_lut_log_data));
	memset(&f_nmasw_lut_fa_dictionary,0,sizeof(f_nmasw_lut_fa_dictionary));
	memset(f_nmasw_lut_log_cmpress,0,sizeof(f_nmasw_lut_log_cmpress));

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "SEM ERROR LUT LOG Index:0x%08x 0x%x",msg->uiIndex,errcd);
		f_nmas_lut_log->head.flash_head[msg->uiIndex].flash_saving = D_RRH_OFF;
		return;
	}

		f_nmas_lut_log->head.flash_head[msg->uiIndex].flash_ofs = 0;
		f_nmas_lut_log->head.flash_head[msg->uiIndex].flash_len = 0;
		memcpy(&f_nmasw_lut_log_header,&(f_nmas_lut_log->head),sizeof(f_nmasw_lut_log_header));
		memcpy(&f_nmasw_lut_log_data,&(f_nmas_lut_log->log[msg->uiIndex]),sizeof(f_nmasw_lut_log_data));
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG copy:0x%08x",msg->uiIndex);

	(void)BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, &errcd);

	rtn_bpf = f_com_FA_encode(
		(unsigned char*)&f_nmasw_lut_log_data,
		f_nmasw_lut_log_cmpress,
		sizeof(f_nmasw_lut_log_data),
		sizeof(f_nmasw_lut_log_cmpress),
		&f_nmasw_lut_fa_dictionary,
		&outlen	);

	if((rtn_bpf != D_RRH_OK) || (outlen > sizeof(f_nmasw_lut_log_cmpress))){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "Compress Error(0x%08x) LUT LOG copy:0x%08x outlen:0x%x",rtn_bpf,msg->uiIndex,outlen);
		return;
	}

	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG saved start 0x%08x outlen:0x%x",msg->uiIndex,outlen);

	/* ヘッダイレース */
	f_nmas_cm_EraseFlash(D_DU_QSPI_FLASH_OFFSET_LUT_LOG_HEAD,D_DU_QSPI_FLASH_SIZE_LUT_LOG_HEAD);
	/* データイレース */
	f_nmas_cm_EraseFlash(f_nmasr_lut_log_addr[msg->uiIndex],outlen);
	/* データ書き込み */
	BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(f_nmasr_lut_log_addr[msg->uiIndex],outlen,f_nmasw_lut_log_cmpress);

	/* ヘッダ書き込み */
	f_nmasw_lut_log_header.flash_head[msg->uiIndex].flash_ofs = f_nmasr_lut_log_addr[msg->uiIndex];
	f_nmasw_lut_log_header.flash_head[msg->uiIndex].flash_len = outlen;
	f_nmasw_lut_log_header.flash_head[msg->uiIndex].flash_saving = D_RRH_OFF;
	f_nmasw_lut_log_header.validFlg = D_RRH_ON;

	BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(D_DU_QSPI_FLASH_OFFSET_LUT_LOG_HEAD,sizeof(f_nmasw_lut_log_header),(unsigned char*)&f_nmasw_lut_log_header);

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "SEM ERROR LUT LOG Index:0x%08x 0x%x",msg->uiIndex,errcd);
		f_nmas_lut_log->head.flash_head[msg->uiIndex].flash_saving = D_RRH_OFF;
		return;
	}
		memcpy(&(f_nmas_lut_log->head.flash_head[msg->uiIndex]),&(f_nmasw_lut_log_header.flash_head[msg->uiIndex]),sizeof(f_nmas_lut_log->head.flash_head[msg->uiIndex]));
		f_nmas_lut_log->head.validFlg = D_RRH_ON;

	(void)BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, &errcd);

	memset(outlogfile,0,sizeof(outlogfile));
	snprintf(outlogfile,sizeof(outlogfile),"/var/log/LUTLog%d.bin",msg->uiIndex);
	fp = fopen( outlogfile , "wb" );
	if(fp != NULL){
		fwrite((void*)f_nmasw_lut_log_cmpress,1,outlen,fp);
		fflush(fp);
		fclose(fp);
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "Updated LUT LOG(%d) filename:%s size:%d",msg->uiIndex,outlogfile,outlen);
	}

	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG saved finish 0x%08x",msg->uiIndex);

	return ;
}
/* @} */

/*!
 * @brief LUT LOG Write Notice
 * @note  LUT LOG Write Notice
 * @param  N/A
 * @return N/A
 * @date  2019/07/16 Taniguchi Create
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nmas_LutLogDecode(VOID)
{
	UINT						logcount;
	UINT						flash_ofs;
	UINT						flash_len;
	UINT						flash_saved;
	CHAR*						outfilename[D_DU_LUT_LOG_MAX] = {
		"/var/log/LUTLog0.bin",
		"/var/log/LUTLog1.bin",
		"/var/log/LUTLog2.bin",
		"/var/log/LUTLog3.bin"
	};

	FILE*						fp;

	BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(D_DU_QSPI_FLASH_OFFSET_LUT_LOG_HEAD, sizeof(f_nmas_lut_log->head),(unsigned char *)&(f_nmas_lut_log->head));
	if(( f_nmas_lut_log->head.index >= D_DU_LUT_LOG_MAX) || (f_nmas_lut_log->head.validFlg != D_RRH_ON)){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG not saved 0x%x flg:0x%x", f_nmas_lut_log->head.index,f_nmas_lut_log->head.validFlg);
		memset((unsigned char *)&(f_nmas_lut_log->head),0,sizeof(f_nmas_lut_log->head));
	}else{
		for( logcount=0 ; logcount < D_DU_LUT_LOG_MAX ; logcount++ ){
			flash_saved = f_nmas_lut_log->head.flash_head[logcount].flash_saved;
			f_nmas_lut_log->head.flash_head[logcount].flash_saved = D_RRH_OFF;
			if(flash_saved != f_comw_rrhVersion){
				cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG Index:%d Version mismatch flashVer:0x%x curVer:0x%x",logcount,flash_saved,f_comw_rrhVersion);
				continue;
			}
			flash_ofs = f_nmas_lut_log->head.flash_head[logcount].flash_ofs;
			flash_len = f_nmas_lut_log->head.flash_head[logcount].flash_len;
			if(
				(flash_ofs != f_nmasr_lut_log_addr[logcount]) || 
				(flash_len  == 0 ) ||
				(flash_len > sizeof(f_nmasw_lut_log_cmpress))
			){
				cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG Index:%d ofs:0x%x len:0x%x",logcount, flash_ofs,flash_len);
				continue;
			}

			BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(flash_ofs, flash_len,f_nmasw_lut_log_cmpress);

			fp = fopen( outfilename[logcount], "wb" );
			if(fp != NULL){
				fwrite((void*)f_nmasw_lut_log_cmpress,1,flash_len,fp);
				fflush(fp);
				fclose(fp);
				cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "LUT LOG OutPut filename:%s size:%d",outfilename[logcount],flash_len);
			}
		}
	}
	return;
}

/* @} */
