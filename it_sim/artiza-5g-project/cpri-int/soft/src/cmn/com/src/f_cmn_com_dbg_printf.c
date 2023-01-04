/*!
 * @skip  $ld:$
 * @file  f_cmn_com_cardstslt_3g.c
 * @brief 共有メモリ取得/設定(デバッグ用printf文のOff/On状態)
 * @date    2016/10/25 KCN)hfuku create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2016
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   デバッグ用printf文(共有メモリのOff/Onを確認してOnなら実行する)
 * @note    -
 * @param   pOutData    [in]   出力文字列
 * @return  処理結果
 * @retval  -
 * @date    2016/10/25 KCN)hfuku create 16B機能追加
 */
VOID f_cmn_com_dbg_printf(CHAR *pOutData)
{
    UCHAR OffOn;

    /* デバッグ用printf文のOff/On状態を取得する */
    f_cmn_com_print_offon_get(&OffOn);

    if(OffOn == 1)
    {
        printf(pOutData);
    }
    return;
}

/*! @} */
