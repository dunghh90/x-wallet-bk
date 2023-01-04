/*!
 * @skip  $ld:$
 * @file  f_du_reg_rafft.h
 * @brief レジスタアドレス定義ヘッダ(RAFFT)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_RAFFT_H
#define F_DU_REG_RAFFT_H

/*!
 * @name RAFFTレジスタアドレス
 * @note RAFFTレジスタアドレス
 * @{
 */
#define D_DU_REG_RAFFT_DGLB0				0xA1001000		/* DGLB0		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFTDGAIN0			0xA1001100		/* RFTDGAIN0	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFTDGAIN1			0xA1001104		/* RFTDGAIN1	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFTDGAINENB			0xA1001108		/* RFTDGAINENB	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFDGAINSUB6			0xA1001110		/* RFFDGAINSUB6	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFDGAINMMW			0xA1001114		/* RFFDGAINMMW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFDGAINENB			0xA1001118		/* RFFDGAINENB	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALENB				0xA1001180		/* RFCALENB		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFBSTENB				0xA1001200		/* RFBSTENB		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFTDGOVFLW			0xA1001300		/* RFTDGOVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFBSTOVFLW			0xA1001304		/* RFBSTOVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALOVFLW			0xA1001308		/* RFCALOVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFFTOVFLW			0xA100130C		/* RFFFTOVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFDGOVFLW			0xA1001310		/* RFFDGOVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_UDBFCVFLW			0xA1001314		/* UDBFCVFLW	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFFTMUX				0xA1004330		/* RFFTMUX		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFPLSSLP				0xA1004340		/* RFPLSSLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFMONSEL				0xA1001380		/* RFMONSEL		(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALCOEFREA			0xA1002000		/* RFCALCOEFREA	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALCOEFREB			0xA1002040		/* RFCALCOEFREB	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALCOEFREC			0xA1002080		/* RFCALCOEFREC	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFCALCOEFRED			0xA10020C0		/* RFCALCOEFRED	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFBSTCOEFA			0xA10020C4		/* RFBSTCOEFA	(sub6 : o / mmw : -)*/
#define D_DU_REG_RAFFT_RFBSTCOEFB			0xA10020C8		/* RFBSTCOEFB	(sub6 : o / mmw : -)*/
/* @} */
#endif
/* @} */
