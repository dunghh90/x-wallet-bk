/*!
 * @skip   $Id$
 * @file   M_dl_ramAccess.c
 * @brief  DLM read/write share ram
 * @date   2015/08/17
 * @date   2015/11/04 TDIPS)sasaki Update
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
*/

/*! 
 * @addtogroup RRH_L3_DLM
 * @{ 
 */


/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "m_cm_header.h"			/* MDIF共通ヘッダファイル				*/
#include "m_dl_header.h"			/* RE監視タスクヘッダファイル			*/

/*-------used for UNIT test--------------------*/
#if(SEND_MSG_UT || TIMER_CHECK_UT || CALL_STACK_UT)
#define cw_max_stored	16
typedef struct {
	ULONG data[cw_max_stored];
	ULONG cursor;
} buff_for_ut;
static VOID buff_save(buff_for_ut* at_buff_p, ULONG aw_code)
{
	USHORT lw_cursor = at_buff_p->cursor;
	lw_cursor = (lw_cursor+1)%cw_max_stored;
	at_buff_p->data[lw_cursor] = aw_code;
	at_buff_p->cursor = lw_cursor;
}
static ULONG buff_get(buff_for_ut* at_buff_p, USHORT aw_backward)
{
	//should ensure 0<aw_backward<16
	aw_backward &= 0x000F;
	USHORT lw_cursor = at_buff_p->cursor + cw_max_stored - aw_backward;
	lw_cursor %= cw_max_stored;
	return at_buff_p->data[lw_cursor];
}
#endif /* buff for unitest */

/* ------(2) DLM manager table (DLM)-----------*/
/* ------DLM管理テーブル (DLM_MNG_TBL)----------*/
VOID init_DLM()
{
	memset(&gt_dlw_dlm_tbl, 0, sizeof(gt_dlw_dlm_tbl));
	set_DownLoading_RE_file_info(0xffFF);
}
USHORT get_executing_DL_start_factor()
{
	return DLM_get_executing_DL_start_factor(&gt_dlw_dlm_tbl);
}
USHORT get_updating_link_info(USHORT aw_CPRI_no)
{
	//should ensure aw_CPRI_no = 1~16
	USHORT lw_value = DLM_get_updating_link_info(&gt_dlw_dlm_tbl, aw_CPRI_no);
	lw_value = (lw_value != 0) ? 1 : 0;
	return lw_value;
}
USHORT get_flash_write_flag()
{
	return DLM_get_flash_write_flag(&gt_dlw_dlm_tbl);
}
USHORT get_DownLoading_RE_file_read_status()
{
	return DLM_get_DownLoading_RE_file_read_status(&gt_dlw_dlm_tbl);
}
USHORT get_DownLoading_RE_file_info()
{
	return DLM_get_DownLoading_RE_file_info(&gt_dlw_dlm_tbl);
}
USHORT get_DLM_maker_id()
{
	return DLM_get_maker_id(&gt_dlw_dlm_tbl);
}
USHORT get_DLM_major_version_using()
{
	return DLM_get_major_version_using(&gt_dlw_dlm_tbl);
}
USHORT get_DLM_minor_version_using()
{
	return DLM_get_minor_version_using(&gt_dlw_dlm_tbl);
}
USHORT get_DLM_major_version_startup()
{
	return DLM_get_major_version_startup(&gt_dlw_dlm_tbl);
}
USHORT get_DLM_minor_version_startup()
{
	return DLM_get_minor_version_startup(&gt_dlw_dlm_tbl);
}
VOID set_executing_DL_start_factor(USHORT aw_value)
{
	DLM_set_executing_DL_start_factor(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_updating_link_info(USHORT aw_CPRI_no, USHORT aw_value)
{
	DLM_set_updating_link_info(&gt_dlw_dlm_tbl, aw_CPRI_no, aw_value);
}
VOID set_flash_write_flag(USHORT aw_value)
{
	DLM_set_flash_write_flag(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DownLoading_RE_file_read_status(USHORT aw_value)
{
	DLM_set_DownLoading_RE_file_read_status(&gt_dlw_dlm_tbl, aw_value);
}
/*aw_value: index in FHM holding RE file info*/
VOID set_DownLoading_RE_file_info(USHORT aw_value)
{
	/*should assert aw_value in 0~16*/
	DLM_set_DownLoading_RE_file_info(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DLM_maker_id(USHORT aw_value)
{
	DLM_set_maker_id(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DLM_major_version_using(USHORT aw_value)
{
	DLM_set_major_version_using(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DLM_minor_version_using(USHORT aw_value)
{
	DLM_set_minor_version_using(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DLM_major_version_startup(USHORT aw_value)
{
	DLM_set_major_version_startup(&gt_dlw_dlm_tbl, aw_value);
}
VOID set_DLM_minor_version_startup(USHORT aw_value)
{
	DLM_set_minor_version_startup(&gt_dlw_dlm_tbl, aw_value);
}

/* ------(4) REC info table----------*/
/*--access REC info table*/
VOID init_REC()
{
	memset(&gt_dlw_rec_tbl, 0, sizeof(gt_dlw_rec_tbl));
}
/*get & set*/
USHORT get_REC_status()
{
	return REC_get_status(&gt_dlw_rec_tbl);
}
USHORT get_REC_using_timer()
{
	return REC_get_using_timer(&gt_dlw_rec_tbl);
}
ULONG  get_REC_recived_part_no()
{
	return REC_get_recived_part_no(&gt_dlw_rec_tbl);
};
ULONG  get_REC_recived_data_size()
{
	return REC_get_recived_data_size(&gt_dlw_rec_tbl);
}
USHORT get_REC_file_info_report_result()
{
	return REC_get_file_info_report_result(&gt_dlw_rec_tbl);
}
USHORT get_REC_file_data_send_complete_report_result()
{
	return REC_get_file_data_send_complete_report_result(&gt_dlw_rec_tbl);
}

VOID set_REC_status(USHORT aw_value)
{
	REC_set_status(&gt_dlw_rec_tbl, aw_value);
}
VOID set_REC_using_timer(USHORT aw_value)
{
	REC_set_using_timer(&gt_dlw_rec_tbl, aw_value);
}
VOID set_REC_recived_part_no(ULONG aw_value)
{
	REC_set_recived_part_no(&gt_dlw_rec_tbl, aw_value);
}
VOID set_REC_recived_data_size(ULONG aw_value)
{
	REC_set_recived_data_size(&gt_dlw_rec_tbl, aw_value);
}
VOID set_REC_file_info_report_result(USHORT aw_value)
{
	REC_set_file_info_report_result(&gt_dlw_rec_tbl, aw_value);
}
VOID set_REC_file_data_send_complete_report_result(USHORT aw_value)
{
	REC_set_file_data_send_complete_report_result(&gt_dlw_rec_tbl, aw_value);
}

/* ------(1) RE-FW information table (REFW)----*/
/* access shared REFW */
/* init */
VOID init_all_REFW()
{
	USHORT lw_index;
	const USHORT lw_count = sizeof(gt_dlw_refw_tbl)/sizeof(gt_dlw_refw_tbl[0]);
	for(lw_index = 0; lw_index < lw_count; lw_index++)
	{
		REFW_init(&gt_dlw_refw_tbl[lw_index]);
	}
}
VOID	init_REFW(USHORT aw_index)
{
	REFW_init(&gt_dlw_refw_tbl[aw_index]);
}
/* get functions */
USHORT	get_CPRI_no(USHORT aw_index)
{
	return REFW_get_CPRI_no(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_RE_status(USHORT aw_index)
{
	USHORT lw_status = REFW_get_RE_status(&gt_dlw_refw_tbl[aw_index]);
	return lw_status;
}
USHORT	get_update_start_factor(USHORT aw_index)
{
	return REFW_get_update_start_factor(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_maker_id(USHORT aw_index)
{
	return REFW_get_maker_id(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_major_version_using(USHORT aw_index)
{
	return REFW_get_major_version_using(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_minor_version_using(USHORT aw_index)
{
	return REFW_get_minor_version_using(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_major_version_startup(USHORT aw_index)
{
	return REFW_get_major_version_startup(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_minor_version_startup(USHORT aw_index)
{
	return REFW_get_minor_version_startup(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_REFW_using_timer(USHORT aw_index)
{
	return REFW_get_using_timer(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_FHMRE_index(USHORT aw_index)
{
	return REFW_get_FHM_holding_RE_file_info(&gt_dlw_refw_tbl[aw_index]);
}
ULONG	get_transfer_complete_part_no(USHORT aw_index)
{
	return REFW_get_transfer_complete_part_no(&gt_dlw_refw_tbl[aw_index]);
}
ULONG	get_transfer_complete_data_size(USHORT aw_index)
{
	return REFW_get_transfer_complete_data_size(&gt_dlw_refw_tbl[aw_index]);
}
USHORT	get_report_result(USHORT aw_index, USHORT aw_type)
{
	return REFW_get_report_result(&gt_dlw_refw_tbl[aw_index], aw_type);
}
USHORT	get_resend_counter(USHORT aw_index, USHORT aw_type)
{
	return REFW_get_resend_counter(&gt_dlw_refw_tbl[aw_index], aw_type);
}
/* set fucntions */
VOID	set_CPRI_no(USHORT aw_index, USHORT aw_CPRI_no)
{
	REFW_set_CPRI_no(&gt_dlw_refw_tbl[aw_index], aw_CPRI_no);
}
VOID	set_RE_status(USHORT aw_index, USHORT aw_status)
{
	REFW_set_RE_status(&gt_dlw_refw_tbl[aw_index], aw_status);
}
VOID	set_update_start_factor(USHORT aw_index, USHORT aw_value)
{
	REFW_set_update_start_factor(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_maker_id(USHORT aw_index, USHORT aw_value)
{
	REFW_set_maker_id(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_major_version_using(USHORT aw_index, USHORT aw_value)
{
	REFW_set_major_version_using(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_minor_version_using(USHORT aw_index, USHORT aw_value)
{
	REFW_set_minor_version_using(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_major_version_startup(USHORT aw_index, USHORT aw_value)
{
	REFW_set_major_version_startup(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_minor_version_startup(USHORT aw_index, USHORT aw_value)
{
	REFW_set_minor_version_startup(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_REFW_using_timer(USHORT aw_index, USHORT aw_value)
{
	REFW_set_using_timer(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_FHMRE_index(USHORT aw_index, USHORT aw_value)
{
	REFW_set_FHM_holding_RE_file_info(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_transfer_complete_part_no(USHORT aw_index, ULONG aw_value)
{
	REFW_set_transfer_complete_part_no(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_transfer_complete_data_size(USHORT aw_index, ULONG aw_value)
{
	REFW_set_transfer_complete_data_size(&gt_dlw_refw_tbl[aw_index], aw_value);
}
VOID	set_report_result(USHORT aw_index, USHORT aw_type, USHORT aw_value)
{
	REFW_set_report_result(&gt_dlw_refw_tbl[aw_index], aw_type, aw_value);
}
VOID	set_resend_counter(USHORT aw_index, USHORT aw_type, USHORT aw_value)
{
	REFW_set_resend_counter(&gt_dlw_refw_tbl[aw_index], aw_type, aw_value);
}
/*--index map*/
USHORT convert_CPRI_no_REFW_index(USHORT aw_CPRI_no)
{
	//should assert aw_CPRI_no = 1~16
	return aw_CPRI_no - 1;
}
/*--search index by maker id*/
//if not found return 0xFFFF
USHORT search_REFW_maker_id(USHORT aw_maker_id)
{
	USHORT lw_index = 0;
	USHORT lw_count = sizeof(gt_dlw_refw_tbl)/sizeof(DLT_REFW_TBL);
	for(; lw_index < lw_count; lw_index++)
	{
		if (aw_maker_id == REFW_get_maker_id(&gt_dlw_refw_tbl[lw_index]))
		{
			break;
		}
	}
	lw_index = (lw_index < lw_count)?lw_index:0xFFFF;
	return lw_index;
}
/*--resend counter*/
USHORT can_retry(USHORT aw_index, USHORT aw_type)
{
	USHORT lw_counter_max = 0;
	USHORT lw_resend_times = get_resend_counter(
		aw_index, aw_type);

	switch(aw_type)
	{
	case DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ:
		lw_counter_max = DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX;
		break;
	case DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE:
		lw_counter_max = DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE_MAX;
		break;
	case DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE:
		lw_counter_max = DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE_MAX;
		break;
	}

	if (lw_resend_times < lw_counter_max)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*--check all RE is IDLE or not*/
USHORT all_RE_idle()
{
	USHORT lw_RE_status = 0;
	USHORT lw_REFW_index = 0;
	USHORT lw_all_RE_idle = 0;

	lw_all_RE_idle = 1;
	for( lw_REFW_index = CMD_NUM0; lw_REFW_index < D_RRH_CPRINO_RE_MAX; lw_REFW_index++ )
	{
		lw_RE_status = get_RE_status(lw_REFW_index);
		if(lw_RE_status != DLD_RESTA_IDLE)
		{
			lw_all_RE_idle = 0;
			break;
		}
	}
	return lw_all_RE_idle;
}
/*--timer - using timer*/
#if(TIMER_CHECK_UT)
static buff_for_ut s_start_timer_buff;
static buff_for_ut s_stop_timer_buff;
#define SAVE_LAST_START_TIMER save_last_start_timer
#define SAVE_LAST_STOP_TIMER  save_last_stop_timer
VOID save_last_start_timer(ULONG al_timer_id)
{
	buff_save(&s_start_timer_buff, al_timer_id);
}
ULONG get_last_start_timer(USHORT aw_backward)
{
	return buff_get(&s_start_timer_buff, aw_backward);
}
VOID save_last_stop_timer(ULONG al_timer_id)
{
	buff_save(&s_stop_timer_buff, al_timer_id);
}
ULONG get_last_stop_timer(USHORT aw_backward)
{
	return buff_get(&s_stop_timer_buff, aw_backward);
}
#else
#define SAVE_LAST_START_TIMER(...)
#define SAVE_LAST_STOP_TIMER(...)
#endif /*TIMER_CHECK_UT*/
USHORT m_dl_StartTimer(ULONG al_timer_id, USHORT aw_CPRI_no)
{
	UINT ll_time_value = 0;
	UINT ll_time_id = 0;
	INT  ll_error_code = 0;
	bpf_ru_hrtm_notify_type_t	notify_type = CMD_TIMMD_SINGLE;

	SAVE_LAST_START_TIMER(al_timer_id);

	switch(al_timer_id) {
		/* ファームウェアファイル報告確認タイマ */
		case CMD_TIMID_FIRMFILE_REP:
			ll_time_value = CMD_TIMVAL_FIRMFILE_REP;
			ll_time_id = CMD_TIMID_FIRMFILE_REP + aw_CPRI_no - 1;
			break;
		/* ファームウェアファイル情報報告確認タイマ */
		case CMD_TIMID_FIRMFILE_INF:
			ll_time_value = CMD_TIMVAL_FIRMFILE_INF_REP;
			ll_time_id = CMD_TIMID_FIRMFILE_INF + aw_CPRI_no - 1;
			break;
		/* ファイルデータ送信間隔タイマ */
		case CMD_TIMID_FILEDATSND:
			ll_time_value = CMD_TIMVAL_FILEDATSND_INTERVAL;
			ll_time_id = CMD_TIMID_FILEDATSND + aw_CPRI_no - 1;
			notify_type = CMD_TIMMD_REPEAT;
			break;
		/* ファームウェア送信完了確認タイマ */
		case CMD_TIMID_FIRMFILE_SNDFIN:
			ll_time_value = CMD_TIMVAL_FIRMFILE_SNDFIN;
			ll_time_id = CMD_TIMID_FIRMFILE_SNDFIN + aw_CPRI_no - 1;
			break;
	}

	ll_error_code = cm_TStat(
		ll_time_id,
		ll_time_value,
		notify_type,
		CMD_TSKIF_TIMOUTNTC,
		CMD_TSKID_DLM,
		&ll_error_code);
	return ll_error_code;
}
USHORT m_dl_StopTimer(ULONG al_timer_id, USHORT aw_CPRI_no)
{
	UINT ll_return = 0;
	INT  ll_error_code = 0;
	UINT ll_time_id = al_timer_id + aw_CPRI_no - 1;

	SAVE_LAST_STOP_TIMER(al_timer_id);
	ll_return = cm_TStop(ll_time_id, &ll_error_code);

	return ll_return;
}
/*-------(3)FHM保持REファイル情報------------*/
VOID init_all_FHMRE()
{
	memset(&gt_dlw_mnt_reinf, 0, sizeof(gt_dlw_mnt_reinf));
}
USHORT get_FHMRE_maker_id(USHORT aw_index)
{
	return FHMRE_get_maker_id(&gt_dlw_mnt_reinf[aw_index]);
}
USHORT get_FHMRE_major_version(USHORT aw_index)
{
	return FHMRE_get_major_version(&gt_dlw_mnt_reinf[aw_index]);
}
USHORT get_FHMRE_minor_version(USHORT aw_index)
{
	return FHMRE_get_minor_version(&gt_dlw_mnt_reinf[aw_index]);
}
USHORT get_maker_id_RE_connection_exist(USHORT aw_index)
{
	return FHMRE_get_maker_id_RE_connection_exist(&gt_dlw_mnt_reinf[aw_index]);
}
UCHAR*  get_FHMRE_file_name(USHORT aw_index)
{
	return FHMRE_get_file_name(&gt_dlw_mnt_reinf[aw_index]);
}
ULONG get_FHMRE_file_size(USHORT aw_index)
{
	return FHMRE_get_file_size(&gt_dlw_mnt_reinf[aw_index]);
}
ULONG get_file_date(USHORT aw_index)
{
	return FHMRE_get_file_date(&gt_dlw_mnt_reinf[aw_index]);
}
ULONG get_number_part(USHORT aw_index)
{
	return FHMRE_get_number_part(&gt_dlw_mnt_reinf[aw_index]);
}
ULONG get_file_data_expand_first_addr(USHORT aw_index)
{
	return FHMRE_get_file_data_expand_first_addr(&gt_dlw_mnt_reinf[aw_index]);
}

VOID set_FHMRE_maker_id(USHORT aw_index, USHORT aw_value)
{
	FHMRE_set_maker_id(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_FHMRE_major_version(USHORT aw_index, USHORT aw_value)
{
	FHMRE_set_major_version(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_FHMRE_minor_version(USHORT aw_index, USHORT aw_value)
{
	FHMRE_set_minor_version(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_maker_id_RE_connection_exist(USHORT aw_index, USHORT aw_value)
{
	FHMRE_set_maker_id_RE_connection_exist(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_FHMRE_file_name(USHORT aw_index, UCHAR* aw_value)
{
	FHMRE_set_file_name(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_FHMRE_file_size(USHORT aw_index, ULONG aw_value)
{
	FHMRE_set_file_size(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
//VOID set_file_date(USHORT aw_index, ULONG aw_value)
//{
//	FHMRE_set_file_date(&gt_dlw_mnt_reinf[aw_index], aw_value);
//}
VOID set_number_part(USHORT aw_index, ULONG aw_value)
{
	FHMRE_set_number_part(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
VOID set_file_data_expand_first_addr(USHORT aw_index, ULONG aw_value)
{
	FHMRE_set_file_data_expand_first_addr(&gt_dlw_mnt_reinf[aw_index], aw_value);
}
/*--index map*/
USHORT convert_CPRI_no_FHMRE_index(USHORT aw_CPRI_no)
{
	//should assert aw_CPRI_no = 1~16
	return aw_CPRI_no;
}

/*!
 *  @brief  FHM保持REファイル格納INDEX取得
 *  @note   
 *  @param  aw_maker_id [in]
 *  @return USHORT
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49対処
 */
USHORT search_FHMRE_maker_id(USHORT aw_maker_id)
{
	USHORT lw_index;
	USHORT lw_count = sizeof(gt_dlw_mnt_reinf)/sizeof(DLT_MTN_REINF);

	for (lw_index = 1; lw_index < lw_count; lw_index++)
	{
		if (gt_dlw_mnt_reinf[lw_index].mk_jdg == aw_maker_id)
		{
			return lw_index;
		}
	}

	return 0xFFFF;
}


/*-------common funtions-----------------------*/

#if(SEND_MSG_UT)
static buff_for_ut s_msg_buff;
USHORT get_last_msg_code(USHORT aw_backward)
{
	return buff_get(&s_msg_buff, aw_backward);
}
VOID save_last_msg_code(USHORT aw_code)
{
	buff_save(&s_msg_buff, aw_code);
}
#define SAVE_LAST_MSG_CODE save_last_msg_code
#else
#define SAVE_LAST_MSG_CODE(...)
#endif /*SEND_MSG_UT*/

VOID RE_forwarding_notice(USHORT aw_code, VOID* at_msg_p)
{
	//should ensure this is FORWARD msg
	send_msg_to_RE(aw_code, at_msg_p);
}

/*!
 *  @brief  REへ メッセー送信
 *  @note   
 *  @param  aw_code [in]
 *  @param  at_msg_p [in] msg buffer
 *  @return INT
 *  @date   2015/08/22 FPT)Khiem Create
 *  @date   2015/10/09 FPT)Yen update メッセージサイズ
 *  @date   2015/10/26 TDIPS)sasaki update IT2問処No.141対処
 */

INT send_msg_to_RE(USHORT aw_code, VOID* at_msg_p)
{
	USHORT 	lw_msg_size = 0;
	INT 	ll_ret = 0;

	SAVE_LAST_MSG_CODE(aw_code);

	switch(aw_code)
	{
	case MSG_FW_FILE_REPORT_REQ:
		//ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)
		//0x2110
		lw_msg_size = sizeof(CMT_TSKIF_CPRIRCV_FIRMFILEREQ);
		break;
	case MSG_FILE_INFO_REPORT_NOTICE:
		//ファイル情報報告通知
		lw_msg_size = sizeof(CMT_TSKIF_CPRIRCV_FILEINFREQ);
		break;
	case MSG_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE:
		//ファイルデータ送信完了報告通知(CMT_CPRIF_FILSNDFINNTC)
		//CPRI信号受信通知(ファイルデータ送信完了報告通知)
		lw_msg_size = sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC);
		break;
	case MSG_RE_FILE_UPDATE_RESPOND:
		//REファイル更新応答
		lw_msg_size = sizeof(CMT_TSKIF_REFILNEWRES);
		break;
	case MSG_FILE_DATA_SEND:
		lw_msg_size = sizeof(CMT_TSKIF_CPRIRCV_FILEDATSNDINT);
		break;
	default:
		//should not fall to this case
		break;
	}
	/* (0x4001)RE Forwarding Notice(運用中ファームウェアファイル報告要求) */
	ll_ret = l3_com_sendMsg(
		CMD_TSKID_DLM,
		D_RRH_PROCQUE_RE,
		0,
		at_msg_p,
		lw_msg_size
		);

	return ll_ret;
}
USHORT is_updating_link_exist()
{
	USHORT lw_updating_link_info_bitvec = 
		DLM_get_updating_link_info(&gt_dlw_dlm_tbl, CMD_NUM_0xFF);
	return (lw_updating_link_info_bitvec != 0);
}
//set DLM updating link info = 0x00
VOID reset_updating_link_info()
{
	DLM_set_updating_link_info(&gt_dlw_dlm_tbl, CMD_NUM_0xFF, 0);
	cm_Assert( CMD_ASL_USELOW,
		0 == DLM_get_updating_link_info(&gt_dlw_dlm_tbl, CMD_NUM_0xFF),
		"[m_dl] init updating link info fail" );
}

#if(CALL_STACK_UT)
static buff_for_ut s_call_buff;
typedef union
{
	ULONG value;
	void* pointer;
} value_8_bytes;
void save_last_call(void* al_func_p)
{
	value_8_bytes ll_func_addr;
	ll_func_addr.pointer = al_func_p;
	buff_save(&s_call_buff, ll_func_addr.value);
}
void* get_last_call(USHORT aw_backward)
{
	value_8_bytes ll_func_addr;
	ll_func_addr.value = buff_get(&s_call_buff, aw_backward);
	return ll_func_addr.pointer;
}
#endif
/** @} */
