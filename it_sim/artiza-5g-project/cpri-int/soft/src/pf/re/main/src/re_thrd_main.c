/*!
 *  @skip   $Id$
 *  @file   RE_thrd_main.c
 *  @brief  
 *  @date   2015/07/30 TDIPS) maruyama create
 *  @date   2015/08/04 TDIPS) maruyama modify DBGスレッド追加
 *  @date   2015/08/05 TDIPS) maruyama modify rcv,send内の処理追加
 *  @date   2015/08/20 TDIPS) maruyama modify re_rec_mainを削除（代わりはf_trp_rec_Fcom_r_main()）
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_RE_MAIN
 * @{
 */

#include "m_cm_header.h"
#include "re_com_inc.h"
#include "f_cmn_cmu_inc.h"
#include "f_dbg_thread_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  RE rcv thread
 *  @note   
 *  @return VOID
 *  @retval none
 * @date  2015/07/30 TDIPS) maruyama create
 * @date  2015/08/05 TDIPS) maruyama modify
 */
/********************************************************************************************************************/
extern T_CMU_CREATE_THREAD re_mainr_cmusRecvThdCreMng;
void re_rcv_main( void *param  )
{
//#ifndef OPT_RRH_SIM
	f_recv_main(&re_mainr_cmusRecvThdCreMng);
//#endif
}



/********************************************************************************************************************/
/*!
 *  @brief  RE send thread
 *  @note   
 *  @return VOID
 *  @retval none
 * @date  2015/07/30 TDIPS) maruyama create
 * @date  2015/08/05 TDIPS) maruyama modify
 */
/********************************************************************************************************************/
void re_send_main( void *param  )
{
//#ifndef OPT_RRH_SIM
	T_CMU_CREATE_THREAD tcrt;

	memset(&tcrt,0,sizeof(tcrt));
	tcrt.uiProcID = D_RRH_PROCID_RE;
	tcrt.uiProcQID = D_RRH_PROCQUE_RE;
	tcrt.uiThreadID = D_RRH_THDID_RE_SEND;
	tcrt.uiThreadQID = CMD_TSKID_SEND;
	tcrt.uidstThreadQID = CMD_TSKID_INI;
	tcrt.uiThreadInfo = E_RRH_CMU_SEND_ID;
	strcpy(tcrt.uiThreadName,(CHAR*)param);
	f_send_main(&tcrt);
//#endif
}


/********************************************************************************************************************/
/*!
 *  @brief  RE debug thread
 *  @note   
 *  @return VOID
 *  @retval none
 * @date  2015/08/04 TDIPS) maruyama create
 */
/********************************************************************************************************************/
extern void f_dbg_main( T_DBGTHREAD_EXEC_INFO  * )	;
void re_dbg_main( void *param  )
{
//#ifndef OPT_RRH_SIM
	T_DBGTHREAD_EXEC_INFO tdbg;

	memset(&tdbg,0,sizeof(tdbg));

	tdbg.uiProcID = D_RRH_PROCID_RE;
	tdbg.uiProcQID = D_RRH_PROCQUE_RE;
	tdbg.uiMainThradQID= CMD_TSKID_INI;
	tdbg.uiDbgThreadID = D_RRH_THDID_RE_DBG;
	tdbg.uiDbgThreadQID = CMD_TSKID_DBG;
	tdbg.cpDbgThreadName = (char*)param;
	f_dbg_main(&tdbg);
//#endif
}
/* @} */
