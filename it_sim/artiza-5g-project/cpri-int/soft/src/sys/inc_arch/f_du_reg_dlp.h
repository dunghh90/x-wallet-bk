/*!
 * @skip  $ld:$
 * @file  f_du_reg_dlp.h
 * @brief レジスタアドレス定義ヘッダ(DLP)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_DLP_H
#define F_DU_REG_DLP_H

/*!
 * @name DLPレジスタアドレス
 * @note DLPレジスタアドレス ハードソフトから抜粋
 * @{
 */
#define D_DUS_REG_DLP_DLPEN                 (0xa2010000)
#define D_DUS_REG_DLP_DLSCS                 (0xa2010014)
#define D_DUS_REG_DLP_DUBW                  (0xa2010018)
#define D_DUS_REG_DLP_BW050MPRM             (0xa2010030)
#define D_DUS_REG_DLP_BW060MPRM             (0xa2010034)
#define D_DUS_REG_DLP_BW080MPRM             (0xa2010038)
#define D_DUS_REG_DLP_BW100MPRM             (0xa201003c)
#define D_DUS_REG_DLP_SLTSTDFLG             (0xa2010040)
#define D_DUS_REG_DLP_A0PRMDLN              (0xa2010100)
#define D_DUS_REG_DLP_A0PRMDLS              (0xa2010104)
#define D_DUS_REG_DLP_A0PRMULN              (0xa2010108)
#define D_DUS_REG_DLP_A0PRMULR              (0xa201010c)
#define D_DUS_REG_DLP_A1PRMDLN              (0xa2010120)
#define D_DUS_REG_DLP_A1PRMDLS              (0xa2010124)
#define D_DUS_REG_DLP_A1PRMULN              (0xa2010128)
#define D_DUS_REG_DLP_A1PRMULR              (0xa201012c)
#define D_DUS_REG_DLP_PMONLTCRALL           (0xa2010300)
#define D_DUS_REG_DLP_PMONLTCRCOM1          (0xa2010304)
#define D_DUS_REG_DLP_PMONLTCRCOM2          (0xa2010308)
#define D_DUS_REG_DLP_PMONLTCRAL            (0xa2010310)
#define D_DUS_REG_DLP_PMONLTCRBL            (0xa2010318)
#define D_DUS_REG_DLP_PMONLTCRCL            (0xa2010320)
#define D_DUS_REG_DLP_PMONLTCRDL            (0xa2010328)
#define D_DUS_REG_DLP_PMONLTCRRBRCV         (0xa2010330)
#define D_DUS_REG_DLP_PMONLTALL             (0xa2010340)
#define D_DUS_REG_DLP_PMONLTCOM1            (0xa2010344)
#define D_DUS_REG_DLP_PMONLTCOM2            (0xa2010348)
#define D_DUS_REG_DLP_PMONLTAL              (0xa2010350)
#define D_DUS_REG_DLP_PMONLTBL              (0xa2010358)
#define D_DUS_REG_DLP_PMONLTCL              (0xa2010360)
#define D_DUS_REG_DLP_PMONLTDL              (0xa2010368)
#define D_DUS_REG_DLP_PMONLTRBRCV           (0xa2010370)
#define D_DUS_REG_DLP_PMRCVPKT              (0xa20103c0)
#define D_DUS_REG_DLP_PMILGCMH              (0xa20103c4)
#define D_DUS_REG_DLP_PMIDINV               (0xa20103c8)
#define D_DUS_REG_DLP_PMSRPERR              (0xa20103cc)
#define D_DUS_REG_DLP_PMILGAPHA0DN          (0xa2010400)
#define D_DUS_REG_DLP_PMILGTIMA0DN          (0xa2010404)
#define D_DUS_REG_DLP_PMTIMTEA0DN           (0xa2010408)
#define D_DUS_REG_DLP_PMTIMTLA0DN           (0xa201040c)
#define D_DUS_REG_DLP_PMONTIMA0DN           (0xa2010410)
#define D_DUS_REG_DLP_PMSEQDUPA0DN          (0xa2010414)
#define D_DUS_REG_DLP_PMSEQMISA0DN          (0xa2010418)
#define D_DUS_REG_DLP_PMRCVFCA0DN           (0xa201041c)
#define D_DUS_REG_DLP_PMRCVUPA0DN           (0xa2010420)
#define D_DUS_REG_DLP_PMILGAPHA0UN          (0xa2010440)
#define D_DUS_REG_DLP_PMILGTIMA0UN          (0xa2010444)
#define D_DUS_REG_DLP_PMTIMTEA0UN           (0xa2010448)
#define D_DUS_REG_DLP_PMTIMTLA0UN           (0xa201044c)
#define D_DUS_REG_DLP_PMONTIMA0UN           (0xa2010450)
#define D_DUS_REG_DLP_PMSEQDUPA0UN          (0xa2010454)
#define D_DUS_REG_DLP_PMSEQMISA0UN          (0xa2010458)
#define D_DUS_REG_DLP_PMRCVFCA0UN           (0xa201045c)
#define D_DUS_REG_DLP_PMILGAPHA0UR          (0xa2010480)
#define D_DUS_REG_DLP_PMILGTIMA0UR          (0xa2010484)
#define D_DUS_REG_DLP_PMTIMTEA0UR           (0xa2010488)
#define D_DUS_REG_DLP_PMTIMTLA0UR           (0xa201048c)
#define D_DUS_REG_DLP_PMONTIMA0UR           (0xa2010490)
#define D_DUS_REG_DLP_PMSEQDUPA0UR          (0xa2010494)
#define D_DUS_REG_DLP_PMSEQMISA0UR          (0xa2010498)
#define D_DUS_REG_DLP_PMRCVFCA0UR           (0xa201049c)
#define D_DUS_REG_DLP_PMILGAPHA1DN          (0xa2010500)
#define D_DUS_REG_DLP_PMILGTIMA1DN          (0xa2010504)
#define D_DUS_REG_DLP_PMTIMTEA1DN           (0xa2010508)
#define D_DUS_REG_DLP_PMTIMTLA1DN           (0xa201050c)
#define D_DUS_REG_DLP_PMONTIMA1DN           (0xa2010510)
#define D_DUS_REG_DLP_PMSEQDUPA1DN          (0xa2010514)
#define D_DUS_REG_DLP_PMSEQMISA1DN          (0xa2010518)
#define D_DUS_REG_DLP_PMRCVFCA1DN           (0xa201051c)
#define D_DUS_REG_DLP_PMRCVUPA1DN           (0xa2010520)
#define D_DUS_REG_DLP_PMILGAPHA1UN          (0xa2010540)
#define D_DUS_REG_DLP_PMILGTIMA1UN          (0xa2010544)
#define D_DUS_REG_DLP_PMTIMTEA1UN           (0xa2010548)
#define D_DUS_REG_DLP_PMTIMTLA1UN           (0xa201054c)
#define D_DUS_REG_DLP_PMONTIMA1UN           (0xa2010550)
#define D_DUS_REG_DLP_PMSEQDUPA1UN          (0xa2010554)
#define D_DUS_REG_DLP_PMSEQMISA1UN          (0xa2010558)
#define D_DUS_REG_DLP_PMRCVFCA1UN           (0xa201055c)
#define D_DUS_REG_DLP_PMILGAPHA1UR          (0xa2010580)
#define D_DUS_REG_DLP_PMILGTIMA1UR          (0xa2010584)
#define D_DUS_REG_DLP_PMTIMTEA1UR           (0xa2010588)
#define D_DUS_REG_DLP_PMTIMTLA1UR           (0xa201058c)
#define D_DUS_REG_DLP_PMONTIMA1UR           (0xa2010590)
#define D_DUS_REG_DLP_PMSEQDUPA1UR          (0xa2010594)
#define D_DUS_REG_DLP_PMSEQMISA1UR          (0xa2010598)
#define D_DUS_REG_DLP_PMRCVFCA1UR           (0xa201059c)
#define D_DUS_REG_DLP_EXPRCVRB              (0xa2010c00)
#define D_DUS_REG_DLP_PMRBRCVLOSA0          (0xa2010c20)
#define D_DUS_REG_DLP_PMRBRCVLOSB0          (0xa2010c24)
#define D_DUS_REG_DLP_PMRBRCVLOSC0          (0xa2010c28)
#define D_DUS_REG_DLP_PMRBRCVLOSD0          (0xa2010c2c)
#define D_DUS_REG_DLP_PMRBRCVLOSA1          (0xa2010c30)
#define D_DUS_REG_DLP_PMRBRCVLOSB1          (0xa2010c34)
#define D_DUS_REG_DLP_PMRBRCVLOSC1          (0xa2010c38)
#define D_DUS_REG_DLP_PMRBRCVLOSD1          (0xa2010c3c)
#define D_DUS_REG_DLP_CKCHGBUFOVF           (0xa2010c60)
#define D_DUS_REG_DLP_UPFIFOOVF             (0xa2010c64)
#define D_DUS_REG_DLP_FCFIFOOVF             (0xa2010c68)
#define D_DUS_REG_DLP_SRACSILG              (0xa2010c6c)
#define D_DUS_REG_DLP_DLNSCNMILG            (0xa2010c80)
#define D_DUS_REG_DLP_DLNSBNMILG            (0xa2010c84)
#define D_DUS_REG_DLP_DTGTGT                (0xa2010d00)
#define D_DUS_REG_DLP_DLPPLOGON             (0xa2010e00)
#define D_DUS_REG_DLP_DLPPLOGEND            (0xa2010e04)
#define D_DUS_REG_DLP_DLPPLOGCNT            (0xa2010e08)
#define D_DUS_REG_DLP_DLPTLOGON             (0xa2010e10)
#define D_DUS_REG_DLP_UPFIFORST             (0xa2010f80)
#define D_DUS_REG_DLP_DBGDLP1               (0xa2010f84)
#define D_DUS_REG_DLP_DBGDLP2               (0xa2010f88)
#define D_DUS_REG_DLP_DBGDLP3               (0xa2010f8c)
#define D_DUS_REG_DLP_LNCHKMSK              (0xa2010f90)
#define D_DUS_REG_DLP_SRAMPERRF             (0xa2010f94)
#define D_DUS_REG_DLP_DBGDLPMON1            (0xa2010fc0)
#define D_DUS_REG_DLP_DLPCAPTIM             (0xa2010fd0)
#define D_DUS_REG_DLP_DLPCAPCTL             (0xa2010fd4)
#define D_DUS_REG_DLP_DLPMONSEL             (0xa2010ffc)
#define D_DUS_REG_DLP_DLPLOGTEL             (0xa2011000)
#define D_DUS_REG_DLP_DLPLOGPKT             (0xa2012000)
#define D_DUS_REG_DLP_B0PRMDLN              (0xa2010180)
#define D_DUS_REG_DLP_B0PRMDLS              (0xa2010184)
#define D_DUS_REG_DLP_B0PRMULN              (0xa2010188)
#define D_DUS_REG_DLP_B0PRMULR              (0xa201018c)
#define D_DUS_REG_DLP_B1PRMDLN              (0xa20101a0)
#define D_DUS_REG_DLP_B1PRMDLS              (0xa20101a4)
#define D_DUS_REG_DLP_B1PRMULN              (0xa20101a8)
#define D_DUS_REG_DLP_B1PRMULR              (0xa20101ac)
#define D_DUS_REG_DLP_C0PRMDLN              (0xa2010200)
#define D_DUS_REG_DLP_C0PRMDLS              (0xa2010204)
#define D_DUS_REG_DLP_C0PRMULN              (0xa2010208)
#define D_DUS_REG_DLP_C0PRMULR              (0xa201020c)
#define D_DUS_REG_DLP_C1PRMDLN              (0xa2010220)
#define D_DUS_REG_DLP_C1PRMDLS              (0xa2010224)
#define D_DUS_REG_DLP_C1PRMULN              (0xa2010228)
#define D_DUS_REG_DLP_C1PRMULR              (0xa201022c)
#define D_DUS_REG_DLP_D0PRMDLN              (0xa2010280)
#define D_DUS_REG_DLP_D0PRMDLS              (0xa2010284)
#define D_DUS_REG_DLP_D0PRMULN              (0xa2010288)
#define D_DUS_REG_DLP_D0PRMULR              (0xa201028c)
#define D_DUS_REG_DLP_D1PRMDLN              (0xa20102a0)
#define D_DUS_REG_DLP_D1PRMDLS              (0xa20102a4)
#define D_DUS_REG_DLP_D1PRMULN              (0xa20102a8)
#define D_DUS_REG_DLP_D1PRMULR              (0xa20102ac)
#define D_DUS_REG_DLP_PMILGAPHB0DN          (0xa2010600)
#define D_DUS_REG_DLP_PMILGTIMB0DN          (0xa2010604)
#define D_DUS_REG_DLP_PMTIMTEB0DN           (0xa2010608)
#define D_DUS_REG_DLP_PMTIMTLB0DN           (0xa201060c)
#define D_DUS_REG_DLP_PMONTIMB0DN           (0xa2010610)
#define D_DUS_REG_DLP_PMSEQDUPB0DN          (0xa2010614)
#define D_DUS_REG_DLP_PMSEQMISB0DN          (0xa2010618)
#define D_DUS_REG_DLP_PMRCVFCB0DN           (0xa201061c)
#define D_DUS_REG_DLP_PMRCVUPB0DN           (0xa2010620)
#define D_DUS_REG_DLP_PMILGAPHB0UN          (0xa2010640)
#define D_DUS_REG_DLP_PMILGTIMB0UN          (0xa2010644)
#define D_DUS_REG_DLP_PMTIMTEB0UN           (0xa2010648)
#define D_DUS_REG_DLP_PMTIMTLB0UN           (0xa201064c)
#define D_DUS_REG_DLP_PMONTIMB0UN           (0xa2010650)
#define D_DUS_REG_DLP_PMSEQDUPB0UN          (0xa2010654)
#define D_DUS_REG_DLP_PMSEQMISB0UN          (0xa2010658)
#define D_DUS_REG_DLP_PMRCVFCB0UN           (0xa201065c)
#define D_DUS_REG_DLP_PMILGAPHB0UR          (0xa2010680)
#define D_DUS_REG_DLP_PMILGTIMB0UR          (0xa2010684)
#define D_DUS_REG_DLP_PMTIMTEB0UR           (0xa2010688)
#define D_DUS_REG_DLP_PMTIMTLB0UR           (0xa201068c)
#define D_DUS_REG_DLP_PMONTIMB0UR           (0xa2010690)
#define D_DUS_REG_DLP_PMSEQDUPB0UR          (0xa2010694)
#define D_DUS_REG_DLP_PMSEQMISB0UR          (0xa2010698)
#define D_DUS_REG_DLP_PMRCVFCB0UR           (0xa201069c)
#define D_DUS_REG_DLP_PMILGAPHB1DN          (0xa2010700)
#define D_DUS_REG_DLP_PMILGTIMB1DN          (0xa2010704)
#define D_DUS_REG_DLP_PMTIMTEB1DN           (0xa2010708)
#define D_DUS_REG_DLP_PMTIMTLB1DN           (0xa201070c)
#define D_DUS_REG_DLP_PMONTIMB1DN           (0xa2010710)
#define D_DUS_REG_DLP_PMSEQDUPB1DN          (0xa2010714)
#define D_DUS_REG_DLP_PMSEQMISB1DN          (0xa2010718)
#define D_DUS_REG_DLP_PMRCVFCB1DN           (0xa201071c)
#define D_DUS_REG_DLP_PMRCVUPB1DN           (0xa2010720)
#define D_DUS_REG_DLP_PMILGAPHB1UN          (0xa2010740)
#define D_DUS_REG_DLP_PMILGTIMB1UN          (0xa2010744)
#define D_DUS_REG_DLP_PMTIMTEB1UN           (0xa2010748)
#define D_DUS_REG_DLP_PMTIMTLB1UN           (0xa201074c)
#define D_DUS_REG_DLP_PMONTIMB1UN           (0xa2010750)
#define D_DUS_REG_DLP_PMSEQDUPB1UN          (0xa2010754)
#define D_DUS_REG_DLP_PMSEQMISB1UN          (0xa2010758)
#define D_DUS_REG_DLP_PMRCVFCB1UN           (0xa201075c)
#define D_DUS_REG_DLP_PMILGAPHB1UR          (0xa2010780)
#define D_DUS_REG_DLP_PMILGTIMB1UR          (0xa2010784)
#define D_DUS_REG_DLP_PMTIMTEB1UR           (0xa2010788)
#define D_DUS_REG_DLP_PMTIMTLB1UR           (0xa201078c)
#define D_DUS_REG_DLP_PMONTIMB1UR           (0xa2010790)
#define D_DUS_REG_DLP_PMSEQDUPB1UR          (0xa2010794)
#define D_DUS_REG_DLP_PMSEQMISB1UR          (0xa2010798)
#define D_DUS_REG_DLP_PMRCVFCB1UR           (0xa201079c)
#define D_DUS_REG_DLP_PMILGAPHC0DN          (0xa2010800)
#define D_DUS_REG_DLP_PMILGTIMC0DN          (0xa2010804)
#define D_DUS_REG_DLP_PMTIMTEC0DN           (0xa2010808)
#define D_DUS_REG_DLP_PMTIMTLC0DN           (0xa201080c)
#define D_DUS_REG_DLP_PMONTIMC0DN           (0xa2010810)
#define D_DUS_REG_DLP_PMSEQDUPC0DN          (0xa2010814)
#define D_DUS_REG_DLP_PMSEQMISC0DN          (0xa2010818)
#define D_DUS_REG_DLP_PMRCVFCC0DN           (0xa201081c)
#define D_DUS_REG_DLP_PMRCVUPC0DN           (0xa2010820)
#define D_DUS_REG_DLP_PMILGAPHC0UN          (0xa2010840)
#define D_DUS_REG_DLP_PMILGTIMC0UN          (0xa2010844)
#define D_DUS_REG_DLP_PMTIMTEC0UN           (0xa2010848)
#define D_DUS_REG_DLP_PMTIMTLC0UN           (0xa201084c)
#define D_DUS_REG_DLP_PMONTIMC0UN           (0xa2010850)
#define D_DUS_REG_DLP_PMSEQDUPC0UN          (0xa2010854)
#define D_DUS_REG_DLP_PMSEQMISC0UN          (0xa2010858)
#define D_DUS_REG_DLP_PMRCVFCC0UN           (0xa201085c)
#define D_DUS_REG_DLP_PMILGAPHC0UR          (0xa2010880)
#define D_DUS_REG_DLP_PMILGTIMC0UR          (0xa2010884)
#define D_DUS_REG_DLP_PMTIMTEC0UR           (0xa2010888)
#define D_DUS_REG_DLP_PMTIMTLC0UR           (0xa201088c)
#define D_DUS_REG_DLP_PMONTIMC0UR           (0xa2010890)
#define D_DUS_REG_DLP_PMSEQDUPC0UR          (0xa2010894)
#define D_DUS_REG_DLP_PMSEQMISC0UR          (0xa2010898)
#define D_DUS_REG_DLP_PMRCVFCC0UR           (0xa201089c)
#define D_DUS_REG_DLP_PMILGAPHC1DN          (0xa2010900)
#define D_DUS_REG_DLP_PMILGTIMC1DN          (0xa2010904)
#define D_DUS_REG_DLP_PMTIMTEC1DN           (0xa2010908)
#define D_DUS_REG_DLP_PMTIMTLC1DN           (0xa201090c)
#define D_DUS_REG_DLP_PMONTIMC1DN           (0xa2010910)
#define D_DUS_REG_DLP_PMSEQDUPC1DN          (0xa2010914)
#define D_DUS_REG_DLP_PMSEQMISC1DN          (0xa2010918)
#define D_DUS_REG_DLP_PMRCVFCC1DN           (0xa201091c)
#define D_DUS_REG_DLP_PMRCVUPC1DN           (0xa2010920)
#define D_DUS_REG_DLP_PMILGAPHC1UN          (0xa2010940)
#define D_DUS_REG_DLP_PMILGTIMC1UN          (0xa2010944)
#define D_DUS_REG_DLP_PMTIMTEC1UN           (0xa2010948)
#define D_DUS_REG_DLP_PMTIMTLC1UN           (0xa201094c)
#define D_DUS_REG_DLP_PMONTIMC1UN           (0xa2010950)
#define D_DUS_REG_DLP_PMSEQDUPC1UN          (0xa2010954)
#define D_DUS_REG_DLP_PMSEQMISC1UN          (0xa2010958)
#define D_DUS_REG_DLP_PMRCVFCC1UN           (0xa201095c)
#define D_DUS_REG_DLP_PMILGAPHC1UR          (0xa2010980)
#define D_DUS_REG_DLP_PMILGTIMC1UR          (0xa2010984)
#define D_DUS_REG_DLP_PMTIMTEC1UR           (0xa2010988)
#define D_DUS_REG_DLP_PMTIMTLC1UR           (0xa201098c)
#define D_DUS_REG_DLP_PMONTIMC1UR           (0xa2010990)
#define D_DUS_REG_DLP_PMSEQDUPC1UR          (0xa2010994)
#define D_DUS_REG_DLP_PMSEQMISC1UR          (0xa2010998)
#define D_DUS_REG_DLP_PMRCVFCC1UR           (0xa201099c)
#define D_DUS_REG_DLP_PMILGAPHD0DN          (0xa2010a00)
#define D_DUS_REG_DLP_PMILGTIMD0DN          (0xa2010a04)
#define D_DUS_REG_DLP_PMTIMTED0DN           (0xa2010a08)
#define D_DUS_REG_DLP_PMTIMTLD0DN           (0xa2010a0c)
#define D_DUS_REG_DLP_PMONTIMD0DN           (0xa2010a10)
#define D_DUS_REG_DLP_PMSEQDUPD0DN          (0xa2010a14)
#define D_DUS_REG_DLP_PMSEQMISD0DN          (0xa2010a18)
#define D_DUS_REG_DLP_PMRCVFCD0DN           (0xa2010a1c)
#define D_DUS_REG_DLP_PMRCVUPD0DN           (0xa2010a20)
#define D_DUS_REG_DLP_PMILGAPHD0UN          (0xa2010a40)
#define D_DUS_REG_DLP_PMILGTIMD0UN          (0xa2010a44)
#define D_DUS_REG_DLP_PMTIMTED0UN           (0xa2010a48)
#define D_DUS_REG_DLP_PMTIMTLD0UN           (0xa2010a4c)
#define D_DUS_REG_DLP_PMONTIMD0UN           (0xa2010a50)
#define D_DUS_REG_DLP_PMSEQDUPD0UN          (0xa2010a54)
#define D_DUS_REG_DLP_PMSEQMISD0UN          (0xa2010a58)
#define D_DUS_REG_DLP_PMRCVFCD0UN           (0xa2010a5c)
#define D_DUS_REG_DLP_PMILGAPHD0UR          (0xa2010a80)
#define D_DUS_REG_DLP_PMILGTIMD0UR          (0xa2010a84)
#define D_DUS_REG_DLP_PMTIMTED0UR           (0xa2010a88)
#define D_DUS_REG_DLP_PMTIMTLD0UR           (0xa2010a8c)
#define D_DUS_REG_DLP_PMONTIMD0UR           (0xa2010a90)
#define D_DUS_REG_DLP_PMSEQDUPD0UR          (0xa2010a94)
#define D_DUS_REG_DLP_PMSEQMISD0UR          (0xa2010a98)
#define D_DUS_REG_DLP_PMRCVFCD0UR           (0xa2010a9c)
#define D_DUS_REG_DLP_PMILGAPHD1DN          (0xa2010b00)
#define D_DUS_REG_DLP_PMILGTIMD1DN          (0xa2010b04)
#define D_DUS_REG_DLP_PMTIMTED1DN           (0xa2010b08)
#define D_DUS_REG_DLP_PMTIMTLD1DN           (0xa2010b0c)
#define D_DUS_REG_DLP_PMONTIMD1DN           (0xa2010b10)
#define D_DUS_REG_DLP_PMSEQDUPD1DN          (0xa2010b14)
#define D_DUS_REG_DLP_PMSEQMISD1DN          (0xa2010b18)
#define D_DUS_REG_DLP_PMRCVFCD1DN           (0xa2010b1c)
#define D_DUS_REG_DLP_PMRCVUPD1DN           (0xa2010b20)
#define D_DUS_REG_DLP_PMILGAPHD1UN          (0xa2010b40)
#define D_DUS_REG_DLP_PMILGTIMD1UN          (0xa2010b44)
#define D_DUS_REG_DLP_PMTIMTED1UN           (0xa2010b48)
#define D_DUS_REG_DLP_PMTIMTLD1UN           (0xa2010b4c)
#define D_DUS_REG_DLP_PMONTIMD1UN           (0xa2010b50)
#define D_DUS_REG_DLP_PMSEQDUPD1UN          (0xa2010b54)
#define D_DUS_REG_DLP_PMSEQMISD1UN          (0xa2010b58)
#define D_DUS_REG_DLP_PMRCVFCD1UN           (0xa2010b5c)
#define D_DUS_REG_DLP_PMILGAPHD1UR          (0xa2010b80)
#define D_DUS_REG_DLP_PMILGTIMD1UR          (0xa2010b84)
#define D_DUS_REG_DLP_PMTIMTED1UR           (0xa2010b88)
#define D_DUS_REG_DLP_PMTIMTLD1UR           (0xa2010b8c)
#define D_DUS_REG_DLP_PMONTIMD1UR           (0xa2010b90)
#define D_DUS_REG_DLP_PMSEQDUPD1UR          (0xa2010b94)
#define D_DUS_REG_DLP_PMSEQMISD1UR          (0xa2010b98)
#define D_DUS_REG_DLP_PMRCVFCD1UR           (0xa2010b9c)

#define D_DUM_REG_DLP_DLPEN                 (0xa2010000)
#define D_DUM_REG_DLP_DLSCS                 (0xa2010014)
#define D_DUM_REG_DLP_DUBW                  (0xa2010018)
#define D_DUM_REG_DLP_BW100MPRM             (0xa2010030)
#define D_DUM_REG_DLP_BW200MPRM             (0xa2010034)
#define D_DUM_REG_DLP_BW400MPRM             (0xa2010038)
#define D_DUM_REG_DLP_SLTSTDFLG             (0xa2010040)
#define D_DUM_REG_DLP_A0PRMDLN              (0xa2010100)
#define D_DUM_REG_DLP_A0PRMDLS              (0xa2010104)
#define D_DUM_REG_DLP_A0PRMULN              (0xa2010108)
#define D_DUM_REG_DLP_A0PRMULR              (0xa201010c)
#define D_DUM_REG_DLP_B0PRMDLN              (0xa2010180)
#define D_DUM_REG_DLP_B0PRMDLS              (0xa2010184)
#define D_DUM_REG_DLP_B0PRMULN              (0xa2010188)
#define D_DUM_REG_DLP_B0PRMULR              (0xa201018c)
#define D_DUM_REG_DLP_PMONLTCRALL           D_DUS_REG_DLP_PMONLTCRALL
#define D_DUM_REG_DLP_PMONLTCRCOM1          (0xa2010304)
#define D_DUM_REG_DLP_PMONLTCRCOM2          (0xa2010308)
#define D_DUM_REG_DLP_PMONLTCRAL            (0xa2010310)
#define D_DUM_REG_DLP_PMONLTCRAU            (0xa2010314)
#define D_DUM_REG_DLP_PMONLTCRBL            (0xa2010318)
#define D_DUM_REG_DLP_PMONLTCRBU            (0xa201031c)
#define D_DUM_REG_DLP_PMONLTCRRBRCV         (0xa2010330)
#define D_DUM_REG_DLP_PMONLTALL             (0xa2010340)
#define D_DUM_REG_DLP_PMONLTCOM1            (0xa2010344)
#define D_DUM_REG_DLP_PMONLTCOM2            (0xa2010348)
#define D_DUM_REG_DLP_PMONLTAL              (0xa2010350)
#define D_DUM_REG_DLP_PMONLTAU              (0xa2010354)
#define D_DUM_REG_DLP_PMONLTBL              (0xa2010358)
#define D_DUM_REG_DLP_PMONLTBU              (0xa201035c)
#define D_DUM_REG_DLP_PMONLTRBRCV           (0xa2010370)
#define D_DUM_REG_DLP_PMRCVPKT              D_DUS_REG_DLP_PMRCVPKT
#define D_DUM_REG_DLP_PMILGCMH              D_DUS_REG_DLP_PMILGCMH
#define D_DUM_REG_DLP_PMIDINV               D_DUS_REG_DLP_PMIDINV 
#define D_DUM_REG_DLP_PMSRPERR              D_DUS_REG_DLP_PMSRPERR
#define D_DUM_REG_DLP_PMILGAPHA0DN          (0xa2010400)
#define D_DUM_REG_DLP_PMILGTIMA0DN          (0xa2010404)
#define D_DUM_REG_DLP_PMTIMTEA0DN           (0xa2010408)
#define D_DUM_REG_DLP_PMTIMTLA0DN           (0xa201040c)
#define D_DUM_REG_DLP_PMONTIMA0DN           (0xa2010410)
#define D_DUM_REG_DLP_PMSEQDUPA0DN          (0xa2010414)
#define D_DUM_REG_DLP_PMSEQMISA0DN          (0xa2010418)
#define D_DUM_REG_DLP_PMRCVFCA0DN           (0xa201041c)
#define D_DUM_REG_DLP_PMRCVUPA0DN           (0xa2010420)
#define D_DUM_REG_DLP_PMILGAPHA0UN          (0xa2010440)
#define D_DUM_REG_DLP_PMILGTIMA0UN          (0xa2010444)
#define D_DUM_REG_DLP_PMTIMTEA0UN           (0xa2010448)
#define D_DUM_REG_DLP_PMTIMTLA0UN           (0xa201044c)
#define D_DUM_REG_DLP_PMONTIMA0UN           (0xa2010450)
#define D_DUM_REG_DLP_PMSEQDUPA0UN          (0xa2010454)
#define D_DUM_REG_DLP_PMSEQMISA0UN          (0xa2010458)
#define D_DUM_REG_DLP_PMRCVFCA0UN           (0xa201045c)
#define D_DUM_REG_DLP_PMILGAPHA0UR          (0xa2010480)
#define D_DUM_REG_DLP_PMILGTIMA0UR          (0xa2010484)
#define D_DUM_REG_DLP_PMTIMTEA0UR           (0xa2010488)
#define D_DUM_REG_DLP_PMTIMTLA0UR           (0xa201048c)
#define D_DUM_REG_DLP_PMONTIMA0UR           (0xa2010490)
#define D_DUM_REG_DLP_PMSEQDUPA0UR          (0xa2010494)
#define D_DUM_REG_DLP_PMSEQMISA0UR          (0xa2010498)
#define D_DUM_REG_DLP_PMRCVFCA0UR           (0xa201049c)
#define D_DUM_REG_DLP_PMILGAPHA0DS          (0xa20104c0)
#define D_DUM_REG_DLP_PMILGTIMA0DS          (0xa20104c4)
#define D_DUM_REG_DLP_PMTIMTEA0DS           (0xa20104c8)
#define D_DUM_REG_DLP_PMTIMTLA0DS           (0xa20104cc)
#define D_DUM_REG_DLP_PMONTIMA0DS           (0xa20104d0)
#define D_DUM_REG_DLP_PMSEQDUPA0DS          (0xa20104d4)
#define D_DUM_REG_DLP_PMSEQMISA0DS          (0xa20104d8)
#define D_DUM_REG_DLP_PMRCVFCA0DS           (0xa20104dc)
#define D_DUM_REG_DLP_PMRCVUPA0DS           (0xa20104e0)
#define D_DUM_REG_DLP_PMILGAPHB0DN          (0xa2010800)
#define D_DUM_REG_DLP_PMILGTIMB0DN          (0xa2010804)
#define D_DUM_REG_DLP_PMTIMTEB0DN           (0xa2010808)
#define D_DUM_REG_DLP_PMTIMTLB0DN           (0xa201080c)
#define D_DUM_REG_DLP_PMONTIMB0DN           (0xa2010810)
#define D_DUM_REG_DLP_PMSEQDUPB0DN          (0xa2010814)
#define D_DUM_REG_DLP_PMSEQMISB0DN          (0xa2010818)
#define D_DUM_REG_DLP_PMRCVFCB0DN           (0xa201081c)
#define D_DUM_REG_DLP_PMRCVUPB0DN           (0xa2010820)
#define D_DUM_REG_DLP_PMILGAPHB0UN          (0xa2010840)
#define D_DUM_REG_DLP_PMILGTIMB0UN          (0xa2010844)
#define D_DUM_REG_DLP_PMTIMTEB0UN           (0xa2010848)
#define D_DUM_REG_DLP_PMTIMTLB0UN           (0xa201084c)
#define D_DUM_REG_DLP_PMONTIMB0UN           (0xa2010850)
#define D_DUM_REG_DLP_PMSEQDUPB0UN          (0xa2010854)
#define D_DUM_REG_DLP_PMSEQMISB0UN          (0xa2010858)
#define D_DUM_REG_DLP_PMRCVFCB0UN           (0xa201085c)
#define D_DUM_REG_DLP_PMILGAPHB0UR          (0xa2010880)
#define D_DUM_REG_DLP_PMILGTIMB0UR          (0xa2010884)
#define D_DUM_REG_DLP_PMTIMTEB0UR           (0xa2010888)
#define D_DUM_REG_DLP_PMTIMTLB0UR           (0xa201088c)
#define D_DUM_REG_DLP_PMONTIMB0UR           (0xa2010890)
#define D_DUM_REG_DLP_PMSEQDUPB0UR          (0xa2010894)
#define D_DUM_REG_DLP_PMSEQMISB0UR          (0xa2010898)
#define D_DUM_REG_DLP_PMRCVFCB0UR           (0xa201089c)
#define D_DUM_REG_DLP_PMILGAPHB0DS          (0xa20108c0)
#define D_DUM_REG_DLP_PMILGTIMB0DS          (0xa20108c4)
#define D_DUM_REG_DLP_PMTIMTEB0DS           (0xa20108c8)
#define D_DUM_REG_DLP_PMTIMTLB0DS           (0xa20108cc)
#define D_DUM_REG_DLP_PMONTIMB0DS           (0xa20108d0)
#define D_DUM_REG_DLP_PMSEQDUPB0DS          (0xa20108d4)
#define D_DUM_REG_DLP_PMSEQMISB0DS          (0xa20108d8)
#define D_DUM_REG_DLP_PMRCVFCB0DS           (0xa20108dc)
#define D_DUM_REG_DLP_PMRCVUPB0DS           (0xa20108e0)
#define D_DUM_REG_DLP_EXPRCVRB              D_DUS_REG_DLP_EXPRCVRB
#define D_DUM_REG_DLP_PMRBRCVLOSA0          D_DUS_REG_DLP_PMRBRCVLOSA0
#define D_DUM_REG_DLP_PMRBRCVLOSA1          D_DUS_REG_DLP_PMRBRCVLOSB0
#define D_DUM_REG_DLP_PMRBRCVLOSA2          D_DUS_REG_DLP_PMRBRCVLOSC0
#define D_DUM_REG_DLP_PMRBRCVLOSA3          D_DUS_REG_DLP_PMRBRCVLOSD0
#define D_DUM_REG_DLP_PMRBRCVLOSB0          D_DUS_REG_DLP_PMRBRCVLOSA1
#define D_DUM_REG_DLP_PMRBRCVLOSB1          D_DUS_REG_DLP_PMRBRCVLOSB1
#define D_DUM_REG_DLP_PMRBRCVLOSB2          D_DUS_REG_DLP_PMRBRCVLOSC1
#define D_DUM_REG_DLP_PMRBRCVLOSB3          D_DUS_REG_DLP_PMRBRCVLOSD1
#define D_DUM_REG_DLP_CKCHGBUFOVF           (0xa2010c60)
#define D_DUM_REG_DLP_UPFIFOOVF             (0xa2010c64)
#define D_DUM_REG_DLP_FCFIFOOVF             (0xa2010c68)
#define D_DUM_REG_DLP_SRACSILG              (0xa2010c6c)
#define D_DUM_REG_DLP_DLNSCNMILG            (0xa2010c80)
#define D_DUM_REG_DLP_DLNSBNMILG            (0xa2010c84)
#define D_DUM_REG_DLP_DLSSCNMILG            (0xa2010c88)
#define D_DUM_REG_DLP_DLSSBNMILG            (0xa2010c8c)
#define D_DUM_REG_DLP_DTGTGT                (0xa2010d00)
#define D_DUM_REG_DLP_DLPPLOGON             (0xa2010e00)
#define D_DUM_REG_DLP_DLPPLOGEND            (0xa2010e04)
#define D_DUM_REG_DLP_DLPPLOGCNT            (0xa2010e08)
#define D_DUM_REG_DLP_DLPTLOGON             (0xa2010e10)
#define D_DUM_REG_DLP_UPFIFORST             (0xa2010f80)
#define D_DUM_REG_DLP_DBGDLP1               (0xa2010f84)
#define D_DUM_REG_DLP_DBGDLP2               (0xa2010f88)
#define D_DUM_REG_DLP_DBGDLP3               (0xa2010f8c)
#define D_DUM_REG_DLP_LNCHKMSK              (0xa2010f90)
#define D_DUM_REG_DLP_SRAMPERRF             (0xa2010f94)
#define D_DUM_REG_DLP_DBGBMENB              (0xa2010fa0)
#define D_DUM_REG_DLP_DBGBMCNT              (0xa2010fa4)
#define D_DUM_REG_DLP_DBGDLPMON1            (0xa2010fc0)
#define D_DUM_REG_DLP_DLPCAPTIM             (0xa2010fd0)
#define D_DUM_REG_DLP_DLPCAPCTL             (0xa2010fd4)
#define D_DUM_REG_DLP_DLPMONSEL             (0xa2010ffc)
#define D_DUM_REG_DLP_DLPLOGTEL             (0xa2011000)
#define D_DUM_REG_DLP_DLPLOGPKT             (0xa2012000)
#define D_DUM_REG_DLP_A1PRMDLN              (0xa2010120)
#define D_DUM_REG_DLP_A1PRMDLS              (0xa2010124)
#define D_DUM_REG_DLP_A1PRMULN              (0xa2010128)
#define D_DUM_REG_DLP_A1PRMULR              (0xa201012c)
#define D_DUM_REG_DLP_B1PRMDLN              (0xa20101a0)
#define D_DUM_REG_DLP_B1PRMDLS              (0xa20101a4)
#define D_DUM_REG_DLP_B1PRMULN              (0xa20101a8)
#define D_DUM_REG_DLP_B1PRMULR              (0xa20101ac)
#define D_DUM_REG_DLP_A2PRMDLN              (0xa2010140)
#define D_DUM_REG_DLP_A2PRMDLS              (0xa2010144)
#define D_DUM_REG_DLP_A2PRMULN              (0xa2010148)
#define D_DUM_REG_DLP_A2PRMULR              (0xa201014c)
#define D_DUM_REG_DLP_B2PRMDLN              (0xa20101c0)
#define D_DUM_REG_DLP_B2PRMDLS              (0xa20101c4)
#define D_DUM_REG_DLP_B2PRMULN              (0xa20101c8)
#define D_DUM_REG_DLP_B2PRMULR              (0xa20101cc)
#define D_DUM_REG_DLP_A3PRMDLN              (0xa2010160)
#define D_DUM_REG_DLP_A3PRMDLS              (0xa2010164)
#define D_DUM_REG_DLP_A3PRMULN              (0xa2010168)
#define D_DUM_REG_DLP_A3PRMULR              (0xa201016c)
#define D_DUM_REG_DLP_B3PRMDLN              (0xa20101e0)
#define D_DUM_REG_DLP_B3PRMDLS              (0xa20101e4)
#define D_DUM_REG_DLP_B3PRMULN              (0xa20101e8)
#define D_DUM_REG_DLP_B3PRMULR              (0xa20101ec)
#define D_DUM_REG_DLP_PMILGAPHA1DN          (0xa2010500)
#define D_DUM_REG_DLP_PMILGTIMA1DN          (0xa2010504)
#define D_DUM_REG_DLP_PMTIMTEA1DN           (0xa2010508)
#define D_DUM_REG_DLP_PMTIMTLA1DN           (0xa201050c)
#define D_DUM_REG_DLP_PMONTIMA1DN           (0xa2010510)
#define D_DUM_REG_DLP_PMSEQDUPA1DN          (0xa2010514)
#define D_DUM_REG_DLP_PMSEQMISA1DN          (0xa2010518)
#define D_DUM_REG_DLP_PMRCVFCA1DN           (0xa201051c)
#define D_DUM_REG_DLP_PMRCVUPA1DN           (0xa2010520)
#define D_DUM_REG_DLP_PMILGAPHA1UN          (0xa2010540)
#define D_DUM_REG_DLP_PMILGTIMA1UN          (0xa2010544)
#define D_DUM_REG_DLP_PMTIMTEA1UN           (0xa2010548)
#define D_DUM_REG_DLP_PMTIMTLA1UN           (0xa201054c)
#define D_DUM_REG_DLP_PMONTIMA1UN           (0xa2010550)
#define D_DUM_REG_DLP_PMSEQDUPA1UN          (0xa2010554)
#define D_DUM_REG_DLP_PMSEQMISA1UN          (0xa2010558)
#define D_DUM_REG_DLP_PMRCVFCA1UN           (0xa201055c)
#define D_DUM_REG_DLP_PMILGAPHA1UR          (0xa2010580)
#define D_DUM_REG_DLP_PMILGTIMA1UR          (0xa2010584)
#define D_DUM_REG_DLP_PMTIMTEA1UR           (0xa2010588)
#define D_DUM_REG_DLP_PMTIMTLA1UR           (0xa201058c)
#define D_DUM_REG_DLP_PMONTIMA1UR           (0xa2010590)
#define D_DUM_REG_DLP_PMSEQDUPA1UR          (0xa2010594)
#define D_DUM_REG_DLP_PMSEQMISA1UR          (0xa2010598)
#define D_DUM_REG_DLP_PMRCVFCA1UR           (0xa201059c)
#define D_DUM_REG_DLP_PMILGAPHA1DS          (0xa20105c0)
#define D_DUM_REG_DLP_PMILGTIMA1DS          (0xa20105c4)
#define D_DUM_REG_DLP_PMTIMTEA1DS           (0xa20105c8)
#define D_DUM_REG_DLP_PMTIMTLA1DS           (0xa20105cc)
#define D_DUM_REG_DLP_PMONTIMA1DS           (0xa20105d0)
#define D_DUM_REG_DLP_PMSEQDUPA1DS          (0xa20105d4)
#define D_DUM_REG_DLP_PMSEQMISA1DS          (0xa20105d8)
#define D_DUM_REG_DLP_PMRCVFCA1DS           (0xa20105dc)
#define D_DUM_REG_DLP_PMRCVUPA1DS           (0xa20105e0)
#define D_DUM_REG_DLP_PMILGAPHB1DN          (0xa2010900)
#define D_DUM_REG_DLP_PMILGTIMB1DN          (0xa2010904)
#define D_DUM_REG_DLP_PMTIMTEB1DN           (0xa2010908)
#define D_DUM_REG_DLP_PMTIMTLB1DN           (0xa201090c)
#define D_DUM_REG_DLP_PMONTIMB1DN           (0xa2010910)
#define D_DUM_REG_DLP_PMSEQDUPB1DN          (0xa2010914)
#define D_DUM_REG_DLP_PMSEQMISB1DN          (0xa2010918)
#define D_DUM_REG_DLP_PMRCVFCB1DN           (0xa201091c)
#define D_DUM_REG_DLP_PMRCVUPB1DN           (0xa2010920)
#define D_DUM_REG_DLP_PMILGAPHB1UN          (0xa2010940)
#define D_DUM_REG_DLP_PMILGTIMB1UN          (0xa2010944)
#define D_DUM_REG_DLP_PMTIMTEB1UN           (0xa2010948)
#define D_DUM_REG_DLP_PMTIMTLB1UN           (0xa201094c)
#define D_DUM_REG_DLP_PMONTIMB1UN           (0xa2010950)
#define D_DUM_REG_DLP_PMSEQDUPB1UN          (0xa2010954)
#define D_DUM_REG_DLP_PMSEQMISB1UN          (0xa2010958)
#define D_DUM_REG_DLP_PMRCVFCB1UN           (0xa201095c)
#define D_DUM_REG_DLP_PMILGAPHB1UR          (0xa2010980)
#define D_DUM_REG_DLP_PMILGTIMB1UR          (0xa2010984)
#define D_DUM_REG_DLP_PMTIMTEB1UR           (0xa2010988)
#define D_DUM_REG_DLP_PMTIMTLB1UR           (0xa201098c)
#define D_DUM_REG_DLP_PMONTIMB1UR           (0xa2010990)
#define D_DUM_REG_DLP_PMSEQDUPB1UR          (0xa2010994)
#define D_DUM_REG_DLP_PMSEQMISB1UR          (0xa2010998)
#define D_DUM_REG_DLP_PMRCVFCB1UR           (0xa201099c)
#define D_DUM_REG_DLP_PMILGAPHB1DS          (0xa20109c0)
#define D_DUM_REG_DLP_PMILGTIMB1DS          (0xa20109c4)
#define D_DUM_REG_DLP_PMTIMTEB1DS           (0xa20109c8)
#define D_DUM_REG_DLP_PMTIMTLB1DS           (0xa20109cc)
#define D_DUM_REG_DLP_PMONTIMB1DS           (0xa20109d0)
#define D_DUM_REG_DLP_PMSEQDUPB1DS          (0xa20109d4)
#define D_DUM_REG_DLP_PMSEQMISB1DS          (0xa20109d8)
#define D_DUM_REG_DLP_PMRCVFCB1DS           (0xa20109dc)
#define D_DUM_REG_DLP_PMRCVUPB1DS           (0xa20109e0)
#define D_DUM_REG_DLP_PMILGAPHA2DN          (0xa2010600)
#define D_DUM_REG_DLP_PMILGTIMA2DN          (0xa2010604)
#define D_DUM_REG_DLP_PMTIMTEA2DN           (0xa2010608)
#define D_DUM_REG_DLP_PMTIMTLA2DN           (0xa201060c)
#define D_DUM_REG_DLP_PMONTIMA2DN           (0xa2010610)
#define D_DUM_REG_DLP_PMSEQDUPA2DN          (0xa2010614)
#define D_DUM_REG_DLP_PMSEQMISA2DN          (0xa2010618)
#define D_DUM_REG_DLP_PMRCVFCA2DN           (0xa201061c)
#define D_DUM_REG_DLP_PMRCVUPA2DN           (0xa2010620)
#define D_DUM_REG_DLP_PMILGAPHA2UN          (0xa2010640)
#define D_DUM_REG_DLP_PMILGTIMA2UN          (0xa2010644)
#define D_DUM_REG_DLP_PMTIMTEA2UN           (0xa2010648)
#define D_DUM_REG_DLP_PMTIMTLA2UN           (0xa201064c)
#define D_DUM_REG_DLP_PMONTIMA2UN           (0xa2010650)
#define D_DUM_REG_DLP_PMSEQDUPA2UN          (0xa2010654)
#define D_DUM_REG_DLP_PMSEQMISA2UN          (0xa2010658)
#define D_DUM_REG_DLP_PMRCVFCA2UN           (0xa201065c)
#define D_DUM_REG_DLP_PMILGAPHA2UR          (0xa2010680)
#define D_DUM_REG_DLP_PMILGTIMA2UR          (0xa2010684)
#define D_DUM_REG_DLP_PMTIMTEA2UR           (0xa2010688)
#define D_DUM_REG_DLP_PMTIMTLA2UR           (0xa201068c)
#define D_DUM_REG_DLP_PMONTIMA2UR           (0xa2010690)
#define D_DUM_REG_DLP_PMSEQDUPA2UR          (0xa2010694)
#define D_DUM_REG_DLP_PMSEQMISA2UR          (0xa2010698)
#define D_DUM_REG_DLP_PMRCVFCA2UR           (0xa201069c)
#define D_DUM_REG_DLP_PMILGAPHA2DS          (0xa20106c0)
#define D_DUM_REG_DLP_PMILGTIMA2DS          (0xa20106c4)
#define D_DUM_REG_DLP_PMTIMTEA2DS           (0xa20106c8)
#define D_DUM_REG_DLP_PMTIMTLA2DS           (0xa20106cc)
#define D_DUM_REG_DLP_PMONTIMA2DS           (0xa20106d0)
#define D_DUM_REG_DLP_PMSEQDUPA2DS          (0xa20106d4)
#define D_DUM_REG_DLP_PMSEQMISA2DS          (0xa20106d8)
#define D_DUM_REG_DLP_PMRCVFCA2DS           (0xa20106dc)
#define D_DUM_REG_DLP_PMRCVUPA2DS           (0xa20106e0)
#define D_DUM_REG_DLP_PMILGAPHB2DN          (0xa2010a00)
#define D_DUM_REG_DLP_PMILGTIMB2DN          (0xa2010a04)
#define D_DUM_REG_DLP_PMTIMTEB2DN           (0xa2010a08)
#define D_DUM_REG_DLP_PMTIMTLB2DN           (0xa2010a0c)
#define D_DUM_REG_DLP_PMONTIMB2DN           (0xa2010a10)
#define D_DUM_REG_DLP_PMSEQDUPB2DN          (0xa2010a14)
#define D_DUM_REG_DLP_PMSEQMISB2DN          (0xa2010a18)
#define D_DUM_REG_DLP_PMRCVFCB2DN           (0xa2010a1c)
#define D_DUM_REG_DLP_PMRCVUPB2DN           (0xa2010a20)
#define D_DUM_REG_DLP_PMILGAPHB2UN          (0xa2010a40)
#define D_DUM_REG_DLP_PMILGTIMB2UN          (0xa2010a44)
#define D_DUM_REG_DLP_PMTIMTEB2UN           (0xa2010a48)
#define D_DUM_REG_DLP_PMTIMTLB2UN           (0xa2010a4c)
#define D_DUM_REG_DLP_PMONTIMB2UN           (0xa2010a50)
#define D_DUM_REG_DLP_PMSEQDUPB2UN          (0xa2010a54)
#define D_DUM_REG_DLP_PMSEQMISB2UN          (0xa2010a58)
#define D_DUM_REG_DLP_PMRCVFCB2UN           (0xa2010a5c)
#define D_DUM_REG_DLP_PMILGAPHB2UR          (0xa2010a80)
#define D_DUM_REG_DLP_PMILGTIMB2UR          (0xa2010a84)
#define D_DUM_REG_DLP_PMTIMTEB2UR           (0xa2010a88)
#define D_DUM_REG_DLP_PMTIMTLB2UR           (0xa2010a8c)
#define D_DUM_REG_DLP_PMONTIMB2UR           (0xa2010a90)
#define D_DUM_REG_DLP_PMSEQDUPB2UR          (0xa2010a94)
#define D_DUM_REG_DLP_PMSEQMISB2UR          (0xa2010a98)
#define D_DUM_REG_DLP_PMRCVFCB2UR           (0xa2010a9c)
#define D_DUM_REG_DLP_PMILGAPHB2DS          (0xa2010ac0)
#define D_DUM_REG_DLP_PMILGTIMB2DS          (0xa2010ac4)
#define D_DUM_REG_DLP_PMTIMTEB2DS           (0xa2010ac8)
#define D_DUM_REG_DLP_PMTIMTLB2DS           (0xa2010acc)
#define D_DUM_REG_DLP_PMONTIMB2DS           (0xa2010ad0)
#define D_DUM_REG_DLP_PMSEQDUPB2DS          (0xa2010ad4)
#define D_DUM_REG_DLP_PMSEQMISB2DS          (0xa2010ad8)
#define D_DUM_REG_DLP_PMRCVFCB2DS           (0xa2010adc)
#define D_DUM_REG_DLP_PMRCVUPB2DS           (0xa2010ae0)
#define D_DUM_REG_DLP_PMILGAPHA3DN          (0xa2010700)
#define D_DUM_REG_DLP_PMILGTIMA3DN          (0xa2010704)
#define D_DUM_REG_DLP_PMTIMTEA3DN           (0xa2010708)
#define D_DUM_REG_DLP_PMTIMTLA3DN           (0xa201070c)
#define D_DUM_REG_DLP_PMONTIMA3DN           (0xa2010710)
#define D_DUM_REG_DLP_PMSEQDUPA3DN          (0xa2010714)
#define D_DUM_REG_DLP_PMSEQMISA3DN          (0xa2010718)
#define D_DUM_REG_DLP_PMRCVFCA3DN           (0xa201071c)
#define D_DUM_REG_DLP_PMRCVUPA3DN           (0xa2010720)
#define D_DUM_REG_DLP_PMILGAPHA3UN          (0xa2010740)
#define D_DUM_REG_DLP_PMILGTIMA3UN          (0xa2010744)
#define D_DUM_REG_DLP_PMTIMTEA3UN           (0xa2010748)
#define D_DUM_REG_DLP_PMTIMTLA3UN           (0xa201074c)
#define D_DUM_REG_DLP_PMONTIMA3UN           (0xa2010750)
#define D_DUM_REG_DLP_PMSEQDUPA3UN          (0xa2010754)
#define D_DUM_REG_DLP_PMSEQMISA3UN          (0xa2010758)
#define D_DUM_REG_DLP_PMRCVFCA3UN           (0xa201075c)
#define D_DUM_REG_DLP_PMILGAPHA3UR          (0xa2010780)
#define D_DUM_REG_DLP_PMILGTIMA3UR          (0xa2010784)
#define D_DUM_REG_DLP_PMTIMTEA3UR           (0xa2010788)
#define D_DUM_REG_DLP_PMTIMTLA3UR           (0xa201078c)
#define D_DUM_REG_DLP_PMONTIMA3UR           (0xa2010790)
#define D_DUM_REG_DLP_PMSEQDUPA3UR          (0xa2010794)
#define D_DUM_REG_DLP_PMSEQMISA3UR          (0xa2010798)
#define D_DUM_REG_DLP_PMRCVFCA3UR           (0xa201079c)
#define D_DUM_REG_DLP_PMILGAPHA3DS          (0xa20107c0)
#define D_DUM_REG_DLP_PMILGTIMA3DS          (0xa20107c4)
#define D_DUM_REG_DLP_PMTIMTEA3DS           (0xa20107c8)
#define D_DUM_REG_DLP_PMTIMTLA3DS           (0xa20107cc)
#define D_DUM_REG_DLP_PMONTIMA3DS           (0xa20107d0)
#define D_DUM_REG_DLP_PMSEQDUPA3DS          (0xa20107d4)
#define D_DUM_REG_DLP_PMSEQMISA3DS          (0xa20107d8)
#define D_DUM_REG_DLP_PMRCVFCA3DS           (0xa20107dc)
#define D_DUM_REG_DLP_PMRCVUPA3DS           (0xa20107e0)
#define D_DUM_REG_DLP_PMILGAPHB3DN          (0xa2010b00)
#define D_DUM_REG_DLP_PMILGTIMB3DN          (0xa2010b04)
#define D_DUM_REG_DLP_PMTIMTEB3DN           (0xa2010b08)
#define D_DUM_REG_DLP_PMTIMTLB3DN           (0xa2010b0c)
#define D_DUM_REG_DLP_PMONTIMB3DN           (0xa2010b10)
#define D_DUM_REG_DLP_PMSEQDUPB3DN          (0xa2010b14)
#define D_DUM_REG_DLP_PMSEQMISB3DN          (0xa2010b18)
#define D_DUM_REG_DLP_PMRCVFCB3DN           (0xa2010b1c)
#define D_DUM_REG_DLP_PMRCVUPB3DN           (0xa2010b20)
#define D_DUM_REG_DLP_PMILGAPHB3UN          (0xa2010b40)
#define D_DUM_REG_DLP_PMILGTIMB3UN          (0xa2010b44)
#define D_DUM_REG_DLP_PMTIMTEB3UN           (0xa2010b48)
#define D_DUM_REG_DLP_PMTIMTLB3UN           (0xa2010b4c)
#define D_DUM_REG_DLP_PMONTIMB3UN           (0xa2010b50)
#define D_DUM_REG_DLP_PMSEQDUPB3UN          (0xa2010b54)
#define D_DUM_REG_DLP_PMSEQMISB3UN          (0xa2010b58)
#define D_DUM_REG_DLP_PMRCVFCB3UN           (0xa2010b5c)
#define D_DUM_REG_DLP_PMILGAPHB3UR          (0xa2010b80)
#define D_DUM_REG_DLP_PMILGTIMB3UR          (0xa2010b84)
#define D_DUM_REG_DLP_PMTIMTEB3UR           (0xa2010b88)
#define D_DUM_REG_DLP_PMTIMTLB3UR           (0xa2010b8c)
#define D_DUM_REG_DLP_PMONTIMB3UR           (0xa2010b90)
#define D_DUM_REG_DLP_PMSEQDUPB3UR          (0xa2010b94)
#define D_DUM_REG_DLP_PMSEQMISB3UR          (0xa2010b98)
#define D_DUM_REG_DLP_PMRCVFCB3UR           (0xa2010b9c)
#define D_DUM_REG_DLP_PMILGAPHB3DS          (0xa2010bc0)
#define D_DUM_REG_DLP_PMILGTIMB3DS          (0xa2010bc4)
#define D_DUM_REG_DLP_PMTIMTEB3DS           (0xa2010bc8)
#define D_DUM_REG_DLP_PMTIMTLB3DS           (0xa2010bcc)
#define D_DUM_REG_DLP_PMONTIMB3DS           (0xa2010bd0)
#define D_DUM_REG_DLP_PMSEQDUPB3DS          (0xa2010bd4)
#define D_DUM_REG_DLP_PMSEQMISB3DS          (0xa2010bd8)
#define D_DUM_REG_DLP_PMRCVFCB3DS           (0xa2010bdc)
#define D_DUM_REG_DLP_PMRCVUPB3DS           (0xa2010be0)



/* @} */
#endif
/* @} */
