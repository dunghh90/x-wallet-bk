/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eio_main.c
 *  @brief  5GDU-LLS externalio main function
 *  @date   2019/03/18 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/**  
* @mainpage		the thread supervise and control external IO for 5GDU-LLS  (pf_eio) 
* @section		Introdution		   
* 				-# supervise line-in of external IO.
*   			-# control line-out of external IO.
*				-# supervise mount & power of fan unit.
*				-# supervise speed of fan#1~3. and if detect abnormal fan speed, reset the fan.
*				-# control fan unit power & speed.
*
*/


/**  
* @defgroup RRH_PF_EIO    5GDU-LLS supervision and control external IO Thread 
* @ingroup RRH   
* This class is 5GDU-LLS supervision and control external IO.
*/

/** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */
/**
* @brief 5GDU-LLS external IO main function
* @note  external IO main function.\n
* @param None
* @return None
* @date 2019/03/18 FJT)Koshida create
* @warning	N/A
* @FeatureID	5GDU-002-007
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eio_main()
{
	VOID			*pMsg;
	UINT			rcvSize;
	INT		errcd;

	prctl(PR_SET_NAME, "pf_eio", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_EIO,				/* ThreadID			*/
									D_SYS_THDQID_PF_EIO ,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */

	/* 初期化処理を実施 */
	f_eio_initProc();
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/************************************************************************************************************/
		/* メッセージ受信																							*/
		/************************************************************************************************************/
		errcd = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_EIO,												/* 受信MSGキューID			*/
					0,																	/* 受信待ち時間				*/
					(VOID**)&pMsg,														/* メッセージ受信バッファ	*/
					&rcvSize	);														/* 受信Messageサイズ		*/
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RECIEVE NG 0x%p(0x%08x)", pMsg, errcd );
			break;
		}

		/* analyze and dispatch message   */
		f_eio_anlz( pMsg ); 

		/* 受信バッファ解放	*/
		errcd = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FREE NG 0x%p(0x%08x)", pMsg, errcd );
			break;
		}
		
	}
	return ;
}

/**
* @brief 5GDU-LLS external IO init function
* @note  external IO init function.\n
* @param None
* @return None
* @date 2019/03/18 FJT)Koshida create
* @warning	N/A
* @FeatureID	5GDU-002-007
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eio_initProc()
{
	INT		errcd;
	INT		rtn_bpf;
	UINT	regVal;
	UCHAR	eepVal;
	UINT	fanIdx, loop/* , fanSta */;
	T_SYS_INIT_COMP_NTC*	sndMsg;			/* 送信メッセージ(初期化完了通知)	*/
	
	/*	外部IO状態を初期化する	*//*	lineIn情報は外部IO入力監視開始時に初期化する	*/
	/*	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_GPIO_DATA_2, D_DU_REGBIT_OTH_GPIO_DATA_2_LINEOUT);	*/
	f_eiow_extIo.lineOutReg = D_EIO_EXTIO_OPEN;	/*	外部IO出力状態	*/
	f_eiow_extIo.svStart    = D_EIO_EXTIO_OPEN;	/*	外部IO入力監視開始未	*/
	
	/* 外部IO情報テーブル(共有メモリ)ポインタを初期化  */
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_EIO_SVCTRL_MNG_TBL, (VOID **)&f_eiow_svctrl_mngp, &errcd);
	
	/* 外部IO制御履歴テーブル(共有メモリ)ポインタを初期化  */
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_EIO_SVCTRL_LOG_TBL, (VOID **)&f_eiow_svctrl_logp, &errcd);
	
/*	*//*	FAN DAC値を初期化	*//*	ハソv1.05削除	*/
/*	for(fanSta=E_DU_FANSPEEDSTS_STOP; fanSta<E_DU_FANSPEEDSTS_MAXNO; fanSta++)*/
/*	{*/
/*		f_eiow_svctrl_mngp->fanCtrl.fanDacParam[fanSta].fanDacVal = f_eior_fanDacVal[fanSta];*/
/*	}*/
	
	/*	FAN回転数ログを初期化する(for dbg)	*/
	f_eiow_svctrl_mngp->fanSpeedMon.fanMonCnt = 0;
	f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH = 0;
	f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM = 0;
	
	BPF_RU_IPCM_PROCSHM_ADDRGET(
			E_RRH_SHMID_APL_DU_PROCESS_INFO,
			(VOID **)&f_eiow_DuProcInfo,
			&errcd );
		
	for(loop = 0; loop < D_RRH_ANT_MAX; loop++) {
		f_eiow_antMngTbl[loop].powref = 0;
		f_eiow_antMngTbl[loop].change_cnt = 0;
	}
	for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
	{
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[0][fanIdx][0] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[0][fanIdx][1] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[0][fanIdx][2] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[0][fanIdx][3] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[0][fanIdx][4] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[0][fanIdx][0] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[0][fanIdx][1] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[0][fanIdx][2] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[0][fanIdx][3] = 0;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[0][fanIdx][4] = 0;
	}
	for(loop=1; loop<100; loop++)
	{
		for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
		{
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[loop][fanIdx][0] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[loop][fanIdx][1] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[loop][fanIdx][2] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[loop][fanIdx][3] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[loop][fanIdx][4] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[loop][fanIdx][0] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[loop][fanIdx][1] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[loop][fanIdx][2] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[loop][fanIdx][3] = 0xffff;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[loop][fanIdx][4] = 0xffff;
		}
	}
	/*	FANユニット監視関連のカウンタを初期化	*/
	f_eiow_fanMngTbl.fanSv.fanUnit.unMountCnt = 0;
	f_eiow_fanMngTbl.fanSv.fanUnit.mountCnt = 0;
	f_eiow_fanMngTbl.fanSv.fanUnit.psAbnCnt = 0;
	f_eiow_fanMngTbl.fanSv.fanUnit.psNorCnt = 0;
	
	/*	FAN回転数監視状態を初期化	*/
	f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault = D_DU_BIT_FAN_ALLOFF;
	
	/*	fault回転数状態を初期化する		*/
	for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
	{
		/*	FAN回転数を初期化	*/
		f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeed[fanIdx] = 0;
		
		/*	FAN回転数監視関連のカウンタを初期化	*/
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt = 0;
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt = 0;
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt = 0;
	}

	/* EEPROMからFAN回転数モードを取得	*/
	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_FAN_SPIN_MODE, &eepVal );

	if (eepVal == D_RRH_ON) {	/* 低速モード	*/
		regVal = D_EIO_SPINMODE_LOW_SPEED;
	} else {					/* 通常モード	*/
		regVal = D_EIO_SPINMODE_NML_SPEED;
	}
	/* FAN DAC制御 (低回転/通常回転)	*/
	BPF_HM_DEVC_FANDAC_WRITE( regVal );

	/*	FAN実装状態を取得	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regVal);
	
	/*	FANユニット状態を初期化(未実装)	*/
	f_eiow_svctrl_mngp->fanSv.fanUnitFault = D_DU_VAL_FAN_NORMAL;
	f_eiow_fanMngTbl.fanCtrl.statusNow = E_DU_FANSPEEDSTS_STOP;
	
	/*	FANユニット未実装の場合	*/
	if((regVal & D_DU_REGBIT_CTB_STA1_FANUNMOUNT) != 0)
	{
		/*	FANユニット未実装fault発生	*/
		f_com_SVCtl(E_DU_SVCTL_OPE_FID40_ALM, D_DU_SVCTL_ON, D_DU_FLTID_NO_FAN_DETECT);
		
		/*	FANユニット状態に未実装bitをON	*/
		f_eiow_svctrl_mngp->fanSv.fanUnitFault |= D_DU_BIT_FAN_UNMOUNT;
		
		/*	次状態にFAN停止を設定	*/
		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_STOP;
/*		f_eiow_fanMngTbl.fanCtrl.fanMaxCnt = D_EIO_MAXSTATIM_MAX;	*//*	最初の1秒目からFAN制御を行う	*//*	ハソv1.05削除(n=3->0)	*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "FAN UNMOUNT(0x%08x)", regVal );
	}
	/*	FANユニット実装の場合	*/
	else
	{
		/*	次状態にFAN通常を設定	*/
		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_LOWDAC;
/*		*//*	次状態にFAN MAXを設定	*/
/*		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_MAXDAC;	*//*	ハソv1.05変更(n=MAX->LOW)	*/
/*		f_eiow_fanMngTbl.fanCtrl.fanMaxCnt = D_EIO_MAXSTATIM_MIN;	*//*	ここからn秒間はFAN制御しない	*//*	ハソv1.05削除(n=3->0)	*/
	}
	/********************/
	/*	FAN起動			*/
	/********************/
	/*	FAN電圧制御	*//*	ハソv1.05削除	*/
/*	BPF_HM_DEVC_FANDAC_WRITE(f_eiow_svctrl_mngp->fanCtrl.fanDacParam[f_eiow_fanMngTbl.fanCtrl.statusNxt].fanDacVal);	*/
	
	/*	FAN停止以外	*/
	if(f_eiow_fanMngTbl.fanCtrl.statusNxt != E_DU_FANSPEEDSTS_STOP)
	{
		/*	FAN電源制御(ON)	*/
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_FANPWR, D_DU_REGBIT_CTB_FANPWR_ON);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "FAN MOUNT, PowerON");
	}
	/*	外部IO制御履歴ログ	*/
	f_eio_extIoSvCtrlLog("[INI]FAN StartUP : FAN StsNo:%d->%d FANDAC=0x%x",
							 f_eiow_fanMngTbl.fanCtrl.statusNow, f_eiow_fanMngTbl.fanCtrl.statusNxt,
							 f_eiow_svctrl_mngp->fanCtrl.fanDacParam[f_eiow_fanMngTbl.fanCtrl.statusNxt].fanDacVal);
	
	/*	状態を更新する	*/
	f_eiow_fanMngTbl.fanCtrl.statusNow = f_eiow_fanMngTbl.fanCtrl.statusNxt;
	
	/*	FAN監視制御周期タイマ(100ms)生成	*/
	rtn_bpf = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_PERIODIC,						/* Timer種別						*/
									E_RRH_TIMID_EIO_FANSVCTRL,						/* TimerID							*/
									D_EIO_FANSVCTRL_TIMVAL,							/* Timer値							*/
									D_SYS_MSGID_EIO_FANSVCTRL_TO,					/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PF_EIO,							/* TimeOut時送信先Queue番号			*/
									&errcd );
	
	if( rtn_bpf != 0 )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "FAN SVCTRL TIMER START NG 0x%08x(0x%08x)", rtn_bpf, errcd );
	}
	
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
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET NG 0x%08x(0x%08x)", rtn_bpf, errcd );
	}

	/* 送信MSG作成 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID(0x00000102)			*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_EIO;							/* 送信元TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長					*/
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,								/* 送信MSGキューID					*/
							(VOID *)sndMsg );	
	
	/* Update task state to all  task init complete wait state */
	f_eiow_thrdstate = E_EIO_STAT_RUN;
	
	return ;
}

/* @} */

