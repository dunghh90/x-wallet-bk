/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_ram.c
 *  @brief  CPRI management RAM table define
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理  header file */

/** @name CPRI management task state */
/* @{ */

/*	task state definition		*/
USHORT   crw_taskstate ;
/* @} */

/** @name CPRI management task event */
/* @{ */

/*	task event definition*/
UINT     crw_taskevent ;
/* @} */

/**
 * @brief   2次リセット対象
 * @note    [ 値の説明 ] 2次リセット対象をbitで表す. bit0:RE#1, bit2:RE#2 ...
 * @note    [ 用途     ] 「RE CPRI Link Start Notice」受信時に配下REに2次リセットを送信するか判定するため
 * @note    [ 注意事項 ]
 * @note    [ 更新契機 ] 以下の契機で更新する.
 *                       - スレッド起動時にRE使用/未使用が「使用」のREについてbitをセット
 *                       - REプロセスに2次リセット要求送信時にbitをクリア
 */
UINT crw_poweron_reset_target;

/** @name CPRI management task runhistory table */
/* @{ */

/************************/
/* 走行履歴情報テーブル */
/************************/
CRT_RUNHIS_TBL	crw_runhis_tbl[D_RRH_CPRINO_NUM] ;
USHORT     crw_hissetflag ;
/* @} */

/** @name Health check  management table */
/* @{ */
CRT_HC_CTL_TBL		crw_hc_ctl_tbl[CMD_MAX_SYSNUM];
/* @} */

/* @{ */
CRT_HC_CTL_MST_TBL		crw_hc_ctl_mst_tbl[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];
/* @} */

/* @} */

