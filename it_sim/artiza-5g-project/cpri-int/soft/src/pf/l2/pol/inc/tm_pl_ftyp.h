/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_pl_ftyp.h
 * @brief Polling data colloction function declaration.
 * @date  2008/07/24 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_PL_FTYP
#define    TM_PL_FTYP


/* Main part */
extern VOID tm_pl_Main(UINT source, UINT event, UINT *inf_p);

extern VOID tm_pl_Init(UINT *inf_p);

extern VOID tm_pl_Abort(UINT *inf_p);

extern VOID tm_pl_Nop(UINT *inf_p);


/* Sub part */
extern VOID tm_pl_Loop(UINT *inf_p);

extern UINT tm_pl_Anlz(UINT source, UINT event);

extern VOID tm_pl_TxPo(USHORT cprino);

extern VOID tm_pl_RxPo(VOID);

#endif	/*TM_PL_FTYP*/
