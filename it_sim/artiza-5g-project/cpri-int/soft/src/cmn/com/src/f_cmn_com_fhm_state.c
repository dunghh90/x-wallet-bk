/*!
 * @skip  $ld:$
 * @file  f_cmn_com_fhm_state.c
 * @brief 共有メモリ取得/設定(FHM状態)
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   FHM状態取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_fhm_state_get(T_RRH_FHM_STATE *pOutData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_FHM_STATE; /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_FHM_STATE; /* セマフォナンバー   */

    return f_cmn_com_shm_get(shm_num, sem_num, sizeof(*pOutData), pOutData);
}

/*!
 * @brief   FHM状態設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_fhm_state_set(T_RRH_FHM_STATE *pInData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_FHM_STATE; /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_FHM_STATE; /* セマフォナンバー   */

    return f_cmn_com_shm_set(shm_num, sem_num, sizeof(*pInData), pInData);
}

/*! @} */
