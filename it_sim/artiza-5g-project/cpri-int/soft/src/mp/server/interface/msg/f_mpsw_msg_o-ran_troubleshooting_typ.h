/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_TROUBLESHOOTING.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_TROUBLESHOOTING_TYP_H
#define	F_MPSW_MSG_O_RAN_TROUBLESHOOTING_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_troubleshooting
 *  @{
 */

/**
 * @brief	(0x81220611)MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ
 * @note	rpc start-troubleshooting-logs REQ要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ;

/**
 * @brief	(0x81220612)MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM
 * @note	rpc start-troubleshooting-logs CFM応答(結果応答OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM;

/**
 * @brief	(0x81220613)rpc start-troubleshooting-logs REJ応答(結果応答NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr[1];							/**< エラー情報  */
}T_MPSW_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ;

/**
 * @brief	(0x81220613)MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ
 * @note	rpc start-troubleshooting-logs REJ応答(結果応答NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ;

/**
 * @brief	(0x81220621)MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ
 * @note	rpc stop-troubleshooting-logs REQ要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ;

/**
 * @brief	(0x81220622)MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM
 * @note	rpc stop-troubleshooting-logs CFM応答(結果応答OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM;

/**
 * @brief	(0x81220623)rpc stop-troubleshooting-logs REJ応答(結果応答NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr[1];							/**< エラー情報  */
}T_MPSW_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ;

/**
 * @brief	(0x81220623)MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ
 * @note	rpc stop-troubleshooting-logs REJ応答(結果応答NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ;

/**
 * @brief	(0x81220715)troubleshooting-log-generated notification通知
 * @note	
 * 
 */
typedef struct{
	UCHAR				file_num;							/**< ログファイル数  */
	CHAR				log_file_name[1][128];				/**< ログファイル名  */
}T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND;

/**
 * @brief	(0x81220715)MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND
 * @note	troubleshooting-log-generated notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_TROUBLESHOOTING_TYP_H */
