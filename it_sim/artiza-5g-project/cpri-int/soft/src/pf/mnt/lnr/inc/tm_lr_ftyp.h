/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_lr_ftyp.h
 *  @brief	LAN Message Receiving task common function prototype definition
 *  @date   2008/07/29 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/


#ifndef TM_LR_FTYP
#define TM_LR_FTYP

extern VOID tm_lr_Main(UINT src_id,UINT event_no,UINT*a_inf_p);



extern UINT tm_lr_Anlz(UINT src_id,UINT event_no);



extern VOID tm_lr_Init(UINT*a_inf_p);



extern VOID tm_lr_TcpInit(UINT*a_inf_p);



extern VOID tm_lr_TcpStr(UINT*a_inf_p);



extern INT tm_lr_cm_TcpRead(INT a_sockethandle,CHAR *a_buff_p,INT a_bufflen,INT a_duration);



extern UINT tm_lr_cm_SrvSocketInit(INT*a_srvsocket_p,USHORT a_port);



extern VOID tm_lr_Nop(UINT*a_inf_p);



extern VOID tm_lr_Abort(UINT*a_inf_p);



UINT tm_lr_cm_ChkActTcp(int *,int );

#endif/*TM_LR_FTYP*/

