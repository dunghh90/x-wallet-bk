/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_aih_init.c
 *  @brief  5GDU-LLS ALD(AISG) interface main function
 *  @date   2019/07/31 FJT)Takahashi create
 *  @date   2019/09/13 FJT)Takahashi modify for IT2
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/


#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/* @{ */
/**
* @brief 5GDU-LLS ALD init function
* @note  ALD init function.\n
* @param None
* @return None
* @date 2019/07/31 FJT)Takahashi create
* @date 2019/09/13 FJT)Takahashi modify for IT2
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_initCtl(UINT logSuppressFlg)
{
	f_com_bspAISGStop();	/* 受信停止 */

	f_aih_aldCtrlLogp->rcvstartFlg = E_AIH_FLG_RCVOFF;

	if(logSuppressFlg == D_RRH_OFF){
		f_aih_aldCtrlLogp->logSuppressFlg = logSuppressFlg;
	}else{
		f_aih_aldCtrlLogp->logSuppressFlg  = D_RRH_OFF;
		f_aih_aldCtrlLogp->logSuppressFlg |= logSuppressFlg;
	}
	
	if(f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON){
		memset(&(f_aih_aldCtrlLogp->scan),0,sizeof(f_aih_aldCtrlLogp->scan));
	}

	return;
}
/* @} */

/* @{ */
/**
* @brief 5GDU-LLS ALD init function
* @note  ALD init function.\n
* @param None
* @return None
* @date 2019/07/31 FJT)Takahashi create
* @date 2019/09/13 FJT)Takahashi modify for IT2
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_init()
{
	UINT	uiRet = D_SYS_OK;
	INT		errcd;
	INT		rtn_bpf;
	T_SYS_INIT_COMP_NTC*	sndMsg;			/* 送信メッセージ(初期化完了通知)	*/

	/* ALD送受信履歴 Log Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_ALD_LOG_TBL,(VOID**)&f_aih_aldLogp,&errcd);
	/* ALD制御情報 Log Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_ALD_CTRL_TBL,(VOID**)&f_aih_aldCtrlInfoLogp,&errcd);
	if(uiRet != BPF_RU_IPCM_OK){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", uiRet, errcd);
		f_com_abort(D_SYS_THDID_PF_AIH, D_DU_ALMCD_TSK);
		return;
	}

	f_aih_aldCtrlLogp = (T_RRH_AIH_ALD_CTRL_INFO *)f_aih_aldCtrlInfoLogp; /* AIH内はINFO型pointer使用 */

	/* AISG電源はデフォルトOFFとする */
	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_AISG_ALDPWR, E_AIH_CTRL_ALDON);
	f_aih_aldCtrlLogp->powerState = E_AIH_CTRL_ALDOFF;	/* ALD電源制御をset */
	(VOID)BPF_HM_DEVC_GETTIME(&(f_aih_aldCtrlLogp->powerDate));  /* Set System time */

	f_aih_initCtl(D_RRH_OFF);
	f_aih_aldCtrlLogp->devScanCount = 0;
	f_aih_aldLogp->index = 0;              /* ALD LOG TABLEのindex clear */

	/******************************************/
	/* Call common function to start SCC      */
	/******************************************/
	uiRet = f_com_bspAISGInit(  (VOIDFUNCPTR)f_com_AISGRcvIntNtf,	/* rcv notice func	受信要求 */
								(VOIDFUNCPTR)f_com_AISGRcvDatNtf,	/* rcv end func		受信完了 */
								(VOIDFUNCPTR)f_com_AISGSndNtf );	/* send end func	送信完了 */
	if(uiRet != D_SYS_OK){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "f_com_bspAISGInit abnormal %d", uiRet);
		f_com_abort(D_SYS_THDID_PF_AIH, D_DU_ALMCD_TSK);
		return;
	}
	uiRet = f_com_bspAISGStart();
	if(uiRet != D_SYS_OK){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "f_com_bspAISGStart abnormal %d", uiRet);
		f_com_abort(D_SYS_THDID_PF_AIH, D_DU_ALMCD_TSK);
		return;
	}

	f_aih_thrdstate = E_AIH_STAT_IDLE;	/* Update task state to IDLE(complete initialize) state */

	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別					*/
											sizeof(T_SYS_INIT_COMP_NTC),		/* MessageSize					*/
											(VOID **)&sndMsg,					/* MessageBuffer				*/
											&errcd );							/* ErrorCode					*/
	
	if( rtn_bpf != BPF_RU_IPCM_OK  ){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET NG 0x%08x(0x%08x)", rtn_bpf, errcd );
		f_com_abort(D_SYS_THDID_PF_AIH, D_DU_ALMCD_TSK);
		return;
	}

	/* 送信MSG作成 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID(0x00000102)		*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID					*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID					*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID					*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_AIH;							/* 送信元TQID					*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長				*/
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,								/* 送信MSGキューID				*/
							(VOID *)sndMsg );	

	return ;
}

/* @} */

