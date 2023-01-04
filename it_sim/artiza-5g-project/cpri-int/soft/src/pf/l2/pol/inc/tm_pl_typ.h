/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_pl_typ.h
 * @brief Polling data structure definition.
 * @date  2008/07/24 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/

#ifndef    TM_PL_TYP
#define    TM_PL_TYP

/**
 * @brief   polling log
 */
typedef struct
{
	USHORT		loglable ;		
	USHORT		event_no ;			/*	起動要因番号		*/
	USHORT		year ;				/*	書込発生年			*/
	UCHAR		month ;				/*	書込発生月			*/
	UCHAR		day ;				/*	書込発生月			*/
	UCHAR		hour ;				/*	書込発生時			*/
	UCHAR		min ;				/*	書込発生分			*/
	UCHAR		sec ;				/*	書込発生秒			*/
	UCHAR		msec ;				/*	書込発生ms			*/
	UINT 		regadr;				/* register address */
	UINT		regdat;				/* register data */	
}
PLW_LOG_T;

#endif	/*TM_PL_TYP*/
