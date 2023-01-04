/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_jsfb.h
 * @brief レジスタアドレス定義ヘッダ(JSFB)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_JSFB_H
#define F_RRH_REG_JSFB_H

#if 0
/***** 5G-DUハソ仕様書対応	*****/
/***** D_RRH_REG_JSFB が仮設定であるため、既存defineを有効化	*****/
/*!
 * @name JSFBレジスタアドレス
 * @note JSFBレジスタアドレス
 * @{
 */
#define	D_RRH_REG_JSFB_JSESRCTR					(D_RRH_REG_JSFB & 0x00004000)	/* External SYSREF Control					*/
#define	D_RRH_REG_JSFB_JSESRLOG					(D_RRH_REG_JSFB & 0x00004004)	/* External SYSREF Log						*/
#define	D_RRH_REG_JSFB_JSFBSRCTR				(D_RRH_REG_JSFB & 0x00004020)	/* Internal SYSREF Control					*/
#define	D_RRH_REG_JSFB_JSFBSYNCCTR				(D_RRH_REG_JSFB & 0x00004030)	/* JSFB XSYNC Control						*/
#define	D_RRH_REG_JSFB_JSFBSYNCMON				(D_RRH_REG_JSFB & 0x00004034)	/* JSFB XSYNC Monitor						*/
#define	D_RRH_REG_JSFB_JSFBEN					(D_RRH_REG_JSFB & 0x00004080)	/* JSFB JESD Enable							*/
#define	D_RRH_REG_JSFB_JSFBRF					(D_RRH_REG_JSFB & 0x00004084)	/* JSFB JESD Use RF Device Num				*/
#define	D_RRH_REG_JSFB_JSFBCTR					(D_RRH_REG_JSFB & 0x00004090)	/* JSFB JESD Control						*/
#define	D_RRH_REG_JSFB_JSFBMINDLY				(D_RRH_REG_JSFB & 0x00004098)	/* JSFB JESD Minimum Latency Value			*/
#define	D_RRH_REG_JSFB_JSFBRSMD					(D_RRH_REG_JSFB & 0x000040A0)	/* JSFB Re-Synchronaization Mode			*/
#define	D_RRH_REG_JSFB_JSFBCP0L0				(D_RRH_REG_JSFB & 0x00004100)	/* JSFB Config Parameter0 (Lane0) 			*/
#define	D_RRH_REG_JSFB_JSFBCP1L0				(D_RRH_REG_JSFB & 0x00004104)	/* JSFB Config Parameter1 (Lane0) 			*/
#define	D_RRH_REG_JSFB_JSFBCP2L0				(D_RRH_REG_JSFB & 0x00004108)	/* JSFB Config Parameter2 (Lane0) 			*/
#define	D_RRH_REG_JSFB_JSFBCP3L0				(D_RRH_REG_JSFB & 0x0000410C)	/* JSFB Config Parameter3 (Lane0) 			*/
#define	D_RRH_REG_JSFB_JSFBCP0L1				(D_RRH_REG_JSFB & 0x00004110)	/* JSFB Config Parameter0 (Lane1) 			*/
#define	D_RRH_REG_JSFB_JSFBCP1L1				(D_RRH_REG_JSFB & 0x00004114)	/* JSFB Config Parameter1 (Lane1) 			*/
#define	D_RRH_REG_JSFB_JSFBCP2L1				(D_RRH_REG_JSFB & 0x00004118)	/* JSFB Config Parameter2 (Lane1) 			*/
#define	D_RRH_REG_JSFB_JSFBCP3L1				(D_RRH_REG_JSFB & 0x0000411C)	/* JSFB Config Parameter3 (Lane1) 			*/
#define	D_RRH_REG_JSFB_JSFBCP0L2				(D_RRH_REG_JSFB & 0x00004120)	/* JSFB Config Parameter0 (Lane2) 			*/
#define	D_RRH_REG_JSFB_JSFBCP1L2				(D_RRH_REG_JSFB & 0x00004124)	/* JSFB Config Parameter1 (Lane2) 			*/
#define	D_RRH_REG_JSFB_JSFBCP2L2				(D_RRH_REG_JSFB & 0x00004128)	/* JSFB Config Parameter2 (Lane2) 			*/
#define	D_RRH_REG_JSFB_JSFBCP3L2				(D_RRH_REG_JSFB & 0x0000412C)	/* JSFB Config Parameter3 (Lane2) 			*/
#define	D_RRH_REG_JSFB_JSFBCP0L3				(D_RRH_REG_JSFB & 0x00004130)	/* JSFB Config Parameter0 (Lane3) 			*/
#define	D_RRH_REG_JSFB_JSFBCP1L3				(D_RRH_REG_JSFB & 0x00004134)	/* JSFB Config Parameter1 (Lane3) 			*/
#define	D_RRH_REG_JSFB_JSFBCP2L3				(D_RRH_REG_JSFB & 0x00004138)	/* JSFB Config Parameter2 (Lane3) 			*/
#define	D_RRH_REG_JSFB_JSFBCP3L3				(D_RRH_REG_JSFB & 0x0000413C)	/* JSFB Config Parameter3 (Lane3) 			*/
#define	D_RRH_REG_JSFB_JSFBXBAR					(D_RRH_REG_JSFB & 0x00004200)	/* JSFB Antena Select						*/
#define	D_RRH_REG_JSFB_JSFBST					(D_RRH_REG_JSFB & 0x00004300)	/* JSFB Status								*/
#define	D_RRH_REG_JSFB_JSFBPFST					(D_RRH_REG_JSFB & 0x00004304)	/* JSFB PHY FIFO Status						*/
#define	D_RRH_REG_JSFB_JSFBPEST					(D_RRH_REG_JSFB & 0x00004308)	/* JSFB PHY Error Status					*/
#define	D_RRH_REG_JSFB_JSFBPCST					(D_RRH_REG_JSFB & 0x0000430C)	/* JSFB PHY CDR Status						*/
#define	D_RRH_REG_JSFB_JSFBEST					(D_RRH_REG_JSFB & 0x00004310)	/* JSFB JESD Error Status					*/
#define	D_RRH_REG_JSFB_JSFBPECL0				(D_RRH_REG_JSFB & 0x00004320)	/* Lane0 8B10B Error Count (Read Clear Type)		*/
#define	D_RRH_REG_JSFB_JSFBPECL1				(D_RRH_REG_JSFB & 0x00004324)	/* Lane1 8B10B Error Count (Read Clear Type)		*/
#define	D_RRH_REG_JSFB_JSFBPECL2				(D_RRH_REG_JSFB & 0x00004328)	/* Lane2 8B10B Error Count (Read Clear Type)		*/
#define	D_RRH_REG_JSFB_JSFBPECL3				(D_RRH_REG_JSFB & 0x0000432C)	/* Lane3 8B10B Error Count (Read Clear Type)		*/
#define	D_RRH_REG_JSFB_JSFBFECL0				(D_RRH_REG_JSFB & 0x00004340)	/* Lane0 Frame周期 Error Count (Read Clear Type)	*/
#define	D_RRH_REG_JSFB_JSFBFECL1				(D_RRH_REG_JSFB & 0x00004344)	/* Lane1 Frame周期 Error Count (Read Clear Type)	*/
#define	D_RRH_REG_JSFB_JSFBFECL2				(D_RRH_REG_JSFB & 0x00004348)	/* Lane2 Frame周期 Error Count (Read Clear Type)	*/
#define	D_RRH_REG_JSFB_JSFBFECL3				(D_RRH_REG_JSFB & 0x0000434C)	/* Lane3 Frame周期 Error Count (Read Clear Type)	*/
#define	D_RRH_REG_JSFB_JSFBDTIML0				(D_RRH_REG_JSFB & 0x00004380)	/* JSFB JESD Data Input Timing (Lane0)		*/
#define	D_RRH_REG_JSFB_JSFBDTIML1				(D_RRH_REG_JSFB & 0x00004384)	/* JSFB JESD Data Input Timing (Lane1)		*/
#define	D_RRH_REG_JSFB_JSFBDTIML2				(D_RRH_REG_JSFB & 0x00004388)	/* JSFB JESD Data Input Timing (Lane2)		*/
#define	D_RRH_REG_JSFB_JSFBDTIML3				(D_RRH_REG_JSFB & 0x0000438C)	/* JSFB JESD Data Input Timing (Lane3)		*/
#define	D_RRH_REG_JSFB_JSFBIRQ					(D_RRH_REG_JSFB & 0x00004400)	/* JSFB IRQ									*/
#define	D_RRH_REG_JSFB_JSFBIRQC					(D_RRH_REG_JSFB & 0x00004404)	/* JSFB IRQ									*/
#define	D_RRH_REG_JSFB_JSFBIRQM					(D_RRH_REG_JSFB & 0x00004408)	/* JSFB IRQ									*/
#define	D_RRH_REG_JSFB_JSFBLMFC					(D_RRH_REG_JSFB & 0x00004500)	/* JSFB Timing Offset						*/
#define	D_RRH_REG_JSFB_JSFBLXSYNC				(D_RRH_REG_JSFB & 0x00004504)	/* JSFB Timing Offset						*/
#define	D_RRH_REG_JSFB_JSFBLALIGN				(D_RRH_REG_JSFB & 0x00004508)	/* JSFB Timing Offset						*/
#define	D_RRH_REG_JSFB_JSFBSTMON				(D_RRH_REG_JSFB & 0x00004F00)	/* JSFB State Monitor						*/
#define	D_RRH_REG_JSFB_JSFBSTLOG				(D_RRH_REG_JSFB & 0x00004F04)	/* JSFB State Change Log					*/
#define	D_RRH_REG_JSFB_JSFBKLOGL0				(D_RRH_REG_JSFB & 0x00004F10)	/* Lane0 K Code Log							*/
#define	D_RRH_REG_JSFB_JSFBKLOGL1				(D_RRH_REG_JSFB & 0x00004F14)	/* Lane1 K Code Log							*/
#define	D_RRH_REG_JSFB_JSFBKLOGL2				(D_RRH_REG_JSFB & 0x00004F18)	/* Lane2 K Code Log							*/
#define	D_RRH_REG_JSFB_JSFBKLOGL3				(D_RRH_REG_JSFB & 0x00004F1C)	/* Lane3 K Code Log							*/
#define	D_RRH_REG_JSFB_JSFBTEST					(D_RRH_REG_JSFB & 0x00004FF0)	/* JSFB TEST								*/
#define	D_RRH_REG_JSFB_JSFBMSEL					(D_RRH_REG_JSFB & 0x00004FFC)	/* JSFB Monitor								*/
/* @} */

/*!
 * @name JSFBレジスタbit & Shift
 * @note JSFBレジスタbit & Shift
 * @{
 */
/* @} */
#else
/***** 3.5G-RRE 4TRXハソ仕様書対応 *****/
/*!
 * @name JSFBレジスタアドレス
 * @note JSFBレジスタアドレス
 * @{
 */
#define	D_RRH_REG_JSFB_JSFBSRCTR				0x82004020
#define	D_RRH_REG_JSFB_JSFBSYNCCTR				0x82004030
#define	D_RRH_REG_JSFB_JSFBSYNCMON				0x82004034
#define	D_RRH_REG_JSFB_JSFBEN					0x82004080
#define	D_RRH_REG_JSFB_JSFBRF					0x82004084
#define	D_RRH_REG_JSFB_JSFBCTR					0x82004090
#define	D_RRH_REG_JSFB_JSFBMINDLY				0x82004098
#define	D_RRH_REG_JSFB_JSFBRSMD					0x820040A0
#define	D_RRH_REG_JSFB_JSFBCP0L0				0x82004100
#define	D_RRH_REG_JSFB_JSFBCP1L0				0x82004104
#define	D_RRH_REG_JSFB_JSFBCP2L0				0x82004108
#define	D_RRH_REG_JSFB_JSFBCP3L0				0x8200410C
#define	D_RRH_REG_JSFB_JSFBCP0L1				0x82004110
#define	D_RRH_REG_JSFB_JSFBCP1L1				0x82004114
#define	D_RRH_REG_JSFB_JSFBCP2L1				0x82004118
#define	D_RRH_REG_JSFB_JSFBCP3L1				0x8200411C
#define	D_RRH_REG_JSFB_JSFBCP0L2				0x82004120
#define	D_RRH_REG_JSFB_JSFBCP1L2				0x82004124
#define	D_RRH_REG_JSFB_JSFBCP2L2				0x82004128
#define	D_RRH_REG_JSFB_JSFBCP3L2				0x8200412C
#define	D_RRH_REG_JSFB_JSFBCP0L3				0x82004130
#define	D_RRH_REG_JSFB_JSFBCP1L3				0x82004134
#define	D_RRH_REG_JSFB_JSFBCP2L3				0x82004138
#define	D_RRH_REG_JSFB_JSFBCP3L3				0x8200413C
#define	D_RRH_REG_JSFB_JSFBXBAR					0x82004200
#define	D_RRH_REG_JSFB_JSFBST					0x82004300
#define	D_RRH_REG_JSFB_JSFBPFST					0x82004304
#define	D_RRH_REG_JSFB_JSFBPEST					0x82004308
#define	D_RRH_REG_JSFB_JSFBPCST					0x8200430C
#define	D_RRH_REG_JSFB_JSFBEST					0x82004310
#define	D_RRH_REG_JSFB_JSFBPECL0				0x82004320
#define	D_RRH_REG_JSFB_JSFBPECL1				0x82004324
#define	D_RRH_REG_JSFB_JSFBPECL2				0x82004328
#define	D_RRH_REG_JSFB_JSFBPECL3				0x8200432C
#define	D_RRH_REG_JSFB_JSFBFECL0				0x82004340
#define	D_RRH_REG_JSFB_JSFBFECL1				0x82004344
#define	D_RRH_REG_JSFB_JSFBFECL2				0x82004348
#define	D_RRH_REG_JSFB_JSFBFECL3				0x8200434C
#define	D_RRH_REG_JSFB_JSFBDTIML0				0x82004380
#define	D_RRH_REG_JSFB_JSFBDTIML1				0x82004384
#define	D_RRH_REG_JSFB_JSFBDTIML2				0x82004388
#define	D_RRH_REG_JSFB_JSFBDTIML3				0x8200438C
#define	D_RRH_REG_JSFB_JSFBIRQ					0x82004400
#define	D_RRH_REG_JSFB_JSFBIRQC					0x82004404
#define	D_RRH_REG_JSFB_JSFBIRQM					0x82004408
#define	D_RRH_REG_JSFB_JSFBLMFC					0x82004500
#define	D_RRH_REG_JSFB_JSFBLXSYNC				0x82004504
#define	D_RRH_REG_JSFB_JSFBSTMON				0x82004F00
#define	D_RRH_REG_JSFB_JSFBSTLOG				0x82004F04
#define	D_RRH_REG_JSFB_JSFBKLOGL0				0x82004F10
#define	D_RRH_REG_JSFB_JSFBKLOGL1				0x82004F14
#define	D_RRH_REG_JSFB_JSFBKLOGL2				0x82004F18
#define	D_RRH_REG_JSFB_JSFBKLOGL3				0x82004F1C
#define	D_RRH_REG_JSFB_JSFBTEST					0x82004FF0
#define	D_RRH_REG_JSFB_JSFBMSEL					0x82004FFC

/* @} */

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* JSFBIRQC */
#define	D_RRH_JSFB_JSFBIRQC_LAE2				0x20000000
#define	D_RRH_JSFB_JSFBIRQC_LAE1				0x10000000
#define	D_RRH_JSFB_JSFBIRQC_MINDE2				0x02000000
#define	D_RRH_JSFB_JSFBIRQC_MINDE1				0x01000000
#define	D_RRH_JSFB_JSFBIRQC_ILASE2				0x00200000
#define	D_RRH_JSFB_JSFBIRQC_ILASE1				0x00100000
#define	D_RRH_JSFB_JSFBIRQC_FRM2				0x00020000
#define	D_RRH_JSFB_JSFBIRQC_FRM1				0x00010000
#define	D_RRH_JSFB_JSFBIRQC_LTDE2				0x00002000
#define	D_RRH_JSFB_JSFBIRQC_LTDE1				0x00001000
#define	D_RRH_JSFB_JSFBIRQC_8B10B2				0x00000200
#define	D_RRH_JSFB_JSFBIRQC_8B10B1				0x00000100
#define	D_RRH_JSFB_JSFBIRQC_FIFOE2				0x00000020
#define	D_RRH_JSFB_JSFBIRQC_FIFOE1				0x00000010
#define	D_RRH_JSFB_JSFBIRQC_LDN2				0x00000002
#define	D_RRH_JSFB_JSFBIRQC_LDN1				0x00000001
#define	D_RRH_JSFB_JSFBIRQC_FBJESD_ALM_AB		( D_RRH_JSFB_JSFBIRQC_FRM1 | D_RRH_JSFB_JSFBIRQC_LTDE1 \
												| D_RRH_JSFB_JSFBIRQC_8B10B1 | D_RRH_JSFB_JSFBIRQC_FIFOE1 \
												| D_RRH_JSFB_JSFBIRQC_LDN1 )
#define	D_RRH_JSFB_JSFBIRQC_FBJESD_ALM_CD		( D_RRH_JSFB_JSFBIRQC_FRM2 | D_RRH_JSFB_JSFBIRQC_LTDE2 \
												| D_RRH_JSFB_JSFBIRQC_8B10B2 | D_RRH_JSFB_JSFBIRQC_FIFOE2 \
												| D_RRH_JSFB_JSFBIRQC_LDN2 )
/* @} */
#endif
#endif
/* @} */
