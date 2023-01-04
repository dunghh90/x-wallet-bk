/*!
 * @skip 		$Id:$
 * @file 		f_fhs_def.h
 * @brief		FHS thread define header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_FHS_DEF_H
#define F_FHS_DEF_H

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_fhs state
 * @note pf_fhs state definition
 * @{
 */
#define	D_FHS_THD_INIT				0									/* 初期状態				*/
#define	D_FHS_THD_ACT				1									/* 運用状態				*/
#define	D_FHS_STATENUM				2									/* 状態数				*/
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_fhs event
 * @note pf_fhs event definition
 * @{
 */
#define	D_FHS_EVT_INIT										0x0000		/* 初期化要求							*/
#define	D_FHS_MSGID_MSI_RCV_IND								0x0001		/* MSI受信通知							*/
#define	D_FHS_MSGID_NETCONF_WRITE_RSP						0x0002		/* NETCONFユーザー情報書込み応答		*/
#define	D_FHS_EVENTNUM										0x0003		/* イベント数							*/
/* @} */

/****************************************************************************************************************/
/*  Interface Type																								*/
/****************************************************************************************************************/
/*!
 * @name interface type
 * @note interface type
 * @{
 */
#define	D_FHS_IF_O_RAN_INTERFACE_TYPE						0			/* 0：O_RAN-INTERFACE-TYPE				*/
#define	D_FHS_IF_ETHERNET_CSMACD							1			/* 1：ethernetCsmacd					*/
#define	D_FHS_IF_L2VLAN										2			/* 2：l2vlan							*/
#define	D_FHS_IF_IP											3			/* 3：ip								*/
/* @} */

/****************************************************************************************************************/
/*  Kind of Plane																								*/
/****************************************************************************************************************/
/*!
 * @name Kind of Plane
 * @note Kind of Plane
 * @{
 */
#define	D_FHS_M_PLANE										0			/* 0：M-Plane							*/
#define	D_FHS_UC_PLANE										1			/* 1：UC-Plane							*/
#define	D_FHS_S_PLANE										2			/* 2：S-Plane							*/
/* @} */

/****************************************************************************************************************/
/*  S-Plane communication Port																					*/
/****************************************************************************************************************/
/*!
 * @name S-Plane communication Port
 * @note S-Plane communication Port
 * @{
 */
#define	D_FHS_SP_PORT_NOUSE									0			/* 2：S-Plane通信Port未確定				*/
#define	D_FHS_SP_PORT_0										1			/* 2：Port#0通信中						*/
#define	D_FHS_SP_PORT_1										2			/* 2：Port#1通信中						*/
/* @} */

/****************************************************************************************************************/
/*  Operational status																							*/
/****************************************************************************************************************/
/*!
 * @name Operational status
 * @note Operational status
 * @{
 */
#define	D_FHS_OPER_STATE_UP									1			/* 1:up									*/
#define	D_FHS_OPER_STATE_DOWN								2			/* 2:down (LOS/LOF、interface無効)		*/
#define	D_FHS_OPER_STATE_TESTING							3			/* 3:testing (未使用)					*/
#define	D_FHS_OPER_STATE_UNKOWN								4			/* 4:unknown (未使用)					*/
#define	D_FHS_OPER_STATE_DORMANT							5			/* 5:dormant (未使用)					*/
#define	D_FHS_OPER_STATE_NOT_PRESENT						6			/* 6:not-present (SFP未実装)			*/
#define	D_FHS_OPER_STATE_LOW_LAYER_DOWN						7			/* 7:lower-layer-down (SFP障害)			*/
/* @} */

/****************************************************************************************************************/
/*  Error Description																							*/
/****************************************************************************************************************/
/*!
 * @name Error Description
 * @note Error Description
 * @{
 */
#define	D_FHS_MSIERR_MSG_IP					"IP Address Unmatch"
#define	D_FHS_MSIERR_MSG_PREFIX				"Prefix length Unmatch"
#define	D_FHS_MSIERR_MSG_MP_PORT			"M-Plane Port Unmatch"
#define	D_FHS_MSIERR_MSG_MP_VID				"M-Plane vlan-id Unmatch"
#define	D_FHS_MSIERR_MSG_VLAN_NOTSUP		"vlan not supported"
#define	D_FHS_MSIERR_MSG_SFP_NO_READ		"SFP read timing error"
#define	D_FHS_MSIERR_MSG_SFP_READ_FAIL		"SFP Read to failure"
#define	D_FHS_MSIERR_MSG_USER_DUP_CNT_ERR	"User Inf Control Duplicate Err"
#define	D_FHS_MSIERR_MSG_NUM_OF_USER_OVER	"Registration over of User"
#define	D_FHS_MSIERR_MSG_FLASH_WRITE_FAIL	"FLASH Write failure"
#define	D_FHS_MSIERR_MSG_USER_INF_SET_ERR	"LINUX command error"
/* @} */

/****************************************************************************************************************/
/*  NETCONF user information chg status																			*/
/****************************************************************************************************************/
/*!
 * @name NETCONF user information chg status
 * @note NETCONF user information chg status
 * @{
 */
#define	D_FHS_USER_CHG_STATUS_NORMAL		0
#define	D_FHS_USER_CHG_STATUS_CHANGE		1
#define	D_FHS_USER_CHG_STATUS_ADDITION		2
/* @} */

/****************************************************************************************************************/
/*  NETCONF user information chg status																			*/
/****************************************************************************************************************/
/*!
 * @name NETCONF user information chg status
 * @note NETCONF user information chg status
 * @{
 */
#define	D_FHS_USER_CHG_CMD_LENGTH			512
#define	D_FHS_USER_CHG_CMD_USER_ADD			"useradd %s"
#define	D_FHS_USER_CHG_CMD_PASS_CHG			"echo \"%s:%s\" | chpasswd"
#define	D_FHS_USER_CHG_CMD_USER_ENABLE		"usermod -s /bin/sh %s"
#define	D_FHS_USER_CHG_CMD_USER_DISABLE		"usermod -s /bin/false %s"
/* @} */

#endif
/** @} */
