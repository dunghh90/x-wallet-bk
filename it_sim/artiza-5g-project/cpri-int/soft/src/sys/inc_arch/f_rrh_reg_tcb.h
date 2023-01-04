/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_tcb.h
 * @brief レジスタアドレス定義ヘッダ(TCB)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_TCB_H
#define F_RRH_REG_TCB_H

/*!
 * @name TCBレジスタアドレス
 * @note TCBレジスタアドレス
 * @{
 */
#define	D_RRH_REG_TCB_TCBSFRMSET				(D_RRH_REG_TCB + 0x00000000)	/* RF Frame timing correction				*/
#define	D_RRH_REG_TCB_TCBSFRMREQ				(D_RRH_REG_TCB + 0x00000004)	/* RF Frame timing correction request		*/
#define	D_RRH_REG_TCB_TCBSFRMWIN				(D_RRH_REG_TCB + 0x00000008)	/* RF Frame timing correction window		*/
#define	D_RRH_REG_TCB_TCULFRMAJ					(D_RRH_REG_TCB + 0x00000040)	/* UL-DL timing relation					*/
#define	D_RRH_REG_TCB_TCTWNDLTAJ				(D_RRH_REG_TCB + 0x00000100)	/* DL packet late Window Check Frame timing Adjust	*/
#define	D_RRH_REG_TCB_TCTWNDERAJ				(D_RRH_REG_TCB + 0x00000104)	/* DL packet early Window Check Frame timing Adjust	*/
#define	D_RRH_REG_TCB_TCTWNULTAJ				(D_RRH_REG_TCB + 0x00000108)	/* UL packet late Window Check Frame timing Adjust	*/
#define	D_RRH_REG_TCB_TCTWNUERAJ				(D_RRH_REG_TCB + 0x0000010C)	/* UL packet early Window Check Frame timing Adjust	*/
#define	D_RRH_REG_TCB_TCTWNAJOFS				(D_RRH_REG_TCB + 0x00000110)	/* Tr-Window Check Frame timing Adjust offset		*/
#define	D_RRH_REG_TCB_TCDSBLRDAJ				(D_RRH_REG_TCB + 0x00000200)	/* DL symbol-buf Read Frame timing Adjust	*/
#define	D_RRH_REG_TCB_TCDCPINSAJ				(D_RRH_REG_TCB + 0x00000204)	/* DL cp insert Frame timing Adjust			*/
#define	D_RRH_REG_TCB_TCUICESRAJ				(D_RRH_REG_TCB + 0x00000208)	/* UL IC ES-buf Read Fram timing Adjust		*/
#define	D_RRH_REG_TCB_TCUPKTGNAJ				(D_RRH_REG_TCB + 0x0000020C)	/* UL packet gen Frame timing Adjust		*/
#define	D_RRH_REG_TCB_TCURXDATAJ				(D_RRH_REG_TCB + 0x00000210)	/* UL Rx Data Frame timing Adjust			*/
#define	D_RRH_REG_TCB_TCTDCNTGAJ				(D_RRH_REG_TCB + 0x00000280)	/* DL TDDCNT gen Frame timing Adjust		*/

#define	D_RRH_REG_TCB_TDDMYON					(D_RRH_REG_TCB + 0x00000800)	/* TDD Control Signal Gen Dummy 			*/
#define	D_RRH_REG_TCB_TCBTIMMON					(D_RRH_REG_TCB + 0x00000FF0)	/* TCB TIMMON Select						*/
#define	D_RRH_REG_TCB_TCBTEST					(D_RRH_REG_TCB + 0x00000FF4)	/* TCB TEST									*/
#define	D_RRH_REG_TCB_TCBTSTMON					(D_RRH_REG_TCB + 0x00000FF8)	/* TCB Test Monitor (debug)					*/
#define	D_RRH_REG_TCB_TCBMONSEL					(D_RRH_REG_TCB + 0x00000FFC)	/* TCB Monitor Select						*/

/* Direction Table Generation	*/
#define	D_RRH_REG_TCB_TCDTBLON					(D_RRH_REG_TCB + 0x00000300)	/* Direction Info Detect ON/OFF				*/
#define	D_RRH_REG_TCB_TCDTBLCLR					(D_RRH_REG_TCB + 0x00000304)	/* Direction Info Table Clear Trigger		*/
#define	D_RRH_REG_TCB_TCDTBLLAT					(D_RRH_REG_TCB + 0x00000310)	/* Direction Info Table Latch Trigger		*/
#define	D_RRH_REG_TCB_TCDTBLTM0					(D_RRH_REG_TCB + 0x00000320)	/* Direction Info Table Latch Timing 0		*/
#define	D_RRH_REG_TCB_TCDTBLTM1					(D_RRH_REG_TCB + 0x00000324)	/* Direction Info Table Latch Timing 1		*/
#define	D_RRH_REG_TCB_TCDTBLRDSEL				(D_RRH_REG_TCB + 0x00000328)	/* Direction Info Table Read Slot Select	*/
#define	D_RRH_REG_TCB_TCDTBLREP					(D_RRH_REG_TCB + 0x00000340)	/* Direction Info Table 表示				*/

/* TDD Control Signal Generation	*/
#define	D_RRH_REG_TCB_TDDEN						(D_RRH_REG_TCB + 0x00000400)	/* TDD Enable oFF/oN							*/
#define	D_RRH_REG_TCB_TDCNTOFST					(D_RRH_REG_TCB + 0x00000404)	/* TDD Counter Offset							*/
#define	D_RRH_REG_TCB_TDDLYOFST					(D_RRH_REG_TCB + 0x00000408)	/* TDD Control Signal Output Delay Offset		*/
#define	D_RRH_REG_TCB_TDDMYDSL00				(D_RRH_REG_TCB + 0x00000700)	/* Dummy Direction Table (EvenSFRM min-slot  0)	*/
#define	D_RRH_REG_TCB_TDDMYDSL01				(D_RRH_REG_TCB + 0x00000704)	/* Dummy Direction Table (EvenSFRM min-slot  1)	*/
#define	D_RRH_REG_TCB_TDDMYDSL02				(D_RRH_REG_TCB + 0x00000708)	/* Dummy Direction Table (EvenSFRM min-slot  2)	*/
#define	D_RRH_REG_TCB_TDDMYDSL03				(D_RRH_REG_TCB + 0x0000070C)	/* Dummy Direction Table (EvenSFRM min-slot  3)	*/
#define	D_RRH_REG_TCB_TDDMYDSL04				(D_RRH_REG_TCB + 0x00000710)	/* Dummy Direction Table (EvenSFRM min-slot  4)	*/
#define	D_RRH_REG_TCB_TDDMYDSL05				(D_RRH_REG_TCB + 0x00000714)	/* Dummy Direction Table (EvenSFRM min-slot  5)	*/
#define	D_RRH_REG_TCB_TDDMYDSL06				(D_RRH_REG_TCB + 0x00000718)	/* Dummy Direction Table (EvenSFRM min-slot  6)	*/
#define	D_RRH_REG_TCB_TDDMYDSL07				(D_RRH_REG_TCB + 0x0000071C)	/* Dummy Direction Table (EvenSFRM min-slot  7)	*/
#define	D_RRH_REG_TCB_TDDMYDSL08				(D_RRH_REG_TCB + 0x00000720)	/* Dummy Direction Table (EvenSFRM min-slot  8)	*/
#define	D_RRH_REG_TCB_TDDMYDSL09				(D_RRH_REG_TCB + 0x00000724)	/* Dummy Direction Table (EvenSFRM min-slot  9)	*/
#define	D_RRH_REG_TCB_TDDMYDSL10				(D_RRH_REG_TCB + 0x00000728)	/* Dummy Direction Table (EvenSFRM min-slot 10)	*/
#define	D_RRH_REG_TCB_TDDMYDSL11				(D_RRH_REG_TCB + 0x0000072C)	/* Dummy Direction Table (EvenSFRM min-slot 11)	*/
#define	D_RRH_REG_TCB_TDDMYDSL12				(D_RRH_REG_TCB + 0x00000730)	/* Dummy Direction Table (EvenSFRM min-slot 12)	*/
#define	D_RRH_REG_TCB_TDDMYDSL13				(D_RRH_REG_TCB + 0x00000734)	/* Dummy Direction Table (EvenSFRM min-slot 13)	*/
#define	D_RRH_REG_TCB_TDDMYDSL14				(D_RRH_REG_TCB + 0x00000738)	/* Dummy Direction Table (EvenSFRM min-slot 14)	*/
#define	D_RRH_REG_TCB_TDDMYDSL15				(D_RRH_REG_TCB + 0x0000073C)	/* Dummy Direction Table (EvenSFRM min-slot 15)	*/
#define	D_RRH_REG_TCB_TDDMYDSL16				(D_RRH_REG_TCB + 0x00000740)	/* Dummy Direction Table (EvenSFRM min-slot 16)	*/
#define	D_RRH_REG_TCB_TDDMYDSL17				(D_RRH_REG_TCB + 0x00000744)	/* Dummy Direction Table (EvenSFRM min-slot 17)	*/
#define	D_RRH_REG_TCB_TDDMYDSL18				(D_RRH_REG_TCB + 0x00000748)	/* Dummy Direction Table (EvenSFRM min-slot 18)	*/
#define	D_RRH_REG_TCB_TDDMYDSL19				(D_RRH_REG_TCB + 0x0000074C)	/* Dummy Direction Table (EvenSFRM min-slot 19)	*/
#define	D_RRH_REG_TCB_TDDMYDSL20				(D_RRH_REG_TCB + 0x00000750)	/* Dummy Direction Table (EvenSFRM min-slot 20)	*/
#define	D_RRH_REG_TCB_TDDMYDSL21				(D_RRH_REG_TCB + 0x00000754)	/* Dummy Direction Table (EvenSFRM min-slot 21)	*/
#define	D_RRH_REG_TCB_TDDMYDSL22				(D_RRH_REG_TCB + 0x00000758)	/* Dummy Direction Table (EvenSFRM min-slot 22)	*/
#define	D_RRH_REG_TCB_TDDMYDSL23				(D_RRH_REG_TCB + 0x0000075C)	/* Dummy Direction Table (EvenSFRM min-slot 23)	*/
#define	D_RRH_REG_TCB_TDDMYDSL24				(D_RRH_REG_TCB + 0x00000760)	/* Dummy Direction Table (EvenSFRM min-slot 24)	*/
#define	D_RRH_REG_TCB_TDDMYDSL25				(D_RRH_REG_TCB + 0x00000764)	/* Dummy Direction Table (EvenSFRM min-slot 25)	*/
#define	D_RRH_REG_TCB_TDDMYDSL26				(D_RRH_REG_TCB + 0x00000768)	/* Dummy Direction Table (EvenSFRM min-slot 26)	*/
#define	D_RRH_REG_TCB_TDDMYDSL27				(D_RRH_REG_TCB + 0x0000076C)	/* Dummy Direction Table (EvenSFRM min-slot 27)	*/
#define	D_RRH_REG_TCB_TDDMYDSL28				(D_RRH_REG_TCB + 0x00000770)	/* Dummy Direction Table (EvenSFRM min-slot 28)	*/
#define	D_RRH_REG_TCB_TDDMYDSL29				(D_RRH_REG_TCB + 0x00000774)	/* Dummy Direction Table (EvenSFRM min-slot 29)	*/
#define	D_RRH_REG_TCB_TDDMYDSL30				(D_RRH_REG_TCB + 0x00000778)	/* Dummy Direction Table (EvenSFRM min-slot 30)	*/
#define	D_RRH_REG_TCB_TDDMYDSL31				(D_RRH_REG_TCB + 0x0000077C)	/* Dummy Direction Table (EvenSFRM min-slot 31)	*/
#define	D_RRH_REG_TCB_TDDMYDSL32				(D_RRH_REG_TCB + 0x00000780)	/* Dummy Direction Table (OddSFRM min-slot 0)	*/
#define	D_RRH_REG_TCB_TDDMYDSL33				(D_RRH_REG_TCB + 0x00000784)	/* Dummy Direction Table (OddSFRM min-slot 1)	*/
#define	D_RRH_REG_TCB_TDDMYDSL34				(D_RRH_REG_TCB + 0x00000788)	/* Dummy Direction Table (OddSFRM min-slot  2)	*/
#define	D_RRH_REG_TCB_TDDMYDSL35				(D_RRH_REG_TCB + 0x0000078C)	/* Dummy Direction Table (OddSFRM min-slot  3)	*/
#define	D_RRH_REG_TCB_TDDMYDSL36				(D_RRH_REG_TCB + 0x00000790)	/* Dummy Direction Table (OddSFRM min-slot  4)	*/
#define	D_RRH_REG_TCB_TDDMYDSL37				(D_RRH_REG_TCB + 0x00000794)	/* Dummy Direction Table (OddSFRM min-slot  5)	*/
#define	D_RRH_REG_TCB_TDDMYDSL38				(D_RRH_REG_TCB + 0x00000798)	/* Dummy Direction Table (OddSFRM min-slot  6)	*/
#define	D_RRH_REG_TCB_TDDMYDSL39				(D_RRH_REG_TCB + 0x0000079C)	/* Dummy Direction Table (OddSFRM min-slot  7)	*/
#define	D_RRH_REG_TCB_TDDMYDSL40				(D_RRH_REG_TCB + 0x000007A0)	/* Dummy Direction Table (OddSFRM min-slot  8)	*/
#define	D_RRH_REG_TCB_TDDMYDSL41				(D_RRH_REG_TCB + 0x000007A4)	/* Dummy Direction Table (OddSFRM min-slot  9)	*/
#define	D_RRH_REG_TCB_TDDMYDSL42				(D_RRH_REG_TCB + 0x000007A8)	/* Dummy Direction Table (OddSFRM min-slot 10)	*/
#define	D_RRH_REG_TCB_TDDMYDSL43				(D_RRH_REG_TCB + 0x000007AC)	/* Dummy Direction Table (OddSFRM min-slot 11)	*/
#define	D_RRH_REG_TCB_TDDMYDSL44				(D_RRH_REG_TCB + 0x000007B0)	/* Dummy Direction Table (OddSFRM min-slot 12)	*/
#define	D_RRH_REG_TCB_TDDMYDSL45				(D_RRH_REG_TCB + 0x000007B4)	/* Dummy Direction Table (OddSFRM min-slot 13)	*/
#define	D_RRH_REG_TCB_TDDMYDSL46				(D_RRH_REG_TCB + 0x000007B8)	/* Dummy Direction Table (OddSFRM min-slot 14)	*/
#define	D_RRH_REG_TCB_TDDMYDSL47				(D_RRH_REG_TCB + 0x000007BC)	/* Dummy Direction Table (OddSFRM min-slot 15)	*/
#define	D_RRH_REG_TCB_TDDMYDSL48				(D_RRH_REG_TCB + 0x000007C0)	/* Dummy Direction Table (OddSFRM min-slot 16)	*/
#define	D_RRH_REG_TCB_TDDMYDSL49				(D_RRH_REG_TCB + 0x000007C4)	/* Dummy Direction Table (OddSFRM min-slot 17)	*/
#define	D_RRH_REG_TCB_TDDMYDSL50				(D_RRH_REG_TCB + 0x000007C8)	/* Dummy Direction Table (OddSFRM min-slot 18)	*/
#define	D_RRH_REG_TCB_TDDMYDSL51				(D_RRH_REG_TCB + 0x000007CC)	/* Dummy Direction Table (OddSFRM min-slot 19)	*/
#define	D_RRH_REG_TCB_TDDMYDSL52				(D_RRH_REG_TCB + 0x000007D0)	/* Dummy Direction Table (OddSFRM min-slot 20)	*/
#define	D_RRH_REG_TCB_TDDMYDSL53				(D_RRH_REG_TCB + 0x000007D4)	/* Dummy Direction Table (OddSFRM min-slot 21)	*/
#define	D_RRH_REG_TCB_TDDMYDSL54				(D_RRH_REG_TCB + 0x000007D8)	/* Dummy Direction Table (OddSFRM min-slot 22)	*/
#define	D_RRH_REG_TCB_TDDMYDSL55				(D_RRH_REG_TCB + 0x000007DC)	/* Dummy Direction Table (OddSFRM min-slot 23)	*/
#define	D_RRH_REG_TCB_TDDMYDSL56				(D_RRH_REG_TCB + 0x000007E0)	/* Dummy Direction Table (OddSFRM min-slot 24)	*/
#define	D_RRH_REG_TCB_TDDMYDSL57				(D_RRH_REG_TCB + 0x000007E4)	/* Dummy Direction Table (OddSFRM min-slot 25)	*/
#define	D_RRH_REG_TCB_TDDMYDSL58				(D_RRH_REG_TCB + 0x000007E8)	/* Dummy Direction Table (OddSFRM min-slot 26)	*/
#define	D_RRH_REG_TCB_TDDMYDSL59				(D_RRH_REG_TCB + 0x000007EC)	/* Dummy Direction Table (OddSFRM min-slot 27)	*/
#define	D_RRH_REG_TCB_TDDMYDSL60				(D_RRH_REG_TCB + 0x000007F0)	/* Dummy Direction Table (OddSFRM min-slot 28)	*/
#define	D_RRH_REG_TCB_TDDMYDSL61				(D_RRH_REG_TCB + 0x000007F4)	/* Dummy Direction Table (OddSFRM min-slot 29)	*/
#define	D_RRH_REG_TCB_TDDMYDSL62				(D_RRH_REG_TCB + 0x000007F8)	/* Dummy Direction Table (OddSFRM min-slot 30)	*/
#define	D_RRH_REG_TCB_TDDMYDSL63				(D_RRH_REG_TCB + 0x000007FC)	/* Dummy Direction Table (OddSFRM min-slot 31)	*/
/* @} */

/*!
 * @name TCBレジスタbit & Shift
 * @note TCBレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
