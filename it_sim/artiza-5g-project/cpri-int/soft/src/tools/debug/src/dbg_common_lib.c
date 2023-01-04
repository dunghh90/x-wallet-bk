/*!
 * @skip   $Id$
 * @file   dbg_common_lib.c
 * @brief  デバッグコマンド共通ライブラリ
 * @date   2013/11/25 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"
#include	<errno.h>
#include	"BPF_RM_SVRM.h"
extern	int	BPF_RM_SVRM_ASSIGN(unsigned int);
extern	int	BPF_RM_SVRM_UNASSIGN(unsigned int);
extern	int	dbg_common_terminate();

/*!
 * @brief 関数機能概要:デバッグコマンド共通初期化処理
 * @note  関数処理内容.
 *       -# デバッグコマンドにおける共通の初期化処理を行う
 *       -#  1) プロセス管理機能割付け
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval RTC_NG		1:異常終了
 * @date 2013/11/25 ALPHA)宮崎 create
 */
void dbg_common_sig_hdlr(int no_sig){

	printf("command terminated(%d)\n",no_sig);
	fflush(stdout);
	dbg_common_terminate();
	exit(EXIT_FAILURE);
}

void dbg_common_sig_entry( void ){
	signal(SIGSEGV,dbg_common_sig_hdlr);
	signal(SIGBUS,dbg_common_sig_hdlr);
	signal(SIGFPE,dbg_common_sig_hdlr);
	signal(SIGHUP,dbg_common_sig_hdlr);
	signal(SIGILL,dbg_common_sig_hdlr);
	signal(SIGINT ,dbg_common_sig_hdlr);
	signal(SIGTERM,dbg_common_sig_hdlr);
	signal(SIGABRT,dbg_common_sig_hdlr);
	signal(SIGTSTP,dbg_common_sig_hdlr);
	signal(SIGQUIT,dbg_common_sig_hdlr);
	signal(SIGPIPE,dbg_common_sig_hdlr);
	return;
}

/*!
 * @brief 関数機能概要:デバッグコマンド共通初期化処理
 * @note  関数処理内容.
 *       -# デバッグコマンドにおける共通の初期化処理を行う
 *       -#  1) プロセス管理機能割付け
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval RTC_NG		1:異常終了
 * @date 2013/11/25 ALPHA)宮崎 create
 */

int	dbg_common_init( int argc,	char *argv[]	)	{

	int rtn_bpf;

	/* プロセス管理機能割付け */
	if(( rtn_bpf = BPF_RM_SVRM_ASSIGN((unsigned int)D_RRH_PROCID_DBG) ) != BPF_RM_SVRM_COMPLETE )	{
		fprintf(stderr, "Initial error:%d\n", rtn_bpf);
		return(RTC_NG);
	}
	dbg_common_sig_entry();
	return RTC_OK	;
}
/*!
 * @brief 関数機能概要:デバッグコマンド共通終了処理
 * @note  関数処理内容.
 *       -# デバッグコマンドにおける共通の終了処理を行う
 *       -#  1) UNASSIGN
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval RTC_NG		1:異常終了
 * @date 2013/11/25 ALPHA)宮崎 create
 */

int	dbg_common_terminate()	{

	int bpf_rtn ;

	/* プロセス管理機能割付け解放 */
	if(( bpf_rtn = BPF_RM_SVRM_UNASSIGN((unsigned int)D_RRH_PROCID_DBG)) != BPF_RM_SVRM_COMPLETE ) {
		fprintf(stderr, "Error: Terminate error. (code:%d)\n", bpf_rtn )	;
		return RTC_NG;
	}

	return RTC_OK	;
}

/*!
 * @brief 関数機能概要:１６進文字列数値変換
 * @note  関数処理内容.
 *       -# １６進数文字列を数値へ変換する
 * @param  hexStr		[in]char*		１６進数文字列
 * @param  outVal		[out]unsigned int *	コマンドアーギュメント
 * @return INT
 * @retval RTC_		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */

int  cnvHexStr2Long( char	*hexStr, unsigned int *outVal )
{
	long long		longValue;
	char* 			endptr = "";

	/* １６進文字列変換 */
	longValue = -1;
	errno=0;
	if( hexStr == NULL )	{	/* 空ポインタを受けた	*/
		return RTC_NG;
	}
	if( strlen(hexStr) == 0 )	{	/* 空の文字を受けた	*/
		return RTC_NG;
	}
	longValue = strtoll( hexStr, &endptr, 16);	/* base:16	*/ /* pgr0539 */
	if(errno != 0) {	/* 変換エラー	*/
		return RTC_NG;
	}
	if( strlen(endptr) != 0 ){	/* 変換不能文字列あり	*/
		return RTC_NG;
	}
	*outVal = (int)(longValue & 0xffffffff) ;

	return RTC_OK;
}
/*!
 * @brief 関数機能概要:１０進文字列数値変換
 * @note  関数処理内容.
 *       -# １０進数文字列を数値へ変換する
 * @param  hexStr		[in]char*		１６進数文字列
 * @param  outVal		[out]unsigned int *	コマンドアーギュメント
 * @return INT
 * @retval RTC_		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */

int  cnvDecStr2Long( char	*decStr, unsigned int *outVal )
{
	long long		longValue;
	char* 			endptr = "\0";

	/* １６進文字列変換 */
	longValue = -1;
	longValue = strtoll( decStr, &endptr, 10);	/* base:10	*/ /* pgr0539 */
	if(errno != 0) {	/* 変換エラー	*/
		return RTC_NG;
	}
	if( strlen(endptr)!= 0 ){	/* 変換不能文字列あり	*/
		printf("[%s]\n", endptr );
		return RTC_NG;
	}
	*outVal = (int)(longValue & 0xffffffff) ;

	return RTC_OK;
}
/**
 * @brief レジスタ種別テーブル型定義
 * @note レジスタ種別テーブルの型を定義する
 */
typedef	struct	{
	unsigned int	startAdr		;	/**< 開始アドレス*/
	unsigned int	endAdr			;	/**< 終了アドレス	*/
	const char		*regTypeName	;	/**< レジスタ種別	*/
}	T_REG_TYPES	;

/*
 * @name レジスタ種別テーブル型
 * @note アドレス対レジスタ種別のテーブル
 *       ハソ改訂の度に合わせる 現在 v0.11 へ対応
 */

T_REG_TYPES	regTypeName[]	=	{
	/*  start       end         group					*/
	{	0x40000000,	0x40000FFF,	"APB0CT-CNTM"								},
	{	0x40020000,	0x40020FFF,	"GP0_QSPI"									},
	{	0x41000000,	0x41000FFF,	"AISG-REG"									},
	{	0x41010000,	0x41010FFF,	"AISG-MEM"									},
	{	0x80000000,	0x80000FFF,	"APBCNT-CNTS"								},
	{	0x80010000,	0x800107FF,	"CNT_DRP-CPRI COM"							},
	{	0x80010800,	0x80010FFF,	"CNT_DRP-CPRI Primary"						},
	{	0x80011000,	0x800117FF,	"CNT_DRP-CPRI Secondary"					},
	{	0x80011800,	0x80011FFF,	"CNT_DRP-Debug(JESD)"						},
	{	0x80800000,	0x80800FFF,	"PCIE-Slave Config"							},
	{	0x80F00000,	0x80F00FFF,	"CNT_SYSMON-SYSMON"							},
	{	0x81000000,	0x81000FFF,	"APBCP-CPRI Primary"						},
	{	0x81001000,	0x81001FFF,	"APBCP-CPRI Secondary"						},
	{	0x81002000,	0x810027FF,	"APBCP-Slow C&M Primary"					},
	{	0x81002800,	0x81002FFF,	"APBCP-Slow C&M Secondary"					},
	{	0x81003000,	0x810037FF,	"APBCP-U-Plane"								},
	{	0x81003800,	0x81003FFF,	"APBCP-TDD"									},
	{	0x81010000,	0x81013FFF,	"CPB_CMPTX-CM Tx Primary"					},
	{	0x81014000,	0x81017FFF,	"CPB_CMSTX-CM Tx Secondary"					},
	{	0x81100000,	0x8117FFFF,	"CPB_DTG-DTG"								},
	{	0x82000000,	0x820003FF,	"APBDPD-CC(Br#A)"							},
	{	0x82000400,	0x820007FF,	"APBDPD-CC(Br#B)"							},
	{	0x82000800,	0x82000BFF,	"APBDPD-CC(Br#C)"							},
	{	0x82000C00,	0x82000FFF,	"APBDPD-CC(Br#D)"							},
	{	0x82001000,	0x820013FF,	"APBDPD-PD(Br#A)"							},
	{	0x82001400,	0x820017FF,	"APBDPD-PD(Br#B)"							},
	{	0x82001800,	0x82001BFF,	"APBDPD-PD(Br#C)"							},
	{	0x82001C00,	0x82001FFF,	"APBDPD-PD(Br#D)"							},
	{	0x82002000,	0x820023FF,	"APBDPD-FB(Br#A)"							},
	{	0x82002400,	0x820027FF,	"APBDPD-FB(Br#B)"							},
	{	0x82002800,	0x82002BFF,	"APBDPD-FB(Br#C)"							},
	{	0x82002C00,	0x82002FFF,	"APBDPD-FB(Br#D)"							},
	{	0x82003000,	0x82003FFF,	"APBDPD-JSFW"								},
	{	0x82004000,	0x82004FFF,	"APBDPD-JSFB"								},
	{	0x82010000,	0x82012DFF,	"APBDPD-FB:FIL"								},
	{	0x82013000,	0x820133FF,	"APBDPD-FB:Mu "								},
	{	0x82018000,	0x8201BFFF,	"APBDPD-FB:CALC(B0)"						},
	{	0x8201C000,	0x8201FFFF,	"APBDPD-FB:CALC(B1)"						},
	{	0x82020000,	0x8202E3FF,	"APBDPD-PD:LUT"								},
	{	0x82030000,	0x8203FFFF,	"APBDPD-CC:Impulse"							},
	{	0x82100000,	0x821003FF,	"APBRX-RX(Br#A)"							},
	{	0x82100400,	0x821007FF,	"APBRX-RX(Br#B)"							},
	{	0x82100800,	0x82100BFF,	"APBRX-RX(Br#C)"							},
	{	0x82100C00,	0x82100FFF,	"APBRX-RX(Br#D)"							},
	{	0x82101000,	0x82101FFF,	"APBRX-JSRX"								},
	{	0x82200000,	0x8220FFFF,	"TXB_CAP-CAP"								},
	{	0x84000000,	0x8401FFFF,	"SCM_Rx-Slow C&M Memory(Primary)"			},
	{	0x84200000,	0x8403FFFF,	"SCM_Rx-Slow C&M Memory(Secondry)"			},
	{	0xB0000000,	0xB00FFFFF,	"PCIe_CFG-Config(Root)"						},
	{	0xB0100000,	0xB01FFFFF,	"PCIe_CFG-Config(EndPoint)"					},
	{	0xE0000000,	0xE0001FFF,	"I/O_Peripheral_UART"						},
	{	0xE0002000,	0xE0003FFF,	"I/O_Peripheral_USB"						},
	{	0xE0004000,	0xE0005FFF,	"I/O_Peripheral_I2C"						},
	{	0xE0006000,	0xE0007FFF,	"I/O_Peripheral_SPI"						},
	{	0xE0008000,	0xE0009FFF,	"I/O_Peripheral_CAN"						},
	{	0xE000A000,	0xE000AFFF,	"I/O_Peripheral_GPIO"						},
	{	0xE000B000,	0xE000CFFF,	"I/O_Peripheral_Ethernet"					},
	{	0xE000D000,	0xE000DFFF,	"I/O_Peripheral_Quad-SPI"					},
	{	0xE000E000,	0xE000EFFF,	"I/O_Peripheral_SMC"						},
	{	0xE0100000,	0xE0101FFF,	"I/O_Peripheral_SDIO"						},
	{	0xE1000000,	0xE5FFFFFF,	"SMC_MEM"									},
	{	0xF8000000,	0xF8000BFF,	"SLCR"										},
	{	0xF8001000,	0xF880FFFF,	"PS System"									},
	{	0xF8900000,	0xF8F02FFF,	"CPU Private"								},
	{	0xFC000000,	0xFDFFFFFF,	"Quad SPI"									},
	{	0xFFFC0000,	0xFFFFFFFF,	"OCM"										},
	{	0,			0,			"UNK"										}  /* terminator */
}	;

/*!
 * @brief 関数機能概要:レジスタ種別取得
 * @note  関数処理内容.
 *       -# レジスタのアドレスからレジスタ種別を取得する
 * @param  regAddr		[in]unsigned int 		レジスタアドレス
 * @return name 		[out]unsigned char  *	レジスタグループ名
 * @date 2013/11/24 ALPHA)宮崎 create
 */
const char	*getRegTypeName(unsigned int regAddr ) {
	int					regTypeIndex	;
	
	
	for(regTypeIndex = 0  ;; regTypeIndex++ ){
		if(  regTypeName[regTypeIndex].startAdr == 0 && regTypeName[regTypeIndex].endAdr == 0  )	{	/* reach terminator -> not found */
			return regTypeName[regTypeIndex].regTypeName  ;	/* not found = UN*/
		}
		if( regTypeName[regTypeIndex].startAdr	<= regAddr && regTypeName[regTypeIndex].endAdr	>= regAddr ) {
			return regTypeName[regTypeIndex].regTypeName ;	/* return registor type name */
		}
	}
}

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
 */
T_THCASH	threadCache[100]	;
int			threadCacheCnt	;


/**
 * @name アプリ定義スレッド名定義テーブル用型
 * @note アプリ定義のスレッドＩＤとスレッド名を紐付けるテーブルの型
 */
typedef	struct	{
	unsigned int	thread_id		;	/**< アプリ定義スレッドID		*/
	char			*thread_name	;	/**< スレッド名					*/
}	T_THREAD_DEFINE	;

/**
 * @name アプリ定義スレッド名テーブル
 * @note アプリ定義のスレッドＩＤとスレッド名を紐付ける
 */
T_THREAD_DEFINE		threadDefines[] = {
/** init process */
	{	D_SYS_THDID_INIT,			"init"			},	/**< INIT Main Thread								*/
	{	D_SYS_THDID_INIT_ALM,		"ini_alm"		},	/**< INIT ALM Thread								*/
	{	D_SYS_THDID_INIT_HLT,		"ini_hlt"		},	/**< INIT HLT Thread								*/
	{	D_SYS_THDID_INIT_RUN,		"ini_run"		},	/**< INIT RUN Thread								*/
/** pf process */
	{	D_SYS_THDID_PF_MAIN,		"pf_main"		},	/**< PF Main Thread							*/
	{	D_SYS_THDID_PF_RECV,		"pf_recv"		},	/**< PF Receive Thread(プロセス間受信部) CMU-RCV	*/
	{	D_SYS_THDID_PF_SEND,		"pf_send"		},	/**< PF Send Thread(プロセス間送信部) CMU-SEND		*/
	{	D_SYS_THDID_PF_DBG,			"pf_dbg"		},	/**< dbg Thread								*/
#if 0	/***** TODO	*****/
	{	D_SYS_THDID_PF_DHC,		"pf_dhcp"		},	/**< PF DHCP Thread							*/
	{	D_SYS_THDID_PF_SFP,			"pf_sfp"		},	/**< PF SFP Watch Thread					*/
	{	D_SYS_THDID_PTP_MAIN,		"ptp_main"		},	/**< PTP Main Thread						*/
	{	D_SYS_THDID_PF_HDLX,		"pf_hdl"		},	/**< handle Main Thread						*/
	{	D_SYS_THDID_PF_HDWDT,		"pf_hdwdt"		},	/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
	{	D_SYS_THDID_PF_HD1S,		"pf_hd1s"		},	/**< Handler Thread = IRQ_122:1s周期		*/
	{	D_SYS_THDID_PF_HD1PPS,		"pf_hd1pps"		},	/**< Handler Thread = IRQ_123:1pps Timing	*/
	{	D_SYS_THDID_PF_HDSV,		"pf_hdsv"		},	/**< Handler Thread = IRQ_124:SV状変		*/
	{	D_SYS_THDID_PF_HDMSTA1,		"pf_hdmsta1"	},	/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
	{	D_SYS_THDID_PF_HDMSTA2,		"pf_hdmsta2"	},	/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
	{	D_SYS_THDID_PF_RFA,		"pf_rfa"		},	/**< RFA Thread							*/
	{	D_SYS_THDID_PF_CCA,		"pf_cca"		},	/**< CCA thread								*/
#endif
	{	0,							""				}	/**< TERMINATER								*/
};

/*!
 * @brief 関数機能概要:スレッド名取得
 * @note  関数処理内容.
 *       -# スレッドIDからスレッド名を変換する
 *       -# スレッド名の取得には、 /proc/スレッドID/comm ファイルの内容を取得しスレッド名として返す
 *       -# ただし、ファイルアクセスを減らすため一度取得したスレッドの情報は保持し、同じスレッドIDで
 *       -# の検索があれば、ファイルからではなくキャッシュから取得し表示する。
 * @param  regAddr		[in]unsigned int 		レジスタアドレス
 * @return name 		[out]unsigned char  *	レジスタグループ名
 * @date 2013/11/24 ALPHA)宮崎 create
 */
char *getThreadName( unsigned short ttid ) {
	char 		comm_file_name[256] ;
	static char readbuf[80];
	int thind ;
	FILE *file;

	if( ttid == 0xffff || ttid == 0 ) {
		return (char *)"Unknown" ;
	}

	/* アプリ定義スレッド名テーブルからの検索	*/
	for( thind = 0 ; ; thind++ 	)	{
		if( threadDefines[thind].thread_id == 0 )	break;		/* 終端		*/
		if( threadDefines[thind].thread_id == ttid )	{	/* アプリ定義スレッドＩＤが一致	*/
			return threadDefines[thind].thread_name ;
		}
	}

	/* スレッドキャッシュからの検索	*/
	for( thind = 0 ; thind < threadCacheCnt ; thind++ ) {
		if( threadCache[thind].thid == ttid ) {
			return threadCache[thind].thname ;
		}
	}

	/* not found */
	/* プロセス情報からスレッド名を取得する		*/
	snprintf( comm_file_name, sizeof(comm_file_name), "/proc/%d/comm", ttid ) ;
	file = fopen( comm_file_name, "r");
	if( file == NULL ) {
		return (char *)"Unknown" ;
	}
	if( fgets(readbuf, sizeof(readbuf), file) == NULL ) {
		fclose(file);
		return (char *)"Unknown" ;
	}
	fclose(file);
	readbuf[strlen(readbuf)-1] = 0 ;

	if( threadCacheCnt < 100 ) {	/* スレッドキャッシュに余裕があれば	*/
		/* 新しいスレッド名を登録する	*/
		threadCache[threadCacheCnt].thid = ttid ;
		threadCache[threadCacheCnt].thname = strdup(readbuf) ;
		threadCacheCnt++;
	}
	return readbuf;
}

/*!
 * @brief 関数機能概要:日付出力フォーマット
 * @note  関数処理内容.
 *       -# 日付の文字列変換を行う
 * @param  tData		[in]T_RRH_SYSTEM_TIME 		変換元日付
 * @return      		[out]char  *				変換結果
 * @date 2013/12/25 ALPHA)宮崎 create
 */

char *cnvRRH_SYSTIME_TIME2Str(T_RRH_SYSTEM_TIME *tDate) {
	static char datestr[50] ;
	snprintf( datestr, sizeof(datestr),
				"%04d/%02d/%02d %02d:%02d:%02d:%02d0(%u)",
					tDate->year,
					tDate->month,
					tDate->day,
					tDate->hour,
					tDate->min,
					tDate->sec,
					tDate->msec,
					tDate->ucount) ;
	return datestr;
}

/*!
 * @brief 関数機能概要:日付出力フォーマット2
 * @note  関数処理内容.
 *       -# 日付の文字列変換を行う
 * @param  tData		[in]T_RRH_SYSTEM_TIME 		変換元日付
 * @return      		[out]char  *				変換結果
 * @date 2013/12/25 ALPHA)宮崎 create
 */

char *cnvRRH_SYSTIME_TIME2Str2(T_RRH_SYSTEM_TIME *tDate) {
	static char datestr[50] ;
	snprintf( datestr, sizeof(datestr),
				"%04d/%02d/%02d %02d:%02d:%02d:%02d0",
					tDate->year,
					tDate->month,
					tDate->day,
					tDate->hour,
					tDate->min,
					tDate->sec,
					tDate->msec);
	return datestr;
}


/*!
 * @brief 関数機能概要:AssertLogレコード出力
 * @note  関数処理内容.
 *       -# アサートログの１レコード分を文字列へ変換する
 * @param  astLogRec	[in]T_RRH_LOG_ASSERT_RECORD*	アサートログ１レコード分
 * @return outfp    	[in]FILE*						出力先ファイルポインタ
 * @date 2014/01/09 ALPHA)宮崎 create
 */

void assertLogPrint(T_RRH_LOG_ASSERT_RECORD *astLogRec, FILE *outfp )	{
	fprintf( outfp, "%s;%-s;%-10d;%.*s(%d);%.*s\n",
		cnvRRH_SYSTIME_TIME2Str( &astLogRec->tDate),
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_ENTER)?		"ENTER   ":
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_RETURN)?		"RETURN  ":
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_INFO)? 		"INFO    ":
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_WARNING)?		"WARNING ":
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_ERROR)? 		"ERROR   ":
		(astLogRec->uiLogLevel==D_RRH_LOG_AST_LV_CRITICAL)? 	"CRITICAL":
																"OTHER   ",
		astLogRec->iTid,
		(int)sizeof(astLogRec->ucFileName),
		astLogRec->ucFileName,
		astLogRec->uiLine,
		(int)sizeof(astLogRec->message),
		astLogRec->message
	) ;

}

/*!
 * @brief 関数機能概要:RegisterAccessLogレコード出力
 * @note  関数処理内容.
 *       -# レジスタアクセスログの１レコード分を文字列へ変換する
 * @param  astLogRec	[in]T_RRH_LOG_ASSERT_RECORD*	アサートログ１レコード分
 * @return outfp    	[in]FILE*						出力先ファイルポインタ
 * @date 2014/01/09 ALPHA)宮崎 create
 */
void regLogRecPrint( T_RRH_LOG_REG_ACCESS_RECORD* regLogRec,  FILE *outfp )		{

	fprintf( outfp,"%s;%s;%08x;%08x;%08x;[%s];%s(%d)\n",
		cnvRRH_SYSTIME_TIME2Str(&regLogRec->tDate ),
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_READ 			)? 	"READ      "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_READ_BUF 		)? 	"READ_BUF  "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_WRITE 			)? 	"WRITE     "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_WRITE_BUF 		)? 	"WRITE_BUF "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_WRITE_POL 		)? 	"WRITE_POL "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_READ_WRITE		)? 	"READ_WRITE"	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_READ_BUSY		)? 	"BUSY      "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_HDL_READ		)? 	"HDL_R     "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_HDL_WRITE		)? 	"HDL_W     "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_HDL_READ_BUF	)? 	"HDL_R_BUF "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_HDL_WRITE_BUF	)? 	"HDL_W_BUF "	:
		(regLogRec->loglevel == D_RRH_LOG_REG_LV_HDL_READ_WRITE	)? 	"HDL_RW    "	:
																	"UNKNOWN   ",
		regLogRec->addr,
		regLogRec->data1,
		regLogRec->data2,
		getRegTypeName(regLogRec->addr),
		getThreadName(regLogRec->tid),
		regLogRec->tid
	);

}

/* @} */

