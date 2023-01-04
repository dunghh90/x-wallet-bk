/*!
 * @skip  $ld:$
 * @file  f_com_abortLog.c
 * @brief abort時ログ取得処理
 * @date  2013/12/21 ALPHA)yokoyama Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <stdlib.h>
#include <stdarg.h>
#include "f_cmn_com_inc.h"
#include "f_dbg_com_ext.h"
#include "f_com_inc.h"
#include "BPF_COM_PLOG.h"

/*!
 * @brief 関数機能概要:abort処理
 * @note  関数処理内容.
 *       -# プロセス固有情報出力
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2021/01/19 M&C) Merge 4G FHM src
 */
VOID f_com_abortLogFHM()
{

	FILE				*fp;
	UCHAR				output_file[256];
	UINT				count;
	CHAR				*argvp[10];
	CHAR				*command_com[8][4]	=	{
		{	"cmd_rrhdbg",	"dsp",	"ast",	"10000"	},	/* asertログ				*/

		{	"cmd_rrhdbg",	"dsp",	"reg",	"10000"	},	/* レジスタアクセスログ		*/
		{	"cmd_rrhdbg",	"dsp",	"spi",	"10000"	},	/* SPIアクセスログ			*/

		{	"cmd_rrhdbg",	"dsp",	"tim",	"10000"	},	/* タイマー履歴				*/
		{	"cmd_rrhdbg",	"dsp",	"buf",	"10000"	},	/* 通信用バッファ履歴		*/
		{	"cmd_rrhdbg",	"dsp",	"sem",	"10000"	},	/* セマフォ獲得履歴			*/

		{	"cmd_rrhdbg",	"dsp",	"rst",	"10000"	},	/* リセット履歴				*/

		{	"cmd_rrhdbg",	"dsp",	"bpf",	"10000"	}	/* MWログ					*/

	};

	/* assert */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_assert", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		count = 0 ;	/* assert */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_assert" );
	}

	/* reg + spi */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_reg_spi", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		count = 1 ;	/* reg */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		fprintf(fp, "\n\n");

		count = 2 ;	/* spi */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_reg_spi" );
	}


	/* resource ( tim, buf, sem ) */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_resource", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		count = 3 ;	/* tim */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		count = 4 ;	/* buf */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		count = 5 ;	/* sem */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );

	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_resource" );
	}

	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_alm_hist", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		fprintf(fp,"[cmd_alm_hist]\n");
		argvp[0] = "cmd_alm_hist";
		argvp[1] = "100";
		cmd_alm_hist_log_main( 2, argvp, fp );
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_alm_hist" );
	}

	/* other (version+eeprom) */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_common", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		fprintf(fp,"[cmd_version]\n");
		cmd_version_main( 0, 0, fp );
		fprintf(fp,"\n\n");

		/* cmd_eeprom_disp command */
		fprintf(fp,"[cmd_eeprom_disp]\n");
		cmd_eeprom_disp_main( 0, 0, fp );
		fprintf(fp,"\n\n");

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_common" );
	}

	/* reset */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_rst", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		count = 6 ;	/* reset */
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );

	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_rst" );
	}

	/* mw-log */
	if(( fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "syslog_mw", E_BPF_LOG_CMDLOG_OUTDIR_ALM, output_file )) != NULL ) {
		count = 7;
		fprintf(fp,"[%s %s %s %s]\n", command_com[count][0],command_com[count][1],command_com[count][2],command_com[count][3] );
		cmd_rrhdbg_main( 4, command_com[count], fp );
		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}else	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN syslog_mw" );
	}


	return;
}

/*!
 * @brief 関数機能概要:abort処理
 * @note  関数処理内容.
 *       -# プロセス固有情報出力
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2013/12/21 ALPHA)yokoyama
 */
VOID f_com_abortLog(UINT fid , UINT code)
{
	return;
}
/* @} */




/*!
 * @brief 関数機能概要:abort処理
 * @note  関数処理内容.
 * @param  dumptypes		[in] dump type
 * @param  dumphead_p		[in] dump Header
 * @param  dumpMessage_p	[in] dump message_p
 * @return    N/A
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2015/11/15 FJT)Taniguchi
 */
VOID f_com_dumpMsgInit( VOID )
{
	UINT	regw_data;
	UCHAR	hwSrcAddr[6];
	UCHAR	hwDstAddr[6];

	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DP_CUMAC_H, &regw_data );
	hwDstAddr[0] = (UCHAR)((regw_data & 0x0000FF00) >> 8);
	hwDstAddr[1] = (UCHAR)(regw_data & 0x000000FF);

	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DP_CUMAC_L, &regw_data );
	hwDstAddr[2] = (UCHAR)((regw_data & 0xFF000000) >> 24);
	hwDstAddr[3] = (UCHAR)((regw_data & 0x00FF0000) >> 16);
	hwDstAddr[4] = (UCHAR)((regw_data & 0x0000FF00) >> 8);
	hwDstAddr[5] = (UCHAR)(regw_data & 0x000000FF);

	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DP_RUMAC_H, &regw_data );
	hwSrcAddr[0] = (UCHAR)((regw_data & 0x0000FF00) >> 8);
	hwSrcAddr[1] = (UCHAR)(regw_data & 0x000000FF);

	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DP_RUMAC_L, &regw_data );
	hwSrcAddr[2] = (UCHAR)((regw_data & 0xFF000000) >> 24);
	hwSrcAddr[3] = (UCHAR)((regw_data & 0x00FF0000) >> 16);
	hwSrcAddr[4] = (UCHAR)((regw_data & 0x0000FF00) >> 8);
	hwSrcAddr[5] = (UCHAR)(regw_data & 0x000000FF);

	pthread_mutex_init(&f_comw_dumpmsg.mutex, NULL);
	pthread_mutex_lock(&f_comw_dumpmsg.mutex);

	f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_DBG].sd = BPF_COM_DPLOG_DBG_INT(hwSrcAddr,hwDstAddr,f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_DBG].head);
	f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_DBG].checkbit = 0x00010000;

	f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_AISG].sd = BPF_COM_DPLOG_AISG_INT(hwSrcAddr,hwDstAddr,f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_AISG].head);
	f_comw_dumpmsg.dumpHead[E_BPF_COM_DPLOG_ETHTYPE_AISG].checkbit = 0x01000000;

	pthread_mutex_unlock(&f_comw_dumpmsg.mutex);

	return;
}

/*!
 * @brief 関数機能概要:dump msg send処理
 * @note  関数処理内容.
 * @param  ether_type		[in] ether_type
 * @param  dumpMessage_p	[in] dump message_p
 * @return    N/A
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/11/18 FJT)Taniguchi
 */
VOID f_com_dumpMsgSend(e_bpf_com_dump_etype ether_type ,const char* dumpMessage_p , ... )
{
	if(dumpMessage_p == NULL){
		return;
	}

	T_COM_DUMP_HEAD* dumpCtx_p = &f_comw_dumpmsg.dumpHead[ether_type];

	UINT	dump_bit;
	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DPMODE, dumpCtx_p->checkbit, &dump_bit);
	if(dump_bit == 0){
		return;
	}

	CHAR					str[256];
	T_RRH_SYSTEM_TIME		tDateTime;
	INT						pos;
	INT						ret;
	va_list					args;

	memset(str,0,sizeof(str));
	(VOID)BPF_HM_DEVC_GETTIME_OS(&tDateTime);

	pos = snprintf(str,sizeof(str),"%04d/%02d/%02d %02d:%02d:%02d(UTC) uptime:%u(sec) ",
						tDateTime.year,
						tDateTime.month,
						tDateTime.day,
						tDateTime.hour,
						tDateTime.min,
						tDateTime.sec,
						tDateTime.ucount);

	va_start(args, dumpMessage_p); /* pgr0039  */
	(void)vsnprintf(&str[pos], sizeof(str)-pos , (void *)dumpMessage_p, args);
	va_end(args);

	pthread_mutex_lock(&f_comw_dumpmsg.mutex);
	if(dumpCtx_p->sd >= 0){
		if(ether_type == E_BPF_COM_DPLOG_ETHTYPE_AISG){
			ret = BPF_COM_DPLOG_AISG_TXT(dumpCtx_p->sd,dumpCtx_p->head,strlen(str),str);
		}else{
			ret = BPF_COM_DPLOG_DBG_TXT(dumpCtx_p->sd,dumpCtx_p->head,strlen(str),str);
		}
		if(ret != 0){
			close(dumpCtx_p->sd);
			dumpCtx_p->sd = -1;
		}
	}
	pthread_mutex_unlock(&f_comw_dumpmsg.mutex);
	return;
}

/*!
 * @brief 関数機能概要:dump Data send処理
 * @note  関数処理内容.
 * @param  dumptypes		[in] dump type
 * @param  dumphead_p		[in] dump Header
 * @param  dumpMessage_p	[in] dump message_p
 * @return    N/A
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/11/18 FJT)Taniguchi
 */
VOID f_com_dumpDataSend(e_bpf_com_dump_etype ether_type ,int direct , int dataLen ,CHAR* data_p  )
{
	if((dataLen > 0) && (data_p == NULL)){
		return;
	}

	T_COM_DUMP_HEAD* dumpCtx_p = &f_comw_dumpmsg.dumpHead[ether_type];

	UINT	dump_bit;
	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DPMODE, dumpCtx_p->checkbit, &dump_bit);
	if(dump_bit == 0){
		return;
	}

	INT						ret;

	if(ether_type == E_BPF_COM_DPLOG_ETHTYPE_AISG){
		pthread_mutex_lock(&f_comw_dumpmsg.mutex);
		if(dumpCtx_p->sd >= 0){
			if( direct == BPF_COM_DPLOG_DIR_RX ){
				ret = BPF_COM_DPLOG_AISG_RCV(dumpCtx_p->sd,dumpCtx_p->head,dataLen,data_p);
			}else{
				ret = BPF_COM_DPLOG_AISG_SND(dumpCtx_p->sd,dumpCtx_p->head,dataLen,data_p);
			}
			if(ret != 0){
				close(dumpCtx_p->sd);
				dumpCtx_p->sd = -1;
			}
		}
		pthread_mutex_unlock(&f_comw_dumpmsg.mutex);
	}
	return;
}

/* @} */
