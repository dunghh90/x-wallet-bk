/*!
 * @skip  $ld:$
 * @file  f_ini_procCreate.c
 * @brief プロセス起動処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#include "f_ini_inc.h"

#include "errno.h"


static VOID f_ini_procCreate_sig(UINT sigUsr1Flg ,UINT sigUsr2Flg,  VOID* buf_p)
{
	siginfo_t			sigInfo;
	T_INIT_SIG_INFO_TBL info_tbl;

	memset(&sigInfo,0,sizeof(sigInfo));
				
	if(sigUsr2Flg != 0){
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "WDT ALM detected ...");

		sigInfo.si_signo = SIGUSR2;
		sigInfo.si_status = D_RRH_SIGUSR2_OPT_WDTTO;
		/* Reset */
		f_ini_recieve_SIGUSR2( &sigInfo , buf_p);

	}else if(sigUsr1Flg != 0){
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "CRITICAL or MAJOR ALM detected ...");

		UINT				hwAlmFactAry[E_RRH_ALMHIS_ALM_FACTOR_NUM];

		BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
		f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_HW, 0, hwAlmFactAry);

		sigInfo.si_signo  = SIGUSR1;
		sigInfo.si_status = D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC;
		
		memset(&info_tbl,0,sizeof(info_tbl));
		info_tbl.logType = E_INI_LOGTYPE_TROUBLE;
		info_tbl.uiResetFlg_define = D_INI_SIG_RESET_OFF;
		info_tbl.trouble_logget = D_RRH_ON;
		f_ini_logget_alm_reset(&info_tbl,buf_p);

	}else{
		;
	}
	return;
}

/*!
 *	@brief f_ini_procCreate
 *	@note  関数処理内容.
 *			-# PFプロセス起動処理
 *			-# PFプロセス初期化応答を受信し、初期化応答受信テーブルを更新
 *			-# プロセス起動ループ処理
 *			-# L2/L3/MTプロセスの初期化応答を受信し、初期化応答受信テーブルを更新
 *			-# プロセス起動結果チェック
 *				-# 結果異常時、正常起動したプロセスを強制終了
 *				-# 全プロセスの正常起動時、 All task init noticeをイベントを送信する。
 *
 * @param		argc		[in]	受信パラメータ数
 * @param		*argv[]		[in]	受信パラメータ
 * @retval		D_INI_OK	0:正常終了
 * @retval		D_INI_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @date 2013/11/07 ALPHA)nakamura modify for ZYNQ
 * @date 2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 */
UINT f_ini_procCreate(INT argc, CHAR *argv[] , VOID* buf_p)
{
	UINT							rtn = D_INI_OK;
	INT								rtn_bpf = D_RRH_OK;
	INT								cnt = 0;
	INT								pid = 0;
	UINT							proc_No = 0;
	UINT							proc_kind = 0;
	UINT							proc_num = 0;
	bpf_rm_svrm_result				result;
	INT								errcd = 0;
	struct sched_param				s_param;
	INT								rtn_sys;
	T_SYS_ALL_TASK_INIT_COMP_NTC	*sndMsg;		/* 送信MSG		*/

/* 2020/12/28 M&C) Implement device startup (start modify) */
	UINT	DstPQueueID[D_INI_PROCNO_MAX - 2] = {	D_RRH_PROCQUE_PF,
													D_RRH_PROCQUE_F_PF,
													D_RRH_PROCQUE_L2,
													D_RRH_PROCQUE_L3,
													D_RRH_PROCQUE_RE,
													D_RRH_PROCQUE_MT};
/* 2020/12/28 M&C) Implement device startup (end modify) */

	UCHAR							ptp_start_flag = 0x00;

	CHAR							input[2][16];
	CHAR * const					inpara_pf[2] = {&input[0][0], 0};
	UINT							sigUsr1Flg = 0;
	UINT							sigUsr2Flg = 0;
	sigset_t						sigSetVal;

	T_RRH_BF_MODE_TBL*				bf_mode_tbl;
	FILE*							fp;

	memcpy(&sigSetVal,&f_iniw_sigSetVal,sizeof(sigset_t));


	/************************************************************/
	/* Beamforming対応有無 モード判定							*/
	/************************************************************/
	/* DU情報テーブル 共有メモリアドレス取得	*/
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_BF_MODE_TBL,					/* 共有メモリ番号		*/
						(VOID **)&bf_mode_tbl,							/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/

	if( rtn_bpf != BPF_RU_IPCM_OK ) {
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", rtn_bpf, errcd);
		BPF_COM_LOG_DMESG("[mode] can not identify for beamforming mode \n");
	} else {
		fp = fopen( "/opt/sh/nonbeam", "r" );
		if( fp == NULL ) {
			bf_mode_tbl->flag = D_RRH_OFF;
			BPF_COM_LOG_DMESG("[mode] beamforming mode \n");
		} else {
			bf_mode_tbl->flag = D_RRH_ON;
			BPF_COM_LOG_DMESG("[mode] non-beamforming mode \n");
			fclose( fp );
		}
	}

	/************************************************************/
	/* INITのプロセスIDとプライオリティ設定						*/
	/************************************************************/
	pid = getpid();
	f_iniw_pid[E_INI_STARTING_PROC_INIT] = pid;
	f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_INIT] = D_INI_PROC_OK;
	f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_F_PF] = D_INI_PROC_OK;//C9 add code
	procIdTbl->count = 0;
	procIdTbl->info[ procIdTbl->count ].aplProcessID = D_RRH_PROCID_INI;
	procIdTbl->info[ procIdTbl->count ].realProcessID = pid;
	procIdTbl->info[ procIdTbl->count ].PQID = D_RRH_PROCQUE_INIT;
	procIdTbl->count++;
	
	memset(&s_param, 0, sizeof(s_param));
	s_param.sched_priority = f_inir_procCreMng[E_INI_STARTING_PROC_INIT].procPriority;
	rtn_sys = sched_setscheduler(pid, SCHED_FIFO, &s_param);
	if( rtn_sys == -1 )
	{
		/* Write assert log */
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "sched_setscheduler");
	}

	rtn_bpf = BPF_RM_SVRM_CPUAFFINITY (D_RRH_PROCID_INI, f_inir_procCreMng[E_INI_STARTING_PROC_INIT].procCpu, &errcd);
	if( D_RRH_OK != rtn_bpf )
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "%s CPU affinity rtn_bpf=%08x error=%08x"
			, f_inir_procCreMng[E_INI_STARTING_PROC_INIT].procFunc, rtn_bpf, errcd);
	}
	
	/* 障害ログ用のスレッドを生成する	*/
	rtn = f_ini_threadCreate(buf_p);
	if(D_INI_OK != rtn)
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "Thread Create Error");
		return D_INI_NG;
	}

	T_RRH_DU_PROCESS_INFO*	tDuProcInfo;
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_DU_PROCESS_INFO,
				(VOID **)&tDuProcInfo,
				&errcd );
	if( rtn_bpf == BPF_RU_IPCM_OK ){
		tDuProcInfo->pState[D_RRH_PROCID_INI] = D_RRH_OFF;
		tDuProcInfo->pid[D_RRH_PROCID_INI] = pid;
	}

	BPF_COM_LOG_DMESG("[PROCESS(DU_init)] OK COMPLETE\n");	
	/************************************************************/
	/* PFプロセス起動処理										*/
	/************************************************************/
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ENTER, "%s procCreate", f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc);

	BPF_COM_LOG_DMESG("[PROCESS(%s)] START\n",f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc);
	/* 子プロセスを生成 */
	pid = fork();
	if(pid == 0)
	{
		/* 子プロセス処理 */
		rtn_bpf = BPF_RM_SVRM_CPUAFFINITY (D_RRH_PROCID_PF, f_inir_procCreMng[E_INI_STARTING_PROC_PF].procCpu, &errcd);
		if( D_RRH_OK != rtn_bpf )
		{
			BPF_COM_LOG_DMESG("[PROCESS(%s)] %s CPU affinity rtn_bpf=%08x error=%08x"
				, f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc, rtn_bpf, errcd);
		}

		memset(input, 0, sizeof(input));
		strncpy( &input[0][0], f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc, 
				 strlen(f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc) + 1 );

		execvp(f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc, inpara_pf);

		/* 子プロセスリターン時は終了する */
		_exit(EXIT_SUCCESS);
	}
	else
	{
		/* 親プロセス処理 */
		if(pid > 0)
		{
			/* 子プロセス起動OK */
			f_iniw_pid[E_INI_STARTING_PROC_PF] = pid;
			procIdTbl->info[ procIdTbl->count ].aplProcessID = D_RRH_PROCID_PF;
			procIdTbl->info[ procIdTbl->count ].realProcessID = pid;
			procIdTbl->info[ procIdTbl->count ].PQID = D_RRH_PROCQUE_PF;
			procIdTbl->count++;
			
			/* プライオリティ設定 */
			memset(&s_param, 0, sizeof(s_param));
			s_param.sched_priority = f_inir_procCreMng[E_INI_STARTING_PROC_PF].procPriority;
			rtn_sys = sched_setscheduler(pid, SCHED_FIFO, &s_param);
			if( rtn_sys == -1 )
			{
				/* Write assert log */
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "sched_setscheduler");
			}
				
			/* 初期化完了通知待機 */
			while(1)
			{
				sigUsr1Flg = 0;
				sigUsr2Flg = 0;

				/* 初期化完了通知結果受信待ち */
				rtn_bpf = BPF_RM_SVRM_INITEND_IND_RCV(&proc_kind, &proc_num, &result, &sigSetVal , &sigUsr1Flg , &sigUsr2Flg);
				if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
				{
					/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RM_SVRM_INITEND_IND_RCV error=%08x", rtn_bpf);
					rtn = D_INI_NG;
					BPF_COM_LOG_DMESG("[PROCESS(%s)] NG COMPLETE RCV NG(%d)\n",f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc,rtn_bpf);
					break;
				}

				if(BPF_RM_SVRM_OK != result)
				{
					/* 初期化完了通知結果異常 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RM_SVRM_INITEND_IND_RCV result error=%08x", result);
					rtn = D_INI_NG;
					BPF_COM_LOG_DMESG("[PROCESS(%s)] NG COMPLETE RCV result NG(%d)\n",f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc,result);
					break;
				}

				f_ini_procCreate_sig(sigUsr1Flg,sigUsr2Flg,buf_p);

				proc_No = (proc_kind<<D_INI_BIT8) + proc_num;
				if(f_inir_procCreMng[E_INI_STARTING_PROC_PF].procNo == proc_No)
				{
					/* 初期化応答受信テーブルの更新 */
					f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PF] = D_INI_PROC_OK;

					/* 該当の初期化完了通知 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "%s IniInd OK", f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc);
					BPF_COM_LOG_DMESG("[PROCESS(%s)] OK COMPLETE\n",f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc);
					break;
				}
			}
		}
		else
		{
			BPF_COM_LOG_DMESG("[PROCESS(%s)] NG COMPLETE fork NG(%d)\n",f_inir_procCreMng[E_INI_STARTING_PROC_PF].procFunc,errno);
			/* 子プロセス起動NG */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "fork error pid = %08x pf process", (UINT)pid);
			return D_INI_NG;
		}
	}

	/* PFプロセス起動失敗 */
	if(rtn == D_INI_NG)
	{
		/* 初期化応答受信テーブルの更新 */
		f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PF] = D_INI_PROC_NG;
		return(rtn);
	}

	CHAR * const					inpara_ptp[3] = {&input[0][0], &input[1][0], 0};

	/************************************************************/
	/* PTPプロセス起動処理										*/
	/************************************************************/
	(VOID)BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_PTP_STOP_FLAG, &ptp_start_flag);
	ptp_start_flag=1; //C9 ignore ptp
	/* DU_ptp起動あり	*/
	if( (ptp_start_flag != 1) && (ptp_start_flag != 2) ) {

		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ENTER, "DU_ptp procCreate");
		BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] START\n");

		/* 子プロセスを生成 */
		pid = fork();

		/* 子プロセス起動NG */
		if( pid == -1 ) {

			BPF_COM_LOG_DMESG( "[PROCESS(DU_ptp)] NG COMPLETE fork NG(%d)\n", errno );
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "fork error pid = %08x pf process", (UINT)pid);
			return D_INI_NG;

		/* 子プロセス処理	*/
		} else if( pid == 0 ) {

			/* 子プロセス処理 */
			rtn_bpf = BPF_RM_SVRM_CPUAFFINITY (D_RRH_PROCID_PTP, f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procCpu, &errcd);
			if( D_RRH_OK != rtn_bpf ) {

				BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] DU_ptp CPU affinity rtn_bpf=%08x error=%08x", rtn_bpf, errcd);
			}

			/* PTP Process (Slave Mode Start)	*/
			memset(input, 0, sizeof(input));
			strncpy( &input[0][0], f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procFunc, 
					 strlen(f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procFunc) + 1 );
			strncpy( &input[1][0], "-p", 3 );

			execvp(f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procFunc, inpara_ptp);

			/* 子プロセスリターン時は終了する */
			_exit(EXIT_SUCCESS);

		/* 親プロセス処理	*/
		} else {

			f_iniw_pid[E_INI_STARTING_PROC_PTP] = pid;
			procIdTbl->info[procIdTbl->count].aplProcessID  = D_RRH_PROCID_PTP;
			procIdTbl->info[procIdTbl->count].realProcessID = pid;
			procIdTbl->info[procIdTbl->count].PQID          = D_RRH_PROCQUE_PTP;
			procIdTbl->count++;

			/* プライオリティ設定 */
			memset(&s_param, 0, sizeof(s_param));
			s_param.sched_priority = f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procPriority;
			rtn_sys = sched_setscheduler(pid, SCHED_FIFO, &s_param);
			if( rtn_sys == -1 ) {
				/* Write assert log */
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "sched_setscheduler");
			}

			/* 初期化完了通知待機 */
			while(1)
			{
				sigUsr1Flg = 0;
				sigUsr2Flg = 0;

				/* 初期化完了通知結果受信待ち */
				rtn_bpf = BPF_RM_SVRM_INITEND_IND_RCV(&proc_kind, &proc_num, &result, &sigSetVal, &sigUsr1Flg , &sigUsr2Flg);
				if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
				{
					/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RM_SVRM_INITEND_IND_RCV error=%08x", rtn_bpf);
					rtn = D_INI_NG;
					BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] NG COMPLETE RCV NG(%d)\n", rtn_bpf);
					break;
				}

				if(BPF_RM_SVRM_OK != result)
				{
					/* 初期化完了通知結果異常 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RM_SVRM_INITEND_IND_RCV result error=%08x", result);
					rtn = D_INI_NG;
					BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] NG COMPLETE RCV result NG(%d)\n", result);
					break;
				}

				f_ini_procCreate_sig(sigUsr1Flg,sigUsr2Flg,buf_p);


				proc_No = (proc_kind<<D_INI_BIT8) + proc_num;
				if(f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procNo == proc_No)
				{
					/* 初期化応答受信テーブルの更新 */
					f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PTP] = D_INI_PROC_OK;

					/* 該当の初期化完了通知 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "%s IniInd OK", f_inir_procCreMng[E_INI_STARTING_PROC_PTP].procFunc);
					BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] OK COMPLETE\n");
					break;
				}
			}
		}
	}
	else{
/* 2020/12/28 M&C) Implement device startup (start modify) */
		BPF_COM_LOG_DMESG("[PROCESS(DU_ptp)] OK COMPLETE\n");
		f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PTP] = D_INI_PROC_OK;
/* 2020/12/28 M&C) Implement device startup (end modify) */
	}

	/* PFプロセス起動失敗 */
	if(rtn == D_INI_NG)
	{
		/* 初期化応答受信テーブルの更新 */
		f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PTP] = D_INI_PROC_NG;
		return(rtn);
	}
/* 2020/12/28 M&C) Implement device startup (start modify) */
	else {
		f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PTP] = D_INI_PROC_OK;
	}

	/************************************************************/
	/* 各プロセス(PF/PTP以外)起動処理                               */
	/************************************************************/
// 擬似環境 20210311
//	for(cnt=E_INI_STARTING_PROC_F_PF; cnt<(D_INI_PROCNO_MAX - 2); cnt++)				/* F_PFプロセス(=2)からREプロセス(=6)まで	*/
	for(cnt=E_INI_STARTING_PROC_F_PF; cnt<(D_INI_PROCNO_MAX); cnt++)				/* F_PFプロセス(=3)からREプロセス(=7)まで	*/
// 擬似環境 20210311
	{
		/* プロセス起動なし時はスキップ */
		if(D_INI_SKIP == f_inir_procCreMng[cnt].procCreateFlag)
		{
			f_iniw_tsk_rcv_tbl[cnt] = D_INI_PROC_SKIP;
			continue;
		}

		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "%s procCreate", f_inir_procCreMng[cnt].procFunc);
		BPF_COM_LOG_DMESG("[PROCESS(%s)] START\n",f_inir_procCreMng[cnt].procFunc);

		pid = fork();
		
		if(pid == 0)
		{
			/* 子プロセス処理 */
			rtn_bpf = BPF_RM_SVRM_CPUAFFINITY (D_RRH_PROCID_INI, f_inir_procCreMng[cnt].procCpu, &errcd);
			if( D_RRH_OK != rtn_bpf )
			{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "%s CPU affinity rtn_bpf=%08x error=%08x"
					, f_inir_procCreMng[cnt].procFunc, rtn_bpf, errcd);
			}

			strncpy(&(argv[0][0]), f_inir_procCreMng[cnt].procFunc, 
			strlen(f_inir_procCreMng[cnt].procFunc) + 1);
			execvp(f_inir_procCreMng[cnt].procFunc, argv);

			/* 子プロセスリターン時は終了する */
			_exit(EXIT_SUCCESS);
		}
		else
		{
			/* 親プロセス処理 */
			if(pid > 0)
			{
				/* 子プロセス起動OK */
				f_iniw_pid[cnt] = pid;
				procIdTbl->info[ procIdTbl->count ].aplProcessID = f_inir_procCreMng[cnt].procNo;
				procIdTbl->info[ procIdTbl->count ].realProcessID = pid;
				procIdTbl->info[ procIdTbl->count ].PQID = f_inir_procCreMng[cnt].PQID;
				procIdTbl->count++;
				
				/* プライオリティ設定 */
				memset(&s_param, 0, sizeof(s_param));
				s_param.sched_priority = f_inir_procCreMng[cnt].procPriority;
				rtn_sys = sched_setscheduler(pid, SCHED_RR, &s_param);
				if( rtn_sys == -1 )
				{
					/* Write assert log */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "sched_setscheduler");
				}
			}
			else
			{
				BPF_COM_LOG_DMESG("[PROCESS(%s)] NG COMPLETE fork NG(%d)\n",f_inir_procCreMng[cnt].procFunc,errno);
				/* 子プロセス起動NG */
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "fork error pid = %08x, cnt=%d", (UINT)pid, cnt);
				rtn = D_INI_NG;
				break;
			}

			/* プロセス起動失敗 */
			if(rtn == D_INI_NG)
			{
				/* 初期化応答受信テーブルの更新 */
				f_iniw_tsk_rcv_tbl[cnt] = D_INI_PROC_NG;
			}
		}
	}

	/************************************************************/
	/* パラレル起動したプロセスの初期化完了待ち                 */
	/************************************************************/
	for(;;)
	{
		sigUsr1Flg = 0;
		sigUsr2Flg = 0;
		/* プロセスが全て応答受信したかチェック */
		if(
			( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L2] != 0 ) &&
			( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L3] != 0 ) &&
			( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_MT] != 0 ) &&
			( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_RE] != 0 ) )
		{
			break;
		}

		/* 初期化完了通知結果受信待ち */
		rtn_bpf = BPF_RM_SVRM_INITEND_IND_RCV(&proc_kind, &proc_num, &result, &sigSetVal, &sigUsr1Flg, &sigUsr2Flg);
		if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
		{
			/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RM_SVRM_INITEND_IND_RCV error=%08x", rtn_bpf);
			rtn = D_INI_NG;
			BPF_COM_LOG_DMESG("[PROCESS(ini)] NG COMPLETE RCV NG(%d)\n",rtn_bpf);
			break;
		}

		if(BPF_RM_SVRM_OK != result)
		{
			/* 初期化完了通知結果異常 */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR,
				"BPF_RM_SVRM_INITEND_IND_RCV result NG proc_kind=%08x, proc_num=%08x, result=%08x",
				proc_kind, proc_num, result);
			rtn = D_INI_NG;
		}
		else
		{
			/* 初期化完了通知結果正常 */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN,
				"BPF_RM_SVRM_INITEND_IND_RCV result OK proc_kind=%08x, proc_num=%08x, result=%08x",
				proc_kind, proc_num, result);
		}

		f_ini_procCreate_sig(sigUsr1Flg,sigUsr2Flg,buf_p);
		proc_No = (proc_kind<<D_INI_BIT8) + proc_num;

// 擬似環境 20210311
//		for(cnt=0; cnt<(D_INI_PROCNO_MAX -2); cnt++)		/* REプロセス(=4)まで		*/ //C9
		for(cnt=0; cnt<(D_INI_PROCNO_MAX); cnt++)		/* REプロセス(=7)まで		*/ //C9
// 擬似環境 20210311
		{
			if(f_inir_procCreMng[cnt].procNo == proc_No)
			{
				/* 初期化応答受信テーブルの更新 */
				if(BPF_RM_SVRM_OK != result)
				{
					f_iniw_tsk_rcv_tbl[cnt] = D_INI_PROC_NG;
					BPF_COM_LOG_DMESG("[PROCESS(%s)] NG COMPLETE RCV result NG(%d)\n",f_inir_procCreMng[cnt].procFunc,result);
				}
				else
				{
					f_iniw_tsk_rcv_tbl[cnt] = D_INI_PROC_OK;
					/* 該当の初期化完了通知 */
					cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "%s IniInd OK",
						f_inir_procCreMng[cnt].procFunc);
					BPF_COM_LOG_DMESG("[PROCESS(%s)] OK COMPLETE\n",f_inir_procCreMng[cnt].procFunc);
				}
			}
		}
	}

	if( ( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PF] == D_INI_PROC_OK ) &&
		( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_F_PF] == D_INI_PROC_OK ) &&
		( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L2] == D_INI_PROC_OK ) &&
		( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L3] == D_INI_PROC_OK ) &&
		( f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_RE] == D_INI_PROC_OK ) )
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "create process all OK");
	}
	else
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR,
			"create process (NO RCV = 0,OK=1,NG=2,SKIP=3) PF=%d, PTP=%d, F_PF=%d, L2=%d, L3=%d, MT=%d, RE %d"
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PF]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_PTP]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_F_PF]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L2]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_L3]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_MT]
			, f_iniw_tsk_rcv_tbl[E_INI_STARTING_PROC_RE]);
		return(rtn);
	}
/* 2020/12/28 M&C) Implement device startup (end modify) */

	BPF_COM_LOG_DMESG("[PROCESS(DU_init)] OK COMPLETE\n");	

	/**********************************************************************/
	/* 全プロセスの正常起動時、 All task init noticeをイベントを送信する  */
	/**********************************************************************/
	/* 各プロセスへメッセージを送信 */
	for(cnt=0; cnt < (procIdTbl->count - 1); cnt++)
	{
		/* バッファ取得 */
		rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
											sizeof(T_SYS_MPLANE_SW_INIT_COMP_NTC),	/* MessageSize						*/
											(VOID **)&sndMsg,						/* MessageBuffer					*/
											&errcd );								/* ErrorCode						*/
		if(0 != rtn_bpf)
		{
			/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%08x, error=%08x", rtn_bpf, errcd);
			rtn = D_INI_NG;
			break;
		}

		/* 各プロセスへの「All task init notic」作成	*/
		sndMsg->head.uiEventNo		= D_SYS_MSGID_ALLTSKININTC;						/* メッセージID						*/
		sndMsg->head.uiDstPQueueID	= DstPQueueID[cnt];								/* 送信先Process Queue ID			*/
		sndMsg->head.uiDstTQueueID	= 1;											/* 送信先Thread Queue ID(xx_Main)	*/
		sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_INIT;							/* 送信元Process Queue ID			*/
		sndMsg->head.uiSrcTQueueID	= 0					;							/* 送信元Thread Queue ID			*/
		sndMsg->head.uiLength		= sizeof(T_SYS_ALL_TASK_INIT_COMP_NTC);			/* データ部バイト長					*/

		/* 各プロセスへ「All task init notice」を通知	*/
		rtn_bpf = BPF_RU_IPCM_MSGQ_SEND( DstPQueueID[cnt], (VOID *)sndMsg );
		if(0 != rtn_bpf)
		{
			/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG return=%08x", rtn_bpf);
			rtn = D_INI_NG;
			break;
		}
		else
		{
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "All task init notice send OK DstPQueueID=%08x", DstPQueueID[cnt]);
		}
	}

	/************************************************************/
	/* NETCONF接続処理開始										*/
	/************************************************************/
	rtn = f_ini_threadCreate2(buf_p);
	if(D_INI_OK != rtn)
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "Thread Create Error");
	}

	return(rtn);
}

/* @} */
