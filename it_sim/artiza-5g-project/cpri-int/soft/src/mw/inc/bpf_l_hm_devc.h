/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_hm_devc.h
 *  @brief  Definitions for BPF log library
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_HM_DEVC_H_
#define _BPF_L_HM_DEVC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_HM_DEVC.h"
#define BPF_HM_DEVC_INTER_OK              0      /* OK */
#define BPF_HM_DEVC_INTER_NG              1      /* NG */
#define BPF_HM_DEVC_INTER_ERR_SYS         2      /* システムコール失敗 */
#define BPF_HM_DEVC_INTER_ERR_ASSIGN      3      /* ASSIGN NG */

/* アクセスタイプ	*/
#define BPF_HM_DEVC_ACCESS_1BYTE		0					/* 1byteアクセス						*/
#define BPF_HM_DEVC_ACCESS_2BYTE		1					/* 2byteアクセス						*/
#define BPF_HM_DEVC_ACCESS_4BYTE		2					/* 4byteアクセス						*/

/* SPI情報(LOG用)	*/
#define BPF_HM_DEVC_SPI0_SS0			0x00000000	/* SPI#0 ss#0									*/
#define BPF_HM_DEVC_SPI0_SS1			0x00010000	/* SPI#0 ss#1									*/
#define BPF_HM_DEVC_SPI0_SS2			0x00020000	/* SPI#0 ss#2									*/
#define BPF_HM_DEVC_SPI1_SS0			0x01000000	/* SPI#1 ss#0									*/
#define BPF_HM_DEVC_SPI1_SS1			0x01010000	/* SPI#1 ss#1									*/
#define BPF_HM_DEVC_SPI2_SS0			0x02000000	/* SPI#2 ss#0									*/
#define BPF_HM_DEVC_SPI2_SS1			0x02010000	/* SPI#2 ss#1									*/
#define BPF_HM_DEVC_SPI2_SS2			0x02020000	/* SPI#2 ss#2									*/
#define BPF_HM_DEVC_SPI3_SS0			0x03000000	/* SPI#3 ss#0 QMOD(TX_MIX)	V偏波				*/
#define BPF_HM_DEVC_SPI3_SS1			0x03010000	/* SPI#3 ss#1 QMOD(TX_VGA)	V偏波				*/
#define BPF_HM_DEVC_SPI3_SS2			0x03020000	/* SPI#3 ss#2 QDEM			V偏波				*/
#define BPF_HM_DEVC_SPI3_SS3			0x03030000	/* SPI#3 ss#3 QMOD(TX_MIX)	H偏波				*/
#define BPF_HM_DEVC_SPI3_SS4			0x03040000	/* SPI#3 ss#4 QMOD(TX_VGA)	H偏波				*/
#define BPF_HM_DEVC_SPI3_SS5			0x03050000	/* SPI#3 ss#5 QDEM			H偏波				*/
#define BPF_HM_DEVC_SPI3_SS6			0x03060000	/* SPI#3 ss#6 QMOD/QDEM DAC	mmw					*/
#define BPF_HM_DEVC_SPI3_SS7			0x03070000	/* SPI#3 ss#7 PAVG DAC		mmw					*/
#define BPF_HM_DEVC_SPI3_SS8			0x03080000	/* SPI#3 ss#8 TxVATT DAC	sub6				*/
#define BPF_HM_DEVC_SPI3_SS9			0x03090000	/* SPI#3 ss#8 PAVG DAC		sub6				*/
#define BPF_HM_DEVC_SPI3_SS10			0x030A0000	/* SPI#3 ss#8 TxVATT_DAC	mmw	V偏波			*/
#define BPF_HM_DEVC_SPI3_SS11			0x030B0000	/* SPI#3 ss#8 RxVATT_DAC	mmw	V偏波			*/
#define BPF_HM_DEVC_SPI3_SS12			0x030C0000	/* SPI#3 ss#8 TxVATT_DAC	mmw	H偏波			*/
#define BPF_HM_DEVC_SPI3_SS13			0x030D0000	/* SPI#3 ss#8 RxVATT_DAC	mmw	H偏波			*/


/* SPI方路情報(BPF_COMのLOGでも使用する)	*/
#define BPF_HM_DEVC_CPLD_SET_PLL			0x8000
#define BPF_HM_DEVC_CPLD_SET_FWVGA4			0x0800
#define BPF_HM_DEVC_CPLD_SET_FWVGA3			0x0400
#define BPF_HM_DEVC_CPLD_SET_FWVGA2			0x0200
#define BPF_HM_DEVC_CPLD_SET_FWVGA1			0x0100
#define BPF_HM_DEVC_CPLD_SET_VGDAC4			0x0080
#define BPF_HM_DEVC_CPLD_SET_VGDAC3			0x0040
#define BPF_HM_DEVC_CPLD_SET_VGDAC2			0x0020
#define BPF_HM_DEVC_CPLD_SET_VGDAC1			0x0010
#define BPF_HM_DEVC_CPLD_SET_RFIC4			0x0008
#define BPF_HM_DEVC_CPLD_SET_RFIC3			0x0004
#define BPF_HM_DEVC_CPLD_SET_RFIC2			0x0002
#define BPF_HM_DEVC_CPLD_SET_RFIC1			0x0001


int bpf_hm_devc_assign(unsigned int);
int bpf_hm_devc_init(unsigned int);
void bpf_hm_devc_unassign( void );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_HM_DEVC_H_ */

