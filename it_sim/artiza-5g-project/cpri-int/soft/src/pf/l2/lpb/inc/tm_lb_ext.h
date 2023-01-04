/*!
 * @skip    $Id$
 * @file    tm_lb_ext.h
 * @brief   LAPB 共通外部参照ヘッダファイル
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/
#ifndef    TM_LB_EXT
#define    TM_LB_EXT




extern	UINT		lbw_tskstt;

extern	UINT		lbw_tskevent;

extern	void	(* const lbr_1_m1[LBD_SIGNAL_MX_STATE][LBD_SIGNAL_MX_EVENT])
						(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	UINT		lbw_keepbufflg;

extern	LBT_RUNHIS_TBL	lbw_runhis_tbl[D_RRH_CPRINO_NUM];

extern	UINT	lbw_allrunhis_flg;

extern LPBW_LINKSTAT_LOGTBL lbw_lnkStatLog[D_RRH_CPRINO_NUM];
extern UINT lbw_l1BusyKey[D_RRH_CPRINO_NUM];
#endif/*LB_EXT*/




