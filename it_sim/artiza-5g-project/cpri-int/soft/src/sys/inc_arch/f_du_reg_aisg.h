/*!
 * @skip  $ld:$
 * @file  f_du_reg_aisg.h
 * @brief レジスタアドレス定義ヘッダ(AISG)
 * @date  2015/9/10 FFCS)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_AISG_H
#define F_DU_REG_AISG_H

/*!
 * @name AISGレジスタアドレス
 * @note AISGレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define	D_DU_REG_AISG_RXAISG					0xA0040000
#define	D_DU_REG_AISG_RXASPK					0xA1000004
#define	D_DU_REG_AISG_RXASPKC					0xA0040008
#define	D_DU_REG_AISG_RXASERR					0xA004000C
#define	D_DU_REG_AISG_RXASERRM					0xA0040010
#define	D_DU_REG_AISG_RXASFNM					0xA0040014
#define	D_DU_REG_AISG_RXASFDL					0xA0040018
#define	D_DU_REG_AISG_TXAISG					0xA0040100
#define	D_DU_REG_AISG_TXSIBNK					0xA0040104
#define	D_DU_REG_AISG_TXSILEN					0xA0040108
#define	D_DU_REG_AISG_TXSIPKRQ					0xA100010C
#define	D_DU_REG_AISG_TXSIPKRE					0xA1000110
#define	D_DU_REG_AISG_AISGRATE					0xA0040200
#define	D_DU_REG_AISG_AISGFCS					0xA0040204
#define	D_DU_REG_AISG_AISGTIM					0xA0040208
#define	D_DU_REG_AISG_SNDBUF					0xA0048000
#define	D_DU_REG_AISG_RCVBUF					0xA004A000
/* @} */

/*!
 * @name AISGレジスタbit
 * @note AISGレジスタbit
 * @{
 */
/* RXASPKC */
#define D_DU_AISG_RXASPKC_NEMP				0x80000000
#define D_DU_AISG_RXASPKC_BNK				0x000F0000
#define D_DU_AISG_RXASPKC_LEN				0x0000007F

/* RXASERR */
#define D_DU_AISG_RXASERR_ABF				0x00000001
#define D_DU_AISG_RXASERR_LOF				0x00000002
#define D_DU_AISG_RXASERR_SHF				0x00000004
#define D_DU_AISG_RXASERR_FCS				0x00000008
#define D_DU_AISG_RXASERR_MSK				(D_DU_AISG_RXASERR_ABF|D_DU_AISG_RXASERR_LOF|D_DU_AISG_RXASERR_SHF|D_DU_AISG_RXASERR_FCS)

/* TXAISG */
#define D_DU_AISG_TXAISG_BUSY				0x00010000

/* AISGRATE */
#define D_DU_AISG_AISGRATE_9600			0x00000001

/* @} */
#endif
/* @} */
