/*!
 * @skip  $ld:$
 * @file  f_du_reg_oth.h
 * @brief レジスタアドレス定義ヘッダ(other)
 * @date  2018/08/29 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_OTH_H
#define F_DU_REG_OTH_H

/*!
 * @name レジスタアドレス (other)
 * @note レジスタアドレス ("Lx1DL1X_CCB_xxxx.xlsx"に定義のないもの)
 * @{
 */
/* PCIe起動 (pf_mainで使用)	*/
#define D_DU_REG_OTH_REL_RESET				0xFF0A0054		/**< Zynq ROOT/ENDリセット解除レジスタ		*/

/* 外部IO制御監視 (pf_eioで使用)	*/
#define D_DU_REG_OTH_GPIO_DATA_2			0xFF0A0048		/**< PORT出力設定							*//*	5G-商用DU	*/
#define D_DU_REG_OTH_GPIO_DATA_2_RO			0xFF0A0068		/**< PORT入力状態							*//*	5G-商用DU	*/

#define D_DU_REGBIT_OTH_GPIO_DATA_2_LINEOUT_OFF	0x00000000		/**< PORT出力ビット:B[12] OPEN				*//*	5G-商用DU	*/
#define D_DU_REGBIT_OTH_GPIO_DATA_2_LINEOUT		0x00001000		/**< PORT出力ビット:B[12] SHORT				*//*	5G-商用DU	*/
#define D_DU_REGBIT_OTH_GPIO_DATA_2_RO_LINEIN	0x00000400		/**< PORT入力ビット:B[10]					*//*	5G-商用DU	*/

/* PCIe起動(TRX-FPGA1) (pf_mainで使用)	*/
#define D_DU_REG_OTH_IRQ_SET_DECODE_TRX1	0xBC000138		/**< IRQマスク設定レジスタ					*/
#define D_DU_REG_OTH_IRQ_MASK_TRX1			0xBC00013C		/**< IRQ Decode設定レジスタ					*/
#define D_DU_REG_OTH_PHY_STATE_TRX1			0xBC000144		/**< PHY Statusレジスタ						*/
#define D_DU_REG_OTH_COMMAND_TRX1			0xBC000004		/**< Commandレジスタ						*/
#define D_DU_REG_OTH_SEC_LET_TIMER_TRX1		0xBC000018		/**< Second Latency Timerレジスタ			*/
#define D_DU_REG_OTH_ROOT_PORT_STA_CNT_TRX1	0xBC000148		/**< Root Port Status/ Controlレジスタ		*/

#define D_DU_REG_OTH_BHLC_TRX1				0xBC10000C		/**< BIST/Header/Last timer/CachLn ERROR Reg*/
#define D_DU_REG_OTH_STATE_COMMAND_TRX1		0xBC100004		/**< Status/Commandレジスタ					*/
#define D_DU_REG_OTH_BASE_ADDR_TRX1			0xBC100010		/**< Base Addressレジスタ					*/

#define D_DU_REG_OTH_CHKREG_TRX1			0xB0000004		/**< 設定チェックレジスタ					*/

#define D_DU_REG_OTH_UPPER_END_IRQ_CLR_TRX1	0xB0800138		/**< Upper End IRQクリアレジスタ			*/
#define D_DU_REG_OTH_UPPER_END_IRQ_MSK_TRX1	0xB080013C		/**< Upper End IRQマスクレジスタ			*/

/* PCIe起動(TRX-FPGA2) (pf_mainで使用)	*/
#define D_DU_REG_OTH_IRQ_SET_DECODE_TRX2	0xBC200138		/**< IRQマスク設定レジスタ					*/
#define D_DU_REG_OTH_IRQ_MASK_TRX2			0xBC20013C		/**< IRQ Decode設定レジスタ					*/
#define D_DU_REG_OTH_PHY_STATE_TRX2			0xBC200144		/**< PHY Statusレジスタ						*/
#define D_DU_REG_OTH_COMMAND_TRX2			0xBC200004		/**< Commandレジスタ						*/
#define D_DU_REG_OTH_SEC_LET_TIMER_TRX2		0xBC200018		/**< Second Latency Timerレジスタ			*/
#define D_DU_REG_OTH_ROOT_PORT_STA_CNT_TRX2	0xBC200148		/**< Root Port Status/ Controlレジスタ		*/

#define D_DU_REG_OTH_BHLC_TRX2				0xBC30000C		/**< BIST/Header/Last timer/CachLn ERROR Reg*/
#define D_DU_REG_OTH_STATE_COMMAND_TRX2		0xBC300004		/**< Status/Commandレジスタ					*/
#define D_DU_REG_OTH_BASE_ADDR_TRX2			0xBC300010		/**< Base Addressレジスタ					*/

#define D_DU_REG_OTH_CHKREG_TRX2			0xB2010004		/**< 設定チェックレジスタ					*/

#define D_DU_REG_OTH_UPPER_END_IRQ_CLR_TRX2	0xB2800138		/**< Upper End IRQクリアレジスタ			*/
#define D_DU_REG_OTH_UPPER_END_IRQ_MSK_TRX2	0xB280013C		/**< Upper End IRQマスクレジスタ			*/

/* TRX-PL SV-IF確認 (pf_mainで使用)	*/
#define D_DU_REG_OTH_PL_SVIF_CHK_TRX1		0xB0010200		/**< TRX1 PL SV-IF Checkレジスタ			*/
#define D_DU_REG_OTH_PL_SVIF_CNF_TRX1		0xB0010204		/**< TRX1 PL SV-IF Configレジスタ			*/
#define D_DU_REG_OTH_PL_SVIF_CHK_TRX2		0xB2010200		/**< TRX2 PL SV-IF Checkレジスタ			*/
#define D_DU_REG_OTH_PL_SVIF_CNF_TRX2		0xB2010204		/**< TRX2 PL SV-IF Configレジスタ			*/

/* SRAM診断用	*/
#define D_DU_REG_OTH_SRAM0_CHK_0			0xA3555500		/**< 0xA3555500[15:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_1			0xA3555504		/**< 0xA3555504[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_2			0xA3555508		/**< 0xA3555508[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_3			0xA355550C		/**< 0xA355550C[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_4			0xA3555510		/**< 0xA3555510[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_5			0xA32AAAA0		/**< 0xA32AAAA0[15:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_6			0xA32AAAA4		/**< 0xA32AAAA4[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_7			0xA32AAAA8		/**< 0xA32AAAA8[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_8			0xA32AAAAC		/**< 0xA32AAAAC[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM0_CHK_9			0xA32AAAB0		/**< 0xA32AAAB0[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_0			0xA4555500		/**< 0xA4555500[15:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_1			0xA4555504		/**< 0xA4555504[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_2			0xA4555508		/**< 0xA4555508[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_3			0xA455550C		/**< 0xA455550C[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_4			0xA4555510		/**< 0xA4555510[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_5			0xA42AAAA0		/**< 0xA42AAAA0[15:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_6			0xA42AAAA4		/**< 0xA42AAAA4[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_7			0xA42AAAA8		/**< 0xA42AAAA8[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_8			0xA42AAAAC		/**< 0xA42AAAAC[31:0]レジスタ			*/
#define D_DU_REG_OTH_SRAM1_CHK_9			0xA42AAAB0		/**< 0xA42AAAB0[31:0]レジスタ			*/

/* Reset reason	*/
#define D_DU_REG_OTH_RESET_REASON			0xFF5E0220
#define D_DU_REG_OTH_RESET_REASON_CLR		0x0000007F
#define D_DU_REG_OTH_RESET_BIT_POR			0x00000001
#define D_DU_REG_OTH_RESET_BIT_SWDT			0x00000002
#define D_DU_REG_OTH_RESET_BIT_SRST			0x00000010
#define D_DU_REG_OTH_RESET_BIT_SOFT			0x00000020

#define D_DU_REG_OTH_RESET_SOFT				0xFF5E0218
#define D_DU_REG_OTH_RESET_SOFT_CTL			0x00000010


#define D_DU_REG_OTH_RX_RXCHFGAIN_PRACH		0xA2400150
#define M_DU_REG_OTH_RX_RXCHFGAIN_PRACH(ccofs,antofs)		( D_DU_REG_OTH_RX_RXCHFGAIN_PRACH + (ccofs * 4) + (antofs * 0x400) )

/* @} */
#endif
/* @} */
