/*!
 * @skip  $ld:$
 * @file  f_cmn_com_conn_disk.c
 * @brief 共有メモリ取得/設定(無線送信ONOFF状態)
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   無線送信ONOFF状態取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * 
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 * @date    2015/10/01 TDI)satou 引数lte3gを削除。無線送信ONOFF状態は3G/LTEで共通
 */
INT f_cmn_com_tx_onoff_get(T_RRH_TX_ONOFF *pOutData)
{
    UINT offset  = 0;                                   /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_RRH_TX_ONOFF;  /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_RRH_TX_ONOFF;  /* セマフォナンバー     */

    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
}

/*!
 * @brief   無線送信ONOFF状態設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * 
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 * @date    2015/10/01 TDI)satou 引数lte3gを削除。無線送信ONOFF状態は3G/LTEで共通
 */
INT f_cmn_com_tx_onoff_set(T_RRH_TX_ONOFF *pInData)
{
    UINT offset  = 0;                                   /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_RRH_TX_ONOFF;  /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_RRH_TX_ONOFF;  /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*! @} */
