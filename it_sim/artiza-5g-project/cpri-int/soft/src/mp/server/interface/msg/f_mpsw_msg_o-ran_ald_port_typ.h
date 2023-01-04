/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_ALD_PORT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2019
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_ALD_PORT_TYP_H
#define	F_MPSW_MSG_O_RAN_ALD_PORT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_ald_port
 *  @{
 */

 /**
 * @brief	(0x810C0211)MSI_O_RAN_ALD_PORT_GET_REQ
 * @note	ALD電源ON/OFF状態取得要求(Agent->PF)
 * 
 */
typedef struct{
	UINT		ald_port_id;								/**< ALDポート  */
}T_MPSW_MSI_O_RAN_ALD_PORT_GET_REQ;
 
/**
 * @brief	(0x810C0211)MSI_O_RAN_ALD_PORT_GET_REQ
 * @note	ALD電源ON/OFF状態取得要求(Agent->PF)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_GET_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_GET_REQ;


 /**
 * @brief	(0x810C0212)MSI_O_RAN_ALD_PORT_GET_CFM
 * @note	ALD電源ON/OFF状態取得応答(PF->Agent)
 * 
 */
typedef struct{
	UINT		ald_port_id;								/**< ALDポート  */
	UINT		powerState;									/**< ALD電源状態  */
}T_MPSW_MSI_O_RAN_ALD_PORT_GET_CFM;
 
/**
 * @brief	(0x810C0212)MSI_O_RAN_ALD_PORT_GET_CFM
 * @note	ALD電源ON/OFF状態取得応答(PF->Agent)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_GET_CFM;



 /**
 * @brief	(0x810C0211)MSI_O_RAN_ALD_PORT_EDIT_REQ
 * @note	ALD電源ON/OFF要求(Agent->PF)
 * 
 */
typedef struct{
	UINT		ald_port_id;								/**< ALDポート  */
	UINT		power_control;								/**< ALD DC Power制御  */
}T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REQ;
 
/**
 * @brief	(0x810C0211)MSI_O_RAN_ALD_PORT_EDIT_REQ
 * @note	ALD電源ON/OFF要求(Agent->PF)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_EDIT_REQ;


 /**
 * @brief	(0x810C0212)MSI_O_RAN_ALD_PORT_EDIT_CFM
 * @note	ALD電源ON/OFF応答(PF->Agent)
 * 
 */
typedef struct{
	UINT		ald_port_id;								/**< ALDポート  */
}T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_CFM;
 
/**
 * @brief	(0x810C0212)MSI_O_RAN_ALD_PORT_EDIT_CFM
 * @note	ALD電源ON/OFF応答(PF->Agent)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_EDIT_CFM;

	
/**
 * @brief	(0x810C0213)MSI_O_RAN_ALD_PORT_EDIT_REJ
 * @note	ALD電源ON/OFF応答(異常時：PF->Agent)
 * 
 */
typedef struct{
	UINT					ald_port_id;						/**< ALDポート */
	T_MPSW_SRV_MSGERROR		msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REJ;

/**
 * @brief	(0x810C0213)MSI_O_RAN_ALD_PORT_EDIT_REJ
 * @note	ALD電源ON/OFF応答(異常時：PF->Agent)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;						/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REJ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_EDIT_REJ;


/**
 * @brief	(0x810C0715)MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND
 * @note	ALD電源状態変化 Notification通知
 * 
 */
typedef struct{
	UINT				ald_port_id;						/**< ALDポート */
	UINT				dc_enable_status_val;				/**< ALD電源状態  */
}T_MPSW_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND;

/**
 * @brief	(0x810C0715)MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND
 * @note	ALD電源状態変化 Notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;						   /**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND  data;  /**< Message data  */
}T_MSG_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND;



/** @} */

#endif	/* F_MPSW_MSG_O_RAN_ALD_PORT_TYP_H */
