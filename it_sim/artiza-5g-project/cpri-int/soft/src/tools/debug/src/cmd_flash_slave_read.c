/*!
 * @skip   $Id$
 * @file   cmd_flash_slave_read.c
 * @brief  Flash Read
 * @date   2014/12/16 ALPHA)桑村 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"									/*!< DEBGU共通ヘッダ	*/


extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );

#define FLASH_ACCESS_SIZE 0x10000
unsigned char	buffer_uc[FLASH_ACCESS_SIZE];

/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return
 * @date 2014/12/16 ALPHA)桑村 create
 */


void cmd_flash_slave_read_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE : cmd_flash_slave_read can read flash\n");
	fprintf(outfp, "        cmd_flash_slave_read [offset] [readsize]\n");
	fprintf(outfp, "          offset   : HEX(no need 0x)\n");
	fprintf(outfp, "          readsize : DEC. max = 0x10000\n");
}



/*!
 * @brief 関数機能概要:FlashReadコマンドメイン処理(SLAVE用)
 * @note  関数処理内容.
 *       -# FlashReadを行う
 *       -# パラメタにより次の処理を行う。
 *       -#   cmd_flash_slave_read offset readsize
 *       -#      offsetで指定したFlashアドレスからreadsize分Flashの内容を読み込み表示する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2014/12/16 ALPHA)桑村 create
 */
int cmd_flash_slave_read_main( int argc, char *argv[], FILE *outfp )
{
	int				readsize	;
	unsigned int	regaddr		;
	unsigned int	regaddr_inf	;
	unsigned int	offset		;


	/* パラメータェック	*/
	if( argc != 3 )	/* 引数が不一致	*/
	{
		/* 引数の数が合わなければUsage表示	*/
		cmd_flash_slave_read_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}

	/* アドレス変換	*/
	if( cnvHexStr2Long( argv[1], &regaddr )	!= RTC_OK ) {
		fprintf(stderr, "Error:address error :[%s]\n", argv[1]) ;
		return  D_DBG_CMD_RST_ERROR ;
	}
	
	readsize = atoi( argv[2] )	;	/* 読み込みサイズ	*/
	
	/* MAX越えはエラーとする	*/
	if( readsize >= FLASH_ACCESS_SIZE )
	{
		cmd_flash_slave_read_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}
	
	if( readsize < 4 )
	{
		readsize = 4;
	}
	regaddr_inf = regaddr;
	fprintf(outfp, "%08x : ", regaddr_inf);
	regaddr_inf += 0x10;
	
	(VOID)BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ( regaddr, readsize, buffer_uc );
	
	for( offset = 0; offset < readsize; offset++ )
	{
		fprintf( outfp, "%02x", buffer_uc[offset]);
		if( (offset+1)%4==0 )
		{
			fprintf(outfp, " ");
		}
		if( (offset+1)%16==0 )
		{
			fprintf(outfp, "\n%08x : ", regaddr_inf);
			regaddr_inf += 0x10;
		}
	}
	
	fprintf(outfp, "\n");
	
	return D_DBG_CMD_RST_SUCCESS	;
}

/* @} */

