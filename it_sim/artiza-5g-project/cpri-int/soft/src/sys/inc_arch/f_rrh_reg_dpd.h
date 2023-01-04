/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dpd.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(DPD)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DPD_H
#define F_RRH_REG_DPD_H

/*!
 * @name DPD���W�X�^�A�h���X
 * @note DPD���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_DPD_TXCAREN					(D_RRH_REG_DPD + 0x00000000)	/* TxEnable									*/
#define	D_RRH_REG_DPD_TXCNF0					(D_RRH_REG_DPD + 0x00000010)	/* System (Cr0 config �M���ш�ݒ�)			*/
#define	D_RRH_REG_DPD_TXCNF1					(D_RRH_REG_DPD + 0x00000014)	/* System (Cr1 config �M���ш�ݒ�)			*/
#define	D_RRH_REG_DPD_TXPRM0					(D_RRH_REG_DPD + 0x00000030)	/* Parameter (Cr0 �x�������l(�e:1/3.84MHz))	*/
#define	D_RRH_REG_DPD_TXPRM1					(D_RRH_REG_DPD + 0x00000034)	/* Parameter (Cr1 �x�������l(�e:1/3.84MHz))	*/
#define	D_RRH_REG_DPD_BTIGAIN0					(D_RRH_REG_DPD + 0x00000040)	/* Parameter (Cr0 BTI-GAIN�����l)			*/
#define	D_RRH_REG_DPD_BTIGAIN1					(D_RRH_REG_DPD + 0x00000044)	/* Parameter (Cr1 BTI-GAIN�����l)			*/
#define	D_RRH_REG_DPD_LPFGAIN0					(D_RRH_REG_DPD + 0x00000050)	/* LPF Gain (Cr0 LPF-GAIN�����l)			*/
#define	D_RRH_REG_DPD_LPFGAIN1					(D_RRH_REG_DPD + 0x00000054)	/* LPF Gain (Cr1 LPF-GAIN�����l)			*/
#define	D_RRH_REG_DPD_LPF0						(D_RRH_REG_DPD + 0x00000050)	/* LPF Coefficient (Cr0 LPF�W��)			*/
#define	D_RRH_REG_DPD_LPF1						(D_RRH_REG_DPD + 0x00000054)	/* LPF Coefficient (Cr1 LPF�W��)			*/
#define	D_RRH_REG_DPD_TXANTEN					(D_RRH_REG_DPD + 0x00000080)	/* Antenna Enable							*/
#define	D_RRH_REG_DPD_HCFR						(D_RRH_REG_DPD + 0x00000100)	/* HighPass CFR								*/
#define	D_RRH_REG_DPD_ICFRB						(D_RRH_REG_DPD + 0x00000140)	/* Impulse CFR (Before) (ImpulseCFR�s�[�N���o�͈͐ݒ�(primary))		*/
#define	D_RRH_REG_DPD_IMPMEMB					(D_RRH_REG_DPD + 0x00000144)	/* Impulse CFR (Before) (Impulse�W��������bank�\��(�n�[�h�g�p��))	*/
#define	D_RRH_REG_DPD_IMPMEMA					(D_RRH_REG_DPD + 0x00000148)	/* Impulse CFR (Before) (Impulse�W��������bank�\��(�n�[�h�g�p��))	*/
#define	D_RRH_REG_DPD_IMPMEMRDSEL				(D_RRH_REG_DPD + 0x0000014C)	/* Impulse CFR (Read)						*/
#define	D_RRH_REG_DPD_ECFR						(D_RRH_REG_DPD + 0x00000180)	/* EnvelopeClipping CFR						*/
#define	D_RRH_REG_DPD_ICFRBFRMON				(D_RRH_REG_DPD + 0x000001C0)	/* CFR Monitor (ImpulseCFR���j�^ (Before))	*/
#define	D_RRH_REG_DPD_ICFRAFRMON				(D_RRH_REG_DPD + 0x000001C4)	/* CFR Monitor (ImpulseCFR���j�^ (After))	*/
#define	D_RRH_REG_DPD_ECFRMON					(D_RRH_REG_DPD + 0x000001CC)	/* CFR Monitor (EnvelopeClippingCFR���j�^)	*/
#define	D_RRH_REG_DPD_TSSI0						(D_RRH_REG_DPD + 0x00000200)	/* �d�͕�(�L�����A�d�͗p) (Cr0 TSSI����l)			*/
#define	D_RRH_REG_DPD_TSSI1						(D_RRH_REG_DPD + 0x00000204)	/* �d�͕�(�L�����A�d�͗p) (Cr1 TSSI����l)			*/
#define	D_RRH_REG_DPD_TSSISMPL					(D_RRH_REG_DPD + 0x00000214)	/* �d�͕�(�L�����A�d�͗p) (Cr TSSI�ݐσT���v�����ݒ�)	*/
#define	D_RRH_REG_DPD_TSSILAT					(D_RRH_REG_DPD + 0x00000218)	/* �d�͕�(�L�����A�d�͗p) (Cr TSSI�l�ێ�)				*/
#define	D_RRH_REG_DPD_TSSI						(D_RRH_REG_DPD + 0x0000021C)	/* �d�͕�(�L�����A�d�͗p) (Cr TSSI�ێ��l�N���A)		*/
#define	D_RRH_REG_DPD_CWEN						(D_RRH_REG_DPD + 0x00000280)	/* ���ϒ����M (CW�o�̓L�����A�C�l�[�u��)		*/
#define	D_RRH_REG_DPD_CWAMP						(D_RRH_REG_DPD + 0x00000284)	/* ���ϒ����M (CW�o�͐U���ݒ�(Ich�L�����A����))	*/
#define	D_RRH_REG_DPD_LPFREGEN					(D_RRH_REG_DPD + 0x000002A0)	/* LPF�W��									*/
#define	D_RRH_REG_DPD_TXNCOPH0					(D_RRH_REG_DPD + 0x00000300)	/* TEST-NCO (Cr0 ���g���V�t�g�����ʑ��ݒ�)	*/
#define	D_RRH_REG_DPD_TXNCOPH1					(D_RRH_REG_DPD + 0x00000304)	/* TEST-NCO (Cr1 ���g���V�t�g�����ʑ��ݒ�)	*/
#define	D_RRH_REG_DPD_CCTEST					(D_RRH_REG_DPD + 0x000003F8)	/* TEST���[�h								*/
#define	D_RRH_REG_DPD_CCMONSEL					(D_RRH_REG_DPD + 0x000003FC)	/* CC���j�^									*/
#define	D_RRH_REG_DPD_CRCAREN					(D_RRH_REG_DPD + 0x00000000)	/* TxEnable (�~���[�FCR���ŎQ��)			*/
#define	D_RRH_REG_DPD_CRCNF0					(D_RRH_REG_DPD + 0x00000010)	/* System (Cr0 config �M���ш�ݒ�)			*/
#define	D_RRH_REG_DPD_CRCNF1					(D_RRH_REG_DPD + 0x00000014)	/* System (Cr1 config �M���ш�ݒ�)			*/
#define	D_RRH_REG_DPD_CRTEST					(D_RRH_REG_DPD + 0x000003F8)	/* TEST���[�h(�~���[�FCR���ŎQ��)			*/
#define	D_RRH_REG_DPD_CRMONSEL					(D_RRH_REG_DPD + 0x000003FC)	/* CC���j�^(�~���[�FCR���ŎQ��)				*/
/* @} */

/*!
 * @name DPD���W�X�^bit & Shift
 * @note DPD���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
