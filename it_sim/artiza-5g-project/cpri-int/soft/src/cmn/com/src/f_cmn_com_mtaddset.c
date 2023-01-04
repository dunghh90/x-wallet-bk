/*!
 * @skip  $ld:$
 * @file  f_cmn_com_mtaddset.c
 * @brief 共有メモリ取得/設定(MTアドレス設定)
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "m_cm_def.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   MTアドレス設定取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   mt          [in]    FLD-MT/SV-MT(CMD_FLD_MT/CMD_SV_MT)
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_mtaddset_get(USHORT lte3g, USHORT mt, T_RRH_MTADDSET *pOutData)
{
    UINT offset;                                        /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_MTADDSET;      /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_MTADDSET;      /* セマフォナンバー     */

    offset = 0;
    offset += sizeof(*pOutData) * lte3g;
    offset += sizeof(*pOutData) * (CMD_MT_MAX * mt);

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   MTアドレス設定を設定する
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   mt          [in]    FLD-MT/SV-MT(CMD_FLD_MT/CMD_SV_MT)
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_mtaddset_set(USHORT lte3g, USHORT mt, T_RRH_MTADDSET *pInData)
{
    UINT offset;                                        /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_MTADDSET;      /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_MTADDSET;      /* セマフォナンバー     */

    offset = 0;
    offset += sizeof(*pInData) * lte3g;
    offset += sizeof(*pInData) * (CMD_MT_MAX * mt);

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
