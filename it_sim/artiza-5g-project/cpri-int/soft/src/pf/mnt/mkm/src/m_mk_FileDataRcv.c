/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_FileDataRcv.c
 *  @brief   Receive data packets and save to table
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
 *  @brief     Receive data packets and save to table
 *  @note     Receive data packets and save to table
 *  @param  a_inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_FileDataRcv(UINT *a_inf_p)
{
	MKT_REMTTRAFLBLKSND 	*a_DLData_p		= NULL;			/* Data transmit pointer 			*/
	UINT 				a_SeqNo			= CMD_NUM0;		/* Data packet sequnce number 		*/
	UINT 				a_Size			= CMD_NUM0;		/* Data packet size 				*/
	VOID 				*a_FileData_p		= NULL;			/* File data pointer 				*/
	UINT 				a_RcvSize		= CMD_NUM0;		/* Received total size 				*/
	UINT 				a_ret			= CMD_RES_OK;	/* Return result					*/
	INT                 errcd;
	
	/****************************/
	/*	 Download status check	*/
	/****************************/
	/* Check the downloading stauts */
	if(cmw_trafile_inf_tbl.en_flg != CMD_TDL_ON)
	{
		/* Downlaoding status error ,assert */
		cm_Assert(CMD_ASL_DBGLOW, cmw_trafile_inf_tbl.en_flg, "Downlaoding from RE-MT is not started");
		return;
	}

	/****************************/
	/*		Get parameters		*/
	/****************************/
	/* Get message part */
	a_DLData_p = (MKT_REMTTRAFLBLKSND*)a_inf_p;
	/* Get packet sequence number */
	a_SeqNo = a_DLData_p->SeqNo;
	/* Get length */
	a_Size = a_DLData_p->Length;
	/* Get file data */
	a_FileData_p = &a_DLData_p->Buffer;
	
	/****************************/
	/*		Check parameter		*/
	/****************************/
	/* when packet size out of range */
	if((a_Size > MKD_PACKDAT_MAX) ||(a_Size == CMD_NUM0))
	{
		/* When download timer is running */
		if(mkw_timmng_tbl[CMD_NUM0] != CMD_NUM0)
		{
			/* Stop download timer 		*/
			a_ret = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
			/* Stop timer error,Assert 	*/
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "File download Timer Stop NG" );
			}
			/* Set download timer flag 	*/
			mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
		}
		/* ファイルダウンロード完了応答 */
		m_mk_DataEndNtcRsp(CMD_NUM0, MKD_RES_FILESIZEERR);
		/* Download status clear 		*/
		m_mk_cm_DLEnd();

		return;
	}
	/* Division number in management table update */
	mkw_dlmng_tbl.div_no ++;
	/* When division in table not equal to division received */
	if(a_SeqNo != mkw_dlmng_tbl.div_no)
	{
		/* When download timer is running */
		if(mkw_timmng_tbl[CMD_NUM0]  != CMD_NUM0)
		{
			/* Stop download timer */
			a_ret = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
			/* Stop timer error,Assert */
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "File download Timer Stop NG" );
			}
			/* Set download timer flag */
			mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
		}
		/* ファイルダウンロード完了応答 */
		m_mk_DataEndNtcRsp(CMD_NUM0, MKD_RES_SEQNO_NG);
		/* Download status clear */
		m_mk_cm_DLEnd();

		return;
	}
	/* Get received size before now */
	a_RcvSize = mkw_dlmng_tbl.rcvdata_size;
	/* Calculate the received size */
	a_RcvSize += a_Size;
	/* Check the total size */
	if(a_RcvSize > mkw_dlmng_tbl.total_size)
	{
		/* When download timer is running */
		if(mkw_timmng_tbl[CMD_NUM0]  != CMD_NUM0)
		{
			/* Stop download timer */
			a_ret = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
			/* Stop timer error,Assert */
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "File download Timer Stop NG" );
			}
			/* Set download timer flag */
			mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
		}
		/* ファイルダウンロード完了応答 */
		m_mk_DataEndNtcRsp(CMD_NUM0, MKD_RES_FILESIZEERR);
		/* Download status clear */
		m_mk_cm_DLEnd();
		
		return;
	}
	
	/******************/
	/* データ格納処理 */
	/******************/
	/* データ格納テーブルに受信データをコピーする */
/*	cm_MemCpy(
		(VOID*)((UINT)&cmw_rcvfirm_tbl + mkw_dlmng_tbl.rcvdata_size),
		a_FileData_p,
		a_Size);
*/
	/* call api to save firmware data*/
	rrhApi_File_Fsm_loadMemory(&mkw_handle);

	rrhApi_File_Fsm_SaveData(mkw_handle, a_FileData_p,a_Size);

	rrhApi_File_Fsm_unLoadMemory(mkw_handle, a_Size, 0);

	/* ダウンロード管理テーブル.総受信データサイズを設定する */
	mkw_dlmng_tbl.rcvdata_size = a_RcvSize;
	
	return;
}

/* @} */
