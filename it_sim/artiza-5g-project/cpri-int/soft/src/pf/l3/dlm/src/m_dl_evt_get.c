/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   m_dl_evt_get.c
 * @brief  イベント取得
 * @date   2015/09/04 FPT)DuongCD Create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "m_cm_header.h"				/* MDIF common head file			*/
#include "m_dl_header.h"				/* download management task head file */


/****************************************************************************/
/*!
 * @brief  イベント取得
 * @note   イベントを取得する
 * @param  awSysType [in] system type
 * @return イベント
 * @date   2015/09/04 FPT)DuongCD Create
 */
/****************************************************************************/
EC_DLM_MAIN_EVT m_dl_evt_get(
	USHORT awSysType
) {
	
    EC_DLM_MAIN_EVT evt;
    
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_evt_get] ");
	
    /*******************/
    /* イベント取得 */
    /*******************/
    evt = ge_dlw_main_event[awSysType];

    return evt;
}


/****************************************************************************/
/*
 */
/****************************************************************************/
