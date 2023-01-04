/*!
 * @skip   $Id$
 * @file   cmd_trouble_log_get.c
 * @brief  障害ログのファイル出力
 * @date   2013/12/27 ALPHA)横山 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


#include	"f_dbg_cmd_inc.h"
#include	"BPF_COM_END.h"


#include "BPF_HM_DEVC.h"

UCHAR		f_dbgw_readData[D_RRH_QSPI_FLASH_TROUBLELOG_SIZE];

/*!
 * @brief 関数機能概要:使い方表示
 * @note  関数処理内容.
 *       -# cmd_trouble_log_getの使い方を表示する。
 * @param  outfp	[in]FILE* 出力先ファイルポインタ
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)横山 create
 */

void cmd_trouble_log_get_usage(FILE *outfp ){
	fprintf(outfp,     "Usage : \n");
	fprintf(outfp,     "  cmd_trouble_log_get [face]\n");
	fprintf(outfp,     "    face : 0-2 -> trouble log\n");
	fprintf(outfp,     "    face : 3   -> trouble log(health check disconnect)\n");
}

/*!
 * @brief 関数機能概要:障害ログファイル出力メイン処理
 * @note  関数処理内容.
 *       -# 障害ログのファイル出力を行う。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @date 2013/12/27 ALPHA)横山 create
 */
int cmd_trouble_log_get_main(int argc, char *argv[], FILE* outfp )
{
	FILE* 					output_w;							/* 出力先ファイルポインタ	*/
	UCHAR					rData;
	UINT					uiOffset;
	UINT					uiFileSize;
	UINT					filename_info;
	UINT					device_kind;
	char					filename[4][128] = {"/tmp/trouble00.tar.gz",
												"/tmp/trouble01.tar.gz",
												"/tmp/trouble02.tar.gz",
												"/tmp/healthchklog.tar.gz"};
	
	if( NULL==argv[1])
	{
		/* 使用法説明	*/
		cmd_trouble_log_get_usage(outfp);
		return 0;
	}
	
	/* 装置種別取得	*/
	(void)BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_DEVICE_KIND0, (UCHAR*)&device_kind );
	
	/* 指定面の情報を取得する	*/
	/* 0/1はQSPI-FLASHの0面、2が1面である	*/
	if( strcmp(argv[1], "0" ) == 0  )
	{
		/* 0面	*/
		rData = 0;		/* Flash面設定	*/
		filename_info = 0;
		uiOffset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE0;
	}
	else if( strcmp(argv[1], "1" ) == 0  )
	{
		/* 1面	*/
		rData = 0;		/* Flash面設定	*/
		filename_info = 1;
		uiOffset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE1;
	}
	else if( strcmp(argv[1], "2" ) == 0  )
	{
		/* 2面	*/
		rData = 1;		/* Flash面設定	*/
		filename_info = 2;
		uiOffset = D_RRH_QSPI_FLASH_OFFSET_TROUBLE2;
	}
	else if( strcmp(argv[1], "3" ) == 0  )
	{
		/* 3面(HealthCheck断の障害ログ)	*/
		rData = 1;		/* Flash面設定	*/
		filename_info = 3;
		uiOffset = D_RRH_QSPI_FLASH_OFFSET_HEALTH;
	}
	else
	{
		/* 使用法説明	*/
		cmd_trouble_log_get_usage(outfp);
		return 0;
	}
	BPF_HM_DEVC_QSPI_FLASH_READ(	(UINT)rData,
									uiOffset,
									D_RRH_QSPI_FLASH_TROUBLELOG_SIZE,
									(UCHAR *)f_dbgw_readData);						/* temp領域	*/

	/* 冒頭にゴミが入っていたら異常データなので展開しない				*/
	uiFileSize =(f_dbgw_readData[3] << 24) +
				(f_dbgw_readData[2] << 16) +
				(f_dbgw_readData[1] <<  8) +
				(f_dbgw_readData[0]);

	/* 正常なデータの場合にのみ出力する	*/
	if( (uiFileSize < D_RRH_QSPI_FLASH_TROUBLELOG_SIZE) &&
		(uiFileSize != 0) )
	{
		/* /var/logにtrouble0X.tar.gzとして出力する	*/
		/* Xは障害ログ面							*/
		output_w = fopen( filename[filename_info] , "w");

		if( NULL != output_w )
		{
			/* ファイルに出力する	*/
			fwrite( &f_dbgw_readData[4], 1, uiFileSize, output_w );

			/* ファイルクローズ	*/
			fclose(output_w);
		}
	}


	return 0;
}


