/*!
* @skip $Id:$
* @file f_cpr_ram.h
* @brief EXT RAM table extern declaration
* @date 2013/04/26 FFCS)Xut create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

/** @addtogroup RRH_PF_CPR
 *  @{
 */
#ifndef F_CPR_RAM_H
#define F_CPR_RAM_H

extern UINT                 f_cprw_thrdState;
extern UINT                 f_cprw_extEventNo;
extern UINT                 f_cprw_cprStatChgNtc_QID[4];
extern T_RRH_L1STAT_TBL     *f_cprw_l1StatTbl;
extern T_RRH_L1ERR_TBL      *f_cprw_l1ErrTbl;
extern T_RRH_L1LOSLOF_TBL   *f_cprw_l1LosLofTbl;
extern T_CPR_RUNHIS_TBL     f_cprw_runhis_tbl[D_RRH_CPRINO_NUM];
extern UINT                 f_cprw_autonego_complete_flag;
#ifdef OPT_RRH_ZYNQ_REC
extern UINT                 f_cprw_autonego_complete_flag_RE[D_RRH_CPRINO_RE_MAX];
#endif
extern UINT                 f_cprw_bitrate_real;
extern UINT                 f_sync_hfn_count[D_RRH_CPRINO_NUM];
extern UINT					f_sync_hfn_linkup[D_RRH_CPRINO_NUM];
extern UINT                 f_support_sfp[D_RRH_CPRINO_NUM];
extern UINT                 f_support_98g[D_RRH_CPRINO_NUM];

extern UINT	f_cprw_masterLinkUpTimerNum;
extern UINT	f_cprw_masterLinkUpTimerCnt[D_RRH_CPRINO_RE_MAX];
extern T_RRH_SFP_LOG_ALL *f_cprw_shm_sfplog;

extern UINT f_cprw_SlaveCPPVer_HDLCRate;
extern T_RRH_DELAY_INF f_cprw_dly_inf;

#endif
/** @} */
