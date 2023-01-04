/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_cm_DLEnd.c
 *  @brief   Reset status	 
 *  @date   2008/07/25 FFCS)zhengmh create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

																	
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/


/********************************************************************************************************************/
/**
 *  @brief    Reset status
 *  @note     Reset status
 *  @param  	-
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/

VOID m_mk_cm_DLEnd( VOID )
{
	/**********************/
	/* テーブルクリア処理 */
	/**********************/
	/* ファイル情報報告テーブルをクリアする */
	cm_MemClr( &cmw_trafile_inf_tbl, sizeof( cmw_trafile_inf_tbl ));
	
	/* ダウンロード管理テーブルをクリアする */
	cm_MemClr( &mkw_dlmng_tbl, sizeof( mkw_dlmng_tbl ));
	
	/* タイマ管理テーブルをクリアする */
	cm_MemClr( mkw_timmng_tbl, sizeof( mkw_timmng_tbl ));

	return ;
}

/* @} */
