/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_rom.c
 *  @brief  CPRI management ROM table define
 *  @date 2013/11/14 FFCS)niemsh create
 *  @date 2015/11/06 TDI)satou ハソ102103104-003
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"

/**
 * @brief   main matrix table
 */
void    (* const f_cprr_matrix[D_CPR_STATMAX][D_CPR_EVTMAX]) (UINT *buff_adr) = 
{
	/* 0: init state */
	{  
	  f_cpr_nop,						/* cpri link init indication*/
	  f_cpr_nop,						/* cpri link Enable indication*/
  	  f_cpr_nop,   						/* cpri link Disable indication */
	  f_cpr_nop,						/* CPRI link system param updating*/
  	  f_cpr_nop,   						/* CPRI state change IRQ notice  */
	  f_cpr_nop,						/* The subscription of receving CPRI State  */
  	  f_cpr_nop,   						/* The unsubscription of receving CPRI State */
  	  f_cpr_nop,   						/* CPRI fiber output request */
  	  f_cpr_nop,   						/* CPRI Auto Nego Complete Notification */
  	  f_cpr_nop,   						/* CPRI Linkup Timeout Notification */
  	  f_cpr_nop,                        /* CPRI link stop indication */
  	  f_cpr_nop,                        /* delay adjustment request (step3) */
  	  f_cpr_nop,                        /* delay adjustment request (step7) */
  	  f_cpr_nop,                        /* delay adjustment request (step10) */
  	  f_cpr_nop,                        /* Master SFP power ON notification */
	  f_cpr_nop                         /* delay adjustment state clear request */
	},

	/* 1: running state */
	{	
	  f_cpr_CpriLinkInitProc,			/* cpri link init indication*/
	  f_cpr_CpriLinkEnableProc,			/* cpri link Enable indication*/
	  f_cpr_CpriLinkDisableProc,		/* cpri link Disable indication */
	  f_cpr_CpriParamUpdProc,			/* CPRI link system param updating*/
	  f_cpr_CpriStateChgProc,			/* CPRI state change IRQ notice  */
	  f_cpr_CpriStateSubProc,			/* The subscription of receving CPRI State  */
	  f_cpr_CpriStateUnsubProc,			/* The unsubscription of receving CPRI State */
	  f_cpr_CpriFiberOutput,			/* CPRI fiber output request */
	  f_cpr_CpriAutoNegoCompleteNtc,	/* CPRI Auto Nego Complete Notification */
	  f_cpr_CpriLinkupTimeOutNtc,   	/* CPRI Linkup Timeout Notification */
	  f_cpr_CpriLinkStopProc,           /* CPRI link stop indication */
	  f_cpr_DelayAdjustStep3Proc,       /* delay adjustment request (step3) */
	  f_cpr_DelayAdjustStep7Proc,       /* delay adjustment request (step7) */
	  f_cpr_DelayAdjustStep10Proc,      /* delay adjustment request (step10) */
  	  f_cpr_cpmPwrNoProc,               /* Master SFP power ON notification */
	  f_cpr_DelayAdjust_ClearStateProc  /* delay adjustmentj state clear request */
	}
};

/**
 * @brief   SFPコードチェック用テーブル.
 * @note    SFPのEEPROMから読み出したSFPコードと一致チェックするためのテーブル.
 */
const char *f_cpreeprom_sfp[D_CPR_EEPROM_SFPITEM_MAX] = {
	"CA46860-1094",		/* RE用Aタイプ */
	"CA46860-1095",		/* REC用Aタイプ */
	"CA46860-1096",		/* RE用Bタイプ */
	"CA46860-1097",		/* REC用Bタイプ */
	"CA46860-1348",		/* RE用Cタイプ */
	"CA46860-1349",		/* REC用Cタイプ */
	"CA46860-1350",		/* RE用Dタイプ */
	"CA46860-1351",		/* REC用Dタイプ */
	"CA46860-1386",     /* 9.8G/4.9G Slave用短距離タイプ */
	"CA46860-1385",     /* 9.8G/4.9G Master用短距離タイプ */
};

/**
 * @brief   SFPビットレートチェック用テーブル.
 * @note    SFPのEEPROMから読み出したSFPコードに対応するビットレート.
 * @note    配列要素の内容は f_cpreeprom_sfp の並びと合わせること.
 */
const int f_sfp_bitrate[D_CPR_EEPROM_SFPITEM_MAX] =
{
	D_COM_LINE_BITRATE_24G,
	D_COM_LINE_BITRATE_24G,
	D_COM_LINE_BITRATE_24G,
	D_COM_LINE_BITRATE_24G,
	D_COM_LINE_BITRATE_98G,
	D_COM_LINE_BITRATE_98G,
	D_COM_LINE_BITRATE_98G,
	D_COM_LINE_BITRATE_98G,
	D_COM_LINE_BITRATE_98G,
	D_COM_LINE_BITRATE_98G,
};

/**
 * @brief   SFPがREC用かRE用かチェックするためのテーブル.
 * @note    SFPのEEPROMから読み出したSFPコードに対応する.
 * @note    配列要素の内容は f_cpreeprom_sfp の並びと合わせること.
 */
const int f_sfp_typerec[D_CPR_EEPROM_SFPITEM_MAX] =
{
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1
};

/**
 * @brief   CPS VOD/Emphasis設定値(9.8G).
 * @note    master portの初期設定で使用する.
 */
const UINT CPM_VOD_EMPHASIS_data98g[D_RRH_CPRINO_RE_MAX] =
{
		0x000A0012,
		0x000A0010,
		0x000A0010,
		0x000A0010,
		0x00080010,
		0x00080010,
		0x0008000F,
		0x0008000E,
		0x0007000D,
		0x0008000D,
		0x0008000D,
		0x0007000D,
		0x0008000E,
		0x00090009,
		0x000A0009,
		0x000B0009
};

/**
 * @brief   CPS VOD/Emphasis設定値(4.9G).
 * @note    master portの初期設定で使用する.
 */
const UINT CPM_VOD_EMPHASIS_data49g[D_RRH_CPRINO_RE_MAX] =
{
		0x0007000B,
		0x0007000B,
		0x0007000B,
		0x00070009,
		0x00060009,
		0x00060009,
		0x00060009,
		0x00060009,
		0x00050009,
		0x0007000A,
		0x00060009,
		0x00060009,
		0x00060009,
		0x00060009,
		0x00070009,
		0x0007000A
};

/**
 * @brief   CPS VOD/Emphasis設定値(2.4G).
 * @note    master portの初期設定で使用する.
 */
const UINT CPM_VOD_EMPHASIS_data24g[D_RRH_CPRINO_RE_MAX] =
{
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006,
		0x000F0006
};

/* @} */

