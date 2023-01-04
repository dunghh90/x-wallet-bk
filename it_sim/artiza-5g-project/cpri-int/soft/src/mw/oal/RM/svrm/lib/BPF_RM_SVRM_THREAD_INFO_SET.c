/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_THREAD_INFO_SET.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RM_SVRM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_rm_svrm.h"
#include <sys/syscall.h> 
#include "bpf_i_ru_ipcm.h"


/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Thread IDの情報を登録する
 *  @note   Thread IDの情報を登録する
 *  @param  processID     [in]   プロセスID
 *  @param  threadID      [in]   スレッドID
 *  @param  threadQueueID [in]   スレッドQueueID
 *  @return result code
	
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @date   2013/11/20 ALPHA)yokoyama create
 
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_THREAD_INFO_SET( unsigned int processID, unsigned int threadID, unsigned int threadQueueID ,unsigned long int pThreadID )
{
	T_RRH_TID_MNG*			threadinfoP;
	int						iErrCode;
	int						iRetCode;
	unsigned int			threadCnt;
	
	/************************************************************/
	/* Take Semaphore											*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_TAKE(
		E_RRH_SEMID_APL_THRD_ID_MNG_INFO,	/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,				/* R/Wロック種			*/
		BPF_RU_IPCM_WAIT,					/* ウェイトフラグ		*/
		D_RRH_NULL,							/* タイマ時間			*/
		&iErrCode							/* 詳細NGコード			*/
	);
	
	if( D_RRH_OK != iRetCode )
	{
		return BPF_RM_SVRM_ERR_DISC;
	}
	
	/* 共有メモリのスレッドID管理情報を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_THRD_ID_MNG_INFO,
		(VOID **)&threadinfoP,
		&iErrCode
	);
	
	if( D_RRH_OK != iRetCode )
	{
		/************************************************************/
		/* Give Semaphore											*/
		/************************************************************/
		iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_APL_THRD_ID_MNG_INFO,	/* セマフォ番号			*/
			BPF_RU_IPCM_LOCK_RW,				/* R/Wロック種			*/
			&iErrCode							/* 詳細NGコード			*/
		);
		
		return BPF_RM_SVRM_ERR_DISC;
	}
	
	if( processID < D_RRH_PROCID_MAX )
	{
		/* スレッド数取得	*/
		threadCnt = threadinfoP->infoP[ processID ].count;
		if( threadCnt < D_RRH_THREAD_MAX)
		{
			/* 各種情報を保存する	*/
			threadinfoP->infoP[ processID ].infoT[ threadCnt ].aplThreadID	= threadID;
			threadinfoP->infoP[ processID ].infoT[ threadCnt ].realThreadID	= (int)syscall(SYS_gettid);
			threadinfoP->infoP[ processID ].infoT[ threadCnt ].TQID			= threadQueueID;
			threadinfoP->infoP[ processID ].infoT[ threadCnt ].pThreadID	= (unsigned long int)pthread_self();
			/* スレッド数インクリメント	*/
			threadinfoP->infoP[ processID ].count++;
		}
		else
		{
			IPCM_DBGLOG_FMT_ERR("threadCnt was limit over! p:%d,t:%d,q:%08x",processID,threadID,threadQueueID);
		}
	}
	else
	{
		IPCM_DBGLOG_FMT_ERR("processID was limit over! p:%d,t:%d,q:%08x",processID,threadID,threadQueueID);
	}
	/************************************************************/
	/* Give Semaphore											*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_APL_THRD_ID_MNG_INFO,	/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,				/* R/Wロック種			*/
		&iErrCode							/* 詳細NGコード			*/
	);
	
	
	return BPF_RM_SVRM_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Thread IDの情報を取得する
 *  @note   Thread IDの情報を取得する
 *  @param  processID     [out]   プロセスID
 *  @param  threadID      [out]   スレッドID
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @date   2013/11/20 ALPHA)yokoyama create
 
 */
/********************************************************************************************************************/
int bpf_rm_svrm_thread_info_get( unsigned int *processID, unsigned int *threadID )
{
	INT								tmpPID;
	unsigned long int				tmpTID;
	INT								errcd;
	UINT							count;
	T_RRH_PID_MNG*					pidMng;
	T_RRH_TID_MNG*					threadinfoP;
	
	/********************************************************************/
	/* 送信元プロセスID取得												*/
	/********************************************************************/
	/* PID管理テーブル取得 */
	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&pidMng,
		&errcd
	);
	
	/* 送信元ProcessIDを取得する	*/
	tmpPID = getpid();
	
	/* プロセスIDを一律254(debug用)にしておく	*/
	*processID = 254;
	*threadID = 0;
	
	for( count = 0; count < pidMng->count; count++ )
	{
		if( tmpPID == pidMng->info[ count ].realProcessID )
		{
			/* ProcessIDが一致したらプロセスをAPL用のプロセスIDに書き換え	*/
			*processID = pidMng->info[ count ].aplProcessID;
			break;
		}
	}
	
	/********************************************************************/
	/* 送信元スレッドID取得												*/
	/********************************************************************/
	/* スレッドID管理テーブル取得 */
	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_THRD_ID_MNG_INFO,
		(VOID **)&threadinfoP,
		&errcd
	);
	
	/* 送信元pthreadIdを取得する	*/
	tmpTID = (unsigned long int)pthread_self();
	
	/* 送信元のプロセスIDがDebug以外であればスレッドIDの検索を行う	*/
	if( *processID != 254 )
	{
		for( count = 0; count < threadinfoP->infoP[ *processID ].count; count++ )
		{
			/* 実ThreadIDが一致したらAPL上のThreadIDを取得する	*/
			if( threadinfoP->infoP[ *processID ].infoT[ count ].pThreadID == tmpTID )
			{
				*threadID = threadinfoP->infoP[ *processID ].infoT[ count ].aplThreadID;
				break;
			}
		}
	}
	
	
	
	return BPF_RM_SVRM_COMPLETE;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)yokoyama add end */
