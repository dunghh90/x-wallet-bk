/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_rom.c
 *  @brief  CPRI management ROM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2015/07/27 TDIPS)ikeda * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI 管理 header file */
#include "f_rrh_def.h"

/**
 * @brief global evet code and task internal event code conversion table
 * @date  2015/10/06 TDI)satou CMD_TSKIF_RE_CPRI_STOP_NTC, CRD_INTEREV_RE_CPRI_STOPを追加
 */
CRT_EVENT_CTBL    const crr_event_ctbl[] =
{
	{	CMD_TSKIF_INIREQ,					CRD_INTEREV_INITREQ				},
	{	CMD_TSKIF_REC_CPRI_PRE_REQ,			CRD_INTEREV_RECCPRIPREREQ		},
	{	CMD_TSKIF_CPRISTACHGNTC,			CRD_INTEREV_CPRISTACHG			},
	{	CMD_TSKIF_RE_CPRI_START_NTC,		CRD_INTEREV_RE_CPRI_START		},
	{	CMD_TSKIF_RECMPSTPNTC,				CRD_INTEREV_RECMPSTP			},
	{	CMD_TSKIF_L2STANTC,					CRD_INTEREV_LAPBLNKCHGNTC		},
	{	CMD_TSKIF_REC_CPRI_START_NTC,		CRD_INTEREV_RECCPRISTART		},
	{	CMD_TSKIF_CPRIRCVNTC,				CRD_INTEREV_CPRIIFRCV			},
	{	CMD_TSKIF_TIMOUTNTC,				CRD_INTEREV_TIMEOUT				},
	{	CMD_TSKIF_TIMOUTRSPNTC,				CRD_INTEREV_TIMEOUT_RSP			},
	{	CMD_TSKIF_TIMOUTSNDNTC,				CRD_INTEREV_TIMEOUT_SND			},
	{	CMD_TSKIF_CPRIDBGSTARTREQ,			CRD_INTEREV_CPRIDBGSTART		},
	{	CMD_TSKIF_CPRIDBGSTOPREQ,			CRD_INTEREV_CPRIDBGSTOP			},
	{	CMD_TSKIF_CPRIMONISTARTIND,			CRD_INTEREV_CPRIMONISTART		},
	{	CMD_TSKIF_CPRIMONISTOPIND,			CRD_INTEREV_CPRIMONISTOP		},
	{	CMD_TSKIF_CPRIDSCSTNTC,				CRD_INTEREV_CPRIDSCSTNTC		},
	{   CMD_TSKIF_RE_CPRI_STOP_NTC,         CRD_INTEREV_RE_CPRI_STOP        },
	{	CRD_ECTBL_END,						CRD_ECTBL_END					}
};

/** main matrix table */
void    (* const crr_mtrxtbl[CRD_MX_STATE][CRD_MX_EVENT]) (UINT *buff_adr) =
{
	/* 0: 初期化 state */
	{	m_cr_Init,    					/* 初期化要求 */
		m_cr_Nop,						/* REC CPRI Link Preparation Request */
		m_cr_Nop,						/* CPRI State Change Interrupt Notice */
		m_cr_Nop,						/* RE CPRI Link Start Notice */
		m_cr_Nop,						/* RE force stop notify */
		m_cr_Nop,						/* LAPB link state notice */
		m_cr_Nop,						/* REC CPRI Link Start Notice */
		m_cr_CpriMsgRcvProc, // 擬似環境 仮 20210310 m_cr_Nop,						/* CPRI signal receiving notice */
		m_cr_Nop,						/* Time out notice */
		m_cr_Nop,						/* Time out notice re rsp */
		m_cr_Nop,						/* Time out notice re snd */
		m_cr_Nop,						/* CPRI Simulator start request */
		m_cr_Nop,						/* CPRI Simulator stop request */
		m_cr_Nop,						/* CPRI monitor start indication */
		m_cr_Nop,						/* CPRI monitor stop indication */
		m_cr_Nop,						/* Time out notice cpri dsc */
		m_cr_Nop,                       /* RE CPRI Link Stop Notice */
	},

	/* 1: REC Startup待機 */
	{	m_cr_Nop,						/* 初期化要求 */
		m_cr_CpriLinkPrepareReq,		/* REC CPRI Link Preparation Request */
		m_cr_Nop,						/* CPRI State Change Interrupt Notice */
		m_cr_Nop,						/* RE CPRI Link Start Notice */
		m_cr_Nop,						/* RE force stop notify */
		m_cr_Nop,						/* LAPB link state notice */
		m_cr_Nop,						/* REC CPRI Link Start Notice */
		m_cr_CpriMsgRcvProc, // 擬似環境 仮 20210310 m_cr_Nop,						/* CPRI signal receiving notice */
		m_cr_Nop,						/* Time out notice */
		m_cr_Nop,						/* Time out notice re rsp */
		m_cr_Nop,						/* Time out notice re snd */
		m_cr_Nop,						/* CPRI Simulator start request */
		m_cr_Nop,						/* CPRI Simulator stop request */
		m_cr_Nop,						/* CPRI monitor start indication */
		m_cr_Nop,						/* CPRI monitor stop indication */
		m_cr_Nop,						/* Time out notice cpri dsc */
		m_cr_Nop,                       /* RE CPRI Link Stop Notice */
	},

	/* 2: REC HDLC active待ち */
	{	m_cr_Nop,						/* 初期化要求 */
		m_cr_Nop,						/* REC CPRI Link Preparation Request */
		m_cr_CpriStateChgProc,			/* CPRI State Change Interrupt Notice */
		m_cr_Nop,						/* RE CPRI Link Start Notice */
		m_cr_Nop,						/* RE force stop notify */
		m_cr_Nop2,						/* LAPB link state notice */
		m_cr_Nop,						/* REC CPRI Link Start Notice */
		m_cr_CpriMsgRcvProc, // 擬似環境 仮 20210310 m_cr_Nop,						/* CPRI signal receiving notice */
		m_cr_Nop2,						/* Time out notice */
		m_cr_Nop2,						/* Time out notice re rsp */
		m_cr_Nop2,						/* Time out notice re snd */
		m_cr_Nop,						/* CPRI Simulator start request */
		m_cr_Nop,						/* CPRI Simulator stop request */
		m_cr_Nop,						/* CPRI monitor start indication */
		m_cr_Nop,						/* CPRI monitor stop indication */
		m_cr_Nop2,						/* Time out notice cpri dsc */
		m_cr_Nop,                       /* RE CPRI Link Stop Notice */
	},

	/* 3: 配下RE 起動中 */
	{	m_cr_Nop,						/* 初期化要求 */
		m_cr_Nop,						/* REC CPRI Link Preparation Request */
		m_cr_CpriStateChgProc,			/* CPRI State Change Interrupt Notice */
		m_cr_CpriStartReNotice,			/* RE CPRI Link Start Notice */
		t_cr_forceStopProc,				/* RE force stop notify */
		m_cr_L2LinkStateProc2,			/* LAPB link state notice */
		m_cr_CpriStartRecNotice,		/* REC CPRI Link Start Notice */
		m_cr_CpriMsgRcvProc2,			/* CPRI signal receiving notice */
		m_cr_Nop,						/* Time out notice */
		m_cr_HcTimeoutNtcRSP,			/* Time out notice re rsp */
		m_cr_HcTimeoutNtcSND,			/* Time out notice re snd */
		t_cr_CpriSimStartProc,			/* CPRI Simulator start request */
		t_cr_CpriSimStopProc,			/* CPRI Simulator stop request */
		t_cr_CpriMonitorStrProc,		/* CPRI monitor start indication */
		t_cr_CpriMonitorEndProc,		/* CPRI monitor stop indication */
		m_cr_CpriDscstTimeoutNtc,		/* Time out notice cpri dsc */
		m_cr_Nop,                       /* RE CPRI Link Stop Notice */
	},

	/* 4: 運用中 */
	{	m_cr_Nop,						/* 初期化要求 */
		m_cr_Nop,						/* REC CPRI Link Preparation Request */
		m_cr_CpriStateChgProc,			/* CPRI State Change Interrupt Notice */
		m_cr_CpriStartReNotice,			/* RE CPRI Link Start Notice */
		t_cr_forceStopProc,				/* RE force stop notify */
		m_cr_L2LinkStateProc,			/* LAPB link state notice */
		m_cr_Nop,						/* REC CPRI Link Start Notice */
		m_cr_CpriMsgRcvProc,			/* CPRI signal receiving notice */
		m_cr_HcTimeoutNtc,				/* Time out notice */
		m_cr_HcTimeoutNtcRSP,			/* Time out notice re rsp */
		m_cr_HcTimeoutNtcSND,			/* Time out notice re snd */
		t_cr_CpriSimStartProc,			/* CPRI Simulator start request */
		t_cr_CpriSimStopProc,			/* CPRI Simulator stop request */
		t_cr_CpriMonitorStrProc,		/* CPRI monitor start indication */
		t_cr_CpriMonitorEndProc,		/* CPRI monitor stop indication */
		m_cr_CpriDscstTimeoutNtc,		/* Time out notice cpri dsc */
		m_cr_CpriStopReNotice,          /* RE CPRI Link Stop Notice */
	}
};

/** CPRI link state priority table */
CRT_CPRISTAPRIO    const crr_cprilnksta_priotbl[CRD_MX_CPRILNKSTA][CRD_MX_LINKEV] = 
{
	/* current link state is CRD_CPRILNKSTA_NO0 */
	{ {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO1}, 	    /* CPRI state change (state E down -> state E) */
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO2}, 		/* CPRI state change (HDLC ACT detection)  	 */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0},  	    /* CPRI state change (state E over -> state E down) */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0}, 		/* CPRI state change (HDLC IDLE detection) */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0}, 		/* TX abnormal  */
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0},  		/* L2 data link release indication */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0},  		/* L3 health check OK	 */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0} 		/* L3 health check disconnection */
	},

	/* current link state is CRD_CPRILNKSTA_NO1 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1},	    /* CPRI state change (state E down -> state E) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO2},		/* CPRI state change (HDLC ACT detection)  	 */
	  {CRD_STAPRIO_SAME, CRD_CPRILNKSTA_NO0},	    /* CPRI state change (state E over -> state E down) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1},		/* CPRI state change (HDLC IDLE detection) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1},		/* TX abnormal  */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1}, 		/* L2 data link release indication */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1}, 		/* L3 health check OK	 */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO1} 		/* L3 health check disconnection */
	},
	  
	/* current link state is CRD_CPRILNKSTA_NO2 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO2},	    /* CPRI state change (state E down -> state E) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO2},		/* CPRI state change (HDLC ACT detection)  	 */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO0}, 	    /* CPRI state change (state E over -> state E down) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO1},		/* CPRI state change (HDLC IDLE detection) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO3},		/* TX abnormal  */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_SAME, CRD_CPRILNKSTA_NO2}, 		/* L2 data link release indication */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO2}, 		/* L3 health check OK	 */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO2} 		/* L3 health check disconnection */
	},
	  
	/* current link state is CRD_CPRILNKSTA_NO3 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3},	    /* CPRI state change (state E down -> state E) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3},		/* CPRI state change (HDLC ACT detection)  	 */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO0},	    /* CPRI state change (state E over -> state E down) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO1},		/* CPRI state change (HDLC IDLE detection) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3},		/* TX abnormal  */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3}, 		/* L2 data link release indication */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3}, 		/* L3 health check OK	 */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3} 		/* L3 health check disconnection */
	},
	  
	/* current link state is CRD_CPRILNKSTA_NO4 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO4},	    /* CPRI state change (state E down -> state E) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4},		/* CPRI state change (HDLC ACT detection)  	 */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO0}, 	    /* CPRI state change (state E over -> state E down) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO1},		/* CPRI state change (HDLC IDLE detection) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO3},		/* TX abnormal  */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO2},		/* L2 data link release indication */
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO5}, 		/* L3 health check OK	 */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO4} 		/* L3 health check disconnection */
	},
	  
	/* current link state is CRD_CPRILNKSTA_NO5 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO5},	    /* CPRI state change (state E down -> state E) */ 
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO5},		/* CPRI state change (HDLC ACT detection)  	 */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO0}, 	    /* CPRI state change (state E over -> state E down) */
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO1},		/* CPRI state change (HDLC IDLE detection) */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO3},		/* TX abnormal  */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO5}, 		/* L2 data link establishment indication  */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO2},		/* L2 data link release indication */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO5}, 		/* L3 health check OK	 */ 
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO4}		/* L3 health check disconnection */ 
	},

	/* current link state is CRD_CPRILNKSTA_NO6 */
	{ {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO0},	    /* CPRI state change (state E down -> state E) */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6},		/* CPRI state change (HDLC ACT detection)  	 */
	  {CRD_STAPRIO_HIGH, CRD_CPRILNKSTA_NO0}, 	    /* CPRI state change (state E over -> state E down) */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6},		/* CPRI state change (HDLC IDLE detection) */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO3},		/* TX abnormal  */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6}, 		/* L2 data link establishment indication  */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6},		/* L2 data link release indication */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6}, 		/* L3 health check OK	 */
	  {CRD_STAPRIO_LOW, CRD_CPRILNKSTA_NO6}			/* L3 health check disconnection */
	}
};

/** CPRI link state table*/
CRT_CPRILNKSTA	const crr_cprilnksta_class_tbl[CRD_MX_CPRILNKSTA] =
{
    /* +------------------+---------------+---------------+---------------+--------------+ */
    /* | CPRI State       | HDLC          | StateF        | L2リンク      | L3           | */
    /* |                  | idle/active   | flag          | 異常情報      | health check | */
    /* |                  | flag          |               |               | state        | */
    /* +------------------+---------------+---------------+---------------+--------------+ */
	{	CMD_NUM0,			CMD_OFF,        CMD_OFF,        CMD_ON,	        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_OFF,        CMD_OFF,        CMD_ON,	        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_ON,	        CMD_OFF,        CMD_OFF,        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_ON,	        CMD_OFF,        CMD_ON,	        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_ON,	        CMD_ON,	        CMD_OFF,        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_ON,	        CMD_ON,	        CMD_OFF,        CMD_ON	     },
	{	D_RRH_CPRISTAT_E,	CMD_ON,	        CMD_ON,	        CMD_OFF,        CMD_OFF	     },
	{	D_RRH_CPRISTAT_E,	CMD_OFF,	    CMD_OFF,	    CMD_OFF,        CMD_OFF	     }
};

/** health check response confirm timer ID for each system */
const UINT crr_hctimno[CMD_MAX_SYSNUM] = {		
	CMD_TIMID_HCREQ_CNF_3G,					/* 3G health check timer */
	CMD_TIMID_HCREQ_CNF_S3G				/*  S3G health check timer */	
};

/** L3 carrier max value */
const	USHORT	crr_l3carriermax[CMD_MAX_SYSNUM] = {
				CMD_MAX_CAR_3G,		/* 3G */
				CMD_MAX_CAR_LTE		/* S3G */
			};

/* @} */

