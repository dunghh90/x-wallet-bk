/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_createREslotinfo.c
 *  @brief  スロット情報
 *  @date   2015/08/27 TDIPS)maruyama create スロット情報(m_rc_infoAllRE.cから分割）
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "m_rc_ftyp.h"

/********************************************************************************************************************/
/**
 *  @brief  スロット情報
 *  
 *  @return None
 *  @date   2015/08/07  TDIPS) maruyama create
 *  @date   2015/08/24  TDIPS) maruyama modify RE接続判定の修正
 *  @date   2015/08/27 TDIPS)maruyama create スロット情報(m_rc_infoAllRE.cから分割）
 *  @date   2015/09/03 TDIPS)maruyama modify signal_kindの書き込み設定修正
 *  @date   2015/09/10 TDIPS)maruyama get_re_EqpSlotInfo_＊＊をローカル変数に変更
 *  @date   2015/09/10 TDI)satou MKレビュー指摘修正
 *  @date   2015/11/18 FPT)Yen fix bug IT3 No88
 *  @date	2016/03/10 TDI)satou DUP/BPF/SWITCH(D7)のビットは通知対象外に
 */
/********************************************************************************************************************/
VOID m_rc_createREslotinfo()
{
// [16B] chg start
//	T_RRH_SLOT_S3G	buf_LTEslotInfo	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
//	T_RRH_SLOT_3G	buf_3GslotInfo	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
	T_RRH_SLOT_S3G_RE	buf_LTEslotInfo	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
	T_RRH_SLOT_3G_RE	buf_3GslotInfo	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
// [16B] chg end

	T_RRH_SLOT_S3G	set_LTEslotInfo	= {};
	T_RRH_SLOT_3G	set_3GslotInfo	= {};
	UINT			cnt				= 0;
	T_RRH_SLOT_S3G	get_re_EqpSlotInfo_LTE = {};
	T_RRH_SLOT_3G	get_re_EqpSlotInfo_3G  = {};
// [16B] add start
	USHORT          slot_num;  // スロット数(カード数)
	USHORT          i;         // ループカウンタ
// [16B] add end

	for(cnt = 1 ; cnt < D_RRH_CPRINO_NUM ; cnt++)
	{
	    /*
	     * 共有メモリから配下REの報告内容を取得し、正常応答を受信した(signal_kind != 0)
	     * 配下REの報告内容をREカード機能部情報(card_inf)にORする
	     */

		if(re_startupRSP[cnt][CMD_SYS_S3G] == D_RRH_ON)
		{
			f_cmn_com_slot_s3g_get(cnt,&buf_LTEslotInfo);
// [16B] add start
            slot_num = buf_LTEslotInfo.slot_num;
// [16B] add end

			if((buf_LTEslotInfo.signal_kind != 0))
			{
// [16B] chg start
                for( i=0; i < slot_num; i++ ) {
//                  get_re_EqpSlotInfo_LTE.card_inf |= buf_LTEslotInfo.card_inf;
                  get_re_EqpSlotInfo_LTE.card_inf |= buf_LTEslotInfo.card_inf[i];

                }
// [16B] chg end
			}
		}

		if(re_startupRSP[cnt][CMD_SYS_3G] == D_RRH_ON)
		{
			f_cmn_com_slot_3g_get(cnt,&buf_3GslotInfo);
// [16B] add start
            slot_num = buf_3GslotInfo.slot_num;
// [16B] add end

			if((buf_3GslotInfo.signal_kind != 0))
			{
// [16B] chg start
                for( i=0; i < slot_num; i++ ) {
//                get_re_EqpSlotInfo_3G.card_inf |= buf_3GslotInfo.card_inf;
                get_re_EqpSlotInfo_3G.card_inf |= buf_3GslotInfo.card_inf[i];

                }
// [16B] chg end
			}
		}
	}
	
	/* DUP/BPF/SWITCH(D7)のビットは通知対象外であるためマスクする */
	get_re_EqpSlotInfo_LTE.card_inf &= ~CMD_CRDFNBIT_DUP;

	/*
	 * 共有メモリへの書き込み
	 * signal_kind  固定値(LTE:0x5221, 3G:0x5220)
	 * result       固定値(0)
	 * slot_num     固定値(1)
	 * card_inf     TRX-INFと配下RE集約結果のOR
	 */
	set_LTEslotInfo.signal_kind = CMD_CPRID_SLOTINFRES + CMD_SYS_LTE;
	set_LTEslotInfo.result 		= 0;
	set_LTEslotInfo.slot_num	= 1;
	set_LTEslotInfo.card_inf	= (USHORT)(RCD_CARDFB_BIT | get_re_EqpSlotInfo_LTE.card_inf);
	f_cmn_com_bootslot_s3g_set(&set_LTEslotInfo);

	set_3GslotInfo.signal_kind  = CMD_CPRID_SLOTINFRES;
	set_3GslotInfo.result		= 0;
	set_3GslotInfo.slot_num		= 1;
	set_3GslotInfo.card_inf		= get_re_EqpSlotInfo_3G.card_inf;
	f_cmn_com_bootslot_3g_set(&set_3GslotInfo);
}

/* @} */
