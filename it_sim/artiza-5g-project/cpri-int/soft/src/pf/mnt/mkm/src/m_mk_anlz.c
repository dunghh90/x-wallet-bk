/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_Anlz.c
 *  @brief   Analyze external event, change it into internal event no	 
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/25 FFCS)zhaodx modify for zynq
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
#ifdef OPT_RRH_ZYNQ_REC
#include "rrhApi_L2.h"
#endif
/********************************************************************************************************************/
/**
 *  @brief     Analyze external event, change it into internal event no
 *  @note     Analyze external event, change it into internal event no
 *  @param  srctsk      [in]   Source task
 *  @param  event      [in]   Event no
 *  @return   Result code is returned
 *  @retval   MKD_RET_NORMAL 			0 :	Normal end
 *  @retval   MKD_RET_ABNORMAL 		1 :	Event code not found
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/25 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/


UINT  m_mk_Anlz(UINT srctsk, UINT event)
{
	switch(event)
	{
		case CMD_TSKIF_INIREQ:				/* 初期化要求					*/
			mkw_eventno = MKD_EV_INIREQ;
			break;
		case CMD_TSKIF_TCPSRVINITREQ:		/* TCP Server 初期化要求		*/
			mkw_eventno = MKD_EV_SRVINITREQ;
			break;
		case CMD_TSKIF_TCPSRVSTRRSP:		/* TCP Server初期化応答			*/
			mkw_eventno = MKD_EV_TCPSRVINITRSP;
			break;
		case CMD_TSKIF_TCPSRVSTPNTC:		/* TCP Server Stop Notice */
			mkw_eventno = MKD_EV_TCPSRVSTPNTC;
			break;
		case CMD_TSKIF_TCPCONNNTC:			/* TCP接続通知					*/
			mkw_eventno = MKD_EV_TCPCONNNTC;
			break;
		case CMD_TSKIF_TCPRELNTC:			/* TCP解放通知					*/
			mkw_eventno = MKD_EV_TCPRELNTC;
			break;	
		case CMD_TSKIF_TCPDATNTC:			/* TCP データ受信通知			*/
			mkw_eventno = MKD_EV_TCPDATNTC;
			break;	
		case CMD_TSKIF_TCPSNDRSP:			/* TCPデータ送信応答			*/
			mkw_eventno = MKD_EV_TCPSNDRSP;
			break;	
		case CMD_TSKIF_L1MONDATSNDNTC:		/* L1データ送信要求(Monitor)	*/
			mkw_eventno = MKD_EV_L1MONSNDNTC;
			break;	
		case CMD_TSKIF_L1MONDATRCVNTC:		/* L1データ受信通知(Monitor)	*/
			mkw_eventno = MKD_EV_L1MONRCVNTC;
			break;	
		case CMD_TSKIF_L1DATTXREQ:			/* L1データ送信要求				*/
			mkw_eventno = MKD_EV_L1DATTXREQ;
			break;	
		case CMD_TSKIF_CPRISTANTC:			/* CPRIリンク state通知			*/
			mkw_eventno = MKD_EV_CPRISTANTC;
			break;	
		case CMD_TSKIF_L2STANTC:			/* L2リンク state通知			*/
			mkw_eventno = MKD_EV_L2STANTC;
			break;
		case CMD_TSKIF_L3STANTC:			/* L3リンク state通知			*/
			mkw_eventno = MKD_EV_L3STANTC;
			break;	
		case CMD_TSKIF_CARDSTANTC:			/* Card state通知				*/
			mkw_eventno = MKD_EV_CARDSTANTC;
			break;	
		case CMD_TSKIF_SYSPARA_CHG_NTC:			/* SystemParemeterChangeNotice	*/
			mkw_eventno = MKD_EV_SYSPARA_CHG_NTC;
			break;
		case CMD_TSKIF_TIMOUTNTC:			/* タイムアウト発生通知			*/
			/* Timer ID check */
			switch(srctsk)
			{
				case CMD_TIMID_MKM_MON:	/* If it is Maker MT monitor data timer		*/
					mkw_eventno = MKD_EV_MONDATTO;
					break;
				case CMD_TIMID_REFLDL:
					mkw_eventno = MKD_EV_FLDLTIMOUT;
					break;
				default:
					return MKD_RET_ABNORMAL;
			}
			break;
			
		case CMD_TSKIF_TRAFLWRTRSP:			/* TRAファイルライト応答 		*/
			mkw_eventno = MKD_EV_TRARLWRTRSP;
			break;
#ifdef OPT_RRH_ZYNQ_REC
		case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
			mkw_eventno = MKD_EV_CPRIMSGRCV;/**< CPRI Message rcv通知				*/
			break;
#endif

		default:
			return MKD_RET_ABNORMAL;
	}

	return MKD_RET_NORMAL;
}
/* @} */
