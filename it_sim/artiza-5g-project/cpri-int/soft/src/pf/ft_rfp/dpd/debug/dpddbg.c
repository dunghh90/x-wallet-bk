#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "f_dpda_inc.h"
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RM_SVRM.h"

extern INT dbgcmd_exec_pf_dpda( CHAR *cmdStr, FILE *outFp );
extern INT dbgcmd_exec_pf_dpda_tdd_35g( CHAR *cmdStr, FILE *outFp );


/* アラーム系 */
extern VOID f_dpddbg_printAlmLog(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_printAlarmStateAll(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_printAlmLogALL(int argc, char *argv[], FILE *outFp);

extern VOID f_dpddbg_printAlmLogAntCarState(UINT uiFace, UINT uiIndex, UINT stAnt, UINT dspAntNum, UINT isCarOn, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);
extern VOID f_dpddbg_printAlmLogAlmThreshold(UINT	uiFace, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);
extern VOID f_dpddbg_printAlmLogAlarmState(UINT uiFace, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);
extern VOID f_dpddbg_printAlmLogRegData(UINT uiFace, UINT uiIndex, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);
extern VOID f_dpddbg_printAlmLogStatusInfoAck(UINT uiFace, UINT uiIndex, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);
extern VOID f_dpddbg_printAlmLogTaskInf(	UINT	uiFace, UINT	uiIndex, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp);

/* DPDA情報系 */
extern VOID f_dpddbg_changeTrainingData(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_changeRegisterFlg(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_changeVirtualRegister(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_almThreshSet(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_almThreshSetUsage(FILE *outFp);
extern VOID f_dpddbg_printTemprature(int argc, char *argv[], FILE *outFp);
extern VOID f_dpddbg_TempTimeSet(int argc, char *argv[], FILE *outFp);

/* 定常処理系 */
extern VOID f_dpddbg_1secReport(int argc, char *argv[], FILE *outFp);

/* HELP系 */
extern VOID f_dpddbg_help(FILE *outFp);

/* デバッグコマンドパラメータ数(コマンド自体も含む) */
#define		D_DPDDBG_CMD_DPD_IN_PARAM_NUM			8

/* 表示Bit	*/
#define	D_DPDDBG_ALM_LOG_PRINT_ALM_THRESH			0x00000002	/*!< Alarm Thresholdテーブル表示		*/
#define	D_DPDDBG_ALM_LOG_PRINT_ALM_STATE			0x00000004	/*!< Alarm Stateテーブル表示			*/
#define	D_DPDDBG_ALM_LOG_PRINT_STATUS_ACK			0x00000020	/*!< Status Information ACK表示			*/
#define	D_DPDDBG_ALM_LOG_PRINT_HARDREG				0x00000100	/*!< Hard Register表示					*/
#define	D_DPDDBG_ALM_LOG_PRINT_TSKINFO				0x00000400	/*!< DPDA Task Info情報表示				*/

enum
{
	E_DPDDBG_CMD_DSP=0,
	E_DPDDBG_CMD_SET,
	E_DPDDBG_CMD_CTRL_NUM
};

enum
{
	E_DPDDBG_CMD_ALMLOG_DISP=0,
	E_DPDDBG_CMD_ALMALL_DISP,
	E_DPDDBG_CMD_ALMLOGALL,
	E_DPDDBG_CMD_TRAALL,
	E_DPDDBG_CMD_CARINFO,
	E_DPDDBG_CMD_TEMP,
	E_DPDDBG_CMD_DSP_NUM
};

enum
{
	E_DPDDBG_CMD_SET_REG=0,
	E_DPDDBG_CMD_SET_VREG,
	E_DPDDBG_CMD_SET_ALMTH,
	E_DPDDBG_CMD_SET_ASSING,
	E_DPDDBG_CMD_TEMP_TIM,
	E_DPDDBG_CMD_SET_NUM
};

typedef struct{
	CHAR	cmdName[16];
	VOID	(*func)(INT argc, CHAR *argv[], FILE *outFp );
}T_DBG_CMD_LIST;

typedef struct{
	CHAR					ctrlName[16];
	UINT					cmdNum;
	T_DBG_CMD_LIST			cmdList[E_DPDDBG_CMD_SET_NUM];
}T_DBG_DSP_CMD_ALL_LIST;

UINT f_dpddbgw_rrhKind;

/**
 * @date 2015/08/07 TDI)satou "dsp"の「キャリア情報」を削除
 * @date 2015/08/07 TDI)satou "dsp"の「トレーニングデータ全表示」を削除
 * @date 2015/08/07 TDI)satou "set"の「トレーニングデータ変更」を削除
 * @date 2015/08/07 TDI)satou "set"の「割り込み通知(高速モード要求)」を削除
 * @date 2015/08/07 TDI)satou "set"の「DL100%キャリア設定」を削除
 */
const T_DBG_DSP_CMD_ALL_LIST f_dpddbgr_cmd_list[E_DPDDBG_CMD_CTRL_NUM] =
{
	{
		"dsp",
		E_DPDDBG_CMD_DSP_NUM,
		{
			{
				/*	f_dpddbg_printAlmLog		*/
				"almlog",
				f_dpddbg_printAlmLog,
			},
			{
				/*	f_dpddbg_printAlarmStateAll	*/
				"almall",
				f_dpddbg_printAlarmStateAll,
			},
			{
				/*	f_dpddbg_printAlmLogALL	*/
				"almlogall",
				f_dpddbg_printAlmLogALL,
			},
			{
				/*	温度情報情報	*/
				"temper",
				f_dpddbg_printTemprature,
			}
		}
	},
	{
		"set",
		E_DPDDBG_CMD_SET_NUM,
		{
			{
				/*	レジスタ参照変更		*/
				"reg",
				f_dpddbg_changeRegisterFlg,
			},
			{
				/*	仮想レジスタ値変更処理	*/
				"vreg",
				f_dpddbg_changeVirtualRegister,
			},
			{
				/*	 ALM閾値設定処理		*/
				"almth",
				f_dpddbg_almThreshSet,
			},
			{
				/*	1秒周期情報処理			*/
				"1sec",
				f_dpddbg_1secReport,
			},
			{
				/*	レジログ設定処理	*/
				"temper",
				f_dpddbg_TempTimeSet
			}
		}
	}
};

/*!
 * @brief 関数機能概要:デバッグコマンド 配送
 * @note  関数処理内容.
 *       -# アーキテクチャの情報を読み出す
 *       -# 対応アーキテクチャのデバッグコマンド処理を実行する
 * @param  cmdStr				[in]CHAR*	コマンド文字列(スペース区切り)
 * @param  outFp				[in]FILE*	結果格納用ファイル
 * @retval INT					D_SYS_OK
 * @retval INT					D_SYS_NG
 * @date 2013/06/11 ALPHA)中村 B25対応 Create
 */
INT dbgcmd_exec_pf_dpda( CHAR *cmdStr, FILE *outFp )
{
	INT  ret = D_SYS_OK;
	INT  errcd;
	UINT reKind;
	T_RRH_TRAINVTBL	*invTbl;

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_INVENTORY,  /* Buffer種別 */
		(void**)&invTbl,											/* msgP       */
		&errcd );
	
	reKind = invTbl->cmx_device_typ;
	
	if( reKind == E_RRH_REKIND_TDD_SRE )
	{
		ret = dbgcmd_exec_pf_dpda_tdd_35g( cmdStr, outFp );
	}
	else
	{
		fprintf( outFp, "dpda not supported this archtecture.\n");
		ret = D_SYS_NG;
	}

	return ret;
}

/*!
 * @brief 関数機能概要:デバッグコマンド処理
 * @note  関数処理内容.
 *       -# デバッグコマンド処理
 * @param  cmdStr				[in]CHAR*	コマンド文字列(スペース区切り)
 * @param  outFp				[in]FILE*	結果格納用ファイル
 * @retval INT					D_RRH_OK
 * @retval INT					D_RRH_NG
 * @return VOID
 * @date 2014/12/20 ALPHA)中村 Create
 */
INT dbgcmd_exec_pf_dpda_tdd_35g( CHAR *cmdStr, FILE *outFp )
{
	UINT						cmdCnt;
	UINT						cmdCtlCnt;
	T_DBG_CMD_LIST				*cmdList;
	UINT						argcCnt;
	CHAR						*argvParam[D_DPDDBG_CMD_DPD_IN_PARAM_NUM];
	CHAR						*getToken, *savePtr;
	CHAR						*delimiter = " ";
	UINT						*shm_rrhKind;
	INT							ret;
	INT							errcd;

	getToken = cmdStr;
	for( argcCnt=0; argcCnt < D_DPDDBG_CMD_DPD_IN_PARAM_NUM; argcCnt++ )
	{
		argvParam[ argcCnt ] = strtok_r( getToken, delimiter, &savePtr );
		if( NULL == argvParam[ argcCnt ] )
		{
			break;
		}
		
		/* 2パラメータ以降は NULL指定で取り出す */
		getToken = NULL;
	}
	
	if( argcCnt <= 3 )
	{
		fprintf( outFp, "need input argv.\n");
		f_dpddbg_help( outFp );
		return D_RRH_OK;
	}
	
	/* 装置種別取得  */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
										(void**)&shm_rrhKind,
										&errcd );
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG (%d,%d)", ret, errcd );
	}
	
	f_dpddbgw_rrhKind = *shm_rrhKind;
	
	for( cmdCtlCnt=0; cmdCtlCnt<E_DPDDBG_CMD_CTRL_NUM; cmdCtlCnt++ )
	{
		if(0 == strcmp(argvParam[2], f_dpddbgr_cmd_list[cmdCtlCnt].ctrlName))
		{
			cmdList = (T_DBG_CMD_LIST *)&(f_dpddbgr_cmd_list[cmdCtlCnt].cmdList[0]);
			
			for( cmdCnt=0; cmdCnt < f_dpddbgr_cmd_list[cmdCtlCnt].cmdNum; cmdCnt++ )
			{
				if(0 == strcmp(argvParam[3], cmdList->cmdName))
				{
					(VOID)(*(cmdList->func))((argcCnt-3), &argvParam[3], outFp );
					return D_RRH_OK;
				}
				cmdList++;
			}
			
			if( cmdCnt == f_dpddbgr_cmd_list[cmdCtlCnt].cmdNum )
			{
				f_dpddbg_help( outFp );
				return D_RRH_OK;
			}
		}
	}
	
	if( E_DPDDBG_CMD_CTRL_NUM == cmdCtlCnt )
	{
		f_dpddbg_help( outFp );
	}
	
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ALM Log表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Logテーブルをコンソールに表示する
 * @param  argc  [in] argc
 * @param  argv  [in] (1)uiDisp 表示選択(bitで選択) (2)uiDispFace 表示データ面(1~10で指定面、それ以外：最新面) (3) uiDispNum 表示データ数(0:最新のみ, 1~70表示数指定)
 * @param  outFp [in] output file pointer
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 * @date 2013/12/13 ALPHA)加藤 ZYNQ対応
 * @date 2015/08/07 TDI)satou 「Soft State情報(ANT/CarrierInfo)表示」削除
 */
VOID f_dpddbg_printAlmLog(int argc, char *argv[], FILE *outFp )
{
	UINT					uiDisp = 0;			/* 第一引数 */
	UINT					uiDispFace = 0;		/* 第二引数 */
	UINT					uiDispNum = 0;		/* 第三引数 */
	UINT					uiFace;
	UINT					uiIndex;
	UINT					uiIndexNew;
	UINT					uiDispCnt;
	T_DPDA_ALM_LOG_DEBUG*	almLog;
	UINT					uiNumOfDispLog;
	long int				lastNum;
	char* 					endptr = "\0";
	UINT					uiRet;
	INT						errcd;

	/************************************************************/
	/* Get dpda alarm log(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DPDA_ALM_LOG,(VOID**)&almLog, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	/* 引数を取得 */
	if( argc == 1 )
	{
		uiDisp = 0;
	}
	else
	{
		errno=0;
		lastNum = strtol(argv[1], &endptr, 16); /* pgr0539 */
		if(errno !=0)
		{
			fprintf( outFp, "In Parameter error1\n");
			return;
		}
		else
		{
			uiDisp = (UINT)lastNum; /* pgr0539 */
		}
		
		if( argc >= 3 )
		{
			errno=0;
			lastNum = strtol(argv[2], &endptr, 10);
			if(errno !=0)
			{
				fprintf(  outFp, "In Parameter error2\n");
				return;
			}
			else
			{
				uiDispFace = (UINT)lastNum; /* pgr0539 */
			}
			
			if( argc >= 4 )
			{
				errno=0;
				lastNum = strtol(argv[3], &endptr, 10);
				if(errno !=0)
				{
					fprintf( outFp, "In Parameter error3\n");
					return;
				}
				else
				{
					uiDispNum = (UINT)lastNum; /* pgr0539 */
				}
			}
		}
	}
	
	if( (0 == uiDisp)
		|| (uiDispNum > D_DPDA_ALM_LOG_MAX) )
	{
		fprintf(  outFp, "[Usage]\n");
		fprintf(  outFp, "dsp almlog  kind face num\n");
		fprintf(  outFp, "  kind:\n");
		fprintf(  outFp, "    Select log kind(bit select)\n");
		fprintf(  outFp, "      bit31(LSB) : \n");
		fprintf(  outFp, "      bit30      : Soft table Alarm Threshold Info\n");
		fprintf(  outFp, "      bit29      : Soft table Alarm State Info\n");
		fprintf(  outFp, "      bit28      : \n");
		fprintf(  outFp, "      bit27      : \n");
		fprintf(  outFp, "      bit26      : Soft table Status Information\n");
		fprintf(  outFp, "      bit25      : \n");
		fprintf(  outFp, "      bit24      : \n");
		fprintf(  outFp, "      bit23      : Hard Register\n");
		fprintf(  outFp, "      bit22      : \n");
		fprintf(  outFp, "      bit21      : Soft table Task info\n");
		fprintf(  outFp, "  face:\n");
		fprintf(  outFp, "      1~8       : print specify face\n");
		fprintf(  outFp, "      other      : print the latest face log\n");
		fprintf(  outFp, "  num:\n");
		fprintf(  outFp, "      0          : print the latest log\n");
		fprintf(  outFp, "      1~30       : print num of specified log\n");
		
		return;
	}
	
	/*	表示する面情報を取得	*/
	if( ( 0 == uiDispFace ) ||
		( D_DPDA_ALM_LOG_FACE < uiDispFace ))
	{
		/* 最新面情報を表示	*/
		uiFace = almLog->almLogFace;
	}
	else
	{
		/* 指定面を表示	*/
		uiFace = uiDispFace - 1;
	}
	
	/*	70面のうちの最新位置を算出する	*/
	if( almLog->almLog[uiFace].DataPosi == 0 )
	{
		uiIndexNew = D_DPDA_ALM_LOG_MAX-1;
	}
	else
	{
		uiIndexNew = almLog->almLog[uiFace].DataPosi-1;
	}

	/*	表示データ数の決定										*/
	/*	uiDispNum=0は最新データのみ, それ以外は全ログ表示	*/
	if( uiDispNum == 0 )
	{
		uiNumOfDispLog = 1;
		uiIndex        = uiIndexNew;
	}
	else
	{
		/****************************************************************************************/
		/* INパラで指定されたログ数分古い順に表示												*/
		/* ・1秒ログがMAX分格納されている場合は、指定されたログ表示数からIndexを算出			*/
		/****************************************************************************************/
		if( almLog->almLog[uiFace].almLogNum == D_DPDA_ALM_LOG_MAX )
		{
			uiNumOfDispLog = uiDispNum;
			uiIndex        = (D_DPDA_ALM_LOG_MAX + uiIndexNew - uiDispNum +1) % D_DPDA_ALM_LOG_MAX;
		}
		/****************************************************************************************/
		/* ・1秒ログがMAX分格納されていない場合													*/
		/*  - INパラ指定のログ数のほうが、格納ログ数より大きい場合は、格納している全ログ表示	*/
		/*  - 上記以外の場合は、指定ログ数分表示												*/
		/****************************************************************************************/
		else
		{
			if( (uiIndexNew+1) < uiDispNum )
			{
				uiNumOfDispLog = (uiIndexNew+1);
				uiIndex   = 0;
			}
			else
			{
				uiNumOfDispLog = uiDispNum;
				uiIndex        = (uiIndexNew+1) - uiDispNum;
			}
		}
	}
	
	fprintf(  outFp, "\nLatest  Face = %d\n", almLog->almLogFace);
	fprintf(  outFp, "Display Face = %d\n", uiFace);
	fprintf(  outFp, "Next Index   = %d\n", almLog->almLog[uiFace].DataPosi);
	fprintf(  outFp, "Alarm Log Num= %d\n", almLog->almLog[uiFace].almLogNum);
	fprintf(  outFp, "RRH Kind     = %d\n", almLog->rrhKind );

	/****************************************************************/
	/* ALM閾値情報表示												*/
	/****************************************************************/
	if( D_DPDDBG_ALM_LOG_PRINT_ALM_THRESH == (uiDisp & D_DPDDBG_ALM_LOG_PRINT_ALM_THRESH) )
	{
		f_dpddbg_printAlmLogAlmThreshold( uiFace , almLog, outFp);
	}
	
	/*	Alarm State情報は統計情報であるため最新のみ表示				*/
	/****************************************************************/
	/* ALM State情報表示											*/
	/****************************************************************/
	if( D_DPDDBG_ALM_LOG_PRINT_ALM_STATE == (uiDisp & D_DPDDBG_ALM_LOG_PRINT_ALM_STATE) )
	{
		f_dpddbg_printAlmLogAlarmState( uiFace , almLog, outFp );
	}
	
	/* 1つずつ表示していく	*/
	for( uiDispCnt = 0; uiDispCnt < uiNumOfDispLog; uiDispCnt++ )
	{
		fprintf(  outFp, "\n###################################################\n");
		fprintf(  outFp, "- Index = %d\n", uiIndex);

		/****************************************************************/
		/* 日付表示														*/
		/****************************************************************/
		fprintf(  outFp, "time:%04d/%02d/%02d %02d:%02d:%02d.%03d\n",
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.year,
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.month,
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.day,
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.hour,
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.min,
			almLog->almLog[uiFace].alm_log_info[uiIndex].time.sec,
			(almLog->almLog[uiFace].alm_log_info[uiIndex].time.msec)*10 );
		
		/* 以降はBitが立っているもののみ表示を行う						*/
		/****************************************************************/
		/* Status Information ACK情報表示								*/
		/****************************************************************/
		if( D_DPDDBG_ALM_LOG_PRINT_STATUS_ACK == (uiDisp & D_DPDDBG_ALM_LOG_PRINT_STATUS_ACK) )
		{
			f_dpddbg_printAlmLogStatusInfoAck( uiFace, uiIndex, almLog, outFp );
		}
		
		/****************************************************************/
		/* Hard Register情報表示										*/
		/****************************************************************/
		if( D_DPDDBG_ALM_LOG_PRINT_HARDREG == (uiDisp & D_DPDDBG_ALM_LOG_PRINT_HARDREG) )
		{
			f_dpddbg_printAlmLogRegData( uiFace, uiIndex, almLog, outFp );
		}
		
		/****************************************************************/
		/* DPDA Task 情報表示											*/
		/****************************************************************/
		if( D_DPDDBG_ALM_LOG_PRINT_TSKINFO == (uiDisp & D_DPDDBG_ALM_LOG_PRINT_TSKINFO) )
		{
			f_dpddbg_printAlmLogTaskInf( uiFace, uiIndex, almLog, outFp );
		}
		
		uiIndex = (uiIndex + 1) % D_DPDA_ALM_LOG_MAX;
	}
	
	return;
}

/*!
 * @brief 関数機能概要:デバッグコマンド 全てのアラームごとの状態表示を行う(現在の値)
 * @note  関数処理内容.
 *       -# 現時点のアラーム情報を表示する
 * @param  argc			[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv			[in]char*[]	コマンドラインから入力されたパラメータ
 * @param  outFp		[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_printAlarmStateAll(int argc, char *argv[], FILE *outFp )
{
	T_DPDA_ALM_LOG_DEBUG*	almLog;
	UINT					uiRet;
	INT						errcd;
	UINT					uiFace;
	
	/************************************************************/
	/* Get dpda alarm log(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_ALM_LOG, (VOID **)&almLog, &errcd);
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	/*	DPDA管理の全アラーム状態を表示する	*/
	uiFace = almLog->almLogFace;
	f_dpddbg_printAlmLogAlarmState( uiFace, almLog, outFp );
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド アラーム解析用ログを指定数分表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Log Collectテーブルを指定数分表示する
 * @param  argc				[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv				[in]char*[]	コマンドラインから入力されたパラメータ
 *								uiDispFaceNum  UINT 表示データ面数(1~10で表示面数、それ以外：最新面のみ)
 *								uiDispLogNum   UINT 表示ログ数    (1~70で表示面数、それ以外：10秒分の情報表示)
 * @param  outFp			[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_printAlmLogALL(int argc, char *argv[], FILE *outFp )
{
	UINT							uiDispFaceNum;		/* 第一引数 */
	UINT							uiDispLogNum;		/* 第二引数 */
	T_DPDA_ALM_LOG_DEBUG*			almLog;
	UINT							uiDataCnt;
	UINT							uiDataNum;
	UINT							ui1SecLogNum;
	UINT							uiDispIndex;
	UINT							uiSetDispIndex;
	UINT							uiCyclicFlg;
	int								uiargc = 4;
	CHAR							cDspbit[16];
	CHAR							cSetDispIndex[8];
	CHAR							c1SecLogNum[8];
	char 							*alm_param[4];
	long int						lastNum;
	char* 							endptr = "\0";
	CHAR							cAlmLog[] = "almlog";
	UINT							uiRet;
	INT								errcd;

	/************************************************************/
	/* Get dpda alarm log(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_ALM_LOG, (VOID **)&almLog, &errcd);
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	if( argc != 3 )
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	
	/* 引数を取得 */
	errno=0;
	lastNum = strtol(argv[1], &endptr, 10); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		uiDispFaceNum = (UINT)lastNum; /* pgr0539 */
	}
	errno=0;
	lastNum = strtol(argv[2], &endptr, 10);  /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		uiDispLogNum = (UINT)lastNum; /* pgr0539 */
	}
	/********************************************************************************/
	/*	すでにMax面分のデータが格納済みかどうかを確認する							*/
	/*	(10面目の時刻データが格納されているかどうかで判定する)						*/
	/********************************************************************************/
	if( *(UINT*)&(almLog->almLog[D_DPDA_ALM_LOG_FACE-1].alm_log_info[0].time.year) == 0 )
	{
		uiCyclicFlg = D_SYS_OFF;
	}
	else
	{
		uiCyclicFlg = D_SYS_ON;
	}
	
	/********************************************************************************/
	/*	表示面数、表示開始位置を決定する											*/
	/*	・全ログ表示の場合は格納されている分の全ログを古いものから順に表示			*/
	/*	  (無効分のログデータは表示しない)											*/
	/*	・ログ数指定の場合															*/
	/*	  表示指定ログ数 > 有効ログ数 の場合は、有効ログ数分、古いものから順に表示	*/
	/*	  上記以外の場合は、指定された表示ログ数分、古いものから順に表示			*/
	/********************************************************************************/
	if( (uiDispFaceNum == 0)
		|| (uiDispFaceNum > D_DPDA_ALM_LOG_FACE) )
	{
		if( uiCyclicFlg == D_SYS_OFF )
		{
			uiDataNum = D_DPDA_MACRO_MIN(almLog->almLogFace+1, D_DPDA_ALM_LOG_FACE);
			uiDispIndex = 0;
		}
		else
		{
			uiDataNum = D_DPDA_ALM_LOG_FACE;
			uiDispIndex = (almLog->almLogFace+1) % D_DPDA_ALM_LOG_FACE ;
		}
	}
	else
	{
		if( uiCyclicFlg == D_SYS_OFF )
		{
			uiDataNum = D_DPDA_MACRO_MIN( almLog->almLogFace+1, uiDispFaceNum );
			if( uiDispFaceNum > almLog->almLogFace )
			{
				uiDispIndex = 0;
			}
			else
			{
				uiDispIndex = almLog->almLogFace - uiDispFaceNum +1;
			}
		}
		else
		{
			uiDataNum = uiDispFaceNum;
			uiDispIndex = (D_DPDA_ALM_LOG_FACE+almLog->almLogFace-uiDispFaceNum+1) % D_DPDA_ALM_LOG_FACE;
		}
	}

	/********************************************************************************/
	/*	1面あたりの表示データ数(1秒データ)を決定する								*/
	/*	・0指定(指定なし), および 70以上が指定された場合はデフォルト値指定			*/
	/*	・上記以外の場合は指定数分を指定する										*/
	/********************************************************************************/
	if( (uiDispLogNum > D_DPDA_ALM_LOG_MAX)
		|| (uiDispLogNum == 0) )
	{
		ui1SecLogNum = 10;
	}
	else
	{
		ui1SecLogNum = uiDispLogNum;
	}
	
	alm_param[0] = cAlmLog;
	
	memset(cDspbit, 0x0, sizeof(cDspbit));
	snprintf( cDspbit, sizeof(cDspbit), "%x", 0xFFFF );
	alm_param[1] = cDspbit;
	
	for( uiDataCnt = 0; uiDataCnt < uiDataNum; uiDataCnt++ )
	{
		uiSetDispIndex = uiDispIndex+1;
		
		memset(cSetDispIndex, 0x0, sizeof(cSetDispIndex));
		snprintf( cSetDispIndex, sizeof(cSetDispIndex), "%d", uiSetDispIndex );
		alm_param[2] = cSetDispIndex;		/*	表示面				*/
		
		
		memset(c1SecLogNum, 0x0, sizeof(c1SecLogNum));
		snprintf( c1SecLogNum , sizeof(c1SecLogNum), "%d", ui1SecLogNum );
		alm_param[3] = c1SecLogNum;			/*	表示データ数(全部)	*/

		f_dpddbg_printAlmLog(uiargc, alm_param, outFp);

		uiDispIndex = (uiDispIndex+1)%D_DPDA_ALM_LOG_FACE;
	}
	
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ALM Log Collect表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Log Collectテーブルをコンソールに表示する
 * @param  uiFace	[in]UINT					Face
 * @param  almLog	[in]T_DPDA_ALM_LOG_DEBUG*	alarm log
 * @param  outFp	[in]FILE*					結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 * @date 2015/04/28 ALPHA)近間 TDD SRE 3.5G対応
 */
VOID f_dpddbg_printAlmLogAlarmState(UINT uiFace, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp )
{
	UINT							uiDataCnt;
	T_DPDA_ALM_STATE*				almState;
	T_DPDA_ALM_INFO*				almInfo_p;				/* アラーム情報テーブル参照ポインタ	*/

	/* テーブルポインタ取得 */
	almState = (T_DPDA_ALM_STATE *) almLog->almLog[uiFace].almStateInfo;

	/****************************************************************/
	/* Alarm State表示												*/
	/****************************************************************/
	fprintf( outFp, "\n");
	fprintf( outFp, "  - Alarm State Information(soft table)\n");
	fprintf( outFp, "                                        AlmState  Sts(cur)  OccurNum  ClearNum  OcrTotal  ClrTotal     Ocr1sec     Clr1sec\n");
	fprintf( outFp, "    ----------------------------------------------------------------------------------------------------------------------\n");
	/* ALM Stateテーブル数分情報表示	*/
	for( uiDataCnt = 0; uiDataCnt < D_DPDA_ALM_NUM; uiDataCnt++ )
	{
		/* テーブルポインタ取得 */
		almInfo_p  = (T_DPDA_ALM_INFO *)&f_dpdar_almInfo[uiDataCnt];
		
		fprintf( outFp, "    %-32s :     %5d     %5d     %5d     %5d     %5d     %5d  %10d  %10d\n",
			almInfo_p->Name,
			almState->almState,
			almState->almStateNow,
			almState->almOccurNum1sec,
			almState->almClearNum1sec,
			almState->almOccurNumTotal,
			almState->almClearNumTotal,
			almState->almOccurNumTotal1sec,
			almState->almClearNumTotal1sec );
		almState++;
	}
	
	fprintf( outFp, "\n");
	
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ALM Threshold表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Thresholdテーブルをコンソールに表示する
 * @param  uiFace	[in]UINT					Face
 * @param  almLog	[in]T_DPDA_ALM_LOG_DEBUG*	alarm log
 * @param  outFp	[in]FILE*					結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 * @date 2015/04/28 ALPHA)近間 TDD SRE 3.5G対応
 * @date 2015/08/08 TDI)satou アンテナ関連の情報は出力しないように
 */
VOID f_dpddbg_printAlmLogAlmThreshold(UINT	uiFace, T_DPDA_ALM_LOG_DEBUG* almLog, FILE *outFp )
{
	UINT							*pAlmLogPos;
	UINT							*pAlmLogVal;
	T_DPDA_ALM_THRESHOLD_INFO		*pAlmThInf;
	USHORT							usAlmIndexCnt;
	
	/* テーブルポインタ取得 */
	pAlmLogPos = (UINT *) &(almLog->almLog[uiFace].almThresholdInfo);
	
	fprintf( outFp, "\n");
	/* タイトル作成 */
	fprintf( outFp, "  - Alarm Threshold Information(soft table)\n");

	for( usAlmIndexCnt=0; usAlmIndexCnt<E_DPDA_ALM_THRESH_NUM_MAX; usAlmIndexCnt++ )
	{
		pAlmThInf = (T_DPDA_ALM_THRESHOLD_INFO*)&(f_dpdar_almThreshold[usAlmIndexCnt]);
		/* pAlmLogVal = pAlmLogPos + usAlmIndexCnt*D_DPDA_ANTMAX*D_DPDA_CARMAX; */
		pAlmLogVal = pAlmLogPos + usAlmIndexCnt*D_DPDA_ANTMAX;
		
		fprintf( outFp, "    %-32s :", pAlmThInf->Name);
		fprintf( outFp, "\n");
	}
	fprintf( outFp, "\n");
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド Status Information RPT報告値表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているStatus Information RPT報告値テーブルをコンソールに表示する
 * @param  uiFace	[in]UINT					Face
 * @param  uiIndex	[in]UINT					Index
 * @param  almLog	[in]T_DPDA_ALM_LOG_DEBUG*	alarm log
 * @param  outFp	[in]FILE*					結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 * @date 2015/04/28 ALPHA)近間 TDD SRE 3.5G対応
 * @date 2015/08/07 TDI)satou ANT単位の情報出力を削除
 */
VOID f_dpddbg_printAlmLogStatusInfoAck(	UINT					uiFace,
									UINT					uiIndex,
									T_DPDA_ALM_LOG_DEBUG	*almLog,
									FILE					*outFp )
{
	T_DPDA_STATUS_INFO*			statusInfo;
	
	/* テーブルポインタ取得 */
	statusInfo = &(almLog->almLog[uiFace].alm_log_info[uiIndex].soft_info.statusInfo);

	/****************************************************************/
	/* Status Information RPT表示									*/
	/****************************************************************/
	fprintf( outFp, "\n");
	fprintf( outFp, "  - Status Information(soft table)\n");
	fprintf( outFp, "        Temperature               = %d\n",statusInfo->Temperature);
    fprintf( outFp, "\n");

	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド DPDAタスク情報表示を行う
 * @note  関数処理内容.
 *       -# DPDAタスク情報をコンソールに表示する
 * @param  uiFace	[in]UINT					Face
 * @param  uiIndex	[in]UINT					Index
 * @param  almLog	[in]T_DPDA_ALM_LOG_DEBUG*	alarm log
 * @param  outFp	[in]FILE*					結果出力先ファイル
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 * @date 2015/08/07 TDI)satou システムパラメータ関連情報の出力を削除
 * @date 2015/08/07 TDI)satou 「TxGain係数」情報の出力を削除
 * @date 2015/08/07 TDI)satou 「高速モード」情報の出力を削除
 */
VOID f_dpddbg_printAlmLogTaskInf(	UINT					uiFace,
									UINT					uiIndex,
									T_DPDA_ALM_LOG_DEBUG	*almLog,
									FILE					*outFp )
{
	T_DPDA_SOFT_INFO*			softInfo;

	/* テーブルポインタ取得 */
	softInfo = &(almLog->almLog[uiFace].alm_log_info[uiIndex].soft_info);

	/****************************************************************/
	/* 表示															*/
	/****************************************************************/
	fprintf( outFp, "  - Flg\n");
	fprintf( outFp, "    temperature                = 0x%08x\n",softInfo->dpdaInfo.flg.temperature);
	fprintf( outFp, "    debug mode                 = 0x%08x\n",softInfo->dpdaInfo.flg.dbgmode);

	fprintf( outFp, "  - DBG Flg\n");
	fprintf( outFp, "    register flg               = 0x%08x(%d)\n",softInfo->dpdaInfo.swDbgFlg.virtualRegisterFlg, softInfo->dpdaInfo.swDbgFlg.virtualRegisterFlg);

	fprintf( outFp, "\n");

	return ;
}
/*!
 * @brief 関数機能概要:デバッグコマンド ハードレジスタ値表示を行う
 * @note  1秒周期で取得しているハードレジスタ値テーブルをコンソールに表示する
 * @param  uiFace				[in]UINT	Face
 * @param  uiIndex				[in]UINT	Index
 * @param  almLog               [in] not use
 * @param  outFp                [in] output file pointer
 * @return VOID
 * @date 2013/06/06 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_printAlmLogRegData(	UINT					uiFace,
									UINT					uiIndex,
									T_DPDA_ALM_LOG_DEBUG	*almLog,
									FILE					*outFp )
{
	T_DPDA_ALM_LOG_DEBUG*		dpdLog;
	T_DPDA_REGDATA				*regDataTbl;
	UINT						uiRegCnt;
	UINT						uiRet;
	INT							errcd;

	/************************************************************/
	/* Get dpda alarm log(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_ALM_LOG, (VOID **)&dpdLog, &errcd);
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}

	/* テーブルポインタ取得 */
	regDataTbl = &(dpdLog->almLog[uiFace].alm_log_info[uiIndex].hard_info.RegData );
	
	/****************************************************************/
	/* Register情報表示												*/
	/****************************************************************/
	fprintf( outFp, "\n");
	
	/* 全レジスタ情報(Alarm History用) */
	fprintf( outFp, "  - Register Information\n");
	for( uiRegCnt = 0; uiRegCnt < D_RRH_ALMHIS_REG_VAL_NUM; uiRegCnt++ )
	{
		fprintf( outFp, "    0x%08x = 0x%08x\n", f_dpdar_reg_Tbl[ uiRegCnt ], regDataTbl->regVal[uiRegCnt] );
	}
	
	fprintf( outFp, "\n");
	
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド レジスタ参照変更
 * @note  関数処理内容.
 *       -# 測定系のレジスタを参照するかテーブル参照するか変更する
 * @param  argc				[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv				[in]char*[]	コマンドラインから入力されたパラメータ
 *								uiOnOff UINT 0:OFF(制限無し) other:ON(制限有り)
 * @param  outFp			[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/07 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_changeRegisterFlg(int argc, char *argv[], FILE *outFp)
{
	UINT				uiOnOff;
	T_DPDA_INFO_ALL*	uiInfoall;
	long int			lastNum;
	char* 				endptr = "\0";
	UINT				uiRet;
	INT					errcd;
	
	if( argc <= 1 )
	{
		fprintf( outFp, "parameter not enough.\n");
		fprintf( outFp, "cmd_pfdbg dpda set reg 0\n");
		fprintf( outFp, "cmd_pfdbg dpda set reg 1\n");
		return;
	}

	/************************************************************/
	/* Get dpda manage info (shared memory)						*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DPDA_MNG_TBL, (VOID**)&uiInfoall, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	/* 引数を取得 */
	errno=0;
	lastNum = strtol(argv[1], &endptr, 10);
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		if( (lastNum!=D_SYS_ON) && (lastNum!=D_SYS_OFF) )
		{
			fprintf( outFp, "Parameter out of range.\n");
			return;
		}
		else
		{
			uiOnOff = (UINT)lastNum;
		}
	}

	if( D_SYS_OFF == uiOnOff )
	{
		/* レジスタを参照する	*/
		uiInfoall->taskInfo.swDbgFlg.virtualRegisterFlg = D_SYS_OFF;
		fprintf( outFp, "virtual register flag off.\n");
	}
	else
	{
		/* テーブルを参照する	*/
		uiInfoall->taskInfo.swDbgFlg.virtualRegisterFlg = D_SYS_ON;
		fprintf( outFp, "virtual register flag on.\n");
	}

	return ;
}

/*!
 * @brief 関数機能概要:仮想レジスタ値変更処理
 * @note  関数処理内容.
 *		-# 内部テーブルの値の変更を行う
 * @param  argc				[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv				[in]char*[]	コマンドラインから入力されたパラメータ
 *								uiRegAddr  UINT レジスタアドレス
 *								uiRegVal   UINT レジスタ値
 * @param  outFp			[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/07 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_changeVirtualRegister(int argc, char *argv[], FILE *outFp)
{
	UINT				uiRegAddr;
	UINT				uiRegVal;
	UINT				uiRegCnt;
	T_DPDA_INFO_ALL*	uiInfoall;
	long int			lastNum;
	char* 				endptr = "\0";
	UINT				uiRet;
	INT					errcd;
	
	if( argc <= 2 )
	{
		fprintf( outFp, "parameter not enough.\n");
		fprintf( outFp, "cmd_pfdbg dpda set vreg 81002110 1A000\n");
		return;
	}
	
	/************************************************************/
	/* Get Virtual register info(from shared memory)			*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DPDA_MNG_TBL, (VOID**)&uiInfoall, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	/* 引数を取得 */
	errno=0;
	lastNum = strtoul(argv[1], &endptr, 16); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		uiRegAddr = (UINT)lastNum; /* pgr0539 */
	}

	errno=0;
	lastNum = strtoul(argv[2], &endptr, 16); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		uiRegVal = (UINT)lastNum; /* pgr0539 */
	}

	for( uiRegCnt = 0; uiRegCnt < D_DPDA_DBG_VIRTUAL_REG_CNT; uiRegCnt++ )
	{
		if( uiRegAddr == f_dpdar_virtualRegAddr[uiRegCnt] )
		{
			break;
		}
	}

	if( uiRegCnt != D_DPDA_DBG_VIRTUAL_REG_CNT )
	{
		uiInfoall->virtualRegData[ uiRegCnt ] = uiRegVal;
	}
	else
	{
		fprintf( outFp, "This register can't change value \n");
	}

	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ALM閾値設定処理
 * @note  関数処理内容.
 *       -# Alarm閾値を変更する
 * @param  argc			[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv			[in]char*[]	コマンドラインから入力されたパラメータ
 *							mode		[in]UINT	モード(1:set, 2:clear)
 *							almIndex	[in]UINT	アラーム閾値Index(DPDA管理番号)
 *							alarmThresh	[in]UINT	Alarm閾値
 *							antNo		[in]UINT	アンテナ番号
 * @param  outFp		[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/07 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_almThreshSet(int argc, char *argv[], FILE *outFp)
{
	UINT						mode;
	UINT						almIndex;
	UINT						almThresh;
	UINT						antNo;
	UINT						uiSetAlmThresh;
	USHORT						usAntCnt;
	T_DPDA_ALM_THRESHOLD_INFO*	pAlmThInf;
	T_DPDA_INFO_ALL*			uiInfoall;
	long int					lastNum;
	char* 						endptr = "\0";
	UINT						uiRet;
	INT							errcd;

	/************************************************************/
	/* Get dpda info (from shared memory)						*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DPDA_MNG_TBL, (VOID**)&uiInfoall, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}

	/* 引数を取得 */
	if( argc != 5 )
	{
		f_dpddbg_almThreshSetUsage(outFp);
		return;
	}
	
	errno=0;
	lastNum = strtol(argv[1], &endptr, 10); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		mode = (UINT)lastNum; /* pgr0539 */
	}

	errno=0;
	lastNum = strtol(argv[2], &endptr, 10); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		almIndex = (UINT)lastNum; /* pgr0539 */
	}

	errno=0;
	lastNum = strtol(argv[3], &endptr, 10); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		almThresh = (UINT)lastNum; /* pgr0539 */
	}

	errno=0;
	lastNum = strtol(argv[4], &endptr, 10); /* pgr0539 */
	if(errno !=0)
	{
		fprintf( outFp, "In Parameter error!\n");
		return;
	}
	else
	{
		antNo = (UINT)lastNum; /* pgr0539 */
	}

	/*	INパラ範囲外の場合はusageを表示		*/
	if( ((mode!=D_DPDA_DBG_MODE_ON) && (mode!=D_DPDA_DBG_MODE_OFF))
		|| (almIndex<=0) || ((E_DPDA_ALM_THRESH_NUM_MAX+1)<=almIndex) ) /* pgr1223 */
	{
		f_dpddbg_almThreshSetUsage(outFp);
		return;
	}
	
	pAlmThInf = (T_DPDA_ALM_THRESHOLD_INFO*)&f_dpdar_almThreshold[almIndex-1];
	
	if(antNo > pAlmThInf->AntNum)
	{
		f_dpddbg_almThreshSetUsage(outFp);
		return;
	}
	
	/*	今回設定するアラーム閾値を決定		*/
	if( mode == D_DPDA_DBG_MODE_ON )
	{
		uiSetAlmThresh = almThresh;
	}
	else
	{
		uiSetAlmThresh = pAlmThInf->Threshold;
	}
	
	/*	閾値を変更する						*/
	if( pAlmThInf->AntNum == 0 )
	{
		uiInfoall->almThreshold[almIndex-1][0] = uiSetAlmThresh;
	}
	else
	{
		for( usAntCnt=0; usAntCnt<D_DPDA_ANTMAX; usAntCnt++ )
		{
			if( (usAntCnt < pAlmThInf->AntNum)
				&& ((usAntCnt==(antNo-1)) || (antNo==0)) )
			{
				uiInfoall->almThreshold[almIndex-1][usAntCnt] = uiSetAlmThresh;
			}
		}
	}
	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ALM閾値設定ヘルプ表示
 * @note  関数処理内容.
 *       -# Alarm閾値設定デバッグコマンドのヘルプを表示する
 * @param  outFp			[in]FILE*				結果出力先ファイル
 * @return VOID
 * @date 2013/06/07 ALPHA)中村 B25対応 Create
 * @date 2015/04/28 ALPHA)近間 TDD SRE 3.5G対応
 * @date 2015/08/08 TDI)satou 「antNo」は0固定に
 */
VOID f_dpddbg_almThreshSetUsage( FILE *outFp )
{
	USHORT		usAlmIndexCnt;
	
	fprintf( outFp, "[Usage]\n");
	fprintf( outFp, "cmd_pfdbg dpda set almth [mode] [index] [threshold] [antNo]\n");
	fprintf( outFp, "  mode:\n");
	fprintf( outFp, "    1    : Set   threshold\n");
	fprintf( outFp, "    2    : Clear threshold(set default value)\n");
	fprintf( outFp, "    other: Print USAGE\n");
	fprintf( outFp, "  index:\n");
	fprintf( outFp, "    a kind of alarm threshold(for detail, please refer below list)\n");
	for( usAlmIndexCnt=0; usAlmIndexCnt<E_DPDA_ALM_THRESH_NUM_MAX; usAlmIndexCnt++ )
	{
		fprintf( outFp, "   %02d    : %s\n",
				(usAlmIndexCnt+1),
				f_dpdar_almThreshold[usAlmIndexCnt].Name );
	}
	fprintf( outFp, "  threshold:\n");
	fprintf( outFp, "    a value of alarm threshold\n");
	fprintf( outFp, "  antNo:\n");
	fprintf( outFp, "    0    : fix value\n");
	return;
}

/*!
 * @brief 関数機能概要:デバッグコマンド 1秒周期
 * @note  関数処理内容.
 *       -# リクエストIDによりアサインを行う
 * @param  argc				[in]int		コマンドラインから入力されたパラメータ数
 * @param  argv				[in]char*[]	コマンドラインから入力されたパラメータ
 * @param  outFp			[in]FILE*	結果出力先ファイル
 * @return VOID
 * @date 2013/06/11 ALPHA)中村 B25対応 Create
 */
VOID f_dpddbg_1secReport(int argc, char *argv[], FILE *outFp )
{
	T_SYS_INIT_COMP_NTC			*sndMsg = NULL;					/* 送信メッセージ(初期化完了通知)						*/
	INT							iDetailErrorCode;
	INT							errcd;

	/* 通信用共有メモリを取得 */
	iDetailErrorCode = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別					*/
													sizeof(T_SYS_INIT_COMP_NTC),			/* Size							*/
													(VOID **)&sndMsg,						/* msgP							*/
													&errcd );
	if(iDetailErrorCode != BPF_RM_SVRM_COMPLETE)
	{
		fprintf(  outFp, "add get error");
		return;
	}

	/* 要求送信 */
	/* 送信MSG作成 */
	sndMsg->head.uiEventNo			= D_SYS_MSGID_TIMOUTNTC;			/* イベント番号						*/
	sndMsg->head.uiDstPQueueID		= D_RRH_PROCQUE_F_PF;					/* 最終送信先Process Queue ID		*/
	sndMsg->head.uiDstTQueueID		= D_SYS_THDQID_PF_F_DPD;				/* 最終送信先Thread  Queue ID		*/
	sndMsg->head.uiSrcPQueueID		= D_RRH_PROCQUE_F_PF;					/* 送信元    Process Queue ID		*/
	sndMsg->head.uiSrcTQueueID		= D_SYS_THDQID_PF_DBG;				/* 送信元    Thread  Queue ID		*/

	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_F_DPD,						/* 送信MSGキューID					*/
							sndMsg);									/* 送信MSG							*/

	return ;
}

/*!
 * @brief 関数機能概要:デバッグコマンド 温度情報の表示を行う
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Logテーブルをコンソールに表示する
 * @param  argc				[IN]INT		コマンド指定パラメータ数
 * @param  argv				[IN]CHAR[]*	コマンド
 * @param  outFp			[IN]FILE *	表示データ書き込み用ファイルポインタ
 * @return VOID
 * @date 2013/12/13 ALPHA)加藤 Create ZYNQ対応
 */
VOID f_dpddbg_printTemprature(INT argc, CHAR *argv[], FILE *outFp )
{
	UINT					uiDispNum = 0;
	UINT					uiDispIndex;
	UINT					uiDispCnt;
	T_RRH_LOG_TEMP*			tempLog;
	UINT					uiDispNumInPara;
	long int				lastNum;
	char* 					endptr = "\0";
	UINT					uiRet;
	INT						errcd;

	/* 引数を取得 */
	if( argc == 1 )
	{
		/* INパラ指定なしの場合は 20個分の情報表示 */
		uiDispNumInPara = 20;
	}
	else
	{
		errno=0;
		lastNum = strtol(argv[1], &endptr, 10); /* pgr0539 */
		if(errno !=0)
		{
			fprintf(  outFp, "In Parameter error!\n");
			return;
		}
		else
		{
			uiDispNumInPara = (UINT)lastNum; /* pgr0539 */
			if( uiDispNumInPara > D_RRH_LOG_TEMP_LOG_NUM )
			{
				uiDispNumInPara = D_RRH_LOG_TEMP_LOG_NUM;
			}
		}
	}
	
	/************************************************************/
	/* Get Temprature info(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_TEMP_LOG_TBL,(VOID**)&tempLog, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	/*	最新位置、表示データ数を算出する	*/
	if( D_SYS_OFF == tempLog->uiCycleflg )
	{
		uiDispIndex = 0;
		if( tempLog->uiWriteIdx > uiDispNumInPara )
		{
			uiDispNum  = uiDispNumInPara;
			uiDispIndex = tempLog->uiWriteIdx - uiDispNumInPara;
		}
		else
		{
			uiDispNum  = tempLog->uiWriteIdx;
		}
	}
	else
	{
		uiDispNum	= uiDispNumInPara;
		uiDispIndex	= (D_RRH_LOG_TEMP_LOG_NUM + tempLog->uiWriteIdx - uiDispNum +1) % D_RRH_LOG_TEMP_LOG_NUM;
	}
	
	fprintf(  outFp, "\n###################################################\n");
	fprintf(  outFp, " Temperature information.\n");
	fprintf(  outFp, "###################################################");
	/* 1つずつ表示していく	*/
	for( uiDispCnt = 0; uiDispCnt < uiDispNum; uiDispCnt++ )
	{
		if( 0 == (uiDispCnt % 10) )
		{
			/****************************************************************/
			/* 日付表示														*/
			/****************************************************************/
			fprintf(outFp, "\n %04d/%02d/%02d %02d:%02d:%02d.%03d",
					tempLog->tRecord[uiDispIndex].tDate.year,
					tempLog->tRecord[uiDispIndex].tDate.month,
					tempLog->tRecord[uiDispIndex].tDate.day,
					tempLog->tRecord[uiDispIndex].tDate.hour,
					tempLog->tRecord[uiDispIndex].tDate.min,
					tempLog->tRecord[uiDispIndex].tDate.sec,
					(tempLog->tRecord[uiDispIndex].tDate.msec)*10 );
		}
		
		fprintf(outFp, "%5d ",	tempLog->tRecord[uiDispIndex].tempVal );
		
		uiDispIndex++;
		uiDispIndex %= D_RRH_LOG_TEMP_LOG_NUM;
	}
	fprintf( outFp, "\n");
	
	return;
}

/*!
 * @brief 関数機能概要:デバッグコマンド 温度集計のタイマ値変更
 * @note  関数処理内容.
 *       -# 1秒周期で取得しているALM Logテーブルをコンソールに表示する
 * @param  argc				[IN]INT		コマンド指定パラメータ数
 * @param  argv				[IN]CHAR[]*	コマンド
 * @param  outFp			[IN]FILE *	表示データ書き込み用ファイルポインタ
 * @return VOID
 * @date 2013/12/13 ALPHA)加藤 Create ZYNQ対応
 */
VOID f_dpddbg_TempTimeSet(INT argc, CHAR *argv[], FILE *outFp )
{
	T_RRH_LOG_TEMP*			tempLog;
	long int				lastNum;
	char* 					endptr = "\0";
	UINT					uiRet;
	INT						errcd;
	UINT					timVal;
	
	/* 引数を取得 */
	if( argc < 3 )
	{
		fprintf(  outFp, "parameter not enough.\n");
		return;
	}
	else
	{
		/* 第2パラ(tim)はコマンド側でチェックしているため	*/
		/* ここではチェック不要								*/
		errno=0;
		lastNum = strtol(argv[2], &endptr, 10);
		if(errno !=0)
		{
			fprintf(  outFp, "In Parameter error!\n");
			return;
		}
		else
		{
			/* Max60分でガード */
			if( (lastNum <= 0) || (60 < lastNum) )
			{
				fprintf(  outFp, "In Parameter error!\n");
				return;
			}
				
			timVal = (UINT)lastNum; /* pgr0539 */
		}
	}
	
	/************************************************************/
	/* Get Temprature info(from shared memory)					*/
	/************************************************************/
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_TEMP_LOG_TBL,(VOID**)&tempLog, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}
	
	/*	最新位置、表示データ数を算出する	*/
	tempLog->usTimerVal = timVal * D_RRH_LOG_TEMP_MEAS_PERIOD;
	
	fprintf(  outFp, "measurement temperature period is changed to %d min.\n", timVal );

	return;
}

/*!
 * @brief 関数機能概要:デバッグコマンド ヘルプ表示
 * @note  関数処理内容.
 *       -# ヘルプ表示を行う
 * @param  outFp			[in]FILE*	ヘルプ出力先ファイル
 * @retval VOID
 * @date 2013/06/11 ALPHA)中村 B25対応 Create
 * @date 2015/05/29 ALPHA)近間 RRH-013-000 DCM_3.5G M-RRU-ZSYS-01583対応
 */
VOID f_dpddbg_help( FILE *outFp )
{
	fprintf( outFp, "[Usage]\n");
	fprintf( outFp, "Debug Dpda Help\n");
	fprintf( outFp, "  kind:\n");
	fprintf( outFp, "    Select mode\n");
	fprintf( outFp, "      dsp almlog      :\n");
	fprintf( outFp, "      dsp almall      :\n");
	fprintf( outFp, "      dsp almlogall   :\n");
	fprintf( outFp, "      dsp traall      :\n");
	fprintf( outFp, "      dsp temper      :\n");
	fprintf( outFp, "      set reg         :\n");
	fprintf( outFp, "      set vreg        :\n");
	fprintf( outFp, "      set almth       :\n");
	fprintf( outFp, "      set 1sec        :\n");
	fprintf( outFp, "      set temper tim  :\n");

	return ;
}
/* @} */
