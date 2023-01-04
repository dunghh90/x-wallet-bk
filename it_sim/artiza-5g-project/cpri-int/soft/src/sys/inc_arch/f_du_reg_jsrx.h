/*!
 * @skip  $ld:$
 * @file  f_du_reg_jsrx.h
 * @brief レジスタアドレス定義ヘッダ(JSRX)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_JSRX_H
#define F_DU_REG_JSRX_H

/*!
 * @name JSRXレジスタアドレス
 * @note JSRXレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_JSRX_JSESRCTR				0xB1301000		/* JSESRCT		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSESRLOG				0xB1301004		/* JSESRLO		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXSRCTR				0xB1301020		/* JSRXSRCT		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXSYNCCTR			0xB1301030		/* JSRXSYNCCT	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXSYNCMON			0xB1301034		/* JSRXSYNCMO	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXEN				0xB1301080		/* JSRXE		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXRF				0xB1301084		/* JSRXR		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXRFTYP				0xB1301088		/* JSRXRFTY		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCTR				0xB1301090		/* JSRXCT		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXMINDLY			0xB1301098		/* JSRXMINDL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXRSMD				0xB13010A0		/* JSRXRSM		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP0L0				0xB1301100		/* JSRXCP0L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP1L0				0xB1301104		/* JSRXCP1L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP2L0				0xB1301108		/* JSRXCP2L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP3L0				0xB130110C		/* JSRXCP3L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP0L1				0xB1301110		/* JSRXCP0L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP1L1				0xB1301114		/* JSRXCP1L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP2L1				0xB1301118		/* JSRXCP2L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP3L1				0xB130111C		/* JSRXCP3L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP0L2				0xB1301120		/* JSRXCP0L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP1L2				0xB1301124		/* JSRXCP1L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP2L2				0xB1301128		/* JSRXCP2L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP3L2				0xB130112C		/* JSRXCP3L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP0L3				0xB1301130		/* JSRXCP0L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP1L3				0xB1301134		/* JSRXCP1L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP2L3				0xB1301138		/* JSRXCP2L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXCP3L3				0xB130113C		/* JSRXCP3L		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXXBAR				0xB1301200		/* JSRXXBA		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXST				0xB1301300		/* JSRXS		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPFST				0xB1301304		/* JSRXPFS		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPEST				0xB1301308		/* JSRXPES		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPCST				0xB130130C		/* JSRXPCS		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXEST				0xB1301310		/* JSRXES		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPECL0				0xB1301320		/* JSRXPECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPECL1				0xB1301324		/* JSRXPECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPECL2				0xB1301328		/* JSRXPECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPECL3				0xB130132C		/* JSRXPECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXFECL0				0xB1301340		/* JSRXFECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXFECL1				0xB1301344		/* JSRXFECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXFECL2				0xB1301348		/* JSRXFECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXFECL3				0xB130134C		/* JSRXFECL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXDTIML0			0xB1301380		/* JSRXDTIML	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXDTIML1			0xB1301384		/* JSRXDTIML	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXDTIML2			0xB1301388		/* JSRXDTIML	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXDTIML3			0xB130138C		/* JSRXDTIML	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXIRQ				0xB1301400		/* JSRXIR		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXIRQC				0xB1301404		/* JSRXIRQ		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXIRQM				0xB1301408		/* JSRXIRQ		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXLMFC				0xB1301500		/* JSRXLMF		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXLXSYNC			0xB1301504		/* JSRXLXSYN	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXLALIGN			0xB1301508		/* JSRXLALIG	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXSTMON				0xB1301F00		/* JSRXSTMO		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXSTLOG				0xB1301F04		/* JSRXSTLO		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXKLOGL0			0xB1301F10		/* JSRXKLOGL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXKLOGL1			0xB1301F14		/* JSRXKLOGL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXKLOGL2			0xB1301F18		/* JSRXKLOGL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXKLOGL3			0xB1301F1C		/* JSRXKLOGL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPLLSRREQ			0xB1301F80		/* JSRXPLLSRRE	(sub6 : - / mmw : o)*/
#define D_DU_REG_JSRX_JSRXPLLSYNC			0xB1301F84		/* JSRXPLLSYN	(sub6 : - / mmw : o)*/
#define D_DU_REG_JSRX_JSRXTEST				0xB1301FF0		/* JSRXTES		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSRX_JSRXMSEL				0xB1301FFC		/* JSRXMSE		(sub6 : o / mmw : o)*/
/* @} */

/*!
 * @name JSRXレジスタアドレス算出マクロ
 * @note JSRXレジスタアドレス算出マクロ, アドレスはこのマクロを使用する unit は0～3である
 * @{
 */
#define	D_DU_OFT_JSRX_UNIT					0x00000000
#define M_DU_REG_JSRX_GETADDR(ofs, unit)	(UINT)(ofs  + ((unit < 2) ? 0 : D_DU_OFT_JSRX_UNIT))
#define M_DU_REG_JSRX_JSESRCTR(uint)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSESRCTR,unit)
#define M_DU_REG_JSRX_JSESRLOG(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSESRLOG,unit)
#define M_DU_REG_JSRX_JSRXSRCTR(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXSRCTR,unit)
#define M_DU_REG_JSRX_JSRXSYNCCTR(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXSYNCCTR,unit)
#define M_DU_REG_JSRX_JSRXSYNCMON(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXSYNCMON,unit)
#define M_DU_REG_JSRX_JSRXEN(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXEN,unit)
#define M_DU_REG_JSRX_JSRXRF(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXRF,unit)
#define M_DU_REG_JSRX_JSRXRFTYP(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXRFTYP,unit)
#define M_DU_REG_JSRX_JSRXCTR(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCTR,unit)
#define M_DU_REG_JSRX_JSRXMINDLY(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXMINDLY,unit)
#define M_DU_REG_JSRX_JSRXRSMD(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXRSMD,unit)
#define M_DU_REG_JSRX_JSRXCP0L0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP0L0,unit)
#define M_DU_REG_JSRX_JSRXCP1L0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP1L0,unit)
#define M_DU_REG_JSRX_JSRXCP2L0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP2L0,unit)
#define M_DU_REG_JSRX_JSRXCP3L0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP3L0,unit)
#define M_DU_REG_JSRX_JSRXCP0L1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP0L1,unit)
#define M_DU_REG_JSRX_JSRXCP1L1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP1L1,unit)
#define M_DU_REG_JSRX_JSRXCP2L1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP2L1,unit)
#define M_DU_REG_JSRX_JSRXCP3L1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP3L1,unit)
#define M_DU_REG_JSRX_JSRXCP0L2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP0L2,unit)
#define M_DU_REG_JSRX_JSRXCP1L2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP1L2,unit)
#define M_DU_REG_JSRX_JSRXCP2L2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP2L2,unit)
#define M_DU_REG_JSRX_JSRXCP3L2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP3L2,unit)
#define M_DU_REG_JSRX_JSRXCP0L3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP0L3,unit)
#define M_DU_REG_JSRX_JSRXCP1L3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP1L3,unit)
#define M_DU_REG_JSRX_JSRXCP2L3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP2L3,unit)
#define M_DU_REG_JSRX_JSRXCP3L3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXCP3L3,unit)
#define M_DU_REG_JSRX_JSRXXBAR(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXXBAR,unit)
#define M_DU_REG_JSRX_JSRXST(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXST,unit)
#define M_DU_REG_JSRX_JSRXPFST(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPFST,unit)
#define M_DU_REG_JSRX_JSRXPEST(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPEST,unit)
#define M_DU_REG_JSRX_JSRXPCST(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPCST,unit)
#define M_DU_REG_JSRX_JSRXEST(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXEST,unit)
#define M_DU_REG_JSRX_JSRXPECL0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPECL0,unit)
#define M_DU_REG_JSRX_JSRXPECL1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPECL1,unit)
#define M_DU_REG_JSRX_JSRXPECL2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPECL2,unit)
#define M_DU_REG_JSRX_JSRXPECL3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPECL3,unit)
#define M_DU_REG_JSRX_JSRXFECL0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXFECL0,unit)
#define M_DU_REG_JSRX_JSRXFECL1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXFECL1,unit)
#define M_DU_REG_JSRX_JSRXFECL2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXFECL2,unit)
#define M_DU_REG_JSRX_JSRXFECL3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXFECL3,unit)
#define M_DU_REG_JSRX_JSRXDTIML0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXDTIML0,unit)
#define M_DU_REG_JSRX_JSRXDTIML1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXDTIML1,unit)
#define M_DU_REG_JSRX_JSRXDTIML2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXDTIML2,unit)
#define M_DU_REG_JSRX_JSRXDTIML3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXDTIML3,unit)
#define M_DU_REG_JSRX_JSRXIRQ(unit)			M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXIRQ,unit)
#define M_DU_REG_JSRX_JSRXIRQC(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXIRQC,unit)
#define M_DU_REG_JSRX_JSRXIRQM(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXIRQM,unit)
#define M_DU_REG_JSRX_JSRXLMFC(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXLMFC,unit)
#define M_DU_REG_JSRX_JSRXLXSYNC(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXLXSYNC,unit)
#define M_DU_REG_JSRX_JSRXLALIGN(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXLALIGN,unit)
#define M_DU_REG_JSRX_JSRXSTMON(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXSTMON,unit)
#define M_DU_REG_JSRX_JSRXSTLOG(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXSTLOG,unit)
#define M_DU_REG_JSRX_JSRXKLOGL0(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXKLOGL0,unit)
#define M_DU_REG_JSRX_JSRXKLOGL1(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXKLOGL1,unit)
#define M_DU_REG_JSRX_JSRXKLOGL2(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXKLOGL2,unit)
#define M_DU_REG_JSRX_JSRXKLOGL3(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXKLOGL3,unit)
#define M_DU_REG_JSRX_JSRXPLLSRREQ(unit)	M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPLLSRREQ,unit)
#define M_DU_REG_JSRX_JSRXPLLSYNC(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXPLLSYNC,unit)
#define M_DU_REG_JSRX_JSRXTEST(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXTEST,unit)
#define M_DU_REG_JSRX_JSRXMSEL(unit)		M_DU_REG_JSRX_GETADDR(D_DU_REG_JSRX_JSRXMSEL,unit)
/* @} */
#endif
/* @} */
