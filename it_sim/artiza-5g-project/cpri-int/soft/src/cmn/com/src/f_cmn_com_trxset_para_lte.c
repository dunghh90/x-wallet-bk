/*!
 * @skip  $ld:$
 * @file  f_cmn_com_trxset_para_lte.c
 * @brief 共有メモリ取得/設定(TRX設定パラメータ(S3G))
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   TRX設定パラメータ(S3G)取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   carrier     [in]    キャリア番号(1～)
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_trxset_para_lte_get(USHORT carrier, T_TRXSET_PARA_LTE *pOutData)
{
    UINT offset  = (carrier - 1) * sizeof(*pOutData);       /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_TRXSET_PARA_LTE;   /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_TRXSET_PARA_LTE;   /* セマフォナンバー     */

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   TRX設定パラメータ(S3G)設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   carrier     [in]    キャリア番号(1～)
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_trxset_para_lte_set(USHORT carrier, T_TRXSET_PARA_LTE *pInData)
{
    UINT offset  = (carrier - 1) * sizeof(*pInData);        /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_TRXSET_PARA_LTE;   /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_TRXSET_PARA_LTE;   /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
