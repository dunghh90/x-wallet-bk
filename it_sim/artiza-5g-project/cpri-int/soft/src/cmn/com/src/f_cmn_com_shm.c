/*!
 * @skip  $ld:$
 * @file  f_cmn_com_shm.c
 * @brief 共有メモリ取得/設定
 * @date  2015/08/17 TDI)satou Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"

/*!
 * @brief   指定共有メモリの内容を指定領域にコピーする
 * @note    直接本関数をコールするのではなくWrapperを介すようにすること
 * @param   shm_num     [in]    共有メモリナンバー
 * @param   sem_num     [in]    セマフォナンバー
 * @param   size        [in]    コピーするサイズ(共有メモリのサイズ)
 * @param   pOutData    [out]   コピー先
 * @return  処理結果
 * @retval  D_RRH_OK 正常
 * @retval  D_RRH_NG 異常
 * @date    2015/08/17 TDI)satou create
 */
INT f_cmn_com_shm_get(UINT shm_num, UINT sem_num, UINT size, VOID *pOutData) {
    INT ret;        /* 戻り値評価用 */
    INT errcd;      /* BPFエラーコード */
    VOID* buf_p;    /* 共有メモリアドレス */

    if (NULL == pOutData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error");
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

    /* 取得先にコピー */
    memcpy(pOutData, buf_p, size);

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*!
 * @brief   指定共有メモリのオフセット進めた位置からの内容を指定領域にコピーする
 * @note    直接本関数をコールするのではなくWrapperを介すようにすること
 * @param   shm_num     [in]    共有メモリナンバー
 * @param   sem_num     [in]    セマフォナンバー
 * @param   size        [in]    コピーするサイズ(共有メモリのサイズ)
 * @param   offset      [in]    共有メモリ先頭からのオフセット
 * @param   pOutData    [out]   コピー先
 * @return  処理結果
 * @retval  D_RRH_OK 正常
 * @retval  D_RRH_NG 異常
 * @date    2015/08/17 TDI)satou create
 */
INT f_cmn_com_shm_get_offset(UINT shm_num, UINT sem_num, UINT size, UINT offset, VOID *pOutData) {
    INT ret;        /* 戻り値評価用 */
    INT errcd;      /* BPFエラーコード */
    VOID* buf_p;    /* 共有メモリアドレス */
    UCHAR* buf_ucp; /* 共有メモリアドレス(オフセット加算用) */

    if (NULL == pOutData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error");
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

    /* 取得先にコピー */
    buf_ucp = (UCHAR*)buf_p;
    buf_ucp += offset;
    memcpy(pOutData, buf_ucp, size);

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*!
 * @brief   指定領域の内容を共有メモリにコピーする
 * @note    直接本関数をコールするのではなくWrapperを介すようにすること
 * @param   shm_num     [in]    共有メモリナンバー
 * @param   sem_num     [in]    セマフォナンバー
 * @param   size        [in]    コピーするサイズ(共有メモリのサイズ)
 * @param   pInData     [in]    コピー元
 * @return  処理結果
 * @retval  D_RRH_OK 正常
 * @retval  D_RRH_NG 異常
 * @date    2015/08/17 TDI)satou create
 */
INT f_cmn_com_shm_set(UINT shm_num, UINT sem_num, UINT size, VOID *pInData) {
    INT ret;                                        /* 戻り値評価用 */
    INT errcd;                                      /* BPFエラーコード */
    VOID* buf_p;                                    /* 共有メモリアドレス */

    if(pInData == NULL)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error");
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
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num ,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /* 共有メモリにコピー */
    memcpy(buf_p, pInData, size);

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*!
 * @brief   指定領域の内容を共有メモリ先頭からオフセット進めた領域にコピーする
 * @note    直接本関数をコールするのではなくWrapperを介すようにすること
 * @param   shm_num     [in]    共有メモリナンバー
 * @param   sem_num     [in]    セマフォナンバー
 * @param   size        [in]    コピーするサイズ(共有メモリのサイズ)
 * @param   offset      [in]    共有メモリ先頭からのオフセット
 * @param   pInData     [in]    コピー元
 * @return  処理結果
 * @retval  D_RRH_OK 正常
 * @retval  D_RRH_NG 異常
 * @date    2015/08/17 TDI)satou create
 */
INT f_cmn_com_shm_set_offset(UINT shm_num, UINT sem_num, UINT size, UINT offset, VOID *pInData) {
    INT ret;                                        /* 戻り値評価用 */
    INT errcd;                                      /* BPFエラーコード */
    VOID* buf_p;                                    /* 共有メモリアドレス */
    UCHAR *buf_ucp;                                 /* 共有メモリアドレス(オフセット加算用) */

    if(pInData == NULL)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error");
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
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num ,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /* 共有メモリにコピー */
    buf_ucp = (UCHAR*)buf_p;
    buf_ucp += offset;
    memcpy(buf_ucp, pInData, size);

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*! @} */
