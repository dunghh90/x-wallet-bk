/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dlf.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(DLF)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DLF_H
#define F_RRH_REG_DLF_H

/*!
 * @name DLF���W�X�^�A�h���X
 * @note DLF���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_DLF_DLFDGAIN0					(D_RRH_REG_DLF + 0x00004000)	/* DL��Gain����iCr0 - iFFT���͑��j			*/
#define	D_RRH_REG_DLF_DLTDGAIN0					(D_RRH_REG_DLF + 0x00004004)	/* DL��Gain����iCr0 - iFFF�o�͑��j			*/
#define	D_RRH_REG_DLF_DLFDGAIN1					(D_RRH_REG_DLF + 0x00004008)	/* DL��Gain����iCr1 - iFFT���͑��j			*/
#define	D_RRH_REG_DLF_DLTDGAIN1					(D_RRH_REG_DLF + 0x0000400C)	/* DL��Gain����iCr1 - iFFF�o�͑��j			*/
#define	D_RRH_REG_DLF_DLCAL						(D_RRH_REG_DLF + 0x00004010)	/* DL���L�����u���[�V�����C�l�[�u��			*/
#define	D_RRH_REG_DLF_DLBST						(D_RRH_REG_DLF + 0x00004014)	/* DL���␳�����C�l�[�u��					*/
#define	D_RRH_REG_DLF_DLBBFSEL					(D_RRH_REG_DLF + 0x00004018)	/* DL��BBF�␳�e�[�u������					*/
#define	D_RRH_REG_DLF_DLBBFMON					(D_RRH_REG_DLF + 0x0000401C)	/* DL��BBF�␳�e�[�u���g�p�ʒʒm			*/
#define	D_RRH_REG_DLF_DLCALTBL					(D_RRH_REG_DLF + 0x00004020)	/* TableRAM�A�N�Z�X���H����iCAL�␳�W���j	*/
#define	D_RRH_REG_DLF_DLCHFTBL					(D_RRH_REG_DLF + 0x00004024)	/* TableRAM�A�N�Z�X���H����iCHF�␳�W���j	*/
#define	D_RRH_REG_DLF_DLBBFTBL					(D_RRH_REG_DLF + 0x00004028)	/* TableRAM�A�N�Z�X���H����iDLB�␳�W���j	*/
#define	D_RRH_REG_DLF_DLWINTBL					(D_RRH_REG_DLF + 0x0000402C)	/* TableRAM�A�N�Z�X���H����i���W���j		*/
#define	D_RRH_REG_DLF_DLTBL						(D_RRH_REG_DLF + 0x00004030)	/* TableRAM���[�h�~�X�}�b�`					*/
#define	D_RRH_REG_DLF_DLPLS						(D_RRH_REG_DLF + 0x00004034)	/* TDD��M�p���X�ُ�						*/
#define	D_RRH_REG_DLF_DFDG						(D_RRH_REG_DLF + 0x00004038)	/* ���ZFlow���j�^(iFFT���͑�Gain����OverFlow)	*/
#define	D_RRH_REG_DLF_DBST						(D_RRH_REG_DLF + 0x0000403C)	/* ���ZFlow���j�^(Boost���Z��OverFlow)		*/
#define	D_RRH_REG_DLF_DTDG						(D_RRH_REG_DLF + 0x00004040)	/* ���ZFlow���j�^(iFFT�o�͑�Gain����OverFlow)	*/
#define	D_RRH_REG_DLF_DFFT						(D_RRH_REG_DLF + 0x00004044)	/* ���ZFlow���j�^(FFT���Z����OverFlow)		*/
#define	D_RRH_REG_DLF_DLMXQUE					(D_RRH_REG_DLF + 0x00004048)	/* �f�[�^Flow���j�^							*/
#define	D_RRH_REG_DLF_DWINCOEF					(D_RRH_REG_DLF + 0x00004400)	/* Window�����W���e�[�u��					*/
#define	D_RRH_REG_DLF_DLCALREVAL				(D_RRH_REG_DLF + 0x00004800)	/* CAL�␳�W���i�����j						*/
#define	D_RRH_REG_DLF_DLCALIMVAL				(D_RRH_REG_DLF + 0x00004C00)	/* CAL�␳�W���i�����j						*/
#define	D_RRH_REG_DLF_DLCHFVAL					(D_RRH_REG_DLF + 0x00005000)	/* CHF�␳�W��								*/
#define	D_RRH_REG_DLF_DLBBFVAL0					(D_RRH_REG_DLF + 0x00006000)	/* BBF�␳�W���i0�ʁj						*/
#define	D_RRH_REG_DLF_DLBBFVAL1					(D_RRH_REG_DLF + 0x00007000)	/* BBF�␳�W���i1�ʁj						*/
/* @} */

/*!
 * @name DLF���W�X�^bit & Shift
 * @note DLF���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
