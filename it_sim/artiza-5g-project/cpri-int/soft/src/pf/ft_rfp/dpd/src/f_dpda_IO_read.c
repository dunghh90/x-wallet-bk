/*!
 * @skip  $ld:$
 * @file  f_dpda_IO_read.c
 * @brief DPDAタスク レジスタ値取得処理
 * @date  2011/10/04 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2012
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:レジスタ値取得処理
 * @note  関数処理内容.
 *		-# デバッグフラグがOFFの場合以下の処理を行う
 *		-# レジスタアドレスがCPLDの場合
 *			-# SPIを使用してレジスタ値取得を行う
 *		-# レジスタアドレスがCPLD以外の場合
 *			-# レジスタ値取得を行う
 *		-# デバッグフラグがONの場合以下の処理を行う
 *			-# 内部テーブルの値の取得を行う
 *			-# 内部テーブルの値が無い場合はレジスタ値取得を行う
 *				-# レジスタアドレスがCPLDの場合
 *					-# SPIを使用してレジスタ値取得を行う
 *				-# レジスタアドレスがCPLD以外の場合
 *					-# レジスタ値取得を行う
 *
 *		-# 取得した値をコール元に返す
 * @param  uiRegAddr			[in]UINT	レジスタアドレス
 * @param  uiReadMode			[in]UINT	読み込みモード( 通常:0, SPI(CPLD):3, SPI(FEFB):4, SPI(RX):5, EEPROM:6 )
 * @param  uiLogLvl				[in]UINT	ログレベル(SPI経由、EEPROMアクセスの場合は0を指定)
 * @return UINT								レジスタ値
 * @date 2011/10/04 ALPHA)横山 Create
 * @date 2013/04/03 ALPHA)中村 B25対応
 * @date 2013/11/28 ALPHA)加藤 ZYNQ対応
 * @date 2015/11/25 TDIPS)榎   温度監視のI2C-readをコメント化
 * @date 2015/12/02 FJT)原田   温度監視のI2C-readを首空け
 */
UINT f_dpda_IO_read(UINT uiRegAddr, UINT uiReadMode, UINT uiLogLvl)
{
	UINT	uiRegVal = 0;
	UINT	uiRegCnt = 0;
	UINT	uiRegOffset;
	UINT	uiSpiFlg;
	INT		ret = BPF_HM_DEVC_COMPLETE;
	
	if( D_SYS_OFF == f_dpdaw_info_all->taskInfo.swDbgFlg.virtualRegisterFlg )
	{
		switch( uiReadMode )
		{
			case E_DPDA_BPF_NOMAL_MODE:
				ret = BPF_HM_DEVC_REG_READ_NOMUTEX(
							uiLogLvl,					/**< ログレベル						*/
							uiRegAddr,					/**< レジスタ読み込みオフセット		*/
							&uiRegVal);					/**< 読み込みポインタ				*/
				break;
			case E_DPDA_BPF_TMP_MODE:
				ret = BPF_HM_DEVC_TEMP_READ( (INT *)&uiRegVal );
				break;
			default:
				/* assertログ取得 */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "uiReadMode invalid [%d]", uiReadMode);
				break;
		}
	}
	else
	{
		uiRegOffset = 0;
		uiSpiFlg	= D_SYS_OFF;
		switch( uiReadMode )
		{
			case E_DPDA_BPF_NOMAL_MODE:
				uiRegOffset = 0;
				uiSpiFlg	= D_SYS_OFF;
				break;
			case E_DPDA_BPF_TMP_MODE:
				uiRegOffset	= D_DPDA_DBG_VREG_ADDR_OFFSET_TEMP;
				uiSpiFlg	= D_SYS_ON;
				break;
			default:
				/* assertログ取得 */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "uiReadMode invalid [%d]", uiReadMode);
				break;
		}
		/* デバッグフラグがONの場合はテーブルから値を取得する	*/
		for( uiRegCnt = 0; uiRegCnt < D_DPDA_DBG_VIRTUAL_REG_CNT; uiRegCnt++ )
		{
			if(		( uiSpiFlg == D_SYS_OFF )
				&&	( f_dpdar_virtualRegAddr[uiRegCnt] <  D_DPDA_DBG_VREG_SPI_CHK ) )
			{
				continue;
			}
			
			if( (uiRegAddr + uiRegOffset) == f_dpdar_virtualRegAddr[uiRegCnt] )
			{
				break;
			}
		}
		
		if( uiRegCnt != D_DPDA_DBG_VIRTUAL_REG_CNT )
		{
			/* Virtual Registerが見つかった場合は				*/
			/* テーブルから値を取得する							*/
			uiRegVal = f_dpdaw_info_all->virtualRegData[ uiRegCnt ];
		}
		else
		{
			/* デバッグフラグがONであってもVirtual Registerが	*/
			/* 見つからない場合は本物のレジスタから値を取得する	*/
			switch( uiReadMode )
			{
				case E_DPDA_BPF_NOMAL_MODE:
					ret = BPF_HM_DEVC_REG_READ(
								D_RRH_LOG_REG_LV_READ,		/**< ログレベル						*/
								uiRegAddr,					/**< レジスタ読み込みオフセット		*/
								&uiRegVal);					/**< 読み込みポインタ				*/
					break;
				case E_DPDA_BPF_TMP_MODE:
					ret = BPF_HM_DEVC_TEMP_READ( (INT *)&uiRegVal );
					break;
				default:
					/* assertログ取得 */
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "uiReadMode invalid [%d]", uiReadMode);
					break;
			}
		}
	}
	
	if( BPF_HM_DEVC_COMPLETE != ret )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Reg Read NG(ret=%d(%d,%d,%d) ]", ret, uiRegAddr, uiReadMode, uiLogLvl );
	}
	
	return uiRegVal;
}

/* @} */
