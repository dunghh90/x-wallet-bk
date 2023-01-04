/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_pl_ext.h
 * @brief Polling data colloction external declaration.
 * @date  2008/07/25 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_PL_EXT
#define    TM_PL_EXT


/*    ROM data    */
/*!
 * @name Main matrix table 
 */
extern VOID (* const plr_mtrxtbl[PLD_MX_STATE][PLD_MX_EVENT])(UINT *inf_p);

/*    RAM data    */
/*!
 * @name State number
 */
extern UINT plw_stateno;		/* State number		*/

/*!
 * @name Event number
 */
extern UINT plw_eventno;		/* Event number		*/

/*!
 * @name CPRI link amount
 */
extern USHORT plw_linknum;		/* CPRI link amount		*/

extern UINT plw_L1numer;

extern VOID* plw_assert_p;

#endif	/*TM_PL_EXT*/
