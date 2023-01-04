/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_ext.h
 *  @brief  RE supervision external header file defination
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2011/11/24 COMAS)Uchida  M-S3G-eNBSYS-02539対応
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_RSV_EXT
#define		F_RSV_EXT

/** @addtogroup RRH_PF_RSV
* @{ */

extern USHORT                   tra_sv_newsta[D_RSV_SVTBL_NUM];
extern USHORT                   tra_sv_oldsta[D_RSV_SVTBL_NUM];
extern USHORT                   f_rsvw_first_act_flg;
extern USHORT                   f_rsvw_restart_both_alm_flg;
extern const USHORT             f_rsvr_tra_msk_tbl[D_RSV_SVTBL_NUM];
extern UINT                     f_rsvw_almCount[D_RSV_ALM_NUM];
extern UINT                     f_rsvw_almCyclCount[D_RSV_ALM_NUM];
extern UINT                     f_rsvw_cardStaChgNtc_QID[D_SYS_NUM4];
extern UINT                     f_rsvw_thrdstate;
extern UINT                     f_rsvw_errctl_tbl;
extern const T_RSV_ALMINFO      f_rsvr_almInfo[D_RSV_ALM_NUM];
extern const T_RSV_LEDINFO      f_rsvr_led1Info[3];
extern const T_RSV_LEDINFO      f_rsvr_led2Info[3];
extern const T_RSV_LEDINFO      f_rsvr_led3Info[3];
extern const T_RSV_LEDINFO      f_rsvr_ledCpriActInfo[3];
extern const T_RSV_LEDINFO      f_rsvr_ledCpriDwnInfo[3];

/* @} */
#endif	/* M_RS_EXT */
