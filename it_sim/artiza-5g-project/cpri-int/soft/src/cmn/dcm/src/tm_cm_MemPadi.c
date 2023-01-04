/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_cm_MemPadi.c
 *  @brief	データパディング処理I
 *  @date   2008/07/18 FFCS)Tangj create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 
/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  データパディング処理I	
 *  @note   Addressで示される領域に対して、4バイト単位で、IntSize分、 \n
 *          Patternで指定されたデータパターンで更新する。 \n
 *  @param  Address			[in]		パディング対象領域アドレス
 *  @param  Pattern			[in]		パディングパターン
 *  @param  intSize			[in]		パディングサイズ(4byte単位指定)
 *  @param  FileNamep		[in]		コール元関数ファイル名
 *  @param  LineNo			[in]		コール元関数実行ライン
 *  @return None
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2005/11/24		α)T.Hayashi	new create
 *  @date   2013/11/25		α)H.Kondou		zynq化対応
 */
/**************************************************************************/


VOID cmR_MemPadi( UINT*			Address,
				  UINT 			Pattern,
				  UINT			intSize,
				  const VOID* 	FileNamep,
				  UINT			LineNo )

{
	UINT		a_cnt;		/* ループカウンタ	*/
	
	/* ============================================================= */
	/* ■入力パラメータチェック                                      */
	/* ============================================================= */
	if( (Address == NULL) || (intSize == 0 ))
	{
		BPF_COM_LOG_ASSERT (
			D_RRH_LOG_AST_LV_WARNING,
			"Address or Size miscast. Address=0x%08, intSize=0x%08x",
			(UINT)Address, (UINT)intSize);

		return;
	}
	/* ============================================================= */
	/* ■指定アドレス妥当性検証                                      */
	/* ============================================================= */
	if( ((UINT)Address % sizeof(UINT)) != CMD_NUM0 )
	{
		BPF_COM_LOG_ASSERT (
			D_RRH_LOG_AST_LV_WARNING,
			"Boundary error. Address=0x%08, Pattern=0x%08x",
			(UINT)Address, (UINT)Pattern);
	}
	
	/* ============================================================= */
	/* ■パディング処理                                              */
	/* ============================================================= */
	for( a_cnt = CMD_NUM0; a_cnt < intSize; a_cnt++ )
	{
		/* パディング対象領域アドレスにパディングパターンを設定 */
		*(Address + a_cnt) = Pattern;
	}
	
	return;
}
/* @} */
