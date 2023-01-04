/*!
 * @skip 		$Id:$
 * @file 		f_eom_def.h
 * @brief		pf_eom thread define header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_EOM_DEF_H
#define F_EOM_DEF_H

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_eom state
 * @note pf_eom state definition
 * @{
 */
enum {
	E_EOM_STATE_INIT = 0,												/* 初期状態				*/
	E_EOM_STATE_ACT,													/* 運用状態				*/
	E_EOM_STATE_MAX														/* 状態数				*/
};
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_eom event
 * @note pf_eom event definition
 * @{
 */
enum {
	E_EOM_EVT_INIT = 0,													/* 初期化要求			*/
	E_EOM_EVT_MSI_RCV_IND,												/* MSI受信通知			*/
	E_EOM_EVT_MAX														/* イベント数			*/
};
/* @} */

/****************************************************************************************************************/
/*  o-ran-lbm関連																								*/
/****************************************************************************************************************/
/*!
 * @name relation to o-ran-lbm
 * @note relation to o-ran-lbm
 * @{
 */
#define	D_EOM_LBM_STR_LEN									256			/* MSI_LBM_EDIT_REQ 文字列長			*/
#define	D_EOM_LBM_STR_NAME_LEN								44			/* MSI_LBM_EDIT_REQ name文字列長		*/
#define	D_EOM_LBM_MD_LEVEL_MIN								0			/* MD-LEVEL 最小値 (0)					*/
#define	D_EOM_LBM_MD_LEVEL_MAX								7			/* MD-LEVEL 最大値 (7)					*/
/* @} */

/****************************************************************************************************************/
/*  LBM/LBR関連																								*/
/****************************************************************************************************************/
/*!
 * @name relation to LBM/LBR message
 * @note relation to LBM/LBR message
 * @{
 */
#define	D_EOM_LBX_OPCODE_LBM								3			/* OpCode (LBM)							*/
#define	D_EOM_LBX_OPCODE_LBR								2			/* OpCode (LBR)							*/
#define	D_EOM_LBX_MA_LEVEL_MASK								0xE0		/* MA_LEVELマスク値						*/
#define	D_EOM_LBX_MA_LEVEL_SHFT								5			/* MA_LEVELシフト値						*/
/* @} */

/****************************************************************************************************************/
/*  LINUX command関連																								*/
/****************************************************************************************************************/
/*!
 * @name relation to LINUX command
 * @note relation to LINUX command format
 * @{
 */
#define D_EOM_CMD_IFCONFIG_UP			"ifconfig %s.%d up 1>/dev/null 2>/dev/null"
#define D_EOM_CMD_IFCONFIG_DOWN			"ifconfig %s.%d down 1>/dev/null 2>/dev/null"

#define D_EOM_CMD_VLAN_NAME_TYPE		"vconfig set_name_type DEV_PLUS_VID_NO_PAD 1>/dev/null 2>/dev/null"
#define D_EOM_CMD_VLAN_ADD				"vconfig add %s %d 1>/dev/null 2>/dev/null"
#define D_EOM_CMD_VLAN_DEL				"vconfig rem %s.%d 1>/dev/null 2>/dev/null"
#define D_EOM_CMD_VLAN_INGRESS			"vconfig set_ingress_map %s.%d %d %d 1>/dev/null 2>/dev/null"
#define D_EOM_CMD_VLAN_EGRESS			"vconfig set_egress_map %s.%d %d %d 1>/dev/null 2>/dev/null"
/* @} */

/****************************************************************************************************************/
/*  Error Description																							*/
/****************************************************************************************************************/
/*!
 * @name Error Description
 * @note Error Description
 * @{
 */
#define	D_EOM_MSIERR_MSG_MDLEVEL_DUP_ERR	"md-level Duplicate set Err\0"
#define	D_EOM_MSIERR_MSG_VLAN_INVALID		"uplane vlan-id invalid\0"
/* @} */

#endif
/** @} */
