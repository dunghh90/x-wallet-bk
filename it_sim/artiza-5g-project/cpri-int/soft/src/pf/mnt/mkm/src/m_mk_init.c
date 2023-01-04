/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_Init.c
 *  @brief   Init MKM task
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
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
#include "f_sys_def.h"

/********************************************************************************************************************/
/**
 *  @brief     Init MKM task
 *  @note     1. Client infomation table init process						
 *		     2. Get Maker MT address and send TCP server init req to LAN rx task	
 *		     3. Send init response to init task	
 *  @param  buf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_Init(UINT *buf_p)
{
	CMT_TSKIF_HEAD *head_p;
	INT			ret_buf;
	UINT 					mode;			/* Mode					*/
	USHORT					mtIndx;
	USHORT					mLnkIndx;
	UCHAR				*eeprom_p;

	mode = CMD_NUM0;			/* Mode				*/
	
	mkw_needfrmsg = MKD_TRUE;


	mkw_handle = CMD_NUM0;

	cmw_mkaddr_reset = CMD_OFF;
	

	/* Table mk_tblClientInfo initialization			*/
	for(mode = CMD_NUM0; mode < CMD_TCPCONN_NUM_MAX; mode ++)
	{
		cmw_tblClientInfo[mode].conn = CMD_CONN_N;				/* pgr0013 pgr0862 */ /* Not connect	*/
		cmw_tblClientInfo[mode].tcplinkhdl= CMD_TCPHDL_NULL;	/* pgr0013 pgr0862 */ /* TCP handle null	*/
	}
	/* Table for file download initialization */
	m_mk_cm_DLEnd();

	mkw_mdatsav_p = NULL;	
	
	for(mtIndx=0; mtIndx<3; mtIndx++)
	{
		for(mLnkIndx=0; mLnkIndx<6; mLnkIndx++)
		{
			if((mtIndx>=2)&&(mLnkIndx>=4))
			{
				mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cpriconf = CMD_OFF;
			}
			else
			{
				eeprom_p = (UCHAR*)(&cmw_tra_inv_tbl)+(D_SYS_NUM_0x1A0+mtIndx*6+mLnkIndx);
				mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cpriconf = *eeprom_p;
			}
			mkw_cpristatM[mtIndx].stainfo[mLnkIndx].selfslotno = mLnkIndx+1;
			mkw_cpristatM[mtIndx].stainfo[mLnkIndx].retype = 3;
			mkw_cpristatM[mtIndx].stainfo[mLnkIndx].traclsinfo = CMD_ACTIVE;
			mkw_cpristatM[mtIndx].stainfo[mLnkIndx].actswforbflg = CMD_OFF;
		}
	}
	
	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "m_mk_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_MKM;
	head_p->uiDstTQueueID = CMD_TSKID_INI;
	/* Send init response to init task			*/
	ret_buf = cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}

	/* Task state change into "TCP server 初期化要求受信待ち" state	*/
	mkw_stateno = MKD_ST_TSIREQRXW;
}
/* @} */
