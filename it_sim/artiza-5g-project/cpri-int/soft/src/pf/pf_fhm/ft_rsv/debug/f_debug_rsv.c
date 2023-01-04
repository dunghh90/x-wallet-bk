
#include "f_rsv_header.h"

//TODO FHMのSV制御レジスタのマップにあわせて変更が必要
void f_rsv_dbg_svState(FILE *outf)
{
	UINT regval;
	USHORT tra_sv_newsta[14];

	fprintf(outf,"------- current SV state  -------\n");
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV1INF,&regval);
	tra_sv_newsta[0] = regval & 0xffff;
	tra_sv_newsta[1] = (regval >> 16) & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV2INF,&regval);
	tra_sv_newsta[2] = regval & 0xffff;
	tra_sv_newsta[3] = (regval >> 16) & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV3INF,&regval);
	tra_sv_newsta[4] = regval & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV4INF,&regval);
	tra_sv_newsta[5] = regval & 0xffff;
	tra_sv_newsta[6] = (regval >> 16) & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV5INF,&regval);
	tra_sv_newsta[7] = regval & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV6INF,&regval);
	tra_sv_newsta[8] = regval & 0xffff;
	tra_sv_newsta[9] = (regval >> 16) & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV7INF,&regval);
	tra_sv_newsta[10] = regval & 0xffff;
	tra_sv_newsta[11] = (regval >> 16) & 0xffff;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_RRH_REG_CNT_SV8INF,&regval);
	tra_sv_newsta[12] = regval & 0xffff;
	tra_sv_newsta[13] = (regval >> 16) & 0xffff;

	fprintf(outf,"sv[00]:%04x:",tra_sv_newsta[0]);
	if(tra_sv_newsta[0] & D_RRH_INS_BIT)
	fprintf(outf," INS");
	if(tra_sv_newsta[0] & D_RRH_ACT_BIT)
	fprintf(outf," ACT");
	if(tra_sv_newsta[0] & D_RRH_BLK_BIT)
	fprintf(outf," BLK");
	if(tra_sv_newsta[0] & D_RRH_TST_BIT)
	fprintf(outf," TST");
	if(tra_sv_newsta[0] & 4)
	fprintf(outf," USE");
	if(tra_sv_newsta[0] & 2)
	fprintf(outf," ALM");
	if(tra_sv_newsta[0] & 1)
	fprintf(outf," ERR");
	fprintf(outf,"\n");

	fprintf(outf,"sv[01]:%04x",tra_sv_newsta[1]);
	if(tra_sv_newsta[1] & D_RRH_BR0USE_C0_BIT)
	fprintf(outf," BR0USE C0");
	if(tra_sv_newsta[1] & D_RRH_BR0USE_C1_BIT)
	fprintf(outf," BR0USE C1");
	if(tra_sv_newsta[1] & D_RRH_BR1USE_C0_BIT)
	fprintf(outf," BR1USE C0");
	if(tra_sv_newsta[1] & D_RRH_BR1USE_C1_BIT)
	fprintf(outf," BR1USE C1");
	if(tra_sv_newsta[1] & D_RRH_BR2USE_C0_BIT)
	fprintf(outf," BR2USE C0");
	if(tra_sv_newsta[1] & D_RRH_BR2USE_C1_BIT)
	fprintf(outf," BR2USE C1");
	if(tra_sv_newsta[1] & D_RRH_BR3USE_C0_BIT)
	fprintf(outf," BR3USE C0");
	if(tra_sv_newsta[1] & D_RRH_BR3USE_C1_BIT)
	fprintf(outf," BR3USE C1");
	fprintf(outf,"\n");

	fprintf(outf,"sv[02]:%04x\n",tra_sv_newsta[2]);
	fprintf(outf,"sv[03]:%04x\n",tra_sv_newsta[3]);
	fprintf(outf,"sv[04]:%04x\n",tra_sv_newsta[4]);
	fprintf(outf,"sv[05]:%04x\n",tra_sv_newsta[5]);
	fprintf(outf,"sv[06]:%04x\n",tra_sv_newsta[6]);
	fprintf(outf,"sv[07]:%04x\n",tra_sv_newsta[7]);
	fprintf(outf,"sv[08]:%04x\n",tra_sv_newsta[8]);
	fprintf(outf,"sv[09]:%04x\n",tra_sv_newsta[9]);
	fprintf(outf,"sv[10]:%04x\n",tra_sv_newsta[10]);
	fprintf(outf,"sv[11]:%04x\n",tra_sv_newsta[11]);
	fprintf(outf,"sv[12]:%04x\n",tra_sv_newsta[12]);
	fprintf(outf,"sv[13]:%04x\n",tra_sv_newsta[13]);

	return;
}		

void f_rsv_dbg_svHist(FILE *outf)
{
	UINT	loopcnt;
	UINT	loopcnt2;
	INT	uiRet;
	INT	errcd;
	T_RRH_SVSTATECHG_HIS *f_debg_svStateChg_his = 0;
	UINT	index;

	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_SV_STATE_HISTORY,(VOID**)&f_debg_svStateChg_his, &errcd );
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET failed [%d]", errcd );
		return;
	}

	fprintf(outf,"------- SV state changing history -------\n");
	index = f_debg_svStateChg_his->logIndex;
	for(loopcnt = 0 ,loopcnt2 = 0; loopcnt < D_RRH_SVLOG_MAX ; loopcnt++ , index++)
	{
		index %= D_RRH_SVLOG_MAX;
		if (
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.year == 0 )   ||
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.month == 0 )  ||
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.day == 0 )    ||
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.year > 2099 ) ||
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.month > 12)   ||
			( f_debg_svStateChg_his->svStateRecord[index].sysTime.day > 31 )
		)
		{
			continue;
		}
		loopcnt2++;
		fprintf(outf,"[%04d] %04d/%02d/%02d %02d:%02d:%02d:%02d0(%03d.%03d);",
					loopcnt2,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.year,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.month,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.day,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.hour,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.min,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.sec,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.msec,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.ucount/1000,
					f_debg_svStateChg_his->svStateRecord[index].sysTime.ucount%1000
					);

		switch(f_debg_svStateChg_his->svStateRecord[index].svStateLog.chgType)
		{
			case D_SYS_SVSTATE_IRQ:
				fprintf(outf,"IRQ;");
				break;
				
			case D_SYS_SVSTATE_SET:
				fprintf(outf,"SET;");
				break;
				
			case D_SYS_SVSTATE_CTL:
				fprintf(outf,"CTL;");
				break;
				
			case D_SYS_SVSTATE_CLR:
				fprintf(outf,"CLR;");
				break;
			default:
				fprintf(outf,"UNK;");
				break;
		}
		fprintf(outf,"SV01-%08x,SV02-%08x,SV03-%08x,SV04-%08x,SV05-%08x,SV06-%08x,SV07-%08x,SV08-%08x\n",
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[0],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[1],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[2],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[3],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[4],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[5],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[6],
			f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[7]);
		fprintf(outf,"SV09-%08x,SV10-%08x,SV11-%08x,SV12-%08x,SV13-%08x,SV14-%08x,SV15-%08x,SV16-%08x\n",
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[8],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[9],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[10],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[11],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[12],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[13],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[14],
		    f_debg_svStateChg_his->svStateRecord[index].svStateLog.svState[15]);
	}

	return;
}

void f_rsv_dbg_almSet(int almcode,FILE *outf)
{
	fprintf(outf,"alarm report %08x\n",almcode);

/*	f_com_almReport(D_SYS_THDID_PF_DBG,almcode);	*/

	return;
}

/**
 * @brief LED1 setting value
 * @note  レジスタから読み出した値をソフト内部の値に変換する
 * @note  2015/09/04 TDI)satou FHM用の定義に修正
 */
const	T_RSV_LEDINFO f_dbgr_led1Info[3] = {
		/*	LED state,					Set Value	*/
    {   D_RRH_LED1STATE_OFF,            D_RRH_CNT_FHM_LEDSYSR_OFF       },
    {   D_RRH_LED1STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSR_CYCL      },
    {   D_RRH_LED1STATE_ON,             D_RRH_CNT_FHM_LEDSYSR_ON        }
};

/**
 * @brief LED2 setting value
 * @note  レジスタから読み出した値をソフト内部の値に変換する
 * @note  2015/09/04 TDI)satou FHM用の定義に修正
 */
const	T_RSV_LEDINFO f_dbgr_led2Info[3] = {
		/*	LED state,					Set Value	*/
    {   D_RRH_LED2STATE_OFF,            D_RRH_CNT_FHM_LEDSYSG1_OFF       },
    {   D_RRH_LED2STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSG1_CYCL      },
    {   D_RRH_LED2STATE_ON,             D_RRH_CNT_FHM_LEDSYSG1_ON        }
};

/**
 * @brief LED3 setting value
 * @note  レジスタから読み出した値をソフト内部の値に変換する
 * @note  2015/09/04 TDI)satou FHM用の定義に修正
 */
const   T_RSV_LEDINFO f_dbgr_led3Info[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_LED3STATE_OFF,            D_RRH_CNT_FHM_LEDSYSO_OFF       },
    {   D_RRH_LED3STATE_BLNK,           D_RRH_CNT_FHM_LEDSYSO_CYCL      },
    {   D_RRH_LED3STATE_ON,             D_RRH_CNT_FHM_LEDSYSO_ON        }
};

/**
 * @brief CPRI LINK LED(ACT LED) setting value
 * @note  レジスタから読み出した値をソフト内部の値に変換する
 * @date  2015/09/05 TDI)satou create
 */
const   T_RSV_LEDINFO f_dbgr_ledCpriActInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_ACT_OFF,     D_RRH_CNT_FHM_ACTLED_OFF        },
    {   D_RRH_CPRILEDSTATE_ACT_BLNK,    D_RRH_CNT_FHM_ACTLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_ACT_ON,      D_RRH_CNT_FHM_ACTLED_ON         }
};

/**
 * @brief CPRI LINK LED(DWN LED) setting value
 * @note  レジスタから読み出した値をソフト内部の値に変換する
 * @date  2015/09/05 TDI)satou create
 */
const   T_RSV_LEDINFO f_dbgr_ledCpriDwnInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_DWN_OFF,     D_RRH_CNT_FHM_DWNLED_OFF        },
    {   D_RRH_CPRILEDSTATE_DWN_BLNK,    D_RRH_CNT_FHM_DWNLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_DWN_ON,      D_RRH_CNT_FHM_DWNLED_ON         }
};

/**
 * @brief  display LED state
 * @param  outf [in] file pointer
 * @return none
 * @date   2015/09/04 TDI)satou LED1-3をFHM用に修正
 */
void f_rsv_dbg_ledstate(FILE *outf)
{
	USHORT usLoop;
	UINT	regdat;		/* register data */
	UINT	led1Sta = 0xFFFFFFFF;		/* LED1 state 	*/
	UINT	led2Sta = 0xFFFFFFFF;		/* LED2 state	*/
	UINT    led3Sta = 0xFFFFFFFF;       /* LED3 state   */
	UINT	led1Val = D_SYS_NUM0;
	UINT	led2Val = D_SYS_NUM0;
	UINT    led3Val = D_SYS_NUM0;

	fprintf(outf,"-------- Show LED state --------\n");

	/* Get register data from LED control register */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
				D_RRH_REG_CNT_SYSLED, &regdat);
	led1Val = regdat & D_RRH_CNT_FHM_LEDSYSR_MASK;
	led2Val = regdat & D_RRH_CNT_FHM_LEDSYSG1_MASK;
	led3Val = regdat & D_RRH_CNT_FHM_LEDSYSO_MASK;

	for(usLoop = 0; usLoop < (sizeof(f_dbgr_led1Info)/sizeof(f_dbgr_led1Info[0])); usLoop++)
	{
		if(led1Val == f_dbgr_led1Info[usLoop].setVal)
		{
			led1Sta = f_dbgr_led1Info[usLoop].ledSta;
			break;
		}
	}

	for(usLoop = 0; usLoop < (sizeof(f_dbgr_led2Info)/sizeof(f_dbgr_led2Info[0])); usLoop++)
	{
		if(led2Val == f_dbgr_led2Info[usLoop].setVal)
		{
			led2Sta = f_dbgr_led2Info[usLoop].ledSta;
			break;
		}
	}

	for(usLoop = 0; usLoop < (sizeof(f_dbgr_led3Info)/sizeof(f_dbgr_led3Info[0])); usLoop++) {
	    if(led3Val == f_dbgr_led3Info[usLoop].setVal)
	    {
	        led3Sta = f_dbgr_led3Info[usLoop].ledSta;
	        break;
	    }
	}

	fprintf(outf,"LED1 state is ");
	switch(led1Sta)
	{
	    case D_RRH_LED1STATE_OFF:
			fprintf(outf,"OFF; \n");
			break;
	    case D_RRH_LED1STATE_BLNK:
			fprintf(outf,"Blink; \n");
			break;
	    case D_RRH_LED1STATE_ON:
	        fprintf(outf,"ON; \n");
	        break;
		default:
			fprintf(outf,"ERROR; \n");
			break;
	}
	fprintf(outf,"LED2 state is ");
	switch(led2Sta)
	{
	    case D_RRH_LED2STATE_OFF:
	        fprintf(outf,"OFF; \n");
	        break;
	    case D_RRH_LED2STATE_BLNK:
	        fprintf(outf,"Blink; \n");
	        break;
	    case D_RRH_LED2STATE_ON:
	        fprintf(outf,"ON; \n");
	        break;
	    default:
	        fprintf(outf,"ERROR; \n");
	        break;
	}
	fprintf(outf,"LED3 state is ");
	switch(led3Sta)
	{
	    case D_RRH_LED3STATE_OFF:
	        fprintf(outf,"OFF; \n");
	        break;
	    case D_RRH_LED3STATE_BLNK:
	        fprintf(outf,"Blink; \n");
	        break;
	    case D_RRH_LED3STATE_ON:
	        fprintf(outf,"ON; \n");
	        break;
	    default:
	        fprintf(outf,"ERROR; \n");
	        break;
	}
	
	return;
}

/**
 * @brief  display CPRI LINK LED state
 * @param  outf [in] file pointer
 * @return none
 * @date   2015/09/05 TDI)satou create
 */
void f_rsv_dbg_cpriledstate(FILE *outf)
{
    USHORT  usLoop;             /* for loop */
    UINT    regdat;             /* register data */
    UINT    actLedVal;          /* register data (mask) */
    UINT    dwnLedVal;          /* register data (mask) */
    UINT    actLedSta;          /* CPRI LINK LED(ACT LED) state */
    UINT    dwnLedSta;          /* CPRI LINK LED(DWN LED) state */
    UINT    linkno;             /* CPRIリンク番号 */

    fprintf(outf,"-------- Show CPRI LED state --------\n");

    for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
        /* Get register data from LED control register */
        BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_RRH_REG_CNT_CPSLED(linkno), &regdat);
        actLedVal = regdat & D_RRH_CNT_FHM_ACTLED_MASK;
        dwnLedVal = regdat & D_RRH_CNT_FHM_DWNLED_MASK;

        for (usLoop = 0; usLoop < (sizeof(f_dbgr_ledCpriActInfo)/sizeof(f_dbgr_ledCpriActInfo[0])); usLoop++) {
            if (actLedVal == f_dbgr_ledCpriActInfo[usLoop].setVal) {
                actLedSta = f_dbgr_ledCpriActInfo[usLoop].ledSta;
                break;
            }
        }

        for (usLoop = 0; usLoop < (sizeof(f_dbgr_ledCpriDwnInfo)/sizeof(f_dbgr_ledCpriDwnInfo[0])); usLoop++) {
            if (dwnLedVal == f_dbgr_ledCpriDwnInfo[usLoop].setVal) {
                dwnLedSta = f_dbgr_ledCpriDwnInfo[usLoop].ledSta;
                break;
            }
        }

        fprintf(outf,"CPRI LINK LED(ACT) #%02d is ", linkno);
        switch (actLedSta) {
        case D_RRH_CPRILEDSTATE_ACT_OFF:
            fprintf(outf,"OFF; \n");
            break;
        case D_RRH_CPRILEDSTATE_ACT_BLNK:
            fprintf(outf,"Blink; \n");
            break;
        case D_RRH_CPRILEDSTATE_ACT_ON:
            fprintf(outf,"ON; \n");
            break;
        default:
            fprintf(outf,"ERROR; \n");
            break;
        }

        fprintf(outf,"CPRI LINK LED(DWN) #%02d is ", linkno);
        switch (dwnLedSta) {
        case D_RRH_CPRILEDSTATE_DWN_OFF:
            fprintf(outf,"OFF; \n");
            break;
        case D_RRH_CPRILEDSTATE_DWN_BLNK:
            fprintf(outf,"Blink; \n");
            break;
        case D_RRH_CPRILEDSTATE_DWN_ON:
            fprintf(outf,"ON; \n");
            break;
        default:
            fprintf(outf,"ERROR; \n");
            break;
        }
    }
}

INT dbgcmd_exec_pf_rsv(char * cmdStr, FILE *outf)
{
	UINT						argcCnt;
	CHAR						*argvParam[4];
	CHAR						*getToken, *savePtr;
	CHAR						*delimiter = " ";
	INT							num;

	getToken = cmdStr;
	for( argcCnt=0; argcCnt < 4; argcCnt++ )
	{
		argvParam[ argcCnt ] = strtok_r( getToken, delimiter, &savePtr );
		if( NULL == argvParam[ argcCnt ] )
		{
			break;
		}
		
		/* 2�p�����[�^�ȍ~�� NULL�w��Ŏ��o�� */
		getToken = NULL;
	}
	
	if( argcCnt < 4 )
	{
		fprintf(outf,"please intput\n,led\nsvStat\n");
		return D_RRH_NG;
	}

	if(strcmp(argvParam[ 2 ],"alm" ) == 0)
	{
		for(num = 0; num < strlen(argvParam[ 3 ]);num++)
		{
			if((argvParam[ 3 ][num] < '0') || (argvParam[ 3 ][num] > '9'))
			{
				fprintf(outf,"alrm code is dec ");
				return D_RRH_OK;
			}
		}

		f_rsv_dbg_almSet(atoi(argvParam[ 3 ]),outf);
	}
	else if(strcmp(argvParam[ 2 ],"dsp" ) == 0)
	{
		if(strcmp(argvParam[ 3 ],"led") == 0)
		{
			f_rsv_dbg_ledstate(outf);
			f_rsv_dbg_cpriledstate(outf);
		}
		else if(strcmp(argvParam[ 3 ],"svstat") == 0)
		{
			f_rsv_dbg_svState(outf);
		}
		else if(strcmp(argvParam[ 3 ],"svhist") == 0)
		f_rsv_dbg_svHist(outf);
		else
		fprintf(outf,"please intput\n 1.led\n2.svstat\n3.svhist\n");
	}
	else
	fprintf(outf,"please intput\n 1.alm\n2.dsp\n");

	return D_RRH_OK;
}

/* rsv���o�R�������ڎ��s���� */
INT dbgcmd_exec_direct_rsv_dsp_svhist( int argc, char *argv[], FILE *outf  )
{
	f_rsv_dbg_svHist(outf);
	return D_RRH_OK;
}


