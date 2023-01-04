/*!
 * @skip		$ld:$
 * @file		f_lgc_MSI_rcv_ind.c
 * @brief		MSI受信通知処理
 * @author		FJT)Taniguchi
 * @date		2019/02/25 FJT) Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <signal.h>
#include "f_lgc_inc.h"
#include "f_mpsw_msg_o-ran_troubleshooting_typ.h"
#include "f_mpsw_msg_o-ran_troubleshooting_def.h"


/*!
 * @brief		関数機能概要:ログ取得処理
 * @note		関数処理内容:ログ取得を行う
 * @param		w_requestId		[in]	requestId
 * @param		w_payloadSize	[in]	payloadSize
 * @param		w_payloadPtr	[in]	payloadPtr
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/25 FJT) Taniguchi Create
 */
static UINT	f_lgcw_log_state = D_LGC_LOGSTATE_STOP;
static unsigned long int f_lgcw_log_start_thread = 0;
static UINT f_lgc_log_start_counter = 0;
static volatile sig_atomic_t f_lgcw_tmpthread_alive = 0;

static void f_lgc_log_start_handler(int val){
	f_lgcw_tmpthread_alive = 1;
	BPF_RU_ITCM_THREAD_EXIT();
	return;
}

static VOID f_lgc_log_start(void* counter)
{
	INT													rtn = D_RRH_OK;
	CHAR												cmd[256];
	CHAR												tmpfile[256];
	FILE*												fp;
	LONG												troublelog_size;
	T_SYS_TROUBLE_LOG_OUTPUT_REQ*						sndTroublelogMsg;
	INT													bpf_ret;
	INT													errcd;
	pthread_t											self_Tid = pthread_self();

	memset(tmpfile,0,sizeof(tmpfile));
	snprintf( tmpfile,sizeof(tmpfile),"/var/log/sendlog.tar.gz");

	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
						"TROUBLESHOOTING RUNNING c:0x%08x t:0x%lx s:0x%08x tmp:%s",
						*(UINT*)counter,self_Tid , f_lgcw_log_state , tmpfile);

	if ( signal(SIGRTMIN, f_lgc_log_start_handler) == SIG_ERR ) {	/* pgr0203 */
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
						"TROUBLESHOOTING RUNNING handle error c:0x%08x t:0x%lx s:0x%08x ",
						*(UINT*)counter,self_Tid , f_lgcw_log_state );
	}

	rtn = system("rm -rf /var/log/sendlog/* 1>/dev/null 2> /dev/null");

	rtn = system("cmd_alllog > /var/log/sendlog/cmd_alllog.log");
	if(rtn == D_RRH_MNG){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR,
						"TROUBLESHOOTING RUNNING step1 ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
						*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);

		f_lgcw_log_start_thread = 0;	/* 念のため初期化 */
		return;
	}

	rtn |= system("cmd_logfilescpy /var/log/sendlog/ 1>/dev/null 2> /dev/null");
	rtn |= system("cp -rf /var/log/trouble_rst*.tar.gz /var/log/sendlog/ 1>/dev/null 2> /dev/null");
	rtn |= system("cp -rf /var/log/healthchklog.tar.gz /var/log/sendlog/ 1>/dev/null 2> /dev/null");
	if(rtn == D_RRH_MNG){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
						"TROUBLESHOOTING RUNNING step2 ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
						*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);
		/* 無視 */
	}

	memset(cmd,0,sizeof(cmd));
	snprintf( cmd,sizeof(cmd),"tar zc -f %s /var/log/sendlog 1>/dev/null 2> /dev/null",tmpfile);
	rtn = system(cmd);
	if(rtn == D_RRH_MNG){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR,
						"TROUBLESHOOTING RUNNING step3 ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
						*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);

		f_lgcw_log_start_thread = 0;	/* 念のため初期化 */
		return;
	}

	if((fp = fopen(tmpfile,"r")) != NULL){
		fseek(fp, 0, SEEK_END);
		troublelog_size = ftell(fp);
		fclose(fp);

		if(troublelog_size > D_LGC_TROUBLESHOOTINGLOG_MAXSIZE){

			memset(cmd,0,sizeof(cmd));
			snprintf( cmd,sizeof(cmd),"rm -rf %s 1>/dev/null 2> /dev/null",tmpfile);
			rtn |= system(cmd);

			rtn |= system("rm -rf /var/log/sendlog/trouble_rst*.tar.gz 1>/dev/null 2> /dev/null");
			rtn |= system("rm -rf /var/log/sendlog/healthchklog.tar.gz 1>/dev/null 2> /dev/null");
			rtn |= system("rm -rf /var/log/sendlog/mplane*.pcap 1>/dev/null 2> /dev/null");

			memset(cmd,0,sizeof(cmd));
			snprintf( cmd,sizeof(cmd),"tar zc -f %s /var/log/sendlog 1>/dev/null 2> /dev/null",tmpfile);
			rtn = system(cmd);
			if(rtn == D_RRH_MNG){
				cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
								"TROUBLESHOOTING RUNNING step4 ERROR LOG size split c:0x%08x t:0x%lx s:0x%08x r:0x%08x s:%d",
								*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn,troublelog_size);

				f_lgcw_log_start_thread = 0;	/* 念のため初期化 */
				return;
			}
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
								"TROUBLESHOOTING RUNNING LOG size split c:0x%08x t:0x%lx s:0x%08x r:0x%08x sz:%d",
								*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn,troublelog_size);
		}
		else{
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
								"TROUBLESHOOTING RUNNING LOG size c:0x%08x t:0x%lx s:0x%08x r:0x%08x sz:%d",
								*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn,troublelog_size);

		}
	}

	memset(cmd,0,sizeof(cmd));
	snprintf( cmd,sizeof(cmd),"mv -f %s %s 1>/dev/null 2> /dev/null",tmpfile,D_LGC_TROUBLESHOOTINGLOG_NAME);
	rtn = system(cmd);
	if(rtn == D_RRH_MNG){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR,
						"TROUBLESHOOTING RUNNING step5 ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
						*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);

		f_lgcw_log_start_thread = 0;	/* 念のため初期化 */
		return;
	}

	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
						"TROUBLESHOOTING RUNNING %s create c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
							D_LGC_TROUBLESHOOTINGLOG_NAME, *(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);

	rtn |= system("rm -rf /var/log/sendlog/* 1>/dev/null 2> /dev/null");
	if(rtn == D_RRH_MNG){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
							"TROUBLESHOOTING RUNNING step6 ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
						*(UINT*)counter,self_Tid , f_lgcw_log_state,rtn);
		/* 無視 */
	}

	bpf_ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof( T_SYS_TROUBLE_LOG_OUTPUT_REQ ),		/* MessageSize						*/
										(VOID **)&sndTroublelogMsg,					/* MessageBuffer					*/
										&errcd );									/* ErrorCode						*/
	if(BPF_RU_IPCM_OK == bpf_ret){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
								"TROUBLESHOOTING RUNNING LOG Comp c:0x%08x t:0x%lx s:0x%08x",
								*(UINT*)counter,self_Tid , f_lgcw_log_state);

		sndTroublelogMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 返信先PQID						*/
		sndTroublelogMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_LGC;				/* 送信先TQID						*/
		sndTroublelogMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元PQID						*/
		sndTroublelogMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_LGC;				/* 送信元TQID						*/
		sndTroublelogMsg->head.uiEventNo		= D_SYS_MSGID_TROUBLELOG_OUT_REQ;	/* メッセージID						*/
		sndTroublelogMsg->trouble_kind			= *(UINT*)counter;
		BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_LGC,(VOID *)sndTroublelogMsg );	

	}else{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR,
					"TROUBLESHOOTING IND error c:0x%08x t:0x%lx s:0x%08x ",*(UINT*)counter,self_Tid , f_lgcw_log_state);

		f_lgcw_log_start_thread = 0;	/* 念のため初期化 */
	}
	return;
}

/*!
 * @brief		関数機能概要:ログ取得完了通知処理
 * @note		関数処理内容:ログ取得完了通知を行う
 * @param		w_requestId		[in]	requestId
 * @param		w_payloadSize	[in]	payloadSize
 * @param		w_payloadPtr	[in]	payloadPtr
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/25 FJT) Taniguchi Create
 */
INT f_lgc_MSI_send_ind(VOID* rcvMsg_p)
{
	T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND	ind;
	T_SYS_TROUBLE_LOG_OUTPUT_REQ*	sndTroublelogMsg;
	INT	rtn;

	if(f_lgcw_log_state == D_LGC_LOGSTATE_STOP){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
						"TROUBLESHOOTING FINISH(Already STOP)  c:0x%08x t:0x%lx s:0x%08x ",
						f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);
		return D_RRH_OK;
	}

	sndTroublelogMsg = (T_SYS_TROUBLE_LOG_OUTPUT_REQ*)rcvMsg_p;
	if(f_lgc_log_start_counter != sndTroublelogMsg->trouble_kind){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
						"TROUBLESHOOTING FINISH(MISMATCH counter) c:0x%08x expc:t:0x%lx s:0x%08x ",
						sndTroublelogMsg->trouble_kind,f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);
		return D_RRH_OK;
	}

	f_com_mtCtl_con(D_RRH_ON);

	memset(&ind,0,sizeof(ind));
	snprintf(&ind.log_file_name[0][0],sizeof(ind.log_file_name[0]),D_LGC_TROUBLESHOOTINGLOG_NAME);
	ind.file_num++;
	rtn = f_com_MSIsend(D_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND,
					0,sizeof(ind),&ind,D_RRH_PROCQUE_PF,D_SYS_THDQID_PF_LGC);

	f_lgcw_log_state = D_LGC_LOGSTATE_STOP;

	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
						"TROUBLESHOOTING FINISH c:0x%08x t:0x%lx s:0x%08x ",
						f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);

	f_lgcw_log_start_thread = 0;

	return rtn;
}



/*!
 * @brief		関数機能概要:ログ取得開始処理
 * @note		関数処理内容:ログ取得開始を行う
 * @param		w_requestId		[in]	requestId
 * @param		w_payloadSize	[in]	payloadSize
 * @param		w_payloadPtr	[in]	payloadPtr
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/25 FJT) Taniguchi Create
 */
INT f_lgc_MSI_rcv_ind_start(UINT w_requestId,INT w_payloadSize,VOID* w_payloadPtr)
{
	INT	rtn = D_RRH_OK;
	INT	endcd;
	INT	bpf_rtn;
	unsigned long int thread;
	UINT	count = 0;

	if(f_lgcw_log_state == D_LGC_LOGSTATE_RUNNING){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
					"TROUBLESHOOTING Already RUNNING c:0x%08x t:0x%lx s:0x%08x ",
					f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);

		if(f_lgcw_log_start_thread != 0){
			if(( bpf_rtn = BPF_RU_ITCM_THREAD_DELETE(f_lgcw_log_start_thread)) == BPF_RU_ITCM_OK ){
				cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					"TROUBLESHOOTING LOG Thread KILL START t:0x%lx a:0x%x ",
					f_lgcw_log_start_thread,f_lgcw_tmpthread_alive);
				/* max 9sec待つ(msiの待ち時間 - 1秒とする) */
				for(count = 0; count < 900 ;count++){
					if( f_lgcw_tmpthread_alive != 0 ){
						break;
					}
					f_com_taskDelay(10,0,0,0);
				}
			}
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
				"TROUBLESHOOTING LOG Thread KILL CMP t:0x%lx a:0x%x c:%d",
				f_lgcw_log_start_thread,f_lgcw_tmpthread_alive,count);

			f_lgcw_tmpthread_alive = 0;
			f_lgcw_log_start_thread = 0;
		}
		f_lgcw_log_state = D_LGC_LOGSTATE_STOP;

	}
	else{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					"TROUBLESHOOTING START RCV c:0x%08x t:0x%lx s:0x%08x ",
					f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);
	}

	f_lgc_log_start_counter++;

	bpf_rtn = BPF_RU_ITCM_THREAD_CREATE(
						&thread,
						BPF_RU_ITCM_SCHED_OTHER,
						0,
						D_RRH_STACKSIZE_DEFAULT,
						f_lgc_log_start,
						&f_lgc_log_start_counter,
						&endcd);

	if(BPF_RU_IPCM_OK == bpf_rtn){
		f_lgcw_log_state = D_LGC_LOGSTATE_RUNNING;

		f_com_mtCtl_con(2);

		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					"TROUBLESHOOTING Thread START c:0x%08x t:0x%lx s:0x%08x ",
					f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);

		rtn = f_com_MSIsend(D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM,
							w_requestId,0,NULL,D_RRH_PROCQUE_PF,D_SYS_THDQID_PF_LGC);
							
		f_lgcw_log_start_thread = thread;

	}else{

		T_MPSW_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ	rej;
		memset(&rej,0,sizeof(rej));

		rej.msgErr[0].errorCode = bpf_rtn;
		snprintf(&rej.msgErr[0].errorDescription[0],sizeof(rej.msgErr[0].errorDescription),"Thread create error");

		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING,
			 "TROUBLESHOOTING Thread ERROR c:0x%08x t:0x%lx s:0x%08x r:0x%08x",
			 f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state,bpf_rtn);

		rtn = f_com_MSIsend(D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ,
							w_requestId,sizeof(rej),&rej,D_RRH_PROCQUE_PF,D_SYS_THDQID_PF_LGC);

	}
	return rtn;
}


/*!
 * @brief		関数機能概要:ログ取得中止処理
 * @note		関数処理内容:ログ取得中止を行う
 * @param		w_requestId		[in]	requestId
 * @param		w_payloadSize	[in]	payloadSize
 * @param		w_payloadPtr	[in]	payloadPtr
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/25 FJT) Taniguchi Create
 */
INT f_lgc_MSI_rcv_ind_stop(UINT w_requestId,INT w_payloadSize,VOID* w_payloadPtr)
{
	INT	rtn = D_RRH_OK;
	INT	bpf_rtn;
	UINT	count = 0;

	rtn = f_com_MSIsend(D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM,
						w_requestId,0,NULL,D_RRH_PROCQUE_PF,D_SYS_THDQID_PF_LGC);

	if(f_lgcw_log_state == D_LGC_LOGSTATE_RUNNING){
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					 "TROUBLESHOOTING STOP RCV c:0x%08x t:0x%lx s:0x%08x ",
					 f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);

		if(f_lgcw_log_start_thread != 0){
			if(( bpf_rtn = BPF_RU_ITCM_THREAD_DELETE(f_lgcw_log_start_thread)) == BPF_RU_ITCM_OK ){
				cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					"TROUBLESHOOTING LOG Thread KILL START t:0x%lx a:0x%x ",
					f_lgcw_log_start_thread,f_lgcw_tmpthread_alive);
				/* max 9sec待つ(msiの待ち時間 - 1秒とする) */
				for(count = 0; count < 900 ;count++){
					if( f_lgcw_tmpthread_alive != 0 ){
						break;
					}
					f_com_taskDelay(10,0,0,0);
				}
			}
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
				"TROUBLESHOOTING LOG Thread KILL CMP t:0x%lx a:0x%x c:%d",
				f_lgcw_log_start_thread,f_lgcw_tmpthread_alive,count);

			f_lgcw_tmpthread_alive = 0;
			f_lgcw_log_start_thread = 0;
		}

		f_com_mtCtl_con(D_RRH_ON);
		f_lgcw_log_state = D_LGC_LOGSTATE_STOP;

	}else{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO,
					 "TROUBLESHOOTING STOP RCV(Already STOP) c:0x%08x t:0x%lx s:0x%08x ",
					 f_lgc_log_start_counter,f_lgcw_log_start_thread , f_lgcw_log_state);
	}
	
	return rtn;
}


/*!
 * @brief		関数機能概要:MSI受信通知処理
 * @note		関数処理内容:MSIの判定を実施し、ログ取得もしくはログ取得中止を行う
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/25 FJT) Taniguchi Create
 */
INT f_lgc_MSI_rcv_ind(VOID* rcvMsg_p)
{
	INT						rtn			= D_RRH_NG;		/*	処理結果格納変数		*/
	UINT					w_messageId = 0;
	UINT					w_requestId = 0;
	INT						w_payloadSize;
	VOID*					w_payloadPtr;

	f_com_MSIgetPayload ( rcvMsg_p ,			/* [in]  Receive Message pointer			*/
						  &w_messageId ,		/* [out] Receive Message ID					*/
						  &w_requestId ,		/* [out] Receive Message Request ID			*/
						  &w_payloadSize ,		/* [out] Receive Message payload size		*/
						  &w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	switch(w_messageId){
		case D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ:
			rtn = f_lgc_MSI_rcv_ind_start(w_requestId,w_payloadSize,w_payloadPtr);
			break;
		case D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ:
			rtn = f_lgc_MSI_rcv_ind_stop(w_requestId,w_payloadSize,w_payloadPtr);
			break;
		default:
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "unknown messageId(0x%08x)", w_messageId );
			break;
	}
	return rtn;
}

/* @} */
