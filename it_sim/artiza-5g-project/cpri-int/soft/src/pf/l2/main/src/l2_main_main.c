/*!
 *  @skip   $Id$
 *  @file   l2_main_main.c
 *  @brief  the entry of l2pi
 *  @date   2013/11/07 FFCS)hongj modify for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/

 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */
#include "l2_com_inc.h"
#include "l2_main_inc.h"
#include "f_com_aplCom.h"
#include "f_cmn_com_inc.h"
#include "f_cmn_cmu_inc.h"
#include "f_dbg_thread_inc.h"
#include "f_rrh_reg_eep.h"
#include "rrhApi_L2.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Inv.h"
#include "m_cm_l3log.h"

extern VOID l2_main_logPrint(FILE *outf);

/********************************************************************************************************************/
/*!
 *  @brief  the proccess of L2
 *  @note   This function is processed as follows. \n
 *  @return INT
 *  @date   2013/11/07 FFCS)hongj modify for zynq\n
 */
/********************************************************************************************************************/
INT main(INT argc, CHAR *argv[])
{
#ifdef OPT_RRH_SIM
	printf("********L2 server**********\n");
#endif

	/* process init						*/
	l2_main_Init();
	cm_Assert(CMD_ASL_NOLEVEL,0,"l2 init OK");

	/* process create						*/
	l2_main_Create();
	cm_Assert(CMD_ASL_NOLEVEL,1,"l2 threads OK");
	/* init process */
	l2_main_systemInit();
	cm_Assert(CMD_ASL_NOLEVEL,2,"l2 systemInit OK");
	/* ログ作成スレッド生成 */
	f_cmn_com_log_threadCreate( "l2", l2_main_logPrint, CMD_TSKID_LGMK_L2 );

#ifdef OPT_RRH_SIM
	printf("********l2 server finish**********\n");
#endif

//#ifndef OPT_RRH_SIM
	l2_main_signalProc();
//#else
//	getchar();
//#endif

	return D_RRH_OK;
}

sigset_t		sigSetVal;

/*!
 *  @brief  the process of init API
 *  @note   This function is processed as follows. \n
 *          - call API_l2_Init();
 *  @return VOID
 *  @retval none
 *  @date   2013/04/01 FFCS)honjg Create for zynq\n
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */

VOID l2_main_Init()
{
	int	rtn;
	INT					errcd;							/* error code				*/
	T_RRH_L2_LOG_INFO_ALL		*l2logAll_p;
	USHORT linkno;
	USHORT system_type;
	T_RRH_L2_STATISTICS_L3TABLE * l2_comw_stati_L3Tbl_workp;

//#ifndef OPT_RRH_SIM
	rtn = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( (VOID *)&l2_main_signal, &sigSetVal ,D_RRH_ON );
//#endif

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_L2);	/* proc trace start */
	/***********************************/
	/*	Process manage function assign */
	/***********************************/
	rtn = BPF_RM_SVRM_ASSIGN( D_RRH_PROCID_L2);
	/*NG case*/
	if (rtn != BPF_RM_SVRM_COMPLETE)
	{
		return;
	}

	BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_L2,		/* ProcessID		*/
								D_RRH_THDID_L2_MAIN,	/* ThreadID			*/
								CMD_TSKID_INI,			/* ThreadQueueID	*/
								(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	/* Get l2 l3 statistics table from flash */
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L2_STAT_INFO/**/,(VOID**)&l2_comw_stati_L2Tbl,&errcd);
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L3_STAT_INFO/**/,(VOID**)&l2_comw_stati_L3Tbl,&errcd);
// 擬似環境 20210316 共有メモリをmallocで取得
	//l2_comw_stati_L2Tbl = malloc(sizeof(T_RRH_L2_STATISTICS_L2TABLE)*4);
	//l2_comw_stati_L3Tbl = malloc(sizeof(T_RRH_L2_STATISTICS_L3TABLE)*4);
// 擬似環境 20210316 共有メモリをmallocで取得
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
	{
		l2_comw_stati_L2Tbl[linkno].mng.index = (l2_comw_stati_L2Tbl[linkno].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
		memset(&l2_comw_stati_L2Tbl[linkno].blks[l2_comw_stati_L2Tbl[linkno].mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L2BLK));
		for(system_type=0; system_type<CMD_MAX_SYSNUM; system_type++)
		{
			l2_comw_stati_L3Tbl_workp = l2_comw_stati_L3Tbl+(D_RRH_CPRINO_NUM*system_type+linkno);
			l2_comw_stati_L3Tbl_workp->mng.index = (l2_comw_stati_L3Tbl_workp->mng.index + 1) % D_RRH_L2_STATI_L3BLK_NUM;
			memset(&l2_comw_stati_L3Tbl_workp->blks[l2_comw_stati_L3Tbl_workp->mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L3BLK));
		}
	}

	l2_mainw_stateno = L2_MAIN_ST_USE;
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_LAPB_L3_SR_LOG/**/,(VOID**)&l2logAll_p,&errcd);

	l2_comw_logHead = &(l2logAll_p->head);
	l2_comw_logTbl = &(l2logAll_p->table);
	l2_comw_logSpec = &(l2logAll_p->spec);

	lblap_init_log();
	m_cm_L3LogTblInit();

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  Create threads.
 *  @note   Create threads.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
/*extern const T_SYS_THRDINFO l2_mainr_ThrMngTbl[D_L2_THRDNO_MAX];*/
T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;
VOID l2_main_Create()
{
	INT    ret;

	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)l2_main_abortLog;

	ret = apl_main_main(D_RRH_PROCID_L2, D_RRH_PROCQUE_L2,l2_mainr_ThrMngTbl, D_RRH_THDID_L2_MAX-1);
	if(ret != D_RRH_OK)
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_NG);
		return ;
	}
	else
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_OK);
	}

	return;
}

/* この定義は暫定です。必要に応じて各スレッド内のテーブルを追加してください */
static bpf_rm_svrm_sigget_tbl_t log_siggettble[] =
{
	{"l2_mainw_stateno"			 ,sizeof(l2_mainw_stateno)		,&l2_mainw_stateno,4},
	{"l2_mainw_eventno"			 ,sizeof(l2_mainw_eventno)		,&l2_mainw_eventno,4}
};

/********************************************************************************************************************/
/*!
 *  @brief  the process of related linux signal.
 *  @note   Create threads.
 *  @param  *signum     [in]  signal number
 *  @param  *info       [in]  signal pointer
 *  @param  *ctx        [in]  signal context
 *  @param  fd          [in]  file descriptor
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/12/05 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
INT l2_main_signal( int signum, void *info, void *ctx ,int fd)
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
	return (sizeof(log_siggettble)/sizeof(log_siggettble[0]));
}

void l2_main_signalProc()
{
//#ifndef OPT_RRH_SIM
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

		l2_main_logSave_and_Rsp( sigInfo.si_signo, sigInfo.si_status );
	}
//#endif
	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  the process of save log
 *  @note   the process of save log.
 *  @return void
 *  @retval -
 *  @date   2013/12/05 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
extern void tm_lb_logPrint(FILE*);
VOID l2_main_logPrint(FILE *outf)
{
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	tm_lb_logPrint(outf);
	m_cm_L3LogPrint(outf);
	BPF_RU_HRTM_TIMER_LOG_PRINT(outf);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
	return;
}

void l2_main_abortLog()
{
	FILE				*fp;
	UCHAR				output_file[256];
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l2log_abort", E_BPF_LOG_CMDLOG_OUTDIR_NORMAL, output_file )) != NULL )
	{
		l2_main_logPrint(fp);

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN l3log_abort" );

	return;
}

void l2_main_logSave(int kind)
{
//#ifndef OPT_RRH_SIM
	FILE					*fp;
	UCHAR				output_file[256];

	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l2", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file );
	if( fp != NULL )
	{
		l2_main_logPrint(fp);
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
//#endif
	exit(0);
}

void l2_main_logSave_and_Rsp(INT signo, INT option)
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
	f_cmn_com_log_create_send( output_dir, CMD_TSKID_LGMK_L2 );

#if 0
//	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "l2", output_dir, output_file );
//	if( fp != NULL )
//	{
//		l2_main_logPrint(fp);
//		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
//	}
//	else
//		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
//
//	/* �^�p���O�擾�ȊO�͉�����Ԃ��Ȃ�	*/
//	if(( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI ) ||
//		( sig_option == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G ))
//	{
		/* response to INIT process	*/
//		BPF_RM_SVRM_SIGNAL_SEND(	D_RRH_PROCID_INI,				/*���M��v���Z�XID*/
//									SIGUSR1,						/*���M����signal*/
//									sig_option );					/* signal option*/
//	}
#endif
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "D_RRH_SIGUSR_OPT_RUNLOG_FIN\n" );
	return;
}
/* @} */

