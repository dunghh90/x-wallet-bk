/*!
 * @skip  $ld:$
 * @file  f_dpda_IO_read.c
 * @brief DPDAタスク レジスタ値設定処理
 * @date  2012/01/23 ALPHA)藤井 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2012-2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:レジスタ値取得処理
 * @note  関数処理内容.
 *		-# 書き込みモードが通常の場合
 *			-# BPF_HM_DEVC_REG_WRITEをコールしてレジスタ書き込みを行う
 *		-# 書き込みモードがBit Onの場合
 *			-# BPF_HM_DEVC_REG_WRITE_BITONをコールしてレジスタ書き込みを行う
 *		-# 書き込みモードがBit Offの場合
 *			-# BPF_HM_DEVC_REG_WRITE_BITOFFをコールしてレジスタ書き込みを行う
 *		-# 書き込みモードがSPI(CPLD)の場合
 *			-# SPI経由でCPLDレジスタ書き込みを行う
 *		-# 書き込みモードがSPI(FEFB)の場合
 *			-# SPI経由でFEFBレジスタ書き込みを行う
 *		-# 書き込みモードがSPI(RX)の場合
 *			-# SPI経由でRXレジスタ書き込みを行う
 *		-# 書き込みモードがEEPROMの場合
 *			-# SPI経由でEEPROMレジスタ書き込みを行う
 *		-# 書き込みモードが上記以外の場合
 *			-# ログ出力を行う
 * @param  uiRegAddr			[in]UINT	設定レジスタアドレス
 * @param  uiWriteDat			[in]UINT	設定データ
 * @param  uiWriteMode			[in]UINT	書き込みモード( 通常:0, Bit ON:1, Bit OFF:2, SPI(CPLD):3, SPI(FEFB):4, SPI(RX):5, EEPROM:6 )
 * @param  uiLogLvl				[in]UINT	ログレベル(SPI経由、EEPROMアクセスの場合は0を指定)
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2012/01/23 ALPHA)藤井 Create (M-RRU-ZSYS-00792対処)
 * @date 2013/04/03 ALPHA)中村 B25対応
 * @date 2013/11/25 ALPHA)加藤 ZYNQ対応
 */
VOID f_dpda_IO_write(UINT	uiRegAddr,
					 UINT	uiWriteDat,
					 UINT	uiWriteMode,
					 UINT	uiLogLvl)
{

	switch( uiWriteMode )
	{
		case E_DPDA_BPF_NOMAL_MODE:
			/* 通常の書き込みを行う */
			BPF_HM_DEVC_REG_WRITE(uiLogLvl,						/**< ログレベル						*/
								  uiRegAddr,					/**< レジスタ書き込みオフセット		*/
								  &uiWriteDat);					/**< 書き込みポインタ				*/
			break;
		case E_DPDA_BPF_BITON_MODE:
			/* 指定BITのONを行う */
			BPF_HM_DEVC_REG_WRITE_BITON(uiLogLvl,				/**< ログレベル						*/
								  uiRegAddr,					/**< レジスタ書き込みオフセット		*/
								  uiWriteDat);					/**< 書き込みポインタ				*/
			break;
		case E_DPDA_BPF_BITOFF_MODE:
			/* 指定BITのOFFを行う */
			BPF_HM_DEVC_REG_WRITE_BITOFF(uiLogLvl,				/**< ログレベル						*/
										 uiRegAddr,				/**< レジスタ書き込みオフセット		*/
										 uiWriteDat);			/**< 書き込みポインタ				*/
			break;
		default:
			/* assertログ取得 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "uiWriteMode invalid [%d]", uiWriteMode);
			break;
	}
	return;
}

/* @} */
