/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_InitReq.c
 * @brief		pf_dhcp�X���b�h ����������
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
 * @brief		f_dhcp_InitReq
 * @note		pf_dhcp�X���b�h�̏������������s��
 *				-# pf_dhcp�X���b�h�����e�[�u��������
 *				-# pf_main�ɏ����������ʒm(Thread Init completion notice)�𑗐M����(f_com_msgQSend())
 *				-# �X���b�h��Ԃ�"�^�p���"�ɑJ�ڂ�����
 * @param		*msgP			[in]VOID	��M���b�Z�[�W
 * @retval		D_SYS_OK		0:����I��
 * @retval		D_SYS_NG		1:�ُ�I��
 * @return		INT				���s����
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_dhcp_InitReq( VOID	*msgP )
{
	T_SYS_INIT_COMP_NTC				*sendMsg;							/* ���Mmsg�|�C���^		*/
	INT								errcd;								/* �ڍ�NG�R�[�h			*/
	INT								ret;								/* �Ԃ�l				*/


	/********************************************************************************************/
	/* �����������ʒm ���M																		*/
	/********************************************************************************************/
	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_SYS_INIT_COMP_NTC),	/* Size					*/
										(VOID **)&sendMsg,				/* msgP					*/
										&errcd );

	/* �������ʃ`�F�b�N	*/
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
		return D_SYS_NG;
	}

	/* �����������ʒmmsg�ҏW	*/
	sendMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;		/* �����������ʒm			*/
	sendMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* ���M��Process Queue ID	*/
	sendMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;				/* ���M��Thread Queue ID	*/
	sendMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* ���M��Process Queue ID	*/
	sendMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_DHC;				/* ���M��Thread Queue ID	*/
	sendMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);		/* Length(Header�� + Data��)*/

	/* pf_main�֏����������ʒm�𑗐M	*/
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,			/* ���M��Thread Queue ID	*/
									(void *)sendMsg );				/* ���Mmsg					*/

	/* �������ʃ`�F�b�N	*/
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", ret);
		return D_SYS_NG;
	}

	/* Thread��ԑJ��(�^�p���)	*/
	f_dhcpw_thdInfo.thdState  = D_DHCP_THD_ACT;

	return D_SYS_OK;
}

/* @} */
