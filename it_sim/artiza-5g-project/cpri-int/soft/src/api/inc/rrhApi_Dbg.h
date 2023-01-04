/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also プロセス間IF仕様書(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/
#include "rrhApi_Com.h"

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_DBG_H
#define RRHAPI_DBG_H

/*!
 * @addtogroup RRH_API_DBG
 * @{
 */
#define D_API_MSGID_DBG_BASE						0xA00A0000
#define D_API_MSGID_DBG_CPRISIMU_START_REQ			0xA00A0001
#define D_API_MSGID_DBG_CPRISIMU_START_RSP			0xA00A0002
#define D_API_MSGID_DBG_CPRISIMU_STOP_REQ			0xA00A0003
#define D_API_MSGID_DBG_CPRISIMU_STOP_RSP			0xA00A0004
#define D_API_MSGID_DBG_CPRIMON_START_IND			0xA00A0005
#define D_API_MSGID_DBG_CPRIMON_DATA_RCV_NTC		0xA00A0006
#define D_API_MSGID_DBG_CPRIMON_DATA_SND_NTC		0xA00A0008
#define D_API_MSGID_DBG_CPRIMON_STOP_IND			0xA00A0009
#define D_API_MSGID_DBG_IFCONFIG_REQ				0xA00A000B
#define D_API_MSGID_DBG_IFCONFIG_RSP				0xA00A000C
#define D_API_MSGID_DBG_SEND_SIMUCPRIMSG			0xA00A000D
#define D_API_MSGID_DBG_RECV_SIMUCPRIMSG			0xA00A000E
#define D_API_MSGID_DBG_SIMUAMPCHG_NTC				0xA00A000F

#define D_API_MSGID_PL_DBG_CPRISIMU_START_REQ		0xA00D0101
#define D_API_MSGID_PL_DBG_CPRISIMU_START_RSP		0xA00D0102
#define D_API_MSGID_PL_DBG_CPRISIMU_STOP_REQ		0xA00D0103
#define D_API_MSGID_PL_DBG_CPRISIMU_STOP_RSP		0xA00D0104
#define D_API_MSGID_PL_DBG_CPRIMON_START_IND		0xA00D0105
#define D_API_MSGID_PL_DBG_CPRIMON_STOP_IND			0xA00D0109
#define D_API_MSGID_PL_DBG_SEND_SIMUCPRIMSG			0xA00D010D

#define D_API_DBG_SIMU_MODE_OFF							(0)
#define D_API_DBG_SIMU_MODE_ON_L3						(1)
/** @} */

/*!
 * @addtogroup RRH_API_DBG
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_DBG_CPRISIMU_STARTUP_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of startup cpri simu defintion
 * @note	The request of startup cpri simu defintion is ..
 */
typedef struct{
	UINT                linkno;
	UINT                mode;
}T_API_DBG_CPRISIMU_STARTUP_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_CPRISIMU_STARTUP_DATA data;
}T_API_DBG_CPRISIMU_STARTUP_REQ;
/** @} */

/**
 * @brief	The response of startup cpri simu defintion
 * @note	The response of startup cpri simu defintion is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA	data;
}T_API_DBG_CPRISIMU_STARTUP_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_DBG_CPRISIMU_STOP_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of stop cpri simu defintion
 * @note	The request of stop cpri simu defintion is ..
 */
typedef struct{
	UINT                linkno;
}T_API_DBG_CPRISIMU_STOP_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_CPRISIMU_STOP_DATA	data;
}T_API_DBG_CPRISIMU_STOP_REQ;
/** @} */

/**
 * @brief	The response of stop cpri simu defintion
 * @note	The response of stop cpri simu defintion is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA	data;
}T_API_DBG_CPRISIMU_STOP_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_DBG_CPRIMON_START_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of startup cpri monitor mode defintion
 * @note	The indication of startup cpri monitor mode defintion is ..
 */
typedef struct{
	UINT                linkno;
}T_API_DBG_CPRIMON_START_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_CPRIMON_START_DATA data;
}T_API_DBG_CPRIMON_START_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_DBG_CPRIMON_STOP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of stop cpri monitor mode defintion
 * @note	The indication of stop cpri monitor mode defintion is ..
 */
typedef struct{
	UINT                linkno;
}T_API_DBG_CPRIMON_STOP_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_CPRIMON_STOP_DATA                data;
}T_API_DBG_CPRIMON_STOP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_DBG_IFCONFIG_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of If configuration defintion
 * @note	The request of If configuration defintion is ..
 */
typedef struct{
	UINT	ipAddr;
	UINT	ipMask;
}T_API_DBG_IFCONFIG_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_IFCONFIG_DATA data;
}T_API_DBG_IFCONFIG_REQ;
/** @} */
/**
 * @brief	The response of If configuration defintion
 * @note	The response of If configuration defintion is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA	data;
}T_API_DBG_IFCONFIG_RSP;
/** @} */

/**
 * @brief	Send cpri message to L2 RXS thread
 * @note	Send cpri message to L2 RXS thread is ..
 */
typedef struct{
	VOID* simuMsg;
}T_API_DBG_SEND_SIMUCPRIMSG_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_DBG_SEND_SIMUCPRIMSG_DATA data;
}T_API_DBG_SEND_SIMUCPRIMSG_IND;
/** @} */

/**
 * @brief	Send simu-AMP change to PF RSV thread
 * @note	Send simu-AMP change to PF RSV thread is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	onOff;
}T_API_DBG_SIMUAMPCHG_NTC;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StartCpriSimu(	
								INT		qid, INT		wtime,VOID *data_p,
								UINT    linkno,
								UINT	mode);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StopCpriSimu(
								INT		qid, INT		wtime,VOID *data_p,
								UINT    linkno);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StartupCpriMonitor(	
								INT		qid, INT		wtime,VOID *data_p,
								UINT	linkno);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StopCpriMonitor(	
								INT		qid, INT		wtime,VOID *data_p,
								UINT    linkno);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mqr_IfConfig(	
								INT		qid, INT		wtime,VOID *data_p,
								UINT	ipAddr,
								UINT	ipMask);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mnt_SendSimuCpriMsg(	
								INT		qid, INT		wtime,VOID *data_p,
								VOID*	msg_p);
extern E_RRHAPI_RCODE rrhApi_Dbg_Mnt_SimuAMPOnOff(
								INT	qid,	INT	wtime,VOID *data_p,
								UINT			OnOff);


extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mnt_SendSimuCpriMsg(
								INT		qid, INT		wtime,VOID *data_p,
								VOID*	inf_p);

extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mnt_StartupCpriMonitor(	
								INT	qid,	INT	wtime,VOID *data_p,
								UINT linkno);

extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mnt_StopCpriMonitor(	
								INT		qid, INT		wtime,VOID *data_p,
								UINT linkno);
								
extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mqr_StartCpriSimu( 
								INT		qid, INT		wtime,VOID *data_p,
								UINT	linkno,
								UINT	mode);
								
extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mqr_StopCpriSimu(
								INT		qid, INT		wtime,VOID *data_p,
								UINT linkno);

extern E_RRHAPI_RCODE rrhApi_PL_Dbg_Mnt_SendSimuCpriMsg(
								INT		qid, INT		wtime,VOID *data_p,
								VOID*	inf_p);

/** @} */
#endif
/** @} */
