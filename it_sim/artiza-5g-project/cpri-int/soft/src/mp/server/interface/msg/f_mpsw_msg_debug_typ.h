/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_DEBUG.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_DEBUG_TYP_H
#define	F_MPSW_MSG_DEBUG_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_debug
 *  @{
 */

/**
 * @brief	(0x81FF0005)MSI_DEBUG_IND
 * @note	MSI Desubscribe Indication
 * 
 */
typedef struct{
	UINT				opeCode;							/**< operationCode  */
	UINT				optLen;								/**< option length  */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< MSI擬似エラー情報  */
}T_MPSW_MSI_DEBUG_IND;

/**
 * @brief	(0x81FF0005)MSI_DEBUG_IND
 * @note	MSI_DEBUG_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_DEBUG_IND	data;							/**< Message data  */
}T_MSG_MSI_DEBUG_IND;


/** @} */

#endif	/* F_MPSW_MSG_DEBUG_TYP_H */
