/*!
 * @skip  $ld:$
 * @file  mnt_com_typ.h
 * @brief common structure definition
 * @date  2013/11/20 FFCS)zhaodx create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_MNT
 */
#ifndef MNT_COM_TYP_H
#define MNT_COM_TYP_H

/*!
 * @brief Tcp server init request.
 */
typedef struct
{
	CMT_TSKIF_HEAD header;
}MNT_TCP_SERVER_INIT_REQ;
/* @} */

/*!
 * @brief Tcp server init response.
 */
typedef struct
{
	CMT_TSKIF_HEAD header;
}MNT_TCP_SERVER_INIT_REP;
/* @} */

/**
 * @brief   CPRI state notification
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	DATETIME_T			timestamp;				/**< time stamp				 */
	USHORT			    retype;					/**< RE種別					 */
	USHORT				selfslotno;				/**< 自カードスロット番号		 */
	USHORT				cprista;				/**< CPRI state				 */
	USHORT				hdlcsta;				/**< HDLC state				 */
	USHORT				L2abnminfo;				/**< L2 link 異常情報			 */
	USHORT				hcsta;					/**< ヘルスチェック情報		 */
	USHORT				traclsinfo;				/**< TRA間active/negative情報  */
	USHORT				dummy;					/**< dummy area	*/
} MNT_COM_TSKIF_CPRISTANTC;	

/**
 * @brief   CPRI link state notification (between PC and REC)
 */
#ifndef OPT_RRH_ZYNQ_REC
typedef struct
{
	USHORT		selfslotno;					/* CPRI番号					*/
	USHORT		retype;						/* RE装置番号				*/
	USHORT		cpriconf;					/* linkCPRI使用/未使用情報	*/
	USHORT		cprista;					/* CPRI state				*/
	USHORT		hdlcsta;					/* HDLC state				*/
	USHORT		L2abnminfo;					/* L2 link 異常情報			*/
	USHORT		hcsta;						/* ヘルスチェック情報		*/
	USHORT		traclsinfo;					/* RE共通部リンクactive/negative情報 */
	USHORT		actswforbflg;				/* RE共通部active切替禁止flag	*/
	USHORT		dummy;						/* ダミー領域				*/
}MNT_CPRISTANTC_REC_INFO;	


typedef struct
{
	CMT_TSKIF_HEAD		head;				/**< タスク共通ヘッダ			 */
	DATETIME_T			timestamp;			/**< time stamp				 */
	MNT_CPRISTANTC_REC_INFO stainfo[12];
}MNT_COM_TSKIF_CPRISTANTC_REC;	
#else
typedef struct
{
	CMT_TSKIF_HEAD		head;				/**< タスク共通ヘッダ			 */
	DATETIME_T			timestamp;			/**< time stamp				 */
	USHORT		selfslotno;					/* CPRI番号					*/
	USHORT		retype;						/* RE装置番号				*/
	USHORT		cpriconf;					/* linkCPRI使用/未使用情報	*/
	USHORT		cprista;					/* CPRI state				*/
	USHORT		hdlcsta;					/* HDLC state				*/
	USHORT		L2abnminfo;					/* L2 link 異常情報			*/
	USHORT		hcsta;						/* ヘルスチェック情報		*/
	USHORT		traclsinfo;					/* RE共通部リンクactive/negative情報 */
	USHORT		actswforbflg;				/* RE共通部active切替禁止flag	*/
	USHORT		dummy;						/* ダミー領域				*/
	CHAR		linkdummy[20 * 11];			/* remain 11 links*/
}MNT_COM_TSKIF_CPRISTANTC_REC;	
#endif

/*!
 *  @brief   Little endian <-> Big endian
 *  @note    Little endian <-> Big endian
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	union
	{
	    CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部	 */
	    CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部	 */
	}m;
	union
	{
		USHORT                  signal_kind;
		CHAR                    body[2];        /**< CPRI信号データ部		 */
	}m2;
}MNT_COM_CMT_CPRIF_MAPPING;
/* @} */


#endif
/* @} */
