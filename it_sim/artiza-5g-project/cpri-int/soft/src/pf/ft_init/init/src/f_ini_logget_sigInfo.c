/*!
 * @skip  $ld:$
 * @file  f_ini_logget_sigInfo.c
 * @brief Signal情報Assert出力処理
 * @date  2014/02/01 ALPHA) yokoyama Create PF-Init-001-002-001 初期化
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief f_ini_logget_sigInfo
 * @note  関数処理内容.
 *		-# Signal情報をAssertに出力する
 * @param  pid				[in] Signal情報(プロセスID)
 * @param  signo			[in] Signal情報(Signal番号)
 * @param  status			[in] Signal情報(Option情報)
 * @warning		N/A
 * @FeatureID	PF-Init-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/19 ALPHA)yokoyama Create
 */
VOID f_ini_logget_sigInfo( INT pid, INT signo, INT status , VOID* buf_p )
{
	UINT				count;
	CHAR				message[200];
	CHAR				temp[32];
/* 2020/12/28 M&C) Implement device startup (start modify) */
	CHAR				processName[D_RRH_PROCID_MAX+1][8] = {"INIT", "PF", "F_PF", "L2", "L3", "MT", "DBG", "RE", "PTP", "DUCMD", "UNKNOWN" };
/* 2020/12/28 M&C) Implement device startup (end modify) */

	/* 送信元PID、Signal名、オプション情報を数字ではなく文言でASSERTに出力する	*/
	memset( message, 0, sizeof(message) );
	
	if( pid < D_RRH_PROCID_MAX )
	{
		/* 既知プロセスからの受信  */
		sprintf( message, "SIG INFO:PID=%s ", &processName[ pid ][0] );
	}
	else
	{
		/* 不明なプロセスからの受信	*/
		sprintf( message, "SIG INFO:PID=%s(%d) ", &processName[ D_RRH_PROCID_MAX ][0], pid );
	}

	/* SIGNAL名取得&OPTION名取得	*/
	switch( signo )
	{
	case SIGUSR1:
		/* SIGNAL名取得	*/
		strcat( message, "SIG=SIGUSR1 " );
		
		/* OPTION取得	*/
		for( count = 0; count < (D_INI_SIG1_STATUS_MAX - 1); count++ )
		{
			/* 以下のログ関連だけは下位2byteも含めて確認する必要あり */
			if( (status == D_RRH_SIGUSR_OPT_RUNLOG_REQ)      ||
				(status == D_RRH_SIGUSR_OPT_RUNLOG_REQ_3G)   ||
				(status == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI)  ||
				(status == D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G))
			{
				if(status == f_inir_sigusr1[count].sig_state)
				{
					break;
				}
				else
				{
					continue;
				}
			}

			/* 上位2byteで種別を判定	*/
			if( (status & D_INI_SIGOPT_KIND)== f_inir_sigusr1[count].sig_state)
			{
				break;
			}
		}
		strcat( message, f_inir_sigusr1[count].message );
		
		/* 番号取得	*/
		sprintf( temp, "(%08x)", status );
		strcat( message, temp );
		break;
		
	case SIGUSR2:
		/* SIGNAL名取得	*/
		strcat( message, "SIG=SIGUSR2 " );
		
		/* OPTION取得	*/
		for( count = 0; count < (D_INI_SIG2_STATUS_MAX - 1); count++ )
		{
			if( status == f_inir_sigusr2[count].sig_state )
			{
				break;
			}
		}
		strcat( message, f_inir_sigusr2[count].message );
		
		/* 番号取得	*/
		sprintf( temp, "(%08x)", status );
		strcat( message, temp );
		break;
		
	case SIGCHLD:
		/* SIGNAL名取得	*/
		strcat( message, "SIG=SIGCHLD " );
		sprintf( temp, "(%08x)", status );
		strcat( message, temp );
		
		/* OPTIONは無い	*/
		break;
		
	default:
		/* SIGNAL名取得	*/
		memset( temp, 0, sizeof(temp) );
		sprintf( temp, "SIG=UNKNOWN(%d) ", signo );
		strcat( message, temp );
		
		/* OPTIONは無い	*/
		break;
	}
	
	/* ASSERT出力	*/
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, message );
		
	return;
}

/* @} */
