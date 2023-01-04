/*!
 * @skip  $ld:$
 * @file  main.c
 * @brief 初期化プロセスメイン処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include "BPF_COM_CMD.h"

int f_ini_main_proc_alive_check( void )
{
	CHAR					str[10];
	FILE					*fp;
	UINT					uiFlg = 0;
	CHAR					*ret = NULL;

	memset(str, D_RRH_ZERO, sizeof(str));

	cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_INFO, "DU_pf Process alive check start");	/* pgr0539 */

	if((fp = popen("ps -ef | grep -v grep | grep DU_pf | grep -v defunct | wc ", "r")) == NULL){
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "popen error ");	/* pgr0539 */
		return 0;
	}
	ret = fgets(str, 10, fp);
	if(ret != NULL){
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "Alive num is %s ",ret);	/* pgr0539 */
		if(strcmp(str, "2\n") == 0){
			uiFlg = 1;
		}
	}
	pclose(fp);
	if(uiFlg == 0){
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "DU_pf Not alive");	/* pgr0539 */
		return 1;
	}
	cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "DU_pf Alive");	/* pgr0539 */
	return 0;
}


void f_ini_main_log_create(CHAR* name , INT procNo)
{
	char cmd[256];
	int ret;
	
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"date +\"%%Y/%%m/%%d %%H:%%M:%%S\" >> /var/log/core.%s.%d.info.log",name,procNo);
	ret = system(cmd);
	cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_INFO, "2nd log create command result is %d",ret);	/* pgr0539 */
	return;
}


/*!
 * @brief		main
 * @note		関数処理内容.
 *				-# 内部管理テーブルの初期化処理
 *				-# ミドルウェア初期化
 *				-# 各種ログの共有メモリ展開
 *				-# シグナルハンドラー登録
 *				-# プロセス起動処理
 *				-# 子プロセス終了監視
 *				-# シグナル監視待機
 *					-# シグナル種類の識別を行い、各処理を実行する。
 *					-# タスク状態がシグナルイベント実行中の場合、レスポンスシグナル以外は排他する。
 *					-# SIG_CHILDシグナルを受信時、障害ログを取得後に各プロセスをExitする。
 *					-# 割り込み通知受信時、ResetReasonを指定してリセットを行う。
 * @param  		argc		[in]	受信パラメータ数
 * @param  		*argv[]		[in]	受信パラメータ
 * @retval 		D_INI_OK		0:正常終了
 * @retval 		D_INI_NG		1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @date 2013/11/07 ALPHA)nakamura modify for ZYNQ
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/06/05 ALPHA) tomioka Create RRH-007-000 TDD対応
 */

#ifdef OPT_RRH_CT
INT f_ini_main(INT argc, CHAR *argv[])
#else
INT main(INT argc, CHAR *argv[])
#endif
{
	INT					rtn = D_INI_OK;
	INT					rtn_func = D_INI_OK;
	INT					bpf_rtn = BPF_RM_SVRM_COMPLETE;
	siginfo_t			sigInfo;
	UINT				count;
	UINT				delFlg;
	INT					index =0;
	INT					errcd;
	T_INIT_SIG_NTC		*msgP;
	UINT				eventNo;
	UINT				sigKind;
	CHAR*				name;
	INT					proccheck;
	T_INIT_SIG_INFO_TBL	tmpInfoTbl;

	if(argc >= 2){
		if((!strncmp(argv[1],"-v",2)) || (!strncmp(argv[1],"-V",2))){
			printf("%s was born on %s %s\n",argv[0],__DATE__,__TIME__);
		}
		return 0;
	}

	BPF_COM_LOG_DMESG("[PROCESS(DU_init)] START\n");
	/************************************************************/
	/* RAMテーブルの初期化処理									*/
	/************************************************************/
	f_ini_iniTbl();

	/************************************************************/
	/* ミドルウェア初期化										*/
	/************************************************************/
	rtn_func = f_ini_iniBpf(f_iniw_assert_main_p);
	if(D_INI_OK != rtn_func)
	{
		BPF_COM_LOG_DMESG("[PROCESS(DU_init)] NG COMPLETE BPF INITIALIZE NG(%d)\n",rtn_func);
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_INFO, "f_ini_iniBpf error rtn=0x%08x",rtn_func);
		return(D_INI_NG);
	}

	/************************************************************/
	/* アプリ起動開始ログ取得									*/
	/************************************************************/
	cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_INFO, "APL Start");

	/************************************************************/
	/* シグナルハンドラ登録										*/
	/************************************************************/
	bpf_rtn = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY((VOID*)&f_ini_sigHandler, &f_iniw_sigSetVal ,D_RRH_OFF);

	/* シグナルハンドラ失敗 */
	if(bpf_rtn !=BPF_RM_SVRM_COMPLETE)
	{
		BPF_COM_LOG_DMESG("[PROCESS(DU_init)] NG COMPLETE HANDLE Entry NG(%d)\n",bpf_rtn);
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "signal handle entry error %08x", bpf_rtn);

		tmpInfoTbl.uiResetFlg_define	=	D_INI_SIG_RESET_ON		;
		tmpInfoTbl.resetReason			=	D_RRH_RST_RSN_SFT_FAIL	;
		tmpInfoTbl.logType 				=	E_INI_LOGTYPE_TROUBLE	;
		tmpInfoTbl.trouble_logget		=	D_INI_SIG_RESET_ON	;
		f_ini_logget_alm_reset(&tmpInfoTbl, f_iniw_assert_main_p);

		return(D_INI_NG);
	}
	
	/************************************************************/
	/* boot更新処理												*/
	/************************************************************/
	f_ini_bootUpdate();


	/************************************************************/
	/* PTP config更新処理										*/
	/************************************************************/
	f_ini_ptpConfUpdate();

	
	/* 内部状態をシグナル待ち受け状態に更新 */
	f_iniw_tskstn = D_INI_TSK_STN_SIG_WATE;

	/* プロセスID管理テーブルを共有メモリから取得               */
	bpf_rtn = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&rtn_func
	);

	/************************************************************/
	/* プロセス起動処理											*/
	/************************************************************/
	rtn_func = f_ini_procCreate(argc, argv , f_iniw_assert_main_p);
	if(D_INI_OK != rtn_func)
	{
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "Process Create Error");

		BPF_COM_LOG_DMESG("[PROCESS(DU_init)] PF Process Create error(%d) ...\n",rtn_func);

		tmpInfoTbl.uiResetFlg_define	=	D_INI_SIG_RESET_ON		;
		tmpInfoTbl.resetReason			=	D_RRH_RST_RSN_SFT_FAIL	;
		tmpInfoTbl.logType 				=	E_INI_LOGTYPE_TROUBLE	;
		tmpInfoTbl.trouble_logget		=	D_INI_SIG_RESET_ON	;

		f_ini_logget_alm_reset(&tmpInfoTbl, f_iniw_assert_main_p);

		return rtn_func;
	}

	if((rtn = system("/opt/sh/cmd_iptables 1>/dev/null 2>/dev/null")) != 0 ){
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_WARNING, "iptables setting error(%d)",rtn);
	}
	/************************************************************/
	/* アプリ起動終了ログ取得									*/
	/************************************************************/
	BPF_COM_LOG_DMESG("ALL PROCESS INITIALIZE FINISHED\n");
	BPF_COM_CMD_SETUP();

	f_iniw_alreadyRESET = D_RRH_OFF;
	f_iniw_force_log_save = D_RRH_OFF;
	
	/************************************************************/
	/* シグナル受信待機											*/
	/************************************************************/
	for(;;)
	{
		memset(&sigInfo,0,sizeof(sigInfo));
		/* sigwaitによる受信処理 */
		if ( BPF_RM_SVRM_SIGNAL_WAIT(&f_iniw_sigSetVal,
									&sigInfo.si_pid,
									&sigInfo.si_signo,
									&sigInfo.si_status) == BPF_RM_SVRM_ERR_EXEC )
		{
			cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ENTER, "signal wait error. ");	/* BPF側でログとってるためログレベルはEnter */
			continue;
		}

		if( f_iniw_alreadyRESET == D_RRH_ON ){
			cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_WARNING,"already Reset ...drop pid:%08x signo:0x%08x status:0x%08x.",sigInfo.si_pid, sigInfo.si_signo,sigInfo.si_status);
			continue;
		}

		delFlg = D_INI_DEL_OFF;
		switch(sigInfo.si_signo)
		{
			case SIGCHLD:
				proccheck = 0;
				for( count = 0; count < procIdTbl->count ; count++ ){
					if( sigInfo.si_pid == procIdTbl->info[ count ].aplProcessID ){
						proccheck = f_ini_main_proc_alive_check();
						if(proccheck == 0){
							break;
						}
						delFlg = D_INI_DEL_ON_PROC_PF;
						index = count;
						/* SIGNAL情報をASSERT出力する	*/
						f_ini_logget_sigInfo( sigInfo.si_pid, sigInfo.si_signo, sigInfo.si_status , f_iniw_assert_main_p);
						BPF_COM_LOG_DMESG("!!! +++++ Oops DU_PF(%d.%d) PROCESS DEAD !!!! +(T<>T)+ \n",procIdTbl->info[ count ].realProcessID,sigInfo.si_status);

						(VOID)BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_CLR, D_SYS_SWDT_TIMER_SET_EXT );	/* WDTリセット延長 */
						break;
					}
				}
				if((count < procIdTbl->count) && (proccheck == 0)){
					break;
				}
				if(	delFlg == D_INI_DEL_OFF ){
					for( count = 0; count < procIdTbl->mpsw_count ; count++ ){
						if( sigInfo.si_pid == procIdTbl->mpsw_info[ count ].aplProcessID ){
							delFlg = D_INI_DEL_ON_PROC_MP;
							if(procIdTbl->mpsw_info[ count ].aplProcessID == D_RRH_PROCID_MPSW_NSA){
								cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR,"NSA(%d.%d) Dead !!!",procIdTbl->mpsw_info[ count ].realProcessID,sigInfo.si_status);
								BPF_COM_LOG_DMESG("!!! +++++ Oops NSA(%d.%d) PROCESS DEAD +(T<>T)+ !!! \n",procIdTbl->mpsw_info[ count ].realProcessID,sigInfo.si_status);

								name = "NSA";

							}else{
								cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR,"SAA(%d.%d) Dead !!!",procIdTbl->mpsw_info[ count ].realProcessID,sigInfo.si_status);
								BPF_COM_LOG_DMESG("!!! +++++ Oops SAA(%d.%d) PROCESS DEAD +(T<>T)+ !!!! \n",procIdTbl->mpsw_info[ count ].realProcessID,sigInfo.si_status);

								name = "SAA";

							}
							procIdTbl->mpsw_info[ count ].aplProcessID = 0;
							f_ini_main_log_create(name,procIdTbl->mpsw_info[ count ].realProcessID);
							break;
						}
					}
				}
				break;
			default:
				delFlg = D_INI_DEL_ON;
				break;
		}
		if( delFlg == D_INI_DEL_OFF ){
			continue;
		}

		if(sigInfo.si_signo == SIGCHLD){
			if(delFlg == D_INI_DEL_ON_PROC_PF){
				/* 各処理でsystem()関数を使用しており、それらが実行->終了するとSIGCHLDが飛んでくる	*/
				/* それらは無視する必要がある														*/
				/* また、死亡したプロセスはテーブルから削除する必要がある							*/
				for( count = index; count < procIdTbl->count; count++ ){
					procIdTbl->info[ count ].aplProcessID  = procIdTbl->info[ count+1 ].aplProcessID;
					procIdTbl->info[ count ].realProcessID = procIdTbl->info[ count+1 ].realProcessID;
					procIdTbl->info[ count ].PQID = procIdTbl->info[ count+1 ].PQID;
				}
				procIdTbl->count--;
			}
		}
		
		switch (sigInfo.si_signo)
		{
			case SIGCHLD:
			case SIGUSR1:
				/* SIGUSR1は障害/RESET/HealthCheck断/がある	*/
				/* SIGCHLDは障害に分類する										*/

				f_ini_logget_sigCheck( &sigInfo, &sigKind, &eventNo , f_iniw_assert_main_p);
				memset(&tmpInfoTbl,0,sizeof(tmpInfoTbl));

				if((sigKind == E_INI_LOGTYPE_TROUBLE ) || (sigKind == E_INI_LOGTYPE_HLTCHK)){

					if(( rtn = f_ini_logget_alm_chkRST( &sigInfo , f_iniw_assert_main_p , &tmpInfoTbl)) == D_INI_SIG_RESET_OFF )
					{
						BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
														sizeof( T_INIT_SIG_NTC ),				/* MessageSize						*/
														(VOID **)&msgP,							/* MessageBuffer					*/
														&errcd );								/* ErrorCode						*/
						
						msgP->head.uiDstPQueueID	= D_RRH_PROCQUE_INIT;					/* 返信先PQID						*/
						msgP->head.uiSrcPQueueID	= D_RRH_PROCQUE_INIT;					/* 送信元PQID						*/
						msgP->head.uiSrcTQueueID	= D_SYS_THDQID_INI_MAIN;				/* 送信元TQID						*/
						msgP->head.uiEventNo		= eventNo;								/* メッセージID						*/

						memcpy( &(msgP->sigInfo), &sigInfo, sizeof( sigInfo ) );
						memcpy( &(msgP->infoTbl), &tmpInfoTbl, sizeof( tmpInfoTbl ) );

						msgP->head.uiDstTQueueID	= D_SYS_THDQID_INI_ALM;					/* 送信先TQID						*/			
						BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_INI_ALM,(VOID *)msgP );
					}
					else if(rtn == D_INI_SIG_RESET_ON)
					{
						f_iniw_alreadyRESET = D_RRH_ON;
						f_ini_logget_alm_reset(&tmpInfoTbl, f_iniw_assert_main_p);
					}else{
						cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR,"Unknown type 0x%x !!!",rtn);
					}
				}
				else{
					cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR,"Unknown Kind 0x%x !!!",sigKind);
				}
				break;
			case SIGUSR2:
				f_iniw_alreadyRESET = D_RRH_ON;
				f_ini_recieve_SIGUSR2( &sigInfo , f_iniw_assert_main_p);
				break;
			default:
				cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR,"rcv signal unknown si_signo = %08x.", sigInfo.si_signo);
				break;
		}

#ifdef OPT_RRH_CT
		break;
#endif

	}

	return(rtn);

}

/* @} */
