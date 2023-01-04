/*!
 * @skip  $ld:$
 * @file  f_cmn_com_fw_info_re.c
 * @brief 共有メモリ取得/設定(FHM保持FWファイル情報(RE-FW)
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   FHM保持FWファイル情報 (RE-FW)取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   side        [in]    FLASH面
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_fw_info_re_get(USHORT side, T_RRH_FW_INFO *pOutData)
{
    UINT offset  = side * sizeof(*pOutData);            /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_FWINFO_RE;     /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_FWINFO_RE;     /* セマフォナンバー     */

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   FHM保持FWファイル情報 (RE-FW)設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   side        [in]    FLASH面
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_fw_info_re_set(USHORT side, T_RRH_FW_INFO *pInData)
{
    UINT offset  = side * sizeof(*pInData);             /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_FWINFO_RE;     /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_FWINFO_RE;     /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
