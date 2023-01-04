/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_OPERATIONS.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_OPERATIONS_TYP_H
#define	F_MPSW_MSG_O_RAN_OPERATIONS_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_operations
 *  @{
 */

/**
 * @brief	(0x811b0111)MSI_O_RAN_OPERATIONS_GET_REQ
 * @note	o-ran-operations GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_OPERATIONS_GET_REQ;

/**
 * @brief	(0x811b0112)o-ran-operations GET応答(OK) -> restart-time
 * @note	restart-time
 * 
 */
typedef struct{
	USHORT				year;								/**< 年  */
	UCHAR				month;								/**< 月  */
	UCHAR				day;								/**< 日  */
	UCHAR				hour;								/**< 時  */
	UCHAR				min;								/**< 分  */
	UCHAR				sec;								/**< 秒  */
	UCHAR				dummy;								/**< dummy  */
}T_MPSW_RESTART_TIME;

/**
 * @brief	(0x811b0112)o-ran-operations GET応答(OK)
 * @note	
 * 
 */
typedef struct{
	E_RESTART_CAUSE		restart_cause;						/**< 再起動要因  */
	T_MPSW_RESTART_TIME	restart_time;						/**< restart-time  */
}T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM;

/**
 * @brief	(0x811b0112)MSI_O_RAN_OPERATIONS_GET_CFM
 * @note	o-ran-operations GET応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_OPERATIONS_GET_CFM;

/**
 * @brief	(0x811b0113)o-ran-operations GET応答(NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_OPERATIONS_GET_REJ;

/**
 * @brief	(0x811b0113)MSI_O_RAN_OPERATIONS_GET_REJ
 * @note	o-ran-operations GET応答(NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_OPERATIONS_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_OPERATIONS_GET_REJ;

/**
 * @brief	(0x811b0211)o-ran-operations EDIT要求
 * @note	
 * 
 */
typedef struct{
	USHORT				callhome_retry_timer;				/**< re-call-home-no-ssh-timer  */
}T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ;

/**
 * @brief	(0x811b0211)MSI_O_RAN_OPERATIONS_EDIT_REQ
 * @note	o-ran-operations EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_OPERATIONS_EDIT_REQ;

/**
 * @brief	(0x811b0212)MSI_O_RAN_OPERATIONS_EDIT_CFM
 * @note	o-ran-operations EDIT応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_OPERATIONS_EDIT_CFM;

/**
 * @brief	(0x811b0213)o-ran-operations EDIT応答(NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REJ;

/**
 * @brief	(0x811b0213)MSI_O_RAN_OPERATIONS_EDIT_REJ
 * @note	o-ran-operations EDIT応答(NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REJ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_OPERATIONS_EDIT_REJ;

/**
 * @brief	(0x811b0615)o-ran-operations RESET要求
 * @note	
 * 
 */
typedef struct{
	E_RESTART_CAUSE		restart_cause;						/**< 再起動要因  */
}T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND;

/**
 * @brief	(0x811b0615)MSI_O_RAN_OPERATIONS_RESET_IND
 * @note	o-ran-operations RESET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_OPERATIONS_RESET_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_OPERATIONS_TYP_H */
