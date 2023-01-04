/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_getHardRegInfo.c
 * @brief DPDAタスク ハードレジスタ情報取得処理
 * @date  2014/02/25 ALPHA)中村 Create
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2020
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

static UINT f_dpdaw_l_upsigstop_count = 60;

/*!
 * @brief 関数機能概要:主信号停波履歴取得処理
 * @note  関数処理内容:主信号停波履歴取得処理
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/07/19 Taniguchi
 * @date 2020/01/06 FJT)吉田   DCM5GDU-053-191227 [19BD] : ハードログ情報追加
 */
VOID f_dpda_peri_get_upSigStopLog( VOID )
{
	UINT							index;
	UINT							uiAntCnt;
	T_RRH_LOG_UPSIGSTOP_HIS_RECORD*	recode_p;

	if( f_dpdaw_upsigstop_log->fault != D_RRH_OFF ){
		return;
	}

	/* 60s周期 */
	f_dpdaw_l_upsigstop_count++;
	if(f_dpdaw_l_upsigstop_count >= 60){
		f_dpdaw_l_upsigstop_count = 0;
	}else{
		return;
	}

	index = f_dpdaw_upsigstop_log->uiWriteIdx;

	recode_p = &(f_dpdaw_upsigstop_log->tRecord[index]);
	recode_p->validCode = 0;

	(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_IRQMACSTI, D_RRH_ON , &(recode_p->link_state));
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV4INF, &(recode_p->sv_fault_state));
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &(recode_p->sv_sync_state));

	for(uiAntCnt=0; uiAntCnt < f_dpdaw_ant_max_num ;uiAntCnt++){
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_DU_REG_TCX_TDANTEN(uiAntCnt), &(recode_p->tdd_en_state[uiAntCnt]));
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_DU_REG_CA_TXANTEN(uiAntCnt), &(recode_p->tddant_en_state[uiAntCnt]));
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_DU_REG_CA_TXCAREN(uiAntCnt), &(recode_p->txen_state[uiAntCnt]));
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_DU_REG_RX_RXCAREN(0,uiAntCnt), &(recode_p->rxen_state[uiAntCnt]));

	}

	UINT reg_index = 0;

	recode_p->regaddr[reg_index] = D_DU_REG_TCB_TDDMYON;
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
	reg_index++;

	recode_p->regaddr[reg_index] = D_DU_REG_TCX_TDHDOFFL;
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
	reg_index++;

	recode_p->regaddr[reg_index] = D_DU_REG_TCX_TDHDOFFT;
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
	reg_index++;

	UINT	regTblMaxNum = 0;

	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		recode_p->regaddr[reg_index] = 0xB1203020;
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
		reg_index++;

		recode_p->regaddr[reg_index] = 0xB1203024;
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
		reg_index++;

		recode_p->regaddr[reg_index] = 0xB1203028;
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));
		reg_index++;

		recode_p->regaddr[reg_index] = 0xB120302C;
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, recode_p->regaddr[reg_index], &(recode_p->regdata[reg_index]));

		regTblMaxNum = D_DPDA_REG_SUB6_NUM_MAX;
	}else{
		regTblMaxNum = D_DPDA_REG_MMW_NUM_MAX;
	}

	(void)BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF,0xA2401840,0x30,&(recode_p->rach_fft_mon[0]));
	(void)BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF,0xA2010C60,0x30,&(recode_p->dlp_mon[0]));
	(void)BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF,0xA2403400,0x30,&(recode_p->alm_mon[0]));
    recode_p->pa_status = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ];
	(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_TCB_TDDMYDSL09, &recode_p->pa_control_cnt);

	UINT	uiRegCnt	= 0;
	for(uiRegCnt=0; uiRegCnt < regTblMaxNum ;uiRegCnt++){
		recode_p->ant_reg[uiRegCnt] = f_dpdaw_regdataReal.regVal[uiRegCnt];
	}

	BPF_HM_DEVC_GETTIME(&(recode_p->sysTime));

	recode_p->validCode = 0x11111111;

	/* Fault? */
	UINT	faultIdbit = recode_p->sv_fault_state;
	UINT	faultICheckbit = (UINT)( D_DU_SVCTL_BIT_OCCUR_FID_10 | D_DU_SVCTL_BIT_OCCUR_FID_11 | D_DU_SVCTL_BIT_OCCUR_FID_12 ) ;

	if( ( faultIdbit & faultICheckbit) != D_RRH_OFF ){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Stop Uplane Sig Stop History by Fault(bit:0x%08x index:0x%08x)", faultIdbit ,f_dpdaw_upsigstop_log->uiWriteIdx );
		f_dpdaw_upsigstop_log->fault = faultIdbit;
	}

	index++;
	index %= D_DU_LOG_UPSIGSTOP_RCD_NUM;
	f_dpdaw_upsigstop_log->uiWriteIdx = index;

	return;
}

/*!
 * @brief 関数機能概要:DLF統計情報取得処理
 * @note  関数処理内容:DLF統計情報取得処理
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2020/07/08 FJT)水野  create
 */
VOID f_dpda_peri_get_dlf( VOID ){
	
	UINT dlfCnt;
	UINT dlf_GrpCnt;
	UINT regdata_d = 0;
	UINT regdata_min = 0;
	UINT regdata_pls = 0;
	UINT regaddr = 0xa2015600;
	UINT regindex;
	UINT grpindex = 0;
	UINT grpnum = 0;
	UINT grp_eachnum[D_DU_LOG_DLF_HIS_GRP_NUM] = {1,1,1,1,1,1,1,1,1,1,1,10,10,10,1};	/* -45°偏波カウンタGrp{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15} */
																						/* +45°偏波カウンタGrp{16,17,18,19,20,21,22,23,24,25,26,27,28,29,30} */
	

	/* DLF統計情報取得ラッチ */
	f_dpda_IO_write(0xa20143c4, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
	/* 60秒周期の0秒開始時刻を取得 */
	if(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time == 0){
		memset(&(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].s_date), D_RRH_ZERO, sizeof((f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].s_date)));
		BPF_HM_DEVC_GETTIME(&(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].s_date));
	}
	/* 0~60秒の停止時刻を取得 */
	memset(&(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].e_date), D_RRH_ZERO, sizeof((f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].e_date)));
	BPF_HM_DEVC_GETTIME(&(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].e_date));

	/* レジスタ読み取り([15:0]-45°: [32:16]+45°偏波カウンタ) */
	for(regindex = 0; regindex < D_DU_LOG_DLF_HIS_ADD_NUM; regindex++){
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regdata_d);
		/* レジスタ取得 [15:0]-45° 偏波カウンタ*/
		regdata_min = regdata_d & 0x0000FFFF;
		/* レジスタ取得 [32:16]+45° 偏波カウンタ*/
		regdata_pls = regdata_d >> 16;
		/* 各グループ最大値合算 -45°偏波カウンタ Grp1~15 */
		f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].regdat_sum[D_DPDA_ANT_AC][grpindex] += regdata_min;
		/* 各グループ最大値合算 +45°偏波カウンタ Grp16~30 */
		f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].regdat_sum[D_DPDA_ANT_BD][grpindex] += regdata_pls;
		/* 各グループ数が最大になった時次グループの合計値計算 */
		grpnum++;
		if(grpnum >= grp_eachnum[grpindex]){
			grpindex++;
			grpnum = 0;
		}
		regaddr += D_DU_LOG_DLF_HIS_ADD_INC;
	}
	/* 偏波毎 かつ 42種別毎に60秒分(60回分)を計算 */
	f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time++;
	if(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time >= D_DU_LOG_DLF_HIS_TIME_MIN){
		/* 次の60秒周期の統計情報取得 */
		f_dpdaw_dlf_log->uiWriteIdx++;
		/* DLF統計情報最終レコード取得時、次の周回へ以降する */
		if(f_dpdaw_dlf_log->uiWriteIdx >= D_DU_LOG_DLF_HIS_TIME){
			f_dpdaw_dlf_log->uiWriteIdx = 0;
			f_dpdaw_dlf_log->dlffin_flag = D_RRH_ON;
		}
		/* 2周目以降は次レコードを初期化する */
		if(f_dpdaw_dlf_log->dlffin_flag == D_RRH_ON){
			f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time = 0;
			for(dlfCnt = 0; dlfCnt < D_RRH_POLARIZATION_NUM; dlfCnt++){
				for(dlf_GrpCnt = 0; dlf_GrpCnt < D_DU_LOG_DLF_HIS_GRP_NUM; dlf_GrpCnt++){
					f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].regdat_sum[dlfCnt][dlf_GrpCnt] = 0;
				}
			}
		}
		
	}
	return;
}

/*!
 * @brief 関数機能概要:ハードレジスタ情報取得処理を行う
 * @note  関数処理内容.
 *		-# レジスタ情報取得
 *		-# SV情報取得
 *		-# 温度情報取得
 *		-# DLF統計情報取得
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/25 ALPHA)中村 Create
 * @date 2015/04/28 ALPHA)藤原 TDD対応
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * @date 2015/11/06 ALPHA)藤原 TDD-RRE(Zynq)対応 レジスタ情報収集処理を修正
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 * @date 2020/07/08 FJT)水野   DLF統計情報取得処理追加
 */
VOID f_dpda_peri_getHardRegInfo()
{
	UINT							uiRegCnt	= 0;
	UINT							uiRegVal	= 0;
	INT								iTempVal	= 0;
	UINT							*regTblArray;
	UINT							regTblMaxNum = 0;
	UINT							uiAntCnt = 0;
	UINT							uiRegVal_ref1	= 0;
	UINT							uiRegVal_ref2	= 0;
	UINT							uiRegVal_fb1	= 0;
	UINT							uiRegVal_fb2	= 0;
	UINT							uiVerifyCnt		= 0;

	/************************************************/
	/* ラッチ処理									*/
	/************************************************/
	/* ADC受信電力報告のキャリアRTWP値保持レジスタ(RXPOWLAT),		*/
	/*	キャリア受信電力報告(RTWP)のキャリアRTWP値保持レジスタ(RTWPLAT)それぞれ1をライトする					*/
	/* ラッチ対象には系単位であるものと、システムで一つのものがあるため、各々適した形で処理する					*/
	/* 系(アンテナ)単位で持つもの */
	
	
	switch(f_dpdaw_reKind)
	{
		/* sub6の場合 */
		case E_DPDA_REKIND_5GDU_SUB6:
		
			//アンテナ送信電力 POWLAT
			f_dpda_IO_write(0xB12030C0, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB12034C0, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB12038C0, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1203CC0, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			
			//ADC受信電力 RXPOWLAT
			f_dpda_IO_write(0xB1300238, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300638, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300A38, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300E38, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			
			//キャリア受信電力報告 RTWP
			f_dpda_IO_write(0xB1300174, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300574, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300974, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300D74, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			break;
		
		/* mmWの場合 */
		case E_DPDA_REKIND_5GDU_MMW:

			/* Carrier電力ラッチ設定 */
			//TSSI BTI_TSSILAT
			f_dpda_IO_write(0xB1100124, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);

			//アンテナ送信電力 POWLAT
			f_dpda_IO_write(0xB11002F4, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
		
			//ADC受信電力 RXPOWLAT
			f_dpda_IO_write(0xB1300238, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300638, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
		
			//キャリア受信電力報告 RTWP
			f_dpda_IO_write(0xB1300174, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);
			f_dpda_IO_write(0xB1300574, 0x00000001, E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE_POL);

			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. reKind = %d.", f_dpdaw_reKind );
			return;
	}
	
	
	/************************************************/
	/* レジスタ情報収集								*/
	/************************************************/
	/* 装置種別で処理を分ける */
	switch(f_dpdaw_reKind)
	{
		/* sub6の場合 */
		case E_DPDA_REKIND_5GDU_SUB6:
			regTblArray = (UINT *)f_dpdar_reg_Tbl;		//f_dpda_rom.cで宣言されたUINT型の配列 各要素にアドレスが格納されている 要素数:D_DPDA_REG_SUB6_NUM_MAX
			regTblMaxNum = D_DPDA_REG_SUB6_NUM_MAX;		
			break;
		
		/* mmWの場合 */
		case E_DPDA_REKIND_5GDU_MMW:
			regTblArray = (UINT *)f_dpdar_reg_Tbl_mmW;	//f_dpda_rom.cで宣言されたUINT型の配列 各要素にアドレスが格納されている 要素数:D_DPDA_REG_MMW_NUM_MAX
			regTblMaxNum = D_DPDA_REG_MMW_NUM_MAX;		
			
			/************************************/
			/*	mmW装置のALM判定用に追加で収集	*/
			/************************************/
			f_dpdaw_extdataReal.regVal[0] = f_dpda_IO_read(0x6e,  E_DPDA_BPF_RFPLL_MODE, D_RRH_LOG_REG_LV_READ);
			f_dpdaw_extdataReal.regVal[1] = f_dpda_IO_read(0x281, E_DPDA_BPF_TXDAC_MODE, D_RRH_LOG_REG_LV_READ);
			f_dpdaw_extdataReal.regVal[2] = f_dpda_IO_read(0x7b5, E_DPDA_BPF_TXDAC_MODE, D_RRH_LOG_REG_LV_READ);
			f_dpdaw_extdataReal.regVal[3] = f_dpda_IO_read(0x56f, E_DPDA_BPF_RXADC_MODE, D_RRH_LOG_REG_LV_READ);
			f_dpdaw_extdataReal.regVal[4] = f_dpda_IO_read(0x11b, E_DPDA_BPF_RXADC_MODE, D_RRH_LOG_REG_LV_READ);
			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. reKind = %d.", f_dpdaw_reKind );
			return;
	}
	
	for( uiRegCnt = 0; uiRegCnt < regTblMaxNum; uiRegCnt++ )
	{
		uiRegVal = f_dpda_IO_read( regTblArray[uiRegCnt],
								   E_DPDA_BPF_NOMAL_MODE,
								   D_RRH_LOG_REG_LV_READ );
		
		f_dpdaw_regdataReal.regVal[uiRegCnt] = uiRegVal;
	}

	/* Tx Gain ALM Fail Safe */
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* ANT毎にRef,FBの正当性をチェックする	      */
		/* ２回連続リードしてそれぞれが一致するかチェックする */
		/* 読み出す順番はRef->FB Ref->FB とする       */
		/* 最大10回リトライとする                     */
		for(uiAntCnt=0; uiAntCnt < D_DPDA_ANTMAX ; uiAntCnt++){
			for( uiVerifyCnt = 0 ; uiVerifyCnt < 10 ; uiVerifyCnt++ ){

				uiRegVal_ref1 = f_dpda_IO_read( M_DU_REG_FBANT_POWREFSW(uiAntCnt),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
				uiRegVal_fb1  = f_dpda_IO_read( M_DU_REG_FBANT_POWFBKSW(uiAntCnt),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );

				uiRegVal_ref2 = f_dpda_IO_read( M_DU_REG_FBANT_POWREFSW(uiAntCnt),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
				uiRegVal_fb2  = f_dpda_IO_read( M_DU_REG_FBANT_POWFBKSW(uiAntCnt),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );

				/* 一致してるかチェックする */
				if((uiRegVal_ref1 == uiRegVal_ref2) && (uiRegVal_fb1 == uiRegVal_fb2)){
					/* Ref, FB変化なし(切替えなし) */
					break;
				}else{
					/* Ref, FB変化あり(切替えと衝突) */
					BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "Ant-%d(%d) Mismatch ref1 0x%08x ref2 0x%08x fb1 0x%08x fb2 0x%08x", uiAntCnt, uiVerifyCnt,uiRegVal_ref1,uiRegVal_ref2, uiRegVal_fb1 ,uiRegVal_fb2);
					f_com_taskDelay(5,0,0,0);
				}
			}
			if(uiVerifyCnt >= 10){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Ant-%d(%d) Mismatch retry out ref1 0x%08x ref2 0x%08x fb1 0x%08x fb2 0x%08x", uiAntCnt, uiVerifyCnt,uiRegVal_ref1,uiRegVal_ref2, uiRegVal_fb1 ,uiRegVal_fb2);
				uiRegVal_ref2 = 0;
				uiRegVal_fb2 = 0;
			}
			f_dpdaw_regdataReal.regVal[ f_dpdaw_index_RefPowPos + uiAntCnt ] = uiRegVal_ref2;
			f_dpdaw_regdataReal.regVal[ f_dpdaw_index_FbPowPos  + uiAntCnt ] = uiRegVal_fb2;
		}
	}

	/************************************************/
	/* 温度情報:温度センサ(TMP112)					*/
	/*  レジスタ値12bit signed						*/
	/*  レジスタ値に0.0625かけた値が温度となる		*/
	/************************************************/
	iTempVal = f_dpda_IO_read( 0, E_DPDA_BPF_TMP_MODE2, D_RRH_LOG_REG_LV_READ );
	/* 無効時(0x7FFFFFFF)は前値保持し、有効時のみ設定する */
	if( iTempVal != 0x7FFFFFFF ){
		f_dpdaw_statusInfo.Temperature = iTempVal;
	}


	/* 主信号停波ログ取得 */
	f_dpda_peri_get_upSigStopLog();

	/* DLF統計情報取得 */
	f_dpda_peri_get_dlf();

	return;
}

/* @} */
