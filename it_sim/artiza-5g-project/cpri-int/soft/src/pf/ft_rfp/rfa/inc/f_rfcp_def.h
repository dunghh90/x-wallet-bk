/*!
* @skip $Id:$
* @file f_rfcp_def.h
* @brief RFCP thread Define Header
* @date 2013/03/21 ALPHA)松延 Create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

/** @addtogroup RRH_PF_RFP
 *  @{
 */
 
#ifndef F_RFCP_DEF_H
#define F_RFCP_DEF_H

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_rfcp state
 * @note pf_rfcp state definition
 * @{
 */
#define	D_RFCP_THD_INIT										0			/* Thread state: 初期状態				*/
#define	D_RFCP_THD_ACT										1			/* Thread state: 運用状態				*/
#define	D_RFCP_STATENUM										2			/* Maximum state number					*/
/* @} */

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_rfcp Parameter Check
 * @note pf_rfcp Parameter Check definition
 * @{
 */
#define	D_RFCP_PARA_CHECK_NOBOTH							0			/* BOTH非許容							*/
#define	D_RFCP_PARA_CHECK_BOTH								1			/* BOTH許容								*/
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_rfcp event
 * @note pf_rfcp event definition
 * @{
 */
#define	D_RFCP_EVE_RFCP_INIT								0x0000		/* スレッド起動							*/
#define	D_RFCP_EVE_SYSTEM_PARAM_UPDATE						0x0001		/* System Parameter Update Request		*/
#define	D_RFCP_EVE_CARRIER_ON								0x0002		/* Carrier ON Request					*/
#define	D_RFCP_EVE_CARRIER_OFF								0x0003		/* Carrier OFF Request					*/
#define	D_RFCP_EVE_TXRXCAL_INUSE_REQUEST					0x0004		/* The request of TxCal RxCal in use	*/
#define	D_RFCP_EVE_TXRXCAL_TRAINING_REQUEST					0x0005		/* The request of TxCal RxCal Training	*/
#define D_RFCP_EVE_TXON_COMFIRM								0x0006		/* TX-ON(M-Plane) Comfirm				*/
#define D_RFCP_EVE_RXON_COMFIRM								0x0007		/* RX-ON(M-Plane) Comfirm				*/
#define D_RFCP_EVE_TXOFF_COMFIRM							0x0008		/* TX-OFF(M-Plane) Comfirm				*/
#define D_RFCP_EVE_RXOFF_COMFIRM							0x0009		/* RX-OFF(M-Plane) Comfirm				*/
#define D_RFCP_EVE_PRCCEMT_REQ								0x000A		/* Processing Element GET Request		*/
#define D_RFCP_EVE_STATUS_REQ								0x000B		/* u-plane conf. status GET Request		*/
#define D_RFCP_EVE_TXEDIT_REQ								0x000C		/* u-plane conf. TX-EDIT Request		*/
#define D_RFCP_EVE_RXEDIT_REQ								0x000D		/* u-plane conf. RX-EDIT Request		*/
#define	D_RFCP_EVENTNUM										0x000E		/* イベント数							*/
/* @} */

#define D_RFCP_PRCCEDT_PORT0								0			/* ポート番号 = 0						*/
#define D_RFCP_PRCCEDT_PORT1								1			/* ポート番号 = 1						*/

#define D_RFCP_FILTVID1_EBL									0x00010000	/* u-fc Plane VLan ID有効				*/
#define D_RFCP_FILTVID1_DBL									0x00000000	/* u-fc Plane VLan ID無効				*/

#define D_RFCP_CAPTAG_PCP									0xFFFF0000	/* Up link　VlanID領域無効				*/

#define D_RFCP_UPCNF_TXRX_MAX								2
#define D_RFCP_UPCNF_STSTX									1
#define D_RFCP_UPCNF_STSRX									0

#define D_RFCP_UPCNF_RSLT_OK								0x00000000
#define D_RFCP_UPCNF_RSCE_BUSY								0x00000001
#define D_RFCP_UPCNF_ERR_MSG_SIZE							32

#define D_RFCP_IPCNF_NON_SEND_MSG_LNGH						0
#define D_RFCP_PREMT_MAC_ADDR_INIT							0x00000000


#endif
/** @} */


/*!
 * @name pf_rfcp event for SLC&1Chp
 * @note pf_rfcp event enum
 * @{
 */
enum
{
	E_RFCP_EVE_RFCP_INIT = 0,					/* スレッド起動											*/
	E_RFCP_EVE_SYSTEM_PARAM_UPDATE,				/* System Parameter Update Request						*/
	E_RFCP_EVE_PA_CONTROL,						/* PA Control Request									*/
	E_RFCP_EVE_WAVE_CONTROL,					/* APD Wave Control Request								*/
	E_RFCP_EVE_CARRIER_ON_3G,					/* Carrier ON Request(3G)								*/
	E_RFCP_EVE_CARRIER_ON_S3G,					/* Carrier ON Request(S3G)								*/
	E_RFCP_EVE_CARRIER_OFF_3G,					/* Carrier OFF Request(3G)								*/
	E_RFCP_EVE_CARRIER_OFF_S3G,					/* Carrier OFF Request(S3G)								*/
	E_RFCP_EVE_NO_MOD_CAR_3G,					/* No Modulation Carrier ON/OFF Request(3G)				*/
	E_RFCP_EVE_NO_MOD_CAR_S3G,					/* No Modulation Carrier ON/OFF Request(S3G)			*/
	E_RFCP_EVE_CPRI_CONTROL,					/* CPRI Control Request									*/
	E_RFCP_EVE_GET_CAR_FREQ,					/* Get Carrier Frequency Info Req						*/
	E_RFCP_SLC1CHP_EVENTNUM						/* イベント数(SLC&1Chip用)								*/
};