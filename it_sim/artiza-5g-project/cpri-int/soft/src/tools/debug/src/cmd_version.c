/*!
 * @skip   $Id$
 * @file   sm.c
 * @brief  set memory
 * @date   2013/11/25 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"
#include    "BPF_COM_END.h"

/**
 * 関数プロトタイプ宣言
 */
extern int  cnvHexStr2Long( unsigned char     *hexStr, unsigned int *outVal );
extern int  BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int shm_num, void **addr, int *errcd);


typedef struct {
	char			bootVersion[5]			;			/**< bootバージョン	(4byte ascii)	*/
	unsigned int	aplTotalVersion			;			/**< アプリバージョン				*/
	unsigned int	aplKernelVersion		;			/**< カーネルバージョン				*/
	unsigned int	aplDevTreeVersion		;			/**< デバイスツリーバージョン		*/
	unsigned int	aplRamDiskVersion		;			/**< ラムディスクバージョン			*/
}	T_APL_VERSION	;


#define D_MAIN_INV_UBOOT_FLASH_OFFSET2			0x00100000								/*!< u-boot.binヘッダーFlash読み込みオフセット2		*/

#define D_UBOOT_FLASH_SIZE1 			0x00000050								/*!< u-boot.binヘッダーFlash読み込みサイズ1			*/
#define D_UBOOT_FLASH_SIZE2 			0x00000004								/*!< u-boot.binヘッダーFlash読み込みサイズ2			*/
#define D_UBOOT_FLASH_SIZE3 			0x00000080								/*!< u-boot.binヘッダーFlash読み込みサイズ3			*/
#define D_FSBL_STARTBIT_BUF1			0x33									/*!< FSBL開始位置バッファー1						*/
#define D_FSBL_STARTBIT_BUF2			0x32									/*!< FSBL開始位置バッファー2						*/
#define D_FSBL_STARTBIT_BUF3			0x31									/*!< FSBL開始位置バッファー2						*/
#define D_FSBL_STARTBIT_BUF4			0x30									/*!< FSBL開始位置バッファー3						*/
#define D_FSBL_SIZE1					0x43									/*!< FSBLサイズ1									*/
#define D_FSBL_SIZE2					0x42									/*!< FSBLサイズ2									*/
#define D_FSBL_SIZE3					0x41									/*!< FSBLサイズ2									*/
#define D_FSBL_SIZE4					0x40									/*!< FSBLサイズ3									*/
#define D_FSBL_MASK1 					0xFF000000								/*!< FSBLデータ取得MASK1							*/
#define D_FSBL_MASK2 					0x00FF0000								/*!< FSBLデータ取得MASK2							*/
#define D_FSBL_MASK3 					0x0000FF00								/*!< FSBLデータ取得MASK3							*/
#define D_BOOTVER_SETPOSITION 			0x0000002C								/*!< Boot Version格納位置							*/


#define	D_FLASH_OS_TOTALVER_OFFSET		4
#define	D_FLASH_OS_KERNELVER_OFFSET		44
#define	D_FLASH_OS_DEVTREEVER_OFFSET	60
#define	D_FLASH_OS_RAMDISIVER_OFFSET	76

/**
 *
 */

int getAprVersion( unsigned char face ) {
	FILE			*versionFileFp				;
	int 			flashFace					;		/* フラッシュ面番号					*/
	unsigned char	flashReadBuffer[512]		;		/* フラッシュデータ読み込みバッファ	*/
	int				rtn_bpf;
	unsigned int	vertmp						;

	if(( versionFileFp = fopen( "/var/log/version", "w" )) != NULL ) 	/* バージョンファイルをオープン成功		*/
	{
		flashFace = face ;

		fprintf(versionFileFp, "Side:%d\n", flashFace );

		memset(flashReadBuffer, 0 , sizeof(flashReadBuffer));

		/* bootは0面固定 */
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER( 0, D_RRH_QSPI_FLASH_OFFSET_BOOT, (UCHAR *)flashReadBuffer );
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			fprintf(stderr, "Error: QSPI Flashread error. face:0 Boot-Ver.(code:%d)\n", rtn_bpf ) ;
			fclose(versionFileFp);
			return -1;
		}
		fprintf(versionFileFp, "  Boot Ver         : %s\n", 	flashReadBuffer	);

		/* APL */
		memset(flashReadBuffer, 0 , sizeof(flashReadBuffer));
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ(
						flashFace,
						D_RRH_QSPI_FLASH_OFFSET_OS,
						D_UBOOT_FLASH_SIZE3,
						flashReadBuffer );
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)	{
			fprintf(stderr, "Error: QSPI Flashread error. face:%d OS-Ver.(code:%d)\n", flashFace, rtn_bpf ) ;
			fclose(versionFileFp);
			return -1;
		}

		/* TotalVer */
		memcpy( &vertmp, 	&flashReadBuffer[D_FLASH_OS_TOTALVER_OFFSET], 4 ) ;
		BPF_COM_END_LITTLETOBIG32(vertmp);
		fprintf(versionFileFp, "  APL  total   Ver : %08x\n", 	vertmp	);

		/* Kernel Ver */
		memcpy( &vertmp,  	&flashReadBuffer[D_FLASH_OS_KERNELVER_OFFSET], 4 );
		BPF_COM_END_LITTLETOBIG32(vertmp);
		fprintf(versionFileFp, "       Kernel  Ver : %08x\n",	vertmp	);

		/* Device-Tree Ver */
		memcpy( &vertmp, 	&flashReadBuffer[D_FLASH_OS_DEVTREEVER_OFFSET], 4 ) ;
		BPF_COM_END_LITTLETOBIG32(vertmp);
		fprintf(versionFileFp, "       DevTree Ver : %08x\n",	vertmp);

		/* Ramdisk Ver */
		memcpy( &vertmp, 	&flashReadBuffer[D_FLASH_OS_RAMDISIVER_OFFSET], 4 ) ;
		BPF_COM_END_LITTLETOBIG32(vertmp);
		fprintf(versionFileFp, "       Ramdisk Ver : %08x\n",	vertmp);

		fclose(versionFileFp);
	}

	return 0;
}

/********************************************************************/
/* 装置種別毎にハードウェア情報は異なるのでそれぞれ関数を用意する	*/
/********************************************************************/
/***********************************/
/********	TDD SRE		************/
/***********************************/
int hardware_infomation_TDD_SRE( FILE *outfp )
{
	int					rtn_bpf = 0;
	unsigned int		uiVal = 0;
	unsigned int		board_ver_dat;
#if 0	/***** TODO	*****/
	unsigned int		fpga_ver_dat;
#endif	/***** TODO	*****/
	
	/********************************************************************************/
	/* ボードバージョン & 装置種別表示												*/
	/* MIO(0xE000A040)																*/
	/*	Bit(MSB:31 <-> LSB:0)														*/
	/*	19:16	ボードVersion														*/
	/*	23:20	装置種別															*/
	if(( rtn_bpf = BPF_HM_DEVC_REG_READ(  DBG_CMD_LOGLEVEL, 0xE000A040, &uiVal )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf(stderr, "Error: GPIO board version get error.(ret:%d) \n", rtn_bpf );
		return D_CMD_NG;
	}

	/* ボードバージョン & 装置種別情報を抽出 */
	board_ver_dat = (uiVal >> 16) & 0x000000FF;

	fprintf(outfp, "BOARD  ver       : %04x\n", board_ver_dat );
	
#if 0	/***** TODO	*****/
	/************************************************/
	/* FPGA バージョン(MASTER)						*/
	/*  0x4000_0000									*/
	/*	Bit		機能名	R/W	説明					*/
	/*	31:16	VER	R	FPGAバージョン情報			*/
	/*	15:0	REV	R	FPGAリビジョン情報			*/
	if(( rtn_bpf  = BPF_HM_DEVC_REG_READ(  DBG_CMD_LOGLEVEL, D_RRH_REG_CNT_FPGAVER, &fpga_ver_dat )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "Error: FPGA(Master) version get error.(ret:%d)\n", rtn_bpf );
		return D_CMD_NG;
	}
	fprintf(outfp, "FPGA M ver       : %04x-%04x\n", (fpga_ver_dat&0xffff0000)>>16, (fpga_ver_dat&0xffff) );
	
	/************************************************/
	/* FPGA バージョン(SLAVE)						*/
	/*  0x8000_0000									*/
	/*	Bit		機能名	R/W	説明					*/
	/*	31:16	VER	R	FPGAバージョン情報			*/
	/*	15:0	REV	R	FPGAリビジョン情報			*/
	if(( rtn_bpf  = BPF_HM_DEVC_REG_READ(  DBG_CMD_LOGLEVEL, D_RRH_REG_CNTS_FPGAVER, &fpga_ver_dat )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "Error: FPGA(Slave) version get error.(ret:%d)\n", rtn_bpf );
		return D_CMD_NG;
	}
	fprintf(outfp, "FPGA S ver       : %04x-%04x\n", (fpga_ver_dat&0xffff0000)>>16, (fpga_ver_dat&0xffff) );
	
	/************************************************/
	/* MMI CPLD バージョン							*/
	/* MMI CPLD:0x00								*/
	/*	Bit		機能名	R/W	説明					*/
	/*	15:8	VER	R	CPLDバージョン情報			*/
	/*	 7:0	REV	R	CPLDリビジョン情報			*/
	if(( rtn_bpf  = BPF_HM_DEVC_MMI_CPLD_READ(  D_RRH_REG_CPLD_MMI_VER, &fpga_ver_dat )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "Error: MMI-CPLD version get error.(ret:%d)\n", rtn_bpf );
		return D_CMD_NG;
	}
	fprintf(outfp, "MMI CPLD ver     : %02x-%02x\n", (fpga_ver_dat&0x0000ff00)>>8, (fpga_ver_dat&0x00ff) );
#endif	/***** TODO	*****/
	return D_CMD_OK;
}

/*!
 * @brief 関数機能概要:バージョン表示　メイン処理
 * @note  関数処理内容.
 *       -# 次のバージョン情報を取得し表示する

 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */
int cmd_version_main(int argc, char *argv[], FILE *outfp )	{
	FILE*				fpVersion;
	char				cTemp[128];
	int					rtn_bpf = 0;
	unsigned char		start_side;

	/* EEPROMより起動面取得	*/
	if(( rtn_bpf = BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_START_FLG_ENA, &start_side )) != BPF_HM_DEVC_COMPLETE ) {
		fprintf( stderr, "Error: EEPROM Read Error!!  StartSide:  code:%d\n", rtn_bpf );
		return D_CMD_NG;
	}
	fprintf(outfp, "Start up side    : %d\n", start_side );

	/* Boot Version, APL Version はファイルより取得	*/
	/****************************************************/
	/* QSPIに頻繁にアクセスするのを避けるため			*/
	/* APLのVersionは初期化時にファイルに退避している	*/
	/*--------------------------------------------------*/
	/* 次の内容が取得できる                             */
	/* Boot  ver      : %s                              */
	/* APL total ver  : %02x%02x%02x%02x                */
	/*  - Kernel ver : %02x%02x%02x%02x                 */
	/*  - DevTree ver: %02x%02x%02x%02x                 */
	/*  - Ramdisk ver: %02x%02x%02x%02x                 */
	/****************************************************/

	if( access("/var/log/version" , R_OK ) != 0 ) {	/* バージョンファイルが存在していなければ	*/
		if( getAprVersion( start_side ) != 0 ) {
			return D_CMD_NG;
		}
	}

	/* File Open	*/
	fpVersion = fopen( "/var/log/version" ,"rb");
	if( fpVersion == 0 ){
		fprintf(stderr, "Error: version file open error.\n");
		return D_CMD_NG;
	}
	fseek(fpVersion,0,SEEK_SET);

	while( NULL != fgets( cTemp, 128, fpVersion ) ) {
		fprintf(outfp, "%s", cTemp);
	}
	fclose( fpVersion );
	/* 装置種別毎に情報を表示する	*/
//TODO		rtn_bpf = hardware_infomation_TDD_SRE( outfp );
	return rtn_bpf;
}
/* @} */

