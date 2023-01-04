/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_FILE_MANAGEMENT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_FILE_MANAGEMENT_TYP_H
#define	F_MPSW_MSG_O_RAN_FILE_MANAGEMENT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_file_management
 *  @{
 */

/**
 * @brief	(0x81140611)o-ran-filemanagement ファイルアップロード要求
 * @note	
 * 
 */
typedef struct{
	CHAR				local_logical_file_path[256];		/**< local-logical-file-path  */
	CHAR				remote_file_path [256];				/**< remote-file-path   */
	CHAR				password[64];						/**< password  */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ;

/**
 * @brief	(0x81140611)MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ
 * @note	o-ran-filemanagement ファイルアップロード要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ;

/**
 * @brief	(0x81140612)MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM
 * @note	o-ran-filemanagement ファイルアップロード要求応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM;

/**
 * @brief	(0x81140613)o-ran-filemanagement ファイルアップロード要求応答(NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ;

/**
 * @brief	(0x81140613)MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ
 * @note	o-ran-filemanagement ファイルアップロード要求応答(NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ;

/**
 * @brief	(0x81140621)o-ran-filemanagement ファイルリスト取得要求
 * @note	
 * 
 */
typedef struct{
	CHAR				logical_path[256];					/**< logical-path  */
	CHAR				file_name_filter [256];				/**< file-name-filter   */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ;

/**
 * @brief	(0x81140621)MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ
 * @note	o-ran-filemanagement ファイルリスト取得要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ;

/**
 * @brief	(0x81140622)o-ran-filemanagement ファイルリスト取得要求応答(OK)
 * @note	
 * 
 */
typedef struct{
	CHAR				file_name[256];						/**< filelist  */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM;

/**
 * @brief	(0x81140622)MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM
 * @note	o-ran-filemanagement ファイルリスト取得要求応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM;

/**
 * @brief	(0x81140623)o-ran-filemanagement ファイルアップロード要求応答(NG)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ;

/**
 * @brief	(0x81140623)MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ
 * @note	o-ran-filemanagement ファイルアップロード要求応答(NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ;

/**
 * @brief	(0x81140715)o-ran-filemanagement file-upload notification
 * @note	
 * 
 */
typedef struct{
	E_FLM_STATUS		status;								/**< status  */
	CHAR				local_logical_file_path[256];		/**< local-logical-file-path  */
	CHAR				remote_file_path [256];				/**< remote-file-path   */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND;

/**
 * @brief	(0x81140715)MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND
 * @note	o-ran-filemanagement file-upload notification(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_FILE_MANAGEMENT_TYP_H */
