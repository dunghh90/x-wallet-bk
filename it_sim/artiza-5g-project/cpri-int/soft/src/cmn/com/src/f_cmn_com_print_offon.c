/*!
 * @skip  $ld:$
 * @file  f_cmn_com_cardstslt_3g.c
 * @brief 共有メモリ取得/設定(デバッグ用printf文のOff/On状態)
 * @date    2016/10/25 KCN)hfuku create
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
 * @brief   デバッグ用printf文のOff/On状態取得
 * @note    -
 * @param   pOutData    [out]   取得先（0:Off / 1:ON）
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2016/10/25 KCN)hfuku create 16B機能追加
 */
INT f_cmn_com_print_offon_get(UCHAR *pOutData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_DBG_PRINT;     /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_DBG_PRINT;     /* セマフォナンバー     */
    INT  ret;
    INT errcd;      /* BPFエラーコード */
    UCHAR* buf_p;

    if (NULL == pOutData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:NULL pointer");
        return D_RRH_NG;
    }
    /* 共有メモリのアドレス取得 */
    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(shm_num ,(VOID**)&buf_p, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", shm_num, errcd);
        return D_RRH_NG;
    }

    /* セマフォ取得 */
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /*	固定長領域を読出する	*/
    memcpy(pOutData, buf_p, sizeof(UCHAR));
    
    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }
    return D_RRH_OK;
}

/*!
 * @brief   デバッグ用printf文のOff/On状態設定
 * @note    -
 * @param   pInData     [in]    0:Off / 1:ON
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2016/10/25 KCN)hfuku create 16B機能追加
 */
INT f_cmn_com_print_offon_set(UCHAR *pInData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_DBG_PRINT;     /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_DBG_PRINT;     /* セマフォナンバー     */
    INT  ret;
    INT errcd;      /* BPFエラーコード */
    UCHAR* buf_p;

    if (NULL == pInData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:NULL pointer");
        return D_RRH_NG;
    }
    /* 共有メモリのアドレス取得 */
    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(shm_num ,(VOID**)&buf_p, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", shm_num, errcd);
        return D_RRH_NG;
    }

    /* セマフォ取得 */
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /*	固定長領域を設定する	*/
    memcpy(buf_p, pInData, sizeof(UCHAR));
    
    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }
    return D_RRH_OK;
}

/*! @} */
