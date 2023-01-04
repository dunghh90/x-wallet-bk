/*!
 * @skip  $ld:$
 * @file  l2_com_rom.c
 * @brief ROM table(com)
 * @date  2013/11/20 FFCS)hongj Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_L2
 * @{
 */

#include "l2_com_inc.h"

/**
 * @brief   L3 signal name table.
 * @note    L3統計情報のログ出力( lblap_statis_for_l3 )で使用する.
 */
const T_L2_L3SIGNAMETBL l2_comr_stati_l3sig[D_RRH_L2_STATI_L3SIG_NUM] = 
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
	{	CMD_CPRID_FIRMFILERES2	,	"FWFilInfRsp2   "	},
	{	CMD_CPRID_UFIRMFILERES2	,	"U FWFilInfRsp2 "	},
	{	0						,	"Unknown Signal "	}
};


/* @} */



