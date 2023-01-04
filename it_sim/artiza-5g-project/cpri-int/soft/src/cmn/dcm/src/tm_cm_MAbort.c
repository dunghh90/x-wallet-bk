/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_MAbort.c
 *  @brief  Abort processing
 *  @date   2008/07/29 FFCS)Tangj create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBSYS-00828 ST USE状態のTRAへALM制御した時の前面LED表示
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2013
 */
/***************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */

/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  アボート処理 
 *  @note   指定パラメータの内容／その他デバッグログを取得し、  \n
 *          システムダウン(オフラインデバッガへの移行)を行う。 \n
 *          ※以後、全タスクの動作停止する。 \n
 *          "リセット以外での復旧は不可 \n
 * 
 *          (1) アボートログ設定 \n
 *          (2) アボート処理 \n
 *
 *  @param  AlmCode		            [in]	ALM要因
 *  @param  Message1_p  			[in]	メッセージ
 *  @param  Message2_p				[in]	メッセージ(最大255文字)
 *  @param  userCode1				[in]	ユーザー任意指定コード
 *  @param  userCode2				[in]	ユーザー任意指定コード
 *  @param  userCode3				[in]	ユーザー任意指定コード
 *  @param  FileNamep				[in]	calling function file name
 *  @param  LineNo					[in]	calling function executing line
 *  @return None
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2008/07/29 FFCS)Tangj create based on MB case	
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBSYS-00828 ST USE状態のTRAへALM制御した時の前面LED表示
 *  @date   2009/11/18 FJT)Taniguchi M-S3G-eNBPF-02875/M-S3G-eNBPF-02876
 *  @date   2013/12/10 α)H.Kondou	 zynq化対応
 */
/****************************************************************************/

VOID cmR_MAbort( UINT			AlmCode,
				 const VOID*	Message1_p,
				 const VOID*	Message2_p,
				 UINT			userCode1,
				 UINT			userCode2,
				 UINT			userCode3,
				 const VOID*	FileNamep,
				 UINT			LineNo )
{
	bpf_com_log_assert_real(D_RRH_LOG_AST_LV_CRITICAL,
		(const char* )FileNamep,
		(int)LineNo,
		"%s,%s,0x%08x,0x%08x",
		Message1_p,Message2_p,userCode1, userCode2);

	f_cmn_com_abort( userCode3, AlmCode, FileNamep, LineNo);

	return;
}

/* @} */
