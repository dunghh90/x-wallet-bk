/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_tx_ext.h
 * @brief Txs data colloction external declaration.
 * @date  2008/07/18 FFCS)zouw Create
 * @date  2013.11.22 FFCS)hongj modify for zynq.
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef TM_TX_EXT
#define TM_TX_EXT

/*!
 * @name ROM data 
 */
extern VOID (* const txr_mtrxtbl[TXD_MX_STATE][TXD_MX_EVENT])(UINT *inf_p);/* Main matrix table               */

/*!
 * @name RAM data 
 */
extern USHORT txw_curbankidx[D_RRH_CPRINO_NUM];	/* Current to-be-selected bank index */

/*!
 * @name state number 
 */
extern UINT txw_stateno;					

/*!
 * @name event number 
 */
extern UINT txw_eventno;

/*!
 * @name Need/needn't free message buffer
 */
extern USHORT txw_needfrmsg;

/*!
 * @name monitor link bank count  
 */
extern UINT mon_lnkbakcount;

#endif	/*TM_TX_EXT*/
