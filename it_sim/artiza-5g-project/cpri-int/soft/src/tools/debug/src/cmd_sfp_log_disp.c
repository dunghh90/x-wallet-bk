/* RRH-010 OSE-SHELLD alpha add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_cmd_sfp_log_disp.c
 *  @brief
 *  @date   $Date:$ $Author:$
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include 	"f_dbg_cmd_inc.h"
#include "BPF_COM_LOG.h"
#include "BPF_HM_DEVC.h"
#include "BPF_RM_SVRM.h"

/** @addtogroup SHELLD_CMD_RICR
 * @{ */


typedef struct {
	int		st				;	/* 開始アドレス 			*/
	int		siz				;	/* 取得サイズ				*/
	char*	name			;	/* 項目名					*/
} T_SFP_LOG_ITEM_MAP;
T_SFP_LOG_ITEM_MAP	sfpLogItemMap[35] = {
	{	3,		8,		"Transceiver",					},	/* Transceiver						*/
	{	12,		1,		"BR. Normal",					},	/* BR. Normal						*/
	{	13,		1,		"Rate Identifier ",				},	/*	Resserved						*/
	{	20,		16, 	"Vendor Name",					},	/* Vendor Name	 					*/
	{	36,		1, 		"Transceiver",					},	/*	Reserved 						*/
	{	37,		3,		"Vendor OUI",					},	/* Vendor OUI	 					*/
	{	40, 	16,		"Vendor PN",					},	/* Vendor PN	 					*/
	{	56,		4, 		"Vendor rev",					},	/* Vendor REV	 					*/
	{	68,		16, 	"Vendor SN",					},	/* Vendor SN	 					*/
	{	84,		8, 		"Date Code",					},	/* Date Code	 					*/
	{	92,		1, 		"Diagnostic Monitor Type",		},	/* Diagnostic Monitor Type			*/
	{	93,		1, 		"Enhanced Options",				},	/* Enhancecd Options				*/
	{	96, 	32, 	"Vendor Specific",				},	/* Vencor Specific					*/
	{	0,		2, 		"TH-Temp High Alarm",			},	/* Temp High Alarm(Thresholds)		*/
	{	2,		2, 		"TH-Temp Low Alarm",			},	/* Temp Low Alarm(Thresholds)		*/
	{	4,		2, 		"TH-Temp High Warning",			},	/* Temp High Warning(Thresholds)	*/
	{	6,		2, 		"TH-Temp Low Warning",			},	/* Temp Low Warning(Thresholds)		*/
	{	8,		2, 		"TH-Voltage High Alarm",		},	/* Voltage High Alarm(Thresholds)	*/
	{	10,		2, 		"TH-Voltage Low Alarm",			},	/* Voltage Low Alarm(Thresholds)	*/
	{	12,		2, 		"TH-Voltage High Warning",		},	/* Voltage High Warning(Thresholds)	*/
	{	14,		2, 		"TH-Voltage Low Warning",		},	/* Voltage Low Warning(Thresholds)	*/
	{	16,		2, 		"TH-Bias High Alarm",			},	/* Bias High Alarm(Thresholds)		*/
	{	18,		2, 		"TH-Bias Low Alarm",			},	/* Bias Low Alarm(Thresholds)		*/
	{	20,		2, 		"TH-Bias High Warning",			},	/* Bias High Warning(Thresholds)	*/
	{	22,		2, 		"TH-Bias Low Warning",			},	/* Bias Low Warning(Thresholds)		*/
	{	24,		2, 		"TH-TX Power High Alarm",		},	/* TX POWER High Alarm(Thresholds)	*/
	{	26,		2, 		"TH-TX Power Low Alarm",		},	/* TX POWER Low Alarm(Thresholds)	*/
	{	28,		2, 		"TH-TX Power High Warning",		},	/* TX POWER High Warning(Thresholds)*/
	{	30,		2, 		"TH-TX Power Low Warning",		},	/* TX POWER Low Warning(Thresholds)	*/
	{	32,		2, 		"TH-RX Power High Alarm",		},	/* TX PULSE High Alarm(Thresholds)	*/
	{	34,		2, 		"TH-RX Power Low Alarm",		},	/* TX PULSE Low Alarm(Thresholds)	*/
	{	36,		2, 		"TH-RX Power High Warning",		},	/* TX PULSE High Warning(Thresholds)*/
	{	38,		2, 		"TH-RX Power Low Warning",		},	/* TX PULSE Low Warning(Thresholds)	*/

};


/**
 * 
 */
static char convbuf[329] ;	/* ASCII-HEX String　*/
char *cnvSfpInfo( UCHAR *sfpinfo ) {
	int pos = 0 ;
	int itemlen = 0 ;
	int itemno = 0 ;
	char hexstrbuf[3] ;

	memset( convbuf, 0, sizeof ( convbuf ) ) ;
	for( pos = 0 ; pos < D_RRH_SFPINFO_LEN ; pos ++ ) {	/* sfpinfoバッファのサイズ分のループ */
		if( itemlen == sfpLogItemMap[itemno].siz ) {	/* 項目ごとの切れ目に到達 */
			strcat( convbuf, " " ) ; 	/* 1桁スペースを空ける 	*/
			itemno ++ ;					/* 項目番号をつぎへ		*/
			itemlen = 0 ;				/* 項目長さを初期化 	*/
		}
		sprintf( hexstrbuf, "%02x", ( int ) * ( sfpinfo + pos ) ) ;	/* 文字列化 */
		strcat( convbuf, hexstrbuf ) ;								/* 変換バッファへ追加 */
		itemlen++ ;													/* */
	}
	return convbuf ;
}
/*
 * 日付チェック
 * 次の際にエラー(0)とする
 * ・項目が全て０　　　＝＞未設定部分対策
 * ・0xffの項目がある　＝＞フラッシュの初期状態対策 
 */
int  cmd_sfp_log_checkData( T_RRH_SFPINFO_RECORD *dat  ) {

	if( dat->sysTime.year 	> 2099	||		/* タイムスタンプが異常 */
		dat->sysTime.month 	> 12 	||
		dat->sysTime.day 	> 31 	||
		dat->sysTime.hour 	> 24 	||
		dat->sysTime.min 	> 60 	||
		dat->sysTime.sec 	> 60   ) {
		/* 表示しない */
		return 0 ;
  	}
	if( dat->sysTime.year == 0 &&			/* タイムスタンプが全て空？ */
		dat->sysTime.month == 0 &&
		dat->sysTime.day == 0 &&
		dat->sysTime.hour == 0 &&
		dat->sysTime.min == 0 &&
		dat->sysTime.sec == 0 &&
		dat->sysTime.msec == 0 &&
		dat->sysTime.ucount == 0 ) {
		/* 表示しない */
		return 0 ;
  	}
  	return 1;
  
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  SFP ログ表示コマンド
 *  @note   I2CからSFP情報を読み出し、表示する
 *  @param  argc  [in]   Standard main argc
 *  @param  argv  [in]   Standard main argv
 *  @return result code
 *  @retval D_CMD_OK
 *  @retval D_CMD_NG

 */
/********************************************************************************************************************/
int cmd_sfp_log_disp_main(int argc, char *argv[], FILE *fp )
{
	int					linkNo			;
	int					startLinkNo	=	0	;	/* ログ表示開始ポート(初期値Primary)	*/
	int					endLinkNo	=	1	;	/* ログ表示終了ポート(初期値Secondary)	*/
	T_RRH_SFPINFO_HIS 	sfpInfoHistory		;
	int					historyNo		;

	extern char *cnvRRH_SYSTIME_TIME2Str(T_RRH_SYSTEM_TIME *tDate) ;
	
	
	if( argc > 1 ) {	/* ポート指定あり */

		if( strcmp("0", argv[1]) == 0 ) {	/* Primary SFP Log　表示 */
			startLinkNo =	0 	;	/* ループ開始位置	*/
			endLinkNo	=	1	;	/* ループ終了位置	*/
		}
		else if ( strcmp( "1", argv[1]) == 0 ) {	/* Secondary SFP LOG 表示 */
			startLinkNo	=	1	;
			endLinkNo	=	2	;
		}
		else if ( strcmp( "0c", argv[1] ) == 0 ) {	/* Primary SFP LOG クリア */
			/* Primary SFP LOG clear */
			BPF_COM_LOG_SFP_INFO_CLR(0);
			return D_CMD_OK;
		}
		else if ( strcmp( "1c", argv[1] ) == 0 ){	/* Secondary SFP LOG クリア */
			/* Secondary SFP LOG clear */
			BPF_COM_LOG_SFP_INFO_CLR(1);
			return D_CMD_OK;
		}
		else {										/* ポート指定間違い時 */
			/* 使用法説明表示 */
			fprintf(fp, "usage:\n");
			fprintf(fp, "  cmd_sfp_log     .... Primary & Secondary SFP Log display.\n");
			fprintf(fp, "  cmd_sfp_log 0   .... Primary SFP Log display\n");
			fprintf(fp, "  cmd_sfp_log 0c  .... Primary SFP Log clear\n");
			fprintf(fp, "  cmd_sfp_log 1   .... Secondary SFP log display\n");
			fprintf(fp, "  cmd_sfp_log 1c  .... Secondary SFP log clear\n");
		}
	}
	else {										/* ポートが無い時は両方 */
		startLinkNo	=	0	;
		endLinkNo	=	2	;
	}
	
	fprintf(fp, "****************************************************************\n");
	fprintf(fp, "*   SFP info log history \n");
	fprintf(fp, "****************************************************************\n");
	for( linkNo = startLinkNo ; linkNo < endLinkNo ; linkNo++ )	{	/* ２ポート分 */

		/* ＳＦＰログを取得 */
		if( BPF_COM_LOG_SFP_INFO_LOAD( linkNo, &sfpInfoHistory ) != 0 ) {
			fprintf(fp, "Error: SFP Info Log load error.");
			return D_CMD_NG	;
		}

		fprintf(fp, "[%s]\n", (linkNo == 0)? 
				"SFP-Primary  "	:
				"SFP-Secondary"
		);

		/* 現在～後ろ */
		for( historyNo = sfpInfoHistory.logIndex+1 ; historyNo < D_RRH_SFPLOG_MAX ; historyNo++ ) {
			/* 未設定部分は飛ばす */
		  if( cmd_sfp_log_checkData( &sfpInfoHistory.sfpinfrec[historyNo] ) == 1 ) {
			/* ログ表示 */
			fprintf(fp,
					"[%03d]%s;%s;%s\n",
					historyNo,
					cnvRRH_SYSTIME_TIME2Str( &sfpInfoHistory.sfpinfrec[historyNo].sysTime),
					(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.mountstate==0)? "M":"U",
					(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.mountstate==0)?
					cnvSfpInfo(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.sfpinf):	""
			);
			}
		}
		/* 前から現在（但し先頭は飛ばす)*/
		for( historyNo = 1 ; historyNo <= sfpInfoHistory.logIndex ; historyNo++ ) {
		  /* 未設定部分は飛ばす */
		  if( cmd_sfp_log_checkData( &sfpInfoHistory.sfpinfrec[historyNo] ) == 1 ) {
			/* ログ表示 */
			fprintf(fp,
					"[%03d]%s;%s;%s\n",
					historyNo,
					cnvRRH_SYSTIME_TIME2Str( &sfpInfoHistory.sfpinfrec[historyNo].sysTime),
					(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.mountstate==0)? "M":"U",
					(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.mountstate==0)?
					cnvSfpInfo(sfpInfoHistory.sfpinfrec[historyNo].sfpinfo.sfpinf):	""
			);
		}
		}
		  
	}
	fprintf(fp, "\n");
	
	return D_CMD_OK	;
}

/* @} */

/* @} */
