/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_eval_Server_MSI_IETF_IP.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_IETF_IP_TYP_H
#define	F_MPSW_MSG_IETF_IP_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_ietf_ip
 *  @{
 */

/**
 * @brief	(0x81080211)ietf-ip EDIT要求
 * @note	ietf-ip EDIT要求
 * 
 */
typedef struct{
	UINT				ip;									/**< ip  */
	UINT				prefix_length;						/**< prefix-length  */
}T_MPSW_MSI_IETF_IP_EDIT_REQ;

/**
 * @brief	(0x81080211)MSI_IETF_IP_EDIT_REQ
 * @note	ietf-ip EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_IP_EDIT_REQ	data;						/**< Message data  */
}T_MSG_MSI_IETF_IP_EDIT_REQ;

/**
 * @brief	(0x81080212)MSI_IETF_IP_EDIT_CFM
 * @note	ietf-ip EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_IETF_IP_EDIT_CFM;

/**
 * @brief	(0x81080213)ietf-ip EDIT要求(NG応答)
 * @note	ietf-ip EDIT要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_IETF_IP_EDIT_REJ;

/**
 * @brief	(0x81080213)MSI_IETF_IP_EDIT_REJ
 * @note	ietf-ip EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_IP_EDIT_REJ	data;						/**< Message data  */
}T_MSG_MSI_IETF_IP_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_IETF_IP_TYP_H */
