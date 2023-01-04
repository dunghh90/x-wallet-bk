/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_DELAY.c
 *	@brief	Inter Thread Communication Management Library(THREAD).
 *	        Thread Delay Function.
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

/** @addtogroup BPF_RU_ITCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Thread Delay.
 *  @note   API function. 
 *  @param  delaytime    [in]  delay time
 *  @param  flag         [in]  flag
 *  @param  rem          [out] remain delay time
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_EINTR     signal interupt
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_DELAY(struct bpf_timespec *delaytime, int flag, struct bpf_timespec *rem)
{
	int errcd;
	struct bpf_timespec dtime;
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	if( delaytime == NULL ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
						    "delaytime check NG. delaytime = NULL");
		return BPF_RU_ITCM_EPARAM;
	}
	if( flag != BPF_RU_ITCM_CNT && flag != BPF_RU_ITCM_RET ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
						    "flag check NG. flag = %d", flag);
		return BPF_RU_ITCM_EPARAM;
	}
	
	/********************************************
	* thread delay processing.                  *
	********************************************/
	if( flag == BPF_RU_ITCM_RET ){
		/* sleepが中断された時NG復帰する時はこちらの処理 */
		if( nanosleep((const struct timespec *)delaytime, 
						(struct timespec *)rem) == 0){
			return BPF_RU_ITCM_OK;
		} else {
			errcd = errno;
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
								"nanosleep returned NG. errno = %d", errcd);
			if( errcd == EINTR ){
				/* 割り込みによる中断なら明示的なエラーコードを返す */
				return BPF_RU_ITCM_EINTR;
			} else {
				return BPF_RU_ITCM_OK;
			}
		}
	} else { /* nanosleepがEINTRで復帰した場合は再設定 */
		dtime.tv_sec = delaytime->tv_sec;
		dtime.tv_nsec = delaytime->tv_nsec;
		while(1){
			if( nanosleep((const struct timespec *)&dtime, 
							(struct timespec *)rem) == 0 ){
				return BPF_RU_ITCM_OK; /* 正常終了なので復帰 */ 
			}
			/* 0以外が返ってきたら異常終了 */
			errcd = errno; /* エラー番号の退避 */
			if( errcd == EINTR ){ /* 割り込みによる中断なので残り時間を再設定 */
				/* 念のため残り時間をチェックしておく */
				if( rem->tv_sec <= 0 && rem->tv_nsec <= 0 ){
					return BPF_RU_ITCM_OK; /* 残り時間0なので満了したことにしとく */
				}
				/* 残り時間を再設定する */
				dtime.tv_sec = rem->tv_sec;
				dtime.tv_nsec = rem->tv_nsec;
			} else {
				bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
									"nanosleep returned NG. errno = %d", errcd);
				/* 割り込み以外で異常終了した場合はNGを返す */
				return BPF_RU_ITCM_NG;
			}
		}
	}
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
						"Unexpected error occurred. errno = %d",errcd);
	return BPF_RU_ITCM_NG; /* ここは通らない */
}

/* @} */

/* @} */
