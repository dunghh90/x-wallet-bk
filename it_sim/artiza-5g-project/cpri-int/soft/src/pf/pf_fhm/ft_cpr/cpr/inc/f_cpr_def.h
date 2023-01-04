/*!
* @skip $Id:$
* @file f_cpr_def.h
* @brief pf_cpri thread Define Header
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2017
*/
/** @addtogroup RRH_PF_CPR
 *  @{
 */
#ifndef F_CPR_DEF_H
#define F_CPR_DEF_H

/** @name thread state define */
/* @{ */
#define D_CPR_THRDSTA_INIT	0			/* 初期状態						*/
#define D_CPR_THRDSTA_RUN	1			/* 運用中状態					*/
/* @} */

/**
 * @name thread event define
 * @date 2015/10/06 TDI)satou D_CPR_EVT_CPRILNK_STOP_INDを追加
 * @{
 */
#define D_CPR_EVT_CPRILNK_STARTUP_IND				0			
#define D_CPR_EVT_CPRILNK_ENALBE_IND				1			
#define D_CPR_EVT_CPRILNK_DISABLE_IND 				2	
#define D_CPR_EVT_SYSTEMPARM_UPDATE_IND 			3
#define D_CPR_EVT_CPRILNK_STATE_CHG_NTC 			4
#define D_CPR_EVT_CPRILNK_STATE_SUB_REQ 			5
#define D_CPR_EVT_CPRILNK_STATE_UNSUB_IND 			6
#define D_CPR_EVT_CPRILNK_FIBER_OUTPUT_IND 			7
#define D_CPR_EVT_AUTONEGO_COMPLETE_NTC 			8
#define D_CPR_EVT_LINKUP_TIMEOUT_NTC 			9
#define D_CPR_EVT_CPRILNK_STOP_IND                  10
#define D_CPR_EVT_DELAY_ADJUST_STEP3_REQ            11
#define D_CPR_EVT_DELAY_ADJUST_STEP7_REQ            12
#define D_CPR_EVT_DELAY_ADJUST_STEP10_REQ           13
#define D_CPR_EVT_CPMPOWERON_IND                    14
#define D_CPR_EVT_DELAY_ADJUST_CLEAR_REQ			15

/* @} */
/****************************************************************************/
/* CPRI state define, which is corresponding to IRQ3制御レジスタ要因表示2*/
/****************************************************************************/
#define D_CPR_CPRISTA_BIT_HDLCACT	0x4000   /* HDLC ACT   */
#define D_CPR_CPRISTA_BIT_HDLCIDLE	0x2000   /* HDLC IDLE   */
#define D_CPR_CPRISTA_BIT_EDWN		0x1000   /* CPRI stateE donw */
#define D_CPR_CPRISTA_BIT_E			0x0800   /* CPRI stateE */
#define D_CPR_CPRISTA_BIT_D			0x0400   /* CPRI stateD */
#define D_CPR_CPRISTA_BIT_C			0x0200   /* CPRI stateC */
#define D_CPR_CPRISTA_BIT_B			0x0100   /* CPRI stateB */
#define D_CPR_CPRISTA_BIT_A			0x0080   /* CPRI stateA */
#define D_CPR_CPRISTA_BIT_RATE		0x0040   /* CPRI rate */
#define D_CPR_CPRISTA_BIT_SYNC		0x0008   /* CPRI sync */
/* @} */

/** @name thread event define */
/* @{ */
#define D_CPR_STATMAX		2
#define D_CPR_EVTMAX		16
/* @} */

/** @name runnning history */
/* @{ */
#define D_CPR_RECVIEVEFLG	0
#define D_CPR_SENDIEVEFLG	1
#define D_CPR_RUNHIS_FACENUM_MAX	512
/* @} */

/* CQPLL Lock Check Time polling 1msec */
#define D_CPR_CQPLLLOCK_TIME_POLLING	1

/* CQPLL Lock Check Time 10msec */
#define D_CPR_CQPLLLOCK_TIME_10MSEC		10


/* CPP PHY Done Time polling 10msec */
#define D_CPR_CPSSFPDONE_TIME_POLLING			500

/* CPP PHY Done Time 100msec */
#define D_CPR_CPSSFPDONE_TIME_100MSEC			100

/* CPP PHY Done Time polling 10msec */
#define D_CPM_PHYDONE_TIME_POLLING			10

/* CPP PHY Done Time 100msec */
#define D_CPM_PHYDONE_TIME_100MSEC			100

/* HFN Time 1shot */
#define D_CPM_ONE_SHOT_TIME_10MSEC			10

/* CPP HFNSYNC continuous times */
#define D_CPR_HFNSYNC_TIMES				3

/* MASTER Linkupタイマー */
#define D_CPM_LINKUP_TIME_MAX				600 * D_CPM_ONE_SHOT_TIME_10MSEC


/* eeprom sfp  data */
#define D_CPR_EEPROM_SFPITEM_MAX	10
#define D_CPR_EEPROM_SFPSTR_MAX	3
#define D_CPR_EEPROM_SFPSTR_LENGTH	12

/* LTE下り上り遅延補正/DLUL位相差補正/Wcdma下り上り遅延補正 レジスタ設定上限値 */
#define D_CPR_DRYADJ_UPLIMIT_MAX			0x9FFF

#define D_CPR_LUDELAY_OFFSET				245
#define D_CPR_WUDELAY_OFFSET				245
#define D_CPR_LDDELAY_OFFSET				348
#define D_CPR_WDDELAY_OFFSET				348

#define D_CPR_DELAY_MERGE_MAX				32


#endif
/** @} */
