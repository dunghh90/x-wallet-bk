/*!
 * @skip  $ld:$
 * @file  f_du_reg_tcb.h
 * @brief レジスタアドレス定義ヘッダ(TCB)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_TCB_H
#define F_DU_REG_TCB_H

/*!
 * @name TCBレジスタアドレス
 * @note TCBレジスタアドレス
 * @{
 */
#define	D_DU_REG_TCB_TCBSFRMSET				0xA2000000		/* TCBSFRMSET	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBSFRMREQ				0xA2000004		/* TCBSFRMREQ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBSFRMWIN				0xA2000008		/* TCBSFRMWIN	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBSFRMRPT				0xA2000020		/* TCBSFRMRPT	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCULFRMAJ				0xA2000040		/* TCULFRMAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTWNDLTAJ				0xA2000100		/* TCTWNDLTAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTWNDERAJ				0xA2000104		/* TCTWNDERAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTWNULTAJ				0xA2000108		/* TCTWNULTAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTWNUERAJ				0xA200010C		/* TCTWNUERAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTWNAJOFS				0xA2000110		/* TCTWNAJOFS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDSBLRDAJ				0xA2000200		/* TCDSBLRDAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDCPINSAJ				0xA2000204		/* TCDCPINSAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCUCPRMVAJ				0xA2000208		/* TCUCPRMVAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCUPKTGNAJ				0xA200020C		/* TCUPKTGNAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCURXDATAJ				0xA2000210		/* TCURXDATAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCUABFIRAJ				0xA2000214		/* TCUABFIRAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDABFCTAJ				0xA2000218		/* TCDABFCTAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCTDCNTGAJ				0xA2000280		/* TCTDCNTGAJ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLON				0xA2000300		/* TCDTBLON		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLCLR				0xA2000304		/* TCDTBLCLR	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLLAT				0xA2000310		/* TCDTBLLAT	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLTM0				0xA2000320		/* TCDTBLTM0	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLTM1				0xA2000324		/* TCDTBLTM1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLRDSEL			0xA2000328		/* TCDTBLRDSEL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCDTBLREP				0xA2000340		/* TCDTBLREP	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDEN					0xA2000400		/* TDDEN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDCNTOFST				0xA2000404		/* TDCNTOFST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDLYOFST				0xA2000408		/* TDDLYOFST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDLYCCB				0xA2000410		/* TDDLYCCB		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDLYABF				0xA2000420		/* TDDLYABF		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL00				0xA2000700		/* TDDMYDSL00	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL01				0xA2000704		/* TDDMYDSL01	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL02				0xA2000708		/* TDDMYDSL02	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL03				0xA200070C		/* TDDMYDSL03	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL04				0xA2000710		/* TDDMYDSL04	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL05				0xA2000714		/* TDDMYDSL05	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL06				0xA2000718		/* TDDMYDSL06	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL07				0xA200071C		/* TDDMYDSL07	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL08				0xA2000720		/* TDDMYDSL08	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL09				0xA2000724		/* TDDMYDSL09	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL10				0xA2000728		/* TDDMYDSL10	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL11				0xA200072C		/* TDDMYDSL11	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL12				0xA2000730		/* TDDMYDSL12	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL13				0xA2000734		/* TDDMYDSL13	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL14				0xA2000738		/* TDDMYDSL14	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL15				0xA200073C		/* TDDMYDSL15	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL16				0xA2000740		/* TDDMYDSL16	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL17				0xA2000744		/* TDDMYDSL17	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL18				0xA2000748		/* TDDMYDSL18	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL19				0xA200074C		/* TDDMYDSL19	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL20				0xA2000750		/* TDDMYDSL20	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL21				0xA2000754		/* TDDMYDSL21	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL22				0xA2000758		/* TDDMYDSL22	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL23				0xA200075C		/* TDDMYDSL23	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL24				0xA2000760		/* TDDMYDSL24	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL25				0xA2000764		/* TDDMYDSL25	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL26				0xA2000768		/* TDDMYDSL26	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL27				0xA200076C		/* TDDMYDSL27	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL28				0xA2000770		/* TDDMYDSL28	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL29				0xA2000774		/* TDDMYDSL29	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL30				0xA2000778		/* TDDMYDSL30	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL31				0xA200077C		/* TDDMYDSL31	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL32				0xA2000780		/* TDDMYDSL32	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL33				0xA2000784		/* TDDMYDSL33	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL34				0xA2000788		/* TDDMYDSL34	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL35				0xA200078C		/* TDDMYDSL35	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL36				0xA2000790		/* TDDMYDSL36	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL37				0xA2000794		/* TDDMYDSL37	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL38				0xA2000798		/* TDDMYDSL38	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL39				0xA200079C		/* TDDMYDSL39	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL40				0xA20007A0		/* TDDMYDSL40	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL41				0xA20007A4		/* TDDMYDSL41	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL42				0xA20007A8		/* TDDMYDSL42	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL43				0xA20007AC		/* TDDMYDSL43	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL44				0xA20007B0		/* TDDMYDSL44	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL45				0xA20007B4		/* TDDMYDSL45	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL46				0xA20007B8		/* TDDMYDSL46	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL47				0xA20007BC		/* TDDMYDSL47	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL48				0xA20007C0		/* TDDMYDSL48	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL49				0xA20007C4		/* TDDMYDSL49	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL50				0xA20007C8		/* TDDMYDSL50	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL51				0xA20007CC		/* TDDMYDSL51	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL52				0xA20007D0		/* TDDMYDSL52	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL53				0xA20007D4		/* TDDMYDSL53	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL54				0xA20007D8		/* TDDMYDSL54	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL55				0xA20007DC		/* TDDMYDSL55	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL56				0xA20007E0		/* TDDMYDSL56	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL57				0xA20007E4		/* TDDMYDSL57	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL58				0xA20007E8		/* TDDMYDSL58	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL59				0xA20007EC		/* TDDMYDSL59	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL60				0xA20007F0		/* TDDMYDSL60	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL61				0xA20007F4		/* TDDMYDSL61	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL62				0xA20007F8		/* TDDMYDSL62	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYDSL63				0xA20007FC		/* TDDMYDSL63	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TDDMYON				0xA2000800		/* TDDMYON		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_PKTDGON				0xA2000900		/* PKTDGON		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_PKTDGMD				0xA2000904		/* PKTDGMD		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_PGCPOW					0xA2000AF0		/* PGCPOW		(sub6 : o / mmw : o)*/
#define D_DU_REG_TCB_PGCPSUM01				0xA2000B00		/* PGCPSUM01	(sub6 : - / mmw : o)*/
#define D_DU_REG_TCB_PGCPCNTV				0xA2000C00		/* PGCPCNTV		(sub6 : - / mmw : o)*/
#define D_DU_REG_TCB_PGCPCNTH				0xA2000D00		/* PGCPCNTV		(sub6 : - / mmw : o)*/
#define	D_DU_REG_TCB_PGCHDCTON				0xA2000E00		/* PGCHDCTON	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_PGCSFCTREQ				0xA2000E10		/* PGCSFCTREQ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBTIMMON				0xA2000F00		/* TCBTIMMON	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBTEST				0xA2000FC0		/* TCBTEST		(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBTST0MON				0xA2000FD0		/* TCBTST0MON	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBTST1MON				0xA2000FD4		/* TCBTST1MON	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBTST2MON				0xA2000FD8		/* TCBTST2MON	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_TCBMONSEL				0xA2000FFC		/* TCBMONSEL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_TCB_LABEL					0xA2001000		/* LABEL		(sub6 : o / mmw : o)*/
/* @} */
#endif
/* @} */
