/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_REMTRelease.c
 *  @brief  RE-MT connect stop
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/12/03 FFCS)zhaodx modify for zynq
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

/********************************************************************************************************************/
/**
 *  @brief     RE-MT connect stop	 
 *  @note     RE-MT connect stop	 
 *  @param  inf_p		[in]	Pointer of  buffer
 *  @param  link_flag	[in]	link flag
 *  @param  linkhandle  [in]    TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/12/03 FFCS)zhaodx modify for zynq
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_REMTRelease(UINT *inf_p, UINT link_flag, INT linkhandle)
{
	UINT					a_ret_buf = CMD_RES_OK;			/* Return value of buffer request	*/
	UINT 					a_ngcode = CMD_RES_OK;			/* Ng code 							*/
	UINT					a_okng = CMD_OK;				/* Ok/ng flag						*/
	INT                     errcd;
	MKT_TCPIF_REMTCONSTPRSP	  	*a_tcp_remtstprsp_p = NULL;	/* TCPIF RE-MT stop response pointer	*/
	CMT_TSKIF_TCPSNDREQ 	*a_sndmsg_p			= NULL;		/* Pointer of TCP data send req IF	*/

	
	/* RE MT is disconnected */
	mkw_REMTConnect = CMD_CONN_N;
	
	/* Link flag judge */
	if (link_flag ==  MKD_TCPDATA )
	{
		/************************************************/
		/* 	Clear TRA file download used status 				*/
		/************************************************/
		/* When TRA file download timer is running	*/
		if(mkw_timmng_tbl[CMD_NUM0] != CMD_NUM0)
		{
			/* Stop download timer			*/
			a_ret_buf = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
			/* Stop timer error,Assert 		*/
			if(a_ret_buf != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "file download Timer Stop NG" );
			}
			/* Set download timer flag 		*/
			mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
		}
		
		/* Stop TRA write timer */
		if(mkw_timmng_tbl[CMD_NUM1] != CMD_NUM0)
		{
			/* Stop TRA write timer */
			a_ret_buf = cm_TStop(mkw_timmng_tbl[CMD_NUM1], &errcd);
			/* Stop timer error,Assert 		*/
			if(a_ret_buf != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM1], "TRA write timer Stop NG" );
			}
			/* Set download timer flag 		*/
			mkw_timmng_tbl[CMD_NUM1] = CMD_NUM0;
		}
		/* Clear status table */
		if(cmw_trafile_inf_tbl.en_flg == CMD_TDL_ON) 
		{
			
			/* ファイル情報報告テーブルをクリアする */
			cm_MemClr( &cmw_trafile_inf_tbl, sizeof( cmw_trafile_inf_tbl ));
			
		}
					
		/* ダウンロード管理テーブルをクリアする */
		cm_MemClr( &mkw_dlmng_tbl, sizeof( mkw_dlmng_tbl ));
			
		/* タイマ管理テーブルをクリアする */
		cm_MemClr( mkw_timmng_tbl, sizeof( mkw_timmng_tbl ));

		/* RE-MT stop response buffer request */
		a_ret_buf = cm_BReq(CMD_BUFCA_MON, sizeof(MKT_TCPIF_REMTCONSTPRSP), (VOID **)&a_tcp_remtstprsp_p);
		/* Return value judge */
		if(a_ret_buf != CMD_RES_OK)
		{
			/* Buffer request error,abort */
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_OptRelease", 
			"Buffer request error",
			a_ret_buf, 
			sizeof(MKT_TCPIF_REMTCONSTPRSP), 
			CMD_NUM0);
		}
		/* Message body part */
		a_tcp_remtstprsp_p->constprsp.Result = a_okng;
		a_tcp_remtstprsp_p->constprsp.NGCode = a_ngcode;
		/* head data byte */
		a_tcp_remtstprsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(MKT_REMTCONSTPRSP);
		/* Set TCP data for transmission */
		m_mk_cm_SetSndMsg(MKD_REMTCONSTPRSP, a_tcp_remtstprsp_p, &a_sndmsg_p);
		a_sndmsg_p->linkhandle = linkhandle;

		/* Send TCP data tx request (CPRI data send request) to LAN tx task	*/
		a_ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, a_sndmsg_p);
		if(a_ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret_buf, "cm_Enter NG" );
		}
	}
	else
	{
		/* Phase 3*/
	}
	return;
}
/* @} */
