/*!
 * @skip 		$Id:$
 * @file 		f_dhcp_def.h
 * @brief		DHCP thread define header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_DHCP_DEF_H
#define F_DHCP_DEF_H

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_dhcp state
 * @note pf_dhcp state definition
 * @{
 */
#define	D_DHCP_THD_INIT				0									/* 初期状態				*/
#define	D_DHCP_THD_ACT				1									/* 運用状態				*/
#define	D_DHCP_STATENUM				2									/* 状態数				*/
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_dhcp event
 * @note pf_dhcp event definition
 * @{
 */
#define	D_DHCP_EVT_INIT										0x0000		/* 初期化要求					*/
#define	D_DHCP_EVT_MPLANE_CNCT_READY						0x0001		/* M-Plane接続準備要求			*/
#define	D_DHCP_MSGID_MSI_RCV_IND							0x0002		/* MSI受信通知					*/
#define	D_DHCP_MSGID_NMA_VLAN_WRITE_RSP						0x0003		/* serchable-vlans書込み応答	*/
#define	D_DHCP_EVENTNUM										0x0004		/* イベント数					*/
/* @} */

/****************************************************************************************************************/
/*  VID scan result code																						*/
/****************************************************************************************************************/
/*!
 * @name pf_dhcp vid scan result
 * @note pf_dhcp vid scan result
 * @{
 */
enum {
	D_DHCP_METHOD_LAST_USED_VID=0,						/* vidAcquiringMethod = LastUsedVid		*/
	D_DHCP_METHOD_VID_RANGE_SCAN,						/* vidAcquiringMethod = VidRangeScan	*/
	D_DHCP_METHOD_FULL_RANGE_SCAN,						/* vidAcquiringMethod = FullRangeScan	*/
	D_DHCP_METHOD_MAX
};
/* @} */

/****************************************************************************************************************/
/*  FILE Path / FILE Name																						*/
/****************************************************************************************************************/
/*!
 * @name FILE Path / FILE Name
 * @note FILE Path / FILE Name
 * @{
 */
#define D_DHCP_PATH_DHCP				"/var/lib/dhcp/"
#define D_DHCP_FNAME_LEASES				"dhclient.leases"

#define D_DHCP_CMD_IFCONFIG_UP			"ifconfig %s up 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_IFCONFIG_DOWN		"ifconfig %s down 1>/dev/null 2>/dev/null"

#define D_DHCP_CMD_VLAN_NAME_TYPE		"vconfig set_name_type DEV_PLUS_VID_NO_PAD 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_VLAN_ADD				"vconfig add %s %d 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_VLAN_DEL				"vconfig rem %s.%d 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_VLAN_INGRESS			"vconfig set_ingress_map %s %d %d 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_VLAN_EGRESS			"vconfig set_egress_map %s %d %d 1>/dev/null 2>/dev/null"

#define D_DHCP_CMD_DHCLIENT				"dhclient %s 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_DHCLIENTR			"dhclient %s -r 1>/dev/null 2>/dev/null"

#define D_DHCP_CMD_DHCLIENT_QEMU		"dhclient %s 1>/dev/null 2>/dev/null"
#define D_DHCP_CMD_DHCLIENTR_QEMU		"dhclient %s -r 1>/dev/null 2>/dev/null"

#define D_DHCP_CMD_DEL_LEASES			"rm -f %s%s 1>/dev/null 2>/dev/null"

#define D_DHCP_CMD_KILLALL				"killall -9 dhclient  1>/dev/null 2>/dev/null"
/* @} */

/****************************************************************************************************************/
/*  Register																									*/
/****************************************************************************************************************/
/*!
 * @name Register用define
 * @note Register用define
 * @{
 */
#define D_DHCP_REG_ETH_OFT_PORT1		0x00001000		/* ETH Reg PORT#1用オフセット値			*/
#define D_DHCP_VAL_FILTVID15_EN			0x00010000		/* ETH Reg FILTVID15 Enable bit			*/
#define D_DHCP_VAL_FILTVID15_VID_MSK	0x00000FFF		/* ETH Reg FILTVID15 Enable bit			*/
/** @} */

/****************************************************************************************************************/
/*  Error Description																							*/
/****************************************************************************************************************/
/*!
 * @name Error Description
 * @note Error Description
 * @{
 */
#define	D_DHCP_MSIERR_MSG_FLASH_WRITE_FAIL	"FLASH Write failure\0"
/* @} */

/****************************************************************************************************************/
/*  MSI用define																									*/
/****************************************************************************************************************/
/*!
 * @name MSI用define
 * @note T_MPSW_MSI_O_RAN_DHCP_GET_CFM用define
 * @{
 */
#define D_DHCP_MSI_ENTERPRISE_NUM		0xCF9C			/* private_enterprise_number			*/
/** @} */

#endif
/** @} */
