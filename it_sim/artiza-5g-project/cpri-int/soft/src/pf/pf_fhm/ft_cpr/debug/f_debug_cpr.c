
#include "f_rrh_inc.h"
#include "f_cpr_inc.h"

/**
 * @brief  CPRI L1リンク障害ログテーブル出力
 * @note   共有メモリの内容を指定されたファイルポインタに出力します
 * @param  outf [in] 出力先のファイルポインタ
 * @param  cprino [in] CPRIリンク番号
 * @param  countNum [in]
 * @param  infoNum [in]
 * @return none
 * @date   2015/09/02 TDI)satou 引数にCPRIリンク番号を追加し、指定された番号の内容を出力するように
 */
void f_cpr_dbg_l1Err(FILE *outf,int cprino,int countNum,int infoNum)
{
	INT ret;
	INT errcd;
	int print_num;
	unsigned int print_pos;
	int state_cout;
	int addr_num;
	int count;
	T_RRH_L1ERR_TBL *tbl = 0;

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_LINK_LOG,(VOID**)&tbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(outf,"sharemeory E_RRH_SHMID_APL_CPRI_L1_LINK_LOG NG \n");
		return;
	}

	if(countNum > D_RRH_L1_ERR_COUNT_SIZE)
	{
		countNum = D_RRH_L1_ERR_COUNT_SIZE;
	}
	print_num = tbl[cprino].count_Idx - countNum;
	if( print_num < 0 )
	{
		print_num += D_RRH_L1_ERR_COUNT_SIZE + 1;
	}
	print_pos = print_num;
	count = 1;
	fprintf(outf,"CPRI#%d Per 10min L1 Link error statistic summary \n", cprino);
	fprintf(outf,"-------------------------------------------------------------------------------------------------------------------------\n");
	for(print_num = 0; print_num < countNum;print_num++ , print_pos++)
	{
		print_pos %= D_RRH_L1_ERR_COUNT_SIZE;
		if(
		 (tbl[cprino].countTbl[print_pos].firstTime.year == 0) ||
		 (tbl[cprino].countTbl[print_pos].firstTime.year > 2099)	/* RTC max val */
		)
		{
			continue;
		}
		fprintf(outf,"No.%03d;%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d);\n",
		count++,
		tbl[cprino].countTbl[print_pos].firstTime.year,
		tbl[cprino].countTbl[print_pos].firstTime.month,
		tbl[cprino].countTbl[print_pos].firstTime.day,
		tbl[cprino].countTbl[print_pos].firstTime.hour,
		tbl[cprino].countTbl[print_pos].firstTime.min,
		tbl[cprino].countTbl[print_pos].firstTime.sec,
		tbl[cprino].countTbl[print_pos].firstTime.msec,
		tbl[cprino].countTbl[print_pos].firstTime.ucount / 1000,
		tbl[cprino].countTbl[print_pos].firstTime.ucount % 1000
		);
		for(state_cout = 0;state_cout < 6;state_cout++)
		{
						
			switch(state_cout)
			{
				case D_RRH_CPRISTAT_A:
					fprintf(outf,"+ State A ; ");
					break;
				case D_RRH_CPRISTAT_B:
					fprintf(outf,"State B ; ");
					break;						
				case D_RRH_CPRISTAT_C:
					fprintf(outf,"+ State C ; ");
					break;
				case D_RRH_CPRISTAT_D:
					fprintf(outf,"State D ; ");
					break;
				case D_RRH_CPRISTAT_E:
					fprintf(outf,"+ State E ; ");
					break;						
				case D_RRH_CPRISTAT_F:
					fprintf(outf,"State F ; ");
					break;
				default :
					break;
			}
			fprintf(outf,"%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d) ; %08x ;",
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.year,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.month,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.day,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.hour,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.min,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.sec,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.msec,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.ucount / 1000,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].lastTime.ucount % 1000,
				tbl[cprino].countTbl[print_pos].stateErr[state_cout].count);
				if(state_cout == 1 || state_cout == 3 || state_cout == 5)
					fprintf(outf,"\n");
		}

        fprintf(outf,"+ CPERR-%08x;LOSNUM-%08x;RL1NB-%08x;CPSTCHG-%08x;CPSYNC-%08x;HDLC1-%08x;HDLC2-%08x;HDLC3-%08x;EMI1-%08x;BITRATE-%08x;\n",
				tbl[cprino].countTbl[print_pos].cpeErr,
				tbl[cprino].countTbl[print_pos].los,
				tbl[cprino].countTbl[print_pos].rl1Inb,
                tbl[cprino].countTbl[print_pos].stateChg,
                tbl[cprino].countTbl[print_pos].hfnsyncChg,
				tbl[cprino].countTbl[print_pos].hdlcErr1,
				tbl[cprino].countTbl[print_pos].hdlcErr2,
				tbl[cprino].countTbl[print_pos].hdlcErr3,
				tbl[cprino].countTbl[print_pos].emio1,
				tbl[cprino].countTbl[print_pos].emio2);

		fprintf(outf,"-------------------------------------------------------------------------------------------------------------------------\n");
	}

	if(infoNum > D_RRH_L1_ERR_INFO_SIZE)
	{
		infoNum = D_RRH_L1_ERR_INFO_SIZE;
	}
	else if(infoNum < 1)
	{
		infoNum = 32;
	}
	else
	{
		;
	}
	print_num = tbl[cprino].Info_Idx - infoNum ;
	if( print_num < 0 )
	{
		print_num += D_RRH_L1_ERR_INFO_SIZE + 1;
	}
	print_pos = print_num;
	count = 1;
	fprintf(outf,"CPRI#%d Current 10 min L1 Link error statistic\n", cprino);
	fprintf(outf,"-------------------------------------------------------------------------------------------------------------------------\n");
	for(print_num = 0; print_num < infoNum;print_num++ , print_pos++)
	{
		print_pos %= D_RRH_L1_ERR_INFO_SIZE;
		if(
			(tbl[cprino].infoTbl[print_pos].time.year == 0) ||
			(tbl[cprino].infoTbl[print_pos].time.year > 2099)
		)
		{
			continue;
		}
		fprintf(outf,"No.%04d;%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d);",
		count++,
		tbl[cprino].infoTbl[print_pos].time.year,
		tbl[cprino].infoTbl[print_pos].time.month,
		tbl[cprino].infoTbl[print_pos].time.day,
		tbl[cprino].infoTbl[print_pos].time.hour,
		tbl[cprino].infoTbl[print_pos].time.min,
		tbl[cprino].infoTbl[print_pos].time.sec,
		tbl[cprino].infoTbl[print_pos].time.msec,
		tbl[cprino].infoTbl[print_pos].time.ucount/1000,
		tbl[cprino].infoTbl[print_pos].time.ucount%1000
		);
		/* old */
		switch((UCHAR)((tbl[cprino].infoTbl[print_pos].state >> 16) & 0x000000FF))
		{
			case D_RRH_CPRISTAT_A:
				fprintf(outf,"State A->");
				break;
			case D_RRH_CPRISTAT_B:
				fprintf(outf,"State B->");
				break;						
			case D_RRH_CPRISTAT_C:
				fprintf(outf,"State C->");
				break;
			case D_RRH_CPRISTAT_D:
				fprintf(outf,"State D->");
				break;
			case D_RRH_CPRISTAT_E:
				fprintf(outf,"State E->");
				break;						
			case D_RRH_CPRISTAT_F:
				fprintf(outf,"State F->");
				break;
			default:
				fprintf(outf,"State ?->");
				break;
		}
		/* new */
		switch((UCHAR)(tbl[cprino].infoTbl[print_pos].state & 0x000000FF))
		{
			case D_RRH_CPRISTAT_A:
				fprintf(outf,"State A;\n");
				break;
			case D_RRH_CPRISTAT_B:
				fprintf(outf,"State B;\n");
				break;						
			case D_RRH_CPRISTAT_C:
				fprintf(outf,"State C;\n");
				break;
			case D_RRH_CPRISTAT_D:
				fprintf(outf,"State D;\n");
				break;
			case D_RRH_CPRISTAT_E:
				fprintf(outf,"State E;\n");
				break;						
			case D_RRH_CPRISTAT_F:
				fprintf(outf,"State F;\n");
				break;
			default:
				fprintf(outf,"State ?;\n");
				break;
		}

        fprintf(outf,"+ CPERR-%08x;LOSNUM-%08x;RL1NB-%08x;RXBUFF-%08x;TXBUFF-%08x;HDLC1-%08x;HDLC2-%08x;HDLC3-%08x;EMI1-%08x;BITRATE-%08x;HSYNC-%08x"
                     "  PLLST-%08x;SFPST-%08x;CPSTCHG-%08x;CPSYNC-%08x;\n",
		tbl[cprino].infoTbl[print_pos].cpeErr,
		tbl[cprino].infoTbl[print_pos].los,
		tbl[cprino].infoTbl[print_pos].rl1Inb,
        tbl[cprino].infoTbl[print_pos].rxBuff,	/*	★調査用ログ強化★	*/
        tbl[cprino].infoTbl[print_pos].txBuff,	/*	★調査用ログ強化★	*/
		tbl[cprino].infoTbl[print_pos].hdlcErr1,
		tbl[cprino].infoTbl[print_pos].hdlcErr2,
		tbl[cprino].infoTbl[print_pos].hdlcErr3,
		tbl[cprino].infoTbl[print_pos].emio1,
		tbl[cprino].infoTbl[print_pos].emio2,
		tbl[cprino].infoTbl[print_pos].hfnsync,
		tbl[cprino].infoTbl[print_pos].pllSt,
		tbl[cprino].infoTbl[print_pos].sfpSt,
		tbl[cprino].infoTbl[print_pos].cpstChg,
		tbl[cprino].infoTbl[print_pos].cpSync
		);

		fprintf(outf,"+ SFP(0x38-0x57):");
		for(addr_num = 0; addr_num < 32 ;addr_num++)
		{
			fprintf(outf,"%02x,",tbl[cprino].infoTbl[print_pos].sfp[addr_num]);
		}
		fprintf(outf,"\n");
		fprintf(outf,"+ SFP(0x58-0x77):");
		for(addr_num = 32; addr_num < 64 ;addr_num++)
		{
			fprintf(outf,"%02x,",tbl[cprino].infoTbl[print_pos].sfp[addr_num]);
		}
		fprintf(outf,"\n");

		fprintf(outf,"-------------------------------------------------------------------------------------------------------------------------\n");
	}

	return;
}	

/**
 * @brief  CPRI L1復旧断履歴テーブル出力
 * @note   共有メモリの内容を指定されたファイルポインタに出力します
 * @param  outf [in] 出力先のファイルポインタ
 * @param  cprino [in] CPRIリンク番号
 * @param  countNum [in]
 * @return none
 * @date   2015/09/02 TDI)satou 引数にCPRIリンク番号を追加し、指定された番号の内容を出力するように
 */
void f_cpr_dbg_l1Stat(FILE *outf,int cprino,int countNum)
{
	INT ret;
	INT errcd;
	int print_num;
	unsigned int print_pos;
	int count;
	T_RRH_L1LOSLOF_TBL	*tbl = 0;

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_LOSLOF_HIS,(VOID**)&tbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(outf,"sharemeory E_RRH_SHMID_APL_CPRI_L1_LINK_LOG NG %d \n",errcd);
		return;
	}

	if(countNum > D_RRH_L1_LOSLOF_SIZE)
		countNum = D_RRH_L1_LOSLOF_SIZE;
	if(countNum < 1)
		countNum = 128;

	print_num = tbl[cprino].index - countNum;
	if( print_num < 0 )
	{
		print_num += D_RRH_L1_LOSLOF_SIZE;
	}
	print_pos = print_num;
	count = 1;
	fprintf(outf,"CPRI#%d L1 Link LOSLOF statistic\n", cprino);
	for(print_num = 0; print_num < countNum;print_num++,print_pos++)
	{
		print_pos %= D_RRH_L1_LOSLOF_SIZE;
		if(
		( tbl[cprino].hisInfo[print_pos].time.year == 0) ||
		( tbl[cprino].hisInfo[print_pos].time.year > 2099 )
		)
		{
			continue;
		}
		fprintf(outf,"No.%04d;%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d);STATUS-%08x,TRANS-%08x,IRQ-%08x\n", 
			count++,
			tbl[cprino].hisInfo[print_pos].time.year,
			tbl[cprino].hisInfo[print_pos].time.month,
			tbl[cprino].hisInfo[print_pos].time.day,
			tbl[cprino].hisInfo[print_pos].time.hour,
			tbl[cprino].hisInfo[print_pos].time.min,
			tbl[cprino].hisInfo[print_pos].time.sec,
			tbl[cprino].hisInfo[print_pos].time.msec,
			tbl[cprino].hisInfo[print_pos].time.ucount / 1000,
			tbl[cprino].hisInfo[print_pos].time.ucount % 1000,
			tbl[cprino].hisInfo[print_pos].cpristat,
			tbl[cprino].hisInfo[print_pos].cptrans,
			tbl[cprino].hisInfo[print_pos].irqvalue
		);
	}

	return;
}

/**
 * @brief  CPRI L1統計情報テーブル出力
 * @note   共有メモリの内容を指定されたファイルポインタに出力します
 * @param  outf [in] 出力先のファイルポインタ
 * @param  cprino [in] CPRIリンク番号
 * @return 処理結果
 * @retval D_RRH_OK : 正常
 * @retval D_RRH_NG : 異常
 * @date   2015/09/02 TDI)satou 引数にCPRIリンク番号を追加し、指定された番号の内容を出力するように
 */
INT f_cpr_dbg_l1Num(FILE *outf,int cprino)
{
	INT ret;
	INT errcd;
	UINT index;
	char *strStat[] = {
		"STATE A  ","STATE B  ","STATE C  ","STATE D  ","STATE E  ","STATE F  ","HDLC ACT ","HDLC IDLE"
		};
	T_RRH_L1STAT_TBL *tbl = 0;
	UINT	count;
	UINT	blkindex;
	

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_STAT_INFO,(VOID**)&tbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(outf,"sharemeory E_RRH_SHMID_APL_CPRI_L1_STAT_INFO NG \n");
		return D_RRH_NG;
	}
	fprintf(outf,"CPRI#%d L1 Link state statistic\n", cprino);
	blkindex = ( tbl[cprino].index + 1 ) % D_RRH_L1_STATI_L1BLK_NUM;
	for( count = 0 ; count < D_RRH_L1_STATI_L1BLK_NUM ; count++ , blkindex++ )
	{
		blkindex %= D_RRH_L1_STATI_L1BLK_NUM;
		fprintf(outf,"[No.%02d(%02d)]\n",(UCHAR)(count+1),(UCHAR)(blkindex));
		for(index = 0;index < D_RRH_L1_STATI_L1SIG_NUM;index++)
		{
			if(tbl[cprino].blks[blkindex][index].firstTime_flag == D_SYS_OFF)
			{
				continue;
			}
			fprintf(outf,"[%s][B:%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d)]->[E:%04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d)] occur:%08X,over:%08X \n", 
					strStat[index],
					tbl[cprino].blks[blkindex][index].firstTime.year,
					tbl[cprino].blks[blkindex][index].firstTime.month,
					tbl[cprino].blks[blkindex][index].firstTime.day,
					tbl[cprino].blks[blkindex][index].firstTime.hour,
					tbl[cprino].blks[blkindex][index].firstTime.min,
					tbl[cprino].blks[blkindex][index].firstTime.sec,
					tbl[cprino].blks[blkindex][index].firstTime.msec,
					tbl[cprino].blks[blkindex][index].firstTime.ucount/1000,
					tbl[cprino].blks[blkindex][index].firstTime.ucount%1000,
					tbl[cprino].blks[blkindex][index].lastTime.year,
					tbl[cprino].blks[blkindex][index].lastTime.month,
					tbl[cprino].blks[blkindex][index].lastTime.day,
					tbl[cprino].blks[blkindex][index].lastTime.hour,
					tbl[cprino].blks[blkindex][index].lastTime.min,
					tbl[cprino].blks[blkindex][index].lastTime.sec,
					tbl[cprino].blks[blkindex][index].lastTime.msec,
					tbl[cprino].blks[blkindex][index].lastTime.ucount/1000,
					tbl[cprino].blks[blkindex][index].lastTime.ucount%1000,
					tbl[cprino].blks[blkindex][index].occur_counter,
					tbl[cprino].blks[blkindex][index].overflow_counter
			);
		}
	}

	return D_RRH_OK;
}

/**
 * @brief  Command process.
 * @param  cmdStr [in] Pointer of command string
 * @param  outf   [in] Pointer of output file
 * @return Result code is returned
 * @date   2015/09/02 TDI)satou CPRIリンク番号を引数にとるように
 */
INT dbgcmd_exec_pf_cpr(char * cmdStr, FILE *outf)
{
	UINT						argcCnt;
	CHAR						*argvParam[7];
	CHAR						*getToken, *savePtr;
	CHAR						*delimiter = " ";
	INT							num = 0;
	INT							num2 = 0;
	INT                         cprino;                 /* CPRIリンク番号 */

	getToken = cmdStr;
	for( argcCnt=0; argcCnt < 7; argcCnt++ )
	{
		argvParam[ argcCnt ] = strtok_r( getToken, delimiter, &savePtr );
		if( NULL == argvParam[ argcCnt ] )
		{
			break;
		}
		
		/* 2パラメータ以降は NULL指定で取り出す */
		getToken = NULL;
	}
	
	if( argcCnt < 5 )
	{
		fprintf(outf,"please intput\n <cprino> 1.linkhis\n2.trace\n,cprino : 0-16\n");
		return D_RRH_NG;
	}

	cprino = atoi(argvParam[ 1 ]);
	if ((D_RRH_CPRINO_REC > cprino) || (D_RRH_CPRINO_RE_MAX < cprino)) {
	    fprintf(outf,"please input\n,cprino : 0-16\n");
	    return D_RRH_NG;
	}

	if(strcmp(argvParam[ 2 ],"dsp" ) == 0)
	{
		if(strcmp(argvParam[ 4 ],"hist") == 0)
		{
			if(argcCnt > 5)num = atoi(argvParam[ 5 ]);
			f_cpr_dbg_l1Stat(outf,cprino,num);
		}
		else if(strcmp(argvParam[ 4 ],"trace") == 0)
		{
			if(argcCnt > 5)num = atoi(argvParam[ 5 ]);
			if(argcCnt > 6)num2 = atoi(argvParam[ 6 ]);
			f_cpr_dbg_l1Err(outf,cprino,num,num2);
		}
		else if(strcmp(argvParam[ 4 ],"sum") == 0)
			f_cpr_dbg_l1Num(outf,cprino);
		else
		fprintf(outf,"please intput\n 1.linkhis\n2.trace\n");
	}
	else
	fprintf(outf,"please intput dsp\n");

	return D_RRH_OK;
}

/**
 * @brief  CPRデバッグコマンド直接実行
 * @note   用途は、障害ログ出力
 * @note   障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 * @return 処理結果(正常固定)
 * @retval D_RRH_OK
 * @date   2015/09/02 TDI)satou CPRIリンク番号でループするように
 */
INT dbgcmd_exec_direct_cpr_l1_dsp_hist( int argc, char *argv[], FILE *outf  )
{
    int cprino;     /* CPRIリンク番号 */

    for (cprino = 0; cprino < D_RRH_CPRINO_NUM; cprino++) {
        f_cpr_dbg_l1Stat(outf,cprino,D_RRH_L1_LOSLOF_SIZE);
    }
	return D_RRH_OK;
}

/**
 * @brief  CPRデバッグコマンド直接実行
 * @note   用途は、障害ログ出力
 * @note   障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet(())
 * @return 処理結果(正常固定)
 * @retval D_RRH_OK
 * @date   2015/09/02 TDI)satou CPRIリンク番号でループするように
 */
INT dbgcmd_exec_direct_cpr_l1_dsp_trace( int argc, char *argv[], FILE *outf  )
{
    int cprino;     /* CPRIリンク番号 */

    for (cprino = 0; cprino < D_RRH_CPRINO_NUM; cprino++) {
        f_cpr_dbg_l1Err(outf,cprino,0,256);
    }
	return D_RRH_OK;
}

/**
 * @brief  CPRデバッグコマンド直接実行
 * @note   用途は、障害ログ出力
 * @note   障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 * @return 処理結果(正常固定)
 * @retval D_RRH_OK
 * @date   2015/09/02 TDI)satou CPRIリンク番号でループするように
 */
INT dbgcmd_exec_direct_cpr_l1_dsp_sum( int argc, char *argv[], FILE *outf  )
{
    int cprino;     /* CPRIリンク番号 */

    for (cprino = 0; cprino < D_RRH_CPRINO_NUM; cprino++) {
        f_cpr_dbg_l1Num(outf,cprino);
    }
	return D_RRH_OK;
}

/**
 * @brief  CPRデバッグコマンド直接実行
 * @note   用途は、障害ログ出力
 * @note   障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 * @return 処理結果(正常固定)
 * @retval D_RRH_OK
 * @date   2015/12/01 TDI)satou
 */
INT dbgcmd_exec_direct_cpr_l1_dsp_dly(int argc, char *argv[], FILE *outf)
{
    USHORT cpr_idx;
    UINT regAdr, regDat;
    T_RRH_DELAY_INF *tbl;
    INT errcd, ret;

    ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DELAY_INF, (VOID**)&tbl, &errcd);
    if(ret != BPF_RU_IPCM_OK)
    {
        fprintf(outf, "sharemeory E_RRH_SHMID_APL_DELAY_INF NG %d \n",errcd);
        return D_RRH_NG;
    }

    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_cp_cprtdly\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_cp_cprtdly[cpr_idx].regDat);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_cp_cprtdly[cpr_idx].regDat);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_shm_conn\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_conn.conn_disc[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_conn.conn_disc[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_shm_toffset_s3g\n");
    fprintf(outf, " (1) toffset\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].toffset);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].toffset);
    fprintf(outf, "\n");
    fprintf(outf, " (2) updeleqp\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].updeleqp);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].updeleqp);
    fprintf(outf, "\n");
    fprintf(outf, " (3) downdeleqp\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].downdeleqp);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_s3g[cpr_idx].downdeleqp);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_shm_toffset_3g\n");
    fprintf(outf, " (1) toffset\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].toffset);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].toffset);
    fprintf(outf, "\n");
    fprintf(outf, " (2) updeleqp\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].updeleqp);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].updeleqp);
    fprintf(outf, "\n");
    fprintf(outf, " (3) downdeleqp\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].downdeleqp);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %04X", tbl->f_cprw_shm_toffset_3g[cpr_idx].downdeleqp);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_REn_T14\n");
    fprintf(outf, " [base]\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_base_REn_T14[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_base_REn_T14[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " [now]\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_REn_T14[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_REn_T14[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_ReReport_Toffset\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_ReReport_Dl\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Dl[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_ReReport_Ul\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_ReReport_Ul[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_T12_RE\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_T12_RE.max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_T12_RE.max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_T12_RE.REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_T12_RE.REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_Xn\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Xn[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Xn[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_Yn\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Yn[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Yn[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_Xn_plus_T12_RE\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_MAX_Yn_plus_T12_RE\n");
    fprintf(outf, " (1)3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " (2)S3G\n");
    fprintf(outf, " max_re_num : %d\n",   tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max_re_num);
    fprintf(outf, " max        : %08X\n", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max);
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_inf\n");
    fprintf(outf, " MaxT12Re           : %04X\n", tbl->MaxT12Re);
    fprintf(outf, " MaxFiberDelay      : %08X\n", tbl->MaxFiberDelay);
    fprintf(outf, " (1) 3G\n");
    fprintf(outf, " MaxDlEquipDelayRe  : %d\n",   tbl->MaxDlEquipDelayReNo[CMD_SYS_3G]);
    fprintf(outf, " MaxUlEquipDelayRe  : %d\n",   tbl->MaxUlEquipDelayReNo[CMD_SYS_3G]);
    fprintf(outf, " MaxDlDelayRe       : %04X\n", tbl->MaxDlDelayRe[CMD_SYS_3G]);
    fprintf(outf, " MaxUlDelayRe       : %04X\n", tbl->MaxUlDelayRe[CMD_SYS_3G]);
    fprintf(outf, " MaxFhmDlDelay      : %08X\n", tbl->MaxFhmDlDelay[CMD_SYS_3G]);
    fprintf(outf, " MaxFhmUlDelay      : %08X\n", tbl->MaxFhmUlDelay[CMD_SYS_3G]);
    fprintf(outf, " toffsetRslt        : %d\n",   tbl->toffsetRslt[CMD_SYS_3G]);
    fprintf(outf, " (2) S3G\n");
    fprintf(outf, " MaxDlEquipDelayRe  : %d\n",   tbl->MaxDlEquipDelayReNo[CMD_SYS_S3G]);
    fprintf(outf, " MaxUlEquipDelayRe  : %d\n",   tbl->MaxUlEquipDelayReNo[CMD_SYS_S3G]);
    fprintf(outf, " MaxDlDelayRe       : %04X\n", tbl->MaxDlDelayRe[CMD_SYS_S3G]);
    fprintf(outf, " MaxUlDelayRe       : %04X\n", tbl->MaxUlDelayRe[CMD_SYS_S3G]);
    fprintf(outf, " MaxFhmDlDelay      : %08X\n", tbl->MaxFhmDlDelay[CMD_SYS_S3G]);
    fprintf(outf, " MaxFhmUlDelay      : %08X\n", tbl->MaxFhmUlDelay[CMD_SYS_S3G]);
    fprintf(outf, " toffsetRslt        : %d\n",   tbl->toffsetRslt[CMD_SYS_S3G]);
    fprintf(outf, "-------------------------------------------------------------------------\n");
    regAdr = D_RRH_REG_MIX_FHMTOFST;
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regDat);
    fprintf(outf, " Toffset_FHM        : %08X\n", regDat);
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " DIS_LDL\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_DIS_LDL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_DIS_LDL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " MIX_PMON\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_PMON(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_PMON(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " MIX_LUL\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_LUL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_LUL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " DIS_WDL\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_DIS_WDL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_DIS_WDL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " MIX_WUL\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_WUL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_MIX_WUL(cpr_idx+1), &regDat);
        fprintf(outf, " %08X", regDat);
    }
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    fprintf(outf, " f_cprw_dly_adjLUDly\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjLUDly[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjLUDly[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " f_cprw_dly_adjWUDly\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjWUDly[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjWUDly[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " f_cprw_dly_adjLDDly\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjLDDly[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjLDDly[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " f_cprw_dly_adjWDDly\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjWDDly[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_adjWDDly[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " f_cprw_dly_failsafe_S3G\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_failsafe_S3G[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_failsafe_S3G[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, " f_cprw_dly_failsafe_3G\n");
    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX/2; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_failsafe_3G[cpr_idx]);
    fprintf(outf, "\n");
    for (; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    fprintf(outf, " %08X", tbl->f_cprw_dly_failsafe_3G[cpr_idx]);
    fprintf(outf, "\n");
    fprintf(outf, "-------------------------------------------------------------------------\n");
    return D_RRH_OK;
}

