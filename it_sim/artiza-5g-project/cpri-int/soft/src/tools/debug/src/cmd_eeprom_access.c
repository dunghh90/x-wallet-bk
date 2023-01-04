/*!
 * @skip   $Id$
 * @file   cmd_eeprom_access.c
 * @brief  EEPROMアクセス
 * @date   2013/11/21 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include 	"f_dbg_cmd_inc.h"

extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );

/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp	[in]FILE*	出力先ファイルポインタ
 * @return
 * @date 2013/11/22 ALPHA)宮崎 create
 */


VOID cmd_eeprom_access_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE : cmd_eeprom_access can read/write eeprom\n");
	fprintf(outfp, "read  : ./cmd_eeprom_access r regaddr readsize\n");
	fprintf(outfp, "write : ./cmd_eeprom_access w regaddr writedata\n");

}


/*!
 * @brief 関数機能概要:EEPROMアクセスコマンド　メイン処理
 * @note  関数処理内容.
 *       -# EEPROMへのアクセスを行う
 *       -# パラメタにより次の処理を行う。
 *       -#   cmd_eeprom_access r regaddr readsize
 *       -#      regaddr で指定したEEPROMアドレスからreadsize分EEPROMの内容を読み込み表示する
 *       -#   cmd_eeprom_access w regaddr writedata
 *       -#      regaddr で指定したEEPROMアドレスへwritedataで指定した値を書き込む
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */

int	cmd_eeprom_access_main( int argc,	char *argv[], FILE *outfp	)	{

	int				readsize	;
	unsigned int	regaddr		;
	unsigned int	readdat		;
	int				i			;
	unsigned int	writedat	;
	unsigned int	result		;	/* 処理結果	*/

	/* パラメータェック	*/
	if( argc != 4 )	/* 引数が不一致	*/
	{
		/* 引数の数が合わなければUsage表示	*/
		cmd_eeprom_access_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}

	/* アドレス変換	*/
	if( cnvHexStr2Long( argv[2], &regaddr )	!= RTC_OK ) {
		fprintf(stderr,"Error: address format error[%s]\n",argv[2]) ;
		return  D_DBG_CMD_RST_ERROR ;
	}

	if( strcmp( argv[1] , "R" ) == 0  || strcmp( argv[1], "r" ) == 0  )	{		/* EEPROMリード	*/

		readsize = atoi( argv[3] )	;	/* 読み込みサイズ	*/

		fprintf(outfp, "%04x : ", regaddr);
		for( i = 0; i < readsize; i++ )
		{
			if(( result = BPF_HM_DEVC_EEPROM_READ( (unsigned short)regaddr, (unsigned char *)&readdat )) != BPF_HM_DEVC_COMPLETE ) {	/* 読み込みエラー	*/
				fprintf(stderr, "Error: register read error addr:%08x code:%08x\n", regaddr, result );
				return D_DBG_CMD_RST_ERROR ;
			}
			if( i%16==0 && i !=  0 )		{
				fprintf(outfp, "\n");
	//			regaddr += 0x10;
				fprintf(outfp, "%04x : ", regaddr);
			}
			fprintf(outfp, "%02x ",readdat&0xff);
			regaddr++;
		}
		fprintf(outfp, "\n");

	}
	else if( strcmp( argv[1] , "W" ) == 0  || strcmp( argv[1], "w" ) == 0  )	{		/* EEPROM	*/

		if( cnvHexStr2Long( argv[3], &writedat )!= RTC_OK ) {	/* 書き込みデータ	*/
			fprintf(stderr, "Error: write data format error\n" ) ;
			return D_DBG_CMD_RST_ERROR;
		}
		if(( result = BPF_HM_DEVC_EEPROM_WRITE( regaddr, (unsigned char *)&writedat ))  != BPF_HM_DEVC_COMPLETE ) {	/* 書き込みエラー	*/
			fprintf(stderr, "Error: register write error code:%08d\n", result );
			return D_DBG_CMD_RST_ERROR ;
		}
	}
	else	{	/* パラメータエラー	*/
		cmd_eeprom_access_print_usage(outfp) ;
		return D_DBG_CMD_RST_ERROR;
	}

	return D_DBG_CMD_RST_SUCCESS;
}

/* @} */

