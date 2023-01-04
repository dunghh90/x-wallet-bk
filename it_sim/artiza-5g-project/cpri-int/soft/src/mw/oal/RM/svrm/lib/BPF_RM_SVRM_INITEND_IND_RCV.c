/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_INITEND_IND_RCV.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RM_SVRM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_rm_svrm.h"

/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  初期化完了結果受信
 *  @note   各プロセス毎の初期設定時の結果を受信する。
 *          共通プロセス管理機能割付けを発行したプロセスにて発行すること。
 *  @param  prockind [out] プロセス種別
 *  @param  procnum [out]  プロセス番号
 *  @param  result [out]   結果コード
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_INITEND_IND_RCV (unsigned int *prockind , unsigned int *procnum , bpf_rm_svrm_result *result, sigset_t *sigSetVal , unsigned int* sigUsr1Flg , unsigned int* sigUsr2Flg )
{
    struct svrm_msgbuf msgbuf;
    int msgflg;
    int ret;
    int msgqid;
    int count;
	int proc_cnt;
	int errcd;
	int ret_queue;
	T_RRH_PID_MNG			*procIdTbl;								/** プロセスID管理テーブル		*/
	struct timespec time;
	siginfo_t			sigInfo;
	
    msgflg = MSG_NOERROR | IPC_NOWAIT;

    /********************************************
    * message receive processing.               *
    ********************************************/
    /* メッセージタグ設定(1固定) */
    msgbuf.mtype = SVRM_MSGQ_MSGTYPE;

    /* メッセージキュー確認 */
    msgqid = msgget( BPF_COM_IPCKEY_MASK_INTERNAL,SVRM_MSGFLG);

    if( msgqid == SVRM_MSGGET_NG ){ /* 戻り値-1で作成失敗 */
        SVRM_DBGLOG_FMT_ERR(
            "msgget failed. msgqid = %08x, flg = %d, errno = %d",
            msgqid, SVRM_MSGFLG, errno);
        return BPF_RM_SVRM_ERR_SYS;
    }

    /* LOG(TRACE) */
    SVRM_DBGLOG_FMT_TRC("BPF_RM_SVRM_INITEND_IND_RCV msgsnd key = 0x%08x msgqid = 0x%08x" , BPF_COM_IPCKEY_MASK_INTERNAL, msgqid);

	/* プロセスID管理テーブルを共有メモリから取得               */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&errcd
	);
	
	time.tv_sec = 0;
	time.tv_nsec = 100;
	
   	/* 100ms周期で300s間監視する	*/
	for( count = 0; count < D_DU_STARTUP_TIME ; count++ )
	{
	    ret = msgrcv(msgqid, &msgbuf, sizeof(msgbuf), msgbuf.mtype, msgflg);
		
		/* 正常に受信できたらループを抜ける	*/
		if( ret != SVRM_MSGRCV_NG )
		{
			break;
		}
		
		/* 各プロセス起動時に起動失敗で死亡する場合があり、その際にSignalが飛んでくる。					*/
		/* Signal受信＝プロセス死亡と考える																*/
		/* ただし、system関数のSignalも飛んでくるので共有メモリに登録してあるプロセスからのみ受け付ける	*/
		ret_queue = BPF_RM_SVRM_SIGNAL_WAIT_TIME( sigSetVal, &time, &sigInfo.si_pid,
													&sigInfo.si_signo, &sigInfo.si_status);
		/* 正常であれば何らかのSIGNALを受信したということ	*/
		if(ret_queue == BPF_RM_SVRM_COMPLETE )
		{
			if(sigInfo.si_signo == SIGCHLD){
				/* 認識のあるプロセス数分チェック				*/
				/* 共有メモリの先頭はINITなのでチェックしない	*/
				for( proc_cnt = 1; proc_cnt < procIdTbl->count;  proc_cnt++ )
				{
					/* 登録してあるPIDから何かが来たら死亡と判断する	*/
					if( sigInfo.si_pid == procIdTbl->info[ proc_cnt ].aplProcessID )
					{
						return BPF_RM_SVRM_ERR_SYS;
					}
				}
			}else{
				if((sigInfo.si_signo == SIGUSR1) && (sigInfo.si_status == D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC)){
					*sigUsr1Flg = BPF_RM_SVRM_ERR_EXEC;
					SVRM_DBGLOG_FMT_ERR("SIGUSR1 RESORCE_ALM_NTC recv pid:0x%x no:0x%x sts:0x%x ",
							sigInfo.si_pid,
							sigInfo.si_signo,
							sigInfo.si_status);
				} else if((sigInfo.si_signo == SIGUSR2) && (sigInfo.si_status == D_RRH_SIGUSR2_OPT_WDTTO)){
					*sigUsr2Flg = BPF_RM_SVRM_ERR_EXEC;
					SVRM_DBGLOG_FMT_ERR("SIGUSR2 WDTTO recv pid:0x%x no:0x%x sts:0x%x ",
							sigInfo.si_pid,
							sigInfo.si_signo,
							sigInfo.si_status);
				}
				else{
					SVRM_DBGLOG_FMT_ERR("not SIGCHLD recv pid:0x%x no:0x%x sts:0x%x ",
							sigInfo.si_pid,
							sigInfo.si_signo,
							sigInfo.si_status);
				}
			}
		}
		
		/* 100msec wait	*/
		usleep(100000);
	}

    /* 受信失敗時 */
    if( ret == SVRM_MSGRCV_NG ){ 
    
        SVRM_DBGLOG_FMT_ERR(
            "msgrcv failed. msgqid = %08x, flg = %d, errno = %d",
            msgqid, msgflg, errno);
        return BPF_RM_SVRM_ERR_SYS;
    }

    /* プロセス種別の設定 */
    *prockind = msgbuf.prockind;
    
    /* プロセス番号の設定 */
    *procnum = msgbuf.procnum;

    /* 結果コードの設定 */
    *result = msgbuf.result;

    return BPF_RM_SVRM_COMPLETE;
}

/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
