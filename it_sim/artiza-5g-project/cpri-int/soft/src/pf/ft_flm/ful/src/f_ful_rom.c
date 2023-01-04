/*!
 * @skip $Id:$
 * @file f_ful_rom.c
 * @brief FULタスク 内部テーブル
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_ful_inc.h"

/* FDL State Mng */
const	T_FUL_CALLSETRELL_MTXTBL f_ful_callSetRell_mtxtbl[D_FUL_STATENUM][E_FUL_EVE_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/* init */
		{f_ful_init							},					/**< スレッド起動				*/
		{f_ful_abort						},					/**< File-List転送要求			*/
		{f_ful_abort						}					/**< アップロード要求			*/
	},
	{
		/* 運用状態 */
		{f_ful_abort						},					/**< スレッド起動				*/
		{f_ful_filelist_rcv					},					/**< File-List転送要求			*/
		{f_ful_upload_rcv					}					/**< アップロード要求			*/
	}
};

/** @} */

