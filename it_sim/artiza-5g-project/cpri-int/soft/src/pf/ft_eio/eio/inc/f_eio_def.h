/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_eio_def.h
 *  @brief  5GDU-LLS external IO supervision & cntrol defination
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* ��d�捞�ݖh�~ */
#ifndef		F_EIO_DEF
#define		F_EIO_DEF

/** @addtogroup RRH_PF_EIO
* @{ */

/* ======================================================================== */
/* ��Ԓ�`																	*/
/* ======================================================================== */
enum
{
	E_EIO_STAT_INI = 0,									/* init						*/
	E_EIO_STAT_RUN,										/* running					*/
	E_EIO_STAT_MAX,										/* Status Number			*/
};

/* ======================================================================== */
/* �C�x���g��`																*/
/* ======================================================================== */
enum
{
	E_EIO_EVT_INIT = 0,									/* Thread Create								*/
	E_EIO_EVT_EXTIOSV_TIMEOUT,							/* External IO SV timer timeout indication		*/
	E_EIO_EVT_FANSVCTRL_TIMEOUT,						/* FAN SV & Cntrol timer timeout indication		*/
	E_EIO_EVT_API_FANDBG_IND,							/* FAN debug mode indication					*/
	E_EIO_EVT_API_FANDAC_IND,							/* FAN DAC chage indication						*/
	E_EIO_EVT_MSI_MSGRCV_NTF,							/* MSI message receive notify					*/
	E_EIO_EVT_MAX,										/* Event Number									*/
};

/*	�^�C�}�l	*/
#define D_EIO_EXTOUTSV_TIMVAL			100				/* �O��IO���͏�ԊĎ�����(100ms)				*/
#define D_EIO_FANSVCTRL_TIMVAL			100				/* FAN�Ď����䎞��(100ms)						*/


/* ======================================================================== */
/* �O��IO�Ď������`														*/
/* ======================================================================== */
#define D_EIO_EXTIO_OPEN				0				/*	open(OFF)									*/
#define D_EIO_EXTIO_SHORT				1				/*	short(ON)									*/
#define D_EIO_EXTIO_INVALID				0xffffffff		/*	invalid(����)								*/
#define D_EIO_EXTIO_PORT0				0				/*	Port�ԍ�:0									*/

/* ======================================================================== */
/* FAN�Ď������`															*/
/* ======================================================================== */

#define D_EIO_TONUM_1S					10				/* 1�b�Ԃ̃^�C���A�E�g��						*/

#define D_EIO_MAXSTATIM_MIN				0				/* FAN MAX�p���ŏ���							*/
#define D_EIO_MAXSTATIM_MAX				0				/* FAN MAX�p���ő��							*//*	�n�\v1.05�ύX(3->0)	*/

/*	FAN �Ď��֘A	*/
#define D_EIO_PROTECTCNT_FANMOUNT		5				/* FAN����Normal�ی�i��						*/
#define D_EIO_PROTECTCNT_FANUNMOUNT		5				/* FAN������Fault�ی�i��						*/

#define D_EIO_PROTECTCNT_FANPSFLT		3				/* FAN�d��Fault�ی�i��							*/
#define D_EIO_PROTECTCNT_FANPSNOR		3				/* FAN�d��Normal�ی�i��						*/

#define D_EIO_PROTECTCNT_FANSPEEDFLT	10				/* FAN��]��Fault�ی�i��						*/
#define D_EIO_PROTECTCNT_FANSPEEDNOR	10				/* FAN��]��Normal�ی�i��						*/
#define D_EIO_PROTECTCNT_FANRESET_FLT	60				/* FAN��]��FaultReset�ی�i��					*/
#define D_EIO_PROTECTCNT_FANRESET_NOR	5				/* FAN��]��FaultReset�ی�i��					*/

#define D_EIO_THRESHOLD_FANSPEED		D_DU_THRESHOLD_FANSPEED	/* FAN��]��Fault臒l	*/

#define D_EIO_FANSPEED_MASK_TEMP_SUB6	650				/* FAN��]���ቺ�}�X�N�p�Z���T���x臒l(sub6)	*/
#define D_EIO_FANSPEED_MASK_TEMP_MMW	600				/* FAN��]���ቺ�}�X�N�p�Z���T���x臒l(mmw)		*/

/*	FAN ����֘A	*/
#define D_EIO_SPINMODE_LOW_SPEED		0xFFF			/* FAN��]�����[�h�F���]���[�h				*/
#define D_EIO_SPINMODE_NML_SPEED		0x000			/* FAN��]�����[�h�F�ʏ��]���[�h				*/

/*	FAN Unit�Ď�����	*/
enum
{
	E_EIO_FANITEM_MOUNT = 0,
	E_EIO_FANITEM_SPSTS,
	E_EIO_FANITEM_MAXNO
};

/*	FAN GET�֘A	*/
#define D_EIO_FANSPEED_MAX				D_DU_FANSPEED_MAX		/* FAN MAX��]��(3810[rpm])(TBD)				*/
#define D_EIO_FANSPEED_CALRPM			D_DU_FANSPEED_CALRPM	/* FAN��]��rpm�ϊ�								*/

/* @} */

#endif	/* F_EIO_DEF */

