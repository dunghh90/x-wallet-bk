/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC.h
 *  @brief  Definitions for BPF_HM_DEVC API
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#ifndef _BPF_HM_DEVC_H_
#define _BPF_HM_DEVC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"

/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* define                                                                                                           */
/********************************************************************************************************************/
/* Return code:終了コード */
#define BPF_HM_DEVC_COMPLETE				0		/* 正常終了									*/
#define BPF_HM_DEVC_NG						1		/* 異常終了									*/
#define BPF_HM_DEVC_OPENNG					2		/* デバイスオープン異常						*/
#define BPF_HM_DEVC_EPARAM					3		/* 範囲チェック異常							*/
#define BPF_HM_DEVC_QSPI_ULSELNG			4		/* QSPI Upper/Lower切替エラー				*/
#define BPF_HM_DEVC_UNSUPPORT				5		/* 未サポート								*/
#define BPF_HM_DEVC_TIMEOUT					6		/* タイムアウト異常終了						*/
#define BPF_HM_DEVC_MUTEX_NG				7		/* 排他制御異常終了							*/
#define BPF_HM_DEVC_FAILED					8		/* 書込失敗									*/
#define BPF_HM_DEVC_PARAMERR				9		/* パラメーターエラー						*/
#define BPF_HM_DEVC_RWCHKERR				10		/* 書き込み判定異常終了						*/
#define BPF_HM_DEVC_STOP					11		/* Process is stopped						*/

/* dts内のuio_irq_xxxの順番と合わせること */
#define BPF_IRQ_WDT							1		/* IRQ:WDT									*/
#define BPF_IRQ_1SEC						2		/* IRQ:1SEC割り込み							*/
#define BPF_IRQ_1PPS						3		/* IRQ:1PPS割り込み							*/
#define BPF_IRQ_SV							4		/* IRQ:SV									*/
#define BPF_IRQ_FHETHMAC0					5		/* IRQ:FH EHT 0	MAC							*/
#define BPF_IRQ_FHETHMAC1					6		/* IRQ:FH EHT 1	MAC							*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define BPF_IRQ_CPRI						7		/* IRQ:CPRI状変								*/
#define BPF_IRQ_L1RST						8		/* IRQ:L1RST								*/
#define BPF_IRQ_GPIO						9		/* IRQ:GPIO割り込み							*/
#define BPF_IRQ_CPRI_MASTER					10		/* IRQ:CPRI_MASTER状変割り込み				*/
#define BPF_IRQ_SFP							11		/* IRQ:SFP MOUNT状変割り込み				*/
#define BPF_IRQ_AISG						12		/* IRQ:AISG割り込み							*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* QSPI-Flashアクセス用Define	*/
#define BPF_HM_DEVC_QSPI_FLASH_FACE_0		0		/* QSPI FASH0面								*/
#define BPF_HM_DEVC_QSPI_FLASH_FACE_1		1		/* QSPI FASH1面								*/

/* SFP制御用Define	*/
#define	BPF_HM_DEVC_SFP_A0					0		/* A0アクセス								*/
#define	BPF_HM_DEVC_SFP_A2					1		/* A2アクセス								*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define	BPF_HM_DEVC_SFP_SA0                	2       /* Slave A0アクセス                 */
#define	BPF_HM_DEVC_SFP_SA2                 3       /* Slave A2アクセス                 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/***/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* QSPI-RE-FILE用Flashアクセス用Define	*/
enum{
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_0=0,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_1,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_2,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_3,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_4,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_5,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_6,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_7,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_8,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_9,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_10,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_11,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_12,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_13,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_14,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_15,
	BPF_HM_DEVC_RE_FILE_FLASH_FACE_MAX
};

/* SFP制御用Define	*/
enum{
	BPF_HM_DEVC_SFP0=0, /* Slave */
	BPF_HM_DEVC_SFP1,
	BPF_HM_DEVC_SFP2,
	BPF_HM_DEVC_SFP3,
	BPF_HM_DEVC_SFP4,
	BPF_HM_DEVC_SFP5,
	BPF_HM_DEVC_SFP6,
	BPF_HM_DEVC_SFP7,
	BPF_HM_DEVC_SFP8,
	BPF_HM_DEVC_SFP9,
	BPF_HM_DEVC_SFP10,
	BPF_HM_DEVC_SFP11,
	BPF_HM_DEVC_SFP12,
	BPF_HM_DEVC_SFP13,
	BPF_HM_DEVC_SFP14,
	BPF_HM_DEVC_SFP15,
	BPF_HM_DEVC_SFP16
};
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* SPI関連Branch定義	*/
/* SPI RF-IC定義	*/
enum
{
	BPF_HM_DEVC_RFIC_TXRX_KIND_TX = 0,					/* TX			*/
	BPF_HM_DEVC_RFIC_TXRX_KIND_RX,						/* RX			*/
	BPF_HM_DEVC_RFIC_TXRX_KIND_MAX
};

enum
{
	BPF_HM_DEVC_RFIC_UNIT_KIND0 = 0,					/* UNIT#0			*/
	BPF_HM_DEVC_RFIC_UNIT_KIND1,						/* UNIT#1			*/
	BPF_HM_DEVC_RFIC_UNIT_KIND_MAX
};

enum
{
	BPF_HM_DEVC_RFIC_UNIT_RFIC1 = 0,					/* RFIC(AB)系		*/
	BPF_HM_DEVC_RFIC_UNIT_RFIC2,						/* RFIC(CD)系		*/
	BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX
};
#define BPF_HM_DEVC_RFIC_UNIT_RFICALL	0xFF			/* RFIC ALL制御		*/
/***/

/********************************************************************************************************************/
/* enum                                                                                                             */
/********************************************************************************************************************/

/********************************************************************************************************************/
/* typedef                                                                                                          */
/********************************************************************************************************************/
typedef struct{
    unsigned int status;
    unsigned int trap;
    unsigned int mask;
    unsigned int force;
    unsigned int db;
    unsigned int trg;
}bpf_hm_devc_irq_t;


/********************************************************************************************************************/
/* Function Prototype                                                                                               */
/********************************************************************************************************************/
int BPF_HM_DEVC_RESERVE(unsigned int dev);

int BPF_HM_DEVC_QSPI_FLASH_READ(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_WRITE(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_ERASE(unsigned int face,unsigned int offset, unsigned int size);
int BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER(unsigned int face,unsigned int offset,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND( unsigned int *flashKind );

int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p);
int BPF_HM_DEVC_EEPROM_WRITE(unsigned short offset, unsigned char *data_p);

int BPF_HM_DEVC_SFP_READ( unsigned int kind, unsigned short offset, unsigned char *data_p);
int BPF_HM_DEVC_SFP_READ_BUFFER(unsigned int kind, unsigned short offset,  unsigned int count, unsigned char *data_p );
int BPF_HM_DEVC_SFP_WRITE( unsigned int kind, unsigned short offset, unsigned char *data_p);

int BPF_HM_DEVC_MMI_CPLD_READ(unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_MMI_CPLD_WRITE(unsigned int offset, unsigned int *data_p);

int BPF_HM_DEVC_TEMP_READ(signed int *temp_p);
int BPF_HM_DEVC_TEMP2_READ(signed int *temp_p);
void BPF_HM_DEVC_TEMP_INIT( void );

int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_REG_READ_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p);
int BPF_HM_DEVC_REG_READ_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p);
int BPF_HM_DEVC_REG_READ_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p);
int BPF_HM_DEVC_REG_READ_BIT(unsigned int loglevel, unsigned int offset, unsigned int check_bit, unsigned int *data_p);
int BPF_HM_DEVC_REG_READ_NOMUTEX(unsigned int loglevel, unsigned int offset, unsigned int *data_p);

int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE_BITON(unsigned int loglevel, unsigned int offset, unsigned int bitifno );
int BPF_HM_DEVC_REG_WRITE_BITOFF(unsigned int loglevel, unsigned int offset, unsigned int bitifno );

int BPF_HM_DEVC_GPIO_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_GPIO_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p);

int BPF_HM_DEVC_SETTIME(T_RRH_SYSTEM_TIME sysTime );
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
int BPF_HM_DEVC_SETTIME_BCD(T_RRH_SYSTEM_TIME sysTime );
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
int BPF_HM_DEVC_GETTIME(T_RRH_SYSTEM_TIME* sysTime );
int BPF_HM_DEVC_GETTIME_BCD(T_RRH_SYSTEM_TIME* sysTime );
int BPF_HM_DEVC_GETTIME_OS(T_RRH_SYSTEM_TIME* sysTime );
void BPF_HM_DEVC_SETTIME_OS_SUPPRESS(void);
void BPF_HM_DEVC_SETTIME_OS_CORRECTION(void);

int BPF_HM_DEVC_RFIC_READ( UINT txRx, UINT unit, UINT address, UINT *data_p);
int BPF_HM_DEVC_RFIC_WRITE( UINT txRx, UINT unit, UINT address, UINT data );
int BPF_HM_DEVC_RFIC_RESET( UINT txRx, UINT unit );
int BPF_HM_DEVC_EXT_PLL_READ (UINT address, UINT *data_p);
int BPF_HM_DEVC_EXT_PLL_WRITE (UINT address, UINT data);
int BPF_HM_DEVC_PTP_READ (UINT address, UINT *data_p);
int BPF_HM_DEVC_PTP_WRITE (UINT address, UINT data);

int BPF_HM_DEVC_ASSIGN(unsigned int assign_onoff);

int BPF_HM_DEVC_FORCE_ACCESS( int force_access );
int BPF_HM_DEVC_FPGA_ACCESS( int checkFlg );
int BPF_HM_DEVC_PL_CONFIG_STATE_SET( int state );
void BPF_HM_DEVC_MUTEX_LOCK_FREE();
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
int BPF_HM_DEVC_WDT_SET_LOG(unsigned int uiSec_AXI, unsigned int uiSec_SWDT, const VOID* fileName, UINT lineNo);
// #define  BPF_HM_DEVC_WDT_SET( a, b ) BPF_HM_DEVC_WDT_SET_LOG( a, b, __FILE__, __LINE__)
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

int BPF_HM_DEVC_PL_FLASH_ID_SET( unsigned char id ); /* FLASH EOL対応 */

int BPF_HM_DEVC_WDT_SET(unsigned int uiSec_AXI, unsigned int uiSec_SWDT);
int BPF_HM_DEVC_WDT_CLEAR( );

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(unsigned int flash_address, unsigned int len,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(unsigned int flash_address, unsigned int len,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(unsigned int add, unsigned int len);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ(unsigned int face, unsigned int flash_address, unsigned int len, unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE(unsigned int face, unsigned int flash_address, unsigned int len, unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE(unsigned int face, unsigned int flash_address, unsigned int len);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*	FLASH EOL対応 add start	*/
// int BPF_HM_DEVC_PL_FLASH_ID_SET( unsigned int flashKind, unsigned short ss, unsigned char id );
/*	FLASH EOL対応 add end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT(void);

void BPF_HM_DEVC_SPI_INIT();

int BPF_HM_DEVC_GETTIME_STR(char *timestring);

int BPF_HM_DEVC_RFPLL_READ(unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_RFPLL_WRITE(unsigned int address, unsigned int data);

int BPF_HM_DEVC_TXDAC_READ(unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_TXDAC_WRITE(unsigned int address, unsigned int data);

int BPF_HM_DEVC_RXADC_READ(unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_RXADC_WRITE(unsigned int address, unsigned int data);

int BPF_HM_DEVC_RFEEP_READ(unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_RFEEP_WRITE(unsigned int address, unsigned int data);

int BPF_HM_DEVC_PGC_READ(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int *data_p);
int BPF_HM_DEVC_PGC_WRITE(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int data);

int BPF_HM_DEVC_PGCLUT_READ(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int *data_p);
int BPF_HM_DEVC_PGCLUT_WRITE(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int data);

int BPF_HM_DEVC_QMOD_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_QMOD_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data);

int BPF_HM_DEVC_I2CSOFT_RESET();

int BPF_HM_DEVC_QDEM_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p);
int BPF_HM_DEVC_QDEM_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data);

int BPF_HM_DEVC_PAVGDAC_READ(unsigned char ant, unsigned int *data_p);
int BPF_HM_DEVC_PAVGDAC_WRITE(unsigned char ant, unsigned int data);

int BPF_HM_DEVC_TXVATTDAC_READ(unsigned char ant, unsigned int *data_p);
int BPF_HM_DEVC_TXVATTDAC_WRITE(unsigned char ant, unsigned int data);

int BPF_HM_DEVC_RXVATTDAC_READ(unsigned char ant, unsigned int *data_p);
int BPF_HM_DEVC_RXVATTDAC_WRITE(unsigned char ant, unsigned int data);

int BPF_HM_DEVC_FANDAC_WRITE(unsigned int data);

int BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int gain1 ,
	 unsigned int phase1,
	 unsigned int gain2 ,
	 unsigned int phase2,
	 unsigned int gain3 ,
	 unsigned int phase3,
	 unsigned int gain4 ,
	 unsigned int phase4
);

int BPF_HM_DEVC_PGC_GAIN_PHASE_READ(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
);

int BPF_HM_DEVC_PGC_WRITE2(
	 unsigned char chain_grp,
	 unsigned int data1,
	 unsigned int data2,
	 unsigned int data3,
	 unsigned int data4
);

int BPF_HM_DEVC_PGC_READ2(
	 unsigned char chain_grp,
	 unsigned char bank,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
);

int BPF_HM_DEVC_PGCLUT_WRITE2(unsigned int data_len, unsigned char* data_p);
int BPF_HM_DEVC_GET_ENV( void );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_HM_DEVC_H_ */

