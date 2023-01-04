/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_use_flg_get.c
 * @brief  
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  
 * @TBD_No 
 * @date
 *
 *
 */
/****************************************************************************/
UINT* f_trp_rec_Rc03_m_use_flg_get(VOID *bufp, USHORT awSize) {
	CMT_CPRIF_CARLSTSRES   *rcvp;             /* 受信REキャリア状態アドレス */
	UINT    *rcvmsg_use_flg;      				/* 受信メッセージにキャリア */
	USHORT  *offset         = CMD_NUM0;                   /* 参照オフセット */
	USHORT  crr_no          = CMD_NUM0;                     /* キャリア番号 */
	USHORT  crr_idx         = CMD_NUM0;                    /* キャリアINDEX */
    USHORT  crr_cnt         = CMD_NUM0;             /* キャリアインデックス */
	USHORT  rcv_brcnum      = CMD_NUM0;                      /*  ブランチ数 */
    USHORT  rcv_alminfnum   = CMD_NUM0;                        /* ALM情報数 */
    USHORT  rcv_crdinfnum   = CMD_NUM0;                         /* カード数 */

	/* 受信REカード状態アドレス抽出 */
    rcvp = (CMT_CPRIF_CARLSTSRES*)bufp;

	rcvmsg_use_flg = malloc(sizeof(UINT)*awSize); /* pgr0520 */
    /* キャリア状態詳細先頭情報オフセット設定 */
    offset = &(rcvp->car_num);
	offset++;

    /*******************************************/
    /* 正常CPRI 信号受信（自キャリア情報あり） */
    /*******************************************/
    /* キャリア数分ループする */
    for (crr_cnt = CMD_NUM0; crr_cnt < rcvp->car_num; crr_cnt++)
	{
        /* ループカウンタの該当するキャリア番号を取得 */
        crr_no = *offset;
        /* キャリアインデックス設定(0) */
        crr_idx = (USHORT)(crr_no - CMD_NUM1);

        rcvmsg_use_flg[crr_idx] = CMD_ON;

        /* テーブル設定はせず、次のキャリア情報用に、オフセットのみずらす */
        offset += CMD_NUM3;                   /* キャリア状態OFFSET算出 */
        offset ++;                              /* ブランチ数OFFSET算出 */
        rcv_brcnum   = *offset;                   /* ブランチ情報数取得 */
        offset += (rcv_brcnum * CMD_NUM4);
                                    /* ブランチ情報(最終位置)OFFSET算出 */
        offset ++;                               /* ALM情報数OFFSET算出 */
        rcv_alminfnum = *offset;                       /* ALM情報数取得 */
        offset += rcv_alminfnum;         /* ALM情報(最終位置)OFFSET算出 */
        offset ++;                                /* カード数OFFSET算出 */
        rcv_crdinfnum = *offset;                        /* カード数取得 */
        offset += rcv_crdinfnum;    /* スロット番号(最終位置)OFFSET算出 */
        offset ++;          /* 次のキャリア情報のキャリア番号OFFSET算出 */
    }
	return rcvmsg_use_flg;
}
