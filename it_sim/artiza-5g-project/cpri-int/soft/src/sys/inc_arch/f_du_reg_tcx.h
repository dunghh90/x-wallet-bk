/*!
 * @skip  $ld:$
 * @file  f_du_reg_tcx.h
 * @brief レジスタアドレス定義ヘッダ(TCX)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_TCX_H
#define F_DU_REG_TCX_H

/*!
 * @name TCXレジスタアドレス
 * @note TCXレジスタアドレス, sub6とmmWでdesineが一部異なるので注意！
 * @{
 */
#define D_DU_REG_TCX_TDDEN					0xB1000000		/* TDDEN		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCNTOFST				0xB1000010		/* TDCNTOFST	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDMAGOFST				0xB1000020		/* TDMAGOFST	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTENA				0xB1000040		/* TDANTENA		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTENB				0xB1000044		/* TDANTENB		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTENC				0xB1000048		/* TDANTENC		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTEND				0xB100004C		/* TDANTEND		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTEN2A				0xB1000050		/* TDANTEN2A	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTEN2B				0xB1000054		/* TDANTEN2B	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANTEN2C				0xB1000058		/* TDANTEN2C	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDANTEN2D				0xB100005C		/* TDANTEN2D	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDHDOFFL				0xB1000100		/* TDHDOFFL		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDHDOFFT				0xB1000104		/* TDHDOFFT		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDSLFDIS				0xB1000110		/* TDSLFDIS		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDSLFDISF				0xB1000114		/* TDSLFDISF	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCMDSET				0xB1000200		/* TDCMDSET		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCMDSETSTA			0xB1000210		/* TDCMDSETSTA	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCMDSTA				0xB1000220		/* TDCMDSTA		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLEN					0xB1000230		/* TDCLEN		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT00				0xB1000300		/* TDINT00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT01				0xB1000304		/* TDINT01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT02				0xB1000308		/* TDINT02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT03				0xB100030C		/* TDINT03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT04				0xB1000310		/* TDINT04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT05				0xB1000314		/* TDINT05		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT06				0xB1000318		/* TDINT06		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT07				0xB100031C		/* TDINT07		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDINT08				0xB1000320		/* TDINT08		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDANBS					0xB1000340		/* TDANBS		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRFDL					0xB1000360		/* TDRFDL		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRFUL					0xB1000364		/* TDRFUL		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF100				0xB1000380		/* TDRF100		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF101				0xB1000384		/* TDRF101		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF102				0xB1000388		/* TDRF102		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF103				0xB100038C		/* TDRF103		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF200				0xB10003A0		/* TDRF200		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF201				0xB10003A4		/* TDRF201		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDRF202				0xB10003A8		/* TDRF202		(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDRF203				0xB10003AC		/* TDRF203		(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDEXA00				0xB1000400		/* TDEXA00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXA01				0xB1000404		/* TDEXA01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXA02				0xB1000408		/* TDEXA02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXA03				0xB100040C		/* TDEXA03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXA04				0xB1000410		/* TDEXA04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXA05				0xB1000414		/* TDEXA05		(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDEXA06				0xB1000418		/* TDEXA06		(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDEXA07				0xB100041C		/* TDEXA07		(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDEXB00				0xB1000440		/* TDEXB00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXB01				0xB1000444		/* TDEXB01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXB02				0xB1000448		/* TDEXB02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXB03				0xB100044C		/* TDEXB03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXB04				0xB1000450		/* TDEXB04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXC00				0xB1000480		/* TDEXC00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXC01				0xB1000484		/* TDEXC01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXC02				0xB1000488		/* TDEXC02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXC03				0xB100048C		/* TDEXC03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXC04				0xB1000490		/* TDEXC04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXD00				0xB10004C0		/* TDEXD00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXD01				0xB10004C4		/* TDEXD01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXD02				0xB10004C8		/* TDEXD02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXD03				0xB10004CC		/* TDEXD03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDEXD04				0xB10004D0		/* TDEXD04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO00				0xB1000500		/* TDCLO00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO01				0xB1000504		/* TDCLO01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO02				0xB1000508		/* TDCLO02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO03				0xB100050C		/* TDCLO03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO04				0xB1000510		/* TDCLO04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO05				0xB1000514		/* TDCLO05		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO06				0xB1000518		/* TDCLO06		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLO07				0xB100051C		/* TDCLO07		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW00				0xB1000540		/* TDCLW00		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW01				0xB1000544		/* TDCLW01		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW02				0xB1000548		/* TDCLW02		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW03				0xB100054C		/* TDCLW03		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW04				0xB1000550		/* TDCLW04		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW05				0xB1000554		/* TDCLW05		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW06				0xB1000558		/* TDCLW06		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDCLW07				0xB100055C		/* TDCLW07		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXON					0xB1000700		/* TDFXON		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINAEN				0xB1000710		/* TDFXINAEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINASW				0xB1000714		/* TDFXINASW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINBEN				0xB1000718		/* TDFXINBEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINBSW				0xB100071C		/* TDFXINBSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINCEN				0xB1000720		/* TDFXINCEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINCSW				0xB1000724		/* TDFXINCSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINDEN				0xB1000728		/* TDFXINDEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXINDSW				0xB100072C		/* TDFXINDSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXRF1EN				0xB1000730		/* TDFXRF1EN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXRF1SW				0xB1000734		/* TDFXRF1SW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXRF2EN				0xB1000738		/* TDFXRF2EN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXRF2SW				0xB100073C		/* TDFXRF2SW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXAEN				0xB1000740		/* TDFXEXAEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXASW				0xB1000744		/* TDFXEXASW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXBEN				0xB1000748		/* TDFXEXBEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXBSW				0xB100074C		/* TDFXEXBSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXCEN				0xB1000750		/* TDFXEXCEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXCSW				0xB1000754		/* TDFXEXCSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXDEN				0xB1000758		/* TDFXEXDEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXEXDSW				0xB100075C		/* TDFXEXDSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXCLOEN				0xB1000760		/* TDFXCLOEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXCLOSW				0xB1000764		/* TDFXCLOSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXCLWEN				0xB1000768		/* TDFXCLWEN	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDFXCLWSW				0xB100076C		/* TDFXCLWSW	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGON					0xB1000780		/* TDDGON		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGBW					0xB1000784		/* TDDGBW		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFST				0xB1000788		/* TDDGOFST		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGPTN				0xB100078C		/* TDDGPTN		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGDLY				0xB10007FC		/* TDDGDLY		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA0				0xB1000800		/* TDDGIMPA0	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA1				0xB1000804		/* TDDGIMPA1	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA2				0xB1000808		/* TDDGIMPA2	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA3				0xB100080C		/* TDDGIMPA3	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA4				0xB1000810		/* TDDGIMPA4	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA5				0xB1000814		/* TDDGIMPA5	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA6				0xB1000818		/* TDDGIMPA6	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPA7				0xB100081C		/* TDDGIMPA7	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGIMPB0				0xB1000820		/* TDDGIMPB0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGIMPB1				0xB1000824		/* TDDGIMPB1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGIMPC0				0xB1000840		/* TDDGIMPC0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGIMPC1				0xB1000844		/* TDDGIMPC1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGIMPD0				0xB1000860		/* TDDGIMPD0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGIMPD1				0xB1000864		/* TDDGIMPD1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSA0				0xB1000880		/* TDDGOFSA0	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA1				0xB1000884		/* TDDGOFSA1	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA2				0xB1000888		/* TDDGOFSA2	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA3				0xB100088C		/* TDDGOFSA3	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA4				0xB1000890		/* TDDGOFSA4	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA5				0xB1000894		/* TDDGOFSA5	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA6				0xB1000898		/* TDDGOFSA6	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSA7				0xB100089C		/* TDDGOFSA7	(sub6 : - / mmW : o)*/
#define D_DU_REG_TCX_TDDGOFSB0				0xB10008A0		/* TDDGOFSB0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSB1				0xB10008A4		/* TDDGOFSB1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSC0				0xB10008C0		/* TDDGOFSC0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSC1				0xB10008C4		/* TDDGOFSC1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSD0				0xB10008E0		/* TDDGOFSD0	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TDDGOFSD1				0xB10008E4		/* TDDGOFSD1	(sub6 : o / mmW : -)*/
#define D_DU_REG_TCX_TCULFRMAJ				0xB1001000		/* TCULFRMAJ	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TCRABASEAJ				0xB1001100		/* TCRABASEAJ	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TCXTIMMON				0xB1001F00		/* TCXTIMMON	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TCXTEST				0xB1001F04		/* TCXTEST		(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TCXTSTMON				0xB1001F08		/* TCXTSTMON	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_TCXMONSEL				0xB1001FFC		/* TCXMONSEL	(sub6 : o / mmW : o)*/
#define D_DU_REG_TCX_LABEL					0xB1002000		/* LABEL		(sub6 : o / mmW : o)*/

/*!
 * @name TCXレジスタアドレス算出マクロ
 * @note TCXレジスタアドレス算出マクロ, アドレスはこのマクロを使用する unit は0(TRX-FPGA#0)～1(TRX-FPGA#1)である
 * @{
 */
#define	D_DU_OFT_TCX_UNIT					0x00000000
#define	D_DU_OFT_TCX_UNITANT				0x00000004
#define M_DU_REG_TCX_GETADDR(ofs, unit)				(UINT)( ofs  + (unit * D_DU_OFT_TCX_UNIT) )
#define M_DU_REG_TCX_GETADDR_ANT(ofs, unit_ant)		(UINT)( ofs  + (unit_ant * D_DU_OFT_TCX_UNITANT) )

#define M_DU_REG_TCX_TDDEN(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDEN,unit)
#define M_DU_REG_TCX_TDCNTOFST(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCNTOFST,unit)
#define M_DU_REG_TCX_TDMAGOFST(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDMAGOFST,unit)
#define M_DU_REG_TCX_TDANTEN(unit_ant)		M_DU_REG_TCX_GETADDR_ANT(D_DU_REG_TCX_TDANTENA,unit_ant)
#define M_DU_REG_TCX_TDANTENA(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDANTENA,unit)
#define M_DU_REG_TCX_TDANTENB(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDANTENB,unit)
#define M_DU_REG_TCX_TDANTENC(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDANTENC,unit)
#define M_DU_REG_TCX_TDANTEND(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDANTEND,unit)
#define M_DU_REG_TCX_TDHDOFFL(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDHDOFFL,unit)
#define M_DU_REG_TCX_TDHDOFFT(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDHDOFFT,unit)
#define M_DU_REG_TCX_TDSLFDIS(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDSLFDIS,unit)
#define M_DU_REG_TCX_TDSLFDISF(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDSLFDISF,unit)
#define M_DU_REG_TCX_TDCMDSET(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCMDSET,unit)
#define M_DU_REG_TCX_TDCMDSETSTA(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCMDSETSTA,unit)
#define M_DU_REG_TCX_TDCMDSTA(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCMDSTA,unit)
#define M_DU_REG_TCX_TDCLEN(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLEN,unit)
#define M_DU_REG_TCX_TDINT00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT00,unit)
#define M_DU_REG_TCX_TDINT01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT01,unit)
#define M_DU_REG_TCX_TDINT02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT02,unit)
#define M_DU_REG_TCX_TDINT03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT03,unit)
#define M_DU_REG_TCX_TDINT04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT04,unit)
#define M_DU_REG_TCX_TDINT05(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT05,unit)
#define M_DU_REG_TCX_TDINT06(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT06,unit)
#define M_DU_REG_TCX_TDINT07(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT07,unit)
#define M_DU_REG_TCX_TDINT08(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDINT08,unit)
#define M_DU_REG_TCX_TDANBS(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDANBS,unit)
#define M_DU_REG_TCX_TDRFDL(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRFDL,unit)
#define M_DU_REG_TCX_TDRFUL(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRFUL,unit)
#define M_DU_REG_TCX_TDRF100(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF100,unit)
#define M_DU_REG_TCX_TDRF101(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF101,unit)
#define M_DU_REG_TCX_TDRF102(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF102,unit)
#define M_DU_REG_TCX_TDRF103(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF103,unit)
#define M_DU_REG_TCX_TDRF200(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF200,unit)
#define M_DU_REG_TCX_TDRF201(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF201,unit)
#define M_DU_REG_TCX_TDRF202(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF202,unit)
#define M_DU_REG_TCX_TDRF203(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDRF203,unit)
#define M_DU_REG_TCX_TDEXA00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA00,unit)
#define M_DU_REG_TCX_TDEXA01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA01,unit)
#define M_DU_REG_TCX_TDEXA02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA02,unit)
#define M_DU_REG_TCX_TDEXA03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA03,unit)
#define M_DU_REG_TCX_TDEXA04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA04,unit)
#define M_DU_REG_TCX_TDEXA05(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA05,unit)
#define M_DU_REG_TCX_TDEXA06(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA06,unit)
#define M_DU_REG_TCX_TDEXA07(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXA07,unit)
#define M_DU_REG_TCX_TDEXB00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXB00,unit)
#define M_DU_REG_TCX_TDEXB01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXB01,unit)
#define M_DU_REG_TCX_TDEXB02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXB02,unit)
#define M_DU_REG_TCX_TDEXB03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXB03,unit)
#define M_DU_REG_TCX_TDEXB04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXB04,unit)
#define M_DU_REG_TCX_TDEXC00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXC00,unit)
#define M_DU_REG_TCX_TDEXC01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXC01,unit)
#define M_DU_REG_TCX_TDEXC02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXC02,unit)
#define M_DU_REG_TCX_TDEXC03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXC03,unit)
#define M_DU_REG_TCX_TDEXC04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXC04,unit)
#define M_DU_REG_TCX_TDEXD00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXD00,unit)
#define M_DU_REG_TCX_TDEXD01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXD01,unit)
#define M_DU_REG_TCX_TDEXD02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXD02,unit)
#define M_DU_REG_TCX_TDEXD03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXD03,unit)
#define M_DU_REG_TCX_TDEXD04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDEXD04,unit)
#define M_DU_REG_TCX_TDCLO00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO00,unit)
#define M_DU_REG_TCX_TDCLO01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO01,unit)
#define M_DU_REG_TCX_TDCLO02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO02,unit)
#define M_DU_REG_TCX_TDCLO03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO03,unit)
#define M_DU_REG_TCX_TDCLO04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO04,unit)
#define M_DU_REG_TCX_TDCLO05(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO05,unit)
#define M_DU_REG_TCX_TDCLO06(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO06,unit)
#define M_DU_REG_TCX_TDCLO07(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLO07,unit)
#define M_DU_REG_TCX_TDCLW00(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW00,unit)
#define M_DU_REG_TCX_TDCLW01(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW01,unit)
#define M_DU_REG_TCX_TDCLW02(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW02,unit)
#define M_DU_REG_TCX_TDCLW03(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW03,unit)
#define M_DU_REG_TCX_TDCLW04(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW04,unit)
#define M_DU_REG_TCX_TDCLW05(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW05,unit)
#define M_DU_REG_TCX_TDCLW06(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW06,unit)
#define M_DU_REG_TCX_TDCLW07(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDCLW07,unit)
#define M_DU_REG_TCX_TDFXON(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXON,unit)
#define M_DU_REG_TCX_TDFXINAEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINAEN,unit)
#define M_DU_REG_TCX_TDFXINASW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINASW,unit)
#define M_DU_REG_TCX_TDFXINBEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINBEN,unit)
#define M_DU_REG_TCX_TDFXINBSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINBSW,unit)
#define M_DU_REG_TCX_TDFXINCEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINCEN,unit)
#define M_DU_REG_TCX_TDFXINCSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINCSW,unit)
#define M_DU_REG_TCX_TDFXINDEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINDEN,unit)
#define M_DU_REG_TCX_TDFXINDSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXINDSW,unit)
#define M_DU_REG_TCX_TDFXRF1EN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXRF1EN,unit)
#define M_DU_REG_TCX_TDFXRF1SW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXRF1SW,unit)
#define M_DU_REG_TCX_TDFXRF2EN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXRF2EN,unit)
#define M_DU_REG_TCX_TDFXRF2SW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXRF2SW,unit)
#define M_DU_REG_TCX_TDFXEXAEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXAEN,unit)
#define M_DU_REG_TCX_TDFXEXASW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXASW,unit)
#define M_DU_REG_TCX_TDFXEXBEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXBEN,unit)
#define M_DU_REG_TCX_TDFXEXBSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXBSW,unit)
#define M_DU_REG_TCX_TDFXEXCEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXCEN,unit)
#define M_DU_REG_TCX_TDFXEXCSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXCSW,unit)
#define M_DU_REG_TCX_TDFXEXDEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXDEN,unit)
#define M_DU_REG_TCX_TDFXEXDSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXEXDSW,unit)
#define M_DU_REG_TCX_TDFXCLOEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXCLOEN,unit)
#define M_DU_REG_TCX_TDFXCLOSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXCLOSW,unit)
#define M_DU_REG_TCX_TDFXCLWEN(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXCLWEN,unit)
#define M_DU_REG_TCX_TDFXCLWSW(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDFXCLWSW,unit)
#define M_DU_REG_TCX_TDDGON(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGON,unit)
#define M_DU_REG_TCX_TDDGBW(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGBW,unit)
#define M_DU_REG_TCX_TDDGOFST(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFST,unit)
#define M_DU_REG_TCX_TDDGPTN(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGPTN,unit)
#define M_DU_REG_TCX_TDDGDLY(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGDLY,unit)
#define M_DU_REG_TCX_TDDGIMPA0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA0,unit)
#define M_DU_REG_TCX_TDDGIMPA1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA1,unit)
#define M_DU_REG_TCX_TDDGIMPA2(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA2,unit)
#define M_DU_REG_TCX_TDDGIMPA3(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA3,unit)
#define M_DU_REG_TCX_TDDGIMPA4(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA4,unit)
#define M_DU_REG_TCX_TDDGIMPA5(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA5,unit)
#define M_DU_REG_TCX_TDDGIMPA6(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA6,unit)
#define M_DU_REG_TCX_TDDGIMPA7(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPA7,unit)
#define M_DU_REG_TCX_TDDGIMPB0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPB0,unit)
#define M_DU_REG_TCX_TDDGIMPB1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPB1,unit)
#define M_DU_REG_TCX_TDDGIMPC0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPC0,unit)
#define M_DU_REG_TCX_TDDGIMPC1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPC1,unit)
#define M_DU_REG_TCX_TDDGIMPD0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPD0,unit)
#define M_DU_REG_TCX_TDDGIMPD1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGIMPD1,unit)
#define M_DU_REG_TCX_TDDGOFSA0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA0,unit)
#define M_DU_REG_TCX_TDDGOFSA1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA1,unit)
#define M_DU_REG_TCX_TDDGOFSA2(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA2,unit)
#define M_DU_REG_TCX_TDDGOFSA3(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA3,unit)
#define M_DU_REG_TCX_TDDGOFSA4(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA4,unit)
#define M_DU_REG_TCX_TDDGOFSA5(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA5,unit)
#define M_DU_REG_TCX_TDDGOFSA6(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA6,unit)
#define M_DU_REG_TCX_TDDGOFSA7(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSA7,unit)
#define M_DU_REG_TCX_TDDGOFSB0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSB0,unit)
#define M_DU_REG_TCX_TDDGOFSB1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSB1,unit)
#define M_DU_REG_TCX_TDDGOFSC0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSC0,unit)
#define M_DU_REG_TCX_TDDGOFSC1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSC1,unit)
#define M_DU_REG_TCX_TDDGOFSD0(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSD0,unit)
#define M_DU_REG_TCX_TDDGOFSD1(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TDDGOFSD1,unit)
#define M_DU_REG_TCX_TCULFRMAJ(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCULFRMAJ,unit)
#define M_DU_REG_TCX_TCRABASEAJ(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCRABASEAJ,unit)
#define M_DU_REG_TCX_TCXTIMMON(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCXTIMMON,unit)
#define M_DU_REG_TCX_TCXTEST(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCXTEST,unit)
#define M_DU_REG_TCX_TCXTSTMON(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCXTSTMON,unit)
#define M_DU_REG_TCX_TCXMONSEL(unit)		M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_TCXMONSEL,unit)
#define M_DU_REG_TCX_LABEL(unit)			M_DU_REG_TCX_GETADDR(D_DU_REG_TCX_LABEL,unit)

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* TDDEN */
#define	D_DU_TD_TDDEN_MD				0x00000010
#define	D_DU_TD_TDDEN_ON				0x00000001
#define	D_DU_TD_TDDEN_OFF				0x00000000

/* TDSTAOFST */
#define	D_DU_TD_TDSTAOFST				0x00000035

/* TDANTEN A */
#define	D_DU_TD_TDANTEN_EN				0x00000001
#define	D_DU_TD_TDANTEN_TXON			0x00000002

/* TDHDOFF */
#define	D_DU_TD_TDHDOFFA_STOP			0x00000001

/* TDALM */
#define	D_DU_TD_TDALM_TDDANS			0x00000001
#define	D_DU_TD_TDALM_SHUTDOWN			0x00000002

/* @} */

#define	D_DU_VAL_TD_FIXMD_OFF			0x00000001

/* @} */
#endif	
/* @} */
