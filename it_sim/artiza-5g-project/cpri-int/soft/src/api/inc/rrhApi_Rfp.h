/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfp.h
 * @brief API RF INC定義
 * @date  2013/11/22 ALPHA)ueda Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_API_RFP
 * @{
 */
#ifndef RRHAPI_RFP_H
#define RRHAPI_RFP_H

#include "rrhApi_Com.h"

/********************************************************************************************************************/
/* API RFP 構造体定義                                                                                               */
/********************************************************************************************************************/
/********************************************************************************************************************/
/*!
 * @name (0xA0010001)SystemParameter更新要求
 * @note (0xA0010001)SystemParameter更新要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT sec_chip[D_RRH_ANT_MAX];			/* セクタnチップ補正(0-16)(4branch)										*/
	USHORT bandwidth[D_RRH_CAR_MAX];		/* bandwidth(5000:5M, 10000:10M, 15000:15M, 20000:20M)(2Carrier)		*/
	USHORT tx_sys_num;						/* 送信系統数(2 or 4)													*/
	USHORT cpri_rate;						/* CPRI Rate(1:4.9, 2:9.8)												*/
}T_API_RFP_SYSPARA_SET_INFO;
 	
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_SYSPARA_SET_INFO sysInfo;		/**< SystemParameter更新要求情報 										*/
}T_API_RFP_SYS_PARA_UPDATE_REQ;
/* @} */
 	
/********************************************************************************************************************/
/*!
 * @name (0xA0010003)Carrier設定要求
 * @note (0xA0010003)Carrier設定要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1, 2:branch2, 3:branch3)					*/
	USHORT carrier;							/* キャリア番号(0,1) 													*/
	USHORT bandwidth;						/* bandwidth(5000:5M, 10000:10M, 15000:15M, 20000:20M) 					*/
	USHORT frequency;						/* 周波数番号(3.5G: 42415-42765) 										*/
	USHORT tddUlDlConfig;					/* TDD uplink/downlink configuration (0～6)								*/
	USHORT tddSpeSubframeConfig;			/* TDD special subframe configuration(0～9)								*/
	USHORT tddDlCyclicPrefixLen;			/* TDD downlink cyclic prefix length (0～1)								*/
}T_API_RFP_CAR_SET_INFO;
 
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_CAR_SET_INFO carInfo;			/**< Carrier設定要求情報 												*/
}T_API_RFP_CAR_SET_REQ;
/* @} */
 	
/********************************************************************************************************************/
/*!
 * @name (0xA0010004)Carrier設定応答
 * @note (0xA0010004)Carrier設定応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	UINT result;							/**< Carrier設定応答情報 												*/
}T_API_RFP_CAR_SET_RSP;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010005)Carrier解放要求
 * @note (0xA0010005)Carrier解放要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT carrierId;						/* キャリア番号(0,1) 													*/
	USHORT branch;							/* branch(0:branch0, 1:branch1, 2:branch2, 3:branch3) 					*/
}T_API_RFP_CAR_REL_INFO;

typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_CAR_REL_INFO carInfo;			/**< Carrier解放要求情報 												*/
}T_API_RFP_CAR_REL_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010007)無変調キャリア制御要求
 * @note (0xA0010007)無変調キャリア制御要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* 0-1[Ant#A,B] 														*/
	USHORT carrier;							/* キャリア番号(0) 														*/
	USHORT onoff;							/* 0:off, 1:on 															*/
	USHORT power;							/* 0[dB] LTE: 458-660 													*/
}T_API_RFP_NO_MOD_INFO;
 
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ														*/
	T_API_RFP_NO_MOD_INFO noModInfo;		/**< 無変調キャリア制御要求 											*/
}T_API_RFP_NO_MOD_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010008)無変調キャリア制御応答
 * @note (0xA0010008)無変調キャリア制御応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	UINT result;							/**< 無変調キャリア制御応答情報 										*/
}T_API_RFP_NO_MOD_RSP;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010009)PA制御要求
 * @note (0xA0010009)PA制御要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1) 										*/
	USHORT paOnOff;							/* 0:off, 1:on 															*/
}T_API_RFP_PA_INFO;
 
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_PA_INFO paInfo;				/**< PA制御要求情報 													*/
}T_API_RFP_PA_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA001000A)PA制御応答
 * @note (0xA001000A)PA制御応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	UINT result;							/**< PA制御結果(0:正常) 												*/
}T_API_RFP_PA_RSP;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA001000B)停波制御要求
 * @note (0xA001000B)停波制御要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1) 										*/
	USHORT waveOnOff;						/* 0:off, 1:on 															*/
}T_API_RFP_WAVE_INFO;

typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_WAVE_INFO waveInfo;			/**< 停波制御要求情報 													*/
}T_API_RFP_WAVE_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA00100C)停波制御応答
 * @note (0xA00100C)停波制御応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	USHORT result;							/**< 停波制御結果(0:正常) 												*/
}T_API_RFP_WAVE_RSP;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA001000D)IQ制御要求
 * @note (0xA001000D)IQ制御要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT cpriPort;						/* 0固定 																*/
	USHORT cpriOnOff;						/* 0:off, 1:on															*/
}T_API_RFP_IQ_CTRL_INFO;

typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_IQ_CTRL_INFO iqInfo;			/**< IQ制御要求情報 													*/
}T_API_RFP_IQ_CTRL_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010011)送信出力調整要求
 * @note (0xA0010011)送信出力調整要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1) 										*/
	USHORT command;							/* 0:UP, 1:DOWN 														*/
}T_API_RFP_POW_ADJ_INFO;

typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
	T_API_RFP_POW_ADJ_INFO powAdjInfo;		/**< 送信出力調整要求情報 												*/
}T_API_RFP_POW_ADJ_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010013)送信出力保存要求
 * @note (0xA0010013)送信出力保存要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD head;						/**< 装置共通ヘッダ 													*/
}T_API_RFP_POW_SAVE_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010015)周波数関連情報取得要求
 * @note (0xA0010015)周波数関連情報取得要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT getKind;										/* 取得種別													*/
	USHORT branch;										/* ブランチ番号(0/1)										*/
	USHORT carrier;										/* キャリア番号(0) 											*/
}T_API_RFP_GET_CAR_FREQ_REQ_INFO;

typedef struct{
	T_RRH_HEAD head;									/**< 装置共通ヘッダ 										*/
	T_API_RFP_GET_CAR_FREQ_REQ_INFO carFreqReqInfo;		/**< 周波数関連情報取得要求情報 							*/
}T_API_RFP_GET_CAR_FREQ_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010016)周波数関連情報取得応答
 * @note (0xA0010016)周波数関連情報取得応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UINT frequency;										/* 周波数番号												*/
	UINT power;											/* 無変調電力											 	*/
	UINT carState;										/* carrier state											*/
}T_API_RFP_GET_CAR_FREQ_RSP_INFO;

typedef struct{
	T_RRH_HEAD head;									/**< 装置共通ヘッダ */
	T_API_RFP_GET_CAR_FREQ_RSP_INFO carFreqRspInfo;		/**< 周波数関連情報取得応答情報 							*/
}T_API_RFP_GET_CAR_FREQ_RSP;

/********************************************************************************************************************/
/*!
 * @name (0xA001001D)キャリア電力閾値設定
 * @note (0xA001001D)キャリア電力閾値設定
 * @{
 */
/********************************************************************************************************************/
typedef struct{

	USHORT power;										/* 設定電力													*/
}T_API_RF_CAR_POW_THRESH_SET_INFO;

typedef struct{
	T_RRH_HEAD head;									/**< 装置共通ヘッダ 										*/
	T_API_RF_CAR_POW_THRESH_SET_INFO 	powInfo;		/**< 送信出力調整要求情報 									*/
}T_API_RF_CAR_POW_THRESH_SET;
/* @} */


/********************************************************************************************************************/
/*!
 * @name (0xA001001F)TestMode設定要求
 * @note (0xA001001F)TestMode設定要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1) 										*/
	USHORT onoff;							/* onoff(0:OFF, 1:ON) 													*/
}T_API_RF_TEST_MODE_SET_INFO;

typedef struct{
	T_RRH_HEAD head;								/**< 装置共通ヘッダ 											*/
	T_API_RF_TEST_MODE_SET_INFO testModeInfo;		/**< TestMode設定情報 											*/
}T_API_RF_TEST_MODE_SET;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010021)TestMode状態取得要求
 * @note (0xA0010021)TestMode状態取得要求
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	USHORT branch;							/* branch(0:branch0, 1:branch1) 										*/
}T_API_RF_TEST_MODE_GET_REQ_INFO;

typedef struct{
	T_RRH_HEAD head;									/**< 装置共通ヘッダ 										*/
	T_API_RF_TEST_MODE_GET_REQ_INFO testModeReqInfo;	/**< TestMode状態取得情報									*/
}T_API_RF_TEST_MODE_GET_REQ;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0010021)TestMode状態取得応答
 * @note (0xA0010021)TestMode状態取得応答
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UINT testModeStatus;				/* TestMode状態(0:OFF, 1:ON)												*/
}T_API_RF_TEST_MODE_GET_RSP_INFO;

typedef struct{
	T_RRH_HEAD head;									/**< 装置共通ヘッダ 										*/
	T_API_RF_TEST_MODE_GET_RSP_INFO testModeRspInfo;	/**< TestMode状態取得情報 									*/
}T_API_RF_TEST_MODE_GET_RSP;

/********************************************************************************************************************/
/*!
 * @name frequency check table
 * @note frequency check table definision
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UINT					freqMin;							/* frequency(min)				*/
	UINT					freqMax;							/* frequency(max)				*/
}T_API_FREQ_CHK_INFO;
/** @} */

/********************************************************************************************************************/
/*!
 * @name bandwidth check table
 * @note bandwidth check table definision
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UINT					bwNum;								/* bandwidth数					*/
	UINT					bwVal[D_RRH_BW_NUM];				/* bandwidth					*/
}T_API_BW_CHK_INFO;
/** @} */

/********************************************************************************************************************/
/* プロトタイプ宣言                                                                                                 */
/********************************************************************************************************************/
/* 5GDU_chg */
E_RRHAPI_RCODE rrhApi_Rfa_Mnt_UpdateSystemParam(INT qid, INT wtime, VOID *data_p, USHORT dlul_flag, USHORT ant, USHORT carrier, UINT frequency, USHORT acell_id, USHORT cc_id, USHORT port_id, USHORT car_gain, USHORT car_delay);
extern E_RRHAPI_RCODE rrhApi_Rfa_Mqr_CarSet(int qid, unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiAccellId, unsigned int uiCcId, unsigned int uiDuPortId, unsigned int uiCrCentFreq, unsigned int uiBw, unsigned int uiTxRx, unsigned int uiTestFlg, unsigned int uiScs);
extern E_RRHAPI_RCODE rrhApi_Rfa_Mnt_CarRel(int qid, unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiCrCentFreq, unsigned int uiTxRx, unsigned int uiTestFlg);
E_RRHAPI_RCODE rrhApi_Rfa_com_CarrierCheck(USHORT carrier);
E_RRHAPI_RCODE rrhApi_Rfa_com_BranchCheck(USHORT branch);
E_RRHAPI_RCODE rrhApi_Rfa_Mqr_UseCal(INT qid, INT wtime, VOID *data_p, USHORT txrx_flag);
E_RRHAPI_RCODE rrhApi_Rfa_Mqr_TrainingCal(INT qid, INT wtime, VOID *data_p, USHORT txrx_flag);
E_RRHAPI_RCODE rrhApi_Cca_Mnt_PowerAdjust( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT command );

E_RRHAPI_RCODE rrhApi_Rfp_Mqr_NoModCar( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT carrier, USHORT onoff, USHORT power );
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_General_NoModCar(INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT carrier, USHORT onoff, USHORT power, USHORT sig_kind);
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_PaControl( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT onoff );
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_WaveControl( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT onoff );
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_IQControl( INT qid, INT wtime, VOID *data_p, USHORT cpriPort, USHORT onoff );
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_AllPaCarOff( INT qid, INT wtime, VOID *data_p );
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_PowerSave( INT qid, INT wtime, VOID *data_p );
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_GetCarFreq( INT qid, INT wtime, VOID *data_p, USHORT getKind, USHORT branch, USHORT carrier );
UINT		   rrhApi_Rfp_com_BothCheck(USHORT branch);
/* 5GDU_chg */

/********************************************************************************************************************/
/* 定数定義(enum)                                                                                                    */
/********************************************************************************************************************/
/*!
 * @name Carrier Setting result code
 * @note Carrier Setting result code
 * @{
 */
enum {
	E_API_RFP_RC_CARSET_OK = 0,							/*!< OK														*/
	E_API_RFP_RC_CARSET_CARID_OUT_OF_RANGE,				/*!< carier id out of range									*/
	E_API_RFP_RC_CARSET_BRANCH_OUT_OF_RANGE,			/*!< branch out of range									*/
	E_API_RFP_RC_CARSET_FREQ_OUT_OF_RANGE,				/*!< frequency out of range									*/
	E_API_RFP_RC_CARSET_BW_OUT_OF_RANGE,				/*!< bandwidth out of range									*/
	E_API_RFP_RC_CARSET_ALREADY_ON,						/*!< already on												*/
	E_API_RFP_RC_CARSET_MSG_ORDER_UNMATCH,				/*!< message order unmatch									*/
	E_API_RFP_RC_CARSET_NUM								/*!< num of result code										*/
};
/* @} */

/*!
 * @name PA Control result code
 * @note PA Control result code definition
 * @{
 */
enum {
	E_API_RFP_RC_PACTRL_OK = 0,							/*!< OK														*/
	E_API_RFP_RC_PACTRL_PARAM_OUT_OF_RANGE,				/*!< parameter out of ranges								*/
	E_API_RFP_RC_PACTRL_NUM								/*!< num of result code										*/
};
/* @} */

/*!
 * @name Wave Control result code
 * @note Wave Control result code definition
 * @{
 */
enum {
	E_API_RFP_RC_WAVECTRL_OK = 0,						/*!< OK														*/
	E_API_RFP_RC_WAVECTRL_PARAM_OUT_OF_RANGE,			/*!< parameter out of ranges								*/
	E_API_RFP_RC_WAVECTRL_NUM							/*!< num of result code										*/
};
/* @} */

/*!
 * @name No Modulation Carrier result code
 * @note No Modulation Carrier result code definition
 * @{
 */
enum {
	E_API_RFP_RC_NOMOD_OK = 0,							/*!< OK														*/
	E_API_RFP_RC_NOMOD_PARAM_OUT_OF_RANGE,				/*!< parameter out of ranges								*/
	E_API_RFP_RC_NOMOD_CAR_PTN_NG,						/*!< carreir pattern NG										*/
	E_API_RFP_RC_NOMOD_NUM								/*!< num of result code										*/
};
/* @} */

/*!
 * @name Carrier Information Carrier kind
 * @note Carrier Information Carrier kind definition
 * @{
 */
enum {
	E_API_RFP_CARRIER_KIND_LTE = 0,							/*!< Carrier Kind : LTE									*/
	E_API_RFP_CARRIER_KIND_3G,								/*!< Carrier Kind : 3G									*/
	E_API_RFP_CARRIER_KIND_MAX
};
/* @} */

/*!
 * @name Branch Number 0 and 1
 * @note Branch Number 0 and 1 definition
 * @{
 */
enum {
	E_API_RFP_NUM_BRANCH_0 = 0,							/*!< Branch number 0										*/
	E_API_RFP_NUM_BRANCH_1,								/*!< Branch number 1										*/
	E_API_RFP_NUM_BRANCH_2,								/*!< Branch number 2										*/
	E_API_RFP_NUM_BRANCH_3,								/*!< Branch number 3										*/
	E_API_RFP_NUM_BRANCH_MAX = E_API_RFP_NUM_BRANCH_3,	/*!< Branch number MAX										*/ /* 5GDU_chg */
	E_API_RFP_NUM_BRANCH_ALL = 0xFF						/*!< Branch number ALL										*/
};

#define D_API_RFP_NUM_BRANCH_RX_ONLY		0xFF		/*!< Branch Rx Only											*/


/* @} */

/*!
 * @name Control Off and On
 * @note Control Off and On definition
 * @{
 */
enum {
	E_API_RFP_CONTROL_OFF = 0,							/*!< Control Off											*/
	E_API_RFP_CONTROL_ON								/*!< Control On												*/
};
/* @} */
 	
/*!
 * @name Power Adjust command
 * @note Power Adjust command definition
 * @{
 */
enum {
	E_API_RFP_POWER_ADJUST_UP = 0,						/*!< Power Adjust Up										*/
	E_API_RFP_POWER_ADJUST_DOWN							/*!< Power Adjust Down										*/
};
/* @} */

/*!
 * @name Branch Number 0 and 1
 * @note Branch Number 0 and 1 definition
 * @{
 */
enum {
	E_API_RFP_NUM_SENDBRANCH_0 = 1,							/*!< Send Branch number 0								*/
	E_API_RFP_NUM_SENDBRANCH_1,								/*!< Send Branch number 1								*/
	E_API_RFP_NUM_SENDBRANCH_BOTH							/*!< Send Branch number BOTH							*/
};
/* @} */

/*!
 * @name No Modulation Carrier send power kind
 * @note No Modulation Carrier send power kind definition
 * @{
 */
#define D_API_RFP_NOMOD_SEND_POWER_LTE_MIN	458			/*!< Send power kind LTE(Max)								*/
#define D_API_RFP_NOMOD_SEND_POWER_LTE_MAX	660			/*!< Send power kind LTE(Min)								*/

#define D_API_RFP_NOMOD_SEND_POWER_3G_MIN	350			/*!< Send power kind 3G(Max)								*/
#define D_API_RFP_NOMOD_SEND_POWER_3G_MAX	630			/*!< Send power kind 3G(Min)								*/
/* @} */

/*!
 * @name Get Carrier Freq RSP carrier state
 * @note Get Carrier Freq RSP carrier state definition
 * @{
 */
#define D_API_RFP_GET_CAR_FREQ_CAR_STS_CAR		0x00000001	/*!< Carrier ON State									*/
#define D_API_RFP_GET_CAR_FREQ_CAR_STS_NOMOD	0x00000002	/*!< No Modulation ON State								*/
/* @} */

/********************************************************************************************************************/
/* 定数定義(define)                                                                                                 */
/********************************************************************************************************************/
#define D_API_RFP_BANDWIDTH_NUM_MIN 			0		/*!< Bandwidth number min									*/
#define D_API_RFP_BANDWIDTH_NUM_MAX 			4		/*!< Bandwidth number max									*/
#define D_API_RFP_CPRI_PORT_NUMBER 				0		/*!< CPRI Port number										*/
#define D_API_RFP_CARRIER_NO_MIN 				1		/*!< carrier number min										*/
#define D_API_RFP_CARRIER_NO_MAX 				8		/*!< carrier number man										*/ /* 5GDU_chg */
#define D_API_RFP_CARRIER_NO_3G_MAX 			4		/*!< carrier number max(3G)									*/
#define D_API_RFP_BW_NUM_15G_TRI 				3		/*!< num of bandwidth(1.5G 3sector)							*/
#define D_API_RFP_BW_NUM_17G_TRI 				4		/*!< num of bandwidth(1.7G 3sector)							*/
#define D_API_RFP_BW_NUM_700M	 				2		/*!< num of bandwidth(700M)									*/
#define D_API_RFP_BW_NUM_SRE	 				3		/*!< num of bandwidth(SRE)									*/
#define D_API_RFP_BW_NUM_SLC_2G	 				5		/*!< num of bandwidth SLC(2G)								*/
#define D_API_RFP_BW_NUM_SLC_800M				5		/*!< num of bandwidth SLC(800M)								*/
#define D_API_RFP_BW_NUM_1CHP_15G				4		/*!< num of bandwidth 1Chip(1.5G)							*/
#define D_API_RFP_BW_NUM_1CHP_17G				7		/*!< num of bandwidth 1Chip(1.7G)							*/
#define D_API_RFP_BW_NUM_35G_TDD				1		/*!< num of bandwidth TDD(3.5G)								*/
#define D_API_RFP_SEC_NCHIP_MAX 				16		/*!< sector n-chip max										*/
#define D_API_RFP_BRANCH_NOBOTH					0		/*!< BOTHを非許容											*/
#define D_API_RFP_BRANCH_BOTH					1		/*!< BOTHを許容												*/
#define D_API_RFP_BOTH_BREAK_OFF				0		/*!< SLC/1ChpのBOTH指定でない								*/
#define D_API_RFP_BOTH_BREAK_ON					1		/*!< SLC/1ChpのBOTH指定										*/
#define D_API_RFP_SENDBRANCH_CDMA				3		/*!< SendBranch3G用固定値									*/
#define D_API_RFP_TX_NUM_2						2		/*!< 送信系統数(2)											*/
#define D_API_RFP_TX_NUM_4						4		/*!< 送信系統数(4)											*/
#define D_API_RFP_CPRI_RATE_MIN					1		/*!< CPRIrate  (1:4.9G)										*/
#define D_API_RFP_CPRI_RATE_MAX					2		/*!< CPRIrate  (2:9.8G)										*/
#define D_API_RFP_TDD_ULDLCONFIG_MIN			0		/*!< TDD uplink/downlink configuration min					*/
#define D_API_RFP_TDD_ULDLCONFIG_MAX			6		/*!< TDD uplink/downlink configuration max					*/
#define D_API_RFP_TDD_SUBFRAME_CONFIG_MIN		0		/*!< TDD special subframe configuration min					*/
#define D_API_RFP_TDD_SUBFRAME_CONFIG_MAX		9		/*!< TDD special subframe configuration max					*/
#define D_API_RFP_TDD_DL_CYCLIC_PREFIX_MIN		0		/*!< TDD downlink cyclic prefix length min					*/
#define D_API_RFP_TDD_DL_CYCLIC_PREFIX_MAX		1		/*!< TDD downlink cyclic prefix length max					*/


/********************************************************************************************************************/
/* API RFP EventID																									*/
/********************************************************************************************************************/
#define D_API_RFP_MNT_UPDATE_SYSTEM_PARAM_REQ		0xA0010001						/* SystemParamter更新要求		*/
#define D_API_RFP_MQR_CARRIER_SET_REQ				0xA0010003						/* Carrier設定要求				*/
#define D_API_RFP_MQR_CARRIER_SET_RSP				0xA0010004						/* Carrier設定応答				*/
#define D_API_RFP_MNT_CARRIER_RELEASE_REQ			0xA0010005						/* Carrier解放要求				*/
#define D_API_RFP_MQR_TXRXCAL_INUSE_REQ				0xA0010007						/* 運用中のTxRxCal要求			*/ /* 5GDU_chg */
#define D_API_RFP_MQR_TXRXCAL_TRAINING_REQ			0xA0010009						/* TxCal/RxCalトレーニング要求	*/ /* 5GDU_chg */

//#define D_API_RFP_MQR_NO_MODULATION_CARRIER_REQ	0xA0010007						/* 無変調キャリア制御要求		*/ /* 5GDU_chg */
//#define D_API_RFP_MQR_NO_MODULATION_CARRIER_RSP	0xA0010008						/* 無変調キャリア制御応答		*/ /* 5GDU_chg */
//#define D_API_RFP_MQR_PA_CONTROL_REQ				0xA0010009						/* PA制御要求					*/ /* 5GDU_chg */
//#define D_API_RFP_MQR_PA_CONTROL_RSP				0xA001000A						/* PA制御応答					*/ /* 5GDU_chg */
//#define D_API_RFP_MQR_WAVE_CONTROL_REQ			0xA001000B						/* 停波制御要求					*/ /* 5GDU_chg */
//#define D_API_RFP_MQR_WAVE_CONTROL_RSP			0xA001000C						/* 停波制御応答					*/ /* 5GDU_chg */
#define D_API_RFP_MNT_IQ_CONTROL_REQ				0xA001000D						/* IQ制御要求					*/
#define D_API_RFP_MNT_ALL_PA_CARRIER_RELEASE_REQ	0xA001000F						/* 全PA/Carrier解放要求			*/
#define D_API_RFP_MNT_POWER_ADJUST_REQ				0xA0010011						/* 送信出力調整要求				*/
#define D_API_RFP_MNT_POWER_SAVE_REQ				0xA0010013						/* 送信出力保存要求				*/
#define D_API_RFP_MQR_GET_CARRIER_FREQUENCY_REQ		0xA0010015						/* 周波数関連情報取得要求		*/
#define D_API_RFP_MQR_GET_CARRIER_FREQUENCY_RSP		0xA0010016						/* 周波数関連情報取得応答		*/

#define D_API_RFP_MQR_CARRIER_SET_3G_REQ			0xA0010017						/* Carrier設定要求(3G)			*/
#define D_API_RFP_MQR_CARRIER_SET_3G_RSP			0xA0010018						/* Carrier設定応答(3G)			*/
#define D_API_RFP_MNT_CARRIER_RELEASE_3G_REQ		0xA0010019						/* Carrier解放要求(3G)			*/
#define D_API_RFP_MQR_NO_MODULATION_CARRIER_3G_REQ	0xA001001B						/* 無変調キャリア制御要求(3G)	*/
#define D_API_RFP_MQR_NO_MODULATION_CARRIER_3G_RSP	0xA001001C						/* 無変調キャリア制御応答(3G)	*/
#define D_API_RFP_MNT_CAR_POW_THRESH_SET_REQ		0xA001001D						/* キャリア電力閾値設定			*/
#define D_API_RFP_MNT_SET_TESTMODE_REQ				0xA001001F						/* TestMode設定要求				*/
#define D_API_RFP_MQT_GET_TESTMODE_REQ				0xA0010021						/* TestMode状態取得要求			*/
#define D_API_RFP_MQT_GET_TESTMODE_RSP				0xA0010022						/* TestMode状態取得応答			*/
#endif
/* @} */
