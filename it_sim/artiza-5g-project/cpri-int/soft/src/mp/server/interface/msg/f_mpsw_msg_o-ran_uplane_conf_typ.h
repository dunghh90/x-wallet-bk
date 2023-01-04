/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_UPLANE_CONF.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_UPLANE_CONF_TYP_H
#define	F_MPSW_MSG_O_RAN_UPLANE_CONF_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_uplane_conf
 *  @{
 */

/**
 * @brief	(0x81240111)o-ran-uplane-conf　GET要求
 * @note	
 * 
 */
typedef struct{
	UINT				ant;								/**< ant  */
	UINT				car;								/**< car  */
	UINT				tx_rx;								/**< tx_rx  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REQ;

/**
 * @brief	(0x81240111)MSI_O_RAN_UPLANE_CONF_GET_REQ
 * @note	o-ran-uplane-conf　GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REQ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ;

/**
 * @brief	(0x81240112)o-ran-uplane-conf　GET要求(OK応答)
 * @note	
 * 
 */
typedef struct{
	UINT				state;								/**< state  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM;

/**
 * @brief	(0x81240112)MSI_O_RAN_UPLANE_CONF_GET_CFM
 * @note	o-ran-uplane-conf　GET要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM;

/**
 * @brief	(0x81240113)o-ran-uplane-conf　GET要求(NG応答)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REJ;

/**
 * @brief	(0x81240113)MSI_O_RAN_UPLANE_CONF_GET_REJ
 * @note	o-ran-uplane-conf　GET要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REJ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REJ;

/**
 * @brief	(0x81240211)o-ran-uplane-conf　TX EDIT要求 -> tx_array_car_t -> scs_spec_conf_t
 * @brief	(0x81240221)o-ran-uplane-conf　RX EDIT要求 -> rx_array_car_t -> scs_spec_conf_t
 * @note	
 * 
 */
typedef struct{
	E_UP_SCS			scs;								/**< scs  */
	INT					offset_to_abs_freq_center;			/**< offset_to_abs_freq_center  */
	UINT				num_of_prbs;						/**< num_of_prbs  */
}T_MPSW_SCS_SPEC_CONF_T;

/**
 * @brief	(0x81240211)o-ran-uplane-conf　TX EDIT要求 -> tx_array_car_t
 * @note	
 * 
 */
typedef struct{
	UINT				ant;								/**< ant  */
	UINT				car;								/**< car  */
	UINT				abs_freq_center;					/**< abs_freq_center  */
	UINT64				cent_of_ch_bandwidth;				/**< cent_of_ch_bandwidth  */
	UINT64				ch_bandwidth;						/**< ch_bandwidth  */
	INT64				gain;								/**< gain  */
	E_UP_CAR_ACTIVE		active;								/**< active  */
	UINT				fft_size;							/**< fft_size  */
	UINT				dl_radio_frame_offset;				/**< dl_radio_frame_offset  */
	INT16				dl_sfn_offset;						/**< dl_sfn_offset  */
	E_UP_DEF_SCS		default_scs;						/**< default_scs  */
	T_MPSW_SCS_SPEC_CONF_T	scs_spec_conf_t[7];				/**< scs_spec_conf_t  */
	UINT				cp_length;							/**< cp_length  */
}T_MPSW_TX_ARRAY_CAR_T;

/**
 * @brief	(0x81240211)o-ran-uplane-conf　TX EDIT要求
 * @note	
 * 
 */
typedef struct{
	UINT				fh_port;							/**< front_haul_port  */
	UINT16				eaxc_id[2];							/**< eaxc_id  */
	T_MPSW_TX_ARRAY_CAR_T	tx_array_car_t;					/**< tx_array_car_t  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ;

/**
 * @brief	(0x81240211)MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ
 * @note	o-ran-uplane-conf　TX EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ;

/**
 * @brief	(0x81240212)MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM
 * @note	o-ran-uplane-conf　TX EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM;

/**
 * @brief	(0x81240213)o-ran-uplane-conf　TX EDIT要求(NG応答)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ;

/**
 * @brief	(0x81240213)MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ
 * @note	o-ran-uplane-conf　TX EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ;

/**
 * @brief	(0x81240221)o-ran-uplane-conf　RX EDIT要求 -> rx_array_car_t
 * @note	
 * 
 */
typedef struct{
	UINT				ant;								/**< ant  */
	UINT				car;								/**< car  */
	UINT				abs_freq_center;					/**< abs_freq_center  */
	UINT64				cent_of_ch_bandwidth;				/**< cent_of_ch_bandwidth  */
	UINT64				ch_bandwidth;						/**< ch_bandwidth  */
	E_UP_CAR_ACTIVE		active;								/**< active  */
	UINT				fft_size;							/**< fft_size  */
	UINT				ul_fft_sampling_offset;				/**< ul_fft_sampling_offset  */
	UINT				n_ta_offset;						/**< n_ta_offset  */
	E_UP_DEF_SCS		default_scs;						/**< default_scs  */
	T_MPSW_SCS_SPEC_CONF_T	scs_spec_conf_t[7];				/**< scs_spec_conf_t  */
	UINT				cp_length;							/**< cp_length  */
}T_MPSW_RX_ARRAY_CAR_T;

/**
 * @brief	(0x81240221)o-ran-uplane-conf　RX EDIT要求
 * @note	
 * 
 */
typedef struct{
	UINT				fh_port;							/**< front_haul_port  */
	UINT16				eaxc_id[2];							/**< eaxc_id  */
	T_MPSW_RX_ARRAY_CAR_T	rx_array_car_t;					/**< rx_array_car_t  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ;

/**
 * @brief	(0x81240221)MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ
 * @note	o-ran-uplane-conf　RX EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ;

/**
 * @brief	(0x81240222)MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM
 * @note	o-ran-uplane-conf　RX EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM;

/**
 * @brief	(0x81240223)o-ran-uplane-conf　RX EDIT要求(NG応答)
 * @note	
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ;

/**
 * @brief	(0x81240223)MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ
 * @note	o-ran-uplane-conf　RX EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ	data;		/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ;

/**
 * @brief	(0x81240715)o-ran-uplane-conf　TX アレイキャリア状態変化通知 -> 通知情報
 * @brief	(0x81240725)o-ran-uplane-conf　RX アレイキャリア状態変化通知 -> 通知情報
 * @note	上の要素数に応じて可変長
 * 
 */
typedef struct{
	UINT				ant;								/**< ant  */
	UINT				car;								/**< car  */
	UINT				state;								/**< state  */
}T_MPSW_ARRAY_CARRIER_STATE_T;

/**
 * @brief	(0x81240715)o-ran-uplane-conf　TX アレイキャリア状態変化通知

 * @note	RX用とはメッセージIDのみ異なり内容は同じ。
 * 
 */
typedef struct{
	UINT				number;								/**< 要素数  */
	T_MPSW_ARRAY_CARRIER_STATE_T	array_carrier_state_t[1];
															/**< 通知情報  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81240715)MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND
 * @note	o-ran-uplane-conf　TX アレイキャリア状態変化通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81240725)o-ran-uplane-conf　RX アレイキャリア状態変化通知
 * @note	TX用とはメッセージIDのみ異なり内容は同じ。
 * 
 */
typedef struct{
	UINT				number;								/**< 要素数  */
	T_MPSW_ARRAY_CARRIER_STATE_T	array_carrier_state_t[1];
															/**< 通知情報  */
}T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND;

/**
 * @brief	(0x81240725)MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND
 * @note	o-ran-uplane-conf　RX アレイキャリア状態変化通知(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND	data;
															/**< Message data  */
}T_MSG_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_UPLANE_CONF_TYP_H */
