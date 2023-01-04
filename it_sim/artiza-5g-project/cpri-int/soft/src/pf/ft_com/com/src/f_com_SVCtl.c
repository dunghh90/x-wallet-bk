/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_AlmCtl.c
 *  @brief	TRA common function - hardware alarm report & clear function 
 *  @date   2012/07/04 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2012-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "f_sys_inc.h"			/* TRA common head file				*/
#include "f_com_inc.h"			/* TRA common head file				*/

/* @{ */
/**
* @brief SV state register setting process
* @note Set SV state registers according to control option.\n
* @param srcThrdId [in] Source thread ID
* @param svOpt [in] SV control option
* @param svCnt [in] CHG/ON/OFF Setting(0:OFF, 1:ON, 2:CHG)
* @param svVal [in] Setting Value(bit or code)
* @param logFlag [in] flag for logging ON/OFF(0:OFF, 1~:ON)
* @return None
* @date   2013/11/08 FFCS)Xut create
* @warning	N/A
* @FeatureID	PF-Svp-009-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
void f_com_SVCtl_real_sub(UINT svMask, UINT setVal, UINT regAddr, UINT *befReg, UINT *aftReg)
{
	INT								rtn_bpf;
	INT								errcd;
	
	/* Take semphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SVCTL, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	/*	変更前のレジスタ値を読み出す	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regAddr, befReg);
	
	/*	設定値を算出する	*/
	*aftReg = (*befReg & svMask) | setVal;
	
	/*	設定値に変化がある場合	*/
	if(*befReg != *aftReg)
	{
		/*	変更後のレジスタ値を設定する	*/
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, regAddr, aftReg);
	}
	else
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Not change writing data(Addr=%x, before=%x, after=%x)", 
																				regAddr, *befReg, *aftReg);
	}
	/* Give semphore of SV control */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SVCTL, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	return;
}


void f_com_SVCtl_real( UINT svOpt, UINT svCnt, UINT svVal, UINT logFlag, const VOID* fileName, UINT lineNo)
{
	UINT							befReg, aftReg;
	UINT							svMask;
	UINT							setVal;
	T_RRH_SVSTATELOG				svStateLog;
	T_COM_DU_SVBUS_CTRL_INFO_TBL	*tbl_wrokp;
	UINT							loop, lpNum;
	UINT							svIndex;
	T_COM_DU_FLTLVLNUM				faultLvl;
	T_COM_DU_FLTIDNUM				faultId;
	INT								rtn_bpf;
	INT								errcd;
	UCHAR							opt2char[E_DU_SVCTL_FLVNO_MAX][4] = { "MIN", "MAJ", "CRI" };
	UCHAR							*optStr;
	
	/*	引数が異常な場合	*/
	if((svOpt >= E_DU_SVCTL_OPE_MAX) || (svCnt >= D_DU_SVCTL_CNT_MUX))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "operation err(svOpt=%d/svCnt=%d); %s[%d]", 
																	svOpt, svCnt, fileName, lineNo);
		return;
	}

	/*	SVバス操作マップ情報を取得	*/
	tbl_wrokp = f_comw_svOptMapTbl[svOpt].infop;
	lpNum = f_comw_svOptMapTbl[svOpt].infoSize/sizeof(*tbl_wrokp);
	
	/*	制御種別が値変更の場合	*/
	if(svCnt == D_DU_SVCTL_CHG)
	{
		svMask = ~f_comw_svOptMapTbl[svOpt].mask;
		setVal = svVal<<f_comw_svOptMapTbl[svOpt].shift;
		svStateLog.chgType = D_SYS_SVSTATE_CTL;
		/*	制御するビット位置を検索する	*/
		for(loop=D_SYS_NUM0; loop < lpNum; loop++)
		{
			/*	制御するビット位置が決定した場合	*/
			if(tbl_wrokp->kind == svVal)
			{
				break;
			}
			tbl_wrokp++;
		}
		/*	制御するビット位置が見つからない場合	*/
		if(loop >= lpNum)
		{
			optStr = NULL;
		}
		else
		{
			optStr = tbl_wrokp->optStr;
		}
	}
	else
	{
		/*	制御するビット位置を検索する	*/
		for(loop=D_SYS_NUM0; loop < lpNum; loop++)
		{
			/*	制御するビット位置が決定した場合	*/
			if(tbl_wrokp->kind == svVal)
			{
				break;
			}
			tbl_wrokp++;
		}
		/*	制御するビット位置が見つからない場合	*/
		if(loop >= lpNum)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "on/off cntrl err(svOpt=%d/svCnt=%d/svVal=%d); %s[%d]", 
																						svOpt, svCnt, svVal, fileName, lineNo);
			return;
		}
		optStr = tbl_wrokp->optStr;
		/*	bit ON制御の場合	*/
		if(svCnt == D_DU_SVCTL_ON)
		{
			svMask = 0xffffffff;		/*	マスクしない	*/
			setVal = tbl_wrokp->val;
			svStateLog.chgType = D_SYS_SVSTATE_SET;
		}
		/*	bit OFF制御の場合	*/
		else
		{
			svMask = ~tbl_wrokp->val;
			setVal = 0;					/*	ビットONしない	*/
			svStateLog.chgType = D_SYS_SVSTATE_CLR;
		}
	}
	/*	引数の初期化	*/
	befReg = aftReg = 0;
	
	/*	SVバス制御処理をCALL	*/
	f_com_SVCtl_real_sub(svMask, setVal, f_comw_svOptMapTbl[svOpt].addr, &befReg, &aftReg);
	
	/*	設定値に変化有りの場合	*/
	if(befReg != aftReg)
	{
		/****************************/
		/* SV状態変化履歴ログ取得   */
		/****************************/
		/*	ログ取得ありの場合	*/
		if(logFlag != D_RRH_OFF)
		{
			/* Clear SV log state table */
			memset((void*)svStateLog.svState, 0, sizeof(svStateLog.svState));
			
			/* 変更したレジスタ位置を算出 */
			svIndex = (f_comw_svOptMapTbl[svOpt].addr - D_DU_REG_CTB_SV1INF)/D_SYS_NUM4;
			
			/*	変化種別が制御の場合	*/
			if(svStateLog.chgType == D_SYS_SVSTATE_CTL)
			{
				/*	制御情報を設定する	*/
				svStateLog.svState[svIndex] = aftReg;
			}
			/*	変化種別がON/OFFの場合	*/
			else
			{
				/*	変化ビット情報を設定する	*/
				svStateLog.svState[svIndex] |= tbl_wrokp->val;
			}
			f_com_SVLogSet_real(&svStateLog, optStr, fileName, lineNo);
		}
		/*	SV情報種別がFualt制御の場合	*/
		if(f_comw_svOptMapTbl[svOpt].type == D_DU_SVCTL_TYP_FLT)
		{
			/****************************/
			/* Fault Level/FaultID更新  */
			/****************************/
			if(f_comw_fltMngTblp == NULL)
			{
				/* Get share memory address of SV State history  */
				BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DU_FLT_MNG, (VOID **)&f_comw_fltMngTblp, &errcd);
				
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,
					"Fault mng tbl pointer Set(Null->0x%p); %s[%d]", f_comw_fltMngTblp, fileName, lineNo);
			}
			/*	Assert log取得要の場合	*/
			if(tbl_wrokp->astFlg == D_RRH_ON)
			{
				/*	BIT ON制御かつの前値がOFFの場合	*/
				if((svCnt == D_DU_SVCTL_ON)&&((befReg & tbl_wrokp->val) == 0))
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "Fault detected([%s]%s/Code=0x%x[%d]); %s[%d]", 
											opt2char[tbl_wrokp->opt2 & 3], f_comr_svOptFidTbl[tbl_wrokp->opt1].optStr, svVal, svVal, fileName, lineNo);
				}
				/*	BIT OFF制御かつの前値がONの場合	*/
				else if((svCnt == D_DU_SVCTL_OFF)&&((befReg & tbl_wrokp->val) != 0))
				{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "Fault recovered([%s]%s/Code=0x%x[%d]); %s[%d]", 
											opt2char[tbl_wrokp->opt2 & 3], f_comr_svOptFidTbl[tbl_wrokp->opt1].optStr, svVal, svVal, fileName, lineNo);
				}
			}
			/*	sem wait time	*/
			struct bpf_timespec		dtime;	/* sem waittime */	
			dtime.tv_sec = 0;
			dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;
			
			/*	SVバス情報のFault LevelとFault IFを制御する	*/
			/* Clear SV log state table */
			memset((void*)&faultLvl, 0, sizeof(faultLvl));
			memset((void*)&faultId, 0, sizeof(faultId));
			
			tbl_wrokp = f_comw_svOptMapTbl[svOpt].infop;
			for(loop=0; loop<lpNum; loop++)
			{
				/*	該当BITがOFF->ON変化の場合	*/
				if(((befReg & tbl_wrokp->val) == 0) && ((aftReg & tbl_wrokp->val) != 0))
				{
					/*	該当Fault Levelが初制御の場合	*/
					if(faultLvl.info[tbl_wrokp->opt2].validFlg != D_RRH_ON)
					{
						faultLvl.osft[faultLvl.num]=tbl_wrokp->opt2;
						faultLvl.info[tbl_wrokp->opt2].validFlg = D_RRH_ON;
						faultLvl.num++;
					}
					/*	ON/OFF数をインクリメントする	*/
					faultLvl.info[tbl_wrokp->opt2].onOffNum++;
					
					/*	該当Fault IDが初制御の場合	*/
					if(faultId.info[tbl_wrokp->opt1].validFlg != D_RRH_ON)
					{
						faultId.osft[faultId.num]=tbl_wrokp->opt1;
						faultId.info[tbl_wrokp->opt1].validFlg = D_RRH_ON;
						faultId.num++;
					}
					/*	ON/OFF数をインクリメントする	*/
					faultId.info[tbl_wrokp->opt1].onOffNum++;
				}
				/*	該当BITがON->OFF変化の場合	*/
				else if(((befReg & tbl_wrokp->val) != 0) && ((aftReg & tbl_wrokp->val) == 0))
				{
					/*	該当Fault Levelが初制御の場合	*/
					if(faultLvl.info[tbl_wrokp->opt2].validFlg != D_RRH_ON)
					{
						faultLvl.osft[faultLvl.num]=tbl_wrokp->opt2;
						faultLvl.info[tbl_wrokp->opt2].validFlg = D_RRH_ON;
						faultLvl.num++;
					}
					/*	ON/OFF数をデクリメントするする	*/
					faultLvl.info[tbl_wrokp->opt2].onOffNum--;
					
					/*	該当Fault IDが初制御の場合	*/
					if(faultId.info[tbl_wrokp->opt1].validFlg != D_RRH_ON)
					{
						faultId.osft[faultId.num]=tbl_wrokp->opt1;
						faultId.info[tbl_wrokp->opt1].validFlg = D_RRH_ON;
						faultId.num++;
					}
					/*	ON/OFF数をデクリメントするする	*/
					faultId.info[tbl_wrokp->opt1].onOffNum--;
				}
				tbl_wrokp++;
			}
			/*	レジスタ読出値、設定値を初期化する	*/
			befReg = aftReg = 0;
			/*	fault levelのSVバス制御を行う	*/
			for(loop=0; loop<faultLvl.num; loop++)
			{
				/*	該当のFaultLevelが有効な場合	*/
				if(faultLvl.info[faultLvl.osft[loop]].validFlg == D_RRH_ON)
				{
					/* Take semphore of SV control */
					rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_FLTLVL, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
					if(BPF_RU_IPCM_OK != rtn_bpf)
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
							"SV CTL fault level sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
					}
					/*	Fault検出の場合	*/
					if(faultLvl.info[faultLvl.osft[loop]].onOffNum > 0)
					{
						/*	同一Fault LevelがOFF=>ONの場合	*/
						if(f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] == 0)
						{
							/*	SVバス制御処理(Bit ON)をCALL	*/
							f_com_SVCtl_real_sub(0xffffffff, f_comr_svOptFlvTbl[faultLvl.osft[loop]].val, f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FLV].addr, &befReg, &aftReg);
						}
						f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] += faultLvl.info[faultLvl.osft[loop]].onOffNum;
					}
					/*	Fault復旧の場合	*/
					else if((f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] > 0) && (faultLvl.info[faultLvl.osft[loop]].onOffNum < 0) )
					{
						/*	同一Fault LevelがON=>OFFの場合	*/
						if((f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] + faultLvl.info[faultLvl.osft[loop]].onOffNum) <= 0)
						{
							/*	SVバス制御処理(Bit OFF)をCALL	*/
							f_com_SVCtl_real_sub((~f_comr_svOptFlvTbl[faultLvl.osft[loop]].val), 0, f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FLV].addr, &befReg, &aftReg);
							f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] = 0;
						}
						else
						{
							f_comw_fltMngTblp->fltLvlNum[faultLvl.osft[loop]] += faultLvl.info[faultLvl.osft[loop]].onOffNum;
						}
					}
					/*	ON=>ONまたはOFF=>OFFの場合	*/
					else{}	/*	無処理	*/
					
					/* Give semphore of SV control */
					rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_FLTLVL, BPF_RU_IPCM_LOCK_RW, &errcd);
					if(BPF_RU_IPCM_OK != rtn_bpf)
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
							"SV CTL fault level sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
					}
				}
				/*	該当のFaultLevelが無効な場合	*/
				else
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Fault Level invalid(loop=%d/lvl=%d/valid=%d/onoff=%d)", 
										loop, faultLvl.osft[loop], faultLvl.info[faultLvl.osft[loop]].validFlg, faultLvl.info[faultLvl.osft[loop]].onOffNum);
				}
			}
			/*	fault lDのSVバス制御を行う	*/
			for(loop=0; loop<faultId.num; loop++)
			{
				/*	該当のFaultLevelが有効な場合	*/
				if(faultId.info[faultId.osft[loop]].validFlg == D_RRH_ON)
				{
					/* Take semphore of SV control */
					rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_FLTID, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
					if(BPF_RU_IPCM_OK != rtn_bpf)
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
							"SV CTL fault ID sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
					}
					/*	Fault検出の場合	*/
					if(faultId.info[faultId.osft[loop]].onOffNum > 0)
					{
						/*	同一Fault LevelがOFF=>ONの場合	*/
						if(f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] == 0)
						{
							/*	SVバス制御処理(Bit ON)をCALL	*/
							f_com_SVCtl_real_sub(0xffffffff, f_comr_svOptFidTbl[faultId.osft[loop]].val, f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID].addr, &befReg, &aftReg);
						}
						f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] += faultId.info[faultId.osft[loop]].onOffNum;
					}
					/*	Fault復旧の場合	*/
					else if((f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] > 0) && (faultId.info[faultId.osft[loop]].onOffNum < 0) )
					{
						/*	同一Fault LevelがON=>OFFの場合	*/
						if((f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] + faultId.info[faultId.osft[loop]].onOffNum) <= 0)
						{
							/*	SVバス制御処理(Bit OFF)をCALL	*/
							f_com_SVCtl_real_sub((~f_comr_svOptFidTbl[faultId.osft[loop]].val), 0, f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID].addr, &befReg, &aftReg);
							f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] = 0;
						}
						else
						{
							f_comw_fltMngTblp->fltIdNum[faultId.osft[loop]] += faultId.info[faultId.osft[loop]].onOffNum;
						}
					}
					/*	ON=>ONまたはOFF=>OFFの場合	*/
					else{}	/*	無処理	*/
					
					/* Give semphore of SV control */
					rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_FLTID, BPF_RU_IPCM_LOCK_RW, &errcd);
					if(BPF_RU_IPCM_OK != rtn_bpf)
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
							"SV CTL fault ID sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
					}
				}
				/*	該当のFaultLevelが無効な場合	*/
				else
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Fault ID invalid(loop=%d/lvl=%d/valid=%d/onoff=%d)", 
										loop, faultId.osft[loop], faultId.info[faultId.osft[loop]].validFlg, faultId.info[faultId.osft[loop]].onOffNum);
				}
			}
		}
	}
	return;
}

/* @} */

/**
* @brief Set SV state register runhis log
* @note Set SV state register runhis log.\n
* @param *svStateLog [in] SV log info pointer
* @return None
* @date   2018/08/24 FJT)koshida create
* @warning	N/A
* @FeatureID	5GDU-002-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
void f_com_SVLogSet_real(T_RRH_SVSTATELOG *svStateLog, UCHAR *optStr, const VOID* fileName, UINT lineNo)
{
	UINT					logIndex;
	INT						rtn_bpf;
	INT						errcd;
	
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;
	
	/*	SV CTL LOG table pointer取得未の場合	*/
	if(f_comw_svStateChg_his == NULL)
	{
		/* Get share memory address of SV State history  */
		BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SV_STATE_HISTORY, 
			(VOID **)&f_comw_svStateChg_his, 
			&errcd);
		
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,
			"SV CTL LOG tbl pointer Set(Null->0x%p); %s[%d]", f_comw_svStateChg_his, fileName, lineNo);
	}
	/* Take semphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SVCTL_LOG,
		BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL LOG sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	/* If the index reach the max, set to 0 */
	if(f_comw_svStateChg_his->logIndex >= D_RRH_SVLOG_MAX)
	{
		f_comw_svStateChg_his->logIndex = 0;
	}
	/* Get log index */
	logIndex = f_comw_svStateChg_his->logIndex;
	/* Get system time */
	BPF_HM_DEVC_GETTIME(&(f_comw_svStateChg_his->svStateRecord[logIndex].sysTime));
	/* Set SV state log record */
	memcpy((VOID*)&(f_comw_svStateChg_his->svStateRecord[logIndex].svStateLog), 
			(VOID*)svStateLog, sizeof(*svStateLog));

	size_t Str_len;
	if(optStr != NULL)
	{
		memset((VOID*)&(f_comw_svStateChg_his->svStateRecord[logIndex].optStr),0,sizeof(f_comw_svStateChg_his->svStateRecord[logIndex].optStr));
		Str_len = strlen((const char*)optStr);
		if(Str_len > 0){
			if( Str_len > ( sizeof(f_comw_svStateChg_his->svStateRecord[logIndex].optStr) - 1)){
				Str_len = sizeof(f_comw_svStateChg_his->svStateRecord[logIndex].optStr) - 1;
			}
			strncpy((VOID*)&(f_comw_svStateChg_his->svStateRecord[logIndex].optStr),(VOID*)optStr, Str_len);
		}
	}
	else
	{
		/*	先頭を0で初期化	*/
		f_comw_svStateChg_his->svStateRecord[logIndex].optStr[0] = 0;
	}

	memset((VOID*)&(f_comw_svStateChg_his->svStateRecord[logIndex].ucFileName),0,D_RRH_LOG_MAX_ASSERTFILENAME);
	Str_len = strlen(fileName);
	if(Str_len > 0){
		if(Str_len > (D_RRH_LOG_MAX_ASSERTFILENAME - 1)){
			Str_len = D_RRH_LOG_MAX_ASSERTFILENAME - 1;
		}
		strncpy((VOID*)&(f_comw_svStateChg_his->svStateRecord[logIndex].ucFileName),fileName, Str_len);
	}
	f_comw_svStateChg_his->svStateRecord[logIndex].uiLine = lineNo;

	/* Log index increment */
	(f_comw_svStateChg_his->logIndex)++;
	/* If the index reach the max, set to 0 */
	if(f_comw_svStateChg_his->logIndex >= D_RRH_SVLOG_MAX)
	{
		f_comw_svStateChg_his->logIndex = 0;
	}
	/* Give semphore of SV control */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SVCTL_LOG,
		BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL LOG sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	return;
}

/**
* @brief Set SV state register defaut value
* @note Set SV state register defaut value.\n
* @param infNum [in] 1~16: SV state register offset, 0:all
* @return None
* @date   2018/08/24 FJT)koshida create
* @warning	N/A
* @FeatureID	5GDU-002-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_com_SVCTL_setDefault_part(UINT infOfst, UINT maskOnOff)
{
	UINT	regAddr, regMskAddr;
	UINT	regDat = 0;
	UINT	regMsk = 0;
	UINT	svInfoNum;
	UINT	infLp;
	UINT	svOpt;
	
	regAddr = f_comr_svOptMapDefTbl[infOfst].addr;
	regMskAddr = f_comr_svOptMapDefTbl[infOfst].mskAddr;
	svInfoNum = f_comr_svOptMapDefTbl[infOfst].infoNum;
	/*	SVバス情報数繰り返す	*/
	for(infLp=0; infLp<svInfoNum; infLp++)
	{
		/*	各情報の初期値を設定する	*/
		svOpt = f_comr_svOptMapDefTbl[infOfst].opt[infLp];
		if(svOpt < E_DU_SVCTL_OPE_MAX)
		{
			regDat |= (UINT)((f_comw_svOptMapTbl[svOpt].def << f_comw_svOptMapTbl[svOpt].shift) & f_comw_svOptMapTbl[svOpt].mask);
			regMsk |= f_comw_svOptMapTbl[svOpt].mask;
		}
		else
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"SV CTL INIT error [%d]OPT=%d", infLp, svOpt);
		}
	}
	/*	各SVバスレジスタに初期値を書き込む	*/
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, regAddr, &regDat);
	
	/*	maskフラグがONの場合	*/
	if(maskOnOff == D_RRH_ON)
	{
		/*	各SVバスマスクレジスタに初期値を書き込む	*/
		regMsk = ~regMsk;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, regMskAddr, &regMsk);
	}
	return;
}

VOID f_com_SVCTL_setDefault(UINT infNum)
{
	UINT	svLp;
	UINT	infOfs;

	if(infNum > D_DU_SVINF_NUM)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "f_com_SVCTL_setDefault parameter Err<%d>", infNum);
		infNum = D_DU_SVINF_ALL;
	}
	/*	全SVバス情報初期化の場合	*/
	if(infNum == D_DU_SVINF_ALL)
	{
		/*	SVバスレジスタ数繰り返す	*/
		for(svLp=0; svLp<D_DU_SVINF_NUM; svLp++)
		{
			/*	SVバスレジスタ制御情報を初期化する	*/
			f_com_SVCTL_setDefault_part(svLp, D_RRH_ON);
		}
	}
	/*	ofst指定のSVバス情報のみ初期化の場合	*/
	else
	{
		infOfs = (UINT)(infNum-1);
		f_com_SVCTL_setDefault_part(infOfs, D_RRH_OFF);
	}
	return;
}

void f_com_SVCtl_abort( UINT fltid , UINT abortCode )
{
	UINT	fltfact = abortCode & 0xFFF;
	switch(fltid){
		case 1:
			f_com_SVCtl(E_DU_SVCTL_OPE_SFT_ALMCD,D_DU_SVCTL_CHG,fltfact);
			break;
		case 2:
			f_com_SVCtl(E_DU_SVCTL_OPE_INI_ALMCD,D_DU_SVCTL_CHG,fltfact);
			break;
		default:
			break;
	}
	return;
}

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
struct CMT_SVOpt_MapTbl{
    UINT opt;
    UINT bit;
    UINT addr;
    USHORT cpri_num;
}const tSvOptMapTbl[] = {
    {E_SYS_SVCTL_ERR,                   D_RRH_REG_CNT_SV1INF_ERR,                           D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_ALM,                   D_RRH_REG_CNT_SV1INF_ALM,                           D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_USE,                   D_RRH_REG_CNT_SV1INF_USE,                           D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_ACT,                   D_RRH_REG_CNT_SV1INF_ACT,                           D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_NOINS,                 D_RRH_REG_CNT_SV1INF_NOINS,                         D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_INS,                   D_RRH_REG_CNT_SV1INF_INS,                           D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_S3G_USE_01,            D_RRH_REG_CNT_SV1INF_S3G_USE_01,                    D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_S3G_USE_02,            D_RRH_REG_CNT_SV1INF_S3G_USE_02,                    D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_S3G_USE_03,            D_RRH_REG_CNT_SV1INF_S3G_USE_03,                    D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_S3G_USE_04,            D_RRH_REG_CNT_SV1INF_S3G_USE_04,                    D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_3G_USE_01,             D_RRH_REG_CNT_SV1INF_3G_USE_01,                     D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_3G_USE_02,             D_RRH_REG_CNT_SV1INF_3G_USE_02,                     D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_3G_USE_03,             D_RRH_REG_CNT_SV1INF_3G_USE_03,                     D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_3G_USE_04,             D_RRH_REG_CNT_SV1INF_3G_USE_04,                     D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_PLL_ALM,               D_RRH_REG_CNT_SV1INF_PLL_ALM,                       D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_CPRI_ALM,              D_RRH_REG_CNT_SV1INF_CPRI_ALM,                      D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_COM_ALM,               D_RRH_REG_CNT_SV1INF_COM_ALM,                       D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_SOFT_ALM,              D_RRH_REG_CNT_SV1INF_SOFT_ALM,                      D_RRH_REG_CNT_SV1INF,  0},
    {E_SYS_SVCTL_SOFT_ALM2,             D_RRH_REG_CNT_SV1INF_SOFT_ALM2,                     D_RRH_REG_CNT_SV1INF,  0},

    {E_SYS_SVCTL_CPRI_ALM_01,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE1 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_CPRI_ALM_02,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE2 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_CPRI_ALM_03,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE3 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_CPRI_ALM_04,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE4 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_CPRI_ALM_05,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE5 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_CPRI_ALM_06,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE6 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_CPRI_ALM_07,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE7 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_CPRI_ALM_08,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE8 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_CPRI_ALM_09,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE9 ),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_CPRI_ALM_10,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE10),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_CPRI_ALM_11,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE11),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_CPRI_ALM_12,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE12),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_CPRI_ALM_13,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE13),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_CPRI_ALM_14,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE14),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_CPRI_ALM_15,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE15),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_CPRI_ALM_16,           M_RRH_REG_CNT_CPRI_ALM(D_RRH_CPRINO_RE16),          D_RRH_REG_CNT_SV2INF,  D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_3G_FREQ_CMPLX_ALM,     D_RRH_REG_CNT_SV7INF_3G_FREQ_CMPLX_ALM,             D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_3G_CARR_NUM_ALM,       D_RRH_REG_CNT_SV7INF_3G_CARR_NUM_ALM,               D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_3G_UP_RESRC_NUM_ALM,   D_RRH_REG_CNT_SV7INF_3G_UP_RESRC_NUM_ALM,           D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_3G_DWN_RESRC_NUM_ALM,  D_RRH_REG_CNT_SV7INF_3G_DWN_RESRC_NUM_ALM,          D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_S3G_SHARETYPE_ALM,     D_RRH_REG_CNT_SV7INF_S3G_SHARETYPE_ALM,             D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_S3G_FREQ_CMPLX_ALM,    D_RRH_REG_CNT_SV7INF_S3G_FREQ_CMPLX_ALM,            D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_S3G_BAND_WIDTH_ALM,    D_RRH_REG_CNT_SV7INF_S3G_BAND_WIDTH_ALM,            D_RRH_REG_CNT_SV7INF,  0},
    {E_SYS_SVCTL_S3G_CARR_NUMALM,       D_RRH_REG_CNT_SV7INF_S3G_CARR_NUMALM,               D_RRH_REG_CNT_SV7INF,  0},

    {E_SYS_SVCTL_RE01_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE1 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_RE02_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE2 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_RE03_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE3 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_RE04_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE4 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_RE05_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE5 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_RE06_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE6 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_RE07_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE7 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_RE08_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE8 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_RE09_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE9 ),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_RE10_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE10),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_RE11_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE11),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_RE12_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE12),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_RE13_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE13),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_RE14_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE14),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_RE15_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE15),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_RE16_OPT_STS,          M_RRH_REG_CNT_OPT_STS(D_RRH_CPRINO_RE16),           D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_RE01_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE1 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_RE02_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE2 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_RE03_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE3 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_RE04_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE4 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_RE05_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE5 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_RE06_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE6 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_RE07_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE7 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_RE08_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE8 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_RE09_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE9 ),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_RE10_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE10),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_RE11_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE11),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_RE12_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE12),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_RE13_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE13),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_RE14_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE14),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_RE15_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE15),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_RE16_WAVE_STS,         M_RRH_REG_CNT_WAVE_STS(D_RRH_CPRINO_RE16),          D_RRH_REG_CNT_SV8INF,  D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_L1ERR,                 D_RRH_REG_CNT_SV9INF_L1ERR,                         D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_L2ERR,                 D_RRH_REG_CNT_SV9INF_L2ERR,                         D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_3G_L3ERR,              D_RRH_REG_CNT_SV9INF_3G_L3ERR,                      D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_S3G_L3ERR,             D_RRH_REG_CNT_SV9INF_S3G_L3ERR,                     D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_SFP_ERR1,              D_RRH_REG_CNT_SV9INF_SFP_ERR1,                      D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_SFP_ERR2,              D_RRH_REG_CNT_SV9INF_SFP_ERR2,                      D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_DELAY_ERR,             D_RRH_REG_CNT_SV9INF_DELAY_ERR,                     D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_DELAY_ERR_S3G,         D_RRH_REG_CNT_SV9INF_DELAY_ERR_S3G,                 D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_SYSPARA_DL_ERR,        D_RRH_REG_CNT_SV9INF_SYSPARA_DL_ERR,                D_RRH_REG_CNT_SV9INF,  0},
    {E_SYS_SVCTL_FIRM_DL_ERR,           D_RRH_REG_CNT_SV9INF_FIRM_DL_ERR,                   D_RRH_REG_CNT_SV9INF,  0},
	
    {E_SYS_SVCTL_L1_ERR_01,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE1 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_L1_ERR_02,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE2 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_L1_ERR_03,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE3 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_L1_ERR_04,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE4 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_L1_ERR_05,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE5 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_L1_ERR_06,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE6 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_L1_ERR_07,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE7 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_L1_ERR_08,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE8 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_L1_ERR_09,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE9 ),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_L1_ERR_10,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE10),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_L1_ERR_11,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE11),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_L1_ERR_12,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE12),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_L1_ERR_13,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE13),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_L1_ERR_14,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE14),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_L1_ERR_15,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE15),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_L1_ERR_16,             M_RRH_REG_CNT_L1_ERR(D_RRH_CPRINO_RE16),            D_RRH_REG_CNT_SV9INF,  D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_L2_ERR_01,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE1 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_L2_ERR_02,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE2 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_L2_ERR_03,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE3 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_L2_ERR_04,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE4 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_L2_ERR_05,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE5 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_L2_ERR_06,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE6 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_L2_ERR_07,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE7 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_L2_ERR_08,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE8 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_L2_ERR_09,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE9 ),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_L2_ERR_10,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE10),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_L2_ERR_11,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE11),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_L2_ERR_12,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE12),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_L2_ERR_13,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE13),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_L2_ERR_14,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE14),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_L2_ERR_15,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE15),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_L2_ERR_16,             M_RRH_REG_CNT_L2_ERR(D_RRH_CPRINO_RE16),            D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_3G_L3_ERR_01,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE1 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_3G_L3_ERR_02,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE2 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_3G_L3_ERR_03,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE3 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_3G_L3_ERR_04,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE4 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_3G_L3_ERR_05,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE5 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_3G_L3_ERR_06,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE6 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_3G_L3_ERR_07,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE7 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_3G_L3_ERR_08,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE8 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_3G_L3_ERR_09,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE9 ),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_3G_L3_ERR_10,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE10),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_3G_L3_ERR_11,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE11),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_3G_L3_ERR_12,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE12),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_3G_L3_ERR_13,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE13),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_3G_L3_ERR_14,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE14),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_3G_L3_ERR_15,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE15),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_3G_L3_ERR_16,          M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_RE16),         D_RRH_REG_CNT_SV10INF, D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_S3G_L3_ERR_01,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE1 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_S3G_L3_ERR_02,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE2 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_S3G_L3_ERR_03,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE3 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_S3G_L3_ERR_04,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE4 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_S3G_L3_ERR_05,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE5 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_S3G_L3_ERR_06,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE6 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_S3G_L3_ERR_07,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE7 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_S3G_L3_ERR_08,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE8 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_S3G_L3_ERR_09,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE9 ),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_S3G_L3_ERR_10,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE10),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_S3G_L3_ERR_11,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE11),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_S3G_L3_ERR_12,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE12),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_S3G_L3_ERR_13,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE13),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_S3G_L3_ERR_14,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE14),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_S3G_L3_ERR_15,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE15),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_S3G_L3_ERR_16,         M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_RE16),        D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_SFP_ERR1_01,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE1 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_SFP_ERR1_02,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE2 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_SFP_ERR1_03,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE3 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_SFP_ERR1_04,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE4 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_SFP_ERR1_05,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE5 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_SFP_ERR1_06,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE6 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_SFP_ERR1_07,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE7 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_SFP_ERR1_08,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE8 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_SFP_ERR1_09,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE9 ),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_SFP_ERR1_10,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE10),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_SFP_ERR1_11,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE11),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_SFP_ERR1_12,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE12),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_SFP_ERR1_13,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE13),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_SFP_ERR1_14,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE14),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_SFP_ERR1_15,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE15),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_SFP_ERR1_16,           M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE16),          D_RRH_REG_CNT_SV11INF, D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_SFP_ERR2_01,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE1 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_SFP_ERR2_02,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE2 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_SFP_ERR2_03,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE3 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_SFP_ERR2_04,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE4 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_SFP_ERR2_05,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE5 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_SFP_ERR2_06,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE6 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_SFP_ERR2_07,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE7 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_SFP_ERR2_08,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE8 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_SFP_ERR2_09,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE9 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_SFP_ERR2_10,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE10),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_SFP_ERR2_11,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE11),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_SFP_ERR2_12,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE12),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_SFP_ERR2_13,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE13),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_SFP_ERR2_14,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE14),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_SFP_ERR2_15,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE15),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_SFP_ERR2_16,           M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE16),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE16},
	
//16B
    {E_SYS_SVCTL_SLOT_ERR_01,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE1 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_SLOT_ERR_02,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE2 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_SLOT_ERR_03,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE3 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_SLOT_ERR_04,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE4 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_SLOT_ERR_05,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE5 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_SLOT_ERR_06,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE6 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_SLOT_ERR_07,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE7 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_SLOT_ERR_08,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE8 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_SLOT_ERR_09,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE9 ),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_SLOT_ERR_10,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE10),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_SLOT_ERR_11,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE11),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_SLOT_ERR_12,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE12),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_SLOT_ERR_13,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE13),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_SLOT_ERR_14,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE14),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_SLOT_ERR_15,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE15),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_SLOT_ERR_16,          M_RRH_REG_CNT_SLOT_ERR(D_RRH_CPRINO_RE16),         D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE16},
//  {E_SYS_SVCTL_SFP_ERR3_01,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE1 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE1 },
//  {E_SYS_SVCTL_SFP_ERR3_02,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE2 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE2 },
//  {E_SYS_SVCTL_SFP_ERR3_03,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE3 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE3 },
//  {E_SYS_SVCTL_SFP_ERR3_04,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE4 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE4 },
//  {E_SYS_SVCTL_SFP_ERR3_05,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE5 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE5 },
//  {E_SYS_SVCTL_SFP_ERR3_06,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE6 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE6 },
//  {E_SYS_SVCTL_SFP_ERR3_07,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE7 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE7 },
//  {E_SYS_SVCTL_SFP_ERR3_08,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE8 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE8 },
//  {E_SYS_SVCTL_SFP_ERR3_09,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE9 ),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE9 },
//  {E_SYS_SVCTL_SFP_ERR3_10,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE10),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE10},
//  {E_SYS_SVCTL_SFP_ERR3_11,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE11),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE11},
//  {E_SYS_SVCTL_SFP_ERR3_12,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE12),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE12},
//  {E_SYS_SVCTL_SFP_ERR3_13,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE13),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE13},
//  {E_SYS_SVCTL_SFP_ERR3_14,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE14),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE14},
//  {E_SYS_SVCTL_SFP_ERR3_15,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE15),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE15},
//  {E_SYS_SVCTL_SFP_ERR3_16,           M_RRH_REG_CNT_SFP_ERR3(D_RRH_CPRINO_RE16),          D_RRH_REG_CNT_SV12INF, D_RRH_CPRINO_RE16},
//16B

    {E_SYS_SVCTL_DELAY_ERR1_01,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE1 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_DELAY_ERR1_02,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE2 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_DELAY_ERR1_03,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE3 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_DELAY_ERR1_04,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE4 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_DELAY_ERR1_05,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE5 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_DELAY_ERR1_06,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE6 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_DELAY_ERR1_07,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE7 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_DELAY_ERR1_08,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE8 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_DELAY_ERR1_09,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE9 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_DELAY_ERR1_10,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE10),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_DELAY_ERR1_11,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE11),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_DELAY_ERR1_12,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE12),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_DELAY_ERR1_13,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE13),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_DELAY_ERR1_14,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE14),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_DELAY_ERR1_15,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE15),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_DELAY_ERR1_16,         M_RRH_REG_CNT_DELAY_ERR1(D_RRH_CPRINO_RE16),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_DELAY_ERR2_01,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE1 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_DELAY_ERR2_02,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE2 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_DELAY_ERR2_03,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE3 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_DELAY_ERR2_04,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE4 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_DELAY_ERR2_05,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE5 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_DELAY_ERR2_06,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE6 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_DELAY_ERR2_07,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE7 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_DELAY_ERR2_08,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE8 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_DELAY_ERR2_09,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE9 ),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_DELAY_ERR2_10,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE10),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_DELAY_ERR2_11,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE11),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_DELAY_ERR2_12,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE12),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_DELAY_ERR2_13,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE13),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_DELAY_ERR2_14,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE14),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_DELAY_ERR2_15,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE15),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_DELAY_ERR2_16,         M_RRH_REG_CNT_DELAY_ERR2(D_RRH_CPRINO_RE16),        D_RRH_REG_CNT_SV13INF, D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_STARTUP_ERR1_01,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE1 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_STARTUP_ERR1_02,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE2 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_STARTUP_ERR1_03,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE3 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_STARTUP_ERR1_04,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE4 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_STARTUP_ERR1_05,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE5 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_STARTUP_ERR1_06,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE6 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_STARTUP_ERR1_07,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE7 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_STARTUP_ERR1_08,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE8 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_STARTUP_ERR1_09,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE9 ),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_STARTUP_ERR1_10,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE10),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_STARTUP_ERR1_11,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE11),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_STARTUP_ERR1_12,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE12),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_STARTUP_ERR1_13,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE13),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_STARTUP_ERR1_14,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE14),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_STARTUP_ERR1_15,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE15),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_STARTUP_ERR1_16,       M_RRH_REG_CNT_STARTUP_ERR1(D_RRH_CPRINO_RE16),      D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_DL_NG_ERR_01,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE1 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_DL_NG_ERR_02,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE2 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_DL_NG_ERR_03,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE3 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_DL_NG_ERR_04,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE4 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_DL_NG_ERR_05,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE5 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_DL_NG_ERR_06,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE6 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_DL_NG_ERR_07,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE7 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_DL_NG_ERR_08,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE8 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_DL_NG_ERR_09,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE9 ),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_DL_NG_ERR_10,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE10),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_DL_NG_ERR_11,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE11),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_DL_NG_ERR_12,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE12),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_DL_NG_ERR_13,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE13),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_DL_NG_ERR_14,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE14),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_DL_NG_ERR_15,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE15),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_DL_NG_ERR_16,          M_RRH_REG_CNT_DL_NG_ERR(D_RRH_CPRINO_RE16),         D_RRH_REG_CNT_SV14INF, D_RRH_CPRINO_RE16},

    {E_SYS_SVCTL_3G_CARRRESET_ERR_01,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE1 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_02,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE2 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_03,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE3 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_04,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE4 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_05,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE5 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_06,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE6 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_07,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE7 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_08,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE8 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_09,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE9 ),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_3G_CARRRESET_ERR_10,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE10),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_11,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE11),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_12,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE12),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_13,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE13),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_14,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE14),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_15,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE15),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_3G_CARRRESET_ERR_16,   M_RRH_REG_CNT_3G_CARRRESET_ERR(D_RRH_CPRINO_RE16),  D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_01,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE1 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_02,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE2 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_03,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE3 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_04,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE4 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_05,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE5 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_06,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE6 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_07,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE7 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_08,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE8 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_09,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE9 ), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_10,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE10), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_11,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE11), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_12,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE12), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_13,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE13), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_14,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE14), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_15,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE15), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_S3G_CARRRESET_ERR_16,  M_RRH_REG_CNT_S3G_CARRRESET_ERR(D_RRH_CPRINO_RE16), D_RRH_REG_CNT_SV15INF, D_RRH_CPRINO_RE16},
	
    {E_SYS_SVCTL_STARTUP_ERR_S3G_01,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE1 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_02,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE2 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_03,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE3 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_04,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE4 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_05,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE5 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_06,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE6 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_07,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE7 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_08,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE8 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_09,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE9 ), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_STARTUP_ERR_S3G_10,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE10), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_11,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE11), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_12,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE12), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_13,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE13), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_14,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE14), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_15,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE15), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_STARTUP_ERR_S3G_16,    M_RRH_REG_CNT_S3G_RESTARTUP_ERR(D_RRH_CPRINO_RE16), D_RRH_REG_CNT_SV16INF, D_RRH_CPRINO_RE16},
    
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_01,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE1 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_02,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE2 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_03,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE3 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_04,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE4 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_05,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE5 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_06,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE6 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_07,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE7 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_08,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE8 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_09,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE9 ),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_10,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE10),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_11,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE11),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_12,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE12),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_13,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE13),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_14,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE14),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_15,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE15),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_3G_FAILSAFE_ERR_16,    M_RRH_REG_CNT_3G_FAILSAFE_ERR(D_RRH_CPRINO_RE16),   D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE16},
    
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_01,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE1 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE1 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_02,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE2 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE2 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_03,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE3 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE3 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_04,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE4 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE4 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_05,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE5 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE5 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_06,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE6 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE6 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_07,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE7 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE7 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_08,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE8 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE8 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_09,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE9 ),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE9 },
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_10,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE10),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE10},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_11,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE11),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE11},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_12,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE12),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE12},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_13,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE13),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE13},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_14,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE14),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE14},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_15,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE15),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE15},
    {E_SYS_SVCTL_S3G_FAILSAFE_ERR_16,   M_RRH_REG_CNT_S3G_FAILSAFE_ERR(D_RRH_CPRINO_RE16),  D_RRH_REG_CNT_SV5INF, D_RRH_CPRINO_RE16}
};

/**
 * @brief   他プロセスからのSV制御レジスタ設定用(D_API_MSGID_SVP_SVREG_SET_IND)
 * @param   srcThrdId [in] スレッドID
 * @param   wrAdr     [in] write register address
 * @param   clrBit    [in] clear bit position
 * @param   setBit    [in] set bit position
 * @date    2015/10/23 TDI)satou create
 * @date    2015/10/24 TDIPS)sasaki Warning対処
 */
void f_com_SVCtl_OtherProc(UINT srcThrdId, UINT wrAdr, UINT clrBit, UINT setBit)
{
    UINT idx, bitshift;
    UINT mask;

    for (bitshift = 0; bitshift < 32; bitshift++)
    {
        mask = 1 << bitshift;
        if (0 != (clrBit & mask))
        {
            for (idx = 0; idx < sizeof(tSvOptMapTbl) / sizeof(struct CMT_SVOpt_MapTbl); idx++)
            {
                if ((tSvOptMapTbl[idx].bit == mask) && (tSvOptMapTbl[idx].addr == wrAdr))
                {
                    f_com_SVCtl(srcThrdId, tSvOptMapTbl[idx].opt, D_SYS_OFF);
                }
            }
        }

        if (0 != (setBit & mask))
        {
            for (idx = 0; idx < sizeof(tSvOptMapTbl) / sizeof(struct CMT_SVOpt_MapTbl); idx++)
            {
                if ((tSvOptMapTbl[idx].bit == mask) && (tSvOptMapTbl[idx].addr == wrAdr))
                {
                    f_com_SVCtl(srcThrdId, tSvOptMapTbl[idx].opt, D_SYS_ON);
                }
            }
        }
    }
}

/**
 * @brief  set software alarm code at SV state register
 * @param  almcode [in] software alarm code
 * @return none
 * @date   2015/09/02 TDI)satou create
 */
void f_com_SVCtl_AlmCodeSet(UINT almcode) {
    INT rtn_bpf;                                            /* for BPF */
    INT errcd;                                              /* for BPF */
    const UINT sem_id = (UINT)E_RRH_SEMID_APL_SVCTL;        /* semaphore id */
    UINT regDat;                                            /* for register R/W */

    /* take semaphore of SV control */
    rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(sem_id, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
    if (BPF_RU_IPCM_OK != rtn_bpf) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SV CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
    }

    /* write register */
    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_SV7INF, &regDat);
    regDat &= ~0xFFFF;
    regDat |= (almcode & 0xFFFF);
    BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_CNT_SV7INF, &regDat);

    /* give semaphore of SV control */
    rtn_bpf = BPF_RU_IPCM_PROCSEM_GIVE(sem_id, BPF_RU_IPCM_LOCK_RW, &errcd);
    if (BPF_RU_IPCM_OK != rtn_bpf) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SV CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
    }
}
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */
/* @} */

