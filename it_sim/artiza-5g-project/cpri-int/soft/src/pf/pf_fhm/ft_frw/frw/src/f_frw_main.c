/*!
 * @skip  $ld:$
 * @file  f_frw_main.c
 * @brief the entrance of pf_frw thread
 * @date 2013/11/14 FFCS)linb create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */
#include "f_frw_inc.h"

/* @{ */
/**
* @brief frw management main function
* @note
* 
* @return None
* @date 2013/11/14 FFCS)linb create
* @date 2015/08/21 TDIPS)sasaki コメント修正
* @warning    N/A
* @FeatureID  PF_File-001-001-001
* @Bug_No N/A
* @CR_No  N/A
* @TBD_No N/A
*
*/

VOID f_frw_main( VOID )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	CHAR cRcvMsg[1024];    /* receievd message buffer*/
	INT		rcvsize;
	INT		errcd;
	INT		ret;

	/* スレッド名表示		*/
	prctl(PR_SET_NAME, "pf_frw", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_FRQ,				/* ThreadID			*/
									D_SYS_THDQID_PF_FRQ,				    /* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	/* 使用CPUコア登録 */
	ret = BPF_RM_SVRM_CPUAFFINITY (D_SYS_THDID_PF_FRQ, 1, &errcd);
	if( D_RRH_OK != ret )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_frw CPU affinity rtn_bpf=%08x error=%08x", ret, errcd );
	}

	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	f_frw_init();

	/************************************************************/
	/* Load Information of Firmware Files						*/
	/************************************************************/
	ret = f_frw_load_fwinfo();
	if( ret != D_RRH_OK )
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Load FW Information error=%08x", ret );
	}

	/* Change thread state into running */
	f_frww_thrState = D_FRW_THRDSTA_RUN;

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	while( D_SYS_NUM1 )
	{
		memset(cRcvMsg, 0, sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		f_com_msgQReceive( D_SYS_THDQID_PF_FRQ, 
						   sizeof(cRcvMsg), 
						   cRcvMsg, &rcvsize,&errcd);

		/* save running history */
		f_frw_cm_RunHisSet(D_FRW_RUNHIS_FLG_RCV, (UCHAR *)cRcvMsg);

		cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
		/* analyze and dispatch message   */
		f_frw_anlz( cRcvMsg );
		cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_RETURN,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
	}
	
	return;
}
/* @} */

/* @} */
