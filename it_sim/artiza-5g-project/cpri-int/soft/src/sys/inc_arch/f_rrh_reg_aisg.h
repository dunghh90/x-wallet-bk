/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_aisg.h
 * @brief レジスタアドレス定義ヘッダ(AISG)
 * @date  2015/9/10 FFCS)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_AISG_H
#define F_RRH_REG_AISG_H

/*!
 * @name AISGレジスタアドレス
 * @note AISGレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)

#elif defined  (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_AISG_RXAISG					0x41000000
#define	D_RRH_REG_AISG_RXASPK					0x41000004
#define	D_RRH_REG_AISG_RXASPKC					0x41000008
#define	D_RRH_REG_AISG_RXASERR					0x4100000C
#define	D_RRH_REG_AISG_RXASERRM					0x41000010
#define	D_RRH_REG_AISG_RXASFNM					0x41000014
#define	D_RRH_REG_AISG_RXASFDL					0x41000018
#define	D_RRH_REG_AISG_TXAISG					0x41000100
#define	D_RRH_REG_AISG_TXSIBNK					0x41000104
#define	D_RRH_REG_AISG_TXSILEN					0x41000108
#define	D_RRH_REG_AISG_TXSIPKRQ					0x4100010C
#define	D_RRH_REG_AISG_TXSIPKRE					0x41000110
#define	D_RRH_REG_AISG_AISGRATE					0x41000200
#define	D_RRH_REG_AISG_AISGFCS					0x41000204
#define	D_RRH_REG_AISG_AISGTIM					0x41000208
#define	D_RRH_REG_AISG_SNDBUF					0x41010000
#define	D_RRH_REG_AISG_RCVBUF					0x41010800
#else
#endif
/* @} */

/*!
 * @name AISGレジスタbit
 * @note AISGレジスタbit
 * @{
 */
/* RXASPKC */
#define D_RRH_AISG_RXASPKC_NEMP				0x80000000
#define D_RRH_AISG_RXASPKC_BNK				0x000F0000
#define D_RRH_AISG_RXASPKC_LEN				0x0000007F

/* RXASERR */
#define D_RRH_AISG_RXASERR_ABF				0x00000001
#define D_RRH_AISG_RXASERR_LOF				0x00000002
#define D_RRH_AISG_RXASERR_SHF				0x00000004
#define D_RRH_AISG_RXASERR_FCS				0x00000008
#define D_RRH_AISG_RXASERR_MSK				(D_RRH_AISG_RXASERR_ABF|D_RRH_AISG_RXASERR_LOF|D_RRH_AISG_RXASERR_SHF|D_RRH_AISG_RXASERR_FCS)

/* TXAISG */
#define D_RRH_AISG_TXAISG_BUSY				0x00010000

/* AISGRATE */
#define D_RRH_AISG_AISGRATE_9600			0x00000001

/* @} */
#endif
/* @} */
