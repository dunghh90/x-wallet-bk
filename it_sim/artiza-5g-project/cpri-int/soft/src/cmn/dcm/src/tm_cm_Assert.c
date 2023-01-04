/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_Assert.c
 *  @brief  common function - Assert output function
 *  @date   2008/07/18 FFCS)Tangj create
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
#include "m_cm_header.h"			/* TRIF/MDIF共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  アサート出力処理
 *  @note   デバッグ／保守運用を目的として動作履歴専用のログを残す。 \n
 *          (1)アサートログ管理情報.出力レベルを判定 \n
 *          (2)アサートログ取得 \n
 *  @param  Level			[in]	writing level 
 *  @param  UserCode		[in]	user code
 *  @param  Message_p		[in]	Message pointer ( Max 32 byte)
 *  @param  FileNamep		[in]	calling function executing line
 *  @param  LineNo			[in]	calling function file name
 *  @return None
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2005/11/24	 α)T.Hayashi	新規作成
 *  @date   2006/03/23	 FJT)T.Nagashima	WCDMA119-1110 \n
 *                       Assert/Abortの収集ファイル名不備
 *  @date   2013/12/10	 α)H.Kondou	zynq化対応
 */
/**************************************************************************/

VOID cmR_Assert( UINT			Level,
				 UINT			UserCode,
				 const VOID*	Message_p,
				 const VOID* 	FileNamep,
				 UINT			LineNo	 )
{
	f_cmn_com_assert(Level, UserCode,Message_p, FileNamep , LineNo);
	return;
}
/* @} */
