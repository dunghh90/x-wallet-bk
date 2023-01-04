/*!
 * @skip		$ld:$
 * @file		f_main_rom.c
 * @brief		pf_main ROMテーブル
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date 		2015/05/27 ALPHA) murakami ハソ改版(M-RRU-ZSYS-01582)対応
 * @date  		2015/06/09 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq
 * @date 		2018/06/22 Fujitsu)吉田 5G-DU割込み処理追加
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2018
 */

#include	"f_main_inc.h"
#include	"f_mpsw_msg_MSI.h"
#include	"f_mpsw_msg_o-ran_uplane_conf_def.h"
#include	"f_mpsw_msg_o-ran_sync_def.h"
#include	"f_mpsw_msg_o-ran_processing_element_def.h"
#include	"f_mpsw_msg_o-ran_usermgmt_def.h"
#include	"f_mpsw_msg_o-ran_mplane_int_def.h"
#include	"f_mpsw_msg_o-ran_interface_def.h"
#include	"f_mpsw_msg_o-ran_dhcp_def.h"
#include	"f_mpsw_msg_ietf_ip_def.h"
#include	"f_mpsw_msg_ietf_interfaces_def.h"
#include	"f_mpsw_msg_ready_def.h"
#include	"f_mpsw_msg_o-ran_operations_def.h"
#include	"f_mpsw_msg_o-ran_hardware_def.h"
#include	"f_mpsw_msg_ietf_hardware_def.h"
#include	"f_mpsw_msg_subscribe_def.h"
#include	"f_mpsw_msg_o-ran_troubleshooting_def.h"
#include	"f_mpsw_msg_o-ran_fm_def.h"
#include	"f_mpsw_msg_o-ran_lbm_def.h"
#include	"f_mpsw_msg_o-ran_performance_management_def.h"
#include	"f_mpsw_msg_o-ran_software_management_def.h"
#include	"f_mpsw_msg_o-ran_transceiver_def.h"
#include	"f_mpsw_msg_o-ran_file_management_def.h"
#include	"f_mpsw_msg_o-ran_externalio_def.h"
#include	"f_mpsw_msg_o-ran_fan_def.h"
#include	"f_mpsw_msg_o-ran_supervison_def.h"
#include	"f_mpsw_msg_o-ran_ald_def.h"
#include	"f_mpsw_msg_o-ran_ald_port_def.h"

/** @addtogroup RRH_PF_MAIN
 *  @{
 */

/* Hdl_main paramater thread create manage table	*/
const T_HDL_CRT_EVENT_CTBL	f_hdlr_thdCreMng[D_RRH_HDLTHDNO_MAX]	= {
	{D_RRH_THDNAME_HD1PPS,	D_RRH_PRIORITY_HD1PPS,	(FUNCPTR)f_hdl_main_1pps,			D_RRH_STACKSIZE_DEFAULT},	/**< Handler Thread = IRQ_123:1pps Timing	*/
	{D_RRH_THDNAME_HD1S,	D_RRH_PRIORITY_HD1S,	(FUNCPTR)f_hdl_main_1s,				D_RRH_STACKSIZE_DEFAULT},	/**< Handler Thread = IRQ_122:1s周期		*/
	{D_RRH_THDNAME_HDSV,	D_RRH_PRIORITY_HDSV,	(FUNCPTR)f_hdl_main_sv,				D_RRH_STACKSIZE_DEFAULT},	/**< Handler Thread = IRQ_124:SV状変		*/
	{D_RRH_THDNAME_HDWDT,	D_RRH_PRIORITY_HDWDT,	(FUNCPTR)f_hdl_main_wdt,			D_RRH_STACKSIZE_DEFAULT},	/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
	{D_RRH_THDNAME_HDMSTA1,	D_RRH_PRIORITY_HDMSTA1,	(FUNCPTR)f_hdl_main_state_fheth1,	D_RRH_STACKSIZE_DEFAULT},	/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
	{D_RRH_THDNAME_HDLALD,	D_RRH_PRIORITY_HDLALD,	(FUNCPTR)f_hdl_main_ald,			D_RRH_STACKSIZE_DEFAULT}	/**< Handler Thread = IRQ_127:ALD(AISG)		*/
};

/* CMU recv paramater thread create manage table */
const T_CMU_CREATE_THREAD	f_mainr_cmusRecvThdCreMng	= {
	{												/* メッセージ送信先情報管理テーブル */
		/* EventID */						/* SignalKind */	/* 送信先QID */			/* dummy */
		{	D_SYS_MSGID_ALLTSKININTC,			0x00000000	,	D_SYS_THDQID_PF_MAIN,	0x00000000		},
		{   CMD_TSKIF_DBG_EXEC_CMD_REQ,   		0x00000000	,   D_SYS_THDQID_PF_DBG,    0x00000000      },
// 擬似環境 20210309
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
// 擬似環境 20210309
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
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		}
	},
	D_RRH_PROCID_PF,								/* プロセスID */
	D_RRH_PROCQUE_PF,								/* プロセスQID */
	D_SYS_THDID_PF_RECV,							/* Thread-ID */
	D_SYS_THDQID_PF_RECV,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_RECV_ID,								/* スレッド情報(0:recv, 1:send) */
	"pf_recv"										/* スレッド名	 */
};

/* CMU send paramater thread create manage table */
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
	D_RRH_PROCID_PF,								/* プロセスID */
	D_RRH_PROCQUE_PF,								/* プロセスQID */
	D_SYS_THDID_PF_SEND,							/* Thread-ID */
	D_SYS_THDQID_PF_SEND,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_SEND_ID,								/* スレッド情報(0:recv, 1:send) */
	"pf_send"										/* スレッド名	 */
};


/* CMU recv paramater thread create manage table */
const T_CMU_CREATE_THREAD	f_mainr_cmusRecvThdCreMng_ptp	= {
	{												/* メッセージ送信先情報管理テーブル */
		/* EventID */						/* SignalKind */	/* 送信先QID */			/* dummy */
		{	D_SYS_MSGID_ALLTSKININTC,			0x00000000	,	D_SYS_THDQID_PF_MAIN,	0x00000000		},
		{   CMD_TSKIF_DBG_EXEC_CMD_REQ,   		0x00000000	,   D_SYS_THDQID_PF_DBG,    0x00000000      },
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
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		},
		{	0x00000000	,						0x00000000	,	0x00000000	,			0x00000000		}
	},
	D_RRH_PROCID_PTP,								/* プロセスID */
	D_RRH_PROCQUE_PTP,								/* プロセスQID */
	D_SYS_THDID_PF_RECV,							/* Thread-ID */
	D_SYS_THDQID_PF_RECV,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_RECV_ID,								/* スレッド情報(0:recv, 1:send) */
	"ptp_recv"										/* スレッド名	 */
};

/* CMU send paramater thread create manage table */
const T_CMU_CREATE_THREAD	f_mainr_cmusSendThdCreMng_ptp	= {
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
	D_RRH_PROCID_PTP,								/* プロセスID */
	D_RRH_PROCQUE_PTP,								/* プロセスQID */
	D_SYS_THDID_PF_SEND,							/* Thread-ID */
	D_SYS_THDQID_PF_SEND,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	E_RRH_CMU_SEND_ID,								/* スレッド情報(0:recv, 1:send) */
	"ptp_send"										/* スレッド名	 */
};

UINT	f_mainr_diagBit_tdd			= D_SYS_DIA_1ST_TDD;

const T_DBGTHREAD_EXEC_INFO	f_mainr_debugparam =
{
	D_RRH_PROCID_PF,								/* 起動元プロセスID						*/
	D_RRH_PROCQUE_PF,								/* 起動元プロセスメッセージQID			*/
	D_SYS_THDQID_PF_MAIN,							/* 起動元 pf_main スレッドQID			*/
	"pf_dbg",										/* デバッグスレッド名					*/
	D_SYS_THDID_PF_DBG,								/* デバッグスレッドのスレッドID			*/
	D_SYS_THDQID_PF_DBG,							/* デバッグスレッドのメッセージQID		*/
	NULL											/* ユーザー定義関数						*/
};

const T_DBGTHREAD_EXEC_INFO	f_mainr_debugparam_ptp =
{
	D_RRH_PROCID_PTP,								/* 起動元プロセスID						*/
	D_RRH_PROCQUE_PTP,								/* 起動元プロセスメッセージQID			*/
	D_SYS_THDQID_PF_MAIN,							/* 起動元 pf_main スレッドQID			*/
	"ptp_dbg",										/* デバッグスレッド名					*/
	D_SYS_THDID_PF_DBG,								/* デバッグスレッドのスレッドID			*/
	D_SYS_THDQID_PF_DBG,							/* デバッグスレッドのメッセージQID		*/
	NULL											/* ユーザー定義関数						*/
};


/* Flashログ展開情報 */
/* Invent Info エンディアン変換情報 */
const UCHAR f_mainr_invShortInfo[D_RRH_I2C_EEP_DATANUM/4] =
{
/* CHARエリア:0 SHORTエリア:1 */
/*  0  2  4  6  8  a  c  e  */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0000 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0010 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0020 ～ */
	1, 0, 0, 0, 0, 0, 0, 0, /* addr 0030 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0040 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0050 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0060 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0070 ～ */
	1, 1, 0, 0, 0, 0, 0, 0, /* addr 0080 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0090 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00a0 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 00b0 ～ */
	1, 1, 0, 0,	0, 0, 0, 0, /* addr 00c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0100 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0110 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0120 ～ */
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
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0200 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0210 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0220 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0230 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0240 ～ */
	0, 0, 0, 0,	0, 0, 0, 0, /* addr 0250 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0260 ～ */
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
	1, 1, 1, 0, 0, 0, 0, 1, /* addr 0320 ～ */
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
	0, 1, 1, 0, 0, 0, 0, 0, /* addr 03e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0  /* addr 03f0 ～ */
};
/** @} */

/* SFPタイプ情報 */
const T_MAIN_SFP_TYPE_TBL f_mainr_sfpType[E_COM_SFP_TYPE_NUM] =
{
	/* SFP図番確認出来ない異常ケース */
	{ "",				{'C'},	{0,0},	D_SYS_TRX_INF_TYP_ABNORMAL },
	/* 未サポート */
	{ "",				{'C'},	{0,0},	D_SYS_TRX_INF_TYP_UNSUPT },
	/* Cタイプ */
	{ "CA46860-1348",	{'C'},	{0,0},	D_SYS_TRX_INF_TYP_C },
	/* Dタイプ */
	{ "CA46860-1350",	{'D'},	{0,0},	D_SYS_TRX_INF_TYP_D },
	/* 短距離 */
	{ "CA46860-1385",	{'C'},	{0,0},	D_SYS_TRX_INF_TYP_SHORT },	/* 短距離Red   */
	{ "CA46860-1386",	{'C'},	{0,0},	D_SYS_TRX_INF_TYP_SHORT }	/* 短距離Black */
};



/* CMU mp proxy paramater thread create manage table for DU_PF */
T_CMU_CREATE_THREAD_MP	f_mainr_cmusRecvThdCreMng_mp_pf= {
	43,																/* MSI登録数 */
	{
		/* MSI_msgID */												/* 送信先QID */
		{	D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ,			D_SYS_THDQID_PF_RFA		},
		{	D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ,					D_SYS_THDQID_PF_RFA		},
		{	D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ,				D_SYS_THDQID_PF_RFA		},
		{	D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ,				D_SYS_THDQID_PF_RFA		},
		{	D_MSG_MSI_IETF_INTERFACES_GET_REQ,						D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_IETF_IP_EDIT_REQ,								D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ,						D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ,					D_SYS_THDQID_PF_DHC		},
		{	D_MSG_MSI_O_RAN_MPLANE_INT_GET_REQ,						D_SYS_THDQID_PF_DHC		},
		{	D_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ,						D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_O_RAN_DHCP_GET_REQ,							D_SYS_THDQID_PF_DHC		},
		{	D_MSG_MSI_IETF_HARDWARE_GET_REQ,						D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ,					D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_HARDWARE_GET_REQ,						D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_OPERATIONS_GET_REQ,						D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_OPERATIONS_EDIT_REQ,					D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ,					D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_LBM_EDIT_REQ,							D_SYS_THDQID_PF_EOM		},
		{	D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ,						D_SYS_THDQID_PF_PFM		},
		{	D_MSG_MSI_O_RAN_PFM_MNG_GET_REQ,						D_SYS_THDQID_PF_PFM		},
		{	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ,	D_SYS_THDQID_PF_FDL		},
		{	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ,		D_SYS_THDQID_PF_FDL		},
		{	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ,		D_SYS_THDQID_PF_FDL		},
		{	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ,		D_SYS_THDQID_PF_FDL		},
		{	D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ,					D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ,			D_SYS_THDQID_PF_LGC		},
		{	D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ,			D_SYS_THDQID_PF_LGC		},
		{	D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND,			D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_FM_ALARM_START_IND,						D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND,					D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ,				D_SYS_THDQID_PF_FUL		},
		{	D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ,			D_SYS_THDQID_PF_FUL		},
		{	D_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ,						D_SYS_THDQID_PF_EIO		},
		{	D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ,					D_SYS_THDQID_PF_EIO		},
		{	D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND,		D_SYS_THDQID_PF_EIO		},
		{	D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND,		D_SYS_THDQID_PF_EIO		},
		{	D_MSG_MSI_O_RAN_FAN_GET_REQ,							D_SYS_THDQID_PF_EIO		},
		{	D_MSG_MSI_O_RAN_HARDWARE_EDIT_REQ,						D_SYS_THDQID_PF_EQS		},
		{	D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND,				D_SYS_THDQID_PF_FHS		},
		{	D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ,				D_SYS_THDQID_PF_AIH		},
		{	D_MSG_MSI_O_RAN_ALD_PORT_EDIT_REQ,						D_SYS_THDQID_PF_AIH		},
		{	D_MSG_MSI_O_RAN_ALD_PORT_GET_REQ,						D_SYS_THDQID_PF_AIH		},
		{	D_MSG_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND,	D_SYS_THDQID_PF_AIH	},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				},
		{	0x00000000,												0x00000000				}
	},
	65100,											/* portNo */
	"127.0.0.1",									/* IPadderss */
	D_RRH_PROCID_PF,								/* プロセスID */
	D_RRH_PROCQUE_PF,								/* プロセスQID */
	D_SYS_THDID_PF_MPSEND,							/* Thread-ID */
	D_SYS_THDID_PF_MPRECV,							/* Thread-ID */
	D_SYS_THDQID_PF_MPSEND,							/* Thread-QID */
	D_SYS_THDQID_PF_MPRECV,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	"pf_mpxy_send",									/* スレッド名	 */
	"pf_mpxy_recv",									/* スレッド名	 */
	NULL											/* NULL固定		 */
};


/* CMU mp proxy paramater thread create manage table for DU_PTP */
T_CMU_CREATE_THREAD_MP	f_mainr_cmusRecvThdCreMng_mp_ptp= {
	3,										/* MSI登録数 */
	{
		/* MSI_msgID */												/* 送信先QID */
		{	D_MSG_MSI_O_RAN_SYNC_GET_REQ,							D_SYS_THDQID_PTP_MAIN	},
		{	D_MSG_MSI_O_RAN_SYNC_EDIT_REQ,							D_SYS_THDQID_PTP_MAIN	},
		{	D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND	,			D_SYS_THDQID_PTP_MAIN	},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				},
		{	0x00000000	,											0x00000000				}
	},
	65101,											/* portNo */
	"127.0.0.1",									/* IPadderss */
	D_RRH_PROCID_PF,								/* プロセスID */
	D_RRH_PROCQUE_PF,								/* プロセスQID */
	D_SYS_THDID_PF_MPSEND,							/* Thread-ID */
	D_SYS_THDID_PF_MPRECV,							/* Thread-ID */
	D_SYS_THDQID_PF_MPSEND,							/* Thread-QID */
	D_SYS_THDQID_PF_MPRECV,							/* Thread-QID */
	D_SYS_THDQID_PF_MAIN,							/* 起動元 Thrad-QID */
	"ptp_mpxy_send",								/* スレッド名	 */
	"ptp_mpxy_recv",								/* スレッド名	 */
	NULL											/* NULL固定		 */
};



#ifdef OPT_RRE_ZYNQ_SIM


/* thread create manage table	*/
T_MAIN_CRT_EVENT_CTBL	f_mainr_thdCreMng[D_MAIN_THDID_MAX]	= {
	/*	Thread ID,			Scheduling Poricy,			Thread Priority,			Main Function,				Stack Size,						option,									Create Flag,				Abbreviation	*/
	{D_SYS_THDID_PF_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MAIN,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP				 ,	"pf_main"	},	/**< PF Main Thread									*/
	{D_SYS_THDID_PF_RECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RECV,		(FUNCPTR)f_recv_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusRecvThdCreMng,		D_MAIN_CREATE_PTN_NOT_WDT,	"pf_recv"	},	/**< PF Receive Thread(プロセス間受信部)  CMU-RCV	*/
	{D_SYS_THDID_PF_SEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SEND,		(FUNCPTR)f_send_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusSendThdCreMng,		D_MAIN_CREATE_PTN_NOT_WDT,	"pf_send"	},	/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{D_SYS_THDID_PF_DBG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_DBG,			(FUNCPTR)f_dbg_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_debugparam,				D_MAIN_CREATE_PTN_NOT_WDT,	"pf_dbg"	},	/**< dbg Thread										*/
	{D_SYS_THDID_PF_HDLX,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_HDLX,		(FUNCPTR)f_hdl_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_hdlr_thdCreMng[0],			D_MAIN_CREATE_PTN_NOT_WDT,	"pf_hdlx"	},	/**< handle Main Thread								*/
	{D_SYS_THDID_PF_EQS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EQS,			(FUNCPTR)f_eqs_Main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eqs"	},	/**< Equipment Surveillance handler					*/
	{D_SYS_THDID_PF_WDT,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_WDT,			(FUNCPTR)f_wdt_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,	"pf_wdt"	},	/**< WDT handler									*/
	{D_SYS_THDID_PF_NMA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMA,			(FUNCPTR)f_nma_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_nma"	},	/**< Non-volatile Memory Access handler				*/
	{D_SYS_THDID_PF_NMAS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMAS,		(FUNCPTR)f_nmas_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_nmas"	},	/**< Non-volatile Memory Access handler (Slave)		*/
	{D_SYS_THDID_PF_LGC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_LGC,			(FUNCPTR)f_lgc_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_lgc"	},	/**< LoG Collection handler							*/
	{D_SYS_THDID_PF_CCA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_CCA,			(FUNCPTR)f_dpda_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_OTHER,	"pf_cca"	},	/**< Component Carrier Access handler				*/
	{D_SYS_THDID_PF_RFA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RFA,			(FUNCPTR)f_rfcp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_OTHER,	"pf_rfa"	},	/**< RF Access handler								*/
	{D_SYS_THDID_PF_DHC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_DHC,			(FUNCPTR)f_dhcp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_dhc"	},	/**< DHCP handler									*/
	{D_SYS_THDID_PF_SFP,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SFP,			(FUNCPTR)f_sfp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_sfp"	},	/**< SFP Monitor Thread								*/
	{D_SYS_THDID_PTP_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PTP,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_main"	},	/**< PTP Main Thread								*/
	{D_SYS_THDID_PF_FHS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FHS,			(FUNCPTR)f_fhs_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_fhs"	},	/**< FrontHaul Surveillance Thread					*/
	{D_SYS_THDID_PF_MPRECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPRECV,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_mpxy_send"	},	/**< Plane Proxcy Send Thread					*/
	{D_SYS_THDID_PF_MPSEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPSEND,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_mpxy_recv"	},	/**< Plane Proxcy Recv Thread					*/
	{D_SYS_THDID_PF_EOM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM,			(FUNCPTR)f_eom_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eom"	},	/**< Ether O&M main Thread							*/
	{D_SYS_THDID_PF_EOM_0,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_0,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_eom_0"	},	/**< Ether O&M fheth0 Thread						*/
	{D_SYS_THDID_PF_EOM_1,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_1,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_eom_1"	},	/**< Ether O&M fheth1 Thread						*/
	{D_SYS_THDID_PF_FDL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FDL,			(FUNCPTR)f_fdl_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_fdl"	},	/**< File DownLoad Thread							*/
	{D_SYS_THDID_PF_FUL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FUL,			(FUNCPTR)f_ful_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_ful"	},	/**< File UpLoad Thread								*/
	{D_SYS_THDID_PF_PFM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PFM,			(FUNCPTR)f_pfm_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_pfm"	},	/**< Performance Monitor Thread						*/
	{D_SYS_THDID_PF_EIO,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EIO,			(FUNCPTR)f_eio_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eio"	},	/**< eio Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_MTI,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_MTI,			(FUNCPTR)f_mti_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT/*C9 thread start error*/,	"pf_mti"	},	/**< MTI Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_AIH,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_AIH,			(FUNCPTR)f_aih_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_aih"	}	/**< AIH Thread	(Macro-RU)							*/

};

/* thread create manage table	*/
T_MAIN_CRT_EVENT_CTBL	f_mainr_thdCreMng_ptp[D_MAIN_THDID_MAX]	= {
	/*	Thread ID,			Scheduling Poricy,			Thread Priority,			Main Function,				Stack Size,						option,									Create Flag,				Abbreviation	*/
	{D_SYS_THDID_PF_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MAIN,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP				 ,	"ptp_parent"},	/**< PF Main Thread									*/
	{D_SYS_THDID_PF_RECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RECV,		(FUNCPTR)f_recv_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusRecvThdCreMng_ptp,	D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_recv"	},	/**< PF Receive Thread(プロセス間受信部)  CMU-RCV	*/
	{D_SYS_THDID_PF_SEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SEND,		(FUNCPTR)f_send_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusSendThdCreMng_ptp,	D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_send"	},	/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{D_SYS_THDID_PF_DBG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_DBG,			(FUNCPTR)f_dbg_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_debugparam_ptp,			D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_dbg"	},	/**< dbg Thread										*/
	{D_SYS_THDID_PF_HDLX,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_HDLX,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< handle Main Thread								*/
	{D_SYS_THDID_PF_EQS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EQS,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Equipment Surveillance handler					*/
	{D_SYS_THDID_PF_WDT,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_WDT,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< WDT handler									*/
	{D_SYS_THDID_PF_NMA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Non-volatile Memory Access handler				*/
	{D_SYS_THDID_PF_NMAS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMAS,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Non-volatile Memory Access handler (Slave)		*/
	{D_SYS_THDID_PF_LGC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_LGC,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< LoG Collection handler							*/
	{D_SYS_THDID_PF_CCA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_CCA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Component Carrier Access handler				*/
	{D_SYS_THDID_PF_RFA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RFA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< RF Access handler								*/
	{D_SYS_THDID_PF_DHC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_DHC,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< DHCP handler									*/
	{D_SYS_THDID_PF_SFP,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SFP,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< SFP Monitor Thread								*/
	{D_SYS_THDID_PTP_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PTP,			D_RRH_NULL/*(FUNCPTR)f_ptp_main M&C*/,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_main"	},	/**< PTP Main Thread								*/
	{D_SYS_THDID_PF_FHS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FHS,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< FrontHaul Surveillance Thread					*/
	{D_SYS_THDID_PF_MPRECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPRECV,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_mpxy_send"	},	/**< Plane Proxcy Send Thread					*/
	{D_SYS_THDID_PF_MPSEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPSEND,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_mpxy_recv"	},	/**< Plane Proxcy Recv Thread					*/
	{D_SYS_THDID_PF_EOM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M main Thread							*/
	{D_SYS_THDID_PF_EOM_0,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_0,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M fheth0 Thread						*/
	{D_SYS_THDID_PF_EOM_1,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_1,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M fheth1 Thread						*/
	{D_SYS_THDID_PF_FDL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FDL,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< File DownLoad Thread							*/
	{D_SYS_THDID_PF_FUL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FUL,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< File UpLoad Thread								*/
	{D_SYS_THDID_PF_PFM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PFM,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Performance Monitor Thread						*/
	{D_SYS_THDID_PF_EIO,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EIO,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< eio Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_MTI,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_MTI,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< MTI Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_AIH,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_AIH,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	}	/**< AIH Thread	(Macro-RU)							*/

};
#else
/* thread create manage table	*/
T_MAIN_CRT_EVENT_CTBL	f_mainr_thdCreMng[D_MAIN_THDID_MAX]	= {
	/*	Thread ID,			Scheduling Poricy,			Thread Priority,			Main Function,				Stack Size,						option,									Create Flag,				Abbreviation	*/
	{D_SYS_THDID_PF_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MAIN,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP				 ,	"pf_main"	},	/**< PF Main Thread									*/
	{D_SYS_THDID_PF_RECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RECV,		(FUNCPTR)f_recv_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusRecvThdCreMng,		D_MAIN_CREATE_PTN_NOT_WDT,	"pf_recv"	},	/**< PF Receive Thread(プロセス間受信部)  CMU-RCV	*/
	{D_SYS_THDID_PF_SEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SEND,		(FUNCPTR)f_send_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusSendThdCreMng,		D_MAIN_CREATE_PTN_NOT_WDT,	"pf_send"	},	/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{D_SYS_THDID_PF_DBG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_DBG,			(FUNCPTR)f_dbg_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_debugparam,				D_MAIN_CREATE_PTN_NOT_WDT,	"pf_dbg"	},	/**< dbg Thread										*/
	{D_SYS_THDID_PF_HDLX,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_HDLX,		(FUNCPTR)f_hdl_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_hdlr_thdCreMng[0],			D_MAIN_CREATE_PTN_NOT_WDT,	"pf_hdlx"	},	/**< handle Main Thread								*/
	{D_SYS_THDID_PF_EQS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EQS,			(FUNCPTR)f_eqs_Main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eqs"	},	/**< Equipment Surveillance handler					*/
	{D_SYS_THDID_PF_WDT,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_WDT,			(FUNCPTR)f_wdt_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_wdt"	},	/**< WDT handler									*/
	{D_SYS_THDID_PF_NMA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMA,			(FUNCPTR)f_nma_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_nma"	},	/**< Non-volatile Memory Access handler				*/
	{D_SYS_THDID_PF_NMAS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMAS,		(FUNCPTR)f_nmas_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_nmas"	},	/**< Non-volatile Memory Access handler (Slave)		*/
	{D_SYS_THDID_PF_LGC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_LGC,			(FUNCPTR)f_lgc_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_lgc"	},	/**< LoG Collection handler							*/
	{D_SYS_THDID_PF_CCA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_CCA,			(FUNCPTR)f_dpda_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_OTHER,	"pf_cca"	},	/**< Component Carrier Access handler				*/
	{D_SYS_THDID_PF_RFA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RFA,			(FUNCPTR)f_rfcp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_OTHER,	"pf_rfa"	},	/**< RF Access handler								*/
	{D_SYS_THDID_PF_DHC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_DHC,			(FUNCPTR)f_dhcp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_dhc"	},	/**< DHCP handler									*/
	{D_SYS_THDID_PF_SFP,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SFP,			(FUNCPTR)f_sfp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_sfp"	},	/**< SFP Monitor Thread								*/
	{D_SYS_THDID_PTP_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PTP,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_main"	},	/**< PTP Main Thread								*/
	{D_SYS_THDID_PF_FHS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FHS,			(FUNCPTR)f_fhs_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_fhs"	},	/**< FrontHaul Surveillance Thread					*/
	{D_SYS_THDID_PF_MPRECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPRECV,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_mpxy_send"	},	/**< Plane Proxcy Send Thread					*/
	{D_SYS_THDID_PF_MPSEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPSEND,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_mpxy_recv"	},	/**< Plane Proxcy Recv Thread					*/
	{D_SYS_THDID_PF_EOM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM,			(FUNCPTR)f_eom_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eom"	},	/**< Ether O&M main Thread							*/
	{D_SYS_THDID_PF_EOM_0,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_0,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_eom_0"	},	/**< Ether O&M fheth0 Thread						*/
	{D_SYS_THDID_PF_EOM_1,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_1,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"pf_eom_1"	},	/**< Ether O&M fheth1 Thread						*/
	{D_SYS_THDID_PF_FDL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FDL,			(FUNCPTR)f_fdl_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_fdl"	},	/**< File DownLoad Thread							*/
	{D_SYS_THDID_PF_FUL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FUL,			(FUNCPTR)f_ful_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_ful"	},	/**< File UpLoad Thread								*/
	{D_SYS_THDID_PF_PFM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PFM,			(FUNCPTR)f_pfm_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_pfm"	},	/**< Performance Monitor Thread						*/
	{D_SYS_THDID_PF_EIO,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EIO,			(FUNCPTR)f_eio_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_eio"	},	/**< eio Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_MTI,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_MTI,			(FUNCPTR)f_mti_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_mti"	},	/**< MTI Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_AIH,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_AIH,			(FUNCPTR)f_aih_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"pf_aih"	}	/**< AIH Thread	(Macro-RU)							*/

};

/* thread create manage table	*/
T_MAIN_CRT_EVENT_CTBL	f_mainr_thdCreMng_ptp[D_MAIN_THDID_MAX]	= {
	/*	Thread ID,			Scheduling Poricy,			Thread Priority,			Main Function,				Stack Size,						option,									Create Flag,				Abbreviation	*/
	{D_SYS_THDID_PF_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MAIN,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP				 ,	"ptp_parent"},	/**< PF Main Thread									*/
	{D_SYS_THDID_PF_RECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RECV,		(FUNCPTR)f_recv_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusRecvThdCreMng_ptp,	D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_recv"	},	/**< PF Receive Thread(プロセス間受信部)  CMU-RCV	*/
	{D_SYS_THDID_PF_SEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SEND,		(FUNCPTR)f_send_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_cmusSendThdCreMng_ptp,	D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_send"	},	/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{D_SYS_THDID_PF_DBG,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_DBG,			(FUNCPTR)f_dbg_main,		D_RRH_STACKSIZE_DEFAULT,		(VOID*)&f_mainr_debugparam_ptp,			D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_dbg"	},	/**< dbg Thread										*/
	{D_SYS_THDID_PF_HDLX,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_HDLX,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< handle Main Thread								*/
	{D_SYS_THDID_PF_EQS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EQS,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Equipment Surveillance handler					*/
	{D_SYS_THDID_PF_WDT,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_WDT,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< WDT handler									*/
	{D_SYS_THDID_PF_NMA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Non-volatile Memory Access handler				*/
	{D_SYS_THDID_PF_NMAS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_NMAS,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Non-volatile Memory Access handler (Slave)		*/
	{D_SYS_THDID_PF_LGC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_LGC,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< LoG Collection handler							*/
	{D_SYS_THDID_PF_CCA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_CCA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Component Carrier Access handler				*/
	{D_SYS_THDID_PF_RFA,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_RFA,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< RF Access handler								*/
	{D_SYS_THDID_PF_DHC,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_DHC,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< DHCP handler									*/
	{D_SYS_THDID_PF_SFP,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_SFP,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< SFP Monitor Thread								*/
	{D_SYS_THDID_PTP_MAIN,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PTP,			(FUNCPTR)f_ptp_main,		D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_CREATE_PTN_NOT_WDT,	"ptp_main"	},	/**< PTP Main Thread								*/
	{D_SYS_THDID_PF_FHS,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FHS,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< FrontHaul Surveillance Thread					*/
	{D_SYS_THDID_PF_MPRECV,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPRECV,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_mpxy_send"	},	/**< Plane Proxcy Send Thread					*/
	{D_SYS_THDID_PF_MPSEND,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_MPSEND,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_mpxy_recv"	},	/**< Plane Proxcy Recv Thread					*/
	{D_SYS_THDID_PF_EOM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M main Thread							*/
	{D_SYS_THDID_PF_EOM_0,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_0,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M fheth0 Thread						*/
	{D_SYS_THDID_PF_EOM_1,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EOM_1,		D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Ether O&M fheth1 Thread						*/
	{D_SYS_THDID_PF_FDL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FDL,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< File DownLoad Thread							*/
	{D_SYS_THDID_PF_FUL,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_FUL,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< File UpLoad Thread								*/
	{D_SYS_THDID_PF_PFM,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_PFM,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< Performance Monitor Thread						*/
	{D_SYS_THDID_PF_EIO,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_EIO,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< eio Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_MTI,	BPF_RU_ITCM_SCHED_OTHER,	D_RRH_PRIORITY_MTI,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	},	/**< MTI Thread	(5G-商用DU)							*/
	{D_SYS_THDID_PF_AIH,	BPF_RU_ITCM_SCHED_FIFO,		D_RRH_PRIORITY_AIH,			D_RRH_NULL,					D_RRH_STACKSIZE_DEFAULT,		D_RRH_NULL,								D_MAIN_SKIP,				"ptp_none"	}	/**< AIH Thread	(Macro-RU)							*/

};
#endif

/** @} */

