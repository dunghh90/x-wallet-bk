/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_typ.h
 *  @brief  5GDU-LLS supervision common structure head file defination
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2018/08/21 KCN)Kamimoto 5GDU-LLS Update
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2018
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EQS_TYP
#define		F_EQS_TYP

/** @addtogroup RRH_PF_EQS
* @{ */

/**
 * @brief	EQS Status List structure
 * @note	Contents:  EQS Status List structure \m
 * 
 */

/**************************/
/* ステータスリストデータ */
/**************************/
typedef struct
{
	UINT	sts_kind;				/* ステータス種別		*/
	UINT	sts_data;				/* ステータス情報		*/
}T_EQS_STSLST_DATA ;

typedef struct
{
	UINT				stslst_flg[D_SYS_NUM8];			/* ステータスリスト 256個分の有効/無効 */
	T_EQS_STSLST_DATA	sts_list[D_SYS_NUM256];			/* ステータスリスト */
} T_EQS_STLIST ;


/**
 * @brief	RSV history log structure
 * @note	Contents:	RSV history log structure \n
 * 
 */

/************************/
/* RSVログ情報			*/
/************************/
typedef struct
{
	UINT		tskid ;				/*	タスクID			*/
	UINT		event_no ;			/*	イベント番号		*/
	USHORT		year ;				/*	年表示情報			*/
	UCHAR		month ;				/*	月表示情報			*/
	UCHAR		day ;				/*	日表示情報			*/
	UCHAR		hour ;				/*	時表示情報			*/
	UCHAR		min ;				/*	分表示情報			*/
	UCHAR		sec ;				/*	秒表示情報			*/
	UCHAR		msec ;				/*	ミリ秒情報			*/
	
}T_RSV_LOGHEAD ;

typedef struct
{
	T_RSV_LOGHEAD		 loghead ;							/*	ログヘッダ情報			*/
	USHORT				 sv_tbl1[D_RSV_LOG_SV1_SIZE] ;		/*	SVテーブル情報1			*/
	USHORT				 sv_tbl2[D_RSV_LOG_SV2_SIZE - 2] ;	/*	SVテーブル情報2			*/
	USHORT				 log_timing ;						/*	ログタイミング			*/
	USHORT				 log_offset ;						/*	オフセット情報			*/
} T_RSV_LOG;

typedef struct
{
	UINT	almCode;
	UINT	regType;
	UINT	chkRegAddr;
	UINT	chkBitmap;
	UINT	chkCycle;
	UINT	*cyclCntAddr;
	UINT	count;
	UINT	*cntAddr;
}T_RSV_ALMINFO;

typedef struct
{
	UINT factor;
	UINT	chkRegAddr;
	UINT	chkBitmap;
	UINT	chkCycle;
	UINT	count;
	UINT	*cyclCntAddr;
	UINT	*cntAddr;
	UINT	*cyclRecCntAddr;
	UINT	*recCntAddr;
	UINT	setRegAddr;
	UINT	setBitmap;
}T_RSV_ERRINFO;

typedef struct
{
	UINT	ledSta;
	UINT	setVal;
}T_RSV_LEDINFO;

/* @}  */

/**
 * @brief	RSV state change log structure
 * @note	Contents:	RSV history log structure \n
 * 
 */
typedef struct
{
	UINT	chgType;
	UINT	svState[D_SYS_NUM8];
}T_RSV_SVSTATELOG;
/* @}  */

/**
 * @brief	Port-IN State management structure
 * @note	Contents:	Port-IN State management structure \n
 * 
 */
typedef struct
{
	UINT	portInSta;				/* Port-IN state		*/
	UINT	chgCount;				/* State change count	*/
}T_RSV_PORTINSTA;

/**
 * @brief	Card Control management structure
 * @note	Contents:	Card Control management structure \n
 * 
 */
typedef struct
{
	UINT		cardCntItem;
	UINT		pasw_state;
	USHORT	warmup_flg[2];
}T_RSV_CARDCNT;

typedef struct
{
	UINT	ledSta;
	UINT	ledVal;
}T_EQS_LEDINFO;

typedef struct
{
	UINT16			faultId;									/* Fault-ID							*/
	UINT8		 	affect_cnt ;								/* 詳細情報有効文字数				*/
	UINT8		 	faulttxt_cnt ;								/* 故障情報有効文字数				*/
	UINT8			fault_source;								/* Fault Source情報(enum)			*/
	const CHAR *	affected_objects ;							/* 詳細情報(string)					*/
	UINT			fault_severity;								/* アラーム種別(enum)				*/
	const CHAR *	fault_text;									/* 故障情報							*/
}T_EQS_FAULT_TBL;

typedef struct
{
	UINT	used_flg;									/* アラーム要因設定中フラグ		*/
	T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND	alm_data;	/* アラーム要因データ			*/
}T_EQS_FM_NOTIF_DATA;

typedef struct
{
	UINT	svInfIdx;
	UINT	svInfAdd;
	UINT	svInfMsk;
	UINT	svInfDat;
}T_EQS_SVCTRLMNG;


typedef struct
{
	UINT	svInfIdx;
	UINT	svInfmsk;
	UINT	tddEn;
	UINT	StpWv;
}T_EQS_STPWV_MNG;

#endif	/* M_RS_TYP */
