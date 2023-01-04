/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_DataEndNtcRcv.c
 *  @brief   Check received data , and send TRA file write request to TDL task
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/29 FFCS)zhaodx modify for zynq
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
#include "BPF_RU_HRTM.h"
/********************************************************************************************************************/
/**
 *  @brief     Check received data , and send TRA file write request to TDL task
 *  @note     Check received data , and send TRA file write request to TDL task
 *  @param   a_inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_DataEndNtcRcv(UINT *a_inf_p)
{
	MKT_REMTFLDLCMPREQ 		*a_DlCmp_p 		= NULL;		/* Download finished request message pointer*/
	UINT 					a_functionid 	= CMD_NUM0;	/* Function ID 								*/
	USHORT 					a_FileType 		= CMD_NUM0;	/* File type 								*/
	USHORT 					a_EndType 		= CMD_NUM0;	/* End type 								*/
	USHORT 					a_chksum_rcv 	= CMD_NUM0;	/* Check sum received 						*/
	UINT 					a_ret 			= CMD_NUM0;	/* Return result 							*/
	INT                     wtime;                
	INT                     errcd;
	
	/********************************/
	/*		Downlaod status check	*/
	/********************************/
	/* Check the downloading stauts */
	if(cmw_trafile_inf_tbl.en_flg != CMD_TDL_ON)
	{
		/* Downlaoding status error ,assert */
		cm_Assert(CMD_ASL_DBGLOW, cmw_trafile_inf_tbl.en_flg, "Downlaoding from RE-MT is not started");
		
		return;
	}

	/********************************/
	/*		Stop timer				*/
	/********************************/
	/* When download timer is running	*/
	if(mkw_timmng_tbl[CMD_NUM0] != CMD_NUM0)
	{
		/* Stop download timer			*/
		a_ret = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
		/* Stop timer error,Assert 		*/
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "file download Timer Stop NG" );
		}
		/* Set download timer flag 		*/
		mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
	}

	/********************************/
	/*		Get parameters			*/
	/********************************/
	/* Get message 		*/
	a_DlCmp_p = (MKT_REMTFLDLCMPREQ*)a_inf_p;
	/* Get Function ID	*/
	a_functionid = a_DlCmp_p->FunctionID;
	/* Get file type 	*/
	a_FileType = a_DlCmp_p->FileType;
	/* Get end type 	*/
	a_EndType = a_DlCmp_p->EndType;
	/* Get Check sum 	*/
	a_chksum_rcv = (USHORT)a_DlCmp_p->CheckSum;
	
	/********************************/
	/*		Check parameter			*/
	/********************************/
	/* When End type abnormal */
	if(a_EndType != CMD_RES_OK)
	{
		/* ファイルダウンロード完了応答処理 */
		m_mk_DataEndNtcRsp(a_functionid, CMD_NML);
		/* When TRA write timer is running, stop it */
		/* RE client may send cancel message after normal one has been sended already */
		if(mkw_timmng_tbl[CMD_NUM1] != CMD_NUM0)
		{
			/* Stop TRA write timer */
			a_ret = cm_TStop(mkw_timmng_tbl[CMD_NUM1], &errcd);
			/* Stop timer error,Assert 		*/
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM1], "TRA write timer Stop NG" );
			}
			/* Set download timer flag 		*/
			mkw_timmng_tbl[CMD_NUM1] = CMD_NUM0;
		}
		/* ダウンロード終了処理を起動 		*/
		m_mk_cm_DLEnd();
		
		return;
	}	
	/* File size error */
	if((mkw_dlmng_tbl.div_no !=cmw_trafile_inf_tbl.div_no) ||
		(mkw_dlmng_tbl.rcvdata_size != mkw_dlmng_tbl.total_size))
	{
		/* ファイルダウンロード完了応答処理 */
		m_mk_DataEndNtcRsp(a_functionid, MKD_RES_FILESIZEERR);
		/* ダウンロード終了処理を起動 		*/
		m_mk_cm_DLEnd();
		
		return;		
	}	
	/* File type error */
	if(a_FileType != (USHORT)cmw_trafile_inf_tbl.file_type)
	{
		/* ファイルダウンロード完了応答処理 */
		m_mk_DataEndNtcRsp(a_functionid, MKD_RES_KINDERR);
		/* ダウンロード終了処理を起動 		*/
		m_mk_cm_DLEnd();
		
		return;		
	}
    wtime = 0;
	rrhApi_File_Mnr_UpdateFirm(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL, 0, a_chksum_rcv, 0 );

	return;
}
/* @} */
