/*!
 *  @skip   $Id$
 *  @file   re_main_main.c
 *  @brief  RE process
 *  @date   2015/07/30 TDIPS) maruyama create(他のスレッドmainを参考に作成）
 *  @date   2015/08/04 TDIPS) maruyama modify 初期化処理
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 
 /*!
 * @addtogroup RRH_RE_MAIN
 * @{
 */
#include "m_cm_header.h"
#include "re_com_inc.h"
#include "re_main_inc.h"
#include "f_com_aplCom.h"

extern const T_SYS_THRDINFO re_mainr_ThrMngTbl[D_RE_THRDNO_MAX];

extern VOID re_main_logPrint(FILE *outf);

/********************************************************************************************************************/
/*!
 *  @brief  REプロセスメイン処理
 *  @note   main関数内では以下の処理を行う
 *  @return int
 *  @retval 
 *  @date   2015/07/30 TDIPS) maruyama create
 *  @date   2015/08/04 TDIPS) maruyama modify
 */
/********************************************************************************************************************/
#if OPT_RRH_CT
int re_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef OPT_RRH_SIM
	printf("******** RE proces **********\n");
#endif
	
	/* RE process init						*/
	re_main_Init();
	cm_Assert(CMD_ASL_NOLEVEL,0,"re init OK");

	/* RE process threads create			*/
	re_main_Create();
	cm_Assert(CMD_ASL_NOLEVEL,1,"RE threads OK");

	/* init process */
	re_main_systemInit();
	cm_Assert(CMD_ASL_NOLEVEL,2,"re systemInit OK");
	
	/* ログ作成スレッド生成 */
	f_cmn_com_log_threadCreate( "re", re_main_logPrint, CMD_TSKID_LGMK );
	
#ifdef OPT_RRH_SIM
	printf("********RE process finish**********\n");
#endif

//#ifndef OPT_RRH_SIM
	re_main_signalProc();
//#else
//	getchar();
//#endif
	return (0);
}

sigset_t			sigSetVal;
/********************************************************************************************************************/
/*!
 *  @brief  REプロセス初期化
 *  @note   
 *  @return VOID
 *  @retval none
 *  @date   2015/07/30 TDIPS) maruyama create
 *  @date   2015/08/04 TDIPS) maruyama modify
 */
/********************************************************************************************************************/
int re_main_Init()
{
	int	rtn;
//#ifndef OPT_RRH_SIM
	rtn = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( (VOID*)&re_main_signal, &sigSetVal ,D_RRH_ON);
//#endif

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_RE);	/* proc trace start */

	/***********************************/
	/*	Process manage function assign */
	/***********************************/
	rtn = BPF_RM_SVRM_ASSIGN( D_RRH_PROCID_RE);
	/*NG case*/
	if (rtn != BPF_RM_SVRM_COMPLETE) 
	{  
		return D_RRH_NG; 
	}

	BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_RE,			/* ProcessID		*/
									D_RRH_THDID_RE_MAIN,	/* ThreadID			*/
									CMD_TSKID_INI,		    /* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	return(D_RRH_OK);
}

/********************************************************************************************************************/
/*!
 *  @brief  スレッド作成
 *  @note   
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @retval -
 *  @date   2015/07/30 TDIPS) maruyama create
 *  @date   2015/08/04 TDIPS) maruyama modify
 *  @date   2015/09/14 TDI)satou レビュー指摘反映
 */
/********************************************************************************************************************/
typedef void (*VOIDFUNCPTR)(void);
T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;

void re_main_Create()
{
	int    ret;

	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)re_main_abortLog;

	ret = apl_main_main(D_RRH_PROCID_RE,D_RRH_PROCQUE_RE,re_mainr_ThrMngTbl,D_RE_THRDNO_MAX-1); //C9 add code
	printf("\n C9 debug apl_main_main file %s ,line %d,ret=%d \n",__FILE__,__LINE__,ret); //C9 add code debug
	if(ret != D_RRH_OK)
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_NG);
		return ;
	}
	else
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_OK);
	}

    re_mainw_stateno = RE_MAIN_ST_INIT;

	return;

}


/* この定義は暫定です。必要に応じて各スレッド内のテーブルを追加してください */
static bpf_rm_svrm_sigget_tbl_t log_siggettble[] = 
{
	{"re_mainw_stateno"			 ,sizeof(re_mainw_stateno)		,&re_mainw_stateno,4},
	{"re_mainw_eventno"			 ,sizeof(re_mainw_eventno)		,&re_mainw_eventno,4}
};
/********************************************************************************************************************/
/*!
 *  @brief  the process of related linux signal.
 *  @note   Create threads.
 *  @param  *signum [in]  signal number
 *  @param  *info [in]  signal pointer
 *  @param  *ctx  [in]  signal context
 *  @param  fd    [in]  file descriptor
 *  @return Result code is returned
 *  @retval -
 *  @date   2015/07/30 TDIPS) maruyama create(他スレッドを参考に作成）
 */
/********************************************************************************************************************/
INT re_main_signal( int signum, void *info, void *ctx , int fd )
{
//#ifndef OPT_RRH_SIM
	if(fd > 0)
	{
		/* ローカルテーブル保存処理 */
		BPF_RM_SVRM_SIGNAL_TBLSAVE(fd,
							   sizeof(log_siggettble)/sizeof(log_siggettble[0]),
							   (char*)log_siggettble
							   );
	}
//#endif	
	return (sizeof(log_siggettble)/sizeof(log_siggettble[0])) ;
}


void re_main_signalProc()
{
	siginfo_t			sigInfo;
	int					ret;

	for(;;)
	{
		printf("\nre file %s line  %d before wait\n",__FILE__,__LINE__);//C9 add code
		ret = BPF_RM_SVRM_SIGNAL_WAIT(&sigSetVal,
									&sigInfo.si_pid,
									&sigInfo.si_signo,
									&sigInfo.si_status);
		printf("\nre file %s line  %d wait\n",__FILE__,__LINE__);//C9 add code
		if( ret != BPF_RM_SVRM_OK )
		{
			cm_Assert(CMD_ASL_DBGLOW,ret, "signal wait error. ");
			continue;
		}
		
		re_main_logSave_and_Rsp( sigInfo.si_signo, sigInfo.si_status );
	}

	return;
}


/********************************************************************************************************************/
/*!
 *  @brief  the process of save log
 * @TBD_No  ログ出力の関数が未作成(オフショアが作成？）
 *  @note   the process of save log.
 *  @param  *outf [in]  file pointer
 *  @return void
 *  @retval -
 *  @date   2015/07/30 TDIPS) maruyama create(他スレッドを参考に作成）
 *  @date   2015/09/03 FPT)Lay modify for dump Run History to file
 */
/********************************************************************************************************************/
VOID re_main_logPrint(FILE *outf)
{

    INT prm = D_TCM_LOGOUT_KIND_ALL; 
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    f_trp_rec_Fcom_t_sta_dsp( &prm, outf );
	BPF_RU_HRTM_TIMER_LOG_PRINT(outf);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return;
}


/********************************************************************************************************************/
/*!
 *  @brief  abort log
 *  @note   -
 *  @param  -
 *  @return void
 *  @retval -
 *  @date   2015/07/30 TDIPS) maruyama create(他スレッドを参考に作成）
 *  @date   2015/08/19 TDIPS) maruyama modify  ログ出力内容を訂正
 */
/********************************************************************************************************************/
VOID re_main_abortLog()
{
	FILE				*fp;
	UCHAR				output_file[256];
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "relog_abort", E_BPF_LOG_CMDLOG_OUTDIR_NORMAL, output_file )) != NULL )
	{
		re_main_logPrint(fp);
	
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN relog_abort" );

	return;
}


/********************************************************************************************************************/
/*!
 *  @brief  log save and response
 *  @note   -
 *  @param  signo	[in]	signal number
 *  @param  option	[in]	status
 *  @return void
 *  @retval -
 *  @date   2015/07/30 TDIPS) maruyama create(他スレッドを参考に作成）
 *  @date   2015/08/19 TDIPS) maruyama modify signal numberのcaseにD_RRH_SIGUSR_OPT_RUNLOG_GET_3Gを追加(output_dir,sig_optionの設定）
 */
/********************************************************************************************************************/
VOID re_main_logSave_and_Rsp(INT signo, INT option)
{
//#ifndef OPT_RRH_SIM
//	FILE					*fp;
//	UCHAR				output_file[256];
	UINT				output_dir;
	INT					sig_option;
	

	switch( signo )
	{
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
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "signal kind error si_status=%x", option );
			output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
			sig_option = 0;
		}
		break;
	
	default:
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
		sig_option = 0;
		break;
	}
	/* ログ作成要求送信 */
	f_cmn_com_log_create_send( output_dir, CMD_TSKID_LGMK );
	
#if 0
//	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "re", output_dir, output_file );	
//	if( fp != NULL )
//	{
//		re_main_logPrint(fp);
//		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
//	}
//	else
//		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
	/* 運用ログ出力完了(INITへ) */
//	if(( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI ) ||
//		( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G ))
//	{
//		/* response to INIT process	*/
//		BPF_RM_SVRM_SIGNAL_SEND(	D_RRH_PROCID_INI,
//									SIGUSR1,
//									sig_option );
//	}
#endif
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "D_RRH_SIGUSR_OPT_RUNLOG_FIN\n" );
	return;
}

/* @} */
