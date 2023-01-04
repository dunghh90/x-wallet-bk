/*!
 * @skip  $ld:$
 * @file  f_du_reg_rx.h
 * @brief レジスタアドレス定義ヘッダ(RX)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_RX_H
#define F_DU_REG_RX_H

/*!
 * @name RXレジスタアドレス
 * @note RXレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_RX_RXCAREN					0xB1300000		/* RXCAREN		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXBW0					0xB1300010		/* RXBW0		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXBW1					0xB1300014		/* RXBW1		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXBW2					0xB1300018		/* RXBW2		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXBW3					0xB130001C		/* RXBW3		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXBW4					0xB1300020		/* RXBW4		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXBW5					0xB1300024		/* RXBW5		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXBW6					0xB1300028		/* RXBW6		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXBW7					0xB130002C		/* RXBW7		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXANTEN					0xB1300030		/* RXANTEN		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN0				0xB1300080		/* RXCHFGAIN0	(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN1				0xB1300084		/* RXCHFGAIN1	(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN2				0xB1300088		/* RXCHFGAIN2	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN3				0xB130008C		/* RXCHFGAIN3	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN4				0xB1300090		/* RXCHFGAIN4	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN5				0xB1300094		/* RXCHFGAIN5	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN6				0xB1300098		/* RXCHFGAIN6	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFGAIN7				0xB130009C		/* RXCHFGAIN7	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN0				0xB1300100		/* RXDGCGAIN0	(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN1				0xB1300104		/* RXDGCGAIN1	(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN2				0xB1300108		/* RXDGCGAIN2	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN3				0xB130010C		/* RXDGCGAIN3	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN4				0xB1300110		/* RXDGCGAIN4	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN5				0xB1300114		/* RXDGCGAIN5	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN6				0xB1300118		/* RXDGCGAIN6	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXDGCGAIN7				0xB130011C		/* RXDGCGAIN7	(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF0					0xB1300120		/* RXCHF0		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXCHF1					0xB1300124		/* RXCHF1		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXCHF2					0xB1300128		/* RXCHF2		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF3					0xB130012C		/* RXCHF3		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF4					0xB1300130		/* RXCHF4		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF5					0xB1300134		/* RXCHF5		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF6					0xB1300138		/* RXCHF6		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHF7					0xB130013C		/* RXCHF7		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXCHFSEL				0xB1300140		/* RXCHFSEL		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RTWPREP0				0xB1300150		/* RTWPREP0		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RTWPREP1				0xB1300154		/* RTWPREP1		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RTWPREP2				0xB1300158		/* RTWPREP2		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPREP3				0xB130015C		/* RTWPREP3		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPREP4				0xB1300160		/* RTWPREP4		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPREP5				0xB1300164		/* RTWPREP5		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPREP6				0xB1300168		/* RTWPREP6		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPREP7				0xB130016C		/* RTWPREP7		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RTWPSMPL				0xB1300170		/* RTWPSMPL		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RTWPLAT					0xB1300174		/* RTWPLAT		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RTWP					0xB1300178		/* RTWP			(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCO0					0xB1300200		/* RXNCO0		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCO1					0xB1300204		/* RXNCO1		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCO2					0xB1300208		/* RXNCO2		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCO3					0xB130020C		/* RXNCO3		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCO4					0xB1300210		/* RXNCO4		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCO5					0xB1300214		/* RXNCO5		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCO6					0xB1300218		/* RXNCO6		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCO7					0xB130021C		/* RXNCO7		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_QCHINV					0xB1300220		/* QCHINV		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXPOWREP				0xB1300230		/* RXPOWREP		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXPOWSMPL				0xB1300234		/* RXPOWSMPL	(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXPOWLAT				0xB1300238		/* RXPOWLAT		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXPOW					0xB130023C		/* RXPOW		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDCREP					0xB1300240		/* RXDCREP		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDCSMPL				0xB1300244		/* RXDCSMPL		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDCLAT					0xB1300248		/* RXDCLAT		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXDC					0xB130024C		/* RXDC			(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH0				0xB13002C0		/* RXNCOPH0		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH1				0xB13002C4		/* RXNCOPH1		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH2				0xB13002C8		/* RXNCOPH2		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH3				0xB13002CC		/* RXNCOPH3		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH4				0xB13002D0		/* RXNCOPH4		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH5				0xB13002D4		/* RXNCOPH5		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH6				0xB13002D8		/* RXNCOPH6		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXNCOPH7				0xB13002DC		/* RXNCOPH7		(sub6 : - / mmw : o)*/
#define D_DU_REG_RX_RXTEST					0xB13003F8		/* RXTEST		(sub6 : o / mmw : o)*/
#define D_DU_REG_RX_RXMONSEL				0xB13003FC		/* RXMONSEL		(sub6 : o / mmw : o)*/

/* @} */

/*!
 * @name RXレジスタアドレス算出マクロ
 * @note RXレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Path は0(PathA)～1(PathB), unit は0(TRX-FPGA#0)～1(TRX-FPGA#1)である
 * @{
 */
#define	D_DU_OFT_RX_PATH					0x00000400
#define	D_DU_OFT_RX_UNIT					0x00000000
#define	D_DU_OFT_RX_CC						0x00000004

#define M_DU_REG_RX_GETADDR(ofs,unit,path)	(UINT)(ofs + (unit * D_DU_OFT_RX_UNIT) + (path * D_DU_OFT_RX_PATH))
#define M_DU_REG_RX_GETADDR2(ofs,ccno,path)	(UINT)(ofs + (ccno * D_DU_OFT_RX_CC) + (path * D_DU_OFT_RX_PATH))
#define M_DU_REG_RX_RXCAREN(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCAREN,unit,path)
#define M_DU_REG_RX_RXBW0(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW0,unit,path)
#define M_DU_REG_RX_RXBW1(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW1,unit,path)
#define M_DU_REG_RX_RXBW2(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW2,unit,path)
#define M_DU_REG_RX_RXBW3(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW3,unit,path)
#define M_DU_REG_RX_RXBW4(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW4,unit,path)
#define M_DU_REG_RX_RXBW5(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW5,unit,path)
#define M_DU_REG_RX_RXBW6(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW6,unit,path)
#define M_DU_REG_RX_RXBW7(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXBW7,unit,path)
#define M_DU_REG_RX_RXANTEN(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXANTEN,unit,path)
#define M_DU_REG_RX_RXCHFGAIN(ccno,path)	M_DU_REG_RX_GETADDR2(D_DU_REG_RX_RXCHFGAIN0,ccno,path)
#define M_DU_REG_RX_RXCHFGAIN0(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN0,unit,path)
#define M_DU_REG_RX_RXCHFGAIN1(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN1,unit,path)
#define M_DU_REG_RX_RXCHFGAIN2(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN2,unit,path)
#define M_DU_REG_RX_RXCHFGAIN3(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN3,unit,path)
#define M_DU_REG_RX_RXCHFGAIN4(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN4,unit,path)
#define M_DU_REG_RX_RXCHFGAIN5(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN5,unit,path)
#define M_DU_REG_RX_RXCHFGAIN6(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN6,unit,path)
#define M_DU_REG_RX_RXCHFGAIN7(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFGAIN7,unit,path)
#define M_DU_REG_RX_RXDGCGAIN0(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN0,unit,path)
#define M_DU_REG_RX_RXDGCGAIN1(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN1,unit,path)
#define M_DU_REG_RX_RXDGCGAIN2(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN2,unit,path)
#define M_DU_REG_RX_RXDGCGAIN3(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN3,unit,path)
#define M_DU_REG_RX_RXDGCGAIN4(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN4,unit,path)
#define M_DU_REG_RX_RXDGCGAIN5(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN5,unit,path)
#define M_DU_REG_RX_RXDGCGAIN6(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN6,unit,path)
#define M_DU_REG_RX_RXDGCGAIN7(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDGCGAIN7,unit,path)
#define M_DU_REG_RX_RXCHF0(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF0,unit,path)
#define M_DU_REG_RX_RXCHF1(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF1,unit,path)
#define M_DU_REG_RX_RXCHF2(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF2,unit,path)
#define M_DU_REG_RX_RXCHF3(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF3,unit,path)
#define M_DU_REG_RX_RXCHF4(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF4,unit,path)
#define M_DU_REG_RX_RXCHF5(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF5,unit,path)
#define M_DU_REG_RX_RXCHF6(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF6,unit,path)
#define M_DU_REG_RX_RXCHF7(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHF7,unit,path)
#define M_DU_REG_RX_RXCHFSEL(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXCHFSEL,unit,path)
#define M_DU_REG_RX_RTWPREP0(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP0,unit,path)
#define M_DU_REG_RX_RTWPREP1(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP1,unit,path)
#define M_DU_REG_RX_RTWPREP2(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP2,unit,path)
#define M_DU_REG_RX_RTWPREP3(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP3,unit,path)
#define M_DU_REG_RX_RTWPREP4(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP4,unit,path)
#define M_DU_REG_RX_RTWPREP5(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP5,unit,path)
#define M_DU_REG_RX_RTWPREP6(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP6,unit,path)
#define M_DU_REG_RX_RTWPREP7(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPREP7,unit,path)
#define M_DU_REG_RX_RTWPSMPL(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPSMPL,unit,path)
#define M_DU_REG_RX_RTWPLAT(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWPLAT,unit,path)
#define M_DU_REG_RX_RTWP(unit,path)			M_DU_REG_RX_GETADDR(D_DU_REG_RX_RTWP,unit,path)
#define M_DU_REG_RX_RXNCO0(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO0,unit,path)
#define M_DU_REG_RX_RXNCO1(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO1,unit,path)
#define M_DU_REG_RX_RXNCO2(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO2,unit,path)
#define M_DU_REG_RX_RXNCO3(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO3,unit,path)
#define M_DU_REG_RX_RXNCO4(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO4,unit,path)
#define M_DU_REG_RX_RXNCO5(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO5,unit,path)
#define M_DU_REG_RX_RXNCO6(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO6,unit,path)
#define M_DU_REG_RX_RXNCO7(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCO7,unit,path)
#define M_DU_REG_RX_QCHINV(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_QCHINV,unit,path)
#define M_DU_REG_RX_RXPOWREP(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXPOWREP,unit,path)
#define M_DU_REG_RX_RXPOWSMPL(unit,path)	M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXPOWSMPL,unit,path)
#define M_DU_REG_RX_RXPOWLAT(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXPOWLAT,unit,path)
#define M_DU_REG_RX_RXPOW(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXPOW,unit,path)
#define M_DU_REG_RX_RXDCREP(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDCREP,unit,path)
#define M_DU_REG_RX_RXDCSMPL(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDCSMPL,unit,path)
#define M_DU_REG_RX_RXDCLAT(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDCLAT,unit,path)
#define M_DU_REG_RX_RXDC(unit,path)			M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXDC,unit,path)
#define M_DU_REG_RX_RXNCOPH0(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH0,unit,path)
#define M_DU_REG_RX_RXNCOPH1(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH1,unit,path)
#define M_DU_REG_RX_RXNCOPH2(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH2,unit,path)
#define M_DU_REG_RX_RXNCOPH3(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH3,unit,path)
#define M_DU_REG_RX_RXNCOPH4(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH4,unit,path)
#define M_DU_REG_RX_RXNCOPH5(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH5,unit,path)
#define M_DU_REG_RX_RXNCOPH6(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH6,unit,path)
#define M_DU_REG_RX_RXNCOPH7(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXNCOPH7,unit,path)
#define M_DU_REG_RX_RXTEST(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXTEST,unit,path)
#define M_DU_REG_RX_RXMONSEL(unit,path)		M_DU_REG_RX_GETADDR(D_DU_REG_RX_RXMONSEL,unit,path)
/* @} */
#endif
/* @} */
