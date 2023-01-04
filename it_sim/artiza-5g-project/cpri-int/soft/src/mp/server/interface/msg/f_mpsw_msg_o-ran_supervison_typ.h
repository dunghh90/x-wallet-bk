/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_SUPERVISION.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_SUPERVISON_TYP_H
#define	F_MPSW_MSG_O_RAN_SUPERVISON_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_supervison
 *  @{
 */

/**
 * @brief	(0x811f0715)MSI_O_RAN_SUPERVISION_NTF_TIMEOUT_IND
 * @note	supervision notification timeout(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SUPERVISION_NTF_TIMEOUT_IND;

/**
 * @brief	(0x811f0725)MSI_O_RAN_SUPERVISION_NTF_START_IND
 * @note	supervision start indication(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_SUPERVISON_TYP_H */
