/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_ram.c
 *  @brief   declare file for MKM TRA table
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
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
 *  @brief     declare file for MKM TRA table 
 *  @note     declare file for MKM TRA table 
 *  @param  None		
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/


UINT mkw_stateno ;				/* State number		*/

UINT mkw_eventno ;					/* Event number		*/

USHORT mkw_needfrmsg ;			/* Need/needn't free message buffer	*/

UINT mkw_REMTConnect;			/*RE-MT connection manage table*/

CMT_TSKIF_TCPSNDREQ *mkw_mdatsav_p;	/* Monitor data saving buffer address	*/

MKT_DLMNG_TBL	mkw_dlmng_tbl;		/* ダウンロード管理テーブル 				*/

UINT mkw_timmng_tbl[MKD_TIMER_NUM];				/* ファイルダウンロード確認タイマテーブル	*/

UINT mkw_linkhandle;						/*TCP link handle		*/

#ifndef OPT_RRH_ZYNQ_REC
MNT_COM_TSKIF_CPRISTANTC mkw_cpristat;        /* CPRI status	*/
#else
MNT_COM_TSKIF_CPRISTANTC_REC mkw_cpristat;        /* CPRI status	*/
#endif
MNT_COM_TSKIF_CPRISTANTC_REC mkw_cpristatM[3];        /* CPRI status	*/
USHORT mkw_cardStat[5];                      /* card status A0~A9	*/

CMT_CARDCNTTBL mkw_cardcnt_tbl;              /* card control table  */

INT	   mkw_handle;

MKT_CLIENTINFO mkw_tblClientInfo[MKD_MX_TCPNO];

/* @} */
