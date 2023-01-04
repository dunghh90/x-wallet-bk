/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_PERFORMANCE_MANAGEMENT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_PERFORMANCE_MANAGEMENT_TYP_H
#define	F_MPSW_MSG_O_RAN_PERFORMANCE_MANAGEMENT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_performance_management
 *  @{
 */

/**
 * @brief	(0x811C0211)o-ran-performance-management EDIT要求 -> rx_window_measure_t
 * @note	
 * 
 */
typedef struct{
	E_PFM_OBJ_RX_WINDOW	measure_obj;						/**< measurement-object   */
	UINT16				act_flag;							/**< active  */
}T_MPSW_RX_WINDOW_MEASURE_T;

/**
 * @brief	(0x811C0211)o-ran-performance-management EDIT要求
 * @note	o-ran-performance-management EDIT要求
 * 
 */
typedef struct{
	UINT16				mea_interval;						/**< rx-window-measurement-interval  */
	UINT16				ntf_interval;						/**< notification-interval  */
	UINT16				measure_t_num;						/**< measure_t_num  */
	T_MPSW_RX_WINDOW_MEASURE_T	rx_window_measure_t[6];		/**< rx_window_measure_t  */
}T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ;

/**
 * @brief	(0x811C0211)MSI_O_RAN_PFM_MNG_EDIT_REQ
 * @note	o-ran-performance-management EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ;

/**
 * @brief	(0x811C0212)MSI_O_RAN_PFM_MNG_EDIT_CFM
 * @note	o-ran-performance-management EDIT OK応答(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM;

/**
 * @brief	(0x811C0213)o-ran-performance-management EDIT NG応答
 * @note	o-ran-performance-management EDIT NG応答
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REJ;

/**
 * @brief	(0x811C0213)MSI_O_RAN_PFM_MNG_EDIT_REJ
 * @note	o-ran-performance-management EDIT NG応答(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ;

/**
 * @brief	(0x811C0121)MSI_O_RAN_PFM_MNG_GET_REQ
 * @note	o-ran-performance-management GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_PFM_MNG_GET_REQ;

/**
 * @brief	(0x811C0122)o-ran-performance-management GET応答 -> rx_window_measure_get_t
 * @note	
 * 
 */
typedef struct{
	E_PFM_OBJ_RX_WINDOW	measure_obj;						/**< measurement-object   */
	UINT16				act_flag;							/**< active  */
	UINT64				ru_count;							/**< ru_count  */
	UINT64				tr_count[2];						/**< tr_count  */
}T_MPSW_RX_WINDOW_MEASURE_GET_T;

/**
 * @brief	(0x811C0122)o-ran-performance-management GET応答
 * @note	o-ran-performance-management GET応答
 * 
 */
typedef struct{
	T_MPSW_RX_WINDOW_MEASURE_GET_T	rx_window_measure_get_t[6];
															/**< rx_window_measure_get_t  */
}T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM;

/**
 * @brief	(0x811C0122)MSI_O_RAN_PFM_MNG_GET_CFM
 * @note	o-ran-performance-management GET応答(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_PFM_MNG_GET_CFM;

/**
 * @brief	(0x811C0123)o-ran-performance-management GET NG応答
 * @note	o-ran-performance-management GET NG応答
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_PFM_MNG_GET_REJ;

/**
 * @brief	(0x811C0123)MSI_O_RAN_PFM_MNG_GET_REJ
 * @note	o-ran-performance-management GET NG応答(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PFM_MNG_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_PFM_MNG_GET_REJ;

/**
 * @brief	(0x811C0735)measurement-result-stats 通知 -> rx_window_result_t -> start-time
 * @note	
 * 
 */
typedef struct{
	UINT16				year;								/**< 年  */
	UINT8				month;								/**< 月  */
	UINT8				day;								/**< 日  */
	UINT8				hour;								/**< 時  */
	UINT8				min;								/**< 分  */
	UINT8				sec;								/**< 秒  */
	UINT8				dummy;								/**< dummy  */
}T_MPSW_START_TIME;

/**
 * @brief	(0x811C0735)measurement-result-stats 通知 -> rx_window_result_t -> end-time
 * @note	
 * 
 */
typedef struct{
	UINT16				year;								/**< 年  */
	UINT8				month;								/**< 月  */
	UINT8				day;								/**< 日  */
	UINT8				hour;								/**< 時  */
	UINT8				min;								/**< 分  */
	UINT8				sec;								/**< 秒  */
	UINT8				dummy;								/**< dummy  */
}T_MPSW_END_TIME;

/**
 * @brief	(0x811C0735)measurement-result-stats 通知 -> rx_window_result_t
 * @note	
 * 
 */
typedef struct{
	E_PFM_OBJ_RX_WINDOW	measure_obj;						/**< measurement-object   */
	T_MPSW_START_TIME	start_time;							/**< start-time  */
	T_MPSW_END_TIME		end_time;							/**< end-time  */
	UINT64				ru_count;							/**< ru_count  */
	UINT64				tr_count[2];						/**< tr_count  */
}T_MPSW_RX_WINDOW_RESULT_T;

/**
 * @brief	(0x811C0735)measurement-result-stats 通知
 * @note	measurement-result-stats 通知
 * 
 */
typedef struct{
	UINT16				measure_t_num;						/**< measure_t_num  */
	T_MPSW_RX_WINDOW_RESULT_T	rx_window_result_t[6];		/**< rx_window_result_t  */
}T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND;

/**
 * @brief	(0x811C0735)MSI_O_RAN_PFM_MNG_NTF_IND
 * @note	measurement-result-stats 通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_PFM_MNG_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_PERFORMANCE_MANAGEMENT_TYP_H */
