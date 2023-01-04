/*!
 * @skip  $ld:$
 * @file  f_cmn_com_layer3.c
 * @brief 共有メモリ取得/設定(Layer3状態)
 * @date  2015/08/14 TDI)uchida create
 */
 
/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   Layer3状態取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_layer3_get(USHORT lte3g, T_RRH_LAYER3 *pOutData)
{
    UINT offset  = lte3g * sizeof(*pOutData);           /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_LAYER3;        /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_LAYER3;        /* セマフォナンバー     */
// 擬似環境
    pOutData->layer3_rec = 7; // D_L3_STA_REC_USING RECを運用中にする。
    pOutData->layer3_re[0] = 4; // D_L3_STA_4 REを運用中にする。
    for(int i=1;i<16;i++)
    {
        pOutData->layer3_re[i] = 0;
    }
    return 0;
//    return f_cmn_com_shm_get_offset(shm_num, sem_num, sizeof(*pOutData), offset, pOutData);
// 擬似環境
}

/*!
 * @brief   Layer3状態設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   lte3g       [in]    LTE/3G
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_layer3_set(USHORT lte3g, T_RRH_LAYER3 *pInData)
{
    UINT offset  = lte3g * sizeof(*pInData);            /* 共有メモリオフセット */
    UINT shm_num = (UINT)E_RRH_SHMID_APL_LAYER3;        /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_LAYER3;        /* セマフォナンバー     */

    return f_cmn_com_shm_set_offset(shm_num, sem_num, sizeof(*pInData), offset, pInData);
}

/*!
 * @brief   Layer3状態設定(CPRIリンク単位)
 * @param   link_num    [in]    CPRIリンク番号
 * @param   lte3g       [in]    LTE/3G
 * @param   layer3      [in]    Layer3
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/21 TDI)satou create
 */
INT f_cmn_com_layer3_set_each(USHORT link_num, USHORT lte3g, USHORT layer3) {
    UINT shm_num = (UINT)E_RRH_SHMID_APL_LAYER3;        /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_LAYER3;        /* セマフォナンバー     */
    INT ret;                                            /* 戻り値評価用 */
    INT errcd;                                          /* BPFエラーコード */
    T_RRH_LAYER3 *buf_p;

    /* 共有メモリのアドレス取得 */
    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(shm_num ,(VOID**)&buf_p, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", shm_num, errcd);
        return D_RRH_NG;
    }

    /* セマフォ取得 */
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num ,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /* 共有メモリ更新 */
    buf_p += lte3g;
    if (D_RRH_CPRINO_REC == link_num) {
        buf_p->layer3_rec = layer3;
    } else {
        buf_p->layer3_re[link_num - 1] = layer3;
    }

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*! @} */
