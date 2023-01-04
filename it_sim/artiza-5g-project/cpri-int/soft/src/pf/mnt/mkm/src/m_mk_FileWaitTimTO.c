/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_FileWaitTimTO.c
 *  @brief   File down wait timer time out
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

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/


/********************************************************************************************************************/
/**
 *  @brief     File down wait timer time out
 *  @note     File down wait timer time out
 *  @param  a_inf_p		[in]	message buffer pointer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/

VOID m_mk_FileWaitTimTO(UINT *a_inf_p)
{	
	/* When timer flag is off */
	if(mkw_timmng_tbl[CMD_NUM0] == CMD_NUM0)
	{
		cm_Assert(CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "File download timer already closed");	
		
		return;
	}
	/* Download status check */
	if(cmw_trafile_inf_tbl.en_flg != CMD_TDL_ON)
	{
		/* Not Downloading */
		cm_Assert(CMD_ASL_DBGLOW, cmw_trafile_inf_tbl.en_flg, "Downlaoding is not started");
		
		return;
	}

	/********************************/
	/*ファイルダウンロード完了応答  */
	/********************************/
	m_mk_DataEndNtcRsp(CMD_NUM0, MKD_RES_TIMEOUT);
	/* Clear the status */
	m_mk_cm_DLEnd();

	return;
	
}
/* @} */
