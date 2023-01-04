/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_ASSERT.c (base:f_com_assert.c)
 * @brief Assert log function.
 * @date  2008/10/6 FFCS)Linxl Create\n
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2008-2013
 */
 
/*!
 * @addtogroup BPF_COM_LOG
 * @{
 */

#include "bpf_i_com_log.h"
#include <stdarg.h>
#include <sys/syscall.h>

/*!
 *  @brief  This fuction will be called to write assert log.
 *  @note   This function is processed as follows.\n
 *          - Write assert log for each task
 *  @param  logLevel   [in]   log level
 *  @param  name       [in]   file name
 *  @param  line       [in]   line
 *  @param  *Message_p [in]   Assert log message 
 *  @param  ...        [in]   Assert log message
 *  @return VOID
 *  @retval none
 *  @date   2008/10/6 FFCS)Linxl create
 *  @date   2008/11/26 FFCS)Linxl Improve the method of getting time
 *  @date   2013/03/16 ALPHA)fujiiy    modify for B25(base f_com_assert)
 *  @date   2013/11/13 ALPHA)yokoyama  modify for docomo ZYNQ
 */
VOID BPF_LOG_ASSERT_DWL(	UINT	logLevel,				/* Level					*/
								const char* name ,				/* File Name				*/
								int line ,						/* 行数						*/
								const char*	Message_p,			/* Message					*/
								... )							/* 可変情報					*/
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT					logIndex;	 /* Assert log counter */
	T_RRH_SYSTEM_TIME		tDateTime;
	T_RRH_LOG_ASSERT_DWL*	assertLogDwl;
	INT						iErrCode;
	INT						iRetCode;
	
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	UINT					assertLevel;
	CHAR					str[D_RRH_LOG_MAX_ASSERTLOGBODY];
	va_list					args;			
	const char*				wname;

	/* Check message */
	if(Message_p == NULL)
	{
		return;
	}
	/************************************************************/
	/* Log Levelを取得											*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_LOGLEVEL_MNG,
		(VOID **)&tLogLevelMng,
		&iErrCode );
	
	if( D_RRH_OK != iRetCode )
	{
		return;
	}
	
	/* ログレベルを取得	*/
	assertLevel = tLogLevelMng->assertDwlLevel;
	
	/* BIT判定	*/
	if( (assertLevel & logLevel) == 0)
	{
		/* 取得対象ログでなければ関数を抜ける	*/
		return;
	}
	
	/************************************************************/
	/* Local parameter initialization							*/
	/************************************************************/
	logIndex	= D_RRH_ZERO;
	memset(&tDateTime, D_RRH_ZERO, sizeof(tDateTime));
	
	/************************************************************/
	/* Get Assert log from shared memory						*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_ASSERT_DWL,
		(VOID **)&assertLogDwl,
		&iErrCode
	);
	
	if( D_RRH_OK != iRetCode )
	{
		return;
	}
	
	/************************************************************/
	/*  Write Assert log										*/
	/************************************************************/ 
	/* Get System time			*/
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);

	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;
	
	/************************************************************/
	/* Take Semaphore(Assert log used)							*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_TAKE(
		E_RRH_SEMID_APL_ASSERT_DWL,		/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,			/* R/Wロック種			*/
		BPF_RU_IPCM_WAIT,				/* ウェイトフラグ		*/
		&dtime,							/* タイマ時間			*/
		&iErrCode						/* 詳細NGコード			*/
	);
	if( D_RRH_OK != iRetCode )
	{
		return;
	}
	
	/************************************************************/
	/* Get log counter											*/
	/************************************************************/
	logIndex = (UINT)( ( assertLogDwl->uiWriteIdx ) % D_RRH_LOG_MAX_ASSERTLOGNUM_DWL ) ;	/* index破壊防止 */
	wname = strrchr(name, '/');
	if(wname == NULL)
	{
		wname = name;
	}
	else
	{
		wname++;
	}
	/************************************************************/
	/* Update log counter										*/
	/************************************************************/
	assertLogDwl->uiWriteIdx++;
	if(assertLogDwl->uiWriteIdx >= D_RRH_LOG_MAX_ASSERTLOGNUM_DWL)
	{
		assertLogDwl->uiWriteIdx	= D_RRH_ZERO;
		assertLogDwl->uiCycleflg	= D_RRH_ON;
	}
	
	/************************************************************/
	/* Give Semaphore(Assert log used)							*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_APL_ASSERT_DWL,		/* セマフォ番号			*/
		BPF_RU_IPCM_LOCK_RW,			/* R/Wロック種			*/
		&iErrCode						/* 詳細NGコード			*/
	);
	
	if( D_RRH_OK != iRetCode )
	{
		return;
	}
	
	/************************************************************/
	/* Assert log record clear									*/
	/************************************************************/ 
	memset((VOID*)&(assertLogDwl->tRecord[logIndex]), D_RRH_ZERO, sizeof(T_RRH_LOG_ASSERT_RECORD)); 
	
	/* Set timestamp			*/
	assertLogDwl->tRecord[logIndex].tDate.year		= tDateTime.year;
	assertLogDwl->tRecord[logIndex].tDate.month	= tDateTime.month;
	assertLogDwl->tRecord[logIndex].tDate.day		= tDateTime.day;
	assertLogDwl->tRecord[logIndex].tDate.hour		= tDateTime.hour;
	assertLogDwl->tRecord[logIndex].tDate.min		= tDateTime.min;
	assertLogDwl->tRecord[logIndex].tDate.sec		= tDateTime.sec;
	assertLogDwl->tRecord[logIndex].tDate.msec		= tDateTime.msec;
	assertLogDwl->tRecord[logIndex].tDate.ucount	= tDateTime.ucount;
	
	/* Set Log Level			*/
	assertLogDwl->tRecord[logIndex].uiLogLevel	= logLevel;

	/* Set File Name			*/
	strncpy( (char *)assertLogDwl->tRecord[logIndex].ucFileName, wname, D_RRH_LOG_MAX_ASSERTFILENAME );
	
	/* Set File Line			*/
	assertLogDwl->tRecord[logIndex].uiLine = line;
	
	/* Set assert log message	*/
	/* 保存対象メッセージの編集を行う	*/
	memset(str,0,sizeof(str));
	va_start(args, Message_p); /* pgr0039 呼び出し先関数で値を設定するため問題無し */
	(void)vsnprintf(str, D_RRH_LOG_MAX_ASSERTLOGBODY , (void *)Message_p, args);
	va_end(args);
	
	memcpy(assertLogDwl->tRecord[logIndex].message, str, strlen((const CHAR*)str) );
	
	/* thread id */
	assertLogDwl->tRecord[logIndex].iTid = syscall(SYS_gettid);

	return;
}

/* @} */
