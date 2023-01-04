/*!
* @skip $Id:$
* @file f_fmng_typ.h
* @brief FMNG task type header
* @date 2008/09/8 FFCS)limin create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
*/

/** @addtogroup RRH_PF_FMNG
 * @{
 */

#ifndef F_FMNG_TYP_H
#define F_FMNG_TYP_H

/**
 * @brief	running history
 * @note	Contents:running history	\n
 * 
 */
typedef struct
{
	UCHAR			setFlg;		/* ダミー1         	*/
	UCHAR			month;		/**< 月              */
	UCHAR			day;		/**< 日              */
	UCHAR			hour;		/**< 時              */
	UCHAR			min;		/**< 分              */
	UCHAR			sec;		/**< 秒              */
	UCHAR			msec;		/**< ミリ秒          */
	UCHAR			sndrcv_flg;	/**< 送信/受信フラグ */
	UINT			event_id;	/**< イベントID      */
	USHORT			task_id;	/**< タスクID        */
	USHORT			sig_kind;	/**< 信号種別        */
}T_FMNG_RUNHIS_HEAD;

/**
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 * 
 */
typedef struct
{
	T_FMNG_RUNHIS_HEAD	head;	/**< ヘッダ */
	CHAR 				body[40];	
}T_FMNG_RUNHIS_INF;

/**
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 * 
 */
typedef struct
{
	UINT				write_idx;	
	T_FMNG_RUNHIS_INF	inf[D_FMNG_RUNHIS_FACENUM_MAX];
									
}T_FMNG_RUNHIS_TBL;

/**
 * @brief   FLDC4 header
 */
typedef struct
{
	UCHAR				judge_str[D_SYS_NUM16];				/**< 識別コード			*/
	UINT				fldc4_size;							/**< 圧縮サイズ			*/
	UINT				dummy[D_SYS_NUM3];					/**< ダミー				*/
}T_FMNG_FLDC4HEADER;

#endif
/** @} */
