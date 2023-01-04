/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_INITEND_IND.c
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
#include "bpf_i_ru_ipcm.h"

/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  初期化完了通知
 *  @note   各プロセス毎の初期設定時の結果をINITプロセスに通知する。
 *  @param  result [in]   結果コード
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_INITEND_IND(bpf_rm_svrm_result result)
{

    struct svrm_msgbuf msgbuf;
    int msgflg;
    unsigned int process_desc;
    int proc_seq;
    int ret;
    int msgqid;
	T_RRH_HEAD			dataBuf;			/* RealTimeLog用	*/
	unsigned int		pID;
	unsigned int		tID;
	
    msgflg = 0;                            /* フラグ無し */

    /********************************************
    * プロセス識別子の取得                      *
    ********************************************/
    process_desc = BPF_COM_GET_MY_PROCESS_DESC();

    /* Get the Current proccess Seq No */
    proc_seq = bpf_com_get_proc_num(process_desc);

    /* メッセージタグ設定(1固定) */
    msgbuf.mtype = SVRM_MSGQ_MSGTYPE; /* SVRM_MSGQ_MSGTYPE=1 */
    msgbuf.prockind = bpf_com_get_proc_tkind(process_desc);
    msgbuf.procnum = proc_seq;
    msgbuf.result = result; 

    /* メッセージキュー確認 */
    msgqid = msgget( BPF_COM_IPCKEY_MASK_INTERNAL,SVRM_MSGFLG);

    if( msgqid == SVRM_MSGGET_NG ){ /* 戻り値-1で作成失敗 */
        SVRM_DBGLOG_FMT_ERR(
            "msgget failed. msgqid = %08x, flg = %d, errno = %d",
            msgqid, SVRM_MSGFLG, errno);

        return BPF_RM_SVRM_ERR_SYS;
    }
	
	/* real timeログのために情報を作成する	*/
	memset( &dataBuf, 0 , sizeof(T_RRH_HEAD) );
	dataBuf.uiEventNo = 0x12345678;
	dataBuf.uiLength = sizeof( T_RRH_HEAD );
	
	/* スレッド情報を取得する	*/
	(VOID)bpf_rm_svrm_thread_info_get( &pID, &tID );
	
	/* real time log取得	*/
	bpf_ru_ipcm_real_time_log_get( pID, 254, D_RRH_PROCID_INI, 254, &dataBuf );
	
    /* LOG(TRACE) */
    SVRM_DBGLOG_FMT_TRC("BPF_RM_SVRM_INITEND_IND msgsnd key = 0x%08x msgqid = 0x%08x" , BPF_COM_IPCKEY_MASK_INTERNAL, msgqid);

    ret = msgsnd(msgqid, &msgbuf, sizeof(msgbuf), msgflg);

    if( ret == SVRM_MSGSND_NG ){ /* 戻り値-1で失敗 */
        SVRM_DBGLOG_FMT_ERR(
            "msgsnd failed. msgqid = %08x, flg = %d, errno = %d",
            msgqid, msgflg, errno);

        return BPF_RM_SVRM_ERR_SYS;
    }

    return BPF_RM_SVRM_COMPLETE;
}

/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
