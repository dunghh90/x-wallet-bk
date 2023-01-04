/*!
* @skip $Id:$
* @file f_rfcp_ftyp.h
* @brief RFCP thread's function declaration file
* @date 2013/03/21 ALPHA)松延 Create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

/** @addtogroup RRH_PF_RFP
 *  @{
 */
 
#ifndef F_RFCP_FTYP_H
#define F_RFCP_FTYP_H

#include "f_rfcp_inc.h"

/* スレッド共通処理 */
void f_rfcp_main(void);
void f_rfcp_main_slc1chp(void);
INT  f_rfcp_init(void*);
INT  f_rfcp_anlz(void* msgP);
INT	 f_rfcp_anlz_slc1chp(void* msgP);
INT  f_rfcp_abort(void* rcvMsg_p);

/* メッセージ受信処理 */
INT  f_rfcp_act_systemParamUpdate(void* rcvMsg_p);
INT  f_rfcp_act_paControl(void* rcvMsg_p);
INT  f_rfcp_act_waveControl(void* rcvMsg_p);
INT  f_rfcp_act_carrierOn(void* rcvMsg_p);
INT  f_rfcp_act_carrierOff(void* rcvMsg_p);
INT  f_rfcp_txrxcal_inuse(VOID* msgP);
INT  f_rfcp_txrxcal_training(VOID* msgP);

INT  f_rfcp_act_noModCar(void* rcvMsg_p);
INT  f_rfcp_act_iqControl(void* rcvMsg_p);
INT  f_rfcp_act_getCarFreq(void* rcvMsg_p);
UINT f_rfcp_sendCarrierOnSetting( T_SYS_CARRIER_ON_SETTING_REQ_INFO *pCarSetting, UINT uiSrcPQId, UINT uiSrcTQId );
UINT f_rfcp_sendCarrierOffSetting( T_SYS_CARRIER_OFF_SETTING_REQ_INFO *pCarInfo, UINT uiSrcPQId, UINT uiSrcTQId );
UINT f_rfcp_sendNoModCarSetting( T_SYS_NO_MOD_CAR_REQ_INFO *pNoModSetting, UINT uiSrcPQId, UINT uiSrcTQId, UINT sysType);

INT f_rfcp_act_noModCar_3G(VOID* msgP);
INT f_rfcp_act_noModCar_S3G(VOID* msgP);
INT f_rfcp_act_noModCar_slc1chp(VOID* msgP, UINT sysType);
INT f_rfcp_act_carrierOn_3G(VOID* msgP);
INT f_rfcp_act_carrierOn_S3G(VOID* msgP);
INT f_rfcp_act_carrierOn_slc1chp(VOID* msgP, UINT sysType);
INT f_rfcp_act_carrierOff_3G(VOID* msgP);
INT f_rfcp_act_carrierOff_S3G(VOID* msgP);
INT f_rfcp_act_carrierOff_slc1chp(VOID* msgP, UINT sysType);
INT f_rfcp_prccelmt_EDIT(VOID* msgP);
int f_rfcp_upcnf_GET(VOID* msgP);
int f_rfcp_upcnf_TXEDIT(VOID* msgP);
int f_rfcp_upcnf_RXEDIT(VOID* msgP);
int f_rfcp_txon_comfirm(VOID* msgP);
int f_rfcp_rxon_comfirm(VOID* msgP);
int f_rfcp_txoff_comfirm(VOID* msgP);
int f_rfcp_rxoff_comfirm(VOID* msgP);

INT f_rfcp_com_CarrierCheck(UINT carId);
INT f_rfcp_com_BranchCheck(UINT branch);
INT f_rfcp_com_OnOffStateCheck(UINT branch, UINT carId, UINT onOffState);


#endif

/** @} */
