/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_rom.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2013
 */
/******************************************************************************************************************************/

#ifndef	F_DPDA_ROM_LOCAL_H
#define	F_DPDA_ROM_LOCAL_H

#include "f_dpda_rom.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

extern const	UINT					f_dpdar_reg_Tbl[D_DPDA_REG_SUB6_NUM_MAX];
extern const	UINT					f_dpdar_reg_Tbl_mmW[D_DPDA_REG_MMW_NUM_MAX];
extern const	UINT					f_dpdar_virtualRegAddr[D_DPDA_DBG_VIRTUAL_REG_CNT];
extern const	UINT					f_dpdar_virtualRegData[D_DPDA_DBG_VIRTUAL_REG_CNT];

/* Txキャリア設定関連 */
extern const	DL						f_dpdar_TxGainAdjustCoeff[D_DPDA_GAIN_CONST_MAX];

/* 電力報告関連 */
extern const	UINT					f_dpdar_tssiManageTbl[E_SYS_RRHKIND_NUM][E_DPDA_BW_IDX_MAX][E_DPDA_SET_TSSI_MAX];
extern const	UINT					f_dpdar_antPowerBase[E_SYS_RRHKIND_NUM][E_DPDA_BW_IDX_MAX];

/* 無変調設定 基準振幅値テーブル */
extern const	UINT					f_dpdar_noModPowerBase[E_SYS_RRHKIND_NUM];
extern const	UINT					f_dpdar_noModPowerBase_z[E_SYS_RRHKIND_NUM];

/* 温度補償関連 */
extern const	T_DPDA_TEMP_ADJ_COEF	f_dpdar_tempAdjCoef[D_DPDA_TEMP_ADJ_COEF_NUM];

/* レジスタ初期値設定関連テーブル */
extern const    T_DPDA_INIT_REG_SET_TBL f_dpdar_init_reg_set_sub6[ D_DPDA_INIT_REG_NUM_SUB6 ];
extern const    T_DPDA_INIT_REG_SET_TBL f_dpdar_init_reg_set_mmW[ D_DPDA_INIT_REG_NUM_MMW ];
extern const    T_DPDA_INIT_REG_SET_TBL f_dpdar_init2_reg_set_sub6_45G[ D_DPDA_INIT2_REG_NUM_SUB6 ];
extern const    T_DPDA_INIT_REG_SET_TBL f_dpdar_init2_reg_set_sub6_37G[ D_DPDA_INIT2_REG_NUM_SUB6 ];
extern const    T_DPDA_INIT_REG_SET_TBL f_dpdar_init2_reg_set_mmW_28G[ D_DPDA_INIT2_REG_NUM_MMW ];

/* TDDコンフィグレーション関連テーブル */
extern const	UINT					f_dpdar_tddTdhdOffRegisterMap[ D_DPDA_ANTMAX ];
extern const	T_DPDA_REG_SET_TBL		f_dpdar_tdd_reg_set_common[ D_DPDA_TDD_COMMON_REG_NUM ];
extern const	T_DPDA_REG_SET_TBL		f_dpdar_tdd_reg_set_common_mmW[ D_DPDA_TDD_COMMON_REG_NUM_MMW ];

/* LUT設定値テーブル */
extern const	UINT					f_dpdar_lutValue[D_SYS_CAR_MAX][D_DPDA_LUT_MAX];
extern const	UINT					f_dpdar_lutValue_rre[D_SYS_CAR_MAX][D_DPDA_LUT_MAX];

/* インパルス係数テーブル */
extern const	T_DPDA_IMPULSE_COEFFICIENT	f_dpdar_impulseCoefficient[D_DPDA_IMPULSE_TABLE_TYPE_NUM][D_DPDA_IMPULSE_NUM];
extern const	T_DPDA_IMPULSE_COEFFICIENT	f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_NUM][D_DPDA_IMPULSE_NUM];

/* SVオペレーションテーブル */
extern const	UINT f_dpdar_svOptCarrierOnOff[ D_DPDA_ANTMAX ][ D_DPDA_CARMAX ];
extern const	UINT f_dpdar_svOptPaOnOff[ D_DPDA_ANTMAX ];

/* アラーム関連 */
extern const	T_DPDA_ALM_THRESHOLD_INFO	f_dpdar_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX];
extern const	T_DPDA_ALM_THRESHOLD_INFO	f_dpdar_almThreshold_45g[E_DPDA_ALM_THRESH_NUM_MAX];
extern const	T_DPDA_ALM_THRESHOLD_INFO	f_dpdar_almThreshold_mmW[E_DPDA_ALM_THRESH_NUM_MAX];
extern const	T_DPDA_ALM_THRESHOLD_INFO	*f_dpdar_almThreshold_list[E_DPDA_RUINDEX_NUM];
extern const	T_DPDA_ALM_INFO			f_dpdar_almInfo_sub6[E_DPDA_ALM_SUB6_NUM_MAX];
extern const	T_DPDA_ALM_INFO			f_dpdar_almInfo_mmW[E_DPDA_ALM_MMW_NUM_MAX];
extern const	UINT					f_dpdar_almJdgInfo[E_DPDA_ALM_JDG_INFO_MAX_NUM];
extern const	CHAR					f_dpdar_almJdgOperator[E_DPDA_ALM_OPE_NUM][7];
extern const	CHAR					f_dpdar_almMergeOperator[E_DPDA_ALM_FACTOR_OPE_NUM][9];
extern const	T_DPDA_ALM_INSTID_CNVT	f_dpdar_almInstIdCnvt_rre[ D_DPDA_ALM_INSTID_CNVT_NUM ];

 /* 5GDU_chg */
extern const	UCHAR	f_comr_trainingTx[D_DU_EEP_TRA_TXCNT];  /* sub6 EEPROM Tx デフォルト値 */
extern const	UCHAR	f_comr_trainingRx[D_DU_EEP_TRA_RXCNT];  /* sub6 EEPROM Rx デフォルト値 */
extern const	UCHAR	f_comr_RftrainingTx_mmW_NML[D_DU_RF_EEP_TRA_TXCNT_NML];          /* mmW RFEEPROM Tx デフォルト値 NMLモード */
extern const	UCHAR	f_comr_RftrainingRx_mmW_NML[D_DU_RF_EEP_TRA_RXCNT_NML];          /* mmW RFEEPROM Rx デフォルト値 NMLモード */
 /* 5GDU_chg */

extern T_DPDA_FFT_POINTNO_RB0  f_dpdar_fft_pointo_rb0_txon_mmW_100m[2][2];
extern T_DPDA_FFT_POINTNO_RB0  f_dpdar_fft_pointo_rb0_txon_mmW_200m[2][2];
extern T_DPDA_FFT_POINTNO_RB0  f_dpdar_fft_pointo_rb0_rxon_mmW_100m[2][6];
extern T_DPDA_FFT_POINTNO_RB0  f_dpdar_fft_pointo_rb0_rxon_mmW_200m[2][6];


/* CAL */
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_txantcal_reginit[D_DPDA_TXANTCAL_REGINIT_NUM];
extern const T_DPDA_ANTCAL_SET_TBL f_dpdar_txantcal_regsend[D_DPDA_TXANTCAL_SEND_NUM];
extern const T_DPDA_ANTCAL_PATHSET_TBL f_dpdar_txantcal_regpath[D_DPDA_TXANTCAL_PATH_NUM];
extern const T_DPDA_INIT_REG_SET_TBL f_dpdar_txantcal_regend[D_DPDA_TXANTCAL_END_NUM];
extern const T_DPDA_ORXCAL_TBL f_dpdar_orxcal_regp[D_DPDA_ORXCAL_NUM];
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_rxantcal_reginit[D_DPDA_RXANTCAL_REGINIT_NUM];
extern const T_DPDA_ANTCAL_SET_TBL f_dpdar_rxantcal_regsend[D_DPDA_RXANTCAL_SEND_NUM];
extern const T_DPDA_ANTCAL_PATHSET_TBL f_dpdar_rxantcal_regpath[D_DPDA_RXANTCAL_PATH_NUM];
extern const T_DPDA_INIT_REG_SET_TBL f_dpdar_rxantcal_regend[D_DPDA_RXANTCAL_END_NUM];

/* 運用中CAL */
extern const T_DPDA_ANTCAL_OPE_SEND_TBL  f_dpdar_txantcal_ope_regsend[D_DPDA_TXANTCAL_OPE_SEND_NUM];
extern const T_DPDA_ANTCAL_FAST_OPE_SEND_TBL  f_dpdar_txantfastcal_ope_regsend[D_DPDA_TXANTCAL_OPE_SEND_NUM];
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_txantcal_ope_regend[D_DPDA_TXANTCAL_OPE_END_NUM];
extern const T_DPDA_ANTCAL_PATHSET_TBL f_dpdar_txantcal_ope_regpath[D_DPDA_TXANTCAL_OPE_PATH_NUM];
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_txantcal_ope_regpath_ret[D_DPDA_TXANTCAL_OPE_PATH_RET_NUM];

extern const T_DPDA_ANTCAL_OPE_SEND_TBL  f_dpdar_rxantcal_ope_regsend[D_DPDA_RXANTCAL_OPE_SEND_NUM];
extern const T_DPDA_ANTCAL_FAST_OPE_SEND_TBL  f_dpdar_rxantfastcal_ope_regsend[D_DPDA_RXANTCAL_OPE_SEND_NUM];
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_rxantcal_ope_regend[D_DPDA_RXANTCAL_OPE_END_NUM];
extern const T_DPDA_ANTCAL_PATHSET_TBL f_dpdar_rxantcal_ope_regpath[D_DPDA_RXANTCAL_OPE_PATH_NUM];
extern const T_DPDA_INIT_REG_SET_TBL  f_dpdar_rxantcal_ope_regpath_ret[D_DPDA_RXANTCAL_OPE_PATH_RET_NUM];

/*	CAL	*/
extern const T_DPDA_CAL_POLARIZATION_INFO	f_dpdar_polarizationInf[D_RRH_POLARIZATION_NUM];
#endif	/* F_DPDA_ROM_LOCAL_H */
/** @} */
