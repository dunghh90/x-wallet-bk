/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file    m_mk_CardStat_ChgNfy.c
 *  @brief   Card status change notification process
 *  @date   2013/11/21 FFCS)zhaodx create for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  RRH common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_Svp.h"
/********************************************************************************************************************/
/**
 *  @brief    Car status change notification process
 *  @note     Save card status
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_CardStat_ChgNfy(UINT *inf_p)
{
	T_API_SVP_CARDSTA_INFO		*cardsta_p;		/* Pointer of card state通知 IF*/
	UINT						count;
	USHORT						on_off;

	if(cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].conn == CMD_CONN_N) /* pgr0012 pgr0860 */
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CardStat_ChgNfy] ReMT mode off");
		return;
	}

	cardsta_p = &((T_API_SVP_CARDSTACHG_NTC *)inf_p)->cardSta;

	/* get card status and combine into mkw_cardStat start */
	for(count = 0; count < cardsta_p->chgNum; count++)
	{
		switch(cardsta_p->staChgInfo[count].chgType)
		{
		case D_API_SVP_STACHG_ACT:/* set ACT */
			on_off = cardsta_p->staChgInfo[count].chgSta;
			mkw_cardStat[0] = (mkw_cardStat[0] & 0xDFFF) | (on_off << 13);
			break;
		case D_API_SVP_STACHG_USE:/* set USE */
			on_off = cardsta_p->staChgInfo[count].chgSta;
			mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFB) | (on_off << 2);
			break;
		case D_API_SVP_STACHG_ERR:/* set ERR */
			on_off = cardsta_p->staChgInfo[count].chgSta;
			mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFE) | on_off;
			break;
		case D_API_SVP_STACHG_ALM:/* set ALM */
			on_off = cardsta_p->staChgInfo[count].chgSta;
			/* there is multi alarm case*/
			if(on_off) on_off = 1;
			mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFD) | (on_off << 1);
			/* アラーム情報 */
			mkw_cardStat[2] = cardsta_p->almInfo.almFactor;
			break;
		default:
		    break;
		}
	}

	/* WarmUp does not apply to TDD SRE */
	mkw_cardcnt_tbl.brInfo[CMD_BR0].paInfo.warmup_flg = CMD_NUM0;
	mkw_cardcnt_tbl.brInfo[CMD_BR1].paInfo.warmup_flg = CMD_NUM0;

	/* set INS */
	mkw_cardStat[0] = (mkw_cardStat[0] | 0x8000);
	
	/* TRX状態表示とCPRI状態表示 */
	mkw_cardStat[1] = cardsta_p->brSta[0] & 0x000F;
	mkw_cardStat[1] = mkw_cardStat[1] | ((cardsta_p->brSta[1] & 0x000F) << 4);
	mkw_cardStat[1] = mkw_cardStat[1] | ((cardsta_p->brSta[2] & 0x000F) << 8);
	mkw_cardStat[1] = mkw_cardStat[1] | ((cardsta_p->brSta[3] & 0x000F) << 12);

	/* ハード要因エラー */
//	mkw_cardStat[3] = cardsta_p->errInfo.hardErr;
	/*merge PA bit*/
	if(mkw_cardStat[3] & 0x0100)
	mkw_cardStat[1] = (mkw_cardStat[1] | 0x0008);
	else
	mkw_cardStat[1] = (mkw_cardStat[1] & (~0x0008));

	/*branch 0 alarm*/
	if(mkw_cardStat[1]  & 0x0001) mkw_cardStat[1]  &= 0xfff9;
	/*branch 1 alarm*/
	if(mkw_cardStat[1]  & 0x0010) mkw_cardStat[1]  &= 0xff9f;
	/*branch 2 alarm*/
	if(mkw_cardStat[1]  & 0x0100) mkw_cardStat[1]  &= 0xf9ff;
	/*branch 3 alarm*/
	if(mkw_cardStat[1]  & 0x1000) mkw_cardStat[1]  &= 0x9fff;

	/*merge use*/
	if((mkw_cardStat[1]  &  0x6666) && ((mkw_cardStat[0]  &  0x400) == CMD_NUM0 ))
	mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFB) | (0x04);
	if((mkw_cardStat[1]  &  0x6666) == CMD_NUM0)
	mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFB);
		
	if(mkw_cardStat[3] & 0x0200)
	mkw_cardStat[1] = (mkw_cardStat[1] | 0x0080);
	else
	mkw_cardStat[1] = (mkw_cardStat[1] & (~0x0080));
	
	if(mkw_cardStat[3] & 0x0400)
	{
		mkw_cardStat[1] = (mkw_cardStat[1] | 0x0800);
	}
	else
	{
		mkw_cardStat[1] = (mkw_cardStat[1] & (~0x0800));
	}
	
	if(mkw_cardStat[3] & 0x0800)
	{
		mkw_cardStat[1] = (mkw_cardStat[1] | 0x8000);
	}
	else
	{
		mkw_cardStat[1] = (mkw_cardStat[1] & (~0x8000));
	}
	
	/* ソフト要因エラー */
//	mkw_cardStat[4] = cardsta_p->errInfo.softErr;

	if(mkw_cardStat[4])
	mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFE) | 1;
	else
	mkw_cardStat[0] = (mkw_cardStat[0] & 0xFFFE);

	return;
}
/* @} */
