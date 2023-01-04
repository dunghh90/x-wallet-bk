/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_eval_Server_MSI_READY.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_READY_TYP_H
#define	F_MPSW_MSG_READY_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_ready
 *  @{
 */

/**
 * @brief	(0x81F10005)MSI_READY_IND
 * @note	MSI_READY_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_READY_IND;


/** @} */

#endif	/* F_MPSW_MSG_READY_TYP_H */
