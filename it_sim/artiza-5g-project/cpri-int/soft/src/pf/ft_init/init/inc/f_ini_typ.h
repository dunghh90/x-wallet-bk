/*!
 * @skip  $ld:$
 * @file  f_ini_typ.h
 * @brief 初期化プロセス 構造体宣言
 * @date  2013/03/31 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @data  2014/12/17 ALPHA) okabe Create RRH-007-000 共通ppc対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
/*!
 * @addtogroup RRH_INI
 * @{
 */
#ifndef F_INI_TYP_H
#define F_INI_TYP_H


/************************************************************/
/* RAM Table Type											*/
/************************************************************/

/************************************************************/
/* ROM Table Type											*/
/************************************************************/
/**
 * @brief	プロセス起動管理テーブル
 * @note	プロセス起動管理テーブル型の定義
 * 
 */
typedef struct{
	CHAR		procFunc[32];						/**< プロセスメイン関数	  */
	UINT		procCreateFlag;						/**< 起動フラグ			  */
	UINT		procNo;								/**< プロセス番号(識別子) */
	INT			procPriority;						/**< プライオリティ		  */
	INT			procCpu;							/**< CPU割り当て		  */
	UINT		PQID;								/**< processQID				*/
}T_INI_CRT_EVENT_CTBL;
/** @} */

/**
 * @brief	Signal Reset manage table
 * @note	Signal Reset manage table
 * 
 */
typedef struct{
	UINT				wdt_start;					/**< WDT起動				*/
	UINT				cpri_dis;					/**< CPRI disable			*/
	UINT				pa_onoff;					/**< 無線系停止(PA OFF)		*/
	UINT				log_save;					/**< ログ保存有無			*/
	UINT				led_onoff;					/**< LED消灯				*/
	UINT				tilt_onoff;					/**< TILT制御(OFF)			*/
	UINT				reset_onoff;				/**< リセット制御			*/
}T_INIT_SIG_RESET_TBL;

/** @} */

/**
 * @brief	Signal Name table
 * @note	Signal Name table
 * 
 */
typedef struct{
	UINT				signo;						/**< Signal No				*/
	CHAR				signame[32];				/**< Signal Name			*/
}T_INIT_SIG_NAME_TBL;

/** @} */

/**
 * @brief	callSetRell
 * @note	INIT Trouble State Mng
 * 
 */
typedef struct{
	INT (*eventProcess_addr)(VOID* msg_p , VOID* buf_p );
}T_INIT_TROUBLE_CALLSETRELL_MTXTBL;

/** @} */

/**
 * @brief	Signal information manage table
 * @note	Signal information manage table
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'logcount', 'uiCompelResetFlg', 'uiResetFlg')
 * 
 */
typedef struct{
	UINT				state;					/**< 処理状態						*/
	UINT				logcount;				/**< ログ出力数						*/
	UINT				addInfo;				/**< Signalオプション				*/
	UINT				sigstate;				/**< Signal種別						*/
	UINT				logType;				/**< ログ種別						*/
	UINT				trouble_logget;			/**< 障害ログ取得有無				*/
	UINT				uiCompelResetFlg;		/**< 強制RESETフラグ(SOFT RESET MASK無視)	*/
	UINT				resetReason;			/**< RESET要因						*/
	UINT				uiResetFlg;				/**< RESET実施有無					*/
	UINT				uiResetFlg_define;		/**< RESET実施有無(定義上)			*/
}T_INIT_SIG_INFO_TBL;
/** @} */

/**
 * @brief	Signal information manage table
 * @note	Signal information manage table
 * 
 */
typedef struct{
	CHAR				thdName[16];						/**< name			*/
	UINT				thdPriority;						/**< priority		*/
	FUNCPTR				thdFunc;							/**< Function		*/
	UINT				thdStackSize;						/**< stack size		*/
}T_INIT_CRT_EVENT_CTBL;
/** @} */

/**
 * @brief	Signal information message
 * @note	Signal information message table
 * 
 */
typedef struct{
	T_RRH_HEAD				head;							/**< head			*/
	siginfo_t				sigInfo;						/**< data			*/
	T_INIT_SIG_INFO_TBL		infoTbl;
}T_INIT_SIG_NTC;
/** @} */

/**
 * @brief	Signal Status Info table
 * @note	Signal Status Info table
 * 
 */
typedef struct{
	UINT				sig_state;					/**< Signal State		*/
	CHAR				message[50];				/**< Message			*/
}T_INIT_SIG_STATUS_INFO_TBL;
/** @} */

/**
 * @brief	起動面－アドレス格納TBL
 * @note	起動面－アドレス格納TBL
 * 
 */
typedef struct 
{
	UINT	face;
	UINT	addr;
}T_INIT_LOGSAVE;
/** @} */

/**
 * @brief	BPFバッファアクセス用
 * @note	BPFバッファアクセス用
 * 
 */
typedef struct{
	UINT	reg_addr;
	UINT	size;
	UINT	reg_data;
}T_INI_BPF_REG_BUFFER;
/** @} */

#endif
/* @} */
