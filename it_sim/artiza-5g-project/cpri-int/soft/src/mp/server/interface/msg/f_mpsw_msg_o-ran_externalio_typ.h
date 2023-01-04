/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_EXTERNALIO.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_EXTERNALIO_TYP_H
#define	F_MPSW_MSG_O_RAN_EXTERNALIO_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_externalio
 *  @{
 */

/**
 * @brief	(0x81120111)o-ran-exteralio.yang　GET要求
 * @note	
 * 
 */
typedef struct{
	UINT				port_num;							/**< Port番号  */
}T_MPSW_MSI_O_RAN_EXTERNALIO_GET_REQ;

/**
 * @brief	(0x81120111)MSI_O_RAN_EXTERNALIO_GET_REQ
 * @note	o-ran-exteralio.yang　GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_EXTERNALIO_GET_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ;

/**
 * @brief	(0x81120112)o-ran-exteralio.yang　GET応答(OK)
 * @note	
 * 
 */
typedef struct{
	UINT				line_in;							/**< Port入力値  */
	UINT				line_out;							/**< Port出力値  */
}T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM;

/**
 * @brief	(0x81120112)MSI_O_RAN_EXTERNALIO_GET_CFM
 * @note	o-ran-exteralio.yang　GET応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM;

/**
 * @brief	(0x81120211)o-ran-exteralio.yang　EDIT要求
 * @note	
 * 
 */
typedef struct{
	UINT				port_num;							/**< Port番号  */
	UINT				line_out;							/**< Port出力値  */
}T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ;

/**
 * @brief	(0x81120211)MSI_O_RAN_EXTERNALIO_EDIT_REQ
 * @note	o-ran-exteralio.yang　EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ;

/**
 * @brief	(0x81120212)MSI_O_RAN_EXTERNALIO_EDIT_CFM
 * @note	o-ran-exteralio.yang　EDIT応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM;

/**
 * @brief	(0x81120715)o-ran-exteralio.yang　exteral-input-change　Notification通知
 * @note	
 * 
 */
typedef struct{
	UINT				port_num;							/**< Port番号  */
	UINT				line_in;							/**< Port入力値  */
}T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND;

/**
 * @brief	(0x81120715)MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND
 * @note	o-ran-exteralio.yang　exteral-input-change　Notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND;

/**
 * @brief	(0x81120005)MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND
 * @note	o-ran-exteralio.yang　exteral-input-change　Notification　start通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_EXTERNALIO_TYP_H */
