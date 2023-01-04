/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	PTP Application Main
 *  @date	2018/4/16
 *
 *  COPYRIGHT FUJITSU LIMITED 2018
 */
/******************************************************************************************************************************/
#include <signal.h>
#include "f_ptp_inc.h"
extern int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
                           const cpu_set_t *cpuset);
extern INT duApi_ptp_start(VOID);
extern zlStatusE zl303xx_DebugGetAllAprStatistics(void * hwParams);
/* #if   defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED */
extern zl303xx_ParamsS *zl303xx_Params0;   /* The device instance structure */
/* #endif */
extern UINT	f_syncE_ssm_timeout ;
extern pthread_mutex_t f_syncE_ssm_mutex;
extern zlStatusE zl303xx_GetAprDeviceStatus(void *hwParams);
extern zlStatusE getPtpStatus(void);
extern void getPtpParameter(void);
extern VOID*	f_ptpmw_assert_p;
extern VOID f_esmc_main(VOID) ;
extern VOID f_esmc_send(VOID) ;

extern zlStatusE zl303xx_DebugAllDataSet(void);
extern zlStatusE zl303xx_DebugPllStatus77x(zl303xx_ParamsS *zl303xx_Params);
extern zlStatusE zl303xx_DebugDpllConfig77x(zl303xx_ParamsS *zl303xx_Params);
extern void zl303xx_FJ_AprGetPrint(void* fp);
extern zlStatusE zl303xx_DebugHwRefCfg77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId);
extern zlStatusE zl303xx_DebugHwRefStatus77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId);
extern zlStatusE zl303xx_Read(zl303xx_ParamsS *zl303xx_Params, void* par,
                       Uint32T regAddr, Uint32T *data);
extern zlStatusE zl303xx_Write(zl303xx_ParamsS *zl303xx_Params, void* par,
                        Uint32T regAddr, Uint32T data);
                        
extern void get_mplane_param(unsigned int reqid);                        
extern void edit_mplane_param( VOID * msgP , unsigned int reqid );
extern VOID ntc_mplane_param( VOID );
                        
static INT f_ptpcmd_analyze(VOID* msgP);
static VOID ptp_start(VOID* msgP);
static VOID ptp_shutdown(VOID* msgP);
static VOID ptp_tracelog_setting(VOID* msgP);
static VOID ptp_aprlog_setting(VOID* msgP);
static VOID ptp_get_apr_statistics(VOID* msgP);
static VOID ptp_get_plldev_state(VOID* msgP);
static VOID ptp_get_ptpclock_state(VOID* msgP);
static VOID ptp_get_sync_info(VOID* msgP);
static VOID ptp_get_ptppram(VOID* msgP);
static VOID ptp_ctl_ptpdev(VOID* msgP);

static VOID ptp_msi_rsv_analyze(VOID* msgP);

static VOID ptp_init(VOID);

/* PTPビット定義 */
#define PTP_RESET_BIT 0x00000001
#define BPF_HM_DEVC_SPI_REG_RFIC_RESET        0xA0000040

/*!
 * @brief		関数機能概要:PTP メインスレッド。
 * @note		関数処理内容.
 *				-# PTP初期化関数をCallする
 *
 * @retval		none
 * @return		none
 * @warning		N/A
 * @date 2018/4/16
 */
VOID f_ptp_main(VOID)
{
	VOID							*msgP;				/* 受信msgポインタ		*/
	UINT							rcvmsgsz;			/* 受信msgサイズ		*/

	T_RRH_HEAD*						pSendMsg;
	INT								ret;
	INT								errcd;

	unsigned char					Flag = 0;				/* SFN/FRMフラグ(from EEPROM) */

	INT								endcd		    = 0;
	T_RRH_SFNFRM_CALB_TBL			*f_sfnfrm_tbl_adr;		/*  SFN/FRM補正用テーブル	*/
	INT								ErrorCode       = 0 ;
	unsigned long int				rcv_threadId	= 0;	/*	スレッドID格納変数			*/
	unsigned long int				snd_threadId	= 0;	/*	スレッドID格納変数			*/

	T_RRH_TRAINVTBL					*f_ptp_param;			/* 共有メモリ EEPROMパラメータ	*/
	sigset_t sigSetVal;

	/* スレッド名表示 */
	prctl(PR_SET_NAME, "ptp_main", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PTP, 		/* ProcessID	:1	*/
								 D_SYS_THDID_PTP_MAIN, 	/* ThreadID			*/
								 D_SYS_THDQID_PTP_MAIN,	/* ThreadQueueID	*/
								 D_SYS_THDID_PTP_MAIN);	/* ThreadID			*/
	/* PTPメインスレッド アサートログの生成 */
	f_ptpmw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT , "PTP main thread");

	/* 共有メモリのアドレス取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
			E_RRH_SHMID_APL_SFNFRM_CALB_TBL,				/* 共有メモリ番号	*/
			(VOID **)&f_sfnfrm_tbl_adr,						/* 共有メモリアドレス	*/
			&errcd );										/* エラーコード			*/

	if( ret != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, " f_ptp_main : error = 0x%x  " , errcd);
		return ;
	}
	/* フラグアドレス保存	*/
	f_ptp_sfnfrm_flag = &f_sfnfrm_tbl_adr->sfnfrm_flag;
	f_ptp_ns_flag     = &f_sfnfrm_tbl_adr->ns_flag;

	UCHAR	t3val_h;
	UCHAR	t3val_l;

	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_T3ADJ_H , &t3val_h );
	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_T3ADJ_L , &t3val_l );
	f_ptp_t3_adj_val = (INT)((((INT)t3val_h << 8)&0xFF00) | t3val_l );
	if((f_ptp_t3_adj_val > D_DU_T23ADJVAL_MAX ) || (f_ptp_t3_adj_val < 0)){
		f_ptp_t3_adj_val = 0;
	}else{
		if(f_ptp_t3_adj_val > D_DU_T23ADJVAL_MN){
			f_ptp_t3_adj_val |= 0xFFFFFC00;		/* マイナス値に変換 */
		}
	}

	UCHAR	t2val_h;
	UCHAR	t2val_l;

	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_T2ADJ_H , &t2val_h );
	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_T2ADJ_L , &t2val_l );
	f_ptp_t2_adj_val = (INT)((((INT)t2val_h << 8)&0xFF00) | t2val_l );
	if((f_ptp_t2_adj_val > D_DU_T23ADJVAL_MAX ) || (f_ptp_t2_adj_val < 0)){
		f_ptp_t2_adj_val = 0;
	}else{
		if(f_ptp_t2_adj_val > D_DU_T23ADJVAL_MN){
			f_ptp_t2_adj_val |= 0xFFFFFC00;		/* マイナス値に変換 */
		}
	}

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "t3 adj val:%d(0x%02x%02x) t2 adj val:%d(0x%02x%02x)",
			f_ptp_t3_adj_val,t3val_h,t3val_l,
			f_ptp_t2_adj_val,t2val_h,t2val_l);

	/* SVレジスタ初期設定 (停波設定)	*/
	f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL);
	f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP);

	/*  PTP-PLLのリセット解除処理 */
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE,
										BPF_HM_DEVC_SPI_REG_RFIC_RESET,
										PTP_RESET_BIT );

	ptp_init() ;

	/* 1sec wait */
	sleep(1);

	/* PTPアプリ起動 */
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"examplePtpTelecomPhaseG8275p1Slave START");

	ret = examplePtpTelecomPhaseG8275p1Slave();
	if(ret != ZL303XX_OK) 
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"examplePtpTelecomPhaseG8275p1Slave error ret=%d", ret);
		return;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"examplePtpTelecomPhaseG8275p1Slave COMPLETE");

	/* PTP用のシグナルが大量発生するため、本スレッドではBLOCKする  */
	sigemptyset( &sigSetVal );
	(void)sigaddset( &sigSetVal, SIGRTMAX - 1  );	/* ZLTICKTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 2  );	/* ZLAPRTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 3  );	/* ZLWDTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 10  );	/* ZLCSTTIMERSIG */
	(void)pthread_sigmask(SIG_BLOCK, &sigSetVal, NULL);


	/* 共有メモリからPTP設定を取得し、グローバル変数に格納する */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_INVENTORY,			/* 共有メモリ番号		*/
						(VOID **)&f_ptp_param,				/* 共有メモリアドレス	*/
						&errcd );							/* 詳細NGコード			*/

	/* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK )
	{
		ZL303XX_TRACE_ALWAYS("shared mem get NG = %x ERRCD:%x", endcd, errcd,0,0,0,0);
		return;
	}
	/* グローバル変数に格納 */
	f_ptp_d_num					= &f_ptp_param->cmx_domain_number;
	f_ptp_m_macaddr				= &f_ptp_param->cmx_multicast_mac_address;
	f_ptp_accepted_clockclass	= &f_ptp_param->cmdx_accepted_clock_classes;

	ret = BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_ESMC_SND_FLAG, &Flag ) ;
	if( ret != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPROM SyncE ESMC Send Flag read NG. ret=%08x", ret);
		return ;
	}

/* ESMC */
	/* 受信スレッドの生成 */
	endcd = BPF_RU_ITCM_THREAD_CREATE(
					(unsigned long int *)&rcv_threadId,				/* Thread id						*/
					BPF_RU_ITCM_SCHED_FIFO,							/* Scheduling policy				*/
					D_RRH_PRIORITY_PTPR,                            /* Thread priority					*/
					D_RRH_STACKSIZE_DEFAULT,                        /* Thread stack size				*/
					(FUNCPTR)f_esmc_main,                           /* IAスレッドメイン関数ポインタ		*/
					D_RRH_NULL,                                     /* オプション						*/
					&ErrorCode ) ;                                  /* Error code						*/
	cmn_com_assert_th_get(f_ptpmw_assert_p,D_RRH_LOG_AST_LV_INFO, "ptp_esmcrcv Thread create start");

	if(endcd != BPF_RU_ITCM_OK)
	{
		/* ERRORの場合 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE ptp_esmcrcv NG %d threadid=%ld thdPriority=%d thdStackSize=%d",
					endcd, rcv_threadId, D_RRH_PRIORITY_PTPR ,D_RRH_STACKSIZE_DEFAULT);
		return;
	}
	cmn_com_assert_th_get( f_ptpmw_assert_p,D_RRH_LOG_AST_LV_INFO, "ptp_esmcrcv Thread create comp  id:0x%lx pri:%d stack:0x%x",
						   rcv_threadId, D_RRH_PRIORITY_PTPR ,D_RRH_STACKSIZE_DEFAULT);

	if( Flag != D_RRH_EEP_SYNCE_NOACT)
	{
		/* 送信スレッドの生成 */
		endcd = BPF_RU_ITCM_THREAD_CREATE(
						(unsigned long int *)&snd_threadId,				/* Thread id						*/
						BPF_RU_ITCM_SCHED_FIFO,							/* Scheduling policy				*/
						D_RRH_PRIORITY_PTPS,                            /* Thread priority					*/
						D_RRH_STACKSIZE_DEFAULT,                        /* Thread stack size				*/
						(FUNCPTR)f_esmc_send,                           /* IAスレッドメイン関数ポインタ		*/
						D_RRH_NULL,                                     /* オプション						*/
						&ErrorCode ) ;                                  /* Error code						*/
		cmn_com_assert_th_get(f_ptpmw_assert_p,D_RRH_LOG_AST_LV_INFO, "ptp_esmcsnd Thread create start");

		if(endcd != BPF_RU_ITCM_OK)
		{
			/* ERRORの場合 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE ptp_esmcsnd NG %d threadid=%ld thdPriority=%d thdStackSize=%d",
						endcd, snd_threadId, D_RRH_PRIORITY_PTPR ,D_RRH_STACKSIZE_DEFAULT);
			return;
		}
		cmn_com_assert_th_get( f_ptpmw_assert_p,D_RRH_LOG_AST_LV_INFO, "ptp_esmcsnd Thread create comp  id:0x%lx pri:%d stack:0x%x",
							   snd_threadId, D_RRH_PRIORITY_PTPR ,D_RRH_STACKSIZE_DEFAULT);
	}
/* ESMC-end */

	/* 初期化応答用 バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別		*/
									  sizeof(T_SYS_INIT_COMP_NTC), 	/* MessageSize 		*/
									  (VOID **) &pSendMsg, 			/* MessageBuffer	*/
									  &errcd); 						/* ErrorCode 		*/
	/* OAL処理結果チェック	*/
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return;
	}

	/* ヘッダ設定	*/
	pSendMsg->uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;/* Event ID				*/
	pSendMsg->uiSignalkind = 0; 					/* Signal Kind			*/
	pSendMsg->uiDstPQueueID = D_RRH_PROCQUE_PTP; 	/* Destination P QID	*/
	pSendMsg->uiDstTQueueID = D_SYS_THDQID_PF_MAIN;	/* Destination T QID	*/
	pSendMsg->uiSrcPQueueID = D_RRH_PROCQUE_PTP; 	/* Source P QID			*/
	pSendMsg->uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN; 	/* Source T QID			*/

	/* 初期化応答を送信する	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( pSendMsg->uiDstPQueueID,	/* 送信先Proc Queue ID	*/
								(VOID *) pSendMsg); 	/* 送信Message			*/

	/* OAL処理結果チェック	*/
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}


	while(1) {
		/* メッセージ受信	*/
		ret = BPF_RU_IPCM_MSGQ_RECV( D_SYS_THDQID_PTP_MAIN,	/* Thread Qid		*/
									 0,						/* 応答待ち時間		*/
									 (void **)&msgP,		/* 受信msgポインタ	*/
									 &rcvmsgsz );			/* 受信msgサイズ	*/
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}

		/* 受信メッセージ解析関数Call(戻り値使わない)	*/
		(VOID)f_ptpcmd_analyze((VOID*)msgP);

		/* 通信用共有メモリの解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)msgP );
		
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
	}
	return;
}

/*!
 * @brief		関数機能概要:PTPコマンド分析
 *
 * @retval		none
 * @return		none
 * @warning		N/A
 * @date 2018/4/26
 */
INT f_ptpcmd_analyze(VOID* msgP)
{
	/* 受信msgチェック	*/
	switch( ((T_SYS_COMMON_THDIF *)msgP)->uiEventNo )
	{
	case D_API_MSGID_PTP_START_REQ:										/* PTP起動要求	*/
		ptp_start(msgP);
		break;
	
	case D_API_MSGID_PTP_SHUTDOWN_REQ:									/* PTP終了要求	*/
		ptp_shutdown(msgP);
		break;
		
	case D_API_MSGID_PTP_TLOG_SET_REQ:									/* PTP TraceLog設定	*/
		ptp_tracelog_setting(msgP);
		break;

	case D_API_MSGID_PTP_APRLOG_SET_REQ:								/* PTP AprTraceLog設定	*/
		ptp_aprlog_setting(msgP);
		break;

	case D_API_MSGID_PTP_TRAFFIC_GET_REQ:								/* 統計情報取得	*/
		ptp_get_apr_statistics(msgP);
		break;

	case D_API_MSGID_PTP_DEV_STATE_GET_REQ:								/* PLL 状態取得	*/
		ptp_get_plldev_state(msgP);
		break;

	case D_API_MSGID_PTP_CLOCK_GET_REQ:									/* PTP 状態取得	*/
		ptp_get_ptpclock_state(msgP);
		break;

	case D_API_MSGID_PTP_SYNC_INFO_REQ:									/* PTP SYNC INFO	*/
		ptp_get_sync_info(msgP);
		break;

	case D_API_MSGID_PTP_PRAM_GET_REQ:									/* PTPパラメータ取得	*/
		ptp_get_ptppram(msgP);
		break;

	case D_API_MSGID_PTP_DEV_CTL_REQ:									/* PTPデバイス制御	*/
		ptp_ctl_ptpdev(msgP);
		break;

	case D_SYS_MSGID_MSI_RCV_IND:										/* MSI受信通知(仮)	*/
		ptp_msi_rsv_analyze(msgP);
		break;

	case D_SYS_MSGID_PTPSTSTO_NTC	:									/* PTP-状態保護タイムアウト */
		f_ptp_status_notif_ptpto(msgP);
		break;

	case D_SYS_MSGID_SYNCESTSTO_NTC	:									/* SyncE-状態保護タイムアウト */
		f_ptp_status_notif_synceto(msgP);
		break;

	case D_SYS_MSGID_SYNCSTSTO_NTC	:									/* Sync-状態保護タイムアウト */
		f_ptp_status_notif_syncto(msgP);
		break;

	case D_SYS_MSGID_SYNCESSMTO_NTC	:									/* SyncE-SSMタイムアウト */
		pthread_mutex_lock(&f_syncE_ssm_mutex);
		f_syncE_ssm_timeout = SYNCE_SSM_TIMEOUT_TRUE;
		pthread_mutex_unlock(&f_syncE_ssm_mutex);
		break;

	default:															/* 起動要因異常			*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Unknown Message Recieve :%d", ((T_SYS_COMMON_THDIF *)msgP)->uiEventNo);
	}

	return 0;
}

/* PTP起動要求	*/
static VOID ptp_start(VOID* msgP)
{
	INT ret;
	INT rtnPtp;
	INT errcd;
	T_API_PTP_START_RSP *sendMsg;

	
	/* PTP起動  */
	rtnPtp = (INT)examplePtpTelecomPhaseG8275p1Slave();
	if(rtnPtp != 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"examplePtpTelecomPhaseG8275p1Slave error ret=%d", rtnPtp);
	}

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK, 	/* Buffer種別						*/
									  sizeof(T_API_PTP_START_RSP),		/* MessageSize						*/
									  (VOID **) &sendMsg,				/* MessageBuffer					*/
									  &errcd); 							/* ErrorCode						*/
	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
						   "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%08x, error=%08x", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_START_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_START_RSP);

	sendMsg->data.Result = rtnPtp;

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	
	if(rtnPtp == 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"PTP START OK! ret=%d", rtnPtp);
	}
	else if(rtnPtp == ZL303XX_UNSUPPORTED_OPERATION )
	{
		/* 二重起動 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"PTP Already started ret=%d", rtnPtp);
	}
	else{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"PTP START FAILED ret=%d", rtnPtp);
	}
	
	return;
}

/* PTP終了要求	*/
static VOID ptp_shutdown(VOID* msgP)
{
	INT ret;
	INT rtnPtp;
	T_API_PTP_SHUTDOWN_RSP* sendMsg;
	INT errcd;

	/* PTP停止 */
	rtnPtp = (INT)exampleShutdown();
	if(rtnPtp != 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"exampleShutdown error ret=%d", rtnPtp);
	}

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK, 	/* Buffer種別						*/
									  sizeof(T_API_PTP_SHUTDOWN_RSP),		/* MessageSize					*/
									  (VOID **) &sendMsg,				/* MessageBuffer					*/
									  &errcd); 							/* ErrorCode						*/
	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_START_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_SHUTDOWN_RSP);

	sendMsg->data.Result = rtnPtp;

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}


	if(rtnPtp == 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"PTP Shutdown OK! ret=%d", rtnPtp);
	}
	else if(rtnPtp ==  ZL303XX_UNSUPPORTED_OPERATION )
	{
		/* 二重停止 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"PTP Already stopped ret=%d", rtnPtp);
	}
	else{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"PTP SHUTDOWN FAILED ret=%d", rtnPtp);
	}

	return;
}

/* PTP TraceLog設定	*/
static VOID ptp_tracelog_setting(VOID* msgP)
{
	T_API_PTP_TLOG_SET_REQ* recvMsg = (T_API_PTP_TLOG_SET_REQ*)msgP;
	
	/* Traceログレベル設定 */

	if(recvMsg->data.dbgMode == ALL_TRACE_LOG_LEVEL_MODID)
    {
       	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"zl303xx_TraceSetLevelAll dbgLevel=%d", recvMsg->data.dbgLevel);
		zl303xx_TraceSetLevelAll(recvMsg->data.dbgLevel);
    }
	else
    {
       	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"zl303xx_TraceSetLevel dbgMode=%d, dbgLevel=%d", recvMsg->data.dbgMode, recvMsg->data.dbgLevel);
		zl303xx_TraceSetLevel(recvMsg->data.dbgMode, recvMsg->data.dbgLevel);

    }

	return;
}

/* PTP AprTraceLog設定	*/
static VOID ptp_aprlog_setting(VOID* msgP)
{
	unsigned char level;
	T_API_PTP_APRLOG_SET_REQ* recvMsg = (T_API_PTP_APRLOG_SET_REQ*)msgP;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"zl303xx_SetAprLogLevel dbgLevel=%d", recvMsg->data.dbgLevel);
	zl303xx_SetAprLogLevel(recvMsg->data.dbgLevel);

	/* 設定確認 */
	
	level = zl303xx_GetAprLogLevel();
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"zl303xx_SetAprLogLevel dbgLevel=%d", level);

	return;
}

/* 統計情報取得	*/
static VOID ptp_get_apr_statistics(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_TRAFFIC_GET_RSP	*sendMsg;

	int tmp_fd;
	FILE* fp;

	tmp_fd = dup(STDOUT_FILENO);
	if(tmp_fd == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup(STDOUT_FILENO) error. errno=%d", errno);
		return;
	}

	fp = fopen(PTP_DEBUG_DISP_LOG, "w+");
	if(fp == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Pointer NULL.");
		close(tmp_fd);
		return;
	}
	/* 標準出力をファイルへ出力 */
	if( dup2(fileno(fp), STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(fileno(fp), STDOUT_FILENO) ERROR. errno=%d", errno);
		close(tmp_fd);
		fclose(fp);
		return;
	}

	/* 統計情報収集	*/
	zl303xx_DebugGetAllAprStatistics(zl303xx_Params0);

	/* 標準出力を元に戻す */
	if( dup2(tmp_fd, STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(tmp_fd, STDOUT_FILENO) ERROR. errno=%d", errno);
	}

	close(tmp_fd);
	fflush(fp);
	fclose(fp);

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_TRAFFIC_GET_RSP),		/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_TRAFFIC_GET_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_TRAFFIC_GET_RSP);

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}

/* PLL 状態取得	*/
static VOID ptp_get_plldev_state(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_DEV_STATE_GET_RSP	*sendMsg;

	int tmp_fd;
	FILE* fp;

	tmp_fd = dup(STDOUT_FILENO);
	if(tmp_fd == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup(STDOUT_FILENO) error. errno=%d", errno);
		return;
	}

	fp = fopen(PTP_DEBUG_DISP_LOG, "w+");
	if(fp == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Pointer NULL.");
		close(tmp_fd);
		return;
	}
	/* 標準出力をファイルへ出力 */
	if( dup2(fileno(fp), STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(fileno(fp), STDOUT_FILENO) ERROR. errno=%d", errno);
		close(tmp_fd);
		fclose(fp);
		return;
	}

	zl303xx_GetAprDeviceStatus(zl303xx_Params0);

	/* 標準出力を元に戻す */
	if( dup2(tmp_fd, STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(tmp_fd, STDOUT_FILENO) ERROR. errno=%d", errno);
	}

	close(tmp_fd);
	fflush(fp);
	fclose(fp);

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_DEV_STATE_GET_RSP),	/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_DEV_STATE_GET_REQ;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_DEV_STATE_GET_RSP);

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}

/* PTP 状態取得	*/
static VOID ptp_get_ptpclock_state(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_CLOCK_GET_RSP		*sendMsg;

	int tmp_fd;
	FILE* fp;

	tmp_fd = dup(STDOUT_FILENO);
	if(tmp_fd == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup(STDOUT_FILENO) error. errno=%d", errno);
		return;
	}

	fp = fopen(PTP_DEBUG_DISP_LOG, "w+");
	if(fp == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Pointer NULL.");
		close(tmp_fd);
		return;
	}
	/* 標準出力をファイルへ出力 */
	if( dup2(fileno(fp), STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(fileno(fp), STDOUT_FILENO) ERROR. errno=%d", errno);
		close(tmp_fd);
		fclose(fp);
		return;
	}

	(VOID)getPtpStatus();

	/* 標準出力を元に戻す */
	if( dup2(tmp_fd, STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(tmp_fd, STDOUT_FILENO) ERROR. errno=%d", errno);
	}

	close(tmp_fd);
	fflush(fp);
	fclose(fp);

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_CLOCK_GET_RSP),		/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_CLOCK_GET_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_CLOCK_GET_RSP);

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}

/* PTP SYNC 情報 */
static VOID ptp_get_sync_info(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_PRAM_GET_RSP		*sendMsg;

	int							tmp_fd;
	FILE*						fp;


	tmp_fd = dup(STDOUT_FILENO);
	if(tmp_fd == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup(STDOUT_FILENO) error. errno=%d", errno);
		return;
	}

	fp = fopen(PTP_DEBUG_DISP_LOG, "w+");
	if(fp == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Pointer NULL.");
		close(tmp_fd);
		return;
	}
	/* 標準出力をファイルへ出力 */
	if( dup2(fileno(fp), STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(fileno(fp), STDOUT_FILENO) ERROR. errno=%d", errno);
		close(tmp_fd);
		fclose(fp);
		return;
	}

	UINT mode = ((T_API_PTP_PRAM_GET_REQ*)msgP)->header.uiSignalkind;
	if((mode == 0) || ((mode & 0x1) != 0)){
		printf("zl303xx_DebugAllDataSet --------------------------------------\n");
		(VOID)zl303xx_DebugAllDataSet();
	}
	if((mode == 0) || ((mode & 0x2) != 0)){
		printf("zl303xx_DebugGetAllAprStatistics -----------------------------\n");
		zl303xx_DebugGetAllAprStatistics(zl303xx_Params0);
	}
	if((mode == 0) || ((mode & 0x4) != 0)){
		printf("zl303xx_GetAprDeviceStatus -----------------------------------\n");
		zl303xx_GetAprDeviceStatus(zl303xx_Params0);
	}

	if((mode == 0) || ((mode & 0x8) != 0)){
		printf("zl303xx_DebugPllStatus77x ------------------------------------\n");
		zl303xx_DebugPllStatus77x(zl303xx_Params0);
	}

	if((mode == 0) || ((mode & 0x10) != 0)){
		printf("zl303xx_DebugDpllConfig77x -----------------------------------\n");
		zl303xx_DebugDpllConfig77x(zl303xx_Params0);
	}

	if((mode == 0) || ((mode & 0x20) != 0)){
		printf("zl303xx_DebugHwRefCfg77x ZLS3077X_REF_MAX --------------------\n");
		zl303xx_DebugHwRefCfg77x(zl303xx_Params0,ZLS3077X_REF_MAX);
	}

	if((mode == 0) || ((mode & 0x40) != 0)){
		printf("zl303xx_DebugHwRefStatus77x ZLS3077X_REF_MAX -----------------\n");
		zl303xx_DebugHwRefStatus77x(zl303xx_Params0,ZLS3077X_REF_MAX);
	}

	if((mode == 0) || ((mode & 0x80) != 0)){
		printf("zl303xx_Params.pllParams -------------------------------------\n");
		printf("pllParams.pllId        : 0x%x\n",zl303xx_Params0->pllParams.pllId);
		printf("pllParams.dcoFreq      : 0x%x\n",zl303xx_Params0->pllParams.dcoFreq);
		printf("pllParams.selectedRef  : 0x%x\n",zl303xx_Params0->pllParams.selectedRef);
		printf("pllParams.bRefSyncPair : 0x%x\n",zl303xx_Params0->pllParams.bRefSyncPair);
#if defined ZLS30361_INCLUDED
		printf("pllParams.pllPriorMode : 0x%x\n",zl303xx_Params0->pllParams.d77x.pllPriorMode);
#endif
	}

	
	if((mode == 0) || ((mode & 0x100) != 0)){
		printf("getPtpStatus -------------------------------------------------\n");
		(VOID)getPtpStatus();
	}

	if((mode == 0) || ((mode & 0x200) != 0)){
		printf("zl303xx_FJ_AprGetPrint ---------------------------------------\n");
		zl303xx_FJ_AprGetPrint(stdout);
	}
	
	/* 標準出力を元に戻す */
	if( dup2(tmp_fd, STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(tmp_fd, STDOUT_FILENO) ERROR. errno=%d", errno);
	}

	close(tmp_fd);
	fflush(fp);
	fclose(fp);

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_SYNC_INFO_REQ),		/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_SYNC_INFO_NTF;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_SYNC_INFO_REQ);

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}


/* debug function */
VOID f_ptp_debug_master(VOID){
	/*  PTP-PLLのリセット解除処理 */
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE,
										BPF_HM_DEVC_SPI_REG_RFIC_RESET,
										PTP_RESET_BIT );
	
	/* 1sec wait */
	sleep(1);
	zlStatusE ret = examplePtpTelecomPhaseG8275p1Master();
	while(1){
		if(ret == ZL303XX_OK){
			sleep(1);
		}else{
			printf("Master setting error:%d\n",ret);
			break;
		}
	}
	return;
}

/* debug function */
VOID f_ptp_debug_slave(VOID){
	/*  PTP-PLLのリセット解除処理 */
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE,
										BPF_HM_DEVC_SPI_REG_RFIC_RESET,
										PTP_RESET_BIT );
	
	/* 1sec wait */
	sleep(1);
	zlStatusE ret = examplePtpTelecomPhaseG8275p1Slave();
	while(1){
		if(ret == ZL303XX_OK){
			sleep(1);
		}else{
			printf("Slave setting error:%d\n",ret);
			break;
		}
	}
	return;
}

/* PTPパラメータ取得	*/
static VOID ptp_get_ptppram(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_PRAM_GET_RSP		*sendMsg;

	int							tmp_fd;
	FILE*						fp;


	tmp_fd = dup(STDOUT_FILENO);
	if(tmp_fd == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup(STDOUT_FILENO) error. errno=%d", errno);
		return;
	}

	fp = fopen(PTP_DEBUG_DISP_LOG, "w+");
	if(fp == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Pointer NULL.");
		close(tmp_fd);
		return;
	}
	/* 標準出力をファイルへ出力 */
	if( dup2(fileno(fp), STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(fileno(fp), STDOUT_FILENO) ERROR. errno=%d", errno);
		close(tmp_fd);
		fclose(fp);
		return;
	}

	(VOID)getPtpParameter();

	/* 標準出力を元に戻す */
	if( dup2(tmp_fd, STDOUT_FILENO) == -1)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "dup2(tmp_fd, STDOUT_FILENO) ERROR. errno=%d", errno);
	}

	close(tmp_fd);
	fflush(fp);
	fclose(fp);

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_CLOCK_GET_RSP),		/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}

	sendMsg->header.uiEventNo = D_API_MSGID_PTP_PRAM_GET_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_PRAM_GET_RSP);

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}



/* PTP デバイス制御	*/
static VOID ptp_ctl_ptpdev(VOID* msgP)
{
	int							ret;
	int							errcd;
	T_API_PTP_DEV_CTL_REQ		*recvMsg;
	T_API_PTP_DEV_CTL_RSP		*sendMsg;
	zlStatusE					status = ZL303XX_ERROR;
	UINT						value = 0;
	
	recvMsg = (T_API_PTP_DEV_CTL_REQ*)msgP;

	switch(recvMsg->mode){
		case 1:
			status = zl303xx_Read(zl303xx_Params0, NULL, recvMsg->addr,&value);
			break;
		case 2:
			status = zl303xx_Write(zl303xx_Params0, NULL, recvMsg->addr,recvMsg->value);
			// sendMsg->value = recvMsg->value;
			value = recvMsg->value;
			break;
		default:
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "mode error=%d", recvMsg->mode);
			break;
	}

	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
				E_BPF_RU_IPCM_BUFF_KIND_TASK, 			/* Buffer種別		*/
				sizeof(T_API_PTP_DEV_CTL_RSP),			/* MessageSize		*/
				(VOID **)&sendMsg,						/* MessageBuffer	*/
				&errcd); 								/* ErrorCode		*/

	if (ret != E_API_RCD_OK)
	{
		/* BPF_RU_IPCM_PROCMSG_ADDRGET異常 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%d", ret);
		return;
	}
	
	sendMsg->header.uiEventNo = D_API_MSGID_PTP_DEV_CTL_RSP;
	sendMsg->header.uiDstPQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcPQueueID;
	sendMsg->header.uiDstTQueueID = ((T_SYS_COMMON_THDIF *)msgP)->uiSrcTQueueID;
	sendMsg->header.uiSrcPQueueID = D_RRH_PROCQUE_PTP;
	sendMsg->header.uiSrcTQueueID = D_SYS_THDQID_PTP_MAIN;
	sendMsg->header.uiLength = sizeof(T_API_PTP_DEV_CTL_RSP);
	sendMsg->mode = recvMsg->mode;
	sendMsg->addr = recvMsg->addr;
	sendMsg->value = value;
	sendMsg->result = (UINT)status;

	ret = BPF_RU_IPCM_MSGQ_SEND(sendMsg->header.uiDstPQueueID, sendMsg);
	if (ret != E_API_RCD_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	}
	return;
}


/* MSI受信要求時処理 */

/****************************************************************************************************************************/
/*!
 * @brief		ptp_msi_rsv_analyze
 * @note		MSI受信要求時処理
 *				-# MessageIDを見てget要求かedit要求かを判別
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		void
 * @date		2018/11/24 Suehiro Create
 *              2018/12/3  #12358 汎用で受けれるように修正
 */
/****************************************************************************************************************************/
static VOID ptp_msi_rsv_analyze(void* msgP)
{
	
	UINT						messageId;
	UINT						requestId;
	INT							payloadSizePtr;
	VOID *						payloadPtr;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_ptp_main] ptp_msi_rsv_analyze() start." );

	/* MSIデータ取得関数 f_com_MSIgetPayload() */
	f_com_MSIgetPayload( msgP,					/* [in]  Receive Message pointer			*/
						 &messageId,			/* [out] Receive Message ID					*/
						 &requestId,			/* [out] Receive Message Request ID			*/
						 &payloadSizePtr,		/* [out] Receive Message payload size		*/
						 &payloadPtr );			/* [out] Receive Message payload Pointer	*/

	/* Message IDをみて処理を振り分け */
	switch( messageId )
	{
		case D_MSG_MSI_O_RAN_SYNC_GET_REQ:
			/* パラメータGet処理 */
			(VOID)get_mplane_param(requestId);
			break;

		case D_MSG_MSI_O_RAN_SYNC_EDIT_REQ:
			/* パラメータEdit処理 */
			(VOID)edit_mplane_param( payloadPtr , requestId ) ;
			break ;

		case D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND:
			/* パラメータEdit処理 */
			(VOID)ntc_mplane_param( ) ;
			break ;

		default:
			/* こないはずだけど念のため */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"invalid Message ID ret=%d", messageId);
			break;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_ptp_main] ptp_msi_rsv_analyze() end." );
	return;

}


/* PTP制御初期設定 */

/****************************************************************************************************************************/
/*!
 * @brief		ptp_init
 * @note		PTP制御初期設定処理
 *				-# PTP制御で使用する各種データを初期化する
 * @param		なし
 * @retval		void
 * @date		2019/02/25 LCN)kamimoto Create
 */
/****************************************************************************************************************************/
static VOID ptp_init(void)
{
	
	

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_ptp_main] ptp_init() start." );

	/* タイマ設定データの初期化 */
	f_ptp_reporting_period		= D_RRH_ZERO ;
	f_syncE_reporting_period	= D_RRH_ZERO ;
	f_sync_reporting_period		= D_RRH_ZERO ;
	f_ptp_reporting_guarded		= D_RRH_NUMFFFFFFFF ;
	f_syncE_reporting_guarded	= D_RRH_NUMFFFFFFFF ;
		
	
	/* ptp status dataの初期化 */
	f_ptp_status_data	= D_RRH_NUMFFFFFFFF ;
	f_sync_status_data	= D_RRH_NUMFFFFFFFF ;
	f_synce_status_data	= D_RRH_NUMFFFFFFFF ;

	return ;
}


