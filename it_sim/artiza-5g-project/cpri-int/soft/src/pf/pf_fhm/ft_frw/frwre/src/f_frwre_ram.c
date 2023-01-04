/*!
 * @skip  $ld:$
 * @file  f_frwre_ram.c
 * @brief RAM table definition.
 * @date 2015/08/04 TDIPS)sasaki create
 * @date 2015/08/27 TDIPS)sasaki MKレビュー指摘No.116対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_frwre_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

/**
 * @brief pf_frwre thread state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ] f_frwrer_matrix マトリクステーブルのジャンプ先を決定する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] f_frwrer_matrix マトリクステーブルの各種関数にて更新
 */
UINT f_frwrew_thrState;

/**
 * @brief running history table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_FRWRE_RUNHIS_TBL f_frwrew_runhis_tbl;

/**
 * @brief スレッド内アサート情報
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] スレッド初期化処理において初期化し、以降の各種アサート処理で使用
 */
VOID *f_frwrew_assert_p;

/*! @} */
