/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_ulf.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(ULF)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_ULF_H
#define F_RRH_REG_ULF_H

/*!
 * @name ULF���W�X�^�A�h���X
 * @note ULF���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_ULF_ULFDGAIN0					(D_RRH_REG_ULF + 0x00004000)	/* UL��Gain����iCr0 - iFFT���͑��j			*/
#define	D_RRH_REG_ULF_ULTDGAIN0					(D_RRH_REG_ULF + 0x00004004)	/* UL��Gain����iCr0 - iFFF�o�͑��j			*/
#define	D_RRH_REG_ULF_ULFDGAIN1					(D_RRH_REG_ULF + 0x00004008)	/* UL��Gain����iCr1 - iFFT���͑��j			*/
#define	D_RRH_REG_ULF_ULTDGAIN1					(D_RRH_REG_ULF + 0x0000400C)	/* UL��Gain����iCr1 - iFFF�o�͑��j			*/
#define	D_RRH_REG_ULF_ULCAL						(D_RRH_REG_ULF + 0x00004010)	/* UL���L�����u���[�V�����C�l�[�u��			*/
#define	D_RRH_REG_ULF_ULBST						(D_RRH_REG_ULF + 0x00004014)	/* UL���␳�����C�l�[�u��					*/
#define	D_RRH_REG_ULF_ULBBFSEL					(D_RRH_REG_ULF + 0x00004018)	/* UL��BBF�␳�e�[�u������					*/
#define	D_RRH_REG_ULF_ULBBFMON					(D_RRH_REG_ULF + 0x0000401C)	/* UL��BBF�␳�e�[�u���g�p�ʒʒm			*/
#define	D_RRH_REG_ULF_ULCPRMV					(D_RRH_REG_ULF + 0x00004020)	/* CP Remove�ʒu							*/
#define	D_RRH_REG_ULF_ULCALTBL					(D_RRH_REG_ULF + 0x00004024)	/* TableRAM�A�N�Z�X���H����iCAL�␳�W���j	*/
#define	D_RRH_REG_ULF_ULCHFTBL					(D_RRH_REG_ULF + 0x00004028)	/* TableRAM�A�N�Z�X���H����iCHF�␳�W���j	*/
#define	D_RRH_REG_ULF_ULBBFTBL					(D_RRH_REG_ULF + 0x0000402C)	/* TableRAM�A�N�Z�X���H����iBBF�␳�W���j	*/
#define	D_RRH_REG_ULF_ULTBL						(D_RRH_REG_ULF + 0x00004030)	/* TableRAM���[�h�~�X�}�b�`					*/
#define	D_RRH_REG_ULF_UPLS						(D_RRH_REG_ULF + 0x00004034)	/* TDD��M�p���X�ُ�						*/
#define	D_RRH_REG_ULF_ULFDG						(D_RRH_REG_ULF + 0x00004038)	/* ���ZFlow���j�^(FFT���͑�Gain����OverFlow)*/
#define	D_RRH_REG_ULF_ULBST						(D_RRH_REG_ULF + 0x0000403C)	/* ���ZFlow���j�^(Boost���Z��OverFlow)		*/
#define	D_RRH_REG_ULF_ULTDG						(D_RRH_REG_ULF + 0x00004040)	/* ���ZFlow���j�^(FFT�o�͑�Gain����OverFlow)*/
#define	D_RRH_REG_ULF_ULFFT						(D_RRH_REG_ULF + 0x00004044)	/* ���ZFlow���j�^(FFT���Z����OverFlow)		*/
#define	D_RRH_REG_ULF_ULMXQUE					(D_RRH_REG_ULF + 0x00004048)	/* �f�[�^Flow���j�^							*/
#define	D_RRH_REG_ULF_ULICES					(D_RRH_REG_ULF + 0x0000404C)	/* UL�f�[�^��M�G���[���j�^					*/
#define	D_RRH_REG_ULF_ULCALREVAL				(D_RRH_REG_ULF + 0x00004800)	/* CAL�␳�W�� (����)						*/
#define	D_RRH_REG_ULF_ULCALIMVAL				(D_RRH_REG_ULF + 0x00004C00)	/* CAL�␳�W�� (����)						*/
#define	D_RRH_REG_ULF_ULCHFVAL					(D_RRH_REG_ULF + 0x00005000)	/* CHF�␳�W��								*/
#define	D_RRH_REG_ULF_ULBBFVAL0					(D_RRH_REG_ULF + 0x00006000)	/* BBF�␳�W���i0�ʁj						*/
#define	D_RRH_REG_ULF_ULBBFVAL1					(D_RRH_REG_ULF + 0x00007000)	/* BBF�␳�W���i1�ʁj						*/
/* @} */

/*!
 * @name ULF���W�X�^bit & Shift
 * @note ULF���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
