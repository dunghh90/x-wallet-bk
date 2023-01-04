/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_rom.c
 *  @brief   declare  ROM table in MKM task 
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
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
 *  @brief     declare  ROM table in MKM task  
 *  @note     declare  ROM table in MKM task 
 *  @param  None		
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/



VOID (* const mkr_mtrxtbl[MKD_MX_STATE][MKD_MX_EVENT])(UINT *a_inf_p) = 	
{						
	{  	m_mk_Init, 				/* 初期化要求					*/
		m_mk_Nop, 				/* TCP Server 初期化要求		*/
		m_mk_Nop, 				/* TCP Server初期化応答			*/
		m_mk_Nop,				/* TCP Server Stop Notice */
		m_mk_Nop, 				/* TCP接続通知					*/
		m_mk_Nop,	 			/* TCP解放通知					*/
		m_mk_Nop,				/* TCP データ受信通知			*/
		m_mk_Nop, 				/* TCPデータ送信応答			*/
		m_mk_Nop,				/* L1データ送信要求(Monitor)	*/
		m_mk_Nop,				/* L1データ受信通知(Monitor)	*/
		m_mk_Nop, 				/* L1データ送信要求				*/
		m_mk_Nop,				/* CPRIリンク state通知			*/
		m_mk_Nop,				/* Lapbリンク state通知			*/
		m_mk_Nop,				/* L3リンク state通知			*/
		m_mk_Nop,				/* タイムアウト発生通知(モニタデータタイマ)	*/
		m_mk_Nop,				/* ファイルダウンロード確認タイマT.O*/
		m_mk_Nop,				/* TRAファイルライト応答 		*/
		m_mk_Nop,				/* Card state通知				*/
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_Nop				/* system parameter change notice */
#else
		m_mk_Nop,				/* system parameter change notice */
		m_mk_Nop				/* L1データ受信通知		*/
#endif
	},

	{  	m_mk_Nop, 				/* 初期化要求					*/
		m_mk_TcpServInitReq,	/* TCP Server 初期化要求		*/
		m_mk_Nop, 				/* TCP Server初期化応答			*/
		m_mk_TServQuitNtc,		/* TCP Server Stop Notice */
		m_mk_Nop, 				/* TCP接続通知					*/
		m_mk_Nop,	 			/* TCP解放通知					*/
		m_mk_Nop,				/* TCP データ受信通知			*/
		m_mk_Nop, 				/* TCPデータ送信応答			*/
		m_mk_Nop,				/* L1データ送信要求(Monitor)	*/
		m_mk_Nop,				/* L1データ受信通知(Monitor)	*/
		m_mk_Nop, 				/* L1データ送信要求				*/
		m_mk_L1linkstat_chgnfy,	/* CPRIリンク state通知			*/
		m_mk_Nop,				/* Lapbリンク state通知			*/
		m_mk_Nop,				/* L3リンク state通知			*/
		m_mk_Nop,				/* タイムアウト発生通知(モニタデータタイマ)	*/
		m_mk_Nop,				/* ファイルダウンロード確認タイマT.O*/
		m_mk_Nop,				/* TRAファイルライト応答 		*/
		m_mk_Nop,				/* Card state通知 			*/
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_SysParaChgNtc		/* system parameter change notice */
#else
		m_mk_SysParaChgNtc,		/* system parameter change notice */
		m_mk_Nop				/* L1データ受信通知		*/
#endif
	},
	
	{  	m_mk_Nop, 				/* 初期化要求					*/
		m_mk_Nop, 				/* TCP Server 初期化要求		*/
		m_mk_TcpInit_Rsp, 		/* TCP Server初期化応答			*/
		m_mk_Nop,				/* TCP Server Stop Notice */
		m_mk_Nop, 				/* TCP接続通知					*/
		m_mk_Nop,				/* TCP解放通知					*/
		m_mk_Nop,				/* TCP データ受信通知			*/
		m_mk_Nop, 				/* TCPデータ送信応答			*/
		m_mk_Nop,				/* L1データ送信要求(Monitor)	*/
		m_mk_Nop,				/* L1データ受信通知(Monitor)	*/
		m_mk_Nop, 				/* L1データ送信要求				*/
		m_mk_L1linkstat_chgnfy,	/* CPRIリンク state通知			*/
		m_mk_Nop,				/* Lapbリンク state通知			*/
		m_mk_Nop,				/* L3リンク state通知			*/
		m_mk_Nop,				/* タイムアウト発生通知(モニタデータタイマ)	*/
		m_mk_Nop,				/* ファイルダウンロード確認タイマT.O*/
		m_mk_Nop,				/* TRAファイルライト応答 		*/
		m_mk_Nop,				/* Card state通知 			*/
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_SysParaChgNtc		/* system parameter change notice */
#else
		m_mk_SysParaChgNtc,		/* system parameter change notice */
		m_mk_Nop				/* L1データ受信通知		*/
#endif
	},
	
	{  	m_mk_Nop, 				/* 初期化要求					*/
		m_mk_TcpServInitReq, 	/* TCP Server 初期化要求		*/
		m_mk_Nop, 				/* TCP Server初期化応答			*/
		m_mk_Nop,				/* TCP Server Stop Notice */
		m_mk_TcpLink_Conn, 		/* TCP接続通知					*/
		m_mk_TcpLink_Rel,		/* TCP解放通知					*/
		m_mk_TcpData_Rcv,		/* TCP データ受信通知			*/
		m_mk_TcpData_SndRsp, 	/* TCPデータ送信応答			*/
		m_mk_CpriMon_DataSnd,	/* L1データ送信要求(Monitor)	*/
		m_mk_CpriMon_DataRcv,	/* L1データ受信通知(Monitor)	*/
		m_mk_CpriSimu_DataSnd, 	/* L1データ送信要求				*/
		m_mk_L1linkstat_chgnfy,	/* CPRIリンク state通知			*/
		m_mk_L2linkStat_ChgNfy,	/* Lapbリンク state通知			*/
		m_mk_L3linkStat_ChgNfy,	/* L3リンク state通知			*/
		m_mk_MonDatTO,			/* タイムアウト発生通知(モニタデータタイマ)	*/
		m_mk_FileWaitTimTO,		/* ファイルダウンロード確認タイマT.O*/
		m_mk_FileWtRsp,			/* TRAファイルライト応答 		*/
		m_mk_CardStat_ChgNfy,	/* Card state通知 			*/
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_SysParaChgNtc		/* system parameter change notice */
#else
		m_mk_SysParaChgNtc,		/* system parameter change notice */
		m_mk_CpriMsg_DataRcv	/* L1データ受信通知	*/
#endif
	}
};
/* @} */
