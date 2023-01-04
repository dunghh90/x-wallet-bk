/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	f_BPF_RU_IPCM_MSGQ.c
*	@brief	Inter Process Communication Management Library(MSGQ).
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *  @date   2015/10/22 ALPHA)ueda modify for TDD-RRE(zynq)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <sys/mman.h>

#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm.h"
#include "bpf_i_ru_ipcm_msgq.h"

#include "f_rrh_inc.h"

extern int bpf_rm_svrm_thread_info_get( unsigned int *processID, unsigned int *threadID );
/** @addtogroup BPF_RU_IPCM
 * @{ */

/* マクロ定義 */

/* メッセージキュータイプの範囲チェック */
#define IPCM_MSGQ_TYPECHECK(type)     \
  do { \
    if( type >= IPCM_MSGQ_TYPE_END ){ \
      IPCM_DBGLOG_FMT_ERR("msgq type range NG. msgq type = %d", type);  \
      return BPF_RU_IPCM_EPARAM;         \
    }                                 \
  } while(0)

#define D_IPCM_REALPACKET_MAXSIZE	256			/* max packet size */


typedef struct {
	struct	ip	iphead;
	char		dummy[4];
	char		data[D_IPCM_REALPACKET_MAXSIZE];
} t_rrupdump_msg;


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Send.
 *  @note   API function.
 *  @param  q_num        [in]  queue number
 *  @param  msgp         [in]  send message address
 *  @param  msgsz        [in]  send message size
 *  @param  q_flag       [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_EQFULL    queue full error
 *  @retval BPF_RU_IPCM_ENOSPACE  no message space error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSGQ_SEND(unsigned int q_num, const void *msgp,
			   unsigned int msgsz, int q_flag, int *errcd)
{
	int ret;
    /*****************************************************
    * message send processing.(call internal routine)    *
    *****************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procmsgq_send2(IPCM_MSGQ_USER, q_num, msgp, msgsz, q_flag,
                 errcd);
/* RRH-001 MD alpha)matsuhashi add end */

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Send2.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  q_num        [in]  queue number
 *  @param  msgp         [in]  send message address
 *  @param  msgsz        [in]  send message size
 *  @param  q_flag       [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_ENOSPACE  no message space error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procmsgq_send2(enum ipcm_msgq_type q_type, unsigned int q_num,
                            const void *msgp, unsigned int msgsz, int q_flag,		/* pgr0339 */
			    int *errcd)
{
    struct ipcm_msgbuf *tmpbuf;
    struct ipcm_msgbuf *msgbuf;
    struct bpf_ru_ipcm_msg_databuf_t *sndbuf;
    int msgid, used, msgflg;
    unsigned int maxmsglen;

    int ret;

/* docomo ZYNQ対応	start */
	/* 通信の方式は以下の通りである。							*/
	/* 共有メモリに個別データを設定し							*/
	/* 実際には共通ヘッダのみを送信する							*/
	/* そのため、メッセージバッファにコピーするのは共通ヘッダ部のみである	*/
	msgsz = sizeof(T_RRH_HEAD);		/* Header(32byte)	*/
/* docomo ZYNQ対応 end	*/

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
    IPCM_MSGQ_TYPECHECK(q_type);
    /* キュー番号のチェック */
    if( bpf_ru_ipcm_msgq_numcheck(q_type, q_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq number check NG. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_EPARAM;
    }
    /* メッセージサイズのチェック */
    if( bpf_ru_ipcm_msgq_msgszcheck(q_type, q_num, msgsz) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "msg size check NG. msgq type = %d, num = %u, size = %u",
            q_type, q_num, msgsz);
        return BPF_RU_IPCM_EPARAM;
    }
    /********************************************
    * message queue check processing.           *
    ********************************************/
    /* msgp のチェック(msgszが0の場合はチェックしない) */
    if( msgsz != 0 && msgp == NULL ){
        IPCM_DBGLOG_FMT_ERR(
            "msgp check NG. msgp = NULL");
        return BPF_RU_IPCM_EPARAM;
    }
    /* フラグのチェック */
    if( q_flag == BPF_RU_IPCM_WAIT ){          /* 送信待ちあり */
        msgflg = 0;                            /* フラグ無し */
    } else if( q_flag == BPF_RU_IPCM_NOWAIT ){ /* 送信待ちなし */
        msgflg = IPC_NOWAIT;                   /* フラグにIPC_NOWAIT設定 */
    } else { /* それ以外はパラメータ異常 */
       IPCM_DBGLOG_FMT_ERR(
            "wait flag check NG. flag = %d",q_flag);
       return BPF_RU_IPCM_EPARAM;
   }

    /* データ取得 */
    bpf_ru_ipcm_msgq_read_data(q_type, q_num, &msgid, &maxmsglen, &used);
    /* 未使用ならキュー番号なしを返す．削除がないのでありえない．*/
    if( used == IPCM_MSGQ_UNUSED ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq is unused. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_ENOQNUM; /* キュー無し */
    }

    /* 送信バッファ設定 */
    msgbuf = NULL;
    tmpbuf = NULL;
    /* 送信用領域の確保 */
    bpf_ru_ipcm_msgq_read_sdatabuf(q_type, q_num, &sndbuf);
    ret = pthread_mutex_lock(&(sndbuf->mutex));
    if( ret == IPCM_MUTEX_OK ){

		if( sndbuf->use_flg == BPF_RU_IPCM_FLG_OFF ){
			msgbuf = sndbuf->data_buf;
			sndbuf->use_flg = BPF_RU_IPCM_FLG_ON;
		}
		pthread_mutex_unlock(&(sndbuf->mutex));
	}
	/* 送信用領域が確保できない場合mallocで確保 */
	if( msgbuf == NULL ){
		tmpbuf = malloc(sizeof(struct ipcm_msgbuf)+msgsz);
		if( tmpbuf == NULL ){
			/* バッファ確保失敗 */
	        IPCM_DBGLOG_FMT_ERR(
	            "No send buffer. msgq type = %d, num = %u, size = %u",
	            q_type, q_num, msgsz);
			return BPF_RU_IPCM_NG;
		}
	    msgbuf = tmpbuf;
	}


    /* メッセージタグ設定(1固定) */
    msgbuf->mtype = IPCM_MSGQ_MSGTYPE; /* IPCM_MSGQ_MSGTYPE=1 */

    /* メッセージバッファにメッセージをコピーする					*/
	/* msgszは32byteに変更しているので、共通ヘッダのみコピーする	*/
	memmove(msgbuf->mtext, msgp, msgsz);

	/* 共有メモリをデタッチする	*/
	shmdt( msgp );
    /********************************************
    * message send processing.                  *
    ********************************************/
    /* メッセージ送信 */
    ret = msgsnd(msgid, msgbuf, msgsz, msgflg);
    *errcd = errno;

    if(ret == IPCM_MSGSND_OK){
		/* メッセージ送信正常 */

		/* 送信用バッファの解放 */
		if(tmpbuf != NULL){
			free(tmpbuf);
		}else{
			sndbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
		}
        return BPF_RU_IPCM_OK;
    }

	/* 送信用バッファの解放 */
	if(tmpbuf != NULL){
		free(tmpbuf);
	}else{
		sndbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
	}

    IPCM_DBGLOG_FMT_ERR(
      "msgsnd failed. msgq type = %d, num = %u, ret = %d, errno = %d, msgqid = %d, msgsz = %d, msgflg = %d",
      q_type, q_num, ret, *errcd, msgid, msgsz, msgflg);

    /********************************************
    * result code setting processing.           *
    ********************************************/
    switch(*errcd){
        case EAGAIN: /* NOWAIT指定で送信のサイズがキューの制限を越えていた */
            return BPF_RU_IPCM_ESIZEOVER;
            break;
        case EFAULT: /* msgbufの指しているアドレスが不正(これでたらミドルのバグ)*/
            return BPF_RU_IPCM_EADDR;
            break;
        case EINTR:  /* 割り込みにより中断 */
            return BPF_RU_IPCM_EINTR;
            break;
        case ENOMEM: /* メッセージをコピーするメモリがシステムになかった */
            return BPF_RU_IPCM_ENOSPACE;
            break;
        default:
            return BPF_RU_IPCM_NG;
    }
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Receive.
 *  @note   API function.
 *  @param  q_flag       [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  msgsz        [in]  message size
 *  @param  msgp         [out] receive message address
 *  @param  rcvmsgsz     [out] receive message size
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_ENOMSG    no message error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi    modify for BS3001
 *  @date   2013/11/11 ALPHA)yokoyama      modify for docomo ZYNQ
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSGQ_RECEIVE(unsigned int q_num, int q_flag,
                              unsigned int msgsz, void **msgp,
                              unsigned int *rcvmsgsz, int *errcd)
{
	int ret;

/* docomo ZYNQ対応 start	*/
	/* message queue idはプロセス識別子が付与されているので外す	*/
	q_num &= 0x0000FFFF;
/* docomo ZYNQ対応 end	*/

    /*****************************************************
    * message receive processing.(call internal routine) *
    *****************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procmsgq_receive2(IPCM_MSGQ_USER, q_num, q_flag, msgsz,
                    msgp, rcvmsgsz, errcd);
/* RRH-001 MD alpha)matsuhashi add end */
	return ret;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Receive2.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  q_num        [in]  queue number
 *  @param  q_flag       [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  msgsz        [in]  message size
 *  @param  msgp         [out] receive message address
 *  @param  rcvmsgsz     [out] receive message size
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_ENOMSG    no message error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procmsgq_receive2(enum ipcm_msgq_type q_type,
			       unsigned int q_num, int q_flag,
			       unsigned int msgsz, void **msgp,
			       unsigned int *rcvmsgsz, int *errcd)
{
    struct ipcm_msgbuf *tmpbuf;
    struct ipcm_msgbuf *msgbuf;
    struct bpf_ru_ipcm_msg_databuf_t *rcvbuf;
    int msgid, msgflg;
    int ret, used;
    int retval = 0;
    unsigned int maxmsglen;

	T_RRH_HEAD*			pComHead;
	unsigned int			uiShKeyId;
	unsigned int			uiShSize;
	int						iShmid;


    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
    IPCM_MSGQ_TYPECHECK(q_type);

    /* キュー番号のチェック */
    if( bpf_ru_ipcm_msgq_numcheck(q_type, q_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq number check NG. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_EPARAM;
    }
    /* フラグのチェック */
    /* MSG_NOERRORはメッセージサイズがバッファより大きいときに切り詰める */
    /* ためのフラグ */
    if( q_flag == BPF_RU_IPCM_WAIT ){           /* 待ちあり */
        msgflg = MSG_NOERROR;                  /* フラグ設定 */
    } else if( q_flag == BPF_RU_IPCM_NOWAIT ){  /* 待ち無し */
        msgflg = MSG_NOERROR | IPC_NOWAIT;     /* フラグ設定 */
    } else {
        IPCM_DBGLOG_FMT_ERR(
            "wait flag check NG. flag = %d",q_flag);
        return BPF_RU_IPCM_EPARAM;                /* パラメータ異常 */
    }

    /********************************************
    * message queue check processing.           *
    ********************************************/
    /* データ取得 */
    bpf_ru_ipcm_msgq_read_data(q_type, q_num, &msgid, &maxmsglen, &used);

    /* 未使用ならキュー番号なしを返す．削除がないのでありえない */
    if( used == IPCM_MSGQ_UNUSED ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq is unused. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_ENOQNUM;
    }

    /* 受信バッファ設定 */
    msgbuf = NULL;
    tmpbuf = NULL;
    /* 送信用領域の確保 */
    bpf_ru_ipcm_msgq_read_rdatabuf(q_type, q_num, &rcvbuf);
    ret = pthread_mutex_lock(&(rcvbuf->mutex));
    if( ret == IPCM_MUTEX_OK ){

		if( rcvbuf->use_flg == BPF_RU_IPCM_FLG_OFF ){
			msgbuf = rcvbuf->data_buf;
			rcvbuf->use_flg = BPF_RU_IPCM_FLG_ON;
		}
		pthread_mutex_unlock(&(rcvbuf->mutex));
	}
	/* 送信用領域が確保できない場合mallocで確保 */
	if( msgbuf == NULL ){
		tmpbuf = malloc(sizeof(struct ipcm_msgbuf)+maxmsglen+1);
		if( tmpbuf == NULL ){
			/* バッファ確保失敗 */
	        IPCM_DBGLOG_FMT_ERR(
	            "No recv buffer. msgq type = %d, num = %u, size = %u",
	            q_type, q_num, msgsz);
			return BPF_RU_IPCM_NG;
		}
	    msgbuf = tmpbuf;
	}

    /********************************************
    * message receive processing.               *
    ********************************************/
    /* メッセージタグ設定(1固定) */
    msgbuf->mtype = IPCM_MSGQ_MSGTYPE;  /* IPCM_MSGQ_MSGTYPE=1 */
    /* メッセージ受信．(maxmsglen+1)としているのはサイズオーバーの検出のため */
    /* 領域自体はmaxmsglen(SG値)+1で確保済 */
    /* maxmsglen+1で取得しサイズーバーなら戻り値もmaxmsglen+1になっているはず */
    /* なのでmsgszがmaxmsglenでもサイズオーバーならretのほうが大きくなる */
    ret = msgrcv(msgid, msgbuf, maxmsglen+1, msgbuf->mtype, msgflg);
    *errcd = errno;
    /* 成功した場合retはコピーしたバイト数なのでNG値で判定 */
    if( ret != IPCM_MSGRCV_NG ){
/* docomo ZYNQ化	Start */

    	/* 以下の流れでメッセージ受信をしてユーザに情報を返す	*/
    	/* ①受信したメッセージからKEYIDを取得					*/
    	/* ②取得したKEYIDから共有メモリを取得					*/
    	/* ③取得共有メモリの先頭をユーザに返す					*/
    	pComHead = ( T_RRH_HEAD *)msgbuf->mtext;
    	/* KEYIDを取得	*/
    	uiShKeyId = pComHead->uiKeyID;

    	/* 共有メモリのサイズを指定しなければならないのでKEYIDからサイズを取得	*/
    	switch(uiShKeyId & 0xFFFF0000)
    	{
		case BPF_RU_IPCM_SHMID_TASK_S:
    		uiShSize = BPF_MESSAGE_SIZE_S;
    		break;
		case BPF_RU_IPCM_SHMID_TASK_M:
    		uiShSize = BPF_MESSAGE_SIZE_M;
    		break;
		case BPF_RU_IPCM_SHMID_TASK_L:
    		uiShSize = BPF_MESSAGE_SIZE_L;
    		break;
    	default:
	        IPCM_DBGLOG_FMT_ERR("Shm Key Error. uiShKeyId = %u",uiShKeyId);
			// if(tmpbuf != NULL){
			//	free(tmpbuf);
			// }
			// return BPF_RU_IPCM_NG;
			/* 2021/02/23 M&C) Update for intermediate device - Add start */
			uiShSize = BPF_MESSAGE_SIZE_S;
			// 2021/02/23 M&C) Update for intermediate device - Add end */
			break;
    	}

    	/* 取得KEYIDから共有メモリ取得	*/
    	iShmid = shmget( uiShKeyId, uiShSize, (IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) );

    	/* 共有メモリをOutParameterに設定	*/
   		*msgp  = shmat(iShmid, NULL, 0);

    	/* 受信メッセージサイズを共有メモリの使用サイズに設定	*/
    	*rcvmsgsz = pComHead->uiLength;
/* docomo ZYNQ化	End */

		/* 受信用バッファの解放 */
		if(tmpbuf != NULL){
			free(tmpbuf);
		}else{
			rcvbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
		}
        return retval;
    }

	/* 受信用バッファの解放 */
	if(tmpbuf != NULL){
		free(tmpbuf);
	}else{
		rcvbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
	}

    /* NGの場合はこちら */
    if(( *errcd != ENOMSG ) && (*errcd != EINTR)){
        IPCM_DBGLOG_FMT_ERR(
          "msgrcv failed. msgq type = %d, num = %u, errno = %d, msgqid = %d, maxmsglen+1 = %d, msgflg = %d",
          q_type, q_num, *errcd, msgid, maxmsglen+1, msgflg);
    }

    /********************************************
    * result code setting processing.           *
    ********************************************/
    switch(*errcd){
        case E2BIG:   /* これはMSG_NOERRORが設定されている場合は発生しない？ */
            return BPF_RU_IPCM_ESIZEOVER;
            break;
        case ENOMSG:  /* IPC_NOWAIT指定でメッセージがなかった場合 */
        case EAGAIN:  /* IPC_NOWAIT指定でメッセージがなかった場合 */
            return BPF_RU_IPCM_ENOMSG;
            break;
        case EFAULT:  /*  msgbufのアドレスが不正(これがでたらミドルのバグ) */
            return BPF_RU_IPCM_EADDR;
            break;
        case EINTR:   /* シグナルによる中断 */
            return BPF_RU_IPCM_EINTR;
            break;
        default: /* それ以外のNG */
            return BPF_RU_IPCM_NG;
    }

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Free.
 *  @note   BPF internal function.
 *  @param  q_num_org        [in]  queue number
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2014/01/04 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSGQ_FREE_STACK_BUFF(unsigned int q_num_org)
{
	struct ipcm_msgbuf *tmpbuf;
	struct ipcm_msgbuf *msgbuf;
	struct bpf_ru_ipcm_msg_databuf_t *rcvbuf;
	int msgid, msgflg;
	int ret, used;
	unsigned int maxmsglen;
	unsigned int q_num;

	T_RRH_HEAD*			pComHead;
	unsigned int			uiShKeyId;
	unsigned int			uiShSize;
	int						iShmid;
	unsigned int			count;
	unsigned int			queue_cnt;
	void					*freebuf;
	int						errcd;
	/********************************************
	* parameter check processing.               *
	********************************************/

	/* 使用するQueue番号は下4桁のみ	*/
	q_num = q_num_org & 0x0000FFFF;

	/* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
	IPCM_MSGQ_TYPECHECK(IPCM_MSGQ_USER);

	/* キュー番号のチェック */
	if( bpf_ru_ipcm_msgq_numcheck(IPCM_MSGQ_USER, q_num) != BPF_RU_IPCM_OK ){
		IPCM_DBGLOG_FMT_ERR(
			"msgq number check NG. msgq type = %d, num = %u",
			IPCM_MSGQ_USER, q_num);
		return BPF_RU_IPCM_EPARAM;
	}

	msgflg = MSG_NOERROR | IPC_NOWAIT;     /* フラグ設定 */

	/********************************************
	* message queue check processing.           *
	********************************************/
	/* データ取得 */
	bpf_ru_ipcm_msgq_read_data(IPCM_MSGQ_USER, q_num, &msgid, &maxmsglen, &used);

	/* 未使用ならキュー番号なしを返す．削除がないのでありえない */
	if( used == IPCM_MSGQ_UNUSED ){
		IPCM_DBGLOG_FMT_ERR(
			"msgq is unused. msgq type = %d, num = %u",
			IPCM_MSGQ_USER, q_num);
		return BPF_RU_IPCM_ENOQNUM;
	}

	/* 受信バッファ設定 */
	msgbuf = NULL;
	tmpbuf = NULL;
	/* 送信用領域の確保 */
	bpf_ru_ipcm_msgq_read_rdatabuf(IPCM_MSGQ_USER, q_num, &rcvbuf);
	ret = pthread_mutex_lock(&(rcvbuf->mutex));
	if( ret == IPCM_MUTEX_OK ){

		if( rcvbuf->use_flg == BPF_RU_IPCM_FLG_OFF ){
			msgbuf = rcvbuf->data_buf;
			rcvbuf->use_flg = BPF_RU_IPCM_FLG_ON;
		}
		pthread_mutex_unlock(&(rcvbuf->mutex));
	}
	/* 送信用領域が確保できない場合mallocで確保 */
	if( msgbuf == NULL ){
		tmpbuf = malloc(sizeof(struct ipcm_msgbuf)+maxmsglen+1);
		if( tmpbuf == NULL ){
			/* バッファ確保失敗 */
			IPCM_DBGLOG_FMT_ERR(
				"No recv buffer. msgq type = %d, num = %u",
				IPCM_MSGQ_USER, q_num);
			return BPF_RU_IPCM_NG;
		}
		msgbuf = tmpbuf;
	}

	/********************************************
	* message receive processing.               *
	********************************************/
	/* メッセージタグ設定(1固定) */
	msgbuf->mtype = IPCM_MSGQ_MSGTYPE;  /* IPCM_MSGQ_MSGTYPE=1 */

	/* 溜まっているMessageを刈り取って破棄していく	*/
	ret = BPF_RU_IPCM_PROCMSGQ_NUMMSGS( q_num, &queue_cnt, &errcd );
	if( ret == BPF_RU_IPCM_OK )
	{
		for( count = 0; count < queue_cnt; count++ )
		{
			ret = msgrcv(msgid, msgbuf, maxmsglen+1, msgbuf->mtype, msgflg);

			if( ret != IPCM_MSGRCV_NG )
			{
				pComHead = ( T_RRH_HEAD *)msgbuf->mtext;
				/* KEYIDを取得	*/
				uiShKeyId = pComHead->uiKeyID;
				/* 共有メモリのサイズを指定しなければならないのでKEYIDからサイズを取得	*/
				switch(uiShKeyId & 0xFFFF0000){
				case BPF_RU_IPCM_SHMID_TASK_S:
					uiShSize = BPF_MESSAGE_SIZE_S;
					break;
				case BPF_RU_IPCM_SHMID_TASK_M:
					uiShSize = BPF_MESSAGE_SIZE_M;
					break;
				case BPF_RU_IPCM_SHMID_TASK_L:
					uiShSize = BPF_MESSAGE_SIZE_L;
					break;
				default:
					IPCM_DBGLOG_FMT_ERR("Shm Key Error. uiShKeyId = %u",uiShKeyId);
					if(tmpbuf != NULL){
						free(tmpbuf);
					}
					return BPF_RU_IPCM_NG;
					break;
				}

				/* 取得KEYIDから共有メモリ取得	*/
				iShmid = shmget( uiShKeyId, uiShSize, (IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) );

		    	/* 共有メモリを解放Buffer用に設定	*/
		   		freebuf = shmat(iShmid, NULL, 0);

				/* バッファ解放	*/
				(void)BPF_RU_IPCM_PROCMSG_ADDRFREE( freebuf );
			}
		}
	}


	/* 受信用バッファの解放 */
	if(tmpbuf != NULL){
		free(tmpbuf);
	}else{
		rcvbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
	}

	return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Receive(時間指定有り).
 *  @note   BPF internal function.
 *  @param  q_num        [in]  queue number
 *  @param  q_flag       [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  msgsz        [in]  message size
 *  @param  msgsz        [in]  time_value(sec)
 *  @param  msgp         [out] receive message address
 *  @param  rcvmsgsz     [out] receive message size
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_ENOMSG    no message error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/10/17 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSGQ_TIMER_RECEIVE(unsigned int q_num, int q_flag,
									  unsigned int msgsz, unsigned int uiTime, void **msgp,
									  unsigned int *rcvmsgsz, int *errcd)
{
	struct ipcm_msgbuf *tmpbuf;
	struct ipcm_msgbuf *msgbuf;
	struct bpf_ru_ipcm_msg_databuf_t *rcvbuf;
	int msgid, msgflg;
	int ret, used;
	int retval = 0;
	unsigned int maxmsglen;

	T_RRH_HEAD*			pComHead;
	unsigned int			uiShKeyId;
	unsigned int			uiShSize;
	int						iShmid;
	unsigned int			count;
    struct timespec			dtime;                         /* 中断時間               */
    struct timespec			rem;                           /* 残り中断時間           */

	/********************************************
	* parameter check processing.               *
	********************************************/

	/* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
	IPCM_MSGQ_TYPECHECK(IPCM_MSGQ_USER);

	/* キュー番号のチェック */
	if( bpf_ru_ipcm_msgq_numcheck(IPCM_MSGQ_USER, q_num) != BPF_RU_IPCM_OK ){
		IPCM_DBGLOG_FMT_ERR(
			"msgq number check NG. msgq type = %d, num = %u",
			IPCM_MSGQ_USER, q_num);
		return BPF_RU_IPCM_EPARAM;
	}
	/* フラグのチェック */
	/* MSG_NOERRORはメッセージサイズがバッファより大きいときに切り詰める */
	/* ためのフラグ */
	if( q_flag == BPF_RU_IPCM_WAIT ){           /* 待ちあり */
		msgflg = MSG_NOERROR;                  /* フラグ設定 */
	} else if( q_flag == BPF_RU_IPCM_NOWAIT ){  /* 待ち無し */
		msgflg = MSG_NOERROR | IPC_NOWAIT;     /* フラグ設定 */
	} else {
		IPCM_DBGLOG_FMT_ERR(
			"wait flag check NG. flag = %d",q_flag);
		return BPF_RU_IPCM_EPARAM;                /* パラメータ異常 */
	}

	/********************************************
	* message queue check processing.           *
	********************************************/
	/* データ取得 */
	bpf_ru_ipcm_msgq_read_data(IPCM_MSGQ_USER, q_num, &msgid, &maxmsglen, &used);

	/* 未使用ならキュー番号なしを返す．削除がないのでありえない */
	if( used == IPCM_MSGQ_UNUSED ){
		IPCM_DBGLOG_FMT_ERR(
			"msgq is unused. msgq type = %d, num = %u",
			IPCM_MSGQ_USER, q_num);
		return BPF_RU_IPCM_ENOQNUM;
	}

	/* 受信バッファ設定 */
	msgbuf = NULL;
	tmpbuf = NULL;
	/* 送信用領域の確保 */
	bpf_ru_ipcm_msgq_read_rdatabuf(IPCM_MSGQ_USER, q_num, &rcvbuf);
	ret = pthread_mutex_lock(&(rcvbuf->mutex));
	if( ret == IPCM_MUTEX_OK ){

		if( rcvbuf->use_flg == BPF_RU_IPCM_FLG_OFF ){
			msgbuf = rcvbuf->data_buf;
			rcvbuf->use_flg = BPF_RU_IPCM_FLG_ON;
		}
		pthread_mutex_unlock(&(rcvbuf->mutex));
	}
	/* 送信用領域が確保できない場合mallocで確保 */
	if( msgbuf == NULL ){
		tmpbuf = malloc(sizeof(struct ipcm_msgbuf)+maxmsglen+1);
		if( tmpbuf == NULL ){
			/* バッファ確保失敗 */
			IPCM_DBGLOG_FMT_ERR(
				"No recv buffer. msgq type = %d, num = %u, size = %u",
				IPCM_MSGQ_USER, q_num, msgsz);
			return BPF_RU_IPCM_NG;
		}
		msgbuf = tmpbuf;
	}

	/********************************************
	* message receive processing.               *
	********************************************/
	/* メッセージタグ設定(1固定) */
	msgbuf->mtype = IPCM_MSGQ_MSGTYPE;  /* IPCM_MSGQ_MSGTYPE=1 */

	/* sleep時間設定(100μ)	*/
	dtime.tv_sec = 0;
	dtime.tv_nsec = 100000;

	/* 100μ周期で監視する	*/
	for( count = 0; count < uiTime*10; count++ )
	{
		/* メッセージ受信．(maxmsglen+1)としているのはサイズオーバーの検出のため */
		/* 領域自体はmaxmsglen(SG値)+1で確保済 */
		/* maxmsglen+1で取得しサイズーバーなら戻り値もmaxmsglen+1になっているはず */
		/* なのでmsgszがmaxmsglenでもサイズオーバーならretのほうが大きくなる */
		ret = msgrcv(msgid, msgbuf, maxmsglen+1, msgbuf->mtype, msgflg);

		if( ret != IPCM_MSGRCV_NG )
		{
			break;
		}

		/* 1msec wait	*/
		nanosleep( &dtime, &rem);
	}

	/* タイムアウトした場合は明示的に設定する	*/
	if( count == uiTime )
	{
		*errcd = ENOMSG;
	}
	else
	{
		*errcd = errno;
	}

	/* 成功した場合retはコピーしたバイト数なのでNG値で判定 */
	if( ret != IPCM_MSGRCV_NG ){
/* docomo ZYNQ化	Start */
    	/* 以下の流れでメッセージ受信をしてユーザに情報を返す	*/
    	/* ①受信したメッセージからKEYIDを取得					*/
    	/* ②取得したKEYIDから共有メモリを取得					*/
    	/* ③取得共有メモリの先頭をユーザに返す					*/
    	pComHead = ( T_RRH_HEAD *)msgbuf->mtext;
    	/* KEYIDを取得	*/
    	uiShKeyId = pComHead->uiKeyID;

    	/* 共有メモリのサイズを指定しなければならないのでKEYIDからサイズを取得	*/
		switch(uiShKeyId & 0xFFFF0000){
		case BPF_RU_IPCM_SHMID_TASK_S:
			uiShSize = BPF_MESSAGE_SIZE_S;
			break;
		case BPF_RU_IPCM_SHMID_TASK_M:
			uiShSize = BPF_MESSAGE_SIZE_M;
			break;
		case BPF_RU_IPCM_SHMID_TASK_L:
			uiShSize = BPF_MESSAGE_SIZE_L;
			break;
		default:
			IPCM_DBGLOG_FMT_ERR("Shm Key Error. uiShKeyId = %u",uiShKeyId);
			if(tmpbuf != NULL){
				free(tmpbuf);
			}
			return BPF_RU_IPCM_NG;
			break;
		}

    	/* 取得KEYIDから共有メモリ取得	*/
    	iShmid = shmget( uiShKeyId, uiShSize, (IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) );

    	/* 共有メモリをOutParameterに設定	*/
   		*msgp  = shmat(iShmid, NULL, 0);

    	/* 受信メッセージサイズを共有メモリの使用サイズに設定	*/
    	*rcvmsgsz = pComHead->uiLength;
/* docomo ZYNQ化	End */
		/* 受信用バッファの解放 */
		if(tmpbuf != NULL){
			free(tmpbuf);
		}else{
			rcvbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
		}
		return retval;
	}

	/* 受信用バッファの解放 */
	if(tmpbuf != NULL){
		free(tmpbuf);
	}else{
		rcvbuf->use_flg = BPF_RU_IPCM_FLG_OFF;
	}

	/* NGの場合はこちら */
	if( *errcd != ENOMSG ){
		IPCM_DBGLOG_FMT_ERR(
			"msgrcv failed. msgq type = %d, num = %u, errno = %d, msgqid = %d, maxmsglen+1 = %d, msgflg = %d",
			IPCM_MSGQ_USER, q_num, *errcd, msgid, maxmsglen+1, msgflg);
	}

	/********************************************
	* result code setting processing.           *
	********************************************/
	switch(*errcd){
		case E2BIG:   /* これはMSG_NOERRORが設定されている場合は発生しない？ */
			*msgp = 0;
			return BPF_RU_IPCM_ESIZEOVER;
			break;
		case ENOMSG:  /* IPC_NOWAIT指定でメッセージがなかった場合 */
		case EAGAIN:  /* IPC_NOWAIT指定でメッセージがなかった場合 */
			*msgp = 0;
			return BPF_RU_IPCM_ENOMSG;
			break;
		case EFAULT:  /*  msgbufのアドレスが不正(これがでたらミドルのバグ) */
			return BPF_RU_IPCM_EADDR;
			*msgp = 0;
			break;
		case EINTR:   /* シグナルによる中断 */
			*msgp = 0;
			return BPF_RU_IPCM_EINTR;
			break;
		default: /* それ以外のNG */
			*msgp = 0;
			return BPF_RU_IPCM_NG;
	}

}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Number of Messages Get.
 *  @note   API function.
 *  @param  q_num        [in]  queue number
 *  @param  msgnum       [out] number of messages
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSGQ_NUMMSGS(unsigned int q_num, unsigned int *msgnum,
                              int *errcd)
{
	int ret;
    /***********************************************************
    * number of message get processing.(call internal routine) *
    ***********************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procmsgq_nummsgs2(IPCM_MSGQ_USER, q_num, msgnum, errcd);
/* RRH-001 MD alpha)matsuhashi add end */
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Number of Messages Get 2.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  q_num        [in]  queue number
 *  @param  msgnum       [out] number of messages
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procmsgq_nummsgs2(enum ipcm_msgq_type q_type,
			       unsigned int q_num, unsigned int *msgnum,
			       int *errcd)
{
  int msgid;
  int ret, used;
  struct msqid_ds mqds;
  unsigned int maxmsglen;


    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
    IPCM_MSGQ_TYPECHECK(q_type);

    /* メッセージキュー番号チェック */
    if( bpf_ru_ipcm_msgq_numcheck(q_type, q_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq number check NG. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_EPARAM;
    }

    /********************************************
    * message queue check processing.           *
    ********************************************/
    /* 管理データ読み込み */
    bpf_ru_ipcm_msgq_read_data(q_type, q_num, &msgid, &maxmsglen, &used);

    /* 未使用ならキュー番号なしを返す．削除がないのでありえない */
    if( used == IPCM_MSGQ_UNUSED ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq is unused. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_ENOQNUM;
    }

    /********************************************
    * message queue information get processing. *
    ********************************************/
    /* メッセージキュー情報取得 */
    ret = msgctl(msgid, IPC_STAT, &mqds);
    *errcd = errno;
    if( ret == IPCM_MSGCTL_NG ){  /* -1の時 */
            IPCM_DBGLOG_FMT_ERR(
            "msgctl failed. msgqid = %d, errno = %d",
    	msgid, *errcd);
        return BPF_RU_IPCM_NG;
    }
    /********************************************
    * output parameter set processing.          *
    ********************************************/
    /* キュー保留数格納 */
    *msgnum = mqds.msg_qnum;
    return BPF_RU_IPCM_OK;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Message Queue Create.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  q_num        [in]  queue number
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procmsgq_create(enum ipcm_msgq_type q_type,
			     unsigned int q_num, int *errcd)
{
  int msgqid, max, keymask, flg;
  enum ipcm_msgq_type type;

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* メッセージキュータイプのチェック(異常時はこの中からEPARAMを返す) */
    IPCM_MSGQ_TYPECHECK(q_type);

    /********************************************
    * message queue check processing.           *
    ********************************************/
    /* メッセージキュー番号範囲チェック */
    if( bpf_ru_ipcm_msgq_numcheck(q_type, q_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "msgq number check NG. msgq type = %d, num = %u",
            q_type, q_num);
        return BPF_RU_IPCM_EPARAM;
    }

    /* 管理テーブル取得 */
    bpf_ru_ipcm_msgq_read_tbl(q_type, &type, &max, &keymask, &flg);

    /********************************************
    * message queue create processing.          *
    ********************************************/
    /* メッセージキュー作成 */
    msgqid = msgget( keymask | q_num,  flg);
    *errcd = errno;
    if( msgqid == IPCM_MSGGET_NG ){ /* 戻り値-1で作成失敗 */
        IPCM_DBGLOG_FMT_ERR(
            "msgget failed. keymask = %08x, num = %u, flg = %d, errno = %d",
            keymask, q_num, flg, *errcd);
        return BPF_RU_IPCM_NG;
    }
    /********************************************
    * management table update processing.       *
    ********************************************/
    /* 管理テーブル更新 */
    bpf_ru_ipcm_msgq_write_data(q_type, q_num, msgqid, IPCM_MSGQ_USED);
    return BPF_RU_IPCM_OK;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Real time log get.
 *  @note   BPF internal function.
 *  @param  srcPID       [in]  src Process ID
 *  @param  srcTID       [in]  src Thread ID
 *  @param  dstPID       [in]  dst Process ID
 *  @param  dstTID       [in]  dst Thread ID
 *  @param  msgp         [in]  send message address
 *  @return none
 *  @date   2013/11/14 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
void bpf_ru_ipcm_real_time_log_get(	unsigned int srcPID,
									unsigned int srcTID,
									unsigned int dstPID,
									unsigned int dstTID,
									const void   *msgp )
{
	t_rrupdump_msg		ipmsg;
	struct sockaddr_in	sin;
	struct in_addr		src;
	short				headlen = sizeof(struct ip) + 4 ;
	short				totallen;
	int					datalen;
	char				ipaddr_src[16];
	char				ipaddr_dst[16];
	int					socId;
	int					onoff=1;


	/* TIDがおかしい場合は254に設定しておく	*/
	if( (srcTID == 0) || (srcTID >= 0xFF) )
	{
		srcTID = 254;
	}
	if( (dstTID == 0) || (dstTID >= 0xFF) )
	{
		dstTID = 254;
	}
	datalen = ((T_RRH_HEAD *)msgp)->uiLength;
	if(datalen <= 0)
	{
		return;
	}
	if((((T_RRH_HEAD *)msgp)->uiSignalkind) == D_DU_IFLOG_OFF){
		return;
	}
	if( ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_PTPSTSTO_NTC)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_SYNCESTSTO_NTC)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_SYNCSTSTO_NTC)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_PFM_1S_TIMEOUT_NTF)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_EIO_EXTOUTSV_TO)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_EIO_FANSVCTRL_TO)
	 || ((((T_RRH_HEAD *)msgp)->uiEventNo) == D_DU_MSGID_PFM_LOG_TIMEOUT_NTF)
	  ){
		return;
	}

	if( ( datalen + headlen ) > D_IPCM_REALPACKET_MAXSIZE )
	{
		datalen = (int)( D_IPCM_REALPACKET_MAXSIZE - headlen );
	}
	/* Raw socket Setting */
	/* 送信先IPアドレス取得	*/
	sprintf( ipaddr_src, "127.0.%d.%d", srcPID, srcTID );
	sprintf( ipaddr_dst, "127.0.%d.%d", dstPID, dstTID );

	memset(&sin, 0, sizeof(struct sockaddr_in));
	src.s_addr			= inet_addr(ipaddr_src);
	sin.sin_family		= PF_INET;
	sin.sin_addr.s_addr	= inet_addr(ipaddr_dst);
	sin.sin_port		= 0;

	totallen = headlen + (short)datalen;

	ipmsg.iphead.ip_v	        = 4;				/* ipV4 */
	ipmsg.iphead.ip_hl         = headlen/4;
	ipmsg.iphead.ip_tos        = 0;
	ipmsg.iphead.ip_len        = htons(totallen);
	ipmsg.iphead.ip_id         = htons(0x9999);	/* freeID */
	ipmsg.iphead.ip_off        = htons(0x4000);	/* dont flagment */
	ipmsg.iphead.ip_ttl        = 255;
	ipmsg.iphead.ip_p          = IPPROTO_RAW;
	ipmsg.iphead.ip_sum        = 0;
	ipmsg.iphead.ip_src.s_addr = src.s_addr;
	ipmsg.iphead.ip_dst.s_addr = sin.sin_addr.s_addr;

	/* IP Data Setting */
	memcpy( (ipmsg.data), msgp, datalen );

	if(( socId = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) != -1){
		if((setsockopt(socId, IPPROTO_IP, IP_HDRINCL, &onoff, sizeof(int))) != -1){
			(void)sendto(socId, &ipmsg, totallen, 0, (struct sockaddr *) &sin, sizeof(sin));
		}
		close(socId);
	}

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Communication Management Message Queue Send.
 *  @note   API function.
 *  @param  q_num        [in]  queue number(process / thread)
 *  @param  msgp         [in]  send message address
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_EQFULL    queue full error
 *  @retval BPF_RU_IPCM_ENOSPACE  no message space error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/11/11 ALPHA)yokoyama    docomo ZYNQ対応
 *  @date   2014/12/12 ALPHA)yokoyama    docomo SLC/1CHIP対応
 *  @date   2015/10/22 ALPHA)ueda modify for TDD-RRE(zynq)
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_MSGQ_SEND(	unsigned int	q_num,
							void			*msgp )
{
	int						ret;
	int						errcd;
	int						msgsz = sizeof(T_RRH_HEAD);		/* 送信する実際のサイズはヘッダ部のみのため、32byte固定	*/
	unsigned int			srcPID = 0;
	int						dstPID = 0;
	unsigned int			srcTID = 0;
	int						dstTID = 254;
	unsigned int			count;
	T_RRH_PID_MNG*			pidMng;
	T_RRH_TID_MNG*			threadinfoP;
	unsigned char			buf[D_RRH_BUFSIZE_4096];			/* PAGE_SIZE */
	unsigned int			logskipflg;

		unsigned int q_num_tmp;
		q_num_tmp=q_num;
	/********************************************************************/
	/* 共有メモリが有効(該当プロセスにattachされている)かをチェック		*/
	/********************************************************************/
	if(mincore(msgp,1,buf))
	{
		/* not attach case  */
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "not attach case : queueno = %d, errno = %d",
							q_num, BPF_RU_IPCM_ENOTATTACH );
		return BPF_RU_IPCM_ENOTATTACH;
	}

	logskipflg = D_RRH_ON;
	/********************************************************************/
	/* 送信元プロセスID/スレッドID取得									*/
	/********************************************************************/
	(VOID)bpf_rm_svrm_thread_info_get( &srcPID, &srcTID );

	/* QueueIDによってプロセス間送信かスレッド間送信かをわける	*/
	if( ( q_num & 0xFFFF0000 ) != 0 )
	{
		/* プロセス識別子が付与されていればプロセス間送信	*/

		/* ログ取得対象? */
		if(logskipflg == D_RRH_ON )
		{
			/* PID管理テーブル取得 */
			(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
				(VOID **)&pidMng,
				&errcd
			);

			/********************************************************************/
			/* 送信先プロセスID取得												*/
			/********************************************************************/
			/* QueueIDからプロセス識別子を取得して判断する	*/
			/* プロセスIDを一律254(debug用)にしておく	*/
			dstPID = 254;

			for( count = 0; count < pidMng->count; count++ )
			{
				/* Queueの識別子が一致したらプロセス該当あり	*/
				if( ( q_num & 0xFFFF0000 ) == ( pidMng->info[ count ].PQID & 0xFFFF0000 ) )
				{
					dstPID = pidMng->info[ count ].aplProcessID;
					break;
				}
			}

			/********************************************************************/
			/* 送信先スレッドID取得												*/
			/********************************************************************/
			dstTID = 254;

			/* リアルタイムログを取得する	*/
			/* Process間通信の場合、送信先ThreadIDは不明であるため、			*/
			/* 送信先ThreadIDはゴミ値を設定する									*/
			/* ただし、末尾が0または255は挙動がおかしくなる可能性があるため、	*/
			/* 254を設定することとする											*/
			bpf_ru_ipcm_real_time_log_get( srcPID, srcTID, dstPID, dstTID, msgp );
		}

		/* 実際に送信するためにはプロセス識別子を外す	*/
		printf("\n[INFO][MT][%s:%d] func: %s, PROCESS SEND, uiEventNo= 0x%x, uiSrcPQueueID =0x%x, uiDstPQueueID =0x%x, q_num =0x%x",__FILE__,__LINE__ ,__func__,(((T_RRH_HEAD *)msgp)->uiEventNo),((T_RRH_HEAD *)msgp)->uiDstPQueueID,((T_RRH_HEAD *)msgp)->uiDstPQueueID,q_num_tmp);
		q_num &= 0x0000FFFF;
		/*****************************************************
		* message send PROCESS.(call internal routine)       *
		*****************************************************/
		ret = BPF_RU_IPCM_PROCMSGQ_SEND( q_num, msgp, msgsz, BPF_RU_IPCM_WAIT, &errcd);
	}
	else
	{
		/* プロセス識別子が付与されていなければスレッド間送信	*/

		/* ログ取得対象? */
		if(logskipflg == D_RRH_ON )
		{
			/********************************************************************/
			/* 送信先プロセスID取得												*/
			/********************************************************************/
			/* スレッド間送信なので同一プロセスである	*/
			dstPID = srcPID;
			if( dstPID < 10)
			{
				/********************************************************************/
				/* 送信先スレッドID取得												*/
				/********************************************************************/
				/* スレッドID管理テーブル取得 */
				(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_THRD_ID_MNG_INFO,
					(VOID **)&threadinfoP,
					&errcd
				);

				/* 送信先ThreadQueueIDからThreadIDを導き出す	*/
				for( count = 0; count < threadinfoP->infoP[ dstPID ].count; count++ )
				{
					/* 送信先ThreadQueueIDが一致したらAPL上のThreadIDを取得する	*/
					if( threadinfoP->infoP[ dstPID ].infoT[ count ].TQID == q_num )
					{
						dstTID = threadinfoP->infoP[ dstPID ].infoT[ count ].aplThreadID;
						break;
					}
				}
				/* リアルタイムログを取得する								*/
				/* 末尾(TID)が0の場合、挙動がおかしくなる可能性があるため	*/
				/* TIDには+1をして渡す										*/
				bpf_ru_ipcm_real_time_log_get( srcPID, srcTID, dstPID, dstTID, msgp );
			}
			else
			{
				IPCM_DBGLOG_FMT_ERR("dstPID(%d) was strange!! srcT:%d dstT:%d",dstPID,srcTID,dstTID);
			}
		}
		/*****************************************************
		* message send THREAD.(call internal routine)        *
		*****************************************************/
	if(!((q_num_tmp ==0x2 && ((T_RRH_HEAD *)msgp)->uiDstPQueueID == 0x0 && (((T_RRH_HEAD *)msgp)->uiEventNo) == 0x1601) ||
		(q_num_tmp ==0x19 && ((T_RRH_HEAD *)msgp)->uiDstPQueueID == 0x0 && (((T_RRH_HEAD *)msgp)->uiEventNo ==0x10170003) )))
		printf("\n[INFO][MT][%s:%d] func: %s, THREAD SEND, uiEventNo= 0x%x, uiSrcPQueueID =0x%x, uiDstPQueueID =0x%x, q_num =0x%x", __FILE__, __LINE__, __func__, (((T_RRH_HEAD *)msgp)->uiEventNo), ((T_RRH_HEAD *)msgp)->uiSrcPQueueID, ((T_RRH_HEAD *)msgp)->uiDstPQueueID, q_num_tmp);
		ret = BPF_RU_ITCM_THREAD_MSGQ_SEND( q_num, msgsz, msgp, BPF_RU_ITCM_WAIT, 0 );
	}

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Communication Management Message Queue Recieve.
 *  @note   API function.
 *  @param  q_num        [in]  queue number(process / thread)
 *  @param  wait_time    [in]  wait time(プロセス間Messageの場合にのみ有効)
 *  @param  *msgp        [out] recieve message address
 *  @param  *msgsize     [out] recieve message size
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @retval BPF_RU_IPCM_EPARAM    parameter error
 *  @retval BPF_RU_IPCM_ESIZEOVER message size over error
 *  @retval BPF_RU_IPCM_ENOQNUM   queue number error
 *  @retval BPF_RU_IPCM_EQFULL    queue full error
 *  @retval BPF_RU_IPCM_ENOSPACE  no message space error
 *  @retval BPF_RU_IPCM_EADDR     address error
 *  @retval BPF_RU_IPCM_EINTR     signal interupt error
 *  @date   2013/11/11 ALPHA)yokoyama    docomo ZYNQ対応
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_MSGQ_RECV(	unsigned int	q_num,
							unsigned int	wait_time,
							void			**msgp,
							unsigned int	*msgsize )
{
	int				errcd;
	int				ret;

		unsigned int q_num_tmp;
		q_num_tmp=q_num;
	/* QueueIDによってプロセス間受信かスレッド間受信かをわける	*/
	if( ( q_num & 0xFFFF0000 ) != 0 )
	{
		/* プロセス識別子が付与されていればプロセス間送信	*/

		/* 実際に受信するためにはプロセス識別子を外す	*/
		q_num &= 0x0000FFFF;

		/* 待ち時間が指定されているか否かで受信関数を分ける	*/
		if( 0 == wait_time )
		{
			/* 通常の待ち受け	*/
			ret = BPF_RU_IPCM_PROCMSGQ_RECEIVE( q_num,
												BPF_RU_IPCM_WAIT,
												sizeof(T_RRH_HEAD),
												msgp,
												msgsize,
												&errcd);
			printf("\n[INFO][MT][%s:%d] func: %s, PROCCESS RECEIVE, uiEventNo= 0x%x, uiSrcPQueueID=0x%x, uiDstPQueueID=0x%x,q_num =0x%x", __FILE__, __LINE__, __func__, (((T_RRH_HEAD *)msgp)->uiEventNo), ((T_RRH_HEAD *)msgp)->uiSrcPQueueID, ((T_RRH_HEAD *)msgp)->uiDstPQueueID, q_num_tmp);
		}
		else
		{
			/* 時間指定の待ち受け	*/
			ret = BPF_RU_IPCM_PROCMSGQ_TIMER_RECEIVE(	q_num,
														BPF_RU_IPCM_NOWAIT,
														sizeof(T_RRH_HEAD),
														wait_time,
														msgp,
														msgsize,
														&errcd);
			printf("\n[INFO][MT][%s:%d] func: %s, TIMER RECEIVE, uiEventNo= 0x%x, uiSrcPQueueID=0x%x, uiDstPQueueID=0x%x, q_num =0x%x", __FILE__, __LINE__, __func__, (((T_RRH_HEAD *)msgp)->uiEventNo), ((T_RRH_HEAD *)msgp)->uiSrcPQueueID, ((T_RRH_HEAD *)msgp)->uiDstPQueueID, q_num_tmp);
		}
	}
	else
	{
		/* プロセス識別子が付与されていなければスレッド間受信	*/

		/* スレッド間は常に無限待ちのはず	*/
		ret = BPF_RU_ITCM_THREAD_MSGQ_RECEIVE(	q_num,
												sizeof(T_RRH_HEAD),
												msgp,
												msgsize,
												BPF_RU_ITCM_WAIT,
												NULL );
		if (!((q_num_tmp == 0x2 && ((T_RRH_HEAD *)msgp)->uiDstPQueueID == 0x0 && (((T_RRH_HEAD *)msgp)->uiEventNo) == 0x1601) ||
			(q_num_tmp == 0x2 && ((T_RRH_HEAD *)msgp)->uiDstPQueueID == 0x101 && ((T_RRH_HEAD *)msgp)->uiSrcPQueueID == 0x101)
			))
			printf("\n[INFO][MT][%s:%d] func: %s, THREAD RECEIVE, uiEventNo=0x%x , uiSrcPQueueID=0x%x, uiDstPQueueID =0x%x, q_num =0x%x", __FILE__, __LINE__, __func__, (((T_RRH_HEAD *)msgp)->uiEventNo), ((T_RRH_HEAD *)msgp)->uiSrcPQueueID, ((T_RRH_HEAD *)msgp)->uiDstPQueueID, q_num_tmp);
	}

	return ret;
}
/* @} */

/* @} */
