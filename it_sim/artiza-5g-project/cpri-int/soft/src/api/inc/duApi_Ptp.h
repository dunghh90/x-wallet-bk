/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API common part)
 *  @note	Struct Definition(RRH API common part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_PTP_H
#define RRHAPI_PTP_H
/*!
 * @addtogroup RRH_API_PTP
 * @{
 */

#define D_API_MSGID_PTP_BASE						0xA00E0000
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_START
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_START_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is...
 */
typedef struct{
	INT								Result;			/**< 処理結果			*/
}T_API_PTP_START_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_PTP_START_RSP_DATA		data;
}T_API_PTP_START_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_START_REQ					(D_API_MSGID_PTP_BASE + 0x00000001)
#define D_API_MSGID_PTP_START_RSP					(D_API_MSGID_PTP_BASE + 0x00000002)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_PtpStart(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_SHUTDOWN
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_SHUTDOWN_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is...
 */
typedef struct{
	INT								Result;			/**< 処理結果			*/
}T_API_PTP_SHUTDOWN_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_PTP_SHUTDOWN_RSP_DATA		data;
}T_API_PTP_SHUTDOWN_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_SHUTDOWN_REQ				(D_API_MSGID_PTP_BASE + 0x00000003)
#define D_API_MSGID_PTP_SHUTDOWN_RSP				(D_API_MSGID_PTP_BASE + 0x00000004)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_PtpShutdown(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_TRACE_LOG_SET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	UINT							dbgMode;		/**< モジュール番号		*/
	UINT							dbgLevel;		/**< ログレベル			*/
}T_API_PTP_TLOG_SET_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_PTP_TLOG_SET_REQ_DATA		data;
}T_API_PTP_TLOG_SET_REQ;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_TLOG_SET_REQ				(D_API_MSGID_PTP_BASE + 0x00000005)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_SetTraceLogLevel(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	dbgMode,	/**< モジュール番号		*/
								UINT	dbgLevel);	/**< ログレベル			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_APRLOG_SET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	UINT							dbgLevel;		/**< ログレベル			*/
}T_API_PTP_APRLOG_SET_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_PTP_APRLOG_SET_REQ_DATA	data;
}T_API_PTP_APRLOG_SET_REQ;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_APRLOG_SET_REQ				(D_API_MSGID_PTP_BASE + 0x00000007)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_SetAprLogLevel(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	dbgLevel);	/**< ログレベル			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_TRAFFIC_GET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_TRAFFIC_GET_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_TRAFFIC_GET_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_TRAFFIC_GET_REQ				(D_API_MSGID_PTP_BASE + 0x00000009)
#define D_API_MSGID_PTP_TRAFFIC_GET_RSP				(D_API_MSGID_PTP_BASE + 0x0000000a)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_GetTrafficLog(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_DEV_STATE_GET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_DEV_STATE_GET_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is ...
 */
typedef struct{
	T_API_COMMON_HEADER					header;
}T_API_PTP_DEV_STATE_GET_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_DEV_STATE_GET_REQ			(D_API_MSGID_PTP_BASE + 0x0000000b)
#define D_API_MSGID_PTP_DEV_STATE_GET_REP			(D_API_MSGID_PTP_BASE + 0x0000000c)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_GetDevState(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_CLOCK_GET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_CLOCK_GET_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_CLOCK_GET_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_CLOCK_GET_REQ				(D_API_MSGID_PTP_BASE + 0x0000000d)
#define D_API_MSGID_PTP_CLOCK_GET_RSP				(D_API_MSGID_PTP_BASE + 0x0000000e)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_GetPtpClock(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_SYNC_INFO
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_SYNC_INFO_REQ;
/** @} */

/**
 * @brief	Structure of request notification
 * @note	Structure of request notification is ...
 */
typedef struct{
	UINT							type;			/**< 同期ソース			*/
	UINT							state;			/**< 同期状態			*/
}T_API_PTP_SYNC_INFO;

typedef struct{
	UINT							syncState;		/**< Sync状態			*/
	UINT							Sources;		/**< Sync情報数			*/
	T_API_PTP_SYNC_INFO				info[3];		/**< Sync情報			*/
}T_API_PTP_SYNC_INFO_NTF_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_PTP_SYNC_INFO_NTF_DATA	data;
}T_API_MPLANE_SYNC_INFO_NTF;
/** @} */

/**
 * @brief	Message id of M-Plane message reception ready
 * @note	Defined massage id of M-Plane message reception ready is...
 */
#define D_API_MSGID_PTP_SYNC_INFO_REQ			(D_API_MSGID_PTP_BASE + 0x00000013)
#define D_API_MSGID_PTP_SYNC_INFO_NTF			(D_API_MSGID_PTP_BASE + 0x00000014)

/**
 * @brief	M-Plane message reception ready API
 * @note	Function of M-Plane message reception ready is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mpr_SubscribeSyncInfo(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	mode);		/**< mode				*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_PRAM_GET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_PRAM_GET_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_PTP_PRAM_GET_RSP;
/** @} */

/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_PRAM_GET_REQ				(D_API_MSGID_PTP_BASE + 0x00000015)
#define D_API_MSGID_PTP_PRAM_GET_RSP				(D_API_MSGID_PTP_BASE + 0x00000016)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_GetPtpPram(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */

/******************************************************************************************************************************/
/** T_API_PTP_PRAM_GET
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of request message
 * @note	Structure of request message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
	UINT							mode;
	UINT							addr;
	UINT							value;
}T_API_PTP_DEV_CTL_REQ;
/** @} */

/**
 * @brief	Structure of response message
 * @note	Structure of response message is ...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
	UINT							result;
	UINT							mode;
	UINT							addr;
	UINT							value;
}T_API_PTP_DEV_CTL_RSP;
/** @} */


/**
 * @brief	Message id
 * @note	Defined massage id of req/rsp is...
 */
#define D_API_MSGID_PTP_DEV_CTL_REQ				(D_API_MSGID_PTP_BASE + 0x00000017)
#define D_API_MSGID_PTP_DEV_CTL_RSP				(D_API_MSGID_PTP_BASE + 0x00000018)
/** @} */

/**
 * @brief	API Function
 * @note	API Function is...
 */
extern E_RRHAPI_RCODE duApi_Ptp_Mqr_CtlPtpDevice(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	mode,
								UINT	addr,
								UINT	value
								);
/** @} */




#endif
/** @} */
