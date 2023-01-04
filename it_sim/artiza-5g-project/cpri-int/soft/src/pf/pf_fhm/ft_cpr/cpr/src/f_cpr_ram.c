/*!
 * @skip  $ld:$
 * @file  f_cpr_ram.c
 * @brief The process related to MT Button behaviour.
 * @date 2013/11/14 FFCS)niemsh create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
    
/*!
 * @addtogroup RRH_PF_CPR
 * @{
 */

/*!
 * @brief CPRI task state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_thrdState;

/*!
 * @brief CPRI task external event number
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_extEventNo;

/*!
 * @brief
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_cprStatChgNtc_QID[4];

/*!
 * @brief 共有メモリアドレス(CPRI L1統計情報)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_RRH_L1STAT_TBL *f_cprw_l1StatTbl;

/*!
 * @brief 共有メモリアドレス(CPRI L1リンク障害ログ)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_RRH_L1ERR_TBL *f_cprw_l1ErrTbl;

/*!
 * @brief 共有メモリアドレス(CPRI L1復旧断履歴)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_RRH_L1LOSLOF_TBL *f_cprw_l1LosLofTbl;

/*!
 * @brief running history
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_CPR_RUNHIS_TBL f_cprw_runhis_tbl[D_RRH_CPRINO_NUM];

/*!
 * @brief Auto Nego Complete flag
 * @note  [ 値の説明 ] 未完了 = D_SYS_OFF, 完了 = D_SYS_ON
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_autonego_complete_flag;
#ifdef OPT_RRH_ZYNQ_REC
UINT f_cprw_autonego_complete_flag_RE[D_RRH_CPRINO_RE_MAX];
#endif

/*!
 * @brief real CPRI Bit Rate
 * @note  [ 値の説明 ] 以下の値をとる
 *                     - 2.4G = D_COM_LINE_BITRATE_24G
 *                     - 4.9G = D_COM_LINE_BITRATE_49G
 *                     - 9.8G = D_COM_LINE_BITRATE_98G
 * @note  [ 用途     ] SlaveのCPRI startup結果、判明したCPRIビットレート
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_bitrate_real;

/*!
 * @brief HFN count
 * @note  [ 値の説明 ]
 * @note  [ 用途     ] HFNSYNCの安定化考慮時間
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_sync_hfn_count[D_RRH_CPRINO_NUM];

/*!
 * @brief HFN count
 * @note  [ 値の説明 ]
 * @note  [ 用途     ] master のリンクアップタイマー
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_sync_hfn_linkup[D_RRH_CPRINO_NUM];

/*!
 * @brief SFP support 品種
 * @note  [ 値の説明 ] サポート = D_RRH_ON, サポート外 = D_RRH_OFF
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_support_sfp[D_RRH_CPRINO_NUM];

/*!
 * @brief SFP 9.8Gサポート判定
 * @note  [ 値の説明 ] サポート = D_RRH_ON, サポート外 = D_RRH_OFF
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_support_98g[D_RRH_CPRINO_NUM];

/*!
 * @brief MaterCPRIリンクのFHN sync監視タイマ
 * @note  [ 値の説明 ] タイマー生成中 = D_RRH_ON, タイマ停止中 = D_RRH_OFF
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT	f_cprw_masterLinkUpTimerNum = 0;
UINT	f_cprw_masterLinkUpTimerCnt[D_RRH_CPRINO_RE_MAX] = {0};
T_RRH_SFP_LOG_ALL *f_cprw_shm_sfplog = NULL;

/*!
 * @brief Slave側のCPRI プロトコルVer とHDLCレートのレジスタ値
 * @note  [ 値の説明 ] 
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_cprw_SlaveCPPVer_HDLCRate = 0;

/*! @} */
