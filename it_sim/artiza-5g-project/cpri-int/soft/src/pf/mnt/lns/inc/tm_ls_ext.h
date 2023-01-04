/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_ls_ext.h
 *  @brief	LAN Message Sending task external common table declaration  
 *  @date   2008/07/30 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/

#ifndef TM_LS_EXT
#define TM_LS_EXT
/****************************************************************************/
/*	Grobal area extern definition											*/
/****************************************************************************/
/*	ROM data	*/
										/*	main matrix table	*/
extern	void	( *lsr_mtrxtbl[ LSD_MAX_STATE ][ LSD_MAX_EVENT ] )
															( UINT * );
/*	RAM data	*/
extern UINT lsw_tststat;/*Task state*/
extern UINT lsw_eventno;/*Received event number*/
extern LST_ERRINFO lsw_errinfo;/*Table to record error info returned by socket function*/
#endif/*TM_LS_EXT*/

