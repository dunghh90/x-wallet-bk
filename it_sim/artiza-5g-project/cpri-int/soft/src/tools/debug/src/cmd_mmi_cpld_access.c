/*!
 * @skip   $Id$
 * @file   cmd_mmi_cpld_access.c
 * @brief  CPLDアクセス
 * @date   2015/04/28 ALPHA)横山 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"									/*!< DEBGU共通ヘッダ	*/


extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );

/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return
 * @date 2015/04/28 ALPHA)横山 create
 */


void cmd_mmi_cpld_access_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE  : cmd_mmi_cpld_access [R/W] [addr] [size/data]\n");
	fprintf(outfp, "[R/W]       : r / w \n");
	fprintf(outfp, "[addr]      : address \n");
	fprintf(outfp, "[size/data] : Read=size / Write=data \n");
}


/*!
 * @brief 関数機能概要:MMI CPLDアクセスコマンド　メイン処理
 * @note  関数処理内容.
 *       -# MMI CPLDへのアクセスを行う
 *       -# パラメタにより次の処理を行う。
 *       -#   cmd_mmi_cpld_access r regaddr readsize
 *       -#      regaddr で指定したレジスタアドレスからreadsize分レジスタの内容を読み込み表示する
 *       -#   cmd_mmi_cpld_access dev w regaddr writedata
 *       -#      regaddr で指定したレジスタアドレスへwritedataで指定した値を書き込む
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2015/04/27 ALPHA)横山 create
 */
int cmd_mmi_cpld_access_main( int argc, char *argv[], FILE *outfp )	{

	unsigned int	readsize	;
	unsigned int	regaddr		;
	unsigned int	regaddr_inf	;
	unsigned int	readdat		;
	int				i			;
	unsigned int	writedat	;
	unsigned int	result		;	/* 処理結果	*/
	unsigned int	access_type = 2	;

	/* パラメータェック	*/
	if( argc != 4 )	/* 引数が不一致	*/
	{
		/* 引数の数が合わなければUsage表示	*/
		cmd_mmi_cpld_access_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}

	/* アドレス変換	*/
	if( cnvHexStr2Long( argv[2], &regaddr )	!= RTC_OK ) {
		fprintf(stderr, "Error:address error :[%s]\n", argv[2]) ;
		return  D_DBG_CMD_RST_ERROR ;
	}
	
	/* サイズ/Data変換	*/
	if( cnvHexStr2Long( argv[3], &readsize ) != RTC_OK ) {
		fprintf(stderr, "Error: size/data invalid.\n");
		return D_DBG_CMD_RST_ERROR ;
	}
	
	/* 書き込み値は同じ	*/
	writedat = readsize;
	
	/* Read	*/
	if( strcmp( argv[1] , "R" ) == 0  || strcmp( argv[1], "r" ) == 0  )
	{
		/* CPLDの場合は2byte単位	*/
		if( readsize < 2 )
		{
			readsize = 2;
		}
		
		regaddr_inf = regaddr;
		fprintf(outfp, "%08x : ", regaddr_inf);
		for( i = 0; i < readsize; i += access_type )
		{
			/* MMI CPLD READ	*/
			result = BPF_HM_DEVC_MMI_CPLD_READ( regaddr, &readdat );
			
			if( result != BPF_HM_DEVC_COMPLETE ) {	/* 読み込みエラー	*/
				fprintf(stderr, "Error : read error code:%08x\n", result );
				return D_DBG_CMD_RST_ERROR ;
			}
			
			fprintf(outfp, "%04x ",(readdat & 0xFFFF));
			
			if( (i + access_type)%16==0 )
			{
				if( (i + access_type)==regaddr )
				{
					/* 次がなければアドレス表示はしない	*/
					break;
				}
				fprintf(outfp, "\n");
				regaddr_inf += 0x10;
				fprintf(outfp, "%08x : ", regaddr_inf);
			}
			regaddr+=access_type;
		}
		fprintf(outfp, "\n");

	}
	/* Write	*/
	else if( strcmp( argv[1] , "W" ) == 0  || strcmp( argv[1], "w" ) == 0  )
	{
		/* MMI CPLD WRITE	*/
		result = BPF_HM_DEVC_MMI_CPLD_WRITE( regaddr, &writedat );
		
		if( result != BPF_HM_DEVC_COMPLETE ) {	/* 書き込みエラー	*/
			fprintf(stderr, "Error: write error code:%08x\n", result );
			return D_DBG_CMD_RST_ERROR ;
		}
	}
	/* パラメータエラー	*/
	else
	{
		cmd_mmi_cpld_access_print_usage(outfp) ;
		return D_DBG_CMD_RST_ERROR	;
	}

	return D_DBG_CMD_RST_SUCCESS	;
}

/* @} */

