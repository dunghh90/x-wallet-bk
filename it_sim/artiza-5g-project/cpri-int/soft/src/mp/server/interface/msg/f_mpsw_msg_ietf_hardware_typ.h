/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_IETF_HARDWARE.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2019
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_IETF_HARDWARE_TYP_H
#define	F_MPSW_MSG_IETF_HARDWARE_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_ietf_hardware
 *  @{
 */

/**
 * @brief	(0x81050111)MSI_IETF_HARDWARE_GET_REQ
 * @note	MSI_IETF_HARDWARE_GET_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_IETF_HARDWARE_GET_REQ;

/**
 * @brief	(0x81050112)ietf-hardware　GET要求(OK応答)　 -> componentテーブル -> mfg-date
 * @note	EEPROMより取得 (0x12～0x15) \n
 * 			->製造年月日を取得し、そのまま引き渡す
 * 
 */
typedef struct{
	USHORT				year;								/**< 年  */
	UCHAR				month;								/**< 月  */
	UCHAR				day;								/**< 日  */
	UCHAR				hour;								/**< 時  */
	UCHAR				min;								/**< 分  */
	UCHAR				sec;								/**< 秒  */
	UCHAR				dummy;								/**< dummy  */
}T_MPSW_MFG_DATE;

/**
 * @brief	(0x81050112)ietf-hardware　GET要求(OK応答)　 -> componentテーブル
 * @note	
 * 
 */
typedef struct{
	INT					physical_index;						/**< physical-index  */
	CHAR				hardware_rev[6];					/**< hardware-rev  */
	UINT16				software_rev;						/**< software-rev  */
	UCHAR				serial_num[4];						/**< serial-num  */
	CHAR				mfg_name[16];						/**< mfg-name  */
	CHAR				model_name[16];						/**< model-name  */
	T_MPSW_MFG_DATE		mfg_date;							/**< mfg-date  */
}T_MPSW_MSI_IETF_COMPONENT;

/**
 * @brief	(0x81050112)ietf-hardware　GET要求(OK応答)　
 * @note	
 * 
 */
typedef struct{
	T_MPSW_MSI_IETF_COMPONENT	msi_ietf_component;			/**< componentテーブル  */
}T_MPSW_MSI_IETF_HARDWARE_GET_CFM;

/**
 * @brief	(0x81050112)MSI_IETF_HARDWARE_GET_CFM
 * @note	ietf-hardware　GET要求(OK応答)　(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_GET_CFM;

/**
 * @brief	(0x81050113)MSI_IETF_HARDWARE_GET_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_IETF_HARDWARE_GET_REJ;

/**
 * @brief	(0x81050113)MSI_IETF_HARDWARE_GET_REJ
 * @note	MSI_IETF_HARDWARE_GET_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_GET_REJ;

/**
 * @brief	(0x81050121)MSI_IETF_HARDWARE_STATE_GET_REQ
 * @note	MSI_IETF_HARDWARE_STATE_GET_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ;

/**
 * @brief	(0x81050122)MSI_IETF_HARDWARE_STATE_GET_CFM -> 状態テーブル -> 状態変更日時
 * @note	yyyy/mm/dd hh:mm:ss形式 (+Padding)
 * 
 */
typedef struct{
	USHORT				year;								/**< 年  */
	UCHAR				month;								/**< 月  */
	UCHAR				day;								/**< 日  */
	UCHAR				hour;								/**< 時  */
	UCHAR				min;								/**< 分  */
	UCHAR				sec;								/**< 秒  */
	UCHAR				dummy;								/**< dummy  */
}T_MPSW_LAST_CHANGED;

/**
 * @brief	(0x81050122)MSI_IETF_HARDWARE_STATE_GET_CFM -> 状態テーブル
 * @note	
 * 
 */
typedef struct{
	T_MPSW_LAST_CHANGED	last_changed;						/**< 状態変更日時  */
	E_MPSW_ADMIN_STATE	admin_state;						/**< admin-state  */
	E_MPSW_OPER_STATE	oper_state;							/**< oper-state  */
	E_MPSW_USAGE_STATE	usage_state;						/**< usage-state  */
}T_MPSW_MSI_IETF_STATE_T;

/**
 * @brief	(0x81050122)MSI_IETF_HARDWARE_STATE_GET_CFM
 * @note	
 * 
 */
typedef struct{
	T_MPSW_MSI_IETF_STATE_T	msi_ietf_state_t;				/**< 状態テーブル  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_GET_CFM;

/**
 * @brief	(0x81050122)MSI_IETF_HARDWARE_STATE_GET_CFM
 * @note	MSI_IETF_HARDWARE_STATE_GET_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_GET_CFM	data;			/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM;

/**
 * @brief	(0x81050123)MSI_IETF_HARDWARE_STATE_GET_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_GET_REJ;

/**
 * @brief	(0x81050123)MSI_IETF_HARDWARE_STATE_GET_REJ
 * @note	MSI_IETF_HARDWARE_STATE_GET_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_GET_REJ	data;			/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_GET_REJ;

/**
 * @brief	(0x81050211)ietf-hardware admin EDIT要求
 * @note	
 * 
 */
typedef struct{
	E_MPSW_ADMIN_STATE	admin_state;						/**< admin-state  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ;

/**
 * @brief	(0x81050211)MSI_IETF_HARDWARE_STATE_EDIT_REQ
 * @note	ietf-hardware admin EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ	data;			/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ;

/**
 * @brief	(0x81050212)MSI_IETF_HARDWARE_STATE_EDIT_CFM
 * @note	ietf-hardware admin EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM;

/**
 * @brief	(0x81050213)ietf-hardware admin EDIT要求(NG応答)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REJ;

/**
 * @brief	(0x81050213)MSI_IETF_HARDWARE_STATE_EDIT_REJ
 * @note	ietf-hardware admin EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REJ	data;			/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REJ;

/**
 * @brief	(0x81050715)notification hardware-state-change通知
 * @note	
 * 
 */
typedef struct{
	E_MPSW_ADMIN_STATE	admin_state;						/**< admin-state   */
}T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81050715)MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND
 * @note	notification hardware-state-change通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND	data;	/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81050725)notification hardware-state-oper-enabled 通知
 * @note	
 * 
 */
typedef struct{
	E_MPSW_ADMIN_STATE	admin_state;						/**< admin-state   */
	E_AVAILABILITY_TYPE	availability_state;					/**< availability-state  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND;

/**
 * @brief	(0x81050725)MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND
 * @note	notification hardware-state-oper-enabled 通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND;

/**
 * @brief	(0x81050735)notification hardware-state-oper-disabled 通知
 * @note	
 * 
 */
typedef struct{
	E_MPSW_ADMIN_STATE	admin_state;						/**< admin-state   */
	E_AVAILABILITY_TYPE	availability_state;					/**< availability-state  */
}T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND;

/**
 * @brief	(0x81050735)MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND
 * @note	notification hardware-state-oper-disabled 通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND;

/**
 * @brief	(0x81050005)MSI_IETF_HARDWARE_STATE_CHANGE_START_IND
 * @note	hardware-state notification start indication(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND;


/** @} */

#endif	/* F_MPSW_MSG_IETF_HARDWARE_TYP_H */
