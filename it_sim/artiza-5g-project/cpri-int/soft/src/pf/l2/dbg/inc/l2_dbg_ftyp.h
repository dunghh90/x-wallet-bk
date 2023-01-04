/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  l2_dbg_ftyp.h
 * @brief l2 dbg data colloction function declaration.
 * @date  2013/12/03 FFCS)hongj Create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef L2_DBG_FTYP
#define L2_DBG_FTYP


/* Main part */
extern VOID tm_dbg_Main(CHAR * a_inf_p);

extern VOID l2_dbg_abort(UINT *inf_p);

extern VOID l2_dbg_cpriMsgSub(UINT *inf_p);

extern VOID l2_dbg_cpriMsgUnsub(UINT *inf_p);

extern VOID l2_dbg_l2StateSub(UINT *inf_p);

extern VOID l2_dbg_l2StateUnsub(UINT *inf_p);

extern VOID l2_dbg_monStart(UINT *inf_p);

extern VOID l2_dbg_monStop(UINT *inf_p);

extern VOID l2_dbg_simuStart(UINT *inf_p);

extern VOID l2_dbg_simuStop(UINT *inf_p);

extern VOID l2_dbg_statisticsInd(UINT *inf_p);


/* Sub part */
extern UINT l2_dbg_Anlz(UINT a_event);

#endif	/*L2_DBG_FTYP*/
