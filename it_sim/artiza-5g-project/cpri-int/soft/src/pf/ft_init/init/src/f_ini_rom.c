/*!
 * @skip  $ld:$
 * @file  f_ini_rom.c
 * @brief 初期化ROMテーブル定義
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include	"f_ini_inc.h"

/* proc create manage table */
const	T_INI_CRT_EVENT_CTBL	f_inir_procCreMng[D_INI_PROCNO_MAX] = {
	/*================================================================================================*/
	/* procFunc,	  procCreateFlag,		procNo					procPriority	procCpu	PQID	  */
	/*================================================================================================*/
	{ "DU_pf",		  D_INI_CREATE_PTN1,	D_RRH_PROCID_PF,		95,				1,		D_RRH_PROCQUE_PF	},
	{ "DU_pf",		  D_INI_CREATE_PTN1,	D_RRH_PROCID_PTP,		95,				2,		D_RRH_PROCQUE_PTP	},
	{ "DU_init",	  D_INI_SKIP,			D_RRH_PROCID_INI,		95,				1,		D_RRH_PROCQUE_INIT	},
/* 2020/12/28 M&C) Implement device startup (start add) */
	{ "rrh_pf_fhm",		  D_INI_CREATE_PTN1,	D_RRH_PROCID_F_PF,		95,				0,		D_RRH_PROCQUE_F_PF	},/*rrh_pf*/
	{ "rrh_l2",		  D_INI_CREATE_PTN2,	D_RRH_PROCID_L2,		85,				1,		D_RRH_PROCQUE_L2	},
	{ "rrh_l3",		  D_INI_CREATE_PTN2,	D_RRH_PROCID_L3,		85,				0,		D_RRH_PROCQUE_L3	},
	{ "rrh_mnt",	  D_INI_CREATE_PTN2,	D_RRH_PROCID_MT,		85,				0,		D_RRH_PROCQUE_MT	},//C9
	{ "rrh_re",		  D_INI_CREATE_PTN2,	D_RRH_PROCID_RE,		85,				0,		D_RRH_PROCQUE_RE	} //C9
/* 2020/12/28 M&C) Implement device startup (end add) */
};

/* 障害ログ出力要求シグナルテーブル */
const	UINT	f_inir_AccidentlogType[D_INI_PROCNO_MAX] = {
	D_RRH_PROCID_PF,												/*!< PFプロセス					*/
};

/* INITから各プロセスへの障害ログ出力要求シグナルテーブル */
const	UINT	f_inir_reqSigType[E_INI_LOGTYPE_MAX] = {
	D_RRH_SIGUSR_OPT_TROUBLE_REQ,									/*!< 障害ログ出力要求(ALM発生)				*/
	D_RRH_SIGUSR_OPT_HLTCHK_REQ										/*!< 障害ログ出力要求(ヘルスチェック断)		*/

};

/* 各プロセスからINITへの受信シグナル種別テーブル */
const	UINT	f_inir_resSigType[E_INI_LOGTYPE_MAX] = {
	D_RRH_SIGUSR_OPT_TROUBLE_RES,									/*!< 障害ログ出力要求(ALM発生)				*/
	D_RRH_SIGUSR_OPT_HLTCHK_RES										/*!< 障害ログ出力要求(ヘルスチェック断)		*/
};


/* シグナル受信時のリセット時動作テーブル（TDD用） */
const	T_INIT_SIG_RESET_TBL	f_inir_SigResetMng_tdd[E_INI_SIGNAL_MAX]	= {

/*	WDT起動(ON：起動)		CPRI disable			無線系停止(PA OFF等)	ログ保存有無			LED消灯					TILT制御(TDDはALL OFF)	リセット制御 */
	/*! ローカルリセット[ローカルRST SW]			 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON},
	/*! M-plane リモートリセット	 */
	{D_INI_SIG_RESET_ON,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON},
	/*! RE強制リセット[CPRI L1 RST] (DPDCのみ)	 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF},
	/*! M-plane WDTによる自律リセット	 */
	{D_INI_SIG_RESET_ON,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON},
	/*! ソフト障害検知(ALM発生時)					 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_CHK},
	/*! ソフト障害検知(エラー発生時)				 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF},
	/*! SIGCHLD										 */
	{D_INI_SIG_RESET_ON,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON},
	/*! WDT T.O										 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF},
	/*! リソースALM									 */
	{D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_ON,		D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF,	D_INI_SIG_RESET_OFF}
};


/* Hdl_main paramater thread create manage table */
const T_INIT_CRT_EVENT_CTBL	f_inir_thdCreMng[D_INI_INITHDNO_MAX]	= {
	{"INIALM",		0,	(FUNCPTR)f_ini_main_alm,	0	},	/**< 障害ログThread				*/
/* 2020/12/28 M&C) Implement device startup (start add) */
	{"INIHLT",		0,	(FUNCPTR)f_ini_main_hlt,	0	},	/**< HealthCheck断Thread		*/
	{"INIRUN",		0,	(FUNCPTR)f_ini_main_run,	0	},	/**< 運用ログ取得Thread			*/
/* 2020/12/28 M&C) Implement device startup (end add) */
	{"INICON",		0,	(FUNCPTR)f_ini_main_con,	0	},	/**< NETCONF conection Thread	*/
};
/***** NETCONF conection Threadは、全Process起動後にThread createするため、				*****/
/***** 								必ず最後(D_INI_INITHDNO_MAX - 1)に定義すること。	*****/
 	
/* LOG取得 State Mng (障害発生) */
const T_INIT_TROUBLE_CALLSETRELL_MTXTBL f_inir_callSetRell_mtxtbl_alm[D_INIT_STATENUM_ALM][D_LOG_EVENTNUM_ALM] __attribute__ ((aligned (16))) =
{
	{
		/*	通常	*/
		{f_ini_logget_init_alm			},					/**< ALM発生							*/
	}
};

/* 2020/12/28 M&C) Implement device startup (start add) */
/** LOG取得 State Mng (HealthCheck) */
const T_INIT_TROUBLE_CALLSETRELL_MTXTBL f_inir_callSetRell_mtxtbl_hlt[D_INIT_STATENUM_HLT][D_LOG_EVENTNUM_HLT] __attribute__ ((aligned (16))) =
{
	{
		/*	通常	*/
		{f_ini_logget_init_hlt			}					/**< HealthCheck断発生					*/
	}
};

/** LOG取得 State Mng (運用ログ出力) */
const T_INIT_TROUBLE_CALLSETRELL_MTXTBL f_inir_callSetRell_mtxtbl_run[D_INIT_STATENUM_RUN][D_LOG_EVENTNUM_RUN] __attribute__ ((aligned (16))) =
{
	{
		/*	通常	*/
		{f_ini_logget_init_run			}					/**< 運用ログ出力要求受信				*/
	}
};
/* 2020/12/28 M&C) Implement device startup (end add) */

/* SIGNAL 情報取得用テーブル SIGUSR1		*/
const T_INIT_SIG_STATUS_INFO_TBL f_inir_sigusr1[ D_INI_SIG1_STATUS_MAX ] ={
	{ D_RRH_SIGUSR_OPT_RSET_LOCSW_NTC,						"STATUS=LOCAL RST(RE-MT)"},
	{ D_RRH_SIGUSR_OPT_RSET_L3_RMT_NTC,						"STATUS=RE CARD CONTROL RST"},
	{ D_RRH_SIGUSR_OPT_RSET_L1IND_NTC,						"STATUS=MP LINK DOWN RST"},
	{ D_RRH_SIGUSR_OPT_RSET_L3_RST_NTC,						"STATUS=RE RST"},
	{ D_RRH_SIGUSR_OPT_RSET_SWALM_NTC,						"STATUS=SOFT ALM"},
	{ D_RRH_SIGUSR_OPT_RSET_SWERR_NTC,						"STATUS=SOFT ERR"},
	{ D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC,						"STATUS=RSC ALM"},
	{ D_RRH_SIGUSR_OPT_TROUBLE_REQ,							"STATUS=TROUBLE LOG GET(ALM)"},
	{ (D_RRH_SIGUSR_OPT_HLTCHK_REQ & D_INI_SIGOPT_KIND),	"STATUS=TROUBLE LOG GET(HLT)"},
	{ D_RRH_SIGUSR_OPT_TROUBLE_RES,							"STATUS=TROUBLE LOG RSP(ALM)"},
	{ (D_RRH_SIGUSR_OPT_HLTCHK_RES & D_INI_SIGOPT_KIND),	"STATUS=TROUBLE LOG RSP(HLT)"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_REQ,							"STATUS=RUN LOG GET REQ(PF->INI)"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_REQ_3G,						"STATUS=RUN LOG GET REQ(PF->INI) 3G"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_GET,							"STATUS=RUN LOG GET REQ(INI->OTHER)"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_GET_3G,						"STATUS=RUN LOG GET REQ(INI->OTHER) 3G"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI,						"STATUS=RUN LOG GET FIN(OTHER->INI)"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G,					"STATUS=RUN LOG GET FIN(OTHER->INI) 3G"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF,						"STATUS=RUN LOG GET FIN(INI->PF)"},
	{ D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF_3G,					"STATUS=RUN LOG GET FIN(INI->PF) 3G"},
	{ D_RRH_SIGUSR_OPT_TROUBLE_FLASH0_REQ,					"STATUS=TROUBLE LOG WRITE REQ"},
	{ D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_ALM,					"STATUS=LOG GET NEXT"},
	{ 0,													"STATUS=UNKNOWN"}
};

/* SIGNAL 情報取得用テーブル SIGUSR2		*/
const T_INIT_SIG_STATUS_INFO_TBL f_inir_sigusr2[ D_INI_SIG2_STATUS_MAX ] ={
	{ D_RRH_SIGUSR2_OPT_WDTTO,		"STATUS=WDT"},
	{ D_RRH_SIGUSR2_OPT_CPRIL1RST,	"STATUS=MP LINK DOWN RST"},
	{ D_RRH_SIGUSR2_OPT_RSTSW,		"STATUS=LOCAL RST(BTN)"},
	{ 0,							"STATUS=UNKNOWN"}
};
/** @} */

