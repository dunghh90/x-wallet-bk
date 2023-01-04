/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  l2_com_ftyp.h
 * @brief l2 com data colloction function declaration.
 * @date  2013/11/15 FFCS)hongj Create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef L2_COM_FTYP
#define L2_COM_FTYP

#include "m_lb_ftyp.h"
#include "tm_lb_ftyp.h"
#include "tm_pl_ftyp.h"
#include "tm_rx_ftyp.h"
#include "tm_tx_ftyp.h"
#include "l2_dbg_ftyp.h"

extern VOID l2_com_comMsgSnd(UINT msgQid,UINT msgid,void *msgp,UINT msgsz );

extern INT l2_com_comMsgRcv(UINT msgQ,INT msgSize,VOID*rcvMsg,UINT*rcvLen,INT*errcd);

extern VOID l2_com_monitorMsgToApl(UINT msgQid,void *msgp,UINT msgsz );

extern VOID l2_com_cpriMsgToApl(UINT msgQid,void *msgp,UINT msgsz );

extern INT l2_com_timeCompare(T_RRH_SYSTEM_TIME t1, T_RRH_SYSTEM_TIME t2 );

extern USHORT l2_com_swap_USHORT(USHORT target);

extern UINT l2_com_swap_UINT(UINT target);

extern void l2_com_swap_convert(USHORT linkno, USHORT signal_kind, VOID *body, CHAR direction, UINT l3_len);

#endif	/*L2_COM_FTYP*/
