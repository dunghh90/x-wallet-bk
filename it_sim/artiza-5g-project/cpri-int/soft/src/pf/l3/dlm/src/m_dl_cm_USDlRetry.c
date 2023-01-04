/*!
 * @skip   $Id$
 * @file   m_dl_cm_USDlRetry.c
 * @brief  retry download in USE state.
 * @date   2009/04/18 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/07/06 FFCS)Tangj modify for Error control, remove the error control
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/28 TDIPS)sasaki Update
 * @date   2015/10/29 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"			/* MDIF common head file			*/
#include "m_dl_header.h"			/* download management task head file */

/*!
 *  @brief  the function for retry download.
 *  @note   Give up the receiving data,and set the timer again.
 *          This function is processed as follows.
 *          1) Clear the data saving table,data deploy table,download management table,timer management table
 *          2) Set the timer
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01931
 *  @date   2009/04/14 FFCS)Wuh Create.
 *  @date   2009/07/06 FFCS)Tangj modify for Error control, remove the error control
 *  @date   2015/10/02 TDIPS)sasaki Update 再ダウンロード準備処理を修正
 *  @date   2015/10/28 TDIPS)sasaki Update IT2問処No.151対処
 *  @date   2015/10/29 TDIPS)sasaki M-RRU-ZSYS-01931 Update IT2問処No.154対処
 */
VOID m_dl_cm_USDlRetry( VOID )
{
	UINT	a_rtn __attribute__((unused))		= CMD_RES_OK;				/* 戻り値格納変数		*/
	UINT	a_timid	 	= CMD_NUM0;					/* タイマID格納変数		*/
	USHORT	a_tim_val	= CMD_NUM0;					/* タイマ値格納変数		*/
	INT		errcd;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_cm_USDlRetry] ENTER" );		
	
	/********************************************************************/
	/* テーブルクリア処理 												*/
	/* ファイル情報報告テーブル/ダウンロード管理テーブルの				*/
	/* トータルデータサイズ、ファイル名はクリアしない 					*/
	/********************************************************************/
	
	/* ダウンロード管理テーブルの総受信データサイズ、分割番号をクリアする */
	dlw_dlmng_tbl[CMD_SYS_S3G].rcvdata_size	= CMD_NUM0;
	dlw_dlmng_tbl[CMD_SYS_S3G].div_no		= CMD_NUM0;
	
	/* 対REC情報テーブルの使用中タイマを初期化 */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	
	/* ファイルデータ送信完了報告応答のNG要因領域の初期化 */
	cmw_file_inf_tbl[CMD_SYS_S3G].ng_fct = CMD_NUM0;
	
	/******************/
	/* タイマ設定処理 */
	/******************/
	/* タイマID設定 */
	a_timid = CMD_TIMID_USEFIRMWARESND;   /* firmware for download in USE state */
	/* タイマ値設定 */
	a_tim_val = cmw_sys_mng_tbl[CMD_SYS_S3G].firm_sndtimer;
	/* set the flag ON */
	dlw_abdata_flg[CMD_SYS_S3G] = CMD_ON;
	
	/* タイマ管理テーブルのタイマ起動情報にタイマIDを設定 */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_SND_UPDATE;

	gt_dlw_rec_tbl.rcv_div_no = 0;
	gt_dlw_rec_tbl.rcv_data_size = 0;

	/* reset the handler */
	dlw_handle = CMD_NUM0;

	/* REC向けファイルデータ送信完了報告応答結果初期化 */
	gt_dlw_rec_tbl.filedata_trans_rslt = CMD_ALLRE_NG;

	/* タイマ起動処理を起動 */
	a_rtn = cm_TStat( a_timid, TIMVAL_GET( a_tim_val ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,&errcd);

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"[USDlRetry] Timer Start(ID=%d, value=%d)",
						a_timid, a_tim_val);

	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_cm_USDlRetry] RETURN" );		
	return ;
	
}

/* @} */

