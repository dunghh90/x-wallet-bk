/*!
 * @skip		$ld:$
 * @file		f_main_init.c
 * @brief		pf_main 初期状態処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/05/11 ALPHA) tomioka modify for PGR対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応

 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include <signal.h>
#include "f_main_inc.h"
#include "BPF_COM_CMD.h"
#define D_MAIN_BOOTUPTIME 10	/* 10sec PowerON ~ OS起動開始前までのだいたいの時間 */

/*!
 * @brief 関数機能概要:起動時刻取得関数
 * @note  関数処理内容:EEPROMおよびRTCから起動時刻を取得する
 * @param  N/A
 * @return N/A
 * @date 2015/04/22 FJT)Taniguchi RRH-001-000 BSP
 */
static VOID f_main_setSysTime( VOID )
{
	struct tm				curr;
	struct tm				next;
	time_t					base;
	struct timespec			timecount;
	T_RRH_SYSTEM_TIME		tDateTime_pre;
	T_RRH_SYSTEM_TIME		tDateTime;
	T_RRH_TRAINVTBL			*invTbl;
	int						errcd;

// 擬似環境 20210316
//#ifdef SIM_DEBUG
    printf("\n return func %s",__func__);
	return;
//#endif
// 擬似環境 20210316
    
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(invTbl == NULL){
		cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_CRITICAL, "Sharedmem get errcd=%x",errcd);
		return;
	}
	(void)clock_gettime( CLOCK_MONOTONIC_COARSE , &timecount );
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime_pre);
	if(invTbl->timesave_flg == D_RRH_ON){
		memcpy((UCHAR*)&tDateTime , invTbl->timesaveinfo, sizeof(invTbl->timesaveinfo));	/* マイクロ秒カウンタ以外をコピー */
		tDateTime.ucount = 0;
		tDateTime.year = ntohs(tDateTime.year);	/* 2byteなのでendian変換 */
		if(
			((tDateTime.year > 2099)||(tDateTime.year < 2000)) ||
			((tDateTime.month > 12)||(tDateTime.month < 1))    ||
			((tDateTime.day > 31)||(tDateTime.day < 1))        ||
			(tDateTime.hour > 23)                               ||
			(tDateTime.min > 59)                                ||
			(tDateTime.sec > 59)                                ||
			(tDateTime.msec > 99)
		){
			memcpy((UCHAR*)&tDateTime,(UCHAR*)&tDateTime_pre,sizeof(tDateTime));
		}
		else{
			/* リセット時刻を秒カウンタに変換する */
			memset(&curr,0,sizeof(curr));
			curr.tm_sec =  tDateTime.sec;
			curr.tm_min =  tDateTime.min;
			curr.tm_hour = tDateTime.hour;
			curr.tm_mday =tDateTime.day;
			curr.tm_mon = tDateTime.month - 1;
			curr.tm_year = tDateTime.year - 1900;
			base = mktime(&curr);
			if( -1 == base ){ base = 0; }
			/* 前回のリセット時刻 + リセット~OS起動前(約10秒) +  OS起動~この処理までの時間 */
			base += ( D_MAIN_BOOTUPTIME + timecount.tv_sec );
			if( localtime_r(&base,&next) != NULL){
				tDateTime.msec = 0;
				tDateTime.sec = next.tm_sec;
				tDateTime.min = next.tm_min;
				tDateTime.hour = next.tm_hour;
				tDateTime.day = next.tm_mday;
				tDateTime.month = next.tm_mon + 1;
				tDateTime.year = next.tm_year + 1900;
			}
		}
	}
	else{
		memcpy((UCHAR*)&tDateTime,(UCHAR*)&tDateTime_pre,sizeof(tDateTime));
	}
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, "pre %04d:%02d:%02d %02d:%02d:%02d.%02d0",
		tDateTime_pre.year,
		tDateTime_pre.month,
		tDateTime_pre.day,
		tDateTime_pre.hour,
		tDateTime_pre.min,
		tDateTime_pre.sec,
		tDateTime_pre.msec
	);
	(VOID)BPF_HM_DEVC_SETTIME(tDateTime);
	cmn_com_assert_th_get(f_mainw_assert_p, D_RRH_LOG_AST_LV_INFO, "next %04d:%02d:%02d %02d:%02d:%02d.%02d0",
		tDateTime.year,
		tDateTime.month,
		tDateTime.day,
		tDateTime.hour,
		tDateTime.min,
		tDateTime.sec,
		tDateTime.msec
	);
	if( invTbl->timesave_flg == D_RRH_ON ){
		invTbl->timesave_flg = D_RRH_OFF;
		BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_TIMESAVE_FLG, &(invTbl->timesave_flg));
	}
	/* 起動時間保存	*/
	memcpy( &f_comw_operations_restart_time, &tDateTime, sizeof(T_RRH_SYSTEM_TIME) );
	return;
}


/*!
 * @brief 関数機能概要:EtherPortダンプ開始処理
 * @note  関数処理内容:EtherPortダンプ開始処理
 * @param  N/A
 * @return N/A
 * @date 2019/11/12 FJT)Taniguchi RRH-001-000 BSP
 */
void f_main_iniEthDump(void){

// 擬似環境 20210316
//#ifdef SIM_DEBUG
    printf("\n return func %s",__func__);
	return;
//#endif
// 擬似環境 20210316
	UINT	regw_data;

	regw_data = BPF_COM_DPLOG_ETHTYPE_MP;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DPETYPE, &regw_data );

	regw_data = BPF_COM_DPLOG_MAC_H_CU;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DP_CUMAC_H, &regw_data );

	regw_data = BPF_COM_DPLOG_MAC_L_CU | ( f_comw_rrhVersion & 0xFFFF );
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DP_CUMAC_L, &regw_data );

	regw_data = f_cmw_tra_inv_tbl->cmx_mac_01_p0 & 0x0000FFFF;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DP_RUMAC_H, &regw_data );

	regw_data = ((UINT)(f_cmw_tra_inv_tbl->cmx_mac_02_p0 ) << 16 ) | ((UINT)(f_cmw_tra_inv_tbl->cmx_mac_03_p0) & 0x0000FFFF);
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DP_RUMAC_L, &regw_data );

	regw_data = 0;
	switch( f_cmw_tra_inv_tbl->cmx_dpmode_mp){
		case 1:
		case 2:
		case 3:
			regw_data = f_cmw_tra_inv_tbl->cmx_dpmode_mp;
			break;
		default:
			break;
	}

	switch( f_cmw_tra_inv_tbl->cmx_dpmode_sp){
		case 1:
			regw_data |= (UINT)(f_cmw_tra_inv_tbl->cmx_dpmode_sp) << 8;
			break;
		default:
			break;
	}

	switch( f_cmw_tra_inv_tbl->cmx_dpmode_dbg){
		case 1:
			regw_data |= (UINT)(f_cmw_tra_inv_tbl->cmx_dpmode_dbg) << 16;
			break;
		default:
			break;
	}

	switch( f_cmw_tra_inv_tbl->cmx_dpmode_aisg){
		case 1:
			regw_data |= (UINT)(f_cmw_tra_inv_tbl->cmx_dpmode_aisg) << 24;
			break;
		default:
			break;
	}
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SWWORK_DPMODE, &regw_data );
	if(regw_data != 0){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING , "Starting Dump Mode(0x%08x)",regw_data );
	}

	f_com_dumpMsgInit();

	return;
}


/*!
 * @brief		f_main_init
 * @note		pf_mainスレッド初期状態処理.
 *					-# BPF初期化
 *					-# 各種ログ展開
 *					-# 装置種別取得
 *					-# シグナルハンドラ登録
 *					-# テーブル初期化処理
 *					-# RTC時間設定
 *					-# 装置構成情報取得
 *					-# リセット要因設定
 *					-# プロセス起動
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @return		処理結果
 * @warning		dip_sw(CPRI)確認しOFFの場合pf_wdtのみ起動する。初期化完了通知は返却しない
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date  		2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 */
UINT f_main_init(VOID* rcvMsg_p)
{
	UINT						uiRet 		= D_SYS_OK;
	INT							rtn_bpf			= 0;
	UINT 						resetReason = D_RRH_RST_RSN_UNKNWN;

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_PF);	/* proc trace start */
	f_mainw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"main thread");

	/************************************************************/
	/* BPF初期化												*/
	/************************************************************/
	uiRet = f_main_iniBpf();
	if(D_SYS_OK != uiRet)
	{
		/* BPF初期化失敗のためコンソールにログ出力 */
		BPF_COM_LOG_DMESG("f_main_iniBpf error rtn = 0x%08x \n", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_init] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_MAIN,			/* ThreadID			*/
									D_SYS_THDQID_PF_MAIN,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/************************************************************/
	/* 装置種別取得												*/
	/************************************************************/
	uiRet = f_main_rrhKindGet();
	if(D_SYS_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_rrhKindGet error uiRet=%08x", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/************************************************************/
	/* 各種ログ展開												*/
	/************************************************************/
	uiRet = f_main_loadLog();
	if(D_SYS_OK != uiRet)
	{
		/* 各種ログの展開失敗 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_loadLog error uiRet=%08x", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/************************************************************/
	/* アプリ初期化開始ログ取得									*/
	/************************************************************/
	UCHAR		verFile[32];
	size_t		verSize;
	
	memset(verFile,0,sizeof(verFile));
	FILE * fp = fopen( "/home/f_5g_du/version","rb");
	if(fp != NULL){
		if (( verSize = fread( (VOID *)&verFile, 1, 17, fp )) <= 0 ){
			strncpy((char*)verFile,"Unknown1",sizeof("Unknown1"));
		}
		fclose(fp);
	}
	else{
		strncpy((char*)verFile,"Unknown2",sizeof("Unknown2"));
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "\n*** BOOT UP SW VERSION %s EEP VER:0x%x ***",verFile,f_comw_rrhVersion);
	BPF_COM_LOG_DMESG("*** BOOT UP SW VERSION %s EEP VER:0x%x ***\n",verFile,f_comw_rrhVersion);
	/************************************************************/
	/* シグナルハンドラ登録										*/
	/************************************************************/
	rtn_bpf = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY((VOID*)&f_main_sigHandler, &f_mainw_sigSetVal,D_RRH_ON);
	if(rtn_bpf != BPF_RM_SVRM_COMPLETE)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Signal Handler error. rtn_bpf=%08x", rtn_bpf);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/* ABORT時にCALLされるログ出力関数を登録しておく	*/
	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)f_com_abortLog;
	memset(&f_cmnw_svfunc,0,sizeof(T_CMN_SV_FUNCTION_LIST));
	f_cmnw_svfunc.svfunc.func_addr = (VOIDFUNCPTR)f_com_SVCtl_abort;
	/************************************************************/
	/* テーブル初期化処理										*/
	/************************************************************/
	f_main_iniTbl();
	uiRet = f_main_iniTbl_flash();
	if(D_SYS_OK != uiRet) {

		/* FLASH展開失敗	*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_iniTbl_flash error uiRet=%08x", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/************************************************************/
	/* インベントリ情報展開開始ログ取得							*/
	/************************************************************/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "INV Info Start.");

	/************************************************************/
	/* 装置構成情報取得処理										*/
	/************************************************************/
	uiRet = f_main_getInv();
	if(D_SYS_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_getInv error. uiRet=%08x", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/************************************************************/
	/* インベントリ情報展開完了ログ取得							*/
	/************************************************************/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "INV Info Completion.");

	/************************************************************/
	/* RTC時間をOSに設定										*/
	/************************************************************/
	f_main_setSysTime();

	/************************************************************/
	/* リセット要因設定											*/
	/************************************************************/
	uiRet = f_main_setRstRsn(&resetReason);
	if(D_SYS_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_setRstRsn error. uiRet=%08x", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}
	BPF_COM_CMD_SET_RESET_REASON(resetReason);

	/************************************************************/
	/* カード起動処理											*/
	/************************************************************/
	uiRet = f_main_iniCard();
	if(D_SYS_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "f_main_iniCard error. uiRet=%08x", uiRet);
		return D_SYS_OK;
	}

	/************************************************************/
	/* ダンプ開始処理											*/
	/************************************************************/
	f_main_iniEthDump();

	/************************************************************/
	/* プロセス起動開始ログ取得									*/
	/************************************************************/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Proc Create Start.");
	
	/************************************************************/
	/* プロセス起動処理											*/
	/************************************************************/
	f_main_threadCreate();
	
	/************************************************************/
	/* プロセス起動終了ログ取得									*/
	/************************************************************/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "Proc Create Completion.");

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_init] RETURN" );
	return D_SYS_OK;
}

/*!
 * @brief		f_main_init
 * @note		pf_mainスレッド初期状態処理.
 *					-# BPF初期化
 *					-# 各種ログ展開
 *					-# 装置種別取得
 *					-# シグナルハンドラ登録
 *					-# テーブル初期化処理
 *					-# RTC時間設定
 *					-# 装置構成情報取得
 *					-# リセット要因設定
 *					-# プロセス起動
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @return		処理結果
 * @warning		dip_sw(CPRI)確認しOFFの場合pf_wdtのみ起動する。初期化完了通知は返却しない
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/26 FJT)Taniguchi
 */
UINT f_main_init_ptp(VOID* rcvMsg_p)
{
	UINT						uiRet 		= D_SYS_OK;
	sigset_t sigSetVal;

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_PTP);	/* proc trace start */
	f_mainw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"ptp thread");

	/************************************************************/
	/* BPF初期化												*/
	/************************************************************/
	uiRet = f_main_iniBpf_ptp();
	if(D_SYS_OK != uiRet)
	{
		/* BPF初期化失敗のためコンソールにログ出力 */
		BPF_COM_LOG_DMESG("f_main_iniBpf error rtn = 0x%08x \n", uiRet);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNC);
	}

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PTP,				/* ProcessID		*/
									D_SYS_THDID_PF_MAIN,			/* ThreadID			*/
									D_SYS_THDQID_PF_MAIN,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/* ABORT時にCALLされるログ出力関数を登録しておく	*/
	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)f_com_abortLog;

	memset(&f_cmnw_svfunc,0,sizeof(T_CMN_SV_FUNCTION_LIST));
	f_cmnw_svfunc.svfunc.func_addr = (VOIDFUNCPTR)f_com_SVCtl_abort;

	/************************************************************/
	/* プロセス起動処理											*/
	/************************************************************/
	f_main_threadCreate();

	/* PTP用のシグナルが大量発生するため、BLOCKする  */
	sigemptyset( &sigSetVal );
	(void)sigaddset( &sigSetVal, SIGRTMAX - 1  );	/* ZLTICKTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 2  );	/* ZLAPRTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 3  );	/* ZLWDTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 10  );	/* ZLCSTTIMERSIG */
	(void)pthread_sigmask(SIG_BLOCK, &sigSetVal, NULL);
	
	return D_SYS_OK;
}

