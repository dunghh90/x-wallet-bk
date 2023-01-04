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


#define D_BPF_COM_LOG_ASSERT_DUPLICATE_FLG	1
#define D_BPF_COM_LOG_ASSERT_DUPLICATE_NUM	2

/* 以下のデータはプロセス毎で設定する情報である */
/* 重複チェックをやめる場合はbpf_comw_log_assert_dupflgを0にすればよい */

static char  bpf_comw_log_assert_prefilename[D_RRH_LOG_MAX_ASSERTFILENAME];		/*!< 前回ファイル名 */
static UINT  bpf_comw_log_assert_prelineno = 0;									/*!< 前回ファイル行数  */
static UINT  bpf_comw_log_assert_dupcount = 0;									/*!< 重複回数 */
static UINT  bpf_comw_log_assert_dupflg = 0;									/*!< 重複抑止フラグ:現在無効 */
static UINT  bpf_comw_log_assert_dupindex = 0;									/*!< 重複位置 */

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
VOID bpf_com_log_assert_real(	UINT	logLevel,				/* Level					*/
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
	T_RRH_LOG_ASSERT*		assertLog;
	INT						iErrCode;
	INT						iRetCode;
	
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	UINT					assertLevel;
	CHAR					str[D_RRH_LOG_MAX_ASSERTLOGBODY];
	va_list					args;			
	UINT					count;
	const char*				wname;

	/* Check message */
	if(Message_p == NULL)
	{
		return;
	}
//擬似環境
	memset(str,0,sizeof(str));
	va_start(args, Message_p); /* pgr0039 呼び出し先関数で値を設定するため問題無し */
	(void)vsnprintf(str, D_RRH_LOG_MAX_ASSERTLOGBODY , (void *)Message_p, args);
	va_end(args);
	printf("\nbpf_com_log_assert_real FILE = %s LINE = %d %s\n",name,line,str);
//擬似環境
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
	assertLevel = tLogLevelMng->assertLogLevel;
	
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
		E_RRH_SHMID_APL_ASSERT_LOG,
		(VOID **)&assertLog,
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
		E_RRH_SEMID_APL_ASSERT_LOG,		/* セマフォ番号			*/
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
	logIndex = (UINT)( ( assertLog->uiWriteIdx ) % D_RRH_LOG_MAX_ASSERTLOGNUM ) ;	/* index破壊防止 */
	wname = strrchr(name, '/');
	if(wname == NULL)
	{
		wname = name;
	}
	else
	{
		wname++;
	}
	/* 重複チェックが有効なら */
	if( bpf_comw_log_assert_dupflg == D_BPF_COM_LOG_ASSERT_DUPLICATE_FLG)
	{
		/* プロセス内でファイル行数とファイル名が一致した場合 */
		if( ( bpf_comw_log_assert_prelineno != 0 ) &&
			( bpf_comw_log_assert_prelineno == line ) &&
			( !strncmp( &bpf_comw_log_assert_prefilename[0] , wname , strlen(bpf_comw_log_assert_prefilename)) )
		)
		{
			bpf_comw_log_assert_dupcount++;
		}
		else
		{
			bpf_comw_log_assert_dupcount = 0;
			bpf_comw_log_assert_dupindex = 0;
		}
	}
	else
	{
		bpf_comw_log_assert_dupcount = 0;
		bpf_comw_log_assert_dupindex = 0;
	}
	if( bpf_comw_log_assert_dupcount < D_BPF_COM_LOG_ASSERT_DUPLICATE_NUM )
	{
		/************************************************************/
		/* Update log counter										*/
		/************************************************************/
		assertLog->uiWriteIdx++;
		if(assertLog->uiWriteIdx >= D_RRH_LOG_MAX_ASSERTLOGNUM)
		{
			assertLog->uiWriteIdx	= D_RRH_ZERO;
			assertLog->uiCycleflg	= D_RRH_ON;
		}
		count = bpf_comw_log_assert_prelineno = (UINT)line;
		strncpy( bpf_comw_log_assert_prefilename , wname , sizeof(bpf_comw_log_assert_prefilename));
		bpf_comw_log_assert_dupindex = logIndex;
	}
	/* 2回以上連続重複した場合、ファイル名の部分をDUPLICATEDに設定し、行数の部分を重複回数に変更する */
	else
	{
		count = bpf_comw_log_assert_dupcount;		/* 重複回数 */
		wname = (const char*)"DUPLICATED!!!";		/* 重複時の擬似ファイル名 */		
		logIndex = bpf_comw_log_assert_dupindex;	/* ログ格納位置は前回位置に再設定 */
	}
	
	/************************************************************/
	/* Give Semaphore(Assert log used)							*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_APL_ASSERT_LOG,		/* セマフォ番号			*/
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
	memset((VOID*)&(assertLog->tRecord[logIndex]), D_RRH_ZERO, sizeof(T_RRH_LOG_ASSERT_RECORD)); 
	
	/* Set timestamp			*/
	assertLog->tRecord[logIndex].tDate.year		= tDateTime.year;
	assertLog->tRecord[logIndex].tDate.month	= tDateTime.month;
	assertLog->tRecord[logIndex].tDate.day		= tDateTime.day;
	assertLog->tRecord[logIndex].tDate.hour		= tDateTime.hour;
	assertLog->tRecord[logIndex].tDate.min		= tDateTime.min;
	assertLog->tRecord[logIndex].tDate.sec		= tDateTime.sec;
	assertLog->tRecord[logIndex].tDate.msec		= tDateTime.msec;
	assertLog->tRecord[logIndex].tDate.ucount	= tDateTime.ucount;
	
	/* Set Log Level			*/
	assertLog->tRecord[logIndex].uiLogLevel	= logLevel;

	/* Set File Name			*/
	strncpy( (char *)assertLog->tRecord[logIndex].ucFileName, wname, D_RRH_LOG_MAX_ASSERTFILENAME );
	
	/* Set File Line			*/
	assertLog->tRecord[logIndex].uiLine = count;
	
	/* Set assert log message	*/
	/* 保存対象メッセージの編集を行う	*/
	memset(str,0,sizeof(str));
	va_start(args, Message_p); /* pgr0039 呼び出し先関数で値を設定するため問題無し */
	(void)vsnprintf(str, D_RRH_LOG_MAX_ASSERTLOGBODY , (void *)Message_p, args);
	va_end(args);
	
	memcpy(assertLog->tRecord[logIndex].message, str, strlen((const CHAR*)str) );
	
	/* thread id */
	assertLog->tRecord[logIndex].iTid = (int)syscall(SYS_gettid);

	return;
}

/*!
 *  @brief  This fuction will be called to write assert log.
 *  @note   This function is processed as follows.\n
 *          - Write assert log for each task
 *  @param  outname    [in]   out name
 *  @param  logLevel   [in]   log level
 *  @param  *Message_p [in]   log message 
 *  @param  ...        [in]    log message
 *  @return VOID
 *  @retval none
 *  @date   2015/02/12 FJT)Taniguchi create
 */
VOID bpf_com_log_fprint(const char* outname,			/* Out先					*/
						int   logLevel,					/* Level(dmsegと同じ)		*/
						const char*	Message_p,			/* Message					*/
						... )							/* 可変情報					*/
{
	FILE* fp = NULL;
	va_list args;
	char msg[256];
	if(( fp = fopen(outname,"w") ) != NULL ){
		va_start(args, Message_p);	/* pgr0039 */
		vsnprintf( msg , sizeof(msg) , Message_p, args );
		va_end(args);
		fprintf(fp,"<%d>%s",logLevel,msg);
		fflush(fp);
		fclose(fp);
	}
	return;
}

/* @} */
