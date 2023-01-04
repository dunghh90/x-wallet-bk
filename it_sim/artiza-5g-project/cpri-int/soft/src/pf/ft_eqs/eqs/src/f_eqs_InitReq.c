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
#include "f_eqs_header.h"			/* 5GDU-LLS監視タスクヘッダファイル			*/

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
	T_SYS_INIT_COMP_NTC*	sndMsg;			/* 送信メッセージ(初期化完了通知)	*/
	UINT					loop_cnt, loop ;
	CHAR					wStr[2];
	CHAR					tmp_para[D_PARAM_NUM_MAX] ;
	CHAR					mfg_name[16] = "FUJITSU LIMITED";

	/*******************************************************************/
	/* 初期化                                                          */
	/*******************************************************************/

	/* ログカウンタテーブルを初期化する	*/
	memset( &f_eqsw_logcnt, D_RRH_ZERO, sizeof( f_eqsw_logcnt ));
	
	/* ログ取得テーブルを初期化する	*/
	memset( f_eqsw_log_tbl, D_RRH_ZERO, sizeof( f_eqsw_log_tbl ) );

	/* sv new status table init */
	memset( &f_eqsw_tra_sv_newsta, D_RRH_ZERO, sizeof( f_eqsw_tra_sv_newsta ));

	memset( f_eqsw_almCount, D_RRH_ZERO, sizeof( f_eqsw_almCount ));
	memset( f_eqsw_wvstop, D_RRH_ZERO , sizeof( f_eqsw_wvstop ));
	

	/* アラーム要因設定データの初期化 */
	memset( f_eqs_fm_alrm_data , D_RRH_ZERO , sizeof( f_eqs_fm_alrm_data ));
	/* Init TRA card restart both alm flag */
	f_eqsw_restart_both_alm_flg = D_SYS_OFF;

	/* Notification通知状態の初期化 */
	f_eqs_alm_start_data	= D_EQS_NTF_START_INIT ;
	f_eqs_status_start_data	= D_EQS_NTF_START_INIT ;
	f_eqs_alm_cnt			= D_EQS_FM_ALM_NON ;
	
	/********************************/
	/*	ietfHardwareメッセージ作成	*/
	/********************************/
	/*	メッセージ作成領域の初期化	*/
	memset( &f_eqs_ietfHwMsg, D_RRH_ZERO, sizeof( f_eqs_ietfHwMsg ));
	
	/* physical-indexの設定 */
	f_eqs_ietfHwMsg.msi_ietf_component.physical_index   = D_EQS_PHYSICAL_IDX ;

	/* ハードバージョンの読出し */
	snprintf( tmp_para , sizeof(tmp_para) , "%04x" , (USHORT)f_cmw_tra_inv_tbl->cmx_hver1) ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[0] = tmp_para[2] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[1] = tmp_para[3] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[2] = tmp_para[0] ;
	f_eqs_ietfHwMsg.msi_ietf_component.hardware_rev[3] = tmp_para[1] ;

	/* ソフトバージョンの読出し */
	/* 起動面のチェック */
	if ( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 )
	{
		/* 0面なら */
		f_eqs_ietfHwMsg.msi_ietf_component.software_rev = f_cmw_tra_inv_tbl->cmx_f0_sver[0] ;
	}
	else
	{
		/* 1面なら */
		f_eqs_ietfHwMsg.msi_ietf_component.software_rev = f_cmw_tra_inv_tbl->cmx_f1_sver[0] ;
	}

	/* シリアル番号の読出し */
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[0] = ((f_cmw_tra_inv_tbl->cmx_pcbno2>>8) & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[1] = (f_cmw_tra_inv_tbl->cmx_pcbno2 & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[2] = ((f_cmw_tra_inv_tbl->cmx_pcbno3>>8) & 0xff);
	f_eqs_ietfHwMsg.msi_ietf_component.serial_num[3] = (f_cmw_tra_inv_tbl->cmx_pcbno3 & 0xff);

	/* 会社名設定(mfg_name) */
	strcpy( f_eqs_ietfHwMsg.msi_ietf_component.mfg_name, mfg_name );

	/* カード図番(model_name)の読出し */
	for(loop_cnt=0; loop_cnt<(sizeof(f_cmw_tra_inv_tbl->cmx_mno)/sizeof(f_cmw_tra_inv_tbl->cmx_mno[0])); loop_cnt++)
	{
		/*	2バイトデータから1バイトずつ取り出し	*/
		wStr[0] = ((f_cmw_tra_inv_tbl->cmx_mno[loop_cnt] >> 8) & 0xff);
		wStr[1] = (f_cmw_tra_inv_tbl->cmx_mno[loop_cnt] & 0xff);
		for(loop=0; loop<sizeof(wStr); loop++)
		{
			f_eqs_ietfHwMsg.msi_ietf_component.model_name[loop_cnt*sizeof(wStr)+loop] = wStr[loop];
		}
	}

	/* 時刻情報の読出し */
	/* BCD-HEX変換 */
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.year = (((f_cmw_tra_inv_tbl->cmx_testyer>>12) & 0xf) * 1000)
			 + (((f_cmw_tra_inv_tbl->cmx_testyer>>8) & 0xf) * 100)
			 + (((f_cmw_tra_inv_tbl->cmx_testyer>>4) & 0xf) * 10)
			 + (f_cmw_tra_inv_tbl->cmx_testyer & 0xf);

	/* 月日のBCD-HEX変換 */
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.month =((((f_cmw_tra_inv_tbl->cmx_testday>>12) & 0xf) * 10) + ((f_cmw_tra_inv_tbl->cmx_testday>>8) & 0xf));
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.day =((((f_cmw_tra_inv_tbl->cmx_testday>>4) & 0xf) * 10) + (f_cmw_tra_inv_tbl->cmx_testday & 0xf));
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.hour	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.min	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.sec	= D_RRH_ZERO ;
	f_eqs_ietfHwMsg.msi_ietf_component.mfg_date.dummy	= D_RRH_ZERO ;

	/* 情報のダンプ */
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
	
	/* SV制御情報のデフォルト設定 */
	f_com_SVCTL_setDefault(D_DU_SVINF_ALL);

	/****************************************************************************************************************/
	/* 状態変化はアンテナキャリア状態でありここでは使用しない                                                       */
	/* Set INS bit ON 																								*/
	/* f_com_SVCtl(E_DU_SVCTL_OPE_USA, D_DU_SVCTL_CHG, 2); 															*/
	/****************************************************************************************************************/

	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
											sizeof(T_SYS_INIT_COMP_NTC),		/* MessageSize						*/
											(VOID **)&sndMsg,					/* MessageBuffer					*/
											&errcd );							/* ErrorCode						*/
	
	if( rtn_bpf != BPF_RU_IPCM_OK  )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET error=%08x", rtn_bpf );
	}

	/* 送信MSG作成 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID(0x00000102)			*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_EQS;							/* 送信元TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長					*/
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,								/* 送信MSGキューID					*/
							(VOID *)sndMsg );	
	
	/* Update task state to all  task init complete wait state */
	f_eqsw_thrdstate = D_RSV_TSK_STATE_USE;
	/* Update Last-Change Time */
	f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&f_eqsw_last_chg) ;
	return ;
}
/* @} */

/* @} */
