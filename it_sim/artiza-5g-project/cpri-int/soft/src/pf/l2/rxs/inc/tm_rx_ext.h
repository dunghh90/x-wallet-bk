/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_rx_ext.h
 * @brief Rxs data colloction external declaration.
 * @date  2008/07/23 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_RX_EXT
#define    TM_RX_EXT

/*  Main matrix table               */
/*!
 * @name ROM data 
 */
extern VOID (* const rxr_mtrxtbl[RXD_MX_STATE][RXD_MX_EVENT])(UINT *inf_p) ; 


/*!
 * @name task state no 
 */
extern UINT rxw_stateno ;				/* Task state no		*/

/*!
 * @name event  no 
 */
extern UINT rxw_eventno ;				/* Event no				*/


/*!
 * @name Need/needn't free msg buffer 
 */
extern USHORT rxw_needfrmsg ;			/* Need/needn't free msg buffer	*/

#endif	/*TM_RX_EXT*/
