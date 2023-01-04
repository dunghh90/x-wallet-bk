/*!
 * @skip   $Id$
 * @file   traw.c
 * @brief  TRA reAD
 * @date   2013/11/21 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include "f_dbg_cmd_inc.h"
#include "f_rrh_def.h"
extern int BPF_HM_DEVC_SPI_WRITE(unsigned int kind, unsigned int address, unsigned int data );
extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );


/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# DCM-RRH用コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return
 * @date 2013/11/21 ALPHA)宮崎 create
 */


void traw_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE : traw can write  e2prom or spi devices.\n");
	fprintf(outfp, "write eeprom : ./traw eep addr data\n");
	fprintf(outfp, "write spi    : ./traw spi dev-kind addr data\n");
	fprintf(outfp, " spi device kind is \n");
    fprintf(outfp, "    0:CPLD0  /  1:CPLD1  /\n");
    fprintf(outfp, "    2:RFIC0  /  3:RFIC1  /  4:RFIC2  /  5:RFIC3  /\n");
    fprintf(outfp, "    6:VGDAC0 /  7:VGDAC1 /  8:VGDAC2 /  9:VGDAC3 /\n");
    fprintf(outfp, "   10:FWVGA0 / 11:FWVGA1 / 12:FWVGA2 / 13:FWVGA3 /\n");
    fprintf(outfp, "   14:EXTPLL\n");

}
/*!
 * @brief 関数機能概要:TRA ライト EEPROM書き込み機能
 * @note  関数処理内容.
 *       -# E2P　ROMへデータを書き込む
 *
 * @param  addr			[in]unsigned int		EEPROM書き込みアドレス
 * @param  data			[in]unsigned int		EEPROM書き込みデータ
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */
int traw_eep( unsigned int short addr, unsigned int data, FILE *outfp )	{

	int			 	result = 0 ;
	unsigned char 	eepdat;

	/* EEPROM書き込み */
	eepdat = (unsigned char)data ;
	if(( result =  BPF_HM_DEVC_EEPROM_WRITE( addr,  &eepdat )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "EEPROM write error(code:%d)\n", result );
		return( D_DBG_CMD_RST_ERROR ) ;
	}
	fprintf(outfp, "EEPROM write succeed.\n");
	return D_DBG_CMD_RST_SUCCESS;

}
/*!
 * @brief 関数機能概要:TRA ライト SPI書き込み機能
 * @note  関数処理内容.
 *       -# SPI機器へデータを書き込む
 *
 * @param  devno		[in]unsigned int		SPIデバイス指定
 * @param  addr			[in]char*    			SPI書き込みアドレス
 * @param  data			[in]char*				SPI書き込みデータ
 * @param outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */
int traw_spi( unsigned int devno, char *addr, char *data, FILE *outfp  )	{

	extern	int cmd_spi_access_main( int argc, char *argv[], FILE *outfp )	;
	char	*argv_str[5]	;
	char	*dev_str[15] =	{
			"CPLD0",
			"CPLD1",
			"RFIC0",
			"RFIC1",
			"RFIC2",
			"RFIC3",
			"VGDAC0",
			"VGDAC1",
			"VGDAC2",
			"VGDAC3",
			"FWVGA0",
			"FWVGA1",
			"FWVGA2",
			"FWVGA3",
			"EXTPLL"
	};

	/**********
	 */
	argv_str[0]	=	"cmd_spi_access"		;	/* コマンド名	*/
	argv_str[1]	=	dev_str[devno]			;	/* 機器指定		*/
	argv_str[2]	=	"w"						;	/* 読み込み固定	*/
	argv_str[3] = 	addr					;	/* アドレス(cmd_spi_accessで変換しているため文字列で渡す)		*/
	argv_str[4] =	data					;	/* サイズ１固定	*/
	
	return cmd_spi_access_main( 5, argv_str, outfp )	;

}

/*!
 * @brief 関数機能概要:TRA ライトコマンドメイン処理
 * @note  関数処理内容.
 *       -# E2P I2C SPIからデータを読み込む
 *       -# パラメータはそれぞれ以下の通り
 *       -#          $0    $1  $2   $3   $4
 *       -#   e2p    traw  eep addr data
 *       -#   i2c    traw  i2c dev  addr data
 *       -#   spi    traw  spi dev  addr data
 *
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */


int	traw_main( int argc,	char *argv[], FILE *outfp	)	{

	int				writedev = 0					;	/* 書き込み機器 i2c, spiのみ	*/
	unsigned int	regaddr							;	/* 書き込みアドレス	*/
	unsigned int	result	= D_DBG_CMD_RST_SUCCESS	;	/* 処理結果	*/
	unsigned int	writedat						;	/* 書き込み用データ	*/
	
	/* パラメータェック	及び実行 */
	if( argc == 4 )	{	/* e2p 指定?	*/
		if( strcmp( "eep", argv[1]  ) == 0 )	{

			/* アドレス変換	*/
			if( cnvHexStr2Long( argv[2], &regaddr )	!= RTC_OK ) {
				fprintf(outfp, "address error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}
			/* データ変換	*/
			if( cnvHexStr2Long( argv[3], &writedat )	!= RTC_OK ) {
				fprintf(outfp, "data error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}

			/* E2P書き込み処理 */
			result = traw_eep( (unsigned short)(regaddr&0xffff),  writedat&0xffff, outfp  )	;
		}
		else {
			traw_print_usage(outfp);
		}
	}
	else if( argc == 5 ) {	/* その他 i2c or spi */
		if ( strcmp( argv[1], "spi") == 0 )	{	/* spi指定	*/
			
			/* アドレス変換(１６進数として有効か？のチェックのみ) 	*/
			if( cnvHexStr2Long(argv[3], &regaddr )	!= RTC_OK ) {
				fprintf(outfp, "address error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}
			/* データ変換(１６進数として有効か？のチェックのみ)	*/
			if( cnvHexStr2Long( argv[4], &writedat )	!= RTC_OK ) {
				fprintf(outfp, "data error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}
			/* spi書き込み処理 */
			result = traw_spi( writedev, argv[3], argv[4], outfp  );
		}
	}
	else {				/* アーギュメント数不正	*/

		traw_print_usage(outfp);
				
		return D_DBG_CMD_RST_ERROR;
	}


	return result ;
}
/* @} */

