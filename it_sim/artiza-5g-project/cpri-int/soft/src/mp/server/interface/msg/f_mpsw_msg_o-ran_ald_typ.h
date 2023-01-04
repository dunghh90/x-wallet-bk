/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_ALD.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2019
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_ALD_TYP_H
#define	F_MPSW_MSG_O_RAN_ALD_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_ald
 *  @{
 */

/**
 * @brief	(0x810B0611)MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ
 * @note	
 * 
 */
typedef struct{
	UINT				ald_port_id;						/**< ALDポート  */
	UINT				ald_req_size;						/**< ALD送信データサイズ  */
	CHAR				ald_req_data[1200];					/**< ALD送信データ  */
}T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ;

/**
 * @brief	(0x810B0611)MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ
 * @note	MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ;

/**
 * @brief	(0x810B0612)MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM
 * @note	
 * 
 */
typedef struct{
	UINT				ald_port_id;						/**< ALDポート  */
	UINT				ald_resp_size;						/**< ALD受信データサイズ  */
	CHAR				ald_resp_data[1200];				/**< ALD受信データ  */
	UINT				ald_error_wrong_crc;				/**< CRC情報  */
	UINT				ald_error_stop_flag;				/**< ALD受信データ状態  */
	UINT				ald_number_of_received_octets;		/**< ALD受信データサイズ(合算値)  */
}T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM;

/**
 * @brief	(0x810B0612)MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM
 * @note	MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM;

/**
 * @brief	(0x810B0613)MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ
 * @note	
 * 
 */
typedef struct{
	UINT				ald_port_id;						/**< ALDポート  */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
	UINT				status;								/**< ステータス情報  */
}T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ;

/**
 * @brief	(0x810B0613)MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ
 * @note	MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_ALD_TYP_H */
