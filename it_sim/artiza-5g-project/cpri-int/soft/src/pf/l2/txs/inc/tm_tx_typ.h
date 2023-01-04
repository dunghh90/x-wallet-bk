/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_tx_typ.h
 * @brief Txs data structure definition.
 * @date  2008/07/18 FFCS)zouw Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef TM_TX_TYP
#define TM_TX_TYP


/*!
 * @brief Maker MT mode.
 */
typedef struct
{
	USHORT 		monitor;	/* 1:Monitor mode is ON;    0:Monitor mode is OFF;	*/
	USHORT 		simulator;	/* 1:Simulator mode is ON;  0:Simulator mode is OFF;*/
}TXT_MKMTMODE;

#endif	/*TM_TX_TYP*/
