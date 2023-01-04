/*!
 * @skip    $Id$
 * @file    m_lb_ext.h
 * @brief   LAPB 共通外部参照ヘッダファイル
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/
#ifndef    M_LB_EXT
#define    M_LB_EXT/*!< */


extern	void	(* const lbr_0_m0[LBD_MX_STATE][LBD_MX_EVENT])
						(UINT a_source,UINT a_event, UINT *a_inf_p);


extern	struct LBT_MNGTBL	lbw_mngtbl[D_RRH_CPRINO_NUM];

extern UINT lbw_L3DstrbtTbl_cur[D_RRH_CPRINO_NUM];

extern UINT	lbw_L3DstrbtTbl[D_RRH_CPRINO_NUM][LBD_L3MSGNUM][2];

extern UINT lbw_L2StsNotiTbl_cur[D_RRH_CPRINO_NUM];

extern UINT	lbw_L2StsNotiDstrbtTbl[D_RRH_CPRINO_NUM][LBD_SUBTASKNUM];

extern UINT  lbw_lnk_set_respQId[D_RRH_CPRINO_NUM];

extern UINT  lbw_lnk_rel_respQId[D_RRH_CPRINO_NUM];

extern UINT		sts_cnt[D_RRH_CPRINO_NUM][LBD_L2_STSNUM];

extern UINT		ab_cnt[D_RRH_CPRINO_NUM][LBD_L2_ABNUM];

extern struct	LBT_STSLOG_DAT		lbw_stslog[D_RRH_CPRINO_NUM][CMD_NUM2];	/*L2 statistics  log area*/

extern UINT		lbw_stscnt[D_RRH_CPRINO_NUM];								/*L2 statistics log area count*/

extern struct	LBT_ABTLOG_DAT		lbw_ablog[D_RRH_CPRINO_NUM][CMD_NUM10];	/*L2 abort log area*/

extern UINT		lbw_abcnt[D_RRH_CPRINO_NUM];								/*L2 abort log area count*/

extern VOID  *lbw_mngTblForLog_off[D_RRH_CPRINO_NUM];
extern UINT  lbw_mngTblForLog_size[D_RRH_CPRINO_NUM];


#endif/*LB_EXT*/
