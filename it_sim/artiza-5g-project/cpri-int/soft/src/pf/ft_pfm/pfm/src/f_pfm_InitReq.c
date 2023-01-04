/*!
 * @skip 		$ld:$
 * @file 		f_pfm_InitReq.c
 * @brief		pf_pfm�X���b�h ����������
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_PFM
 * @{
 */

#include "f_pfm_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_pfm_InitReq
 * @note		pf_pfm�X���b�h�̏������������s��
 *				-# pf_pfm�X���b�h�����e�[�u��������
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
INT	f_pfm_InitReq( VOID	*msgP )
{
	T_SYS_INIT_COMP_NTC				*sendMsg;							/* ���Mmsg�|�C���^		*/

	INT								endcd;								/* �Ԃ�l				*/
	INT								errcd;								/* �ڍ�NG�R�[�h			*/


	/********************************************************************************************/
	/* �p�����[�^�����ݒ�																		*/
	/********************************************************************************************/

	if ( M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_SUB6 ){
		f_pfmw_rxwin_reg_addr_offset_cc = 0;
		f_pfmw_rxwin_reg_addr_offset_ccnum = 1;
		f_pfmw_rxwin_reg_addr_offset_ant = 0x200;
		f_pfmw_rxwin_reg_addr_offset_antnum = D_RRH_ANT_MAX;
		memcpy(f_pfmw_rxwin_reg_addr,f_pfmw_rxwin_reg_addr_sub6,sizeof(f_pfmw_rxwin_reg_addr));

	}else{
		f_pfmw_rxwin_reg_addr_offset_cc = 0x100;
		f_pfmw_rxwin_reg_addr_offset_ccnum = D_RRH_CAR_MAX;
		f_pfmw_rxwin_reg_addr_offset_ant = 0x400;
		f_pfmw_rxwin_reg_addr_offset_antnum = D_RRH_ANT_MAX_MMW;
		memcpy(f_pfmw_rxwin_reg_addr,f_pfmw_rxwin_reg_addr_mmw,sizeof(f_pfmw_rxwin_reg_addr));

	}

	/********************************************************************************************/
	/* �����������ʒm ���M																		*/
	/********************************************************************************************/
	/* �ʐM�p���L���������擾����	*/
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET( E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										 sizeof(T_SYS_INIT_COMP_NTC),	/* Size					*/
										 (VOID **)&sendMsg,				/* msgP					*/
										 &errcd );
	/* �������ʃ`�F�b�N	*/
	if( endcd != D_SYS_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
		return D_SYS_NG;
	}

	/* �����������ʒmmsg�ҏW	*/
	sendMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;		/* �����������ʒm			*/
	sendMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* ���M��Process Queue ID	*/
	sendMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;				/* ���M��Thread Queue ID	*/
	sendMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* ���M��Process Queue ID	*/
	sendMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_PFM;				/* ���M��Thread Queue ID	*/
	sendMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);		/* Length(Header�� + Data��)*/

	/* pf_main�֏����������ʒm�𑗐M	*/
	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,			/* ���M��Thread Queue ID	*/
									(void *)sendMsg );				/* ���Mmsg					*/
	/* �������ʃ`�F�b�N	*/
	if( endcd != D_SYS_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", endcd);
		return D_SYS_NG;
	}


	endcd = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_PERIODIC,						/* Timer���						*/
									E_RRH_TIMID_PFM_LOGTIME,						/* TimerID							*/
									D_PFM_LOG_TIME_VAL,								/* Timer�l							*/
									D_SYS_MSGID_PFM_LOG_TIMEOUT_NTF,				/* TimeOut���ʒmEvent�ԍ�			*/
									D_SYS_THDQID_PF_PFM,							/* TimeOut�����M��Queue�ԍ�			*/
									&errcd );

	if(endcd != D_SYS_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "timer start error %d", errcd);
	}

	/* Thread��ԑJ��(�^�p���)	*/
	f_pfmw_thdInfo.thdState  = E_PFM_STATE_ACT;

	return D_SYS_OK;
}

/* @} */
