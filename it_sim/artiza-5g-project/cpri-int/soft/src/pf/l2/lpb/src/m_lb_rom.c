/*!
 * @skip    $Id$
 * @file    m_lb_rom.c
 * @brief   LAPB readable variable 定義
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date   2009/04/16  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"			/*common header		  			*/
#include "tm_lb_header.h"			/*lapb task header	  			*/
#include "m_lb_header.h"			/*lapb task header*/


/**	main matrix table */
void	(* const lbr_0_m0[LBD_MX_STATE][LBD_MX_EVENT])              /*!< main matrix table */
						(UINT a_source,UINT a_event, UINT *a_inf_p) =
{
/*********************************************initialize state****************************************************************/
/*                              L3 test 停止通知                                                                             */
/*                              L3 test 開始通知                                                                             */
/*                              L2 STOP依頼                                                                                  */
/*                              CPRI信号送信通知                                                                             */
/*                              Layer 1 Data CNF                                                                             */
/*                              L1データ受信通知                                                                             */
/*                              自受信ビジーの解除                                                      L2 DEACT依頼         */
/* 	初期化要求                  自受信ビジー            タイムアウト発生通知    データリンク設定要求    データリンク解放要求 */
    {tm_lb_0_m0e0s0p,           tm_lb_Abort,            tm_lb_Abort,            tm_lb_Abort,            tm_lb_Abort},
/*********************************************normal state********************************************************************/
/*                              L3 test 停止通知                                                                             */
/*                              L3 test 開始通知                                                                             */
/*                              L2 STOP依頼                                                                                  */
/*                              CPRI信号送信通知                                                                             */
/*                              Layer 1 Data CNF                                                                             */
/*                              L1データ受信通知                                                                             */
/*                              自受信ビジーの解除                                                      L2 DEACT依頼         */
/*  初期化要求                  自受信ビジー            タイムアウト発生通知    データリンク設定要求    データリンク解放要求 */
    {tm_lb_Nop,                 tm_lb_0_m0e1s1p,        tm_lb_0_m0e2s1p,        tm_lb_0_m0e1s1p,        tm_lb_0_m0e1s1p},
/*********************************************L2 setup************************************************************************/
/*                              L3 test 停止通知                                                                             */
/*                              L3 test 開始通知                                                                             */
/*                              L2 STOP依頼                                                                                  */
/*                              CPRI信号送信通知                                                                             */
/*                              Layer 1 Data CNF                                                                             */
/*                              L1データ受信通知                                                                             */
/*                              自受信ビジーの解除                                                      L2 DEACT依頼         */
/*  初期化要求                  自受信ビジー            タイムアウト発生通知    データリンク設定要求    データリンク解放要求 */
    {tm_lb_Nop,                 tm_lb_0_m0e1s2p,        tm_lb_0_m0e2s2p,        tm_lb_0_m0e3s2p,        tm_lb_0_m0e1s1p}
};

/** L2 message matrix table */
void	(* const lbr_1_m1[LBD_SIGNAL_MX_STATE][LBD_SIGNAL_MX_EVENT])
						(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p) =
{
/****************************************************state 3*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s0p,				tm_lb_1_Nop,				tm_lb_1_m1e2s0p,			tm_lb_1_m1e2s0p,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame      	*/
	tm_lb_1_m1e2s0p,				tm_lb_1_m1e2s0p,			tm_lb_1_m1e2s0p,			tm_lb_1_m1e2s0p,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame   		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop, 
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_Nop, 				tm_lb_1_m1e18s0p,			tm_lb_1_RelNG,
/*	L2 busy happen;		 			L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS   */
	tm_lb_1_Nop,					tm_lb_1_m1e25s0p,		tm_lb_1_Nop},
/****************************************************state 4*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s0p,				tm_lb_1_Nop,				tm_lb_1_m1e2s0p,			tm_lb_1_m1e2s0p,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame			*/
	tm_lb_1_m1e2s0p,				tm_lb_1_m1e2s0p,			tm_lb_1_Nop /*	tm_lb_1_m1e6s1p */,	tm_lb_1_m1e2s0p,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame   		*/
	tm_lb_1_m1e8s1p,				tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request;	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_Nop, 				tm_lb_1_m1e18s0p,			tm_lb_1_RelNG,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_m1e22s1p,			tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS          */
	tm_lb_1_Nop,					tm_lb_1_m1e25s0p,		tm_lb_1_Nop},
/****************************************************state 5*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame; 					RR  Frame      		*/
	{tm_lb_1_m1e0s0p,				tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame   		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_m1e6s2p,			tm_lb_1_m1e7s2p,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame  		*/
	tm_lb_1_m1e8s2p,				tm_lb_1_m1e9s2p,			tm_lb_1_Nop,				tm_lb_1_Nop,
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_m1e12s2p,				tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_m1e17s2p, 			tm_lb_1_m1e18s2p,			tm_lb_1_RelNG,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_m1e22s2p,			tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS  */
	tm_lb_1_Nop,					tm_lb_1_m1e25s2p,		tm_lb_1_Nop},
/****************************************************state 6*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s0p,				tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame			*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_m1e6s3p,			tm_lb_1_Nop,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame  		*/
	tm_lb_1_m1e8s3p,				tm_lb_1_m1e8s3p,			tm_lb_1_Nop,				tm_lb_1_Nop,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_m1e12s3p,				tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_m1e17s3p, 			tm_lb_1_SetupNG,			tm_lb_1_RelNG,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_m1e22s3p,			tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS          */
	tm_lb_1_Nop,					tm_lb_1_m1e25s2p,		tm_lb_1_Nop},
/****************************************************state 7*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s4p,				tm_lb_1_m1e1s4p,			tm_lb_1_m1e2s4p,			tm_lb_1_m1e3s4p,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame       	*/
	tm_lb_1_m1e4s4p,				tm_lb_1_m1e5s4p,			tm_lb_1_m1e6s4p,			tm_lb_1_m1e7s4p,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame 		*/
	tm_lb_1_m1e8s4p,				tm_lb_1_m1e9s4p,			tm_lb_1_m1e10s4p,			tm_lb_1_m1e11s4p,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_m1e12s5p,				tm_lb_1_m1e13s4p,			tm_lb_1_m1e14s4p,			tm_lb_1_m1e15s4p,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_m1e16s4p,				tm_lb_1_m1e17s4p, 			tm_lb_1_m1e18s4p,			tm_lb_1_m1e19s4p,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_m1e20s4p,				tm_lb_1_m1e21s4p,			tm_lb_1_m1e22s4p,			tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS        */
	tm_lb_1_Nop,					tm_lb_1_m1e25s4p,		tm_lb_1_Nop},
/****************************************************state 8*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s4p,				tm_lb_1_m1e1s5p,			tm_lb_1_m1e2s5p,			tm_lb_1_m1e3s5p,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame 			*/
	tm_lb_1_m1e4s5p,				tm_lb_1_m1e3s5p,			tm_lb_1_m1e6s4p,			tm_lb_1_m1e7s4p,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame  		*/
	tm_lb_1_m1e8s4p,				tm_lb_1_m1e9s4p,			tm_lb_1_m1e10s4p,			tm_lb_1_m1e11s4p,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_m1e12s5p,				tm_lb_1_m1e13s4p,			tm_lb_1_m1e14s5p,			tm_lb_1_m1e15s4p,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_m1e16s4p,				tm_lb_1_m1e17s4p, 			tm_lb_1_m1e18s4p,			tm_lb_1_m1e19s4p,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_m1e20s4p,				tm_lb_1_m1e21s4p,			tm_lb_1_m1e22s4p,			tm_lb_1_Nop,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS  */
	tm_lb_1_Nop,					tm_lb_1_m1e25s4p,		tm_lb_1_Nop},
/************************************************ state 7 (N/E)*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame 			*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame   		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_Nop, 				tm_lb_1_SetupNG,				tm_lb_1_RelNG,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request;			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS     */
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop},
/*************************************************state 8 (N/E)**************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification;  I  Frame;  					RR  Frame      		*/
	{tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame 			*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame  		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_Nop, 				tm_lb_1_SetupNG,				tm_lb_1_RelNG,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS     */
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop},
/****************************************************state 10*****************************************************/
/*  L1-DATA-CNF; 					CPRI Sending notification; 	I  Frame;  					RR  Frame      		*/
	{tm_lb_1_m1e0s8p,				tm_lb_1_m1e1s8p,			tm_lb_1_m1e2s8p,			tm_lb_1_Nop,
/*  RNR Frame; 						REJ Frame; 					SABM Frame; 				DISC Frame 			*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*  DM Frame; 						UA Frame; 					FRMR Frame; 				ERROR Frame  		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,  
/*  T1 time out; 					T2 time out; 				T3 time out;				T4 time out; 		*/
	tm_lb_1_Nop, 					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_Nop,
/*	L2 busy detect timer time out,  L2 deact request; 			L2 establishment request; 	L2 release request; */
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_SetupNG, 			tm_lb_1_m1e19s4p,
/*	L2 busy happen; 				L2 busy cancel; 			L2 stop request; 			L3 test start; 		*/
	tm_lb_1_Nop,					tm_lb_1_Nop,				tm_lb_1_Nop,				tm_lb_1_m1e23s1p,
/*  L3 test stop; 					L1 busy detect timer time out,	OTHERS   */
	tm_lb_1_m1e24s8p,				tm_lb_1_m1e0s8p,				tm_lb_1_Nop}
};

/* @} */



