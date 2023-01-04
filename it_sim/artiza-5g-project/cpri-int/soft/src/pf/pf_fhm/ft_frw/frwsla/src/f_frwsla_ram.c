/*!
 * @skip  $ld:$
 * @file  f_frwsla_ram.c
 * @brief RAM table definition.
 * @date 2014/12/12 FFCS)linb create
 * @date 2015/08/13 TDIPS)sasaki delete f_frwslaw_flashReadRsp(unused variable)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

#include "f_frwsla_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */

/**
 * @brief pf_frwsla thread state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ] f_frwslar_matrix マトリクステーブルのジャンプ先を決定する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] f_frwslar_matrix マトリクステーブルの各種関数にて更新
 */
UINT f_frwslaw_thrState;

/**
 * @brief response queue id
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_frwslaw_respQid;

/**
 * @brief running history table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_FRWSLA_RUNHIS_TBL f_frwslaw_runhis_tbl;

/**
 * @brief スレッド内アサート情報
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] スレッド初期化処理において初期化し、以降の各種アサート処理で使用
 */
VOID *f_frwslaw_assert_p;

/*! @} */
