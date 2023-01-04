/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_HRTM_REGISTER.c
 *	@brief	Timer Management Timer Register.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_HRTM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_hrtm.h"
#include <sys/syscall.h> 

/** @addtogroup BPF_RU_HRTM
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Register.
 *  @note   API function. 
 *  @param  notify_type  [in]  notify type
 *  @param  time         [in]  timer value
 *  @param  key_inf      [in]  key information
 *  @param  entry        [in]  event notification entry(function address)
 *  @param  timerid      [out] timer id
 *  @param  errcd        [out] detail error code
 *  @return none
 *  @date   2013/11/18 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
VOID bpf_ru_hrtm_timer_handle(
			bpf_ru_hrtm_key_inf_t	*	keyInfo,		/*	タイムアウト情報			*/
			UINT						timerId)		/*	BPFで払い出したタイマID		*/
{
	T_RRH_TIMERTO_NTC*			ntc;
	int							ret;
	int							errcd;
	
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別	*/
										sizeof(T_RRH_TIMERTO_NTC),		/* size			*/
										(void **)&ntc,					/* アドレス		*/
										&errcd);
	if( ret != D_RRH_OK )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"time out bget error ret:%d e:%x uid:%x qid:%x bid:%x oid:%x seq:%x",
				ret , 
				keyInfo->eventNo,
				keyInfo->keyinf1,
				keyInfo->queueID,
				timerId,
				keyInfo->timerId_os,
				keyInfo->seqno);
		return;
	}
	
	ntc->header.uiEventNo		= keyInfo->eventNo;			/* イベント番号設定		*/
	ntc->header.uiSignalkind	= keyInfo->keyinf1;			/* TimerID(APLの認識しているID)		*/
	ntc->header.uiDstPQueueID	= 0;
	ntc->header.uiDstTQueueID	= keyInfo->queueID;			/* 送信先QueueID設定	*/
	ntc->header.uiSrcPQueueID	= 0;
	ntc->header.uiSrcTQueueID	= 0;
	ntc->header.uiLength		= sizeof( T_RRH_TIMERTO_NTC );
	
	/* data setting */
	ntc->data.id = timerId;
	ntc->data.id_os = keyInfo->timerId_os;
	ntc->data.seqno = keyInfo->seqno;
	ntc->data.userkey = keyInfo->uservalue;
	memcpy(&(ntc->data.tstat_count),&(keyInfo->tcount),sizeof(ntc->data.tstat_count));
	(void)clock_gettime( CLOCK_MONOTONIC_RAW , &(ntc->data.tout_count) );

	bpf_ru_hrtm_timer_log_get(	keyInfo->keyinf1, D_RRH_LOG_TIMERKIND_TIMEOUT,
								keyInfo->eventNo, keyInfo->queueID);
	
	/* Thread間Message送信	*/
	ret =  BPF_RU_IPCM_MSGQ_SEND(	ntc->header.uiDstTQueueID,			/* 送信先QueueID設定	*/
									(void *)ntc );						/* 送信メッセージ		*/
	
	if( ret != D_RRH_OK )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"time out msg send error ret:%d e:%x uid:%x qid:%x bid:%x oid:%x seq:%x",
				ret , 
				keyInfo->eventNo,
				keyInfo->keyinf1,
				keyInfo->queueID,
				timerId,
				keyInfo->timerId_os,
				keyInfo->seqno);
	}
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Register.
 *  @note   API function. 
 *  @param  notify_type  [in]  notify type
 *  @param  time         [in]  timer value
 *  @param  eventno      [in]  event no
 *  @param  queueid      [in]  send thread queue id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_BUSY  resource busy error
 *  @retval BPF_RU_HRTM_ERR_REG   registeration error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *  @date   2013/11/18 ALPHA)yokoyama   modify for docomo
 */
/********************************************************************************************************************/
int BPF_RU_HRTM_REGISTER(
		bpf_ru_hrtm_notify_type_t		notify_type,			/* TimerMode = 1shot/周期						*/
		unsigned int					timerid,				/* 開始するタイマID								*/
		unsigned int					time,					/* Timer値										*/
		unsigned int					eventno,				/* TO時のイベント番号							*/
		unsigned int					queueid,				/* TO時の送信先QueueID(ThreadのQueueID)			*/
		int*							errcd)					/* ErrCode										*/
{
	bpf_ru_hrtm_key_inf_t	key_inf;
	unsigned int			timeridBPF;				/* BPFが払い出したTimerID						*/
	int						iRetCode = 0;
	
	/* タイマ情報を設定する	*/
	key_inf.keyinf1 = timerid;
	key_inf.queueID = queueid;
	key_inf.eventNo = eventno;
	key_inf.timerId_os = (timer_t)-1;		/* pgr0203 */
	key_inf.seqno = 1;
	
	iRetCode = bpf_ru_hrtm_register2(notify_type,					/* 1shot / periodic					*/
								time,								/* Timer値							*/
								&key_inf,							/* Key情報							*/
								&bpf_ru_hrtm_timer_handle,			/* CallBack関数						*/
								&timeridBPF,						/* TimerID							*/
								errcd);								/* ErrCode							*/
	
	bpf_ru_hrtm_timer_log_get(timerid, D_RRH_LOG_TIMERKIND_START, eventno, queueid);
	
	return iRetCode;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Register.
 *  @note   API function. 
 *  @param  notify_type  [in]  notify type
 *  @param  time         [in]  timer value
 *  @param  eventno      [in]  event no
 *  @param  queueid      [in]  send thread queue id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_BUSY  resource busy error
 *  @retval BPF_RU_HRTM_ERR_REG   registeration error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *  @date   2013/11/18 ALPHA)yokoyama   modify for docomo
 */
/********************************************************************************************************************/
int BPF_RU_HRTM_REGISTER_ID(
		bpf_ru_hrtm_notify_type_t		notify_type,			/* TimerMode = 1shot/周期						*/
		unsigned int					timerid,				/* 開始するタイマID								*/
		unsigned int					time,					/* Timer値										*/
		unsigned int					eventno,				/* TO時のイベント番号							*/
		unsigned int					queueid,				/* TO時の送信先QueueID(ThreadのQueueID)			*/
		int*							errcd,					/* ErrCode										*/
		unsigned int*					id_p,					/* Bpf timerid pointer							*/
		timer_t*						osid_p ,				/* OS timer id pointer							*/
		unsigned int 					value)
{
	bpf_ru_hrtm_key_inf_t	key_inf;
	unsigned int			timeridBPF;				/* BPFが払い出したTimerID						*/
	int						iRetCode = 0;

	/* タイマ情報を設定する	*/
	key_inf.keyinf1 = timerid;
	key_inf.queueID = queueid;
	key_inf.eventNo = eventno;
	key_inf.timerId_os = (timer_t)-1;		/* pgr0203 */
	key_inf.uservalue = value;
	key_inf.seqno = 1;

	iRetCode = bpf_ru_hrtm_register2(notify_type,					/* 1shot / periodic					*/
								time,								/* Timer値							*/
								&key_inf,							/* Key情報							*/
								&bpf_ru_hrtm_timer_handle,			/* CallBack関数						*/
								&timeridBPF,						/* TimerID							*/
								errcd);								/* ErrCode							*/
	
	if( id_p != NULL)
	{
		*id_p = timeridBPF;
	}
	if( osid_p != NULL)
	{
		*osid_p = key_inf.timerId_os;
	}
	bpf_ru_hrtm_timer_log_get(timerid, D_RRH_LOG_TIMERKIND_START, eventno, queueid);
	
	return iRetCode;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Register(実体).
 *  @note   API function. 
 *  @param  notify_type  [in]  notify type
 *  @param  time         [in]  timer value
 *  @param  key_inf      [in]  key inf
 *  @param  funcP        [in]  call back function
 *  @param  timerid      [out]  timer id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_BUSY  resource busy error
 *  @retval BPF_RU_HRTM_ERR_REG   registeration error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_hrtm_register2(
        bpf_ru_hrtm_notify_type_t notify_type,
        unsigned int time,
        bpf_ru_hrtm_key_inf_t* key_inf,
        void (*entry)(bpf_ru_hrtm_key_inf_t*,unsigned int),
        unsigned int* timerid,
        int* errcd)
{
	unsigned int quotient,hrtm_id;
	int ret;
	timer_t                 timerId;
	struct itimerspec       itval;
	struct sigevent         evp;
	pthread_attr_t          thr_attr;
	unsigned int			count;
	unsigned int			sameid = HRTM_FLG_OFF;
	
	if(di_hrtm_assign_flag != HRTM_FLG_ON){
		/* need assign */
		return BPF_RU_HRTM_ERR_REG;
	}
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	if( (notify_type != BPF_RU_HRTM_REG_ONCE) && 
			(notify_type != BPF_RU_HRTM_REG_PERIODIC) ){
		return BPF_RU_HRTM_ERR_PARAM;
	}
	if((time == 0) || (time > MAXTIME)){
		return BPF_RU_HRTM_ERR_PARAM;
		/* 10msec以下でもNGにはしない */
	}
	if(entry == NULL){
		return BPF_RU_HRTM_ERR_PARAM;
	}
		
	/* <defined hrtm.h>             */
	/*    MINTIME = 10msec          */
	/*    MAXTIME = 7200000msec(2h) */

	/* MINTIME 以下はMINTIME切り上げ */
	if(time < MINTIME){
		time = MINTIME;
	}
	/* MINTIME以下の端数はMINTIME単位で切り上げ */
	if(time%MINTIME != 0){
		quotient = time/MINTIME;
		time = (quotient*MINTIME)+MINTIME;
	}
	
	/* UserのTimerIDが0だったら弾く	*/
	if( 0 == key_inf->keyinf1 )
	{
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "ng timer id=%d ucnt=%d",
								key_inf->keyinf1, di_tim_tbl_p->usenum );
		return BPF_RU_HRTM_ERR_CAN;
	}

	/* 強制的にキャンセル実施 */
	(void)BPF_RU_HRTM_CANCEL_FORCE( key_inf->keyinf1 , errcd);

	/********************************************
	* mutex lock processing.                    *
	********************************************/
	ret = pthread_mutex_lock(&bpf_ru_hrtm_mutexobj);
	if(ret != 0 ){
		*errcd = errno;
		return BPF_RU_HRTM_ERR_REG;
	}
	
	/* 同一タイマIDが起動されていたらNGとする	*/
	for( count = HRTMID_MIN; count <= di_tim_max; count++ )
	{
		if( ( di_tim_tbl_p->info[count].keyinf.keyinf1 == key_inf->keyinf1 ) &&
			( di_tim_tbl_p->info[count].id_next == HRTM_USING ) )
		{
			sameid = HRTM_FLG_ON;
			break;
		}
	}
	
	if( sameid == HRTM_FLG_ON )
	{
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "same id is already start=%d ucnt=%d",
								key_inf->keyinf1, di_tim_tbl_p->usenum );
		pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		*errcd = EBUSY;
		return BPF_RU_HRTM_ERR_CAN;
	}
	
	/********************************************
	* timer get processing.                     *
	********************************************/
	ret = bpf_ru_hrtm_get( &hrtm_id );
	
	/********************************************
	* mutex unlock processing.                  *
	********************************************/
	pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
	
	if( ret != BPF_RU_HRTM_COMPLETE ){
		*errcd = EBUSY;
		return BPF_RU_HRTM_ERR_BUSY;
	}
	/********************************************
	* timer table update processing.            *
	********************************************/
	di_tim_tbl_p->info[hrtm_id].type		= notify_type;
	di_tim_tbl_p->info[hrtm_id].time_val	= time;
	di_tim_tbl_p->info[hrtm_id].threadId	= (unsigned int)syscall(SYS_gettid);
	
	if(key_inf){
		memcpy(&di_tim_tbl_p->info[hrtm_id].keyinf,
				key_inf,sizeof(di_tim_tbl_p->info[hrtm_id].keyinf));
	}else{
		memset(&di_tim_tbl_p->info[hrtm_id].keyinf,
				0,sizeof(di_tim_tbl_p->info[hrtm_id].keyinf));
	}
	di_tim_tbl_p->info[hrtm_id].entry		= (void *)entry;
	di_tim_tbl_p->info[hrtm_id].cancelFlg	= HRTM_FLG_OFF;
	
	/********************************************
	* POSIX timer create processing.            *
	********************************************/
	memset(&evp, 0, sizeof(evp));
	evp.sigev_notify 			= (int)SIGEV_THREAD;
	evp.sigev_notify_function	= (void *)bpf_ru_hrtm_handler;
	evp.sigev_signo				= SIGRTMIN;	/* D.C */

    /* Timeout thread attribute setting. */
	pthread_attr_init(&thr_attr);
	pthread_attr_setstacksize(&thr_attr, TIMEOUT_THREAD_STACK);
	evp.sigev_notify_attributes	= &thr_attr;

	evp.sigev_value.sival_int	= hrtm_id;
	
	ret = timer_create(CLOCK_MONOTONIC,&evp,&timerId);
	if(ret == HRTM_SYSCALL_NG){
		*errcd = errno;
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "timer create ng. user_timer=%d ucnt=%d",
								key_inf->keyinf1, di_tim_tbl_p->usenum );
		
		pthread_mutex_lock(&bpf_ru_hrtm_mutexobj);
		ret = bpf_ru_hrtm_del(hrtm_id);	/* タイマID解放 */
		pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		
		return BPF_RU_HRTM_ERR_REG;
	}
	di_tim_tbl_p->info[hrtm_id].timerId		= timerId;
	if(key_inf)
	{
		key_inf->timerId_os = timerId;
	}
	/********************************************
	* POSIX timer set processing.               *
	********************************************/
	memset(&itval,0,sizeof(itval));
	itval.it_value.tv_sec	= time / 1000;				/* msec->sec  */
	itval.it_value.tv_nsec	= (time % 1000) * 1000000;	/* msec->nsec */	/* pgr0062 */
	if(notify_type == BPF_RU_HRTM_REG_PERIODIC){
		/* it_interval */
		itval.it_interval.tv_sec  = itval.it_value.tv_sec;
		itval.it_interval.tv_nsec = itval.it_value.tv_nsec;
	}
	ret = timer_settime(timerId,0,&itval,NULL);
	if(ret == HRTM_SYSCALL_NG){
		*errcd = errno;
		pthread_mutex_lock(&bpf_ru_hrtm_mutexobj);
		ret = bpf_ru_hrtm_del(hrtm_id);	/* タイマID解放 */
		pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		timer_delete(timerId);
		return BPF_RU_HRTM_ERR_REG;
	}
	
	/* 起動時刻取得	*/
	(void)clock_gettime( CLOCK_MONOTONIC_RAW , &(di_tim_tbl_p->info[hrtm_id].keyinf.tcount) );
	
	*timerid = hrtm_id;
    return BPF_RU_HRTM_COMPLETE;
}
/* @} */

/* @} */
