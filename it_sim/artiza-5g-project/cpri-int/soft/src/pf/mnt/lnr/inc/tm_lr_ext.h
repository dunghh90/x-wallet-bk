/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_lr_ext.h
 *  @brief	LAN Message Receiving task external common table declaration  
 *  @date   2008/07/30 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/



#ifndef TM_LR_EXT
#define TM_LR_EXT

/****************************************************************************/
/*	Grobal area extern definition											*/
/****************************************************************************/
/*	ROM data	*/
										/*	main matrix table				*/
extern	void	( *lrr_mtrxtbl[ LRD_MAX_STATE ][ LRD_MAX_EVENT ] )
															( UINT * );

/*	RAM data	*/
extern UINT lrw_tststat;			/*task state*/
extern UINT lrw_eventno;          		/*task received event*/

 /*server tcp handle for simulator/monitor data transfer*/
extern INT lrw_srvsocket[LRD_SRVCOUNT];    
extern LRT_ERRINFO lrw_errinfo;		/*table to record error returned by socket function*/
#endif/*TM_LR_EXT*/

