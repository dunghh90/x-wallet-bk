/*!
 * @skip  $ld:$
 * @file  f_dpda_ftyp.h
 * @brief DPDAタスク プロトタイプ宣言
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2011/09/20 ALPHA)藤井 SRRU-2.1G-EVDO対応
 * @date  2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00714 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00716 対処
 * @date  2011/11/08 ALPHA)横山 M-RRU-ZSYS-00721 対処
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/16 ALPHA)横山 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/18 ALPHA)横山 M-RRU-ZSYS-00721 対処(TSSI報告要否判定変更)
 * @date  2011/12/06 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date  2011/12/14 ALPHA)高橋 SRRU-2.1G-LTE対応
 * @date  2012/01/10 ALPHA)高橋 M-RRU-ZSYS-00766対処
 * @date  2012/01/12 ALPHA)横山 M-RRU-ZSYS-00775対処
 * @date  2012/01/20 ALPHA)横山 M-RRU-ZSYS-00788対処
 * @date  2012/01/23 ALPHA)藤井 M-RRU-ZSYS-00792対処
 * @date  2012/01/26 ALPHA)藤井 M-RRU-ZSYS-00798対処
 * @date  2012/01/30 ALPHA)横山 M-RRU-ZSYS-00801対処
 * @date  2012/01/30 ALPHA)横山 M-RRU-ZSYS-00800対処
 * @date  2012/02/08 ALPHA)横山 M-RRU-ZSYS-00807対処
 * @date  2012/03/16 ALPHA)高橋 N21GLTE対応
 * @date  2012/05/10 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00871対処
 * @date  2012/06/14 ALPHA)横山 M-RRU-ZSYS-00887対処
 * @date  2012/12/18 ALPHA)横山 M-RRU-ZSYS-00921対処
 * @date  2012/12/21 ALPHA)横山 M-RRU-ZSYS-00925対処
 * @date  2013/01/10 ALPHA)横山 M-RRU-ZSYS-00930対処
 * @date  2013/04/03 ALPHA)中村 ST800対応
 * @date  2013/11/19 ALPHA)加藤 ZYNQ対応
 * @date  2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応
 * @date  2015/11/05 ALPHA)藤原 TDD-RRE(Zynq)対応(PA周りの修正)
 * @date  2018/09/08 KCN)吉木   5G-DU対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_FTYP_H
#define F_DPDA_FTYP_H

/*!
 * @name DPDAタスク
 * @note プロトタイプ宣言
 * @{
 */

extern	VOID f_dpda_main();

extern	INT  f_dpda_init(VOID*);

extern	INT  f_dpda_anlz(VOID*);
extern	INT  f_dpda_abort(VOID*);

extern	UINT f_dpda_IO_read(UINT uiRegAddr, UINT uiReadMode, UINT uiLogLvl);
extern	VOID f_dpda_IO_write(UINT uiRegAddr, UINT uiWriteDat, UINT uiWriteMode, UINT uiLogLvl);

extern	VOID f_dpda_init_taskTable();
extern	VOID f_dpda_init_reg();
extern	VOID f_dpda_init_reg_tddsw();
extern	VOID f_dpda_init_unMask();

/* タスク処理 */
extern	INT	f_dpda_act_APDWaveOnOffRequest(VOID*);
extern	INT	f_dpda_act_sendPowerAdjRequest(VOID*);
extern	INT	f_dpda_act_sendPowerSaveRequest(VOID*);
extern	INT	f_dpda_act_sysParaSetRequest(VOID*);

extern	INT	f_dpda_act_noModCarSettingRequest(VOID*);

extern	INT f_dpda_act_iqSettingRequest(VOID*);
extern	INT f_dpda_act_carrierOnSettingRequest(VOID*);
extern	VOID f_dpda_act_cardUseSettingRequest(UINT uiAntNum, UINT uiCarNum);
extern	int f_dpda_act_txCarrierOnSettingRequest(unsigned int uiAntNum,
                                                 unsigned int uiCarrier,
                                                 unsigned int uiAccellId,
                                                 unsigned int uiCcId,
                                                 unsigned int uiDuPortId,
                                                 unsigned int uiCrCentFreq,
                                                 unsigned int uiUnitType,
                                                 unsigned int uiBw,
                                                 unsigned int uiScs,
                                                 long long int gain
                                                 ); 

extern	int f_dpda_act_rxCarrierOnSettingRequest(unsigned int uiAntNum,
                                                 unsigned int uiCarrier,
                                                 unsigned int uiAccellId,
                                                 unsigned int uiCcId,
                                                 unsigned int uiDuPortId,
                                                 unsigned int uiCrCentFreq,
                                                 unsigned int uiUnitType,
                                                 unsigned int uiBw,
                                                 unsigned int uiScs); 

extern int f_dpda_act_txCarrierOffSettingRequest(unsigned int uiAntNum,
                                                 unsigned int uiCarrier,
                                                 unsigned int uiCrCentFreq,
                                                 unsigned int uiUnitType);

extern int f_dpda_act_rxCarrierOffSettingRequest(unsigned int uiAntNum,
                                                 unsigned int uiCarrier,
                                                 unsigned int uiUnitType);

extern int f_dpda_act_testtxCarrierOnSettingRequest(unsigned int uiAntNum,
                                                    unsigned int uiCarrier,
                                                    unsigned int uiAccellId,
                                                    unsigned int uiCcId,
                                                    unsigned int uiDuPortId,
                                                    unsigned int uiCrCentFreq,
                                                    unsigned int uiUnitType,
                                                    unsigned int uiBw,
                                                    unsigned int uiScs); 

extern int f_dpda_act_testtxCarrierOffSettingRequest(unsigned int uiAntNum,
                                                     unsigned int uiCarrier,
                                                     unsigned int uiCrCentFreq,
                                                     unsigned int uiUnitType);

extern int f_dpda_act_txCarrierSleepSettingRequest(unsigned intuiAntNum, unsigned intuiCarrier);
extern int f_dpda_act_rxCarrierSleepSettingRequest(unsigned intuiAntNum, unsigned intuiCarrier);
extern int f_dpda_act_txCarrierSleepOffSettingRequest(unsigned intuiAntNum, unsigned intuiCarrier);
extern int f_dpda_act_rxCarrierSleepOffSettingRequest(unsigned intuiAntNum, unsigned intuiCarrier);
extern	INT f_dpda_act_carrierOffSettingRequest(VOID*);
extern	INT f_dpda_act_txrxCal_inUse(VOID*);
extern	INT f_dpda_act_txrxCalTraining(VOID*);

extern	INT f_dpda_act_paControlRequest(VOID*);

extern	INT f_dpda_act_powReportPeriodic(VOID*);

extern	INT f_dpda_act_periodicChk(VOID*);

extern	INT f_dpda_act_periodicChk_rre(VOID*);

extern	UINT f_dpda_checkStopave( UINT uiSetCalibItemNo );

extern	VOID f_dpda_alm_logCollect(USHORT usAlarmNum);

extern	UINT f_dpda_pa_on( UINT uiAntNum );

extern	UINT f_dpda_pa_on_slave_rre( UINT uiAntNum );

extern	UINT f_dpda_pa_off( UINT uiAntNum );
 	
extern	UINT f_dpda_pa_off_slave_rre( UINT uiAntNum );

extern int f_dpda_act_txon_request( void* msgP );
extern int f_dpda_act_rxon_request( void* msgP );
extern int f_dpda_act_txoff_request(VOID* msgP);
extern int f_dpda_act_rxoff_request(VOID* msgP);
extern INT f_dpda_act_tdden_ntc(VOID* rcvMsg_p);
extern INT f_dpda_act_mnt_txgainRequest(VOID* rcvMsg_p);
extern INT f_dpda_act_mnt_beamidSetReq_sub6(VOID* rcvMsg_p);
extern INT f_dpda_act_mnt_beamidSetReq_mmW(VOID* rcvMsg_p);
extern int f_dpda_act_txsleep_request(VOID* msgP);
extern int f_dpda_act_rxsleep_request(VOID* msgP);

extern	VOID f_dpda_com_setHighSpeedMode();
extern	VOID f_dpda_com_convLteBandwidth(UINT uiBandWidth, UINT* uiRegBandWidth);
extern	VOID f_dpda_com_getBwIndex(UINT uiBandWidth, UINT* uiBwIndex);
extern	VOID f_dpda_com_setAux( UINT txSysNum, UINT carNum, UINT* carBw );
extern	VOID f_dpda_com_setTxAntDelay( UINT uiAntNum, UINT uiCarNum, UINT uiSecChip );
extern	VOID f_dpda_com_auxDacEnable( UINT uiDacEn, UINT *uiAntState );
extern	VOID f_dpda_com_setDpdPhase( UINT uiAntNum );
extern	int f_dpda_com_setDpdStart(unsigned int uiAntNum , unsigned int failSafeFlg );
extern	int f_dpda_com_setDpdStop(unsigned int uiAntNum);
extern	VOID f_dpda_com_setDpdHighSpeedStart( UINT uiAntNum );
extern	VOID f_dpda_com_setDpdFailSafe(UINT uiAntNum);
extern	int f_dpda_com_setImpulseCoefficient( unsigned int uiAntNum, unsigned int uiNumberOfCar, unsigned int uiNco );
extern	INT f_dpda_com_TC_calculate(UINT TcCalcFlag, DL* Tc);
extern	INT f_dpda_com_IsCarrierOffOnAnt( UINT uiAntNum , UINT uiDirection );
extern	INT f_dpda_com_IsCarrierOffOnUnit( UINT uiAntNum , UINT uiDirection );
extern	INT f_dpda_com_IsCarrierOffOnSystem( UINT uiDirection );
extern	INT f_dpda_com_canTransmitWave( UINT uiAntNum );
extern	INT f_dpda_com_isTransmitWaveOnUnit( UINT uiAntNum );
extern  int f_dpda_com_setTddConfig(unsigned int uiUnitType);
extern  int f_dpda_com_TddAntEnDis(unsigned int uiCarrier, unsigned int uiAntNum, unsigned int uiUnitType, unsigned int uiEnDis);
extern	INT  f_dpda_com_isTddDl100Setting( UINT uiTddUlDlConfig, UINT uiTddSpeSubframeConfig, UINT uiTddDlCyclicPrefixLen );
extern	VOID f_dpda_com_TddAntEn( UINT uiAntNum, UINT uiEnDis, UINT uiLogLevel );
extern	VOID f_dpda_com_TddTxOn( UINT uiAntNum, UINT uiOnOff );
extern	VOID f_dpda_com_TddEn( UINT uiEnDis );

extern	VOID f_dpda_peri_calcRxPower( UINT  uiAntNum, INT* iExpPow);
//extern	VOID f_dpda_peri_calcRssi( UINT uiAntNum,UINT uiCarNum,INT *iRssi );
extern	VOID f_dpda_peri_calcFBPower( INT iAntPow, UINT uiPowSwRefVal, UINT uiPowSwFbVal, INT *iFbPow);

extern	UINT f_dpda_peri_checkPaCar(UINT uiAntNum);
extern	VOID f_dpda_peri_getHardRegInfo();

extern	VOID f_dpda_alm_factorJudge(UINT almJdgInfo1, UINT almJdgInfo2, UINT almOperator, UCHAR* almResult);
extern	VOID f_dpda_alm_logCollect(USHORT usAlarmNum);
extern	VOID f_dpda_alm_detect(VOID);
extern	VOID f_dpda_alm_infoCollect(VOID);
extern	VOID f_dpda_alm_dataMake(USHORT *usNumOfAlarmOccur, USHORT *usNumOfAlarmClear);
extern	VOID f_dpda_alm_stateUpdate(USHORT *usNumOfAlarmOccur, USHORT *usNumOfAlarmClear);
extern	VOID f_dpda_alm_report(USHORT usNumOfAlarmOccur, USHORT usNumOfAlarmClear);

extern	VOID f_dpda_car_TxcarEn(UINT uiAntNum, UINT uiCarNum, UINT uiOnOff);
extern	VOID f_dpda_car_RxcarEn(UINT uiAntNum, UINT uiCarNum, UINT uiOnOff);
extern	VOID f_dpda_car_TxAntEn(UINT uiAntNum, UINT uiOnOff);
extern	VOID f_dpda_car_RxAntEn(UINT uiAntNum, UINT uiOnOff);
extern	VOID f_dpda_car_TrxcarEn(UINT uiAntNum, UINT uiCarNum, UINT uiOnOff);
extern	UINT f_dpda_car_getCarCnt(UINT uiAntNum, UINT uiDirection);
extern	VOID f_dpda_car_setStrainParaTx_Peak( UINT uiAntNo );
extern	VOID f_dpda_car_setStrainParaTx_Peak_rre( UINT uiAntNo, UINT numOfTxCarrOn );
extern	VOID f_dpda_car_setRxCarDelay( UINT uiAntNum,  UINT uiCarNum, UINT uiBandWidth );
extern	VOID f_dpda_car_setLteBandwidth(UINT uiAntNum, UINT uiCarNum, UINT uiRcvTrs, UINT uiBandWidth);
extern	VOID f_dpda_car_setNcoCarConf(UINT uiAntNum, UINT uiCarNum, UINT uiRcvTrs, UINT uiFreq);
extern	VOID f_dpda_car_setRfPll(UINT uiFreqNo );
extern	int  f_dpda_car_lutUpdate(unsigned int uiAntNum);
extern	int  f_dpda_car_setTxBwandNco(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiNco, unsigned int uiBw, unsigned int uiUnitType);
extern	int  f_dpda_car_setRxBwandNco(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiNco, unsigned int uiBw, unsigned int uiUnitType);
extern	INT  f_dpda_car_calcTxNco(UINT uiFreqNum);
extern	INT  f_dpda_car_calcRxNco(UINT uiFreqNum);
extern	INT  f_dpda_car_calcTxNcoSettingValue(UINT uiFreqNum);
extern	INT  f_dpda_car_calcRxNcoSettingValue(UINT uiFreqNum);
extern	VOID f_dpda_car_setPmcTx( UINT uiAntNum, UINT uiOnOff );
extern	int  f_dpda_car_TxantcarStop(unsigned int uiAntNum);
extern	int  f_dpda_car_RxantcarStop(unsigned int uiAntNum);
extern  int  f_dpda_car_TxantcarStopOff(unsigned int uiAntNum);

extern	VOID f_dpda_ld_TxgainSetting( UINT tempIndex ,UINT uiAntNum , UINT uiSetCoeffNo);
extern	VOID f_dpda_ld_FbgainSetting( UINT tempIndex ,UINT uiAntNum );
extern	VOID f_dpda_ld_RxgainSetting( UINT tempIndex ,UINT uiAntNum );

extern	VOID f_dpda_peri_calcTssi(UINT uiAntNum, UINT uiCarNum, INT *iTssi);

extern	VOID f_dpda_peri_calcAntPower(UINT uiAntNum, INT *iAntPow);
extern	void f_dpda_peri_calcRtwp(unsigned int uiAntNum, unsigned int uiCarNum, int *iRtwp);
extern	void f_dpda_peri_calcVSWR( unsigned int uiAntNum,DL *RL , int* iVSWR);
extern	VOID f_dpda_peri_setTemperature();

extern	VOID f_dpda_peri_fastDpdMode();

extern	INT	f_dpda_act_sysParaSetRequest_mmW(VOID*);
extern	INT f_dpda_act_powReportPeriodic_mmW(VOID*);

extern	INT	f_dpda_act_APDWaveOnOffRequest_rre(VOID*);
extern	INT f_dpda_act_paControlRequest_rre(VOID*);
extern	INT	f_dpda_act_sendPowerAdjRequest_rre(VOID*);
extern	INT	f_dpda_act_sendPowerSaveRequest_rre(VOID*);

extern	VOID f_dpda_alm_report_mmW(USHORT usNumOfAlarmOccur, USHORT usNumOfAlarmClear);
extern	VOID f_dpda_com_branchConverter(UINT* branch);
extern	VOID f_dpda_com_setAux_rre( UINT txSysNum, UINT carNum, UINT* carBw );
extern	INT f_dpda_com_setFailSafe(UINT branch);
extern	VOID f_dpda_com_setRxAntDelay( UINT uiAntNum, UINT uiCarNum, UINT uiSecChip );
extern	VOID f_dpda_init_reg_tddsw_mmW();
extern	VOID f_dpda_init_reg_tddsw_sub6();



extern VOID	f_com_dpda_init(UCHAR dev_kind);
extern VOID	f_com_dpda_init_addCrc(USHORT dat, USHORT* p_crc);

extern VOID f_com_dpda_init_RFtrainingDataRead_Rx_mmW(VOID);
extern VOID f_com_dpda_init_RFtrainingDataRead_Tx_mmW(VOID);
extern VOID f_com_dpda_init_trainingDataRead_Rx_sub6(void);
extern VOID f_com_dpda_init_trainingDataRead_Tx_sub6(void);
extern VOID f_com_dpda_init_trainingDataSet_Tx_mmW(void);
extern VOID f_com_dpda_init_trainingDataSet_Tx_sub6(void);
extern int  f_dpda_car_TxcarEnDis(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, unsigned int uiEnDis);
extern int  f_dpda_car_RxcarEnDis(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, unsigned int uiEnDis);
extern int  f_dpda_car_setTxParameter(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiAccellId, unsigned int uiCcId, unsigned int uiDuPortId, unsigned int uiUnitType, unsigned int uiBw); 
extern int  f_dpda_car_setRxParameter(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiAccellId, unsigned int uiCcId, unsigned int uiDuPortId, unsigned int uiUnitType, unsigned int uiBw); 
extern int  f_dpda_car_setTxCalPhaseCorrection(unsigned int uiCarrier,unsigned int uiAntNum);
extern int  f_dpda_car_setRxCalPhaseCorrection(unsigned int uiCarrier,unsigned int uiAntNum);
extern int  f_dpda_car_setTxPhaseCompensationTable(unsigned int uiCarrier, unsigned int uiCrCentFreq, unsigned int uiScsn, unsigned int uiUnitType);
extern int  f_dpda_car_setRxPhaseCompensationTable(unsigned int uiCarrier, unsigned int uiCrCentFreq, unsigned int uiScsn, unsigned int uiUnitType);
extern int  f_dpda_car_setTxLPFGain(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, long long int Gain); 

extern INT f_dpda_txcal_startup( VOID );
extern INT f_dpda_rxcal_startup( VOID );
extern INT f_dpda_txcal_operation( UINT uiAntPairNum );
extern INT f_dpda_rxcal_operation( UINT uiAntPairNum );
extern INT f_dpda_txfastcal_operation( UINT uiAntPairNum );
extern INT f_dpda_rxfastcal_operation(UINT uiAntPairNum);

extern VOID f_dpda_peri_setQEC(VOID);
extern VOID f_dpda_com_clearQEC( VOID );

#endif
/* @} */
