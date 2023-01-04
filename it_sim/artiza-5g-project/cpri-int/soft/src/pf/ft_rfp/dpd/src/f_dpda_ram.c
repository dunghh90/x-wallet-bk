/**
 *  @skip	$Id:$
 *  @file
 *  @brief	RAM Table
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2015
 */
/******************************************************************************************************************************/

#include	"f_dpda_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

/**
 * @brief register data table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_DPDA_REGDATA f_dpdaw_regdataReal __attribute__ ((aligned (32)));

/**
 * @brief status information テーブル
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_DPDA_STATUS_INFO f_dpdaw_statusInfo __attribute__ ((aligned (32)));

/**
 * @brief alm threshold table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_dpdaw_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];

/**
 * @brief 発生アラームInstanceIDリスト
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_dpdaw_occurAlmInstanceId[D_DPDA_ALM_NUM];

/**
 * @brief 回復アラームInstanceIDリスト
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_dpdaw_clearAlmInstanceId[D_DPDA_ALM_NUM];

/**
 * @brief dpda alarm state table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_DPDA_ALM_STATE f_dpdaw_almState[D_DPDA_ALM_NUM];

/**
 * @brief DPDAデバッグ用情報ログtable
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_DPDA_INFO_ALL *f_dpdaw_info_all;

/**
 * @brief アラーム発生時解析用ログtable
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_DPDA_ALM_LOG_DEBUG *f_dpdaw_almLog;

/**
 * @brief アラームヒストリー用ログ情報格納位置
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_RRH_ALM_LOG_FOR_HIST *f_dpdaw_almLogForHistory;

/**
 * @brief 温度ログ情報格納位置
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_RRH_LOG_TEMP *f_dpdaw_tempLog;
/** @} */
