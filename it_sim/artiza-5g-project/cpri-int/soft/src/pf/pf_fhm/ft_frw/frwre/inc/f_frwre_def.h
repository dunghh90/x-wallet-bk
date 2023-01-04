/*!
* @skip $Id:$
* @file f_frwre_def.h
* @brief frw task Define Header
* @date 2015/08/04 TDIPS)sasaki create
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/
/** @addtogroup RRH_PF_FRWRE
 *  @{
 */
#ifndef F_FRWRE_DEF_H
#define F_FRWRE_DEF_H


/************************************************************************************************************/
/* MAXIMUM constant definition                                                                              */
/************************************************************************************************************/
/*!
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 * @{
 */
#define D_FRWRE_STATMAX					2			/*!< Maximum task state number							*/
#define D_FRWRE_EVTMAX					3			/*!< Maximum event number for frwre thread				*/
#define D_FRWRE_MSGLENMAX				512			/*!< Maximum length of msg received by FRWRE task		*/
/* @} */


/*!
 * @name Event index
 * @note Event index definition used in f_frwrer_matrix_tbl
 * @{
 */
#define D_FRWRE_EVTIDX_FLSHERASE_REQ	0
#define D_FRWRE_EVTIDX_FLSHWRITE_REQ	1
#define D_FRWRE_EVTIDX_FLSHREAD_REQ		2
/* @} */

/*!
 * @name EEPROM index 
 * @note EEPROM index  definition
 * @{
 */
#define D_FRWRE_THRDSTA_INIT			0			/* Initial 					*/
#define D_FRWRE_THRDSTA_RUN				1			/* Runing					*/
/* @} */


/** @name running history log			*/
/* @{ */
#define	D_FRWRE_RUNHIS_BODYSIZE_MAX		48			/**< ボディサイズMAX		*/
#define	D_FRWRE_RUNHIS_FACENUM_MAX		1024		/**< 面数MAX				*/
#define	D_FRWRE_RUNHIS_FLG_RCV			0x00		/**< 送受信フラグ(受信)		*/
#define	D_FRWRE_RUNHIS_FLG_SND			0x01		/**< 送受信フラグ(送信)		*/
#define	D_FRWRE_RUNHIS_FLG_SND_NG		0xff		/**< 送受信フラグ(送信)(NG)	*/
/* @} */

/*!
 * @name RE-FLASH READ SIZE
 * @note RE-FLASH Read Block size
 * @{
 */
#define D_FRWRE_BLOCK_SIZE_READ			4096
/* @} */



/**
 * @brief	running history
 * @note	Contents:running history	\n
 * 
 */
typedef struct
{
	UCHAR			setFlg;		
	UCHAR			month;	
	UCHAR			day;	
	UCHAR			hour;	
	UCHAR			min;	
	UCHAR			sec;
	UCHAR			msec;	
	UCHAR			sndrcv_flg;	
	UINT			event_id;	
	USHORT			task_id;	
	USHORT			sig_kind;	
}T_FRWRE_RUNHIS_HEAD;

/**
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 * 
 */
typedef struct
{
	T_FRWRE_RUNHIS_HEAD		head;
	CHAR 					body[40];	
}T_FRWRE_RUNHIS_INF;

/**
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 * 
 */
typedef struct
{
	UINT				write_idx;	
	T_FRWRE_RUNHIS_INF	inf[D_FRWRE_RUNHIS_FACENUM_MAX];
										/**< 情報         */
}T_FRWRE_RUNHIS_TBL;



#endif
/** @} */
