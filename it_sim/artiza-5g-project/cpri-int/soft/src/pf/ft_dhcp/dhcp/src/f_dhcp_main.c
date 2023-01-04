/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_main.c
 * @brief		pf_dhcp�X���b�h ���C������
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dhcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_main
 * @note		pf_dhcp�X���b�h���C���B�X���b�h�N������MSG��M�҂���Loop���������{
 *				-# BPF�փX���b�h���o�^����
 *				-# DHCP�X���b�h��Ԃ�"�������"�ɐݒ肷��
 *				-# �������֐���Call����(f_dhcp_init())
 *				-# MSG�҂�����Loop�����ֈڍs����
 *					-# �X���b�h�ԒʐM�L���[��M�֐���Call����(BPF_RU_IPCM_MSGQ_RECV())
 *					-# ��M���b�Z�[�W��͊֐���Call����(f_dhcp_anlz())
 *					-# �ʐM�p���L�������̉��(BPF_RU_IPCM_PROCMSG_ADDRFREE())
 * @param		N/A
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID f_dhcp_main(void)
{
	VOID							*msgP;								/* ��Mmsg�|�C���^		*/
	UINT							rcvmsgsz;							/* ��Mmsg�T�C�Y		*/
	INT								ret;								/* �߂�l				*/

	/* �X���b�h���\��	*/
	prctl(PR_SET_NAME, "pf_dhcp", 0, 0, 0);

	/* BPF�փX���b�h���o�^	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,						/* ProcessID			*/
								 D_SYS_THDID_PF_DHC,					/* ThreadID				*//*��*/
								 D_SYS_THDQID_PF_DHC,					/* ThreadQueueID		*//*��*/
								 (unsigned long int)pthread_self());	/* pThreadID			*/

	/* �����������֐�Call	*/
	(VOID)f_dhcp_InitReq( NULL );
	
	/************************************************************************************************************************/
	/* ���b�Z�[�W��M�҂�																									*/
	/************************************************************************************************************************/
	while(1)
	{
		/* ���b�Z�[�W��M	*/
		ret = BPF_RU_IPCM_MSGQ_RECV(	D_SYS_THDQID_PF_DHC,			/* Thread Qid			*/
										0,								/* �����҂�����			*/
										(void **)&msgP,					/* ��Mmsg�|�C���^		*/
										&rcvmsgsz );					/* ��Mmsg�T�C�Y		*/

		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}

		/* ��M���b�Z�[�W��͊֐�Call	*/
		(VOID)f_dhcp_anlz((VOID*)msgP);

		/* �ʐM�p���L�������̉��	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)msgP );
		
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
	}
	return;
}

/* @} */
