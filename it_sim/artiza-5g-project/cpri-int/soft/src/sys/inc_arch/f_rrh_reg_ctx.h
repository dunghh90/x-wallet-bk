/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_ctx.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(CTX)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CTX_H
#define F_RRH_REG_CTX_H

/*!
 * @name CTX���W�X�^�A�h���X
 * @note CTX���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_CTX_FPGAVER					(D_RRH_REG_CTX + 0x00000000)	/* VERSION(TRX-FPGA)						*/
#define	D_RRH_REG_CTX_REGCHK					(D_RRH_REG_CTX + 0x00000004)	/* CHECK									*/
#define	D_RRH_REG_CTX_BID						(D_RRH_REG_CTX + 0x00000008)	/* Board ID									*/
#define	D_RRH_REG_CTX_DID						(D_RRH_REG_CTX + 0x0000000C)	/* Device ID								*/
#define	D_RRH_REG_CTX_PWR						(D_RRH_REG_CTX + 0x00000020)	/* �O��Device�d������						*/
#define	D_RRH_REG_CTX_RST						(D_RRH_REG_CTX + 0x00000040)	/* �N��(Reset)����@��Reserve				*/
#define	D_RRH_REG_CTX_RSTPHY1					(D_RRH_REG_CTX + 0x00000044)	/* �N��(Reset)����(�e��SerDes)				*/
#define	D_RRH_REG_CTX_RSTPHY2					(D_RRH_REG_CTX + 0x00000048)	/* �N��(Reset)����(�e��SerDes)				*/
#define	D_RRH_REG_CTX_PHYPWR1					(D_RRH_REG_CTX + 0x00000050)	/* SerDes PowerOn����						*/
#define	D_RRH_REG_CTX_PHYPWR2					(D_RRH_REG_CTX + 0x00000054)	/* SerDes PowerOn����						*/
#define	D_RRH_REG_CTX_STA1						(D_RRH_REG_CTX + 0x00000200)	/* FPGA��ԕ\�����W�X�^1					*/
#define	D_RRH_REG_CTX_STA1M						(D_RRH_REG_CTX + 0x00000204)	/* FPGA��ԕ\�����W�X�^1�}�X�N				*/
#define	D_RRH_REG_CTX_FCSTA1					(D_RRH_REG_CTX + 0x00000208)	/* FPGA��ԕ\�����W�X�^1�[���ݒ�			*/
#define	D_RRH_REG_CTX_STA2						(D_RRH_REG_CTX + 0x00000210)	/* FPGA��ԕ\�����W�X�^2					*/
#define	D_RRH_REG_CTX_STA2M						(D_RRH_REG_CTX + 0x00000214)	/* FPGA��ԕ\�����W�X�^2�}�X�N				*/
#define	D_RRH_REG_CTX_FCSTA2					(D_RRH_REG_CTX + 0x00000218)	/* FPGA��ԕ\�����W�X�^2�[���ݒ�			*/
#define	D_RRH_REG_CTX_CDRST						(D_RRH_REG_CTX + 0x00000280)	/* CPRI PHY CDR�Ď�							*/
#define	D_RRH_REG_CTX_ULIQSDM					(D_RRH_REG_CTX + 0x00000300)	/* UL IQ�V���b�g�_�E���v���}�X�N			*/
#define	D_RRH_REG_CTX_DLIQSDM					(D_RRH_REG_CTX + 0x00000304)	/* DL IQ�V���b�g�_�E���v���}�X�N			*/
#define	D_RRH_REG_CTX_GPIFSET					(D_RRH_REG_CTX + 0x00000700)	/* �ėp�|�[�g����							*/
#define	D_RRH_REG_CTX_GPIFCK					(D_RRH_REG_CTX + 0x00000704)	/* �ėpI/F�ݒ�(�N���b�N)					*/
#define	D_RRH_REG_CTX_GPIFEN					(D_RRH_REG_CTX + 0x00000708)	/* �ėpI/F�ݒ�(�C�l�[�u��)					*/
#define	D_RRH_REG_CTX_GPIFOE					(D_RRH_REG_CTX + 0x0000070C)	/* �ėpI/F�ݒ�(�o�͐���)					*/
#define	D_RRH_REG_CTX_GPIFTXD					(D_RRH_REG_CTX + 0x00000710)	/* �ėpI/F�ݒ�(���M�f�[�^)					*/
#define	D_RRH_REG_CTX_GPIFRXD					(D_RRH_REG_CTX + 0x00000714)	/* �ėpI/F�\��(��M�f�[�^)					*/
#define	D_RRH_REG_CTX_JS1PHYCNT					(D_RRH_REG_CTX + 0x00000900)	/* JESD Ch1 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_JS2PHYCNT					(D_RRH_REG_CTX + 0x00000904)	/* JESD Ch2 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_JS3PHYCNT					(D_RRH_REG_CTX + 0x00000908)	/* JESD Ch3 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_JS4PHYCNT					(D_RRH_REG_CTX + 0x0000090C)	/* JESD Ch4 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_IC1PHYCNT					(D_RRH_REG_CTX + 0x00000910)	/* InterConnect Ch1 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_IC2PHYCNT					(D_RRH_REG_CTX + 0x00000914)	/* InterConnect Ch2 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_IC3PHYCNT					(D_RRH_REG_CTX + 0x00000918)	/* InterConnect Ch3 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_IC4PHYCNT					(D_RRH_REG_CTX + 0x0000091C)	/* InterConnect Ch4 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTX_SVPROT					(D_RRH_REG_CTX + 0x00000970)	/* SV�ω��Ď�����							*/
#define	D_RRH_REG_CTX_SVIFERR					(D_RRH_REG_CTX + 0x00000980)	/* LLB-FPGA�� SVI/F �G���[�\��				*/
#define	D_RRH_REG_CTX_SVIFTO					(D_RRH_REG_CTX + 0x00000984)	/* FPGA�� SVI/F Timeout�\��					*/
#define	D_RRH_REG_CTX_LLSVIFTXEN				(D_RRH_REG_CTX + 0x00000988)	/* PS-FPGA�� SVI/F TX Enable				*/
#define	D_RRH_REG_CTX_LLSVIFTXD4				(D_RRH_REG_CTX + 0x00000990)	/* PS-FPGA�� SVI/F TX Data4�ݒ�				*/
#define	D_RRH_REG_CTX_LLSVIFTXD3				(D_RRH_REG_CTX + 0x00000994)	/* PS-FPGA�� SVI/F TX Data3�ݒ�				*/
#define	D_RRH_REG_CTX_LLSVIFTXD2				(D_RRH_REG_CTX + 0x00000998)	/* PS-FPGA�� SVI/F TX Data2�ݒ�				*/
#define	D_RRH_REG_CTX_LLSVIFTXD1				(D_RRH_REG_CTX + 0x0000099C)	/* PS-FPGA�� SVI/F TX Data1�ݒ�				*/
#define	D_RRH_REG_CTX_LLSVIFRXD4				(D_RRH_REG_CTX + 0x000009A0)	/* PS-FPGA�� SVI/F RX Data4�\��				*/
#define	D_RRH_REG_CTX_LLSVIFRXD3				(D_RRH_REG_CTX + 0x000009A4)	/* PS-FPGA�� SVI/F RX Data3�\��				*/
#define	D_RRH_REG_CTX_LLSVIFRXD2				(D_RRH_REG_CTX + 0x000009A8)	/* PS-FPGA�� SVI/F RX Data2�\��				*/
#define	D_RRH_REG_CTX_LLSVIFRXD1				(D_RRH_REG_CTX + 0x000009AC)	/* PS-FPGA�� SVI/F RX Data1�\��				*/
#define	D_RRH_REG_CTX_ICNOTDONE					(D_RRH_REG_CTX + 0x00000B00)	/* InterConnect SerDes Not Done				*/
#define	D_RRH_REG_CTX_ICNOTDONEM				(D_RRH_REG_CTX + 0x00000B04)	/* InterConnect SerDes Not Done Mask		*/
#define	D_RRH_REG_CTX_ICTXERR					(D_RRH_REG_CTX + 0x00000B10)	/* InterConnect TX Error					*/
#define	D_RRH_REG_CTX_ICTXMSK					(D_RRH_REG_CTX + 0x00000B14)	/* InterConnect TX Error Mask				*/
#define	D_RRH_REG_CTX_ICRXERR					(D_RRH_REG_CTX + 0x00000B20)	/* InterConnect RX Error					*/
#define	D_RRH_REG_CTX_ICRXMSK					(D_RRH_REG_CTX + 0x00000B24)	/* InterConnect RX Error Mask				*/
#define	D_RRH_REG_CTX_RDYCHK					(D_RRH_REG_CTX + 0x00000F00)	/* [DBG] HPM1�nBus Error�\��(Ready check)	*/
#define	D_RRH_REG_CTX_JSDBG						(D_RRH_REG_CTX + 0x00000F10)	/* [DBG] JESD Debug							*/
#define	D_RRH_REG_CTX_JSDBGMON					(D_RRH_REG_CTX + 0x00000F14)	/* [DBG] JESD Debug Monitor					*/
#define	D_RRH_REG_CTX_ICDBG						(D_RRH_REG_CTX + 0x00000F18)	/* [DBG] InterConnect Debug					*/
#define	D_RRH_REG_CTX_ICTEST					(D_RRH_REG_CTX + 0x00000F1C)	/* [DBG] InterConnect Test					*/
#define	D_RRH_REG_CTX_ICRSTSTA					(D_RRH_REG_CTX + 0x00000F30)	/* [DBG] InterConnect Reset State			*/
#define	D_RRH_REG_CTX_ICPRBSLOCK				(D_RRH_REG_CTX + 0x00000F34)	/* [DBG] ICX Status (InterConnect PRBS Lock)		*/
#define	D_RRH_REG_CTX_ICDBGMON					(D_RRH_REG_CTX + 0x00000F38)	/* [DBG] ICX Status (InterConnect Debug Monitor)	*/
#define	D_RRH_REG_CTX_LLSVIFTST					(D_RRH_REG_CTX + 0x00000F60)	/* [DBG] SVIF(LLB-FPGA��)�e�X�g				*/
#define	D_RRH_REG_CTX_DIPSW						(D_RRH_REG_CTX + 0x00000F70)	/* [DBG] DIP-SW��ԕ\��						*/
#define	D_RRH_REG_CTX_SYS10MEN					(D_RRH_REG_CTX + 0x00000F90)	/* [DBG] 10MHz Clock�o�� (10MHz Clock�o�͐���)					*/
#define	D_RRH_REG_CTX_SYS10MULCK				(D_RRH_REG_CTX + 0x00000F94)	/* [DBG] 10MHz Clock�o�� (10MHz Clock PLL UnLock�X�e�[�^�X�\��)	*/
#define	D_RRH_REG_CTX_CAP						(D_RRH_REG_CTX + 0x00000FD0)	/* [DBG] �L���v�`�����W�X�^					*/
#define	D_RRH_REG_CTX_CAPEXT					(D_RRH_REG_CTX + 0x00000FD4)	/* [DBG] �L���v�`�����W�X�^�g��				*/
#define	D_RRH_REG_CTX_CAPINT					(D_RRH_REG_CTX + 0x00000FD8)	/* [DBG] �L���v�`���Ԍ��ݒ�					*/
#define	D_RRH_REG_CTX_DBGVER					(D_RRH_REG_CTX + 0x00000FE0)	/* [DBG] �f�o�b�O�o�[�W����					*/
#define	D_RRH_REG_CTX_CTTEST					(D_RRH_REG_CTX + 0x00000FF0)	/* [DBG] �e�X�g								*/
/* @} */

/*!
 * @name CTX���W�X�^bit & Shift
 * @note CTX���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
