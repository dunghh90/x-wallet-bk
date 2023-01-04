/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also プロセス間IF仕様書(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/******************************************************************************************************************************/

#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_MCPRI_H
#define RRHAPI_MCPRI_H

/*!
 * @addtogroup RRH_API_MCPRI
 * @{
 */
#define D_API_MSGID_MCPRI_BASE						0xA00C0000
#define D_API_MSGID_MCPRI_LINK_STARTUP_IND			0xA00C0001
#define D_API_MSGID_MCPRI_LINK_ENABLE_IND			0xA00C0005
#define D_API_MSGID_MCPRI_LINK_DISABLE_IND			0xA00C0007
#define D_API_MSGID_MCPRI_STATE_SUB_REQ				0xA00C0009
#define D_API_MSGID_MCPRI_STATE_STATE_NTC			0xA00C000A
#define D_API_MSGID_MCPRI_STATE_UNSUB_IND			0xA00C000B
#define D_API_MSGID_MCPRI_PARAM_UPDATE_IND			0xA00C000C
#define D_API_MSGID_MCPRI_FIBEROUTPU_IND			0xA00C0010
#define D_API_MSGID_MCPRI_PORT_STOP_REQ				0xA00C0011
#define D_API_MSGID_MCPRI_CPSLINKUPSVTIMEOUT_NTC	0xA00C0013
#define D_API_MSGID_MCPRI_RESET_REQ					0xA00C0015
/** @} */

/*!
 * @addtogroup RRH_API_MCPRI
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_MCPRILINK_STARTUP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of startup cpri link
 * @note	The request of startup cpri link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT				cpriLineBitrate;
}T_API_MCPRILINK_STARTUP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_STOP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of stop cpri link
 * @note	The request of stop cpri link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_STOP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_ENABLE_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup cpri link
 * @note	The request of setup cpri link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_ENABLE_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_DISABLE_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup cpri link
 * @note	The request of setup cpri link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_DISABLE_IND;
/** @} */
 /******************************************************************************************************************************/
/** T_API_MCPRIMSG_SUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of the subscription of receving CPRI state
 * @note	The request of the subscription of receving CPRI state is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_STATE_SUB_REQ;
/**
 * @brief	The request of the subscription of receving CPRI state
 * @note	The request of the subscription of receving CPRI state is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	cpriHDLCBitrate;
	UINT	cpriLineBitrate;
	UINT	cpriHdlc;
	UINT	cpriEDown;
	UINT	hfnSync;
	UINT	cpriState;
	UINT	cscdKind;
	UINT	fileverDisagreement;
	UINT	vssUnMatch;
}T_API_MCPRILINK_STATE_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_API_MCPRILINK_STATE_UNSUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of the unsubscription of receving CPRI state
 * @note	The request of the unsubscription of receving CPRI state is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_STATE_UNSUB_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_PARAM_UPDATE_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notice of updating related cpri system parameter
 * @note	The notice of receving related cpri system parameter is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	stableValue;
	/*T.B.D*/
}T_API_MCPRILINK_PARAM_UPDATE_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_MCPRILINK_PARAM_UPDATE_DATA data;
	/*T.B.D*/
}T_API_MCPRILINK_PARAM_UPDATE_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_PARAM_UPDATE_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notice of cpri fiber ouptut
 * @note	The notice of rcpri fiber ouptut is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	onOff; 
}T_API_MCPRILINK_FIBEROUTPUT_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_PORTSTOP_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of Port stop   
 * @note	The request of Port stop is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	cscd_kind;
}T_API_MCPRILINK_PORTSTOP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_MCPRILINK_RESET_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of Reset stop   
 * @note	The request of Reset stop is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_MCPRILINK_RESET_IND;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_StartupLink(
								INT		qid, INT		wtime,VOID *data_p, UINT cpriLineBitrate);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_EnableLink(	
								INT		qid, 	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_DisableLink(	
								INT		qid,	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mpr_SubscribeLinkState(
								INT		qid,	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_UnsubscribeLinkState(
								INT		qid, 	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_UpdateSystemPara(
								INT		qid, 	INT		wtime,VOID *data_p,
								UINT	value);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_FiberOutput(
								INT		qid, 	INT		wtime,VOID *data_p,UINT	onOff);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_PortStopReq(
								INT		qid,	INT		wtime,VOID *data_p,UINT cscd_kind);
extern E_RRHAPI_RCODE rrhApi_Mcpri_Mnt_ResetReq(
								INT qid, INT wtime, VOID *data_p );

/** @} */
#endif
/** @} */
