/*!
 * @skip   $Id$
 * @file   cmd_alm_hist_log_main.c
 * @brief  アラームヒスト表示
 * @date   2013/12/ ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013 - 2015
 */

#include "f_dbg_cmd_inc.h"
#include "f_dpda_ram.c"
#include "f_dpda_rom.c"
#include <math.h>

extern int BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int shm_num, void **addr, int *errcd);

#define	LINE_SEP_SINGLE	\
"-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"

#define	LINE_SEP_DOUBLE	\
"=============================================================================================================================================================================================================\n"
#define	LINE_SCALE			\
	"                               20       19       18       17       16       15       14       13       12       11       10       09       08       07       06       05       04       03       02       01\n"
#define		ASSERT_LOGS_HEADER	\
"\n* Assert logs(last 10 count)                   --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#define		ANT_CAR_INFO_HEADER	\
"\n* Ant, Car Information                      --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#define		SV_STAT_HEADER	\
"\n* SV Status                                    --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"

#define		TX_ANT_INFO_HEADER	\
"\n* TX Antenna Information                       --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#define		RX_ANT_INFO_HEADER	\
"\n* RX Antenna Information                       --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#define		REG_INFO_HEADER \
"\n* Register Information                         --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#define		REG_INFO_HEADER_EXT \
"\n* Register Information extra                   --------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
/**
 *
 */
const char cAntNum[D_RRH_ANT_MAX+1] = {
	'A',
	'B',
	'C',
	'D',
	'\0'
};
const char stAntDirNum[2][16] = {
	{
		"RX"
	},
	{
		"TX"
	}
};
 
const char *svNames[D_RRH_ALMHIS_SVREG_INFO_NUM] = {
	/* DU(sub6/mmW) */
	"A000_0150",
	"A000_0154",
	"A000_0158",
	"A000_015C",
	"A000_0160",
	"A000_0164",
	"A000_0168",
	"A000_016C",
	"A000_0170",
	"A000_0174",
	"A000_0178",
	"A000_017C",
	"A000_0180",
	"A000_0184",
	"A000_0188",
	"A000_018C"
};


/********************************************************************************************************************/
/**
 *  @brief  alarm history log表示コマンド
 *  @note   alarm history logを表示する
 *  @param  buffer  [in]   buffer
 *  @return none
 */
/********************************************************************************************************************/
extern char *getThreadName( unsigned short ttid ) ;
extern const char	*getRegTypeName(unsigned int regAddr ) ;
extern char *cnvRRH_SYSTIME_TIME2Str(T_RRH_SYSTEM_TIME *tDate) ;
extern char *cnvRRH_SYSTIME_TIME2Str2(T_RRH_SYSTEM_TIME *tDate) ;
extern void assertLogPrint(T_RRH_LOG_ASSERT_RECORD *astLogRec, FILE *outfp );
extern void regLogRecPrint( T_RRH_LOG_REG_ACCESS_RECORD* regLogRec,  FILE *outfp )	;

UINT alm_hist_log_time_chk( T_RRH_SYSTEM_TIME *chkTime )
{
	UINT		rtnCode;
	
	/* 正常な範囲内の時刻であればOKを返す */
	if(	( 2000 <= chkTime->year) &&
		( 2100 > chkTime->year) &&
		( 0  != chkTime->month) &&
		( 12 >= chkTime->month) &&
		( 0  != chkTime->day  ) &&
		( 31 >= chkTime->day  ) &&
		( 23 >= chkTime->hour ) &&
		( 59 >= chkTime->min  ) &&
		( 59 >= chkTime->sec  ) )
	{
		rtnCode = D_RRH_OK;
	}
	else
	{
		rtnCode = D_RRH_NG;
	}
	
	return rtnCode;
}

typedef	struct{
	UINT	svbit;
	CHAR*	id;
	USHORT	fid;
}T_FAULT_ID_INFO;


static T_FAULT_ID_INFO	fault_id[] = 
{
	{ D_DU_SVCTL_BIT_OCCUR_FID_01, "Software fault"						,0x0001	},	/*	E_DU_SVCTL_OCCUR_FIDNO_01	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_02, "Initialization failure"				,0x0002	},  /*	E_DU_SVCTL_OCCUR_FIDNO_02	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_03, "Fronthaul Synchronization Error"	,0x0003	},  /*	E_DU_SVCTL_OCCUR_FIDNO_03	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_04, "Module Clock faulty"				,0x0004	},  /*	E_DU_SVCTL_OCCUR_FIDNO_04	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_05, "Fronthaul faulty by Port1"			,0x0005	},  /*	E_DU_SVCTL_OCCUR_FIDNO_05	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_06, "PS ALM"								,0x0006	},  /*	E_DU_SVCTL_OCCUR_FIDNO_06	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_07, "Common hardware (module) fault"		,0x0007	},  /*	E_DU_SVCTL_OCCUR_FIDNO_07	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_08, "RF module fault"					,0x0008	},  /*	E_DU_SVCTL_OCCUR_FIDNO_08	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_10, "TX out of order"					,0x0010	},  /*	E_DU_SVCTL_OCCUR_FIDNO_10	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_11, "RX out of order"					,0x0011	},  /*	E_DU_SVCTL_OCCUR_FIDNO_11	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_12, "ANT port abnormal"					,0x0012	},  /*	E_DU_SVCTL_OCCUR_FIDNO_12	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_30, "Temperature too low"				,0x0030	},  /*	E_DU_SVCTL_OCCUR_FIDNO_30	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_31, "Unit dangerously overheating"		,0x0031	},  /*	E_DU_SVCTL_OCCUR_FIDNO_31	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_40, "No FAN detected"					,0x0040	},  /*	E_DU_SVCTL_OCCUR_FIDNO_40	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_41, "FAN ps alarm"						,0x0041	},  /*	E_DU_SVCTL_OCCUR_FIDNO_41	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_42, "Cooling FAN broken FAN1"			,0x0042	},  /*	E_DU_SVCTL_OCCUR_FIDNO_42	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_43, "Cooling FAN broken FAN2"			,0x0043	},  /*	E_DU_SVCTL_OCCUR_FIDNO_43	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_44, "Cooling FAN broken FAN3"			,0x0044	},  /*	E_DU_SVCTL_OCCUR_FIDNO_44	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_50, "SFP Error"							,0x0050	},  /*	E_DU_SVCTL_OCCUR_FIDNO_50	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_F0, "Tx Sleep"							,0x00F0	},  /*	E_DU_SVCTL_OCCUR_FIDNO_F0	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_F1, "Tx OFF"								,0x00F1	},  /*	E_DU_SVCTL_OCCUR_FIDNO_F1	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_F2, "TEST Mode"							,0x00F2	},  /*	E_DU_SVCTL_OCCUR_FIDNO_F2	*/
	{ D_DU_SVCTL_BIT_OCCUR_FID_FF, "Forced alarm"						,0x00FF	}   /*	E_DU_SVCTL_OCCUR_FIDNO_FF	*/
};


static void alm_hist_log_fid_print(FILE *ofp, UINT svBus )
{
	UINT	count;

	if(svBus != 0){
		for(count= 0  ; count < (sizeof(fault_id)/sizeof(fault_id[0])) ; count++){
			if((svBus & fault_id[count].svbit) != 0){
				fprintf(ofp,"                  FID:0x%04X %s\n",fault_id[count].fid,fault_id[count].id) ;
			}
		}
	}
	return;
}

static T_FAULT_ID_INFO	sw_fault_id[] = 
{
	{ D_DU_FLTID_OS			, "OS or Internal exception"		},
	{ D_DU_FLTID_EXC		, "Machine check exception"			},
	{ D_DU_FLTID_DSI		, "DSI exception"					},
	{ D_DU_FLTID_ISI		, "ISI exception"					},
	{ D_DU_FLTID_ARAI		, "Alignment exception"				},
	{ D_DU_FLTID_PRO		, "Program exception"				},
	{ D_DU_FLTID_TLB		, "Instruction TLB Miss"			},
	{ D_DU_FLTID_MIS		, "Data TLB Miss exception"			},
	{ D_DU_FLTID_INS		, "Instruction TLB Miss exception"	},
	{ D_DU_FLTID_FNC		, "Funciton Return Failure"			},
	{ D_DU_FLTID_TSK		, "Task Start Failure"				},
	{ D_DU_FLTID_BGET		, "Buffer Get Failure"				},
	{ D_DU_FLTID_BFRE		, "Buffer Release Failure"			},
	{ D_DU_FLTID_TSTA		, "Timer Start Failure"				},
	{ D_DU_FLTID_TEND		, "Timer Stop Failure"				},
	{ D_DU_FLTID_SGET		, "Semaphore Get Failure"			},
	{ D_DU_FLTID_SFRE		, "Semaphore Release Failure"		},
	{ D_DU_FLTID_ANOT		, "Other Failure"					},
	{ D_DU_FLTID_ARNG		, "Logical Failure"					},
	{ D_DU_FLTID_STNG		, "Status Failure"					},
	{ D_DU_FLTID_TKST		, "Thread Status Failure"			},
	{ D_DU_FLTID_CSIF		, "Card Status Failure"				},
	{ D_DU_FLTID_TIF		, "Thread IF Failure"				},
	{ D_DU_FLTID_UBUF		, "User Buffer Manage Failure"		},
	{ D_DU_FLTID_DNG		, "Data Failure."					},
	{ D_DU_FLTID_RNG		, "Route Failure"					},
	{ D_DU_FLTID_FNG		, "Funciton Return Failure 2"		},
	{ D_DU_FLTID_ANG		, "Other Failure 2"					},
	{ D_DU_FLTID_CIF		, "Card IF Failure"					},
	{ D_DU_FLTID_SUM		, "Recv Data Check Sum Failure"		},
	{ D_DU_FLTID_ANNG		, "Other Failure 3"					},
	{ D_DU_FLTID_FWNG		, "FLASH Write Failure"				},
	{ D_DU_FLTID_EENG		, "EEPROM Access Failure"			},
	{ D_DU_FLTID_BUSNG	    , "Serial Bus Control Failure"		},
	{ D_DU_FLTID_ETHNG	    , "Ethernet Control Failure"		},
	{ D_DU_FLTID_ANNG2	    , "Other Failure 4"					},
	{ D_DU_FLTID_FPGACNF	, "FPGA Config Failure"				},
	{ D_DU_FLTID_DIA1		, "1st Diagnostic NG 1"				},
	{ D_DU_FLTID_DIA2		, "1st Diagnostic NG 2"				},
	{ D_DU_FLTID_DIA3		, "1st Diagnostic NG 3"				},
	{ D_DU_FLTID_DIA4		, "1st Diagnostic NG 4"				},
	{ D_DU_FLTID_DIA5		, "1st Diagnostic NG 5"				},
	{ D_DU_FLTID_WDT		, "WDT T.O."						},
	{ D_DU_FLTID_CSTP		, "Check STOP"						},
	{ D_DU_FLTID_REMOTEALM	, "Remote Card ALM"					}
};


static void alm_hist_log_almcd_print(FILE *ofp, UINT almcode )
{
	UINT	count;

	if(almcode != 0){
		for(count= 0  ; count < (sizeof(sw_fault_id)/sizeof(sw_fault_id[0])) ; count++){
			if(almcode != sw_fault_id[count].svbit){
				continue;
			}
			fprintf(ofp,"                  %s\n",sw_fault_id[count].id) ;
			break;
		}
	}
	return;
}


void alm_hist_log_print( CHAR *buffer, UINT uiCnt, UINT uiShort, FILE *ofp )
{
	T_RRH_ALARM_HISTORYMNG*		tAlmHisLog;
	USHORT						usStartIndex;
	USHORT						usDispNum;
	USHORT						usDispCnt;
	USHORT						usAntCnt;
	USHORT						usCarCnt;
	USHORT						usAntDir;
	USHORT						usAstIndex;
	USHORT						usSecIndex;
	USHORT						usSvIndex;
	USHORT						usRegIndex;
	INT							rtn_bpf;
	INT							errcd;
	INT							rtn_timeChk;
	T_RRH_TRAINVTBL				*invTbl;
	UINT						*rrhKind;
	USHORT						carMax;
	USHORT						antMax;
	USHORT						usRegNum, usRegExtNum;
	UINT						uiRegAddr;
	UINT						*regTblArray;
	UINT						uiSVcount;
	UINT						uiSVcount2;
	DL							dlGain;
	UCHAR						regName_mmW[D_RRH_ALMHIS_REG_NUM_MMW][16] = {"RF-PLL:0x0e6", "TXDAC:0x281 ", "TXDAC:0x7B5 ", "RXADC:0x56F ", "RXADC:0x11B "};
	UINT						uiTxPwrDtCnt;

	if( buffer == NULL )
	{
		return;
	}
	
	tAlmHisLog = ( T_RRH_ALARM_HISTORYMNG* )buffer;
	
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_INVENTORY,
											(VOID **)&invTbl,
											&errcd );
	if( BPF_RU_IPCM_OK != rtn_bpf )
	{
		return;
	}
	
	if( tAlmHisLog->alm_log_index > D_RRH_ALMHIS_LOG_RECORD_NUM )
	{
		/* INDEXがMAXより大きい場合はログに何も無いということ	*/
		/* 故にログの表示を行わない								*/
		return;
	}

	/* 表示数を設定する					*/
	/* MAXより大きい場合はクリップする	*/
	if( uiCnt > D_RRH_ALMHIS_LOG_RECORD_NUM )
	{
		uiCnt = D_RRH_ALMHIS_LOG_RECORD_NUM;
	}
	usDispNum = uiCnt;

	/* 表示開始位置を設定する	*/
	if( tAlmHisLog->uiCycleFlg == 0 )
	{
		if( uiCnt >= tAlmHisLog->alm_log_index )
		{
			/* 表示個数の方が多い場合は0から表示	*/
			usStartIndex = 0;
		}
		else
		{
			/* Indexの方が大きい場合は新しい物から表示	*/
			usStartIndex = tAlmHisLog->alm_log_index - uiCnt;
		}
	}
	else
	{
		usStartIndex = ((tAlmHisLog->alm_log_index + D_RRH_ALMHIS_LOG_RECORD_NUM) - uiCnt) % D_RRH_ALMHIS_LOG_RECORD_NUM;
	}

	/* 装置種別を取得する	*/
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,			/* Buffer種別		*/
											(void**)&rrhKind,					/* msgP				*/
											&errcd );
	if( BPF_HM_DEVC_COMPLETE != rtn_bpf )
	{
		fprintf( stderr, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d, errcd = %d", rtn_bpf, errcd);
		return;
	}
	
	/* 装置種別で処理を切り分ける */
	switch( M_DU_RRHKIND_KIND_GET(*rrhKind) )
	{
		case E_RRH_RRHKIND_5GDU_LLS_SUB6:
			carMax = D_RRH_CAR_2;	/* キャリア数=1 */
			antMax = D_RRH_ANT_MAX;		/* アンテナ数=4 */
			regTblArray = (UINT *)f_dpdar_reg_Tbl;
			usRegNum = D_DPDA_REG_SUB6_NUM_MAX;
			usRegExtNum = D_RRH_ALMHIS_EXT_NUM_SUB6;
			break;
		
		case E_RRH_RRHKIND_5GDU_LLS_MMW:
			carMax = D_RRH_CAR_MAX;		/* キャリア数=4 */
			antMax = D_RRH_ANT_MAX_MMW;	/* アンテナ数=2 */
			regTblArray = (UINT *)f_dpdar_reg_Tbl_mmW;
			usRegNum = D_DPDA_REG_MMW_NUM_MAX;
			usRegExtNum = D_RRH_ALMHIS_EXT_NUM_MMW;
			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. rrhKind = %d.", *rrhKind );
			return;
	}
	
	if( 0 != usDispNum  )
	{
		for( usDispCnt=0; usDispCnt<usDispNum; usDispCnt++ )
		{
			/* 異常なログは表示しない	*/
			rtn_timeChk = alm_hist_log_time_chk( &(tAlmHisLog->histlog[usStartIndex].tDate) );
			if(	D_RRH_OK == rtn_timeChk )
			{
				fprintf(ofp, LINE_SEP_DOUBLE );
				fprintf(ofp, "[%04d] \n", usDispCnt );
				fprintf(ofp, LINE_SEP_SINGLE );

				/* バージョン表示	*/
				fprintf(ofp, "  Version       : SW:%04x\n", (tAlmHisLog->histlog[usStartIndex].dlFile_ver&0x0000ffff)  ) ;

				/* 時刻表示	*/
				fprintf(ofp, "  Date          : %s\n", cnvRRH_SYSTIME_TIME2Str(&tAlmHisLog->histlog[usStartIndex].tDate));

				/* アラーム要因		*/
				if(tAlmHisLog->histlog[usStartIndex].factorType == D_RRH_ALARM_FACTOR_TYPE_SW){
				fprintf(ofp, "  SW Fault      : 0x%08x \n",tAlmHisLog->histlog[usStartIndex].swAlarmFactor);			/* Soft 		*/
				alm_hist_log_almcd_print(ofp, tAlmHisLog->histlog[usStartIndex].swAlarmFactor & 0xFF);

				}else if(tAlmHisLog->histlog[usStartIndex].factorType  == D_RRH_ALARM_FACTOR_TYPE_HW ){

				fprintf(ofp, "  HW Fault      : 0x%08x \n",tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3] );
				alm_hist_log_fid_print(ofp,tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3]);

				}else{
	
				fprintf(ofp, "  Fault         : SW:0x%08x HW:0x%08x \n", tAlmHisLog->histlog[usStartIndex].swAlarmFactor , tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3]);
				alm_hist_log_almcd_print(ofp, tAlmHisLog->histlog[usStartIndex].swAlarmFactor & 0xFF);
				alm_hist_log_fid_print(ofp,tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3]);

				}

				fprintf(ofp, "  SV BUS Info   : \n");
				
				for(uiSVcount=0;uiSVcount < E_RRH_ALMHIS_ALM_FACTOR_NUM;uiSVcount+=4){
				fprintf(ofp, "       %08X : ",uiSVcount*4);
					for(uiSVcount2=0;uiSVcount2 < 4;uiSVcount2++){
						fprintf(ofp, "%08x ",tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[uiSVcount+uiSVcount2]);
					}
				fprintf(ofp, "\n");
				}
				fprintf(ofp, LINE_SEP_DOUBLE );
				
				/* アサートログ(直前32個分)	*/
				fprintf(ofp, ASSERT_LOGS_HEADER ) ;
				for( usAstIndex=0; usAstIndex<D_RRH_ALMHIS_AST_LOG_NUM; usAstIndex++ )	/* 蓄積されたアサートログから32件分	*/
				{
					rtn_timeChk = alm_hist_log_time_chk( &(tAlmHisLog->histlog[usStartIndex].astLog[usAstIndex].tDate) );
					if(	D_RRH_OK == rtn_timeChk )
					{
						/* １レコード分出力	*/
						assertLogPrint( &tAlmHisLog->histlog[usStartIndex].astLog[usAstIndex], ofp )	;
					}
				}
				
				if( 1 == uiShort )
				{
					/* short versionの場合はここまでしか表示しない	*/
					/* 出力位置更新	*/
					usStartIndex = ((usStartIndex+1) % D_RRH_ALMHIS_LOG_RECORD_NUM);
					continue;
				}
				
				/************************************************************************************/
				/*  ALM発生直前20秒分の情報															*/
				/************************************************************************************/

				/* Ant,Car情報 */
				fprintf(ofp, ANT_CAR_INFO_HEADER ) ;
				fprintf(ofp, LINE_SCALE );
				
				for(usAntCnt=0; usAntCnt<antMax; usAntCnt++)
				{
					for(usCarCnt=0; usCarCnt<carMax; usCarCnt++)
					{
						for(int i=0; i<2; i++)	//usAntDirのループ
						{
							usAntDir = 1 - i;	//TXから表示するための処理
							/* CarState */
							fprintf(ofp, " Car State(Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].CarState );
							}
							fprintf(ofp, "\n") ;
							
							/* Freq */
							fprintf(ofp, " Freq     (Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].Freq );
							}
							fprintf(ofp, "\n") ;
							
							/* Bandwidth */
							fprintf(ofp, " Bandwidth(Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].Bandwidth );
							}
							fprintf(ofp, "\n") ;
							
							/* AcellId */
							fprintf(ofp, " AcellId  (Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].AcellId );
							}
							fprintf(ofp, "\n") ;
							
							/* CcId */
							fprintf(ofp, " CcId     (Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].CcId );
							}
							fprintf(ofp, "\n") ;
							
							/* PortId */
							fprintf(ofp, " PortId   (Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].PortId );
							}
							fprintf(ofp, "\n") ;
							if(usAntDir == 1){
							/* Gain */
							fprintf(ofp, " Gain     (Ant%c, Cr%d, %s): ", cAntNum[usAntCnt], usCarCnt, stAntDirNum[usAntDir]);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
								dlGain = tAlmHisLog->histlog[usStartIndex].info[usSecIndex].antInfo[usAntCnt].carInfo[usCarCnt][usAntDir].Gain;
								if((dlGain == 0)||(dlGain == 0xFFFFFFFF)){
									fprintf(ofp, " 000.0000");
								}else{
									fprintf(ofp, " %08.4f", dlGain / 10000.0000 );
								}
							}
							fprintf(ofp, "\n") ;
							}
						}
					}
				}
				
				
				/* SV情報 */
				fprintf(ofp, SV_STAT_HEADER ) ;
				fprintf(ofp, LINE_SCALE );
				for( usSvIndex = 0 ; usSvIndex < D_RRH_ALMHIS_SVREG_INFO_NUM ; usSvIndex++ )	{
					if( NULL != svNames[usSvIndex] )
					{
						fprintf(ofp, " Sv#%02d %-10s      : ", usSvIndex, svNames[usSvIndex] );
						for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].svInfo.svReg[usSvIndex] );	}
						fprintf(ofp, "\n");
					}
				}
				
				/* 送信アンテナ情報 */
				fprintf(ofp, TX_ANT_INFO_HEADER ) ;
				fprintf(ofp, LINE_SCALE );
				
				for( usAntCnt=0; usAntCnt<antMax; usAntCnt++ )
				{
					for( usCarCnt=0; usCarCnt<carMax; usCarCnt++ )
					{
						/* TSSI */
						fprintf(ofp, " TSSI (ANT%d C%d)       : ", usAntCnt, usCarCnt);
						for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.tssi[usAntCnt][usCarCnt]);	}
						fprintf(ofp, "\n");
					}
					
					/* Ant Power */
					fprintf(ofp, " Ref Power(ANT%d)      : ", usAntCnt);
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.refPow[usAntCnt]);	}
					fprintf(ofp, "\n");
					
					/* FbPower */
					fprintf(ofp, " FB Power(ANT%d)       : ", usAntCnt);
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.fbPow[usAntCnt]);	}
					fprintf(ofp, "\n");

					/* VSWR(RL) */
					fprintf(ofp, " VSWR RL(ANT%d)        : ", usAntCnt);
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{
							if(
							( tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.vswrRL[usAntCnt] == 0 ) ||
							( isnan(tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.vswrRL[usAntCnt]) != 0 ) ||
							( isinf(tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.vswrRL[usAntCnt]) != 0 )
							)
							{
								fprintf(ofp, " 000.0000");
							}else{
								fprintf(ofp, " %08.4f", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.vswrRL[usAntCnt]);
							}
					}
					fprintf(ofp, "\n");

					if(antMax == D_RRH_ANT_MAX_MMW){	/* MMWのみ表示 */
						for( uiTxPwrDtCnt=0; uiTxPwrDtCnt<16; uiTxPwrDtCnt++ )
						{
							fprintf(ofp, " TX Power TC(ANT%d) #%02d: ", usAntCnt,uiTxPwrDtCnt + 1);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.txPwrPsum[usAntCnt][uiTxPwrDtCnt]);	}
							fprintf(ofp, "\n");
						}

						for( uiTxPwrDtCnt=0; uiTxPwrDtCnt<45; uiTxPwrDtCnt++ ){
							fprintf(ofp, " TX Power ID(ANT%d) #%02d: ", usAntCnt,uiTxPwrDtCnt + 1);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.txPwrCnt[usAntCnt][uiTxPwrDtCnt]);	}
							fprintf(ofp, "\n");
						}

							fprintf(ofp, " TX Power REF(ANT%d)   : ", usAntCnt);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.txPwrMon[usAntCnt]);	}
							fprintf(ofp, "\n");

							fprintf(ofp, " TX Power PWR(ANT%d)   : ", usAntCnt);
							for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.txPwrPsum_avg[usAntCnt]);	}
							fprintf(ofp, "\n");

					}

				}
					/* TEMP */
					fprintf(ofp, " TEMP                 : ");
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].txAntInfo.temp);	}
					fprintf(ofp, "\n");

				
				/* 受信アンテナ情報	*/
				fprintf(ofp, RX_ANT_INFO_HEADER ) ;
				fprintf(ofp, LINE_SCALE );
				
				for( usAntCnt=0; usAntCnt<antMax; usAntCnt++ )
				{
					/* RcPower */
					fprintf(ofp, " RxPower(ANT%d)        : ", usAntCnt);
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].rxAntInfo.rxPower[usAntCnt]);	}
					fprintf(ofp, "\n");
					
					/* RTWP */
					for( usCarCnt=0; usCarCnt<carMax; usCarCnt++ )
					{
						fprintf(ofp, " RTWP(ANT%d C%d)        : ", usAntCnt, usCarCnt );
						for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].rxAntInfo.Rtwp[usAntCnt][usCarCnt]);	}
						fprintf(ofp, "\n");
					}
				}

				/* 温度補償制御RX 温度補償リダクション量[dBm] */
				if( M_DU_RRHKIND_KIND_GET(*rrhKind) == E_RRH_RRHKIND_5GDU_LLS_MMW ) {

					fprintf(ofp, " Rx Temp Reduction val: ");
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].rxAntInfoExt.RxTempReduction);	}
					fprintf(ofp, "\n");

					for( usAntCnt=0; usAntCnt<antMax; usAntCnt++ )
					{
						/* RcPower */
						fprintf(ofp, " RxPower(ANT%d)        : ", usAntCnt);
						for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )	{	fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].rxAntInfo.rxPower[usAntCnt]);	}
						fprintf(ofp, "\n");
					}
				}


				/* レジスタ生情報	*/
				fprintf(ofp, REG_INFO_HEADER ) ;
				fprintf(ofp, LINE_SCALE );
				
				for( usRegIndex = 0 ; usRegIndex < usRegNum ; usRegIndex++ )
				{
					uiRegAddr = regTblArray[ usRegIndex ];
					
					fprintf(ofp, " %04x_%04x            : ",
						 (( uiRegAddr & 0xffff0000 ) >> 16 ),
						 ((uiRegAddr & 0x0000ffff )       ) ) ;
					
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )
					{
						fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].regInfo.regVal[usRegIndex]);
					}
					fprintf(ofp, "\n");
				}
				
				/* 拡張レジスタ生情報	*/
				fprintf(ofp, REG_INFO_HEADER_EXT ) ;
				fprintf(ofp, LINE_SCALE );
				
				for( usRegIndex = 0 ; usRegIndex < usRegExtNum ; usRegIndex++ )
				{
					fprintf(ofp, " %s         : ", regName_mmW[ usRegIndex ]) ;
					
					for( usSecIndex = 0 ; usSecIndex < D_RRH_ALMHIS_LOG_MEAS_NUM ; usSecIndex++ )
					{
						fprintf(ofp, " %08x", tAlmHisLog->histlog[usStartIndex].info[usSecIndex].regExt.regVal[usRegIndex]);
					}
					fprintf(ofp, "\n");
				}
				
				fprintf(ofp, "\n");
			}
						
			/* 出力位置更新	*/
			usStartIndex = ((usStartIndex+1) % D_RRH_ALMHIS_LOG_RECORD_NUM);
		}
	}

}

UINT alm_hist_factget( UINT uiSVcount , UINT* SVbus , UINT* FactReg ){
	UINT	FactNum = 0;
	
	switch(uiSVcount){
		case E_DU_SVCTL_OCCUR_FIDNO_01:
			FactReg[FactNum] = SVbus[4] & D_DU_ALMBIT_FLT01_ALL;
			FactNum++;
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_02:
			FactReg[FactNum] = (SVbus[4] & D_DU_ALMBIT_FLT02_ALL) >> 16;
			FactNum++;
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_03:
			if((SVbus[5] & D_DU_ALMBIT_FLT03_ALL) != 0){
				FactReg[FactNum] = D_DU_FLTID_PTP_UNLOCK;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_04:
			if((SVbus[5] & D_DU_ALMBIT_JC_PLL_UNLOCK) != 0){
				FactReg[FactNum] = D_DU_FLTID_JC_PLL_UNLOCK;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_RF_PLL_UNLOCK) != 0){
				FactReg[FactNum] = D_DU_FLTID_RF_PLL_UNLOCK;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_05:
			if((SVbus[5] & D_DU_ALMBIT_L1_LDOWN_FH1) != 0){
				FactReg[FactNum] = D_DU_FLTID_L1_LDOWN_FH1;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_BER_DET_FH1) != 0){
				FactReg[FactNum] = D_DU_FLTID_BER_DET_FH1;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_SFP_UNMOUNT_FH1) != 0){
				FactReg[FactNum] = D_DU_FLTID_SFP_UNMOUNT_FH1;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_06:
			if((SVbus[5] & D_DU_ALMBIT_PS_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_PS_ALM;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_POW_ERR1) != 0){
				FactReg[FactNum] = D_DU_FLTID_POW_ERR1;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_POW_ERR2) != 0){
				FactReg[FactNum] = D_DU_FLTID_POW_ERR2;
				FactNum++;
			}
			if((SVbus[5] & D_DU_ALMBIT_SFP_PDOWN) != 0){
				FactReg[FactNum] = D_DU_FLTID_SFP_PDOWN;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_07:
			if((SVbus[6] & D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_SYS_PLL_UNLOCK_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_IC_PLL_UNLOCK_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_IC_PLL_UNLOCK_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_ETH_PLL_UNLOCK_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_PCIE_ERR1_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_PCIE_ERR2_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_SV_FAIL_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_SV_FAIL_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_ETH_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_ETH_ALM;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_SRAM_CORRECT_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_SRAM_CORRECT_ERR;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_SRAM_UNCORRECT_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_SRAM_UNCORRECT_ERR;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_IC_TX_ERR_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_IC_RX_ERR_LLB;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_LLB_IC_TX_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_LLB_IC_RX_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_AUTO_CRUISE_HC_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_AUTO_CRUISE_HC_ERR;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_SYS_PLL_UNLOCK_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_IC_PLL_UNLOCK_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_PHYICPLLULCK1_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_PHYICPLLULCK1_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_PHYICPLLULCK2_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_PHYICPLLULCK2_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_PHY_POW_ALM_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_PHY_POW_ALM_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_PHYJS1PLLULCK_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_PHYJS1PLLULCK_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_PHYJS2PLLULCK_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_PHYJS2PLLULCK_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_TRX1_SV_FAIL_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_TRX1_SV_FAIL_TRX1;
				FactNum++;
			}	
			if((SVbus[6] & D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R1_TX_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_R1_FB_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R1_FB_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R1_RX_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_R2_TX_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R2_TX_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_R2_FB_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R2_FB_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_R2_RX_JESD_ERR_TRX1) != 0){
				FactReg[FactNum] = D_DU_FLTID_R2_RX_JESD_ERR_TRX1;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_LMB_CORRECT_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_LMB_CORRECT_ERR;
				FactNum++;
			}
			if((SVbus[6] & D_DU_ALMBIT_LMB_UNCORRECT_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_LMB_UNCORRECT_ERR;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_08:
			if((SVbus[7] & D_DU_ALMBIT_RFIC1_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_RFIC1_ALM;
				FactNum++;
			}
			if((SVbus[7] & D_DU_ALMBIT_RFIC2_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_RFIC2_ALM;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_10:
			if((SVbus[8] & D_DU_ALMBIT_TX_GAIN_ALM_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_GAIN_ALM_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_TX_GAIN_ALM_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_GAIN_ALM_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TX_GAIN_ALM_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_GAIN_ALM_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TX_GAIN_ALM_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_GAIN_ALM_ANT_D;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_TOVER_ALM_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_TOVER_ALM_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_TOVER_ALM_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_TOVER_ALM_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TOVER_ALM_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_TOVER_ALM_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TOVER_ALM_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_TOVER_ALM_ANT_D;
				FactNum++;
			}
			
			if((SVbus[8] & D_DU_ALMBIT_TX_CAL_FAIL_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_CAL_FAIL_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_TX_CAL_FAIL_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_CAL_FAIL_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TX_CAL_FAIL_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_CAL_FAIL_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_TX_CAL_FAIL_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_CAL_FAIL_ANT_D;
				FactNum++;
			}

			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V1) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V1;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V2) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V2;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V3) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V3;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V4) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V4;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V5) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V5;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V6) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V6;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V7) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V7;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V8) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V8;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V9) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V9;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V10) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V10;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V11) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V11;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V12) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V12;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V13) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V13;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V14) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V14;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V15) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V15;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_V16) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_V16;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H1) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H1;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H2) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H2;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H3) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H3;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H4) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H4;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H5) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H5;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H6) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H6;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H7) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H7;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H8) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H8;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H9) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H9;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H10) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H10;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H11) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H11;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H12) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H12;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H13) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H13;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H14) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H14;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H15) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H15;
				FactNum++;
			}
			if((SVbus[10] & D_DU_ALMBIT_TX_PW_ALM_H16) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_PW_ALM_H16;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_11:
			if((SVbus[8] & D_DU_ALMBIT_RX_GAIN_ALM_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_GAIN_ALM_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_RX_GAIN_ALM_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_GAIN_ALM_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_RX_GAIN_ALM_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_GAIN_ALM_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_RX_GAIN_ALM_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_GAIN_ALM_ANT_D;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_RX_CAL_FAIL_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_CAL_FAIL_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_RX_CAL_FAIL_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_CAL_FAIL_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_RX_CAL_FAIL_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_CAL_FAIL_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_RX_CAL_FAIL_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_RX_CAL_FAIL_ANT_D;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_LNA_ABNORMAL_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_LNA_ABNORMAL_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_LNA_ABNORMAL_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_LNA_ABNORMAL_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_LNA_ABNORMAL_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_LNA_ABNORMAL_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_LNA_ABNORMAL_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_LNA_ABNORMAL_ANT_D;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_12:
			if((SVbus[8] & D_DU_ALMBIT_VSWR_ALM_ANT_A) != 0){
				FactReg[FactNum] = D_DU_FLTID_VSWR_ALM_ANT_A;
				FactNum++;
			}
			if((SVbus[8] & D_DU_ALMBIT_VSWR_ALM_ANT_B) != 0){
				FactReg[FactNum] = D_DU_FLTID_VSWR_ALM_ANT_B;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_VSWR_ALM_ANT_C) != 0){
				FactReg[FactNum] = D_DU_FLTID_VSWR_ALM_ANT_C;
				FactNum++;
			}
			if((SVbus[9] & D_DU_ALMBIT_VSWR_ALM_ANT_D) != 0){
				FactReg[FactNum] = D_DU_FLTID_VSWR_ALM_ANT_D;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_30:
			if((SVbus[12] & D_DU_ALMBIT_TEMP_ALM_LOW) != 0){
				FactReg[FactNum] = D_DU_FLTID_TEMP_ALM_LOW;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_31:
			if((SVbus[12] & D_DU_ALMBIT_TEMP_ALM_HIGH) != 0){
				FactReg[FactNum] = D_DU_FLTID_TEMP_ALM_HIGH;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_40:
			if((SVbus[12] & D_DU_ALMBIT_NO_FAN_DETECT) != 0){
				FactReg[FactNum] = D_DU_FLTID_NO_FAN_DETECT;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_41:
			if((SVbus[12] & D_DU_ALMBIT_FAN_PS_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_FAN_PS_ALM;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_42:
			if((SVbus[12] & D_DU_ALMBIT_COOL_FAN1_BROK) != 0){
				FactReg[FactNum] = D_DU_FLTID_COOL_FAN1_BROK;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_43:
			if((SVbus[12] & D_DU_ALMBIT_COOL_FAN2_BROK) != 0){
				FactReg[FactNum] = D_DU_FLTID_COOL_FAN2_BROK;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_44:
			if((SVbus[12] & D_DU_ALMBIT_COOL_FAN3_BROK) != 0){
				FactReg[FactNum] = D_DU_FLTID_COOL_FAN3_BROK;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_50:
			if((SVbus[12] & D_DU_ALMBIT_SFP_TYP_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_SFP_TYP_ERR;
				FactNum++;
			}
			if((SVbus[12] & D_DU_ALMBIT_SFP_ACCESS_ERR) != 0){
				FactReg[FactNum] = D_DU_FLTID_SFP_ACCESS_ERR;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_F0:
			if((SVbus[13] & D_DU_ALMBIT_TX_SLEEP) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_SLEEP;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_F1:
			if((SVbus[13] & D_DU_ALMBIT_TX_OFF) != 0){
				FactReg[FactNum] = D_DU_FLTID_TX_OFF;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_F2:
			if((SVbus[13] & D_DU_ALMBIT_BEAM_ID_FIX) != 0){
				FactReg[FactNum] = D_DU_FLTID_BEAM_ID_FIX;
				FactNum++;
			}
			break;
		case E_DU_SVCTL_OCCUR_FIDNO_FF:
			if((SVbus[13] & D_DU_ALMBIT_FORCE_ALM) != 0){
				FactReg[FactNum] = D_DU_FLTID_FORCE_ALM;
				FactNum++;
			}
			break;
		default:
			break;
	}
	return FactNum;
}

void alm_hist_log_print_factory( CHAR *buffer, UINT factory,FILE *ofp )
{
	T_RRH_ALARM_HISTORYMNG*		tAlmHisLog;
	USHORT						usStartIndex;
	USHORT						usDispCnt;
	INT							rtn_timeChk;
	UINT						uiSVcount;
	UINT						uiSVcount2;
	UINT						svBus;
	UINT						FactNum;
	UINT						FactReg[128];

	fprintf(ofp, "TIME,Ver,FaultName,FaultID,Code\n");
	if(factory == 2){
		UINT					state[D_SYS_NUM16];
		T_RRH_SYSTEM_TIME		date;
		T_RRH_TRAINVTBL			*invTbl = NULL;
		INT						errcd;
		USHORT					sw_ver;

		BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
		if(invTbl != NULL){
			if( invTbl->cmx_bootup_face == 0 ){
				sw_ver = invTbl->cmx_f0_sver[0];
			}
			else{
				sw_ver = invTbl->cmx_f1_sver[0];
			}
		
			BPF_HM_DEVC_GETTIME(&date);
			BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV1INF,sizeof(state),state);
			svBus = state[3];
			for(uiSVcount= 0  ; uiSVcount < (sizeof(fault_id)/sizeof(fault_id[0])) ; uiSVcount++){
				if((svBus & fault_id[uiSVcount].svbit) == 0){
					continue;
				}
				FactNum = alm_hist_factget(uiSVcount,state,FactReg);
				for(uiSVcount2=0 ; uiSVcount2 < FactNum ;  uiSVcount2++){
					/* 時刻表示	*/
					fprintf(ofp, "%s,0x%04X,%s,0x%04X,0x%04X\n",
						cnvRRH_SYSTIME_TIME2Str2(&date),
						sw_ver,
						fault_id[uiSVcount].id,
						fault_id[uiSVcount].fid,
						(USHORT)FactReg[uiSVcount2]
					);
				}
			}
		}

	}else{
		tAlmHisLog = ( T_RRH_ALARM_HISTORYMNG* )buffer;
		if( tAlmHisLog->alm_log_index > D_RRH_ALMHIS_LOG_RECORD_NUM ){
			return;
		}
		if( tAlmHisLog->uiCycleFlg == 0 ){
			usStartIndex = 0;
		}else{
			usStartIndex = tAlmHisLog->alm_log_index % D_RRH_ALMHIS_LOG_RECORD_NUM;
		}

		for( usDispCnt=0; usDispCnt < D_RRH_ALMHIS_LOG_RECORD_NUM; usDispCnt++ ){
			rtn_timeChk = alm_hist_log_time_chk( &(tAlmHisLog->histlog[usStartIndex].tDate) );
			if(	D_RRH_OK == rtn_timeChk ){
				if(tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3] != 0){
					svBus = tAlmHisLog->histlog[usStartIndex].hwAlarmFactor[3];
					for(uiSVcount= 0  ; uiSVcount < (sizeof(fault_id)/sizeof(fault_id[0])) ; uiSVcount++){
						if((svBus & fault_id[uiSVcount].svbit) == 0){
							continue;
						}
						FactNum = alm_hist_factget(uiSVcount,tAlmHisLog->histlog[usStartIndex].hwAlarmFactor,FactReg);
						for(uiSVcount2=0 ; uiSVcount2 < FactNum ;  uiSVcount2++){
							/* 時刻表示	*/
							fprintf(ofp, "%s,0x%04X,%s,0x%04X,0x%04X\n",
								cnvRRH_SYSTIME_TIME2Str2(&tAlmHisLog->histlog[usStartIndex].tDate),
								(USHORT)(tAlmHisLog->histlog[usStartIndex].dlFile_ver & 0x0000ffff),
								fault_id[uiSVcount].id,
								fault_id[uiSVcount].fid,
								(USHORT)FactReg[uiSVcount2]
							);
						}

					}
				}
			}
			/* 出力位置更新	*/
			usStartIndex = ((usStartIndex+1) % D_RRH_ALMHIS_LOG_RECORD_NUM);
		}
	}
}
/*!
 * @brief 関数機能概要:使用法説明表示
 * @note  関数処理内容.
 *       -# アラームヒストログ表示の使用法を説明する
 * @date 2013/12 ALPHA)宮崎 create
 */

void cmd_alm_hist_log_printUsage()
{
	printf("Usage : cmd_alm_hist\n");
	printf("  cmd_alm_hist [detail]\n");
	printf("    detail: numeric -> long  version \n");
	printf("            s       -> short version \n");
	printf("            f       -> factory display version \n");
	printf("            n       -> current display version \n");
	printf("            c       -> clear\n\n");
	printf("example: cmd_alm_hist 10\n");
	printf("         cmd_alm_hist s\n");
	printf("         cmd_alm_hist c\n");
	return;
}
/*!
 * @brief alarm history log表示コマンド
 * @note  関数処理内容.
 *       -# alarm history logを表示する
 *  @note   alarm history logを表示する
 *  @param  argc  [in]   Standard main argc
 *  @param  argv  [in]   Standard main argv
 *  @return result code
 *  @retval D_CMD_OK
 *  @retval D_CMD_NG
 * @date 2013/12 ALPHA)宮崎 create
 */

int cmd_alm_hist_log_main( int argc, char *argv[], FILE *ofp )
{
	T_RRH_ALARM_HISTORYMNG*		tAlmHisLog;
	INT							iErrCode;
	INT							iCnt = 1;
	CHAR* 						endptr = "\0";
	UINT						uiShort = 0;
	UINT						uiClear = 0;
	UINT						uiFactory = 0;

	if( argc >= 2 )
	{
		if( (strcmp(argv[1],"s")==0) )
		{
			uiShort = 1;
			iCnt = 32;
		}
		else if( (strcmp(argv[1],"c")==0) )
		{
			uiClear = 1;
		}
		else if( (strcmp(argv[1],"f")==0) )
		{
			uiFactory = 1;
		}
		else if( (strcmp(argv[1],"n")==0) )
		{
			uiFactory = 2;
		}
		else
		{
			/* 10進値に変換できるかチェック */
			iCnt = strtol(argv[1], &endptr, 10);

			if( (strcmp(endptr,"\0")!=0) || ( 0 == iCnt ) )
			{
				/* Usage表示を行って終了する	*/
				cmd_alm_hist_log_printUsage();
				return D_CMD_NG;
			}
		}
	}

	/************************************************************/
	/* Get Alarm History log(from shared memory)				*/
	/************************************************************/
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_ALM_HISTORY_LOG,
										(VOID**)&tAlmHisLog,
										&iErrCode);
	
	/* NULLアクセス防止	*/
	if( NULL == tAlmHisLog )
	{
		return 0;
	}

	if( 1 == uiClear )
	{
		/* AlarmHistoryLogをClearする	*/
		memset( (unsigned char*)tAlmHisLog, 0, sizeof(T_RRH_ALARM_HISTORYMNG) );
	}
	else
	{
		if(uiFactory != 0){
			/* Alarm History Log time表示	*/
			alm_hist_log_print_factory( (CHAR *)tAlmHisLog,uiFactory,ofp );
		}else{
			/* Alarm History Log表示	*/
			alm_hist_log_print( (CHAR *)tAlmHisLog, (UINT)iCnt, uiShort, ofp );
		}
	}

	return 0;
}

