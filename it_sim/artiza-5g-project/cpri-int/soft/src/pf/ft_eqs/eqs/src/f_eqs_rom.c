/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_rom.c
 *  @brief  RE supervision ROM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2015/06/17 ALP)Murakami TDD-ZYNQ 対応
 *  @date   2019/06/28 KCN)Fujita   O-RAN 対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/

/****************************************************************************/
/*  LED setting value							*/
/****************************************************************************/
const	T_EQS_LEDINFO	f_eqs_ledInfo[E_EQS_LED_MAX][D_SYS_NUM8] =
	{
		/*	LED1 state			*/
		{
			{ D_API_EQS_LEDSTATE_POWERON,		D_EQS_LED1SET_OFF		},	/*	0:電源ON		;消灯			*/
			{ D_API_EQS_LEDSTATE_ALM_CRITICAL,	D_EQS_LED1SET_REDON		},	/*	1:Ctiteical		;赤点灯			*/
			{ D_API_EQS_LEDSTATE_USE,			D_EQS_LED1SET_GRNON		},	/*	2:Active or busy;緑点灯			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED1SET_PREVAL	},	/*	3:定義無し		;赤緑点灯(禁止)	*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED1SET_PREVAL	},	/*	4:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_ALM_MAJOR,		D_EQS_LED1SET_REDBLNK	},	/*	5:Major			;赤点滅			*/
			{ D_API_EQS_LEDSTATE_OPERATION,		D_EQS_LED1SET_GRNBLNK	},	/*	6:Oper			;緑点滅			*/
			{ D_API_EQS_LEDSTATE_ADMINLOCK,		D_EQS_LED1SET_REDGRN	}	/*	7:Locked		;赤緑交互点灯	*/
		},
		/*	LED2 state			*/
		{
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_OFF		},	/*	0:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_BUSY,			D_EQS_LED2SET_ORGON		},	/*	1:L1断			;橙点灯			*/
			{ D_API_EQS_LEDSTATE_POWERON,		D_EQS_LED2SET_GRNON		},	/*	2:電源ON		;緑点灯			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	3:定義無し		;橙緑点灯(禁止)	*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	4:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_MAINT_FLDL,	D_EQS_LED2SET_ORGBLNK	},	/*	5:ファイル更新中;橙点滅			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_GRNBLNK	},	/*	6:定義無し		;緑点滅			*/
			{ D_API_EQS_LEDSTATE_ALM_MINOR,		D_EQS_LED2SET_ORGGRN	}	/*	7:Minor			;橙緑交互点灯	*/
		},
		/*	LED3 state			*/
		{
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_OFF		},	/*	0:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_BUSY,			D_EQS_LED2SET_ORGON		},	/*	1:L1断			;橙点灯			*/
			{ D_API_EQS_LEDSTATE_POWERON,		D_EQS_LED2SET_GRNON		},	/*	2:電源ON		;緑点灯			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	3:定義無し		;橙緑点灯(禁止)	*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	4:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_MAINT_FLDL,	D_EQS_LED2SET_ORGBLNK	},	/*	5:ファイル更新中;橙点滅			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_GRNBLNK	},	/*	6:定義無し		;緑点滅			*/
			{ D_API_EQS_LEDSTATE_ALM_MINOR,		D_EQS_LED2SET_ORGGRN	}	/*	7:Minor			;橙緑交互点灯	*/
		},
		/*	LED4 state			*/
		{
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_OFF		},	/*	0:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_BUSY,			D_EQS_LED2SET_ORGON		},	/*	1:L1断			;橙点灯			*/
			{ D_API_EQS_LEDSTATE_POWERON,		D_EQS_LED2SET_GRNON		},	/*	2:電源ON		;緑点灯			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	3:定義無し		;橙緑点灯(禁止)	*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	4:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_MAINT_FLDL,	D_EQS_LED2SET_ORGBLNK	},	/*	5:ファイル更新中;橙点滅			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_GRNBLNK	},	/*	6:定義無し		;緑点滅			*/
			{ D_API_EQS_LEDSTATE_ALM_MINOR,		D_EQS_LED2SET_ORGGRN	}	/*	7:Minor			;橙緑交互点灯	*/
		},
		/*	LED5 state			*/
		{
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_OFF		},	/*	0:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_BUSY,			D_EQS_LED2SET_ORGON		},	/*	1:L1断			;橙点灯			*/
			{ D_API_EQS_LEDSTATE_POWERON,		D_EQS_LED2SET_GRNON		},	/*	2:電源ON		;緑点灯			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	3:定義無し		;橙緑点灯(禁止)	*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_PREVAL	},	/*	4:定義無し		;消灯			*/
			{ D_API_EQS_LEDSTATE_MAINT_FLDL,	D_EQS_LED2SET_ORGBLNK	},	/*	5:ファイル更新中;橙点滅			*/
			{ D_API_EQS_LEDSTATE_UNDECIDED,		D_EQS_LED2SET_GRNBLNK	},	/*	6:定義無し		;緑点滅			*/
			{ D_API_EQS_LEDSTATE_ALM_MINOR,		D_EQS_LED2SET_ORGGRN	}	/*	7:Minor			;橙緑交互点灯	*/
		}
	};

/****************************************************************************/
/* 故障情報設定データテーブル												*/
/****************************************************************************/
const	T_EQS_FAULT_TBL f_eqs_fault_info[E_EQS_FAULTID_MAX] = {												/*0    5    10   15   20   25   30	*/
	{	D_EQS_FALT_FID01   ,  0 ,  8 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Soft ALM"						},	/*!< (00)Fault大項目種別 = Soft ALM							*//*	Critical	*/
	{	D_EQS_FALT_FID02   ,  0 , 22 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Initialization failure"			},	/*!< (01)Fault大項目種別 = Initialization Error				*//*	Critical	*/
	{	D_EQS_FALT_FID03   ,  0 , 31 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "Fronthaul Synchronization Error"	},	/*!< (02)Fault大項目種別 = Fronthaul Synchronization Error	*//*	Major		*/
	{	D_EQS_FALT_FID04   ,  0 , 19 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Module Clock Faulty"				},	/*!< (03)Fault大項目種別 = Module Clock Faulty				*//*	Critical	*/
	{	D_EQS_FALT_FID05   ,  0 , 16 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "Fronthaul faulty"				},	/*!< (04)Fault大項目種別 = Fronthaul faulty					*//*	Minor		*/
	{	D_EQS_FALT_FID06   ,  0 ,  6 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "PS ALM"							},	/*!< (05)Fault大項目種別 = PS ALM							*//*	Major		*/
	{	D_EQS_FALT_FID07   ,  0 , 17 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Unit out of order"				},	/*!< (06)Fault大項目種別 = common hardware fault			*//*	Critical	*/
	{	D_EQS_FALT_FID08   ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "RF module fault"					},	/*!< (07)Fault大項目種別 = RF module fault					*//*	Major		*/
	{	D_EQS_FALT_FID2010 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "TX out of order"					},	/*!< (08)Fault大項目種別 = TX out of order(AntA)			*//*	Major		*/
	{	D_EQS_FALT_FID2410 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "TX out of order"					},	/*!< (09)Fault大項目種別 = TX out of order(AntB)			*//*	Major		*/
	{	D_EQS_FALT_FID2810 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "TX out of order"					},	/*!< (10)Fault大項目種別 = TX out of order(AntC)			*//*	Major		*/
	{	D_EQS_FALT_FID2C10 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "TX out of order"					},	/*!< (11)Fault大項目種別 = TX out of order(AntD)			*//*	Major		*/
	{	D_EQS_FALT_FID2011 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "RX out of order"					},	/*!< (12)Fault大項目種別 = RX out of order(AntA)			*//*	Major		*/
	{	D_EQS_FALT_FID2411 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "RX out of order"					},	/*!< (13)Fault大項目種別 = RX out of order(AntB)			*//*	Major		*/
	{	D_EQS_FALT_FID2811 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "RX out of order"					},	/*!< (14)Fault大項目種別 = RX out of order(AntC)			*//*	Major		*/
	{	D_EQS_FALT_FID2C11 ,  0 , 15 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "RX out of order"					},	/*!< (15)Fault大項目種別 = RX out of order(AntD)			*//*	Major		*/
	{	D_EQS_FALT_FID2012 ,  0 , 17 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "ANT port abnormal"				},	/*!< (16)Fault大項目種別 = ANT port  abnormal(AntA)			*//*	Major		*/
	{	D_EQS_FALT_FID2412 ,  0 , 17 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "ANT port abnormal"				},	/*!< (17)Fault大項目種別 = ANT port  abnormal(AntB)			*//*	Major		*/
	{	D_EQS_FALT_FID2812 ,  0 , 17 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "ANT port abnormal"				},	/*!< (18)Fault大項目種別 = ANT port  abnormal(AntC)			*//*	Major		*/
	{	D_EQS_FALT_FID2C12 ,  0 , 17 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "ANT port abnormal"				},	/*!< (19)Fault大項目種別 = ANT port  abnormal(AntD)			*//*	Major		*/
	{	D_EQS_FALT_FID30   ,  0 , 19 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_MAJOR	, "Temperature too low"				},	/*!< (20)Fault大項目種別 = Temperature too low				*//*	Major		*/
	{	D_EQS_FALT_FID31   ,  0 , 28 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Unit dangerously overheating"	},	/*!< (21)Fault大項目種別 = Temperature too high				*//*	Critical	*/
	{	D_EQS_FALT_FID40   ,  0 , 15 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "No fan detected"					},	/*!< (22)Fault大項目種別 = No fan detected					*//*	Minor		*/
	{	D_EQS_FALT_FID41   ,  0 , 10 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "FAN PS ALM"						},	/*!< (23)Fault大項目種別 = FAN PS ALM						*//*	Minor		*/
	{	D_EQS_FALT_FID42   ,  0 , 24 , E_FAULT_SOURCE_FAN_1		, NULL			, E_FAULT_SEVERITY_MINOR	, "Cooling fan broken FAN#1"		},	/*!< (24)Fault大項目種別 = Cooling fan broken FAN#1			*//*	Minor		*/
	{	D_EQS_FALT_FID43   ,  0 , 24 , E_FAULT_SOURCE_FAN_2		, NULL			, E_FAULT_SEVERITY_MINOR	, "Cooling fan broken FAN#2"		},	/*!< (25)Fault大項目種別 = Cooling fan broken FAN#2			*//*	Minor		*/
	{	D_EQS_FALT_FID44   ,  0 , 24 , E_FAULT_SOURCE_FAN_3		, NULL			, E_FAULT_SEVERITY_MINOR	, "Cooling fan broken FAN#3"		},	/*!< (26)Fault大項目種別 = Cooling fan broken FAN#3			*//*	Minor		*/
	{	D_EQS_FALT_FID50   ,  0 ,  9 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "SFP Error"						},	/*!< (27)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
	{	D_EQS_FALT_FIDF0   ,  0 ,  8 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "Tx Sleep"						},	/*!< (28)Fault大項目種別 = Tx Sleep							*//*	Minor		*/
	{	D_EQS_FALT_FIDF1   ,  0 ,  6 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "Tx OFF"							},	/*!< (29)Fault大項目種別 = Tx OFF							*//*	Minor		*/
	{	D_EQS_FALT_FIDF2   ,  0 ,  9 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "TEST Mode"						},	/*!< (30)Fault大項目種別 = Beam ID Fix						*//*	Minor		*/
	{	D_EQS_FALT_FIDFF   ,  0 , 12 , E_FAULT_SOURCE_LLSDU		, NULL			, E_FAULT_SEVERITY_CRITICAL	, "Forced alarm"					},	/*!< (31)Fault大項目種別 = Forced alarm						*//*	Critical	*/
	{	D_EQS_FALT_FID51   ,  0 , 13 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "SFP Fail Safe"					},	/*!< (32)Fault大項目種別 = SFP Fail Safe					*//*	Minor		*/
	{	D_EQS_FALT_FIDEA60 ,  0 , 15 , E_FAULT_SOURCE_UNKOWN	, NULL			, E_FAULT_SEVERITY_MINOR	, "SFP Error"						}	/*!< (33)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
};																											/*0    5    10   15   20   25   30	*/

/*	Stop Wave check manage	*/
const	T_EQS_STPWV_MNG	f_eqs_stpWvMng[E_EQS_STPWVMSK_MAX][E_EQS_STPWV_FCT_MAX] =
	{
		/*	mask off	*/
		{
			/*	svinf Index		, svinf mask					, TDD En					, StpWv						*/
			{ E_EQS_SVINF_01	, D_EQS_STPWV_SV01INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	locked										*/
			{ E_EQS_SVINF_05	, D_EQS_STPWV_SV05INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	soft ALM & Initialization failure			*/
			{ E_EQS_SVINF_06	, D_EQS_STPWV_SV06INF_PTP_DIS   , D_EQS_STPWV_ANT_EN		, D_EQS_STPWV_ANT_PAOFF_ALL	},	/*	PTP UnLock									*/
			{ E_EQS_SVINF_06	, D_EQS_STPWV_SV06INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Module Clock Faulty & PS ALM				*/
			{ E_EQS_SVINF_07	, D_EQS_STPWV_SV07INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	common hardware fault						*/
			{ E_EQS_SVINF_08	, D_EQS_STPWV_SV08INF_FCT_DIS_AB, D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	RF module fault(RFIC#1)						*/
			{ E_EQS_SVINF_08	, D_EQS_STPWV_SV08INF_FCT_DIS_CD, D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	RF module fault(RFIC#2)						*/
			{ E_EQS_SVINF_09	, D_EQS_STPWV_SV09INF_FCT_DIS_A , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#A)	*/
			{ E_EQS_SVINF_09	, D_EQS_STPWV_SV09INF_FCT_DIS_B , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#B)	*/
			{ E_EQS_SVINF_10	, D_EQS_STPWV_SV10INF_FCT_DIS_C , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#C)	*/
			{ E_EQS_SVINF_10	, D_EQS_STPWV_SV10INF_FCT_DIS_D , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#D)	*/
			{ E_EQS_SVINF_11	, D_EQS_STPWV_SV11INF_FCT_DIS_A , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order(tx power detector ALM V#n)	*/
			{ E_EQS_SVINF_11	, D_EQS_STPWV_SV11INF_FCT_DIS_B , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order(tx power detector ALM H#n)	*/
			{ E_EQS_SVINF_13	, D_EQS_STPWV_SV13INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Temperature too high						*/
			{ E_EQS_SVINF_14	, D_EQS_STPWV_SV14INF_FCT_OFF   , D_EQS_STPWV_ANT_PAOFF_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Tx Sleep & Tx OFF							*/
			{ E_EQS_SVINF_14	, D_EQS_STPWV_SV14INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Forced alarm								*/
			{ E_EQS_SVINF_16	, D_EQS_STPWV_SV16INF_FCT_OFF   , D_EQS_STPWV_ANT_EN		, D_EQS_STPWV_ANT_PAOFF_ALL	}	/*	PTP, SyncE									*/
		},
		/*	mask on	*/
		{
			/*	svinf Index		, svinf mask					, TDD En					, StpWv						*/
			{ E_EQS_SVINF_01	, D_EQS_STPWV_SV01INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	locked										*/
			{ E_EQS_SVINF_05	, D_EQS_STPWV_SV05INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	soft ALM & Initialization failure			*/
			{ E_EQS_SVINF_06	, D_EQS_STPWV_SVINF_NONFCT      , D_EQS_STPWV_ANT_EN		, D_EQS_STPWV_ANT_EN		},	/*	PTP UnLock									*/
			{ E_EQS_SVINF_06	, D_EQS_STPWV_SV06INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Module Clock Faulty & PS ALM				*/
			{ E_EQS_SVINF_07	, D_EQS_STPWV_SV07INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	common hardware fault						*/
			{ E_EQS_SVINF_08	, D_EQS_STPWV_SV08INF_FCT_DIS_AB, D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	RF module fault(RFIC#1)						*/
			{ E_EQS_SVINF_08	, D_EQS_STPWV_SV08INF_FCT_DIS_CD, D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	RF module fault(RFIC#2)						*/
			{ E_EQS_SVINF_09	, D_EQS_STPWV_SV09INF_FCT_DIS_A , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#A)	*/
			{ E_EQS_SVINF_09	, D_EQS_STPWV_SV09INF_FCT_DIS_B , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#B)	*/
			{ E_EQS_SVINF_10	, D_EQS_STPWV_SV10INF_FCT_DIS_C , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#C)	*/
			{ E_EQS_SVINF_10	, D_EQS_STPWV_SV10INF_FCT_DIS_D , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order & RX out of order(ANT#D)	*/
			{ E_EQS_SVINF_11	, D_EQS_STPWV_SV11INF_FCT_DIS_A , D_EQS_STPWV_ANT_DIS_AC	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order(tx power detector ALM V#n)	*/
			{ E_EQS_SVINF_11	, D_EQS_STPWV_SV11INF_FCT_DIS_B , D_EQS_STPWV_ANT_DIS_BD	, D_EQS_STPWV_ANT_EN		},	/*	TX out of order(tx power detector ALM H#n)	*/
			{ E_EQS_SVINF_13	, D_EQS_STPWV_SV13INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Temperature too high						*/
			{ E_EQS_SVINF_14	, D_EQS_STPWV_SV14INF_FCT_OFF   , D_EQS_STPWV_ANT_PAOFF_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Tx Sleep & Tx OFF							*/
			{ E_EQS_SVINF_14	, D_EQS_STPWV_SV14INF_FCT_DIS   , D_EQS_STPWV_ANT_DIS_ALL	, D_EQS_STPWV_ANT_EN		},	/*	Forced alarm								*/
			{ E_EQS_SVINF_16	, D_EQS_STPWV_SVINF_NONFCT      , D_EQS_STPWV_ANT_EN		, D_EQS_STPWV_ANT_EN		}	/*	PTP, SyncE									*/
		}
	};

/*	USE Sts check bit manage	*/
const	UINT	f_eqs_useChkBit[E_EQS_DEVKIND_MAX] = {D_DU_SVCTL_BIT_USE_ALL_SUB6, D_DU_SVCTL_BIT_USE_ALL_MMW};

/* @} */
