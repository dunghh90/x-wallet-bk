/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_timmng_tbl03_get.c
 * @brief  Get timer Id 
 * @date   2015/08/07 FPT)Yen create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Get timer Id for rec
 * @note   
 *         
 * @param  elm [in]  タイマ要素
 * @return タイマID
 * @date   2015/08/14 FPT)Yen create.
 * @date   2015/11/13 FPT)Yen fix bug IT2 No 178.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_timmng_tbl03_get(                     /* タイマ識別子:R */
    T_REC_TIMELM elm                              /* タイマ要素:I */
) {
	USHORT			lwTimerTbl[][CMD_NUM3] =
					{{	EC_REC_TMK_TXS_CNF	,	EC_REC_TMID_TXS_CNF_3G,		EC_REC_TMID_TXS_CNF_S3G },
					 {	EC_REC_TMK_TXR_CNF	,	EC_REC_TMID_TXR_CNF_3G,		EC_REC_TMID_TXR_CNF_S3G }
					};
	USHORT			lwTimerCnt;
	USHORT			lwTimerMax;
	USHORT			lwTimeIdStart;
	USHORT			lwTimeId;
	USHORT			lwSysKind;
	USHORT			lwReNo;
	USHORT			lwCarrNo;

	lwTimerMax = sizeof(lwTimerTbl) / sizeof(lwTimerTbl[0]);

	if( EC_REC_C03 == elm.cls )
	{
		lwSysKind = CMD_SYS_LTE;
		lwCarrNo = CMD_MAX_CARNUM_S3G;
	}
	else
	{
		lwSysKind = CMD_SYS_3G;
		lwCarrNo = CMD_MAX_CARNUM_3G;
	}

	/* Search Timer Id */
	for( lwTimerCnt = CMD_NUM0; lwTimerCnt < lwTimerMax; lwTimerCnt++ )
	{
		if(elm.tmk == lwTimerTbl[lwTimerCnt][0])
		{
			lwTimeIdStart = lwTimerTbl[lwTimerCnt][CMD_NUM1 + lwSysKind];
			break;
		}
	}

	if( lwTimerCnt == lwTimerMax )
	{
		/* Timer Id not found */
		return CMD_NUM0;
	}


	/* Re number (1~16) */
	lwReNo = elm.cpr_no;

	/* Timer Id: Carr#( RE#1 ~ RE#16 ) */
	lwTimeId = lwTimeIdStart + ((lwReNo-1)*lwCarrNo) + (elm.soc_no - CMD_NUM1); /* pgr0000 */

	return lwTimeId;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
