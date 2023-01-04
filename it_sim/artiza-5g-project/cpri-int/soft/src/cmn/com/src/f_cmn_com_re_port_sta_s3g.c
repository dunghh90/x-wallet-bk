/********************************************************************************/
/*!
 * @skip  $Id:$
 * @file  f_cmn_com_re_port_sta_s3g.c
 * @brief RE PORT状態報告
 * @date  2016/04/19 TDI)satou create
 * @date  2016/05/13 FJT)koshida update 16A開発(Port対応NG時のPort入力状態を0初期化する仕様変更)
 */
/********************************************************************************/
/*!
 * @addtogroup RRH_COM
 * @{
 */
/********************************************************************************/
/* include																		*/
/********************************************************************************/
#include "f_cmn_com_inc.h"
#include "BPF_RU_IPCM.h"

/********************************************************************************/
/* variable																		*/
/********************************************************************************/
/** RE PORT状態報告(LTE)の共有メモリアドレス */
static T_RRH_REPORTSTA_S3G *f_cmn_com_shmRePortStaS3g = NULL;

/********************************************************************************/
/* define																		*/
/********************************************************************************/
#define D_RRH_REPORTSTA_S3G_SEM_TAKE(ret, errcd) \
ret = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_REPORTSTA_S3G ,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &errcd); \
if (BPF_RU_IPCM_OK != ret) \
{ \
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_TAKE() NG. sem_num=%d errcd=0x%08x", E_RRH_SEMID_APL_REPORTSTA_S3G, errcd); \
}

#define D_RRH_REPORTSTA_S3G_SEM_GIVE(ret, errcd) \
ret = BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_REPORTSTA_S3G, BPF_RU_IPCM_LOCK_RW, &errcd); \
if (BPF_RU_IPCM_OK != ret) \
{ \
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSEM_GIVE() NG. sem_num=%d errcd=0x%08x", E_RRH_SEMID_APL_REPORTSTA_S3G, errcd); \
}

/********************************************************************************/
/* prototype																	*/
/********************************************************************************/
static INT f_cmn_com_re_port_sta_shm_addrget(VOID);

/********************************************************************************/
/* function																		*/
/********************************************************************************/
/*!
 * @brief   RE PORT状態報告(LTE)の共有メモリアドレスを取得し、大域変数に保存する
 * @note	既に共有メモリのアドレス取得済みであれば何もしない
 * @return  処理結果
 * @date    2016/04/19 TDI)satou create
 */
static INT f_cmn_com_re_port_sta_shm_addrget(VOID)
{
	INT ret;
	INT errcd;

	if (NULL != f_cmn_com_shmRePortStaS3g)
	{	/* 既に共有メモリのアドレス取得済みであれば何もしない */
		return D_RRH_OK;
	}

	/* 共有メモリのアドレス取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_REPORTSTA_S3G, (VOID**)&f_cmn_com_shmRePortStaS3g, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", E_RRH_SHMID_APL_REPORTSTA_S3G, errcd);
        return D_RRH_NG;
    }

    return D_RRH_OK;
}

/*!
 * @brief   FHM-MTからPORT入力状態の設定値を共有メモリとEEPROMに反映する
 * @note
 * @param   linkno		[in]	CPRIリンク番号
 * @param   setting		[in]	設定値
 * @return  処理結果
 * @date    2016/04/19 TDI)satou create
 */
INT f_cmn_com_portInputSetting_s3g(T_RRH_REPORTSTA_S3G *setting)
{
	INT				ret;
	INT				errcd;
	USHORT			link_idx;
	USHORT			alloc_idx;
	INT				alloc_en_old;
	INT				alloc_en_new;

	/********************************************************
	 * 変数初期化
	 ********************************************************/
	ret = f_cmn_com_re_port_sta_shm_addrget();
	if (D_RRH_OK != ret)
	{	/* 共有メモリのアドレス取得 */
		return D_RRH_NG;
	}

	/********************************************************
	 * 共有メモリ更新
	 ********************************************************/
	/* 排他制御開始 */
	D_RRH_REPORTSTA_S3G_SEM_TAKE(ret, errcd);

	/* CPRIリンクの数だけループ */
	for (link_idx = 0; link_idx < D_RRH_CPRINO_RE_MAX; link_idx++)
	{
		alloc_en_old = alloc_en_new = D_RRH_OFF;

		/* 現在の設定値について割り当てがあるのか確認する. 後の処理の「割り当て無し」から「割り当て有り」に変更された場合の比較処理のため */
		for (alloc_idx = 0; alloc_idx < 16; alloc_idx++)
		{
			if ((link_idx+1) == f_cmn_com_shmRePortStaS3g->cpri_no[alloc_idx])
			{
				alloc_en_old = D_RRH_ON;
				break;
			}
		}

		/* 新しい設定値について割り当てがあるのか確認する. 後の処理の「割り当て無し」から「割り当て有り」に変更された場合の比較処理のため */
		for (alloc_idx = 0; alloc_idx < 16; alloc_idx++)
		{
			if ((link_idx+1) == setting->cpri_no[alloc_idx])
			{
				alloc_en_new = D_RRH_ON;
				break;
			}
		}

		if ((D_RRH_OFF == alloc_en_old) && (D_RRH_ON == alloc_en_new))
		{	/* 該当REのPORT入力状態の全bitに対して「割り当て無し」から「割り当て有り」に設定された場合、他メンバー変数を初期化する.
		 	 * 「割り当て有り」から「割り当て有り」(変化なし)の場合、前値保持なので他メンバー変数の初期化はしない.
		 	 */
			f_cmn_com_shmRePortStaS3g->received_rsp[link_idx] = D_RRH_REPORTSTA_RSP_NOT_RECEIVED;
			f_cmn_com_shmRePortStaS3g->rsp_result[link_idx]   = 0;
			f_cmn_com_shmRePortStaS3g->rsp_portin[link_idx]   = 0;
		}

		f_cmn_com_shmRePortStaS3g->alloc_en[link_idx] = alloc_en_new;
	}

	/* 設定値を保存 */
	for (alloc_idx = 0; alloc_idx < 16; alloc_idx++)
	{
		f_cmn_com_shmRePortStaS3g->cpri_no[alloc_idx] = setting->cpri_no[alloc_idx];
		f_cmn_com_shmRePortStaS3g->bit_no [alloc_idx] = setting->bit_no [alloc_idx];
	}

    D_RRH_REPORTSTA_S3G_SEM_GIVE(ret, errcd);
    /* 排他制御終了 */

    return D_RRH_OK;
}

/*!
 * @brief   配下REからのRE PORT状態報告応答の内容を共有メモリに設定する
 * @note	未受信の場合は応答結果とPORT入力状態を初期化
 * @note	正常応答の場合は応答結果とPORT入力状態を更新
 * @note	NG応答の場合は応答結果の部分だけ更新し、PORT入力状態は前置保持
 * @note	タイムアウトの場合は応答結果の部分だけ更新し、PORT入力状態は前置保持
 * @param   linkno			[in]	CPRIリンク番号.
 * @param   received_rsp	[in]	REからのRE PORT状態報告応答の受信状態.
 * 									- D_RRH_REPORTSTA_RSP_NOT_RECEIVED
 * 									- D_RRH_REPORTSTA_RSP_NORMAL
 * 									- D_RRH_REPORTSTA_RSP_NG
 * 									- D_RRH_REPORTSTA_RSP_TO
 * @param	rsp_result		[in]	RE PORT状態報告応答の応答結果.
 * 									※received_rspが D_RRH_REPORTSTA_RSP_TO の場合、Don't Care
 * @param	rsp_portin		[in]	RE PORT状態報告応答のPORT入力状態.
 * 									※received_rspが D_RRH_REPORTSTA_RSP_NORMAL 以外の場合、Don't Care
 * @return  処理結果
 * @date    2016/04/19 TDI)satou create
 */
INT f_cmn_com_RePortSta_s3g_set(USHORT linkno, UCHAR received_rsp, USHORT rsp_result, USHORT rsp_portin)
{
	INT				ret;
	INT				errcd;
	USHORT			link_idx		= linkno - 1;

	/********************************************************
	 * 変数初期化
	 ********************************************************/
	ret = f_cmn_com_re_port_sta_shm_addrget();
	if (D_RRH_OK != ret)
	{	/* 共有メモリのアドレス取得 */
		return D_RRH_NG;
	}

	/********************************************************
	 * 共有メモリ更新
	 ********************************************************/
	/* 排他制御開始 */
	D_RRH_REPORTSTA_S3G_SEM_TAKE(ret, errcd);

	/*	受信状態を更新する	*/
	f_cmn_com_shmRePortStaS3g->received_rsp[link_idx] = received_rsp;
	
	switch (received_rsp)
	{
	case D_RRH_REPORTSTA_RSP_NOT_RECEIVED:
		f_cmn_com_shmRePortStaS3g->rsp_result[link_idx] = 0;
		f_cmn_com_shmRePortStaS3g->rsp_portin[link_idx] = 0;
		break;
	case D_RRH_REPORTSTA_RSP_NORMAL:
		f_cmn_com_shmRePortStaS3g->rsp_result[link_idx] = rsp_result;
		f_cmn_com_shmRePortStaS3g->rsp_portin[link_idx] = rsp_portin;
		break;
	case D_RRH_REPORTSTA_RSP_NG:
		f_cmn_com_shmRePortStaS3g->rsp_result[link_idx] = rsp_result;
		f_cmn_com_shmRePortStaS3g->rsp_portin[link_idx] = 0;
		break;
	case D_RRH_REPORTSTA_RSP_TO:
	case D_RRH_REPORTSTA_RSP_CPRIDSC:
		f_cmn_com_shmRePortStaS3g->rsp_result[link_idx] = 0;
		f_cmn_com_shmRePortStaS3g->rsp_portin[link_idx] = 0;
		break;
	default:
		break;
	}

    D_RRH_REPORTSTA_S3G_SEM_GIVE(ret, errcd);
    /* 排他制御終了 */

    return D_RRH_OK;
}

/*!
 * @brief   RE PORT状態関連の情報を共有メモリからまとめて取得する
 * @param   pOutData	[out]	取得先. 取得元でD_RRH_CPRINO_RE_MAXだけ領域を確保すること.
 * @return  処理結果
 * @date    2016/04/19 TDI)satou create
 */
INT f_cmn_com_RePortSta_s3g_allget(T_RRH_REPORTSTA_S3G *pOutData)
{
	INT ret, errcd;

	/********************************************************
	 * 変数初期化
	 ********************************************************/
	ret = f_cmn_com_re_port_sta_shm_addrget();
	if (D_RRH_OK != ret)
	{	/* 共有メモリのアドレス取得 */
		return D_RRH_NG;
	}

	/* 排他制御開始 */
	D_RRH_REPORTSTA_S3G_SEM_TAKE(ret, errcd);
	*pOutData = *f_cmn_com_shmRePortStaS3g;
    D_RRH_REPORTSTA_S3G_SEM_GIVE(ret, errcd);
    /* 排他制御終了 */

    return D_RRH_OK;
}
/*! @} */
