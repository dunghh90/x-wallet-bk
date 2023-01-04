/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_eval_Server_MSI_O_RAN_LBM.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_LBM_TYP_H
#define	F_MPSW_MSG_O_RAN_LBM_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_lbm
 *  @{
 */

/**
 * @brief	(0x81180211)o-ran-lbm EDIT要求
 * @note	o-ran-lbm EDIT要求
 * 
 */
typedef struct{
	CHAR				id[256];							/**< id  */
	CHAR				name[44];							/**< name  */
	USHORT				md_level;							/**< md-level  */
}T_MPSW_MSI_O_RAN_LBM_EDIT_REQ;

/**
 * @brief	(0x81180211)MSI_O_RAN_LBM_EDIT_REQ
 * @note	o-ran-lbm EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_LBM_EDIT_REQ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_LBM_EDIT_REQ;

/**
 * @brief	(0x81180212)MSI_O_RAN_LBM_EDIT_CFM
 * @note	o-ran-lbm EDIT応答(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_LBM_EDIT_CFM;

/**
 * @brief	(0x81180213)o-ran-lbm EDIT応答(NG応答)
 * @note	o-ran-lbm EDIT応答(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_LBM_EDIT_REJ;

/**
 * @brief	(0x81180213)MSI_O_RAN_LBM_EDIT_REJ
 * @note	o-ran-lbm EDIT応答(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_LBM_EDIT_REJ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_LBM_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_LBM_TYP_H */
