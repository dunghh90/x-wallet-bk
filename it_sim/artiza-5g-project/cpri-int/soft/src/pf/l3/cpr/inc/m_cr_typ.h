/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_typ.h
 *  @brief  CPRI management structure define file
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

#ifndef    M_CR_TYP
#define    M_CR_TYP

/** @addtogroup RRH_L3_CPR
* @{ */


/**
 * @brief	global evet code and task internal event code conversion table structure
 * @note	Contents:	global evet code and task internal event code conversion table structure \n
 * 
 */

/* **************************************************************************/
/* table structure define for global evet code and task internal event code conversion */
/* **************************************************************************/
typedef struct
{
    UINT                    gbleventno;        /* global event code */
	UINT			        intereventno;      /* task internal event code */ 
 }CRT_EVENT_CTBL;

/**
 * @brief	CPRI link state structure
 * @note	Contents:	CPRI link state structure \n
 * 
 */


/*CPRI link state structure define */
typedef struct
{
	USHORT		cprista;	     /* CPRI state */
	USHORT		hdlcsta;	     /* HDLC idle/active flag */
	USHORT		stafflg;	     /* stateF flag */
	USHORT		L2abnminfo;	     /* L2 リンク異常情報  */
	USHORT		hcsta;	         /* L3 health check state */
 }CRT_CPRILNKSTA;

/**
 * @brief	CPRI state priority transition table structure
 * @note	Contents:	CPRI state priority transition table structure \n
 * 
 */

/****************************************************************************/
/* CPRI state priority transition table structure define					*/
/****************************************************************************/
typedef struct
{
	USHORT				priority;			/* CPRI state priority			*/
	USHORT				newcprista;			/* new CPRI state priority	*/
}CRT_CPRISTAPRIO;	


/**
 * @brief	CPRI management runhistory table structure
 * @note	Contents:	CPRI management runhistory table structure \n
 * 
 */

/****************************************************************************/
/* 走行履歴情報テーブル														*/
/****************************************************************************/
/* -- 説明 --                                                               */
/* 走行履歴情報サンプル : 走行履歴情報のフィールドの切れ目がわかりやすいよ  */
/*                        数字を表示します。(必ずこの値)                    */
/* 走行履歴情報[   x]   : タスク間イベントの送受信ログです。write_idx-1の場 */
/*                        所が最新ログになります。(write_idxの場所は0クリア */
/*                        されてるはず)                                     */
/*                        各フィールドに格納されているものは以下            */
/*                        A : 月                                            */
/*                        B : 日                                            */
/*                        C : 時                                            */
/*                        D : 分                                            */
/*                        E : 秒                                            */
/*                        F : ミリ秒                                        */
/*                        G : 送信/受信フラグ                               */
/*                        H : イベントID                                    */
/*                        I : タスクID(タイムアウト発生通知は、下2バイトのみ収集) */
/*                        J : 信号種別                                      */
/*                        K : 外部CPRI番号                                  */
/*                        L : 結果コード                                    */
/****************************************************************************/

/* 走行履歴情報ヘッダ */
typedef struct
{
	UCHAR			setFlg;		/* ダミー1         	*/
	UCHAR			month;		/* 月              	*/
	UCHAR			day;		/* 日              	*/
	UCHAR			hour;		/* 時              	*/
	UCHAR			min;		/* 分              	*/
	UCHAR			sec;		/* 秒              	*/
	UCHAR			msec;		/* ミリ秒          	*/
	UCHAR			sndrcvFlg;	/* 送信/受信フラグ 	*/
	UINT			msgId;		/* イベントID      	*/
	USHORT			taskId;		/* タスクID        	*/
	USHORT			sig_kind;	/* 信号種別        	*/
}CRT_RUNHIS_HEAD;

/*!
 * @brief   ヘルスチェック管理型
 */
typedef struct {
	USHORT		re_state;		/*!< RE状態 */
	USHORT		to_count;		/*!< T.Oカウント値 */
} CRT_HC_CTL_TBL;

/*!
 * @brief   ヘルスチェック管理型
 */
typedef struct {
	USHORT	hlc_srtupsts;		/*!< ヘルスチェック起動状態		*/
	USHORT	hlc_eststs;			/*!< ヘルスチェック確立状態		*/
	USHORT	hlc_sndsts;			/*!< ヘルスチェック送信状態		*/
	USHORT	hlc_nores_cnt;		/*!< ヘルスチェック無応答回数	*/
} CRT_HC_CTL_MST_TBL;

/** 走行履歴情報 */
typedef struct
{
	CRT_RUNHIS_HEAD	head;	/* ヘッダ */
	CHAR body[32];
}CRT_RUNHIS_INF;

/** 走行履歴情報テーブル */
typedef struct
{
	UINT			write_idx;			/* 書込位置     */
	CRT_RUNHIS_INF	inf[CRD_RUNHIS_FACENUM_MAX];
}CRT_RUNHIS_TBL;

/* @}  */
#endif/*M_CR_TYP*/


