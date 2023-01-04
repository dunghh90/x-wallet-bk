/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_PROCESSING_ELEMENT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_PROCESSING_ELEMENT_TYP_H
#define	F_MPSW_MSG_O_RAN_PROCESSING_ELEMENT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_processing_element
 *  @{
 */

/**
 * @brief	(0x811d0211)o-ran-processing-element　EDIT要求
 * @note	o-ran-processing-element　EDIT要求
 * 
 */
typedef struct{
	UINT				port;								/**< ポート番号  */
	UCHAR				ru_mac_address[6];					/**< RU-MACアドレス  */
	UINT16				vlan_id;							/**< vlan_id  */
	UCHAR				o_du_mac_address[6];				/**< O-DU-MACアドレス  */
}T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ;

/**
 * @brief	(0x811d0211)MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ
 * @note	o-ran-processing-element　EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ;

/**
 * @brief	(0x811d0212)MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM
 * @note	o-ran-processing-element　EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM;

/**
 * @brief	(0x811d0213)o-ran-processing-element　EDIT要求(NG応答)
 * @note	o-ran-processing-element　EDIT要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ;

/**
 * @brief	(0x811d0213)MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ
 * @note	o-ran-processing-element　EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_PROCESSING_ELEMENT_TYP_H */
