/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_cnt.h
 * @brief レジスタアドレス定義ヘッダ(CNT)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CNT_H
#define F_RRH_REG_CNT_H

/*!
 * @name CNTレジスタアドレス(master and slave)
 * @note CNTレジスタアドレス(master and slave)
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
/* master */
#define D_RRH_REG_CNT_FPGAVER					0xC0000000
#define D_RRH_REG_CNT_REGCHK					0xC0000004
#define D_RRH_REG_CNT_RST						0xC0000040
#define D_RRH_REG_CNT_SRSTREQ					0xC0000050
#define D_RRH_REG_CNT_CLKCNT					0xC0000060
#define D_RRH_REG_CNT_STA1						0xC0000200
#define D_RRH_REG_CNT_STA1M						0xC0000204
#define D_RRH_REG_CNT_STA2						0xC0000210
#define D_RRH_REG_CNT_STA2M						0xC0000214
#define D_RRH_REG_CNT_EXPLLALM1					0xC0000380
#define D_RRH_REG_CNT_SFPVALM					0xC0000390
#define D_RRH_REG_CNT_MNTLED					0xC0000800
#define D_RRH_REG_CNT_RSTM						0xC0000900
#define D_RRH_REG_CNT_RSTWAIT					0xC0000904
#define D_RRH_REG_CNT_SVIFERR					0xC0000980
#define D_RRH_REG_CNT_SVIFTXEN					0xC000098C

/* slave */
#define D_RRH_REG_CNTS_FPGAVER					0xD0000000
#define D_RRH_REG_CNTS_REGCHK					0xD0000004
#define D_RRH_REG_CNTS_RTC1						0xD0000010
#define D_RRH_REG_CNTS_RTC2						0xD0000014
#define D_RRH_REG_CNTS_RSTPHY					0xD0000044
#define D_RRH_REG_CNTS_CLKCNT					0xD0000060
#define D_RRH_REG_CNTS_IRQCPP					0xD0000120
#define D_RRH_REG_CNTS_IRQCPPM					0xD0000124
#define D_RRH_REG_CNTS_IRQCPS					0xD0000128
#define D_RRH_REG_CNTS_IRQCPSM					0xD000012C
#define D_RRH_REG_CNTS_IRQDPD					0xD0000130
#define D_RRH_REG_CNTS_IRQDPDM					0xD0000134
#define D_RRH_REG_CNTS_IRQRX					0xD0000138
#define D_RRH_REG_CNTS_IRQRXM					0xD000013C
#define D_RRH_REG_CNTS_SV1INF					0xD0000150
#define D_RRH_REG_CNTS_SV2INF					0xD0000154
#define D_RRH_REG_CNTS_SV3INF					0xD0000158
#define D_RRH_REG_CNTS_SV4INF					0xD000015C
#define D_RRH_REG_CNTS_SV5INF					0xD0000160
#define D_RRH_REG_CNTS_SV6INF					0xD0000164
#define D_RRH_REG_CNTS_SV7INF					0xD0000168
#define D_RRH_REG_CNTS_SV8INF					0xD000016C
#define D_RRH_REG_CNTS_SV1CMPM					0xD0000170
#define D_RRH_REG_CNTS_SV2CMPM					0xD0000174
#define D_RRH_REG_CNTS_SV3CMPM					0xD0000178
#define D_RRH_REG_CNTS_SV4CMPM					0xD000017C
#define D_RRH_REG_CNTS_SV5CMPM					0xD0000180
#define D_RRH_REG_CNTS_SV6CMPM					0xD0000184
#define D_RRH_REG_CNTS_SV7CMPM					0xD0000188
#define D_RRH_REG_CNTS_SV8CMPM					0xD000018C
#define D_RRH_REG_CNTS_STA1						0xD0000200
#define D_RRH_REG_CNTS_STA1M					0xD0000204
#define D_RRH_REG_CNTS_STA2						0xD0000210
#define D_RRH_REG_CNTS_STA2M					0xD0000214
#define D_RRH_REG_CNTS_CPRSTA					0xD0000280
#define D_RRH_REG_CNTS_TDDDISSW					0xD0000290
#define D_RRH_REG_CNTS_TDDISM1L					0xD00002A0
#define D_RRH_REG_CNTS_TDDISM1H					0xD00002A4
#define D_RRH_REG_CNTS_TDDISM2L					0xD00002A8
#define D_RRH_REG_CNTS_TDDISM2H					0xD00002AC
#define D_RRH_REG_CNTS_TDDISM3L					0xD00002B0
#define D_RRH_REG_CNTS_TDDISM3H					0xD00002B4
#define D_RRH_REG_CNTS_TDDISM4L					0xD00002B8
#define D_RRH_REG_CNTS_TDDISM4H					0xD00002BC
#define D_RRH_REG_CNTS_UL1IQSDM					0xD0000300
#define D_RRH_REG_CNTS_DL1IQSDM					0xD0000308
#define D_RRH_REG_CNTS_MIO3CHK					0xD00003A0
#define D_RRH_REG_CNTS_MIO5CHK					0xD00003A4
#define D_RRH_REG_CNTS_MNTLED					0xD0000800




#elif defined  (OPT_RRH_ARCH_ZYNQ)
/* master */
#define D_RRH_REG_CNT_FPGAVER					0x40000000
#define D_RRH_REG_CNT_REGCHK					0x40000004
#define D_RRH_REG_CNT_PAPWREN					0x40000020
#define D_RRH_REG_CNT_PAPWRON					0x40000024
#define D_RRH_REG_CNT_LDMOSEN					0x40000028
#define   D_RRH_REG_CNT_LDMOSEN_LDMOSEN		1
#define D_RRH_REG_CNT_DRAINCNT					0x4000002c
#define   D_RRH_REG_CNT_DRAINCNT_MASK					3
#define D_RRH_REG_CNT_FANPOWEN					0x40000030
#define D_RRH_REG_CNT_RST						0x40000040
#define D_RRH_REG_CNT_SRSTREQ					0x40000050
#define D_RRH_REG_CNT_CLKCNT					0x40000060
#define D_RRH_REG_CNT_IRQAISG					0x40000110
#define D_RRH_REG_CNT_IRQAISGM					0x40000114
#define D_RRH_REG_CNT_IRQSFPMNT					0x40000150
#define D_RRH_REG_CNT_CNTM_SFPPMNT				0x40000154
#define D_RRH_REG_CNT_STA1						0x40000200
#define D_RRH_REG_CNT_STA1M						0x40000204
#define D_RRH_REG_CNT_STA2						0x40000210
#define D_RRH_REG_CNT_STA2M						0x40000214
#define D_RRH_REG_CNT_EXPLLALM1					0x40000380
#define D_RRH_REG_CNT_SFPVALM					0x40000390
#define D_RRH_REG_CNT_FANEDGCNT					0x400003A0
#define D_RRH_REG_CNT_TRXCFG					0x40000600
#define D_RRH_REG_CNT_GPIFSET					0x40000700
#define D_RRH_REG_CNT_GPIFTCK					0x40000704
#define D_RRH_REG_CNT_GPIFEN					0x40000708
#define D_RRH_REG_CNT_GPIFTXD					0x40000710
#define D_RRH_REG_CNT_MNTLED					0x40000800
#define D_RRH_REG_CNT_RSTM						0x40000900
#define D_RRH_REG_CNT_RSTWAIT					0x40000904
#define D_RRH_REG_CNT_SVIFERR					0x40000980
#define D_RRH_REG_CNT_SVIFTXEN					0x4000098C

/* slave */
#define D_RRH_REG_CNTS_FPGAVER					0x80000000
#define D_RRH_REG_CNTS_REGCHK					0x80000004
#define D_RRH_REG_CNTS_RTC1						0x80000010
#define D_RRH_REG_CNTS_RTC2						0x80000014
#define D_RRH_REG_CNTS_RSTPHY					0x80000044
#define D_RRH_REG_CNTS_PHYPWR					0x80000048
#define D_RRH_REG_CNTS_CLKCNT					0x80000060
#define D_RRH_REG_CNTS_IRQCPP					0x80000120
#define D_RRH_REG_CNTS_IRQCPPM					0x80000124
#define D_RRH_REG_CNTS_IRQCPS					0x80000128
#define D_RRH_REG_CNTS_IRQCPSM					0x8000012C
#define D_RRH_REG_CNTS_IRQDPD					0x80000130
#define D_RRH_REG_CNTS_IRQDPDM					0x80000134
#define D_RRH_REG_CNTS_IRQRX					0x80000138
#define D_RRH_REG_CNTS_IRQRXM					0x8000013C
#define D_RRH_REG_CNTS_SV1INF					0x80000150
#define D_RRH_REG_CNTS_SV2INF					0x80000154
#define D_RRH_REG_CNTS_SV3INF					0x80000158
#define D_RRH_REG_CNTS_SV4INF					0x8000015C
#define D_RRH_REG_CNTS_SV5INF					0x80000160
#define D_RRH_REG_CNTS_SV6INF					0x80000164
#define D_RRH_REG_CNTS_SV7INF					0x80000168
#define D_RRH_REG_CNTS_SV8INF					0x8000016C
#define D_RRH_REG_CNTS_SV1CMPM					0x80000170
#define D_RRH_REG_CNTS_SV2CMPM					0x80000174
#define D_RRH_REG_CNTS_SV3CMPM					0x80000178
#define D_RRH_REG_CNTS_SV4CMPM					0x8000017C
#define D_RRH_REG_CNTS_SV5CMPM					0x80000180
#define D_RRH_REG_CNTS_SV6CMPM					0x80000184
#define D_RRH_REG_CNTS_SV7CMPM					0x80000188
#define D_RRH_REG_CNTS_SV8CMPM					0x8000018C
#define D_RRH_REG_CNTS_SVCAS1INF				0x80000190
#define D_RRH_REG_CNTS_SVCAS2INF				0x80000194
#define D_RRH_REG_CNTS_SVCAS3INF				0x80000198
#define D_RRH_REG_CNTS_SVCAS4INF				0x8000019C
#define D_RRH_REG_CNTS_SVCAS5INF				0x800001A0
#define D_RRH_REG_CNTS_SVCAS6INF				0x800001A4
#define D_RRH_REG_CNTS_SVCAS7INF				0x800001A8
#define D_RRH_REG_CNTS_SVCAS8INF				0x800001AC
#define D_RRH_REG_CNTS_SVCAS1CMPM				0x800001B0
#define D_RRH_REG_CNTS_SVCAS2CMPM				0x800001B4
#define D_RRH_REG_CNTS_SVCAS3CMPM				0x800001B8
#define D_RRH_REG_CNTS_SVCAS4CMPM				0x800001BC
#define D_RRH_REG_CNTS_SVCAS5CMPM				0x800001C0
#define D_RRH_REG_CNTS_SVCAS6CMPM				0x800001C4
#define D_RRH_REG_CNTS_SVCAS7CMPM				0x800001C8
#define D_RRH_REG_CNTS_SVCAS8CMPM				0x800001CC
#define D_RRH_REG_CNTS_STA1						0x80000200
#define D_RRH_REG_CNTS_STA1M					0x80000204
#define D_RRH_REG_CNTS_STA2						0x80000210
#define D_RRH_REG_CNTS_STA2M					0x80000214
#define D_RRH_REG_CNTS_CPRSTA					0x80000280
#define D_RRH_REG_CNTS_TDDDISSW					0x80000290
#define D_RRH_REG_CNTS_TDDISM1L					0x800002A0
#define D_RRH_REG_CNTS_TDDISM1H					0x800002A4
#define D_RRH_REG_CNTS_TDDISM2L					0x800002A8
#define D_RRH_REG_CNTS_TDDISM2H					0x800002AC
#define D_RRH_REG_CNTS_TDDISM3L					0x800002B0
#define D_RRH_REG_CNTS_TDDISM3H					0x800002B4
#define D_RRH_REG_CNTS_TDDISM4L					0x800002B8
#define D_RRH_REG_CNTS_TDDISM4H					0x800002BC
#define D_RRH_REG_CNTS_UL1IQSDM					0x80000300
#define D_RRH_REG_CNTS_UL2IQSDM					0x80000304
#define D_RRH_REG_CNTS_DL1IQSDM					0x80000308
#define D_RRH_REG_CNTS_MIO3CHK					0x800003A0
#define D_RRH_REG_CNTS_MIO5CHK					0x800003A4
#define D_RRH_REG_CNTS_MNTLED					0x80000800
#define D_RRH_REG_CNTS_CPPPHYCNT				0x80000900
#define D_RRH_REG_CNTS_CPSPHYCNT                0x80000904
#define D_RRH_REG_CNTS_JS1PHYCNT                0x80000908
#define D_RRH_REG_CNTS_JS2PHYCNT                0x8000090C
#define D_RRH_REG_CNTS_JS3PHYCNT                0x80000910
#define D_RRH_REG_CNTS_JS4PHYCNT                0x80000914
#define D_RRH_REG_CNTS_JS5PHYCNT                0x80000918
#define D_RRH_REG_CNTS_JS6PHYCNT                0x8000091C
#define D_RRH_REG_CNTS_JS7PHYCNT                0x80000920
#define D_RRH_REG_CNTS_JS8PHYCNT                0x80000924
#define D_RRH_REG_CNTS_SVPRI1INF				0x80000A00
#define D_RRH_REG_CNTS_SVPRI2INF				0x80000A04
#define D_RRH_REG_CNTS_SVPRI3INF				0x80000A08
#define D_RRH_REG_CNTS_SVPRI4INF				0x80000A0C
#define D_RRH_REG_CNTS_SVPRI5INF				0x80000A10
#define D_RRH_REG_CNTS_SVPRI6INF				0x80000A14
#define D_RRH_REG_CNTS_SVPRI7INF				0x80000A18
#define D_RRH_REG_CNTS_SVPRI8INF				0x80000A1C
#define D_RRH_REG_CNTS_SVPRI1CMPM				0x80000A20
#define D_RRH_REG_CNTS_SVPRI2CMPM				0x80000A24
#define D_RRH_REG_CNTS_SVPRI3CMPM				0x80000A28
#define D_RRH_REG_CNTS_SVPRI4CMPM				0x80000A2C
#define D_RRH_REG_CNTS_SVPRI5CMPM				0x80000A30
#define D_RRH_REG_CNTS_SVPRI6CMPM				0x80000A34
#define D_RRH_REG_CNTS_SVPRI7CMPM				0x80000A38
#define D_RRH_REG_CNTS_SVPRI8CMPM				0x80000A3C
#define D_RRH_REG_CNTS_SV9INF					0x80000A80
#define D_RRH_REG_CNTS_SV10INF					0x80000A84
#define D_RRH_REG_CNTS_SV11INF					0x80000A88
#define D_RRH_REG_CNTS_SV12INF					0x80000A8C
#define D_RRH_REG_CNTS_SV13INF					0x80000A90
#define D_RRH_REG_CNTS_SV14INF					0x80000A94
#define D_RRH_REG_CNTS_SV15INF					0x80000A98
#define D_RRH_REG_CNTS_SV16INF					0x80000A9C
#define D_RRH_REG_CNTS_SV9CMPM					0x80000AA0
#define D_RRH_REG_CNTS_SV10CMPM					0x80000AA4
#define D_RRH_REG_CNTS_SV11CMPM					0x80000AA8
#define D_RRH_REG_CNTS_SV12CMPM					0x80000AAC
#define D_RRH_REG_CNTS_SV13CMPM					0x80000AB0
#define D_RRH_REG_CNTS_SV14CMPM					0x80000AB4
#define D_RRH_REG_CNTS_SV15CMPM					0x80000AB8
#define D_RRH_REG_CNTS_SV16CMPM					0x80000ABC
#define D_RRH_REG_CNTS_SVCAS9INF				0x80000AC0
#define D_RRH_REG_CNTS_SVCAS10INF				0x80000AC4
#define D_RRH_REG_CNTS_SVCAS11INF				0x80000AC8
#define D_RRH_REG_CNTS_SVCAS12INF				0x80000ACC
#define D_RRH_REG_CNTS_SVCAS13INF				0x80000AD0
#define D_RRH_REG_CNTS_SVCAS14INF				0x80000AD4
#define D_RRH_REG_CNTS_SVCAS15INF				0x80000AD8
#define D_RRH_REG_CNTS_SVCAS16INF				0x80000ADC
#define D_RRH_REG_CNTS_SVCAS9CMPM				0x80000AE0
#define D_RRH_REG_CNTS_SVCAS10CMPM				0x80000AE4
#define D_RRH_REG_CNTS_SVCAS11CMPM				0x80000AE8
#define D_RRH_REG_CNTS_SVCAS12CMPM				0x80000AEC
#define D_RRH_REG_CNTS_SVCAS13CMPM				0x80000AF0
#define D_RRH_REG_CNTS_SVCAS14CMPM				0x80000AF4
#define D_RRH_REG_CNTS_SVCAS15CMPM				0x80000AF8
#define D_RRH_REG_CNTS_SVCAS16CMPM				0x80000AFC
#define D_RRH_REG_CNTS_SVPRI9INF				0x80000B00
#define D_RRH_REG_CNTS_SVPRI10INF				0x80000B04
#define D_RRH_REG_CNTS_SVPRI11INF				0x80000B08
#define D_RRH_REG_CNTS_SVPRI12INF				0x80000B0C
#define D_RRH_REG_CNTS_SVPRI13INF				0x80000B10
#define D_RRH_REG_CNTS_SVPRI14INF				0x80000B14
#define D_RRH_REG_CNTS_SVPRI15INF				0x80000B18
#define D_RRH_REG_CNTS_SVPRI16INF				0x80000B1C
#define D_RRH_REG_CNTS_SVPRI9CMPM				0x80000B20
#define D_RRH_REG_CNTS_SVPRI10CMPM				0x80000B24
#define D_RRH_REG_CNTS_SVPRI11CMPM				0x80000B28
#define D_RRH_REG_CNTS_SVPRI12CMPM				0x80000B2C
#define D_RRH_REG_CNTS_SVPRI13CMPM				0x80000B30
#define D_RRH_REG_CNTS_SVPRI14CMPM				0x80000B34
#define D_RRH_REG_CNTS_SVPRI15CMPM				0x80000B38
#define D_RRH_REG_CNTS_SVPRI16CMPM				0x80000B3C

#else

#endif
/* @} */





/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* PAPWREN	*/
#define D_RRH_CNT_PAPWREN_PAPWREN				D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNT_PAPWREN_AISG					D_RRH_VAL_SHIFT_L(1 , 1)
/* PAPWRON */
#define D_RRH_CNT_PAPWRON_PA1					D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNT_PAPWRON_PA2					D_RRH_VAL_SHIFT_L(1 , 1)
/* RST */
#define D_RRH_CNT_RST_UNIRX2					D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNT_RST_UNIRX1					D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNT_RST_UNIFWFB2					D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNT_RST_UNIFWFB1					D_RRH_VAL_SHIFT_L(1 , 3)
#define D_RRH_CNT_RST_SYSLOG					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_RST_SYSPLL					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_RST_EXPLL						D_RRH_VAL_SHIFT_L(1 , 0)

#define D_RRH_CNT_STA1_PCIEERR					D_RRH_VAL_SHIFT_L(1 , 30)
#define D_RRH_CNT_STA1_SLVSVERR					D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNT_STA1_MMISVERR					D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNT_STA1_PORTIN					D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNT_STA1_SFPSTXFLT				D_RRH_VAL_SHIFT_L(1 , 21)
#define D_RRH_CNT_STA1_SFPPTXFLT				D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNT_STA1_SFPSMNT					D_RRH_VAL_SHIFT_L(1 , 19)
#define D_RRH_CNT_STA1_SFPPMNT					D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNT_STA1_SFPSLOS					D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNT_STA1_SFPPLOS					D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNT_STA1_PFA						D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNT_STA1_EPLLALM					D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNT_STA1_PSALM8					D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNT_STA1_PSALM7					D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNT_STA1_PSALM6					D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNT_STA1_PSALM5					D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNT_STA1_PSALM4					D_RRH_VAL_SHIFT_L(1 , 3)
#define D_RRH_CNT_STA1_PSALM3					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_STA1_PSALM2					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_STA1_PSALM1					D_RRH_VAL_SHIFT_L(1 , 0)

#define D_RRH_CNT_STA1M_PCIEERR					D_RRH_VAL_SHIFT_L(1 , 30)
#define D_RRH_CNT_STA1M_SLVSVERR				D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNT_STA1M_MMISVERR				D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNT_STA1M_PORTIN					D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNT_STA1M_FANALM1					D_RRH_VAL_SHIFT_L(1 , 23)
#define D_RRH_CNT_STA1M_FANALM0					D_RRH_VAL_SHIFT_L(1 , 22)
#define D_RRH_CNT_STA1M_SFPSTXFLT				D_RRH_VAL_SHIFT_L(1 , 21)
#define D_RRH_CNT_STA1M_SFPPTXFLT				D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNT_STA1M_SFPSMNT					D_RRH_VAL_SHIFT_L(1 , 19)
#define D_RRH_CNT_STA1M_SFPPMNT					D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNT_STA1M_SFPSLOS					D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNT_STA1M_SFPPLOS					D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNT_STA1M_PFA						D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNT_STA1M_EPLLALM					D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNT_STA1M_PSALM8					D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNT_STA1M_PSALM7					D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNT_STA1M_PSALM6					D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNT_STA1M_PSALM5					D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNT_STA1M_PSALM4					D_RRH_VAL_SHIFT_L(1 , 3)
#define D_RRH_CNT_STA1M_PSALM3					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_STA1M_PSALM2					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_STA1M_PSALM1					D_RRH_VAL_SHIFT_L(1 , 0)

/* 割込み(IRQ87)マスク：SFP Mount変化 bit定義 */
#define D_RRH_REG_CNT_CNTM_SFPPMNT_CPP				D_RRH_VAL_SHIFT_L(1 , 0)	/* PrimarySFP */
#define D_RRH_REG_CNT_CNTM_SFPPMNT_CPS				D_RRH_VAL_SHIFT_L(1 , 1)	/* SecondarySFP */

/* LED2 */
#define D_RRH_CNT_LED2SYSG2_ON					0x00000400
#define D_RRH_CNT_LED2SYSO_NOUSE				0x00000100
#define D_RRH_CNT_LED2SYSO_CYCL					0x00000200
#define D_RRH_CNT_LED2SYSO_ON					0x00000300
#define D_RRH_CNT_LED2OFF						0x00000000
#define D_RRH_CNT_LED2MSK						( D_RRH_CNT_LED2SYSO_NOUSE | D_RRH_CNT_LED2SYSO_CYCL | D_RRH_CNT_LED2SYSO_ON | D_RRH_CNT_LED2SYSG2_ON )

/* LED1 */
#define D_RRH_CNT_LED1SYSGR_ON					0x00000010
#define D_RRH_CNT_LED1SYSG1_NOUSE				0x00000004
#define D_RRH_CNT_LED1SYSG1_CYCL				0x00000008
#define D_RRH_CNT_LED1SYSG1_ON					0x0000000C
#define D_RRH_CNT_LED1SYSR_NOUSE				0x00000001
#define D_RRH_CNT_LED1SYSR_CYCL					0x00000002
#define D_RRH_CNT_LED1SYSR_ON					0x00000003
#define D_RRH_CNT_LED1OFF						0x00000000
#define D_RRH_CNT_LED1MSK						\
			( D_RRH_CNT_LED1SYSGR_ON | D_RRH_CNT_LED1SYSG1_NOUSE | D_RRH_CNT_LED1SYSG1_CYCL | \
			  D_RRH_CNT_LED1SYSG1_ON | D_RRH_CNT_LED1SYSR_NOUSE | D_RRH_CNT_LED1SYSR_CYCL | D_RRH_CNT_LED1SYSR_ON )

#define D_RRH_CNT_TRXCFG_CFGDONE				D_RRH_VAL_SHIFT_L(1 , 8)

/* RST PHY */
#define D_RRH_CNTS_RSTPHY_JSRX2					D_RRH_VAL_SHIFT_L(1 , 11)
#define D_RRH_CNTS_RSTPHY_JSFB2					D_RRH_VAL_SHIFT_L(1 , 10)
#define D_RRH_CNTS_RSTPHY_JSFW2					D_RRH_VAL_SHIFT_L(1 , 9)
#define D_RRH_CNTS_RSTPHY_JSPLL2				D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNTS_RSTPHY_JSRX1					D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNTS_RSTPHY_JSFB1					D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNTS_RSTPHY_JSFW1					D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNTS_RSTPHY_JSPLL1				D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNTS_RSTPHY_CPS					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNTS_RSTPHY_CPP					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNTS_RSTPHY_CPPLL					D_RRH_VAL_SHIFT_L(1 , 0)

/* PHY PWR */
#define D_RRH_CNTS_PHYPWR_CPS					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNTS_PHYPWR_CPP					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNTS_PHYPWR_CPPLL					D_RRH_VAL_SHIFT_L(1 , 0)

/* IRQCPP */
#define D_RRH_CNTS_IRQCPP_HDLC_ACT				0x00004000
#define D_RRH_CNTS_IRQCPP_HDLC_IDLE				0x00002000
#define D_RRH_CNTS_IRQCPP_STED					0x00001000
#define D_RRH_CNTS_IRQCPP_STE					0x00000800
#define D_RRH_CNTS_IRQCPP_STD					0x00000400
#define D_RRH_CNTS_IRQCPP_STC					0x00000200
#define D_RRH_CNTS_IRQCPP_STB					0x00000100
#define D_RRH_CNTS_IRQCPP_STA					0x00000080
#define D_RRH_CNTS_IRQCPP_HDLCRATE				0x00000040
#define D_RRH_CNTS_IRQCPP_HFNSYNC				0x00000008
#define D_RRH_CNTS_IRQCPPM_MASK				\
			( D_RRH_CNTS_IRQCPP_HDLC_ACT | D_RRH_CNTS_IRQCPP_HDLC_IDLE | D_RRH_CNTS_IRQCPP_STED | \
			D_RRH_CNTS_IRQCPP_STE | D_RRH_CNTS_IRQCPP_STD | D_RRH_CNTS_IRQCPP_STC | \
			D_RRH_CNTS_IRQCPP_STB | D_RRH_CNTS_IRQCPP_STA )

#define D_RRH_CNTS_IRQCPPM_UNMASK				(~(D_RRH_CNTS_IRQCPPM_MASK) & 0xFFFF)

/* IRQCPS */
#define D_RRH_CNTS_IRQCPS_HDLC_ACT				0x00004000
#define D_RRH_CNTS_IRQCPS_HDLC_IDLE				0x00002000
#define D_RRH_CNTS_IRQCPS_STED					0x00001000
#define D_RRH_CNTS_IRQCPS_STE					0x00000800
#define D_RRH_CNTS_IRQCPS_STD					0x00000400
#define D_RRH_CNTS_IRQCPS_STC					0x00000200
#define D_RRH_CNTS_IRQCPS_STB					0x00000100
#define D_RRH_CNTS_IRQCPS_STA					0x00000080
#define D_RRH_CNTS_IRQCPS_HDLCRATE				0x00000040
#define D_RRH_CNTS_IRQCPS_HFNSYNC				0x00000008
#define D_RRH_CNTS_IRQCPSM_MASK				\
			( D_RRH_CNTS_IRQCPS_HDLC_ACT | D_RRH_CNTS_IRQCPS_HDLC_IDLE | D_RRH_CNTS_IRQCPS_STED | \
			D_RRH_CNTS_IRQCPS_STE | D_RRH_CNTS_IRQCPS_STD | D_RRH_CNTS_IRQCPS_STC | \
			D_RRH_CNTS_IRQCPS_STB | D_RRH_CNTS_IRQCPS_STA )

#define D_RRH_CNTS_IRQCPSM_UNMASK				(~(D_RRH_CNTS_IRQCPSM_MASK) & 0xFFFF)


#define D_RRH_CNTS_STA1_PCIEERR				D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNTS_STA1_SVERR				D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNTS_STA1_DL1IQSHT			D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNTS_STA1_UL2IQSHT			D_RRH_VAL_SHIFT_L(1 , 25)
#define D_RRH_CNTS_STA1_UL1IQSHT			D_RRH_VAL_SHIFT_L(1 , 24)
#define D_RRH_CNTS_STA1_PFA					D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNTS_STA1_JSRX2SERDONE		D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNTS_STA1_JSFB2SERDONE		D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNTS_STA1_JSFW2SERDONE		D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNTS_STA1_JSRX1SERDONE		D_RRH_VAL_SHIFT_L(1 , 14)
#define D_RRH_CNTS_STA1_JSFB1SERDONE		D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNTS_STA1_JSFW1SERDONE		D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNTS_STA1_CPSSERDONE			D_RRH_VAL_SHIFT_L(1 , 9)
#define D_RRH_CNTS_STA1_CPPSERDONE			D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNTS_STA1_PHYJS2PLLLOCK		D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNTS_STA1_PHYJS1PLLLOCK		D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNTS_STA1_PHYCPPLLLOCK		D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNTS_STA1_PHYPWRGOOD			D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNTS_STA1_CHK_SYSPLL			D_RRH_VAL_SHIFT_L(1 , 0)

#define D_RRH_CNTS_STA1M_PCIEERR			D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNTS_STA1M_SVERR				D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNTS_STA1M_DL1IQSHT			D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNTS_STA1M_UL2IQSHT			D_RRH_VAL_SHIFT_L(1 , 25)
#define D_RRH_CNTS_STA1M_UL1IQSHT			D_RRH_VAL_SHIFT_L(1 , 24)
#define D_RRH_CNTS_STA1M_PFA				D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNTS_STA1M_JSRX2SERDONE		D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNTS_STA1M_JSFB2SERDONE		D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNTS_STA1M_JSFW2SERDONE		D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNTS_STA1M_JSRX1SERDONE		D_RRH_VAL_SHIFT_L(1 , 14)
#define D_RRH_CNTS_STA1M_JSFB1SERDONE		D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNTS_STA1M_JSFW1SERDONE		D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNTS_STA1M_CPSSERDONE			D_RRH_VAL_SHIFT_L(1 , 9)
#define D_RRH_CNTS_STA1M_CPPSERDONE			D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNTS_STA1M_PHYJS2PLLLOCK		D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNTS_STA1M_PHYJS1PLLLOCK		D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNTS_STA1M_PHYCPPLLLOCK		D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNTS_STA1M_PHYPWRGOOD			D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNTS_STA1M_SYSPULCK			D_RRH_VAL_SHIFT_L(1 , 0)

#define	D_RRH_REG_CNT_FANPOWERON_BIT	D_RRH_VAL_SHIFT_L(1 , 0)		/* 40000030 FANPOWEN FAN Power Enable制御 bit0 1：oN 0：oFF	外部端子"OXFANPOWEN"反転出力 ※自律制御なし
*/
#define	D_RRH_REG_CNT_STA1M_PSALM4_BIT	D_RRH_VAL_SHIFT_L(1,3)			/* 40000204 FPGA状態表示レジスタ1マスク bit3 PSALM4	FAN電源アラーム */

#if defined (OPT_RRH_ARCH_A10)

#define D_RRH_CNTS_STA2_TDDDISSW			D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNTS_STA2_SVERR				D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNTS_STA2_TDDANS				D_RRH_VAL_SHIFT_L(1 , 27)
#define D_RRH_CNTS_STA2_CPSL1LOF			D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNTS_STA2_CPPL1LOF			D_RRH_VAL_SHIFT_L(1 , 25)
#define D_RRH_CNTS_STA2_HFNERR				D_RRH_VAL_SHIFT_L(1 , 24)
#define D_RRH_CNTS_STA2_RXIRQ2				D_RRH_VAL_SHIFT_L(1 , 23)
#define D_RRH_CNTS_STA2_RXIRQ1				D_RRH_VAL_SHIFT_L(1 , 22)
#define D_RRH_CNTS_STA2_FWFBIRQ2			D_RRH_VAL_SHIFT_L(1 , 21)
#define D_RRH_CNTS_STA2_FWFBIRQ1			D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNTS_STA2_JSFW2LNKERR			D_RRH_VAL_SHIFT_L(1 , 19)
#define D_RRH_CNTS_STA2_JSFW1LNKERR			D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNTS_STA2_JSFW2DISC			D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNTS_STA2_JSFW1DISC			D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNTS_STA2_JSRXERR				D_RRH_VAL_SHIFT_L(1 , 14)
#define D_RRH_CNTS_STA2_JSFBERR				D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNTS_STA2_JSFWERR				D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNTS_STA2_JSRXSER2DONE		D_RRH_VAL_SHIFT_L(1 , 9)
#define D_RRH_CNTS_STA2_JSRXSER1DONE		D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNTS_STA2_JSFBSER2DONE		D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNTS_STA2_JSFBSER1DONE		D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNTS_STA2_JSFWSER2DONE		D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNTS_STA2_JSFWSER1DONE		D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNTS_STA2_CPSSERDONE			D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNTS_STA2_CPPSERDONE			D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNTS_STA2_CHK_SERDESDONE		 ( D_RRH_CNTS_STA2_CPPSERDONE | D_RRH_CNTS_STA2_CPSSERDONE )




#elif defined  (OPT_RRH_ARCH_ZYNQ)

#define D_RRH_CNTS_STA2_TDDDISSW4			D_RRH_VAL_SHIFT_L(1 , 31)
#define D_RRH_CNTS_STA2_TDDDISSW3			D_RRH_VAL_SHIFT_L(1 , 30)
#define D_RRH_CNTS_STA2_TDDDISSW2			D_RRH_VAL_SHIFT_L(1 , 29)
#define D_RRH_CNTS_STA2_TDDDISSW1			D_RRH_VAL_SHIFT_L(1 , 28)
#define D_RRH_CNTS_STA2_TXONANS4			D_RRH_VAL_SHIFT_L(1 , 27)
#define D_RRH_CNTS_STA2_TXONANS3			D_RRH_VAL_SHIFT_L(1 , 26)
#define D_RRH_CNTS_STA2_TXONANS2			D_RRH_VAL_SHIFT_L(1 , 25)
#define D_RRH_CNTS_STA2_TXONANS1			D_RRH_VAL_SHIFT_L(1 , 24)
#define D_RRH_CNTS_STA2_TDDANS4				D_RRH_VAL_SHIFT_L(1 , 23)
#define D_RRH_CNTS_STA2_TDDANS3				D_RRH_VAL_SHIFT_L(1 , 22)
#define D_RRH_CNTS_STA2_TDDANS2				D_RRH_VAL_SHIFT_L(1 , 21)
#define D_RRH_CNTS_STA2_TDDANS1				D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNTS_STA2_EPLLALM				D_RRH_VAL_SHIFT_L(1 , 19)
#define D_RRH_CNTS_STA2_CPSL1LOF			D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNTS_STA2_CPPL1LOF			D_RRH_VAL_SHIFT_L(1 , 17)
#define D_RRH_CNTS_STA2_HFNERR				D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNTS_STA2_TDDANSYA4			D_RRH_VAL_SHIFT_L(1 , 15)
#define D_RRH_CNTS_STA2_TDDANSYA3			D_RRH_VAL_SHIFT_L(1 , 14)
#define D_RRH_CNTS_STA2_TDDANSYA2			D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNTS_STA2_TDDANSYA1			D_RRH_VAL_SHIFT_L(1 , 12)
#define D_RRH_CNTS_STA2_JSFW2LNKERR			D_RRH_VAL_SHIFT_L(1 , 11)
#define D_RRH_CNTS_STA2_JSFW1LNKERR			D_RRH_VAL_SHIFT_L(1 , 10)
#define D_RRH_CNTS_STA2_JSFW2DISC			D_RRH_VAL_SHIFT_L(1 , 9)
#define D_RRH_CNTS_STA2_JSFW1DISC			D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNTS_STA2_RXIRQ2				D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNTS_STA2_RXIRQ1				D_RRH_VAL_SHIFT_L(1 , 6)
#define D_RRH_CNTS_STA2_FWFBIRQ2			D_RRH_VAL_SHIFT_L(1 , 5)
#define D_RRH_CNTS_STA2_FWFBIRQ1			D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNTS_STA2_JSRXERR				D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNTS_STA2_JSFBERR				D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNTS_STA2_JSFWERR				D_RRH_VAL_SHIFT_L(1 , 0)

#define D_RRH_CNTS_STA2M_TDDANSCA2			D_RRH_VAL_SHIFT_L(1 , 21)
#define D_RRH_CNTS_STA2M_TDDANSCA1			D_RRH_VAL_SHIFT_L(1 , 20)
#define D_RRH_CNTS_STA2M_TDDANSYA2			D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNTS_STA2M_TDDANSYA1			D_RRH_VAL_SHIFT_L(1 , 12)

#else

#endif

/* MIO3CHK */
#define D_RRH_CNTS_MIO3CHK_RX2MIO3				0x00000008
#define D_RRH_CNTS_MIO3CHK_RX1MIO3				0x00000004
#define D_RRH_CNTS_MIO3CHK_FWFB2MIO3			0x00000002
#define D_RRH_CNTS_MIO3CHK_FWFB1MIO3			0x00000001

/* MIO3CHK */
#define D_RRH_CNTS_MIO5CHK_RX2MIO5				0x00000008
#define D_RRH_CNTS_MIO5CHK_RX1MIO5				0x00000004
#define D_RRH_CNTS_MIO5CHK_FWFB2MIO5			0x00000002
#define D_RRH_CNTS_MIO5CHK_FWFB1MIO5			0x00000001

#define D_RRH_CNTS_MIO5CHK_RFIC_ALM1_AB			( D_RRH_CNTS_MIO5CHK_RX1MIO5 | D_RRH_CNTS_MIO5CHK_FWFB1MIO5 )
#define D_RRH_CNTS_MIO5CHK_RFIC_ALM1_CD			( D_RRH_CNTS_MIO5CHK_RX2MIO5 | D_RRH_CNTS_MIO5CHK_FWFB2MIO5 )

/* GPIFSET */
#define D_RRH_CNTM_GPIFSET_REQ					0x00020000
#define D_RRH_CNTM_GPIFSET_PHS					0x00010000

/* VSSレジスタ関連(親→子) */
#define D_RRH_CNTS_SVPRI9INF_PAOFFBR2			0x00000001
#define D_RRH_CNTS_SVPRI9INF_PAONBR2			0x00000002
#define D_RRH_CNTS_SVPRI9INF_ADJDOWNBR2			0x00000004
#define D_RRH_CNTS_SVPRI9INF_ADJUPBR2			0x00000008
#define D_RRH_CNTS_SVPRI9INF_BLKBR2				0x00000010
#define D_RRH_CNTS_SVPRI9INF_BLKRELBR2			0x00000020
#define D_RRH_CNTS_SVPRI9INF_PAOFFBR3			0x00000100
#define D_RRH_CNTS_SVPRI9INF_PAONBR3			0x00000200
#define D_RRH_CNTS_SVPRI9INF_ADJDOWNBR3			0x00000400
#define D_RRH_CNTS_SVPRI9INF_ADJUPBR3			0x00000800
#define D_RRH_CNTS_SVPRI9INF_BLKBR3				0x00001000
#define D_RRH_CNTS_SVPRI9INF_BLKRELBR3			0x00002000
#define D_RRH_CNTS_SVPRI9INF_POWSAVE			0x00010000
#define D_RRH_CNTS_SVPRI9INF_PAOFF				0x00020000
#define D_RRH_CNTS_SVPRI9INF_PAON				0x00040000
#define D_RRH_CNTS_SVPRI9INF_FANOFF				0x01000000
#define D_RRH_CNTS_SVPRI9INF_FANON				0x02000000

/* VSSレジスタ関連(子→親) */
#define D_RRH_CNTS_SV10INF_FAN_EN				0x00000001

/* AISG */
#define D_RRH_CNT_IRQAISG_RX					D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNT_IRQAISG_TX					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_IRQAISG_ER					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_IRQAISG_RXING					D_RRH_VAL_SHIFT_L(1 , 4)
#define D_RRH_CNT_IRQAISGM_RX					D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNT_IRQAISGM_TX					D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_IRQAISGM_ER					D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_IRQAISGM_RXING				D_RRH_VAL_SHIFT_L(1 , 4)

/* SVINF */
enum{
	E_RRH_CNT_SVSTS0 = 0,
	E_RRH_CNT_SVSTS1,
	E_RRH_CNT_SVSTS2,
	E_RRH_CNT_SVSTS3,
	E_RRH_CNT_SVSTS4,
	E_RRH_CNT_SVSTS6,
	E_RRH_CNT_SVSTS7,
};

#define D_RRH_CNT_SVINF_USE						D_RRH_VAL_SHIFT_L(1 , 2)
#define D_RRH_CNT_SVINF_BLK						D_RRH_VAL_SHIFT_L(1 , 10)
#define D_RRH_CNT_SVINF_ALM						D_RRH_VAL_SHIFT_L(1 , 1)
#define D_RRH_CNT_SVINF_ACT						D_RRH_VAL_SHIFT_L(1 , 13)
#define D_RRH_CNT_SVINF_SLEEP_BR2				D_RRH_VAL_SHIFT_L(1 , 7)
#define D_RRH_CNT_SVINF_SLEEP_BR3				D_RRH_VAL_SHIFT_L(1 , 8)
#define D_RRH_CNT_SVINF_AMPOFF					D_RRH_VAL_SHIFT_L(1 , 4)

#define D_RRH_CNT_SVINF_L1ERR					D_RRH_VAL_SHIFT_L(1 , 16)
#define D_RRH_CNT_SVINF_L2ERR					D_RRH_VAL_SHIFT_L(1 , 0)
#define D_RRH_CNT_SVINF_L1ERR_CAS				D_RRH_VAL_SHIFT_L(1 , 18)
#define D_RRH_CNT_SVINF_L2ERR_CAS				D_RRH_VAL_SHIFT_L(1 , 19)

/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_REG_CNT_BID               0x40000008  /**< ボード情報表示                                   */
#define D_RRH_REG_CNT_RSTQSPI           0x40000044  /**< QSPI-Flashリセット(4個共通)                      */
#define D_RRH_REG_CNT_SV1INF            0x40000150  /**< SV情報1                                          */
#define D_RRH_REG_CNT_SV2INF            0x40000154  /**< SV情報2                                          */
#define D_RRH_REG_CNT_SV3INF            0x40000158  /**< SV情報3                                          */
#define D_RRH_REG_CNT_SV4INF            0x4000015C  /**< SV情報4                                          */
#define D_RRH_REG_CNT_SV5INF            0x40000160  /**< SV情報5                                          */
#define D_RRH_REG_CNT_SV6INF            0x40000164  /**< SV情報6                                          */
#define D_RRH_REG_CNT_SV7INF            0x40000168  /**< SV情報7                                          */
#define D_RRH_REG_CNT_SV8INF            0x4000016C  /**< SV情報8                                          */
#define D_RRH_REG_CNT_SV9INF            0x40000170  /**< SV情報9                                          */
#define D_RRH_REG_CNT_SV10INF           0x40000174  /**< SV情報10                                         */
#define D_RRH_REG_CNT_SV11INF           0x40000178  /**< SV情報11                                         */
#define D_RRH_REG_CNT_SV12INF           0x4000017C  /**< SV情報12                                         */
#define D_RRH_REG_CNT_SV13INF           0x40000180  /**< SV情報13                                         */
#define D_RRH_REG_CNT_SV14INF           0x40000184  /**< SV情報14                                         */
#define D_RRH_REG_CNT_SV15INF           0x40000188  /**< SV情報15                                         */
#define D_RRH_REG_CNT_SV16INF           0x4000018C  /**< SV情報16                                         */
#define D_RRH_REG_CNT_SV1CMPM           0x40000190  /**< SV情報1 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV2CMPM           0x40000194  /**< SV情報2 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV3CMPM           0x40000198  /**< SV情報3 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV4CMPM           0x4000019C  /**< SV情報4 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV5CMPM           0x400001A0  /**< SV情報5 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV6CMPM           0x400001A4  /**< SV情報6 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV7CMPM           0x400001A8  /**< SV情報7 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV8CMPM           0x400001AC  /**< SV情報8 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV9CMPM           0x400001B0  /**< SV情報9 変化割り込みマスク                       */
#define D_RRH_REG_CNT_SV10CMPM          0x400001B4  /**< SV情報10 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV11CMPM          0x400001B8  /**< SV情報11 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV12CMPM          0x400001BC  /**< SV情報12 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV13CMPM          0x400001C0  /**< SV情報13 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV14CMPM          0x400001C4  /**< SV情報14 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV15CMPM          0x400001C8  /**< SV情報15 変化割り込みマスク                      */
#define D_RRH_REG_CNT_SV16CMPM          0x400001CC  /**< SV情報16 変化割り込みマスク                      */
#define D_RRH_REG_CNT_STA1              0x40000200  /**< ステータス1表示                                  */
#define D_RRH_REG_CNT_STA1M             0x40000204  /**< ステータス1表示マスク                            */
#define D_RRH_REG_CNT_FCSTA1            0x40000208  /**< STA1擬似アラーム設定                             */
#define D_RRH_REG_CNT_EXPLLALM          0x40000380  /**< ExPLL Statusバタツキ発生数カウント(Edge検出)     */
#define D_RRH_REG_CNT_SFP_ST_CPS        0x40000500
#define D_RRH_REG_CNT_SFP_ST_CPM1       0x40000510
#define D_RRH_REG_CNT_SFP_ST_CPM2       0x40000520
#define D_RRH_REG_CNT_SFP_ST_CPM3       0x40000530
#define D_RRH_REG_CNT_SFP_ST_CPM4       0x40000540
#define D_RRH_REG_CNT_SFP_ST_CPM5       0x40000550
#define D_RRH_REG_CNT_SFP_ST_CPM6       0x40000560
#define D_RRH_REG_CNT_SFP_ST_CPM7       0x40000570
#define D_RRH_REG_CNT_SFP_ST_CPM8       0x40000580
#define D_RRH_REG_CNT_SFP_ST_CPM9       0x40000590
#define D_RRH_REG_CNT_SFP_ST_CPM10      0x400005A0
#define D_RRH_REG_CNT_SFP_ST_CPM11      0x400005B0
#define D_RRH_REG_CNT_SFP_ST_CPM12      0x400005C0
#define D_RRH_REG_CNT_SFP_ST_CPM13      0x400005D0
#define D_RRH_REG_CNT_SFP_ST_CPM14      0x400005E0
#define D_RRH_REG_CNT_SFP_ST_CPM15      0x400005F0
#define D_RRH_REG_CNT_SFP_ST_CPM16      0x40000600
#define D_RRH_REG_CNT_SLVCFG            0x40000600  /**< Configステータス：CRC Error                      */
#define D_RRH_REG_CNT_SLVCFGSET         0x40000604  /**<                                                  */
#define D_RRH_REG_CNT_SYSLED            0x40000800  /**<                                                  */
#define D_RRH_REG_CNT_RSTM              0x40000900  /**< リセットマスク                                   */
#define D_RRH_REG_CNT_RSTWAIT           0x40000904  /**< リセット要因⇒リセット発行までのWait時間設定     */
#define D_RRH_REG_CNT_SVPROT            0x40000940  /**< SV変化保護時間設定                               */
#define D_RRH_REG_CNT_SVIFSLVERR        0x40000980  /**< SVIF エラー表示                                  */
#define D_RRH_REG_CNT_SVIFSLVTO         0x40000984  /**< SVIF Timeout表示                                 */
#define D_RRH_REG_CNT_SVIFSLVTXEN       0x4000098C  /**< SVIF 送信アドレス選択                            */
#define D_RRH_REG_CNT_SVIFSLVTXD4       0x40000990  /**< SVIF送信データ設定(Address 15～12)               */
#define D_RRH_REG_CNT_SVIFSLVTXD3       0x40000994  /**< SVIF送信データ設定(Address 11～8)                */
#define D_RRH_REG_CNT_SVIFSLVTXD2       0x40000998  /**< SVIF送信データ設定(Address 7～4)                 */
#define D_RRH_REG_CNT_SVIFSLVTXD1       0x4000099C  /**< SVIF送信データ設定(Address 3～0)                 */
#define D_RRH_REG_CNT_SVIFSLVRXD4       0x400009A0  /**< SVIF受信データ表示(Address 15～12)               */
#define D_RRH_REG_CNT_SVIFSLVRXD3       0x400009A4  /**< SVIF受信データ表示(Address 11～8)                */
#define D_RRH_REG_CNT_SVIFSLVRXD2       0x400009A8  /**< SVIF受信データ表示(Address 7～4)                 */
#define D_RRH_REG_CNT_SVIFSLVRXD1       0x400009AC  /**< SVIF受信データ表示(Address 3～0)                 */
#define D_RRH_REG_CNT_SVIFMMIERR        0x400009B0  /**< SVIF エラー表示                                  */
#define D_RRH_REG_CNT_SVIFMMITO         0x400009B4  /**< SVIF Timeout表示                                 */
#define D_RRH_REG_CNT_SVIFMMITXEN       0x400009BC  /**< SVIF 送信アドレス選択                            */
#define D_RRH_REG_CNT_SVIFMMITXD4       0x400009C0  /**< SVIF送信データ設定(Address 15～12)               */
#define D_RRH_REG_CNT_SVIFMMITXD3       0x400009C4  /**< SVIF送信データ設定(Address 11～8)                */
#define D_RRH_REG_CNT_SVIFMMITXD2       0x400009C8  /**< SVIF送信データ設定(Address 7～4)                 */
#define D_RRH_REG_CNT_SVIFMMITXD1       0x400009CC  /**< SVIF送信データ設定(Address 3～0)                 */
#define D_RRH_REG_CNT_SVIFMMIRXD4       0x400009D0  /**< SVIF受信データ表示(Address 15～12)               */
#define D_RRH_REG_CNT_SVIFMMIRXD3       0x400009D4  /**< SVIF受信データ表示(Address 11～8)                */
#define D_RRH_REG_CNT_SVIFMMIRXD2       0x400009D8  /**< SVIF受信データ表示(Address 7～4)                 */
#define D_RRH_REG_CNT_SVIFMMIRXD1       0x400009DC  /**< SVIF受信データ表示(Address 3～0)                 */
#define D_RRH_REG_CNT_RSTST             0x40000F40  /**< リセットステータス                               */
#define D_RRH_REG_CNT_SVIFSLVTST        0x40000F60  /**< SVIF擬似エラー                                   */
#define D_RRH_REG_CNT_SVIFMMITST        0x40000F64  /**< SVIF擬似エラー                                   */
#define D_RRH_REG_CNT_DIPSW             0x40000F70  /**< DIPSW状態表示                                    */
#define D_RRH_REG_CNT_IRQDN             0x40000F80  /**<  F2PIRQ 接続設定(割り込み接続)                   */
#define D_RRH_REG_CNT_IRQSTA            0x40000F84  /**< 履歴表示                                         */
#define D_RRH_REG_CNT_TESTEPLLALM       0x40000FB0  /**< ExPLL Status ALMカウントテストレジスタ           */
#define D_RRH_REG_CNT_ELYIRQ1S          0x40000FC0  /**< IRQ1S 1/5、1/100早回し                           */
#define D_RRH_REG_CNT_DBGVER            0x40000FE0  /**< デバッグバージョン                               */
#define D_RRH_REG_CNT_CTTEST            0x40000FF0  /**< TEST用設定                                       */
#define D_RRH_REG_CNT_PORREQ            0x40000FFC  /**< ispPACへのPOR要求                                */
#define D_RRH_REG_CNT_CPSLED            0x40001000  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPSSFPRATE        0x40001004  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM1LED           0x40001010  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM1SFPRATE       0x40001014  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM2LED           0x40001020  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM2SFPRATE       0x40001024  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM3LED           0x40001030  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM3SFPRATE       0x40001034  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM4LED           0x40001040  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM4SFPRATE       0x40001044  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM5LED           0x40001050  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM5SFPRATE       0x40001054  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM6LED           0x40001060  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM6SFPRATE       0x40001064  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM7LED           0x40001070  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM7SFPRATE       0x40001074  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM8LED           0x40001080  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM8SFPRATE       0x40001084  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM9LED           0x40001090  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM9SFPRATE       0x40001094  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM10LED          0x400010A0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM10SFPRATE      0x400010A4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM11LED          0x400010B0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM11SFPRATE      0x400010B4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM12LED          0x400010C0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM12SFPRATE      0x400010C4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM13LED          0x400010D0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM13SFPRATE      0x400010D4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM14LED          0x400010E0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM14SFPRATE      0x400010E4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM15LED          0x400010F0  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM15SFPRATE      0x400010F4  /**< SFPRATE設定                                      */
#define D_RRH_REG_CNT_CPM16LED          0x40001100  /**< SV転送情報(⇔MMI-CPLD)                           */
#define D_RRH_REG_CNT_CPM16SFPRATE      0x40001104  /**< SFPRATE設定                                      */

#define M_RRH_REG_CNT_SFP_ST(cprino) (UINT)(D_RRH_REG_CNT_SFP_ST_CPS + (0x10*(cprino)))

#define D_RRH_REG_CNTS_SFPLDODIS        0x80000020  /**< SFP LDO Disable設定                              */
#define D_RRH_REG_CNTS_RAMSLP           0x80000024  /**< BRAM Sleep設定                                   */
#define D_RRH_REG_CNTS_CPSSERRST        0x80000040  /**< 各種Reset制御                                    */
#define D_RRH_REG_CNTS_CPMSERRST        0x80000044  /**< 各種SerDes Reset制御                             */
#define D_RRH_REG_CNTS_CPSSERPWR        0x80000048  /**< 各種SerDes PowerOn制御(Level)                    */
#define D_RRH_REG_CNTS_CPMSERPWR        0x8000004C  /**< 各種SerDes PowerOn制御(Level)                    */
#define D_RRH_REG_CNTS_L1RST2MST        0x80000050  /**< Master port(x16)へのL1 Inband Reset一斉送信      */
#define D_RRH_REG_CNTS_BITRATE          0x80000070  /**< CPRI Lineビットレート                            */
#define D_RRH_REG_CNTS_IRQCPSTA         0x800000F0  /**< CPRI State変化割込み状態表示                     */
#define D_RRH_REG_CNTS_IRQCPM12         0x80000110  /**< CPRI Master 1/2系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPM12M        0x80000114  /**< CPRI Master 3/4系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPM34         0x80000118  /**< CPRI Master 3/4系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPM34M        0x8000011C  /**< CPRI Master 3/4系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPM56         0x80000120  /**< CPRI Master 5/6系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPM56M        0x80000124  /**< CPRI Master 5/6系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPM78         0x80000128  /**< CPRI Master 7/8系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPM78M        0x8000012C  /**< CPRI Master 7/8系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPM9A         0x80000130  /**< CPRI Master 9/A系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPM9AM        0x80000134  /**< CPRI Master 9/A系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPMBC         0x80000138  /**< CPRI Master B/C系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPMBCM        0x8000013C  /**< CPRI Master B/C系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPMDE         0x80000140  /**< CPRI Master D/E系代表割込み                      */
#define D_RRH_REG_CNTS_IRQCPMDEM        0x80000144  /**< CPRI Master D/E系代表割込みマスク                */
#define D_RRH_REG_CNTS_IRQCPMF10        0x80000148  /**< CPRI Master F/10系代表割込み                     */
#define D_RRH_REG_CNTS_IRQCPMF10M       0x8000014C  /**< CPRI Master F/10系代表割込みマスク               */
#define D_RRH_REG_CNTS_IRQSFPMNT        0x80000150  /**< SFP Mount 変化割込み                             */
#define D_RRH_REG_CNTS_IRQSFPMNTM       0x80000154  /**< SFP Mount 変化割込みマスク                       */
#define D_RRH_REG_CNTS_IRQLRXST			0x80000180	/**< LAPB受信割込み									  */
#define D_RRH_REG_CNTS_IRQLRXSTM		0x80000184	/**< LAPB受信割込みマスク							  */
#define D_RRH_REG_CNTS_IRQLTXST			0x80000190	/**< LAPB送信完了割込み								  */
#define D_RRH_REG_CNTS_IRQLTXSTM		0x80000194	/**< LAPB送信完了割込みマスク						  */

#define D_RRH_REG_CNTS_FCSTA1           0x80000208  /**< STA1擬似('1')設定                                */
#define D_RRH_REG_CNTS_CDRST            0x80000280  /**< CPRI PHY CDR状態表示                             */
#define D_RRH_REG_CNTS_CPDLSWSD         0x80000300  /**<                                                  */
#define D_RRH_REG_CNTS_CPSDLSDM         0x80000304  /**<                                                  */
#define D_RRH_REG_CNTS_CPULSWSD         0x80000308  /**<                                                  */
#define D_RRH_REG_CNTS_CPMULSDM         0x8000030C  /**<                                                  */
#define D_RRH_REG_CNTS_CPL1RSTSTA       0x80000400  /**< CPRI Slave/Master L1 Rst状態表示                 */
#define D_RRH_REG_CNTS_CPSSFPSTA        0x80000500  /**< CPRI Slave port SFP状態表示レジスタ              */
#define D_RRH_REG_CNTS_CPSSFPSTAM       0x80000504  /**< CPRI Slave port SFP状態表示レジスタマスク        */
#define D_RRH_REG_CNTS_CPSSFPSTAF       0x80000508  /**< CPRI Slave port SFP状態表示レジスタ擬似          */
#define D_RRH_REG_CNTS_CPM1SFPSTA       0x80000510  /**< CPRI Master1 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM1SFPSTAM      0x80000514  /**< CPRI Master1 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM1SFPSTAF      0x80000518  /**< CPRI Master1 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM2SFPSTA       0x80000520  /**< CPRI Master2 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM2SFPSTAM      0x80000524  /**< CPRI Master2 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM2SFPSTAF      0x80000528  /**< CPRI Master2 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM3SFPSTA       0x80000530  /**< CPRI Master3 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM3SFPSTAM      0x80000534  /**< CPRI Master3 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM3SFPSTAF      0x80000538  /**< CPRI Master3 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM4SFPSTA       0x80000540  /**< CPRI Master4 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM4SFPSTAM      0x80000544  /**< CPRI Master4 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM4SFPSTAF      0x80000548  /**< CPRI Master4 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM5SFPSTA       0x80000550  /**< CPRI Master5 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM5SFPSTAM      0x80000554  /**< CPRI Master5 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM5SFPSTAF      0x80000558  /**< CPRI Master5 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM6SFPSTA       0x80000560  /**< CPRI Master6 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM6SFPSTAM      0x80000564  /**< CPRI Master6 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM6SFPSTAF      0x80000568  /**< CPRI Master6 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM7SFPSTA       0x80000570  /**< CPRI Master7 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM7SFPSTAM      0x80000574  /**< CPRI Master7 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM7SFPSTAF      0x80000578  /**< CPRI Master7 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM8SFPSTA       0x80000580  /**< CPRI Master8 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM8SFPSTAM      0x80000584  /**< CPRI Master8 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM8SFPSTAF      0x80000588  /**< CPRI Master8 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM9SFPSTA       0x80000590  /**< CPRI Master9 port SFP状態表示レジスタ            */
#define D_RRH_REG_CNTS_CPM9SFPSTAM      0x80000594  /**< CPRI Master9 port SFP状態表示レジスタマスク      */
#define D_RRH_REG_CNTS_CPM9SFPSTAF      0x80000598  /**< CPRI Master9 port SFP状態表示レジスタ擬似        */
#define D_RRH_REG_CNTS_CPM10SFPSTA      0x800005A0  /**< CPRI Master10 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM10SFPSTAM     0x800005A4  /**< CPRI Master10 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM10SFPSTAF     0x800005A8  /**< CPRI Master10 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM11SFPSTA      0x800005B0  /**< CPRI Master11 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM11SFPSTAM     0x800005B4  /**< CPRI Master11 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM11SFPSTAF     0x800005B8  /**< CPRI Master11 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM12SFPSTA      0x800005C0  /**< CPRI Master12 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM12SFPSTAM     0x800005C4  /**< CPRI Master12 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM12SFPSTAF     0x800005C8  /**< CPRI Master12 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM13SFPSTA      0x800005D0  /**< CPRI Master13 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM13SFPSTAM     0x800005D4  /**< CPRI Master13 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM13SFPSTAF     0x800005D8  /**< CPRI Master13 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM14SFPSTA      0x800005E0  /**< CPRI Master14 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM14SFPSTAM     0x800005E4  /**< CPRI Master14 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM14SFPSTAF     0x800005E8  /**< CPRI Master14 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM15SFPSTA      0x800005F0  /**< CPRI Master15 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM15SFPSTAM     0x800005F4  /**< CPRI Master15 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM15SFPSTAF     0x800005F8  /**< CPRI Master15 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_CPM16SFPSTA      0x80000600  /**< CPRI Master16 port SFP状態表示レジスタ           */
#define D_RRH_REG_CNTS_CPM16SFPSTAM     0x80000604  /**< CPRI Master16 port SFP状態表示レジスタマスク     */
#define D_RRH_REG_CNTS_CPM16SFPSTAF     0x80000608  /**< CPRI Master16 port SFP状態表示レジスタ擬似       */
#define D_RRH_REG_CNTS_TMONSEL          0x80000930  /**< デバグ用タイミングモニタ出力選択                 */
#define D_RRH_REG_CNTS_SYS10MEN         0x80000934  /**< 10MHz Clock出力制御                              */
#define D_RRH_REG_CNTS_SYS10MULCK       0x80000938  /**< 10MHz Clock PLL UnLockステータス表示             */
#define D_RRH_REG_CNTS_SVIFERR          0x80000980  /**< SVIF エラー表示                                  */
#define D_RRH_REG_CNTS_SVIFTO           0x80000984  /**< SVIF Timeout表示                                 */
#define D_RRH_REG_CNTS_SVIFTXEN         0x8000098C  /**< SVIF 送信アドレス選択                            */
#define D_RRH_REG_CNTS_SVIFTXD4         0x80000990  /**< SVIF送信データ設定(Address 15～12)               */
#define D_RRH_REG_CNTS_SVIFTXD3         0x80000994  /**< SVIF送信データ設定(Address 11～8)                */
#define D_RRH_REG_CNTS_SVIFTXD2         0x80000998  /**< SVIF送信データ設定(Address 7～4)                 */
#define D_RRH_REG_CNTS_SVIFTXD1         0x8000099C  /**< SVIF送信データ設定(Address 3～0)                 */
#define D_RRH_REG_CNTS_SVIFRXD4         0x800009A0  /**< SVIF受信データ表示(Address 15～12)               */
#define D_RRH_REG_CNTS_SVIFRXD3         0x800009A4  /**< SVIF受信データ表示(Address 11～8)                */
#define D_RRH_REG_CNTS_SVIFRXD2         0x800009A8  /**< SVIF受信データ表示(Address 7～4)                 */
#define D_RRH_REG_CNTS_SVIFRXD1         0x800009AC  /**< SVIF受信データ表示(Address 3～0)                 */
#define D_RRH_REG_CNTS_CPNOTDONE        0x80000F00  /**< CPRI SerDes Not DONE                             */
#define D_RRH_REG_CNTS_CLKCNTTST        0x80000F04  /**< EXPLL関連デバッグレジスタ                        */
#define D_RRH_REG_CNTS_RDYCHK           0x80000F10  /**< GP1系Bus Error表示(Ready check) ※Debug          */
#define D_RRH_REG_CNTS_SVIFTST          0x80000F60  /**< SVIF擬似エラー                                   */
#define D_RRH_REG_CNTS_DIPSW            0x80000F70  /**< DIPSW状態表示                                    */
#define D_RRH_REG_CNTS_CPSLED           0x80000FA0  /**< CPRI Slave LED制御                               */
#define D_RRH_REG_CNTS_CPMLED           0x80000FA4  /**< CPRI Master LED制御                              */
#define D_RRH_REG_CNTS_DBGVER           0x80000FE0  /**< デバッグバージョン                               */
#define D_RRH_REG_CNTS_CTTEST           0x80000FF0  /**< TEST用設定                                       */
#define D_RRH_REG_CNTS_REGCLR           0x80000FF4  /**< TEST用設定2                                      */
#define D_RRH_REG_CNTS_FCRE1            0x81001200  /**< CPM Slave(擬似RE)Mode設定1                       */
#define D_RRH_REG_CNTS_FCRE2            0x81001A00  /**< CPM Slave(擬似RE)Mode設定2                       */
#define D_RRH_REG_CNTS_FCRE3            0x81002200  /**< CPM Slave(擬似RE)Mode設定3                       */
#define D_RRH_REG_CNTS_FCRE4            0x81002A00  /**< CPM Slave(擬似RE)Mode設定4                       */
#define D_RRH_REG_CNTS_FCRE5            0x81003200  /**< CPM Slave(擬似RE)Mode設定5                       */
#define D_RRH_REG_CNTS_FCRE6            0x81003A00  /**< CPM Slave(擬似RE)Mode設定6                       */
#define D_RRH_REG_CNTS_FCRE7            0x81004200  /**< CPM Slave(擬似RE)Mode設定7                       */
#define D_RRH_REG_CNTS_FCRE8            0x81004A00  /**< CPM Slave(擬似RE)Mode設定8                       */
#define D_RRH_REG_CNTS_FCRE9            0x81005200  /**< CPM Slave(擬似RE)Mode設定9                       */
#define D_RRH_REG_CNTS_FCRE10           0x81005A00  /**< CPM Slave(擬似RE)Mode設定10                      */
#define D_RRH_REG_CNTS_FCRE11           0x81006200  /**< CPM Slave(擬似RE)Mode設定11                      */
#define D_RRH_REG_CNTS_FCRE12           0x81006A00  /**< CPM Slave(擬似RE)Mode設定12                      */
#define D_RRH_REG_CNTS_FCRE13           0x81007200  /**< CPM Slave(擬似RE)Mode設定13                      */
#define D_RRH_REG_CNTS_FCRE14           0x81007A00  /**< CPM Slave(擬似RE)Mode設定14                      */
#define D_RRH_REG_CNTS_FCRE15           0x81008200  /**< CPM Slave(擬似RE)Mode設定15                      */
#define D_RRH_REG_CNTS_FCRE16           0x81008A00  /**< CPM Slave(擬似RE)Mode設定16                      */

#define M_RRH_REG_CNTS_CPSFPSTA(cprino)	(UINT)((D_RRH_CPRINO_REC==cprino) ? D_RRH_REG_CNTS_CPSSFPSTA : (D_RRH_REG_CNTS_CPM1SFPSTA+(cprino-D_RRH_CPRINO_RE_MIN)*0x10))
#define M_RRH_REG_CNTS_CPSFPSTAM(cprino)	(UINT)((D_RRH_CPRINO_REC==cprino) ? D_RRH_REG_CNTS_CPSSFPSTAM : (D_RRH_REG_CNTS_CPM1SFPSTAM+(cprino-D_RRH_CPRINO_RE_MIN)*0x10))
#define M_RRH_REG_CNTS_IRQCPS(cprino)	(UINT)((D_RRH_CPRINO_REC==cprino) ? D_RRH_REG_CNTS_IRQCPS : (D_RRH_REG_CNTS_IRQCPM12+(((cprino)-1)/2)*8))
#define M_RRH_REG_CNTS_IRQCPSM(cprino)	(UINT)((D_RRH_CPRINO_REC==cprino) ? D_RRH_REG_CNTS_IRQCPSM : (D_RRH_REG_CNTS_IRQCPM12M+(((cprino)-1)/2)*8))
#define M_RRH_REG_CNTS_FCRE(cprino)	(UINT)(D_RRH_REG_CNTS_FCRE1+(cprino-D_RRH_CPRINO_RE_MIN)*0x800)

#define M_RRH_REG_CNTS_VALUE(cprino,value)	(UINT)(((D_RRH_CPRINO_REC==cprino) ? value : ((cprino-D_RRH_CPRINO_RE_MIN)&1) ? value >> 16 : value) & 0x0000FFFF)
#define M_RRH_REG_CNTS_MAKEBIT(cprino,value)	(UINT)((D_RRH_CPRINO_REC==cprino) ? value : ((cprino-D_RRH_CPRINO_RE_MIN)&1) ? value << 16 : value)

/* CPRI LINK LED Address	*/
#define M_RRH_REG_CNT_CPSLED(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) ? D_RRH_REG_CNT_CPSLED : (D_RRH_REG_CNT_CPM1LED+(0x10*(cprino-1))))

#define D_RRH_REG_CNTS_CPSFPSTA_OPTMNT			0x00000001


/* FHM LED1(RED)	*/
#define D_RRH_CNT_FHM_LEDSYSR_OFF				0x00000000		// B[1:0]=00 消灯
#define D_RRH_CNT_FHM_LEDSYSR_NOUSE				0x00000001		// B[1:0]=01 未使用
#define D_RRH_CNT_FHM_LEDSYSR_CYCL				0x00000002		// B[1:0]=10 0.5S間隔点滅
#define D_RRH_CNT_FHM_LEDSYSR_ON				0x00000003		// B[1:0]=11 点灯
#define D_RRH_CNT_FHM_LEDSYSR_MASK				( D_RRH_CNT_FHM_LEDSYSR_OFF | D_RRH_CNT_FHM_LEDSYSR_NOUSE | D_RRH_CNT_FHM_LEDSYSR_CYCL | D_RRH_CNT_FHM_LEDSYSR_ON )

/* FHM LED2(GREEN)	*/
#define D_RRH_CNT_FHM_LEDSYSG1_OFF				0x00000000		// B[3:2]=00 消灯
#define D_RRH_CNT_FHM_LEDSYSG1_NOUSE			0x00000004		// B[3:2]=01 未使用
#define D_RRH_CNT_FHM_LEDSYSG1_CYCL				0x00000008		// B[3:2]=10 0.5S間隔点滅
#define D_RRH_CNT_FHM_LEDSYSG1_ON				0x0000000C		// B[3:2]=11 点灯
#define D_RRH_CNT_FHM_LEDSYSG1_MASK				( D_RRH_CNT_FHM_LEDSYSG1_OFF | D_RRH_CNT_FHM_LEDSYSG1_NOUSE | D_RRH_CNT_FHM_LEDSYSG1_CYCL | D_RRH_CNT_FHM_LEDSYSG1_ON )

/* FHM LED3(ORANGE)	*/
#define D_RRH_CNT_FHM_LEDSYSO_OFF				0x00000000		// B[9:8]=00 消灯
#define D_RRH_CNT_FHM_LEDSYSO_NOUSE				0x00000100		// B[9:8]=01 未使用
#define D_RRH_CNT_FHM_LEDSYSO_CYCL				0x00000200		// B[9:8]=10 0.5S間隔点滅
#define D_RRH_CNT_FHM_LEDSYSO_ON				0x00000300		// B[9:8]=11 点灯
#define D_RRH_CNT_FHM_LEDSYSO_MASK				( D_RRH_CNT_FHM_LEDSYSO_OFF | D_RRH_CNT_FHM_LEDSYSO_NOUSE | D_RRH_CNT_FHM_LEDSYSO_CYCL | D_RRH_CNT_FHM_LEDSYSO_ON )


/* FHM CPRI LINK LED(DWNLED) */
#define D_RRH_CNT_FHM_DWNLED_OFF				0x00000000		// B[3:2]=00 消灯
#define D_RRH_CNT_FHM_DWNLED_NOUSE				0x00000004		// B[3:2]=01 未使用
#define D_RRH_CNT_FHM_DWNLED_CYCL				0x00000008		// B[3:2]=10 点滅
#define D_RRH_CNT_FHM_DWNLED_ON					0x0000000C		// B[3:2]=11 点灯
#define D_RRH_CNT_FHM_DWNLED_MASK				( D_RRH_CNT_FHM_DWNLED_OFF | D_RRH_CNT_FHM_DWNLED_NOUSE | D_RRH_CNT_FHM_DWNLED_CYCL | D_RRH_CNT_FHM_DWNLED_ON)
 	
/* FHM CPRI LINK LED(ACTLED) */
#define D_RRH_CNT_FHM_ACTLED_OFF				0x00000000		// B[1:0]=00 消灯
#define D_RRH_CNT_FHM_ACTLED_NOUSE				0x00000001		// B[1:0]=01 未使用
#define D_RRH_CNT_FHM_ACTLED_CYCL				0x00000002		// B[1:0]=10 点滅
#define D_RRH_CNT_FHM_ACTLED_ON					0x00000003		// B[1:0]=11 点灯
#define D_RRH_CNT_FHM_ACTLED_MASK				( D_RRH_CNT_FHM_ACTLED_OFF | D_RRH_CNT_FHM_ACTLED_NOUSE | D_RRH_CNT_FHM_ACTLED_CYCL | D_RRH_CNT_FHM_ACTLED_ON)


/* SV */
#define D_RRH_REG_CNT_SV1INF_ERR                    0x00000001
#define D_RRH_REG_CNT_SV1INF_ALM                    0x00000002
#define D_RRH_REG_CNT_SV1INF_USE                    0x00000004
#define D_RRH_REG_CNT_SV1INF_ACT                    0x00002000
#define D_RRH_REG_CNT_SV1INF_NOINS                  0x00004000
#define D_RRH_REG_CNT_SV1INF_INS                    0x00008000
#define D_RRH_REG_CNT_SV1INF_S3G_USE_01             0x00010000
#define D_RRH_REG_CNT_SV1INF_S3G_USE_02             0x00020000
#define D_RRH_REG_CNT_SV1INF_S3G_USE_03             0x00040000
#define D_RRH_REG_CNT_SV1INF_S3G_USE_04             0x00080000
#define D_RRH_REG_CNT_SV1INF_3G_USE_01              0x00100000
#define D_RRH_REG_CNT_SV1INF_3G_USE_02              0x00200000
#define D_RRH_REG_CNT_SV1INF_3G_USE_03              0x00400000
#define D_RRH_REG_CNT_SV1INF_3G_USE_04              0x00800000
#define D_RRH_REG_CNT_SV1INF_PLL_ALM                0x01000000
#define D_RRH_REG_CNT_SV1INF_CPRI_ALM               0x02000000
#define D_RRH_REG_CNT_SV1INF_COM_ALM                0x04000000
#define D_RRH_REG_CNT_SV1INF_SOFT_ALM               0x20000000
#define D_RRH_REG_CNT_SV1INF_SOFT_ALM2              0x40000000

#define D_RRH_REG_CNT_SV2INF_CPRI_ALM_01            0x00000001
#define M_RRH_REG_CNT_CPRI_ALM(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV1INF_CPRI_ALM : (D_RRH_REG_CNT_SV2INF_CPRI_ALM_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV7INF_3G_FREQ_CMPLX_ALM      0x00020000
#define D_RRH_REG_CNT_SV7INF_3G_CARR_NUM_ALM        0x00080000
#define D_RRH_REG_CNT_SV7INF_3G_UP_RESRC_NUM_ALM    0x00100000
#define D_RRH_REG_CNT_SV7INF_3G_DWN_RESRC_NUM_ALM   0x00200000
#define D_RRH_REG_CNT_SV7INF_S3G_SHARETYPE_ALM      0x01000000
#define D_RRH_REG_CNT_SV7INF_S3G_FREQ_CMPLX_ALM     0x02000000
#define D_RRH_REG_CNT_SV7INF_S3G_BAND_WIDTH_ALM     0x04000000
#define D_RRH_REG_CNT_SV7INF_S3G_CARR_NUMALM        0x08000000
#define D_RRH_REG_CNT_SV7INF_RE_FLASH_01            0x10000000
#define D_RRH_REG_CNT_SV7INF_RE_FLASH_02            0x20000000
#define D_RRH_REG_CNT_SV7INF_RE_FLASH_03            0x40000000
#define D_RRH_REG_CNT_SV7INF_RE_FLASH_04            0x80000000

#define M_RRH_REG_CNT_SV7INF_RE_FLASH(flashIdx) (D_RRH_REG_CNT_SV7INF_RE_FLASH_01 << (flashIdx))
#define D_RRH_REG_CNT_SV7INF_BIT_REFLASH            0xF0000000
#define D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH     0x0F3A0000
#define D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH_3G  0x003A0000
#define D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH_S3G 0x0F000000

#define D_RRH_REG_CNT_SV8INF_RE01_OPT_STS           0x00000001
#define M_RRH_REG_CNT_OPT_STS(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV8INF_RE01_OPT_STS << (2 * (cprino - D_RRH_CPRINO_RE_MIN))))

#define D_RRH_REG_CNT_SV8INF_RE01_WAVE_STS          0x00000002
#define M_RRH_REG_CNT_WAVE_STS(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV8INF_RE01_WAVE_STS << (2 * (cprino - D_RRH_CPRINO_RE_MIN))))

#define D_RRH_REG_CNT_SV9INF_L1ERR                  0x00000001
#define D_RRH_REG_CNT_SV9INF_L2ERR                  0x00000002
#define D_RRH_REG_CNT_SV9INF_3G_L3ERR               0x00000004
#define D_RRH_REG_CNT_SV9INF_S3G_L3ERR              0x00000008
#define D_RRH_REG_CNT_SV9INF_SFP_ERR1               0x00000020
#define D_RRH_REG_CNT_SV9INF_SFP_ERR2               0x00000040
#define D_RRH_REG_CNT_SV9INF_DELAY_ERR              0x00000100
#define D_RRH_REG_CNT_SV9INF_DELAY_ERR_S3G          0x00000200
#define D_RRH_REG_CNT_SV9INF_SYSPARA_DL_ERR         0x00001000
#define D_RRH_REG_CNT_SV9INF_FIRM_DL_ERR            0x00002000

#define D_RRH_REG_CNT_SV9INF_L1_ERR_01              0x00010000
#define M_RRH_REG_CNT_L1_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_L1ERR : (D_RRH_REG_CNT_SV9INF_L1_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV10INF_L2_ERR_01             0x00000001
#define M_RRH_REG_CNT_L2_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_L2ERR : (D_RRH_REG_CNT_SV10INF_L2_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))
#define M_RRH_REG_CNT_L2_ERR_ADR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF : D_RRH_REG_CNT_SV10INF)

#define D_RRH_REG_CNT_SV10INF_3G_L3_ERR_01          0x00010000
#define M_RRH_REG_CNT_3G_L3_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_3G_L3ERR : (D_RRH_REG_CNT_SV10INF_3G_L3_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))
#define M_RRH_REG_CNT_3G_L3_ERR_ADR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF : D_RRH_REG_CNT_SV10INF)

#define D_RRH_REG_CNT_SV11INF_S3G_L3_ERR_01         0x00000001
#define M_RRH_REG_CNT_S3G_L3_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_S3G_L3ERR : (D_RRH_REG_CNT_SV11INF_S3G_L3_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))
#define M_RRH_REG_CNT_S3G_L3_ERR_ADR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF : D_RRH_REG_CNT_SV11INF)

#define D_RRH_REG_CNT_SV11INF_SFP_ERR1_01           0x00010000
#define M_RRH_REG_CNT_SFP_ERR1(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_SFP_ERR1 : (D_RRH_REG_CNT_SV11INF_SFP_ERR1_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV12INF_SFP_ERR2_01           0x00000001
#define M_RRH_REG_CNT_SFP_ERR2(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_SFP_ERR2 : (D_RRH_REG_CNT_SV12INF_SFP_ERR2_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV12INF_SFP_ERR3_01           0x00010000
#define M_RRH_REG_CNT_SFP_ERR3(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV12INF_SFP_ERR3_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV13INF_DELAY_ERR1_01         0x00000001
#define M_RRH_REG_CNT_DELAY_ERR1(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? D_RRH_REG_CNT_SV9INF_DELAY_ERR : (D_RRH_REG_CNT_SV13INF_DELAY_ERR1_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV13INF_DELAY_ERR2_01         0x00010000
#define M_RRH_REG_CNT_DELAY_ERR2(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV13INF_DELAY_ERR2_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV14INF_STARTUP_ERR1_01       0x00000001
#define M_RRH_REG_CNT_STARTUP_ERR1(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV14INF_STARTUP_ERR1_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV14INF_DL_NG_ERR_01          0x00010000
#define M_RRH_REG_CNT_DL_NG_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV14INF_DL_NG_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV15INF_3G_CARRRESET_ERR_01   0x00000001
#define M_RRH_REG_CNT_3G_CARRRESET_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV15INF_3G_CARRRESET_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV15INF_S3G_CARRRESET_ERR_01  0x00010000
#define M_RRH_REG_CNT_S3G_CARRRESET_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV15INF_S3G_CARRRESET_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV16INF_S3G_RESTARTUP_ERR_01  0x00000001
#define M_RRH_REG_CNT_S3G_RESTARTUP_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV16INF_S3G_RESTARTUP_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

//16B
#define D_RRH_REG_CNT_SV12INF_SLOT_ERR_01          0x00010000
#define M_RRH_REG_CNT_SLOT_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV12INF_SLOT_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))
//16B

#define D_RRH_REG_CNT_SV5INF_3G_FAILSAFE_ERR_01    0x00000001
#define M_RRH_REG_CNT_3G_FAILSAFE_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV5INF_3G_FAILSAFE_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

#define D_RRH_REG_CNT_SV5INF_S3G_FAILSAFE_ERR_01    0x00010000
#define M_RRH_REG_CNT_S3G_FAILSAFE_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (D_RRH_REG_CNT_SV5INF_S3G_FAILSAFE_ERR_01 << (cprino - D_RRH_CPRINO_RE_MIN)))

/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#endif
/* @} */
