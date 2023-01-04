/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  mnt_com_ftyp.h
 * @brief mnt com data colloction function declaration.
 * @date  2013/11/28 FFCS)hongj Create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef MNT_COM_FTYP
#define MNT_COM_FTYP

extern T_CMD_TCPCONN_NUM mnt_com_get_tcpconn_simulator(USHORT link_num);
extern T_CMD_TCPCONN_NUM mnt_com_get_tcpconn_monitor(USHORT link_num);
extern USHORT mnt_com_conv_linkno_internal_to_simu(USHORT link_num);
extern USHORT mnt_com_conv_linkno_simu_to_internal(USHORT link_num, INT linkhandle);
extern VOID mnt_com_conv_linkhandle_to_internalrange(INT linkhandle, UINT *link_min, UINT *link_max);

extern INT mnt_com_msgQSend(UINT q_num, const VOID *msgp, UINT msgsz, INT *errcd);

extern INT mnt_com_msgQReceive(UINT q_num, INT q_flag, VOID *timeout, UINT msgsz, VOID *msgp, UINT *rcvmsgsz, INT *errcd);

extern USHORT mnt_com_swap_USHORT(USHORT target);

extern UINT mnt_com_swap_UINT(UINT target);


#endif	/*MNT_COM_FTYP*/
