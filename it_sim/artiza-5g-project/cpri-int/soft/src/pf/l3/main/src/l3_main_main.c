/*!
 *  @skip   $Id$
 *  @file   l3_main_main.c
 *  @brief  the entry of l3 server
 *  @date   2013/11/11 FFCS)linb create for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_L3_MAIN
 * @{
 */

#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "l3_com_inc.h"
#include "l3_main_inc.h"
#include "f_com_aplCom.h"
#include "f_cmn_com_inc.h"
#include "rrhApi_Inv.h"
#include "rrhApi_L2.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_Svp.h"
#include "rrhApi_File.h"

extern VOID l3_main_logPrint(FILE *outf);

/*!
 *  @brief  the proccess of hunter server
 *  @note   This function is processed as follows. \n
 *          - directly call apl_main_main
 *          - send response to init process
 *          - receive system init ntc from  init process
 *          - subscribe cpri message to l2 proc
 *          - send cpri link startup notice to l3_cpr
 *  @return VOID
 *  @retval none
 *  @date   2013/11/11 FFCS)linb create for zynq\n
 */

INT main(INT argc, CHAR *argv[])
{
#ifdef OPT_RRH_SIM
	printf("********L3 server**********\n");
#endif
	/* process init						*/
	l3_main_Init();
	cm_Assert(CMD_ASL_NOLEVEL,0,"l3 init OK");

	/* process create						*/
	l3_main_Create();
	cm_Assert(CMD_ASL_NOLEVEL,1,"l3 threads OK");
printf("l3 threads OK\n");//C9 add code debug
	 /* init process */
	l3_main_systemInit();
	cm_Assert(CMD_ASL_NOLEVEL,2,"l3 systemInit OK");
printf("l3 systemInit OK\n");//C9 add code debug
	/* ログ作成スレッド生成 */
	f_cmn_com_log_threadCreate( "l3", l3_main_logPrint, CMD_TSKID_LGMK );
	
#ifdef OPT_RRH_SIM
	printf("********L3 server finish**********\n");
#endif

//#ifndef OPT_RRH_SIM
	l3_main_signalProc();
//#else
	getchar();
//#endif
	return 0;
}

sigset_t			sigSetVal;

/*!
 *  @brief  the process of init API
 *  @note   This function is processed as follows. \n
 *          - call API_L3_Init();
 *  @return VOID
 *  @retval none
 *  @date   2013/11/11 FFCS)linb create for zynq\n
 */
INT	l3_main_Init(VOID)
{
	int	rtn;
//#ifndef OPT_RRH_SIM
	rtn = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( (INT*)&l3_main_signal, &sigSetVal , D_RRH_ON);
//#endif

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_L3);	/* proc trace start */
	/***********************************/
	/*	Process manage function assign */
	/***********************************/
	rtn = BPF_RM_SVRM_ASSIGN( D_RRH_PROCID_L3);
	/*NG case*/
	if (rtn != BPF_RM_SVRM_COMPLETE) 
	{  
		return D_RRH_NG; 
	}

	BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_L3,		/* ProcessID		*/
								D_RRH_THDID_L3_MAIN,	/* ThreadID			*/
								CMD_TSKID_INI,			/* ThreadQueueID	*/
								(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	/* read cmw_sys_mng_tbl*/
	rrhApi_Inv_Fsm_GetSystemPara((VOID*)cmw_sys_mng_tbl,sizeof(cmw_sys_mng_tbl));
	/* read cmw_re_dev_tbl*/
	rrhApi_Inv_Fsm_GetDevInfo((VOID*)&cmw_re_dev_tbl,sizeof(cmw_re_dev_tbl));
	/* read cmw_tra_inv_tbl*/
	rrhApi_Inv_Fsm_GetTraInvInfo((VOID*)&cmw_tra_inv_tbl,sizeof(cmw_tra_inv_tbl));
	/* read cmw_device_kind*/
	rrhApi_Inv_Fsm_GetDevKind((VOID*)&cmw_device_kind,sizeof(cmw_device_kind));

	/* Set RE Kind according to device kind */
	switch(cmw_device_kind)
	{			
		case E_RRH_RRHKIND_SRE_35G:
			cmw_device_kind = CMD_DEVKND_RRE_35G_SRE;
			cmw_reKind = E_RRH_REKIND_TDD_SRE;
			break;

		default:
			cm_Assert(CMD_ASL_DBGLOW, cmw_device_kind,  "device kind error.");
			break;

	}

	/* read cmw_slf_inv_tbl*/
	rrhApi_Inv_Fsm_GetSlfInvInfo((VOID*)&cmw_slf_inv_tbl,sizeof(cmw_slf_inv_tbl));

	return D_RRH_OK;
}

extern  T_SYS_THRDINFO l3_comr_ThrMngTbl[D_L3_THRDNO_MAX];

typedef void (*VOIDFUNCPTR)(void);
T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;

/********************************************************************************************************************/
/*!
 *  @brief  Create threads.
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01685
 *  @date   2013/11/15 FFCS)linb Create 
 *  @date   2015/10/05 TDIPS)maruyama M-RRU-ZSYS-01685 デバッグスレッドが生成されない不具合を修正
 *  
 *  
 */
/********************************************************************************************************************/
VOID l3_main_Create()
{
	INT    ret;

	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)l3_main_abortLog;
	ret = apl_main_main(D_RRH_PROCID_L3,D_RRH_PROCQUE_L3,l3_comr_ThrMngTbl,D_L3_THRDNO_MAX -2);//c9 擬似環境
	if(ret != D_RRH_OK)
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_NG);
		return ;
	}
	else
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_OK);
	}

	BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_L3,		/* ProcessID		*/
								D_RRH_THDID_L3_MAIN,	/* ThreadID			*/
								CMD_TSKID_INI,			/* ThreadQueueID	*/
								(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	l3_mainw_stateno = L3_MAIN_ST_USE;

	return;
}

/* この定義は暫定です。必要に応じて各スレッド内のテーブルを追加してください */
static bpf_rm_svrm_sigget_tbl_t log_siggettble[] = 
{
	{"l3_mainw_stateno"			 ,sizeof(l3_mainw_stateno)		,&l3_mainw_stateno,4},
	{"l3_mainw_eventno"			 ,sizeof(l3_mainw_eventno)		,&l3_mainw_eventno,4}
};

/********************************************************************************************************************/
/*!
 *  @brief  the process of related linux signal.
 *  @note   Create threads.
 *  @param  signum  [in]  signal number
 *  @param  info    [in]  signal pointer
 *  @param  ctx     [in]  signal context
 *  @param  fd      [in]  file descriptor
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/12/05 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
INT l3_main_signal( INT signum, VOID *info, VOID *ctx , INT fd)
{
//#ifndef OPT_RRH_SIM
	if(fd > 0)
	{
		/* 注意:											 */
		/* 本関数はシグナルハンドラ上で動作する				 */
		/* 本関数内では非同期シグナル安全な関数のみ使用可能	 */
		BPF_RM_SVRM_SIGNAL_TBLSAVE(fd,
							   sizeof(log_siggettble)/sizeof(log_siggettble[0]),
							   (char*)log_siggettble
							   );
	}
//#endif	
	return (sizeof(log_siggettble)/sizeof(log_siggettble[0])) ;
}

void l3_main_signalProc()
{
	siginfo_t			sigInfo;
	int					ret;

	for(;;)
	{
		ret = BPF_RM_SVRM_SIGNAL_WAIT(&sigSetVal,
									&sigInfo.si_pid,
									&sigInfo.si_signo,
									&sigInfo.si_status);
		if( ret != BPF_RM_SVRM_OK )
		{
			cm_Assert(CMD_ASL_DBGLOW,ret, "signal wait error. ");
			continue;
		}
		
		l3_main_logSave_and_Rsp( sigInfo.si_signo, sigInfo.si_status );
	}
	
	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  the process of save log
 *  @note   the process of save log.
 *  @return void
 *  @retval -
 *  @date   2013/12/05 FFCS)hongj Create for zynq
 *  @date   2015/10/23 FPT)Lay FHM IT3の問処No15対応
 */
/********************************************************************************************************************/
extern void  m_cr_logPrint(FILE *);
extern void  m_dl_logPrint(FILE *);
extern void  m_rc_logPrint(FILE *);
VOID l3_main_logPrint(FILE *outf)
{
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT2
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	m_cr_logPrint(outf);
	m_dl_logPrint(outf);
	m_rc_logPrint(outf);
	BPF_RU_HRTM_TIMER_LOG_PRINT(outf);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT2
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

	return;
}

VOID l3_main_abortLog()
{
	FILE				*fp;
	UCHAR				output_file[256];
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l3log_abort", E_BPF_LOG_CMDLOG_OUTDIR_NORMAL, output_file )) != NULL )
	{
		l3_main_logPrint(fp);
	
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN l3log_abort" );

	return;
}

VOID l3_main_logSave(INT kind)
{
//#ifndef OPT_RRH_SIM
	FILE					*fp;
	UCHAR				output_file[256];

	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l3", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file );	
	if( fp != NULL )
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
//#endif
	exit(0);
}


VOID l3_main_logSave_and_Rsp(INT signo, INT option)
{
//#ifndef OPT_RRH_SIM
//	FILE				*fp = NULL;
//	UCHAR				output_file[256];
	UINT				output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
	INT					sig_option = 0;
	
	switch( signo )
	{
	/* SIGUSR1は障害/RESET/HealthCheck断/運用ログ出力要求などがある	*/
	case SIGUSR1:
		if( D_RRH_SIGUSR_OPT_TROUBLE_REQ == option )
		{
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
			sig_option = D_RRH_SIGUSR_OPT_TROUBLE_RES;
		}
		else if( D_RRH_SIGUSR_OPT_TROUBLE_REQ_RST == option )
		{
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM_RST;
			sig_option = D_RRH_SIGUSR_OPT_TROUBLE_RES;
		}
		else if( D_RRH_SIGUSR_OPT_HLTCHK_REQ == option )
		{
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK;
			sig_option = D_RRH_SIGUSR_OPT_HLTCHK_RES;
		}
		else if( D_RRH_SIGUSR_OPT_RUNLOG_GET == option )
		{
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_SEND;
			sig_option = D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI;
		}
		else if( D_RRH_SIGUSR_OPT_RUNLOG_GET_3G == option )
		{
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_SEND_3G;
			sig_option = D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G;
		}		
		else
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "In SIGUSR1 signal kind error si_status=%x", option );
		}
		break;

	case SIGUSR2:	
		//BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "SIGUSR2 happened!!");
#ifdef FHM_SIGUSR1_DEBUG_FOR_IT1
    printf( "[%d]%s <SIGUSR2 signo>0x%x\n", __LINE__, __FUNCTION__,signo);
#endif
		break;

	/* logスレッドで障害ログ取得時のファイル書庫処理時にsystem()による作成された子プロセスが、
 	 * 実行し終わった後に自ら終了してSIGCHLDによりl3の親プロセスに通知する場合がある。
 	 */
	case SIGCHLD:	
//		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "SIGCHLD happened!!");
#ifdef FHM_SIGUSR1_DEBUG_FOR_IT1
//    printf( "[%d]%s <SIGCHLD signo>0x%x\n", __LINE__, __FUNCTION__,signo);
#endif
		break;

	default:
		//BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Oher signal happened!!");
		break;
	}
#ifdef FHM_SIGUSR1_DEBUG_FOR_IT1
    printf( "[%d]%s <signo>0x%x <output_dir>%d <sig_option>%d\n", __LINE__, __FUNCTION__,signo, output_dir, sig_option);
#endif
	
	/* ログ作成要求送信 */
	f_cmn_com_log_create_send( output_dir, CMD_TSKID_LGMK );
	
#if 0
//	if( sig_option != 0 )
//	{
//		fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l3", output_dir, output_file );	
//		if( fp != NULL )
//		{
//			(VOID)l3_main_logPrint(fp);
//			(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
//		}
//		else
//			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
//	}
	/* 運用ログ取得以外は応答を返さない	*/
//	if(( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI ) ||
//		( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G ))
//	{
#ifdef FHM_SIGUSR1_DEBUG_FOR_IT1
//  printf( "[%d]%s Send signal sigusr1 to INIT\n", __LINE__, __FUNCTION__);
#endif
//		/* response to INIT process	*/
//		BPF_RM_SVRM_SIGNAL_SEND(	D_RRH_PROCID_INI,				/* 送信先プロセスID			*/
//									SIGUSR1,						/* 送信するSignal			*/
//									sig_option );					/* オプション				*/
//	}
#endif
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "D_RRH_SIGUSR_OPT_RUNLOG_FIN\n" );
	return;
}

/* @} */
