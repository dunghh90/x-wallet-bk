/*!
 * @skip		$ld:$
 * @file		f_cmn_cmu_ftyp.h
 * @brief		cmu プロトタイプ宣言
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#ifndef F_CMU_FTYP_H
#define F_CMU_FTYP_H

/* com */
VOID f_cmu_com_abort(VOID *rcvMsg_p);
UINT f_cmu_com_init(T_CMU_CREATE_THREAD* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl_p, USHORT* usThdState);
VOID f_cmu_com_iniTbl(T_CMU_CREATE_THREAD* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl_p);
UINT f_cmu_com_addSearch(UINT uiEventID , UINT uiSignalKind, UINT uiQueID, T_CMU_ROUTE_MNG* tRouteTbl_p);
UINT f_cmu_com_delSearch(UINT uiEventID , UINT uiSignalKind, UINT uiQueID, T_CMU_ROUTE_MNG* tRouteTbl_p);
UINT f_cmu_com_searchID( UINT EventNo , UINT SignalKind , T_CMU_ROUTE_MNG* tRouteTbl_p , UINT* uiQueBox_p);
VOID f_cmu_com_msgSend(T_RRH_MESSAGE_TYPE* rcvMsg_p, T_CMU_THREAD_MNG* tProcTbl, UINT uiDstQueID);
UINT f_cmu_com_msgRecvSend(T_RRH_MESSAGE_TYPE* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl);

/* recv */
UINT f_recv_addMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID);
UINT f_recv_anlz(VOID *msgP);
UINT f_recv_delMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID);
VOID f_recv_main(T_CMU_CREATE_THREAD *tRcvMsg);
VOID f_recv_msgRecvSend(VOID* rcvMsg_p);

/* send */
UINT f_send_addMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID);
UINT f_send_anlz(VOID *msgP);
UINT f_send_delMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID);
VOID f_send_main(T_CMU_CREATE_THREAD *tRcvMsg);
VOID f_send_msgRecvSend(VOID* rcvMsg_p);



VOID f_mpproxy_recv(T_CMU_CREATE_THREAD_MP *tRcvMsg);
VOID f_mpproxy_send(T_CMU_CREATE_THREAD_MP *tRcvMsg);



#endif
/* @} */
