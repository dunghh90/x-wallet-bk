/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_rach.h
 * @brief レジスタアドレス定義ヘッダ(RACH)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_RACH_H
#define F_RRH_REG_RACH_H

/*!
 * @name RACHレジスタアドレス
 * @note RACHレジスタアドレス
 * @{
 */
#define	D_RRH_REG_RACH_RACALTBL					(D_RRH_REG_RACH + 0x00000050)	/* [Ant/Cr共通] CAL補正係数RAMアクセス方路設定	*/
#define	D_RRH_REG_RACH_RACHFTBL					(D_RRH_REG_RACH + 0x00000054)	/* [Ant/Cr共通] CHF補正係数RAMアクセス方路設定	*/
#define	D_RRH_REG_RACH_RABBFTBL					(D_RRH_REG_RACH + 0x00000058)	/* [Ant/Cr共通] BBF補正係数RAMアクセス方路設定	*/
#define	D_RRH_REG_RACH_RATEST					(D_RRH_REG_RACH + 0x000003F8)	/* TESTモード								*/
#define	D_RRH_REG_RACH_RAMONSEL					(D_RRH_REG_RACH + 0x000003FC)	/* RXモニタ									*/

/* RACH LPF Gain	*/
#define	D_RRH_REG_RACH_RALPFGAIN00				(D_RRH_REG_RACH + 0x00000000)	/* Cr0 起動(FDM) 0 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN01				(D_RRH_REG_RACH + 0x00000004)	/* Cr0 起動(FDM) 1 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN02				(D_RRH_REG_RACH + 0x00000008)	/* Cr0 起動(FDM) 2 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN03				(D_RRH_REG_RACH + 0x0000000C)	/* Cr0 起動(FDM) 3 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN04				(D_RRH_REG_RACH + 0x00000010)	/* Cr0 起動(FDM) 4 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN05				(D_RRH_REG_RACH + 0x00000014)	/* Cr0 起動(FDM) 5 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN06				(D_RRH_REG_RACH + 0x00000018)	/* Cr0 起動(FDM) 6 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN07				(D_RRH_REG_RACH + 0x0000001C)	/* Cr0 起動(FDM) 7 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN10				(D_RRH_REG_RACH + 0x00000020)	/* Cr1 起動(FDM) 0 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN11				(D_RRH_REG_RACH + 0x00000024)	/* Cr1 起動(FDM) 1 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN12				(D_RRH_REG_RACH + 0x00000028)	/* Cr1 起動(FDM) 2 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN13				(D_RRH_REG_RACH + 0x0000002C)	/* Cr1 起動(FDM) 3 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN14				(D_RRH_REG_RACH + 0x00000030)	/* Cr1 起動(FDM) 4 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN15				(D_RRH_REG_RACH + 0x00000034)	/* Cr1 起動(FDM) 5 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN16				(D_RRH_REG_RACH + 0x00000038)	/* Cr1 起動(FDM) 6 LPF GAIN設定				*/
#define	D_RRH_REG_RACH_RALPFGAIN17				(D_RRH_REG_RACH + 0x0000003C)	/* Cr1 起動(FDM) 7 LPF GAIN設定				*/

/* RACH LPF Coef	*/
#define	D_RRH_REG_RACH_RALPFCOEF0				(D_RRH_REG_RACH + 0x00000050)	/* RACH LPF Filter係数(Preamble Format0)	*/
#define	D_RRH_REG_RACH_RALPFCOEF1				(D_RRH_REG_RACH + 0x00000054)	/* RACH LPF Filter係数(Preamble Format1)	*/
#define	D_RRH_REG_RACH_RALPFCOEF2				(D_RRH_REG_RACH + 0x00000058)	/* RACH LPF Filter係数(Preamble Format2)	*/
#define	D_RRH_REG_RACH_RALPFCOEF3				(D_RRH_REG_RACH + 0x0000005C)	/* RACH LPF Filter係数(Preamble Format3)	*/
#define	D_RRH_REG_RACH_RALPFCOEF4				(D_RRH_REG_RACH + 0x00000060)	/* RACH LPF Filter係数(Preamble Format4)	*/
#define	D_RRH_REG_RACH_RALPFCOEF5				(D_RRH_REG_RACH + 0x00000064)	/* RACH LPF Filter係数(Preamble Format5)	*/
/* @} */

/*!
 * @name RACHレジスタbit & Shift
 * @note RACHレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
