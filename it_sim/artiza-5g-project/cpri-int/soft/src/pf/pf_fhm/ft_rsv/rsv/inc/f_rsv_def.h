/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_def.h
 *  @brief  RE supervision defination
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/11/19 QNET)Kabasima M-S3G-eNBPF-02864:[昳幙夵慞]僴乕僪ALM忬曄庴怣帪偺張棟岆傝
 *  @date   2009/12/27 QNET)Kabasima MOD CR-xxxxx-xxx[DCM岦偗]LRE3G_LTE嫟梡壔
 *  @date   2010/08/03 CMS)Yorozuya  CR-00014-000 : 僴僜V2.19-02斉
 *  @date   2011/10/11 FFCS)Linlj  modify for 徣揹椡懳墳
 *  @date   2015/06/17 ALP)Murakami TDD-ZYNQ対応
 *  @date   2015/06/23 ALP)Ueda TDD-ZYNQ対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

#ifndef		M_RS_DEF
#define		M_RS_DEF

/** @addtogroup RRH_PF_RSV
* @{ */

/** @name RSV task state judge defination */
/* @{ */

#define D_RSV_TSK_STATE_OK		0
#define D_RSV_TSK_STATE_NG		1
/* @} */

/** @name RSV Task state define */
/* @{ */

/* ======================================================================== */
/* RSV Task state define 				*/
/* ======================================================================== */
#define D_RSV_TSK_STATE_INI		0
#define D_RSV_TSK_STATE_ALLTSK_STA_WAIT		1
#define D_RSV_TSK_STATE_USE		2					
/* @} */

/** @name register bit  define */
/* @{ */

#define D_RSV_ACT_BIT				0x2000
#define D_RSV_NOINS_BIT			    0x4000
#define D_RSV_INS_BIT				0x8000
#define D_RSV_SOFTALM_BIT			0x2000
#define D_RSV_SOFTALM2_BIT          0x4000
#define D_RSV_RESOURCE_ALM_BIT      0xFF3A
#define D_RSV_MAJALM_PLL_BIT		0x0100
#define D_RSV_MAJALM_CPRI_BIT		0x0200
#define D_RSV_MAJALM_APDCOM_BIT		0x0400

#define D_RSV_ERR_BIT				0x0301
#define D_RSV_ACT_BIT_ON			0x2000		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_ACT_BIT_OFF			0xC607		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_USE_BIT_ON			0x0004		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_USE_BIT_OFF			0xE603		/* USE bit set OFF value in SV A0/A1 */
#define D_RSV_ALM_BIT_ON			0x0002		/* ALM bit set ON value in SV A0/A1 */
#define D_RSV_ALM_BIT_OFF			0xE605		/* ALM bit set OFF value in SV A0/A1 */
#define D_RSV_ERR_BIT_ON			0x0001		/* ERR bit set ON value in SV A0/A1 */
#define D_RSV_ERR_BIT_OFF			0xE606		/* ERR bit set OFF value in SV A0/A1 */

#define D_RSV_PLLALM_BIT			0x0001
#define D_RSV_CPRIALM_BIT			0x0002
#define D_RSV_COM2ALM_BIT			0x1D00
/* @} */


/** @name register mask  define */
/* @{ */

#define D_RSV_MSK_TRA0			0xE607			
#define D_RSV_MSK_TRA1			0xFFFF			
#define D_RSV_MSK_TRA2			0x183F			
#define D_RSV_MSK_TRA3			0xFFFF			
#define D_RSV_MSK_TRA4			0xFFFF			
#define D_RSV_MSK_TRA5			0x3037			
#define D_RSV_MSK_TRA6			0x007C			
#define D_RSV_MSK_TRA7			0x00FF			
#define D_RSV_MSK_TRA8			0x0007			
#define D_RSV_MSK_TRA9			0x005D			
#define D_RSV_MSK_TRA10			0x7DF0			
#define D_RSV_MSK_TRA11			0x7DF0			
#define D_RSV_MSK_TRA12			0x7DF0			
#define D_RSV_MSK_TRA13			0x7DF0			

/** @name RSV log define */
/* @{ */

#define D_RSV_LOG_MAX			160			
#define D_RSV_LOG_SV1_SIZE		16			
#define D_RSV_LOG_SV2_SIZE		384			
/* @} */

/** @name table max index define */
/* @{ */

#define D_RSV_SVTBL_NUM			32			
#define D_RSV_PLLALM_NUM		2			/* minor alarm factor number of PLL ALARM */
#define D_RSV_CPRIALM_NUM		2			/* minor alarm factor number of CPRI ALARM */
#define D_RSV_APDCOMALM_NUM	    5			/* minor alarm factor number of APD COM ALARM */
#define D_RSV_APDALM_NUM		7			/* minor alarm factor number of APD0/APD1 ALARM */
/* @} */

/** @name Alarm detecting number define */
/* @{ */
#define D_RSV_ALM_NUM			4
/* @} */

/** @name register type define */
/* @{ */
#define D_RSV_REGTYPE_CPRI		1
#define D_RSV_REGTYPE_CPLD		2
/* @} */

/** @name alarm detecting cycle define */
/* @{ */
#define D_RSV_ALMCYCL_100MS		1		/* 100ms 	*/
#define D_RSV_ALMCYCL_200MS		2		/* 200ms 	*/
#define D_RSV_ALMCYCL_1S		10		/* 1 second 	*/
#define D_RSV_ALMCYCL_3S		30		/* 3 second 	*/
/* @} */

/** @name alarm detecting bitmap define */
/* @{ */
#define D_RSV_ALMBIT_PLLNG		0x00000100		/* PLL UNLOCK 				*/
#define D_RSV_ALMBIT_FPGANG1	0x00000001		/* FPGA-PLL-UNLOCK1			*/
#define D_RSV_ALMBIT_FPGANG2	0x000000C0		/* FPGA-PLL-UNLOCK2			*/
#define D_RSV_ALMBIT_FPGANG3	0x00000100		/* FPGA-PLL-UNLOCK3			*/
#define D_RSV_ALMBIT_FPGAIFNG2  0x20000000		/* FPGA-IF ALM2				*/
#define D_RSV_ALMBIT_CPLDIF1	0x10000000		/* CPLD IF ALM_1			*/
/* @} */

/** @name alarm triggering count define */
/* @{ */
#define D_RSV_ALMCNT_PLLNG		30
#define D_RSV_ALMCNT_FPGANG1	6
#define D_RSV_ALMCNT_FPGANG2    6
#define D_RSV_ALMCNT_FPGANG3	6
#define D_RSV_ALMCNT_FPGAIFNG2	3
#define D_RSV_ALMCNT_CPLDIF1	3
/* @} */

#endif	/* M_RS_DEF */
/** @} */
