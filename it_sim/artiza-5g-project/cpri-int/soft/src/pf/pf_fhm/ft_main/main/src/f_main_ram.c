/*!
 * @skip		$ld:$
 * @file		f_main_ram.c
 * @brief		pf_main RAMテーブル
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date        2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include	"f_main_inc.h"

/**
 * @brief  thread state management table
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
USHORT f_mainw_thdmng_state;

/**
 * @brief  thread initializing manage table
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
USHORT f_mainw_thdIniMng[D_SYS_THDQNO_PF_MAX];

/**
 * @brief  thread ID
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
T_MAIN_THDID f_mainw_thdId;

/**
 * @brief  inventory manage table
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
T_RRH_TRAINVTBL f_mainw_inv_mng;

/**
 * @brief  set signal date
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
sigset_t f_mainw_sigSetVal;

/**
 * @brief  alarm log function list
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;

/**
 * @brief  圧縮データ用元データ
 * @note   [ 値の説明 ]
 * @note   [ 用途     ]
 * @note   [ 注意事項 ]
 * @note   [ 更新契機 ]
 */
UCHAR f_mainw_orgData[D_RRH_FLASH_LOG_SIZE];

/**
 * @brief   初期化完了通知受信管理テーブル
 * @date    2015/08/03 TDIPS) maruyama modify FHMで使用しないスレッドをD_MAIN_THD_STS_COMPに変更
 * @date    2015/08/03 TDIPS) maruyama modify FRQREスレッド追加
 * @TBD_No FHMで起動対象ではなくなったスレッドについてD_MAIN_THD_STS_COMPで初期化しないといけない
 */
UINT	f_mainw_uiQueChgTDD[D_SYS_THDQNO_PF_MAX] = {
													D_MAIN_THD_STS_COMP,
													D_MAIN_THD_STS_COMP,		/**< 1:PF Main Thread									*/
													0,                          /**< 2:handle Main Thread								*/
													D_MAIN_THD_STS_COMP,		/**< 3:-												*/
													D_MAIN_THD_STS_COMP,		/**< 4:-												*/
													D_MAIN_THD_STS_COMP,		/**< 5:-												*/
													D_MAIN_THD_STS_COMP,		/**< 6:-												*/
													D_MAIN_THD_STS_COMP,		/**< 7:-												*/
													0,                          /**< 8:PF Receive Thread(プロセス間受信部)  CMU-RCV		*/
												 	0,                          /**< 9:PF Send Thread(プロセス間送信部) CMU-SEND		*/
													0,                          /**< 10:Ethernet Set Thread								*/
													0,                          /**< 11:CPRI Management Thread							*/
													0,                          /**< 12:RE Supervision Thread							*/
													D_MAIN_THD_STS_COMP,        /**< 13:rfcp Thread										*/
													0,                          /**< 14:DPD thread										*/
													0,                          /**< 15:CPRI Auto Nego Thread							*/
													D_MAIN_THD_STS_COMP,		/**< 16:-												*/
												 	0,                          /**< 17:Trouble Log Thread								*/
													D_MAIN_THD_STS_COMP,        /**< 18:Diagnosis Thread								*/
													0,                          /**< 19:fmg Thread										*/
													0,                          /**< 20:flash(QSPI) writing Thread						*/
													0,                          /**< 21:Watch Dog Timer Thread							*/
													0,                          /**< 22:dbg Thread										*/
													0,							/**< 23:flash(SLAVE FLASH QSPI) writing Thread			*/
													0							/**< flash(QSPI) writing （for RE)Thread				*/
													};

VOID*					f_mainw_assert_p;							/**< main thread assert pointer */
UINT                    f_mainw_reboot_status;                      /**< Reset reason */

/**
 * メモリ診断用領域.
 * DDR3 Memoryのアクセスチェックに使用する.
 * 0x5555_5555/0xAAAA_AAAAを書き込み後、読出し値が書込み値を一致していることを確認する.
 * ハードソフト仕様書で「空き領域を対象とする(範囲等はソフト一任)」とあるためグローバル変数で領域を用意.
 */
INT f_mainw_diagmem;

/** @} */

