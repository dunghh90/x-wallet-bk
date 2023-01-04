/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_HARDWARE.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_HARDWARE_TYP_H
#define	F_MPSW_MSG_O_RAN_HARDWARE_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_hardware
 *  @{
 */

/**
 * @brief	(0x81160111)MSI_O_RAN_HARDWARE_GET_REQ
 * @note	o-ran-hardware GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_HARDWARE_GET_REQ;

/**
 * @brief	(0x81160112)o-ran-hardware GET要求(OK応答)
 * @note	o-ran-hardware GET要求(OK応答)
 * 
 */
typedef struct{
	CHAR				product_code[16];					/**< product-code  */
	E_HARD_POW_STAT		power_state;						/**< power-state  */
	E_AVAILABILITY_TYPE	availability_state;					/**< availability-state  */
}T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM;

/**
 * @brief	(0x81160112)MSI_O_RAN_HARDWARE_GET_CFM
 * @note	o-ran-hardware GET要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_HARDWARE_GET_CFM;

/**
 * @brief	(0x81160113)o-ran-hardware GET要求(NG応答)
 * @note	o-ran-hardware GET要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr[1];							/**< エラー情報  */
}T_MPSW_MSI_O_RAN_HARDWARE_GET_REJ;

/**
 * @brief	(0x81160113)MSI_O_RAN_HARDWARE_GET_REJ
 * @note	o-ran-hardware GET要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_HARDWARE_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_HARDWARE_GET_REJ;

/**
 * @brief	(0x81160211)o-ran-hardware EDIT要求(商用のみ)
 * @note	o-ran-hardware EDIT要求(商用のみ)
 * 
 */
typedef struct{
	E_HARD_POW_STAT		energy_saving_enabled;				/**< energy-saving-enabled  */
}T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ;

/**
 * @brief	(0x81160211)MSI_O_RAN_HARDWARE_EDIT_REQ
 * @note	o-ran-hardware EDIT要求(商用のみ)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_HARDWARE_EDIT_REQ;

/**
 * @brief	(0x81160212)MSI_O_RAN_HARDWARE_EDIT_CFM
 * @note	o-ran-hardware EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_HARDWARE_EDIT_CFM;

/**
 * @brief	(0x81160213)o-ran-hardware EDIT要求(NG応答)
 * @note	o-ran-hardware EDIT要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr[1];							/**< エラー情報  */
}T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REJ;

/**
 * @brief	(0x81160213)MSI_O_RAN_HARDWARE_EDIT_REJ
 * @note	o-ran-hardware EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_HARDWARE_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_HARDWARE_TYP_H */
