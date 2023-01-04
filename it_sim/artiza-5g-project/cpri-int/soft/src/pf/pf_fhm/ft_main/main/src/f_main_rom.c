/*!
 * @skip		$ld:$
 * @file		f_main_rom.c
 * @brief		pf_main ROMテーブル
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date 		2015/05/27 ALPHA) murakami ハソ改版(M-RRU-ZSYS-01582)対応
 * @date  		2015/06/09 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date        2015/07/27 TDI) satou pf_rfcpを削除(FHMで使用しないスレッドのため)
 * @date        2015/07/30 TDIPS) maruyama スレッド管理テーブルから見直し(DIAスレッド削除、FRWREスレッド追加)
 * @date        2015/08/19 TDIPS) sasaki f_mainr_cmusRecvThdCreMngにD_API_MSGID_FILE_FIRM_READ_REQ追加
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include	"f_main_inc.h"

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

/*!
 * @brief   thread create manage table.
 * @date    2015/07/30 TDIPS) maruyama スレッド管理テーブル見直し(DIAスレッド削除、FRWREスレッド追加)
 */
T_MAIN_CRT_EVENT_CTBL	f_mainr_thdCreMng_TDD[D_MAIN_THDID_MAX]	= {
	{D_SYS_THDID_PF_F_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MAIN,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_SKIP				 ,	"pf_main"	},		/**< PF Main Thread									*/
	{D_SYS_THDID_PF_HDLM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_HDL,			(FUNCPTR)f_hdl_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_hdlr_thdCreMng[0],		D_MAIN_CREATE_PTN_NOT_WDT,	"pf_hdl"	},		/**< handle Main Thread								*/
	{D_SYS_THDID_PF_RECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RECV,		(FUNCPTR)f_recv_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusRecvThdCreMng,	D_MAIN_CREATE_PTN_NOT_WDT,	"pf_recv"	},		/**< PF Receive Thread(プロセス間受信部)  CMU-RCV	*/
	{D_SYS_THDID_PF_SEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SEND,		(FUNCPTR)f_send_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusSendThdCreMng,	D_MAIN_CREATE_PTN_NOT_WDT,	"pf_send"	},		/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{D_SYS_THDID_PF_ETH,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_ETHR,		(FUNCPTR)f_eth_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eth"	},		/**< Ethernet Set Thread							*/
	{D_SYS_THDID_PF_CPRI,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_CPRI,		(FUNCPTR)f_cpr_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_cpr"	},		/**< CPRI Management Thread							*/
	{D_SYS_THDID_PF_RSV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RSV,			(FUNCPTR)f_rsv_Main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_rsv"	},		/**< RE Supervision Thread							*/
    {D_SYS_THDID_PF_F_DPD,  BPF_RU_ITCM_SCHED_FIFO,     D_RRH_PRIORITY_DPDA,        (FUNCPTR)f_dpda_main_fhm,   D_RRH_STACKSIZE_DEFAULT,        D_RRH_NULL,                         D_MAIN_CREATE_PTN_NOT_WDT,  "pf_dpda"   },      /**< DPD thread                                     */
	{D_SYS_THDID_PF_NEGO,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NEGO,		(FUNCPTR)f_nego_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_nego"	},		/**< nego thread									*/
	{D_SYS_THDID_PF_LOG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_LOGM,		(FUNCPTR)f_log_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_log"	},		/**< Trouble Log Thread								*/
	{D_SYS_THDID_PF_FRMG,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FMNG,		(FUNCPTR)f_fmng_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_fmng"	},		/**< fmg Thread										*/
	{D_SYS_THDID_PF_FRQ,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FRQS,		(FUNCPTR)f_frw_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_frq"	},		/**< flash(QSPI) writing Thread						*/
	{D_SYS_THDID_PF_F_WDT,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_WDTT,		(FUNCPTR)f_wdt_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_wdt"	},		/**< Watch Dog Timer Thread							*/
	{D_SYS_THDID_PF_DBG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_DEBG,		(FUNCPTR)f_dbg_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_debugparam,			D_MAIN_CREATE_PTN_NOT_WDT,	"pf_dbg"	},		/**< dbg Thread										*/
	{D_SYS_THDID_PF_FRQS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FRQS,		(FUNCPTR)f_frwsla_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_frwsla"	},		/**< flash(QSPI) writing Thread(slave)				*/
	{D_SYS_THDID_PF_FRQRE,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FRQRE,		(FUNCPTR)f_frwre_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,							D_MAIN_CREATE_PTN_NOT_WDT,	"pf_frqre"	}		/**< flash(QSPI) writing(for RE) Thread				*/
};

/** Hdl_main paramater thread create manage table(TDD) */
const T_HDL_CRT_EVENT_CTBL	f_hdlr_thdCreMng[D_RRH_HDLTHDNO_MAX]	= {
	{D_RRH_THDNAME_HDCPRI,		D_RRH_PRIORITY_HDCPRI,		(FUNCPTR)f_hdl_main_cpri,		D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ84 Thread(CPRI_State)		*/
	{D_RRH_THDNAME_HDCPRI_RE,	D_RRH_PRIORITY_HDCPRI_RE,	(FUNCPTR)f_hdl_main_cpri_re,	D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ84 Thread(CPRIRE_State)		*/
	{D_RRH_THDNAME_HD1S,		D_RRH_PRIORITY_HD1S,		(FUNCPTR)f_hdl_main_1s,			D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ62 Thread(1SEC)				*/
	{D_RRH_THDNAME_HDSV,		D_RRH_PRIORITY_HDSV,		(FUNCPTR)f_hdl_main_sv,			D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ67 Thread(SV状変)			*/
	{D_RRH_THDNAME_HDL1,		D_RRH_PRIORITY_HDL1,		(FUNCPTR)f_hdl_main_l1,			D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ65 Thread(L1 RST受信)		*/
	{D_RRH_THDNAME_HDWDT,		D_RRH_PRIORITY_HDWDT,		(FUNCPTR)f_hdl_main_wdt,		D_RRH_STACKSIZE_DEFAULT	},	/**< Handler IRQ61 Thread(WDT TO)			*/
	{D_RRH_THDNAME_HDPCIE,		D_RRH_PRIORITY_HDPCIE,		(FUNCPTR)f_hdl_main_pcie,		D_RRH_STACKSIZE_DEFAULT	}	/**< Handler IRQ52 Thread(PCIe断)			*/
};


/**
 * CMU recv paramater thread create manage table
 * @date 2015/08/12 TDI)satou D_API_MSGID_DBG_SIMUAMPCHG_NTCを削除
 * @date 2015/10/06 TDI)satou D_API_MSGID_CPRI_LINK_STOP_INDを追加
 * @date 2015/10/19 TDI)satou D_API_MSGID_SVP_ALL_RE_L1RESET_INDを追加
 * @date 2015/10/19 TDI)satou D_API_MSGID_SVP_RE_L1RESET_INDを追加
 * @date 2015/10/30 TDI)satou その他未実装-002 D_API_MSGID_SVP_SET_WAVE_INDを追加
 * @date 2015/11/03 TDI)satou その他未実装-003 D_API_MSGID_SVP_SET_AxC_INDを追加
 * @date 2015/11/04 TDI)satou その他未実装-008 D_API_MSGID_CPRI_DLY_ADJ_STEP3_REQを追加
 * @date 2015/11/04 TDI)satou その他未実装-008 D_API_MSGID_CPRI_DLY_ADJ_STEP7_REQを追加
 * @date 2015/11/10 TDI)satou その他未実装-017 D_API_MSGID_CPRI_DLY_ADJ_STEP10_REQを追加
 */
const T_CMU_CREATE_THREAD	f_mainr_cmusRecvThdCreMng	= {
	{												/* メッセージ送信先情報管理テーブル */
		/* EventID */						/* SignalKind */	/* 送信先QID */			/* dummy */
		{	D_SYS_MSGID_ALLTSKININTC,			0x00000000	,	D_SYS_THDQID_PF_F_MAIN,	0x00000000		},
		/** pf_cpr **/
		{	D_API_MSGID_CPRI_LINK_STARTUP_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_LINK_ENABLE_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_LINK_DISABLE_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_STATE_SUB_REQ,		0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_STATE_UNSUB_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_PARAM_UPDATE_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_CPRI_FIBEROUTPU_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{   D_API_MSGID_CPRI_LINK_STOP_IND,     0x00000000  ,   D_SYS_THDQID_PF_CPRI,   0x00000000      },
		{   D_API_MSGID_CPRI_DLY_ADJ_STEP3_REQ, 0x00000000  ,   D_SYS_THDQID_PF_CPRI,   0x00000000      },
		{	D_API_MSGID_CPRI_DLY_ADJ_STEP7_REQ,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{   D_API_MSGID_CPRI_DLY_ADJ_STEP10_REQ,0x00000000  ,   D_SYS_THDQID_PF_CPRI,   0x00000000      },
		{	D_API_MSGID_CPRI_CPMPOWERON_IND,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		/** pf_fmng **/
		{	D_API_MSGID_FILE_FIRM_VERSION_REQ,	0x00000000	,	D_SYS_THDQID_PF_FRMG,	0x00000000		},
		{	D_API_MSGID_FILE_FIRM_ERASE_REQ,	0x00000000	,	D_SYS_THDQID_PF_FRMG,	0x00000000		},
		{	D_API_MSGID_FILE_FIRM_UPDATE_REQ,	0x00000000	,	D_SYS_THDQID_PF_FRMG,	0x00000000		},
		{	D_API_MSGID_FILE_FIRM_CANCEL_IND,	0x00000000	,	D_SYS_THDQID_PF_FRMG,	0x00000000		},
		{	D_API_MSGID_FILE_FIRM_READ_REQ,		0x00000000	,	D_SYS_THDQID_PF_FRMG,	0x00000000		},
		/** pf_rsv **/
		{	D_API_MSGID_SVP_STATE_SUB_REQ,		0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_STATE_UNSUB_IND,	0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_CARD_CONCTL_IND,	0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_LED_SET_IND,		0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_DATETIME_SET_IND,	0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_PORTSTATE_SUB_REQ,	0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_CARD_RESET_IND,		0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_ERRSET_IND,			0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_LED_SET2_IND,		0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{   D_API_MSGID_SVP_ALL_RE_L1RESET_IND, 0x00000000  ,   D_SYS_THDQID_PF_RSV,    0x00000000      },
		{	D_API_MSGID_SVP_RE_L1RESET_IND,	    0x00000000	,	D_SYS_THDQID_PF_RSV,	0x00000000		},
		{	D_API_MSGID_SVP_SVREG_SET_IND,		0x00000000  ,   D_SYS_THDQID_PF_RSV,    0x00000000      },
		{	D_API_MSGID_SVP_SET_WAVE_IND,       0x00000000  ,   D_SYS_THDQID_PF_RSV,    0x00000000      },
		{	D_API_MSGID_SVP_SET_AxC_IND,		0x00000000	,	D_SYS_THDQID_PF_RSV,    0x00000000      },
		/** pf_eth **/
		{	D_API_MSGID_DBG_IFCONFIG_REQ,		0x00000000	,	D_SYS_THDQID_PF_ETH,	0x00000000		},
		/** pf_dbg **/
		{   CMD_TSKIF_DBG_EXEC_CMD_REQ,   		0x00000000	,   D_SYS_THDQID_PF_DBG,    0x00000000      },
		{	D_API_MSGID_CPRI_LINK_SETUP_DEBUG,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	D_API_MSGID_NEGO_BITRATERESULT_DBG,	0x00000000	,	D_SYS_THDQID_PF_NEGO,	0x00000000		},
		{	D_API_MSGID_CPRI_DLY_ADJ_CLEAR_REQ,	0x00000000	,	D_SYS_THDQID_PF_CPRI,	0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		}

	},
	D_RRH_PROCID_F_PF,								/* プロセスID */
	D_RRH_PROCQUE_F_PF,								/* プロセスQID */
	D_SYS_THDID_PF_RECV,							/* Thread-ID */
	D_SYS_THDQID_PF_RECV,							/* Thread-QID */
	D_SYS_THDQID_PF_F_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_RECV_ID,								/* スレッド情報(0:recv, 1:send) */
	"pf_recv"										/* スレッド名	 */
};

/** CMU send paramater thread create manage table */
const T_CMU_CREATE_THREAD	f_mainr_cmusSendThdCreMng	= {
	{												/* メッセージ送信先情報管理テーブル */
		/* EventID */	/* SignalKind */	/* 送信先QID */	/* dummy */
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		},
		{	0x00000000	,	0x00000000	,	0x00000000	,	0x00000000		}

	},
	D_RRH_PROCID_F_PF,								/* プロセスID */
	D_RRH_PROCQUE_F_PF,								/* プロセスQID */
	D_SYS_THDID_PF_SEND,							/* Thread-ID */
	D_SYS_THDQID_PF_SEND,							/* Thread-QID */
	D_SYS_THDQID_PF_F_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_SEND_ID,								/* スレッド情報(0:recv, 1:send) */
	"pf_send"										/* スレッド名	 */
};

/** debug thread execute information */
const T_DBGTHREAD_EXEC_INFO	f_mainr_debugparam =
{
	D_RRH_PROCID_F_PF,								/* 起動元プロセスID						*/
	D_RRH_PROCQUE_F_PF,								/* 起動元プロセスメッセージQID			*/
	D_SYS_THDQID_PF_F_MAIN,							/* 起動元 pf_main スレッドQID			*/
	"pf_dbg",										/* デバッグスレッド名					*/
	D_SYS_THDID_PF_DBG,								/* デバッグスレッドのスレッドID			*/
	D_SYS_THDQID_PF_DBG,							/* デバッグスレッドのメッセージQID		*/
	NULL											/* ユーザー定義関数						*/
};

/** Invent Info エンディアン変換情報 */
const UCHAR f_mainr_invShortInfo[D_RRH_I2C_EEP_DATANUM/2] =
{
/*  0  2  4  6  8  a  c  e  */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0000 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0010 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0020 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0030 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0040 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0050 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0060 ～ */
	0, 1, 1, 1, 0, 0, 0, 0, /* addr 0070 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0080 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0090 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00b0 ～ */
	0, 0, 0, 0,	0, 0, 0, 0, /* addr 00c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0100 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0110 ～ */
	1, 1, 1, 1, 1, 0, 1, 1, /* addr 0120 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0130 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0140 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0150 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0160 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0170 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0180 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0190 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0200 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0210 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0220 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0230 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0240 ～ */
	0, 0, 0, 0,	0, 0, 0, 0, /* addr 0250 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0260 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0270 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0280 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0290 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0300 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0310 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0320 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0330 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0340 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0350 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0360 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0370 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0380 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0390 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0  /* addr 03f0 ～ */
};
/** @} */
