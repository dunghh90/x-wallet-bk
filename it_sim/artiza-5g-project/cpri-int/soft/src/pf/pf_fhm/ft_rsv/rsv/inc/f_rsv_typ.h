/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_typ.h
 *  @brief  RE supervision common structure head file defination
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/* “ñdŽæž‚Ý–hŽ~ */
#ifndef		F_RSV_TYP
#define		F_RSV_TYP

/** @addtogroup RRH_PF_RSV
* @{ */


/**
 * @brief	RSV history log structure
 * @note	Contents:	RSV history log structure \n
 * 
 */

/************************/
/* 屄暿儘僌娗棟宆		*/
/************************/
typedef struct
{
	UINT		tskid ;				/*	婲摦尦僞僗僋ID		*/
	UINT		event_no ;			/*	婲摦梫場斣崋		*/
	USHORT		year ;				/*	彂崬敪惗擭			*/
	UCHAR		month ;				/*	彂崬敪惗寧			*/
	UCHAR		day ;				/*	彂崬敪惗寧			*/
	UCHAR		hour ;				/*	彂崬敪惗帪			*/
	UCHAR		min ;				/*	彂崬敪惗暘			*/
	UCHAR		sec ;				/*	彂崬敪惗昩			*/
	UCHAR		msec ;				/*	彂崬敪惗ms			*/
	
}T_RSV_LOGHEAD ;

typedef struct
{
	T_RSV_LOGHEAD		 loghead ;				/*	ƒƒOƒwƒbƒ_•”			*/
	USHORT				 sv_tbl1[D_RSV_LOG_SV1_SIZE] ;/*	ƒZ[ƒu1–Ê			*/
	USHORT				 sv_tbl2[D_RSV_LOG_SV2_SIZE - 2] ;/*	ƒZ[ƒu2–Ê			*/
	USHORT				 log_timing ;					/*	ƒZ[ƒu2–Ê			*/
	USHORT				 log_offset ;					/*	ƒZ[ƒu2–Ê			*/
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

#endif	/* M_RS_TYP */
