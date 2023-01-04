/*!
* @skip $Id:$
* @file f_nmas_def.h
* @brief nmas task Define Header
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
*/
/** @addtogroup RRH_PF_NMAS
 *  @{
 */
#ifndef F_NMAS_DEF_H
#define F_NMAS_DEF_H

/*!
 * @name Data length 
 * @note Data length definition
 * @{
 */
#define D_NMAS_SWVER_LEN				32
#define D_NMAS_PRODUCTDATE_LEN			10
#define D_NMAS_PRODUCTNUM_LEN			26
#define D_NMAS_PRODUCTNAME_LEN			14
#define D_NMAS_PRODUCTREV_LEN			8
#define D_NMAS_SERIALNUM_LEN			14
#define D_NMAS_CRC1SIZE					56
#define D_NMAS_CHKSUM_OFFSET			0x44
/* @} */


/*!
 * @name Event index
 * @note Event index definition used in f_fmngr_matrix_tbl
 * @{
 */
#define D_NMAS_EVTIDX_LMCRUN_TOUT			0
#define D_NMAS_EVTIDX_LOADUPD_REQ			1
#define D_NMAS_EVTIDX_FLSHERASE_RSP			2
#define D_NMAS_EVTIDX_FLSHWRT_RSP			3
#define D_NMAS_EVTIDX_FLSHERASE_TOUT		4
#define D_NMAS_EVTIDX_FLSHWRT_TOUT			5
#define D_NMAS_EVTIDX_CHGBTSIDE_REQ			6

/* @} */

/************************************************************************************************************/
/* MAXIMUM constant definition                                                                              */
/************************************************************************************************************/
/*!
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 * @{
 */
#define D_NMAS_STATMAX					2                    /*!< Maximum task state number                  */
#define D_NMAS_EVTMAX					3                    /*!< Maximum event number                       */
#define D_NMAS_MSGLENMASX				512                  /*!< Maximum length of msg received by NMAS task */
/* @} */

/*!
 * @name Event index
 * @note Event index definition used in f_NMASr_matrix_tbl
 * @{
 */

#define	D_NMAS_EVTIDX_FLSHFILWR_REQ			1
#define	D_NMAS_EVTIDX_LUTLOGWRITE_NTF		2

/* @} */

/*!
 * @name EEPROM index 
 * @note EEPROM index  definition
 * @{
 */
#define D_NMAS_THRDSTA_INIT				0			/* Initial 					*/
#define D_NMAS_THRDSTA_RUN				1			/* Runing 				*/
/* @} */

/** @name running history log		*/
/* @{ */
#define	D_NMAS_RUNHIS_BODYSIZE_MAX		48		/**< ボディサイズMAX */
#define	D_NMAS_RUNHIS_FACENUM_MAX		1024	/**< 面数MAX */
#define	D_NMAS_RUNHIS_FLG_RCV			0x00	/**<  送受信フラグ(受信)   */
#define	D_NMAS_RUNHIS_FLG_SND			0x01	/**<  送受信フラグ(送信)   */
#define	D_NMAS_RUNHIS_FLG_SND_NG		0xff	/**<  送受信フラグ(送信)(NG)   */
/* @} */

/** @name 圧縮情報読み取りサイズ		*/
/* @{ */
#define	D_NMAS_COMPDATA				0x00010000				/*!< 圧縮情報サイズ						*/
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
}T_NMAS_RUNHIS_HEAD;

/**
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 * 
 */
typedef struct
{
	T_NMAS_RUNHIS_HEAD	head;
	CHAR 				body[40];	
}T_NMAS_RUNHIS_INF;

/**
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 * 
 */
typedef struct
{
	UINT				write_idx;	
	T_NMAS_RUNHIS_INF	inf[D_NMAS_RUNHIS_FACENUM_MAX];
										/**< 情報         */
}T_NMAS_RUNHIS_TBL;

#endif
/** @} */
