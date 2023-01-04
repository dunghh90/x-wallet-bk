/*!
 * @skip  $ld:$
 * @file  f_cmn_com_cardst_s3g.c
 * @brief 共有メモリ取得/設定(REカード状態(S3G))
 * @date  2015/08/14 TDI)uchida create
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/*!
 * @brief   REカード状態(S3G)取得
 * @note    f_cmn_com_shm_get()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   linkno      [in]    CPRIリンク番号
 * @param   pOutData    [in]    取得先
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_cardst_s3g_get(USHORT linkno, T_RRH_CARDST_S3G *pOutData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CARDST_S3G;    /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CARDST_S3G;    /* セマフォナンバー     */
	UINT lnkOfs;
    UINT size;
    INT  ret;
    INT errcd;      /* BPFエラーコード */
    T_RRH_CARDST_S3G* buf_p;

    if (NULL == pOutData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:NULL pointer");
        return D_RRH_NG;
    }
    if((linkno < 1) ||(linkno > 17)){
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:linkNo(%d) NG", linkno);
        return D_RRH_NG;
    }
    /* 共有メモリのアドレス取得 */
    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(shm_num ,(VOID**)&buf_p, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", shm_num, errcd);
        return D_RRH_NG;
    }
    /*	該当リンクの先頭に移動	*/
    lnkOfs = linkno - 1;
    buf_p += lnkOfs;
    
    /* セマフォ取得 */
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /*	固定長領域を読出する	*/
    size = 32;
    memcpy(pOutData, buf_p, size);
    
    /*	初期化以外の場合	*/
    if(buf_p->signal_kind != 0)
    {
	    /*	機能部情報領域を読出する	*/
	    size = (sizeof(buf_p->func_every[0]) * (buf_p->func_num));
	    memcpy(&pOutData->func_every[0], &buf_p->func_every[0], size);
	    /*	ALM数を読出する	*/
	    if(buf_p->alm_num > CMD_CRDALM_INFNUMMAX_HIG)
	    {
	        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "alm_num err=%d", buf_p->alm_num);
			buf_p->alm_num = CMD_CRDALM_INFNUMMAX_HIG;
		}
	    pOutData->alm_num = buf_p->alm_num;
	    /*	ALM領域を読出する	*/
	    if(buf_p->alm_num != 0)
	    {
	        size = (buf_p->alm_num * 2);
	        memcpy(&pOutData->alm_inf[0], &buf_p->alm_inf[0], size);
	    }
	    /*	ERR数を読出する	*/
	    if(buf_p->err_num > CMD_CRDERR_INFNUMMAX_HIG)
	    {
	        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "err_num err=%d", buf_p->err_num);
			buf_p->err_num = CMD_CRDERR_INFNUMMAX_HIG;
		}
	    pOutData->err_num = buf_p->err_num;
	    /*	ERR領域を読出する	*/
	    if(buf_p->err_num != 0)
	    {
	        size = (buf_p->err_num * 2);
	        memcpy(&pOutData->err_inf[0], &buf_p->err_inf[0], size);
	    }
	}
	else
	{
		pOutData->func_num = 0;
		pOutData->alm_num = 0;
		pOutData->err_num = 0;
	}
    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }
    return D_RRH_OK;
}

/*!
 * @brief   REカード状態(S3G)設定
 * @note    f_cmn_com_shm_set()のWrapper. Grepした時に呼び出し箇所がわかりやすくなるように
 * @param   linkno      [in]    CPRIリンク番号
 * @param   pInData     [in]    設定元
 * @return  処理結果
 * @retval  D_RRH_OK
 * @retval  D_RRH_NG
 * @date    2015/08/14 TDI)uchida create
 * @date    2015/08/17 TDI)satou 共通の関数をコールするように見直し
 */
INT f_cmn_com_cardst_s3g_set(USHORT linkno, T_RRH_CARDST_S3G *pInData)
{
    UINT shm_num = (UINT)E_RRH_SHMID_APL_CARDST_S3G;    /* 共有メモリナンバー   */
    UINT sem_num = (UINT)E_RRH_SEMID_APL_CARDST_S3G;    /* セマフォナンバー     */
	UINT lnkOfs;
    UINT size;
    INT  ret;
    INT errcd;      /* BPFエラーコード */
    T_RRH_CARDST_S3G* buf_p;

    if (NULL == pInData) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:NULL pointer");
        return D_RRH_NG;
    }
    if((linkno < 1) ||(linkno > 17)){
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "parameter error:linkNo(%d) NG", linkno);
        return D_RRH_NG;
    }
    /* 共有メモリのアドレス取得 */
    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(shm_num ,(VOID**)&buf_p, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", shm_num, errcd);
        return D_RRH_NG;
    }
    /*	該当リンクの先頭に移動	*/
    lnkOfs = linkno - 1;
    buf_p += lnkOfs;
    
    /* セマフォ取得 */
    ret = BPF_RU_IPCM_PROCSEM_TAKE(sem_num, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }

    /*	固定長領域を設定する	*/
    size = 32;
    memcpy(buf_p, pInData, size);
    
    /*	初期化以外の場合	*/
    if(pInData->signal_kind != 0)
    {
	    /*	機能部情報領域を設定する	*/
	    size = (sizeof(pInData->func_every[0]) * (pInData->func_num));
	    memcpy(&buf_p->func_every[0], &pInData->func_every[0], size);
	    /*	ALM数を設定する	*/
	    if(pInData->alm_num > CMD_CRDALM_INFNUMMAX_HIG)
	    {
	        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "alm_num err=%d", buf_p->alm_num);
			pInData->alm_num = CMD_CRDALM_INFNUMMAX_HIG;
		}
	    buf_p->alm_num = pInData->alm_num;
	    /*	ALM領域を設定する	*/
	    if(pInData->alm_num != 0)
	    {
	        size = (pInData->alm_num * 2);
	        memcpy(&buf_p->alm_inf[0], &pInData->alm_inf[0], size);
	    }
	    /*	ERR数を設定する	*/
	    if(pInData->err_num > CMD_CRDERR_INFNUMMAX_HIG)
	    {
	        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "err_num err=%d", buf_p->err_num);
			pInData->err_num = CMD_CRDERR_INFNUMMAX_HIG;
		}
	    buf_p->err_num = pInData->err_num;
	    /*	ERR領域を設定する	*/
	    if(pInData->err_num != 0)
	    {
	        size = (pInData->err_num * 2);
	        memcpy(&buf_p->err_inf[0], &pInData->err_inf[0], size);
	    }
	}
	else
	{
		buf_p->func_num = 0;
		buf_p->alm_num = 0;
		buf_p->err_num = 0;
	}
    /* セマフォ解放 */
    ret = BPF_RU_IPCM_PROCSEM_GIVE(sem_num, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", sem_num, errcd);
    }
    return D_RRH_OK;
}

/*! @} */
