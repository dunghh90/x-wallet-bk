/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eqs_InitReq.c
 *  @brief  task init function
 *  @date   2018/08/21 KCN)Kamimoto create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-
 */
/*********************************************************************************/

/** @addtogroup TRA_RSV
* @{ */
#include "f_eqs_header.h"			/* 5GDU-LLS�Ď��^�X�N�w�b�_�t�@�C��			*/

/* @{ */
/**
* @brief task init function
* @note Init table and send init response.\n
* @param -
* @return None.
* @date 2018/08/21 KCN)Kamimoto create
* @warning	N/A
* @FeatureID	5GDU-002-004-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_InitReq( VOID )
{
	INT		errcd;
	INT		rtn_bpf;
	T_SYS_INIT_COMP_NTC*	sndMsg;			/* ���M���b�Z�[�W(�����������ʒm)	*/
	UINT					loop_cnt, loop ;
	CHAR					wStr[2];
	CHAR					tmp_para[D_PARAM_NUM_MAX] ;
	CHAR					mfg_name[16] = "FUJITSU LIMITED";

	/*******************************************************************/
	/* ������                                                          */
	/*******************************************************************/

	/* ���O�J�E���^�e�[�u��������������	*/
	memset( &f_eqsw_logcnt, D_RRH_ZERO, sizeof( f_eqsw_logcnt ));
	
	/* ���O�擾�e�[�u��������������	*/
	memset( f_eqsw_log_tbl, D_RRH_ZERO, sizeof( f_eqsw_log_tbl ) );

	/* sv new status table init */
	memset( &f_eqsw_tra_sv_newsta, D_RRH_ZERO, sizeof( f_eqsw_tra_sv_newsta ));

	memset( f_eqsw_almCount, D_RRH_ZERO, sizeof( f_eqsw_almCount ));
	memset( f_eqsw_wvstop, D_RRH_ZERO , sizeof( f_eqsw_wvstop ));
	

	/* �A���[���v���ݒ�f�[�^�̏����� */
	memset( f_eqs_fm_alrm_data , D_RRH_ZERO , sizeof( f_eqs_fm_alrm_data ));
	/* Init TRA card restart both alm flag */
	f_eqsw_restart_both_alm_flg = D_SYS_OFF;

	/* Notification�ʒm��Ԃ̏����� */
	f_eqs_alm_start_data	= D_EQS_NTF_START_INIT ;
	f_eqs_status_start_data	= D_EQS_NTF_START_INIT ;
	f_eqs_alm_cnt			= D_EQS_FM_ALM_NON ;
	
	/********************************/
	/*	ietfHardware���b�Z�[�W�쐬	*/
	/********************************/
	/*	���b�Z�[�W�쐬�̈�̏�����	*/
	memset( &f_eqs_ietfHwMsg, D_RRH_ZERO, sizeof( f_eqs_ietfHwMsg ));
	
	/* physical-index�̐ݒ� */
	f_eqs_ietfHwMsg.msi_ietf_component.physical_index   = D_EQS_PHYSICAL_IDX ;

	/* �n�[�h�o�[�W�����̓Ǐo�� */
	snprintf( tmp_para , sizeof(tmp_para) , "%04x" , (USHORT)f_cmw_tra_inv_tbl->cmx_hver1) ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[0] = tmp_para[2] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[1] = tmp_para[3] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[2] = tmp_para[0] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[3] = tmp_para[1] ;

	/* �\�t�g�o�[�W�����̓Ǐo�� */
	/* �N���ʂ̃`�F�b�N */
	if ( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 )
	{
		/* 0�ʂȂ� */
		f_eqs_ietfHwMsg.msi_ietf_component.software_rev = f_cmw_tra_inv_tbl->cmx_f0_sver[0] ;
	}
	else
	{
		/* 1�ʂȂ� */
		f_eqs_ietfHwMsg.msi_ietf_component.software_rev = f_cmw_tra_inv_tbl->cmx_f1_sver[0] ;
	}

	/* �V���A���ԍ��̓Ǐo�� */
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[0] = ((f_cmw_tra_inv_tbl->cmx_pcbno2>>8) & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[1] = (f_cmw_tra_inv_tbl->cmx_pcbno2 & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[2] = ((f_cmw_tra_inv_tbl->cmx_pcbno3>>8) & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[3] = (f_cmw_tra_inv_tbl->cmx_pcbno3 & 0xff);

	/* ��Ж��ݒ�(mfg_name) */
	strcpy( f_eqs_ietfHwMsg.msi_ietf_component.mfg_name, mfg_name );

	/* �J�[�h�}��(model_name)�̓Ǐo�� */
	for(loop_cnt=0; loop_cnt<(sizeof(f_cmw_tra_inv_tbl->cmx_mno)/sizeof(f_cmw_tra_inv_tbl->cmx_mno[0])); loop_cnt++)
	{
		/*	2�o�C�g�f�[�^����1�o�C�g�����o��	*/
		wStr[0] = ((f_cmw_tra_inv_tbl->cmx_mno[loop_cnt] >> 8) & 0xff);
		wStr[1] = (f_cmw_tra_inv_tbl->cmx_mno[loop_cnt] & 0xff);
		for(loop=0; loop<sizeof(wStr); loop++)
		{
			f_eqs_ietfHwMsg.msi_ietf_component.model_name[loop_cnt*sizeof(wStr)+loop] = wStr[loop];
		}
	}

	/* �������̓Ǐo�� */
	/* BCD-HEX�ϊ� */
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.year = (((f_cmw_tra_inv_tbl->cmx_testyer>>12) & 0xf) * 1000)
			 + (((f_cmw_tra_inv_tbl->cmx_testyer>>8) & 0xf) * 100)
			 + (((f_cmw_tra_inv_tbl->cmx_testyer>>4) & 0xf) * 10)
			 + (f_cmw_tra_inv_tbl->cmx_testyer & 0xf);

	/* ������BCD-HEX�ϊ� */
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.month =((((f_cmw_tra_inv_tbl->cmx_testday>>12) & 0xf) * 10) + ((f_cmw_tra_inv_tbl->cmx_testday>>8) & 0xf));
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.day =((((f_cmw_tra_inv_tbl->cmx_testday>>4) & 0xf) * 10) + (f_cmw_tra_inv_tbl->cmx_testday & 0xf));
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.hour	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.min	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.sec	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.dummy	= D_RRH_ZERO ;

	/* ���̃_���v */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "ietfHardware:HV=%4s,SV=%04x,sNo=%02x%02x%02x%02x",
																			f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev,
																			f_eqs_ietfHwMsg.msi_ietf_component.software_rev,
																			f_eqs_ietfHwMsg.msi_ietf_component.serial_num[0],
																			f_eqs_ietfHwMsg.msi_ietf_component.serial_num[1],
																			f_eqs_ietfHwMsg.msi_ietf_component.serial_num[2],
																			f_eqs_ietfHwMsg.msi_ietf_component.serial_num[3]);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "mfg+mdl=%24s,%04d/%02d/%02d", f_eqs_ietfHwMsg.msi_ietf_component.mfg_name, 
																			f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.year ,
																			f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.month ,
																			f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.day );
	
	/* SV������̃f�t�H���g�ݒ� */
	f_com_SVCTL_setDefault(D_DU_SVINF_ALL);

	/****************************************************************************************************************/
	/* ��ԕω��̓A���e�i�L�����A��Ԃł��肱���ł͎g�p���Ȃ�                                                       */
	/* Set INS bit ON 																								*/
	/* f_com_SVCtl(E_DU_SVCTL_OPE_USA, D_DU_SVCTL_CHG, 2); 															*/
	/****************************************************************************************************************/

	/****************************************************************************************************************/
	/* �����������ʒm���M																							*/
	/****************************************************************************************************************/
	/* �o�b�t�@�擾 */
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer���						*/
											sizeof(T_SYS_INIT_COMP_NTC),		/* MessageSize						*/
											(VOID **)&sndMsg,					/* MessageBuffer					*/
											&errcd );							/* ErrorCode						*/
	
	if( rtn_bpf != BPF_RU_IPCM_OK  )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET error=%08x", rtn_bpf );
	}

	/* ���MMSG�쐬 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* ���b�Z�[�WID(0x00000102)			*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* �ԐM��PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* ���M��TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* ���M��PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_EQS;							/* ���M��TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* �f�[�^���o�C�g��					*/
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,								/* ���MMSG�L���[ID					*/
							(VOID *)sndMsg );	
	
	/* Update task state to all  task init complete wait state */
	f_eqsw_thrdstate = D_RSV_TSK_STATE_USE;
	/* Update Last-Change Time */
	f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&f_eqsw_last_chg) ;
	return ;
}
/* @} */

/* @} */
