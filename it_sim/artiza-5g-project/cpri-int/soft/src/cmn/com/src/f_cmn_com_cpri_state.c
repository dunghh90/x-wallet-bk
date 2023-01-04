/*!
 * @skip  $ld:$
 * @file  f_cmn_com_cpri_state.c
 * @brief 共有メモリ取得/設定(CPRI State)
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   CPRI State取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_cpri_state_get(T_RRH_CPRI_STATE *pOutData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CPRI_STATE;    /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CPRI_STATE;    /* セマフォナンバー   */

    return f_cmn_com_shm_get(shm_num, sem_num, sizeof(*pOutData), pOutData);
}

/*!
 * @brief   CPRI State設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_cpri_state_set(T_RRH_CPRI_STATE *pInData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CPRI_STATE;    /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CPRI_STATE;    /* セマフォナンバー   */

    return f_cmn_com_shm_set(shm_num, sem_num, sizeof(*pInData), pInData);
}

/*!
 * @brief   CPRI State設定(CPRI state, HDLC ACT/IDLE, CPRI bitrate, CPRI接続構成の更新)
 * @note    L3_CPRから実行されることを想定
 * @param   link_num        [in]    CPRIリンク番号
 * @param   cpriState       [in]    CPRI State
 * @param   cpriHdlc        [in]    HDLC IDLE(0)/HDLC ACT(1)
 * @param   lineBitrate     [in]    ビットレート
 * @param   cpri_linktype   [in]    接続構成。RECの場合のみ使用(CMD_CONN_S3G_SINGLE/CMD_CONN_CPRI_SHARED)
 * @return  処理結果
 * @date    2015/08/21 TDI)satou create
 */
INT f_cmn_com_cpri_state_set_cprist(USHORT link_num, USHORT cpriState, USHORT cpriHdlc, USHORT lineBitrate, UINT cpri_linktype) {
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CPRI_STATE;    /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CPRI_STATE;    /* セマフォナンバー   */
    INT ret;                                            /* 戻り値評価用 */
    INT errcd;                                          /* BPFエラーコード */
    T_RRH_CPRI_STATE *buf_p;

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
    if (D_RRH_CPRINO_REC == link_num) {
        buf_p->cprista_rec   = cpriState;
        buf_p->hdlcst_rec    = cpriHdlc;
        buf_p->cpri_bitrate  = lineBitrate;
        buf_p->cpri_linktype = cpri_linktype;
    } else {
        buf_p->cprista_re[link_num - 1] = cpriState;
        buf_p->hdlcst_re [link_num - 1] = cpriHdlc;
    }

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*!
 * @brief   CPRI State設定(ヘルスチェック状態の更新)
 * @note    L3_CPR m_cr_CpriStateChgProc(), m_cr_CpriStateChgProcRE()から実行されることを想定
 * @param   link_num    [in]    CPRIリンク番号
 * @param   link_type   [in]    リンク種別(CMD_SYS_3G/CMD_SYS_S3G)
 * @param   hcst        [in]    ヘルスチェック状態(D_RRH_OFF:未確認/D_RRH_ON:確認)
 * @return  処理結果
 * @date    2015/08/21 TDI)satou create
 */
INT f_cmn_com_cpri_state_set_hc(USHORT link_num, USHORT link_type, USHORT hcst) {
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CPRI_STATE;    /* 共有メモリナンバー */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CPRI_STATE;    /* セマフォナンバー   */
    INT ret;                                            /* 戻り値評価用 */
    INT errcd;                                          /* BPFエラーコード */
    T_RRH_CPRI_STATE *buf_p;

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
    if (D_RRH_CPRINO_REC == link_num) {
        buf_p->healthchk_rec[link_type] = hcst;
    } else {
        buf_p->healthchk_re[link_type][link_num - 1] = hcst;
    }

    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    return D_RRH_OK;
}

/*! @} */
