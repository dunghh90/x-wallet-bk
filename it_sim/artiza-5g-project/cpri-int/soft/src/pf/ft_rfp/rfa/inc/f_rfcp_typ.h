/*!
* @skip $Id:$
* @file f_rfcp_typ.h
* @brief RFCP thread type header
* @date 2013/03/21 ALPHA)松延 Create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

/** @addtogroup RRH_PF_RFP

 *  @{
 */
#ifndef F_RFCP_TYP_H
#define F_RFCP_TYP_H

/******************************************************************************************************************************/
/*!
 * @brief time stamp
 * @note  time stamp
 * @{
 */
/******************************************************************************************************************************/
typedef struct{
	USHORT					year;													/* year								*/
	UCHAR					month;													/* month							*/
	UCHAR					day;													/* day								*/
	UCHAR					hour;													/* hour								*/
	UCHAR					min;													/* minute							*/
	UCHAR					sec;													/* second							*/
	UCHAR					mSec;													/* milisecond						*/
}T_RFCP_DATETIME;
/* @} */

/******************************************************************************************************************************/
/*!
 * @brief rfcp info
 * @note  rfcp info
 * @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					thdState;												/* Thread State						*/
}T_RFCP_INFO;
/* @} */

/******************************************************************************************************************************/
/** antenna carrier info table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					onOffState;				/* ON OFF状態					*/
	UINT					frequency;				/* 周波数						*/
	UINT					bandWidth;				/* BandWidth					*/
	UINT					gain;					/* gain							*/
	UINT					delay;					/* delay						*/
	UINT					noModState;				/* 無変調設定状態				*/
	UINT					noModPower;				/* 無変調送信電力				*/
	UINT					tddUlDlConfig;			/**< TDD uplink/downlink configuration			*/
	UINT					tddSpeSubframeConfig;	/**< TDD special subframe configuration			*/
	UINT					tddDlCyclicPrefixLen;	/**< TDD downlink cyclic prefix length			*/
}T_RFCP_CAR_INFO;
/** @} */

/******************************************************************************************************************************/
/** antenna carrier info table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	
	UINT					paState;							/* PA状態						*/
	UINT					stopWaveState;						/* 停波状態						*/
	T_RFCP_CAR_INFO			carInfo[D_SYS_CAR_MAX][D_SYS_ANT_DIRECTION_NUM];	/* carInfo		*/
	T_RFCP_CAR_INFO			carInfo_3g[D_SYS_CAR_3G_MAX];		/* carInfo for 3G				*/
}T_RFCP_ANT_INFO;
/** @} */

/******************************************************************************************************************************/
/** cpri info table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					cpriState;							/* CPRI状態						*/
}T_RFCP_CPRI_INFO;
/** @} */

/******************************************************************************************************************************/
/** carrier info table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					sysBandwidth[D_RRH_CAR_MAX];		/* bandwidth(system parameter)(2Carrier)*/
	USHORT 					sec_chip[D_RRH_ANT_MAX];			/* セクタnチップ補正(0-16)(4branch)		*/
	USHORT 					cpri_rate;							/* CPRI Rate(1:4.9, 2:9.8)				*/
	USHORT					tx_sys_num;							/* 送信系統数値保持						*/
	T_RFCP_ANT_INFO			antInfo[D_SYS_ANT_MAX];				/* ANT Info								*/
	T_RFCP_CPRI_INFO		cpriInfo[D_SYS_CPRI_PORT_NUM];		/* CPRI Info							*/
}T_RFCP_CAR_MNG_INFO;
/** @} */

/******************************************************************************************************************************/
/** alarm manage table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					alarmState[D_SYS_ANT_MAX];			/* Alarm発生状態				*/
}T_RFCP_ALM_MNG_INFO;
/** @} */

/******************************************************************************************************************************/
/** alarm mask table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					instanceId;							/* Instance Id					*/
	UINT					mask;								/* mask							*/
}T_RFCP_ALM_MASK_INFO;
/** @} */

/******************************************************************************************************************************/
/** frequency check table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					freqMin;							/* frequency(min)				*/
	UINT					freqMax;							/* frequency(max)				*/
}T_RFCP_FREQ_CHK_INFO;
/** @} */

/******************************************************************************************************************************/
/** bandwidth check table
 *  @addtogroup RRH_PF_RFP

 *  @{
 */
/******************************************************************************************************************************/
typedef struct{
	UINT					bwNum;								/* bandwidth数					*/
	UINT					bwVal[D_SYS_BW_NUM];				/* bandwidth					*/
}T_RFCP_BW_CHK_INFO;
/** @} */


typedef T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ	T_RFCP_PRCCEDT_REG ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REQ			T_RFCP_UPCNF_GET_REQ ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM			T_RFCP_UPCNF_GET_CMF ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ		T_RFCP_UPCNF_TXEDIT_REQ ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ		T_RFCP_UPCNF_TXEDIT_REJ ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ		T_RFCP_UPCNF_RXEDIT_REQ ;
typedef T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ		T_RFCP_UPCNF_RXEDIT_REJ ;

#endif
/** @} */

