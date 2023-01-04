/*!
 * @skip  $ld:$
 * @file  f_fmng_main.c
 * @brief the entrance of pf_fmg thread
 * @date 2013/11/14 FFCS)linb create
 * @date 2015/08/14 TDIPS)sasaki update 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
#include "f_fmng_inc.h"

/*!
* @brief fmng management main function
* @note Judge the input message and dispatch it to the corresponding handle function.\n
*
*
*
* @return None
* @date 2013/11/14 FFCS)linb create
* @date 2015/08/14 TDIPS)sasaki update RE-FLASH制御用イベント解析関数コールを追加
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/
VOID f_fmng_main()
{

	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	CHAR cRcvMsg[1024];    /* receievd message buffer*/
	INT		rcvsize;
	INT		errcd;

	/* スレッド名表示		*/
	prctl(PR_SET_NAME, "pf_fmng", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_FRMG,			/* ThreadID			*/
								    D_SYS_THDQID_PF_FRMG,				    /* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	f_fmng_init( ); 				   

	/* Change thread state into running */
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		memset(cRcvMsg,0,sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		f_com_msgQReceive( D_SYS_THDQID_PF_FRMG, 
						   sizeof(cRcvMsg),
						   cRcvMsg, &rcvsize,&errcd);

		/* save running history */
		f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_RCV, (UCHAR *)cRcvMsg);

		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
		/* analyze and dispatch message   */
		f_fmng_anlz( cRcvMsg );
		f_fmng_anlz_frwre( cRcvMsg );
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
	}

	return;
}

/*! @} */
