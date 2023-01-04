/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_CheckSumCalcCpri.c
 *  @brief  common function - Checksum calculation for CPRI signal function
 *  @date   2008/07/18 FFCS)Tangj create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/***************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */

/****************************************************************************/
/* �w�b�_�C���N���[�h                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRIF/MDIF���ʃw�b�_�t�@�C��			*/

/**************************************************************************/
/**
 *  @brief  �`�F�b�N�T���v�Z����(CPRI�M���p) 
 *  @note   �Y���̈�̉��Z���s���T���l��ԋp����B  \n
 *          �A�h���X�A�f�[�^��莞�̓A�{�[�g���������{���� \n
 *          ���擪�A�h���X�^�T�C�Y�͂S�o�C�g�o�E���_���� \n
 *          �l�������l�Ƃ��邱�ƁB \n
 * 
 *          (1) ���̓p�����[�^�`�F�b�N \n
 *          (2) �T���l�Z�o \n
 *
 *  @param  chkSumAdr_p			[in]	�`�F�b�N�T���擪�A�h���X
 *  @param  dataLen				[in]	�`�F�b�N�T���T�C�Y
 *  @param  chkSum_p			[out]	�`�F�b�N�T���l
 *  @param  FileNamep			[in]	calling function file name
 *  @param  LineNo				[in]	calling function executing line
 *  @return None
 *  @date   2006/02/07	��)T.Saito	 �V�K�쐬	WCDMA119-0476�Ώ���
 *  @date   2006/03/22	��)A.Etou	 WCDMA119-1099�Ή�
 *  @date   2007/02/19	FJT)H.Sujihara	 WCDMA130-106�Ή�
 *									�P�̕␔�a�v�Z�����C�� 
 *  @date   2009/11/10	QNET)Kabasima M-S3G-eNBSYS-00764
 *									�f�[�^��������̃o�C���f�B���O(0x00�t��)�����ǉ�
 */
/****************************************************************************/

VOID cmR_CheckSumCalcCpri( const VOID*	chkSumAdr_p,
						   UINT			dataLen,
						   USHORT*		chkSum_p,
						   VOID*		FileNamep,
						   UINT			LineNo )
{
	UINT	a_loopnum;      			/* �`�F�b�N�T���Z�o���[�v�� 	*/
	UINT	a_cnt;						/* ���[�v�J�E���^				*/
	USHORT*	a_work_p;					/* �A�h���X���[�N�̈�			*/
	UINT    a_chksum_value  = CMD_NUM0; /* CheckSUM�l�v�Z�i�[�p         */
	USHORT  a_chksum_value2 = CMD_NUM0; /* CheckSUM�l�v�Z�i�[�p         */
	USHORT  temp = CMD_NUM0;
	
	/* input parameter ( 0 Address) check */
	if(((INT)chkSumAdr_p == CMD_NUM0)||((INT)chkSum_p == CMD_NUM0))
	{
		return;

	}

	/* ===================================================================== */
	/* ���T���l�v�Z����                                                      */
	/* ===================================================================== */
	a_work_p = (USHORT*)chkSumAdr_p;
	a_loopnum = dataLen / sizeof(USHORT) ;
    
	for( a_cnt=CMD_NUM0 ; a_cnt<a_loopnum ; a_cnt++ )
	{
		/* �|�C���^�����Z���A�u���b�N�P�ʂɃf�[�^�����Z */
		/* �t�@�C���f�[�^��16bits����1�̕␔�a�����B 	*/
		a_chksum_value = a_chksum_value + *(a_work_p + a_cnt);

		if(a_chksum_value > CMD_CHECKSUM_VALUE)
		{
			/* ���オ�肵���ꍇ */
			a_chksum_value2 = (USHORT)a_chksum_value;
			/* �ŉ��ʃr�b�g��+1 */
			a_chksum_value2 ++;
			a_chksum_value = (UINT)a_chksum_value2;
	
			if(a_chksum_value == CMD_NUM0)
			{
				/* �ŉ��ʃr�b�g��+1�������̂��X�Ɍ��オ�肵���ꍇ */
				a_chksum_value ++;
			}
		}
	}
	
	/* �f�[�^������ł���Ƃ� */
	if((dataLen % CMD_NUM2) == CMD_NUM1)
	{
		/* �`�F�b�N�T���l�v�Z */
		a_chksum_value = 
					a_chksum_value + (USHORT)((*(a_work_p + a_loopnum)) & CMD_MSK_00FF);

		if(a_chksum_value > CMD_CHECKSUM_VALUE)
		{
			/* ���オ�肵���ꍇ */
			a_chksum_value2 = (USHORT)a_chksum_value;
			/* �ŉ��ʃr�b�g��+1 */
			a_chksum_value2 ++;
			a_chksum_value = (UINT)a_chksum_value2;
	
			if(a_chksum_value == CMD_NUM0)
			{
				/* �ŉ��ʃr�b�g��+1�������̂��X�Ɍ��オ�肵���ꍇ */
				a_chksum_value ++;
			}
		}
	}
	
	/* ===================================================================== */
	/* ��CheckSum�l���o�̓p�����[�^�ɐݒ�                                    */
	/* ===================================================================== */
	/* ����ɏ�L�v�Z���ʂ�1�̕␔�����A������`�F�b�N�T���Ƃ��� */
	*chkSum_p = ~(USHORT)a_chksum_value ;

	/* swap the checksum */
	temp = *chkSum_p;
	*chkSum_p = ((temp & 0xff00) >> 8) | ((temp & 0x00ff) << 8);

	return;
}
/* @} */
