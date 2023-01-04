/*!
 * @skip  $ld:$
 * @file  f_du_reg_rxb.h
 * @brief レジスタアドレス定義ヘッダ(RXB)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_RXB_H
#define F_DU_REG_RXB_H

/*!
 * @name RXBレジスタアドレス
 * @note RXBレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define	D_DU_REG_RXB_RXCAREN				0xB1300000		/* RXCAREN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXBW0					0xB1300010		/* RXBW0		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXBW1					0xB1300014		/* RXBW1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXBW2					0xB1300018		/* RXBW2		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXBW3					0xB130001C		/* RXBW3		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXBW4					0xB1300020		/* RXBW4		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXBW5					0xB1300024		/* RXBW5		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXBW6					0xB1300028		/* RXBW6		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXBW7					0xB130002C		/* RXBW7		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXANTEN				0xB1300030		/* RXANTEN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN0				0xB1300080		/* RXCHFGAIN0	(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN1				0xB1300084		/* RXCHFGAIN1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN2				0xB1300088		/* RXCHFGAIN2	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN3				0xB130008C		/* RXCHFGAIN3	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN4				0xB1300090		/* RXCHFGAIN4	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN5				0xB1300094		/* RXCHFGAIN5	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN6				0xB1300098		/* RXCHFGAIN6	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFGAIN7				0xB130009C		/* RXCHFGAIN7	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN0				0xB1300100		/* RXDGCGAIN0	(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN1				0xB1300104		/* RXDGCGAIN1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN2				0xB1300108		/* RXDGCGAIN2	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN3				0xB130010C		/* RXDGCGAIN3	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN4				0xB1300110		/* RXDGCGAIN4	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN5				0xB1300114		/* RXDGCGAIN5	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN6				0xB1300118		/* RXDGCGAIN6	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXDGCGAIN7				0xB130011C		/* RXDGCGAIN7	(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF0					0xB1300120		/* RXCHF0		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF1					0xB1300124		/* RXCHF1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF2					0xB1300128		/* RXCHF2		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF3					0xB130012C		/* RXCHF3		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF4					0xB1300130		/* RXCHF4		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF5					0xB1300134		/* RXCHF5		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF6					0xB1300138		/* RXCHF6		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHF7					0xB130013C		/* RXCHF7		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXCHFSEL				0xB1300140		/* RXCHFSEL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP0				0xB1300150		/* RTWPREP0		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP1				0xB1300154		/* RTWPREP1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP2				0xB1300158		/* RTWPREP2		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP3				0xB130015C		/* RTWPREP3		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP4				0xB1300160		/* RTWPREP4		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP5				0xB1300164		/* RTWPREP5		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP6				0xB1300168		/* RTWPREP6		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPREP7				0xB130016C		/* RTWPREP7		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RTWPSMPL				0xB1300170		/* RTWPSMPL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RTWPLAT				0xB1300174		/* RTWPLAT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RTWP					0xB1300178		/* RTWP			(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO0					0xB1300200		/* RXNCO0		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO1					0xB1300204		/* RXNCO1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO2					0xB1300208		/* RXNCO2		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO3					0xB130020C		/* RXNCO3		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO4					0xB1300210		/* RXNCO4		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO5					0xB1300214		/* RXNCO5		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO6					0xB1300218		/* RXNCO6		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCO7					0xB130021C		/* RXNCO7		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_QCHINV					0xB1300220		/* QCHINV		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXPOWREP				0xB1300230		/* RXPOWREP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXPOWSMPL				0xB1300234		/* RXPOWSMPL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXPOWLAT				0xB1300238		/* RXPOWLAT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXPOW					0xB130023C		/* RXPOW		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDCREP				0xB1300240		/* RXDCREP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDCSMPL				0xB1300244		/* RXDCSMPL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDCLAT				0xB1300248		/* RXDCLAT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXDC					0xB130024C		/* RXDC			(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH0				0xB13002C0		/* RXNCOPH0		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH1				0xB13002C4		/* RXNCOPH1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH2				0xB13002C8		/* RXNCOPH2		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH3				0xB13002CC		/* RXNCOPH3		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH4				0xB13002D0		/* RXNCOPH4		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH5				0xB13002D4		/* RXNCOPH5		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH6				0xB13002D8		/* RXNCOPH6		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXNCOPH7				0xB13002DC		/* RXNCOPH7		(sub6 : - / mmw : o)*/
#define	D_DU_REG_RXB_RXTEST					0xB13001F8		/* RXTEST		(sub6 : o / mmw : o)*/
#define	D_DU_REG_RXB_RXMONSEL				0xB13001FC		/* RXMONSEL		(sub6 : o / mmw : o)*/
/* @} */

/*!
 * @name RXBレジスタアドレス算出マクロ
 * @note RXBレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_RXB_PATH					0x00000400
#define M_DU_REG_RXB_GETADDR(ofs, path)		(UINT)(ofs  + (path * D_DU_OFT_RXB_PATH))
#define	M_DU_REG_RXB_RXCAREN(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCAREN, path)
#define	M_DU_REG_RXB_RXBW0(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW0, path)
#define	M_DU_REG_RXB_RXBW1(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW1, path)
#define	M_DU_REG_RXB_RXBW2(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW2, path)
#define	M_DU_REG_RXB_RXBW3(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW3, path)
#define	M_DU_REG_RXB_RXBW4(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW4, path)
#define	M_DU_REG_RXB_RXBW5(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW5, path)
#define	M_DU_REG_RXB_RXBW6(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW6, path)
#define	M_DU_REG_RXB_RXBW7(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXBW7, path)
#define	M_DU_REG_RXB_RXANTEN(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXANTEN, path)
#define	M_DU_REG_RXB_RXCHFGAIN0(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN0, path)
#define	M_DU_REG_RXB_RXCHFGAIN1(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN1, path)
#define	M_DU_REG_RXB_RXCHFGAIN2(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN2, path)
#define	M_DU_REG_RXB_RXCHFGAIN3(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN3, path)
#define	M_DU_REG_RXB_RXCHFGAIN4(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN4, path)
#define	M_DU_REG_RXB_RXCHFGAIN5(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN5, path)
#define	M_DU_REG_RXB_RXCHFGAIN6(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN6, path)
#define	M_DU_REG_RXB_RXCHFGAIN7(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFGAIN7, path)
#define	M_DU_REG_RXB_RXDGCGAIN0(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN0, path)
#define	M_DU_REG_RXB_RXDGCGAIN1(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN1, path)
#define	M_DU_REG_RXB_RXDGCGAIN2(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN2, path)
#define	M_DU_REG_RXB_RXDGCGAIN3(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN3, path)
#define	M_DU_REG_RXB_RXDGCGAIN4(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN4, path)
#define	M_DU_REG_RXB_RXDGCGAIN5(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN5, path)
#define	M_DU_REG_RXB_RXDGCGAIN6(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN6, path)
#define	M_DU_REG_RXB_RXDGCGAIN7(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDGCGAIN7, path)
#define	M_DU_REG_RXB_RXCHF0(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF0, path)
#define	M_DU_REG_RXB_RXCHF1(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF1, path)
#define	M_DU_REG_RXB_RXCHF2(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF2, path)
#define	M_DU_REG_RXB_RXCHF3(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF3, path)
#define	M_DU_REG_RXB_RXCHF4(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF4, path)
#define	M_DU_REG_RXB_RXCHF5(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF5, path)
#define	M_DU_REG_RXB_RXCHF6(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF6, path)
#define	M_DU_REG_RXB_RXCHF7(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHF7, path)
#define	M_DU_REG_RXB_RXCHFSEL(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXCHFSEL, path)
#define	M_DU_REG_RXB_RTWPREP0(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP0, path)
#define	M_DU_REG_RXB_RTWPREP1(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP1, path)
#define	M_DU_REG_RXB_RTWPREP2(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP2, path)
#define	M_DU_REG_RXB_RTWPREP3(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP3, path)
#define	M_DU_REG_RXB_RTWPREP4(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP4, path)
#define	M_DU_REG_RXB_RTWPREP5(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP5, path)
#define	M_DU_REG_RXB_RTWPREP6(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP6, path)
#define	M_DU_REG_RXB_RTWPREP7(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPREP7, path)
#define	M_DU_REG_RXB_RTWPSMPL(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPSMPL, path)
#define	M_DU_REG_RXB_RTWPLAT(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWPLAT, path)
#define	M_DU_REG_RXB_RTWP(path)				M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RTWP, path)
#define	M_DU_REG_RXB_RXNCO0(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO0, path)
#define	M_DU_REG_RXB_RXNCO1(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO1, path)
#define	M_DU_REG_RXB_RXNCO2(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO2, path)
#define	M_DU_REG_RXB_RXNCO3(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO3, path)
#define	M_DU_REG_RXB_RXNCO4(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO4, path)
#define	M_DU_REG_RXB_RXNCO5(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO5, path)
#define	M_DU_REG_RXB_RXNCO6(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO6, path)
#define	M_DU_REG_RXB_RXNCO7(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCO7, path)
#define	M_DU_REG_RXB_QCHINV(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_QCHINV, path)
#define	M_DU_REG_RXB_RXPOWREP(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXPOWREP, path)
#define	M_DU_REG_RXB_RXPOWSMPL(path)		M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXPOWSMP, path)
#define	M_DU_REG_RXB_RXPOWLAT(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXPOWLAT, path)
#define	M_DU_REG_RXB_RXPOW(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXPOW, path)
#define	M_DU_REG_RXB_RXDCREP(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDCREP, path)
#define	M_DU_REG_RXB_RXDCSMPL(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDCSMPL, path)
#define	M_DU_REG_RXB_RXDCLAT(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDCLAT, path)
#define	M_DU_REG_RXB_RXDC(path)				M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXDC, path)
#define	M_DU_REG_RXB_RXNCOPH0(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH0, path)
#define	M_DU_REG_RXB_RXNCOPH1(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH1, path)
#define	M_DU_REG_RXB_RXNCOPH2(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH2, path)
#define	M_DU_REG_RXB_RXNCOPH3(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH3, path)
#define	M_DU_REG_RXB_RXNCOPH4(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH4, path)
#define	M_DU_REG_RXB_RXNCOPH5(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH5, path)
#define	M_DU_REG_RXB_RXNCOPH6(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH6, path)
#define	M_DU_REG_RXB_RXNCOPH7(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXNCOPH7, path)
#define	M_DU_REG_RXB_RXTEST(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXTEST, path)
#define	M_DU_REG_RXB_RXMONSEL(path)			M_DU_REG_RXB_GETADDR(D_DU_REG_RXB_RXMONSEL, path)
/* @} */
#endif
/* @} */
