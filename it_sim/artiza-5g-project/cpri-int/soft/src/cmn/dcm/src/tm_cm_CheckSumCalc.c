/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_CheckSumCalc.c
 *  @brief  common function - Checksum calculation function
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
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  Checksum calculation 
 *  @note   該当領域の加算を行いサム値を返却する。  \n
 *          アドレス、データ誤り時はアボート処理を実施する \n
 *          ※先頭アドレス／サイズは４バイトバウンダリを \n
 *          考慮した値とすること。 \n
 *          (1) 入力パラメータチェック \n
 *          (2) サム値算出 \n
 *  @param  chkSumAdr_p			[in]	チェックサム先頭アドレス
 *  @param  dataLen				[in]	チェックサムサイズ
 *  @param  chkSum_p			[out]	チェックサム値
 *  @param  FileNamep			[in]	calling function file name
 *  @param  LineNo				[in]	calling function executing line
 *  @return None
 *  @date   2005/12/12	α)M.Takeuchi	 新規作成
 *  @date   2006/03/30	α)M.Oohashi	 WCDMA119-1230
 */
/****************************************************************************/
VOID cmR_CheckSumCalc( const VOID*	chkSumAdr_p,
					   UINT			dataLen,
					   USHORT*		chkSum_p,
					   VOID*		FileNamep,
					   UINT			LineNo )
{
	UINT	a_loopnum;      			/* チェックサム算出ループ回数 	*/
	UINT	a_cnt;						/* ループカウンタ				*/
	USHORT*	a_work_p;					/* アドレスワーク領域			*/
	USHORT  a_chksum_value  = CMD_NUM0; /* CheckSUM値計算格納用         */

	/* input parameter ( 0 Address) check */
	if((chkSumAdr_p == NULL) ||(chkSum_p == NULL))
	{
		return;
	}
	
	/* ===================================================================== */
	/* ■サム値計算処理                                                      */
	/* ===================================================================== */
	a_work_p = (USHORT*)chkSumAdr_p;
	a_loopnum = dataLen / sizeof(USHORT) ;
    
	for( a_cnt=CMD_NUM0 ; a_cnt<a_loopnum ; a_cnt++ )
	{
		/* ポインタを加算し、ブロック単位にデータを加算 */
		a_chksum_value = (USHORT)(a_chksum_value + *(a_work_p + a_cnt));
	}
	
	/* ===================================================================== */
	/* ■CheckSum値を出力パラメータに設定                                    */
	/* ===================================================================== */
	*chkSum_p = a_chksum_value ;

	/* データ長が奇数であるとき */
	if((dataLen % CMD_NUM2) == CMD_NUM1)
	{
		/* チェックサム値計算 */
		(*chkSum_p) += *(UCHAR *)(a_work_p + a_loopnum);
	}

	return;
}
/* @} */
