/*!
 * @skip  $ld:$
 * @file  f_lgc_abortLog.c
 * @brief abort時ログ取得処理
 * @date  2013/12/21 ALPHA)yokoyama Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_LGC_LGC
 * @{
 */

#include "f_cmn_com_inc.h"
#include "f_dbg_com_ext.h"
#include "f_lgc_inc.h"
/*!
 * @brief 関数機能概要:ログ出力
 * @note  関数処理内容.
 *       -# /var/log/troublelog/に統計情報出力
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2018/09/07 KCN)若水
 */
VOID f_lgc_abortLog(UINT kind )
{
	FILE				*fp;
	UCHAR				output_file[256];
	UINT				count;
	CHAR				*command_com[8][4]	=	{
		/* 共通ログ */
		/* syslog_assert_ */
		{	"cmd_rrhdbg",	"dsp",	"ast",	"10000"	},	/* asertログ				*/
		/* 統計情報 statistics_log */
		/* statistics_log */
		{	"stat_dbg",	"os",			"stat",	"10000"	},	/* OS統計情報					*/
		{	"stat_dbg",	"ptp",			"stat",	"10000"	},	/* ネットワーク統計(PTP)		*/
		{	"stat_dbg",	"s_plane",		"stat",	"10000"	},	/* ネットワーク統計(S-Plane)	*/
		{	"stat_dbg",	"u_c_plane",	"stat",	"10000"	},	/* ネットワーク統計(U/C-Plane)	*/
		{	"stat_dbg",	"m_plane",		"stat",	"10000"	},	/* ネットワーク統計(M-Plane)	*/
		{	"stat_dbg",	"rsv_wind",		"stat",	"10000"	},	/* 受信ウィンド統計				*/
		{	"stat_dbg",	"sfp_trance",	"stat",	"10000"	},	/* トランシーバー統計			*/
	};

	
	
	
	/* assert */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_assert", kind, output_file )) != NULL ) {
		count = 0 ;	/* assert */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_assert" );
	}

	/* 統計情報 statistics_log */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "statistics_log", kind, output_file )) != NULL ) {
		count = 1 ;	/* OS統計情報 */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 2 ;	/* ネットワーク統計(PTP) */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 3 ;	/* ネットワーク統計(S-Plane) */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 4 ;	/* ネットワーク統計(U/C-Plane) */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 5 ;	/* ネットワーク統計(M-Plane) */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 6 ;	/* 受信ウィンド統計 */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		count = 7 ;	/* トランシーバー統計 */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		
		/**/
		
		
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN statistics_log" );
	}
	
	


	return;
}
/* @} */
