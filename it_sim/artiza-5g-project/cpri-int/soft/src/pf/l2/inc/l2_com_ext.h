/*!
 * @skip  $ld:$
 * @file  l2_com_ext.h
 * @brief 外部参照宣言
 * @date  2013/11/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2
 * @{
 */
#ifndef L2_COM_EXT_H
#define L2_COM_EXT_H

/*!
 * @name 関数外部宣言
 * @note 関数外部宣言定義
 * @{
 */

extern T_RRH_L2_STATISTICS_L2TABLE * l2_comw_stati_L2Tbl;

extern T_RRH_L2_STATISTICS_L3TABLE * l2_comw_stati_L3Tbl;

extern T_RRH_L2_LOG_INFO_HEAD * l2_comw_logHead;

extern T_RRH_L2_LOG_INFO_TABLE * l2_comw_logTbl;

extern T_RRH_L2_LOG_INFO_SPEC* l2_comw_logSpec;

extern const T_L2_L3SIGNAMETBL l2_comr_stati_l3sig[D_RRH_L2_STATI_L3SIG_NUM];

extern L2_MKMTMODE l2_dbgw_mkmtmode[D_RRH_CPRINO_NUM];	/* Maker MT mode	*/

/* @} */

#endif
/* @} */
