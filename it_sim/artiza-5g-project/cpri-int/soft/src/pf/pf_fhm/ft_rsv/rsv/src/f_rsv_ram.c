/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_ram.c
 *  @brief  RE supervision RAM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2011/11/24 COMAS)Uchida  M-S3G-eNBSYS-02539対応
 *  @date   2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04092 CR-00054-000 15G-RREのPA OFF問題対応
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/**
 * @brief TRA状態変化検出テーブル(old)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT tra_sv_newsta[D_RSV_SVTBL_NUM];

/**
 * @brief TRA状態変化検出テーブル(new)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT tra_sv_oldsta[D_RSV_SVTBL_NUM];

/**
 * @brief TRA card first active flag
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_rsvw_first_act_flg;

/**
 * @brief TRA card restart both alarm flag
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_rsvw_restart_both_alm_flg;

/**
 * @brief alarm cycle counting table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_rsvw_almCyclCount[D_RSV_ALM_NUM];

/**
 * @brief alarm counting table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_rsvw_almCount[D_RSV_ALM_NUM];

/**
 * @brief response queue id of card state change notification
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_rsvw_cardStaChgNtc_QID[D_SYS_NUM4];

/**
 * @brief thread state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_rsvw_thrdstate;

/**
 * @brief error control table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_rsvw_errctl_tbl;

/*! @} */
