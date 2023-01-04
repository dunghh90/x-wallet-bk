/*!
 * @skip  $ld:$
 * @file  f_cmn_com_abort.c
 * @brief abort処理
 * @date  2013/11/26 ALPHA) 近藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <sys/syscall.h> 
#include <signal.h>
#include <time.h>
#include "f_cmn_com_inc.h"

/*!
 * @brief 関数機能概要:abort処理
 * @note  関数処理内容.
 *       -# Assert取得
 *       -# ソフトALM要因の設定
 *       -# プロセス固有情報出力
 *       -# シグナル送信
 * @param  processID     [in]UINT         プロセス識別子
 * @param  almCode       [in]UINT         ALM要因
 * @param  fileName      [in]VOID const*  ファイル名
 * @param  lineNo        [in]UINT         行番号
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2013/11/26 ALPHA)近藤 zynq化対応
 */
VOID f_cmn_com_abort(UINT processID, UINT almCode, const VOID* fileName, UINT lineNo )
{
	UINT 			cnt;
	union sigval	value;
	int				selfpid = getpid();
	int				selftid = (int)syscall(SYS_gettid);
	struct timespec	boottime;
	const char*		wname;
	UINT			faultId;

	(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &boottime);
	/* Assert取得 */
	wname = strrchr((const char*)fileName, '/');
	if(wname == NULL)
	{
		wname = (const char*)fileName;
	}
	else
	{
		wname++;
	}
	
	switch(almCode & 0xFFFF0000)
	{
		case D_RRH_SIGUSR_OPT_RSET_SWALM_NTC :
			faultId = 1;
			break;
		case D_RRH_SIGUSR_OPT_RSET_INIALM_NTC :
			faultId = 2;
			break;
		default :
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "UNKOWN ALM CODE<0x%08x>", almCode);
			faultId = 0xff;
			break;
	}
	
	if( f_cmnw_svfunc.svfunc.func_addr != NULL ){
		(VOID)(*(f_cmnw_svfunc.svfunc.func_addr))(faultId,(almCode & 0xFFF));
	}

	BPF_COM_LOG_DMESG("[ !!!!!!!!!!!! FAULT DETECT FID<%d>+FLT CODE<0x%08x> PID:%d TID:%d Pos<%s(%d)> !!!!!!!!!!!! ] \n",
						faultId, ( almCode & 0xFFF ) ,selfpid ,selftid , wname , lineNo );
	BPF_COM_LOG_ASSERT(
		D_RRH_LOG_AST_LV_CRITICAL,
		"!!! FAULT !!! detect by Proc<p:%d,t:%d,u:%d>,Fid<%d>,Code<0x%08x>,Up<%ld.%09ldsec>,Pos<%s(%d)>",
			selfpid , selftid , processID , faultId, (almCode & 0xFFF), boottime.tv_sec, boottime.tv_nsec, wname, lineNo );

	/* プロセス固有情報出力 */
	for(cnt=0; cnt < f_cmnw_almlogfunc.func_num ; cnt++)
	{
		if( f_cmnw_almlogfunc.func_list[cnt].func_addr != NULL )
		{
			(VOID)(*(f_cmnw_almlogfunc.func_list[cnt].func_addr))(faultId,(almCode & 0xFFF));
		}
	}

	if(faultId == 1){
	/* 自分自身のシグナルハンドラを呼び出し、シグナルハンドラ経由でINITにUSER1を通知する  */
	/* なお、ABORT時はBPFがコールできない可能性があるため直接sigqueueする */
	value.sival_int = (int)(almCode);	/* ALMコード*/
	sigqueue(	selftid,			 /* 自分のスレッドID	*/
				SIGTERM,			 /* Signal番号			*/
				value );			 /* オプション			*/
	}
	/* exitはシグナルハンドラで行う */
}
/* @} */
