/*!
 * @skip 		$ld:$
 * @file 		f_fhs_abort.c
 * @brief		pf_fhs�X���b�h Abort����
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_FHS
 * @{
 */

#include "f_fhs_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_fhs_abort
 * @note		Abort���������s����B
 *		-# Abort���������{�B				f_com_abort() �֐�Call
 *		-# �Œ�Ő������ʂɕԋp����B
 * @param		*rcvMsg_p	[in]	��M���b�Z�[�W
 * @return		INT
 * @retval		D_SYS_OK		0:����I��
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_fhs_abort(VOID	*msgP)
{
	INT								rtn = D_SYS_OK;						/* ���A�l				*/

	/* Assert���O�o��	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 						/* Assert���x��			*/
						"Matrix Function Call NG, sta=%d, event=%08x",	/* message				*/
						f_fhsw_thdInfo.thdState,						/* Information			*/
						((T_SYS_COMMON_THDIF *)msgP)->uiEventNo );		/* Information			*/

	/* Abort���� : �^�X�N�ԃ��b�Z�[�W�ُ�	*/
	f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TIF );
	return	rtn;
}

/* @} */
