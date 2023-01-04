/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_FM.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2019
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_FM_TYP_H
#define	F_MPSW_MSG_O_RAN_FM_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_fm
 *  @{
 */

/**
 * @brief	(0x81150715)notification alarm-notif 通知 -> affected-objects
 * @note	emun型
 * 
 */
typedef struct{
	E_FAULT_SOURCE		name[1];							/**< name  */
}T_MPSW_AFFECTED_OBJECTS;

/**
 * @brief	(0x81150715)notification alarm-notif 通知
 * @note	notification alarm-notif 通知
 * 
 */
typedef struct{
	UINT16				fault_id;							/**< fault-id  */
	E_FAULT_SOURCE		fault_source;						/**< fault-source  */
	E_FAULT_SEVERITY	fault_severity;						/**< fault-severity  */
	bool				is_cleared;							/**< is-cleared  */
	CHAR				fault_text[64];						/**< fault-text  */
	UCHAR				name_num;							/**< affected-objects数  */
	T_MPSW_AFFECTED_OBJECTS	affected_objects;				/**< affected-objects  */
}T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND;

/**
 * @brief	(0x81150715)MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND
 * @note	notification alarm-notif 通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND;

/**
 * @brief	(0x81150005)MSI_O_RAN_FM_ALARM_START_IND
 * @note	o-ran-fm alarm notification start indication(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_FM_ALARM_START_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_FM_TYP_H */
