/*!
* @skip $Id:$
* @file f_frwsla_def.h
* @brief frwsla task Define Header
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
*/
/** @addtogroup RRH_PF_FRWSLA
 *  @{
 */
#ifndef F_FRWSLA_DEF_H
#define F_FRWSLA_DEF_H

/************************************************************************************************************/
/* MAXIMUM constant definition                                                                              */
/************************************************************************************************************/
/*!
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 * @{
 */
#define D_FRWSLA_STATMAX                  2                    /*!< Maximum task state number                  */
#define D_FRWSLA_EVTMAX                	  3                    /*!< Maximum event number for frwsla thread */
#define D_FRWSLA_MSGLENMAX                512                  /*!< Maximum length of msg received by FRW task */
/* @} */

/*!
 * @name Event index
 * @note Event index definition used in f_frwr_matrix_tbl
 * @{
 */
#define D_FRWSLA_EVTIDX_FLSHERASE_REQ			0
#define D_FRWSLA_EVTIDX_FLSHWRITE_REQ			1
#define D_FRWSLA_EVTIDX_FLSHREAD_REQ			2
/* @} */

/*!
 * @name EEPROM index 
 * @note EEPROM index  definition
 * @{
 */
#define D_FRWSLA_THRDSTA_INIT			0			/* Initial 					*/
#define D_FRWSLA_THRDSTA_RUN			1			/* Runing 				*/
/* @} */

/** @name running history log		*/
/* @{ */
#define	D_FRWSLA_RUNHIS_BODYSIZE_MAX	48		/**< ボディサイズMAX */
#define	D_FRWSLA_RUNHIS_FACENUM_MAX		1024	/**< 面数MAX */
#define	D_FRWSLA_RUNHIS_FLG_RCV			0x00	/**<  送受信フラグ(受信)   */
#define	D_FRWSLA_RUNHIS_FLG_SND			0x01	/**<  送受信フラグ(送信)   */
#define	D_FRWSLA_RUNHIS_FLG_SND_NG		0xff	/**<  送受信フラグ(送信)(NG)   */
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
}T_FRWSLA_RUNHIS_HEAD;

/**
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 * 
 */
typedef struct
{
	T_FRWSLA_RUNHIS_HEAD	head;
	CHAR 					body[40];	
}T_FRWSLA_RUNHIS_INF;

/**
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 * 
 */
typedef struct
{
	UINT				write_idx;	
	T_FRWSLA_RUNHIS_INF	inf[D_FRWSLA_RUNHIS_FACENUM_MAX];
										/**< 情報         */
}T_FRWSLA_RUNHIS_TBL;

#endif
/** @} */
