/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_cmdProcess.c
 *  @brief  command process
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
 
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"
#include "l2_dbg_inc.h"
#include "tm_lb_def.h"

#define DBGCMD_DATA_COLUMNS	16
const T_L2_L3SIGNAMETBL l2_comr_stati_l3sig_cmd[D_RRH_L2_STATI_L3SIG_NUM] = 
{
	/* signal */
	{	CMD_CPRID_HCREQ			,	"HealthCheckReq "	},
	{	CMD_CPRID_HCRES			,	"HealthCheckRsp "	},
	{	CMD_CPRID_RERSTREQ		,	"RE ResetReq    "	},
	{	CMD_CPRID_RETIMSETREQ	,	"RE TimeSetReq  "	},
	{	CMD_CPRID_LINKSTSREQ	,	"RE ComLnkSetReq"	},
	{	CMD_CPRID_LINKSTSRES	,	"RE ComLnkSetRsp"	},
	{	CMD_CPRID_FIRMFILEREQ	,	"FWFilInfReq    "	},
	{	CMD_CPRID_FIRMFILERES	,	"FWFilInfRsp    "	},
	{	CMD_CPRID_FILEINFREQ	,	"FilInfRepNtc   "	},
	{	CMD_CPRID_FILEINFRES	,	"FilInfRepRsp   "	},
	{	CMD_CPRID_FILEDATSND	,	"FilDatSnd      "	},
	{	CMD_CPRID_FILSNDFINNTC	,	"FilSndFinNtc   "	},
	{	CMD_CPRID_FILSNDFINRES	,	"FilSndFinRsp   "	},
	{	CMD_CPRID_UFIRMFILEREQ	,	"U FWFilInfReq  "	},
	{	CMD_CPRID_UFIRMFILERES	,	"U FWFilInfRsp  "	},
	{	CMD_CPRID_UFILEINFREQ	,	"U FilInfInqNtc "	},
	{	CMD_CPRID_UFILEINFRES	,	"U FilInfInqRsp "	},
	{	CMD_CPRID_UFILEDATSND	,   "U FilDatSnd    "	},
	{	CMD_CPRID_UFILSNDFINNTC	,	"U FilSndFinReq "	},
	{	CMD_CPRID_UFILSNDFINRES	,	"U FilSndFinRsp "	},
	{	CMD_CPRID_TRXSETREQ		,	"TrxSetReq      "	},
	{	CMD_CPRID_TRXSETRES		,	"TrxSetRsp      "	},
	{	CMD_CPRID_TRXRELREQ		,	"TrxRelReq      "	},
	{	CMD_CPRID_TRXRELRES		,	"TrxRelRsp      "	},
	{	CMD_CPRID_TOFFSETINFREQ	,	"ToffsetInfoReq "	},
	{	CMD_CPRID_TOFFSETINFRES	,	"ToffsetInfoRsp "	},
	{	CMD_CPRID_CARONOFFREQ	,	"NonMdCarOnOfReq"	},
	{	CMD_CPRID_CARONOFFRES	,	"NonMdCarOnOfRsp"	},
	{	CMD_CPRID_EQPSTRREQ		,	"RE EqpStrReq   "	},
	{	CMD_CPRID_EQPSTRRES		,	"RE EqpStrRsp   "	},
	{	CMD_CPRID_SLOTINFREQ	,	"RE SlotInfoReq "	},
	{	CMD_CPRID_SLOTINFRES	,	"RE SlotInfoRsp "	},
	{	CMD_CPRID_RESTSREQ		,	"RE StsInqReq   "	},
	{	CMD_CPRID_RESTSRES		,	"RE StsInqRsp   "	},
	{	CMD_CPRID_CARDINFREQ	,	"RE CardInfReq  "	},
	{	CMD_CPRID_CARDINFRES	,	"RE CardInfRsp  "	},
	{	CMD_CPRID_REVERREQ		,	"RE VerInqReq   "	},
	{	CMD_CPRID_REVERRES		,	"RE VerInqRsp   "	},
	{	CMD_CPRID_RECARDCNTREQ	,	"RE CardCtrlReq "	},
	{	CMD_CPRID_RECARDCNTRES	,	"RE CardCtrlRsp "	},
	{	CMD_CPRID_CARDSTSREQ	,	"RE CardStsReq  "	},
	{	CMD_CPRID_CARDSTSRES	,	"RE CardStsRsp  "	},
	{	CMD_CPRID_CARLSTSREQ	,	"RE CarStsInqReq"	},
	{	CMD_CPRID_CARLSTSRES	,	"RE CarStsInqRsp"	},
	{	CMD_CPRID_REPORTCNTREQ	,	"RE PortCtrlReq "	},
	{	CMD_CPRID_REPORTCNTRES	,	"RE PortCtrlRsp "	},
	{	CMD_CPRID_REPORTSTSREQ	,	"RE PortStsReq  "	},
	{	CMD_CPRID_REPORTSTSRES	,	"RE PortStsRsp  "	},
	{	CMD_CPRID_RECRDDIAREQ	,	"RE CardDiagReq "	},
	{	CMD_CPRID_RECRDDIARES	,	"RE CardDiagRsp "	},
	{	CMD_CPRID_REOBSLOGREQ	,	"RE TrblLogReq  "	},
	{	CMD_CPRID_REOBSLOGRES	,	"RE TrblLogRsp  "	},
	{	CMD_CPRID_REOBLOGSTPREQ	,	"RE TrblLgStpReq"	},
	{	CMD_CPRID_REOBLOGSTPRES	,	"RE TrblLgStpRsp"	},
	{	CMD_CPRID_EXTDATSNDREQ	,	"ExtEqpDatSndReq"	},
	{	CMD_CPRID_EXTDATSNDRES	,	"ExtEqpDatSndRsp"	},
	{	CMD_CPRID_EXTDATRCVREQ	,	"ExtEqpDatRcvReq"	},
	{	CMD_CPRID_EXTDATRCVRES	,	"ExtEqpDatRcvRsp"	},
	{	CMD_CPRID_MTADDSETREQ	,	"MT AddrSetReq  "	},
	{	CMD_CPRID_MTADDSETRES	,	"MT AddrSetRsp  "	},
	{	CMD_CPRID_FLDMTCNCREQ	,	"FLD CncReq     "	},
	{	CMD_CPRID_FLDMTCNCRES	,	"FLD CncRsp     "	},
	{	CMD_CPRID_FLDCOMSNDREQ	,	"FLD CmdTrSndReq"	},
	{	CMD_CPRID_FLDCOMSNDRES	,	"FLD CmdTrSndRsp"	},
	{	CMD_CPRID_FLDCOMRCVREQ	,	"FLD CmdTrRcvReq"	},
	{	CMD_CPRID_FLDCOMRCVRES	,	"FLD CmdTrRcvRsp"	},
	{	CMD_CPRID_FLDFILSNDREQ	,	"FLD FilTrSndReq"	},
	{	CMD_CPRID_FLDFILSNDRES	,	"FLD FilTrSndRsp"	},
	{	CMD_CPRID_FLDFILRCVREQ	,	"FLD FilTrRcvReq"	},
	{	CMD_CPRID_FLDFILRCVRES	,	"FLD FilTrRcvRsp"	},
	{	CMD_CPRID_FLMTCNCRELREQ	,	"FLD CncRelReq  "	},
	{	CMD_CPRID_FLMTCNCRELRES	,	"FLD CncRelRsp  "	},
	{	CMD_CPRID_FLMTCMPRELREQ	,	"FLD CnFrcRelReq"	},
	{	CMD_CPRID_FLMTCMPRELRES	,	"FLD CnFrcRelRsp"	},
	{	CMD_CPRID_SVMTCNTREQ	,	"eNB CncReq     "	},
	{	CMD_CPRID_SVMTCNTRES	,	"eNB CncRsp     "	},
	{	CMD_CPRID_SVMTDATSNDREQ	,	"eNB DatTrSndReq"	},
	{	CMD_CPRID_SVMTDATSNDRES	,	"eNB DatTrSndRsp"	},
	{	CMD_CPRID_SVMTDATRCVREQ	,	"eNB DatTrRcvReq"	},
	{	CMD_CPRID_SVMTDATRCVRES	,	"eNB DatTrRcvRsp"	},
	{	CMD_CPRID_SVMTCNTRELREQ	,	"eNB CncRelReq  "	},
	{	CMD_CPRID_SVMTCNTRELRES	,	"eNB CncRelRsp  "	},
	{	CMD_CPRID_SVMTFORRELREQ	,	"eNB CnFrcRelReq"	},
	{	CMD_CPRID_SVMTFORRELRES	,	"eNB CnFrcRelRsp"	},
	{	CMD_CPRID_MKMTDATSND	,	"Mkr DatSnd     "	},
	{	CMD_CPRID_MKMTCNTREQ	,	"Mkr CncReq     "	},
	{	CMD_CPRID_MKMTCNTRES	,	"Mkr CncRsp     "	},
	{	CMD_CPRID_MKMTCNTRELREQ	,	"Mkr CncRelReq  "	},
	{	CMD_CPRID_MKMTCNTRELRES	,	"Mkr CncRelRsp  "	},
	{	CMD_CPRID_MKMTFORRELREQ	,	"Mkr CnFrcRelReq"	},
	{	CMD_CPRID_MKMTFORRELRES	,	"Mkr CnFrcRelRsp"	},
	{	CMD_CPRID_FTPPORTSETREQ ,	"FTP PortSetReq "	},
	{	CMD_CPRID_FTPPORTSETRES ,	"FTP PortSetRsp "	},
	{	0						,	"Unknown Signal "	}
};


/********************************************************************************************************************/
/*!
 *  @brief  Command of l2 sum process.
 *  @note   Command of l2 sum process.
 *  @param  fp     [in]  Pointer of output file
 *  @param  cprino [in]  CPRIリンク番号
 *  @param  blk    [in]
 *  @return Result code is returned
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01717
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 *  @date   2015/09/02 TDI)satou CPRIリンク番号を引数に追加し、指定リンク番号のテーブル情報を出力するように
 *  @date   2015/10/23 TDIPS)Lay M-RRU-ZSYS-01717 strSigテーブルの宣言・初期化を修正。そのテーブルの範囲外をアクセスしないように修正
 */
/********************************************************************************************************************/
void dbgcmd_exec_l2_lpb_sum(FILE *fp,int cprino,int blk)
{
	int ret;
	int errcd;
	int cnt_area;
	int index;
	int printIdx;
	int dispCnt;
	T_RRH_L2_STATISTICS_L2TABLE * dbgcmd_l2_comw_stati_L2Tbl = 0; /* l2 statistics information table */
	int strSigIdx = 0;
	int strSndRcvIdx = 0;
	char *strSig[LBD_L2_STSNUM] = 
		{	"L2_DOV", 
			"L2_STO",   
			"L2_DRV",   
			"L2_DSD",   
			"L2_INS",    
			"L2_ILR",    
			"L2_ITO",    
			"L2_LTO",   
			"L2_IRO",    
			"L2_LRO",   
			"L2_RST",   
			"L2_TO3",   
			"L2_RXF",   
			"L2_TXF",   
			"L2_RXO",   
			"L2_TXO",   
			"L2_LRST", 
			"L2_RBSY", 
			"L2_NTXF", 
			"L2_NTXO",
			"L2_RDTO",
			"L2_SDTO",
			"L2_RI",     
			"L2_SI",     
			"L2_NI",      
			"L2_RS",     
			"L2_SS",     
			"L2_NS",     
			"L2_RU",     
			"L2_SU",     
			"L2_NU",     
			"L2_RRNR",
			"L2_SRNR", 
			"L2_RREJ", 
			"L2_SREJ", 
			"L2_RDM",  
			"L2_SDM",  
			"L2_RDSC", 
			"L2_SDSC", 
			"L2_RUA",   
			"L2_SUA",   
			"L2_RFMR", 
			"L2_SFMR",
			"L2_SFMR",
			"L2_RSBM",
			"L2_SSBM",
			"L2_RC",     
			"L2_SC",     
			"L2_DSC_I",
			"L2_ESTIND_N",
			"L2_ESTIND_U",
			"L2_ESTCNF",
			"L2_RELIND_N",
			"L2_RELIND_U",
			"L2_RELCNF",
			"L2_INGIND_N",
			"L2_INGIND_U",
			"L2_DSR2T1O",
			"L2_WAI2T1O",
			"L2_T4O",
			"L2_RDE",
			"L2_RSTO",
			"L2_DSRT1O",
			"L2_WAITIO",
			"L2_IDSC",
			"L2_BIDSC",
			"L2_RSEND",
			"L2_RFRMW",
			"L2_RFRMX",
			"L2_RFRMY",
			"L2_RFRMZ",
			"L2_SFRMW",
			"L2_SFRMX",
			"L2_SFRMY",
			"L2_SFRMZ",
			"L2_SRRP1",
			"L2_SRRF1",
			"L2_SRRF0",
			"L2_QUENUM",
			"L2_BSRNR",
			"L2_BCSRR",
			"L2_OBRRNR",
			"L2_OBCRRR",
			"L2_NS2END",
};
	char *strSndRcv[3] = {"Snd","Rcv","Eve"};

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L2_STAT_INFO/**/,(VOID**)&dbgcmd_l2_comw_stati_L2Tbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(fp,"sharemeory E_RRH_SHMID_APL_CPRI_L2_STAT_INFO NG \n");
		return;
	}

	if(blk == 0)
	{
		/*	current block noly display	*/
		printIdx = dbgcmd_l2_comw_stati_L2Tbl[cprino].mng.index;
		dispCnt  = 1;
	}
	else if((blk > 0) && (blk <= D_RRH_L2_STATI_L2BLK_NUM))
	{
		/*	indicated block noly display	*/
		printIdx = blk-1;
		dispCnt  = 1;
	}
	else
	{
		/*	all index display	*/
		printIdx = (dbgcmd_l2_comw_stati_L2Tbl[cprino].mng.index + 1) % D_RRH_L2_STATI_L2BLK_NUM;
		dispCnt  = D_RRH_L2_STATI_L2BLK_NUM;
	}

	fprintf(fp, "l2 summary***********************************************************\n");
	fprintf(fp, "cprino : %d\n", cprino);
	fprintf(fp, "l2 statistics  current index is %d:\n", dbgcmd_l2_comw_stati_L2Tbl[cprino].mng.index);

	for(index = 0; index < dispCnt; index++)
	{
		fprintf(fp, " l2 statistics  Block[%2d]-------------------------------------\n", printIdx);
		for(cnt_area = 0; cnt_area < D_RRH_L2_STATI_L2SIG_NUM; cnt_area++)
		{
			if(dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime_flag == CMD_FLG_ON)
			{
				strSigIdx = dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].singal_kind;
				strSndRcvIdx = dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].opt.l2_opt.l2_tx_rv_flag;
				if( (strSigIdx < LBD_L2_STSNUM) && (strSndRcvIdx < 3) ) 
				{
					fprintf(fp, "sig(%-11s):[%s] [B:%04x/%02x/%02x %02x:%02x:%02x:%02x0(%03d.%03d)]->[E:%04x/%02x/%02x %02x:%02x:%02x:%02x0(%03d.%03d)] occur:%08X,over:%08X \n",
						strSig[strSigIdx],
						strSndRcv[strSndRcvIdx],
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.year,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.month,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.day,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.hour,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.min,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.sec,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.msec,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.ucount/1000,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].firstTime.ucount%1000,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.year,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.month,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.day,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.hour,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.min,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.sec,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.msec,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.ucount/1000,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].lastTime.ucount%1000,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].occur_counter,
						dbgcmd_l2_comw_stati_L2Tbl[cprino].blks[printIdx].sigs[cnt_area].overflow_counter);
				}
			}
		}
		printIdx = ((printIdx + 1) % D_RRH_L2_STATI_L2BLK_NUM);
	}

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  Command of l3 sum process.
 *  @note   Command of l3 sum process.
 *  @param  fp     [in]  Pointer of output file
 *  @param  cprino [in]  CPRIリンク番号
 *  @param  blk    [in]
 *  @return Result code is returned
 *  @retval -
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 *  @date   2015/09/02 TDI)satou CPRIリンク番号を引数に追加し、指定リンク番号のテーブル情報を出力するように
 */
/********************************************************************************************************************/
void dbgcmd_exec_l3_lpb_sum(FILE *fp,int cprino,int blk)
{
	int ret;
	int errcd;
	int index;
	int cnt_area;
	int printIdx;
	int dispCnt;
	UINT system_type;
	T_RRH_L2_STATISTICS_L3TABLE * dbgcmd_l2_comw_stati_L3Tbl = 0; /* l2 statistics information table */
	T_RRH_L2_STATISTICS_L3TABLE * dbgcmd_l2_comw_stati_L3Tbl_workp; /* l2 statistics information table */

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L3_STAT_INFO/**/,(VOID**)&dbgcmd_l2_comw_stati_L3Tbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(fp,"sharemeory E_RRH_SHMID_APL_CPRI_L2_STAT_INFO NG \n");
		return;
	}
	for(system_type=0; system_type<CMD_MAX_SYSNUM; system_type++)
	{
		fprintf(fp, "l3 summary***********************************************************\n");
		dbgcmd_l2_comw_stati_L3Tbl_workp = dbgcmd_l2_comw_stati_L3Tbl+(D_RRH_CPRINO_NUM*system_type+cprino);
		if(blk == 0)
		{
			/*	current block noly display	*/
			printIdx = dbgcmd_l2_comw_stati_L3Tbl_workp->mng.index;
			dispCnt  = 1;
		}
		else if((blk > 0) && (blk <= D_RRH_L2_STATI_L3BLK_NUM))
		{
			/*	indicated block noly display	*/
			printIdx = blk-1;
			dispCnt  = 1;
		}
		else
		{
			/*	all index display	*/
			printIdx = (dbgcmd_l2_comw_stati_L3Tbl_workp->mng.index + 1) % D_RRH_L2_STATI_L3BLK_NUM;
			dispCnt  = D_RRH_L2_STATI_L3BLK_NUM;
		}

		fprintf(fp, "l3 statistics  current index is %d:", dbgcmd_l2_comw_stati_L3Tbl_workp->mng.index);
		if(system_type == CMD_SYS_3G)
		{
			fprintf(fp, "[ 3G signal]");
		}
		else
		{
			fprintf(fp, "[LTE signal]");
		}
		fprintf(fp, "cprino : %d\n", cprino);
		for(index = 0; index < dispCnt; index++)
		{
			fprintf(fp, " l3 statistics  Block[%2d]-------------------------------------\n", printIdx);
			for(cnt_area = 0; cnt_area < D_RRH_L2_STATI_L3SIG_NUM; cnt_area++)
			{
				if(dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime_flag == CMD_FLG_ON)
				{
					fprintf(fp, "sig(0x%04X):[B:%04x/%02x/%02x %02x:%02x:%02x:%02x0(%03d.%03d)]->[E:%04x/%02x/%02x %02x:%02x:%02x:%02x0(%03d.%03d)] occur:%08X,over:%08X,OK:%08X,NG:%08X,CODE:%08X\n",
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].singal_kind,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.year,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.month,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.day,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.hour,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.min,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.sec,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.msec,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.ucount / 1000,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].firstTime.ucount % 1000,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.year,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.month,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.day,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.hour,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.min,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.sec,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.msec,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.ucount / 1000,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].lastTime.ucount % 1000,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].occur_counter,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].overflow_counter,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].opt.l3_opt.l3_ok_counter,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].opt.l3_opt.l3_ng_counter,
						dbgcmd_l2_comw_stati_L3Tbl_workp->blks[printIdx].sigs[cnt_area].opt.l3_opt.l3_last_ng_code);
				}
			}
			printIdx = ((printIdx + 1) % D_RRH_L2_STATI_L3BLK_NUM);
		}
	}
	return ;
}
/********************************************************************************************************************/
/*!
 *  @brief  Command of l2 hist process.
 *  @note   Command of l2 hist process.
 *  @param  fp          [in]  Pointer of output file
 *  @param  logTblp     [in]  pointer of log table
 *  @param  printIdx    [in]  print index
 *  @return Result code is returned
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
void dbgcmd_disp_l23_lpb_hist(FILE *fp, T_RRH_L2_LOG_INFO *logTblp, UINT printIdx )
{
	int data_idx;
	UINT	loop;
	USHORT	*signal_kind;
	USHORT	sigkind;
	USHORT	len;
	union CTL_FLD1	*wk_ctl;                               

	fprintf(fp, "[%3d] [%04x/%02x/%02x %02x:%02x:%02x:%02x0(%03d.%03d)] ",
		printIdx,
		logTblp->dt.year,
		logTblp->dt.month,
		logTblp->dt.day,
		logTblp->dt.hour,
		logTblp->dt.min,
		logTblp->dt.sec,
		logTblp->dt.msec,
		logTblp->dt.ucount/1000,
		logTblp->dt.ucount%1000
		);
	
	if(logTblp->link_num == D_RRH_CPRINO_REC)
	{
		if(logTblp->tx_rv_flag == 'R')
		{
			fprintf(fp, "[REC--->FHM         ] ");
		}
		else
		{
			fprintf(fp, "[REC<---FHM         ] ");
		}
	}
	else
	{
		if(logTblp->tx_rv_flag == 'R')
		{
			fprintf(fp, "[       FHM<---RE#%02d] ", logTblp->link_num);
		}
		else
		{
			fprintf(fp, "[       FHM--->RE#%02d] ", logTblp->link_num);
		}
	}
	switch(logTblp->lapb_type)
	{
		case D_L2_ICTL:
			wk_ctl = (union CTL_FLD1*)&(logTblp->lapb_cmd);
			fprintf(fp, "add:0x%02X, cmd:0x%02X [I   (NS=%d, NR=%d)](len %5d) : ",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						wk_ctl->ctli_8.i8ns, wk_ctl->ctli_8.i8nr,
						logTblp->date_len);
			
			signal_kind = (USHORT *)&logTblp->data[0];
			sigkind = ntohs(*signal_kind);
			
			for(loop=0; loop<D_RRH_L2_STATI_L3SIG_NUM; loop++)
			{
				if(l2_comr_stati_l3sig_cmd[loop].signal_kind == (sigkind & 0xFFFE))
				{
					break;
				}
				else if(l2_comr_stati_l3sig_cmd[loop].signal_kind == 0)
				{
					break;
				}
			}
			fprintf(fp,"(%04X)%s\n", sigkind, l2_comr_stati_l3sig_cmd[loop].sigName);
			if(logTblp->date_len > D_RRH_L2_TXRV_LOG_DATA_LEN)
			{
				len = D_RRH_L2_TXRV_LOG_DATA_LEN;
			}
			else
			{
				len = logTblp->date_len;
			}
			for(data_idx = 0; data_idx < len; data_idx++)
			{
				fprintf(fp, "%02X ", logTblp->data[data_idx]);
				
				if(((data_idx % DBGCMD_DATA_COLUMNS) == (DBGCMD_DATA_COLUMNS - 1))
				   && ((data_idx + 1) != len))
				{
					fprintf(fp,"\n");
				}
			}
			fprintf(fp,"\n");
			break;
		case D_L2_RRCTL:
			wk_ctl = (union CTL_FLD1*)&(logTblp->lapb_cmd);
			fprintf(fp, "add:0x%02X, cmd:0x%02X [RR  (      NR=%d)](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						wk_ctl->ctli_8.i8nr,
						logTblp->date_len);
			break;
		case D_L2_RNRCTL:
			wk_ctl = (union CTL_FLD1*)&(logTblp->lapb_cmd);
			fprintf(fp, "add:0x%02X, cmd:0x%02X [RNR (      NR=%d)](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						wk_ctl->ctli_8.i8nr,
						logTblp->date_len);
			break;
		case D_L2_REJCTL:
			wk_ctl = (union CTL_FLD1*)&(logTblp->lapb_cmd);
			fprintf(fp, "add:0x%02X, cmd:0x%02X [REJ (      NR=%d)](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						wk_ctl->ctli_8.i8nr,
						logTblp->date_len);
			break;
		case D_L2_SBMCTL:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [SABM(          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
		case D_L2_DISCCTL:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [DISC(          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
		case D_L2_DMCTL:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [DM  (          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
		case D_L2_UACTL:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [UA  (          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
		case D_L2_FRMRCTL:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [FRMR(          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
		default:
			fprintf(fp, "add:0x%02X, cmd:0x%02X [OTHR(          )](len %5d)\n",
						logTblp->lapb_addr,
						logTblp->lapb_cmd,
						logTblp->date_len);
			break;
	}
	return ;
}

/********************************************************************************************************************/
/*!
 *  @brief  Command of l2 hist process.
 *  @note   Command of l2 hist process.
 *  @param  fp       [in]  Pointer of output file
 *  @param  startCnt [in]
 *  @param  runCnt   [in]
 *  @param  l3flg    [in]
 *  @return Result code is returned
 *  @retval -
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
void dbgcmd_exec_l23_lpb_hist(FILE *fp,int startCnt,int runCnt,int l3flg )
{
	int ret;
	int errcd;
	int index;
	int blk;
	int printIdx;
	int startCnt2;
	T_RRH_L2_LOG_INFO_ALL		*l2logAll_p = 0;
	T_RRH_L2_LOG_INFO_TABLE * dbgcmd_l2_comw_logTbl;
	T_RRH_L2_LOG_INFO_SPEC* dbgcmd_l2_comw_logSpec;

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_LAPB_L3_SR_LOG/**/,(VOID**)&l2logAll_p,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		fprintf(fp,"sharemeory E_RRH_SHMID_APL_CPRI_L2_STAT_INFO NG \n");
		return;
	}
	dbgcmd_l2_comw_logTbl = &(l2logAll_p->table);
	dbgcmd_l2_comw_logSpec = &(l2logAll_p->spec);

	if(startCnt < 1)
	{
		/*	最初の起動時のみ出力	*/
		startCnt = D_L2_TXRV_LOG_INF_NONCYC/4;
		startCnt2 = 0;
	}
	else if(startCnt < D_L2_TXRV_LOG_INF_NONCYC)
	{
		/*	最初の起動時のみ出力	*/
		startCnt2 = 0;
	}
	else if(startCnt > D_L2_TXRV_LOG_INF_NONCYC2)
	{
		/*	最初の起動時と最新の起動時両方	*/
		startCnt  = D_L2_TXRV_LOG_INF_NONCYC;
		startCnt2 = D_L2_TXRV_LOG_INF_NONCYC;
	}
	else
	{
		startCnt  = D_L2_TXRV_LOG_INF_NONCYC;
		startCnt2 = startCnt - D_L2_TXRV_LOG_INF_NONCYC;
	}

	if(runCnt < 1)
	runCnt = (D_RRH_L2_TXRV_LOG_INF_NUM - D_L2_TXRV_LOG_INF_NONCYC2)/8;
	if(runCnt > (D_RRH_L2_TXRV_LOG_INF_NUM - D_L2_TXRV_LOG_INF_NONCYC2))
	runCnt  = (D_RRH_L2_TXRV_LOG_INF_NUM - D_L2_TXRV_LOG_INF_NONCYC2);

	if(l3flg)
	fprintf(fp, "l3 history[startup]:***********************************************************\n");
	else
	fprintf(fp, "l2 history[startup]:***********************************************************\n");

	printIdx = 0;
	for(index = 0; index < startCnt; index++)
	{
		if(dbgcmd_l2_comw_logTbl->records[index].dt.year == 0)
			continue;

		if(l3flg && (dbgcmd_l2_comw_logTbl->records[index].lapb_type != 2))
			continue;
		
		dbgcmd_disp_l23_lpb_hist(fp, &dbgcmd_l2_comw_logTbl->records[index], printIdx);
		printIdx++;
	}

	if(l3flg)
	fprintf(fp, "\nl3 history[startup2]:**********************************************************\n");
	else
	fprintf(fp, "\nl2 history[startup2]:**********************************************************\n");
	
	printIdx = 0;
	for(index = D_L2_TXRV_LOG_INF_NONCYC; index < (D_L2_TXRV_LOG_INF_NONCYC + startCnt2); index++)
	{
		if(dbgcmd_l2_comw_logTbl->records[index].dt.year == 0)
			continue;

		if(l3flg && (dbgcmd_l2_comw_logTbl->records[index].lapb_type != 2))
			continue;
		
		dbgcmd_disp_l23_lpb_hist(fp, &dbgcmd_l2_comw_logTbl->records[index], printIdx);
		printIdx++;
	}

	if(l3flg)
	fprintf(fp, "\nl3 history[running]:***********************************************************\n");
	else
	fprintf(fp, "\nl2 history[running]:***********************************************************\n");

	printIdx = 0;
	for(index = D_L2_TXRV_LOG_INF_NONCYC2; index < (D_L2_TXRV_LOG_INF_NONCYC2 + runCnt) ; index++)
	{
		if(dbgcmd_l2_comw_logTbl->records[index].dt.year == 0)
		continue;

		if(l3flg && (dbgcmd_l2_comw_logTbl->records[index].lapb_type != 2))
			continue;

		dbgcmd_disp_l23_lpb_hist(fp, &dbgcmd_l2_comw_logTbl->records[index], printIdx);
		printIdx++;
	}
	for(blk = 0; blk < D_RRH_L2_TXRV_LOG_BLKS; blk++)
	{
		if(l3flg)
		fprintf(fp, "\nl3 Spec[%d] history[startup/running]:******************************************\n", blk);
		else
		fprintf(fp, "\nl2 Spec[%d] history[startup/running]:******************************************\n", blk);
	
		if(l3flg && (blk == 0))
			continue;
		
		printIdx = 0;
		for(index = 0; index < D_RRH_L2_TXRV_LOG_SPEC_INF_NUM; index++)
		{
			if(dbgcmd_l2_comw_logSpec->spec[blk][index].dt.year == 0)
				continue;
			
			dbgcmd_disp_l23_lpb_hist(fp, &dbgcmd_l2_comw_logSpec->spec[blk][index], printIdx);
			printIdx++;
		}
		fprintf(fp, "\n");
	}
	return ;
}

/********************************************************************************************************************/
/*!
 *  @brief  Command process.
 *  @note   Command process.
 *  @param  cmdStr [in]  Pointer of command string
 *  @param  fp     [in]  Pointer of output file
 *  @return Result code is returned
 *  @retval -
 *  @date   2014/01/03 FFCS)hongj Create for zynq
 *  @date   2015/09/02 TDI)satou CPRIリンク番号を引数にとるように
 */
/********************************************************************************************************************/
int dbgcmd_exec_l2_lpb(char * cmdStr, FILE *fp)
{
	UINT						argcCnt;
	CHAR						*argvParam[7];
	CHAR						*getToken, *savePtr;
	CHAR						*delimiter = " ";

	int							startcnt;
	int							runcnt;
	int                         cprino;                 /* CPRIリンク番号 */

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
		fprintf(fp,"please intput\n,<cprino> dsp l2...\n,cprino : 0-16\n");
		return D_RRH_NG;
	}

	cprino = atoi(argvParam[ 1 ]);
	if ((D_RRH_CPRINO_REC > cprino) || (D_RRH_CPRINO_RE_MAX < cprino)) {
	    fprintf(fp,"please input\n,cprino : 0-16\n");
	    return D_RRH_NG;
	}

	if(strcmp(argvParam[ 2 ],"dsp" ) == 0)
	{
		if(strcmp(argvParam[ 4 ],"hist") == 0)
		{
			startcnt = runcnt = 0;
			if(argcCnt > 5)
			startcnt = atoi(argvParam[ 5 ]);
			if(argcCnt > 6)
			runcnt = atoi(argvParam[ 6 ]);

			if(strcmp(argvParam[ 3 ],"l2") == 0)
			dbgcmd_exec_l23_lpb_hist(fp,startcnt,runcnt,0);
			else
			dbgcmd_exec_l23_lpb_hist(fp,startcnt,runcnt,1);
		}
		else if(strcmp(argvParam[ 4 ],"sum") == 0)
		{
			runcnt = 0;
			if(argcCnt > 5)
			runcnt = atoi(argvParam[ 5 ]);

			if(strcmp(argvParam[ 3 ],"l2") == 0)
			dbgcmd_exec_l2_lpb_sum(fp,cprino,runcnt);
			else
			dbgcmd_exec_l3_lpb_sum(fp,cprino,runcnt);
		}

		else
		fprintf(fp,"please intput\n 1.linkhis\n2.sum\n");
	}
	else
	fprintf(fp,"please intput dsp\n");

	return 0;
 }

/**
 * @brief L2デバッグコマンド直接実行
 * @note  用途は、障害ログ出力
 * @note  障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 * @date  2015/09/02 TDI)satou CPRIリンク番号でループするように
 */
INT dbgcmd_exec_direct_l2_lpb_sum( int argc, char *argv[], FILE *outf  )
{
    int cprino;     /* CPRIリンク番号 */

    for (cprino = 0; cprino < D_RRH_CPRINO_NUM; cprino++) {
        dbgcmd_exec_l2_lpb_sum(outf, cprino, (D_RRH_L2_STATI_L2BLK_NUM+1));
    }
	return D_RRH_OK;
}

/**
 * @brief L2デバッグコマンド直接実行
 * @note  用途は、障害ログ出力
 * @note  障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 */
INT dbgcmd_exec_direct_l2_lpb_hist( int argc, char *argv[], FILE *outf  )
{
    dbgcmd_exec_l23_lpb_hist(outf, D_L2_TXRV_LOG_INF_NONCYC2, D_RRH_L2_TXRV_LOG_INF_NUM, 0);
	return D_RRH_OK;
}

/**
 * @brief L2デバッグコマンド直接実行
 * @note  用途は、障害ログ出力
 * @note  障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 * @date  2015/09/02 TDI)satou CPRIリンク番号でループするように
 */
INT dbgcmd_exec_direct_l3_lpb_sum( int argc, char *argv[], FILE *outf  )
{
    int cprino;     /* CPRIリンク番号 */

    for (cprino = 0; cprino < D_RRH_CPRINO_NUM; cprino++) {
        dbgcmd_exec_l3_lpb_sum(outf, cprino, (D_RRH_L2_STATI_L3BLK_NUM+1));
    }
	return D_RRH_OK;
}

/**
 * @brief L2デバッグコマンド直接実行
 * @note  用途は、障害ログ出力
 * @note  障害ログ出力時にINITプロセスから実行される(f_ini_AccidentlogGet())
 */
INT dbgcmd_exec_direct_l3_lpb_hist( int argc, char *argv[], FILE *outf  )
{
    dbgcmd_exec_l23_lpb_hist(outf, D_L2_TXRV_LOG_INF_NONCYC2, D_RRH_L2_TXRV_LOG_INF_NUM, 1);
	return D_RRH_OK;
}

/* @} */
