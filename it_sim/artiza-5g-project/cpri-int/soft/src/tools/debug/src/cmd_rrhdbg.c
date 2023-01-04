/*!
 * @skip   $Id$
 * @file   cmd_rrhdbg.c
 * @brief  RRHデバッグコマンド
 * @date   2013/11/24 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

#include <sys/ioctl.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/msg.h>
#include <net/if.h> 
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> 

#include "f_dbg_cmd_inc.h"
#include "bpf_l_com.h"
#include "bpf_i_com.h"
#include "BPF_COM_PLOG.h"
#include "f_du_reg_ctb.h"

/*
 * Function Prototype
 */

int		cmd_rrhdbg_main( int argc, char *argv[], FILE* outfp );
VOID	dbg_AssertLogDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID	dbg_AssertLogLevelDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID	dbg_RegisterLogLevelDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_ResetLogDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_BpfLogDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_CountDisp( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_LbmMngDisp( INT argc, CHAR *argv[], FILE *outfp );

VOID 	dbg_AssertLogLevelSet( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_RegisterLogLevelSet( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_CycleLogCountSet( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_AlmResetCountSet( INT argc, CHAR *argv[], FILE *outfp );
VOID 	dbg_DumpMsgSet( INT argc, CHAR *argv[], FILE *outfp );

VOID 	dbg_AssertLogClear( INT argc, CHAR *argv[], FILE *outfp );
VOID	dbg_ResetLogClear( INT argc, CHAR *argv[], FILE *outfp);
VOID	dbg_BpfLogClear( INT argc, CHAR *argv[], FILE *outfp);
VOID	dbg_BpfMutexClear( INT argc, CHAR *argv[], FILE *outfp);
VOID	dbg_SVBusDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_FHinfDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_PfminfDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_eioLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_fanSpeedDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_fhStatDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_mtLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_txPowDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_txPowLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_lutLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_fhl1hisLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_upSigStopHisLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_calHisLogDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_tOverFailSafeHisDisp(int argc, char *argv[], FILE *outfp );

VOID	dbg_macStsDisp(int argc, char *argv[], FILE *outfp );
VOID	dbg_macStsClear(int argc, char *argv[], FILE *outfp );

VOID	dbg_LoopBackMessageSend0( int argc, char *argv[], FILE *outfp );
VOID	dbg_LoopBackMessageSend1( int argc, char *argv[], FILE *outfp );
INT		dbg_LoopBackMessageSend(int port_no, FILE *outfp );

extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );
extern char *cnvRRH_SYSTIME_TIME2Str(T_RRH_SYSTEM_TIME *tDate) ;
extern void assertLogPrint(T_RRH_LOG_ASSERT_RECORD *astLogRec, FILE *outfp );
extern void regLogRecPrint( T_RRH_LOG_REG_ACCESS_RECORD* regLogRec,  FILE *outfp )	;
extern char *cnvRRH_SYSTIME_TIME2Str2(T_RRH_SYSTEM_TIME *tDate);
/*
 * @name RRH DEBUG Command process type
 * @note デバッグコマンド処理区分
 */
enum
{
	E_RRHDBG_CMD_DSP=0,			/*!< 表示			*/
	E_RRHDBG_CMD_SET,			/*!< 設定			*/
	E_RRHDBG_CMD_CLR,			/*!< クリア			*/
	E_RRHDBG_CMD_SND,			/*!< クリア			*/
	E_RRHDBG_CMD_CTRL_NUM
};

/*!
 * @name Display Command Define
 * @note 表示処理コマンド順定義
 * @{
 */

enum
{
	E_RRHDBG_CMD_ASSERT_LOG_DISP=0,		/*!< Assertログを全表示する。						*/
	E_RRHDBG_CMD_ASSERT_LOGLEVEL_DISP,	/*!< Assertログの保存閾値を表示する。				*/
	E_RRHDBG_CMD_REG_LOGLEVEL_DISP,		/*!< レジスタログ保存閾値を表示する。				*/
	E_RRHDBG_CMD_RESET_LOG_DISP,		/*!< リセット履歴ログを表示する。					*/
 	E_RRHDBG_CMD_BPF_LOG_DISP,			/*!< BPFログ表示									*/
 	E_RRHDBG_CMD_ALM_SELF_DISP,			/*!< ALM時の自律リセットカウント表示				*/
 	E_RRHDBG_CMD_MACSTS_LOG_DISP,		/*!< MACSTATUSカウント表示							*/
 	E_RRHDBG_CMD_SVBUS_LOG_DISP,		/*!< SV-Bus履歴ログ表示								*/
 	E_RRHDBG_CMD_FHINF_LOG_DISP,		/*!< Fronthaul接続情報表示							*/
 	E_RRHDBG_CMD_PFMINF_LOG_DISP,		/*!< Performance情報表示							*/
 	E_RRHDBG_CMD_LBMMNG_DISP,			/*!< LBM/LBR管理テーブル							*/
 	E_RRHDBG_CMD_EIO_LOG_DISP,			/*!< externalIO履歴ログ表示							*/
 	E_RRHDBG_CMD_FAN_SPEED_DISP,		/*!< fan Speed monitor表示							*/
 	E_RRHDBG_CMD_FHSTAT_DISP,			/*!< FHSTAT表示										*/
 	E_RRHDBG_CMD_MTLOG_DISP,			/*!< MTLOG表示										*/
 	E_RRHDBG_CMD_TXPOW_DISP,			/*!< TXPOW表示										*/
 	E_RRHDBG_CMD_TXPOW_LOG_DISP,		/*!< TXPOW LOG表示									*/
 	E_RRHDBG_CMD_LUT_LOG_DISP,			/*!< LUT LOG表示									*/
 	E_RRHDBG_CMD_FHL1HIS_LOG_DISP,		/*!< FHL1 HIS表示									*/
 	E_RRHDBG_CMD_UPSIGSTOPHIS_LOG_DISP,		/*!< FHL1 HIS表示									*/
 	E_RRHDBG_CMD_CALHIS_LOG_DISP,		/*!< CAL HIS表示									*/
 	E_RRHDBG_CMD_DLF_LOG_DISP,			/*!< 過出力フェールセーフ HIS表示					*/
	E_RRHDBG_DSP_CMD_NUM				/*!< 表示コマンド最大数								*/
};

/*
 * @name Setting Command Define
 * @note 設定処理コマンド順定義
 */
enum
{
	E_RRHDBG_CMD_AST_LOGLEVEL_SET=0,	/*!< アサートログレベル設定							*/
	E_RRHDBG_CMD_REG_LOGLEVEL_SET,		/*!< レジスタログレベル設定							*/
	E_RRHDBG_CMD_CYCLE_LOGCOUNT_SET,	/*!< Cyclicログ退避カウンタ設定						*/
	E_RRHDBG_CMD_ALM_SELF_SET,			/*!< ALM時の自律リセットカウンタ設定				*/
	E_RRHDBG_CMD_DUMP_MSG_SET,			/*!< ダンプメッセージ設定							*/
	E_RRHDBG_SET_CMD_NUM				/*!< 設定処理コマンド最大数							*/
};

/*
 * @name Clear Command Define
 * @note 初期化コマンド順定義
 */
enum
{
	E_RRHDBG_CMD_AST_LOG_CLR=0,			/*!< アサートログクリア								*/
	E_RRHDBG_CMD_RST_LOG_CLR,			/*!< リセットログクリア								*/
	E_RRHDBG_CLR_BPF_LOG_CLR,			/*!< BPFログクリア									*/
	E_RRHDBG_CMD_MACSTS_LOG_CLR,		/*!< MAC STSクリア									*/
	E_RRHDBG_CLR_CMD_NUM				/*!< 初期化処理最大数								*/
};

/*
 * @name Send Command Define
 * @note 初期化コマンド順定義
 */
enum
{
	E_RRHDBG_CMD_LBM0_SEND=0,			/*!< LBM送信 (fheth#0)								*/
	E_RRHDBG_CMD_LBM1_SEND,				/*!< LBM送信 (fheth#1)								*/
	E_RRHDBG_SND_CMD_NUM				/*!< 初期化処理最大数								*/
};

/**
 * @brief 実行コマンド定義テーブル詳細型定義
 * @note コマンドパラメータに対応した実行関数を定義する
 *
 */
typedef struct{
	const CHAR	*cmdName;							/**< コマンド名		*/
	VOID	(*func)(INT argc, CHAR *argv[], FILE *);	/**< 実行関数		*/
}T_DBG_CMD_LIST;

/**
 * @brief 実行コマンド定義テーブル型定義
 * @note 各機能毎の実行コマンドを定義する
 *
 */
typedef struct{
	const CHAR					*ctrlName;			/**< 機能名			*/
	USHORT					cmdNum;					/**< コマンド数		*/
	T_DBG_CMD_LIST			cmdList[E_RRHDBG_DSP_CMD_NUM];	/* コマンド詳細	*/
}T_DBG_DSP_CMD_ALL_LIST;

/**
 * コマンド実行定義
 *
 */


/**
 * @name 実行コマンド定義テーブル
 * @note 各機能毎の実行コマンドを定義する
 *
 */
const T_DBG_DSP_CMD_ALL_LIST f_rrhdbg_cmd_list[E_RRHDBG_CMD_CTRL_NUM] =
{
	/* 表示	*/
	{
		"dsp",
		E_RRHDBG_DSP_CMD_NUM,
		{
			{	"ast",				dbg_AssertLogDisp				},
			{	"astloglevel",		dbg_AssertLogLevelDisp			},
			{	"regloglevel",		dbg_RegisterLogLevelDisp		},
			{	"rst",				dbg_ResetLogDisp				},
			{	"bpf",				dbg_BpfLogDisp					},
			{	"count",			dbg_CountDisp					},
			{	"mac",				dbg_macStsDisp					},
			{	"svb",				dbg_SVBusDisp					},
			{	"fhinf",			dbg_FHinfDisp					},
			{	"pfminf",			dbg_PfminfDisp					},
			{	"lbmmng",			dbg_LbmMngDisp					},
			{	"eio",				dbg_eioLogDisp					},
			{	"fanSpeed",			dbg_fanSpeedDisp				},
			{	"fhstat",			dbg_fhStatDisp					},
			{	"mtlog",			dbg_mtLogDisp					},
			{	"txpow",			dbg_txPowDisp					},
			{	"txpowlog",			dbg_txPowLogDisp				},
			{	"lutlog",			dbg_lutLogDisp					},
			{	"fhl1his",			dbg_fhl1hisLogDisp				},
			{	"upsighis",			dbg_upSigStopHisLogDisp			},
			{	"calhis",			dbg_calHisLogDisp				},
			{	"dlfhis",			dbg_tOverFailSafeHisDisp		}
		}
	},
	/* 設定	*/
	{
		"set",
		E_RRHDBG_SET_CMD_NUM,
		{
			{	"astloglevel",		dbg_AssertLogLevelSet			},
			{	"regloglevel",		dbg_RegisterLogLevelSet			},
			{	"cyclelogcount",	dbg_CycleLogCountSet			},
			{	"almresetcnt",		dbg_AlmResetCountSet			},
			{	"dumpmsg",			dbg_DumpMsgSet					}

		}
	},
	/* クリア	*/
	{
		"clr",
		E_RRHDBG_CLR_CMD_NUM,
		{
			{	"ast",				dbg_AssertLogClear				},
			{	"rst",				dbg_ResetLogClear				},
			{	"bpf",				dbg_BpfLogClear					},
			{	"mac",				dbg_macStsClear					}
		}
	},
	/* debug送信	*/
	{
		"snd",
		E_RRHDBG_SND_CMD_NUM,
		{
			{	"lbm0",				dbg_LoopBackMessageSend0		},
			{	"lbm1",				dbg_LoopBackMessageSend1		}
		}
	}
};


/**
 * @brief スレッド名キャッシュ定義
 * @note スレッドキャッシュ
 *
 */
typedef struct  {
	UINT		thid	;	/**<	スレッドID	*/
	char		*thname	;	/**<	スレッド名	*/
}	T_THCASH  ;

/**
 * @name スレッド名キャッシュテーブル
 * @note スレッド名のキャッシュに使用するテーブル
 *
 */
T_THCASH	threadCache[100]	;
int			threadCacheCnt	;



/*!
 * @brief 関数機能概要:デバッグコマンド　メイン処理
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @date 2013/11/24 ALPHA)宮崎 create
 */

int cmd_rrhdbg_main( int argc, char *argv[], FILE* outfp )
{
	UINT						cmdCnt;
	UINT						cmdCtlCnt;
	T_DBG_CMD_LIST				*cmdList;

	if( argc <= 2 )
	{

		fprintf(outfp, "need input argv.\n\n"
		"cmd_rrhdbg dsp ast\n"
		"cmd_rrhdbg dsp astloglevel\n"
		"cmd_rrhdbg dsp regloglevel\n"
		"cmd_rrhdbg dsp rst\n"
		"cmd_rrhdbg dsp bpf\n"
		"cmd_rrhdbg dsp count\n"
		"cmd_rrhdbg dsp mac\n"
		"cmd_rrhdbg dsp svb\n"
		"cmd_rrhdbg dsp fhinf\n"
		"cmd_rrhdbg dsp pfminf\n"
		"cmd_rrhdbg dsp lbmmng\n"
		"cmd_rrhdbg dsp eio\n"
		"cmd_rrhdbg dsp fanSpeed\n"
		"cmd_rrhdbg dsp fhstat\n"
		"cmd_rrhdbg dsp mtlog\n"
		"cmd_rrhdbg dsp txpow\n"
		"cmd_rrhdbg dsp txpowlog\n"
		"cmd_rrhdbg dsp lutlog\n"
		"cmd_rrhdbg dsp fhl1his\n"
		"cmd_rrhdbg dsp upsighis\n"
		"cmd_rrhdbg dsp calhis\n"
		"cmd_rrhdbg set astloglevel\n"
		"cmd_rrhdbg set regloglevel\n"
		"cmd_rrhdbg set cyclelogcount\n"
		"cmd_rrhdbg set almresetcnt\n"
		"cmd_rrhdbg set dumpmsg\n"
		"cmd_rrhdbg clr ast\n"
		"cmd_rrhdbg clr rst\n"
		"cmd_rrhdbg clr bpf\n"
		"cmd_rrhdbg clr mac\n"
		);
		return	0;
	}




	for( cmdCtlCnt=0; cmdCtlCnt<E_RRHDBG_CMD_CTRL_NUM; cmdCtlCnt++ )
	{
		if(0 == strcmp(argv[1], f_rrhdbg_cmd_list[cmdCtlCnt].ctrlName))
		{
			cmdList = (T_DBG_CMD_LIST *)&(f_rrhdbg_cmd_list[cmdCtlCnt].cmdList[0]);

			for( cmdCnt=0; cmdCnt < f_rrhdbg_cmd_list[cmdCtlCnt].cmdNum; cmdCnt++ )
			{
				if(0 == strcmp(argv[2], cmdList->cmdName))
				{

					(VOID)(*(cmdList->func))((argc-2), &argv[2], outfp );

					return	0;
				}
				cmdList++;
			}

			if( cmdCnt == f_rrhdbg_cmd_list[cmdCtlCnt].cmdNum )
			{
				fprintf(stderr, "Error: command %s %s is not support.\n",argv[1],argv[2]);
				return 0;
			}
		}
	}

	if( E_RRHDBG_CMD_CTRL_NUM == cmdCtlCnt )
	{
		fprintf(stderr, "Error: command rrhdbg %s is not support.\n",argv[1]);
	}

	return 0;
}

/*!
 * @brief 関数機能概要:アサートログ表示
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_AssertLogDisp(int argc, char *argv[], FILE *outfp )
{
	USHORT					usStartIndex;
	USHORT					usDispNum;
	USHORT					usDispCnt;
	USHORT					usAstIndex;
	T_RRH_LOG_ASSERT*		assertLog;
	UINT					uiDispNum_inPara=0;
	INT						iRetCode;
	INT						iErrCode;

	/************************************************************/
	/* Get Assert log(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_ASSERT_LOG, (VOID **)&assertLog, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	/****************************************************************************************************************/
	/* Assertログ表示数の決定																						*/
	/* 指定なし: default数分表示																					*/
	/* Max以上 : Maxでガード																						*/
	/* 上記以外: 指定数分表示																						*/
	/****************************************************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum_inPara = atoi(argv[1]);
	}

	if( uiDispNum_inPara == 0 )
	{
		usDispNum = 50;
	}
	else if( uiDispNum_inPara > D_RRH_LOG_MAX_ASSERTLOGNUM )
	{
		usDispNum = D_RRH_LOG_MAX_ASSERTLOGNUM;
	}
	else
	{
		usDispNum = uiDispNum_inPara;
	}

	/****************************************************************************************************************/
	/* AssertLogサイクリック確認																					*/
	/* 最終位置の年月日が0以外であればサイクリック有りとしてログ表示開始位置指定									*/
	/* 上記以外の場合は、指定ログ数と取得ログ数からログ表示開始位置指定												*/
	/****************************************************************************************************************/
	if( 0 != assertLog->tRecord[D_RRH_LOG_MAX_ASSERTLOGNUM-1].tDate.year )
	{
		usStartIndex = ((D_RRH_LOG_MAX_ASSERTLOGNUM + assertLog->uiWriteIdx - usDispNum) % D_RRH_LOG_MAX_ASSERTLOGNUM);
	}
	else
	{
		if( usDispNum < assertLog->uiWriteIdx )
		{
			usStartIndex = assertLog->uiWriteIdx - usDispNum;
		}
		else
		{
			usStartIndex = 0;
			usDispNum = assertLog->uiWriteIdx;
		}
	}

	if( 0 != usDispNum  )
	{
		fprintf(outfp, "NO;Data;Time;Code;Tid;Message\n");
		usAstIndex = usStartIndex;
		for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
		{
/*
表示形式例
			2001/01/01 01:01:01:200.0000000;ERROR;000       ;f_pf_test.c(12);fopen error return code -4
*/
			/* １レコード分出力	*/
			assertLogPrint( &assertLog->tRecord[usAstIndex], outfp );


			/* 出力位置更新	*/
			usAstIndex = ((usAstIndex+1) % D_RRH_LOG_MAX_ASSERTLOGNUM);

		}
	}
	return ;
}

/*!
 * @brief 関数機能概要:アサートログレベル表示
 * @note  関数処理内容.
 *       -# 共有メモリからアサートログレベルを取得して表示する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_AssertLogLevelDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;

	/* 共有メモリからアサートログレベルを取得	*/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)", iRetCode) ;
		return ;
	}

	fprintf(outfp, "AsserLogLevel: %08x\n", tLogLevelMng->assertLogLevel	);
	return ;
}





/*!
 * @brief 関数機能概要:レジスタログレベル表示
 * @note  関数処理内容.
 *       -# 共有メモリからレジスタログレベルを取得して表示する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_RegisterLogLevelDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*			tLogLevelMng;

	/* 共有メモリからログレベル管理情報を取得	*/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}

	fprintf(outfp, "Register LogLevel: %08x\n", tLogLevelMng->registerLogLevel	);
	return ;
}

/*
 * @name cMsg
 * @note リセットリーズン→リセット要因名(英)変換用テーブル
 */
const  CHAR					*resetResaonDesc[D_RRH_RST_RSN_NUM+1]=
{
	"UNKNOWN"		,
	"POWER"			,
	"M-plane_RPC_RESET"	,
	"MAN"			,
	"MMI"			,
	"M-Plane_Ether_LinkDown"	,
	"CPRI"			,
	"WDT"			,
	"PRM_CHG"		,
	"IMG_FAIL"		,
	"HRD_FAIL"		,
	"SFT_FAIL"		,
	"OSE_SHELLD"	,
	"PROCESS_DEAD"	,
	"M-plane_SV_TIMEOUT"	,
	"EWDT"			,
	"RLBK"			,
	"BOOTDWL"		,
	"RSTSW"			,
	"SWRST1"		,
	"SWRST2"		,
	"RSV1"			,
	"L1RST1"		,
	"L1RST2"		,
	"L3_FAIL"		,
	"NUM"
};


/* アプリプロセス名 f_rrh_def.h の順番	*/
char	*aprProcNames[D_RRH_PROCID_MAX]	= {
	"init",		/*!< D_RRH_PROCID_INI プロセスID(INIT)	*/
	"pf",		/*!< D_RRH_PROCID_PF  プロセスID(PF)	*/
	"dbg",		/*!< D_RRH_PROCID_DBG プロセスID(DBG)	*/
	"ptp",		/*!< D_RRH_PROCID_PTP  プロセスID(PTP)	*/
	"duCmd"		/*!< D_RRH_PROCID_DUCMD  プロセスID(DUCMD)	*/
};

char	*aprProcNames_MP[2]	= {
	"SAA",		/*!< SAA	*/
	"NSA"		/*!< NSA	*/
};

	/*!
 * @brief 関数機能概要:リセットログ表示
 * @note  関数処理内容.
 *       -# 共有メモリよりリセットのログを取得し表示する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */
char *editResetReasonDesc( T_RRH_LOG_RST_REASON_RECORD *rstRec )	{
	static char rstRsnDesc[100];	/* リセット要因詳細編集バッファ	*/

	/* リセット要因を設定	*/
	strcpy( rstRsnDesc, ( rstRec->resetReason < D_RRH_RST_RSN_NUM )?
							resetResaonDesc[rstRec->resetReason]:
							resetResaonDesc[0] ) ;

	if(rstRec->resetReason == D_RRH_RST_RSN_PROCESS_DEAD ){	/* 子プロセス死亡？	*/
		strcat( rstRsnDesc, "(");

		/* 付加情報に入っている死亡プロセスＩＤ（アプリプロセスＩＤ）から死亡プロセス名を表示	*/
		strcat( rstRsnDesc, "Dead Process is ");
		switch(rstRec->addInfo){
			case D_RRH_PROCID_MPSW_SAA:
				strcat( rstRsnDesc, (const char *)aprProcNames_MP[0]) ;
				break;
			case D_RRH_PROCID_MPSW_NSA:
				strcat( rstRsnDesc, (const char *)aprProcNames_MP[1]) ;
				break;
			default:
				strcat( rstRsnDesc, ( rstRec->addInfo < D_RRH_PROCID_MAX)?
								aprProcNames[rstRec->addInfo]: "UNKOWN" ) ;
				break;
		}

		strcat( rstRsnDesc, ")");
	}
	return rstRsnDesc;
}
VOID dbg_ResetLogDisp(int argc, char *argv[], FILE *outfp )
{
	USHORT					usStartIndex;
	USHORT					usDispNum;
	USHORT					usDispCnt;
	USHORT					usRstIndex;

	T_RRH_LOG_RST_REASON*	reset;
	UINT					uiDispNum_inPara=0;
	INT						iRetCode;
	INT						iErrCode;

	/************************************************************/
	/* Get Assert log(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_RST_REASON_LOG, (VOID **)&reset, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	if( reset->uiWriteIdx >= D_RRH_LOG_RSTRSN_RCD_NUM )
	{
		fprintf(outfp, "reset reason log is empty.\n");
		return ;
	}

	/****************************************************************************************************************/
	/* Resetログ表示数の決定																						*/
	/* 指定なし: default数分表示																					*/
	/* Max以上 : Maxでガード																						*/
	/* 上記以外: 指定数分表示																						*/
	/****************************************************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum_inPara = atoi(argv[1]);
	}

	if( uiDispNum_inPara == 0 )
	{
		usDispNum = 10;
	}
	else if( uiDispNum_inPara > D_RRH_LOG_RSTRSN_RCD_NUM )
	{
		usDispNum = D_RRH_LOG_RSTRSN_RCD_NUM;
	}
	else
	{
		usDispNum = uiDispNum_inPara;
	}

	/****************************************************************************************************************/
	/* ResetLogサイクリック確認																						*/
	/* サイクリックしている場合は、指定ログ数と取得ログ数からログ表示開始位置指定									*/
	/****************************************************************************************************************/
	if( D_RRH_ON == reset->uiCycleFlg )
	{
		/* サイクリック有り	*/
		usStartIndex = ((D_RRH_LOG_RSTRSN_RCD_NUM + reset->uiWriteIdx - usDispNum) % D_RRH_LOG_RSTRSN_RCD_NUM);
	}
	else
	{
		/* サイクリック無し	*/
		if( usDispNum < reset->uiWriteIdx )
		{
			usStartIndex = reset->uiWriteIdx - usDispNum;
		}
		else
		{
			usStartIndex = 0;
			usDispNum = reset->uiWriteIdx;
		}
	}

	if( 0 != usDispNum  )
	{
		usRstIndex = usStartIndex;
		fprintf( outfp, "DateTime;[ResetReasonNo]ResetReasonDesc;Version(HW&SW)\n");
		for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
		{
			if(reset->tRecord[usRstIndex].tDate.day != 0 && reset->tRecord[usRstIndex].tDate.month != 0 && reset->tRecord[usRstIndex].tDate.year != 0 ) {
				fprintf( outfp, "%s;[%2d]%s;HW:%04x;SW:%04x\n",
						cnvRRH_SYSTIME_TIME2Str( &reset->tRecord[usRstIndex].tDate),
						reset->tRecord[usRstIndex].resetReason,
						editResetReasonDesc( &reset->tRecord[usRstIndex] ),
						(reset->tRecord[usRstIndex].version&0xffff0000)>>16,
						(reset->tRecord[usRstIndex].version&0xffff)
				);
			}
			/* 出力位置更新	*/
			usRstIndex = ((usRstIndex+1) % D_RRH_LOG_RSTRSN_RCD_NUM);
		}
	}

	return;
}

/*
 *  BPFログ表示
 */
VOID dbg_BpfLogDisp(int argc, char *argv[], FILE *outfp )
{
	int 	cnt		;
	int 	tmpcnt	;


	t_bpf_ru_log_data_tbl* 		log_data_tbl;            /* ログ管理情報           */
	extern t_bpf_ru_log_data* di_log_data_p;

	tmpcnt = di_log_data_p->index;
	for(cnt = 0; cnt <LOG_DATA_LINE;cnt++)	{

		if (tmpcnt >= LOG_DATA_LINE)	{
			tmpcnt = 0;
		}
		log_data_tbl = &(di_log_data_p->data_info[tmpcnt]);
		tmpcnt++;
		if (log_data_tbl->no == 0)	{
			continue;
		}
		if(log_data_tbl->time.year != 0 && log_data_tbl->time.day != 0  )	{
			//999;9999-99-99 99:99:99:999.999;99999.c;99;9999();x9x9x9x9x;999;x9x9x9x\n
			fprintf(outfp, "%d;%s;%s;%d;%s;[%s];%s\n",
					log_data_tbl->no,
					cnvRRH_SYSTIME_TIME2Str(&log_data_tbl->time),
					log_data_tbl->filename,
					log_data_tbl->line,
					log_data_tbl->funcname,
					log_data_tbl->event,
					log_data_tbl->threadname);
		}
	}
	return;
}

/*!
 * @brief 関数機能概要:アサートログマスク設定
 * @note  関数処理内容.
 *       -# 共有メモリへアサートログマスクの設定を行う
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_AssertLogLevelSet(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	unsigned int			set_log_level	;

	if( argc == 2 ){	/* 上位で既に-2されているため */
		/* 共有メモリからアサートログレベルのアドレスを取得	*/
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
		/* パラメータ文字列を数値に変換	*/
		if( cnvHexStr2Long( argv[3-2], &set_log_level )	!= RTC_OK ) {
			fprintf(stderr, "Error: setting loglevel error\n") ;
			return   ;
		}
		/* アサートログレベル設定	*/
		tLogLevelMng->assertLogLevel	= set_log_level;
	}
	else {
		fprintf(outfp, "usage: cmd_rrhdbg set astloglevel x\n");
	}
	return ;
}
/*!
 * @brief 関数機能概要:レジスタログマスク設定
 * @note  関数処理内容.
 *       -# 共有メモリへレジスタログマスクの設定を行う
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_RegisterLogLevelSet(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	unsigned int			set_log_level	;

	if( argc == 2 ){ 	/* 上位で既に-2されているため */
		/* 共有メモリからレジスタログレベルのアドレスを取得	*/
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
		/* パラメータ文字列を数値に変換	*/
		if( cnvHexStr2Long(argv[3-2], &set_log_level )	!= RTC_OK ) {
			fprintf(stderr, "Error: setting loglevel error\n") ;
			return   ;
		}

		tLogLevelMng->registerLogLevel	= set_log_level;
	}
	else {
		fprintf(outfp, "usage: cmd_rrhdbg set regloglevel x\n");
	}
	return ;
}

/*!
 * @brief 関数機能概要:Cyclicログ退避カウンタ設定
 * @note  関数処理内容.
 *       -# 共有メモリへCyclicログ退避カウンタの設定を行う
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2014/02/04 ALPHA)横山 create
 */
VOID dbg_CycleLogCountSet(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	unsigned int			set_log_count;

	if( argc == 2 ){ 	/* 上位で既に-2されているため */
		/* 共有メモリからレジスタログレベルのアドレスを取得	*/
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
		/* パラメータ文字列を数値に変換	*/
		if( cnvHexStr2Long(argv[3-2], &set_log_count )	!= RTC_OK ) {
			fprintf(stderr, "Error: setting cycle log count error\n") ;
			return   ;
		}

		tLogLevelMng->cycleLogCount	= set_log_count;
	}
	else {
		fprintf(outfp, "usage: cmd_rrhdbg set cyclelogcount x\n");
	}
	return ;
}

/*!
 * @brief 関数機能概要:ALMリセットカウンタ設定
 * @note  関数処理内容.
 *       -# 共有メモリへCyclicログ退避カウンタの設定を行う
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_AlmResetCountSet(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
	unsigned int			set_almreset_count;

	if( argc == 2 ){ 	/* 上位で既に-2されているため */
		/* 共有メモリからレジスタログレベルのアドレスを取得	*/
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
		/* パラメータ文字列を数値に変換	*/
		if( cnvHexStr2Long(argv[3-2], &set_almreset_count )	!= RTC_OK ) {
			fprintf(stderr, "Error: setting alm reset count error\n") ;
			return   ;
		}
		tLogLevelMng->selfResetCount = set_almreset_count;
		tLogLevelMng->selfResetFlg = D_RRH_ON;
	}
	else {
		fprintf(outfp, "usage: cmd_rrhdbg set almreset x\n");
	}
	return ;
}


/*!
 * @brief 関数機能概要:ダンプメッセージ設定
 * @note  関数処理内容.
 *       -# ダンプメッセージ設定を行う
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2013/11/24 ALPHA)宮崎 create
 */

VOID dbg_DumpMsgSet(int argc, char *argv[], FILE *outfp )
{
	UCHAR	hwSrcAddr[6];
	UCHAR	hwDstAddr[6];
	CHAR	dp_header[32];
	INT		sd;
	INT	mode;
	INT		length;
	CHAR	value[1024];
	INT		ret;
	UINT		count;
	UINT		sendcount;
	char* 		endptr = "\0";
	INT			tmpval;
	UINT		readdat;

	if( argc >= 2 ){	/* 上位で既に-2されているため */
		mode = atoi(argv[1]);

		BPF_HM_DEVC_REG_READ( DBG_CMD_LOGLEVEL, D_DU_REG_CTB_SWWORK_DP_CUMAC_H, &readdat );
		hwDstAddr[0] = (UCHAR)((readdat & 0x0000FF00) >> 8);
		hwDstAddr[1] = (UCHAR)(readdat & 0x000000FF);

		BPF_HM_DEVC_REG_READ( DBG_CMD_LOGLEVEL, D_DU_REG_CTB_SWWORK_DP_CUMAC_L, &readdat );
		hwDstAddr[2] = (UCHAR)((readdat & 0xFF000000) >> 24);
		hwDstAddr[3] = (UCHAR)((readdat & 0x00FF0000) >> 16);
		hwDstAddr[4] = (UCHAR)((readdat & 0x0000FF00) >> 8);
		hwDstAddr[5] = (UCHAR)( readdat & 0x000000FF);

		BPF_HM_DEVC_REG_READ( DBG_CMD_LOGLEVEL, D_DU_REG_CTB_SWWORK_DP_RUMAC_H, &readdat );
		hwSrcAddr[0] = (UCHAR)((readdat & 0x0000FF00) >> 8);
		hwSrcAddr[1] = (UCHAR)(readdat & 0x000000FF);

		BPF_HM_DEVC_REG_READ( DBG_CMD_LOGLEVEL, D_DU_REG_CTB_SWWORK_DP_RUMAC_L, &readdat );
		hwSrcAddr[2] = (UCHAR)((readdat & 0xFF000000) >> 24);
		hwSrcAddr[3] = (UCHAR)((readdat & 0x00FF0000) >> 16);
		hwSrcAddr[4] = (UCHAR)((readdat & 0x0000FF00) >> 8);
		hwSrcAddr[5] = (UCHAR)( readdat & 0x000000FF);

		memset(dp_header,0,sizeof(dp_header));
		memset(value,0,sizeof(value));

		fprintf(outfp, "Startting dumpmsg(%d) \n",mode);
		switch(mode){
			case 0:
			case 1:
				if (( sd = BPF_COM_DPLOG_AISG_INT(hwSrcAddr,hwDstAddr,dp_header) ) < 0 ){
					fprintf(outfp, "aisg init error(%d) \n",errno);
					break;
				}
				if(mode == 0){
					if(argc >= 3){
						length = 0;
						for(count=2 ; count < argc ;count++,length++){
							tmpval = strtol(argv[count], &endptr, 16);
							if((errno == ERANGE) || ( strlen(endptr) != 0 )){
								break;
							}
							value[length] = (CHAR)tmpval;
						}
						sendcount = 1;
					}else{
						sendcount = 256;
					}
					for(count = 0 ; count < sendcount ;count++){
						if(sendcount == 256){
							length = count + 2;
							memset(value,count,length);
						}
						ret = BPF_COM_DPLOG_AISG_SND(sd,dp_header,length,value);
						if(ret != 0){
							fprintf(outfp, "aisg send(%d) error(%d) \n",length,ret);
							break;
						}
						ret = BPF_COM_DPLOG_AISG_RCV(sd,dp_header,length,value);
						if(ret != 0){
							fprintf(outfp, "aisg recv(%d) error(%d) \n",length,ret);
							break;
						}
					}
					if(count == sendcount){
						fprintf(outfp, "aisg send(%d) OK \n",length);
						fprintf(outfp, "aisg recv(%d) OK \n",length);
					}
				}else{
					if(argc >= 3){
						length = snprintf(value,sizeof(value),"%s",argv[2]);
					}else{
						length = snprintf(value,sizeof(value),"This is aisg message !!!!");
					}
					ret = BPF_COM_DPLOG_AISG_TXT(sd,dp_header,length,value);
					if(ret == 0){
						fprintf(outfp, "aisg txt(%d) OK\n",length);
					}else{
						fprintf(outfp, "aisg txt(%d) error(%d)\n",length,ret);
					}
				}
				close(sd);
				break;
			case 2:
				if (( sd = BPF_COM_DPLOG_DBG_INT(hwSrcAddr,hwDstAddr,dp_header) ) < 0 ){
					fprintf(outfp, "dbg init error(%d) \n",errno);
					break;
				}
				if(argc >= 3){
					length = snprintf(value,sizeof(value),"%s",argv[2]);
				}else{
					length = snprintf(value,sizeof(value),"This is debug message !!!!");
				}
				ret = BPF_COM_DPLOG_DBG_TXT(sd,dp_header,length,value);
				if(ret == 0){
					fprintf(outfp, "debug txt(%d) OK\n",length);
				}else{
					fprintf(outfp, "debug txt(%d) error(%d)\n",length,ret);
				}
				close(sd);
				break;
			default:
				break;
		}
		fprintf(outfp, "finish dumpmsg(%d) \n",mode);
	}
	else {
		fprintf(outfp, "usage: cmd_rrhdbg set dumpmsg <mode> <option> \n");
	}
	return ;
}

typedef struct {
	unsigned int	shmkey;		/* 共有メモリキー	*/
	unsigned int	semkey;		/* セマフォキー		*/
	unsigned int	logsiz;		/* ログサイズ		*/
} T_CLEAR_LOG;
T_CLEAR_LOG		clLogSetting[]	=	{
	{	E_RRH_SHMID_APL_ASSERT_LOG,			E_RRH_SEMID_APL_ASSERT_LOG,			sizeof(T_RRH_LOG_ASSERT)		},
	{	E_RRH_SHMID_APL_RST_REASON_LOG,		E_RRH_SEMID_APL_RST_REASON_LOG,		sizeof(T_RRH_LOG_RST_REASON)	},
	{	BPF_COM_IPCKEY_MASK_LOG,			0,									sizeof(t_bpf_ru_log_data)		}
};



VOID dbg_ClearLog( int logNo )	{
	int		iRetCode;
	int		iErrCode;
	VOID*	logbuf = NULL;
	/* 共有メモリアドレスアタッチ	*/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( clLogSetting[logNo].shmkey, (VOID **)&logbuf, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d,err:%d)\n", iRetCode, iErrCode ) ;
		return ;
	}

	/* セマフォ取得	*/
	if(( iRetCode = BPF_RU_IPCM_PROCSEM_TAKE( clLogSetting[logNo].semkey, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, D_RRH_NULL, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: semaphore  get error.  (code:%d,err:%d)\n", iRetCode, iErrCode ) ;
		return ;
	}

	/* クリア	*/
	memset( logbuf, 0, clLogSetting[logNo].logsiz );

	/* セマフォ解放	*/
	if(( iRetCode = BPF_RU_IPCM_PROCSEM_GIVE( clLogSetting[logNo].semkey, BPF_RU_IPCM_LOCK_RW, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory release error.  (code:%d,err:%d)\n", iRetCode, iErrCode ) ;
		return ;
	}

}

/*!
 * @brief 関数機能概要:アサートログクリア
 * @note  関数処理内容.
 *       -# 共有メモリのアサートログをクリアする
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2014/01/15 ALPHA)宮崎 create
 */

VOID dbg_AssertLogClear(int argc, char *argv[], FILE *outfp )
{
	dbg_ClearLog(E_RRHDBG_CMD_AST_LOG_CLR);
	fprintf(outfp, "ast log clear\n");

	return;
}
/*!
 * @brief 関数機能概要:リセットログクリア
 * @note  関数処理内容.
 *       -# 共有メモリのリセットログをクリアする
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2014/01/15 ALPHA)宮崎 create
 */

VOID dbg_ResetLogClear(int argc, char *argv[], FILE *outfp )
{
	dbg_ClearLog(E_RRHDBG_CMD_RST_LOG_CLR);
	fprintf(outfp, "reset log clear\n");
	return;
}

/*!
 * @brief 関数機能概要:BPFログクリア
 * @note  関数処理内容.
 *       -# 共有メモリのBPFログをクリアする
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2014/01/15 ALPHA)宮崎 create
 */

VOID dbg_BpfLogClear(int argc, char *argv[], FILE *outfp )
{
	extern t_bpf_ru_log_data* di_log_data_p;
	memset(di_log_data_p, 0, sizeof(t_bpf_ru_log_data));
	fprintf(outfp, "Bpf log clear\n");
	return;
}


/*!
 * @brief 関数機能概要:カウンタ表示
 * @note  関数処理内容:カウンタ表示
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_CountDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;

	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_LOGLEVEL_MNG, (VOID **)&tLogLevelMng, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	fprintf(outfp, "LogCount      :cycle %d,cur %d \n",tLogLevelMng->cycleLogCount,tLogLevelMng->cycleLogCount_cur);
	fprintf(outfp, "selfResetCount:cycle %d,cur %d,flg %d\n",tLogLevelMng->selfResetCount,tLogLevelMng->selfResetCount_cur , tLogLevelMng->selfResetFlg);
	return ;
}




/*!
 * @brief 関数機能概要:MAC statusログ表示
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2018/9/6 FJT)Taniguchi
  */

static char* dbg_macSts_linkupdown[2] = {
	"UP","DOWN"
};


VOID dbg_macStsDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_ETH_ERR_HIS*	mac_p;
	UINT					uiIndex;
	USHORT					usDispNo1 = 0;
	USHORT					usDispNo2 = 0;
	USHORT					usDispNo3 = 0;
	UINT					shmid;

	shmid = E_RRH_SHMID_APL_MAC_STAT_ETH0;
	for(usDispNo1 = 0 ; usDispNo1 < 1 ;usDispNo1++){
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( shmid, (VOID **)&mac_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
				fprintf( outfp, "Port%d [No ]Time                               Sts      Info     SfpInfo  MacHis   JcPllalm LinkSts \n", usDispNo1+1);
		uiIndex = mac_p->uiWriteIdx;
		for( usDispNo2=0; usDispNo2 < D_RRH_LOG_ETHERR_RCD_NUM; usDispNo2++ )
		{
			if(		(mac_p->tRecord[ uiIndex ].sysTime.year >= 2000 )
				&&	(mac_p->tRecord[ uiIndex ].sysTime.year <= 2099 )
				&&	(mac_p->tRecord[ uiIndex ].validCode   == 0x11111111 )
				 ){
				usDispNo3++;
				if(mac_p->tRecord[ uiIndex ].regdata == 0xFFFFFFFF){
				fprintf( outfp, "      [%03d]%s : Startup  %08X %08X %08X %08X LINK-%s\n",
					 usDispNo3,
					 cnvRRH_SYSTIME_TIME2Str( &(mac_p->tRecord[ uiIndex ].sysTime)),
					 mac_p->tRecord[ uiIndex ].regdata_info,
					 mac_p->tRecord[ uiIndex ].regdata_sfp,
					 mac_p->tRecord[ uiIndex ].regdata_his,
					 mac_p->tRecord[ uiIndex ].regdata_jcpll,
					dbg_macSts_linkupdown[mac_p->tRecord[ uiIndex ].linkupdown & D_RRH_ON]
					  );
				}
				else{
				fprintf( outfp, "      [%03d]%s : %08X %08X %08X %08X %08X LINK-%s\n",
					 usDispNo3,
					 cnvRRH_SYSTIME_TIME2Str( &(mac_p->tRecord[ uiIndex ].sysTime)),
					 mac_p->tRecord[ uiIndex ].regdata,
					 mac_p->tRecord[ uiIndex ].regdata_info,
					 mac_p->tRecord[ uiIndex ].regdata_sfp,
					 mac_p->tRecord[ uiIndex ].regdata_his,
					 mac_p->tRecord[ uiIndex ].regdata_jcpll,
					 dbg_macSts_linkupdown[mac_p->tRecord[ uiIndex ].linkupdown & D_RRH_ON]
					  );
				}
			}
			/* 出力位置更新	*/
			uiIndex = ((uiIndex+1) % D_RRH_LOG_ETHERR_RCD_NUM);
		}
				fprintf( outfp, "Port%d [No ]Time                               Sts      Info     SfpInfo  MacHis   JcPllalm LinkSts \n", usDispNo1+1);
	}
	return ;
}

/*!
 * @brief 関数機能概要:FH statusログ表示
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2018/9/6 FJT)Taniguchi
  */
VOID dbg_fhStatDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_FHSTAT_HIS*	his_p = NULL;
	UINT					uiIndex;
	USHORT					usDispNo1 = 0;
	USHORT					usDispIndex = 1;


	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_FH_STAT, (VOID **)&his_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}

	uiIndex = his_p->uiWriteIdx;
	for( usDispNo1=0; usDispNo1 < D_DU_FHSTATLOG_RCD_NUM ; usDispNo1++ )
	{
		if((his_p->tRecord[ uiIndex ].sysTime.year >= 2000 ) &&	(his_p->tRecord[ uiIndex ].sysTime.year <= 2099 )){

			fprintf( outfp, "-------------------------------------------------------------------------------------------------------------------\n");
			fprintf( outfp, "No.%02d:%s LINK-%s \n", usDispIndex, cnvRRH_SYSTIME_TIME2Str( &(his_p->tRecord[ uiIndex ].sysTime)) ,  dbg_macSts_linkupdown[his_p->tRecord[ uiIndex ].linkinfo & D_RRH_ON] );
			fprintf( outfp, "  RXUPLANE     :%08x RXMPLANE         :%08x RXSPLANE        :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[0],his_p->tRecord[ uiIndex ].regdata[1],his_p->tRecord[ uiIndex ].regdata[2]);
			fprintf( outfp, "  RXDAERR      :%08x RXETYPEERR       :%08x RXVERERR        :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[3],his_p->tRecord[ uiIndex ].regdata[4],his_p->tRecord[ uiIndex ].regdata[5]);
			fprintf( outfp, "  STATRXFRERR  :%08x STATRXSMALL      :%08x STATRXJABER     :%08x STATRXLARGE      :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[6],his_p->tRecord[ uiIndex ].regdata[7],his_p->tRecord[ uiIndex ].regdata[8],his_p->tRecord[ uiIndex ].regdata[9]);
			fprintf( outfp, "  STATRXOVRSIZE:%08x STATRXUDRSIZE    :%08x STATRXTOOLONG   :%08x STATRXFRAGMNT    :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[10],his_p->tRecord[ uiIndex ].regdata[11],his_p->tRecord[ uiIndex ].regdata[12],his_p->tRecord[ uiIndex ].regdata[13]);
			fprintf( outfp, "  STATRXBADCODE:%08x STATRXTOTALPACKET:%08x STATRXBADFCS    :%08x STATRXSTOMPEDFCS :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[14],his_p->tRecord[ uiIndex ].regdata[15],his_p->tRecord[ uiIndex ].regdata[16],his_p->tRecord[ uiIndex ].regdata[17]);
			fprintf( outfp, "  STATRXUNICAST:%08x STATRXMULTICAST  :%08x STATRXBROADCAST :%08x STATRXPAUSE      :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[18],his_p->tRecord[ uiIndex ].regdata[19],his_p->tRecord[ uiIndex ].regdata[20],his_p->tRecord[ uiIndex ].regdata[21]);
			fprintf( outfp, "  OVFFRAMES    :%08x AXIERRFRAMES     :%08x OVFFRAMEM       :%08x AXIERRFRAMEM     :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[22],his_p->tRecord[ uiIndex ].regdata[23],his_p->tRecord[ uiIndex ].regdata[24],his_p->tRecord[ uiIndex ].regdata[25]);
			fprintf( outfp, "  TXUPLANE     :%08x TXMPLANE         :%08x TXSPLANE        :%08x \n",
								his_p->tRecord[ uiIndex ].regdata[26],his_p->tRecord[ uiIndex ].regdata[27],his_p->tRecord[ uiIndex ].regdata[28]);
			fprintf( outfp, "  STATTXPACKETS:%08x STATTXBADFCS     :%08x STATTXFRAMEERROR:%08x \n",
								his_p->tRecord[ uiIndex ].regdata[29],his_p->tRecord[ uiIndex ].regdata[30],his_p->tRecord[ uiIndex ].regdata[31]);
			fprintf( outfp, "  FECCOR       :%08x FECCANTCOR       :%08x RSFECCOR        :%08x RSFECUNCOR       :%08x RSFECERR:%08x \n",
								his_p->tRecord[ uiIndex ].regdata[32],his_p->tRecord[ uiIndex ].regdata[33],his_p->tRecord[ uiIndex ].regdata[34],his_p->tRecord[ uiIndex ].regdata[35],his_p->tRecord[ uiIndex ].regdata[36]);

			usDispIndex++;
		}
		/* 出力位置更新	*/
		uiIndex = ((uiIndex+1) % D_DU_FHSTATLOG_RCD_NUM);
	}
	return ;
}

/*!
 * @brief 関数機能概要:MAC statusログクリア
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2018/9/6 FJT)Taniguchi
  */
VOID dbg_macStsClear(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_ETH_ERR_HIS*	mac_p;
	USHORT					usDispNo1 = 0;
	UINT					shmid;

	shmid = E_RRH_SHMID_APL_MAC_STAT_ETH0;
	for(usDispNo1 = 0 ; usDispNo1 < 1 ;usDispNo1++){
		if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( shmid, (VOID **)&mac_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
		}
		memset(mac_p,0,sizeof(T_RRH_LOG_ETH_ERR_HIS));
	}
	fprintf(outfp, "MAC status log clear\n");
	return;
}


/*!
 * @brief 関数機能概要:SV-Busカウンタ履歴ログ表示
 * @note  関数処理内容:カウンタ表示
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_SVBusDisp(int argc, char *argv[], FILE *outfp )
{
	USHORT					usStartIndex;
	USHORT					usAstIndex;
	USHORT					usDispNum;
	USHORT					usDispCnt;

	T_RRH_SVSTATECHG_HIS*	svStateChg_his;
	UINT					uiDispNum_inPara=0;
	INT						iRetCode;
	INT						iErrCode;
	UCHAR					logTypName[D_SYS_SVSTATE_MAX][4] = { "???", "IRQ", "SET", "CTL", "CLR"};

	/*********************************************************************/
	/* Get SV-Bus Hist log(from sharded memory)                          */
	/*********************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_SV_STATE_HISTORY, (VOID **)&svStateChg_his, &iErrCode )) != BPF_RU_IPCM_OK )
	{
		fprintf(outfp, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	/************************************************************************/
	/* SV-Bus履歴ログ表示数の決定											*/
	/* 指定なし：default数分表示											*/
	/* Max以上 : Maxでガード												*/
	/* 上記以外: 指定数分表示												*/
	/************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum_inPara = atoi(argv[1]);
	}

	if( uiDispNum_inPara == 0 )
	{
		usDispNum = 50;
	}
	else if( uiDispNum_inPara > D_RRH_SVLOG_MAX )
	{
		usDispNum = D_RRH_SVLOG_MAX;
	}
	else
	{
		usDispNum = uiDispNum_inPara;
	}
	/****************************************************************************************************************/
	/* AssertLogサイクリック確認																					*/
	/* 最終位置の年月日が0以外であればサイクリック有りとしてログ表示開始位置指定									*/
	/* 上記以外の場合は、指定ログ数と取得ログ数からログ表示開始位置指定												*/
	/****************************************************************************************************************/
	if( 0 != svStateChg_his->svStateRecord[D_RRH_SVLOG_MAX-1].sysTime.year )
	{
		usStartIndex = ((D_RRH_SVLOG_MAX + svStateChg_his->logIndex - usDispNum) % D_RRH_SVLOG_MAX);
	}
	else
	{
		if( usDispNum < svStateChg_his->logIndex )
		{
			usStartIndex = svStateChg_his->logIndex - usDispNum;
		}
		else
		{
			usStartIndex = 0;
			usDispNum = svStateChg_his->logIndex;
		}
	}

	if( 0 != usDispNum  )
	{
		fprintf( outfp, "YYYY/MM/DD hh:mm:ss.ms0(xxx);[TYP];Description[filename(ln)];\n");
		fprintf( outfp, "  [ SV01 ] [ SV02 ] [ SV03 ] [ SV04 ]-[ SV05 ] [ SV06 ] [ SV07 ] [ SV08 ];\n");
		fprintf( outfp, "  [ SV09 ] [ SV10 ] [ SV11 ] [ SV12 ]-[ SV13 ] [ SV14 ] [ SV15 ] [ SV16 ];\n");
		fprintf( outfp, "=======================================================================================\n");
		usAstIndex = usStartIndex;
		for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
		{
			if(svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType > D_SYS_SVSTATE_CLR)
			{
				svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType = D_SYS_SVSTATE_UNKOWN;
			}
			if(svStateChg_his->svStateRecord[usAstIndex].optStr[0] != 0)
			{
				fprintf( outfp, "%s;[%s];%-32s[%-31s(%d)];\n",
						cnvRRH_SYSTIME_TIME2Str( &svStateChg_his->svStateRecord[usAstIndex].sysTime),
						logTypName[svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType],
						svStateChg_his->svStateRecord[usAstIndex].optStr,
						svStateChg_his->svStateRecord[usAstIndex].ucFileName,
						svStateChg_his->svStateRecord[usAstIndex].uiLine );
			}
			else if(svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType == D_SYS_SVSTATE_IRQ)
			{
				fprintf( outfp, "%s;[%s];IRQ                             [%-31s(%d)];\n",
						cnvRRH_SYSTIME_TIME2Str( &svStateChg_his->svStateRecord[usAstIndex].sysTime),
						logTypName[svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType],
						svStateChg_his->svStateRecord[usAstIndex].ucFileName,
						svStateChg_his->svStateRecord[usAstIndex].uiLine );
			}
			else
			{
				fprintf( outfp, "%s;[%s];No message                      [%-31s(%d)];\n",
						cnvRRH_SYSTIME_TIME2Str( &svStateChg_his->svStateRecord[usAstIndex].sysTime),
						logTypName[svStateChg_his->svStateRecord[usAstIndex].svStateLog.chgType],
						svStateChg_his->svStateRecord[usAstIndex].ucFileName,
						svStateChg_his->svStateRecord[usAstIndex].uiLine );
			}

			fprintf( outfp, "  %08x %08x %08x %08x-%08x %08x %08x %08x;\n",
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[0] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[1] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[2] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[3] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[4] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[5] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[6] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[7] );
			fprintf( outfp, "  %08x %08x %08x %08x-%08x %08x %08x %08x;\n",
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[8] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[9] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[10] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[11] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[12] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[13] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[14] ,
					svStateChg_his->svStateRecord[usAstIndex].svStateLog.svState[15] );

			/* 出力位置更新	*/
			usAstIndex = ((usAstIndex+1) % D_RRH_SVLOG_MAX);
		}
	}

	return;
}


/*!
 * @brief 関数機能概要:Fronthaul接続情報表示
 * @note  関数処理内容:Fronthaul接続に使用したパラメータを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_FHinfDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_DU_INFORMATION_TBL		*du_info_tbl_adr;					/**< DU情報テーブル		*/
	T_RRH_NETCONF_INFO_TBL			*netconf_tbl;						/**< NETCONF情報テーブル*/

	INT								iRetCode;
	INT								iErrCode;
	INT								i, j;

	UCHAR							data, pos;
	USHORT							vlan_id = 0, index = 0;


	/************************************************************/
	/* DU情報テーブル 共有メモリアドレス取得					*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
						(VOID **)&du_info_tbl_adr,						/* 共有メモリアドレス	*/
						&iErrCode );									/* エラーコード			*/

	if( iRetCode != BPF_RU_IPCM_OK ) {

		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return ;
	}

	/************************************************************/
	/* NETCONFユーザー情報テーブル初期設定						*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,			/* 共有メモリ番号		*/
						(VOID **)&netconf_tbl,							/* 共有メモリアドレス	*/
						&iErrCode );									/* エラーコード			*/

	if( iRetCode != BPF_RU_IPCM_OK ) {

		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return ;
	}

	/************************************************************/
	/* EEPROM展開情報表示										*/
	/************************************************************/
	fprintf(outfp, "===== Fronthaul information for initial value    =====\n");
	fprintf(outfp, "mac address (fheth0)               -> %02x:%02x:%02x:%02x:%02x:%02x \n", 
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][0],
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][1],
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][2],
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][3],
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][4],
				   du_info_tbl_adr->eeprom_info.localMacAddress[0][5]);
	fprintf(outfp, "lowest-vlan-id                     -> %d \n", du_info_tbl_adr->eeprom_info.vid_search_info.lowVid);
	fprintf(outfp, "highest-vlan-id                    -> %d \n", du_info_tbl_adr->eeprom_info.vid_search_info.highVid);
	fprintf(outfp, "last-used-vlan-id                  -> %d \n", du_info_tbl_adr->eeprom_info.vid_search_info.lastUsedVid);
	fprintf(outfp, "now-search-vlan-id                 -> %d \n", du_info_tbl_adr->eeprom_info.vid_search_info.nowVid);

	for( i = 0; i < 512; i++ ) {

		data = du_info_tbl_adr->flash_info.seachable_vlan_id[i];
		pos  = 0x80;

		for( j = 0; j < 8; j++ ) {

			if( (data & pos) != pos ) {
				fprintf(outfp, "searchable-access-vlans[%04d]      -> %d \n", index, vlan_id);
				index++;
			}
			pos = pos >> 1;
			vlan_id++;
		}
	}
	fprintf(outfp, "\n");

	/************************************************************/
	/* Fronthaul情報表示 (DHCP Client)							*/
	/************************************************************/
	fprintf(outfp, "===== Fronthaul information [DHCP Client]        =====\n");
	if( du_info_tbl_adr->du_connection_info.mp.dhcpFlag == D_RRH_OFF ) {
		fprintf(outfp, "DHCP connect                       -> NG \n");
	} else {
		fprintf(outfp, "DHCP connect                       -> OK \n");
		fprintf(outfp, "\n");

		/********************************************************/
		/* Fronthaul情報表示 (M-Plane)							*/
		/********************************************************/
		fprintf(outfp, "===== Fronthaul information [M-Plane    ]        =====\n");
		if( du_info_tbl_adr->du_connection_info.mp.port_no == D_RRH_FHPORT0 ) {
			fprintf(outfp, "[m-plane  ] Port                   -> fheth0 \n");
		} else {
			fprintf(outfp, "[m-plane  ] Port                   -> fheth1 \n");
		}
		fprintf(outfp, "[m-plane  ] vlan-id                -> %d \n", du_info_tbl_adr->du_connection_info.mp.mac.vid);
		fprintf(outfp, "[m-plane  ] marking                -> %d \n", du_info_tbl_adr->du_connection_info.mp.mac.pcp);
		fprintf(outfp, "[m-plane  ] Local IP Addr          -> %d.%d.%d.%d \n", 
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_du & 0xFF000000) >> 24),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_du & 0x00FF0000) >> 16),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_du & 0x0000FF00) >>  8),
						 (du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_du & 0x000000FF));
		fprintf(outfp, "[m-plane  ] NETCONF Client IP Addr -> %d.%d.%d.%d \n", 
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_cu & 0xFF000000) >> 24),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_cu & 0x00FF0000) >> 16),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_cu & 0x0000FF00) >>  8),
						 (du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_cu & 0x000000FF));
//		fprintf(outfp, "[m-plane  ] NETCONF Client IP Addr2 -> %d.%d.%d.%d \n", 
//						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_nms & 0xFF000000) >> 24),
//						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_nms & 0x00FF0000) >> 16),
//						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_nms & 0x0000FF00) >>  8),
//						 (du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_nms & 0x000000FF));
		fprintf(outfp, "[m-plane  ] DHCP Server IP Addr    -> %d.%d.%d.%d \n", 
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_dhcps & 0xFF000000) >> 24),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_dhcps & 0x00FF0000) >> 16),
						((du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_dhcps & 0x0000FF00) >>  8),
						 (du_info_tbl_adr->du_connection_info.mp.ip.ip_addr_dhcps & 0x000000FF));
		fprintf(outfp, "[m-plane  ] Subnet Mask            -> %d.%d.%d.%d \n", 
						((du_info_tbl_adr->du_connection_info.mp.ip.subnet_mask & 0xFF000000) >> 24),
						((du_info_tbl_adr->du_connection_info.mp.ip.subnet_mask & 0x00FF0000) >> 16),
						((du_info_tbl_adr->du_connection_info.mp.ip.subnet_mask & 0x0000FF00) >>  8),
						 (du_info_tbl_adr->du_connection_info.mp.ip.subnet_mask & 0x000000FF));
		fprintf(outfp, "[m-plane  ] Default Gateway        -> %d.%d.%d.%d \n", 
						((du_info_tbl_adr->du_connection_info.mp.ip.default_gateway & 0xFF000000) >> 24),
						((du_info_tbl_adr->du_connection_info.mp.ip.default_gateway & 0x00FF0000) >> 16),
						((du_info_tbl_adr->du_connection_info.mp.ip.default_gateway & 0x0000FF00) >>  8),
						 (du_info_tbl_adr->du_connection_info.mp.ip.default_gateway & 0x000000FF));
		fprintf(outfp, "[m-plane  ] vendor-specific-option -> %s \n", du_info_tbl_adr->du_connection_info.mp.ip.vendor_specific_option);
	}
	fprintf(outfp, "\n");

	/************************************************************/
	/* Fronthaul情報表示 (U/C-Plane)							*/
	/************************************************************/
	fprintf(outfp, "===== Fronthaul information [U/C-Plane  ]        =====\n");
	fprintf(outfp, "[u/c-plane] Port                   -> fheth0 \n");
	if(du_info_tbl_adr->du_connection_info.ucp[0].umac.vid != 0xFFFF) {
		fprintf(outfp, "[u/c-plane] vlan-id                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[0].umac.vid);
		fprintf(outfp, "[u-plane  ] marking                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[0].umac.pcp);
		fprintf(outfp, "[c-plane  ] marking                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[0].cmac.pcp);
	} else {
		fprintf(outfp, "[u/c-plane] vlan-id                -> Not initialize \n");
		fprintf(outfp, "[u-plane  ] marking                -> %d (init Val) \n", du_info_tbl_adr->du_connection_info.ucp[0].umac.pcp);
		fprintf(outfp, "[c-plane  ] marking                -> %d (init Val) \n", du_info_tbl_adr->du_connection_info.ucp[0].cmac.pcp);
	}
	fprintf(outfp, "\n");

	fprintf(outfp, "[u/c-plane] Port                   -> fheth1 \n");
	if(du_info_tbl_adr->du_connection_info.ucp[1].umac.vid != 0xFFFF) {
		fprintf(outfp, "[u/c-plane] vlan-id                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[1].umac.vid);
		fprintf(outfp, "[u-plane  ] marking                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[1].umac.pcp);
		fprintf(outfp, "[c-plane  ] marking                -> %d \n", du_info_tbl_adr->du_connection_info.ucp[1].cmac.pcp);
	} else {
		fprintf(outfp, "[u/c-plane] vlan-id                -> Not initialize \n");
		fprintf(outfp, "[u-plane  ] marking                -> %d (init Val) \n", du_info_tbl_adr->du_connection_info.ucp[1].umac.pcp);
		fprintf(outfp, "[c-plane  ] marking                -> %d (init Val) \n", du_info_tbl_adr->du_connection_info.ucp[1].cmac.pcp);
	}
	fprintf(outfp, "\n");

	/************************************************************/
	/* Fronthaul情報表示 (NETCONF)								*/
	/************************************************************/
	fprintf(outfp, "===== NETCONF/SSH information [face count = %d]    =====\n", netconf_tbl->face_cnt);
	fprintf(outfp, "callhome-ssh-port                  -> %d \n", netconf_tbl->callhome_ssh_port);
	fprintf(outfp, "server-ssh-port                    -> %d \n", netconf_tbl->server_ssh_port);
	fprintf(outfp, "\n");

	fprintf(outfp, "number of NETCONF user account     -> %d \n", netconf_tbl->num_of_use);
	fprintf(outfp, "\n");

	for(i = 0; i < netconf_tbl->face_cnt; i++) {
		if(netconf_tbl->info[i].enabled == D_RRH_ON) {
			fprintf(outfp, "[info(%d)] status                   -> enable \n", i);
		} else {
			fprintf(outfp, "[info(%d)] status                   -> disable \n", i);
		}
		fprintf(outfp, "[info(%d)] user                     -> %s \n", i, netconf_tbl->info[i].user);
#if 0
		fprintf(outfp, "[info(%d)] password                 -> %s \n", i, netconf_tbl->info[i].password);
#endif
		fprintf(outfp, "\n");
	}
	fprintf(outfp, "\n");
	return;
}


static char*	dbg_PfminfDisp_antname[D_RRH_ANT_MAX] = {"A","B","C","D"};
static char*	dbg_PfmRginfDisp_name_com[D_RRH_PFM_NUM_OF_REG_COM] = 
{
	"RCVPKT    ",
	"ILGCMH    ",
	"IDINV     ",
	"SRPERR    ",
	"EXPRCVRB  ",
	"RBRCVLOSA0",
	"RBRCVLOSB0",
	"RBRCVLOSC0",
	"RBRCVLOSD0",
	"RBRCVLOSA1",
	"RBRCVLOSB1",
	"RBRCVLOSC1",
	"RBRCVLOSD1",
	"PMSNDUP   "
};

static char*	dbg_PfmRginfDisp_name_sub6[25] = 
{
	"ILGAPHA0DN",
	"ILGTIMA0DN",
	"TIMTEA0DN ",
	"TIMTLA0DN ",
	"ONTIMA0DN ",
	"SEQDUPA0DN",
	"SEQMISA0DN",
	"RCVFCA0DN ",
	"RCVUPA0DN ",
	"ILGAPHA0UN",
	"ILGTIMA0UN",
	"TIMTEA0UN ",
	"TIMTLA0UN ",
	"ONTIMA0UN ",
	"SEQDUPA0UN",
	"SEQMISA0UN",
	"RCVFCA0UN ",
	"ILGAPHA0UR",
	"ILGTIMA0UR",
	"TIMTEA0UR ",
	"TIMTLA0UR ",
	"ONTIMA0UR ",
	"SEQDUPA0UR",
	"SEQMISA0UR",
	"RCVFCA0UR "
};

static char*	dbg_PfmRginfDisp_name_mmw[34] = 
{
	"ILGAPHA0DN",
	"ILGTIMA0DN",
	"TIMTEA0DN ",
	"TIMTLA0DN ",
	"ONTIMA0DN ",
	"SEQDUPA0DN",
	"SEQMISA0DN",
	"RCVFCA0DN ",
	"RCVUPA0DN ",
	"ILGAPHA0UN",
	"ILGTIMA0UN",
	"TIMTEA0UN ",
	"TIMTLA0UN ",
	"ONTIMA0UN ",
	"SEQDUPA0UN",
	"SEQMISA0UN",
	"RCVFCA0UN ",
	"ILGAPHA0UR",
	"ILGTIMA0UR",
	"TIMTEA0UR ",
	"TIMTLA0UR ",
	"ONTIMA0UR ",
	"SEQDUPA0UR",
	"SEQMISA0UR",
	"RCVFCA0UR ",
	"ILGAPHA0DS",
	"ILGTIMA0DS",
	"TIMTEA0DS ",
	"TIMTLA0DS ",
	"ONTIMA0DS ",
	"SEQDUPA0DS",
	"SEQMISA0DS",
	"RCVFCA1DS ",
	"RCVUPA1DS "
};


/*!
 * @brief 関数機能概要:Fronthaul接続情報表示
 * @note  関数処理内容:Fronthaul接続に使用したパラメータを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_PfminfDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_PFM_PERFORMANCE_TBL		*pfmlog_tbl;						/**< PFM情報テーブル	*/

	INT								iRetCode;
	INT								iErrCode;
	INT								i,j,k;
	INT								num_of_loop;						/**< ループ回数			*/
	INT								index;
	UINT							carnum;
	UINT							antum;
	INT								optnum;
	INT								logcount;
	INT								logcount2;
	INT								logcount3;
	CHAR**							optname;
	INT								separate;

	/************************************************************/
	/* Performance情報テーブル 共有メモリアドレス取得			*/
	/************************************************************/
	/* Performance情報テーブル 共有メモリアドレス取得	*/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,			/* 共有メモリ番号		*/
						(VOID **)&pfmlog_tbl,							/* 共有メモリアドレス	*/
						&iErrCode);										/* 詳細NGコード			*/
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return ;
	}

	if(pfmlog_tbl->counter_peri > 0){
		if( pfmlog_tbl->counter_peri >= D_RRH_PFM_NUM_OF_INFO_PER){
			num_of_loop = D_RRH_PFM_NUM_OF_INFO_PER;
			index       = pfmlog_tbl->index_peri;
		}else{
			num_of_loop = pfmlog_tbl->index_peri;
			index       = 0;
		}
	
		if(pfmlog_tbl->devkind == D_RRH_EEP_DEV_KIND_5GDU_SUB6){
			optnum = 25;
			optname = &dbg_PfmRginfDisp_name_sub6[0];
		}else{
			optnum = 34;
			optname = &dbg_PfmRginfDisp_name_mmw[0];
		}
	
		fprintf(outfp, "Performance NO TIME/ RegAddr-Value [Last Fault Bit]:0x%08x\n",pfmlog_tbl->fault_peri);
		fprintf(outfp, "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		for( i = 0; i < num_of_loop; i++ ) {
			fprintf(outfp,"No:%02d %04d/%02d/%02d %02d:%02d:%02d \n   ", i+1,pfmlog_tbl->info_peri[index].time.year, 
															   pfmlog_tbl->info_peri[index].time.month, 
															   pfmlog_tbl->info_peri[index].time.day, 
															   pfmlog_tbl->info_peri[index].time.hour, 
															   pfmlog_tbl->info_peri[index].time.min, 
															   pfmlog_tbl->info_peri[index].time.sec);
			logcount = 0;
			logcount2 = 0;
			logcount3 = 0;
			separate = 0;
			for(j=0; j < D_RRH_PFM_NUM_OF_REG_ALL ; j++){
				if(pfmlog_tbl->info_peri[index].reg_data_addr[j] == 0){
					continue;
				}
				if(logcount < D_RRH_PFM_NUM_OF_REG_COM){
					fprintf(outfp, "%s   :%08x ",dbg_PfmRginfDisp_name_com[logcount],pfmlog_tbl->info_peri[index].reg_data[j]);
					if( ((j+1) % 8) == 0){
						fprintf(outfp,"\n   ");
					}
				}
				else{
					if(logcount == D_RRH_PFM_NUM_OF_REG_COM){
						fprintf(outfp,"\n   ");
					}
					fprintf(outfp, "%s-#%d:%08x ",optname[logcount2] , logcount3, pfmlog_tbl->info_peri[index].reg_data[j]);
					logcount2++;
					if(logcount2 >= optnum){
						fprintf(outfp,"\n   ");
						logcount2=0;
						logcount3++;
						separate = 0;
					}else{
						separate++;
					}
				}
				logcount++;
				if((( separate + 1) % 9) == 0){
				fprintf(outfp,"\n   ");
				separate = 0;
				}
			}
			fprintf(outfp,"\n");
			fprintf(outfp, "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			index = ((index + 1) % D_RRH_PFM_NUM_OF_INFO_PER);
		}
	}

	/************************************************************/
	/* Performance情報テーブル表示								*/
	/************************************************************/
	fprintf(outfp, " \n") ;
	fprintf(outfp, "=== index : 0x%08x / count : 0x%08x === \n", pfmlog_tbl->index, pfmlog_tbl->counter ) ;
	fprintf(outfp, " \n") ;

	if( pfmlog_tbl->counter == 0 ) {

		fprintf(outfp, "***** No data of Performance Information Table ***** \n") ;
		return ;

	} else if( pfmlog_tbl->counter >= D_RRH_PFM_NUM_OF_INFO ) {

		num_of_loop = D_RRH_PFM_NUM_OF_INFO;
		index       = pfmlog_tbl->index;

	} else {

		num_of_loop = pfmlog_tbl->index;
		index       = 0;
	}

	if(pfmlog_tbl->devkind == D_RRH_EEP_DEV_KIND_5GDU_SUB6){
		carnum = 1;
		antum = D_RRH_ANT_MAX;
	}else{
		carnum = D_RRH_CAR_MAX;
		antum = D_RRH_ANT_MAX_MMW;
	}


	if(num_of_loop > 0){

					fprintf(outfp, "TIME                FACT                    RCVPKT   ILGCMH   IDINV    SRPERR   EXPRCVRB RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS PMSNDUP  \n");
					fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	}

	for( i = 0; i < num_of_loop; i++ ) {
					fprintf(outfp, "%04d/%02d/%02d %02d:%02d:%02d fact:%d                  %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x \n", 
																   pfmlog_tbl->info[index].time.year, 
																   pfmlog_tbl->info[index].time.month, 
																   pfmlog_tbl->info[index].time.day, 
																   pfmlog_tbl->info[index].time.hour, 
																   pfmlog_tbl->info[index].time.min, 
																   pfmlog_tbl->info[index].time.sec, 
																   pfmlog_tbl->info[index].fact,
																   pfmlog_tbl->info[index].reg_data_com[0],
																   pfmlog_tbl->info[index].reg_data_com[1],
																   pfmlog_tbl->info[index].reg_data_com[2],
																   pfmlog_tbl->info[index].reg_data_com[3],
																   pfmlog_tbl->info[index].reg_data_com[4],
																   pfmlog_tbl->info[index].reg_data_com[5],
																   pfmlog_tbl->info[index].reg_data_com[6],
																   pfmlog_tbl->info[index].reg_data_com[7],
																   pfmlog_tbl->info[index].reg_data_com[8],
																   pfmlog_tbl->info[index].reg_data_com[9],
																   pfmlog_tbl->info[index].reg_data_com[10],
																   pfmlog_tbl->info[index].reg_data_com[11],
																   pfmlog_tbl->info[index].reg_data_com[12],
																   pfmlog_tbl->info[index].reg_data_com[13]
																   );

					fprintf(outfp, "                                            ILGAPHA  ILGTIMA  TIMTEA   TIMTLA   ONTIMA   SEQDUPA  SEQMISA  RCVFCA   RCVUPA\n");

		for(j = 0 ; j < carnum ; j++ ){
			for(k = 0 ; k < antum ; k++ ){
					fprintf(outfp, "                                  C#%d-%s-DLN %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", j,dbg_PfminfDisp_antname[k],
								 pfmlog_tbl->info[index].reg_data[j][k][0], 
								 pfmlog_tbl->info[index].reg_data[j][k][1], 
								 pfmlog_tbl->info[index].reg_data[j][k][2], 
								 pfmlog_tbl->info[index].reg_data[j][k][3], 
								 pfmlog_tbl->info[index].reg_data[j][k][4], 
								 pfmlog_tbl->info[index].reg_data[j][k][5], 
								 pfmlog_tbl->info[index].reg_data[j][k][6], 
								 pfmlog_tbl->info[index].reg_data[j][k][7], 
								 pfmlog_tbl->info[index].reg_data[j][k][8]);
								 
					fprintf(outfp, "                                        ULN %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", 
								 pfmlog_tbl->info[index].reg_data[j][k][9], 
								 pfmlog_tbl->info[index].reg_data[j][k][10], 
								 pfmlog_tbl->info[index].reg_data[j][k][11], 
								 pfmlog_tbl->info[index].reg_data[j][k][12], 
								 pfmlog_tbl->info[index].reg_data[j][k][13], 
								 pfmlog_tbl->info[index].reg_data[j][k][14], 
								 pfmlog_tbl->info[index].reg_data[j][k][15], 
								 pfmlog_tbl->info[index].reg_data[j][k][16], 
								 pfmlog_tbl->info[index].reg_data[j][k][17]);

					
					fprintf(outfp, "                                        ULR %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", 
								 pfmlog_tbl->info[index].reg_data[j][k][18], 
								 pfmlog_tbl->info[index].reg_data[j][k][19], 
								 pfmlog_tbl->info[index].reg_data[j][k][20], 
								 pfmlog_tbl->info[index].reg_data[j][k][21], 
								 pfmlog_tbl->info[index].reg_data[j][k][22], 
								 pfmlog_tbl->info[index].reg_data[j][k][23], 
								 pfmlog_tbl->info[index].reg_data[j][k][24], 
								 pfmlog_tbl->info[index].reg_data[j][k][25], 
								 pfmlog_tbl->info[index].reg_data[j][k][26]);

					if(pfmlog_tbl->devkind == D_RRH_EEP_DEV_KIND_5GDU_MMW){
					fprintf(outfp, "                                        DLS %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", 
								 pfmlog_tbl->info[index].reg_data[j][k][27], 
								 pfmlog_tbl->info[index].reg_data[j][k][28], 
								 pfmlog_tbl->info[index].reg_data[j][k][29], 
								 pfmlog_tbl->info[index].reg_data[j][k][30], 
								 pfmlog_tbl->info[index].reg_data[j][k][31], 
								 pfmlog_tbl->info[index].reg_data[j][k][32], 
								 pfmlog_tbl->info[index].reg_data[j][k][33], 
								 pfmlog_tbl->info[index].reg_data[j][k][34], 
								 pfmlog_tbl->info[index].reg_data[j][k][35]);

					}
			}
		}
		fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		if((i+1) < num_of_loop){
			fprintf(outfp, "TIME                FACT                    RCVPKT   ILGCMH   IDINV    SRPERR   EXPRCVRB RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS RBRCVLOS PMSNDUP  \n");
		}
		index = ((index + 1) % D_RRH_PFM_NUM_OF_INFO);
	}


	
	return;
}


/*!
 * @brief 関数機能概要:LBM/LBR管理情報表示
 * @note  関数処理内容:LBM/LBR送受信に使用するパラメータを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_LbmMngDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_LBM_MNGMENT_TBL			*lbmmng_tbl;						/**< LBM情報テーブル	*/

	INT								iRetCode;
	INT								iErrCode;
	INT								i;


	/************************************************************/
	/* LBM/LBR情報管理テーブル 共有メモリアドレス取得			*/
	/************************************************************/
	/* LBM/LBR情報管理テーブル 共有メモリアドレス取得	*/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_LBM_MNGMENT_TBL,				/* 共有メモリ番号		*/
						(VOID **)&lbmmng_tbl,							/* 共有メモリアドレス	*/
						&iErrCode);										/* 詳細NGコード			*/
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return ;
	}
	/************************************************************/
	/* LBM/LBR情報管理テーブル表示								*/
	/************************************************************/
	fprintf(outfp, " \n") ;

	fprintf(outfp, "[Port#0] Valid Flag = %d, sockFd = %08x \n", lbmmng_tbl->port_info[0].valid_flag,
																 lbmmng_tbl->port_info[0].sock);
	fprintf(outfp, "[Port#1] Valid Flag = %d, sockFd = %08x \n", lbmmng_tbl->port_info[1].valid_flag,
																 lbmmng_tbl->port_info[1].sock);

	fprintf(outfp, "=== num_of_use : %d  === \n", lbmmng_tbl->num_of_use ) ;
	fprintf(outfp, " \n") ;

	for( i = 0; i < 8; i++ ) {

		if( lbmmng_tbl->info[i].valid_flag == D_RRH_OFF ) {

			fprintf(outfp, "[%d] Valid Flag = Invalid \n", i ) ;
		} else {

			fprintf(outfp, "[%d] Valid Flag = Valid \n", i ) ;
		}
		fprintf(outfp, "[%d] MA_ID      = %s \n", i, lbmmng_tbl->info[i].ma_id ) ;
		fprintf(outfp, "[%d] MA_NAME    = %s \n", i, lbmmng_tbl->info[i].ma_name ) ;
		fprintf(outfp, "[%d] md-level   = %d \n", i, lbmmng_tbl->info[i].md_level ) ;
	}
	return;
}

/*!
 * @brief 関数機能概要:LBM/LBR管理情報表示
 * @note  関数処理内容:LBM/LBR送受信に使用するパラメータを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_LoopBackMessageSend0( int argc, char *argv[], FILE *outfp )
{
	INT								iRetCode = D_RRH_OK;

	iRetCode = dbg_LoopBackMessageSend(0, outfp);
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf( outfp, "Error: LBM send error (fheth0 = %08x)\n", iRetCode ) ;
	}
	return;
}

VOID dbg_LoopBackMessageSend1( int argc, char *argv[], FILE *outfp )
{
	INT								iRetCode = D_RRH_OK;

	iRetCode = dbg_LoopBackMessageSend(1, outfp);
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf( outfp, "Error: LBM send error (fheth1 = %08x)\n", iRetCode ) ;
	}
	return;
}

typedef struct {
	char				dst_mac_addr[6];				/* Destination address					*/
	char				src_mac_addr[6];				/* Source address						*/
	char				vlan_eth_type[2];				/* Ethertype (VLAN:0xxxxx)				*/
	char				vlan_tag[2];					/* vlan-tag								*/
	char				eth_type[2];					/* EtherType (CFM :0x8902)				*/
} T_LBM_HEADER;

typedef struct {
	char				version;						/* version (MA-Level:3bit + Ver:5bit)	*/
	char				OpCode;							/* OpCode (LBM=0x03/LBR=2)				*/
	char				Flag;							/* Flag (0固定)							*/
	char				TLVoffset;						/* TLV_Offset (4固定)					*/
	char				TransactionID[4];				/* TransactionID						*/
	char				PatternData_type;
	char				PatternData_len[2];
	char				PatternData_id[64];				/* パターンデータ						*/
	char				PatternData_name[64];			/* パターンデータ						*/
} T_LBM_PAYLOAD;

typedef struct {
	T_LBM_HEADER		head;							/* Etherヘッダ							*/
	T_LBM_PAYLOAD		body;							/* Payload								*/
} T_LBM_FRAME;
/* @} */


INT		dbg_LoopBackMessageSend(int port_no, FILE *outfp )
{
	T_RRH_LBM_MNGMENT_TBL			*lbmmng_tbl;						/**< LBM情報テーブル	*/
	T_RRH_DU_INFORMATION_TBL		*du_info_tbl;						/**< DU情報テーブル		*/

	T_LBM_FRAME						sndData;

	INT								iRetCode = D_RRH_OK;
	INT								iErrCode;
	INT								i;

	INT								sock;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;


	/************************************************************/
	/* LBM/LBR情報管理テーブル 共有メモリアドレス取得			*/
	/************************************************************/
	/* LBM/LBR情報管理テーブル 共有メモリアドレス取得	*/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_LBM_MNGMENT_TBL,				/* 共有メモリ番号		*/
						(VOID **)&lbmmng_tbl,							/* 共有メモリアドレス	*/
						&iErrCode);										/* 詳細NGコード			*/
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return	iRetCode;
	}

	/************************************************************/
	/* DU情報テーブル 共有メモリアドレス取得					*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
						(VOID **)&du_info_tbl,							/* 共有メモリアドレス	*/
						&iErrCode );									/* エラーコード			*/
	if( iRetCode != BPF_RU_IPCM_OK ) {
		fprintf(outfp, "Error: shared memory get error. (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		return	iRetCode;
	}

	/************************************************************/
	/* VLAN設定													*/
	/************************************************************/
	/* Socket Open	*/
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(0x8902))) < 0 ) {
		fprintf(outfp, "Error: socket open error (return:%08x, code:%08x)\n", sock, iErrCode) ;
		return sock;
	}
	/* Interface number Get	*/
	memset( &ifr, 0, sizeof(struct ifreq) );
	snprintf( ifr.ifr_name, sizeof(ifr.ifr_name), "fheth%d", port_no );
	if( (iRetCode = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		fprintf(outfp, "Error: ioctl (SIOCGIFINDEX) error (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		close( sock );
		return iRetCode;
	}
	/* ソケット情報設定 */
	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(0x8902);
	sll.sll_halen = ETHER_ADDR_LEN;
	/* bind	*/
	if( (iRetCode = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		fprintf(outfp, "Error: bind error (return:%08x, code:%08x)\n", iRetCode, iErrCode) ;
		close( sock );
		return iRetCode;
	}

	/************************************************************/
	/* LBM編集													*/
	/************************************************************/
	/* Frame格納領域初期化	*/
	memset( (UCHAR *)&sndData.head, 0, sizeof(T_LBM_HEADER) );

	/* MACヘッダ編集	*/
	memcpy( sndData.head.dst_mac_addr, &du_info_tbl->eeprom_info.localMacAddress[0][0], D_RRH_MAC_ADDR_SIZE );
	memcpy( sndData.head.src_mac_addr, &du_info_tbl->eeprom_info.localMacAddress[0][0], D_RRH_MAC_ADDR_SIZE );

	sndData.head.vlan_eth_type[0] = 0x81;
	sndData.head.vlan_eth_type[1] = 0x00;
	sndData.head.vlan_tag[0]      = (du_info_tbl->du_connection_info.ucp[port_no].umac.pcp << 5);
	sndData.head.vlan_tag[0]     |= (char)((du_info_tbl->du_connection_info.ucp[port_no].umac.vid & 0x0F00) >> 8);
	sndData.head.vlan_tag[1]      = (char)(du_info_tbl->du_connection_info.ucp[port_no].umac.vid & 0x00FF);
	sndData.head.eth_type[0] = 0x89;
	sndData.head.eth_type[1] = 0x02;

	for( i = 0; i < 8; i++ ) {
		if( lbmmng_tbl->info[i].valid_flag == D_RRH_OFF ) {
			continue;
		}
		/* Frame格納領域初期化	*/
		memset( (UCHAR *)&sndData.body, 0, sizeof(T_LBM_PAYLOAD) );

		/* Payload編集	*/
		sndData.body.version   = (lbmmng_tbl->info[i].md_level << 5);
		sndData.body.OpCode    = 3;
		sndData.body.Flag      = 0;
		sndData.body.TLVoffset = 4;
		sndData.body.PatternData_type   = 3;
		sndData.body.PatternData_len[0] = 0;
		sndData.body.PatternData_len[1] = 128;
		memcpy( sndData.body.PatternData_id,   lbmmng_tbl->info[i].ma_id,   64 );
		memcpy( sndData.body.PatternData_name, lbmmng_tbl->info[i].ma_name, 64 );

		/* LBM送信	*/
		if( (iRetCode = send(sock, (VOID *)&sndData, sizeof(T_LBM_FRAME), 0)) < 0) {
			fprintf(outfp, "Error: msg send error. (ma-level:%d, sock:%08x, return:%08x, errno:%08x)\n", 
					lbmmng_tbl->info[i].md_level, sock, iRetCode, errno) ;
		} else {
			fprintf(outfp, "LBM send : msg send OK! (port : fheth%d, ma-level:%d, sock:%08x, return:%08x, errno:%08x)\n", 
					port_no, lbmmng_tbl->info[i].md_level, sock, iRetCode, errno) ;
			iRetCode = 0;
		}
	}
	close( sock );
	return iRetCode;
}

/*!
 * @brief 関数機能概要:external IO制御履歴ログ表示
 * @note  関数処理内容:external IO制御履歴ログを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_eioLogDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_DU_EIO_SVCTRL_LOG*		pEioLog;
	INT								iRetCode;
	INT								iErrCode;
	UINT							uiDispNum=0;
	USHORT							usDispNum, usStartIdx, usEndIdx, usDispIdx, usDispCnt;
	
	/************************************************************/
	/* Get log pointer(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_EIO_SVCTRL_LOG_TBL, (VOID **)&pEioLog, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	/****************************************************************************************************************/
	/* ログ表示数の決定																						*/
	/* 指定なし: default数分表示																					*/
	/* Max以上 : Maxでガード																						*/
	/* 上記以外: 指定数分表示																						*/
	/****************************************************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum = atoi(argv[1]);
	}

	if( uiDispNum == 0 )
	{
		usDispNum = 50;
	}
	else if( uiDispNum > D_DU_EIOLOG_MAX )
	{
		usDispNum = D_DU_EIOLOG_MAX;
	}
	else
	{
		usDispNum = uiDispNum;
	}
	/*	LOGが一周している場合	*/
	if(pEioLog->logInfo[pEioLog->index].date.year != 0)
	{
		usStartIdx = (USHORT)((D_DU_EIOLOG_MAX + pEioLog->index - usDispNum) % D_DU_EIOLOG_MAX);
	}
	else
	{
		if( usDispNum < pEioLog->index )
		{
			usStartIdx = (USHORT)pEioLog->index - usDispNum;
		}
		else
		{
			usStartIdx = 0;
			usDispNum = pEioLog->index;
		}
	}
	if( 0 != usDispNum  )
	{
		usEndIdx = (USHORT)((pEioLog->index-1) % D_DU_EIOLOG_MAX);
		fprintf( outfp , "DateTime                       ;Message[Start=%d,End=%d(Num=%d)]\n", usStartIdx, usEndIdx, usDispNum );
		
		
		for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
		{
			usDispIdx = (USHORT)((usStartIdx + usDispCnt) % D_DU_EIOLOG_MAX);
			fprintf( outfp, "%s;%s[%d]\n",
					 cnvRRH_SYSTIME_TIME2Str( &pEioLog->logInfo[usDispIdx].date), pEioLog->logInfo[usDispIdx].message, usDispIdx );
		}
	}
	else
	{
		fprintf( outfp , "log empty:[Start=%d,End=%d(Num=%d)]\n", usStartIdx, pEioLog->index, uiDispNum);
	}
	fprintf(outfp, "\n");
	return;
}

/*!
 * @brief 関数機能概要:fan Speed表示
 * @note  関数処理内容:fan Speedを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_fanSpeedDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_DU_EIO_SVCTRL_MNG*		pEioMng;
	INT								iErrCode;
	USHORT							usDispNum;
	USHORT							usStartIdx, usDispIdx, usDispCnt, fanIdx, fanNo;
	
	/* 外部IO情報テーブル(共有メモリ)ポインタを初期化  */
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_EIO_SVCTRL_MNG_TBL, (VOID **)&pEioMng, &iErrCode);
	
	/************/
	/*	1hour	*/
	/************/
	/*	Indexが最後の場合	*/
	if(pEioMng->fanSpeedMon.fanMonIdxH >= 99)
	{
		usDispNum = 100;
		usStartIdx = 0;
	}
	/*	Indexの最後でなく、その次のカウンタが無効値(0xffff)の場合	*/
	else if(pEioMng->fanSpeedMon.fanMonH[(pEioMng->fanSpeedMon.fanMonIdxH+1)][0][0] == 0xffff)
	{
		usStartIdx = 0;
		usDispNum = pEioMng->fanSpeedMon.fanMonIdxH+1;
	}
	/*	ログが一周している場合	*/
	else
	{
		usDispNum = 100;
		usStartIdx = pEioMng->fanSpeedMon.fanMonIdxH+1;
	}
	fprintf( outfp , "Disp fan speed counter for 1hour\n");
	fprintf( outfp , "              ;   0[rpm]~;%4d[rpm]~;%4d[rpm]~;%4d[rpm]~;%4d[rpm]~;\n",
						 (D_DU_THRESHOLD_FANSPEED*30), ((D_DU_THRESHOLD_FANSPEED+15)*30), ((D_DU_THRESHOLD_FANSPEED+30)*30), ((D_DU_THRESHOLD_FANSPEED+45)*30));
	fprintf( outfp , " Indx ; FAN#X ;   count  ;   count  ;   count  ;   count  ;   count  ;\n");
	fprintf( outfp , " ====================================================================;\n");
	
	for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
	{
		usDispIdx = (USHORT)((usStartIdx + usDispCnt) % 100);
		
		/*	FAN#1~3まで繰り返す	*/
		for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
		{
			fanNo = fanIdx+1;
			fprintf( outfp, "%5d; FAN#%d ; %8d ; %8d ; %8d ; %8d ; %8d ;\n",
					 usDispCnt, fanNo, pEioMng->fanSpeedMon.fanMonH[usDispIdx][fanIdx][0],
					 pEioMng->fanSpeedMon.fanMonH[usDispIdx][fanIdx][1],
					 pEioMng->fanSpeedMon.fanMonH[usDispIdx][fanIdx][2],
					 pEioMng->fanSpeedMon.fanMonH[usDispIdx][fanIdx][3],
					 pEioMng->fanSpeedMon.fanMonH[usDispIdx][fanIdx][4] );
		}
	}
	fprintf(outfp, "\n");
	fprintf(outfp, "\n");
	/************/
	/*	1min	*/
	/************/
	/*	Indexが最後の場合	*/
	if(pEioMng->fanSpeedMon.fanMonIdxM >= 99)
	{
		usDispNum = 100;
		usStartIdx = 0;
	}
	/*	Indexの最後でなく、その次のカウンタが無効値(0xffff)の場合	*/
	else if(pEioMng->fanSpeedMon.fanMonM[(pEioMng->fanSpeedMon.fanMonIdxM+1)][0][0] == 0xffff)
	{
		usStartIdx = 0;
		usDispNum = pEioMng->fanSpeedMon.fanMonIdxM+1;
	}
	/*	ログが一周している場合	*/
	else
	{
		usDispNum = 100;
		usStartIdx = pEioMng->fanSpeedMon.fanMonIdxM+1;
	}
	fprintf( outfp , "Disp fan speed counter for 1min\n");
	fprintf( outfp , "              ;   0[rpm]~;%4d[rpm]~;%4d[rpm]~;%4d[rpm]~;%4d[rpm]~;\n",
						 (D_DU_THRESHOLD_FANSPEED*30), ((D_DU_THRESHOLD_FANSPEED+15)*30), ((D_DU_THRESHOLD_FANSPEED+30)*30), ((D_DU_THRESHOLD_FANSPEED+45)*30));
	fprintf( outfp , " Indx ; FAN#X ;   count  ;   count  ;   count  ;   count  ;   count  ;\n");
	fprintf( outfp , " ====================================================================;\n");
	
	for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
	{
		usDispIdx = (USHORT)((usStartIdx + usDispCnt) % 100);
		
		/*	FAN#1~3まで繰り返す	*/
		for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
		{
			fanNo = fanIdx+1;
			fprintf( outfp, "%5d; FAN#%d ; %8d ; %8d ; %8d ; %8d ; %8d ;\n",
					 usDispCnt, fanNo, pEioMng->fanSpeedMon.fanMonM[usDispIdx][fanIdx][0],
					 pEioMng->fanSpeedMon.fanMonM[usDispIdx][fanIdx][1],
					 pEioMng->fanSpeedMon.fanMonM[usDispIdx][fanIdx][2],
					 pEioMng->fanSpeedMon.fanMonM[usDispIdx][fanIdx][3],
					 pEioMng->fanSpeedMon.fanMonM[usDispIdx][fanIdx][4] );
		}
	}
	fprintf(outfp, "\n");
	return;
}


/*!
 * @brief 関数機能概要:MT制御履歴ログ表示
 * @note  関数処理内容:MT制御履歴ログを表示する。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_mtLogDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_MT_INFO*					pMtLog;
	INT								iRetCode;
	INT								iErrCode;
	UINT							uiDispNum=0;
	USHORT							usDispNum, usStartIdx, usDispIdx, usDispCnt;
	UINT							index;
	
	/************************************************************/
	/* Get log pointer(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_MT_INFO, (VOID **)&pMtLog, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	index = pMtLog->logInfo.index % D_DU_MTLOG_RCD_NUM;
	/****************************************************************************************************************/
	/* ログ表示数の決定																						*/
	/* 指定なし: default数分表示																					*/
	/* Max以上 : Maxでガード																						*/
	/* 上記以外: 指定数分表示																						*/
	/****************************************************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum = atoi(argv[1]);
	}

	if( uiDispNum == 0 )
	{
		usDispNum = 50;
	}
	else if( uiDispNum > D_DU_MTLOG_RCD_NUM )
	{
		usDispNum = D_DU_MTLOG_RCD_NUM;
	}
	else
	{
		usDispNum = uiDispNum;
	}
	/*	LOGが一周している場合	*/
	if(pMtLog->logInfo.tRecord[index].date.year != 0)
	{
		usStartIdx = (USHORT)((D_DU_MTLOG_RCD_NUM + index - usDispNum) % D_DU_MTLOG_RCD_NUM);
	}
	else
	{
		if( usDispNum < index )
		{
			usStartIdx = (USHORT)index - usDispNum;
		}
		else
		{
			usStartIdx = 0;
			usDispNum = index;
		}
	}

	fprintf( outfp , "MT Information \n");
	fprintf( outfp , "MT conn  : %d(%d)\n",							pMtLog->comInfo.mt_con_state,pMtLog->comInfo.mt_con_count);
	fprintf( outfp , "CU conn  : %d \n",							pMtLog->comInfo.cu_con_state);
	fprintf( outfp , "Server   : Ip:0x%08x Port:%d \n",				pMtLog->comInfo.server.ipAddr,pMtLog->comInfo.server.portNo);
	fprintf( outfp , "         : %04d/%02d/%02d %02d:%02d:%02d:%02d0(%u) - %04d/%02d/%02d %02d:%02d:%02d:%02d0(%u)\n",
					pMtLog->comInfo.server.s_date.year,
					pMtLog->comInfo.server.s_date.month,
					pMtLog->comInfo.server.s_date.day,
					pMtLog->comInfo.server.s_date.hour,
					pMtLog->comInfo.server.s_date.min,
					pMtLog->comInfo.server.s_date.sec,
					pMtLog->comInfo.server.s_date.msec,
					pMtLog->comInfo.server.s_date.ucount,
					pMtLog->comInfo.server.e_date.year,
					pMtLog->comInfo.server.e_date.month,
					pMtLog->comInfo.server.e_date.day,
					pMtLog->comInfo.server.e_date.hour,
					pMtLog->comInfo.server.e_date.min,
					pMtLog->comInfo.server.e_date.sec,
					pMtLog->comInfo.server.e_date.msec,
					pMtLog->comInfo.server.e_date.ucount
					);
	fprintf( outfp , "LastCli  : Ip:0x%08x Port:%d \n",				pMtLog->comInfo.client.ipAddr,pMtLog->comInfo.client.portNo);
	fprintf( outfp , "         : %04d/%02d/%02d %02d:%02d:%02d:%02d0(%u) - %04d/%02d/%02d %02d:%02d:%02d:%02d0(%u)\n",
					pMtLog->comInfo.client.s_date.year,
					pMtLog->comInfo.client.s_date.month,
					pMtLog->comInfo.client.s_date.day,
					pMtLog->comInfo.client.s_date.hour,
					pMtLog->comInfo.client.s_date.min,
					pMtLog->comInfo.client.s_date.sec,
					pMtLog->comInfo.client.s_date.msec,
					pMtLog->comInfo.client.s_date.ucount,
					pMtLog->comInfo.client.e_date.year,
					pMtLog->comInfo.client.e_date.month,
					pMtLog->comInfo.client.e_date.day,
					pMtLog->comInfo.client.e_date.hour,
					pMtLog->comInfo.client.e_date.min,
					pMtLog->comInfo.client.e_date.sec,
					pMtLog->comInfo.client.e_date.msec,
					pMtLog->comInfo.client.e_date.ucount
					);
	fprintf( outfp , "BeamId   : %d \n",							pMtLog->comInfo.beamId);
	fprintf( outfp , "LocalBLK : %d \n",							pMtLog->comInfo.blk_ctl);
	fprintf( outfp , "SFPInfo  : %16s / %16s / %16s \n",			pMtLog->sfpInfo.sfpType,pMtLog->sfpInfo.vendorCode,pMtLog->sfpInfo.serialNo);

	fprintf( outfp , "TxCarrier: Ant-A %x %x %x %x\n",				pMtLog->carInfo_tx.carrierState[0][0] , pMtLog->carInfo_tx.carrierState[0][1] , pMtLog->carInfo_tx.carrierState[0][2], pMtLog->carInfo_tx.carrierState[0][3]);
	fprintf( outfp , "         : Ant-B %x %x %x %x\n",				pMtLog->carInfo_tx.carrierState[1][0] , pMtLog->carInfo_tx.carrierState[1][1] , pMtLog->carInfo_tx.carrierState[1][2], pMtLog->carInfo_tx.carrierState[1][3]);
	fprintf( outfp , "         : Ant-C %x %x %x %x\n",				pMtLog->carInfo_tx.carrierState[2][0] , pMtLog->carInfo_tx.carrierState[2][1] , pMtLog->carInfo_tx.carrierState[2][2], pMtLog->carInfo_tx.carrierState[2][3]);
	fprintf( outfp , "         : Ant-D %x %x %x %x\n",				pMtLog->carInfo_tx.carrierState[3][0] , pMtLog->carInfo_tx.carrierState[3][1] , pMtLog->carInfo_tx.carrierState[3][2], pMtLog->carInfo_tx.carrierState[3][3]);
	fprintf( outfp , "   Freq  : 0x%x 0x%x 0x%x 0x%x\n",			pMtLog->carInfo_tx.FreqNo[0]          , pMtLog->carInfo_tx.FreqNo[1]          , pMtLog->carInfo_tx.FreqNo[2]         , pMtLog->carInfo_tx.FreqNo[3]);
	fprintf( outfp , "   BW    : 0x%x 0x%x 0x%x 0x%x\n",			pMtLog->carInfo_tx.bandWidth[0]       , pMtLog->carInfo_tx.bandWidth[1]       , pMtLog->carInfo_tx.bandWidth[2]      , pMtLog->carInfo_tx.bandWidth[3]);

	fprintf( outfp , "RxCarrier: Ant-A %x %x %x %x\n",				pMtLog->carInfo_rx.carrierState[0][0] , pMtLog->carInfo_rx.carrierState[0][1] , pMtLog->carInfo_rx.carrierState[0][2], pMtLog->carInfo_rx.carrierState[0][3]);
	fprintf( outfp , "         : Ant-B %x %x %x %x\n",				pMtLog->carInfo_rx.carrierState[1][0] , pMtLog->carInfo_rx.carrierState[1][1] , pMtLog->carInfo_rx.carrierState[1][2], pMtLog->carInfo_rx.carrierState[1][3]);
	fprintf( outfp , "         : Ant-C %x %x %x %x\n",				pMtLog->carInfo_rx.carrierState[2][0] , pMtLog->carInfo_rx.carrierState[2][1] , pMtLog->carInfo_rx.carrierState[2][2], pMtLog->carInfo_rx.carrierState[2][3]);
	fprintf( outfp , "         : Ant-D %x %x %x %x\n",				pMtLog->carInfo_rx.carrierState[3][0] , pMtLog->carInfo_rx.carrierState[3][1] , pMtLog->carInfo_rx.carrierState[3][2], pMtLog->carInfo_rx.carrierState[3][3]);
	fprintf( outfp , "   Freq  : %x %x %x %x\n",					pMtLog->carInfo_rx.FreqNo[0]          , pMtLog->carInfo_rx.FreqNo[1]          , pMtLog->carInfo_rx.FreqNo[2]         , pMtLog->carInfo_rx.FreqNo[3]);
	fprintf( outfp , "   BW    : 0x%x 0x%x 0x%x 0x%x\n",			pMtLog->carInfo_rx.bandWidth[0]       , pMtLog->carInfo_rx.bandWidth[1]       , pMtLog->carInfo_rx.bandWidth[2]      , pMtLog->carInfo_rx.bandWidth[3]);
		
	if( 0 != usDispNum  )
	{
		fprintf( outfp , "DateTime                        ;Message\n");
		for( usDispCnt=0; usDispCnt < usDispNum; usDispCnt++ )
		{
			usDispIdx = (USHORT)((usStartIdx + usDispCnt) % D_DU_MTLOG_RCD_NUM);
			if((pMtLog->logInfo.tRecord[usDispIdx].date.year > 2099) || (pMtLog->logInfo.tRecord[usDispIdx].date.year < 2000)){
				continue;
			}
			fprintf( outfp, "%s;%s\n", cnvRRH_SYSTIME_TIME2Str( &(pMtLog->logInfo.tRecord[usDispIdx].date)), pMtLog->logInfo.tRecord[usDispIdx].message);
		}
	}
	else
	{
		fprintf( outfp , "log empty:[Start=%d,End=%d(Num=%d)]\n", usStartIdx, index, uiDispNum);
	}
	fprintf(outfp, "\n");
	return;
}


/*!
 * @brief 関数機能概要:tx power detectテーブル表示
 * @note  関数処理内容:tx power detectテーブル表示
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_txPowDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_TXPOW_DETECT_TBL*			pTxPow;
	T_RRH_TXPOW_DETECT_GAIN_IN*		pTxPow_in[D_DU_TXPOW_DETECT_MAX][E_DU_TXPOW_DETECT_VH_NUM];
	T_RRH_TXPOW_DETECT_GAIN_OUT*	pTxPow_out[E_DU_TXPOW_DETECT_VH_NUM];

	INT								iRetCode;
	INT								iErrCode;
	UINT							uiDispNum=0;
	UINT							uiDispNum2=0;
	CHAR*							vh_str[2] = {"V","H"};
	INT								iDispmode;
	CHAR*							valid_str[2] = {"INVALID","VALID"};
	char* 							endptr = "\0";
	INT								iValid;

	if( argc >= 2 ){
		iDispmode = strtol(argv[1], &endptr, 16);
		if((errno == ERANGE) || ( strlen(endptr) != 0 )){
			fprintf(stderr, "Error: input param(%s) error(code:%d)\n", argv[1],errno);
			fprintf(stderr, "Option \n");
			fprintf(stderr, " 0:all\n");
			fprintf(stderr, " 1:Current output Tx pow only\n");
			fprintf(stderr, " 2:flash only\n");
			fprintf(stderr, " 4:Z only\n");
			fprintf(stderr, " 8:Average only\n");
			fprintf(stderr, "10:Rate only\n");
			return;
		}
	}else{
		iDispmode = 0;
	}

	/************************************************************/
	/* Get log pointer(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_TXPOW_DETECT_TBL, (VOID **)&pTxPow, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}

	if(pTxPow->head.validFlg == D_RRH_OFF){
		fprintf(outfp, "Not available\n");
		return;
	}
	
	fprintf(outfp, "----------------------------------------------------------------------------------\n");
	fprintf(outfp, "TX Power Detect Informaiton \n");
	fprintf(outfp, "----------------------------------------------------------------------------------\n");
	fprintf(outfp, "PGC DATA Status         : %s\n",valid_str[pTxPow->head.pgclut_validFlg]);
	fprintf(outfp, "PGC DATA Header         : Ver:%02X%02X Date:%02X%02X %02X-%02X CRC:%02X%02X\n",
					pTxPow->head.pgclut_version[0],
					pTxPow->head.pgclut_version[1],
					pTxPow->head.pgclut_version[2],
					pTxPow->head.pgclut_version[3],
					pTxPow->head.pgclut_version[4],
					pTxPow->head.pgclut_version[5],
					pTxPow->head.pgclut_version[6],
					pTxPow->head.pgclut_version[7]);
	fprintf(outfp, "PGC TX POW DETECT START : %s \n",cnvRRH_SYSTIME_TIME2Str2( &(pTxPow->head.output_date_pre) ));
	fprintf(outfp, "                  END   : %s \n",    cnvRRH_SYSTIME_TIME2Str2( &(pTxPow->head.output_date) ));

	for( uiDispNum=0; uiDispNum < D_DU_TXPOW_DETECT_MAX ; uiDispNum++ ){
		for( uiDispNum2=0; uiDispNum2 < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum2++ ){
			pTxPow_in[uiDispNum][uiDispNum2] = &(pTxPow->gainIn[uiDispNum][uiDispNum2]);
		}
	}

	for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
		pTxPow_out[uiDispNum] = &(pTxPow->gainOut[uiDispNum]);
	}


	if((iDispmode == 0) || ((iDispmode & 2) != 0)){
				fprintf(outfp, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "Input Gain Val((1)FLASH) \n");
				fprintf(outfp, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "V/H   01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45\n");
				fprintf(outfp, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
			for( uiDispNum2=0; uiDispNum2 < D_DU_TXPOW_DETECT_VH_MAX ; uiDispNum2++ ){
				fprintf(outfp, "%s#%02d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",
					vh_str[uiDispNum],
					uiDispNum2+1,
					(UCHAR)(pTxPow_in[ 0][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 1][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 2][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 3][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 4][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 5][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 6][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 7][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 8][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[ 9][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[10][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[11][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[12][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[13][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[14][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[15][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[16][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[17][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[18][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[19][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[20][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[21][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[22][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[23][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[24][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[25][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[26][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[27][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[28][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[29][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[30][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[31][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[32][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[33][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[34][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[35][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[36][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[37][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[38][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[39][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[40][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[41][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[42][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[43][uiDispNum]->FLASH_VH[uiDispNum2]),
					(UCHAR)(pTxPow_in[44][uiDispNum]->FLASH_VH[uiDispNum2]));
			}
		}
	}

	if((iDispmode == 0) || ((iDispmode & 4) != 0)){
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "Input Gain Val((2)Z) \n");
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "V/H       01      02      03      04      05      06      07      08      09      10      11      12      13      14      15      16      17      18      19      20      21      22      23\n");
				fprintf(outfp, "          24      25      26      27      28      29      30      31      32      33      34      35      36      37      38      39      40      41      42      43      44      45\n");
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
			for( uiDispNum2=0; uiDispNum2 < D_DU_TXPOW_DETECT_VH_MAX ; uiDispNum2++ ){
				fprintf(outfp, "%s#%02d %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n"\
				               "     %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
					vh_str[uiDispNum],
					uiDispNum2+1,
					pTxPow_in[ 0][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 1][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 2][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 3][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 4][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 5][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 6][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 7][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 8][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[ 9][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[10][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[11][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[12][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[13][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[14][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[15][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[16][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[17][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[18][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[19][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[20][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[21][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[22][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[23][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[24][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[25][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[26][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[27][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[28][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[29][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[30][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[31][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[32][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[33][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[34][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[35][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[36][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[37][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[38][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[39][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[40][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[41][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[42][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[43][uiDispNum]->YZ_VH[uiDispNum2],
					pTxPow_in[44][uiDispNum]->YZ_VH[uiDispNum2]);
			}
		}
	}
	
	if((iDispmode == 0) || ((iDispmode & 8) != 0)){
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "Input Gain Val((3)Average) \n");
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "V/H      01      02      03      04      05      06      07      08      09      10      11      12      13      14      15      16      17      18      19      20      21      22      23\n");
				fprintf(outfp, "         24      25      26      27      28      29      30      31      32      33      34      35      36      37      38      39      40      41      42      43      44      45\n");
				fprintf(outfp, "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
				fprintf(outfp, "%s   %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n"\
				               "    %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
					vh_str[uiDispNum],
					pTxPow_in[ 0][uiDispNum]->AVG_VH,
					pTxPow_in[ 1][uiDispNum]->AVG_VH,
					pTxPow_in[ 2][uiDispNum]->AVG_VH,
					pTxPow_in[ 3][uiDispNum]->AVG_VH,
					pTxPow_in[ 4][uiDispNum]->AVG_VH,
					pTxPow_in[ 5][uiDispNum]->AVG_VH,
					pTxPow_in[ 6][uiDispNum]->AVG_VH,
					pTxPow_in[ 7][uiDispNum]->AVG_VH,
					pTxPow_in[ 8][uiDispNum]->AVG_VH,
					pTxPow_in[ 9][uiDispNum]->AVG_VH,
					pTxPow_in[10][uiDispNum]->AVG_VH,
					pTxPow_in[11][uiDispNum]->AVG_VH,
					pTxPow_in[12][uiDispNum]->AVG_VH,
					pTxPow_in[13][uiDispNum]->AVG_VH,
					pTxPow_in[14][uiDispNum]->AVG_VH,
					pTxPow_in[15][uiDispNum]->AVG_VH,
					pTxPow_in[16][uiDispNum]->AVG_VH,
					pTxPow_in[17][uiDispNum]->AVG_VH,
					pTxPow_in[18][uiDispNum]->AVG_VH,
					pTxPow_in[19][uiDispNum]->AVG_VH,
					pTxPow_in[20][uiDispNum]->AVG_VH,
					pTxPow_in[21][uiDispNum]->AVG_VH,
					pTxPow_in[22][uiDispNum]->AVG_VH,
					pTxPow_in[23][uiDispNum]->AVG_VH,
					pTxPow_in[24][uiDispNum]->AVG_VH,
					pTxPow_in[25][uiDispNum]->AVG_VH,
					pTxPow_in[26][uiDispNum]->AVG_VH,
					pTxPow_in[27][uiDispNum]->AVG_VH,
					pTxPow_in[28][uiDispNum]->AVG_VH,
					pTxPow_in[29][uiDispNum]->AVG_VH,
					pTxPow_in[30][uiDispNum]->AVG_VH,
					pTxPow_in[31][uiDispNum]->AVG_VH,
					pTxPow_in[32][uiDispNum]->AVG_VH,
					pTxPow_in[33][uiDispNum]->AVG_VH,
					pTxPow_in[34][uiDispNum]->AVG_VH,
					pTxPow_in[35][uiDispNum]->AVG_VH,
					pTxPow_in[36][uiDispNum]->AVG_VH,
					pTxPow_in[37][uiDispNum]->AVG_VH,
					pTxPow_in[38][uiDispNum]->AVG_VH,
					pTxPow_in[39][uiDispNum]->AVG_VH,
					pTxPow_in[40][uiDispNum]->AVG_VH,
					pTxPow_in[41][uiDispNum]->AVG_VH,
					pTxPow_in[42][uiDispNum]->AVG_VH,
					pTxPow_in[43][uiDispNum]->AVG_VH,
					pTxPow_in[44][uiDispNum]->AVG_VH);
		}
	}
	
	if((iDispmode == 0) || ((iDispmode & 0x10) != 0)){
				fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "Input Gain Val((5)Rate) \n");
				fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "V/H       01       02       03       04       05       06       07       08       09       10       11       12       13       14       15       16       17       18       19       20       21       22       23\n");
				fprintf(outfp, "          24       25       26       27       28       29       30       31       32       33       34       35       36       37       38       39       40       41       42       43       44       45\n");
				fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
				fprintf(outfp, "%s   %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f\n"\
				               "    %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f %8.6f\n",
					vh_str[uiDispNum],
					pTxPow_in[ 0][uiDispNum]->RVS_VH,
					pTxPow_in[ 1][uiDispNum]->RVS_VH,
					pTxPow_in[ 2][uiDispNum]->RVS_VH,
					pTxPow_in[ 3][uiDispNum]->RVS_VH,
					pTxPow_in[ 4][uiDispNum]->RVS_VH,
					pTxPow_in[ 5][uiDispNum]->RVS_VH,
					pTxPow_in[ 6][uiDispNum]->RVS_VH,
					pTxPow_in[ 7][uiDispNum]->RVS_VH,
					pTxPow_in[ 8][uiDispNum]->RVS_VH,
					pTxPow_in[ 9][uiDispNum]->RVS_VH,
					pTxPow_in[10][uiDispNum]->RVS_VH,
					pTxPow_in[11][uiDispNum]->RVS_VH,
					pTxPow_in[12][uiDispNum]->RVS_VH,
					pTxPow_in[13][uiDispNum]->RVS_VH,
					pTxPow_in[14][uiDispNum]->RVS_VH,
					pTxPow_in[15][uiDispNum]->RVS_VH,
					pTxPow_in[16][uiDispNum]->RVS_VH,
					pTxPow_in[17][uiDispNum]->RVS_VH,
					pTxPow_in[18][uiDispNum]->RVS_VH,
					pTxPow_in[19][uiDispNum]->RVS_VH,
					pTxPow_in[20][uiDispNum]->RVS_VH,
					pTxPow_in[21][uiDispNum]->RVS_VH,
					pTxPow_in[22][uiDispNum]->RVS_VH,
					pTxPow_in[23][uiDispNum]->RVS_VH,
					pTxPow_in[24][uiDispNum]->RVS_VH,
					pTxPow_in[25][uiDispNum]->RVS_VH,
					pTxPow_in[26][uiDispNum]->RVS_VH,
					pTxPow_in[27][uiDispNum]->RVS_VH,
					pTxPow_in[28][uiDispNum]->RVS_VH,
					pTxPow_in[29][uiDispNum]->RVS_VH,
					pTxPow_in[30][uiDispNum]->RVS_VH,
					pTxPow_in[31][uiDispNum]->RVS_VH,
					pTxPow_in[32][uiDispNum]->RVS_VH,
					pTxPow_in[33][uiDispNum]->RVS_VH,
					pTxPow_in[34][uiDispNum]->RVS_VH,
					pTxPow_in[35][uiDispNum]->RVS_VH,
					pTxPow_in[36][uiDispNum]->RVS_VH,
					pTxPow_in[37][uiDispNum]->RVS_VH,
					pTxPow_in[38][uiDispNum]->RVS_VH,
					pTxPow_in[39][uiDispNum]->RVS_VH,
					pTxPow_in[40][uiDispNum]->RVS_VH,
					pTxPow_in[41][uiDispNum]->RVS_VH,
					pTxPow_in[42][uiDispNum]->RVS_VH,
					pTxPow_in[43][uiDispNum]->RVS_VH,
					pTxPow_in[44][uiDispNum]->RVS_VH);
		}
	}
	
	if((iDispmode == 0) || ((iDispmode & 0x1) != 0)){
				fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				fprintf(outfp, "Output Tx Pow Val((6)(7)) \n");
				fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		iValid = D_RRH_OFF;
		for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
				if(pTxPow_out[uiDispNum]->validFlg == D_RRH_OFF){
				fprintf(outfp, "%s Output data is invalid \n",vh_str[uiDispNum]);
					continue;
				}
				iValid = D_RRH_ON;
		}

		if(iValid == D_RRH_ON){
					fprintf(outfp, "V/H Cnt   01       02       03       04       05       06       07       08       09       10       11       12       13       14       15       16       17       18       19       20       21       22       23\n");
					fprintf(outfp, "          24       25       26       27       28       29       30       31       32       33       34       35       36       37       38       39       40       41       42       43       44       45\n");
					fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
					fprintf(outfp, "%s   %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n"\
					               "    %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						vh_str[uiDispNum],
						pTxPow_out[uiDispNum]->Reg_Cnt[ 0].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 1].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 2].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 3].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 4].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 5].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 6].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 7].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 8].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 9].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[10].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[11].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[12].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[13].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[14].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[15].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[16].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[17].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[18].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[19].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[20].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[21].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[22].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[23].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[24].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[25].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[26].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[27].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[28].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[29].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[30].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[31].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[32].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[33].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[34].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[35].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[36].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[37].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[38].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[39].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[40].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[41].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[42].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[43].regAddr,
						pTxPow_out[uiDispNum]->Reg_Cnt[44].regAddr);

					fprintf(outfp, "    %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n"\
					               "    %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						pTxPow_out[uiDispNum]->Reg_Cnt[ 0].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 1].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 2].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 3].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 4].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 5].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 6].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 7].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 8].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[ 9].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[10].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[11].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[12].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[13].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[14].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[15].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[16].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[17].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[18].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[19].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[20].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[21].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[22].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[23].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[24].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[25].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[26].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[27].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[28].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[29].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[30].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[31].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[32].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[33].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[34].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[35].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[36].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[37].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[38].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[39].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[40].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[41].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[42].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[43].regVal,
						pTxPow_out[uiDispNum]->Reg_Cnt[44].regVal);
			}
					fprintf(outfp, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					fprintf(outfp, "N*V/H      01        02        03        04        05        06        07        08        09        10        11        12        13        14        15        16        17        18        19        20        21        22        23\n");
					fprintf(outfp, "           24        25        26        27        28        29        30        31        32        33        34        35        36        37        38        39        40        41        42        43        44        45\n");
					fprintf(outfp, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){
					fprintf(outfp, "N*%s %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f\n"\
								   "    %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f \n",
										vh_str[uiDispNum],
										pTxPow_out[uiDispNum]->N_VAL[ 0],
										pTxPow_out[uiDispNum]->N_VAL[ 1],
										pTxPow_out[uiDispNum]->N_VAL[ 2],
										pTxPow_out[uiDispNum]->N_VAL[ 3],
										pTxPow_out[uiDispNum]->N_VAL[ 4],
										pTxPow_out[uiDispNum]->N_VAL[ 5],
										pTxPow_out[uiDispNum]->N_VAL[ 6],
										pTxPow_out[uiDispNum]->N_VAL[ 7],
										pTxPow_out[uiDispNum]->N_VAL[ 8],
										pTxPow_out[uiDispNum]->N_VAL[ 9],
										pTxPow_out[uiDispNum]->N_VAL[10],
										pTxPow_out[uiDispNum]->N_VAL[11],
										pTxPow_out[uiDispNum]->N_VAL[12],
										pTxPow_out[uiDispNum]->N_VAL[13],
										pTxPow_out[uiDispNum]->N_VAL[14],
										pTxPow_out[uiDispNum]->N_VAL[15],
										pTxPow_out[uiDispNum]->N_VAL[16],
										pTxPow_out[uiDispNum]->N_VAL[17],
										pTxPow_out[uiDispNum]->N_VAL[18],
										pTxPow_out[uiDispNum]->N_VAL[19],
										pTxPow_out[uiDispNum]->N_VAL[20],
										pTxPow_out[uiDispNum]->N_VAL[21],
										pTxPow_out[uiDispNum]->N_VAL[22],
										pTxPow_out[uiDispNum]->N_VAL[23],
										pTxPow_out[uiDispNum]->N_VAL[24],
										pTxPow_out[uiDispNum]->N_VAL[25],
										pTxPow_out[uiDispNum]->N_VAL[26],
										pTxPow_out[uiDispNum]->N_VAL[27],
										pTxPow_out[uiDispNum]->N_VAL[28],
										pTxPow_out[uiDispNum]->N_VAL[29],
										pTxPow_out[uiDispNum]->N_VAL[30],
										pTxPow_out[uiDispNum]->N_VAL[31],
										pTxPow_out[uiDispNum]->N_VAL[32],
										pTxPow_out[uiDispNum]->N_VAL[33],
										pTxPow_out[uiDispNum]->N_VAL[34],
										pTxPow_out[uiDispNum]->N_VAL[35],
										pTxPow_out[uiDispNum]->N_VAL[36],
										pTxPow_out[uiDispNum]->N_VAL[37],
										pTxPow_out[uiDispNum]->N_VAL[38],
										pTxPow_out[uiDispNum]->N_VAL[39],
										pTxPow_out[uiDispNum]->N_VAL[40],
										pTxPow_out[uiDispNum]->N_VAL[41],
										pTxPow_out[uiDispNum]->N_VAL[42],
										pTxPow_out[uiDispNum]->N_VAL[43],
										pTxPow_out[uiDispNum]->N_VAL[44]);
			}

					fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");
					fprintf(outfp, "TCB       01       02       03       04       05       06       07       08       09       10       11       12       13       14       15       16\n");
					fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for( uiDispNum=0; uiDispNum < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum++ ){

					fprintf(outfp, "%s   %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						vh_str[uiDispNum],
						pTxPow_out[uiDispNum]->Reg_Pow[ 0].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 1].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 2].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 3].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 4].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 5].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 6].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 7].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 8].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[ 9].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[10].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[11].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[12].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[13].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[14].regAddr,
						pTxPow_out[uiDispNum]->Reg_Pow[15].regAddr);


					fprintf(outfp, "    %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						pTxPow_out[uiDispNum]->Reg_Pow[ 0].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 1].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 2].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 3].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 4].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 5].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 6].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 7].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 8].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[ 9].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[10].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[11].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[12].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[13].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[14].regVal,
						pTxPow_out[uiDispNum]->Reg_Pow[15].regVal);


			}

			fprintf(outfp, "-----------------------------------------------------------------------------\n");
			fprintf(outfp, " P          V : %f(mW)	H : %f(mW)\n", pTxPow_out[0]->POW_VAL      ,pTxPow_out[1]->POW_VAL );
			fprintf(outfp, " K_*        V : %f	H : %f\n",     pTxPow_out[0]->K_VAL        ,pTxPow_out[1]->K_VAL );
			fprintf(outfp, " Average    V : %f(V)	H : %f(V) \n",  pTxPow_out[0]->TCB_AVG_VAL  ,pTxPow_out[1]->TCB_AVG_VAL );
			fprintf(outfp, " Ref Reg    V : 0x%08x	H : 0x%08x\n", pTxPow_out[0]->REF_VAL      ,pTxPow_out[1]->REF_VAL);
			fprintf(outfp, "-----------------------------------------------------------------------------\n");

		}

	}
	

	fprintf(outfp, "\n");
	return;
}



/*!
 * @brief 関数機能概要:tx power detect log表示
 * @note  関数処理内容:tx power detect log表示
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_txPowLogDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_TXPOW_DETECT_LOG*			pTxPowLog;
	INT								iRetCode;
	INT								iErrCode;
	UINT							uiDispNum=0;
	USHORT							usDispNum, usStartIdx, usDispIdx, usDispCnt;
	UINT							index;
	UINT							uiDispNum2;
	CHAR*							vh_str[E_DU_TXPOW_DETECT_VH_NUM] = {"V","H"};
	T_RRH_TXPOW_DETECT_GAIN_OUT*	pTxPow_out[E_DU_TXPOW_DETECT_VH_NUM];

	
	/************************************************************/
	/* Get log pointer(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_TXPOW_DETECT_LOG, (VOID **)&pTxPowLog, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	
	if(pTxPowLog->validFlg == D_RRH_OFF){
		fprintf(outfp, "Not available\n") ;
		return; 
	}
	
	index = pTxPowLog->index % D_DU_TXPOW_DETECT_HIS_RCD_NUM;
	/****************************************************************************************************************/
	/* ログ表示数の決定																						*/
	/* 指定なし: default数分表示																					*/
	/* Max以上 : Maxでガード																						*/
	/* 上記以外: 指定数分表示																						*/
	/****************************************************************************************************************/
	if( argc >= 2 )
	{
		uiDispNum = atoi(argv[1]);
	}

	if( uiDispNum == 0 )
	{
		usDispNum = 10;
	}
	else if( uiDispNum > D_DU_TXPOW_DETECT_HIS_RCD_NUM )
	{
		usDispNum = D_DU_TXPOW_DETECT_HIS_RCD_NUM;
	}
	else
	{
		usDispNum = uiDispNum;
	}
	/*	LOGが一周している場合	*/
	if(pTxPowLog->tRecord[index].date.year != 0)
	{
		usStartIdx = (USHORT)((D_DU_TXPOW_DETECT_HIS_RCD_NUM + index - usDispNum) % D_DU_TXPOW_DETECT_HIS_RCD_NUM);
	}
	else
	{
		if( usDispNum < index )
		{
			usStartIdx = (USHORT)index - usDispNum;
		}
		else
		{
			usStartIdx = 0;
			usDispNum = index;
		}
	}

	if( 0 != usDispNum  )
	{
		fprintf( outfp , "DateTime                        ;Message\n");
		for( usDispCnt=0; usDispCnt < usDispNum; usDispCnt++ )
		{
			usDispIdx = (USHORT)((usStartIdx + usDispCnt) % D_DU_TXPOW_DETECT_HIS_RCD_NUM);
			if(pTxPowLog->tRecord[usDispIdx].validFlg == D_RRH_OFF){
				continue;
			}
			fprintf( outfp, "%s;%s\n", cnvRRH_SYSTIME_TIME2Str( &(pTxPowLog->tRecord[usDispIdx].date)), pTxPowLog->tRecord[usDispIdx].message);
			if(pTxPowLog->tRecord[usDispIdx].gainOutValidFlg == D_RRH_OFF){
				continue;
			}

			pTxPow_out[0] = &(pTxPowLog->tRecord[usDispIdx].gainOut[0]);
			pTxPow_out[1] = &(pTxPowLog->tRecord[usDispIdx].gainOut[1]);

			fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fprintf(outfp, "V/H Cnt   01       02       03       04       05       06       07       08       09       10       11       12       13       14       15       16       17       18       19       20       21       22       23\n");
			fprintf(outfp, "          24       25       26       27       28       29       30       31       32       33       34       35       36       37       38       39       40       41       42       43       44       45\n");
			fprintf(outfp, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			for( uiDispNum2=0; uiDispNum2 < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum2++ ){
			fprintf(outfp, "%s   %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n"\
			               "    %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						vh_str[uiDispNum2],
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 0].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 1].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 2].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 3].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 4].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 5].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 6].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 7].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 8].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[ 9].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[10].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[11].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[12].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[13].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[14].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[15].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[16].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[17].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[18].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[19].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[20].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[21].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[22].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[23].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[24].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[25].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[26].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[27].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[28].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[29].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[30].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[31].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[32].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[33].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[34].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[35].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[36].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[37].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[38].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[39].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[40].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[41].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[42].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[43].regVal,
						pTxPow_out[uiDispNum2]->Reg_Cnt[44].regVal);
			}
			fprintf(outfp, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fprintf(outfp, "N*V/H      01        02        03        04        05        06        07        08        09        10        11        12        13        14        15        16        17        18        19        20        21        22        23\n");
			fprintf(outfp, "           24        25        26        27        28        29        30        31        32        33        34        35        36        37        38        39        40        41        42        43        44        45\n");
			fprintf(outfp, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			for( uiDispNum2=0; uiDispNum2 < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum2++ ){
					fprintf(outfp, "N*%s %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f\n"\
								   "    %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f \n",
										vh_str[uiDispNum2],
										pTxPow_out[uiDispNum2]->N_VAL[ 0],
										pTxPow_out[uiDispNum2]->N_VAL[ 1],
										pTxPow_out[uiDispNum2]->N_VAL[ 2],
										pTxPow_out[uiDispNum2]->N_VAL[ 3],
										pTxPow_out[uiDispNum2]->N_VAL[ 4],
										pTxPow_out[uiDispNum2]->N_VAL[ 5],
										pTxPow_out[uiDispNum2]->N_VAL[ 6],
										pTxPow_out[uiDispNum2]->N_VAL[ 7],
										pTxPow_out[uiDispNum2]->N_VAL[ 8],
										pTxPow_out[uiDispNum2]->N_VAL[ 9],
										pTxPow_out[uiDispNum2]->N_VAL[10],
										pTxPow_out[uiDispNum2]->N_VAL[11],
										pTxPow_out[uiDispNum2]->N_VAL[12],
										pTxPow_out[uiDispNum2]->N_VAL[13],
										pTxPow_out[uiDispNum2]->N_VAL[14],
										pTxPow_out[uiDispNum2]->N_VAL[15],
										pTxPow_out[uiDispNum2]->N_VAL[16],
										pTxPow_out[uiDispNum2]->N_VAL[17],
										pTxPow_out[uiDispNum2]->N_VAL[18],
										pTxPow_out[uiDispNum2]->N_VAL[19],
										pTxPow_out[uiDispNum2]->N_VAL[20],
										pTxPow_out[uiDispNum2]->N_VAL[21],
										pTxPow_out[uiDispNum2]->N_VAL[22],
										pTxPow_out[uiDispNum2]->N_VAL[23],
										pTxPow_out[uiDispNum2]->N_VAL[24],
										pTxPow_out[uiDispNum2]->N_VAL[25],
										pTxPow_out[uiDispNum2]->N_VAL[26],
										pTxPow_out[uiDispNum2]->N_VAL[27],
										pTxPow_out[uiDispNum2]->N_VAL[28],
										pTxPow_out[uiDispNum2]->N_VAL[29],
										pTxPow_out[uiDispNum2]->N_VAL[30],
										pTxPow_out[uiDispNum2]->N_VAL[31],
										pTxPow_out[uiDispNum2]->N_VAL[32],
										pTxPow_out[uiDispNum2]->N_VAL[33],
										pTxPow_out[uiDispNum2]->N_VAL[34],
										pTxPow_out[uiDispNum2]->N_VAL[35],
										pTxPow_out[uiDispNum2]->N_VAL[36],
										pTxPow_out[uiDispNum2]->N_VAL[37],
										pTxPow_out[uiDispNum2]->N_VAL[38],
										pTxPow_out[uiDispNum2]->N_VAL[39],
										pTxPow_out[uiDispNum2]->N_VAL[40],
										pTxPow_out[uiDispNum2]->N_VAL[41],
										pTxPow_out[uiDispNum2]->N_VAL[42],
										pTxPow_out[uiDispNum2]->N_VAL[43],
										pTxPow_out[uiDispNum2]->N_VAL[44]);
			}

			fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fprintf(outfp, "TCB       01       02       03       04       05       06       07       08       09       10       11       12       13       14       15       16\n");
			fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for( uiDispNum2=0; uiDispNum2 < E_DU_TXPOW_DETECT_VH_NUM ; uiDispNum2++ ){
					fprintf(outfp, "%s   %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
						vh_str[uiDispNum2],
						pTxPow_out[uiDispNum2]->Reg_Pow[ 0].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 1].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 2].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 3].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 4].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 5].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 6].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 7].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 8].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[ 9].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[10].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[11].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[12].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[13].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[14].regVal,
						pTxPow_out[uiDispNum2]->Reg_Pow[15].regVal);


			}

			fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fprintf(outfp, " P          V : %f(mW)	H : %f(mW)\n", pTxPow_out[0]->POW_VAL      ,pTxPow_out[1]->POW_VAL );
			fprintf(outfp, " K_*        V : %f	H : %f\n",     pTxPow_out[0]->K_VAL        ,pTxPow_out[1]->K_VAL );
			fprintf(outfp, " Average    V : %f(V)	H : %f(V) \n",  pTxPow_out[0]->TCB_AVG_VAL  ,pTxPow_out[1]->TCB_AVG_VAL );
			fprintf(outfp, " Ref Reg    V : 0x%08x	H : 0x%08x\n", pTxPow_out[0]->REF_VAL      ,pTxPow_out[1]->REF_VAL);
			fprintf(outfp, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");

		}

	}
	else
	{
		fprintf( outfp , "log empty:[Start=%d,End=%d(Num=%d)]\n", usStartIdx, index, uiDispNum);
	}
	fprintf(outfp, "\n");
	return;
}

/*!
 * @brief 関数機能概要:lut log表示
 * @note  関数処理内容:lut log表示
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 */
VOID dbg_lutLogDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_LUT_LOG*	plutLog;
	INT				iRetCode;
	INT				iErrCode;
	UINT			index;
	UINT			count;
	UINT			antCnt;
	CHAR*			antCnt_str[D_RRH_ANT_MAX] = {"A","B","C","D"};
	UCHAR			antCnt_index[D_RRH_ANT_MAX] = {0x1,0x5,0x9,0xD};
	UINT			logCnt1;
	UINT			logCnt2;
	UINT			logOfs;
	UINT			uiDispKind=0;


	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_LUT_LOG, (VOID **)&plutLog, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	if((plutLog->head.validFlg != D_RRH_ON) || (plutLog->head.index >= D_DU_LUT_LOG_MAX)){
		fprintf(outfp, "Not available\n");
		return;
	}

	if( argc >= 2 ){
		uiDispKind = atoi(argv[1]);
	}
	
	fprintf(outfp, "LAST Index : %d\n",plutLog->head.index);
	for(count=0 , index = plutLog->head.index;count < D_DU_LUT_LOG_MAX ;count++,index++){
		index %= D_DU_LUT_LOG_MAX;
		if(
			(plutLog->head.flash_head[index].flash_len == 0) || 
			(plutLog->head.flash_head[index].flash_len >= D_DU_QSPI_FLASH_SIZE_LUT_LOG_ONE )
		){
			continue;
		}
		fprintf(outfp, "[%d] flash_saving:0x%x flash_ofs:0x%x flash_len:0x%x flash_saved:0x%x \n",
					index,
					plutLog->head.flash_head[index].flash_saving,
					plutLog->head.flash_head[index].flash_ofs,
					plutLog->head.flash_head[index].flash_len,
					plutLog->head.flash_head[index].flash_saved
		);
	}

	fprintf(outfp, "LUT LOG Data\n");
	for(count=0 , index = plutLog->head.index;count < D_DU_LUT_LOG_MAX ;count++,index++){
		index %= D_DU_LUT_LOG_MAX;

		if( plutLog->head.flash_head[index].flash_saved == D_RRH_OFF){
			continue;
		}

		fprintf(outfp, "[%d] %s Fault ANT-%s SW Ver:0x%x \n",
					index,
					cnvRRH_SYSTIME_TIME2Str(&(plutLog->log[index].date)),
					antCnt_str[plutLog->log[index].antOfs & 0x3],
					plutLog->log[index].version
		);
		
		for(antCnt=0; antCnt < D_RRH_ANT_MAX ;antCnt++){
			fprintf(outfp, "ANT-%s B1200%XC0:0x%08x\n",antCnt_str[antCnt],antCnt_index[antCnt],plutLog->log[index].log_info[antCnt]);
			if(uiDispKind > 0){
				for(logCnt1=0;logCnt1 < 2 ; logCnt1++){
					logOfs = 0xB1220000;
					fprintf(outfp, "%08x ",logOfs);
					for(logCnt2=0;logCnt2 < 0x4000 ; logCnt2++){
						fprintf(outfp, "%08x ",plutLog->log[index].lut_data[antCnt][logCnt1][logCnt2]);
						if((((logCnt2 + 1) % 8) == 0) && ((logCnt2 + 1) < 0x4000 )){
							logOfs += 0x20;
							fprintf(outfp, "\n%08x ",logOfs);
						}
					}
					fprintf(outfp, "\n");
				}
			}
			fprintf(outfp, "----------------------------------------------------------------------------------\n");
		}
	}
	return;
}

/*!
 * @brief 関数機能概要:FH L1 HISログ表示
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2018/9/6 FJT)Taniguchi
  */
VOID dbg_fhl1hisLogDisp(int argc, char *argv[], FILE *outfp )
{
	INT						iErrCode;
	INT						iRetCode;
	T_RRH_LOG_FH_L1_HIS*	his_p;
	T_RRH_LOG_FH_L1_HIS_RECORD*	his_recd_p;
	T_RRH_LOG_FH_L1_HIS_RECORD*	his_recd_top_p;
	UINT					uiIndex;
	USHORT					usDispNo1 = 0;
	USHORT					usDispNo2 = 0;
	USHORT					usDispNo3 = 0;

	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_FH_L1_HIS, (VOID **)&his_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
	}

	for(usDispNo1 = 0; usDispNo1 < 2 ; usDispNo1++){
		if(usDispNo1 == 0){
			fprintf( outfp, "FH L1 HISTORY(LAST 10min) ----------------------------------------------------------\n");
			uiIndex = his_p->uiWriteIdx;
			his_recd_top_p = &(his_p->tRecord[ 0 ]);
		}
		else{
			fprintf( outfp, "FH L1 HISTORY(CHANGE) --------------------------------------------------------------\n");
			uiIndex = his_p->uiWriteIdx_chg;
			his_recd_top_p = &(his_p->tRecord_chg[ 0 ]);

		}
		if(uiIndex >= D_DU_LOG_FH_L1_HIS_RCD_NUM){
			fprintf(outfp, "Not available\n");
			continue;
		}
		usDispNo3 = 0;
		fprintf( outfp, "[No ]Time                               Version  MacHis   SfpInfo  JcPllalm LinkSts\n");
		for( usDispNo2=0; usDispNo2 < D_DU_LOG_FH_L1_HIS_RCD_NUM ; usDispNo2++ )
		{
			his_recd_p = his_recd_top_p + uiIndex;

			if(		(his_recd_p->sysTime.year >= 2000 )
				&&	(his_recd_p->sysTime.year <= 2099 )
				&&	(his_recd_p->version != 0 )
				 ){
				usDispNo3++;
				if(his_recd_p->link_state == 0xFFFFFFFF){
				fprintf( outfp, "[%03d]%s : %08X %08X %08X %08X Startup\n",
					 usDispNo3,
					 cnvRRH_SYSTIME_TIME2Str( &(his_recd_p->sysTime)),
					 his_recd_p->version,
					 his_recd_p->regdata[0],
					 his_recd_p->regdata[1],
					 his_recd_p->regdata[2]
					  );
				}
				else{
				fprintf( outfp, "[%03d]%s : %08X %08X %08X %08X LINK-%s\n",
					 usDispNo3,
					 cnvRRH_SYSTIME_TIME2Str( &(his_recd_p->sysTime)),
					 his_recd_p->version,
					 his_recd_p->regdata[0],
					 his_recd_p->regdata[1],
					 his_recd_p->regdata[2],
					 dbg_macSts_linkupdown[ his_recd_p->link_state & D_RRH_ON ]
					  );
				}
			}
			/* 出力位置更新	*/
			uiIndex = ((uiIndex+1) % D_DU_LOG_FH_L1_HIS_RCD_NUM);
		}
		fprintf( outfp, "[No ]Time                               Version  MacHis   SfpInfo  JcPllalm LinkSts\n");
	}
	return ;
}

#include "f_du_reg_ca.h"
#include "f_dpda_def.h"

extern const	UINT					f_dpdar_reg_Tbl[D_DPDA_REG_SUB6_NUM_MAX];
extern const	UINT					f_dpdar_reg_Tbl_mmW[D_DPDA_REG_MMW_NUM_MAX];

/*!
 * @brief 関数機能概要:主信号停波履歴
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2018/9/6 FJT)Taniguchi
  */
VOID dbg_upSigStopHisLogDisp(int argc, char *argv[], FILE *outfp )
{
	INT							iErrCode;
	INT							iRetCode;
	T_RRH_LOG_UPSIGSTOP_HIS*	his_p;
	T_RRH_LOG_UPSIGSTOP_HIS_RECORD*	his_recd_p;
	T_RRH_LOG_UPSIGSTOP_HIS_RECORD*	his_recd_top_p;
	UINT					uiIndex;
	UINT					uiAntCnt;
	UINT					uiAntMax;

	USHORT					usDispNo1 = 0;
	USHORT					usDispNo2 = 0;
	USHORT					usDispNo3 = 0;
	UINT					uiRegCnt	= 0;
	UINT					uiRegMaxCnt	= 0;
	UINT					*regTblArray;

	CHAR*			antCnt_str[D_RRH_ANT_MAX] = {"A","B","C","D"};


	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_UPSIGSTOP_HIS, (VOID **)&his_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
			fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
			return ;
	}

	if(his_p->uiWriteIdx >= D_DU_LOG_UPSIGSTOP_RCD_NUM){
		fprintf(outfp, "Not available(Index;%d)\n",his_p->uiWriteIdx);
		return ;
	}

	uiIndex = his_p->uiWriteIdx;

	if(his_p->devkind >= E_DPDA_REKIND_5GDU_NUM){
		fprintf(outfp, "Not available(dev;%d)\n",his_p->devkind);
		return ;
	}
	
	if(his_p->devkind == E_DPDA_REKIND_5GDU_MMW){
		uiAntMax = D_RRH_ANT_MAX_MMW;
		uiRegMaxCnt = D_DPDA_REG_MMW_NUM_MAX;
		regTblArray = (UINT *)f_dpdar_reg_Tbl_mmW;

	}else{
		uiAntMax = D_RRH_ANT_MAX;
		uiRegMaxCnt = D_DPDA_REG_SUB6_NUM_MAX;
		regTblArray = (UINT *)f_dpdar_reg_Tbl;

	}

	fprintf( outfp, "U-Plane Signal Stop HISTORY [Last Fault Bit]:0x%08x ----------------------------------------------------------\n",his_p->fault);
	fprintf( outfp, "[No ]Time\n");
	his_recd_top_p = &(his_p->tRecord[ 0 ]);
	usDispNo2 = 0;
	for( usDispNo1=0; usDispNo1 < D_DU_LOG_UPSIGSTOP_RCD_NUM ; usDispNo1++ ){
		his_recd_p = his_recd_top_p + uiIndex;
		if(	(his_recd_p->sysTime.year >= 2000 ) && (his_recd_p->sysTime.year <= 2099 ) && (his_recd_p->validCode == 0x11111111 ) ){
			usDispNo2++;
			fprintf( outfp, "[%03d]%s : Fault:%08x Sync:%08x LINK-%s\n",
				 usDispNo2,
				 cnvRRH_SYSTIME_TIME2Str( &(his_recd_p->sysTime)),
				 his_recd_p->sv_fault_state,
				 his_recd_p->sv_sync_state,
				 dbg_macSts_linkupdown[ his_recd_p->link_state & D_RRH_ON ]
				  );
			for( uiAntCnt=0; uiAntCnt < uiAntMax ; uiAntCnt++ ){
				fprintf( outfp, "                                  Ant-%s %08x %08x %08x %08x\n",
					antCnt_str[uiAntCnt],
					his_recd_p->tdd_en_state[uiAntCnt],
					his_recd_p->tddant_en_state[uiAntCnt],
					his_recd_p->txen_state[uiAntCnt],
					his_recd_p->rxen_state[uiAntCnt]
				);
			}
				fprintf( outfp,   "     REG Monitor                      : ");
			for( usDispNo3=0; usDispNo3 < D_DU_LOG_UPSIGSTOP_REG_NUM ; usDispNo3++ ){
				if(his_recd_p->regaddr[usDispNo3] == 0){
					break;
				}
				fprintf( outfp, "%08X-%08X ", his_recd_p->regaddr[usDispNo3],his_recd_p->regdata[usDispNo3]);
			}
				fprintf( outfp, "\n     RACH FFT FUNC Monitor            : ");
			for( usDispNo3=0; usDispNo3 < 12 ; usDispNo3++ ){
				fprintf( outfp, "%08X ", his_recd_p->rach_fft_mon[usDispNo3]);
			}
				fprintf( outfp, "\n     DLP Monitor                      : ");
			for( usDispNo3=0; usDispNo3 < 12 ; usDispNo3++ ){
				fprintf( outfp, "%08X ", his_recd_p->dlp_mon[usDispNo3]);
			}
				fprintf( outfp, "\n     ALM Monitor                      : ");
			for( usDispNo3=0; usDispNo3 < 12 ; usDispNo3++ ){
				fprintf( outfp, "%08X ", his_recd_p->alm_mon[usDispNo3]);
			}
				fprintf( outfp, "\n     HW auto PA control status        : %08X ON:%ucounts OFF:%ucounts\n", 
							his_recd_p->pa_status ,
							((his_recd_p->pa_control_cnt & 0xFFFF0000) >> 16) ,
							(his_recd_p->pa_control_cnt & 0x0000FFFF)
							);
				fprintf( outfp,   "     Ant REG                          : \n");

			for(uiRegCnt = 0 ; uiRegCnt < uiRegMaxCnt ; uiRegCnt++){
				if(D_DU_REG_CA_TXCAREN == regTblArray[uiRegCnt]){
					break;
				}
				fprintf( outfp, "%08X-%08X ",regTblArray[uiRegCnt], his_recd_p->ant_reg[uiRegCnt]);
				if( ((uiRegCnt + 1) % 8 ) == 0 ){
					fprintf( outfp, "\n");
				}
			}
			if( ( uiRegCnt % 8 ) > 0 ){
				fprintf( outfp, "\n");
			}
			fprintf( outfp, "\n");
		}
		/* 出力位置更新	*/
		uiIndex = ((uiIndex+1) % D_DU_LOG_UPSIGSTOP_RCD_NUM);
	}
	return ;
}

#include <float.h>

/*!
 * @brief 関数機能概要:CAL履歴
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2020/03/20 FJT)Taniguchi
  */
VOID dbg_calHisLogDisp(int argc, char *argv[], FILE *outfp )
{
	INT							iErrCode;
	INT							iRetCode;
	T_RRH_LOG_CAL_HIS*			his_p;
	T_RRH_LOG_CAL_HIS_INFO*		info_p;
	T_RRH_LOG_CAL_HIS_RECORD*	his_recd_p;
	T_RRH_LOG_CAL_HIS_RECORD_ONE*	recode_p;
	UINT					uiIndex;
	UINT					uiAntCnt;
	UINT					usDispNo1 = 0;
	UINT					usDispNo2 = 0;
	UINT					validFlg_tx[D_RRH_ANT_MAX];
	UINT					validFlg_tx_fast[D_RRH_ANT_MAX];
	UINT					validFlg_rx[D_RRH_ANT_MAX];
	UINT					validFlg_rx_fast[D_RRH_ANT_MAX];

	CHAR*			antCnt_str[D_RRH_ANT_MAX] = {"A","B","C","D"};

	DL				AMPREFSUM_TX_MAX[D_RRH_ANT_MAX];
	DL				AMPREFSUM_TX_MIN[D_RRH_ANT_MAX];
	DL				AMPREFSUM_TX_MAX_FAST[D_RRH_ANT_MAX];
	DL				AMPREFSUM_TX_MIN_FAST[D_RRH_ANT_MAX];
	
	DL				AMPFBSUM_TX_MAX[D_RRH_ANT_MAX];
	DL				AMPFBSUM_TX_MIN[D_RRH_ANT_MAX];
	DL				AMPFBSUM_TX_MAX_FAST[D_RRH_ANT_MAX];
	DL				AMPFBSUM_TX_MIN_FAST[D_RRH_ANT_MAX];

	DL				AMPREFSUM_RX_MAX[D_RRH_ANT_MAX];
	DL				AMPREFSUM_RX_MIN[D_RRH_ANT_MAX];
	DL				AMPREFSUM_RX_MAX_FAST[D_RRH_ANT_MAX];
	DL				AMPREFSUM_RX_MIN_FAST[D_RRH_ANT_MAX];

	DL				AMPFBSUM_RX_MAX[D_RRH_ANT_MAX];
	DL				AMPFBSUM_RX_MIN[D_RRH_ANT_MAX];
	DL				AMPFBSUM_RX_MAX_FAST[D_RRH_ANT_MAX];
	DL				AMPFBSUM_RX_MIN_FAST[D_RRH_ANT_MAX];

	CHAR*			cal_type[D_DU_LOG_CAL_TYPE_MAX] = {"OPE","FAST","OPE","FAST"};


	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_CAL_HIS, (VOID **)&his_p, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}

	if(his_p->validFlg != D_RRH_ON){
		fprintf(outfp, "Not available\n");
		return ;
	}

	for( uiAntCnt=0; uiAntCnt < D_RRH_ANT_MAX ; uiAntCnt++ ){
		AMPREFSUM_TX_MAX_FAST[uiAntCnt] = -DBL_MAX;
		AMPREFSUM_TX_MIN_FAST[uiAntCnt] = DBL_MAX;
		AMPFBSUM_TX_MAX_FAST[uiAntCnt] = -DBL_MAX;
		AMPFBSUM_TX_MIN_FAST[uiAntCnt] = DBL_MAX;


	fprintf( outfp, "ANT-%s TXCAL HISTORY START\n",antCnt_str[uiAntCnt]);

		his_recd_p = &(his_p->tRecord_tx[uiAntCnt]);

		info_p = &(his_p->info_tx[uiAntCnt]);

		uiIndex = info_p->fastIndex;
		fprintf( outfp, "[FAST-TX]ANT-%s TotalCnt:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->fastAllCnt,uiIndex);

		validFlg_tx_fast[uiAntCnt] = D_RRH_OFF;
		
		if(info_p->fastAllCnt > 0){
			for( usDispNo1=0 ,usDispNo2 = 0; usDispNo1 < D_DU_LOG_CAL_HIS_FAST_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_FAST_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->fast[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}
				fprintf( outfp, "[FAST-TX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/6.0,
					 recode_p->fbPower,
					 recode_p->fbPower/6.0
				);
				
				if(recode_p->refPower > AMPREFSUM_TX_MAX_FAST[uiAntCnt]){
					AMPREFSUM_TX_MAX_FAST[uiAntCnt] = recode_p->refPower;
				}
				if(recode_p->refPower < AMPREFSUM_TX_MIN_FAST[uiAntCnt]){
					AMPREFSUM_TX_MIN_FAST[uiAntCnt] = recode_p->refPower;
				}

				if(recode_p->fbPower > AMPFBSUM_TX_MAX_FAST[uiAntCnt]){
					AMPFBSUM_TX_MAX_FAST[uiAntCnt] = recode_p->fbPower;
				}
				if(recode_p->fbPower < AMPFBSUM_TX_MIN_FAST[uiAntCnt]){
					AMPFBSUM_TX_MIN_FAST[uiAntCnt] = recode_p->fbPower;
				}
				validFlg_tx_fast[uiAntCnt] = D_RRH_ON;
				usDispNo2++;
			}
		}
	fprintf( outfp, "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		AMPREFSUM_TX_MAX[uiAntCnt] = -DBL_MAX;
		AMPREFSUM_TX_MIN[uiAntCnt] = DBL_MAX;
		AMPFBSUM_TX_MAX[uiAntCnt] = -DBL_MAX;
		AMPFBSUM_TX_MIN[uiAntCnt] = DBL_MAX;

		uiIndex = info_p->slowIndex;
		fprintf( outfp, "[OPE-TX]ANT-%s TotalCnt:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->slowAllCnt,uiIndex);

		validFlg_tx[uiAntCnt] = D_RRH_OFF;
		if(info_p->slowAllCnt > 0){
			for( usDispNo1=0,usDispNo2 = 0;usDispNo1 < D_DU_LOG_CAL_HIS_SLOW_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_SLOW_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->slow[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}
				fprintf( outfp, "[OPE-TX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/10.0,
					 recode_p->fbPower,
					 recode_p->fbPower/10.0
				);


				if(recode_p->refPower > AMPREFSUM_TX_MAX[uiAntCnt]){
					AMPREFSUM_TX_MAX[uiAntCnt] = recode_p->refPower;
				}
				if(recode_p->refPower < AMPREFSUM_TX_MIN[uiAntCnt]){
					AMPREFSUM_TX_MIN[uiAntCnt] = recode_p->refPower;
				}

				if(recode_p->fbPower > AMPFBSUM_TX_MAX[uiAntCnt]){
					AMPFBSUM_TX_MAX[uiAntCnt] = recode_p->fbPower;
				}
				if(recode_p->fbPower < AMPFBSUM_TX_MIN[uiAntCnt]){
					AMPFBSUM_TX_MIN[uiAntCnt] = recode_p->fbPower;
				}

				validFlg_tx[uiAntCnt] = D_RRH_ON;
				usDispNo2++;

			}
		}

	fprintf( outfp, "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		uiIndex = info_p->errIndex;
		fprintf( outfp, "[ERR-TX]ANT-%s TotalFastErr:%ld TotalOpeErr:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->fastErrAllCnt,info_p->slowErrAllCnt,uiIndex);

		if((info_p->fastErrAllCnt > 0) || (info_p->slowErrAllCnt > 0)){
			for( usDispNo1=0,usDispNo2 = 0;usDispNo1 < D_DU_LOG_CAL_HIS_ERR_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_ERR_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->err[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}

				fprintf( outfp, "[ERR-TX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f) avgCnt:%d FreqNo:0x%x Type:%s\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/(DL)(recode_p->calCount),
					 recode_p->fbPower,
					 recode_p->fbPower/(DL)(recode_p->calCount),
					 recode_p->calCount,
					 recode_p->freqNo,
					 cal_type[recode_p->calType & 0x3 ]
				);

				usDispNo2++;

			}
		}

	fprintf( outfp, "ANT-%s TXCAL HISTORY END\n\n",antCnt_str[uiAntCnt]);
	fprintf( outfp, "ANT-%s RXCAL HISTORY START\n",antCnt_str[uiAntCnt]);


		AMPREFSUM_RX_MAX_FAST[uiAntCnt] = -DBL_MAX;
		AMPREFSUM_RX_MIN_FAST[uiAntCnt] = DBL_MAX;
		AMPFBSUM_RX_MAX_FAST[uiAntCnt] = -DBL_MAX;
		AMPFBSUM_RX_MIN_FAST[uiAntCnt] = DBL_MAX;

		his_recd_p = &(his_p->tRecord_rx[uiAntCnt]);

		info_p = &(his_p->info_rx[uiAntCnt]);

		uiIndex = info_p->fastIndex;
		fprintf( outfp, "[FAST-RX]ANT-%s TotalCnt:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->fastAllCnt,uiIndex);

		validFlg_rx_fast[uiAntCnt] = D_RRH_OFF;
		if(info_p->fastAllCnt > 0){
			for( usDispNo1=0,usDispNo2 = 0; usDispNo1 < D_DU_LOG_CAL_HIS_FAST_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_FAST_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->fast[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}
				fprintf( outfp, "[FAST-RX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/6.0,
					 recode_p->fbPower,
					 recode_p->fbPower/6.0
				);


				if(recode_p->refPower > AMPREFSUM_RX_MAX_FAST[uiAntCnt]){
					AMPREFSUM_RX_MAX_FAST[uiAntCnt] = recode_p->refPower;
				}
				if(recode_p->refPower < AMPREFSUM_RX_MIN_FAST[uiAntCnt]){
					AMPREFSUM_RX_MIN_FAST[uiAntCnt] = recode_p->refPower;
				}

				if(recode_p->fbPower > AMPFBSUM_RX_MAX_FAST[uiAntCnt]){
					AMPFBSUM_RX_MAX_FAST[uiAntCnt] = recode_p->fbPower;
				}
				if(recode_p->fbPower < AMPFBSUM_RX_MIN_FAST[uiAntCnt]){
					AMPFBSUM_RX_MIN_FAST[uiAntCnt] = recode_p->fbPower;
				}

				validFlg_rx_fast[uiAntCnt] = D_RRH_ON;
				usDispNo2++;

			}
		}

	fprintf( outfp, "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");


		AMPREFSUM_RX_MAX[uiAntCnt] = -DBL_MAX;
		AMPREFSUM_RX_MIN[uiAntCnt] = DBL_MAX;
		AMPFBSUM_RX_MAX[uiAntCnt] = -DBL_MAX;
		AMPFBSUM_RX_MIN[uiAntCnt] = DBL_MAX;

		uiIndex = info_p->slowIndex;
		fprintf( outfp, "[OPE-RX]ANT-%s TotalCnt:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->slowAllCnt,uiIndex);

		validFlg_rx[uiAntCnt] = D_RRH_OFF;
		if(info_p->slowAllCnt > 0){
			for( usDispNo1=0,usDispNo2 = 0; usDispNo1 < D_DU_LOG_CAL_HIS_SLOW_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_SLOW_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->slow[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}
				fprintf( outfp, "[OPE-RX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/10.0,
					 recode_p->fbPower,
					 recode_p->fbPower/10.0
				);

				if(recode_p->refPower > AMPREFSUM_RX_MAX[uiAntCnt]){
					AMPREFSUM_RX_MAX[uiAntCnt] = recode_p->refPower;
				}
				if(recode_p->refPower < AMPREFSUM_RX_MIN[uiAntCnt]){
					AMPREFSUM_RX_MIN[uiAntCnt] = recode_p->refPower;
				}

				if(recode_p->fbPower > AMPFBSUM_RX_MAX[uiAntCnt]){
					AMPFBSUM_RX_MAX[uiAntCnt] = recode_p->fbPower;
				}
				if(recode_p->fbPower < AMPFBSUM_RX_MIN[uiAntCnt]){
					AMPFBSUM_RX_MIN[uiAntCnt] = recode_p->fbPower;
				}

				validFlg_rx[uiAntCnt] = D_RRH_ON;
				usDispNo2++;
			}
		}


	fprintf( outfp, "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		uiIndex = info_p->errIndex;
		fprintf( outfp, "[ERR-RX]ANT-%s TotalFastErr:%ld TotalOpeErr:%ld Index:%u\n",antCnt_str[uiAntCnt],info_p->fastErrAllCnt,info_p->slowErrAllCnt,uiIndex);

		if((info_p->fastErrAllCnt > 0) || (info_p->slowErrAllCnt > 0)){

			for( usDispNo1=0,usDispNo2 = 0;usDispNo1 < D_DU_LOG_CAL_HIS_ERR_NUM ; usDispNo1++ ,uiIndex++){
				if(uiIndex >= D_DU_LOG_CAL_HIS_ERR_NUM){
					uiIndex = 0;
				}
				recode_p = &(his_recd_p->err[uiIndex]);
				if(recode_p->validCode != D_RRH_ON){
					continue;
				}

				fprintf( outfp, "[ERR-RX%03d]%s :ANT-%s AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f) avgCnt:%d FreqNo:0x%x Type:%s\n",
					 usDispNo2+1,
					 cnvRRH_SYSTIME_TIME2Str( &(recode_p->sysTime)),
					 antCnt_str[uiAntCnt],
					 recode_p->refPower,
					 recode_p->refPower/(DL)(recode_p->calCount),
					 recode_p->fbPower,
					 recode_p->fbPower/(DL)(recode_p->calCount),
					 recode_p->calCount,
					 recode_p->freqNo,
					 cal_type[recode_p->calType & 0x3 ]
				);

				usDispNo2++;

			}
		}

	fprintf( outfp, "ANT-%s RXCAL HISTORY END\n\n",antCnt_str[uiAntCnt]);
	}

	fprintf( outfp, "FAST CAL Summary --------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	fprintf( outfp, "TXCAL ---\n");
	for( uiAntCnt=0; uiAntCnt < D_RRH_ANT_MAX ; uiAntCnt++ ){
		if(validFlg_tx_fast[uiAntCnt] == D_RRH_ON){

	fprintf( outfp, "ANT-%s FAST-TX MAX AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_TX_MAX_FAST[uiAntCnt],AMPREFSUM_TX_MAX_FAST[uiAntCnt] / 6.0 ,
			AMPFBSUM_TX_MAX_FAST[uiAntCnt] ,AMPFBSUM_TX_MAX_FAST[uiAntCnt]  / 6.0);

	fprintf( outfp, "ANT-%s FAST-TX MIN AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_TX_MIN_FAST[uiAntCnt],AMPREFSUM_TX_MIN_FAST[uiAntCnt] / 6.0 ,
			AMPFBSUM_TX_MIN_FAST[uiAntCnt] ,AMPFBSUM_TX_MIN_FAST[uiAntCnt]  / 6.0);

	fprintf( outfp, "\n");

		}
	}

	fprintf( outfp, "RXCAL ---\n");
	for( uiAntCnt=0; uiAntCnt < D_RRH_ANT_MAX ; uiAntCnt++ ){
		if(validFlg_rx_fast[uiAntCnt] == D_RRH_ON){

	fprintf( outfp, "ANT-%s FAST-RX MAX AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_RX_MAX_FAST[uiAntCnt],AMPREFSUM_RX_MAX_FAST[uiAntCnt] / 6.0 ,
			AMPFBSUM_RX_MAX_FAST[uiAntCnt] ,AMPFBSUM_RX_MAX_FAST[uiAntCnt]  / 6.0);

	fprintf( outfp, "ANT-%s FAST-RX MIN AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_RX_MIN_FAST[uiAntCnt],AMPREFSUM_RX_MIN_FAST[uiAntCnt] / 6.0 ,
			AMPFBSUM_RX_MIN_FAST[uiAntCnt] ,AMPFBSUM_RX_MIN_FAST[uiAntCnt]  / 6.0);

	fprintf( outfp, "\n");

		}
	}

	fprintf( outfp, "OPE CAL Summary ---------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	fprintf( outfp, "TXCAL ---\n");

	for( uiAntCnt=0; uiAntCnt < D_RRH_ANT_MAX ; uiAntCnt++ ){
		if(validFlg_tx[uiAntCnt] == D_RRH_ON){

	fprintf( outfp, "ANT-%s  OPE-TX MAX AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_TX_MAX[uiAntCnt]     ,AMPREFSUM_TX_MAX[uiAntCnt] / 10.0 ,
			AMPFBSUM_TX_MAX[uiAntCnt]      ,AMPFBSUM_TX_MAX[uiAntCnt]  / 10.0);

	fprintf( outfp, "ANT-%s  OPE-TX MIN AMPREFSUM:%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_TX_MIN[uiAntCnt]     ,AMPREFSUM_TX_MIN[uiAntCnt] / 10.0 ,
			AMPFBSUM_TX_MIN[uiAntCnt]      ,AMPFBSUM_TX_MIN[uiAntCnt]  / 10.0);

	fprintf( outfp, "\n");

		}
	}

	fprintf( outfp, "RXCAL ---\n");
	for( uiAntCnt=0; uiAntCnt < D_RRH_ANT_MAX ; uiAntCnt++ ){
		if(validFlg_rx[uiAntCnt] == D_RRH_ON){

	fprintf( outfp, "ANT-%s  OPE-RX MAX AMPREFSUM%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_RX_MAX[uiAntCnt]     ,AMPREFSUM_RX_MAX[uiAntCnt] / 10.0,
			AMPFBSUM_RX_MAX[uiAntCnt]      ,AMPFBSUM_RX_MAX[uiAntCnt]  / 10.0);

	fprintf( outfp, "ANT-%s  OPE-RX MIN AMPREFSUM%f(avg:%f) AMPFBSUM:%f(avg:%f)\n",
			antCnt_str[uiAntCnt],
			AMPREFSUM_RX_MIN[uiAntCnt]     ,AMPREFSUM_RX_MIN[uiAntCnt] / 10.0,
			AMPFBSUM_RX_MIN[uiAntCnt]      ,AMPFBSUM_RX_MIN[uiAntCnt]  / 10.0);

	fprintf( outfp, "\n");

		}
	}

	return ;
}

/*!
 * @brief 関数機能概要:DLF過出力フェールセーフ履歴表示
 * @note  関数処理内容.
 *       -#
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @date 2020/07/09 FJT)Koshida
  */
VOID dbg_tOverFailSafeHisDisp(int argc, char *argv[], FILE *outfp )
{
	T_RRH_LOG_DLF_HIS		*pShm;
	UINT					uiStartIndex;
	UINT					uiDispNum;
	UINT					uiDispCnt;
	UINT					uiIndex;
	UINT					i,j;
	INT						iRetCode;
	INT						iErrCode;
	char					logName[D_RRH_POLARIZATION_NUM][8] = {"-45C","+45C"};

	/************************************************************/
	/* Get Assert log(from shared memory)						*/
	/************************************************************/
	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DLF_LOG, (VOID **)&pShm, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return ;
	}
	/*	ログが一周以上している場合	*/
	if(pShm->dlffin_flag == D_RRH_ON)
	{
		uiDispNum = D_DU_LOG_DLF_HIS_TIME;
		uiStartIndex = ((pShm->uiWriteIdx + 1) % D_DU_LOG_DLF_HIS_TIME);
	}
	/*	一周していない場合	*/
	else
	{
		uiDispNum = ((pShm->uiWriteIdx + 1) % D_DU_LOG_DLF_HIS_TIME);
		uiStartIndex = 0;
	}
	
	fprintf( outfp, "[1st]YYYY/MM/DD hh:mm:ss.ms0(xxx)-[last]YYYY/MM/DD hh:mm:ss.ms0(xxx);addCnt\n");
	fprintf( outfp, "  -45C;[  SPEC  ][SPEC+0.0][SPEC+0.1][SPEC+0.2][SPEC+0.3]/[SPEC+0.4][SPEC+0.5][SPEC+0.6][SPEC+0.7][SPEC+0.8]/[SPEC+0.9][SPEC+1.X][SPEC+2.X][SPEC+3.X][SPEC+X.X]/\n");
	fprintf( outfp, "  +45C;[  SPEC  ][SPEC+0.0][SPEC+0.1][SPEC+0.2][SPEC+0.3]/[SPEC+0.4][SPEC+0.5][SPEC+0.6][SPEC+0.7][SPEC+0.8]/[SPEC+0.9][SPEC+1.X][SPEC+2.X][SPEC+3.X][SPEC+X.X]/\n");
	fprintf( outfp, "================================================================================================================================================================\n");
	for(uiDispCnt=0; uiDispCnt<uiDispNum; uiDispCnt++)
	{
		uiIndex = ((uiStartIndex+uiDispCnt) % D_DU_LOG_DLF_HIS_TIME);
		/*	有効情報がない場合	*/
		if(pShm->dlf_info[uiIndex].add_time == 0)
		{
			/*	次Indexへ	*/
			continue;
		}
		fprintf( outfp, "[1st]:%s", cnvRRH_SYSTIME_TIME2Str( &pShm->dlf_info[uiIndex].s_date ) );
		fprintf( outfp, "-[last]:%s", cnvRRH_SYSTIME_TIME2Str( &pShm->dlf_info[uiIndex].e_date ) );
		fprintf( outfp, ";%2d\n", pShm->dlf_info[uiIndex].add_time );
		
		/*	偏波面数分繰り返し	*/
		for(i=0; i<D_RRH_POLARIZATION_NUM; i++)
		{
			fprintf( outfp, "  %s;", logName[i] );
			/*	過出力フェールセーフGrp数分繰り返し	*/
			for(j=0; j<D_DU_LOG_DLF_HIS_GRP_NUM; j++)
			{
					fprintf( outfp, " %08x ", pShm->dlf_info[uiIndex].regdat_sum[i][j] );
					/*	5回に1回マークを挿入	*/
					if((j % 5) == 4)
					{
							fprintf( outfp, "/" );
					}
			}
			fprintf( outfp, "\n" );
		}
	}
	
	return ;
}

