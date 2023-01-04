/*!
 * @skip		$ld:$
 * @file		f_main_iniWait.c
 * @brief		pf_main 初期化完了通知受信処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_iniWait
 * @note		初期化完了通知受信処理.
 *					-# 初期化処理状態管理テーブル内の各スレッド処理状態を完了(1)に設定.
 *					-# 対象となる全スレッドの初期化完了状態チェック.
 *						- 全対象スレッドからの通知受信完了.
 *							-# INITプロセスに初期化完了通知を送信.
 *					- 全対象スレッドからの通知受信完了の場合.
 *						-# シグナル待ち.
 *					- 全対象スレッドからの通知受信未完了の場合.
 *						-# 異常終了を返却し、処理終了.
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date		2015/08/27 ALPHA) miyazaki modify for TDD_RRE_ZYNQ
 */
UINT f_main_iniWait(VOID* rcvMsg_p)
{
	unsigned long int				threadId = 0;	/*	スレッドID格納変数				*/
	UINT							srcThdNo = 0;
	T_SYS_INIT_COMP_NTC*			msgPtr;
	T_SYS_ALL_TASK_INIT_COMP_NTC	*resp = NULL;
	UINT							rcvSize;
	INT								endcd;
	T_MAIN_CRT_EVENT_CTBL			*f_mainr_pThdCreMng ;
	T_RRH_DU_PROCESS_INFO*			tDuProcInfo;

	INT								rtn_bpf;
	INT								errcd;
	char							proc_name[4];
	UINT							i;

	/* 受信MSGをTask Init completion notice型に変換		*/
	msgPtr = (T_SYS_INIT_COMP_NTC*)rcvMsg_p;

	/* 送信元スレッドID取得								*/
	srcThdNo = msgPtr->head.uiSrcTQueueID;

	if(f_mainw_process_start_mode == 1) {
		sprintf( proc_name, "%s", "PTP" );
		f_mainr_pThdCreMng = &f_mainr_thdCreMng_ptp[(srcThdNo - 1)];
	} else {
		f_mainr_pThdCreMng = &f_mainr_thdCreMng[(srcThdNo - 1)];
		sprintf( proc_name, "%s", "PF" );
	}

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_main_iniWait(%s)] ENTER", proc_name);

	if(srcThdNo>=D_SYS_THDQNO_PF_MAX)
	{
		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_WARNING, "uiSrcTQueueID NG=0x%08x", srcThdNo);
		return D_SYS_OK;
	}

	f_mainw_threadinitcmpbit &= (UINT)~((UINT)(D_RRH_ON) << (srcThdNo - 1));
	cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, 
						   "%s thread cmp recv id:0x%08x off:0x%08x rst:0x%08x ",
						   f_mainr_pThdCreMng->thdName, 
						   srcThdNo, 
						   (INT)((UINT)(D_RRH_ON) << ( srcThdNo - 1)), 
						   f_mainw_threadinitcmpbit);

	if(f_mainw_threadinitcmpbit != D_RRH_OFF){
		return D_SYS_OK;
	}

	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
			E_RRH_SHMID_APL_DU_PROCESS_INFO,
			(VOID **)&tDuProcInfo,
			&errcd );

	if( rtn_bpf == BPF_RU_IPCM_OK ){
		if(f_mainw_process_start_mode == 1){
			tDuProcInfo->pState[D_RRH_PROCID_PTP] = D_RRH_OFF;
			tDuProcInfo->pid[D_RRH_PROCID_PTP] = getpid();
		}else{
			/* 最後のmessageのfree */
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( rcvMsg_p );
		}
		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "ALL %s thread cmp", proc_name);

	}else{
		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ERROR, "E_RRH_SHMID_APL_DU_PROCESS_INFO ERROR");
		return D_SYS_NG;
	}

	/************************************************************************************************************/
	/* INITプロセスに初期化完了通知を送信																		*/
	/************************************************************************************************************/
	/*	初期化完了	*/
	endcd = BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_OK);

	/* エラーログ取得				*/
	if(endcd != BPF_RM_SVRM_COMPLETE)
	{
		/* assert log				*/
		switch(endcd)
		{
			case BPF_RM_SVRM_ERR_EXEC:	/* 内部処理異常									*/
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCMSGQ_RECEIVE inter process error. endcd=%08x", endcd);
				break;
			case BPF_RM_SVRM_ERR_SYS:	/* システムコール失敗									*/
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCMSGQ_RECEIVE system call error. endcd=%08x", endcd);
				break;
			default:
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCMSGQ_RECEIVE error. endcd=%08x", endcd);
		}

		cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_iniWait(%s)] RETURN", proc_name);
		return D_SYS_NG;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "1st recv All task init wait(%d)",f_mainw_process_start_mode);

	/********************************************************************************************/
	/* INITプロセスからのAll task init complete notice待ち										*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_MAIN,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&resp,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/

		cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, 
							   "%s Process MSG RECV Event No=0x%08x", 
							   proc_name, 
							   resp->head.uiEventNo );

		/* "Init completion notice" 受信時 */
		if(resp->head.uiEventNo == D_SYS_MSGID_ALLTSKININTC)
		{
			/* メモリの解放	*/
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( resp );
			
			/* メッセージ待機を終了 */
			break;
		}
		/* メモリの解放	*/
		(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( resp );
	}

	/********************************************************************************************/
	/* pf_cca Thread / pf_rfa Thread 起動														*/
	/********************************************************************************************/
	if(f_mainw_process_start_mode != 1){
		/***** Thread起動	*****/
		f_mainw_threadinitcmpbit = D_RRH_OFF;
		for( i = 0, f_mainr_pThdCreMng = &f_mainr_thdCreMng[0]; i < D_MAIN_THDID_MAX; i++, f_mainr_pThdCreMng++ ) {

			/* シリアル起動以外のスレッドを生成・起動	*/
			if( f_mainr_pThdCreMng->thdCreateFlag == D_MAIN_CREATE_PTN_OTHER) {

				/*	スレッドを生成	*/
				endcd = BPF_RU_ITCM_THREAD_CREATE(
								(unsigned long int *)&threadId,				/* Thread id					*/
								(INT)f_mainr_pThdCreMng->SchPolicy,			/* Scheduling policy			*/
								(INT)f_mainr_pThdCreMng->thdPriority,		/* Thread priority				*/
								(INT)f_mainr_pThdCreMng->thdStackSize,		/* Thread stack size			*/
								(INT*)(FUNCPTR)f_mainr_pThdCreMng->thdFunc,	/* IAスレッドメイン関数ポインタ	*/
								(VOID*)f_mainr_pThdCreMng->option,			/* オプション					*/
								&errcd );									/* Error code					*/

				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "%s Thread create start", f_mainr_pThdCreMng->thdName);

				/*	ERRORの場合	*/
				if(endcd != BPF_RU_ITCM_OK)
				{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE %s NG %d threadid=%ld thdPriority=%d thdStackSize=%d",
								f_mainr_pThdCreMng->thdName, 
								endcd, 
								threadId, 
								(INT)f_mainr_pThdCreMng->thdPriority, 
								(INT)f_mainr_pThdCreMng->thdStackSize);
				}
				/* スレッドID保持	*/
				f_mainw_thdId.thdId[i]	= threadId;
				f_mainw_threadinitcmpbit |= (UINT)((UINT)(D_RRH_ON) << i);

				cmn_com_assert_th_get( f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, 
									   "%s Thread create comp  id:0x%lx pri:%d stack:0x%x set:0x%08x exp:0x%08x",
									   f_mainr_pThdCreMng->thdName,
									   (ULONG)threadId,
									   (INT)f_mainr_pThdCreMng->thdPriority,
									   (INT)f_mainr_pThdCreMng->thdStackSize,
									   (INT)( (UINT)((UINT)(D_RRH_ON) << i)),
									   f_mainw_threadinitcmpbit);
			}
		}

		/***** Thread起動完了待ち	*****/
		while(1) {
			rtn_bpf = BPF_RU_IPCM_MSGQ_RECV( D_SYS_THDQID_PF_MAIN,	/* 受信MSGキューID			*/
											 0,						/* 受信待ち時間 (無限)		*/
											 (VOID**)&msgPtr,		/* メッセージ受信バッファ	*/
											 &rcvSize );			/* 受信Messageサイズ		*/

			if( rtn_bpf != BPF_RU_IPCM_OK ) {

				if( errno != ENOMSG ) {
					/* TimeOut以外のNG	*/
					cmn_com_assert_th_get(f_mainw_assert_p, 
										  D_RRH_LOG_AST_LV_ERROR, 
										  "BPF_RU_IPCM_MSGQ_RECV ERROR, rtn=%08x, errcd=%08x",
										  rtn_bpf, errno);
					return D_SYS_NG;
				} else {
					/* TimeOut	*/
					continue;
				}
			}
			/* 送信元スレッドID取得	*/
			srcThdNo = msgPtr->head.uiSrcTQueueID;
			switch(srcThdNo){
				case D_SYS_THDQID_PF_RFA:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_rfa init resp recv");
					break;
				case D_SYS_THDQID_PF_CCA:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_cca init resp recv");
					break;
				default:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "unknown(0x%x 0x%x) recv",srcThdNo,msgPtr->head.uiEventNo);
					break;
			}

			f_mainw_threadinitcmpbit &= (UINT)~((UINT)(D_RRH_ON) << (srcThdNo - 1));
			cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, 
								   "%s thread cmp recv id:0x%08x off:0x%08x rst:0x%08x ",
								   f_mainr_thdCreMng[(srcThdNo - 1)].thdName, 
								   srcThdNo, 
								   (INT)((UINT)(D_RRH_ON) << ( srcThdNo - 1)), 
								   f_mainw_threadinitcmpbit);

			if(f_mainw_threadinitcmpbit != D_RRH_OFF) {
				continue;
			}

			if( rtn_bpf == BPF_RU_IPCM_OK ){

				tDuProcInfo->pState[D_RRH_PROCID_PF] = D_RRH_OFF;
				tDuProcInfo->pid[D_RRH_PROCID_PF] = getpid();
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "ALL PF thread cmp");

			} else {
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ERROR, "E_RRH_SHMID_APL_DU_PROCESS_INFO ERROR");
				return D_SYS_NG;
			}
			/* messageのfree */
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( msgPtr );
			if(f_mainw_threadinitcmpbit == D_RRH_OFF){
				f_comw_rf_setup_flg = D_RRH_ON;
				break;
			}
		}

		f_com_dumpMsgEventSend("M-Plane decoding dump initial state was ENABLED");
		f_com_dumpMsgAisgSend("AISG dump initial state was ENABLED");

		if(f_cmw_tra_inv_tbl->cmx_ptp_device_access_flg != D_RRH_ON){
			/********************************************************************************************/
			/* INITプロセスからのAll task init complete notice待ち(PTPプロセス完了待ち)					*/
			/********************************************************************************************/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "2nd recv All task init wait(%d)",f_mainw_process_start_mode);

			resp = NULL;
			rtn_bpf = BPF_RU_IPCM_MSGQ_RECV( D_SYS_THDQID_PF_MAIN,	/* 受信MSGキューID			*/
											 0,						/* 受信待ち時間 (無限)		*/
											 (VOID**)&resp,		/* メッセージ受信バッファ	*/
											 &rcvSize );			/* 受信Messageサイズ		*/
			if( rtn_bpf != BPF_RU_IPCM_OK ) {
				if( errno != ENOMSG ) {
					/* TimeOut以外のNG	*/
					cmn_com_assert_th_get(f_mainw_assert_p, 
										  D_RRH_LOG_AST_LV_ERROR, 
										  "BPF_RU_IPCM_MSGQ_RECV ERROR, rtn=%08x, errcd=%08x",
										  rtn_bpf, errno);
				}
				else{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Not recv All task init complete %d", errno);
				}
			}

			if(resp != NULL){
				/* メモリの解放	*/
				(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( resp );
			}

			/************************************************************************************************************/
			/* disableからenableに変更しFaultが無ければLED1はGreen Blinkする											*/
			/************************************************************************************************************/
			f_com_SVCtl(E_DU_SVCTL_OPE_OPE,D_DU_SVCTL_CHG,D_DU_SVCTL_ON_OPE_ENABLE);
		
			/************************************************************************************************************/
			/* M-Plane SW起動完了待ち (PF)(自律送信モード時はM-plane起動しない)											*/
			/************************************************************************************************************/
			f_main_mp_comp_wait();

		}else{
			/************************************************************************************************************/
			/* disableからenableに変更しFaultが無ければLED1はGreen Blinkする											*/
			/************************************************************************************************************/
			f_com_SVCtl(E_DU_SVCTL_OPE_OPE,D_DU_SVCTL_CHG,D_DU_SVCTL_ON_OPE_ENABLE);

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Tx Standalone mode. 2nd recv All task init wait skip(%d)",
							f_cmw_tra_inv_tbl->cmx_ptp_device_access_flg );
		}

		/************************************************************************************************************/
		/* アプリ初期化完了ログ取得																					*/
		/************************************************************************************************************/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "DU_PF Init Completion. %d" , D_RRH_ZERO);

		/************************************************************************************************************/
		/* シグナル待機																								*/
		/************************************************************************************************************/
		f_main_sigWait();
	}

	/****************************************************************************************************************/
	/* アプリ初期化完了ログ取得																						*/
	/****************************************************************************************************************/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "DU_PTP Init Completion. %d" , D_RRH_ZERO);

	/****************************************************************************************************************/
	/* M-Plane SW起動完了待ち (PTP)																					*/
	/****************************************************************************************************************/
	f_main_mp_comp_wait();

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_iniWait(%s)] RETURN", proc_name);

	return D_SYS_OK;
}

/* この定義は暫定です。必要に応じて各スレッド内のテーブルを追加してください */
static bpf_rm_svrm_sigget_tbl_t log_siggettble[] = 
{
	{"f_mainw_inv_mng"		 ,sizeof(f_mainw_inv_mng)		,&f_mainw_inv_mng 		,0}
};

/*!
 * @brief f_main_sigHandler
 * @note  関数処理内容.
 * @param		signal   [in] signal number
 * @para		siginfo  [in] signal information
 * @param		u_contxt [in] user context
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/20 alpha)中村
 */
INT f_main_sigHandler(INT signal, siginfo_t *siginfo, VOID *u_contxt , int fd)
{
	/* 注意:											 */
	/* 本関数はシグナルハンドラ上で動作する				 */
	/* 本関数内では非同期シグナル安全な関数のみ使用可能	 */
	if(fd > 0)
	{
		BPF_RM_SVRM_SIGNAL_TBLSAVE(fd,
							   sizeof(log_siggettble)/sizeof(log_siggettble[0]),
							   (char*)log_siggettble
							   );
	}
	return(sizeof(log_siggettble)/sizeof(log_siggettble[0]));
}

/*!
 * @brief f_main_mp_comp_wait
 * @note  M-Plane SW起動完了通知を待ち受けする。
 * @return		なし。
 * @warning		N/A
 * @FeatureID	5GDU-002-001
 * @date		2018/11/23 FJT)吉田
 */
VOID f_main_mp_comp_wait( VOID ) {

	T_SYS_MPLANE_SW_INIT_COMP_NTC	*resp = NULL;
	UINT							rcvSize;
	unsigned long int				threadId	= 0;		/*	スレッドID格納変数				*/
	INT								iErrorCode	= 0;		/*	詳細エラーコード				*/
	INT								endcd;

	const T_CMU_CREATE_THREAD_MP	*cmusRecvThdCreMng_mp;

	char							proc_name[4];			/* Process名称						*/
	T_RRH_TRAINVTBL*				inv_tbl_p = NULL;


	/* PTPの場合 f_cmw_tra_inv_tblが未設定のため、共有メモリアドレスを取得 */	
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY,(VOID**)&inv_tbl_p,&endcd);	
	if((inv_tbl_p != NULL) && (inv_tbl_p->cmx_mp_conn_flg == D_RRH_ON)){
		cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_WARNING, "M-Plane Skip. M-plane mode is Disable(%d)!!",f_mainw_process_start_mode);
		return;
	}

	if(f_mainw_process_start_mode == 1) {
		sprintf( proc_name, "%s", "PTP" );
		cmusRecvThdCreMng_mp = &f_mainr_cmusRecvThdCreMng_mp_ptp;
	} else {
		sprintf( proc_name, "%s", "PF" );
		cmusRecvThdCreMng_mp = &f_mainr_cmusRecvThdCreMng_mp_pf;
	}

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_main_mp_comp_wait(%s)] ENTER", proc_name );
	/********************************************************************************************/
	/* INITプロセスからのM-Plane SW起動完了通知待ち												*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_MAIN,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&resp,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/

		cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, 
							   "%s Process MSG RECV Event No=0x%08x", 
							   proc_name, resp->head.uiEventNo );

		/* "D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC" 受信時 */
		if(resp->head.uiEventNo == D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC)
		{
			/* メモリの解放	*/
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( resp );

			cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, 
								   "%s Process D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC MSG RECV!!!", proc_name );


			endcd = BPF_RU_ITCM_THREAD_CREATE( &threadId,					/* Thread id					*/
											   BPF_RU_ITCM_SCHED_FIFO,		/* Scheduling policy			*/
											   D_RRH_PRIORITY_MPRECV,		/* Thread priority				*/
											   D_RRH_STACKSIZE_DEFAULT,		/* Thread stack size			*/
											   f_mpproxy_recv,				/* IAスレッドメイン関数ポインタ	*/
											   (VOID*)cmusRecvThdCreMng_mp,	/* オプション					*/
											   &iErrorCode );				/* Error code					*/

			cmn_com_assert_th_get( f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, 
								   "MP Proxy create(%d)!!",endcd);

			/* メッセージ待機を終了 */
			break;
		}
		/* メモリの解放	*/
		(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( resp );
	}
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_mp_comp_wait(%s)] RETURN", proc_name );
	return;
}

/*!
 * @brief f_main_sigWait
 * @note		シグナル受信待機処理.
 *					-# INITプロセスからのシグナル要求の待機中処理を行う。
 *					-# PFプロセスが受信するシグナルは下記
 *						-# SIGCHLD
 *						-# SIGUSR1:障害ログ出力要求(通常)
 *						-# SIGUSR1:障害ログ出力要求(ヘルスチェック断)
 *						-# SIGUSR1:障害ログ出力要求(運用ログ出力完了通知)
 *					-# SIGCHLD受信時、INITプロセスへSIGCHLDを通知する。
 *					-# 運用ログ完了通知受信時、LOGスレッドへ障害ログ出力要求を通知する。
 *					-# 障害ログ出力要求受信時、LOGスレッドへ障害ログ完了を通知する。
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/20 alpha)中村
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 */
VOID f_main_sigWait()
{
	siginfo_t					sigInfo;

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_main_sigWait] ENTER" );

	/************************************************************************************************************/
	/* PFプロセス向けのシグナル受信を待機																		*/
	/************************************************************************************************************/
	for(;;)
	{
		/* sigwaitによる受信処理 */
		if ( BPF_RM_SVRM_SIGNAL_WAIT(&f_mainw_sigSetVal,
								&sigInfo.si_pid,
								&sigInfo.si_signo,
								&sigInfo.si_status) == BPF_RM_SVRM_ERR_EXEC )
		{
			continue;
		}
		
		switch (sigInfo.si_signo) 
		{
		/* プロセス終了時 */
		case SIGCHLD:
		/* 自プロセスが死亡した場合はSignal Handlerで捕捉されるはずなのでここでは何もしない	*/
			break;
				
		/* ソフトウェア障害系シグナル */
		case SIGUSR1:
			/* オプションを判別する */
			switch(sigInfo.si_status)
			{
			case D_RRH_SIGUSR_OPT_TROUBLE_REQ:		/*	障害ログ出力要求(ALM発生)			*/
			case D_RRH_SIGUSR_OPT_TROUBLE_REQ_RST:	/*	障害ログ出力要求(ALM発生_RESET有)	*/
			case D_RRH_SIGUSR_OPT_HLTCHK_REQ:		/*	障害ログ出力要求(ヘルスチェック断)	*/
			case D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF:	/*	運用ログ出力完了通知	*/
			case D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF_3G:	/*	運用ログ出力完了通知(3G)	*/
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "LOG OUT rcv status:0x%08x Pid:%d", (UINT)sigInfo.si_status ,sigInfo.si_pid);

				/* pf_logに障害ログ出力要求メッセージを投げる	*/
				f_main_troubelelog_send( sigInfo.si_status );
				
				break;
				
			case D_RRH_SIGUSR_OPT_TROUBLE_FLASH0_REQ:	/* 障害ログFlash書き込み要求0	*/
			case D_RRH_SIGUSR_OPT_TROUBLE_FLASH1_REQ:	/* 障害ログFlash書き込み要求1	*/
			case D_RRH_SIGUSR_OPT_TROUBLE_FLASH2_REQ:	/* 障害ログFlash書き込み要求2	*/
			case D_RRH_SIGUSR_OPT_HEALTH_FLASH_REQ:		/* 障害ログFlash書き込み要求(ヘルスチェック)	*/

				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "FLASH REQ rcv status:0x%08x Pid:%d", (UINT)sigInfo.si_status,sigInfo.si_pid);
				
				/* pf_frwに障害ログFlash書き込み要求メッセージを投げる	*/
				f_main_flash_write_req_send( sigInfo.si_status );
				break;
				
			default:
				cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ENTER, "UNKNOWN status rcv status:0x%08x Pid:%d", (UINT)sigInfo.si_status,sigInfo.si_pid);
				break;
			}
			break;

		default:
			cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_WARNING, "UNKNOWN rcv signal:%d. status:0x%08x Pid:%d", sigInfo.si_signo,(UINT)sigInfo.si_status,sigInfo.si_pid);
			break;
		}
	}
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_sigWait] RETURN" );

	return;
}

/*!
 * @brief	f_main_troubelelog_send
 * @note  関数処理内容.
 *				-# logスレッドに障害ログ/運用ログの要求メッセージを送信する
 *				-# 送信メッセージのバッファを取得する。
 *				-# 引数から要求するログの種別ごとにパラメータを設定する。
 *				-# 送信メッセージを作成し、ログを送信する。
 * @param		log_status  [in] 要求ログ種別
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/20 alpha)中村
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 */
VOID f_main_troubelelog_send(INT log_status)
{
#if 0 // TODO 
	T_SYS_TROUBLE_LOG_OUTPUT_REQ*	sndTroublelogMsg;	/* 障害ログ出力要求送信メッセージ		*/
	T_SYS_RUN_LOG_OUTPUT_NTC*		sndRunlogMsg;		/* 運用ログ出力完了通知送信メッセージ	*/
	UINT							uiSignalkind[2] = {D_SYS_S3G,D_SYS_3G};

	INT								errcd = 0;
	INT								bpf_ret;
#endif
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_main_troubelelog_send] ENTER" );

#if 0 // TODO 
	/* バッファ取得 */
	if( (log_status == D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF    ) ||
	    (log_status == D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF_3G ) )
	{
		/* 運用ログ出力完了通知送信メッセージ	*/
		bpf_ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof( T_SYS_RUN_LOG_OUTPUT_NTC ),			/* MessageSize						*/
											(VOID **)&sndRunlogMsg,						/* MessageBuffer					*/
											&errcd );									/* ErrorCode						*/
		
		if(bpf_ret != BPF_RU_IPCM_OK)
		{
			cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ERROR, " BPF_RU_IPCM_PROCMSG_ADDRGET fault. bpf_ret= %08x", bpf_ret);
			cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_troubelelog_send] RETURN" );

			return;
		}
		/* ヘッダ部設定	*/
		sndRunlogMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 返信先PQID						*/
		sndRunlogMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_LOG;					/* 送信先TQID						*/
		sndRunlogMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元PQID						*/
		sndRunlogMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_MAIN;					/* 送信元TQID						*/
		sndRunlogMsg->head.uiEventNo		= D_SYS_MSGID_RUNLOG_OUT_NTC;			/* メッセージID						*/
		
		sndRunlogMsg->head.uiSignalkind		= uiSignalkind[log_status & 0x00000001];/* Signal種別						*/

		BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_LOG,								/* 送信MSGキューID					*/
								(VOID *)sndRunlogMsg );	
		
		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "SIGUSR1(OPT_RUNLOG_FIN_PF) send for Log. log_status = %08x", (UINT)log_status);
	}
	else
	{
		/* 障害ログ出力要求送信メッセージ		*/
		bpf_ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof( T_SYS_TROUBLE_LOG_OUTPUT_REQ ),		/* MessageSize						*/
											(VOID **)&sndTroublelogMsg,					/* MessageBuffer					*/
											&errcd );									/* ErrorCode						*/
		
		if(bpf_ret != BPF_RU_IPCM_OK)
		{
			cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ERROR, " BPF_RU_IPCM_PROCMSG_ADDRGET fault. bpf_ret= %08x", bpf_ret);
			cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_troubelelog_send] RETURN" );

			return;
		}
		/* ヘッダ部設定	*/
		sndTroublelogMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 返信先PQID						*/
		sndTroublelogMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_LOG;				/* 送信先TQID						*/
		sndTroublelogMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元PQID						*/
		sndTroublelogMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_MAIN;				/* 送信元TQID						*/
		sndTroublelogMsg->head.uiEventNo		= D_SYS_MSGID_TROUBLELOG_OUT_REQ;	/* メッセージID						*/
		
		/* 個別部設定	*/
		if(log_status == D_RRH_SIGUSR_OPT_TROUBLE_REQ)
		{
			sndTroublelogMsg->trouble_kind = E_SYS_TROUBLE_KIND_NORMAL;
		}
		else if(log_status == D_RRH_SIGUSR_OPT_TROUBLE_REQ_RST)
		{
			sndTroublelogMsg->trouble_kind = E_SYS_TROUBLE_KIND_RST;
		}
		else
		{
			sndTroublelogMsg->trouble_kind = E_SYS_TROUBLE_KIND_HLTCHK;
		}
		
		BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_LOG,								/* 送信MSGキューID					*/
								(VOID *)sndTroublelogMsg );	

		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "SIGUSR1(TROUBLE_REQ) send for Log. log_status = %08x", (UINT)log_status);
	}
#endif
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_troubelelog_send] RETURN" );

	return;
}

/*!
 * @brief	f_main_flash_write_req_send
 * @note  関数処理内容.
 *				-# frwスレッドに障害ログ要求メッセージを送信する
 *				-# 送信メッセージのバッファを取得する。
 *				-# 引数から要求するログの種別ごとにパラメータを設定する。
 *				-# 送信メッセージを作成し、ログを送信する。
 * @param		log_status  [in] ログ種別
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2014/01/30 alpha)横山
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 */
VOID f_main_flash_write_req_send(INT log_status)
{
	T_SYS_TROUBLE_LOG_WRITE_REQ*	sndTroubleWriteMsg;		/* 障害ログ書き込み要求送信メッセージ	*/

	INT						errcd = 0;
	INT						bpf_ret;
	INT						log_status_tmp;

	typedef struct{
		UINT		kind;
		UINT		face;
		UINT		offset;
	}T_MAIN_SNDTROUBLE;
	
	T_MAIN_SNDTROUBLE f_inir_sndTroubleW[4] =
	{
 		/* logtype  ,  face   ,  addr */
		{ D_RRH_LOGTYPE_TROUBLE,		D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE0 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE0 */
		{ D_RRH_LOGTYPE_TROUBLE,		D_RRH_QSPI_FLASH_FACE0,	D_RRH_QSPI_FLASH_OFFSET_TROUBLE1 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE1 */
		{ D_RRH_LOGTYPE_TROUBLE,		D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE2 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE2 */
		{ D_RRH_LOGTYPE_HEALTH_CHCEK,	D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_HEALTH   }         /* D_RRH_QSPI_FLASH_OFFSET_HEALTH */
	};

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_main_flash_write_req_send] ENTER" );

	/* 運用ログ出力完了通知送信メッセージ	*/
	bpf_ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof( T_SYS_TROUBLE_LOG_WRITE_REQ ),		/* MessageSize						*/
										(VOID **)&sndTroubleWriteMsg,				/* MessageBuffer					*/
										&errcd );									/* ErrorCode						*/
		
	if(bpf_ret != BPF_RU_IPCM_OK)
	{
		cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_ERROR, " BPF_RU_IPCM_PROCMSG_ADDRGET fault. bpf_ret= %08x", bpf_ret);
		cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_flash_write_req_send] RETURN" );
		return;
	}
	/* ヘッダ部設定	*/
	sndTroubleWriteMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 返信先PQID						*/
	sndTroubleWriteMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先TQID						*/
	sndTroubleWriteMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元PQID						*/
	sndTroubleWriteMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_MAIN;					/* 送信元TQID						*/
	sndTroubleWriteMsg->head.uiEventNo		= D_SYS_MSGID_TROUBLELOG_WRITE_REQ;		/* メッセージID						*/

	/* 下位1byteを抽出してログ種別を取得する。*/
	log_status_tmp = log_status & 0x000000ff;
	
	sndTroubleWriteMsg->kind	= f_inir_sndTroubleW[log_status_tmp].kind;
	sndTroubleWriteMsg->face	= f_inir_sndTroubleW[log_status_tmp].face;
	sndTroubleWriteMsg->offset	= f_inir_sndTroubleW[log_status_tmp].offset;
	
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_NMA,									/* 送信MSGキューID					*/
							(VOID *)sndTroubleWriteMsg );	

	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_main_flash_write_req_send] RETURN" );

	return;
}


/* @} */
