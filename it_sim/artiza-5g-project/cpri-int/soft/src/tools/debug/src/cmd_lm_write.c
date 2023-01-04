/*!
 * @skip   $Id$
 * @file   cmd_lm_write.c
 * @brief  LM書き込み
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>

#include	"f_dbg_cmd_inc.h"
#include	"BPF_COM_END.h"


#include "BPF_HM_DEVC.h"

#define TEMP_BUFSIZE 0x10000
#define KIND_QSPI 0
#define KIND_SPI  1

/**
 * @name フラッシュマップ
 * @note フラッシュ書き込み時に利用し、書き込み時に必要な情報を定義する
 */
typedef struct {
	const char			*select			;	/* 選択番号						*/
	const char			*type			;	/* 書き込み種別					*/
	unsigned	int		side			;	/* 書き込み面					*/
	unsigned 	int		start_adr		;	/* 書き込み開始アドレス			*/
	unsigned	int		era_size		;	/* 消去サイズ					*/
	unsigned	int		era_flg_adr		;	/* EEPROM消去フラグアドレス		0xffffffff時EEPROM更新対象外	*/
	unsigned	int		file_size_adr	;	/* EEPROMファイルサイズアドレス(前半)	0xffffffff時EEPROM更新対象外	*/
} T_FLASH_INFO	;

/**
 * @name フラッシュマップ
 * @note フラッシュ書き込み時に利用し、書き込み時に必要な情報を定義する
 */
T_FLASH_INFO	flashMapInfo[]	=	{
/*      sel     type			side	start     								erasize  	eraflg		filesize */
	{	"0",	"Boot/TP",		0,		D_RRH_QSPI_FLASH_OFFSET_BOOT,			0x00100000,	0xffffffff,	0xffffffff},
	{	"1",	"Boot/TP",		1,		D_RRH_QSPI_FLASH_OFFSET_BOOT,			0x00100000,	0xffffffff,	0xffffffff	},

	{	"2",	"Boot P",		0,		D_RRH_QSPI_FLASH_OFFSET_BOOT,			0x00080000,	0xffffffff,	0xffffffff	},
	{	"3",	"Boot B",		0,		D_RRH_QSPI_FLASH_OFFSET_BOOT_2,			0x00080000,	0xffffffff,	0xffffffff	},
	{	"4",	"Boot P",		1,		D_RRH_QSPI_FLASH_OFFSET_BOOT,			0x00080000,	0xffffffff,	0xffffffff	},
	{	"5",	"Boot B",		1,		D_RRH_QSPI_FLASH_OFFSET_BOOT_2,			0x00080000,	0xffffffff,	0xffffffff	},

	{	"10",	"APL OS",		0,		D_RRH_QSPI_FLASH_OFFSET_OS,				0x00700000,	D_RRH_EEP_DL_ERASE_FLG_FIRM0,	D_RRH_EEP_FIRM0_CHECKSUM_CALC_H0	},
	{	"11",	"APL OS",		1,		D_RRH_QSPI_FLASH_OFFSET_OS,				0x00700000,	D_RRH_EEP_DL_ERASE_FLG_FIRM0,	D_RRH_EEP_FIRM1_CHECKSUM_CALC_H0	},

	{	"20",	"PS-FPGA",		0,		D_RRH_QSPI_FLASH_OFFSET_PL,				0x00400000,	D_RRH_EEP_DL_ERASE_FLG_RFRX0,	D_RRH_EEP_FPGA0_CHECKSUM_CALC_H0	},
	{	"21",	"PS-FPGA",		1,		D_RRH_QSPI_FLASH_OFFSET_PL,				0x00400000,	D_RRH_EEP_DL_ERASE_FLG_RFRX0,	D_RRH_EEP_FPGA1_CHECKSUM_CALC_H0	},

	{	"30",	"RFIC",			0,		D_RRH_QSPI_FLASH_OFFSET_RFIC,			0x00090000,	0xffffffff,	0xffffffff	},
	{	"31",	"RFIC",			1,		D_RRH_QSPI_FLASH_OFFSET_RFIC,			0x00090000,	0xffffffff,	0xffffffff	},
	
	{	"40",	"RFIC-DATA",	0,		D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX,	0x00010000,	0xffffffff,	0xffffffff	},
	{	"41",	"RFIC-DATA",	1,		D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX,	0x00010000,	0xffffffff,	0xffffffff	},
	
	{	"50",	"Slave-FPGA",	0,		D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE0,0x01000000,	D_RRH_EEP_DL_ERASE_FLG_FPGA0,	D_RRH_EEP_APDIC0_CHECKSUM_CALC_H0	},
	{	"51",	"Slave-FPGA",	1,		D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE1,0x01000000,	D_RRH_EEP_DL_ERASE_FLG_FPGA0,	D_RRH_EEP_APDIC1_CHECKSUM_CALC_H0	},
};

T_FLASH_INFO *flashMapInfo_ptr = NULL;
int flashMapLen = 0;
/*!
 * @brief 関数機能概要:使い方表示
 * @note  関数処理内容.
 *       -# cmd_lm_versionの使い方を表示する。
 * @param  outfp	[in]FILE* 出力先ファイルポインタ
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */

void cmd_lm_write_print_usage(FILE *outfp ){
	int	flashMapIndex	;

	fprintf(outfp,     "Usage : \n");
	fprintf(outfp,     "  cmd_lm_write [filename] [filekind]\n");
	fprintf(outfp,     "    filekind\n");
	for( flashMapIndex = 0 ; flashMapIndex < flashMapLen ; flashMapIndex++ )	{
		fprintf(outfp, "       %2s : Side#%d %s\n",
					flashMapInfo_ptr[flashMapIndex].select,
					flashMapInfo_ptr[flashMapIndex].side,
					flashMapInfo_ptr[flashMapIndex].type	)	;
	}
}

/*!
 * @brief 関数機能概要:LM書き込みメイン処理
 * @note  関数処理内容.
 *       -# LMの書き込みを行う。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*	コマンドアーギュメント
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */
int cmd_lm_write_main(int argc, char *argv[], FILE* outfp )
{
	int					lmFd;
	unsigned char		buffer[TEMP_BUFSIZE];
	int					iReadSize;
	int					iRet;
	int					foundIndex		= -1 ;
	unsigned char		eepromData		=	0;
	unsigned int		flashWriteSize	=	0;
	unsigned int		uiBaseAddr		=	0;
	unsigned char		*eepWritePointer	=	NULL;
	unsigned int		eepWriteIndex		;
	struct		stat	fst	;


	flashMapInfo_ptr = flashMapInfo;
	flashMapLen = 12;


	if( NULL==argv[1] || NULL==argv[2]  )
	{
		/* 使用法説明	*/
		cmd_lm_write_print_usage(outfp);
		return 0;
	}

	/* 選択肢から指定された組み合わせをフラッシュマップから探す	*/
	{
		int flashmapInd ;	/* */
		for( flashmapInd = 0 ; flashmapInd < flashMapLen ; flashmapInd++ )		{	/* フラッシュマップテーブル数分	*/
			if( strcmp(argv[2], flashMapInfo_ptr[flashmapInd].select ) == 0  )	{	/* 選択肢と一致 */

				/* 指定された選択肢と一致した場合	*/
				foundIndex = flashmapInd;
				break;
			}
		}
	}

	if( foundIndex == -1 )	{	/*指定された選択肢が見つからない場合	*/
		/* 使用法説明表示	*/
		cmd_lm_write_print_usage(outfp);
		return 1;
	}

    /* 以降は見つかった場合の処理	*/

	fprintf(outfp, "Side#%d - %s\n",flashMapInfo_ptr[foundIndex].side, flashMapInfo_ptr[foundIndex].type );

	/* 書き込みファイルのサイズを取得 */
	if ( stat( argv[1], &fst ) < 0 )	{
		fprintf(stderr, "Error: file not found (errno:%d)\n", errno );
		return 1;
	}
	/* 消去サイズよりも大きなファイルの場合エラーとする	*/
	if( fst.st_size > flashMapInfo_ptr[foundIndex].era_size )	{
		fprintf(stderr, "Error: file size to big.\n");
		return 1;
	}

	/* EEPROMフラッシュ消去フラグ更新	*/
	eepromData = 0 ;				/* イレース未(0)へ設定	*/
	if( flashMapInfo_ptr[foundIndex].era_flg_adr   != 0xffffffff ) {	/* EEPROMフラッシュ消去フラグ設定対象？*/

		if( BPF_HM_DEVC_EEPROM_WRITE( flashMapInfo_ptr[foundIndex].era_flg_adr,   &eepromData ) != BPF_HM_DEVC_COMPLETE ){
			fprintf(stderr, "Error:EEPROM Flash erase flag(%s)-0\n",flashMapInfo_ptr[foundIndex].type );
			return 1;
		}
		if( BPF_HM_DEVC_EEPROM_WRITE( flashMapInfo_ptr[foundIndex].era_flg_adr+1, &eepromData ) != BPF_HM_DEVC_COMPLETE ){
			fprintf(stderr, "Error:EEPROM Flash erase flag(%s)-1\n",flashMapInfo_ptr[foundIndex].type );
			return 1;
		}
	}

	/* EEPROMフラッシュサイズ更新	*/
	flashWriteSize = 0 ;				/* サイズ０へ設定	*/
	if( flashMapInfo_ptr[foundIndex].file_size_adr   != 0xffffffff ) {	/* EEPROMフラッシュ消去フラグ設定対象？*/

		eepWritePointer = (unsigned char *)&flashWriteSize	;
		for( eepWriteIndex = 0 ; eepWriteIndex < 4 ; eepWriteIndex++ )	{
			if( BPF_HM_DEVC_EEPROM_WRITE( flashMapInfo_ptr[foundIndex].file_size_adr + eepWriteIndex,   ( eepWritePointer + eepWriteIndex ) ) != BPF_HM_DEVC_COMPLETE ){
				fprintf(stderr, "Error:EEPROM Flash Write Size to 0 fail.(%s-Side#%d)-%d\n", flashMapInfo_ptr[foundIndex].type, flashMapInfo_ptr[foundIndex].side, eepWriteIndex );
				return 1;
			}
		}
	}
	
	/*フラッシュ消去*/
	fprintf(outfp, "QSPI flash erase. side:%d addr:0x%08x size:0x%08x\n",
			flashMapInfo_ptr[foundIndex].side,
			flashMapInfo_ptr[foundIndex].start_adr,
			flashMapInfo_ptr[foundIndex].era_size	);
	if( strcmp(flashMapInfo_ptr[foundIndex].type, "Slave-FPGA") == 0) {
		if((iRet = BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(flashMapInfo_ptr[foundIndex].start_adr, flashMapInfo_ptr[foundIndex].era_size)) != BPF_HM_DEVC_COMPLETE ) {
			/*消去エラー	*/
			fprintf(stderr, "Error: Flash erace... code:(%d)\n", iRet );
			return 1;
		}
	} else {
		if(( iRet = BPF_HM_DEVC_QSPI_FLASH_ERASE(
							flashMapInfo_ptr[foundIndex].side,
							flashMapInfo_ptr[foundIndex].start_adr,
							flashMapInfo_ptr[foundIndex].era_size	))	!= BPF_HM_DEVC_COMPLETE ) {
			/*消去エラー	*/
			fprintf(stderr, "Error: Flash erace... code:(%d)\n", iRet );
			return 1;
		}
	}
	fprintf(outfp, "Erase complete.\n");

	/* 読み込み領域初期化	*/
	memset( buffer, 0xFF, sizeof(buffer) );

	/* File Open	*/
	lmFd = open( (char*)argv[1] , O_RDONLY);
	if( lmFd == -1 )	{
		fprintf(stderr, "Error: cannot open %s \n", argv[1]);
		return 1;
	}
	lseek(lmFd,0,SEEK_SET);	/* ファイルの先頭へ移動	*/

	/* フラッシュ書き込み	*/
	fprintf(stderr, "QSPI flash write. side:%d addr:0x%08x\n",
			flashMapInfo_ptr[foundIndex].side,
			flashMapInfo_ptr[foundIndex].start_adr );

	uiBaseAddr = flashMapInfo_ptr[foundIndex].start_adr	;	/* 書き込みアドレスの設定(初期化)	*/
	while(1)
	{
		iReadSize = read( lmFd, buffer, TEMP_BUFSIZE );
		if( iReadSize == -1 ) {
			fprintf(stderr, "Error: File read error.(%d)\n", errno ) ;
			close(lmFd);
			return 1;
		}
		if( iReadSize == 0 ) { /* 最後	*/
			break;
		}
		/* フラッシュ書き込み	*/
		if( strcmp(flashMapInfo_ptr[foundIndex].type, "Slave-FPGA") == 0 ) {
			iRet = BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(
				uiBaseAddr,
				iReadSize,
				buffer);
		} else {
			iRet = BPF_HM_DEVC_QSPI_FLASH_WRITE(
						flashMapInfo_ptr[foundIndex].side,
						uiBaseAddr,
						iReadSize,
						buffer);
		}
		if( iRet != BPF_HM_DEVC_COMPLETE )
		{
			fprintf(stderr,"write fail\n");
			break;
		}
		memset( buffer, 0xFF, sizeof(buffer) );	/* 読み込み領域初期化	*/
		uiBaseAddr += iReadSize;		/* 書き込みアドレスの設定（更新)	*/
		flashWriteSize +=  iReadSize;	/* 書き込みサイズの更新	*/
		fprintf(outfp, ".");
		fflush(outfp);
	}
	fprintf(outfp, "\n");

	close( lmFd );	/* ファイルクローズ	*/

	if( iRet == BPF_HM_DEVC_COMPLETE )	{	/* 最後まで正常に書きこめた?	*/

		/* EEPROMチェックサム計算用サイズ設定	*/
		if( flashMapInfo_ptr[foundIndex].file_size_adr   != 0xffffffff ) {	/* EEPROMチェックサム計算用サイズ設定対象？*/
			eepWritePointer = (unsigned char *)&flashWriteSize	;
			BPF_COM_END_LITTLETOBIG32(flashWriteSize);
			for( eepWriteIndex = 0 ; eepWriteIndex < 4 ; eepWriteIndex++ )	{
				if( BPF_HM_DEVC_EEPROM_WRITE( flashMapInfo_ptr[foundIndex].file_size_adr + eepWriteIndex,   ( eepWritePointer + eepWriteIndex ) ) != BPF_HM_DEVC_COMPLETE ){
					fprintf(stderr, "Error:EEPROM Flash Write Size to %d fail.(%s-Side#%d)-%d\n", flashWriteSize, flashMapInfo_ptr[foundIndex].type, flashMapInfo_ptr[foundIndex].side, eepWriteIndex );
					return 1;
				}
			}
		}

	}


	return (iRet == BPF_HM_DEVC_COMPLETE )?  D_DBG_CMD_RST_SUCCESS: D_DBG_CMD_RST_ERROR ;
}


