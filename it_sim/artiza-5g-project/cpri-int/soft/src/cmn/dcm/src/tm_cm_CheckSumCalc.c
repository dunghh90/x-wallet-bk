/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_CheckSumCalc.c
 *  @brief  common function - Checksum calculation function
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
#include "m_cm_header.h"			/* TRA���ʃw�b�_�t�@�C��			*/

/**************************************************************************/
/**
 *  @brief  Checksum calculation 
 *  @note   �Y���̈�̉��Z���s���T���l��ԋp����B  \n
 *          �A�h���X�A�f�[�^��莞�̓A�{�[�g���������{���� \n
 *          ���擪�A�h���X�^�T�C�Y�͂S�o�C�g�o�E���_���� \n
 *          �l�������l�Ƃ��邱�ƁB \n
 *          (1) ���̓p�����[�^�`�F�b�N \n
 *          (2) �T���l�Z�o \n
 *  @param  chkSumAdr_p			[in]	�`�F�b�N�T���擪�A�h���X
 *  @param  dataLen				[in]	�`�F�b�N�T���T�C�Y
 *  @param  chkSum_p			[out]	�`�F�b�N�T���l
 *  @param  FileNamep			[in]	calling function file name
 *  @param  LineNo				[in]	calling function executing line
 *  @return None
 *  @date   2005/12/12	��)M.Takeuchi	 �V�K�쐬
 *  @date   2006/03/30	��)M.Oohashi	 WCDMA119-1230
 */
/****************************************************************************/
VOID cmR_CheckSumCalc( const VOID*	chkSumAdr_p,
					   UINT			dataLen,
					   USHORT*		chkSum_p,
					   VOID*		FileNamep,
					   UINT			LineNo )
{
	UINT	a_loopnum;      			/* �`�F�b�N�T���Z�o���[�v�� 	*/
	UINT	a_cnt;						/* ���[�v�J�E���^				*/
	USHORT*	a_work_p;					/* �A�h���X���[�N�̈�			*/
	USHORT  a_chksum_value  = CMD_NUM0; /* CheckSUM�l�v�Z�i�[�p         */

	/* input parameter ( 0 Address) check */
	if((chkSumAdr_p == NULL) ||(chkSum_p == NULL))
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
		a_chksum_value = (USHORT)(a_chksum_value + *(a_work_p + a_cnt));
	}
	
	/* ===================================================================== */
	/* ��CheckSum�l���o�̓p�����[�^�ɐݒ�                                    */
	/* ===================================================================== */
	*chkSum_p = a_chksum_value ;

	/* �f�[�^������ł���Ƃ� */
	if((dataLen % CMD_NUM2) == CMD_NUM1)
	{
		/* �`�F�b�N�T���l�v�Z */
		(*chkSum_p) += *(UCHAR *)(a_work_p + a_loopnum);
	}

	return;
}
/* @} */
