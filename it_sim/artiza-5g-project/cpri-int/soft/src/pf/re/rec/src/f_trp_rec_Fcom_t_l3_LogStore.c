/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_LogStore.c
 * @brief  L3メッセージ(要求/応答)送受信ログ格納
 * @date   2010/07/24 FJT)Tokunaga create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  L3メッセージ(要求/応答)送受信ログ格納
 * @note   L3メッセージ(要求/応答)送受信ログ格納
 * @param  log_kind	[in]  格納ログ種別
 * @param  cpr_no	[in]  CPRI番号
 * @param  msg_kind	[in]  要求 or 応答
 * @return 終了コード
 * @return None
 * @date   2010/07/24 FJT)Tokunaga create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_l3_LogStore( USHORT log_kind, USHORT cpr_no, USHORT msg_kind )
{
#if 0
    T_REC_BCDTIM       bcdtim;                                   /* BCD時刻 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* カウンタUp	*/
	if( msg_kind == D_REC_MSGKIND_REQ ){
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Count ++;
	}
	else if( msg_kind == D_REC_MSGKIND_RSP ){
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Count ++;
	}

	/* 時刻情報格納	*/
    f_trp_rec_Fcom_t_bcdtim_get(&bcdtim);
	if( msg_kind == D_REC_MSGKIND_REQ ){
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.year	= bcdtim.year;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.month	= bcdtim.month;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.day		= bcdtim.day;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.hour	= bcdtim.hour;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.min		= bcdtim.min;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.sec		= bcdtim.sec;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.msec1	= bcdtim.msec1;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Req_Time.msec2	= bcdtim.msec2;
	}
	else if( msg_kind == D_REC_MSGKIND_RSP ){
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.year	= bcdtim.year;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.month	= bcdtim.month;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.day		= bcdtim.day;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.hour	= bcdtim.hour;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.min		= bcdtim.min;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.sec		= bcdtim.sec;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.msec1	= bcdtim.msec1;
		f_trp_rec_Wcom_ReqRspLog_tbl[log_kind][cpr_no - 1].Rsp_Time.msec2	= bcdtim.msec2;
	}
#endif
    return;
}


/****************************************************************************/
/*!
 * @brief  L3メッセージ(要求/応答)送受信ログクリア
 * @note   L3メッセージ(要求/応答)送受信ログクリア
 * @param  cpr_no	[in]  CPRI番号
 * 
 * 
 * @return 終了コード
 * @return None
 * @date   2010/07/24 FJT)Tokunaga create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_l3_LogClr( USHORT cpr_no )
{
	USHORT	i;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* カウンタクリア	*/
	for(i=0;i<D_REC_LOGKIND_MAX;i++){
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Count = 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Count = 0x0;
	}

	/* 時刻情報クリア	*/
	for(i=0;i<D_REC_LOGKIND_MAX;i++){
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.year	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.month	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.day	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.hour	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.min	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.sec	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.msec1	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Req_Time.msec2	= 0x0;

		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.year	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.month	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.day	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.hour	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.min	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.sec	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.msec1	= 0x0;
		f_trp_rec_Wcom_ReqRspLog_tbl[i][cpr_no - 1].Rsp_Time.msec2	= 0x0;
	}

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
