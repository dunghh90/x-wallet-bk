/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_AutoResetTime_chk.c
 * @brief  RE 自律リセット時刻チェック
 * @date   2010/06/20 FJT)Tokunaga create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-2011
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE 自律リセット時刻チェック
 * @note   RE 自律リセット時刻チェック
 * 
 * 
 * @return 自律リセット要/不要
 * @retval CMD_ON:要
 * @retval CMD_OFF:不要
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2010/11/17 CMS)Kudou M-S3G-eNBPF-03898
 * @date   2011/11/16 FJT)K.Koshida D-MICRO-008 3002形対応(RE自律リセット,H/S inf変更)
 * @date   2012/04/28 CMS)N.Uchida  M-DCM-SeNBPF-00511
 * @date   2015/07/08 FPT)Quynh modify
 * @date   2015/11/17 FPT)Tuan fix bug.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_AutoResetTime_chk_3g()
{
	struct bpf_timeval	rstBaseTim;
	LONG				resetPastTime =0;
	LONG				AlmPastCheckTime;
	USHORT				rtn = CMD_OFF;
	//INT					ret;
	//INT					errcode;
	CHAR				errdata[CMD_NUM48];
	//UINT				logDat;

	//UINT				uit_ret;		/*	共通関数復帰値					*/
	//UINT				regaddr;		/*	RE自律リセットカウンタレジスタアドレス	*/
	//UINT				red_val;		/*	RE自律リセットカウンタレジスタ読出値	*/
	//USHORT				rtn;			/*	関数復帰値						*/
	//CHAR				errdata[CMD_NUM48];
	USHORT cpri_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;             /* CPRI番号インデックス */
	
	BPF_RU_UTTM_GETTIME(&rstBaseTim);

#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( " [%d] %s Start\n", __LINE__, __FUNCTION__ );
#endif
	/* Semaphore (Lock)	*/
	//for(;;) {
	//	ret = BPF_RU_IPCM_PROCSEM_TAKE( 1/*D_SEM_CPU_PROC_AUTORESET*/, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, 0, &errcode);
	//	if( ret == BPF_RU_IPCM_EINTR )
	//	{
	//		continue;
	//	}
	//	break;
	//}
	//if (ret != BPF_RU_IPCM_OK)
	//{
		/* 無効処理履歴 */
	//	snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d]SemaphoreTake NG",D_REC_C02_CPR_NO());
	//	D_REC_IVLINI();
	//	D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		/* RE自律リセットしない */
	//	return(CMD_OFF);
	//}

	/* RE自律リセット実施時刻が保持されていない */
	//if( f_trp_com_AutoResetMngTbl[lwCprNo].resetTime == D_TCM_DEFAULT_RSTTIM)
	//{
	//	rtn = CMD_ON;
	//	snprintf(errdata, CMD_NUM48, "[NTC]RE ALM TIME ON");
	//	D_REC_IVLINI();
	//	logDat = D_REC_C02_CPR_NO();
	//	D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
	//	logDat = rstBaseTim.tv_sec;
	//	D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
	//	logDat = table_pos;
	//	D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
	//	D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
	//}
	//else
	//{
		if( f_trp_com_AutoResetMngTbl_3g[cpri_idx].resetTime <= rstBaseTim.tv_sec )
		{
			resetPastTime = rstBaseTim.tv_sec - f_trp_com_AutoResetMngTbl_3g[cpri_idx].resetTime;
			
			if(RE_AlarmProtectTime_Debug == CMD_ON){
				AlmPastCheckTime = RE_AlarmProtectTime;
			}
			else{
				AlmPastCheckTime = D_TCM_ALM_PROTECT_TIM;
			}
			
			/* RE自律リセット実施時刻から48時間経過している */
			if(resetPastTime >= AlmPastCheckTime)
			{
				rtn = CMD_ON;
			}
			else
			{
				rtn = CMD_OFF;
			}
		}
		else
		{
			/* 無効処理履歴 */
			snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]ALM Watch Time Data Error",D_REC_C02_3G_CPR_NO());
			D_REC_IVLINI();
			D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		}
	//}

	/* Semaphore (unLock) */
	//ret = BPF_RU_IPCM_PROCSEM_GIVE( 1/*D_SEM_CPU_PROC_AUTORESET*/, BPF_RU_IPCM_LOCK_RW, &errcode);
	//if (ret != BPF_RU_IPCM_OK)
	//{
		/* 無効処理履歴 */
	//	snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d]SemaphoreGive NG",D_REC_C02_CPR_NO());
	//	D_REC_IVLINI();
	//	D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
	//}
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( " [%d] %s End\n", __LINE__, __FUNCTION__ );
#endif
	return( rtn );
}
/** @} */
