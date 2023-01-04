/*!
 * @skip  $ld:$
 * @file  f_cmn_com_slotsrch_s3g.c
 * @brief スロット番号から何番スロットか抽出(S3G) 16B KCN add
 * @date  2016/10/04 KCN)fukushima create
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
 * @brief   スロット番号から何番スロットか抽出(S3G) 16B KCN add
 * @note    -
 * @param   linkno      [in]    CPRIリンク番号
 * @param   slotno      [in]    スロット番号
 * @param   slot_index  [out]   何番スロットかの情報
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2016/10/04 KCN)fukushima create 16B機能追加
 */
INT f_cmn_com_slotsrch_s3g(USHORT linkno, USHORT slotno, USHORT *slot_index)
{
    T_RRH_SLOT_S3G_RE slot_info;

    f_cmn_com_slot_s3g_get(linkno, &slot_info);

    *slot_index = slot_info.slt_idx[slotno - 1];

    if (*slot_index >= CMD_SLOTINF_MAX)
    {/* 16以上(0xFFFF)の場合は存在しないスロット番号のためNG */
        *slot_index = CMD_NUM0;
        return D_RRH_NG;
    }

    return D_RRH_OK;
}

/*! @} */
