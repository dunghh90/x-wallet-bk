/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API TILT part)
 *  @note	Struct Definition(RRH API TILT part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/
#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_TILT_H
#define RRHAPI_TILT_H

/*!
 * @addtogroup RRH_API_TILT
 * @{
 */
#define D_API_MSGID_TILT_BASE						0xA0060000
#define D_API_MSGID_TILT_STATE_SUB_REQ				0xA0060001
#define D_API_MSGID_TILT_STATE_NTC					0xA0060002
#define D_API_MSGID_TILT_STATE_UNSUB_IND			0xA0060003
#define D_API_MSGID_TILT_DATA_RECV_REQ				0xA0060005
#define D_API_MSGID_TILT_DATA_RECV_RSP				0xA0060006
#define D_API_MSGID_TILT_DATA_SEND_REQ				0xA0060007
#define D_API_MSGID_TILT_DATA_SEND_RSP				0xA0060008
#define D_API_MSGID_TILT_DATA_DELETE_IND			0xA0060009
#define D_API_MSGID_TILT_SWITCH						0xA006000b
#define D_API_MSGID_TILT_SWITCH_RSP					0xA006000C
#define D_API_MSGID_AISG_DEVSCAN_NTC				0xA006000D
#define D_API_MSGID_AISG_FUNCSTOP_NTC				0xA006000F
/** @} */

/*!
 * @addtogroup RRH_API_TILT
 * @{
 */
#define D_TILT_NORMAL		0
#define D_TILT_RENG			1
#define D_TILT_LENNG	 	3
/**TDD-RRE use*/
#define D_LINK_DISCONNECT	1
#define D_DEV_NULL			2
#define D_TILT_BUFFUL 		3

/** @} */


/*!
 * @addtogroup RRH_API_TILT
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_TILT_DATA_SUB_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Make subscription of receiving Tilt data notice
 * @note	Make subscription of receiving Tilt data notice is ..
 */
typedef struct{
	UINT system;
	UINT devno;
}T_API_TILT_STAT_SUB_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_TILT_STAT_SUB_DATA	data;
}T_API_TILT_STAT_SUB_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_STAT_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief   Tilt data receiving status response
 * @note	Tilt data receiving status response is ..
 */
typedef struct{
	UINT system;
	UINT devno;
}T_API_TILT_DATA_STAT_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_TILT_DATA_STAT_DATA data;
}T_API_TILT_DATA_STAT_NTC;
/** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_UNSUB_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Make un-subscription of receiving Tilt data notice
 * @note	Make un-subscription of receiving Tilt data notice is ..
 */
typedef struct{
	UINT system;
	UINT devno;
}T_API_TILT_STAT_UNSUB_DATA; 
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_TILT_STAT_UNSUB_DATA data;
}T_API_TILT_STAT_UNSUB_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_RECV_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of receiving Tilt data
 * @note	The request of receiving Tilt data is ..
 */
typedef struct{
	UINT				system;
	UINT				devno;
}T_API_TILT_DATA_RECV_REQ_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_TILT_DATA_RECV_REQ_DATA data;
}T_API_TILT_DATA_RECV_REQ;
/** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_RECV_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The response of receiving Tilt data
 * @note	The response of receiving Tilt data is ..
 */
typedef struct{
	UINT				system;
	UINT				devno;
	UINT           		length;
	UCHAR           	data[2048];
}T_API_TILT_DATA_RECV_RSP_DATA;
typedef struct{
	T_API_COMMON_HEADER  header;
	T_API_TILT_DATA_RECV_RSP_DATA data;
}T_API_TILT_DATA_RECV_RSP;
/** @} */
	
 /******************************************************************************************************************************/
/** T_API_TILT_DATA_SEND_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of sending Tilt data
 * @note    The request of sending Tilt data is ..
 */
typedef struct{
	UINT				 system;
	UINT				 devno;
	UINT             	 length;
	UCHAR            	 data[2048];
}T_API_TILT_DATA_SEND_DATA;
typedef struct{
	T_API_COMMON_HEADER  header;
	T_API_TILT_DATA_SEND_DATA data;
}T_API_TILT_DATA_SEND_REQ;
/** @} */

  /******************************************************************************************************************************/
 /** T_API_TILT_DATA_SEND_RSP
  *  @addtogroup
  *  @{
  */
 /******************************************************************************************************************************/
 /**
  * @brief	 The request of sending Tilt data
  * @note	 The request of sending Tilt data is ..
  */
 typedef struct{
	 UINT				system;
	 UINT				devno;
	 UINT				check_flg;
 }T_API_TILT_DATA_SEND_RSP_DATA;
 typedef struct{
	 T_API_COMMON_HEADER  header;
	T_API_TILT_DATA_SEND_RSP_DATA data;
}T_API_TILT_DATA_SEND_RSP;
 /** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_DEL_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notice of deleting Tilt data
 * @note	The notice of deletiAng Tilt data is ..
 */
typedef struct{
	UINT				system;
	UINT				devno;
}T_API_TILT_DATA_DEL_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_TILT_DATA_DEL_DATA data;
}T_API_TILT_DATA_DEL_NTC;
/** @} */

 /******************************************************************************************************************************/
/** T_API_TILT_DATA_SEND_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of sending Tilt data
 * @note    The request of sending Tilt data is ..
 */
typedef struct{
	UINT				result;
	UINT				value;
}T_API_TILT_SWITCH_DATA;
typedef struct{
	T_API_COMMON_HEADER  header;
	T_API_TILT_SWITCH_DATA data;
}T_API_TILT_SWITCH_RSP;
/** @} */

/******************************************************************************************************************************/
/** T_API_TILT_ASIG_START_NOTICE
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	notice to AL7 device scan can start
 */
typedef struct{
	UINT	value;
}T_API_AISG_DEVSCAN_DATA;
typedef struct{
	T_API_COMMON_HEADER		header;
	T_API_AISG_DEVSCAN_DATA data;
}T_API_AISG_DEVSCAN_NOTICE;
/** @} */

/******************************************************************************************************************************/
/** T_API_AISG_STOP_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	notice to AL7 AISG stop notice
 */
typedef struct{
	T_API_COMMON_HEADER		header;
}T_API_AISG_STOP_NTC;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Tilt_Mpr_SubscribeState(	
								INT		qid, 
								INT		wtime,VOID *data_p, UINT system );
extern E_RRHAPI_RCODE rrhApi_Tilt_Mnt_UnsubscribeState(
								INT		qid, 
								INT		wtime,VOID *data_p, UINT system );
extern E_RRHAPI_RCODE rrhApi_Tilt_Mqr_ReceiveData(	
								INT		qid, 
								INT		wtime,VOID *data_p,
								UINT	system,
								UINT	devno);

extern E_RRHAPI_RCODE rrhApi_Tilt_Mqr_SendData(	INT	qid, INT wtime,VOID *data_p,
								UINT system,UINT devno,UCHAR *data,UINT datasize);

extern E_RRHAPI_RCODE rrhApi_Tilt_Mnt_DeleteData( INT		qid, INT		wtime,VOID *data_p,
								UINT system,UINT		devno );

extern E_RRHAPI_RCODE rrhApi_Tilt_Mqr_Switch( INT	qid, INT		wtime,VOID *data_p);

/*3.5G RRE*/
extern E_RRHAPI_RCODE rrhApi_Tilt_Mpr_DevScan ( INT	qid, INT	wtime,VOID *data_p );

extern E_RRHAPI_RCODE rrhApi_Tilt_Mnt_AisgStopNtc ( INT	qid, INT	wtime,VOID *data_p );

/** @} */
#endif
/** @} */
