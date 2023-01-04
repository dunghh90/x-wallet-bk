#include "f_dbg_cmd_inc.h"
#include  <ctype.h>


typedef struct {
	int		shared_key	;
	char	*desc		;
} T_SHM_SELECT;

T_SHM_SELECT shsel[E_RRH_SHMID_APL_MAX]	=		{
	{	E_RRH_SHMID_BPF_MNG,					"BPF_MNG"						},	/*!< 00 : 共有メモリ管理				*/
	{	E_RRH_SHMID_MNG_COM_MEMORY,				"MNG_COM_MEMORY"				},	/*!< 01 : 通信用共有メモリ管理			*/
	{	E_RRH_SHMID_LOGLEVEL_MNG,				"LOGLEVEL_MNG"					},	/*!< 02 : ログ取得レベル管理テーブル	*/
	{	E_RRH_SHMID_TIMER_MNG,					"TIMER_MNG"						},	/*!< 03 : タイマ管理テーブル			*/
	{	E_RRH_SHMID_APL_PROC_ID_MNG_INFO,		"APL_PROC_ID_MNG_INFO"			},	/*!< 04 : プロセスID管理テーブル		*/
	{	E_RRH_SHMID_APL_THRD_ID_MNG_INFO,		"APL_THRD_ID_MNG_INFO"			},	/*!< 05 : スレッドID管理テーブル		*/
	{	E_RRH_SHMID_APL_RST_REASON_LOG,			"APL_RST_REASON_LOG"			},	/*!< 06 : reset reason log				*/
	{	E_RRH_SHMID_APL_ASSERT_LOG,				"APL_ASSERT_LOG"				},	/*!< 07 : Assert Log					*/
	{	E_RRH_SHMID_APL_ALM_HISTORY_LOG,		"APL_ALM_HISTORY_LOG"			},	/*!< 08 : Alarm Hisotry Log				*/
	{	E_RRH_SHMID_APL_BUFFER_HISTORY,			"APL_BUFFER_HISTORY"			},	/*!< 09 : メッセージバッファ獲得履歴	*/
	{	E_RRH_SHMID_APL_TIMER_HISTORY,			"APL_TIMER_HISTORY"				},	/*!< 10 : タイマ履歴					*/
	{	E_RRH_SHMID_APL_SEMAPHORE_HISTORY,		"APL_SEMAPHORE_HISTORY"			},	/*!< 11 : セマフォ履歴					*/
	{	E_RRH_SHMID_APL_DPDA_ALM_LOG,			"APL_DPDA_ALM_LOG"				},	/*!< 12 : dpda alarm log				*/
	{	E_RRH_SHMID_APL_DPDA_MNG_TBL,			"APL_DPDA_MNG_TBL"				},	/*!< 13 : dpda情報管理テーブル			*/
	{	E_RRH_SHMID_APL_INVENTORY,				"APL_INVENTORY"					},	/*!< 14 : インベントリ情報				*/
	{	E_RRH_SHMID_APL_RRH_KIND,				"APL_RRH_KIND"					},	/*!< 15 : DU種別						*/
	{	E_RRH_SHMID_APL_ALM_HIS_PREPARE,		"APL_ALM_HIS_PREPARE"			},	/*!< 16 : Alarm History log収集準備Table*/
	{	E_RRH_SHMID_APL_TEMP_LOG_TBL,			"APL_TEMP_LOG_TBL"				},	/*!< 17 : Temprature Log Table			*/
	{	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS,		"APL_SFP0_MNG_INFO_HIS"			},	/*!< 18 : SFP#0管理情報履歴				*/
	{	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,		"APL_SFP1_MNG_INFO_HIS"			},	/*!< 19 : SFP#1管理情報履歴				*/
	{	E_RRH_SHMID_APL_CAL_HIS,				"APL_CAL_HIS"					},	/*!< 20 : CAL履歴						*/
	{	E_RRH_SHMID_APL_ALM_NTC_HIS,			"APL_ALM_NTC_HIS"				},	/*!< 21 : Alarm通知ログ					*/
	{	E_RRH_SHMID_APL_EQ_HIS,					"APL_EQ_HIS"					},	/*!< 22 : 装置状態ログ					*/
	{	E_RRH_SHMID_APL_ACTIVE_ALM_LIST,		"APL_ACTIVE_ALM_LIST"			},	/*!< 23 : Active-Alarm-list				*/
	{	E_RRH_SHMID_APL_CAR_STAT_HIS,			"APL_CAR_STAT_HIS"				},	/*!< 24 : キャリア状態ログ				*/
	{	E_RRH_SHMID_APL_PTP_CONF,				"APL_PTP_CONF"					},	/*!< 25 : PTP起動時パラメータ			*/
	{	E_RRH_SHMID_APL_PTP_STAT_HIS,			"APL_PTP_STAT_HIS"				},	/*!< 26 : PTP状態ログ					*/
	{	E_RRH_SHMID_APL_PTP_TIME_ADJ,			"APL_PTP_TIME_ADJ"				},	/*!< 27 : PTP時刻補正					*/
	{	E_RRH_SHMID_APL_UPSIGSTOP_HIS,			"APL_UPSIGSTOP_HIS"				},	/*!< 28 : 主信号停波履歴				*/
	{	E_RRH_SHMID_APL_FH_L1_HIS,				"APL_FH_L1_HIS"					},	/*!< 29 : FH L1履歴						*/
	{	E_RRH_SHMID_APL_SFNFRM_CALB_TBL,		"APL_SFNFRM_CALB_TBL"			},	/*!< 30 : SFN/FRM補正用テーブル			*/
	{	E_RRH_SHMID_APL_TXPOW_DETECT_TBL,		"APL_TXPOW_DETECT_TBL"			},	/*!< 31 : Tx power detector テーブル	*/
	{	E_RRH_SHMID_APL_TXPOW_DETECT_LOG,		"APL_TXPOW_DETECT_LOG"			},	/*!< 32 : Tx power detector LOG			*/
	{	E_RRH_SHMID_APL_LUT_LOG,				"APL_LUT_LOG"					},	/*!< 33 : LUT LOG						*/
	{	E_RRH_SHMID_APL_FH_STAT,				"APL_FH_STAT"					},	/*!< 34 : FH統計情報					*/
	{	E_RRH_SHMID_APL_MAC_STAT_ETH0,			"APL_MAC_STAT_ETH0"				},	/*!< 35 : Ethernet リンク障害履歴ログ(ETH#1)	*/
	{	E_RRH_SHMID_APL_MAC_STAT_ETH1,			"APL_MAC_STAT_ETH1"				},	/*!< 36 : Ethernet リンク障害履歴ログ(ETH#2)	*/
	{	E_RRH_SHMID_APL_PKT_STAT_MPLANE,		"APL_PKT_STAT_MPLANE"			},	/*!< 37 : ネットワーク統計(M-Plane)		*/
	{	E_RRH_SHMID_APL_SV_STATE_HISTORY,		"APL_SV_STATE_HISTORY"			},	/*!< 38 : SV状態変化履歴ログ			*/
	{	E_RRH_SHMID_APL_TRAINING_DATA,			"APL_TRAINING_DATA"				},	/*!< 39 : TPコマンド仮想メモリデータ	*/
	{	E_RRH_SHMID_APL_DU_INFORMATION_TBL,		"APL_DU_INFORMATION_TBL"		},	/*!< 40 : DU DHCP情報テーブル			*/
	{	E_RRH_SHMID_APL_TALISE_CONF_INFO,		"APL_TALISE_CONF_INFO"			},	/*!< 41 : RFIC Configデータ				*/
	{	E_RRH_SHMID_APL_DU_PROCESS_INFO,		"APL_DU_PROCESS_INFO"			},	/*!< 42 : DU information Table			*/
	{	E_RRH_SHMID_APL_RF_TRAINING_DATA,		"APL_RF_TRAINING_DATA"			},	/*!< 43 : RF個別トレーニングデータ		*/
	{	E_RRH_SHMID_APL_DU_CMD_INFO,			"APL_DU_CMD_INFO"				},	/*!< 44 : DUコマンド管理情報			*/
	{	E_RRH_SHMID_APL_DU_FLT_MNG,				"APL_DU_FLT_MNG"				},	/*!< 45 : Fault管理情報					*/
	{	E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,	"APL_RRH_NETCONF_INFO_TBL"		},	/*!< 46 : NETCONFユーザー情報テーブル	*/
	{	E_RRH_SHMID_APL_TXCAL_COEF,				"APL_TXCAL_COEF"				},	/*!< 47 : Tx CAL係数					*/
	{	E_RRH_SHMID_APL_RXCAL_COEF,				"APL_RXCAL_COEF"				},	/*!< 48 : Rx CAL係数					*/
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,	"APL_PFM_PERFORMANCE_TBL"		},	/*!< 49 : Performanceログテーブル		*/
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,	"APL_PFM_PERFORMANCE_TBL"		},	/*!< 50 : Performance情報管理テーブル	*/
	{	E_RRH_SHMID_APL_LBM_MNGMENT_TBL,		"APL_LBM_MNGMENT_TBL"			},	/*!< 51 : LBM/LBR管理テーブル			*/
	{	E_RRH_SHMID_EIO_SVCTRL_MNG_TBL,			"EIO_SVCTRL_MNG_TBL"			},	/*!< 52 : 外部IO情報テーブル			*//*	5G-商用DU	*/
	{	E_RRH_SHMID_EIO_SVCTRL_LOG_TBL,			"EIO_SVCTRL_LOG_TBL"			},	/*!< 53 : 外部IO制御履歴テーブル		*//*	5G-商用DU	*/
	{	E_RRH_SHMID_APL_TP_CMD_INFO,			"APL_TP_CMD_INFO"				},	/*!< 54 : TPコマンド管理情報			*/
	{	E_RRH_SHMID_APL_MT_INFO,				"APL_MT_INFO"					},	/*!< 55 : MT管理情報			*/
	{	E_RRH_SHMID_APL_BF_MODE_TBL,			"APL_BF_MODE_TBL"				},	/*!< 56 : Beamforming Mode Flag			*/
	{	E_RRH_SHMID_APL_SFPFS_FLG,			"APL_SFP_FAILSAFE_FLG"				},	/*!< 57 : SFP Fail Safe ON/OFF Flag		*/
	{	E_RRH_SHMID_APL_DLF_LOG,				"APL_DLF_STAT_LOG"				}	/*!< 58 : DLF統計情報					*/
};

int cmd_shm_dmp_main(int argc, char *argv[], FILE *ofp ) {

	int	sel;
	int size ;
	int i ;
	void *shadr ;
	char *dmpp;
	int	iErrCode;
	int iRetCode;

	if( argc < 2 )	{
		fprintf(ofp, "shared memory dump select key from \n");
		for( i = 0 ; i < E_RRH_SHMID_APL_MAX ; i++ )	{
			printf( " %d %s\n", i, shsel[i].desc );
		}
		printf( "select     => ");
		iRetCode = scanf("%d", &sel );
	}
	else {	/* キー選択も指定 */
		sel = atoi(argv[1] );
	}
	if ( argc == 3 ) {	/* サイズ選択まで指定	*/
		size = atoi(argv[2]);
	}
	else {
		printf("dump size  => ");
		iRetCode = scanf("%d", &size );
	}
	printf("select key :[%s](%d) dump size:%d\n", shsel[sel].desc, shsel[sel].shared_key, size );

	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( shsel[sel].shared_key, &shadr, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return RTC_NG;
	}
	dmpp = (char *)shadr ;
	fprintf(ofp, "shared key get. addr : %p\n", dmpp );
	for( i = 0 ; i < size ; i++ )	{
		if(( i % 0x10 ) == 0 )	{
			fprintf( ofp, "\n%08x: ", i );
		}
		fprintf(ofp, "%02x ", *(dmpp+i) & 0xff );
	}
	fprintf(ofp, "\n");
	return 0;
}


