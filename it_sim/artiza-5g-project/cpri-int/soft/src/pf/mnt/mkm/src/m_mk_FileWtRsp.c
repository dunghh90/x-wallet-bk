/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_FileWtRsp.c
 *  @brief   TRA file write response
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
#include "rrhApi_File.h"
/********************************************************************************************************************/
/**
 *  @brief     TRA file write response
 *  @note     TRA file write response
 *  @param  a_inf_p		[in]	message buffer pointer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/


VOID m_mk_FileWtRsp(UINT *a_inf_p)
{
	T_API_FILE_UPDATE_FIRM_RSP 	*a_FlWrtRsp_p 	= NULL;		/* write response pointer 	*/
	UINT 					a_NgCode 		= CMD_RES_OK;/* NG code					*/
	UINT					a_FuncId 		= CMD_NUM0;	/* Function ID 				*/
	
	/* Downloading check */
	if(cmw_trafile_inf_tbl.en_flg  != CMD_TDL_ON)
	{
		/* Not Downloading */
		cm_Assert(CMD_ASL_DBGLOW, cmw_trafile_inf_tbl.en_flg,"Downlaoding is not started");
		
		return;
	}
	
	/****************************/
	/*		Get parameters		*/
	/****************************/
	/* Get TRA Message */
	a_FlWrtRsp_p = (T_API_FILE_UPDATE_FIRM_RSP*)a_inf_p;
	/* Get ng code */
	if( a_FlWrtRsp_p->data.dlwResult != CMD_RES_OK)
	{
		/* Transfer NG code */
		a_NgCode = m_mk_REMTNgcodeTrans(a_FlWrtRsp_p->data.dlwResult);
	}
	/* Get function Id */
	a_FuncId = 0;/*a_FlWrtRsp_p->traflwrtrsp.FunctionID;*/

	/***********************************/
	/*	 ファイルダウンロード完了応答	*/
	/************************************/
	m_mk_DataEndNtcRsp(a_FuncId, a_NgCode);
	/* Clear the status */
	m_mk_cm_DLEnd();

	return;	
}
/* @} */
