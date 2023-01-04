/*!
 * @skip  $ld:$
 * @file  f_eio_msgFanTimOutInd.c
 * @brief Indication of FAN sv & ctrl timeout.
 * @date 2019/03/19 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/*!
** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */

/*!
 *  @brief  Indication of FAN sv & ctrl timeout
 *  @note   This function is FAN sv & ctrl timeout.\n
 *  @param  *pMsg    [in]   receive message pointer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/19 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_msgFanTimOutInd( VOID *pMsg )
{
	UINT					regVal;
	INT						ret;           /* センサ温度情報取得結果 */
	INT						regVal_temp;   /* センサ温度レジスタ値 */
	UINT					fanIdx;
	USHORT					indexH, indexM, fanMonIdx;
	
	/*	入力値が異常の場合	*/
	if(pMsg == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Message pointer NULL");
	}
	/*	TO回数をインクリメント	*/
	f_eiow_fanMngTbl.fanToCnt++;
	
	/*	FANユニット状態を取得	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regVal);
	
	/****************************/
	/*	FAN実装状態監視			*/
	/****************************/
	f_eio_fanUnitStsDetect(regVal, &f_eior_fanUnitSvInfo[E_EIO_FANITEM_MOUNT]);
	
	/****************************/
	/*	FAN電源状態監視			*/
	/****************************/
	/*	FAN停止状態以外(FAN電源ON)の場合に監視する	*/
	if(f_eiow_fanMngTbl.fanCtrl.statusNow != E_DU_FANSPEEDSTS_STOP)
	{
		f_eio_fanUnitStsDetect(regVal, &f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS]);
	}
	/*	FAN停止状態(FAN電源OFF)の場合、PS ALM発生中ならfaultを復旧する	*/
	else if((f_eiow_svctrl_mngp->fanSv.fanUnitFault & D_DU_BIT_FAN_PSALM) != D_DU_VAL_FAN_NORMAL)
	{
		/*	fault復旧	*/
		f_com_SVCtl(f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].faultID, D_DU_SVCTL_OFF, f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].faultFact);
		
		/*	FANユニット状態にfault復旧	*/
		f_eiow_svctrl_mngp->fanSv.fanUnitFault &= ~(f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].stsBit);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s(0x%08x) for FAN UNMOUNT", f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].norLogMsg, regVal );
		
		/*	normal回数を初期化する	*/
		*(f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].normalCnt) = 0;
		/*	fault回数を初期化する	*/
		*(f_eior_fanUnitSvInfo[E_EIO_FANITEM_SPSTS].faultCnt) = 0;
	}
	/****************************/
	/*	電力変動監視処理          */
	/****************************/
    if(f_cmw_tra_inv_tbl->cmx_device_typ == E_RRH_RRHKIND_5GDU_LLS_SUB6 &&
		f_eiow_DuProcInfo->dbgmode != D_SYS_ON) {
        f_eio_PowerMonitor();
    }

	/****************************/
	/*	1秒周期処理の要否決定	*/
	/****************************/
	/*	1秒(100ms*10回)タイミングではない場合	*/
	if(f_eiow_fanMngTbl.fanToCnt < D_EIO_TONUM_1S)
	{
		/*	処理終了	*/
		return;
	}
	/*	1秒(100ms*10回)タイミングの場合	*/
	else
	{
		/*	TO回数を初期化	*/
		f_eiow_fanMngTbl.fanToCnt = 0;
	}
	
	/****************************/
	/*	FAN回転数監視			*/
	/****************************/
	/*	FANユニット状態がNormalの場合に監視を実施する	*/
	if(f_eiow_svctrl_mngp->fanSv.fanUnitFault == D_DU_VAL_FAN_NORMAL)
	{
		/*	センサ温度を取得	*/
		ret = BPF_HM_DEVC_TEMP2_READ(&regVal_temp);
		if(ret != BPF_HM_DEVC_COMPLETE)
		{
			/* 取得失敗時は無効値を設定する	*/
			regVal_temp = 0x7FFFFFFF;
		}

		/*	FAN#1~3まで繰り返す	*/
		for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
		{
			/*	FAN回転数を取得	*/
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, f_eior_fanSpeedInfo[fanIdx].regAddr, &regVal);
			
			/*	レジスタ読出値が範囲外の場合	*/
			if(regVal > D_DU_REGMSK_CTB_TAC1CNT)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN#%d SPEED Regster Error(0x%08x)", (fanIdx+1), regVal );
				/*	有効bitにてマスクする	*/
				regVal &= D_DU_REGMSK_CTB_TAC1CNT;
			}
			/*	FAN回転数をテーブルに格納する	*/
			f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeed[fanIdx] = regVal;
			
			/*	レジスタ読出結果が閾値未満(回転数異常)の場合	*/
			/*	かつ、センサ温度マスク無効の場合(マスク条件となるセンサ温度の閾値は機種ごとに異なる)	*/
			if((regVal < D_EIO_THRESHOLD_FANSPEED) &&
			   (((M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_SUB6) && (regVal_temp >= D_EIO_FANSPEED_MASK_TEMP_SUB6)) ||
			   	((M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_MMW)  && (regVal_temp >= D_EIO_FANSPEED_MASK_TEMP_MMW))))
			{
				/*	FAN回転数状態がnormalの場合	*/
				if((f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault & f_eior_fanSpeedInfo[fanIdx].stsBit) == 0)
				{
					/*	fault回数をインクリメントする	*/
					f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt++;
					
					/*	fault保護段回数超過	*/
					if(f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt >= D_EIO_PROTECTCNT_FANSPEEDFLT)
					{
						/*	fault発生	*/
						f_com_SVCtl(f_eior_fanSpeedInfo[fanIdx].fltId, D_DU_SVCTL_ON, f_eior_fanSpeedInfo[fanIdx].fltFct);
						
						/*	FAN回転数状態にfault設定	*/
						f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault |= f_eior_fanSpeedInfo[fanIdx].stsBit;
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN#%d SPEED FAULT(0x%08x)", (fanIdx+1), regVal );
						
						/*	fault回数を初期化する	*/
						f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt = 0;
					}
					/*	reset保護段回数超過	*/
					else if(f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt == D_EIO_PROTECTCNT_FANRESET_NOR)
					{
						/*	FAN Reset実施	*/
						f_eio_fanReset( fanIdx );
					}
				}
				/*	FAN回転数状態がfaultの場合	*/
				else
				{
					/*	FAN回転数fault中のFAN回転数異常カウンタをインクリメントする	*/
					f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt++;
					
					/*	reset保護段回数超過	*/
					if(f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt >= D_EIO_PROTECTCNT_FANRESET_FLT)
					{
						/*	FAN Reset実施	*/
						f_eio_fanReset( fanIdx );
						
						/*	FAN回転数fault中のFAN回転数異常カウンタを初期化	*/
						f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt = 0;
					}
				}
				/*	normal回数を初期化する	*/
				f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt = 0;
			}
			/*	レジスタ読出結果が閾値異常(回転数正常)の場合	*/
			else
			{
				/*	FAN回転数状態がfaultの場合	*/
				if((f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault & f_eior_fanSpeedInfo[fanIdx].stsBit) != 0)
				{
					/*	normal回数をインクリメントする	*/
					f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt++;
					
					/*	normal保護段回数超過	*/
					if(f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt >= D_EIO_PROTECTCNT_FANSPEEDNOR)
					{
						/*	fault復旧	*/
						f_com_SVCtl(f_eior_fanSpeedInfo[fanIdx].fltId, D_DU_SVCTL_OFF, f_eior_fanSpeedInfo[fanIdx].fltFct);
						
						/*	FANユニット状態にfault復旧	*/
						f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault &= ~f_eior_fanSpeedInfo[fanIdx].stsBit;
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN#%d SPEED NORMAL(0x%08x)", (fanIdx+1), regVal );
						
						/*	実装保護段回数を初期化する	*/
						f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt = 0;
					}
				}
				/*	fault回数を初期化する	*/
				f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt = 0;
				f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt = 0;
			}
			/****************************/
			/*	FAN回転数ログ(for dbg)	*/
			/****************************/
			/*	fanMonIdx:[0]=0[rpm]~/[1]=1890[rpm]~/[2]=2370[rpm]~/[3]=2850[rpm]~/[4]=3330[rpm]~	*/
			if(regVal < (D_EIO_THRESHOLD_FANSPEED))
			{
				fanMonIdx = 0;
			}
			else
			{
				fanMonIdx = 1 + (regVal-D_EIO_THRESHOLD_FANSPEED)/15;
			}
			if( fanMonIdx > 4 )
			{
				fanMonIdx = 4;
			}
			/*	1..3600	*/
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][fanIdx][fanMonIdx]++;
			/*	1..60	*/
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][fanIdx][fanMonIdx]++;
		}
		/************************************/
		/*	FAN回転数ログIndex算出(for dbg)	*/
		/************************************/
		indexH = f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH;
		indexM = f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM;
		f_eiow_svctrl_mngp->fanSpeedMon.fanMonCnt++;
		/*	1分間(60sec)の加算した場合	*/
		if((f_eiow_svctrl_mngp->fanSpeedMon.fanMonCnt % 60) == 0)
		{
			/*	100分間(0..99)のIndexを移動する	*/
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM = (USHORT)((indexM + 1) % 100);
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][0][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][1][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][2][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][0][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][1][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][2][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][0][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][1][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][2][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][0][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][1][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][2][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][0][4] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][1][4] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonM[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxM][2][4] = 0;
		}
		/*	1時間(3600sec)の加算した場合	*/
		if((f_eiow_svctrl_mngp->fanSpeedMon.fanMonCnt % 3600) == 0)
		{
			/*	100時間(0..99)ログに残す	*/
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH = (USHORT)((indexH + 1) % 100);
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonCnt = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][0][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][1][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][2][0] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][0][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][1][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][2][1] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][0][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][1][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][2][2] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][0][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][1][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][2][3] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][0][4] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][1][4] = 0;
			f_eiow_svctrl_mngp->fanSpeedMon.fanMonH[f_eiow_svctrl_mngp->fanSpeedMon.fanMonIdxH][2][4] = 0;
		}
	}
	
	/****************************/
	/*	FAN制御					*/
	/****************************/
/*	*//*	MAX状態カウンタがn未満の場合	*//*	ハソv1.05変更(n=3->0)	*/
/*	if(f_eiow_fanMngTbl.fanCtrl.fanMaxCnt < D_EIO_MAXSTATIM_MAX)*/
/*	{*/
/*		f_eiow_fanMngTbl.fanCtrl.fanMaxCnt++;*/
/*	}*/
/*	*//*	デバッグモードまたはMAX状態がn秒未満の場合	*//*	ハソv1.05変更(n=3->0)	*/
/*	if((f_eiow_svctrl_mngp->fanCtrl.fanDbgFlg == D_RRH_ON) ||*/
/*		(f_eiow_fanMngTbl.fanCtrl.fanMaxCnt < D_EIO_MAXSTATIM_MAX))*/
	/*	デバッグモードの場合	*/
	if(f_eiow_svctrl_mngp->fanCtrl.fanDbgFlg == D_RRH_ON)
	{
		/*	処理終了	*/
		return;
	}
/*	*//*	通常モードかつMAX状態がn秒以上継続の場合に以下を実施	*//*	ハソv1.05変更(n=3->0)	*/
	/*	通常モードの場合に以下を実施	*/
	/********************/
	/*	要因解析		*/
	/********************/
	/*	FANユニット状態が未実装の場合	*/
	if((f_eiow_svctrl_mngp->fanSv.fanUnitFault & D_DU_BIT_FAN_UNMOUNT) != D_DU_VAL_FAN_NORMAL)
	{
		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_STOP;
	}
	/*	FAN#1～3全ての回転数がnormalの場合	*/
/*	else if(f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault == D_DU_BIT_FAN_ALLOFF)*/
/*	{*/
/*		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_LOWDAC;*/
/*	}*/
/*	*//*	FAN#1～3のいずれかが回転数faultの場合	*/
	/*	FANユニット状態が実装の場合	*/
	else
	{
/*		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_HIGHDAC;*/
		f_eiow_fanMngTbl.fanCtrl.statusNxt = E_DU_FANSPEEDSTS_LOWDAC;
	}
	/********************/
	/*	状態遷移処理	*/
	/********************/
	/*	現状態と次状態が不一致の場合	*/
	if(f_eiow_fanMngTbl.fanCtrl.statusNow != f_eiow_fanMngTbl.fanCtrl.statusNxt)
	{
		/*	次状態がFAN停止状態の場合	*/
		if(f_eiow_fanMngTbl.fanCtrl.statusNxt == E_DU_FANSPEEDSTS_STOP)
		{
			/*	FAN電源制御(OFF)	*/
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_FANPWR, D_DU_REGBIT_CTB_FANPWR_ON);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN PowerOFF");
		}
		/*	FAN電圧制御	*/
/*		BPF_HM_DEVC_FANDAC_WRITE(f_eiow_svctrl_mngp->fanCtrl.fanDacParam[f_eiow_fanMngTbl.fanCtrl.statusNxt].fanDacVal);	*/
		
		/*	現状態がFNA停止状態の場合	*/
		if(f_eiow_fanMngTbl.fanCtrl.statusNow == E_DU_FANSPEEDSTS_STOP)
		{
			/*	FAN電源制御(ON)	*/
			BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_FANPWR, D_DU_REGBIT_CTB_FANPWR_ON);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN PowerON");
		}
		/*	外部IO制御履歴ログ	*/
		f_eio_extIoSvCtrlLog("[TO]FAN CTRL : FAN StsNo:%d->%d FANDAC=0x%x",
								 f_eiow_fanMngTbl.fanCtrl.statusNow, f_eiow_fanMngTbl.fanCtrl.statusNxt,
								 f_eiow_svctrl_mngp->fanCtrl.fanDacParam[f_eiow_fanMngTbl.fanCtrl.statusNxt].fanDacVal);
		/*	状態を更新する	*/
		f_eiow_fanMngTbl.fanCtrl.statusNow = f_eiow_fanMngTbl.fanCtrl.statusNxt;
	}
	return ;
}

/* @} */

