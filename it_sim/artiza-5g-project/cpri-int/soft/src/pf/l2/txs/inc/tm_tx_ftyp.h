/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_tx_ftyp.h
 * @brief Txs data colloction function declaration.
 * @date  2008/07/18 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef TM_TX_FTYP
#define TM_TX_FTYP


/* Main part */
extern VOID tm_tx_Main(UINT source, UINT event, UINT *inf_p);

extern VOID tm_tx_Init(UINT *inf_p);

extern VOID tm_tx_Send(UINT *inf_p);

extern VOID tm_tx_MkMtStr(UINT *inf_p);

extern VOID tm_tx_MkMtStp(UINT *inf_p);

extern VOID tm_tx_Abort(UINT *inf_p);

extern VOID tm_tx_Nop(UINT *inf_p);


/* Sub part */
extern UINT  tm_tx_Anlz(UINT srctsk, UINT event);

extern VOID tm_tx_SelectBank(USHORT link_idx, UINT *bank_idx_p);

extern VOID tm_tx_LocBnBuf(USHORT linkno, USHORT bank_idx, UINT *bnbuf_p);

extern VOID tm_tx_cm_SndMonMsg( INT linkno, CMT_TSKIF_HEAD *monbuf_p,INT a_msglen);

extern VOID tm_tx_cm_SndSimuMsg( CMT_TSKIF_L1DATRQ *a_msg_p);

#endif	/*TM_TX_FTYP*/
