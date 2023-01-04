/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_nego_typ.h
 *  @brief  pf_nego thread common structure head file defination
 *  @date   2015/04/22 FFCS)fuxg create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/

#ifndef		F_NEGO_TYP_H
#define		F_NEGO_TYP_H

/** @addtogroup RRH_PF_NEGO
* @{ */

/** 走行履歴情報ヘッダ */
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
}T_NEGO_RUNHIS_HEAD;

/** 走行履歴情報 */
typedef struct
{
	T_NEGO_RUNHIS_HEAD	head;	/* ヘッダ */
	UINT body[2];
}T_NEGO_RUNHIS_INF;

/** 走行履歴情報テーブル */
typedef struct
{
	UINT				write_idx;						/* 書込位置     */
	T_NEGO_RUNHIS_INF 	inf[D_NEGO_RUNHIS_FACENUM_MAX];
}T_NEGO_RUNHIS_TBL;


/* @}  */
#endif

