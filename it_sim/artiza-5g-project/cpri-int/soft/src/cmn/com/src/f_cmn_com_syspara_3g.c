/*!
 * @skip  $ld:$
 * @file  f_cmn_com_syspara_3g.c
 * @brief 共有メモリ取得/設定(シスパラ(3G))
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   シスパラ(3G)取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_syspara_3g_get(T_RRH_SYSPAR_3G *pOutData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_SYSPAR_3G; /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_SYSPAR_3G; /* セマフォナンバー   */

    return f_cmn_com_shm_get(shm_num, sem_num, sizeof(*pOutData), pOutData);
}

/*!
 * @brief   シスパラ(3G)設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_syspara_3g_set(T_RRH_SYSPAR_3G *pInData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_SYSPAR_3G; /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_SYSPAR_3G; /* セマフォナンバー   */

    return f_cmn_com_shm_set(shm_num, sem_num, sizeof(*pInData), pInData);
}

/*! @} */
