/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_eth.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(ETH)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_ETH_H
#define F_RRH_REG_ETH_H

/*!
 * @name ETH���W�X�^�A�h���X
 * @note ETH���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_ETH_MACSYSRST					(D_RRH_REG_ETH + 0x00000000)	/* MAC�V�X�e�����Z�b�g						*/
#define	D_RRH_REG_ETH_MACRST					(D_RRH_REG_ETH + 0x00000004)	/* MAC���Z�b�g								*/
#define	D_RRH_REG_ETH_RSTDONE					(D_RRH_REG_ETH + 0x00000010)	/* MAC���Z�b�g����							*/
#define	D_RRH_REG_ETH_RXCTL						(D_RRH_REG_ETH + 0x00000020)	/* MAC��M����								*/
#define	D_RRH_REG_ETH_TXCTL						(D_RRH_REG_ETH + 0x00000040)	/* MAC���M����								*/
#define	D_RRH_REG_ETH_UTXEN						(D_RRH_REG_ETH + 0x00000044)	/* U-Plane���M����							*/
#define	D_RRH_REG_ETH_RSFECCTL					(D_RRH_REG_ETH + 0x00000060)	/* RS FEC����								*/
#define	D_RRH_REG_ETH_FECCTL					(D_RRH_REG_ETH + 0x00000064)	/* FEC����									*/
#define	D_RRH_REG_ETH_PTPCTL					(D_RRH_REG_ETH + 0x00000080)	/* PTP����									*/
#define	D_RRH_REG_ETH_UPDCHKSUM					(D_RRH_REG_ETH + 0x00000084)	/* PTP UPD�`�F�b�N�T���ݒ�					*/
#define	D_RRH_REG_ETH_TSTAMPOFFSET				(D_RRH_REG_ETH + 0x00000088)	/* PTP�^�C���X�^���v�I�t�Z�b�g�ݒ�			*/
#define	D_RRH_REG_ETH_CHKSUMOFFSET				(D_RRH_REG_ETH + 0x0000008C)	/* PTP�`�F�b�N�T���I�t�Z�b�g�ݒ�			*/
#define	D_RRH_REG_ETH_SFPCNT					(D_RRH_REG_ETH + 0x000000E0)	/* SFP���� (SFP TX Enable)					*/
#define	D_RRH_REG_ETH_SFPRATE					(D_RRH_REG_ETH + 0x000000E4)	/* SFP���� (Driver Swing����)				*/
#define	D_RRH_REG_ETH_PHYCNT					(D_RRH_REG_ETH + 0x000000F0)	/* Ether PHY����							*/
#define	D_RRH_REG_ETH_IRQMACST					(D_RRH_REG_ETH + 0x00000100)	/* ������ MAC Status						*/
#define	D_RRH_REG_ETH_IRQMACSTM					(D_RRH_REG_ETH + 0x00000104)	/* �����݃}�X�N MAC Status					*/
#define	D_RRH_REG_ETH_IRQMACSTI					(D_RRH_REG_ETH + 0x00000108)	/* ������(��) MAC Status					*/
#define	D_RRH_REG_ETH_IRQMP						(D_RRH_REG_ETH + 0x00000110)	/* ������ M-plane���M����/��M����			*/
#define	D_RRH_REG_ETH_IRQMPM					(D_RRH_REG_ETH + 0x00000114)	/* �����݃}�X�N M-plane���M����/��M����	*/
#define	D_RRH_REG_ETH_IRQSP						(D_RRH_REG_ETH + 0x00000120)	/* ������ S-plane���M����/��M����			*/
#define	D_RRH_REG_ETH_IRQSPM					(D_RRH_REG_ETH + 0x00000124)	/* �����݃}�X�N S-plane���M����/��M����	*/
#define	D_RRH_REG_ETH_RXSTATUS					(D_RRH_REG_ETH + 0x00000180)	/* MAC��M�X�e�[�^�X1						*/
#define	D_RRH_REG_ETH_RXPAUSEREQ				(D_RRH_REG_ETH + 0x00000184)	/* MAC��M�X�e�[�^�X2						*/
#define	D_RRH_REG_ETH_RXVALIDCODE				(D_RRH_REG_ETH + 0x00000188)	/* MAC��M�X�e�[�^�X3						*/
#define	D_RRH_REG_ETH_RXBLOCKLOCK				(D_RRH_REG_ETH + 0x0000018C)	/* MAC��M�X�e�[�^�X4						*/
#define	D_RRH_REG_ETH_TXSTATUS					(D_RRH_REG_ETH + 0x000001A0)	/* MAC���M�X�e�[�^�X						*/
#define	D_RRH_REG_ETH_RSFECSTATUS				(D_RRH_REG_ETH + 0x000001C0)	/* RS-FEC�X�e�[�^�X							*/
#define	D_RRH_REG_ETH_FECSTATUS					(D_RRH_REG_ETH + 0x000001C4)	/* FEC�X�e�[�^�X							*/
#define	D_RRH_REG_ETH_BANKSTATUSM				(D_RRH_REG_ETH + 0x00000200)	/* ��MBank�X�e�[�^�X(M-Plane)				*/
#define	D_RRH_REG_ETH_BANKNOM					(D_RRH_REG_ETH + 0x00000204)	/* ��MBank�ԍ��ʒm(M-Plane)				*/
#define	D_RRH_REG_ETH_BANKSTATUSS				(D_RRH_REG_ETH + 0x00000210)	/* ��MBank�X�e�[�^�X(S-Plane)				*/
#define	D_RRH_REG_ETH_BANKNOS					(D_RRH_REG_ETH + 0x00000214)	/* ��MBank�ԍ��ʒm(S-Plane)				*/
#define	D_RRH_REG_ETH_FILTDA11					(D_RRH_REG_ETH + 0x00000280)	/* ��MFilter�pDA�ݒ�11						*/
#define	D_RRH_REG_ETH_FILTDA12					(D_RRH_REG_ETH + 0x00000284)	/* ��MFilter�pDA�ݒ�12						*/
#define	D_RRH_REG_ETH_FILTDA21					(D_RRH_REG_ETH + 0x00000288)	/* ��MFilter�pDA�ݒ�21						*/
#define	D_RRH_REG_ETH_FILTDA22					(D_RRH_REG_ETH + 0x0000028C)	/* ��MFilter�pDA�ݒ�22						*/
#define	D_RRH_REG_ETH_FILTDA31					(D_RRH_REG_ETH + 0x00000290)	/* ��MFilter�pDA�ݒ�31						*/
#define	D_RRH_REG_ETH_FILTDA32					(D_RRH_REG_ETH + 0x00000294)	/* ��MFilter�pDA�ݒ�32						*/
#define	D_RRH_REG_ETH_FILTDA41					(D_RRH_REG_ETH + 0x00000298)	/* ��MFilter�pDA�ݒ�41						*/
#define	D_RRH_REG_ETH_FILTDA42					(D_RRH_REG_ETH + 0x0000029C)	/* ��MFilter�pDA�ݒ�42						*/
#define	D_RRH_REG_ETH_FILTDA51					(D_RRH_REG_ETH + 0x000002A0)	/* ��MFilter�pDA�ݒ�51						*/
#define	D_RRH_REG_ETH_FILTDA52					(D_RRH_REG_ETH + 0x000002A4)	/* ��MFilter�pDA�ݒ�52						*/
#define	D_RRH_REG_ETH_FILTDA61					(D_RRH_REG_ETH + 0x000002A8)	/* ��MFilter�pDA�ݒ�61						*/
#define	D_RRH_REG_ETH_FILTDA62					(D_RRH_REG_ETH + 0x000002AC)	/* ��MFilter�pDA�ݒ�62						*/
#define	D_RRH_REG_ETH_FILTDA71					(D_RRH_REG_ETH + 0x000002B0)	/* ��MFilter�pDA�ݒ�71						*/
#define	D_RRH_REG_ETH_FILTDA72					(D_RRH_REG_ETH + 0x000002B4)	/* ��MFilter�pDA�ݒ�72						*/
#define	D_RRH_REG_ETH_FILTDA81					(D_RRH_REG_ETH + 0x000002B8)	/* ��MFilter�pDA�ݒ�81						*/
#define	D_RRH_REG_ETH_FILTDA82					(D_RRH_REG_ETH + 0x000002BC)	/* ��MFilter�pDA�ݒ�82						*/
#define	D_RRH_REG_ETH_FILTVID1					(D_RRH_REG_ETH + 0x00000300)	/* U/FC-Plane��M����pVID�ݒ�1				*/
#define	D_RRH_REG_ETH_FILTVID2					(D_RRH_REG_ETH + 0x00000304)	/* U/FC-Plane��M����pVID�ݒ�2				*/
#define	D_RRH_REG_ETH_FILTVID3					(D_RRH_REG_ETH + 0x00000308)	/* U/FC-Plane��M����pVID�ݒ�3				*/
#define	D_RRH_REG_ETH_FILTVID4					(D_RRH_REG_ETH + 0x0000030C)	/* U/FC-Plane��M����pVID�ݒ�4				*/
#define	D_RRH_REG_ETH_FILTVID5					(D_RRH_REG_ETH + 0x00000310)	/* U/FC-Plane��M����pVID�ݒ�5				*/
#define	D_RRH_REG_ETH_FILTVID6					(D_RRH_REG_ETH + 0x00000314)	/* U/FC-Plane��M����pVID�ݒ�6				*/
#define	D_RRH_REG_ETH_FILTVID7					(D_RRH_REG_ETH + 0x00000318)	/* U/FC-Plane��M����pVID�ݒ�7				*/
#define	D_RRH_REG_ETH_FILTVID8					(D_RRH_REG_ETH + 0x0000031C)	/* U/FC-Plane��M����pVID�ݒ�8				*/
#define	D_RRH_REG_ETH_FILTVID9					(D_RRH_REG_ETH + 0x00000320)	/* U/FC-Plane��M����pVID�ݒ�9				*/
#define	D_RRH_REG_ETH_FILTVID10					(D_RRH_REG_ETH + 0x00000324)	/* U/FC-Plane��M����pVID�ݒ�10			*/
#define	D_RRH_REG_ETH_FILTVID11					(D_RRH_REG_ETH + 0x00000328)	/* U/FC-Plane��M����pVID�ݒ�11			*/
#define	D_RRH_REG_ETH_FILTVID12					(D_RRH_REG_ETH + 0x0000032C)	/* U/FC-Plane��M����pVID�ݒ�12			*/
#define	D_RRH_REG_ETH_FILTVID13					(D_RRH_REG_ETH + 0x00000330)	/* U/FC-Plane��M����pVID�ݒ�13			*/
#define	D_RRH_REG_ETH_FILTVID14					(D_RRH_REG_ETH + 0x00000334)	/* U/FC-Plane��M����pVID�ݒ�14			*/
#define	D_RRH_REG_ETH_FILTVID15					(D_RRH_REG_ETH + 0x00000338)	/* U/FC-Plane��M����pVID�ݒ�15			*/
#define	D_RRH_REG_ETH_FILTVID16					(D_RRH_REG_ETH + 0x0000033C)	/* U/FC-Plane��M����pVID�ݒ�16			*/
#define	D_RRH_REG_ETH_FILTETYPE1				(D_RRH_REG_ETH + 0x00000380)	/* S-Plane��MFilter�pE-Type�ݒ�1			*/
#define	D_RRH_REG_ETH_FILTETYPE2				(D_RRH_REG_ETH + 0x00000384)	/* S-Plane��MFilter�pE-Type�ݒ�2			*/
#define	D_RRH_REG_ETH_TXTRIGM					(D_RRH_REG_ETH + 0x00000400)	/* ���M�g���K(M-Plane)						*/
#define	D_RRH_REG_ETH_TXLENGTHM					(D_RRH_REG_ETH + 0x00000404)	/* ���M�t���[����(M-Plane)					*/
#define	D_RRH_REG_ETH_TXTRIGS					(D_RRH_REG_ETH + 0x00000410)	/* ���M�g���K(S-Plane)						*/
#define	D_RRH_REG_ETH_TXLENGTHS					(D_RRH_REG_ETH + 0x00000414)	/* ���M�t���[����(S-Plane)					*/
#define	D_RRH_REG_ETH_TXPTPTAGS					(D_RRH_REG_ETH + 0x00000418)	/* ���MPTP�^�O�ݒ�							*/
#define	D_RRH_REG_ETH_TXPTPTAG					(D_RRH_REG_ETH + 0x00000420)	/* ���MPTP�^�C���X�^���v�^�O�t�B�[���h		*/
#define	D_RRH_REG_ETH_TXTSTAMP1					(D_RRH_REG_ETH + 0x00000424)	/* ���MPTP�^�C���X�^���v1					*/
#define	D_RRH_REG_ETH_TXTSTAMP2					(D_RRH_REG_ETH + 0x00000428)	/* ���MPTP�^�C���X�^���v2					*/
#define	D_RRH_REG_ETH_TXTSTAMP3					(D_RRH_REG_ETH + 0x0000042C)	/* ���MPTP�^�C���X�^���v3					*/
#define	D_RRH_REG_ETH_MTIMER					(D_RRH_REG_ETH + 0x00000470)	/* M-Plane���M�D��^�C�}					*/
#define	D_RRH_REG_ETH_CAPDA11					(D_RRH_REG_ETH + 0x00000480)	/* U-Plane ���M�J�v�Z���pDA�ݒ�1			*/
#define	D_RRH_REG_ETH_CAPDA12					(D_RRH_REG_ETH + 0x00000484)	/* U-Plane ���M�J�v�Z���pDA�ݒ�2			*/
#define	D_RRH_REG_ETH_CAPSA11					(D_RRH_REG_ETH + 0x00000488)	/* U-Plane ���M�J�v�Z���pSA�ݒ�1			*/
#define	D_RRH_REG_ETH_CAPSA12					(D_RRH_REG_ETH + 0x0000048C)	/* U-Plane ���M�J�v�Z���pSA�ݒ�2			*/
#define	D_RRH_REG_ETH_CAPTPID					(D_RRH_REG_ETH + 0x00000490)	/* U-Plane ���M�J�v�Z���pTPID				*/
#define	D_RRH_REG_ETH_CAPTAG					(D_RRH_REG_ETH + 0x00000494)	/* U-Plane ���M�J�v�Z���pTAG				*/
#define	D_RRH_REG_ETH_SYSTIMLDSH				(D_RRH_REG_ETH + 0x00000600)	/* SystemTimer LD�l�ݒ�1					*/
#define	D_RRH_REG_ETH_SYSTIMLDSL				(D_RRH_REG_ETH + 0x00000604)	/* SystemTimer LD�l�ݒ�2					*/
#define	D_RRH_REG_ETH_SYSTIMLDN					(D_RRH_REG_ETH + 0x00000608)	/* SystemTimer LD�l�ݒ�3					*/
#define	D_RRH_REG_ETH_SYSTIMLDEN				(D_RRH_REG_ETH + 0x0000060C)	/* SystemTimer LD EN�ݒ�					*/
#define	D_RRH_REG_ETH_SYSTIMVALLT				(D_RRH_REG_ETH + 0x00000610)	/* SystemTimer���b�`�g���K					*/
#define	D_RRH_REG_ETH_SYSTIMVALSH				(D_RRH_REG_ETH + 0x00000614)	/* SystemTimer�l 1							*/
#define	D_RRH_REG_ETH_SYSTIMVALSL				(D_RRH_REG_ETH + 0x00000618)	/* SystemTimer�l 2							*/
#define	D_RRH_REG_ETH_SYSTIMVALN				(D_RRH_REG_ETH + 0x0000061C)	/* SystemTimer�l 3							*/
#define	D_RRH_REG_ETH_STATLAT					(D_RRH_REG_ETH + 0x00000800)	/* ���v�J�E���^���b�`&�N���A�g���K(�ꊇ)	*/
#define	D_RRH_REG_ETH_STATLAT1					(D_RRH_REG_ETH + 0x00000804)	/* ���v�J�E���^���b�`&�N���A�g���K1(��)	*/
#define	D_RRH_REG_ETH_STATLAT2					(D_RRH_REG_ETH + 0x00000808)	/* ���v�J�E���^���b�`&�N���A�g���K2(��)	*/
#define	D_RRH_REG_ETH_RXUPLANE					(D_RRH_REG_ETH + 0x00000810)	/* DRP���v(��M)1							*/
#define	D_RRH_REG_ETH_RXMPLANE					(D_RRH_REG_ETH + 0x00000814)	/* DRP���v(��M)2							*/
#define	D_RRH_REG_ETH_RXSPLANE					(D_RRH_REG_ETH + 0x00000818)	/* DRP���v(��M)3							*/
#define	D_RRH_REG_ETH_RXDAERR					(D_RRH_REG_ETH + 0x0000081C)	/* DRP���v(��M)4							*/
#define	D_RRH_REG_ETH_RXETYPEERR				(D_RRH_REG_ETH + 0x00000820)	/* DRP���v(��M)5							*/
#define	D_RRH_REG_ETH_RXVERERR					(D_RRH_REG_ETH + 0x00000824)	/* DRP���v(��M)6							*/
#define	D_RRH_REG_ETH_STATRXFRERR				(D_RRH_REG_ETH + 0x00000828)	/* MAC���v_RX1								*/
#define	D_RRH_REG_ETH_STATRXSMALL				(D_RRH_REG_ETH + 0x0000082C)	/* MAC���v_RX2								*/
#define	D_RRH_REG_ETH_STATRXJABER				(D_RRH_REG_ETH + 0x00000830)	/* MAC���v_RX3								*/
#define	D_RRH_REG_ETH_STATRXLARGE				(D_RRH_REG_ETH + 0x00000834)	/* MAC���v_RX4								*/
#define	D_RRH_REG_ETH_STATRXOVRSIZE				(D_RRH_REG_ETH + 0x00000838)	/* MAC���v_RX5								*/
#define	D_RRH_REG_ETH_STATRXUDRSIZE				(D_RRH_REG_ETH + 0x0000083C)	/* MAC���v_RX6								*/
#define	D_RRH_REG_ETH_STATRXTOOLONG				(D_RRH_REG_ETH + 0x00000840)	/* MAC���v_RX7								*/
#define	D_RRH_REG_ETH_STATRXFRAGMNT				(D_RRH_REG_ETH + 0x00000844)	/* MAC���v_RX8								*/
#define	D_RRH_REG_ETH_STATRXBADCODE				(D_RRH_REG_ETH + 0x00000848)	/* MAC���v_RX9								*/
#define	D_RRH_REG_ETH_STATRXTOTALPACKET			(D_RRH_REG_ETH + 0x0000084C)	/* MAC���v_RX10								*/
#define	D_RRH_REG_ETH_STATRXBADFCS				(D_RRH_REG_ETH + 0x00000850)	/* MAC���v_RX11								*/
#define	D_RRH_REG_ETH_STATRXSTOMPEDFCS			(D_RRH_REG_ETH + 0x00000854)	/* MAC���v_RX12								*/
#define	D_RRH_REG_ETH_STATRXUNICAST				(D_RRH_REG_ETH + 0x00000858)	/* MAC���v_RX13								*/
#define	D_RRH_REG_ETH_STATRXMULTICAST			(D_RRH_REG_ETH + 0x0000085C)	/* MAC���v_RX14								*/
#define	D_RRH_REG_ETH_STATRXBROADCAST			(D_RRH_REG_ETH + 0x00000860)	/* MAC���v_RX15								*/
#define	D_RRH_REG_ETH_STATRXPAUSE				(D_RRH_REG_ETH + 0x00000864)	/* MAC���v_RX16								*/
#define	D_RRH_REG_ETH_OVFFRAMES					(D_RRH_REG_ETH + 0x00000868)	/* DRP���v(S-Plane)1						*/
#define	D_RRH_REG_ETH_AXIERRFRAMES				(D_RRH_REG_ETH + 0x0000086C)	/* DRP���v(S-Plane)2						*/
#define	D_RRH_REG_ETH_OVFFRAMEM					(D_RRH_REG_ETH + 0x00000870)	/* DRP���v(M-Plane)1						*/
#define	D_RRH_REG_ETH_AXIERRFRAMEM				(D_RRH_REG_ETH + 0x00000874)	/* DRP���v(M-Plane)2						*/
#define	D_RRH_REG_ETH_TXUPLANE					(D_RRH_REG_ETH + 0x00000878)	/* INS���v(���M)1							*/
#define	D_RRH_REG_ETH_TXMPLANE					(D_RRH_REG_ETH + 0x0000087C)	/* INS���v(���M)2							*/
#define	D_RRH_REG_ETH_TXSPLANE					(D_RRH_REG_ETH + 0x00000880)	/* INS���v(���M)3							*/
#define	D_RRH_REG_ETH_STATTXPACKETS				(D_RRH_REG_ETH + 0x00000884)	/* MAC���v_TX1								*/
#define	D_RRH_REG_ETH_STATTXBADFCS				(D_RRH_REG_ETH + 0x00000888)	/* MAC���v_TX2								*/
#define	D_RRH_REG_ETH_STATTXFRAMEERROR			(D_RRH_REG_ETH + 0x0000088C)	/* MAC���v_TX3								*/
#define	D_RRH_REG_ETH_FECCOR					(D_RRH_REG_ETH + 0x00000890)	/* FEC���v1									*/
#define	D_RRH_REG_ETH_FECCANTCOR				(D_RRH_REG_ETH + 0x00000894)	/* FEC���v2									*/
#define	D_RRH_REG_ETH_RSFECCOR					(D_RRH_REG_ETH + 0x00000898)	/* RS-FEC���v1								*/
#define	D_RRH_REG_ETH_RSFECUNCOR				(D_RRH_REG_ETH + 0x0000089C)	/* RS-FEC���v2								*/
#define	D_RRH_REG_ETH_RSFECERR					(D_RRH_REG_ETH + 0x000008A0)	/* RS-FEC���v3								*/
#define	D_RRH_REG_ETH_GTLOOPBACK				(D_RRH_REG_ETH + 0x00000C00)	/* ���[�v�o�b�N								*/
#define	D_RRH_REG_ETH_IBERTSEL					(D_RRH_REG_ETH + 0x00000C10)	/* InsystemIBERT�pDRP�ؑ�					*/
#define	D_RRH_REG_ETH_EYESCAN					(D_RRH_REG_ETH + 0x00000C20)	/* GT�f�o�b�O (eyescantrigger�C�x���g)		*/
#define	D_RRH_REG_ETH_PCSRSVDIN					(D_RRH_REG_ETH + 0x00000C24)	/* GT�f�o�b�O (PCSRSVDIN)					*/
#define	D_RRH_REG_ETH_GTRXCTL					(D_RRH_REG_ETH + 0x00000C28)	/* GT�f�o�b�O (rx�o�̓N���b�N�I��)			*/
#define	D_RRH_REG_ETH_GTTXCTL1					(D_RRH_REG_ETH + 0x00000C2C)	/* GT�f�o�b�O (���C���J�[�\���W��)			*/
#define	D_RRH_REG_ETH_GTTXCTL2					(D_RRH_REG_ETH + 0x00000C30)	/* GT�f�o�b�O (Tx�o�̓N���b�N�I��)			*/
#define	D_RRH_REG_ETH_RXLEN						(D_RRH_REG_ETH + 0x00000C80)	/* RX�R���g���[�� (��MMAX�p�P�b�g��)			*/
#define	D_RRH_REG_ETH_RXCTL2					(D_RRH_REG_ETH + 0x00000C84)	/* RX�R���g���[�� (Custom Preamble�C�l�[�u��)	*/
#define	D_RRH_REG_ETH_RXFORCERESYNC				(D_RRH_REG_ETH + 0x00000C88)	/* RX�R���g���[�� (RX�����ē���)				*/
#define	D_RRH_REG_ETH_RXFCCTL1					(D_RRH_REG_ETH + 0x00000CC0)	/* RX PAUSE�R���g���[�� (��M�|�[�Y�C�l�[�u��)							*/
#define	D_RRH_REG_ETH_RXFCCTL2					(D_RRH_REG_ETH + 0x00000CC4)	/* RX PAUSE�R���g���[�� (�O���[�o��Pause OpeCode�����L��)				*/
#define	D_RRH_REG_ETH_RXETYPE					(D_RRH_REG_ETH + 0x00000CC8)	/* RX PAUSE�R���g���[�� (�O���[�o���|�[�Y�����p��EtherType�t�B�[���h)	*/
#define	D_RRH_REG_ETH_RXOCGCP					(D_RRH_REG_ETH + 0x00000CCC)	/* RX PAUSE�R���g���[�� (�O���[�o������p�P�b�g�I�y�R�[�h(Max))			*/
#define	D_RRH_REG_ETH_RXOCGPP					(D_RRH_REG_ETH + 0x00000CD0)	/* RX PAUSE�R���g���[�� (�O���[�o���|�[�Y�p�P�b�g�I�y�R�[�h)			*/
#define	D_RRH_REG_ETH_PAUSEDAMC1				(D_RRH_REG_ETH + 0x00000CD4)	/* RX PAUSE�R���g���[�� (Pause �����pMC DA(MSB))						*/
#define	D_RRH_REG_ETH_PAUSEDAMC2				(D_RRH_REG_ETH + 0x00000CD8)	/* RX PAUSE�R���g���[�� (Pause �����pMC DA(LSB))						*/
#define	D_RRH_REG_ETH_RSTMASK					(D_RRH_REG_ETH + 0x00000D00)	/* �n�[�h�������Z�b�g�}�X�N					*/
#define	D_RRH_REG_ETH_RSTTIMER					(D_RRH_REG_ETH + 0x00000D04)	/* �n�[�h�������Z�b�g�ی쎞��				*/
#define	D_RRH_REG_ETH_STATEMONS					(D_RRH_REG_ETH + 0x00000E00)	/* ��M�t���[���]��State���j�^ (S-Plane)	*/
#define	D_RRH_REG_ETH_STATEMONM					(D_RRH_REG_ETH + 0x00000E04)	/* ��M�t���[���]��State���j�^ (M-Plane)	*/
#define	D_RRH_REG_ETH_FIFOSTATUS				(D_RRH_REG_ETH + 0x00000E80)	/* INS��FIFO Status���j�^					*/
#define	D_RRH_REG_ETH_PAUSESTATUS				(D_RRH_REG_ETH + 0x00000F00)	/* RX Pause�X�e�[�^�X						*/
#define	D_RRH_REG_ETH_GTSTATUS					(D_RRH_REG_ETH + 0x00000F04)	/* GT�X�e�[�^�X								*/
#define	D_RRH_REG_ETH_DBSYTIMS					(D_RRH_REG_ETH + 0x00000F80)	/* SystemTimer Debug Set					*/
#define	D_RRH_REG_ETH_DBSYTIMRD					(D_RRH_REG_ETH + 0x00000F84)	/* SystemTimer Debug Rd						*/
/* @} */

/*!
 * @name ETH���W�X�^bit & Shift
 * @note ETH���W�X�^bit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
