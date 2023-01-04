/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dlp.h
 * @brief レジスタアドレス定義ヘッダ(DLP)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DLP_H
#define F_RRH_REG_DLP_H

/*!
 * @name DLPレジスタアドレス
 * @note DLPレジスタアドレス
 * @{
 */
#define	D_RRH_REG_DLP_UPBITW					(D_RRH_REG_DLP + 0x00000000)	/* U-plane Bit Width						*/
#define	D_RRH_REG_DLP_A_C0PRM0					(D_RRH_REG_DLP + 0x00000100)	/* Ant/Car Parameter (Ant% Car0 Parameter0)	*/
#define	D_RRH_REG_DLP_A_C0PRM1					(D_RRH_REG_DLP + 0x00000104)	/* Ant/Car Parameter (Ant% Car0 Parameter1)	*/
#define	D_RRH_REG_DLP_A_C1PRM0					(D_RRH_REG_DLP + 0x00000110)	/* Ant/Car Parameter (Ant% Car1 Parameter0)	*/
#define	D_RRH_REG_DLP_A_C1PRM1					(D_RRH_REG_DLP + 0x00000114)	/* Ant/Car Parameter (Ant% Car1 Parameter1)	*/

#define	D_RRH_REG_DLP_CKCHGBUFOVF				(D_RRH_REG_DLP + 0x00000F00)	/* Monitor（CLK乗換Buffer Overflow）		*/
#define	D_RRH_REG_DLP_DLFIFOOVF					(D_RRH_REG_DLP + 0x00000F04)	/* Monitor（個別FiFo Overflow）				*/
#define	D_RRH_REG_DLP_DLPMONSEL					(D_RRH_REG_DLP + 0x00000FFC)	/* Monitor（DLP Monitor Select）			*/

/* Performance Monitor Control	*/
#define	D_RRH_REG_DLP_PMONLATALL				(D_RRH_REG_DLP + 0x00000300)	/* 統計カウンタラッチ&クリア (一括)			*/
#define	D_RRH_REG_DLP_PMONLATCOM				(D_RRH_REG_DLP + 0x00000304)	/* 統計カウンタラッチ&クリア (個別)			*/
#define	D_RRH_REG_DLP_PMONLATA					(D_RRH_REG_DLP + 0x00000308)	/* 統計カウンタラッチ&クリア (個別)			*/
/* Performance Monitor(Common)	*/
#define	D_RRH_REG_DLP_ILGCOMHEADER				(D_RRH_REG_DLP + 0x00000400)	/* Common Header Illegal					*/
#define	D_RRH_REG_DLP_ILGCOMLAST				(D_RRH_REG_DLP + 0x00000404)	/* Last Flag Illegal						*/
#define	D_RRH_REG_DLP_ILGCOMPLEN				(D_RRH_REG_DLP + 0x00000408)	/* Payload Length Illegal					*/
#define	D_RRH_REG_DLP_ILGCOMEARLY				(D_RRH_REG_DLP + 0x0000040C)	/* Timing Too Early							*/
#define	D_RRH_REG_DLP_ILGCOMLATE				(D_RRH_REG_DLP + 0x00000410)	/* Timing Too Late							*/
#define	D_RRH_REG_DLP_ILGCOMTIM					(D_RRH_REG_DLP + 0x00000414)	/* Timing Illegal							*/
#define	D_RRH_REG_DLP_COMTIMNML					(D_RRH_REG_DLP + 0x00000418)	/* Timing Normal							*/
/* [Cr0] Performance Monitor FC	*/
#define	D_RRH_REG_DLP_ILGFSEQIDA_C0				(D_RRH_REG_DLP + 0x00000500)	/* FC-plane SEQID Illega					*/
#define	D_RRH_REG_DLP_ILGFFRMSTRA_C0			(D_RRH_REG_DLP + 0x00000504)	/* FC-plane Frame Structure Illegal			*/
#define	D_RRH_REG_DLP_ILGFSTYPEA_C0				(D_RRH_REG_DLP + 0x00000508)	/* FC-plane Section Type Illegal			*/
#define	D_RRH_REG_DLP_ILGFVERA_C0				(D_RRH_REG_DLP + 0x0000050C)	/* FC-plane Version Illegal					*/
#define	D_RRH_REG_DLP_RCVFTYPE0A_C0				(D_RRH_REG_DLP + 0x00000510)	/* FC-Plane Type0 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE1A_C0				(D_RRH_REG_DLP + 0x00000514)	/* FC-Plane Type1 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE2A_C0				(D_RRH_REG_DLP + 0x00000518)	/* FC-Plane Type2 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE3A_C0				(D_RRH_REG_DLP + 0x0000051C)	/* FC-Plane Type2 Receive					*/
/* [Cr0] Performance Monitor UP	*/
#define	D_RRH_REG_DLP_ILGUSEQIDA_C0				(D_RRH_REG_DLP + 0x00000520)	/* U-plane SEQID Illegal					*/
#define	D_RRH_REG_DLP_ILGUDIRA_C0				(D_RRH_REG_DLP + 0x00000524)	/* U-plane Directio Illegal					*/
#define	D_RRH_REG_DLP_ILGUVERA_C0				(D_RRH_REG_DLP + 0x00000528)	/* U-plane Version Illegal					*/
#define	D_RRH_REG_DLP_RCVUPA_C0					(D_RRH_REG_DLP + 0x0000052C)	/* U-plane Receive							*/
/* [Cr1] Performance Monitor FC	*/
#define	D_RRH_REG_DLP_ILGFSEQIDA_C1				(D_RRH_REG_DLP + 0x00000540)	/* FC-plane SEQID Illega					*/
#define	D_RRH_REG_DLP_ILGFFRMSTRA_C1			(D_RRH_REG_DLP + 0x00000544)	/* FC-plane Frame Structure Illegal			*/
#define	D_RRH_REG_DLP_ILGFSTYPEA_C1				(D_RRH_REG_DLP + 0x00000548)	/* FC-plane Section Type Illegal			*/
#define	D_RRH_REG_DLP_ILGFVERA_C1				(D_RRH_REG_DLP + 0x0000054C)	/* FC-plane Version Illegal					*/
#define	D_RRH_REG_DLP_RCVFTYPE0A_C1				(D_RRH_REG_DLP + 0x00000550)	/* FC-Plane Type0 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE1A_C1				(D_RRH_REG_DLP + 0x00000554)	/* FC-Plane Type1 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE2A_C1				(D_RRH_REG_DLP + 0x00000558)	/* FC-Plane Type2 Receive					*/
#define	D_RRH_REG_DLP_RCVFTYPE3A_C1				(D_RRH_REG_DLP + 0x0000055C)	/* FC-Plane Type2 Receive					*/
/* [Cr1] Performance Monitor UP	*/
#define	D_RRH_REG_DLP_ILGUSEQIDA_C1				(D_RRH_REG_DLP + 0x00000560)	/* U-plane SEQID Illegal					*/
#define	D_RRH_REG_DLP_ILGUDIRA_C1				(D_RRH_REG_DLP + 0x00000564)	/* U-plane Directio Illegal					*/
#define	D_RRH_REG_DLP_ILGUVERA_C1				(D_RRH_REG_DLP + 0x00000568)	/* U-plane Version Illegal					*/
#define	D_RRH_REG_DLP_RCVUPA_C1					(D_RRH_REG_DLP + 0x0000056C)	/* U-plane Receive							*/
/* @} */

/*!
 * @name DLPレジスタbit & Shift
 * @note DLPレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
