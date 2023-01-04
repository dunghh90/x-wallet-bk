/* m_dl_typ.c */
#include "m_cm_header.h"															/* MDIF common head file			  */
#include "m_dl_header.h"															/* download management task head file */

/*----------------RE FW info table (1)-------------------*/
/*!
 *  @brief 
 *  @note   対RE-FW情報テーブル初期化
 *  @param  at_REFW_p [in] 初期化対象テーブルへのポインタ
 *  @return Void.
 *  @retval -
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49対処(メーカ識別＆バージョン情報を初期化しないよう修正)
 */
VOID REFW_init(DLT_REFW_TBL* at_REFW_p)
{
	at_REFW_p->link_num = 0;
	at_REFW_p->re_sta = DLD_RESTA_IDLE;
	at_REFW_p->start_factor = DLD_UPDATE_FACTOR_NONE;
	at_REFW_p->use_timer = DLD_RERETIM_NONE;
	at_REFW_p->fhmrefile_inf = 0xFFFF;
	at_REFW_p->trx_div_no = 0;
	at_REFW_p->trx_data_size = 0;
	at_REFW_p->fw_rep_rslt = DLD_RESLT_IDLE;
	at_REFW_p->fileinf_rep_rslt = DLD_RESLT_IDLE;
	at_REFW_p->filedata_trans_rslt = DLD_RESLT_IDLE;
	at_REFW_p->fw_rep_tc = 0;
	at_REFW_p->fileinf_rep_tc = 0;
	at_REFW_p->filedata_trans_tc = 0;
	/* should assert all REFW members are zero */
}
/* REFW get functions */
USHORT	REFW_get_CPRI_no(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->link_num;
}
USHORT	REFW_get_RE_status(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->re_sta;
}
USHORT	REFW_get_update_start_factor(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->start_factor;
}
USHORT	REFW_get_maker_id(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->mk_jdg;
}
USHORT	REFW_get_major_version_using(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->usmjr_ver;
}
USHORT	REFW_get_minor_version_using(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->usmin_ver;
}
USHORT	REFW_get_major_version_startup(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->mjr_ver;
}
USHORT	REFW_get_minor_version_startup(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->min_ver;
}
USHORT	REFW_get_using_timer(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->use_timer;
}
USHORT	REFW_get_FHM_holding_RE_file_info(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->fhmrefile_inf;
}
ULONG	REFW_get_transfer_complete_part_no(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->trx_div_no;
}
ULONG	REFW_get_transfer_complete_data_size(DLT_REFW_TBL* at_REFW_p)
{
	return at_REFW_p->trx_data_size;
}
USHORT	REFW_get_report_result(DLT_REFW_TBL* at_REFW_p, USHORT aw_type)
{
	switch(aw_type)
	{
	case REPORT_RESULT_FW_FILE:
		return at_REFW_p->fw_rep_rslt;
	case REPORT_RESULT_FILE_INFO:
		return at_REFW_p->fileinf_rep_rslt;
	case REPORT_RESULT_FILE_DATA_SEND_COMPLETE:
		return at_REFW_p->filedata_trans_rslt;
	}
	/* should set ero code before return */
	return 0;
}
USHORT	REFW_get_resend_counter(DLT_REFW_TBL* at_REFW_p, USHORT type)
{
	switch(type)
	{
	case DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ:
		return at_REFW_p->fw_rep_tc;
	case DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE:
		return at_REFW_p->fileinf_rep_tc;
	case DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE:
		return at_REFW_p->filedata_trans_tc;
	}
	/* should set ero code before return */
	return 0;
}
/* REFW set functions */
USHORT REFW_set_CPRI_no(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->link_num = aw_value;
	return aw_value;
}
USHORT REFW_set_RE_status(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->re_sta = aw_value;
	return aw_value;
}
USHORT REFW_set_update_start_factor(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->start_factor = aw_value;
	return aw_value;
}
USHORT REFW_set_maker_id(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->mk_jdg = aw_value;
	return aw_value;
}
USHORT REFW_set_major_version_using(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->usmjr_ver = aw_value;
	return aw_value;
}
USHORT REFW_set_minor_version_using(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->usmin_ver = aw_value;
	return aw_value;
}
USHORT REFW_set_major_version_startup(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->mjr_ver = aw_value;
	return aw_value;
}
USHORT REFW_set_minor_version_startup(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->min_ver = aw_value;
	return aw_value;
}
USHORT REFW_set_using_timer(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->use_timer = aw_value;
	return aw_value;
}
USHORT REFW_set_FHM_holding_RE_file_info(DLT_REFW_TBL* at_REFW_p, USHORT aw_value)
{
	at_REFW_p->fhmrefile_inf = aw_value;
	return aw_value;
}
ULONG REFW_set_transfer_complete_part_no(DLT_REFW_TBL* at_REFW_p, ULONG aw_value)
{
	at_REFW_p->trx_div_no = aw_value;
	return aw_value;
}
ULONG REFW_set_transfer_complete_data_size(DLT_REFW_TBL* at_REFW_p, ULONG aw_value)
{
	at_REFW_p->trx_data_size = aw_value;
	return aw_value;
}
USHORT REFW_set_report_result(DLT_REFW_TBL* at_REFW_p, USHORT aw_type, USHORT aw_value)
{
	switch(aw_type)
	{
	case REPORT_RESULT_FW_FILE:
		at_REFW_p->fw_rep_rslt = aw_value;
		break;
	case REPORT_RESULT_FILE_INFO:
		at_REFW_p->fileinf_rep_rslt = aw_value;
		break;
	case REPORT_RESULT_FILE_DATA_SEND_COMPLETE:
		at_REFW_p->filedata_trans_rslt = aw_value;
		break;
	}
	return aw_value;
}
USHORT REFW_set_resend_counter(DLT_REFW_TBL* at_REFW_p, USHORT aw_type, USHORT aw_value)
{
	switch(aw_type)
	{
	case DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ:
		at_REFW_p->fw_rep_tc = aw_value;
		break;
	case DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE:
		at_REFW_p->fileinf_rep_tc = aw_value;
		break;
	case DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE:
		at_REFW_p->filedata_trans_tc = aw_value;
		break;
	}
	return aw_value;
}

/* ------DLM管理テーブル (DLMTBL)----------*/
USHORT DLM_get_executing_DL_start_factor(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->dlstart_factor;
}
/* if (link[i] not exist) return 0
 * else if (link[i] exist) return !=0
 * NOTE: if (aw_CPRI_no == CMD_NUM_0xFF) return link bit vector
 */
USHORT DLM_get_updating_link_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_CPRI_no)
{
	USHORT lw_ret = 0;
	USHORT lw_mask = 0x01 << (aw_CPRI_no -1);

	if (aw_CPRI_no == CMD_NUM_0xFF)
	{
		lw_ret = at_DLM_p->link_inf;
	}
	else
	{
		if(aw_CPRI_no < D_RRH_CPRINO_RE_MIN || aw_CPRI_no > D_RRH_CPRINO_RE_MAX)
		{
			cm_Assert(CMD_ASL_USELOW, aw_CPRI_no, "[m_dl] invalid CPRI no");
		}
		lw_ret = (lw_mask & at_DLM_p->link_inf);
	}
	return lw_ret;
}
USHORT DLM_get_flash_write_flag(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->write_flg;
}
USHORT DLM_get_DownLoading_RE_file_read_status(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->dlrefile_read_sta;
}
USHORT DLM_get_DownLoading_RE_file_info(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->dlrefile_inf;
}
USHORT DLM_get_maker_id(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->mk_jdg;
}
USHORT DLM_get_major_version_using(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->mjr_ver;
}
USHORT DLM_get_minor_version_using(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->min_ver;
}
USHORT DLM_get_major_version_startup(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->usmjr_ver;
}
USHORT DLM_get_minor_version_startup(DLT_DLM_TBL* at_DLM_p)
{
	return at_DLM_p->usmin_ver;
}
USHORT DLM_set_executing_DL_start_factor(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->dlstart_factor = aw_value;
	return aw_value;
}
USHORT DLM_set_updating_link_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_CPRI_no, USHORT aw_value)
{
	/* bit mask */
	USHORT lw_mask = 0x01 << (aw_CPRI_no - 1);
	if (aw_CPRI_no == CMD_NUM_0xFF)
	{
		at_DLM_p->link_inf = aw_value;
	}
	else
	{
		if(aw_CPRI_no < D_RRH_CPRINO_RE_MIN || aw_CPRI_no > D_RRH_CPRINO_RE_MAX)
		{
			cm_Assert(CMD_ASL_USELOW, aw_CPRI_no, "[m_dl] invalid CPRI no");
		}

		/* set bit CPI# ON/OFF */
		if (aw_value == 0)
		{
			at_DLM_p->link_inf &= ~lw_mask;
		}
		else
		{
			//should assert aw_value == 1
			at_DLM_p->link_inf |= lw_mask;
		}
	}
	return at_DLM_p->link_inf;
}
USHORT DLM_set_flash_write_flag(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->write_flg = aw_value;
	return aw_value;
}
USHORT DLM_set_DownLoading_RE_file_read_status(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->dlrefile_read_sta = aw_value;
	return aw_value;
}
USHORT DLM_set_DownLoading_RE_file_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->dlrefile_inf = aw_value;
	return aw_value;
}
USHORT DLM_set_maker_id(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->mk_jdg = aw_value;
	return aw_value;
}
USHORT DLM_set_major_version_using(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->mjr_ver = aw_value;
	return aw_value;
}
USHORT DLM_set_minor_version_using(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->min_ver = aw_value;
	return aw_value;
}
USHORT DLM_set_major_version_startup(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->usmjr_ver = aw_value;
	return aw_value;
}
USHORT DLM_set_minor_version_startup(DLT_DLM_TBL* at_DLM_p, USHORT aw_value)
{
	at_DLM_p->usmin_ver = aw_value;
	return aw_value;
}
/*-------(3)FHM保持REファイル情報------------*/
/* FHM holding RE file info */
USHORT FHMRE_get_maker_id(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->mk_jdg;
}
USHORT FHMRE_get_major_version(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->mjr_ver;
}
USHORT FHMRE_get_minor_version(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->min_ver;
}
USHORT FHMRE_get_maker_id_RE_connection_exist(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->re_conn_exist;
}
UCHAR* FHMRE_get_file_name(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->file_name;
}
ULONG FHMRE_get_file_size(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->file_size;
}
ULONG FHMRE_get_file_date(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->file_size;
}
ULONG FHMRE_get_number_part(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->div_num;
}
ULONG FHMRE_get_file_data_expand_first_addr(DLT_MTN_REINF* at_FHMRE_p)
{
	return at_FHMRE_p->file_addr;
}

USHORT FHMRE_set_maker_id(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value)
{
	at_FHMRE_p->mk_jdg = aw_value;
	return aw_value;
}
USHORT FHMRE_set_major_version(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value)
{
	at_FHMRE_p->mjr_ver = aw_value;
	return aw_value;
}
USHORT FHMRE_set_minor_version(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value)
{
	at_FHMRE_p->min_ver = aw_value;
	return aw_value;
}
USHORT FHMRE_set_maker_id_RE_connection_exist(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value)
{
	at_FHMRE_p->re_conn_exist = aw_value;
	return aw_value;
}
UCHAR* FHMRE_set_file_name(DLT_MTN_REINF* at_FHMRE_p, UCHAR* b_value_p)
{
	const USHORT lw_nchar_max = sizeof(at_FHMRE_p->file_name);
	USHORT lw_len = strlen((char*)b_value_p);

	lw_len = (lw_len < lw_nchar_max)?lw_len:lw_nchar_max;
	memset(at_FHMRE_p->file_name, 0, lw_nchar_max);
	memcpy(at_FHMRE_p->file_name, b_value_p, lw_len);

	return b_value_p;
}
ULONG FHMRE_set_file_size(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value)
{
	at_FHMRE_p->file_size = aw_value;
	return aw_value;
}
//ULONG FHMRE_set_file_date(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value)
//{
//	at_FHMRE_p->file_date = aw_value;
//	return aw_value;
//}
ULONG FHMRE_set_number_part(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value)
{
	at_FHMRE_p->div_num = aw_value;
	return aw_value;
}
ULONG FHMRE_set_file_data_expand_first_addr(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value)
{
	at_FHMRE_p->file_addr = aw_value;
	return aw_value;
}
/* ------REC info table--------------*/
USHORT REC_get_status(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->rec_sta;
}
USHORT REC_get_using_timer(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->use_timer;
}
//USHORT REC_get_FHM_hodling_file_info(DLT_REC_TBL* at_REC_p)
//{
//	return at_REC_p->;
//}
ULONG REC_get_recived_part_no(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->rcv_div_no;
}
ULONG REC_get_recived_data_size(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->rcv_data_size;
}
//USHORT REC_get_FW_file_report_result(DLT_REC_TBL* at_REC_p)
//{
//	return at_REC_p->;
//}
USHORT REC_get_file_info_report_result(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->fileinf_rep_rslt;
}
USHORT REC_get_file_data_send_complete_report_result(DLT_REC_TBL* at_REC_p)
{
	return at_REC_p->filedata_trans_rslt;
}
/*--set methods*/
USHORT REC_set_status(DLT_REC_TBL* at_REC_p, USHORT aw_value)
{
	at_REC_p->rec_sta = aw_value;
	return aw_value;
}
USHORT REC_set_using_timer(DLT_REC_TBL* at_REC_p, USHORT aw_value)
{
	at_REC_p->use_timer = aw_value;
	return aw_value;
}
//USHORT REC_set_FHM_hodling_file_info(DLT_REC_TBL* at_REC_p, USHORT aw_value)
ULONG REC_set_recived_part_no(DLT_REC_TBL* at_REC_p, ULONG aw_value)
{
	at_REC_p->rcv_div_no = aw_value;
	return aw_value;
}
ULONG REC_set_recived_data_size(DLT_REC_TBL* at_REC_p, ULONG aw_value)
{
	at_REC_p->rcv_data_size = aw_value;
	return aw_value;
}
//USHORT REC_set_FW_file_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value)
USHORT REC_set_file_info_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value)
{
	at_REC_p->fileinf_rep_rslt = aw_value;
	return aw_value;
}
USHORT REC_set_file_data_send_complete_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value)
{
	at_REC_p->filedata_trans_rslt = aw_value;
	return aw_value;
}
