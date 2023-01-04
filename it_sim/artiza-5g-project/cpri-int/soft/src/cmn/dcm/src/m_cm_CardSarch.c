/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_CardSarch.c
 *  @brief  Card search.
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02866:[�i�����P]RE�J�[�h�f�f�v����M���̃X���b�g�ԍ��`�F�b�N����
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/***************************************************************************/

/** @addtogroup RRH_DCM
 *  @{
 */

/*************************************************************************** */
/*		define(s)/include(s)												 */
/*************************************************************************** */
#include "m_cm_header.h"

/**************************************************************************/
/**
 *  @brief  card search.
 *  @note   �J�[�h��������. \n
 *          �X���b�g�ԍ�������RE�\�����l�����A���̃X���b�g�ԍ��� \n
 *          �ǂ̃J�[�h������������֐� \n
 *  @param  slot_no	[in]	slot number
 *  @param  file	[in]	calling fucntion file name
 *  @param  line	[in]	calling function executing line
 *  @return RETURN CODE
 *  @retval CMD_CARDSARCH_R_BRE		0: 			BRE card
 *  @retval CMD_CARDSARCH_R_RRE		1: 			RRE card
 *  @retval CMD_CARDSARCH_R_LRE		2: 			LRE card
 *  @retval CMD_RES_NG				0xF0000001: Abnormal end
 *  @date   2008/07/22		FFCS)Tang	new create
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02866:[�i�����P]RE�J�[�h�f�f�v����M���̃X���b�g�ԍ��`�F�b�N����
 */
/**************************************************************************/

UINT m_cmR_CardSarch(UINT slot_no, VOID *file, UINT line)
{
	/* �߂�l������  */
	UINT rtn = CMD_RES_OK;
		
	if (cmw_re_dev_tbl.dev_kind == CMD_DEVKND_RRE)
	{
		if (slot_no == CMD_SLTNO_01)
		{
				/* RRE(1)��ݒ�		 */
			rtn = CMD_CARDSARCH_R_RRE;
		}
		else
		{
			/* ------------------------  */
			/* �X���b�g�ԍ��͈͊O		 */
			/* ------------------------  */
			/* ������				 */
			cm_Assert(CMD_ASL_USELOW, line, file) ;
			rtn = CMD_RES_NG ;
		} /* slot number judgement END	 */
			
		return rtn;
	}

	/* ������				 */
	cm_Assert(CMD_ASL_USELOW, (UINT)((cmw_re_dev_tbl.dev_kind<<16)+slot_no), "m_cmR_CardSarch NG") ;
	rtn = CMD_RES_NG ;

	return rtn;
}
/* @} */
