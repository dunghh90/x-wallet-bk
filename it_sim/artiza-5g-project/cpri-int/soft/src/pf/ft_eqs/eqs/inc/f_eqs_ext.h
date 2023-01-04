/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_ext.h
 *  @brief  5GDU-LLS supervision external header file defination
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 5GDU-LLS AUTO RESET and 3GRF-INF)
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2011/11/24 COMAS)Uchida  M-S3G-eNBSYS-02539対応
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 *  @date   2018/08/21 KCN)Kamimoto 5GDU-LLS Update
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2018
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EQS_EXT
#define		F_EQS_EXT

/** @addtogroup RRH_PF_EQS
* @{ */


/** @name TRA card status change management table */
/* @{ */

/****************************************************************************/
/* TRA状変検出テーブル												*/
/****************************************************************************/
extern T_SYS_EQS_ALM_INFO					f_eqsw_tra_sv_newsta;

/* @} */

extern UINT						f_eqsw_wvstop[D_SYS_NUM8];

/** @name TRA card first install flag */
/* @{ */
/****************************************************************************/
/* TRA card first install flag */
/****************************************************************************/
extern	USHORT					f_rsvw_first_ins_flg;

/* @} */

/** @name TRA card first active flag */
/* @{ */

/****************************************************************************/
/* TRA card first active flag */
/****************************************************************************/
extern	USHORT					f_rsvw_first_act_flg;

/* @} */

/** @name TRA card first active flag */
/* @{ */

/****************************************************************************/
/* TRA card restart both alm flagg */
/****************************************************************************/
extern	USHORT					f_rsvw_restart_both_alm_flg;

/* @} */

/** @name3G RF alarm status flag */
/* @{ */

/****************************************************************************/
/* 3G RF alarm status flag */
/****************************************************************************/
extern	USHORT					f_rsvw_3grf_alm_flg;

/* @} */

/** @name Remote card control flag */
/* @{ */

extern 	USHORT					f_rsvw_remotecnt_flg;
/* @} */


/** @name RSV log management table */
/* @{ */


/****************************************************************************/
/* ステータスリストデータ                                                   */
/****************************************************************************/
extern	T_EQS_STLIST			f_eqsw_stslist;


/****************************************************************************/
/* ログカウンタテーブル														*/
/****************************************************************************/
extern	USHORT				f_rsvw_logcnt;


/****************************************************************************/
/* ログ取得テーブル															*/
/****************************************************************************/
extern	T_RSV_LOG			f_rsvw_log_tbl[D_RSV_LOG_MAX];

/* @} */

/*	FAN ALM 累積カウンタ	*/
extern	USHORT				f_rsvw_FANALMCumulativeCount;

/****************************************************************************/
/* SLP_PA-OFF 解除フラグ													*/
/****************************************************************************/
extern USHORT				f_rsvw_PaStateFlag;

/** @name TRA SV register unuse bit mask  table */
/* @{ */

/****************************************************************************/
/* TRA SVレジスタ未使用ビットマスクテーブル									*/
/****************************************************************************/
extern	const	USHORT		f_rsvr_tra_msk_tbl_rre[D_RSV_SVTBL_NUM];/* FAN情報用にエリアを+1 4TRx */

/* @} */

/** @name 	PLL Alarm code define table	 */
/* @{ */

/****************************************************************************/
/*	PLL Alarm code define table						*/
/****************************************************************************/
extern	const	USHORT		f_rsvr_pllalmcode[D_RSV_PLLALM_NUM] ;

/* @} */

/****************************************************************************/
/*	Alarm counting table													*/
/****************************************************************************/
extern UINT	f_rsvw_almCount[D_RSV_ALM_NUM];

/****************************************************************************/
/*	Alarm Cycle counting table												*/
/****************************************************************************/
extern UINT	f_rsvw_almCyclCount[D_RSV_ALM_NUM];

/****************************************************************************/
/*	Error Cycle counting table													*/
/****************************************************************************/
extern UINT	f_rsvw_errCyclCount[D_RSV_ERR_NUM];
extern UINT	f_rsvw_errCyclCount_200ms[D_RSV_ERR_NUM_200MS];

/****************************************************************************/
/*	Error counting table													*/
/****************************************************************************/
extern UINT	f_rsvw_errCount[D_RSV_ERR_NUM];
extern UINT	f_rsvw_errCount_200ms[D_RSV_ERR_NUM_200MS];

/****************************************************************************/
/*	Error Recovery Cycle counting table													*/
/****************************************************************************/
extern UINT	f_rsvw_errRecCyclCount[D_RSV_ERR_NUM];
extern UINT	f_rsvw_errRecCyclCount_200ms[D_RSV_ERR_NUM_200MS];

/****************************************************************************/
/*	Error Recovery counting table													*/
/****************************************************************************/
extern UINT	f_rsvw_errRecCount[D_RSV_ERR_NUM];
extern UINT	f_rsvw_errRecCount_200ms[D_RSV_ERR_NUM_200MS];

/** @name Response QID of Card State Change Notification */
/* @{ */
extern UINT		f_rsvw_cardStaChgNtc_QID[D_SYS_NUM4];
/* @} */

/** @name RSV thread state */
/* @{ */
extern UINT		f_rsvw_thrdstate;
/* @} */

extern UINT		f_rsvw_errctl_tbl;

extern const	T_RSV_ALMINFO f_rsvr_almInfo[D_RSV_ALM_NUM];

extern const	T_RSV_ALMINFO f_rsvr_almInfo_rre[D_RSV_ALM_NUM_RRE];

extern const	T_RSV_ERRINFO f_rsvr_errInfo[D_RSV_ERR_NUM];
extern const	T_RSV_ERRINFO f_rsvr_errInfo_200ms[D_RSV_ERR_NUM_200MS];

extern const	T_RSV_LEDINFO f_rsvr_led1Info[D_SYS_NUM6];

extern const	T_RSV_LEDINFO f_rsvr_led2Info[D_SYS_NUM3];

extern UINT f_rsvw_portStaChgNtc_QID;

/****************************************************************************/
/*	Port-IN state management table											*/
/****************************************************************************/
extern T_RSV_PORTINSTA	f_rsvw_portInSta_tbl;

/****************************************************************************/
/*	Card Control management table											*/
/****************************************************************************/
extern T_RSV_CARDCNT		f_rsvw_cardcnt_tbl;

/****************************************************************************/
/*	TX system number											*/
/****************************************************************************/
extern USHORT			f_rsvw_tx_sys_num;

extern UINT				f_rsvw_portout;

/** @name FAN control time cycle */
/* @{ */
extern USHORT	f_rsvw_fan_cycle; /* 4TRx-RRE */
/* @} */

/** @name FAN state */
/* @{ */
extern USHORT	f_rsvw_fan_state; /* 4TRx-RRE */
/* @} */

/** @name FAN control state */
/* @{ */
extern USHORT	f_rsvw_fan_control_state; /* 4TRx-RRE */
/* @} */

/** @name fan temp threshold */
/* @{ */
extern int		f_rsvw_fantemp_threshold[4][2]; /* 4TRx-RRE */
/* @} */

/* ログカウンタテーブル */
extern USHORT					f_eqsw_logcnt;

/* ログ取得テーブル */
extern T_RSV_LOG				f_eqsw_log_tbl[D_RSV_LOG_MAX];

/* アラームカウントテーブル */
extern UINT	f_eqsw_almCount[D_RSV_ALM_NUM];

/* Port-IN 状態テーブル */
extern T_RSV_PORTINSTA		f_eqsw_portInSta_tbl;

/* TRA カードfirst install フラグ */
extern USHORT					f_eqsw_first_ins_flg;

/* TRA card first active flag */
extern USHORT					f_eqsw_first_act_flg;

/* TRA card restart both alm flag */
extern USHORT					f_eqsw_restart_both_alm_flg;

/* SLP_PA-OFF 解除フラグ */
extern USHORT					f_eqsw_PaStateFlag;

extern UINT		f_eqsw_errctl_tbl;

/* TX sys number */
extern USHORT					f_eqsw_tx_sys_num;

/* Port out  */
extern USHORT					f_eqsw_fan_cycle;

/* FAN state */
extern USHORT					f_eqsw_fan_state;

/* FAN control state */
extern USHORT					f_eqsw_fan_control_state;

/* fan temp threshold */
extern int						f_eqsw_fantemp_threshold[4][2];

/* RSV thread state */
extern UINT		f_eqsw_thrdstate;

/*	Alarm Cycle counting table */
extern UINT	f_eqsw_almCyclCount[D_RSV_ALM_NUM];

/* f_eqsw_errCyclCount */
extern UINT	f_eqsw_errCyclCount[D_RSV_ERR_NUM];
extern UINT	f_eqsw_errCyclCount_200ms[D_RSV_ERR_NUM_200MS];

/*	Error counting table	*/
extern UINT	f_eqsw_errCount[D_RSV_ERR_NUM];
extern UINT	f_eqsw_errCount_200ms[D_RSV_ERR_NUM_200MS];/* 4TRx */

/*	Error Recovery Cycle counting table */
extern UINT	f_eqsw_errRecCyclCount[D_RSV_ERR_NUM];
extern UINT	f_eqsw_errRecCyclCount_200ms[D_RSV_ERR_NUM_200MS];/* 4TRx */

/*	Error Recovery counting table */
extern UINT	f_eqsw_errRecCount[D_RSV_ERR_NUM];
extern UINT	f_eqsw_errRecCount_200ms[D_RSV_ERR_NUM_200MS];/* 4TRx */

extern T_MPSW_MFG_DATE			f_eqsw_last_chg;

extern UINT		f_eqs_alm_start_data;
extern UINT		f_eqs_status_start_data;
extern UINT		f_eqs_alm_cnt;

extern UINT		f_eqs_faultid_alm;

extern T_EQS_FM_NOTIF_DATA	f_eqs_fm_alrm_data[D_SYS_NUM32] ;

extern	UINT	f_eqs_StpWv_inhbitFlg;
extern	UINT	f_eqs_ledRegVal;
extern	T_EQS_SVCTRLMNG	f_eqs_stsInfoMng[E_EQS_STS_MAX];
extern	T_EQS_SVCTRLMNG	f_eqs_faultMng[E_EQS_FAULTID_MAX];
extern	T_EQS_SVCTRLMNG	f_eqs_faultMng_ex[E_EQS_FAULTID_MAX];
extern	UINT	f_eqs_stpWvFlag[E_EQS_STPWV_REG_MAX];
extern	UINT	f_eqs_operIniFlg;
extern	UINT	sigFltId;
extern	T_MPSW_MSI_IETF_HARDWARE_GET_CFM	f_eqs_ietfHwMsg;


extern	const	T_EQS_LEDINFO	f_eqs_ledInfo[E_EQS_LED_MAX][D_SYS_NUM8];
extern	const	T_EQS_STPWV_MNG	f_eqs_stpWvMng[E_EQS_STPWVMSK_MAX][E_EQS_STPWV_FCT_MAX];
extern	const	UINT	f_eqs_useChkBit[2];
extern	const	T_EQS_FAULT_TBL f_eqs_fault_info[E_EQS_FAULTID_MAX];

/* @} */
#endif	/* M_RS_EXT */
