/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   m_dl_rec_ownstn_get.c
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
 * @param  awSysType [in] system type
 * @param  cpri_no   [in] CPRI link number
 * @return 状態番号
 * @date   2015/09/04 FPT)DuongCD Create
 */
/****************************************************************************/
USHORT m_dl_rec_ownstn_get(
	USHORT awSysType, USHORT cpri_no
) {
	
    USHORT stn;                         /* 状態番号 */

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_ownstn_get] " );
	
    /*******************/
    /* 状態番号取得 */
    /*******************/
	stn = gt_dlw_refw_all_tbl[awSysType][cpri_no - CMD_NUM1].re_sta;
	
    return stn;
}

/****************************************************************************/
/*
 */
/****************************************************************************/
