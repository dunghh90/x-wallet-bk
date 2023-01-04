/*!
 * @skip  $ld:$
 * @file  f_frw_ram.c
 * @brief RAM table definition.
 * @date 2013/11/14 FFCS)linb create
 * @date 2015/08/13 TDIPS)sasaki delete flashReadRsp(unused variable)
 * @date 2015/08/26 TDIPS)satou 未使用変数削除(f_frww_thrID)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_frw_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

/*!
 * @brief	pf_frw thread state
 * @note	pf_frw thread state
 * 
 */
UINT	f_frww_thrState;

/*!
 * @brief   response queue id
 */
UINT 	f_frww_respQid;

/*!
 * @brief   圧縮情報
 */
UCHAR	f_frww_compressData[D_FRW_COMPDATA];

/*!
 * @brief   REファイル情報
 */
T_RRH_FW_INFO	f_frww_fwInfo[D_RRH_CPRINO_RE_MAX];

/*!
 * @brief   running history
 */
T_FRW_RUNHIS_TBL	f_frww_runhis_tbl ;

/*!
 * @brief   スレッド内アサートログ情報
 * @note    スレッド初期化処理で初期化する(f_cmn_com_assert_th_create())
 */
VOID*	f_frww_assert_p;

/*! @} */
