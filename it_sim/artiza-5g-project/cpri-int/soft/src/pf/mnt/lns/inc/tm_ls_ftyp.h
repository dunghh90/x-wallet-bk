/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_ls_ftyp.h
 *  @brief	LAN Message Sending task common function prototype definition
 *  @date   2008/07/29 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/


#ifndef TM_LS_FTYP
#define TM_LS_FTYP

/**/
extern VOID tm_ls_Main(UINT src_id,UINT event_no,UINT* a_inf_p);


/**/
extern UINT tm_ls_Anlz(UINT src_id,UINT event_no);


/**/
extern VOID tm_ls_Init(UINT* a_inf_p);


/**/
extern VOID tm_ls_TcpSnd(UINT*a_inf_p);


/**/
extern VOID tm_ls_TcpRel(UINT*a_inf_p);


/**/
extern VOID tm_ls_Nop(UINT*a_inf_p);


/**/
extern VOID tm_ls_Abort(UINT*a_inf_p);


extern int tm_ls_cm_send(int,char*,int);
#endif/*TM_LS_FTYP*/

