/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_Itoa.c
 *  @brief  整数文字列変換処理
 *  @date   2008/07/18 FFCS)Tangj create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/***************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */

/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA 共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  整数文字列変換処理
 *  @note   整数を指定した基数の文字列に変換する。 \n
 *  @param  val				[in]	変換対象の数値
 *  @param  buf_p			[out]	変換後文字列格納用バッファ
 *  @param  size			[in]	変換後文字列格納用バッファサイズ
 *  @param  radix			[in]	基数(2, 8, 10, 16)
 *  @param  chCase			[in]	大文字(0)・小文字(1)
 *  @param  FileNamep		[in]	calling function file name
 *  @param  LineNo			[in]	calling function executing line
 *  @return 書き込み文字列長
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2006/11/21	FJT)T.Nagashima	新規作成 (WCDMA130-0007)
 *  @date   2013/09/05	alpha)Nakajima	zynq化対応
 */
/**************************************************************************/

UINT
cmR_Itoa(
	UINT				val,					/*I	変換対象の数値								*/
	UCHAR				*buf_p,					/*O	変換後文字列格納用バッファ					*/
	UINT				size,					/*I	変換後文字列格納用バッファサイズ			*/
	UINT				radix,					/*I	基数(8, 10, 16)								*/
	UINT				chCase,					/*I	大文字(0)・小文字(1)						*/
	const VOID*			FileNamep,
	UINT				LineNo)
{
	/****************************/
	/*	入力パラメータチェック	*/
	/****************************/
	if( (buf_p == NULL) || (size == 0 ))
	{
		BPF_COM_LOG_ASSERT (
			D_RRH_LOG_AST_LV_WARNING,
			"Address or Size miscast. buf_p=0x%08x, size=0x%08x",
			(UINT)buf_p, (UINT)size);

		return 0;
	}
	
	/* 変換指定子 */
	if(radix == 8)
	{
		return (snprintf((CHAR *)buf_p, size, "%o",val));
	}
	else if(radix == 16)
	{
		if(chCase == 1)
		{
			return (snprintf((CHAR *)buf_p, size, "%x",val));
		}
		else{
			return (snprintf((CHAR *)buf_p, size, "%X",val));
		}
	}else
	{
		return (snprintf((CHAR *)buf_p, size, "%d",val));
	}
}
/* @} */
