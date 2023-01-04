/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dpd.h
 * @brief レジスタアドレス定義ヘッダ(DPD)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DPD_H
#define F_RRH_REG_DPD_H

/*!
 * @name DPDレジスタアドレス
 * @note DPDレジスタアドレス
 * @{
 */
#define	D_RRH_REG_DPD_TXCAREN					(D_RRH_REG_DPD + 0x00000000)	/* TxEnable									*/
#define	D_RRH_REG_DPD_TXCNF0					(D_RRH_REG_DPD + 0x00000010)	/* System (Cr0 config 信号帯域設定)			*/
#define	D_RRH_REG_DPD_TXCNF1					(D_RRH_REG_DPD + 0x00000014)	/* System (Cr1 config 信号帯域設定)			*/
#define	D_RRH_REG_DPD_TXPRM0					(D_RRH_REG_DPD + 0x00000030)	/* Parameter (Cr0 遅延調整値(粗:1/3.84MHz))	*/
#define	D_RRH_REG_DPD_TXPRM1					(D_RRH_REG_DPD + 0x00000034)	/* Parameter (Cr1 遅延調整値(粗:1/3.84MHz))	*/
#define	D_RRH_REG_DPD_BTIGAIN0					(D_RRH_REG_DPD + 0x00000040)	/* Parameter (Cr0 BTI-GAIN調整値)			*/
#define	D_RRH_REG_DPD_BTIGAIN1					(D_RRH_REG_DPD + 0x00000044)	/* Parameter (Cr1 BTI-GAIN調整値)			*/
#define	D_RRH_REG_DPD_LPFGAIN0					(D_RRH_REG_DPD + 0x00000050)	/* LPF Gain (Cr0 LPF-GAIN調整値)			*/
#define	D_RRH_REG_DPD_LPFGAIN1					(D_RRH_REG_DPD + 0x00000054)	/* LPF Gain (Cr1 LPF-GAIN調整値)			*/
#define	D_RRH_REG_DPD_LPF0						(D_RRH_REG_DPD + 0x00000050)	/* LPF Coefficient (Cr0 LPF係数)			*/
#define	D_RRH_REG_DPD_LPF1						(D_RRH_REG_DPD + 0x00000054)	/* LPF Coefficient (Cr1 LPF係数)			*/
#define	D_RRH_REG_DPD_TXANTEN					(D_RRH_REG_DPD + 0x00000080)	/* Antenna Enable							*/
#define	D_RRH_REG_DPD_HCFR						(D_RRH_REG_DPD + 0x00000100)	/* HighPass CFR								*/
#define	D_RRH_REG_DPD_ICFRB						(D_RRH_REG_DPD + 0x00000140)	/* Impulse CFR (Before) (ImpulseCFRピーク検出範囲設定(primary))		*/
#define	D_RRH_REG_DPD_IMPMEMB					(D_RRH_REG_DPD + 0x00000144)	/* Impulse CFR (Before) (Impulse係数メモリbank表示(ハード使用側))	*/
#define	D_RRH_REG_DPD_IMPMEMA					(D_RRH_REG_DPD + 0x00000148)	/* Impulse CFR (Before) (Impulse係数メモリbank表示(ハード使用側))	*/
#define	D_RRH_REG_DPD_IMPMEMRDSEL				(D_RRH_REG_DPD + 0x0000014C)	/* Impulse CFR (Read)						*/
#define	D_RRH_REG_DPD_ECFR						(D_RRH_REG_DPD + 0x00000180)	/* EnvelopeClipping CFR						*/
#define	D_RRH_REG_DPD_ICFRBFRMON				(D_RRH_REG_DPD + 0x000001C0)	/* CFR Monitor (ImpulseCFRモニタ (Before))	*/
#define	D_RRH_REG_DPD_ICFRAFRMON				(D_RRH_REG_DPD + 0x000001C4)	/* CFR Monitor (ImpulseCFRモニタ (After))	*/
#define	D_RRH_REG_DPD_ECFRMON					(D_RRH_REG_DPD + 0x000001CC)	/* CFR Monitor (EnvelopeClippingCFRモニタ)	*/
#define	D_RRH_REG_DPD_TSSI0						(D_RRH_REG_DPD + 0x00000200)	/* 電力報告(キャリア電力用) (Cr0 TSSI測定値)			*/
#define	D_RRH_REG_DPD_TSSI1						(D_RRH_REG_DPD + 0x00000204)	/* 電力報告(キャリア電力用) (Cr1 TSSI測定値)			*/
#define	D_RRH_REG_DPD_TSSISMPL					(D_RRH_REG_DPD + 0x00000214)	/* 電力報告(キャリア電力用) (Cr TSSI累積サンプル数設定)	*/
#define	D_RRH_REG_DPD_TSSILAT					(D_RRH_REG_DPD + 0x00000218)	/* 電力報告(キャリア電力用) (Cr TSSI値保持)				*/
#define	D_RRH_REG_DPD_TSSI						(D_RRH_REG_DPD + 0x0000021C)	/* 電力報告(キャリア電力用) (Cr TSSI保持値クリア)		*/
#define	D_RRH_REG_DPD_CWEN						(D_RRH_REG_DPD + 0x00000280)	/* 無変調送信 (CW出力キャリアイネーブル)		*/
#define	D_RRH_REG_DPD_CWAMP						(D_RRH_REG_DPD + 0x00000284)	/* 無変調送信 (CW出力振幅設定(Ichキャリア共通))	*/
#define	D_RRH_REG_DPD_LPFREGEN					(D_RRH_REG_DPD + 0x000002A0)	/* LPF係数									*/
#define	D_RRH_REG_DPD_TXNCOPH0					(D_RRH_REG_DPD + 0x00000300)	/* TEST-NCO (Cr0 周波数シフト初期位相設定)	*/
#define	D_RRH_REG_DPD_TXNCOPH1					(D_RRH_REG_DPD + 0x00000304)	/* TEST-NCO (Cr1 周波数シフト初期位相設定)	*/
#define	D_RRH_REG_DPD_CCTEST					(D_RRH_REG_DPD + 0x000003F8)	/* TESTモード								*/
#define	D_RRH_REG_DPD_CCMONSEL					(D_RRH_REG_DPD + 0x000003FC)	/* CCモニタ									*/
#define	D_RRH_REG_DPD_CRCAREN					(D_RRH_REG_DPD + 0x00000000)	/* TxEnable (ミラー：CR部で参照)			*/
#define	D_RRH_REG_DPD_CRCNF0					(D_RRH_REG_DPD + 0x00000010)	/* System (Cr0 config 信号帯域設定)			*/
#define	D_RRH_REG_DPD_CRCNF1					(D_RRH_REG_DPD + 0x00000014)	/* System (Cr1 config 信号帯域設定)			*/
#define	D_RRH_REG_DPD_CRTEST					(D_RRH_REG_DPD + 0x000003F8)	/* TESTモード(ミラー：CR部で参照)			*/
#define	D_RRH_REG_DPD_CRMONSEL					(D_RRH_REG_DPD + 0x000003FC)	/* CCモニタ(ミラー：CR部で参照)				*/
/* @} */

/*!
 * @name DPDレジスタbit & Shift
 * @note DPDレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
