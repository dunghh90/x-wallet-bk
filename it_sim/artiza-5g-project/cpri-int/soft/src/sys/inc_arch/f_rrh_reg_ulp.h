/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_ulp.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(ULP)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_ULP_H
#define F_RRH_REG_ULP_H

/*!
 * @name ULP���W�X�^�A�h���X
 * @note ULP���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_ULP_UPBITW					(D_RRH_REG_ULP + 0x00000000)	/* U-plane Bit Width						*/
#define	D_RRH_REG_ULP_A_C0PRM0					(D_RRH_REG_ULP + 0x00000100)	/* Ant/Car Parameter (Ant% Car0 Parameter0)	*/
#define	D_RRH_REG_ULP_A_C0PRM1					(D_RRH_REG_ULP + 0x00000104)	/* Ant/Car Parameter (Ant% Car0 Parameter1)	*/
#define	D_RRH_REG_ULP_A_C1PRM0					(D_RRH_REG_ULP + 0x00000110)	/* Ant/Car Parameter (Ant% Car1 Parameter0)	*/
#define	D_RRH_REG_ULP_A_C1PRM1					(D_RRH_REG_ULP + 0x00000114)	/* Ant/Car Parameter (Ant% Car1 Parameter1)	*/
#define	D_RRH_REG_ULP_SBBUFOVF					(D_RRH_REG_ULP + 0x00000700)	/* Monitor�i��Symbol Buffer Overflow�j	*/
#define	D_RRH_REG_ULP_ULPMONSEL					(D_RRH_REG_ULP + 0x000007FC)	/* Monitor�iULP Monitor Select�j			*/

/* Performance Monitor Control	*/
#define	D_RRH_REG_ULP_PMONLATALL				(D_RRH_REG_ULP + 0x00000300)	/* ���v�J�E���^���b�`&�N���A (�ꊇ)			*/
#define	D_RRH_REG_ULP_PMONLATA					(D_RRH_REG_ULP + 0x00000304)	/* ���v�J�E���^���b�`&�N���A (��)			*/

/* Performance Monitor UP(��)	*/
#define	D_RRH_REG_ULP_SNDUA_C0					(D_RRH_REG_ULP + 0x00000400)	/* U-plane Send(Car0)						*/
#define	D_RRH_REG_ULP_SNDUA_C1					(D_RRH_REG_ULP + 0x00000420)	/* U-plane Send(Car1)						*/
/* @} */

/*!
 * @name ULP���W�X�^bit & Shift
 * @note ULP���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
