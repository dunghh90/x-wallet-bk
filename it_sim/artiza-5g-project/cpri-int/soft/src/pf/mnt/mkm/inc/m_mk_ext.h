/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_mk_ext.h
 *  @brief	Maker MT manage task external common table declaration  
 *  @date   2008/07/29 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/



#ifndef M_MK_EXT
#define M_MK_EXT

/*    ROM data    */
/*  Main matrix table               */
extern VOID (* const mkr_mtrxtbl[MKD_MX_STATE][MKD_MX_EVENT])(UINT *inf_p);

/*    RAM data    */
extern UINT mkw_stateno;				/* State number		*/

extern UINT mkw_eventno;				/* Event number		*/

extern USHORT mkw_needfrmsg;			/* Need/needn't free message buffer	*/

extern MKT_CLIENTINFO mkw_tblClientInfo[MKD_MX_TCPNO];	/* Client infomation	*/

extern UINT mkw_REMTConnect;			/*RE MT接続状態管理テーブル*/

extern CMT_TSKIF_TCPSNDREQ *mkw_mdatsav_p;	/* Monitor data saving buffer address	*/

extern MKT_DLMNG_TBL	mkw_dlmng_tbl;					/* ダウンロード管理テーブル */

extern UINT mkw_timmng_tbl[MKD_TIMER_NUM];			/* ファイルダウンロード確認タイマテーブル  */

extern UINT mkw_trafiledl_flag;				/* tra file download flag */

extern UINT mkw_linkhandle;						/*TCP link handle		*/

#ifndef OPT_RRH_ZYNQ_REC
extern MNT_COM_TSKIF_CPRISTANTC mkw_cpristat;        /* CPRI status	*/
#else
extern MNT_COM_TSKIF_CPRISTANTC_REC mkw_cpristat;        /* CPRI status	*/
#endif
extern MNT_COM_TSKIF_CPRISTANTC_REC mkw_cpristatM[3];
extern USHORT mkw_cardStat[5];
extern CMT_CARDCNTTBL mkw_cardcnt_tbl;              /* card control table  */
extern INT mkw_handle;

#endif	/*M_MK_EXT*/
