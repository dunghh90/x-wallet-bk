/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_def.h
 *  @brief  5GDU-LLS supervision defination
 *  @date   2018/08/21 KCN)Kamimoto create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-2019
 */
/*********************************************************************************/

/* デファイン定義 */
#ifndef		F_EQS_DEF
#define		F_EQS_DEF

/** @addtogroup RRH_PF_EQS
* @{ */

/** @name EQS task state judge defination */
/* @{ */

/* ======================================================================== */
/* タスク状態定義    														*/
/* ======================================================================== */
#define D_RSV_TSK_STATE_OK		0			/* 僞蜒怜ル蠢ｬ懺惓蠢｢				*/
#define D_RSV_TSK_STATE_NG		1			/* 僞蜒怜ル蠢ｬ懺蝣主ｿ｢				*/
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

/** @name RE state define */
/* @{ */

/* ======================================================================== */
/* 5GDU-LLS状態定義											*/
/* ======================================================================== */
#define D_RSV_RESTATE_IDLE			0			/* 5GDU-LLSアイドル状態			*/
#define D_RSV_RESTATE_OPERATE		1			/* 5GDU-LLS運転状態				*/
/* @} */

/** @name TRX set/release define */
/* @{ */

/* ======================================================================== */
/* TRX設定/解放定義    														*/
/* ======================================================================== */
#define D_RSV_TRX_SET				1			/* TRX設定 					*/
#define D_RSV_TRX_RLS				0			/* TRX解放					*/
/* @} */

/** @name ON/OFF setting define */
/* @{ */

/* ======================================================================== */
/* ON/OFF設定定義															*/
/* ======================================================================== */
#define D_RSV_SET_OFF				0			/* OFF設定						*/
#define D_RSV_SET_ON				1			/* ON設定						*/
/* @} */


/** @name card state define */
/* @{ */

/* ======================================================================== */
/* 蜒・h蜒ｪ蠢ｬ懺																*/
/* ======================================================================== */
#define	D_RSV_NOINS					0			/* 枹蟷壽・						*/
#define	D_RSV_INS					1			/* 蟷壽・							*/
#define	D_RSV_ACT					3			/* ACT							*/
#define	D_RSV_USE					4			/* USE							*/
#define	D_RSV_ALM					5			/* ALM							*/
#define	D_RSV_ERR					6			/* ERR							*/
#define	D_RSV_ERROFF				7			/* ERR螟晏ｽ贀						*/
#define	D_RSV_BLK					8			/* 證ｵ蠏｡							*/
#define	D_RSV_BLKOFF				9			/* 證ｵ蠏｡螟晏ｽ贀						*/
#define	D_RSV_TST					10			/* 諱梧叶							*/
#define	D_RSV_TSTOFF				11			/* 諱梧叶蟒疲､蘀						*/
#define	D_RSV_LCL					15			/* 儘乕蜒⊿O						*/
#define	D_RSV_RMT					16			/* 儕蜆悟h蜒ｩ						*/
#define	D_RSV_ACTIVE				17			/* Active						*/
#define	D_RSV_NEGATIVE				18			/* Negative						*/
#define	D_RSV_HI_Z					19			/* Hi-Z							*/
#define	D_RSV_NOFACT				20			/* 譴ｫ場側蛛鼀						*/
#define	D_RSV_UNUSE					21			/* 枹蟾頑｢｡						*/
#define D_RSV_AMPPWDSC				22			/* AMP 譎ｹ謠ｹ蟆ｮ謚退 				*/
#define D_RSV_WARMUP				23			/* Warmup 						*/	
#define	D_RSV_ELSE					30			/* 蛛ｦ蛛ｺ諛ꀀ蜒・Q乕					*/
/* @} */

/** @name BRE-TERM related register  define */
/* @{ */

/* ======================================================================== */
/* EXT螽ｭ楢蜆蘭l蜒吏E謗募ｪ頑歯謔｢													*/
/* ======================================================================== */
#define D_RSV_PORTIN				0			/* Port入力設定				*/
#define D_RSV_PORTOUT				1			/* Port出力設定				*/
#define D_RSV_SHF					2			/* 蜒泌ャ儖蜒ｼ譎・褂亠螽泌ｸ・ｿ｣譖ｬ	*/
/* @} */



/** @name register bit  define */
/* @{ */

/* ======================================================================== */
/* 蜆蘭l蜒吏E價蜒｢蜒ｩ謗募ｪ頑歯謔｢													*/
/* ======================================================================== */
/* 螂懈匯諞ｰ謚ｲ螽ｭ楢 */
#define D_RSV_BRETERM_ALM_BIT		0x8000		/* BRE-TERM蜒・h蜒ｪ傾儔乕儉専弌價蜒｢蜒ｩ		*/
#define D_RSV_FAN_ALM_BIT			0x0004		/* FAN-ALM専弌價蜒｢蜒ｩ			*/

/* TRA SV蜆蘭l蜒吏E螽ｭ楢 */
#define D_RSV_ACT_BIT				0x2000		/* 蜒・h蜒ｪACT價蜒｢蜒ｩ				*/
#define D_RSV_NOINS_BIT				0x4000		/* 蜒・h蜒ｪ枹蟷壽・價蜒｢蜒ｩ			*/
#define D_RSV_INS_BIT				0x8000		/* 蜒・h蜒ｪ蟷壽・價蜒｢蜒ｩ				*/
#define D_RSV_TST_BIT				0x0200		/* 蜒・h蜒ｪ諱梧叶價蜒｢蜒ｩ				*/
#define D_RSV_BLK_BIT				0x0400		/* 蜒・h蜒ｪ證ｵ蠏｡價蜒｢蜒ｩ				*/
#define D_RSV_RMTERR_BIT			0x0080		/* 儕蜆悟h蜒ｩERR價蜒｢蜒ｩ			*/
#define D_RSV_USE_BIT				0x0066		/* 蜒・h蜒ｪUSE専弌價蜒｢蜒ｩ				*/
#define D_RSV_ALM_BIT				0x183F		/* 蜒・h蜒ｪALM専弌價蜒｢蜒ｩ				*/
#define D_RSV_RSALM_BIT				0x0018		/* 儕蜒懷h蜒輸LM 専弌價蜒｢蜒ｩ */
#define D_RSV_HDALM_BIT				0x0007		/* 僴乕蜒ｪALM 専弌價蜒｢蜒ｩ*/
#define D_RSV_SOFTALM_BIT			0x0020		/* Soft ALM 専弌價蜒｢蜒ｩ */
#define D_RSV_MAJALM_PLL_BIT		0x0001		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:PLL螳ｯ傾儔乕儉 */
#define D_RSV_MAJALM_CPRI_BIT		0x0002		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:CPRI螳ｯ傾儔乕儉*/
#define D_RSV_MAJALM_APDCOM_BIT		0x0004		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX螳ｯ傾儔乕儉 */
#define D_RSV_MAJALM_APD0_BIT		0x0008		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#0螳ｯ傾儔乕儉	*/
#define D_RSV_MAJALM_APD1_BIT		0x0010		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#1螳ｯ傾儔乕儉	*/
#define D_RSV_MAJALM_APD2_BIT		0x0800		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#2螳ｯ傾儔乕儉	*/
#define D_RSV_MAJALM_APD3_BIT		0x1000		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#3螳ｯ傾儔乕儉	*/

#define D_RSV_MAJALM_COM_BIT		0x0027		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#1螳ｯ傾儔乕儉	*/
#define D_RSV_MAJALM_BR_BIT			0x0018		/* 蜆竃l蜆榊h傾儔乕儉譴ｫ場:TRX-Br#1螳ｯ傾儔乕儉	*/

#define D_RSV_ERR_BIT				0x0301		/* 蜒・h蜒ｪERR専弌價蜒｢蜒ｩ				*/
#define D_RSV_ERR_PAOFF_BIT			0x0300		/* 蜒・h蜒ｪPA#0_OFF/PA#1_OFF ERR専弌價蜒｢蜒ｩ */
#define D_RSV_RFUSE_BIT				0x0044		/* RF shared 3G carrier USE bit*/
#define D_RSV_ACT_BIT_ON			0x2000		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_ACT_BIT_OFF			0xC607		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_BLK_BIT_ON			0x0400		/* BLK bit set ON value in SV A0/A1 */
#define D_RSV_BLK_BIT_OFF			0xE207		/* BLK bit set ON value in SV A0/A1 */
#define D_RSV_USE_BIT_ON			0x0004		/* USE bit set ON value in SV A0/A1 */
#define D_RSV_USE_BIT_OFF			0xE603		/* USE bit set OFF value in SV A0/A1 */
#define D_RSV_ALM_BIT_ON			0x0002		/* ALM bit set ON value in SV A0/A1 */
#define D_RSV_ALM_BIT_OFF			0xE605		/* ALM bit set OFF value in SV A0/A1 */
#define D_RSV_ERR_BIT_ON			0x0001		/* ERR bit set ON value in SV A0/A1 */
#define D_RSV_ERR_BIT_OFF			0xE606		/* ERR bit set OFF value in SV A0/A1 */
#define D_RSV_PAOFF_BOTH			0x0300		/* PAOFF(BR0&BR1) bit in SV A6/A7 */
#define D_RSV_PAOFF_BR0				0x0100		/* PAOFF(BR0) bit in SV A6/A7 */
#define D_RSV_PAOFF_BR1				0x0200		/* PAOFF(BR1) bit in SV A6/A7 */
#define D_RSV_RMT_ERR_BIT			0x8000		/* Remote ERR bit in SV A8/A9 */
#define D_RSV_TRX0ALM_BIT			0x0008		/* TRX0 ALM 専弌價蜒｢蜒ｩ */
#define D_RSV_TRX1ALM_BIT			0x0010		/* TRX1 ALM 専弌價蜒｢蜒ｩ */
#define D_RSV_BR0ALM_BIT			0x0001		/* BR0 ALM 専弌價蜒｢蜒ｩ */
#define D_RSV_BR1ALM_BIT			0x0010		/* BR1 ALM 専弌價蜒｢蜒ｩ */
#define D_RSV_FANALM_BIT			0x003F		/* FAN alarm bit in SV A10 *//* A4/A5→A10,C0C0→003F 4TRx */

#define D_RSV_PLLALM_BIT			0x0001
#define D_RSV_CPRIALM_BIT			0x0006
#define D_RSV_COM1ALM_BIT			0x0000
#define D_RSV_COM2ALM_BIT			0x005D
#define D_RSV_APD0ALM_BIT			0x7DF0
#define D_RSV_APD1ALM_BIT			0x7DF0
#define D_RSV_APD2ALM_BIT			0x7DF0
#define D_RSV_APD3ALM_BIT			0x7DF0
#define D_RSV_APD2ALM_BIT_RRE		0x7DF1
#define D_RSV_APD3ALM_BIT_RRE		0x7DF1

/* 儘乕蜒⊿O/儕蜆悟h蜒ｩ */
#define D_RSV_LCL_BIT				0x0080		/* 儘乕蜒⊿O諠ょｱｼ價蜒｢蜒ｩ			*/
#define D_RSV_RMT_BIT				0x0040		/* 儕蜆悟h蜒ｩ諠ょｱｼ價蜒｢蜒ｩ			*/

/* @} */


/** @name register mask  define */
/* @{ */

/* ======================================================================== */
/* 蜆蘭l蜒吏E蜆・・蜒区歯蟐頑歯謔｢													*/
/* ======================================================================== */

/* TRA SV蜆蘭l蜒吏E螽ｭ楢 */
#define D_RSV_MSK_TRA0			0xE607			/* TRA SV蜒ｶ蜒嶺ｹｽA0荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA1			0xFFFF			/* TRA SV蜒ｶ蜒嶺ｹｽA1荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA2			0x183F			/* TRA SV蜒ｶ蜒嶺ｹｽA2荵ｿ昞蟶ｵ		*//* RRE用の設定値は削除 4TRx */
#define D_RSV_MSK_TRA3			0x0F01			/* TRA SV蜒ｶ蜒嶺ｹｽA3荵ｿ昞蟶ｵ		*//* RRE用の設定値は削除 4TRx */
#define D_RSV_MSK_TRA4			0x19FD			/* TRA SV蜒ｶ蜒嶺ｹｽA4荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA4_RRE		0xFDFD			/* TRA SV蜒ｶ蜒嶺ｹｽA4荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA5			0x3037			/* TRA SV蜒ｶ蜒嶺ｹｽA5荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA6			0x007C			/* TRA SV蜒ｶ蜒嶺ｹｽA6荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA7			0x00FF			/* TRA SV蜒ｶ蜒嶺ｹｽA7荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA8			0x0007			/* TRA SVﾆ弛ﾆ湛ﾂ「A8ﾂ」窶｢\ﾅｽ|		*/
#define D_RSV_MSK_TRA9			0x005D			/* TRA SV蜒ｶ蜒嶺ｹｽA9荵ｿ昞蟶ｵ		*/
#define D_RSV_MSK_TRA10			0x3DF0			/* TRA SV蜒ｶ蜒嶺ｹｽA10荵ｿ昞蟶ｵ	*//* 7DF0→3DF0 4TRx */
#define D_RSV_MSK_TRA11			0x3DF0			/* TRA SV蜒ｶ蜒嶺ｹｽA11荵ｿ昞蟶ｵ	*//* 7DF0→3DF0 4TRx */
#define D_RSV_MSK_TRA12			0x3DF0			/* TRA SV蜒ｶ蜒嶺ｹｽA12荵ｿ昞蟶ｵ	*//* 7DF0→3DF0(RRE用の設定値は削除) 4TRx */
#define D_RSV_MSK_TRA13			0x3DF0			/* TRA SV蜒ｶ蜒嶺ｹｽA13荵ｿ昞蟶ｵ	*//* 7DF0→3DF0(RRE用の設定値は削除) 4TRx */
#define D_RSV_MSK_FAN_RRE		0x003F			/* TRA SV蜒ｶ蜒嶺ｹｽA4荵ｿ昞蟶ｵ		*//* FAN情報の設定値を追加 4TRx */

/** @name RSV log define */
/* @{ */

/* ======================================================================== */
/* RE螽泌ｸ・ｱ・ﾝ儘蜒梧歯蟐頑歯謔｢													*/
/* ======================================================================== */
#define D_RSV_LOG_MAX			160			/* RE螽泌ｸ・ｱ・ﾝ儘蜒梧渕謔｢			*/
#define D_RSV_LOG_SV1_SIZE		16			/* RE螽泌ｸ・ｱ・ﾝ儘蜒谀1譟ｺ蜒貞ム僘		*/
#define D_RSV_LOG_SV2_SIZE		384			/* RE螽泌ｸ・ｱ・ﾝ儘蜒谀2譟ｺ蜒貞ム僘		*/
/* @} */

/** @name other define */
/* @{ */
/* 蜒・Q乕蠢｣譖ｬ蜆蘭l蜒吏E偵PA-SW蠢ｬ懺蛛ｺbit */
#define	D_RSV_PASW_BIT		0x0001				/**< PA-SW蠢ｬ懺蛛ｺbit(T.B.D)  */


/* ======================================================================== */
/* CPRI state define															*/
/* ======================================================================== */
#define D_RSV_CPRISTA_E               4           /**< CPRI stateE            */

/* ======================================================================== */
/* LED setting value define														*/
/* ======================================================================== */
#define D_RSV_LED1SET_OFF				0					/* LED1: OFF					*/
#define D_RSV_LED1SET_GRNBLNK			0x00000008			/* LED1: GREEN Blink			*/
#define D_RSV_LED1SET_GRNON				0x0000000C			/* LED1: GREEN ON				*/
#define D_RSV_LED1SET_REDBLNK			0x00000002			/* LED1: RED Blink				*/
#define D_RSV_LED1SET_REDON				0x00000003			/* LED1: RED ON					*/
#define D_RSV_LED1SET_REDGRN			0x00000010			/* LED1: RED/GREEN Blink		*/

#define D_RSV_LED2SET_OFF				0					/* LED2: OFF					*/
#define D_RSV_LED2SET_BLNK				0x00000200			/* LED2: Blink					*/
#define D_RSV_LED2SET_ON				0x00000300			/* LED2: ON						*/
#define D_RSV_LED1_MASK					0x00000700
/* @} */

/** @name table max index define */
/* @{ */

/* ======================================================================== */
/* 僥乕蜒ｽ儖譴ｫ諷ｺ謔｢謗募ｪ頑歯謔｢													*/
/* ======================================================================== */
#define D_RSV_SVTBL_NUM			15			/* SV僥乕蜒ｽ儖蛛ｺ譴ｫ諷ｺ謔｢			*//* 14→15 4TRx */
#define D_RSV_PLLALM_NUM		2			/* minor alarm factor number of PLL ALARM */
#define D_RSV_CPRIALM_NUM		2			/* minor alarm factor number of CPRI ALARM */
#define D_RSV_APDCOMALM_NUM	5			/* minor alarm factor number of APD COM ALARM */
#define D_RSV_APDALM_NUM		7			/* minor alarm factor number of APD0/APD1 ALARM */
#define D_RSV_3GRFINFALM_NUM	0			/* alarm factor number of 3GRFINF ALARM */
#define D_RSV_EXTIFTBL_NUM		0			/* EXT interface table number*/
/* @} */

/** @name Alarm detecting number define */
/* @{ */
#define D_RSV_ALM_NUM			6 /* 5→6 配列の不足が発生する為 4TRx */
#define D_RSV_ALM_NUM_RRE		6 /* 7→6 4TRx */
/* @} */

/** @name Error detecting number define */
/* @{ */
#define D_RSV_ERR_NUM				7 /* 4→9 4TRx */
#define D_RSV_ERR_NUM_200MS			2 /* 200ms用 4TRx */
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
#define D_RSV_ALMBIT_PLLNG			0x00000100		/* PLL UNLOCK 				*/
#define D_RSV_ALMBIT_FPGANG1		0x00000001		/* FPGA-PLL-UNLOCK1			*/
#define D_RSV_ALMBIT_FPGANG2_1		0x00000040		/* FPGA-PLL-UNLOCK2(bit6)	*/
#define D_RSV_ALMBIT_FPGANG2_2		0x00000080		/* FPGA-PLL-UNLOCK2(bit7)	*/
#define D_RSV_ALMBIT_FPGANG2		0x000000C0		/* FPGA-PLL-UNLOCK2			*/
#define D_RSV_ALMBIT_FPGANG2_RRE	0x00000040		/* FPGA-PLL-UNLOCK2			*/
#define D_RSV_ALMBIT_FPGANG3		0x00000100		/* FPGA-PLL-UNLOCK3			*/
#define D_RSV_ALMBIT_FPGAIFNG2  	0x20000000		/* FPGA-IF ALM2				*/
#define D_RSV_ALMBIT_CPLDIF1		0x10000000		/* CPLD IF ALM_1			*/
#define D_RSV_ALMBIT_FAN1			0x000000FF		/* FAN ALM1			*//* 0000FFFF→000000FF 4TRx */
#define D_RSV_ALMBIT_FAN2			0x0000FF00		/* FAN ALM2			*//* FFFF0000→0000FF00 4TRx */
#define D_RSV_ALMBIT_FAN3			0x00FF0000		/* FAN ALM3			*//* 4TRx */
#define D_RSV_ALMBIT_FAN4			0xFF000000		/* FAN ALM4			*//* 4TRx */
#define D_RSV_ALMBIT_FAN5			0x000000FF		/* FAN ALM5			*//* 4TRx */
#define D_RSV_ALMBIT_FAN6			0x0000FF00		/* FAN ALM6			*//* 4TRx */

/* Admin State 状態 */
#define D_RSV_ALMBIT_ADMIN			0x00000007		/* Admin Status					*/
#define D_RSV_ALMBIT_ADMIN_LOCK		0x00000001		/* Admin State = Locked         */
#define D_RSV_ALMBIT_ADMIN_SDWN		0x00000002		/* Admin State = shutting down  */
#define D_RSV_ALMBIT_ADMIN_UNLCK	0x00000004		/* Admin State = Unlocked       */
/* Energysaving state状態 */
#define D_RSV_ALMBIT_PWRSV			0x00000030		/* Energysaving state			*/
#define D_RSV_ALMBIT_PWRSV_SLP 		0x00000010		/* Energysaving state = sleep   */
#define D_RSV_ALMBIT_PWRSV_AWK 		0x00000020		/* Energysaving state = awake   */
/* Operational State状態 */
#define D_RSV_ALMBIT_OPRT	 		0x00000300		/* Operational State 			*/
#define D_RSV_ALMBIT_OPRT_EBL 		0x00000100		/* Operational State = Enable   */
#define D_RSV_ALMBIT_OPRT_DIS 		0x00000200		/* Operational State = Disable  */
/* availability State状態 */
#define D_RSV_ALMBIT_AVAIL			0x00007000		/* availability State 			  */
#define D_RSV_ALMBIT_AVAIL_FAULT	0x00001000		/* availability State = faulty    */
#define D_RSV_ALMBIT_AVAIL_DEGRA	0x00002000		/* availability State = degraded  */
#define D_RSV_ALMBIT_AVAIL_NORML	0x00004000		/* availability State = normal    */
/* usage State状態 */
#define D_RSV_ALMBIT_USAGE			0x00070000		/* usage State 					*/
#define D_RSV_ALMBIT_USAGE_BUSY		0x00010000		/* usage State = Busy           */
#define D_RSV_ALMBIT_USAGE_ACTV		0x00020000		/* usage State = Active         */
#define D_RSV_ALMBIT_USAGE_IDLE		0x00040000		/* usage State = Idle           */

#define D_EQS_ALMBIT_ANTA			0x00000107		/* Ant.A アラーム				*/
#define D_EQS_ALMBIT_ANTA_RXGAIN	0x00000100		/* Rx Gain ALM(Ant.A)			*/
#define D_EQS_ALMBIT_ANTA_TDDSW		0x00000004		/* TDD SW ALM(Ant.A)			*/
#define D_EQS_ALMBIT_ANTA_TOVER		0x00000002		/* TOVER ALM(Ant.A)				*/
#define D_EQS_ALMBIT_ANTA_TXGAIN	0x00000001		/* Tx Gain ALM(Ant.A)			*/

#define D_EQS_ALMBIT_ANTB			0x01070000		/* Ant.B アラーム				*/
#define D_EQS_ALMBIT_ANTB_RXGAIN	0x01000000		/* Rx Gain ALM(Ant.B)			*/
#define D_EQS_ALMBIT_ANTB_TDDSW		0x00040000		/* TDD SW ALM(Ant.B)			*/
#define D_EQS_ALMBIT_ANTB_TOVER		0x00020000		/* TOVER ALM(Ant.B)				*/
#define D_EQS_ALMBIT_ANTB_TXGAIN	0x00010000		/* Tx Gain ALM(Ant.B)			*/

#define D_EQS_ALMBIT_ANTC			0x00000107		/* Ant.C アラーム				*/
#define D_EQS_ALMBIT_ANTC_RXGAIN	0x00000100		/* Rx Gain ALM(Ant.C)			*/
#define D_EQS_ALMBIT_ANTC_TDDSW		0x00000004		/* TDD SW ALM(Ant.C)			*/
#define D_EQS_ALMBIT_ANTC_TOVER		0x00000002		/* TOVER ALM(Ant.C)				*/
#define D_EQS_ALMBIT_ANTC_TXGAIN	0x00000001		/* Tx Gain ALM(Ant.C)			*/

#define D_EQS_ALMBIT_ANTD			0x01070000		/* Ant.D アラーム				*/
#define D_EQS_ALMBIT_ANTD_RXGAIN	0x01000000		/* Rx Gain ALM(Ant.D)			*/
#define D_EQS_ALMBIT_ANTD_TDDSW		0x00040000		/* TDD SW ALM(Ant.D)			*/
#define D_EQS_ALMBIT_ANTD_TOVER		0x00020000		/* TOVER ALM(Ant.D)				*/
#define D_EQS_ALMBIT_ANTD_TXGAIN	0x00010000		/* Tx Gain ALM(Ant.D)			*/

#define D_EQS_ALMBIT_ANTE			0x00000107		/* Ant.E アラーム				*/
#define D_EQS_ALMBIT_ANTE_RXGAIN	0x00000100		/* Rx Gain ALM(Ant.E)			*/
#define D_EQS_ALMBIT_ANTE_TDDSW		0x00000004		/* TDD SW ALM(Ant.E)			*/
#define D_EQS_ALMBIT_ANTE_TOVER		0x00000002		/* TOVER ALM(Ant.E)				*/
#define D_EQS_ALMBIT_ANTE_TXGAIN	0x00000001		/* Tx Gain ALM(Ant.E)			*/

#define D_EQS_ALMBIT_ANTF			0x01070000		/* Ant.F アラーム				*/
#define D_EQS_ALMBIT_ANTF_RXGAIN	0x01000000		/* Rx Gain ALM(Ant.F)			*/
#define D_EQS_ALMBIT_ANTF_TDDSW		0x00040000		/* TDD SW ALM(Ant.F)			*/
#define D_EQS_ALMBIT_ANTF_TOVER		0x00020000		/* TOVER ALM(Ant.F)				*/
#define D_EQS_ALMBIT_ANTF_TXGAIN	0x00010000		/* Tx Gain ALM(Ant.F)			*/

#define D_EQS_ALMBIT_ANTG			0x00000107		/* Ant.G アラーム				*/
#define D_EQS_ALMBIT_ANTG_RXGAIN	0x00000100		/* Rx Gain ALM(Ant.G)			*/
#define D_EQS_ALMBIT_ANTG_TDDSW		0x00000004		/* TDD SW ALM(Ant.G)			*/
#define D_EQS_ALMBIT_ANTG_TOVER		0x00000002		/* TOVER ALM(Ant.G)				*/
#define D_EQS_ALMBIT_ANTG_TXGAIN	0x00000001		/* Tx Gain ALM(Ant.G)			*/

#define D_EQS_ALMBIT_ANTH			0x01070000		/* Ant.H アラーム				*/
#define D_EQS_ALMBIT_ANTH_RXGAIN	0x01000000		/* Rx Gain ALM(Ant.H)			*/
#define D_EQS_ALMBIT_ANTH_TDDSW		0x00040000		/* TDD SW ALM(Ant.H)			*/
#define D_EQS_ALMBIT_ANTH_TOVER		0x00020000		/* TOVER ALM(Ant.H)				*/
#define D_EQS_ALMBIT_ANTH_TXGAIN	0x00010000		/* Tx Gain ALM(Ant.H)			*/

#define D_EQS_ALMBIT_ANT_NON		0x00000000		/* アラームなし					*/

/* availability State状態 */
#define D_RSV_ALMBIT_FAULT			0x00000001		/* availability State = faulty    */
#define D_RSV_ALMBIT_DEGRA			0x00000002		/* availability State = degraded  */
#define D_RSV_ALMBIT_NORML			0x00000004		/* availability State = normal    */
/* usage State状態 */
#define D_RSV_ALMBIT_BUSY			0x00000001		/* usage State = Busy           */
#define D_RSV_ALMBIT_ACTV			0x00000002		/* usage State = Active         */
#define D_RSV_ALMBIT_IDLE			0x00000004		/* usage State = Idle           */

#define D_RSV_ALMBIT_PLLUNLCK		0x00000030		/* Fault ID 0x03のアラーム要因	*/

#define D_RSV_ALMBIT_PSALM			0x37000000		/* Fault ID 0x06のアラーム要因	*/

#define D_RSV_ALMBIT_HRDFLT1		0x3FFF1FFF		/* Fault ID 0x07のアラーム要因	*/
#define D_RSV_ALMBIT_HRDFLT2		0x00003FFF		/* Fault ID 0x07のアラーム要因	*/

#define D_RSV_ALMBIT_RFICALM		0x000F0000		/* Fault ID 0x08のアラーム要因	*/

#define D_RSV_ALMBIT_TXODDALM		0x00000007		/* Fault ID 0x10のアラーム要因	*/
#define D_RSV_ALMBIT_RXODDALM		0x00000100		/* Fault ID 0x20のアラーム要因	*/
#define D_RSV_ALMBIT_TXEVNALM		0x00070000		/* Fault ID 0x10のアラーム要因	*/
#define D_RSV_ALMBIT_RXEVNALM		0x01000000		/* Fault ID 0x20のアラーム要因	*/

#define	D_RSV_ALMBIT_NON			0x00000000

/* Ant状態 */
#define D_RSV_ANTA_STATE			0x000000FF		/* Ant.A State                  */
#define D_RSV_ANTB_STATE			0x0000FF00		/* Ant.B State                  */
#define D_RSV_ANTC_STATE			0x00030000		/* Ant.C State                  */
#define D_RSV_ANTD_STATE			0x03000000		/* Ant.D State                  */
#define D_RSV_ANTE_STATE			0x00000003		/* Ant.E State                  */
#define D_RSV_ANTF_STATE			0x00000300		/* Ant.F State                  */
#define D_RSV_ANTG_STATE			0x00030000		/* Ant.G State                  */
#define D_RSV_ANTH_STATE			0x03000000		/* Ant.H State                  */

/* Fault ID */
#define D_RSV_FALT_IDARA			0x0311F000		/* Fault ID Area				*/
#define D_RSV_FALT_CRIT				0x00000004		/* Fault Kind(Critical)			*/
#define D_RSV_FALT_MAJOR			0x00000002		/* Fault Kind(Major)			*/
#define D_RSV_FALT_MINOR			0x00000001		/* Fault Kind(Minor)			*/
#define D_RSV_FALT_KIND				0x00000007		/* Fault Kind Area				*/

/* PTP State */
#define D_RSV_PTP_SYNCLCK			0x70000000		/* PTP SyncE Locked State		*/
#define D_RSV_PTP_LCKSTS			0x02000000		/* PTP Locked Status			*/
#define D_RSV_SYNCE_LCKSTS			0x01000000		/* SyncE Locked State			*/
#define D_RSV_SYNCE_STS				0x000F0000		/* SyncE Status					*/
#define D_RSV_PTP_ANT				0x00000003		/* All ANT 停波					*/

#define D_RSV_PTP_NON_ANT			0x00000000		/* ALL ANT 停波解除				*/

/* L1 Port Down State */
#define D_RSV_PRT1_L1DWN			0x00000100		/* Port1 L1 断					*/
#define D_RSV_PRT2_L1DWN			0x00010000		/* Port2 L1 断					*/

/*O-RAN対応 faultId値見直し*/
#define D_EQS_FALT_FID01			0x1001			/* Fault ID = 0x1001 (H/S:0x0001)	*/
#define D_EQS_FALT_FID02			0x1002			/* Fault ID = 0x1002 (H/S:0x0002)	*/
#define D_EQS_FALT_FID03			0x1003			/* Fault ID = 0x1003 (H/S:0x0003)	*/
#define D_EQS_FALT_FID04			0x1004			/* Fault ID = 0x1004 (H/S:0x0004)	*/
#define D_EQS_FALT_FID05			0x1005			/* Fault ID = 0x1005 (H/S:0x0005)	*/
#define D_EQS_FALT_FID06			0x1006			/* Fault ID = 0x1006 (H/S:0x0006)	*/
#define D_EQS_FALT_FID07			0x000F			/* Fault ID = 0x000F (H/S:0x0007)	*/
#define D_EQS_FALT_FID08			0x1008			/* Fault ID = 0x1008 (H/S:0x0008)	*/
#define D_EQS_FALT_FID10			0x0010			/* Fault ID = 0x0010 ※未使用：アンテナALM用を適用	*/
#define D_EQS_FALT_FID11			0x0011			/* Fault ID = 0x0011 ※未使用：アンテナALM用を適用	*/
#define D_EQS_FALT_FID12			0x0012			/* Fault ID = 0x0012 ※未使用：アンテナALM用を適用	*/
#define D_EQS_FALT_FID30			0x0004			/* Fault ID = 0x0004 (H/S:0x0030)	*/
#define D_EQS_FALT_FID31			0x0002			/* Fault ID = 0x0002 (H/S:0x0031)	*/
#define D_EQS_FALT_FID40			0x0006			/* Fault ID = 0x0006 (H/S:0x0040)	*/
#define D_EQS_FALT_FID41			0x1041			/* Fault ID = 0x1041 (H/S:0x0041)	*/
#define D_EQS_FALT_FID42			0x1042			/* Fault ID = 0x1042 (H/S:0x0042)	*/
#define D_EQS_FALT_FID43			0x1043			/* Fault ID = 0x1043 (H/S:0x0043)	*/
#define D_EQS_FALT_FID44			0x1044			/* Fault ID = 0x1044 (H/S:0x0044)	*/
#define D_EQS_FALT_FID50			0x1050			/* Fault ID = 0x1050 (H/S:0x0050)	*/
#define D_EQS_FALT_FID51			0x1051			/* Fault ID = 0x1051 (H/S:0x0051)	*/
#define D_EQS_FALT_FIDF0			0x10F0			/* Fault ID = 0x10F0 (H/S:0x00F0)	*/
#define D_EQS_FALT_FIDF1			0x10F1			/* Fault ID = 0x10F1 (H/S:0x00F1)	*/
#define D_EQS_FALT_FIDF2			0x10F2			/* Fault ID = 0x10F2 (H/S:0x00F2)	*/
#define D_EQS_FALT_FIDFF			0x10FF			/* Fault ID = 0x10FF (H/S:0x00FF)	*/

/*O-RAN対応 faultId値(アンテナALM用)*/
#define D_EQS_FALT_FID2010			0x2010			/* Fault ID = 0x2010 (H/S:0x0010)	*/
#define D_EQS_FALT_FID2410			0x2410			/* Fault ID = 0x2410 (H/S:0x0010)	*/
#define D_EQS_FALT_FID2810			0x2810			/* Fault ID = 0x2810 (H/S:0x0010)	*/
#define D_EQS_FALT_FID2C10			0x2C10			/* Fault ID = 0x2C10 (H/S:0x0010)	*/
#define D_EQS_FALT_FID2011			0x2011			/* Fault ID = 0x2011 (H/S:0x0011)	*/
#define D_EQS_FALT_FID2411			0x2411			/* Fault ID = 0x2411 (H/S:0x0011)	*/
#define D_EQS_FALT_FID2811			0x2811			/* Fault ID = 0x2811 (H/S:0x0011)	*/
#define D_EQS_FALT_FID2C11			0x2C11			/* Fault ID = 0x2C11 (H/S:0x0011)	*/
#define D_EQS_FALT_FID2012			0x2012			/* Fault ID = 0x2012 (H/S:0x0012)	*/
#define D_EQS_FALT_FID2412			0x2412			/* Fault ID = 0x2412 (H/S:0x0012)	*/
#define D_EQS_FALT_FID2812			0x2812			/* Fault ID = 0x2812 (H/S:0x0012)	*/
#define D_EQS_FALT_FID2C12			0x2C12			/* Fault ID = 0x2C12 (H/S:0x0012)	*/

/*O-RAN対応 faultId中間装置状態変化時*/
#define D_EQS_FALT_FIDEA60			0xEA60			/* Fault ID = 0xEA60 (H/S:0x0012)	*/

#define D_EQS_WVSTP_ANTA			0x00000000		/* Ant - Aアラーム停波状態		*/
#define D_EQS_WVSTP_ANTB			0x00000001		/* Ant - Bアラーム停波状態		*/
#define D_EQS_WVSTP_ANTC			0x00000002		/* Ant - Cアラーム停波状態		*/
#define D_EQS_WVSTP_ANTD			0x00000003		/* Ant - Dアラーム停波状態		*/
#define D_EQS_WVSTP_ANTE			0x00000004		/* Ant - Eアラーム停波状態		*/
#define D_EQS_WVSTP_ANTF			0x00000005		/* Ant - Fアラーム停波状態		*/
#define D_EQS_WVSTP_ANTG			0x00000006		/* Ant - Gアラーム停波状態		*/
#define D_EQS_WVSTP_ANTH			0x00000007		/* Ant - Hアラーム停波状態		*/

#define D_EQS_WVSTP_ST0				0x00000001
#define D_EQS_WVSTP_ST1				0x00000002
#define D_EQS_WVSTP_ST2				0x00000004
#define D_EQS_WVSTP_ST3				0x00000008
#define D_EQS_WVSTP_ST4				0x00000010
#define D_EQS_WVSTP_ST5				0x00000020
#define D_EQS_WVSTP_ST6				0x00000040
#define D_EQS_WVSTP_ST7				0x00000080
#define D_EQS_WVSTP_ST8				0x00000100
#define D_EQS_WVSTP_ST9				0x00000200
#define D_EQS_WVSTP_ST10			0x00000400
#define D_EQS_WVSTP_ST11			0x00000800
#define D_EQS_WVSTP_ST12			0x00001000
#define D_EQS_WVSTP_ST13			0x00002000
#define D_EQS_WVSTP_ST14			0x00004000
#define D_EQS_WVSTP_ST15			0x00008000
#define D_EQS_WVSTP_ST8_RFIC		0x00010000
#define D_EQS_WVSTP_CHG				0x80000000
#define D_EQS_WVSTP_STNON			0x00000000		/* Ant 停波解除状態				*/


#define D_EQS_ST05_WVFLT			0x37000031		/* 故障停波						*/
#define D_EQS_PTP_UNLOCK			0x00000001		/* PTP Unlock(HoldOver)			*/
#define D_EQS_JCPLL_UNLOCK			0x00000010		/* LC-PLL UnLock				*/
#define D_EQS_RFPLL_UNLOCK			0x00000020		/* RF-PLL UnLock				*/
#define D_EQS_RFMDL_PSALM			0x01000000		/* RF Module PS ALM				*/
#define D_EQS_GTBS_POWER			0x02000000		/* Sub6: GateBias Power			*/
#define D_EQS_RFIC_POWER			0x04000000		/* Sub6: RFIC Power				*/
#define D_EQS_TXDC_POWER			0x10000000		/*  mmW: TXDAC Power			*/
#define D_EQS_RXDC_POWER			0x20000000		/*  mmW: RXDAC Power			*/

#define D_EQS_WVSP_LLB_SYSPLL_UNLCK					0x00000001
#define D_EQS_WVSP_LLB_ICPLL_UNLCK					0x00000002
#define D_EQS_WVSP_LLB_ETHPL_UULCK					0x00000004
#define D_EQS_WVSP_LLB_TRX1_PCIE_RTERR				0x00000008
#define D_EQS_WVSP_LLB_TRX1_PCIE_EPNTER				0x00000010
#define D_EQS_WVSP_LLB_TRX2_PCIE_RRTERR				0x00000020
#define D_EQS_WVSP_LLB_TRX2_PCIE_EPNTER				0x00000040
#define D_EQS_WVSP_LLB_TRX1_SVBUS_FAIL				0x00000080
#define D_EQS_WVSP_LLB_TRX2_SVBUS_FAIL				0x00000100
#define D_EQS_WVSP_LLB_TRX1_ICTX_ERR				0x00000200
#define D_EQS_WVSP_LLB_TRX1_ICRX_ERR				0x00000400
#define D_EQS_WVSP_LLB_TRX2_ICTX_ERR				0x00000800
#define D_EQS_WVSP_LLB_TRX2_ICRX_ERR				0x00001000
#define D_EQS_WVSP_TRX1_SYS PLL UNLCK				0x00010000
#define D_EQS_WVSP_TRX1_ICPLL UNLCK					0x00020000
#define D_EQS_WVSP_TRX1_PHYICPLLULCK1				0x00040000
#define D_EQS_WVSP_TRX1_PHYICPLLULCK2				0x00080000
#define D_EQS_WVSP_TRX1_PHYICPLLULCK3				0x00100000
#define D_EQS_WVSP_TRX1_PHYJS1PLLULCK				0x00200000
#define D_EQS_WVSP_TRX1_PHYJS2PLLULCK				0x00400000
#define D_EQS_WVSP_TRX1_SVBUS_FAIL					0x00800000
#define D_EQS_WVSP_TRX1_RFIC1_TX_JESD_ERR			0x01000000
#define D_EQS_WVSP_TRX1_RFIC1_FB_JESD_ERR			0x02000000
#define D_EQS_WVSP_TRX1_RFIC1_RX_JESD_ERR			0x04000000
#define D_EQS_WVSP_TRX1_RFIC2_TX_JESD_ERR			0x08000000
#define D_EQS_WVSP_TRX1_RFIC2_FB_JESD_ERR			0x10000000
#define D_EQS_WVSP_TRX1_RFIC2_RX_JESD_ERR			0x20000000
#define D_EQS_WVSP_RFIC_RFIC1_ALRM					0x00010000
#define D_EQS_WVSP_RFIC_RFIC2_ALRM					0x00020000
#define D_EQS_WVSP_RFIC_RFIC3_ALRM					0x00040000
#define D_EQS_WVSP_RFIC_RFIC4_ALRM					0x00080000

#define D_EQS_WVSP_STS6_FLT_ERR						0x3FFF1FFF
#define D_EQS_WVSP_STS7_FLT_ERR						0x00003FFF
#define D_EQS_MVSP_STS7_RFIC_FLT					0x000F0000
#define D_EQS_MVSP_STS12_FLT_ERR					0x00000100
#define D_EQS_CCA_STS13_FLT_ERR						0x0000000F
#define D_EQS_DLUL_STS14_LOADING					0x00000001

#define D_EQS_WVSTP_ON				0x00000001		/* アンテナ停波中				*/
#define D_EQS_WVSTP_OFF				0x00000000		/* アンテナ停波解除中			*/

#define D_EQS_NTF_START_INIT		0x00000000		/*!< Notif通知状態 = 初期化     */
#define D_EQS_NTF_START_RUN			0x00000001		/*!< Notif通知状態 = 通知開始   */

#define D_EQS_NOTIF_NON				0x00
#define D_EQS_NOTIF_CHANGE			0x01			/*!< 状態変化通知				*/
#define D_EQS_NOTIF_ENABLE			0x02			/*!< 状態変化通知(イネーブル)	*/
#define D_EQS_NOTIF_DIASBLE			0x04			/*!< 状態変化通知(ディセーブル)	*/

#define	D_EQS_NOTIF_CHAR_CNT		0x21			/*!< 文字数32文字と定義			*/

#define	D_EQS_FMNTF_SEND_OK			0				/*!< 送信可能					*/
#define	D_EQS_FMNTF_SEND_NG			1				/*!< 送信不可能					*/

/* @} */

/*!
 * @name SVINF
 * @note 
 * @{
 */
enum
{
	E_EQS_SVINF_01 = 0,						/*	(00)SVINF1		*/
	E_EQS_SVINF_02,							/*	(01)SVINF2		*/
	E_EQS_SVINF_03,							/*	(02)SVINF3		*/
	E_EQS_SVINF_04,							/*	(03)SVINF4		*/
	E_EQS_SVINF_05,							/*	(04)SVINF5		*/
	E_EQS_SVINF_06,							/*	(05)SVINF6		*/
	E_EQS_SVINF_07,							/*	(06)SVINF7		*/
	E_EQS_SVINF_08,							/*	(07)SVINF8		*/
	E_EQS_SVINF_09,							/*	(08)SVINF9		*/
	E_EQS_SVINF_10,							/*	(09)SVINF10		*/
	E_EQS_SVINF_11,							/*	(10)SVINF11		*/
	E_EQS_SVINF_12,							/*	(11)SVINF12		*/
	E_EQS_SVINF_13,							/*	(12)SVINF13		*/
	E_EQS_SVINF_14,							/*	(13)SVINF14		*/
	E_EQS_SVINF_15,							/*	(14)SVINF15		*/
	E_EQS_SVINF_16,							/*	(15)SVINF16		*/
	E_EQS_SVINF_MAX							/*	SVINF Number	*/
};

enum
{
		E_EQS_STS_KIND_ADMIN = 1,				/* ステータス種別 = Admin               */
		E_EQS_STS_KIND_PWRSV,					/* ステータス種別 = Energysaving        */
		E_EQS_STS_KIND_OPRT,					/* ステータス種別 = Operational         */
		E_EQS_STS_KIND_AVAIL,					/* ステータス種別 = availability        */
		E_EQS_STS_KIND_USAGE,					/* ステータス種別 = usage               */
		E_EQS_STS_KIND_ANTA,					/* ステータス種別 = Ant.A               */
		E_EQS_STS_KIND_ANTB,					/* ステータス種別 = Ant.B               */
		E_EQS_STS_KIND_ANTC,					/* ステータス種別 = Ant.C               */
		E_EQS_STS_KIND_ANTD,					/* ステータス種別 = Ant.D               */
		E_EQS_STS_KIND_ANTE,					/* ステータス種別 = Ant.E               */
		E_EQS_STS_KIND_ANTF,					/* ステータス種別 = Ant.F               */
		E_EQS_STS_KIND_ANTG,					/* ステータス種別 = Ant.G               */
		E_EQS_STS_KIND_ANTH,					/* ステータス種別 = Ant.H               */
		E_EQS_STS_KIND_PTP_SYNC,				/* ステータス種別 = Sync Locked         */
		E_EQS_STS_KIND_PTP_PLCK,				/* ステータス種別 = PTP Locked Status   */
		E_EQS_STS_KIND_PTP_SLCK,				/* ステータス種別 = SyncE Locked Status */
		E_EQS_STS_KIND_PTP_SYNCE,				/* ステータス種別 = SyncE Status        */
		E_EQS_STS_KIND_PTP_ALANT,				/* ステータス種別 = all ANT 停波        */
		E_EQS_STS_KIND_TRANING,					/* ステータス種別 = トレーニングNG		*/
		E_EQS_STS_KIND_DL						/* ステータス種別 = DL					*/
};

#define D_EQS_ALLANT_STOP		0x03CC			/* 停波制御フラグ アドレス(EEP)			*/
#define D_EQS_ALLANT_ENABLE		0x01			/* 停波制御イネーブル					*/

#define D_EQS_TXANTENA_ADDR		0xB1100040		/* Ant.A CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENB_ADDR		0xB1100440		/* Ant.B CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENC_ADDR		0xB1100840		/* Ant.C CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTEND_ADDR		0xB1100C40		/* Ant.D CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENE_ADDR		0xB3100040		/* Ant.E CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENF_ADDR		0xB3100440		/* Ant.F CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENG_ADDR		0xB3100840		/* Ant.G CA_TXANTENレジスタアドレス		*/
#define D_EQS_TXANTENH_ADDR		0xB3100C40		/* Ant.H CA_TXANTENレジスタアドレス		*/

#define D_EQS_TXANTEN_ENABLE	0x80000000		/* 停波解除(Enable)						*/
#define D_EQS_TXANTEN_DISABLE	0x00000000		/* 停波(Disable)						*/

#define D_EQS_TCX_TDANTEN_DIS		0x00000000		/* Disable							*/
#define D_EQS_TCX_TDANTEN_PAOFF		0x00000001		/* Enable/PA OFF					*/
#define D_EQS_TCX_TDANTEN_EN		0x00000003		/* Enable/PA ON						*/
#define D_EQS_TCX_TDANTEN_DPDON		0xFFFFFFFF		/* DPDON only						*/
#define D_EQS_TCX_TDANTEN_DPDOFF	0xFFFFFFFE		/* DPDOFF only						*/

#define D_EQS_STPWV_ANT_EN			0x00000000		/*	ANT#X 通常(Enable)				*/
#define D_EQS_STPWV_ANT_DIS_ALL		0x000000FF		/*	ANT#A~D 閉塞orFault(Disable)	*/
#define D_EQS_STPWV_ANT_DIS_AB		0x00000033		/*	ANT#A/B 閉塞orFault(Disable)	*/
#define D_EQS_STPWV_ANT_DIS_CD		0x000000CC		/*	ANT#C/D 閉塞orFault(Disable)	*/
#define D_EQS_STPWV_ANT_DIS_AC		0x00000055		/*	ANT#A/C 閉塞orFault(Disable)	*/
#define D_EQS_STPWV_ANT_DIS_BD		0x000000AA		/*	ANT#B/D 閉塞orFault(Disable)	*/
#define D_EQS_STPWV_ANT_PAOFF_ALL	0x0000000F		/*	ANT#A~D PA OFF					*/
#define D_EQS_STPWV_ANT_PAOFF_AB	0x00000003		/*	ANT#A~B PA OFF					*/
#define D_EQS_STPWV_ANT_PAOFF_CD	0x0000000C		/*	ANT#C~D PA OFF					*/
#define D_EQS_STPWV_ANT_DIS_A		0x00000011		/*	ANT#A 閉塞orFault(Disable)		*/
#define D_EQS_STPWV_ANT_PAOFF_A		0x00000001		/*	ANT#A PA OFF					*/
#define D_EQS_STPWV_ANT_DIS_B		0x00000022		/*	ANT#B 閉塞orFault(Disable)		*/
#define D_EQS_STPWV_ANT_PAOFF_B		0x00000002		/*	ANT#B PA OFF					*/
#define D_EQS_STPWV_ANT_DIS_C		0x00000044		/*	ANT#C 閉塞orFault(Disable)		*/
#define D_EQS_STPWV_ANT_PAOFF_C		0x00000004		/*	ANT#C PA OFF					*/
#define D_EQS_STPWV_ANT_DIS_D		0x00000088		/*	ANT#D 閉塞orFault(Disable)		*/
#define D_EQS_STPWV_ANT_PAOFF_D		0x00000008		/*	ANT#D PA OFF					*/

#define D_EQS_STPWV_SV01INF_FCT_DIS		D_DU_SVCTL_BIT_ADM_LOCKED_ALL						/*	locked										*/
#define D_EQS_STPWV_SV05INF_FCT_DIS		( D_DU_ALMBIT_FLT01_ALL | D_DU_ALMBIT_FLT02_ALL )	/*	soft ALM & Initialization failure			*/
#define D_EQS_STPWV_SV06INF_PTP_DIS		( D_DU_ALMBIT_FLT03_ALL )							/*	PTP UnLock									*/
#define D_EQS_STPWV_SV06INF_FCT_DIS		( D_DU_ALMBIT_FLT06_ALL | D_DU_ALMBIT_FLT04_ALL )	/*	Module Clock Faulty & PS ALM				*/
#define D_EQS_STPWV_SV07INF_FCT_DIS		( D_DU_ALMBIT_FLT07_ALL )							/*	Module Clock Faulty & PS ALM				*/
#define D_EQS_STPWV_SV08INF_FCT_DIS_AB	( D_DU_ALMBIT_RFIC1_ALM )							/*	RF module fault(RFIC#1)						*/
#define D_EQS_STPWV_SV08INF_FCT_DIS_CD	( D_DU_ALMBIT_RFIC2_ALM )							/*	RF module fault(RFIC#2)						*/
#define D_EQS_STPWV_SV09INF_FCT_DIS_A	( D_DU_ALMBIT_TX_GAIN_ALM_ANT_A | D_DU_ALMBIT_TOVER_ALM_ANT_A |\
										  D_DU_ALMBIT_RX_GAIN_ALM_ANT_A | D_DU_ALMBIT_LNA_ABNORMAL_ANT_A |\
										  D_DU_ALMBIT_VSWR_ALM_ANT_A | D_DU_ALMBIT_TX_CAL_FAIL_ANT_A |\
										  D_DU_ALMBIT_RX_CAL_FAIL_ANT_A)					/*	TX out of order & RX out of order(ANT#A)	*/
#define D_EQS_STPWV_SV09INF_FCT_DIS_B	( D_DU_ALMBIT_TX_GAIN_ALM_ANT_B | D_DU_ALMBIT_TOVER_ALM_ANT_B |\
										  D_DU_ALMBIT_RX_GAIN_ALM_ANT_B | D_DU_ALMBIT_LNA_ABNORMAL_ANT_B |\
										  D_DU_ALMBIT_VSWR_ALM_ANT_B | D_DU_ALMBIT_TX_CAL_FAIL_ANT_B |\
										  D_DU_ALMBIT_RX_CAL_FAIL_ANT_B)					/*	TX out of order & RX out of order(ANT#B)	*/
#define D_EQS_STPWV_SV10INF_FCT_DIS_C	( D_DU_ALMBIT_TX_GAIN_ALM_ANT_A | D_DU_ALMBIT_TOVER_ALM_ANT_A |\
										  D_DU_ALMBIT_RX_GAIN_ALM_ANT_A | D_DU_ALMBIT_LNA_ABNORMAL_ANT_A |\
										  D_DU_ALMBIT_VSWR_ALM_ANT_A | D_DU_ALMBIT_TX_CAL_FAIL_ANT_A |\
										  D_DU_ALMBIT_RX_CAL_FAIL_ANT_A)					/*	TX out of order & RX out of order(ANT#C)	*/
#define D_EQS_STPWV_SV10INF_FCT_DIS_D	( D_DU_ALMBIT_TX_GAIN_ALM_ANT_B | D_DU_ALMBIT_TOVER_ALM_ANT_B |\
										  D_DU_ALMBIT_RX_GAIN_ALM_ANT_B | D_DU_ALMBIT_LNA_ABNORMAL_ANT_B |\
										  D_DU_ALMBIT_VSWR_ALM_ANT_B | D_DU_ALMBIT_TX_CAL_FAIL_ANT_B |\
										  D_DU_ALMBIT_RX_CAL_FAIL_ANT_B)					/*	TX out of order & RX out of order(ANT#D)	*/
#define D_EQS_STPWV_SV11INF_FCT_DIS_A	( D_DU_ALMBIT_TX_PW_ALM_V1 | D_DU_ALMBIT_TX_PW_ALM_V2 |\
										  D_DU_ALMBIT_TX_PW_ALM_V3 | D_DU_ALMBIT_TX_PW_ALM_V4 |\
										  D_DU_ALMBIT_TX_PW_ALM_V5 | D_DU_ALMBIT_TX_PW_ALM_V6 |\
										  D_DU_ALMBIT_TX_PW_ALM_V7 | D_DU_ALMBIT_TX_PW_ALM_V8 |\
										  D_DU_ALMBIT_TX_PW_ALM_V9 | D_DU_ALMBIT_TX_PW_ALM_V10 |\
										  D_DU_ALMBIT_TX_PW_ALM_V11 | D_DU_ALMBIT_TX_PW_ALM_V12 |\
										  D_DU_ALMBIT_TX_PW_ALM_V13 | D_DU_ALMBIT_TX_PW_ALM_V14 |\
										  D_DU_ALMBIT_TX_PW_ALM_V15 | D_DU_ALMBIT_TX_PW_ALM_V16 )	/*	TX out of order(ANT#A)				*/
#define D_EQS_STPWV_SV11INF_FCT_DIS_B	( D_DU_ALMBIT_TX_PW_ALM_H1 | D_DU_ALMBIT_TX_PW_ALM_H2 |\
										  D_DU_ALMBIT_TX_PW_ALM_H3 | D_DU_ALMBIT_TX_PW_ALM_H4 |\
										  D_DU_ALMBIT_TX_PW_ALM_H5 | D_DU_ALMBIT_TX_PW_ALM_H6 |\
										  D_DU_ALMBIT_TX_PW_ALM_H7 | D_DU_ALMBIT_TX_PW_ALM_H8 |\
										  D_DU_ALMBIT_TX_PW_ALM_H9 | D_DU_ALMBIT_TX_PW_ALM_H10 |\
										  D_DU_ALMBIT_TX_PW_ALM_H11 | D_DU_ALMBIT_TX_PW_ALM_H12 |\
										  D_DU_ALMBIT_TX_PW_ALM_H13 | D_DU_ALMBIT_TX_PW_ALM_H14 |\
										  D_DU_ALMBIT_TX_PW_ALM_H15 | D_DU_ALMBIT_TX_PW_ALM_H16 )	/*	TX out of order(ANT#B)				*/
#define D_EQS_STPWV_SV13INF_FCT_DIS		( D_DU_ALMBIT_TEMP_ALM_HIGH )						/*	Temperature too high						*/
#define D_EQS_STPWV_SV14INF_FCT_OFF		( D_DU_ALMBIT_TX_SLEEP | D_DU_ALMBIT_TX_OFF )		/*	Tx Sleep & Tx OFF							*/
#define D_EQS_STPWV_SV14INF_FCT_DIS		( D_DU_ALMBIT_FORCE_ALM )							/*	Forced alarm								*/
#define D_EQS_STPWV_SV16INF_FCT_OFF		( D_DU_SVCTL_WAVE_STOP )							/*	PTP, SyncE									*/
#define D_EQS_STPWV_SVINF_NONFCT		( 0x00000000 )										/*	要因無し									*/


enum
{
	E_EQS_TCX_TDANTEN = 0,						/*	TDD Enable		*/
	E_EQS_CA_TXANTEN,							/*	停波レジスタ	*/
	E_EQS_STPWV_REG_MAX
};

enum
{
	E_EQS_STPWVMSK_OFF = 0,						/*	停波マスク解除	*/
	E_EQS_STPWVMSK_ON,							/*	停波マスク		*/
	E_EQS_STPWVMSK_MAX
};

enum
{
	E_EQS_STPWV_LOCKED = 0,		/*	(00)locked										*/
	E_EQS_STPWV_SFTDALM,		/*	(01)soft ALM									*/
	E_EQS_STPWV_PTPFLT,			/*	(02)PTP UnLock									*/
	E_EQS_STPWV_CLKFLT,			/*	(03)Module Clock Faulty & PS ALM				*/
	E_EQS_STPWV_HWFLT,			/*	(04)common hardware fault						*/
	E_EQS_STPWV_RF1FLT,			/*	(05)RF module fault(RFIC#1)						*/
	E_EQS_STPWV_RF2FLT,			/*	(06)RF module fault(RFIC#2)						*/
	E_EQS_STPWV_ANTAFLT,		/*	(07)TX out of order & RX out of order(ANT#A)	*/
	E_EQS_STPWV_ANTBFLT,		/*	(08)TX out of order & RX out of order(ANT#B)	*/
	E_EQS_STPWV_ANTCFLT,		/*	(09)TX out of order & RX out of order(ANT#C)	*/
	E_EQS_STPWV_ANTDFLT,		/*	(10)TX out of order & RX out of order(ANT#D)	*/
	E_EQS_STPWV_ANTAFLT_V,		/*	(11)TX out of order(tx power detector ALM V#n)	*/
	E_EQS_STPWV_ANTDFLT_H,		/*	(12)TX out of order(tx power detector ALM H#n)	*/
	E_EQS_STPWV_TMPHIGH,		/*	(13)Temperature too high						*/
	E_EQS_STPWV_PAOFF,			/*	(14)Tx Sleep & Tx OFF							*/
	E_EQS_STPWV_FRCEALM,		/*	(15)Forced alarm								*/
	E_EQS_STPWV_PTP,			/*	(16)PTP, SyncE									*/
	E_EQS_STPWV_FCT_MAX			/*	(17)MAX Number									*/
};

/** @name alarm triggering count define */
/* @{ */
#define D_RSV_ALMCNT_PLLNG		30
#define D_RSV_ALMCNT_FPGANG1	6
#define D_RSV_ALMCNT_FPGANG2    6
#define D_RSV_ALMCNT_FPGANG3	6
#define D_RSV_ALMCNT_FPGAIFNG2	3
#define D_RSV_ALMCNT_CPLDIF1	3
#define D_RSV_ALMCNT_FAN		10
#define D_RSV_ALMCNT_CASCD		3
#define D_RSV_ERRCNT_AISGPOW	3
#define D_RSV_ERRCNT_FANPOW		5 /* 3→5 4TRx */
#define D_RSV_ERRCNT_FANDISC	3 /* 4TRx */
/* @} */

/** @name error factor define */
/* @{ */
/* 1s周期で監視するERR */
#define D_RSV_ERRFACT_FAN1		0
#define D_RSV_ERRFACT_FAN2		1
#define D_RSV_ERRFACT_FAN3		2/* 4TRx */
#define D_RSV_ERRFACT_FAN4		3/* 4TRx */
#define D_RSV_ERRFACT_FAN5		4/* 4TRx */
#define D_RSV_ERRFACT_FAN6		5/* 4TRx */
#define D_RSV_ERRFACT_AISGPOW	6/* 2→6 4TRx */
/* 200ms周期で監視するERR */
#define D_RSV_ERRFACT_FANPOW	0/* 3→0 4TRx */
#define D_RSV_ERRFACT_FANDISC	1/* 4TRx */
/* @} */

/** @name FAN alarm threshold define */
/* @{ */
#define D_RSV_FANALM_THRSHD		64/* 180では早すぎでALMとなる64(0x40)に変更 */
/* @} */

/** @name FAN control define 4TRx-RRE */
/* @{ */
#define D_RSV_FANCNT_DISABLE	0
#define D_RSV_FANCNT_ENABLE		1
#define D_RSV_FANCNT_STOP		0
#define D_RSV_FANCNT_LOW		1
#define D_RSV_FANCNT_HIGH		2
#define D_RSV_FANCNT_TEMP_STOPLOW	72
#define D_RSV_FANCNT_TEMP_SLOWHIGH	77
#define D_RSV_FANCNT_TEMP_LOWSTOP	48
#define D_RSV_FANCNT_TEMP_HIGHLOW	68
/* @} */

#define D_SYS_SVCTL_INS			1
#define D_SYS_SVCTL_NON			0

enum
{
	E_EQS_LED1 = 0,				/* LED1側設定 					*/
	E_EQS_LED2,					/* LED2側設定					*/
	E_EQS_LED3,					/* LED3側設定 BDE-EX			*/
	E_EQS_LED4,					/* LED4側設定 BDE				*/
	E_EQS_LED5,					/* LED5側設定 RE				*/
	E_EQS_LED_MAX
};

#define D_API_EQS_LEDSTATE_POWERON			 1
#define D_API_EQS_LEDSTATE_OPERATION		 2
#define D_API_EQS_LEDSTATE_USE				 3
#define D_API_EQS_LEDSTATE_BUSY				 4
#define D_API_EQS_LEDSTATE_ALM_MINOR		 5
#define D_API_EQS_LEDSTATE_ALM_MAJOR		 6
#define D_API_EQS_LEDSTATE_ALM_CRITICAL		 7
#define D_API_EQS_LEDSTATE_MAINT_FLDL		 8
#define D_API_EQS_LEDSTATE_ADMINLOCK		 9
#define D_API_EQS_LEDSTATE_UNDECIDED		10

#define D_EQS_MDLNM_CNT						14		/*	モデル名の文字数	*/

/* ======================================================================== */
/* LED setting value define													*/
/* ======================================================================== */
#define D_EQS_LED1SET_OFF				0					/* LED1: OFF					*/
#define D_EQS_LED1SET_REDON				0x00000001			/* LED1: RED ON					*/
#define D_EQS_LED1SET_GRNON				0x00000002			/* LED1: GREEN ON				*/
#define D_EQS_LED1SET_REDBLNK			0x00000005			/* LED1: RED Blink				*/
#define D_EQS_LED1SET_GRNBLNK			0x00000006			/* LED1: GREEN Blink			*/
#define D_EQS_LED1SET_REDGRN			0x00000007			/* LED1: RED/GREEN Blink		*/
#define D_EQS_LED1SET_PREVAL			0x0000000F			/* LED1: previous value(NotSettingValue)*/

#define D_EQS_LED2SET_OFF				0					/* LED2: OFF					*/
#define D_EQS_LED2SET_ORGON				0x00000010			/* LED2: ORANGE ON				*/
#define D_EQS_LED2SET_GRNON				0x00000020			/* LED2: GREEN ON				*/
#define D_EQS_LED2SET_ORGBLNK			0x00000050			/* LED2: ORANGE Blink			*/
#define D_EQS_LED2SET_GRNBLNK			0x00000060			/* LED2: GREEN Blink			*/
#define D_EQS_LED2SET_ORGGRN			0x00000070			/* LED2: ORANGE/GREEN Blink		*/
#define D_EQS_LED2SET_PREVAL			0x000000F0			/* LED2: previous value(NotSettingValue)*/

#define D_EQS_LED1_MASK					0x00000007
#define D_EQS_LED2_MASK					0x00000070

#define	D_EQS_PHYSICAL_IDX				1

#define	D_EQS_ASCII_MAX					0x80

#define D_EQS_RST_CAUSE_CU				0x00				/* CU rpc resetコマンド契機						*/
#define D_EQS_RST_CAUSE_TO				0x01				/* supervision wathcdog timerタイムアウト契機	*/

#define	D_EQS_RST_POWEROFF				0x00				/* ソフトリセット無し(PowerOffとみなす)			*/
#define	D_EQS_RST_CMDRESET				0x01				/* ソフトリセットあり(リセットコマンド)			*/
#define	D_EQS_RST_CURESET				0x02				/* ソフトリセットあり(CUからのリセット)			*/
#define	D_EQS_RST_HARDALM				0x03				/* ALMによる自律リセット						*/
#define	D_EQS_RST_MPLNTO				0x04				/* supervision wathcdog timerタイムアウト		*/
#define	D_EQS_RST_WDTTO					0x05				/* wathcdog timerタイムアウト					*/
#define	D_EQS_RST_FACTRY				0xFF				/* 工場出荷										*/

#define D_EQS_FM_ALM_NON				0x00000000			/* アラーム発生カウンタの初期値					*/
#define	D_EQS_FM_ALM_ON					0x00000001			/* FAULTアラーム ON								*/
#define	D_EQS_FM_ALM_OFF				0x00000000			/* FAULTアラーム OFF							*/

#define D_EQS_FM_USED_FLG_OFF			0x00000000			/* アラーム要因未発生							*/
#define D_EQS_FM_USED_FLG_ON			0x00000001			/* アラーム要因発生								*/

#define D_EQS_REQ_NOTIF					0x00000000

#define	D_EQS_BOOL_TRUE					1
#define	D_EQS_BOOL_FALSE				0

#define	D_EQS_CHKMODE_FAULTON			0
#define	D_EQS_CHKMODE_FAULTOFF			1

/*!
 * @name 5GDU状態
 * @note 
 * @{
 */
enum
{
	E_EQS_STS_OPER = 0,			/*	Operational State	*/
	E_EQS_STS_ADMIN,			/*	Admin State			*/
	E_EQS_STS_USAGE,			/*	usage State			*/
	E_EQS_STS_AVAIL,			/*	availability State	*/
	E_EQS_STS_POWER,			/*	power State			*/
	E_EQS_STS_MAX
};

enum
{
	E_EQS_FALT_FID01_CRITI = 0,							/*!< (00)Fault大項目種別 = Soft ALM							*//*	Critical	*/
	E_EQS_FALT_FID02_CRITI,								/*!< (01)Fault大項目種別 = Initialization Error				*//*	Critical	*/
	E_EQS_FALT_FID03_MAJOR,								/*!< (02)Fault大項目種別 = Fronthaul Synchronization Error	*//*	Major		*/
	E_EQS_FALT_FID04_CRITI,								/*!< (03)Fault大項目種別 = Module Clock Faulty				*//*	Critical	*/
	E_EQS_FALT_FID05_MINOR,								/*!< (04)Fault大項目種別 = Fronthaul faulty					*//*	Minor		*/
	E_EQS_FALT_FID06_MAJOR,								/*!< (05)Fault大項目種別 = PS ALM							*//*	Major		*/
	E_EQS_FALT_FID07_CRITI,								/*!< (06)Fault大項目種別 = common hardware (module) fault	*//*	Critical	*/
	E_EQS_FALT_FID08_MAJOR,								/*!< (07)Fault大項目種別 = RF module fault					*//*	Major		*/
	E_EQS_FALT_FID10_ANTA_MAJOR,						/*!< (08)Fault大項目種別 = TX out of order(AntA)			*//*	Major		*/
	E_EQS_FALT_FID10_ANTB_MAJOR,						/*!< (09)Fault大項目種別 = TX out of order(AntB)			*//*	Major		*/
	E_EQS_FALT_FID10_ANTC_MAJOR,						/*!< (10)Fault大項目種別 = TX out of order(AntC)			*//*	Major		*/
	E_EQS_FALT_FID10_ANTD_MAJOR,						/*!< (11)Fault大項目種別 = TX out of order(AntD)			*//*	Major		*/
	E_EQS_FALT_FID11_ANTA_MAJOR,						/*!< (12)Fault大項目種別 = RX out of order(AntA)			*//*	Major		*/
	E_EQS_FALT_FID11_ANTB_MAJOR,						/*!< (13)Fault大項目種別 = RX out of order(AntB)			*//*	Major		*/
	E_EQS_FALT_FID11_ANTC_MAJOR,						/*!< (14)Fault大項目種別 = RX out of order(AntC)			*//*	Major		*/
	E_EQS_FALT_FID11_ANTD_MAJOR,						/*!< (15)Fault大項目種別 = RX out of order(AntD)			*//*	Major		*/
	E_EQS_FALT_FID12_ANTA_MAJOR,						/*!< (16)Fault大項目種別 = ANT port  abnormal(AntA)			*//*	Major		*/
	E_EQS_FALT_FID12_ANTB_MAJOR,						/*!< (17)Fault大項目種別 = ANT port  abnormal(AntB)			*//*	Major		*/
	E_EQS_FALT_FID12_ANTC_MAJOR,						/*!< (18)Fault大項目種別 = ANT port  abnormal(AntC)			*//*	Major		*/
	E_EQS_FALT_FID12_ANTD_MAJOR,						/*!< (19)Fault大項目種別 = ANT port  abnormal(AntD)			*//*	Major		*/
	E_EQS_FALT_FID30_MAJOR,								/*!< (20)Fault大項目種別 = Temperature too low				*//*	Major		*/
	E_EQS_FALT_FID31_CRITI,								/*!< (21)Fault大項目種別 = Temperature too high				*//*	Critical	*/
	E_EQS_FALT_FID40_MINOR,								/*!< (22)Fault大項目種別 = No fan detected					*//*	Minor		*/
	E_EQS_FALT_FID41_MINOR,								/*!< (23)Fault大項目種別 = FAN PS ALM						*//*	Minor		*/
	E_EQS_FALT_FID42_MINOR,								/*!< (24)Fault大項目種別 = Cooling fan broken FAN#1			*//*	Minor		*/
	E_EQS_FALT_FID43_MINOR,								/*!< (25)Fault大項目種別 = Cooling fan broken FAN#2			*//*	Minor		*/
	E_EQS_FALT_FID44_MINOR,								/*!< (26)Fault大項目種別 = Cooling fan broken FAN#3			*//*	Minor		*/
	E_EQS_FALT_FID50_MINOR,								/*!< (27)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
	E_EQS_FALT_FIDF0_MINOR,								/*!< (28)Fault大項目種別 = Tx Sleep							*//*	Minor		*/
	E_EQS_FALT_FIDF1_MINOR,								/*!< (29)Fault大項目種別 = Tx OFF							*//*	Minor		*/
	E_EQS_FALT_FIDF2_MINOR,								/*!< (30)Fault大項目種別 = TEST Mode						*//*	Minor		*/
	E_EQS_FALT_FIDFF_CRITI,								/*!< (31)Fault大項目種別 = Forced alarm						*//*	Critical	*/
	E_EQS_FALT_FID51_MINOR,								/*!< (32)Fault大項目種別 = SFP Failsafe						*//*	Minor		*/
	E_EQS_FALT_FIDEA60_MINOR,							/*!< (33)Fault大項目種別 = SFP Type Error					*//*	Minor		*/
	E_EQS_FAULTID_MAX
};

enum
{
	E_EQS_DEVKIND_SB6 = 0,		/*	(00)sub6										*/
	E_EQS_DEVKIND_MMW,			/*	(01)mmW									*/
	E_EQS_DEVKIND_MAX			/*	(02)MAX Number									*/
};

#define	D_EQS_MAJOR_OUTOFPTP_FLTBIT		(D_DU_SVCTL_BIT_OCCUR_FID_06 | D_DU_SVCTL_BIT_OCCUR_FID_08 | D_DU_SVCTL_BIT_OCCUR_FID_10 |\
										 D_DU_SVCTL_BIT_OCCUR_FID_11 | D_DU_SVCTL_BIT_OCCUR_FID_12 | D_DU_SVCTL_BIT_OCCUR_FID_30)
#define	D_EQS_CRITI_FLTBIT				(D_DU_SVCTL_BIT_OCCUR_FID_01 | D_DU_SVCTL_BIT_OCCUR_FID_02 | D_DU_SVCTL_BIT_OCCUR_FID_04 |\
										 D_DU_SVCTL_BIT_OCCUR_FID_07 | D_DU_SVCTL_BIT_OCCUR_FID_31 | D_DU_SVCTL_BIT_OCCUR_FID_FF)

#define	D_PARAM_NUM_MAX						8								/**<	パラメータ文字数最大値		*/

#endif	/* F_EQQS_DEF */

