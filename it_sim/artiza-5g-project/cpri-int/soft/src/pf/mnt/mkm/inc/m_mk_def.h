/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_mk_def.h
 *  @brief	 Maker MT manage task Common Definition
 *  @date   2008/07/29 FFCS)zhengmh create
 *  @date   2013/11/25 FFCS)zhaodx modify for zynq
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/



#ifndef M_MK_DEF
#define M_MK_DEF


/** @name	Main matrix table definition											*/
/* @{ */
#define MKD_MX_STATE			4
#ifndef OPT_RRH_ZYNQ_REC
#define MKD_MX_EVENT 			19
#else
#define MKD_MX_EVENT 			20
#endif
/* @} */


/** @name	Task status defination											*/
/* @{ */
#define MKD_ST_INIT     		0        	/**< Init state			*/
#define MKD_ST_TSIREQRXW  		1			/**< TCP server 初期化要求受信待ち*/
#define MKD_ST_TSIW      		2 			/**< TCP server init wait	*/
#define MKD_ST_USE    			3 			/**< Use					*/
/* @} */

/** @name	Task internal event code											*/
/* @{ */
#define MKD_EV_INIREQ			0			/**< 初期化要求					*/
#define MKD_EV_SRVINITREQ		1			/**< TCP Server 初期化要求		*/
#define MKD_EV_TCPSRVINITRSP	2			/**< TCP Server初期化応答			*/
#define MKD_EV_TCPSRVSTPNTC		3			/**< TCP Server Stop Notice */
#define MKD_EV_TCPCONNNTC		4			/**< TCP接続通知					*/
#define MKD_EV_TCPRELNTC		5			/**< TCP解放通知					*/
#define MKD_EV_TCPDATNTC		6			/**< TCP データ受信通知			*/
#define MKD_EV_TCPSNDRSP		7			/**< TCPデータ送信応答			*/
#define MKD_EV_L1MONSNDNTC		8			/**< L1データ送信要求(Monitor)	*/
#define MKD_EV_L1MONRCVNTC		9			/**< L1データ受信通知(Monitor)	*/
#define MKD_EV_L1DATTXREQ		10			/**< L1データ送信要求				*/
#define MKD_EV_CPRISTANTC		11			/**< CPRIリンク state通知			*/
#define MKD_EV_L2STANTC			12			/**< L2リンク state通知			*/
#define MKD_EV_L3STANTC			13			/**< L3リンク state通知			*/
#define MKD_EV_MONDATTO			14			/**< CPRI monitor data timeout	*/
#define MKD_EV_FLDLTIMOUT		15			/**< ファイルダウンロード確認タイマT.O*/
#define MKD_EV_TRARLWRTRSP		16			/**< TRAファイルライト応答			*/
#define MKD_EV_CARDSTANTC		17			/**< Card state通知				*/
#define MKD_EV_SYSPARA_CHG_NTC	18			/**< SystemParemeterChangeNotice	*/

#ifdef OPT_RRH_ZYNQ_REC
#define MKD_EV_CPRIMSGRCV		19			/**< CPRI Message rcv通知				*/
#endif
/* @} */

/** @name	Return value defination											*/
/* @{ */

#define	MKD_RET_NORMAL			0			/**< Normal return		*/
#define	MKD_RET_ABNORMAL   		1			/**< Abnormal return		*/

#define	MKD_RES_MDIFMULTICON	0x1			/**< TRAカードにはRE-MTローカル接続中	*/
#define	MKD_RES_CPRIDOWN		0x3			/**< CPRIリンク断				*/
#define	MKD_RES_REMOTECON		0x4			/**< TRAカードにはRE-MT遠隔接続中		*/
#define	MKD_RES_NOTMDIF			0x5			/**< Connecting card is not TRA card	    */
#define	MKD_RES_REMTDISCMDIF	0x7			/**< RE-MT非接続中（TRA)			    */
#define	MKD_RES_REMTDISC 		0x9			/**< RE-MT非接続						    */
#define	MKD_RES_PARAMERR		0x11		/**< パラメータ指定誤り		*/
#define	MKD_RES_FAILED			0x12		/**< 処理失敗					*/
#define	MKD_RES_TRAUNINS		0x13		/**< TRA 未実装				*/
#define	MKD_RES_KINDERR			0x14		/**< 指定種別誤り				*/
#define	MKD_RES_TIMEOUT			0x15		/**< 処理タイムアウト			*/
#define	MKD_RES_PARITYERR		0x16		/**< パリティエラー			*/
#define	MKD_RES_RWCHKERR		0x17		/**< R/WチェックNG			*/
#define	MKD_RES_PRTCTCNLSETERR	0x18		/**< プロテクト解除設定誤り		*/
#define	MKD_RES_PRTCTSETERR		0x19		/**< プロテクト設定誤り			*/
#define	MKD_RES_FILESIZEERR		0x1A		/**< file size error			*/
#define	MKD_RES_CHKSUM_NG		0x1B		/**< check sum error			*/
#define MKD_RES_SEQNO_NG		0x1C		/**< sequence number error 	*/
#define MKD_RES_TRASTA_NG		0x1D		/**< TRA status error 	*/
#define MKD_RES_DOWNLOAD_NG		0x1E		/**< TRA file download error 	*/


#define	MKD_SHELFPROTECTCANCEL	1			/**< Shelf protect cancel flag*/
#define	MKD_SHELFPROTECT		2			/**< Shelf protect flag*/
#define 	MKD_SHELFEEPROMADDR_MASK	0xFF		/**< EEPROM address mask*/
/* @} */

/** @name	Shelf EEPROM register and Mask											*/
/* @{ */

#define	MKD_MX_MODE				3			/**< 3 modes: RE, simulator and monitor*/

#define	MKD_MX_TCPNO  			3			/**< Max TCP amount		*/

#define	MKD_CONN_Y  			1			/**< In connection		*/
#define	MKD_CONN_N  			0			/**< Not in connection	*/

#define	MKD_TCPHDL_NULL 		-1			/**< TCP handle null		*/

#define	MKD_TCPNO_SIM			0			/**< TCP link no : Simulator	*/
#define	MKD_TCPNO_MON   		1			/**< TCP link no : Monitor	*/
#define	MKD_TCPNO_REMT   		2			/**< TCP link no : RE	*/

#define MKD_TRUE				1			/**< TRUE					*/
#define MKD_FALSE				0			/**< FALSE				*/

#define MKD_RSP_TCPTXOK			1			/**< TCP data tx normal	*/
#define MKD_RSP_TCPTXNG			0			/**< TCP data tx abnormal	*/

#define MKD_PORT_REMT			60029		/**< RE MT port		*/
#define MKD_PORT_MON			60027		/**< Monitor port			*/

#ifndef OPT_RRH_ZYNQ_REC
#define MKD_PORT_MON_RE1		60024		/**< Monitor port (RE1)			*/
#define MKD_PORT_MON_RE2		60025		/**< Monitor port (RE2)			*/
#define MKD_PORT_MON_RE3		60026		/**< Monitor port (RE3)			*/
#else
#define MKD_PORT_MON_RE1		60031		/**< Monitor port (RE1)	*/
#define MKD_PORT_MON_RE2		60032		/**< Monitor port (RE2)	*/
#define MKD_PORT_MON_RE3		60033		/**< Monitor port (RE3)	*/
#define MKD_PORT_MON_RE4		60034		/**< Monitor port (RE4)	*/
#define MKD_PORT_MON_RE5		60035		/**< Monitor port (RE5)	*/
#define MKD_PORT_MON_RE6		60036		/**< Monitor port (RE6)	*/
#define MKD_PORT_MON_RE7		60037		/**< Monitor port (RE7)	*/
#define MKD_PORT_MON_RE8		60038		/**< Monitor port (RE8)	*/
#define MKD_PORT_MON_RE9		60039		/**< Monitor port (RE9)	*/
#define MKD_PORT_MON_RE10		60040		/**< Monitor port (RE10)	*/
#define MKD_PORT_MON_RE11		60041		/**< Monitor port (RE11)	*/
#define MKD_PORT_MON_RE12		60042		/**< Monitor port (RE12)	*/
#define MKD_PORT_MON_RE13		60043		/**< Monitor port (RE13)	*/
#define MKD_PORT_MON_RE14		60044		/**< Monitor port (RE14)	*/
#define MKD_PORT_MON_RE15		60045		/**< Monitor port (RE15)	*/
#define MKD_PORT_MON_RE16		60046		/**< Monitor port (RE16)	*/
#endif

#define MKD_PORT_SIM			60023		/**< Simulator port		*/

#ifndef OPT_RRH_ZYNQ_REC
#define MKD_PORT_SIM_RE1		60020		/**< Simulator port (RE1)	*/
#define MKD_PORT_SIM_RE2		60021		/**< Simulator port (RE2)	*/
#define MKD_PORT_SIM_RE3		60022		/**< Simulator port (RE3)	*/
#else
#define MKD_PORT_SIM_RE1		60001		/**< Simulator port (RE1)	*/
#define MKD_PORT_SIM_RE2		60002		/**< Simulator port (RE2)	*/
#define MKD_PORT_SIM_RE3		60003		/**< Simulator port (RE3)	*/
#define MKD_PORT_SIM_RE4		60004		/**< Simulator port (RE4)	*/
#define MKD_PORT_SIM_RE5		60005		/**< Simulator port (RE5)	*/
#define MKD_PORT_SIM_RE6		60006		/**< Simulator port (RE6)	*/
#define MKD_PORT_SIM_RE7		60007		/**< Simulator port (RE7)	*/
#define MKD_PORT_SIM_RE8		60008		/**< Simulator port (RE8)	*/
#define MKD_PORT_SIM_RE9		60009		/**< Simulator port (RE9)	*/
#define MKD_PORT_SIM_RE10		60010		/**< Simulator port (RE10)	*/
#define MKD_PORT_SIM_RE11		60011		/**< Simulator port (RE11)	*/
#define MKD_PORT_SIM_RE12		60012		/**< Simulator port (RE12)	*/
#define MKD_PORT_SIM_RE13		60013		/**< Simulator port (RE13)	*/
#define MKD_PORT_SIM_RE14		60014		/**< Simulator port (RE14)	*/
#define MKD_PORT_SIM_RE15		60015		/**< Simulator port (RE15)	*/
#define MKD_PORT_SIM_RE16		60016		/**< Simulator port (RE16)	*/
#endif

#define MKD_MACADDR				0x2B		/**< MAC start address	*/
#define MKD_IPADDR_H			0x30		/**< IP address (high)		*/
#define MKD_IPADDR_L			0x31		/**< IP address (low)		*/

#define MKD_NETMASK_H			0x32		/**< Net mask (high)		*/
#define MKD_NETMASK_L			0x33		/**< Net mask (low)		*/

#define MKD_XFFFF				0xFFFF
#define MKD_X0000				0x0
/* @} */



/** @name	IP address/sub net mask  defination											*/
/* @{ */
#define MKD_IPADR_192			192	
#define MKD_IPADR_168			168	
#define MKD_IPADR_10			10
#define MKD_IPADR_200			200

#define MKD_NETMSK_255			255
#define MKD_NETMSK_0			0
/* @} */


/** @name	CPRI message  defination											*/
/* @{ */
#define  MKD_HBREQ				0x520b0000		/*heart beat request			*/
#define  MKD_HBRSP				0x520b0001		/*heart beat response			*/

#define MKD_ATMSIMSTRREQ		0x1001		/* ATM simulator start request	*/
#define MKD_ATMSIMSTRRSP		0x1002		/* ATM simulator start response	*/
#define MKD_ATMSIMSTPREQ		0x1003		/* ATM simulator stop request	*/
#define MKD_ATMSIMSTPRSP		0x1004		/* ATM simulator stop response	*/

#define MKD_ATMSIMDATSNDREQ	0x1005		/* ATM simulator data send request	*/
#define MKD_ATMSIMDATRCVREQ 0x1006		/* ATM simulator data reception request	*/

#define MKD_CPRISIMSTRREQ	0x52000000		/* CPRI simulator start request	*/
#define MKD_CPRISIMSTRRSP	0x52000001		/* CPRI simulator start response*/
#define MKD_CPRISIMSTPREQ	0x52010000		/* CPRI simulator stop request	*/
#define MKD_CPRISIMSTPRSP	0x52010001		/* CPRI simulator stop response	*/

#define MKD_CPRISIMDATSNDREQ	0x52020004	/* CPRI simulator data send request	*/
#define MKD_CPRISIMDATRCVREQ	0x52030004	/* CPRI simulator data reception request*/


#define MKD_ATMMONSTRREQ	0x2001		/* ATM monitor start request	*/
#define MKD_ATMMONSTRRSP	0x2002		/* ATM monitor start response	*/
#define MKD_ATMMONSTPREQ	0x2003		/* ATM monitor stop request	*/
#define MKD_ATMMONSTPRSP	0x2004		/* ATM monitor stop response	*/

#define MKD_ATMMONDATSNDREQ	0x2005	/* ATM monitor data send request		*/
#define MKD_ATMMONDATRCVNTC	0x2006	/* ATM monitor data reception notification*/

#define MKD_CPRIMONSTRREQ		0x52040000		/* CPRI monitor start request	*/
#define MKD_CPRIMONSTRRSP		0x52040001		/* CPRI monitor start response	*/
#define MKD_CPRIMONSTPREQ		0x52050000		/* CPRI monitor stop request	*/
#define MKD_CPRIMONSTPRSP		0x52050001		/* CPRI monitor stop response	*/

#define MKD_CPRIMONDATSNDNTC	0x52060004	/* CPRI monitor data send notification	*/
#define MKD_CPRIMONDATRCVNTC	0x52070004	/* CPRI monitor data reception notification*/

#define MKD_CPRILNKSTANTC		0x52080004		/* CPRI Link Status Notification*/

#define MKD_ACTSWREQ			0x52090000		/* Active switch request		*/
#define MKD_ACTSWRSP			0x52090001		/* Active switch response		*/
#define MKD_ACTSWFBDREQ			0x520a0000		/* Active switch forbid request	*/
#define MKD_ACTSWFBDRSP			0x520a0001		/* Active switch forbid response*/

#define MKD_SBUSMONSTRREQ	0x2301		/* Serial bus monitor start request	*/
#define MKD_SBUSMONSTRRSP	0x2302		/* Serial bus monitor start response*/
#define MKD_SBUSMONSTPREQ	0x2303		/* Serial bus monitor stop request	*/
#define MKD_SBUSMONSTPRSP	0x2304		/* Serial bus monitor stop response	*/
#define MKD_SBUSSIGNTC		0x2305		/* Serial bus signal notification	*/
/* @} */


/** @name  external PC interface for RE-MT define*/
/* @{ */
#define MKD_REMTCONSTRREQ			0x3001		/**< RE-MT Connection Startup Request		*/
#define MKD_REMTCONSTRRSP			0x3002		/**< RE-MT Connection Startup Response		*/
#define MKD_REMTCONSTPREQ			0x3004		/**< RE-MT disconnection request			*/
#define MKD_REMTCONSTPRSP			0x3005		/**< RE-MT disconnection  response			*/

#define MKD_REMT_IVTINFGET_REQ		0x3141		/**< インベントリ情報取得要求(TDD)		*/
#define MKD_REMT_IVTINFGET_RES		0x3142		/**< インベントリ情報取得応答(TDD)		*/

#define MKD_REMT_SVINFGET_REQ		0x3021		/**< SV情報取得要求(TDD)		*/
#define MKD_REMT_SVINFGET_RES		0x3022		/**< SV情報取得応答(TDD)		*/

#define MKD_REMT_CARRFREQGET_REQ	0x3031		/**< キャリア情報取得要求(TDD)	*/
#define MKD_REMT_CARRFREQGET_RES	0x3032		/**< キャリア情報取得応答(TDD)	*/

#define MKD_REMT_BLK_REQ			0x3041		/**< 閉塞制御要求(TDD)			*/
#define MKD_REMT_BLK_RES			0x3042		/**< 閉塞制御応答(TDD)			*/

#define MKD_REMT_RST_REQ			0x3051		/**< リセット制御要求(TDD)		*/
#define MKD_REMT_RST_RES			0x3052		/**< リセット制御応答(TDD)		*/

#define MKD_REMT_PAONOFF_REQ		0x3061		/**< PA ONOFF制御要求(TDD)		*/
#define MKD_REMT_PAONOFF_RES		0x3062		/**< PA ONOFF制御応答(TDD)		*/

#define MKD_REMT_FIBEROUTPUTGET_REQ	0x3071		/**< 光出力状態取得要求(TDD)	*/
#define MKD_REMT_FIBEROUTPUTGET_RES	0x3072		/**< 光出力状態取得応答(TDD)	*/
#define MKD_REMT_FIBEROUTPUTSET_REQ	0x3073		/**< 光出力状態設定要求(TDD)	*/
#define MKD_REMT_FIBEROUTPUTSET_RES	0x3074		/**< 光出力状態設定応答(TDD)	*/

#define MKD_REMT_POWADJ_REQ			0x3081		/**< 送信出力設定要求(TDD)		*/
#define MKD_REMT_POWADJ_RES			0x3082		/**< 送信出力設定応答(TDD)		*/
#define MKD_REMT_POWSAV_REQ			0x3083		/**< 送信出力保存要求(TDD)		*/
#define MKD_REMT_POWSAV_RES			0x3084		/**< 送信出力保存応答(TDD)		*/

#define MKD_REMT_AISGGET_REQ		0x3091		/**< AISG状態取得要求(TDD)		*/
#define MKD_REMT_AISGGET_RES		0x3092		/**< AISG状態取得応答(TDD)		*/
#define MKD_REMT_AISGSET_REQ		0x3093		/**< AISG状態設定要求(TDD)		*/
#define MKD_REMT_AISGSET_RES		0x3094		/**< AISG状態設定応答(TDD)		*/

#define MKD_REMTFLDLSTRREQ			0x3301		/**< ファイルダウンロード開始要求			*/
#define MKD_REMTFLDLSTRRSP			0x3302		/**< ファイルダウンロード開始応答			*/
#define MKD_REMTTRAFLBLKSND			0x3303		/**< TRAファイルブロック転送				*/
#define MKD_REMTFLDLCMPREQ			0x3304		/**< ファイルダウンロード完了要求			*/
#define MKD_REMTFLDLCMPRSP			0x3305		/**< ファイルダウンロード完了応答			*/

#define MKD_TCPDATA					1		/**< TCP link data*/
#define MKD_CPRIDATA				2		/**< CPRI link data*/

/* @} */

/** @name  external PC interface for FHM-MT define*/
/* @{ */
#define MKD_FHMMT_FIBEROUTPUTGET_REQ	0x4001		/**< 光出力状態取得要求					*/
#define MKD_FHMMT_FIBEROUTPUTGET_RES	0x4002		/**< 光出力状態取得応答					*/
#define MKD_FHMMT_FIBEROUTPUTSET_REQ	0x4011		/**< 光出力状態設定要求					*/
#define MKD_FHMMT_FIBEROUTPUTSET_RES	0x4012		/**< 光出力状態設定応答					*/
#define MKD_FHMMT_STATEGET_REQ			0x4031		/**< 状態取得							*/
#define MKD_FHMMT_STATEGET_RES			0x4032		/**< 状態情報取得応答					*/
#define MKD_FHMMT_REUSEIFGET_REQ		0x4071		/**< 使用/未使用状態取得要求			*/
#define MKD_FHMMT_REUSEIFGET_RES		0x4072		/**< 使用/未使用状態取得応答 			*/
#define MKD_FHMMT_REUSEIFSET_REQ		0x4073		/**< RE使用/未使用状態設定要求			*/
#define MKD_FHMMT_REUSEIFSET_RES		0x4074		/**< RE使用/未使用状態設定応答 			*/
#define MKD_FHMMT_RESTATEINFOGET_REQ	0x4075		/**< 配下REのALM,ERR,FAN要因取得要求 	*/
#define MKD_FHMMT_RESTATEINFOGET_RES	0x4076		/**< 配下REのALM,ERR,FAN要因取得応答 	*/
#define MKD_FHMMT_REPORTSTATEGET_REQ	0x4081		/**< RE PORT状態取得要求 */
#define MKD_FHMMT_REPORTSTATEGET_RES	0x4082		/**< RE PORT状態取得応答 */
#define MKD_FHMMT_REPORTSTATESET_REQ	0x4083		/**< RE PORT状態設定要求 */
#define MKD_FHMMT_REPORTSTATESET_RES	0x4084		/**< RE PORT状態設定応答 */

/** @name  external PC common FHM-MT define*/
/* @{ */
#define MKD_FHMMT_ERRNO 			 0
#define MKD_FHMMT_ERRYS				 1
#define MKD_FHMMT_HWVER_LEN			2
#define MKD_FHMMT_FWVER_LEN			2
/* ■TRA SV状態管理テーブル */
#define MKD_CARDSV_ERR_BIT		CMD_CARDSV_ERR_BIT
#define MKD_CARDSV_ALM_BIT		CMD_CARDSV_ALM_BIT
#define MKD_CARDSV_ACT_BIT		CMD_CARDSV_ACT_BIT
#define MKD_CARDSV_BLK_BIT		CMD_CARDSV_BLK_BIT
#define MKD_CARDSV_USE_BIT		CMD_CARDSV_USE_BIT
#define MKD_CARDSV_BLK_BIT_3G		CMD_CARDSV_BLK_BIT_3G
#define	MKD_TRA_AMPDWN_3G_BIT			0x8000	/**<  AMP部電源断	3G					*/
#define	MKD_TRA_AMPDWN_S3G_BIT			0x0004	/**<  AMP部電源断	S3G					*/
#define	MKD_TRA_WARMUP_BIT			0x0002	/**<  WarmUp							*/
#define	MKD_TRA_SLEEP_BIT			0x0040	/**<  スリープ							*/

#define MKD_FIBERFORCE_BIT			0x00000002
#define D_RRH_REG_CPRI_SFPCNT        		0x81000204  /**< SFP Control                                      */
#define M_RRH_REG_CPRI_GETADR(cprino, ofs) (UINT)((D_RRH_CPRINO_REC==cprino) ? ofs : (ofs+0x1000+(0x800*(cprino-1))))
#define M_RRH_REG_CPRI_SFPCNT( cprino )       M_RRH_REG_CPRI_GETADR( cprino, D_RRH_REG_CPRI_SFPCNT         )

/* @} */

/** @name  REMT connection type*/
/* @{ */

#define MKD_LOCALCON					1		/**< REMT directly connect to TRA*/
#define MKD_REMOTECON					2		/**< REMT connect to TRA through Jig*/

/* @} */

/** @name  Target card in REMT connection request*/
/* @{ */
#define MKD_MDIF					0		/**< Connection target is TRA card	*/


#define MKD_REMTSERLBUSADDR_MASK	0x3FFF		/*Serial bus address mask*/
#define MKD_REMTSERLBUSTYPE_MASK	0xC000		/*Serial bus access type mask*/

#define	MKD_BYTE		0x1		/* byte*/
#define	MKD_HALFWORD		0x2		/* half word*/
#define	MKD_WORD		0x3		/* word*/

#define MKD_SZ_LANPK		1460		/* Size of a LAN packet				*/

/* @} */

/** @name  Timer interval defination*/
/* @{ */
#define MKD_INTV_MONDAT 	50			/* Timer interval for monitor data(500ms)*/

#define 	MKD_FPGA				0x0			/* FPGA file 							*/
#define 	MKD_EEPROM				0x1			/* EEPROM parameter 					*/

#define 	MKD_PACKDAT_MAX			10228		/* ファイル転送サイズ最大値				*/

#define 	MKD_TIMER_NUM			0x2		/* Tijmer number		*/
#define 	MKD_TIMER_EEPROM		13		/* ダウンロード確認タイマInterval for EEPROM file from */
#define 	MKD_TIMER_FPGA		35000	/* ダウンロード確認タイマInterval for FPGA file from TRA */
#define 	MKD_TIMER_EEPROMWR	800		/* TRAファイルライトタイマInterval */
#define 	MKD_TIMER_FPGAWR		84000	/* TRAファイルライトタイマInterval */
/* @} */

#endif	/*M_MK_DEF*/
