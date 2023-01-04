/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_ctb.h
 * @brief ���W�X�^�A�h���X��`�w�b�_(CTB)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CTB_H
#define F_RRH_REG_CTB_H

/*!
 * @name CTB���W�X�^�A�h���X
 * @note CTB���W�X�^�A�h���X
 * @{
 */
#define	D_RRH_REG_CTB_FPGAVER					(D_RRH_REG_CTB + 0x00000000)	/* VERSION(PS FPGA)							*/
#define	D_RRH_REG_CTB_REGCHK					(D_RRH_REG_CTB + 0x00000004)	/* CHECK									*/
#define	D_RRH_REG_CTB_BID						(D_RRH_REG_CTB + 0x00000008)	/* Borad ID									*/
#define	D_RRH_REG_CTB_RTC1						(D_RRH_REG_CTB + 0x00000010)	/* RTC1 (Date)								*/
#define	D_RRH_REG_CTB_RTC2						(D_RRH_REG_CTB + 0x00000014)	/* RTC2 (Time)								*/
#define	D_RRH_REG_CTB_SFPPSSD					(D_RRH_REG_CTB + 0x00000028)	/* SFP LDO ShutDown����						*/
#define	D_RRH_REG_CTB_RST						(D_RRH_REG_CTB + 0x00000040)	/* �N��(Reset)����							*/
#define	D_RRH_REG_CTB_RSTPHY1					(D_RRH_REG_CTB + 0x00000044)	/* �N��(Reset)����(�e��SerDes)				*/
#define	D_RRH_REG_CTB_RSTPHY2					(D_RRH_REG_CTB + 0x00000048)	/* �N��(Reset)����(�e��SerDes)				*/
#define	D_RRH_REG_CTB_PHYPWR1					(D_RRH_REG_CTB + 0x00000050)	/* SerDes PowerOn����						*/
#define	D_RRH_REG_CTB_PHYPWR2					(D_RRH_REG_CTB + 0x00000054)	/* SerDes PowerOn����						*/
#define	D_RRH_REG_CTB_SV1INF					(D_RRH_REG_CTB + 0x00000150)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#0)			*/
#define	D_RRH_REG_CTB_SV2INF					(D_RRH_REG_CTB + 0x00000154)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#1)			*/
#define	D_RRH_REG_CTB_SV3INF					(D_RRH_REG_CTB + 0x00000158)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#2)			*/
#define	D_RRH_REG_CTB_SV4INF					(D_RRH_REG_CTB + 0x0000015C)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#3)			*/
#define	D_RRH_REG_CTB_SV5INF					(D_RRH_REG_CTB + 0x00000160)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#4)			*/
#define	D_RRH_REG_CTB_SV6INF					(D_RRH_REG_CTB + 0x00000164)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#5)			*/
#define	D_RRH_REG_CTB_SV7INF					(D_RRH_REG_CTB + 0x00000168)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#6)			*/
#define	D_RRH_REG_CTB_SV8INF					(D_RRH_REG_CTB + 0x0000016C)	/* SV�ݒ� (SV�o�X���䃌�W�X�^#7)			*/
#define	D_RRH_REG_CTB_SV1CMPM					(D_RRH_REG_CTB + 0x00000170)	/* SV�ω������݃}�X�N #0					*/
#define	D_RRH_REG_CTB_SV2CMPM					(D_RRH_REG_CTB + 0x00000174)	/* SV�ω������݃}�X�N #1					*/
#define	D_RRH_REG_CTB_SV3CMPM					(D_RRH_REG_CTB + 0x00000178)	/* SV�ω������݃}�X�N #2					*/
#define	D_RRH_REG_CTB_SV4CMPM					(D_RRH_REG_CTB + 0x0000017C)	/* SV�ω������݃}�X�N #3					*/
#define	D_RRH_REG_CTB_SV5CMPM					(D_RRH_REG_CTB + 0x00000180)	/* SV�ω������݃}�X�N #4					*/
#define	D_RRH_REG_CTB_SV6CMPM					(D_RRH_REG_CTB + 0x00000184)	/* SV�ω������݃}�X�N #5					*/
#define	D_RRH_REG_CTB_SV7CMPM					(D_RRH_REG_CTB + 0x00000188)	/* SV�ω������݃}�X�N #6					*/
#define	D_RRH_REG_CTB_SV8CMPM					(D_RRH_REG_CTB + 0x0000018C)	/* SV�ω������݃}�X�N #7					*/
#define	D_RRH_REG_CTB_STA1						(D_RRH_REG_CTB + 0x00000200)	/* FPGA��ԕ\�����W�X�^1					*/
#define	D_RRH_REG_CTB_STA1M						(D_RRH_REG_CTB + 0x00000204)	/* FPGA��ԕ\�����W�X�^1�}�X�N				*/
#define	D_RRH_REG_CTB_STA1F						(D_RRH_REG_CTB + 0x00000208)	/* FPGA��ԕ\�����W�X�^1�[���ݒ�			*/
#define	D_RRH_REG_CTB_STA2						(D_RRH_REG_CTB + 0x00000210)	/* FPGA��ԕ\�����W�X�^2					*/
#define	D_RRH_REG_CTB_STA2M						(D_RRH_REG_CTB + 0x00000214)	/* FPGA��ԕ\�����W�X�^2�}�X�N				*/
#define	D_RRH_REG_CTB_STA2F						(D_RRH_REG_CTB + 0x00000218)	/* FPGA��ԕ\�����W�X�^2�[���ݒ�			*/
#define	D_RRH_REG_CTB_SFPSTA					(D_RRH_REG_CTB + 0x00000300)	/* SFP��ԕ\�����W�X�^						*/
#define	D_RRH_REG_CTB_SFPSTA2M					(D_RRH_REG_CTB + 0x00000304)	/* SFP��ԕ\�����W�X�^�}�X�N				*/
#define	D_RRH_REG_CTB_SFPSTA2F					(D_RRH_REG_CTB + 0x00000308)	/* SFP��ԕ\�����W�X�^�[���ݒ�				*/
#define	D_RRH_REG_CTB_PTPPLLSTA					(D_RRH_REG_CTB + 0x00000370)	/* PTPPLL�X�e�[�^�X�\��						*/
#define	D_RRH_REG_CTB_JCPLLALM					(D_RRH_REG_CTB + 0x00000380)	/* JCPLL ALM�����J�E���g					*/
#define	D_RRH_REG_CTB_SFPVALM					(D_RRH_REG_CTB + 0x00000390)	/* SFP �ߓd���A���[�����\��(SV��M���)	*/
#define	D_RRH_REG_CTB_IVSEN						(D_RRH_REG_CTB + 0x00000420)	/* �d��/�d���Z���T�[������\��			*/
#define	D_RRH_REG_CTB_RFICSPICS					(D_RRH_REG_CTB + 0x00000500)	/* RF-IC��SPI CS�I��						*/
#define	D_RRH_REG_CTB_I2CSEL					(D_RRH_REG_CTB + 0x00000580)	/* SFP I2C�I��								*/
#define	D_RRH_REG_CTB_CFG						(D_RRH_REG_CTB + 0x00000600)	/* TRX-FPGA Config����						*/
#define	D_RRH_REG_CTB_CFGSET1					(D_RRH_REG_CTB + 0x00000604)	/* TRX-FPGA Config�ݒ�1						*/
#define	D_RRH_REG_CTB_CFGSET2					(D_RRH_REG_CTB + 0x00000608)	/* TRX-FPGA Config�ݒ�2						*/
#define	D_RRH_REG_CTB_CFGSET3					(D_RRH_REG_CTB + 0x0000060C)	/* TRX-FPGA Config�ݒ�3						*/
#define	D_RRH_REG_CTB_CFGSET4					(D_RRH_REG_CTB + 0x00000610)	/* TRX-FPGA Config�ݒ�4						*/
#define	D_RRH_REG_CTB_CFGSET5					(D_RRH_REG_CTB + 0x00000614)	/* TRX-FPGA Config�ݒ�5						*/
#define	D_RRH_REG_CTB_FRAMCTL					(D_RRH_REG_CTB + 0x00000700)	/* �e�q�`�l����								*/
#define	D_RRH_REG_CTB_FRAMDTC					(D_RRH_REG_CTB + 0x00000704)	/* �e�q�`�l�������ݐݒ�						*/
#define	D_RRH_REG_CTB_SYSLED					(D_RRH_REG_CTB + 0x00000800)	/* System LED���� ��SVIF�]��(PMCPLD��)		*/
#define	D_RRH_REG_CTB_IC1PHYCNT					(D_RRH_REG_CTB + 0x00000910)	/* InterConnect Ch1 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC2PHYCNT					(D_RRH_REG_CTB + 0x00000914)	/* InterConnect Ch2 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC3PHYCNT					(D_RRH_REG_CTB + 0x00000918)	/* InterConnect Ch3 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC4PHYCNT					(D_RRH_REG_CTB + 0x0000091C)	/* InterConnect Ch4 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC5PHYCNT					(D_RRH_REG_CTB + 0x00000920)	/* InterConnect Ch5 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC6PHYCNT					(D_RRH_REG_CTB + 0x00000924)	/* InterConnect Ch6 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC7PHYCNT					(D_RRH_REG_CTB + 0x00000928)	/* InterConnect Ch7 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC8PHYCNT					(D_RRH_REG_CTB + 0x0000092C)	/* InterConnect Ch8 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC9PHYCNT					(D_RRH_REG_CTB + 0x00000930)	/* InterConnect Ch9 Driver Swing Control & Emphasis		*/
#define	D_RRH_REG_CTB_IC10PHYCNT				(D_RRH_REG_CTB + 0x00000934)	/* InterConnect Ch10 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC11PHYCNT				(D_RRH_REG_CTB + 0x00000938)	/* InterConnect Ch11 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC12PHYCNT				(D_RRH_REG_CTB + 0x0000093C)	/* InterConnect Ch12 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC13PHYCNT				(D_RRH_REG_CTB + 0x00000940)	/* InterConnect Ch13 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC14PHYCNT				(D_RRH_REG_CTB + 0x00000944)	/* InterConnect Ch14 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC15PHYCNT				(D_RRH_REG_CTB + 0x00000948)	/* InterConnect Ch15 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_IC16PHYCNT				(D_RRH_REG_CTB + 0x0000094C)	/* InterConnect Ch16 Driver Swing Control & Emphasis	*/
#define	D_RRH_REG_CTB_SVPROT					(D_RRH_REG_CTB + 0x00000960)	/* SV(Vender Spec)�ω��Ď�����							*/
#define	D_RRH_REG_CTB_LEDTIM					(D_RRH_REG_CTB + 0x00000970)	/* SystemLED�_�Ŏ����ݒ�								*/
#define	D_RRH_REG_CTB_SVIFERR					(D_RRH_REG_CTB + 0x00000980)	/* SVI/F �G���[�\��										*/
#define	D_RRH_REG_CTB_SVIFTO					(D_RRH_REG_CTB + 0x00000984)	/* SVI/F Timeout�\��									*/
#define	D_RRH_REG_CTB_PMSVIFTXEN				(D_RRH_REG_CTB + 0x00000988)	/* CPLD�� SVI/F TX Enable								*/
#define	D_RRH_REG_CTB_TRXSVIFTXEN				(D_RRH_REG_CTB + 0x0000098C)	/* TRX-FPGA�� SVI/F TX Enable(����)						*/
#define	D_RRH_REG_CTB_PMSVIFTXD4				(D_RRH_REG_CTB + 0x00000990)	/* PM-PLD-FPGA�� SVI/F TX Data4�ݒ�						*/
#define	D_RRH_REG_CTB_PMSVIFTXD3				(D_RRH_REG_CTB + 0x00000994)	/* PM-PLD-FPGA�� SVI/F TX Data3�ݒ�						*/
#define	D_RRH_REG_CTB_PMSVIFTXD2				(D_RRH_REG_CTB + 0x00000998)	/* PM-PLD-FPGA�� SVI/F TX Data2�ݒ�						*/
#define	D_RRH_REG_CTB_PMSVIFTXD1				(D_RRH_REG_CTB + 0x0000099C)	/* PM-PLD-FPGA�� SVI/F TX Data1�ݒ�						*/
#define	D_RRH_REG_CTB_PMSVIFRXD4				(D_RRH_REG_CTB + 0x000009A0)	/* PM-PLD-FPGA�� SVI/F RX Data4�\��						*/
#define	D_RRH_REG_CTB_PMSVIFRXD3				(D_RRH_REG_CTB + 0x000009A4)	/* PM-PLD-FPGA�� SVI/F RX Data3�\��						*/
#define	D_RRH_REG_CTB_PMSVIFRXD2				(D_RRH_REG_CTB + 0x000009A8)	/* PM-PLD-FPGA�� SVI/F RX Data2�\��						*/
#define	D_RRH_REG_CTB_PMSVIFRXD1				(D_RRH_REG_CTB + 0x000009AC)	/* PM-PLD-FPGA�� SVI/F RX Data1�\��						*/
#define	D_RRH_REG_CTB_TRX1SVIFTXD4				(D_RRH_REG_CTB + 0x000009B0)	/* TRX1-FPGA�� SVI/F TX Data4�ݒ�						*/
#define	D_RRH_REG_CTB_TRX1SVIFTXD3				(D_RRH_REG_CTB + 0x000009B4)	/* TRX1-FPGA�� SVI/F TX Data3�ݒ�						*/
#define	D_RRH_REG_CTB_TRX1SVIFTXD2				(D_RRH_REG_CTB + 0x000009B8)	/* TRX1-FPGA�� SVI/F TX Data2�ݒ�						*/
#define	D_RRH_REG_CTB_TRX1SVIFTXD1				(D_RRH_REG_CTB + 0x000009BC)	/* TRX1-FPGA�� SVI/F TX Data1�ݒ�						*/
#define	D_RRH_REG_CTB_TRX1SVIFRXD4				(D_RRH_REG_CTB + 0x000009C0)	/* TRX1-FPGA�� SVI/F RX Data4�\��						*/
#define	D_RRH_REG_CTB_TRX1SVIFRXD3				(D_RRH_REG_CTB + 0x000009C4)	/* TRX1-FPGA�� SVI/F RX Data3�\��						*/
#define	D_RRH_REG_CTB_TRX1SVIFRXD2				(D_RRH_REG_CTB + 0x000009C8)	/* TRX1-FPGA�� SVI/F RX Data2�\��						*/
#define	D_RRH_REG_CTB_TRX1SVIFRXD1				(D_RRH_REG_CTB + 0x000009CC)	/* TRX1-FPGA�� SVI/F RX Data1�\��						*/
#define	D_RRH_REG_CTB_TRX2SVIFTXD4				(D_RRH_REG_CTB + 0x000009D0)	/* TRX2-FPGA�� SVI/F TX Data4�ݒ�						*/
#define	D_RRH_REG_CTB_TRX2SVIFTXD3				(D_RRH_REG_CTB + 0x000009D4)	/* TRX2-FPGA�� SVI/F TX Data3�ݒ�						*/
#define	D_RRH_REG_CTB_TRX2SVIFTXD2				(D_RRH_REG_CTB + 0x000009D8)	/* TRX2-FPGA�� SVI/F TX Data2�ݒ�						*/
#define	D_RRH_REG_CTB_TRX2SVIFTXD1				(D_RRH_REG_CTB + 0x000009DC)	/* TRX2-FPGA�� SVI/F TX Data1�ݒ�						*/
#define	D_RRH_REG_CTB_TRX2SVIFRXD4				(D_RRH_REG_CTB + 0x000009E0)	/* TRX2-FPGA�� SVI/F RX Data4�\��						*/
#define	D_RRH_REG_CTB_TRX2SVIFRXD3				(D_RRH_REG_CTB + 0x000009E4)	/* TRX2-FPGA�� SVI/F RX Data3�\��						*/
#define	D_RRH_REG_CTB_TRX2SVIFRXD2				(D_RRH_REG_CTB + 0x000009E8)	/* TRX2-FPGA�� SVI/F RX Data2�\��						*/
#define	D_RRH_REG_CTB_TRX2SVIFRXD1				(D_RRH_REG_CTB + 0x000009EC)	/* TRX2-FPGA�� SVI/F RX Data1�\��						*/
#define	D_RRH_REG_CTB_TRX3SVIFTXD4				(D_RRH_REG_CTB + 0x000009F0)	/* TRX3-FPGA�� SVI/F TX Data4�ݒ�						*/
#define	D_RRH_REG_CTB_TRX3SVIFTXD3				(D_RRH_REG_CTB + 0x000009F4)	/* TRX3-FPGA�� SVI/F TX Data3�ݒ�						*/
#define	D_RRH_REG_CTB_TRX3SVIFTXD2				(D_RRH_REG_CTB + 0x000009F8)	/* TRX3-FPGA�� SVI/F TX Data2�ݒ�						*/
#define	D_RRH_REG_CTB_TRX3SVIFTXD1				(D_RRH_REG_CTB + 0x000009FC)	/* TRX3-FPGA�� SVI/F TX Data1�ݒ�						*/
#define	D_RRH_REG_CTB_TRX3SVIFRXD4				(D_RRH_REG_CTB + 0x00000A00)	/* TRX3-FPGA�� SVI/F RX Data4�\��						*/
#define	D_RRH_REG_CTB_TRX3SVIFRXD3				(D_RRH_REG_CTB + 0x00000A04)	/* TRX3-FPGA�� SVI/F RX Data3�\��						*/
#define	D_RRH_REG_CTB_TRX3SVIFRXD2				(D_RRH_REG_CTB + 0x00000A08)	/* TRX3-FPGA�� SVI/F RX Data2�\��						*/
#define	D_RRH_REG_CTB_TRX3SVIFRXD1				(D_RRH_REG_CTB + 0x00000A0C)	/* TRX3-FPGA�� SVI/F RX Data1�\��						*/
#define	D_RRH_REG_CTB_TRX4SVIFTXD4				(D_RRH_REG_CTB + 0x00000A10)	/* TRX4-FPGA�� SVI/F TX Data4�ݒ�						*/
#define	D_RRH_REG_CTB_TRX4SVIFTXD3				(D_RRH_REG_CTB + 0x00000A14)	/* TRX4-FPGA�� SVI/F TX Data3�ݒ�						*/
#define	D_RRH_REG_CTB_TRX4SVIFTXD2				(D_RRH_REG_CTB + 0x00000A18)	/* TRX4-FPGA�� SVI/F TX Data2�ݒ�						*/
#define	D_RRH_REG_CTB_TRX4SVIFTXD1				(D_RRH_REG_CTB + 0x00000A1C)	/* TRX4-FPGA�� SVI/F TX Data1�ݒ�						*/
#define	D_RRH_REG_CTB_TRX4SVIFRXD4				(D_RRH_REG_CTB + 0x00000A20)	/* TRX4-FPGA�� SVI/F RX Data4�\��						*/
#define	D_RRH_REG_CTB_TRX4SVIFRXD3				(D_RRH_REG_CTB + 0x00000A24)	/* TRX4-FPGA�� SVI/F RX Data3�\��						*/
#define	D_RRH_REG_CTB_TRX4SVIFRXD2				(D_RRH_REG_CTB + 0x00000A28)	/* TRX4-FPGA�� SVI/F RX Data2�\��						*/
#define	D_RRH_REG_CTB_TRX4SVIFRXD1				(D_RRH_REG_CTB + 0x00000A2C)	/* TRX4-FPGA�� SVI/F RX Data1�\��						*/
#define	D_RRH_REG_CTB_ICNOTDONE					(D_RRH_REG_CTB + 0x00000B00)	/* InterConnect SerDes Not Done							*/
#define	D_RRH_REG_CTB_ICNOTDONEM				(D_RRH_REG_CTB + 0x00000B04)	/* InterConnect SerDes Not Done Mask					*/
#define	D_RRH_REG_CTB_ICTXERR					(D_RRH_REG_CTB + 0x00000B10)	/* InterConnect TX Error								*/
#define	D_RRH_REG_CTB_ICTXMSK					(D_RRH_REG_CTB + 0x00000B14)	/* InterConnect TX Error Mask							*/
#define	D_RRH_REG_CTB_ICRXERR1					(D_RRH_REG_CTB + 0x00000B20)	/* InterConnect RX Error								*/
#define	D_RRH_REG_CTB_ICRXMSK1					(D_RRH_REG_CTB + 0x00000B24)	/* InterConnect RX Error Mask							*/
#define	D_RRH_REG_CTB_ICRXERR2					(D_RRH_REG_CTB + 0x00000B30)	/* InterConnect RX Error2								*/
#define	D_RRH_REG_CTB_ICRXMSK2					(D_RRH_REG_CTB + 0x00000B34)	/* InterConnect RX Error2 Mask							*/
#define	D_RRH_REG_CTB_DTGSR						(D_RRH_REG_CTB + 0x00000E00)	/* DataGen����											*/
#define	D_RRH_REG_CTB_RDYCHK					(D_RRH_REG_CTB + 0x00000F00)	/* [DBG] HPM0�nBus Error�\��(Ready check)				*/
#define	D_RRH_REG_CTB_LEDCTL					(D_RRH_REG_CTB + 0x00000F10)	/* [DBG] Debug LED����									*/
#define	D_RRH_REG_CTB_SYSPLLRST					(D_RRH_REG_CTB + 0x00000F14)	/* [DBG] Sys PLL Reset�ʐ���							*/
#define	D_RRH_REG_CTB_SYSLOGRST					(D_RRH_REG_CTB + 0x00000F18)	/* [DBG] Sys Logic Reset�ʐ���						*/
#define	D_RRH_REG_CTB_ETHLOGRST					(D_RRH_REG_CTB + 0x00000F1C)	/* [DBG] ETH Logic Reset�ʐ���						*/
#define	D_RRH_REG_CTB_ICDBG						(D_RRH_REG_CTB + 0x00000F20)	/* [DBG] InterConnect Debug								*/
#define	D_RRH_REG_CTB_ICTEST					(D_RRH_REG_CTB + 0x00000F24)	/* [DBG] InterConnect Test								*/
#define	D_RRH_REG_CTB_ICRSTSTA1					(D_RRH_REG_CTB + 0x00000F30)	/* [DBG] InterConnect Reset State (InterConnect CH  1-  4 Reset State)	*/
#define	D_RRH_REG_CTB_ICRSTSTA2					(D_RRH_REG_CTB + 0x00000F34)	/* [DBG] InterConnect Reset State (InterConnect CH  5-  8 Reset State)	*/
#define	D_RRH_REG_CTB_ICRSTSTA3					(D_RRH_REG_CTB + 0x00000F38)	/* [DBG] InterConnect Reset State (InterConnect CH  9- 12 Reset State)	*/
#define	D_RRH_REG_CTB_ICRSTSTA4					(D_RRH_REG_CTB + 0x00000F3C)	/* [DBG] InterConnect Reset State (InterConnect CH 13- 16 Reset State)	*/
#define	D_RRH_REG_CTB_ICPRBSLOCK				(D_RRH_REG_CTB + 0x00000F40)	/* [DBG] ICB Status (InterConnect PRBS Lock)			*/
#define	D_RRH_REG_CTB_ICDBGMON					(D_RRH_REG_CTB + 0x00000F44)	/* [DBG] ICB Status (InterConnect PRBS Lock)			*/
#define	D_RRH_REG_CTB_PLDSVIFTST				(D_RRH_REG_CTB + 0x00000F50)	/* [DBG] SVIF(CPLD��)�e�X�g								*/
#define	D_RRH_REG_CTB_TRX1SVIFTST				(D_RRH_REG_CTB + 0x00000F54)	/* [DBG] SVIF(TRX1��)�e�X�g								*/
#define	D_RRH_REG_CTB_TRX2SVIFTST				(D_RRH_REG_CTB + 0x00000F58)	/* [DBG] SVIF(TRX2��)�e�X�g								*/
#define	D_RRH_REG_CTB_TRX3SVIFTST				(D_RRH_REG_CTB + 0x00000F5C)	/* [DBG] SVIF(TRX3��)�e�X�g								*/
#define	D_RRH_REG_CTB_TRX4SVIFTST				(D_RRH_REG_CTB + 0x00000F60)	/* [DBG] SVIF(TRX4��)�e�X�g								*/
#define	D_RRH_REG_CTB_DIPSW						(D_RRH_REG_CTB + 0x00000F70)	/* [DBG] DIP-SW��ԕ\��									*/
#define	D_RRH_REG_CTB_IRQDN						(D_RRH_REG_CTB + 0x00000F80)	/* [DBG] F2PIRQ�ڑ��ݒ�									*/
#define	D_RRH_REG_CTB_IRQSTA					(D_RRH_REG_CTB + 0x00000F84)	/* [DBG] F2PIRQ����\��									*/
#define	D_RRH_REG_CTB_CFGDBG					(D_RRH_REG_CTB + 0x00000F90)	/* [DBG] TRX-FPGA Config Debug Status					*/
#define	D_RRH_REG_CTB_RSTM						(D_RRH_REG_CTB + 0x00000FA0)	/* Reset�v���}�X�N										*/
#define	D_RRH_REG_CTB_RSTWAIT					(D_RRH_REG_CTB + 0x00000FA4)	/* Reset���sWait���Ԑݒ�								*/
#define	D_RRH_REG_CTB_TESTJPLLALM				(D_RRH_REG_CTB + 0x00000FB0)	/* [DBG] EPLL Alarm�J�E���g�e�X�g���W�X�^				*/
#define	D_RRH_REG_CTB_DBGBRGACT					(D_RRH_REG_CTB + 0x00000FC0)	/* [DBG] Debug Bridge����								*/
#define	D_RRH_REG_CTB_CAP						(D_RRH_REG_CTB + 0x00000FD0)	/* [DBG] �L���v�`�����W�X�^								*/
#define	D_RRH_REG_CTB_CAPEXT					(D_RRH_REG_CTB + 0x00000FD4)	/* [DBG] �L���v�`�����W�X�^�g��							*/
#define	D_RRH_REG_CTB_CAPINT					(D_RRH_REG_CTB + 0x00000FD8)	/* [DBG] �L���v�`���Ԍ��ݒ�								*/
#define	D_RRH_REG_CTB_DBGVER					(D_RRH_REG_CTB + 0x00000FE0)	/* [DBG] �f�o�b�O�o�[�W����								*/
#define	D_RRH_REG_CTB_CTTEST					(D_RRH_REG_CTB + 0x00000FF0)	/* [DBG] �e�X�g											*/
#define	D_RRH_REG_CTB_SRSTREQ					(D_RRH_REG_CTB + 0x00000FF8)	/* System Reset(SRST) Req								*/
#define	D_RRH_REG_CTB_PORREQ					(D_RRH_REG_CTB + 0x00000FFC)	/* [DBG] Power oN Reset�v��								*/
/* @} */

/*!
 * @name CTB���W�X�^bit & Shift
 * @note CTB���W�X�^bit & Shift
 * @{
 */
/* FPGAVER (0xA0000000)	*/
#define D_RRH_REG_CTB_FPGAVER_VER_MASK			0xFFFF0000
#define D_RRH_REG_CTB_FPGAVER_REV_MASK			0x0000FFFF
#define D_RRH_REG_CTB_FPGAVER_VER_SHFT			16

/* RTC1 (0xA0000010)	*/
#define D_RRH_REG_CTB_RTC1_HOU_MASK				0xFF000000
#define D_RRH_REG_CTB_RTC1_MIN_MASK				0x00FF0000
#define D_RRH_REG_CTB_RTC1_SEC_MASK				0x0000FF00
#define D_RRH_REG_CTB_RTC1_OHS_MASK				0x000000FF
#define D_RRH_REG_CTB_RTC1_HOU_SHFT				24
#define D_RRH_REG_CTB_RTC1_MIN_SHFT				16
#define D_RRH_REG_CTB_RTC1_SEC_SHFT				8

/* RTC2 (0xA0000014)	*/
#define D_RRH_REG_CTB_RTC2_YER_MASK				0xFFFF0000
#define D_RRH_REG_CTB_RTC2_MON_MASK				0x0000FF00
#define D_RRH_REG_CTB_RTC2_DAY_MASK				0x000000FF
#define D_RRH_REG_CTB_RTC2_YER_SHFT				16
#define D_RRH_REG_CTB_RTC2_MON_SHFT				8

/* RFICSPICS (0xA0000500)	*/
#define D_RRH_REG_CTB_RFICSPICS_RFIC1			0x00000001
#define D_RRH_REG_CTB_RFICSPICS_RFIC2			0x00000002
#define D_RRH_REG_CTB_RFICSPICS_RFIC3			0x00000004
#define D_RRH_REG_CTB_RFICSPICS_RFIC4			0x00000008

/* I2CSEL (0xA0000580)	*/
#define D_RRH_REG_CTB_I2CSEL1_SFP1				0x00000001
#define D_RRH_REG_CTB_I2CSEL2_SFP2				0x00000002

/* SYSLED (0xA0000800) */
#define D_RRH_REG_CTB_SYSLED_BSY_OFF			0xFFFFF8FF	/* Light Off	*/
#define D_RRH_REG_CTB_SYSLED_BSY_SB				0xFFFFF9FF	/* Slow Blink	*/
#define D_RRH_REG_CTB_SYSLED_BSY_FB				0xFFFFFAFF	/* Fast Blink	*/
#define D_RRH_REG_CTB_SYSLED_BSY_ON				0xFFFFFBFF	/*Light On		*/

#define D_RRH_REG_CTB_SYSLED_ALM_OFF			0xFFFFFF8F	/* Light Off	*/
#define D_RRH_REG_CTB_SYSLED_ALM_SB				0xFFFFFF9F	/* Slow Blink	*/
#define D_RRH_REG_CTB_SYSLED_ALM_FB				0xFFFFFFAF	/* Fast Blink	*/
#define D_RRH_REG_CTB_SYSLED_ALM_ON				0xFFFFFFBF	/*Light On		*/

#define D_RRH_REG_CTB_SYSLED_ACT_OFF			0xFFFFFFF8	/* Light Off	*/
#define D_RRH_REG_CTB_SYSLED_ACT_SB				0xFFFFFFF9	/* Slow Blink	*/
#define D_RRH_REG_CTB_SYSLED_ACT_FB				0xFFFFFFFA	/* Fast Blink	*/
#define D_RRH_REG_CTB_SYSLED_ACT_ON				0xFFFFFFFB	/*Light On		*/
/* @} */
#endif
/* @} */
