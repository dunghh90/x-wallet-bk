/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_SOFTWARE_MANAGEMENT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_SOFTWARE_MANAGEMENT_TYP_H
#define	F_MPSW_MSG_O_RAN_SOFTWARE_MANAGEMENT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_software_management
 *  @{
 */

/**
 * @brief	(0x811e0111)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ;

/**
 * @brief	(0x811e0112)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM -> software-inventory -> software-slot -> files
 * @note	スロット内のファイルのリスト。 \n
 * 			配列数は可変でfile_numの数だけ作成する。
 * 
 */
typedef struct{
	CHAR				name[256];							/**< name  */
	CHAR				version[16];						/**< version  */
	CHAR				local_path[256];					/**< local-path  */
	E_SWM_SLOT_INTEGRITY	integrity;						/**< integrity  */
}T_MPSW_FILE;

/**
 * @brief	(0x811e0112)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM -> software-inventory -> software-slot
 * @note	要素数は2固定。
 * 
 */
typedef struct{
	CHAR				name[16];							/**< name  */
	E_SWM_SLOT_STATUS	status;								/**< status  */
	bool				active;								/**< active  */
	bool				running;							/**< running  */
	E_SWM_SLOT_ACCESS	access;								/**< access  */
	CHAR				product_code[16];					/**< product-code  */
	CHAR				vendor_code[16];					/**< vendor-code  */
	CHAR				build_id[16];						/**< build-id  */
	CHAR				build_name[32];						/**< build-name  */
	CHAR				build_version[16];					/**< build-version  */
	UCHAR				file_num;							/**< file_num  */
	T_MPSW_FILE			file[1];							/**< files  */
}T_MPSW_SOFTWARE_SLOT;

/**
 * @brief	(0x811e0112)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM -> software-inventory
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SOFTWARE_SLOT	software_slot[2];				/**< software-slot  */
}T_MPSW_SOFTWARE_INVENTORY;

/**
 * @brief	(0x811e0112)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SOFTWARE_INVENTORY	software_inventory;			/**< software-inventory  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM;

/**
 * @brief	(0x811e0112)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM;

/**
 * @brief	(0x811e0113)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ;

/**
 * @brief	(0x811e0113)MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ;

/**
 * @brief	(0x811e0611)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ
 * @note	
 * 
 */
typedef struct{
	CHAR				uri[256];							/**< remote-file-path  */
	CHAR				password[256];						/**< password  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ;

/**
 * @brief	(0x811e0611)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ;

/**
 * @brief	(0x811e0612)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM
 * @note	
 * 
 */
typedef struct{
	UINT				notification_timeout;				/**< notification-timeout  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM;

/**
 * @brief	(0x811e0612)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM;

/**
 * @brief	(0x811e0613)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ;

/**
 * @brief	(0x811e0613)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ;

/**
 * @brief	(0x811e0621)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ
 * @note	
 * 
 */
typedef struct{
	CHAR				slot_name[16];						/**< slot-name  */
	UCHAR				file_num;							/**< file-num  */
	CHAR				file_name[1][256];					/**< file-names  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ;

/**
 * @brief	(0x811e0621)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ;

/**
 * @brief	(0x811e0622)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM;

/**
 * @brief	(0x811e0623)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ;

/**
 * @brief	(0x811e0623)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ;

/**
 * @brief	(0x811e0631)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ
 * @note	
 * 
 */
typedef struct{
	CHAR				slot_name[16];						/**< slot-name  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ;

/**
 * @brief	(0x811e0631)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ;

/**
 * @brief	(0x811e0632)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM
 * @note	
 * 
 */
typedef struct{
	UINT				notification_timeout;				/**< notification-timeout  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM;

/**
 * @brief	(0x811e0632)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM;

/**
 * @brief	(0x811e0633)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ;

/**
 * @brief	(0x811e0633)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ;

/**
 * @brief	(0x811e0715)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND
 * @note	
 * 
 */
typedef struct{
	CHAR				file_name[256];						/**< file-name  */
	E_SWM_DOWNLOAD_STATUS	status;							/**< status  */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND;

/**
 * @brief	(0x811e0715)MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NFT_IND
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND;

/**
 * @brief	(0x811e0725)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND
 * @note	
 * 
 */
typedef struct{
	CHAR				slot_name[16];						/**< slot-name  */
	E_SWM_INSTALL_STATUS	status;							/**< status  */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND;

/**
 * @brief	(0x811e0725)MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NFT_IND
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND;

/**
 * @brief	(0x811e0735)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND
 * @note	
 * 
 */
typedef struct{
	CHAR				slot_name[16];						/**< slot-name  */
	E_SWM_ACTIVATE_STATUS	status;							/**< status  */
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND;

/**
 * @brief	(0x811e0735)MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NFT_IND
 * @note	MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_SOFTWARE_MANAGEMENT_TYP_H */
