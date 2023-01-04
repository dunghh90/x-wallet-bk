/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_IETF_INTERFACES.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_IETF_INTERFACES_TYP_H
#define	F_MPSW_MSG_IETF_INTERFACES_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_ietf_interfaces
 *  @{
 */

/**
 * @brief	(0x81070111)ietf-interfaces　GET要求
 * @note	ietf-interfaces　GET要求
 * 
 */
typedef struct{
	UINT				type;								/**< Interface Type  */
	UINT				plane;								/**< Plane種別  */
	UINT				port;								/**< Port番号  */
}T_MPSW_MSI_IETF_INTERFACES_GET_REQ;

/**
 * @brief	(0x81070111)MSI_IETF_INTERFACES_GET_REQ
 * @note	ietf-interfaces　GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_INTERFACES_GET_REQ	data;				/**< Message data  */
}T_MSG_MSI_IETF_INTERFACES_GET_REQ;

/**
 * @brief	(0x81070112)ietf-interfaces　GET要求(OK応答) -> discontinuity-time
 * @note	
 * 
 */
typedef struct{
	USHORT				year;								/**< 年  */
	UCHAR				month;								/**< 月  */
	UCHAR				day;								/**< 日  */
	UCHAR				hour;								/**< 時  */
	UCHAR				min;								/**< 分  */
	UCHAR				sec;								/**< 秒  */
	UCHAR				dummy;								/**< dummy  */
}T_MPSW_DISCONTINUITY_TIME;

/**
 * @brief	(0x81070112)ietf-interfaces　GET要求(OK応答)
 * @note	ietf-interfaces　GET要求(OK応答)
 * 
 */
typedef struct{
	UINT				enabled;							/**< enabled  */
	UINT				oper_status;						/**< oper-status  */
	char				phys_address[6];					/**< phys-address  */
	T_MPSW_DISCONTINUITY_TIME	discontinuity_time;			/**< discontinuity-time  */
	ULLINT				in_octets;							/**< in-octets  */
	ULLINT				in_unicast_pkts;					/**< in-unicast-pkts  */
	ULLINT				in_broadcast_pkts;					/**< in-broadcast-pkts  */
	ULLINT				in_multicast_pkts;					/**< in-multicast-pkts  */
	UINT				in_discards;						/**< in-discards  */
	UINT				in_errors;							/**< in-errors  */
	UINT				in_unknown_protos;					/**< in-unknown-protos  */
	ULLINT				out_octets;							/**< out-octets  */
	ULLINT				out_unicast_pkts;					/**< out-unicast-pkts  */
	ULLINT				out_broadcast_pkts;					/**< out-broadcast-pkts  */
	ULLINT				out_multicast_pkts;					/**< out-multicast-pkts  */
	UINT				out_discards;						/**< out-discards  */
	UINT				out_errors;							/**< out-errors  */
}T_MPSW_MSI_IETF_INTERFACES_GET_CFM;

/**
 * @brief	(0x81070112)MSI_IETF_INTERFACES_GET_CFM
 * @note	ietf-interfaces　GET要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_INTERFACES_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_IETF_INTERFACES_GET_CFM;

/**
 * @brief	(0x81070113)ietf-interfaces　GET要求(NG応答)
 * @note	ietf-interfaces　GET要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_IETF_INTERFACES_GET_REJ;

/**
 * @brief	(0x81070113)MSI_IETF_INTERFACES_GET_REJ
 * @note	ietf-interfaces　GET要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_IETF_INTERFACES_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_IETF_INTERFACES_GET_REJ;


/** @} */

#endif	/* F_MPSW_MSG_IETF_INTERFACES_TYP_H */
