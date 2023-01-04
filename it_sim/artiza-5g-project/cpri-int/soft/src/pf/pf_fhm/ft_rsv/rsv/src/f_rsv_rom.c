/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_rom.c
 *  @brief  RE supervision ROM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2015/06/17 ALP)Murakami TDD-ZYNQ 対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/


/** TRA SVレジスタ未使用ビットマスクテーブル */
const	USHORT		f_rsvr_tra_msk_tbl[D_RSV_SVTBL_NUM] = {
	
							D_RSV_MSK_TRA0,		/* TRA SVバス「A0」表示		*/
							D_RSV_MSK_TRA1,		/* TRA SVバス「A1」表示		*/
							D_RSV_MSK_TRA2,		/* TRA SVバス「A2」表示		*/
							D_RSV_MSK_TRA3,		/* TRA SVバス「A3」表示		*/
							D_RSV_MSK_TRA4,		/* TRA SVバス「A4」表示		*/
							D_RSV_MSK_TRA5,		/* TRA SVバス「A5」表示		*/
							D_RSV_MSK_TRA6,		/* TRA SVバス「A6」表示		*/
							D_RSV_MSK_TRA7,		/* TRA SVバス「A7」表示		*/
							D_RSV_MSK_TRA8,		/* TRA SVバス「A8」表示		*/
							D_RSV_MSK_TRA9,		/* TRA SVバス「A9」表示		*/
							D_RSV_MSK_TRA10,	/* TRA SVバス「A10」表示	*/
							D_RSV_MSK_TRA11,	/* TRA SVバス「A11」表示	*/
							D_RSV_MSK_TRA12,	/* TRA SVバス「A12」表示	*/
							D_RSV_MSK_TRA13	    /* TRA SVバス「A13」表示	*/
					};

/** 
 * @brief   alarm information 
 * @note    周期監視処理( f_rsv_AlmCyclChk )において検出処理用に使用する
 */
const	T_RSV_ALMINFO f_rsvr_almInfo[D_RSV_ALM_NUM] = {
		/* PLL UNLOCK */
		{
			D_RRH_ALMCD_PLLNG,
			D_RSV_REGTYPE_CPRI,
			D_RRH_REG_CNT_STA1,
			D_RSV_ALMBIT_PLLNG,
			D_RSV_ALMCYCL_100MS,
			&(f_rsvw_almCyclCount[0]),
			D_RSV_ALMCNT_PLLNG,
			&(f_rsvw_almCount[0])
		},
		/* FPGA-PLL-UNLOCK1 */
		{
			D_RRH_ALMCD_FPGANG1,
			D_RSV_REGTYPE_CPRI,
			D_RRH_REG_CNTS_STA1,
			D_RSV_ALMBIT_FPGANG1,
			D_RSV_ALMCYCL_200MS,
			&(f_rsvw_almCyclCount[1]),
			D_RSV_ALMCNT_FPGANG1,
			&(f_rsvw_almCount[1])
		},
		/* FPGA-IF ALM2	*/
		{
			D_RRH_ALMCD_FPGAIF2,
			D_RSV_REGTYPE_CPRI,
			D_RRH_REG_CNT_STA1,
			D_RSV_ALMBIT_FPGAIFNG2,
			D_RSV_ALMCYCL_1S,
			&(f_rsvw_almCyclCount[3]),
			D_RSV_ALMCNT_FPGAIFNG2,
			&(f_rsvw_almCount[3])
		},
        /* CPLD-IF ALM1	*/
		{
			D_RRH_ALMCD_CPLDIF1,
			D_RSV_REGTYPE_CPRI,
			D_RRH_REG_CNT_STA1,
			D_RSV_ALMBIT_CPLDIF1,
			D_RSV_ALMCYCL_1S,
			&(f_rsvw_almCyclCount[4]),
			D_RSV_ALMCNT_CPLDIF1,
			&(f_rsvw_almCount[4])
		}
};

/**
 * @brief LED1 setting value
 * @note  APIで指定されたLED stateをレジスタ設定値(Set Value)に変換する
 * @date  2015/09/04 TDI)satou FHM用の定義に修正
 */
const	T_RSV_LEDINFO f_rsvr_led1Info[3] = {
		/*	LED state,					Set Value	*/
    {   D_RRH_LED1STATE_OFF,            D_RRH_CNT_FHM_LEDSYSR_OFF       },
    {   D_RRH_LED1STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSR_CYCL      },
    {   D_RRH_LED1STATE_ON,             D_RRH_CNT_FHM_LEDSYSR_ON        }
};

/**
 * @brief LED2 setting value
 * @note  APIで指定されたLED stateをレジスタ設定値(Set Value)に変換する
 * @date  2015/09/04 TDI)satou FHM用の定義に修正
 */
const	T_RSV_LEDINFO f_rsvr_led2Info[3] = {
		/*	LED state,					Set Value	*/
    {   D_RRH_LED2STATE_OFF,            D_RRH_CNT_FHM_LEDSYSG1_OFF       },
    {   D_RRH_LED2STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSG1_CYCL      },
    {   D_RRH_LED2STATE_ON,             D_RRH_CNT_FHM_LEDSYSG1_ON        }
};

/**
 * @brief LED3 setting value
 * @note  APIで指定されたLED stateをレジスタ設定値(Set Value)に変換する
 * @date  2015/09/04 TDI)satou FHM用の定義に修正
 */
const   T_RSV_LEDINFO f_rsvr_led3Info[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_LED3STATE_OFF,            D_RRH_CNT_FHM_LEDSYSO_OFF       },
    {   D_RRH_LED3STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSO_CYCL      },
    {   D_RRH_LED3STATE_ON,             D_RRH_CNT_FHM_LEDSYSO_ON        }
};

/**
 * @brief CPRI LINK LED(ACT LED) setting value
 * @note  APIで指定されたLED stateをレジスタ設定値(Set Value)に変換する
 * @date  2015/09/05 TDI)satou create
 */
const   T_RSV_LEDINFO f_rsvr_ledCpriActInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_ACT_OFF,     D_RRH_CNT_FHM_ACTLED_OFF        },
    {   D_RRH_CPRILEDSTATE_ACT_BLNK,    D_RRH_CNT_FHM_ACTLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_ACT_ON,      D_RRH_CNT_FHM_ACTLED_ON         }
};

/**
 * @brief CPRI LINK LED(DWN LED) setting value
 * @note  APIで指定されたLED stateをレジスタ設定値(Set Value)に変換する
 * @date  2015/09/05 TDI)satou create
 */
const   T_RSV_LEDINFO f_rsvr_ledCpriDwnInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_DWN_OFF,     D_RRH_CNT_FHM_DWNLED_OFF        },
    {   D_RRH_CPRILEDSTATE_DWN_BLNK,    D_RRH_CNT_FHM_DWNLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_DWN_ON,      D_RRH_CNT_FHM_DWNLED_ON         }
};

/* @} */
