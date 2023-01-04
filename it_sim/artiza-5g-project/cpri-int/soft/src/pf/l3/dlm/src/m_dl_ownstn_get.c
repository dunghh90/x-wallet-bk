/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   m_dl_ownstn_get.c
 * @brief  自状態番号取得
 * @date   2015/09/04 FPT)DuongCD Create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "m_cm_header.h"				/* MDIF common head file			*/
#include "m_dl_header.h"				/* download management task head file */


/****************************************************************************/
/*!
 * @brief  自状態番号取得
 * @note   自状態番号を取得する
 * @param  awSysType [in] system type
 * @return 状態番号
 * @date   2015/09/04 FPT)DuongCD Create
 */
/****************************************************************************/
EC_DLM_MAIN_STN m_dl_ownstn_get(
	USHORT awSysType
) {
	
    EC_DLM_MAIN_STN stn;                         /* 状態番号 */

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_ownstn_get] " );
	
    /*******************/
    /* 状態番号取得 */
    /*******************/
	stn = (*gt_dlw_rec_all_tbl[awSysType]).rec_sta;
	
    return stn;
}

/****************************************************************************/
/*
 */
/****************************************************************************/
