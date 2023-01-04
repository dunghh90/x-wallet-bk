/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_Main.c
 *  @brief  RE supervision main function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2008/11/28 FFCS)Linlj (M-S3G-eNBPF-00857) debug function improvement 
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02864:[品質改善]ハードALM状変受信時の処理誤り
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2011/10/11 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/10/21 FJT)Tokunaga M-S3G-eNBPF-04071対応
 *  @date   2012/07/12 FFCS)niemsh modify for 1.5GVA
 *  @date   2012/07/19 FFCS)Xut modify for CeNB-F-083-028対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief RE supervision main function
* @note RE supervision main function.\n
* 
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2008/11/28 FFCS)Linlj (M-S3G-eNBPF-00857) debug function improvement
* @date 2009/11/17 QNET)Kabasima M-S3G-eNBPF-02864:[品質改善]ハードALM状変受信時の処理誤り
* @date 2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
* @date 2010/09/17 CMS)Takahashi modify for M-S3G-eNBPF-03763
* @date 2011/07/21 FJT)Tokunaga M-S3G-eNBPF-04059:[IT3]REリモート閉塞時の(3003)カード状態監視開始マクロメール通知について
* @date 2011/08/18 FJT)Tokunaga CR-00043-000 FAN ALM問題対応
* @date 2011/10/11 FFCS)Linlj  modify for 省電力対応
* @date 2012/01/12 FJT)Tokunaga modify for M-S3G-eNBPF-04092 CR-00054-000 15G-RREのPA OFF問題対応
* @date   2012/07/12 FFCS)niemsh modify for 1.5GVA
* @warning	N/A
* @FeatureID	PF-Svp-000-000-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_Main(  )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT	a_ret __attribute__((unused))		= D_SYS_OK;		/* 戻り値格納変数				*/
	CHAR cRcvMsg[1024];    /* receievd message buffer*/
	INT  	rcvmsgsz ;
	INT		errcd;
	
	prctl(PR_SET_NAME, "pf_rsv", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_RSV,				/* ThreadID			*/
									D_SYS_THDQID_PF_RSV,			/* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/* 初期化処理を実施 */
	f_rsv_InitReq();

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		memset(cRcvMsg,0,sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		a_ret = f_com_msgQReceive( D_SYS_THDQID_PF_RSV, 
						   sizeof(cRcvMsg), 
						   cRcvMsg, 
						   &rcvmsgsz,
						   &errcd );
		
		/* write running history		  */						   

		/* analyze and dispatch message   */
		if(((T_RRH_HEAD*)cRcvMsg)->uiEventNo != D_SYS_MSGID_TIMOUTNTC)
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
		f_rsv_anlz( cRcvMsg ); 
		if(((T_RRH_HEAD*)cRcvMsg)->uiEventNo != D_SYS_MSGID_TIMOUTNTC)
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
	}
	return ;
}

/* @} */

/* @} */
