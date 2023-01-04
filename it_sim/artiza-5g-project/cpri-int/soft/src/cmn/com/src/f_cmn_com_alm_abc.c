/*!
 * @skip  $Id:$
 * @file  f_cmn_com_alm_abc.c
 * @brief 共有メモリALM_ABCの呼び出し
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   ALM_ABC取得時
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/18 TDI)uchida 共通の関数をコールするように見直し
 */
INT f_cmn_com_alm_abc_get(USHORT lte3g, T_RRH_ALM_ABC *pOutData)
{
    UINT offset  = lte3g * sizeof(*pOutData);           /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_ALM_ABC;       /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_ALM_ABC;       /* セマフォナンバー     */

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   ALM_ABC設定時
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   pInData     [in]    設定値
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/18 TDI)uchida 共通の関数をコールするように見直し
 */
INT f_cmn_com_alm_abc_set(USHORT lte3g, T_RRH_ALM_ABC *pInData)
{
    UINT offset  = lte3g * sizeof(*pInData);            /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_ALM_ABC;       /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_ALM_ABC;       /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
