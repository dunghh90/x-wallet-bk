/*!
 * @skip  $ld:$
 * @file  f_rfp_typ.h
 * @brief DPDAタスク 構造体宣言
 * @date  2018/11/24 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_RFP_TYP_H
#define F_RFP_TYP_H

/******************************************************************************************************************************/
/** T_RFP_MPL_TX_ON_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_TX_ON_SETTING_REQ
 * @note	T_RFP_MPL_TX_ON_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ			body;					/**< Message body 								*/
}T_RFP_MPL_TX_ON_SETTING_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_TX_ON_SETTING_CFM
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_TX_ON_SETTING_CFM
 * @note	T_RFP_MPL_TX_ON_SETTING_CFM(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	UINT											ant;					/**< Antena Number								*/
	UINT											car;					/**< Carrer Number								*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ			msgErr;					/**< Message body								*/
}T_RFP_MPL_TX_ON_SETTING_CFM;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_RX_ON_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_RX_ON_SETTING_REQ
 * @note	T_RFP_MPL_RX_ON_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ			body;					/**< Message body								*/
}T_RFP_MPL_RX_ON_SETTING_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_RX_ON_SETTING_CFM
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_RX_ON_SETTING_CFM
 * @note	T_RFP_MPL_RX_ON_SETTING_CFM(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	UINT											ant;					/**< Antena Number								*/
	UINT											car;					/**< Carrer Number								*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ			msgErr;					/**< Message body								*/
}T_RFP_MPL_RX_ON_SETTING_CFM;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_TX_OFF_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_TX_OFF_SETTING_REQ
 * @note	T_RFP_MPL_TX_OFF_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ			body;					/**< Message body 								*/
}T_RFP_MPL_TX_OFF_SETTING_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_TX_OFF_SETTING_CFM
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_TX_OFF_SETTING_CFM
 * @note	T_RFP_MPL_TX_OFF_SETTING_CFM(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	UINT											ant;					/**< Antena Number								*/
	UINT											car;					/**< Carrer Number								*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ			msgErr;					/**< Message body								*/
}T_RFP_MPL_TX_OFF_SETTING_CFM;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_RX_OFF_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_RX_OFF_SETTING_REQ
 * @note	T_RFP_MPL_RX_OFF_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ			body;					/**< Message body								*/
}T_RFP_MPL_RX_OFF_SETTING_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_RX_OFF_SETTING_CFM
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_RX_OFF_SETTING_CFM
 * @note	T_RFP_MPL_RX_OFF_SETTING_CFM(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	UINT											ant;					/**< Antena Number								*/
	UINT											car;					/**< Carrer Number								*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ			msgErr;					/**< Message body								*/
}T_RFP_MPL_RX_OFF_SETTING_CFM;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_TX_SLEEP_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_TX_SLEEP_SETTING_REQ
 * @note	T_RFP_MPL_TX_SLEEP_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ			body;					/**< Message body 								*/
}T_RFP_MPL_TX_SLEEP_SETTING_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_RFP_MPL_RX_SLEEP_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_RFP_MPL_RX_SLEEP_SETTING_REQ
 * @note	T_RFP_MPL_RX_SLEEP_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UINT											requestId;				/**< Request ID									*/
	T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ			body;					/**< Message body 								*/
}T_RFP_MPL_RX_SLEEP_SETTING_REQ;
/** @} */

#endif
/* @} */
