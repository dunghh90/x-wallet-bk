/*!
 * @skip  $ld:$
 * @file  f_du_reg_ctx.h
 * @brief レジスタアドレス定義ヘッダ(CTX)
 * @date  2015/08/18 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CTX_H
#define F_DU_REG_CTX_H

/*!
 * @name CTXレジスタアドレス
 * @note CTXレジスタアドレス
 * @{
 */

#define D_DU_REG_CTX_TRX1					0x00000000		/* TRXのベースアドレス				*/
#define D_DU_REG_CTX_TRX2					0x02000000		/* TRXのベースアドレス				*/

#define D_DU_REG_CTX_FPGAVER                (0xb0000000)
#define D_DU_REG_CTX_REGCHK                 (0xb0000004)
#define D_DU_REG_CTX_BID                    (0xb0000008)
#define D_DU_REG_CTX_RST                    (0xb0000040)
#define D_DU_REG_CTX_RSTPHY1                (0xb0000044)
#define D_DU_REG_CTX_RSTPHY2                (0xb0000048)
#define D_DU_REG_CTX_RSTPHY3                (0xb000004c)
#define D_DU_REG_CTX_PHYPWR1                (0xb0000050)
#define D_DU_REG_CTX_PHYPWR2                (0xb0000054)
#define D_DU_REG_CTX_PHYPWR3                (0xb0000058)
#define D_DU_REG_CTX_STA1                   (0xb0000200)
#define D_DU_REG_CTX_STA1M                  (0xb0000204)
#define D_DU_REG_CTX_FCSTA1                 (0xb0000208)
#define D_DU_REG_CTX_STA2                   (0xb0000210)
#define D_DU_REG_CTX_STA2M                  (0xb0000214)
#define D_DU_REG_CTX_FCSTA2                 (0xb0000218)
#define D_DU_REG_CTX_TDDISM                 (0xb00002a0)
#define D_DU_REG_CTX_MBCEUE                 (0xb00002c0)
#define D_DU_REG_CTX_GPIFSET                (0xb0000700)
#define D_DU_REG_CTX_GPIFCK                 (0xb0000704)
#define D_DU_REG_CTX_GPIFEN                 (0xb0000708)
#define D_DU_REG_CTX_GPIFTXD                (0xb0000710)
#define D_DU_REG_CTX_GPIFRXD                (0xb0000714)
#define D_DU_REG_CTX_ICPHYCNT1              (0xb0000900)
#define D_DU_REG_CTX_ICPHYCNT2              (0xb0000904)
#define D_DU_REG_CTX_ICPHYCNT3              (0xb0000908)
#define D_DU_REG_CTX_ICPHYCNT4              (0xb000090c)
#define D_DU_REG_CTX_ICPHYCNT5              (0xb0000910)
#define D_DU_REG_CTX_ICPHYCNT6              (0xb0000914)
#define D_DU_REG_CTX_ICPHYCNT7              (0xb0000918)
#define D_DU_REG_CTX_ICPHYCNT8              (0xb000091c)
#define D_DU_REG_CTX_JSPHYCNT1              (0xb0000940)
#define D_DU_REG_CTX_JSPHYCNT2              (0xb0000944)
#define D_DU_REG_CTX_JSPHYCNT3              (0xb0000948)
#define D_DU_REG_CTX_JSPHYCNT4              (0xb000094c)
#define D_DU_REG_CTX_JSPHYCNT5              (0xb0000950)
#define D_DU_REG_CTX_JSPHYCNT6              (0xb0000954)
#define D_DU_REG_CTX_JSPHYCNT7              (0xb0000958)
#define D_DU_REG_CTX_JSPHYCNT8              (0xb000095c)
#define D_DU_REG_CTX_SVIFERR                (0xb0000980)
#define D_DU_REG_CTX_SVIFTXEN               (0xb0000988)
#define D_DU_REG_CTX_SVIFTXD4               (0xb0000990)
#define D_DU_REG_CTX_SVIFTXD3               (0xb0000994)
#define D_DU_REG_CTX_SVIFTXD2               (0xb0000998)
#define D_DU_REG_CTX_SVIFTXD1               (0xb000099c)
#define D_DU_REG_CTX_SVIFRXD4               (0xb00009a0)
#define D_DU_REG_CTX_SVIFRXD3               (0xb00009a4)
#define D_DU_REG_CTX_SVIFRXD2               (0xb00009a8)
#define D_DU_REG_CTX_SVIFRXD1               (0xb00009ac)
#define D_DU_REG_CTX_ICNOTDONE              (0xb0000b00)
#define D_DU_REG_CTX_ICNOTDONEM             (0xb0000b04)
#define D_DU_REG_CTX_ICTXERR                (0xb0000b10)
#define D_DU_REG_CTX_ICTXMSK                (0xb0000b14)
#define D_DU_REG_CTX_ICRXERR                (0xb0000b20)
#define D_DU_REG_CTX_ICRXMSK                (0xb0000b24)
#define D_DU_REG_CTX_SWWORK                 (0xb0000c00)
#define D_DU_REG_CTX_RDYCHK                 (0xb0000f00)
#define D_DU_REG_CTX_JSDBG                  (0xb0000f10)
#define D_DU_REG_CTX_JSDBGMON               (0xb0000f14)
#define D_DU_REG_CTX_ICDBG                  (0xb0000f18)
#define D_DU_REG_CTX_ICTEST                 (0xb0000f1c)
#define D_DU_REG_CTX_ICRSTSTA1              (0xb0000f30)
#define D_DU_REG_CTX_ICRSTSTA2              (0xb0000f34)
#define D_DU_REG_CTX_ICPRBSLOCK             (0xb0000f40)
#define D_DU_REG_CTX_ICDBGMON               (0xb0000f44)
#define D_DU_REG_CTX_MBRST                  (0xb0000f50)
#define D_DU_REG_CTX_LLSVIFTST              (0xb0000f60)
#define D_DU_REG_CTX_DIPSW                  (0xb0000f70)
#define D_DU_REG_CTX_CAP                    (0xb0000fd0)
#define D_DU_REG_CTX_CAPEXT                 (0xb0000fd4)
#define D_DU_REG_CTX_CAPINT                 (0xb0000fd8)
#define D_DU_REG_CTX_DBGVER                 (0xb0000fe0)
#define D_DU_REG_CTX_CTTEST                 (0xb0000ff0)


/* @} */
#endif
/* @} */
