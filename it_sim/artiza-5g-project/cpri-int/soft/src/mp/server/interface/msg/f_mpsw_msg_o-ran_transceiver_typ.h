/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_eval_Server_MSI_O_RAN_TRANSCEIVER.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_TRANSCEIVER_TYP_H
#define	F_MPSW_MSG_O_RAN_TRANSCEIVER_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_transceiver
 *  @{
 */

/**
 * @brief	(0x81210111)MSI_O_RAN_TRANSCEIVER_GET_REQ
 * @note	o-ran-transceiver GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ;

/**
 * @brief	(0x81210112)o-ran-transceiver GET要求(OK応答) -> port-transceiver-data
 * @note	・0：Port#0 \n
 * 			・1：Port#1
 * 
 */
typedef struct{
	UINT				present;							/**< present  */
	UCHAR				vendor_id[8];						/**< vendor-id  */
	UCHAR				vendor_part[20];					/**< vendor-part  */
	UCHAR				vendor_rev[8];						/**< vendor-rev  */
	UCHAR				serial_no[20];						/**< serial-no  */
	UCHAR				SFF8472_compliance_code;			/**< SFF8472-compliance-code  */
	UCHAR				connector_type;						/**< connector-type  */
	UCHAR				nominal_bitrate;					/**< nominal-bitrate  */
	UCHAR				low_bitrate_margin;					/**< low-bitrate-margin  */
	UCHAR				high_bitrate_margin;				/**< high-bitrate-margin  */
	UCHAR				rx_power_type;						/**< rx-power-type  */
	DL					rx_power;							/**< rx-power  */
	DL					tx_power;							/**< tx-power  */
	DL					tx_bias_current;					/**< tx-bias-current  */
	DL					voltage;							/**< voltage  */
	DL					temperature;						/**< temperature  */
}T_MPSW_TRANSCEIVER_DATA;

/**
 * @brief	(0x81210112)o-ran-transceiver GET要求(OK応答)
 * @note	o-ran-transceiver GET要求(OK応答)
 * 
 */
typedef struct{
	T_MPSW_TRANSCEIVER_DATA	transceiver_data[2];			/**< port-transceiver-data  */
}T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM;

/**
 * @brief	(0x81210112)MSI_O_RAN_TRANSCEIVER_GET_CFM
 * @note	o-ran-transceiver GET要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM;

/**
 * @brief	(0x81210113)o-ran-transceiver GET要求(NG応答)
 * @note	o-ran-transceiver GET要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_REJ;

/**
 * @brief	(0x81210113)MSI_O_RAN_TRANSCEIVER_GET_REJ
 * @note	o-ran-transceiver GET要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_TRANSCEIVER_TYP_H */
