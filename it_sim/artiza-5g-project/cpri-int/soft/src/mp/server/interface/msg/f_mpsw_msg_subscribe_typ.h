/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_eval_Server_MSI_SUBSCRIBE.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_SUBSCRIBE_TYP_H
#define	F_MPSW_MSG_SUBSCRIBE_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_subscribe
 *  @{
 */

/**
 * @brief	(0x81F00001)MSI_SUBSCRIBE_REQ -> module entry情報
 * @note	
 * 
 */
typedef struct{
	UINT				moduleNo;							/**< moduleNo  */
	UINT				moduleOpe;							/**< moduleOperation  */
}T_MPSW_MSI_MODULE_ENTRY;

/**
 * @brief	(0x81F00001)MSI_SUBSCRIBE_REQ
 * @note	MSI Subscribe Request
 * 
 */
typedef struct{
	INT					moduleNum;							/**< module数  */
	T_MPSW_MSI_MODULE_ENTRY	msi_module_entry[256];			/**< module entry情報  */
}T_MPSW_MSI_SUBSCRIBE_REQ;

/**
 * @brief	(0x81F00001)MSI_SUBSCRIBE_REQ
 * @note	MSI_SUBSCRIBE_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_SUBSCRIBE_REQ	data;						/**< Message data  */
}T_MSG_MSI_SUBSCRIBE_REQ;

/**
 * @brief	(0x81F00002)MSI_SUBSCRIBE_CFM
 * @note	MSI Subscribe Confirm
 * 
 */
typedef struct{
	INT					subscriberId;						/**< subscriberId  */
}T_MPSW_MSI_SUBSCRIBE_CFM;

/**
 * @brief	(0x81F00002)MSI_SUBSCRIBE_CFM
 * @note	MSI_SUBSCRIBE_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_SUBSCRIBE_CFM	data;						/**< Message data  */
}T_MSG_MSI_SUBSCRIBE_CFM;

/**
 * @brief	(0x81F00003)MSI_SUBSCRIBE_REJ
 * @note	MSI Subscribe Confirm
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	errorInfo;							/**< MSGERROR  */
}T_MPSW_MSI_SUBSCRIBE_REJ;

/**
 * @brief	(0x81F00003)MSI_SUBSCRIBE_REJ
 * @note	MSI_SUBSCRIBE_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_SUBSCRIBE_REJ	data;						/**< Message data  */
}T_MSG_MSI_SUBSCRIBE_REJ;

/**
 * @brief	(0x81F00005)MSI_DESUBSCRIBE_IND
 * @note	MSI_DESUBSCRIBE_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_DESUBSCRIBE_IND;


/** @} */

#endif	/* F_MPSW_MSG_SUBSCRIBE_TYP_H */
