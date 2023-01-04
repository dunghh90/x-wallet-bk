/*!
 * @skip  $ld:$
 * @file  f_cmn_com_eqp_s3g.c
 * @brief 共有メモリ取得/設定(装置構成情報(S3G))
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   装置構成情報(S3G)取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   linkno      [in]    CPRIリンク番号
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_eqp_s3g_get(USHORT linkno, T_RRH_EQP_S3G *pOutData)
{
    UINT offset  = (linkno - 1) * sizeof(*pOutData);    /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_EQP_S3G;       /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_EQP_S3G;       /* セマフォナンバー     */

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   装置構成情報(S3G)設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   linkno      [in]    CPRIリンク番号
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_eqp_s3g_set(USHORT linkno, T_RRH_EQP_S3G *pInData)
{
    UINT offset  = (linkno - 1) * sizeof(*pInData);     /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_EQP_S3G;       /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_EQP_S3G;       /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
