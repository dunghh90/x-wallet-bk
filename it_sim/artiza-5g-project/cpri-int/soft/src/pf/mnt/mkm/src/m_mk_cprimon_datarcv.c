/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_CpriMon_DataRcv.c
 *  @brief   Send CPRI monitor data to PC through LAN tx task	 
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
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/


/********************************************************************************************************************/
/**
 *  @brief    Send CPRI monitor data to PC through LAN tx task
 *  @note     Send CPRI monitor data to PC through LAN tx task
 *  @param  inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2015/09/24 TDI)satou CPRIリンク番号から対応するTCPコネクション情報を参照するように
 */
/********************************************************************************************************************/
VOID m_mk_CpriMon_DataRcv(UINT *inf_p)
{
    CMT_TSKIF_MKMTMONDAT *mondat_p;
    T_CMD_TCPCONN_NUM tcpconn;
    USHORT link_num;

    mondat_p = (CMT_TSKIF_MKMTMONDAT*)inf_p;
    link_num = *(USHORT*)(mondat_p + 1);
    link_num = mnt_com_swap_USHORT(link_num);
    tcpconn  = mnt_com_get_tcpconn_monitor(link_num);
    *(USHORT*)(mondat_p + 1) = mnt_com_conv_linkno_internal_to_simu(link_num);
    *(USHORT*)(mondat_p + 1) = mnt_com_swap_USHORT(*(USHORT*)(mondat_p + 1));

	/* If not  in monitor connection		*/
	if(cmw_tblClientInfo[tcpconn].conn != CMD_CONN_Y)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriMon_DataRcv] Connect mode NG");
		return;
	}
	
	/* Monitor data receiving processing 	*/
	m_mk_cm_MonDatProc(MKD_CPRIMONDATRCVNTC, inf_p, link_num);
}
/* @} */
