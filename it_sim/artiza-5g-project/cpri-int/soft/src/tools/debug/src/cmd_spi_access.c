/*!
 * @skip   $Id$
 * @file   cmd_spi_access.c
 * @brief  SPIアクセス
 * @date   2015/04/27 ALPHA)横山 Create
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
 * @date 2015/04/27 ALPHA)横山 create
 */


void cmd_spi_access_print_usage(FILE *outfp)
{
	fprintf(outfp, "USAGE  : cmd_spi_access [dev] [R/W] [addr] [size/data]\n");
	fprintf(outfp, "[dev]  : RFIC0~3 / EXTPLL \n");
	fprintf(outfp, "[R/W]  : r / w \n");
	fprintf(outfp, "[addr] : address \n");
	fprintf(outfp, "[size/data] : Read=size / Write=data \n");
	fprintf(outfp, "\n");
	fprintf(outfp, "Example : cmd_spi_access RFIC0 w 10 FF\n");
}


/*!
 * @brief 関数機能概要:SPIアクセスコマンド　メイン処理
 * @note  関数処理内容.
 *       -# SPIへのアクセスを行う
 *       -# パラメタにより次の処理を行う。
 *       -#   cmd_spi_access dev r regaddr readsize
 *       -#      regaddr で指定したレジスタアドレスからreadsize分レジスタの内容を読み込み表示する
 *       -#   cmd_spi_access dev w regaddr writedata
 *       -#      regaddr で指定したレジスタアドレスへwritedataで指定した値を書き込む
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2015/04/27 ALPHA)横山 create
 */
int cmd_spi_access_main( int argc, char *argv[], FILE *outfp )	{

	unsigned int	readsize	;
	unsigned int	regaddr		;
	unsigned int	regaddr_inf	;
	unsigned int	readdat		;
	int				cnt			;
	unsigned int	writedat	;
	unsigned int	result		;	/* 処理結果	*/
	unsigned int	branch = 0		;
	unsigned int	unit = 0		;
	unsigned int	devkind		;
	unsigned int	access_type = 0	;
	unsigned int	rficflg = 0	;

	/* パラメータェック	*/
	if( argc != 5 )	/* 引数が不一致	*/
	{
		/* 引数の数が合わなければUsage表示	*/
		cmd_spi_access_print_usage(outfp);
		return D_DBG_CMD_RST_ERROR;
	}

	/* アドレス変換	*/
	if( cnvHexStr2Long( argv[3], &regaddr )	!= RTC_OK ) {
		fprintf(stderr, "Error:address error :[%s]\n", argv[3]) ;
		return  D_DBG_CMD_RST_ERROR ;
	}
	
	/* サイズ/Data変換	*/
	if( cnvHexStr2Long( argv[4], &readsize ) != RTC_OK ) {
		fprintf(stderr, "Error: size/data invalid.\n");
		return D_DBG_CMD_RST_ERROR ;
	}
	
	/* 書き込み値は同じ	*/
	writedat = readsize;
	
	/* Device情報取得	*/
	if( strcmp( argv[1] , "RFIC0" ) == 0 ) {
		devkind = 1;
		branch = BPF_HM_DEVC_RFIC_TXRX_KIND_TX;
		unit   = BPF_HM_DEVC_RFIC_UNIT_KIND0;
		access_type = 1;			/* RFICは2byte単位でREAD/WRITEするが		*/
		rficflg = 1;				/* アドレスは1ずつ遷移するという特殊な処理	*/
	}else if( strcmp( argv[1] , "RFIC1" ) == 0 ) {
		devkind = 1;
		branch = BPF_HM_DEVC_RFIC_TXRX_KIND_TX;
		unit   = BPF_HM_DEVC_RFIC_UNIT_KIND1;
		access_type = 1;
		rficflg = 1;
	}else if( strcmp( argv[1] , "RFIC2" ) == 0 ) {
		devkind = 1;
		branch = BPF_HM_DEVC_RFIC_TXRX_KIND_RX;
		unit   = BPF_HM_DEVC_RFIC_UNIT_KIND0;
		access_type = 1;
		rficflg = 1;
	}else if( strcmp( argv[1] , "RFIC3" ) == 0 ) {
		devkind = 1;
		branch = BPF_HM_DEVC_RFIC_TXRX_KIND_RX;
		unit   = BPF_HM_DEVC_RFIC_UNIT_KIND1;
		access_type = 1;
		rficflg = 1;
	}else if( strcmp( argv[1] , "EXTPLL" ) == 0 ) {
		devkind = 4;
		access_type = 1;
	}else{
		fprintf(stderr, "Error: devkind invalid.\n");
		return D_DBG_CMD_RST_ERROR ;
	}
	
	/* Read	*/
	if( strcmp( argv[2] , "R" ) == 0  || strcmp( argv[2], "r" ) == 0  )
	{
		/* CPLD/RFICの場合は2byte単位, PLL/DAC/VGAは1byte単位	*/
		if( access_type == 2 )
		{
			if( readsize < 2 )
			{
				readsize = 2;
			}
		}
		
		regaddr_inf = regaddr;
		fprintf(outfp, "%08x : ", regaddr_inf);
		for( cnt = 0; cnt < readsize; cnt += access_type )
		{
			switch( devkind ){
			case 1:		/* RFIC	*/
				result = BPF_HM_DEVC_RFIC_READ( branch, unit, regaddr, &readdat );
				break;
			case 4:		/* EXTPLL	*/
				result = BPF_HM_DEVC_EXT_PLL_READ( regaddr, &readdat );
				break;
			default:
				/* 通らない	*/
				return D_DBG_CMD_RST_ERROR;
			}
			
			if( result != BPF_HM_DEVC_COMPLETE ) {	/* 読み込みエラー	*/
				fprintf(stderr, "Error : read error code:%08x\n", result );
				return D_DBG_CMD_RST_ERROR ;
			}
			
			/* RFICはアドレスが1ずつずれるが、1アドレスで2byteのアクセス	*/
			if( (access_type == 2) || (rficflg == 1) )
			{
				fprintf(outfp, "%04x ",(readdat & 0xFFFF));
			}
			else
			{
				fprintf(outfp, "%02x ",(readdat & 0xFF));
			}
			
			if( (cnt + access_type)%16==0 )
			{
				if( (cnt + access_type)==regaddr )
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
	else if( strcmp( argv[2] , "W" ) == 0  || strcmp( argv[2], "w" ) == 0  )
	{
		switch( devkind ){
		case 1:		/* RFIC	*/
			result = BPF_HM_DEVC_RFIC_WRITE( branch, unit, regaddr, writedat );
			break;
		case 4:		/* EXTPLL	*/
			result = BPF_HM_DEVC_EXT_PLL_WRITE( regaddr, writedat );
			break;
		default:
			/* 通らない	*/
			return D_DBG_CMD_RST_ERROR;
		}
		
		if( result != BPF_HM_DEVC_COMPLETE ) {	/* 書き込みエラー	*/
			fprintf(stderr, "Error: write error code:%08x\n", result );
			return D_DBG_CMD_RST_ERROR ;
		}
	}
	/* パラメータエラー	*/
	else
	{
		cmd_spi_access_print_usage(outfp) ;
		return D_DBG_CMD_RST_ERROR	;
	}

	return D_DBG_CMD_RST_SUCCESS	;
}

/* @} */

