/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TRAFLWRTTO.c
 *  @brief   TRA file write timer time out
 *  @date   2008/07/25  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

/********************************************************************************************************************/
/**
 *  @brief     TRA file write timer time out
 *  @note     TRA file write timer time out
 *  @param  a_inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 */
/********************************************************************************************************************/



VOID m_mk_TRAFLWRTTO(UINT *a_inf_p)
{	
	/* When timer flag is off */
	if(mkw_timmng_tbl[CMD_NUM1] == CMD_NUM0)
	{
		cm_Assert(CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM1], "TRA file write timer already closed");	
		
		return;
	}
	/* Download status check */
	if(cmw_trafile_inf_tbl.en_flg != CMD_TDL_ON)
	{
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

/* @}*/

