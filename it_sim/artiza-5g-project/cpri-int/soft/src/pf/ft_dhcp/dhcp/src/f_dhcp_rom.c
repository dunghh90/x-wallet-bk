/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_rom.c
 * @brief		pf_dhcpスレッド ROMテーブル
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	ROM Table
 *  @note	
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_dhcp_inc.h"

/** @addtogroup RRH_PF_DHC
 *  @{
 */

/****************************************************************************************************************/
/*  Matrix function of DHCP Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of DHCP Thread
 * @note	Matrix function of DHCP Thread
 * @{
 */
																		/* Matrix function of DHCP Thread		*/
const	T_DHCP_CALLSETRELL_MTXTBL	f_dhcpr_callSetRell_mtxtbl[D_DHCP_STATENUM][D_DHCP_EVENTNUM] = 
{
	{
		{ f_dhcp_InitReq			},									/* 初期化要求							*/
		{ f_dhcp_abort				},									/* M-Plane接続準備要求					*/
		{ f_dhcp_abort				},									/* MSIメッセージ受信通知				*/
		{ f_dhcp_abort				}									/* serchable-vlans書込み応答			*/
	},
	{
		{ f_dhcp_abort				},									/* 初期化要求							*/
		{ f_dhcp_ConnectReadyWrap	},									/* M-Plane接続準備要求					*/
		{ f_dhcp_msi_recv_ind		},									/* MSIメッセージ受信通知				*/
		{ f_dhcp_vlan_write_rsp		}									/* serchable-vlans書込み応答			*/
	}
};
/* @} */

/** @} */

