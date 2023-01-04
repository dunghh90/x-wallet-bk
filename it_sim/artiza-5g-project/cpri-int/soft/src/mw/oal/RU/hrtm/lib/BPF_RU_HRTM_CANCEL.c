/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_HRTM_CANCEL.c
 *	@brief	Timer Management Cancel.
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
 *  @brief  Timer Management Timer Cancel.
 *  @note   API function. 
 *  @param  timerid      [in]  timer id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_NOENT no entry error
 *  @retval BPF_RU_HRTM_ERR_CAN   cancel error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_HRTM_CANCEL(
        unsigned int timerid,
        int* errcd)
{
	/* ログ取得	*/
	/* EventIDおよびQueueIDは不明のため0とする	*/
	bpf_ru_hrtm_timer_log_get(	timerid, D_RRH_LOG_TIMERKIND_STOP, 0, 0);
	
	/* タイマ停止	*/
	return bpf_ru_hrtm_cancel2(timerid,errcd, HRTM_FLG_OFF);
}
/* @} */

/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Cancel.
 *  @note   API function. 
 *  @param  timerid      [in]  timer id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_NOENT no entry error
 *  @retval BPF_RU_HRTM_ERR_CAN   cancel error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_HRTM_CANCEL_FORCE(
        unsigned int timerid,
        int* errcd)
{
	/* ログ取得	*/
	/* EventIDおよびQueueIDは不明のため0とする	*/
	bpf_ru_hrtm_timer_log_get(	timerid, D_RRH_LOG_TIMERKIND_STOP, 0, 0);
	
	/* タイマ停止	*/
	return bpf_ru_hrtm_cancel2(timerid,errcd, HRTM_FLG_ON);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Cancel(実体).
 *  @note   API function. 
 *  @param  timerid      [in]  timer id
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_PARAM parameter error
 *  @retval BPF_RU_HRTM_ERR_NOENT no entry error
 *  @retval BPF_RU_HRTM_ERR_CAN   cancel error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_hrtm_cancel2( unsigned int timerid,int* errcd,unsigned int force)
{
	int						ret;
	timer_t					timerId;
	unsigned int			timeridBPF = 0xFFFFFFFF;
	unsigned int			count;
	unsigned int			threadId;
	struct bpf_timespec		tcount;
	unsigned int			passed_sec;
	unsigned int			passed_nsec;
	unsigned int			passed_msec;
	
	if(di_hrtm_assign_flag != HRTM_FLG_ON){
		/* need assign */
		return BPF_RU_HRTM_ERR_CAN;
	}
	
	/********************************************
	* mutex lock processing.                    *
	********************************************/
	ret = pthread_mutex_lock(&bpf_ru_hrtm_mutexobj);
	if( ret != 0 ){
		*errcd = errno;
		return  BPF_RU_HRTM_ERR_CAN;
	}
	
	/* Timer使用中かつUserのTimerIDとBPFで保持しているUserTimerIDが一致したら	*/
	/* BPFが払い出したTimerIDを取得する											*/
	for( count = HRTMID_MIN; count <= di_tim_max; count++ )
	{
		if( ( di_tim_tbl_p->info[count].keyinf.keyinf1 == timerid ) &&
			( di_tim_tbl_p->info[count].id_next == HRTM_USING ) )
		{
			timeridBPF = count;
			break;
		}
	}
	
	/* BPF払い出しTimerIDが見つからなければNGとする	*/
	/* またはUser指定のTimerIDが0だったらNGとする	*/
	if( ((timeridBPF < HRTMID_MIN) || (timeridBPF > di_tim_max)) ||
		( timerid == 0 ) )
	{
		/********************************************
		* mutex unlock processing.                  *
		********************************************/
	    pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		/* 範囲外 */

		/* 大量にとられるので抑止 */
/*		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "timerid ng=%d user_timer=%d ucount=%d",		*/
/*								timeridBPF, timerid, di_tim_tbl_p->usenum );					*/
		return BPF_RU_HRTM_ERR_CAN;
	}
	
	/* 登録したスレッドと違うスレッドが停止に来た場合はエラーとする	*/
	threadId = (unsigned int)syscall(SYS_gettid);
	if( di_tim_tbl_p->info[timeridBPF].threadId != threadId )
	{
		/* 範囲外 */
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "thread error! TIMID=%d, STA THDID=%d, IN THDID=%d ucnt=%d",
							timerid, di_tim_tbl_p->info[timeridBPF].threadId, threadId, di_tim_tbl_p->usenum );
	    pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		return BPF_RU_HRTM_ERR_CAN;
	}
	
	/* TIMER乗っ取り問題再対処	START	*/
	/* 指定されたTIMERが指定時間超過している場合は							*/
	/* TIMER停止処理は行わない(停止してしまうとOSIDが払い戻されてしまう)	*/
	if( force == HRTM_FLG_OFF )
	{
		/* 現時刻取得	*/
		(void)clock_gettime( CLOCK_MONOTONIC_RAW , &(tcount) );
		
		/* タイマ起動時間からの差分を算出する	*/
		if( di_tim_tbl_p->info[timeridBPF].keyinf.tcount.tv_nsec > tcount.tv_nsec )
		{
			/* nsecが現時刻の方が小さい場合	*/
			passed_sec  = (tcount.tv_sec - 1) - di_tim_tbl_p->info[timeridBPF].keyinf.tcount.tv_sec;
			passed_nsec = (1000000000 + tcount.tv_nsec) - di_tim_tbl_p->info[timeridBPF].keyinf.tcount.tv_nsec;
		}
		else
		{
			passed_sec  = tcount.tv_sec  - di_tim_tbl_p->info[timeridBPF].keyinf.tcount.tv_sec;
			passed_nsec = tcount.tv_nsec - di_tim_tbl_p->info[timeridBPF].keyinf.tcount.tv_nsec;
		}
		
		/* msec単位に変換する	*/
		passed_msec = (passed_sec * 1000) + (passed_nsec / 1000000);
		
		/* 時間が経過していたら停止処理は行わない	*/
		if( passed_msec >= di_tim_tbl_p->info[timeridBPF].time_val )
		{
			/* 経過していてもTOが実際に上がる前だったらTOさせたくないので	*/
			/* フラグによる停止(ハンドラで参照)を行う						*/
			di_tim_tbl_p->info[timeridBPF].cancelFlg = HRTM_FLG_ON;
		    pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
			
			/* 終了コードはOKの方が良い?	*/
			return BPF_RU_HRTM_COMPLETE;
		}
	}
	/* TIMER乗っ取り問題再対処	END		*/
	
	/********************************************
	* timer delete processing.                  *
	********************************************/
	timerId = di_tim_tbl_p->info[timeridBPF].timerId;
	ret = bpf_ru_hrtm_del(timeridBPF);
	
    /********************************************
    * mutex unlock processing.                  *
    ********************************************/
    pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
	
	if( ret != BPF_RU_HRTM_COMPLETE ){
		return BPF_RU_HRTM_ERR_NOENT;
	}
	/********************************************
	* POSIX timer delete processing.            *
	********************************************/
	ret = timer_delete( timerId );
	if(ret == HRTM_SYSCALL_NG){
		*errcd = errno;
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "timer delete ng=%d user_timer=%d ucnt=%d",
								ret, timerid, di_tim_tbl_p->usenum );
		return BPF_RU_HRTM_ERR_CAN;
	}
	
    return BPF_RU_HRTM_COMPLETE;
}
/* @} */

/* @} */
