/*!
 * @skip	$Id$
 * @file	m_cm_ChkCardSta.c
 * @brief	
 * @date	2008/8/25 FFCS)Tangj create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
*/

/** @addtogroup RRH_DCM
 *  @{
 */
/*************************************************************************** */
/*		define(s)/include(s)												 */
/*************************************************************************** */
#include "m_cm_header.h"


/**************************************************************************/
/**
 *  @brief  Card status check function
 *  @note   Specify the card type or slot number \n
 *          if card status is the same as specified card status, return OK \n
 *          if card status is different from specified card status, return NG \n
 *  @param  targetkind		[in]		specified target kind
 *  @param  chktarget		[in]		specified target value
 *  @param  chkcardsta		[in]		specified card status 
 *  @param  file			[in]		calling fucntion file name
 *  @param  line			[in]		calling function executing line
 *  @return Check status 
 *  @retval CMD_CHKCARDSTA_R_OK				0: 	Card status is the same as the specified card status
 *  @retval CMD_CHKCARDSTA_R_NG				1: 	Card status is different from the specified card status	
 *  @retval CMD_RES_PARAMERR		0xF0000008: Parameter Specify error
 *  @date   2008/07/22		FFCS)Tangj	new create
 */
/**************************************************************************/
/*		 <チェック状態の項目>												 */
/*		┌───┬───┬───┬───┬───┬───┬───┬───┐	 */
/*		│予備  │予備  │予備  │予備  │ 予備 │ USE  │ ALM  │ ERR  │	 */
/*		├───┼───┼───┼───┼───┼───┼───┼───┤	 */
/*		│実装  │未実装│ ACT  │予備  │ 予備 │ BLK  │ CHK  │      │	 */
/*		└───┴───┴───┴───┴───┴───┴───┴───┘	 */
/**************************************************************************/

USHORT m_cmR_ChkCardSta(UINT targetkind, UINT chktarget, USHORT chkcardsta, 
														VOID *file, UINT line)
{
	/* 変数初期化  */
	USHORT	getcardsta = CMD_OK;		/* カード状態格納先					 */
	USHORT	rtn        = CMD_OK;		/* 戻り値格納変数					 */
	UINT	sarchcard  = CMD_OK;		/* カード検索関数戻り値				 */
	
	/********************************************************************/
	/* パラメータチェック												*/
	/********************************************************************/
	if ((targetkind != CMD_TARGET_SLOT) &&
		(targetkind != CMD_TARGET_CARD))
	{
		return CMD_CHKCARDSTA_R_NG;
	}
	
	/******************************* */
	/* スロット番号指定時処理		 */
	/******************************* */
	/* 対象種別を判定  */
	if (targetkind == CMD_TARGET_SLOT)
	{
		/* カード検索関数をコールし、指定スロット番号に対応する	 */
		/* カードを求める										 */
		sarchcard = m_cm_CardSarch(chktarget);
		
		if (sarchcard == CMD_RES_NG)
		{
			cm_Assert(CMD_ASL_USELOW, line, file) ;
			return CMD_CHKCARDSTA_R_NG;

		}
	}
	
	/***************** */
	/* カード状態取得  */
	/***************** */
	/* カード状態取得関数をコールする  */
	m_cm_GetCardSta(&getcardsta);
	
	/***************** */
	/* カード状態判定  */
	/***************** */
	/* 指定された状態が全てONの場合は戻り値に正常を設定  */
	if ((getcardsta & chkcardsta) == chkcardsta)
	{
		rtn = CMD_CHKCARDSTA_R_OK;
	}
	/* 指定されたチェック状態と取得状態が一致しない場合  */
	else
	{
		rtn = CMD_CHKCARDSTA_R_NG;

	}
	return rtn;
}


/**************************************************************************/
/**
 *  @brief  Card status check function
 *  @note   カード状態取得処理 \n
 *          実際のカード状態をテーブルから取得し、取得したデータを \n
 *          チェック用の形(下図ビットイメージ)に編集し \n
 *          OUTパラメータに格納する。 \n
 *		┌───┬───┬───┬───┬───┬───┬───┬───┐	\n
 *		│予備  │予備  │予備  │予備  │ 予備 │ USE  │ ALM  │ ERR  │	\n
 *		├───┼───┼───┼───┼───┼───┼───┼───┤	\n
 *		│実装  │未実装│ ACT  │予備  │ 予備 │ BLK  │ CHK  │      │	\n
 *		└───┴───┴───┴───┴───┴───┴───┴───┘	\n
 *  @param  usCardSta		[in]		specified target kind
 *  @param  file			[in]		specified target value
 *  @param  line			[in]		specified card status 
 *  @return None 
 *  @date   2008/07/22		FFCS)Tangj	new create
 */
/**************************************************************************/
VOID m_cmR_GetCardSta( USHORT *usCardSta, VOID *file, UINT line)
{
	/* Input parameter (0 Address) check */
	if(usCardSta == NULL)
	{
		return;
	}
	/* カード格納先を初期化  */
	*usCardSta = CMD_NUM0;
	
	/* テーブル有効無効判定  */
	if(cmw_tra_sv_tbl.en_flg == CMD_OFF )
	{
		/* ASSERT  */
		cm_Assert(CMD_ASL_USELOW, line, file) ;
		return;
	}
	/************************************************************************/
	/* 実装 (0x8000⇒実装)													*/
	/************************************************************************/
	if ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & CMD_TRA_REG_INS) == CMD_TRA_REG_INS)
	{
		/* カード状態格納先に実装(0x0080)を設定 */
		*usCardSta |= CMD_CHK_INS;
	}
	/************************************************************************/
	/* ACT (0x2000⇒ACT)													*/
	/************************************************************************/
	if ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & CMD_TRA_REG_ACT) == CMD_TRA_REG_ACT)
	{
		/* カード状態格納先にACT(0x0020)を設定 */
		*usCardSta |= CMD_CHK_ACT;
	}
	/************************************************************************/
	/* USE (0x0004⇒USE)													*/
	/************************************************************************/
	if ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & CMD_TRA_REG_USE) == CMD_TRA_REG_USE)
	{
		/* カード状態格納先にUSE(0x0400)を設定 */
		*usCardSta |= CMD_CHK_USE;
	}
	/************************************************************************/
	/* ALM (0x0002⇒ALM)													*/
	/************************************************************************/
	if ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & CMD_TRA_REG_ALM) == CMD_TRA_REG_ALM)
	{
		/* カード状態格納先にALM(0x0200)を設定 */
		*usCardSta |= CMD_CHK_ALM;
	}
	/************************************************************************/
	/* ERR (0x0001⇒ERR)													*/
	/************************************************************************/
	if ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & CMD_TRA_REG_ERR) == CMD_TRA_REG_ERR)
	{
		/* カード状態格納先にERR(0x0100)を設定 */
		*usCardSta |= CMD_CHK_ERR;
	}
	
	return;

}
/**************************************************************************/
/**
 *  @brief  Card status check(extend) function
 *  @note   カード状態取得処理 \n
		  if alarm state, check whether multi branch alarm
 *  @param  usCardSta		[in]		specified target kind
 *  @param  br_alm          [out]       branch alarm
 *  @param  file			[in]		specified target value
 *  @param  line			[in]		specified card status 
 *  @return None 
 *  @date   2012/07/19		FFCS)niemsh	new create for 1.5GVA
  * @date   2015/04/23		FFCS)zhangxq	modify for TDD-SRE
  * @date   2015/10/28      TDI)satou FHM技説-QA-113
 */
/**************************************************************************/
VOID m_cmR_GetCardSta2( USHORT *usCardSta, USHORT *br_alm,VOID *file, UINT line)
{
	m_cm_GetCardSta(usCardSta);

	*br_alm = CMD_OFF;	
	
	return;
}
/* @} */

