/*!
 * @skip  $ld:$
 * @file  f_rrh_typ.h
 * @brief System共通定義
 * @date  2013/11/11 ALPHA) yokoyama Create
 * @date  2015/10/30 ALPHA) ueda modify for M-RRU-ZSYS-01812
 * @date  2015/11/12 ALPHA) ueda modify for TDD-RRE(zynq)
 * @date  2015/11/20 ALPHA) murakami TDD-RRE(ZYNQ)対応
 * @date  2018/02/14 FJT)   吉田 add 5G-DU対応
 * @date  2018/02/23 FJT)   吉田 modify
 * @date  2020/01/06 FJT)吉田   DCM5GDU-053-191227 [19BD] : ハードログ情報追加
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2020
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_TYPE_H
#define F_RRH_TYPE_H

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "../../cmn/dcm/inc/m_cm_def.h"
#include "../../sys/inc/f_rrh_def.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name 共通型定義
 * @note
 * @{
 */
typedef		void							VOID;
typedef		int								INT;
typedef		unsigned int					UINT;
typedef		short							SHORT;
typedef		unsigned short					USHORT;
typedef		char							CHAR;
typedef		unsigned char					UCHAR;
typedef		float							FLOAT;
typedef		double							DL;
typedef		long							LONG;
typedef		unsigned long					ULONG;
typedef		long long int					LLINT;
typedef		unsigned long long int			ULLINT;
/* @} */

/*!
 * @name メッセージ共通ヘッダ
 * @note メッセージ共通ヘッダ定義:どのprocessでどのようなmessageを送信する場合でも本ヘッダを使用すること
 * @{
 */
typedef struct{
	UINT		uiEventNo;					/**< イベント番号						*/
	UINT		uiSignalkind;				/**< SignalKind(EventIDが重複しない場合はDon't care)*/
	UINT		uiDstPQueueID;				/**< 最終送信先Process Queue ID			*/
	UINT		uiDstTQueueID;				/**< 最終送信先Thread  Queue ID			*/
	UINT		uiSrcPQueueID;				/**< 送信元    Process Queue ID			*/
	UINT		uiSrcTQueueID;				/**< 送信元    Thread  Queue ID			*/
	UINT		uiLength;					/**< Length(Head部 + Data部)			*/
	UINT		uiKeyID;					/**< 通信用共有メモリ管理用KEYID		*/
}T_RRH_HEAD;
/* @} */

/**
 * @brief       Common Header
 * @note        Common Header is ..
 *
 */
typedef struct{
       T_RRH_HEAD				header;									/**< Thread Common Header							*/
       UCHAR					msgData[8168];  						/**< データ部										*/
}T_RRH_MSGFORM;

/****************************************************************************************************************************/
/** 共有メモリ作成用構造体
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	共有メモリ作成用構造体
 * @note	共有メモリ作成用構造体 is ..
 */
typedef struct{
	unsigned int				shmCnt;									/**< 共有メモリ数									*/
	unsigned int				shmSize;								/**< 共有メモリサイズ								*/
}T_RRH_SYS_CREATE_SHMMEM;
/* @} */

/****************************************************************************************************************************/
/** System Time table
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	System Time table
 * @note	System Time table is ..
 */
typedef struct{
	USHORT						year;									/**< 年												*/
	UCHAR						month;									/**< 月												*/
	UCHAR						day;									/**< 日												*/
	UCHAR						hour;									/**< 時間											*/
	UCHAR						min;									/**< 分												*/
	UCHAR						sec;									/**< 秒												*/
	UCHAR						msec;									/**< ミリ秒											*/
    UINT						ucount;									/**< μsec カウンタ									*/
}T_RRH_SYSTEM_TIME;
/** @} */

/****************************************************************************************************************************/
/** T_CMM_LOG_LEVEL_MNG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_CMM_LOG_LEVEL_MNG
 * @note	T_CMM_LOG_LEVEL_MNG is ..
 */
typedef struct{
	UINT				dummy[4];									/**< dummy(暫定)		*/
	UINT				assertLogLevel;								/**< AssertLogLevel		*/
	UINT				registerLogLevel;							/**< RegisterLogLevel	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT				assertDwlLevel;								/**< assertDwlLogLevel	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT				cycleLogCount;								/**< CyclicLog退避カウンタ			*/
	UINT				cycleLogCount_cur;							/**< CyclicLog退避カウンタ(現在値)	*/
	UINT				selfResetFlg;								/**< 自律リセットカウンタ有効無効	*/
	UINT				selfResetCount;								/**< 自律リセットカウンタ			*/
	UINT				selfResetCount_cur;							/**< 自律リセットカウンタ(現在値)	*/
	UINT				reserve;									/**< 予約 */
}T_RRH_LOG_LEVEL_MNG;
/** @} */

/****************************************************************************************************************************/
/** プロセスID管理用構造体
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	プロセスID管理用構造体
 * @note	プロセスID管理用構造体 is ..
 */
typedef struct{
	UINT						aplProcessID;							/**< apl側のプロセスID								*/
	INT							realProcessID;							/**< 実プロセスID									*/
	UINT						PQID;									/**< ProcessのQID識別子								*/
}T_RRH_PID_MNG_INFO;

/**
 * @brief	プロセスID管理用構造体
 * @note	プロセスID管理用構造体 is ..
 */
typedef struct{
	UINT						count;									/**< プロセスID数									*/
	T_RRH_PID_MNG_INFO			info[D_RRH_PROCID_MAX];					/**< 情報											*/
	UINT						mpsw_count;								/**< プロセスID数									*/
	T_RRH_PID_MNG_INFO			mpsw_info[2];							/**< 情報											*/
}T_RRH_PID_MNG;
/* @} */

/****************************************************************************************************************************/
/** スレッドID管理用構造体
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	スレッドID管理用構造体
 * @note	スレッドID管理用構造体 is ..
 */
typedef struct{
	UINT						aplThreadID;							/**< apl側のスレッドID								*/
	INT							realThreadID;							/**< 実スレッドID									*/
	UINT						TQID;									/**< スレッドQueueID								*/
	unsigned long int			pThreadID;								/**< pthreadID */
}T_RRH_TID_MNG_INFO;

/**
 * @brief	スレッドID管理用構造体
 * @note	スレッドID管理用構造体 is ..
 */
typedef struct{
	UINT						count;									/**< スレッド数										*/
	T_RRH_TID_MNG_INFO			infoT[D_RRH_THREAD_MAX];				/**< スレッド情報									*/
}T_RRH_PTID_MNG_INFO;

/**
 * @brief	スレッドID管理用構造体
 * @note	スレッドID管理用構造体 is ..
 */
typedef struct{
	T_RRH_PTID_MNG_INFO			infoP[D_RRH_PROCID_MAX];				/**< プロセス数										*/
}T_RRH_TID_MNG;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_RST_REASON_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_RST_REASON_RECORD
 * @note	T_RRH_LOG_RST_REASON_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						version;								/**< Version										*/
	UINT						resetReason;							/**< Reset Reason									*/
	UINT						addInfo;								/**< Reset Reason add infomation					*/
}T_RRH_LOG_RST_REASON_RECORD;

/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_RST_REASON
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_RST_REASON
 * @note	T_RRH_LOG_RST_REASON is ..
 */
typedef struct{
	UINT						uiWriteIdx;								/**< write index									*/
	UINT						uiCycleFlg;								/**< Cycle FLG										*/
	UINT						dummy[2];								/**< dummy											*/
	T_RRH_LOG_RST_REASON_RECORD	tRecord[D_RRH_LOG_RSTRSN_RCD_NUM];		/**< reset reason log								*/
}T_RRH_LOG_RST_REASON;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_ASSERT_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_ASSERT_RECORD
 * @note	T_RRH_LOG_ASSERT_RECORD is ..
 */
typedef	struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< Date time										*/
	UINT						uiLogLevel;								/**< LogLevel										*/
	UCHAR						ucFileName[D_RRH_LOG_MAX_ASSERTFILENAME];	/**< filename									*/
	UINT						uiLine;									/**< Line											*/
	UCHAR						message[D_RRH_LOG_MAX_ASSERTLOGBODY];	/**< assert message									*/
	INT							iTid;									/**< thread id										*/
}T_RRH_LOG_ASSERT_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_ASSERT
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_ASSERT
 * @note	T_RRH_LOG_ASSERT is ..
 */
typedef	struct{
	UINT						uiWriteIdx;								/**< write index									*/
	UINT						uiCycleflg;								/**< Cyclic flg										*/
	UINT						dummy[2];								/**< DUMMY											*/
	T_RRH_LOG_ASSERT_RECORD		tRecord[D_RRH_LOG_MAX_ASSERTLOGNUM];	/**< record											*/
}T_RRH_LOG_ASSERT;
/** @} */

/**
 * @brief	T_RRH_LOG_ASSERT for DWL
 * @note	T_RRH_LOG_ASSERT for DWL is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef	struct{
	UINT						uiWriteIdx;									/**< write index								*/
	UINT						uiCycleflg;									/**< Cyclic flg									*/
	UINT						dummy[2];									/**< DUMMY										*/
	T_RRH_LOG_ASSERT_RECORD		tRecord[D_RRH_LOG_MAX_ASSERTLOGNUM_DWL];	/**< record										*/
}T_RRH_LOG_ASSERT_DWL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_REG_ACCESS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_REG_ACCESS_RECORD
 * @note	T_RRH_LOG_REG_ACCESS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						loglevel;								/**< loglevel										*/
	UINT						addr;									/**< address										*/
	UINT						data1;									/**< data1											*/
	UINT						data2;									/**< data2											*/
	UINT						tid;									/**< thread id										*/
}T_RRH_LOG_REG_ACCESS_RECORD;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_REG_ACCESS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_REG_ACCESS
 * @note	T_RRH_LOG_REG_ACCESS is ..
 */
typedef struct{
	UINT							uiWriteIdx;							/**< write index									*/
	UINT							uiCycleflg;							/**< Cyclic flg										*/
	UINT							maskInfo;							/**< maskInfo										*/
	UINT							dummy;								/**< dummy											*/
	T_RRH_LOG_REG_ACCESS_RECORD		tRecord[D_RRH_LOG_REGLOG_RCD_NUM];	/**< register access log							*/
}T_RRH_LOG_REG_ACCESS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_SPI_ACCESS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_SPI_ACCESS_RECORD
 * @note	T_RRH_LOG_SPI_ACCESS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						devkind;								/**< device kind									*/
	UINT						rwkind;									/**< READ/WRITE kind								*/
	UINT						addr;									/**< address										*/
	UINT						data;									/**< data											*/
}T_RRH_LOG_SPI_ACCESS_RECORD;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_SPI_ACCESS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_SPI_ACCESS
 * @note	T_RRH_LOG_SPI_ACCESS is ..
 */
typedef struct{
	UINT							uiWriteIdx;							/**< write index									*/
	UINT							uiCycleflg;							/**< Cyclic flg										*/
	UINT							dummy[2];							/**< dummy											*/
	T_RRH_LOG_SPI_ACCESS_RECORD		tRecord[D_RRH_LOG_SPILOG_RCD_NUM];	/**< SPI access log									*/
}T_RRH_LOG_SPI_ACCESS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_APD_BUF_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_APD_BUF_RECORD
 * @note	T_RRH_LOG_APD_BUF_RECORD is ..
 */
typedef struct{
	UCHAR						branch;									/**< branch											*/
	UCHAR						addr;									/**< device kind									*/
	USHORT						data;									/**< data											*/
}T_RRH_LOG_APD_BUF_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_PROC_DEAD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_PROC_DEAD
 * @note	T_RRH_LOG_PROC_DEAD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				tDate;									/**< time											*/
	UCHAR							data[D_RRH_LOG_PROC_DEAD_LOG_DATA_SIZE];/**< Log Data										*/
}T_RRH_LOG_PROC_DEAD;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_INFO_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_INFO_RECORD
 * @note	T_RRH_LOG_INFO_RECORD is ..
 */
typedef	struct{
	UINT							uiLogKind;							/**< ログ種別										*/
	UINT							uiOffset;							/**< Log退避オフセット								*/
	UINT							uiSize;								/**< Logサイズ(圧縮前)								*/
	UINT							uiCompOnOff;						/**< 圧縮有無										*/
	UINT							uiCompSize;							/**< 圧縮時サイズ									*/
}T_RRH_LOG_INFO_RECORD;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_INFO
 * @note	T_RRH_LOG_INFO is ..
 */
typedef	struct{
	UINT							uiLogNum;								/**< Log Num									*/
	UINT							dummy[3];								/**< DUMMY										*/
	T_RRH_LOG_INFO_RECORD			tRecord[D_RRH_FLASH_LOG_MAX];			/**< record										*/
}T_RRH_LOG_INFO;



 /* 5GDU_chg */
/****************************************************************************************************************************/
/** T_RRH_ALMHIS_CARCTRL_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log L3 Control Information Table
 * @note	Alarm History Log L3 Control Information Table is
 *
 */
typedef struct{
	USHORT							CarState;							/* !< キャリア設定有無								*/
	UINT							Freq;								/* !< 周波数										*/
	USHORT							Bandwidth;							/* !< 帯域											*/
	UINT							AcellId;							/* !< ACELL ID										*/
	UINT							CcId;								/* !< CC ID											*/
	UINT							PortId;								/* !< PORT ID										*/
	DL								Gain;								/* !< Gain											*/
}T_RRH_ALMHIS_CARCTRL_INFO;

typedef struct{
	T_RRH_ALMHIS_CARCTRL_INFO		carInfo[D_RRH_CAR_MAX][D_RRH_ANT_ULDL_MAX];
}T_RRH_ALMHIS_ANTCTRL_INFO;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
typedef struct{
    USHORT lteCarState[D_RRH_CAR_MAX][D_RRH_ANT_ULDL_MAX];              /* !< LTEキャリア設定有無                           */
    USHORT lteFreq[D_RRH_CAR_MAX];                                      /* !< LTE周波数                                     */
    USHORT lteBandwidth[D_RRH_CAR_MAX];                                 /* !< LTE帯域                                       */
    USHORT txTyp[D_RRH_CAR_MAX];                                        /* !< 送信系                                        */
    USHORT noModState;                                                  /* !< 無変調キャリア設定有無                        */
    USHORT noModPower;                                                  /* !< 無変調キャリア電力値                          */
    USHORT sysParam[D_RRH_CAR_MAX];                                     /* !< システムパラメータ                            */
    USHORT rscAlarm[D_RRH_ANT_MAX];                                     /* !< リソースALM状態                               */
    USHORT paState[D_RRH_ANT_MAX];                                      /* !< PA ON/OFF                                     */
    USHORT stopWaveState[D_RRH_ANT_MAX];                                /* !< 停波状態                                      */
}T_RRH_ALMHIS_L3CTRL_INFO;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

 /* 5GDU_chg */
/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_3GCAR_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log 3G Carrier Information Table
 * @note	Alarm History Log 3G Carrier Information Table is
 *
 */
typedef struct{
	USHORT							carState;							/* !< キャリア設定有無								*/
	USHORT							freq;								/* !< 周波数										*/
	USHORT							noModState;							/* !< 無変調キャリア設定有無						*/
	USHORT							noModPower;							/* !< 無変調キャリア電力値							*/
}T_RRH_ALMHIS_3GCAR_INFO;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_CPRI_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log CPRI Information Table
 * @note	Alarm History Log CPRI Information Table is
 *
 */
typedef struct{
	UINT							cpriStatus;							/* !< CPRIステータス								*/
	UINT							dummy[3];							/* !< dummy											*/
}T_RRH_ALMHIS_CPRI_INFO;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_SV_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log SV Information Table
 * @note	Alarm History Log SV Information Table is
 *
 */
typedef struct{
	UINT							svReg[D_RRH_ALMHIS_SVREG_INFO_NUM];	/* !< SV Register Info								*/
}T_RRH_ALMHIS_SV_INFO;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_TX_ANT_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Tx Antenna Information Table
 * @note	Alarm History Log Tx Antenna Information Table is
 *
 */
typedef struct{
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT							txState[D_RRH_ANT_MAX];				/* !< Tx State										*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT							tssi[D_RRH_ANT_MAX][D_RRH_CAR_MAX];	/* !< tssi											*/
	UINT							refPow[D_RRH_ANT_MAX];				/* !< REF電力										*/
	UINT							fbPow[D_RRH_ANT_MAX];				/* !< FB電力										*/
	UINT							temp;								/* !< 温度											*/	//★温度はTX配下なの？
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT							dpdMode[(D_RRH_ANT_MAX/2)];			/* !< 歪保障動作モード								*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	DL								vswrRL[D_RRH_ANT_MAX];				/* !< VSWR RL										*/
	UINT							txPwrPsum[D_RRH_ANT_MAX][16];		/* !< PWR PSUM										*/
	UINT							txPwrCnt[D_RRH_ANT_MAX][45];		/* !< PWR Cnt										*/
	UINT							txPwrMon[D_RRH_ANT_MAX];			/* !< PWR MON										*/
	UINT							txPwrPsum_avg[D_RRH_ANT_MAX];		/* !< PWR PSUM AVG									*/
}T_RRH_ALMHIS_TX_ANT_INFO;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_RX_ANT_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Rx Antenna Information Table
 * @note	Alarm History Log Rx Antenna Information Table is
 *
 */

typedef struct{
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT							rxState[D_RRH_ANT_MAX];				/* !< Rx State										*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT							rxPower[D_RRH_ANT_MAX];				/* !< rxPower										*/
	UINT							Rtwp[D_RRH_ANT_MAX][D_RRH_CAR_MAX];	/* !< rssi											*/
}T_RRH_ALMHIS_RX_ANT_INFO;

typedef struct{
	INT								RxTempReduction;					/* 温度補償制御RX 温度補償リダクション量[dBm]		*/
	INT								RxGainCompVal[D_RRH_ANT_MAX_MMW];	/* Rx Gain比較値[dBm]								*/
	INT								dummy[2];
}T_RRH_ALMHIS_RX_ANT_INFO_EXT;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_REG_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Register Information Table
 * @note	Alarm History Log Register Information Table is
 *
 */
typedef struct{
	UINT							regVal[D_RRH_ALMHIS_REG_VAL_NUM];	/* !< register value								*/
}T_RRH_ALMHIS_REG_INFO;

typedef struct{
	UINT							regVal[D_RRH_ALMHIS_REG_EXT_NUM];	/* !< register value								*/
}T_RRH_ALMHIS_REG_INFO_EXT;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALMHIS_APD_BUF_INFO ZynqSLC
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log APD Buf Information Table
 * @note	Alarm History Log APD Buf Information Table is
 *
 */
typedef struct{
																		/*!< APD BUF 情報									*/
	T_RRH_LOG_APD_BUF_RECORD		data[D_RRH_ANT_MAX][D_RRH_ALMHIS_APD_BUF_ADR_NUM_SLC];
}T_RRH_ALMHIS_APD_BUF_INFO;

/** @} */

 /* 5GDU_chg */
/****************************************************************************************************************************/
/** T_RRH_ALARM_HISTORYLOG_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Information Table
 * @note	Alarm History Log Information Table is
 *
 */

typedef struct{
	T_RRH_ALMHIS_ANTCTRL_INFO		antInfo[D_RRH_ANT_MAX]; 			/*!< L3制御情報										*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	T_RRH_ALMHIS_CPRI_INFO			cpriInfo;							/*!< CPRI情報										*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	T_RRH_ALMHIS_SV_INFO			svInfo;								/*!< SV情報											*/
	T_RRH_ALMHIS_TX_ANT_INFO		txAntInfo;							/*!< 送信アンテナ情報								*/
	T_RRH_ALMHIS_RX_ANT_INFO		rxAntInfo;							/*!< 受信アンテナ情報								*/
	T_RRH_ALMHIS_REG_INFO			regInfo;							/*!< レジスタ生情報									*/
	T_RRH_ALMHIS_REG_INFO_EXT		regExt;								/*!< レジスタ生情報(拡張)							*/
	T_RRH_ALMHIS_RX_ANT_INFO_EXT	rxAntInfoExt;						/*!< 受信アンテナ情報								*/
}T_RRH_ALARM_HISTORYLOG_INFO;
 /* 5GDU_chg */
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_APD_BUF_30SEC ZynqSLC
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log APD Buf 30sec Information Table
 * @note	Alarm History Log APD Buf 30sec Information Table is
 *
 */
typedef struct{
	T_RRH_SYSTEM_TIME				tDate;								/*!< Date time										*/
																		/*!< APD BUF 情報									*/
	T_RRH_LOG_APD_BUF_RECORD		data[D_RRH_ANT_MAX][D_RRH_ALMHIS_APD_BUF_ADR_NUM_SLC];
}T_RRH_LOG_APD_BUF_30SEC;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALARM_HISTORYLOG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Table
 * @note	Alarm History Log Table is
 *
 */
typedef struct{
	/* 時刻 */
	T_RRH_SYSTEM_TIME				tDate;								/*!< Date time										*/
	UINT							swAlarmFactor;						/*!< Alarm要因(S/W)									*/
	UINT							hwAlarmFactor[E_RRH_ALMHIS_ALM_FACTOR_NUM];		/*!< Alarm要因(H/W)						*/
	UINT							dlFile_ver;							/*!< Version(統合版数)								*/
	UINT							factorType;
	UINT							dummy[2];							/*!< dummy領域                                      */
	T_RRH_LOG_ASSERT_RECORD			astLog[D_RRH_ALMHIS_AST_LOG_NUM];	/*!< assert log情報									*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	T_RRH_LOG_REG_ACCESS_RECORD		regLog[D_RRH_ALMHIS_REG_LOG_NUM];	/*!< register log情報								*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	T_RRH_ALARM_HISTORYLOG_INFO		info[D_RRH_ALMHIS_LOG_MEAS_NUM];	/*!< alarm history information						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT							regVal[D_RRH_ALMHIS_REG_ALM_NUM];	/*!< register log情報(alm発生時情報)				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}T_RRH_ALARM_HISTORYLOG;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALARM_HISTORYLOG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Manage Table
 * @note
 *
 */
typedef struct {
	USHORT							alm_log_index;						/*!< Alarm log index								*/
	UCHAR							alm_log_wrtflg;						/*!< Alarm log flag									*/
	UCHAR							uiCycleFlg;							/*!< Cyclic flag									*/
	UCHAR							dummy[12];							/*!< dummy											*/
	T_RRH_ALARM_HISTORYLOG			histlog[D_RRH_ALMHIS_LOG_RECORD_NUM];	/*!< Alarm History Log Area						*/
} T_RRH_ALARM_HISTORYMNG;

/** @} */

/****************************************************************************************************************************/
/** T_RRH_ALARM_HISTORYLOG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Manage Table
 * @note
 *
 */
typedef struct {
	UINT							dataPosi;							/*!< 次回更新位置									*/
	UINT							dummy[3];							/*!< dummy											*/
	T_RRH_ALARM_HISTORYLOG_INFO		logInfo[D_RRH_ALMHIS_LOG_MEAS_NUM];	/*!< Alarm History Log Area							*/
} T_RRH_ALM_LOG_FOR_HIST;

/** @} */
 /* 5GDU_chg */
/****************************************************************************************************************************/
/** T_RRH_INVENTORY_DATA
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Manage Table
 * @note
 *
 */
typedef struct {
	UCHAR							traValcom[0x800];		/*!< TrainingData(su6,mmW共通)						*/
	UCHAR							traValindiv[0x800];		/*!< TrainingData(mmW個別)							*/
} T_RRH_INVENTORY_DATA;
 /* 5GDU_chg */

/****************************************************************************************************************************/
/** T_RRH_TRAINING_DATA
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Manage Table
 * @note
 *
 */
typedef struct {
	UCHAR							traVal[D_RRH_TRAING_DATA_SIZE];		/*!< TrainingData									*/
	UCHAR							traVal_vt[D_RRH_TRAING_DATA_SIZE];		/*!< Virtual memory									*/
} T_RRH_TRAINING_DATA;

 /* 5GDU_chg */
/****************************************************************************************************************************/
/** T_RRH_RF_TRAINING_DATA
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief   Alarm History Log Manage Table
 * @note
 *
 */
typedef struct {
	UCHAR							traVal[D_RRH_RF_TRAING_DATA_SIZE];		/*!< TrainingData									*/
} T_RRH_RF_TRAINING_DATA;


/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_TEMP_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_TEMP_RECORD
 * @note	T_RRH_LOG_TEMP_RECORD is ..
 */
typedef	struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< Date time										*/
	INT							tempVal;								/**< temprature										*/
	INT							tempVal2;								/**< temprature2									*/
}T_RRH_LOG_TEMP_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_TEMP
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_TEMP
 * @note	T_RRH_LOG_TEMP is ..
 */
typedef	struct{
	UINT						uiWriteIdx;								/**< write index									*/
	UINT						uiCycleflg;								/**< Cyclic flg										*/
	USHORT						usCurCnt;								/**< 現在のカウンタ値[秒]							*/
	USHORT						usTimerVal;								/**< 現在のタイマ値[分]								*/
	UINT						dummy;									/**< DUMMY											*/
	T_RRH_LOG_TEMP_RECORD		tRecord[D_RRH_LOG_TEMP_LOG_NUM];		/**< record											*/
}T_RRH_LOG_TEMP;

/**
 * @brief	T_RRH_LOG_TEMP_RECORD_SLC
 * @note	T_RRH_LOG_TEMP_RECORD_SLC is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef	struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< Date time										*/
	INT							temp[D_RRH_SENSOR_SHT20_NUM_SLC];		/**< temprature										*/
	INT							hum[D_RRH_SENSOR_SHT20_NUM_SLC];		/**< 湿度											*/
	INT							cur;									/**< 電流											*/
	INT							vol;									/**< 電圧											*/
}T_RRH_LOG_TEMP_RECORD_SLC;

/**
 * @brief	T_RRH_LOG_TEMP_MAX_SLC
 * @note	T_RRH_LOG_TEMP_MAX_SLC is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef	struct{
	T_RRH_LOG_TEMP_RECORD_SLC	temp[D_RRH_SENSOR_SHT20_NUM_SLC];		/**< temprature										*/
	T_RRH_LOG_TEMP_RECORD_SLC	hum[D_RRH_SENSOR_SHT20_NUM_SLC];		/**< 湿度											*/
	T_RRH_LOG_TEMP_RECORD_SLC	pow;									/**< 電流電圧										*/
}T_RRH_LOG_TEMP_MAX_SLC;

/**
 * @brief	T_RRH_LOG_TEMP_SLC
 * @note	T_RRH_LOG_TEMP_SLC is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef	struct{
	UINT						uiWriteIdx;								/**< write index									*/
	UINT						uiCycleflg;								/**< Cyclic flg										*/
	USHORT						usCurCnt;								/**< 現在のカウンタ値[秒]							*/
	USHORT						usTimerVal;								/**< 現在の測定間隔[分]								*/
	USHORT						res;									/**< シャント抵抗値									*/
	USHORT						dummy;									/**< dummy											*/
	T_RRH_LOG_TEMP_RECORD_SLC	tRecord[D_RRH_LOG_TEMP_LOG_NUM];		/**< record											*/
	T_RRH_LOG_TEMP_MAX_SLC		tMax;									/**< sensor情報最大値								*/
}T_RRH_LOG_TEMP_SLC;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_SHARED_MEMORY_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SHARED_MEMORY_INFO
 * @note	T_RRH_SHARED_MEMORY_INFO is ..
 */
typedef struct{
	UINT									shmId;					/* shared memory id										*/
	UINT									shmSize;				/* shared memory size									*/
}T_RRH_SHARED_MEMORY_INFO;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_MNG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_MNG
 * @note	T_RRH_L2_STATISTICS_MNG is ..
 */
typedef struct
{
	UINT    index;
	UINT    dummy1;
	UINT    dummy2;
	UINT    dummy3;
	UINT    dummy4;
}T_RRH_L2_STATISTICS_MNG;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_SIG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_SIG
 * @note	T_RRH_L2_STATISTICS_SIG is ..
 */
typedef struct
{
	USHORT      singal_kind;
	USHORT      firstTime_flag;
	T_RRH_SYSTEM_TIME  firstTime;
	T_RRH_SYSTEM_TIME  lastTime;
	UINT        occur_counter;
	UINT        overflow_counter;
	union
	{
		struct
		{
			USHORT l2_tx_rv_flag;
		}l2_opt;
		struct
		{
			UINT   l3_ok_counter;
			UINT   l3_ng_counter;
			UINT   l3_last_ng_code;
		}l3_opt;
	}opt;
}T_RRH_L2_STATISTICS_SIG;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_L2BLK
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_L2BLK
 * @note	T_RRH_L2_STATISTICS_L2BLK is ..
 */
typedef struct
{
	T_RRH_L2_STATISTICS_SIG    sigs[D_RRH_L2_STATI_L2SIG_NUM];
}T_RRH_L2_STATISTICS_L2BLK;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_L2TABLE
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_L2TABLE
 * @note	T_RRH_L2_STATISTICS_L2TABLE is ..
 */
typedef struct
{
	T_RRH_L2_STATISTICS_MNG    mng;
	T_RRH_L2_STATISTICS_L2BLK  blks[D_RRH_L2_STATI_L2BLK_NUM];
}T_RRH_L2_STATISTICS_L2TABLE;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_L3BLK
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_L3BLK
 * @note	T_RRH_L2_STATISTICS_L3BLK is ..
 */
typedef struct
{
	T_RRH_L2_STATISTICS_SIG    sigs[D_RRH_L2_STATI_L3SIG_NUM];
}T_RRH_L2_STATISTICS_L3BLK;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_L2_STATISTICS_L3TABLE
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_STATISTICS_L3TABLE
 * @note	T_RRH_L2_STATISTICS_L3TABLE is ..
 */
typedef struct
{
	T_RRH_L2_STATISTICS_MNG    mng;
	T_RRH_L2_STATISTICS_L3BLK  blks[D_RRH_L2_STATI_L3BLK_NUM];
}T_RRH_L2_STATISTICS_L3TABLE;
/* @} */


/****************************************************************************************************************************/
/** T_RRH_L2_LOG_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_LOG_INFO
 * @note	T_RRH_L2_LOG_INFO is ..
 */
typedef struct
{
	T_RRH_SYSTEM_TIME  dt;
	UINT        tx_rv_flag;
	UINT        pro_thd_no;
	UINT        dummy1;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT        dummy2;
	USHORT      link_num;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT        lapb_type;
	USHORT      date_len;
	UCHAR       lapb_addr;
	UCHAR       lapb_cmd;
	UCHAR       data[D_RRH_L2_TXRV_LOG_DATA_LEN];
}T_RRH_L2_LOG_INFO;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_L2_LOG_INFO_TABLE
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_LOG_INFO_TABLE
 * @note	T_RRH_L2_LOG_INFO_TABLE is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	USHORT	cntTable;
	USHORT	dummy;
	USHORT	cntSpec[D_RRH_L2_TXRV_LOG_BLKS];
}T_RRH_L2_LOG_INFO_HEAD;

typedef struct
{
	T_RRH_L2_LOG_INFO  records[D_RRH_L2_TXRV_LOG_INF_NUM];
}T_RRH_L2_LOG_INFO_TABLE;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_L2_LOG_INFO_SPEC
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_LOG_INFO_SPEC
 * @note	T_RRH_L2_LOG_INFO_SPEC is ..
 */
typedef struct
{
	T_RRH_L2_LOG_INFO  spec[D_RRH_L2_TXRV_LOG_BLKS][D_RRH_L2_TXRV_LOG_SPEC_INF_NUM];
}T_RRH_L2_LOG_INFO_SPEC;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_L2_LOG_INFO_ALL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L2_LOG_INFO_ALL
 * @note	T_RRH_L2_LOG_INFO_ALL is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'head')
 */
typedef struct
{
	T_RRH_L2_LOG_INFO_HEAD	head;
	T_RRH_L2_LOG_INFO_TABLE	table;
	T_RRH_L2_LOG_INFO_SPEC	spec;
}T_RRH_L2_LOG_INFO_ALL;
/* @} */

/****************************************************************************************************************************/
/** T_CPR_L1ERR_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_CPR_L1ERR_TBL
 * @note	T_CPR_L1ERR_TBL is ..
 */
typedef struct
{
	T_RRH_SYSTEM_TIME lastTime;
	UINT count;
}T_CPR_L1ERR_STAT;

/**
 * @brief   L1 error information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	T_RRH_SYSTEM_TIME firstTime;
	T_CPR_L1ERR_STAT stateErr[6];
	UINT	cpeErr;
	UINT	los;
	UINT	rl1Inb;
    UINT    stateChg;
    UINT    hfnsyncChg;
	UINT	hdlcErr1;
	UINT	hdlcErr2;
	UINT	hdlcErr3;
	UINT	emio1;
	UINT	emio2;
}T_RRH_L1ERR_INFO;

/**
 * @brief   L1 error information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	T_RRH_SYSTEM_TIME time;
	UINT	state;
	UINT	cpeErr;
	UINT	cpmsk;
	UINT	los;
	UINT	rl1Inb;
	UINT	rxBuff;/*	★調査用ログ強化★	*/
	UINT	txBuff;/*	★調査用ログ強化★	*/
	UINT	hdlcErr1;
	UINT	hdlcErr2;
	UINT	hdlcErr3;
	UINT	lapmes;
	UINT	emio1;
	UINT	emio2;
	UCHAR	sfp[64];
	UINT	hfnsync;
	UINT    pllSt;
	UINT    sfpSt;
	UINT    cpstChg;
	UINT    cpSync;
}T_RRH_L1ERR_CRNTINFO;

/**
 * @brief   L1 error table
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	UINT	count_Idx;
	UINT	Info_Idx;
	UINT	dummy;

	T_RRH_L1ERR_INFO		countTbl[D_RRH_L1_ERR_COUNT_SIZE];
	T_RRH_L1ERR_CRNTINFO	infoTbl[D_RRH_L1_ERR_INFO_SIZE];
}T_RRH_L1ERR_TBL_FHM;

typedef struct
{
	T_RRH_SYSTEM_TIME firstTime;
	T_CPR_L1ERR_STAT stateErr[6];
	UINT	cpeErr;
	UINT	los;
	UINT	rl1Inb;
	UINT	stateChg;
	UINT	hfnsyncChg;
	UINT	hdlcErr1;
	UINT	hdlcErr2;
	UINT	hdlcErr3;
	UINT	emio1;
	UINT	emio2;
	UINT	phase;
	UINT	crcNg;
}T_RRH_L1ERR_20MCOUNT;

typedef struct
{
	T_RRH_SYSTEM_TIME time;
	UINT	state;
	UINT	cpeErr;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT	cpmsk;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT	los;
	UINT	rl1Inb;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT	rxBuff;/*	★調査用ログ強化★	*/
	UINT	txBuff;/*	★調査用ログ強化★	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT	stateChg;
	UINT	hfnsyncChg;
	UINT	hdlcErr1;
	UINT	hdlcErr2;
	UINT	hdlcErr3;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT	lapmes;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT	emio1;
	UINT	emio2;
	UCHAR	sfp[64];
	UINT	hfnsync;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT    pllSt;
	UINT    sfpSt;
	UINT    cpstChg;
	UINT    cpSync;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT	phase;
	UINT	crcNg;
}T_RRH_L1ERR_20MINFO;

typedef struct
{
	UINT	count20m_Idx;
	UINT	Info20m_Idx;
	UINT	dummy;

	T_RRH_L1ERR_20MCOUNT countTbl[D_RRH_L1_ERR_20MCOUNT_SIZE];
	T_RRH_L1ERR_20MINFO	infoTbl[D_RRH_L1_ERR_20MINFO_SIZE];
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT	count_Idx;
	UINT	Info_Idx;
	// UINT	dummy;

	T_RRH_L1ERR_INFO		countTbl_fhm[D_RRH_L1_ERR_COUNT_SIZE];
	T_RRH_L1ERR_CRNTINFO	infoTbl_fhm[D_RRH_L1_ERR_INFO_SIZE];
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}T_RRH_L1ERR_TBL;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_L1ERR_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L1ERR_TBL
 * @note	T_RRH_L1ERR_TBL is ..
 */

typedef struct
{
	T_RRH_SYSTEM_TIME time;
	UINT cpristat;
	UINT cptrans;
	UINT irqvalue;
}T_RRH_L1LOSLOF_HIS;

typedef struct
{
	UINT index;
	UINT dummy[7];
	T_RRH_L1LOSLOF_HIS hisInfo[D_RRH_L1_LOSLOF_SIZE];
}T_RRH_L1LOSLOF_TBL;
/* @} */


/****************************************************************************************************************************/
/** T_RRH_L1STAT_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_L1STAT_TBL
 * @note	T_RRH_L1STAT_TBL is ..
 */
typedef struct
{
	USHORT      singal_kind;
	USHORT      firstTime_flag;
	T_RRH_SYSTEM_TIME  firstTime;
	T_RRH_SYSTEM_TIME  lastTime;
	UINT        occur_counter;
	UINT        overflow_counter;
	UINT        dummy[3];
}T_RRH_L1STAT_BLK;

typedef struct
{
	UINT index;
	UINT dummy[3];
	T_RRH_L1STAT_BLK blks[D_RRH_L1_STATI_L1BLK_NUM][D_RRH_L1_STATI_L1SIG_NUM];
}T_RRH_L1STAT_TBL;
/* @} */

/****************************************************************************************************************************/
/** T_RRH_SVSTATELOG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SVSTATELOG
 * @note	T_RRH_SVSTATELOG is ..
 */
typedef struct
{
	UINT	chgType;
	UINT	svState[16];
}T_RRH_SVSTATELOG;
/* @}  */

/****************************************************************************************************************************/
/** T_RRH_SVSTATE_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SVSTATE_RECORD
 * @note	T_RRH_SVSTATE_RECORD is ..
 */
typedef struct
{
	T_RRH_SYSTEM_TIME	sysTime;
	T_RRH_SVSTATELOG	svStateLog;
	UCHAR				optStr[36];	/**< filename									*/
	UCHAR				ucFileName[D_RRH_LOG_MAX_ASSERTFILENAME];	/**< filename									*/
	UINT				uiLine;									/**< Line											*/
}T_RRH_SVSTATE_RECORD;
/* @}  */

/****************************************************************************************************************************/
/** T_RRH_SVSTATECHG_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SVSTATECHG_HIS
 * @note	T_RRH_SVSTATECHG_HIS is ..
 */
typedef struct
{
	UINT		logIndex;
	T_RRH_SVSTATE_RECORD		svStateRecord[D_RRH_SVLOG_MAX];
}T_RRH_SVSTATECHG_HIS;
/* @}  */
/****************************************************************************************************************************/
/** T_RRH_SVCSTATECHG_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SVCSTATECHG_HIS
 * @note	T_RRH_SVCSTATECHG_HIS is ..
 */
typedef struct
{
	UINT		logIndex;
	T_RRH_SVSTATE_RECORD		svStateRecord[D_RRH_SVCLOG_MAX];
}T_RRH_SVCSTATECHG_HIS;
/* @}  */

/****************************************************************************************************************************/
/** T_RRH_SFPINFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPINFO
 * @note	T_RRH_SFPINFO is ..
 */
typedef struct
{
	USHORT	fefver;			/* FEF version			*/
	USHORT	mountstate;		/* 実装/未実装 			*/
	UCHAR	sfpinf[D_RRH_SFPINFO_LEN];	/* SFP情報(実装時のみ) 	*/
}T_RRH_SFPINFO;

 /****************************************************************************************************************************/
/** T_RRH_SFPINFO_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPINFO_RECORD
 * @note	T_RRH_SFPINFO_RECORD is ..
 */
typedef struct
{
	T_RRH_SYSTEM_TIME	sysTime;	/* RTCから取得した時刻(BCD+umsecカウンタ) */
	T_RRH_SFPINFO		sfpinfo;
}T_RRH_SFPINFO_RECORD;
/* @}  */

/* @}  */
/****************************************************************************************************************************/
/** T_RRH_SFPINFO_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPINFO_HIS
 * @note	T_RRH_SFPINFO_HIS is ..
 */
typedef struct
{
	UINT					logIndex;						/* 次書き込み位置 */
	T_RRH_SFPINFO_RECORD	sfpinfrec[D_RRH_SFPLOG_MAX];
}T_RRH_SFPINFO_HIS;
/* @}  */



/****************************************************************************************************************************/
/** T_RRH_LOG_BUFF_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_BUFF_HIS_RECORD
 * @note	T_RRH_LOG_BUFF_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						pid;									/**< process id										*/
	UINT						tid;									/**< thread id										*/
	UINT						get_release;							/**< 取得解放種別									*/
	UINT						buffer_kind;							/**< バッファ種別									*/
}T_RRH_LOG_BUFF_HIS_RECORD;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_BUFF_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_BUFF_HIS
 * @note	T_RRH_LOG_BUFF_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;							/**< write index									*/
	UINT							uiCycleflg;							/**< Cyclic flg										*/
	UINT							dummy[2];							/**< dummy											*/
	T_RRH_LOG_BUFF_HIS_RECORD		tRecord[D_RRH_LOG_BUFFLOG_RCD_NUM];	/**< 通信用共有メモリ取得解放 log					*/
}T_RRH_LOG_BUFF_HIS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_TIMER_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_TIMER_HIS_RECORD
 * @note	T_RRH_LOG_TIMER_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						timerID;								/**< timer ID										*/
	UINT						kind;									/**< 種別											*/
	UINT						eventNo;								/**< eventNo										*/
	UINT						sendQueue;								/**< QueueID										*/
}T_RRH_LOG_TIMER_HIS_RECORD;

typedef struct {
	UINT							uiWriteIdx;							/**< write index									*/
	UINT							uiCycleflg;							/**< Cyclic flg										*/
	UINT							dummy[2];							/**< dummy											*/
	T_RRH_LOG_TIMER_HIS_RECORD		tRecord[D_RRH_LOG_TIMERLOG_RCD_NUM];	/**< Timer履歴 log								*/
}T_RRH_LOG_TIMER_HIS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_SEM_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_SEM_HIS_RECORD
 * @note	T_RRH_LOG_SEM_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			tDate;									/**< time											*/
	UINT						semID;									/**< semaphore id									*/
	UINT						kind;									/**< kind											*/
}T_RRH_LOG_SEM_HIS_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_SEM_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_SEM_HIS
 * @note	T_RRH_LOG_SEM_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;							/**< write index									*/
	UINT							uiCycleflg;							/**< Cyclic flg										*/
	UINT							dummy[2];							/**< dummy											*/
	T_RRH_LOG_SEM_HIS_RECORD		tRecord[D_RRH_LOG_SEMAPHORE_RCD_NUM];	/**< semaphore log								*/
}T_RRH_LOG_SEM_HIS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_WDT
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_WDT
 * @note	T_RRH_LOG_WDT is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				tDate;								/**< time											*/
	UINT							softVer;							/**< ソフトファイルバージョン						*/
	UINT							logGetFlg;							/**< 6秒オーバー直後、一時的にログ取得を停止する	*/
}T_RRH_LOG_WDT_HEAD;

typedef struct{
	struct timespec					tDate;								/**< time											*/
	struct timespec					tDate_end;							/**< time(終了時刻)									*/
	UINT							regVal[2];							/**< レジスタ情報									*/
}T_RRH_LOG_WDT_CLR_RCD;

typedef struct{
	UINT							index;								/**< log index										*/
	T_RRH_LOG_WDT_CLR_RCD			rcd[D_RRH_LOG_WDT_CLR_NUM];			/**< log record										*/
}T_RRH_LOG_WDT_CLR;

typedef struct{
	struct timespec					tDate;								/**< time											*/
	struct timespec					tDate_end;							/**< time(終了時刻)									*/
	UINT							regVal[2];							/**< レジスタ情報(残り時刻)							*/
}T_RRH_LOG_WDT_OVER_RCD;

typedef struct{
	UINT							index;								/**< log index										*/
	T_RRH_LOG_WDT_OVER_RCD			rcd[D_RRH_LOG_WDT_OVER_NUM];		/**< log record										*/
}T_RRH_LOG_WDT_OVER;

typedef struct{
	struct timespec					tDate;								/**< time											*/
	struct timespec					tDate_end;							/**< time(終了時刻)									*/
	UINT							evtNo;								/**< イベント番号									*/
}T_RRH_LOG_WDT_RUNHIS_RCD;

typedef struct{
	UINT							index;								/**< log index										*/
	T_RRH_LOG_WDT_RUNHIS_RCD		rcd[D_RRH_LOG_WDT_RUNHIS_NUM];		/**< log record										*/
}T_RRH_LOG_WDT_RUNHIS;

typedef struct {
	T_RRH_LOG_WDT_HEAD				head;								/**< header info									*/
	T_RRH_LOG_WDT_CLR				clr;								/**< クリアログ										*/
	T_RRH_LOG_WDT_OVER				over;								/**< WDTクリアタイミング超過ログ					*/
	T_RRH_LOG_WDT_RUNHIS			runhis[E_RRH_LOG_WDT_RUNHIS_NUM];	/**< 各スレッドの走行ログ							*/
}T_RRH_LOG_WDT;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_ECC
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_WDT
 * @note	T_RRH_LOG_WDT is ..
 */

typedef struct{
	T_RRH_SYSTEM_TIME				tDate;								/**< time											*/
	USHORT							ver;								/**< version										*/
	USHORT							dummy;								/**< dummy											*/
	UINT							regVal[D_RRH_LOG_ECC_REG_TBL_NUM];	/**< レジスタ情報									*/
}T_RRH_LOG_ECC_RCD;

typedef struct{
	UINT							index;								/**< log index										*/
	T_RRH_LOG_ECC_RCD				rcd[D_RRH_LOG_ECC_RCD_NUM];			/**< log record										*/
}T_RRH_LOG_ECC;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_DIARESULT_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_DIARESULT_TBL
 * @note	T_RRH_DIARESULT_TBL is ..
 */
typedef struct{
	UINT		LMChksumRet[2];											/**< APL-LM checksum result							*/
	UINT		PLFPGAChksumRet[2];										/**< PL-FPGA checksumReg result						*/
	UINT		TXFPGAChksumRet[2];										/**< TX-FPGA checksumReg result						*/
	UINT		CPRIFPGAChksumRet[2];									/**< CPRI-FPGA checksumReg result					*/
	UINT		workMemRWChkRet;										/**< Work Memory access check result				*/
	UINT		regAccessChk[E_RRH_DIA_REGCHK_NUM];						/**< Reg R/W check result							*/
	UINT		clementeRWChkRet[2];									/**< クレメンテ R/W check result					*/
	UINT		clementeChksumRet[2];									/**< クレメンテ checksum result						*/
	UINT		sfpCheck;												/**< SPF check result								*/
	UINT		fpgaConfig;												/**< FPGA Config result								*/
}T_RRH_DIARESULT_TBL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_SFPINFLOG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPINFLOG
 * @note	T_RRH_SFPINFLOG is ..
 */
typedef struct{
	USHORT					fef_ver;										/**< FEF Version								*/
	USHORT					sfp_mount;										/**< 実装/未実装情報								*/
	USHORT					sfp_inf;										/**< SFP情報(実装時のみ)								*/
}T_RRH_SFPINFLOG;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_SFPINF_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPINF_RECORD
 * @note	T_RRH_SFPINF_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME		sysTime;										/**< System Time								*/
	T_RRH_SFPINFLOG			sfpInfLog;										/**< SFP Info Log									*/
}T_RRH_SFPINF_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_SFPMNGINF_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_SFPMNGINF_HIS
 * @note	T_RRH_SFPMNGINF_HIS is ..
 */
typedef struct{
	UINT					logIndex;										/**< write index								*/
	T_RRH_SFPINF_RECORD		sfpInfRecord[2];								/**< SFP record									*/
}T_RRH_SFPMNGINF_HIS;
/** @} */

/******************************************************************************************************************************/
/** Inventory Manage Table
 *  @addtogroup RRH_RRH
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Inventory Manage Table
 * @note	Inventory Manage Table is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */

typedef struct{
	USHORT				cmx_cardname[6];		/**< Card name   */
	USHORT				cmx_pcbno1;				/**< Product number (Year/Month/Day)   */
	USHORT				cmx_pcbno2;				/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;				/**< Product number serial (low word)   */
	USHORT				cmx_pcbbyer;			/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_pcbbday;			/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver1;				/**< Hardware version number(high word)   */
	USHORT				cmx_hver2;				/**< Hardware version number(low word)   */
	USHORT				cmx_hver3;				/**< Hardware version number(low word)   */

	USHORT				cmx_mno[6];				/**< Card map number   */
	USHORT				cmx_f0sver[3];			/**< FLASH 0 plane software version   */
	USHORT				cmx_f1sver[3];			/**< FLASH 1 plane software version   */
	USHORT				cmx_fprom[4];			/**< Factory usage Address    */
	USHORT				cmx_mac_01;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03;				/**< MAC address (card)   */

	USHORT				cmx_ipup;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn;				/**< IP address (low word)   */
	USHORT				cmx_netmskup;			/**< Net mask (high word)   */
	USHORT				cmx_netmskdwn;			/**< Net mask (low word)   */
	USHORT				cmx_dfltrtrup;			/**< Default router (high word)   */
	USHORT				cmx_dfltrtrdwn;			/**< Default router (low word)   */
	USHORT				cmx_command_port;		/**< Command transfer port   */
	USHORT				cmx_file_port;			/**< File transfer port   */
	USHORT				cmx_port;				/**< Port number   */

	USHORT				cmx_erase_firm_flg;		/**< Flash erase flag for DL use (FIRM)   */
	USHORT				cmx_erase_fpga_flg;		/**< Flash erase flag for DL use (FPGA)   */
	USHORT				cmx_erase_tx_flg;		/**< Flash erase flag for DL use (TX-FPGA)   */
	USHORT				cmx_erase_rx_flg;		/**< Flash erase flag for DL use (RX-FPGA)   */
	USHORT				cmx_0fpgaver[3];		/**< Flash 0 plane FPGA version   */
	USHORT				cmx_1fpgaver[3];		/**< Flash 1 plane FPGA version   */

	USHORT				cmx_com_bootmen;		/**< FPGA boot info   */
	USHORT				cmx_cpri_bootmen;		/**< CPRI boot info   */
	USHORT				cmx_vupu_bootmen;		/**< VUPU boot info   */
	USHORT				cmx_dummy;				/**< MPU boot info   */

	UCHAR				cmx_device_typ;			/**< device type   */
	UCHAR				cmx_device_BW;			/**< bandwith type   */
	USHORT				cmx_syspara_rcv_flg;	/**< system parameter receive flg */
	USHORT				cmx_tilt_con_inf;		/**< TILT connection information   */
	USHORT				cmx_tra_inf_typ;		/**< RE side TRAINF type */
	UCHAR				cmx_ipadd[4];			/**< ip address */
	UCHAR				cmx_netmask[4];			/**< netmask address */
	USHORT				cmx_gateaddr_h;			/**< gate address(hword) */
	USHORT				cmx_gateaddr_l;			/**< gate address(hword) */
	USHORT				cmx_fldcmd_port_s3g;	/**< fld command port */
	USHORT				cmx_fldfil_port_s3g;	/**< fld file port  */
	USHORT				cmx_svdata_s3g;
	UCHAR				cmx_dummy3[20];
	UCHAR				cmx_rollbackOFF;		/**< ロールバックOFF設定値情報【SRE】  */
	UCHAR				cmx_rollback_hist;		/**< ロールバック状態履歴情報【SRE】  */
	UCHAR				cmx_erase_apdic_flg[2];	/**< DL用Flashイレースフラグ(APD-IC Boot)【SRE】  */
	UCHAR				cmx_0apdicver[2];		/**< Flash0面FPGA代表バージョン3(APD-IC Boot)【SRE】  */
	UCHAR				cmx_1apdicver[2];		/**< Flash1面FPGA代表バージョン3(APD-IC Boot)【SRE】  */
	UCHAR				cmx_dummy4[86];

	UCHAR				eeprom_checksum[2];					/**< EEPROMチェックサム												*/
	UCHAR				eeprom_data_ver[2];					/**< EEPデータバージョン											*/
	UCHAR				re_system_info[2];					/**< RE優先システム情報												*/
	UCHAR				s3g_bandwidth[2];					/**< S3G帯域幅														*/
	UCHAR				carrier_flag[2];					/**< 3Gキャリア運用有無												*/
	UCHAR				send_tono[2];						/**< 送信系統数														*/
	UCHAR				sector_234[2];						/**sector chip revision (#2,#3,#4)									*/
	UCHAR				sector_567[2];						/**sector chip revision (#5,#6,#7)									*/
	UCHAR				dummy1[4];							/**< 予備															*/
	UCHAR				carrier_3gfrq[8];					/**< 3Gキャリア運用周波数											*/
	UCHAR				mtExt_data_3g_flg[2];				/**< 対3G用MT信号/外部装置データ対応有無							*/
	UCHAR				syspara_wait_timer[2];				/**< システムパラメータ転送待ちタイマ								*/
	UCHAR				firm_sndtimer[2];					/**< ファームウェア送信確認タイマ									*/
	UCHAR				varNameF[2];						/**< 変数名#F予備													*/
	USHORT				vupu_flash_write_flg;				/**< VUPU用FLASHエリア書込み有無フラグ								*/
	USHORT				vupu_flash_0_ver;					/**< Flash0面VUPU代表バージョン										*/
	USHORT				vupu_flash_1_ver;					/**< Flash1面VUPU代表バージョン										*/
	USHORT				vupu_size_h;						/**< VUPUサイズ格納(上位)											*/
	USHORT				vupu_size_l;						/**< VUPUサイズ格納(下位)											*/
	UCHAR				dummy2[2];							/**< 予備															*/
	USHORT				cmx_erase_vupu_tmp_flg;				/**< DL用Flashイレースフラグ(VUPU-TMP-FLASH(QSPI側)用)				*/
	USHORT				cmx_erase_vupu_flg;					/**< DL用Flashイレースフラグ(VUPU用FLASH)							*/
	UCHAR				firm_flash0_mpu_checksum[4];		/**< チェックサム計算用サイズMPUファームウェア FLASH 0面			*/
	UCHAR				firm_flash0_rre_checksum[4];		/**< チェックサム計算用サイズCPRI-FPGA(RRE) FLASH 0面				*/
	UCHAR				firm_flash0_tx_checksum[4];			/**< チェックサム計算用サイズTX側RFデバイス FLASH 0面				*/
	UCHAR				firm_flash0_rx_checksum[4];			/**< チェックサム計算用サイズRX側RFデバイス FLASH 0面				*/
	UCHAR				firm_flash0_pl_checksum[4];			/**< チェックサム計算用サイズPL-FPGA FLASH 0面						*/
	UCHAR				firm_flash1_mpu_checksum[4];		/**< チェックサム計算用サイズMPUファームウェア FLASH 1面			*/
	UCHAR				firm_flash1_rre_checksum[4];		/**< チェックサム計算用サイズCPRI-FPGA(RRE) FLASH 1面				*/
	UCHAR				firm_flash1_tx_checksum[4];			/**< チェックサム計算用サイズTX側RFデバイス FLASH 1面				*/
	UCHAR				firm_flash1_rx_checksum[4];			/**< チェックサム計算用サイズRX側RFデバイス FLASH 1面				*/
	UCHAR				firm_flash1_pl_checksum[4];			/**< チェックサム計算用サイズPL-FPGA FLASH 1面						*/
	UCHAR				dummy3[8];							/**< 予備															*/
	UCHAR				bootver0_pr[2];						/**< BOOT版数(0面 Primary)											*/
	UCHAR				bootver0_bk[2];						/**< BOOT版数(0面 Backup)											*/
	UCHAR				bootver1_pr[2];						/**< BOOT版数(1面 Primary)											*/
	UCHAR				bootver1_bk[2];						/**< BOOT版数(1面 Backup)											*/
	UCHAR				sta_bootver0_pr[2];					/**< BOOT版数(FHM起動時 0面 Primary)								*/
	UCHAR				sta_bootver0_bk[2];					/**< BOOT版数(FHM起動時 0面 Backup)									*/
	UCHAR				sta_bootver1_pr[2];					/**< BOOT版数(FHM起動時 1面 Primary)								*/
	UCHAR				sta_bootver1_bk[2];					/**< BOOT版数(FHM起動時 1面 Backup)									*/
	UCHAR				dummy4[144];						/**< 予備															*/
	UCHAR				troublelog_File[1];					/**< 障害ログ退避面													*/
	UCHAR				cyclicklog_File[1];					/**< サイクリックログ退避面											*/
	UCHAR				cyclicklog0_erase[1];				/**< サイクリックログイレースフラグ0面用【SRE】						*/
	UCHAR				cyclicklog1_erase[1];				/**< サイクリックログイレースフラグ1面用【SRE】						*/
	UCHAR				dummy5[1];							/**< 予備															*/
	UCHAR				softreset_flag[1];					/**< ソフトリセットフラグ											*/
	UCHAR				almreset_flag[1];					/**< ALM自律リセット有無フラグ										*/
	UCHAR				gain_setting[4];					/**< ゲイン微調設定値												*/
	UCHAR				dummy6[5];							/**< 予備															*/
	UCHAR				real_trlog_ipfilter[4];				/**< リアル通信ログIPフィルタ										*/
	UCHAR				real_trlog_output[1];				/**< リアル通信ログ出力先											*/
	UCHAR				softreset_mask[1];					/**< ソフトリセットマスク											*/
	UCHAR				softboot_mode[1];					/**< ソフト起動モード												*/
	UCHAR				dummy7[5];							/**< 予備															*/
	UCHAR				cpri_ctle1;							/**< CPRI CTLE1														*/
	UCHAR				cpri_ctle2;							/**< CPRI CTLE2														*/
	UCHAR				cpri_ctle3;							/**< CPRI CTLE3														*/
	UCHAR				cpri_dcd;							/**< CPRI DCD														*/
	UCHAR				pa_onoff_status[1];					/**< PA ON/OFF状態													*/
	UCHAR				cpri_rate_static;					/**< 9.8G強制切り替えモード											*/
	UCHAR				timesave_flg;						/**< 時刻退避フラグ													*/
	UCHAR				timesaveinfo[8];					/**< 時刻退避情報													*/
	UCHAR				dummy8[213];						/**< 予備															*/
	UCHAR				tp_debug[2];						/**< TPデバック														*/
	UCHAR				dummy9[253];						/**< 予備															*/
}T_RRH_TRAINVTBL_FHM;

typedef struct{
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				cmx_cardname[6];			/**< Card name   */
	USHORT				cmx_pcbno1;				    /**< Product number (Year/Month/Day)   */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT				cmx_dummy0[2];
	USHORT				cmx_mno[6];					/**< Card map number   */
	USHORT				cmx_dummy00[2];
	USHORT				cmx_pcbbyer;				/**< Product number (Year)              */
	USHORT				cmx_pcbbday;				/**< Product number (Month/Day)         */
	USHORT				cmx_pcbno2;					/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;					/**< Product number serial (low word)   */
	USHORT				cmx_testyer;				/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_testday;				/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver1;					/**< Hardware version number(high word)   */
	USHORT				cmx_hver2;					/**< Hardware version number(low word)   */
	USHORT				cmx_hver3;					/**< Hardware version number(low word)   */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				cmx_f0sver[3];			/**< FLASH 0 plane software version   */
	USHORT				cmx_f1sver[3];			/**< FLASH 1 plane software version   */
	USHORT				cmx_fprom[4];			/**< Factory usage Address    */
	USHORT				cmx_mac_01;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03;				/**< MAC address (card)   */

	USHORT				cmx_ipup;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn;				/**< IP address (low word)   */
	USHORT				cmx_netmskup;			/**< Net mask (high word)   */
	USHORT				cmx_netmskdwn;			/**< Net mask (low word)   */
	USHORT				cmx_dfltrtrup;			/**< Default router (high word)   */
	USHORT				cmx_dfltrtrdwn;			/**< Default router (low word)   */
	USHORT				cmx_command_port;		/**< Command transfer port   */
	USHORT				cmx_file_port;			/**< File transfer port   */
	USHORT				cmx_port;				/**< Port number   */

	USHORT				cmx_erase_firm_flg;		/**< Flash erase flag for DL use (FIRM)   */
	USHORT				cmx_erase_fpga_flg;		/**< Flash erase flag for DL use (FPGA)   */
	USHORT				cmx_erase_tx_flg;		/**< Flash erase flag for DL use (TX-FPGA)   */
	USHORT				cmx_erase_rx_flg;		/**< Flash erase flag for DL use (RX-FPGA)   */
	USHORT				cmx_0fpgaver[3];		/**< Flash 0 plane FPGA version   */
	USHORT				cmx_1fpgaver[3];		/**< Flash 1 plane FPGA version   */

	USHORT				cmx_com_bootmen;		/**< FPGA boot info   */
	USHORT				cmx_cpri_bootmen;		/**< CPRI boot info   */
	USHORT				cmx_vupu_bootmen;		/**< VUPU boot info   */
	USHORT				cmx_dummy;				/**< MPU boot info   */

	USHORT				cmx_syspara_rcv_flg;	/**< system parameter receive flg */
	USHORT				cmx_tilt_con_inf;		/**< TILT connection information   */
	USHORT				cmx_tra_inf_typ;		/**< RE side TRAINF type */
	UCHAR				cmx_ipadd[4];			/**< ip address */
	UCHAR				cmx_netmask[4];			/**< netmask address */
	USHORT				cmx_gateaddr_h;			/**< gate address(hword) */
	USHORT				cmx_gateaddr_l;			/**< gate address(hword) */
	USHORT				cmx_fldcmd_port_s3g;	/**< fld command port */
	USHORT				cmx_fldfil_port_s3g;	/**< fld file port  */
	USHORT				cmx_svdata_s3g;
	UCHAR				cmx_rollbackOFF;		/**< ロールバックOFF設定値情報【SRE】  */
	UCHAR				cmx_rollback_hist;		/**< ロールバック状態履歴情報【SRE】  */
	UCHAR				cmx_erase_apdic_flg[2];	/**< DL用Flashイレースフラグ(APD-IC Boot)【SRE】  */
	UCHAR				cmx_0apdicver[2];		/**< Flash0面FPGA代表バージョン3(APD-IC Boot)【SRE】  */
	UCHAR				cmx_1apdicver[2];		/**< Flash1面FPGA代表バージョン3(APD-IC Boot)【SRE】  */
	UCHAR				eeprom_checksum[2];					/**< EEPROMチェックサム												*/
	UCHAR				eeprom_data_ver[2];					/**< EEPデータバージョン											*/
	UCHAR				re_system_info[2];					/**< RE優先システム情報												*/
	UCHAR				s3g_bandwidth[2];					/**< S3G帯域幅														*/
	UCHAR				carrier_flag[2];					/**< 3Gキャリア運用有無												*/
	UCHAR				send_tono[2];						/**< 送信系統数														*/
	UCHAR				sector_234[2];						/**sector chip revision (#2,#3,#4)									*/
	UCHAR				sector_567[2];						/**sector chip revision (#5,#6,#7)									*/
	UCHAR				dummy1[4];							/**< 予備															*/
	UCHAR				carrier_3gfrq[8];					/**< 3Gキャリア運用周波数											*/
	UCHAR				mtExt_data_3g_flg[2];				/**< 対3G用MT信号/外部装置データ対応有無							*/
	UCHAR				syspara_wait_timer[2];				/**< システムパラメータ転送待ちタイマ								*/
	UCHAR				firm_sndtimer[2];					/**< ファームウェア送信確認タイマ									*/
	UCHAR				varNameF[2];						/**< 変数名#F予備													*/
	USHORT				vupu_flash_write_flg;				/**< VUPU用FLASHエリア書込み有無フラグ								*/
	USHORT				vupu_flash_0_ver;					/**< Flash0面VUPU代表バージョン										*/
	USHORT				vupu_flash_1_ver;					/**< Flash1面VUPU代表バージョン										*/
	USHORT				vupu_size_h;						/**< VUPUサイズ格納(上位)											*/
	USHORT				vupu_size_l;						/**< VUPUサイズ格納(下位)											*/
	UCHAR				dummy2[2];							/**< 予備															*/
	USHORT				cmx_erase_vupu_tmp_flg;				/**< DL用Flashイレースフラグ(VUPU-TMP-FLASH(QSPI側)用)				*/
	USHORT				cmx_erase_vupu_flg;					/**< DL用Flashイレースフラグ(VUPU用FLASH)							*/
	UCHAR				firm_flash0_mpu_checksum[4];		/**< チェックサム計算用サイズMPUファームウェア FLASH 0面			*/
	UCHAR				firm_flash0_rre_checksum[4];		/**< チェックサム計算用サイズCPRI-FPGA(RRE) FLASH 0面				*/
	UCHAR				firm_flash0_tx_checksum[4];			/**< チェックサム計算用サイズTX側RFデバイス FLASH 0面				*/
	UCHAR				firm_flash0_rx_checksum[4];			/**< チェックサム計算用サイズRX側RFデバイス FLASH 0面				*/
	UCHAR				firm_flash0_pl_checksum[4];			/**< チェックサム計算用サイズPL-FPGA FLASH 0面						*/
	UCHAR				firm_flash1_mpu_checksum[4];		/**< チェックサム計算用サイズMPUファームウェア FLASH 1面			*/
	UCHAR				firm_flash1_rre_checksum[4];		/**< チェックサム計算用サイズCPRI-FPGA(RRE) FLASH 1面				*/
	UCHAR				firm_flash1_tx_checksum[4];			/**< チェックサム計算用サイズTX側RFデバイス FLASH 1面				*/
	UCHAR				firm_flash1_rx_checksum[4];			/**< チェックサム計算用サイズRX側RFデバイス FLASH 1面				*/
	UCHAR				firm_flash1_pl_checksum[4];			/**< チェックサム計算用サイズPL-FPGA FLASH 1面						*/
	UCHAR				dummy3[8];							/**< 予備															*/
	UCHAR				bootver0_pr[2];						/**< BOOT版数(0面 Primary)											*/
	UCHAR				bootver0_bk[2];						/**< BOOT版数(0面 Backup)											*/
	UCHAR				bootver1_pr[2];						/**< BOOT版数(1面 Primary)											*/
	UCHAR				bootver1_bk[2];						/**< BOOT版数(1面 Backup)											*/
	UCHAR				sta_bootver0_pr[2];					/**< BOOT版数(FHM起動時 0面 Primary)								*/
	UCHAR				sta_bootver0_bk[2];					/**< BOOT版数(FHM起動時 0面 Backup)									*/
	UCHAR				sta_bootver1_pr[2];					/**< BOOT版数(FHM起動時 1面 Primary)								*/
	UCHAR				sta_bootver1_bk[2];					/**< BOOT版数(FHM起動時 1面 Backup)									*/
	UCHAR				dummy4[144];						/**< 予備															*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UCHAR				cmx_dummy1[14];
	UCHAR				cmx_device_typ;				/**< 装置種別 */
	UCHAR				cmx_device_BW;				/**< 対応周波数帯 */
	UCHAR				cmd_boardver;				/**< ボードバージョン */
	UCHAR				cmx_dummy2[13];
	UCHAR				cmx_bootup_face;			/**< 起動面   */
	UCHAR				cmx_swconf_face;			/**< Softパラメータ用   */
	UCHAR				cmx_dummy3[30];

	/* slot0 */
	USHORT				cmx_f0_sver[3];				/**< FLASH 0 plane software version   */
	USHORT				cmx_f0_soft_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f0_fpga_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f0_trxfpga_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f0_rficboot_ver;		/**< FLASH 0 software version   */
	USHORT				cmx_f0_rficarm_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f0_swconf_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f0_ptpconf_ver;			/**< FLASH 0 PTP version   */
	UCHAR				cmx_dummy4[18];
	UCHAR				cmx_f0_soft_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f0_fpga_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f0_trxfpga_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f0_rficboot_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f0_rficarm_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f0_swconf_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f0_ptpconf_eraseflg;		/**< FLASH 0 PTP version   */
	UCHAR				cmx_dummy5[19];

	/* slot1 */
	USHORT				cmx_f1_sver[3];				/**< FLASH 0 plane software version   */
	USHORT				cmx_f1_soft_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f1_fpga_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f1_trxfpga_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f1_rficboot_ver;		/**< FLASH 0 software version   */
	USHORT				cmx_f1_rficarm_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f1_swconf_ver;			/**< FLASH 0 software version   */
	USHORT				cmx_f1_ptpconf_ver;			/**< FLASH 1 PTP version   */
	UCHAR				cmx_dummy6[18];
	UCHAR				cmx_f1_soft_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f1_fpga_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f1_trxfpga_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f1_rficboot_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f1_rficarm_eraseflg;	/**< FLASH 0 software version   */
	UCHAR				cmx_f1_swconf_eraseflg;		/**< FLASH 0 software version   */
	UCHAR				cmx_f1_ptpconf_eraseflg;	/**< FLASH 1 PTP version   */
	UCHAR				cmx_dummy7[19];
	UCHAR				cmx_dummy8[64];				/**< factory */
	UCHAR				cmx_soft_fsize[3][4];		/**< FLASH 0 software version   */
	UCHAR				cmx_fpga_fsize[3][4];		/**< FLASH 0 software version   */
	UCHAR				cmx_trxfpga_fsize[3][4];	/**< FLASH 0 software version   */
	UCHAR				cmx_rficboot_fsize[3][4];	/**< FLASH 0 software version   */
	UCHAR				cmx_rficarm_fsize[3][4];	/**< FLASH 0 software version   */
	UCHAR				cmx_swconf_fsize[3][4];		/**< FLASH 0 software version   */
	UCHAR				cmx_dummy9[4];	
	UCHAR				cmx_slot_status[2];	
	UCHAR				cmx_dummy10[2];	
	UCHAR				cmx_build0_name[32];
	UCHAR				cmx_dummy11_1[2];
	UCHAR				cmx_build_size[3][4];
	UCHAR				cmx_ptpcond_size[3][4];
	UCHAR				cmx_dummy11_2[6];
	UCHAR				cmx_build1_name[32];
	UCHAR				cmx_dummy11[32];
	USHORT				cmx_mac_01_mt;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02_mt;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03_mt;				/**< MAC address (card)   */
	USHORT				cmx_ipup_mt;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn_mt;				/**< IP address (low word)   */
	UCHAR				cmx_dummy12[22];	
	USHORT				cmx_mac_01_p0;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02_p0;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03_p0;				/**< MAC address (card)   */
	USHORT				cmx_ipup_p0;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn_p0;				/**< IP address (low word)   */
	UCHAR				cmx_dummy13[54];
	USHORT				cmx_mac_01_p1;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02_p1;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03_p1;				/**< MAC address (card)   */
	USHORT				cmx_ipup_p1;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn_p1;				/**< IP address (low word)   */
	UCHAR				cmx_dummy14[54];
	UCHAR				cmx_dummy15[64];
	UCHAR				cmx_dummy16[64];
	USHORT				cmx_low_vid;				/**< lowest-vlan-id   */
	USHORT				cmx_high_vid;				/**< highest-vlan-id   */
	USHORT				cmx_used_vid;				/**< last-used-vlan-id   */
	UCHAR				cmx_ptp_conf_pattern;		/**< PTP config pattern   */
	UCHAR				cmx_mp_pcp;					/**< m-plane pcp   */
	UCHAR				cmx_vlanscan_flag;			/**< VLAN scan実行モード   */
	UCHAR				cmx_sshport_flag;			/**< ssh-port有効フラグ   */
	USHORT				cmx_callhome_ssh_port;		/**< call-home-ssh-port   */
	USHORT				cmx_server_ssh_port;		/**< server-ssh-port   */
	USHORT				cmx_callhome_retry_timer;	/**< callhome retry timer   */
	UCHAR				cmx_dummy17[64];
	UCHAR				cmx_domain_number;			/**< domain-number   */
	UCHAR				cmx_multicast_mac_address;	/**< multi-mac-address   */
	UCHAR				cmdx_accepted_clock_classes;/**< accept-clock-classes   */
	UCHAR				cmx_dummy18[13];
	/**** 装置_ハードパラメータ : 0x0380-0x03bf	*****/
	UCHAR				cmx_temp_alm_val;			/**< 温度ALM閾値   */
	UCHAR				cmx_paon;					/**< PA ON/OFF制御Status   */
	UCHAR				cmx_txgain;					/**< Gain微調設定値(Ant共通)   */
	UCHAR				cmx_rxcalcmp_flag;			/**< RxCalトレーニング完了フラグ   */
	UCHAR				cmx_dummy19;
	UCHAR				cmx_bf_envelope_mode;		/**< mmW BeamID 新旧モード設定   */
	UCHAR				cmx_dummy20;
	UCHAR				cmx_fan_spin_mode;			/**< FAN回転数モード設定   */
	UCHAR				cmx_dummy20_1[9];
	UCHAR				cmx_mmWtrainingRfSno[8];	/**< mmWトレーニング完了 RFボードインベントリ情報   */
	UCHAR				cmx_dummy21[13];
	UCHAR				cmx_t3_adjust_val[2];		/**< T3補正値X   */
	UCHAR				cmx_t2_adjust_val[2];		/**< T2補正値Y   */
	UCHAR				cmx_sfp_failsafe_ab_cnt;	/**< フェールセーフABカウンタ */
	UCHAR				cmx_sfp_failsafe_abcd_cnt;	/**< フェールセーフABCDカウンタ */
	UCHAR				cmx_dummy21_1[4];
	UCHAR				cmx_dhcp_serial[8];			/**< radioId   */
	UCHAR				cmx_dummy21_1_2[8];	
	/**** debug : 0x03c0-0x03ff	*****/
	UCHAR				cmx_dummy21_2;
	UCHAR				cmx_dhcp_on_off_flg;		/**< DHCP有無フラグ   */
	UCHAR				cmx_ptp_device_access_flg;	/**< PTPdeviceアクセス抑止フラグ   */
	UCHAR				cmx_ptp_master_mode;		/**< PTP masterモード   */
	UCHAR				cmx_dummy21_3;
	UCHAR				cmx_dummy21_4;
	UCHAR				cmx_dummy21_5[4];
	UCHAR				cmx_dummy21_6;
	UCHAR				cmx_synce_esmc_send_flg;	/**< SyncE ESMC送信フラグ   */
	UCHAR				cmx_dummy22[2];
	UCHAR				cmx_fwrite_ssh_user_flag;	/**< SSHユーザーアカウント情報FLASH書込みフラグ   */
	UCHAR				cmx_fwrite_vlan_info_flag;	/**< VLAN情報FLASH書込みフラグ   */
	UCHAR				softreset_mask[1];			/**< ソフトリセットマスク  */
	UCHAR				softreset_flag[1];			/**< ソフトリセットフラグ  */
	UCHAR				almreset_flag[1];			/**< ALM自律リセット有無フラグ  */
	UCHAR				troublelog_File[1];			/**< 障害ログ退避面  */
	UCHAR				cyclicklog_File[1];			/**< サイクリックログ退避面  */
	UCHAR				cyclicklog0_erase[1];		/**< サイクリックログイレースフラグ0面用  */
	UCHAR				cyclicklog1_erase[1];		/**< サイクリックログイレースフラグ1面用  */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UCHAR				dummy5[1];							/**< 予備															*/
	UCHAR				gain_setting[4];					/**< ゲイン微調設定値												*/
	UCHAR				dummy6[5];							/**< 予備															*/
	UCHAR				real_trlog_ipfilter[4];				/**< リアル通信ログIPフィルタ										*/
	UCHAR				real_trlog_output[1];				/**< リアル通信ログ出力先											*/
	UCHAR				softboot_mode[1];					/**< ソフト起動モード												*/
	UCHAR				dummy7[5];							/**< 予備															*/
	UCHAR				cpri_ctle1;							/**< CPRI CTLE1														*/
	UCHAR				cpri_ctle2;							/**< CPRI CTLE2														*/
	UCHAR				cpri_ctle3;							/**< CPRI CTLE3														*/
	UCHAR				cpri_dcd;							/**< CPRI DCD														*/
	UCHAR				pa_onoff_status[1];					/**< PA ON/OFF状態													*/
	UCHAR				cpri_rate_static;					/**< 9.8G強制切り替えモード											*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UCHAR				timesave_flg;				/**< 時刻退避フラグ  */
	UCHAR				timesaveinfo[8];			/**< 時刻退避情報  */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UCHAR				dummy8[213];						/**< 予備															*/
	UCHAR				tp_debug[2];						/**< TPデバック														*/
	UCHAR				dummy9[253];						/**< 予備															*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UCHAR				cmx_pc_conn_flg;			/**< PC接続フラグ   */
	UCHAR				cmx_mp_conn_flg;			/**< M-plane接続抑止フラグ   */
	USHORT				cmx_ipup_dbg;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn_dbg;				/**< IP address (low word)   */
	UCHAR				cmx_pgclog;					/**< pgc log */
	UCHAR				cmx_bfmode;					/**< ビームフォーミングモード */
	UCHAR				cmx_dpmode_mp;				/**< M-planeメッセージ出力モード   */
	UCHAR				cmx_dpmode_sp;				/**< S-planeメッセージ出力モード   */
	UCHAR				cmx_dpmode_dbg;				/**< デバッグメッセージ出力モード   */
	UCHAR				cmx_dpmode_aisg;			/**< AISGメッセージ出力モード   */
	UCHAR				cmx_dummy24[5];	
	UCHAR				cmx_sfn_offset_a[3];		/**< SFN_Offset_α(downlink-radio-frame-offset)   */
	UCHAR				cmx_sfn_offset_b[2];		/**< SFN_Offset_β(downlink-sfn-offset)   */
	UCHAR				cmx_dummy25[10];
	
}T_RRH_TRAINVTBL;
 
/** @} */

/****************************************************************************************************************************/
/** Time out notification
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
typedef struct{
	unsigned int		id;		/*!< mw time id */
	timer_t				id_os;	/*!< os time id */
	unsigned int		seqno;	/*!< counter for periodic */
	unsigned int		userkey;
	struct timespec		tstat_count;
	struct timespec		tout_count;
}T_RRH_TIMERTO_NTC_DATA;

typedef struct{
	T_RRH_HEAD					header;
	T_RRH_TIMERTO_NTC_DATA		data;
}T_RRH_TIMERTO_NTC;
/** @} */

/*!
 * @name メッセージ送受信構造体
 * @note メッセージ送受信構造体定義
 * @{
 */

typedef struct {
	T_RRH_HEAD		header;
	UINT				data;
}T_RRH_MESSAGE_TYPE;
/* @} */

/**
 * @brief	RE device management table
 * @note	RE device management table
 *
  */
typedef struct{
	USHORT				mkr_nm;								/**< Maker name   */
	USHORT				bts_type;							/**< BTS type   */
	USHORT				re_type;							/**< RE type   */
	USHORT				dev_kind;							/**< device kind (TBD)   */
	USHORT				dev_no;								/**< device number (TBD)   */
	USHORT				div_type;							/**< Diversity type (3G use)   */
	USHORT				pwr_type;							/**< power type   */
	USHORT				pwr_class_unit;						/**< power class unit   */
	USHORT				pwr_class0;							/**< power class (Br0)   */
	USHORT				pwr_class1;							/**< power class (Br1)   */
	USHORT				pwr_class2;							/**< power class (Br2)   */
	USHORT				pwr_class3;							/**< power class (Br3)   */
	USHORT				crp_frq;							/**< corresponding frequency area   */
	USHORT				max_bandwidth;						/**< Max bandwidth   */
	USHORT				max_tx_ant_no;						/**< Max TX antenna number   */
	USHORT				fii_type;							/**< 800MHz filter type   */
	USHORT				cmx_2gcarcnt;						/**< 2GHz frequency carrier number   */
	USHORT				cmx_800mcarcnt;						/**< 800MHz frequency carrier number   */
	USHORT				cmx_17gcarcnt;						/**< 1.7GHz frequency carrier number   */
	USHORT				cmx_trxinf_typ;						/**< RE side TRX-INF type   */
	USHORT				cmx_15gcarcnt;						/**< 1.5GHz frequency carrier number   */
	USHORT				cmx_35gcarcnt;						/**< 3.5GHz frequency carrier number   */
	USHORT				mca_filt;
}T_RRH_REDEVCOMPTBL;

/* ======================================================================== */
/* DU network information Table												*/
/* ======================================================================== */
/**
 * @brief	DU network information from eeprom
 * @note	DU network information from eeprom
  */
typedef struct{
	USHORT						lowVid;						/**< 検索VID最小値  */
	USHORT						highVid;					/**< 検索VID最大値  */
	USHORT						lastUsedVid;				/**< VID前回値  */
	USHORT						nowVid;						/**< VID検索現在値  */
}T_RRH_VID_SEARCH_INFO;

typedef struct{
															/**< MAC Address (Port#0/#1)  */
	UCHAR					localMacAddress[D_RRH_IF_MAX][D_RRH_MAC_ADDR_SIZE];
	T_RRH_VID_SEARCH_INFO	vid_search_info;				/**< VID検索情報  */
	USHORT					pcp;							/**< M-Plane pcp値  */
	UCHAR					dummy[4];
}T_RRH_DU_INFORMATION_EEPROM;

/**
 * @brief	DU network information from flash
 * @note	DU network information from flash
  */
typedef struct{
	UCHAR					seachable_vlan_id[D_DU_SEARCHABLE_VLAN_MAX];	/**< 検索対象vlan-id(bitmap)  */
}T_RRH_DU_INFORMATION_FLASH;

/**
 * @brief	DU connection information
 * @note	DU connection information
  */
typedef struct{
	USHORT			vid;									/**< VID  */
	USHORT			pcp;									/**< PCP  */
} T_RRH_MAC_LAYER_INFO;

typedef struct{
	UINT						ip_addr_cu;					/**< IP Address (CU)  */
	UINT						ip_addr_dhcps;				/**< IP Address (DHCP Server)  */
	UINT						ip_addr_du;					/**< IP Address (DU)  */
	UINT						subnet_mask;				/**< Subnet Mask  */
	UINT						default_gateway;			/**< Default Gateway  */
	CHAR						vendor_specific_option[20];	/**< vendor-specific-option  */
} T_RRH_IP_LAYER_INFO ;

typedef struct{
	UINT						port_no;					/**< 物理Port番号  **/
	UCHAR						dhcpFlag;					/**< DHCP状態フラグ  **/
	UCHAR						dummy[3];
	T_RRH_MAC_LAYER_INFO		mac;						/**< MAC Layer情報		*/
	T_RRH_IP_LAYER_INFO			ip;							/**< IP Layer情報		*/
}T_RRH_CONNECTION_INFO_MP;

typedef struct{
	UINT						port_no;					/**< 物理Port番号  **/
	UCHAR						dummy[4];
	T_RRH_MAC_LAYER_INFO		umac;						/**< MAC Layer情報		*/
	T_RRH_MAC_LAYER_INFO		cmac;						/**< MAC Layer情報		*/
}T_RRH_CONNECTION_INFO_UCP;

typedef struct{
	T_RRH_CONNECTION_INFO_MP	mp;							/**< M-Planeコネクション情報  */
	T_RRH_CONNECTION_INFO_UCP	ucp[2];						/**< U/FC-Planeコネクション情報  */
}T_RRH_DU_CONNECTION_INFO;

/**
 * @brief	DU Network information management table
 * @note	Contents: DU netwaork information management \n
 * 			Initial	: pf_main \n
 * 			Read	: pf_fhs/pf_dhc \n
 * 			Write	: pf_fhs/pf_dhc \n
 */
typedef struct{
	T_RRH_DU_INFORMATION_EEPROM		eeprom_info;			/**< EEPROM情報  */
	T_RRH_DU_INFORMATION_FLASH		flash_info;				/**< FLASH情報  */
	T_RRH_DU_CONNECTION_INFO		du_connection_info;		/**< DUコネクション情報  */
}T_RRH_DU_INFORMATION_TBL;

/* ======================================================================== */
/* NETCONF User information Table											*/
/* ======================================================================== */
typedef struct{
	char						user[D_DU_NETCONF_CHAR_OF_USER];		/**< netconf-user-name  */
	char						password[D_DU_NETCONF_CHAR_OF_PASS];	/**< netconf-password  */
	UINT						enabled;					/**< netconf-enabled  */
	UCHAR						dummy[4];
}T_RRH_NETCONF_USER_INF;

/**
 * @brief	NETCONF User information Table (shared memory)
 * @note	Contents: NETCONF User information \n
 * 			Initial	: pf_main \n
 * 			Read	: pf_fhs/pf_nma \n
 * 			Write	: pf_fhs/pf_nma \n
 */
typedef struct{
	USHORT						callhome_ssh_port;			/**< call-home-ssh-port  */
	USHORT						server_ssh_port;			/**< server-ssh-port  */
	UINT						face_cnt;					/**< USER情報有効面数  */
	UINT						num_of_use;					/**< 有効ユーザー数  */
	UCHAR						dummy[12];
	T_RRH_NETCONF_USER_INF		info[D_DU_NETCONF_NUM_OF_USER];
}T_RRH_NETCONF_INFO_TBL;

/**
 * @brief	NETCONF User information Table (FLASH memory)
 * @note	Contents: NETCONF User information
 */
typedef struct{
	UINT						num_of_use;					/**< 有効ユーザー数  */
	UCHAR						dummy[12];
	T_RRH_NETCONF_USER_INF		info[D_DU_NETCONF_NUM_OF_USER_FLASH];
}T_RRH_NETCONF_INFO_FLASH_TBL;


/**
 * @brief	System parameter management table
 * @note	Contents: System parameter management table \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 */
typedef struct{
	USHORT				cpri_sttimer;						/**< CPRI state stability timer  */
	USHORT				hls_timer;							/**< health check request timer  */
	USHORT				hls_unconfcnt;						/**< health check request unconfirmation counter  */
	USHORT				sys_timer;							/**< system parameter sending confirmation timer  */
	USHORT				firm_timer;							/**< firmware sending confirmation timer  */
	USHORT				sec_chip0;							/**< sector chip revision (#0)  */
	USHORT				sec_chip1;							/**< sector chip revision (#1)  */
	USHORT				oudevt_rcvtimer;					/**< external equipment receiving confirmation timer  */
	USHORT				fldmt_cttimer;						/**< FLD-MT connection confirmation timer  */
	USHORT				fldmt_rltimer;						/**< FLD-MT release confirmation timer  */
	USHORT				fldmt_dtcd_timer;					/**< FLD-MT data command receiving confirmation timer  */
	USHORT				fldmt_dtfl_timer;					/**< FLD-MT data file receiving confirmation timer  */
	USHORT				svmt_cttimer;						/**< SV-MT connection confirmation timer  */
	USHORT				svmt_rltimer;						/**< SV-MT release confirmation timer  */
	USHORT				svmt_rcvtimer;						/**< SV-MT data receiving confirmation timer  */
	USHORT				tx_div_value_3G;					/**< tx diversity setting value for 3G */
	USHORT				pri_sys_info;						/**< RE primary system information  */
	USHORT				s3g_bandwidth;						/**< S3G bandwidth  */
	USHORT				car_wk_flg;							/**< 3G carrier#1-4 working flag 0x00001111  */
	USHORT				tx_sys_num;							/**< tx system number  */
	USHORT				reserveE[4];						/**< reserve for system parameter E  */
	USHORT				car1_wk_frq;						/**< 3G carrier#1 working frequency  */
	USHORT				car2_wk_frq;						/**< 3G carrier#2 working frequency  */
	USHORT				car3_wk_frq;						/**< 3G carrier#3 working frequency  */
	USHORT				car4_wk_frq;						/**< 3G carrier#4 working frequency  */
	USHORT				mtExt_data_3g_flg;					/**< 3G MT signaling/External equipment data corresponse flag (TBD)  */
	USHORT				syspara_wait_timer;					/**< system parameter wait timer */
	USHORT				firm_sndtimer;						/**< firm send timer */
	USHORT				reserveF;							/**< reserve for system parameter F  */
}T_RRH_SYSMNGTBL;

/**
 * @brief	shelf inventory information management table
 * @note	Contents:	shelf inventory information management  \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 *
  */
typedef struct{
	USHORT				cmx_cardname[6];		/**< Card name   */
	USHORT				cmx_pcbno1;				/**< Product number (Year/Month/Day)   */
	USHORT				cmx_pcbno2;				/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;				/**< Product number serial (low word)   */
	USHORT				cmx_pcbbyer;			/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_pcbbday;			/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver;				/**< Hardware version number   */
	USHORT				cmx_mno[6];				/**< Card map number   */
}T_RRH_SHELFINVTBL;

/****************************************************************************************************************************/
/** T_RRH_CASCADEINFOTBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_CASCADEINFOTBL
 * @note	T_RRH_CASCADEINFOTBL is ..
 */
typedef struct{
	USHORT				cscd_kind;
	USHORT				timer_state;
	USHORT				filever_disagreement;
}T_RRH_CASCADEINFOTBL;
/* @}  */

/*************************************************************************** */
/* RTC time	*/
/*************************************************************************** */
typedef struct
{
	USHORT			year  ;					/**<year			*/
	UCHAR			month ;					/**<month			*/
	UCHAR			day   ;					/**<day				*/
	UCHAR			hour  ;					/**<hour			*/
	UCHAR			min   ;					/**<min				*/
	UCHAR			sec   ;					/**<sec				*/
	UCHAR			msec  ;					/**<msec			*/

} T_RRH_DATETIME ;
/** @} */

/****************************************************************************************************************************/
/**
 * @name ALM-A ALM-B ALM-C
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_ALM_ABC
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              alm_a;                      /**< ALM-A */
    UCHAR               alm_b[D_RRH_CPRINO_RE_MAX]; /**< ALM-B */
    UCHAR               alm_c;                      /**< ALM-C */
    UCHAR               reserved;                   /**< reserved */
} T_RRH_ALM_ABC;
/** @} */

/****************************************************************************************************************************/
/**
 * @name CPRI state
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_CPRI_STATE
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              cprista_rec;                                            /**< CPRI state(REC) */
    USHORT              cprista_re[D_RRH_CPRINO_RE_MAX];                        /**< CPRI state(RE) */
    USHORT              cpri_bitrate;                                           /**< CPRI bit rate */
    USHORT              cpri_linktype;                                          /**< CPRIリンク種別 */
    USHORT              hdlcst_rec;                                             /**< HDLC ACT/IDLE(REC) */
    USHORT              hdlcst_re[D_RRH_CPRINO_RE_MAX];                         /**< HDLC ACT/IDLE(RE) */
    USHORT              healthchk_rec[CMD_SYS_BOTH_3G_S3G];                      /**< ヘルスチェック状態(REC) */
    USHORT              healthchk_re[CMD_SYS_BOTH_3G_S3G][D_RRH_CPRINO_RE_MAX];  /**< ヘルスチェック状態(RE) */
} T_RRH_CPRI_STATE;
/** @} */

/****************************************************************************************************************************/
/**
 * @name FHM state
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_FHM_STATE
 * @note
 * @date    2015/12/04 TDI)satou IT3問処番号No.108
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    USHORT              FhmBase;                        /**< FHM基本監視項目 */
    USHORT              alm_num;                        /**< ALM情報数 */
    USHORT              alm_inf[255];                   /**< ALM情報 */
    USHORT              err_num;                        /**< ERR情報数 */
    USHORT              err_inf[255];                   /**< ERR情報 */
    USHORT              fan_num;                        /**< FAN情報数n */
    USHORT              fan_inf[16];                    /**< FAN情報 */
    USHORT              maker_inf_num;                  /**< メーカ固有情報数n */
    USHORT              maker_inf[32];                  /**< メーカ固有情報 */
    struct {
        USHORT          FhmBase;
        USHORT          alm_num;
        USHORT          alm_inf[255];
        USHORT          err_num;
        USHORT          err_inf[255];
    } sys[CMD_MAX_SYSNUM];
} T_RRH_FHM_STATE;
/** @} */

/****************************************************************************************************************************/
/**
 * @name layer3
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_LAYER3
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              layer3_rec;                     /**< Layer3(REC) */
    USHORT              layer3_re[D_RRH_CPRINO_RE_MAX]; /**< Layer3(RE) */
} T_RRH_LAYER3;
/** @} */

/****************************************************************************************************************************/
/**
 * @name MT address setting
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_MTADDSET MTアドレス設定
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              tgtmt;                              /**< 対象MT */
    UCHAR               ipadd_1;                            /**< IPアドレス#1 */
    UCHAR               ipadd_2;                            /**< IPアドレス#2 */
    UCHAR               ipadd_3;                            /**< IPアドレス#3 */
    UCHAR               ipadd_4;                            /**< IPアドレス#4 */
    UCHAR               sbnetmsk_1;                         /**< サブネットマスク#1 */
    UCHAR               sbnetmsk_2;                         /**< サブネットマスク#2 */
    UCHAR               sbnetmsk_3;                         /**< サブネットマスク#3 */
    UCHAR               sbnetmsk_4;                         /**< サブネットマスク#4 */
    USHORT              cmdtrsport;                         /**< コマンド転送用ポート番号 */
    USHORT              filtrsport;                         /**< ファイル転送用ポート番号 */
    USHORT              portno;                             /**< ポート番号 */
} T_RRH_MTADDSET;
/** @} */
	
/****************************************************************************************************************************/
/**
 * @name REカード状態
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_CARDST_S3G_BR_EVERY (LTE)ブランチ単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              br_no;                              /**< ブランチ番号 */
    USHORT              func_svitem;                        /**< 機能部基本監視項目 */
    USHORT              func_extsvitem;                     /**< 機能部拡張監視項目 */
} T_RRH_CARDST_S3G_BR_EVERY;

/**
 * @brief   T_RRH_CARDST_S3G_CAR_EVERY (LTE)キャリア単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT                      car_no;                             /**< キャリア番号 */
    USHORT                      band_width;                         /**< システム帯域幅 */
    USHORT                      func_svitem;                        /**< 機能部基本監視項目 */
    USHORT                      func_extsvitem;                     /**< 機能部拡張監視項目 */
    USHORT                      br_num;                             /**< 対象ブランチ数m */
    USHORT                      reserved;                           /**< 予約 */
    T_RRH_CARDST_S3G_BR_EVERY   br_every[8];                        /**< ブランチ単位情報 */
} T_RRH_CARDST_S3G_CAR_EVERY;

/**
 * @brief   T_RRH_CARDST_S3G_FUNC_EVERY (LTE)機能部単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT                      func_kind;                      /**< 機能部種別 */
    USHORT                      func_failure;                   /**< 機能部故障状態 */
    USHORT                      func_svitem;                    /**< 機能部基本監視項目 */
    USHORT                      func_extsvitem;                 /**< 機能部拡張監視項目 */
    USHORT                      car_num;                        /**< 対象キャリア数m */
    USHORT                      reserved;                       /**< 予約 */
    T_RRH_CARDST_S3G_CAR_EVERY  car_every[5];                   /**< キャリア単位情報 */
}T_RRH_CARDST_S3G_FUNC_EVERY;

/**
 * @brief   T_RRH_CARDST_S3G (LTE)REカード状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT                      signal_kind;                        /**< 信号種別 */
    USHORT                      result;                             /**< 応答結果 */
    USHORT                      slot_no;                            /**< スロット番号 */
    UCHAR                       card_name[8];                       /**< カード名 */
    USHORT                      year;                               /**< 年 */
    USHORT                      month_day;                          /**< 月日 */
    USHORT                      hour;                               /**< 時 */
    USHORT                      minsec;                             /**< 分秒 */
    USHORT                      card_svitem;                        /**< カード基本監視項目 */
    USHORT                      card_extsvitem;                     /**< カード拡張監視項目 */
    USHORT                      card_cntitem;                       /**< カード基本制御項目 */
    USHORT                      card_extcntitem;                    /**< カード拡張制御項目 */
    USHORT                      func_num;                           /**< 機能部数 */
    T_RRH_CARDST_S3G_FUNC_EVERY func_every[29];                     /**< 機能部単位情報 */
    USHORT                      alm_num;                            /**< ALM情報数 */
    USHORT                      alm_inf[5107];                       /**< ALM情報 */
    USHORT                      err_num;                            /**< ERR情報数 */
    USHORT                      err_inf[5107];                       /**< ERR情報 */
} T_RRH_CARDST_S3G;

/**
 * @brief   T_RRH_CARDST_3G  (3G)REカード状態
 * @note
 * @date    2015/10/22 FPT)Tan add カード制御項目, fix bug BUG_122_IT2_20-7-1
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              result;                             /**< 応答結果 */
    USHORT              slot_no;                            /**< スロット番号 */
    USHORT              year;                               /**< 年 */
    USHORT              month_day;                          /**< 月日 */
    USHORT              hour;                               /**< 時 */
    USHORT              minsec;                             /**< 分秒 */
    USHORT              card_svitem;                        /**< カード基本監視項目 */
    USHORT              card_extsvitem;                     /**< カード拡張監視項目 */
	USHORT				card_cntitem;						/**< カード制御項目	*/  /* FPT)Tan Add */
    USHORT              cnt_code;                           /**< 制御コード */
    USHORT              func_item;                          /**< 機能部固有制御項目 */
    USHORT              car_inf;                            /**< キャリア情報 */
    USHORT              alm_num;                            /**< ALM情報数 */
    USHORT              alm_inf[255];                       /**< ALM情報 */
    USHORT              err_num;                            /**< ERR情報数 */
    USHORT              err_inf[255];                       /**< ERR情報 */
} T_RRH_CARDST_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name REスロット情報
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_SLOT_S3G  (LTE)スロット情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              result;                             /**< 応答結果 */
    USHORT              slot_num;                           /**< スロット番号 */
    USHORT              card_inf;                           /**< REカード機能部情報 */
} T_RRH_SLOT_S3G;

/**
 * @brief   T_RRH_SLOT_3G  (3G)スロット情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              result;                             /**< 応答結果 */
    USHORT              slot_num;                           /**< スロット番号 */
    USHORT              card_inf;                           /**< REカード機能部情報 */
} T_RRH_SLOT_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name REスロット情報(複数Slot対応)
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_SLOT_S3G  (LTE)スロット情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              result;                             /**< 応答結果 */
    USHORT              slot_num;                           /**< スロット数 */
    USHORT              slot_no[16];                        /**< スロット番号 */
    USHORT              card_inf[16];                       /**< REカード機能部情報 */
    USHORT              slt_idx[56];                        /**< スロットIndex値 */
} T_RRH_SLOT_S3G_RE;

/**
 * @brief   T_RRH_SLOT_3G  (3G)スロット情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                        /**< 信号種別 */
    USHORT              result;                             /**< 応答結果 */
    USHORT              slot_num;                           /**< スロット数 */
    USHORT              slot_no[16];                        /**< スロット番号 */
    USHORT              card_inf[16];                       /**< REカード機能部情報 */
    USHORT              slt_idx[56];                        /**< スロットIndex値 */
} T_RRH_SLOT_3G_RE;
/** @} */

/****************************************************************************************************************************/
/**
 * @name システムパラメータ格納領域
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_SYSPAR_LTE シスパラ格納領域(LTE)
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              syspar_lte[32];                     /**< System parameter for LTE */
} T_RRH_SYSPAR_LTE;

/**
 * @brief   T_RRH_SYSPAR_3G シスパラ格納領域(3G)
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              syspar_3g[16];                      /**< System Parameter for 3G */
} T_RRH_SYSPAR_3G;
/** @} */
 	
/****************************************************************************************************************************/
/**
 * @name FHM保持FWファイル
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_SIDE_USE  FHM保持FWファイル運用面
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              use_side;                       /**< Use Side */
} T_RRH_SIDE_USE;
/** @} */

/**
 * @brief   T_RRH_FW_INFO  FHM保持FWファイル情報(0.1面)
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              maker_id;                       /**< Maker Identifier */
    USHORT              major_ver;                      /**< Major Version */
    USHORT              minor_ver;                      /**< Minor Version */
    USHORT              dummy;                          /**< Dummy */
    ULONG               file_size;                      /**< File SIze */
    UCHAR               file_name[20];                  /**< File Name*/
    USHORT              year;                           /**< Year */
    UCHAR               month;                          /**< Month */
    UCHAR               day;                            /**< Day */
} T_RRH_FW_INFO;

/**
 * @brief   T_RRH_FW_DATA FWファイルデータ格納領域
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    ULONG               pgm_size;                       /**< Program Size */
    USHORT              chksum;                         /**< Checksum */
    USHORT              maker_id;                       /**< Maker Identifier */
    UCHAR               maker_name[16];                 /**< Maker Name */
    UCHAR               ftype_name[16];                 /**< File-Type Name */
    USHORT              ftype;                          /**< File-Type */
    USHORT              major_ver;                      /**< Major Version */
    USHORT              minor_ver;                      /**< Minor Version */
    USHORT              revision;                       /**< Revision */
    USHORT              year;                           /**< Year */
    UCHAR               month;                          /**< Month */
    UCHAR               day;                            /**< Day */
    USHORT              maker_id2;                      /**< Second Maker Identifier */
    UCHAR               reserve[458];                   /**< Reserve */
    UCHAR               mk_reserve[512];                /**< Maker Reserved */
    UCHAR               pgm_data[116776192];            /**< Program Data (16MB-1KB) */
} T_RRH_FW_DATA;
/** @} */

/****************************************************************************************************************************/
/**
 * @name RE状態
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_REST_S3G (LTE)RE状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              maker_name;                     /**< メーカ名 */
    USHORT              fan_num;                        /**< FAN情報数n */
    USHORT              fan_inf[16];                    /**< ファン情報 */
    USHORT              maker_inf_num;                  /**< メーカ固有RE情報数n */
    USHORT              maker_inf[32];                  /**< メーカ固有RE情報 */
} T_RRH_REST_S3G;

/**
 * @brief   T_RRH_REST_3G (3G)RE状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              maker_name;                     /**< メーカ名 */
    USHORT              fan_num;                        /**< FAN情報数n */
    USHORT              fan_inf[16];                    /**< ファン情報 */
    USHORT              maker_inf_num;                  /**< メーカ固有RE情報数n */
    USHORT              maker_inf[32];                  /**< メーカ固有RE情報 */
} T_RRH_REST_3G;
/** @} */
 	
/****************************************************************************************************************************/
/**
 * @name Toffset
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_TOFFSET_S3G (LTE)Toffset値報告
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              toffset;                        /**< Toffset値 */
    USHORT              updeleqp;                       /**< 上り装置内遅延 */
    USHORT              downdeleqp;                     /**< 下り装置内遅延 */
} T_RRH_TOFFSET_S3G;

/**
 * @brief   T_RRH_TOFFSET_3G (3G)Toffset値報告
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              toffset;                        /**< Toffset値 */
    USHORT              updeleqp;                       /**< 上り装置内遅延n */
    USHORT              downdeleqp;                     /**< 下り装置内遅延 */
} T_RRH_TOFFSET_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name TRX設定パラメータ
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_TRXSET_PARA_LTE (LTE)TRX設定パラメータテーブル
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              wSigKind;                       /**< 信号種別 */
    USHORT              wCarrNo;                        /**< キャリア番号 */
    USHORT              wBandWidth;                     /**< システム帯域幅 */
    USHORT              wDlFreqNo;                      /**< 下り周波数番号 */
    USHORT              wSend;                          /**< 送信系 */
    USHORT              wTddUlDlConfig;                 /**< TDD uplink/downlink configuration */
    USHORT              wTddSpeSubConfig;               /**< TDD special subframe configuration */
    USHORT              wTddDlCycPreLen;                /**< TDD downlink cyclic prefix length */
    USHORT              wStatus;                        /**< TRX設定/解放状態 */
    USHORT				wdatSize;						/**< TRX設定要求データサイズ */
} T_TRXSET_PARA_LTE;

/**
 * @brief   T_TRXSET_PARA_3G (3G)TRX設定パラメータテーブル
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              wSigKind;                       /**< 信号種別 */
    USHORT              wCarrNo;                        /**< キャリア番号 */
    USHORT              wFreqNo;                        /**< 周波数番号 */
    USHORT              wStatus;                        /**< TRX設定/解放状態 */
} T_TRXSET_PARA_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name REキャリア状態
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_CARST_S3G_BR_EVERY (LTE)ブランチ単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              br_num;                             /**< ブランチ番号 */
    USHORT              car_st;                             /**< REキャリア状態 */
    USHORT              car_st_ul;                          /**< REキャリア状態(UL) */
    USHORT              car_st_dl;                          /**< REキャリア状態(DL) */
}T_RRH_CARST_S3G_BR_EVERY;

/**
 * @brief   T_RRH_CARST_S3G_CAR_EVERY (LTE)キャリア単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT                      car_num;                    /**< キャリア番号 */
    USHORT                      car_st;                     /**< REキャリア情報 */
    USHORT                      car_st_ul;                  /**< REキャリア状態(UL) */
    USHORT                      car_st_dl;                  /**< REキャリア状態(DL) */
    USHORT                      br_num;                     /**< ブランチ数n */
    USHORT                      reserved;                   /**< reserved */
    T_RRH_CARST_S3G_BR_EVERY    br_every[8];                /**< ブランチ単位情報 */
    USHORT                      alm_num;                    /**< ALM情報数 */
    USHORT                      alm_inf[255];               /**< ALM情報 */
    USHORT                      card_num;                   /**< カード数 */
    USHORT                      slot_num[6];                /**< スロット番号 */
}T_RRH_CARST_S3G_CAR_EVERY;

/**
 * @brief   T_RRH_CARST_S3G (LTE)キャリア状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT                      signal_kind;                /**< 信号種別 */
    USHORT                      result;                     /**< 応答結果 */
    USHORT                      car_num;                    /**< キャリア数 */
    USHORT                      reserved;                   /**< reserved */
    T_RRH_CARST_S3G_CAR_EVERY   car_every[5];               /**< キャリア単位情報 */
} T_RRH_CARST_S3G;

/**
 * @brief   T_RRH_CARST_3G_CAR_EVERY (3G)キャリア単位情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */ 	
typedef struct {
    USHORT                      carrier_num;                /**< キャリア番号 */
    USHORT                      carrier_st;                 /**< キャリア状態 */
    USHORT                      carrier_eqp;                /**< キャリア構成状態 */
    USHORT                      alm_num;                    /**< ALM情報数 */
    USHORT                      alm_inf[255];               /**< ALM情報 */
    USHORT                      err_num;                    /**< ERR情報数 */
    USHORT                      err_inf[255];               /**< ERR情報 */
    USHORT                      card_num;                   /**< カード数 */
    USHORT                      slot_num[6];                /**< スロット番号 */
}T_RRH_CARST_3G_CAR_EVERY;

/**
 * @brief   T_RRH_CARST_3G (3G)キャリア状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */ 	
typedef struct {
    USHORT                      signal_kind;                /**< 信号種別 */
    USHORT                      result;                     /**< 応答結果 */
    USHORT                      car_num;                    /**< キャリア数 */
    USHORT                      reserved;                   /**< reserved */
    T_RRH_CARST_3G_CAR_EVERY    car_every[4];               /**< キャリア単位情報 */
} T_RRH_CARST_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name RE装置構成情報
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_EQP_S3G (LTE)装置構成情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              eqp_inf[64];                    /**< CPRIリンク装置構成情報 */
} T_RRH_EQP_S3G;

/**
 * @brief   T_RRH_EQP_3G (3G)装置構成情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              signal_kind;                    /**< 信号種別 */
    USHORT              result;                         /**< 応答結果 */
    USHORT              eqp_inf[64];                    /**< CPRIリンク装置構成情報 */
} T_RRH_EQP_3G;
/** @} */

/****************************************************************************************************************************/
/**
 * @name 無線送信ON/OFF
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_TX_ONOFF 無線送信ONOFF状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              onoff_fhm;                          /**< ONOFF(FHM) */
    USHORT              onoff_re[D_RRH_CPRINO_RE_MAX];      /**< ONOFF(RE) */
} T_RRH_TX_ONOFF;
/** @} */

/****************************************************************************************************************************/
/**
 * @name 配下RE使用/未使用
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_CONN_DISC RE接続有無状態
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              conn_disc[D_RRH_CPRINO_RE_MAX];     /**< RE接続有無状態 */
} T_RRH_CONN_DISC;
/** @} */
/****************************************************************************************************************************/
/**
 * @name SFP履歴ログ
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   SFP履歴ログヘッダ
 * @date    2015/11/16 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    UINT idxNextTime_slave;                         /**< 次回書き込み位置(Slave)  */
    UINT dummy1[3];                                 /**< ダミー                   */
    UINT idxNextTime_master[D_RRH_CPRINO_RE_MAX];   /**< 次回書き込み位置(Master) */
    UINT dummy2[12];                                /**< ダミー                   */
} T_RRH_SFP_LOG_HEADER;
/**
 * @brief   SFP履歴ログ内容
 * @date    2015/11/16 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_RRH_SYSTEM_TIME time;     /**< 時刻             */
    USHORT trigger;             /**< 取得契機         */
    USHORT enable_disable;      /**< 有効/無効        */
    UINT sfpSt;                 /**< SFP状態          */
    UINT sfpSupport;            /**< SFP対応          */
    UINT sfpSpeed;              /**< SFP速度          */
    UINT sfpSlaveSpeed;         /**< Slave SFP速度    */
    UCHAR sfpInf[2][128];       /**< SFP情報A0#000～127, A2#000～127 */
} T_RRH_SFP_LOG;
/**
 * @brief   SFP履歴ログ全体情報
 * @note    各CPRIリンク(Slave/Master#1～16)毎に最大16個のSFP情報を残す.
 *          16個は、サイクリックに使用し、最新の16履歴を有効とする.
 * @note    以下のタイミングで取得する.
 *          カード起動、運用中共に、変化がある場合のみLOGに残す.
 *          カード起動時は、CPRIリンク未使用でもSFPが実装であれば、情報を取得する.
 * @date    2015/11/16 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_RRH_SFP_LOG_HEADER header;                    /**< ヘッダ             */
    struct {
        T_RRH_SFP_LOG log;                   /**< 最新SFP情報        */
    } cpr_latest[D_RRH_CPRINO_NUM];
    struct {
        T_RRH_SFP_LOG log[D_RRH_SFPLOG_TOTAL_NUM];  /**< SFP情報            */
    } cpr[D_RRH_CPRINO_NUM];
} T_RRH_SFP_LOG_ALL;
/** @} */
/****************************************************************************************************************************/
/**
 * @name 遅延補正情報
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief   T_RRH_RE_DELAY_INF RE単位遅延補正情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              UseKind;                            /**< 有効無効情報 */
    USHORT              RoundTrip;                          /**< RoundTripDelay(CP_CPRTDLY) */
    USHORT              RtDelay;                            /**< RTDelay_offset */
    USHORT              Toffset;                            /**< Toffset */
    USHORT              T12;                                /**< T12 */
    USHORT              DlEquipDelay[CMD_SYS_BOTH_3G_S3G];   /**< 下り装置内遅延 */
    USHORT              UlEquipDelay[CMD_SYS_BOTH_3G_S3G];   /**< 上り装置内遅延 */
    USHORT              DlDelay[CMD_SYS_BOTH_3G_S3G];        /**< 下り遅延量 */
    USHORT              UlDelay[CMD_SYS_BOTH_3G_S3G];        /**< 上り遅延量 */
} T_RRH_RE_DELAY_INF;

/**
 * @brief   遅延補正(CPRT)
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
    UINT regDat;
    UINT valid;
    UINT hyper_frame;
    UINT basic_frame;
    UINT clock;
} T_CPR_DLY_CPRT;

/**
 * @brief   遅延補正最大値情報
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
    USHORT max_re_num;
    INT    max;
    INT    REn[D_RRH_CPRINO_RE_MAX];
} T_CPR_DLY_MAX;

/**
 * @brief   T_RRH_DELAY_INF 遅延補正情報
 * @note
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */
typedef struct {
    USHORT              MaxT12Re;                                   /**< 最大T12_RE番号 */
    USHORT              MaxDlEquipDelayReNo[CMD_MAX_SYSNUM];        /**< 最大下り装置内遅延_RE番号 */
    USHORT              MaxUlEquipDelayReNo[CMD_MAX_SYSNUM];        /**< 最大上り装置内遅延_RE番号 */
    USHORT              MaxDlDelayReNo[CMD_MAX_SYSNUM];             /**< 最大下り遅延量_RE番号 */
    USHORT              MaxUlDelayReNo[CMD_MAX_SYSNUM];             /**< 最大上り遅延量_RE番号 */
    USHORT              MaxDlDelayRe[CMD_MAX_SYSNUM];               /**< 最大下り遅延量 */
    USHORT              MaxUlDelayRe[CMD_MAX_SYSNUM];               /**< 最大上り遅延量 */
    UINT                MaxFhmDlDelay[CMD_MAX_SYSNUM];
    UINT                MaxFhmUlDelay[CMD_MAX_SYSNUM];
    UINT                MaxFiberDelay;
    USHORT              DlDelayBase[CMD_MAX_SYSNUM];                /**< 下り遅延量基準値 */
    USHORT              UlDelayBase[CMD_MAX_SYSNUM];                /**< 上り遅延量基準値 */
    USHORT              DlDelayBaseAlign[CMD_MAX_SYSNUM];           /**< 下り遅延基準値アライン */
    USHORT              UlDelayBaseAlign[CMD_MAX_SYSNUM];           /**< 上り遅延基準値アライン */
    USHORT              X_Fhm;                                      /**< X_FHM */
    USHORT              Y_Fhm;                                      /**< F_FHM */
    USHORT              ToffsetReport;                              /**< Toffset_Report */
    USHORT              FhmToffset;                                 /**< FHM_Toffset */
    INT                 Calc42n[D_RRH_CPRINO_RE_MAX];
    USHORT              toffsetRslt[CMD_MAX_SYSNUM];                /**< RECに通知するToffset報告応答結果 */
    /** CPM#n LTE DL補正済み */
    INT                 f_cprw_dly_alreadyAdjLteDL[D_RRH_CPRINO_RE_MAX];
    /** CP_CPRTDLY */
    T_CPR_DLY_CPRT      f_cprw_dly_cp_cprtdly[D_RRH_CPRINO_RE_MAX];
    /** RE使用/未使用 */
    T_RRH_CONN_DISC     f_cprw_shm_conn;
    /** Toffset報告内容(S3G) */
    T_RRH_TOFFSET_S3G   f_cprw_shm_toffset_s3g[D_RRH_CPRINO_RE_MAX];
    /** Toffset報告内容(3G) */
    T_RRH_TOFFSET_3G    f_cprw_shm_toffset_3g[D_RRH_CPRINO_RE_MAX];
    /** REn_T14<@245.76MHz> */
    INT                 f_cprw_dly_REn_T14[D_RRH_CPRINO_RE_MAX];
    /** RE Toffset<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_ReReport_Toffset[CMD_MAX_SYSNUM];
    /** RE下り装置内遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_ReReport_Dl[CMD_MAX_SYSNUM];
    /** RE上り装置内遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_ReReport_Ul[CMD_MAX_SYSNUM];
    /** ファイバ伝播遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_T12_RE;
    /** FHM下り装置内遅延とファイバ伝播遅延とRE下り装置内遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_Xn[CMD_MAX_SYSNUM];
    /** FHM上り装置内遅延とファイバ伝播遅延とRE上り装置内遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_Yn[CMD_MAX_SYSNUM];
    /** LTE下り遅延＋ファイバ遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_MAX_SYSNUM];
    /** LTE上り遅延＋ファイバ遅延<@245.76MHz> */
    T_CPR_DLY_MAX       f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_MAX_SYSNUM];
    /** 比較に使用するT14 */
    UINT                f_cprw_base_REn_T14[D_RRH_CPRINO_RE_MAX];
    /** 最大遅延超過時の固定遅延量から減算する遅延量(1~32)<@245.76MHz> */
    UINT                f_cprw_dly_adjLUDly[D_RRH_CPRINO_RE_MAX];
    UINT                f_cprw_dly_adjWUDly[D_RRH_CPRINO_RE_MAX];
    UINT                f_cprw_dly_adjLDDly[D_RRH_CPRINO_RE_MAX];
    UINT                f_cprw_dly_adjWDDly[D_RRH_CPRINO_RE_MAX];
    UINT                f_cprw_dly_failsafe_3G[D_RRH_CPRINO_RE_MAX];
    UINT                f_cprw_dly_failsafe_S3G[D_RRH_CPRINO_RE_MAX];
} T_RRH_DELAY_INF;
/** @} */
/****************************************************************************************************************************/
/**
 * @name RE PORT状態報告
 * @{
 */
/****************************************************************************************************************************/
/**
 * @brief	RE PORT状態報告(S3G)
 * @date	2016/04/19 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
	/**
	 * 割り当て有効.
	 *
	 * <Array index definition> \n
	 * CPRIリンク番号. [0]:RE#1 .. [15]:RE#16
	 *
	 * <Description>
	 * 該当REがPORT入力状態報告の集約対象かどうかを表す.
	 * 起動時、EEPROMから読み出したPORT入力状態設定CPRI番号を判定し、CPRI番号が格納されているREを集約対象として初期値を設定する.
	 * FHM-MTから設定要求を受信時、要求のPORT入力状態設定CPRI番号を判定し、CPRI番号が格納されているREを集約対象として更新する.
	 * - 0(D_RRH_OFF) : 対象外
	 * - 1(D_RRH_ON)  : 対象
	 */
	UCHAR alloc_en[D_RRH_CPRINO_RE_MAX];

	/**
	 * PORT入力状態設定CPRI番号.
	 *
	 * <Array index definition> \n
	 * RECに報告するPORT入力状態のBit番号.
	 * [0]:Bit0 .. [15]:Bit15
	 *
	 * <Description> \n
	 * RECに報告するPORT入力状態の該当Bitが、どのCPRI番号のREと紐づいているかを表す.
	 * 起動時、EEPROMから読み出した内容で初期値を設定する.
	 * FHM-MTから設定要求を受信時、設定値を保存する.
	 */
	UCHAR cpri_no[16];

	/**
	 * PORT入力状態設定BIT位置.
	 *
	 * <Array index definition> \n
	 * RECに報告するPORT入力状態のBit番号.
	 * [0]:Bit0 .. [15]:Bit15
	 *
	 * <Description> \n
	 * RECに報告するPORT入力状態の該当Bitが、REから報告されたPORT入力状態のいずれのBitと紐づいているかを表す.
	 * 起動時、EEPROMから読み出した内容で初期値を設定する.
	 * FHM-MTから設定要求を受信時、設定値を保存する.
	 */
	UCHAR bit_no[16];

	/**
	 * REからのRE PORT状態報告応答の受信状態を表す.
	 *
	 * <Array index definition> \n
	 * CPRIリンク番号. [0]:RE#1 .. [15]:RE#16
	 *
	 * <Description>
	 * - 0( D_RRH_REPORTSTA_RSP_NOT_RECEIVED	) : 未受信
	 * - 1( D_RRH_REPORTSTA_RSP_NORMAL			) : 正常応答
	 * - 2( D_RRH_REPORTSTA_RSP_NG				) : NG応答
	 * - 3( D_RRH_REPORTSTA_RSP_TO				) : タイムアウト
	 * - 未使用/使用を切替時、初期化する.
	 * - 割り当て有効を無効から有効に切替時、初期化する.
	 * - RECへの報告で参照時、PORT入力状態割り当てのいずれかが0以外
	 *   かつ、RE使用/未使用を決定するON/OFFがONの場合のみ参照すること.
	 *
	 * <Initial Value>
	 * - 0x0000
	 *
	 * <Range of Value>
	 * - 0x0000 .. 0x0003
	 */
	UCHAR received_rsp[D_RRH_CPRINO_RE_MAX];
	/**
	 * RE PORT状態報告応答の応答結果.
	 *
	 * <Array index definition> \n
	 * CPRIリンク番号. [0]:RE#1 .. [15]:RE#16
	 *
	 * <Description>
	 * - デバッグ用途.
	 * - 未使用/使用を切替時、初期化する.
	 * - 割り当て有効を無効から有効に切替時、初期化する.
	 * - RECへの報告で参照時、PORT入力状態割り当てのいずれかが0以外
	 *   かつ、RE使用/未使用を決定するON/OFFがONの場合のみ、参照すること.
	 *
	 * <Initial Value>
	 * - 0x0000
	 *
	 * <Range of Value>
	 * - 0x0000 .. 0xFFFF
	 */
	USHORT rsp_result[D_RRH_CPRINO_RE_MAX];
	/**
	 * RE PORT状態報告応答のPORT入力状態.
	 *
	 * <Array index definition> \n
	 * CPRIリンク番号. [0]:RE#1 .. [15]:RE#16
	 *
	 * <Description>
	 * - 未使用/使用を切替時、初期化する.
	 * - 割り当て有効を無効から有効に切替時、初期化する.
	 * - RECへの報告で参照時、PORT入力状態割り当てのいずれかが0以外
	 *   かつ、RE使用/未使用を決定するON/OFFがONの場合のみ、参照すること.
	 *
	 * <Initial Value>
	 * - 0x0000
	 *
	 * <Range of Value>
	 * - 0x0000 .. 0xFFFF
	 */
	USHORT rsp_portin[D_RRH_CPRINO_RE_MAX];
	/**
	 * RE PORT状態報告応答(for REC)統計情報.
	 *
	 * <Array index definition> \n
	 * 応答要因. [0]:無応答(全配下RE応答TO)
	 *           [1]:NG応答
	 *           [2]:OK応答(全配下RE未使用)
	 *           [3]:OK応答(FHM-MT Port割当なし)
	 *
	 * <Description>
	 * - RECへの報告で参照時に正常応答しなかった回数をカウントする.
	 *
	 * <Initial Value>
	 * - 0x0000
	 *
	 * <Range of Value>
	 * - 0x0000 .. 0xFFFF
	 */
	USHORT rsp_result_rec[4];
	/**
	 * RE PORT状態報告応答(for REC)NON OK応答統計情報.
	 *
	 * <Array index definition> \n
	 * 応答要因. [0]:無応答(全配下RE応答TO)
	 *           [1]:NG応答
	 *           [2]:OK応答(全配下RE未使用)
	 *           [3]:OK応答(FHM-MT Port割当なし)
	 *
	 * <Description>
	 * - RECへの報告でRECに正常OK応答する場合に初期化
	 * - RECへの報告で参照、各要因にてAssertログを取得するかを決定する.
	 *
	 * <Initial Value>
	 * - 0x0000
	 *
	 * <Range of Value>
	 * - 0x0000 .. 0xFFFF
	 */
	USHORT rsp_result_rec_nonok[4];
} T_RRH_REPORTSTA_S3G;
/** @} */

/* ======================================================================== */
/* DU Process information													*/
/* ======================================================================== */
/**
 * @brief	DU Process information
 * @note	DU Process information
  */
typedef struct{
	UINT						pState[D_RRH_PROCID_MAX];			/**< ProcessInit状態(initCmp(0)/notCmp(1))		*/
	INT							pid[D_RRH_PROCID_MAX];
	UINT						dbgmode;
	UINT						almmsk;
}T_RRH_DU_PROCESS_INFO;


/* ======================================================================== */
/* SFN/FRM Calibration Table												*/
/* ======================================================================== */
/**
 * @brief	SFN/FRM Calibration Table
 * @note	2018/6/26 Add
  */
typedef struct{
	UCHAR						sfnfrm_flag;
	UCHAR						ns_flag;
	INT							offset_a;
	INT							offset_b;
}T_RRH_SFNFRM_CALB_TBL;

/****************************************************************************************************************************/
/** T_RRH_LOG_ETH_ERR_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_ETH_ERR_HIS_RECORD
 * @note	T_RRH_LOG_ETH_ERR_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME		sysTime;										/**< System Time								*/
	UINT					regdata;										/**< Register Log (MAC Status Register)			*/
	UINT					regdata_info;									/**< Register Log (MAC Status Register)			*/
	UINT					linkupdown;										/**< Register Log (MAC Status Register)			*/
	UINT					regdata_sfp;									/**< Register Log (MAC Status Register)			*/
	UINT					regdata_his;									/**< Register Log (MAC Status Register)			*/
	UINT					regdata_jcpll;									/**< Register Log (MAC Status Register)			*/
	UINT					dummy[1];										/**< Dummy										*/
	UINT					validCode;										/**< Register Log (MAC Status Register)			*/
}T_RRH_LOG_ETH_ERR_HIS_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_ETH_ERR_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_ETH_ERR_HIS
 * @note	T_RRH_LOG_ETH_ERR_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;								/**< 書込みindex								*/
	UINT							dummy[3];
	T_RRH_LOG_ETH_ERR_HIS_RECORD	tRecord[D_RRH_LOG_ETHERR_RCD_NUM];		/**< Ethernet リンク障害履歴ログ				*/
}T_RRH_LOG_ETH_ERR_HIS;
/** @} */


/* ======================================================================== */
/* PTP Parameter Table														*/
/* ======================================================================== */
/**
 * @brief	PTP Parameter Table
 * @note	2018/9/2 Add
 *          2018/09/12 actport add
  */
typedef struct{
	INT						clockType;
	INT						domainNumber;
	INT						clockClass;
	INT						clockAccuracy;
	INT						priority1;
	INT						priority2;
	UCHAR					sfnfrm_flag;
	UCHAR					ptp_lockedStatus;
	UCHAR					actport;
}T_RRH_PTPCONF_MNG;

/* ======================================================================== */
/* PTP Status log Table														*/
/* ======================================================================== */
/**
 * @brief	PTP Status Record Table
 * @note	2018/9/5 Add
  */
typedef struct{
	T_RRH_SYSTEM_TIME		tDate;
	UCHAR					ptp_State;
}T_RRH_LOG_PTPSTATUS_RECORD;

/* ======================================================================== */
/* PTP Status log Table														*/
/* ======================================================================== */
/**
 * @brief	PTP Parameter Table
 * @note	2018/9/5 Add
  */
typedef struct{
	UINT						uiWriteIdx;
	UINT						uiCycleflg;
	UINT						maskInfo;
	UINT						dummy;
	T_RRH_LOG_PTPSTATUS_RECORD	tRecord[D_RRH_LOG_REGLOG_RCD_NUM];
}T_RRH_LOG_PTP_STATUS;


/* ======================================================================== */
/* PTP APR Table														*/
/* ======================================================================== */
/**
 * @brief	PTP APR Parameter Table
 * @note	2018/9/24 Add
  */
typedef struct{
	UINT					valid;
	UINT					index;
	INT						len;
	UINT					val;
}T_RRH_PTP_APR_PARM_INFO;

typedef struct{
	UINT					saveflg;
	UINT					length;
	UINT					dummy[2];
	T_RRH_PTP_APR_PARM_INFO	info[127];
}T_RRH_PTP_APR_PARM;

/****************************************************************************************************************************/
/** T_RRH_DU_CMD_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/

/**
 * @brief	T_RRH_DU_CMD_INFO_COMMON
 * @note	T_RRH_DU_CMD_INFO_COMMON is ..
 */
typedef struct{
	INT									pid;
	UINT								device_type;
	UINT								support_band;
	UINT								mmw_power_mode;
	UINT								tx_mode;
	UINT								radio_frame_ofs;
	INT									sfn_ofs;
	UINT								ptp_valid;
	UINT								fh_valid;
	UINT								reset_reason;
	UINT								force_uplane;

	UINT								mplane_con;
	UINT								low_vlan;
	UINT								high_vlan;
	UINT								last_vlan;
	UINT								pcp;
	UINT								ssh_port;
	UINT								ssh_port_callhome;

	UINT								startup_face;

	UINT								bfmode;

	UINT								txgain;

}T_RRH_DU_CMD_INFO_COMMON;
/** @} */

/**
 * @brief	T_RRH_DU_CMD_INFO_PTP
 * @note	T_RRH_DU_CMD_INFO_PTP is ..
 */
typedef struct{
	UINT								port_number;
	UINT								domain_number;
	UINT								multicast_mac_addr;
	UINT								accepted_clock_classes;
	UINT								sync_status;
	UINT								syncE_status;
	UINT								ptp_status;
	UINT								ptp_clock_class;
}T_RRH_DU_CMD_INFO_PTP;
/** @} */

/**
 * @brief	T_RRH_DU_CMD_INFO_FH
 * @note	T_RRH_DU_CMD_INFO_FH is ..
 */
typedef struct{
	UINT								link_onoff;
	UINT								remote_mac_addr_h;
	UINT								remote_mac_addr_l;
	UINT								vid_dl;
	UINT								vid_ul;
	UINT								pcp_ul;
}T_RRH_DU_CMD_INFO_FH;
/** @} */

/**
 * @brief	T_RRH_DU_CMD_INFO_TX
 * @note	T_RRH_DU_CMD_INFO_TX is ..
 */
typedef struct{
	UINT								onoff;				/* du -txendpw or du -rxendpw */
	UINT								Acell_id;			/* du -txendpw or du -rxendpw */
	UINT								CC_id;				/* du -txendpw or du -rxendpw */
	UINT								Port_id;			/* du -txendpw or du -rxendpw */
	UINT								Acell_id_ex;		/* du -txendpw or du -rxendpw */
	UINT								CC_id_ex;			/* du -txendpw or du -rxendpw */
	UINT								Port_id_ex;			/* du -txendpw or du -rxendpw */
}T_RRH_DU_CMD_INFO_ENDP;

typedef struct{
	UINT								onoff;				/* du -txon or du -rxon */
	UINT								test_onoff;			/* du -testtxon */
	UINT								nr_freq;			/* du -txcw or du -rxcw */
	UINT								nr_bandwidth;		/* du -txcw or du -rxcw */
	UINT								ul_fft_smpl_ofs;	/* du -rxcw only */
	UINT								ta_offset;			/* du -rxcw only  */
	UINT								nr_scs;			/* du -txcw only  */
}T_RRH_DU_CMD_INFO_ARRAYCAR;

typedef struct{
	T_RRH_DU_CMD_INFO_ENDP				endpoint[8];
	T_RRH_DU_CMD_INFO_ARRAYCAR			array[8];
}T_RRH_DU_CMD_INFO_TXCC;
/** @} */

/**
 * @brief	T_RRH_DU_CMD_INFO_TX
 * @note	T_RRH_DU_CMD_INFO_TX is ..
 */
typedef struct{
	T_RRH_DU_CMD_INFO_ENDP				endpoint[8];
	T_RRH_DU_CMD_INFO_ARRAYCAR			array[8];
}T_RRH_DU_CMD_INFO_RXCC;
/** @} */

/**
 * @brief	T_RRH_DU_CMD_INFO
 * @note	T_RRH_DU_CMD_INFO is ..
 */
typedef struct{
	T_RRH_DU_CMD_INFO_COMMON			com_info;
	T_RRH_DU_CMD_INFO_PTP				ptp_info;
	T_RRH_DU_CMD_INFO_FH				fh_info[2];
	T_RRH_DU_CMD_INFO_TXCC				txcc_info[8];
	T_RRH_DU_CMD_INFO_RXCC				rxcc_info[8];
}T_RRH_DU_CMD_INFO;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_DU_FAULT_MNG
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/

/**
 * @brief	T_RRH_DU_FAULT_MNG
 * @note	T_RRH_DU_FAULT_MNG is ..
 */
typedef struct{
	UINT								fltLvlNum[E_DU_SVCTL_FLVNO_MAX];
	UINT								fltIdNum[E_DU_SVCTL_OCCUR_FIDNO_MAX];
}T_RRH_DU_FAULT_MNG;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_CAL_COEF_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_DU_FAULT_MNG
 * @note	T_RRH_DU_FAULT_MNG is ..
 */
typedef struct{
	UINT								cal_coef[D_RRH_ANT_MAX];
	DL									phase[D_RRH_ANT_MAX];
}T_RRH_CAL_COEF_TBL;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_PFM_PERFORMANCE_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_PFM_PERFORMANCE_TBL
 * @note	T_RRH_PFM_PERFORMANCE_TBL is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME					time;
	UINT								reg_data_addr[D_RRH_PFM_NUM_OF_REG_ALL];
	UINT								reg_data[D_RRH_PFM_NUM_OF_REG_ALL];
}T_RRH_PFM_PERFORMANCE_INFO_PER;

typedef struct{
	UINT								fact;
	T_RRH_SYSTEM_TIME					time;
	UINT								reg_data[D_RRH_CAR_MAX][D_RRH_ANT_MAX][D_RRH_PFM_NUM_OF_REG];
	UINT								reg_data_com[D_RRH_PFM_NUM_OF_REG_COM];
}T_RRH_PFM_PERFORMANCE_INFO;

typedef struct{
	UINT								index;
	UINT								counter;
	UINT								devkind;
	UINT								index_peri;
	UINT								counter_peri;
	UINT								fault_peri;
	T_RRH_PFM_PERFORMANCE_INFO			info[D_RRH_PFM_NUM_OF_INFO];
	T_RRH_PFM_PERFORMANCE_INFO_PER		info_peri[D_RRH_PFM_NUM_OF_INFO_PER];
}T_RRH_PFM_PERFORMANCE_TBL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_PFM_PERFORMANCE_MNG_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_PFM_PERFORMANCE_MNG_TBL
 * @note	T_RRH_PFM_PERFORMANCE_MNG_TBL is ..
 */

typedef struct{
	T_RRH_SYSTEM_TIME					start;
	T_RRH_SYSTEM_TIME					end;
	ULLINT								count[D_RRH_IF_FHMAX];
}T_RRH_PFM_MEASUREMENT_INFO;

typedef struct{
	UINT								interval;
	UINT								init_interval;
	UINT								time_cnt;
	T_RRH_PFM_MEASUREMENT_INFO			object[D_RRH_PFM_NUM_OF_RXWIN];
}T_RRH_PFM_INTERVAL_INFO;

typedef struct{
	USHORT								pfm_state[D_RRH_PFM_NUM_OF_RXWIN];
	USHORT								activate_state[D_RRH_PFM_NUM_OF_RXWIN];
	UINT								timer_state;
	UCHAR								dummy[4];
	T_RRH_PFM_INTERVAL_INFO				info;
	T_RRH_PFM_INTERVAL_INFO				measurement;
	T_RRH_PFM_INTERVAL_INFO				notification;
}T_RRH_PFM_PERFORMANCE_MNG_TBL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LBM_MNGMENT_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LBM_MNGMENT_TBL
 * @note	T_RRH_LBM_MNGMENT_TBL is ..
 */

typedef struct{
	UINT								valid_flag;				/* 有効/無効フラグ					*/
	CHAR								ma_id[260];				/* xran-lbm/id	(D_EOM_LBM_STR_LEN+4)byte		*/
	CHAR								ma_name[48];			/* xran-lbm/name(D_EOM_LBM_STR_NAME_LEN+4)byte	*/
	UINT								md_level;				/* xran-lbm/md-level				*/
}T_RRH_LBM_MNGMENT_INFO;

typedef struct{
	UINT								port;					/* Port								*/
	UINT								valid_flag;				/* 有効/無効フラグ					*/
	INT									sock;					/* ソケットディスクリプタ			*/
}T_RRH_LBM_PORT_INFO;

typedef struct{
	UINT								num_of_use;
	T_RRH_LBM_MNGMENT_INFO				info[8];				/* LBR/LBM管理テーブル				*/
	T_RRH_LBM_PORT_INFO					port_info[D_RRH_IF_FHMAX];
}T_RRH_LBM_MNGMENT_TBL;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_BF_MODE_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_BF_MODE_TBL
 * @note	T_RRH_BF_MODE_TBL is ..
 */
typedef struct{
	UINT								flag;					/*Beamforming Mode (ON:nonBF/OFF:BF)*/
}T_RRH_BF_MODE_TBL;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_DU_EIO_SVCTRL_MNG(5G-商用DU用)
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_DU_EIO_SVCTRL_MNG
 * @note	T_RRH_DU_EIO_SVCTRL_MNG is ..
 */



typedef struct{		/*	デバッグ用	*/
	USHORT								fanMonCnt;						/*	1...3600(sec)	*/
	USHORT								fanMonIdxH;						/*	0...99			*/
	USHORT								fanMonIdxM;						/*	0...99			*/
	USHORT								fanMonReserve;					/*	予備			*/
	USHORT								fanMonH[100][E_DU_FAN_MAXNO][5];	/*	統計情報(Hour)	*/
	USHORT								fanMonM[100][E_DU_FAN_MAXNO][5];	/*	統計情報(min)	*/
}T_RRH_DU_FANSPEED_MON;

typedef struct{
	UINT								fanSpeedFault;
	UINT								fanSpeed[E_DU_FAN_MAXNO];
	UINT								fanSpeedReserve[4];
}T_RRH_DU_FANSPEED_INFO;

typedef struct{
	UINT								fanUnitFault;
	UINT								fanSvReserve[3];
	T_RRH_DU_FANSPEED_INFO				fanSpeedInfo;
}T_RRH_DU_FANSV_INFO;

typedef struct{
	UINT								fanDacVal;
	UINT								fanDacCtrlReserve[3];
}T_RRH_DU_FANDACCTRL_PARAM;

typedef struct{
	UINT								fanDbgFlg;
	UINT								fanFlgReseve[3];
	T_RRH_DU_FANDACCTRL_PARAM			fanDacParam[E_DU_FANSPEEDSTS_MAXNO];
}T_RRH_DU_FANCTRL_INFO;

typedef struct{
	T_RRH_DU_FANCTRL_INFO				fanCtrl;
	T_RRH_DU_FANSV_INFO					fanSv;
	T_RRH_DU_FANSPEED_MON				fanSpeedMon;
}T_RRH_DU_EIO_SVCTRL_MNG;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_DU_EIO_SVCTRL_LOG(5G-商用DU用)
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_DU_EIO_SVCTRL_LOG
 * @note	T_RRH_DU_EIO_SVCTRL_LOG is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME					date;
	UCHAR								message[116];
}T_RRH_DU_SVCTRL_LOG_INFO;
/** @} */

typedef struct{
	UINT								index;
	UINT								eioLogReserve[3];
	T_RRH_DU_SVCTRL_LOG_INFO			logInfo[D_DU_EIOLOG_MAX];
}T_RRH_DU_EIO_SVCTRL_LOG;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_FHSTAT_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_FHSTAT_HIS_RECORD
 * @note	T_RRH_LOG_FHSTAT_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				sysTime;												/**< System Time								*/
	UINT							linkinfo;
	UINT							regdata[D_DU_FHSTATLOG_MAX];
}T_RRH_LOG_FHSTAT_HIS_RECORD;
/** @} */

/**
 * @brief	T_RRH_LOG_FHSTAT_HIS
 * @note	T_RRH_LOG_FHSTAT_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;								/**< 書込みindex								*/
	UINT							linkinfo_pre;
	UINT							dummy;
	UINT							counter;
	T_RRH_LOG_FHSTAT_HIS_RECORD		tRecord[D_DU_FHSTATLOG_RCD_NUM];
}T_RRH_LOG_FHSTAT_HIS;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_MT_INFO
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/

/**
 * @brief	T_RRH_MT_COM_CON
 * @note	T_RRH_MT_COM_CON is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				s_date;
	T_RRH_SYSTEM_TIME				e_date;
	UINT							ipAddr;
	USHORT							portNo;
	USHORT							reserve;
}T_RRH_MT_COM_CON;
/** @} */


/**
 * @brief	T_RRH_MT_COM
 * @note	T_RRH_MT_COM is ..
 */
typedef struct{
	T_RRH_MT_COM_CON server;
	T_RRH_MT_COM_CON client;
	UINT			 mt_con_count;
	UCHAR			 beamId;
	UCHAR			 mt_con_state;
	UCHAR			 cu_con_state;
	UCHAR			 dummy;
	UCHAR			 aisg_state; // M-RU chg
	UINT			 blk_ctl;
}T_RRH_MT_COM;
/** @} */

/**
 * @brief	T_RRH_MT_SFP
 * @note	T_RRH_MT_SFP is ..
 */
typedef struct{
	UCHAR			sfpType[16];
	UCHAR			vendorCode[17];	/* NULL文字込 */
	UCHAR			serialNo[17];	/* NULL文字込 */
}T_RRH_MT_SFP;
/** @} */

/**
 * @brief	T_RRH_MT_CAR
 * @note	T_RRH_MT_CAR is ..
 */
typedef struct{
	UCHAR			carrierState[4][4];	/* ant,car */
	UINT			FreqNo[4];			/* kHz */
	UINT			bandWidth[4];		/* kHz */
}T_RRH_MT_CAR;
/** @} */


/**
 * @brief	T_RRH_LOG_MT_HIS_RECORD
 * @note	T_RRH_LOG_MT_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				date;													/**< System Time								*/
	UCHAR							message[116];
}T_RRH_LOG_MT_HIS_RECORD;
/** @} */

/**
 * @brief	T_RRH_LOG_MT_HIS
 * @note	T_RRH_LOG_MT_HIS is ..
 */
typedef struct{
	UINT							index;								/**< 書込みindex								*/
	UINT							dummy;
	T_RRH_LOG_MT_HIS_RECORD			tRecord[D_DU_MTLOG_RCD_NUM];
}T_RRH_LOG_MT_HIS;
/** @} */

/**
 * @brief	T_RRH_MT_INFO
 * @note	T_RRH_MT_INFO is ..
 */
typedef struct{
	T_RRH_MT_COM		comInfo;
	T_RRH_MT_SFP		sfpInfo;
	T_RRH_MT_CAR		carInfo_tx;
	T_RRH_MT_CAR		carInfo_rx;
	T_RRH_LOG_MT_HIS	logInfo;
}T_RRH_MT_INFO;

/** @} */

/**
 * @brief	T_RRH_TXPOW_DETECT_HEAD
 * @note	T_RRH_TXPOW_DETECT_HEAD is ..
 */
typedef struct{
	UINT					validFlg;
	UINT					pgclut_validFlg;
	UCHAR					pgclut_version[8];
	T_RRH_SYSTEM_TIME		output_date;
	UINT					dummy;
	T_RRH_SYSTEM_TIME		output_date_pre;
	UINT					dummy2;
	UINT					dummy3[4];
}T_RRH_TXPOW_DETECT_HEAD;
/** @} */

/**
 * @brief	T_RRH_TXPOW_DETECT_GAIN_IN
 * @note	T_RRH_TXPOW_DETECT_GAIN_IN is ..
 */
typedef struct{
	DL			FLASH_VH[D_DU_TXPOW_DETECT_VH_MAX];
	DL			YZ_VH[D_DU_TXPOW_DETECT_VH_MAX];
	DL			AVG_VH;
	DL			RVS_VH;
}T_RRH_TXPOW_DETECT_GAIN_IN;
/** @} */


/**
 * @brief	T_RRH_TXPOW_DETECT_GAIN_OUT
 * @note	T_RRH_TXPOW_DETECT_GAIN_OUT is ..
 */
typedef struct{
	UINT		regAddr;
	UINT		regVal;
}T_RRH_TXPOW_DETECT_GAIN_REG;
/** @} */

/**
 * @brief	T_RRH_TXPOW_DETECT_GAIN_OUT
 * @note	T_RRH_TXPOW_DETECT_GAIN_OUT is ..
 */
typedef struct{
	DL								N_VAL[D_DU_TXPOW_DETECT_MAX];
	DL								K_VAL;
	DL								TCB_AVG_VAL;
	DL								POW_VAL;
	T_RRH_TXPOW_DETECT_GAIN_REG		Reg_Cnt[D_DU_TXPOW_DETECT_MAX];
	T_RRH_TXPOW_DETECT_GAIN_REG		Reg_Pow[D_DU_TXPOW_DETECT_VH_MAX];
	UINT							REF_VAL;
	UINT							validFlg;
}T_RRH_TXPOW_DETECT_GAIN_OUT;
/** @} */


/**
 * @brief	T_RRH_TXPOW_DETECT_HIS_RECORD
 * @note	T_RRH_TXPOW_DETECT_HIS_RECORD is ..
 */
typedef struct{
	UINT							validFlg;
	T_RRH_SYSTEM_TIME				date;
	UCHAR							message[140];
	UINT							gainOutValidFlg;
	T_RRH_TXPOW_DETECT_GAIN_OUT		gainOut[E_DU_TXPOW_DETECT_VH_NUM];
}T_RRH_TXPOW_DETECT_HIS_RECORD;
/** @} */

/**
 * @brief	T_RRH_TXPOW_DETECT_HIS
 * @note	T_RRH_TXPOW_DETECT_HIS is ..
 */
typedef struct{
	UINT							index;								/**< 書込みindex								*/
	UINT							validFlg;
	T_RRH_TXPOW_DETECT_HIS_RECORD	tRecord[D_DU_TXPOW_DETECT_HIS_RCD_NUM];
}T_RRH_TXPOW_DETECT_LOG;
/** @} */


/**
 * @brief	T_RRH_TXPOW_DETECT_TBL
 * @note	T_RRH_TXPOW_DETECT_TBL is ..
 */
typedef struct{
	T_RRH_TXPOW_DETECT_HEAD			head;
	T_RRH_TXPOW_DETECT_GAIN_IN		gainIn[D_DU_TXPOW_DETECT_MAX][E_DU_TXPOW_DETECT_VH_NUM];
	T_RRH_TXPOW_DETECT_GAIN_OUT		gainOut[E_DU_TXPOW_DETECT_VH_NUM];
}T_RRH_TXPOW_DETECT_TBL;
/** @} */




/**
 * @brief	T_RRH_LUT_LOG_HEAD_FLASH
 * @note	T_RRH_LUT_LOG_HEAD_FLASH is ..
 */
typedef struct{
	UINT						flash_saving;
	UINT						flash_ofs;
	UINT						flash_len;
	UINT						flash_saved;
}T_RRH_LUT_LOG_HEAD_FLASH;
/** @} */


/**
 * @brief	T_RRH_LUT_LOG_HEAD
 * @note	T_RRH_LUT_LOG_HEAD is ..
 */
typedef struct{
	T_RRH_LUT_LOG_HEAD_FLASH	flash_head[D_DU_LUT_LOG_MAX];
	UINT						dummy[2];
	UINT						validFlg;
	UINT						index;
}T_RRH_LUT_LOG_HEAD;
/** @} */


/**
 * @brief	T_RRH_LUT_DATA
 * @note	T_RRH_LUT_DATA is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME			date;
	USHORT						antOfs;
	USHORT						version;
	UINT						log_info[D_RRH_ANT_MAX];	/* 0xB1200XC0 */
	UINT						lut_data[D_RRH_ANT_MAX][2][0x4000];	/* LUT 0x10000/4 */
}T_RRH_LUT_LOG_DATA;
/** @} */

/**
 * @brief	T_RRH_LUT_LOG
 * @note	T_RRH_LUT_LOG is ..
 */
typedef struct{
	T_RRH_LUT_LOG_HEAD			head;
	T_RRH_LUT_LOG_DATA			log[D_DU_LUT_LOG_MAX];
}T_RRH_LUT_LOG;
/** @} */



/****************************************************************************************************************************/
/** T_RRH_LOG_UPSIGSTOP_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_UPSIGSTOP_HIS_RECORD
 * @note	T_RRH_LOG_UPSIGSTOP_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME		sysTime;										/**< System Time						12byte	*/
	UINT					link_state;
	UINT					sv_fault_state;
	UINT					sv_sync_state;
	UINT					tdd_en_state[D_RRH_ANT_MAX];
	UINT					tddant_en_state[D_RRH_ANT_MAX];
	UINT					txen_state[D_RRH_ANT_MAX];
	UINT					rxen_state[D_RRH_ANT_MAX];
	UINT					regaddr[D_DU_LOG_UPSIGSTOP_REG_NUM];			/**< Register Log 								*/
	UINT					regdata[D_DU_LOG_UPSIGSTOP_REG_NUM];			/**< Register Log 								*/
	UINT					rach_fft_mon[12];
	UINT					dlp_mon[12];
	UINT					alm_mon[12];
	UINT					pa_status;										/**< ハード自律PA OFF/ON履歴 ON/OFF 生Status	*/
	UINT					pa_control_cnt;									/**< ハード自律PA 変化履歴 [31:16 ON/15:0 OFF]	*/
	UINT					ant_reg[D_RRH_ALMHIS_REG_VAL_NUM];
	UINT					validCode;										/**< Valid		*/
}T_RRH_LOG_UPSIGSTOP_HIS_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_UPSIGSTOP_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_UPSIGSTOP_HIS
 * @note	T_RRH_LOG_UPSIGSTOP_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;								/**< 書込みindex								*/
	UINT							devkind;
	UINT							fault;
	UINT							dummy[1];
	T_RRH_LOG_UPSIGSTOP_HIS_RECORD	tRecord[D_DU_LOG_UPSIGSTOP_RCD_NUM];		/**< 主信号停波履歴ログ					*/
}T_RRH_LOG_UPSIGSTOP_HIS;
/** @} */


/****************************************************************************************************************************/
/** T_RRH_LOG_FH_L1_HIS_RECORD
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_FH_L1_HIS_RECORD
 * @note	T_RRH_LOG_FH_L1_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME		sysTime;										/**< System Time						12byte	*/
	UINT					version;
	UINT					link_state;
	UINT					regdata[D_DU_LOG_FH_L1_HIS_REG_NUM];			/**< Register Log 								*/
}T_RRH_LOG_FH_L1_HIS_RECORD;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_FH_L1_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_LOG_FH_L1_HIS
 * @note	T_RRH_LOG_FH_L1_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;											/**< 書込みindex		*/
	UINT							uiWriteIdx_chg;										/**< 書込みindex		*/
	T_RRH_LOG_FH_L1_HIS_RECORD		tRecord[D_DU_LOG_FH_L1_HIS_RCD_NUM];				/**< FH L1履歴ログ		*/
	T_RRH_LOG_FH_L1_HIS_RECORD		tRecord_chg[D_DU_LOG_FH_L1_HIS_RCD_NUM];			/**< FH L1履歴ログ		*/
}T_RRH_LOG_FH_L1_HIS;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_AIH_ALD_LOG_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_AIH_ALD_LOG_TBL
 * @note	T_RRH_AIH_ALD_LOG_TBL is .. ALD time stamp log tbl
 */
typedef struct{
	T_RRH_SYSTEM_TIME				date;											/**< 時刻情報				*/
	UCHAR							message[116];									/**< ALD message			*/
}T_RRH_AIH_ALD_LOG_INFO;

typedef struct{
	UINT							index;											/**< 書込みindex			*/
	UINT							logReserve;										/**< reserve				*/
	T_RRH_AIH_ALD_LOG_INFO			logInfo[500];									/**< ALD TxRx log情報		*/
}T_RRH_AIH_ALD_LOG_TBL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_AIH_ALD_CTRL_TBL
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	T_RRH_AIH_ALD_CTRL_STAT
 * @note	T_RRH_AIH_ALD_CTRL_STAT is .. ALD latest info log tbl
 */
typedef struct{
	T_RRH_SYSTEM_TIME				s_date;											/**< 時刻情報				*/
	T_RRH_SYSTEM_TIME				e_date;											/**< 時刻情報				*/
	ULONG							count;											/**< カウンタ				*/
	ULONG							size;											/**< サイズ					*/
	UCHAR							firstCtl;										/**< 最初のctrl				*/
	UCHAR							lastCtl;										/**< 最後のctrl				*/
	UINT							validFlg;										/**< 有効フラグ				*/
}T_RRH_AIH_ALD_CTRL_STAT;


/**
 * @brief	T_RRH_AIH_ALD_CTRL_TBL
 * @note	T_RRH_AIH_ALD_CTRL_TBL is .. ALD latest info log tbl
 */
typedef struct{
	struct timespec					maxTimes;
	struct timespec					minTimes;
	struct timespec					lastTimes;
}T_RRH_AIH_ALD_CTRL_TIMES;

/**
 * @brief	T_RRH_AIH_ALD_CTRL_TBL
 * @note	T_RRH_AIH_ALD_CTRL_TBL is .. ALD latest info log tbl
 */
typedef struct{
	UINT		deviceScan;
	UINT		l_received_octets;									/**< last received octets	*/
	UINT		f_received_octets;									/**< first received octets	*/
	UCHAR		f_received_data[D_DU_AIH_ALD_CTRL_DATA_MAX];		/**< first received data	*/	
}T_RRH_AIH_ALD_CTRL_DEVSCAN;


/**
 * @brief	T_RRH_AIH_ALD_CTRL_TBL
 * @note	T_RRH_AIH_ALD_CTRL_TBL is .. ALD latest info log tbl
 */
typedef struct{
	T_RRH_SYSTEM_TIME				powerDate;										/**< 時刻情報				*/
	UINT							powerState;										/**< ALD curent pwr state	*/
	UINT							logSuppressFlg;									/**< Log suppress			*/
	UINT							rcvstartFlg;									/**< rcv start flg			*/
	UINT							last_requestId;									/**< last requestId			*/
	UCHAR							last_devAddr;									/**< last device Address	*/
	UCHAR							reserve[3];										/**< reserve				*/
	T_RRH_AIH_ALD_CTRL_DEVSCAN		scan;											/**< device scan			*/
	ULONG							devScanCount;									/**< device scan count		*/
	UCHAR							devValid[D_DU_AIH_ALD_CTRL_DEV_MAX];			/**< 有効フラグ				*/
	UCHAR							devName[D_DU_AIH_ALD_CTRL_DEV_MAX][D_DU_AIH_ALD_CTRL_UIDLEN];			/**< 有効フラグ				*/
	T_RRH_AIH_ALD_CTRL_STAT			devAddrSnd[D_DU_AIH_ALD_CTRL_DEV_MAX][D_DU_AIH_ALD_CTRL_FRAME_TYPE_NUM];		/**< counter				*/
	T_RRH_AIH_ALD_CTRL_STAT			devAddrRcv[D_DU_AIH_ALD_CTRL_DEV_MAX][D_DU_AIH_ALD_CTRL_FRAME_TYPE_NUM];		/**< counter				*/
	T_RRH_AIH_ALD_CTRL_TIMES		devTdiffRR[D_DU_AIH_ALD_CTRL_DEV_MAX];			/**< 時刻差(RU-ALD)			*/
	T_RRH_AIH_ALD_CTRL_TIMES		devTdiffCR[D_DU_AIH_ALD_CTRL_DEV_MAX];			/**< 時刻差(CU-RU)			*/
}T_RRH_AIH_ALD_CTRL_INFO;

typedef struct{
	T_RRH_AIH_ALD_CTRL_INFO			ctrlInfo;										/**< ALD latest info 		*/
}T_RRH_AIH_ALD_CTRL_TBL;
/** @} */

/****************************************************************************************************************************/
/** T_RRH_LOG_CAL_HIS
 *  @addtogroup RRH_RRH
 *  @{
 */
/****************************************************************************************************************************/

/**
 * @brief	T_RRH_LOG_CAL_HIS_INDEX
 * @note	T_RRH_LOG_CAL_HIS_INDEX is ..
 */
typedef struct{
	UINT	fastIndex;
	UINT	slowIndex;
	UINT	errIndex;
	ULONG	fastAllCnt;
	ULONG	slowAllCnt;
	ULONG	fastErrAllCnt;
	ULONG	slowErrAllCnt;
}T_RRH_LOG_CAL_HIS_INFO;
/** @} */

/**
 * @brief	T_RRH_LOG_CAL_HIS_RECORD
 * @note	T_RRH_LOG_CAL_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME		sysTime;										/**< System Time						12byte	*/
	UINT					calType;
	UINT					freqNo;
	DL						refPower;
	DL						fbPower;
	UINT					calCount;
	UINT					validCode;										/**< Valid		*/
}T_RRH_LOG_CAL_HIS_RECORD_ONE;
/** @} */

/**
 * @brief	T_RRH_LOG_CAL_HIS_RECORD_ONE
 * @note	T_RRH_LOG_CAL_HIS_RECORD_ONE is ..
 */
typedef struct{
	T_RRH_LOG_CAL_HIS_RECORD_ONE	fast[D_DU_LOG_CAL_HIS_FAST_NUM];
	T_RRH_LOG_CAL_HIS_RECORD_ONE	slow[D_DU_LOG_CAL_HIS_SLOW_NUM];
	T_RRH_LOG_CAL_HIS_RECORD_ONE	err[D_DU_LOG_CAL_HIS_ERR_NUM];
}T_RRH_LOG_CAL_HIS_RECORD;
/** @} */

/**
 * @brief	T_RRH_LOG_CAL_HIS
 * @note	T_RRH_LOG_CAL_HIS is ..
 */
typedef struct{
	T_RRH_LOG_CAL_HIS_INFO			info_tx[D_RRH_ANT_MAX];
	T_RRH_LOG_CAL_HIS_INFO			info_rx[D_RRH_ANT_MAX];
	T_RRH_LOG_CAL_HIS_RECORD		tRecord_tx[D_RRH_ANT_MAX];
	T_RRH_LOG_CAL_HIS_RECORD		tRecord_rx[D_RRH_ANT_MAX];
	UINT							validFlg;
}T_RRH_LOG_CAL_HIS;
/** @} */

/**
 * @brief	T_RRH_LOG_DLF_HIS_RECORD
 * @note	T_RRH_LOG_DLF_HIS_RECORD is ..
 */
typedef struct{
	T_RRH_SYSTEM_TIME				s_date;															/**< 1回目のラッチ時刻情報				*/
	T_RRH_SYSTEM_TIME				e_date;															/**< 最後のラッチ時刻情報				*/
	UINT							add_time;														/**< 累積回数(1~60)			*/
	UINT							regdat_sum[D_RRH_POLARIZATION_NUM][D_DU_LOG_DLF_HIS_GRP_NUM];	/**< レジスタ合計値			*/
	
}T_RRH_DLF_LOG_INFO;

/**
 * @brief	T_RRH_LOG_DLF_HIS
 * @note	T_RRH_LOG_DLF_HIS is ..
 */
typedef struct{
	UINT							uiWriteIdx;						/**< 書込みindex								       */
	UINT							dlffin_flag;					/**< DLF統計情報収集完了フラグ	0: 未完了 1:完了		*/
	T_RRH_DLF_LOG_INFO				dlf_info[D_DU_LOG_DLF_HIS_TIME];					

}T_RRH_LOG_DLF_HIS;

#endif
/** @} */
