/*!
 * @skip 		$ld:$
 * @file 		f_fhs_InitReq.c
 * @brief		pf_fhs�X���b�h ����������
 * @date		2018/02/19 FJT)H.Yoshida Create
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
 * @brief		f_fhs_InitReq
 * @note		pf_fhs�X���b�h�̏������������s��
 *				-# pf_fhs�X���b�h�����e�[�u��������
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
INT	f_fhs_InitReq( VOID	*msgP )
{
	T_SYS_INIT_COMP_NTC				*sendMsg;							/* ���Mmsg�|�C���^		*/

	INT								i;
	INT								errcd;								/* �ڍ�NG�R�[�h			*/
	INT								ret;								/* �Ԃ�l				*/

	char							cmd_str[D_FHS_USER_CHG_CMD_LENGTH];
	INT								cmd_rtn;


	/********************************************************************************************/
	/* �p�����[�^�����ݒ�																		*/
	/********************************************************************************************/
	/*** fhs thread�N�����Ԑݒ�  ***/
	f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&f_fhsw_startup_time );

	/********************************************************************************************/
	/* NETCONF���[�U�[�A�J�E���g�o�^															*/
	/********************************************************************************************/
	/* ���[�U�[�o�^�L���`�F�b�N	*/
	if( f_comw_netconf_tbl->num_of_use != 0 ) {

		for( i = 0; i < f_comw_netconf_tbl->num_of_use; i++ ) {

			/* ======================================================== */
			/* Add to user of NETCONF user Information					*/
			/* ======================================================== */
			memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
			sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_ADD, f_comw_netconf_tbl->info[i].user );
			cmd_rtn = system ( cmd_str );
			if( cmd_rtn !=  D_RRH_OK ) {

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
			}
			/* ======================================================== */
			/* Set to password fo new user of NETCONF user Information	*/
			/* ======================================================== */
			memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
			sprintf( cmd_str, D_FHS_USER_CHG_CMD_PASS_CHG, f_comw_netconf_tbl->info[i].user, f_comw_netconf_tbl->info[i].password );
			cmd_rtn = system ( cmd_str );
			if( cmd_rtn !=  D_RRH_OK ) {

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
			}
			/* ================================================================ */
			/* Set to enabed/disabled fo new user of NETCONF user Information	*/
			/* ================================================================ */
			if( f_comw_netconf_tbl->info[i].enabled == 0 ) {	/* disable	*/

				/* LINUX�R�}���h���s (�A�J�E���g�����ύX:disable)	*/
				memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_DISABLE, f_comw_netconf_tbl->info[i].user );
				cmd_rtn = system ( cmd_str );

			}
			if( cmd_rtn !=  D_RRH_OK ) {

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
			}
		}
	}

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
	sendMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_FHS;				/* ���M��Thread Queue ID	*/
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
	f_fhsw_thdInfo.thdState  = D_FHS_THD_ACT;

	return D_SYS_OK;
}

/* @} */
