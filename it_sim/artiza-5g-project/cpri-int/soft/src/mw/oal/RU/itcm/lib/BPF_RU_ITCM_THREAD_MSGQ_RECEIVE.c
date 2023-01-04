/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_MSGQ_RECEIVE.c
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
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Message Queue Receive.
 *  @note   API function.
 *  @param  thr_q_num    [in]  queue number
 *  @param  size         [in]  message size
 *  @param  msgp         [out] receive message address
 *  @param  msg_size     [out] receive message size
 *  @param  thr_q_flag   [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ESIZEOVER message size over error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @retval BPF_RU_ITCM_ENOMSG    no message error
 *  @retval BPF_RU_ITCM_EADDR     address error
 *  @retval BPF_RU_ITCM_EINTR     signal interupt error
 *  @retval BPF_RU_ITCM_ENOSEMGET semaphore get error
 *  @retval BPF_RU_ITCM_ETIMEOUT  timeout error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_MSGQ_RECEIVE(unsigned int thr_q_num, unsigned int size,
									void **msgp, unsigned int *msg_size,
									int thr_q_flag, struct bpf_timespec *timeout)
{
	/*****************************************************
	* message receive processing.(call internal routine) *
	*****************************************************/
	return bpf_ru_itcm_thread_msgq_receive2(ITCM_MSGQ_USER, thr_q_num, size, msgp,
											msg_size, thr_q_flag, timeout);

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Message Queue Receive2.
 *  @note   BPF internal function.
 *  @param  q_type       [in]  queue type
 *  @param  thr_q_num    [in]  queue number
 *  @param  size         [in]  message size
 *  @param  msgp         [out] receive message address
 *  @param  msg_size     [out] receive message size
 *  @param  thr_q_flag   [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ESIZEOVER message size over error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @retval BPF_RU_ITCM_ENOMSG    no message error
 *  @retval BPF_RU_ITCM_EADDR     address error
 *  @retval BPF_RU_ITCM_EINTR     signal interupt error
 *  @retval BPF_RU_ITCM_ENOSEMGET semaphore get error
 *  @retval BPF_RU_ITCM_ETIMEOUT  timeout error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_itcm_thread_msgq_receive2(enum itcm_msgq_type q_type,
				  unsigned int thr_q_num, unsigned int size,
				  void **msgp, unsigned int *msg_size,
				  int thr_q_flag, struct bpf_timespec *timeout)
{
	int ret, retcode = BPF_RU_ITCM_OK;
	struct itcm_msg_data_t *msg_data = NULL;
	struct itcm_msg_t *msg = NULL;
	struct bpf_timespec dtime;

	T_RRH_HEAD*			pComHead;

	/********************************************
	* parameter check processing.               *
	********************************************/
	/* Invalid cases, Macro return  */
	ITCM_MSGQ_TYPECHECK(q_type);			/* Msgq Type Check        */

	ITCM_MSGQ_NUMCHECK(thr_q_num);			/* Msgq number Check      */

	ITCM_MSGQ_FLAGCHECK(thr_q_flag);		/* Wait Flag Check        */

	ITCM_MSGQ_USEDCHECK(thr_q_num);			/* Msgq used status Check */

	msg_data = &(itcm_msg.msgq[thr_q_num]);

	/********************************************
	* mutex get processing.                     *
	********************************************/
	if( thr_q_flag == BPF_RU_ITCM_WAIT ){
		if( timeout == NULL ){ /* 無限待ち */
			ret = pthread_mutex_lock( &(msg_data->mutex) );
			if( ret != ITCM_THREAD_FUNC_OK ){ /* NGの時 */
				bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
									"pthread_mutex_lock NG : errno = %d",ret);
				if( ret == EINTR ){
					return BPF_RU_ITCM_EINTR;
				} else {
					return BPF_RU_ITCM_NG;
				}
			}
		} else {
			/* タイマ時間設定．時間はEpocTimeからの絶対時間 */
			clock_gettime(CLOCK_REALTIME, (struct timespec *)&dtime);
			ITCM_TS_ADD(&dtime, &dtime, timeout);

			while(1){
				ret = pthread_mutex_timedlock( &(msg_data->mutex),(struct timespec *)&dtime);

				if( ret == ITCM_THREAD_FUNC_OK ){ /* ロック取得成功 */
					break;
				} else { /* ロック取得失敗 */
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
										"pthread_mutex_timedlock NG : errno = %d",ret);
					if( ret == ETIMEDOUT ){
						return BPF_RU_ITCM_ETIMEOUT;
					} else if( ret != EINTR ){
						return BPF_RU_ITCM_NG;
					}
				}/* 割り込みによる中断の場合は再度lock処理をする */
			}
		}
	} else { /* BPF_RU_ITCM_NOWAITの場合 */
		ret = pthread_mutex_trylock( &(msg_data->mutex) );
		if( ret != ITCM_THREAD_FUNC_OK ){ /* NG時 */
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"pthread_mutex_trylock NG : errno = %d",ret);
			if( ret == EBUSY ){
				return BPF_RU_ITCM_ENOSEMGET;
			} else {
				return BPF_RU_ITCM_NG;
			}
		}
	}

	/* Mutex Get Success */
	/********************************************
	* msg get processing.                       *
	********************************************/

	if( msg_data->msgcount == 0 ){ /* メッセージ無し */
		if( thr_q_flag == BPF_RU_ITCM_NOWAIT ){
			/* 誰か待っているかもしれないのでシグナルを出しておく */
			pthread_cond_signal( &(msg_data->cond) );
			/* ロック解除して終了 */
			pthread_mutex_unlock( &(msg_data->mutex) );

			return BPF_RU_ITCM_ENOMSG; /* メッセージ無し */

		} else { /* BPF_RU_ITCM_WAIT */
			while(1){
				/* メッセージくるかエラーになるまで繰り返す */
				/* 時間は絶対時間を設定してある */
				if( timeout == NULL ){
					/* 無限待 (pthread_cond_waitはエラーを返さない) */
					ret = pthread_cond_wait( &(msg_data->cond), &(msg_data->mutex) );
				} else {
					ret = pthread_cond_timedwait( &(msg_data->cond), &(msg_data->mutex),
												(struct timespec *)&dtime);
				}

				if( ret != ITCM_THREAD_FUNC_OK ){ /* エラー復帰 */
					if( ret == ETIMEDOUT ){ /* タイムアウト */
						retcode = BPF_RU_ITCM_ETIMEOUT;
					} else if( ret == EINTR ){
						retcode = BPF_RU_ITCM_EINTR; /* 割り込み中断発生 */
					} else {
						retcode = BPF_RU_ITCM_NG; /* 予期せぬエラー */
					}
					/* 誰か待っているかもしれないのでシグナルを出しておく */
					pthread_cond_signal( &(msg_data->cond) );
					/* エラー復帰なのでunlockして終了 */
					pthread_mutex_unlock( &(msg_data->mutex) );
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
									"pthread_cond_timedwait NG : errno = %d", ret);
					return retcode;
				}
				if( msg_data->msgcount != 0 ){
					break; /* メッセージあるのでループを抜ける */
				}
			} /* while */
		} /* wait or nowait */
	}



	/* ここにきたときはメッセージ有るとき */
	/* カウントを減らす */
	msg_data->msgcount--;
    itcm_msgq_statis_p->statis_info[thr_q_num].msgnum_now=msg_data->msgcount;
	/* メッセージ取り出し */
	msg = msg_data->head;
	ITCM_LIST_REMOVE(msg_data->head, msg); /* 別の変数から値を設定しているため問題無し */ /* pgr0689 */
	/* 誰か待ってるひとがいるかもしれないのでシグナルを出しておく */
	pthread_cond_signal( &(msg_data->cond) );
	/* mutex解除 */
	pthread_mutex_unlock( &(msg_data->mutex) );

	if( msg == NULL )
	{
		return BPF_RU_ITCM_NG;
	}
	/* メッセージの情報をユーザに渡す */
/* docomo ZYNQ化 Start	*/
	/* 以下の流れでメッセージ受信をしてユーザに情報を返す	*/
	/* ①受信したメッセージからKEYIDを取得					*/
	/* ②取得したKEYIDから共有メモリを取得					*/
	/* ③取得共有メモリの先頭をユーザに返す					*/
	if(msg->msgp != NULL){
	pComHead = ( T_RRH_HEAD *)msg->msgp;
	*msgp = ((t_bpf_ru_itcm_msgq_expand*)(msg->msgp))->msg_p;
	/* 受信メッセージサイズを共有メモリの使用サイズに設定	*/
	*msg_size = pComHead->uiLength;
/* docomo ZYNQ化 end	*/

	/* 不要になったメッセージテーブルとバッファを解放 */
		free(msg->msgp);
	}


	/* メッセージテーブルを未使用リストに繋げる */
	ret = pthread_mutex_lock( &(itcm_msg_flist.mutex) );
	if( ret == ITCM_THREAD_FUNC_OK ){
		/* ロックが取れたので未使用リストに接続 */
		if( itcm_msg_flist.head == NULL ){
			/* 追加 */
			ITCM_LIST_ADD_HEAD(itcm_msg_flist.head, msg);
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
		pthread_mutex_unlock( &(itcm_msg_flist.mutex) );
	} else { /* ロックが取れなかったのでfreeしとく */
		free(msg);
	}

	return retcode;
}
/* @} */

/* @} */
