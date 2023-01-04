/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_timmng_tbl_get.c
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
 * @date   2015/08/07 FPT)Yen create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_timmng_tbl_get(                     /* タイマ識別子:R */
    T_REC_TIMELM elm                              /* タイマ要素:I */
) {
	USHORT			lwTimerTbl[][CMD_NUM2] =
					{{	EC_REC_TMK_TOFVAL_CNF	,	EC_REC_TMID_TOFVAL_CNF   },
					 {	EC_REC_TMK_REEQPINF_CNF	,	EC_REC_TMID_REEQPINF_CNF },
					 {	EC_REC_TMK_RESLTINF_CNF	,	EC_REC_TMID_RESLTINF_CNF },
					 {	EC_REC_TMK_RESTS_INT	,	EC_REC_TMID_RESTS_INT    },
					 {	EC_REC_TMK_RESTS_CNF	,	EC_REC_TMID_RESTS_CNF    },
					 {  EC_REC_TMK_REPRTSTS_INT	,	EC_REC_TMID_REPRTSTS_INT },
					 {	EC_REC_TMK_REPRTSTS_CNF	,	EC_REC_TMID_REPRTSTS_CNF },
					 {	EC_REC_TMK_RECRDSSC_INT	,	EC_REC_TMID_RECRDSSC_INT },
					 {	EC_REC_TMK_RECRDSSC_CNF	,	EC_REC_TMID_RECRDSSC_CNF },
					 {	EC_REC_TMK_RECRRSTS_INT	,	EC_REC_TMID_RECRRSTS_INT },
					 {	EC_REC_TMK_RECRRSTS_CNF	,	EC_REC_TMID_RECRRSTS_CNF },
					 {	EC_REC_TMK_REAUTRST_WAT	,	EC_REC_TMID_REAUTRST_WAT },
					 {	EC_REC_TMK_MT_ADD_SET	,	EC_REC_TMID_MT_ADD_SET   },
					 {	EC_REC_TMK_RERST_CNF	,	EC_REC_TMID_RERST_CNF    },
					 {	EC_REC_TMK_REFRCRST_CNF	,	EC_REC_TMID_REFRCRST_CNF },
					 {	EC_REC_TMK_2NDRST_WAT	,	EC_REC_TMID_2NDRST_WAT   }
					};
	USHORT			lwTimerCnt;
	USHORT			lwTimerMax;
	USHORT			lwTimeIdStart = CMD_NUM0;
	USHORT			lwTimeId;
	USHORT			lwSysKind;
	USHORT			lwReNo;
	USHORT			extNum;
	USHORT			socNo;

	lwTimerMax = sizeof(lwTimerTbl) / sizeof(lwTimerTbl[0]);

	/* Search Timer Id */
	for( lwTimerCnt = CMD_NUM0; lwTimerCnt < lwTimerMax; lwTimerCnt++ )
	{
		if(elm.tmk == lwTimerTbl[lwTimerCnt][0])
		{
			lwTimeIdStart = lwTimerTbl[lwTimerCnt][1];
			break;
		}
	}
	if( lwTimerCnt == lwTimerMax )
	{
		/* Timer Id not found */
		return CMD_NUM0;
	}

	if( EC_REC_C02 == elm.cls )
	{
		lwSysKind = CMD_SYS_LTE;
	}
	else
	{
		lwSysKind = CMD_SYS_3G;
	}
	/* Re number (1~16) */
	lwReNo = elm.cpr_no;
	if((elm.tmk == EC_REC_TMK_RECRDSSC_INT) || (elm.tmk == EC_REC_TMK_RECRDSSC_CNF))
	{
		extNum = CMD_MAXSLOTNO;
		socNo = elm.soc_no;
	}
	else if(elm.tmk == EC_REC_TMK_REAUTRST_WAT)
	{
		extNum = CMD_MAXSLOTNO;
		socNo = elm.soc_no;
		/*	LTE only	*/
		lwSysKind = 0;
	}
	else
	{
		extNum = 1;
		socNo = 1;
	}

	/* Timer Id: 3G( RE#1 ~ RE#16 ), LTE( RE#1 ~ RE#16 ) */
	lwTimeId = lwTimeIdStart + (((lwReNo-1) + (D_RRH_CPRINO_RE_MAX * lwSysKind)) * extNum) + (socNo - CMD_NUM1);

	return lwTimeId;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
