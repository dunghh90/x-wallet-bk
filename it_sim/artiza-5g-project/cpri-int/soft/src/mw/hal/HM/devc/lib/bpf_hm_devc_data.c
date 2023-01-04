/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_hm_devc_data.c
 *	@brief	HM DEVC Data.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  HM DEVC Data.
 *  @note   BPF internal function. 
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 *  @date   2015/05/21 ALPHA)Murakami    M-RRU-ZSYS-01584対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/

/********************************************
* global data definition.                   *
********************************************/
unsigned int  di_devc_assign_flag = DEVC_FLG_OFF;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 add start	*/
unsigned int reQspiFlashIndx;
/* Flash EOL対応 add end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

t_bpf_hm_devc_data* di_devc_data_p;
t_bpf_hm_devc_fd_data di_devc_data_fd;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* RE FILE用FLASH faceをレジスタ書き込みデータに変換用のテーブル */
const unsigned int flash_re_file_face2data[BPF_HM_DEVC_RE_FILE_FLASH_FACE_MAX][3] = 
 	{
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS0, BPF_HM_DEVC_PLQSPI_FACE_LOWER,  0},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS0, BPF_HM_DEVC_PLQSPI_FACE_SECOND, 0},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS0, BPF_HM_DEVC_PLQSPI_FACE_THIRD,  0},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS0, BPF_HM_DEVC_PLQSPI_FACE_UPPER,  0},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS1, BPF_HM_DEVC_PLQSPI_FACE_LOWER,  1},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS1, BPF_HM_DEVC_PLQSPI_FACE_SECOND, 1},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS1, BPF_HM_DEVC_PLQSPI_FACE_THIRD,  1},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS1, BPF_HM_DEVC_PLQSPI_FACE_UPPER,  1},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS2, BPF_HM_DEVC_PLQSPI_FACE_LOWER,  2},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS2, BPF_HM_DEVC_PLQSPI_FACE_SECOND, 2},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS2, BPF_HM_DEVC_PLQSPI_FACE_THIRD,  2},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS2, BPF_HM_DEVC_PLQSPI_FACE_UPPER,  2},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS3, BPF_HM_DEVC_PLQSPI_FACE_LOWER,  3},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS3, BPF_HM_DEVC_PLQSPI_FACE_SECOND, 3},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS3, BPF_HM_DEVC_PLQSPI_FACE_THIRD,  3},
 		{ BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS3, BPF_HM_DEVC_PLQSPI_FACE_UPPER,  3}
 	};
/* Flash EOL対応 chg end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* Device情報テーブル	*/
/************************************************************************/
/* 中身のデータはそれぞれの装置のenum + 共通部のenumに合わせること		*/
/************************************************************************/
const t_bpf_hm_devc_fd_data  di_devc_data_fd_data_zynqU =
{
	{	/*	ValidFlg	DeviceName			StartOffset	Size		AccessType					IRQ番号				FPGA	PLCONFIG	FD		MD		*/
/* 共通部	*/
		{	1,			BPF_HM_DEVC_MTD0,	0,			0,			0,							0,					0,		0,			0,		0		},		/* MTD0					*/
		{	1,			BPF_HM_DEVC_I2C0,	0,			0,			0,							0,					0,		0,			0,		0		},		/* I2C0					*/
		{	1,			BPF_HM_DEVC_I2C1,	0,			0,			0,							0,					0,		0,			0,		0		},		/* I2C1					*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* I2C2	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SPI0	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SPI1	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SPI2	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SPI3	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SPI4	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* WDT	mutex用			*/
		{	1,			NULL,				0,			0,			0,							0,					0,		0,			0,		0		},		/* SLAVE-flash mutex用	*/

		{	0,			BPF_HM_DEVC_UIO0,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO1,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO2,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO3,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO4,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO5,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO6,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO7,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO8,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO9,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO10,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO11,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO12,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO13,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO14,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO15,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO16,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO17,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO18,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO19,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO20,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO21,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO22,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO23,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO24,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO25,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO26,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO27,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO28,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO29,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO30,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/
		{	0,			BPF_HM_DEVC_UIO31,	0,			0,			BPF_HM_DEVC_ACCESS_4BYTE,	0,					0,		0,			0,		0		},		/* 				*/

		{	0,			0,					0,			0,			0,							0,					0,		0,			0,		0		}		/* END OF TABLE	*/
	}
};

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 add start	*/
/* PL QSPI FILE用のwriteコマンドに変換用のテーブル */
const unsigned char plQspiFlash_write_cmd[2] = {BPF_HM_DEVC_PLQSPI_QUAD_FAST_PROGRAM, BPF_HM_DEVC_PLQSPI_PAGE_PROGRAM};	/*	0x32,0x20	*/
const unsigned char plQspiFlash_stsReg[2] = {BPF_HM_DEVC_PLQSPI_READ_FLG_STS_REG, BPF_HM_DEVC_PLQSPI_READ_STS_REG};	/*	0x70,0x05	*/
const unsigned char plQspiFlash_stsReg_chkbit[2] = {BPF_HM_DEVC_PLQSPI_FLGSTSCHKBIT, BPF_HM_DEVC_PLQSPI_STSREGCHKBIT};	/*	bit7(0x80), bit0(0x01)	*/
const unsigned char plQspiFlash_stsReg_rsltbit[2] = {BPF_HM_DEVC_PLQSPI_FLGSTSCHKBIT, 0};	/*	bit7(ON), bit0(OFF)	*/
/* Flash EOL対応 add end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
