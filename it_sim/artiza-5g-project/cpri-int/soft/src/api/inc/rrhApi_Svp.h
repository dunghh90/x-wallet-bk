/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API TILT part)
 *  @note	Struct Definition(RRH API TILT part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/

#include "f_rrh_type.h"
#include "rrhApi_Com.h"

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_SVP_H
#define RRHAPI_SVP_H

/*!
 * @addtogroup RRH_API_SVP
 * @{
 */
#define D_API_MSGID_SVP_BASE						0xA0040000
/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define D_API_MSGID_SVP_STATE_SUB_REQ				0xA0040001
#define D_API_MSGID_SVP_STATE_CHG_NTC				0xA0040002
#define D_API_MSGID_SVP_STATE_UNSUB_IND				0xA0040003
#define D_API_MSGID_SVP_CARD_CONCTL_IND				0xA0040005
#define D_API_MSGID_SVP_LED_SET_IND					0xA0040007
#define D_API_MSGID_SVP_LED_GET_REQ					0xA0040009
#define D_API_MSGID_SVP_LED_GET_RSP					0xA004000A
#define D_API_MSGID_SVP_DATETIME_SET_IND			0xA004000B
#define D_API_MSGID_SVP_PORTSTATE_SUB_REQ			0xA004000D
#define D_API_MSGID_SVP_PORTSTATE_CHG_NTC			0xA004000E
#define D_API_MSGID_SVP_CARD_RESET_IND				0xA004000F
#define D_API_MSGID_SVP_ERRSET_IND					0xA0040011
#define D_API_MSGID_SVP_PORT_CONTRL_IND				0xA0040013
#define D_API_MSGID_SVP_LED_SET2_IND				0xA0040015
#define D_API_MSGID_SVP_ALL_RE_L1RESET_IND          0xA0040017
#define D_API_MSGID_SVP_RE_L1RESET_IND              0xA0040019
#define D_API_MSGID_SVP_SVREG_SET_IND               0xA004001B
#define D_API_MSGID_SVP_SET_WAVE_IND                0xA004001D
#define D_API_MSGID_SVP_SET_AxC_IND                 0xA004001F

#define D_API_SVP_STACHG_ACT		1
#define D_API_SVP_STACHG_USE		2
#define D_API_SVP_STACHG_ERR		4
#define D_API_SVP_STACHG_ALM		5
#define D_API_SVP_STACHG_AMPDISC	6
#define D_API_SVP_STACHG_WARUPEND	9
#define D_API_SVP_STACHG_INS		10
#define D_API_SVP_STACHG_CARR		13

#define D_API_SVP_ALMSTA_SINGLE		1
#define D_API_SVP_ALMSTA_MULTI		2

#define D_API_LCL_RST				0		/**<  ローカルリセット					*/
#define D_API_RMT_RST_COM			1		/**<  リモートリセット(共通部含)		*/
#define D_API_RMT_RST_NOCOM			2		/**<  リモートリセット(共通部含まず)	*/
#define D_API_RMT_RST_RE_ALL		3		/**<  リモートリセット(RE全体)			*/
#define D_API_CPRI_DL_RST			4		/**<  ダウンロード後の自律リセット(共通部含)*/
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	Structure of Set DateTime Request  
 * @note	Structure of Set DateTime Request
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
	T_RRH_SYSTEM_TIME	datetime;			/**< Date time			*/
}T_API_SVP_SETTIME_REQ;

/**
 * @brief	Structure of Get DateTime Request  
 * @note	Structure of Get DateTime Request
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
}T_API_SVP_GETTIME_REQ;

/**
 * @brief	Structure of Get DateTime Response  
 * @note	Structure of Get DateTime Response
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
	T_RRH_DATETIME	datetime;			/**< Date time			*/
}T_API_SVP_GETTIME_RSP;

/**
 * @brief	Structure of Card Reset Request  
 * @note	Structure of Card Reset Request
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
	UINT	rstkind;					/**< Reset Kind			*/
	UINT	rstFact;					/**< Reset Factor			*/
}T_API_SVP_RESET_REQ;

/**
 * @brief	Structure of Error Set Notification  
 * @note	Structure of Error Set Notification
 * @date    2015/09/04 TDI)satou 「write address」の変数を追加。「write data」の型をUSHORT→UINTに変更。
 *                                SREではSV制御レジスタ#3にPFプロセス以外のプロセスから設定する内容が
 *                                すべて含まれていたため、書き込み先のレジスタアドレスを通知する必要がなかった。
 *                                FHMでは、複数のSV制御レジスタに対して設定を行うため、書き込みアドレスが必要。
 *                                また、上位bitにも設定するため、write dataはUINT型である必要がある。
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header;                 /**< Message header		*/
	USHORT		act_flag;				        /**< Action flag			*/
    UINT        writeAdr;                       /**< Write address */
    UINT        writeDat;                       /**< Write data */
	UINT		chkalm_flg;			            /**< Check alarm flag		*/
}T_API_SVP_ERRSET_NTC;

/**
 * @brief   Structure of D_API_MSGID_SVP_SVREG_SET_IND
 * @date    2015/10/23 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                 /**< message header */
    UINT wrAdr;                                 /**< write address  */
    UINT clrBit;                                /**< clear bit      */
    UINT setBit;                                /**< set bit        */
}T_API_SVP_SVSET_NTC;

/**
 * @brief	Structure of LED Set Notification  
 * @note	Structure of LED Set Notification
 * @date    2015/09/02 TDI)satou LED3, CPRI LINK LEDを追加
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header;                 /**< Message header		*/
    UINT        led1Sta;                        /**< LED1 state         */
    UINT        led2Sta;                        /**< LED2 state         */
    UINT        led3Sta;                        /**< LED3 state         */
    UINT        ledCpriAct[D_RRH_CPRINO_NUM];   /**< CPRI LINK LED(ACT) */
    UINT        ledCpriDwn[D_RRH_CPRINO_NUM];   /**< CPRI LINK LED(DWN) */
}T_API_SVP_LEDSET_NTC;

/**
 * @brief	The Request of Card state subscription  
 * @note	The Request of Card state subscription
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
}T_API_SVP_SUBSCARDSTA_REQ;

/**
 * @brief	The cancel of subscribe Card Control Information  
 * @note	The cancel of subscribe Card Control Information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
}T_API_SVP_UNSUBSCARDSTA_IND;

/**
 * @brief   state change information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	USHORT				chgType;
	USHORT				chgSta;
}T_API_SVP_STACHG_INFO;

/**
 * @brief   alarm information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	USHORT				almFactor;
	USHORT				softAlm;
	USHORT              softAlm2;
	USHORT				pllAlm;
	USHORT				cpriAlm;
	USHORT				apdComAlm;
	USHORT				apd0Alm;
	USHORT				apd1Alm;
	USHORT				apd2Alm;
	USHORT				apd3Alm;
	USHORT				dummy;
}T_API_SVP_ALMINFO;

/**
 * @brief   error information
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    USHORT              err[18];
}T_API_SVP_ERRINFO;

/**
 * @brief	The common defintion of test fault request
 * @note	The common defintion of test fault faults request ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	UINT		chgNum;
	T_API_SVP_STACHG_INFO	staChgInfo[8];
	USHORT		brSta[4];
	USHORT		cardCntItem;
	USHORT		cardExtItem;
	USHORT		carrOnOff;
	USHORT		dummy;
	USHORT		warmUpFlag[2];
	T_API_SVP_ALMINFO	almInfo;
	T_API_SVP_ERRINFO	errInfo;
}T_API_SVP_CARDSTA_INFO;

/**
 * @brief	Structure of Card state change notification  
 * @note	Structure of Card state change notification
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; 				/**< Message header		*/
	T_API_SVP_CARDSTA_INFO		cardSta;		/**< Notice message		*/
}T_API_SVP_CARDSTACHG_NTC;

/**
 * @brief	Structure of LED Set Notification  
 * @note	Structure of LED Set Notification
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER	header; /**< Message header		*/
	USHORT		tx_sys_num;			/**< TX Sys number		*/
	USHORT		dummy;
}T_API_SVP_UPDATE_SYSPARA_NTC;

/**
 * @brief   Structure of D_API_MSGID_SVP_ALL_RE_L1RESET_IND
 * @date    2015/10/19 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;
}T_API_SVP_ALLRE_L1RESET_IND;

/**
 * @brief   Structure of D_API_MSGID_SVP_RE_L1RESET_IND
 * @date    2015/10/19 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;
    USHORT link_num;
}T_API_SVP_RE_L1RESET_IND;

/**
 * @brief   Structure of D_API_MSGID_SVP_SET_WAVE_IND
 * @date    2015/10/30 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;     /**< message header           */
    USHORT dis_ldisen;              /**< 下りLTE用停波設定/解除   */
    USHORT mix_lmixen;              /**< 上りLTE用停波設定/解除   */
    USHORT dis_wdisen;              /**< 下りWcdma用停波設定/解除 */
    USHORT mix_wmixen;              /**< 上りWcdma用停波設定/解除 */
}T_API_SVP_SET_WAVE_IND;

/**
 * @brief   Structure of D_API_MSGID_SVP_SET_AxC_IND
 * @date    2015/11/03 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;
    USHORT bruse;
    USHORT axcbw;
    USHORT ltebw;
    USHORT lcnum;
    USHORT wcnum;
    USHORT wcuse;
}T_API_SVP_SET_AxC_IND;

/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetDateTime(	
								INT		qid, 	INT		wtime,VOID *data_p,
								T_RRH_SYSTEM_TIME		datetime );
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_ResetCard(	
								INT		qid, 	INT		wtime,VOID *data_p,
								UINT	rstkind,
								UINT	rstFact);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_ErrSet(		
								INT		qid, 	INT		wtime,VOID *data_p,
								UINT	act_flag,
								UINT    writeAdr, UINT	writeDat,
								USHORT	chkalm_flg);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_SvRegSet(INT qid, UINT wrAdr, UINT clrBit, UINT setBit);
extern E_RRHAPI_RCODE rrhApi_Svp_Mpr_SubscribeCardState(	
								INT		qid, 	INT		wtime,VOID *data_p );
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_UnsubscribeCardState(	
								INT		qid, 	INT		wtime,VOID *data_p );
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_LedSet(		
								INT		qid, 	INT		wtime,VOID *data_p ,
								UINT	led1Sta, 
								UINT	led2Sta,
								UINT    led3Sta,
								UINT    *actLedSta,
								UINT    *dwnLedSta);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_AllReL1Reset(INT qid);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_ReL1Reset(INT qid, USHORT link_num);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetWave(INT qid, USHORT dis_ldisen, USHORT mix_lmixen, USHORT dis_wdisen, USHORT mix_wmixen);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetAxC(INT qid, USHORT bruse, USHORT axcbw, USHORT ltebw, USHORT lcnum, USHORT wcnum, USHORT wcuse);
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

#define D_API_MSGID_SVP_CMD_STPWVMSK_NTC			0xA0040021

#define D_API_MSGID_SVP_DBG_SVCTL_NTC				0xA0040091

/******************************************************************************************************************************/
/** T_API_CMD_STPWVMSK_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Notification of stop wave inhibition by tp command
 * @note   Notification of stop wave inhibition by tp command
 */
/** @{ */

typedef struct{
	T_API_COMMON_HEADER			header;
}T_API_SVP_CMD_STPWVMSK_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_API_DBG_SVCTL_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	sv bus Ctl Ntc for debug
 * @note   sv bus Ctl Ntc for debug
 */
/** @{ */
typedef struct{
	UINT		svOpt;	/*	OPE			*/
	UINT		svCnt;	/*	OFF/ON/CGH	*/
	UINT		svVal;	/*	SET val		*/
}T_API_DBG_SVCTL_DATA;

typedef struct{
	T_API_COMMON_HEADER			header;
	T_API_DBG_SVCTL_DATA		data;
}T_API_SVP_DBG_SVCTL_NTC;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_Inhibit_stpWv_tp(INT	qid, INT	wtime, VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_Svp_Mnt_Dbg_svCtl(INT	qid, INT	wtime, VOID	*data_p, UINT	svOpt, UINT	svCnt, UINT	svVal);


/** @} */
#endif
/** @} */


