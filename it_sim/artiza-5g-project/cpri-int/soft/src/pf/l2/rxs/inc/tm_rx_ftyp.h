/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_rx_ftyp.h
 * @brief Rxs data colloction function declaration.
 * @date  2008/07/18 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_RX_FTYP
#define    TM_RX_FTYP


/* Main part */
extern VOID tm_rx_Main(UINT source, UINT event, UINT *inf_p);

extern VOID tm_rx_Init(UINT *inf_p);

extern VOID tm_rx_Recv(UINT *inf_p);

extern VOID tm_rx_Simrecv(UINT *inf_p);

extern VOID tm_rx_MkMtStr(UINT *inf_p);

extern VOID tm_rx_MkMtStp(UINT *inf_p);

extern VOID tm_rx_Nop(UINT *inf_p);

extern VOID tm_rx_Abort(UINT *inf_p);

/* Sub part */
extern UINT tm_rx_Anlz(UINT source, UINT event);

extern UINT tm_rx_ReadLinks(USHORT *linkno, USHORT frame_num);

extern VOID tm_rx_RxLinkMng(USHORT linkno, USHORT *frame_cnt_p);

extern UINT tm_rx_cm_GetFrmNum(USHORT linkno);

extern UINT tm_rx_cm_OneBitNum(UINT rxstareg);

extern VOID tm_rx_LocBnBuf(USHORT linkno, USHORT bank_idx, UINT*bnbuf_p);

extern VOID tm_rx_cm_SndMonMsg(CMT_TSKIF_HEAD *a_msg_p,INT a_msglen);

#endif	/*TM_RX_FTYP*/
