/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API common part)
 *  @note	Struct Definition(RRH API common part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_COM_H
#define RRHAPI_COM_H
/*!
 * @addtogroup RRH_API_COM
 * @{
 */
typedef enum 
{
	E_API_RCD_OK = 0,
	E_API_RCD_NG,
	E_API_RCD_EPARAM,
	E_API_RCD_ESIZEOVER,
	E_API_RCD_ENOQNUM,
	E_API_RCD_ENOSPACE,
	E_API_RCD_EADDR,
	E_API_RCD_EINTR,
	E_API_RCD_ENOMSG,
	E_API_RCD_ERR_BPF_CALL,
	E_API_RCD_CHECKSUMNG,
/* 2020/12/29 M&C) Merge 4G FHM src (add) */
	E_API_RCD_OVER_MAX_DELAY,
/* 2020/12/29 M&C) Merge 4G FHM src (end) */
	E_API_RCD_OTHER
}E_RRHAPI_RCODE;

/** @} */

/*!
 * @addtogroup RRH_API_COM
 * @{
 */

/******************************************************************************************************************************/
/** T_API_COMMON_HEADER
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Process Common Header
 * @note	Process Common Header is ..
 * 
 */
typedef struct{
	UINT		uiEventNo;					/**< イベント番号						*/
	UINT		uiSignalkind;				/**< SignalKind(EventIDが重複しない場合はDon't care)*/
	UINT		uiDstPQueueID;				/**< 最終送信先Process Queue ID			*/
	UINT		uiDstTQueueID;				/**< 最終送信先Thread  Queue ID			*/
	UINT		uiSrcPQueueID;				/**< 送信元    Process Queue ID			*/
	UINT		uiSrcTQueueID;				/**< 送信元    Thread  Queue ID			*/
	UINT		uiLength;					/**< Length(Head部 + Data部)			*/
	UINT		uiKeyID;					/**< 通信用共有メモリ管理用KEYID		*/
}T_API_COMMON_HEADER;
/** @} */

/******************************************************************************************************************************/
/** T_APIIF_COM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The common request defintion
 * @note	The common request defintion is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_APIIF_COM_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_APIIF_COM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The common response defintion
 * @note	The common response defintion is ..
 */
typedef struct{
	UINT	result;
	CHAR	errMsg[256];
}T_API_COM_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA	data;
}T_API_COM_RSP;
/** @} */

INT rrhApi_com_msgQSend( UINT		q_num,
						 const VOID			*msgp,
						 UINT				msgsz,
						 INT				*errcd);

INT rrhApi_com_msgQReceive( UINT		q_num,
							INT				timeout,
							UINT			msgsz,
							VOID			*msgp,
							UINT			*rcvmsgsz,
							INT				*errcd);

void rrhApi_com_log_real(UINT	logLevel , const char* log , INT err ,const VOID * fileName, UINT lineNo );
#define rrhApi_com_log( log ,err) rrhApi_com_log_real(D_RRH_LOG_AST_LV_RETURN ,log , err , __FILE__ , __LINE__ )

INT	rrhApi_com_checkRespQ(UINT);

/** @} */
#endif
/** @} */
