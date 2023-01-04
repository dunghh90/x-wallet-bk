/*!
 * @skip   $Id$
 * @file   m_dl_rl_save_fw_rsp.c
 * @brief  the processing for "FWファイル更新応答" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update
 * @date   2015/10/27 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"
#include "f_rrh_reg_cnt.h"

/*!
 *  @brief  the processing for "FWファイル更新応答" receiving at the ファイルデータ送信完了報告応答(REC←FHM) 送信待ち state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01914
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/16 FPT)Yen Update comment
 *  @date   2015/10/02 TDIPS)sasaki Update FWファイル更新応答の型誤りを修正
 *  @date   2015/10/17 TDIPS)sasaki Update IT2問処No.97対処
 *  @date   2015/10/17 TDIPS)sasaki Update IT2問処No.104対処
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/10/26 TDIPS)sasaki Update IT2問処No.145対処
 *  @date   2015/10/27 TDIPS)sasaki M-RRU-ZSYS-01914 Update IT2問処No.147水平展開
 *  @date   2015/11/12 TDI)satou その他未実装-006 「配下RE用FLASH-ROM書込失敗」検出処理追加
 */
VOID m_dl_rl_save_fw_rsp(VOID* bufp, USHORT cpri_no)
{
	UINT	ldwResult = ((T_API_FILE_SAVE_FW_RSP *)bufp)->data.dlwResult;
	INT     flashIdx;
	UINT    svRegDat;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_save_fw_rsp] ENTER" );
	
	/* フラッシュ書込フラグをOFF */
	gt_dlw_dlm_tbl.write_flg = CMD_OFF;

	/* FHM file の場合 */
	if (DLD_FILENAME_FHM == gw_dl_FileType)
	{
		/* 更新結果NG */
		if (ldwResult != CMD_RES_OK)
		{
			/* (LTE)(0x2321)ファイルデータ送信完了報告応答(221:ダウンロードファイル異常) */
			m_dl_cm_CRDataEndRspSnd_S3G((USHORT)ldwResult, CMD_SYS_S3G, dlw_prisys_tbl);

			m_dl_cm_DlRetry(CMD_SYS_S3G);

			/* 8: ファイルデータ送信(FW)待ち*/
			gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT;
			
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s gw_dl_FileType[%d], ldwResult[%d], rec_sta[%d], dlw_prisys_tbl[%d], \n",__LINE__,__FUNCTION__,
						gw_dl_FileType,
									ldwResult,
										gt_dlw_rec_tbl.rec_sta,
															dlw_prisys_tbl);
			printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
			#endif
			return;
		}
		/* (LTE)(0x2321)ファイルデータ送信完了報告応答(0:正常) */
		m_dl_cm_CRDataEndRspSnd_S3G((USHORT)ldwResult, CMD_SYS_S3G, dlw_prisys_tbl);
		
		/* 実行中DL開始要因に0:未実行を設定 */
		gt_dlw_dlm_tbl.dlstart_factor	= DLD_FACTOR_NONE;
		
		/* 起動用バージョン情報を更新 */
		gt_dlw_dlm_tbl.mjr_ver = gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver;
		gt_dlw_dlm_tbl.min_ver = gt_dlw_mnt_reinf[CMD_NUM0].min_ver;

		/* ダウンロード終了処理起動 */
		m_dl_cm_DlEnd(CMD_SYS_S3G);

		/* 対REC状態をアイドルへ遷移 */
		gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;

		/* LED設定処理 */
		m_cm_WriteLEDReg();
	}
	/* RE file の場合 */
	else
	{
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s gw_dl_FileType[%d] \n",__LINE__,__FUNCTION__,gw_dl_FileType);
		#endif

		if (ldwResult != CMD_RES_OK)
		{
			/* REC向けファイルデータ送信完了報告応答結果設定 */
			gt_dlw_rec_tbl.filedata_trans_rslt = (USHORT)ldwResult;
			memset(&gt_dlw_mnt_reinf[gt_dlw_dlm_tbl.dlrefile_inf], 0, sizeof(DLT_MTN_REINF));

			/* ALM検出(配下RE用FLASH#1～4) */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "It could not save RE file to FlashROM. dlrefile_inf=%x mk_jdg=%x",
			        gt_dlw_dlm_tbl.dlrefile_inf, gt_dlw_dlm_tbl.mk_jdg);
			flashIdx  = (gt_dlw_dlm_tbl.dlrefile_inf -1) /4;
			svRegDat  = M_RRH_REG_CNT_SV7INF_RE_FLASH(flashIdx);
			rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV7INF, 0, svRegDat);
		}
		if (ldwResult == CMD_RES_OK)
		{
			memcpy(&gt_dlw_mnt_reinf[gt_dlw_dlm_tbl.dlrefile_inf], &gt_dlw_mnt_reinf[CMD_NUM0], sizeof(DLT_MTN_REINF));
		}

		/* DL中REファイル情報初期化 */
		gt_dlw_dlm_tbl.dlrefile_inf = 0xFFFF;
		/* 立ち上げ時REファイル更新終了処理 */
		m_dl_reFileUpdateTermination();
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s write_flg[%d], rec_sta[%d], gw_dl_FileType[%d], dlstart_factor[%d] \n",__LINE__,__FUNCTION__,
					gt_dlw_dlm_tbl.write_flg,
						gt_dlw_rec_tbl.rec_sta, 
											gw_dl_FileType,
													gt_dlw_dlm_tbl.dlstart_factor);
	
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

