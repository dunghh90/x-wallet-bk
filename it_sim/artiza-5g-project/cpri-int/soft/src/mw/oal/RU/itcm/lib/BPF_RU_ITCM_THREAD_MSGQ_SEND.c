/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_MSGQ_SEND.c
 *	@brief	Inter Thread Communication Management Library(MSGQ).
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_ITCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_itcm.h"
#include "bpf_i_ru_itcm_macro.h"

/** @addtogroup BPF_RU_ITCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Message Queue Send.
 *  @note   API function.
 *  @param  thr_q_num    [in]  queue number
 *  @param  size         [in]  send message size
 *  @param  msgp         [in]  send message address
 *  @param  thr_q_flag   [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ENOSEMGET semaphore get error
 *  @retval BPF_RU_ITCM_ENOSPACE  no message space error
 *  @retval BPF_RU_ITCM_ESIZEOVER message size over error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @retval BPF_RU_ITCM_EQFULL    queue full error
 *  @retval BPF_RU_ITCM_ETIMEOUT  timeout error
 *  @retval BPF_RU_ITCM_EADDR     address error
 *  @retval BPF_RU_ITCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_MSGQ_SEND(unsigned int thr_q_num, unsigned int size,
									const void *msgp, int thr_q_flag,
									struct bpf_timespec *timeout)
{
	/*****************************************************
	* message send processing.(call internal routine)    *
	*****************************************************/
	return bpf_ru_itcm_thread_msgq_send2(ITCM_MSGQ_USER, thr_q_num, size, msgp, thr_q_flag, timeout);

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Message Queue Send2.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  thr_q_num    [in]  queue number
 *  @param  size         [in]  send message size
 *  @param  msgp         [in]  send message address
 *  @param  thr_q_flag   [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ENOSEMGET semaphore get error
 *  @retval BPF_RU_ITCM_ENOSPACE  no message space error
 *  @retval BPF_RU_ITCM_ESIZEOVER message size over error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @retval BPF_RU_ITCM_EQFULL    queue full error
 *  @retval BPF_RU_ITCM_ETIMEOUT  timeout error
 *  @retval BPF_RU_ITCM_EADDR     address error
 *  @retval BPF_RU_ITCM_EINTR     signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_itcm_thread_msgq_send2(enum itcm_msgq_type q_type,
									unsigned int thr_q_num, unsigned int size, /* pgr0339 */
									const void *msgp, int thr_q_flag,
									struct bpf_timespec *timeout)
{
	int ret, retcode = BPF_RU_ITCM_OK;
	struct itcm_msg_data_t *msg_data = NULL;
	struct itcm_msg_t *msg = NULL;
	struct bpf_timespec dtime;
	int errcd = 0;
	void *buff = NULL;		/* pgr0060 */
	
/* docomo ZYNQ対応	start */
	/* 通信の方式は以下の通りである。							*/
	/* 共有メモリに個別データを設定し							*/
	/* 実際には共通ヘッダのみを送信する							*/
	/* そのため、メッセージバッファにコピーするのは共通ヘッダ部のみである	*/
	size = sizeof(t_bpf_ru_itcm_msgq_expand);		/* Header(32byte)	*/
/* docomo ZYNQ対応 end	*/
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	/* Invalid cases, Macro return  */
	ITCM_MSGQ_TYPECHECK(q_type);		/* Msgq Type Check      */

	ITCM_MSGQ_NUMCHECK(thr_q_num);		/* Msgq number Check    */

	ITCM_MSGQ_MSGSZCHECK(thr_q_num, size);	/* Msg size Check   */

	ITCM_MSGQ_MSGPCHECK(size, msgp);		/* Msgpointer Check */

	ITCM_MSGQ_FLAGCHECK(thr_q_flag);		/* Wait Flag Check  */

	ITCM_MSGQ_USEDCHECK(thr_q_num);			/* Msgq used status Check */


	msg_data = &(itcm_msg.msgq[thr_q_num]);

	/* Check msgq full */
	if( msg_data->msgcount >= msg_data->maxqlen ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,
						"msgq full : q_num = %d, (%d / %d)",
							thr_q_num,msg_data->msgcount,msg_data->maxqlen);
        itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_ng++;
		return BPF_RU_ITCM_EQFULL;
	}

	/********************************************
	* message memory alloc processing.          *
	********************************************/
	/* メッセージがある場合はメッセージをコピーする領域確保 */
	if( size != 0 ){
		buff = malloc(size);
		errcd = errno;
		if( buff == NULL){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"malloc NG : errno = %d",errcd);
			return BPF_RU_ITCM_ENOSPACE;
		}
	}

	/* Attention
		これよりエラー復帰するときはbuffを忘れず解放しましょう
	*/

	/* メッセージ領域を取る */
	/* 未使用の領域があるかチェックしてあればそれを使用 */
	ret = pthread_mutex_lock( &(itcm_msg_flist.mutex) );
	if( ret == ITCM_THREAD_FUNC_OK ){
		if( itcm_msg_flist.head != NULL ){ /* 未使用領域あり */
			/* 未使用リストからメッセージ領域を取得 */
			msg = itcm_msg_flist.head;
			ITCM_LIST_REMOVE(itcm_msg_flist.head, msg);	/* pgr0689 */
			itcm_msg_flist.msgcount--;   /* 未使用リストデクリメント */
		}
		/* mutex unlock */
		pthread_mutex_unlock(&(itcm_msg_flist.mutex));
	}
	if( msg == NULL ){/* msg領域を確保できてない場合 */
		msg = malloc(sizeof(struct itcm_msg_t));
		errcd = errno;
		if( msg == NULL ){
			/* 領域とれないのでbuffを開放してエラーを返す */
			if( buff != NULL ){
				free(buff);
			}
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"malloc NG : errno = %d",errcd);
			return BPF_RU_ITCM_ENOSPACE;
		}
	}

	/* Set Date */
	/* msgszは32byteに変更しているので、共通ヘッダのみコピーする	*/
	msg->size = size;			/* send msg size */
	memmove(buff, msgp, (size - sizeof(void*)));	/* send date *//*pgr0060*/	
	((t_bpf_ru_itcm_msgq_expand*)buff)->msg_p = (void*)msgp;
	msg->msgp = buff; 			/* send buffer (the case of "size=0"--> buff=NULL) */

	/* Wait Flag */
	if( thr_q_flag == BPF_RU_ITCM_WAIT ){
		if( timeout == NULL ){ /* 無限 */
			ret = pthread_mutex_lock(&(msg_data->mutex));
		} else {
			clock_gettime(CLOCK_REALTIME, (struct timespec *)&dtime);
			ITCM_TS_ADD(&dtime, &dtime, timeout);
			ret = pthread_mutex_timedlock(&(msg_data->mutex),(struct timespec *)&dtime);
		}
	} else { /* BPF_RU_ITCM_NOWAIT */
		ret = pthread_mutex_trylock(&(msg_data->mutex));
	}

	if( ret == ITCM_THREAD_FUNC_OK ){
		/* Check msgq full */
		if( msg_data->msgcount >= msg_data->maxqlen ){
			/* QFULL */
			/* キューがたまってるからシグナル出しとく */
			pthread_cond_signal(&(msg_data->cond));

			pthread_mutex_unlock(&(msg_data->mutex));

			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"msgq full : q_num = %d, (%d / %d)",
								thr_q_num,msg_data->msgcount,msg_data->maxqlen);
        	itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_ng++;
			retcode = BPF_RU_ITCM_EQFULL;
		} else {
			msg_data->msgcount++; /* count up */
        	itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_total++;
        	itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_now = msg_data->msgcount;
			if( msg_data->msgcount > msg_data->peakmsgnum ){
				/* peak msg num */
				msg_data->peakmsgnum = msg_data->msgcount;
				itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_peek = msg_data->peakmsgnum;
			}
			if( msg_data->head == NULL ){
				/* リストが空だったら先頭にくっつける */
				ITCM_LIST_ADD_HEAD(msg_data->head, msg);
				/* そして最後尾の設定 */
				msg_data->tail = msg;
			} else {
				/* リストがあれば最後尾にくっつける */
				ITCM_LIST_ADD(msg_data->tail, msg);
			}
			/********************************************
			* message send processing.(queue attach)    *
			********************************************/

			pthread_cond_signal(&(msg_data->cond));
			pthread_mutex_unlock(&(msg_data->mutex));
			return BPF_RU_ITCM_OK; /* NORMAL END *//* recv側で解放しているためfree不要 */ /* pgr0524 */
		}
	} else {
		/* can't get mutex lock */
		errcd = ret;
		switch(ret){
			case ETIMEDOUT: /* time out */
				retcode = BPF_RU_ITCM_ETIMEOUT;
				break;
			case EBUSY:		/*can't get lock */
				retcode = BPF_RU_ITCM_ENOSEMGET;
				break;
			case EINTR:		/* einter */
				retcode = BPF_RU_ITCM_EINTR;
				break;
			default:		/* another */
				retcode = BPF_RU_ITCM_NG;
		}
	}

	/*
		ERROR ROUTIN
	*/
	if( buff != NULL ){
		free(buff);
	}
	/* メッセージテーブルを未使用リストに繋げる */
	ret = pthread_mutex_lock(&itcm_msg_flist.mutex);
	if( ret == ITCM_THREAD_FUNC_OK ){
		/* ロックが取れたので未使用リストに接続 */
		if( itcm_msg_flist.head == NULL ){
			/* 追加 */
			ITCM_LIST_ADD_HEAD(itcm_msg_flist.head, msg);
			itcm_msg_flist.tail = msg;
		} else {
			/* 挿入 */
			ITCM_LIST_INSERT(itcm_msg_flist.head,
							(itcm_msg_flist.head)->p, msg);
		}
		itcm_msg_flist.msgcount++;

		if( itcm_msg_flist.msgcount > itcm_msg_flist.peakmsgnum ){
			/* ピーク値が越えたので更新する */
			itcm_msg_flist.peakmsgnum = itcm_msg_flist.msgcount;
		}
		/* ロック解除 */
		pthread_mutex_unlock(&(itcm_msg_flist.mutex));
	} else { /* ロックが取れなかったのでfreeしとく */
		free(msg);
	}
	/* recv側で解放しているためfree不要 */
    return retcode;  /* pgr0524 */
}
/* @} */



/* @} */
