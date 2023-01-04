/*!
 * @skip   $Id$
 * @file   trar.c
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
extern int BPF_HM_DEVC_I2C_READ(unsigned int rev_dv,unsigned int rev_ad,unsigned short *rev_dt);
extern int  cnvHexStr2Long(  char     *hexStr, unsigned int *outVal );
/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# DCM-RRH用コマンドのパラメータエラー等の場合に使い方を表示する
 * @param outfp			[in]FILE*		出力先ファイルポインタ
 * @return
 * @date 2013/11/21 ALPHA)宮崎 create
 */


void trar_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE : trar can read  e2prom or spi or i2c devices.\n");
	fprintf(outfp, " read eeprom : ./trar eep addr data\n");
	fprintf(outfp, " read spi    : ./trar spi dev-kind addr\n");
	fprintf(outfp, "  spi device kind is \n");
    fprintf(outfp, "    0:CPLD0  /  1:CPLD1  /\n");
    fprintf(outfp, "    2:RFIC0  /  3:RFIC1  /  4:RFIC2  /  5:RFIC3  /\n");
    fprintf(outfp, "    6:VGDAC0 /  7:VGDAC1 /  8:VGDAC2 /  9:VGDAC3 /\n");
    fprintf(outfp, "   10:FWVGA0 / 11:FWVGA1 / 12:FWVGA2 / 13:FWVGA3 /\n");
    fprintf(outfp, "   14:EXTPLL\n");

}

int trar_eep( unsigned short addr, FILE *outfp )	{

	unsigned int readdat ;
	int			 result;

	/* EEPROM読み込み */
	if(( result =  BPF_HM_DEVC_EEPROM_READ( addr, (unsigned char*)&readdat )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "EEPROM read error(code:%d)\n", result );
		return( D_DBG_CMD_RST_ERROR ) ;
	}
	/* 読み込み内容表示 */
	fprintf(outfp, "EEPROM [%04x] : %02x\n", addr&0xffff, readdat & 0xff );
	return D_DBG_CMD_RST_SUCCESS;

}

/*!
 * @brief 関数機能概要:TRA リード SPI読み込み機能
 * @note  関数処理内容.
 *       -# SPI機器よりデータを読み込む
 *
 * @param  dev			[in]unsigned int		SPIデバイス指定
 * @param  addr			[in]unsigned int		SPI読み込みアドレス
 * @param outfp			[in]FILE*		出力先ファイルポインタ
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 * @date 2015/05/12 ALPHA)宮崎 TDD対応
 */
int trar_spi( unsigned int devno, char *addr, FILE *outfp  )	{
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
	argv_str[2]	=	"r"						;	/* 読み込み固定	*/
	argv_str[3] = 	addr					;	/* アドレス(cmd_spi_accessで変換しているため文字列で渡す)		*/
	argv_str[4] =	"1"						;	/* サイズ１固定	*/

	return cmd_spi_access_main( 5, argv_str, outfp )	;
}


/*!
 * @brief 関数機能概要:TRA リードコマンド
 * @note  関数処理内容.
 *       -# デバッグ共通初期化処理を実行する(dbg_common_init)
 *       	-  エラー発生時
 *       		-# エラーメッセージを表示し上位へエラーを返す
 *       	-  パラメータ数=3の時
 *       		- argv[1] が "eep"の時
 *       			-# EEPROM読み込みアドレスを数値に変換する
 *       				-  変換エラーの場合
 *       					-# アドレスエラーを表示し上位へエラーを返す
 *       			-# EEPROM読み込み処理を実行(trar_eep)
 *       		- argv[1] が "eep"以外の時
 *       			-# エラーメッセージを表示上位へエラーを返す
 *       	-  パラメータ数=4の時
 *       		- argv[1] が "spi"の時
 *       			-# EEPROM読み込みアドレスを数値に変換する
 *       				-  変換エラーの場合
 *       					-# アドレスエラーを表示し上位へエラーを返す
 *       			-# EEPROM読み込み処理を実行(trar_spi)
 *       		- argv[1] が "spi"以外の時
 *       			-# エラーメッセージを表示上位へエラーを返す
 *       -# デバッグ共通終了処理を実行
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */


int trar_main( int argc, char *argv[], FILE *outfp  )	{
	int				readdev = 0 ;	/* 読み込み機器 i2c, spiのみ	*/
	unsigned int	regaddr		;	/* 読み込みアドレス	*/
	unsigned int	result		;	/* 処理結果	*/
	
	/* パラメータェック	及び実行 */
	if( argc == 3 )	{	/* e2p 指定?	*/
		if( strcmp( argv[1], "eep" ) == 0 )	{

			/* アドレス変換	*/
			if( cnvHexStr2Long( argv[2], &regaddr )	!= RTC_OK ) {
				fprintf(stderr, "Error: address error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}

			/* E2P読み込み処理 */
			result = trar_eep( (unsigned short)regaddr, outfp  )	;
		}
		else {
			trar_print_usage(outfp);
			return D_DBG_CMD_RST_ERROR;
		}
	}
	else if( argc == 4 ) {	/* その他 i2c or spi */
		
		if ( strcmp( argv[1], "spi") == 0 )	{	/* spi指定	*/
			
			/* デバイス番号取得	*/
			readdev = atoi(argv[2]);
			if( (readdev < 0) || (readdev > 14) )	{
				fprintf(stderr, "Error: dev no invalid.\n");
				return D_DBG_CMD_RST_ERROR;
			}
			
			/* アドレス変換	*/
			if( cnvHexStr2Long( argv[3], &regaddr )	!= RTC_OK ) {
				fprintf(stderr, "Error: address error\n") ;
				return  D_DBG_CMD_RST_ERROR ;
			}

			/* SPI読み込み処理 */
			result = trar_spi( readdev, argv[3], outfp  );
		}
		else {
			trar_print_usage(outfp);
			return D_DBG_CMD_RST_ERROR;
		}
	}
	else {				/* アーギュメント数不正	*/
		trar_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}


	return result ;
}
/* @} */

