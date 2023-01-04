/*!
 * @skip   $Id$
 * @file   cmd_reg_access.c
 * @brief  レジスターアクセス
 * @date   2013/11/21 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"									/*!< DEBGU共通ヘッダ	*/


extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );

#define		D_SYS_REG_IO_PERI			0xe0000000
#define		D_SYS_REG_IO_PERI_END		0xe02f0000
/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return
 * @date 2013/11/21 ALPHA)宮崎 create
 */


void cmd_reg_access_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE : cmd_reg_access can read/write register\n");
	fprintf(outfp, "read  : ./cmd_reg_access r regaddr readsize\n");
	fprintf(outfp, "write : ./cmd_reg_access w regaddr writedata\n");
}



/*!
 * @brief 関数機能概要:レジスタアクセスコマンド　メイン処理
 * @note  関数処理内容.
 *       -# レジスタへのアクセスを行う
 *       -# パラメタにより次の処理を行う。
 *       -#   cmd_reg_access r regaddr readsize
 *       -#      regaddr で指定したレジスタアドレスからreadsize分レジスタの内容を読み込み表示する
 *       -#   cmd_reg_access w regaddr writedata
 *       -#      regaddr で指定したレジスタアドレスへwritedataで指定した値を書き込む
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/21 ALPHA)宮崎 create
 */
int cmd_reg_access_main( int argc, char *argv[], FILE *outfp )	{

	unsigned int	readsize	;
	unsigned int	regaddr		;
	unsigned int	regaddr_inf	;
	unsigned int	readdat		;
	int				i			;
	unsigned int	writedat	;
	unsigned int	result		;	/* 処理結果	*/


	/* パラメータェック	*/
	if( argc != 4 )	/* 引数が不一致	*/
	{
		/* 引数の数が合わなければUsage表示	*/
		cmd_reg_access_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}

	/* アドレス変換	*/
	if( cnvHexStr2Long( argv[2], &regaddr )	!= RTC_OK ) {
		fprintf(stderr, "Error:address error :[%s]\n", argv[2]) ;
		return  D_DBG_CMD_RST_ERROR ;
	}


	if( strcmp( argv[1] , "R" ) == 0  || strcmp( argv[1], "r" ) == 0  )	{		/* レジスタリード	*/

		if( cnvHexStr2Long( argv[3], &readsize ) != RTC_OK ) {	/* 読み込みサイズ	*/
			fprintf(stderr, "Error: read size invalid.\n");
			return D_DBG_CMD_RST_ERROR ;
		}
		if( readsize < 4 )
		{
			readsize = 4;
		}
		regaddr_inf = regaddr;
		fprintf(outfp, "%08x : ", regaddr_inf);
		for( i = 0; i < readsize; i += 4 )
		{
			if( (D_SYS_REG_IO_PERI <= regaddr) && (regaddr <= D_SYS_REG_IO_PERI_END) ) {

				if(( result = BPF_HM_DEVC_GPIO_READ( DBG_CMD_LOGLEVEL, regaddr, &readdat )) != BPF_HM_DEVC_COMPLETE ) {	/* 読み込みエラー	*/
					fprintf(stderr, "Error:register(GPIO) read error code:%08x\n", result );
					return D_DBG_CMD_RST_ERROR ;
				}
			}
			else {
				if(( result = BPF_HM_DEVC_REG_READ( DBG_CMD_LOGLEVEL, regaddr, &readdat )) != BPF_HM_DEVC_COMPLETE ) {	/* 読み込みエラー	*/
					fprintf(stderr, "Error:register read error code:%08x\n", result );
					return D_DBG_CMD_RST_ERROR ;
				}
			}
			fprintf(outfp, "%08x ",readdat);
			if( (i+4)%16==0 )
			{
				if( (i+4)==regaddr )
				{
					/* 次がなければアドレス表示はしない	*/
					break;
				}
				fprintf(outfp, "\n");
				regaddr_inf += 0x10;
				fprintf(outfp, "%08x : ", regaddr_inf);
			}
			regaddr+=4;
		}
		fprintf(outfp, "\n");

	}
	else if( strcmp( argv[1] , "W" ) == 0  || strcmp( argv[1], "w" ) == 0  )	{		/* レジスタ	*/

		if( cnvHexStr2Long( argv[3], &writedat )!= RTC_OK ) {	/* 書き込みデータ	*/
			fprintf(stderr, "Error: write data error<%s>\n", argv[3] ) ;
			return D_DBG_CMD_RST_ERROR;
		}
		if( (D_SYS_REG_IO_PERI <= regaddr) && (regaddr <= D_SYS_REG_IO_PERI_END) ) {

			if(( result = BPF_HM_DEVC_GPIO_WRITE( DBG_CMD_LOGLEVEL, regaddr, &writedat ))  != BPF_HM_DEVC_COMPLETE ) {	/* 書き込みエラー	*/
				fprintf(stderr, "Error: register read error code:%08x\n", result );
				return D_DBG_CMD_RST_ERROR ;
			}
		}
		else {
			if(( result = BPF_HM_DEVC_REG_WRITE( DBG_CMD_LOGLEVEL, regaddr, &writedat ))  != BPF_HM_DEVC_COMPLETE ) {	/* 書き込みエラー	*/
				fprintf(stderr, "Error: register read error code:%08x\n", result );
				return D_DBG_CMD_RST_ERROR ;
			}

		}
	}
	else	{	/* パラメータエラー	*/

		cmd_reg_access_print_usage(outfp) ;


		return D_DBG_CMD_RST_ERROR	;
	}


	return D_DBG_CMD_RST_SUCCESS	;
}

/* @} */

