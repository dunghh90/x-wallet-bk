/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_HRTM_LOG.c
 *	@brief	Timer Management Timer LOG.
 *  @date   2013/11/19 ALPHA)yokoyama create
 *  @date   2015/10/22 ALPHA)ueda modify for TDD-RRE(zynq)
 *  @date   2015/12/07 ALPHA)ueda modify for M-RRU-ZSYS-01960
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_HRTM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_hrtm.h"

/** @addtogroup BPF_RU_HRTM
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer log get.
 *  @note   API function.
 *  @param  timerid      [in]  timer id
 *  @param  kind         [in]  kind
 *  @param  eventno      [in]  event no
 *  @param  sendQueue    [in]  send queue
 *  @return none
 *  @date   2013/11/19 ALPHA)yokoyama create
 *  @date   2015/10/22 ALPHA)ueda modify for TDD-RRE(zynq)
 *  @date   2015/12/07 ALPHA)ueda modify for M-RRU-ZSYS-01960
 */
/********************************************************************************************************************/
VOID bpf_ru_hrtm_timer_log_get(	UINT		timerId,		/*	BPFで払い出したタイマID		*/
								UINT		kind,			/*	種別						*/
								UINT		eventno,		/*	EventNo						*/
								UINT		sendQueue )		/*	Send Queue					*/
{
#if 0
	int ofs = 3;											/* high固定 lowは0を設定		*/
	switch(kind)
	{
		case D_RRH_LOG_TIMERKIND_STOP:
			BPF_COM_PLOG_RSC_TIM(E_TIM_LV_CAN_LOW + ofs,timerId,eventno,sendQueue,0);
			break;
		case D_RRH_LOG_TIMERKIND_START:
			BPF_COM_PLOG_RSC_TIM(E_TIM_LV_SET_LOW + ofs,timerId,eventno,sendQueue,0);
			break;
		case D_RRH_LOG_TIMERKIND_TIMEOUT:
			BPF_COM_PLOG_RSC_TIM(E_TIM_LV_TOUT_LOW + ofs,timerId,eventno,sendQueue,0);
			break;
		default:
			break;
	}
#else
	T_RRH_SYSTEM_TIME		tDateTime;
	T_RRH_LOG_TIMER_HIS*	timerHistoryLog;
	int						iRetCode;
	int						iErrCode;
	unsigned int			logIndex;

	/* 通信用共有メモリのタイマ履歴を取得する	*/

	/* 時間取得 */
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);

	/************************************************************/
	/* Take Semaphore											*/
	/************************************************************/
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	iRetCode = BPF_RU_IPCM_PROCSEM_TAKE(
		E_RRH_SEMID_APL_TIMER_HISTORY,	/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,			/* R/Wロック種			*/
		BPF_RU_IPCM_WAIT,				/* ウェイトフラグ		*/
		&dtime,							/* タイマ時間			*/
		&iErrCode						/* 詳細NGコード			*/
	);

	if( D_RRH_OK != iRetCode )
	{
		return;
	}

	/* 共有メモリのタイマ履歴を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_TIMER_HISTORY,
		(VOID **)&timerHistoryLog,
		&iErrCode
	);

	if( D_RRH_OK != iRetCode )
	{
		/************************************************************/
		/* Give Semaphore(Assert log used)							*/
		/************************************************************/
		iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_APL_TIMER_HISTORY,	/* セマフォ番号			*/
			BPF_RU_IPCM_LOCK_RW,			/* R/Wロック種			*/
			&iErrCode						/* 詳細NGコード			*/
		);

		return;
	}

	/* Index取得	*/
	logIndex = timerHistoryLog->uiWriteIdx;

	/* 共有メモリの情報を更新	*/
	/* Set timestamp */
	timerHistoryLog->tRecord[logIndex].tDate.year	= tDateTime.year;
	timerHistoryLog->tRecord[logIndex].tDate.month	= tDateTime.month;
	timerHistoryLog->tRecord[logIndex].tDate.day	= tDateTime.day;
	timerHistoryLog->tRecord[logIndex].tDate.hour	= tDateTime.hour;
	timerHistoryLog->tRecord[logIndex].tDate.min	= tDateTime.min;
	timerHistoryLog->tRecord[logIndex].tDate.sec	= tDateTime.sec;
	timerHistoryLog->tRecord[logIndex].tDate.msec	= tDateTime.msec;
	timerHistoryLog->tRecord[logIndex].tDate.ucount	= tDateTime.ucount;

	/* timer ID、種別、eventID、QueueIDを取得する */
	timerHistoryLog->tRecord[logIndex].timerID 		= timerId;
	timerHistoryLog->tRecord[logIndex].kind 		= kind;
	timerHistoryLog->tRecord[logIndex].eventNo		= eventno;
	timerHistoryLog->tRecord[logIndex].sendQueue	= sendQueue;

	/* Indexインクリメント	*/
	logIndex++;

	/* サイクリック判定	*/
	if( logIndex >= D_RRH_LOG_TIMERLOG_RCD_NUM )
	{
		/* Indexを0に戻す	*/
		timerHistoryLog->uiWriteIdx = 0;
		/* サイクリックフラグONする	*/
		timerHistoryLog->uiCycleflg = D_RRH_ON;
	}else{
		timerHistoryLog->uiWriteIdx = logIndex;
	}

	/************************************************************/
	/* Give Semaphore(Assert log used)							*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_APL_TIMER_HISTORY,	/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,			/* R/Wロック種			*/
		&iErrCode						/* 詳細NGコード			*/
	);
#endif
    return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer log print.
 *  @note   API function.
 *  @param  file*      [in]  file discriptor
 *  @return none
 *  @date   2013/11/19 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
VOID BPF_RU_HRTM_TIMER_LOG_PRINT( FILE* fp )		/* file discriptor	*/
{
	T_RRH_LOG_TIMER_HIS*	timerHistoryLog;
	int						iRetCode;
	int						iErrCode;
	unsigned int			logCount;
	unsigned int			logIndex;
	unsigned char 			kindMessages[D_RRH_LOG_TIMERKIND_NUM][10] = { "START  ", "STOP   ", "TIMEOUT" };

	/* 共有メモリのタイマ履歴を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_TIMER_HISTORY,
		(VOID **)&timerHistoryLog,
		&iErrCode
	);

	if( D_RRH_OK != iRetCode )
	{
		return;
	}

    logIndex = ( timerHistoryLog->uiWriteIdx + D_RRH_LOG_TIMERLOG_RCD_NUM ) % D_RRH_LOG_TIMERLOG_RCD_NUM ;


	fprintf( fp, "=======================================================================\n");
	fprintf( fp, "[xxxx] Date       Time              TimerID  Kind    EventNo  QueueID\n");
	fprintf( fp, "-----------------------------------------------------------------------\n");
    for( logCount = 0; logCount < D_RRH_LOG_TIMERLOG_RCD_NUM; logCount++ )
	{
		if( timerHistoryLog->tRecord[logIndex].tDate.year != 0 ) {
			/* 時刻表示	*/
			fprintf( fp, "[%04d] %04d/%02d/%02d %02d:%02d:%02d:%03d.%u  ",
					logIndex,
					timerHistoryLog->tRecord[logIndex].tDate.year,
					timerHistoryLog->tRecord[logIndex].tDate.month,
					timerHistoryLog->tRecord[logIndex].tDate.day,
					timerHistoryLog->tRecord[logIndex].tDate.hour,
					timerHistoryLog->tRecord[logIndex].tDate.min,
					timerHistoryLog->tRecord[logIndex].tDate.sec,
					timerHistoryLog->tRecord[logIndex].tDate.msec*10,
					timerHistoryLog->tRecord[logIndex].tDate.ucount
			);

			/* 詳細情報表示	*/
			fprintf( fp, "%08x %s %08x %08x\n",
					timerHistoryLog->tRecord[logIndex].timerID,
					&kindMessages[timerHistoryLog->tRecord[logIndex].kind][0],
					timerHistoryLog->tRecord[logIndex].eventNo,
					timerHistoryLog->tRecord[logIndex].sendQueue );
		}
		logIndex = ( logIndex + 1) % D_RRH_LOG_TIMERLOG_RCD_NUM ;

	}

	return ;
}
/* @} */

/* @} */
