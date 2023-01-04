/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_CheckSumCalcCpri.c
 *  @brief  common function - Checksum calculation for CPRI signal function
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
#include "m_cm_header.h"			/* TRIF/MDIF共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  チェックサム計算処理(CPRI信号用) 
 *  @note   該当領域の加算を行いサム値を返却する。  \n
 *          アドレス、データ誤り時はアボート処理を実施する \n
 *          ※先頭アドレス／サイズは４バイトバウンダリを \n
 *          考慮した値とすること。 \n
 * 
 *          (1) 入力パラメータチェック \n
 *          (2) サム値算出 \n
 *
 *  @param  chkSumAdr_p			[in]	チェックサム先頭アドレス
 *  @param  dataLen				[in]	チェックサムサイズ
 *  @param  chkSum_p			[out]	チェックサム値
 *  @param  FileNamep			[in]	calling function file name
 *  @param  LineNo				[in]	calling function executing line
 *  @return None
 *  @date   2006/02/07	α)T.Saito	 新規作成	WCDMA119-0476対処済
 *  @date   2006/03/22	α)A.Etou	 WCDMA119-1099対応
 *  @date   2007/02/19	FJT)H.Sujihara	 WCDMA130-106対応
 *									１の補数和計算処理修正 
 *  @date   2009/11/10	QNET)Kabasima M-S3G-eNBSYS-00764
 *									データ長が奇数時のバインディング(0x00付加)処理追加
 */
/****************************************************************************/

VOID cmR_CheckSumCalcCpri( const VOID*	chkSumAdr_p,
						   UINT			dataLen,
						   USHORT*		chkSum_p,
						   VOID*		FileNamep,
						   UINT			LineNo )
{
	UINT	a_loopnum;      			/* チェックサム算出ループ回数 	*/
	UINT	a_cnt;						/* ループカウンタ				*/
	USHORT*	a_work_p;					/* アドレスワーク領域			*/
	UINT    a_chksum_value  = CMD_NUM0; /* CheckSUM値計算格納用         */
	USHORT  a_chksum_value2 = CMD_NUM0; /* CheckSUM値計算格納用         */
	USHORT  temp = CMD_NUM0;
	
	/* input parameter ( 0 Address) check */
	if(((INT)chkSumAdr_p == CMD_NUM0)||((INT)chkSum_p == CMD_NUM0))
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
		/* ファイルデータを16bits毎に1の補数和を取る。 	*/
		a_chksum_value = a_chksum_value + *(a_work_p + a_cnt);

		if(a_chksum_value > CMD_CHECKSUM_VALUE)
		{
			/* 桁上がりした場合 */
			a_chksum_value2 = (USHORT)a_chksum_value;
			/* 最下位ビットに+1 */
			a_chksum_value2 ++;
			a_chksum_value = (UINT)a_chksum_value2;
	
			if(a_chksum_value == CMD_NUM0)
			{
				/* 最下位ビットに+1したものが更に桁上がりした場合 */
				a_chksum_value ++;
			}
		}
	}
	
	/* データ長が奇数であるとき */
	if((dataLen % CMD_NUM2) == CMD_NUM1)
	{
		/* チェックサム値計算 */
		a_chksum_value = 
					a_chksum_value + (USHORT)((*(a_work_p + a_loopnum)) & CMD_MSK_00FF);

		if(a_chksum_value > CMD_CHECKSUM_VALUE)
		{
			/* 桁上がりした場合 */
			a_chksum_value2 = (USHORT)a_chksum_value;
			/* 最下位ビットに+1 */
			a_chksum_value2 ++;
			a_chksum_value = (UINT)a_chksum_value2;
	
			if(a_chksum_value == CMD_NUM0)
			{
				/* 最下位ビットに+1したものが更に桁上がりした場合 */
				a_chksum_value ++;
			}
		}
	}
	
	/* ===================================================================== */
	/* ■CheckSum値を出力パラメータに設定                                    */
	/* ===================================================================== */
	/* さらに上記計算結果の1の補数を取り、これをチェックサムとする */
	*chkSum_p = ~(USHORT)a_chksum_value ;

	/* swap the checksum */
	temp = *chkSum_p;
	*chkSum_p = ((temp & 0xff00) >> 8) | ((temp & 0x00ff) << 8);

	return;
}
/* @} */
