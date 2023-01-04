/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_FileDlStrReq.c
 *  @brief   File download start notification
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/29 FFCS)zhaodx modify for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_File.h"
#include "BPF_RU_HRTM.h"

/********************************************************************************************************************/
/**
 *  @brief     File download start notification
 *  @note     File download start notification
 *  @param  a_inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_FileDl_StrReq(UINT * a_inf_p )
{
	UINT 				a_type 			= CMD_NUM0;		/* File type 				*/
	UINT 				a_size 			= CMD_NUM0;		/* File size 				*/
	UINT 				a_functionid 	= CMD_NUM0;		/* Function ID 				*/
	MKT_REMTFLDLSTRREQ	*a_DlStart_p	= NULL;				/* Download start pointer	*/
	UINT 				a_timerid		= CMD_TIMID_REFLDL;		/* Timer ID 				*/
	USHORT 				a_timeval		= CMD_NUM0;		/* Timer interval 			*/
	UINT 				a_ret			= CMD_RES_OK;		/* Return result  			*/
	INT                 errcd;
	
	/* RE MT is disconnected,assert */
	if(mkw_REMTConnect == CMD_CONN_N)
	{
		/* RE not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE Client is not conncted");
		return;
	}
	
	/********************/
	/* 	Get parameter 	*/
	/********************/
	/* Get data part */
	a_DlStart_p = (MKT_REMTFLDLSTRREQ *)a_inf_p;
	/* Get file type */
	a_type = a_DlStart_p->FileType;
	/* Get file size */
	a_size = a_DlStart_p->FileSize;
	/* Get function id */
	a_functionid = a_DlStart_p->FunctionID;
	
	/********************/
	/* 	Parameter check */
	/********************/
	/* Downloading check */
	if(cmw_trafile_inf_tbl.en_flg == CMD_TDL_ON)
	{
		/* Downloading */
		m_mk_FileDlStrRsp(a_functionid, MKD_RES_DOWNLOAD_NG);
		return;
	}	

	/* When file type out of range 	*/
	if(a_type != MKD_FPGA)
	{
		/* ファイルダウンロード開始応答 */
		m_mk_FileDlStrRsp(a_functionid, MKD_RES_KINDERR);
		return;

	}

	/* When total size out of rang */
	if(( a_size > CMD_TRAFILE_MAX) || (a_size == CMD_NUM0))
	{
		/* ファイルダウンロード開始応答 */
		m_mk_FileDlStrRsp(a_functionid, MKD_RES_FILESIZEERR);
		return;
	}


	/********************/
	/* テーブル設定処理 */
	/********************/
	/*==========================*/
	/* ファイル情報報告テーブル */
	/*==========================*/
	/* Total file size set */
	mkw_dlmng_tbl.total_size = a_size;
	/* ファイル情報報告テーブルのENフラグ設定 */
	cmw_trafile_inf_tbl.en_flg = CMD_TDL_ON;
	/* The file type in file information table set */	
	cmw_trafile_inf_tbl.file_type = a_type;
	/* Table download target set */
	cmw_trafile_inf_tbl.dl_target = a_DlStart_p->DLTarget;
	/* When the type is FPGA file */
	if(a_type == MKD_FPGA)
	{
		/* トータルデータサイズが10228(byte)で割り切れるとき */
		if ((a_size % MKD_PACKDAT_MAX) == CMD_NUM0)
		{
			/* 分割数を算出する */
			cmw_trafile_inf_tbl.div_no = a_size / MKD_PACKDAT_MAX;
		}
		/* トータルデータサイズが10228(byte)で割り切れないとき */
		else
		{
			/* 分割数を算出する */
			cmw_trafile_inf_tbl.div_no = (a_size / MKD_PACKDAT_MAX) + CMD_NUM1;
		}	
		/* Timer interval set */
		a_timeval = MKD_TIMER_FPGA;
	}
	
	/* 正常ファイルダウンロード開始応答 */
	m_mk_FileDlStrRsp(a_functionid, CMD_NML);

	/* タイマ起動処理を起動 */
	if(mkw_timmng_tbl[CMD_NUM0] == CMD_NUM0)
	{
		/* Timer flag set */
		mkw_timmng_tbl[CMD_NUM0] = a_timerid;
		/* start download timer */
		a_ret = cm_TStat(a_timerid, TIMVAL_GET(a_timeval), CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_MKM,&errcd);
		/* When start timer NG */
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, a_timerid, "File download Timer Start NG" );
		}
	}
	
	return;	
}

/* @} */
