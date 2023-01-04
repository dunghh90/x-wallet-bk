/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_ram.c
 *  @brief  RE supervision RAM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2011/11/24 COMAS)Uchida  M-S3G-eNBSYS-02539対応
 *  @date   2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04092 CR-00054-000 15G-RREのPA OFF問題対応
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/

/** @name RSV log management table */
/* @{ */

/****************************************************************************/
/* ステータスリストデータ                                                   */
/****************************************************************************/
T_EQS_STLIST			f_eqsw_stslist;


/****************************************************************************/
/* ログカウンタテーブル														*/
/****************************************************************************/
USHORT					f_eqsw_logcnt;

/****************************************************************************/
/* ログ取得テーブル															*/
/****************************************************************************/
T_RSV_LOG				f_eqsw_log_tbl[D_RSV_LOG_MAX];
/* @} */

/** @name TRA card status change management table */
/* @{ */

/****************************************************************************/
/* TRA状変検出テーブル												*/
/****************************************************************************/
T_SYS_EQS_ALM_INFO					f_eqsw_tra_sv_newsta;
/* @} */


UINT					f_eqsw_wvstop[D_SYS_NUM8];

/** @name TRA card restart both alm flagg */
/* @{ */

/****************************************************************************/
/* TRA card restart both alm flag */
/****************************************************************************/
USHORT					f_eqsw_restart_both_alm_flg;
/* @} */

/****************************************************************************/
/*	Alarm counting table													*/
/****************************************************************************/
UINT	f_eqsw_almCount[D_RSV_ALM_NUM];

/** @name RSV thread state */
/* @{ */
UINT		f_eqsw_thrdstate;
/* @} */

/** @name Error Control table */
/* @{ */
UINT		f_eqsw_errctl_tbl;
/* @} */

/** @name TX sys number */
/* @{ */
USHORT					f_eqsw_tx_sys_num;
/* @} */

T_MPSW_MFG_DATE			f_eqsw_last_chg;

/** @name Alarm Start Status */
/* @{ */
UINT		f_eqs_alm_start_data;
/* @} */

/** @name Status Change Start Status */
/* @{ */
UINT		f_eqs_status_start_data;
/* @} */

/** @name FM Alarm Counter */
/* @{ */
UINT		f_eqs_alm_cnt;
/* @} */

T_EQS_FM_NOTIF_DATA	f_eqs_fm_alrm_data[D_SYS_NUM32] ;

UINT		f_eqs_faultid_alm;

/** @name Stop Wave inhibit flag */
/* @{ */
UINT		f_eqs_StpWv_inhbitFlg = E_EQS_STPWVMSK_OFF;
/* @} */

/** @name led manage */
/* @{ */
UINT		f_eqs_ledRegVal;
/* @} */

/** @name fault manage */
/* @{ */
T_EQS_SVCTRLMNG	f_eqs_stsInfoMng[E_EQS_STS_MAX] =
	{
		{ E_EQS_SVINF_01, D_DU_REG_CTB_SV1INF , D_DU_SVCTL_BIT_OPE_ALL	, 0 },	/*	Oper State				*/
		{ E_EQS_SVINF_01, D_DU_REG_CTB_SV1INF , D_DU_SVCTL_BIT_ADM_ALL	, 0 },	/*	Admin State				*/
		{ E_EQS_SVINF_01, D_DU_REG_CTB_SV1INF , D_DU_SVCTL_BIT_USA_ALL	, 0 },	/*	usage State				*/
		{ E_EQS_SVINF_01, D_DU_REG_CTB_SV1INF , D_DU_SVCTL_BIT_AVL_ALL	, 0 },	/*	availability State		*/
		{ E_EQS_SVINF_01, D_DU_REG_CTB_SV1INF , D_DU_SVCTL_BIT_ENG_ALL	, 0 }	/*	power State				*/
	};

/** @name fault manage */
/* @{ */
T_EQS_SVCTRLMNG	f_eqs_faultMng[E_EQS_FAULTID_MAX] =
	{
		{ E_EQS_SVINF_05,	D_DU_REG_CTB_SV5INF , D_DU_ALMBIT_FLT01_ALL     , 0 },	/*!< (00)Fault大項目種別 = Soft ALM							*//*	Critical	*/
		{ E_EQS_SVINF_05,	D_DU_REG_CTB_SV5INF , D_DU_ALMBIT_FLT02_ALL     , 0 },	/*!< (01)Fault大項目種別 = Initialization Error				*//*	Critical	*/
		{ E_EQS_SVINF_06,	D_DU_REG_CTB_SV6INF , D_DU_ALMBIT_FLT03_ALL     , 0 },	/*!< (02)Fault大項目種別 = Fronthaul Synchronization Error	*//*	Major		*/
		{ E_EQS_SVINF_06,	D_DU_REG_CTB_SV6INF , D_DU_ALMBIT_FLT04_ALL     , 0 },	/*!< (03)Fault大項目種別 = Module Clock Faulty				*//*	Critical	*/
		{ E_EQS_SVINF_06,	D_DU_REG_CTB_SV6INF , D_DU_ALMBIT_FLT05_ALL     , 0 },	/*!< (04)Fault大項目種別 = Fronthaul faulty					*//*	Minor		*/
		{ E_EQS_SVINF_06,	D_DU_REG_CTB_SV6INF , D_DU_ALMBIT_FLT06_ALL     , 0 },	/*!< (05)Fault大項目種別 = PS ALM							*//*	Major		*/
		{ E_EQS_SVINF_07,	D_DU_REG_CTB_SV7INF , D_DU_ALMBIT_FLT07_ALL     , 0 },	/*!< (06)Fault大項目種別 = common hardware (module) fault	*//*	Critical	*/
		{ E_EQS_SVINF_08,	D_DU_REG_CTB_SV8INF , D_DU_ALMBIT_FLT08_ALL     , 0 },	/*!< (07)Fault大項目種別 = RF module fault					*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT10_A_ALL   , 0 },	/*!< (08)Fault大項目種別 = TX out of order(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT10_B_ALL   , 0 },	/*!< (09)Fault大項目種別 = TX out of order(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT10_C_ALL   , 0 },	/*!< (10)Fault大項目種別 = TX out of order(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT10_D_ALL   , 0 },	/*!< (11)Fault大項目種別 = TX out of order(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT11_A_ALL   , 0 },	/*!< (12)Fault大項目種別 = RX out of order(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT11_B_ALL   , 0 },	/*!< (13)Fault大項目種別 = RX out of order(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT11_C_ALL   , 0 },	/*!< (14)Fault大項目種別 = RX out of order(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT11_D_ALL   , 0 },	/*!< (15)Fault大項目種別 = RX out of order(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT12_A_ALL   , 0 },	/*!< (16)Fault大項目種別 = ANT port  abnormal(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_09,	D_DU_REG_CTB_SV9INF , D_DU_ALMBIT_FLT12_B_ALL   , 0 },	/*!< (17)Fault大項目種別 = ANT port  abnormal(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT12_C_ALL   , 0 },	/*!< (18)Fault大項目種別 = ANT port  abnormal(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_10,	D_DU_REG_CTB_SV10INF, D_DU_ALMBIT_FLT12_D_ALL   , 0 },	/*!< (19)Fault大項目種別 = ANT port  abnormal(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT30_ALL     , 0 },	/*!< (20)Fault大項目種別 = Temperature too low				*//*	Major		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT31_ALL     , 0 },	/*!< (21)Fault大項目種別 = Temperature too high				*//*	Critical	*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT40_ALL     , 0 },	/*!< (22)Fault大項目種別 = No fan detected					*//*	Minor		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT41_ALL     , 0 },	/*!< (23)Fault大項目種別 = FAN PS ALM						*//*	Minor		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT42_ALL     , 0 },	/*!< (24)Fault大項目種別 = Cooling fan broken FAN#1			*//*	Minor		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT43_ALL     , 0 },	/*!< (25)Fault大項目種別 = Cooling fan broken FAN#2			*//*	Minor		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT44_ALL     , 0 },	/*!< (26)Fault大項目種別 = Cooling fan broken FAN#3			*//*	Minor		*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT50_ALL     , 0 },	/*!< (27)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
		{ E_EQS_SVINF_14,	D_DU_REG_CTB_SV14INF, D_DU_ALMBIT_FLTF0_ALL     , 0 },	/*!< (28)Fault大項目種別 = Tx Sleep							*//*	Minor		*/
		{ E_EQS_SVINF_14,	D_DU_REG_CTB_SV14INF, D_DU_ALMBIT_FLTF1_ALL     , 0 },	/*!< (29)Fault大項目種別 = Tx OFF							*//*	Minor		*/
		{ E_EQS_SVINF_14,	D_DU_REG_CTB_SV14INF, D_DU_ALMBIT_FLTF2_ALL     , 0 },	/*!< (30)Fault大項目種別 = TEST Mode						*//*	Minor		*/
		{ E_EQS_SVINF_14,	D_DU_REG_CTB_SV14INF, D_DU_ALMBIT_FLTFF_ALL     , 0 },	/*!< (31)Fault大項目種別 = Forced alarm						*//*	Critical	*/
		{ E_EQS_SVINF_13,	D_DU_REG_CTB_SV13INF, D_DU_ALMBIT_FLT51ALM_ALL  , 0 },	/*!< (32)Fault大項目種別 = SFP Fail Safe					*//*	Minor		*/
		{ E_EQS_SVINF_15,	D_DU_REG_CTB_SV15INF, D_DU_ALMBIT_FLTEA60_ALL   , 0 }	/*!< (33)Fault大項目種別 = SFP Type Error					*//*	Minor		*/

	};

T_EQS_SVCTRLMNG	f_eqs_faultMng_ex[E_EQS_FAULTID_MAX] =
	{
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (00)Fault大項目種別 = Soft ALM							*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (01)Fault大項目種別 = Initialization Error				*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (02)Fault大項目種別 = Fronthaul Synchronization Error	*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (03)Fault大項目種別 = Module Clock Faulty				*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (04)Fault大項目種別 = Fronthaul faulty					*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (05)Fault大項目種別 = PS ALM							*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (06)Fault大項目種別 = common hardware (module) fault	*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (07)Fault大項目種別 = RF module fault					*//*	Major		*/
		{ E_EQS_SVINF_11,	D_DU_REG_CTB_SV11INF, D_DU_ALMBIT_FLT10_V_ALL   , 0 },	/*!< (08)Fault大項目種別 = TX out of order(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_11,	D_DU_REG_CTB_SV11INF, D_DU_ALMBIT_FLT10_H_ALL   , 0 },	/*!< (09)Fault大項目種別 = TX out of order(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (10)Fault大項目種別 = TX out of order(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (11)Fault大項目種別 = TX out of order(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (12)Fault大項目種別 = RX out of order(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (13)Fault大項目種別 = RX out of order(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (14)Fault大項目種別 = RX out of order(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (15)Fault大項目種別 = RX out of order(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (16)Fault大項目種別 = ANT port  abnormal(AntA)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (17)Fault大項目種別 = ANT port  abnormal(AntB)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (18)Fault大項目種別 = ANT port  abnormal(AntC)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (19)Fault大項目種別 = ANT port  abnormal(AntD)			*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (20)Fault大項目種別 = Temperature too low				*//*	Major		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (21)Fault大項目種別 = Temperature too high				*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (22)Fault大項目種別 = No fan detected					*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (23)Fault大項目種別 = FAN PS ALM						*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (24)Fault大項目種別 = Cooling fan broken FAN#1			*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (25)Fault大項目種別 = Cooling fan broken FAN#2			*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (26)Fault大項目種別 = Cooling fan broken FAN#3			*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (27)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (28)Fault大項目種別 = Tx Sleep							*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (29)Fault大項目種別 = Tx OFF							*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (30)Fault大項目種別 = TEST Mode						*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (31)Fault大項目種別 = Forced alarm						*//*	Critical	*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 },	/*!< (32)Fault大項目種別 = SFP Fail Safe					*//*	Minor		*/
		{ E_EQS_SVINF_MAX,	0					, D_DU_SVCTL_ON_OFF_ALL     , 0 }	/*!< (33)Fault大項目種別 = SFP Type Error					*//*	Minor		*/

	};

/*	Stop wave flag	*/
UINT	f_eqs_stpWvFlag[E_EQS_STPWV_REG_MAX];

UINT	f_eqs_operIniFlg = D_RRH_OFF;

UINT	sigFltId = D_RRH_OFF;

T_MPSW_MSI_IETF_HARDWARE_GET_CFM	f_eqs_ietfHwMsg;							/* 送信メッセージ(ietf-hardware GET(OK応答))	*/


/* @} */
