/*!
 *  @skip   $Id$
 *  @file   mnt_main_main.c
 *  @brief  the entry of mnt server
 *  @date   2013/11/07 FFCS)zhaodx create for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_MNT_MAIN
 * @{
 */

#include "m_cm_header.h"
#include "mnt_com_inc.h"
#include "mnt_main_inc.h"
#include "f_com_aplCom.h"

#include "rrhApi_L2.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Inv.h"

#include "f_cmn_cmu_inc.h"
#include "f_dbg_thread_inc.h"

extern VOID mnt_main_logPrint(FILE *outf);

extern const T_SYS_THRDINFO mnt_comr_ThrMngTbl[D_RRH_THDID_MT_MAX];

/********************************************************************************************************************/
/*!
 *  @brief  the proccess of hunter server
 *  @note   This function is processed as follows. \n
 *          - directly call apl_main_main
 *          - send response to init process
 *          - receive system init ntc from  init process
 *          - subscribe cpri message to l2 proc
 *          - subscribe cpri state change to pf proc
 *  @return VOID
 *  @retval none
 *  @date   2013/11/07 FFCS)zhaodx create for zynq\n
 */
/********************************************************************************************************************/
#ifdef OPT_RRH_CT
INT mnt_main(INT argc, CHAR *argv[])
#else
INT main(INT argc, CHAR *argv[])
#endif
{

#ifdef FOR_DBG
	printf("********MNT server**********");
#endif

	/* process init						*/
	mnt_main_Init();
	cm_Assert(CMD_ASL_NOLEVEL,0,"mnt init OK");
		
	/* process create						*/
	mnt_main_Create();
	cm_Assert(CMD_ASL_NOLEVEL,1,"mnt threads OK");
	
	/* Tcp server Init request to mkm thread */
	mnt_main_TcpServerInit();
	cm_Assert(CMD_ASL_NOLEVEL,2,"mnt TCP server OK");

	/* init process */
	mnt_main_systemInit();
	cm_Assert(CMD_ASL_NOLEVEL,3,"mnt  system Init OK");

	/* ログ作成スレッド生成 */
	f_cmn_com_log_threadCreate( "mnt", mnt_main_logPrint, CMD_TSKID_LGMK );
	
#ifdef OPT_RRH_SIM
	printf("********mnt server finish**********\n");
#endif
//#ifndef OPT_RRH_SIM
	mnt_main_signalProc();
//#else
	getchar();
//#endif

	return D_RRH_OK;
}

sigset_t			sigSetVal;
/*!
 *  @brief  the process of init API
 *  @note   This function is processed as follows. \n
 *          - call API_MNT_Init();
 *  @return VOID
 *  @retval none
 *  @date   2013/11/07 FFCS)zhaodx create for zynq\n
 */
INT mnt_main_Init()
{
	int	rtn;
//#ifndef OPT_RRH_SIM
	rtn = BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( (VOID*)&mnt_main_signal, &sigSetVal ,D_RRH_ON);
//#endif

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_MT);	/* proc trace start */

	/***********************************/
	/*	Process manage function assign */
	/***********************************/
	rtn = BPF_RM_SVRM_ASSIGN( D_RRH_PROCID_MT);
	/*NG case*/
	if (rtn != BPF_RM_SVRM_COMPLETE) 
	{  
		return D_RRH_NG; 
	}

	BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_MT,			/* ProcessID		*/
									D_RRH_THDID_MT_MAIN,	/* ThreadID			*/
								    CMD_TSKID_INI,			/* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
								
	/* 共有メモリからデバイス情報を取得する */
	rrhApi_Inv_Fsm_GetDevInfo(&cmw_re_dev_tbl,sizeof(cmw_re_dev_tbl));

	/* 共有メモリからTRAインベントリ情報を取得する */
	rrhApi_Inv_Fsm_GetTraInvInfo(&cmw_tra_inv_tbl,sizeof(cmw_tra_inv_tbl));
	/* read cmw_device_kind*/
	rrhApi_Inv_Fsm_GetDevKind(&cmw_device_kind,sizeof(cmw_device_kind));
	switch(cmw_device_kind)
	{
		//TDD SRE
		case E_RRH_RRHKIND_SRE_35G:
			cmw_validsysinfo = CMD_SYS_S3G;
			cmw_reKind = E_RRH_REKIND_TDD_SRE;
			break;
		default:
			cm_Assert(CMD_ASL_DBGLOW, cmw_device_kind,  "device kind error.");
			cmw_device_kind = CMD_NUM0;
			cmw_validsysinfo = CMD_NUM0;
			cmw_reKind = CMD_NUM0;
			break;
	}

	rrhApi_Inv_Fsm_GetSystemPara(&cmw_sys_mng_tbl,sizeof(cmw_sys_mng_tbl));
	return 0;
}

/********************************************************************************************************************/
/*!
 *  @brief  Create threads.
 *  @note   Create threads.
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @retval -
 *  @date   2013/11/19 FFCS)zhaodx Create 
 */
/********************************************************************************************************************/
T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;

VOID mnt_main_Create()
{
	INT    ret;

	memset(&f_cmnw_almlogfunc,0,sizeof(T_CMN_ALMLOG_FUNCTION_LIST));
	f_cmnw_almlogfunc.func_num = 1;
	f_cmnw_almlogfunc.func_list[0].func_addr = (VOIDFUNCPTR)mnt_main_abortLog;

	ret = apl_main_main(D_RRH_PROCID_MT,D_RRH_PROCQUE_MT,mnt_comr_ThrMngTbl,D_RRH_THDID_MT_MAX -2);//C9 擬似環境
	if(ret != D_RRH_OK)
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_NG);
		return ;
	}
	else
	{
		BPF_RM_SVRM_INITEND_IND(BPF_RM_SVRM_OK);
	}

    mnt_mainw_stateno = MNT_MAIN_ST_INIT;

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  Tcp Server Init
 *  @note   Tcp Server Init.
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @retval -
 *  @date   2013/11/19 FFCS)zhqodx Create 
 *  @date   2015/08/18 TDI)satou 「Send tcp server Init request to mtm thread」削除
 */
/********************************************************************************************************************/
VOID mnt_main_TcpServerInit()
{
	CMT_TSKIF_TCPSRVINITREQ initReq;
	INT    					ret;
	
    /* Send tcp server Init request to mkm thread */
    memset(&initReq, 0, sizeof(initReq));
	initReq.head.uiEventNo		= CMD_TSKIF_TCPSRVINITREQ;			/**< Event  ID         */
	initReq.head.uiSignalkind	= 0;								/**< Signal Kind       */
	initReq.head.uiDstPQueueID	= D_RRH_PROCQUE_MT;					/**< Destination P QID */
	initReq.head.uiDstTQueueID	= CMD_TSKID_MKM;					/**< Destination T QID */
	initReq.head.uiSrcPQueueID	= D_RRH_PROCQUE_MT;					/**< Source P QID      */
	initReq.head.uiSrcTQueueID	= CMD_TSKID_INI;					/**< Source T QID      */
	initReq.head.uiLength		= sizeof(initReq);					/**< Length            */

	ret = apl_com_msgQSend(
			CMD_TSKID_MKM,					/* Thread queue ID         */
			CMD_TSKIF_TCPSRVINITREQ,
			(VOID *)&initReq,				/* The message to transmit */
			sizeof(initReq)	 				/* The size of message     */
		);
	/* checking parameter */
	if( ret != D_RRH_OK ) {
		cm_Assert( CMD_ASL_DBGLOW, ret,
		    (CHAR*)"[mnt_main_TcpServerInit] Send tcp server Init request to mtm thread NG" );
		return;
	}

#if 0
	/* Get response from mtm thread */
	ret = apl_com_msgQReceive(
			CMD_TSKID_INI,						/* Thread queue ID         */
			sizeof(rcvMsg),		 				/* received buffer size    */
			(VOID *)rcvMsg, 					/* received buffer         */
			&rcvmsgsz, 							/* received message length */
			&errcd								/* Error code              */
		);
	if(ret != D_RRH_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, ret,
		    (UCHAR*)"[mnt_main_TcpServerInit] Get Tcp server init response from mkm thread NG" );
		return;
	}
#endif

	return;
}


static bpf_rm_svrm_sigget_tbl_t log_siggettble[] = 
{
	{"mnt_mainw_stateno"			 ,sizeof(mnt_mainw_stateno)		,&mnt_mainw_stateno,4},
	{"mnt_mainw_eventno"			 ,sizeof(mnt_mainw_eventno)		,&mnt_mainw_eventno,4}
};

/********************************************************************************************************************/
/*!
 *  @brief  the process of related linux signal.
 *  @note   Create threads.
 *  @param  *signum [in]  signal number
 *  @param  *info [in]  signal pointer
 *  @param  *ctx  [in]  signal context
 *  @param  fd [in] file descriptor
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/12/05 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
INT mnt_main_signal( int signum, void *info, void *ctx , int fd )
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

void mnt_main_signalProc()
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
		
		mnt_main_logSave_and_Rsp( sigInfo.si_signo, sigInfo.si_status );
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
 *  @date   2015/08/03 TDI)satou m_mt_logPrint()削除
 */
/********************************************************************************************************************/
extern void m_mk_logPrint(FILE*);
VOID mnt_main_logPrint(FILE *outf)
{
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	m_mk_logPrint(outf);
	BPF_RU_HRTM_TIMER_LOG_PRINT(outf);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

	return;
}

VOID mnt_main_abortLog()
{
	FILE				*fp;
	UCHAR				output_file[256];
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "mntlog_abort", E_BPF_LOG_CMDLOG_OUTDIR_NORMAL, output_file )) != NULL )
	{
		mnt_main_logPrint(fp);
	
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN mntlog_abort" );

	return;
}

VOID mnt_main_logSave(INT kind)
{
//#ifndef OPT_RRH_SIM
	FILE					*fp;
	UCHAR				output_file[256];

	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "mnt", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file );	
	if( fp != NULL )
	{
		mnt_main_logPrint(fp);
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2\n" );
//#endif
	exit(0);
}

/********************************************************************************************************************/
/*!
 *  @brief  log save and response
 *  @note   -
 *  @param  signo	[in]	signal number
 *  @param  option	[in]	status
 *  @return void
 *  @retval -
 *  @date   2015/08/19 TDIPS) maruyama modify signal numberのcaseにD_RRH_SIGUSR_OPT_RUNLOG_GET_3Gを追加(output_dir,sig_optionの設定）
 */
/********************************************************************************************************************/
VOID mnt_main_logSave_and_Rsp(INT signo, INT option)
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
//	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "mnt", output_dir, output_file );	
//	if( fp != NULL )
//	{
//		mnt_main_logPrint(fp);
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
