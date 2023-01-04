/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrdssc_sta.c
 * @brief  運用中REカード状態報告(セクタ部)開始処理
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"
//#include "l3_com_inc.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REカード状態報告(セクタ部)開始処理
 * @note   運用中REカード状態報告(セクタ部)開始処理を行う
 *         - L3(REカード状態報告要求(セクタ部))送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date    2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrdssc_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT counter;
	USHORT port_no;
	USHORT slot_num;
	USHORT slot_no1;
	USHORT slot_no2;
	USHORT slot_snd1;
	USHORT slot_snd2;
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
	USHORT slot_bit[CMD_NUM16] = {0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
									0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
//	CHAR   errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 16B(0)周期カウンタ加算(+1) */
    timeRecrdcscCount[cpr_no_idx] += CMD_NUM1;
    counter = timeRecrdcscCount[cpr_no_idx];
    if (timeRecrdcscCount[cpr_no_idx] >= CMD_NUM10)
    {   /* カウンタが10になったら、周期カウンタを0にしておく */
        timeRecrdcscCount[cpr_no_idx] = CMD_NUM0;
    }

    /* 16B(1)CPRI番号からPort番号を抽出 */
    port_no = cpr_no_idx + 1;

    /* 16B(2)slot数を抽出 */
    slot_num = f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf.slt_num;

    if(counter == CMD_NUM1)
    {/* 1回目の時、48時間経過チェックのベース時刻を取得して (次の１回目で再度取得する) */
        BPF_RU_UTTM_GETTIME(&rst48hTim[cpr_no_idx]);
    }

    if(counter < CMD_NUM9)
    {/* 1～8回目の時は、各slotへ送信処理を行う */
        /* 送信#1のslot番号を算出 */
        /* 16B(3)周期カウンタ値とPort番号から送信slot(#1～#16)を抽出 */
        slot_snd1 = ((counter + CMD_NUM17) - (cpr_no_idx + CMD_NUM1)) % CMD_NUM16;
        if (slot_snd1 == CMD_NUM0)
        {
            slot_snd1 = CMD_NUM16;
        }
        /* 16B:(4)送信slot(#1～16)が、slot数以内の場合はslot番号抽出 */
        if (slot_snd1 <= slot_num)
        {
            slot_no1 = f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf.slt_no[slot_snd1 - 1];

            /******************************************/
            /* L3(REカード状態報告要求(セクタ部))送信 */
            /******************************************/
            f_trp_rec_Fc02_t_l3_recrdsscreq_snd(                      /* なし:R */
                slot_no1                                      /* スロット番号:I */
            );
            timeRecrdSndCount[cpr_no_idx] |= slot_bit[slot_snd1 - 1]; /* REカード状態報告 送信slotのBitをON */
        }
        /* 送信#2のslot番号を算出 */
        /* 16B(5)送信#2の送信slot(#1～#16)から抽出 */
        slot_snd2 = (slot_snd1 + CMD_NUM8) % CMD_NUM16;
        if (slot_snd2 == CMD_NUM0)
        {
            slot_snd2 = CMD_NUM16;
        }
        /* 16B:(6)送信slot(#1～16)が、slot数以内の場合はslot番号抽出 */
        if (slot_snd2 <= slot_num)
        {
            slot_no2 = f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf.slt_no[slot_snd2 - 1];

            /******************************************/
            /* L3(REカード状態報告要求(セクタ部))送信 */
            /******************************************/
            f_trp_rec_Fc02_t_l3_recrdsscreq_snd(                      /* なし:R */
                slot_no2                                      /* スロット番号:I */
            );
            timeRecrdSndCount[cpr_no_idx] |= slot_bit[slot_snd2 - 1]; /* REカード状態報告 送信slotのBitをON */
        }
    }
    else if (counter == CMD_NUM10)
    {
	    /* (7)周期カウンタが10の時は、ポート単位での集約処理を実施し、カウンタを0クリアしてreturn */

		timeRecrdcscCount[cpr_no_idx] = CMD_NUM0;


		f_trp_rec_Fc02_t_recrdssc_i_merge(); /* LTEのPort集約(マージ)を行う */


		/* 返信なしのスロットがある場合 */
//		if (timeRecrdSndCount[cpr_no_idx] != CMD_NUM0)
//		{
//			snprintf(errdata, CMD_NUM48, "[cpr:%02d][slt:%04x][LTE]RE Card sts report No Resp.", D_REC_C02_CPR_NO(), timeRecrdSndCount[cpr_no_idx]);
//			D_REC_IVLINI();
//			D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
//			D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
//			
//		}

    }/* counter = 9の時は無処理 */

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
