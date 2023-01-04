/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_USERMGMT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_USERMGMT_TYP_H
#define	F_MPSW_MSG_O_RAN_USERMGMT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_usermgmt
 *  @{
 */

/**
 * @brief	(0x81010211)o-ran-usermgmt EDIT要求
 * @note	o-ran-usermgmt EDIT要求
 * 
 */
typedef struct{
	CHAR				user[64];							/**< ユーザ名  */
	CHAR				password[256];						/**< パスワード  */
	UINT				enabled;							/**< enabled  */
}T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ;

/**
 * @brief	(0x81010211)MSI_O_RAN_USERMGMT_EDIT_REQ
 * @note	o-ran-usermgmt EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ;

/**
 * @brief	(0x81010212)MSI_O_RAN_USERMGMT_EDIT_CFM
 * @note	o-ran-usermgmt EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM;

/**
 * @brief	(0x81010213)o-ran-usermgmt EDIT要求(NG応答)
 * @note	o-ran-usermgmt EDIT要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ;

/**
 * @brief	(0x81010213)MSI_O_RAN_USERMGMT_EDIT_REJ
 * @note	o-ran-usermgmt EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_USERMGMT_TYP_H */
