/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_rx_typ.h
 * @brief Rxs data structure definition.
 * @date  2008/07/22 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_RX_TYP
#define    TM_RX_TYP

/*!
 * @brief Maker MT mode.
 */
typedef struct 
{
	USHORT 				simulator;       /* Simulator mode			*/
	USHORT				monitor;      	   /* Monitor mode			*/
}RXT_MKMTMODE;

/**
 * @brief   RX log
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
RXW_LOG_T;

#endif	/*TM_RX_TYP*/
