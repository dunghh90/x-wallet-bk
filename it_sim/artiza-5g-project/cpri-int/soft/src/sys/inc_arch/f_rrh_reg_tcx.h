/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_tcx.h
 * @brief レジスタアドレス定義ヘッダ(TCX)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_TCX_H
#define F_RRH_REG_TCX_H

/*!
 * @name TCXレジスタアドレス
 * @note TCXレジスタアドレス
 * @{
 */
#define	D_RRH_REG_TCX_TDDEN						(D_RRH_REG_TCX + 0x00000000)	/* TDD Control								*/
#define	D_RRH_REG_TCX_TDCNTOFST					(D_RRH_REG_TCX + 0x00000010)	/* TDD Counter offset						*/
#define	D_RRH_REG_TCX_TDMAGOFST					(D_RRH_REG_TCX + 0x00000020)	/* TDD Delay Margin offset					*/
#define	D_RRH_REG_TCX_TDANTENA					(D_RRH_REG_TCX + 0x00000040)	/* TDD Antenna Enable (Ant A)				*/
#define	D_RRH_REG_TCX_TDANTENB					(D_RRH_REG_TCX + 0x00000044)	/* TDD Antenna Enable (Ant B)				*/
#define	D_RRH_REG_TCX_TDHDOFFL					(D_RRH_REG_TCX + 0x00000100)	/* TDD Hard自律停止表示 (LLB Chip)			*/
#define	D_RRH_REG_TCX_TDHDOFFT					(D_RRH_REG_TCX + 0x00000104)	/* TDD Hard自律停止表示 (From CNT)			*/
#define	D_RRH_REG_TCX_TDSLFDIS					(D_RRH_REG_TCX + 0x00000110)	/* TDD Hard自律制御表示 (debug) (TD部自律oFF表示)		*/
#define	D_RRH_REG_TCX_TDSLFDISF					(D_RRH_REG_TCX + 0x00000114)	/* TDD Hard自律制御表示 (debug) (TD部自律oFF要因表示)	*/
#define	D_RRH_REG_TCX_TDCMDSET					(D_RRH_REG_TCX + 0x00000200)	/* TDD 処理コマンド設定 (Command Set)				*/
#define	D_RRH_REG_TCX_TDCMDSETSTA				(D_RRH_REG_TCX + 0x00000210)	/* TDD 処理コマンド設定 (Command Setting Status)	*/
#define	D_RRH_REG_TCX_TDCMDSTA					(D_RRH_REG_TCX + 0x00000220)	/* TDD 処理コマンド設定 (Execution command Status)	*/
#define	D_RRH_REG_TCX_TDCLEN					(D_RRH_REG_TCX + 0x00000230)	/* TDD AntCal 実行Enable					*/

/* TDD Switching Set(Internal Signal)	*/
#define	D_RRH_REG_TCX_TDIN00					(D_RRH_REG_TCX + 0x00000300)	/* Internal Switching Set(00)				*/
#define	D_RRH_REG_TCX_TDIN01					(D_RRH_REG_TCX + 0x00000304)	/* Internal Switching Set(01)				*/
#define	D_RRH_REG_TCX_TDIN02					(D_RRH_REG_TCX + 0x00000308)	/* Internal Switching Set(02)				*/
#define	D_RRH_REG_TCX_TDIN03					(D_RRH_REG_TCX + 0x0000030C)	/* Internal Switching Set(03)				*/
#define	D_RRH_REG_TCX_TDIN04					(D_RRH_REG_TCX + 0x00000310)	/* Internal Switching Set(04)				*/
#define	D_RRH_REG_TCX_TDIN05					(D_RRH_REG_TCX + 0x00000314)	/* Internal Switching Set(05)				*/
#define	D_RRH_REG_TCX_TDIN06					(D_RRH_REG_TCX + 0x00000318)	/* Internal Switching Set(06)				*/
#define	D_RRH_REG_TCX_TDIN07					(D_RRH_REG_TCX + 0x0000031C)	/* Internal Switching Set(07)				*/
#define	D_RRH_REG_TCX_TDIN08					(D_RRH_REG_TCX + 0x00000320)	/* Internal Switching Set(08)				*/

/* TDD Switching (RFIC)	*/
#define	D_RRH_REG_TCX_TDRF00					(D_RRH_REG_TCX + 0x00000380)	/* RFIC Switching Set(00)					*/
#define	D_RRH_REG_TCX_TDRF01					(D_RRH_REG_TCX + 0x00000384)	/* RFIC Switching Set(01)					*/
#define	D_RRH_REG_TCX_TDRF02					(D_RRH_REG_TCX + 0x00000388)	/* RFIC Switching Set(02)					*/
#define	D_RRH_REG_TCX_TDRF03					(D_RRH_REG_TCX + 0x0000038C)	/* RFIC Switching Set(03)					*/
#define	D_RRH_REG_TCX_TDRF04					(D_RRH_REG_TCX + 0x00000390)	/* RFIC Switching Set(04)					*/
#define	D_RRH_REG_TCX_TDRF05					(D_RRH_REG_TCX + 0x00000394)	/* RFIC Switching Set(05)					*/
#define	D_RRH_REG_TCX_TDRF06					(D_RRH_REG_TCX + 0x00000398)	/* RFIC Switching Set(06)					*/

/* TDD Switching (External AntA)	*/
#define	D_RRH_REG_TCX_TDEA00					(D_RRH_REG_TCX + 0x00000400)	/* Extarnal AntA Switching Set(00)			*/
#define	D_RRH_REG_TCX_TDEA01					(D_RRH_REG_TCX + 0x00000404)	/* Extarnal AntA Switching Set(01)			*/
#define	D_RRH_REG_TCX_TDEA02					(D_RRH_REG_TCX + 0x00000408)	/* Extarnal AntA Switching Set(02)			*/
#define	D_RRH_REG_TCX_TDEA03					(D_RRH_REG_TCX + 0x0000040C)	/* Extarnal AntA Switching Set(03)			*/

/* TDD Switching (External AntB)	*/
#define	D_RRH_REG_TCX_TDEB00					(D_RRH_REG_TCX + 0x00000480)	/* Extarnal AntB Switching Set(00)			*/
#define	D_RRH_REG_TCX_TDEB01					(D_RRH_REG_TCX + 0x00000484)	/* Extarnal AntB Switching Set(01)			*/
#define	D_RRH_REG_TCX_TDEB02					(D_RRH_REG_TCX + 0x00000488)	/* Extarnal AntB Switching Set(02)			*/
#define	D_RRH_REG_TCX_TDEB03					(D_RRH_REG_TCX + 0x0000048C)	/* Extarnal AntB Switching Set(03)			*/

/* TDD Switching (Calibration)	*/
#define	D_RRH_REG_TCX_TDCL00					(D_RRH_REG_TCX + 0x00000500)	/* Cal RFSW Switching Set1(00)				*/
#define	D_RRH_REG_TCX_TDCL01					(D_RRH_REG_TCX + 0x00000504)	/* Cal RFSW Switching Set1(01)				*/
#define	D_RRH_REG_TCX_TDCL02					(D_RRH_REG_TCX + 0x00000508)	/* Cal RFSW Switching Set1(02)				*/
#define	D_RRH_REG_TCX_TDCL03					(D_RRH_REG_TCX + 0x0000050C)	/* Cal RFSW Switching Set1(03)				*/
#define	D_RRH_REG_TCX_TDCL04					(D_RRH_REG_TCX + 0x00000510)	/* Cal RFSW Switching Set1(04)				*/
#define	D_RRH_REG_TCX_TDCL05					(D_RRH_REG_TCX + 0x00000514)	/* Cal RFSW Switching Set1(05)				*/
#define	D_RRH_REG_TCX_TDCL06					(D_RRH_REG_TCX + 0x00000518)	/* Cal RFSW Switching Set1(06)				*/
#define	D_RRH_REG_TCX_TDCL07					(D_RRH_REG_TCX + 0x0000051C)	/* Cal RFSW Switching Set1(07)				*/
#define	D_RRH_REG_TCX_TDCL08					(D_RRH_REG_TCX + 0x00000520)	/* Cal RFSW Switching Set1(08)				*/
#define	D_RRH_REG_TCX_TDCL09					(D_RRH_REG_TCX + 0x00000524)	/* Cal RFSW Switching Set1(09)				*/
#define	D_RRH_REG_TCX_TDCL10					(D_RRH_REG_TCX + 0x00000528)	/* Cal RFSW Switching Set1(10)				*/
#define	D_RRH_REG_TCX_TDCL11					(D_RRH_REG_TCX + 0x0000052C)	/* Cal RFSW Switching Set1(11)				*/
#define	D_RRH_REG_TCX_TDCL12					(D_RRH_REG_TCX + 0x00000530)	/* Cal RFSW Switching Set1(12)				*/
#define	D_RRH_REG_TCX_TDCL13					(D_RRH_REG_TCX + 0x00000534)	/* Cal RFSW Switching Set1(13)				*/
#define	D_RRH_REG_TCX_TDCL14					(D_RRH_REG_TCX + 0x00000538)	/* Cal RFSW Switching Set1(14)				*/
#define	D_RRH_REG_TCX_TDCL15					(D_RRH_REG_TCX + 0x0000053C)	/* Cal RFSW Switching Set1(15)				*/
#define	D_RRH_REG_TCX_TDCL16					(D_RRH_REG_TCX + 0x00000540)	/* Cal RFSW Switching Set2(16)				*/

/* TDD FIX Mode	*/
#define	D_RRH_REG_TCX_TDFXON					(D_RRH_REG_TCX + 0x00000700)	/* Fixed Mode oFF/oN (ChangeTRG)			*/
#define	D_RRH_REG_TCX_TDFXIAEN					(D_RRH_REG_TCX + 0x00000710)	/* Fixed Enable (Internal AntA)				*/
#define	D_RRH_REG_TCX_TDFXIASW					(D_RRH_REG_TCX + 0x00000714)	/* Fixed Logic (Internal AntA)				*/
#define	D_RRH_REG_TCX_TDFXIBEN					(D_RRH_REG_TCX + 0x00000718)	/* Fixed Enable (Internal AntB)				*/
#define	D_RRH_REG_TCX_TDFXIBSW					(D_RRH_REG_TCX + 0x0000071C)	/* Fixed Logic (Internal AntB)				*/
#define	D_RRH_REG_TCX_TDFXRFEN					(D_RRH_REG_TCX + 0x00000720)	/* Fixed Enable (RFIC)						*/
#define	D_RRH_REG_TCX_TDFXRFSW					(D_RRH_REG_TCX + 0x00000724)	/* Fixed Logic (RFIC)						*/
#define	D_RRH_REG_TCX_TDFXEAEN					(D_RRH_REG_TCX + 0x00000728)	/* Fixed Enable (External AntA)				*/
#define	D_RRH_REG_TCX_TDFXEASW					(D_RRH_REG_TCX + 0x0000072C)	/* Fixed Logic (External AntA)				*/
#define	D_RRH_REG_TCX_TDFXEBEN					(D_RRH_REG_TCX + 0x00000730)	/* Fixed Enable (External AntB)				*/
#define	D_RRH_REG_TCX_TDFXEBSW					(D_RRH_REG_TCX + 0x00000734)	/* Fixed Logic (External AntB)				*/
#define	D_RRH_REG_TCX_TDFXCLEN					(D_RRH_REG_TCX + 0x00000738)	/* Fixed Enable (Calibration)				*/
#define	D_RRH_REG_TCX_TDFXCLSW					(D_RRH_REG_TCX + 0x0000073C)	/* Fixed Logic (Calibration)				*/

/* TDD DTG Mode	*/
#define	D_RRH_REG_TCX_TDDGON					(D_RRH_REG_TCX + 0x00000780)	/* DTG Mode oFF/oN							*/
#define	D_RRH_REG_TCX_TDDGMD					(D_RRH_REG_TCX + 0x00000784)	/* DTG TDD Mode Select						*/
#define	D_RRH_REG_TCX_TDDGOFST					(D_RRH_REG_TCX + 0x00000788)	/* DTG TDD Counter Offset					*/
#define	D_RRH_REG_TCX_TDDGPTN					(D_RRH_REG_TCX + 0x0000078C)	/* DTG TDD Pattern Set						*/
#define	D_RRH_REG_TCX_TDDGIMPA0					(D_RRH_REG_TCX + 0x00000790)	/* DTG Impulse Mode (AntA Car0)				*/
#define	D_RRH_REG_TCX_TDDGIMPA1					(D_RRH_REG_TCX + 0x00000794)	/* DTG Impulse Mode (AntA Car1)				*/
#define	D_RRH_REG_TCX_TDDGIMPB0					(D_RRH_REG_TCX + 0x00000798)	/* DTG Impulse Mode (AntB Car0)				*/
#define	D_RRH_REG_TCX_TDDGIMPB1					(D_RRH_REG_TCX + 0x0000079C)	/* DTG Impulse Mode (AntB Car1)				*/
#define	D_RRH_REG_TCX_TDDGOFSA0					(D_RRH_REG_TCX + 0x000007A0)	/* DTG Data Phase Offset (AntA Car0)		*/
#define	D_RRH_REG_TCX_TDDGOFSA1					(D_RRH_REG_TCX + 0x000007A4)	/* DTG Data Phase Offset (AntA Car1)		*/
#define	D_RRH_REG_TCX_TDDGOFSB0					(D_RRH_REG_TCX + 0x000007A8)	/* DTG Data Phase Offset (AntB Car0)		*/
#define	D_RRH_REG_TCX_TDDGOFSB1					(D_RRH_REG_TCX + 0x000007AC)	/* DTG Data Phase Offset (AntB Car1)		*/
#define	D_RRH_REG_TCX_TDDGDLY					(D_RRH_REG_TCX + 0x000007B0)	/* DTG DG Block Delay						*/

#define	D_RRH_REG_TCX_TCRSV						(D_RRH_REG_TCX + 0x00000800)	/* TC  Reerve								*/
#define	D_RRH_REG_TCX_TCULFRMAJ					(D_RRH_REG_TCX + 0x00000840)	/* TC UL-DL timing relation					*/
#define	D_RRH_REG_TCX_TCRARXRDAJ				(D_RRH_REG_TCX + 0x00000900)	/* TC Rach Frame timing Adjust				*/
#define	D_RRH_REG_TCX_TCRAPKTGAJ				(D_RRH_REG_TCX + 0x00000904)	/* TC Rach Frame timing Adjust				*/
#define	D_RRH_REG_TCX_TCXTIMMON					(D_RRH_REG_TCX + 0x00000FF0)	/* TCX TIMMON Select						*/
#define	D_RRH_REG_TCX_TCXTEST					(D_RRH_REG_TCX + 0x00000FF4)	/* TCX TEST									*/
#define	D_RRH_REG_TCX_TCXTSTMON					(D_RRH_REG_TCX + 0x00000FF8)	/* TCX Test Monitor (debug)					*/
#define	D_RRH_REG_TCX_TCXMONSEL					(D_RRH_REG_TCX + 0x00000FFC)	/* TCX Monitor Select						*/
/* @} */

/*!
 * @name TCXレジスタbit & Shift
 * @note TCXレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
