/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_SYNC.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_SYNC_TYP_H
#define	F_MPSW_MSG_O_RAN_SYNC_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o-ran_sync
 *  @{
 */

/**
 * @brief	(0x81200101)MSI_O_RAN_SYNC_GET_REQ
 * @note	MSI_O_RAN_SYNC_GET_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SYNC_GET_REQ;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t -> sync_status_sync_t
 * @note	
 * 
 */
typedef struct{
	E_SYNC_STATE		sync_state;							/**< sync-state  */
}T_MPSW_SYNC_STATUS_SYNC_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t -> ptp_status_t -> ptp_source_t
 * @note	
 * 
 */
typedef struct{
	UINT16				local_port_number;					/**< local_port_number  */
	E_PTP_STATE			state;								/**< state  */
	UINT8				twostep_flag;						/**< two-step-flag  */
	UINT8				leap61;								/**< leap61  */
	UINT8				leap59;								/**< leap59  */
	UINT8				current_utc_offset_valid;			/**< current-utc-offset-valid  */
	UINT8				ptp_timescale;						/**< ptp-timescale  */
	UINT8				time_traceable;						/**< time-traceable  */
	UINT8				frequency_traceable;				/**< frequency-traceable  */
	CHAR				source_clock_identity[20];			/**< source-clock-identity  */
	UINT16				source_port_number;					/**< source-port-number  */
	INT16				current_utc_offset;					/**< current-utc-offset  */
	INT8				priority1;							/**< priority1  */
	INT8				clock_class;						/**< clock-class  */
	INT8				clock_accuracy;						/**< clock-accuracy  */
	UINT16				offset_scaled_log_variance;			/**< offset-scaled-log-variance  */
	UINT8				priority2;							/**< priority2  */
	CHAR				grandmaster_clock_identity[20];		/**< grandmaster-clock-identity  */
	UINT16				steps_removed;						/**< steps-removed  */
	UINT8				time_source;						/**< time-source  */
}T_MPSW_PTP_SOURCE_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t -> ptp_status_t
 * @note	
 * 
 */
typedef struct{
	E_PTP_LOCK_STATE	lock_state;							/**< lock-state  */
	UINT8				clock_class;						/**< clock-class  */
	CHAR				clock_identity[20];					/**< clock-identity  */
	T_MPSW_PTP_SOURCE_T	ptp_source_t;						/**< ptp_source_t  */
}T_MPSW_PTP_STATUS_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t -> synce_status_t -> synce_source_t
 * @note	
 * 
 */
typedef struct{
	UINT16				local_port_number;					/**< local_port_number  */
	E_SYNCE_STATE		state;								/**< state  */
	UINT8				quality_level;						/**< quality-level  */
}T_MPSW_SYNCE_SOURCE_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t -> synce_status_t
 * @note	-
 * 
 */
typedef struct{
	E_SYNCE_LOCK_STATE	lockstate;							/**< lock-state  */
	T_MPSW_SYNCE_SOURCE_T	synce_source_t;					/**< synce_source_t  */
}T_MPSW_SYNCE_STATUS_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM -> sync_t
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SYNC_STATUS_SYNC_T	sync_status_sync_t;			/**< sync_status_sync_t  */
	T_MPSW_PTP_STATUS_T	ptp_status_t;						/**< ptp_status_t  */
	T_MPSW_SYNCE_STATUS_T	synce_status_t;					/**< synce_status_t  */
}T_MPSW_SYNC_T;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SYNC_T		sync_t;								/**< sync_t  */
}T_MPSW_MSI_O_RAN_SYNC_GET_CFM;

/**
 * @brief	(0x81200102)MSI_O_RAN_SYNC_GET_CFM
 * @note	MSI_O_RAN_SYNC_GET_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_GET_CFM	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_GET_CFM;

/**
 * @brief	(0x81200201)MSI_O_RAN_SYNC_EDIT_REQ -> ptp_config_t
 * @note	
 * 
 */
typedef struct{
	UINT16				domain_number;						/**< domain-number  */
	E_SYNC_MULTICAST_ADDR	multicast_mac_address;			/**< multicast-mac-address  */
	UINT16				clock_classes;						/**< clock-classes  */
	INT16				delay_asymmetry ;					/**< delay-asymmetry   */
	UINT16				ptp_reporting_period;				/**< reporting_period(PTP)  */
	UINT16				syncE_reporting_period;				/**< reporting_period(SyncE)  */
	E_SYNC_ACCEPTANCE_SSM	syncE_acceptance_ssm[11];		/**< acceptance-list-of-ssm  */
	UINT				syncE_ssm_timeout;					/**< ssm-timeout  */
}T_MPSW_PTP_CONFIG_T;

/**
 * @brief	(0x81200201)MSI_O_RAN_SYNC_EDIT_REQ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_PTP_CONFIG_T	ptp_config_t;						/**< ptp_config_t  */
}T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ;

/**
 * @brief	(0x81200201)MSI_O_RAN_SYNC_EDIT_REQ
 * @note	MSI_O_RAN_SYNC_EDIT_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_EDIT_REQ;

/**
 * @brief	(0x81200202)MSI_O_RAN_SYNC_EDIT_CFM
 * @note	MSI_O_RAN_SYNC_EDIT_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_SYNC_EDIT_CFM;

/**
 * @brief	(0x81200203)MSI_O_RAN_SYNC_EDIT_REJ
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_SYNC_EDIT_REJ;

/**
 * @brief	(0x81200203)MSI_O_RAN_SYNC_EDIT_REJ
 * @note	MSI_O_RAN_SYNC_EDIT_REJ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_EDIT_REJ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_EDIT_REJ;

/**
 * @brief	(0x81200715)synchronization-state-change　Notification通知
 * @note	
 * 
 */
typedef struct{
	E_SYNC_STATE		sync_state;							/**< sync状態  */
}T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81200715)MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND
 * @note	synchronization-state-change　Notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81200725)ptp-state-change　Notification通知
 * @note	
 * 
 */
typedef struct{
	E_PTP_LOCK_STATE	state;								/**< PTP状態  */
}T_MPSW_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81200725)MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND
 * @note	ptp-state-change　Notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81200735)syncE-state-change　Notification通知
 * @note	
 * 
 */
typedef struct{
	E_SYNCE_LOCK_STATE	lockstate;							/**< syncE　状態  */
}T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81200735)MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND
 * @note	syncE-state-change　Notification通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND	data;	/**< Message data  */
}T_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O-RAN_SYNC_TYP_H */
