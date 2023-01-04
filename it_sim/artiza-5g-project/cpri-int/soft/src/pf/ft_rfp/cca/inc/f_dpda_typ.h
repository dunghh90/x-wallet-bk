/*!
 * @skip  $ld:$
 * @file  f_dpda_typ.h
 * @brief DPDAタスク 構造体宣言
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2011/09/20 ALPHA)藤井 SRRU-2.1G対応
 * @date  2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00715 対処
 * @date  2011/11/07 ALPHA)横山 M-RRU-ZSYS-00716 対処
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00734 対処
 * @date  2011/11/30 ALPHA)藤井 M-RRU-ZSYS-00726 対処
 * @date  2011/12/06 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date  2012/03/06 ALPHA)横山 N21GLTE対応
 * @date  2012/04/10 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00842対応(ハソ09版)
 * @date  2012/05/09 ALPHA)藤井 M-RRU-ZSYS-00871対処
 * @date  2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 * @date  2015/06/19 ALPHA)近間 RRH-007-000 DCM_3.5G対応
 * @date  2018/09/08 KCN)吉木   5G-DU対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_TYP_H
#define F_DPDA_TYP_H


/******************************************************************************************************************************/
/** carrier info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT					CarOnOff;							/* キャリアONOFF情報							*/
	USHORT					BandWidth;							/* BandWidth									*/
	UINT					ChannelNum;							/* キャリア周波数								*/
	UINT					CarSleep;							/* キャリアスリープ								*/

	UINT					AcellId;							/* ACELL ID										*/
	UINT					CcId;								/* CC ID										*/
	UINT					PortId;								/* PORT ID										*/

	DL						Gain;								/* Gain										*/

}T_DPDA_CAR_INFO;
/** @} */

/******************************************************************************************************************************/
/** 運用中CAL info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT					calFailCount;	 			  					 /* 運用中CALの失敗回数		*/
    DL						phase[D_DPDA_CAL_FREQ_NUM];                      /* 運用中CAL Phase               */
	UINT					cal_ant_flg[D_DPDA_CAL_FREQ_NUM+1]; 			 /**< CalFLAG#ANT 周波数分+高速CAL分	*/
}T_DPDA_CAL_OPE_INFO;
/** @} */

/******************************************************************************************************************************/
/** ant info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT					PaOnOff;							/* PA ON/OFF情報				*/
	UINT					ApdStopWaveState;					/* 停波 ON/OFF情報				*/
	UINT					rscAlm;								/* リソースアラーム状態			*/
	UINT					dpdMode;							/* DPD mode						*/
	T_DPDA_CAR_INFO			car_info[D_SYS_ANT_DIRECTION_NUM][D_DPDA_CARMAX];			/* キャリア情報					*/
	T_DPDA_CAL_OPE_INFO		txopecal_info;							/* 運用中TXCAL情報 */
	T_DPDA_CAL_OPE_INFO		rxopecal_info;							/* 運用中RXCAL情報 */
}T_DPDA_ANT_INFO;
/** @} */

/******************************************************************************************************************************/
/** ant carrier state info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_DPDA_ANT_INFO			ant_info[D_DPDA_ANTMAX];			/* ANT情報						*/
}T_DPDA_ANTCARSTATEINFO;
/** @} */

/******************************************************************************************************************************/
/** register data table
 *  ※この構造体を修正するときはAlarm History Log用テーブル(T_COM_S_MEASREG_DATA)も修正すること
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT			regVal[D_RRH_ALMHIS_REG_VAL_NUM];						/* alarm his log用レジスタリード値  */
}T_DPDA_REGDATA;

typedef struct{
	UINT			regVal[D_RRH_ALMHIS_REG_EXT_NUM];						/* alarm his log用レジスタリード値  */
}T_DPDA_EXTDATA;

/** @} */

/******************************************************************************************************************************/
/** status information System Parameter Setting info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT			sysBandWidth[D_DPDA_CARMAX];							/* LTE帯域										*/
	UINT			sec_nChip[D_DPDA_ANTMAX];								/* sector n-chip								*/
	UINT			txSysNum;												/* 送信系統数									*/
	UINT			cpriRate;												/* CPRI Rate									*/
}T_DPDA_SYS_PARA_INFO;
/** @} */

/******************************************************************************************************************************/
/** status information RPT table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	INT				Rtwp[D_DPDA_ANTMAX_RX][D_DPDA_CARMAX];					/* キャリア受信電力(RTWP)						*/
	INT				AntPower[D_DPDA_ANTMAX];								/* アンテナ電力[dBm]							*/
	INT				FbPower[D_DPDA_ANTMAX];									/* FB電力										*/
	INT				Temperature;											/* Temperature									*/
	INT				Tssi[D_DPDA_ANTMAX][D_DPDA_CARMAX];						/* TSSI											*/
	INT				RxPower[D_DPDA_ANTMAX];									/* Rx Power										*/
	INT				iVswrRL[D_DPDA_ANTMAX];									/* VSWR	RL										*/
	DL				VswrRL[D_DPDA_ANTMAX];									/* VSWR	RL										*/
	UINT			txPwrPsum[D_DPDA_ANTMAX][16];							/* pgcpsum										*/
	UINT			txPwrCnt[D_RRH_ANT_MAX][45];							/* pgcpcnt										*/
	UINT			txPwrMon[D_DPDA_ANTMAX];								/* pgcpowmon	 */
	UINT			txPwrPsum_avg[D_DPDA_ANTMAX];							/* pgcpsum average	 */
}T_DPDA_STATUS_INFO;

typedef struct{
	INT				RxTempReduction;										/* 温度補償制御RX 温度補償リダクション量[dBm]	*/
	INT				RxGainCompVal[D_DPDA_ANTMAX_MMW];						/* Rx Gain比較値[dBm]							*/
	UINT			dummy[2];												/* alarm his log用レジスタリード値				*/
}T_DPDA_EXT_STATUS_INFO;
/** @} */

/******************************************************************************************************************************/
/** status information for debug table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT			dpdState;												/* DPD状態(高速・通常・停止)		*/
	UINT			fwDelay;												/* FW遅延							*/
	UINT			fbDelay;												/* FB遅延							*/
	INT				digitalGain[D_DPDA_CARMAX];								/* デジタルGain						*/
	UINT			localFreq;												/* ローカル周波数					*/
	INT				attSetting;												/* ATT設定							*/
}T_DPDA_DBG_STATUS_INFO;
/** @} */

/******************************************************************************************************************************/
/** TaskInfoテーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT						highSpeedIrqUnmask;							/* 高速モード割り込みマスクカウンタ	*/
	USHORT						almMaskFlg;									/* アラームマスクフラグ				*/
	USHORT						vswrMeasCnt[D_DPDA_ANTMAX];					/* VSWR測定カウンタ					*/
	USHORT						lutInitFlg[D_DPDA_ANTMAX];					/* LUT初期化フラグ					*/
}T_DPDA_CNT;

/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT						temperature;								/* 温度補償実施フラグ				*/
	USHORT						dbgmode;									/* TPデバッグ用						*/
/* SRE対応 - Start */
	USHORT						fbPowerFlg;									/* FB電力計算処理フラグ				*/
	USHORT						txApcFlg;									/* 送信APC実施制御フラグ			*/
/* SRE対応 - End */
	USHORT						txpower_detect_check;						/* TX power deteceted check			*/
	USHORT						txpower_detect_count;						/* TX power deteceted				*/

	USHORT						txBypassStopWaveFlag;						/* TXバイパスモード停波フラグ         */
	USHORT						sfpload_cmp;								/* SFN LOAD comp					*/
	USHORT						qec_count;									/* QEC count						*/
	USHORT						cal_ac_skip_count;							/* Normal Cal skip count			*/
	USHORT						cal_bd_skip_count;							/* Normal Cal skip count			*/
	UINT						cal_error_flg;     			        				/**< Cal異常フラグ	*/
	UINT						txcal_ant_fast_mode_flg[D_RRH_ANT_UNIT_MAX] ;           /**< TxCal高速モードフラグAC	*/
	UINT						rxcal_ant_fast_mode_flg[D_RRH_ANT_UNIT_MAX] ;           /**< RxCal高速モードフラグAC	*/
}T_DPDA_FLG;

/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT						startStop;									/* report start stop				*/
	USHORT						respQid;									/* 応答用QID						*/
}T_DPDA_SUBSC_INFO;

/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT						virtualRegisterFlg;							/* 擬似レジスタ参照フラグ			*/
	USHORT						debugTime;									/* debug time flg					*/
	USHORT						regLogAllFlag;								/* レジスタ設定値取得要否フラグ		*/
	USHORT						periSkipFlg;								/* 1秒周期処理Skipフラグ			*/
}T_DPDA_SW_DBG_FLG;

/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT						stateNo;									/* 状態番号							*/
	USHORT						lowStateCnt;								/* 低電力状態カウンタ				*/
	USHORT						CarSetFlag;									/* キャリア設定直後フラグ			*/
}T_DPDA_LUT_STATE_INFO;

/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT						taskState;									/* Task State						*/
	T_DPDA_CNT					cnt;										/* Counter							*/
	T_DPDA_FLG					flg;										/* Flg								*/
	T_DPDA_SUBSC_INFO			subscInfo[D_DPDA_ANTMAX][D_DPDA_CARMAX];	/* Report Subscribe情報				*/
	T_DPDA_SW_DBG_FLG			swDbgFlg;									/* Debug Flg						*/
	T_DPDA_LUT_STATE_INFO		lutState[D_DPDA_ANTMAX];					/* LUT State						*/
	UINT						txGainCoefNo;								/* TxGain係数						*/
	UINT						fbGainCoefNo[D_DPDA_ANTMAX];				/* fbGain係数						*/
	USHORT						calibState;									/* キャリブレーション状態			*/
}T_DPDA_INFO;
/** @} */

/******************************************************************************************************************************/
/** TPコマンド用情報保持テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT							VswrFwPow;									/* VSWR計算用のFwPow				*/
	UINT							VswrRevPow;									/* VSWR計算用のRevPow				*/
	DL								VswrRL;										/* VSWR計算用のRL					*/
	INT								Vswr;										/* VSWR計算結果						*/
}T_DPDA_INFO_FOR_TP;
/** @} */

/******************************************************************************************************************************/
/** DPDAデバッグログ情報保持テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_DPDA_INFO					taskInfo;										/* dpda task information table		*/
	UINT						almThreshold[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];
	UINT						virtualRegData[D_DPDA_DBG_VIRTUAL_REG_CNT];		/* 仮想レジスタデータ格納テーブル	*/
	UINT						regLogAllFlag;									/* レジスタログ全取得フラグ			*/
	INT							fwAdjGain[D_DPDA_ANTMAX];						/* FW補正用Gain						*/
	INT							prevTempAdjVal;									/* 温度補償RxGain調整値				*/
	UINT						startUpTime;									/* 装置起動時間						*/
	UINT						carModflg;										/* キャリアモードフラグ(tp用)		*/
	UINT						almmsk;											/* アラームマスク用(tp用)			*/
}T_DPDA_INFO_ALL;
/** @} */

/******************************************************************************************************************************/
/** IQ Setting Data Info table
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					numOfAxC;											/**< AxCの数						*/
	UINT					setVal;												/**< 設定値							*/
}T_DPDA_IQ_SETTING_DATA_INFO;
/** @} */

/******************************************************************************************************************************/
/** dpda alarm state table
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	USHORT					almState;							/* アラーム状態					*/
	USHORT					almStateNow;						/* アラーム状態(今回周期)		*/
	USHORT					almOccurNum1sec;					/* アラーム発生回数				*/
	USHORT					almClearNum1sec;					/* アラーム回復回数				*/
	USHORT					almOccurNumTotal;					/* アラーム発生累計				*/
	USHORT					almClearNumTotal;					/* アラーム回復累計				*/
	UINT					almOccurNumTotal1sec;				/* アラーム発生累計(1秒周期)	*/
	UINT					almClearNumTotal1sec;				/* アラーム回復累計(1秒周期)	*/
}T_DPDA_ALM_STATE;
/** @} */

/******************************************************************************************************************************/
/** DPDA Alarm Factor Information
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct
{
	UINT					mergeOperator;						/* アラーム要因マージ演算子		*/
	UINT*					jdgInfo1;							/* 判定情報1					*/
	UINT*					jdgInfo2;							/* 判定情報2					*/
	UINT					jdgOperator;						/* 判定演算子					*/
}T_DPDA_ALM_FACTOR;
/** @} */

/******************************************************************************************************************************/
/** DPDA Alarm Information
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct
{
	CHAR					Name[32];							/* アラーム名称					*/
	T_DPDA_ALM_FACTOR		almFactor[D_DPDA_ALM_FACTOR_NUM];	/* アラーム要因					*/
	USHORT					numOfAlmFactor;						/* アラーム要因数				*/
	USHORT					almOccurNum;						/* アラーム発生回数				*/
	USHORT					almClearNum;						/* アラーム回復回数				*/
	USHORT					noClearFlag;						/* アラーム回復なしフラグ		*/
	UINT					instanceId;							/* Instance ID					*/
}T_DPDA_ALM_INFO;
/** @} */

/******************************************************************************************************************************/
/** Soft側保持情報テーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_DPDA_ANTCARSTATEINFO		softStateInfo;								/* SoftState情報					*/
	T_DPDA_INFO					dpdaInfo;									/* DPDAタスク情報					*/
	T_DPDA_STATUS_INFO			statusInfo;									/* Status Info						*/
	T_DPDA_EXT_STATUS_INFO		statusInfoExt;								/* Status Info (拡張領域)			*/
}T_DPDA_SOFT_INFO;
/** @} */

/******************************************************************************************************************************/
/** Hard側保持情報テーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_DPDA_REGDATA				RegData;									/* レジスタデータ					*/
	T_DPDA_EXTDATA				ExtData;
}T_DPDA_HARD_INFO;
/** @} */

/******************************************************************************************************************************/
/** アラーム解析用ログ情報テーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_RRH_SYSTEM_TIME			time;										/* 時刻情報							*/
	T_DPDA_SOFT_INFO			soft_info;									/* ソフト側保持情報					*/
	T_DPDA_HARD_INFO			hard_info;									/* ハード側保持情報					*/
}T_DPDA_ALM_LOG_INFO;
/** @} */

/******************************************************************************************************************************/
/** アラーム発生時解析用ログテーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT						DataPosi;									/* 次回格納位置						*/
	UINT						almLogNum;									/* 格納済みアラームログ数			*/
	UINT						almThresholdInfo[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];
	T_DPDA_ALM_STATE			almStateInfo[D_DPDA_ALM_NUM];				/* AlmState情報(Alm情報は1面1log)	*/
	T_DPDA_ALM_LOG_INFO			alm_log_info[D_DPDA_ALM_LOG_MAX];			/* アラーム解析用ログ情報			*/
																			/* Alm閾値情報(Alm情報は1面1log)	*/
}T_DPDA_ALM_LOG;



/******************************************************************************************************************************/
/** キャリア状態ログ
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	T_RRH_SYSTEM_TIME			time;										/* 時刻情報							*/
	UINT						event;										/* イベント(1:キャリア設定 2:キャリア解放)	*/
	UINT						antno;										/* アンテナ番号 					*/
	UINT						carrier_no;									/* キャリア番号						*/
	UINT						CarOnOff;									/* キャリアONOFF情報				*/
	UINT						Bandwidth;									/* 帯域幅							*/
}T_DPDA_LOG_CAR_STATE_RECORD;


/******************************************************************************************************************************/
/** キャリア状態ログテーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT							uiWriteIdx;									/**< write index				*/
	UINT							uiCycleflg;									/**< Cyclic flg					*/
	T_DPDA_LOG_CAR_STATE_RECORD		tCarStatRecord[D_DPDA_CARSTATE_LOG_MAX];	/**< キャリア状態ログ			*/
}T_DPDA_LOG_CAR_STATE;





/** @} */

/******************************************************************************************************************************/
/** アラームログ共有メモリ参照用テーブル
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/* CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと	*/
typedef struct{
	UINT						almLogFace;										/**< アラームログ収集面				*/
	UINT						rrhKind;										/**< 装置種別						*/
	UINT						dummy[2];										/**< dummy							*/
	T_DPDA_ALM_LOG				almLog[ D_DPDA_ALM_LOG_FACE ]; 					/**< アラーム発生時解析用ログtable	*/
}T_DPDA_ALM_LOG_DEBUG;
/** @} */

/******************************************************************************************************************************/
/** alm threshold information table(for rom)
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	USHORT					AntNum;								/* アンテナ数								*/
	USHORT					CarNum;								/* キャリア数								*/
	CHAR					Name[32];							/* 閾値名称									*/
	INT						Threshold;							/* 閾値(初期値)								*/
}T_DPDA_ALM_THRESHOLD_INFO;
/** @} */


/******************************************************************************************************************************/
/** alm mask check table(for rom)
 *  @addtogroup RRH_RF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT		almmsk;
	UINT		dbgmode;
}T_DPDA_ALM_MSK_CHECK;
/** @} */

/******************************************************************************************************************************/
/** インパルス係数格納テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	SHORT					real;								/* インパルス係数(実数部)					*/
	SHORT					imag;								/* インパルス係数(虚数部)					*/
}T_DPDA_IMPULSE_COEFFICIENT;
/** @} */

/******************************************************************************************************************************/
/** 電力測定LAT情報テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					latBit;									/* ラッチビット								*/
	UINT					antOffset;								/* アンテナオフセット						*/
}T_DPDA_PERI_POWER_REPORT_LAT;
/** @} */

/******************************************************************************************************************************/
/** レジスタ初期設定情報テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ							*/
}T_DPDA_INIT_REG_SET_TBL;
/** @} */

/******************************************************************************************************************************/
/** レジスタ設定情報テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ							*/
	UINT					regSetNum;								/* 設定回数									*/
	UINT					regOffset;								/* オフセット								*/
}T_DPDA_REG_SET_TBL;
/** @} */

/******************************************************************************************************************************/
/** アラームInstanceID変換情報テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					beforeId;								/* InstanceID(変換前)						*/
	UINT					afterId;								/* InstanceID(変換後)						*/
}T_DPDA_ALM_INSTID_CNVT;
/** @} */

/******************************************************************************************************************************/
/** FFT PointNo Vs RB#0対応テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ							*/
}T_DPDA_FFT_POINTNO_RB0;
/** @} */



/******************************************************************************************************************************/
/** TXANTCAL用送信設定
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData_on;								/* レジスタデータ							*/
	UINT					regData_off;							/* レジスタデータ							*/
}T_DPDA_ANTCAL_SET_TBL;
/** @} */

/******************************************************************************************************************************/
/** 運用中TX/RX ANTCAL用送信設定
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr[D_RRH_ANT_UNIT_MAX];			/* レジスタアドレス							*/
	UINT					regData[D_DPDA_CAL_FREQ_NUM];			/* レジスタデータ							*/
}T_DPDA_ANTCAL_OPE_SEND_TBL;
/** @} */

/******************************************************************************************************************************/
/** 運用中TX/RX ANTCAL用送信設定(高速モード)
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr[D_RRH_ANT_UNIT_MAX];			/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ							*/
}T_DPDA_ANTCAL_FAST_OPE_SEND_TBL;
/** @} */

/******************************************************************************************************************************/
/** TXANTCAL用PATH
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData_ant[D_DPDA_ANTMAX];				/* レジスタデータ						*/
}T_DPDA_ANTCAL_PATHSET_TBL;
/** @} */

/******************************************************************************************************************************/
/** TEMP ADJ用
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	INT					tempVal;
	DL					tempCoef_tx[E_DPDA_RUINDEX_NUM];		/* 3.7G 4.5G 28Gの順 */
	DL					tempCoef_fb[E_DPDA_RUINDEX_NUM];		/* 3.7G 4.5G 28Gの順 */
	DL					tempCoef_rx[E_DPDA_RUINDEX_NUM];		/* 3.7G 4.5G 28Gの順 */
}T_DPDA_TEMP_ADJ_COEF;
/** @} */

/******************************************************************************************************************************/
/** ALM report用
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT	ope_fid;
	UINT	fltid;
	UINT	alm_hislog;
	UINT	alm_mask;
}T_DPDA_ALM_REPORT;
/** @} */


/******************************************************************************************************************************/
/** BeamID固定設定テーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ							*/
}T_DPDA_BEAMID_SET;
/** @} */


/******************************************************************************************************************************/
/** oRXCAL
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData[3];								/* レジスタデータ						*/
}T_DPDA_ORXCAL_TBL;
/** @} */

/******************************************************************************************************************************/
/** CALINFO
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					antNum;										/* 偏波毎のANT数						*/
	UINT					antNo[D_RRH_ANT_MAX];						/* 偏波毎のANT番号						*/
}T_DPDA_CAL_POLARIZATION_INFO;
/** @} */
/******************************************************************************************************************************/
/** QEC
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					regAddr;								/* レジスタアドレス							*/
	UINT					regData;								/* レジスタデータ						*/
}T_DPDA_QEC_TBL;
/** @} */

/******************************************************************************************************************************/
/** 運用中CALのPHASE計算に使用する値のテーブル
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	DL					CALCOS;								/* レジスタアドレス							*/
	DL					CALSIN;								/* レジスタデータ						*/
	DL					AMPREFSUM;							/* REF									*/
	DL					AMPFBSUM;							/* FB									*/
}T_DPDA_OPECAL_PHASE_VALUE_TBL;
/** @} */


#endif
/* @} */
