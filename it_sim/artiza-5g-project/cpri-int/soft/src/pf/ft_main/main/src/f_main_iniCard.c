/*!
 * @skip		$ld:$
 * @file		f_main_iniCard.c
 * @brief		pf_main ハードウェア初期化処理
 * @date  		2015/06/09 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"
#include "RFIC_PMC.h"


/*!
 * @brief		f_main_iniDiag1st
 * @note		1次診断処理
 *  SRAMのアクセスをするために初期でアクセスする必要があるため実施する
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2018/09/21 FJT)Taniguchi Create
 */
UINT f_main_iniDiag1st_HW(UINT ptn )
{
	UINT		reg_data_wcheck;
	UINT		reg_data_rcheck;
	UINT		reg_addr;
	UINT		reg_data_wcheck_sram;

	reg_data_wcheck = ptn;

	/* PL FPGA check */
	reg_addr = D_DU_REG_CTB_REGCHK;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_addr, &reg_data_wcheck);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-PL ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_PL);
		return	D_SYS_NG;
	}

	reg_addr = D_DU_REG_CTX_REGCHK;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_addr, &reg_data_wcheck);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-TRX1 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_TRX1);
		return	D_SYS_NG;
	}

	reg_data_wcheck_sram = reg_data_wcheck & 0x0000FFFF;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_0, &reg_data_wcheck_sram);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_1, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_2, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_3, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_4, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_5, &reg_data_wcheck_sram);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_6, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_7, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_8, &reg_data_wcheck);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SRAM0_CHK_9, &reg_data_wcheck);

	reg_addr = D_DU_REG_OTH_SRAM0_CHK_0;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck_sram != (reg_data_rcheck& 0x0000FFFF)){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck_sram,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_1;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_2;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_3;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_4;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_5;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck_sram != (reg_data_rcheck & 0x0000FFFF)){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck_sram,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_6;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_7;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_8;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	reg_addr = D_DU_REG_OTH_SRAM0_CHK_9;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &reg_data_rcheck);
	if(reg_data_wcheck != reg_data_rcheck){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "1stDiagNG-SRAM0 ADD:0x%x W:0x%x R:0x%x",reg_addr,reg_data_wcheck,reg_data_rcheck);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DIAG_NG_SRAM1);
		return	D_SYS_NG;
	}
	return	D_SYS_OK;
}

UINT f_main_iniDiag1st(VOID)
{
	/********************************/
	/* 一次診断						*/
	/********************************/
	UINT		fret = D_SYS_OK;

	/* SRAM */
	fret = f_main_iniDiag1st_HW(0x55555555);
	if(fret == D_SYS_OK){
		fret = f_main_iniDiag1st_HW(0xAAAAAAAA);
	}
	return fret;
}
	
/*!
 * @brief		f_main_iniCard
 * @note		ハードウェアの初期設定
 *					-# TRX-FPGA1/2 Config完了確認チェック
 *					-# AXI Timer初期設定(skip)
 *					-# SWD起動(skip)
 *					-# PL配下QSPI(IP) 初期化
 *					-# LLB/TRX PCIe起動 (TRX-FPGA1)
 *						-# PCIe Root Complex 設定
 *						-# PCIe Endpint設定
 *						-# PCIe Interrupt mask CLR
 *						-# PCIe connect CNF
 *					-# LLB/TRX PCIe起動 (TRX-FPGA2)
 *						-# PCIe Root Complex 設定
 *						-# PCIe Endpint設定
 *						-# PCIe Interrupt mask CLR
 *						-# PCIe connect CNF
 *					-# CTB STAマスク解除
 *					-# SRAM cnt init CNF
 *					-# PL-TRX SV-IF確認
 *					-# TRX1 CTX STAマスク解除
 *					-# TRX1 PL SV-IF CNF
 *					-# TRX2 CTX STAマスク解除
 *					-# TRX2 PL SV-IF CNF
 *					-# CTB SFP STAマスク解除
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/09 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date  		2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq
 * @date		2015/10/27 ワイシス) kawano PAPS Drain電圧値設定修正
 */
UINT f_main_iniCard()
{
	int			ret;
	int			i;
	UINT		fret = D_SYS_OK;
	UINT		regw_data;
	UINT		regr_data;
	struct timespec		tDelayTime;		/*	遅延時間					*/
	struct timespec		tRestTime;		/*	残り時間					*/
	UCHAR			busycount;
// 擬似環境 20210316
//#ifdef SIM_DEBUG
	return D_SYS_OK;//C9
//#endif
// 擬似環境 20210316
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniCard] ENTER" );

	/********************************/
	/* TRX-FPGA1 Config完了確認	*/
	/********************************/
	for( i = 0; i < D_MAIN_CTB_CFG_WAIT_CNT; i++ ) {

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_CFG, &regr_data);
		/*	bit[24]=0,[8]=1	*/
		if( (regr_data & D_MAIN_CTB_CFG_MASK) == D_MAIN_CTB_CFG_EXPECT ) {
			break;
		}
		/* 100ms wait	*/
		/* 5秒wait (1s×5回)	*/
		/*	タイマを起動	*/
		tDelayTime.tv_sec  = 0;
		tDelayTime.tv_nsec = (D_MAIN_1MS_WAIT * 100);
		(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );
	}
	if( i >= D_MAIN_CTB_CFG_WAIT_CNT ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "<%d> Times TRX-FPGA#n ConfigNG<0x%08x>", i, regr_data);
		/*	Fail detect	*/
		/*	TRX-FPGA#1 ConfigがDONEでない場合	*/
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_CONF_NG_TRX1);
		return	D_SYS_NG;
	}

	regw_data = D_DU_REG_OTH_RESET_REASON_CLR;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_RESET_REASON, &regw_data);

	(VOID)BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_STUP_BUSY_COUNTER, &busycount);
	if(busycount > 0){

		/* Roll back時(busycount==2)かつ逆面のslot statusがVALIDの場合、*/
		/* 逆面を強制的にINVALIDに変更しCUからの再ダウンロードを促す      */
		if(busycount == 2){
			UCHAR slot_face;
			if(f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0){
				slot_face = D_RRH_BOOT_ACT1;
			}else{
				slot_face = D_RRH_BOOT_ACT0;
			}
			if( f_cmw_tra_inv_tbl->cmx_slot_status[slot_face] == D_DU_EEP_STATUS_VALID ){
				UCHAR slot_status = D_DU_EEP_STATUS_INVALID ;

				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"Roll back StartUp !!! slot(%d) status was %d -> INVALID", 
								slot_face,f_cmw_tra_inv_tbl->cmx_slot_status[slot_face]);

				f_cmw_tra_inv_tbl->cmx_slot_status[slot_face] = slot_status ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SWSLOT_STATUS + slot_face, &slot_status);
			}else{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"Roll back StartUp !!! already slot(%d) status was %d", 
								slot_face,f_cmw_tra_inv_tbl->cmx_slot_status[slot_face]);
			}

		}else{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"StartUp Busy Counter Clear(%d)",  busycount );
		}

		busycount = 0;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_STUP_BUSY_COUNTER, &busycount);

	}

	/********************************/
	/* AXI Timer + SWD起動			*/
	/********************************/
	BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_SET, D_SYS_SWDT_TIMER_SET );

	/********************************/
	/* PL配下QSPI(IP) 初期化		*/
	/********************************/
	ret = BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT();
	if((ret = BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT()) != BPF_HM_DEVC_COMPLETE){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT error=%d",ret);
		fret = D_SYS_NG;
	}

	/********************************/
	/* 温度センサ初期化				*/
	/********************************/
	BPF_HM_DEVC_TEMP_INIT();

	/********************************/
	/* FreqID						*/
	/********************************/
	regw_data = 0xFF;
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);

	UCHAR	device_kind1;
	BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &device_kind1);

	regw_data = (UINT)device_kind1;
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);

	/********************************/
	/* LLB/TRX PCIe起動 (TRX-FPGA1)	*/
	/********************************/
	/*	PCIe ROOT/END reset CLR	*/
	regw_data = D_MAIN_SET_REL_RESET_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);

	/* 100ms wait	*/
	tDelayTime.tv_sec  = 0;
	tDelayTime.tv_nsec = (D_MAIN_1MS_WAIT * 100);
	(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );

	/*	CTB STA1 mask 	*/
	regw_data = D_MAIN_SET_STA1_RESET_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_STA1M, regw_data);

	/*	dummy read 	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regr_data);
	
	/*	TRX1 PCIe Ready CHK 	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regr_data);
	if( (regr_data & D_MAIN_SET_STA1_RESET_TRX1) != 0x00000000 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "TRX-FPGA#1 PCIe NotReady<0x%08x>", regr_data);
		/*	Fail detect	*/
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG1_TRX1);
		return	D_SYS_NG;
	}
	/*--------------------------------------*/
	/* PCIe Root Complex 設定(TRX-FPGA1)	*/
	/*--------------------------------------*/
	/*	IRQ Mask SET 	*/
	regw_data = 0x00000000;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_IRQ_MASK_TRX1, &regw_data);

	/*	IRQ Decode SET 	*/
	regw_data = D_MAIN_SET_IRQ_SET_DECODE_TRX1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_IRQ_SET_DECODE_TRX1, &regw_data);

	i = 0;
	while(1) {

		/*	PHY status Read		*/
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_PHY_STATE_TRX1, &regr_data);
		tDelayTime.tv_sec  = 0;
		tDelayTime.tv_nsec = (D_MAIN_1MS_WAIT * 10);
		(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );

		/*	PHY Link UP CNF(NG)		*/
		if((regr_data & D_MAIN_SET_PHY_STATE_VAL) != D_MAIN_SET_PHY_STATE_VAL) {

			i++;
			/*	NG count more than 3 times		*/
			if( i >= D_MAIN_VAL_PHY_STATE_CNT ) {
				/*	Fail detect	*/
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "<%d> Times TRX-FPGA#1 PCIe PHY Link UP NG<0x%08x>", i, regr_data);
				f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG2_TRX1);
				return	D_SYS_NG;
			/*	NG count less than 3 times		*/
			} else {
				/*	PCIe ROOT reset and reset clr	*/
				regw_data = D_MAIN_SET_REL_RESET_TRX1;
				BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);
				BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);
			}
			continue;
		/*	PHY Link UP CNF(OK)		*/
		} else {
			/*	Command Reg SET		*/
			regw_data = D_MAIN_SET_COMMAND_TRX1;
			BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_COMMAND_TRX1, regw_data);

			/*	Second Lat Timer SET		*/
			regw_data = D_MAIN_SET_SEC_LET_TIMER_TRX1;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_SEC_LET_TIMER_TRX1, &regw_data);
		}
		break;
	}
	/*--------------------------------------*/
	/* PCIe Endpint設定(TRX-FPGA1)			*/
	/*--------------------------------------*/
	/*	BIST/Header/Lat timer/CacheLn Read	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_BHLC_TRX1, &regr_data);

	/*	Bit mask result CNF	*/
	if( (regr_data & D_MAIN_SET_BHLC_TRX1) != 0 ) {
		/*	Fail detect	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "TRX-FPGA#1 PCIe Bit mask result<0x%08x>", regr_data);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG3_TRX1);
		return	D_SYS_NG;
	}
	/*	Satus/Command Reg SET	*/
	regw_data = D_MAIN_SET_STATE_COMMAND_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_STATE_COMMAND_TRX1, regw_data);

	/*	Base Address SET	*/
	regw_data = D_MAIN_SET_BASE_ADDR_TRX1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_BASE_ADDR_TRX1, &regw_data);

	/*	Root port Status/Control SET	*/
	regw_data = D_MAIN_SET_ROOT_PORT_STA_CNT_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_ROOT_PORT_STA_CNT_TRX1, regw_data);

	/*--------------------------------------*/
	/* PCIe Interrupt mask CLR(TRX-FPGA1)	*/
	/*--------------------------------------*/
	/*	IRQ clr SET	*/
	regw_data = D_MAIN_SET_IRQ_SET_DECODE_TRX1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_IRQ_SET_DECODE_TRX1, &regw_data);

	/*	mask clr SET	*/
	regw_data = D_MAIN_SET_IRQ_MASK_TRX1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_IRQ_MASK_TRX1, &regw_data);

	/*--------------------------------------*/
	/* PCIe connect CNF(TRX-FPGA1)			*/
	/*--------------------------------------*/
	/*	TRX-FPGA CTX reaset CLR	*/
	regw_data = D_MAIN_SET_REL_RESET2_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_REL_RESET, regw_data);

	/* PCI確立 */
	BPF_HM_DEVC_PL_CONFIG_STATE_SET(D_RRH_ON);

	/*	REGCHK pattern1(Write)	*/
	regw_data = D_MAIN_SET_CHKREG_PTN1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_CHKREG_TRX1, &regw_data);
	/*	REGCHK pattern1(Read)	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_CHKREG_TRX1, &regr_data);

	/*	REGCHK pattern1(Confirm)	*/
	if( regr_data != D_MAIN_SET_CHKREG_PTN1 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "TRX-FPGA#1 PCIe REGCHK pattern1<0x%08x>", regr_data);
		/*	Fail detect	*/
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG4_TRX1);
		return	D_SYS_NG;
	}
	/*	REGCHK pattern2(Write)	*/
	regw_data = D_MAIN_SET_CHKREG_PTN2;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_CHKREG_TRX1, &regw_data);
	/*	REGCHK pattern2(Read)	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_CHKREG_TRX1, &regr_data);

	/*	REGCHK pattern2(Confirm)	*/
	if( regr_data != D_MAIN_SET_CHKREG_PTN2 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "TRX-FPGA#1 PCIe REGCHK pattern2<0x%08x>", regr_data);
		/*	Fail detect	*/
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG5_TRX1);
		return	D_SYS_NG;
	}
	/*	Upper End IRQ clr SET	*/
	regw_data = D_MAIN_SET_UPPER_END_IRQ_CLR;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_UPPER_END_IRQ_CLR_TRX1, &regw_data);

	/*	Upper End IRQ mask clr SET	*/
	regw_data = D_MAIN_SET_UPPER_END_IRQ_MSK;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_OTH_UPPER_END_IRQ_MSK_TRX1, &regw_data);

	/*	CTB STA1 IRQ mask CLR	*/
	regw_data = D_MAIN_SET_STA1_RESET2_TRX1;
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_STA1M, regw_data);

	/*	1ms wait	*/
	tDelayTime.tv_sec  = 0;
	tDelayTime.tv_nsec = D_MAIN_1MS_WAIT;
	(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );
	/*	PCIe ERR CNF	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regr_data);
	if( (regr_data & D_MAIN_SET_STA1_RESET2_TRX1) != 0x00000000 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "TRX-FPGA#1 PCIe ERR<0x%08x>", regr_data);
		/*	Fail detect	*/
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PCIE_EST_NG6_TRX1);
		return	D_SYS_NG;
	}

	/********************************/
	/* CTB STAマスク解除			*/
	/********************************/
	regw_data = D_MAIN_SET_CTB_STA1M;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_STA1M, &regw_data);

	regw_data = D_MAIN_SET_CTB_STA2M;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_STA2M, &regw_data);

	/********************************/
	/* CTB STAダミーリード			*/
	/********************************/

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regw_data);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "CTB_STA1 dummy read :0x%08x", regw_data );

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA2, &regw_data);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "CTB_STA2 dummy read :0x%08x", regw_data );

	/********************************/
	/* 診断処理						*/
	/********************************/
	if(fret == D_SYS_OK){
		fret = f_main_iniDiag1st();
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "f_main_iniDiag1st END(ret=%d)", fret );

	/********************************/
	/* SRAM cnt init CNF			*/
	/********************************/
	i = 0;
	while(1) {

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA2, &regr_data);
		/*	bit[24]=0 chk	*/
		if( (regr_data & D_MAIN_SET_CTB_STA2) != 0x00000000 ) {
			i++;
			if( i >= D_MAIN_CTB_STA2_WAIT_CNT ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "<%d> Times SRAM CHK NG<0x%08x>", i, regr_data);
				/*	Fail detect	*/
				f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_CNT_INIT_NG_SRAM1);
				return	D_SYS_NG;
			}
			tDelayTime.tv_sec  = 0;
			tDelayTime.tv_nsec = D_MAIN_1MS_WAIT;
			(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );
			continue;
		}
		break;
	}

	/********************************/
	/* PL-TRX SV-IF確認				*/
	/********************************/
	i = 0;
	while(1) {

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_STA1, &regr_data);
		/*	bit[24]=0 chk	*/
		if( (regr_data & D_MAIN_SET_CTB_STA1) != 0x00000000 ) {
			i++;
			if( i >= D_MAIN_CTB_STA1_WAIT_CNT ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "<%d> Times TRX-FPGA#1 PL-TRX SV-IF CHK NG<0x%08x>", i, regr_data);
				/*	Fail detect	*/
				f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_TRX1_PL_SVIF_NG);
				return	D_SYS_NG;
			}
			tDelayTime.tv_sec  = 0;
			tDelayTime.tv_nsec = ( D_MAIN_1US_WAIT * 100);
			(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );
			continue;
		}
		break;
	}
	/********************************/
	/* TRX1 CTX STAマスク解除		*/
	/********************************/
	regw_data = D_MAIN_SET_PL_SVIF_CNF;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_STA1M, &regw_data);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_STA2M, &regw_data);

	/*	dummy read	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTX_STA1, &regr_data);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "CTX_STA1 dummy read :0x%08x", regw_data );

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTX_STA2, &regr_data);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "CTX_STA2 dummy read :0x%08x", regw_data );


	/********************************/
	/* TRX1 PL SV-IF CNF			*/
	/********************************/
	i = 0;
	while(1) {

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTX_STA1, &regr_data);
		/*	bit[28]=00 chk	*/
		if( (regr_data & D_MAIN_SET_PL_SVIF_CHK) != 0x00000000 ) {

			i++;
			if( i >= D_MAIN_PL_SVIF_CHK_CNT ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "<%d> Times TRX1 PL SV-IF CHK NG<0x%08x>", i, regr_data);
				/*	Fail detect	*/
				f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_PL_TRX1_SVIF_NG);
				return	D_SYS_NG;
			}
			tDelayTime.tv_sec  = 0;
			tDelayTime.tv_nsec = D_MAIN_1MS_WAIT;
			(void)BPF_RU_ITCM_THREAD_DELAY( &tDelayTime, 1, &tRestTime );
			continue;
		}
		break;
	}
	/********************************/
	/* CTB SFP STAマスク解除		*/
	/********************************/
	regw_data = D_MAIN_SET_CTB_SFPSTA2M;
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SFPSTAM, regw_data);

	/********************************/
	/* CTB AISG 割り込みマスク解除	*/
	/********************************/
	regw_data = D_MAIN_SET_CTB_IRQAISGM;
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_IRQAISGM, regw_data);
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniCard] RETURN" );
	return fret;
}

/* @} */
