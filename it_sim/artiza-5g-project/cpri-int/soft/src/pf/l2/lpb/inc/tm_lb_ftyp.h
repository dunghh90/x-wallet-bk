/*!
 * @skip    $Id$
 * @file    tm_lb_ext.h
 * @brief   LAPB 共通関数プロトタイプ定義
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/

/********************************************************************************************************/

#ifndef    TM_LB_FTYP
#define    TM_LB_FTYP

#include "tm_lb_typ.h"

extern	void	tm_lb_Main(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	UINT tm_lb_Anlz(UINT a_event);

extern	void	tm_lb_0_m0e0s0p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_0_m0e1s1p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_0_m0e2s1p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_0_m0e1s2p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_0_m0e2s2p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_0_m0e3s2p(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	lblap_estlnk(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_transp(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_transf(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_conf1(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_conf2(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_retrs(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vanr1(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vanr2(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_frmrer(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vrinc(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vanrvs(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_chkctl(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkdisc(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkua(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkdm(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkrej(struct LBT_MNGTBL * s_lblap_p);

extern	void lblap_statis(USHORT link_num , UINT cnt_area);
//#define lblap_statis( link_num ,  cnt_area) {}

extern  void lblap_statis_for_l3(USHORT linkno, USHORT system_type, UINT cnt_area, UINT rsp_code);
//#define lblap_statis_for_l3( linkno,  system_type,  cnt_area,  rsp_code) {}

//extern	void lblap_stats1(USHORT link_num,UINT cnt_area,USHORT add_val);
#define	 lblap_stats1( link_num, cnt_area, add_val) {}

extern	void lblap_ablog(USHORT link_num,UINT cnt_area);

extern	void	lblap_l2datpr(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_clrcnd(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_stopt2(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_clrval(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_disciq(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_sndu(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_snds(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_sndi(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vsvak(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mki(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_vsinc(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mksabm(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkrr(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkrnr(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_mkfrmr(struct LBT_MNGTBL * s_lblap_p);

extern	void	lblap_freeak(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_Disc10kBf(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_checknr(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_StpAllTmr(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_DstrbtL3Msg(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_Nop(UINT a_source,UINT a_event,UINT * a_inf_p);

extern	void	tm_lb_Abort(UINT a_source,UINT a_event,UINT * a_inf_p);

/*new added function*/

extern	void tm_lb_NrError(struct LBT_MNGTBL * s_lblap_p);

extern	UINT tm_lb_trans(UINT a_event, struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e0s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e2s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e18s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e25s0p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e6s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e8s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e22s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e23s1p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e6s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e7s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e8s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e9s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e12s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e17s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e18s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e22s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e25s2p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e6s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e8s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e12s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e17s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e22s3p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e0s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e1s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e2s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e3s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e4s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e5s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e6s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e7s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e8s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e9s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e10s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e11s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e12s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e13s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e14s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e15s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e16s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e17s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e18s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e19s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e20s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e21s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e22s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e25s4p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e1s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e2s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e3s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e4s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e12s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e14s5p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e0s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e1s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e2s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_m1e24s8p(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern	void	tm_lb_1_Nop(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern  void	tm_lb_1_RelNG(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern  void	tm_lb_1_SetupNG(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p);

extern  void tm_lb_subrunhis(struct LBT_MNGTBL * s_lblap_p);

extern	void	tm_lb_RunHisTmrInfGet(UCHAR *link_sta_p, UCHAR *in_cpri_no_p, 
												UCHAR *inter_event_no_p, UINT  a_source,UINT * a_inf_p);

extern	void	tm_lb_RunHisMngInfGet(UCHAR *link_sta_p, UCHAR *in_cpri_no_p, 
													UCHAR *inter_event_no_p, UINT a_event, 
													UINT *  a_inf_p);

extern	void	tm_lb_RunHisSet(UINT sndrcv_flg, UINT a_taskid,UINT a_event,UINT * a_inf_p);

extern	UCHAR	tm_lb_RunHisInterEvGet(UINT length, UCHAR addr, UCHAR ctl,
													UCHAR own_adr,UCHAR per_adr,UINT n1);

/*extern void tm_lb_subrunhis(struct LBT_MNGTBL * s_lblap_p);*/

extern void tm_lb_little_to_big(UINT * a_inf_p);

extern void tm_lb_big_to_little(UINT * a_inf_p);

//extern void lblap_tx_rv_log(UINT a_event, UINT inter_event_no, UINT * a_inf_p, USHORT link_num);
#define lblap_tx_rv_log( a_event,  inter_event_no, a_inf_p,  link_num) {}

extern void lblap_init_log();

extern void tm_lb_rell2link_rsp(USHORT linkno, UINT result, CHAR* errMsg);

extern void tm_lb_setupl2link_rsp(USHORT linkno, UINT result, CHAR* errMsg);

extern void tm_lb_l2state_sub_ntc(USHORT linkno, INT lnkState, const CHAR* astlog);

extern void tm_lb_l2state_log_begin(USHORT linkno, int event);

extern void tm_lb_l2state_log_end(USHORT linkno);

extern void tm_lb_set_stateF(USHORT linkno, INT flag);

extern void tm_lb_set_L2Error(USHORT linkno, INT flag);

extern struct LBT_MNGTBL* get_mngtbl(UINT * a_inf_p);

#endif/*LB_FTYP*/

