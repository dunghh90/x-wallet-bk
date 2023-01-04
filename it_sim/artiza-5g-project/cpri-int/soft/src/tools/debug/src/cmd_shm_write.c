#include "f_dbg_cmd_inc.h"

extern int cnvDecStr2Long(char*, unsigned int*);
extern int cnvHexStr2Long(char*, unsigned int*);

typedef struct {
	int		shared_key	;
	char	*desc		;
}T_SHM_SELECT;

static T_SHM_SELECT shsel[E_RRH_SHMID_APL_MAX]	=		{
	{	E_RRH_SHMID_BPF_MNG,				"BPF_MNG"							},	/*!< 00 : ���L�������Ǘ�				*/
	{	E_RRH_SHMID_MNG_COM_MEMORY,			"MNG_COM_MEMORY"					},	/*!< 01 : �ʐM�p���L�������Ǘ�			*/
	{	E_RRH_SHMID_LOGLEVEL_MNG,			"LOGLEVEL_MNG"						},	/*!< 02 : ���O�擾���x���Ǘ��e�[�u��	*/
	{	E_RRH_SHMID_TIMER_MNG,				"TIMER_MNG"							},	/*!< 03 : �^�C�}�Ǘ��e�[�u��			*/
	{	E_RRH_SHMID_APL_PROC_ID_MNG_INFO,	"APL_PROC_ID_MNG_INFO"				},	/*!< 04 : �v���Z�XID�Ǘ��e�[�u��		*/
	{	E_RRH_SHMID_APL_THRD_ID_MNG_INFO,	"APL_THRD_ID_MNG_INFO"				},	/*!< 05 : �X���b�hID�Ǘ��e�[�u��		*/
	{	E_RRH_SHMID_APL_RST_REASON_LOG,		"APL_RST_REASON_LOG"				},	/*!< 06 : reset reason log				*/
	{	E_RRH_SHMID_APL_ASSERT_LOG,			"APL_ASSERT_LOG"					},	/*!< 07 : Assert Log					*/
	{	E_RRH_SHMID_APL_ALM_HISTORY_LOG,	"APL_ALM_HISTORY_LOG"				},	/*!< 08 : Alarm Hisotry Log				*/
	{	E_RRH_SHMID_APL_BUFFER_HISTORY,		"APL_BUFFER_HISTORY"				},	/*!< 09 : ���b�Z�[�W�o�b�t�@�l������	*/
	{	E_RRH_SHMID_APL_TIMER_HISTORY,		"APL_TIMER_HISTORY"					},	/*!< 10 : �^�C�}����					*/
	{	E_RRH_SHMID_APL_SEMAPHORE_HISTORY,	"APL_SEMAPHORE_HISTORY"				},	/*!< 11 : �Z�}�t�H����					*/
	{	E_RRH_SHMID_APL_DPDA_ALM_LOG,		"APL_DPDA_ALM_LOG"					},	/*!< 12 : dpda alarm log				*/
	{	E_RRH_SHMID_APL_DPDA_MNG_TBL,		"APL_DPDA_MNG_TBL"					},	/*!< 13 : dpda���Ǘ��e�[�u��			*/
	{	E_RRH_SHMID_APL_INVENTORY,			"APL_INVENTORY"						},	/*!< 14 : �C���x���g�����				*/
	{	E_RRH_SHMID_APL_RRH_KIND,			"APL_RRH_KIND"						},	/*!< 15 : DU���						*/
	{	E_RRH_SHMID_APL_ALM_HIS_PREPARE,	"APL_ALM_HIS_PREPARE"				},	/*!< 16 : Alarm History log���W����Table*/
	{	E_RRH_SHMID_APL_TEMP_LOG_TBL,		"APL_TEMP_LOG_TBL"					},	/*!< 17 : Temprature Log Table			*/
	{	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS,	"APL_SFP0_MNG_INFO_HIS"				},	/*!< 18 : SFP#0�Ǘ���񗚗�				*/
	{	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,	"APL_SFP1_MNG_INFO_HIS"				},	/*!< 19 : SFP#1�Ǘ���񗚗�				*/
	{	E_RRH_SHMID_APL_CAL_HIS,			"APL_CAL_HIS"						},	/*!< 20 : CAL����						*/
	{	E_RRH_SHMID_APL_ALM_NTC_HIS,		"APL_ALM_NTC_HIS"					},	/*!< 21 : Alarm�ʒm���O					*/
	{	E_RRH_SHMID_APL_EQ_HIS,				"APL_EQ_HIS"						},	/*!< 22 : ���u��ԃ��O					*/
	{	E_RRH_SHMID_APL_ACTIVE_ALM_LIST,	"APL_ACTIVE_ALM_LIST"				},	/*!< 23 : Active-Alarm-list				*/
	{	E_RRH_SHMID_APL_CAR_STAT_HIS,		"APL_CAR_STAT_HIS"					},	/*!< 24 : �L�����A��ԃ��O				*/
	{	E_RRH_SHMID_APL_PTP_CONF,			"APL_PTP_CONF"						},	/*!< 25 : PTP�N�����p�����[�^			*/
	{	E_RRH_SHMID_APL_PTP_STAT_HIS,		"APL_PTP_STAT_HIS"					},	/*!< 26 : PTP��ԃ��O					*/
	{	E_RRH_SHMID_APL_PTP_TIME_ADJ,		"APL_PTP_TIME_ADJ"					},	/*!< 27 : PTP�����␳					*/
	{	E_RRH_SHMID_APL_UPSIGSTOP_HIS,		"APL_UPSIGSTOP_HIS"					},	/*!< 28 : ��M����g����				*/
	{	E_RRH_SHMID_APL_FH_L1_HIS,			"APL_FH_L1_HIS"						},	/*!< 29 : FH L1����						*/
	{	E_RRH_SHMID_APL_SFNFRM_CALB_TBL,	"APL_SFNFRM_CALB_TBL"				},	/*!< 30 : SFN/FRM�␳�p�e�[�u��			*/
	{	E_RRH_SHMID_APL_TXPOW_DETECT_TBL,	"APL_TXPOW_DETECT_TBL"				},	/*!< 31 : Tx power detector �e�[�u��	*/
	{	E_RRH_SHMID_APL_TXPOW_DETECT_LOG,	"APL_TXPOW_DETECT_LOG"				},	/*!< 32 : Tx power detector LOG			*/
	{	E_RRH_SHMID_APL_LUT_LOG,			"APL_LUT_LOG"						},	/*!< 33 : LUT LOG						*/
	{	E_RRH_SHMID_APL_FH_STAT,			"APL_FH_STAT"						},	/*!< 34 : FH���v���					*/
	{	E_RRH_SHMID_APL_MAC_STAT_ETH0,		"APL_MAC_STAT_ETH0"					},	/*!< 35 : Ethernet �����N��Q�������O(ETH#1)	*/
	{	E_RRH_SHMID_APL_MAC_STAT_ETH1,		"APL_MAC_STAT_ETH1"					},	/*!< 36 : Ethernet �����N��Q�������O(ETH#2)	*/
	{	E_RRH_SHMID_APL_PKT_STAT_MPLANE,	"APL_PKT_STAT_MPLANE"				},	/*!< 37 : �l�b�g���[�N���v(M-Plane)		*/
	{	E_RRH_SHMID_APL_SV_STATE_HISTORY,	"APL_SV_STATE_HISTORY"				},	/*!< 38 : SV��ԕω��������O			*/
	{	E_RRH_SHMID_APL_TRAINING_DATA,		"APL_TRAINING_DATA"					},	/*!< 39 : TP�R�}���h���z�������f�[�^	*/
	{	E_RRH_SHMID_APL_DU_INFORMATION_TBL,	"APL_DU_INFORMATION_TBL"			},	/*!< 40 : DU DHCP���e�[�u��			*/
	{	E_RRH_SHMID_APL_TALISE_CONF_INFO,	"APL_TALISE_CONF_INFO"				},	/*!< 41 : RFIC Config�f�[�^				*/
	{	E_RRH_SHMID_APL_DU_PROCESS_INFO,	"APL_DU_PROCESS_INFO"				},	/*!< 42 : DU information Table			*/
	{	E_RRH_SHMID_APL_RF_TRAINING_DATA,	"APL_RF_TRAINING_DATA"				},	/*!< 43 : RF�ʃg���[�j���O�f�[�^		*/
	{	E_RRH_SHMID_APL_DU_CMD_INFO,		"APL_DU_CMD_INFO"					},	/*!< 44 : DU�R�}���h�Ǘ����			*/
	{	E_RRH_SHMID_APL_DU_FLT_MNG,			"APL_DU_FLT_MNG"					},	/*!< 45 : Fault�Ǘ����					*/
	{	E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,	"APL_RRH_NETCONF_INFO_TBL"		},	/*!< 46 : NETCONF���[�U�[���e�[�u��	*/
	{	E_RRH_SHMID_APL_TXCAL_COEF,			"APL_TXCAL_COEF"					},	/*!< 47 : Tx CAL�W��					*/
	{	E_RRH_SHMID_APL_RXCAL_COEF,			"APL_RXCAL_COEF"					},	/*!< 48 : Rx CAL�W��					*/
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,	"APL_PFM_PERFORMANCE_TBL"		},	/*!< 49 : Performance���O�e�[�u��		*/
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,	"APL_PFM_PERFORMANCE_TBL"		},	/*!< 50 : Performance���Ǘ��e�[�u��	*/
	{	E_RRH_SHMID_APL_LBM_MNGMENT_TBL,		"APL_LBM_MNGMENT_TBL"			},	/*!< 51 : LBM/LBR�Ǘ��e�[�u��			*/
	{	E_RRH_SHMID_EIO_SVCTRL_MNG_TBL,		"EIO_SVCTRL_MNG_TBL"				},	/*!< 52 : �O��IO���e�[�u��			*//*	5G-���pDU	*/
	{	E_RRH_SHMID_EIO_SVCTRL_LOG_TBL,		"EIO_SVCTRL_LOG_TBL"				},	/*!< 53 : �O��IO���䗚���e�[�u��		*//*	5G-���pDU	*/
	{	E_RRH_SHMID_APL_TP_CMD_INFO,		"APL_TP_CMD_INFO"					},	/*!< 54 : TP�R�}���h�Ǘ����			*//*	5G-���pDU	*/
	{	E_RRH_SHMID_APL_MT_INFO,			"APL_MT_INFO"						},	/*!< 55 : MT�Ǘ����			*/
	{	E_RRH_SHMID_APL_BF_MODE_TBL,			"APL_BF_MODE_TBL"				},	/*!< 56 : Beamforming Mode Flag			*/
	{	E_RRH_SHMID_APL_SFPFS_FLG,			"APL_SFP_FAILSAFE_FLG"				},	/*!< 57 : SFP Fail Safe ON/OFF Flag		*/
	{	E_RRH_SHMID_APL_DLF_LOG,				"APL_DLF_LOG"					}	/*!< 58 : DLF���v���					*/
};

int cmd_shm_write_main(int argc, char *argv[], FILE *ofp ) {
	int 	i ;
	char	*shadrc;
	void 	*shadr ;
	int		iErrCode;
	int 	iRetCode;
	unsigned int	sh_key ;
	unsigned int	offset;
	unsigned int	wdat;


	if( argc < 4 ) {
		printf("usage: cmd_shm_write sh-key-id offset data...\n");
		printf("  sh-key-id: shared-memory-key(decimal)\n") ;
        printf("  offset: offset from shared-memory address(decimal)\n");
		printf("  data:   1byte write data(hex) (vararg)\n");
		printf("  key-id:\n");
		for( i =0 ;i < E_RRH_SHMID_APL_MAX; i++ ) {
			printf ("     %2d : %s\n", i, shsel[i].desc );
		}
		printf("\n");
		return RTC_NG;
	}
	sh_key = atoi(argv[1] );
	offset = atoi(argv[2]);

	if( sh_key >= E_RRH_SHMID_APL_MAX  ) {
		fprintf(stderr, "Error: invalid sh-key-id(%d).\n", sh_key) ;
		fprintf(stderr, "valid sh-key-id is:\n");
		for( i =0 ;i < E_RRH_SHMID_APL_MAX; i++ ) {
			printf ("     %2d : %s\n", i, shsel[i].desc );
		}
		printf("\n");
		return RTC_NG;
	}

	if(( iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET( shsel[sh_key].shared_key, (VOID **)&shadr, &iErrCode )) != BPF_RU_IPCM_OK ) {
		fprintf(stderr, "Error: shared memory get error.  (code:%d)\n", iRetCode) ;
		return RTC_NG;
	}
	shadrc = (char *)shadr ;
	printf("[(%d)%s] addr:%p\n", sh_key, shsel[sh_key].desc, shadr ) ;

	if( argc >= 4 ) {

		for( i = 0 ; i < argc - 3 ; i++ ) {
			if( cnvHexStr2Long( argv[3+i], &wdat ) != RTC_OK ) {
				fprintf(stderr, "Error: write data invalid.\n");
				return RTC_NG;
			}
			if( wdat > 0xff ){
				fprintf(stderr, "Error: write value maximum limits 0xff\n");
				return RTC_NG;
			}
			printf("[%4d] %p:%02x -> %02x\n", i, shadrc + offset + i ,*(shadrc + offset + i), wdat ) ;
			*(shadrc + offset + i   ) = (char)wdat ;
		}
		printf("\n");
	}
	return RTC_OK;
}


