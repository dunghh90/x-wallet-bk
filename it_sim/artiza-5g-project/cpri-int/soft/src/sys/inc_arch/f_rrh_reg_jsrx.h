/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_jsrx.h
 * @brief レジスタアドレス定義ヘッダ(JSRX)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_JSRX_H
#define F_RRH_REG_JSRX_H

#if 0
/***** 5G-DUハソ仕様書対応	*****/
/***** D_RRH_REG_JSRX が仮設定であるため、既存defineを有効化	*****/
/*!
 * @name JSRXレジスタアドレス
 * @note JSRXレジスタアドレス
 * @{
 */
#define	D_RRH_REG_JSRX_JSESRCTR					(D_RRH_REG_JSRX + 0x00001000)	/* External SYSREF Control					*/
#define	D_RRH_REG_JSRX_JSESRLOG					(D_RRH_REG_JSRX + 0x00001004)	/* External SYSREF Log						*/
#define	D_RRH_REG_JSRX_JSRXSRCTR				(D_RRH_REG_JSRX + 0x00001020)	/* Internal SYSREF Control					*/
#define	D_RRH_REG_JSRX_JSRXSYNCCTR				(D_RRH_REG_JSRX + 0x00001030)	/* JSRX XSYNC Control						*/
#define	D_RRH_REG_JSRX_JSRXSYNCMON				(D_RRH_REG_JSRX + 0x00001034)	/* JSRX XSYNC Monitor						*/
#define	D_RRH_REG_JSRX_JSRXEN					(D_RRH_REG_JSRX + 0x00001080)	/* JSRX JESD Enable							*/
#define	D_RRH_REG_JSRX_JSRXRF					(D_RRH_REG_JSRX + 0x00001084)	/* JSRX JESD Use RF Device Num				*/
#define	D_RRH_REG_JSRX_JSRXRFTYP				(D_RRH_REG_JSRX + 0x00001088)	/* JSRX JESD Use RF Device					*/
#define	D_RRH_REG_JSRX_JSRXCTR					(D_RRH_REG_JSRX + 0x00001090)	/* JSRX JESD Control						*/
#define	D_RRH_REG_JSRX_JSRXMINDLY				(D_RRH_REG_JSRX + 0x00001098)	/* JSRX JESD Minimum Latency Value			*/
#define	D_RRH_REG_JSRX_JSRXRSMD					(D_RRH_REG_JSRX + 0x000010A0)	/* JSRX Re-Synchronaization Mode			*/
#define	D_RRH_REG_JSRX_JSRXCP0L0				(D_RRH_REG_JSRX + 0x00001100)	/* JSRX Config Parameter0 Lane0				*/
#define	D_RRH_REG_JSRX_JSRXCP1L0				(D_RRH_REG_JSRX + 0x00001104)	/* JSRX Config Parameter1 Lane0				*/
#define	D_RRH_REG_JSRX_JSRXCP2L0				(D_RRH_REG_JSRX + 0x00001108)	/* JSRX Config Parameter2 Lane0				*/
#define	D_RRH_REG_JSRX_JSRXCP3L0				(D_RRH_REG_JSRX + 0x0000110C)	/* JSRX Config Parameter3 Lane0				*/
#define	D_RRH_REG_JSRX_JSRXCP0L1				(D_RRH_REG_JSRX + 0x00001110)	/* JSRX Config Parameter0 Lane1				*/
#define	D_RRH_REG_JSRX_JSRXCP1L1				(D_RRH_REG_JSRX + 0x00001114)	/* JSRX Config Parameter1 Lane1				*/
#define	D_RRH_REG_JSRX_JSRXCP2L1				(D_RRH_REG_JSRX + 0x00001118)	/* JSRX Config Parameter2 Lane1				*/
#define	D_RRH_REG_JSRX_JSRXCP3L1				(D_RRH_REG_JSRX + 0x0000111C)	/* JSRX Config Parameter3 Lane1				*/
#define	D_RRH_REG_JSRX_JSRXCP0L2				(D_RRH_REG_JSRX + 0x00001120)	/* JSRX Config Parameter0 Lane2				*/
#define	D_RRH_REG_JSRX_JSRXCP1L2				(D_RRH_REG_JSRX + 0x00001124)	/* JSRX Config Parameter1 Lane2				*/
#define	D_RRH_REG_JSRX_JSRXCP2L2				(D_RRH_REG_JSRX + 0x00001128)	/* JSRX Config Parameter2 Lane2				*/
#define	D_RRH_REG_JSRX_JSRXCP3L2				(D_RRH_REG_JSRX + 0x0000112C)	/* JSRX Config Parameter3 Lane2				*/
#define	D_RRH_REG_JSRX_JSRXCP0L3				(D_RRH_REG_JSRX + 0x00001130)	/* JSRX Config Parameter0 Lane3				*/
#define	D_RRH_REG_JSRX_JSRXCP1L3				(D_RRH_REG_JSRX + 0x00001134)	/* JSRX Config Parameter1 Lane3				*/
#define	D_RRH_REG_JSRX_JSRXCP2L3				(D_RRH_REG_JSRX + 0x00001138)	/* JSRX Config Parameter2 Lane3				*/
#define	D_RRH_REG_JSRX_JSRXCP3L3				(D_RRH_REG_JSRX + 0x0000113C)	/* JSRX Config Parameter3 Lane3				*/
#define	D_RRH_REG_JSRX_JSRXXBAR					(D_RRH_REG_JSRX + 0x00001200)	/* JSRX Antena Select						*/
#define	D_RRH_REG_JSRX_JSRXST					(D_RRH_REG_JSRX + 0x00001300)	/* JSRX Status								*/
#define	D_RRH_REG_JSRX_JSRXPFST					(D_RRH_REG_JSRX + 0x00001304)	/* JSRX PHY FIFO Status						*/
#define	D_RRH_REG_JSRX_JSRXPEST					(D_RRH_REG_JSRX + 0x00001308)	/* JSRX PHY Error Status					*/
#define	D_RRH_REG_JSRX_JSRXPCST					(D_RRH_REG_JSRX + 0x0000130C)	/* JSRX PHY CDR Status						*/
#define	D_RRH_REG_JSRX_JSRXEST					(D_RRH_REG_JSRX + 0x00001310)	/* JSRX JESD Error Status					*/
#define	D_RRH_REG_JSRX_JSRXPECL0				(D_RRH_REG_JSRX + 0x00001320)	/* Lane0 8B10B Error Count					*/
#define	D_RRH_REG_JSRX_JSRXPECL1				(D_RRH_REG_JSRX + 0x00001324)	/* Lane1 8B10B Error Count					*/
#define	D_RRH_REG_JSRX_JSRXPECL2				(D_RRH_REG_JSRX + 0x00001328)	/* Lane2 8B10B Error Count					*/
#define	D_RRH_REG_JSRX_JSRXPECL3				(D_RRH_REG_JSRX + 0x0000132C)	/* Lane3 8B10B Error Count					*/
#define	D_RRH_REG_JSRX_JSRXFECL0				(D_RRH_REG_JSRX + 0x00001340)	/* Lane0 Frame周期 Error Count				*/
#define	D_RRH_REG_JSRX_JSRXFECL1				(D_RRH_REG_JSRX + 0x00001344)	/* Lane1 Frame周期 Error Count				*/
#define	D_RRH_REG_JSRX_JSRXFECL2				(D_RRH_REG_JSRX + 0x00001348)	/* Lane2 Frame周期 Error Count				*/
#define	D_RRH_REG_JSRX_JSRXFECL3				(D_RRH_REG_JSRX + 0x0000134C)	/* Lane3 Frame周期 Error Count				*/
#define	D_RRH_REG_JSRX_JSRXDTIML0				(D_RRH_REG_JSRX + 0x00001380)	/* JSRX JESD Data Input Timing (Lane0)		*/
#define	D_RRH_REG_JSRX_JSRXDTIML1				(D_RRH_REG_JSRX + 0x00001384)	/* JSRX JESD Data Input Timing (Lane1)		*/
#define	D_RRH_REG_JSRX_JSRXDTIML2				(D_RRH_REG_JSRX + 0x00001388)	/* JSRX JESD Data Input Timing (Lane2)		*/
#define	D_RRH_REG_JSRX_JSRXDTIML3				(D_RRH_REG_JSRX + 0x0000138C)	/* JSRX JESD Data Input Timing (Lane3)		*/
#define	D_RRH_REG_JSRX_JSRXIRQ					(D_RRH_REG_JSRX + 0x00001400)	/* JSRX IRQ									*/
#define	D_RRH_REG_JSRX_JSRXIRQC					(D_RRH_REG_JSRX + 0x00001404)	/* JSRX IRQ									*/
#define	D_RRH_REG_JSRX_JSRXIRQM					(D_RRH_REG_JSRX + 0x00001408)	/* JSRX IRQ									*/
#define	D_RRH_REG_JSRX_JSRXLMFC					(D_RRH_REG_JSRX + 0x00001500)	/* JSRX Timing Offset						*/
#define	D_RRH_REG_JSRX_JSRXLXSYNC				(D_RRH_REG_JSRX + 0x00001504)	/* JSRX Timing Offset						*/
#define	D_RRH_REG_JSRX_JSRXLALIGN				(D_RRH_REG_JSRX + 0x00001508)	/* JSRX Timing Offset						*/
#define	D_RRH_REG_JSRX_JSRXSTMON				(D_RRH_REG_JSRX + 0x00001F00)	/* JSRX State Monitor						*/
#define	D_RRH_REG_JSRX_JSRXSTLOG				(D_RRH_REG_JSRX + 0x00001F04)	/* JSRX State Change Log					*/
#define	D_RRH_REG_JSRX_JSRXKLOGL0				(D_RRH_REG_JSRX + 0x00001F10)	/* Lane0 K CODE受信表示						*/
#define	D_RRH_REG_JSRX_JSRXKLOGL1				(D_RRH_REG_JSRX + 0x00001F14)	/* Lane1 K CODE受信表示						*/
#define	D_RRH_REG_JSRX_JSRXKLOGL2				(D_RRH_REG_JSRX + 0x00001F18)	/* Lane2 K CODE受信表示						*/
#define	D_RRH_REG_JSRX_JSRXKLOGL3				(D_RRH_REG_JSRX + 0x00001F1C)	/* Lane3 K CODE受信表示						*/
#define	D_RRH_REG_JSRX_JSRXTEST					(D_RRH_REG_JSRX + 0x00001FF0)	/* JSRX TEST								*/
#define	D_RRH_REG_JSRX_JSRXMSEL					(D_RRH_REG_JSRX + 0x00001FFC)	/* JSRX Monitor								*/
/* @} */

/*!
 * @name JSRXレジスタbit & Shift
 * @note JSRXレジスタbit & Shift
 * @{
 */
/* @} */
#else
/*!
* @name	JSRXレジスタアドレス
* @note	JSRXレジスタアドレス
*@{
*/
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_JSRX_JSRXSRCTR				0xD2101020
#define	D_RRH_REG_JSRX_JSRXSYNCCTR				0xD2101030
#define	D_RRH_REG_JSRX_JSRXSYNCMON				0xD2101034
#define	D_RRH_REG_JSRX_JSRXEN					0xD2101080
#define	D_RRH_REG_JSRX_JSRXRF					0xD2101084
#define	D_RRH_REG_JSRX_JSRXRFTYP				0xD2101088
#define	D_RRH_REG_JSRX_JSRXCTR					0xD2101090
#define	D_RRH_REG_JSRX_JSRXMINDLY				0xD2101098
#define	D_RRH_REG_JSRX_JSRXRSMD					0xD21010A0
#define	D_RRH_REG_JSRX_JSRXCP0L0				0xD2101100
#define	D_RRH_REG_JSRX_JSRXCP1L0				0xD2101104
#define	D_RRH_REG_JSRX_JSRXCP2L0				0xD2101108
#define	D_RRH_REG_JSRX_JSRXCP3L0				0xD210110C
#define	D_RRH_REG_JSRX_JSRXCP0L1				0xD2101110
#define	D_RRH_REG_JSRX_JSRXCP1L1				0xD2101114
#define	D_RRH_REG_JSRX_JSRXCP2L1				0xD2101118
#define	D_RRH_REG_JSRX_JSRXCP3L1				0xD210111C
#define	D_RRH_REG_JSRX_JSRXXBAR					0xD2101200
#define	D_RRH_REG_JSRX_JSRXST					0xD2101300
#define	D_RRH_REG_JSRX_JSRXPFST					0xD2101304
#define	D_RRH_REG_JSRX_JSRXPEST					0xD2101308
#define	D_RRH_REG_JSRX_JSRXPCST					0xD210130C
#define	D_RRH_REG_JSRX_JSRXEST					0xD2101310
#define	D_RRH_REG_JSRX_JSRXPECL0				0xD2101320
#define	D_RRH_REG_JSRX_JSRXPECL1				0xD2101324
#define	D_RRH_REG_JSRX_JSRXFECL0				0xD2101340
#define	D_RRH_REG_JSRX_JSRXFECL1				0xD2101344
#define	D_RRH_REG_JSRX_JSRXDTIML0				0xD2101380
#define	D_RRH_REG_JSRX_JSRXDTIML1				0xD2101384
#define	D_RRH_REG_JSRX_JSRXIRQ					0xD2101400
#define	D_RRH_REG_JSRX_JSRXIRQC					0xD2101404
#define	D_RRH_REG_JSRX_JSRXIRQM					0xD2101408
#define	D_RRH_REG_JSRX_JSRXLMFC					0xD2101500
#define	D_RRH_REG_JSRX_JSRXLXSYNC				0xD2101504
#define	D_RRH_REG_JSRX_JSRXSTMON				0xD2101F00
#define	D_RRH_REG_JSRX_JSRXSTLOG				0xD2101F04
#define	D_RRH_REG_JSRX_JSRXKLOGL0				0xD2101F10
#define	D_RRH_REG_JSRX_JSRXKLOGL1				0xD2101F14
#define	D_RRH_REG_JSRX_JSRXTEST					0xD2101FF0
#define	D_RRH_REG_JSRX_JSRXMSEL					0xD2101FFC

#elif defined (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_JSRX_JSRXSRCTR				0x82101020
#define	D_RRH_REG_JSRX_JSRXSYNCCTR				0x82101030
#define	D_RRH_REG_JSRX_JSRXSYNCMON				0x82101034
#define	D_RRH_REG_JSRX_JSRXEN					0x82101080
#define	D_RRH_REG_JSRX_JSRXRF					0x82101084
#define	D_RRH_REG_JSRX_JSRXRFTYP				0x82101088
#define	D_RRH_REG_JSRX_JSRXCTR					0x82101090
#define	D_RRH_REG_JSRX_JSRXMINDLY				0x82101098
#define	D_RRH_REG_JSRX_JSRXRSMD					0x821010A0
#define	D_RRH_REG_JSRX_JSRXCP0L0				0x82101100
#define	D_RRH_REG_JSRX_JSRXCP1L0				0x82101104
#define	D_RRH_REG_JSRX_JSRXCP2L0				0x82101108
#define	D_RRH_REG_JSRX_JSRXCP3L0				0x8210110C
#define	D_RRH_REG_JSRX_JSRXCP0L1				0x82101110
#define	D_RRH_REG_JSRX_JSRXCP1L1				0x82101114
#define	D_RRH_REG_JSRX_JSRXCP2L1				0x82101118
#define	D_RRH_REG_JSRX_JSRXCP3L1				0x8210111C
#define	D_RRH_REG_JSRX_JSRXXBAR					0x82101200
#define	D_RRH_REG_JSRX_JSRXST					0x82101300
#define	D_RRH_REG_JSRX_JSRXPFST					0x82101304
#define	D_RRH_REG_JSRX_JSRXPEST					0x82101308
#define	D_RRH_REG_JSRX_JSRXPCST					0x8210130C
#define	D_RRH_REG_JSRX_JSRXEST					0x82101310
#define	D_RRH_REG_JSRX_JSRXPECL0				0x82101320
#define	D_RRH_REG_JSRX_JSRXPECL1				0x82101324
#define	D_RRH_REG_JSRX_JSRXFECL0				0x82101340
#define	D_RRH_REG_JSRX_JSRXFECL1				0x82101344
#define	D_RRH_REG_JSRX_JSRXDTIML0				0x82101380
#define	D_RRH_REG_JSRX_JSRXDTIML1				0x82101384
#define	D_RRH_REG_JSRX_JSRXIRQ					0x82101400
#define	D_RRH_REG_JSRX_JSRXIRQC					0x82101404
#define	D_RRH_REG_JSRX_JSRXIRQM					0x82101408
#define	D_RRH_REG_JSRX_JSRXLMFC					0x82101500
#define	D_RRH_REG_JSRX_JSRXLXSYNC				0x82101504
#define	D_RRH_REG_JSRX_JSRXSTMON				0x82101F00
#define	D_RRH_REG_JSRX_JSRXSTLOG				0x82101F04
#define	D_RRH_REG_JSRX_JSRXKLOGL0				0x82101F10
#define	D_RRH_REG_JSRX_JSRXKLOGL1				0x82101F14
#define	D_RRH_REG_JSRX_JSRXTEST					0x82101FF0
#define	D_RRH_REG_JSRX_JSRXMSEL					0x82101FFC
#else
#endif

/* @} */

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* JSRXIRQC */
#define	D_RRH_JSRX_JSRXIRQC_MINDE2				0x02000000
#define	D_RRH_JSRX_JSRXIRQC_MINDE1				0x01000000
#define	D_RRH_JSRX_JSRXIRQC_ILASE2				0x00200000
#define	D_RRH_JSRX_JSRXIRQC_ILASE1				0x00100000
#define	D_RRH_JSRX_JSRXIRQC_FRM2				0x00020000
#define	D_RRH_JSRX_JSRXIRQC_FRM1				0x00010000
#define	D_RRH_JSRX_JSRXIRQC_LTDE2				0x00002000
#define	D_RRH_JSRX_JSRXIRQC_LTDE1				0x00001000
#define	D_RRH_JSRX_JSRXIRQC_8B10B2				0x00000200
#define	D_RRH_JSRX_JSRXIRQC_8B10B1				0x00000100
#define	D_RRH_JSRX_JSRXIRQC_FIFOE2				0x00000020
#define	D_RRH_JSRX_JSRXIRQC_FIFOE1				0x00000010
#define	D_RRH_JSRX_JSRXIRQC_LDN2				0x00000002
#define	D_RRH_JSRX_JSRXIRQC_LDN1				0x00000001
#define	D_RRH_JSRX_JSRXIRQC_RXJESD_ALM_AB		( D_RRH_JSRX_JSRXIRQC_FRM1 \
												| D_RRH_JSRX_JSRXIRQC_LTDE1 | D_RRH_JSRX_JSRXIRQC_8B10B1 \
												| D_RRH_JSRX_JSRXIRQC_FIFOE1 | D_RRH_JSRX_JSRXIRQC_LDN1 )
#define	D_RRH_JSRX_JSRXIRQC_RXJESD_ALM_CD		( D_RRH_JSRX_JSRXIRQC_FRM2 \
												| D_RRH_JSRX_JSRXIRQC_LTDE2 | D_RRH_JSRX_JSRXIRQC_8B10B2 \
												| D_RRH_JSRX_JSRXIRQC_FIFOE2 | D_RRH_JSRX_JSRXIRQC_LDN2 )

/* @} */
#endif
#endif
/* @} */
