/*
 * @file    f_mpsw_msg_enum.h
 */

#ifndef _H_f_mpsw_msg_enum
#define _H_f_mpsw_msg_enum


/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
/* F_MPSW_MSG_IETF_HARDWARE_DEF_H */
typedef enum
{
	E_MPSW_ADMIN_STATE_UNKNOWN = 0,							/**< Admin状態 = unknown			*/
	E_MPSW_ADMIN_STATE_LOCKED,								/**< Admin状態 = locked				*/
	E_MPSW_ADMIN_STATE_SHUTTINGDOWN,						/**< Admin状態 = shutting-down		*/
	E_MPSW_ADMIN_STATE_UNLOCKED								/**< Admin状態 = unlocked"			*/
}E_MPSW_ADMIN_STATE;

typedef enum
{
	E_MPSW_OPER_STATE_UNKNOWN = 0 ,							/**< Operationaly状態 = unknown		*/
	E_MPSW_OPER_STATE_DISABLED ,							/**< Operationaly状態 = disabled	*/
	E_MPSW_OPER_STATE_ENABLED ,								/**< Operationaly状態 = enabled		*/
	E_MPSW_OPER_STATE_TESTING								/**< Operationaly状態 = testing"	*/
}E_MPSW_OPER_STATE;

typedef enum
{
	E_MPSW_USAGE_STATE_UNKNOWN = 0 ,						/**< Usage状態 = unknown			*/
	E_MPSW_USAGE_STATE_IDLE ,								/**< Usage状態 = idle				*/
	E_MPSW_USAGE_STATE_ACTIVE ,								/**< Usage状態 = active				*/
	E_MPSW_USAGE_STATE_BUSY									/**< Usage状態 = busy				*/
}E_MPSW_USAGE_STATE;

typedef enum
{
	E_MPSW_ALARM_STATE_UNKNOWN = 0 ,						/**< Alarm状態 = unknown			*/
	E_MPSW_ALARM_STATE_UNDER_REPAIR ,						/**< Alarm状態 = under-repair		*/
	E_MPSW_ALARM_STATE_CRITICAL ,							/**< Alarm状態 = critical			*/
	E_MPSW_ALARM_STATE_MAJOR ,								/**< Alarm状態 = major				*/
	E_MPSW_ALARM_STATE_MINOR ,								/**< Alarm状態 = minor				*/
	E_MPSW_ALARM_STATE_WARNING ,							/**< Alarm状態 = warning			*/
	E_MPSW_ALARM_STATE_INDETERMINATE						/**< Alarm状態 = indeterminate"		*/
}E_MPSW_ALARM_STATE;

typedef enum
{
	E_AVAILABILITY_TYPE_UNKNOWN = 0,						/**< Availability状態 = unknown		*/
	E_AVAILABILITY_TYPE_NORMAL ,							/**< Availability状態 = normal		*/
	E_AVAILABILITY_TYPE_DEGRADED ,							/**< Availability状態 = degraded	*/
	E_AVAILABILITY_TYPE_FAULTY								/**< Availability状態 = faulty		*/
}E_AVAILABILITY_TYPE;
/* F_MPSW_MSG_IETF_HARDWARE_DEF_H */


/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */
typedef enum
{
	E_UP_CAR_ACTIVE_INACTIVE = 0,							/**< Carrier OFF						*/
	E_UP_CAR_ACTIVE_ACTIVE,									/**< Carrier ON							*/
    E_UP_CAR_ACTIVE_SLEEP                                   /**< Carrier SLEEP                      */
}E_UP_CAR_ACTIVE;
/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */

/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */
typedef enum
{
	E_UP_CAR_DEF_SCS_KHZ_15 = 0,							/**< DEF SCS 15kHZ						*/
	E_UP_CAR_DEF_SCS_KHZ_30,								/**< DEF SCS 30kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_60,								/**< DEF SCS 60kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_120,								/**< DEF SCS 120kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_240,								/**< DEF SCS 240kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_1_25,								/**< DEF SCS 125kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_5,									/**< DEF SCS 5kHz						*/
	E_UP_CAR_DEF_SCS_KHZ_EMPTY								/**< DEF SCS is not set					*/
}E_UP_DEF_SCS ;
/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */

/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */
typedef enum
{
	E_UP_CAR_SCS_KHZ_15 = 0,								/**< SCS 15kHZ							*/
	E_UP_CAR_SCS_KHZ_30,									/**< SCS 30kHz							*/
	E_UP_CAR_SCS_KHZ_60,									/**< SCS 60kHz							*/
	E_UP_CAR_SCS_KHZ_120,									/**< SCS 120kHz							*/
	E_UP_CAR_SCS_KHZ_240,									/**< SCS 240kHz							*/
	E_UP_CAR_SCS_KHZ_1_25,									/**< SCS 125kHz							*/
	E_UP_CAR_SCS_KHZ_5,										/**< SCS 5kHz							*/
	E_UP_CAR_SCS_KHZ_EMPTY									/**< SCS is not set						*/
}E_UP_SCS;
/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */

/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */
typedef enum
{
	E_RFCP_UPCNF_STS_DISABLE = 0 , 							/**<  U-Plane Conf. State = Disable 	*/
	E_RFCP_UPCNF_STS_BUSY , 								/**<  U-Plane Conf. State = Busy 		*/
	E_RFCP_UPCNF_STS_READY 									/**<  U-Plane Conf. State = Ready 		*/
}E_RFCP_UPCNF_STS;
/* F_MPSW_MSG_O_RAN_UPLANE_CONF_DEF_H */

typedef	enum
{
	E_MDULE_CAPA_CATEGRY_CAT_A = 0,							/**< Category CAT-A						*/
	E_MDULE_CAPA_CATEGRY_CAT_B 								/**< Category CAT-B						*/
}E_MDULE_CAPA_CATEGRY ;

typedef enum
{
	E_MPSW_ERR_CODE_PARAMETER_ERROR = 1,					/**< MSI error code = Parameter Error	*/
	E_MPSW_ERR_CODE_RESOURCE_BUSY,							/**< MSI error code = Resource Busy		*/
	E_MPSW_ERR_CODE_HW_ACCESS_ERROR							/**< MSI error code = Hardware Access error	*/
}E_MPSW_ERR_CODE;


/* F_MPSW_MSG_O_RAN_SYNC_DEF_H */
typedef enum
{
	E_PTP_STATE_DISABLE = 0,
	E_PTP_STATE_PARENT,
	E_PTP_STATE_OK,
	E_PTP_STATE_NOK
}E_PTP_STATE;

typedef enum
{
	E_SYNC_STATE_FREERUN = 0,
	E_SYNC_STATE_LOCKED,
	E_SYNC_STATE_HOLDOVER
}E_SYNC_STATE;

typedef enum
{
	E_PTP_STATE_UNLOCKED = 0,
	E_PTP_STATE_LOCKED
}E_PTP_LOCK_STATE;

typedef enum
{
	E_SYNCE_LOCK_STATE_UNLOCKED = 0,
	E_SYNCE_LOCK_STATE_LOCKED
}E_SYNCE_LOCK_STATE;

typedef enum
{
	E_SYNCE_STATE_DISABLE = 0,
	E_SYNCE_STATE_PARENT,
	E_SYNCE_STATE_OK,
	E_SYNCE_STATE_NOK
}E_SYNCE_STATE;

typedef enum
{
	E_SYNC_MULTICAST_ADDR_FORWARDABLE = 0,
	E_SYNC_MULTICAST_ADDR_NONFORWARDABLE,
	E_SYNC_MULTICAST_ADDR_UNCAHNGE = 0xff
}E_SYNC_MULTICAST_ADDR;

typedef enum
{
	E_SYNC_ACCEPTANCE_SSM_PRC = 0,
	E_SYNC_ACCEPTANCE_SSM_PRS,
	E_SYNC_ACCEPTANCE_SSM_SSU_A,
	E_SYNC_ACCEPTANCE_SSM_SSU_B,
	E_SYNC_ACCEPTANCE_SSM_ST2,
	E_SYNC_ACCEPTANCE_SSM_ST3,
	E_SYNC_ACCEPTANCE_SSM_ST3E,
	E_SYNC_ACCEPTANCE_SSM_EEC1,
	E_SYNC_ACCEPTANCE_SSM_EEC2,
	E_SYNC_ACCEPTANCE_SSM_DNU,
	E_SYNC_ACCEPTANCE_SSM_NONE,
	E_SYNC_ACCEPTANCE_SSM_UNCAHNGE = 0xff
}E_SYNC_ACCEPTANCE_SSM;
/* F_MPSW_MSG_O_RAN_SYNC_DEF_H */

typedef enum
{
	E_USER_GROUP_EMPTY = 0,
	E_USER_GROUP_SUDO,
}E_USER_GROUP;

/* F_MPSW_MSG_O_RAN_OPERATIONS_DEF_H */
typedef enum
{
	E_RESTART_CAUSE_POWER_ON = 0,
	E_RESTART_CAUSE_SUPERVISION_WATCHDOG,
	E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART,
	E_RESTART_CAUSE_SOFTWARE_FAILURE,
	E_RESTART_CAUSE_OTHER_WATCHDOG_TIMER,
	E_RESTART_CAUSE_UNKNOWN
}E_RESTART_CAUSE;
/* F_MPSW_MSG_O_RAN_OPERATIONS_DEF_H */

/* F_MPSW_MSG_O_RAN_FM_DEF_H */
typedef enum
{
	E_FAULT_SOURCE_LLSDU = 0,
	E_FAULT_SOURCE_PORT_0,
	E_FAULT_SOURCE_PORT_1,
	E_FAULT_SOURCE_TX_ANT_A,
	E_FAULT_SOURCE_TX_ANT_B,
	E_FAULT_SOURCE_TX_ANT_C,
	E_FAULT_SOURCE_TX_ANT_D,
	E_FAULT_SOURCE_TX_ANT_E,
	E_FAULT_SOURCE_TX_ANT_F,
	E_FAULT_SOURCE_TX_ANT_G,
	E_FAULT_SOURCE_TX_ANT_H,
	E_FAULT_SOURCE_RX_ANT_A,
	E_FAULT_SOURCE_RX_ANT_B,
	E_FAULT_SOURCE_RX_ANT_C,
	E_FAULT_SOURCE_RX_ANT_D,
	E_FAULT_SOURCE_RX_ANT_E,
	E_FAULT_SOURCE_RX_ANT_F,
	E_FAULT_SOURCE_RX_ANT_G,
	E_FAULT_SOURCE_RX_ANT_H,
    E_FAULT_SOURCE_FAN_1,
    E_FAULT_SOURCE_FAN_2,
    E_FAULT_SOURCE_FAN_3,
	E_FAULT_SOURCE_UNKOWN
}E_FAULT_SOURCE;

typedef enum
{
	E_FAULT_SEVERITY_CRITICAL = 0,
	E_FAULT_SEVERITY_MAJOR,
	E_FAULT_SEVERITY_MINOR
}E_FAULT_SEVERITY;
/* F_MPSW_MSG_O_RAN_FM_DEF_H */

typedef enum {

	E_MPSW_PFM_RXWIN_OBJ_RX_ON_TIME = 0,				/**< rx-window-measurement : RX_ON_TIME	*/
	E_MPSW_PFM_RXWIN_OBJ_RX_EARLY,						/**< rx-window-measurement : RX_EARLY	*/
	E_MPSW_PFM_RXWIN_OBJ_RX_LATE,						/**< rx-window-measurement : RX_LATE	*/
	E_MPSW_PFM_RXWIN_OBJ_RX_CORRUPT,					/**< rx-window-measurement : RX_CORRUPT	*/
	E_MPSW_PFM_RXWIN_OBJ_RX_DUPL,						/**< rx-window-measurement : RX_DUPL	*/
	E_MPSW_PFM_RXWIN_OBJ_RX_TOTAL,						/**< rx-window-measurement : RX_TOTAL	*/
}E_PFM_OBJ_RX_WINDOW;
/* F_MPSW_MSG_O_RAN_PERFORMANCE_MANAGEMENT_DEF_H */

/* F_MPSW_MSG_O_RAN_SOFTWARE_MANAGEMENT_H */
typedef enum
{
	E_SWM_SLOT_STATUS_VALID = 0,
	E_SWM_SLOT_STATUS_INVALID,
	E_SWM_SLOT_STATUS_EMPTY
} E_SWM_SLOT_STATUS;

typedef enum
{
	E_SWM_SLOT_ACCESS_RO = 0,
	E_SWM_SLOT_ACCESS_RW
} E_SWM_SLOT_ACCESS;

typedef enum
{
	E_SWM_SLOT_INTEGRITY_OK = 0,
	E_SWM_SLOT_INTEGRITY_NOK
} E_SWM_SLOT_INTEGRITY;


typedef enum
{
	E_SWM_DOWNLOAD_STATUS_COMPLETED = 0,
	E_SWM_DOWNLOAD_STATUS_AUTHENTICATION_ERROR,
	E_SWM_DOWNLOAD_STATUS_PROTOCOL_ERROR,
	E_SWM_DOWNLOAD_STATUS_FILE_NOT_FOUND,
	E_SWM_DOWNLOAD_STATUS_APPLICATION_ERROR,
	E_SWM_DOWNLOAD_STATUS_TIMEOUT
} E_SWM_DOWNLOAD_STATUS;


typedef enum
{
	E_SWM_INSTALL_STATUS_COMPLETED = 0,
	E_SWM_INSTALL_STATUS_FILE_ERROR,
	E_SWM_INSTALL_STATUS_INTEGRITY_ERROR,
	E_SWM_INSTALL_STATUS_APPLICATION_ERROR
} E_SWM_INSTALL_STATUS;

typedef enum
{
	E_SWM_ACTIVATE_STATUS_COMPLETED = 0,
	E_SWM_ACTIVATE_STATUS_APPLICATION_ERROR
} E_SWM_ACTIVATE_STATUS;

/* F_MPSW_MSG_O_RAN_SOFTWARE_MANAGEMENT_H */

/* F_MPSW_MSG_O_RAN_FILE_MANAGEMENT_H */
typedef enum
{
	E_FLM_STATUS_SUCCESS = 0,
	E_FLM_STATUS_FAILURE
} E_FLM_STATUS;


/* F_MPSW_MSG_O_RAN_FILE_MANAGEMENT_H */

/* F_MPSW_MSG_O_RAN_HARDWARE_H */
typedef enum
{
    E_HARD_POW_STAT_UNKNOWN = 0,
    E_HARD_POW_STAT_SLEEPING,
    E_HARD_POW_STAT_AWAKE
} E_HARD_POW_STAT;
/* F_MPSW_MSG_O_RAN_HARDWARE_H */
#endif
